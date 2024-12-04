#include "tetromino.h"

//-----------------------------------------------------------------------------
Tetromino::Tetromino(int x, int y, int type)
    : m_x{x}
    , m_y{y}
    , m_isDawn{false}
    , m_type{type}
{}

//-----------------------------------------------------------------------------
void Tetromino::rotate()
{
    std::vector<std::vector<int>> arr(4, std::vector<int>(4));
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            arr[j][i] = m_arr[i][j];
        }
    }

    std::swap(arr, m_arr);
    mulMatrix();
}

//-----------------------------------------------------------------------------
int Tetromino::x() const
{
    return m_x;
}

//-----------------------------------------------------------------------------
void Tetromino::setX(int newX)
{
    m_x = newX;
}

//-----------------------------------------------------------------------------
int Tetromino::y() const
{
    return m_y;
}

//-----------------------------------------------------------------------------
void Tetromino::setY(int newY)
{
    m_y = newY;
}

//-----------------------------------------------------------------------------
const std::vector<std::vector<int> > &Tetromino::arr() const
{
    return m_arr;
}

//-----------------------------------------------------------------------------
void Tetromino::setArr(const std::vector<std::vector<int> > &newArr)
{
    m_arr = newArr;
}

//-----------------------------------------------------------------------------
void Tetromino::mulMatrix()
{
    int mul[4][4] = {
        {0,0,0,1},
        {0,0,1,0},
        {0,1,0,0},
        {1,0,0,0}
    };

    std::vector<std::vector<int>> arr(4, std::vector<int>(4));
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            int sum = 0;
            for(int m = 0; m < 4; ++m) {
                sum += m_arr[i][m] * mul[m][j];
            }
            arr[i][j] = sum;
        }
    }
    std::swap(arr, m_arr);
}

int Tetromino::type() const
{
    return m_type;
}

bool Tetromino::isDawn() const
{
    return m_isDawn;
}

void Tetromino::setIsDawn(bool newIsDawn)
{
    m_isDawn = newIsDawn;
}
