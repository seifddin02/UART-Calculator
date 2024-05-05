#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>
#include "int_handler.h"


void IntPortJHandler(void) {
    // Check if the interrupt is due to PJ0 (SW1)
    if (GPIO_PORTJ_AHB_MIS_R & (1 << 0)) {
        // Clear the interrupt flag
        GPIO_PORTJ_AHB_ICR_R = 0x01;


        char str[50];
        val++;
        sprintf(str, "%.3f", val); // Convert to string with 3 decimal places


        while ((UART0_FR_R & (1 << 5)) != 0); // Wait until transmit FIFO is empty
        UART0_DR_R = '#';

        int i = 0;
        while (str[i] != '\0')
        {
            while ((UART0_FR_R & (1 << 5)) != 0); // Wait until transmit FIFO is empty
            UART0_DR_R = str[i];
            i++;
        }

        while (UART0_FR_R & (1 << 5)); // Wait until transmit FIFO is empty
        UART0_DR_R = '\r';

    }

    // Check if the interrupt is due to PJ1 (SW2)
    if (GPIO_PORTJ_AHB_MIS_R & (1 << 1)) {
        // Clear the interrupt flag
        GPIO_PORTJ_AHB_ICR_R = 0x02;


        char str[50];
        val--;
        sprintf(str, "%.3f", val); // Convert to string with 3 decimal places


        while ((UART0_FR_R & (1 << 5)) != 0); // Wait until transmit FIFO is empty
        UART0_DR_R = '#';

        int i = 0;
        while (str[i] != '\0')
        {
            while ((UART0_FR_R & (1 << 5)) != 0); // Wait until transmit FIFO is empty
            UART0_DR_R = str[i];
            i++;
        }

        while (UART0_FR_R & (1 << 5)); // Wait until transmit FIFO is empty
        UART0_DR_R = '\r';

    }

    int k;
    for (k = 0; k < 600000; k++); // => Wait loop counting for delay


}

