enum Position{
	X = 0,
	Y
};

#define SIZE 256

void handleInput();

int main(int argc, char *argv[])
{
	bool running = true;
	setlocale(LC_ALL, "");
	
	int cursor[] = { 0, 0 };
	int max[] = { 0, 0 };
	char ch;
	
	char input_memory[SIZE][SIZE] = { 0 };
	int input_memory_lengths[SIZE] = { 0 };
	int input_memory_size = 0;
	char input[SIZE] = { 0 };
	input[0] = '_';
	
	initscr();
	noecho();
	curs_set(FALSE);
	
	getmaxyx(stdscr, max[Y], max[X]);
	nonl();
	cursor[X] = 0;
	cursor[Y] = max[Y] - 1;
	
	char separator[max[X]] = { 0 };
	memset(separator, '=', sizeof(separator));
	
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);

	while(running)
	{
		// draw
		clear();
		
		if(input_memory_size <= max[Y] - 3)
		{
			for(unsigned i = 0; i < max[Y] && i < input_memory_size; i++)
				mvprintw(i, 0, input_memory[i]);
		}
		else
		{
			for(int i = input_memory_size - 1, y = max[Y] - 3; i >= 0 && y >= 0; i--, y--)
				mvprintw(y, 0, input_memory[i]);
		}
		
		attron(COLOR_PAIR(1));
		mvprintw(cursor[Y] - 1, 0, separator);
		attroff(COLOR_PAIR(1));
		
		mvaddstr(cursor[Y], 0, input);
		
		refresh();
		
		// get input
		ch = getch();
		
		// process input
		if(ch == '\r' && input_memory_size < SIZE)// press enter
		{
			input[cursor[X]] = 0;// replace cursor with null-terminator
			memcpy(input_memory[input_memory_size], input, SIZE);
			input_memory_lengths[input_memory_size] = strlen(input);
			input_memory_size++;
			
			memset(input, 0, sizeof(input));
			input[0] = '_';
			
			cursor[X] = 0;
			cursor[Y] = max[Y] - 1;
		}
		else if(ch >= 32 && ch <= 126 && cursor[X] < SIZE - 1)// pretty much any character
		{
			input[cursor[X]] = ch;
		
			// move cursor
			if(cursor[X] < SIZE - 1)
			{
				cursor[X]++;
				input[cursor[X]] = '_';
			}
			
			cursor[Y] = max[Y] - 1 - cursor[X]/max[X];
		}
		else if(ch == 127 && cursor[X] > 0)// delete character (backspace)
		{
			input[cursor[X]] = 0;
			
			cursor[X]--;
			
			input[cursor[X]] = '_';
		}
		else if(ch == 27)// quit (escape)
		{
			running = false;
		}
	}

	endwin(); // Restore normal terminal behavior
	
	return 0;
}
