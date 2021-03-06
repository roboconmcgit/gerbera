/**
 ******************************************************************************
 ** ファイル名 : SonarParts.cpp
 **
 ** 概要 : ソナーセンサパーツクラス
 **
 ** 注記 : 各種初期パラメータは_prmファイル参照
 ******************************************************************************
 **/

#include "SonarParts.h"
#include "SonarParts_prm.h"
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
SonarParts::SonarParts(){
	memset(&SonarParts_Calib, 0, sizeof(SONARPARTS_CALIBRA));
	memset(&Fillter_Data[0], 0, sizeof(SONAR_FILLTER_NUM));
	
	SonarParts_State = false;
    sonarSensor  = new SonarSensor(SONARPARTS_CH);
}

//*****************************************************************************
// 関数名 : デストラクタ
// 引数 : unused
// 返り値 : なし
// 概要 : 
//*****************************************************************************
SonarParts::~SonarParts(){
	delete sonarSensor;
}

//*****************************************************************************
// 関数名 : SonarPartsCalibra
// 引数 : SonarParts_CALIBRA s_calibra
// 返り値 : なし
// 概要 : キャリブレーションを設定する
//*****************************************************************************
void SonarParts::SonarPartsCalibra(SONARPARTS_CALIBRA s_calibra){
	memcpy(&SonarParts_Calib, &s_calibra, sizeof(SONARPARTS_CALIBRA));
}

//*****************************************************************************
// 関数名 : SonarPartsReset
// 引数 : unused
// 返り値 : なし
// 概要 : ソナーセンサパーツをリセットする
//*****************************************************************************
void SonarParts::SonarPartsReset(){
	int set = GetSonarPartsData();
	SonarParts_State = false;
	memset(&Fillter_Data[0], set, sizeof(SONAR_FILLTER_NUM));
}

//*****************************************************************************
// 関数名 : SonarPartsTask
// 引数 : unused
// 返り値 : なし
// 概要 : フィルタリング用の定期処理
//*****************************************************************************
void SonarParts::SonarPartsTask(){
	//定期的にデータ取得
	for(int i = 0;i < SONAR_FILLTER_NUM - 1; i++){
		Fillter_Data[i+1] = Fillter_Data[i];
	}
	Fillter_Data[0] = GetSonarPartsData();
}

//*****************************************************************************
// 関数名 : GetSonarPartsData
// 引数 : unused
// 返り値 : 現在のソナーセンサ値
// 概要 : ソナーセンサセンサの現在値を取得する
//*****************************************************************************
int32_t SonarParts::GetSonarPartsData(){
	int32_t ret = sonarSensor->getDistance();
	return(ret + SonarParts_Calib.Offset);
}

//*****************************************************************************
// 関数名 : GetSonarPartsFillter
// 引数 : unused
// 返り値 : フィルタリングしたソナーセンサ値
// 概要 : ソナーセンサ値をフィルタリングした値を取得する
//*****************************************************************************
int32_t SonarParts::GetSonarPartsFillter(){
	double sum = 0;
	int32_t ret;
	for(int i = 0;i < SONAR_FILLTER_NUM; i++){
		sum += Fillter_Data[i];
	}
	ret = (int32_t)(sum / SONAR_FILLTER_NUM);
	return(ret);
}

//*****************************************************************************
// 関数名 : GetObstacleDetection
// 引数 : 無し
// 返り値 : 1(障害物あり)/0(障害物無し)
// 概要 : 超音波センサによる障害物検知
//*****************************************************************************
bool SonarParts::GetObstacleDetection(void)
{
    bool ret = false;
    int32_t distance = GetSonarPartsFillter();

	if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
	{
		ret = true; /* 障害物を検知 */
	}
    return (ret);
}