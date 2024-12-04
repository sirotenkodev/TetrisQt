#include <QPainter>
#include <QPixmap>
#include <tuple>

#include "menuform.h"
#include "../dialogs/saveleader.h"
#include "../utils/scoreboardutils.h"
#include "ui_menuform.h"
#include "../utils/settings.h"

//-----------------------------------------------------------------------------
MenuForm::MenuForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuForm)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->quitButton, &QPushButton::clicked, this, &QWidget::close);
}

//-----------------------------------------------------------------------------
MenuForm::~MenuForm()
{
    m_scoreUtils.save();
    delete ui;
}

//-----------------------------------------------------------------------------
void MenuForm::showNextPiece()
{
    int type = Settings::instance().getVisualType();

    Tetromino print{};

    if(type != 2) {
        print = m_sfmlWidget->nextTetromino();
    } else {
        print = m_tetrisWidget->nextTetromino();
    }

    QPixmap pixmap(4 * 18, 4 * 18);
    QPainter painter(&pixmap);

    painter.fillRect(pixmap.rect(), QColor(240,240,240));

    QColor color;
    switch(print.type()) {
    case 0:
        color.setRgb(51,122,233);
        break;
    case 1:
        color.setRgb(145,108,206);
        break;
    case 2:
        color.setRgb(224,48,32);
        break;
    case 3:
        color.setRgb(106,194,48);
        break;
    case 4:
        color.setRgb(246,204,39);
        break;
    case 5:
        color.setRgb(56,195,232);
        break;
    case 6:
        color.setRgb(238,128,39);
        break;
    default:
        color.setRgb(51,122,233);
        break;
    }

    color.setAlpha(110);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if(print.arr()[i][j] == 1) {
                if(type == 0) {
                    painter.drawText(QRectF{static_cast<qreal>(i * 15), static_cast<qreal>(j * 15), 15, 15}, "[ ]");
                } else {
                    painter.fillRect(QRectF{static_cast<qreal>(i * 15), static_cast<qreal>(j * 15), 15, 15}, color);
                    painter.drawRect(i * 15, j * 15, 15, 15);
                }
            }
        }
    }
    ui->nextPiece->setPixmap(pixmap);
}

//-----------------------------------------------------------------------------
void MenuForm::on_playButton_clicked()
{
    int type = Settings::instance().getVisualType();
    if(type != 2) {
        m_sfmlWidget = std::make_unique<QSFMLCanvas>(ui->gameWidget, static_cast<QSFMLCanvas::ViewType>(Settings::instance().getVisualType()));
        m_sfmlWidget->setVolume(Settings::instance().getGameVolume(), Settings::instance().getMoveVolume());
        connect(m_sfmlWidget.get(), &QSFMLCanvas::gameOver, this, &MenuForm::gameOver);
        connect(m_sfmlWidget.get(), &QSFMLCanvas::scoreChanged, ui->lcdScore, qOverload<int>(&QLCDNumber::display));
        connect(m_sfmlWidget.get(), &QSFMLCanvas::linesChanged, ui->lcdLines, qOverload<int>(&QLCDNumber::display));
        connect(m_sfmlWidget.get(), &QSFMLCanvas::pieceChanged, this, &MenuForm::showNextPiece);
        ui->gameGrid->addWidget(m_sfmlWidget.get(), 0, 0);
        setFocusProxy(m_sfmlWidget.get());
    } else {
        m_tetrisWidget = std::make_unique<TetrisWidget>();
        connect(m_tetrisWidget.get(), &TetrisWidget::gameOver, this, &MenuForm::gameOver);
        connect(m_tetrisWidget.get(), &TetrisWidget::scoreChanged, ui->lcdScore, qOverload<int>(&QLCDNumber::display));
        connect(m_tetrisWidget.get(), &TetrisWidget::linesChanged, ui->lcdLines, qOverload<int>(&QLCDNumber::display));
        connect(m_tetrisWidget.get(), &TetrisWidget::pieceChanged, this, &MenuForm::showNextPiece);
        ui->gameGrid->addWidget(m_tetrisWidget.get(), 0, 0);
        setFocusProxy(m_tetrisWidget.get());
    }

    ui->stackedWidget->setCurrentIndex(1);
    showNextPiece();
}

