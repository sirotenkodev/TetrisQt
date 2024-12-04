#ifndef MENUFORM_H
#define MENUFORM_H

#include <memory>

#include <QWidget>
#include <QGridLayout>

#include "qsfmlcanvas.h"
#include "tetriswidget.h"
#include "../utils/scoreboardutils.h"

namespace Ui {
class MenuForm;
}

class MenuForm : public QWidget
{
    Q_OBJECT

public:
    explicit MenuForm(QWidget *parent = nullptr);
    ~MenuForm();



private slots:
    void on_playButton_clicked();
    void gameOver();
    void on_moveSoundSlider_valueChanged(int value);
    void on_gameSoundSlider_valueChanged(int value);
    void on_optionsButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_playPauseButton_clicked();
    void on_leadersButton_clicked();
    void on_endLeaderButton_clicked();
    void on_endSettingsButton_clicked();
    void showNextPiece();

    void on_endGameButton_clicked();

private:
    Ui::MenuForm *ui;
    std::unique_ptr<QSFMLCanvas> m_sfmlWidget;
    std::unique_ptr<TetrisWidget> m_tetrisWidget;

    ScoreBoardUtils m_scoreUtils;
};

#endif // MENUFORM_H
