#ifndef TMINO_H
#define TMINO_H

#include "tetromino.h"

class TPiece final : public Tetromino
{
public:
    TPiece(int x, int y);
};

#endif // TMINO_H
