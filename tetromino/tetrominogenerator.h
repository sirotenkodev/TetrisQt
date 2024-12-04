#ifndef TETROMINOGENERATOR_H
#define TETROMINOGENERATOR_H

#include <random>

#include "tetromino.h"

class TetrominoGenerator
{

    enum ETetromino { I, J, L, O, S, T, Z };

public:
    TetrominoGenerator();

    Tetromino generate(int x, int y);

private:
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_int_distribution<int> m_distr;
};

#endif // TETROMINOGENERATOR_H
