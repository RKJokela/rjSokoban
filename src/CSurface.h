#ifndef		CSURFACE_H
#define		CSURFACE_H

#include <SDL.h>

class CSurface {
public:
	CSurface();

	static SDL_Surface* OnLoad(const char* File, const SDL_PixelFormat* fmt = NULL);

	static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, const SDL_Rect* frame = NULL);
	static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);

	static bool Transparent(SDL_Surface* s, Uint8 r, Uint8 g, Uint8 b);
};

#endif	//	CSURFACE_H
