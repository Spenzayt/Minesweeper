#include <ctime>
#include <iostream>
#include <random>
#include <unordered_set>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;

HANDLE console_color;

// Struct used by each cell of the grid
struct Cell {
	bool isMine = false;
	bool isRevealed = false;
	bool isFlagged = false;
	int adjacentMinesCount = 0;
};


// Game default settings
int ROWS = 9;
int COLS = 9;
int MINES_COUNT = 10;
Cell** gameGrid;


// Cell display symbols
const string displayCellLine = "| ";
const string displayCellBottom = "_ ";
const string displayCell = displayCellLine + displayCellBottom;
const string displayReveal = "0 ";
const string displayMine = "B ";
const string displayFlag = "F ";


// Frees the allocated memory for the game grid.
void freeGrid() {
	for (int i = 0; i < ROWS; i++) {
		delete[] gameGrid[i];
	}
	delete[] gameGrid;
}


// Center text in the console
void centerText(const string text, bool endline, int add) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int consoleWidth;

	// Get console width
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	// Calculate leading spaces
	int padding = (consoleWidth - (text.length() + add)) / 2;
	if (padding > 0) {
		if (endline == true) {
			cout << string(padding, ' ') << text << endl;
		}
		else if (endline == false) {
			cout << string(padding, ' ') << text;
		}
	}
	else {
		// If text is longer than console width, print normally
		if (endline == true) {
			cout << text << endl;
		}
		else if (endline == false) {
			cout << text << endl;
		}
	}
}


// Display the "MineSwepper" title
void displayTitle() {
	centerText("   ____ ___   (_)____   ___   _____ _      __ ___   ___   ____   ___   _____", true, 0);
	centerText("  / __ `__ \\ / // __ \\ / _ \\ / ___/| | /| / // _ \\ / _ \\ / __ \\ / _ \\ / ___/", true, 0);
	centerText(" / / / / / // // / / //  __/(__  ) | |/ |/ //  __//  __// /_/ //  __// /    ", true, 0);
	centerText("/_/ /_/ /_//_//_/ /_/ \\___/ \\___/  |__/|__/ \\___/ \\___// .___/ \\___//_/     ", true, 0);
	centerText("                                                      /_/                   ", true, 0);
}

void clearScreen() {
	system("cls");
}


// Check if the player has won
bool checkWinCondition() {
	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLS; x++) {
			if (gameGrid[x][y].isMine == false && gameGrid[x][y].isRevealed == false) {
				return false;
			}
		}
	}
	cout << "You win !" << endl;
	return true;
}


// Display the upper numbers
void displayUpperNumbers() {
	centerText("  ", false, (COLS * 4));
	for (int i = 0; i < COLS; i++) {
		if (i < 9) {
			cout << (i + 1) << "   ";
		}
		else if (i >= 9) {
			cout << (i + 1) << "  ";
		}
	}
	cout << endl;
}


// Display the upper line of the game grid
void displayUpperLineGrid() {
	centerText("  ", false, (COLS * 4));
	for (int i = 0; i < COLS; i++) {
		cout << displayCellBottom << "  ";
	}
	cout << endl << endl;
}


// Display flagged cell
void displayFlaggedCell() {
	SetConsoleTextAttribute(console_color, 1);
	cout << displayFlag;
	SetConsoleTextAttribute(console_color, 15);
}


// Display revealed mine cell
void displayMineCell() {
	SetConsoleTextAttribute(console_color, 4);
	cout << displayMine;
	SetConsoleTextAttribute(console_color, 15);
}


// Display revealed cell with adjacent mines count
void displayAdjacentMineCountCell(int count) {
	SetConsoleTextAttribute(console_color, 6);
	cout << to_string(count) << " ";
	SetConsoleTextAttribute(console_color, 15);
}


// Display revealed empty cell
void displayEmptyCell() {
	SetConsoleTextAttribute(console_color, 2);
	cout << displayReveal;
	SetConsoleTextAttribute(console_color, 15);
}

// Display hidden cell
void displayHiddenCell() {
	cout << displayCellBottom;
}


