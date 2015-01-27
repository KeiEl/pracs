#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 3

struct Position {
	int x;
	int y;
};

typedef enum { false, true } bool;
typedef enum { INVALID = -2, O_WON, DRAW, X_WON, X_TURN, O_TURN } state_t;

// Utility functions

int min( int a, int b ) {
	return a < b ? a : b;
}

int max( int a, int b ) {
	return a > b ? a : b;
}

void print_board( const char b[SIZE][SIZE] ) {
	for ( int i = 0; i < SIZE; ++i ) {
		for ( int j = 0; j < SIZE; ++j ) {
			printf( "%c ", b[i][j] );
		}
		printf( "\n" );
	}
}

bool is_valid( const char b[SIZE][SIZE], const struct Position *pos ) {
	return pos->x >= 0 && pos->x < SIZE && pos->y >= 0 && pos->y < SIZE 
		   && b[pos->x][pos->y] == '-';
}

state_t get_state( const char b[SIZE][SIZE] ) {
	state_t state = X_TURN;

	int x_num = 0, o_num = 0;
	// Checking vertical and horizontal
	for ( int i = 0; i < SIZE; ++i ) {
		int h_counter = 0, v_counter = 0;
		for ( int j = 0; j < SIZE; ++j ) {
			if ( b[i][j] == 'x' ) {
				++h_counter;
				++x_num;
			} else if ( b[i][j] == 'o' ) {
				--h_counter;
				++o_num;
			}

			if ( b[j][i] == 'x' ) ++v_counter;
			else if ( b[j][i] == 'o' ) --v_counter;
		}
		if ( abs(h_counter) == 3 || abs(v_counter) == 3 ) {
			state_t tmp = h_counter == 3 || v_counter == 3 ? X_WON : O_WON;

			if ( state == X_TURN ) state = tmp;
			else if ( state != tmp ) state = INVALID;
		}
	}

	// Checking diagonal
	int r = 0, l = 0;
	for ( int i = 0; i < SIZE; ++i ) {
		if ( b[i][i] == 'x' ) ++r;
		else if ( b[i][i] == 'o' ) --r;

		if ( b[SIZE-i-1][i] == 'x' ) ++l;
		else if ( b[SIZE-i-1][i] == 'o' ) --l;
	}
	if ( abs(r) == 3 || abs(l) == 3 ) {
		state_t tmp = r == 3 || l == 3 ? X_WON : O_WON;
		
		if ( state == X_TURN ) state = tmp;
		else if ( state != tmp ) state = INVALID;
	}

	// No change made
	if ( state != X_TURN ) return state;

	if ( abs(x_num - o_num) > 1 ) return INVALID;
	if ( x_num + o_num == SIZE*SIZE ) return DRAW;

	return x_num <= o_num ? X_TURN : O_TURN;
}

// AI

int min_max( char b[SIZE][SIZE], struct Position *pos ) {
	// All filled
	state_t state = get_state(b);
	if ( state != X_TURN && state != O_TURN ) {
		return (int)state;
	}

	int res = state == X_TURN ? -2 : 2;
	for ( int i = 0; i < SIZE; ++i ) {
		for ( int j = 0; j < SIZE; ++j ) {
			if ( b[i][j] == '-' ) {
				b[i][j] = state == X_TURN ? 'x' : 'o'; 
				int tmp = state == X_TURN ? max( res, min_max(b, NULL) )
									  	  : min( res, min_max(b, NULL) );
				if ( res != tmp ) {
					res = tmp;
					if ( pos != NULL ) {
						pos->x = i;
						pos->y = j;
					}
				}
				b[i][j] = '-';
			}

		}
	}

	return res;
}

struct Position get_move( char b[SIZE][SIZE] ) {
	struct Position pos = { -1, -1 };
	min_max(b, &pos);

	return pos;
}

// Testing

#define CHECKRESULT(r,e) do { \
								if (e == r) \
									printf( "PASSED.\n" ); \
								else \
									printf( "FAILED - Expected %d Received %d\n", e, r ); \
							} while (false)

void run_test_set( const char *filename ) {
	char b[SIZE][SIZE];
	int expected;

	FILE *f = fopen( filename, "r" );
	if ( f == NULL ) {
		printf( "Cannot open file\n" );
		return;
	}

	while (true) {
		int num_read = fscanf( f, "%c%c%c%c%c%c%c%c%c %d\n", 
						b[0], b[0]+1, b[0]+2, b[1], b[1]+1, b[1]+2, b[2], b[2]+1, b[2]+2, &expected );
		if ( num_read != 10 ) break;

		int res = min_max(b, NULL);
		CHECKRESULT(res,expected);
	}
	fclose(f);
}

// Game

void run_game() {
	char board[SIZE][SIZE] = { { '-', '-', '-' },
							   { '-', '-', '-' },
							   { '-', '-', '-' } };
	int remaining = SIZE*SIZE;
	bool player = true;
	state_t state = X_TURN;

	// TODO: Error handling
	while ( remaining-- && (state == X_TURN || state == O_TURN) ) {
		if ( player ) {
			struct Position pos;
			int num_read;
			do {
				printf( "x,y = " );
				num_read = scanf( "%d,%d", &pos.x, &pos.y );
				if ( num_read != 2 || !is_valid(board, &pos) )
					printf( "Invalid input\n" );
				else
					break;
			} while (true);

			board[pos.x][pos.y] = 'x';
		} else {
			struct Position pos = get_move( board );
			printf( "x,y = %d,%d\n", pos.x, pos.y );
			board[pos.x][pos.y] = 'o';
		}

		print_board(board);
		player = !player;
		state = get_state(board);
	}

	switch(state) {
		case X_WON:
			printf( "X won\n" );
			break;
		case O_WON:
			printf( "O won\n" );
			break;
		case DRAW:
			printf( "Draw!\n" );
			break;
		default:
			printf( "Invalid state %d", state );
	}
}

int main( int argc, char **argv ) {
	if ( argc > 1 && strcmp(argv[1], "-t") == 0 ) {
		// Test min max
		run_test_set( "minmax_test" );
	} else if ( argc > 1 && strcmp(argv[1], "-p") == 0 ) {
		// Run game
		run_game();
	} else {
		printf( "Usage: %s [-t|-p]", argv[0] );
	}

	return 0;
}