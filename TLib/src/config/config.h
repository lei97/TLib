/*
@tlei 
data��2019/12/08
function: creat Config of class
    1. use json data 
*/
#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <json.h>

using json = nlohmann::json;

namespace tlib {
    using std::string;
    using std::map;
    class Config {
    public:
        Config() = delete;
        Config(string fileName);
        ~Config();
    public:
        json get();
        void setMap();      // TODO:���� map ���ݣ�������
        bool update();      // TODO:�����ļ�����
    private:
        void readFile();
        void parseData();
        json m_data;
        string m_fileName;
        string m_fileData;
    };
}

namespace tlib {
    Config::Config(string filename):m_fileName(filename)
    {
        readFile();
        parseData();
    } 

    Config::~Config()
    {
        
    }

    json Config::get()
    {
        return m_data;
    }

    void Config::readFile()
    {
        std::ifstream ifs;
        ifs.open(m_fileName);   // ��ȡ�����ļ�
        if (ifs.is_open() == NULL)
        {
            throw;      //TODO: �쳣������������ݶ�
        }
        ifs.seekg(0, std::ios::end); //�����ļ�ָ�뵽�ļ�����β��
        std::streampos size = ifs.tellg(); //��ȡ�ļ�ָ���λ��
        ifs.seekg(0);
        ifs.read((char*)&m_fileData, size);     //TODO���ļ���ת�ַ�������� frist
        ifs.close();
    }

    void Config::parseData()
    {
        m_data = m_fileData;      //TODO
    }
}
#endif