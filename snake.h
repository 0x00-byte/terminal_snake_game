#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>

#include <ncurses.h>

#define WIDTH 20   	//menu
#define HEIGHT 20	//menu

typedef struct s_snake {	
	int row;
	int col;
	struct s_snake *next;
} t_snake;


