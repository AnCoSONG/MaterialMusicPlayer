#ifndef ROTATECOVER_H
#define ROTATECOVER_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QTimer>
#include <QBitmap>
#include <QSize>
//功能已实现 但是由于小图片下锯齿化还是很严重
class RotateCover: public QLabel
{
public:
    RotateCover(QWidget *parent = 0);
    qreal imageRotate = 0;
    void paintEvent(QPaintEvent *);
    void updateRotate();
    void setRotatePixmap(QPixmap);
    static QPixmap PixmapToRound(QPixmap,int);
    bool startRotate = false;
    QPixmap *cover;

};

#endif // ROTATECOVER_H
