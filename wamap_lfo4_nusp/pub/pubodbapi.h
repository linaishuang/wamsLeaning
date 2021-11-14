/**************************************************************************

   pubodbapi.h: interface for the CPubOdbApi class.

   HISTORY :
   Date        Version       Author                Activities
	  ----------------------------------------------------------------------
   2011/03/08   v1.0.0        zhw                  ����
   2011/04/01   v1.0.1        zhw                  ����MAXBUFΪ512
   ɾ��Ӧ�ú�,̬�ź궨��,����ȫ�ֱ���
   2011/04/16   v1.0.2        zhw                  ��Ӹ���������ȡ��ź���
   2011/05/04   v1.0.3        zhw                  ��ӽ���ע�ắ��
   ɾ��ͷ�ļ�
   2011/05/05   v1.0.4        zhw                  �����Ϣ��װ
   2011/05/11   v1.0.5        zhw                  ���PODB_TableModifyByKey����
   2011/05/20   v1.0.6        zhw                  ��ӹ����豸�ӿں���,�޸����˳��,���ң����
   2011/06/01   v1.0.7        zhw                  ��Ӳ��ң����Ϣ����
   2011/06/02   v1.0.8        zhw                  ������ÿ���º���(���Բ�ͨ��)
   2011/06/03   v1.0.9        zhw                  ����ʱ�������ֵ
   2011/10/19   v1.0.10       zhw                  ���ӹ����豸������Ӧ������Ƶ����
   2012/05/17   v1.0.11       zhw                  �ӳ�վ�л�ȡ�豸Ƶ������
   ���Ӹ����豸ID��ȡ�豸���ƺ���
   TODO:������ƺ��豸���Ͷ�Ӧ
   ������Ϣ����ֵ���
   2012/07/12   v1.0.12       zhw                  ���PODB_GetDevId
   2012/08/02   v1.0.13       zhw                  USE_DB_OP_API�޸�ΪĬ��USE_DB_NET_API
   2012/08/13   v1.0.14       zhw                  ����PODB_GetFieldNoByName���������Ĭ��ֵ;�޸�PODB_InitColID
   ͬʱ����ʵʱ������ӿ�m_IDBTableNet�ͱ��ؽӿ�m_IDBTableOp,m_ODBApiNet,m_ODBApiOp
   2012/09/05   v1.0.15       zhw                  �޸�PODB_GetNameStringByID����is_net
   2012/09/13   v1.0.16       zhw                  ����PODB_GetVarNsquare,SAFE_DELETE->SAFE_DELETE_ARRAY
   2012/12/21   v1.0.17       zhw                  ���Ӻ궨��_PODB_MSGBUS_,_PODB_SAMPLE_,_PODB_WARN_,����˳��
   ������_PODB_MSGBUS_,��⺯���������
   -lpthread -lpub -lsam_common -lsam_switch -lsam_service -lodb_apiall_lib -lrte -lpub -lodb_apiall_lib -lpthread -ldbclient -lsqlspclient -ltriggersampleclient -lalarm_com
   ������_PODB_SAMPLE_,��⺯���������-ltriggersampleclient
   ������_PODB_WARN_,��D5000������Դ���������warn_service_message_m.h,warn_service_message_m.cpp,�⺯���������-lalarm_com
   OPEN3000������Դ���������warn_service_message.h,warn_service_message.cpp
**************************************************************************/

#if !defined(AFX_PUBODBAPI_H__8882DD38_C1E1_4828_AAFC_2069A3DB9465__INCLUDED_)
#define AFX_PUBODBAPI_H__8882DD38_C1E1_4828_AAFC_2069A3DB9465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _PODB_MSGBUS_
#ifndef _WAMAP_
#define _WAMAP_
#endif
#endif

#ifdef _PODB_SAMPLE
#ifndef _PODB_MSGBUS_
#define _PODB_MSGBUS_
#endif

#ifdef _PODB_MSGBUS_
#ifndef _WAMAP_
#define _WAMAP_
#endif
#endif
#endif

#ifdef _PODB_WARN_
#ifndef _PODB_MSGBUS_
#define _PODB_MSGBUS_
#endif

