#include "init.h"

/********************************************************************
 *                              main                                *
 ********************************************************************/
 
union SPEED {
	short value;
	char byte[2];
} speed = {0};
bool increment = 1;
char status_data[8]; // task 4
void main(void)
{
    /* board initialization */
    Init();
	/* configure channel0 of PIT to give interrupt every 200 ms */
	PIT_ConfigureTimer( 0, 200 );
	/* start channel0 timer */
	//PIT_StartTimer( 0 );

    /* forever */
    for(;;)
    {
/********************************************************************
 *                           Main Loop                              *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                  Write down your logic here.                     *
 ********************************************************************/  

    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                Interrupts can be handled below.                  *
 ********************************************************************/  
void PITCHANNEL0() {
    /* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1;
	
	/* toggle LED0 */
	LED0 = ~LED0;
	
	/* send message */
	CAN_0.BUF[0].DATA.B[0] = speed.byte[0];
	CAN_0.BUF[0].DATA.B[1] =  speed.byte[1];
	CAN_0.BUF[0].CS.B.CODE = 12;
	
	/* calculate next speed */
	if(increment = 1) {
		speed.value += 5;
		if(speed.value == 300)
			increment = 0;
	} else {
		speed.value -= 5;
		if(speed.value == 0)
			increment = 1;
	}
}

void MB_0_3() {
	CAN_0.IFRL.B.BUF00I = 1;
	CAN_0.IFRL.B.BUF01I = 1;
	CAN_0.IFRL.B.BUF02I = 1;
	CAN_0.IFRL.B.BUF03I = 1;
}

void MB_4_7() {
	switch(CAN_0.RXFIFO.ID.B.STD_ID) {
		case 0xff:
			LED1 = ~LED1;
			break;
		case 0x00:
			LED2 = 0;
			PIT_StartTimer( 0 );
			break;
		case 0x81:
		case 0x83:
		case 0x85:
		case 0x87:
		case 0x89:
		case 0x8b:
		case 0x8d:
		case 0x8f:
			LED3 = ~LED3;
			break;
		default:
			break;
		
		
	}
	
	CAN_0.IFRL.B.BUF04I = 1;
	CAN_0.IFRL.B.BUF05I = 1;
	CAN_0.IFRL.B.BUF06I = 1;
	CAN_0.IFRL.B.BUF07I = 1;
}

void MB_8_11() {
	CAN_0.IFRL.B.BUF08I = 1;
	CAN_0.IFRL.B.BUF09I = 1;
	CAN_0.IFRL.B.BUF10I = 1;
	CAN_0.IFRL.B.BUF11I = 1;
}

void MB_12_15() {
	CAN_0.IFRL.B.BUF12I = 1;
	CAN_0.IFRL.B.BUF13I = 1;
	CAN_0.IFRL.B.BUF14I = 1;
	CAN_0.IFRL.B.BUF15I = 1;
}
/* ... ISRs ... */

/********************************************************************
 *                   Interrupt Vector Table                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
    /* ... ISR calls should be here ... */
		case 59:
			PITCHANNEL0();
            break;
		case 68:
			MB_0_3();
			break;
		case 69:
			MB_4_7();
			break;
		case 70:
			MB_8_11();
			break;
		case 71:
			MB_12_15();
			break;
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
