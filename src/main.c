#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

void init_ncurses();
void draw_window(int max_disc_num, int pegs[3][max_disc_num], int selected);
int get_max_base_size();
int get_max_disc_num();
bool move_disc(int from, int to, int max_disc_num, int pegs[3][max_disc_num]);

//TODO: Add better win message
int main(int argc, char* argv[])
{

	init_ncurses();

	// initialize pegs
	int max_disc_num = get_max_disc_num();
	int pegs[3][max_disc_num];

	int disc_num = 5;

	if(argc > 1)
	{
		int i = 1;
		while(i < argc)
		{
			if(argv[i][0] == '-')
			{
				switch(argv[i][1])
				{
					case 'n':
						disc_num = atoi(argv[i+1]);
						if(disc_num > max_disc_num)
						{
							disc_num = max_disc_num;
						}else if(disc_num < 3){
							disc_num = 3;
						}
						break;
					case 'c':
						if(strcmp("black", argv[i+1]) == 0){init_pair(2, COLOR_WHITE, COLOR_BLACK);}
						else if(strcmp("red", argv[i+1]) == 0){init_pair(2, COLOR_BLACK, COLOR_RED);}
						else if(strcmp("green", argv[i+1]) == 0){init_pair(2, COLOR_BLACK, COLOR_GREEN);}
						else if(strcmp("yellow", argv[i+1]) == 0){init_pair(2, COLOR_BLACK, COLOR_YELLOW);}
						else if(strcmp("blue", argv[i+1]) == 0){init_pair(2, COLOR_BLACK, COLOR_BLUE);}
						else if(strcmp("magenta", argv[i+1]) == 0){init_pair(2, COLOR_BLACK, COLOR_MAGENTA);}
						else if(strcmp("cyan", argv[i+1]) == 0){init_pair(2, COLOR_BLACK, COLOR_CYAN);}
						else if(strcmp("white", argv[i+1]) == 0){init_pair(2, COLOR_BLACK, COLOR_WHITE);}
						break;
					case 'm':
						disc_num = max_disc_num;
						break;
				}
			}
			i+=2;
		}
	}

	int i;
	for(i = 0; i < max_disc_num; i++)
	{
		if(disc_num - i > 0)
		{
			pegs[0][i] = disc_num - i;
		}else{
			pegs[0][i] = 0;
		}
		pegs[1][i] = 0;
		pegs[2][i] = 0;
	}

	int selected = -1;

	draw_window(max_disc_num, pegs, selected);

	int moves = 0;

	// draw indicators
	attron(COLOR_PAIR(1));
	mvprintw(0, 0, "moves: %d", moves);
	mvprintw(0, COLS-8, "size: %d", disc_num);
	attroff(COLOR_PAIR(1));


	while(true)
	{
		if(selected == -1)
		{
			switch(getch())
			{
				case 'j':
					selected = 0;
					break;
				case 'k':
					selected = 1;
					break;
				case 'l':
					selected = 2;
					break;
				case '\033':
					getch();
					switch(getch())
					{
						case 'B':
							selected = 1;
							break;
						case 'C':
							selected = 2;
							break;
						case 'D':
							selected = 0;
							break;
					}
					break;
			}
		}else/* a peg is selected */{
			bool moved = false;
			switch(getch())
			{
				case 'j':
					moved = move_disc(selected, 0, max_disc_num, pegs);
					selected = -1;
					break;
				case 'k':
					moved = move_disc(selected, 1, max_disc_num, pegs);
					selected = -1;
					break;
				case 'l':
					moved = move_disc(selected, 2, max_disc_num, pegs);
					selected = -1;
					break;
				case '\033':
					getch();
					switch(getch())
					{
						case 'B':
							moved = move_disc(selected, 1, max_disc_num, pegs);
							selected = -1;
							break;
						case 'C':
							moved = move_disc(selected, 2, max_disc_num, pegs);
							selected = -1;
							break;
						case 'D':
							moved = move_disc(selected, 0, max_disc_num, pegs);
							selected = -1;
							break;
					}
					break;
			}
			if(moved)
			{
				moves++;
			}
		}// end else

		draw_window(max_disc_num, pegs, selected);

		// draw indicators
		attron(COLOR_PAIR(1));
		mvprintw(0, 0, "moves: %d", moves);
		mvprintw(0, COLS-8, "size: %d", disc_num);
		attroff(COLOR_PAIR(1));
		refresh();
		
		bool won = true;
		int a;
		for(a = 0; a < max_disc_num; a++)
		{
			if(pegs[0][a] != 0 || pegs[1][a] != 0)
			{
				won = false;
			}
		}

		if(won)
		{
			mvprintw(LINES/2, COLS/2-5, "You won!");
			break;
		}
	}// end while(true)

	getch();
	endwin();
	exit(1);

	return 0;
}

