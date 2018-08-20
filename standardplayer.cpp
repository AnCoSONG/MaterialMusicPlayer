#include "standardplayer.h"
#include "ui_standardplayer.h"


StandardPlayer::StandardPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StandardPlayer)
{

    ui->setupUi(this);
    initStandardWidget();
}

StandardPlayer::~StandardPlayer()
{
    remove("cover.jpeg");
    delete ui;

}
/**
 * @brief StandardPlayer::on_pushButton_CLOSE_toggled
 * @param checked
 * 关闭按钮槽
 */
void StandardPlayer::on_pushButton_CLOSE_toggled(bool checked)
{
    qDebug()<< "close signal";
    if(singleWindow){//单窗口时
        int reval = QMessageBox::warning(this,"提示","是要关闭程序还是最小化到托盘?","取消","最小化到托盘","关闭");
        switch (reval) {
        case 2:
            this->close();
            break;
        case 1://最小化
             qDebug()<<"最小化";
             trayIcon = new QSystemTrayIcon(this);
             trayIcon->setIcon(QIcon(":/image/icon.png"));
             trayIcon->setToolTip(tr("音乐播放器(双击打开主界面)"));
             connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
             this->hide();
             trayIcon->show();
             break;
        default:
            break;
        }
    }else{ //多窗口时发送信号给高层窗体来操作关闭
        emit closeSignal();
    }
}

/**
 * @brief StandardPlayer::showVolumeBar
 * 显示音量widget
 */
void StandardPlayer::showVolumeBar(){
    ui->widget_VOLUMECONTROLBAR->show();
}

/**
 * @brief 播放全局初始化
 */
