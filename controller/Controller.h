/*
 *      コントローラクラスのヘッダファイル
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "ev3api.h"
#include "PartsAll.h"
#include "Cruise.h"

using namespace ev3api;

/*
 *  関数のプロトタイプ宣言
 */
 
class Controller{
private:

//protected:
    PARTS_ALL           *mC_allParts;               //パーツ用のクラスポインタ
    Cruise              *mC_Cruise;                   //走行制御用のクラスポインタ

public:
    Controller(PARTS_ALL* allParts);            //コンストラクタ
    ~Controller();                                 //デストラクタ
    void ControllerOperation();                    //動作判断
};

#endif // !CONTROLLER_H_
