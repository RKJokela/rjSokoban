#include <SDL_image.h>
#include "CSurface.h"

CSurface::CSurface() {}

SDL_Surface* CSurface::OnLoad(const char* File, const SDL_PixelFormat* fmt) {
	SDL_Surface* tmpSurf = IMG_Load(File);
	SDL_Surface* retSurf = tmpSurf;
	if (tmpSurf && fmt) {
		retSurf = SDL_ConvertSurface(tmpSurf, fmt, 0);
		SDL_FreeSurface(tmpSurf);
	}

	return retSurf;
}

bool CSurface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, const SDL_Rect* frame) {
	if (!Surf_Dest) {
		SDL_SetError("Destination surface must not be NULL.");
		return false;
	}
	if (!Surf_Src) {
		SDL_SetError("Source surface must not be NULL.");
		return false;
	}

	SDL_Rect DestR;
	DestR.x = X;
	DestR.y = Y;

	SDL_BlitSurface(Surf_Src, frame, Surf_Dest, &DestR);

	return true;
}

bool CSurface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H) {
	SDL_Rect SrcR;
	SrcR.x = X2;
	SrcR.y = Y2;
	SrcR.w = W;
	SrcR.h = H;

	return OnDraw(Surf_Dest, Surf_Src, X, Y, &SrcR);

	return true;
}

bool CSurface::Transparent(SDL_Surface* s, Uint8 r, Uint8 g, Uint8 b) {
	if (!s) {
		SDL_SetError("Source surface must not be NULL.");
		return false;
	}
	SDL_SetColorKey(s, 1, SDL_MapRGB(s->format, r, g, b));
	return true;
}
