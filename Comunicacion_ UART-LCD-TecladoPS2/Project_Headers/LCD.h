/*
 * LCD.h
 *
 *  Created on: Nov 19, 2013
 *      Author: ANTONIO
 */

#ifndef LCD_H_
#define LCD_H_

#define GPIOB_EN_MASK (1<<2)
#define GPIOB_RS_MASK (1)
#define GPIOB_RW_MASK (1<<1)
#define GPIOB_MASK 7
#define GPIOD_DATA_MASK (0xFF)

#define CLEAR             (0x01)
#define TURN_ON           (0X0C)
#define LINE_FEED         (0xC0)
#define SECOND_LINE_EN    (0x38)
#define INCREASE_CURSOR   (0x06)
#define DECREASE_CURSOR   (0x04)
#define BACKSPACE         (0x10)
#define DDRAM_ADDRESS_0   (0x80)
#define DDRAM_ADDRESS_1   (0x81)
#define DDRAM_ADDRESS_15  (0x8F)
#define DDRAM_ADDRESS_64  (0xC0)
#define DDRAM_ADDRESS_67  (0xC3)
#define DDRAM_ADDRESS_71  (0xC7)
#define DDRAM_ADDRESS_74  (0xCA)
#define DDRAM_ADDRESS_79  (0xCF)
#define CGRAM_ADDRESS_0   (0x40)
#define SPACE             (0x20)
#define UP                (0x80)
#define DOWN              (0xC0)
#define HOME              (0x02)
#define CURSOR_ON 		  (0xD)
#define CURSOR_OFF        (0xC)

#define WAIT_TIME (0xFFF)

#define INS (1)
#define CHAR (0)

void vfnLCD_Init(void);
void vfnSet_RS(void);
void vfnClear_RS(void);
void vfnEnable(void);
void vfnLCD_Write(char bCommand, char bData);
void vfnLCD_Write_Array(char bcounter,char * pbArray);


#endif /* LCD_H_ */
