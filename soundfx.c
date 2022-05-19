#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif
#include <SDL.h>

#include "soundfx.h"
#include "invfreq.h"

SDL_AudioDeviceID g_audio_dev;

// SPEED TOJETPACK: 0x0A
// SPEED HITWALL: 0x0A
uint16_t silence[] = {
    0x0000, 0xFFFF
};

// SPEED: 0x0F
uint16_t got_trophy[] = {
    0x0BF4, 0x0BF4, 0x0BF4, 0x0BF4, 0x0BB8, 0x0BB8, 0x0B7C, 0x0B7C,
    0x0B40, 0x0B40, 0x0B40, 0x0B04, 0x0B04, 0x0B04, 0x0B04, 0x0AC8,
    0x0AC8, 0x0AC8, 0x0AC8, 0x0AC8, 0x0AC8, 0x0B04, 0x0B04, 0x0B04,
    0x0B04, 0x0B40, 0x0B7C, 0x0B7C, 0x0BB8, 0x0BB8, 0x0C30, 0x0C6C,
    0x0CA8, 0x0CE4, 0x0CE4, 0x0CE4, 0x0D20, 0x0D20, 0x0D20, 0x0D20,
    0x0D20, 0x0D20, 0x0D20, 0x0D20, 0x0D20, 0x0D20, 0x0CE4, 0x0CE4,
    0x0C6C, 0x0C6C, 0x0C30, 0x0BF4, 0x0B7C, 0x0B7C, 0x0B04, 0x0AC8,
    0x0A8C, 0x0A50, 0x0A14, 0x0A14, 0x099C, 0x099C, 0x0960, 0x0924,
    0x0924, 0x08E8, 0x08AC, 0x08AC, 0x0870, 0x0870, 0x0834, 0x0834,
    0x07F8, 0x07F8, 0x07BC, 0x07BC, 0x07BC, 0x07BC, 0x07BC, 0x0780,
    0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780, 0x0780,
    0x0780, 0x0780, 0x0780, 0x07BC, 0x07BC, 0x07BC, 0x07F8, 0x0834,
    0x0870, 0x08AC, 0x0924, 0x0960, 0x0A14, 0x0A50, 0x0B04, 0x0B40,
    0x0BB8, 0x0BF4, 0x0C30, 0x0C6C, 0x0CA8, 0x0CE4, 0x0D20, 0x0D5C,
    0x0D98, 0x0DD4, 0x0DD4, 0x0E10, 0x0E10, 0x0E10, 0x0E4C, 0x0E4C,
    0x0E4C, 0x0E4C, 0x0E4C, 0x0E4C, 0x0E4C, 0x0E4C, 0x0E4C, 0x0E10,
    0x0DD4, 0x0DD4, 0x0D98, 0x0D5C, 0x0CE4, 0x0CA8, 0x0C6C, 0x0C6C,
    0x0C30, 0x0BF4, 0x0BF4, 0x0BF4, 0x0BB8, 0x0BB8, 0x0BB8, 0x0B40,
    0x0B40, 0x0B40, 0x0B40, 0x0B40, 0x0B40, 0x0B40, 0x0B40, 0x0B40,
    0x0B7C, 0x0B7C, 0x0B7C, 0x0B7C, 0x0B7C, 0x0BB8, 0x0BF4, 0x0BF4,
    0x0BF4, 0x0C30, 0x0C6C, 0x0C6C, 0x0CA8, 0x0CA8, 0x0D20, 0x0D5C,
    0x0DD4, 0x0E10, 0x0E4C, 0x0EC4, 0x0F3C, 0x0FB4, 0x1068, 0x10A4,
    0x111C, 0x1194, 0x11D0, 0x1248, 0x12C0, 0x12FC, 0x1374, 0x13B0,
    0x1428, 0x1464, 0x14A0, 0x14DC, 0x14DC, 0x1518, 0x1518, 0x1518,
    0x1518, 0x1518, 0x14DC, 0x14DC, 0x1464, 0x1464, 0x1428, 0x13EC,
    0x13EC, 0x13EC, 0x13B0, 0x13B0, 0x1374, 0x1374, 0x1374, 0x1374,
    0x1338, 0x1338, 0x1338, 0x1338, 0x1338, 0x1338, 0x1338, 0x1338,
    0x1338, 0x1338, 0x1338, 0x1338, 0x1338, 0x1338, 0x1338, 0x1338,
    0x1338, 0x1338, 0x1338, 0x1338, 0x13B0, 0x13B0, 0x13EC, 0x13EC,
    0x1464, 0x1428, 0x14A0, 0x14A0, 0x14DC, 0x1518, 0x1590, 0x15CC,
    0x1644, 0x16F8, 0x1770, 0x17AC, 0x1860, 0x189C, 0x1950, 0x19C8,
    0x1A7C, 0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF,
};

