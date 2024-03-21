// Silver Fox //
// Coded by tibonev using sverx's sms dev kit //
// tibonev: http://classicgames.com.br //
// All assets, code, music etc: made by tibonev //

// SDCC Includes //
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// SMS Includes //
#include "lib/SMSlib.h"
#include "PSGlib/PSGlib.h"

// Assets (gfx and music) //
#include "bank2.h"

// Init Variables //
unsigned int pl_x = 100; 			// player x axis position //
unsigned int pl_y = 100; 			// player y axis position //
unsigned int pl_lim_left = 30;		// player limit on the left border of the screen //
unsigned int pl_lim_right = 200;	// player limit on the right border of the screen //
unsigned int pl_lim_top = 20;		// player limit on the top border of the screen //
unsigned int pl_lim_bottom = 150;	// player limit on the bottom border of the screen //
unsigned int pl_dir = 0;			// player direcion (0 = up, 1 = left, 2 = right, 3 = down)
unsigned int pl_spd = 1;			// player speed (0 = stationary, 1 = regular, 2 = boosted)
unsigned int pl_spd_regular = 2;    // how fast is regular speed;
unsigned int pl_spd_boost = 3; 		// how fast is boosted speed;
char lives = 3;						// player lives //
int score = 0;						// player score //
int hiscore = 0;					// player hi-score //

unsigned int en_anim = 0;			// used for animate the pellets
unsigned int en_lim_top = 20;		// used to animate the turrets 
unsigned int en_lim_bot = 150;		// used to animate the turrets 
unsigned int en_l_y = 20;			// used to animate the turrets 
unsigned int en_r_y = 20;			// used to animate the turrets 
unsigned int en_l_dir = 0;			// used to animate the turrets 
unsigned int en_r_dir = 1;			// used to animate the turrets  
unsigned int en_s_l = 0;			// used to animate the enemy fire (from the left turret);
unsigned int en_s_l_x = 20;			// used to animate the enemy fire (from the left turret);
unsigned int en_s_l_y = 0;			// used to animate the enemy fire (from the left turret);
unsigned int en_s_r = 0;			// used to animate the enemy fire (from the right turret);
unsigned int en_s_r_x = 240;		// used to animate the enemy fire (from the right turret);
unsigned int en_s_r_y = 0;			// used to animate the enemy fire (from the right turret);

unsigned int level_pellet_num = 0;			// how many pellets are in a level 
unsigned int level_pellet_collected = 0; 	// how many pellets have been collected 
int level_fixed_current = 1;				// current level of the fixed world
int pellets_x_y[32][2];						// array used to draw/check collision (max of 32 pellet sprites, 2 coordinates per sprite (x,y))

unsigned int level_walls_num = 0;
int walls_x_y[64][2];						// walls position, max of 64 tiles, and 2 coordinates per tile (x,y);
int walls_col_dir = -1;


unsigned int bg_scroll_x = 0;				// used to control the background scroll //
unsigned int bg_scroll_y = 0;				// used to control the background scroll //

int gamestate = 0; 					// used to control the game flow //	// 0 = initial / title screen //
									// 1 = gameplay // 	// 2 = game over // // 3 = level completed //
									// 4 = player dies // // 5 = game paused //
int gamemode = 1; 					// 1 - fixed world / 0 - random world //
						
bool paused = false;
int wall_pellet_col = 0;

unsigned int level_counter = 0;
unsigned int frame_counter = 0;
						

// Tools //
int rand_num(int lb, int ub) {
	int ret;
	ret = rand() % (ub - lb + 1) + lb;
	return ret;
}

void reset_enemy_variables(void) { 
	en_l_y = rand_num(en_lim_top, en_lim_bot);
	en_l_dir = 	0;
	en_s_l = 	0;
	en_s_l_x = 	20;
	en_s_l_y = 	0;

	en_r_y = rand_num(en_lim_top+10, en_lim_bot-10);
	en_r_dir = 	1;
	en_s_r = 	0;
	en_s_r_x = 	240;
	en_s_r_y = 	0;
}

void reset_player_variables(void) {
	pl_x = 100;
	pl_y = 100;
	pl_dir = 0;
}

