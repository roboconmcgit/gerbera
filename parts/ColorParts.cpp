/**
 ******************************************************************************
 ** ファイル名 : ColorParts.cpp
 **
 ** 概要 : カラーセンサパーツクラス
 **
 ** 注記 : 各種初期パラメータは_prmファイル参照
 ******************************************************************************
 **/

#include "ColorParts.h"
#include "ColorParts_prm.h"
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
ColorParts::ColorParts(){
	memset(&ColorParts_Calib, 0, sizeof(COLORPARTS_CALIBRA));
	memset(&Fillter_Data[0], 0, sizeof(COLOR_FILLTER_NUM));
	
	ColorParts_State = false;
    colorSensor  = new ColorSensor(COLORPARTS_CH);
}

//*****************************************************************************
// 関数名 : デストラクタ
// 引数 : unused
// 返り値 : なし
// 概要 : 
//*****************************************************************************
ColorParts::~ColorParts(){
	delete colorSensor;
}

//*****************************************************************************
// 関数名 : ColorPartsCalibra
// 引数 : ColorParts_CALIBRA s_calibra
// 返り値 : なし
// 概要 : キャリブレーションを設定する
//*****************************************************************************
void ColorParts::ColorPartsCalibra(COLORPARTS_CALIBRA s_calibra){
	memcpy(&ColorParts_Calib, &s_calibra, sizeof(COLORPARTS_CALIBRA));
	ColorParts_Calib.White_Data = LIGHT_WHITE;
	ColorParts_Calib.Black_Data = LIGHT_BLACK;
	LineEdge_Data = (ColorParts_Calib.White_Data + ColorParts_Calib.Black_Data)/2;
}

//*****************************************************************************
// 関数名 : ColorPartsReset
// 引数 : unused
// 返り値 : なし
// 概要 : カラーセンサパーツをリセットする
//*****************************************************************************
void ColorParts::ColorPartsReset(){
	int set = GetColorPartsData();
	memset(&Fillter_Data[0], set, sizeof(COLOR_FILLTER_NUM));
}

//*****************************************************************************
// 関数名 : ColorPartsTask
// 引数 : unused
// 返り値 : なし
// 概要 : フィルタリング用の定期処理
//*****************************************************************************
void ColorParts::ColorPartsTask(){
	//定期的にデータ取得
	for(int i = 0;i < COLOR_FILLTER_NUM - 1; i++){
		Fillter_Data[i+1] = Fillter_Data[i];
	}
	Fillter_Data[0] = GetColorPartsData();
}

//*****************************************************************************
// 関数名 : GetColorPartsData
// 引数 : unused
// 返り値 : 現在のカラーセンサ値
// 概要 : カラーセンサセンサの現在値を取得する
//*****************************************************************************
int8_t ColorParts::GetColorPartsData(){
	int8_t ret = colorSensor->getBrightness();
	return(ret + ColorParts_Calib.Offset);
}

//*****************************************************************************
// 関数名 : GetColorPartsFillter
// 引数 : unused
// 返り値 : フィルタリングしたカラーセンサ値
// 概要 : カラーセンサ値をフィルタリングした値を取得する
//*****************************************************************************
int8_t ColorParts::GetColorPartsFillter(){
	int16_t sum = 0;
	int8_t ret;	
	for(int i = 0;i < COLOR_FILLTER_NUM; i++){
		sum += Fillter_Data[i];
	}
	ret = (int8_t)(sum / COLOR_FILLTER_NUM);
	return(ret);
}

//*****************************************************************************
// 関数名 : GetLineDetection
// 引数 : unused
// 返り値 : ライン検知有無
// 概要 : ラインを検知を通知する
//*****************************************************************************
bool ColorParts::GetLineDetection(){
	bool ret = false;
    if(GetColorPartsFillter() >= LineEdge_Data){
		ret = true;
	}
	return(ret);
}
