#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
	float biggest;
	float x_temp, y_temp;
	float *x, *y;
	int size;
	FILE *file = fopen(argv[1], "r");
	while (fscanf(file, "%f %f", &x_temp, &y_temp) != EOF)
		size += 1;
	fseek(file, 0, SEEK_SET);
	x = (float *)malloc(sizeof(float) * size);
	y = (float *)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++) {
		fscanf(file, "%f %f", &x[i], &y[i]);
		if (fabs(x[i]) > biggest)
			biggest = fabs(x[i]);
		if (fabs(y[i]) > biggest)
			biggest = fabs(y[i]);
	}
	fclose(file);
	file = fopen(argv[1], "w");
	for (int i = 0; i < size; i++) {
		fprintf(file, "%f %f\n", -x[i] / biggest, y[i] / biggest);
	}
	fclose(file);
}
