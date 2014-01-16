//***********************************************************
//* display_status.c
//***********************************************************

//***********************************************************
//* Includes
//***********************************************************

#include <avr/io.h>
#include <stdlib.h>
#include "io_cfg.h"
#include "glcd_driver.h"
#include "mugui.h"
#include <avr/pgmspace.h>
#include "glcd_menu.h"
#include "main.h"
#include "vbat.h"
#include <util/delay.h>
#include "menu_ext.h"
#include "mixer.h"

extern uint16_t StackCount(void);	

//************************************************************
// Prototypes
//************************************************************

void Display_status(void);

//************************************************************
// Code
//************************************************************

void Display_status(void)
{
	int16_t temp;
	uint16_t vbat_temp; 
	int8_t	pos1, pos2, pos3;
	mugui_size16_t size;

	clear_buffer(buffer);

	// Display text
	LCD_Display_Text(3,(prog_uchar*)Verdana8,0,0); 		// Version text
	LCD_Display_Text(5,(prog_uchar*)Verdana8,0,16); 	// RX sync
	LCD_Display_Text(6,(prog_uchar*)Verdana8,0,27); 	// Profile
	LCD_Display_Text(23,(prog_uchar*)Verdana8,88,27); 	// Pos
	LCD_Display_Text(133,(prog_uchar*)Verdana8,0,38); 	// Battery
	mugui_lcd_puts(itoa(transition,pBuffer,10),(prog_uchar*)Verdana8,110,27); 		// Raw transition value
	
	// Display menu and markers
	LCD_Display_Text(9, (prog_uchar*)Wingdings, 0, 59);	// Down
	LCD_Display_Text(14,(prog_uchar*)Verdana8,10,55);	// Menu

	// Display values
	print_menu_text(0, 1, (62 + Config.RxMode), 45, 16); // Rx mode

	// Display transition point
	if (transition <= 0)
	{
		LCD_Display_Text(48,(prog_uchar*)Verdana8,45,27);
	}
	else if (transition >= 100)
	{
		LCD_Display_Text(50,(prog_uchar*)Verdana8,45,27);
	}
	else if (transition == Config.Transition_P1n)
	{
		LCD_Display_Text(49,(prog_uchar*)Verdana8,45,27);
	}
	else if (transition < Config.Transition_P1n)
	{
		LCD_Display_Text(51,(prog_uchar*)Verdana8,45,27);
	}
	else if (transition > Config.Transition_P1n)
	{
		LCD_Display_Text(52,(prog_uchar*)Verdana8,45,27);
	}

	// Display voltage
	uint8_t x_loc = 45;		// X location of voltage display
	uint8_t y_loc = 38;		// Y location of voltage display

	vbat_temp = GetVbat();
	temp = vbat_temp/100;	// Display whole decimal part first
	mugui_text_sizestring(itoa(temp,pBuffer,10), (prog_uchar*)Verdana8, &size);
	mugui_lcd_puts(itoa(temp,pBuffer,10),(prog_uchar*)Verdana8,x_loc,y_loc);
	pos1 = size.x;

	vbat_temp = vbat_temp - (temp * 100); // Now display the parts to the right of the decimal point

	LCD_Display_Text(7,(prog_uchar*)Verdana8,(x_loc + pos1),y_loc);
	mugui_text_sizestring(".", (prog_uchar*)Verdana8, &size);
	pos3 = size.x;
	mugui_text_sizestring("0", (prog_uchar*)Verdana8, &size);
	pos2 = size.x;

	if (vbat_temp >= 10)
	{
		mugui_lcd_puts(itoa(vbat_temp,pBuffer,10),(prog_uchar*)Verdana8,(x_loc + pos1 + pos3),y_loc);
	}
	else
	{
		LCD_Display_Text(8,(prog_uchar*)Verdana8,(x_loc + pos1 + pos3),y_loc);
		mugui_lcd_puts(itoa(vbat_temp,pBuffer,10),(prog_uchar*)Verdana8,(x_loc + pos1 + pos2 + pos3),y_loc);
	}

	// Display error messages
	if (General_error != 0)
	{
		// Create message box
		fillrect(buffer, 14,8, 96, 48, 0);	// White box
		drawrect(buffer, 14,8, 96, 48, 1); 	// Outline

		// Prioritise error from top to bottom
		if((General_error & (1 << SENSOR_ERROR)) != 0)
		{
			LCD_Display_Text(72,(prog_uchar*)Verdana14,35,14); 	// Sensor
			LCD_Display_Text(19,(prog_uchar*)Verdana14,43,34); 	// Error
			menu_beep(9);
		}
		else if((General_error & (1 << LVA_ALARM)) != 0)
		{
			LCD_Display_Text(134,(prog_uchar*)Verdana14,33,14); // Battery
			LCD_Display_Text(73,(prog_uchar*)Verdana14,46,34); 	// Low
		}
		else if((General_error & (1 << NO_SIGNAL)) != 0)
		{
			LCD_Display_Text(75,(prog_uchar*)Verdana14,51,13); 	// No
			LCD_Display_Text(76,(prog_uchar*)Verdana14,39,33);  // Signal
		}
		else if((General_error & (1 << THROTTLE_HIGH)) != 0)
		{
			LCD_Display_Text(105,(prog_uchar*)Verdana14,28,14); // Throttle
			LCD_Display_Text(55,(prog_uchar*)Verdana14,46,34);	// High
		}
		else if((General_error & (1 << DISARMED)) != 0)
		{
			LCD_Display_Text(18,(prog_uchar*)Verdana14,25,24); 	// Disarmed
		}
	}

	// Write buffer to complete
	write_buffer(buffer,1);
	clear_buffer(buffer);
}
