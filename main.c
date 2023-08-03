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

CY_ISR(isr_1_Handler) {
    // Toggle the LED output
    red_Write(!red_Read());

    // Clear the Timer/Counter interrupt
    Timer_ClearInterrupt(Timer_INTR_MASK_TC);
}

int main(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    // Set the initial LED state (OFF)
    red_Write(0);

    // Attach the Timer/Counter ISR
    isr_1_StartEx(isr_1_Handler);

    for (;;) {
        // Turn the LED on
        red_Write(1);

        // Enable the Timer/Counter
        Timer_Start();

        // Wait for the Timer/Counter to interrupt after 100 ms (0.1 second)
        CyDelay(1);

        // Disable the Timer/Counter
        Timer_Stop();

        // Turn the LED off
        red_Write(0);

        // Enter low power mode for 900 ms (0.9 seconds)
        CyDelay(900);
    }
}

/* [] END OF FILE */
