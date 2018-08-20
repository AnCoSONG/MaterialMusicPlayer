#ifndef STANDARDPLAYER_H
#define STANDARDPLAYER_H

#include <QWidget>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QSystemTrayIcon>
#include <QCursor>
#include <QPoint>
#include <QTimerEvent>
#include <QMediaPlayer>
#include <QFileInfo>
#include <QMediaMetaData>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTime>
#include <QMap>
#include <QMouseEvent>
#include <QPoint>
#include <QMetaObject>
#include <QPropertyAnimation>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QScrollBar>
#include <QButtonGroup>
#include "playerui.h"
#include "readcover.h"
//修一下播放器列表定位的问题 或者将定位功能单独列出
//修复删除歌曲功能
//准备进行大改动:将歌单widget添加进链表
namespace Ui {
class StandardPlayer;
}
class StandardPlayer : public QWidget
{
    Q_OBJECT

public:

    //歌单序号
    enum ORDER{
        LOCAL_MUSIC = 0,
        MY_FAVORITE = 1

    };


    //拖动用变量
    bool bPressFlag = false;
    QPoint beginDrag;
    int musicNumber;//歌曲数量
    int currentIndex = 0; //当前歌单位置
    QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation); //默认获取音乐的路径
    /*本地音乐存储*/
    QStringList *localmusicPathList; //读取到的音乐路径会存在这里,将来存在数据库里的音乐数据也需要将路径从本地读取到这个list中
    QStringList *myfavoritePathList; //我最喜爱的歌曲的音乐路径
    QMediaPlayer *player; //播放器
    QButtonGroup *playbackStyle;//播放速度
    QMediaPlaylist *songlist; //默认的本地音乐歌单
    QMediaPlaylist *myfavorite; //默认的我最喜欢歌单
    QList<QStringList*> *ADDEDLIST; //所有被添加的歌曲的路径 根据index索引到歌单 然后根据 index 访问到具体的路径
    QList<QMediaPlaylist*> *PLAYLIST; //所有歌单的链表
    QList<QTableWidget*> *SONGLISTTABLELIST;//存储歌单表格控件的地址
    void initPLAYLIST();
    bool singleWindow = true;
    PLAYERUI::TYPE type = PLAYERUI::STANDARD;
    explicit StandardPlayer(QWidget *parent = 0);
    ~StandardPlayer();
    void initStandardWidget();
    void timerEvent(QTimerEvent *);
    QSystemTrayIcon *trayIcon;
    void updateTable(int);
    void new_updateTable(QString);
    void initMySongListTable();
    void updateSongListTable_primary();
    void getLocalMusicFiles();//获取当前默认路径的歌曲文件
    void locatedPlayingMusic();
    //拖拽用方法
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent*);

private slots:
    void on_pushButton_CLOSE_toggled(bool checked); //关闭信号
    void showVolumeBar();//显示音量调
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason);//最小化
    void on_pushButton_SWITCHUI_toggled(bool checked);
    void on_pushButton_ST_MINI_toggled(bool checked);
    void on_pushButton_VOLUMECONTROL_toggled(bool checked);
    void updateMusicInfo();
    void updateMusicPosition(qint64);
    void setPosition(int);
    void on_volume_changed(int);
    void on_pushButton_ADDMUSIC_toggled(bool checked);
    void on_player_state_change(QMediaPlayer::State);
    void on_tableWidget_MUSICLIST_cellDoubleClicked(int row, int column);

    void on_verticalSlider_VOLUMEBAR_sliderMoved(int position);

    void on_pushButton_PLAY_toggled(bool checked);

    void on_pushButton_PAUSE_toggled(bool checked);

    void on_pushButton_NEXTSONG_toggled(bool checked);

    void on_pushButton_PRESONG_toggled(bool checked);


    void on_pushButton_ONLINEMUSIC_toggled(bool checked);

    void on_pushButton_LOCALMUSIC_toggled(bool checked);

    void on_pushButton_MYSONGLIST_toggled(bool checked);

    void on_pushButton_MYFAVORITE_toggled(bool checked);

    void on_tableWidget_MYFAVORITE_cellDoubleClicked(int row, int column);

    void on_ContextMenu();

    void on_RemoveMusics();

    void on_pushButtonPLAYBACKMODE_toggled(bool checked);

    void on_pushButton_COVER_toggled(bool checked);

    void on_pushButton_SETTING_toggled(bool checked);

    void on_pushButton_YES_toggled(bool checked);

    void on_pushButton_CANCEL_toggled(bool checked);

    void on_pushButton_GETPATH_toggled(bool checked);

signals:
    void closeSignal();
    void switchUiSignal(PLAYERUI::TYPE);

private:
    Ui::StandardPlayer *ui;

};

#endif // STANDARDPLAYER_H
