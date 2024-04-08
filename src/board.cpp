#include "board.h"
#include <iostream>
#include <windows.h>

void Board::initialize()
{
    // White pieces
    board[0][0] = std::make_unique<Rook>(Color::WHITE);   // a1 Rook
    board[1][0] = std::make_unique<Knight>(Color::WHITE); // b1 Knight
    board[2][0] = std::make_unique<Bishop>(Color::WHITE); // c1 Bishop
    board[3][0] = std::make_unique<Queen>(Color::WHITE);  // d1 Queen
    board[4][0] = std::make_unique<King>(Color::WHITE);   // e1 King
    board[5][0] = std::make_unique<Bishop>(Color::WHITE); // f1 Bishop
    board[6][0] = std::make_unique<Knight>(Color::WHITE); // g1 Knight
    board[7][0] = std::make_unique<Rook>(Color::WHITE);   // h1 Rook
    for (int i = 0; i < 8; i++)
        board[i][1] = std::make_unique<Pawn>(Color::WHITE); // a2-h2 Pawns

    // Black pieces
    board[0][7] = std::make_unique<Rook>(Color::BLACK);   // a8 Rook
    board[1][7] = std::make_unique<Knight>(Color::BLACK); // b8 Knight
    board[2][7] = std::make_unique<Bishop>(Color::BLACK); // c8 Bishop
    board[3][7] = std::make_unique<Queen>(Color::BLACK);  // d8 Queen
    board[4][7] = std::make_unique<King>(Color::BLACK);   // e8 King
    board[5][7] = std::make_unique<Bishop>(Color::BLACK); // f8 Bishop
    board[6][7] = std::make_unique<Knight>(Color::BLACK); // g8 Knight
    board[7][7] = std::make_unique<Rook>(Color::BLACK);   // h8 Rook
    for (int i = 0; i < 8; i++)
        board[i][6] = std::make_unique<Pawn>(Color::BLACK); // a7-h7 Pawns
}

Board::Board()
{
    // Initialize the board with nullptrs
    board.resize(8);
    for (auto &row : board)
    {
        row.resize(8);
        for (auto &piece : row)
            piece = nullptr;
    }
    initialize();
}

std::ostream &operator<<(std::ostream &os, WCHAR c)
{
    DWORD n;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &c, 1, &n, NULL);
    return os;
}

void Board::__move(std::pair<int,int> from, std::pair<int,int> to)
{
    board[to.first][to.second] = std::move(board[from.first][from.second]);
    board[from.first][from.second] = nullptr;
}

std::shared_ptr<Board> Board::getInstance()
{
    static std::shared_ptr<Board> instance(new Board());
    return instance;

}

void Board::print() const
{
    system("cls");
    std::cout << "White captured pieces: ";
    for (const auto &piece : capturedWhitePieces)
        std::cout << piece->getSymbol() << " ";
    std::cout << std::endl;
    std::cout << "  a b c d e f g h" << std::endl;
    for (int y = 7; y >= 0; y--)
    {
        std::cout << y + 1 << " ";
        for (int x = 0; x < 8; x++)
        {
            if (board[x][y] == nullptr)
                std::cout << " ";
            else
                std::cout << board[x][y]->getSymbol();
            std::cout << " ";
        }
        std::cout << y + 1 << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
    std::cout << "Black captured pieces: ";
    for (const auto &piece : capturedBlackPieces)
        std::cout << piece->getSymbol() << " ";
    std::cout << std::endl;
}

const AbstractPiece *Board::getPiece(std::pair<int, int> position) const
{
    if (position.first < 0 || position.first > 7 || position.second < 0 || position.second > 7)
        return nullptr;
    return board[position.first][position.second].get();
}

void Board::capture(std::pair<int, int> position)
{
    if (board[position.first][position.second]->getColor() == Color::WHITE)
        capturedWhitePieces.push_back(std::move(board[position.first][position.second]));
    else
        capturedBlackPieces.push_back(std::move(board[position.first][position.second]));
    board[position.first][position.second] = nullptr;
}

void Board::enPassantChance(std::pair<int, int> position)
{
    isEnPassantChance = true;
    enPassantPosition = position;
}

bool Board::move(std::pair<int, int> from, std::pair<int, int> to)
{
    const AbstractPiece *fromPiece = this->getPiece(from);
    const AbstractPiece *toPiece = this->getPiece(to);
    if (fromPiece == nullptr)
        return false;
    if (fromPiece->isMoveValid(from, to))
    {
        if (toPiece != nullptr)
        {
            // Cannot capture own piece
            if (fromPiece->getColor() == toPiece->getColor())
                return false;
            else
                this->capture(to);
        }
        board[from.first][from.second]->move(); // Mark the piece as moved
        this->__move(from, to);
        if (typeid(*fromPiece) == typeid(Pawn) && abs(from.second - to.second) == 2)
            this->enPassantChance(to);
        else
            isEnPassantChance = false;
        // Pawn promotion
        if (typeid(*fromPiece) == typeid(Pawn) && (to.second == 0 || to.second == 7))
            handlePromotion(to);
        return true;
    }
    else    // Special movements E.g. En passant, Castling
    {
        // En passant
        if (this->isEnPassantChance &&
            toPiece == nullptr &&
            (fromPiece->getColor() == Color::WHITE ? enPassantPosition.second + 1 : enPassantPosition.second - 1) == to.second &&
            abs(enPassantPosition.first - from.first) == 1)
        {
            // En passant
            this->capture(enPassantPosition);
            this->__move(from, to);
            isEnPassantChance = false;
            board[to.first][to.second]->move(); // Mark the piece as moved
            return true;
        }
        // Castling
        else if((typeid(*fromPiece) == typeid(King) && typeid(*toPiece) == typeid(Rook)) ||
               (typeid(*toPiece) == typeid(King) && typeid(*fromPiece) == typeid(Rook)))
        {
            const AbstractPiece *kingPiece = (typeid(*fromPiece) == typeid(King)) ? fromPiece : toPiece;
            const AbstractPiece *rookPiece = (typeid(*toPiece) == typeid(Rook)) ? toPiece : fromPiece;
            std::pair<int, int> kingPos = (typeid(*fromPiece) == typeid(King)) ? from : to;
            std::pair<int, int> rookPos = (typeid(*toPiece) == typeid(Rook)) ? to : from;
            return handleCastling(kingPiece, rookPiece, kingPos, rookPos);
        }
    }
    return false;
}

bool Board::isCheck(Color color) const
{
    std::pair<int, int> kingPosition = getKingPosition(color);
    return isUnderAttack(kingPosition, color == Color::WHITE ? Color::BLACK : Color::WHITE);
}

bool Board::isCheck(Color color, std::pair<int, int> from, std::pair<int, int> to)
{
    __move(from, to);
    bool check = isCheck(color);
    __move(to, from);
    return check;
}

bool Board::isUnderAttack(std::pair<int, int> location, Color color) const
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            const AbstractPiece *piece = board[x][y].get();
            if (piece != nullptr && piece->getColor() == color && piece->isMoveValid({x, y}, location))
                return true;
        }
    }
    return false;
}

