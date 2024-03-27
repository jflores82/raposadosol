// SDCC Includes //
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// SMS Includes //
#include "lib/SMSlib.h"

// Header //
#include "vdp.h"

// VDP Stuff //
void vdp_config(void) {
	SMS_VDPturnOnFeature(VDPFEATURE_HIDEFIRSTCOL);
	SMS_VRAMmemsetW(0, 0x0000, 16384); // CLEAR VRAM
	// SMS_useFirstHalfTilesforSprites(true); // not used atm //
}

void vdp_vram_clear(void) { 
	SMS_VRAMmemsetW(0, 0x0000, 16384); // Clear VRAM //
}

void vdp_clear_sprites(void) { 
	SMS_initSprites();
	SMS_copySpritestoSAT();
}

void reset_display(void) {
	SMS_displayOff();
	SMS_waitForVBlank();
	SMS_displayOn();
}