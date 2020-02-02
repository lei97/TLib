#pragma once

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

typedef char Byte;
typedef unsigned char Uint8;
const int BUF_SIZE_1KB = 1024;

template <typename T>
class ShareMem
{
public:
    ShareMem();       //构造
    ~ShareMem();      //析构
public:
    int Init(string bufName);   //初始化
    int Close(void);			//关闭
    T ReadData();	    //读取数据
    void LoadData();			//将实际数据压入Buf结构体
    int WriteData();			//写入数据
private:
    string BufName;
    HANDLE MapFile;
    int _bufSize;
    Byte* __byteBuf;
    T _data;
};

template <typename T> ShareMem<T>::ShareMem()
{
    _bufSize = sizeof(T) + 1;
    T* tmpBuf = new T;
    _byteBuf = (Byte*)tmpBuf;
}

template <typename T> ShareMem<T>::~ShareMem()
{
    delete _byteBuf;
    _byteBuf = NULL;
}

template <typename T>
int ShareMem<T>::Init(string BufName)
{
    // 创建共享文件句柄
    MapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // 物理文件句柄
        NULL,                    // 默认安全级别
        PAGE_READWRITE,          // 可读可写
        0,                       // 高位文件大小
        _bufSize,                 // 地位文件大小
        BufName.c_str()          // 共享内存名称
    );

    _byteBuf = (char*)MapViewOfFile(
        MapFile,				 // 共享内存的句柄
        FILE_MAP_ALL_ACCESS,     // 可读写许可
        0,
        0,
        _bufSize
    );
    return true;
}

template <typename T> int ShareMem<T>::Close(void)
{
    UnmapViewOfFile(_byteBuf);
    CloseHandle(MapFile);
    return true;
}

template <typename T> T ShareMem<T>::ReadData()
{
    T tmpData;
    tmpData = *(T *)_byteBuf;
    return tmpData;
}

template <typename T> int ShareMem<T>::WriteData()
{
    memcpy(_byteBuf, &T, sizeof(T));
    _byteBuf[_bufSize] = '\0';
    return true;
}

template <typename T> void ShareMem<T>::LoadData()
{
 
}