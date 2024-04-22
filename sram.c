// SDCC Includes //
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// SMS Includes //
#include "lib/SMSlib.h"
#include "sram.h"

void checkHiScore(int hiscore, int score) {
	if(score > hiscore) { 
		hiscore = score; // set the new hi-score //
		SMS_enableSRAM(); // enable save ram //
		savegame->points = hiscore; // write the score to SRAM
		SMS_disableSRAM();
		return;
	}
	return;
}

int loadHiScore(void) {
	int hiscore;	
	SMS_enableSRAM();
	hiscore = savegame->points;
	SMS_disableSRAM();
	return hiscore;
}

void clearHiScore(void) { 
	int hiscore = 0;
	SMS_enableSRAM();
	savegame->points = hiscore;
	SMS_disableSRAM();
}