// SPEED: 0x0F
uint16_t treasure[] = {
    0x0528, 0x04B0, 0x0438, 0x0438, 0x03FC, 0x0438, 0x0474, 0x0474,
    0x04B0, 0x04B0, 0x0528, 0x05A0, 0x0618, 0x05A0, 0x04EC, 0x0438,
    0x0348, 0x030C, 0x02D0, 0x02D0, 0x02D0, 0x02D0, 0x02D0, 0x02D0,
    0x030C, 0x0348, 0x0384, 0x03C0, 0x0474, 0x04B0, 0x05A0, 0xFFFF,
};

// SPEED: 0x0F
uint16_t nextlevel[] = {
    0x2670, 0x2670, 0x2634, 0x2634, 0x25F8, 0x25F8, 0x25BC, 0x25BC,
    0x2580, 0x2580, 0x2544, 0x2508, 0x24CC, 0x24CC, 0x2454, 0x2454,
    0x23DC, 0x23DC, 0x2364, 0x2328, 0x22B0, 0x2274, 0x2238, 0x21FC,
    0x2148, 0x210C, 0x20D0, 0x2094, 0x1FE0, 0x1F68, 0x1EB4, 0x1E78,
    0x1DC4, 0x1D4C, 0x1C5C, 0x1B6C, 0x1AF4, 0x1A40, 0x198C, 0x189C,
    0x1518, 0x0FF0, 0x12C0, 0x1284, 0x120C, 0x0D20, 0x0000, 0x0BF4,
    0x111C, 0x0B7C, 0x0000, 0x1068, 0x0B04, 0x0000, 0x0FB4, 0x0F78,
    0x0A8C, 0x0F3C, 0x0A50, 0x0EC4, 0x0000, 0x0A14, 0x0E4C, 0x0000,
    0x0E4C, 0x09D8, 0x0000, 0x0E4C, 0x099C, 0x0E4C, 0x099C, 0x0000,
    0x0E88, 0x099C, 0x0EC4, 0x0F00, 0x099C, 0x0F3C, 0x0F78, 0x099C,
    0x0FB4, 0x0FF0, 0x099C, 0x1068, 0x099C, 0x0000, 0x099C, 0x099C,
    0x11D0, 0x09D8, 0x1248, 0x0A14, 0x0000, 0x0A50, 0x12C0, 0x0A8C,
    0x12C0, 0x0AC8, 0x0B04, 0x0000, 0x0B40, 0x0B7C, 0x0000, 0x0BB8,
    0x0C30, 0x10E0, 0x0CA8, 0x0DD4, 0x0000, 0x0FF0, 0x0F78, 0x0F00,
    0x0000, 0x0E88, 0x0000, 0x0E10, 0x0D98, 0x0000, 0x0D5C, 0x0CE4,
    0x0000, 0x0C6C, 0x0000, 0x0BF4, 0x0BB8, 0x0000, 0x0B7C, 0x0000,
    0x0B40, 0x0B04, 0x0000, 0x0000, 0x0B04, 0x0000, 0x0B04, 0x0000,
    0x0000, 0x09D8, 0x0960, 0x0000, 0x08E8, 0x0B04, 0x0834, 0x0000,
    0x0B04, 0x07BC, 0x0B40, 0x0000, 0x0744, 0x0B7C, 0x0000, 0x0708,
    0x06CC, 0x0C6C, 0x0CE4, 0x0690, 0x0000, 0x0690, 0x0EC4, 0x0690,
    0x0FF0, 0x1068, 0x0654, 0x111C, 0x1158, 0x0618, 0x1284, 0x0000,
    0x0618, 0x0000, 0x0000, 0x0618, 0x0000, 0x1194, 0x0618, 0x10A4,
    0x0618, 0x0F00, 0x0000, 0x0618, 0x0DD4, 0x0CE4, 0x0618, 0x0B40,
    0x0654, 0x0A50, 0x0654, 0x0924, 0x06CC, 0x08AC, 0x0000, 0x0834,
    0x0000, 0x0000, 0x07BC, 0x0000, 0x0000, 0x0744, 0x0000, 0x06CC,
    0x0000, 0x0690, 0x0000, 0x0000, 0x0690, 0x0000, 0x0000, 0x0690,
    0x0000, 0x0690, 0x0000, 0x0690, 0x0000, 0x0000, 0x06CC, 0x0618,
    0x05A0, 0x0000, 0x0528, 0x04B0, 0x0474, 0x0000, 0x03FC, 0x0000,
    0x099C, 0x03C0, 0x0000, 0x0A14, 0x0384, 0x0000, 0x0A8C, 0x0000,
    0x0348, 0x0AC8, 0x030C, 0x0AC8, 0x0AC8, 0x02D0, 0x02D0, 0x0000,
    0x0000, 0x02D0, 0x0000, 0x0A8C, 0x030C, 0x0A14, 0x0348, 0x0960,
    0x030C, 0x08AC, 0x0348, 0x07F8, 0x0000, 0x0384, 0x0654, 0x0000,
    0x03C0, 0x0000, 0x03FC, 0x0564, 0x0438, 0x0528, 0x04EC, 0x04EC,
    0x0474, 0x04B0, 0x0474, 0x0474, 0x0438, 0x0438, 0x03FC, 0x03C0,
    0x03C0, 0x0384, 0x0348, 0x0384, 0x0348, 0x0348, 0x02D0, 0x02D0,
    0x02D0, 0x02D0, 0x0294, 0x0258, 0x0258, 0x021C, 0x021C, 0x01E0,
    0x01E0, 0x01E0, 0x01A4, 0x0168, 0x0168, 0x0168, 0x0168, 0x012C,
    0x012C, 0x012C, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x00B4, 0x00B4,
    0x00B4, 0x00B4, 0x00B4, 0x0078, 0x0078, 0x0078, 0x0078, 0x0078,
    0x0078, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C,
    //0xFFFF,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x1AF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0xFFFF,
};

