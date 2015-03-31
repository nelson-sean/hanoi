#include <stdlib.h>
#include <ncurses.h>

void init_ncurses();
void draw_window(int max_disc_num, int pegs[3][max_disc_num], int selected);
int get_max_base_size();
int get_max_disc_num();
void move_disc(int from, int to, int max_disc_num, int pegs[3][max_disc_num]);

int main()
{

	init_ncurses();

	// initialize pegs
	int max_disc_num = get_max_disc_num();
	int pegs[3][max_disc_num];

	int disc_num = 5;

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
			}
		}else/* a peg is selected */{
			switch(getch())
			{
				case 'j':
					move_disc(selected, 0, max_disc_num, pegs);
					selected = -1;
					break;
				case 'k':
					move_disc(selected, 1, max_disc_num, pegs);
					selected = -1;
					break;
				case 'l':
					move_disc(selected, 2, max_disc_num, pegs);
					selected = -1;
					break;
			}
		}// end else

		draw_window(max_disc_num, pegs, selected);

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
	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	
}

void draw_window(int max_disc_num, int pegs[3][max_disc_num], int selected)
{

	clear();

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

void move_disc(int from, int to, int max_disc_num, int pegs[3][max_disc_num])
{
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
		return;
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
	}else{
		if(pegs[to][to_i] > pegs[from][from_i])
		{
			pegs[to][to_i+1] = pegs[from][from_i];
			pegs[from][from_i] = 0;
		}
	}
}