void reset_level_arrays(void) { 
	for(int i = 0; i < 50;i++) { 
		walls_x_y[i][0] = 400;
		walls_x_y[i][1] = 400;
		pellets_x_y[i][0] = 0;
		pellets_x_y[i][1] = 0;
	}
}



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

// Sprite Tiles
void level_load_sprites(void) {
	SMS_loadPSGaidencompressedTiles(level_sprites_psgcompr, 256);
	SMS_loadSpritePalette(level_sprites_bin);
}

// Player Related Stuff //
void player_draw(void) {

	unsigned int tile_1 = 2;
	unsigned int tile_2 = 3;
	unsigned int tile_3 = 4;
	unsigned int tile_4 = 5;

	// select which tiles to load as the ship main sprite (4 tiles), based on the direction you're going //
	switch(pl_dir) { 
		case 0: 
			tile_1 = 256;
			tile_2 = 257;
			tile_3 = 269;
			tile_4 = 270;
		break;

		case 1:
			tile_1 = 262;
			tile_2 = 263;
			tile_3 = 275;
			tile_4 = 276;
		break;

		case 2:
			tile_1 = 258;
			tile_2 = 259;
			tile_3 = 271;
			tile_4 = 272;
		break;

		case 3:
			tile_1 = 260;
			tile_2 = 261;
			tile_3 = 273;
			tile_4 = 274;
		break;
	}

	// Add the correct tiles as sprites as the correct positions //
	SMS_addSprite(pl_x,     pl_y,     tile_1);
	SMS_addSprite(pl_x + 8, pl_y,     tile_2);
	SMS_addSprite(pl_x,     pl_y + 8, tile_3);
	SMS_addSprite(pl_x + 8, pl_y + 8, tile_4);

}

void player_movement(void) {
	// set bounderies // 
	if((pl_x < pl_lim_left) && (pl_dir == 1)) {
		pl_spd = 0; 
		return;
	}
	if((pl_y < pl_lim_top) && (pl_dir == 0)) {
		pl_spd = 0;
		return;
	}

	if((pl_x > pl_lim_right) && (pl_dir == 2)) {
		pl_spd = 0; 
		return;
	}
	if((pl_y > pl_lim_bottom) && (pl_dir == 3)) {
		pl_spd = 0;
		return;
	}

	// move //
	if(pl_spd != 0 ) { 
		switch(pl_dir) { 
			case 0:
				pl_y -= pl_spd;
			break;

			case 1:
				pl_x -= pl_spd;
			break;

			case 2:
				pl_x += pl_spd;
			break;

			case 3:
				pl_y += pl_spd;
			break;
		}
	}
}

// Controller Operations //
void player_control(void) { 
	unsigned int key=SMS_getKeysStatus();

	if(key & PORT_A_KEY_UP && walls_col_dir != 0) 	{ pl_dir = 0; pl_spd = pl_spd_regular; }
	if(key & PORT_A_KEY_DOWN && walls_col_dir != 3)	{ pl_dir = 3; pl_spd = pl_spd_regular; }
	if(key & PORT_A_KEY_LEFT && walls_col_dir != 1)	{ pl_dir = 1; pl_spd = pl_spd_regular; }
	if(key & PORT_A_KEY_RIGHT && walls_col_dir != 2)	{ pl_dir = 2; pl_spd = pl_spd_regular; }
	if(key & PORT_A_KEY_1)	{ if(pl_spd != 0) { pl_spd = pl_spd_boost; } }
}

// Player-Pellet Collision //
void player_pellet_collision(void) { 
	int pel_x = 0; // just for easy reading //
	int pel_y = 0;
	for(int i = 0; i < level_pellet_num; i++) {
		pel_x = pellets_x_y[i][0];
		pel_y = pellets_x_y[i][1];

		// collision bounderies //
		if(	pl_x < pel_x + 8 &&
    		pl_x + 16 > pel_x &&
    		pl_y < pel_y + 8 &&
    		pl_y + 16 > pel_y) {
				pellets_x_y[i][0] = 200; //remove pellet//
				pellets_x_y[i][1] = 200;
				level_pellet_collected++;
				PSGSFXPlay(pickup_psg,SFX_CHANNEL2);
				score += 10;
			}
	}
	
}

