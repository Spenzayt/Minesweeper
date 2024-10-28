#include <ctime>
#include <iostream>
#include <random>
#include <unordered_set>
#include <cstdlib>
#include <string>


using namespace std;

string tab[9][9];

// Default Case = "|_"
string defaultCaseLine = "|";
string defaultCaseBottom = "_";


void updateArray() {
	// Upper Line
	cout << " ";
	for (int i = 0; i < 9; i++) {
		cout << defaultCaseBottom << " ";
	}
	cout << endl;

	// Draw Array
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			cout << tab[x][y];
		}
		cout << defaultCaseLine << endl;
	}
	cout << endl;
}


void placeBomb() {
	unordered_set<int> values;
	srand(time(0));

	// Generate unique bomb positions
	while (values.size() < 10) {
		values.insert(rand() % 81);
	}

	// Place bombs
	for (const int& value : values) {
		int y = value / 9;
		int x = value % 9;

		tab[x][y] = defaultCaseLine + "B";
	}
	updateArray();
}


void InitializeArray() {
	// Default Value Array
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			tab[x][y] = defaultCaseLine + defaultCaseBottom;
		}
	}
	placeBomb();
}


//
struct Coordinate { int x; int y; };


bool IsInArray(int x, int y) {
	if (x < 0 or x > 8 or y < 0 or y > 8) {
		return false;
	}
	else
		return true;
}


Coordinate askCoordinate() {
	Coordinate coord;
	bool isValid = false;

	while (isValid != true) {
		cout << "Give an X coordiante -> ";
		cin >> coord.x;
		coord.x = coord.x - 1;

		cout << "Give an Y coordiante -> ";
		cin >> coord.y;
		coord.y = coord.y - 1;

		isValid = IsInArray(coord.x, coord.y);

		if (isValid != true) {
			cout << "Coordinate aren't valid" << endl;
		}
	}
	return coord;
}


bool OnBomb(int x, int y) {
	if (tab[x][y] == "|B") {
		return true;
	}
	if (tab[x][y] != "|B") {
		return false;
	}
}


int NearBomb(int x, int y) {
	int nearBombNumber = 0;

	if (tab[x + 1][y] == "|B") {
		nearBombNumber++;
	}
	if (tab[x - 1][y] == "|B") {
		nearBombNumber++;
	}
	if (tab[x][y + 1] == "|B") {
		nearBombNumber++;
	}
	if (tab[x][y - 1] == "|B") {
		nearBombNumber++;
	}

	return nearBombNumber;
}

int clearSpace(int x, int y) {
	tab[x][y] = "|X";

	if (IsInArray(x + 1, y) == true) {
		tab[x + 1][y] = "|X";
	}
	if (IsInArray(x - 1, y) == true) {
		tab[x + 1][y] = "|X";
	}
	if (IsInArray(x, y + 1) == true) {
		tab[x + 1][y] = "|X";
	}
	if (IsInArray(x, y - 1) == true) {
		tab[x + 1][y] = "|X";
	}
	

	updateArray();
	return 0;
}


int main() {
	InitializeArray();

	bool win = false;
	bool loose = false;

	while (win != true and loose != true) {
		// Ask for Coordinate
		Coordinate coord = askCoordinate();
		
		//Is on a Bomb ?
		bool dead = OnBomb(coord.x, coord.y);
		if (dead == true) {
			loose = true;
			cout << "You lose" << endl;
		}

		//Is near a Bomb ?
		int nearBombNumber = NearBomb(coord.x, coord.y);
		if (nearBombNumber != 0) {
			tab[coord.x][coord.y] = defaultCaseLine + to_string(nearBombNumber);
			updateArray();
		}

		//Clear all near space if nothing close
		else if (nearBombNumber == 0) {
			cout << "Nothing" << endl;
			clearSpace(coord.x, coord.y);
		}
	}
}