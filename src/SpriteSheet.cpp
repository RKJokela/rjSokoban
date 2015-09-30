#include "SpriteSheet.h"
#include "CSurface.h"

SpriteSheet::SpriteSheet(Uint32 numFrames) : sheet(NULL), frames(numFrames) { }

SpriteSheet::~SpriteSheet() {
	if (sheet)
		SDL_FreeSurface(sheet);
}

VerticalSpriteSheet::VerticalSpriteSheet(const char* filename, Uint32 numFrames, const SDL_PixelFormat* fmt, bool hasTransparency) :
	SpriteSheet(numFrames),
	frame_w(0),
	frame_h(0)
{
	sheet = CSurface::OnLoad(filename, fmt);
	if (!sheet) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "CSurface::OnLoad: %s", SDL_GetError());
		return;
	}
	// Assuming top left pixel is to be transparent!
	if (hasTransparency)
		SDL_SetColorKey(sheet, SDL_TRUE, *(Uint32*)sheet->pixels);
	frame_w = sheet->w;
	frame_h = sheet->h/numFrames;
	SDL_Rect rect;
	rect.x = 0;
	rect.w = frame_w;
	rect.h = frame_h;
	frames.clear();
	for (Uint32 i = 0; i < numFrames; i++) {
		rect.y = i*frame_h;
		frames.push_back(rect);
	}
}

TiledSpriteSheet::TiledSpriteSheet(const char* filename, Uint32 numCols, Uint32 numRows, const SDL_PixelFormat* fmt) :
	SpriteSheet(numCols*numRows),
	tile_w(0),
	tile_h(0),
	cols(numCols),
	rows(numRows)
{
	sheet = CSurface::OnLoad(filename, fmt);
	if (!sheet) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "CSurface::OnLoad: %s", SDL_GetError());
		return;
	}
	tile_w = sheet->w/cols;
	tile_h = sheet->h/rows;
	SDL_Rect rect;
	rect.w = tile_w;
	rect.h = tile_h;
	frames.clear();
	for (Uint32 i = 0; i < rows; i++) {
		for (Uint32 j = 0; j < cols; j++) {
			rect.x = j*tile_w;
			rect.y = i*tile_h;
			frames.push_back(rect);
		}
	}
}
