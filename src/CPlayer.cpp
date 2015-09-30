#include "CPlayer.h"
#include "Defines.h"

_undo::_undo(int x, int y, bool Push, _undo* Prev, _undo* Next) :
	dx(x),
	dy(y),
	pushed(Push),
	prev(Prev),
	next(Next) { }

CPlayer::CPlayer(int x, int y, CMap* onMap) : CEntity(),
	map(onMap), head(NULL), tail(NULL), memories(0) { 
	X = x;
	Y = y;
}

void CPlayer::SetLoc(int x, int y) {
	X = x;
	Y = y;
}

void CPlayer::OnMove(int x, int y, bool pushed) {
	X += x;
	Y += y;
	// add to the undo list
	pUndo lastMove = new Undo(x, y, pushed, NULL, head);
	if (head) // head is now second in chain - must link to new head
		head->prev = lastMove;
	head = lastMove;
	memories++;
	if (memories == 1) // first move to remember, must set tail
		tail = head;
	else if (memories > MAX_UNDO) { // trim the list to cap remembered moves
		// could do this without a tail to save memory, but simpler this way
		tail = tail->prev;
		delete tail->next;
		tail->next = NULL;
		memories--;
	}
}

void CPlayer::UndoLastMove() {
	if (!head)
		return; // don't undo anything, don't modify list, and DON'T decrement memories!
	pUndo toUndo = head;
	if (toUndo->dx == 0 && toUndo->dy == 0) {
		// do nothing
	}
	else { // undo the move (must check for blocks to replace too!)
		CBlock* pBlock = NULL;
		if (toUndo->pushed) {
			for (auto itr = CBlock::BlockList.begin(); itr != CBlock::BlockList.end(); itr++)
			{
				if (*itr) {
					int x, y;
					(*itr)->GetLoc(&x, &y);
					if ((x == X + toUndo->dx) && (y == Y + toUndo->dy))
						pBlock = *itr;
				}
			}
		}
		X -= toUndo->dx;
		Y -= toUndo->dy;
		if (pBlock)
			pBlock->OnMove(-toUndo->dx, -toUndo->dy);
	}
	// remove the top of the list
	head = head->next;
	delete toUndo;
	if (head) // list is not empty
		head->prev = NULL;
	else // that was the last undo
		tail = NULL;
	memories--;
}

void CPlayer::ClearUndoList() {
	tail = NULL;
	while (head) {
		pUndo tmp = head;
		head = head->next;
		delete tmp;
	}
	memories = 0;
}

void CPlayer::OnCleanup() {
	ClearUndoList();
	if (SpriteSheet) {
		delete SpriteSheet;
		SpriteSheet = NULL;
	}
	Anim_Control.SpriteSheet = NULL;
}

int CPlayer::CanPlayerMoveLeft(CBlock** pushingBlock)  { return CanPlayerMove(-1,  0, pushingBlock); }
int CPlayer::CanPlayerMoveRight(CBlock** pushingBlock) { return CanPlayerMove( 1,  0, pushingBlock); }
int CPlayer::CanPlayerMoveUp(CBlock** pushingBlock)    { return CanPlayerMove( 0, -1, pushingBlock); }
int CPlayer::CanPlayerMoveDown(CBlock** pushingBlock)  { return CanPlayerMove( 0,  1, pushingBlock); }

void CPlayer::ChangeMap(CMap* newMap, int startX, int startY) {
	map = newMap;
	X = startX;
	Y = startY;
}

int CPlayer::CanPlayerMove(int dx, int dy, CBlock** pushingBlock) {
	CBlock* push = NULL;
	// hit a wall
	if (map->TypeOfTileAt(X + dx, Y + dy) == TILE_TYPE_WALL)
		return NO;
	// hit a block
	if ((push = CBlock::BlockExistsAt(X + dx, Y + dy)) != NULL) {
		// 00@  and  #0@
		if (CBlock::BlockExistsAt(X + 2*dx, Y + 2*dy) || map->TypeOfTileAt(X + 2*dx, Y + 2*dy) == TILE_TYPE_WALL)
			return NO;
		else { // _0@
			*pushingBlock = push;
			return BLOCK;
		}
	}
	// empty space
	return YES;
}
