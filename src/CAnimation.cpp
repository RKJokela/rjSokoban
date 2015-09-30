#include "CAnimation.h"
#include "CSurface.h"

CAnimation::CAnimation() : 
	CurrentFrame(0),
	MaxFrames(0),
	FrameInc(1),
	FrameRate(100),
	OldTime(0),
	Oscillate(false),
	SpriteSheet(NULL) { }

bool CAnimation::OnAnimate() {
	if (OldTime + FrameRate > SDL_GetTicks())
		return false;

	OldTime = SDL_GetTicks();
	CurrentFrame += FrameInc;

	if (Oscillate) {
		if (FrameInc > 0) {
			if (CurrentFrame >= MaxFrames - 1)
				FrameInc = -FrameInc;
		}
		else { // FrameInc < 0
			if (CurrentFrame <= 0)
				FrameInc = -FrameInc;
		}
	}
	else {
		if (CurrentFrame >= MaxFrames)
			CurrentFrame = 0;
	}

	return true;
}

bool CAnimation::OnDraw(SDL_Surface* dest, int X, int Y) {
	if (!SpriteSheet) {
		SDL_SetError("SpriteSheet must not be NULL.");
		return false;
	}
	const SDL_Rect* frame = &SpriteSheet->frames[CurrentFrame];
	return CSurface::OnDraw(dest, SpriteSheet->sheet, X, Y, frame);
}

void CAnimation::SetFrameRate(int Rate) { FrameRate = Rate; }
int  CAnimation::GetFrameRate() { return FrameRate; }
void CAnimation::SetCurrentFrame(int Frame) { 
	CurrentFrame = Frame%MaxFrames; 
	if (CurrentFrame == 0 && FrameInc < 0)
		FrameInc = -FrameInc;
	if (CurrentFrame == MaxFrames - 1 && FrameInc > 0)
		CurrentFrame--; // hack
}
int  CAnimation::GetCurrentFrame() { return CurrentFrame; }
