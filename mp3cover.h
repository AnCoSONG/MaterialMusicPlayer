#ifndef MP3COVER_H
#define MP3COVER_H
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <cstring>
#include <QString>
#include <iostream>
#include <QTextCodec>
#include <QDebug>

typedef unsigned char byte;
using namespace std;

class MP3cover
{
public:
    MP3cover();
    //ID3v2标签头部结构体定义
    struct ID3V2Header
    {
        char  identi[3];//ID3头部校验，必须为“ID3”否则认为不存在ID3标签
        byte  major;	//ID3版本号，3是ID3v2.3，4是ID3v2.4，以此类推
        byte  revsion;	//ID3副版本号，此版本为00
        byte  flags;    //标志位
        byte  size[4];	//标签大小，不含标签头的10个字节
    };

    //ID3v2标签帧头部结构体定义
    struct ID3V2FrameHeader
    {
        char FrameId[4];//标识符，用于描述此标签帧的内容类型
        byte size[4];	//标签帧的大小，不含标签头的10个字节
        byte flags[2];	//标志位
    };

    struct ID3V22FrameHeader
    {
        char FrameId[3];//标识符，用于描述此标签帧的内容类型
        byte size[3];	//标签帧的大小，不含标签头的6个字节
    };

    byte *pPicData = 0;		//指向图片数据的指针
    int picLength = 0;		//存放图片数据长度
    char picFormat[4] = {};	//存放图片数据的格式（扩展名）

    bool verificationPictureFormat(char *data);
    void freePictureData();
    bool loadPictureData(const char *inFilePath);
    int getPictureLength();
    byte *getPictureDataPtr();
    char *getPictureFormat();
    bool writePictureDataToFile(const char *outFilePath);
    bool extractPicture(const char *inFilePath, const char *outFilePath);
    bool writeCover(char *filename);
    bool readCover(QString inPath, const char * outName = "cover");

};

#endif // MP3COVER_H
