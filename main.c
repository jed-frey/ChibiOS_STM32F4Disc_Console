/*
 ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"
#include "shell.h"
#include "ansi.h"
#include "status.h"

#define SHELL_WA_SIZE   THD_WA_SIZE(2048)
#define TEST_WA_SIZE    THD_WA_SIZE(256)

/* Virtual serial port over USB.*/
SerialUSBDriver SDU1;

static const ShellCommand commands[] = {
	{"mem", cmd_mem},
	{"threads",cmd_threads},
	{"status",cmd_status},
	{"ansiTest",cmd_ansiColorTest},
	{"echo",cmd_echo},
	{"getKey",cmd_getKey},
	{"extended",cmd_ExtendedAscii},
	{"box",cmd_box},
	{"draw",cmd_draw},
	{NULL,NULL}
};

static const ShellConfig shell_cfg0 = {(BaseSequentialStream *)&SDU1, commands};
static const ShellConfig shell_cfg1 = {(BaseSequentialStream *)&SD1,  commands};
static const ShellConfig shell_cfg2 = {(BaseSequentialStream *)&SD2,  commands};
static const ShellConfig shell_cfg3 = {(BaseSequentialStream *)&SD3,  commands};
static const ShellConfig shell_cfg4 = {(BaseSequentialStream *)&SD4,  commands};
static const ShellConfig shell_cfg5 = {(BaseSequentialStream *)&SD5,  commands};
static const ShellConfig shell_cfg6 = {(BaseSequentialStream *)&SD6,  commands};

/*
 * Application entry point.
 */
int main(void) {
	Thread *shelltp0 = NULL;
	Thread *shelltp2 = NULL;

	unsigned int i = 0;
	/*
	 * System initializations.
	 * - HAL initialization, this also initializes the configured device drivers
	 *   and performs the board-specific initializations.
	 * - Kernel initialization, the main() function becomes a thread and the
	 *   RTOS is active.
	 */
	halInit();
	chSysInit();
	/*
	 * Initializes a serial-over-USB CDC driver.
	 */
	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);
	// Activate all serial drivers.
	sdStart(&SD1, NULL);
	sdStart(&SD2, NULL);
	sdStart(&SD3, NULL);
	sdStart(&SD4, NULL);
	sdStart(&SD5, NULL);
	sdStart(&SD6, NULL);
	// Activates the UART driver 1, PA9(TX) and PA10(RX) are routed to USART1.
	palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(7));
	// Activates the UART driver 2, PA2(TX) and PA3(RX) are routed to USART2.
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
	// Activates the UART driver 3, PB10(TX) and PB11(RX) are routed to USART3.
	palSetPadMode(GPIOB, 10, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOB, 11, PAL_MODE_ALTERNATE(7));
	// Activates the UART driver 4, PC10(TX) and PC11(RX) are routed to UART4.
	palSetPadMode(GPIOC, 10, PAL_MODE_ALTERNATE(8));
	palSetPadMode(GPIOC, 11, PAL_MODE_ALTERNATE(8));
	// Activates the UART driver 5, PC12(TX) and PD2(RX) are routed to UART5.
	palSetPadMode(GPIOC, 12, PAL_MODE_ALTERNATE(8));
	palSetPadMode(GPIOD, 2, PAL_MODE_ALTERNATE(8));
	// Activates the UART driver 6, PC6(TX) and PC7(RX) are routed to USART6.
	palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(8));
	palSetPadMode(GPIOC, 7, PAL_MODE_ALTERNATE(8));

	/*
	 * Activates the USB driver and then the USB bus pull-up on D+.
	 * Note, a delay is inserted in order to not have to disconnect the cable
	 * after a reset.
	 */
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(250);
	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);

	shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
	shellCreate(&shell_cfg2, SHELL_WA_SIZE, NORMALPRIO);
	shellCreate(&shell_cfg3, SHELL_WA_SIZE, NORMALPRIO);
	shellCreate(&shell_cfg4, SHELL_WA_SIZE, NORMALPRIO);
	shellCreate(&shell_cfg5, SHELL_WA_SIZE, NORMALPRIO);
	shellCreate(&shell_cfg6, SHELL_WA_SIZE, NORMALPRIO);
	/*
	 * Normal main() thread activity, in this demo it just performs
	 * a shell respawn upon its termination.
	 */
	while (true) {
		if (!shelltp0) {
			if (SDU1.config->usbp->state == USB_ACTIVE) {
				/* Spawns a new shell.*/
				shelltp0 = shellCreate(&shell_cfg0, SHELL_WA_SIZE, NORMALPRIO);
			}
		} else {
			/* If the previous shell exited.*/
			if (chThdTerminated(shelltp0)) {
				/* Recovers memory of the previous shell.*/
				chThdRelease(shelltp0);
				shelltp0 = NULL;
			}
		}
		chThdSleepMilliseconds(1000);
	}
}
