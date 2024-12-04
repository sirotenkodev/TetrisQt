#include "tetriswidget.h"

#include <QPainter>

//-----------------------------------------------------------------------------
TetrisWidget::TetrisWidget(QWidget *parent)
    : QWidget{parent}
    , m_width{10}
    , m_height{20}
    , m_blockSize{18}
    , m_lines{0}
    , m_speed{0.7}
    , m_initialized{false}
    , m_isGameOnPause{false}
    , m_tileImage {"images/tiles.png"}
    , m_backgroundImage {"images/tilesback.png"}
    , m_start{ std::chrono::high_resolution_clock::now() }
    , m_currentTetromino(m_tetrominoGenerator.generate(4, 0))
    , m_nextTetromino(m_tetrominoGenerator.generate(4, 0))
{
    m_board.resize(m_height, std::vector<int>(m_width, 0));
    setFocusPolicy(Qt::StrongFocus);
    m_timer.start(50, this);
}

//-----------------------------------------------------------------------------
int TetrisWidget::getPlayerPoints()
{
    return m_stats.score();
}

//-----------------------------------------------------------------------------
void TetrisWidget::stopTimer()
{
    m_timer.stop();
    m_isGameOnPause = true;
}

//-----------------------------------------------------------------------------
void TetrisWidget::startTimer()
{
    m_timer.start(50, this);
    m_isGameOnPause = false;
}

//-----------------------------------------------------------------------------
bool TetrisWidget::isGameOnPause() const
{
    return m_isGameOnPause;
}

//-----------------------------------------------------------------------------
const Tetromino &TetrisWidget::nextTetromino() const
{
    return m_nextTetromino;
}

//-----------------------------------------------------------------------------
void TetrisWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QRectF sourcePiece(m_blockSize * m_currentTetromino.type(), 0, m_blockSize, m_blockSize);
    QRectF sourceBackground(0, 0, m_blockSize, m_blockSize);
    QRect rect = contentsRect();

    QPainter painter(this);

    if (m_isGameOnPause) {
        painter.drawText(rect, Qt::AlignCenter, tr("Pause"));
        return;
    }

    painter.scale(1.3, 1.3);
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if(m_board[i][j] == 1) {
                painter.setOpacity(1.0);
                painter.drawImage(QPointF{static_cast<qreal>(m_blockSize * j), static_cast<qreal>(m_blockSize * i)}, m_backgroundImage, sourceBackground);
            } else {
                painter.setOpacity(0.2);
                painter.drawImage(QPointF{static_cast<qreal>(m_blockSize * j), static_cast<qreal>(m_blockSize * i)}, m_backgroundImage, sourceBackground);
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_currentTetromino.arr()[i][j] != 0) {
                painter.setOpacity(1.0);
                painter.drawImage(QPointF{static_cast<qreal>((m_currentTetromino.x() + j) * m_blockSize),
                                          static_cast<qreal>((m_currentTetromino.y() + i) * m_blockSize)},
                                          m_tileImage, sourcePiece);
            }
        }
    }
}

//-----------------------------------------------------------------------------
void TetrisWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        emit linesChanged(0);
        emit scoreChanged(0);
        emit gameOver();
        break;
    case Qt::Key_Left:
        if(canMove(-1, 0)) {
            m_currentTetromino.setX(m_currentTetromino.x() - 1);
        }
        break;
    case Qt::Key_Right:
        if(canMove(1, 0)) {
            m_currentTetromino.setX(m_currentTetromino.x() + 1);
        }
        break;
    case Qt::Key_Down:
        if(canMove(0, 1)) {
            m_stats.addScore(10);
            emit scoreChanged(m_stats.score());
            m_currentTetromino.setY(m_currentTetromino.y() + 1);
        } else {
            m_currentTetromino.setIsDawn(true);
        }
        break;
    case Qt::Key_Up: {
        std::vector<std::vector<int>> tempShape(4, std::vector<int>(4));
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                tempShape[i][j] = m_currentTetromino.arr()[i][j];
            }
        }
        m_currentTetromino.rotate();

        if (!canMove(0, 0)) {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                   m_currentTetromino.setArr(tempShape);
                }
            }
        }
        break;
    }
    case Qt::Key_Space: {
        while (canMove(0, 1)) {
            m_currentTetromino.setY(m_currentTetromino.y() + 1);
        }
        if(!m_currentTetromino.isDawn()) {
            m_stats.addScore(100);
            emit scoreChanged(m_stats.score());
            m_currentTetromino.setIsDawn(true);
        }
        break;
    }
    default:
        QWidget::keyPressEvent(event);
    }
}

//-----------------------------------------------------------------------------
void TetrisWidget::timerEvent(QTimerEvent *event)
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - m_start;

    if(m_stats.lines() - m_lines >= 30  && m_speed != 0.1) {
        m_lines = m_stats.lines();
        m_speed -= 0.1;
    }

    if (canMove(0, 1)) {
        if(elapsed.count() >= m_speed) {
            m_start = now;
            m_currentTetromino.setY(m_currentTetromino.y() + 1);
        }
    } else {
        if(elapsed.count() >= m_speed) {
            placeTetromino();
            clearLines();
            spawnTetromino();
        }

    }
    if(!m_isGameOnPause) {
        repaint();
        update();
    }
}

//-----------------------------------------------------------------------------
void TetrisWidget::spawnTetromino()
{
    m_currentTetromino = m_nextTetromino;
    m_nextTetromino = m_tetrominoGenerator.generate(4, 0);
    emit pieceChanged();//TODO
    if (!canMove(0, 0)) {
        emit linesChanged(0);
        emit scoreChanged(0);
        emit gameOver();
    }
}

void TetrisWidget::placeTetromino()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_currentTetromino.arr()[i][j] != 0) {
                m_board[m_currentTetromino.y() + i][m_currentTetromino.x() + j] = 1;
            }
        }
    }
}

void TetrisWidget::clearLines()
{
    for (int y = m_height - 1; y >= 0; --y) {
        bool fullLine = true;
        for (int x = 0; x < m_width; ++x) {
            if (m_board[y][x] == 0) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            m_board.erase(m_board.begin() + y);
            m_board.insert(m_board.begin(), std::vector<int>(m_width, 0));
            y++;
            m_stats.addLines(1);
            m_stats.addScore(100);
            emit linesChanged(m_stats.lines());
            emit scoreChanged(m_stats.score());
        }
    }
}

bool TetrisWidget::canMove(int dx, int dy)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_currentTetromino.arr()[i][j] != 0) {
                int newX = m_currentTetromino.x() + j + dx;
                int newY = m_currentTetromino.y() + i + dy;
                if (newX < 0 || newX >= m_width || newY < 0 || newY >= m_height || m_board[newY][newX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}
