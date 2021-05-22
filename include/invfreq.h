#ifndef _INVFREQ_H_
#define _INVFREQ_H_

/**
 * Utility functions 
 */
#define PC_PIT_RATE 1193182
#define ENGINE_SNDDEFAULTSAMPLERATE 44100


void invfreq_decode_soundfx(uint16_t *data, uint8_t *out, size_t out_max_sz);

#endif