void wall_pellet_collision(int wall_x, int wall_y) { 
	int pel_x = 0; // just for easy reading //
	int pel_y = 0;
	for(int i = 0; i < level_pellet_num; i++) {
		pel_x = pellets_x_y[i][0];
		pel_y = pellets_x_y[i][1];

		// collision bounderies //
		if(	wall_x < pel_x + 8 &&
    		wall_x + 16 > pel_x &&
    		wall_y < pel_y + 8 &&
    		wall_y + 16 > pel_y) {
				wall_pellet_col = 1;
				return;
			} 
	}
	wall_pellet_col = 0;	
	return;
}

// Player-Enemy Shot Collision //
void player_enshot_collision(void) {
	// Left Shot //
	if(	pl_x < en_s_l_x + 8 &&
    	pl_x + 16 > en_s_l_x &&
    	pl_y < en_s_l_y + 8 &&
    	pl_y + 16 > en_s_l_y) 
		{
			lives--;
			gamestate = 4;
		}

	// Right Shot //
	if(	pl_x < en_s_r_x + 8 &&
    	pl_x + 16 > en_s_r_x &&
    	pl_y < en_s_r_y + 8 &&
    	pl_y + 16 > en_s_r_y) 
		{
			lives--;
			gamestate = 4;
			
		}
}

// Player-Walls Collision //
void player_walls_collision(void) { 
	int wall_x = 0; // just for easy reading //
	int wall_y = 0;
	walls_col_dir = -1;
	for(int i = 0; i < level_walls_num; i++) {
		wall_x = walls_x_y[i][0];
		wall_y = walls_x_y[i][1];

		// collision bounderies //
		if(	pl_x < wall_x + 8 &&
    		pl_x + 16 > wall_x &&
    		pl_y < wall_y + 8 &&
    		pl_y + 16 > wall_y) {
				pl_spd = 0;
				walls_col_dir = pl_dir;
				switch(walls_col_dir) { 
					case 0:
						pl_y += 1; 
					break;
					
					case 1:
						pl_x += 1;
					break;
					
					case 2:
						pl_x -= 1;
					break;
					
					case 3:
						pl_y -= 1; 
					break;
				}
			}
	}
}



// Backgrounds //
void black_bg_load(void) { 
	SMS_loadPSGaidencompressedTiles(bg_psgcompr, 0); // files, tilefrom
	SMS_loadSTMcompressedTileMap(0,0,bg_stmcompr); // x,y, files
	SMS_loadBGPalette(bg_bin); // palette file //
}

void bg_scroll(void) {
	
	switch(pl_dir) {
		case 0:
			if(pl_spd != 0) { SMS_setBGScrollY(bg_scroll_y--); }
		break;
		case 1:
			if(pl_spd != 0) { SMS_setBGScrollX(bg_scroll_x++); }
		break;
		case 2:
			if(pl_spd != 0) { SMS_setBGScrollX(bg_scroll_x--); }
		break;
		case 3:
			if(pl_spd != 0) { SMS_setBGScrollY(bg_scroll_y++); }
		break;
	}
}

// Wall sprites //
void walls_draw(void) { 
	for(int i = 0; i < level_walls_num; i++) {
		SMS_setTileatXY(walls_x_y[i][0] / 8 , walls_x_y[i][1] / 8, 100);
	}
}

// Level Stuff //
void level_bg_load(void) {
	int x;
	int y;
	SMS_loadPSGaidencompressedTiles(tiles_psgcompr, 100); // files, tilefrom
	//SMS_loadSTMcompressedTileMap(0,0,bg2_stmcompr); // x,y, files
	int tiles_pallete = 1;
	if(gamemode == 0) { tiles_pallete = rand_num(1,4); }
	switch(tiles_pallete) { 
		case 1:
			SMS_loadBGPalette(tiles_bin);
		break;
		case 2:
			SMS_loadBGPalette(tiles2_bin);
		break;
		case 3:
			SMS_loadBGPalette(tiles3_bin);
		break;
		case 4:
			SMS_loadBGPalette(tiles4_bin);
		break;
	}
	
	SMS_loadPSGaidencompressedTiles(timer_psgcompr,200); // timer tiles //

	// Background Color //
	for(x = 0; x < 32; x++) {
		for(y = 0; y < 22; y++) {
			SMS_setTileatXY(x,y,104);
		}
	}

	// Background Stars //
	for(x = 0; x < 15; x++) { 
		int bgx = rand_num(2,30);
		int bgy = rand_num(2,20);
		int tile = rand_num(101,103);
		SMS_setTileatXY(bgx,bgy,tile);
	}

	// Vertical Walls //
	for(y = 0; y < 22; y++) {
		SMS_setTileatXY(1,y,108);
		SMS_setTileatXY(31,y,108);
	}

	// Horizontal Walls //
	for(x = 0; x < 32; x++) {
		SMS_setTileatXY(x,0,107);
		SMS_setTileatXY(x,21,107);
	}
	
	// Corners //
	SMS_setTileatXY(1,0,105);
	SMS_setTileatXY(1,21,105);
	SMS_setTileatXY(31,0,106);
	SMS_setTileatXY(31,21,106);
		
}

