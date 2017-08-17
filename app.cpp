/**
 ******************************************************************************
 ** ファイル名 : app.cpp
 **
 ** 概要 : 2輪倒立振子ライントレースロボットのTOPPERS/HRP2用C++サンプルプログラム
 **
 ** 注記 : sample_cpp (ライントレース/尻尾モータ/超音波センサ/リモートスタート)
 ******************************************************************************
 **/

#include "ev3api.h"

#include "ClockTimer.h"
#include "lcdDraw.h"
#include "logging.h"
#include "PartsAll.h"

#include "Controller.h"
#include "calibration.h"

#include "balancer.h"

#include "app.h"
#include "app_setting.h"

using namespace ev3api;

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/* ライントレース用変数 */
static int8_t trunState[2];     // 0:現在の状態、1:変化前の状態
static int8_t leftRun_count;
static int8_t turn_count;

static uint16_t u_4msTask_Counter;
#define U4MSTASK_COUNTER_MAX (60000)

static bool waitFlag;
static bool waitFlag_old;

inline void StateSet( int8_t a) {
    trunState[1] = trunState[0];
    trunState[0] = a;
}

/* 関数プロトタイプ宣言 */
static void InitializeFunc(void);
static void FinalizeFunc(void);

static void controller_temp(void);

/* オブジェクトへのポインタ定義 */
Controller*     pC_controller;
GyroParts*      pC_gyroParts;
MotorParts*     pC_motorParts;
ColorParts*     pC_colorParts;
SonarParts*     pC_sonarParts;
TouchParts*     pC_touchParts;
Logging*        pC_logging;
lcdDraw*        pC_lcdDraw;
ClockTimer*     pC_clockTimer;

PARTS_ALL      tag_partsAll;

/* メインタスク */
void main_task(intptr_t unused)
{
    /* 各オブジェクトを生成・初期化する */
    pC_clockTimer = new ClockTimer();
    pC_lcdDraw = new lcdDraw();
    pC_motorParts  = new MotorParts(pC_lcdDraw);
    pC_gyroParts = new GyroParts();
    pC_colorParts = new ColorParts();
    pC_sonarParts = new SonarParts();
    pC_touchParts = new TouchParts();
    pC_logging     = new Logging();

    tag_partsAll.tag_pC_gyroParts = pC_gyroParts;
    tag_partsAll.tag_pC_motorParts = pC_motorParts;
    tag_partsAll.tag_pC_colorParts = pC_colorParts;
    tag_partsAll.tag_pC_sonarParts = pC_sonarParts;
    tag_partsAll.tag_pC_touchParts = pC_touchParts;
    pC_controller  = new Controller(&tag_partsAll);

    InitializeFunc();
    /**
    * Main loop for the self-balance control algorithm
    */
    //4msTask起動
    pC_lcdDraw->LCD_DRAW((char*)"4ms Start1");
    ev3_sta_cyc(CYC_PRD_TSK_1);

    while(1)
    {
        //ボタンが押されたら停止
        if (ev3_button_is_pressed(BACK_BUTTON)) break;

        pC_clockTimer->sleep(100);
    }

    //4msTask停止
    pC_lcdDraw->LCD_DRAW((char*)"4ms stop");
    ev3_stp_cyc(CYC_PRD_TSK_1);
    FinalizeFunc();
    ext_tsk();
}

//*****************************************************************************
// 関数名 : InitializeFunc
// 引数 : parameterNo キャリブレーションNO
// 返り値 : 無し
// 概要 : 初期化処理
//*****************************************************************************
static void InitializeFunc(void) {
    calibration calib;

    trunState[0] = TURN_STRAIGHT;
    trunState[1] = TURN_STRAIGHT;
    leftRun_count = 0;
    turn_count = 0;

    u_4msTask_Counter = 0;
    waitFlag = true;
    waitFlag_old = waitFlag;

    /* LCD画面表示 */
    pC_lcdDraw->LCD_INIT();
    pC_lcdDraw->LCD_DRAW((char*)"EV3way-ET sample_cpp");

    /* モーターのリセット */
    pC_motorParts->MotorPartsReset(MOTORPARTS_TAIL_BIT);
    
    calib.set_calibration(pC_motorParts);
    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */

    
    pC_lcdDraw->LCD_DRAW((char*)"Start wait");
    /* スタート待機 */
    while(1)
    {
        pC_motorParts->setMotorPartsTail(TAIL_STAND_UP); /* 完全停止用角度に制御 */
        if (pC_touchParts->GetTouchPartsData())
        {
            break; /* タッチセンサが押された */
        }

        pC_clockTimer->sleep(10);
    }

    /* 走行モーターエンコーダーリセット */
    pC_motorParts->MotorPartsReset(MOTORPARTS_RIGHT_BIT&MOTORPARTS_TAIL_BIT);
    
    /* ジャイロセンサーリセット */
    pC_gyroParts->GyroPartsReset();
    balance_init(); /* 倒立振子API初期化 */
    
//    pC_clockTimer->sleep(1000);

    ev3_led_set_color(LED_GREEN); /* スタート通知 */
    pC_lcdDraw->LCD_DRAW((char*)"InitializeFunc end");
}

