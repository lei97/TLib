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
    ShareMem();       //����
    ~ShareMem();      //����
public:
    int Init(string bufName);   //��ʼ��
    int Close(void);			//�ر�
    T ReadData();	    //��ȡ����
    void LoadData();			//��ʵ������ѹ��Buf�ṹ��
    int WriteData();			//д������
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
    // ���������ļ����
    MapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // �����ļ����
        NULL,                    // Ĭ�ϰ�ȫ����
        PAGE_READWRITE,          // �ɶ���д
        0,                       // ��λ�ļ���С
        _bufSize,                 // ��λ�ļ���С
        BufName.c_str()          // �����ڴ�����
    );

    _byteBuf = (char*)MapViewOfFile(
        MapFile,				 // �����ڴ�ľ��
        FILE_MAP_ALL_ACCESS,     // �ɶ�д���
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