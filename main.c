/*
 *
 * Test & Troubleshooting Shell
 *
 */

#define PART_TM4C123GH6PM

/*
 * Includes
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"

/*
 * Definitions
 */

// Launchpad I/O
#define LP_LED_RED		GPIO_PIN_1
#define LP_LED_BLUE		GPIO_PIN_2
#define LP_LED_GREEN	GPIO_PIN_3
#define LP_SW_1			GPIO_PIN_4
#define LP_SW_2			GPIO_PIN_0
#define LP_LEDS			(LP_LED_RED|LP_LED_BLUE|LP_LED_GREEN)
#define LP_SWS			(LP_SW_1|LP_SW_2)

/*
 * Functions
 */
void cmdHelp(int argc, char *argv[]) {
	UARTprintf("Available Commands: \n");
	UARTprintf("authors\t\t\t Prints the names of the authors\n");
	UARTprintf("ledOn [color]\t\t Turns on LED\n");
	UARTprintf("ledOff [color]\t\t Turns off LED \n");
}

void cmdAuthors(int argc, char *argv[]) {
	UARTprintf("+-----------------> Authors <----------------+\n");
	UARTprintf("|  Brought to you by:                        |\n");
	UARTprintf("|  Luis Bañuelos                             |\n");
	UARTprintf("|  Graciela Bribiescas                       |\n");
	UARTprintf("|  Daniel Fernandez                          |\n");
	UARTprintf("|  Adrian Melendez                           |\n");
	UARTprintf("+--------------------------------------------+\n");
}

void cmdLedOn(int argc, char *argv[]) {

	if (argc > 1) {
		if (strcmp(argv[1], "red") == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, LP_LED_RED, LP_LED_RED);
		}

		if (strcmp(argv[1], "green") == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, LP_LED_GREEN, LP_LED_GREEN);
		}

		if (strcmp(argv[1], "blue") == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, LP_LED_BLUE, LP_LED_BLUE);
		}

		if (strcmp(argv[1], "all") == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, LP_LEDS, LP_LEDS);
		}
	} else {
		UARTprintf("Usage: ledOn [color]");
	}

}

void cmdLedOff(int argc, char *argv[]) {

	if (argc > 1) {
		if (strcmp(argv[1], "red") == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, LP_LED_RED, 0);
		}

		if (strcmp(argv[1], "green") == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, LP_LED_GREEN, 0);
		}

		if (strcmp(argv[1], "blue") == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, LP_LED_BLUE, 0);
		}

		if (strcmp(argv[1], "all") == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, LP_LEDS, 0);
		}
	} else {
		UARTprintf("Usage: ledOff [color]");
	}
}


/*
 * Commands List
 */
tCmdLineEntry g_psCmdTable[] = {
	{"help", (pfnCmdLine) cmdHelp, "Provides a list of available commands."},
	{"authors", (pfnCmdLine) cmdAuthors, "Provides a list of the authors of this project."},
	{"ledOn", (pfnCmdLine) cmdLedOn, "Usage: ledOff [color]"},
	{"ledOff", (pfnCmdLine) cmdLedOff, "Usage: ledOff [color]"},
	{0, 0, 0}
};

/*
 * Main
 */

void main() {
	// Enable FPU
	FPUEnable();
	FPULazyStackingEnable();

	// Setup the clock to run from crystal
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//Configure GPIO port for LEDs and PBs
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LP_LEDS);
	/* Unlock F0 */
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	/* Unlock F0 */
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, LP_SWS);
	GPIOPadConfigSet(GPIO_PORTF_BASE, LP_SWS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	// Configure UART0 STDIO
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, 115200, 16000000);

	// Print Welcome Message
	UARTprintf("+---------------> T/TS Shell <---------------+\n");
	UARTprintf("|  Use the 'help' command to get a list of   |\n");
	UARTprintf("|  available commands on the shell.          |\n");
	UARTprintf("+--------------------------------------------+\n");
	
	// Main Function
	while(1) {
		char uartRx[128];

		// Print prompt
		UARTprintf("\n> ");

		// Get input
		UARTgets(uartRx, 128);
		CmdLineProcess(uartRx);

	}
}















