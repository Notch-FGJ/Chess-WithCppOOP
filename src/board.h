#pragma once
#include <memory> // For std::unique_ptr
#include <utility>
#include <vector>
#include "piece.h"
// Forward declaration
class AbstractPiece;
class Pawn;
class Rook;
class Knight;
class Bishop;
class Queen;
class King;

class Board // Singleton class
{
private:
    std::vector<std::vector<std::unique_ptr<AbstractPiece>>> board;
    std::vector<std::unique_ptr<AbstractPiece>> capturedWhitePieces;
    std::vector<std::unique_ptr<AbstractPiece>> capturedBlackPieces;
    void initialize();
    Board();

    Board(const Board &) = delete;
    Board &operator=(const Board &) = delete;

    bool isEnPassantChance;
    std::pair<int, int> enPassantPosition; // Position of the pawn that can be captured en passant

    // Private move function with no checks
    void __move(std::pair<int, int> from, std::pair<int, int> to);
    bool isCheckmateOrStalemate(Color color);   // Returns true if the given color is in checkmate or stalemate
    bool handleCastling(const AbstractPiece *kingPiece, const AbstractPiece *rookPiece, std::pair<int, int> kingPos, std::pair<int, int> rookPos);
    void handlePromotion(std::pair<int, int> position);
    std::pair<int, int> getKingPosition(Color color) const;

public:
    ~Board() = default;
    static std::shared_ptr<Board> getInstance();
    void print() const;
    const AbstractPiece *getPiece(std::pair<int, int> position) const;
    void capture(std::pair<int, int> position);
    void enPassantChance(std::pair<int, int> position);
    bool move(std::pair<int, int> from, std::pair<int, int> to);
    // Check if the king of the given color is in check
    bool isCheck(Color color) const;
    // Check if the king of the given color is in check after the move
    bool isCheck(Color color, std::pair<int, int> from, std::pair<int, int> to);
    // The Color given is the color of the piece that will be checked
    bool isUnderAttack(std::pair<int, int> location, Color color) const;


    bool isCheckmate(Color color);  // Returns true if the given color is in checkmate
    bool isStalemate(Color color);  // Returns true if the given color is in stalemate

    void clear();
};