#include "snake.h"

t_snake *start_snake = NULL;
char *choices[] = {"Start game", "Exit"};
int n_choices = sizeof(choices) / sizeof(char *);
int score = 0;
int ball_row = 0;
int ball_col = 0;

void freeList(t_snake *snake){
	t_snake *save_ptr = snake->next;
	for(;;){
		if(save_ptr != NULL){
			free(snake);
			snake = save_ptr;
			save_ptr = save_ptr->next;
		} else {
			free(save_ptr);
			break;
		}
	}
}

void ball_generation(int max_row, int max_col){
	srand(time(NULL)); // to generate really random numbers
	ball_row = rand() % max_row;
	ball_col = rand() % max_col;
}

int check_snake_position(int row, int col){
	t_snake *check = start_snake;
	for(;;){
		if(check->row == row && check->col == col) return 1;
		if(check->next->next == NULL) break;
		check = check->next;
	}
	return 0;
}

void write_data(t_snake *snake, int row, int col){
	snake->col = col;
	snake->row = row;
	snake->next = NULL;
}

t_snake *push_head(t_snake *snake){
	t_snake *snake_head = (t_snake*)malloc(sizeof(t_snake));
	write_data(snake_head, 0, 0);
	snake->next = snake_head;
	return snake_head;
}

t_snake *memory_allocation(void){
	t_snake *snake = (t_snake*)malloc(sizeof(t_snake));
	return snake;
}

t_snake *create_snake(int row, int col){
	t_snake *snake = memory_allocation();
	start_snake = snake;
	write_data(snake, row, col);
	for(int i = col + 1; i <= col + 4; ++i){
		snake = push_head(snake);
		write_data(snake, row, i);
	}
	return snake;
}

int game(int *row, int *col){
	int c;
	int operator = 3;
	int start_row = *row / 2;
	int start_col = *col / 2;
	score = 0;
	
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE); // off waiting press button for getch()

	t_snake *snake = create_snake(start_row, start_col);

	t_snake *e_snake = start_snake;
	for(;;){
		mvprintw(e_snake->row, e_snake->col, "●");
		if(e_snake->next == NULL){
			mvprintw(e_snake->row, e_snake->col, "●");
			break;
		}
		e_snake = e_snake->next;
	}
	ball_generation(*row, *col);
	mvprintw(ball_row, ball_col, "●"); 
	mvprintw(2, 2, "Score: %i", score);
	refresh();
	usleep(300000);
	wclear(stdscr);
	refresh();
	while (1){
		if((*row > snake->row && 0 != snake->row) && (*col > snake->col + 1 && 0 != snake->col - 1)){
			int start_row = start_snake->row;
			int start_col = start_snake->col;

			e_snake = start_snake;
			for(;;){
				if(e_snake->next != NULL){
					e_snake->row = e_snake->next->row;
					e_snake->col = e_snake->next->col;
					e_snake = e_snake->next;
				}
				else break;
			}

			e_snake = start_snake;
			for(;;){
				if(e_snake->next == NULL) break;
				mvprintw(e_snake->row, e_snake->col, "●");
				refresh();
				e_snake = e_snake->next;
			}

			if((c = getch()) != ERR){
				if(c == KEY_UP){
					--snake->row;
					operator = 0;
				} else if(c == KEY_DOWN){
					++snake->row;
					operator = 1;
				} else if(c == KEY_LEFT){
					--snake->col;
					operator = 2;
				} else if(c == KEY_RIGHT){
					++snake->col;
					operator = 3;
				} else if(c == 'q')	break;

			} else{
				if		(operator == 0) 	--snake->row;
				else if	(operator == 1) 	++snake->row;
				else if	(operator == 2)		--snake->col;
				else if	(operator == 3)		++snake->col;
			}
			if((check_snake_position(snake->row, snake->col)) == 1) break; 	// confrontation with myself
			mvprintw(snake->row, snake->col, "●"); 							// print new head position
			if(snake->row == ball_row && snake->col == ball_col){  			// ball bump
				ball_generation(*row, *col);
				score += 10;
				t_snake *new_start_snake = memory_allocation();
				new_start_snake->row = start_row;
				new_start_snake->col = start_col;
				new_start_snake->next = start_snake;
				start_snake = new_start_snake;
			}
			mvprintw(2, 2, "Score: %i", score);
			mvprintw(ball_row, ball_col, "●"); 
			refresh();
			usleep(300000); //	microseconds
			wclear(stdscr);
			refresh();
		}
		else break;
	}
	freeList(start_snake);
	free(snake);	
	clrtoeol();
	refresh();
	wclear(stdscr);
	return 0;
}

void print_menu(WINDOW *menu_win, int highlight){
	int i, row, col;
	getmaxyx(menu_win, row, col);
	row = row / 2 - 1;
	col = (col / 2) / 2 - 1;
	box(menu_win, 0, 0);
	for (i = 0; i < n_choices; ++i){
		if (highlight == i + 1){ 			// High light the present choice 
			wattron(menu_win, A_BOLD );
			mvwprintw(menu_win, row, col, "%s", choices[i]);
			wattroff(menu_win, A_BOLD);
		} else mvwprintw(menu_win, row, col, "%s", choices[i]);
		++row;
	}
	wrefresh(menu_win);
}

void game_menu(int *row, int *col){
	char mesg[] = "Snake game by ";
	char nikname[] = "0x00_byte";

	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	mvprintw((*row / 2) / 2, (*col - strlen(mesg)) / 2 - strlen(nikname) / 2, "%s", mesg);
	attron(COLOR_PAIR(1));
	mvprintw((*row / 2) / 2, (*col - strlen(mesg)) / 2 + strlen(nikname) + 1, "%s", nikname);
	attroff(COLOR_PAIR(1));
	mvprintw((*row / 2), *col / 2 - strlen("Score:") /2 - 2 , "Score: %i", score);
	refresh();

	int startx = (*col - (strlen(mesg))) / 2;
	int starty = (*row / 2) / 2 + 1;
	menu_win = newwin(4, 14, starty, startx);
	keypad(menu_win, TRUE);
	print_menu(menu_win, highlight);
	
	while (1){
		c = wgetch(menu_win);
		switch (c){
			case KEY_UP:
				if (highlight == 1) highlight = n_choices;
				else --highlight;
				break;

			case KEY_DOWN:
				if (highlight == n_choices) highlight = 1;
				else ++highlight;
				break;

			case 10:
				choice = highlight;
				break;

			default:
				refresh();
				break;
		}
		print_menu(menu_win, highlight);
		if (choice != 0) break; // User did a choice come out of the infinite loop 
	}
	clrtoeol();

	if (choice == 1){
		wclear(stdscr);
		delwin(menu_win);
		game(row, col);
		game_menu(row, col);
	} else if (choice == 2){	
		delwin(menu_win);
		refresh();
		endwin();
	}
}

void color_test(int *row, int *col){
	if (has_colors() == FALSE){
		char err[] = "Your terminal does not support color";
		mvprintw(*row / 2, (*col - strlen(err)) / 2, "%s", err);
		refresh();
		sleep(3);
		endwin();
		exit(1);
	}
}

int main(void){
	int row, col;

	setlocale( LC_ALL, ""); // for normal print "alt codes"

	initscr(); // Start curses mode
	//raw();
	cbreak(); 
	curs_set(0);
	noecho();
	getmaxyx(stdscr, row, col);

	color_test(&row, &col);
	game_menu(&row, &col);

	endwin();
	exit(0);
}
