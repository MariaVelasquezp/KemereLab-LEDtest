#include <project.h>

/******************************************************************************
 * Macro definition
 * ----------------------------------------------------------------------------
 * These Macro are only used in this module for button pressing event 
 * detection. These Macro should not be populated to other modules.
 ******************************************************************************/
#define LED_ON                (uint8)0x01
#define LED_OFF               (uint8)0x00
#define BLINK_INTERVAL_MS     500  /* Blink interval in milliseconds */
#define ILO_FREQ              32000 /* Hz */
#define DoSomething()         CyDelay(500) /* Delay for demonstration */

/******************************************************************************
 * Global variables definition
 * ----------------------------------------------------------------------------
 * These variables should be populated to other modules. The header file contains
 * the extern statement for these variables.
 ******************************************************************************/
void InitWatchdog(uint16 sleep_interval);
CY_ISR(isr_WDT);

int main(void)
{
    /*===========================================================================================
     * this code piece initializes the watchdog function 
     *==========================================================================================*/
    /* Initialize watchdog */
    InitWatchdog(BLINK_INTERVAL_MS);
    /* Connect ISR routine to Watchdog interrupt */
    isr_WDT_StartEx(isr_WDT);
    /* Set the highest priority to ensure ISR is executed in all conditions */
    isr_WDT_SetPriority(0);
    /* Enable global interrupts */
    CyGlobalIntEnable;

    for(;;)
    {
        /* Clear watchdog counter before deep sleep */
        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET);
       
        /* Go to deep sleep mode */
        CySysPmDeepSleep();

        /* Turn on Red LED after wakeup by Watchdog interrupt */
        red_Write(LED_ON);        

        /* Do something here after wakeup from deep sleep */
        DoSomething();  

        /* Turn off Red LED after doing something */
        red_Write(LED_OFF);
    }
}

/*******************************************************************************
* Function Name: InitWatchdog
********************************************************************************
* Summary:
*  Initialize watchdog counter0 with the specific sleep interval parameter.
*
* Parameters:
*  uint16 sleep_interval  Reset interval in millisecond unit.
*
* Return:
*  void
*
*******************************************************************************/
void InitWatchdog(uint16 sleep_interval)
{
    /*==============================================================================*/
    /* Configure counter 0 for wakeup interrupt                   */
    /*==============================================================================*/
    /* Counter 0 of the Watchdog time generates periodically interrupt to wake up the system */
    CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
    /* Set interval as desired value */
    CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, ((uint32)(sleep_interval * ILO_FREQ) / 1000));
    /* Clear counter on match event */
    CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, 1u);
    /*==============================================================================*/
    /* Enable watchdog                                                              */
    /*==============================================================================*/
    /* Enable the counter 0 */
    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
    /* Check if counter 0 is enabled, otherwise keep looping here */
    while (!CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER0));
}

CY_ISR(isr_WDT)
{
    /* Clear interrupt flag to enable the next interrupt */
    CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
}

/* [] END OF FILE */
