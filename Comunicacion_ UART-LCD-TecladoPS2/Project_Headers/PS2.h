/*
 * PS2.h
 *
 *  Created on: Mar 16, 2014
 *      Author: ANTONIO
 */

#ifndef PS2_H_
#define PS2_H_

#define RESET 0xFF
#define SET_REMOTE_MODE 0xF0
#define SET_STREAM_MODE 0xEA
#define SAMPLE_RATE     0xF3
#define SET_RES         0xE8
#define READ_DATA       0xEB
#define EN_DATA_REPORT  0xF4


void vfnPS2_Config(void);
void vfnPS2_Send(char bData, char bReceive);
void vfnMouse_Init(void);
void vfnLCD_Write_Mouse(void);
void vfnBCD_Covert(char bData, char *pbBCD);
void vfnGetKey(void);
void vfnKeyboardLCD(void);

#endif /* PS2_H_ */
