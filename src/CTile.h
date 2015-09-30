#ifndef		CTILE_H
#define		CTILE_H

enum TileType {
	TILE_TYPE_NORMAL = 0,
	TILE_TYPE_WALL,
	TILE_TYPE_HOMEOFBLOCK,
	TILE_TYPE_HOMEWITHBLOCK,

	TILE_TYPE_NONE
};

class CTile {
public:
	// used to locate it on the spritesheet
	int	Index;
	// TileType value
	int	Type;

	CTile(int index = 0, int type = TILE_TYPE_NONE) : Index(index), Type(type) {}

	void operator=(const CTile& Right) { Index = Right.Index; Type = Right.Type; }
};

#endif	//	CTILE_H
