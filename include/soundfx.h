#include <stdint.h>

#ifndef SOUNDFX_H
#define SOUNDFX_H



#define TUNE_SILENCE     0
#define TUNE_GOT_TROPHY  1

typedef struct soundfx_tune_struct {
    char name[64];
    uint8_t *raw;
    uint64_t sz;

} soundfx_tune_t;

typedef struct soundfx_struct {
    soundfx_tune_t tunes[2];
    int tune_idx;
    int tune_offset;

    void (*play)(struct soundfx_struct *sfx, int tune);
    void (*stop)(struct soundfx_struct *sfx);
} soundfx_t;

soundfx_t* soundfx_create();
void soundfx_destroy(soundfx_t *sfx);

#endif
