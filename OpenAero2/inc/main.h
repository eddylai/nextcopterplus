/*********************************************************************
 * main.h
 ********************************************************************/
#include <stdbool.h>

//***********************************************************
//* Externals
//***********************************************************

extern bool	AutoLevel;
extern bool	Stability;
extern char pBuffer[16];
extern bool	Failsafe;
extern uint8_t	buffer[];
extern bool	RefreshStatus;
extern volatile int8_t General_error;
extern uint32_t ticker_32;	
extern uint16_t StackCount(void);	