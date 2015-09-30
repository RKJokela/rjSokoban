#ifndef		CAPP_H
#define		CAPP_H

#include <SDL.h>
#include <SDL_image.h>
#include "Defines.h"
#include "CEvent.h"
// OTHER INCLUDES HERE
#include "CEntity.h"
#include "CMap.h"
#include "CPlayer.h"
// -------------------

struct GameInputs {
	bool Up;
	bool Down;
	bool Left;
	bool Right;
	bool PrevLevel;
	bool NextLevel;
	bool Reset;
	bool Confirm;
	bool Cancel;
	bool Quit;
};

class CApp : public CEvent {
public:
	CApp();

	// called by main()
	int  Execute();

public:
	bool OnInit();
	void OnEvent(SDL_Event* Event);
	void OnLoop();
	void OnRender();
	void OnCleanup();

	void OnExit();
	void OnKeyDown(SDL_Keycode key, Uint16 mod, SDL_Scancode scancode, bool repeat);
	void OnKeyUp  (SDL_Keycode key, Uint16 mod, SDL_Scancode scancode, bool repeat);

	// PROGRAM-SPECIFIC FUNCTIONS HERE
	bool LoadMaps(const char* file, TiledSpriteSheet* ss);
	void ResetMap(int mapIndex);

private:
	int  CurrentMap;
	bool Running;
	bool InputHappened;
	bool NeedsRedraw;

	SDL_Surface*  Surf_Display;
	SDL_Texture*  Texture;
	SDL_Window*   Window;
	SDL_Renderer* Renderer;

	// PROGRAM-SPECIFIC MEMBERS HERE
	SDL_Surface* bg;
	std::vector<CMap*> maps;
	VerticalSpriteSheet* block;
	TiledSpriteSheet* tiles;
	CPlayer ActionJackson;
	int highestLevel;
};

#endif	//	CAPP_H
