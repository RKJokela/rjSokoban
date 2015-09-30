#include <algorithm>
#include <iostream>
#include <fstream>
#include "CApp.h"
#include "CSurface.h"
#include "CEventFilter.h"
#include "CMap.h"
#include "CBlock.h"
//#include "rj_timer.h"

GameInputs inputs = { 0 };

// big for_each loop over entity list
#define FOR_EACH_ENTITY(LOCALVAR)	std::for_each(CEntity::EntityList.begin(),\
												  CEntity::EntityList.end(),\
												  [LOCALVAR](CEntity* entity){if(entity){
#define FOR_EACH_BLOCK(LOCALVAR)	std::for_each(CBlock::BlockList.begin(),\
												  CBlock::BlockList.end(),\
												  [LOCALVAR](CBlock* block){if(block){
#define FOR_EACH_END	}});

CApp::CApp() : CurrentMap(0),
			   Running(true),
			   InputHappened(false),
			   NeedsRedraw(true),
			   Surf_Display(NULL),
			   Texture(NULL),
			   Renderer(NULL),
			   Window(NULL),
			   // OTHER STUFF HERE
			   maps(),
			   block(NULL),
			   tiles(NULL),
			   ActionJackson(0, 0, NULL),
               highestLevel(0) { maps.reserve(MAX_MAPS); }

int CApp::Execute() {
	if (!OnInit()) return -1;

	//jokela::Timer timer(FPS);

	SDL_Event Event;

	//timer.Start();

	while (Running) {
		while (SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}

		if (true/*timer.ReadyForNextFrame()*/) {
			OnLoop();
			OnRender();
		}
	}

	OnCleanup();

	return 0;
}

SDL_LogOutputFunction logfunc;
void* data;

void myLog(void* userdata, int category, SDL_LogPriority priority, const char* msg) {
	std::cerr << msg << std::endl;
	logfunc(userdata, category, priority, msg);
}

bool CApp::OnInit() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { return false; }

	if (IMG_Init(IMG_INIT_PNG) == 0) { return false; }

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
	SDL_LogGetOutputFunction(&logfunc, NULL);
	SDL_LogSetOutputFunction(myLog, NULL);

	SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &Window, &Renderer);
	if (!Window || !Renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "CreateWindowAndRenderer: %s", SDL_GetError());
		return false;
	}
	SDL_SetWindowTitle(Window, TITLE);
	
	Surf_Display = SDL_CreateRGBSurface(0, SCREEN_W, SCREEN_H, BPP,
		RMASK, GMASK, BMASK, AMASK);
	if (!Surf_Display) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "CreateRGBSurface: %s", SDL_GetError());
		return false;
	}
	SDL_FillRect(Surf_Display, NULL, 0);

	Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_W, SCREEN_H);
	if (!Texture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "CreateTexture: %s", SDL_GetError());
		return false;
	}

	CEventFilter::SetDefaults(); // or use your own filters
	CEventFilter::TurnFilterOn(&InputHappened);

	// PROGRAM SPECIFIC CODE HERE
	
	bg = SDL_CreateRGBSurface(0, SCREEN_W, SCREEN_H, BPP, RMASK, GMASK, BMASK, AMASK);
	if (!bg) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "CSurface::OnLoad: %s", SDL_GetError());
		return false;
	}
	SDL_FillRect(bg, NULL, 0xfff0f0f0);

	VerticalSpriteSheet* dude = new VerticalSpriteSheet("gfx/char.png", 1, Surf_Display->format);

	ActionJackson.SetSpriteSheet(dude);
	ActionJackson.AnimState = 0;
	CEntity::EntityList.push_back(&ActionJackson);

	SDL_SetWindowIcon(Window, dude->sheet);

	tiles = new TiledSpriteSheet("gfx/tiles.png", 2, 2, Surf_Display->format);

	block = new VerticalSpriteSheet("gfx/block.png", 1, Surf_Display->format);

	if (!LoadMaps("maps/sokoban.map", tiles))
		return false;

	std::ifstream fin("save");
	if (fin) {
		fin >> CurrentMap;
		fin.ignore(1, '\n');
		fin >> highestLevel;
	}
	fin.close();
	ResetMap(CurrentMap);

	return true;
}

void CApp::OnEvent(SDL_Event* Event) {
	if (Event->type == SDL_WINDOWEVENT)
		// need to redraw the window
		NeedsRedraw = true;
	CEvent::OnEvent(Event);
}

// these should process the first time and then reset the InputHappened flag
//#define PROCESS_ONCE		(inputs.Confirm || inputs.Cancel || inputs.Quit)
// these should update every frame as long as the key is pressed
//#define	PROCESS_CONTINUOUS	(inputs.Up || inputs.Down || inputs.Left || inputs.Right)