// Display a single cell based on its current state
void displayGridCell(int x,int y) {
	if (x != 0) {
		cout << displayCellLine;
	}
	if (gameGrid[x][y].isFlagged == true && gameGrid[x][y].isRevealed == false) {
		displayFlaggedCell();
	}
	else if (gameGrid[x][y].isRevealed == true) {
		if (gameGrid[x][y].isMine == true) {
			displayMineCell();
		}
		else if (gameGrid[x][y].adjacentMinesCount > 0) {
			displayAdjacentMineCountCell(gameGrid[x][y].adjacentMinesCount);
		}
		else {
			displayEmptyCell();
		}
	}
	else {
		displayHiddenCell();
	}
}

// Display the full game grid
void displayGrid() {
	clearScreen();
	displayTitle();

	displayUpperNumbers();
	displayUpperLineGrid();

	for (int y = 0; y < ROWS; y++) {
		centerText(to_string(y + 1) + "  ", false, (COLS * 4) + 6);
		cout << displayCellLine;
		for (int x = 0; x < COLS; x++) {
			displayGridCell(x,y);
		}
		cout << displayCellLine << endl;
		cout << endl;
	}
	cout << endl;
}


// Verify if the coordinates are in the grid
bool isInGrid(int x, int y) {
	return (x >= 0 and x < COLS and y >= 0 and y < ROWS);
}


// Update adjacent cells mine count
void updateAdjacentMineCount(int x, int y) {
	if (isInGrid(x + 1, y)) { gameGrid[x + 1][y].adjacentMinesCount++; }
	if (isInGrid(x - 1, y)) { gameGrid[x - 1][y].adjacentMinesCount++; }
	if (isInGrid(x, y + 1)) { gameGrid[x][y + 1].adjacentMinesCount++; }
	if (isInGrid(x, y - 1)) { gameGrid[x][y - 1].adjacentMinesCount++; }
	if (isInGrid(x + 1, y + 1)) { gameGrid[x + 1][y + 1].adjacentMinesCount++; }
	if (isInGrid(x + 1, y - 1)) { gameGrid[x + 1][y - 1].adjacentMinesCount++; }
	if (isInGrid(x - 1, y + 1)) { gameGrid[x - 1][y + 1].adjacentMinesCount++; }
	if (isInGrid(x - 1, y - 1)) { gameGrid[x - 1][y - 1].adjacentMinesCount++; }
}

// Randomly place mines on the grid
void placeMines() {
	unordered_set<int> values;
	srand(time(0));

	// Generate unique mines positions
	while (values.size() < MINES_COUNT) {
		values.insert(rand() % (ROWS * COLS));
	}

	// Place mines
	for (const int& value : values) {
		int y = value / COLS;
		int x = value % COLS;

		gameGrid[x][y].isMine = true;
		updateAdjacentMineCount(x, y);
	}
	displayGrid();
}


// Initialize the grid with default settings for each cell
void initializeGrid() {
	gameGrid = new Cell * [ROWS];
	for (int i = 0; i < ROWS; i++) {
		gameGrid[i] = new Cell[COLS];
	}

	for (int y = 0; y < ROWS; y++) {
		for (int x = 0; x < COLS; x++) {
			gameGrid[x][y] = { false, false, false, 0 };
		}
	}
	placeMines();
}


// Struct for coordinates
struct Coordinate { int x; int y; };


// Check if the input is valid (integer) and clear any bad input
bool validateInput() {
	if (cin.fail()) {
		cin.clear();
		cin.ignore(1000, '\n');
		return false;
	}
	return true;
}


// Adjust the coordinate (subtracts 1)
int adjustCoordinate(int coord) {
	return coord - 1;
}


// Prompt and validate a single coordinate
int promptForCoordinate(string axis, int difficultyMode) {
	int coord;
	while (true) {
		if (difficultyMode == 1) {
			cout << "Give a " << axis << " coordinate (1 to 9) -> ";
		}
		else if (difficultyMode == 2) {
			cout << "Give a " << axis << " coordinate (1 to 16) -> ";
		}
		else if (difficultyMode == 3) {
			cout << "Give a " << axis << " coordinate (1 to 30) -> ";
		}

		cin >> coord;

		if (validateInput() == true) {
			coord = adjustCoordinate(coord);
			break;
		}
		else {
			cout << "Invalid choice. Please enter a number." << endl;
		}
	}
	return coord;
}


