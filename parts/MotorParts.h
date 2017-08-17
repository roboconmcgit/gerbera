/*
 *      モーターバーツクラスのヘッダファイル
 */

#ifndef MOTORPARTS_H_
#define MOTORPARTS_H_
#include "ev3api.h"
#include "Motor.h"
#include "lcdDraw.h"

using namespace ev3api;

// モータ種別　番号
enum{
    MOTORPARTS_LEFT_NO = 0,
    MOTORPARTS_RIGHT_NO,
    MOTORPARTS_TAIL_NO,
    MOTORPARTS_NUM
};

// モータ種別　Bit位置
#define MOTORPARTS_LEFT_BIT  (0x01<<MOTORPARTS_LEFT_NO)
#define MOTORPARTS_RIGHT_BIT (0x01<<MOTORPARTS_RIGHT_NO)
#define MOTORPARTS_TAIL_BIT  (0x01<<MOTORPARTS_TAIL_NO)

// テール動作モード
enum{
    TAIL_RESET = 0,
    TAIL_STAND_UP,
    TAIL_DRIVE
};

// キャリブレーションパラメータ
typedef struct MotorParts_Calibra{
    int32_t Offset[MOTORPARTS_NUM];
}MOTORPARTS_CALIBRA;

/*
 *  関数のプロトタイプ宣言
 */
 
class MotorParts{
private:
    MOTORPARTS_CALIBRA MotorParts_Calib;          //キャリブレーションデータ
    bool MotorParts_State[MOTORPARTS_NUM];        //状態フラグ　0:正常、1:以上

//protected:
    Motor          *leftMotor;               //左モータパーツ用のモータクラス
    Motor          *rightMotor;              //右モータパーツ用のモータクラス
    Motor          *tailMotor;               //テールモータパーツ用のモータクラス

    lcdDraw          *pC_lcdDraw;               //LCD描画クラス

public:
    MotorParts(lcdDraw *lcdDraw);                       //コンストラクタ
    ~MotorParts();                      //デストラクタ
    void MotorPartsCalibra(MOTORPARTS_CALIBRA s_calibra);
                                        //全てのモータのキャリブレーションを設定する
    void MotorPartsReset(int8_t motorBit);
                                        //指定したモータをリセットする
    int32_t getMotorPartsPwm(int8_t motorNo);
                                        //指定したモータの現在値を取得する
    void setMotorPartsLeftRight(int rightMotorPwm, int leftMotorPwm);
                                        //左右モータを動かす
    void stopMotorPartsLeftRight(void);
                                        //左右モータを止める
    void setMotorPartsTail(int8_t tailMove);
                                        //テールモータを動かす

};

#endif // !MOTORPARTS_H_
