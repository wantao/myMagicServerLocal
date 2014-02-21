#include "../include/MultiServer.h"   
#include "../include/structDef.h"
#include "../include/procClientMessage.h"
#include "../include/myGlog.h"
 
Conn::Conn(int fd) : m_fd(fd)    
{    
    m_Prev = NULL;    
    m_Next = NULL;  
	m_strReadData = "";
	m_readDataCount = 1;
	m_protobufDataSize = 0;
}    
  
Conn::~Conn()    
{    
  
}    
  
ConnQueue::ConnQueue()    
{    
    //����ͷβ��㣬��������ָ��    
    m_head = new Conn(0);    
    m_tail = new Conn(0);    
    m_head->m_Prev = m_tail->m_Next = NULL;    
    m_head->m_Next = m_tail;    
    m_tail->m_Prev = m_head;    
}    
  
ConnQueue::~ConnQueue()    
{    
    Conn *tcur, *tnext;    
    tcur = m_head;    
    //ѭ��ɾ�������еĸ������    
    while( tcur != NULL )    
    {    
        tnext = tcur->m_Next;    
        delete tcur;    
        tcur = tnext;    
    }    
}    
  
Conn *ConnQueue::InsertConn(int fd, LibeventThread *t)    
{    
    Conn *c = new Conn(fd);    
    c->m_Thread = t;    
    Conn *next = m_head->m_Next;    
  
    c->m_Prev = m_head;    
    c->m_Next = m_head->m_Next;    
    m_head->m_Next = c;    
    next->m_Prev = c;    
    return c;    
}    
  
void ConnQueue::DeleteConn(Conn *c)    
{    
    c->m_Prev->m_Next = c->m_Next;    
    c->m_Next->m_Prev = c->m_Prev;
    delete c;    
}    
  
/*  
void ConnQueue::PrintQueue()  
{  
Conn *cur = m_head->m_Next;  
while( cur->m_Next != NULL )  
{  
printf("%d ", cur->m_fd);  
cur = cur->m_Next;  
}  
printf("\n");  
}  
*/    
  
MultiServer::MultiServer(int count)    
{    
    //��ʼ����������    
    m_ThreadCount = count;    
    m_Port = -1;    
    m_MainBase = new LibeventThread;    
    m_Threads = new LibeventThread[m_ThreadCount];    
    m_MainBase->tid = pthread_self();    
    m_MainBase->base = event_base_new();   
    memset(m_SignalEvents, 0, sizeof(m_SignalEvents));  
  
    //��ʼ���������̵߳Ľṹ��    
    for(int i=0; i<m_ThreadCount; i++)    
    {    
        SetupThread(&m_Threads[i]);    
    }    
  
}    
  
MultiServer::~MultiServer()    
{    
    //ֹͣ�¼�ѭ��������¼�ѭ��û��ʼ����ûЧ����    
    StopRun(NULL);    
  
    //�ͷ��ڴ�    
    event_base_free(m_MainBase->base);    
    for(int i=0; i<m_ThreadCount; i++)    
        event_base_free(m_Threads[i].base);    
  
    delete m_MainBase;    
    delete [] m_Threads;    
}    
  
void MultiServer::SetupThread(LibeventThread *me)    
{    
    int res;   
  
    //����libevent�¼��������    
    me->tcpConnect = this;    
    me->base = event_base_new();    
    assert( me->base != NULL );  
  
    //�����̺߳����߳�֮�佨���ܵ�    
    int fds[2];    
    res = pipe(fds);    
    assert( res == 0 );  
    me->notifyReceiveFd = fds[0];    
    me->notifySendFd = fds[1];    
  
    //�����̵߳�״̬�������ܵ�    
    event_set( &me->notifyEvent, me->notifyReceiveFd,    
        EV_READ | EV_PERSIST, ThreadProcess, me );    
    event_base_set(me->base, &me->notifyEvent);    
    res = event_add(&me->notifyEvent, 0);  
    assert( res == 0 );  
}    
  
void *MultiServer::WorkerLibevent(void *arg)    
{    
    //����libevent���¼�ѭ����׼������ҵ��    
    LibeventThread *me = (LibeventThread*)arg;    
    //printf("thread %u started\n", (unsigned int)me->tid);    
    event_base_dispatch(me->base);    
    //printf("subthread done\n");
	pthread_exit((void*)0);
}    
  
