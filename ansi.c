/*
 * ansi.c
 *
 *  Created on: Nov 20, 2013
 *      Author: Jed Frey
 */

#include "ansi.h"
#include "chprintf.h"
#include "shell.h"

static void ansiXY(BaseSequentialStream *chp, unsigned int x, unsigned int y) {
  chprintf(chp, "\e[%d;%dH",y,x);
}

static void ansiClear(BaseSequentialStream *chp) {
  chprintf(chp, "\e[2J\e[H");
}
