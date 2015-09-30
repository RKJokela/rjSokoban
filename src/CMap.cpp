#include "CMap.h"
#include "Defines.h"
#include "CSurface.h"
#include "CBlock.h"

void clearTile(std::vector<TileType> &vec, int x, int y);
void clearOutsideFloors(std::vector<TileType> &vec, int bottom);

CMap::CMap() :
	Tileset(NULL),
	startX(0), startY(0),
	startBlocks(),
	TileList(),
	width(0), height(0) {	
		startBlocks.clear();
		TileList.reserve(MAP_W*MAP_H);
		int n = startBlocks.size();
		int m = TileList.size();
	}

bool CMap::LoadMapFromText(const std::vector<std::string> &lines) {
	int maxW = 0;
	int y = 0;
	startBlocks.clear();
	TileList.clear();
	for (int i = 0; i < MAP_W*MAP_H; i++) {
		TileList.push_back(TILE_TYPE_NONE);
	}
	int test = TileList.size();
	for (auto line = lines.begin(); line != lines.end(); line++, y++) {
		if (line->length() > maxW)
			maxW = line->length();
		const char* ch = line->c_str();
		int x = 0;
		TileType t;
		bool outsideWall = true;
		bool hasBlock = false;
		while (*ch) {
			switch (*ch) {
			case '@': // player
				startX = x;
				startY = y;
			case ' ': // floor
				if (outsideWall)
					t = TILE_TYPE_NONE;
				else
					t = TILE_TYPE_NORMAL;
				break;
			case '#': // wall
				t = TILE_TYPE_WALL;
				if (outsideWall)
					outsideWall = false;
				break;
			case '.': // home of block
				t = TILE_TYPE_HOMEOFBLOCK;
				break;
			case '$': // block
				t = TILE_TYPE_NORMAL;
				hasBlock = true;
				break;
			case '*': // block in home
				t = TILE_TYPE_HOMEWITHBLOCK;
				hasBlock = true;
				break;
			default:
				break;
			}
			TileList[x + y*MAP_W] = t;
			if (hasBlock) {
				SDL_Point p = { x, y };
				startBlocks.push_back(p);
			}
			hasBlock = false;
			x++;
			ch++;
		}
	}
	this->width = maxW;
	this->height = y;

	clearOutsideFloors(TileList, height);

	return true;
}

void CMap::OnRender(SDL_Surface* Surf_Display, int MapX, int MapY) {
	if (!Tileset)
		return;

	// convert to pixel coords
	MapX *= TILE_W;
	MapY *= TILE_H;

	int x0 = MapX;
	int id = 0;

	for (int y = 0; y < MAP_H; y++) {
		for (int x = 0; x < MAP_W; x++) {
			if (TileList[id] != TILE_TYPE_NONE) {
				if (TileList[id] >= Tileset->frames.size()) {
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "CMap::OnRender: Tile index %d in map file is too large for tileset", TileList[id]);
					return;
				}
				CSurface::OnDraw(Surf_Display, Tileset->sheet, MapX, MapY, &Tileset->frames.at(TileList[id]));
			}
			id++;
			MapX += TILE_W;
		}
		MapY += TILE_H;
		MapX = x0;
	}
}

int CMap::TypeOfTileAt(int x, int y) {
	if (x < 0 || x > MAP_W ||
		y < 0 || y > MAP_H)
		return TILE_TYPE_NONE;
	int checkID = y*MAP_W + x;
	return TileList[checkID];
}

void CMap::BlockMoved(int x0, int y0, int dx, int dy) {
	int x1 = x0 + dx;
	int y1 = y0 + dy;
	// check if moving off its home
	if (TypeOfTileAt(x0, y0) == TILE_TYPE_HOMEWITHBLOCK)
		TileList[x0 + y0*MAP_W] = TILE_TYPE_HOMEOFBLOCK;
	// check if moving onto its home
	if (TypeOfTileAt(x1, y1) == TILE_TYPE_HOMEOFBLOCK)
		TileList[x1 + y1*MAP_W] = TILE_TYPE_HOMEWITHBLOCK;
}

void clearTile(std::vector<TileType> &vec, int x, int y) {
	vec.at(x + y*MAP_W) = TILE_TYPE_NONE;
}

void clearFromUp(std::vector<TileType> &vec, bool* checked, int x, int y) {
	if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
		return;
	clearTile(vec, x, y);
	int i = x + y*MAP_W;
	checked[i] = true;
	
	// up
	if (!checked[i-MAP_W] && vec[i-MAP_W] != TILE_TYPE_WALL)
		clearFromUp(vec, checked, x, y-1);
	// right
	if (!checked[i+1] && vec[i+1] != TILE_TYPE_WALL)
		clearFromUp(vec, checked, x+1, y);
	// left
	if (!checked[i-1] && vec[i-1] != TILE_TYPE_WALL)
		clearFromUp(vec, checked, x-1, y);
}

void clearFromDown(std::vector<TileType> &vec, bool* checked, int x, int y) {
	if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
		return;
	clearTile(vec, x, y);
	int i = x + y*MAP_W;
	checked[i] = true;
	
	// down
	if (!checked[i+MAP_W] && vec[i+MAP_W] != TILE_TYPE_WALL)
		clearFromDown(vec, checked, x, y+1);
	// left
	if (!checked[i-1] && vec[i-1] != TILE_TYPE_WALL)
		clearFromDown(vec, checked, x-1, y);
	// right
	if (!checked[i+1] && vec[i+1] != TILE_TYPE_WALL)
		clearFromDown(vec, checked, x+1, y);
}

void clearOutsideFloors(std::vector<TileType> &vec, int bottom) {
	bool checked[MAP_W*MAP_H];
	memset(checked, 0, sizeof(checked));
	int i;
	bottom--;
	int right0 = 0;
	int right1 = 0;
	for (int i = MAP_W-1; i > 0; i--)
		if (vec[bottom*MAP_W+i] == TILE_TYPE_WALL) {
			right0 = i;
			break;
		}
	for (int i = MAP_W-1; i > 0; i--)
		if (vec[i] == TILE_TYPE_WALL) {
			right1 = i;
			break;
		}
	for (i = 0; i <= right0+1; i++) {
		if ((vec[bottom*MAP_W + i] != TILE_TYPE_WALL) && !checked[bottom*MAP_W + i])
			clearFromUp(vec, checked, i, bottom);
	}
	memset(checked, 0, sizeof(checked));
	for (i = 0; i <= right1+1; i++) {
		if ((vec[i] != TILE_TYPE_WALL) && !checked[i])
			clearFromDown(vec, checked, i, 0);
	}
}
