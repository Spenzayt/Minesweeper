#include <ctime>
#include <iostream>
#include <random>
#include <unordered_set>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <queue>


using namespace std;

HANDLE console_color;

// Struct for each case
struct Case {
	bool IsMine;
	bool IsReveal;
	int NumberOfNearMine;
};


// Game default Stats
const int ROWS = 9;
const int COLS = 9;
const int NumberOfMines = 10;

Case GameArray[ROWS][COLS];


// Default Case = "|_"
const string defaultCaseLine = "|";
const string defaultCaseBottom = "_";

const string defaultCase = defaultCaseLine + defaultCaseBottom;
const string defaultMine = "B";
const string defaultCaseReveal = "0";


// Draw Array
void reloadArray() {
	// Upper Line
	SetConsoleTextAttribute(console_color, 15);
	cout << " ";
	for (int i = 0; i < COLS; i++) {
		cout << defaultCaseBottom << " ";
	}
	cout << endl;

	// Draw Array
	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLS; x++) {
			if (GameArray[x][y].IsReveal) {
				if (GameArray[x][y].IsMine) {
					cout << defaultCaseLine;
					SetConsoleTextAttribute(console_color, 4);
					cout << defaultMine;
					SetConsoleTextAttribute(console_color, 15);
				}
				else if (GameArray[x][y].NumberOfNearMine > 0) {
					cout << defaultCaseLine;
					SetConsoleTextAttribute(console_color, 6);
					cout << to_string(GameArray[x][y].NumberOfNearMine);
					SetConsoleTextAttribute(console_color, 15);
				}
				else {
					cout << defaultCaseLine;
					SetConsoleTextAttribute(console_color, 2);
					cout << defaultCaseReveal;
					SetConsoleTextAttribute(console_color, 15);
				}
			}
			else {
				cout << defaultCase;
			}
		}
		cout << defaultCaseLine << endl;
	}
	cout << endl;
}


// Check if the coordinate is in the Array
bool IsInArray(int x, int y) {
	return (x >= 0 and x < COLS and y >= 0 and y < ROWS);
}


// Set Near Mines Number
void SetNearMines(int x, int y) {
	if (IsInArray(x + 1, y)) {
		GameArray[x + 1][y].NumberOfNearMine = GameArray[x + 1][y].NumberOfNearMine + 1;
	}
	if (IsInArray(x - 1, y)) {
		GameArray[x - 1][y].NumberOfNearMine = GameArray[x - 1][y].NumberOfNearMine + 1;
	}
	if (IsInArray(x, y + 1)) {
		GameArray[x][y + 1].NumberOfNearMine = GameArray[x][y + 1].NumberOfNearMine + 1;
	}
	if (IsInArray(x, y - 1)) {
		GameArray[x][y - 1].NumberOfNearMine = GameArray[x][y - 1].NumberOfNearMine + 1;
	}

	if (IsInArray(x + 1, y + 1)) {
		GameArray[x + 1][y + 1].NumberOfNearMine = GameArray[x + 1][y + 1].NumberOfNearMine + 1;
	}
	if (IsInArray(x + 1, y - 1)) {
		GameArray[x + 1][y - 1].NumberOfNearMine = GameArray[x + 1][y - 1].NumberOfNearMine + 1;
	}
	if (IsInArray(x - 1, y + 1)) {
		GameArray[x - 1][y + 1].NumberOfNearMine = GameArray[x - 1][y + 1].NumberOfNearMine + 1;
	}
	if (IsInArray(x - 1, y - 1)) {
		GameArray[x - 1][y - 1].NumberOfNearMine = GameArray[x - 1][y - 1].NumberOfNearMine + 1;
	}
}

// Place all Mines
void placeMines() {
	unordered_set<int> values;
	srand(time(0));

	// Generate unique mines positions
	while (values.size() < NumberOfMines) {
		values.insert(rand() % (ROWS * COLS));
	}

	// Place mines
	for (const int& value : values) {
		int y = value / COLS;
		int x = value % COLS;

		GameArray[x][y].IsMine = true;
		SetNearMines(x, y);
	}
	reloadArray();
}


// Create a default array with all mines
void InitializeArray() {
	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLS; x++) {
			GameArray[x][y] = { false, false, 0 };
		}
	}
	placeMines();
}


// Struct with X and Y coordinate 
struct Coordinate { int x; int y; };


// Ask an X and Y Coordinate
Coordinate askCoordinate() {
	Coordinate coord;
	bool isValid = false;

	while (isValid != true) {
		cout << "Give an X coordiante -> ";
		cin >> coord.x;
		coord.x--;

		cout << "Give an Y coordiante -> ";
		cin >> coord.y;
		coord.y--;

		isValid = IsInArray(coord.x, coord.y);

		if (isValid != true) {
			cout << "Coordinate aren't valid" << endl;
		}
	}
	return coord;
}

/*
if (cin.faul()) {
	cin.clear()

	cin.ignore(1000. \n)

*/


// Clear All Clear Space close
void foodFill(int x, int y) {
	if (IsInArray(x, y) == false) {
		return;
	}
	if (GameArray[x][y].IsReveal == true) {
		return;
	}


	if (GameArray[x][y].IsMine != true) {
		GameArray[x][y].IsReveal = true;

		if (GameArray[x][y].NumberOfNearMine == 0) {
			foodFill(x + 1, y);
			foodFill(x - 1, y);
			foodFill(x, y + 1);
			foodFill(x, y - 1);
		}
	}
}


int main() {
	// Color of the console 
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);


	// Draw Default Array
	InitializeArray();

	bool win = false;
	bool loose = false;

	while (win != true and loose != true) {
		// Ask for Coordinate
		Coordinate coord = askCoordinate();

		// Is already revealed?
		if (GameArray[coord.x][coord.y].IsReveal) {
			cout << "Already revealed" << endl;
			continue;
		}

		// Is on a Mine ?
		if (GameArray[coord.x][coord.y].IsMine) {
			GameArray[coord.x][coord.y].IsReveal = true;
			loose = true;
			reloadArray();
			cout << "You lose" << endl;
		}

		// Clear all space
		else {
			if (GameArray[coord.x][coord.y].NumberOfNearMine == 0) {
				cout << "Nothing" << endl;
				foodFill(coord.x, coord.y);
			}
			else {
				GameArray[coord.x][coord.y].IsReveal = true;
			}
			reloadArray();
		}
	}
}