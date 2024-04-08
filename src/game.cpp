#include "game.h"
#include "board.h"
#include <iostream>


std::pair<int, int> Game::decodeInput(std::string input)
{
    return {input[0] - 'a', input[1] - '1'};
}

Game::Game()
{
    turn = Color::WHITE;
}

void Game::start()
{
    while (true)
    {
        Board::getInstance()->print();
        std::string turnColor = (turn == Color::WHITE) ? "White" : "Black";
        std::cout << turnColor << "'s turn: ";
        std::string input;
        std::cin >> input;
        std::pair<int, int> from = decodeInput(input);
        std::cin >> input;
        std::pair<int, int> to = decodeInput(input);
        // Check if the input is valid(Is there a piece at the from position 
        // Is the piece the same color as the turn)
        if (Board::getInstance()->getPiece(from) == nullptr)
        {
            std::cout << "No piece at the given position!" << std::endl;
            system("pause");
            continue;
        }
        else if (Board::getInstance()->getPiece(from)->getColor() != turn)
        {
            std::cout << "It's not " << turnColor << "'s turn!" << std::endl;
            system("pause");
            continue;
        }
        else if (Board::getInstance()->move(from, to))
        {
            if (Board::getInstance()->isCheckmate(turn == Color::WHITE ? Color::BLACK : Color::WHITE))
            {
                Board::getInstance()->print();
                std::cout << turnColor << " wins!" << std::endl;
                system("pause");
                Board::getInstance()->clear();
                turn = Color::WHITE;
                continue;
            }
            if (Board::getInstance()->isStalemate(turn == Color::WHITE ? Color::BLACK : Color::WHITE))
            {
                Board::getInstance()->print();
                std::cout << "Stalemate!" << std::endl;
                system("pause");
                Board::getInstance()->clear();
                turn = Color::WHITE;
            }
            turn = (turn == Color::WHITE) ? Color::BLACK : Color::WHITE;
        }
        else
        {
            std::cout << "Invalid move!" << std::endl;
            system("pause");
        }
    }
}

void Game::test()
{
/*
En Passant test
b2 b4
g8 h6
b4 b5
c7 c5

b5 c6
*/
    Board::getInstance()->print();
    Board::getInstance()->move({1, 1}, {1, 3});
    Board::getInstance()->move({6, 7}, {7, 5});
    Board::getInstance()->move({1, 3}, {1, 4});
    Board::getInstance()->move({2, 6}, {2, 4});
    Board::getInstance()->print();
    Board::getInstance()->move({1, 4}, {2, 5});
    Board::getInstance()->print();
    system("pause");
}
