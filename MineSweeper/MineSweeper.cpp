#include <iostream>
using namespace std;


string tab[8][8];

//Default Case = "|_"
string defaultCaseLine = "|";
string defaultCaseBottom = "_";


void InitializeArray() {
	//Default Value Array
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			tab[x][y] = defaultCaseLine + defaultCaseBottom;
		}
	}

	//Upper Line
	cout << " ";
	for (int i = 0; i < 8; i++) {
		cout << defaultCaseBottom << " ";
	}
	cout << endl;

	//Draw Array
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			cout << tab[x][y];
		}
		cout << defaultCaseLine << endl;
	}
	cout << endl;
}

void placeBombe() {
	while (i != 1) {
		int j = 1;
		int x = rand(0, 8);
		int y = rand(0, 8);
		if (tab)
		tab[x][y] = defaultCaseLine + "B";
	}
}


void updateArray() {
	//Upper Line
	cout << " ";
	for (int i = 0; i < 10; i++) {
		cout << defaultCaseBottom << " ";
	}
	cout << endl;

	//Draw Array
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 10; x++) {
			cout << tab[x][y];
		}
		cout << defaultCaseLine << endl;
	}
	cout << endl;
}


int askCoordinateX() {
	int x;

	cout << "X -> ";
	cin >> x;

	return x - 1;
}


int askCoordinateY() {
	int y;

	cout << "Y -> ";
	cin >> y;

	return y - 1;
}


void draw(int x, int y) {
	tab[x][y] = defaultCaseLine + "X";
	updateArray();
}


void erase(int x, int y) {
	tab[x][y] = defaultCaseLine + defaultCaseBottom;
	updateArray();
}


void eraseALL() {
	InitializeArray();
}

void drawLine(int x1, int y1, int x2, int y2) {

	int rx = x2 - x1;
	int ry = y2 - y1;

	if (x1 == x2) {
		for (int i = 1; i < ry + 1; i++) {
			tab[x1][i] = defaultCaseLine + "X";
		}
	}

	if (y1 == y2) {
		for (int i = 2; i < rx + 1; i++) {
			tab[i][y1] = defaultCaseLine + "X";
		}
	}

	else {
		cout << "The line must be straight" << endl;
	}

	updateArray();
}

int menu() {
	int mode = 0;
	cout << "Choose an Action :" << endl;
	cout << " - 1 : Draw" << endl;
	cout << " - 2 : Erase" << endl;
	cout << " - 3 : Erase Everything" << endl;
	cout << " - 4 : Draw Line" << endl;

	cout << " - 9 : Exit" << endl;

	cout << "Mode = ";
	cin >> mode;
	cout << endl;

	return mode;
}


int main() {
	InitializeArray();

	int exit = 0;

	int x = 1;
	int y = 1;
	int x2;
	int y2;

	while (exit != 1) {
		int mode = menu();

		if (mode == 9) {
			exit = 1;
		}

		if (mode == 1) {
			x = askCoordinateX();
			y = askCoordinateY();
			draw(x, y);
		}

		if (mode == 2) {
			x = askCoordinateX();
			y = askCoordinateY();
			erase(x, y);
		}

		if (mode == 3) {
			eraseALL();
		}

		if (mode == 4) {
			x = askCoordinateX();
			y = askCoordinateY();
			x2 = askCoordinateX();
			y2 = askCoordinateY();
			drawLine(x, y, x2, y2);
		}
	}
}