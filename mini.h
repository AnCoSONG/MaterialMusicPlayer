#ifndef MINI_H
#define MINI_H

#include <QWidget>
#include <QDebug>
#include "rotatecover.h"

namespace Ui {
class Mini;
}

class Mini : public QWidget
{
    Q_OBJECT

public:
    explicit Mini(QWidget *parent = 0);
    QWidget *standard; //存储父类地址 用来进行切换
    ~Mini();
    void createStandard(QWidget *);

private slots:
    void on_pushButton_toggled(bool checked);

    void on_pushButton_play_toggled(bool checked);

    void on_pushButton_pause_toggled(bool checked);

private:
    Ui::Mini *ui;
};

#endif // MINI_H
