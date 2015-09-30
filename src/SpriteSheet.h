#ifndef		SPRITESHEET_H
#define		SPRITESHEET_H

#include <SDL.h>
#include <vector>

struct SpriteSheet {
	SDL_Surface* sheet;
	std::vector<SDL_Rect> frames;

	SpriteSheet(Uint32 numFrames = 1);
	~SpriteSheet();
};

// a sprite sheet for animation purposes
struct VerticalSpriteSheet : public SpriteSheet {
	Uint32 frame_w;
	Uint32 frame_h;

	VerticalSpriteSheet(const char* filename, Uint32 numFrames, const SDL_PixelFormat* fmt = NULL, bool hasTransparency = true);
};

// a sprite sheet for tilesets & general purposes
// - no transparency for now, but you can set a colorkey manually on the surface
struct TiledSpriteSheet : public SpriteSheet {
	Uint32 tile_w;
	Uint32 tile_h;
	Uint32 rows;
	Uint32 cols;

	TiledSpriteSheet(const char* filename, Uint32 numCols, Uint32 numRows, const SDL_PixelFormat* fmt = NULL);
};

#endif	//	SPRITESHEET_H
