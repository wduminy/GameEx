#pragma once
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define MAP_WIDTH 15 // number of tiles in a row (based on xevious estimation)
#define LEVEL_HEIGHT 137 // number of rows in a level  
#define TILE_SIZE_PX 58 // number of pixels in a tile  (it is a square)
#define LEVEL_WIDTH_PX MAP_WIDTH*TILE_SIZE_PX
#define LEVEL_HEIGHT_PX LEVEL_HEIGHT*TILE_SIZE_PX
#define SHOOTER_WIDTH_PX 30*2
#define SHOOTER_HEIGHT_PX 18*2
#define STATIC_DOME_PX 58 // width and height of static dome
#define SHOOTER_MAX_RIGHT_PX LEVEL_WIDTH_PX - SHOOTER_WIDTH_PX
#define SHOOTER_MAX_OFFSET_PX WINDOW_HEIGHT * 0.9f
#define SHOOTER_GUTTER TILE_SIZE_PX // the shooter is never lower than this point
#define SECONDS_PER_LEVEL 120 // the number of seconds to scroll through a level
#define SPEED_FACTOR 5.0f
#define MIN_SPEED LEVEL_HEIGHT_PX/(SECONDS_PER_LEVEL*SPEED_FACTOR)
#define MAX_SPEED LEVEL_HEIGHT_PX/(SECONDS_PER_LEVEL/SPEED_FACTOR) 
#define HOR_PIXELS_PER_SECOND MAX_SPEED * 0.8  
#define OFFSET_PIXELS_PER_SECOND MAX_SPEED
#define LEVEL_SIZE MAP_WIDTH*LEVEL_HEIGHT




