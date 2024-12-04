#ifndef OMINO_H
#define OMINO_H

#include "tetromino.h"

class OPiece final : public Tetromino
{
public:
    OPiece(int x, int y);
};

#endif // OMINO_H