void StandardPlayer::initStandardWidget(){

    //设置Windows图标 这个只是项目运行时的
    this->setWindowIcon(QIcon(":/image/icon.png"));
    //设置checkable按钮 qt内部有bug,同时setChecked会崩溃
//    ui->pushButton_LOCALMUSIC->setCheckable(true);
//    ui->pushButton_LOCALMUSIC->setChecked(true);
//    ui->pushButton_ONLINEMUSIC->setCheckable(true);
//    ui->pushButton_MYSONGLIST->setCheckable(true);
//    ui->pushButton_MYFAVORITE->setCheckable(true);

    //初始化数据库 并读取数据库信息

    //初始化存储路径字符串链表
    localmusicPathList = new QStringList;
    myfavoritePathList = new QStringList;
    //初始化播放器
    player = new QMediaPlayer(this);
    songlist = new QMediaPlaylist(this);
    myfavorite = new QMediaPlaylist(this);
    myfavorite->setPlaybackMode(QMediaPlaylist::Loop);
    songlist->setPlaybackMode(QMediaPlaylist::Loop); //默认循环播放 之后添加按钮来调节
    initPLAYLIST();//初始化歌单和歌单对应歌曲路径链表
    currentIndex = 0;
    player->setPlaylist(PLAYLIST->at(currentIndex));
    player->setPlaybackRate(1);
    player->setVolume(30);

    //初始化堆栈窗口
    ui->stackedWidget->setCurrentIndex(0);//设置为本地音乐
    //初始化本地歌曲表格
    ui->tableWidget_MUSICLIST->horizontalHeader()->setStretchLastSection(true);//设置充满宽度
    ui->tableWidget_MUSICLIST->setColumnWidth(0,285);
    ui->tableWidget_MUSICLIST->setColumnWidth(1,200);
    ui->tableWidget_MUSICLIST->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget_MUSICLIST->setShowGrid(false); //设置不显示格子线
//    ui->tableWidget_MUSICLIST->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget_MUSICLIST->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget_MUSICLIST->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_MUSICLIST->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置支持多选
    ui->tableWidget_MUSICLIST->verticalScrollBar()->setStyleSheet("QScrollBar{background:white; width: 10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
    ui->tableWidget_MUSICLIST->horizontalScrollBar()->setStyleSheet("QScrollBar{background:white; height:10px;}"
     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
     "QScrollBar::handle:hover{background:gray;}"
     "QScrollBar::sub-line{background:transparent;}"
     "QScrollBar::add-line{background:transparent;}");
    //初始化我最喜爱表格
    ui->tableWidget_MYFAVORITE->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_MYFAVORITE->setColumnWidth(0,285);
    ui->tableWidget_MYFAVORITE->setColumnWidth(1,200);
    ui->tableWidget_MYFAVORITE->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->tableWidget_MYFAVORITE->setShowGrid(false); //设置不显示格子线
//    ui->tableWidget_MYFAVORITE->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget_MYFAVORITE->setContextMenuPolicy(Qt::CustomContextMenu);//设置支持右键菜单
    ui->tableWidget_MYFAVORITE->setFocusPolicy(Qt::NoFocus);

    ui->tableWidget_MYFAVORITE->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置支持多选
    ui->tableWidget_MYFAVORITE->verticalScrollBar()->setStyleSheet("QScrollBar{background:white; width: 10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
    ui->tableWidget_MYFAVORITE->horizontalScrollBar()->setStyleSheet("QScrollBar{background:white; height:10px;}"
     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
     "QScrollBar::handle:hover{background:gray;}"
     "QScrollBar::sub-line{background:transparent;}"
     "QScrollBar::add-line{background:transparent;}");
    //初始化我的歌单表格
    ui->tableWidget_MYSONGLIST->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_MYSONGLIST->setColumnWidth(0,200);
    ui->tableWidget_MYSONGLIST->setColumnWidth(1,200);
    ui->tableWidget_MYSONGLIST->setColumnWidth(2,105);
    ui->tableWidget_MYSONGLIST->setFrameShape(QFrame::NoFrame);
    ui->tableWidget_MYSONGLIST->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget_MYSONGLIST->setShowGrid(false);
    ui->tableWidget_MYSONGLIST->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget_MYSONGLIST->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_MYSONGLIST->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置支持多选
    initMySongListTable();

    //初始化在线音乐界面和功能


    //设置阴影
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setOffset(4,4);
    effect->setColor(QColor(0,0,0,50));
    effect->setBlurRadius(10);
    ui->frame->setGraphicsEffect(effect);
    //设置 set widget种的musicpath不可编辑
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);
    playbackStyle = new QButtonGroup(ui->widget_SET);
    playbackStyle->setExclusive(true);
    playbackStyle->addButton(ui->radioButton_normal);
    playbackStyle->addButton(ui->radioButton_baby);
    playbackStyle->addButton(ui->radioButton_girl);
    playbackStyle->addButton(ui->radioButton_oldGEGE);
    playbackStyle->addButton(ui->radioButton_rich);
    playbackStyle->addButton(ui->radioButton_scream);
    //隐藏一些初始不需要被看到的widget
    ui->widget_SET->hide();
    ui->label_COVER->hide();
    ui->widget_VOLUMECONTROLBAR->hide();
    ui->pushButton_PAUSE->hide();
    //链接信号和槽
    connect(ui->pushButton_CLOSE,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_CLOSE_toggled(bool)));
    connect(ui->pushButton_ST_MINI,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_ST_MINI_toggled(bool)));
    connect(ui->pushButton_VOLUMECONTROL,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_VOLUMECONTROL_toggled(bool)));
    connect(ui->pushButton_ADDMUSIC,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_ADDMUSIC_toggled(bool)));
    connect(player,SIGNAL(metaDataAvailableChanged(bool)),this,SLOT(updateMusicInfo()));
    connect(ui->tableWidget_MUSICLIST,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(on_tableWidget_MUSICLIST_cellDoubleClicked(int,int)));
    connect(ui->verticalSlider_VOLUMEBAR,SIGNAL(sliderMoved(int)),this,SLOT(on_verticalSlider_VOLUMEBAR_sliderMoved(int)));

    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(updateMusicPosition(qint64)));
    connect(ui->horizontalSlider_PROGRESSBAR,SIGNAL(sliderMoved(int)),this,SLOT(setPosition(int)));
    connect(ui->pushButton_PLAY,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_PLAY_toggled(bool)));
    connect(ui->pushButton_PAUSE,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_PAUSE_toggled(bool)));

    connect(ui->pushButton_SWITCHUI,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_SWITCHUI_toggled(bool)));
    connect(ui->pushButton_NEXTSONG,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_NEXTSONG_toggled(bool)));
    connect(ui->pushButton_PRESONG,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_PRESONG_toggled(bool)));
    connect(player,SIGNAL(volumeChanged(int)),this,SLOT(on_volume_changed(int))); //槽函数名称定义必须为on_信源_信号名,否则会弹出QMetaObject::connectSlotsByName: No matching signal for on_volume_changed(int)的报错
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(on_player_state_change(QMediaPlayer::State)));
    connect(ui->pushButton_ONLINEMUSIC,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_ONLINEMUSIC_toggled(bool)));
    connect(ui->pushButton_LOCALMUSIC,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_LOCALMUSIC_toggled(bool)));
    connect(ui->pushButton_MYSONGLIST,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_MYSONGLIST_toggled(bool)));
    connect(ui->pushButton_MYFAVORITE,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_MYFAVORITE_toggled(bool)));
    connect(ui->tableWidget_MUSICLIST,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_ContextMenu()));
    connect(ui->tableWidget_MYFAVORITE,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_ContextMenu()));
    connect(ui->pushButtonPLAYBACKMODE,SIGNAL(clicked(bool)),this,SLOT(on_pushButtonPLAYBACKMODE_toggled(bool)));
    connect(ui->pushButton_COVER,SIGNAL(toggled(bool)),this,SLOT(on_pushButton_COVER_toggled(bool)));
    connect(ui->pushButton_SWITCHUI,SIGNAL(toggled(bool)),this,SLOT(on_pushButton_SWITCHUI_toggled(bool)));
    connect(ui->pushButton_SETTING,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_SETTING_toggled(bool)));
    connect(ui->pushButton_cancle,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_CANCEL_toggled(bool)));
    connect(ui->pushButton_getPath,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_GETPATH_toggled(bool)));
    connect(ui->pushButton_YES,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_YES_toggled(bool)));

    //获取默认路径下的歌曲 1.4版本新增
    getLocalMusicFiles();

}

//1.6版本新增
void StandardPlayer::locatedPlayingMusic(){
    QTableWidget *temp = SONGLISTTABLELIST->at(currentIndex);
    for(int i = 0; i < temp->rowCount(); i ++){
        temp->item(i,0)->setSelected(false);
    }
    qDebug()<<PLAYLIST->at(currentIndex)->currentIndex();
    temp->item(PLAYLIST->at(currentIndex)->currentIndex(),0)->setSelected(true);
}

//1.4版本新增
/**
 * @brief StandardPlayer::getLocalMusicFiles
 * 清空之前的本地歌单，并将新的路径下的音乐文件添加到本地歌曲
 */