void level_load(int level, int seed) {

	int w_x;
	int w_y;
	int i = 0;
	int j = 0;
	level_counter = 10;
	frame_counter = 0;
	level_bg_load(); // Load the Background Tiles //

	SMS_setTileatXY(3,22,210);  
	SMS_setTileatXY(4,22,211);  
	SMS_setTileatXY(5,22,212);  
	SMS_setTileatXY(6,22,213);  
	SMS_setTileatXY(7,22,214);  
		
	reset_level_arrays();

	if(level == 1) { 
		level_pellet_collected = 0;
		en_l_y = 10; 
		en_l_dir = 1;
		en_r_y = 30;
		en_r_dir = 0;
		
		walls_x_y[0][0] = 80; walls_x_y[0][1] = 40;
		walls_x_y[1][0] = 88; walls_x_y[1][1] = 40;
		walls_x_y[2][0] = 96; walls_x_y[2][1] = 40;
		walls_x_y[3][0] = 104; walls_x_y[3][1] = 40;
		walls_x_y[4][0] = 152; walls_x_y[4][1] = 40;
		walls_x_y[5][0] = 160; walls_x_y[5][1] = 40;
		walls_x_y[6][0] = 168; walls_x_y[6][1] = 40;
		walls_x_y[7][0] = 176; walls_x_y[7][1] = 40;
		walls_x_y[8][0] = 104; walls_x_y[8][1] = 48;
		walls_x_y[9][0] = 152; walls_x_y[9][1] = 48;
		walls_x_y[10][0] = 104; walls_x_y[10][1] = 56;
		walls_x_y[11][0] = 152; walls_x_y[11][1] = 56;
		walls_x_y[12][0] = 104; walls_x_y[12][1] = 64;
		walls_x_y[13][0] = 152; walls_x_y[13][1] = 64;
		walls_x_y[14][0] = 104; walls_x_y[14][1] = 72;
		walls_x_y[15][0] = 152; walls_x_y[15][1] = 72;
		walls_x_y[16][0] = 104; walls_x_y[16][1] = 80;
		walls_x_y[17][0] = 152; walls_x_y[17][1] = 80;
		walls_x_y[18][0] = 56; walls_x_y[18][1] = 88;
		walls_x_y[19][0] = 64; walls_x_y[19][1] = 88;
		walls_x_y[20][0] = 72; walls_x_y[20][1] = 88;
		walls_x_y[21][0] = 80; walls_x_y[21][1] = 88;
		walls_x_y[22][0] = 88; walls_x_y[22][1] = 88;
		walls_x_y[23][0] = 96; walls_x_y[23][1] = 88;
		walls_x_y[24][0] = 104; walls_x_y[24][1] = 88;
		walls_x_y[25][0] = 152; walls_x_y[25][1] = 88;
		walls_x_y[26][0] = 160; walls_x_y[26][1] = 88;
		walls_x_y[27][0] = 168; walls_x_y[27][1] = 88;
		walls_x_y[28][0] = 176; walls_x_y[28][1] = 88;
		walls_x_y[29][0] = 184; walls_x_y[29][1] = 88;
		walls_x_y[30][0] = 192; walls_x_y[30][1] = 88;
		walls_x_y[31][0] = 200; walls_x_y[31][1] = 88;
		level_walls_num=32;
		
		pellets_x_y[0][0] = 32; pellets_x_y[0][1] = 15;
		pellets_x_y[1][0] = 208; pellets_x_y[1][1] = 15;
		pellets_x_y[2][0] = 112; pellets_x_y[2][1] = 40;
		pellets_x_y[3][0] = 144; pellets_x_y[3][1] = 56;
		pellets_x_y[4][0] = 112; pellets_x_y[4][1] = 72;
		pellets_x_y[5][0] = 144; pellets_x_y[5][1] = 88;
		pellets_x_y[6][0] = 112; pellets_x_y[6][1] = 104;
		pellets_x_y[7][0] = 144; pellets_x_y[7][1] = 112;
		pellets_x_y[8][0] = 112; pellets_x_y[8][1] = 128;
		pellets_x_y[9][0] = 32; pellets_x_y[9][1] = 136;
		pellets_x_y[10][0] = 56; pellets_x_y[10][1] = 136;
		pellets_x_y[11][0] = 184; pellets_x_y[11][1] = 136;
		pellets_x_y[12][0] = 208; pellets_x_y[12][1] = 136;
		pellets_x_y[13][0] = 40; pellets_x_y[13][1] = 144;
		pellets_x_y[14][0] = 48; pellets_x_y[14][1] = 144;
		pellets_x_y[15][0] = 192; pellets_x_y[15][1] = 144;
		pellets_x_y[16][0] = 200; pellets_x_y[16][1] = 144;
		pellets_x_y[17][0] = 40; pellets_x_y[17][1] = 152;
		pellets_x_y[18][0] = 48; pellets_x_y[18][1] = 152;
		pellets_x_y[19][0] = 192; pellets_x_y[19][1] = 152;
		pellets_x_y[20][0] = 200; pellets_x_y[20][1] = 152;
		pellets_x_y[21][0] = 32; pellets_x_y[21][1] = 160;
		pellets_x_y[22][0] = 56; pellets_x_y[22][1] = 160;
		pellets_x_y[23][0] = 184; pellets_x_y[23][1] = 160;
		pellets_x_y[24][0] = 208; pellets_x_y[24][1] = 160;
		level_pellet_num=25;

	}
	
	// Level Random //
	if(level == 0) {
		srand(seed);  // re-seed the random number generator each time //
		
		level_pellet_num = rand_num(5,20);
		level_walls_num = rand_num(5,15);
		en_l_y = rand_num(en_lim_top, en_lim_bot);
		en_l_dir = 0;
		en_r_y = rand_num(en_lim_top+10, en_lim_bot-10);
		en_r_dir = 1;
		level_pellet_collected = 0;
		// Pellets //
		for(int i = 0; i < level_pellet_num; i++ ) {
			pellets_x_y[i][0] = rand_num(pl_lim_left, pl_lim_right);
			pellets_x_y[i][1] = rand_num(pl_lim_top, pl_lim_bottom);
		}
		
		// Walls //
		while(i < level_walls_num) { 
			w_x = rand_num(pl_lim_left, pl_lim_right);
			w_y = rand_num(pl_lim_top, pl_lim_bottom);
			wall_pellet_collision(w_x, w_y);
			//wall_pellet_col = 1;
			if(wall_pellet_col == 0) { 
				walls_x_y[i][0] = w_x;
				walls_x_y[i][1] = w_y;
				
			}
			i++;
		}

	}
	walls_draw();
	gamestate = 1; // goto gameplay //
}

