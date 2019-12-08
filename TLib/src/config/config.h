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
#ifdef _WIN32
#endif
#include <json.h>

using json = nlohmann::json;

namespace tlib {
    using std::string;
    using std::map;
    class Config {
    public:
        Config();
        json get();
    private:
        void readFile();
        json m_data;
        string m_fileName;
    };
}

namespace tlib {
    Config::Config()
    {

    } 

    json Config::get()
    {
        return m_data;
    }

    void Config::readFile()
    {
        std::ifstream ifs;
        ifs.open(m_fileName);// 读取配置文件
        if (ifs.is_open() == NULL)
        {
            throw;      //TODO: 异常处理类待整理，暂定
        }
        string data;
        while (!ifs.eof())
        {
            ifs >> data;
        }
    }
}
#endif