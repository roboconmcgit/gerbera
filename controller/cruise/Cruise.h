/*
 *      走行制御クラスのヘッダファイル
 */

#ifndef CRUISE_H_
#define CRUISE_H_
#include "ev3api.h"
#include "PartsAll.h"

using namespace ev3api;

/*
 *  関数のプロトタイプ宣言
 */
 
class Cruise{
private:

//protected:
    PARTS_ALL           *mC_allParts;               //パーツ用のクラスポインタ

public:
    Cruise(PARTS_ALL* allParts);            //コンストラクタ
    ~Cruise();                                 //デストラクタ
    void CruiseOperation();                    //動作判断
};

#endif // !CRUISE_H_
