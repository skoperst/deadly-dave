#ifndef _INVFREQ_H_
#define _INVFREQ_H_

double invfreq_exp(double x);
double invfreq_log(double x);
double invfreq_sqrt(double x);
double invfreq_pow(double x, double a);
double invfreq_sin(double x);
size_t invfreq_decode_soundfx(uint16_t *data, uint8_t *out, int samples_per_beep);

#endif
