/**************************************************************************

   pubodbapi.h: interface for the CPubOdbApi class.

   HISTORY :
   Date        Version       Author                Activities
	  ----------------------------------------------------------------------
   2011/03/08   v1.0.0        zhw                  创建
   2011/04/01   v1.0.1        zhw                  更新MAXBUF为512
   删除应用号,态号宏定义,增加全局变量
   2011/04/16   v1.0.2        zhw                  添加根据域名获取域号函数
   2011/05/04   v1.0.3        zhw                  添加进程注册函数
   删除头文件
   2011/05/05   v1.0.4        zhw                  添加消息封装
   2011/05/11   v1.0.5        zhw                  添加PODB_TableModifyByKey类型
   2011/05/20   v1.0.6        zhw                  添加公共设备接口函数,修改域号顺序,添加遥测名
   2011/06/01   v1.0.7        zhw                  添加测点遥测信息表函数
   2011/06/02   v1.0.8        zhw                  添加商用库更新函数(测试不通过)
   2011/06/03   v1.0.9        zhw                  错误时输出返回值
   2011/10/19   v1.0.10       zhw                  增加公共设备域名对应，增加频率域
   2012/05/17   v1.0.11       zhw                  从厂站中获取设备频率域名
   增加根据设备ID获取设备名称函数
   TODO:测点名称和设备类型对应
   增加消息返回值输出
   2012/07/12   v1.0.12       zhw                  添加PODB_GetDevId
   2012/08/02   v1.0.13       zhw                  USE_DB_OP_API修改为默认USE_DB_NET_API
   2012/08/13   v1.0.14       zhw                  增加PODB_GetFieldNoByName函数，添加默认值;修改PODB_InitColID
   同时增加实时库网络接口m_IDBTableNet和本地接口m_IDBTableOp,m_ODBApiNet,m_ODBApiOp
   2012/09/05   v1.0.15       zhw                  修改PODB_GetNameStringByID参数is_net
   2012/09/13   v1.0.16       zhw                  增加PODB_GetVarNsquare,SAFE_DELETE->SAFE_DELETE_ARRAY
   2012/12/21   v1.0.17       zhw                  增加宏定义_PODB_MSGBUS_,_PODB_SAMPLE_,_PODB_WARN_,调整顺序
   若定义_PODB_MSGBUS_,则库函数中需包含
   -lpthread -lpub -lsam_common -lsam_switch -lsam_service -lodb_apiall_lib -lrte -lpub -lodb_apiall_lib -lpthread -ldbclient -lsqlspclient -ltriggersampleclient -lalarm_com
   若定义_PODB_SAMPLE_,则库函数中需包含-ltriggersampleclient
   若定义_PODB_WARN_,则D5000环境下源码中需包含warn_service_message_m.h,warn_service_message_m.cpp,库函数中需包含-lalarm_com
   OPEN3000环境下源码中需包含warn_service_message.h,warn_service_message.cpp
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
#include "sysadm/scn_common.h"   // Name_to_ID 接口
#include "sysadm/scn_service.h"   // IsOnDuty 接口
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


/*监视设备类型*/
#ifndef TOOL_DEV_TYPE_GEN
#define	TOOL_DEV_TYPE_GEN  MENU_LFO_DEV_GEN	//0	/* 发电机 */
#define	TOOL_DEV_TYPE_LINE MENU_LFO_DEV_LINE	//1	/* 交流线路 */
#define	TOOL_DEV_TYPE_FAC  MENU_LFO_DEV_FAC	//3	/* 厂站 */
#define	TOOL_DEV_TYPE_TR2  MENU_LFO_DEV_TR2	//5	/* 2绕组变压器 */
#define	TOOL_DEV_TYPE_TR3  MENU_LFO_DEV_TR3	//5	/* 3绕组变压器 */
#endif

#ifndef DB_TABLE_BASE
#define DB_TABLE_BASE 1000000
#endif

#ifndef MAXBUF
#define MAXBUF 512
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x) delete x; x=NULL;} // 安全删除new分配出来的变量空间  
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x) delete[] x; x=NULL;} // 安全删除new分配出来的数组空间   
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
	//根据KEY_ID_STRU keyid获取表号，记录号，域号，设备ID号
	void PODB_ParseKeyid(const KEY_ID_STRU keyid, int& table_no, int& rec_id, int& column_id, INT_REC& devid);
	// 根据INT_REC keyid获取表号，区域号，记录号，域号
	void PODB_ParseKeyid(const INT_REC keyid, int& table_no, int& area_no, int& serial_no, int& column_no);
	// 将记录ID中记录序号增加 n
	void IncrRtdbId(INT_REC& id, int no = 1);
	// 将记录ID中记录序号减少 n
	void DescRtdbId(INT_REC& id, int no = 1);
#ifdef _NUSP
	// 根据INT_REC keyid获取设备ID号
	INT_REC PODB_GetRecordId(const INT_REC keyid);
	// 根据INT_REC keyid获取域号
	int PODB_GetColumnId(const INT_REC keyid);
#else // _NUSP
#ifdef _D5000
	// 根据long keyid获取设备ID号
	INT_REC PODB_GetRecordId(const long keyid);
	// 根据long keyid获取域号
	int PODB_GetColumnId(const long keyid);
#else // _D5000
	// 根据KEY_ID_STRU keyid获取设备ID号
	INT_REC PODB_GetRecordId(const KEY_ID_STRU keyid);
	// 根据KEY_ID_STRU keyid获取域号
	int PODB_GetColumnId(const KEY_ID_STRU keyid);
