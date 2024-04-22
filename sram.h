struct sram { unsigned int points; };
struct sram *savegame = (struct sram*)(&SMS_SRAM);

void checkHiScore(int hiscore, int score);
int loadHiScore(void);
void clearHiScore(void);
