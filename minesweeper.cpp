#include <iostream>
#include <string>
#include <vector>
#include "randgen.h"

using namespace std;


struct MineCell { // I used this struct in order to facilitate the operations made on a cell and solving the problem without using more than one matrix
	int cellNo; // cellNo determines the cell's number in the board. EXAMPLE: if we had a 3x3 board cellNo's would be like this: 0 1 2 / 3 4 5 / 6 7 8
	bool isBomb; // isBomb = bomb status
	int BombsAround; // how many bombs around that cell
	bool isOpened; // whether it was opened by option 2 before or not
};

int searchinvector(vector<int> domain, int x) { // counts the integer x in a vector and returns the result
	int counter = 0;
	for (int i = 0; i < domain.size(); i++) {
		if (domain[i] == x) { counter++; }
	}
	return counter;
}

int searchNearbyBombs(vector<int> bombs, int a, int i, int j, int x, int y) { 
	// vector bombs = cell numbers of the bombs, a = cell number, (i,j) = (row,column) coordinates, x = rows, y = columns
	// this function looks nearby cells of given coordinate and checks all nearby cells for whether they have bomb or not and returns the number that will be shown on that cell

	vector<int> neighborCellNos;

	// I used these if parts to classify the current cell according to it's specific location such as in corner, on edge or none of them. 
	// This will facilitate the process of looking for nearby bombs.
	// Cell number (single integer) of nearby cells are stored in a vector in order to check whether they contain bomb or not (look at the for loop at the end of this function).

	if (i==0) { //first row
		if (j==0) { //up left corner
			neighborCellNos.push_back(a+1); //right
			neighborCellNos.push_back(a+y); //below
			neighborCellNos.push_back(a+y+1); //lower right
		}
		else if (j==y-1) { //up right corner
			neighborCellNos.push_back(a-1); //left
			neighborCellNos.push_back(a+y); //below
			neighborCellNos.push_back(a+y-1); //lower left
		}
		else { //first row, between corners
			neighborCellNos.push_back(a-1); //left
			neighborCellNos.push_back(a+1); //right
			neighborCellNos.push_back(a+y-1); //lower left
			neighborCellNos.push_back(a+y); //below
			neighborCellNos.push_back(a+y+1); // lower right
		}
	}

	else if (i==x-1) { //last row
		if (j==0) { //down left corner
			neighborCellNos.push_back(a+1); //right
			neighborCellNos.push_back(a-y); //above
			neighborCellNos.push_back(a-y+1); //upper right
		}
		else if (j==y-1) { //down right corner
			neighborCellNos.push_back(a-1); //left
			neighborCellNos.push_back(a-y); //above
			neighborCellNos.push_back(a-y-1); //upper left
		}
		else { //last row, between corners
			neighborCellNos.push_back(a-1); //left
			neighborCellNos.push_back(a+1); //right
			neighborCellNos.push_back(a-y-1); //upper left
			neighborCellNos.push_back(a-y); //above
			neighborCellNos.push_back(a-y+1); // upper right
		}
	}

	else { // rows between first and last
		if (j==0) { //first column, between corners
			neighborCellNos.push_back(a-y); //above
			neighborCellNos.push_back(a-y+1); //upper right
			neighborCellNos.push_back(a+1); //right
			neighborCellNos.push_back(a+y+1); // lower right
			neighborCellNos.push_back(a+y); //below
		}
		else if (j==y-1) { //last column, between corners
			neighborCellNos.push_back(a-y); //above
			neighborCellNos.push_back(a-y-1); //upper left
			neighborCellNos.push_back(a-1); //left
			neighborCellNos.push_back(a+y-1); //lower left
			neighborCellNos.push_back(a+y); //below
		}
		else { //surrounded cells that have 8 neighbors)
			neighborCellNos.push_back(a-1); //left
			neighborCellNos.push_back(a-y-1); //upper left
			neighborCellNos.push_back(a-y); //above
			neighborCellNos.push_back(a-y+1); // upper right
			neighborCellNos.push_back(a+1); //right
			neighborCellNos.push_back(a+y+1); // lower right
			neighborCellNos.push_back(a+y); //below
			neighborCellNos.push_back(a+y-1); //lower left	
		}
	}

	int nearbybombs = 0;

	// In this for loop, neighbor cells' cell numbers are checked for their bomb status by looking the 'bombs' vector
	for (int i = 0; i < neighborCellNos.size(); i++) { 
		if (searchinvector(bombs,neighborCellNos[i]) == 1) { nearbybombs++; }
	}
	
	return nearbybombs;

}

void printStartingBoard(int x, int y) { // this function prints the empty board that contains only 'X's at the beginning.
	for (int i = 0; i < x; i++){
		for (int j = 0; j < y; j++){
			cout << "X ";
		}
		cout << endl;
	}
	cout << endl;
}


