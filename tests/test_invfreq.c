#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "invfreq.h"

uint16_t jumping[] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0B7C, 0x0B7C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x099C, 0x099C, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0834, 0x0834, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0564, 0x0564, 0x0000, 0x0000, 0x04EC, 0x04EC, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x03C0, 0x03C0,
    0x0000, 0x0000, 0x0384, 0x0384, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x02D0, 0x02D0, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01E0, 0x01E0,
    0x0000, 0x0000, 0x01E0, 0x01E0, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0168, 0x0168,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x012C, 0x012C,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x00B4, 0x00B4, 0x0000, 0x0000, 0x0000, 0x0000,
    0x00B4, 0x00B4, 0x00B4, 0x00B4, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x00B4, 0x00B4, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x00B4, 0x00B4, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0078, 0x0078, 0x0000, 0x0000,
    0x0078, 0x0078, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0078, 0x0078, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0078, 0x0078, 0x0000, 0x0000,
    0x0078, 0x0078, 0x0000, 0x0000, 0x00B4, 0x00B4, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x00F0, 0x00F0, 0x0000, 0x0000,
    0x012C, 0x012C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x01E0, 0x01E0, 0x0000, 0x0000,
    0x021C, 0x021C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0258, 0x0258, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0384, 0x0384, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0438, 0x0438, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x04B0, 0x04B0, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0564, 0x0564, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0654, 0x0654,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x06CC, 0x06CC,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x07F8, 0x07F8, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x08AC, 0x08AC,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0A14, 0x0A14, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0B40, 0x0B40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0C30, 0x0C30, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0CA8, 0x0CA8, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0CE4, 0x0CE4, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0E10, 0x0E10, 0x0000, 0x0000,
    0x0E4C, 0x0E4C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0F3C, 0x0F3C,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0FB4, 0x0FB4, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0FF0, 0x0FF0, 0x102C, 0x102C, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x10E0, 0x10E0, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x120C, 0x120C, 0x0000, 0x0000, 0x1284, 0x1284,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1374, 0x1374,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x14A0, 0x14A0, 0x14DC, 0x14DC,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x16F8, 0x16F8, 0x1734, 0x1734, 0x0000, 0x0000,
    0x0000, 0x0000, 0x17E8, 0x17E8, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x1A7C, 0x1A7C, 0x1AB8, 0x1AB8,
    0x0000, 0x0000, 0x0000, 0x0000, 0x1B6C, 0x1B6C, 0x0000, 0x0000,
    0x1BA8, 0x1BA8, 0x0000, 0x0000, 0x0000, 0x0000, 0x1C5C, 0x1C5C,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x1D88, 0x1D88, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x1FA4, 0x1FA4, 0x0000, 0x0000,
    0x0000, 0x0000, 0x201C, 0x201C, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x2184, 0x2184, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x2544, 0x2544, 0x0000, 0x0000, 0x0000, 0x0000,
    0x25F8, 0x25F8, 0x0000, 0x0000, 0x26AC, 0x26AC, 0xFFFF, 0xFFFF,
};





int main(int argc, char **argv)
{
    FILE *outfile;
    uint8_t *raw = malloc(4096 * 512);
    size_t raw_sz;
    int ret = 0;

    memset(raw, 0x00, 4096 * 512);
    raw_sz = invfreq_decode_soundfx(jumping, raw, 4096 * 512);

    outfile = fopen("out.raw", "w+");
    ret = fwrite(raw, raw_sz, 1, outfile);
    printf("wrote %d bytes \n", ret);
    fclose(outfile);

    free(raw);


    return 0;
}
