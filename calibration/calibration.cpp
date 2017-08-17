/**
 ******************************************************************************
 ** ファイル名 : calibration.cpp
 **
 ** 概要 : キャリブレーションクラス
 **
 ** 注記 : 各種初期パラメータは_prmファイル参照
 ******************************************************************************
 **/

#include "calibration.h"
#include "calibration_prm.h"
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
calibration::calibration(){

}

//*****************************************************************************
// 関数名 : デストラクタ
// 引数 : unused
// 返り値 : なし
// 概要 : 
//*****************************************************************************
calibration::~calibration(){

}

//*****************************************************************************
// 関数名 : calibrationCalibra
// 引数 : calibration_CALIBRA s_calibra
// 返り値 : なし
// 概要 : キャリブレーションを設定する
//*****************************************************************************
void calibration::set_calibration(MotorParts* motorParts){
	MOTORPARTS_CALIBRA motorparts_calib;
	memset((void *)motorparts_calib.Offset, 0, MOTORPARTS_NUM);
	motorParts->MotorPartsCalibra(motorparts_calib);
}
