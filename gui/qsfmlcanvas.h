#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <memory>
#include <vector>
#include <chrono>

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "../stats.h"

#include "../tetromino/tetromino.h"
#include "../tetromino/tetrominogenerator.h"

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
    Q_OBJECT

public:
    enum ViewType { OLD, NEW };
public:
    explicit QSFMLCanvas(QWidget *parent = nullptr, ViewType type = ViewType::NEW, uint frameTime = 50);
    virtual ~QSFMLCanvas() {}
    void setVolume(float game, float move);
    int getPlayerPoints();
    void stopTimer();
    void startTimer();    
    bool isGameOnPause() const;

    const Tetromino &nextTetromino() const;

private:
    void onInit();
    void renderUI();
    void drawBoard();
    void drawBorder();
    void drawTetromino();
    void spawnTetromino();
    void placeTetromino();
    void clearLines();
    bool canMove(int dx, int dy);

private :
    virtual QPaintEngine* paintEngine() const override;
    virtual void showEvent(QShowEvent*) override;
    virtual void paintEvent(QPaintEvent*) override;

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void onTimeout();

signals:
    void gameOver();
    void linesChanged(int);
    void scoreChanged(int);
    void pieceChanged();

private:
    QTimer m_timer;
    int m_width;
    int m_height;
    int m_blockSize;
    int m_lines;
    double m_speed;
    bool   m_initialized;
    bool m_isGameOnPause;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::vector<std::vector<int>> board;
    TetrominoGenerator m_tetrominoGenerator;
    Tetromino m_currentTetromino;
    Tetromino m_nextTetromino;

    sf::Texture m_texture;
    sf::Texture m_textureBack;
    sf::Texture m_textureFrame;
    sf::Sprite m_sprite;
    sf::Sprite m_spriteBack;
    sf::Sprite m_spriteFrame;
    Stats m_stats;

    ViewType m_type;
    sf::Font m_fontMines;
    sf::Text m_mines;
    sf::Text m_clears;
    sf::Text m_borderLeft;
    sf::Text m_borderRight;
    sf::Text m_borderDown;

    sf::Music m_music;
    sf::SoundBuffer m_turnBuffer;
    sf::Sound m_turnSound;
    sf::SoundBuffer m_turnOutLine;
    sf::Sound m_outlineSound;
};

#endif // QSFMLCANVAS_H
