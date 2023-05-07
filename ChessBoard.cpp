#include "ChessBoard.h"

ChessBoard::ChessBoard(QWidget* parent)
	: QWidget(parent), firstClick(true)
{
	//TODO: initialize chessPieces
	//GUI
}

void ChessBoard::chessPiecesClicked(Position pos) {
	// 如果點到空格
	if (chessPieces[pos.row][pos.col] == nullptr) {

		if (firstClick) {
			return; // do nothing
		}
		else {
			// 移動旗子
			// 如果移動成功
				// 切換移動方
			// 否則
				// do nothing
		}
	}
	else {
		if (firstClick) {
			// 顯示可移動/攻擊的位置
		}
		else {
			// 吃旗子
			// 如果吃成功
				// 切換移動方
			// 否則
			// do nothing
		}
	}

	firstClick = !firstClick;
}
