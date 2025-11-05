/*
 * sp_hash.h
 * funções de geração e atualização da tabela hash e de geração de índices
 * usada para armazenar apenas os círculos
 */

#ifndef SP_HASH_H
#define SP_HASH_H

#include "b_spline.h"
#include "circle.h"
#include "sp_hash.h"

/* dimensões do grid */
#define GRID_W 32
#define GRID_H 32

/* direção que o algoritmo de verificação de colisões do círculo vai checar */
enum direction {
	DIR_SW = 0,
	DIR_NW = 1,
	DIR_SE = 2,
	DIR_NE = 3
};

/*
 * arrays utilizados na geração da tabela hash
 * para mais informações, ler o artigo
 * "A Hash Table Construction Algorithm for Spatial Hashing Based on Linear Memory"
 */

extern int used[GRID_W * GRID_H];
extern int initial_index[GRID_W * GRID_H];
extern int final_index[GRID_W * GRID_H];
extern int hash_table[CIRC_NUM];
extern int object_index[CIRC_NUM];

/* gera a chave do objeto baseada na localização */
int hash_function(glm::vec3 point);
/* verifica qual direção o algoritmo de verificação de colisões deve checar */
enum direction hash_direction(glm::vec3 point);
/* atualiza a tabela hash, baseado no artigo */
void spatial_hashing_update();

#endif
