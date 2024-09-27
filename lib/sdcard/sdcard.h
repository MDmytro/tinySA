#ifndef SD_CARD_H
#define SD_CARD_H

#ifdef __USE_SD_CARD__
#ifdef __DISABLE_HOT_INSERT__
extern uint16_t sd_card_inserted_at_boot;
#endif
// Enable SD card console command
#define ENABLE_SD_CARD_CMD
#endif

#endif