void level_reset(void) { 
	reset_display();
	reset_level_arrays();
	level_load_sprites();
	reset_enemy_variables();
	reset_player_variables();
	frame_counter = 0;
	level_counter = 10;
}

// Pellet Sprites //
void pellets_draw(void) {
	unsigned int pellet_anim = 264;
	if(en_anim < 40) { pellet_anim = 264; }
	if(en_anim == 40) { pellet_anim = 265; }
	if(en_anim > 80) { en_anim = 0; }

	for(int i = 0; i < level_pellet_num; i++) {
		SMS_addSprite(pellets_x_y[i][0], pellets_x_y[i][1], pellet_anim);
	}
	en_anim++;
}

// Turrets / Turrets Fire //
void turrets_draw(void) {
	// Left Turret //
	SMS_addSprite(16, en_l_y,		266);
	SMS_addSprite(16, en_l_y+8, 	279);

	// Right Turret //
	SMS_addSprite(240, en_r_y,		267);
	SMS_addSprite(240, en_r_y+8,	280);

}

void turrets_movement(void) {
	// Left Turret //
	if(en_l_dir == 0 && en_l_y < en_lim_bot) { en_l_y++; }
	if(en_l_dir == 0 && en_l_y >= en_lim_bot) { en_l_dir = 1; en_l_y--; }
	if(en_l_dir == 1 && en_l_y < en_lim_bot) { en_l_y--; }
	if(en_l_dir == 1 && en_l_y <= en_lim_top) { en_l_dir = 0; }

	// Right Turret //
	if(en_r_dir == 0 && en_r_y < en_lim_bot) { en_r_y++; }
	if(en_r_dir == 0 && en_r_y >= en_lim_bot) { en_r_dir = 1; en_r_y--; }
	if(en_r_dir == 1 && en_r_y < en_lim_bot) { en_r_y--; }
	if(en_r_dir == 1 && en_r_y <= en_lim_top) { en_r_dir = 0; }
}

