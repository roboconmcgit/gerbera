/**
 ******************************************************************************
 ** ファイル名 : GyroParts.cpp
 **
 ** 概要 : ジャイロパーツクラス
 **
 ** 注記 : 各種初期パラメータは_prmファイル参照
 ******************************************************************************
 **/

#include "GyroParts.h"
#include "GyroParts_prm.h"
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
GyroParts::GyroParts(){
	memset(&GyroParts_Calib, 0, sizeof(GYROPARTS_CALIBRA));
	GyroParts_State = false;
    gyroSensor  = new GyroSensor(GYRO_CH);
}

//*****************************************************************************
// 関数名 : デストラクタ
// 引数 : unused
// 返り値 : なし
// 概要 : 
//*****************************************************************************
GyroParts::~GyroParts(){
	delete gyroSensor;
}

//*****************************************************************************
// 関数名 : GyroPartsCalibra
// 引数 : GyroParts_CALIBRA s_calibra
// 返り値 : なし
// 概要 : 全てのモータのキャリブレーションを設定する
//*****************************************************************************
void GyroParts::GyroPartsCalibra(GYROPARTS_CALIBRA s_calibra){
	memcpy(&GyroParts_Calib, &s_calibra, sizeof(GYROPARTS_CALIBRA));
}

//*****************************************************************************
// 関数名 : GyroPartsCalibra
// 引数 : GyroParts_CALIBRA s_calibra
// 返り値 : なし
// 概要 : 全てのモータのキャリブレーションを設定する
//*****************************************************************************
void GyroParts::GyroPartsReset(){
	gyroSensor->reset();
	//gyroSensor->getAnglerVelocity();
}

//*****************************************************************************
// 関数名 : GetGyroPartsData
// 引数 : unused
// 返り値 : なし
// 概要 : ジャイロセンサの現在値を取得する
//*****************************************************************************
int32_t GyroParts::GetGyroPartsData(){
	int32_t ret = gyroSensor->getAnglerVelocity();
	return(ret + GyroParts_Calib.Offset);
}