bool MultiServer::StartRun()    
{    
    evconnlistener *listener;    
  
    //����˿ںŲ���EXIT_CODE���ͼ����ö˿ں�    
    if( m_Port != EXIT_CODE )    
    {    
        sockaddr_in sin;    
        memset(&sin, 0, sizeof(sin));    
        sin.sin_family = AF_INET;    
        sin.sin_port = htons(m_Port);    
        listener = evconnlistener_new_bind(m_MainBase->base,     
            ListenerEventCb, (void*)this,    
            LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,    
            (sockaddr*)&sin, sizeof(sockaddr_in));    
        if( NULL == listener )  
        {  
            fprintf(stderr, "listen error: %s\n", strerror(errno));  
            exit(1);  
        }  
    }    
  
    //�����������߳�    
    for(int i=0; i<m_ThreadCount; i++)    
    {    
        pthread_create(&m_Threads[i].tid, NULL,      
            WorkerLibevent, (void*)&m_Threads[i]);    
    }    
  
    //�������̵߳��¼�ѭ��    
    event_base_dispatch(m_MainBase->base);    
  
    //�¼�ѭ��������ͷż����ߵ��ڴ�    
    if( m_Port != EXIT_CODE )    
    {    
        //printf("free listen\n");    
        evconnlistener_free(listener);    
    }    
}    
  
void MultiServer::StopRun(timeval *tv)    
{    
    int contant = EXIT_CODE;    
    //��������̵߳Ĺ�����д��EXIT_CODE��֪ͨ�����˳�    
    for(int i=0; i<m_ThreadCount; i++)    
    {    
        write(m_Threads[i].notifySendFd, &contant, sizeof(int));    
    }    
    //������̵߳��¼�ѭ��    
    event_base_loopexit(m_MainBase->base, tv);    
}    
  
void MultiServer::ListenerEventCb(struct evconnlistener *listener,     
                                     evutil_socket_t fd,    
struct sockaddr *sa,     
    int socklen,     
    void *user_data)    
{    
    MultiServer *server = (MultiServer*)user_data;    
  
    //���ѡ��һ�����̣߳�ͨ���ܵ����䴫��socket������    
    int num = rand() % server->m_ThreadCount;    
    int sendfd = server->m_Threads[num].notifySendFd;    
    write(sendfd, &fd, sizeof(evutil_socket_t));    
}    
  
void MultiServer::ThreadProcess(int fd, short which, void *arg)    
{ 
	//cerr<<"in ThreadId:"<<pthread_self()<<endl;
    LibeventThread *me = (LibeventThread*)arg;    
  
    //�ӹܵ��ж�ȡ���ݣ�socket��������������룩    
    int pipefd = me->notifyReceiveFd;    
    evutil_socket_t confd;    
    read(pipefd, &confd, sizeof(evutil_socket_t));    
  
    //�����������EXIT_CODE���������¼�ѭ��    
    if( EXIT_CODE == confd )    
    {    
        event_base_loopbreak(me->base);    
        return;    
    }    
  
    //�½�����    
    struct bufferevent *bev;    
    bev = bufferevent_socket_new(me->base, confd, BEV_OPT_CLOSE_ON_FREE);    
    if (!bev)    
    {    
        fprintf(stderr, "Error constructing bufferevent!");    
        event_base_loopbreak(me->base);    
        return;    
    }    
  
    //�������ӷ������    
    Conn *conn = me->connectQueue.InsertConn(confd, me);   
	//cerr<<"connect fd:"<<confd<<endl;
	//���������5s��û���¼���������ϵ�������
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	bufferevent_set_timeouts(bev, &tv, NULL);
  
    //׼����socket�ж�д����    
    bufferevent_setcb(bev, ReadEventCb, WriteEventCb, CloseEventCb, conn);    
    bufferevent_enable(bev, EV_WRITE); 
//#ifdef DEBUG_MOD
//	bufferevent_setwatermark(bev, EV_READ, 0, 102400);
//#endif
    bufferevent_enable(bev, EV_READ|EV_PERSIST);    
  
    //�����û��Զ���������¼�������    
    //me->tcpConnect->ConnectionEvent(conn);    
}    

