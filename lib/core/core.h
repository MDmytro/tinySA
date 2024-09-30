#ifndef CORE_H
#define CORE_H

#include "hal.h"

void update_frequencies(void);
bool sweep(bool break_on_operation);

#ifdef TINYSA4
extern freq_t ULTRA_MAX_FREQ;           // Start of harmonic mode
extern freq_t ULTRA_THRESHOLD;
extern freq_t NORMAL_MAX_FREQ;
extern int max2871;
#endif

void clear_backup(void);

#ifdef TINYSA4
#else
extern bool has_esd;
#endif

// Main
#ifdef TINYSA4
#define DELAY_TIMER GPTD4
#else
#define DELAY_TIMER GPTD14
#endif

extern const GPTConfig gpt4cfg;

int shell_printf(const char *fmt, ...);

void dac_init(void);

#ifdef TINYSA4__
#undef PULSE
#define PULSE {   palClearPad(GPIOB, 14); my_microsecond_delay(2); palSetPad(GPIOB, 14); }
#else
#undef PULSE
#define PULSE
#endif

extern uint16_t hwid;
extern uint16_t hw_if;

extern const freq_t v5_2_correction_frequency[CORRECTION_SIZE][CORRECTION_POINTS];
extern const float v5_2_correction_value[CORRECTION_SIZE][CORRECTION_POINTS];

extern const float v5_2_harmonic_lna_level_offset; // should be in correction table now -7;        // Depends on where the transition to harmonic is done!!!!!! TODO find best frequency to transition to harmonic
extern const float v5_2_harmonic_level_offset; // should be in correction table now -7;        // Depends on where the transition to harmonic is done!!!!!! TODO find best frequency to transition to harmonic
extern const float v5_2_lna_level_offset;

extern const freq_t v4_6_correction_frequency[CORRECTION_SIZE][CORRECTION_POINTS];
extern const float  v4_6_correction_value[CORRECTION_SIZE][CORRECTION_POINTS];

extern const float v4_6_harmonic_lna_level_offset; // should be in correction table now -7;        // Depends on where the transition to harmonic is done!!!!!! TODO find best frequency to transition to harmonic
extern const float v4_6_harmonic_level_offset; // should be in correction table now -7;        // Depends on where the transition to harmonic is done!!!!!! TODO find best frequency to transition to harmonic
extern const float v4_6_lna_level_offset;

#ifdef TINYSA4
void set_freq_boundaries(void);
#endif

void load_LCD_properties(void);

void shell_init_connection(void);
void set_sweep_points(uint16_t points);

#ifdef __AUDIO__
extern const I2SConfig i2sconfig;
#endif

void set_sweep_frequency(int type, freq_t freq);
bool shell_check_connect(void);

int VNAShell_readLine(char *line, int max_size);
void VNAShell_executeLine(char *line);

#endif
