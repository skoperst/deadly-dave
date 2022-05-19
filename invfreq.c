#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "invfreq.h"

uint16_t engine_sndCount;
int16_t engine_currSoundPlaying;
uint8_t sound_limiter, *sndPriorities;
char game_ext[4];
uint32_t pc_pit_rate = 1193180;
const uint32_t pitAltCounter = 0x2000;

typedef struct invfreq_block_struct {
    uint16_t pcm[4096];
    size_t len;
} invfreq_block_t;

double invfreq_exp(double x) {
    double ret = 1, nom = 1, fac = 1;
    int i;
    for (i = 0; i < 64; i++) {
        nom *= x;
        ret += nom/fac;
        fac = fac * (double)(i + 2);
    }

    return ret;
}

/*
 * Taylor series impl. of log
 */
double invfreq_log(double x) {
    double euler_c = 2.718281828459045235;
    // Trap illegal values
    if (x <= 0) {
        return 0.0/0.0;  // NaN
    }

    // Confine x to a sensible range
    int power_adjust = 0;
    while (x > 1.0) {
        x /= euler_c;
        power_adjust++;
    }
    while (x < .25) {
        x *= euler_c;
        power_adjust--;
    }

    // Now use the Taylor series to calculate the logarithm
    x -= 1.0;
    double t = 0.0, s = 1.0, z = x;
    int k;
    for (k=1; k <= 20; k++) {
        t += z * s / k;
        z *= x;
        s = -s;
    }

    // Combine the result with the power_adjust value and return
    return t + power_adjust;
}

double invfreq_log2(double x) {
    return invfreq_log(x)/invfreq_log(2);
}

double invfreq_pow(double x, double a) {
    return invfreq_exp(a * invfreq_log(x));
}

double invfreq_sqrt(double x) {
    return invfreq_pow(2, 0.5 * invfreq_log2(x));
}

double invfreq_fmod(double x, double y)
{
    union {double f; uint64_t i;} ux = {x}, uy = {y};
    int ex = ux.i>>52 & 0x7ff;
    int ey = uy.i>>52 & 0x7ff;
    int sx = ux.i>>63;
    uint64_t i;

    /* in the followings uxi should be ux.i, but then gcc wrongly adds */
    /* float load/store to inner loops ruining performance and code size */
    uint64_t uxi = ux.i;

    if (uy.i<<1 == 0 || isnan(y) || ex == 0x7ff)
    	return (x*y)/(x*y);
    if (uxi<<1 <= uy.i<<1) {
        if (uxi<<1 == uy.i<<1)
        	return 0*x;
        return x;
    }

    /* normalize x and y */
    if (!ex) {
		for (i = uxi<<12; i>>63 == 0; ex--, i <<= 1);
		uxi <<= -ex + 1;
	} else {
		uxi &= -1ULL >> 12;
		uxi |= 1ULL << 52;
	}
	if (!ey) {
		for (i = uy.i<<12; i>>63 == 0; ey--, i <<= 1);
		uy.i <<= -ey + 1;
	} else {
		uy.i &= -1ULL >> 12;
		uy.i |= 1ULL << 52;
	}

	/* x mod y */
	for (; ex > ey; ex--) {
		i = uxi - uy.i;
		if (i >> 63 == 0) {
			if (i == 0)
                return 0*x;
            uxi = i;
        }
        uxi <<= 1;
    }
    i = uxi - uy.i;
    if (i >> 63 == 0) {
        if (i == 0)
            return 0*x;
        uxi = i;
    }
    for (; uxi>>52 == 0; uxi <<= 1, ex--);

    /* scale result */
    if (ex > 0) {
        uxi -= 1ULL << 52;
        uxi |= (uint64_t)ex << 52;
    } else {
        uxi >>= -ex + 1;
    }
    uxi |= (uint64_t)sx << 63;
    ux.i = uxi;
    return ux.f;
}

