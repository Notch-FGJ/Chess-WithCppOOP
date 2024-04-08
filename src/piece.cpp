#include "piece.h"
#include "board.h"

bool isValidMove(std::pair<int, int> from, std::pair<int, int> to)
{
	// Out of bounds
	if (from.first < 0 || from.first > 7 || from.second < 0 || from.second > 7 || to.first < 0 || to.first > 7 || to.second < 0 || to.second > 7)
		return false;
	// Same position
	if (from == to)
		return false;
	return true;
}

bool isBlocked(std::pair<int, int> from, std::pair<int, int> to)
{
	// Check if there is a piece between from and to
	int dx = to.first - from.first;
	int dy = to.second - from.second;

	// Horizontal move
	if (dx == 0)
	{
		for (int i = std::min(from.second, to.second) + 1; i < std::max(from.second, to.second); i++)
		{
			if (Board::getInstance()->getPiece({ from.first, i }) != nullptr)
				return true;
		}
	}

	// Vertical move
	if (dy == 0)
	{
		for (int i = std::min(from.first, to.first) + 1; i < std::max(from.first, to.first); i++)
		{
			if (Board::getInstance()->getPiece({ i, from.second }) != nullptr)
				return true;
		}
	}

	// Diagonal move
	if (abs(dx) == abs(dy))
	{
		if (dx == dy)   // Slope = 1
		{
			int step = (dx > 0) ? 1 : -1;
			for (int i = 1; i < abs(dx); i++)
			{
				if (Board::getInstance()->getPiece({ from.first + i * step, from.second + i * step }) != nullptr)
					return true;
			}
		}
		else            // Slope = -1
		{
			int step = (dx > 0) ? 1 : -1;
			for (int i = 1; i < abs(dx); i++)
			{
				if (Board::getInstance()->getPiece({ from.first + i * step, from.second - i * step }) != nullptr)
					return true;
			}
		}
	}
	return false;
}

AbstractPiece::AbstractPiece(Color color)
{
	this->color = color;
	this->moved = false;
}

Color AbstractPiece::getColor() const
{
	return color;
}

bool AbstractPiece::isMoved() const
{
	return moved;
}

void AbstractPiece::move()
{
	moved = true;
}

Pawn::Pawn(Color color) : AbstractPiece(color) {}

WCHAR Pawn::getSymbol() const
{
	return (getColor() == Color::WHITE) ? whiteSymbol : blackSymbol;
}

Rook::Rook(Color color) : AbstractPiece(color) {}

WCHAR Rook::getSymbol() const
{
	return (getColor() == Color::WHITE) ? whiteSymbol : blackSymbol;
}

Knight::Knight(Color color) : AbstractPiece(color) {}

WCHAR Knight::getSymbol() const
{
	return (getColor() == Color::WHITE) ? whiteSymbol : blackSymbol;
}

Bishop::Bishop(Color color) : AbstractPiece(color) {}

WCHAR Bishop::getSymbol() const
{
	return (getColor() == Color::WHITE) ? whiteSymbol : blackSymbol;
}

Queen::Queen(Color color) : AbstractPiece(color) {}

WCHAR Queen::getSymbol() const
{
	return (getColor() == Color::WHITE) ? whiteSymbol : blackSymbol;
}

King::King(Color color) : AbstractPiece(color) {}

WCHAR King::getSymbol() const
{
	return (getColor() == Color::WHITE) ? whiteSymbol : blackSymbol;
}

bool Pawn::isMoveValid(std::pair<int, int> from, std::pair<int, int> to) const
{
	if (!isValidMove(from, to))
		return false;

	const AbstractPiece* toPiece = Board::getInstance()->getPiece(to);

	// Cannot capture own piece
	if (toPiece && this->getColor() == toPiece->getColor())
		return false;

	int dx = to.first - from.first;
	int dy = to.second - from.second;

	// White pawn can only move up
	if (this->getColor() == Color::WHITE && dy < 0)
		return false;
	// Same that, black pawn can only move down
	if (this->getColor() == Color::BLACK && dy > 0)
		return false;

	// If the pawn has not moved yet
	if (!this->isMoved())
	{
		// Two squares forward
		if (dx == 0 && abs(dy) == 2 && !isBlocked(from, to) && toPiece == nullptr)
		{
			return true;
		}
	}

	// Otherwise, the pawn can only move one square forward or capture diagonally
	// One square forward
	if (dx == 0 && abs(dy) == 1 && toPiece == nullptr)
		return true;
	// Capture diagonally
	if (abs(dx) == 1 && abs(dy) == 1 && toPiece != nullptr)
		return true;

	return false;
}

bool Rook::isMoveValid(std::pair<int, int> from, std::pair<int, int> to) const
{
	if (!isValidMove(from, to))
		return false;

	const AbstractPiece* toPiece = Board::getInstance()->getPiece(to);

	// Cannot capture own piece
	if (toPiece && this->getColor() == toPiece->getColor())
		return false;

	int dx = to.first - from.first;
	int dy = to.second - from.second;

	if (dx == 0 || dy == 0)
	{
		if (!isBlocked(from, to))
			return true;
	}

	return false;
}

bool Knight::isMoveValid(std::pair<int, int> from, std::pair<int, int> to) const
{
	if (!isValidMove(from, to))
		return false;

	const AbstractPiece* toPiece = Board::getInstance()->getPiece(to);

	// Cannot capture own piece
	if (toPiece && this->getColor() == toPiece->getColor())
		return false;

	int dx = to.first - from.first;
	int dy = to.second - from.second;

	if ((abs(dx) == 2 && abs(dy) == 1) ||
		(abs(dx) == 1 && abs(dy) == 2))
		return true;

	return false;
}

bool Bishop::isMoveValid(std::pair<int, int> from, std::pair<int, int> to) const
{
	if (!isValidMove(from, to))
		return false;


	const AbstractPiece* toPiece = Board::getInstance()->getPiece(to);

	// Cannot capture own piece
	if (toPiece && this->getColor() == toPiece->getColor())
		return false;

	int dx = to.first - from.first;
	int dy = to.second - from.second;

	if (abs(dx) == abs(dy))
	{
		if (!isBlocked(from, to))
			return true;
	}

	return false;
}

bool Queen::isMoveValid(std::pair<int, int> from, std::pair<int, int> to) const
{
	if (!isValidMove(from, to))
		return false;

	const AbstractPiece* toPiece = Board::getInstance()->getPiece(to);

	// Cannot capture own piece
	if (toPiece && this->getColor() == toPiece->getColor())
		return false;

	int dx = to.first - from.first;
	int dy = to.second - from.second;

	if (dx == 0 ||          // Vertical move
		dy == 0 ||          // Horizontal move
		abs(dx) == abs(dy)) // Diagonal move
	{
		if (!isBlocked(from, to))
			return true;
	}

	return false;
}

bool King::isMoveValid(std::pair<int, int> from, std::pair<int, int> to) const
{
	if (!isValidMove(from, to))
		return false;

	const AbstractPiece* toPiece = Board::getInstance()->getPiece(to);

	// Cannot capture own piece
	if (toPiece && this->getColor() == toPiece->getColor())
		return false;

	int dx = to.first - from.first;
	int dy = to.second - from.second;

	if (abs(dx) <= 1 && abs(dy) <= 1 && !Board::getInstance()->isUnderAttack(to, this->getColor() == Color::WHITE ? Color::BLACK : Color::WHITE))
		return true;

	return false;
}