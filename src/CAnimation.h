#ifndef		CANIMATION_H
#define		CANIMATION_H

#include <SDL.h>
#include "SpriteSheet.h"

class CAnimation {
public:
	int MaxFrames;
	bool Oscillate;
	VerticalSpriteSheet* SpriteSheet;

	CAnimation();
	// returns true when frame changed
	bool OnAnimate();

	// draw the current frame on a surface
	// returns true on success
	bool OnDraw(SDL_Surface* dest, int X, int Y);

	void SetFrameRate(int Rate);
	int  GetFrameRate();
	void SetCurrentFrame(int Frame);
	int  GetCurrentFrame();

private:
	int CurrentFrame;
	int FrameInc;
	int FrameRate; // milliseconds
	Uint32 OldTime;
};

#endif	//	CANIMATION_H