void StandardPlayer::getLocalMusicFiles(){
    ADDEDLIST->at(0)->clear();
    PLAYLIST->at(0)->clear();
    ui->tableWidget_MUSICLIST->clearContents();
    for(int i = ui->tableWidget_MUSICLIST->rowCount();i>0; i --){
        ui->tableWidget_MUSICLIST->removeRow(0);
    }
    QStringList nameFilters;
    nameFilters <<"*.mp3" <<"*.flac"<<"*.m4a" <<"*.wav";
    QString path(musicPaths.isEmpty()?QDir::homePath():musicPaths.first());
    QDir dir(path);
    QStringList files2Add = dir.entryList(nameFilters,QDir::Files|QDir::Readable,QDir::Name);
    for(int i = 0; i < files2Add.size(); i ++){
        qDebug()<<files2Add.at(i);
        QString tFileName = files2Add.at(i);
        qDebug()<<path;
        QString tAbsolutePath;
        tAbsolutePath = path + "/" + tFileName; //获取绝对路径
        ADDEDLIST->at(0)->append(tAbsolutePath);  // 添加目录
        PLAYLIST->at(0)->addMedia(QUrl::fromLocalFile(tAbsolutePath)); //添加歌曲
        int num = ui->tableWidget_MUSICLIST->rowCount();
        ui->tableWidget_MUSICLIST->insertRow(num);
        QTableWidgetItem *musicInfo = new QTableWidgetItem(tFileName);
        QTableWidgetItem *beizhu = new QTableWidgetItem(QString());
        musicInfo->setFlags(musicInfo->flags()&(~Qt::ItemIsEditable));
        ui->tableWidget_MUSICLIST->setItem(num,0,musicInfo);
        ui->tableWidget_MUSICLIST->setItem(num,1,beizhu);

    }
    updateSongListTable_primary();

}
/**
 * @brief StandardPlayer::on_pushButton_SWITCHUI_toggled
 * @param checked
 */
void StandardPlayer::on_pushButton_SWITCHUI_toggled(bool checked)
{//暂时不准备设置多ui界面
    qDebug()<<"switch ui";
    if(singleWindow){
        QMessageBox::information(this,"提示","现在为单UI模式，不支持切换",QMessageBox::Yes);
    }else{
        emit switchUiSignal(type);
    }
}

/**
 * @brief StandardPlayer::on_pushButton_ST_MINI_toggled 窗口最小化
 * @param checked
 */
void StandardPlayer::on_pushButton_ST_MINI_toggled(bool checked)//托盘菜单实现模块
{

    //最小化
    if(windowState()!=Qt::WindowMinimized){
        //后期加入最小化动画
        setWindowState(Qt::WindowMinimized);
    }
}

/**
 * @brief StandardPlayer::on_activatedSysTrayIcon 使系统托盘响应操作的方法
 * @param reason QSystemTrayIcon响应操作的枚举
 */
void StandardPlayer::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason){
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        //显示消息球 1s后消失 加入后会无法读取到双击信息
//        trayIcon->showMessage(tr("提示"),tr("这是一个音乐播放器，双击系统托盘图标显示界面"),QSystemTrayIcon::Information,1000);
        break;
    case QSystemTrayIcon::DoubleClick:
        //双击显示
        this->show();
        trayIcon->hide();
        break;
    default:
        break;
    }
}

void StandardPlayer::on_pushButton_VOLUMECONTROL_toggled(bool checked)
{
    ui->widget_VOLUMECONTROLBAR->show();
    this->startTimer(1000);
    //之后就可以设置音量信号和槽
}

void StandardPlayer::timerEvent(QTimerEvent *ev){
    if(ui->widget_VOLUMECONTROLBAR->isVisible()){
        QPoint oPoint = QCursor::pos();
//        QPoint oselfPoint = this->mapFromGlobal(oPoint);
        QPoint oWidgetPoint = ui->widget_VOLUMECONTROLBAR->mapFromGlobal(oPoint);
//        QRect oselfRect = this->rect();
        QRect oWidgetRect = ui->widget_VOLUMECONTROLBAR->rect();
//        !oselfRect.contains(oselfPoint));
        if(!oWidgetRect.contains(oWidgetPoint)){
                ui->widget_VOLUMECONTROLBAR->hide();
                killTimer(ev->timerId());
    }
    }else{
            killTimer(ev->timerId());
    }
}

