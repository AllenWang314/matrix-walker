#include "matrix_walker.h"
#include <iostream>
#include <cstring>

void matrix_rand_move(matrix_walker *matrix) {
	// Perform 10,000,000 random moves in matrix, starting from the middle.
    matrix->teleport((matrix->size() - 1) / 2, (matrix->size() - 1) / 2);
	for (int i; i < 1e7; i++) {
		int r = rand() % 4;
		switch(r){
			case 0:
				matrix->move_up();
				break;
			case 1:
				matrix->move_down();
				break;
			case 2:
				matrix->move_left();
				break;
			case 3:
				matrix->move_right();
				break;
		}
	}
}

void matrix_row_major(matrix_walker *matrix) {
	// Traverse the entire matrix, through each entry exactly once,
	// in row-major order. Every other row goes backwards
	// to avoid teleports (snake game style).
	matrix->teleport(0, 0);
	for (int i = 0; i < matrix->size() / 2; i++) {
		for (int j = 0; j < matrix->size() - 1; j++) {
			matrix->move_right();
		}
		matrix->move_down();
		for (int j = matrix->size() - 1; j > 0; j--) {
			matrix->move_left();
		}
		matrix->move_down();
	}
}

void matrix_col_major(matrix_walker *matrix) {
	// Traverse the entire matrix in column-major order.
	matrix->teleport(0, 0);
	for (int i = 0; i < matrix->size() / 2; i++) {
		for (int j = 0; j < matrix->size() - 1; j++) {
			matrix->move_down();
		}
		matrix->move_right();
		for (int j = matrix->size() - 1; j > 0; j--) {
			matrix->move_up();
		}
		matrix->move_right();
	}
}

// DIRECTIONAL TESTS
// For many random rows/columns, move up/down/left/right
// through the whole row/column. One direction per test.
void matrix_move_up(matrix_walker *matrix) {
	for (int x = 0; x < 1e4; x++) {
		int j = rand() % matrix->size();
		matrix->teleport(matrix->size()-1, j);
		for (int i = matrix->size()-1; i > 0; i--)
			matrix->move_up();
	}
}

void matrix_move_down(matrix_walker *matrix) {
	for (int x = 0; x < 1e4; x++) {
		int j = rand() % matrix->size();
		matrix->teleport(0, j);
		for (int i = 0; i < matrix->size()-1; i++)
			matrix->move_down();
	}
}

void matrix_move_left(matrix_walker *matrix) {
	for (int x = 0; x < 1e4; x++) {
		int i = rand() % matrix->size();
		matrix->teleport(i, matrix->size()-1);
		for (int j = matrix->size()-1; j > 0; j--)
			matrix->move_left();
	}
}

void matrix_move_right(matrix_walker *matrix) {
	for (int x = 0; x < 1e4; x++) {
		int i = rand() % matrix->size();
		matrix->teleport(i, 0);
		for (int j = 0; j < matrix->size()-1; j++)
			matrix->move_right();
	}
}

int main(int argc, char* argv[]) {
	#ifdef DEBUG
		// run with ```make benchmark DEBUG=1```
		std::cout << "In debug mode!" << std::endl;
	#else
		// run with ```make benchmark``` or ```make benchmark DEBUG = 0```
		std::cout << "In production mode!" << std::endl;
	#endif

	if (argc != 3) {
		fprintf(stderr, "please provide matrix_walker type and test");
		return -1;
	}
	char* mw_type = argv[1];
	char* test = argv[2];

	int MATRIX_SIZE = 1000; // must be even
	
	matrix_walker *mw;

	if (strcmp(mw_type, "naive") == 0){
		mw = new naive_matrix_walker(MATRIX_SIZE);
	}
	else if (strcmp(mw_type, "z") == 0){
		mw = new z_matrix_walker(MATRIX_SIZE);
	}
	else if (strcmp(mw_type, "hilbert") == 0){
		mw = new hilbert_matrix_walker(MATRIX_SIZE);
	}
	else {
		fprintf(stderr, "invalid matrix_walker type specified: %s. must be one of 'naive', 'z', 'hilbert'", mw_type);
		return -1;
	}

	fprintf(stderr, "Initializing...\n");
	mw->set_default();

	fprintf(stderr, "Benchmarking...\n");
	if (strcmp(test, "rand_move") == 0){
		matrix_rand_move(mw);
	}
	else if (strcmp(test, "row_major") == 0){
		matrix_row_major(mw);
	}
	else if (strcmp(test, "col_major") == 0){
		matrix_col_major(mw);
	}
	else if (strcmp(test, "move_up") == 0){
		matrix_move_up(mw);
	}
	else if (strcmp(test, "move_down") == 0){
		matrix_move_down(mw);
	}
	else if (strcmp(test, "move_left") == 0){
		matrix_move_left(mw);
	}
	else if (strcmp(test, "move_right") == 0){
		matrix_move_right(mw);
	}
	else {
		fprintf(stderr, "invalid test specified. must be one of 'rand_move', 'row_major', 'col_major'.");
		return -1;
	}
	return 0;
}

// ssh -i "aws_key.pem" ec2-user@ec2-18-219-65-100.us-east-2.compute.amazonaws.com