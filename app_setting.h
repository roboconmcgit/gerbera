/* Parameter Setting File */
#ifndef PARAM_SETTING_H_
#define PARAM_SETTING_H_

/* 下記のマクロは個体/環境に合わせて変更する必要があります */

#define DEVICE_NAME     "ET0"  /* Bluetooth名 hrp2/target/ev3.h BLUETOOTH_LOCAL_NAMEで設定 */
#define PASS_KEY        "1234" /* パスキー    hrp2/target/ev3.h BLUETOOTH_PIN_CODEで設定 */
#define CMD_START         '1'    /* リモートスタートコマンド */

/* 共通 */
#define TURN_STRAIGHT       0
#define TURN_RIGHT          1
#define TURN_LEFT           2
#define CHATA_MAX           3
#define TURN_WAIT_TIME      3

/* 速度設定 */
#define FORWORD_STOP        0
#define FORWORD_NOMAL       30

/* ライントレースパラメータ */
#define LINETRACE_TURN_MAX  30
#define LINETRACE_BRIGHTNESS    30
#define LEFTRUN_MAX         3

#endif // !PARAM_SETTING_H_

