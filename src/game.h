#pragma once
#include <utility>
#include <string>
#include "piece.h"

class Game
{
private:
    Color turn;
    static std::pair<int, int> decodeInput(std::string input);
public:
    Game();
    void start();
    void test();
};