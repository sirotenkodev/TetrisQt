#include "tetrominogenerator.h"

#include "tpiece.h"
#include "ipiece.h"
#include "jpiece.h"
#include "lpiece.h"
#include "opiece.h"
#include "spiece.h"
#include "zpiece.h"

//-----------------------------------------------------------------------------
TetrominoGenerator::TetrominoGenerator()
    : m_gen{m_rd()}
    , m_distr{0,6}
{

}

//-----------------------------------------------------------------------------
Tetromino TetrominoGenerator::generate(int x, int y)
{
    ETetromino shape = static_cast<ETetromino>(m_distr(m_gen));

    switch (shape) {
    case TetrominoGenerator::I:
        return IPiece(x, y);
    case TetrominoGenerator::J:
        return JPiece(x, y);
    case TetrominoGenerator::L:
        return LPiece(x, y);
    case TetrominoGenerator::O:
        return OPiece(x, y);
    case TetrominoGenerator::S:
        return SPiece(x, y);
    case TetrominoGenerator::Z:
        return ZPiece(x, y);
    case TetrominoGenerator::T:
        return TPiece(x, y);
    }
    return TPiece(x, y);
}
