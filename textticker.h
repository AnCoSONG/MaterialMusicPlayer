#ifndef TEXTTICKER_H
#define TEXTTICKER_H

//跑马 label类 还需研究和优化
#include <QtWidgets/QLabel>
#include <QFontMetrics>
#include <QDebug>


class TextTicker : public QLabel
{
    Q_OBJECT


public:
    TextTicker(QWidget *parent = 0);
    ~TextTicker();
    void setShowText(QString);


protected:
    void paintEvent(QPaintEvent *event);
    void updateIndex();


private:
    int m_charWidth;
    int m_curIndex;
    QString m_showText;
};


#endif // TEXTTICKER_H
