#ifndef TESTSERVER_H
#define TESTSERVER_H

#include "../include/MultiServer.h"  
#include <set>    
#include <vector>    
using namespace std; 
//����ʾ��    
class TestServer : public MultiServer    
{    
private:    
	vector<Conn*> vec;    
protected:    
	//���ظ�������ҵ����麯��    
	void ReadEvent(Conn *conn);    
	void WriteEvent(Conn *conn);    
	void ConnectionEvent(Conn *conn);    
	void CloseEvent(Conn *conn, short events);    
public:    
	TestServer(int count) : MultiServer(count) { }    
	~TestServer() { } 
	bool Init();

	//�˳��¼�����ӦCtrl+C    
	static void QuitCb(int sig, short events, void *data);    
	//��ʱ���¼���ÿ10�������пͻ��˷�һ��hello, world    
	static void TimeOutCb(int id, int short events, void *data);    
};  

//extern TestServer g_cTestServer;

#endif