void turrets_shoot(int turret) {
	// 0 = Left Turret //
	if(turret == 0) { 
		if(en_s_l == 0 ) { 
			switch(en_l_dir) { 
				case 0:
					en_s_l_y = en_l_y + 32;
				break;
				case 1:
					en_s_l_y = en_l_y - 32;
				break;
			}
			SMS_addSprite(en_s_l_x, en_s_l_y, 276);
			PSGSFXPlay(en_shot_psg,SFX_CHANNEL3);
			en_s_l = 1;
		}
	}

	// 1 = Right Turret //
	if(turret == 1) { 
		if(en_s_r == 0 ) { 
			switch(en_r_dir) { 
				case 0:
					en_s_r_y = en_r_y + 32;
				break;
				case 1:
					en_s_r_y = en_r_y - 32;
				break;
			}
			SMS_addSprite(en_s_r_x, en_s_r_y, 276);
			PSGSFXPlay(en_shot_psg,SFX_CHANNEL3);
			en_s_r = 1;
		}
	}
}

void turrets_check_shoot(void) {
	if(en_l_y == pl_y) {
		turrets_shoot(0);
	}

	if(en_r_y == pl_y) {
		turrets_shoot(1);
	}
}

void turrets_shot_anim(void) {
	
	// Left Turret //
	if(en_s_l == 1) { en_s_l_x++; SMS_addSprite(en_s_l_x, en_s_l_y, 277); }
	if(en_s_l_x > 250) { en_s_l = 0; en_s_l_x = 0; return; }
	
	// Right Turret //
	if(en_s_r == 1) { en_s_r_x--; SMS_addSprite(en_s_r_x, en_s_r_y, 278); }
	if(en_s_r_x < 10) { en_s_r = 0; en_s_r_x = 240; return; }
	
}

// Player Explosion //
void explosion_draw(void) {
	int tile1e = 282;
	int tile2e = 283;
	int tile3e = 294;
	int tile4e = 295;

	int i = 0;
	PSGSFXPlay(explosion_psg,SFX_CHANNEL3);
	for(int a = 0; a < 5; a++) { 
		SMS_initSprites();
		pellets_draw(); 
		walls_draw();
		turrets_draw();
		SMS_waitForVBlank();
		SMS_copySpritestoSAT();

		for(i = 0; i < 5; i++) { 
			SMS_addSprite(pl_x, pl_y, tile1e);
			SMS_addSprite(pl_x+8, pl_y, tile2e);
			SMS_addSprite(pl_x, pl_y+8, tile3e);
			SMS_addSprite(pl_x+8, pl_y+8, tile4e);
			SMS_waitForVBlank();
			PSGSFXFrame();
			SMS_copySpritestoSAT();		
		}

		tile1e += 2;
		tile2e += 2;
		tile3e += 2;
		tile4e += 2;
	}

	for(i = 0; i < 50; i++) { 
		SMS_initSprites();
		pellets_draw(); 
		walls_draw();
		turrets_draw();
		SMS_waitForVBlank();
		SMS_copySpritestoSAT();
	}
	PSGSFXStop();
	return;

}

