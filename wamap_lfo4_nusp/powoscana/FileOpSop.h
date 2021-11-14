#ifndef _CFILECASEOP_H_SOP
#define _CFILECASEOP_H_SOP

#include "pub.h"
#include "ClientOp.h"

#define HISFILEPORT_SOP                  10012          //历史文件服务访问端口
#define HISFILEREADBUFSIZE_SOP           1024*10    //历史文件读取缓冲区大小 KB

// -lman
class CFileOpSop
{
public:
	CFileOpSop();
	virtual ~CFileOpSop(); 

    //操作函数，返回值>=0表示成功
	virtual int Download(const char * host, const char * svrfile, const char * localfile); 
	virtual int Upload(const char * host, const char * svrfile, const char * localfile, int &filesize); 
	virtual int Delete(const char * host, const char * svrfile); 
	bool GetFileHost(char * host, int flag=1); //flag=1主机, 2备机
	
private:
	FileAdminstrator  m_FileOp; 
    char logstr[2048];
};

#endif