// SPEED: 0x05
uint16_t walking[] = {
    0x1AF4, 0x0000,0xFFFF
};

// SPEED: 0x0A
uint16_t flying[] = {
    0x20D0, 0xFFFF
};

// SPEED: 0x07
// Silence size:
// 1 = 330
// 3 = 1064
// 4 = 1372/1384
// 6 = 2056
uint16_t jumping[] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0B7C, 0x0000, 0x0000, 0x0000,
    0x0000, 0x099C, 0x0000, 0x0000, 0x0000, 0x0834, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0564, 0x0000, 0x04EC, 0x0000,
    0x0000, 0x0000, 0x0000, 0x03C0, 0x0000, 0x0384, 0x0000, 0x0000,
    0x0000, 0x0000, 0x02D0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x01E0, 0x0000, 0x01E0, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0168, 0x0000, 0x0000, 0x0000, 0x012C,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00B4, 0x0000, 0x0000,
    0x00B4, 0x00B4, 0x0000, 0x0000, 0x0000, 0x00B4, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00B4, 0x0000,
    0x0000, 0x0000, 0x0078, 0x0000, 0x0078, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0078, 0x0000, 0x0000, 0x0000, 0x0000, 0x0078, 0x0000,
    0x0078, 0x0000, 0x00B4, 0x0000, 0x0000, 0x0000, 0x00F0, 0x0000,
    0x012C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01E0, 0x0000,
    0x021C, 0x0000, 0x0000, 0x0000, 0x0258, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0384, 0x0000, 0x0000,
    0x0000, 0x0438, 0x0000, 0x0000, 0x0000, 0x04B0, 0x0000, 0x0000,
    0x0000, 0x0564, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0654,
    0x0000, 0x0000, 0x0000, 0x06CC, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x07F8, 0x0000, 0x0000, 0x0000, 0x0000, 0x08AC,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0A14, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0B40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0C30, 0x0000, 0x0000,
    0x0000, 0x0CA8, 0x0000, 0x0000, 0x0CE4, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0E10, 0x0000, 0x0E4C, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0F3C, 0x0000, 0x0000, 0x0FB4, 0x0000,
    0x0000, 0x0FF0, 0x102C, 0x0000, 0x0000, 0x0000, 0x10E0, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x120C, 0x0000, 0x1284,
    0x0000, 0x0000, 0x0000, 0x1374, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x14A0, 0x14DC, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x16F8, 0x1734, 0x0000,
    0x0000, 0x17E8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1A7C, 0x1AB8,
    0x0000, 0x0000, 0x1B6C, 0x0000, 0x1BA8, 0x0000, 0x0000, 0x1C5C,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1D88, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1FA4, 0x0000,
    0x0000, 0x201C, 0x0000, 0x0000, 0x0000, 0x2184, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x2544, 0x0000, 0x0000, 0x25F8, 0x0000, 0x26AC, 0xFFFF,
};

