#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_2048.h"

static int** matrix_new(void)
{
	int** mat = NULL;
	do
	{
		mat = (int**)malloc(sizeof(int*) * 4);
		if(NULL == mat)
		{
			break;
		}
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			mat[i] = (int*)malloc(sizeof(int) * 4);
			if(NULL == mat[i])
			{
				break;
			}
		}
	} while(0);
	return mat;
}

static void matrix_delete(int*** mat)
{
	do
	{
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			free((*mat)[i]);
		}
		free((*mat));
		*mat = NULL;
	} while(0);
}

static void matrix_fill(int** mat, int val)
{
	do
	{
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			int j = 0;
			for(j = 0; j < 4; j++)
			{
				mat[i][j] = val;
			}
		}
	} while(0);
}

static int matrix_reverse(int*** mat)
{
	int** reversed = matrix_new();
	int i = 0;
	for(i = 0; i < 4; i++)
	{
		int j = 0;
		for(j = 0; j < 4; j++)
		{
			reversed[i][j] = (*mat)[i][4 - j - 1];
		}
	}
	matrix_delete(mat);
	*mat = reversed;
	return EXIT_SUCCESS;
}

static int matrix_transpose(int*** mat)
{
	int** transposed = matrix_new();
	int i = 0;
	for(i = 0; i < 4; i++)
	{
		int j = 0;
		for(j = 0; j < 4; j++)
		{
			transposed[i][j] = (*mat)[j][i];
		}
	}
	matrix_delete(mat);
	*mat = transposed;
	return EXIT_SUCCESS;
}

static int matrix_compress(int*** mat, int* changed)
{
	*changed = 0;
	int** compressed = matrix_new();
	matrix_fill(compressed, 0);
	int i = 0;
	for(i = 0; i < 4; i++)
	{
		int pos = 0;
		int j = 0;
		for(j = 0; j < 4; j++)
		{
			if(0 != (*mat)[i][j])
			{
				compressed[i][pos] = (*mat)[i][j];
				if(j != pos)
				{
					*changed = 1;
				}
				pos++;
			}
		}
	}
	if(0 == *changed)
	{
		matrix_delete(&compressed);
	}
	else
	{
		matrix_delete(mat);
		*mat = compressed;
	}
	return EXIT_SUCCESS;
}

static int matrix_merge(int** mat, int* changed)
{
	*changed = 0;
	int i = 0;
	for(i = 0; i < 4; i++)
	{
		int j = 0;
		for(j = 0; j < 4; j++)
		{
			if(mat[i][j] == mat[i][j + 1] && 0 != mat[i][j])
			{
				mat[i][j] = mat[i][j] * 2;
				mat[i][j + 1] = 0;
				*changed = 1;
			}
		}
	}
	return EXIT_SUCCESS;
}

int game_2048_init(int*** mat)
{
	srand(time(NULL));
	do
	{
		if(NULL == mat)
		{
			break;
		}
		*mat = matrix_new();
		matrix_fill(*mat, 0);
		return EXIT_SUCCESS;
	} while(0);
	return EXIT_FAILURE;
}

int game_2048_term(int*** mat)
{
	do
	{
		if(NULL == mat)
		{
			break;
		}
		matrix_delete(mat);
		return EXIT_SUCCESS;
	} while(0);
	return EXIT_FAILURE;
}

int game_2048_add_new_2(int** mat)
{
	int i = 0;
	const int MAX = 128;
	do
	{
		int r = rand() % 4;
		int c = rand() % 4;
		if(0 == mat[r][c])
		{
			mat[r][c] = 2;
			break;
		}
		i++;
	} while(i < MAX);
	return (i < MAX)?EXIT_SUCCESS:EXIT_FAILURE;
}

int game_2048_get_status(int** mat, game_2048_status* status)
{
	int i = 0;
	int j = 0;

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(GAME_2048_TARGET == mat[i][j])
			{
				*status = GAME_2048_STATUS_WON;
				return EXIT_SUCCESS;
			}
		}
	}

	for(i = 0; i< 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(0 == mat[i][j])
			{
				*status = GAME_2048_STATUS_NOT_OVER;
				return EXIT_SUCCESS;
			}
		}
	}

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(mat[i][j] == mat[i + 1][j] || mat[i][j] == mat[i][j + 1])
			{
				*status = GAME_2048_STATUS_NOT_OVER;
				return EXIT_SUCCESS;
			}
		}
	}

	for(j = 0; j < 3; j++)
	{
		if(mat[3][j] == mat[3][j + 1])
		{
			*status = GAME_2048_STATUS_NOT_OVER;
			return EXIT_SUCCESS;
		}
	}

	for(i = 0; i < 3; i++)
	{
		if(mat[i][3] == mat[i + 1][3])
		{
			*status = GAME_2048_STATUS_NOT_OVER;
			return EXIT_SUCCESS;
		}
	}

	*status = GAME_2048_STATUS_LOST;
	return EXIT_SUCCESS;
}

int game_2048_move_left(int*** mat, int* changed)
{
	int changed1 = 0;
	matrix_compress(mat, &changed1);
	int changed2 = 0;
	matrix_merge(*mat, &changed2);
	int changed3 = 0;
	matrix_compress(mat, &changed3);
	*changed = changed1 && changed2;
	return EXIT_SUCCESS;
}

int game_2048_move_right(int*** mat, int* changed)
{
	matrix_reverse(mat);
	int ret = game_2048_move_left(mat, changed);
	matrix_reverse(mat);
	return ret;
}

int game_2048_move_up(int*** mat, int* changed)
{
	matrix_transpose(mat);
	int ret = game_2048_move_left(mat, changed);
	matrix_transpose(mat);
	return ret;
}

int game_2048_move_down(int*** mat, int* changed)
{
	matrix_transpose(mat);
	int ret = game_2048_move_right(mat, changed);
	matrix_transpose(mat);
	return ret;
}