// Prompt the player for both X and Y coordinates and validate grid range
Coordinate promptAndValidateCoordinate(int difficultyMode) {
	Coordinate coord;

	while (true) {
		coord.x = promptForCoordinate("X", difficultyMode);
		coord.y = promptForCoordinate("Y", difficultyMode);

		if (isInGrid(coord.x, coord.y)) {
			break;
		}
		else {
			if (difficultyMode == 1) {
				cout << "Coordinates must be between 1 and 9." << endl;
			}
			else if (difficultyMode == 2) {
				cout << "Coordinates must be between 1 and 16." << endl;
			}
			else if (difficultyMode == 3) {
				cout << "Coordinates must be between 1 and 30." << endl;
			}
		}
	}
	return coord;
}


// Reveal all adjacent empty cells with a flood fill
void revealEmptyCells(int x, int y) {
	if (isInGrid(x, y) == false) {
		return;
	}
	if (gameGrid[x][y].isRevealed == true) {
		return;
	}


	if (gameGrid[x][y].isMine != true) {
		gameGrid[x][y].isRevealed = true;

		if (gameGrid[x][y].adjacentMinesCount == 0) {
			revealEmptyCells(x + 1, y);
			revealEmptyCells(x - 1, y);
			revealEmptyCells(x, y + 1);
			revealEmptyCells(x, y - 1);

			revealEmptyCells(x + 1, y + 1);
			revealEmptyCells(x - 1, y - 1);
			revealEmptyCells(x - 1, y + 1);
			revealEmptyCells(x + 1, y - 1);
		}
	}
}


// Display the action menu
int displayActionMenu() {
	int choice;

	while (true) {
		cout << "Choose an action :" << endl;
		cout << "1. Reveal a cell" << endl;
		cout << "2. Place a flag" << endl;
		cout << "3. Return to the main menu" << endl;

		cin >> choice;

		if (validateInput() == true && (choice == 1 or choice == 2 or choice == 3)) {
			break;
		}
		else {
			cout << "Invalid choice. Please select 1, 2, or 3." << endl;
		}
	}
	return choice;
}


// Place a flag on a cell in the grid
void placeFlag(int difficultyMode) {
	Coordinate coord = promptAndValidateCoordinate(difficultyMode);

	if (gameGrid[coord.x][coord.y].isRevealed == false) {
		gameGrid[coord.x][coord.y].isFlagged = !gameGrid[coord.x][coord.y].isFlagged;
		displayGrid();
	}
	else {
		cout << "Cannot place a flag on a revealed cell." << endl;
	}
}


// Press space bar to return to the main menu
void PressSpaceToReturnMainMenu(bool &gameOver) {
	cout << "You lose" << endl;
	cout << "Press space bar to return to the main menu" << endl;

	// Input
	char input = _getch();

	while (true) {
		if (input == ' ') {
			gameOver = true;
			break;
		}
	}
}


// Reveal a cell in the grid
void revealCell(bool &gameOver, int difficultyMode) {
	Coordinate coord = promptAndValidateCoordinate(difficultyMode);

	// Check if the cell has already been revealed
	if (gameGrid[coord.x][coord.y].isRevealed == true) {
		cout << "Cell already revealed" << endl;
		return;
	}

	// Check if the selected cell is a mine
	if (gameGrid[coord.x][coord.y].isMine == true) {
		gameGrid[coord.x][coord.y].isRevealed = true;
		displayGrid();
		PressSpaceToReturnMainMenu(gameOver);
	}
	else {
		// If the cell has no adjacent mines, reveal surrounding empty cells
		if (gameGrid[coord.x][coord.y].adjacentMinesCount == 0) {
			revealEmptyCells(coord.x, coord.y);
		}
		else {
			gameGrid[coord.x][coord.y].isRevealed = true;
		}
		displayGrid();
	}
}


