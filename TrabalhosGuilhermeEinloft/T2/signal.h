struct signal {
	int size;
	float *x;
	float *y;
	float *re_X;
	float *im_X;
	float *re_Y;
	float *im_Y;
	float *re_XY;
	float *im_XY;
	float *amp_X;
	float *phase_X;
	float *freq_X;
	float *amp_Y;
	float *phase_Y;
	float *freq_Y;
	float *amp_XY;
	float *phase_XY;
	float *freq_XY;
};

struct signal *load_signal(const char *path);