// Silence size
// 4 = 440
// 3 = 310
// 2 = 260
// SPEED: 0x19
uint16_t got_something[] = {
    0x1E00, 0x1D10, 0x1C5C, 0x1B6C, 0x1A7C, 0x1A40, 0x198C, 0x189C,
    0x17AC, 0x16F8, 0x15CC, 0x1590, 0x14DC, 0x1464, 0x13EC, 0x13B0,
    0x1338, 0x12FC, 0x12C0, 0x1284, 0x120C, 0x11D0, 0x11D0, 0x11D0,
    0x1194, 0x1194, 0x1158, 0x1158, 0x111C, 0x111C, 0x111C, 0x111C,
    0x111C, 0x111C, 0x111C, 0x111C, 0x111C, 0x111C, 0x111C, 0x1158,
    0x1158, 0x1158, 0x1194, 0x11D0, 0x11D0, 0x120C, 0x1248, 0x1248,
    0x1284, 0x1284, 0x12C0, 0x12FC, 0x1338, 0x1374, 0x13B0, 0x13EC,
    0x1464, 0x14A0, 0x14DC, 0x1554, 0x15CC, 0x1680, 0x1734, 0x1860,
    0x1914, 0x1A04, 0x1AB8, 0x1AF4, 0x1BA8, 0x1C5C, 0x1C5C, 0x1CD4,
    0x1D4C, 0x1D88, 0x1E3C, 0x1EB4, 0x1F68, 0x201C, 0x20D0, 0x210C,
    0x2184, 0x21C0, 0x22B0, 0x22EC, 0x23A0, 0x23DC, 0x2418, 0x2454,
    0x2490, 0x24CC, 0x24CC, 0x2544, 0x2580, 0x25BC, 0x2634, 0x2634,
    0x2670, 0x26AC, 0x26E8, 0x2724, 0x2724, 0x2760, 0x2760, 0x2760,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0,
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 
    0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x07D0, 0xFFFF, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF
};


