#ifndef		CENTITY_H
#define		CENTITY_H

#include <vector>

#include "CAnimation.h"
#include "CSurface.h"

class CEntity {
public:
	static std::vector<CEntity*> EntityList;

protected:
	CAnimation Anim_Control;
	VerticalSpriteSheet* SpriteSheet;

	int	X;
	int	Y;

public:
	int		AnimState;

	CEntity();
	virtual ~CEntity();

	virtual void SetAnimationParams(int FrameRate, bool Oscillate);
	virtual bool SetSpriteSheet(VerticalSpriteSheet* SS);
	virtual bool OnLoad(const char* File, Uint32 MaxFrames, const SDL_PixelFormat* fmt = NULL);
	// Returns true when screen should be redrawn
	virtual bool OnLoop();
	virtual void OnRender(SDL_Surface* Surf_Display, int offX = 0, int offY = 0);
	virtual void OnCleanup() = 0;
	virtual void GetLoc(int* x, int* y);
};

#endif	//	CENTITY_H
