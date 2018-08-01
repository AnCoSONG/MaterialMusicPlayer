#ifndef MINIPLAYER_H
#define MINIPLAYER_H

#include <QWidget>
#include "playerui.h"

namespace Ui {
class MiniPlayer;
}
//暂时不用它
class MiniPlayer : public QWidget
{
    Q_OBJECT

public:
    PLAYERUI::TYPE type = PLAYERUI::MINI;
    explicit MiniPlayer(QWidget *parent = 0);
    ~MiniPlayer();

private:
    Ui::MiniPlayer *ui;
};

#endif // MINIPLAYER_H
