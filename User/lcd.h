/*

LCD Controller: ST7789s, IM0 connect to 3V3, 8bit mode;
IM3,IM2,IM1,IM0:
1001,80-8bit parallel 		DB[17:10], 
1000,80-16bit parallel   	DB[17:10],DB[8:1]

*/

#ifndef __LCD_H
#define __LCD_H		
#include "stdlib.h" 
#include "Config.h"
//#define USE_HORIZONTAL 3 
#define DFT_SCAN_DIR  U2D_R2L  //默认的扫描方向
#define LCD_USE8BIT_MODEL   1  //8位16位切换，1：8位;0:16位

//扫描方向定义
#define L2R_U2D  0x00 //从左到右,从上到下（正向,0度）
#define L2R_D2U  0x80 //从左到右,从下到上
#define R2L_U2D  0x40 //从右到左,从上到下
#define R2L_D2U  0xc0 //从右到左,从下到上（旋转180度）

#define U2D_L2R  0x20 //从上到下,从左到右
#define U2D_R2L  0x60 //从上到下,从右到左（旋转90度）
#define D2U_L2R  0xa0 //从下到上,从左到右（旋转270度）
#define D2U_R2L  0xe0 //从下到上,从右到左

//LCD重要参数集
typedef struct  
{			    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				  //LCD ID
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数

//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD端口定义---------------- 
// #define	LCD_LED PAout(5) //LCD背光  PA5

#define LCD_CS(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_CS_PIN) : (LCD_CMD_PORT->BRR|=LCD_CS_PIN))
#define LCD_DC(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_DC_PIN) : (LCD_CMD_PORT->BRR|=LCD_DC_PIN))
#define LCD_WR(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_WR_PIN) : (LCD_CMD_PORT->BRR|=LCD_WR_PIN))
#define LCD_RD(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_RD_PIN) : (LCD_CMD_PORT->BRR|=LCD_RD_PIN))
#define LCD_RE(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_RST_PIN) : (LCD_CMD_PORT->BRR|=LCD_RST_PIN))

#define LCD_DATA_OUT() {GPIOB->MODER=0X55555555;}
#define LCD_DATA_IN()  {GPIOB->MODER=0U;}

#define DATAOUT(x)  GPIOB->ODR=x;       //数据输出
#define DATAIN      GPIOB->IDR;         //数据输入

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    				
void LCD_WR_DAT8(unsigned char data);
void LCD_WR_DAT16(unsigned int data);
void LCD_WR_REG(u16 reg);//写寄存器
u16 LCD_RD_DATA(void);//读点
u16 LCD_ReadReg(u16 LCD_Reg);//读寄存器
void LCD_SetCursor(u16 x,u16 y);//设置光标位置
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_DisplayOn(void);//开显示
void LCD_DisplayOff(void);//关显示
u16 LCD_ReadPoint(u16 x,u16 y);//读点函数
void LCD_Clear(u16 color);//清屏函数
	
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//指定区域填充颜色
void LCD_DrawPoint(u16 x,u16 y,u16 color);//在指定位置画一个点
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//在指定位置画一条线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//在指定位置画一个矩形
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//在指定位置画一个圆

void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示汉字串
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个32x32汉字

void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);//显示一个字符
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);//显示字符串
u32 mypow(u8 m,u8 n);//求幂
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//显示整数变量
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//显示两位小数变量

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);//显示图片

void Set_Dir(u8 dir);
void LCD_Init(void);													   	//初始化
				   						   																			 

#endif  
	 
	 



