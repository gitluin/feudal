enum { IsOccupied, IsGrass, IsHill, IsMountain, IsCastle };

typedef struct player player;
typedef struct unit unit;

struct player {
	const char color;
	const char* name;
	unit army[MAXUNITS];
};

/* unit coords are relative to the game board,
 * whereas tiles have their own origin
 *
 * move_x, move_y, and move_diag max spaces moveable in that direction
 */
struct unit {
	const char symbol;
	const char color;
	int x, y;
	int move_x, move_y, move_diag;
};

/* 0 0 0 0 0
 * ^ ^ ^ ^ ^
 * | | | | isoccupied
 * | | | isgrass
 * | | ishill
 * | ismountain
 * iscastle
 */
struct tile {
	unsigned int board[TWIDTH][THEIGHT];
};

/*
 * quadrant 0
 * quadrant 1
 * quadrant 2
 * quadrant 3
 *
 * 	|
 *   0  |  1
 * -----|-----
 *   2	|  3
 *  	|
 *
 * quadrant_1->x = quadrant_0->x + TWIDTH
 * quadrant_1->y = quadrant_0->y
 *
 * quadrant_2->x = quadrant_0->x
 * quadrant_2->y = quadrant_0->y + THEIGHT
 *
 * quadrant_3->x = quadrant_0->x + TWIDTH
 * quadrant_3->y = quadrant_0->y + THEIGHT
 *
 */
struct game_board {
	tile quadrants[4];
};

unit dud_unit = {
	.symbol = 'U',
	.color = 'N',
	.x = -69,
	.y = -69,
	.move_x = -69,
	.move_y = -69,
	.move_pat = 'U'
};

/* rotate the tile 90 degrees clockwise
 * 1 2 3
 * 4 5 6
 * 7 8 9
 *
 * 7 4 1
 * 8 5 2
 * 9 6 3
 *
 * Row 0 becomes Col 2
 * Row 1 becomes Col 1
 * Row 2 becomes Col 0
 *
 */
void
rotate_tile(int dir, int mag, tile* t){
	int i, j, tmp;

	unsigned int new_board[TWIDTH][THEIGHT];

//	new_board[0][0] = tile->board[2][0]
//	new_board[0][1] = tile->board[1][0]
//	new_board[0][2] = tile->board[0][0]

	for (i=0;i < TWIDTH;i++)
		for (j=THEIGHT,k=0;j > -1 && k < THEIGHT;k++,j--)
			new_board[i][k] = tile->board[j][i];

	tile->board = new_board;
}

unit*
find_unit(player* p, int x, int y){
	int i;

	// TODO: MAXUNITS probably not
	for (i=0;i < MAXUNITS;i++)
		if (p->army[i].x == x && p->army[i].y == y)
			return &p->army[i];

	return NULL;
}

/* Unit movement
 * K	King		1 all directions
 * N	Knight		Any all directions
 * D	Duke		Any all directions
 * R	Prince		Any all directions
 * S	Squire		2 cardinal with 1 diagonal in same cardinal direction
 * G	Sargeant	12 diagonal, 1 cardinal
 * P	Pikeman		12 cardinal, 1 diagonal
 * A	Archer		3 all directions
 */
// TODO:
// 	1. check that new_x and new_y are in the movement pattern for u
// 		TODO: pre-store movement shifts as unit struct variables
// 	2. check that new_x and new_y are in bounds/not mountainous
// 	3. check that the path to new_x, new_y is unobstructed
int
is_valid_move(game_board* g, unit* u, int new_x, int new_y){
	// Determine if move is cardinal, diagonal, or special
	// 	Then 1, then 2, then 3


	/* footmen */
	if (u->move_pat == 'C'){

	/* mounted units */
	} else if (u->move_pat == 'D'){

	/* squire or archer */
	} else {

	}
}

void
attack_unit(game_board* g, player* p, int x, int y, int new_x, int new_y, int move){
	unit* u;

	if ( !(u = find_unit(p, x, y)) )
		return;

	if (!is_valid_move(g, u, new_x, new_y))
		return;

	// TODO: attacking from distance
	if (u->symbol == 'A' && !move){

	/* for all other units, attack decays to move */
	} else {
		move_unit(g, u, new_x, new_y);
	}
}

void
move_unit(game_board* g, unit* u, int new_x, int new_y){
	int i;
	unit* other;

	for (i=0;i < num_players;i++){
		if ( (other = find_unit(players[i], new_x, new_y)) ){
			remove_unit(players[i], other);
			break;
		}
	}

	u->x = new_x;
	u->y = new_y;
}

// TODO:
void
remove_unit(player* p, unit* u){
	int i;

	// TODO: MAXUNITS probably not
	for (i=0;i < MAXUNITS;i++){
		if (p->army[i].x == x && p->army[i].y == y){
			p->army[i] = &dud_unit;
			break;
		}
	}
}
