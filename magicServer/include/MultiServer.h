#ifndef MULTISERVER_H_    
#define MULTISERVER_H_    

#include <stdio.h>    
#include <stdlib.h>    
#include <unistd.h>    
#include <string.h>    
#include <errno.h>    
#include <signal.h>    
#include <time.h>    
#include <pthread.h>    
#include <fcntl.h>    
#include <assert.h>  

#include <iostream>

using namespace std;
//#include <event.h>   
#include </usr/include/event.h>   
#include <event2/bufferevent.h>    
#include <event2/buffer.h>    
#include <event2/listener.h>    
#include <event2/util.h>    
#include <event2/event.h>    

class MultiServer;    
class Conn;    
class ConnQueue;    
struct LibeventThread;    

//�����һ������Ľ���࣬�����洢�������ӵ���Ϣ��    
//���ṩ�˶�д���ݵĽӿ�    
class Conn    
{    
	//����ֻ����TcpBaseServer������    
	//����ConnQueue�����    
	friend class ConnQueue;    
	friend class MultiServer;    

private:    
	const int m_fd;             //socket��ID    
	evbuffer *m_ReadBuf;        //�����ݵĻ�����    
	evbuffer *m_WriteBuf;       //д���ݵĻ�����    

	Conn *m_Prev;               //ǰһ������ָ��    
	Conn *m_Next;               //��һ������ָ��    
	LibeventThread *m_Thread;  

	string m_strReadData;
	int m_readDataCount;
	int m_protobufDataSize;

	Conn(int fd=0);    
	~Conn();    

public: 
	string getReadData() {return m_strReadData;}
	void appendReadData(const string& u_strData) {m_strReadData.append(u_strData.c_str(),u_strData.size());};
	int getReadDataCount() {return m_readDataCount;} 
	void setReadDataCount(const int& u_readDataCount) {m_readDataCount = u_readDataCount;}
	int getProtobufDatasize() {return m_protobufDataSize;}
	void setProtobufDataSize(const int& u_protobufDataSize) {m_protobufDataSize = u_protobufDataSize;}
	LibeventThread *GetThread() { return m_Thread; }    
	int GetFd() { return m_fd; }    

	//��ȡ�ɶ����ݵĳ���    
	int GetReadBufferLen()    
	{ return evbuffer_get_length(m_ReadBuf); }    

	//�Ӷ���������ȡ��len���ֽڵ����ݣ�����buffer�У����������������������    
	//���ض������ݵ��ֽ���    
	int GetReadBuffer(char *buffer, int len)    
	{ return evbuffer_remove(m_ReadBuf, buffer, len); }    

	//�Ӷ��������и��Ƴ�len���ֽڵ����ݣ�����buffer�У������������Ƴ���������    
	//���ظ��Ƴ����ݵ��ֽ���    
	//ִ�иò��������ݻ������ڻ������У�buffer�е�����ֻ��ԭ���ݵĸ���    
	int CopyReadBuffer(char *buffer, int len)    
	{ return evbuffer_copyout(m_ReadBuf, buffer, len); }    

	//��ȡ��д���ݵĳ���    
	int GetWriteBufferLen()    
	{ return evbuffer_get_length(m_WriteBuf); }    

	//�����ݼ���д��������׼������    
	int AddToWriteBuffer(char *buffer, int len)    
	{ return evbuffer_add(m_WriteBuf, buffer, len); }    

	//�����������е������ƶ���д������ 
	void MoveBufferData()    
	{ evbuffer_add_buffer(m_WriteBuf, m_ReadBuf);}    

};    

//��ͷβ����˫�����࣬ÿ�����洢һ�����ӵ�����    
class ConnQueue    
{    
private:    
	Conn *m_head;    
	Conn *m_tail;    
public:    
	ConnQueue();    
	~ConnQueue();    
	Conn *InsertConn(int fd, LibeventThread *t);    
	void DeleteConn(Conn *c);    
	//void PrintQueue();    
};    

