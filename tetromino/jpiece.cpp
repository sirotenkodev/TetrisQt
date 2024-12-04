#include "jpiece.h"

//-----------------------------------------------------------------------------
JPiece::JPiece(int x, int y) : Tetromino(x, y, 1)
{
    setArr({
            {0,0,0,0},
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0} });
}