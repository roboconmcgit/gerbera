/*
 *      カラーセンサバーツクラスのヘッダファイル
 */

#ifndef COLORPARTS_H_
#define COLORPARTS_H_
#include "ev3api.h"
#include "ColorSensor.h"

using namespace ev3api;

// キャリブレーションパラメータ
typedef struct ColorParts_Calibra{
    int32_t Offset;
    int8_t White_Data;
    int8_t Black_Data;
}COLORPARTS_CALIBRA;

#define COLOR_FILLTER_NUM           (5)  //フィルタ回数

/*
 *  関数のプロトタイプ宣言
 */
 
class ColorParts{
private:
    COLORPARTS_CALIBRA ColorParts_Calib;    //キャリブレーションデータ
    bool ColorParts_State;                  //状態フラグ　0:正常、1:以上
    int8_t LineEdge_Data;                   //ライン際の色情報
    int8_t Fillter_Data[COLOR_FILLTER_NUM];                  //フィルタ用格納データ

//protected:
    ColorSensor*     colorSensor;              //ColorParts用のクラス

public:
    ColorParts();                       //コンストラクタ
    ~ColorParts();                      //デストラクタ
    void ColorPartsCalibra(COLORPARTS_CALIBRA s_calibra);
                                        //キャリブレーションを設定する
    void ColorPartsReset();
                                        //カラーセンサパーツをリセットする
    void ColorPartsTask();
                                        //フィルタリング用の定期処理
    int8_t GetColorPartsData();
                                        //カラーセンサセンサの現在値を取得する
    int8_t GetColorPartsFillter();
                                        //カラーセンサ値をフィルタリングした値を取得する
    bool GetLineDetection();
                                        //ラインを検知を通知する
};

#endif // !COLORPARTS_H_
