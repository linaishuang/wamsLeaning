
#ifndef GUO__HisdbAccess
#define GUO__HisdbAccess

//CBuffer
#ifndef _PCS9000
#include "pub_buf.h"

#ifndef _D5000
#include "db_com/public.h"
#endif

#include "db_com/wholesqlsp.h"
#include "db_com/server_define.h"
#include "db_com/sql_sp_client_base.h"
#include "db_com/wholepubtools.h"
#include "db_com/CSqlResultAlignClient.h"
#ifdef _D5000
#include "public_m.h"
#ifdef _NUSP
	//����޸����ÿ�ӿ�
#else
#include "db_com/modify_client_base.h"
#endif // _NUSP
#else
#include "db_com/wholemodify.h"
#endif
//rtdb
#include "db_api/odb_tablenet.h"

//HISDB��
#define D5000_HISDB_PORT 11132
//ALARM��
#define D5000_ALARM_PORT 11134

class  HisdbAccess
{
public:
    //portno�˿ںţ���D5000��Ч, HISDBΪ11132
	HisdbAccess( int portno=0, bool bdebug=false);
	~HisdbAccess();

//////////////////////////////////////////////////////////////////////���ÿ�/////////////////////////////////
	int QueryHisBySql( const char * sql ); //��ѯ �����ؼ�¼����
	int QueryHisBySql( const char * sql,  int structlen, CBuffer &buf ); //��ѯ ��������
	int QueryHisBySql( const char * sql,  int structlen, char ** buf ); //��ѯ ��������
	int ExecHisBySql( const char * sql ); //ִ��update,delete
	int m_queryerr;
	
/////////////////////////////////////////////////////////////////ʵʱ��//////////////////////////////////////
    int QueryRtDB(const int appid, const int tableid, const char * fieldstr, CBuffer &buf);
    int QueryRtDBBySql(const int appid, const int tableid, const char * sqlstr, CBuffer &buf);

/////////////////////////////////////////////////////////////////���ÿ�ʵʱ��ͬ���ӿ�//////////////////////////////////////
    int ExecRHDBBySql(const char * sql, const int appid, const int delay_ms=500);

private:
	sql_sp_client_base * sql_sp_client;
#ifdef _NUSP
	//lns20210809
#else
	modify_client_base * sql_modify_client;
#endif // _NSUP	
	NET_ODB::CTableNet * table_net;
	    
	bool isDebug; //�Ƿ��ӡ������Ϣ
};
#endif
#endif