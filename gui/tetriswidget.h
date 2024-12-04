#ifndef TETRISWIDGET_H
#define TETRISWIDGET_H

#include <memory>
#include <vector>
#include <chrono>

#include <QWidget>
#include <QBasicTimer>
#include <QKeyEvent>

#include "../stats.h"

#include "../tetromino/tetromino.h"
#include "../tetromino/tetrominogenerator.h"

class TetrisWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TetrisWidget(QWidget *parent = nullptr);
    int getPlayerPoints();
    void stopTimer();
    void startTimer();
    bool isGameOnPause() const;
    const Tetromino &nextTetromino() const;

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    void spawnTetromino();
    void placeTetromino();
    void clearLines();
    bool canMove(int dx, int dy);

signals:
    void gameOver();
    void linesChanged(int);
    void scoreChanged(int);
    void pieceChanged();

private:
    QBasicTimer m_timer;
    int m_width;
    int m_height;
    int m_blockSize;
    int m_lines;
    double m_speed;
    bool   m_initialized;
    bool m_isGameOnPause;
    QImage m_tileImage;
    QImage m_backgroundImage;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    std::vector<std::vector<int>> m_board;
    TetrominoGenerator m_tetrominoGenerator;
    Tetromino m_currentTetromino;
    Tetromino m_nextTetromino;
    Stats m_stats;
};

#endif // TETRISWIDGET_H
