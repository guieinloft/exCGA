#include <stdio.h>
#include <stdlib.h>
#include "dft.h"
#include "signal.h"

struct signal *load_signal(const char *path)
{
	float x_temp, y_temp;
	struct signal *s = (struct signal *)malloc(sizeof(struct signal));
	s->size = 0;
	FILE* file;
	fopen_s(&file, path, "r");
	while (fscanf_s(file, "%f %f", &x_temp, &y_temp) != EOF)
		s->size += 1;
	fseek(file, 0, SEEK_SET);
	s->x = (float *)malloc(sizeof(float) * s->size);
	s->y = (float *)malloc(sizeof(float) * s->size);
	s->re_X = (float *)malloc(sizeof(float) * s->size);
	s->im_X = (float *)malloc(sizeof(float) * s->size);
	s->re_Y = (float *)malloc(sizeof(float) * s->size);
	s->im_Y = (float *)malloc(sizeof(float) * s->size);
	s->re_XY = (float *)malloc(sizeof(float) * s->size);
	s->im_XY = (float *)malloc(sizeof(float) * s->size);
	s->amp_X = (float *)malloc(sizeof(float) * s->size);
	s->phase_X = (float *)malloc(sizeof(float) * s->size);
	s->freq_X = (float *)malloc(sizeof(float) * s->size);
	s->amp_Y = (float *)malloc(sizeof(float) * s->size);
	s->phase_Y = (float *)malloc(sizeof(float) * s->size);
	s->freq_Y = (float *)malloc(sizeof(float) * s->size);
	s->amp_XY = (float *)malloc(sizeof(float) * s->size);
	s->phase_XY = (float *)malloc(sizeof(float) * s->size);
	s->freq_XY = (float *)malloc(sizeof(float) * s->size);

	for (int i = 0; i < s->size; i++) {
		fscanf_s(file, "%f %f", &s->x[i], &s->y[i]);
	}

	fclose(file);

	dft(s->x, s->re_X, s->im_X, s->size);
	siginfo(s->re_X, s->im_X, s->phase_X, s->amp_X, s->freq_X, s->size);
	sort_amplitude(s->re_X, s->im_X, s->phase_X, s->amp_X, s->freq_X, s->size);
	//print_siginfo(s->re_X, s->im_X, s->phase_X, s->amp_X, s->freq_X, s->size);
	
	dft(s->y, s->re_Y, s->im_Y, s->size);
	siginfo(s->re_Y, s->im_Y, s->phase_Y, s->amp_Y, s->freq_Y, s->size);
	sort_amplitude(s->re_Y, s->im_Y, s->phase_Y, s->amp_Y, s->freq_Y, s->size);
	//print_siginfo(s->re_Y, s->im_Y, s->phase_Y, s->amp_Y, s->freq_Y, s->size);
	
	cdft(s->x, s->y, s->re_XY, s->im_XY, s->size);
	siginfo(s->re_XY, s->im_XY, s->phase_XY, s->amp_XY, s->freq_XY, s->size);
	sort_amplitude(s->re_XY, s->im_XY, s->phase_XY, s->amp_XY, s->freq_XY, s->size);
	//print_siginfo(s->re_XY, s->im_XY, s->phase_XY, s->amp_XY, s->freq_XY, s->size);
	return s;
}

void free_signal(struct signal *s)
{
	free(s->x);
	free(s->y);
	free(s->re_X);
	free(s->im_X);
	free(s->re_Y);
	free(s->im_Y);
	free(s->re_XY);
	free(s->im_XY);
	free(s->amp_X);
	free(s->phase_X);
	free(s->freq_X);
	free(s->amp_Y);
	free(s->phase_Y);
	free(s->freq_Y);
	free(s->amp_XY);
	free(s->phase_XY);
	free(s->freq_XY);
	free(s);
}