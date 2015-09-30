#include "CEntity.h"
#include "Defines.h"

std::vector<CEntity*> CEntity::EntityList;

CEntity::CEntity() : 
	Anim_Control(),
	SpriteSheet(NULL),
	X(0), Y(0),
	AnimState(0) { }

CEntity::~CEntity() {
	//this->OnCleanup();
}

void CEntity::SetAnimationParams(int FrameRate, bool Oscillate) {
	Anim_Control.SetFrameRate(FrameRate);
	Anim_Control.Oscillate = Oscillate;
}

bool CEntity::SetSpriteSheet(VerticalSpriteSheet* SS) {
	if (!SS) return false;
	SpriteSheet = SS;
	Anim_Control.MaxFrames = SS->frames.size();
	Anim_Control.SpriteSheet = SS;
	return true;
}

bool CEntity::OnLoad(const char* File, Uint32 MaxFrames, const SDL_PixelFormat* fmt) {
	OnCleanup();
	VerticalSpriteSheet* SS = new VerticalSpriteSheet(File, MaxFrames, fmt);
	if (!SS->sheet) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "CEntity::OnLoad: failed to load spritesheet %s: ", File);
		return false;
	}
	
	return SetSpriteSheet(SS);
}

bool CEntity::OnLoop() {
	return Anim_Control.OnAnimate();
}

void CEntity::OnRender(SDL_Surface* Surf_Display, int offX, int offY) {
	Anim_Control.OnDraw(Surf_Display, (X + offX)*TILE_W, (Y + offY)*TILE_H);
}

void CEntity::GetLoc(int* x, int* y) {
	if (x)
		*x = this->X;
	if (y)
		*y = this->Y;
}
