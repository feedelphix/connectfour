#include <ncurses.h>
#include <stdlib.h>

#define SIZE 16
#define seed 1

char items[SIZE][SIZE];
char itemsToAdd[] = {'a', 'b', 'c'};
char selectedItems[4][2];
int cursory, cursorx, selectedItemsCount, score = 0;

int selectedCheck() {
	int y, x, nextY, nextX;
	int connectedCount = 1; // as we compare only 4 but not more for now, it compare 3 xs and ys variables of current item with next and adds 1 on success. we get true on 4 if all goes well starting with 1 and not 0
	char charToCheck = items[selectedItems[0][0]][selectedItems[0][1]];
	char nextCharToCheck;
	for (int itemToCheck = 0; itemToCheck < 3; itemToCheck++) {
		y = selectedItems[itemToCheck][0];
		x = selectedItems[itemToCheck][1];
		nextY = selectedItems[itemToCheck + 1][0];
		nextX = selectedItems[itemToCheck + 1][1];
		nextCharToCheck = items[selectedItems[itemToCheck + 1][0]][selectedItems[itemToCheck + 1][1]];
		if ((y + 1 == nextY || y - 1 == nextY || y == nextY && x + 1 == nextX || x - 1 == nextX || x == nextX) && charToCheck == nextCharToCheck) {
			connectedCount++;
		}
	}
	if (connectedCount >= 4) return true;
	return false;
}

void clearSelected(bool destroy) {
	for (int currentItem = 0; currentItem < 4; currentItem++) {
		if (destroy) {
			items[selectedItems[currentItem][0]][selectedItems[currentItem][1]] = itemsToAdd[rand() % sizeof(itemsToAdd)];
			score++;
		}
		selectedItems[currentItem][0] = -1;
		selectedItems[currentItem][1] = -1;
	}
	selectedItemsCount = 0;
}

void output() {
	clear();
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			for (int itemToCheck = 0; itemToCheck < 4; itemToCheck++) {
				if (y == selectedItems[itemToCheck][0] && x == selectedItems[itemToCheck][1]) attron(COLOR_PAIR(3));
			}
			if (y == cursory && x == cursorx) attron(COLOR_PAIR(2));
			addch(items[y][x]);
			attron(COLOR_PAIR(1));
		}
		addch('\n');
	}
	printw("Score: %d\n", score);
	refresh();
}

void init() {
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			items[y][x] = itemsToAdd[rand() % sizeof(itemsToAdd)];
		}
	}
	clearSelected(false); // makes selected items to not appear on output
}

int main() {
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK); // default color
	init_pair(2, COLOR_BLACK, COLOR_WHITE); // cursored item color
	init_pair(3, COLOR_WHITE, COLOR_RED); // selected items color
	srand(seed);
	init();
	bool quit;
	while(!quit) {
		output();
		switch(getch()) {
			case 'q':
				quit = true;
				break;
			case 'k':
			case '8':
				if (cursory - 1 != -1) cursory--;
				break;
			case 'h':
			case '4':
				if (cursorx - 1 != -1) cursorx--;
				break;
			case '6':
			case 'l':
				if (cursorx + 1 != SIZE) cursorx++;
				break;
			case '2':
			case 'j':
				if (cursory + 1 != SIZE) cursory++;
				break;
			case '7':
				if (cursory - 1 != -1 && cursorx - 1 != -1) {
					cursory--;
					cursorx--;
				}
				break;
			case '9':
				if (cursory - 1 != -1 && cursorx + 1 != SIZE) {
					cursory--;
					cursorx++;
				}
				break;
			case '1':
				if (cursory + 1 != SIZE && cursorx - 1 != -1) {
					cursory++;
					cursorx--;
				}
				break;
			case '3':
				if (cursory + 1 != SIZE && cursorx + 1 != SIZE) {
					cursory++;
					cursorx++;
				}
				break;
			case '5':
			case '\n':
				if (cursory == selectedItems[selectedItemsCount - 1][0] && cursorx == selectedItems[selectedItemsCount - 1][1]) {
					selectedItemsCount--;
					selectedItems[selectedItemsCount][0] = -1;
					selectedItems[selectedItemsCount][1] = -1;
				} else {
					if (selectedItemsCount < 4) {
						selectedItems[selectedItemsCount][0] = cursory;
						selectedItems[selectedItemsCount][1] = cursorx;
						selectedItemsCount++;
					} else {
						clearSelected(selectedCheck());
					}
				}
				break;
			case 'c':
				clearSelected(false);
				break;
		}
	}
	endwin();
	return 0;
}
