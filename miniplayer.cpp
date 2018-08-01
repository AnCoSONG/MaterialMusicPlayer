#include "miniplayer.h"
#include "ui_miniplayer.h"

MiniPlayer::MiniPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiniPlayer)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

MiniPlayer::~MiniPlayer()
{
    delete ui;
}
