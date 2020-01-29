/****************************************************************
版权所有 (C), 2019-2021,tlei
*****************************************************************
*  文件名 : net.h
*  摘要 : 网络实现
*  当前版本 : 1.0
*          : 基本完成实现构建 ，待验证测试
*  TODO:   实现跨平台，功能整理 
*  作    者   : tlei
*  日    期   : 2020/01/29
*  
*****************************************************************/
#ifndef TLIB_BASE_NET_H
#define TLIB_BASE_NET_H

#ifdef _WIN32
#include <Winsock2.h>
#endif
#include <iostream>

namespace Tlib {

    using std::cout;
    using std::endl;
    using std::string;

    const int RECV_DATA_LEN = 2048;
    const int PORT = 61557;                  //指定数据池服务器端口号

    class Net {
    public:
        enum class Type {       // 用户端类型
            TCP_Client,
            TCP_Server,
            UDP
        };

        Net() = delete;
        Net& operator= (const Net& net) =delete;    //赋值构造函数
        Net(Net& net) = delete;                     //拷贝构造函数
        Net(const Net& net) = delete;

        Net(Type type, int port, string address) : _type(type), _port(port), _address(address)
        {
            switch (_type)
            {
            case Type::TCP_Client:
                InitTcpClient();
                break;
            case Type::TCP_Server:
                InitTcpServer();
                break;
            case Type::UDP:
                InitUdp();
                break;
            default:
                throw;
            }
            m_pRecvData = new char[RECV_DATA_LEN];    // 开辟空间
            if (m_pRecvData == NULL)
            {
                cout << "pszRecv new char Error " << endl;
            }
        };
        
        ~Net()
        {
            delete m_pRecvData;
            m_pRecvData = nullptr;
        }

        void Close();

    private:
        bool InitTcpClient();
        bool InitTcpServer();
        bool InitUdp();

        bool RecvData();

        string GetData();

        bool SendData(string data);

    private:
        Type _type;
        int _port;
        string _address;
        WSAData m_wsd;                        //初始化信息  
        SOCKET m_SoRecv;                      //接收SOCKET
        SOCKADDR_IN m_SiRemote, m_SiLocal;    //远程发送机地址和本机接收机地址  
        char* m_pRecvData;
        int m_RecvNum;
        string m_StrRecvData;
#if 1
        SOCKADDR_IN m_cAddr = { 0 };
        SOCKET m_clientSocket;
#endif
    };
}
#endif

namespace Tlib {
    bool Net::InitTcpClient()
    {
        //1、启动Winsock
        if (WSAStartup(MAKEWORD(2, 2), &m_wsd) != 0)
        {
            cout << "WSAStartup Error = " << WSAGetLastError() << endl;
            return 0;
        }

        //2、创建socket
        m_SoRecv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (m_SoRecv == SOCKET_ERROR)
        {
            cout << "socket Error = " << WSAGetLastError() << endl;
            return 0;
        }
        //3、设置端口号、绑定端口号
        int nPort = _port;
        m_SiLocal.sin_family = AF_INET;
        m_SiLocal.sin_port = htons(nPort);
        m_SiLocal.sin_addr.s_addr = inet_addr(_address.c_str());   //UDP远程端口号

        m_clientSocket = m_SoRecv;
        if (connect(m_SoRecv, (sockaddr*)&m_SiLocal, sizeof(m_SiLocal)) == SOCKET_ERROR)
        {
            cout << "connect error = " << WSAGetLastError() << endl;
            return false;
        }
        return true;
    }

    bool Net::InitTcpServer()
    {
        //1、启动Winsock
        if (WSAStartup(MAKEWORD(2, 2), &m_wsd) != 0)
        {
            cout << "WSAStartup Error = " << WSAGetLastError() << endl;
            return 0;
        }
        //2、创建socket
        m_SoRecv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_SoRecv == SOCKET_ERROR)
        {
            cout << "socket Error = " << WSAGetLastError() << endl;
            return 0;
        }
        //3、设置端口号、绑定端口号
        int nPort = _port;
        m_SiLocal.sin_family = AF_INET;
        m_SiLocal.sin_port = htons(nPort);
        m_SiLocal.sin_addr.s_addr = inet_addr(_address.c_str());   

        //监听 Tcp-Server
        if (listen(m_SoRecv, SOMAXCONN) == SOCKET_ERROR)
        {
            cout << "listen Error = " << WSAGetLastError() << endl;
        }
        //接受客户端连接
        int size = sizeof m_cAddr;
        m_clientSocket = accept(m_SoRecv, (sockaddr*)&m_cAddr, &size);
        cout << "init is ok" << endl;

        return true;
    }

    bool Net::InitUdp()
    {
        //1、启动Winsock
        if (WSAStartup(MAKEWORD(2, 2), &m_wsd) != 0)
        {
            cout << "WSAStartup Error = " << WSAGetLastError() << endl;
            return 0;
        }
        //2、创建socket
        m_SoRecv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (m_SoRecv == SOCKET_ERROR)
        {
            cout << "socket Error = " << WSAGetLastError() << endl;
            return 0;
        }
        //3、设置端口号、绑定端口号
        int nPort = _port;
        m_SiLocal.sin_family = AF_INET;
        m_SiLocal.sin_port = htons(nPort);

       //4、绑定本地地址到socket
        if (bind(m_SoRecv, (SOCKADDR*)&m_SiLocal, sizeof(m_SiLocal)) == SOCKET_ERROR)
        {
            cout << "bind Error = " << WSAGetLastError() << endl;
            return false;
        }

        return true;
    }

    bool Net::RecvData()
    {
        int RecSize = 0;
        RecSize = sizeof(m_SiRemote);
        switch (_type)
        {
        case Type::TCP_Client:
        case Type::TCP_Server:
            m_RecvNum = recv(m_clientSocket, m_pRecvData, RECV_DATA_LEN, NULL);
            break;
        case Type::UDP:
            m_RecvNum = recvfrom(m_SoRecv, m_pRecvData, RECV_DATA_LEN, 0, (SOCKADDR*)&m_SiRemote, &RecSize);
            break;
        default:
            break;
        }

        if (m_RecvNum == SOCKET_ERROR)
        {
            cout << "recvfrom Error " << WSAGetLastError() << endl;
            return false;
        }
        else if (m_RecvNum == 0)
        {
            return 0;
        }
        else if (m_RecvNum >= RECV_DATA_LEN)                   //数据截断 20190306  tlei
        {
            m_pRecvData[RECV_DATA_LEN - 1] = '\0';
        }
        else
        {
            m_pRecvData[m_RecvNum] = '\0';
        }
        return 1;
    }

    bool Net::SendData(string data)
    {
        int SendSize = 0;
        SendSize = sizeof(m_SiRemote);
        int Len = data.length();

        int num = 0;
        switch (_type)
        {
        case Type::TCP_Client:
        case Type::TCP_Server:
            num = send(m_clientSocket, data.c_str(), Len, NULL);
            break;
        case Type::UDP:
            num = sendto(m_SoRecv, data.c_str(), Len, 0, (SOCKADDR*)&m_SiRemote, SendSize);
            break;
        default:
            break;
        }

        if (num == SOCKET_ERROR)
        {
            cout << "Send Error " << WSAGetLastError() << endl;
            return false;
        }
        return true;
    }

    string Net::GetData(void)
    {
        m_StrRecvData = m_pRecvData;
        return m_StrRecvData;
    }

    void Net::Close()
    {
        closesocket(m_SoRecv);
        WSACleanup();
        cout << "close udp server ..." << endl;
    }
}