#include <24FV16KM202.h>
#device ICSP = 1
//#device icd = true
//For pin 14 & pin 15
//#device ICD = 3
#FUSES FRC_PLL
#use delay(clock = 32MHZ,internal= 8MHZ)
#USE RS232(UART2,BAUD = 115200, PARITY = N, BITS = 8, STOP = 1, TIMEOUT = 500)

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES CKSFSM                   //Clock Switching is enabled, fail Safe clock monitor is enabled
#FUSES NOBROWNOUT               //No brownout reset
#FUSES BORV_LOW                 //Brown-out Reset set to lowest voltage
#use fast_io(B)



#define LCD_ENABLE_PIN PIN_A7 
#define LCD_RS_PIN     PIN_B8
#define LCD_RW_PIN     PIN_B9
#define LCD_DATA4      PIN_B12 
#define LCD_DATA5      PIN_B13
#define LCD_DATA6      PIN_B14
#define LCD_DATA7      PIN_B15

#include <lcd.c>
#include "keypad.c"
#include "uart.c"

