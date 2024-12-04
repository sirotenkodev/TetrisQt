#include "qsfmlcanvas.h"

#include <QEventTransition>
#include <QPropertyAnimation>
#include <QStateMachine>

//-----------------------------------------------------------------------------
QSFMLCanvas::QSFMLCanvas(QWidget *parent, ViewType type, uint frameTime)
    : QWidget(parent)
    , sf::RenderWindow(sf::VideoMode(10 * 18, 20 * 18), "Tetris", sf::Style::Default, sf::ContextSettings(24))
    , m_width{10}
    , m_height{20}
    , m_blockSize{18}
    , m_lines{0}
    , m_speed{0.7}
    , m_initialized{false}
    , m_isGameOnPause{false}
    , start{ std::chrono::high_resolution_clock::now() }
    , m_currentTetromino(m_tetrominoGenerator.generate(4, 0))
    , m_nextTetromino(m_tetrominoGenerator.generate(4, 0))
    , m_type{type}
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setFocusPolicy(Qt::StrongFocus);

    setKeyRepeatEnabled(true);

    m_timer.setInterval(frameTime);

    if (!m_turnBuffer.loadFromFile("images/turn.wav")) {
            return;
    }

    m_turnSound.setBuffer(m_turnBuffer);

    if (!m_turnOutLine.loadFromFile("images/lineout.wav")) {
            return;
    }
    m_outlineSound.setBuffer(m_turnOutLine);


    m_music.openFromFile("images/ding.flac");
    m_music.play();
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::onInit()
{
    board.resize(m_height, std::vector<int>(m_width, 0));

    if(m_type == ViewType::NEW) {
        m_texture.loadFromFile("images/tiles.png");
        m_sprite.setTexture(m_texture);

        m_textureFrame.loadFromFile("images/frame.png");
        m_spriteFrame.setTexture(m_textureFrame);
        m_spriteFrame.setColor(sf::Color::Green);

        m_textureBack.loadFromFile("images/tilesback.png");
        m_spriteBack.setTexture(m_textureBack);
        m_spriteBack.setColor(sf::Color(128, 255, 128, 50));
    } else {
        m_fontMines.loadFromFile("images/lucon.ttf");
        m_mines.setFont(m_fontMines);
        m_mines.setString("[]");
        m_mines.setCharacterSize(18);
        m_mines.setColor(sf::Color::Green);

        m_clears.setFont(m_fontMines);
        m_clears.setString(" .");
        m_clears.setCharacterSize(18);
        m_clears.setColor(sf::Color::Green);

        m_borderLeft.setFont(m_fontMines);
        m_borderLeft.setString("<!");
        m_borderLeft.setCharacterSize(18);
        m_borderLeft.setColor(sf::Color::Green);

        m_borderRight.setFont(m_fontMines);
        m_borderRight.setString("!>");
        m_borderRight.setCharacterSize(18);
        m_borderRight.setColor(sf::Color::Green);

        m_borderDown.setFont(m_fontMines);
        m_borderDown.setString("=");
        m_borderDown.setCharacterSize(18);
        m_borderDown.setColor(sf::Color::Green);
    }
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::renderUI()
{
    if(m_type == ViewType::NEW) {
        clear(sf::Color(240,240,240));
    } else {
        clear(sf::Color::Transparent);
    }

    drawBoard();
    drawTetromino();
    if(m_type == ViewType::NEW) {
        draw(m_spriteFrame);
    } else {
        drawBorder();
    }
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::drawBoard()
{
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (board[y][x] != 0) {
                if(m_type == ViewType::NEW) {
                    m_sprite.setTextureRect(sf::IntRect(0, 0, 18, 18));
                    m_sprite.setPosition(x * m_blockSize, y * m_blockSize);
                    m_sprite.move(28,31);
                    draw(m_sprite);
                } else {
                    m_mines.setPosition(x * m_blockSize, y * m_blockSize);
                    m_mines.move(28,31);
                    draw(m_mines);
                }
            } else {
                if(m_type == ViewType::NEW) {
                    m_spriteBack.setTextureRect(sf::IntRect(0, 0, 18, 18));
                    m_spriteBack.setPosition(x * m_blockSize, y * m_blockSize);
                    m_spriteBack.move(28,31);
                    draw(m_spriteBack);
                } else {
                    m_clears.setPosition(x * m_blockSize, y * m_blockSize);
                    m_clears.move(28,31);
                    draw(m_clears);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::drawBorder()
{
    for (int y = 0; y < m_height + 1; ++y) {
        for (int x = 0; x < m_width + 2; ++x) {
            if (x == 0) {
                m_borderLeft.setPosition(x * m_blockSize, y * m_blockSize);
                m_borderLeft.move(10,31);
                draw(m_borderLeft);
            } else if(x == 11) {
                m_borderRight.setPosition(x * m_blockSize, y * m_blockSize);
                m_borderRight.move(10,31);
                draw(m_borderRight);
            }
            if(y == 20 && x > 0 && x < 11) {
                m_borderDown.setPosition(x * m_blockSize, y * m_blockSize);
                m_borderDown.move(10,31);
                draw(m_borderDown);
            }
        }
    }
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::drawTetromino()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_currentTetromino.arr()[i][j] != 0) {
                if(m_type == ViewType::NEW) {
                    m_sprite.setTextureRect(sf::IntRect(m_currentTetromino.type() * m_blockSize, 0, m_blockSize, m_blockSize));
                    m_sprite.setPosition((m_currentTetromino.x() + j) * m_blockSize, (m_currentTetromino.y() + i) * m_blockSize);
                    m_sprite.move(28,31);
                    draw(m_sprite);
                } else {
                    m_mines.setPosition((m_currentTetromino.x() + j) * m_blockSize, (m_currentTetromino.y() + i) * m_blockSize);
                    m_mines.move(28,31);
                    draw(m_mines);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::spawnTetromino()
{
    m_currentTetromino = m_nextTetromino;
    m_nextTetromino = m_tetrominoGenerator.generate(4, 0);
    emit pieceChanged();//TODO
    if (!canMove(0, 0)) {
        sf::RenderWindow::clear();
        emit linesChanged(0);
        emit scoreChanged(0);
        emit gameOver();
    }
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::placeTetromino()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_currentTetromino.arr()[i][j] != 0) {
                board[m_currentTetromino.y() + i][m_currentTetromino.x() + j] = 1;
            }
        }
    }
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::clearLines()
{
    for (int y = m_height - 1; y >= 0; --y) {
        bool fullLine = true;
        for (int x = 0; x < m_width; ++x) {
            if (board[y][x] == 0) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            board.erase(board.begin() + y);
            board.insert(board.begin(), std::vector<int>(m_width, 0));
            y++;
            m_stats.addLines(1);
            m_stats.addScore(100);
            emit linesChanged(m_stats.lines());
            emit scoreChanged(m_stats.score());
            m_outlineSound.play();
        }
    }
}

//-----------------------------------------------------------------------------
bool QSFMLCanvas::canMove(int dx, int dy)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_currentTetromino.arr()[i][j] != 0) {
                int newX = m_currentTetromino.x() + j + dx;
                int newY = m_currentTetromino.y() + i + dy;
                if (newX < 0 || newX >= m_width || newY < 0 || newY >= m_height || board[newY][newX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::setVolume(float game, float move)
{
    m_turnSound.setVolume(move);
    m_outlineSound.setVolume(game);
    m_music.setVolume(game);
}

//-----------------------------------------------------------------------------
int QSFMLCanvas::getPlayerPoints()
{
    return m_stats.score();
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::stopTimer()
{
    m_timer.stop();
    m_isGameOnPause = true;
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::startTimer()
{
    m_timer.start();
    m_isGameOnPause = false;
}

//-----------------------------------------------------------------------------
QPaintEngine *QSFMLCanvas::paintEngine() const
{
    return nullptr;
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::showEvent(QShowEvent *)
{
    if(!m_initialized) {

        RenderWindow::create(sf::WindowHandle(winId()));

        onInit();

        connect(&m_timer, &QTimer::timeout, this, &QSFMLCanvas::onTimeout);
        m_timer.start();

        m_initialized = true;
    }
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::paintEvent(QPaintEvent *)
{
    renderUI();
    display();
}

//-----------------------------------------------------------------------------
void QSFMLCanvas::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        sf::RenderWindow::clear();
        emit linesChanged(0);
        emit scoreChanged(0);
        emit gameOver();
        break;
    case Qt::Key_Left:
        m_turnSound.play();
        if(canMove(-1, 0)) {
            m_currentTetromino.setX(m_currentTetromino.x() - 1);
        }
        break;
    case Qt::Key_Right:
        m_turnSound.play();
        if(canMove(1, 0)) {
            m_currentTetromino.setX(m_currentTetromino.x() + 1);
        }
        break;
    case Qt::Key_Down:
        m_turnSound.play();
        if(canMove(0, 1)) {
            m_stats.addScore(10);
            emit scoreChanged(m_stats.score());
            m_currentTetromino.setY(m_currentTetromino.y() + 1);
        } else {
            m_currentTetromino.setIsDawn(true);
        }
        break;
    case Qt::Key_Up: {
        m_turnSound.play();
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
        m_turnSound.play();
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
void QSFMLCanvas::onTimeout()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - start;

    if(m_stats.lines() - m_lines >= 30  && m_speed != 0.1) {
        m_lines = m_stats.lines();
        m_speed -= 0.1;
    }

    if (canMove(0, 1)) {
        if(elapsed.count() >= m_speed) {
            start = now;
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
    }
}

//-----------------------------------------------------------------------------
const Tetromino &QSFMLCanvas::nextTetromino() const
{
    return m_nextTetromino;
}

//-----------------------------------------------------------------------------
bool QSFMLCanvas::isGameOnPause() const
{
    return m_isGameOnPause;
}
