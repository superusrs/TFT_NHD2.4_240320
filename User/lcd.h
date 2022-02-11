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
#define DFT_SCAN_DIR  U2D_R2L  //Ĭ�ϵ�ɨ�跽��
#define LCD_USE8BIT_MODEL   1  //8λ16λ�л���1��8λ;0:16λ

//ɨ�跽����
#define L2R_U2D  0x00 //������,���ϵ��£�����,0�ȣ�
#define L2R_D2U  0x80 //������,���µ���
#define R2L_U2D  0x40 //���ҵ���,���ϵ���
#define R2L_D2U  0xc0 //���ҵ���,���µ��ϣ���ת180�ȣ�

#define U2D_L2R  0x20 //���ϵ���,������
#define U2D_R2L  0x60 //���ϵ���,���ҵ�����ת90�ȣ�
#define D2U_L2R  0xa0 //���µ���,�����ң���ת270�ȣ�
#define D2U_R2L  0xe0 //���µ���,���ҵ���

//LCD��Ҫ������
typedef struct  
{			    
	u16 width;			//LCD ����
	u16 height;			//LCD �߶�
	u16 id;				  //LCD ID
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����

//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD�˿ڶ���---------------- 
// #define	LCD_LED PAout(5) //LCD����  PA5

#define LCD_CS(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_CS_PIN) : (LCD_CMD_PORT->BRR|=LCD_CS_PIN))
#define LCD_DC(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_DC_PIN) : (LCD_CMD_PORT->BRR|=LCD_DC_PIN))
#define LCD_WR(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_WR_PIN) : (LCD_CMD_PORT->BRR|=LCD_WR_PIN))
#define LCD_RD(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_RD_PIN) : (LCD_CMD_PORT->BRR|=LCD_RD_PIN))
#define LCD_RE(x) ((x==1)? (LCD_CMD_PORT->BSRR|=LCD_RST_PIN) : (LCD_CMD_PORT->BRR|=LCD_RST_PIN))

#define LCD_DATA_OUT() {GPIOB->MODER=0X55555555;}
#define LCD_DATA_IN()  {GPIOB->MODER=0U;}

#define DATAOUT(x)  GPIOB->ODR=x;       //�������
#define DATAIN      GPIOB->IDR;         //��������

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    				
void LCD_WR_DAT8(unsigned char data);
void LCD_WR_DAT16(unsigned int data);
void LCD_WR_REG(u16 reg);//д�Ĵ���
u16 LCD_RD_DATA(void);//����
u16 LCD_ReadReg(u16 LCD_Reg);//���Ĵ���
void LCD_SetCursor(u16 x,u16 y);//���ù��λ��
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//�������꺯��
void LCD_DisplayOn(void);//����ʾ
void LCD_DisplayOff(void);//����ʾ
u16 LCD_ReadPoint(u16 x,u16 y);//���㺯��
void LCD_Clear(u16 color);//��������
	
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//ָ�����������ɫ
void LCD_DrawPoint(u16 x,u16 y,u16 color);//��ָ��λ�û�һ����
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//��ָ��λ�û�һ����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//��ָ��λ�û�һ������
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//��ָ��λ�û�һ��Բ

void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ���ִ�
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ����12x12����
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ����16x16����
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ����24x24����
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ����32x32����

void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾһ���ַ�
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);//��ʾ�ַ���
u32 mypow(u8 m,u8 n);//����
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//��ʾ��������
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//��ʾ��λС������

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);//��ʾͼƬ

void Set_Dir(u8 dir);
void LCD_Init(void);													   	//��ʼ��
				   						   																			 

#endif  
	 
	 