void StandardPlayer::on_pushButton_ADDMUSIC_toggled(bool checked)
{
    QString fileExtension = "";
    bool isRepeated = false;
    const QStringList thisMusicPath =
            QFileDialog::getOpenFileNames(this,"添加歌曲",musicPaths.isEmpty()?QDir::homePath():musicPaths.first(),tr("MP3 files (*.mp3);;FLAC files (*.flac);;WAV files(*.wav);;M4A files(*.m4a);;All files (*.*)"));
    for(int j = 0; j < thisMusicPath.size();j++){//添加音乐

        isRepeated = false;
        QFileInfo tempThisMusicPathMusicName(thisMusicPath.at(j));
        fileExtension = tempThisMusicPathMusicName.completeSuffix();
        if(fileExtension=="flac"||fileExtension=="wav"||fileExtension=="ape"||fileExtension=="m4a"){
            QMessageBox::warning(this,"警告","非mp3文件暂不支持获取专辑封面",QMessageBox::Yes);
        }else if(fileExtension=="mp3"||fileExtension=="MP3"){

        }else if(fileExtension=="txt"||fileExtension=="mp4"||fileExtension=="avi"||fileExtension=="jpg"||fileExtension=="cpp"){
           QMessageBox::critical(this,"警告","不要导入非音乐文件!",QMessageBox::Yes);
           continue;
        }else if(fileExtension=="ncm"){
           QMessageBox::critical(this,"警告",".ncm为网易云收费音乐使用的加密文件，暂时不支持！",QMessageBox::Yes);
           continue;
        }
        QString tAddingMusicName = tempThisMusicPathMusicName.fileName();
        for(int i=0; i < ADDEDLIST->at(currentIndex)->size();i++){
            QFileInfo *tempAddedMusic = new QFileInfo(ADDEDLIST->at(currentIndex)->at(i));
            QString tAddedMusicName = tempAddedMusic->fileName();

            if(tAddedMusicName==tAddingMusicName){
                QMessageBox::information(this,"提示","添加音乐出错-"+tAddingMusicName+"已存在在列表",QMessageBox::Yes);
                qDebug()<<tAddingMusicName<<"已存在";
                isRepeated = true;
                break;
            }
        }
        if(!isRepeated){
            musicNumber++;//总共导入的歌曲数量
            ADDEDLIST->at(currentIndex)->append(thisMusicPath.at(j));
            PLAYLIST->at(currentIndex)->addMedia(QUrl::fromLocalFile(thisMusicPath.at(j)));
            new_updateTable(tAddingMusicName);
        }
    }
    updateSongListTable_primary(); //添加完成后update歌曲数量
    //最后update tablewidget 使内容显示出来
    //    updateTable(thisTimeAddNumber);有bug
}

void StandardPlayer::on_player_state_change(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::PlayingState){
        ui->pushButton_PLAY->hide();
        ui->pushButton_PAUSE->show();
    }else if(state==QMediaPlayer::PausedState||state==QMediaPlayer::StoppedState){
        ui->pushButton_PAUSE->hide();
        ui->pushButton_PLAY->show();
    }
}

