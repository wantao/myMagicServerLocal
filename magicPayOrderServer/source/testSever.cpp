    /*  
    这是一个测试用的服务器，只有两个功能：  
    1：对于每个已连接客户端，每10秒向其发送一句hello, world  
    2：若客户端向服务器发送数据，服务器收到后，再将数据回发给客户端  
    */    
    //test.cpp    
#include "../include/testServer.h"     
      
	//TestServer g_cTestServer;
    void TestServer::ReadEvent(Conn *conn)    
    {    
        conn->MoveBufferData();    
    }    
      
    void TestServer::WriteEvent(Conn *conn)    
    {    
      
    }    
      
    void TestServer::ConnectionEvent(Conn *conn)    
    {    
        TestServer *me = (TestServer*)conn->GetThread()->tcpConnect;    
        //printf("new connection: %d\n", conn->GetFd());    
        //me->vec.push_back(conn);    
    }    
      
    void TestServer::CloseEvent(Conn *conn, short events)    
    {    
        //printf("connection closed: %d\n", conn->GetFd());
    }    
      
    void TestServer::QuitCb(int sig, short events, void *data)    
    {     
        printf("Catch the SIGINT signal, quit in one second\n");    
        TestServer *me = (TestServer*)data;    
        timeval tv = {1, 0};    
        me->StopRun(&tv);    
    }    
      
    void TestServer::TimeOutCb(int id, short events, void *data)    
    {    
        TestServer *me = (TestServer*)data;    
        char temp[33] = "hello, world\n"; 
		cerr<<"size:"<<(me->vec).size()<<endl;
        for(int i=0; i<me->vec.size(); i++)    
            me->vec[i]->AddToWriteBuffer(temp, strlen(temp));    
    } 

	bool TestServer::Init()
	{
		printf("pid: %d\n", getpid());    
		TestServer server(3);    
		server.AddSignalEvent(SIGINT, TestServer::QuitCb);    
		//timeval tv = {10, 0};    
		//server.AddTimerEvent(TestServer::TimeOutCb, tv, false);    
		server.SetPort(2111);    
		server.StartRun();    
		printf("done\n");
		return true;
	}
      
    /*int main()    
    {    
        printf("pid: %d\n", getpid());    
        TestServer server(3);    
        server.AddSignalEvent(SIGINT, TestServer::QuitCb);    
        timeval tv = {10, 0};    
        server.AddTimerEvent(TestServer::TimeOutCb, tv, false);    
        server.SetPort(2111);    
        server.StartRun();    
        printf("done\n");    
      
        return 0;    
    }    */
