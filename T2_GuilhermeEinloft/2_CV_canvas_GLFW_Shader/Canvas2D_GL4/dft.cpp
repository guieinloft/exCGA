#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void dft(float *x, float *yre, float *yim, int n)
{
	for (int i = 0; i < n; i++) {
		yre[i] = 0.f;
		yim[i] = 0.f;
		for (int j = 0; j < n; j++) {
			yre[i] += x[j] * cos(2 * M_PI * i * j / n);
			yim[i] += x[j] * sin(2 * M_PI * i * j / n);
		}
		yre[i] /= n;
		yim[i] /= -n;
	}
}

void idft(float *x, float *yre, float *yim, int n)
{
	for (int i = 0; i < n; i++) {
		x[i] = 0.f;
		for (int j = 0; j < n; j++) {
			x[i] += yre[j] * cos(2 * M_PI * i * j / n);
			x[i] -= yim[j] * sin(2 * M_PI * i * j / n);
		}
	}
}

void cdft(float *xre, float *xim, float *yre, float *yim, int n)
{
	for (int i = 0; i < n; i++) {
		yre[i] = 0.f;
		yim[i] = 0.f;
		for (int j = 0; j < n; j++) {
			yre[i] += xre[j] * cos(2 * M_PI * i * j / n) +
					xim[j] * sin(2 * M_PI * i * j / n);
			yim[i] += xim[j] * cos(2 * M_PI * i * j / n) -
					xre[j] * sin(2 * M_PI * i * j / n);
		}
		yre[i] /= n;
		yim[i] /= n;
	}
}

void cidft(float *xre, float *xim, float *yre, float *yim, int n)
{
	for (int i = 0; i < n; i++) {
		xre[i] = 0.f;
		xim[i] = 0.f;
		for (int j = 0; j < n; j++) {
			xre[i] += yre[j] * cos(2 * M_PI * i * j / n) -
					yim[j] * sin(2 * M_PI * i * j / n);
			xim[i] += yim[j] * cos(2 * M_PI * i * j / n) +
					yre[j] * sin(2 * M_PI * i * j / n);
		}
	}
}

void siginfo(float *yre, float *yim,
		float *yphase, float *yamp, float *yfreq, int n)
{
	for (int i = 0; i < n; i++) {
		yfreq[i] = i - n * (i > (n + 1) / 2);
		yphase[i] = atan2(yim[i], yre[i]);
		yamp[i] = sqrt(yre[i] * yre[i] + yim[i] * yim[i]);
	}
}

void print_siginfo(float *yre, float *yim,
		float *yphase, float *yamp, float *yfreq, int n)
{
	printf("k   Re X[k]\tIm X[k]\t\tFase\t\tAmplitude\tFrequencia\n");
	for (int i = 0; i < n; i++) {
		printf("%02d: %+f\t%+f\t%+f\t%+f\t%+f\n",
				i, yre[i], yim[i],
				yphase[i], yamp[i], yfreq[i]);
	}
}

/*
 * a funcao usa insertion sort, pois sao poucos pontos, portanto nao afeta
 * muito o desempenho.
 * no futuro, pode-se implementar uma funcao melhor.
 */
void sort_amplitude(float *yre, float *yim,
		float *yphase, float *yamp, float *yfreq, int n)
{
	int j;
	float temp_re, temp_im, temp_phase, temp_amp, temp_freq;
	for (int i = 1; i < n; i++) {
		temp_re = yre[i];
		temp_im = yim[i];
		temp_phase = yphase[i];
		temp_amp = yamp[i];
		temp_freq = yfreq[i];
		j = i - 1;
		while (j >= 0 && temp_amp > yamp[j]) {
			yre[j + 1] = yre[j];
			yim[j + 1] = yim[j];
			yphase[j + 1] = yphase[j];
			yamp[j + 1] = yamp[j];
			yfreq[j + 1] = yfreq[j];
			j--;
		}
		yre[j + 1] = temp_re;
		yim[j + 1] = temp_im;
		yphase[j + 1] = temp_phase;
		yamp[j + 1] = temp_amp;
		yfreq[j + 1] = temp_freq;
	}
}

