//#include "../include/listenClientConnect.h"
//#include "../include/CSocketBase.h"
//#include "../include/procClientMessage.h"
//
//
//#include <netinet/in.h>
//#include <netinet/tcp.h>
//#include <unistd.h>
//#include <arpa/inet.h>
//
//cListenClientConnect g_cListenClientConnect;
//
//
//typedef struct _myClientEvent 
//{
//	struct client *client;
//	unsigned int fd;
//	_myClientEvent()
//	{
//		client = NULL;
//		fd = 0;
//	}
//}myClientEvent;
//
//cListenClientConnect::cListenClientConnect()
//{
//	m_serverFd = 0;
//}
//
//cListenClientConnect::~cListenClientConnect()
//{
//
//}
//
//void* procClientReq(void* pArg)
//{
//	myClientEvent u_myClientEvent = *((myClientEvent*)pArg);
//	g_cProClientMessage.recvClientMessageAndSend(u_myClientEvent.fd);
//	printf("u_myClientEvent:%p\n",&u_myClientEvent);
//	cerr<<"fd:"<<u_myClientEvent.fd<<endl;
//	printf("u_myClientEvent.client:%p\n",&(u_myClientEvent.client));
//	printf("((u_myClientEvent.client)->ev_read):%p\n",&((u_myClientEvent.client)->ev_read));
//	g_cSockBase.mvCloseSocket(u_myClientEvent.fd);
//	event_del(&((u_myClientEvent.client)->ev_read));
//	free(u_myClientEvent.client);
//	pthread_exit((void*)0);
//}
//
//
////这个函数当客户端的socket可读时由libevent调用
//void ServerRead(int fd, short ev, void *arg)
//{
//    struct client *client = (struct client *)arg;
//  
//	unsigned int cfd = fd;
//	//cerr<<"1111"<<endl;
//	//myClientEvent u_myClientEvent;
//	//u_myClientEvent.client = client;
//	//u_myClientEvent.fd = cfd;
//
//	////线程id
//	//pthread_t id;
//	////线程属性
//	//pthread_attr_t   attr;
//	////初始化
//	//pthread_attr_init(&attr);
//	////分离线程
//	//pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
//
//	////启动事件监控线程
//	//int nret=pthread_create(&id,&attr,procClientReq,(void*)&u_myClientEvent);
//	////成功
//	//if(nret!=0)
//	//{
//	//	//失败
//	//	cerr<<"pthread procClientReq failure"<<endl;
//	//	cerr<<"errno:"<<errno<<" err:"<<strerror(errno)<<endl;
//	//	pthread_attr_destroy(&attr);
//	//	return;
//	//}
//	//pthread_attr_destroy(&attr);
//
//	g_cProClientMessage.recvClientMessageAndSend(cfd);
//	
//	g_cSockBase.mvCloseSocket(cfd);
//	event_del(&client->ev_read);
//	free(client);
//	
//    return;
//}
//
//
////static void timeout_cb(int fd, short event, void *arg)
////{
////	struct client *client = (struct client *)arg;
////	unsigned int cfd = fd;
////	cerr<<"time up delete fd:"<<fd<<endl;
////	g_cSockBase.mvCloseSocket(cfd);
////	event_del(&client->ev_read);
////	free(client);
////}
//
///*
//   当有一个连接请求准备被接受时，这个函数将被libevent调用并传递给三个变量: 
//   int fd:触发事件的文件描述符. 
//   short event:触发事件的类型EV_TIMEOUT,EV_SIGNAL, EV_READ, or EV_WRITE. 
//   void* :由arg参数指定的变量. 
//*/
//void ServerAccept(int fd, short ev, void *arg)
//{
//    int cfd;
//    struct sockaddr_in addr;
//    socklen_t addrlen = sizeof(addr);
//    int yes = 1;
//
//	struct client *client;
//
//    //将从连接请求队列中获得连接信息，创建新的套接字，并返回该套接字的文件描述符。
//    //新创建的套接字用于服务器与客户机的通信，而原来的套接字仍然处于监听状态。
//    //该函数的第一个参数指定处于监听状态的流套接字
//    cfd = accept(fd, (struct sockaddr *)&addr, &addrlen);
//    if (cfd == -1) {
//        printf("accept(): can not accept client connection");
//        return;
//    }
//	unsigned int u_fd = cfd;
//	g_cSockBase.mvSetSocketBlock(u_fd, false);
//	//cerr<<"client fd connect:"<<cfd<<endl;
//    //设置与某个套接字关联的选项
//    //参数二 IPPROTO_TCP:TCP选项
//    //参数三 TCP_NODELAY 不使用Nagle算法 选择立即发送数据而不是等待产生更多的数据然后再一次发送
//    // 更多参数TCP_NODELAY 和 TCP_CORK
//    //参数四 新选项TCP_NODELAY的值
//    if (setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
//        printf("setsockopt(): TCP_NODELAY %s\n", strerror(errno));
//        close(cfd);
//        return;
//    }
//
//	client = (struct client *)calloc(1, sizeof(*client));
//	if (client == NULL)
//		printf("malloc failed\n");
//	//test
//	/*struct event timeout;
//	struct timeval tv;
//
//	evtimer_set(&timeout, timeout_cb, &timeout);
//	evutil_timerclear(&tv);
//	tv.tv_sec = 2;
//	event_add(&timeout, &tv);*/
//	//test
//	event_set(&client->ev_read, cfd, EV_READ|EV_PERSIST, ServerRead,
//		client);
//	event_add(&client->ev_read, NULL);
//	//printf("Accepted connection from %s\n", inet_ntoa(addr.sin_addr));
//}
//
//int cListenClientConnect::NewSocket()
//{
//	//创建服务端监听套接字
//	if(!g_cSockBase.mvCreateSocket(m_serverFd,1))
//	{
//		cerr<<"mvCreateSocket failure"<<endl;
//		cerr<<"err:"<<strerror(errno)<<endl;
//		return false;
//	}
//
//	//设置套接字为非阻塞
//	g_cSockBase.mvSetSocketBlock(m_serverFd,false);
//
//	//套接字重复使用
//	if(!g_cSockBase.mvSetSocketOpt(m_serverFd,SO_REUSEADDR))
//	{
//		cerr<<"mvSetSocketOpt failure"<<endl;
//		cerr<<"err:"<<strerror(errno)<<endl;
//		return false;
//	}
//
//	//设置接收及发送缓存
//	//.....
//
//	//套接字与端口绑定
//	if(!g_cSockBase.mvBindPort(m_serverFd, SERVER_PORT))
//	{
//		cerr<<"mvBindPort "<<SERVER_PORT<<" failure"<<endl;
//		cerr<<"err:"<<strerror(errno)<<endl;
//		return false;
//	}
//	
//	//监听连接
//	if (!g_cSockBase.mvStartListen(m_serverFd))
//	{
//		cerr<<"mvStartListen failure"<<endl;
//		cerr<<"err:"<<strerror(errno)<<endl;
//		return false;
//	}
//
//
//	/*
//     event_set的参数：
//     + 参数1: 为要创建的event
//     + 参数2: file descriptor，创建纯计时器可以设置其为-1，即宏evtimer_set定义的那样
//     + 参数3: 设置event种类，常用的EV_READ, EV_WRITE, EV_PERSIST, EV_SIGNAL, EV_TIMEOUT，纯计时器设置该参数为0
//     + 参数4: event被激活之后触发的callback函数
//     + 参数5: 传递给callback函数的参数
//     备注：
//            如果初始化event的时候设置其为persistent的(设置了EV_PERSIST)，
//            则使用event_add将其添加到侦听事件集合后(pending状态)，
//            该event会持续保持pending状态，即该event可以无限次参加libevent的事件侦听。
//            每当其被激活触发callback函数执行之后，该event自动从active转回为pending状态，
//            继续参加libevent的侦听(当激活条件满足，又可以继续执行其callback)。
//            除非在代码中使用event_del()函数将该event从libevent的侦听事件集合中删除。
//            如果不通过设置EV_PERSIST使得event是persistent的，需要在event的callback中再次调用event_add
//            (即在每次pending变为active之后，在callback中再将其设置为pending)
//     */
//    event_set(&m_ServerEvent, m_serverFd, EV_READ | EV_PERSIST, ServerAccept, NULL);
//    //将event添加到libevent侦听的事件集中
//    if (event_add(&m_ServerEvent, 0) == -1) {
//        printf("event_add(): can not add accept event into libevent");
//        close(m_serverFd);
//        return false;
//    }
//	return true;
//
//}
//
//void* beginLibEvent(void* pArg)
//{
//	event_init(); //初始化event base使用默认的全局current_base
//
//	if (!g_cListenClientConnect.NewSocket())
//	{
//		exit(-1);
//	}
//	event_dispatch(); //启动事件队列系统，开始监听（并接受）请求
//
//	pthread_exit((void*)0);
//}
//
//bool cListenClientConnect::Init()
//{
//	 
//	//启动接收连接线程
//	//线程id
//	pthread_t id;
//	//线程属性
//	pthread_attr_t   attr;
//	//初始化
//	pthread_attr_init(&attr);
//	//分离线程
//	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
//
//	//启动事件监控线程
//	int nret=pthread_create(&id,&attr,beginLibEvent,NULL);
//	//成功
//	if(nret!=0)
//	{
//		//失败
//		cerr<<"pthread beginLibEvent failure"<<endl;
//		cerr<<"errno:"<<errno<<" err:"<<strerror(errno)<<endl;
//		return false;
//	}
//	pthread_attr_destroy(&attr);
//	return true;
//}
//
//void cListenClientConnect::unInit()
//{
//
//}