#ifdef _PODB_MSGBUS_
#ifndef _WAMAP_
#define _WAMAP_
#endif
#endif
#endif
//////////////////////////////////////////////////////////////////////////
#ifdef _WAMAP_
#ifdef _D5000
#ifndef _WINDOWS32
#ifdef _NUSP
#include "sysadm/proc_common.h"
#else
#include "proc_inv.h"  //PODB_ProcRegister
#endif // _NUSP
#ifndef _KEDONG
#include "const.h"
#endif
#endif // _WINDOWS32
#ifdef _PODB_MSGBUS_
#include "msg_api/message_inv.h"
#include "msg_api/messageheader.h"
#ifdef _PODB_SAMPLE_
#include "db_com/trigger_sample_m.h"
#include "db_com/trigger_sample_service_interface.h"
#endif
#ifdef _PODB_WARN_
#include "warn_service_message_m.h"
#endif
#endif  // #ifdef _PODB_MSGBUS_
#else  // #ifdef _D5000
#ifdef _PODB_MSGBUS_
#include "msg_bus.h"
#ifdef _PODB_SAMPLE_
#include "db_com/trigger_sample.h"
#endif
#ifdef _PODB_WARN_
#include "warn_service_message.h"
#endif // _PODB_WARN_
#endif // _PODB_MSGBUS_
#endif // _D5000
#include "db_api/odb_tableop.h"      
#include "db_api/odb_tablenet.h"
#include "db_api/odb_apiop.h"
#include "db_api/odb_apinet.h"
#include "db_api/odb_define.h"
#ifndef _NUSP
#include "db_com/modify_client_base.h"
#endif // !_NUSP
#ifdef _KEDONG
#ifdef _WAMSTOOL
#include "CHisDbOp.h"
#endif // _WAMSTOOL
#else
#include "db_com/sql_sp_client_base.h"
#include "db_com/wholesqlsp.h"
#endif // _KEDONG

#include "message_type.h"
#include "message_channel.h"
#include "system.h"
#ifndef _WINDOWS32
#ifndef _KEDONG
#include "const.h"
#endif // !_KEDONG
#endif // !_WINDOW32
#ifdef _NUSP
#include "sysadm/scn_common.h"   // Name_to_ID �ӿ�
#include "sysadm/scn_service.h"   // IsOnDuty �ӿ�
#else
#include <sam_service.h>
#include <sam_common.h>
#endif // _NUSP
#endif // _WAMAP_

#include "db_def/db_define.h"
#include "db_def/sys_menu_info.h"
#include "db_def/warn_define.h"
#ifdef _D5000
#include "db_def/db_struct_wams.h"
#else
#include "db_def/db_struct_wamap.h"
#include "db_def/db_struct_eaccs.h"
#endif
#include "pubodbstruct.h"

#include <string>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
#ifndef VEC_V_NUM
#define VEC_V_NUM  2  //  v_a_value v_a_angle
//#define VEC_V_NUM  6  //  UAV UAA UBV UBA UCV UCA
#endif

#ifndef VEC_VU_NUM
#define VEC_VU_NUM  8  //  UAV UAA UBV UBA UCV UCA U F
#endif

#ifndef VEC_VI_NUM
#define VEC_VI_NUM  12  //  UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA
#endif

#ifndef VEC_VIP_NUM
#define VEC_VIP_NUM 4 // v_a_value v_a_angle p q //f
//#define VEC_VIP_NUM 12 // UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
#endif

#ifndef VEC_ALL_NUM
#define VEC_ALL_NUM 22 // UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA QWR U1V U1A I1V I1A
#endif

#ifndef VEC_USE_NUM
#define VEC_USE_NUM 17 // UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA  
#endif


/*�����豸����*/
#ifndef TOOL_DEV_TYPE_GEN
#define	TOOL_DEV_TYPE_GEN  MENU_LFO_DEV_GEN	//0	/* ����� */
#define	TOOL_DEV_TYPE_LINE MENU_LFO_DEV_LINE	//1	/* ������· */
#define	TOOL_DEV_TYPE_FAC  MENU_LFO_DEV_FAC	//3	/* ��վ */
#define	TOOL_DEV_TYPE_TR2  MENU_LFO_DEV_TR2	//5	/* 2�����ѹ�� */
#define	TOOL_DEV_TYPE_TR3  MENU_LFO_DEV_TR3	//5	/* 3�����ѹ�� */
#endif

#ifndef DB_TABLE_BASE
#define DB_TABLE_BASE 1000000
#endif

#ifndef MAXBUF
#define MAXBUF 512
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x) delete x; x=NULL;} // ��ȫɾ��new��������ı����ռ�  
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x) delete[] x; x=NULL;} // ��ȫɾ��new�������������ռ�   
#endif

#ifndef SAFE_SWAP
#define SAFE_SWAP(x,s) vector<s>().swap(x)
#endif

#ifndef LTDEBUG
#define LTDEBUG 0
#define LTINFO  1
#define LTDEV   2
#define LTWARN  3
#define LTERROR 4
#endif

