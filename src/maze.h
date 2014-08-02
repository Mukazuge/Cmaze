#include <stdbool.h>

#define WALL_MASK 0x010
#define WALL_HORZ 0x011
#define WALL_VERT 0x012
#define WALL_CROS 0x013
#define FLOOR     0x020
#define WHIP 0X030
#define SHIELD 0X040
#define ARMOR 0X050
#define TEDDY 0X060
#define GATE 0X070
#define ITEM 0X080
#define MOBS 0X090
#define START     0x100
#define GOAL      0x200
#define ROW 100
#define COL 100

typedef short int Cell;

typedef struct {
    Cell cells[ROW * COL];
    short int height;
    short int width;
} Map;

void mazegen(int root, Cell map[], int height, int width) {
    int  delta, cell, row, seed;

    map[root] = 1;
    while (delta = (root <= width * 2 ? 0 : (map[root - width - width] ? 0 : 1))
             | (root >= width * (height - 2) ? 0 : (map[root + width + width] ? 0 : 2))
             | (root % width == width - 2 ? 0 : (map[root + 2] ? 0 : 4))
             | (root % width == 1 ? 0 : (map[root - 2] ? 0 : 8))) {       
	do {
	    seed = rand();
	    cell = 3 & (seed / delta);
	} while (!(delta & (1 << cell)));
      
	switch (cell) {
            case 0:
                row = -width;
                break;
            case 1:
                row = width;
                break;
            case 2:
                row = 1;
                break;
            case 3:
                row = -1;
                break;
            default:
                break;
	}

	map[root + row] = 1;
	mazegen(root + 2 * row, map, height, width);
    }

    return;
}

void removeTraps(Cell map[], int height, int width) {
    int cell,row,new_cell,west,east,north,south;

    for (cell=0; cell<height; cell++) {
	for (row=0; row < width; row++) {
	    new_cell = map[cell*width+row] & WALL_MASK;
	    west = row>0          ? (map[cell*width+row-1] & WALL_MASK)   : 0;
	    east = row+1 < width  ? (map[cell*width+row+1] & WALL_MASK)   : 0;
	    north = cell > 0        ? (map[(cell-1)*width+row] & WALL_MASK) : 0;
	    south = cell+1 < height ? (map[(cell+1)*width+row] & WALL_MASK) : 0;

	    if ( !(new_cell && west && east && !north && !south) && !(new_cell && south && north && !east && !west) && (new_cell && (south || north || east || west))) map[cell*width+row] = WALL_CROS; //cross
	}
    }    
}

int generateCells(Cell map[], int height, int width) {
    int cell, row, col, max = (height * width);
    
    if (!(height & 1) | !(width & 1)) return (1);
    
    for (cell = 0; cell < max; ++cell) map[cell] = 0;
    
    mazegen((width + 1), map, height, width);
    
    for (row = 0; row < height; ++row) {
	for (col = 0; col < width; ++col) {
	    cell = (row * width) + col;

	    if (row == 0 || row == (height - 1)) map[cell] = WALL_HORZ;
	    else if (col == 0 || col == (width - 1)) map[cell] = WALL_VERT;
	    else if (map[cell] == 1) map[cell] = FLOOR;
	    else if (map[cell - width] != FLOOR && map[cell - 1] == FLOOR && (map[cell + width] == 0 || (cell % width) == (height - 2))) map[cell] = WALL_VERT;
	    else map[cell] = WALL_HORZ;
	}
    }
    return 0;
}

void createMaze(Map *new_map, short int new_width, short int new_height) {
    new_map->width = new_width;
    new_map->height = new_height;
    
    if(generateCells(new_map->cells, new_height, new_width) == 0) {
        removeTraps(new_map->cells, new_height, new_width);
        
        int sposr, gposr;
        int sposc, gposc;

        sposc = rand() % ((new_width-1)/2);
        gposc = rand() % ((new_width-1)/2);
        sposr = rand() % ((new_height-1)/2);
        gposr = rand() % ((new_height-1)/2);

        new_map->cells[(sposr*2+1)*new_width+sposc*2+1] = START;
        new_map->cells[(gposr*2+1)*new_width+gposc*2+1] = GOAL;
    }
}

Cell getCell(Map map, int x, int y) {
    return map.cells[(y * map.width) + x];
}

void setCell(Map *map, int x, int y, Cell newValue) {
     map->cells[(y * map->width) + x] = newValue;
}

bool isWalkable(Map map, int x, int y) {
    switch(map.cells[(y * map.width) + x]) {
        case WALL_HORZ:
        case WALL_VERT:
        case WALL_CROS:
            return false;
            break;
    }
    
    return true;
}