#ifndef SONGLISTWIDGET_H
#define SONGLISTWIDGET_H

#include <QTableWidget>

class SongListWidget: public QTableWidget
{

    Q_OBJECT
public:
    SongListWidget();
    QString songListName; //名称
    QString songListStyle; //风格
    QString songlistRemark; //备注
    QString songListContains; //含有歌曲数量

};

#endif // SONGLISTWIDGET_H