#endif // _D5000
#endif // _NUSP
	// 根据KEY_ID_STRU keyid获取表号
	//int GetTableNOByKeyID(const KEY_ID_STRU keyid);
	//根据设备ID获取表号
	int PODB_GetTabelNoByDevId(const INT_REC dev_id);
	//根据表号，记录号，域号获取KEY_ID_STRU keyid
	void PODB_SetKeyid(const int table_no, const int rec_id, const int column_id, KEY_ID_STRU& keyid);
	//根据设备ID、域号获取KEY_ID_STRU keyid		
	void PODB_SetKeyid(const INT_REC dev_id, const int column_id, KEY_ID_STRU& keyid);
	//根据表号，记录号获取设备ID
	void PODB_GetDevIdByKeyID(const int& table_no, const int& rec_id, INT_REC& devid);
	INT_REC PODB_GetDevIdByKeyID(const int& table_no, const int& rec_id);
	// 根据表号，记录号，域号获取设备ID
	INT_REC PODB_GetDevId(const int table_no, const int rec_id, const int field_id);
	// 改变记录号中的表号(域号、记录号不变)
	INT_REC PODB_ChangeDevId(const int table_no, const INT_REC rec_id);
	//根据KEY_ID_STRU keyid获取设备ID
	//void GetDevIdByKeyID(const KEY_ID_STRU & keyid, INT_REC & devid);
	//获取系统主目录
	void PODB_GetHomePath(string& home_path);
	//获取Sys目录
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
	// 获取记录个数
	int  PODB_TableGet(const int tableno, const std::string deffields, CBuffer& buf_base, const int appno = g_appNo, const int contextno = g_ctxNo);
	int  PODB_TableGet(const int tableno, CBuffer& buf_base, const int appno = g_appNo, const int contextno = g_ctxNo);  // 读取全表
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
	int  PODB_IsOnDuty(const int app_id = g_appNo, const int ctx_no = g_ctxNo); // 获取值班主机
	int  PODB_GetHostName(string& hostname, const int app_id = g_appNo, const int ctx_no = g_ctxNo); // 获取主机名
	int  PODB_GetHostName(char* hostname, const int bufsize, const int app_id = g_appNo, const int ctx_no = g_ctxNo);  // 获取主机名
#ifdef _D5000
	int  PODB_ProcRegister(char* context_name, char* app_name, char* proc_name);
#else
	int  PODB_ProcRegister(const int app_id, const string register_name, const short mon_cycle, const short context = 0);  // 进程注册
#endif // _D5000
public:
	int  PODB_InitColID(vector<int>& VecCol, const int tableno);
public:
	// 商用库修改
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
	// 	int CheckProcessRunByPid(int PID);//根据PID检查进程是否存在 返回值大于0，进程正常运行
	// 	int CheckProcessRunByName(char *ProcessDir,char *ProcessName);//根据进程名称和进程运行路径检查进程是否运行 返回值大于0，进程正常运行
private:
#ifdef _TABLE_NET
	NET_ODB::CTableNet	m_IDBTable;      //  实时库网络接口
	NET_ODB::CTableNet      m_IDBTable_wams_fes;      //lns at jb 科东环境读取不同应用下数据需建各自的对象
#ifdef _KEDONG
	CApiNet    	m_ODBApi;
#else
	NET_ODB::CApiNet        m_ODBApi;
#endif // _KEDONG
#else
	ODB::CTableOp       m_IDBTable;      //  实时库本地接口
	ODB::CTableOp       m_IDBTable_wams_fes;      //lns at jb 科东环境读取不同应用下数据需建各自的对象
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
	// 消息封装
#ifdef _D5000
	int PODB_messageInit(char* context_name = NULL, char* app_name = NULL, char* proc_name = NULL);
	int PODB_messageExit(int proc_key = -1);
	int PODB_messageSend(Message* messageP, int messageLength, Msg_destination* msg_dst_p = NULL);
	int PODB_messageReceive(Message* messageP, Msg_source* msg_src_p = NULL, int sync = 1);
	int PODB_messageSubscribe(short set_id, char* context_name = NULL);
	int PODB_messageUnSubscribe(short set_id, char* context_name = NULL);
	//采样 修改为全局变量 cyj 2013/10/11
#ifdef _PODB_SAMPLE_
	SEQSampleProcessRequest tmptsmp;
	trigger_sample_service_interface tri_client;
#endif
#else
	int  PODB_InitMsgBus(const string proc_name, const string thread_name, const int persist = 1, const int context = g_ctxNo, const int system = 0);  // 初始化消息
	int  PODB_ReleaseMsgBus();
	int  PODB_SendMessageToHost(const char* message, const int msg_type, const int length, const char* host_name, const char* process_name, const char* instance_name = NULL);
	int  PODB_SendMessageToChannel(const char* message, const int msg_type, const int length, const int channel_id, const char* process_name = NULL, const char* instance_name = NULL);
	int  PODB_JoinChannel(const int channel_id);
	int  PODB_RecvMessage(struct MESSAGE_HEADER* msg_header, ::CBuffer& buf_base);
#endif  // _D5000
#ifdef _PODB_SAMPLE_
	int  PODB_TriggerSample(vector<TRIGGER_SAMPLE_DEFINE>& vectrisample);
	int  PODB_TriggerSample(TRIGGER_SAMPLE_DEFINE& trisample);//采单条记录
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