//ÿ�����̵߳��߳���Ϣ    
struct LibeventThread    
{    
	pthread_t tid;              //�̵߳�ID    
	struct event_base *base;    //libevent���¼������    
	struct event notifyEvent;   //����������¼���    
	int notifyReceiveFd;        //����Ľ��ն�    
	int notifySendFd;           //�ܵ��ķ��Ͷ�    
	ConnQueue connectQueue;     //socket���ӵ�����    

	//��libevent���¼�������Ҫ�õ��ܶ�ص�����������ʹ����������thisָ��    
	//������������ʽ��TcpBaseServer����ָ�봫��ȥ    
	MultiServer *tcpConnect;  //TcpBaseServer���ָ��    
};    

class MultiServer    
{    
private:  
	static const int EXIT_CODE = -1;    
	static const int MAX_SIGNAL = 256;  

private:    
	int m_ThreadCount;                  //���߳���    
	int m_Port;                         //�����Ķ˿�    
	LibeventThread *m_MainBase;         //���̵߳�libevent�¼������    
	LibeventThread *m_Threads;          //�洢�������߳���Ϣ������    
	event *m_SignalEvents[MAX_SIGNAL];  //�Զ�����źŴ���    

private:    
	//��ʼ�����̵߳�����    
	void SetupThread(LibeventThread *thread);    

	//���̵߳����ź���    
	static void *WorkerLibevent(void *arg);    
	//�����߳��յ�����󣩣���Ӧ���̵߳Ĵ�����    
	static void ThreadProcess(int fd, short which, void *arg);
	/**
 * ��һ��socket���óɷ�����ģʽ
 */
	//int setnonblock(int fd);
	//��libevent�ص��ĸ�����̬����    
	static void ListenerEventCb(evconnlistener *listener, evutil_socket_t fd,    
		sockaddr *sa, int socklen, void *user_data);    
	static void ReadEventCb(struct bufferevent *bev, void *data);    
	static void WriteEventCb(struct bufferevent *bev,void *data);     
	static void CloseEventCb(struct bufferevent *bev, short events, void *data);   
	static void ProcDataAndSendToBev(struct bufferevent *&bev,const string& u_beProcedData);

protected:    
	//������麯����һ����Ҫ������̳У��������д������ҵ���    

	//�½����ӳɹ��󣬻���øú���    
	virtual void ConnectionEvent(Conn *conn) { }    

	//��ȡ�����ݺ󣬻���øú���    
	virtual void ReadEvent(Conn *conn) { }    

	//������ɹ��󣬻���øú�������Ϊ���������⣬���Բ�����ÿ�η��������ݶ��ᱻ���ã�    
	virtual void WriteEvent(Conn *conn) { }    

	//�Ͽ����ӣ��ͻ��Զ��Ͽ����쳣�Ͽ����󣬻���øú���    
	virtual void CloseEvent(Conn *conn, short events) { }    

public:    
	MultiServer(int count);    
	~MultiServer();    

	//���ü����Ķ˿ںţ��������Ҫ�������뽫������ΪEXIT_CODE    
	void SetPort(int port)    
	{ m_Port = port; }    

	//��ʼ�¼�ѭ��    
	bool StartRun();    
	//��tvʱ��������¼�ѭ��    
	//��tvΪ�գ�������ֹͣ    
	void StopRun(timeval *tv);    

	//��Ӻ�ɾ���źŴ����¼�    
	//sig���źţ�ptrΪҪ�ص��ĺ���    
	bool AddSignalEvent(int sig, void (*ptr)(int, short, void*));    
	bool DeleteSignalEvent(int sig);    

	//��Ӻ�ɾ����ʱ�¼�    
	//ptrΪҪ�ص��ĺ�����tv�Ǽ��ʱ�䣬once�����Ƿ�ִֻ��һ��    
	event *AddTimerEvent(void(*ptr)(int, short, void*),    
		timeval tv, bool once);    
	bool DeleteTImerEvent(event *ev);    
};    

#endif    