// Process the chosen action by the user
void processAction(int action, bool &gameOver, int difficultyMode) {
	switch (action) {
	case 1: // Reveal a cell
		revealCell(gameOver, difficultyMode);
		break;
	case 2: // Place a flag
		placeFlag(difficultyMode);
		break;
	case 3: // Return to the main menu
		gameOver = true;
		break;
	default:
		cout << "Invalid action. Please try again." << endl; // Handle invalid action
		break;
	}
}


// Display the Difficulty Menu
int displayDifficultyMenu() {
	int choice = 0;
	int dev = 0;

	while (true) {
		clearScreen();
		displayTitle();

		if (dev == 1) {
			cout << "This difficulty is in development" << endl;
			dev = 0;
		}
		else {
		}


		centerText("========================================", true, 0);
		centerText("=            Difficulty Menu           =", true, 0);
		centerText("========================================", true, 0);
		if (choice == 0) {
			centerText("> Easy", true, 0);
			centerText("  Medium", true, 0);
			centerText("  Hard", true, 0);
			centerText("  Infinite", true, 0);
		}
		else if (choice == 1) {
			centerText("  Easy", true, 0);
			centerText("> Medium", true, 0);
			centerText("  Hard", true, 0);
			centerText("  Infinite", true, 0);
		}
		else if (choice == 2) {
			centerText("  Easy", true, 0);
			centerText("  Medium", true, 0);
			centerText("> Hard", true, 0);
			centerText("  Infinite", true, 0);
		}
		else if (choice == 3) {
			centerText("  Easy", true, 0);
			centerText("  Medium", true, 0);
			centerText("  Hard", true, 0);
			centerText("> Infinite", true, 0);
		}
		centerText("========================================", true, 0);

		// Input
		char input = _getch();

		if ((input == 'z' or input == 'Z') and choice > 0) {
			choice--;
		}
		else if ((input == 's' or input == 'S') and choice < 3) {
			choice++;
		}
		else if (input == ' ') {
			if (choice == 0) {
				return 1;
			}
			else if (choice == 1) {
				return 2;
			}
			else if (choice == 2) {
				return 3;
			}
			else if (choice == 3) {
				dev = 1;
			}
		}
	}
}


// Display the Main Menu
int displayMainMenu() {
	int choice = 0;

	while (true) {
		clearScreen();
		displayTitle();

		centerText("========================================", true, 0);
		centerText("=               Main Menu              =", true, 0);
		centerText("========================================", true, 0);
		if (choice == 0) {
			centerText("> Play", true, 0);
			centerText("  Exit", true, 0);
		}
		else if (choice == 1) {
			centerText("  Play", true, 0);
			centerText("> Exit", true, 0);
		}
		centerText("========================================", true, 0);

		// Input
		char input = _getch();

		if (input == 'z' or input == 'Z') {
			choice = 0;
		}
		else if (input == 's' or input == 'S') {
			choice = 1;
		}
		else if (input == ' ') {
			if (choice == 0) {
				int difficultyMode = 0;
				difficultyMode = displayDifficultyMenu();
				while (difficultyMode == 0) {
					continue;
				}
				return difficultyMode;
			}
			else if (choice == 1) {
				exit(0);
			}
		}
	}
}


// Start a New Game
void start(int difficultyMode) {
	if (difficultyMode == 1) {
		ROWS = 9;
		COLS = 9;
		MINES_COUNT = 10;
	}
	else if (difficultyMode == 2) {
		ROWS = 16;
		COLS = 16;
		MINES_COUNT = 40;
	}
	else if (difficultyMode == 3) {
		ROWS = 30;
		COLS = 30;
		MINES_COUNT = 99;
	}

	initializeGrid();
	bool gameOver = false;

	while (gameOver == false) {
		if (checkWinCondition() == true) {
			gameOver = true;
			break;
		}
		else {
			int action = displayActionMenu();
			processAction(action, gameOver, difficultyMode);
		}
	}
}


int main() {
	// Color of the console 
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);

	while (true) {
		int difficultyMode = 0;
		difficultyMode = displayMainMenu();
		while (difficultyMode == 0) {
			continue;
		}
		start(difficultyMode);
		freeGrid();
	}
}