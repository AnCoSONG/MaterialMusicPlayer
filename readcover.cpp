#include "readcover.h"

bool isFrameAPIC(const unsigned char* cID3V2Fra_head)

{
    // 在win7中vs2012下，没发现strncasecmp函数，不然不需要两次判断
    // 当然了，这种判断也可以逐字节判断
    char a[4];
    a[0] = cID3V2Fra_head[0];
    a[1] = cID3V2Fra_head[1];
    a[2] = cID3V2Fra_head[2];
    a[3] = cID3V2Fra_head[3];
    if ((strncmp(a, "APIC", 4) == 0) ||
        (strncmp(a, "apic", 4) == 0))

        return true;

    return false;

}

int calcID3V2Len(const unsigned char* cID3V2_head)
{
    int len = (cID3V2_head[6] & 0x7f) << 21
        | (cID3V2_head[7] & 0x7f) << 14
        | (cID3V2_head[8] & 0x7f) << 7
        | (cID3V2_head[9] & 0x7f);

    return len;
}

int calclID3V2FraLength(const char* cID3V2Fra_head)

{
    int len = (int)(cID3V2Fra_head[4] * 0x100000000 +
        cID3V2Fra_head[5] * 0x10000 +
        cID3V2Fra_head[6] * 0x100 +
        cID3V2Fra_head[7]);
    return len;

}

bool isJPEG(const char* data)
{
    // JPEG格式数据的起始为0xFFD8，当然了，结束也有标志，但是这里不需要了
    if (((unsigned char)data[0] == 0xFF) &&
        ((unsigned char)data[1] == 0xD8))

        return true;
    return false;
}



int ReadAPICFromMP3(QString tpath)
{
    QTextCodec *code = QTextCodec::codecForName("gbk");
    std::string ts = code->fromUnicode(tpath).data(); //utf8转gdk gb2312
    const char* path = ts.c_str();
    FILE* fp = fopen(path, "rb");

    if (!fp)
    {
        qDebug()<<"cannot open this mp3\n";
//		system("pause");

        return -1;
    }



    // 这里的ID3V2以及帧标识的大小由标准规定均为10个字节

    // 这里其实应该是作为字节存储，用unsigned char更好，这里就简单用char替代吧

    unsigned char  cID3V2_head[10] = { 0 }, cID3V2Fra_head[10] = { 0 };
    long  ID3V2_len = 0, lID3V2Fra_length = 0;
    char* cID3V2Fra = NULL;



    // 读取帧头，这里就是为了判断是否是ID3V2的标签头

    fread(cID3V2_head, 10, 1, fp);
    if ((cID3V2_head[0] == 'I' || cID3V2_head[0] == 'i') &&
        (cID3V2_head[1] == 'D' || cID3V2_head[1] == 'd') &&
        cID3V2_head[2] == '3')
    {
        // 获取ID3V2标签的长度
        ID3V2_len = calcID3V2Len(cID3V2_head);
    }

    bool hasAPIC = false;


    while ((ftell(fp) + 10) <= ID3V2_len)
    {
        // 这里每个帧标识的长度也为10，由于每个帧标识的存储的数据的长度不一
        // 每次要读取出来，进行运算获取真正数据长度

        memset(cID3V2Fra_head, 0, 10);
        fread(cID3V2Fra_head, 10, 1, fp);
//        printf("\ncurrent:%d %s", ftell(fp), cID3V2Fra_head);
        lID3V2Fra_length = cID3V2Fra_head[4] * 0x100000000
            + cID3V2Fra_head[5] * 0x10000
            + cID3V2Fra_head[6] * 0x100
            + cID3V2Fra_head[7];
        if (lID3V2Fra_length == 0)
            break;
        if (isFrameAPIC(cID3V2Fra_head))
        {
            cID3V2Fra = (char*)calloc(lID3V2Fra_length, 1);
            if (cID3V2Fra != NULL)
            {
                hasAPIC = true;

                fread(cID3V2Fra, lID3V2Fra_length, 1, fp);
                qDebug()<<"fread";
            }
            break;
        }else
        {
            // 移动到下一帧标识
            fseek(fp, lID3V2Fra_length, SEEK_CUR);
            qDebug()<<ftell(fp);
        }
    }

    fclose(fp);

    // 到这里如果，专辑图片要么读出，要么就不存在

    if (hasAPIC)
    {
        // 这里整个数据的前面一部分数据是用来记录专辑图片的格式
        // 例如 image/jpeg image/png等，这里大部分的专辑图片都是jpeg的
        // 因此这里简单的只判断jpeg的格式，除去图片格式，数据前部依然有些是空数据
        // 因此以jpeg的标识来定位图片数据的起始
        int start = 0;

        while (start < lID3V2Fra_length)
        {
            if (isJPEG(cID3V2Fra + start))
            {
                break;

            }
                ++start;
        }

        if (start != lID3V2Fra_length)
        {
            // 这里没有错误处理，从简
              if(start!=13&&start!=14){start=13;}
              FILE* jpegFP = fopen("cover.jpeg", "wb");
              fwrite(cID3V2Fra +start, lID3V2Fra_length - start-10, 1, jpegFP);
              fclose(jpegFP);
              qDebug()<<lID3V2Fra_length;
              qDebug()<<start;
        }
    }
    return hasAPIC;
}

