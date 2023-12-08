#ifndef GAME_2048_H
#define	GAME_2048_H

#define GAME_2048_TARGET 2048
typedef enum
{
	GAME_2048_STATUS_NOT_OVER,
	GAME_2048_STATUS_WON,
	GAME_2048_STATUS_LOST,
} game_2048_status;

int game_2048_init(int***);
int game_2048_term(int***);
int game_2048_add_new_2(int**);
int game_2048_get_status(int**, game_2048_status*);
int game_2048_move_left(int***, int*);
int game_2048_move_right(int***, int*);
int game_2048_move_up(int***, int*);
int game_2048_move_down(int***, int*);

#endif //GAME_2048_H
