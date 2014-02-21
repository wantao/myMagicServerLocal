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
////����������ͻ��˵�socket�ɶ�ʱ��libevent����
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
//	////�߳�id
//	//pthread_t id;
//	////�߳�����
//	//pthread_attr_t   attr;
//	////��ʼ��
//	//pthread_attr_init(&attr);
//	////�����߳�
//	//pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
//
//	////�����¼�����߳�
//	//int nret=pthread_create(&id,&attr,procClientReq,(void*)&u_myClientEvent);
//	////�ɹ�
//	//if(nret!=0)
//	//{
//	//	//ʧ��
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
//   ����һ����������׼��������ʱ�������������libevent���ò����ݸ���������: 
//   int fd:�����¼����ļ�������. 
//   short event:�����¼�������EV_TIMEOUT,EV_SIGNAL, EV_READ, or EV_WRITE. 
//   void* :��arg����ָ���ı���. 
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
//    //����������������л��������Ϣ�������µ��׽��֣������ظ��׽��ֵ��ļ���������
//    //�´������׽������ڷ�������ͻ�����ͨ�ţ���ԭ�����׽�����Ȼ���ڼ���״̬��
//    //�ú����ĵ�һ������ָ�����ڼ���״̬�����׽���
//    cfd = accept(fd, (struct sockaddr *)&addr, &addrlen);
//    if (cfd == -1) {
//        printf("accept(): can not accept client connection");
//        return;
//    }
//	unsigned int u_fd = cfd;
//	g_cSockBase.mvSetSocketBlock(u_fd, false);
//	//cerr<<"client fd connect:"<<cfd<<endl;
//    //������ĳ���׽��ֹ�����ѡ��
//    //������ IPPROTO_TCP:TCPѡ��
//    //������ TCP_NODELAY ��ʹ��Nagle�㷨 ѡ�������������ݶ����ǵȴ��������������Ȼ����һ�η���
//    // �������TCP_NODELAY �� TCP_CORK
//    //������ ��ѡ��TCP_NODELAY��ֵ
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
//	//��������˼����׽���
//	if(!g_cSockBase.mvCreateSocket(m_serverFd,1))
//	{
//		cerr<<"mvCreateSocket failure"<<endl;
//		cerr<<"err:"<<strerror(errno)<<endl;
//		return false;
//	}
//
//	//�����׽���Ϊ������
//	g_cSockBase.mvSetSocketBlock(m_serverFd,false);
//
//	//�׽����ظ�ʹ��
//	if(!g_cSockBase.mvSetSocketOpt(m_serverFd,SO_REUSEADDR))
//	{
//		cerr<<"mvSetSocketOpt failure"<<endl;
//		cerr<<"err:"<<strerror(errno)<<endl;
//		return false;
//	}
//
//	//���ý��ռ����ͻ���
//	//.....
//
//	//�׽�����˿ڰ�
//	if(!g_cSockBase.mvBindPort(m_serverFd, SERVER_PORT))
//	{
//		cerr<<"mvBindPort "<<SERVER_PORT<<" failure"<<endl;
//		cerr<<"err:"<<strerror(errno)<<endl;
//		return false;
//	}
//	
//	//��������
//	if (!g_cSockBase.mvStartListen(m_serverFd))
//	{
//		cerr<<"mvStartListen failure"<<endl;
//		cerr<<"err:"<<strerror(errno)<<endl;
//		return false;
//	}
//
//
//	/*
//     event_set�Ĳ�����
//     + ����1: ΪҪ������event
//     + ����2: file descriptor����������ʱ������������Ϊ-1������evtimer_set���������
//     + ����3: ����event���࣬���õ�EV_READ, EV_WRITE, EV_PERSIST, EV_SIGNAL, EV_TIMEOUT������ʱ�����øò���Ϊ0
//     + ����4: event������֮�󴥷���callback����
//     + ����5: ���ݸ�callback�����Ĳ���
//     ��ע��
//            �����ʼ��event��ʱ��������Ϊpersistent��(������EV_PERSIST)��
//            ��ʹ��event_add������ӵ������¼����Ϻ�(pending״̬)��
//            ��event���������pending״̬������event�������޴βμ�libevent���¼�������
//            ÿ���䱻�����callback����ִ��֮�󣬸�event�Զ���activeת��Ϊpending״̬��
//            �����μ�libevent������(�������������㣬�ֿ��Լ���ִ����callback)��
//            �����ڴ�����ʹ��event_del()��������event��libevent�������¼�������ɾ����
//            �����ͨ������EV_PERSISTʹ��event��persistent�ģ���Ҫ��event��callback���ٴε���event_add
//            (����ÿ��pending��Ϊactive֮����callback���ٽ�������Ϊpending)
//     */
//    event_set(&m_ServerEvent, m_serverFd, EV_READ | EV_PERSIST, ServerAccept, NULL);
//    //��event��ӵ�libevent�������¼�����
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
//	event_init(); //��ʼ��event baseʹ��Ĭ�ϵ�ȫ��current_base
//
//	if (!g_cListenClientConnect.NewSocket())
//	{
//		exit(-1);
//	}
//	event_dispatch(); //�����¼�����ϵͳ����ʼ�����������ܣ�����
//
//	pthread_exit((void*)0);
//}
//
//bool cListenClientConnect::Init()
//{
//	 
//	//�������������߳�
//	//�߳�id
//	pthread_t id;
//	//�߳�����
//	pthread_attr_t   attr;
//	//��ʼ��
//	pthread_attr_init(&attr);
//	//�����߳�
//	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
//
//	//�����¼�����߳�
//	int nret=pthread_create(&id,&attr,beginLibEvent,NULL);
//	//�ɹ�
//	if(nret!=0)
//	{
//		//ʧ��
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