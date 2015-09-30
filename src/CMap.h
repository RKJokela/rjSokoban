#ifndef		CMAP_H
#define		CMAP_H

#include <SDL.h>
#include <vector>
#include <string>

#include "CTile.h"
#include "SpriteSheet.h"
//#include "CSurface.h"

class CMap {
public:
	CMap();

	bool LoadMapFromText(const std::vector<std::string> &lines);
	void OnRender(SDL_Surface* Surf_Display, int MapX = 0, int MapY = 0);

	int  TypeOfTileAt(int x, int y);
	void BlockMoved(int x0, int y0, int dx, int dy);

	TiledSpriteSheet* Tileset;
	int startX;
	int startY;
	int width;
	int height;
	std::vector<SDL_Point> startBlocks;

private:
	std::vector<TileType> TileList;
};

#endif	//	CMAP_H
