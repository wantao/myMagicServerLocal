#ifndef MYGLOG_H
#define MYGLOG_H

class cMyGlog
{
public:
	cMyGlog();
	~cMyGlog();
	bool Init(char* argv[]);
	void errorlogRecord(const char* format,...);
	void debugLog(const char* format,...);
	void normalLog(const char* format,...);
	void UnInit();
protected:
private:
};

extern cMyGlog g_cMyGlog;

#endif