// SPEED: 0x10
uint16_t explosion[] = {
    0x13B0, 0x189C, 0x15CC, 0x0000, 0x0000, 0x11D0, 0x0F78, 0x1B30,
    0x0000, 0x12C0, 0x0870, 0x13EC, 0x17E8, 0x0000, 0x16F8, 0x111C,
    0x10A4, 0x0000, 0x102C, 0x1CD4, 0x0000, 0x1464, 0x102C, 0x1A40,
    0x0000, 0x0000, 0x0E88, 0x0000, 0x1158, 0x0000, 0x0708, 0x0EC4,
    0x0000, 0x0000, 0x18D8, 0x0000, 0x0F00, 0x1A04, 0x1C98, 0x1C5C,
    0x0E4C, 0x0000, 0x0000, 0x0CE4, 0x0000, 0x0C6C, 0x0000, 0x0000,
    0x0000, 0x0000, 0x06CC, 0x1194, 0x0000, 0x0000, 0x1AF4, 0x0000,
    0x0000, 0x0000, 0x0EC4, 0x1CD4, 0x0000, 0x0AC8, 0x0C30, 0x1AF4,
    0x10E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0708,
    0x1AB8, 0x0000, 0x1C98, 0x0000, 0x0000, 0x0FB4, 0x0CE4, 0x0C30,
    0x1914, 0x1C20, 0x2328, 0x0000, 0x0000, 0x0000, 0x18D8, 0x08AC,
    0x0000, 0x1644, 0x0000, 0x0000, 0x0000, 0x1644, 0x0A8C, 0x24CC,
    0x0CA8, 0x1AB8, 0x16BC, 0x0000, 0x0000, 0x1068, 0x0000, 0x0000,
    0x0000, 0x1914, 0x0000, 0x1464, 0x0DD4, 0x17AC, 0x0000, 0x16BC,
    0x0000, 0x1518, 0x0FB4, 0x0000, 0x0000, 0x1608, 0x0000, 0x11D0,
    0x0000, 0x1BA8, 0x0000, 0x0FF0, 0x0000, 0x11D0, 0x1AF4, 0x0000,
    0x1E3C, 0x0000, 0x23DC, 0x0000, 0x0000, 0x0000, 0x0000, 0x1A40,
    0x0000, 0x1608, 0x0000, 0x0000, 0x0000, 0x0000, 0x1FE0, 0x0000,
    0x21FC, 0x0000, 0x0000, 0x0000, 0x0000, 0x1950, 0x0000, 0x19C8,
    0x1F68, 0x0000, 0x21C0, 0x2490, 0x0000, 0x1D4C, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x2238, 0x0000, 0x2490, 0x1B6C,
    0x0000, 0x0000, 0x19C8, 0x0000, 0x0000, 0x2490, 0x0000, 0x2238,
    0x0000, 0x0000, 0x26AC, 0x0000, 0x0000, 0x0000, 0x1A7C, 0x0000,
    0x16BC, 0x1F68, 0x0000, 0x26E8, 0x2418, 0x1A40, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1E00, 0x1A40,
    0x0000, 0x2508, 0x0000, 0x0000, 0x1F2C, 0x1BE4, 0x0000, 0x2580,
    0x24CC, 0x0000, 0x0000, 0x1644, 0x0000, 0x1DC4, 0x0000, 0x1A7C,
    0x0000, 0x0000, 0x0000, 0x1E78, 0x0000, 0x0000, 0x24CC, 0x2490,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1B6C, 0x2148, 0x1EF0,
    0x2328, 0x0000, 0x0000, 0x0000, 0x1CD4, 0x23A0, 0x1EB4, 0x0000,
    0x0000, 0x24CC, 0x2544, 0x0000, 0x0000, 0x0000, 0x20D0, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x2328, 0x26AC, 0x0000, 0x2508, 0xFFFF, 0xFFFF
};

