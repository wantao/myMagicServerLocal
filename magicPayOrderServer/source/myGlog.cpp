#include <glog/logging.h>
#include <stdlib.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include "../include/myGlog.h"
using namespace std;

cMyGlog g_cMyGlog;

cMyGlog::cMyGlog()
{

}
cMyGlog::~cMyGlog()
{

}

bool cMyGlog::Init(char* argv[])
{
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::ERROR,"/home/wt/myGlog/");
	return true;
}

void cMyGlog::errorlogRecord(const char* format,...)
{
	va_list args;
	va_start(args,format);
	//多参数
	char szBuffer[1024];
	memset(szBuffer,0,sizeof(szBuffer));
	vsprintf(szBuffer,format,args);
	va_end(args);
	std::string log = szBuffer;
	LOG(ERROR) << log;
}

void cMyGlog::debugLog(const char* format,...)
{
#ifdef DEBUG_MOD
	va_list args;
	va_start(args,format);
	//多参数
	char szBuffer[1024];
	memset(szBuffer,0,sizeof(szBuffer));
	vsprintf(szBuffer,format,args);
	va_end(args);
	std::string log = szBuffer;
	LOG(ERROR) << log;
#endif
}

void cMyGlog::normalLog(const char* format,...)
{
	va_list args;
	va_start(args,format);
	//多参数
	char szBuffer[1024];
	memset(szBuffer,0,sizeof(szBuffer));
	vsprintf(szBuffer,format,args);
	va_end(args);
	std::string log = szBuffer;
	LOG(ERROR) << log;
}