void StandardPlayer::updateMusicInfo(){
    remove("cover.jpeg");
    if(PLAYLIST->at(currentIndex)->currentIndex()>=0){ //稍稍有些延迟！ 现在可以在无法读出标题解和
        QString titleTemp = player->metaData(QMediaMetaData::Title).toString();
        if(!titleTemp.isNull()){
            ui->label_MUSICTITLE->setText(titleTemp);
            ui->label_MUSICTITLE->setShowText(titleTemp);//必须同时set两个！
        }else{
            QFileInfo *temp = new QFileInfo(ADDEDLIST->at(currentIndex)->at(PLAYLIST->at(currentIndex)->currentIndex()));
            QString tempName = temp->fileName();
            ui->label_MUSICTITLE->setText(tempName);
            ui->label_MUSICTITLE->setShowText(tempName);
        }

        QStringList authorList = player->metaData(QMediaMetaData::Author).toStringList();
        if(!authorList.isEmpty()){
        QString author;
        for(int i = 0; i < authorList.size(); i ++){
            author += authorList.at(i);
            if(authorList.size()-i>1){
                author += " / ";
            }
        }
        ui->label_MUSICIANS->setText(author);
        }else{
            ui->label_MUSICIANS->setText("未知艺术家");
        }
    QVariant durantion = player->metaData(QMediaMetaData::Duration);
    ui->horizontalSlider_PROGRESSBAR->setMaximum(durantion.toLongLong());  //设置总长
    qint64 dura = durantion.toLongLong();
    QTime posit(0,dura/60000,dura%60000/1000);
    ui->label_RESTTIME->setText(posit.toString("mm:ss")); //这个可以实现mm:ss的样式
    //get cover
    qDebug()<<"当前位置"<<currentIndex;
    qDebug()<<"路径链表大小"<<ADDEDLIST->size();
    qDebug()<<"歌单链表大小"<<PLAYLIST->size();
    qDebug()<<"当前歌单存储路径大小"<<ADDEDLIST->at(currentIndex)->size();
    qDebug()<<"当前歌单播放列表位置"<<PLAYLIST->at(currentIndex)->currentIndex();


        QString path = ADDEDLIST->at(currentIndex)->at(PLAYLIST->at(currentIndex)->currentIndex()); //这行代码会导致崩溃！！ 由于空列表的情况下currentIndex会返回-1 导致崩溃！！
        //set cover
        ReadAPICFromMP3(path);
        qDebug()<<QPixmap("cover.jpeg").isNull();
        if(!QPixmap("cover.jpeg").isNull()){
            ui->label_COVER->setAlignment(Qt::AlignCenter);
            ui->label_COVER->setPixmap(QPixmap("cover.jpeg").scaled(360,360,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        }else{
            ui->label_COVER->setAlignment(Qt::AlignCenter);
            ui->label_COVER->setPixmap(QPixmap(":/image/icon.png"));
        }
    }

//    locatedPlayingMusic();  不能放在这里
}

void StandardPlayer::updateMusicPosition(qint64 posi){
    ui->horizontalSlider_PROGRESSBAR->setValue(posi);
    QTime posit(0,posi/60000,posi%60000/1000.0);
    qint64 rest = player->metaData(QMediaMetaData::Duration).toLongLong()-posi;
    QTime restT(0,rest/60000,rest%60000/1000);
    ui->label_PLAYEDTIME->setText(posit.toString("mm:ss"));
    ui->label_RESTTIME->setText(restT.toString("mm:ss"));
}

void StandardPlayer::setPosition(int pos){
    player->setPosition(pos);
}

void StandardPlayer::updateTable(int numThisTimeAdd){ //调用songlist.mediaObject.metaDate(QMediaMetaDate::)遍历 可能依然有问题 暂时先不用
    for(int i = 0 ; i < numThisTimeAdd; i ++){
        int num = ui->tableWidget_MUSICLIST->rowCount();
        ui->tableWidget_MUSICLIST->insertRow(num);
        //第一次 i+num+1 为当前歌曲的真实序号

        QTableWidgetItem *index = new QTableWidgetItem(QString::number(i+num+1));
        index->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *musictitle = new QTableWidgetItem;
        QTableWidgetItem *musicians = new QTableWidgetItem;
        QTableWidgetItem *albumtitle = new QTableWidgetItem;
        songlist->setCurrentIndex(i+num);
        player->play();
        qDebug()<<player->state();
        qDebug()<<player->metaData(QMediaMetaData::Title);
        qDebug()<<player->metaData(QMediaMetaData::Author);
        musictitle->setText(player->metaData(QMediaMetaData::Title).toString());
        musicians->setText(player->metaData(QMediaMetaData::Author).toString());
        albumtitle->setText(player->metaData(QMediaMetaData::AlbumTitle).toString());
        player->pause();//设置connect 使播放暂停瞬间音乐信息可以被读取
        ui->tableWidget_MUSICLIST->setItem(i+num,0,index);
        ui->tableWidget_MUSICLIST->setItem(i+num,1,musictitle);
        ui->tableWidget_MUSICLIST->setItem(i+num,2,musicians);
        ui->tableWidget_MUSICLIST->setItem(i+num,3,albumtitle);
    }
}

void StandardPlayer::new_updateTable(QString fileinfo){
    if(currentIndex == 0){
        int num = ui->tableWidget_MUSICLIST->rowCount();
        ui->tableWidget_MUSICLIST->insertRow(num);
        QTableWidgetItem *musicInfo = new QTableWidgetItem(fileinfo);
        QTableWidgetItem *beizhu = new QTableWidgetItem(QString());
        musicInfo->setFlags(musicInfo->flags()&(~Qt::ItemIsEditable));
        ui->tableWidget_MUSICLIST->setItem(num,0,musicInfo);
        ui->tableWidget_MUSICLIST->setItem(num,1,beizhu);
    }else if(currentIndex == 1){
        int num = ui->tableWidget_MYFAVORITE->rowCount();
        ui->tableWidget_MYFAVORITE->insertRow(num);
        QTableWidgetItem *musicInfo = new QTableWidgetItem(fileinfo);
        QTableWidgetItem *beizhu = new QTableWidgetItem(QString());
        musicInfo->setFlags(musicInfo->flags()&(~Qt::ItemIsEditable)); //设置部分不可编辑
        ui->tableWidget_MYFAVORITE->setItem(num,0,musicInfo);
        ui->tableWidget_MYFAVORITE->setItem(num,1,beizhu);
    }


}

void StandardPlayer::initMySongListTable()
{
    //添加本地音乐歌单
    int rows = ui->tableWidget_MYSONGLIST->rowCount();
    ui->tableWidget_MYSONGLIST->insertRow(rows);
    QTableWidgetItem *songListName = new QTableWidgetItem(QString(tr("本地音乐")));
    QTableWidgetItem *songListStyle = new QTableWidgetItem(QString(tr("欧美/日本")));
    QTableWidgetItem *songListRemark = new QTableWidgetItem(QString::number(ADDEDLIST->at(0)->size()));
    songListName->setFlags(songListName->flags()&(~Qt::ItemIsEditable));
    songListStyle->setFlags(songListStyle->flags()&(~Qt::ItemIsEditable));
    songListRemark->setFlags(songListStyle->flags()&(~Qt::ItemIsEditable));
    ui->tableWidget_MYSONGLIST->setItem(rows,0,songListName);
    ui->tableWidget_MYSONGLIST->setItem(rows,1,songListStyle);
    ui->tableWidget_MYSONGLIST->setItem(rows,2,songListRemark);
    //添加我最喜爱歌单
    ui->tableWidget_MYSONGLIST->insertRow(rows+1);
    QTableWidgetItem *songListName1 = new QTableWidgetItem(QString(tr("我最喜爱")));
    QTableWidgetItem *songListStyle1 = new QTableWidgetItem(QString(tr("欧美/电子")));
    QTableWidgetItem *songListRemark1 = new QTableWidgetItem(QString::number(ADDEDLIST->at(1)->size()));
    songListName1->setFlags(songListName1->flags()&(~Qt::ItemIsEditable));
    songListStyle1->setFlags(songListStyle1->flags()&(~Qt::ItemIsEditable));
    songListRemark1->setFlags(songListStyle->flags()&(~Qt::ItemIsEditable));
    ui->tableWidget_MYSONGLIST->setItem(rows+1,0,songListName1);
    ui->tableWidget_MYSONGLIST->setItem(rows+1,1,songListStyle1);
    ui->tableWidget_MYSONGLIST->setItem(rows+1,2,songListRemark1);

    //循环添加其他歌单 可以选择创建一个类 包含名称 风格 备注 内部歌曲路径 内部歌曲名称 以及 歌单对应widget界面 现在已经来不及写这么大的工程 但之后要重写一下歌单这部分！！

}

void StandardPlayer::updateSongListTable_primary()
{
    for(int i = 0; i < ui->tableWidget_MYSONGLIST->rowCount();i ++){
         ui->tableWidget_MYSONGLIST->item(i,2)->setText(QString::number(ADDEDLIST->at(i)->size()));

    }
}

// 之后三个method实现全局拖拽
void StandardPlayer::mouseMoveEvent(QMouseEvent *event)
{
    if(bPressFlag)
    {
        QPoint relaPos(QCursor::pos() - beginDrag);
        move(relaPos);
    }
    QWidget::mouseMoveEvent(event);
}

void StandardPlayer::mouseReleaseEvent(QMouseEvent *event)
{
    bPressFlag = false;
    QWidget::mouseReleaseEvent(event);
}

void StandardPlayer::mousePressEvent(QMouseEvent *event)
{
    bPressFlag = true;
    beginDrag = event->pos();
    QWidget::mousePressEvent(event);
}

// 双击本地音乐单元格
void StandardPlayer::on_tableWidget_MUSICLIST_cellDoubleClicked(int row, int column)
{
    if(column==0){
        songlist->setCurrentIndex(row);
        player->play();
    }else if(column == 1){
        //调用数据库存入备注信息 每次编辑时都存一次
    }
}

//
void StandardPlayer::on_verticalSlider_VOLUMEBAR_sliderMoved(int position)
{
    player->setVolume(position);
}



void StandardPlayer::on_volume_changed(int value){
    ui->verticalSlider_VOLUMEBAR->setValue(value);
}


void StandardPlayer::on_pushButton_PLAY_toggled(bool checked)
{
    if(!PLAYLIST->at(currentIndex)->isEmpty()){
        player->play();
        locatedPlayingMusic();
    }else{
        QMessageBox::information(this,"提示","当前歌曲列表为空",QMessageBox::Yes);
    }


//    if(player->state()==QMediaPlayer::PlayingState){
//        ui->pushButton_PLAY->hide();
//        ui->pushButton_PAUSE->show();
//    }else{
//        QMessageBox::information(this,"出错了","无法播放",QMessageBox::Yes);
//    }
}

void StandardPlayer::on_pushButton_PAUSE_toggled(bool checked)
{
    player->pause();
}

void StandardPlayer::on_pushButton_NEXTSONG_toggled(bool checked)
{
    //列表移动到那首歌的位置 那首歌的那一行被选中
    PLAYLIST->at(currentIndex)->next();
    //1.5版本 加入列表定位
    int index = PLAYLIST->at(currentIndex)->currentIndex();
    switch (currentIndex) {
    case 0:
        ui->tableWidget_MUSICLIST->scrollToItem(ui->tableWidget_MUSICLIST->item(index,0),QAbstractItemView::PositionAtCenter);

//        ui->tableWidget_MUSICLIST->item(index,0)->setSelected(true);
        break;
    case 1:
        ui->tableWidget_MYFAVORITE->scrollToItem(ui->tableWidget_MYFAVORITE->item(index,0),QAbstractItemView::PositionAtCenter);
//        ui->tableWidget_MYFAVORITE->item(index,0)->setSelected(true);
        break;
    default:
        break;
    }
    player->play();
    locatedPlayingMusic();


}

void StandardPlayer::on_pushButton_PRESONG_toggled(bool checked)
{
    //列表移动到那首歌的位置 那首歌的哪一行被选中
    PLAYLIST->at(currentIndex)->previous();
    int index = PLAYLIST->at(currentIndex)->currentIndex();
    switch (currentIndex) {
    case 0:
        ui->tableWidget_MUSICLIST->scrollToItem(ui->tableWidget_MUSICLIST->item(index,0),QAbstractItemView::PositionAtCenter);
//        ui->tableWidget_MUSICLIST->item(index,0)->setSelected(true);
        break;
    case 1:
        ui->tableWidget_MYFAVORITE->scrollToItem(ui->tableWidget_MYFAVORITE->item(index,0),QAbstractItemView::PositionAtCenter);
//        ui->tableWidget_MYFAVORITE->item(index,0)->setSelected(true);

        break;
    default:
        break;
    }
    player->play();
    locatedPlayingMusic(); //这个函数必须在播放方法之后执行，否则会闪退

}




void StandardPlayer::on_pushButton_ONLINEMUSIC_toggled(bool checked)//同时又两个checked会报错！ 可以将白块设成一个label 这样也好做动画！
{

    ui->label_HANDLEBETWEEN->move(207,212);
    ui->label_WHITEHANDLE->move(80,212);
    ui->stackedWidget->setCurrentWidget(ui->OnlinePage);
}

void StandardPlayer::on_pushButton_LOCALMUSIC_toggled(bool checked)
{
    ui->label_WHITEHANDLE->move(80,132);
    ui->label_HANDLEBETWEEN->move(207,132);
    ui->stackedWidget->setCurrentWidget(ui->tableMusicList);
    if(currentIndex!=0){
        currentIndex = 0;
//        player->stop();
        player->setPlaylist(PLAYLIST->at(currentIndex));
        qDebug()<<"当前歌单歌曲数量为:"<<ADDEDLIST->at(currentIndex)->size();
    }
}

/**
 * @brief StandardPlayer::on_pushButton_MYSONGLIST_toggled
 * @param checked
 * 选中我的歌单列表
 */
void StandardPlayer::on_pushButton_MYSONGLIST_toggled(bool checked)
{
    ui->label_WHITEHANDLE->move(80,292);
    ui->label_HANDLEBETWEEN->move(207,292);
    ui->stackedWidget->setCurrentWidget(ui->MySongList);


}

/**
 * @brief StandardPlayer::on_pushButton_MYFAVORITE_toggled
 * @param checked 暂时不用
 * 选中歌单槽函数
 */
void StandardPlayer::on_pushButton_MYFAVORITE_toggled(bool checked)//选中歌单时 切换至此歌单
{
    ui->label_WHITEHANDLE->move(80,372);
    ui->label_HANDLEBETWEEN->move(207,372);

    ui->stackedWidget->setCurrentWidget(ui->MyFavorite);
    if(currentIndex!=1){

        currentIndex = 1;
//        player->stop();
        player->setPlaylist(PLAYLIST->at(currentIndex));
        qDebug()<<"当前歌单歌曲数量为:"<<ADDEDLIST->at(currentIndex)->size();
    }
}

/**
 * @brief StandardPlayer::initPLAYLIST
 *  初始化播放列表
 */
void StandardPlayer::initPLAYLIST(){ //这是创建一个新歌单时也必须要做的 添加一个播放列表 添加一个歌曲路径列表 添加一个歌单表格窗体列表
    PLAYLIST = new QList<QMediaPlaylist*>();
    PLAYLIST->append(songlist);
    PLAYLIST->append(myfavorite);
    ADDEDLIST = new QList<QStringList*>();
    ADDEDLIST->append(localmusicPathList);
    ADDEDLIST->append(myfavoritePathList);
    SONGLISTTABLELIST = new QList<QTableWidget*>();
    SONGLISTTABLELIST->append(ui->tableWidget_MUSICLIST);
    SONGLISTTABLELIST->append(ui->tableWidget_MYFAVORITE);
}

/**
 * @brief StandardPlayer::on_tableWidget_MYFAVORITE_cellDoubleClicked
 * @param row 行
 * @param column 列
 * 双击我最喜爱表格槽函数
 */
void StandardPlayer::on_tableWidget_MYFAVORITE_cellDoubleClicked(int row, int column)
{
    if(column==0){
        PLAYLIST->at(currentIndex)->setCurrentIndex(row);
        player->play();
    }else if(column == 1){
        //调用数据库存入备注信息 每次编辑时都存一次
    }
}

void StandardPlayer::on_ContextMenu()
{
    if(currentIndex == 0){
        QMenu *tableMenu = new QMenu(ui->tableWidget_MUSICLIST);
        QAction *pAddMusic = new QAction("添加歌曲",ui->tableWidget_MUSICLIST);
        QAction *pRemoveMusic = new QAction("删除歌曲",ui->tableWidget_MUSICLIST);
        connect(pAddMusic,SIGNAL(triggered(bool)),this,SLOT(on_pushButton_ADDMUSIC_toggled(bool)));
        connect(pRemoveMusic,SIGNAL(triggered(bool)),this,SLOT(on_RemoveMusics()));
        tableMenu->addAction(pAddMusic);
        tableMenu->addAction(pRemoveMusic);
        tableMenu->move(cursor().pos());
        tableMenu->show();

    }else if(currentIndex == 1){
        QMenu *tableMenu = new QMenu(ui->tableWidget_MYFAVORITE);
        QAction *pAddMusic = new QAction("添加歌曲",ui->tableWidget_MYFAVORITE);
        QAction *pRemoveMusic = new QAction("删除歌曲",ui->tableWidget_MYFAVORITE);
        connect(pAddMusic,SIGNAL(triggered(bool)),this,SLOT(on_pushButton_ADDMUSIC_toggled(bool)));
        connect(pRemoveMusic,SIGNAL(triggered(bool)),this,SLOT(on_RemoveMusics()));
        tableMenu->addAction(pAddMusic);
        tableMenu->addAction(pRemoveMusic);
        tableMenu->move(cursor().pos());
        tableMenu->show();
    }
}

void StandardPlayer::on_RemoveMusics() //删除部分待修复 思路仿照更换目录清空时的思路 删除某一行之后，行数会变化，行标也会变化，这个需要特别注意！
{
        //获取选中的行标 从0开始
        qDebug()<<"开始删除";
        QList<QTableWidgetItem*> itemsSelected = SONGLISTTABLELIST->at(currentIndex)->selectedItems();
        qDebug()<<itemsSelected.size();
        QList<int> *indexes = new QList<int>();
        for(int i = 0; i < itemsSelected.size(); i ++){
            int rows = SONGLISTTABLELIST->at(currentIndex)->row(itemsSelected.at(i));
            indexes->append(rows);
            qDebug()<<"删第"<<rows<<"行";
        }
        //添加对行数排序的代码
        std::sort(indexes->begin(),indexes->end());
        qDebug()<<"排序后";
        //检查是否完成排序
        foreach (int i, *indexes) {
            qDebug()<<i;
        }
        //排序完成
        int rowc = indexes->size();
        for(int i = 0; i < rowc; i ++){
            int row = indexes->at(i);
            qDebug()<<"repeat:删第"<<row<<"行";
            qDebug()<<"实际删除第"<<row-i<<"项";
            SONGLISTTABLELIST->at(currentIndex)->removeRow(row-i);  //从表中删除
            PLAYLIST->at(currentIndex)->removeMedia(row-i);//从playlist中删除
            //从路径库中删除
            ADDEDLIST->at(currentIndex)->removeAt(row-i);
//            qDebug()<<"当前歌单列表大小"<<SONGLISTTABLELIST->at(currentIndex)->rowCount();
//            qDebug()<<"当前歌单路径库大小"<<ADDEDLIST->at(currentIndex)->size();
            //从json表中删除 if has
        }
        updateSongListTable_primary();//更新歌曲数量

}

void StandardPlayer::on_pushButtonPLAYBACKMODE_toggled(bool checked)
{
    switch (PLAYLIST->at(currentIndex)->playbackMode()) {
    case QMediaPlaylist::CurrentItemInLoop:
        PLAYLIST->at(currentIndex)->setPlaybackMode(QMediaPlaylist::Loop);
        ui->pushButtonPLAYBACKMODE->setStyleSheet(QString("QPushButton#pushButtonPLAYBACKMODE{border-image: url(:/image/ST_LOOP_CHECKED.png);}"));
        break;
    case QMediaPlaylist::Loop:
        PLAYLIST->at(currentIndex)->setPlaybackMode(QMediaPlaylist::Random);
        ui->pushButtonPLAYBACKMODE->setStyleSheet(QString("QPushButton#pushButtonPLAYBACKMODE{border-image: url(:/image/ST_RANDOM_CHECKED.png);}"));
        break;
    case QMediaPlaylist::Random:
        PLAYLIST->at(currentIndex)->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        ui->pushButtonPLAYBACKMODE->setStyleSheet(QString("QPushButton#pushButtonPLAYBACKMODE{border-image: url(:/image/ST_CURRENTINLOOP_CHECKED.png);}"));
        break;
    default:
        break;
    }
}

void StandardPlayer::on_pushButton_COVER_toggled(bool checked)
{
    if(checked){
        //显示动画
        ui->label_COVER->show();
        //隐藏动画

        //把背后的那些部件都hide掉
        ui->widget_SELECTBAR->hide();
        ui->stackedWidget->hide();
        ui->label_HANDLEBETWEEN->hide();
        ui->label_LOCLMUSIC->hide();
        ui->label_MYSONGLIST->hide();
        ui->label_MYFAVOURITE->hide();
        ui->label_ONLINEICON->hide();
        ui->label_WHITEHANDLE->hide();
        ui->pushButton_LOCALMUSIC->hide();
        ui->pushButton_ONLINEMUSIC->hide();
        ui->pushButton_MYSONGLIST->hide();
        ui->pushButton_MYFAVORITE->hide();
    }else{
        ui->label_COVER->hide();
        ui->widget_SELECTBAR->show();
        ui->stackedWidget->show();
        ui->label_HANDLEBETWEEN->show();
        ui->label_LOCLMUSIC->show();
        ui->label_MYSONGLIST->show();
        ui->label_MYFAVOURITE->show();
        ui->label_ONLINEICON->show();
        ui->label_WHITEHANDLE->show();
        ui->pushButton_LOCALMUSIC->show();
        ui->pushButton_ONLINEMUSIC->show();
        ui->pushButton_MYSONGLIST->show();
        ui->pushButton_MYFAVORITE->show();
    }
}

void StandardPlayer::on_pushButton_SETTING_toggled(bool checked)
{
    ui->widget_SET->show();
    //setting init;
    ui->lineEdit->setText(musicPaths.isEmpty()?QDir::homePath():musicPaths.first());
    ui->doubleSpinBox->setValue(player->playbackRate());
    //debug 时注释下面两行
    ui->doubleSpinBox->hide();
    ui->label_playbackrate->hide();


}

void StandardPlayer::on_pushButton_YES_toggled(bool checked)
{
    //setMusicPath 当路径不同时才执行重新获取文件
    if(musicPaths.first()!=ui->lineEdit->text()){
        player->stop();
        musicPaths.first() = ui->lineEdit->text();
        //1.4版本新增
        getLocalMusicFiles();
    }

    //set Playback Rate
//    player->setPlaybackRate(ui->doubleSpinBox->text().toDouble());
    QAbstractButton *checkedButton =  playbackStyle->checkedButton();
    QString checkedObjectName = checkedButton->objectName();
    qDebug()<<checkedObjectName;
    if(QString::compare(checkedObjectName,ui->radioButton_normal->objectName())==0){
        qDebug()<<"normal";
        player->setPlaybackRate(1.00);
    }else if(QString::compare(checkedObjectName,ui->radioButton_girl->objectName())==0){
        qDebug()<<"girl";
        player->setPlaybackRate(1.25);
    }else if(QString::compare(checkedObjectName,ui->radioButton_baby->objectName())==0){
        qDebug()<<"baby";
        player->setPlaybackRate(1.50);
    }else if(QString::compare(checkedObjectName,ui->radioButton_oldGEGE->objectName())==0){
        qDebug()<<"oldGEGE";
        player->setPlaybackRate(0.75);
    }else if(QString::compare(checkedObjectName,ui->radioButton_rich->objectName())==0){
        qDebug()<<"rich";
        player->setPlaybackRate(0.90);
    }else if(QString::compare(checkedObjectName,ui->radioButton_scream->objectName())==0){
        qDebug()<<"scream";
        player->setPlaybackRate(2.00);
    }
    ui->widget_SET->hide();


}

void StandardPlayer::on_pushButton_CANCEL_toggled(bool checked)
{
    ui->widget_SET->hide();
}

void StandardPlayer::on_pushButton_GETPATH_toggled(bool checked)
{
    //open filedialog get Dir
    QString dir = QFileDialog::getExistingDirectory(this,"选取默认路径",musicPaths.isEmpty()?QDir::homePath():musicPaths.first());
    //set Text
    ui->lineEdit->setText(dir);

}
