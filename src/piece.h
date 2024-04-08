#pragma once
#include <windows.h>    // For WCHAR
#include <utility>      // For std::pair

enum class Color
{
    WHITE,
    BLACK
};

class AbstractPiece     // Abstract class
{
private:
    Color color;
    bool moved;
public:
    AbstractPiece(Color color);
    virtual WCHAR getSymbol() const = 0;
    // General move validation
    virtual bool isMoveValid(std::pair<int,int> from, std::pair<int,int> to) const = 0;
    Color getColor() const;
    bool isMoved() const;
    void move();
};

class Pawn : public AbstractPiece
{
private:
    static const WCHAR whiteSymbol = L'\u2659';
    static const WCHAR blackSymbol = L'\u265F';
public:
    Pawn(Color color);
    WCHAR getSymbol() const override;
    bool isMoveValid(std::pair<int,int> from, std::pair<int,int> to) const override;
};

class Rook : public AbstractPiece
{
private:
    static const WCHAR whiteSymbol = L'\u2656';
    static const WCHAR blackSymbol = L'\u265C';
public:
    Rook(Color color);
    WCHAR getSymbol() const override;
    bool isMoveValid(std::pair<int,int> from, std::pair<int,int> to) const override;
};

class Knight : public AbstractPiece
{
private:
    static const WCHAR whiteSymbol = L'\u2658';
    static const WCHAR blackSymbol = L'\u265E';
public:
    Knight(Color color);
    WCHAR getSymbol() const override;
    bool isMoveValid(std::pair<int,int> from, std::pair<int,int> to) const override;
};

class Bishop : public AbstractPiece
{
private:
    static const WCHAR whiteSymbol = L'\u2657';
    static const WCHAR blackSymbol = L'\u265D';
public:
    Bishop(Color color);
    WCHAR getSymbol() const override;
    bool isMoveValid(std::pair<int,int> from, std::pair<int,int> to) const override;
};

class Queen : public AbstractPiece
{
private:
    static const WCHAR whiteSymbol = L'\u2655';
    static const WCHAR blackSymbol = L'\u265B';
public:
    Queen(Color color);
    WCHAR getSymbol() const override;
    bool isMoveValid(std::pair<int,int> from, std::pair<int,int> to) const override;
};

class King : public AbstractPiece
{
private:
    static const WCHAR whiteSymbol = L'\u2654';
    static const WCHAR blackSymbol = L'\u265A';
public:
    King(Color color);
    WCHAR getSymbol() const override;
    bool isMoveValid(std::pair<int,int> from, std::pair<int,int> to) const override;
};