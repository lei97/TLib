/*
@tlei 
data：2019/12/08
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
        void setMap();      // TODO:生成 map 数据，待整理
        bool update();      // TODO:配置文件更新
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
        ifs.open(m_fileName);   // 读取配置文件
        if (ifs.is_open() == NULL)
        {
            throw;      //TODO: 异常处理类待整理，暂定
        }
        ifs.seekg(0, std::ios::end); //设置文件指针到文件流的尾部
        std::streampos size = ifs.tellg(); //读取文件指针的位置
        ifs.seekg(0);
        ifs.read((char*)&m_fileData, size);     //TODO：文件流转字符串代完成 frist
        ifs.close();
    }

    void Config::parseData()
    {
        m_data = m_fileData;      //TODO
    }
}
#endif