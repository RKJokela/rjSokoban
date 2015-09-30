#ifndef		DEFINES_H
#define		DEFINES_H

//--GRAPHICS--
// Pixel format used for everything
#define PIXELFORMAT	SDL_PIXELFORMAT_ARGB8888
#define	BPP			32
#define RMASK		0x00ff0000
#define GMASK		0x0000ff00
#define BMASK		0x000000ff
#define AMASK		0xff000000
#define COLORKEY	0x0000ffff	// Cyan

//--TILESET--
// Tile dimensions in pixels
// - square tiles (but leave flexibility)
#define TILE_W		16
#define TILE_H		TILE_W

// Map dimensions in tiles
#define	MAP_W		22
#define MAP_H		20
// Map dimensions in pixels
#define	MAP_W_PX	(MAP_W*TILE_W)
#define MAP_H_PX	(MAP_H*TILE_H)

// Maximum number of maps to load
#define MAX_MAPS	128

//--WINDOW--
// Window dimensions in pixels
#define SCREEN_W	MAP_W_PX
#define SCREEN_H	MAP_H_PX

// Goes in window title bar
#define TITLE		"Sokoban v1.0 by RKJ"

//--GAME--
// Game Framerate cap in frames/sec
#define FPS			60
// Time per frame in ms
#define MS_PER_FRAME	(1000.0/FPS)
// Camera scroll rate in pixels/sec
#define SCROLLRATE	240
// Maximum amount of turns you can undo
#define MAX_UNDO    15
// Uncomment if you don't want keys to repeat when held
//#define WALK_ONLY

#endif	//	DEFINES_H
