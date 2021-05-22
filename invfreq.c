#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "invfreq.h"

uint16_t engine_sndCount;
int16_t engine_currSoundPlaying;
uint8_t sound_limiter, *sndPriorities;
char game_ext[4];
const uint32_t pitAltCounter = 0x1000;
char string_buf[0x50];

typedef struct invfreq_block_struct {
    uint16_t pcm[4096];
    size_t len;
} invfreq_block_t;

double invfreq_wavelength(double frequency) {
    return (double)1 / frequency;
}

int invfreq_samples_per_waveform(double frequency, int samplerate) {
    double wavelength = invfreq_wavelength(frequency);
    double samples_d = wavelength * (double) samplerate;

    return (int)samples_d;
}

int invfreq_samples_per_half_waveform(double frequency, int samplerate) {
    int samples = invfreq_samples_per_waveform(frequency, samplerate);
    return samples / 2;
}

// Returns frequency value in hz from inverse-frequency format value
double invfreq_to_freq(uint16_t invfreq) {
    double invfreq_d = (double)invfreq;
    double frequency = ((double)PC_PIT_RATE / invfreq_d);

    return frequency;
}


void invfreq_decode(uint16_t invfreq, invfreq_block_t *out) {
    uint32_t samples_per_beep = pitAltCounter * ENGINE_SNDDEFAULTSAMPLERATE / PC_PIT_RATE;
    uint32_t samples_counter = 0;
    double freq = invfreq_to_freq(invfreq);

    out->len = 0;
    memset(out->pcm, 0x00, sizeof(out->pcm));
    printf("sizeofpcm: %ld \n", sizeof(out->pcm));

    int samples_halfwave = invfreq_samples_per_half_waveform(freq, ENGINE_SNDDEFAULTSAMPLERATE);
    printf("samples per half wave: %d \n", samples_halfwave);

    int samples_per_switch = (int)samples_halfwave;

    uint16_t beepWaveVal = 4095; // 32767 - Too loud

    uint32_t beepHalfCycleCounter = 0;
    int tup = 0;
    int tdown = 0;

    for (samples_counter = 0; samples_counter < samples_per_beep * 5; samples_counter++) {
        out->pcm[samples_counter] = beepWaveVal;
        out->len++;

        if (beepWaveVal <= 8001) { 
            tup++;
        }else{
            tdown++;
        }
        //fwrite(currSndChunkPtr, 2, 1, outfp);

        beepHalfCycleCounter++;
        if (beepHalfCycleCounter >= samples_per_switch) {
            beepHalfCycleCounter = 0;
            beepWaveVal = -beepWaveVal;
            if (samples_counter >= samples_per_beep && beepWaveVal <= 8001) {
                break;
            }
        }
    }
    printf("This beep was %d up, %d down \n", tup, tdown);
}

void invfreq_decode_soundfx(uint16_t *data, uint8_t *out, size_t out_max_sz) {
    int invfreq_idx = 0;
    double freq;
    invfreq_block_t block;
    int pcmval = 4095; // 32767 - Too loud
    int outidx = 0;
    int i;

    while(data[invfreq_idx] != 0xFFFF) {
        invfreq_decode(data[invfreq_idx], &block);
        invfreq_idx++;

        for (i = 0; i < block.len; i++) {
            out[outidx*2] = block.pcm[i] & 0xFF;
            out[outidx*2 + 1] = (block.pcm[i] >> 8) & 0xFF;
            outidx++;
        }
    }
}
