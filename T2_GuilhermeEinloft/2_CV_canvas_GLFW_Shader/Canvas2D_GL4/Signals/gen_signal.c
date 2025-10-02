#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	float tri_x[64];
	float tri_y[64];
	double inc = 2 * M_PI / 64;
	for (int i = 0; i < 64; i++) {
		double t = i * inc;
		tri_x[i] = 16 * pow(sin(t), 3);
		tri_y[i] = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);
		printf("%f %f\n", tri_x[i], tri_y[i]);
	}
	return 0;
}