//*****************************************************************************
// 関数名 : FinalizeFunc
// 引数 : 無し
// 返り値 : 無し
// 概要 : 初期化処理
//*****************************************************************************
static void FinalizeFunc(void){
    pC_motorParts->stopMotorPartsLeftRight();
    pC_motorParts->MotorPartsReset(MOTORPARTS_RIGHT_BIT&MOTORPARTS_TAIL_BIT);

    // Logging書き込み
    if(pC_logging->LogCount()!=0) pC_logging->file_save();   //LOGを取っていれば保存

    pC_clockTimer->sleep(1000);
    pC_lcdDraw->LCD_DRAW((char*)"FinalizeFunc end");

    delete pC_lcdDraw;
    delete pC_clockTimer;
    delete pC_motorParts;
    delete pC_gyroParts;
    delete pC_colorParts;
    delete pC_sonarParts;
    delete pC_touchParts;
    delete pC_logging;

}

//*****************************************************************************
// 関数名 : periodic_4mstask
// 引数 : 無し
// 返り値 : 無し
// 概要 : 4msPeriodicTask
//*****************************************************************************
void periodic_4mstask(intptr_t unused) {
    int8_t tail_mode = -1;
    
    tail_mode = TAIL_DRIVE;

#if 0 // ここはまだ整備中　きれいになり次第、最終的にはこっちを使う
    /* タッチセンサが押された */
    if (pC_touchParts->GetTouchPartsData())
    {
        waitFlag = !waitFlag;   //反転する
    }
    //待機処理
    if(waitFlag == true){
#if 0
        //待機後の初回動作
        if(waitFlag != waitFlag_old){
            pC_lcdDraw->LCD_DRAW((char*)"Waitting...");
            ev3_led_set_color(LED_ORANGE); /* 待機通知 */
            pC_lcdDraw->LCD_DRAW((char*)"Start wait");
            tail_mode = TAIL_STAND_UP; /* 完全停止用角度に制御 */
        }
        /* 待機中 */
#endif

    //待機解除
    }else{
        //待機解除後の初回動作
        if(waitFlag != waitFlag_old){
#if 0
            pC_lcdDraw->LCD_DRAW((char*)"Start!");
            /* 走行モーターエンコーダーリセット */
            pC_motorParts->MotorPartsReset(MOTORPARTS_RIGHT_BIT&MOTORPARTS_TAIL_BIT);
            pC_motorParts->stopMotorPartsLeftRight();
            /* ジャイロセンサーリセット */
            pC_gyroParts->GyroPartsReset();
            balance_init(); /* 倒立振子API初期化 */
            
        //    pC_clockTimer->sleep(1000);
            tail_mode = TAIL_DRIVE;   /* バランス走行用角度に制御 */

            ev3_led_set_color(LED_GREEN); /* スタート通知 */
#endif

        }

#if 0

        /*  4msタスク  */
        pC_colorParts->ColorPartsTask();

        controller_temp();

        
        /*  40msタスク  */
        if((u_4msTask_Counter%10) == 0){
            //ソナーセンサパーツフィルタリング用処理
            pC_sonarParts->SonarPartsTask();    //40ms間隔で呼び出す
        }
#endif
        
    }
#endif


    /*  4msタスク  */
    //カラーセンサパーツフィルタリング用処理
    pC_colorParts->ColorPartsTask();

    controller_temp();

    /*  20msタスク  */
    if((u_4msTask_Counter%5) == 0){
        pC_motorParts->setMotorPartsTail(tail_mode); /* テール角度制御 */
    }

    /*  40msタスク  */
    if((u_4msTask_Counter%10) == 0){
        //ソナーセンサパーツフィルタリング用処理
        pC_sonarParts->SonarPartsTask();    //40ms間隔で呼び出す
    }

    /*  100msタスク  */
    if((u_4msTask_Counter%25) == 0){
        pC_touchParts->TouchPartsTask();
    }


    waitFlag_old = waitFlag;

    // 4msタスクの監視カウンタ
    u_4msTask_Counter++;
    if(u_4msTask_Counter >= U4MSTASK_COUNTER_MAX) u_4msTask_Counter = 0;
}