//-----------------------------------------------------------------------------
void MenuForm::gameOver()
{
    int type = Settings::instance().getVisualType();
    if(type != 2) {
        m_sfmlWidget->stopTimer();
        auto dlg = std::make_unique<SaveLeader>(this);
        if(dlg->exec() == QDialog::Accepted) {
            std::pair<std::string, int> score = {dlg->getName(), m_sfmlWidget->getPlayerPoints()};
            m_scoreUtils.addScore(score);
        }
        disconnect(m_sfmlWidget.get(), &QSFMLCanvas::gameOver, this, &MenuForm::gameOver);
        disconnect(m_sfmlWidget.get(), &QSFMLCanvas::scoreChanged, ui->lcdScore, qOverload<int>(&QLCDNumber::display));
        disconnect(m_sfmlWidget.get(), &QSFMLCanvas::linesChanged, ui->lcdLines, qOverload<int>(&QLCDNumber::display));
        disconnect(m_sfmlWidget.get(), &QSFMLCanvas::pieceChanged, this, &MenuForm::showNextPiece);
        ui->stackedWidget->setCurrentIndex(0);
        m_sfmlWidget.reset();
    } else {
        m_tetrisWidget->stopTimer();
        auto dlg = std::make_unique<SaveLeader>(this);
        if(dlg->exec() == QDialog::Accepted) {
            std::pair<std::string, int> score = {dlg->getName(), m_sfmlWidget->getPlayerPoints()};
            m_scoreUtils.addScore(score);
        }
        disconnect(m_tetrisWidget.get(), &TetrisWidget::gameOver, this, &MenuForm::gameOver);
        disconnect(m_tetrisWidget.get(), &TetrisWidget::scoreChanged, ui->lcdScore, qOverload<int>(&QLCDNumber::display));
        disconnect(m_tetrisWidget.get(), &TetrisWidget::linesChanged, ui->lcdLines, qOverload<int>(&QLCDNumber::display));
        disconnect(m_tetrisWidget.get(), &TetrisWidget::pieceChanged, this, &MenuForm::showNextPiece);
        ui->stackedWidget->setCurrentIndex(0);
        m_tetrisWidget.reset();
    }
}

//-----------------------------------------------------------------------------
void MenuForm::on_moveSoundSlider_valueChanged(int value)
{
    Settings::instance().setMoveVolume(value);
}

//-----------------------------------------------------------------------------
void MenuForm::on_gameSoundSlider_valueChanged(int value)
{
    Settings::instance().setGameVolume(value);
}

//-----------------------------------------------------------------------------
void MenuForm::on_optionsButton_clicked()
{
    ui->gameSoundSlider->setValue(Settings::instance().getGameVolume());
    ui->moveSoundSlider->setValue(Settings::instance().getMoveVolume());
    ui->comboBox->setCurrentIndex(Settings::instance().getVisualType());
    ui->stackedWidget->setCurrentIndex(2);
}

//-----------------------------------------------------------------------------
void MenuForm::on_comboBox_currentIndexChanged(int index)
{
    Settings::instance().setVisualType(index);
}

//-----------------------------------------------------------------------------
void MenuForm::on_playPauseButton_clicked()
{
    int type = Settings::instance().getVisualType();
    if(type != 2) {
        if(m_sfmlWidget->isGameOnPause()) {
            m_sfmlWidget->startTimer();
        } else {
            m_sfmlWidget->stopTimer();
        }
    } else {
        if(m_tetrisWidget->isGameOnPause()) {
            m_tetrisWidget->startTimer();
        } else {
            m_tetrisWidget->stopTimer();
        }
    }

}

//-----------------------------------------------------------------------------
void MenuForm::on_leadersButton_clicked()
{
    ui->listWidget->clear();
    for(int i = 0; i < m_scoreUtils.size(); ++i) {
       ui->listWidget->addItem(QString::number(i + 1) + ": \t" + QString::fromStdString(m_scoreUtils.valueByIndex(i)));
    }
    ui->stackedWidget->setCurrentIndex(3);
}

//-----------------------------------------------------------------------------
void MenuForm::on_endLeaderButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//-----------------------------------------------------------------------------
void MenuForm::on_endSettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//-----------------------------------------------------------------------------
void MenuForm::on_endGameButton_clicked()
{
    int type = Settings::instance().getVisualType();
    if(type != 2) {
        m_sfmlWidget->stopTimer();
        auto dlg = std::make_unique<SaveLeader>(this);
        if(dlg->exec() == QDialog::Accepted) {
            std::pair<std::string, int> score = {dlg->getName(), m_sfmlWidget->getPlayerPoints()};
            m_scoreUtils.addScore(score);
        }
        disconnect(m_sfmlWidget.get(), &QSFMLCanvas::gameOver, this, &MenuForm::gameOver);
        disconnect(m_sfmlWidget.get(), &QSFMLCanvas::scoreChanged, ui->lcdScore, qOverload<int>(&QLCDNumber::display));
        disconnect(m_sfmlWidget.get(), &QSFMLCanvas::linesChanged, ui->lcdLines, qOverload<int>(&QLCDNumber::display));
        disconnect(m_sfmlWidget.get(), &QSFMLCanvas::pieceChanged, this, &MenuForm::showNextPiece);
        ui->stackedWidget->setCurrentIndex(0);
        m_sfmlWidget.reset();
    } else {
        m_tetrisWidget->stopTimer();
        auto dlg = std::make_unique<SaveLeader>(this);
        if(dlg->exec() == QDialog::Accepted) {
            std::pair<std::string, int> score = {dlg->getName(), m_sfmlWidget->getPlayerPoints()};
            m_scoreUtils.addScore(score);
        }
        disconnect(m_tetrisWidget.get(), &TetrisWidget::gameOver, this, &MenuForm::gameOver);
        disconnect(m_tetrisWidget.get(), &TetrisWidget::scoreChanged, ui->lcdScore, qOverload<int>(&QLCDNumber::display));
        disconnect(m_tetrisWidget.get(), &TetrisWidget::linesChanged, ui->lcdLines, qOverload<int>(&QLCDNumber::display));
        disconnect(m_tetrisWidget.get(), &TetrisWidget::pieceChanged, this, &MenuForm::showNextPiece);
        ui->stackedWidget->setCurrentIndex(0);
        m_tetrisWidget.reset();
    }
}