bool Board::isCheckmate(Color color)
{
    if (!isCheck(color))
        return false;
    return isCheckmateOrStalemate(color);
}

bool Board::isStalemate(Color color)
{
    if (isCheck(color))
        return false;
    return isCheckmateOrStalemate(color);
}

void Board::clear()
{
    for (auto &row : board)
    {
        for (auto &piece : row)
            piece = nullptr;
    }
    capturedWhitePieces.clear();
    capturedBlackPieces.clear();
    initialize();
    isEnPassantChance = false;
}

bool Board::isCheckmateOrStalemate(Color color)
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            const AbstractPiece *piece = board[x][y].get();
            if (piece != nullptr && piece->getColor() == color)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if (piece->isMoveValid({x, y}, {i, j}) && !isCheck(color, {x, y}, {i, j}))
                            return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::handleCastling(const AbstractPiece *kingPiece, const AbstractPiece *rookPiece, std::pair<int, int> kingPos, std::pair<int, int> rookPos)
{
    // The king and rook must not be moved
    if (kingPiece->isMoved() || rookPiece->isMoved())
        return false;
    // Between the king and rook must be empty and not under attack
    for (int i = std::min(kingPos.first, rookPos.first) + 1; i < std::max(kingPos.first, rookPos.first); i++)
    {
        if (this->getPiece({i, kingPos.second}) != nullptr || this->isUnderAttack({i, kingPos.second}, kingPiece->getColor() == Color::WHITE ? Color::BLACK : Color::WHITE))
            return false;
    }
    if (abs(kingPos.first - rookPos.first) == 4)
    {
        board[kingPos.first][kingPos.second]->move();
        board[rookPos.first][rookPos.second]->move();
        // king from e1/e8 to c1/c8
        __move(kingPos, {kingPos.first - 2, kingPos.second});
        // rook from a1/a8 to d1/d8
        __move(rookPos, {rookPos.first + 3, rookPos.second});
        isEnPassantChance = false;

        return true;
    }
    else if (abs(kingPos.first - rookPos.first) == 3)
    {
        board[kingPos.first][kingPos.second]->move();
        board[rookPos.first][rookPos.second]->move();
        // king from e1/e8 to g1/g8
        __move(kingPos, {kingPos.first + 2, kingPos.second});
        // rook from h1/h8 to f1/f8
        __move(rookPos, {rookPos.first - 2, rookPos.second});
        isEnPassantChance = false;
        return true;
    }
    return false;
}

void Board::handlePromotion(std::pair<int, int> position)
{
    std::cout << "Pawn promotion!" << std::endl;
    std::cout << "Choose a piece to promote to (Q/R/N/B): ";
    char choice;
    std::cin >> choice;
    switch (choice)
    {
    case 'Q':
        board[position.first][position.second] = std::make_unique<Queen>(board[position.first][position.second]->getColor());
        break;
    case 'R':
        board[position.first][position.second] = std::make_unique<Rook>(board[position.first][position.second]->getColor());
        break;
    case 'N':
        board[position.first][position.second] = std::make_unique<Knight>(board[position.first][position.second]->getColor());
        break;
    case 'B':
        board[position.first][position.second] = std::make_unique<Bishop>(board[position.first][position.second]->getColor());
        break;
    default:
        std::cout << "Invalid choice! Promoting to Queen." << std::endl;
        board[position.first][position.second] = std::make_unique<Queen>(board[position.first][position.second]->getColor());
        break;
    }
}

std::pair<int, int> Board::getKingPosition(Color color) const
{
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            const AbstractPiece *piece = board[x][y].get();
            if (piece != nullptr && typeid(*piece) == typeid(King) && piece->getColor() == color)
            {
                return {x, y};
            }
        }
    }
    throw std::runtime_error("King not found!");
}
