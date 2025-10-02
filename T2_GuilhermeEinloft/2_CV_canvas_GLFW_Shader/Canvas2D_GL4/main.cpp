#include <vector>
#include "Canvas2D/gl_canvas2d.h"
#include "dft.h"
#include "signal.h"

#define HALF_PI 1.5707963267948966
#define OFFSET 2

int screenWidth, screenHeight;

std::vector<float> x_render;
std::vector<float> y_render;

struct signal *sigs[10];
int cur_sig = 1;

float t = 0.f;
bool draw_complex = false;
bool show_points = true;
float scale = 128.f;

void draw_points(float *x, float *y, int size)
{
	for (int i = 0; i < size; i++) {
		CV::rectFill(x[i] * scale - 2, y[i] * scale - 2,
				x[i] * scale + 2, y[i] * scale + 2);
		CV::line(x[i] * scale, y[i] * scale,
				x[(i + 1) % size] * scale,
				y[(i + 1) % size] * scale);
	}
}

void draw_epicycle(float *amp, float *freq, float *phase, float p_ofst,
		float x_ofst, float y_ofst, float *x, float *y, int size)
{
	float x_1 = 0, x_2 = 0;
	float y_1 = 0, y_2 = 0;
	for (int i = 0; i < size; i++) {
		x_2 += amp[i] * cos(freq[i] * t + phase[i] + p_ofst);
		y_2 += amp[i] * sin(freq[i] * t + phase[i] + p_ofst);
		CV::circle(x_1 * scale + x_ofst, y_1 * scale + y_ofst,
				amp[i] * scale, 16); 
		CV::line(x_1 * scale + x_ofst, y_1 * scale + y_ofst,
				x_2 * scale + x_ofst, y_2 * scale + y_ofst);
		x_1 = x_2;
		y_1 = y_2;
	}
	*x = x_2;
	*y = y_2;
}

void draw_dual_epicycles(float *x_amp, float *x_freq, float *x_phase,
		float *y_amp, float *y_freq, float *y_phase, int size)
{
	float x_1, x_2, y_1, y_2, x, y;
	draw_epicycle(y_amp, y_freq, y_phase, HALF_PI,
			-OFFSET * scale, 0, &x_1, &y_1, size);
	y = y_1;
	draw_epicycle(x_amp, x_freq, x_phase, 0,
			0, -OFFSET * scale, &x_2, &y_2, size);
	x = x_2;
	CV::line((x_1 - OFFSET) * scale, y_1 * scale, x * scale, y * scale);
	CV::line(x_2 * scale, (y_2 - OFFSET) * scale, x * scale, y * scale);
	x_render.push_back(x);
	y_render.push_back(y);
}

void draw_complex_epicycle(float *amp, float *freq, float *phase, int size)
{
	float x, y;
	draw_epicycle(amp, freq, phase, 0, 0, 0, &x, &y, size);
	x_render.push_back(x);
	y_render.push_back(y);
}

void render() {
	CV::clear(0, 0, 0);
	CV::translate(screenWidth / 2, screenHeight / 2);
	CV::color(1.0, 0.0, 0.0);
	if (draw_complex) {
		draw_complex_epicycle(sigs[cur_sig]->amp_XY,
				sigs[cur_sig]->freq_XY, sigs[cur_sig]->phase_XY,
				sigs[cur_sig]->size);
	} else {
		draw_dual_epicycles(sigs[cur_sig]->amp_X, sigs[cur_sig]->freq_X,
				sigs[cur_sig]->phase_X, sigs[cur_sig]->amp_Y,
				sigs[cur_sig]->freq_Y, sigs[cur_sig]->phase_Y,
				sigs[cur_sig]->size);
	}
	CV::color(0.0, 0.6, 0.0);
	if (show_points)
		draw_points(sigs[cur_sig]->x, sigs[cur_sig]->y, sigs[cur_sig]->size);
	CV::color(1, 1, 1);
	for (int i = 0; i < x_render.size() - 1; i++) {
		CV::line(x_render[i] * scale, y_render[i] * scale,
				x_render[i + 1] * scale, y_render[i + 1] * scale);
	}
	//t += 2 * M_PI / 8;
	t += 0.01;
}

void keyboard(int key) {
	switch (key) {
	case 'C':
		draw_complex = !draw_complex;
		break;
	case 'P':
		show_points = !show_points;
		break;
	case 265:
		scale *= 2;
		break;
	case 264:
		scale /= 2;
		break;
	}
	if (key >= '0' && key <= '9')
		cur_sig = key - '0';
	x_render.clear();
	y_render.clear();
}

void keyboardUp(int key) {
    //printf("Liberou a tecla: %c\n", key);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("Mouse button: %d, state: %d, wheel: %d, direction: %d, x: %d, y: %d\n", button, state, wheel, direction, x, y);
}

int main(void)
{
	sigs[1] = load_signal("Signals/sig_1.dat");
	sigs[2] = load_signal("Signals/sig_2.dat");
	sigs[3] = load_signal("Signals/sig_3.dat");
	sigs[4] = load_signal("Signals/sig_4.dat");
	sigs[5] = load_signal("Signals/sig_5.dat");
	sigs[6] = load_signal("Signals/sig_6.dat");
	sigs[7] = load_signal("Signals/sig_7.dat");
	sigs[8] = load_signal("Signals/sig_8.dat");
	sigs[9] = load_signal("Signals/sig_9.dat");
	sigs[0] = load_signal("Signals/sig_0.dat");
	for (int i = 0; i < 10; i++) {
		if (sigs[i] == NULL) {
			printf("Nao consegui carregar os sinais\n");
			return(EXIT_FAILURE);
		}
	}
	CV::init(800, 600, "Canvas2D", true);
	CV::run();
	for (int i = 0; i < 10; i++) {
		free_signal(sigs[i]);
	}
	return 0;
}
