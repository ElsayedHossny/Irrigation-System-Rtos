

#ifndef LCD_H_
#define LCD_H_

#define  F_CPU 16000000UL //change
#include "../StdTypes.h"
#include "../MCAL/DIO_Interface.h"
#include <util/delay.h>

#define   _4_BIT   0
#define  _8_BIT   1


/*********************************config***************************************/

#define LCD_MODE    _4_BIT

#define   RS           PINC0
#define   RW           PINC1
#define   EN           PINC2

/*for 8 bit*/
#define   LCD_PORT     PD
/*for 4bit */
#define    D4    PIND4
#define    D5    PIND5
#define    D6    PINC7
#define    D7    PIND7


/*****************************************************************************/
void LCD_Init(void);
void LCD_WriteString(u8*str);
void LCD_CreateCharacter(u8 *Pattern,u8 Location);

void LCD_WriteChar(u8 ch);
void LCD_WriteString(u8*str);
void LCD_WriteNumber(s64 num);
void LCD_WriteNumberInBinary(u8 num);
void LCD_WriteNumberInBinary2(u8 num);
void LCD_GoTo(u8 Line, u8 x);
void LCD_Clear(void);
void LCD_WriteNumber_4D(u16 num);



void LCD_WriteNumber_GoTo(u8 line ,u8 x,s64 num);
#endif /* LCD_H_ */


