#include <algorithm>
#include <glm/glm.hpp>
#include "circle.h"
#include "sp_hash.h"
#include <stdio.h>
#include <math.h>

int used[GRID_W * GRID_H];
int initial_index[GRID_W * GRID_H];
int final_index[GRID_W * GRID_H];
int hash_table[CIRC_NUM];
int object_index[CIRC_NUM];

int hash_function(glm::vec3 point)
{
	int scale_x = SCREEN_W / GRID_W;
	int scale_y = SCREEN_H / GRID_H;

	int x = (int)std::clamp(point.x, 0.0f, (float)SCREEN_W - 1) / scale_x;
	int y = (int)std::clamp(point.y, 0.0f, (float)SCREEN_H - 1) / scale_y;

	return y * GRID_W + x;
}

enum direction hash_direction(glm::vec3 point)
{
	int scale_x = SCREEN_W / GRID_W;
	int scale_y = SCREEN_H / GRID_H;

	int x = (int)std::clamp(point.x, 0.0f, (float)SCREEN_W - 1) / scale_x;
	int y = (int)std::clamp(point.y, 0.0f, (float)SCREEN_H - 1) / scale_y;
	
	int x_1 = round(std::clamp(point.x, 0.0f, (float)SCREEN_W - 1) / scale_x);
	int y_1 = round(std::clamp(point.y, 0.0f, (float)SCREEN_H - 1) / scale_y);

	int dir = 0;
	dir += 2 * (x != x_1);
	dir += (y != y_1);
	return (enum direction)dir;
}

void spatial_hashing_update()
{
	for (int i = 0; i < GRID_W * GRID_H; i++) {
		used[i] = 0;
	}
	for (int i = 0; i < circ_num; i++) {
		int index = hash_function(circles[i].center);
		object_index[i] = index;
		used[index] += 1;
	}

	int accum = 0;
	for (int i = 0; i < GRID_W * GRID_H; i++) {
		initial_index[i] = accum;
		accum += used[i];
		final_index[i] = accum;
	}

	for (int i = 0; i < circ_num; i++) {
		hash_table[initial_index[object_index[i]]] = i;
		initial_index[object_index[i]]++;
	}

	for (int i = 0; i < GRID_W * GRID_H; i++) {
		initial_index[i] = final_index[i] - used[i];
	}
}

