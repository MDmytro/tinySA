/*
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * The software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include "ch.h"
#include "hal.h"

#include "usbcfg.h"
#include "nanovna.h"

#include <chprintf.h>
#include <string.h>
#include <math.h>

#include "core.h"

int main(void)
{
  halInit();
  /*
   *  Initiate 1/8 micro second timer
   */
  gptStart(&DELAY_TIMER, &gpt4cfg);
  gptPolledDelay(&DELAY_TIMER, 80); // 10 us delay

  PULSE
  chSysInit();
  PULSE
 /*
  * Initialize RTC library (not used ChibiOS RTC module)
  */
  #ifdef __USE_RTC__
    rtc_init();
  #endif
  PULSE

  //palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(1) | PAL_STM32_OTYPE_OPENDRAIN);
  //palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(1) | PAL_STM32_OTYPE_OPENDRAIN);
#ifdef __VNA__
  i2cStart(&I2CD1, &i2ccfg);
  si5351_init();
#endif

#ifdef TINYSA3
  has_esd = ((palReadPort(GPIOB) & (1<<12)) ? false : true );
  bool has_new_switch = ((palReadPort(GPIOA) & (1<<5)) ? false : true ) || ((palReadPort(GPIOB) & (1<<12)) ? false : true );
#endif


#ifdef __SI4432__
 /*
  * Powercycle the RF part to reset SI4432
  */

#if 0
  palClearPad(GPIOB, GPIOA_RF_PWR);
  chThdSleepMilliseconds(200);
#endif
  palSetPad(GPIOB, GPIO_RF_PWR);
  chThdSleepMilliseconds(500);
#endif

#if 0
  palSetPadMode(GPIOA, 9, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOA, 10, PAL_MODE_OUTPUT_PUSHPULL);
  int s;

  adc_stop();

  // drive high to low on Y line (coordinates from left to right)
  palSetPad(GPIOB, GPIOB_YN);
  palClearPad(GPIOA, GPIOA_YP);
  // Set Y line as output
  palSetPadMode(GPIOB, GPIOB_YN, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, GPIOA_YP, PAL_MODE_OUTPUT_PUSHPULL);
  // Set X line as input
  palSetPadMode(GPIOB, GPIOB_XN, PAL_MODE_INPUT);        // Hi-z mode
  palSetPadMode(GPIOA, GPIOA_XP, PAL_MODE_INPUT_ANALOG); // <- ADC_TOUCH_X channel


  while (1) {
//      palSetPad(GPIOA, 10);
//      shell_printf("%d\n\r", adc_single_read(ADC_CHSELR_CHSEL9));
//      palClearPad(GPIOA, 10);
      shell_printf("%d\n\r", adc_single_read(ADC_TOUCH_X));
  }
#endif

  spi_init();
  PULSE

//#ifdef __PWM__
//  pwm_init();
//  pwm_start(2000);
//  pwm_stop();
//#endif
  /*
   * Set LCD display brightness (use DAC2 for control)
   * Starting DAC1 driver, setting up the output pin as analog as suggested by the Reference Manual.
   */
  dac_init();
  adc_init();
  PULSE
  DAC->CR|= DAC_CR_EN1 | DAC_CR_EN2; // Use DAC: CH1 and CH2
  #ifdef  __LCD_BRIGHTNESS__
    lcd_setBrightness(DEFAULT_BRIGHTNESS);
  #endif
  DAC->DHR12R1 = 0;                  // Setup DAC: CH1 value

  /*
 * SPI LCD Initialize
 */
#ifdef TINYSA4
  (void) get_hw_version_text();     // This sets the hwid, must be before ili9341_init;
#endif
  ili9341_init();
  PULSE

#ifdef TINYSA4
  ili9341_set_foreground(LCD_FG_COLOR);
  PULSE
  ili9341_drawstring_7x13("Starting...", 0, 0);
  PULSE
#ifdef __DISABLE_HOT_INSERT__
  sd_card_inserted_at_boot = SD_Inserted();
