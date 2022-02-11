#ifndef _ST7789_H_
#define _ST7789_H_
void comm_out(unsigned char c);
void data_out(unsigned char d);
void window_set(unsigned s_x, unsigned e_x, unsigned s_y, unsigned e_y);
void disp(void);
void setup(void);
void loop(void);

#define LCD_CS(x) (x==1)?HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_CS_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_CS_PIN,GPIO_PIN_RESET)
#define LCD_DC(x) (x==1)?HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_DC_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_DC_PIN,GPIO_PIN_RESET)
#define LCD_WR(x) (x==1)?HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_WR_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_WR_PIN,GPIO_PIN_RESET)
#define LCD_RD(x) (x==1)?HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_RD_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_RD_PIN,GPIO_PIN_RESET)
#define LCD_RST(x) (x==1)?HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_RST_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CMD_PORT,LCD_RST_PIN,GPIO_PIN_RESET)

#endif