// SPEED: 0x0C
uint16_t ouch[] = {
    0x2580, 0x22EC, 0x2058, 0x1F68, 0x1D10, 0x1BE4, 0x189C, 0x16F8,
    0x1608, 0x1590, 0x1338, 0x111C, 0x1068, 0x0F00, 0x0E4C, 0x0D5C,
    0x0CA8, 0x0BB8, 0x0B04, 0x0A50, 0x099C, 0x08E8, 0x0870, 0x0834,
    0x07F8, 0x07BC, 0x0780, 0x0744, 0x0708, 0x0690, 0x0654, 0x0618,
    0x05DC, 0x05DC, 0x05A0, 0x0564, 0x0564, 0x0564, 0x0528, 0x0528,
    0x0528, 0x0528, 0x0528, 0x0528, 0x0528, 0x0564, 0x05A0, 0x05DC,
    0x0618, 0x0654, 0x0690, 0x0708, 0x0780, 0x07F8, 0x0834, 0x08AC,
    0x0960, 0x0A50, 0x0B40, 0x0C6C, 0x0CA8, 0x0D5C, 0x1068, 0x11D0,
    0x18D8, 0x1C20, 0x1E3C, 0x1F2C, 0x210C, 0x21FC, 0x2274, 0x2364,
    0x23A0, 0x25BC, 0x2634, 0x26AC, 0x2760, 0x2760, 0xFFFF, 0xFFFF
};
// SPEED: 0x0A
uint16_t falling[] = {
    0x01E0, 0x01E0, 0x0000, 0x0000, 0x021C, 0x0000, 0x0000, 0x0258,
    0x0000, 0x0000, 0x0294, 0x0000, 0x0000, 0x0000, 0x02D0, 0x0000,
    0x0000, 0x0000, 0x02D0, 0x0000, 0x0000, 0x0000, 0x030C, 0x0000,
    0x0000, 0x030C, 0x0000, 0x0000, 0x0000, 0x0348, 0x0000, 0x0000,
    0x0384, 0x0000, 0x0000, 0x03C0, 0x0000, 0x0000, 0x03FC, 0x0000,
    0x0000, 0x03FC, 0x0000, 0x0000, 0x0438, 0x0000, 0x0000, 0x0000,
    0x0438, 0x0000, 0x0000, 0x0000, 0x0474, 0x0000, 0x0000, 0x0000,
    0x0000, 0x04B0, 0x0000, 0x0000, 0x0000, 0x0000, 0x04EC, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0528, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0564, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x05DC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0654,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0690, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0708, 0x0000, 0x0000, 0x0000, 0x0000, 0x0744, 0x0000,
    0x0000, 0x0000, 0x07BC, 0x0000, 0x0000, 0x0000, 0x07F8, 0x0000,
    0x0000, 0x0000, 0x0870, 0x0000, 0x0000, 0x0000, 0x08AC, 0x0000,
    0x0000, 0x0000, 0x0000, 0x08E8, 0x0000, 0x0000, 0x0000, 0x0960,
    0x0000, 0x0000, 0x0000, 0x0000, 0x099C, 0x0000, 0x0000, 0x0000,
    0x0A14, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0A50, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0A8C, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0B04, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0B40, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0B7C, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0BF4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0C30, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0C6C, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0CE4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0D5C, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0DD4, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0E4C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0EC4,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0F78, 0x0000, 0x0000,
    0x0000, 0x0000, 0x102C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x10E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x1194, 0x0000, 0x0000,
    0x0000, 0x0000, 0x1248, 0x0000, 0x0000, 0x0000, 0x0000, 0x1338,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x13EC, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x1518, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x1608, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x16F8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1824,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1914, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x1A40, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x1B30, 0x0000, 0x0000, 0x0000, 0x0000, 0x1C20,
    0x0000, 0x0000, 0x0000, 0x1D10, 0x0000, 0x0000, 0x1E00, 0x0000,
    0x0000, 0x0000, 0x0000, 0x1F2C, 0x0000, 0x0000, 0x0000, 0x2058,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x21C0, 0xFFFF, 0xFFFF
};

void soundfx_play(soundfx_t *sfx, int tune) {
    printf("[soundfx] soundfx_play tune: %d \n", tune);
    sfx->tune_idx = tune;
    sfx->tune_offset = 0;
    SDL_PauseAudioDevice(g_audio_dev, 0);
}

void soundfx_resume(soundfx_t *sfx) {
    printf("[soundfx] resume \n");
    SDL_PauseAudioDevice(g_audio_dev, 0);
}

void soundfx_stop(soundfx_t *sfx) {
    printf("[soundfx] stop \n");
    SDL_PauseAudioDevice(g_audio_dev, 1);
}

void game_audio_callback(void *data, uint8_t* stream, int len) {
    soundfx_t *sfx = (soundfx_t *)data;

    //printf("audio callback len: %d , tune: %d \n", len, sfx->tune_idx);
    if (sfx->tune_offset <= sfx->tunes[sfx->tune_idx].sz) {
        //printf("Playing tune: %d, offset: %d out of %lu \n", sfx->tune_idx, sfx->tune_offset,
        //        sfx->tunes[sfx->tune_idx].sz);
        memcpy(stream, &sfx->tunes[sfx->tune_idx].raw[sfx->tune_offset], len);
        sfx->tune_offset += len;
    }
    else {
        sfx->tune_idx = 0;
        sfx->tune_offset = 0;
        SDL_PauseAudioDevice(g_audio_dev, 1);
    }
    return;
}

