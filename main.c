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

/* Virtual serial port over USB.*/
SerialUSBDriver SDU1;

/*
 * Application entry point.
 */
int main(void) {
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

	/*
	 * Normal main() thread activity, in this demo it just performs
	 * a shell respawn upon its termination.
	 */
	while (TRUE) {
		i++;
		if (SDU1.config->usbp->state == USB_ACTIVE) {
			chprintf((BaseSequentialStream *)&SDU1, "Serial USB: %d\r\n", i);
		}
		chprintf((BaseSequentialStream *)&SD1, "Serial 1: %d\r\n", i);
		chprintf((BaseSequentialStream *)&SD2, "Serial 2: %d\r\n", i);
		chprintf((BaseSequentialStream *)&SD3, "Serial 3: %d\r\n", i);
		chprintf((BaseSequentialStream *)&SD4, "Serial 4: %d\r\n", i);
		chprintf((BaseSequentialStream *)&SD5, "Serial 5: %d\r\n", i);
		chprintf((BaseSequentialStream *)&SD6, "Serial 6: %d\r\n", i);
		chThdSleepMilliseconds(1000);
	}
}
