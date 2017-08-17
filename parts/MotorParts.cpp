/**
 ******************************************************************************
 ** ファイル名 : MotorParts.cpp
 **
 ** 概要 : モーターバーツクラス
 **
 ** 注記 : 各種初期パラメータは_prmファイル参照
 ******************************************************************************
 **/

#include "MotorParts.h"
#include "MotorParts_prm.h"
#include <string.h>

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

//*****************************************************************************
// 関数名 : コンストラクタ
// 引数 : unused
// 返り値 : なし
// 概要 : 
//*****************************************************************************
MotorParts::MotorParts(lcdDraw *lcdDraw){
	memset((void *)&MotorParts_Calib, 0, sizeof(MOTORPARTS_CALIBRA));
	memset((void *)&MotorParts_State, 0, MOTORPARTS_NUM);
	
    leftMotor   = new Motor(LEFT_MOTOR_CH);
    rightMotor  = new Motor(RIGHT_MOTOR_CH);
    tailMotor   = new Motor(TAIL_MOTOR_CH);

	pC_lcdDraw = lcdDraw;
}

//*****************************************************************************
// 関数名 : デストラクタ
// 引数 : unused
// 返り値 : なし
// 概要 : 
//*****************************************************************************
MotorParts::~MotorParts(){
	delete leftMotor;
	delete rightMotor;
	delete tailMotor;
	delete pC_lcdDraw;
}

//*****************************************************************************
// 関数名 : MotorPartsCalibra
// 引数 : MOTORPARTS_CALIBRA s_calibra
// 返り値 : なし
// 概要 : 全てのモータのキャリブレーションを設定する
//*****************************************************************************
void MotorParts::MotorPartsCalibra(MOTORPARTS_CALIBRA s_calibra){
	memcpy(&MotorParts_Calib, &s_calibra, sizeof(MOTORPARTS_CALIBRA));
}

//*****************************************************************************
// 関数名 : MotorPartsCalibra
// 引数 : int8_t motorBit
// 返り値 : なし
// 概要 : 指定したモータをリセットする
//*****************************************************************************
void MotorParts::MotorPartsReset(int8_t motorBit){
	if((motorBit&MOTORPARTS_LEFT_BIT)>0){
		leftMotor->reset();
    	pC_lcdDraw->LCD_DRAW((char*)"leftreset");
	}
	if((motorBit&MOTORPARTS_RIGHT_BIT)>0){
		rightMotor->reset();
    	pC_lcdDraw->LCD_DRAW((char*)"rightreset");
	}
	if((motorBit&MOTORPARTS_TAIL_BIT)>0){
		tailMotor->reset();
    	pC_lcdDraw->LCD_DRAW((char*)"tailreset");
	}
}

//*****************************************************************************
// 関数名 : getMotorPartsPwm
// 引数 : int8_t motorNo
// 返り値 : なし
// 概要 : 指定したモータの現在値を取得する
//*****************************************************************************
int32_t MotorParts::getMotorPartsPwm(int8_t motorNo){
	int32_t ret = 0;
	switch(motorNo){
		case MOTORPARTS_LEFT_NO:
			ret = leftMotor->getCount();
			//pC_lcdDraw->LCD_DRAW_DATA_INT32((char*)"LEFT_NO", (int32_t)ret, 12);
			break;
		case MOTORPARTS_RIGHT_NO:
			ret = rightMotor->getCount();
			//pC_lcdDraw->LCD_DRAW_DATA_INT32((char*)"RIGHT_NO", (int32_t)ret, 13);
			break;
		case MOTORPARTS_TAIL_NO:
			ret = tailMotor->getCount();
			//pC_lcdDraw->LCD_DRAW_DATA_INT32((char*)"TAIL_NO", (int32_t)ret, 14);
			break;
		default:
			break;
	}

	return(ret);
}

//*****************************************************************************
// 関数名 : setMotorPartsLeftRight
// 引数 : int rightMotorPwm, int leftMotorPwm
// 返り値 : なし
// 概要 : 左右モータを動かす
//*****************************************************************************
void MotorParts::setMotorPartsLeftRight(int rightMotorPwm, int leftMotorPwm){
	int pwm;
	pwm = rightMotorPwm + MotorParts_Calib.Offset[MOTORPARTS_RIGHT_NO];
    rightMotor->setPWM(pwm);
	pwm = leftMotorPwm + MotorParts_Calib.Offset[MOTORPARTS_LEFT_NO];
    leftMotor->setPWM(pwm);
}

//*****************************************************************************
// 関数名 : stopMotorPartsLeftRight
// 引数 : unused
// 返り値 : なし
// 概要 : 左右モータを止める
//*****************************************************************************
void MotorParts::stopMotorPartsLeftRight(void){
    rightMotor->stop();
    leftMotor->stop();
}

//*****************************************************************************
// 関数名 : setMotorPartsTail
// 引数 : int8_t tailMove
// 返り値 : なし
// 概要 : テールモータを動かす
//*****************************************************************************
void MotorParts::setMotorPartsTail(int8_t tailMove){
	int32_t angle = 0;
	float pwm = 0;

	switch(tailMove){
		case TAIL_RESET:
			angle = 0;
		break;
		case TAIL_STAND_UP:
			angle = TAIL_ANGLE_STAND_UP;
			break;
		case TAIL_DRIVE:
			angle = TAIL_ANGLE_DRIVE;
			break;
		default:
			break;
	}

    pwm = (float)(angle - tailMotor->getCount()) * P_GAIN; /* 比例制御 */
    /* PWM出力飽和処理 */
    if (pwm > PWM_ABS_MAX)
    {
        pwm = PWM_ABS_MAX;
    }
    else if (pwm < -PWM_ABS_MAX)
    {
        pwm = -PWM_ABS_MAX;
    }
    tailMotor->setPWM(pwm);
}