int main() {

	int board_row, board_column, nrMines;
	cout << "Give the dimensions of the matrix: "; cin >> board_row >> board_column;
	cout << "How many bombs: "; cin >> nrMines;
	int area = board_row*board_column;

	while (true) { // validity check of entered number of mines 
		if (nrMines < 1) {
			cout << "The number of bombs could not be less than one. Please give the number of bombs again: " ;
			cin >> nrMines;
		}

		else {
			if (nrMines >= area) {
				cout << "The number of bombs can not be greater than the whole number of cells minus one. Please give the number of bombs again: " ;
				cin >> nrMines;
			}
			else { break; }
		}
	} 

	vector<int> cellNumbers(area); // creates a vector for storing cell numbers. EXAMPLE: cellNumbers vector of 2x2 table = [0,1,2,3]
	for (int i = 0; i < area; i++) {
		cellNumbers[i] = i;
	}

	vector<int> mineNumbers; // a vector to store cell numbers that will have bomb.

	while (mineNumbers.size() < nrMines) { //creating a vector (minenumbers) consists of random numbers which destinates the cells that will have bomb by choosing random integers from cellNumbers vector 'nrMines' times.
		RandGen temp_random;
		int domain = cellNumbers.size();
		int temp_idx = temp_random.RandInt(domain);
		int temp_mineNo = cellNumbers[temp_idx];
		mineNumbers.push_back(temp_mineNo);
		cellNumbers[temp_idx] = cellNumbers[domain-1];
		cellNumbers.pop_back();    // choosen integers are removed for avoiding to choose them again
	} 

	vector<vector<MineCell>> board(board_row,vector<MineCell>(board_column)); // main board created

	int cellNumber = 0; // This integer will be assigned to a cell and increase by 1 as every coordinates iterated. This means cell number of (0,0) will be 0, (0,1) will be 1 and so it will go on.

	for (int i = 0; i < board.size(); i++) { // creating the board cell by cell
		for (int j = 0; j < board[i].size(); j++) {

			board[i][j].cellNo = cellNumber; // assigning the cellNumber
			board[i][j].isOpened = false; // initializing the 'isOpened' 

			if (searchinvector(mineNumbers,cellNumber) == 0) { board[i][j].isBomb = false; } // assigning the cell's bomb status
			else if (searchinvector(mineNumbers,cellNumber) == 1) { board[i][j].isBomb = true; }

			board[i][j].BombsAround = searchNearbyBombs(mineNumbers, board[i][j].cellNo, i, j, board_row, board_column); // assigning the cell's neighbor bomb number 

			cellNumber++;
		}
	}

	printStartingBoard(board_row,board_column); // function was described above

	int choice,score = 0; // choice determines the option number (1,2 or 3) - score will increase as non-bomb cell revealed
	bool gameover = false;

	while (true) {
		cout << "Press:" << endl << "1. If you want to find out the surrounding of a cell" << endl <<"2. If you want to open the cell" << endl << "3. If you want to exit." << endl;
		cin >> choice;
		while (choice != 1 && choice != 2 && choice != 3) { // option validity check
			cout << "Your input is wrong. Please select one of the options: 1, 2 or 3." << endl;
			cin >> choice;
		}

		int crow, ccol;

		if (choice == 1) { //option 1
			cout << "Give the coordinates: "; cin >> crow >> ccol;
			while (crow >= board_row || crow < 0 || ccol >= board_column || ccol < 0) { // coordinates range check
				cout << "It is out of range. Please give a valid coordinates: "; cin >> crow >> ccol;
			}

			cout << "Displaying the surrounding of " << "(" << crow << "," << ccol << "):" << endl;

			for (int i = 0; i < board.size(); i++){ // printing the board according to the coordinates given by user and considering previous turns
				for (int j = 0; j < board[i].size(); j++){
					if (i == crow && j == ccol) {
						cout << board[i][j].BombsAround << " ";
					}
					else {
						if (board[i][j].isOpened == true) {
							cout << board[i][j].BombsAround << " ";
						}
						else{
							cout << "X ";
						}
					}
				}
				cout << endl;
			}

			cout << "Around (" << crow << "," << ccol << ") you have " << board[crow][ccol].BombsAround << " bomb(s)" << endl << endl;
		}

		else if (choice == 2) { //option 2
			cout << "Give the coordinates: "; cin >> crow >> ccol;
			while (crow >= board_row || crow < 0 || ccol >= board_column || ccol < 0) { // coordinates range check
				cout << "It is out of range. Please give a valid coordinates: "; cin >> crow >> ccol;
			}
			cout << "Opening cell " << "(" << crow << "," << ccol << "):" << endl;

			for (int i = 0; i < board.size(); i++) { // printing the board according to the coordinates given by user and considering previous turns
				for (int j = 0; j < board[i].size(); j++){

					MineCell tempCell = board[i][j];
					if (tempCell.isOpened == false) {
						if (i == crow && j == ccol) {
							board[i][j].isOpened = true;
							if (tempCell.isBomb == false) {
								cout << tempCell.BombsAround << " ";
								score++;
							}
							else { // in case of opening a cell with a bomb
								gameover = true; 
								cout << "B ";
							}
						}
						else {
							cout << "X ";
						}
					}

					else {
						cout << tempCell.BombsAround << " ";
					}

				}
				cout << endl;
			}
			cout << endl;
		}
		
		else if (choice == 3) { // option 3
			cout << "Program exiting ..." << endl; break;
		}

		if (gameover == true) { // after opening a cell with a bomb
			cout << "Unfortunately, you stepped on a mine" << endl << "Arrangement of mines:" << endl;
			for (int i = 0; i < board.size(); i++){
				for (int j = 0; j < board[i].size(); j++){
					if (board[i][j].isBomb == true) { cout << "B "; }
					else { cout << board[i][j].BombsAround << " ";}
				}
				cout << endl;
			}
			cout << endl << ">>>>> Game Over! <<<<<" << endl;
			break;
		}
		if (score == (area-nrMines)) { // if score, in other words opened cell count reaches '(Rows x Columns) - Number of mines' (reopened cells are not counted)
			cout << "Congratulations! All the non-mined cells opened successfully" << endl << "You won!" << endl;
			break;
		}

	}
	
	return 0;
}