#endif
  disk_initialize(0);
  SI4463_init_rx();             // Needed bacause calldata recall may change SI4463 parameters
  PULSE
//  SD_PowerOn();
#endif


/*
 *  Initiate 1 micro second timer
 */
 gptStart(&DELAY_TIMER, &gpt4cfg);
 gptPolledDelay(&DELAY_TIMER, 80); // 10 us delay

/* restore config */
#ifdef TINYSA3
  if (has_new_switch)
    config.switch_offset = -5.0;
#endif
  int reset_state = btn_side();
#ifdef TINYSA4
  if (hwid >= 100) {
    max2871 = true;
    memcpy(config.correction_frequency, v5_2_correction_frequency, sizeof(config.correction_frequency));
    memcpy(config.correction_value, v5_2_correction_value, sizeof(config.correction_value));
    config.harmonic_level_offset = v5_2_harmonic_level_offset;
    config.harmonic_lna_level_offset = v5_2_harmonic_lna_level_offset;
    if (hwid >= 103)
      config.frequency_IF1 = DEFAULT_IF_PLUS;
   ULTRA_MAX_FREQ      = 7300000000ULL;
  } else {
    if (hw_if) {
      memcpy(config.correction_frequency, v4_6_correction_frequency, sizeof(config.correction_frequency));
      memcpy(config.correction_value, v4_6_correction_value, sizeof(config.correction_value));
      config.harmonic_level_offset = v4_6_harmonic_level_offset;
      config.harmonic_lna_level_offset = v4_6_harmonic_lna_level_offset;
      ULTRA_MAX_FREQ      = 5450000000ULL;
    } else
      ULTRA_MAX_FREQ      = 5340000000ULL;
  }
  if (hw_if) {
    ULTRA_THRESHOLD = 800000000ULL;
    NORMAL_MAX_FREQ = 900000000ULL;
  } else {
    ULTRA_THRESHOLD = 700000000ULL;
    NORMAL_MAX_FREQ = 800000000ULL;
  }
  set_freq_boundaries();
#endif
  if (!reset_state) {
    if(config_recall()) {
      clear_backup();
    }
  }
#ifdef TINYSA4
  set_freq_boundaries();        // In case harmonic_start was set
#endif
  config.cor_am = 0;        // Should be removed from config
  config.cor_nfm = 0;
  config.cor_wfm = 0;
  ili9341_flip(config.flip);

  if (reset_state || caldata_recall(0) == -1) {
    load_LCD_properties();
  }
#ifdef __ULTRA__
  else
    ultra_start = (config.ultra_start == ULTRA_AUTO ? ULTRA_THRESHOLD : config.ultra_start);
#endif


/*
 * Init Shell console connection data (after load config for settings)
 */

  shell_init_connection();

  set_sweep_points(POINTS_COUNT);

  #ifdef __AUDIO__
/*
 * I2S Initialize
 */
  tlv320aic3204_init();
  i2sInit();
  i2sObjectInit(&I2SD2);
  i2sStart(&I2SD2, &i2sconfig);
  i2sStartExchange(&I2SD2);
#endif

  setup_sa();
//  if (setting.mode != -1) {
//    menu_mode_cb(setting.mode,0);
//  }
  redraw_frame();
#ifdef TINYSA3
  set_mode(M_HIGH);
  set_sweep_frequency(ST_STOP, (freq_t) 30000000);
  sweep(false);
  osalThreadSleepMilliseconds(100);

  set_mode(M_LOW);
  set_sweep_frequency(ST_STOP, (freq_t) 4000000);
  sweep(false);
#endif
#ifdef __WATCHDOG__
  static const WDGConfig scan_wdgcfg = {
    STM32_IWDG_PR_256,
    STM32_IWDG_RL(1 * (40000 / 256)), /* 1 second */
    STM32_IWDG_WIN_DISABLED
  };
  wdgInit();
  wdgStart(&WDGD1, &scan_wdgcfg);