void CApp::OnLoop() {
	// process input
	if (InputHappened) {
		NeedsRedraw = true;

		// quit game
		if (inputs.Quit) {
			Running = false;
			return;
		}
		// reset color modulation (easier on the eyes)
		if (inputs.Confirm) {
			SDL_SetTextureColorMod(Texture, 255, 255, 255);
		}
		// reset/change level
		if (inputs.Reset) {
			ResetMap(CurrentMap);
		}
		else if (inputs.NextLevel) {
			if (highestLevel > CurrentMap && CurrentMap < maps.size() - 1) 
				ResetMap(CurrentMap+1);
		}
		else if (inputs.PrevLevel) {
			if (CurrentMap > 0) 
				ResetMap(CurrentMap-1);
		}
		else {
			// movement
			CBlock* pushed = NULL;
			if (inputs.Left) {
				int result = ActionJackson.CanPlayerMoveLeft(&pushed);
				if (result != NO) {
					ActionJackson.OnMove(-1, 0, result == BLOCK);
					if (result == BLOCK)
						pushed->OnMove(-1, 0);
				}
			}
			else if (inputs.Right) {
				int result = ActionJackson.CanPlayerMoveRight(&pushed);
				if (result != NO) {
					ActionJackson.OnMove(1, 0, result == BLOCK);
					if (result == BLOCK)
						pushed->OnMove(1, 0);
				}
			}
			else if (inputs.Up) {
				int result = ActionJackson.CanPlayerMoveUp(&pushed);
				if (result != NO) {
					ActionJackson.OnMove(0, -1, result == BLOCK);
					if (result == BLOCK)
						pushed->OnMove(0, -1);
				}
			}
			else if (inputs.Down) {
				int result = ActionJackson.CanPlayerMoveDown(&pushed);
				if (result != NO) {
					ActionJackson.OnMove(0, 1, result == BLOCK);
					if (result == BLOCK)
						pushed->OnMove(0, 1);
				}
			}
			else if (inputs.Cancel) {
				ActionJackson.UndoLastMove();
			}
		} // ~movement

		//if (PROCESS_ONCE)
			InputHappened = false;
	}

	// add other logic here
	FOR_EACH_ENTITY(this)
		if (entity->OnLoop()) {
			NeedsRedraw = true;
		}
	FOR_EACH_END

	// checking if all blocks are home
	bool blocksAreHome = true;
	FOR_EACH_BLOCK(&blocksAreHome)
		if (!block->CheckForHome())
			blocksAreHome = false;
	FOR_EACH_END

	if (blocksAreHome) {
		if (CurrentMap == (maps.size() - 1))
			Running = false;
		else {
			ResetMap(CurrentMap + 1);
		}
	}
}

void CApp::OnRender() {
	if (NeedsRedraw) {
		//SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Rendering the frame at %dms", SDL_GetTicks());
		//Drawing on Surf_Display
		int offX = (MAP_W - maps[CurrentMap]->width)/2;
		int offY = (MAP_H - maps[CurrentMap]->height)/2;
		CSurface::OnDraw(Surf_Display, bg, 0, 0);
		maps[CurrentMap]->OnRender(Surf_Display, offX, offY);
		FOR_EACH_ENTITY(&)
			entity->OnRender(Surf_Display, offX, offY);
		FOR_EACH_END
		FOR_EACH_BLOCK(&)
			block->OnRender(Surf_Display, offX, offY);
		FOR_EACH_END

		//REUSABLE
		SDL_UpdateTexture(Texture, NULL, Surf_Display->pixels, Surf_Display->pitch);
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, Texture, NULL, NULL);
		SDL_RenderPresent(Renderer);

		NeedsRedraw = false;
	}
}

void CApp::OnCleanup() {
	//OTHER CLEANUP HERE
	// save current map
	std::ofstream fout("save");
	if (fout) {
		fout << CurrentMap << std::endl;
		fout << highestLevel << std::endl;
	}
	fout.close();
	// free memory
	while (!CBlock::BlockList.empty()) {
		CBlock* e = CBlock::BlockList.back();
		if (e != NULL) delete e;
		CBlock::BlockList.pop_back();
	}
	if (tiles)
		delete tiles;
	if (block)
		delete block;
	// free maps
	while (!maps.empty()) {
		CMap* m = maps.back();
		if (m) delete m;
		maps.pop_back();
	}

	//REUSABLE
	SDL_DestroyTexture(Texture);
	SDL_FreeSurface(Surf_Display);
	SDL_FreeSurface(bg);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	IMG_Quit();
	SDL_Quit();
}