// Timer on Screen //
void drawTimer(void) { 
	switch(level_counter) {
		case 20:
			SMS_setTileatXY(8,22,202);
			SMS_setTileatXY(9,22,200);
		break;
		case 19:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,209);
		break;
		case 18:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,208);
		break;
		case 17:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,207);
		break;
		case 16:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,206);
		break;
		case 15:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,205);
		break;
		case 14:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,204);
		break;
		case 13:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,203);
		break;
		case 12:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,202);
		break;
		case 11:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,201);
		break;
		case 10:
			SMS_setTileatXY(8,22,201);
			SMS_setTileatXY(9,22,200);
		break;
		case 9:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,209);
		break;
		case 8:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,208);
		break;
		case 7:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,207);
		break;
		case 6:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,206);
		break;
		case 5:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,205);
		break;
		case 4:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,204);
		break;
		case 3:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,203);
		break;
		case 2:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,202);
		break;
		case 1:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,201);
		break;
		case 0:
			SMS_setTileatXY(8,22,200);
			SMS_setTileatXY(9,22,200);
		break;

	}
}

// Text Screens // 
void doTitleScreen(void) {
	int a = 0;
	char temp[5];
	
	black_bg_load();
	SMS_setBGScrollX(0);
	SMS_setBGScrollY(0);
	vdp_clear_sprites();
	SMS_autoSetUpTextRenderer();
	SMS_printatXY(10,11,"RAPOSA DO SOL");
	SMS_printatXY(11,15,"PRESS START");

	SMS_printatXY(11,2,"HI:");
	sprintf((char*)temp,"%u",hiscore);
	SMS_printatXY(14,2, temp);

	PSGPlay(raposa_1_psg);

	SMS_displayOn();
	
	while(a == 0) { 
		unsigned int key=SMS_getKeysStatus();
		if(key & PORT_A_KEY_1)	{ a = 1; }
		PSGFrame();
		SMS_waitForVBlank();

	}
	PSGStop();
	level_load_sprites();
	return;
}

void doMenuScreen(void) { 
	int a = 0;
	gamemode = 1;

	black_bg_load();
	vdp_clear_sprites();
	SMS_setBGScrollX(0);
	SMS_setBGScrollY(0);
	SMS_autoSetUpTextRenderer();
	SMS_printatXY(10,8, "SELECT GAME MODE");
	SMS_printatXY(10,10,"> FIXED WORLD");
	SMS_printatXY(10,12,"  RANDOM WORLD");
	SMS_waitForVBlank();

	while(a == 0) {
		unsigned int key=SMS_getKeysStatus();
		if(key & PORT_A_KEY_UP && gamemode !=1) { 
			PSGSFXPlay(menu_tick_psg,SFX_CHANNEL2);
			gamemode = 1;
			SMS_printatXY(10,10,"> FIXED WORLD");
			SMS_printatXY(10,12,"  RANDOM WORLD");
		}
		if(key & PORT_A_KEY_DOWN && gamemode !=0) {
			gamemode = 0;
			PSGSFXPlay(menu_tick_psg,SFX_CHANNEL2);
			SMS_printatXY(10,10,"  FIXED WORLD");
			SMS_printatXY(10,12,"> RANDOM WORLD");
		}
		if(key & PORT_A_KEY_1) { a = 1; }
		PSGSFXFrame();
		SMS_waitForVBlank();
					
	}
	return;

}

void doLivesScreen(void) { 
	int i = 0;
	int lives_tile = lives + 16;
	
	black_bg_load();
	vdp_clear_sprites();
	SMS_setBGScrollX(0);
	SMS_setBGScrollY(0);
	SMS_autoSetUpTextRenderer();
	SMS_printatXY(10,10,"SHIPS REMAINING");
	SMS_setNextTileatXY(7,10);
	SMS_setTile(lives_tile);
	PSGSFXStop();
	while(i < 200) { 
		SMS_waitForVBlank();
		i++;
	}
	level_bg_load();
	level_load_sprites();
	return;
}

void doGameOver(void) {
	int a = 0;
	char temp[5];

	SMS_displayOff();
	// vdp_vram_clear();
	black_bg_load();
	vdp_clear_sprites();
	SMS_setBGScrollX(0);
	SMS_setBGScrollY(0);
	SMS_autoSetUpTextRenderer();
	SMS_printatXY(11,5,"GAME OVER");
	SMS_printatXY(11,10, "SCORE");
	sprintf((char*)temp,"%u",score);
	SMS_printatXY(12,12, temp);
	SMS_printatXY(11,2,"HI:");
	sprintf((char*)temp,"%u",hiscore);
	SMS_printatXY(14,2, temp);

	while(a < 200) { 
		SMS_waitForVBlank();
		a++; 
	}
	level_load_sprites();
	return;
}

