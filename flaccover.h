#ifndef FLACCOVER_H
#define FLACCOVER_H
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <cstring>
#include <iostream>
#include <QString>
#include <QTextCodec>
#include <QDebug>

typedef unsigned char byte;
using namespace std;

class FLACcover
{
public:
    FLACcover();
    struct FlacMetadataBlockHeader
    {
        byte flag;		//标志位，高1位：是否为最后一个数据块，低7位：数据块类型
        byte length[3];	//数据块长度，不含数据块头部
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
    bool writeCover(char *filename);
    bool writePictureDataToFile(const char *outFilePath);
    bool extractPicture(const char *inFilePath, const char *outFilePath);
    bool readCover(QString inPath, const char * outName = "cover");
};

#endif // FLACCOVER_H
