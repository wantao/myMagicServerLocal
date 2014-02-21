#include "../include/procUserResourceDownLoad.h"
#include "../include/common.h"
#include "../include/magicServerDb.h"
#include "../include/global.h"
#include "../include/define.h"
#include "../include/errorCode.h"
#include "../include/structDef.h"
#include "../include/myGlog.h"
//#include <iconv.h>
//#include <memory.h>


cProcUserResourceDownLoad g_cProcUserResourceDownLoad;

cProcUserResourceDownLoad::cProcUserResourceDownLoad()
{
	
}


cProcUserResourceDownLoad::~cProcUserResourceDownLoad()
{

}

//加载初始资源数据
void cProcUserResourceDownLoad::InitLoadResourceData()
{
	string strSql = "select resourceVersionName,resourcePath from resourceVersionInfo";
	MysqlQuery q = g_cMagicServerDbRead.master_slave_exQuery(strSql);
	while (!q.eof())
	{
		if ((q.getStringFileds("resourcePath")).size() > 0)
		{
			//string u_resourceData = GetFileDataByPath(q.getStringFileds("resourcePath"));
			string u_resourcePath = q.getStringFileds("resourcePath");
			if (u_resourcePath.size() > 0)
			{
				g_cMyGlog.errorlogRecord("InitLoadResourceData\n");
				g_resourceDataMap.insert(make_pair(q.getStringFileds("resourceVersionName"),u_resourcePath));
			}
		}
		q.nextRow();
	}
	q.finalize();
	return;
}

//int cProcUserResourceDownLoad::convertGbk2Utf(string& str)  
//{  
//	iconv_t     gbk2UtfDescriptor;  
//	gbk2UtfDescriptor = iconv_open("UTF-8", "GBK");  
//	//size_t inlen = instr.length();  
//	//char* in = const_cast<char*>(instr.c_str());  
//	//size_t outlen = inlen * 2 + 1; // inlen * 1.5 + 1 >= outlen >= inlen + 1  
//	//char* outbuf = (char*)::malloc(outlen);  
//	//char* out = outbuf;  
//	//memset(outbuf, 0x0, outlen);  
//	//if((size_t)-1 == iconv(gbk2UtfDescriptor, &in, &inlen, &out, &outlen))  
//	//{  
//	//	::free(outbuf);  
//	//	return false;  
//	//}  
//	//outstr.clear();  
//	//outstr.append(outbuf);  
//	//::free(outbuf); 
//	if( gbk2UtfDescriptor== (iconv_t)(-1) )
//		return -1;
//	std::string from = str;
//	char* fptr = (char*)from.c_str();
//	char* tptr = (char*)str.c_str();
//
//	size_t fl = from.length();
//	size_t tl = str.length();
//
//	int ret;
//
//	size_t len = str.size();
//
//	char*  out= NULL;
//	out = (char*)malloc(sizeof(char)*(len+1));
//	if (out == NULL)
//	{
//		return -1;
//	}
//	char* pChar = out;
//	memcpy(out,str.c_str(),str.length());
//	ret = iconv( gbk2UtfDescriptor, &fptr, &fl, &pChar, &len );
//	std::string strOut(out);
//	str = strOut;
//	free(out);
//	out = NULL;
//	return ret;  
//} 

