/*
 *      モーターバーツクラスのパラメータファイル
 */

#ifndef MOTORPARTS_PRM_H_
#define MOTORPARTS_PRM_H_

#define LEFT_MOTOR_CH    (PORT_C)
#define RIGHT_MOTOR_CH   (PORT_B)
#define TAIL_MOTOR_CH    (PORT_A)

#define TAIL_ANGLE_STAND_UP  80  /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3  /* バランス走行時の角度[度] */
#define P_GAIN             2.5F  /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60  /* 完全停止用モータ制御PWM絶対最大値 */

#endif // !MOTORPARTS_PRM_H_
