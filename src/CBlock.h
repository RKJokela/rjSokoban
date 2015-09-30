#ifndef		CBLOCK_H
#define		CBLOCK_H

#include "CEntity.h"
#include "CMap.h"
#include "SpriteSheet.h"
#include <vector>

class CBlock : public CEntity {
public:
	static std::vector<CBlock*> BlockList;
	static CBlock* BlockExistsAt(int x, int y);
	static bool LoadBlocksFromFile(const char* file, VerticalSpriteSheet* SS);
	CBlock(int x, int y, CMap* onMap);
	~CBlock();
	bool CheckForHome();
	void OnMove(int x, int y);
	void OnCleanup();
private:
	CMap* map;
};

#endif	//	CBLOCK_H
