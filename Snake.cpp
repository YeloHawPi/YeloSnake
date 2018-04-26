/**

#########################################################
#                                                       #
#                  Created by YeloHawPi                 #
#                                                       #
#########################################################

**/

#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stropts.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string>

using namespace std;

string banner[] = {
"   _____                _           _   _           ",
"  / ____|              | |         | | | |          ",
" | |     _ __ ___  __ _| |_ ___  __| | | |__  _   _ ",
" | |    | '__/ _ \\/ _` | __/ _ \\/ _` | | '_ \\| | | |",
" | |____| | |  __/ (_| | ||  __/ (_| | | |_) | |_| |",
"  \\_____|_|  \\___|\\__,_|\\__\\___|\\__,_| |_.__/ \\__, |",
"                                               __/ |",
" __     __  _       _    _                ____|___/ ",
" \\ \\   / / | |     | |  | |              |  __ (_)  ",
"  \\ \\_/ /__| | ___ | |__| | __ ___      _| |__) |   ",
"   \\   / _ \\ |/ _ \\|  __  |/ _` \\ \\ /\\ / /  ___/ |  ",
"    | |  __/ | (_) | |  | | (_| |\\ V  V /| |   | |  ",
"    |_|\\___|_|\\___/|_|  |_|\\__,_| \\_/\\_/ |_|   |_|  ",
"                                                    ",
"                                                    ",
"         W                                          ",
"         |                                          ",
"  A <- Move -> D                                    ",
"         |                                          ",
"         S                               X - Exit   "
};

int n = 0;

string sbanner[] = {
"      ___           ___           ___           ___           ___     ",
"     /  /\\         /__/\\         /  /\\         /__/|         /  /\\    ",
"    /  /:/_        \\  \\:\\       /  /::\\       |  |:|        /  /:/_   ",
"   /  /:/ /\\        \\  \\:\\     /  /:/\\:\\      |  |:|       /  /:/ /\\  ",
"  /  /:/ /::\\   _____\\__\\:\\   /  /:/~/::\\   __|  |:|      /  /:/ /:/_ ",
" /__/:/ /:/\\:\\ /__/::::::::\\ /__/:/ /:/\\:\\ /__/\\_|:|____ /__/:/ /:/ /\\",
" \\  \\:\\/:/~/:/ \\  \\:\\~~\\~~\\/ \\  \\:\\/:/__\\/ \\  \\:\\/:::::/ \\  \\:\\/:/ /:/",
"  \\  \\::/ /:/   \\  \\:\\  ~~~   \\  \\::/       \\  \\::/~~~~   \\  \\::/ /:/ ",
"   \\__\\/ /:/     \\  \\:\\        \\  \\:\\        \\  \\:\\        \\  \\:\\/:/  ",
"     /__/:/       \\  \\:\\        \\  \\:\\        \\  \\:\\        \\  \\::/   ",
"     \\__\\/         \\__\\/         \\__\\/         \\__\\/         \\__\\/    "
};

string gbanner[] = {
" _____                        _____                ",
"|  __ \\                      |  _  |               ",
"| |  \\/ __ _ _ __ ___   ___  | | | |_   _____ _ __ ",
"| | __ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|",
"| |_\\ \\ (_| | | | | | |  __/ \\ \\_/ /\\ V /  __/ |   ",
" \\____/\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|   "
};

const int width = 20;
const int height = 20;

bool Game_Over;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Setup();
void Draw(); 
void Input();
void Logic();

int _getch();
int _kbhit();

int main(int argc, char const *argv[])
{
	system("clear");
	for (int i = 0; i < 11; i++)
	{
		cout << sbanner[i] << endl;
	}
	sleep(5);

	Setup();

	while (!Game_Over)
	{
		sleep(1);
		Draw();
		Input();
		Logic();
	}

	system("clear");
	for (int i = 0; i < 6; i++)
	{
		cout << gbanner[i] << endl;
	}
	sleep(5);
	system("clear");

	return 0;
}

void Setup()
{
	Game_Over = false;
	dir = STOP;
	x = width / 2 - 1;
	y = height / 2 - 1;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void Draw()
{
	system("clear");

	// Верхняя граница

	for (int i = 0; i < width + 1; i++) { cout << "#"; }

	cout << endl;

	// Боковые границы

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0 || j == width - 1) 

			{ cout << "#"; if (j == width - 1 && n < 20) { cout << banner[n]; n++;} if (n >= 20) {n = 0;}}


			if (i == y && j == x) 
			{ cout << "0"; }

			else if (i == fruitY && j == fruitX)
			{ cout << "F"; }
			
			else
			{ 
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i) 
					{
						print = true;
						cout << "o";
					} 
				}

				if (!print) {cout << " ";} 
			}
		}

		cout << endl;
	}

	// Нижняя граница

	for (int i = 0; i < width + 1; i++) { cout << "#"; }


	cout << endl;
	cout << "Score: " << score << endl;
}

void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
			case 'a':
				dir = LEFT;
			break;

			case 'd':
				dir = RIGHT;
			break;

			case 'w':
				dir = UP;
			break;

			case 's':
				dir = DOWN;
			break;

			case 'x':
				system("clear");
				Game_Over = true;
			break;
		}
	}
}

void Logic()
{
	int lastX = tailX[0];
	int lastY = tailY[0];
	int last2X, last2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		last2X = tailX[i];
		last2Y = tailY[i];
		tailX[i] = lastX;
		tailY[i] = lastY;
		lastX = last2X;
		lastY = last2Y;
	}

	switch (dir)
	{
		case LEFT:
			x--;
		break;

		case RIGHT:
			x++;
		break;

		case UP:
			y--;
		break;

		case DOWN:
			y++;
		break;
	}

	if (x >= width - 1)
		x = 0;
	else if (x < 0)
		x = width - 2;

	if (y >= height)
		y = 0;
	else if (y < 0)
		y = height - 1;

	for (int i = 0; i < nTail; i++)
	{
		if (tailX[i] == x && tailY[i] == y) 
		{
			Game_Over = true;
		}
	}

	if (fruitX == x && fruitY == y)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
}

int _getch( ) {
	struct termios oldt,
	newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

int _kbhit() {
static const int STDIN = 0;
static bool initialized = false;

if (! initialized) {
    termios term;
    tcgetattr(STDIN, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN, TCSANOW, &term);
    setbuf(stdin, NULL);
    initialized = true;
}

int bytesWaiting;
ioctl(STDIN, FIONREAD, &bytesWaiting);
return bytesWaiting;
}
