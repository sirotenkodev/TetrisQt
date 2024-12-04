#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>

class Tetromino
{
public:
    Tetromino() = default;
    Tetromino(int x, int y, int type);
    void rotate();

    int x() const;
    void setX(int newX);

    int y() const;
    void setY(int newY);

    const std::vector<std::vector<int> > &arr() const;
    void setArr(const std::vector<std::vector<int> > &newArr);

    bool isDawn() const;
    void setIsDawn(bool newIsDawn);

    int type() const;

private:
    void mulMatrix();

private:
    int m_x;
    int m_y;
    bool m_isDawn;
    std::vector<std::vector<int>> m_arr;
    int m_type;
};

#endif // TETROMINO_H
