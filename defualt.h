/*
 *      モーターバーツクラスのヘッダファイル
 */

#ifndef default_H_
#define default_H_
#include "ev3api.h"
#include "xxx.h"

using namespace ev3api;

// キャリブレーションパラメータ
typedef struct default_Calibra{
    int32_t Offset[default_NUM];
}default_CALIBRA;

/*
 *  関数のプロトタイプ宣言
 */
 
class default{
private:
    int32_t defualt_Offset[default_NUM];    //オフセットデータ
    bool defualt_State[default_NUM];        //状態フラグ　0:正常、1:以上

//protected:
    xxx          *xxx;               //default用のクラス

public:
    default();                       //コンストラクタ
    ~default();                      //デストラクタ
    void defaultCalibra(default_CALIBRA s_calibra);
                                        //全てのモータのキャリブレーションを設定する
    void defaultReset(int8_t motorBit);
                                        //指定したモータをリセットする
};

#endif // !default_H_