// Event handlers - add as necessary
void CApp::OnExit() {
	Running = false;
}

void CApp::OnKeyDown(SDL_Keycode key, Uint16 mod, SDL_Scancode scancode, bool repeat) {
#ifdef		WALK_ONLY
	// Don't process repeats
	if (repeat) {
		InputHappened = false;
		return;
	}
#endif	//	WALK_ONLY

	switch (key) {
	case SDLK_UP:
		inputs.Up = true;
		break;
	case SDLK_DOWN:
		inputs.Down = true;
		break;
	case SDLK_LEFT:
		inputs.Left = true;
		break;
	case SDLK_RIGHT:
		inputs.Right = true;
		break;
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		inputs.Confirm = true;
		break;
	case SDLK_LEFTBRACKET:
		inputs.PrevLevel = true;
		break;
	case SDLK_RIGHTBRACKET:
		inputs.NextLevel = true;
		break;
	case SDLK_r:
		inputs.Reset = true;
		break;
	case SDLK_u:
	case SDLK_BACKSPACE:
		inputs.Cancel = true;
		break;
	case SDLK_q:
	case SDLK_ESCAPE:
		inputs.Quit = true;
		break;
	default:
		break;
	}
}

void CApp::OnKeyUp(SDL_Keycode key, Uint16 mod, SDL_Scancode scancode, bool repeat) {
	switch (key) {
	case SDLK_UP:
		inputs.Up = false;
		break;
	case SDLK_DOWN:
		inputs.Down = false;
		break;
	case SDLK_LEFT:
		inputs.Left = false;
		break;
	case SDLK_RIGHT:
		inputs.Right = false;
		break;
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		inputs.Confirm = false;
		break;
	case SDLK_LEFTBRACKET:
		inputs.PrevLevel = false;
		break;
	case SDLK_RIGHTBRACKET:
		inputs.NextLevel = false;
		break;
	case SDLK_r:
		inputs.Reset = false;
		break;
	case SDLK_u:
	case SDLK_BACKSPACE:
		inputs.Cancel = false;
		break;
	case SDLK_q:
	case SDLK_ESCAPE:
		inputs.Quit = false;
		break;
	default:
		break;
	}
}

bool CApp::LoadMaps(const char* file, TiledSpriteSheet* ss) {
	if (!file)
		return false;
	std::ifstream fin;
	fin.open(file);

	// get all non-comment lines
	std::vector<std::string> lines;
	std::string line(MAP_W, 0);
	int count = 0;
	while (fin) {
		char tmpLine[80];
		fin.getline(tmpLine, 80);
		if (tmpLine[0] == 0) {
			tmpLine[0] = 'X';
			count++;
		}
		if (tmpLine[0] != ';') {
			line.clear();
			line.assign(tmpLine);
			lines.push_back(line);
		}
	}
	fin.close();

	// split it into maps
	//maps.reserve(count);
	auto first = lines.begin();
	auto last = first;
	for (int i = 0; i < count; i++) {
		// step to find the last row
		while ((++last)->at(0) != 'X');
		// copy to the new vector
		std::vector<std::string> map(first, last);
		// load the map object
		CMap* cmap = new CMap();
		cmap->LoadMapFromText(map);
		cmap->Tileset = ss;
		maps.push_back(cmap);
		// set first to next map
		last++;
		first = last;
	}
	lines.clear();

	return true;
}

void CApp::ResetMap(int mapIndex) {
	// remove all blocks
	for (auto itr = CBlock::BlockList.begin(); itr != CBlock::BlockList.end(); itr++) {
		CBlock* pBlock = *itr;
		if (pBlock) {
			pBlock->OnMove(-MAP_W, -MAP_H); // this will reset the tile if it was on its home
			delete pBlock;
		}
	}
	CBlock::BlockList.clear();

	CurrentMap = mapIndex;
	if (CurrentMap > highestLevel)
		highestLevel = CurrentMap;
	CMap* pMap = maps[mapIndex];
	ActionJackson.SetLoc(pMap->startX, pMap->startY);
	ActionJackson.map = pMap;
	ActionJackson.ClearUndoList();

	// place all blocks
	for (int i = 0; i < pMap->startBlocks.size(); i++) {
		SDL_Point p = pMap->startBlocks[i];
		CBlock* pBlock = new CBlock(p.x, p.y, pMap);
		pBlock->SetSpriteSheet(block);
		pBlock->OnMove(0, 0); // this will make the tile change to HOMEWITHBLOCK if applicable
		CBlock::BlockList.push_back(pBlock);
	}

	// add some flavor
	Uint8 redness = 2*(Uint8)CurrentMap;
	SDL_SetTextureColorMod(Texture, 255, 255 - redness, 255 - redness);
}
