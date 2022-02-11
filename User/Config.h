#ifndef _CONFIG_H_
#define _CONFIG_H_


#if defined USE_HAL_DRIVER
#include "main.h"
#define LCD_CS_PIN GPIO_PIN_0
#define LCD_DC_PIN GPIO_PIN_1
#define LCD_WR_PIN GPIO_PIN_2
#define LCD_RD_PIN GPIO_PIN_3
#define LCD_RST_PIN GPIO_PIN_4

#define LCD_DAT_PORT GPIOB
#define LCD_CMD_PORT GPIOA

#define u16 unsigned int
#define u32 unsigned int
#define u8	unsigned char
#define delay_ms(x) HAL_Delay(x)

#endif

#endif
