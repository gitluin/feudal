/* What operations are most common in this game?
 * 	1. Moving units
 * 	2. Removing units from the field
 *
 * How to easily access units:
 *	game_board->uboard[x][y] selects the unit* at the x and y coord
 *	game_board->board[x][y] contains the landscape information
 *
 * How to move units:
 * 	1. Check that u->owner == current_player
 * 	2. Check that target move is valid
 * 	3. game_board->uboard[x][y] = NULL;
 * 	4. if ((other = game_board->uboard[new_x][new_y]))
 * 		free that unit
 * 		if it's royalty, other->owner->royalty_count--;
 * 	5. game_board->uboard[new_x][new_y] = u;
 *
 * How to represent movement patterns:
 * 	K	King 			1 space cardinal, 1 space diagonal
 * 	N/D/R	Knight/Duke/Prince	Any number of spaces cardinal or diagonal
 * 	S	Sargeant		12 diagonal, 1 cardinal
 * 	P	Pikeman			12 cardinal, 1 diagonal
 * 	Q	Squire			1 cardinal and 1 diagonal 45 degrees "out" from
 * 						cardinal direction
 * 	A	Archer			3 cardinal or diagonal, can also attack without
 * 						moving
 *
 * 	mov_x - how many spaces can be moved horizontally
 * 	mov_y - how many spaces can be moved vertically
 * 	mov_diag - how many spaces can be moved diagonally
 *
 * 	if 256 for any, then only limited by LoS and terrain
 * 	if u->symbol == 'A' || u->symbol == 'Q'
 * 		[[ special code here ]]
 *
 */

#include <math.h>

#define BWIDTH		24
#define BHEIGHT		24

#define MAXLEN		256

#define BETWEEN(A,B,C)	((A <= (C) && (C) <= (B)))

typedef struct board board;
typedef struct player player;
typedef struct unit unit;

struct player {
	int royalty_count;
	const char name[MAXLEN];
};

struct unit {
	const char symbol;
	player* owner;
	int mov_x, mov_y, mov_diag;
};

/* uboard is a 2d array of pointers to units
 * 	these units should "live" in player->army
 */
struct board {
	unit* uboard[BWIDTH][BHEIGHT];
	unsigned int board[BWIDTH][BHEIGHT];
};

// TODO: can i fix the width of these given the board dimensions?
struct move {
	int x, int y;
	move* next;
};

int
is_mounted(unit* u){
	switch(u->symbol){
	case 'N':
	case 'D':
	case 'R':
		return 1;
	default:
		return 0;
	};
}

/* on cardinal means if new_x == x || new_y == y
 * 	and the other is within limits
 * on diagonal means delta x == delta y
 * 	delta_x = new_x - x
 * 	delta_y = new_y - y
 *
 * 	and delta_x (or delta_y, since same) within limits
 */

// TODO:
void
new_move(x, y, move* head){
	move* m = ecalloc(1, sizeof(move));
	m->x = x;
	m->y = y;

}

/* if space is occupied, or a mountain, or a hill and u is mounted, or
 * 	a wall and u is not on the green, not pathable
 * else, pathable!
 */
int
is_pathable(board g, unit* u, int x, int y, int new_x, int new_y){
	if (g->uboard[new_x][new_y] ||
	(g->board[new_x][new_y] & IsMountain) ||
	((g->board[new_x][new_y] & IsHill) && is_mounted(u)) ||
	((g->board[new_x][new_y] & IsWall) && !(g->board[x][y] & IsGreen)))
		return 0;

	return 1;
}

move*
generate_moves(board g, int x, int y){
	int i;
	unit* u;
	move* moves;

	if (!(u = g->uboard[x][y]))
		return NULL;

	/* horizontal */
	for (i=x - u->mov_x;i < x + u->mov_x;i++){
		if (i < 0 || i > BWIDTH - 1)
			continue;

		if (is_pathable(g, u, x, y, i, y))
			new_move(i, y, moves);
	}

	/* vertical */
	for (j=y - u->mov_y;j < y + u->mov_y;j++){
		if (j < 0 || j > BWIDTH - 1)
			continue;

		if (is_pathable(g, u, x, y, x, j))
			new_move(x, j, moves);
	}

	/* diagonal from bottom left to top right */
	for (i=x - u->mov_diag, j=y - u->mov_diag;i < x + u->mov_diag, j < y + u->mov_diag;i++,j++){
		if (i < 0 || j < 0 || i > BWIDTH - 1 || j > BHEIGHT - 1)
			continue;

		if (is_pathable(g, u, x, y, i, j))
			new_move(i, j, moves);
	}

	/* diagonal from top left to bottom right */
	for (i=x - u->mov_diag, j=y + u->mov_diag;i < x + u->mov_diag, j < y - u->mov_diag;i++,j--){
		if (i < 0 || j < 0 || i > BWIDTH - 1 || j > BHEIGHT - 1)
			continue;

		if (is_pathable(g, u, x, y, i, j))
			new_move(i, j, moves);
	}

	return moves;
}

/* 1. Check unobstructed line of sight
 * 	Check terrain and other units
 * 2. Check destination is not mountain, not castle wall, or not hill if unit
 * 	is mounted
 * 3. Check destination is within movement pattern of unit
 * 	either along a diagonal or along a horizontal or vertical
 */
int
is_move_valid(board g, unit* u, int x, int y, int new_x, int new_y){
	int i;

	for (i=0;i < 
}
