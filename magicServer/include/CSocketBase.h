
#ifndef CSOCKETBASE_H
#define CSOCKETBASE_H

#include "global.h"
#include <iostream>

using namespace std; 

class mvCSocketBase
{
    public:
        /*构造函数*/
        mvCSocketBase();
        /*析构函数*/
        ~mvCSocketBase();

    public:
        /*创建套接字*/
        bool    mvCreateSocket(unsigned int &nSocket, int nType = 1);  //1:tcp; 2:udp
        /*为套接字绑定端口*/
        bool    mvBindPort(unsigned int& nSocket, int nPort);
        /*设置套接字属性*/
        bool    mvSetSocketOpt(unsigned int& nSocket, int nOpt);
        /*开始监听*/
        bool    mvStartListen(unsigned int& nSocket);
        /*主动连接*/
        bool    mvWaitConnect(unsigned int& nSocket, const string& strHost, int nPort, int nSec = 0);
        /*发送消息*/
        bool    mvSendMsgToSocket(unsigned int& nSocket, const string& strMsg,bool bBlock = false);
        /*关闭套接字*/
        void    mvCloseSocket(unsigned int &nSocket);
        /*回收线程*/
        void    mvJoinThread(pthread_t& uThreadId);
        /*设置套接字是否阻塞*/
        void    mvSetSocketBlock(unsigned int &nSocket, bool bBlock = true);
        /*准备套接字*/
        bool    mvPrepareSocket(unsigned int& nSocket);

    protected:
        /*等待套接字可写*/
        bool    mvWaitToWriteSocket(unsigned int& nSocket);

    private:
        pthread_mutex_t m_SendMutex;
};

extern mvCSocketBase g_cSockBase;

#endif
