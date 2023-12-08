#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include "game_2048.h"

static int matrix_dump(int** mat)
{
	if(NULL != mat)
	{
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			printf("[%4d %4d %4d %4d]\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
		}
	}
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	const char* USAGE = "\
2048, a tile-based game.\n\
\tpress 'left-arrow' for move-left,\n\
\tpress 'right-arrow' for move-right,\n\
\tpress 'up-arrow' for move-up,\n\
\tpress 'down-arrow' for move-down,\n\
\tpress 'end' for quit.\n\
";
	printf("%s", USAGE);
	struct termios gterm;
	tcgetattr(STDIN_FILENO, &gterm);
	struct termios term = gterm;
	term.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	int valid = 1;
	int kept = 1;
	int** mat = NULL;
	game_2048_init(&mat);
	game_2048_add_new_2(mat);
	do
	{
		if(valid)
		{
			matrix_dump(mat);
		}

		game_2048_status status = GAME_2048_STATUS_NOT_OVER;
		int changed = 0;
		char input = getchar();
		switch(input)
		{
			case 68:
				{
					printf("last move is %s\n", "left");
					game_2048_move_left(&mat, &changed);
					game_2048_get_status(mat, &status);
					if(GAME_2048_STATUS_NOT_OVER == status)
					{
						game_2048_add_new_2(mat);
					}
					else
					{
						printf("%s\n", (GAME_2048_STATUS_WON == status)?"Won":"Lost");
						kept = 0;
					}
					valid = 1;
				}
				break;
			case 67:
				{
					printf("last move is %s\n", "right");
					game_2048_move_right(&mat, &changed);
					game_2048_get_status(mat, &status);
					if(GAME_2048_STATUS_NOT_OVER == status)
					{
						game_2048_add_new_2(mat);
					}
					else
					{
						printf("%s\n", (GAME_2048_STATUS_WON == status)?"Won":"Lost");
						kept = 0;
					}
					valid = 1;
				}
				break;
			case 65:
				{
					printf("last move is %s\n", "up");
					game_2048_move_up(&mat, &changed);
					game_2048_get_status(mat, &status);
					if(GAME_2048_STATUS_NOT_OVER == status)
					{
						game_2048_add_new_2(mat);
					}
					else
					{
						printf("%s\n", (GAME_2048_STATUS_WON == status)?"Won":"Lost");
						kept = 0;
					}
					valid = 1;
				}
				break;
			case 66:
				{
					printf("last move is %s\n", "down");
					game_2048_move_down(&mat, &changed);
					game_2048_get_status(mat, &status);
					if(GAME_2048_STATUS_NOT_OVER == status)
					{
						game_2048_add_new_2(mat);
					}
					else
					{
						printf("%s\n", (GAME_2048_STATUS_WON == status)?"Won":"Lost");
						kept = 0;
					}
					valid = 1;
				}
				break;
			case 70:
				{
					valid = 1;
					printf("last move is %s\n", "quit");
					kept = 0;
				}
				break;
			default:
				{
					valid = 0;
					//printf("Invalid input (0x%x; %c)\n", input, isprint(input)?input:'?');
				}
				break;
		}
	} while(kept);
	game_2048_term(&mat);

	tcsetattr(STDIN_FILENO, TCSANOW, &gterm);
	return EXIT_SUCCESS;
}