void init_ncurses()
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	start_color();
	use_default_colors();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	
}

void draw_window(int max_disc_num, int pegs[3][max_disc_num], int selected)
{

	clear();

	// draw top bar
	attron(COLOR_PAIR(1));
	mvprintw(0, 0, "%*s", COLS, " ");
	attroff(COLOR_PAIR(1));
	

	// draw pegs
	attron(COLOR_PAIR(1));

	int max_base_size = get_max_base_size();
	int peg_height = max_disc_num+5;

	int first_peg_pos = (max_base_size-1)/2;

	int c;
	for(c = first_peg_pos; c < COLS-(max_base_size/2); c += (max_base_size-1))
	{
		int r;
		for(r = LINES - peg_height; r < LINES; r++)
		{
			mvprintw(r, c, " ");
		}
	}

	// draw selection indicator
	c = first_peg_pos + selected*(max_base_size-1);
	mvprintw(LINES - peg_height - 3, c, " ");

	attroff(COLOR_PAIR(1));

	// draw discs
	attron(COLOR_PAIR(2));

	int peg_num, i;
	for(peg_num = 0; peg_num < 3; peg_num++)
	{
		// iterate through peg arrays
		for(i = 0; i < max_disc_num; i++)
		{
			int disc_num = pegs[peg_num][i];
			if(disc_num != 0)
			{
				int disc_width = (2*disc_num)+1;
				int start_col = first_peg_pos + (peg_num*(max_base_size-1)) - (disc_width-1)/2;

				int a;
				for(a = start_col; a < start_col + disc_width; a++)
				{
					mvprintw(LINES-1-i, a, "_");
				}
			}
		}
	}

	attroff(COLOR_PAIR(2));

	refresh();
}

int get_max_base_size()
{
	int max_base_size = COLS/3;
	if(max_base_size % 2 == 0)
	{
		max_base_size--;
	}
	return max_base_size;
}


int get_max_disc_num()
{
	return (get_max_base_size()-1)/2;
}

bool move_disc(int from, int to, int max_disc_num, int pegs[3][max_disc_num])
{
	bool moved = false;

	//find top disc index on 'from'
	int from_i; 
	for(from_i = max_disc_num-1; from_i >= 0; from_i-- )
	{
		if(pegs[from][from_i] != 0)
		{
			break;
		}
	}

	if(from_i == -1)// there is nothing to move
	{
		return false;
	}

	//find top disc index on 'to'
	int to_i; 
	for(to_i = max_disc_num-1; to_i >= 0; to_i-- )
	{
		if(pegs[to][to_i] != 0)
		{
			break;
		}
	}

	if(to_i == -1)// there is nothing in the way
	{
		pegs[to][to_i+1] = pegs[from][from_i];
		pegs[from][from_i] = 0;
		moved = true;
	}else{
		if(pegs[to][to_i] > pegs[from][from_i])
		{
			pegs[to][to_i+1] = pegs[from][from_i];
			pegs[from][from_i] = 0;
			moved = true;
		}
	}

	return moved;
}
