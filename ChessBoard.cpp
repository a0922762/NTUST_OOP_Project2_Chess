#include "ChessBoard.h"

ChessBoard::ChessBoard(QWidget* parent)
	: QWidget(parent), firstClick(true)
{
	//TODO: initialize chessPieces
	//GUI
}

void ChessBoard::chessPiecesClicked(Position pos) {
	// �p�G�I��Ů�
	if (chessPieces[pos.row][pos.col] == nullptr) {

		if (firstClick) {
			return; // do nothing
		}
		else {
			// ���ʺX�l
			// �p�G���ʦ��\
				// �������ʤ�
			// �_�h
				// do nothing
		}
	}
	else {
		if (firstClick) {
			// ��ܥi����/��������m
		}
		else {
			// �Y�X�l
			// �p�G�Y���\
				// �������ʤ�
			// �_�h
			// do nothing
		}
	}

	firstClick = !firstClick;
}
