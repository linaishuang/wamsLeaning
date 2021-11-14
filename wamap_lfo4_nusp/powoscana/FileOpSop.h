#ifndef _CFILECASEOP_H_SOP
#define _CFILECASEOP_H_SOP

#include "pub.h"
#include "ClientOp.h"

#define HISFILEPORT_SOP                  10012          //��ʷ�ļ�������ʶ˿�
#define HISFILEREADBUFSIZE_SOP           1024*10    //��ʷ�ļ���ȡ��������С KB

// -lman
class CFileOpSop
{
public:
	CFileOpSop();
	virtual ~CFileOpSop(); 

    //��������������ֵ>=0��ʾ�ɹ�
	virtual int Download(const char * host, const char * svrfile, const char * localfile); 
	virtual int Upload(const char * host, const char * svrfile, const char * localfile, int &filesize); 
	virtual int Delete(const char * host, const char * svrfile); 
	bool GetFileHost(char * host, int flag=1); //flag=1����, 2����
	
private:
	FileAdminstrator  m_FileOp; 
    char logstr[2048];
};

#endif

