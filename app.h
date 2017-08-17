/*
 *      サンプルプログラム(1)のヘッダファイル
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ターゲット依存の定義
 */
#include "ev3api.h"
#include "target_test.h"


/*
 *  各タスクの優先度の定義
 */
#define PRIORITY_PRD_TSK_1 TMIN_APP_TPRI
#define PRIORITY_PRD_TSK_2 TMIN_APP_TPRI
#define PRIORITY_MAIN_TASK (TMIN_APP_TPRI + 1)
#define PRIORITY_BT_TASK   (TMIN_APP_TPRI + 2)


/*
 *  ターゲットに依存する可能性のある定数の定義
 */

/**
 * Task periods in ms
 */
#define PERIOD_PRD_TSK_1  (4)
#define PERIOD_PRD_TSK_2  (10)

#ifndef TASK_PORTID
#define TASK_PORTID     1           /* 文字入力するシリアルポートID */
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define STACK_SIZE      4096        /* タスクのスタックサイズ */
#endif /* STACK_SIZE */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void bt_task(intptr_t exinf);

extern void periodic_4mstask(intptr_t);
//extern void periodic_task_2(intptr_t);

extern void task_activator(intptr_t tskid);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
