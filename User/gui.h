#ifndef __GUI_H__
#define __GUI_H__
//#include "sys.h"
#include "Config.h"
void GUI_DrawPoint(u16 x,u16 y,u16 color);
void _draw_circle_8(int xc, int yc, int x, int y, u16 c);
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Show_Str(u16 x, u16 y,u8 *str,u16 fc, u16 bc,u8 sizey,u8 mode);
void Gui_StrCenter(u16 x, u16 y,u8 *str,u16 fc,u16 bc,u8 sizey,u8 mode);
#endif
