#ifndef		CPLAYER_H
#define		CPLAYER_H

#include "CEntity.h"
#include "CMap.h"
#include "CBlock.h"

enum {
	YES = 0,
	NO,
	BLOCK
};

typedef struct _undo {
	int dx;
	int dy;
	bool pushed;
	_undo* prev;
	_undo* next;

	_undo(int x, int y, bool Push, _undo* Prev, _undo* Next);
} Undo, *pUndo;

class CPlayer : public CEntity {
public:
	CPlayer(int x, int y, CMap* onMap);
	void SetLoc(int x, int y);
	void OnMove(int x, int y, bool pushed = false);
	void OnCleanup();
	void UndoLastMove();
	void ClearUndoList();

	int CanPlayerMoveLeft(CBlock** pushingBlock);
	int CanPlayerMoveRight(CBlock** pushingBlock);
	int CanPlayerMoveUp(CBlock** pushingBlock);
	int CanPlayerMoveDown(CBlock** pushingBlock);

	void ChangeMap(CMap* newMap, int startX, int startY);
	
	CMap* map;

private:
	int CanPlayerMove(int dx, int dy, CBlock** pushingBlock);
	// Undo moves list
	pUndo head;
	pUndo tail;
	int memories;
};

#endif	//	CPLAYER_H