void MultiServer::ProcDataAndSendToBev(struct bufferevent *&bev,const string& u_beProcedData)
{
	string u_retData = g_cProClientMessage.procClientReqData(u_beProcedData);
	//cerr<<"u_retData.size:"<<u_retData.size()<<endl;
	if (u_retData.size() > 0)
	{
		int nLength = u_retData.size();
		int nHasSend = 0;
		if (bufferevent_write(bev,u_retData.c_str()+nHasSend,nLength) != 0)
		{
			g_cMyGlog.errorlogRecord("ProcDataAndSendToBev bufferevent_write error,sendData.size:%d\n",nLength);
		}
		/*while (nLength > 0)
		{
			if (nLength >= 16384)
			{
				bufferevent_write(bev,u_retData.c_str()+nHasSend,16384);
				nLength -= 16384;
				nHasSend += 16384;
			}
			else
			{
				bufferevent_write(bev,u_retData.c_str()+nHasSend,nLength);
				nHasSend += nLength;
				nLength -= nLength;

			}

		}*/
	}
}
  
void MultiServer::ReadEventCb(struct bufferevent *bev, void *data)    
{    

	Conn *conn = (Conn*)data;
    conn->m_ReadBuf = bufferevent_get_input(bev);    
    conn->m_WriteBuf = bufferevent_get_output(bev);
	int u_readBufLength = evbuffer_get_length(conn->m_ReadBuf);

	//int u_writeBufLength = evbuffer_get_length(conn->m_WriteBuf);

//#ifdef DEBUG_MOD
	
	/*cerr<<"u_readBufLength:"<<u_readBufLength<<endl;
	cerr<<"u_writeBufLength:"<<u_writeBufLength<<endl;*/
//#endif

	int u_readDataCount = conn->getReadDataCount();
	//g_cMyGlog.debugLog("u_readdataCount:%d\n",u_readDataCount);
	string u_strClientData = "";
	if (u_readDataCount == 1)
	{
		dataPacket u_dataPacket;
		int nreadByte = bufferevent_read(bev,&u_dataPacket,sizeof(u_dataPacket));
		if (nreadByte != sizeof(u_dataPacket))
		{
			g_cMyGlog.errorlogRecord("bufferevent_read u_dataPacket error,nreadByte:%d\n",nreadByte);
			return;
		}
		//g_cMyGlog.debugLog("nreadByte:%d\n",nreadByte);
		u_strClientData.append((char*)&u_dataPacket,sizeof(u_dataPacket));
		if (u_dataPacket.protobufLength > 0)
		{
			char* protoBufData = NULL;
			protoBufData = (char*)malloc(sizeof(char)*(u_dataPacket.protobufLength+1));
			if (protoBufData != NULL)
			{
				nreadByte = bufferevent_read(bev,protoBufData,u_dataPacket.protobufLength);

				if (nreadByte == u_dataPacket.protobufLength)
				{
					u_strClientData.append(protoBufData,nreadByte);
					//cerr<<"u_strClientData.size equal:"<<u_strClientData.size()<<endl;
					//g_cMyGlog.debugLog("u_strClientData.size equal:%d\n",u_strClientData.size());
					free(protoBufData);
					protoBufData = NULL;
					ProcDataAndSendToBev(bev,u_strClientData);
					//g_cMyGlog.errorlogRecord("bufferevent_read protoBufData error:nhasreadByte:%d,realByte:%d\n",nreadByte,u_dataPacket.protobufLength);
					return;
				}
				else
				{
					u_strClientData.append(protoBufData,nreadByte);
					u_readDataCount += 1;
					conn->setReadDataCount(u_readDataCount);
					conn->appendReadData(u_strClientData);
					conn->setProtobufDataSize(u_dataPacket.protobufLength);
					free(protoBufData);
					protoBufData = NULL;
					//g_cMyGlog.errorlogRecord("bufferevent_read protoBufData :nhasreadByte:%d,realByte:%d\n",nreadByte,u_dataPacket.protobufLength);
					return;
				}
				
			}
			else
			{
				return;
			}
		}
		ProcDataAndSendToBev(bev,u_strClientData);
		//g_cMyGlog.debugLog("ProcDataAndSendToBev success when count = %d\n",u_readDataCount);
	}
	else
	{
		u_readBufLength = evbuffer_get_length(conn->m_ReadBuf);
		//g_cMyGlog.debugLog("u_readBufLength:%d\n",u_readBufLength);
		int nreadByte = 0;
		char* u_BufData = NULL;
		u_BufData = (char*)malloc(sizeof(char)*(u_readBufLength+1));
		if (u_BufData != NULL)
		{
			nreadByte = bufferevent_read(bev,u_BufData,u_readBufLength);
			u_strClientData.append(u_BufData,nreadByte);
			conn->appendReadData(u_strClientData);
			string n_finalDataTmp = conn->getReadData();
			int n_protobufDataSize = conn->getProtobufDatasize();
			//g_cMyGlog.debugLog("n_finalDataTmp.size:%d,n_protobufDataSize:%d\n",n_finalDataTmp.size(),n_protobufDataSize);
			if ((n_finalDataTmp.size() - sizeof(dataPacket)) == n_protobufDataSize)
			{
				ProcDataAndSendToBev(bev,n_finalDataTmp);
				//g_cMyGlog.debugLog("ProcDataAndSendToBev success when count = %d\n",u_readDataCount);
			}
			else
			{
				//u_strClientData.append(u_BufData,nreadByte);
				u_readDataCount += 1;
				conn->setReadDataCount(u_readDataCount);
				//conn->appendReadData(u_strClientData);
				//g_cMyGlog.errorlogRecord("bufferevent_read protoBufData :nhasreadByte:%d,count:%d\n",nreadByte,u_readDataCount);
			}
			free(u_BufData);
			u_BufData = NULL;
		}
		else
		{
			g_cMyGlog.errorlogRecord("u_BufData malloc failure\n");
		}

	}
	
	
	//cerr<<"u_retData:"<<u_retData<<endl;
	/*int u_writeBufLength = evbuffer_get_length(conn->m_WriteBuf);
	cerr<<"u_writeBufLength:"<<u_writeBufLength<<endl;*/

	//evutil_closesocket(conn->m_fd);
	
    //�����û��Զ���Ķ�ȡ�¼�������    
   // conn->m_Thread->tcpConnect->ReadEvent(conn);  
	//cerr<<"1111"<<endl;
}     
  
