#include "ChessPieces.h"

ChessPieces::ChessPieces(int id) {
	dead = false;
	if (id >= 16) {
		color = COLOR::White;
		id -= 16;
	}
	else {
		color = COLOR::Black;
	}

	switch (id) {
		case 0: type = TYPE::Rook; break;
		case 1: type = TYPE::Knight; break;
		case 2: type = TYPE::Bishop; break;
		case 3: type = TYPE::Queen; break;
		case 4: type = TYPE::King; break;
		case 5: type = TYPE::Bishop; break;
		case 6: type = TYPE::Knight; break;
		case 7: type = TYPE::Rook; break;
		default: type = TYPE::Pawn; break;
	}

	this->id = id;

	if (color == COLOR::Black) {
		row = id / 8;
		col = id % 8;
	}
	else {
		row = 7 - id / 8;
		col = id % 8;
	}
}
