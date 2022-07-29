#include <stdint.h>

#ifndef SOUNDFX_H
#define SOUNDFX_H



#define TUNE_SILENCE        0
#define TUNE_GOT_TROPHY     1
#define TUNE_TREASURE       2
#define TUNE_NEXTLEVEL      3
#define TUNE_WALKING        4
#define TUNE_JUMPING        5
#define TUNE_GOT_SOMETHING  6
#define TUNE_EXPLOSION      7
#define TUNE_OUCH           8
#define TUNE_FLYING         9
#define TUNE_FALLING        10
#define TUNE_TOJETPACK      11

typedef struct soundfx_tune_struct {
    char name[64];
    uint8_t *raw;
    uint64_t sz;

} soundfx_tune_t;

typedef struct soundfx_struct {
    soundfx_tune_t tunes[12];
    int tune_idx;
    int tune_offset;

    void (*play)(struct soundfx_struct *sfx, int tune);
    void (*stop)(struct soundfx_struct *sfx);
    void (*resume)(struct soundfx_struct *sfx);
} soundfx_t;

soundfx_t* soundfx_create();
void soundfx_destroy(soundfx_t *sfx);

#endif