//*****************************************************************************
// 関数名 : controller_temp
// 引数 : 無し
// 返り値 : 無し
// 概要 : 一時退避
//*****************************************************************************
static void controller_temp(void){

    int8_t forward = FORWORD_STOP;      /* 前後進命令 */
    int8_t turn = 0;         /* 旋回命令 */
    int8_t pwm_L = 0, pwm_R = 0; /* 左右モータPWM出力 */

    int32_t motor_ang_l, motor_ang_r;
    int32_t gyro, volt;
#if 1
    if (pC_sonarParts->GetObstacleDetection() == 1) /* 障害物検知 */
    {
        forward = FORWORD_STOP;
        turn = 0; /* 障害物を検知したら停止 */
    }
    else
    {
    	forward = FORWORD_NOMAL; /* 前進命令 */


        // 状態による向き制御の選択
        switch(trunState[1]){
            case TURN_STRAIGHT:
                leftRun_count = 0;
                break;
            case TURN_LEFT:
                // 目標距離到達検知
                if(leftRun_count >= LEFTRUN_MAX){
                    leftRun_count = 0;
                    StateSet(TURN_RIGHT);
                }
                leftRun_count++;
                break;
            case TURN_RIGHT:
                //ライン検知：
                if(pC_colorParts->GetLineDetection() == true){
                    StateSet(TURN_LEFT);
                }
                //ライン検知　ここまで
                break;
            default:
                leftRun_count = 0;
                trunState[0] = TURN_STRAIGHT;
                trunState[1] = TURN_STRAIGHT;
                break;
        }

        // 向き調整
        switch(trunState[0]){
            case TURN_STRAIGHT:
                turn = 0;
                turn_count = 0;
                break;
            case TURN_LEFT:
    			turn =  LINETRACE_TURN_MAX; // 左旋回命令 
                if(turn_count >= TURN_WAIT_TIME){
                    StateSet(TURN_STRAIGHT);
                }
                turn_count++;
                break;
            case TURN_RIGHT:
    			turn = (-1)*LINETRACE_TURN_MAX; // 右旋回命令
                if(turn_count >= TURN_WAIT_TIME){
                    StateSet(TURN_STRAIGHT);
                }
                turn_count++;
                break;
            default:
                turn = 0;
                turn_count = 0;
                StateSet(TURN_STRAIGHT);
                break;
        }
    }
#else
    forward = 0;//FORWORD_NOMAL; /* 前進命令 */
    turn = 0;

#endif
    /* 倒立振子制御API に渡すパラメータを取得する */
    motor_ang_l = pC_motorParts->getMotorPartsPwm(MOTORPARTS_LEFT_NO);
    pC_lcdDraw->LCD_DRAW_DATA_INT32((char*)"motor_ang_l", (int32_t)motor_ang_l, 12);

    motor_ang_r = pC_motorParts->getMotorPartsPwm(MOTORPARTS_RIGHT_NO);
    pC_lcdDraw->LCD_DRAW_DATA_INT32((char*)"motor_ang_r", (int32_t)motor_ang_r, 13);

    gyro = pC_gyroParts->GetGyroPartsData();
    pC_lcdDraw->LCD_DRAW_DATA_INT32((char*)"gyro", (int32_t)gyro, 14);

    volt = ev3_battery_voltage_mV();
    pC_lcdDraw->LCD_DRAW_DATA_INT32((char*)"volt", (int32_t)volt, 15);

    /* 倒立振子制御APIを呼び出し、倒立走行するための */
    /* 左右モータ出力値を得る */
    balance_control(
        (float)forward,             /* 前進最高速命令 */
        (float)turn,                /* 無旋回命令 */
        (float)gyro,                /* ジャイロセンサ値 */
        (float)GYRO_OFFSET,         /* 車体停止時のジャイロセンサ値 */
        (float)motor_ang_l,         /* 左モータエンコーダ値 */
        (float)motor_ang_r,         /* 右モータエンコーダ値 */
        (float)volt,                /* バッテリ電圧値(mV) */
        (int8_t *)&pwm_L,           /* 左モータPWM出力値 */
        (int8_t *)&pwm_R);          /* 右モータPWM出力値 */

    //pC_motorParts->setMotorPartsLeftRight(pwm_R,pwm_L);

    //pC_lcdDraw->LCD_DRAW_DATA_FLOAT((char*)"pwm_R", (float)pwm_R);
    //pC_lcdDraw->LCD_DRAW_DATA_FLOAT((char*)"pwm_L", (float)pwm_L);
    //pC_lcdDraw->LCD_DRAW((char*)"periodic_4mstask end");
}

//*****************************************************************************
// 関数名 : task_activator
// 引数 : 無し
// 返り値 : 無し
// 概要 : a cyclic handler to activate a task
//*****************************************************************************
void task_activator(intptr_t tskid) {
    ER ercd = act_tsk(tskid);
    assert(ercd == E_OK);
}