void doNewGame(void) { 
	score = 0;
	lives = 3;
	reset_level_arrays();
	reset_enemy_variables();
	reset_player_variables();

}

void checkPause(void) {
	if (SMS_queryPauseRequested()) {
    	SMS_resetPauseRequest();
      	paused = !paused;

		if(paused) { 
			PSGSFXStop();
			gamestate = 5; 
		} else { 
			gamestate = 1; 
		}
	}
}

//struct sram { unsigned int points; };
//struct sram *savegame = (struct sram*)(&SMS_SRAM);

void checkHiScore(void) {
	if(score > hiscore) { 
		hiscore = score; // set the new hi-score //
		//SMS_enableSRAM(); // enable save ram //
		//savegame->points = hiscore;
		//SMS_disableSRAM();
		return;
	}
	return;
}

void loadHiScore(void) {
	//SMS_enableSRAM();
	//hiscore = savegame->points;
	//SMS_disableSRAM();
}

void clearHiScore(void) { 
	hiscore = 0;
	//SMS_enableSRAM();
	//savegame->points = hiscore;
	//SMS_disableSRAM();
}

// Main Program //
void main(void) {

	int c = 30; // Used to seed the rand command //
	
	vdp_config();

	vdp_vram_clear();

	clearHiScore();
	loadHiScore();

	black_bg_load();

	doTitleScreen();
	doMenuScreen();
	doLivesScreen();
		
	SMS_displayOn();

	level_load(gamemode,c); // load the first level //
	SMS_resetPauseRequest();
	PSGSFXStop();

	while(true) { 

		// Main Gameplay Loop //
		while(gamestate == 1) {

			// Check if the pause button has been pressed //
			checkPause();

			// Graphics //
			SMS_initSprites();
			player_draw();
			pellets_draw(); 
			//walls_draw();
			turrets_draw();
			//SMS_finalizeSprites(); // deprecated according to the documentation //

			// Collision Checks //
			player_pellet_collision();
			player_enshot_collision();
			player_walls_collision();
				
			// Player Movement //
			player_control(); // this is the joy input
			player_movement(); // this checks bounderies/direction and speed.

			turrets_movement();
			turrets_check_shoot();
			turrets_shot_anim();
			
			//bg_scroll(); // scrolls the background, duh?

			drawTimer();

			
					
			// For the random_level_generator //
			c++;
			if(c == 25600) { c = 0; }

			// timer
			frame_counter++;
			if(frame_counter == 60) { 
				level_counter--;
				frame_counter = 0;
			}
			
			if(level_counter == 0) { 
				lives--;
				gamestate = 4;
			}
						
			// SMS Loop Stuff //
			PSGFrame();
			PSGSFXFrame();
			SMS_waitForVBlank();
			SMS_copySpritestoSAT();
			

			if(level_pellet_collected >= level_pellet_num) { 
				gamestate = 3; // you passed the level //
			}

			if(lives <= 0) {
				gamestate = 2; // game over yeah! //
			}

			
		}

		// Title Screen //
		if(gamestate == 0 ) {
			doNewGame();
			doTitleScreen();
			doMenuScreen();
			doLivesScreen();
			level_reset();
			level_load(gamemode,c);
			gamestate = 1;
		}

		// Game Over //
		if(gamestate == 2) { 
			checkHiScore();
			doGameOver();
			reset_enemy_variables();
			reset_player_variables();
			gamestate = 0;
		}

		// Level Complete // 
		if(gamestate == 3) {
			doLivesScreen();
			level_reset();
			level_fixed_current++;
			if(gamemode == 1) { 
				if(level_fixed_current > 1) { gamestate = 0; doNewGame(); doTitleScreen(); }	
				level_load(level_fixed_current,c); 
			}
			if(gamemode == 0) { level_load(0,c); }
		}

		// Player Died // 
		if(gamestate == 4) { 
			explosion_draw();
			reset_enemy_variables();
			reset_player_variables();
			doLivesScreen();
			walls_draw();
			if(level_counter == 0) { level_counter = 10; }
			frame_counter = 0; 
			gamestate = 1;
		}

		// Game Paused //
		if(gamestate == 5) { 
			checkPause();
		}

	}
}