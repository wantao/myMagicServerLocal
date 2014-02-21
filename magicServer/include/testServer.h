#ifndef TESTSERVER_H
#define TESTSERVER_H

#include "../include/MultiServer.h"  
#include <set>    
#include <vector>    
using namespace std; 
//测试示例    
class TestServer : public MultiServer    
{    
private:    
	vector<Conn*> vec;    
protected:    
	//重载各个处理业务的虚函数    
	void ReadEvent(Conn *conn);    
	void WriteEvent(Conn *conn);    
	void ConnectionEvent(Conn *conn);    
	void CloseEvent(Conn *conn, short events);    
public:    
	TestServer(int count) : MultiServer(count) { }    
	~TestServer() { } 
	bool Init();

	//退出事件，响应Ctrl+C    
	static void QuitCb(int sig, short events, void *data);    
	//定时器事件，每10秒向所有客户端发一句hello, world    
	static void TimeOutCb(int id, int short events, void *data);    
};  

//extern TestServer g_cTestServer;

#endif