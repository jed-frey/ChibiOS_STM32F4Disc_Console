/*
 * ansi.h
 *
 *  Created on: Nov 20, 2013
 *      Author: Jed Frey
 */

#ifndef ANSI_H_
#define ANSI_H_

void ansiXY(BaseSequentialStream *chp, unsigned int x, unsigned int y);
void ansiClear(BaseSequentialStream *chp);
void cmd_ansiColorTest(BaseSequentialStream *chp, int argc, char *argv[]);
#endif /* ANSI_H_ */