soundfx_t* soundfx_create() {
    soundfx_t *sfx = malloc(sizeof(soundfx_t));
    SDL_AudioSpec audio_spec_want, audio_spec;

    strcpy(sfx->tunes[0].name, "silence");
    sfx->tunes[0].raw = malloc(4096 * 512);
    memset(sfx->tunes[0].raw, 0x00, 4096 * 512);
    sfx->tunes[0].sz = invfreq_decode_soundfx(silence, sfx->tunes[0].raw, 240);

    strcpy(sfx->tunes[1].name, "got_trophy");
    sfx->tunes[1].raw = malloc(4096 * 512);
    sfx->tunes[1].sz = invfreq_decode_soundfx(got_trophy, sfx->tunes[1].raw, 162);

    strcpy(sfx->tunes[2].name, "treasure");
    sfx->tunes[2].raw = malloc(4096 * 512);
    sfx->tunes[2].sz = invfreq_decode_soundfx(treasure, sfx->tunes[2].raw, 162);

    strcpy(sfx->tunes[3].name, "nextlevel");
    sfx->tunes[3].raw = malloc(4096 * 512);
    sfx->tunes[3].sz = invfreq_decode_soundfx(nextlevel, sfx->tunes[3].raw, 162);

    strcpy(sfx->tunes[4].name, "walking");
    sfx->tunes[4].raw = malloc(4096 * 512);
    sfx->tunes[4].sz = invfreq_decode_soundfx(walking, sfx->tunes[4].raw, 440);

    strcpy(sfx->tunes[5].name, "jumping");
    sfx->tunes[5].raw = malloc(4096 * 512);
    sfx->tunes[5].sz = invfreq_decode_soundfx(jumping, sfx->tunes[5].raw, 300);

    strcpy(sfx->tunes[6].name, "got_something");
    sfx->tunes[6].raw = malloc(4096 * 512);
    sfx->tunes[6].sz = invfreq_decode_soundfx(got_something, sfx->tunes[6].raw, 162);

    strcpy(sfx->tunes[7].name, "explosion");
    sfx->tunes[7].raw = malloc(4096 * 512);
    memset(sfx->tunes[7].raw, 0x00, 4096 * 512);
    sfx->tunes[7].sz = invfreq_decode_soundfx(explosion, sfx->tunes[7].raw, 240);

    strcpy(sfx->tunes[8].name, "ouch");
    sfx->tunes[8].raw = malloc(4096 * 512);
    memset(sfx->tunes[8].raw, 0x00, 4096 * 512);
    sfx->tunes[8].sz = invfreq_decode_soundfx(ouch, sfx->tunes[8].raw, 240);

    strcpy(sfx->tunes[9].name, "flying");
    sfx->tunes[9].raw = malloc(4096 * 512);
    memset(sfx->tunes[9].raw, 0x00, 4096 * 512);
    sfx->tunes[9].sz = invfreq_decode_soundfx(flying, sfx->tunes[9].raw, 240);

    strcpy(sfx->tunes[10].name, "falling");
    sfx->tunes[10].raw = malloc(4096 * 512);
    memset(sfx->tunes[10].raw, 0x00, 4096 * 512);
    sfx->tunes[10].sz = invfreq_decode_soundfx(falling, sfx->tunes[10].raw, 240);

    sfx->tune_idx = 0;
    sfx->tune_offset = 0;
    sfx->play = &soundfx_play;
    sfx->stop = &soundfx_stop;
    sfx->resume = &soundfx_resume;

    // Init SDL
    audio_spec_want.freq = 44100;
    audio_spec_want.format = AUDIO_S16LSB;
    audio_spec_want.channels = 1;
    audio_spec_want.samples = 1024;
    audio_spec_want.callback = &game_audio_callback;
    audio_spec_want.userdata = (void*)sfx;

    g_audio_dev = SDL_OpenAudioDevice(NULL, 0,
            &audio_spec_want, &audio_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (g_audio_dev == 0) {
        printf("Failed to open audio device \n");
        return NULL;
    }
    SDL_PauseAudioDevice(g_audio_dev, 1);

    return sfx;
}

void soundfx_destroy(soundfx_t *sfx)
{
    free(sfx->tunes[0].raw);
    free(sfx->tunes[1].raw);
    free(sfx->tunes[2].raw);
    free(sfx->tunes[3].raw);
    free(sfx->tunes[4].raw);
    free(sfx->tunes[5].raw);
    free(sfx->tunes[6].raw);
    free(sfx->tunes[7].raw);
    free(sfx->tunes[8].raw);
    free(sfx->tunes[9].raw);
    free(sfx->tunes[10].raw);
    free(sfx);
    SDL_CloseAudioDevice(g_audio_dev);
}