#endif
  if (reset_state|| (caldata_recall(0) == -1)) {
    load_LCD_properties();
  }
  ui_mode_normal();
  if ((!reset_state) && !(config._mode & _MODE_DONT_SAVE_STATE) ) {
    backup_t b;
    uint32_t *f = &backup;
    uint32_t *t = (uint32_t *)&b;
    int i = USED_BACKUP_SIZE;
    while (i--)
      *t++ = *f++;
    uint8_t *c = (uint8_t *)&b;
    int ci = sizeof(backup_t)-1;
    uint8_t checksum = 0x55;
    while (ci--) {
      checksum ^= *c++;
    }
    if (b.data.checksum == checksum) {
#ifdef TINYSA4       // Set mode not working reliably
      set_mode(b.data.mode);
      switch (b.data.mode) {
      case M_LOW:
      case M_HIGH:
        break;
      case M_GENLOW:
        menu_push_submenu(menu_mode);
        menu_push_submenu(menu_lowoutputmode);
        break;
      case M_GENHIGH:
        menu_push_submenu(menu_mode);
        menu_push_submenu(menu_highoutputmode);
        break;
      }
      set_harmonic(b.data.harmonic);
#endif
      set_external_gain(b.data.external_gain/2);
      if (b.data.frequency0 != 0 || b.data.frequency1 != 0) {
        if (b.data.mode <= M_HIGH){
          set_sweep_frequency(ST_START, b.data.frequency0);
          set_sweep_frequency(ST_STOP, b.data.frequency1);
        } else {
          set_sweep_frequency(ST_CW, (b.data.frequency0 + b.data.frequency1)/2);
          set_sweep_frequency(ST_SPAN, (b.data.frequency1 - b.data.frequency0));
          ui_mode_menu();
        }
        if (b.data.attenuation == 0) {
          set_auto_attenuation();
        } else {
          set_attenuation((b.data.attenuation - 1)/2.0);
        }
        if (b.data.reflevel == 0) {
          set_auto_reflevel(true);
        } else {
          set_auto_reflevel(false);
          user_set_reflevel((float)(b.data.reflevel-140));
        }
        if (b.data.RBW == 0) {
          setting.rbw_x10 = 0;
        } else {
          set_RBW(force_rbw(b.data.RBW-1));
        }
      }
    }
  }
  set_refer_output(-1);
  //  ui_mode_menu();       // Show menu when autostarting mode

  /*
   * Set LCD display brightness (use DAC2 for control)
   * Starting DAC1 driver, setting up the output pin as analog as suggested by the Reference Manual.
   */
#ifdef  __LCD_BRIGHTNESS__
    lcd_setBrightness(config._brightness);
  #else
    DAC->DHR12R2 = config.dac_value; // Setup DAC: CH2 value
  #endif

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO-1, Thread1, NULL);

  while (1) {
//    if (SDU1.config->usbp->state == USB_ACTIVE) {
    if (shell_check_connect()) {
#ifdef VNA_SHELL_THREAD
#if CH_CFG_USE_WAITEXIT == FALSE
#error "VNA_SHELL_THREAD use chThdWait, need enable CH_CFG_USE_WAITEXIT in chconf.h"
#endif
      thread_t *shelltp = chThdCreateStatic(waThread2, sizeof(waThread2),
                                            NORMALPRIO + 1,
                                            myshellThread, NULL);
      chThdWait(shelltp);
#else
      shell_printf(VNA_SHELL_NEWLINE_STR"tinySA Shell"VNA_SHELL_NEWLINE_STR);
      shell_printf(VNA_SHELL_PROMPT_STR);
      do {
        if (VNAShell_readLine(shell_line, VNA_SHELL_MAX_LENGTH))
          VNAShell_executeLine(shell_line);
        else
          chThdSleepMilliseconds(200);
//      } while (SDU1.config->usbp->state == USB_ACTIVE);
      } while (shell_check_connect());
#endif
    }
    chThdSleepMilliseconds(1000);
  }
}
