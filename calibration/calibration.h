/*
 *      キャリブレーションクラスのヘッダファイル
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_
#include "ev3api.h"
#include "MotorParts.h"

using namespace ev3api;

/*
 *  関数のプロトタイプ宣言
 */
 
class calibration{
private:

//protected:
//    xxx          *xxx;               //calibration用のクラス

public:
    calibration();                       //コンストラクタ
    ~calibration();                      //デストラクタ
    void set_calibration(MotorParts* motorParts);
                                        //キャリブレーションを設定する
};

#endif // !CALIBRATION_H_
