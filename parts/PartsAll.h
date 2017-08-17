/*
 *      バーツクラスの統括ヘッダファイル
 */

#ifndef PARTSALL_H_
#define PARTSALL_H_
#include "ColorParts.h"
#include "GyroParts.h"
#include "MotorParts.h"
#include "SonarParts.h"
#include "TouchParts.h"

using namespace ev3api;

// キャリブレーションパラメータ
typedef struct Parts_AllHandler{
    GyroParts*      tag_pC_gyroParts;
    MotorParts*     tag_pC_motorParts;
    ColorParts*     tag_pC_colorParts;
    SonarParts*     tag_pC_sonarParts;
    TouchParts*     tag_pC_touchParts;
}PARTS_ALL;


#endif // !PARTSALL_H_
