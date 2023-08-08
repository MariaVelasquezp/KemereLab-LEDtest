/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdbool.h>
bool state = true;

void interrupcion_malu(){
    state=!state;
    /*
    if(state){
        state=false;
        red_Write(0);
        red_2_Write(0);
    }
    else {
     state= true;   
         red_Write(1);
        red_2_Write(1);
    }*/
    
    uint32 reason = CySysWdtGetInterruptSource();  
    CySysWdtClearInterrupt(reason); 
}

int main(void)
{
    //WDT2 definido en user design
    //User Provided implica dar dirección de función a la ISR
    isr_1_StartEx(interrupcion_malu);
    
    CySysWdtUnlock(); // Enable modification of the WDT Timers
    
    // Turn off the WDTs (all three of them)
    CySysWdtDisable(CY_SYS_WDT_COUNTER0_MASK); 
    CySysWdtDisable(CY_SYS_WDT_COUNTER1_MASK);
    CySysWdtDisable(CY_SYS_WDT_COUNTER2_MASK);
    
    // Make Timer 0 & 1 run with no interrupt, and 2 cause an interrupt
    CySysWdtSetMode(CY_SYS_WDT_COUNTER0,CY_SYS_WDT_MODE_NONE);
    CySysWdtSetMode(CY_SYS_WDT_COUNTER1,CY_SYS_WDT_MODE_NONE);
    CySysWdtSetMode(CY_SYS_WDT_COUNTER2,CY_SYS_WDT_MODE_INT);
    
    // Set the time to 32768/(64*64*2^6) = 32768/(2^18) = 0.125 = 1/8 Hz
    CySysWdtSetMatch(CY_SYS_WDT_COUNTER0, 16);
    CySysWdtSetMatch(CY_SYS_WDT_COUNTER1, 16);
    CySysWdtSetToggleBit(6);
    
    CySysWdtSetClearOnMatch(CY_SYS_WDT_COUNTER0,1); // When counter his period reset counter
    CySysWdtSetClearOnMatch(CY_SYS_WDT_COUNTER1,1);

    CySysWdtSetCascade(CY_SYS_WDT_CASCADE_01 | CY_SYS_WDT_CASCADE_12); // Cascade 0-1-2
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK | CY_SYS_WDT_COUNTER1_MASK | CY_SYS_WDT_COUNTER2_MASK );
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    for(;;) {
    CySysPmDeepSleep(); // wait for interrupt in deep sleep    
    }
}
/* [] END OF FILE */
