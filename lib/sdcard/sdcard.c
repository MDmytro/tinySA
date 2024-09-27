#include "sdcard.h"

#ifdef __USE_SD_CARD__
#ifdef __DISABLE_HOT_INSERT__
uint16_t sd_card_inserted_at_boot = false;
#endif
#endif
