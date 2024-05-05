#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "int_handler.h"
#include<string.h>
double val;


void configPortA(void)
{
    // initialize Port A
    SYSCTL_RCGCGPIO_R |= (1 << 0); // switch on clock for Port A
    while ((SYSCTL_PRGPIO_R & (1 << 0)) == 0)
        ; // wait for stable clock
    GPIO_PORTA_AHB_DEN_R |= 0x03; // digital I/O enable for pin PA(1:0)
    GPIO_PORTA_AHB_AFSEL_R |= 0x03; // PA(1:0) set to alternate func
    GPIO_PORTA_AHB_PCTL_R |= 0x00000011; // alternate func is U0Rx+Tx
}

void configUart0(void)
{
    // initialize UART0
    SYSCTL_RCGCUART_R |= (1 << 0); // switch on clock for UART0
    while ((SYSCTL_PRUART_R & (1 << 0)) == 0)
        ; // wait for stable clock
    UART0_CTL_R &= ~(1 << 0); // disable UART0 for config
    // initialize bitrate of 9600 bit per second
    UART0_IBRD_R = 104; // set DIVINT of BRD floor(16MHz/16/9600Hz)
    UART0_FBRD_R = 11; // set DIVFRAC of BRD
    UART0_LCRH_R = 0x00000040; // serial format 7N1
    UART0_CTL_R |= 0x0301; // re-enable UART0

    /* // Enable UART0 interrupts
     UART0_ICR_R = 0x0F; // Clear all interrupts for UART0
     UART0_IM_R |= 1 << 4; // Activate UART0 Rx interrupts

     // Enable UART0 interrupt in NVIC
     NVIC_EN0_R |= 1 << (5);*/
}

void initGPIO(void)
{
    // Enable clock for Port N (LEDs) and Port J (SW1)
    SYSCTL_RCGCGPIO_R |= 0x00001120; // => Enabling the ports J

    // Wait until the clock is stable
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R12) == 0
        || (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R8) == 0)
        ;


    // Configure PJ0 and PJ1 for interrupt on both edges
    GPIO_PORTJ_AHB_DEN_R = 0x03;
    GPIO_PORTJ_AHB_DIR_R = 0x00;
    GPIO_PORTJ_AHB_PUR_R = 0x03;

    GPIO_PORTJ_AHB_IS_R &= ~0x03;   // Set as edge-sensitive for PJ0 and PJ1
    GPIO_PORTJ_AHB_IBE_R &= ~0x03;  // Trigger on one for PJ0 and PJ1
    GPIO_PORTJ_AHB_IEV_R &= ~0x03;  // falling edge direction for PJ0 and PJ1

    GPIO_PORTJ_AHB_ICR_R |= 0x03;   // Clear potential interrupt event for PJ0 and PJ1
    GPIO_PORTJ_AHB_IM_R |= 0x03;    // Unmask PJ0 and PJ1 interrupt


    // Enable NVIC for Port J (IRQ number 51)
    NVIC_EN1_R |= (1 << (51 - 32)) | (1 << (52 - 32));
}



double calculateCmd(unsigned char* command) {
    double result = 0;
    int i = 0; // Initialize counter to start of string
    char* nextChar;
    result = strtod(command, &nextChar); // Parse first number


    i += nextChar - command; // Update index to position after first number

    // Loop over the rest of the string
    while (command[i] != '\0' && command[i] != '\n') {
        char op = command[i++]; // Get operator and increment index

        // Check if operator is valid
        if (op == '+' || op == '-' || op == '*' || op == '/') {
            // Check for consecutive operators
            if (command[i] == '+' || command[i] == '-' || command[i] == '*' || command[i] == '/') {
                printf("Error: consecutive operators\n");
                return 0;
            }

            // Parse next number
            double value = strtod(&command[i], &nextChar);
            i += nextChar - &command[i]; // Update i to position after number

            // Perform operation based on operator
            switch (op) {
            case '+': result += value; break;
            case '-': result -= value; break;
            case '*': result *= value; break;
            case '/': result /= value; break;
            }
        }
        else {
            // if operator is not valid, print error message and return 0
            printf("Error: invalid operator '%c'\n", op);
            return 0;
        }
    }

    // Return final result
    return result;
}


int main(void) {
    configPortA();  // Configure Port A for UART0
    initGPIO(); // Initialize GPIO
    configUart0(); // Configure UART0

    unsigned char cmdLineUart[16];
    const char promptText = '>';
    int i = 0;
    char str[50];



    while (1) {
        while ((UART0_FR_R & (1 << 5)) != 0); // Wait until Tx FIFO is not full
        UART0_DR_R = promptText;    // Send prompt to the uart

        i = 0;   // reset i and clear cmdLineUart from the promptText
        memset(cmdLineUart, 0, sizeof(cmdLineUart));

        while (1) {
            // Check if data is received
            if ((UART0_FR_R & (1 << 4)) == 0) { // Check if Rx FIFO is not empty

                // End of command check for carriage return or if maximum no of characters in the string is reached and set that character to null
                if (UART0_DR_R == 0x0D || i == 15) {
                    cmdLineUart[i] = '\0';
                    printf("%s\n", cmdLineUart); // print to console
                    val =calculateCmd(cmdLineUart); // process the command to control LEDs

                    sprintf(str, "%.3f", val); // Convert to string with 3 decimal places


                          while ((UART0_FR_R & (1 << 5)) != 0); // Wait until transmit FIFO is empty
                          UART0_DR_R = '=';

                          int i = 0;
                          while (str[i] != '\0')
                          {
                              while ((UART0_FR_R & (1 << 5)) != 0); // Wait until transmit FIFO is empty
                              UART0_DR_R = str[i];
                              i++;
                          }
                          while (UART0_FR_R & (1 << 5)); // Wait until transmit FIFO is empty
                          UART0_DR_R = '\r';



                    break;
                }
                else {
                    cmdLineUart[i] = UART0_DR_R;
                    i++;
                }
            }
        }
    }
}