#ifndef MAX_MSGBUF_LEN
#define MAX_MSGBUF_LEN		32767
#endif
//////////////////////////////////////////////////////////////////////////
extern int   g_appNo;
extern string   g_appName;
extern int   g_ctxNo;
extern string   g_ctxName;
//////////////////////////////////////////////////////////////////////////
class CPubOdbApi
{
public:
	CPubOdbApi();
	virtual ~CPubOdbApi();
public:
	virtual void MessShow(const char* log, const int logtype = LTINFO);
public:
	//����KEY_ID_STRU keyid��ȡ��ţ���¼�ţ���ţ��豸ID��
	void PODB_ParseKeyid(const KEY_ID_STRU keyid, int& table_no, int& rec_id, int& column_id, INT_REC& devid);
	// ����INT_REC keyid��ȡ��ţ�����ţ���¼�ţ����
	void PODB_ParseKeyid(const INT_REC keyid, int& table_no, int& area_no, int& serial_no, int& column_no);
	// ����¼ID�м�¼������� n
	void IncrRtdbId(INT_REC& id, int no = 1);
	// ����¼ID�м�¼��ż��� n
	void DescRtdbId(INT_REC& id, int no = 1);
#ifdef _NUSP
	// ����INT_REC keyid��ȡ�豸ID��
	INT_REC PODB_GetRecordId(const INT_REC keyid);
	// ����INT_REC keyid��ȡ���
	int PODB_GetColumnId(const INT_REC keyid);
#else // _NUSP
#ifdef _D5000
	// ����long keyid��ȡ�豸ID��
	INT_REC PODB_GetRecordId(const long keyid);
	// ����long keyid��ȡ���
	int PODB_GetColumnId(const long keyid);
#else // _D5000
	// ����KEY_ID_STRU keyid��ȡ�豸ID��
	INT_REC PODB_GetRecordId(const KEY_ID_STRU keyid);
	// ����KEY_ID_STRU keyid��ȡ���
	int PODB_GetColumnId(const KEY_ID_STRU keyid);
#endif // _D5000
#endif // _NUSP
	// ����KEY_ID_STRU keyid��ȡ���
	//int GetTableNOByKeyID(const KEY_ID_STRU keyid);
	//�����豸ID��ȡ���
	int PODB_GetTabelNoByDevId(const INT_REC dev_id);
	//���ݱ�ţ���¼�ţ���Ż�ȡKEY_ID_STRU keyid
	void PODB_SetKeyid(const int table_no, const int rec_id, const int column_id, KEY_ID_STRU& keyid);
	//�����豸ID����Ż�ȡKEY_ID_STRU keyid		
	void PODB_SetKeyid(const INT_REC dev_id, const int column_id, KEY_ID_STRU& keyid);
	//���ݱ�ţ���¼�Ż�ȡ�豸ID
	void PODB_GetDevIdByKeyID(const int& table_no, const int& rec_id, INT_REC& devid);
	INT_REC PODB_GetDevIdByKeyID(const int& table_no, const int& rec_id);
	// ���ݱ�ţ���¼�ţ���Ż�ȡ�豸ID
	INT_REC PODB_GetDevId(const int table_no, const int rec_id, const int field_id);
	// �ı��¼���еı��(��š���¼�Ų���)
	INT_REC PODB_ChangeDevId(const int table_no, const INT_REC rec_id);
	//����KEY_ID_STRU keyid��ȡ�豸ID
	//void GetDevIdByKeyID(const KEY_ID_STRU & keyid, INT_REC & devid);
	//��ȡϵͳ��Ŀ¼
	void PODB_GetHomePath(string& home_path);
	//��ȡSysĿ¼
	void PODB_GetSysPath(string& sys_path);
	string PODB_GetYcName(const int nVIP);
	int  PODB_GetVarNsquare(int var, int n);
public:
	int  PODB_ClearSample(vector<TRIGGER_SAMPLE_DEFINE>& vectrisample);
	int  PODB_ClearSample(TRIGGER_SAMPLE_DEFINE& trisample);
	int  PODB_SmpBaseINT(TRIGGER_SAMPLE_DEFINE& tmpSample, const int col_id, const INT_REC vint);
	int  PODB_SmpBaseFLT(TRIGGER_SAMPLE_DEFINE& tmpSample, const int col_id, const float vfloat);
	int  PODB_SmpBaseSTR(TRIGGER_SAMPLE_DEFINE& tmpSample, const int col_id, const string vstring);
public:
	int  PODB_ClearWarnMsg(vector<WARN_MESSAGE_STRUCT>& tmpVecWarnMsg);
	int  PODB_ClearWarnMsg(WARN_MESSAGE_STRUCT& tmpWarnMsg);
	int  PODB_WarnMsgBaseINT(WARN_MESSAGE_STRUCT& tmpWarnMsg, const int vint);
	int  PODB_WarnMsgBaseTime(WARN_MESSAGE_STRUCT& tmpWarnMsg, const RTDB_TIME vtime);
#ifdef _D5000
	int  PODB_WarnMsgBaseLONG(WARN_MESSAGE_STRUCT& tmpWarnMsg, const INT_REC vlong);
#endif
	int  PODB_WarnMsgBaseSTRING(WARN_MESSAGE_STRUCT& tmpWarnMsg, const string vstring);
	int  PODB_IsFlagAll(int* nFlag, const int nFlagNum);
private:
	char m_LogString[MAXBUF];
#ifdef _WAMAP_
public:
	// ��ȡ��¼����
	int  PODB_TableGet(const int tableno, const std::string deffields, CBuffer& buf_base, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_TableGet(const int tableno, CBuffer& buf_base, const int appno = g_appNo, const int contextno = g_ctxNo);  // ��ȡȫ��
	int  PODB_GetTabRecNum(const int tableno, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_GetFieldNoByName(const int tableno, const char* field_name, const int ret_value, const int appno = g_appNo, const int contextno = g_ctxNo);
	//int	 PODB_TableGetByKey(const int tableno,INT_REC &key_ptr, const char* field_name, char* field_buf_ptr, const int buf_size,const int appno = g_appNo, const int contextno = g_ctxNo);

public:
	int  PODB_TableWrite(const char* pTabBuf, const int TabBufSize, const int tableno, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_TableModify(const char* field_name, const char* field_buf_ptr, const int buf_size, const int tableno, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_TableModifyByKey(const char* key_ptr, const char* field_name, const char* field_buf_ptr, const int buf_size, const int tableno, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_TableModifyByKey(const char* key_ptr, const int field_no, const char* field_buf_ptr, const int buf_size, const int tableno, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_DeleteRecord(const INT_REC& key_id, const int tableno, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_DeleteRecord(const vector<INT_REC>& key_id, const int tableno, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_TableClear(const int tableno, const int appno = g_appNo, const int contextno = g_ctxNo);

public:
	int  PODB_IsOnDuty(const int app_id = g_appNo, const int ctx_no = g_ctxNo); // ��ȡֵ������
	int  PODB_GetHostName(string& hostname, const int app_id = g_appNo, const int ctx_no = g_ctxNo); // ��ȡ������
	int  PODB_GetHostName(char* hostname, const int bufsize, const int app_id = g_appNo, const int ctx_no = g_ctxNo);  // ��ȡ������
#ifdef _D5000
	int  PODB_ProcRegister(char* context_name, char* app_name, char* proc_name);
#else
	int  PODB_ProcRegister(const int app_id, const string register_name, const short mon_cycle, const short context = 0);  // ����ע��
#endif // _D5000
public:
	int  PODB_InitColID(vector<int>& VecCol, const int tableno);
public:
	// ���ÿ��޸�
	int  PODB_ExecuteMultiSql(const std::vector<string>& vecsqlstr, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_TableClearSql(const string tablename);
	int  PODB_ModifyTableBySqls(const std::vector<string>& vecsqlstr, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_ModifyTableBySqlsp(const std::vector<string>& vecsqlstr, const int appno = g_appNo, const int contextno = g_ctxNo);
public:
	int  PODB_GetFacIDByFacName(INT_REC& fac_id, const char* fac_name, const int appno = g_appNo, const int contextno = g_ctxNo);
	int	 PODB_GetFacNameByFacID(char* fac_name, const INT_REC fac_id, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_GetNameStringByID(const INT_REC reference_id, std::string& ref_string, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_GetNameStringByID(const std::vector<INT_REC>& vec_refid, std::vector<std::string>& vec_ref_string, const int appno = g_appNo, const int contextno = g_ctxNo);
public:
	int  PODB_GetFacInfo(vector<FAC_TAB_CRE>& VecFacTab);
	int  PODB_GetAreaInfo(vector<AREA_TAB_CRE>& VecAreaTab);
	int  PODB_GetVLTypeInfo(vector<VL_TYPE_TAB_CRE>& VecVLTypeTab);
	int  PODB_GetFesYcInfo(vector<FES_YC_TAB_CRE>& VecFesYcTab);
	int  PODB_GetBusInfo(vector<BUS_TAB_CRE>& VecBusTab);
	int  PODB_GetAclnInfo(vector<ACLINE_TAB_CRE>& VecAclnTab);
	int  PODB_GetAclnDotInfo(vector<ACLNDOT_TAB_CRE>& VecAclnDotTab);
	int  PODB_GetGenInfo(vector<GEN_TAB_CRE>& VecGenTab);
	int  PODB_GetTrInfo(vector<TR_TAB_CRE>& VecTrTab);
	int  PODB_GetTrwdInfo(vector<TRWD_TAB_CRE>& VecTrwdTab);
	int  PODB_GetValueInfo(vector<VALUEINFO_TAB_CRE>& VecValueInfoTab);
public:
	// 	int CheckProcessRunByPid(int PID);//����PID�������Ƿ���� ����ֵ����0��������������
	// 	int CheckProcessRunByName(char *ProcessDir,char *ProcessName);//���ݽ������ƺͽ�������·���������Ƿ����� ����ֵ����0��������������
private:
#ifdef _TABLE_NET
	NET_ODB::CTableNet	m_IDBTable;      //  ʵʱ������ӿ�
	NET_ODB::CTableNet      m_IDBTable_wams_fes;      //lns at jb �ƶ�������ȡ��ͬӦ���������轨���ԵĶ���
#ifdef _KEDONG
	CApiNet    	m_ODBApi;
#else
	NET_ODB::CApiNet        m_ODBApi;
#endif // _KEDONG
#else
	ODB::CTableOp       m_IDBTable;      //  ʵʱ�Ȿ�ؽӿ�
	ODB::CTableOp       m_IDBTable_wams_fes;      //lns at jb �ƶ�������ȡ��ͬӦ���������轨���ԵĶ���
#ifdef _KEDONG
	CApiOp		m_ODBApi;
#else
	ODB::CApiOp         m_ODBApi;
#endif // _KEDONG
#endif // _TABLR_NET

#ifdef _NUSP
	NS_SYSADMIN::CScnService m_ServMan;
#else
	CServicesManage     m_ServMan;
	CSamPub             m_SamMan;
#endif // _NUSP

public:
#ifdef _PODB_MSGBUS_
	// ��Ϣ��װ
#ifdef _D5000
	int PODB_messageInit(char* context_name = NULL, char* app_name = NULL, char* proc_name = NULL);
	int PODB_messageExit(int proc_key = -1);
	int PODB_messageSend(Message* messageP, int messageLength, Msg_destination* msg_dst_p = NULL);
	int PODB_messageReceive(Message* messageP, Msg_source* msg_src_p = NULL, int sync = 1);
	int PODB_messageSubscribe(short set_id, char* context_name = NULL);
	int PODB_messageUnSubscribe(short set_id, char* context_name = NULL);
	//���� �޸�Ϊȫ�ֱ��� cyj 2013/10/11
#ifdef _PODB_SAMPLE_
	SEQSampleProcessRequest tmptsmp;
	trigger_sample_service_interface tri_client;
#endif
#else
	int  PODB_InitMsgBus(const string proc_name, const string thread_name, const int persist = 1, const int context = g_ctxNo, const int system = 0);  // ��ʼ����Ϣ
	int  PODB_ReleaseMsgBus();
	int  PODB_SendMessageToHost(const char* message, const int msg_type, const int length, const char* host_name, const char* process_name, const char* instance_name = NULL);
	int  PODB_SendMessageToChannel(const char* message, const int msg_type, const int length, const int channel_id, const char* process_name = NULL, const char* instance_name = NULL);
	int  PODB_JoinChannel(const int channel_id);
	int  PODB_RecvMessage(struct MESSAGE_HEADER* msg_header, ::CBuffer& buf_base);
#endif  // _D5000
#ifdef _PODB_SAMPLE_
	int  PODB_TriggerSample(vector<TRIGGER_SAMPLE_DEFINE>& vectrisample);
	int  PODB_TriggerSample(TRIGGER_SAMPLE_DEFINE& trisample);//�ɵ�����¼
#endif
#ifdef _PODB_WARN_
	int  PODB_SendWarnMessage(vector<WARN_MESSAGE_STRUCT>& tmpVecWarnMsg, const int appno = g_appNo, const int contextno = g_ctxNo);
#endif
public:
#ifdef _D5000
	message_invocation  m_MsgBus;
#else
	CMsgBus* m_MsgBus;
#endif

#endif  // _PODB_MSGBUS_
#endif  // _WAMAP_
};

#endif // !defined(AFX_DEVTABLE_H__8882DD38_C1E1_4828_AAFC_2069A3DB9465__INCLUDED_)
