#ifndef CORE_H
#define CORE_H

void update_frequencies(void);
bool sweep(bool break_on_operation);

#ifdef TINYSA4
extern freq_t ULTRA_MAX_FREQ;           // Start of harmonic mode
extern freq_t ULTRA_THRESHOLD;
extern freq_t NORMAL_MAX_FREQ;
extern int max2871;
#endif

void clear_backup(void);

#endif
