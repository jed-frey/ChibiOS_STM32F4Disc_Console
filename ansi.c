/*
 * ansi.c
 *
 *  Created on: Nov 20, 2013
 *      Author: Jed Frey
 */
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "usbcfg.h"
#include "shell.h"
#include "ansi.h"
#include "status.h"

void ansiXY(BaseSequentialStream *chp, unsigned int x, unsigned int y) {
  chprintf(chp, "\e[%d;%dH",y,x);
}

void ansiClear(BaseSequentialStream *chp) {
	chprintf(chp, "\e[2J\e[H");
}

void cmd_ansiColorTest(BaseSequentialStream *chp, int argc, char *argv[]) {
	(void)argv;
	(void)argc;
	unsigned int i,j, k;
	for (i=0;i<8;i++) {
		for (k=0;k<8;k++) {
			for (j=0;j<8;j++) {
				if (false) {
					chprintf(chp, "\x1b[%d;%dm",i+30,j);
				} else {
					chprintf(chp, "\e[%dm",i+30);
					chprintf(chp, "\e[%dm",j);
					chprintf(chp, "\e[%dm",k+40);
				}
				chprintf(chp, "Hello");
				chprintf(chp, "\x1b[0m ");
			}
			chprintf(chp,"\r\n");
		}
	}
}