void MultiServer::WriteEventCb(struct bufferevent *bev,void *data)    
{   
	//cerr<<"MultiServer::WriteEventCb"<<endl;
    Conn *conn = (Conn*)data;    
   /* conn->m_ReadBuf = bufferevent_get_input(bev);    
    conn->m_WriteBuf = bufferevent_get_output(bev); 

	int u_readBufLength = evbuffer_get_length(conn->m_ReadBuf);

	int u_writeBufLength = evbuffer_get_length(conn->m_WriteBuf);*/
	//cerr<<"WriteEventCb u_readBufLength:"<<u_readBufLength<<endl;
	//cerr<<"WriteEventCb u_writeBufLength:"<<u_writeBufLength<<endl;
  
    //�����û��Զ����д���¼�������    
    conn->m_Thread->tcpConnect->WriteEvent(conn);    
  
}    
  
void MultiServer::CloseEventCb(struct bufferevent *bev, short events, void *data)    
{    
    Conn *conn = (Conn*)data;    
    //�����û��Զ���ĶϿ��¼�������    
    conn->m_Thread->tcpConnect->CloseEvent(conn, events);    
    conn->GetThread()->connectQueue.DeleteConn(conn);    
    bufferevent_free(bev); 
	//g_cMyGlog.debugLog("in CloseEventCb\n");
}    
  
bool MultiServer::AddSignalEvent(int sig, void (*ptr)(int, short, void*))    
{    
    if( sig >= MAX_SIGNAL )  
        return false;  
  
    //�½�һ���ź��¼�    
    event *ev = evsignal_new(m_MainBase->base, sig, ptr, (void*)this);    
    if ( !ev ||     
        event_add(ev, NULL) < 0 )    
    {    
        event_del(ev);    
        return false;    
    }    
  
    //ɾ���ɵ��ź��¼���ͬһ���ź�ֻ����һ���ź��¼���   
    if( NULL != m_SignalEvents[sig] )  
        DeleteSignalEvent(sig);    
    m_SignalEvents[sig] = ev;    
  
    return true;    
}    
  
bool MultiServer::DeleteSignalEvent(int sig)    
{    
    event *ev = m_SignalEvents[sig];  
    if( sig >= MAX_SIGNAL || NULL == ev )  
        return false;  
  
    event_del(ev);    
    ev = NULL;  
    return true;    
}    
  
event *MultiServer::AddTimerEvent(void (*ptr)(int, short, void *),     
                                     timeval tv, bool once)    
{    
    int flag = 0;    
    if( !once )    
        flag = EV_PERSIST;    
  
    //�½���ʱ���ź��¼�    
    event *ev = new event;    
    event_assign(ev, m_MainBase->base, -1, flag, ptr, (void*)this);    
    if( event_add(ev, &tv) < 0 )    
    {    
        event_del(ev);    
        return NULL;    
    }    
    return ev;    
}    
  
bool MultiServer::DeleteTImerEvent(event *ev)    
{    
    int res = event_del(ev);    
    return (0 == res);    
}    