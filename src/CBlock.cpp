#include "CBlock.h"
#include "Defines.h"

std::vector<CBlock*> CBlock::BlockList;

CBlock::CBlock(int x, int y, CMap* onMap) : CEntity(),
	map(onMap) {
	this->X = x;
	this->Y = y;
	BlockList.push_back(this);
}

CBlock::~CBlock() { OnCleanup(); }

bool CBlock::CheckForHome() {
	if (map->TypeOfTileAt(X, Y) == TILE_TYPE_HOMEWITHBLOCK)
		return true;
	return false;
}

void CBlock::OnMove(int moveX, int moveY) {
	map->BlockMoved(X, Y, moveX, moveY);
	X += moveX;
	Y += moveY;
}

CBlock* CBlock::BlockExistsAt(int x, int y) {
	if (x < 0 || x > MAP_W ||
		y < 0 || y > MAP_H)
		return NULL;
	for (auto block = BlockList.begin(); block != BlockList.end(); block++) {
		if (*block
			&& (*block)->X == x
			&& (*block)->Y == y)
			return *block;
	}
	return NULL;
}

bool CBlock::LoadBlocksFromFile(const char* file, VerticalSpriteSheet* SS) {
	FILE* FileHandle = fopen(file, "r");
	if (!FileHandle)
		return false;
	int BlockCount;
	fscanf(FileHandle, "%d\n", &BlockCount);
	for (int i = 0; i < BlockCount; i++) {
		CBlock* tmp;
		int x, y;
		fscanf(FileHandle, "%d ",  &x);
		fscanf(FileHandle, "%d\n", &y);
		tmp = new CBlock(x, y, NULL);
		tmp->SetSpriteSheet(SS);
	}

	fclose(FileHandle);
	return true;
}

void CBlock::OnCleanup() {
	// remove it from the list
	for (int i = 0; i < BlockList.size(); i++) {
		if (BlockList[i] == this)
			BlockList[i] = NULL;
	}
	// must delete spritesheet separately
}