double invfreq_sin (double theta)
{
    int n;
    double sx;
    double sign = 1.0;
    double pi = 3.14159265358979323846;
    /* Chebyshev constants for cos, range -PI/4 - PI/4.  */
    const double C0 = -0x1.ffffffffe98aep-2;
    const double C1 =  0x1.55555545c50c7p-5;
    const double C2 = -0x1.6c16b348b6874p-10;
    const double C3 =  0x1.a00eb9ac43ccp-16;
    const double C4 = -0x1.23c97dd8844d7p-22;
    /* Chebyshev constants for sin, range -PI/4 - PI/4.  */
    const double S0 = -0x1.5555555551cd9p-3;
    const double S1 =  0x1.1111110c2688bp-7;
    const double S2 = -0x1.a019f8b4bd1f9p-13;
    const double S3 =  0x1.71d7264e6b5b4p-19;
    const double S4 = -0x1.a947e1674b58ap-26;

    if (theta < 0) {
        theta = -1.0 * theta;
        sign = -1.0 * sign;
    }

    theta = invfreq_fmod(theta, 2 * pi);
    if (theta > pi) {
        theta = theta - pi;
        sign = -1.0 * sign;
    }

    n = 0;
    if (theta > (pi/2)) {
        theta = theta - (pi/2);
        n = 2;
    }

    const double theta2 = theta * theta;
    /* We are operating on |x|, so we need to add back the original
     signbit for sinf.  */
  /* Determine positive or negative primary interval.  */
  /* Are we in the primary interval of sin or cos?  */
  if ((n & 2) == 0)
    {
      /* Here sinf() is calculated using sin Chebyshev polynomial:
	x+x^3*(S0+x^2*(S1+x^2*(S2+x^2*(S3+x^2*S4)))).  */
      sx = S3 + theta2 * S4;     /* S3+x^2*S4.  */
      sx = S2 + theta2 * sx;     /* S2+x^2*(S3+x^2*S4).  */
      sx = S1 + theta2 * sx;     /* S1+x^2*(S2+x^2*(S3+x^2*S4)).  */
      sx = S0 + theta2 * sx;     /* S0+x^2*(S1+x^2*(S2+x^2*(S3+x^2*S4))).  */
      sx = theta + theta * theta2 * sx;
    }
  else
    {
     /* Here sinf() is calculated using cos Chebyshev polynomial:
	1.0+x^2*(C0+x^2*(C1+x^2*(C2+x^2*(C3+x^2*C4)))).  */
      sx = C3 + theta2 * C4;     /* C3+x^2*C4.  */
      sx = C2 + theta2 * sx;     /* C2+x^2*(C3+x^2*C4).  */
      sx = C1 + theta2 * sx;     /* C1+x^2*(C2+x^2*(C3+x^2*C4)).  */
      sx = C0 + theta2 * sx;     /* C0+x^2*(C1+x^2*(C2+x^2*(C3+x^2*C4))).  */
      sx = 1.0 + theta2 * sx;
    }

  /* Add in the signbit and assign the result.  */
  return sign * sx;
}

double invfreq_wavelength(double frequency) {
    return (double)1 / frequency;
}

/*int invfreq_samples_per_waveform(double frequency, int samplerate) {
    double wavelength = invfreq_wavelength(frequency);
    double samples_d = wavelength * (double) samplerate;

    return (int)samples_d;
}

int invfreq_samples_per_half_waveform(double frequency, int samplerate) {
    int samples = invfreq_samples_per_waveform(frequency, samplerate);
    return samples / 2;
}*/

// Returns frequency value in hz from inverse-frequency format value
double invfreq_to_freq(uint16_t invfreq) {
    double invfreq_d = (double)invfreq;
    double frequency = ((double)pc_pit_rate / invfreq_d);

    return frequency;
}

/*
 * Approximate a square wave without ringing, originally published by
 * M. Fernández Guasti, in 'Squdel function'.
 */
double sqd(double x, double freq) {
    double intensity = 1;
    if (freq >= 6000)
        return invfreq_sin(x);
    if (freq >= 4000)
        intensity = 0.1;
    else if (freq >= 2000) 
        intensity = 0.1;
    else 
        intensity = 0.001;

    return invfreq_sin(x) / invfreq_sqrt((invfreq_sin(x) * invfreq_sin(x)) + intensity);
}

size_t invfreq_decode_soundfx(uint16_t *data, uint8_t *out, int samples_per_symbol) {
    int idx = 0;
    double freq = 0.0, fs = 44100;
    double pi = 3.14159265358979323846;
    double d_phase = 0.0, m_phase = 0.0;
    double sigval;
    int i;
    size_t len = 0;
    int16_t *out16 = (int16_t*)out;

    while (data[idx] != 0xFFFF) {
        if (data[idx] != 0x0000) {
            freq = invfreq_to_freq(data[idx]);
            d_phase = 2 * pi * freq / fs;

            for (i = 0; i < samples_per_symbol; i++) {
                sigval = ((sqd(m_phase, freq) * 5000) + 5000);
                m_phase = m_phase + d_phase;
                m_phase = invfreq_fmod(m_phase, 2 * pi);

                *out16 = (int16_t) sigval;
                out16++;
                len +=2;
            }
        } else {

            memset(out16, 0, 2 * samples_per_symbol);
            out16 += samples_per_symbol;
            len += (samples_per_symbol * 2);
            m_phase = 0;
        }

        idx++;
    }

    return len;
}

