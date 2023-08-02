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
#include <project.h>

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Start Sleep Timer with desired interval for flashing the LED */
    CySysTickStart();

    for (;;)
    {
        /* Flash the LED by toggling its state */
        red_Write(1); /* Turn LED on */
        CyDelay(100); /* Delay for ON state */
        red_Write(0); /* Turn LED off */
        CyDelay(900); /* Delay for OFF state */

        /* Enter low power mode (Sleep) until Sleep Timer interrupt */
        CySysPmSleep();
    }
}


/* [] END OF FILE */
