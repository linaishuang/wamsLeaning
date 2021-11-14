//#include  "../FastProny_factor/stdafx.h"
/******************************************************************************
  ModuleName :   WAMS LFO Analysis interface function
  FileName   :   WamLfoAnaObjItfc.cpp
  Description:   WAMS LFO Analysis interface Implemention
  HISTORY :
	  Date        Version       Author                Activities
   ----------------------------------------------------------------------
   09/11/2007       1.0         grm                    created
******************************************************************************/
#include  "WamSsoMonObj.h"

// (输入输出接口)
// 告警信息发送
// 采样触发
// tmd接口
//  请将接口部分定义的结构体 写入 WamLfoAnaObjItfc.h
/******************************************************************************/
//  系统管理头文件
// #ifdef _WINDOWS32
//  #include "warn_service_message.h"                        //  APP_TO_WARN_SERVICE_MESSAGE_STRU
//  #include "msg_bus.h"                                     //  CMsgBus SendMessageToHost()
//  #include "message_type.h"                                //  DB_TRIGGER_SAMPLE_TYPE  APP_TO_WARN_SERVICE_TYPE
//  #include "db_com\trigger_sample.h"                       //  TSEQSampleProcessRequest
//  #include "pub_ctx.h"                                     //  CContext::
//  #include "sam_common.h"                                  //  CSamPub
//  #include "sam_service.h"                                 //  CServicesManage
//  #include "db_def\warn_define.h"                          //  WAMS_LFO_WARN_TYPE
// #else
//  #include "msg_bus.h"                                     //  CMsgBus
//  #include "message_type.h"                                //  DB_TRIGGER_SAMPLE_TYPE  APP_TO_WARN_SERVICE_TYPE
//  #include "db_com/trigger_sample.h"                       //  TSEQSampleProcessRequest
//  #include "pub_ctx.h"                                     //  CContext::
//  #include "sam_common.h"                                  //  CSamPub
//  #include "warn_service_message.h"                        //  APP_TO_WARN_SERVICE_MESSAGE_STRU
//  #include "db_def/warn_define.h"                          //  WAMS_LFO_WARN_TYPE
// #endif
//*/
/******************************************************************************/
/////////////////////////////  接口函数头文件  ////////////////////////////
// #include "system.h"                                     //  AC_REALTIME_NO, AP_WAMAP
// //  系统管理
// #include  "sam_common.h"                                    //  CSamPub
// #include  "sam_service.h"                                   //  CServicesManage
// 
// //  告警管理
// #include  "warn_service_message.h"                          //  APP_TO_WARN_SERVICE_MESSAGE_STRU
// #ifndef _WINDOWS32
// 	#include  "db_def/warn_define.h"                        //  WAMS_LFO_WARN_TYPE
// #else
// 	#include  "db_def\warn_define.h"                        //  WAMS_LFO_WARN_TYPE
// #endif
// 
// //  消息总线
// #include  "msg_bus.h"                                       //  CMsgBus
// #include  "message_type.h"                                  //  DB_TRIGGER_SAMPLE_TYPE  APP_TO_WARN_SERVICE_TYPE
// #include  "message_channel.h"                               //  消息通道  召唤100帧文件

//  触发采样
//#include  "pub_ctx.h"                                       //  CContext::
// #ifndef _WINDOWS32
// 	#include  "../publicfunction/trigger_sample.h"                     //  TSEQSampleProcessRequest
//     #include "../publicfunction/tmd_dyn_get.h"
//     #include "../publicfunction/fad_trans_comtr.h"
//     #include "../WamLfoAna_his/wamap_lfo_trans.h"
//     #include "../WamLfoAna_his/WamapLfoCommPara.h"
// #else
// 	#include  "..\publicfunction\trigger_sample.h"                     //  TSEQSampleProcessRequest
//     #include "..\publicfunction\tmd_dyn_get.h"
//     #include "..\publicfunction\fad_trans_comtr.h"
//     #include "..\WamLfoAna_his\wamap_lfo_trans.h"
//     #include "..\WamLfoAna_his\WamapLfoCommPara.h"
// #endif

#ifndef _D5000
// 三态数据库makefile中包含 -ltmd_event_client_base -levent_s_req -ltcs_access_client_base -ltci_access_tool
#ifdef _WINDOWS32
#include  "lfo_s_req_tool.h"     //  LFO_S_REQ_STRU;send_lfo_save_req
#include  "lfo_r_req_tool.h"     //  LFO_R_REQ_STRU;enum_lfo_item_from_hdb;read_lfo_prof_from_hdb
//#include  "pub_r_req_tool.h"     //  enum_agc_item_from_hdb()
#else
#include  "../../tmd/work/i_lfo/save/lfo_s_req_tool.h"                            //  LFO_S_REQ_STRU;send_lfo_save_req
#include  "../../tmd/work/i_lfo/load/lfo_r_req_tool.h"                            //  LFO_R_REQ_STRU;enum_lfo_item_from_hdb;read_lfo_prof_from_hdb
//#include  "../../tmd/work/i_lfo/load/pub_r_req_tool.h"                            //  read_pmu_data_from_hdb()
#endif
#endif
//////////////////////////////////////////////////////////////////////////
//  外部宏定义
/******************************************************************************/
//  变量声明
// extern  CMsgBus *              g_pMsgBus;                    //  消息总线
// extern  CServicesManage *      g_pServMan;                   //  平台管理
////extern  CBuffer                g_BufBase;                    //  
////extern  MESSAGE_HEADER *       g_pMsgHeader;                 //  消息头
// 
////extern  short                  g_contextNo;                  //  态号
// 2008-07-29 by zhw at ty
////  变量声明
// #ifdef _WINDOWS32
// 	#include "WamapDBLib.h"                                 //  CWamapDatabase CWamapWeekDatabase CNetWamapDatabase
// #else
// 	#include "../../wamap_lib/wamap/inc/WamapDBLib.h"          //  CWamapDatabase CWamapWeekDatabase CNetWamapDatabase
// #endif
// #ifdef  USE_LOCAL_WAM_MEM  //Local
// 	#ifdef  USE_WEEK_WAM_MEM       // week
// 		extern CWamapWeekDatabase *               g_IWMDb;            //  高速缓存区历史数据本地接口
// 	#else
// 		extern CWamapDatabase *                   g_IWMDb;                //  高速缓存区实时数据本地接口
// 	#endif
// #else                      //Net
// 	#ifdef  USE_WEEK_WAM_MEM       // week
// 		extern CNetWamapDatabase *               g_IWMDb;                //  高速缓存区历史数据网络接口
// 	#else
// 		extern CNetWamapDatabase *               g_IWMDb;                //  高速缓存区实时数据网络接口
// 	#endif
// #endif

//////////////////////////////////////////////////////////////////////////
const short SF_PMU_TYPE = 1;
const short DKY_PMU_TYPE = 2;
const short NR_PMU_TYPE = 3;
const string  TR_FILE_PATH = "/users/ems/comtr_file/";
const string  DYN_FILE_PATH_FIR = "/users/ems/open2000e/tmd_data/dyn/";
const string  DYN_FILE_PATH_SEC = "/users/ems/open2000e/tmd_data/dyn_bk/";

//////////////////////////////////////////////////////////////////////////
INT_REC MF_CalcRecId(const int nTableNo, const int nColId, const int nRecId)
{
	INT_REC devid; ZERO_INT_REC(devid);
#ifdef _NUSP
	ARA_SER_NO_T area_serial_no = CUR_AREA_NO;
	area_serial_no = area_serial_no * 100000000 + nRecId;
	devid.create_key((TABLE_NO_T)nTableNo, area_serial_no, nColId);
#else
#ifdef _D5000
	KEY_STRU  buf;
	buf.table_no = nTableNo;
	buf.key = nRecId;
	buf.field_id = nColId;
	memcpy(&devid, &buf, sizeof(INT_REC));
#else
	devid = nTableNo * DB_TABLE_BASE + nRecId;
#endif // _D5000
#endif // _NUSP
	return devid;
}
/////////////////////////////// 实时库访问 ///////////////////////////////
int CLfoAnaObj::GetVarNsquare(int var, int n)         //计算某数的N次方
{
	int i, nsquare;
	nsquare = 1;

	for (i = 0; i < n; i++)
	{
		nsquare *= var;
	}

	return nsquare;
}

// 根据KeyId和域号获取PMU数据文件号列号
// int CLfoAnaObj::GetFileColumNoByKeyID( int fileno, int colno, MY_KEY_ID Mykeyid )
// {
// #ifdef _WAMAP_
// 	int                 i = 0;
// 
//     int                 TabBufSize= 0;
// 	int			        ret_code  = 0;
// 	CBuffer             buf_base;
// 
//     FES_YC_TAB_CRE *    pFesyc = NULL;
//     int                 nFesyc = 0;
// 	
//     //if ( g_IDBTableOp.Open(LFO_AP_ID, WAM_FES_YC_DEFINE_NO, g_contextNo)  == DB_OK )
// 	if ( g_IDBTableOp.Open(LFO_AP_ID, WAM_FES_YC_DEFINE_NO, AC_REALTIME_NO)  == DB_OK )
// 	{
// 		if ( g_IDBTableOp.TableGet( FES_YC_TAB_CRE_FLD, buf_base ) > 0 )
// 		{
// 			TabBufSize = sizeof(FES_YC_TAB_CRE);
// 			nFesyc = buf_base.GetLength()/TabBufSize;
// 			pFesyc = (FES_YC_TAB_CRE *)calloc(nFesyc, sizeof(FES_YC_TAB_CRE));
// 			memset((void *)pFesyc, 0, nFesyc * sizeof(FES_YC_TAB_CRE));
// 			memcpy((void*)pFesyc, (void*)buf_base.GetBufPtr(), TabBufSize*nFesyc);
// 		}
// 	}
//     if( nFesyc <= 0 )
// 	{
// 		sprintf( m_LogString, "GetFileColumNoByKeyID(): WAM_FES_YC_DEFINE_NO[%d] no record ",  WAM_FES_YC_DEFINE_NO ); 
// 		WriteLogFile( m_LogString );
// 		
// 		if( pFesyc )  {free(pFesyc);  pFesyc = NULL;}
// 		return -1;
// 	}
// 
// 	for ( i = 0; i < nFesyc; i++ )
// 	{
// 		if ( pFesyc[i].yc_id.record_id == Mykeyid.record_id )
// 		{
// 			if( pFesyc[i].yc_id.column_id == Mykeyid.column_id )
// 			{
// 				fileno = pFesyc[i].file_no;
// 				colno  = pFesyc[i].col_no;
// 
// 				break;
// 			}
// 		}
// 	}
// 
// 	if ( pFesyc )  {free(pFesyc);  pFesyc = NULL;}
// 	if ( i == nFesyc ) return 0;
// #endif
// 	return 1;
// }

// 厂站参数
int CLfoAnaObj::GetFcCtl(int num, SSO_FC_CTL* pFcCtl)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_FAC_NO_TAB, SSO_FC_CTL_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_FC_CTL);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO厂站表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO厂站表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_FC_CTL* pInfo = NULL;
	pInfo = new SSO_FC_CTL[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_FC_CTL));

	int i = 0, j = 0, nflag;
	for (i = 0; i < num; i++)
	{
		nflag = 0;
		for (j = 0; j < nInfo; j++)
		{
			if (pFcCtl[i].station_id == pInfo[j].station_id)
			{
				pFcCtl[i].is_monitor = pInfo[j].is_monitor;
				pFcCtl[i].is_part_ana = pInfo[j].is_part_ana;
				// 				pFcCtl[i].is_set_bus	= pInfo[j].is_set_bus;
				// 				pFcCtl[i].set_bus_id	= pInfo[j].set_bus_id;
				// 				pFcCtl[i].is_m_bs	    = pInfo[j].is_m_bs;
				nflag = 1;
				break;
			}
		}
		if (nflag == 0)
		{
			sprintf(m_LogString, "未找到匹配厂站["INT_REC_FMT"]", INT_REC_VAL(pFcCtl[i].station_id));
			MessShow(m_LogString);
		}
	}

	SAFE_DELETE_ARRAY(pInfo);

	// 设备个数变化
	if (num != nInfo)
	{
		return 1;
	}
#endif
	return 0;
}

// 机组参数
int CLfoAnaObj::GetGnCtl(int num, SSO_GN_CTL* pGnCtl)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_GN_NO_TAB, SSO_GN_CTL_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_GN_CTL);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO发电机表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO发电机表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_GN_CTL* pInfo = NULL;
	pInfo = new SSO_GN_CTL[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_GN_CTL));

	int i = 0, j = 0, nflag;
	for (i = 0; i < num; i++)
	{
		nflag = 0;
		for (j = 0; j < nInfo; j++)
		{
			if (pGnCtl[i].gn_id == pInfo[j].gn_id)
			{
				pGnCtl[i].is_monitor = pInfo[j].is_monitor;
				// 				pGnCtl[i].is_part_ana	= pInfo[j].is_part_ana;
				// 				pGnCtl[i].is_agl_calc	= pInfo[j].is_agl_calc;
				pGnCtl[i].sso_gn_swn = pInfo[j].sso_gn_swn;
				pGnCtl[i].sso_gn_ewn = pInfo[j].sso_gn_ewn;
				// 				sprintf(m_LogString,"LFO发电机振幅限值[%f]-[%f]", pGnCtl[i].lfo_gn_swn,pGnCtl[i].lfo_gn_ewn);
				// 				MessShow(m_LogString);
				nflag = 1;
				break;
			}
		}
		if (nflag == 0)
		{
			sprintf(m_LogString, "未找到匹配机组["INT_REC_FMT"]", INT_REC_VAL(pGnCtl[i].gn_id));
			MessShow(m_LogString);
		}
	}

	SAFE_DELETE_ARRAY(pInfo);
	// 设备个数变化
	if (num != nInfo)
	{
		return 1;
	}
#endif
	return 0;
}

// 线路参数
int CLfoAnaObj::GetLnCtl(int num, SSO_LN_CTL* pLnCtl)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_ACLN_NO_TAB, SSO_LN_CTL_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_LN_CTL);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO交流线路表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO交流线路表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_LN_CTL* pInfo = NULL;
	pInfo = new SSO_LN_CTL[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_LN_CTL));

	int i = 0, j = 0, nflag;
	for (i = 0; i < num; i++)
	{
		nflag = 0;
		for (j = 0; j < nInfo; j++)
		{
			if (pLnCtl[i].acln_id == pInfo[j].acln_id)
			{
				pLnCtl[i].is_monitor = pInfo[j].is_monitor;
				// 				pLnCtl[i].is_part_ana	= pInfo[j].is_part_ana;
				pLnCtl[i].is_end = pInfo[j].is_end;
				pLnCtl[i].sso_ln_swn = pInfo[j].sso_ln_swn;
				pLnCtl[i].sso_ln_ewn = pInfo[j].sso_ln_ewn;
				// 				sprintf(m_LogString,"LFO线路振幅限值[%f]-[%f]", pLnCtl[i].lfo_ln_swn,pLnCtl[i].lfo_ln_ewn);
				// 				MessShow(m_LogString);
				nflag = 1;
				break;
			}
		}
		if (nflag == 0)
		{
			sprintf(m_LogString, "未找到匹配线路["INT_REC_FMT"]", INT_REC_VAL(pLnCtl[i].acln_id));
			MessShow(m_LogString);
		}
	}

	SAFE_DELETE_ARRAY(pInfo);
	// 设备个数变化
	if (num != nInfo)
	{
		return 1;
	}
#endif
	return 0;
}

// 变压器参数
int CLfoAnaObj::GetTrCtl(int num, SSO_TR_CTL* pTrCtl)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_TR_NO_TAB, SSO_TR_CTL_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_TR_CTL);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO变压器表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO变压器表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_TR_CTL* pInfo = NULL;
	pInfo = new SSO_TR_CTL[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_TR_CTL));

	int i = 0, j = 0, nflag;
	for (i = 0; i < num; i++)
	{
		nflag = 0;
		for (j = 0; j < nInfo; j++)
		{
			if (pTrCtl[i].tr_id == pInfo[j].tr_id)
			{
				pTrCtl[i].is_monitor = pInfo[j].is_monitor;
				//                 pTrCtl[i].is_part_ana	= pInfo[j].is_part_ana;
				//                 pTrCtl[i].is_pow_calc	= pInfo[j].is_pow_calc;
				//                 pTrCtl[i].mon_wind_slct	= pInfo[j].mon_wind_slct;
				pTrCtl[i].sso_tr_swn = pInfo[j].sso_tr_swn;
				pTrCtl[i].sso_tr_ewn = pInfo[j].sso_tr_ewn;
				// 				sprintf(m_LogString,"LFO变压器振幅限值[%f]-[%f]", pTrCtl[i].lfo_tr_swn,pTrCtl[i].lfo_tr_ewn);
				// 				MessShow(m_LogString);
				nflag = 1;
				break;
			}
		}
		if (nflag == 0)
		{
			sprintf(m_LogString, "未找到匹配变压器["INT_REC_FMT"]", INT_REC_VAL(pTrCtl[i].tr_id));
			MessShow(m_LogString);
		}
	}

	SAFE_DELETE_ARRAY(pInfo);
	// 设备个数变化
	if (num != nInfo)
	{
		return 1;
	}
#endif
	return 0;
}

// LFO分析参数
int CLfoAnaObj::GetLfoPara(PARA_LFO_ANA& LfoPara)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_PARA_NO_TAB, TAB_PARA_SSO_ANA_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(TAB_PARA_SSO_ANA);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO参数表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO参数表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	TAB_PARA_SSO_ANA* pInfo = NULL;
	pInfo = new TAB_PARA_SSO_ANA[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(TAB_PARA_SSO_ANA));

	int parano = 0;
	LfoPara.mem_max_sec = pInfo[parano].mem_max_sec;                        //  缓冲最大秒数
	LfoPara.smp_freq = pInfo[parano].smp_freq;	                            //  采样频率
	LfoPara.flt_len = pInfo[parano].flt_len;	                            //  滤波窗宽
	LfoPara.flt_coe = pInfo[parano].flt_coe;	                            //  滤波系数
	LfoPara.time_leng = pInfo[parano].time_leng;	                        //  观察时间窗口
	LfoPara.int_time = pInfo[parano].int_time;	                            //  交叠窗口
	LfoPara.freq_min = pInfo[parano].freq_min;	                            //  频率下限
	LfoPara.freq_max = pInfo[parano].freq_max;	                            //  频率上限
	LfoPara.sso_jud_num = pInfo[parano].sso_jud_num;	                            //  判断次数
#ifdef _D5000
	LfoPara.volt_thre1 = pInfo[parano].volt_thre1;	                        //  电压等级1
	LfoPara.volt_thre2 = pInfo[parano].volt_thre2;	                        //  电压等级2
//广东项目增加
	LfoPara.gn_swn_ampl = pInfo[parano].gn_swn_ampl;	                        //  机组结束振荡振幅限值
	LfoPara.gn_ewn_ampl = pInfo[parano].gn_ewn_ampl;	                        //  线路结束振荡振幅限值
	LfoPara.ln_swn_ampl1 = pInfo[parano].ln_swn_ampl1;	                        //  线路开始振荡振幅限值1
	LfoPara.ln_swn_ampl2 = pInfo[parano].ln_swn_ampl2;	                        //  线路开始振荡振幅限值2
	LfoPara.ln_swn_ampl3 = pInfo[parano].ln_swn_ampl3;	                        //  线路开始振荡振幅限值3
	LfoPara.ln_ewn_ampl1 = pInfo[parano].ln_ewn_ampl1;	                        //  线路结束振荡振幅限值1
	LfoPara.ln_ewn_ampl2 = pInfo[parano].ln_ewn_ampl2;	                        //  线路结束振荡振幅限值2
	LfoPara.ln_ewn_ampl3 = pInfo[parano].ln_ewn_ampl3;	                        //  线路结束振荡振幅限值3
	LfoPara.tr_swn_ampl1 = pInfo[parano].tr_swn_ampl1;	                        //  变压器开始振荡振幅限值1
	LfoPara.tr_swn_ampl2 = pInfo[parano].tr_swn_ampl2;	                        //  变压器开始振荡振幅限值2
	LfoPara.tr_swn_ampl3 = pInfo[parano].tr_swn_ampl3;	                        //  变压器开始振荡振幅限值3
	LfoPara.tr_ewn_ampl1 = pInfo[parano].tr_ewn_ampl1;	                        //  变压器结束振荡振幅限值1
	LfoPara.tr_ewn_ampl2 = pInfo[parano].tr_ewn_ampl2;	                        //  变压器结束振荡振幅限值2
	LfoPara.tr_ewn_ampl3 = pInfo[parano].tr_ewn_ampl3;	                        //  变压器结束振荡振幅限值3
#endif
	SAFE_DELETE_ARRAY(pInfo)
		return 0;
#else
	// 从文件读取参数
	return -1;
#endif

}

// 显示控制参数
int CLfoAnaObj::GetDispCtl(PARA_DISP_CTL& DispCtl)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_PARA_NO_TAB, TAB_PARA_DISP_CTL_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(TAB_PARA_DISP_CTL);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO参数表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO参数表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	TAB_PARA_DISP_CTL* pInfo = NULL;
	pInfo = new TAB_PARA_DISP_CTL[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(TAB_PARA_DISP_CTL));

	int parano = 0;
	DispCtl.dsource = pInfo[parano].dsource;	                            //  数据源
// 	DispCtl.show_max	= pInfo[parano].show_max;									  //  最大允许显示
	DispCtl.sso_fac_id = pInfo[parano].sso_fac_id;	                            //  对应虚拟厂站ID
	DispCtl.is_warn = pInfo[parano].is_warn;	                            //  是否发送告警
	DispCtl.is_samp = pInfo[parano].is_samp;	                            //  是否触发采样
	SAFE_DELETE_ARRAY(pInfo)
		return 0;
#else
	// 从文件读取参数
	return -1;
#endif
}

// 进程控制
int CLfoAnaObj::GetProcCtl(PARA_PROC_CTL& ProcCtl)
{
	// #ifdef _WAMAP_
	// 	
	// 	CBuffer                buf_base;
	// 	int                    TabBufSize = 0;
	// 	int                    ret_code;
	// 	
	// 	TAB_PARA_PROC_CTL	  *p_ProcCtl=NULL;
	// 	int					   n_ProcCtl=0;
	// 	
	// 	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_PARA_NO, g_ctxNo)  == DB_OK )
	// 	{
	// 		ret_code = g_IDBTableOp.TableGet( TAB_PARA_PROC_CTL_FLD, buf_base );
	// 		if ( ret_code > 0 )
	// 		{
	// 			TabBufSize = sizeof(TAB_PARA_PROC_CTL);
	// 			n_ProcCtl = buf_base.GetLength()/TabBufSize; 
	// 			p_ProcCtl = (TAB_PARA_PROC_CTL *)calloc(n_ProcCtl, sizeof(TAB_PARA_PROC_CTL));
	// 			memset((void *)p_ProcCtl, 0, n_ProcCtl * sizeof(TAB_PARA_PROC_CTL));
	// 			memcpy((void*)p_ProcCtl, (void*)buf_base.GetBufPtr(), n_ProcCtl*TabBufSize);
	// 		}
	// 	}
	// 	
	// 	if (ret_code <=0)
	// 	{
	// 		TF_DelayMinSec(5);
	// 		if( p_ProcCtl )  {free(p_ProcCtl);  p_ProcCtl = NULL;}
	// 
	// 		if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_PARA_NO, g_ctxNo)  == DB_OK )
	// 		{
	// 			ret_code = g_IDBTableOp.TableGet( TAB_PARA_PROC_CTL_FLD, buf_base );
	// 			if ( ret_code > 0 )
	// 			{
	// 				TabBufSize = sizeof(TAB_PARA_PROC_CTL);
	// 				n_ProcCtl = buf_base.GetLength()/TabBufSize; 
	// 				p_ProcCtl = (TAB_PARA_PROC_CTL *)calloc(n_ProcCtl, sizeof(TAB_PARA_PROC_CTL));
	// 				memset((void *)p_ProcCtl, 0, n_ProcCtl * sizeof(TAB_PARA_PROC_CTL));
	// 				memcpy((void*)p_ProcCtl, (void*)buf_base.GetBufPtr(), n_ProcCtl*TabBufSize);
	// 			}
	// 		}
	// 	}
	//     if( n_ProcCtl <= 0 )
	// 	{
	// 		sprintf( m_LogString, "GetProcCtl(): LFO_PARA_NO[%d]  no record ", LFO_PARA_NO ); 
	// 		WriteLogFile( m_LogString );
	// 		
	// 		if( p_ProcCtl )  {free(p_ProcCtl);  p_ProcCtl = NULL;}
	// 		return -2;
	// 	}
	// 	
	// 	ProcCtl.proc_ctl = p_ProcCtl[0].mon_ctl;            /* 监视运行控制 */
	// 	
	// // 	ProcCtl.proc_sts= p_ProcCtl[0].mon_sts;	/* 监视运行状态 */
	// // 	memcpy(ProcCtl.proc_ctim, p_ProcCtl[0].mon_use_time,20);	/* 监视计算耗时 */
	// // 	memcpy(ProcCtl.proc_ctx, p_ProcCtl[0].mon_run_ctx,64);	/* 监视运行信息 */
	// 
	// 	if( p_ProcCtl )  {free(p_ProcCtl);  p_ProcCtl = NULL;}
	//     return 0;
	// #else
	//     // 从文件读取参数
	//     return -1;
	// #endif
	return 0;
}

///////////////////////////// 结果入库 ///////////////////////////////////

//  厂站监视信息
void CLfoAnaObj::UpdateMonFcTable(int num, SSO_FC_INF* pfc_inf, int num_gn, SSO_GN_MON* pgn_mon, int num_tr, SSO_TR_MON* ptr_mon)
{
#ifdef _WAMAP_
	sprintf(m_LogString, "更新lfo厂站表,厂站个数[%d]", num);
	MessShow(m_LogString);
	if (num < 0 || !pfc_inf) return;

	int ii, jj;
	float max_ampl;
	float max_freq;
	int stat;
	pair<multimap<INT_REC, int>::iterator, multimap<INT_REC, int>::iterator> range;
	multimap<INT_REC, int>::iterator it;
	TAB_LFO_FC_MON  temp_pfc_mon;
	for (ii = 0; ii < num; ii++)
	{
		max_ampl = 0;
		stat = 0;
		range = g_map_gn_fac.equal_range(pfc_inf[ii].fac_id);
		for (it = range.first; it != range.second; it++)
		{
			if (pgn_mon[it->second].lead_ampl > max_ampl)
			{
				max_ampl = pgn_mon[it->second].lead_ampl;
				max_freq = pgn_mon[it->second].lead_freq;
			}
			if (pgn_mon[it->second].stat > stat)
			{
				stat = pgn_mon[it->second].stat;
			}
		}
		range = g_map_tr_fac.equal_range(pfc_inf[ii].fac_id);
		for (it = range.first; it != range.second; it++)
		{
			if (ptr_mon[it->second].lead_ampl > max_ampl)
			{
				max_ampl = ptr_mon[it->second].lead_ampl;
				max_freq = ptr_mon[it->second].lead_freq;
			}
			if (ptr_mon[it->second].stat > stat)
			{
				stat = ptr_mon[it->second].stat;
			}
		}


		memset((void*)&temp_pfc_mon, 0, sizeof(TAB_LFO_FC_MON));
		temp_pfc_mon.lead_ampl = max_ampl;
		temp_pfc_mon.lead_freq = max_freq;
		temp_pfc_mon.stat = stat;
		if (temp_pfc_mon.stat == MENU_LFO_DEV_STS_WRN)
		{
			temp_pfc_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
			temp_pfc_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
		}
		else
		{
			temp_pfc_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
			temp_pfc_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
		}

		PODB_TableModifyByKey((const char*)&pfc_inf[ii].fac_idx, TAB_LFO_FC_MON_FLD, (const char*)&temp_pfc_mon.stat, sizeof(TAB_LFO_FC_MON), SSO_FAC_NO_TAB);

		if (temp_pfc_mon.stat > MENU_LFO_DEV_STS_SCT)
		{
			stat = MULTI_MENU(MENU_STATE_FAC_NOSTABIL);
		}
		else
		{
			stat = 0;
		}
#ifdef _NUSP
		if (pfc_inf[ii].fac_id.GetTableID() == PLANT_INFO_NO_TAB) {
			PODB_TableModifyByKey((const char*)&pfc_inf[ii].fac_id, "status", (const char*)&stat, sizeof(stat), PLANT_INFO_NO_TAB, AP_WAMS_SSO);
		}
		else if(pfc_inf[ii].fac_id.GetTableID() == SUBSTATION_INFO_NO_TAB) {
			PODB_TableModifyByKey((const char*)&pfc_inf[ii].fac_id, "status", (const char*)&stat, sizeof(stat), SUBSTATION_INFO_NO_TAB, AP_WAMS_SSO);
		}
		else {
			sprintf(m_LogString, "修改厂站表失败，厂站记录id【"INT_REC_FMT"】", INT_REC_VAL(pfc_inf[ii].fac_id));
			MessShow(m_LogString, LTERROR);
		}
#else
		PODB_TableModifyByKey((const char*)&pfc_inf[ii].fac_id, "status", (const char*)&stat, sizeof(stat), FAC_INFO_NO_TAB, AP_WAMS_SSO);
#endif // _NUSP		
	}
#endif
	return;
}

void CLfoAnaObj::UpdateMonFcTableStat(int num, SSO_FC_INF* pfc_inf, int num_gn, SSO_GN_MON* pgn_mon, int num_tr, SSO_TR_MON* ptr_mon)
{
#ifdef _WAMAP_
	sprintf(m_LogString, "更新lfo厂站表,厂站个数[%d]", num);
	MessShow(m_LogString);
	if (num < 0 || !pfc_inf) return;

	int ii, jj;
	float max_ampl;
	float max_freq;
	int stat;
	pair<multimap<INT_REC, int>::iterator, multimap<INT_REC, int>::iterator> range;
	multimap<INT_REC, int>::iterator it;
	TAB_LFO_FC_MON_STAT  temp_pfc_mon;
	for (ii = 0; ii < num; ii++)
	{
		max_ampl = 0;
		stat = 0;
		range = g_map_gn_fac.equal_range(pfc_inf[ii].fac_id);
		for (it = range.first; it != range.second; it++)
		{
			if (pgn_mon[it->second].lead_ampl > max_ampl)
			{
				max_ampl = pgn_mon[it->second].lead_ampl;
				max_freq = pgn_mon[it->second].lead_freq;
			}
			if (pgn_mon[it->second].stat > stat)
			{
				stat = pgn_mon[it->second].stat;
			}
		}
		range = g_map_tr_fac.equal_range(pfc_inf[ii].fac_id);
		for (it = range.first; it != range.second; it++)
		{
			if (ptr_mon[it->second].lead_ampl > max_ampl)
			{
				max_ampl = ptr_mon[it->second].lead_ampl;
				max_freq = ptr_mon[it->second].lead_freq;
			}
			if (ptr_mon[it->second].stat > stat)
			{
				stat = ptr_mon[it->second].stat;
			}
		}

		memset((void*)&temp_pfc_mon, 0, sizeof(TAB_LFO_FC_MON));
		temp_pfc_mon.lead_ampl = max_ampl;
		temp_pfc_mon.lead_freq = max_freq;
		temp_pfc_mon.stat = stat;
		if (temp_pfc_mon.stat == MENU_LFO_DEV_STS_WRN)
		{
			temp_pfc_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
			temp_pfc_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
		}
		else
		{
			temp_pfc_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
			temp_pfc_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
		}

		PODB_TableModifyByKey((const char*)&pfc_inf[ii].fac_idx, TAB_LFO_FC_MON_STAT_FLD, (const char*)&temp_pfc_mon.stat, sizeof(TAB_LFO_FC_MON), SSO_FAC_NO_TAB);

		if (temp_pfc_mon.stat > MENU_LFO_DEV_STS_SCT)
		{
			stat = MULTI_MENU(MENU_STATE_FAC_NOSTABIL);
		}
		else
		{
			stat = 0;
		}
#ifdef _NUSP
		if (pfc_inf[ii].fac_id.GetTableID() == PLANT_INFO_NO_TAB) {
			PODB_TableModifyByKey((const char*)&pfc_inf[ii].fac_id, "status", (const char*)&stat, sizeof(stat), PLANT_INFO_NO_TAB, AP_WAMS_SSO);
		}
		else if (pfc_inf[ii].fac_id.GetTableID() == SUBSTATION_INFO_NO_TAB) {
			PODB_TableModifyByKey((const char*)&pfc_inf[ii].fac_id, "status", (const char*)&stat, sizeof(stat), SUBSTATION_INFO_NO_TAB, AP_WAMS_SSO);
		}
		else {
			sprintf(m_LogString, "修改厂站表失败，厂站记录id【"INT_REC_FMT"】", INT_REC_VAL(pfc_inf[ii].fac_id));
			MessShow(m_LogString, LTERROR);
		}
#else
		PODB_TableModifyByKey((const char*)&pfc_inf[ii].fac_id, "status", (const char*)&stat, sizeof(stat), FAC_INFO_NO_TAB, AP_WAMS_SSO);
#endif // _NUSP
	}
#endif
	return;
}
//  机组监视信息
void CLfoAnaObj::UpdateMonGnTable(int num, SSO_GN_INF* pgn_inf, SSO_GN_MON* pgn_mon)
{
#ifdef _WAMAP_
	if (num < 0 || !pgn_inf || !pgn_mon) return;

	int ii;
	TAB_SSO_GN_MON  temp_pgn_mon;
	for (ii = 0; ii < num; ii++)
	{
		memset((void*)&temp_pgn_mon, 0, sizeof(TAB_SSO_GN_MON));
		temp_pgn_mon.stat = pgn_mon[ii].stat;
		temp_pgn_mon.stat_val = GetVarNsquare(2, pgn_mon[ii].stat - 1);
		temp_pgn_mon.lead_ampl = pgn_mon[ii].lead_ampl;
		temp_pgn_mon.lead_freq = pgn_mon[ii].lead_freq;
		if (temp_pgn_mon.stat == MENU_LFO_DEV_STS_WRN)
		{
			temp_pgn_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
			temp_pgn_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
		}
		else
		{
			temp_pgn_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
			temp_pgn_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
		}
		temp_pgn_mon.if_sso = pgn_mon[ii].if_sso;
		temp_pgn_mon.sso_bgn_t = pgn_mon[ii].sso_bgn;
		temp_pgn_mon.sso_end_t = pgn_mon[ii].sso_end;
		sprintf(temp_pgn_mon.sso_time, "%s", pgn_mon[ii].sso_time);
		temp_pgn_mon.max_ampl = pgn_mon[ii].max_ampl;
		temp_pgn_mon.lfo_max_ampl_t = pgn_mon[ii].lfo_max_ampl_t;
		temp_pgn_mon.aver_freq1 = pgn_mon[ii].aver_freq1;

		if (pgn_inf[ii].gn_id == m_SysMon.warn_dev_id)
		{
			temp_pgn_mon.sso_mon = MENU_LFO_MENU_YES;
		}
		else
		{
			temp_pgn_mon.sso_mon = MENU_LFO_MENU_NO;
		}

		PODB_TableModifyByKey((const char*)&pgn_inf[ii].gn_idx, TAB_SSO_GN_MON_FLD, (const char*)&temp_pgn_mon.stat, sizeof(TAB_SSO_GN_MON), SSO_GN_NO_TAB);
	}
#endif	
	return;
}

void CLfoAnaObj::UpdateMonGnTableStat(int num, SSO_GN_INF* pgn_inf, SSO_GN_MON* pgn_mon)
{
#ifdef _WAMAP_
	if (num < 0 || !pgn_inf || !pgn_mon) return;

	int ii;
	tab_SSO_GN_MON_STAT  temp_pgn_mon;
	for (ii = 0; ii < num; ii++)
	{
		memset((void*)&temp_pgn_mon, 0, sizeof(tab_SSO_GN_MON_STAT));
		temp_pgn_mon.stat = pgn_mon[ii].stat;
		temp_pgn_mon.stat_val = GetVarNsquare(2, pgn_mon[ii].stat - 1);
		temp_pgn_mon.lead_ampl = pgn_mon[ii].lead_ampl;
		temp_pgn_mon.lead_freq = pgn_mon[ii].lead_freq;
		if (temp_pgn_mon.stat == MENU_LFO_DEV_STS_WRN)
		{
			temp_pgn_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
			temp_pgn_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
		}
		else
		{
			temp_pgn_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
			temp_pgn_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
		}

		PODB_TableModifyByKey((const char*)&pgn_inf[ii].gn_idx, TAB_SSO_GN_MON_STAT_FLD, (const char*)&temp_pgn_mon.stat, sizeof(tab_SSO_GN_MON_STAT), SSO_GN_NO_TAB);
	}
#endif	
	return;
}

//  线路监视信息
void CLfoAnaObj::UpdateMonLnTable(int num, SSO_LN_INF* pln_inf, SSO_LN_MON* pln_mon)
{
#ifdef _WAMAP_
	if (num < 0 || !pln_inf || !pln_mon) return;

	int ii, stat;
	float p = 0.0;
	pair<multimap<INT_REC, INT_REC>::iterator, multimap<INT_REC, INT_REC>::iterator> range;
	multimap<INT_REC, INT_REC>::iterator it;
	TAB_SSO_LN_MON  temp_pln_mon;
	for (ii = 0; ii < num; ii++)
	{
		memset((void*)&temp_pln_mon, 0, sizeof(TAB_SSO_LN_MON));
		// 		temp_pln_mon.pwr_val		= pln_mon[ii].pwr_val;
		temp_pln_mon.stat = pln_mon[ii].stat;
		temp_pln_mon.stat_val = GetVarNsquare(2, pln_mon[ii].stat - 1);
		temp_pln_mon.lead_ampl = pln_mon[ii].lead_ampl;
		temp_pln_mon.lead_freq = pln_mon[ii].lead_freq;
		if (temp_pln_mon.stat == MENU_LFO_DEV_STS_WRN)
		{
			temp_pln_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
			temp_pln_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
		}
		else
		{
			temp_pln_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
			temp_pln_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
		}
		temp_pln_mon.if_sso = pln_mon[ii].if_sso;
		temp_pln_mon.sso_bgn_t = pln_mon[ii].sso_bgn;
		temp_pln_mon.sso_end_t = pln_mon[ii].sso_end;
		sprintf(temp_pln_mon.sso_time, "%s", pln_mon[ii].sso_time);
		temp_pln_mon.max_ampl = pln_mon[ii].max_ampl;
		temp_pln_mon.lfo_max_ampl_t = pln_mon[ii].lfo_max_ampl_t;
		temp_pln_mon.aver_freq1 = pln_mon[ii].aver_freq1;
		if (pln_inf[ii].acln_id == m_SysMon.warn_dev_id)
		{
			temp_pln_mon.sso_mon = MENU_LFO_MENU_YES;
		}
		else
		{
			temp_pln_mon.sso_mon = MENU_LFO_MENU_NO;
		}
		sprintf(m_LogString, " temp_pln_mon.sso_time=[%s],pln_mon[ii].sso_time=[%s]", temp_pln_mon.sso_time, pln_mon[ii].sso_time);
		MessShow(m_LogString);

		PODB_TableModifyByKey((const char*)&pln_inf[ii].acln_idx, TAB_SSO_LN_MON_FLD, (const char*)&temp_pln_mon.stat, sizeof(TAB_SSO_LN_MON), SSO_ACLN_NO_TAB);

		range = g_map_ac_dot.equal_range(pln_inf[ii].acln_id);
		for (it = range.first; it != range.second; ++it)
		{
			if (pln_mon[ii].stat > MENU_LFO_DEV_STS_SCT)
			{
				stat = MULTI_MENU(MENU_STATE_DEV_P_HIGH1);
			}
			else
			{
				stat = MULTI_MENU(MENU_STATE_DEV_NORMAL);
			}
			sprintf(m_LogString, "更新线路[%s]端点["INT_REC_FMT"],状态[%d]", pln_inf[ii].acln_name, INT_REC_VAL(it->second), stat);
			MessShow(m_LogString);
			PODB_TableModifyByKey((const char*)&it->second, "status", (const char*)&stat, sizeof(stat), ACLN_DOT_NO_TAB, AP_WAMS_SSO);
			PODB_TableModifyByKey((const char*)&it->second, "p", (const char*)&p, sizeof(p), ACLN_DOT_NO_TAB, AP_WAMS_SSO);
		}
	}
	// 	if (m_DispCtl.dsource != MENU_LFO_DATA_RT)
	// 	{
	// 		stat = MULTI_MENU(MENU_STATE_DEV_P_HIGH1);
	// 		for(ii = 0;ii< m_vAcDot_id.size();ii++)
	// 		{
	// 			PODB_TableModifyByKey((const char *)&m_vAcDot_id[ii], "status", (const char *)&stat, sizeof(stat),ACLN_DOT_NO_TAB,AP_WAMS_SSO);
	// 			PODB_TableModifyByKey((const char *)&m_vAcDot_id[ii], "p", (const char *)&p, sizeof(p),ACLN_DOT_NO_TAB,AP_WAMS_SSO);
	// 		}
	// 	}
#endif

	return;
}

void CLfoAnaObj::UpdateMonLnTableStat(int num, SSO_LN_INF* pln_inf, SSO_LN_MON* pln_mon)
{
#ifdef _WAMAP_
	if (num < 0 || !pln_inf || !pln_mon) return;

	int ii;
	INT_REC recordid;
	map<INT_REC, int>::iterator it;
	TAB_SSO_LN_MON_STAT  temp_pln_mon;
	float p = 0.0;
	for (ii = 0; ii < num; ii++)
	{
		memset((void*)&temp_pln_mon, 0, sizeof(TAB_SSO_LN_MON_STAT));
		// 		temp_pln_mon.pwr_val		= pln_mon[ii].pwr_val;
		temp_pln_mon.stat = pln_mon[ii].stat;
		temp_pln_mon.stat_val = GetVarNsquare(2, pln_mon[ii].stat - 1);
		temp_pln_mon.lead_ampl = pln_mon[ii].lead_ampl;
		temp_pln_mon.lead_freq = pln_mon[ii].lead_freq;
		if (temp_pln_mon.stat == MENU_LFO_DEV_STS_WRN)
		{
			temp_pln_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
			temp_pln_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
		}
		else
		{
			temp_pln_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
			temp_pln_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
		}

		PODB_TableModifyByKey((const char*)&pln_inf[ii].acln_idx, TAB_SSO_LN_MON_STAT_FLD, (const char*)&temp_pln_mon.stat, sizeof(TAB_SSO_LN_MON_STAT), SSO_ACLN_NO_TAB);
	}
	int stat = MULTI_MENU(MENU_STATE_DEV_OUTAGE);
	for (it = g_map_acln_dot.begin(); it != g_map_acln_dot.end(); it++)
	{
		recordid = it->first;
		sprintf(m_LogString, "更新线路端点[%ld]", recordid);
		MessShow(m_LogString);
		PODB_TableModifyByKey((const char*)&recordid, "status", (const char*)&stat, sizeof(int), ACLN_DOT_NO_TAB, AP_WAMS_SSO);
		PODB_TableModifyByKey((const char*)&recordid, "p", (const char*)&p, sizeof(p), ACLN_DOT_NO_TAB, AP_WAMS_SSO);
	}
	// 	if (m_DispCtl.dsource != MENU_LFO_DATA_RT)
	// 	{
	// 		for(ii = 0;ii< m_vAcDot_id.size();ii++)
	// 		{
	// 			recordid = m_vAcDot_id[ii];
	// 			PODB_TableModifyByKey((const char *)&recordid, "status", (const char *)&stat, sizeof(int),ACLN_DOT_NO_TAB,AP_WAMS_SSO);
	// 			PODB_TableModifyByKey((const char *)&recordid, "p", (const char *)&p, sizeof(p),ACLN_DOT_NO_TAB,AP_WAMS_SSO);
	// 		}
	// 	}
#endif

	return;
}

//  变压器监视信息
void CLfoAnaObj::UpdateMonTrTable(int num, SSO_TR_INF* ptr_inf, SSO_TR_MON* ptr_mon)
{

#ifdef _WAMAP_
	if (num < 0 || !ptr_inf || !ptr_mon) return;

	int ii;
	TAB_SSO_TR_MON  temp_ptr_mon;
	for (ii = 0; ii < num; ii++)
	{
		memset((void*)&temp_ptr_mon, 0, sizeof(TAB_SSO_TR_MON));
		// 		temp_ptr_mon.pwr_val			= ptr_mon[ii].pwr_val;
		temp_ptr_mon.stat = ptr_mon[ii].stat;
		temp_ptr_mon.stat_val = GetVarNsquare(2, ptr_mon[ii].stat - 1);
		temp_ptr_mon.lead_ampl = ptr_mon[ii].lead_ampl;
		temp_ptr_mon.lead_freq = ptr_mon[ii].lead_freq;
		if (temp_ptr_mon.stat == MENU_LFO_DEV_STS_WRN)
		{
			temp_ptr_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
			temp_ptr_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
		}
		else
		{
			temp_ptr_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
			temp_ptr_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
		}
		temp_ptr_mon.if_sso = ptr_mon[ii].if_sso;
		temp_ptr_mon.sso_bgn_t = ptr_mon[ii].sso_bgn;
		temp_ptr_mon.sso_end_t = ptr_mon[ii].sso_end;
		sprintf(temp_ptr_mon.sso_time, "%s", ptr_mon[ii].sso_time);
		temp_ptr_mon.max_ampl = ptr_mon[ii].max_ampl;
		temp_ptr_mon.lfo_max_ampl_t = ptr_mon[ii].lfo_max_ampl_t;
		temp_ptr_mon.aver_freq1 = ptr_mon[ii].aver_freq1;
		// 		sprintf( m_LogString," temp_ptr_mon.sso_time=[%s],ptr_mon[ii].sso_time=[%s]",temp_ptr_mon.sso_time,ptr_mon[ii].sso_time);
		// 		MessShow(m_LogString);
		if (ptr_inf[ii].tr_id == m_SysMon.warn_dev_id)
		{
			temp_ptr_mon.sso_mon = MENU_LFO_MENU_YES;
		}
		else
		{
			temp_ptr_mon.sso_mon = MENU_LFO_MENU_NO;
		}

		PODB_TableModifyByKey((const char*)&ptr_inf[ii].tr_idx, TAB_SSO_TR_MON_FLD, (const char*)&temp_ptr_mon.stat, sizeof(TAB_SSO_TR_MON), SSO_TR_NO_TAB);
	}
#endif

	return;
}

void CLfoAnaObj::UpdateMonTrTableStat(int num, SSO_TR_INF* ptr_inf, SSO_TR_MON* ptr_mon)
{
#ifdef _WAMAP_
	if (num < 0 || !ptr_inf || !ptr_mon) return;

	int ii;
	TAB_SSO_TR_MON_STAT  temp_ptr_mon;
	for (ii = 0; ii < num; ii++)
	{
		memset((void*)&temp_ptr_mon, 0, sizeof(TAB_SSO_TR_MON_STAT));
		// 		temp_ptr_mon.pwr_val			= ptr_mon[ii].pwr_val;
		temp_ptr_mon.stat = ptr_mon[ii].stat;
		temp_ptr_mon.stat_val = GetVarNsquare(2, ptr_mon[ii].stat - 1);
		temp_ptr_mon.lead_ampl = ptr_mon[ii].lead_ampl;
		temp_ptr_mon.lead_freq = ptr_mon[ii].lead_freq;
		if (temp_ptr_mon.stat == MENU_LFO_DEV_STS_WRN)
		{
			temp_ptr_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
			temp_ptr_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_WARN - 1);
		}
		else
		{
			temp_ptr_mon.lead_amplclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
			temp_ptr_mon.lead_freqclr = GetVarNsquare(2, MENU_EACCS_SAFESTATUS_NORMAL - 1);
		}

		PODB_TableModifyByKey((const char*)&ptr_inf[ii].tr_idx, TAB_SSO_TR_MON_STAT_FLD, (const char*)&temp_ptr_mon.stat, sizeof(TAB_SSO_TR_MON_STAT), SSO_TR_NO_TAB);
	}
#endif
}
//////////////////////////////// 更新设备表LFO状态 ////////////////////////////////
//  厂站信息表LFO状态
void CLfoAnaObj::UpdateFcInfTableLfoStat(int num, SSO_FC_MON* pfc_mon)
{
	// 	if ( para_UpDevLfoStat == 0 )
	// 	{
	// 		sprintf( m_LogString, "UpdateFcInfTableLfoStat(): 不要求更新厂站信息表LFO状态" ); 
	// 		MessShow( m_LogString );
	// 		return;
	// 	}
	// #ifdef _WAMAP_
	// 	if ( num < 0 || !pfc_mon ) return;
	// 	vector<FAC_TAB_CRE> VecFacTab;
	// 	if (PODB_GetFacInfo(VecFacTab)<0)
	// 	{
	// 		return ;
	// 	}
	// 
	// 	int ii,jj;
	// 	TAB_FAC_INFO_WAMAP tempfacinfo;
	// 	for ( ii =0; ii<(int)VecFacTab.size(); ii++)
	// 	{
	// 		memset((void *)&tempfacinfo,0,sizeof(TAB_FAC_INFO_WAMAP));
	// 		tempfacinfo.lfo_fact = 0.0f;
	//         tempfacinfo.lfo_clus = MENU_LFO_CLUST_RMAIN;  //  lfo所属群
	//         tempfacinfo.is_cntr  = MENU_LFO_MON_NO;       //  lfo是否中心
	// 		for (jj =0; jj<num; jj++)
	// 		{
	// 			if (VecFacTab[ii].fac_id == pfc_mon[jj].fac_id )
	// 			{
	// 				tempfacinfo.lfo_clus = pfc_mon[jj].clus;
	//                 tempfacinfo.is_cntr  = pfc_mon[jj].is_cntr;
	//                 break;
	// 			}
	// 		}
	// 		PODB_TableModifyByKey((const char *)&VecFacTab[ii].fac_id,TAB_FAC_INFO_WAMAP_FLD,(const char *)&tempfacinfo,sizeof(TAB_FAC_INFO_WAMAP),FAC_INFO_NO_TAB);
	// 	}
	// #endif
	return;
}

//  发电机表LFO状态
void CLfoAnaObj::UpdateGnDevTableLfoStat(MODE_CHRCT& DispMode)
{
	// 	if ( para_UpDevLfoStat == 0 )
	// 	{
	// 		sprintf( m_LogString, "UpdateGnDevTableLfoStat: 不要求更新发电机表LFO状态 " ); 
	// 		MessShow( m_LogString );
	// 		return;
	// 	}
	// #ifdef _WAMAP_
	// 	vector<GEN_TAB_CRE> VecGenTab;
	// 	if (PODB_GetGenInfo(VecGenTab)<0)
	// 	{
	// 		return;
	// 	}
	// 
	// 	int ii,jj;
	// 	TAB_GEN_DEVICE_WAMAP tempgeninfo;
	// 	for ( ii =0; ii<(int)VecGenTab.size(); ii++)
	// 	{
	// 		memset((void *)&tempgeninfo,0,sizeof(TAB_GEN_DEVICE_WAMAP));
	// 		for ( jj =0; jj<DispMode.nPartGn; jj++)
	// 		{
	// 			if ( VecGenTab[ii].gn_id == DispMode.PartGn[jj].obj_id)
	//             {
	// 				tempgeninfo.lfo_fact  = DispMode.PartGn[jj].fact;
	//                 tempgeninfo.lfo_clus  = DispMode.PartGn[jj].clus;
	//                 break;
	//             }
	// 		}
	// 		PODB_TableModifyByKey((const char *)&VecGenTab[ii].gn_id,TAB_GEN_DEVICE_WAMAP_FLD,(const char *)&tempgeninfo,sizeof(TAB_GEN_DEVICE_WAMAP),GN_DEVICE_NO_TAB);
	// 	}
	// #endif
	return;
}

//  交流线段表LFO状态
void CLfoAnaObj::UpdateLnDevTableLfoStat(int num, SSO_LN_MON* pln_mon)
{
	// 	if ( para_UpDevLfoStat == 0 )
	// 	{
	// 		sprintf( m_LogString, "UpdateLnDevTableLfoStat(): 不要求更新交流线段表LFO状态 " ); 
	// 		MessShow( m_LogString );
	// 		return;
	// 	}
	// 
	// #ifdef _WAMAP_
	// 	if ( num < 0 || !pln_mon ) return;
	// 	
	// 	vector<ACLINE_TAB_CRE> VecAclineTab;
	// 	if (PODB_GetAclnInfo(VecAclineTab)<0)
	// 	{
	// 		return;
	// 	}
	// 
	// 	int ii,jj;
	// 	TAB_ACLN_DEVICE_WAMAP tmpaclineinfo;
	// 	for ( ii =0; ii<(int)VecAclineTab.size(); ii++)
	// 	{
	// 		memset((void *)&tmpaclineinfo,0,sizeof(TAB_ACLN_DEVICE_WAMAP));
	// 		tmpaclineinfo.if_lfo = MENU_LFO_MON_NO;
	// 		for ( jj = 0; jj<num; jj++)
	// 		{
	// 			if ((VecAclineTab[ii].acln_id == pln_mon[jj].acln_id) && (pln_mon[jj].stat > MENU_LFO_DEV_STS_FWN))
	// 			{
	// 				tmpaclineinfo.if_lfo = MENU_LFO_MON_YES;
	// 				break;
	// 			}
	// 		}
	// 
	// 		PODB_TableModifyByKey((const char *)&VecAclineTab[ii].acln_id,TAB_ACLN_DEVICE_WAMAP_FLD,(const char *)&tmpaclineinfo,sizeof(TAB_ACLN_DEVICE_WAMAP),ACLN_DEVICE_NO_TAB);
	// 	}
	// #endif
	return;
}
//////////////////////////////////////////////////////////////////////////

//  obj_mode信息
void CLfoAnaObj::UpdateMonMdTable(int num, MODE_PART_OBJ* pobj_mode)
{
#ifdef _WAMAP_
	if (num < 1 || !pobj_mode) return;

	TAB_LFO_DEVICE_MODE* m_pobj_mode = new TAB_LFO_DEVICE_MODE[num];
	INT_REC recordid = PODB_GetDevIdByKeyID(LFO_ALLDEV_MODE_NO_TAB, 1);
	for (int ii = 0; ii < num; ii++)
	{
		m_pobj_mode[ii].record_id = recordid;
		m_pobj_mode[ii].dev_id = pobj_mode[ii].obj_id;
		m_pobj_mode[ii].dev_type = pobj_mode[ii].type;
		m_pobj_mode[ii].is_show = MENU_LFO_MENU_NO;
		m_pobj_mode[ii].ampl = pobj_mode[ii].ampl;
		m_pobj_mode[ii].freq = pobj_mode[ii].freq;
		m_pobj_mode[ii].damp = pobj_mode[ii].damp;
		m_pobj_mode[ii].phas = pobj_mode[ii].phas;
		m_pobj_mode[ii].real = pobj_mode[ii].real;
		m_pobj_mode[ii].imag = pobj_mode[ii].imag;
		m_pobj_mode[ii].engy = pobj_mode[ii].engy;
		m_pobj_mode[ii].fail = 0;

		IncrRtdbId(recordid);
	}

	PODB_TableClear(LFO_ALLDEV_MODE_NO_TAB);

	int ret_code = PODB_TableWrite((const char*)&m_pobj_mode[0].record_id, num * sizeof(TAB_LFO_DEVICE_MODE), LFO_ALLDEV_MODE_NO_TAB);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "写SSO实时设备模式全表[app：%d，table：%d]失败\n", g_appNo, LFO_ALLDEV_MODE_NO_TAB);
		MessShow(m_LogString);
	}

	delete m_pobj_mode;
#endif // _DEBUG
	return;
}

//  电网信息
void CLfoAnaObj::UpdateMonSysTable(SSO_SYS_MON sys_mon)
{
#ifdef _WAMAP_
	INT_REC keyrecord = PODB_GetDevIdByKeyID(SSO_PARA_NO_TAB, 1);
	SSO_SYS_SSO tmp_sso_para;
	memset(&tmp_sso_para, 0, sizeof(SSO_SYS_SSO));
	tmp_sso_para.stat = sys_mon.stat;
	tmp_sso_para.stat_val = GetVarNsquare(2, sys_mon.stat - 1);
	if (sys_mon.sso_gn_num < 1)
	{
		tmp_sso_para.sso_gn_num = DB_PIC_NO_DIS;
	}
	else
	{
		tmp_sso_para.sso_gn_num = sys_mon.sso_gn_num;
	}

	if (sys_mon.sso_ln_num < 1)
	{
		tmp_sso_para.sso_ln_num = DB_PIC_NO_DIS;
	}
	else
	{
		tmp_sso_para.sso_ln_num = sys_mon.sso_ln_num;
	}

	if (sys_mon.sso_tr_num < 1)
	{
		tmp_sso_para.sso_tr_num = DB_PIC_NO_DIS;
	}
	else
	{
		tmp_sso_para.sso_tr_num = sys_mon.sso_tr_num;
	}
	if (sys_mon.stat == MENU_LFO_DEV_STS_WRN)
	{
		m_nWarnWdsnum++;
		m_fAverFreq = sys_mon.mon_freq + m_fAverFreq;
	}
	tmp_sso_para.lead_freq = m_fAverFreq / m_nWarnWdsnum;

	tmp_sso_para.lead_ampl = sys_mon.mon_ampl;

	tmp_sso_para.warn_gn_num = tmp_sso_para.sso_gn_num;
	tmp_sso_para.warn_ln_num = tmp_sso_para.sso_ln_num;
	tmp_sso_para.warn_tr_num = tmp_sso_para.sso_tr_num;
	tmp_sso_para.sso_bgn = sys_mon.sso_bgn;
	tmp_sso_para.sso_end = sys_mon.sso_end;
	tmp_sso_para.warn_dev_id = sys_mon.warn_dev_id;
	tmp_sso_para.warn_freq = sys_mon.warn_freq;
	tmp_sso_para.is_sso = MENU_LFO_MENU_YES;
	sprintf(tmp_sso_para.warn_dev_name, "%s", sys_mon.warn_dev_name);
	sprintf(tmp_sso_para.sso_time, "%s", sys_mon.sso_time);
	sprintf(tmp_sso_para.stat_ctx, "%s", sys_mon.stat_ctx);
	sprintf(m_LogString, "lns_test 写SSO参数表,record_id["INT_REC_FMT"],stat[%d],case_des[%s],warn_freq[%f],lead_ampl[%f]", INT_REC_VAL(keyrecord), tmp_sso_para.stat, tmp_sso_para.stat_ctx,tmp_sso_para.warn_freq,tmp_sso_para.lead_ampl);
	MessShow(m_LogString);
	// 	PODB_TableModifyByKey((const char *)&keyrecord, "stat,stat_val", (const char *)&tmp_sso_para.stat, 2*sizeof(int),SSO_PARA_NO_TAB);
	// 	PODB_TableModifyByKey((const char *)&keyrecord, "sso_ln_num,sso_gn_num,sso_tr_num", (const char *)&tmp_sso_para.sso_ln_num, 3*sizeof(int),SSO_PARA_NO_TAB);

	PODB_TableModifyByKey((const char*)&keyrecord, SSO_SYS_MON_FLD, (const char*)&tmp_sso_para.stat, sizeof(SSO_SYS_SSO), SSO_PARA_NO_TAB);
#endif
	return;
}

void CLfoAnaObj::UpdateMonSysTableStat(SSO_SYS_MON sys_mon)
{
#ifdef _WAMAP_
	INT_REC keyrecord = PODB_GetDevIdByKeyID(SSO_PARA_NO_TAB, 1);
	SSO_SYS_SSO tmp_sso_para;
	memset(&tmp_sso_para, 0, sizeof(SSO_SYS_SSO));
	tmp_sso_para.stat = sys_mon.stat;
	tmp_sso_para.stat_val = GetVarNsquare(2, sys_mon.stat - 1);
	PODB_TableModifyByKey((const char*)&keyrecord, "stat,stat_val", (const char*)&tmp_sso_para.stat, 2 * sizeof(int), SSO_PARA_NO_TAB);
	// 	PODB_TableModifyByKey((const char *)&keyrecord, "sso_ln_num,sso_gn_num,sso_tr_num", (const char *)&tmp_sso_para.sso_ln_num, 3*sizeof(int),SSO_PARA_NO_TAB);

// 	PODB_TableModifyByKey((const char *)&keyrecord, SSO_SYS_MON_FLD, (const char *)&tmp_sso_para.stat, sizeof(SSO_SYS_SSO),SSO_PARA_NO_TAB);
#endif
	return;
}

int CLfoAnaObj::UpdateAppVariable(SSO_SYS_MON sys_mon)
{
#ifdef _WAMAP_

#ifdef _WAMS_APP_WARN
	if (para_UpAppVariable == 0)
	{
		// 不更新
		return 0;
	}

	if (sys_mon.stat <= MENU_LFO_DEV_STS_SCT)
	{
		// 系统安全,不更新
		return 0;
	}

	WAMS_APP_WARN_INFO_WAMS_LFO LfoAppWarn;
	memset((void*)&LfoAppWarn, 0, sizeof(WAMS_APP_WARN_INFO_WAMS_LFO));

	LfoAppWarn.warn_level = MENU_SMART_WARN_LEVEL_2; // 二级告警
	sprintf(LfoAppWarn.app_info, "低频振荡告警\n振荡机组数:%d\n振荡线路数:%d\n振荡变压数:%d", sys_mon.warn_gn_num, sys_mon.warn_ln_num, sys_mon.warn_tr_num);
	LfoAppWarn.refresh_tm = sys_mon.sso_bgn;
	LfoAppWarn.app_info_1 = sys_mon.sso_gn_num;
	sprintf(LfoAppWarn.app_info_desc_1, "振荡机组数:%d", sys_mon.sso_gn_num);
	LfoAppWarn.app_info_2 = sys_mon.sso_ln_num;
	sprintf(LfoAppWarn.app_info_desc_2, "振荡线路数:%d", sys_mon.sso_ln_num);
	LfoAppWarn.app_info_3 = sys_mon.sso_tr_num;
	sprintf(LfoAppWarn.app_info_desc_3, "振荡变压器数:%d", sys_mon.sso_tr_num);
	LfoAppWarn.is_popup_theme = MENU_YES;

	INT_REC record_id = PODB_GetDevIdByKeyID(WAMS_APP_WARN_INFO_NO_TAB, 3);  // 第3条记录
	int ret_code = PODB_TableModifyByKey((const char*)&record_id, WAMS_APP_WARN_INFO_WAMS_LFO_DEFFIELDS, (const char*)&LfoAppWarn, sizeof(WAMS_APP_WARN_INFO_WAMS_LFO), WAMS_APP_WARN_INFO_NO_TAB);
	if (ret_code != DB_OK)
	{
		sprintf(m_LogString, "WAMS应用模块告警信息表[%d],修改失败["INT_REC_FMT"],%s,is_popup_theme=[%d]", WAMS_APP_WARN_INFO_NO_TAB, INT_REC_VAL(record_id), LfoAppWarn.app_info, LfoAppWarn.is_popup_theme);
		MessShow(m_LogString);
	}
#endif
#endif
	return 0;
}
//  厂站lfo结果
void CLfoAnaObj::UpdateLfoFcTable(int num, SSO_FC_INF* pfc_inf, SSO_FC_MON* pfc_lfo)
{/*
#ifdef _WAMAP_
	if ( num < 0 || !pfc_inf || !pfc_lfo ) return;

	int ii;

	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_FAC_NO, g_contextNo ) == DB_OK )
	{
		for ( ii = 0; ii<num; ii++ )
		{
			g_IDBTableOp.TableModifyByKey((const char *)&pfc_inf[ii].fac_idx, LFO_FC_LFO_FLD, (const char *)&pfc_lfo[ii], sizeof(LFO_FC_LFO));
		}
	}
#endif */
	return;
}

//  机组lfo结果
void CLfoAnaObj::UpdateLfoGnTable(int num, SSO_GN_INF* pgn_inf, LFO_OBJ_RES_SAVE* pgn_lfo)
{
	// #ifdef _WAMAP_
	// 	if ( num < 0 || !pgn_inf || !pgn_lfo ) return;
	// 
	// 	int i,j;
	//     INT_REC gn_idx =0;
	// 	LFO_GN_LFO  temp_pgn;
	// 	for(i=0; i<num; i++)
	// 	{
	// 		memset((void *)&temp_pgn,0,sizeof(LFO_GN_LFO));
	// 		gn_idx = 0;
	// 		for( j=0; j<num; j++)
	// 		{
	// 			if( pgn_lfo[i].obj_id == pgn_inf[j].gn_id )
	// 			{
	// 				gn_idx = pgn_inf[j].gn_idx;
	// 				break;
	// 			}
	// 		}
	// 		
	// 		//if (pgn_lfo[i].lfo_b_t>0)
	// 		{
	// 			temp_pgn.if_lfo		     = pgn_lfo[i].if_lfo;
	// 			temp_pgn.lfo_bgn_val     = pgn_lfo[i].b_aver_pqv0[0];
	// 			temp_pgn.lfo_end_val     = pgn_lfo[i].e_aver_pqv0[0];
	// 			temp_pgn.slfo_bgn_t      = pgn_lfo[i].lfo_b_t;
	// 			temp_pgn.slfo_end_t      = pgn_lfo[i].lfo_e_t;
	// 			strcpy(temp_pgn.slfo_time, PT_GetStringOfTimeInterval(pgn_lfo[i].hold_time*1000));
	// 			temp_pgn.slfo_bgn_val    = pgn_lfo[i].b_aver_pqv1[0];
	// 			temp_pgn.slfo_end_val    = pgn_lfo[i].e_aver_pqv1[0];
	// 			temp_pgn.lfo_max_val     = pgn_lfo[i].max_value.value;
	// 			temp_pgn.lfo_max_val_t   = pgn_lfo[i].max_value.time;
	// 			temp_pgn.lfo_min_val	 = pgn_lfo[i].min_value.value;
	// 			temp_pgn.lfo_min_val_t   = pgn_lfo[i].min_value.time;
	// 			temp_pgn.lfo_max_min_dif = fabs(temp_pgn.lfo_max_val - temp_pgn.lfo_min_val);
	// 			temp_pgn.aver_freq1      = pgn_lfo[i].mode1[0].freq;
	// 			temp_pgn.aver_freq2      = pgn_lfo[i].max_min_dif *2;  // 平均频率改为单摆振幅（峰峰值）
	// 		}
	// 
	// 		if (gn_idx > 0)
	// 		{
	// 			PODB_TableModifyByKey((const char *)&gn_idx, LFO_GN_LFO_FLD, (const char *)&temp_pgn.if_lfo, sizeof(LFO_GN_LFO),LFO_GN_NO_TAB);
	// 		}
	// 	}
	// #endif
	return;
}

//  线路lfo结果
void CLfoAnaObj::UpdateLfoLnTable(int num, SSO_LN_INF* pln_inf, LFO_OBJ_RES_SAVE* pln_lfo)
{
	// #ifdef _WAMAP_
	// 	if ( num < 0 || !pln_inf || !pln_lfo ) return;
	// 
	// 	int i, j;
	//     //int ret_code = -1;
	//     INT_REC acln_idx =0;
	// 	LFO_LN_LFO  temp_pln;
	// 	for(i=0; i<num; i++)
	// 	{
	// 		memset((void *)&temp_pln,0,sizeof(LFO_LN_LFO));
	// 		acln_idx = 0;
	// 		for( j=0; j<num; j++)
	// 		{
	// 			if( pln_lfo[i].obj_id == pln_inf[j].acln_id )
	// 			{
	// 				acln_idx = pln_inf[j].acln_idx;
	// 				break;
	// 			}
	// 		}
	// 		//if (pln_lfo[i].lfo_b_t>0)
	// 		{
	// 			temp_pln.if_lfo		     = pln_lfo[i].if_lfo;
	// 			temp_pln.lfo_bgn_val     = pln_lfo[i].b_aver_pqv0[0];
	// 			temp_pln.lfo_end_val     = pln_lfo[i].e_aver_pqv0[0];
	// 			temp_pln.slfo_bgn_t      = pln_lfo[i].lfo_b_t;
	// 			temp_pln.slfo_end_t      = pln_lfo[i].lfo_e_t;
	// 			strcpy(temp_pln.slfo_time, PT_GetStringOfTimeInterval(pln_lfo[i].hold_time*1000));
	// 			temp_pln.slfo_bgn_val    = pln_lfo[i].b_aver_pqv1[0];
	// 			temp_pln.slfo_end_val    = pln_lfo[i].e_aver_pqv1[0];
	// 			temp_pln.lfo_max_val     = pln_lfo[i].max_value.value;
	// 			temp_pln.lfo_max_val_t   = pln_lfo[i].max_value.time;
	// 			temp_pln.lfo_min_val	 = pln_lfo[i].min_value.value;
	// 			temp_pln.lfo_min_val_t   = pln_lfo[i].min_value.time;
	// 			temp_pln.lfo_max_min_dif = fabs(temp_pln.lfo_max_val - temp_pln.lfo_min_val);
	// 			temp_pln.aver_freq1      = pln_lfo[i].mode1[0].freq;
	// 			temp_pln.aver_freq2      = pln_lfo[i].max_min_dif *2;  // 平均频率改为单摆振幅（峰峰值）
	// 		}
	// 		
	// 		if (acln_idx>0)
	// 		{
	// 			PODB_TableModifyByKey((const char *)&acln_idx, LFO_LN_LFO_FLD, (const char *)&temp_pln.if_lfo, sizeof(LFO_LN_LFO),LFO_ACLN_NO_TAB);
	// 		}
	// 	}
	// #endif
	return;
}

//  变压器lfo结果
void CLfoAnaObj::UpdateLfoTrTable(int num, SSO_TR_INF* ptr_inf, LFO_OBJ_RES_SAVE* ptr_lfo)
{
	// #ifdef _WAMAP_
	//     if ( num < 0 || !ptr_inf || !ptr_lfo ) return;
	//     
	//     int i, j;
	//     //int ret_code = -1;
	//     INT_REC tr_idx =0;
	//     LFO_TR_LFO  temp_ptr;
	// 	for(i=0; i<num; i++)
	// 	{
	// 		memset((void *)&temp_ptr,0,sizeof(LFO_TR_LFO));
	// 		tr_idx = 0;
	// 		for( j=0; j<num; j++)
	// 		{
	// 			if( ptr_lfo[i].obj_id == ptr_inf[j].tr_id )
	// 			{
	// 				tr_idx = ptr_inf[j].tr_idx;
	// 				break;
	// 			}
	// 		}
	// 		//if (ptr_lfo[i].lfo_b_t>0)
	// 		{
	// 			temp_ptr.if_lfo		     = ptr_lfo[i].if_lfo;
	// 			temp_ptr.lfo_bgn_val     = ptr_lfo[i].b_aver_pqv0[0];
	// 			temp_ptr.lfo_end_val     = ptr_lfo[i].e_aver_pqv0[0];
	// 			temp_ptr.slfo_bgn_t      = ptr_lfo[i].lfo_b_t;
	// 			temp_ptr.slfo_end_t      = ptr_lfo[i].lfo_e_t;
	// 			strcpy(temp_ptr.slfo_time, PT_GetStringOfTimeInterval(ptr_lfo[i].hold_time*1000));
	// 			temp_ptr.slfo_bgn_val    = ptr_lfo[i].b_aver_pqv1[0];
	// 			temp_ptr.slfo_end_val    = ptr_lfo[i].e_aver_pqv1[0];
	// 			temp_ptr.lfo_max_val     = ptr_lfo[i].max_value.value;
	// 			temp_ptr.lfo_max_val_t   = ptr_lfo[i].max_value.time;
	// 			temp_ptr.lfo_min_val	 = ptr_lfo[i].min_value.value;
	// 			temp_ptr.lfo_min_val_t   = ptr_lfo[i].min_value.time;
	// 			temp_ptr.lfo_max_min_dif = fabs(temp_ptr.lfo_max_val - temp_ptr.lfo_min_val);
	// 			temp_ptr.aver_freq1      = ptr_lfo[i].mode1[0].freq;
	// 			temp_ptr.aver_freq2      = ptr_lfo[i].max_min_dif *2;  // 平均频率改为单摆振幅（峰峰值）
	// 		}
	// 
	// 		if (tr_idx>0)
	// 		{
	// 			PODB_TableModifyByKey((const char *)&tr_idx, LFO_TR_LFO_FLD, (const char *)&temp_ptr.if_lfo, sizeof(LFO_TR_LFO),LFO_TR_NO_TAB);
	// 		}
	// 	}
	// #endif
	return;
}

//  模式lfo结果
void CLfoAnaObj::UpdateLfoMdTable(int num, LFO_DEV_LEAD_MODE* pobj_mode)
{
	// #ifdef _WAMAP_
	// 	if ( num < 0 || !pobj_mode) return;
	// 
	//     TAB_LFO_DEV_LEAD_MODE *m_pobj_mode=NULL;
	//     m_pobj_mode = new TAB_LFO_DEV_LEAD_MODE[num];
	//     memset((void *)m_pobj_mode, 0, num * sizeof(TAB_LFO_DEV_LEAD_MODE));
	// 
	// 	INT_REC recordid = PODB_GetDevIdByKeyID(LFO_DEV_LEADMODE_NO_TAB,1);
	// 	for(int ii=0;ii<num;ii++)
	// 	{
	// 		m_pobj_mode[ii].record_id=recordid;
	// 		m_pobj_mode[ii].wds_no=pobj_mode[ii].wds_no;
	// 		m_pobj_mode[ii].wds_bgn=pobj_mode[ii].wds_bgn;
	// 		m_pobj_mode[ii].wds_end=pobj_mode[ii].wds_end;
	// 		m_pobj_mode[ii].dev_id=pobj_mode[ii].dev_id;
	// 		m_pobj_mode[ii].dev_type=pobj_mode[ii].dev_type;
	// 		m_pobj_mode[ii].is_show=pobj_mode[ii].is_show;
	// 		m_pobj_mode[ii].ampl=pobj_mode[ii].ampl;
	// 		m_pobj_mode[ii].freq=pobj_mode[ii].freq;
	// 		m_pobj_mode[ii].damp=pobj_mode[ii].damp;
	// 		m_pobj_mode[ii].phas=pobj_mode[ii].phas;
	// 		m_pobj_mode[ii].real=pobj_mode[ii].real;
	// 		m_pobj_mode[ii].imag=pobj_mode[ii].imag;
	// 		m_pobj_mode[ii].engy=pobj_mode[ii].engy;
	// 		m_pobj_mode[ii].fail=pobj_mode[ii].fail;
	// 		recordid++;
	// 	}
	// 
	// 	PODB_TableClear(LFO_DEV_LEADMODE_NO_TAB);
	// 	int ret_code = PODB_TableWrite((const char*)&m_pobj_mode[0].record_id, num*sizeof(TAB_LFO_DEV_LEAD_MODE),LFO_DEV_LEADMODE_NO_TAB);
	// 		
	// 	if(ret_code < 0)
	// 	{
	// 		sprintf(m_LogString, "写实时设备主导模式表[app：%d，table：%d, record: "INT_REC_FMT", num: %d]失败\n",g_appNo,LFO_DEV_LEADMODE_NO_TAB,m_pobj_mode[0].record_id, num);
	// 		MessShow(m_LogString);
	// 	}
	// 	
	// 	SAFE_DELETE_ARRAY(m_pobj_mode);
	// #endif
	return;
}

//  电网lfo结果
void CLfoAnaObj::UpdateLfoSysTable()
{
#ifdef _WAMAP_

	//int ret_code =0;
	int ii = 0;
	memset((void*)&m_SysLfo, 0, sizeof(LFO_SYS_LFO));
	strcpy(m_SysLfo.case_des, m_SysResSave.lfo_case_des);
	m_SysLfo.lfo_bgn = m_SysResSave.lfo_b_t;
	m_SysLfo.lfo_end = m_SysResSave.lfo_e_t;
	strcpy(m_SysLfo.lfo_time, PT_GetStringOfTimeInterval(m_SysResSave.hold_time * 1000));
	m_SysLfo.warn_dev_id = m_SysResSave.obj_id;
#ifdef _ADDFACID_
	int nflag = 0;
	int tableno = PODB_GetTabelNoByDevId(m_SysLfo.warn_dev_id);
	if (tableno == GN_DEVICE_NO_TAB)
	{
		for (ii = 0; ii < m_nGnNum; ii++)
		{
			if (m_SysLfo.warn_dev_id == m_pGnInf[ii].gn_id)
			{
				m_SysLfo.warn_dev_facid = m_pGnInf[ii].fac_id;
				nflag = 1;
				break;
			}
		}
	}
	else if (tableno == ACLN_DEVICE_NO_TAB)
	{
		for (ii = 0; ii < m_nLnNum; ii++)
		{
			if (m_SysLfo.warn_dev_id == m_pLnInf[ii].acln_id)
			{
				// 待完善,选取首端厂站
				m_SysLfo.warn_dev_facid = m_pLnInf[ii].fac_id_1;
				nflag = 1;
				break;
			}
		}
	}
	else if (tableno == TR_DEVICE_NO_TAB)
	{
		for (ii = 0; ii < m_nTrNum; ii++)
		{
			if (m_SysLfo.warn_dev_id == m_pTrInf[ii].tr_id)
			{
				m_SysLfo.warn_dev_facid = m_pTrInf[ii].fac_id;
				nflag = 1;
				break;
			}
		}
	}
	else
	{
		sprintf(m_LogString, "设备类型错误[%d]", tableno);
		MessShow(m_LogString);
	}

	if (nflag == 0)
	{
		sprintf(m_LogString, "未找到触发设备厂站["INT_REC_FMT"]", INT_REC_VAL(m_SysLfo.warn_dev_id));
		MessShow(m_LogString);
	}
#endif
	strcpy(m_SysLfo.warn_dev_name, m_SysResSave.obj_name);
	m_SysLfo.warn_freq = m_SysResSave.freq;
	m_SysLfo.aver_freq = 0;
	m_SysLfo.lfo_ln_num = m_SysResSave.lfo_line_num;
	m_SysLfo.lfo_gn_num = m_SysResSave.act_gen_num;
	m_SysLfo.lfo_tr_num = m_SysResSave.lfo_tr_num;

	if (m_SysResSave.mode1_num > 0)
	{
		m_SysLfo.aver_freq2 = 100.0f;
		for (ii = 0; ii < m_SysResSave.mode1_num; ii++)
		{
			if (m_SysResSave.mode1[ii].damp < m_SysLfo.aver_freq2)
			{
				m_SysLfo.aver_freq2 = m_SysResSave.mode1[ii].damp;
			}
			m_SysLfo.aver_freq += m_SysResSave.mode1[ii].freq;
		}
		m_SysLfo.aver_freq = m_SysLfo.aver_freq / m_SysResSave.mode1_num;
	}

	INT_REC keyrecord = PODB_GetDevIdByKeyID(LFO_PARA_NO_TAB, 1);
	PODB_TableModifyByKey((const char*)&keyrecord, LFO_SYS_LFO_FLD, (const char*)&m_SysLfo.case_des, sizeof(LFO_SYS_LFO), LFO_PARA_NO_TAB);
#endif	
	return;
}
//  更新最近一次低频振荡时间
// void CLfoAnaObj::UpdateLfoHisTime( LFO_HIS_TIME his_time )
// {
// #ifdef _WAMAP_
//     
//     int ret_code =0;
//     int ii =0;
//     
//     if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_PARA_NO, g_ctxNo ) == DB_OK )
//     {
//         int keyrecord = LFO_PARA_NO*DB_TABLE_BASE+1;
//         
//         ret_code = g_IDBTableOp.TableModifyByKey((char *)&keyrecord, TAB_LFO_HIS_TIME, (char *)&his_time, sizeof(LFO_HIS_TIME));
//         if( ret_code <0 )
//         {
//             sprintf(m_LogString, "UpdateLfoSysTable()::修改LFO参数表[app: %d, table: %d]失败\n", LFO_AP_ID, LFO_PARA_NO);
//             WriteLogFile(m_LogString);
//         }
//     }
// #endif	
//     return ;
// }
/*
//  厂站ana结果
void CLfoAnaObj::UpdateAnaFcTable(  int num, LFO_FC_INF *fc_inf, LFO_FC_LFO *fc_stat)
{
#ifdef _WAMAP_
	if ( num < 0 || !fc_inf || !fc_stat ) return;

	int ii;

	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_FAC_NO, g_contextNo ) == DB_OK )
	{
		for ( ii = 0; ii<num; ii++ )
		{
			g_IDBTableOp.TableModifyByKey((char *)&fc_inf[ii].fac_idx, LFO_FC_LFO_FLD, (char *)&fc_stat[ii], sizeof(LFO_FC_LFO));
		}
	}
#endif
	return ;
}

//  机组ana结果
void CLfoAnaObj::UpdateAnaGnTable( int num, LFO_GN_INF *gn_inf, LFO_GN_LFO *gn_stat)
{
#ifdef _WAMAP_
	if ( num < 0 || !gn_inf || !gn_stat ) return;

	int ii;

	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_GN_NO, g_contextNo ) == DB_OK )
	{
		for ( ii = 0; ii<num; ii++ )
		{
			g_IDBTableOp.TableModifyByKey((char *)&gn_inf[ii].gn_idx, LFO_GN_LFO_FLD, (char *)&gn_stat[ii], sizeof(LFO_GN_LFO));
		}
	}
#endif
	return ;
}

//  线路ana结果
void CLfoAnaObj::UpdateAnaLnTable( int num, LFO_LN_INF *ln_inf, LFO_LN_LFO *ln_stat )
{
#ifdef _WAMAP_
	if ( num < 0 || !ln_inf || !ln_stat ) return;

	int ii;

	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_ACLN_NO, g_contextNo ) == DB_OK )
	{
		for ( ii = 0; ii<num; ii++ )
		{
			g_IDBTableOp.TableModifyByKey((char *)&ln_inf[ii].acln_idx, LFO_LN_LFO_FLD, (char *)&ln_stat[ii], sizeof(LFO_LN_LFO));
		}
	}
#endif
	return ;
}
*/
//  电网ana结果
void CLfoAnaObj::UpdateAnaSysTable(LFO_SYS_ANA sys_ana)
{
#ifdef _WAMAP_
	INT_REC keyrecord = PODB_GetDevIdByKeyID(LFO_PARA_NO_TAB, 1);
	PODB_TableModifyByKey((const char*)&keyrecord, LFO_SYS_ANA_FLD, (const char*)&sys_ana.wds_bgn, sizeof(sys_ana), LFO_PARA_NO_TAB);
#endif
	return;
}

//  LFO厂站表（部分厂站）
void CLfoAnaObj::UpdateAnaCntrTable(int num, SSO_FC_INF* pfc_inf, SSO_FC_MON* pcntr_fc)
{/*
#ifdef _WAMAP_
	if ( num < 0 || !pfc_inf || !pcntr_fc ) return;

	int ii;

	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_FAC_NO, g_contextNo ) == DB_OK )
	{
		for ( ii = 0; ii<num; ii++ )
		{
			g_IDBTableOp.TableModifyByKey((char *)&pfc_inf[ii].fac_idx, LFO_FC_LFO_FLD, (char *)&pcntr_fc[ii], sizeof(LFO_FC_LFO));
		}
	}
#endif	*/
	return;
}

//  LFO实时模态表
void CLfoAnaObj::UpdateAnaShapTable(int num, MODE_CHRCT* pModeShap)
{
#ifdef _WAMAP_
	if (num < 0 || !pModeShap) return;
	int i, j, k;
	int sum = 0, nTemp = 0;
	for (i = 0; i < num; i++)
	{
		if (pModeShap[i].nPartGn > 0)   sum += pModeShap[i].nPartGn;
		if (pModeShap[i].nPartLn > 0)   sum += pModeShap[i].nPartLn;
		if (pModeShap[i].nPartTr > 0)   sum += pModeShap[i].nPartTr;
		if (pModeShap[i].nPartFc > 0)   sum += pModeShap[i].nPartFc;
	}

	if (sum <= 0)
	{
		return;
	}
	TAB_LFO_WDS_SHAPE* m_pModeShap = NULL;
	m_pModeShap = (TAB_LFO_WDS_SHAPE*)calloc(sum, sizeof(TAB_LFO_WDS_SHAPE));
	memset((void*)m_pModeShap, 0, sum * sizeof(TAB_LFO_WDS_SHAPE));

	INT_REC recordid = PODB_GetDevIdByKeyID(LFO_WDS_SHAPE_NO_TAB, 1);
	nTemp = 0;
	for (i = 0; i < num; i++)
	{
		if (pModeShap[i].nPartGn > 0)
		{
			for (j = 0; j < pModeShap[i].nPartGn; j++)
			{
				m_pModeShap[nTemp + j].record_id = recordid;
				m_pModeShap[nTemp + j].wdcase_no = 0;
				m_pModeShap[nTemp + j].app_no = 0;
				m_pModeShap[nTemp + j].mode_no = pModeShap[i].mode_id;
				m_pModeShap[nTemp + j].shape_no = j + 1;
				m_pModeShap[nTemp + j].rreall = pModeShap[i].PartGn[j].real;
				m_pModeShap[nTemp + j].rimag = pModeShap[i].PartGn[j].imag;
				m_pModeShap[nTemp + j].rmgnt = pModeShap[i].PartGn[j].mgnt;
				m_pModeShap[nTemp + j].rphas = pModeShap[i].PartGn[j].phas;
				m_pModeShap[nTemp + j].fact = pModeShap[i].PartGn[j].fact;
				m_pModeShap[nTemp + j].fact_sts2 = 0;
				m_pModeShap[nTemp + j].dev_id = pModeShap[i].PartGn[j].obj_id;
				m_pModeShap[nTemp + j].dev_type = MENU_LFO_DEV_GEN;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartGn[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartGn[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartGn[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartGn[j].engy;
				m_pModeShap[nTemp + j].clus = pModeShap[i].PartGn[j].clus;
				for (k = 0; k < m_nGnNum; k++)
				{
					if (pModeShap[i].PartGn[j].obj_id == m_pGnInf[k].gn_id)
					{
						if (para_ShowFacName == 1) {
							sprintf(m_pModeShap[nTemp + j].dev_name, "%s.%s", m_pGnInf[k].fac_name, m_pGnInf[k].gn_name);
						}
						else {
							strcpy(m_pModeShap[nTemp + j].dev_name, m_pGnInf[k].gn_name);
						}
						m_pModeShap[nTemp + j].fac_id = m_pGnInf[k].fac_id;
						m_pModeShap[nTemp + j].area_id = m_pGnInf[k].area_id;

						//                         m_pModeShap[nTemp+j].aver_val = m_pGnMon[k].aver_val;
						//                         m_pModeShap[nTemp+j].max_ampl = m_pGnMon[k].max_ampl*2;  // 显示峰峰值
						//                         m_pModeShap[nTemp+j].max_ampl_rat = m_pGnMon[k].max_ampl_rat;

						break;
					}
				}
				IncrRtdbId(recordid);
			}
		}

		nTemp += pModeShap[i].nPartGn;
		if (pModeShap[i].nPartLn > 0)
		{
			for (j = 0; j < pModeShap[i].nPartLn; j++)
			{
				m_pModeShap[nTemp + j].record_id = recordid;
				m_pModeShap[nTemp + j].wdcase_no = 0;
				m_pModeShap[nTemp + j].app_no = 0;
				m_pModeShap[nTemp + j].mode_no = pModeShap[i].mode_id;
				m_pModeShap[nTemp + j].shape_no = pModeShap[i].nPartGn + j + 1;
				m_pModeShap[nTemp + j].rreall = pModeShap[i].PartLn[j].real;
				m_pModeShap[nTemp + j].rimag = pModeShap[i].PartLn[j].imag;
				m_pModeShap[nTemp + j].rmgnt = pModeShap[i].PartLn[j].mgnt;
				m_pModeShap[nTemp + j].rphas = pModeShap[i].PartLn[j].phas;
				m_pModeShap[nTemp + j].fact = pModeShap[i].PartLn[j].fact;
				m_pModeShap[nTemp + j].fact_sts2 = 0;
				m_pModeShap[nTemp + j].dev_id = pModeShap[i].PartLn[j].obj_id;
				m_pModeShap[nTemp + j].dev_type = MENU_LFO_DEV_LINE;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartLn[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartLn[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartLn[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartLn[j].engy;
				m_pModeShap[nTemp + j].clus = pModeShap[i].PartLn[j].clus;
				for (k = 0; k < m_nLnNum; k++)
				{
					if (pModeShap[i].PartLn[j].obj_id == m_pLnInf[k].acln_id)
					{
						strcpy(m_pModeShap[nTemp + j].dev_name, m_pLnInf[k].acln_name);
						m_pModeShap[nTemp + j].fac_id = m_pLnInf[k].fac_id_1;
						m_pModeShap[nTemp + j].area_id = m_pLnInf[k].area_id_1;

						//                         m_pModeShap[nTemp+j].aver_val = m_pLnMon[k].aver_val;
						//                         m_pModeShap[nTemp+j].max_ampl = m_pLnMon[k].max_ampl*2;
						//                         m_pModeShap[nTemp+j].max_ampl_rat = m_pLnMon[k].max_ampl_rat;
						break;
					}
				}
				IncrRtdbId(recordid);
			}
		}

		nTemp += pModeShap[i].nPartLn;
		if (pModeShap[i].nPartTr > 0)
		{
			for (j = 0; j < pModeShap[i].nPartTr; j++)
			{
				m_pModeShap[nTemp + j].record_id = recordid;
				m_pModeShap[nTemp + j].wdcase_no = 0;
				m_pModeShap[nTemp + j].app_no = 0;
				m_pModeShap[nTemp + j].mode_no = pModeShap[i].mode_id;
				m_pModeShap[nTemp + j].shape_no = pModeShap[i].nPartLn + pModeShap[i].nPartGn + j + 1;
				m_pModeShap[nTemp + j].rreall = pModeShap[i].PartTr[j].real;
				m_pModeShap[nTemp + j].rimag = pModeShap[i].PartTr[j].imag;
				m_pModeShap[nTemp + j].rmgnt = pModeShap[i].PartTr[j].mgnt;
				m_pModeShap[nTemp + j].rphas = pModeShap[i].PartTr[j].phas;
				m_pModeShap[nTemp + j].fact = pModeShap[i].PartTr[j].fact;
				m_pModeShap[nTemp + j].fact_sts2 = 0;
				m_pModeShap[nTemp + j].dev_id = pModeShap[i].PartTr[j].obj_id;
				m_pModeShap[nTemp + j].dev_type = MENU_LFO_DEV_TR;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartTr[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartTr[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartTr[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartTr[j].engy;
				m_pModeShap[nTemp + j].clus = pModeShap[i].PartTr[j].clus;

				for (k = 0; k < m_nTrNum; k++)
				{
					if (pModeShap[i].PartTr[j].obj_id == m_pTrInf[k].tr_id)
					{
						if(para_ShowFacName == 1) {
							sprintf(m_pModeShap[nTemp + j].dev_name, "%s.%s", m_pTrInf[k].fac_name, m_pTrInf[k].tr_name);
						}
						else {
							strcpy(m_pModeShap[nTemp + j].dev_name, m_pTrInf[k].tr_name);
						}
						m_pModeShap[nTemp + j].fac_id = m_pTrInf[k].tr_id;
						m_pModeShap[nTemp + j].area_id = m_pTrInf[k].area_id;

						//                         m_pModeShap[nTemp+j].aver_val = m_pTrMon[k].aver_val;
						//                         m_pModeShap[nTemp+j].max_ampl = m_pTrMon[k].max_ampl*2;
						//                         m_pModeShap[nTemp+j].max_ampl_rat = m_pTrMon[k].max_ampl_rat;
						break;
					}
				}
				IncrRtdbId(recordid);
			}
		}

		nTemp += pModeShap[i].nPartTr;
		if (pModeShap[i].nPartFc > 0)
		{
			for (j = 0; j < pModeShap[i].nPartFc; j++)
			{
				m_pModeShap[nTemp + j].record_id = recordid;
				m_pModeShap[nTemp + j].wdcase_no = 0;
				m_pModeShap[nTemp + j].app_no = 0;
				m_pModeShap[nTemp + j].mode_no = pModeShap[i].mode_id;
				m_pModeShap[nTemp + j].shape_no = pModeShap[i].nPartGn + pModeShap[i].nPartLn + pModeShap[i].nPartTr + j + 1;
				m_pModeShap[nTemp + j].rreall = pModeShap[i].PartFc[j].real;
				m_pModeShap[nTemp + j].rimag = pModeShap[i].PartFc[j].imag;
				m_pModeShap[nTemp + j].rmgnt = pModeShap[i].PartFc[j].mgnt;
				m_pModeShap[nTemp + j].rphas = pModeShap[i].PartFc[j].phas;
				m_pModeShap[nTemp + j].fact = pModeShap[i].PartFc[j].fact;
				m_pModeShap[nTemp + j].fact_sts2 = 0;
				m_pModeShap[nTemp + j].dev_id = pModeShap[i].PartFc[j].obj_id;
				m_pModeShap[nTemp + j].dev_type = MENU_LFO_DEV_FAC;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartFc[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartFc[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartFc[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartFc[j].engy;
				m_pModeShap[nTemp + j].clus = pModeShap[i].PartFc[j].clus;
				for (k = 0; k < m_nFcNum; k++)
				{
					if (pModeShap[i].PartFc[j].obj_id == m_pFcInf[k].fac_id)
					{
						strcpy(m_pModeShap[nTemp + j].dev_name, m_pFcInf[k].fac_name);
						m_pModeShap[nTemp + j].fac_id = m_pFcInf[k].fac_id;
						m_pModeShap[nTemp + j].area_id = m_pFcInf[k].area_id;

						//                         m_pModeShap[nTemp+j].aver_val = m_pFcMon[k].v_aver;
						//                         m_pModeShap[nTemp+j].max_ampl = m_pFcMon[k].v_min;
						//                         m_pModeShap[nTemp+j].max_ampl_rat = (m_pFcMon[k].v_max-m_pFcMon[k].v_min)/m_pFcMon[k].v_aver;
						break;
					}
				}
				IncrRtdbId(recordid);
			}
		}
		nTemp += pModeShap[i].nPartFc;
	}
	if (nTemp <= 0)
	{
		SAFE_DELETE_ARRAY(m_pModeShap);
		return;
	}

	PODB_TableClear(LFO_WDS_SHAPE_NO_TAB);
	PODB_TableWrite((const char*)&m_pModeShap[0].record_id, sum * sizeof(TAB_LFO_WDS_SHAPE), LFO_WDS_SHAPE_NO_TAB);

	SAFE_DELETE_ARRAY(m_pModeShap);
#endif
	return;
}

// 更新LFO发电机表参与因子
void CLfoAnaObj::UpdateGnFact(int num, MODE_CHRCT* pModeShap)
{
#ifdef _WAMAP_
	int i, j, k;
	for (i = 0; i < num; i++)
	{
		if (pModeShap[i].nPartGn > 0)
		{
			for (j = 0; j < pModeShap[i].nPartGn; j++)
			{
				for (k = 0; k < m_nGnNum; k++)
				{
					if (pModeShap[i].PartGn[j].obj_id == m_pGnFact[k].obj_id)
					{
						m_pGnFact[k].fact = pModeShap[i].PartGn[j].fact;
						m_pGnFact[k].fact_sts = 0;
						m_pGnFact[k].clus = pModeShap[i].PartGn[j].clus;
						break;
					}
				}
			}
		}
	}

	for (i = 0; i < m_nGnNum; i++)
	{
		PODB_TableModifyByKey((const char*)&m_pGnFact[i].obj_idx, "fact", (const char*)&m_pGnFact[i].fact, sizeof(float), LFO_GN_NO_TAB);
		PODB_TableModifyByKey((const char*)&m_pGnFact[i].obj_idx, "fact_sts", (const char*)&m_pGnFact[i].fact_sts, sizeof(int), LFO_GN_NO_TAB);
		PODB_TableModifyByKey((const char*)&m_pGnFact[i].obj_idx, "clus", (const char*)&m_pGnFact[i].clus, sizeof(unsigned char), LFO_GN_NO_TAB);
	}
#endif
	return;
}
// 更新LFO交流线路表参与因子  2008-05-21 added by zhw at nj
void CLfoAnaObj::UpdateLnFact(int num, MODE_CHRCT* pModeShap)
{
#ifdef _WAMAP_
	int i, j, k;
	for (i = 0; i < num; i++)
	{
		if (pModeShap[i].nPartLn > 0)
		{
			for (j = 0; j < pModeShap[i].nPartLn; j++)
			{
				for (k = 0; k < m_nLnNum; k++)
				{
					if (pModeShap[i].PartLn[j].obj_id == m_pLnFact[k].obj_id)
					{
						m_pLnFact[k].fact = pModeShap[i].PartLn[j].fact;
						m_pLnFact[k].fact_sts = 0;
						m_pLnFact[k].clus = pModeShap[i].PartLn[j].clus;
						break;
					}
				}
			}
		}
	}

	for (i = 0; i < m_nLnNum; i++)
	{
		PODB_TableModifyByKey((const char*)&m_pLnFact[i].obj_idx, "fact", (const char*)&m_pLnFact[i].fact, sizeof(float), LFO_ACLN_NO_TAB);
	}

#endif
	return;
}

//  参与因子平均值入库
void CLfoAnaObj::UpdateAverFact(int num, MODE_CHRCT* pModeShap)
{
#ifdef _WAMAP_
	if (num < 0 || !pModeShap) return;

	int i, j, k;

	for (i = 0; i < num; i++)
	{
		if (pModeShap[i].nPartGn > 0)
		{
			for (j = 0; j < pModeShap[i].nPartGn; j++)
			{
				for (k = 0; k < m_nGnNum; k++)
				{
					if (pModeShap[i].PartGn[j].obj_id == m_pGnFact[k].obj_id)
					{
						m_pGnFact[k].aver_fact += pModeShap[i].PartGn[j].fact;
						m_pGnFact[k].num_fact += 1;
						break;
					}

				}

			}
		}

	}

	for (k = 0; k < m_nGnNum; k++)
	{
		if (m_pGnFact[k].num_fact > 0)
		{
			m_pGnFact[k].aver_fact = m_pGnFact[k].aver_fact / m_pGnFact[k].num_fact;
		}
	}

	for (i = 0; i < m_nGnNum; i++)
	{
		PODB_TableModifyByKey((const char*)&m_pGnFact[i].obj_idx, TYPE_OBJ_AVER_FACT, (const char*)&m_pGnFact[i].aver_fact, sizeof(float), LFO_GN_NO_TAB);
	}

#endif
	return;
}

//  LFO实时case窗口表
void CLfoAnaObj::UpdateCasWndsTable(int num, LFO_CALC_WDS CalcWds, int is_clear)
{
	if (m_RunCtrCom == MENU_LFO_PRO_CTL_SAVE)
	{
		sprintf(m_LogString, "UpdateCasWndsTable( ):: 案例保存不更新，返回！");
		MessShow(m_LogString);
		return;
	}
#ifdef _WAMAP_       //num不为1时，程序需修改

	int ret_code, get_num;

	if (is_clear == 1)  // 清除记录
	{
		PODB_TableClear(LFO_CASE_WDS_NO_TAB);
	}

	get_num = PODB_GetTabRecNum(LFO_CASE_WDS_NO_TAB);
	CalcWds.wds_id = PODB_GetDevIdByKeyID(LFO_CASE_WDS_NO_TAB, get_num + 1);
	ret_code = PODB_TableWrite((const char*)&CalcWds.wds_id, sizeof(LFO_CALC_WDS), LFO_CASE_WDS_NO_TAB);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "写LFO实时case窗口表[app：%d，table：%d]失败\n", g_appNo, LFO_CASE_WDS_NO_TAB);
		MessShow(m_LogString);
		return;
	}
#endif
	return;
}

// LFO实时case模态全表
void CLfoAnaObj::UpdateCasAllShapeTable(int num, MODE_CHRCT* pModeShap)
{
	if (m_RunCtrCom == MENU_LFO_PRO_CTL_SAVE)
	{
		sprintf(m_LogString, "UpdateCasAllShapeTable( ):: 案例保存不更新，返回！");
		MessShow(m_LogString);
		return;
	}
#ifdef _WAMAP_
	if (num < 0 || !pModeShap) return;
	int ret_code, wdsno;
	INT_REC recordid;
	int i, j, k;
	int sum = 0, nTemp = 0;
	for (i = 0; i < num; i++)
	{
		if (pModeShap[i].nPartGn > 0)   sum += pModeShap[i].nPartGn;
		if (pModeShap[i].nPartLn > 0)   sum += pModeShap[i].nPartLn;
		if (pModeShap[i].nPartTr > 0)   sum += pModeShap[i].nPartTr;
		if (pModeShap[i].nPartFc > 0)   sum += pModeShap[i].nPartFc;
	}

	TAB_LFO_CASE_ALLSHAPE* m_pModeShap = NULL;
	m_pModeShap = (TAB_LFO_CASE_ALLSHAPE*)calloc(sum, sizeof(TAB_LFO_CASE_ALLSHAPE));
	memset((void*)m_pModeShap, 0, sum * sizeof(TAB_LFO_CASE_ALLSHAPE));

	recordid = PODB_GetDevIdByKeyID(LFO_CASE_ALLSHAPE_NO_TAB, 1);
	wdsno = 1;
	nTemp = 0;
	for (i = 0; i < num; i++)
	{
		if (pModeShap[i].nPartGn > 0)
		{
			for (j = 0; j < pModeShap[i].nPartGn; j++)
			{
				m_pModeShap[nTemp + j].record_id = recordid;
				m_pModeShap[nTemp + j].wds_no = wdsno + i;
				m_pModeShap[nTemp + j].app_no = g_appNo;
				m_pModeShap[nTemp + j].mode_no = pModeShap[i].mode_id;
				m_pModeShap[nTemp + j].shape_no = j + 1;
				m_pModeShap[nTemp + j].rreall = pModeShap[i].PartGn[j].real;
				m_pModeShap[nTemp + j].rimag = pModeShap[i].PartGn[j].imag;
				m_pModeShap[nTemp + j].rmgnt = pModeShap[i].PartGn[j].mgnt;
				m_pModeShap[nTemp + j].rphas = pModeShap[i].PartGn[j].phas;
				m_pModeShap[nTemp + j].fact = pModeShap[i].PartGn[j].fact;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartGn[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartGn[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartGn[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartGn[j].engy;
				m_pModeShap[nTemp + j].fact_sts2 = 0;
				m_pModeShap[nTemp + j].dev_id = pModeShap[i].PartGn[j].obj_id;
				m_pModeShap[nTemp + j].dev_type = MENU_LFO_DEV_GEN;
				m_pModeShap[nTemp + j].is_prim = MENU_LFO_MENU_NO;
				m_pModeShap[nTemp + j].is_curt = MENU_LFO_MENU_NO;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartGn[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartGn[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartGn[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartGn[j].engy;
				m_pModeShap[nTemp + j].clus = pModeShap[i].PartGn[j].clus;
				for (k = 0; k < m_nGnNum; k++)
				{
					if (pModeShap[i].PartGn[j].obj_id == m_pGnInf[k].gn_id)
					{
						if (para_ShowFacName == 1) {
							sprintf(m_pModeShap[nTemp + j].dev_name, "%s.%s", m_pGnInf[k].fac_name, m_pGnInf[k].gn_name);
						}
						else {
							strcpy(m_pModeShap[nTemp + j].dev_name, m_pGnInf[k].gn_name);
						}
						m_pModeShap[nTemp + j].fac_id = m_pGnInf[k].fac_id;
						m_pModeShap[nTemp + j].area_id = m_pGnInf[k].area_id;
						//                         m_pModeShap[nTemp+j].aver_val = m_pGnMon[k].aver_val;
						//                         m_pModeShap[nTemp+j].max_ampl = m_pGnMon[k].max_ampl*2;
						//                         m_pModeShap[nTemp+j].max_ampl_rat = m_pGnMon[k].max_ampl_rat;
						break;
					}
				}
				IncrRtdbId(recordid);
			}
		}

		nTemp += pModeShap[i].nPartGn;
		if (pModeShap[i].nPartLn > 0)
		{
			for (j = 0; j < pModeShap[i].nPartLn; j++)
			{
				m_pModeShap[nTemp + j].record_id = recordid;
				m_pModeShap[nTemp + j].wds_no = wdsno + i;
				m_pModeShap[nTemp + j].app_no = g_appNo;
				m_pModeShap[nTemp + j].mode_no = pModeShap[i].mode_id;
				m_pModeShap[nTemp + j].shape_no = pModeShap[i].nPartGn + j + 1;
				m_pModeShap[nTemp + j].rreall = pModeShap[i].PartLn[j].real;
				m_pModeShap[nTemp + j].rimag = pModeShap[i].PartLn[j].imag;
				m_pModeShap[nTemp + j].rmgnt = pModeShap[i].PartLn[j].mgnt;
				m_pModeShap[nTemp + j].rphas = pModeShap[i].PartLn[j].phas;
				m_pModeShap[nTemp + j].fact = pModeShap[i].PartLn[j].fact;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartLn[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartLn[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartLn[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartLn[j].engy;
				m_pModeShap[nTemp + j].fact_sts2 = 0;
				m_pModeShap[nTemp + j].dev_id = pModeShap[i].PartLn[j].obj_id;
				m_pModeShap[nTemp + j].dev_type = MENU_LFO_DEV_LINE;
				m_pModeShap[nTemp + j].is_prim = MENU_LFO_MENU_NO;
				m_pModeShap[nTemp + j].is_curt = MENU_LFO_MENU_NO;
				m_pModeShap[nTemp + j].clus = MENU_LFO_CLUST_RMAIN;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartLn[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartLn[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartLn[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartLn[j].engy;
				m_pModeShap[nTemp + j].clus = pModeShap[i].PartLn[j].clus;
				for (k = 0; k < m_nLnNum; k++)
				{
					if (pModeShap[i].PartLn[j].obj_id == m_pLnInf[k].acln_id)
					{
						strcpy(m_pModeShap[nTemp + j].dev_name, m_pLnInf[k].acln_name);
						m_pModeShap[nTemp + j].fac_id = m_pLnInf[k].fac_id_1;
						m_pModeShap[nTemp + j].area_id = m_pLnInf[k].area_id_1;
						//                         m_pModeShap[nTemp+j].aver_val = m_pLnMon[k].aver_val;
						//                         m_pModeShap[nTemp+j].max_ampl = m_pLnMon[k].max_ampl*2;
						//                         m_pModeShap[nTemp+j].max_ampl_rat = m_pLnMon[k].max_ampl_rat;
						break;
					}
				}
				IncrRtdbId(recordid);
			}
		}

		nTemp += pModeShap[i].nPartLn;
		if (pModeShap[i].nPartTr > 0)
		{
			for (j = 0; j < pModeShap[i].nPartTr; j++)
			{
				m_pModeShap[nTemp + j].record_id = recordid;
				m_pModeShap[nTemp + j].wds_no = wdsno + i;
				m_pModeShap[nTemp + j].app_no = g_appNo;
				m_pModeShap[nTemp + j].mode_no = pModeShap[i].mode_id;
				m_pModeShap[nTemp + j].shape_no = pModeShap[i].nPartLn + pModeShap[i].nPartGn + j + 1;
				m_pModeShap[nTemp + j].rreall = pModeShap[i].PartTr[j].real;
				m_pModeShap[nTemp + j].rimag = pModeShap[i].PartTr[j].imag;
				m_pModeShap[nTemp + j].rmgnt = pModeShap[i].PartTr[j].mgnt;
				m_pModeShap[nTemp + j].rphas = pModeShap[i].PartTr[j].phas;
				m_pModeShap[nTemp + j].fact = pModeShap[i].PartTr[j].fact;
				m_pModeShap[nTemp + j].fact_sts2 = 0;
				m_pModeShap[nTemp + j].dev_id = pModeShap[i].PartTr[j].obj_id;
				m_pModeShap[nTemp + j].dev_type = MENU_LFO_DEV_TR;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartTr[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartTr[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartTr[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartTr[j].engy;
				m_pModeShap[nTemp + j].clus = pModeShap[i].PartTr[j].clus;

				for (k = 0; k < m_nTrNum; k++)
				{
					if (pModeShap[i].PartTr[j].obj_id == m_pTrInf[k].tr_id)
					{
						if(para_ShowFacName == 1) {
							sprintf(m_pModeShap[nTemp + j].dev_name, "%s.%s", m_pTrInf[k].fac_name, m_pTrInf[k].tr_name);
						}
						else {
							strcpy(m_pModeShap[nTemp + j].dev_name, m_pTrInf[k].tr_name);
						}
						m_pModeShap[nTemp + j].fac_id = m_pTrInf[k].tr_id;
						m_pModeShap[nTemp + j].area_id = m_pTrInf[k].area_id;
						//                         m_pModeShap[nTemp+j].aver_val = m_pTrMon[k].aver_val;
						//                         m_pModeShap[nTemp+j].max_ampl = m_pTrMon[k].max_ampl*2;
						//                         m_pModeShap[nTemp+j].max_ampl_rat = m_pTrMon[k].max_ampl_rat;
						break;
					}
				}
				IncrRtdbId(recordid);
			}
		}

		nTemp += pModeShap[i].nPartTr;
		if (pModeShap[i].nPartFc > 0)
		{
			for (j = 0; j < pModeShap[i].nPartFc; j++)
			{
				m_pModeShap[nTemp + j].record_id = recordid;
				m_pModeShap[nTemp + j].wds_no = wdsno + i;
				m_pModeShap[nTemp + j].app_no = g_appNo;
				m_pModeShap[nTemp + j].mode_no = pModeShap[i].mode_id;
				m_pModeShap[nTemp + j].shape_no = pModeShap[i].nPartGn + pModeShap[i].nPartLn + pModeShap[i].nPartTr + j + 1;
				m_pModeShap[nTemp + j].rreall = pModeShap[i].PartFc[j].real;
				m_pModeShap[nTemp + j].rimag = pModeShap[i].PartFc[j].imag;
				m_pModeShap[nTemp + j].rmgnt = pModeShap[i].PartFc[j].mgnt;
				m_pModeShap[nTemp + j].rphas = pModeShap[i].PartFc[j].phas;
				m_pModeShap[nTemp + j].fact = pModeShap[i].PartFc[j].fact;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartFc[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartFc[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartFc[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartFc[j].engy;
				m_pModeShap[nTemp + j].fact_sts2 = 0;
				m_pModeShap[nTemp + j].dev_id = pModeShap[i].PartFc[j].obj_id;
				m_pModeShap[nTemp + j].dev_type = MENU_LFO_DEV_FAC;
				m_pModeShap[nTemp + j].is_prim = MENU_LFO_MENU_NO;
				m_pModeShap[nTemp + j].is_curt = MENU_LFO_MENU_NO;
				m_pModeShap[nTemp + j].freq = pModeShap[i].PartFc[j].freq;
				m_pModeShap[nTemp + j].damp = pModeShap[i].PartFc[j].damp;
				m_pModeShap[nTemp + j].ampl = pModeShap[i].PartFc[j].ampl;
				m_pModeShap[nTemp + j].engy = pModeShap[i].PartFc[j].engy;
				m_pModeShap[nTemp + j].clus = pModeShap[i].PartFc[j].clus;

				for (k = 0; k < m_nFcNum; k++)
				{
					if (pModeShap[i].PartFc[j].obj_id == m_pFcInf[k].fac_id)
					{
						strcpy(m_pModeShap[nTemp + j].dev_name, m_pFcInf[k].fac_name);
						m_pModeShap[nTemp + j].fac_id = m_pFcInf[k].fac_id;
						m_pModeShap[nTemp + j].area_id = m_pFcInf[k].area_id;
						//                         m_pModeShap[nTemp+j].aver_val = m_pFcMon[k].v_aver;
						//                         m_pModeShap[nTemp+j].max_ampl = fabs(m_pFcMon[k].v_max-m_pFcMon[k].v_min);
						//                         if( m_pFcMon[k].v_aver)
						//                         m_pModeShap[nTemp+j].max_ampl_rat = (m_pFcMon[k].v_max-m_pFcMon[k].v_min)/m_pFcMon[k].v_aver;
						break;
					}
				}
				IncrRtdbId(recordid);
			}
		}
		nTemp += pModeShap[i].nPartFc;
	}

	PODB_TableClear(LFO_CASE_ALLSHAPE_NO_TAB);

	ret_code = PODB_TableWrite((const char*)&m_pModeShap[0].record_id, nTemp * sizeof(TAB_LFO_CASE_ALLSHAPE), LFO_CASE_ALLSHAPE_NO_TAB);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "写LFO实时case模态全表[app：%d，table：%d]失败\n", g_appNo, LFO_CASE_ALLSHAPE_NO_TAB);
		MessShow(m_LogString);
	}

	if (m_pModeShap) { free(m_pModeShap);  m_pModeShap = NULL; }
#endif
	return;
}

//   写LFO曲线
void CLfoAnaObj::UpdateLfoCurve(TAB_LFO_RT_CURVE RtCurve)
{
#ifdef _WAMAP_
#ifndef _NUSP
	int i = 0;

	TAB_LFO_RT_CURVE1    temp_RtCurve1;
	TAB_LFO_RT_CURVE2    temp_RtCurve2;

	memset((void*)&temp_RtCurve1, 0, sizeof(TAB_LFO_RT_CURVE1));
	memset((void*)&temp_RtCurve2, 0, sizeof(TAB_LFO_RT_CURVE2));

	INT_REC keyrecord = RtCurve.dev_idx;
	// 
	// 	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_RT_CURVE_NO, g_ctxNo ) != DB_OK )  
	// 	{
	// 		sprintf(m_LogString, "when open app: %d  table: %d,  error",LFO_AP_ID,LFO_RT_CURVE_NO);
	// 		WriteLogFile(m_LogString);
	// 		return;
	// 	}

		//temp_RtCurve1.dev_id      = RtCurve.dev_id;
	sprintf(temp_RtCurve1.dev_name, "%s", RtCurve.dev_name);
	sprintf(temp_RtCurve1.unit, "%s", RtCurve.unit);

	temp_RtCurve1.sam_freq = RtCurve.sam_freq;
	temp_RtCurve1.cuv_bgn_sec = RtCurve.cuv_bgn_sec;
	temp_RtCurve1.cuv_end_sec = RtCurve.cuv_end_sec;
	temp_RtCurve1.cuv_sec = RtCurve.cuv_sec;
	temp_RtCurve1.cuv_pnt = RtCurve.cuv_pnt;
	temp_RtCurve1.cuv_stp = RtCurve.cuv_stp;
	// 	temp_RtCurve1.max_val     = RtCurve.max_val;
	// 	temp_RtCurve1.min_val     = RtCurve.min_val;
	// 	temp_RtCurve1.ave_val     = RtCurve.ave_val;

	for (i = 0; i < LFO_CURVE_PNT; i++)
	{
		temp_RtCurve1.seq_cuv_data[i] = RtCurve.seq_cuv_data[i];
	}
	for (i = 0; i < LFO_CURVE_PNT; i++)
	{
		temp_RtCurve2.seq_cuv_data[i] = RtCurve.seq_cuv_data[i + LFO_CURVE_PNT];
	}
	PODB_TableModifyByKey((const char*)&keyrecord, TAB_LFO_RT_CURVE1_FLD, (char*)&temp_RtCurve1.dev_name, sizeof(TAB_LFO_RT_CURVE1), SSO_RT_CURVE_NO_TAB);
	PODB_TableModifyByKey((const char*)&keyrecord, TAB_LFO_RT_CURVE2_FLD, (char*)&temp_RtCurve2.seq_cuv_data[0], sizeof(TAB_LFO_RT_CURVE2), SSO_RT_CURVE_NO_TAB);
#endif // !_NUSP
#endif // _WAMAP_
	return;
}

//  LFO实时曲线表（记录4、5、6）
void CLfoAnaObj::UpdateMonModeCurce(PERIOD_LEAD_MODE* pLfoModeCurve)
{
#ifdef _WAMAP_

	TAB_LFO_RT_CURVE  LfoModeCurve;
	SEQ_BASE_INFO	  dataBaseInfo;
	int i, j;

	for (i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			LfoModeCurve.dev_idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 4);
			ZERO_INT_REC(LfoModeCurve.dev_id);
			sprintf(LfoModeCurve.dev_name, "%s", "模式曲线时间");
			sprintf(LfoModeCurve.unit, "%s", "秒");
			LfoModeCurve.sam_freq = 0;
			LfoModeCurve.cuv_bgn_sec = pLfoModeCurve[0].bgn_t;
			LfoModeCurve.cuv_end_sec = pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t;
			LfoModeCurve.cuv_sec = LfoModeCurve.cuv_end_sec - LfoModeCurve.cuv_bgn_sec;
			LfoModeCurve.cuv_pnt = LFO_CURVE_MAX_PNT;
			LfoModeCurve.cuv_stp = (float)((LfoModeCurve.cuv_sec * 1.0) / LfoModeCurve.cuv_pnt);
			LfoModeCurve.max_val = 0.0;
			LfoModeCurve.min_val = 0.0;
			LfoModeCurve.ave_val = 0.0;

			for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
			{
				LfoModeCurve.seq_cuv_data[j] = (pLfoModeCurve[j].bgn_t - pLfoModeCurve[0].bgn_t) * 1.0;
#ifdef _NUSP
				m_SsoCurve2[j].time_sec = LfoModeCurve.seq_cuv_data[j];
#endif // _NUSP

			}
			UpdateLfoCurve(LfoModeCurve);
			sprintf(m_LogString, "更新模式曲线时间完成");
			MessShow(m_LogString);
		}

		if (i == 1)
		{
			LfoModeCurve.dev_idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 5);
			ZERO_INT_REC(LfoModeCurve.dev_id);
			sprintf(LfoModeCurve.dev_name, "%s", "主导模式频率曲线");
			sprintf(LfoModeCurve.unit, "%s", "Hz");
			LfoModeCurve.sam_freq = 0;
			LfoModeCurve.cuv_bgn_sec = pLfoModeCurve[0].bgn_t;
			LfoModeCurve.cuv_end_sec = pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t;
			LfoModeCurve.cuv_sec = LfoModeCurve.cuv_end_sec - LfoModeCurve.cuv_bgn_sec;
			LfoModeCurve.cuv_pnt = LFO_CURVE_MAX_PNT;
			LfoModeCurve.cuv_stp = (float)((LfoModeCurve.cuv_sec * 1.0) / LfoModeCurve.cuv_pnt);

			for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
			{
				LfoModeCurve.seq_cuv_data[j] = pLfoModeCurve[j].freq;
#ifdef _NUSP
				m_SsoCurve2[j].lead_freq = pLfoModeCurve[j].freq;
#endif // _NUSP
			}

			DP_CalcDataSeqBaseInfo((float*)&LfoModeCurve.seq_cuv_data[0], LFO_CURVE_MAX_PNT, dataBaseInfo);
			LfoModeCurve.max_val = dataBaseInfo.max_val;
			LfoModeCurve.min_val = dataBaseInfo.min_val;
			LfoModeCurve.ave_val = dataBaseInfo.ave_val;

			UpdateLfoCurve(LfoModeCurve);
			sprintf(m_LogString, "更新主导模式频率曲线完成");
			MessShow(m_LogString);
		}

		if (i == 2)
		{
			LfoModeCurve.dev_idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 6);
			ZERO_INT_REC(LfoModeCurve.dev_id);
			sprintf(LfoModeCurve.dev_name, "%s", "主导模式振幅曲线");
			sprintf(LfoModeCurve.unit, "%s", "%");
			LfoModeCurve.sam_freq = 0;
			LfoModeCurve.cuv_bgn_sec = pLfoModeCurve[0].bgn_t;
			LfoModeCurve.cuv_end_sec = pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t;
			LfoModeCurve.cuv_sec = LfoModeCurve.cuv_end_sec - LfoModeCurve.cuv_bgn_sec;
			LfoModeCurve.cuv_pnt = LFO_CURVE_MAX_PNT;
			LfoModeCurve.cuv_stp = (float)((LfoModeCurve.cuv_sec * 1.0) / LfoModeCurve.cuv_pnt);

			for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
			{
				LfoModeCurve.seq_cuv_data[j] = pLfoModeCurve[j].ampl;
#ifdef _NUSP
				m_SsoCurve2[j].lead_damp = pLfoModeCurve[j].ampl;
#endif // _NUSP

			}

			DP_CalcDataSeqBaseInfo((float*)&LfoModeCurve.seq_cuv_data[0], LFO_CURVE_MAX_PNT, dataBaseInfo);
			LfoModeCurve.max_val = dataBaseInfo.max_val;
			LfoModeCurve.min_val = dataBaseInfo.min_val;
			LfoModeCurve.ave_val = dataBaseInfo.ave_val;

			UpdateLfoCurve(LfoModeCurve);
			sprintf(m_LogString, "更新主导模式振幅曲线完成");
			MessShow(m_LogString);
		}
	}

#endif
	return;
}


//  LFO断面定义表
// void CLfoAnaObj::UpdateItfcCurce(    int num, LFO_INTERFACE *pItfcCurve )
// {
// #ifdef _WAMAP_
// 	if(num<0 || !pItfcCurve) return;
// 
// 	int ii;
// 
// 	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_INTERFACE_DEF_NO, g_contextNo ) == DB_OK )
//     {
// 		int keyrecord = LFO_INTERFACE_DEF_NO*DB_TABLE_BASE+1;
// 					
// 		for ( ii = 0; ii<num; ii++ )
// 		{
// 			g_IDBTableOp.TableModifyByKey((const char *)&keyrecord, TAB_LFO_INTERFACE_DEF_FLD, (const char *)&pItfcCurve->itfc_name, sizeof(TAB_LFO_INTERFACE_DEF));
// 			keyrecord++;
// 		}
//     }
// #endif	
//   return;
// }

///////////////////////////////  消息接口  ///////////////////////////////
//  查询是否应用主机
// int CLfoAnaObj::IsOnDuty( int app_id, short ctx_no )
// {
// #ifdef _WAMAP_
// 	return ( g_pServMan->IsOnDuty( app_id, ctx_no ) );
// #endif
// 	return 0;
// }

//  查询应用主机名
// int CLfoAnaObj::GetAppHostName( int app_id, char * host_name, short ctx_no )
// {
// #ifdef _WAMAP_
// 	int retval;
// 	retval = g_pServMan->RequestService( app_id, 1, host_name, ctx_no );
// 	if (retval != 1)
// 	{
// 		sprintf( m_LogString, "GetAppHostName(): get HostName by app_id[%d] ctx_no[%d] error!", app_id, ctx_no ); 
// 		WriteLogFile( m_LogString );
// 
// 		return -1;
// 	}
// 
// 	sprintf( m_LogString, "GetAppHostName(): HostName by app_id[%d] ctx_no[%d] is %s !", app_id, ctx_no, host_name ); 
// 	WriteLogFile( m_LogString );
// #endif
// 	return 0;
// }
void CLfoAnaObj::UpdateLfoWarnDev()
{
	// #ifdef _WAMAP_
	// 	
	// 	TAB_WARN_DEV_CURVE  LfoWarnDevCurve;
	// 	int i, j, k, kkk, cts_flag, jj,mmm;
	// 	int no=0,cts_num=0,cts_nSamples=0;
	//	PODB_TableClear(LFO_DEV_CURVE_TAB);
	// 	for (i=0;i<LFO_CURVE_MAX_PNT;i++)
	// 	{
	// 		INT_REC m_recordid=PODB_GetDevIdByKeyID(LFO_DEV_CURVE_TAB,i+1);
	// 		int ret_code = PODB_TableWrite((const char*)&m_recordid,1*sizeof(TAB_WARN_DEV_CURVE),LFO_DEV_CURVE_TAB);
	// 		if(ret_code < 0)
	// 		{
	// 			sprintf(m_LogString, "创建LFO设备振荡曲线表[app：%d，table：%d]失败\n",g_appNo,LFO_DEV_CURVE_TAB);
	// 			MessShow(m_LogString);
	// 		}
	// 	} 
	// 	cts_num = 1;
	// 	LfoWarnDevCurve.dev_idx     = PODB_GetDevIdByKeyID(LFO_DEV_CURVE_TAB,cts_num);
	// 	LfoWarnDevCurve.dev_id      = 0;
	// 	sprintf(LfoWarnDevCurve.dev_name,"%s","曲线时间");
	// 	sprintf(LfoWarnDevCurve.unit,"%s","秒");
	// 	LfoWarnDevCurve.sam_freq    = 0;
	// 	LfoWarnDevCurve.cuv_bgn_sec = m_CalcStartTime;
	// 	LfoWarnDevCurve.cuv_end_sec = m_CurGpsTime;
	// 	LfoWarnDevCurve.cuv_sec     = LfoWarnDevCurve.cuv_end_sec - LfoWarnDevCurve.cuv_bgn_sec;
	// 	LfoWarnDevCurve.cuv_pnt     = LFO_CURVE_MAX_PNT;
	// 	LfoWarnDevCurve.cuv_stp     = (float) ((LfoWarnDevCurve.cuv_sec*1.0)/LfoWarnDevCurve.cuv_pnt);
	// 	LfoWarnDevCurve.max_val     =0.0;
	// 	LfoWarnDevCurve.min_val     =0.0;
	// 	LfoWarnDevCurve.ave_val     =0.0;		
	// 	for(j=0; j<LFO_CURVE_MAX_PNT; j++)
	// 	{
	// 		LfoWarnDevCurve.seq_cuv_data[j] = LfoWarnDevCurve.cuv_stp*j;					
	// 	}		
	// 	UpdateWarnDevCurve( LfoWarnDevCurve );
	// 
	// 	cts_num = 2;
	// 	LfoWarnDevCurve.dev_idx     = PODB_GetDevIdByKeyID(LFO_DEV_CURVE_TAB,cts_num);
	// 	LfoWarnDevCurve.dev_id      = 0;
	// 	sprintf(LfoWarnDevCurve.dev_name,"%s"," ");
	// 	sprintf(LfoWarnDevCurve.unit,"%s","Mw");
	// 	LfoWarnDevCurve.sam_freq    = 0;
	// 	LfoWarnDevCurve.cuv_bgn_sec = m_CalcStartTime;
	// 	LfoWarnDevCurve.cuv_end_sec = m_CurGpsTime;
	// 	LfoWarnDevCurve.cuv_sec     = LfoWarnDevCurve.cuv_end_sec - LfoWarnDevCurve.cuv_bgn_sec;
	// 	LfoWarnDevCurve.cuv_pnt     = LFO_CURVE_MAX_PNT;
	// 	LfoWarnDevCurve.cuv_stp     = (float) ((LfoWarnDevCurve.cuv_sec*1.0)/LfoWarnDevCurve.cuv_pnt);
	// 	for(j=0; j<LFO_CURVE_MAX_PNT; j++)
	// 	{
	// 		LfoWarnDevCurve.seq_cuv_data[j] = 999999.;					
	// 	}
	// 	UpdateWarnDevCurve(  LfoWarnDevCurve );
	// 	sprintf(m_LogString, "LFO监视设备数目:机组=[%d],交流线路=[%d],变压器=[%d]\n",m_nGnNum,m_nLnNum,m_nTrNum);
	// 	MessShow(m_LogString);
	// 	sprintf(m_LogString, "告警设备数目:机组=[%d],交流线路=[%d],变压器=[%d]\n",m_nGnWarn,m_nLnWarn,m_nTrWarn);
	// 	MessShow(m_LogString);
	// 
	// 	sprintf(m_LogString, "开始累加告警机组设备");
	// 	MessShow(m_LogString);
	// 	for ( i=0; i<m_nGnWarn; i++ )
	// 	{
	// 		cts_flag = 0;
	// 		for ( j=0; j<sum_WarnGen; j++ )
	// 		{
	// 			sprintf(m_LogString, "m_nGnWarn=[%d];sum_WarnGn=[%d];ID1=[%ld],ID2=[%ld]",m_nGnWarn,sum_WarnGen,m_DevGnWarn[i].gn_id,sum_DevGnWarn[j].gn_id);
	// 			MessShow(m_LogString);
	// 			if ( m_DevGnWarn[i].gn_id == sum_DevGnWarn[j].gn_id )
	// 			{
	// 				cts_flag = 1;
	// 				kkk = j;
	// 				break;
	// 			}
	// 		}
	// 		for ( jj=0; jj<m_nGnNum; jj++ )
	// 		{
	// 			if ( m_DevGnWarn[i].gn_id == m_pGnInf[jj].gn_id )
	// 			{
	// 				mmm = jj;
	// 				break;
	// 			}
	// 		}
	// 		if (cts_flag == 1)
	// 		{
	// 			memcpy( (void*)&sum_DevGnWarn[kkk], (void*)&m_DevGnWarn[i], sizeof(SSO_GN_MON) );
	// 			memcpy( (void*)&sum_pGnDat[kkk], (void*)&m_pGnDat[mmm], sizeof(SSO_GN_DAT) );
	// 			memcpy( (void*)&sum_pGnInf[kkk], (void*)&m_pGnInf[mmm], sizeof(SSO_GN_INF) );
	// 			gn_warn_time[kkk].m_GnStartTime = m_CalcStartTime;
	// 			gn_warn_time[kkk].m_GnEndTime = m_CurGpsTime;
	// 		}
	// 		else if(cts_flag == 0)
	// 		{
	// 			sum_WarnGen += 1;
	// 			memcpy( (void*)&sum_DevGnWarn[sum_WarnGen-1], (void*)&m_DevGnWarn[i], sizeof(SSO_GN_MON) );
	// 			memcpy( (void*)&sum_pGnDat[sum_WarnGen-1], (void*)&m_pGnDat[mmm], sizeof(SSO_GN_DAT) );
	// 			memcpy( (void*)&sum_pGnInf[sum_WarnGen-1], (void*)&m_pGnInf[mmm], sizeof(SSO_GN_INF) );
	// 			gn_warn_time[sum_WarnGen-1].m_GnStartTime = m_CalcStartTime;
	// 			gn_warn_time[sum_WarnGen-1].m_GnEndTime = m_CurGpsTime;
	// 		}
	// 	}
	// 	sprintf(m_LogString, "本次分析累加告警机组设备结束,数目[%d]",sum_WarnGen);
	// 	MessShow(m_LogString);
	// 
	// 	sprintf(m_LogString, "开始累加告警线路设备");
	// 	MessShow(m_LogString);
	// 	for ( i=0; i<m_nLnWarn; i++ )
	// 	{
	// 		cts_flag = 0;
	// 		for ( j=0; j<sum_WarnLn; j++ )
	// 		{
	// 			sprintf(m_LogString, "m_nLnWarn=[%d];sum_WarnLn=[%d];ID1=[%ld],ID2=[%ld]",m_nLnWarn,sum_WarnLn,m_DevLnWarn[i].ln_id,sum_DevLnWarn[j].ln_id);
	// 			MessShow(m_LogString);
	// 			if ( m_DevLnWarn[i].ln_id == sum_DevLnWarn[j].ln_id )
	// 			{
	// 				cts_flag = 1;
	// 				kkk = j;
	// 				break;
	// 			}
	// 		}
	// 		for ( jj=0; jj<m_nLnNum; jj++ )
	// 		{
	// 			if ( m_DevLnWarn[i].ln_id == m_pLnInf[jj].acln_id )
	// 			{
	// 				mmm = jj;
	// 				break;
	// 			}
	// 		}
	// 		if (cts_flag == 1)
	// 		{
	// 			memcpy( (void*)&sum_DevLnWarn[kkk], (void*)&m_DevLnWarn[i], sizeof(SSO_LN_MON) );
	// 			memcpy( (void*)&sum_pLnDat[kkk], (void*)&m_pLnDat[mmm], sizeof(SSO_LN_DAT) );
	// 			memcpy( (void*)&sum_pLnInf[kkk], (void*)&m_pLnInf[mmm], sizeof(SSO_LN_INF) );
	// 			ln_warn_time[kkk].m_LnStartTime = m_CalcStartTime;
	// 			ln_warn_time[kkk].m_LnEndTime = m_CurGpsTime;
	// 		}
	// 		else if(cts_flag == 0)
	// 		{
	// 			sum_WarnLn += 1;
	// 			memcpy( (void*)&sum_DevLnWarn[sum_WarnLn-1], (void*)&m_DevLnWarn[i], sizeof(SSO_LN_MON) );
	// 			memcpy( (void*)&sum_pLnDat[sum_WarnLn-1], (void*)&m_pLnDat[mmm], sizeof(SSO_LN_DAT) );
	// 			memcpy( (void*)&sum_pLnInf[sum_WarnLn-1], (void*)&m_pLnInf[mmm], sizeof(SSO_LN_INF) );
	// 			ln_warn_time[sum_WarnLn-1].m_LnStartTime = m_CalcStartTime;
	// 			ln_warn_time[sum_WarnLn-1].m_LnEndTime = m_CurGpsTime;
	// 		}
	// 	}
	// 	sprintf(m_LogString, "本次分析累加告警线路设备结束,数目[%d]",sum_WarnLn);
	// 	MessShow(m_LogString);
	// 
	// 	sprintf(m_LogString, "开始累加告警变压器设备");
	// 	MessShow(m_LogString);
	// 	for ( i=0; i<m_nTrWarn; i++ )
	// 	{
	// 		cts_flag = 0;
	// 		for ( j=0; j<sum_WarnTr; j++ )
	// 		{
	// 			sprintf(m_LogString, "m_nTrWarn=[%d];sum_WarnTr=[%d];ID1=[%ld],ID2=[%ld]",m_nTrWarn,sum_WarnTr,m_DevTrWarn[i].tr_id,sum_DevTrWarn[j].tr_id);
	// 			MessShow(m_LogString);
	// 			if ( m_DevTrWarn[i].tr_id == sum_DevTrWarn[j].tr_id )
	// 			{
	// 				cts_flag = 1;
	// 				kkk = j;
	// 				break;
	// 			}
	// 		}
	// 		for ( jj=0; jj<m_nTrNum; jj++ )
	// 		{
	// 			if ( m_DevTrWarn[i].tr_id == m_pTrInf[jj].tr_id )
	// 			{
	// 				mmm = jj;
	// 				break;
	// 			}
	// 		}
	// 		if (cts_flag == 1)
	// 		{
	// 			memcpy( (void*)&sum_DevTrWarn[kkk], (void*)&m_DevTrWarn[i], sizeof(SSO_TR_MON) );
	// 			memcpy( (void*)&sum_pTrDat[kkk], (void*)&m_pTrDat[mmm], sizeof(SSO_TR_DAT) );
	// 			memcpy( (void*)&sum_pTrInf[kkk], (void*)&m_pTrInf[mmm], sizeof(SSO_TR_INF) );
	// 			tr_warn_time[kkk].m_TrStartTime = m_CalcStartTime;
	// 			tr_warn_time[kkk].m_TrEndTime = m_CurGpsTime;
	// 		}
	// 		else if(cts_flag == 0)
	// 		{
	// 			sum_WarnTr += 1;
	// 			memcpy( (void*)&sum_DevTrWarn[sum_WarnTr-1], (void*)&m_DevTrWarn[i], sizeof(SSO_TR_MON) );
	// 			memcpy( (void*)&sum_pTrDat[sum_WarnTr-1], (void*)&m_pTrDat[mmm], sizeof(SSO_TR_DAT) );
	// 			memcpy( (void*)&sum_pTrInf[sum_WarnTr-1], (void*)&m_pTrInf[mmm], sizeof(SSO_TR_INF) );
	// 			tr_warn_time[sum_WarnTr-1].m_TrStartTime = m_CalcStartTime;
	// 			tr_warn_time[sum_WarnTr-1].m_TrEndTime = m_CurGpsTime;
	// 		}
	// 	}
	// 	sprintf(m_LogString, "本次分析累加告警变压器设备结束,数目[%d]",sum_WarnTr);
	// 	MessShow(m_LogString);
	// 
	// 	for (i=0;i<sum_WarnGen;i++)
	// 	{
	// 		sprintf(m_LogString, "sum_WarnGn=[%d];ID1=[%ld],ID2=[%ld]",sum_WarnGen,sum_DevGnWarn[i].gn_id,sum_pGnInf[i].gn_id);
	// 		MessShow(m_LogString);
	// 		if( sum_DevGnWarn[i].gn_id == sum_pGnInf[i].gn_id )
	// 		{
	// 			cts_num = cts_num+1;
	// 			sprintf(m_LogString, "更新第[%d]条记录\n",cts_num);
	// 			MessShow(m_LogString);
	// 			strcpy( LfoWarnDevCurve.dev_name, sum_pGnInf[i].gn_name );
	// 			LfoWarnDevCurve.dev_id		= sum_pGnInf[i].gn_id;
	// 			LfoWarnDevCurve.dev_idx     = PODB_GetDevIdByKeyID(LFO_DEV_CURVE_TAB,cts_num);						
	// 			sprintf(LfoWarnDevCurve.unit,"%s","Mw");						
	// 			LfoWarnDevCurve.sam_freq    = 0;
	//  			LfoWarnDevCurve.cuv_bgn_sec = gn_warn_time[i].m_GnStartTime;
	//  			LfoWarnDevCurve.cuv_end_sec = gn_warn_time[i].m_GnEndTime;
	// 			LfoWarnDevCurve.cuv_sec     = LfoWarnDevCurve.cuv_end_sec - LfoWarnDevCurve.cuv_bgn_sec;
	// 			LfoWarnDevCurve.cuv_pnt     = LFO_CURVE_MAX_PNT;
	// 			LfoWarnDevCurve.cuv_stp     = (float) ((LfoWarnDevCurve.cuv_sec*1.0)/LfoWarnDevCurve.cuv_pnt);
	// 			sprintf(m_LogString, "计算用采样点个数[%d],采样间隔[%d]\n",m_nSamples,m_nSmpStp);
	// 			MessShow(m_LogString);	
	// 			cts_nSamples = m_nSamples*2;
	// 			if(cts_nSamples >= LFO_CURVE_MAX_PNT)
	// 			{
	// 				for(k=0; k<LFO_CURVE_MAX_PNT; k++)
	// 				{
	// 					//no = m_nSmpStp*k;
	// // 					LfoWarnDevCurve.seq_cuv_data[k] = sum_pGnDat[i].pwr[k];
	// 				}
	// 			}
	// 			else
	// 			{
	// 				for(k=0; k < cts_nSamples; k++)
	// 				{
	// 					//	no = m_nSmpStp*k;
	// // 					LfoWarnDevCurve.seq_cuv_data[k] = sum_pGnDat[i].pwr[k];
	// 				}
	// 				for(k=cts_nSamples; k < LFO_CURVE_MAX_PNT; k++)
	// 				{
	// 				//	no = m_nSmpStp*k;
	// 					LfoWarnDevCurve.seq_cuv_data[k] = 999999.;
	// 				}
	// 			}              
	// 		   	UpdateWarnDevCurve(  LfoWarnDevCurve );
	// 			//break ;
	// 		}
	// 	}
	// 	
	// 	for (i=0;i<sum_WarnLn;i++)
	// 	{
	// 		sprintf(m_LogString, "sum_WarnLn=[%d];ID1=[%ld],ID2=[%ld]",sum_WarnLn,sum_DevLnWarn[i].ln_id,sum_pLnInf[i].acln_id);
	// 		MessShow(m_LogString);
	// 		if( sum_DevLnWarn[i].ln_id == sum_pLnInf[i].acln_id )
	// 		{
	// 			cts_num = cts_num+1;
	// 			sprintf(m_LogString, "更新第[%d]条记录\n",cts_num);
	// 			MessShow(m_LogString);
	// 			strcpy( LfoWarnDevCurve.dev_name, sum_pLnInf[i].acln_name );
	// 			LfoWarnDevCurve.dev_id		= sum_pLnInf[i].acln_id;
	// 			LfoWarnDevCurve.dev_idx     = PODB_GetDevIdByKeyID(LFO_DEV_CURVE_TAB,cts_num);						
	// 			sprintf(LfoWarnDevCurve.unit,"%s","Mw");						
	// 			LfoWarnDevCurve.sam_freq    = 0;
	// 			LfoWarnDevCurve.cuv_bgn_sec = ln_warn_time[i].m_LnStartTime;
	// 			LfoWarnDevCurve.cuv_end_sec = ln_warn_time[i].m_LnEndTime;
	// 			LfoWarnDevCurve.cuv_sec     = LfoWarnDevCurve.cuv_end_sec - LfoWarnDevCurve.cuv_bgn_sec;
	// 			LfoWarnDevCurve.cuv_pnt     = LFO_CURVE_MAX_PNT;
	// 			LfoWarnDevCurve.cuv_stp     = (float) ((LfoWarnDevCurve.cuv_sec*1.0)/LfoWarnDevCurve.cuv_pnt);
	// 			sprintf(m_LogString, "计算用采样点个数[%d],采样间隔[%d]\n",m_nSamples,m_nSmpStp);
	// 			MessShow(m_LogString);						
	// 			cts_nSamples = m_nSamples*2;
	// 			if(cts_nSamples >= LFO_CURVE_MAX_PNT)
	// 			{
	// 				for(k=0; k<LFO_CURVE_MAX_PNT; k++)
	// 				{
	// 					//no = m_nSmpStp*k;
	// // 					LfoWarnDevCurve.seq_cuv_data[k] = sum_pLnDat[i].pwr[k];
	// 				}
	// 			}
	// 			else
	// 			{
	// 				for(k=0; k < cts_nSamples; k++)
	// 				{
	// 				//	no = m_nSmpStp*k;
	// // 					LfoWarnDevCurve.seq_cuv_data[k] = sum_pLnDat[i].pwr[k];
	// 				}
	// 				for(k=cts_nSamples; k < LFO_CURVE_MAX_PNT; k++)
	// 				{
	// 					//	no = m_nSmpStp*k;
	// 					LfoWarnDevCurve.seq_cuv_data[k] = 999999.;
	// 				}
	// 			}              
	// 	    	UpdateWarnDevCurve(  LfoWarnDevCurve );
	// 			//break ;
	// 		}
	// 	}
	// 
	// 	for (i=0;i<sum_WarnTr;i++)
	// 	{
	// 		sprintf(m_LogString, "sum_WarnTr=[%d];ID1=[%ld],ID2=[%ld]",sum_WarnTr,sum_DevTrWarn[i].tr_id,sum_pTrInf[i].tr_id);
	// 		MessShow(m_LogString);
	// 		if( sum_DevTrWarn[i].tr_id == sum_pTrInf[i].tr_id )
	// 		{
	// 			cts_num = cts_num+1;
	// 			sprintf(m_LogString, "更新第[%d]条记录\n",cts_num);
	// 			MessShow(m_LogString);
	// 			strcpy( LfoWarnDevCurve.dev_name, sum_pTrInf[i].tr_name );
	// 			LfoWarnDevCurve.dev_id		= sum_pTrInf[i].tr_id;
	// 			LfoWarnDevCurve.dev_idx     = PODB_GetDevIdByKeyID(LFO_DEV_CURVE_TAB,cts_num);						
	// 			sprintf(LfoWarnDevCurve.unit,"%s","Mw");						
	// 			LfoWarnDevCurve.sam_freq    = 0;
	// 			LfoWarnDevCurve.cuv_bgn_sec = tr_warn_time[i].m_TrStartTime;
	// 			LfoWarnDevCurve.cuv_end_sec = tr_warn_time[i].m_TrEndTime;
	// 			LfoWarnDevCurve.cuv_sec     = LfoWarnDevCurve.cuv_end_sec - LfoWarnDevCurve.cuv_bgn_sec;
	// 			LfoWarnDevCurve.cuv_pnt     = LFO_CURVE_MAX_PNT;
	// 			LfoWarnDevCurve.cuv_stp     = (float) ((LfoWarnDevCurve.cuv_sec*1.0)/LfoWarnDevCurve.cuv_pnt);
	// 			sprintf(m_LogString, "计算用采样点个数[%d],采样间隔[%d]\n",m_nSamples,m_nSmpStp);
	// 			MessShow(m_LogString);	
	// 			cts_nSamples = m_nSamples*2;
	// 			if(cts_nSamples >= LFO_CURVE_MAX_PNT)
	// 			{
	// 				for(k=0; k<LFO_CURVE_MAX_PNT; k++)
	// 				{
	// 					//no = m_nSmpStp*k;
	// // 					LfoWarnDevCurve.seq_cuv_data[k] = sum_pTrDat[i].pwr[k];
	// 				}
	// 			}
	// 			else
	// 			{
	// 				for(k=0; k < cts_nSamples; k++)
	// 				{
	// 					//	no = m_nSmpStp*k;
	// // 					LfoWarnDevCurve.seq_cuv_data[k] = sum_pTrDat[i].pwr[k];
	// 				}
	// 				for(k=cts_nSamples; k < LFO_CURVE_MAX_PNT; k++)
	// 				{
	// 					//	no = m_nSmpStp*k;
	// 					LfoWarnDevCurve.seq_cuv_data[k] = 999999.;
	// 				}
	// 			}              
	// 			UpdateWarnDevCurve(  LfoWarnDevCurve );
	// 			//break ;
	// 		}
	// 	}
	// 	
	// #endif
	return;
}

//  发送报警信息const LFO_SYS_MON status_info, int time_leng, int factory_id
int CLfoAnaObj::LFOSendWarnMessage(const SSO_SYS_MON status_info, int warn_time, INT_REC factory_id)
{
#ifndef _NUSP
#ifdef _WAMAP_
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
	// 7天缓冲数据 画面显示利用平台研究态research context 
	if (g_ctxNo == PUB_AC_RESEARCH_NO)
	{
		sprintf(m_LogString, "LFOSendWarnMessage(): 离线分析，退出告警信息发送!");
		MessShow(m_LogString);
		return 0;
	}

	if (m_DispCtl.is_warn == MENU_LFO_MENU_NO)
	{
		sprintf(m_LogString, "LFOSendWarnMessage(): m_DispCtl.is_warn = %s[%d],退出告警信息发送!", "否", MENU_LFO_MENU_NO);
		MessShow(m_LogString);
		return 0;
	}

	string tmpstring;
	tmpstring = PT_GetChineseTimeStringBySeconds(warn_time);
	if (status_info.stat >= MENU_LFO_DEV_STS_FWN)
	{
		tmpstring += " 次同步振荡告警,";
	}
	tmpstring += status_info.stat_ctx;

	sprintf(m_LogString, "LFOSendWarnMessage(): %s,fac_id = "INT_REC_FMT"", tmpstring.c_str(), INT_REC_VAL(factory_id));
	MessShow(m_LogString);

	vector<WARN_MESSAGE_STRUCT> tmpVecWarn;
	WARN_MESSAGE_STRUCT tmpWarn;
	tmpWarn.appno = g_appNo;
	tmpWarn.warntype = WAMS_SSO_WARN_TYPE;
	PODB_WarnMsgBaseTime(tmpWarn, warn_time);
	PODB_WarnMsgBaseINT(tmpWarn, status_info.stat);
	PODB_WarnMsgBaseSTRING(tmpWarn, tmpstring);
#ifdef _D5000
	PODB_WarnMsgBaseLONG(tmpWarn, factory_id);
#else
	PODB_WarnMsgBaseINT(tmpWarn, factory_id);
#endif

	tmpVecWarn.push_back(tmpWarn);

#ifdef _PODB_WARN_
	PODB_SendWarnMessage(tmpVecWarn, para_WarnAppNo);
#endif
#endif  // _WAMAP_
#endif // !_NUSP
	return 0;
}

int CLfoAnaObj::SendWarnToIA(const int mode)
{
	if (para_IsWarnIA == 0)
	{
		return 0;
	}

	// 填充告警结构体
	LOW_FRE_OSCI_INFO_TO_IA_STRU tmpLFOIA;
	memset((void*)&tmpLFOIA, 0, sizeof(LOW_FRE_OSCI_INFO_TO_IA_STRU));
	tmpLFOIA.search_id = m_SysResSave.obj_id;
	tmpLFOIA.occur_time = m_SysResSave.lfo_b_t;
	tmpLFOIA.msec = 0;
	tmpLFOIA.end_time = m_SysResSave.lfo_e_t;
	tmpLFOIA.dev_id = m_SysResSave.obj_id;
	sprintf(tmpLFOIA.dev_descr, "%s", m_SysResSave.obj_name);
	tmpLFOIA.st_id = m_SysResSave.fac_id;
	tmpLFOIA.app_id = g_appNo;
	tmpLFOIA.criterion_num = m_SysResSave.lfo_line_num + m_SysResSave.act_gen_num + m_SysResSave.lfo_tr_num;
	if (mode == L_FST_FORM_RESULTE)
	{
		tmpLFOIA.osci_id = m_SysResSave.obj_id;
		tmpLFOIA.status = 1;
	}
	else if (mode == L_SnD_FORM_RESULTE)
	{
		//tmpLFOIA.osci_id = 1;
		tmpLFOIA.status = 2;
	}
	else if (mode == L_END_FORM_RESULTE)
	{
		//tmpLFOIA.osci_id = 1;
		tmpLFOIA.status = 3;
	}

	if (m_SysResSave.mode1_num >= 1)
	{
		tmpLFOIA.damp_rate = m_SysResSave.mode1[m_SysResSave.mode1_num - 1].damp;
		tmpLFOIA.oscillation_fre = m_SysResSave.mode1[m_SysResSave.mode1_num - 1].freq;
		tmpLFOIA.oscillation_value = m_SysResSave.mode1[m_SysResSave.mode1_num - 1].ampl;
		tmpLFOIA.phase_angle = m_SysResSave.mode1[m_SysResSave.mode1_num - 1].phase;
	}

	tmpLFOIA.continus_flag = 0;

	OSCI_DEV_INFO_STRU tmpDev;
	vector<OSCI_DEV_INFO_STRU> tmpVecDev;
	int ii;
	for (ii = 0; ii < m_nGnNum; ii++)
	{
		if (m_pGnResSave[ii].lfo_b_t > 0)
		{
			memset((void*)&tmpDev, 0, sizeof(OSCI_DEV_INFO_STRU));
			//tmpDev.osci_id = m_pGnResSave[ii].obj_id;
			tmpDev.key_id = m_pGnResSave[ii].obj_id;
			// 机组分群默认置为1
			tmpDev.group_num = 1;
			sprintf(tmpDev.descr, "%s", m_pGnResSave[ii].obj_name);
			if (m_pGnResSave[ii].mode1_num >= 1)
			{
				tmpDev.damp_rate = m_pGnResSave[ii].mode1[m_pGnResSave[ii].mode1_num - 1].damp;
				tmpDev.oscillation_fre = m_pGnResSave[ii].mode1[m_pGnResSave[ii].mode1_num - 1].freq;
				tmpDev.oscillation_value = m_pGnResSave[ii].mode1[m_pGnResSave[ii].mode1_num - 1].ampl;
				tmpDev.phase_angle = m_pGnResSave[ii].mode1[m_pGnResSave[ii].mode1_num - 1].phase;
			}

			tmpVecDev.push_back(tmpDev);
		}
	}
	for (ii = 0; ii < m_nLnNum; ii++)
	{
		if (m_pLnResSave[ii].lfo_b_t > 0)
		{
			memset((void*)&tmpDev, 0, sizeof(OSCI_DEV_INFO_STRU));
			//tmpDev.osci_id = m_pGnResSave[ii].obj_id;
			tmpDev.key_id = m_pLnResSave[ii].obj_id;
			// 线路分群默认置为0
			tmpDev.group_num = 0;
			sprintf(tmpDev.descr, "%s", m_pLnResSave[ii].obj_name);
			if (m_pLnResSave[ii].mode1_num >= 1)
			{
				tmpDev.damp_rate = m_pLnResSave[ii].mode1[m_pLnResSave[ii].mode1_num - 1].damp;
				tmpDev.oscillation_fre = m_pLnResSave[ii].mode1[m_pLnResSave[ii].mode1_num - 1].freq;
				tmpDev.oscillation_value = m_pLnResSave[ii].mode1[m_pLnResSave[ii].mode1_num - 1].ampl;
				tmpDev.phase_angle = m_pLnResSave[ii].mode1[m_pLnResSave[ii].mode1_num - 1].phase;
			}

			tmpVecDev.push_back(tmpDev);
		}
	}
	for (ii = 0; ii < m_nTrNum; ii++)
	{
		if (m_pTrResSave[ii].lfo_b_t > 0)
		{
			memset((void*)&tmpDev, 0, sizeof(OSCI_DEV_INFO_STRU));
			//tmpDev.osci_id = m_pGnResSave[ii].obj_id;
			tmpDev.key_id = m_pTrResSave[ii].obj_id;
			// 变压器分群默认置为0
			tmpDev.group_num = 0;
			sprintf(tmpDev.descr, "%s", m_pTrResSave[ii].obj_name);
			if (m_pTrResSave[ii].mode1_num >= 1)
			{
				tmpDev.damp_rate = m_pTrResSave[ii].mode1[m_pTrResSave[ii].mode1_num - 1].damp;
				tmpDev.oscillation_fre = m_pTrResSave[ii].mode1[m_pTrResSave[ii].mode1_num - 1].freq;
				tmpDev.oscillation_value = m_pTrResSave[ii].mode1[m_pTrResSave[ii].mode1_num - 1].ampl;
				tmpDev.phase_angle = m_pTrResSave[ii].mode1[m_pTrResSave[ii].mode1_num - 1].phase;
			}

			tmpVecDev.push_back(tmpDev);
		}
	}

	tmpLFOIA.criterion_num = (int)tmpVecDev.size();

	// 	sprintf(m_LogString,"search_id["INT_REC_FMT"],occur_time[%d],end_time[%d],dev_id["INT_REC_FMT"],dev_descr[%s]",
	// 		tmpLFOIA.search_id,tmpLFOIA.occur_time,tmpLFOIA.end_time,tmpLFOIA.dev_id,tmpLFOIA.dev_descr);
	// 	MessShow(m_LogString,LTDEBUG);
	// 	sprintf(m_LogString,"st_id["INT_REC_FMT"],app_id[%d],criterion_num[%d],osci_id["INT_REC_FMT"],status[%d]",
	// 		tmpLFOIA.st_id,tmpLFOIA.app_id,tmpLFOIA.criterion_num,tmpLFOIA.osci_id,tmpLFOIA.status);
	// 	MessShow(m_LogString,LTDEBUG);
	// 	sprintf(m_LogString,"damp_rate[%f],oscillation_fre[%f],oscillation_value[%f],phase_angle[%f],continus_flag[%d]",
	// 		tmpLFOIA.damp_rate,tmpLFOIA.oscillation_fre,tmpLFOIA.oscillation_value,tmpLFOIA.phase_angle,tmpLFOIA.continus_flag);
	// 	MessShow(m_LogString,LTDEBUG);
	// 	sprintf(m_LogString,"no,osci_id,key_id,group_num,group_name,descr,damp_rate,oscillation_fre,oscillation_value,phase_angle");
	// 	MessShow(m_LogString,LTDEBUG);
	for (ii = 0; ii < (int)tmpVecDev.size(); ii++)
	{
		sprintf(m_LogString, "%d,"INT_REC_FMT","INT_REC_FMT",%d,%s,%s,%f,%f,%f,%f", ii + 1,
			INT_REC_VAL(tmpVecDev[ii].osci_id), INT_REC_VAL(tmpVecDev[ii].key_id), tmpVecDev[ii].group_num, tmpVecDev[ii].group_name,
			tmpVecDev[ii].descr, tmpVecDev[ii].damp_rate, tmpVecDev[ii].oscillation_fre, tmpVecDev[ii].oscillation_value, tmpVecDev[ii].phase_angle);
		MessShow(m_LogString, LTDEBUG);
	}

#ifdef _WAMAP_
	sprintf(m_LogString, "向综合智能告警(科东)发送告警消息[%d]", para_IsWarnIA);
	MessShow(m_LogString, LTDEBUG);
	// 判断值班主机
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
#ifdef _PODB_MSGBUS_
#ifdef _D5000
	Message msg;
	size_t buf_size = 0;
	memset(msg.Msg_buf, 0, sizeof(msg.Msg_buf));
	msg.header.event = LOW_FRE_OSCI_INFO_TO_IA;
	msg.header.serv = CH_TRANS_EVENT;

	memcpy((void*)(msg.Msg_buf + buf_size), (void*)&tmpLFOIA, sizeof(LOW_FRE_OSCI_INFO_TO_IA_STRU));
	buf_size += sizeof(LOW_FRE_OSCI_INFO_TO_IA_STRU);

	for (ii = 0; ii < (int)tmpVecDev.size(); ii++)
	{
		if ((buf_size + sizeof(OSCI_DEV_INFO_STRU)) >= MAX_MSGBUF_LEN)
		{
			sprintf(m_LogString, "消息内存越界["INT_REC_FMT"]>[%d],截取前["INT_REC_FMT"]", (buf_size + sizeof(OSCI_DEV_INFO_STRU)), MAX_MSGBUF_LEN, buf_size);
			MessShow(m_LogString, LTERROR);
			break;
		}

		OSCI_DEV_INFO_STRU& tmpDevStruct = *((OSCI_DEV_INFO_STRU*)(msg.Msg_buf + buf_size));
		tmpDevStruct = tmpVecDev[ii];
		buf_size += sizeof(OSCI_DEV_INFO_STRU);
	}

	if (PODB_messageSend((Message*)&msg, buf_size, NULL) < 0)
	{
		sprintf(m_LogString, "低频振荡告警消息发送失败");
		MessShow(m_LogString, LTINFO);
	}
#endif
#endif  // #ifdef _PODB_MSGBUS_
#endif
	return 0;
}

// 2008-05-31 by zhw at fj
/**************************************************************
* 功能：向EMS转发LFO信息
* 参数：
* 返回：int
***************************************************************/
int CLfoAnaObj::LFOTriEmsSingle(SSO_SYS_MON sys_mon)
{
	// #ifdef _WAMAP_
	// #ifndef  _WINDOWS32 
	//     if(PODB_IsOnDuty(g_appNo, g_ctxNo) != 1 )
	//     {
	//         sprintf( m_LogString, "LFOTriEmsSingle(): 本机不是WAMAP主机,退出向EMS转发LFO信息!" ); 
	//         MessShow( m_LogString );
	//         return 0;
	//     }
	// #endif
	//     
	// #ifdef  USE_WEEK_WAM_MEM // 7天缓冲数据 画面显示利用平台研究态research context 
	//     sprintf( m_LogString, "LFOTriEmsSingle(): 离线分析，退出向EMS转发LFO信息!" ); 
	//     MessShow( m_LogString );
	//     return 0;
	// #endif
	//     int app_no   = g_appNo;
	//     int table_no = 0; 
	//     unsigned char triEms_value = MENU_LFO_MENU_YES;  // 南网开关状态，安全状态下合[1]，告警状态下分[0]
	//     
	//     if ( m_DispCtl.is_triems == MENU_LFO_MENU_NO )
	//     {
	//         return 0;
	//     }
	//     
	// 	table_no = PODB_GetTabelNoByDevId(m_DispCtl.ems_keyid.record_id);
	//     int record_size = sizeof(unsigned char);
	//     if ( sys_mon.stat >= MENU_LFO_DEV_STS_WRN ) triEms_value = MENU_LFO_MENU_NO;
	// 	int ret_code = PODB_TableModifyByKey((const char *)&m_DispCtl.ems_keyid.record_id, m_DispCtl.ems_keyid.column_id, (const char *)&triEms_value, sizeof(unsigned char), table_no);
	// 
	//     if ( ret_code < 0 ) 
	//     {
	//         sprintf(m_LogString, "LFOTriEmsSingle(): Write record to table: %d ,error. record_id = %d, column_id = %d \n",\
	//             table_no, m_DispCtl.ems_keyid.record_id, m_DispCtl.ems_keyid.column_id ) ;
	//         MessShow( m_LogString );
	//         return -1 ;
	//     } 
	//     sprintf(m_LogString, "LFOTriEmsSingle(): Write record to table: %d ,success \n",\
	//         table_no, m_DispCtl.ems_keyid.record_id, m_DispCtl.ems_keyid.column_id ) ;
	//     MessShow( m_LogString );
	// #endif
	return 0;
}


//触发采样，需在sample_define中定义
//  2008-08-17
int CLfoAnaObj::LFOTriggerSample()
{
	if (m_DispCtl.is_samp == 0)
	{
		sprintf(m_LogString, "退出触发采样![%d]", m_DispCtl.is_samp);
		MessShow(m_LogString, LTINFO);
		return 0;
	}
#ifdef _WAMAP_
#ifdef _PODB_SAMPLE_
#ifndef  _WINDOWS32 
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		sprintf(m_LogString, "本机不是WAMAP主机,退出触发采样!");
		MessShow(m_LogString, LTINFO);
		return 0;
	}
#endif

#ifdef  USE_WEEK_WAM_MEM // 7天缓冲数据 画面显示利用平台研究态research context 
	sprintf(m_LogString, "离线分析，退出触发采样!");
	MessShow(m_LogString, LTINFO);
	return 0;
#endif

#ifdef _D5000
	//	SampleHisEve();
	SampleHisCase();
	SampleHisShape();
#else
	int   k;

	char hostname[50];
	int  ret_code = 0;
	int  key_id = LFO_PARA_NO * DB_TABLE_BASE + 1;

	//	CServicesManage *m_SerManage = new CServicesManage;
	//得到WAMAP主机                        // AP_WAMAP
	ret_code = PODB_GetHostName(hostname, sizeof(hostname), g_appNo, g_ctxNo);
	if (ret_code <= 0)
	{
		sprintf(m_LogString, "GetAppHostName()::Get Host Name Error");
		MessShow(m_LogString);
		return ret_code;
	}

	//触发采样
	TSEQSampleProcessRequest    lfoSample;

	lfoSample.seq_length = 1;
	lfoSample.seq_sample_process_request.length(lfoSample.seq_length);
	lfoSample.seq_sample_process_request[0].sample_type = 148;  // 采样类型，即定义的触发采样表
	lfoSample.seq_sample_process_request[0].app_no = g_appNo;//AP_WAMAP;
	lfoSample.seq_sample_process_request[0].sample_mode = OFFERDATA_SAMPLE_MODE;
	lfoSample.seq_sample_process_request[0].occur_time = m_CurGpsTime;
	lfoSample.seq_sample_process_request[0].flag = 0; // flag = 0, 执行insert操作, 默认; flag = 1, 执行update操作; flag = 2, 智能操作, 判断是否存在作insert或update操作
	lfoSample.seq_sample_process_request[0].reserve_1 = 0;
	lfoSample.seq_sample_process_request[0].reserve_2 = 0;
	lfoSample.seq_sample_process_request[0].keyid_num = 8;
	lfoSample.seq_sample_process_request[0].seq_keyid_value.length(lfoSample.seq_sample_process_request[0].keyid_num);
	k = 0;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.record_id = key_id;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.column_id = 74;  // 振荡案例描述
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_datatype = SAMPLE_DATATYPE_STRING;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_value.c_string((const ::CORBA::String_var)CORBA::string_dup(m_SysResSave.lfo_case_des));
	++k;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.record_id = key_id;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.column_id = 75;  // LFO发生时间
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_datatype = SAMPLE_DATATYPE_INT;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_value.c_int(m_SysResSave.lfo_b_t);
	++k;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.record_id = key_id;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.column_id = 76;  // LFO消失时间
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_datatype = SAMPLE_DATATYPE_INT;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_value.c_int(m_SysResSave.lfo_e_t);
	++k;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.record_id = key_id;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.column_id = 77;  // 持续时间
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_datatype = SAMPLE_DATATYPE_STRING;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_value.c_string((const ::CORBA::String_var)CORBA::string_dup(PT_GetStringOfTimeInterval(m_SysResSave.hold_time * 1000)));
	++k;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.record_id = key_id;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.column_id = 79;  // 触发告警设备
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_datatype = SAMPLE_DATATYPE_STRING;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_value.c_string((const ::CORBA::String_var)CORBA::string_dup(m_SysResSave.obj_name));
	++k;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.record_id = key_id;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.column_id = 80;  // 触发告警频率
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_datatype = SAMPLE_DATATYPE_FLOAT;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_value.c_float(m_SysResSave.freq);
	++k;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.record_id = key_id;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.column_id = 82;  // LFO线路数
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_datatype = SAMPLE_DATATYPE_INT;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_value.c_int(m_SysResSave.lfo_line_num);
	++k;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.record_id = key_id;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid.column_id = 83;  // LFO发电机数
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_datatype = SAMPLE_DATATYPE_INT;
	lfoSample.seq_sample_process_request[0].seq_keyid_value[k].keyid_value.c_int(m_SysResSave.act_gen_num);

	char* msg_buf = NULL;
	int     buf_len = 0;
	MARSHAL(lfoSample, msg_buf, buf_len);

	//    //ret_code = WAMAPMsgBus->SendMessageToHost(msg_buf, DB_TRIGGER_SAMPLE_TYPE, buf_len, hostname, "TRIGGER_SAMPLE_SERVER");
	ret_code = PODB_SendMessageToHost(msg_buf, DB_TRIGGER_SAMPLE_TYPE, buf_len, hostname, "TRIGGER_SAMPLE_SERVER");

	if (msg_buf) { free(msg_buf); msg_buf = NULL; }
	//free(msg_buf);

	if (ret_code <= 0)
	{
		sprintf(m_LogString, "LFOTriggerSample(): SendMessageToHost() error");
		MessShow(m_LogString);
		return -1;
	}

	sprintf(m_LogString, "LFOTriggerSample(): LFOTriggerSample success");
	MessShow(m_LogString);
#endif // _D5000
#endif // #ifdef _PODB_SAMPLE_
#endif
	return 0;
}

int CLfoAnaObj::UpdateLfoEvent()
{
	// #ifdef _D5000
	// #ifdef _WAMAP_
	// #ifdef _PODB_SAMPLE_
	// 	//LFO_LPO_EVE_CRE tmpLfoEvent;
	// 	//memset((void *)&tmpLfoEvent,0,sizeof(LFO_LPO_EVE_CRE));
	// 	LFO_LPO_EVE_WAMS tmpLfoEvent;
	// 	memset((void *)&tmpLfoEvent,0,sizeof(LFO_LPO_EVE_WAMS));
	// 	int num = PODB_GetTabRecNum(LFO_LPO_EVE_NO_TAB);
	// 	if (num > 500)
	// 	{
	// 		// 超过500条记录，清空表
	// 		PODB_TableClear(LFO_LPO_EVE_NO_TAB);
	// 		num = 0;
	// 	}
	// 	tmpLfoEvent.record_id = PODB_GetDevIdByKeyID(LFO_LPO_EVE_NO_TAB,num+1);
	// 	tmpLfoEvent.eve_id = m_SysResSave.lfo_b_t;
	// 	tmpLfoEvent.win_id = m_SysResSave.lfo_b_t;
	// 	tmpLfoEvent.analy_bgn = m_SysResSave.lfo_b_t;
	// 	tmpLfoEvent.analy_end = m_SysResSave.lfo_e_t;
	// // 	tmpLfoEvent.ana_his_bgn = m_SysResSave.lfo_b_t - m_LfoPara.rcd_pre_t;
	// // 	tmpLfoEvent.ana_his_end = m_SysResSave.lfo_e_t + m_LfoPara.rcd_aft_t;
	// 	tmpLfoEvent.stat = MENU_LFO_DEV_STS_WRN;
	// 	tmpLfoEvent.show_gn_num = m_SysResSave.act_gen_num;
	// 	tmpLfoEvent.show_ln_num = m_SysResSave.lfo_line_num;
	// 	tmpLfoEvent.show_tr_num = m_SysResSave.lfo_tr_num;
	// 	tmpLfoEvent.freq = m_SysResSave.freq;
	// 	tmpLfoEvent.is_show = MENU_LFO_MON_YES;
	// 	sprintf(tmpLfoEvent.case_des,"%s",m_SysResSave.lfo_case_des);
	// 	tmpLfoEvent.warn_dev_id = m_SysResSave.obj_id;
	// 	tmpLfoEvent.warn_dev_facid = m_SysResSave.fac_id;
	// 	sprintf(tmpLfoEvent.warn_dev_facname,"%s",m_SysResSave.fac_name);
	// 	sprintf(tmpLfoEvent.warn_dev_name,"%s",m_SysResSave.obj_name);
	// 	sprintf(tmpLfoEvent.lfo_time,"%s",PT_GetStringOfTimeInterval(m_SysResSave.hold_time*1000));
	// 	tmpLfoEvent.save_type = 0;

	// 	vector<string> VecSql;
	// 	char SqlBuffer[1024];
	// 	char time1[20],time2[20],time3[20],time4[20],time5[20],time6[20];
	// 	sprintf(time1,"%s",PT_GetTimeStringBySeconds(tmpLfoEvent.eve_id));
	// 	sprintf(time2,"%s",PT_GetTimeStringBySeconds(tmpLfoEvent.win_id));
	// 	sprintf(time3,"%s",PT_GetTimeStringBySeconds(tmpLfoEvent.analy_bgn));
	// 	sprintf(time4,"%s",PT_GetTimeStringBySeconds(tmpLfoEvent.analy_end));
	// 	sprintf(time5,"%s",PT_GetTimeStringBySeconds(tmpLfoEvent.ana_his_bgn));
	// 	sprintf(time6,"%s",PT_GetTimeStringBySeconds(tmpLfoEvent.ana_his_end));
	// 	sprintf(SqlBuffer,""LFO_LPO_EVE_CRE_FMT"",
	// 		"lfo_lpo_eve",
	// 		LFO_LPO_EVE_CRE_FLD,
	// 		tmpLfoEvent.record_id,
	// 		time1,time2,time3,time4,time5,time6,
	// 		LFO_LPO_EVE_CRE_VAL(tmpLfoEvent)
	// 		);
	// 	VecSql.push_back(SqlBuffer);
	// 	MessShow(SqlBuffer,LTDEBUG);
	// 	if (PODB_ModifyTableBySqls(VecSql)<0)
	// 	{
	// 		sprintf(m_LogString,"更新LFO_LPO信息表失败");
	// 		MessShow(m_LogString,LTERROR);
	// 	}
	// 	else
	// 	{
	// 		sprintf(m_LogString,"更新LFO_LPO信息表成功");
	// 		MessShow(m_LogString,LTERROR);
	// 	}

	// 	MessShow("更新LFO_LPO信息表");
	// 	//PODB_TableModifyByKey((const char *)&tmpLfoEvent.record_id,LFO_LPO_EVE_CRE_FLD,(const char *)&tmpLfoEvent.eve_id,sizeof(LFO_LPO_EVE_CRE)-sizeof(tmpLfoEvent.record_id),LFO_LPO_EVE_NO_TAB);
	// 	PODB_TableWrite((const char *)&tmpLfoEvent.record_id,sizeof(tmpLfoEvent),LFO_LPO_EVE_NO_TAB);
	// #endif
	// #endif
	// #endif
	return 0;
}


int CLfoAnaObj::UpdateHisCase()
{
	// #ifdef _D5000
	// #ifdef _WAMAP_
	// #ifdef _PODB_SAMPLE_
	// 	// 清空原表内容
	// 	//PODB_TableClearSql("lfo_case_wds");
	// 	//PODB_TableClear(LFO_CASE_WDS_NO_TAB);
	// 
	// 	MessShow("更新LFO实时CASE窗口表");
	// 
	// 	int idx = PODB_GetTabRecNum(LFO_CASE_WDS_NO_TAB);
	// 	LFO_CASE_WDS_WAMS tmpLfoCase;
	// 	vector<LFO_CASE_WDS_WAMS> tmpVecLfoCase;
	// 	int ii;
	// 	vector<string> VecSql;
	// 	char SqlBuffer[1024];
	// 	char time1[20],time2[20],time3[20],time4[20],time5[20],time6[20];
	// 	for ( ii =0; ii<m_nGnNum; ii++)
	// 	{
	// 		if (m_pGnResSave[ii].lfo_b_t > 0 && m_pGnResSave[ii].lfo_e_t > 0)
	// 		{
	// 			memset((void *)&tmpLfoCase,0,sizeof(LFO_CASE_WDS_WAMS));
	// 			sprintf(tmpLfoCase.lfo_time,"%s",PT_GetStringOfTimeInterval(m_pGnResSave[ii].hold_time*1000));
	// 
	// 			tmpLfoCase.eve_id = m_SysResSave.lfo_b_t;
	// 			tmpLfoCase.win_id = m_SysResSave.lfo_b_t;
	// 			tmpLfoCase.analy_bgn = m_pGnResSave[ii].lfo_b_t;
	// 			tmpLfoCase.analy_end = m_pGnResSave[ii].lfo_e_t;
	// 			tmpLfoCase.lfodev_id = m_pGnResSave[ii].obj_id;
	// 			sprintf(tmpLfoCase.lfodev_name,"%s",m_pGnResSave[ii].obj_name);
	// 			tmpLfoCase.fac_id = m_pGnResSave[ii].fac_id;
	// 			sprintf(tmpLfoCase.fac_name,"%s",m_pGnResSave[ii].fac_name);
	// 			tmpLfoCase.dev_type = MENU_LFO_DEV_GEN;
	// 			tmpLfoCase.max_ampl_rat = m_pGnResSave[ii].max_value.value - m_pGnResSave[ii].min_value.value;
	// 			tmpLfoCase.lfo_max_val = m_pGnResSave[ii].max_value.value;
	// 			tmpLfoCase.lfo_max_val_t = m_pGnResSave[ii].max_value.time;
	// 			tmpLfoCase.lfo_min_val = m_pGnResSave[ii].min_value.value;
	// 			tmpLfoCase.lfo_min_val_t = m_pGnResSave[ii].min_value.time;
	// 
	// 			idx ++;
	// 			tmpLfoCase.wds_id = PODB_GetDevIdByKeyID(LFO_CASE_WDS_NO_TAB,idx);
	// 			tmpVecLfoCase.push_back(tmpLfoCase);
	// 			//PODB_TableModifyByKey((const char *)&tmpLfoCase.wds_id,LFO_CASE_WDS_CRE_FLD,(const char *)&tmpLfoCase.eve_id,sizeof(LFO_CASE_WDS_CRE)-sizeof(tmpLfoCase.wds_id),LFO_CASE_WDS_NO_TAB);

	// 			sprintf(time1,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.eve_id));
	// 			sprintf(time2,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.win_id));
	// 			sprintf(time3,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.analy_bgn));
	// 			sprintf(time4,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.analy_end));
	// 			sprintf(time5,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.lfo_max_val_t));
	// 			sprintf(time6,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.lfo_max_val_t));
	// 
	// 			sprintf(SqlBuffer,""LFO_CASE_WDS_CRE_FMT"",
	// 				"lfo_case_wds",
	// 				LFO_CASE_WDS_CRE_FLD,
	// 				time1,time2,time3,time4,
	// 				LFO_CASE_WDS_CRE_VAL(tmpLfoCase),
	// 				time5,tmpLfoCase.lfo_min_val,time6
	// 				);
	// 			VecSql.push_back(SqlBuffer);
	// 		}
	// 	}
	// 
	// 	for ( ii =0; ii<m_nLnNum; ii++)
	// 	{
	// 		if (m_pLnResSave[ii].lfo_b_t > 0 && m_pLnResSave[ii].lfo_e_t > 0)
	// 		{
	// 			memset((void *)&tmpLfoCase,0,sizeof(LFO_CASE_WDS_WAMS));
	// 			sprintf(tmpLfoCase.lfo_time,"%s",PT_GetStringOfTimeInterval(m_pLnResSave[ii].hold_time*1000));
	// 			tmpLfoCase.eve_id = m_SysResSave.lfo_b_t;
	// 			tmpLfoCase.win_id = m_SysResSave.lfo_b_t;
	// 			tmpLfoCase.analy_bgn = m_pLnResSave[ii].lfo_b_t;
	// 			tmpLfoCase.analy_end = m_pLnResSave[ii].lfo_e_t;
	// 			tmpLfoCase.lfodev_id = m_pLnResSave[ii].obj_id;
	// 			tmpLfoCase.fac_id = m_pLnResSave[ii].fac_id;
	// 			sprintf(tmpLfoCase.fac_name,"%s",m_pLnResSave[ii].fac_name);
	// 			tmpLfoCase.dev_type = MENU_LFO_DEV_LINE;
	// 			tmpLfoCase.max_ampl_rat = m_pLnResSave[ii].max_value.value - m_pLnResSave[ii].min_value.value;
	// 			tmpLfoCase.lfo_max_val = m_pLnResSave[ii].max_value.value;
	// 			tmpLfoCase.lfo_max_val_t = m_pLnResSave[ii].max_value.time;
	// 			tmpLfoCase.lfo_min_val = m_pLnResSave[ii].min_value.value;
	// 			tmpLfoCase.lfo_min_val_t = m_pLnResSave[ii].min_value.time;
	// 			
	// 			idx ++;
	// 			tmpLfoCase.wds_id = PODB_GetDevIdByKeyID(LFO_CASE_WDS_NO_TAB,idx);
	// 			tmpVecLfoCase.push_back(tmpLfoCase);
	// 			//PODB_TableModifyByKey((const char *)&tmpLfoCase.wds_id,LFO_CASE_WDS_CRE_FLD,(const char *)&tmpLfoCase.eve_id,sizeof(LFO_CASE_WDS_CRE)-sizeof(tmpLfoCase.wds_id),LFO_CASE_WDS_NO_TAB);
	// 
	// // 			sprintf(time1,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.eve_id));
	// // 			sprintf(time2,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.win_id));
	// // 			sprintf(time3,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.analy_bgn));
	// // 			sprintf(time4,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.analy_end));
	// // 			sprintf(time5,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.lfo_max_val_t));
	// // 			sprintf(time6,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.lfo_max_val_t));
	// // 			
	// // 			sprintf(SqlBuffer,""LFO_CASE_WDS_CRE_FMT"",
	// // 				"lfo_case_wds",
	// // 				LFO_CASE_WDS_CRE_FLD,
	// // 				time1,time2,time3,time4,
	// // 				LFO_CASE_WDS_CRE_VAL(tmpLfoCase),
	// // 				time5,tmpLfoCase.lfo_min_val,time6
	// // 				);
	// // 			VecSql.push_back(SqlBuffer);
	// 		}
	// 	}
	// 
	// 	for ( ii =0; ii<m_nTrNum; ii++)
	// 	{
	// 		if (m_pTrResSave[ii].lfo_b_t > 0 && m_pTrResSave[ii].lfo_e_t > 0)
	// 		{
	// 			memset((void *)&tmpLfoCase,0,sizeof(LFO_CASE_WDS_WAMS));
	// 			sprintf(tmpLfoCase.lfo_time,"%s",PT_GetStringOfTimeInterval(m_pTrResSave[ii].hold_time*1000));
	// 			tmpLfoCase.eve_id = m_SysResSave.lfo_b_t;
	// 			tmpLfoCase.win_id = m_SysResSave.lfo_b_t;
	// 			tmpLfoCase.analy_bgn = m_pTrResSave[ii].lfo_b_t;
	// 			tmpLfoCase.analy_end = m_pTrResSave[ii].lfo_e_t;
	// 			tmpLfoCase.lfodev_id = m_pTrResSave[ii].obj_id;
	// 			tmpLfoCase.fac_id = m_pTrResSave[ii].fac_id;
	// 			sprintf(tmpLfoCase.fac_name,"%s",m_pTrResSave[ii].fac_name);
	// 			tmpLfoCase.dev_type = MENU_LFO_DEV_TR;
	// 			tmpLfoCase.max_ampl_rat = m_pTrResSave[ii].max_value.value - m_pTrResSave[ii].min_value.value;
	// 			tmpLfoCase.lfo_max_val = m_pTrResSave[ii].max_value.value;
	// 			tmpLfoCase.lfo_max_val_t = m_pTrResSave[ii].max_value.time;
	// 			tmpLfoCase.lfo_min_val = m_pTrResSave[ii].min_value.value;
	// 			tmpLfoCase.lfo_min_val_t = m_pTrResSave[ii].min_value.time;
	// 			
	// 			idx ++;
	// 			tmpLfoCase.wds_id = PODB_GetDevIdByKeyID(LFO_CASE_WDS_NO_TAB,idx);
	// 			tmpVecLfoCase.push_back(tmpLfoCase);
	// 			//PODB_TableModifyByKey((const char *)&tmpLfoCase.wds_id,LFO_CASE_WDS_CRE_FLD,(const char *)&tmpLfoCase.eve_id,sizeof(LFO_CASE_WDS_CRE)-sizeof(tmpLfoCase.wds_id),LFO_CASE_WDS_NO_TAB);
	// 
	// // 			sprintf(time1,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.eve_id));
	// // 			sprintf(time2,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.win_id));
	// // 			sprintf(time3,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.analy_bgn));
	// // 			sprintf(time4,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.analy_end));
	// // 			sprintf(time5,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.lfo_max_val_t));
	// // 			sprintf(time6,"%s",PT_GetTimeStringBySeconds(tmpLfoCase.lfo_max_val_t));
	// // 			
	// // 			sprintf(SqlBuffer,""LFO_CASE_WDS_CRE_FMT"",
	// // 				"lfo_case_wds",
	// // 				LFO_CASE_WDS_CRE_FLD,
	// // 				time1,time2,time3,time4,
	// // 				LFO_CASE_WDS_CRE_VAL(tmpLfoCase),
	// // 				time5,tmpLfoCase.lfo_min_val,time6
	// // 				);
	// // 			VecSql.push_back(SqlBuffer);
	// 		}
	// 	}
	// 
	// // 	for (ii =0; ii<(int)VecSql.size(); ii++)
	// // 	{
	// // 		//sprintf(m_LogString,"%s",VecSql[ii].c_str());
	// // 		//MessShow(m_LogString,LTDEBUG);
	// // 	}
	// 
	// // 	if (PODB_ModifyTableBySqls(VecSql)<0)
	// // 	{
	// // 		sprintf(m_LogString,"更新LFO实时CASE窗口表失败");
	// // 		MessShow(m_LogString,LTERROR);
	// // 	}
	// // 	else
	// // 	{
	// // 		sprintf(m_LogString,"更新LFO实时CASE窗口表成功");
	// // 		MessShow(m_LogString,LTERROR);
	// // 	}
	// 	
	// 	if (!tmpVecLfoCase.empty())
	// 	{
	// 		PODB_TableWrite((const char *)&tmpVecLfoCase[0].wds_id,tmpVecLfoCase.size() *sizeof(LFO_CASE_WDS_WAMS),LFO_CASE_WDS_NO_TAB);
	// 	}
	// #endif
	// #endif
	// #endif

	return 0;
}

int CLfoAnaObj::UpdateHisShape()
{
	// #ifdef _D5000
	// #ifdef _WAMAP_
	// #ifdef _PODB_SAMPLE_
	// 	// 清空原表内容
	// 	//PODB_TableClearSql("lfo_case_sllshape");
	// 	PODB_TableClear(LFO_CASE_ALLSHAPE_NO_TAB);
	// 
	// 	MessShow("更新LFO实时CASE模态全表");
	// 
	// 	int idx = 0;
	// 	LFO_CASE_ALLSHAPE_WAMS tmpLfoShape;
	// 	vector<LFO_CASE_ALLSHAPE_WAMS> tmpVecLfoShape;
	// 	int ii,jj;
	// // 	vector<string> VecSql;
	// // 	char SqlBuffer[1024];
	// 	for ( ii =0; ii<m_nGnNum; ii++)
	// 	{
	// 		if (m_pGnResSave[ii].lfo_b_t > 0 && m_pGnResSave[ii].lfo_e_t > 0)
	// 		{
	// 			for (jj =0; jj<m_pGnResSave[ii].mode1_num; jj++)
	// 			{
	// 				memset((void *)&tmpLfoShape,0,sizeof(LFO_CASE_ALLSHAPE_WAMS));
	// 				//tmpLfoShape.fact
	// 				tmpLfoShape.dev_id = m_pGnResSave[ii].obj_id;
	// 				sprintf(tmpLfoShape.dev_name,"%s",m_pGnResSave[ii].obj_name);
	// 				tmpLfoShape.dev_type = MENU_LFO_DEV_GEN;
	// 				tmpLfoShape.fac_id = m_pGnResSave[ii].fac_id;
	// 				sprintf(tmpLfoShape.fac_name,"%s",m_pGnResSave[ii].fac_name);
	// 				//tmpLfoShape.area_id
	// 				//tmpLfoShape.clus
	// 				tmpLfoShape.freq = m_pGnResSave[ii].mode1[jj].freq;
	// 				tmpLfoShape.ampl = m_pGnResSave[ii].mode1[jj].ampl;
	// 				tmpLfoShape.damp = m_pGnResSave[ii].mode1[jj].damp;
	// 				tmpLfoShape.iniphas = m_pGnResSave[ii].mode1[jj].phase;
	// 				//tmpLfoShape.aver_val
	// 				tmpLfoShape.eve_id = m_SysResSave.lfo_b_t;
	// 				tmpLfoShape.win_id = m_pGnResSave[ii].mode1[jj].bgn_t;
	// 				tmpLfoShape.analy_bgn = m_pGnResSave[ii].mode1[jj].bgn_t;
	// 				tmpLfoShape.analy_end = m_pGnResSave[ii].mode1[jj].end_t;
	// 
	// 				idx++;
	// 				tmpLfoShape.record_id = PODB_GetDevIdByKeyID(LFO_CASE_ALLSHAPE_NO_TAB,idx);
	// 				tmpVecLfoShape.push_back(tmpLfoShape);
	// 				//PODB_TableModifyByKey((const char *)&tmpLfoShape.record_id,LFO_CASE_ALLSHAPE_CRE_FLD,(const char *)&tmpLfoShape.fact,sizeof(LFO_CASE_ALLSHAPE_CRE)-sizeof(tmpLfoShape.record_id),LFO_CASE_ALLSHAPE_NO_TAB);
	// // 				sprintf(SqlBuffer,""LFO_CASE_ALLSHAPE_CRE_FMT"",
	// // 					"lfo_case_sllshape",
	// // 					LFO_CASE_ALLSHAPE_CRE_FLD,
	// // 					LFO_CASE_ALLSHAPE_CRE_VAL(tmpLfoShape)
	// // 					);
	// // 				VecSql.push_back(SqlBuffer);
	// 			}
	// 		}
	// 	}
	// 
	// 	for ( ii =0; ii<m_nLnNum; ii++)
	// 	{
	// 		if (m_pLnResSave[ii].lfo_b_t > 0 && m_pLnResSave[ii].lfo_e_t > 0)
	// 		{
	// 			for (jj =0; jj<m_pLnResSave[ii].mode1_num; jj++)
	// 			{
	// 				memset((void *)&tmpLfoShape,0,sizeof(LFO_CASE_ALLSHAPE_WAMS));
	// 				//tmpLfoShape.fact
	// 				tmpLfoShape.dev_id = m_pLnResSave[ii].obj_id;
	// 				sprintf(tmpLfoShape.dev_name,"%s",m_pLnResSave[ii].obj_name);
	// 				tmpLfoShape.dev_type = MENU_LFO_DEV_LINE;
	// 				tmpLfoShape.fac_id = m_pLnResSave[ii].fac_id;
	// 				sprintf(tmpLfoShape.fac_name,"%s",m_pLnResSave[ii].fac_name);
	// 				//tmpLfoShape.area_id
	// 				//tmpLfoShape.clus
	// 				tmpLfoShape.freq = m_pLnResSave[ii].mode1[jj].freq;
	// 				tmpLfoShape.ampl = m_pLnResSave[ii].mode1[jj].ampl;
	// 				tmpLfoShape.damp = m_pLnResSave[ii].mode1[jj].damp;
	// 				tmpLfoShape.iniphas = m_pLnResSave[ii].mode1[jj].phase;
	// 				//tmpLfoShape.aver_val
	// 				tmpLfoShape.eve_id = m_SysResSave.lfo_b_t;
	// 				tmpLfoShape.win_id = m_pLnResSave[ii].mode1[jj].bgn_t;
	// 				tmpLfoShape.analy_bgn = m_pLnResSave[ii].mode1[jj].bgn_t;
	// 				tmpLfoShape.analy_end = m_pLnResSave[ii].mode1[jj].end_t;
	// 				
	// 				idx++;
	// 				tmpLfoShape.record_id = PODB_GetDevIdByKeyID(LFO_CASE_ALLSHAPE_NO_TAB,idx);
	// 				tmpVecLfoShape.push_back(tmpLfoShape);
	// 				//PODB_TableModifyByKey((const char *)&tmpLfoShape.record_id,LFO_CASE_ALLSHAPE_CRE_FLD,(const char *)&tmpLfoShape.fact,sizeof(LFO_CASE_ALLSHAPE_CRE)-sizeof(tmpLfoShape.record_id),LFO_CASE_ALLSHAPE_NO_TAB);
	// 
	// 
	// // 				sprintf(SqlBuffer,""LFO_CASE_ALLSHAPE_CRE_FMT"",
	// // 					"lfo_case_sllshape",
	// // 					LFO_CASE_ALLSHAPE_CRE_FLD,
	// // 					LFO_CASE_ALLSHAPE_CRE_VAL(tmpLfoShape)
	// // 					);
	// // 				VecSql.push_back(SqlBuffer);
	// 			}
	// 		}
	// 	}
	// 
	// 	for ( ii =0; ii<m_nTrNum; ii++)
	// 	{
	// 		if (m_pTrResSave[ii].lfo_b_t > 0 && m_pTrResSave[ii].lfo_e_t > 0)
	// 		{
	// 			for (jj =0; jj<m_pTrResSave[ii].mode1_num; jj++)
	// 			{
	// 				memset((void *)&tmpLfoShape,0,sizeof(LFO_CASE_ALLSHAPE_WAMS));
	// 				//tmpLfoShape.fact
	// 				tmpLfoShape.dev_id = m_pTrResSave[ii].obj_id;
	// 				tmpLfoShape.dev_type = MENU_LFO_DEV_TR;
	// 				tmpLfoShape.fac_id = m_pTrResSave[ii].fac_id;
	// 				sprintf(tmpLfoShape.fac_name,"%s",m_pTrResSave[ii].fac_name);
	// 				//tmpLfoShape.area_id
	// 				//tmpLfoShape.clus
	// 				tmpLfoShape.freq = m_pTrResSave[ii].mode1[jj].freq;
	// 				tmpLfoShape.ampl = m_pTrResSave[ii].mode1[jj].ampl;
	// 				tmpLfoShape.damp = m_pTrResSave[ii].mode1[jj].damp;
	// 				tmpLfoShape.iniphas = m_pTrResSave[ii].mode1[jj].phase;
	// 				//tmpLfoShape.aver_val
	// 				tmpLfoShape.eve_id = m_SysResSave.lfo_b_t;
	// 				tmpLfoShape.win_id = m_pTrResSave[ii].mode1[jj].bgn_t;
	// 				tmpLfoShape.analy_bgn = m_pTrResSave[ii].mode1[jj].bgn_t;
	// 				tmpLfoShape.analy_end = m_pTrResSave[ii].mode1[jj].end_t;
	// 				
	// 				idx++;
	// 				tmpLfoShape.record_id = PODB_GetDevIdByKeyID(LFO_CASE_ALLSHAPE_NO_TAB,idx);
	// 				tmpVecLfoShape.push_back(tmpLfoShape);
	// 				//PODB_TableModifyByKey((const char *)&tmpLfoShape.record_id,LFO_CASE_ALLSHAPE_CRE_FLD,(const char *)&tmpLfoShape.fact,sizeof(LFO_CASE_ALLSHAPE_CRE)-sizeof(tmpLfoShape.record_id),LFO_CASE_ALLSHAPE_NO_TAB);
	// 
	// 
	// // 				sprintf(SqlBuffer,""LFO_CASE_ALLSHAPE_CRE_FMT"",
	// // 					"lfo_case_sllshape",
	// // 					LFO_CASE_ALLSHAPE_CRE_FLD,
	// // 					LFO_CASE_ALLSHAPE_CRE_VAL(tmpLfoShape)
	// // 					);
	// // 				VecSql.push_back(SqlBuffer);
	// 			}
	// 		}
	// 	}
	// 	
	// // 	for (ii =0; ii<(int)VecSql.size(); ii++)
	// // 	{
	// // 		sprintf(m_LogString,"%s",VecSql[ii].c_str());
	// // 		MessShow(m_LogString,LTDEBUG);
	// // 	}
	// 
	// // 	if (PODB_ModifyTableBySqls(VecSql)<0)
	// // 	{
	// // 		sprintf(m_LogString,"更新LFO实时CASE模态全表失败");
	// // 		MessShow(m_LogString,LTERROR);
	// // 	}
	// // 	else
	// // 	{
	// // 		sprintf(m_LogString,"更新LFO实时CASE模态全表成功");
	// // 		MessShow(m_LogString,LTERROR);
	// // 	}
	// 
	// 	if (!tmpVecLfoShape.empty())
	// 	{
	// 		PODB_TableWrite((const char *)&tmpVecLfoShape[0].record_id,tmpVecLfoShape.size() *sizeof(LFO_CASE_ALLSHAPE_WAMS),LFO_CASE_ALLSHAPE_NO_TAB);
	// 	}
	// #endif
	// #endif
	// #endif
	// 
	return 0;
}


int CLfoAnaObj::SampleSsoPara()
{
#ifdef _D5000
#ifdef _WAMAP_
#ifdef _PODB_SAMPLE_
	sprintf(m_LogString, "%s", "SSO参数表采样");
	MessShow(m_LogString, LTINFO);

	His_SSO_HIS_DECI tmpLfoHis;
	memset((void*)&tmpLfoHis, 0, sizeof(His_SSO_HIS_DECI));
	tmpLfoHis.dsource = m_DispCtl.dsource;
	tmpLfoHis.time_leng = m_LfoPara.time_leng;
	tmpLfoHis.int_time = m_LfoPara.int_time;
	tmpLfoHis.freq_min = m_LfoPara.freq_min;
	tmpLfoHis.freq_max = m_LfoPara.freq_max;
	tmpLfoHis.sso_jud_num = m_LfoPara.sso_jud_num;
	strcpy(tmpLfoHis.stat_ctx, PT_GetChineseTimeStringBySeconds(m_SysMon.sso_bgn));
	strcat(tmpLfoHis.stat_ctx, "检测到次同步振荡");
	sprintf(tmpLfoHis.case_des, "%s", tmpLfoHis.stat_ctx);
	tmpLfoHis.sso_bgn = m_SysMon.sso_bgn;
	tmpLfoHis.sso_end = m_SysMon.sso_end;
	sprintf(tmpLfoHis.sso_time, "%s", m_SysMon.sso_time);
	tmpLfoHis.warn_dev_id = m_SysMon.warn_dev_id;
	sprintf(tmpLfoHis.warn_dev_name, "%s", m_SysMon.warn_dev_name);
	tmpLfoHis.warn_freq = m_SysMon.warn_freq;
	tmpLfoHis.sso_ln_num = m_SysMon.sso_ln_num;
	tmpLfoHis.sso_gn_num = m_SysMon.sso_gn_num;
	tmpLfoHis.sso_tr_num = m_SysMon.sso_tr_num;
	tmpLfoHis.is_warn = m_DispCtl.is_warn;
	tmpLfoHis.is_samp = m_DispCtl.is_samp;
	tmpLfoHis.gn_swn_ampl = m_LfoPara.gn_swn_ampl;
	tmpLfoHis.gn_ewn_ampl = m_LfoPara.gn_ewn_ampl;
	tmpLfoHis.ln_swn_ampl1 = m_LfoPara.ln_swn_ampl1;
	tmpLfoHis.ln_swn_ampl2 = m_LfoPara.ln_swn_ampl2;
	tmpLfoHis.ln_swn_ampl3 = m_LfoPara.ln_swn_ampl3;
	tmpLfoHis.ln_ewn_ampl1 = m_LfoPara.ln_ewn_ampl1;
	tmpLfoHis.ln_ewn_ampl2 = m_LfoPara.ln_ewn_ampl2;
	tmpLfoHis.ln_ewn_ampl3 = m_LfoPara.ln_ewn_ampl3;
	tmpLfoHis.tr_swn_ampl1 = m_LfoPara.tr_swn_ampl1;
	tmpLfoHis.tr_swn_ampl2 = m_LfoPara.tr_swn_ampl2;
	tmpLfoHis.tr_swn_ampl3 = m_LfoPara.tr_swn_ampl3;
	tmpLfoHis.tr_ewn_ampl1 = m_LfoPara.tr_ewn_ampl1;
	tmpLfoHis.tr_ewn_ampl2 = m_LfoPara.tr_ewn_ampl2;
	tmpLfoHis.tr_ewn_ampl3 = m_LfoPara.tr_ewn_ampl3;

	vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
	TRIGGER_SAMPLE_DEFINE tmpTrigger;
	PODB_ClearSample(tmpTrigger);
	tmpTrigger.appno = g_appNo;
	tmpTrigger.mode = 0;
	tmpTrigger.occur_time = PT_GetNowSeconds();
	tmpTrigger.sampleno = HIS_SSO_HIS_DECI_NO;
	tmpTrigger.tableno = SSO_PARA_NO_TAB;
	PODB_SmpBaseINT(tmpTrigger, 1, tmpLfoHis.record_id);
	PODB_SmpBaseINT(tmpTrigger, 3, tmpLfoHis.dsource);
	PODB_SmpBaseINT(tmpTrigger, 8, tmpLfoHis.time_leng);
	PODB_SmpBaseINT(tmpTrigger, 9, tmpLfoHis.int_time);
	PODB_SmpBaseFLT(tmpTrigger, 10, tmpLfoHis.freq_min);
	PODB_SmpBaseFLT(tmpTrigger, 11, tmpLfoHis.freq_max);
	PODB_SmpBaseINT(tmpTrigger, 12, tmpLfoHis.sso_jud_num);
	PODB_SmpBaseSTR(tmpTrigger, 18, tmpLfoHis.stat_ctx);
	PODB_SmpBaseFLT(tmpTrigger, 22, tmpLfoHis.mon_ampl);
	PODB_SmpBaseFLT(tmpTrigger, 23, tmpLfoHis.mon_freq);
	PODB_SmpBaseFLT(tmpTrigger, 44, tmpLfoHis.lead_ampl);
	PODB_SmpBaseFLT(tmpTrigger, 46, tmpLfoHis.lead_freq);
	PODB_SmpBaseSTR(tmpTrigger, 70, tmpLfoHis.case_des);
	PODB_SmpBaseINT(tmpTrigger, 71, tmpLfoHis.sso_bgn);
	PODB_SmpBaseINT(tmpTrigger, 72, tmpLfoHis.sso_end);
	PODB_SmpBaseSTR(tmpTrigger, 73, tmpLfoHis.sso_time);
	PODB_SmpBaseINT(tmpTrigger, 74, tmpLfoHis.warn_dev_id);
	PODB_SmpBaseSTR(tmpTrigger, 75, tmpLfoHis.warn_dev_name);
	PODB_SmpBaseFLT(tmpTrigger, 76, tmpLfoHis.warn_freq);
	PODB_SmpBaseINT(tmpTrigger, 77, tmpLfoHis.sso_ln_num);
	PODB_SmpBaseINT(tmpTrigger, 78, tmpLfoHis.sso_gn_num);
	PODB_SmpBaseINT(tmpTrigger, 79, tmpLfoHis.sso_tr_num);
	PODB_SmpBaseINT(tmpTrigger, 80, tmpLfoHis.is_warn);
	PODB_SmpBaseINT(tmpTrigger, 81, tmpLfoHis.is_samp);
	PODB_SmpBaseFLT(tmpTrigger, 87, tmpLfoHis.gn_swn_ampl);
	PODB_SmpBaseFLT(tmpTrigger, 88, tmpLfoHis.gn_ewn_ampl);
	PODB_SmpBaseFLT(tmpTrigger, 89, tmpLfoHis.ln_swn_ampl1);
	PODB_SmpBaseFLT(tmpTrigger, 90, tmpLfoHis.ln_swn_ampl2);
	PODB_SmpBaseFLT(tmpTrigger, 91, tmpLfoHis.ln_swn_ampl3);
	PODB_SmpBaseFLT(tmpTrigger, 92, tmpLfoHis.ln_ewn_ampl1);
	PODB_SmpBaseFLT(tmpTrigger, 93, tmpLfoHis.ln_ewn_ampl2);
	PODB_SmpBaseFLT(tmpTrigger, 94, tmpLfoHis.ln_ewn_ampl3);
	PODB_SmpBaseFLT(tmpTrigger, 95, tmpLfoHis.tr_swn_ampl1);
	PODB_SmpBaseFLT(tmpTrigger, 96, tmpLfoHis.tr_swn_ampl2);
	PODB_SmpBaseFLT(tmpTrigger, 97, tmpLfoHis.tr_swn_ampl3);
	PODB_SmpBaseFLT(tmpTrigger, 98, tmpLfoHis.tr_ewn_ampl1);
	PODB_SmpBaseFLT(tmpTrigger, 99, tmpLfoHis.tr_ewn_ampl2);
	PODB_SmpBaseFLT(tmpTrigger, 100, tmpLfoHis.tr_ewn_ampl3);
	PODB_SmpBaseFLT(tmpTrigger, 101, tmpLfoHis.gn_stor_ampl);
	PODB_SmpBaseFLT(tmpTrigger, 102, tmpLfoHis.ln_stor_ampl);
	PODB_SmpBaseFLT(tmpTrigger, 103, tmpLfoHis.tr_stor_ampl);
	PODB_SmpBaseFLT(tmpTrigger, 104, tmpLfoHis.sm_md_freq);

	tmpVecTrigger.push_back(tmpTrigger);

#ifdef _PODB_SAMPLE_
	PODB_TriggerSample(tmpVecTrigger);
#endif
#endif
#endif
#endif
	return 0;
}

int CLfoAnaObj::SampleHisDev()
{
#ifdef _D5000
#ifdef _WAMAP_
#ifdef _PODB_SAMPLE_
	sprintf(m_LogString, "%s", "次同步振荡设备表采样");
	MessShow(m_LogString, LTINFO);

	His_SSO_HIS_DEV tmpHisDev;
	vector<His_SSO_HIS_DEV> tmpvecHisDev;
	int nIdx = 1;

	for (int ii = 0; ii < m_nDevEveWarn; ii++)
	{
		memset((void*)&tmpHisDev, 0, sizeof(His_SSO_HIS_DEV));
		tmpHisDev.record_id = PODB_GetDevIdByKeyID(SSO_DEV_NO_TAB, nIdx);
		tmpHisDev.eve_id = m_SysMon.sso_bgn;
		tmpHisDev.sso_bgn = m_pDevEveWarn[ii].sso_bgn;
		tmpHisDev.sso_end = m_pDevEveWarn[ii].sso_end;
		tmpHisDev.freq = m_pDevEveWarn[ii].lead_freq;
		tmpHisDev.ampl = m_pDevEveWarn[ii].lead_ampl;
		tmpHisDev.dev_id = m_pDevEveWarn[ii].dev_id;
		tmpHisDev.dev_facid = m_pDevEveWarn[ii].fac_id;
		sprintf(tmpHisDev.dev_name, "%s", m_pDevEveWarn[ii].dev_name);
		sprintf(tmpHisDev.dev_facname, "%s", m_pDevEveWarn[ii].fac_name);
		sprintf(tmpHisDev.sso_time, "%s", m_pDevEveWarn[ii].sso_time);

		tmpvecHisDev.push_back(tmpHisDev);
		nIdx++;
	}

	sprintf(m_LogString, "次同步振荡设备表采样记录数[%d]", tmpvecHisDev.size());
	MessShow(m_LogString, LTINFO);

	int nOccurTime = PT_GetNowSeconds();
	vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
	TRIGGER_SAMPLE_DEFINE tmpTrigger;


	for (ii = 0; ii < (int)tmpvecHisDev.size(); ii++)
	{
		PODB_ClearSample(tmpTrigger);
		tmpTrigger.appno = g_appNo;
		tmpTrigger.mode = 0;
		tmpTrigger.occur_time = nOccurTime;
		tmpTrigger.sampleno = HIS_SSO_HIS_DEV_NO;
		tmpTrigger.tableno = SSO_DEV_NO_TAB;

		PODB_SmpBaseINT(tmpTrigger, 1, tmpvecHisDev[ii].record_id);  // 记录ID
		PODB_SmpBaseINT(tmpTrigger, 2, tmpvecHisDev[ii].eve_id);  // 事件ID
		PODB_SmpBaseINT(tmpTrigger, 3, tmpvecHisDev[ii].sso_bgn);  // 振荡开始时间
		PODB_SmpBaseINT(tmpTrigger, 4, tmpvecHisDev[ii].sso_end);  // 振荡结束时间
		PODB_SmpBaseFLT(tmpTrigger, 5, tmpvecHisDev[ii].freq);  // 频率
		PODB_SmpBaseFLT(tmpTrigger, 6, tmpvecHisDev[ii].ampl);  // 振幅
		PODB_SmpBaseINT(tmpTrigger, 7, tmpvecHisDev[ii].dev_id);  // 振荡设备ID
		PODB_SmpBaseINT(tmpTrigger, 8, tmpvecHisDev[ii].dev_facid);  // 振荡厂站ID
		PODB_SmpBaseSTR(tmpTrigger, 9, tmpvecHisDev[ii].dev_name); // 振荡设备名称
		PODB_SmpBaseSTR(tmpTrigger, 10, tmpvecHisDev[ii].sso_time); // 振荡持续时间
		PODB_SmpBaseSTR(tmpTrigger, 11, tmpvecHisDev[ii].dev_facname); // 设备厂站名称

		tmpVecTrigger.push_back(tmpTrigger);
	}


#ifdef _PODB_SAMPLE_
	PODB_TriggerSample(tmpVecTrigger);
#endif
#endif
#endif
#endif
	return 0;
}

int CLfoAnaObj::SampleHisCase()
{
#ifdef _D5000
#ifdef _WAMAP_
#ifdef _PODB_SAMPLE_
	sprintf(m_LogString, "%s", "次同步振荡实时CASE窗口表采样");
	MessShow(m_LogString, LTINFO);
	His_SSO_HIS_CASE_WDS tmpHisCase;
	vector<His_SSO_HIS_CASE_WDS> tmpVecHisCase;

	int ii, nIdx = 1;
	for (ii = 0; ii < m_nGnWarn; ii++)
	{
		memset((void*)&tmpHisCase, 0, sizeof(His_SSO_HIS_CASE_WDS));
		tmpHisCase.record_id = PODB_GetDevIdByKeyID(SSO_CASE_WDS_NO_TAB, nIdx);
		tmpHisCase.eve_id = m_SysMon.sso_bgn;
		//tmpHisCase.wds_id = ;
		tmpHisCase.wds_bgn = m_CalcStartTime;
		tmpHisCase.wds_end = m_CurGpsTime;
		tmpHisCase.dev_id = m_pGnWarn[ii].gn_id;
		sprintf(tmpHisCase.dev_name, "%s", m_pGnWarn[ii].gn_name);
		tmpHisCase.freq = m_pGnWarn[ii].lead_freq;
		tmpHisCase.ampl = m_pGnWarn[ii].lead_ampl;

		tmpVecHisCase.push_back(tmpHisCase);
		nIdx++;
	}

	for (ii = 0; ii < m_nLnWarn; ii++)
	{
		memset((void*)&tmpHisCase, 0, sizeof(His_SSO_HIS_CASE_WDS));
		tmpHisCase.record_id = PODB_GetDevIdByKeyID(SSO_CASE_WDS_NO_TAB, nIdx);
		tmpHisCase.eve_id = m_SysMon.sso_bgn;
		//tmpHisCase.wds_id = ;
		tmpHisCase.wds_bgn = m_CalcStartTime;
		tmpHisCase.wds_end = m_CurGpsTime;
		tmpHisCase.dev_id = m_pLnWarn[ii].ln_id;
		sprintf(tmpHisCase.dev_name, "%s", m_pLnWarn[ii].ln_name);
		tmpHisCase.freq = m_pLnWarn[ii].lead_freq;
		tmpHisCase.ampl = m_pLnWarn[ii].lead_ampl;

		tmpVecHisCase.push_back(tmpHisCase);
		nIdx++;
	}

	for (ii = 0; ii < m_nTrWarn; ii++)
	{
		if (m_pTrResSave[ii].lfo_b_t > 0 && m_pTrResSave[ii].lfo_e_t > 0)
		{
			memset((void*)&tmpHisCase, 0, sizeof(His_SSO_HIS_CASE_WDS));
			tmpHisCase.record_id = PODB_GetDevIdByKeyID(SSO_CASE_WDS_NO_TAB, nIdx);
			tmpHisCase.eve_id = m_SysMon.sso_bgn;
			//tmpHisCase.wds_id = ;
			tmpHisCase.wds_bgn = m_CalcStartTime;
			tmpHisCase.wds_end = m_CurGpsTime;
			tmpHisCase.dev_id = m_pTrWarn[ii].tr_id;
			sprintf(tmpHisCase.dev_name, "%s", m_pTrWarn[ii].tr_name);
			tmpHisCase.freq = m_pTrWarn[ii].lead_freq;
			tmpHisCase.ampl = m_pTrWarn[ii].lead_ampl;

			tmpVecHisCase.push_back(tmpHisCase);
			nIdx++;
		}
	}

	int nOccurTime = PT_GetNowSeconds();
	vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
	TRIGGER_SAMPLE_DEFINE tmpTrigger;
	for (ii = 0; ii < (int)tmpVecHisCase.size(); ii++)
	{
		PODB_ClearSample(tmpTrigger);
		tmpTrigger.appno = g_appNo;
		tmpTrigger.mode = 0;
		tmpTrigger.occur_time = nOccurTime;
		tmpTrigger.sampleno = HIS_SSO_HIS_CASE_WDS_NO;
		tmpTrigger.tableno = SSO_CASE_WDS_NO_TAB;

		PODB_SmpBaseINT(tmpTrigger, 1, tmpVecHisCase[ii].record_id);  // 窗口ID
		PODB_SmpBaseINT(tmpTrigger, 2, tmpVecHisCase[ii].wds_id);  // 窗口ID
		PODB_SmpBaseINT(tmpTrigger, 3, tmpVecHisCase[ii].eve_id);  // 事件ID
		PODB_SmpBaseINT(tmpTrigger, 4, tmpVecHisCase[ii].wds_bgn);  // 事件ID
		PODB_SmpBaseINT(tmpTrigger, 5, tmpVecHisCase[ii].wds_end);  // 事件ID
		PODB_SmpBaseFLT(tmpTrigger, 6, tmpVecHisCase[ii].ampl);  // 振幅
		PODB_SmpBaseFLT(tmpTrigger, 7, tmpVecHisCase[ii].freq);  // 频率
		PODB_SmpBaseINT(tmpTrigger, 9, tmpVecHisCase[ii].dev_id);  // 振荡设备ID
		PODB_SmpBaseSTR(tmpTrigger, 10, tmpVecHisCase[ii].dev_name); // 振荡设备名称

		tmpVecTrigger.push_back(tmpTrigger);
	}

#ifdef _PODB_SAMPLE_
	PODB_TriggerSample(tmpVecTrigger);
#endif
#endif
#endif
#endif
	return 0;

}

int CLfoAnaObj::SampleHisShape()
{
	/*
	#ifdef _D5000
	#ifdef _WAMAP_
	#ifdef _PODB_SAMPLE_
		sprintf(m_LogString,"%s","低频振荡实时CASE模态全表采样");
		MessShow(m_LogString,LTINFO);
		His_LFO_HIS_SLLSHAPE tmpHisShape;
		vector<His_LFO_HIS_SLLSHAPE> tmpVecHisShape;

		int ii,jj,nIdx =1;
		for ( ii =0; ii<m_nGnNum; ii++)
		{
			if (m_pGnResSave[ii].lfo_b_t > 0 && m_pGnResSave[ii].lfo_e_t > 0)
			{
				for (jj =0; jj<m_pGnResSave[ii].mode1_num; jj++)
				{
					memset((void *)&tmpHisShape,0,sizeof(His_LFO_HIS_SLLSHAPE));
					tmpHisShape.record_id = PODB_GetDevIdByKeyID(LFO_CASE_ALLSHAPE_NO_TAB,nIdx);
					//tmpHisShape.fact
					tmpHisShape.dev_id = m_pGnResSave[ii].obj_id;
					sprintf(tmpHisShape.dev_name,"%s",m_pGnResSave[ii].obj_name);
					tmpHisShape.dev_type = MENU_LFO_DEV_GEN;
					tmpHisShape.fac_id = m_pGnResSave[ii].fac_id;
					//tmpHisShape.area_id = m_pGnResSave[ii]
					//tmpHisShape.clus
					tmpHisShape.freq = m_pGnResSave[ii].mode1[jj].freq;
					tmpHisShape.ampl = m_pGnResSave[ii].mode1[jj].ampl;
					tmpHisShape.damp = m_pGnResSave[ii].mode1[jj].damp;
					//tmpHisShape.engy = m_pGnResSave[ii].mode1[jj].engy;
					//tmpHisShape.aver_val
					//freq_max
					// freq_min
					tmpHisShape.eve_id = m_SysResSave.lfo_b_t;
					tmpHisShape.win_id = m_pGnResSave[ii].mode1[jj].bgn_t;
					tmpHisShape.analy_bgn = m_pGnResSave[ii].mode1[jj].bgn_t;
					tmpHisShape.analy_end = m_pGnResSave[ii].mode1[jj].end_t;
					tmpHisShape.iniphas = m_pGnResSave[ii].mode1[jj].phase;
					sprintf(tmpHisShape.fac_name,"%s",m_pGnResSave[ii].fac_name);
					//sprintf(tmpHisShape.area_name)

					tmpVecHisShape.push_back(tmpHisShape);
					nIdx ++;
				}
			}
		}

		for ( ii =0; ii<m_nLnNum; ii++)
		{
			if (m_pLnResSave[ii].lfo_b_t > 0 && m_pLnResSave[ii].lfo_e_t > 0)
			{
				for (jj =0; jj<m_pLnResSave[ii].mode1_num; jj++)
				{
					memset((void *)&tmpHisShape,0,sizeof(His_LFO_HIS_SLLSHAPE));
					tmpHisShape.record_id = PODB_GetDevIdByKeyID(LFO_CASE_ALLSHAPE_NO_TAB,nIdx);
					//tmpHisShape.fact
					tmpHisShape.dev_id = m_pLnResSave[ii].obj_id;
					sprintf(tmpHisShape.dev_name,"%s",m_pLnResSave[ii].obj_name);
					tmpHisShape.dev_type = MENU_LFO_DEV_GEN;
					tmpHisShape.fac_id = m_pLnResSave[ii].fac_id;
					//tmpHisShape.area_id = m_pGnResSave[ii]
					//tmpHisShape.clus
					tmpHisShape.freq = m_pLnResSave[ii].mode1[jj].freq;
					tmpHisShape.ampl = m_pLnResSave[ii].mode1[jj].ampl;
					tmpHisShape.damp = m_pLnResSave[ii].mode1[jj].damp;
					//tmpHisShape.engy = m_pGnResSave[ii].mode1[jj].engy;
					//tmpHisShape.aver_val
					//freq_max
					// freq_min
					tmpHisShape.eve_id = m_SysResSave.lfo_b_t;
					tmpHisShape.win_id = m_pLnResSave[ii].mode1[jj].bgn_t;
					tmpHisShape.analy_bgn = m_pLnResSave[ii].mode1[jj].bgn_t;
					tmpHisShape.analy_end = m_pLnResSave[ii].mode1[jj].end_t;
					tmpHisShape.iniphas = m_pLnResSave[ii].mode1[jj].phase;
					sprintf(tmpHisShape.fac_name,"%s",m_pLnResSave[ii].fac_name);
					//sprintf(tmpHisShape.area_name)

					tmpVecHisShape.push_back(tmpHisShape);
					nIdx ++;
				}
			}
		}

		for ( ii =0; ii<m_nTrNum; ii++)
		{
			if (m_pTrResSave[ii].lfo_b_t > 0 && m_pTrResSave[ii].lfo_e_t > 0)
			{
				for (jj =0; jj<m_pTrResSave[ii].mode1_num; jj++)
				{
					memset((void *)&tmpHisShape,0,sizeof(His_LFO_HIS_SLLSHAPE));
					tmpHisShape.record_id = PODB_GetDevIdByKeyID(LFO_CASE_ALLSHAPE_NO_TAB,nIdx);
					//tmpHisShape.fact
					tmpHisShape.dev_id = m_pTrResSave[ii].obj_id;
					sprintf(tmpHisShape.dev_name,"%s",m_pTrResSave[ii].obj_name);
					tmpHisShape.dev_type = MENU_LFO_DEV_GEN;
					tmpHisShape.fac_id = m_pTrResSave[ii].fac_id;
					//tmpHisShape.area_id = m_pGnResSave[ii]
					//tmpHisShape.clus
					tmpHisShape.freq = m_pTrResSave[ii].mode1[jj].freq;
					tmpHisShape.ampl = m_pTrResSave[ii].mode1[jj].ampl;
					tmpHisShape.damp = m_pTrResSave[ii].mode1[jj].damp;
					//tmpHisShape.engy = m_pGnResSave[ii].mode1[jj].engy;
					//tmpHisShape.aver_val
					//freq_max
					// freq_min
					tmpHisShape.eve_id = m_SysResSave.lfo_b_t;
					tmpHisShape.win_id = m_pTrResSave[ii].mode1[jj].bgn_t;
					tmpHisShape.analy_bgn = m_pTrResSave[ii].mode1[jj].bgn_t;
					tmpHisShape.analy_end = m_pTrResSave[ii].mode1[jj].end_t;
					tmpHisShape.iniphas = m_pTrResSave[ii].mode1[jj].phase;
					sprintf(tmpHisShape.fac_name,"%s",m_pTrResSave[ii].fac_name);
					//sprintf(tmpHisShape.area_name)

					tmpVecHisShape.push_back(tmpHisShape);
					nIdx ++;
				}
			}
		}

		int nOccurTime = PT_GetNowSeconds();
		vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
		TRIGGER_SAMPLE_DEFINE tmpTrigger;
		for (ii =0; ii< (int)tmpVecHisShape.size(); ii++)
		{
			PODB_ClearSample(tmpTrigger);
			tmpTrigger.appno = g_appNo;
			tmpTrigger.mode = 0;
			tmpTrigger.occur_time = nOccurTime;
			tmpTrigger.sampleno = HIS_LFO_HIS_SLLSHAPE_NO;
			tmpTrigger.tableno = LFO_CASE_ALLSHAPE_NO_TAB;

			PODB_SmpBaseINT(tmpTrigger,1,tmpVecHisShape[ii].record_id);  // 记录号
			PODB_SmpBaseFLT(tmpTrigger,10,tmpVecHisShape[ii].fact);  //  参与因子
			PODB_SmpBaseINT(tmpTrigger,12,tmpVecHisShape[ii].dev_id);  // 设备ID
			PODB_SmpBaseSTR(tmpTrigger,13,tmpVecHisShape[ii].dev_name);  // 设备名称
			PODB_SmpBaseINT(tmpTrigger,14,tmpVecHisShape[ii].dev_type);  // 设备类型
			PODB_SmpBaseINT(tmpTrigger,15,tmpVecHisShape[ii].fac_id);  // 所属厂站
			PODB_SmpBaseINT(tmpTrigger,16,tmpVecHisShape[ii].area_id);  // 所属区域
			PODB_SmpBaseINT(tmpTrigger,19,tmpVecHisShape[ii].clus);  // 所属群
			PODB_SmpBaseFLT(tmpTrigger,20,tmpVecHisShape[ii].freq);  // 频率
			PODB_SmpBaseFLT(tmpTrigger,21,tmpVecHisShape[ii].damp);  // 阻尼比
			PODB_SmpBaseFLT(tmpTrigger,22,tmpVecHisShape[ii].ampl);  // 振幅
			PODB_SmpBaseFLT(tmpTrigger,23,tmpVecHisShape[ii].engy);  // 能量
			PODB_SmpBaseFLT(tmpTrigger,24,tmpVecHisShape[ii].aver_val); // 平均值
			PODB_SmpBaseFLT(tmpTrigger,25,tmpVecHisShape[ii].max_ampl);  // 最大幅值
			PODB_SmpBaseFLT(tmpTrigger,28,tmpVecHisShape[ii].freq_max);  // 频率上限
			PODB_SmpBaseFLT(tmpTrigger,29,tmpVecHisShape[ii].freq_min);  // 频率下限
			PODB_SmpBaseINT(tmpTrigger,30,tmpVecHisShape[ii].eve_id);  // 事件ID
			PODB_SmpBaseINT(tmpTrigger,31,tmpVecHisShape[ii].win_id);  // 窗口号
			PODB_SmpBaseINT(tmpTrigger,32,tmpVecHisShape[ii].analy_bgn);  // 分析起始时间
			PODB_SmpBaseINT(tmpTrigger,33,tmpVecHisShape[ii].analy_end);  // 分析截止时间
			PODB_SmpBaseFLT(tmpTrigger,35,tmpVecHisShape[ii].iniphas);  // 初相
			PODB_SmpBaseSTR(tmpTrigger,36,tmpVecHisShape[ii].fac_name);  // 厂站名称
			PODB_SmpBaseSTR(tmpTrigger,37,tmpVecHisShape[ii].area_name);  // 区域名称

			tmpVecTrigger.push_back(tmpTrigger);
		}

	#ifdef _PODB_SAMPLE_
		PODB_TriggerSample(tmpVecTrigger);
	#endif

	#endif
	#endif
	#endif
		return 0;

	*/
}

int CLfoAnaObj::SaveCaseFile()
{
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
	// 在res目录下面生成带时标的标志文件,然后拷贝到统一目录下
	LFO_CASE_FILE_STRUCT tmpCase;
	sprintf(tmpCase.version, "%s", "v1.0.0");
	strcpy(tmpCase.time, PT_GetChineseTimeStringBySeconds(m_SysMon.sso_bgn));
	// 	sprintf(tmpCase.system,"%s","次同步振荡在线监视");
	tmpCase.eve_id = m_SysMon.sso_bgn;
	tmpCase.start_time = m_SysMon.sso_bgn;
	tmpCase.end_time = m_SysMon.sso_end;
	tmpCase.hold_time = m_SysMon.sso_end - m_SysMon.sso_bgn;
	// 	tmpCase.Data_Save_start_time = tmpCase.start_time - m_LfoPara.rec_num*(m_LfoPara.time_leng-m_LfoPara.int_time);
	// 	tmpCase.Data_Save_end_time = tmpCase.end_time + m_LfoPara.rec_num*m_LfoPara.time_leng;
	if ((tmpCase.start_time - tmpCase.Data_Save_start_time) < 0.0001)
	{
		tmpCase.Data_Save_start_time = tmpCase.start_time;
	}
	if ((tmpCase.end_time - tmpCase.Data_Save_end_time) > 0.0001)
	{
		tmpCase.Data_Save_end_time = tmpCase.end_time;
	}
	strcpy(tmpCase.case_des, PT_GetChineseTimeStringBySeconds(m_SysMon.sso_bgn));
	strcat(tmpCase.case_des, "检测到次同步振荡");
	tmpCase.warn_dev_id = m_SysMon.warn_dev_id;
	sprintf(tmpCase.warn_dev_name, "%s", m_SysMon.warn_dev_name);
	tmpCase.warn_freq = m_SysMon.warn_freq;
	tmpCase.max_ampl = m_SysMon.mon_ampl;

	int ii, jj, table_no;
	// 	LFO_CASE_FILE_DEV tmpdev;
	// 	vector<FES_YC_TAB_CRE> tmpVecYc;
	// #ifdef _WAMAP_
	// 	PODB_GetFesYcInfo(tmpVecYc);
	// #endif
	// 	for (ii =0; ii<(int)tmpVecYc.size(); ii++)
	// 	{
	// 		if (tmpVecYc[ii].file_no < 0 || tmpVecYc[ii].col_no < 0)
	// 		{
	// 			continue;
	// 		}
	// 		memset((void *)&tmpdev,0,sizeof(LFO_CASE_FILE_DEV));
	// 		tmpdev.fac_id = tmpVecYc[ii].fac_id;
	// 		tmpdev.dev_id = PODB_GetRecordId(tmpVecYc[ii].yc_id);
	// 		tmpdev.column_id = PODB_GetColumnId(tmpVecYc[ii].yc_id);
	// 		tmpdev.yc_id = tmpVecYc[ii].yc_id;
	// 		tmpdev.file_no = tmpVecYc[ii].file_no;
	// 		tmpdev.col_no = tmpVecYc[ii].col_no;
	// 		table_no = PODB_GetTabelNoByDevId(tmpdev.dev_id);
	// 		if (table_no == BS_DEVICE_NO_TAB || table_no == FAC_INFO_NO_TAB)
	// 		{
	// 			tmpdev.is_warn = 0;
	// 		}
	// 		else if (table_no == GN_DEVICE_NO_TAB)
	// 		{
	// 			tmpdev.dev_id1 = tmpdev.dev_id;
	// 			tmpdev.is_warn = 0;
	// 			if (tmpdev.dev_id1 > 0)
	// 			{
	// 				for (jj =0; jj<m_nGnNum; jj++)
	// 				{
	// 					if (m_pGnResSave[jj].lfo_b_t > 0 && m_pGnResSave[jj].lfo_e_t > 0 )
	// 					{
	// 						if (tmpdev.dev_id1 == m_pGnResSave[jj].obj_id)
	// 						{
	// 							tmpdev.is_warn = 1;
	// 							break;
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 		else if (table_no == ACLN_DOT_NO_TAB)
	// 		{
	// 			// 根据线路端点查找线路ID
	// 			for (jj =0; jj<m_nLnNum; jj++)
	// 			{
	// 				if (tmpdev.dev_id == m_pLnInf[jj].dot_id_1 || tmpdev.dev_id == m_pLnInf[jj].dot_id_2)
	// 				{
	// 					tmpdev.dev_id1 = m_pLnInf[jj].acln_id;
	// 					break;
	// 				}
	// 			}
	// 			tmpdev.is_warn = 0;
	// 			if (tmpdev.dev_id1 > 0)
	// 			{
	// 				for (jj =0; jj<m_nLnNum; jj++)
	// 				{
	// 					if (m_pLnResSave[jj].lfo_b_t > 0 && m_pLnResSave[jj].lfo_e_t > 0 )
	// 					{
	// 						if (tmpdev.dev_id1 == m_pLnResSave[jj].obj_id)
	// 						{
	// 							tmpdev.is_warn = 1;
	// 							break;
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 		else if (table_no == TR_DEVICE_NO_TAB  || table_no == TRWD_DEVICE_NO_TAB)
	// 		{
	// 			// 根据变压器绕组查找变压器ID
	// 			for (jj =0; jj<m_nTrNum; jj++)
	// 			{
	// 				if (tmpdev.dev_id == m_pTrInf[jj].high_wind_id || tmpdev.dev_id == m_pTrInf[jj].mid_wind_id
	// 					|| tmpdev.dev_id == m_pTrInf[jj].low_wind_id)
	// 				{
	// 					tmpdev.dev_id1 = m_pTrInf[jj].tr_id;
	// 					break;
	// 				}
	// 			}
	// 
	// 			tmpdev.is_warn = 0;
	// 			if (tmpdev.dev_id1 > 0)
	// 			{
	// 				for (jj =0; jj<m_nTrNum; jj++)
	// 				{
	// 					if (m_pTrResSave[jj].lfo_b_t > 0 && m_pTrResSave[jj].lfo_e_t > 0 )
	// 					{
	// 						if (tmpdev.dev_id1 == m_pTrResSave[jj].obj_id)
	// 						{
	// 							tmpdev.is_warn = 1;
	// 							break;
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 		else
	// 		{
	// 			//sprintf(m_LogString,"根据表号判断设备类型错误[%d]",table_no);
	// 			//MessShow(m_LogString,LTERROR);
	// 		}
	// 		
	// 		tmpCase.vecdev.push_back(tmpdev);
	// 	}

	fstream fs;
	RTDB_TIME start_time, end_time;
	char filename[MAXBUF];
	char filenamebas[MAXBUF];
	char Examplefbas[MAXBUF];
	sprintf(filename, "sso_casefile_%s.txt", PT_GetNoSpaceMarkTimeStringBySeconds(PT_GetNowSeconds()));
#ifdef _WINDOWS32
	sprintf(filenamebas, "%s\\res\\%s", m_RunInfDir, filename);
	sprintf(Examplefbas, "%s\\data", m_RunInfDir);
#else
	sprintf(filenamebas, "%s/res/%s", m_RunInfDir, filename);
	sprintf(Examplefbas, "%s/data", m_RunInfDir);
#endif
	fs.open(filenamebas, ios::out);
	if (fs.fail())
	{
		sprintf(m_LogString, "案例标志文件生成失败[%s]", filenamebas);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	fs << "[概要信息]" << endl;
	fs << "Version = " << tmpCase.version << ";" << endl;
	fs << "Time = " << tmpCase.time << ";" << endl;
	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	{
		fs << "案例类型 = 0" << ";" << endl;
		fs << "演示案例目录 = " << endl;
	}
	else
	{
		fs << "案例类型 = 1" << ";" << endl;
		fs << "演示案例目录 = " << m_DataFileDir << ";" << endl;
	}
	fs << "事件ID = " << tmpCase.eve_id << ";" << endl;
	fs << "起始时间 = " << tmpCase.start_time << ";" << endl;
	fs << "截止时间 = " << tmpCase.end_time << ";" << endl;
	fs << "持续时间 = " << tmpCase.hold_time << ";" << endl;
	fs << "事件描述 = " << tmpCase.case_des << ";" << endl;
	fs << "触发告警设备ID = " << INT_REC_VAL(tmpCase.warn_dev_id) << ";" << endl;
	fs << "触发告警设备名 = " << tmpCase.warn_dev_name << ";" << endl;
	fs << "振荡频率 = " << tmpCase.warn_freq << ";" << endl;
	fs << "振荡最大幅值 = " << tmpCase.max_ampl << ";" << endl;
	fs << "[/概要信息]" << endl;

	// 	fs<<"[参数信息]"<<endl;
	// 	fs<<"time_leng = "<<m_LfoPara.time_leng<<";"<<endl; /* 观察时间窗口 */
	// 	fs<<"int_time = "<<m_LfoPara.int_time<<";"<<endl; /* 交叠窗口  */
	// 	fs<<"freq_min = "<<m_LfoPara.freq_min<<";"<<endl; /* 频率下限 */
	// 	fs<<"freq_max = "<<m_LfoPara.freq_max<<";"<<endl; /* 频率上限  */
	// 	fs<<"stor_damp = "<<m_LfoPara.stor_damp<<";"<<endl; /* 保存模式阻尼限值 */
	// 	fs<<"lack_damp = "<<m_LfoPara.lack_damp<<";"<<endl; /* 阻尼不足限值  */
	// 	fs<<"flfo_damp = "<<m_LfoPara.flfo_damp<<";"<<endl; /* 预警阻尼限值  */	
	// 	fs<<"lfo_damp = "<<m_LfoPara.lfo_damp<<";"<<endl; /* 告警阻尼限值 */
	// 	fs<<"gn_wn_ampl = "<<m_LfoPara.gn_wn_ampl<<";"<<endl; /* 机组振幅限值  */
	// 	fs<<"gn_wn2_ampl = "<<m_LfoPara.gn_wn2_ampl<<";"<<endl; /* 机组振幅限值2 */
	// 	fs<<"ln_wn_ampl = "<<m_LfoPara.ln_wn_ampl<<";"<<endl; /* 线路振幅限值  */
	// 	fs<<"ln_wn2_ampl = "<<m_LfoPara.ln_wn2_ampl<<";"<<endl; /* 线路振幅限值2  */	
	// 	fs<<"lfo_jud_num = "<<m_LfoPara.lfo_jud_num<<";"<<endl; /* 判断次数 */
	// 	fs<<"rcd_pre_t = "<<m_LfoPara.rcd_pre_t<<";"<<endl; /* 记录超前时间  */
	// 	fs<<"rcd_aft_t = "<<m_LfoPara.rcd_aft_t<<";"<<endl; /* 记录延后时间 */
	// 	fs<<"tr_wn_ampl = "<<m_LfoPara.tr_wn_ampl<<";"<<endl; /* 线路振幅限值  */
	// 	fs<<"tr_wn2_ampl = "<<m_LfoPara.tr_wn2_ampl<<";"<<endl; /* 变压器振幅限值2  */	
	// 	fs<<"lowfreq_min = "<<m_LfoPara.lowfreq_min<<";"<<endl; /* 低频振荡频率下限 */
	// 	fs<<"lowfreq_max = "<<m_LfoPara.lowfreq_max<<";"<<endl; /* 低频振荡频率上限  */
	// 	fs<<"time_leng_coe = "<<m_LfoPara.time_leng_coe<<";"<<endl; /* 观察时间窗口系数 */
	// 	fs<<"smp_freq_coe = "<<m_LfoPara.smp_freq_coe<<";"<<endl; /* 采样频率系数  */
	// 	fs<<"int_time_coe = "<<m_LfoPara.int_time_coe<<";"<<endl; /* 交叠窗口系数  */
	// 	fs<<"warnlfo_time = "<<m_LfoPara.warnlfo_time<<";"<<endl; /* 告警持续时间限值 */
	// 	fs<<"safelfo_time = "<<m_LfoPara.safelfo_time<<";"<<endl; /* 安全持续时间限值  */
	// 	fs<<"time_leng_max = "<<m_LfoPara.time_leng_max<<";"<<endl; /* 最大观察时间窗口 */
	// 	fs<<"time_leng_min = "<<m_LfoPara.time_leng_min<<";"<<endl; /* 最小观察时间窗口  */
	// 	fs<<"volt_thre1 = "<<m_LfoPara.volt_thre1<<";"<<endl; /* 电压等级1  */	
	// 	fs<<"volt_thre2 = "<<m_LfoPara.volt_thre2<<";"<<endl; /* 电压等级2 */
	// 	fs<<"ampl_thre1 = "<<m_LfoPara.ampl_thre1<<";"<<endl; /* 线路和变压器振幅限值1  */
	// 	fs<<"ampl_thre2 = "<<m_LfoPara.ampl_thre2<<";"<<endl; /* 线路和变压器振幅限值2 */
	// 	fs<<"ampl_thre3 = "<<m_LfoPara.ampl_thre3<<";"<<endl; /* 线路和变压器振幅限值3  */
	// 	fs<<"gn_wn_ampl1 = "<<m_LfoPara.gn_wn_ampl1<<";"<<endl; /* 机组结束振荡振幅限值  */			
	// 	fs<<"ln_wn_ampl1 = "<<m_LfoPara.ln_wn_ampl1<<";"<<endl; /* 线路结束振荡振幅限值 */
	// 	fs<<"tr_wn_ampl1 = "<<m_LfoPara.tr_wn_ampl1<<";"<<endl; /* 变压结束振荡器振幅限值  */
	// 	fs<<"ln_swn_ampl1 = "<<m_LfoPara.ln_swn_ampl1<<";"<<endl; /* 线路开始振荡振幅限值1 */
	// 	fs<<"ln_swn_ampl2 = "<<m_LfoPara.ln_swn_ampl2<<";"<<endl; /* 线路开始振荡振幅限值2  */
	// 	fs<<"ln_swn_ampl3 = "<<m_LfoPara.ln_swn_ampl3<<";"<<endl; /* 线路开始振荡振幅限值3  */	
	// 	fs<<"ln_ewn_ampl1 = "<<m_LfoPara.ln_ewn_ampl1<<";"<<endl; /* 线路结束振荡振幅限值1 */
	// 	fs<<"ln_ewn_ampl2 = "<<m_LfoPara.ln_ewn_ampl2<<";"<<endl; /* 线路结束振荡振幅限值2  */
	// 	fs<<"ln_ewn_ampl3 = "<<m_LfoPara.ln_ewn_ampl3<<";"<<endl; /* 线路结束振荡振幅限值3 */
	// 	fs<<"tr_swn_ampl1 = "<<m_LfoPara.tr_swn_ampl1<<";"<<endl; /* 变压器开始振荡振幅限值1  */
	// 	fs<<"tr_swn_ampl2 = "<<m_LfoPara.tr_swn_ampl2<<";"<<endl; /* 变压器开始振荡振幅限值2  */	
	// 	fs<<"tr_swn_ampl3 = "<<m_LfoPara.tr_swn_ampl3<<";"<<endl; /* 变压器开始振荡振幅限值3 */
	// 	fs<<"tr_ewn_ampl1 = "<<m_LfoPara.tr_ewn_ampl1<<";"<<endl; /* 变压器结束振荡振幅限值1  */
	// 	fs<<"tr_ewn_ampl2 = "<<m_LfoPara.tr_ewn_ampl2<<";"<<endl; /* 变压器结束振荡振幅限值2 */
	// 	fs<<"tr_ewn_ampl3 = "<<m_LfoPara.tr_ewn_ampl3<<";"<<endl; /* 变压器结束振荡振幅限值3  */
	// 	fs<<"sh_fact = "<<m_LfoPara.sh_fact<<";"<<endl; /* 参与因子阀值  */	
	// 	fs<<"sh_phas = "<<m_LfoPara.sh_phas<<";"<<endl; /* 相角超前阀值 */
	// 	fs<<"sh_gn_pow = "<<m_LfoPara.sh_gn_pow<<";"<<endl; /* 机组耗散功率阀值  */
	// 	fs<<"sh_ln_pow = "<<m_LfoPara.sh_ln_pow<<";"<<endl; /* 支路耗散功率阀值 */
	// 	fs<<"osc_pow_ampl = "<<m_LfoPara.osc_pow_ampl<<";"<<endl; /* 小扰动振幅门槛值  */
	// 	fs<<"osc_sec_ampl = "<<m_LfoPara.osc_sec_ampl<<";"<<endl; /* 小扰动持续时间门槛值  */
	// 	fs<<"if_flit_lfo = "<<"0;"<<endl; /* 是否过滤低频振荡 */
	// 	fs<<"[/参数信息]"<<endl;

	fs << "[详细信息]" << endl;
	for (ii = 0; ii < m_nDevEveWarn; ii++)
	{
		fs << INT_REC_VAL(m_pDevEveWarn[ii].dev_id) << " ";
		fs << m_pDevEveWarn[ii].dev_name << " ";
		fs << m_pDevEveWarn[ii].sso_bgn << " ";
		fs << m_pDevEveWarn[ii].sso_end << " ";
		fs << m_pDevEveWarn[ii].lead_ampl << " ";
		fs << m_pDevEveWarn[ii].lead_freq << ";" << endl;
	}
	fs << "[/详细信息]" << endl;

	fs << "[振荡厂站]" << endl;
	for (ii = 0; ii < m_nEveFcWarn; ii++)
	{
		start_time = 0;
		end_time = 0;
		if ((m_pEveFcWarn[ii].sso_bgn % 60) == 0)
		{
			start_time = m_pEveFcWarn[ii].sso_bgn;
		}
		else
		{
			start_time = m_pEveFcWarn[ii].sso_bgn - (m_pEveFcWarn[ii].sso_bgn % 60);
		}
		if ((m_pEveFcWarn[ii].sso_end % 60) == 0)
		{
			end_time = m_pEveFcWarn[ii].sso_end;
		}
		else
		{
			end_time = m_pEveFcWarn[ii].sso_end - (m_pEveFcWarn[ii].sso_end % 60) + 60;
		}
		sprintf(m_LogString, "sso_bgn=%ld,start_time=%ld %ld", m_pEveFcWarn[ii].sso_bgn, start_time, m_pEveFcWarn[ii].sso_bgn % 60);
		MessShow(m_LogString, LTINFO);
		sprintf(m_LogString, "sso_end=%ld,end_time=%ld %ld", m_pEveFcWarn[ii].sso_end, end_time, m_pEveFcWarn[ii].sso_end % 60);
		MessShow(m_LogString, LTINFO);
		fs << INT_REC_VAL(m_pEveFcWarn[ii].fac_id) << " ";
		fs << m_pEveFcWarn[ii].fac_name << " ";
		fs << m_pEveFcWarn[ii].sso_bgn << " ";
		fs << m_pEveFcWarn[ii].sso_end << " ";
		fs << start_time << " ";
		fs << end_time << ";" << endl;
	}
	fs << "[/振荡厂站]" << endl;

	fs << "[录波文件]" << endl;
	fs << "[/录波文件]" << endl;

	fs << "[测点信息]" << endl;
	// 	for (ii =0; ii<(int)tmpCase.vecdev.size(); ii++)
	// 	{
	// 		fs<<tmpCase.vecdev[ii].fac_id<<" ";
	// 		fs<<tmpCase.vecdev[ii].dev_id<<" ";
	// 		fs<<tmpCase.vecdev[ii].column_id<<" ";
	// #ifdef _D5000
	// 		fs<<tmpCase.vecdev[ii].yc_id<<" ";
	// #else
	// 		fs<<tmpCase.vecdev[ii].dev_id<<tmpCase.vecdev[ii].column_id<<" ";
	// #endif
	// 		fs<<tmpCase.vecdev[ii].file_no<<" ";
	// 		fs<<tmpCase.vecdev[ii].col_no<<" ";
	// 		fs<<tmpCase.vecdev[ii].is_warn<<";"<<endl;
	// 	}
	fs << "[/测点信息]" << endl;

	fs.close();

	// 将文件从res目录拷贝到run目录下
	char filenmaeabs1[MAXBUF];
#ifdef _WINDOWS32
	sprintf(filenmaeabs1, "%s\\%s", m_CasefDir, filename);
#else
	sprintf(filenmaeabs1, "%s/%s", m_CasefDir, filename);
#endif
	sprintf(m_LogString, "案例标志文件[%s]", filenmaeabs1);
	MessShow(m_LogString, LTINFO);
	PF_CopyFileM(filenamebas, filenmaeabs1);

	return 0;
}

//  召唤录波文件 
//  2008-09-18
/*int CLfoAnaObj::CallComtrFile()
{
	if( para_IsCallComtrFile == 0 )
	{
		sprintf( m_LogString, "CallComtrFile(): 退出录波文件召唤![%d]" ,para_IsCallComtrFile);
		WriteLogFile( m_LogString );
		return 0;
	}
#ifdef _WAMAP_
#ifndef  _WINDOWS32
	if(IsOnDuty(LFO_AP_ID, g_contextNo) != 1 )
	{
		sprintf( m_LogString, "CallDynFile(): 本机不是WAMAP主机,退出召唤录波文件!" );
		WriteLogFile( m_LogString );
		return 0;
	}
#endif

#ifdef  USE_WEEK_WAM_MEM // 7天缓冲数据 画面显示利用平台研究态research context
	sprintf( m_LogString, "CallComtrFile(): 离线分析，退出召唤录波文件!" );
	WriteLogFile( m_LogString );
	return 0;
#endif
	//  召唤所有有测点的厂站
	//  起始时间为告警开始时间
	//  结束时间为告警结束时间
	//  每一分钟发一次消息，召唤所有测点数据
	//  需完善：未判断是否召唤成功
	//          未判断本地是否已存在该文件
	int ii, jj, MinNum=0;
	struct tm *timePtr = NULL;
	tmd::TMD_DYN_GET_STRU tmd_buf;

	MinNum = (int)(m_SysResSave.hold_time/60)+1;
	timePtr = localtime((time_t *)&(m_SysResSave.lfo_b_t));

	for( ii=0; ii<MinNum; ii++)
	{
		tmd_buf.beg_time.tm_year = timePtr->tm_year;
		tmd_buf.beg_time.tm_mon  = timePtr->tm_mon;
		tmd_buf.beg_time.tm_mday = timePtr->tm_mday;
		tmd_buf.beg_time.tm_hour = timePtr->tm_hour;
		tmd_buf.beg_time.tm_min  = timePtr->tm_min+ii;

		tmd_buf.end_time.tm_year = timePtr->tm_year;
		tmd_buf.end_time.tm_mon  = timePtr->tm_mon;
		tmd_buf.end_time.tm_mday = timePtr->tm_mday;
		tmd_buf.end_time.tm_hour = timePtr->tm_hour;
		tmd_buf.end_time.tm_min  = timePtr->tm_min+ii+1;

		tmd_buf.seq_fac_id.length(m_nFcNum);
		for( jj=0; jj<m_nFcNum; jj++ )
		{
			tmd_buf.seq_fac_id[jj]  =  m_pFcInf[jj].fac_id;
		}

		char *msg_buf = NULL;
		int  buf_size;
		MARSHAL(tmd_buf,msg_buf,buf_size);

		int ret_code=g_pMsgBus->SendMessageToChannel(msg_buf,tmd::TMD_MSG_TYPE_DYN_GET,buf_size,CHID_WAMAP_TMD_REVOKE);

		if (msg_buf) { free(msg_buf); msg_buf =NULL; }

		if ( ret_code < 0)
		{
			sprintf(m_LogString,"CallDynFile()::Send Message To Call Dyn File Fail! Time=[%d]\n",timePtr->tm_min+ii);
			WriteLogFile(m_LogString);
		}
	}

	sprintf(m_LogString, "CallDynFile(): Call Dyn File Finished!\n" ) ;
	WriteLogFile( m_LogString );
	return 0;
#endif
	return 0;
}*/

//  召唤100帧文件 
//  2008-08-19
int CLfoAnaObj::CallDynFile()
{
	//     if( para_IsCallDynFile == LFO_TMD_DYN_GET_NO )
	//     {
	//         sprintf( m_LogString, "CallDynFile(): 退出100帧文件召唤![%d]" ,para_IsCallDynFile); 
	//         WriteLogFile( m_LogString );
	//         return 0;
	//     }
	// #ifdef _WAMAP_
	// #ifndef  _WINDOWS32 
	//     if(IsOnDuty(LFO_AP_ID, g_ctxNo) != 1 )
	//     {
	//         sprintf( m_LogString, "CallDynFile(): 本机不是WAMAP主机,退出召唤100帧文件!" ); 
	//         WriteLogFile( m_LogString );
	//         return 0;
	//     }
	// #endif
	//     
	// #ifndef  USE_WEEK_WAM_MEM
	//     if ( m_DispCtl.is_tmd == MENU_LFO_MENU_NO )
	//     {
	//         sprintf( m_LogString, "CallDynFile(): m_DispCtl.is_tmd = %s[%d],召唤100帧文件!", "否", MENU_LFO_MENU_NO ); 
	//         WriteLogFile( m_LogString );
	//         return 0;
	//     }
	// #else
	//     // 2008-05-19 by zhw 
	//     if( m_RunCtrCom != MENU_LFO_PRO_CTL_SAVE )
	//     {
	//         sprintf( m_LogString, "CallDynFile(): 离线分析退出召唤100帧文件!" ); 
	//         WriteLogFile( m_LogString );
	//         return 0;
	//     }
	// #endif
	//     //  起始时间为告警开始时间
	//     //  结束时间为告警结束时间
	//     int ii, jj, kk;
	//     int fac_num = 0;
	//     int findflag = -1;
	// 	int ret_code;
	//     DynFileStru *pDynVec = NULL;
	// 
	//     pDynVec = (DynFileStru *)calloc(para_nFcMaxMem, sizeof(DynFileStru));
	// 	memset((void *)pDynVec, 0, para_nFcMaxMem * sizeof(DynFileStru));
	// 
	//     if( para_CallAllFacDyn == 1) //  召唤所有有测点的厂站
	//     {
	//         for( ii=0; ii<m_nFcNum; ii++ )
	//         {
	//             if ( m_pFcPmu[ii].m_bs_vecV.viFilNo[0] < 0 && m_pFcPmu[ii].m_bs_vecV.viColNo[0] < 0 )
	//             {
	//                 continue;
	//             }
	//             sprintf(m_LogString,"CallDynFile()::fac_id=%d,fac_name=%s\n",m_pFcInf[ii].fac_id,m_pFcInf[ii].fac_name);
	//             WriteLogFile(m_LogString);
	//             if( IsDynFileExist(m_pFcInf[ii].fac_id,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
	//             {
	//                 pDynVec[fac_num].fac_id = m_pFcInf[ii].fac_id;
	//                 fac_num = fac_num +1;
	//             }
	//         }
	//     }
	//     else if ( para_CallAllFacDyn == 2)
	// 	{
	// 		// 读取厂站表,获取所有厂站信息
	// 		// 厂站表
	// 		CBuffer                buf_base;
	// 		int                    TabBufSize = 0;
	// 		FAC_TAB_CRE *       pFacInf = NULL;
	// 		int                 nFacInf = 0;
	// 		if ( g_IDBTableOp.Open( LFO_AP_ID, FAC_INFO_NO, g_ctxNo)  == DB_OK )
	// 		{
	// 			ret_code = g_IDBTableOp.TableGet( FAC_TAB_CRE_FLD, buf_base );
	// 			if ( ret_code > 0 )
	// 			{
	// 				TabBufSize = sizeof(FAC_TAB_CRE);
	// 				nFacInf = buf_base.GetLength()/TabBufSize; 
	// 				pFacInf = (FAC_TAB_CRE *)calloc(nFacInf, sizeof(FAC_TAB_CRE));
	// 				memset((void *)pFacInf, 0, nFacInf * sizeof(FAC_TAB_CRE));
	// 				memcpy((void*)pFacInf, (void*)buf_base.GetBufPtr(), nFacInf*TabBufSize);
	// 			}
	// 		}
	// 		if( nFacInf <= 0 )
	// 		{
	// 			sprintf( m_LogString, " FAC_INFO_NO[%d]  no record ", FAC_INFO_NO ); 
	// 			WriteLogFile( m_LogString );
	// 			
	// 			if( pFacInf    )  {free(  pFacInf );  pFacInf    = NULL;}
	// 		}
	// 		else
	// 		{
	// 			fac_num = nFacInf;
	// 			for ( ii =0; ii<nFacInf; ii++ )
	// 			{
	// 				pDynVec[ii].fac_id = pFacInf[ii].fac_id;
	// 			}
	// 		}
	// 	}
	// 	else
	//     {
	//         for( ii=0;ii<m_nGnNum;ii++)
	//         {
	//             if( m_pGnResSave[ii].lfo_b_t > 0 )
	//             {
	//                 for(jj=0;jj<m_nGnNum;jj++)
	//                 {
	//                     if( m_pGnResSave[ii].obj_id == m_pGnInf[jj].gn_id )
	//                     {
	//                         sprintf(m_LogString,"CallDynFile()::gn_name=%s,fac_id=%d,fac_name=%s\n",m_pGnInf[jj].gn_name,m_pGnInf[jj].fac_id,m_pGnInf[jj].fac_name);
	//                         WriteLogFile(m_LogString);
	//                         if( IsDynFileExist(m_pGnInf[jj].fac_id,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
	//                         {
	//                             findflag = -1;
	//                             for( kk=0; kk<fac_num; kk++)
	//                             {
	//                                 if( m_pGnInf[jj].fac_id == pDynVec[kk].fac_id)
	//                                 {
	//                                     findflag = kk;
	//                                     break;
	//                                 }
	//                             }
	//                             if(findflag == -1)
	//                             {
	//                                 pDynVec[fac_num].fac_id = m_pGnInf[jj].fac_id;
	//                                 fac_num = fac_num +1;
	//                             }
	//                         }
	//                         break;
	//                     }
	//                 }
	//             }
	//         }
	//         for( ii=0;ii<m_nLnNum;ii++)
	//         {
	//             if( m_pLnResSave[ii].lfo_b_t > 0 )
	//             {
	//                 for(jj=0;jj<m_nLnNum;jj++)
	//                 {
	//                     if( m_pLnResSave[ii].obj_id == m_pLnInf[jj].acln_id )
	//                     {
	//                         if( m_pLnPmu[jj].vecPwr1.viFilNo >=0 && m_pLnPmu[jj].vecPwr1.viColNo >=0 )
	//                         //if( m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_1)
	//                         {
	//                             sprintf(m_LogString,"CallDynFile()::acln_name=%s,fac_id_1=%d,fac_name=%s\n",m_pLnInf[jj].acln_name,m_pLnInf[jj].fac_id_1,m_pLnInf[jj].fac_name_1);
	//                             WriteLogFile(m_LogString);
	//                             if( IsDynFileExist(m_pLnInf[jj].fac_id_1,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
	//                             {
	//                                 findflag = -1;
	//                                 for( kk=0; kk<fac_num; kk++)
	//                                 {
	//                                     if( m_pLnInf[jj].fac_id_1 == pDynVec[kk].fac_id)
	//                                     {
	//                                         findflag = kk;
	//                                         break;
	//                                     }
	//                                 }
	//                                 if(findflag == -1)
	//                                 {
	//                                     pDynVec[fac_num].fac_id = m_pLnInf[jj].fac_id_1;
	//                                     fac_num = fac_num +1;
	//                                 }
	//                             }
	//                             break;
	//                         }
	//                         else if( m_pLnPmu[jj].vecPwr2.viFilNo >=0 && m_pLnPmu[jj].vecPwr2.viColNo >=0 )
	//                         {
	//                             sprintf(m_LogString,"CallDynFile()::acln_name=%s,fac_id_2=%d,fac_name=%s\n",m_pLnInf[jj].acln_name,m_pLnInf[jj].fac_id_2,m_pLnInf[jj].fac_name_2);
	//                             WriteLogFile(m_LogString);
	//                             if( IsDynFileExist(m_pLnInf[jj].fac_id_2,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
	//                             {
	//                                 findflag = -1;
	//                                 for( kk=0; kk<fac_num; kk++)
	//                                 {
	//                                     if( m_pLnInf[jj].fac_id_2 == pDynVec[kk].fac_id)
	//                                     {
	//                                         findflag = kk;
	//                                         break;
	//                                     }
	//                                 }
	//                                 if(findflag == -1)
	//                                 {
	//                                     pDynVec[fac_num].fac_id = m_pLnInf[jj].fac_id_2;
	//                                     fac_num = fac_num +1;
	//                                 }
	//                             }
	//                             break;
	//                         }
	//                         else
	//                         {
	//                             sprintf(m_LogString,"CallDynFile()::get acln fac error! slct_dot_id: %d, dot1: %d, dot2: %d\n",m_pLnMon[jj].slct_dot_id,m_pLnInf[jj].dot_id_1,m_pLnInf[jj].dot_id_2);
	//                             WriteLogFile(m_LogString);
	//                             break;
	//                         }
	// 
	//                     }
	//                 }
	//             }
	//         }
	//         for( ii=0;ii<m_nTrNum;ii++)
	//         {
	//             if( m_pTrResSave[ii].lfo_b_t > 0 )
	//             {
	//                 for(jj=0;jj<m_nTrNum;jj++)
	//                 {
	//                     if( m_pTrResSave[ii].obj_id == m_pTrInf[jj].tr_id )
	//                     {
	//                         sprintf(m_LogString,"CallDynFile()::tr_name=%s,fac_id=%d,fac_name=%s\n",m_pTrInf[jj].tr_name,m_pTrInf[jj].fac_id,m_pTrInf[jj].fac_name);
	//                         WriteLogFile(m_LogString);
	//                         if( IsDynFileExist(m_pTrInf[jj].fac_id,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
	//                         {
	//                             findflag = -1;
	//                             for( kk=0; kk<fac_num; kk++)
	//                             {
	//                                 if( m_pTrInf[jj].fac_id == pDynVec[kk].fac_id)
	//                                 {
	//                                     findflag = kk;
	//                                     break;
	//                                 }
	//                             }
	//                             if(findflag == -1)
	//                             {
	//                                 pDynVec[fac_num].fac_id = m_pTrInf[jj].fac_id;
	//                                 fac_num = fac_num +1;
	//                             }
	//                         }
	//                         break;
	//                     }
	//                 }
	//             }
	//         }
	//     }
	//     if( fac_num <= 0 )
	//     {
	//         if( pDynVec ) {free(pDynVec); pDynVec = NULL;}
	//         return 0;
	//     }
	// 
	// 	// 开始延时召唤
	// 	struct tm *timePtr = NULL;
	//     tmd::TMD_DYN_GET_STRU tmd_buf;
	// 
	// 	// 先延时,等待子站记录文件生成
	// 	sprintf(m_LogString,"CallDynFile()::共召唤厂站个数[%d]",fac_num);
	//     WriteLogFile(m_LogString);
	// 	sprintf(m_LogString,"CallDynFile()::开始延时[%d]",para_CallDynDelaySec);
	//     WriteLogFile(m_LogString);
	// 	PT_DelaySecond(para_CallDynDelaySec);
	// 
	//     char *msg_buf = NULL;
	//     int  buf_size;
	// 	time_t beg_time = m_SysResSave.lfo_b_t - m_LfoPara.rcd_pre_t;
	// 	time_t end_time = m_SysResSave.lfo_e_t + m_LfoPara.rcd_aft_t;
	// 	int fac_times = 10;
	// 	int callfacnum = fac_num / fac_times;
	// 	int endfacnum = fac_num - fac_times * callfacnum;
	// 	int facidx =0;
	// 	for ( ii =0; ii<callfacnum; ii++ )
	// 	{
	// 		memset((void *)&tmd_buf,0,sizeof(tmd::TMD_DYN_GET_STRU));
	// 		timePtr = localtime(&beg_time);
	// 		tmd_buf.beg_time.tm_year = timePtr->tm_year;
	// 		tmd_buf.beg_time.tm_mon  = timePtr->tm_mon;
	// 		tmd_buf.beg_time.tm_mday = timePtr->tm_mday;
	// 		tmd_buf.beg_time.tm_hour = timePtr->tm_hour;
	// 		tmd_buf.beg_time.tm_min  = timePtr->tm_min;
	// 		timePtr = localtime(&end_time);
	// 		tmd_buf.end_time.tm_year = timePtr->tm_year;
	// 		tmd_buf.end_time.tm_mon  = timePtr->tm_mon;
	// 		tmd_buf.end_time.tm_mday = timePtr->tm_mday;
	// 		tmd_buf.end_time.tm_hour = timePtr->tm_hour;
	// 		tmd_buf.end_time.tm_min  = timePtr->tm_min;
	// 		tmd_buf.seq_fac_id.length(fac_times);
	// 		for ( jj =0; jj<fac_times; jj++ )
	// 		{
	// 			tmd_buf.seq_fac_id[jj] = pDynVec[facidx].fac_id;
	// 			sprintf(m_LogString,"no= %d, fac_id= %d,bgn_time= %d, end_time= %d",jj,tmd_buf.seq_fac_id[jj],tmd_buf.beg_time.tm_min,tmd_buf.end_time.tm_min );
	// 			WriteLogFile(m_LogString);
	// 			facidx ++;
	// 		}
	// 		if (msg_buf) { free(msg_buf); msg_buf =NULL; }
	// 		MARSHAL(tmd_buf,msg_buf,buf_size);
	// 		ret_code=g_pMsgBus->SendMessageToChannel(msg_buf,tmd::TMD_MSG_TYPE_DYN_GET,buf_size,CHID_WAMAP_TMD_REVOKE);
	// 		if ( ret_code < 0)
	// 		{
	// 			sprintf(m_LogString,"CallDynFile()::Send Message To Call Dyn File Fail!\n");
	// 			WriteLogFile(m_LogString);
	// 		}
	// 
	// 		PT_DelaySecond(para_CallDynDelaySec);
	// 	}
	// 
	// 	memset((void *)&tmd_buf,0,sizeof(tmd::TMD_DYN_GET_STRU));
	// 	timePtr = localtime(&beg_time);
	// 	tmd_buf.beg_time.tm_year = timePtr->tm_year;
	// 	tmd_buf.beg_time.tm_mon  = timePtr->tm_mon;
	// 	tmd_buf.beg_time.tm_mday = timePtr->tm_mday;
	// 	tmd_buf.beg_time.tm_hour = timePtr->tm_hour;
	// 	tmd_buf.beg_time.tm_min  = timePtr->tm_min;
	// 	timePtr = localtime(&end_time);
	// 	tmd_buf.end_time.tm_year = timePtr->tm_year;
	// 	tmd_buf.end_time.tm_mon  = timePtr->tm_mon;
	// 	tmd_buf.end_time.tm_mday = timePtr->tm_mday;
	// 	tmd_buf.end_time.tm_hour = timePtr->tm_hour;
	// 	tmd_buf.end_time.tm_min  = timePtr->tm_min;
	// 	tmd_buf.seq_fac_id.length(endfacnum);
	// 	for ( jj =0; jj<endfacnum; jj++ )
	// 	{
	// 		tmd_buf.seq_fac_id[jj] = pDynVec[facidx].fac_id;
	// 		sprintf(m_LogString,"no= %d, fac_id= %d,bgn_time= %d, end_time= %d",jj,tmd_buf.seq_fac_id[jj],tmd_buf.beg_time.tm_min,tmd_buf.end_time.tm_min );
	// 		WriteLogFile(m_LogString);
	// 		facidx ++;
	// 	}
	// 	if (msg_buf) { free(msg_buf); msg_buf =NULL; }
	// 	MARSHAL(tmd_buf,msg_buf,buf_size);
	// 	ret_code=g_pMsgBus->SendMessageToChannel(msg_buf,tmd::TMD_MSG_TYPE_DYN_GET,buf_size,CHID_WAMAP_TMD_REVOKE);
	// 	if ( ret_code < 0)
	// 	{
	// 		sprintf(m_LogString,"CallDynFile()::Send Message To Call Dyn File Fail!\n");
	// 		WriteLogFile(m_LogString);
	// 	}
	// 	
	//     if (msg_buf) { free(msg_buf); msg_buf =NULL; }
	//     sprintf(m_LogString, "CallDynFile(): Call Dyn File Finished!\n" ) ;
	//     WriteLogFile( m_LogString );
	//     if( pDynVec ) {free(pDynVec); pDynVec = NULL;}
	// #endif
	return 0;
}

//  召唤录波文件 
//  2008-10-01
int CLfoAnaObj::CallTrFile()
{
	/*if( para_IsCallComtrFile == 0 )
	{
		sprintf( m_LogString, "CallTrFile: 退出录波文件召唤![%d]" ,para_IsCallDynFile);
		WriteLogFile( m_LogString );
		return 0;
	}
#ifdef _WAMAP_
#ifndef  _WINDOWS32
	if(IsOnDuty(LFO_AP_ID, g_contextNo) != 1 )
	{
		sprintf( m_LogString, "CallTrFile: 本机不是WAMAP主机,退出召唤录波文件!" );
		WriteLogFile( m_LogString );
		return 0;
	}
#endif

#ifdef  USE_WEEK_WAM_MEM // 7天缓冲数据 画面显示利用平台研究态research context
	sprintf( m_LogString, "CallTrFile: 离线分析，退出召唤录波文件!" );
	WriteLogFile( m_LogString );
	return 0;
#endif

	//  起始时间为告警开始时间
	//  结束时间为告警结束时间
	int ii, jj;
	int fac_num=0;
	struct FAD_CALLUP_COMTR_NAME_STRU comtr_buf;

	comtr_buf.start_time = m_SysResSave.lfo_b_t;
	comtr_buf.end_time   = m_SysResSave.lfo_e_t;
	if( para_CallAllFacDyn == 1) //  召唤所有有测点的厂站
	{
		for( ii=0; ii<m_nFcNum; ii++)
		{
			if ( m_pFcPmu[ii].m_bs_vecV.viFilNo[0] < 0 && m_pFcPmu[ii].m_bs_vecV.viColNo[0] < 0 )
			{
				continue;
			}
			sprintf(m_LogString,"CallTrFile()::fac_id=%d,fac_name=%s\n",m_pFcInf[ii].fac_id,m_pFcInf[ii].fac_name);
			WriteLogFile(m_LogString);
			// 需判断录波文件是否已经存在
			//if( IsTrFileExist(m_pFcInf[ii].fac_id,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
			{
				comtr_buf.fac_seq[fac_num] = m_pFcInf[ii].fac_id;
				fac_num    = fac_num +1;
			}
		}
	}
	else
	{
		for( ii=0;ii<m_nGnNum;ii++)
		{
			if( m_pGnResSave[ii].if_lfo == MENU_LFO_MENU_YES )
			{
				for(jj=0;jj<m_nGnNum;jj++)
				{
					if( m_pGnResSave[ii].obj_id == m_pGnInf[jj].gn_id )
					{
						sprintf(m_LogString,"CallTrFile()::gn_name=%s,fac_id=%d,fac_name=%s\n",m_pGnInf[jj].gn_name,m_pGnInf[jj].fac_id,m_pGnInf[ii].fac_name);
						WriteLogFile(m_LogString);
						//if( IsTrFileExist(m_pGnInf[ii].fac_id,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
						{
							comtr_buf.fac_seq[fac_num] = m_pGnInf[jj].fac_id;
							fac_num = fac_num +1;
						}
						break;
					}
				}
			}
		}
		for( ii=0;ii<m_nLnNum;ii++)
		{
			if( m_pLnResSave[ii].if_lfo == MENU_LFO_MENU_YES )
			{
				for(jj=0;jj<m_nLnNum;jj++)
				{
					if( m_pLnResSave[ii].obj_id == m_pLnInf[jj].acln_id )
					{
						if( m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_1)
						{
							sprintf(m_LogString,"CallTrFile()::acln_name=%s,fac_id=%d,fac_name=%s\n",m_pLnInf[jj].acln_name,m_pLnInf[jj].fac_id_1,m_pLnInf[ii].fac_name_1);
							WriteLogFile(m_LogString);
							//if( IsTrFileExist(m_pLnInf[ii].fac_id_1,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
							{
								comtr_buf.fac_seq[fac_num] = m_pLnInf[jj].fac_id_1;
								fac_num = fac_num +1;
							}
							break;
						}
						else
						{
							sprintf(m_LogString,"CallTrFile()::acln_name=%s,fac_id=%d,fac_name=%s\n",m_pLnInf[jj].acln_name,m_pLnInf[jj].fac_id_2,m_pLnInf[ii].fac_name_2);
							WriteLogFile(m_LogString);
							//if( IsTrFileExist(m_pLnInf[ii].fac_id_2,m_SysResSave.lfo_b_t,m_SysResSave.lfo_e_t) ==0 )
							{
								comtr_buf.fac_seq[fac_num] = m_pLnInf[ii].fac_id_2;
								fac_num = fac_num +1;
							}
							break;
						}
					}
				}
			}
		}
	}

	sprintf(m_LogString,"CallTrFile()::共召唤厂站个数[%d]",fac_num);
	WriteLogFile(m_LogString);
	comtr_buf.fac_num = fac_num;

	char *msg_buf = NULL;
	int  buf_size;
	MARSHAL(comtr_buf,msg_buf,buf_size);

	int ret_code=g_pMsgBus->SendMessageToChannel(msg_buf,FAD_GET_COMTR_FILE,buf_size,CHID_FAD_CALL_COMTR);

	if (msg_buf) { free(msg_buf); msg_buf =NULL; }

	if ( ret_code < 0)
	{
		sprintf(m_LogString,"CallTrFile()::Send Message To Call Dyn File Fail! fac_name=[%s]\n",m_pFcInf[ii].fac_name);
		WriteLogFile(m_LogString);
	}

	sprintf(m_LogString, "CallTrFile(): Call COMTR File Finished!\n" ) ;
	WriteLogFile( m_LogString );
	return 0;
#endif*/
	return 0;
}

int CLfoAnaObj::IsDynFileExist(int fac_id, int bgn_t, int end_t)
{
	vector<string> dynVec;
	if (FormDynFile(fac_id, bgn_t, end_t, dynVec) < 0)
	{
		sprintf(m_LogString, "IsDynFileExist()::Form PMU Dynamic File Name Fail!\n");
		MessShow(m_LogString);

		return (-1);
	}

	int    ii = 0;
	struct stat buf;
	string dyn_fir;
	string dyn_sec;
	for (ii = 0; ii < (int)dynVec.size(); ++ii)
	{
		dyn_fir = DYN_FILE_PATH_FIR;
		dyn_fir += dynVec[ii];

		dyn_sec = DYN_FILE_PATH_SEC;
		dyn_sec += dynVec[ii];

		if (stat(dyn_fir.c_str(), &buf) == -1 && stat(dyn_sec.c_str(), &buf) == -1)
		{
			sprintf(m_LogString, "%s", dynVec[ii].c_str());
			MessShow(m_LogString);
			return (0);
		}
	}

	return (1);
}

/*int CLfoAnaObj::IsTrFileExist(int fac_id, int bgn_t, int end_t)
{

}*/

int CLfoAnaObj::FormDynFile(int fac_id, int beg_sec, int end_sec, vector<string>& dynVec)
{
	// #ifdef _WAMAP_
	//     if (!dynVec.empty())
	//     {
	//         dynVec.clear();
	//     }
	//     
	//     vector<string> dynTimeVec;
	//     GetDynTimeStr(beg_sec,end_sec,dynTimeVec);
	//     
	//     int                    nWamChnInfo =0;
	//     WAM_CHANNEL_INFO_STRU *pWamChnInfo  =  NULL;
	//     CBuffer                buf_base;
	//     int                    TabBufSize=0;
	//     int                    ret_code;
	//     if ( g_IDBTableOp.Open( LFO_AP_ID, WAM_CHANNEL_INFO_NO, g_ctxNo)  == DB_OK )
	//     {
	//         ret_code = g_IDBTableOp.TableGet( TAB_WAM_CHANNEL_INFO_FLD, buf_base );
	//         if (ret_code > 0 )
	//         {
	//             TabBufSize = sizeof(WAM_CHANNEL_INFO_STRU);
	//             nWamChnInfo = buf_base.GetLength()/TabBufSize; 
	//             pWamChnInfo = (WAM_CHANNEL_INFO_STRU *)calloc(nWamChnInfo, sizeof(WAM_CHANNEL_INFO_STRU));
	//             memset((void *)pWamChnInfo, 0, nWamChnInfo * sizeof(WAM_CHANNEL_INFO_STRU));
	//             memcpy((void*)pWamChnInfo, (void*)buf_base.GetBufPtr(), nWamChnInfo*TabBufSize);
	//         }
	//     }
	//     if( nWamChnInfo <= 0 )
	//     {
	//         sprintf( m_LogString, "FormDynFile: WAM_CHANNEL_INFO_NO[%d]  no record ", WAM_CHANNEL_INFO_NO ); 
	//         WriteLogFile( m_LogString );
	//         
	//         if( pWamChnInfo )  {free(pWamChnInfo);  pWamChnInfo = NULL;}
	//         return false;
	//     }
	// 
	//     char stn[16];
	//     char idcode[9];
	//     int  ii  =  0;
	//     if (nWamChnInfo > 0)
	//     {
	//         for (ii = 0; ii < nWamChnInfo; ++ii)
	//         {
	//             if (fac_id == pWamChnInfo[ii].fac_id && strcmp(pWamChnInfo[ii].stn,""))
	//             {
	//                 strncpy(stn,"",16);
	//                 strncpy(stn,&pWamChnInfo[ii].stn[10],6);
	//                 strncat(stn,&pWamChnInfo[ii].stn[0],1);
	//                 stn[7] = '\0';
	//                 
	//                 strcpy(idcode,pWamChnInfo[ii].idcode);
	//                 
	//                 break;
	//             }
	//         }
	//         
	//         if( pWamChnInfo )  {free(pWamChnInfo);  pWamChnInfo = NULL;}
	//     }
	//     
	//     if (ii < nWamChnInfo)
	//     {
	//         short pmu_type;
	//         int   size;
	//         GetPmuType(idcode,pmu_type);
	//         
	//         if (SF_PMU_TYPE == pmu_type)
	//         {
	//             size  =  dynTimeVec.size();
	//         }
	//         else
	//         {
	//             size  =  dynTimeVec.size();
	//         }
	//         
	//         string dyn_file;
	//         for (ii = 0; ii < size; ++ii)
	//         {
	//             dyn_file  =  "RT-";
	//             dyn_file +=  stn;
	//             dyn_file +=  "-";
	//             dyn_file +=  dynTimeVec[ii];
	//             dyn_file +=  "-00100.dyn";
	//             
	//             dynVec.push_back(dyn_file);
	//         }
	//     }
	// #endif
	return (0);
}

void CLfoAnaObj::GetDynTimeStr(int beg_sec, int end_sec, vector<string>& dynTimeVec)
{
	int    off_min = (end_sec - beg_sec) / 60;
	int    cur_sec;
	char   time_str[16];
	string buf;
	struct tm* time_ptr = NULL;
	for (int ii = 0; ii <= off_min; ++ii)
	{
		cur_sec = beg_sec + ii * 60;
		time_ptr = localtime((time_t*)&cur_sec);

		sprintf(time_str, "%04d%02d%02d%02d%02d", time_ptr->tm_year + 1900, time_ptr->tm_mon + 1,
			time_ptr->tm_mday, time_ptr->tm_hour, time_ptr->tm_min);

		buf = time_str;

		dynTimeVec.push_back(buf);
	}

	return;
}
void CLfoAnaObj::GetPmuType(string idcode, short& pmuType)
{
	if (idcode.find("BSF") != string::npos)
	{
		pmuType = SF_PMU_TYPE;
	}
	else if (idcode.find("DKY") != string::npos)
	{
		pmuType = DKY_PMU_TYPE;
	}
	else
	{
		pmuType = NR_PMU_TYPE;
	}

	return;
}

///////////////////////////////  WAMS缓冲区接口  ///////////////////////////////

//  获取缓冲区最后一次刷新时间
// bool CLfoAnaObj::GetLastTimeFromWAMAP(time_t *LastUpDateTime)
// {
// #ifndef _WAMAP_
// 	return true;
// #else
// 	WT_VEC_TIME  lastVecTime ;
// 	bool retStatus = g_IWMDb->TableGet(lastVecTime) ;
// 	if( retStatus == false )
// 	{
// 		sprintf( m_LogString, "GetLastTimeFromWAMAP(): Get GPS time error: [%s]", g_IWMDb->GetError() ); 
// 		WriteLogFile( m_LogString );
// 		return false;
// 	}
// 
// 	*LastUpDateTime = lastVecTime.csecond;
// 	
// 	sprintf( m_LogString, " PMU BUFFER now time %s[%d]! ", PT_GetHMSTimeStringBySeconds(lastVecTime.csecond), lastVecTime.csecond ); 
// 	MessShow( m_LogString );
// 
// 	return true;
// 
// #endif
// }

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 根据配置信息，获取一段时间数据，逐秒判断质量码、修复，并向oFcDat赋值
//  处理过程 : 
//  备    注 : 取数成功返回true, 同时返回数据秒状态
//  返 回 值 : 取数失败返回false
//  参数说明 : iFcPmu                   相量文件号列号
//             tBgnSec                  取数时刻
//             nVecSec                  取数秒数[MAX_FRA_PER_SEC]           [OUT]
//             oFcDat                   返回数据                            [OUT]
////////////////////////////////////////////////////////////////////////////////
bool CLfoAnaObj::GetFcDataFromWAMAP(LFO_FC_PMU iFcPmu, time_t tBgnSec, int nVecSec, LFO_FC_DAT& oFcDat, int nOffSec)
{
#ifndef _WAMAP_
	return true;
#else
	int              ii, jj;
#ifndef _D5000
	short            iFilNo = -1;
	short            iColNo = -1;
#else
	int            iFilNo = -1;
	int            iColNo = -1;
#endif
	int              nRealNum = 0;

	int              nNeedNum = 0;
	int              vnData = 0;
	float* vvData = NULL;
	PUB_WT_VEC_DATA* tmpData = NULL;
	WT_VEC_DESC      desc;

	VEC_DATA_STS    vtmpSts;
	VEC_DATA_STS* vSecSts = NULL;

	if (tBgnSec <= 0 || nVecSec <= 0 || nOffSec < 0) return false;
	//  三相电压
	nNeedNum = nVecSec * MAX_FRA_PER_SEC;
	tmpData = (PUB_WT_VEC_DATA*)calloc(nNeedNum, sizeof(PUB_WT_VEC_DATA));
	memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));

	vnData = VEC_V_NUM * nNeedNum;
	vvData = (float*)calloc(vnData, sizeof(float));
	memset((void*)vvData, 0, vnData * sizeof(float));

	vSecSts = (VEC_DATA_STS*)calloc(nVecSec, sizeof(VEC_DATA_STS));
	memset((void*)vSecSts, 0, nVecSec * sizeof(VEC_DATA_STS));

	//  读取三相电压
	for (ii = 0; ii < VEC_V_NUM; ++ii)
	{
		// 是否主母
		if (iFcPmu.is_m_bs == MENU_LFO_MENU_YES)
		{
			iFilNo = iFcPmu.m_bs_vecV.viFilNo[ii];
			iColNo = iFcPmu.m_bs_vecV.viColNo[ii];
		}
		else
		{
			iFilNo = iFcPmu.v_bs_vecV.viFilNo[ii];
			iColNo = iFcPmu.v_bs_vecV.viColNo[ii];
		}

		memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
		if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
		if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
		{
			// 			sprintf(m_LogString, "GetFcDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d", g_IWMDb->GetError(),iFilNo, iColNo );
			// 			MessShow(m_LogString);
						/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
						if( vvData  )  {free(vvData );  vvData  = NULL;}
						if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
						return false;*/
		}
		if (nNeedNum != nRealNum)
		{
			/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
			if( vvData  )  {free(vvData );  vvData  = NULL;}
			if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
			return false;*/
		}

		// 逐秒逐桢修复并统计无效桢
		for (jj = 0; jj < nVecSec; jj++)
		{
			memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
			IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvData[ii * nNeedNum + jj * MAX_FRA_PER_SEC], vtmpSts);
			if (vtmpSts.nBadNum > 0)
			{
				vSecSts[jj].tBadSec = tBgnSec;

				if (vtmpSts.bPmuOff == 1)
				{
					vSecSts[jj].bPmuOff = 1;
					vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[0].viColNo = iColNo;
				}
				if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
				// 2008-07-16 by zhw
				if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
				{
					vSecSts[jj].nBadNum = vtmpSts.nBadNum;
					vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[1].viColNo = iColNo;
				}
				if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
				{
					vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
					vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[2].viColNo = iColNo;
				}
			}
		}
	}


	//  根据三相电压实采量计算线电压动态数据，相角使用A相电压相角
	double  fTmpVal1 = 0.0, fTmpVal2 = 0.0;
	int     nOffPos = nOffSec * MAX_FRA_PER_SEC;
	for (jj = 0; jj < nNeedNum; jj++)
	{
		fTmpVal1 = 0.0;
		fTmpVal2 = 0.0;      //2008-03-28 added by zhw at yn
		fTmpVal1 += vvData[0 * nNeedNum + jj] * DP_Cos(vvData[1 * nNeedNum + jj]);
		fTmpVal1 -= vvData[4 * nNeedNum + jj] * DP_Cos(vvData[5 * nNeedNum + jj]);

		fTmpVal2 += vvData[0 * nNeedNum + jj] * DP_Sin(vvData[1 * nNeedNum + jj]);
		fTmpVal2 -= vvData[4 * nNeedNum + jj] * DP_Sin(vvData[5 * nNeedNum + jj]);

		fTmpVal1 = fTmpVal1 / VF_VOLTBASE;
		fTmpVal2 = fTmpVal2 / VF_VOLTBASE;

		oFcDat.u1v[nOffPos + jj] = (float)sqrt(fTmpVal1 * fTmpVal1 + fTmpVal2 * fTmpVal2);
		oFcDat.u1a[nOffPos + jj] = vvData[1 * nNeedNum + jj];  // 2008-08-14 by zhw

		if (DP_ABS(oFcDat.u1v[nOffPos + jj]) <= 1.0f) oFcDat.u1v[nOffPos + jj] = 0.0f;
	}

	//设置数据秒状态
	for (jj = 0; jj < nVecSec; jj++)
	{
		oFcDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_OK;
		if (vSecSts[jj].bPmuOff == 1)  // 通道退出
		{
			oFcDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_OFF;
			oFcDat.VecBadData[0].viFilNo = vSecSts[jj].VecBadData[0].viFilNo;
			oFcDat.VecBadData[0].viColNo = vSecSts[jj].VecBadData[0].viColNo;
			continue;
		}
		if (IsFcStsValid(vSecSts[jj]) == false)  // 根据质量码判断，数据不可用
		{
			oFcDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_BAD;
			oFcDat.VecBadData[1].viFilNo = vSecSts[jj].VecBadData[1].viFilNo;
			oFcDat.VecBadData[1].viColNo = vSecSts[jj].VecBadData[1].viColNo;
			oFcDat.VecBadData[2].viFilNo = vSecSts[jj].VecBadData[2].viFilNo;
			oFcDat.VecBadData[2].viColNo = vSecSts[jj].VecBadData[2].viColNo;
		}
	}

	if (tmpData) { free(tmpData);  tmpData = NULL; }
	if (vvData) { free(vvData);  vvData = NULL; }
	if (vSecSts) { free(vSecSts);  vSecSts = NULL; }

	return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 获取机组PMU量测数据
//  处理过程 : 
//  备    注 : 通过三相电压电流计算功率
//  返 回 值 : 取数成功返回true,取数失败返回false
//  参数说明 : iGnPmu                   相量文件号列号
//             tBgnSec                  取数时刻
//             nVecSec                  取数秒数
//             oGnDat                   返回数据
//             nOffSec                  偏移秒
////////////////////////////////////////////////////////////////////////////////
//	para_CalcPQVByUI          = 0 ;                         //  是否计算PQV 1 通过ui计算PQV 0 直接获取PQV
bool CLfoAnaObj::GetGnDataFromWAMAP(SSO_GN_PMU iGnPmu, time_t tBgnSec, int nVecSec, SSO_GN_DAT& oGnDat, int nOffSec)
{
#ifndef _WAMAP_
	return true;
#else
	int              ii, jj;
#ifndef _D5000
	short            iFilNo = -1;
	short            iColNo = -1;
#else
	int            iFilNo = -1;
	int            iColNo = -1;
#endif
	int              nRealNum = 0;

	int              nNeedNum = 0;
	int              vnData = 0;
	float* vvAmpl = NULL;
	float* vvFreq = NULL;
	float* vvStat = NULL;
	PUB_WT_VEC_DATA* tmpData = NULL;
	WT_VEC_DESC      desc;

	VEC_DATA_STS    vtmpSts;
	VEC_DATA_STS* vSecSts = NULL;

	if (tBgnSec <= 0 || nVecSec <= 0 || nOffSec < 0) return false;
	/*
		if ( g_IWMDb->GetStatus() == false )
		{
			sprintf(m_LogString, "GetGnDataFromWAMAP(): %s", g_IWMDb->GetError() );
			MessShow(m_LogString);
			return false;
		}*/


		//  三相电压电流
	nNeedNum = nVecSec * MAX_FRA_PER_SEC;
	tmpData = (PUB_WT_VEC_DATA*)calloc(nNeedNum, sizeof(PUB_WT_VEC_DATA));
	memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));

	vnData = VEC_VI_NUM * nNeedNum;
	vvAmpl = (float*)calloc(vnData, sizeof(float));
	memset((void*)vvAmpl, 0, vnData * sizeof(float));
	vvFreq = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vvFreq, 0, nNeedNum * sizeof(float));
	vvStat = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vvStat, 0, nNeedNum * sizeof(float));

	vSecSts = (VEC_DATA_STS*)calloc(nVecSec, sizeof(VEC_DATA_STS));
	memset((void*)vSecSts, 0, nVecSec * sizeof(VEC_DATA_STS));

	// 	if ( para_CalcPQVByUI == 1 )                            ////  通过ui计算P
	// 	{
	// 		//  读取三相电压电流
	// 		for( ii = 0; ii < VEC_VI_NUM; ++ii )
	// 		{
	// 			iFilNo = iGnPmu.vecVI.viFilNo[ii];
	// 			iColNo = iGnPmu.vecVI.viColNo[ii];
	// 
	// 			memset((void *)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
	// #ifndef _D5000
	//             if ( PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false )
	// #else
	//             if ( PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false )
	// #endif
	// 			{
	// // 				sprintf(m_LogString, "GetGnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
	// // 				MessShow(m_LogString);
	// 				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
	// 				if( vviData )  {free(vviData ); vviData = NULL;}
	// 				if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
	// 				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
	// 				return false;*/
	// 			}
	// 			if ( nNeedNum != nRealNum )
	// 			{
	// 				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
	// 				if( vviData )  {free(vviData ); vviData = NULL;}
	// 				if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
	// 				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
	// 				return false;*/
	// 			}
	// 			// 逐秒逐桢修复并统计无效桢
	// 			for ( jj = 0; jj < nVecSec; jj++ )
	// 			{
	// 				memset((void *)&vtmpSts, 0, sizeof(VEC_DATA_STS));
	// 				IWM_PmuDataCheck( (PUB_WT_VEC_DATA *)&tmpData[jj*MAX_FRA_PER_SEC], tBgnSec+jj, (float*)&vviData[ii*nNeedNum + jj*MAX_FRA_PER_SEC], vtmpSts );
	// 				if( vtmpSts.nBadNum > 0 )
	// 				{
	// 					vSecSts[jj].tBadSec = tBgnSec;
	// 
	//                     if( vtmpSts.bPmuOff == 1 )
	//                     {
	//                         vSecSts[jj].bPmuOff = 1;
	//                         vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[0].viColNo = iColNo;
	//                     }
	//                     if( vtmpSts.iBadFra > vSecSts[jj].iBadFra ) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
	//                     // 2008-07-16 by zhw
	//                     if( vtmpSts.nBadNum > vSecSts[jj].nBadNum ) 
	//                     {
	//                         vSecSts[jj].nBadNum = vtmpSts.nBadNum;
	//                         vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[1].viColNo = iColNo;
	//                     }
	//                     if( vtmpSts.nBadSeq > vSecSts[jj].nBadSeq ) 
	//                     {
	//                         vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
	//                         vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[2].viColNo = iColNo;
	//                     }
	//                 }
	// 			}
	// 		}
	// 	}
	// 	else                                                    ////  直接获取 P uaa
	// 	{
	// 		//  读取三相电压电流
	// 		for( ii = 0; ii < VEC_VI_NUM; ++ii )
	// 		{
	// 			if ( ii != 7 )	continue;
	// 
	// 			iFilNo = iGnPmu.vecVI.viFilNo[ii];
	// 			iColNo = iGnPmu.vecVI.viColNo[ii];
	// 
	// 			memset((void *)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
	// #ifndef _D5000
	//             if ( PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false )
	// #else
	//             if ( PWD_TableGet(iFilNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false )
	// #endif
	// 			{
	// //                 sprintf(m_LogString, "GetGnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
	// // 				MessShow(m_LogString);
	// 				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
	// 				if( vviData )  {free(vviData ); vviData = NULL;}
	// 				if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
	// 				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
	// 				return false;*/
	// 			}
	// 			if ( nNeedNum != nRealNum )
	// 			{
	// 				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
	// 				if( vviData )  {free(vviData ); vviData = NULL;}
	// 				if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
	// 				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
	// 				return false;*/
	// 			}
	// 			// 逐秒逐桢修复并统计无效桢
	// 			for ( jj = 0; jj < nVecSec; jj++ )
	// 			{
	// 				memset((void *)&vtmpSts, 0, sizeof(VEC_DATA_STS));
	// 				IWM_PmuDataCheck( (PUB_WT_VEC_DATA *)&tmpData[jj*MAX_FRA_PER_SEC], tBgnSec+jj, (float*)&vviData[ii*nNeedNum + jj*MAX_FRA_PER_SEC], vtmpSts );
	// 				if( vtmpSts.nBadNum > 0 )
	// 				{
	// 					vSecSts[jj].tBadSec = tBgnSec;
	// 
	// 					if( vtmpSts.bPmuOff == 1 )
	//                     {
	//                         vSecSts[jj].bPmuOff = 1;
	//                         vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[0].viColNo = iColNo;
	//                     }
	// 					if( vtmpSts.iBadFra > vSecSts[jj].iBadFra ) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
	//                     // 2008-07-16 by zhw
	//                     if( vtmpSts.nBadNum > vSecSts[jj].nBadNum ) 
	//                     {
	//                         vSecSts[jj].nBadNum = vtmpSts.nBadNum;
	//                         vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[1].viColNo = iColNo;
	//                     }
	//                     if( vtmpSts.nBadSeq > vSecSts[jj].nBadSeq ) 
	//                     {
	//                         vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
	//                         vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[2].viColNo = iColNo;
	//                     }
	// 				}
	// 			}
	// 		}
			//  读取 p 放于 vviData[0]中
	{
		// 			sprintf(m_LogString, "读取SSP");
		// 			MessShow(m_LogString);
		iFilNo = iGnPmu.vecAmpl.viFilNo;
		iColNo = iGnPmu.vecAmpl.viColNo;

		memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
		if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
		if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
		{
			//                 sprintf(m_LogString, "GetGnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
			// 				MessShow(m_LogString);
							/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
							if( vviData )  {free(vviData ); vviData = NULL;}
							if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
							if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
							return false;*/
		}
		if (nNeedNum != nRealNum)
		{
			/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
			if( vviData )  {free(vviData ); vviData = NULL;}
			if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
			if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
			return false;*/
		}
		// 逐秒逐桢修复并统计无效桢
		for (jj = 0; jj < nVecSec; jj++)
		{
			memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
			IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvAmpl[0 * nNeedNum + jj * MAX_FRA_PER_SEC], vtmpSts);
			if (vtmpSts.nBadNum > 0)
			{
				vSecSts[jj].tBadSec = tBgnSec;

				if (vtmpSts.bPmuOff == 1)
				{
					vSecSts[jj].bPmuOff = 1;
					vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[0].viColNo = iColNo;
				}
				if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
				// 2008-07-16 by zhw
				if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
				{
					vSecSts[jj].nBadNum = vtmpSts.nBadNum;
					vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[1].viColNo = iColNo;
				}
				if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
				{
					vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
					vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[2].viColNo = iColNo;
				}
			}
		}
	}
	//  读取频率
	{
		iFilNo = iGnPmu.vecFreq.viFilNo;
		iColNo = iGnPmu.vecFreq.viColNo;

		memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
		if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
		if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
		{
			//             sprintf(m_LogString, "GetGnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
			// 			MessShow(m_LogString);
						/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
						if( vviData )  {free(vviData ); vviData = NULL;}
						if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
						if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
						return false;*/
		}
		if (nNeedNum != nRealNum)
		{
			/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
			if( vviData )  {free(vviData ); vviData = NULL;}
			if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
			if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
			return false;*/
		}
		// 逐秒逐桢修复并统计无效桢
		for (jj = 0; jj < nVecSec; jj++)
		{
			memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
			IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvFreq[jj * MAX_FRA_PER_SEC], vtmpSts);
			if (vtmpSts.nBadNum > 0)
			{
				vSecSts[jj].tBadSec = tBgnSec;

				if (vtmpSts.bPmuOff == 1)
				{
					vSecSts[jj].bPmuOff = 1;
					vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[0].viColNo = iColNo;
				}
				if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
				// 2008-07-16 by zhw
				if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
				{
					vSecSts[jj].nBadNum = vtmpSts.nBadNum;
					vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[1].viColNo = iColNo;
				}
				if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
				{
					vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
					vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[2].viColNo = iColNo;
				}
			}
		}
	}

	{
		// 		sprintf(m_LogString, "读取SSO");
		// 		MessShow(m_LogString);
		iFilNo = iGnPmu.vecSSO.viFilNo;
		iColNo = iGnPmu.vecSSO.viColNo;

		memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
		if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
		if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
		{
			//             sprintf(m_LogString, "GetGnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
			// 			MessShow(m_LogString);
						/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
						if( vviData )  {free(vviData ); vviData = NULL;}
						if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
						if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
						return false;*/
		}
		// 		if ( nNeedNum != nRealNum )
		// 		{
		// 			if( tmpData )  {free(tmpData);  tmpData = NULL;}
		// 			if( vviData )  {free(vviData ); vviData = NULL;}
		// 			if( vvEqa   )  {free(vvEqa  );  vvEqa   = NULL;}
		// 			if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
		// 			return false;
		// 		}
				// 逐秒逐桢修复并统计无效桢
		for (jj = 0; jj < nVecSec; jj++)
		{
			memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
			IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvStat[jj * MAX_FRA_PER_SEC], vtmpSts);
			if (vtmpSts.nBadNum > 0)
			{
				vSecSts[jj].tBadSec = tBgnSec;

				if (vtmpSts.bPmuOff == 1)
				{
					vSecSts[jj].bPmuOff = 1;
					vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[0].viColNo = iColNo;
				}
				if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
				// 2008-07-16 by zhw
				if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
				{
					vSecSts[jj].nBadNum = vtmpSts.nBadNum;
					vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[1].viColNo = iColNo;
				}
				if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
				{
					vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
					vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
					vSecSts[jj].VecBadData[2].viColNo = iColNo;
				}
			}
		}
	}

	//  填充有功、内电势、相角（使用A相电压相角）
	int     nOffPos = nOffSec * MAX_FRA_PER_SEC;
	for (jj = 0; jj < nNeedNum; jj++)
	{
		// 		if ( para_CalcPQVByUI == 1 )                            ////  通过ui计算P
		// 		{
		// 			//  根据三相电压电流实采量计算有功
		// 			oGnDat.pwr[ nOffPos+jj ]  = (float)(vviData[ 6*nNeedNum+jj ] * vviData[0*nNeedNum+jj ] * DP_Cos(vviData[ 7*nNeedNum+jj ] - vviData[1*nNeedNum+jj ])) / VF_VOLTBASE / VF_VOLTBASE;
		// 			oGnDat.pwr[ nOffPos+jj ] += (float)(vviData[ 8*nNeedNum+jj ] * vviData[2*nNeedNum+jj ] * DP_Cos(vviData[ 9*nNeedNum+jj ] - vviData[3*nNeedNum+jj ])) / VF_VOLTBASE / VF_VOLTBASE;
		// 			oGnDat.pwr[ nOffPos+jj ] += (float)(vviData[10*nNeedNum+jj ] * vviData[4*nNeedNum+jj ] * DP_Cos(vviData[11*nNeedNum+jj ] - vviData[5*nNeedNum+jj ])) / VF_VOLTBASE / VF_VOLTBASE;
		// 			oGnDat.pwr[ nOffPos+jj ] /= 1000;  // KW转为MW
		// 		}
		// 		else                                                    ////  直接获取 P uaa
		{
			oGnDat.ampl[nOffPos + jj] = vvAmpl[0 * nNeedNum + jj];
			oGnDat.freq[nOffPos + jj] = vvFreq[0 * nNeedNum + jj];
			oGnDat.sso[nOffPos + jj] = vvStat[0 * nNeedNum + jj] < 0.01 ? 0 : 1;
		}

		//  有功零漂处理
//         if( DP_ABS( oGnDat.ampl[ nOffPos+jj ] ) <= 1.0f ) oGnDat.ampl[ nOffPos+jj ] = 0.0f;
// 
//         oGnDat.ang[nOffPos + jj ] = vvEqa[   jj ];  // 2008-08-14
// 		oGnDat.u1a[nOffPos + jj ] = vviData[ 7*nNeedNum+jj ];
	}

	//设置数据秒状态
	for (jj = 0; jj < nVecSec; jj++)
	{
		oGnDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_OK;
		if (vSecSts[jj].bPmuOff == 1)  // 通道退出
		{
			oGnDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_OFF;
			oGnDat.VecBadData[0].viFilNo = vSecSts[jj].VecBadData[0].viFilNo;   // 2008-07-16 by zhw
			oGnDat.VecBadData[0].viColNo = vSecSts[jj].VecBadData[0].viColNo;
			continue;
		}
		// 2008-07-01 by zhw at xb
		if (IsGnStsValid(vSecSts[jj]) == false)  // 根据质量码判断，数据不可用
		{
			oGnDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_BAD;
			oGnDat.VecBadData[1].viFilNo = vSecSts[jj].VecBadData[1].viFilNo;   // 2008-07-16 by zhw
			oGnDat.VecBadData[1].viColNo = vSecSts[jj].VecBadData[1].viColNo;
			oGnDat.VecBadData[2].viFilNo = vSecSts[jj].VecBadData[2].viFilNo;
			oGnDat.VecBadData[2].viColNo = vSecSts[jj].VecBadData[2].viColNo;
		}
	}

	if (tmpData) { free(tmpData);  tmpData = NULL; }
	if (vvFreq) { free(vvFreq);  vvFreq = NULL; }
	if (vvAmpl) { free(vvAmpl);  vvAmpl = NULL; }
	if (vvStat) { free(vvStat);  vvStat = NULL; }
	if (vSecSts) { free(vSecSts);  vSecSts = NULL; }

	return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 获取线路PMU量测数据
//  处理过程 : 
//  备    注 : 通过三相电压电流计算功率
//  返 回 值 : 取数成功返回true,取数失败返回false
//  参数说明 : iLnPmu                   相量文件号列号
//             tBgnSec                  取数时刻
//             nVecSec                  取数秒数
//             oLnDat                   返回数据
//             nOffSec                  偏移秒
////////////////////////////////////////////////////////////////////////////////
bool CLfoAnaObj::GetLnDataFromWAMAP(SSO_LN_PMU iLnPmu, time_t tBgnSec, int nVecSec, SSO_LN_DAT& oLnDat, int nOffSec)
{
#ifndef _WAMAP_
	return true;
#else
	int              ii, jj;
#ifndef _D5000
	short            iFilNo = -1;
	short            iColNo = -1;
#else
	int            iFilNo = -1;
	int            iColNo = -1;
#endif
	int              nRealNum = 0;

	int              nNeedNum = 0;
	int              vnData = 0;
	float* vviData = NULL;
	float* vvAmpl = NULL;
	float* vvFreq = NULL;
	float* vvStat = NULL;
	PUB_WT_VEC_DATA* tmpData = NULL;
	WT_VEC_DESC      desc;

	VEC_DATA_STS    vtmpSts;
	VEC_DATA_STS* vSecSts = NULL;

	if (tBgnSec <= 0 || nVecSec <= 0 || nOffSec < 0) return false;
	/*
		if ( g_IWMDb->GetStatus() == false )
		{
			sprintf(m_LogString, "GetLnDataFromWAMAP(): %s", g_IWMDb->GetError() );
			MessShow(m_LogString);
			return false;
		}*/


		//  三相电压电流
	nNeedNum = nVecSec * MAX_FRA_PER_SEC;
	tmpData = (PUB_WT_VEC_DATA*)calloc(nNeedNum, sizeof(PUB_WT_VEC_DATA));
	memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));

	vnData = VEC_VI_NUM * nNeedNum;
	vviData = (float*)calloc(vnData, sizeof(float));
	memset((void*)vviData, 0, vnData * sizeof(float));

	vvAmpl = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vvAmpl, 0, nNeedNum * sizeof(float));

	vvFreq = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vvFreq, 0, nNeedNum * sizeof(float));

	vvStat = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vvStat, 0, nNeedNum * sizeof(float));

	vSecSts = (VEC_DATA_STS*)calloc(nVecSec, sizeof(VEC_DATA_STS));
	memset((void*)vSecSts, 0, nVecSec * sizeof(VEC_DATA_STS));

	// 	if ( para_CalcPQVByUI == 1 && m_DispCtl.dsource == MENU_LFO_DATA_RT )                            ////  通过ui计算P
	// 	{
	// 		//  读取三相电压电流
	// 		for( ii = 0; ii < VEC_VI_NUM; ++ii )
	// 		{
	// 			// 是否末端
	// 			if ( iLnPmu.is_end == MENU_LFO_MENU_YES )
	// 			{
	// 				iFilNo = iLnPmu.vecVI2.viFilNo[ii];
	// 				iColNo = iLnPmu.vecVI2.viColNo[ii];
	// 			}
	//             else
	// 			{
	// 				iFilNo = iLnPmu.vecVI1.viFilNo[ii];
	// 				iColNo = iLnPmu.vecVI1.viColNo[ii];
	// 			}
	// 
	// 			memset((void *)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
	// #ifndef _D5000
	//             if ( PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false )
	// #else
	//             if ( PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false )
	// #endif
	// 			{
	// //                 sprintf(m_LogString, "GetLnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
	// // 			    MessShow(m_LogString);
	// 				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
	// 				if( vviData )  {free(vviData ); vviData = NULL;}
	// 				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
	// 				return false;*/
	// 			}
	// 			if ( nNeedNum != nRealNum )
	// 			{
	// 				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
	// 				if( vviData )  {free(vviData ); vviData = NULL;}
	// 				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
	// 				return false;*/
	// 			}
	// 			// 逐秒逐桢修复并统计无效桢
	// 			for ( jj = 0; jj < nVecSec; jj++ )
	// 			{
	// 				memset((void *)&vtmpSts, 0, sizeof(VEC_DATA_STS));
	// 				IWM_PmuDataCheck( (PUB_WT_VEC_DATA *)&tmpData[jj*MAX_FRA_PER_SEC], tBgnSec+jj, (float*)&vviData[ii*nNeedNum + jj*MAX_FRA_PER_SEC], vtmpSts );
	// 				if( vtmpSts.nBadNum > 0 )
	// 				{
	// 					vSecSts[jj].tBadSec = tBgnSec;
	// 
	// 					if( vtmpSts.bPmuOff == 1 )
	//                     {
	//                         vSecSts[jj].bPmuOff = 1;
	//                         vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[0].viColNo = iColNo;
	//                     }
	// 					if( vtmpSts.iBadFra > vSecSts[jj].iBadFra ) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
	// 					if( vtmpSts.nBadNum > vSecSts[jj].nBadNum ) 
	//                     {
	//                         vSecSts[jj].nBadNum = vtmpSts.nBadNum;
	//                         vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[1].viColNo = iColNo;
	//                     }
	// 					if( vtmpSts.nBadSeq > vSecSts[jj].nBadSeq ) 
	//                     {
	//                         vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
	//                         vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[2].viColNo = iColNo;
	//                     }
	// 				}
	// 			}
	// 		}
	// 	}
	// 	else                                                    ////  直接获取 P
	{  //  读取 ampl 放于 vvAmpl[0]中
		{
			// 是否末端
			if (iLnPmu.is_end == MENU_LFO_MENU_YES)
			{
				iFilNo = iLnPmu.vecAmpl2.viFilNo;
				iColNo = iLnPmu.vecAmpl2.viColNo;
			}
			else
			{
				iFilNo = iLnPmu.vecAmpl1.viFilNo;
				iColNo = iLnPmu.vecAmpl1.viColNo;
			}

			memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
			if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
			if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
			{
				//                 sprintf(m_LogString, "GetLnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
				// 			    MessShow(m_LogString);
								/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
								if( vviData )  {free(vviData ); vviData = NULL;}
								if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
								return false;*/
			}
			if (nNeedNum != nRealNum)
			{
				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
				if( vviData )  {free(vviData ); vviData = NULL;}
				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
				return false;*/
			}
			// 逐秒逐桢修复并统计无效桢
			for (jj = 0; jj < nVecSec; jj++)
			{
				memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
				IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvAmpl[0 * nNeedNum + jj * MAX_FRA_PER_SEC], vtmpSts);
				if (vtmpSts.nBadNum > 0)
				{
					vSecSts[jj].tBadSec = tBgnSec;

					if (vtmpSts.bPmuOff == 1)
					{
						vSecSts[jj].bPmuOff = 1;
						vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[0].viColNo = iColNo;
					}
					if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
					if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
					{
						vSecSts[jj].nBadNum = vtmpSts.nBadNum;
						vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[1].viColNo = iColNo;
					}
					if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
					{
						vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
						vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[2].viColNo = iColNo;
					}
				}
			}
		}
	}
	{  //  读取 freq 放于 vvFreq[0]中
		{
			// 是否末端
			if (iLnPmu.is_end == MENU_LFO_MENU_YES)
			{
				iFilNo = iLnPmu.vecFreq2.viFilNo;
				iColNo = iLnPmu.vecFreq2.viColNo;
			}
			else
			{
				iFilNo = iLnPmu.vecFreq1.viFilNo;
				iColNo = iLnPmu.vecFreq1.viColNo;
			}

			memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
			if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
			if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
			{
				//                 sprintf(m_LogString, "GetLnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
				// 			    MessShow(m_LogString);
								/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
								if( vviData )  {free(vviData ); vviData = NULL;}
								if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
								return false;*/
			}
			if (nNeedNum != nRealNum)
			{
				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
				if( vviData )  {free(vviData ); vviData = NULL;}
				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
				return false;*/
			}
			// 逐秒逐桢修复并统计无效桢
			for (jj = 0; jj < nVecSec; jj++)
			{
				memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
				IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvFreq[0 * nNeedNum + jj * MAX_FRA_PER_SEC], vtmpSts);
				if (vtmpSts.nBadNum > 0)
				{
					vSecSts[jj].tBadSec = tBgnSec;

					if (vtmpSts.bPmuOff == 1)
					{
						vSecSts[jj].bPmuOff = 1;
						vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[0].viColNo = iColNo;
					}
					if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
					if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
					{
						vSecSts[jj].nBadNum = vtmpSts.nBadNum;
						vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[1].viColNo = iColNo;
					}
					if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
					{
						vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
						vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[2].viColNo = iColNo;
					}
				}
			}
		}
	}
	{  //  读取 stat 放于 vvStat[0]中
		{
			// 是否末端
			if (iLnPmu.is_end == MENU_LFO_MENU_YES)
			{
				iFilNo = iLnPmu.vecSSO2.viFilNo;
				iColNo = iLnPmu.vecSSO2.viColNo;
			}
			else
			{
				iFilNo = iLnPmu.vecSSO1.viFilNo;
				iColNo = iLnPmu.vecSSO1.viColNo;
			}

			memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
			if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
			if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
			{
				//                 sprintf(m_LogString, "GetLnDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
				// 			    MessShow(m_LogString);
								/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
								if( vviData )  {free(vviData ); vviData = NULL;}
								if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
								return false;*/
			}
			if (nNeedNum != nRealNum)
			{
				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
				if( vviData )  {free(vviData ); vviData = NULL;}
				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
				return false;*/
			}
			// 逐秒逐桢修复并统计无效桢
			for (jj = 0; jj < nVecSec; jj++)
			{
				memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
				IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvStat[0 * nNeedNum + jj * MAX_FRA_PER_SEC], vtmpSts);
				if (vtmpSts.nBadNum > 0)
				{
					vSecSts[jj].tBadSec = tBgnSec;

					if (vtmpSts.bPmuOff == 1)
					{
						vSecSts[jj].bPmuOff = 1;
						vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[0].viColNo = iColNo;
					}
					if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
					if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
					{
						vSecSts[jj].nBadNum = vtmpSts.nBadNum;
						vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[1].viColNo = iColNo;
					}
					if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
					{
						vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
						vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[2].viColNo = iColNo;
					}
				}
			}
		}
	}

	//  填充有功
	int     nOffPos = nOffSec * MAX_FRA_PER_SEC;
	for (jj = 0; jj < nNeedNum; jj++)
	{
		// 		if ( para_CalcPQVByUI == 1 && m_DispCtl.dsource == MENU_LFO_DATA_RT )                            ////  通过ui计算P
		// 		{
		// 			//  根据三相电压电流实采量计算有功
		// 			oLnDat.pwr[ nOffPos+jj ]  = (float)(vviData[ 6*nNeedNum+jj ] * vviData[0*nNeedNum+jj ] * DP_Cos(vviData[ 7*nNeedNum+jj ] - vviData[1*nNeedNum+jj ])) / VF_VOLTBASE /VF_VOLTBASE;
		// 			oLnDat.pwr[ nOffPos+jj ] += (float)(vviData[ 8*nNeedNum+jj ] * vviData[2*nNeedNum+jj ] * DP_Cos(vviData[ 9*nNeedNum+jj ] - vviData[3*nNeedNum+jj ])) / VF_VOLTBASE /VF_VOLTBASE;
		// 			oLnDat.pwr[ nOffPos+jj ] += (float)(vviData[10*nNeedNum+jj ] * vviData[4*nNeedNum+jj ] * DP_Cos(vviData[11*nNeedNum+jj ] - vviData[5*nNeedNum+jj ])) / VF_VOLTBASE /VF_VOLTBASE;
		// 			oLnDat.pwr[ nOffPos+jj ] /= 1000;  // KW转为MW
		// 		}
		// 		else                                                    ////  直接获取 P
		{
			oLnDat.ampl[nOffPos + jj] = vvAmpl[0 * nNeedNum + jj];
			oLnDat.freq[nOffPos + jj] = vvFreq[0 * nNeedNum + jj];
			oLnDat.sso[nOffPos + jj] = vvStat[0 * nNeedNum + jj] < 0.01 ? 0 : 1;
		}
		//  有功零漂处理
//         if( DP_ABS( oLnDat.pwr[ nOffPos+jj ] ) <= 1.0f ) oLnDat.pwr[ nOffPos+jj ] = 0.0f;
	}

	//设置数据秒状态
	for (jj = 0; jj < nVecSec; jj++)
	{
		oLnDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_OK;
		if (vSecSts[jj].bPmuOff == 1)  // 通道退出
		{
			oLnDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_OFF;
			oLnDat.VecBadData[0].viFilNo = vSecSts[jj].VecBadData[0].viFilNo;
			oLnDat.VecBadData[0].viColNo = vSecSts[jj].VecBadData[0].viColNo;
			continue;
		}
		// 2008-07-01 by zhw at xb
		if (IsLnStsValid(vSecSts[jj]) == false)  // 根据质量码判断，数据不可用
		{
			oLnDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_BAD;
			oLnDat.VecBadData[1].viFilNo = vSecSts[jj].VecBadData[1].viFilNo;
			oLnDat.VecBadData[1].viColNo = vSecSts[jj].VecBadData[1].viColNo;
			oLnDat.VecBadData[2].viFilNo = vSecSts[jj].VecBadData[2].viFilNo;
			oLnDat.VecBadData[2].viColNo = vSecSts[jj].VecBadData[2].viColNo;
		}
	}

	if (tmpData) { free(tmpData); tmpData = NULL; }
	if (vviData) { free(vviData); vviData = NULL; }
	if (vvAmpl) { free(vvAmpl); vviData = NULL; }
	if (vvFreq) { free(vvFreq); vviData = NULL; }
	if (vvStat) { free(vvStat); vviData = NULL; }
	if (vSecSts) { free(vSecSts); vSecSts = NULL; }

	return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 获取变压器PMU量测数据
//  处理过程 : 
//  备    注 : 通过三相电压电流计算功率
//  返 回 值 : 取数成功返回true,取数失败返回false
//  参数说明 : iTrPmu                   相量文件号列号
//             tBgnSec                  取数时刻
//             nVecSec                  取数秒数
//             oTrDat                   返回数据
//             nOffSec                  偏移秒
////////////////////////////////////////////////////////////////////////////////
bool CLfoAnaObj::GetTrDataFromWAMAP(SSO_TR_PMU iTrPmu, time_t tBgnSec, int nVecSec, SSO_TR_DAT& oTrDat, int nOffSec)
{
#ifndef _WAMAP_
	return true;
#else
	int              ii, jj;
#ifndef _D5000
	short            iFilNo = -1;
	short            iColNo = -1;
#else
	int            iFilNo = -1;
	int            iColNo = -1;
#endif
	int              nRealNum = 0;

	int              nNeedNum = 0;
	int              vnData = 0;
	float* vviData = NULL;
	float* vvAmpl = NULL;
	float* vvFreq = NULL;
	float* vvStat = NULL;
	PUB_WT_VEC_DATA* tmpData = NULL;
	WT_VEC_DESC      desc;

	VEC_DATA_STS    vtmpSts;
	VEC_DATA_STS* vSecSts = NULL;

	if (tBgnSec <= 0 || nVecSec <= 0 || nOffSec < 0) return false;
	/*
		if ( g_IWMDb->GetStatus() == false )
		{
			sprintf(m_LogString, "GetLnDataFromWAMAP(): %s", g_IWMDb->GetError() );
			MessShow(m_LogString);
			return false;
		}*/


		//  三相电压电流
	nNeedNum = nVecSec * MAX_FRA_PER_SEC;
	tmpData = (PUB_WT_VEC_DATA*)calloc(nNeedNum, sizeof(PUB_WT_VEC_DATA));
	memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));

	vnData = VEC_VI_NUM * nNeedNum;
	vviData = (float*)calloc(vnData, sizeof(float));
	memset((void*)vviData, 0, vnData * sizeof(float));

	vviData = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vviData, 0, nNeedNum * sizeof(float));

	vvAmpl = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vvAmpl, 0, nNeedNum * sizeof(float));

	vvFreq = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vvFreq, 0, nNeedNum * sizeof(float));

	vvStat = (float*)calloc(nNeedNum, sizeof(float));
	memset((void*)vvStat, 0, nNeedNum * sizeof(float));

	vSecSts = (VEC_DATA_STS*)calloc(nVecSec, sizeof(VEC_DATA_STS));
	memset((void*)vSecSts, 0, nVecSec * sizeof(VEC_DATA_STS));

	// 	if ( para_CalcPQVByUI == 1 && m_DispCtl.dsource == MENU_LFO_DATA_RT )                            ////  通过ui计算P
	// 	{
	// 		//  读取三相电压电流
	// 		for( ii = 0; ii < VEC_VI_NUM; ++ii )
	// 		{
	// 			if ( iTrPmu.slct_wind_id == iTrPmu.high_wind_id )
	// 			{
	// 				iFilNo = iTrPmu.vecVI_high.viFilNo[ii];
	// 				iColNo = iTrPmu.vecVI_high.viColNo[ii];
	// 			}
	//             else if( iTrPmu.slct_wind_id == iTrPmu.mid_wind_id )
	// 			{
	// 				iFilNo = iTrPmu.vecVI_mid.viFilNo[ii];
	// 				iColNo = iTrPmu.vecVI_mid.viColNo[ii];
	// 			}
	//             else if( iTrPmu.slct_wind_id == iTrPmu.low_wind_id )
	//             {
	//                 iFilNo = iTrPmu.vecVI_low.viFilNo[ii];
	// 				iColNo = iTrPmu.vecVI_low.viColNo[ii];
	//             }
	// 
	// 			memset((void *)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
	// #ifndef _D5000
	//             if ( PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false )
	// #else
	//             if ( PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false )
	// #endif
	// 			{
	// //                 sprintf(m_LogString, "GetTrDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
	// // 			    MessShow(m_LogString);
	// 				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
	// 				if( vviData )  {free(vviData ); vviData = NULL;}
	// 				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
	// 				return false;*/
	// 			}
	// 			if ( nNeedNum != nRealNum )
	// 			{
	// 				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
	// 				if( vviData )  {free(vviData ); vviData = NULL;}
	// 				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
	// 				return false;*/
	// 			}
	// 			// 逐秒逐桢修复并统计无效桢
	// 			for ( jj = 0; jj < nVecSec; jj++ )
	// 			{
	// 				memset((void *)&vtmpSts, 0, sizeof(VEC_DATA_STS));
	// 				IWM_PmuDataCheck( (PUB_WT_VEC_DATA *)&tmpData[jj*MAX_FRA_PER_SEC], tBgnSec+jj, (float*)&vviData[ii*nNeedNum + jj*MAX_FRA_PER_SEC], vtmpSts );
	// 				if( vtmpSts.nBadNum > 0 )
	// 				{
	// 					vSecSts[jj].tBadSec = tBgnSec;
	// 
	// 					if( vtmpSts.bPmuOff == 1 )
	//                     {
	//                         vSecSts[jj].bPmuOff = 1;
	//                         vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[0].viColNo = iColNo;
	//                     }
	// 					if( vtmpSts.iBadFra > vSecSts[jj].iBadFra ) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
	// 					if( vtmpSts.nBadNum > vSecSts[jj].nBadNum ) 
	//                     {
	//                         vSecSts[jj].nBadNum = vtmpSts.nBadNum;
	//                         vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[1].viColNo = iColNo;
	//                     }
	// 					if( vtmpSts.nBadSeq > vSecSts[jj].nBadSeq ) 
	//                     {
	//                         vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
	//                         vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
	//                         vSecSts[jj].VecBadData[2].viColNo = iColNo;
	//                     }
	// 				}
	// 			}
	// 		}
	// 	}
	// 	else                                                    ////  直接获取 P
	{  //  读取 AMLP 放于 vvAmpl[0]中
		{
			if (iTrPmu.slct_wind_id == iTrPmu.high_wind_id)
			{
				iFilNo = iTrPmu.vecAmpl_high.viFilNo;
				iColNo = iTrPmu.vecAmpl_high.viColNo;
			}
			else if (iTrPmu.slct_wind_id == iTrPmu.mid_wind_id)
			{
				iFilNo = iTrPmu.vecAmpl_mid.viFilNo;
				iColNo = iTrPmu.vecAmpl_mid.viColNo;
			}
			else if (iTrPmu.slct_wind_id == iTrPmu.low_wind_id)
			{
				iFilNo = iTrPmu.vecAmpl_low.viFilNo;
				iColNo = iTrPmu.vecAmpl_low.viColNo;
			}

			memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
			if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
			if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
			{
				//                 sprintf(m_LogString, "GetTrDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
				// 			    MessShow(m_LogString);
								/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
								if( vviData )  {free(vviData ); vviData = NULL;}
								if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
								return false;*/
			}
			if (nNeedNum != nRealNum)
			{
				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
				if( vviData )  {free(vviData ); vviData = NULL;}
				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
				return false;*/
			}
			// 逐秒逐桢修复并统计无效桢
			for (jj = 0; jj < nVecSec; jj++)
			{
				memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
				IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvAmpl[0 * nNeedNum + jj * MAX_FRA_PER_SEC], vtmpSts);
				if (vtmpSts.nBadNum > 0)
				{
					vSecSts[jj].tBadSec = tBgnSec;

					if (vtmpSts.bPmuOff == 1)
					{
						vSecSts[jj].bPmuOff = 1;
						vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[0].viColNo = iColNo;
					}
					if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
					if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
					{
						vSecSts[jj].nBadNum = vtmpSts.nBadNum;
						vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[1].viColNo = iColNo;
					}
					if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
					{
						vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
						vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[2].viColNo = iColNo;
					}
				}
			}
		}
	}

	{  //  读取 Freq 放于 vvFreq[0]中
		{
			if (iTrPmu.slct_wind_id == iTrPmu.high_wind_id)
			{
				iFilNo = iTrPmu.vecFreq_high.viFilNo;
				iColNo = iTrPmu.vecFreq_high.viColNo;
			}
			else if (iTrPmu.slct_wind_id == iTrPmu.mid_wind_id)
			{
				iFilNo = iTrPmu.vecFreq_mid.viFilNo;
				iColNo = iTrPmu.vecFreq_mid.viColNo;
			}
			else if (iTrPmu.slct_wind_id == iTrPmu.low_wind_id)
			{
				iFilNo = iTrPmu.vecFreq_low.viFilNo;
				iColNo = iTrPmu.vecFreq_low.viColNo;
			}

			memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
			if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
			if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
			{
				//                 sprintf(m_LogString, "GetTrDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
				// 			    MessShow(m_LogString);
								/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
								if( vviData )  {free(vviData ); vviData = NULL;}
								if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
								return false;*/
			}
			if (nNeedNum != nRealNum)
			{
				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
				if( vviData )  {free(vviData ); vviData = NULL;}
				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
				return false;*/
			}
			// 逐秒逐桢修复并统计无效桢
			for (jj = 0; jj < nVecSec; jj++)
			{
				memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
				IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvFreq[0 * nNeedNum + jj * MAX_FRA_PER_SEC], vtmpSts);
				if (vtmpSts.nBadNum > 0)
				{
					vSecSts[jj].tBadSec = tBgnSec;

					if (vtmpSts.bPmuOff == 1)
					{
						vSecSts[jj].bPmuOff = 1;
						vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[0].viColNo = iColNo;
					}
					if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
					if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
					{
						vSecSts[jj].nBadNum = vtmpSts.nBadNum;
						vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[1].viColNo = iColNo;
					}
					if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
					{
						vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
						vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[2].viColNo = iColNo;
					}
				}
			}
		}
	}

	{  //  读取 stat 放于 vvStat[0]中
		{
			if (iTrPmu.slct_wind_id == iTrPmu.high_wind_id)
			{
				iFilNo = iTrPmu.vecSSO_high.viFilNo;
				iColNo = iTrPmu.vecSSO_high.viColNo;
			}
			else if (iTrPmu.slct_wind_id == iTrPmu.mid_wind_id)
			{
				iFilNo = iTrPmu.vecSSO_mid.viFilNo;
				iColNo = iTrPmu.vecSSO_mid.viColNo;
			}
			else if (iTrPmu.slct_wind_id == iTrPmu.low_wind_id)
			{
				iFilNo = iTrPmu.vecSSO_low.viFilNo;
				iColNo = iTrPmu.vecSSO_low.viColNo;
			}

			memset((void*)tmpData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
#ifndef _D5000
			if (PWD_TableGet(iFilNo, iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum, desc) == false)
#else
			if (PWD_TableGet(iColNo, tBgnSec, tBgnSec + nVecSec, tmpData, nRealNum) == false)
#endif
			{
				//                 sprintf(m_LogString, "GetTrDataFromWAMAP(): %s, iFilNo = %d, iColNo = %d,", g_IWMDb->GetError(), iFilNo, iColNo);
				// 			    MessShow(m_LogString);
								/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
								if( vviData )  {free(vviData ); vviData = NULL;}
								if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
								return false;*/
			}
			if (nNeedNum != nRealNum)
			{
				/*if( tmpData )  {free(tmpData);  tmpData = NULL;}
				if( vviData )  {free(vviData ); vviData = NULL;}
				if( vSecSts )  {free(vSecSts);  vSecSts = NULL;}
				return false;*/
			}
			// 逐秒逐桢修复并统计无效桢
			for (jj = 0; jj < nVecSec; jj++)
			{
				memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
				IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[jj * MAX_FRA_PER_SEC], tBgnSec + jj, (float*)&vvStat[0 * nNeedNum + jj * MAX_FRA_PER_SEC], vtmpSts);
				if (vtmpSts.nBadNum > 0)
				{
					vSecSts[jj].tBadSec = tBgnSec;

					if (vtmpSts.bPmuOff == 1)
					{
						vSecSts[jj].bPmuOff = 1;
						vSecSts[jj].VecBadData[0].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[0].viColNo = iColNo;
					}
					if (vtmpSts.iBadFra > vSecSts[jj].iBadFra) vSecSts[jj].iBadFra = vtmpSts.iBadFra;
					if (vtmpSts.nBadNum > vSecSts[jj].nBadNum)
					{
						vSecSts[jj].nBadNum = vtmpSts.nBadNum;
						vSecSts[jj].VecBadData[1].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[1].viColNo = iColNo;
					}
					if (vtmpSts.nBadSeq > vSecSts[jj].nBadSeq)
					{
						vSecSts[jj].nBadSeq = vtmpSts.nBadSeq;
						vSecSts[jj].VecBadData[2].viFilNo = iFilNo;
						vSecSts[jj].VecBadData[2].viColNo = iColNo;
					}
				}
			}
		}
	}

	//  填充有功
	int     nOffPos = nOffSec * MAX_FRA_PER_SEC;
	for (jj = 0; jj < nNeedNum; jj++)
	{
		// 		if ( para_CalcPQVByUI == 1 && m_DispCtl.dsource == MENU_LFO_DATA_RT )                            ////  通过ui计算P
		// 		{
		// 			//  根据三相电压电流实采量计算有功
		// 			oTrDat.pwr[ nOffPos+jj ]  = (float)(vviData[ 6*nNeedNum+jj ] * vviData[0*nNeedNum+jj ] * DP_Cos(vviData[ 7*nNeedNum+jj ] - vviData[1*nNeedNum+jj ])) / VF_VOLTBASE /VF_VOLTBASE;
		// 			oTrDat.pwr[ nOffPos+jj ] += (float)(vviData[ 8*nNeedNum+jj ] * vviData[2*nNeedNum+jj ] * DP_Cos(vviData[ 9*nNeedNum+jj ] - vviData[3*nNeedNum+jj ])) / VF_VOLTBASE /VF_VOLTBASE;
		// 			oTrDat.pwr[ nOffPos+jj ] += (float)(vviData[10*nNeedNum+jj ] * vviData[4*nNeedNum+jj ] * DP_Cos(vviData[11*nNeedNum+jj ] - vviData[5*nNeedNum+jj ])) / VF_VOLTBASE /VF_VOLTBASE;
		// 			oTrDat.pwr[ nOffPos+jj ] /= 1000;  // KW转为MW
		// 		}
		// 		else                                                    ////  直接获取 P
		{
			oTrDat.ampl[nOffPos + jj] = vvAmpl[0 * nNeedNum + jj];
			oTrDat.freq[nOffPos + jj] = vvFreq[0 * nNeedNum + jj];
			oTrDat.sso[nOffPos + jj] = vvStat[0 * nNeedNum + jj] < 0.01 ? 0 : 1;
			// 			sprintf(m_LogString, "变压器数据[%d],[%f],[%f] 文件号[%d]",oTrDat.sso[ nOffPos + jj  ], oTrDat.ampl[ nOffPos + jj ], oTrDat.freq[ nOffPos + jj ],iColNo);
			// 			MessShow(m_LogString);
		}
		//  有功零漂处理
//         if( DP_ABS( oTrDat.pwr[ nOffPos+jj ] ) <= 1.0f ) oTrDat.pwr[ nOffPos+jj ] = 0.0f;
	}

	//设置数据秒状态
	for (jj = 0; jj < nVecSec; jj++)
	{
		oTrDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_OK;
		if (vSecSts[jj].bPmuOff == 1)  // 通道退出
		{
			oTrDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_OFF;
			oTrDat.VecBadData[0].viFilNo = vSecSts[jj].VecBadData[0].viFilNo;
			oTrDat.VecBadData[0].viColNo = vSecSts[jj].VecBadData[0].viColNo;
			continue;
		}
		// 2008-07-01 by zhw at xb
		if (IsTrStsValid(vSecSts[jj]) == false)  // 根据质量码判断，数据不可用
		{
			oTrDat.SecSts[nOffSec + jj] = MENU_LFO_DEV_STS_BAD;
			oTrDat.VecBadData[1].viFilNo = vSecSts[jj].VecBadData[1].viFilNo;
			oTrDat.VecBadData[1].viColNo = vSecSts[jj].VecBadData[1].viColNo;
			oTrDat.VecBadData[2].viFilNo = vSecSts[jj].VecBadData[2].viFilNo;
			oTrDat.VecBadData[2].viColNo = vSecSts[jj].VecBadData[2].viColNo;
		}
	}

	if (tmpData) { free(tmpData); tmpData = NULL; }
	if (vviData) { free(vviData); vviData = NULL; }
	if (vvAmpl) { free(vvAmpl); vviData = NULL; }
	if (vvFreq) { free(vvFreq); vviData = NULL; }
	if (vvStat) { free(vvStat); vviData = NULL; }
	if (vSecSts) { free(vSecSts); vSecSts = NULL; }

	return true;
#endif
}
//////////////////////////////////////////////////////////////////////////

///////////////////////////////  tmd接口  ///////////////////////////////
//  三态数据整合接口
// 功能:发送保存 LFO case信息，启动TMD记录PMU数据
//////////////////////////////////////////////////////////////////////////
bool CLfoAnaObj::SaveLfoCaseByTmd()
{
#ifndef _D5000
#ifdef  _WAMAP_
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)// 判断是否主机，主机发送消息
	{
		sprintf(m_LogString, "SaveLfoCaseByTmd(): 本机不是WAMAP主机,退出启动TMD信息发送!");
		MessShow(m_LogString);
		return false;
	}
#endif

#ifndef  USE_WEEK_WAM_MEM
	if (m_DispCtl.is_tmd == MENU_LFO_MENU_NO)
	{
		sprintf(m_LogString, "SaveLfoCaseByTmd(): m_DispCtl.is_tmd = %s[%d],退出启动TMD信息发送!", "否", MENU_LFO_MENU_NO);
		MessShow(m_LogString);
		return false;
	}
#else
	// 2008-05-19 by zhw 
	if (m_RunCtrCom != MENU_LFO_PRO_CTL_SAVE)
	{
		sprintf(m_LogString, "SaveLfoCaseByTmd(): 离线分析退出tmd保存!");
		MessShow(m_LogString);
		return true;
	}
#endif

	sprintf(m_LogString, "SaveLfoCaseByTmd(): 准备启动TMD，记录实测数据!");
	MessShow(m_LogString);

	int             ii, jj = 0;
	char            strTime1[28], strTime2[28];
	char            szString[256];
	KEY_ID_STRU     keyid;

	struct timeval  tvBgn, tvEnd;
	tvBgn.tv_sec = m_SysResSave.lfo_b_t;
	tvBgn.tv_usec = 0;
	tvEnd.tv_sec = m_SysResSave.lfo_e_t;
	tvEnd.tv_usec = 0;

	/*	//////////////////////////////////////////////////////////////////////////
	#if LFO_DEBUG_TMD == 1
	   // 测试tmd // 2008-01-05 lsf
	   time_t  temp_time;
	   time(&temp_time);
		tvBgn.tv_sec  = temp_time - 3*24*60*60;
		tvBgn.tv_usec = 0;
		tvEnd.tv_sec  = temp_time;
		tvEnd.tv_usec = 0;
	#endif
	*/
	//int retVal = init_sei_read_req();
	///+++++++++++++++++++++++++++++++++++
	//sprintf( m_LogString, "SaveLfoCaseByTmd(): 准备启动TMD，记录实测数据!" ); 
	//WriteLogFile( m_LogString );
	//////////////////////////////////////////////////////////////////////////

	 //检查case是否已经保存
	std::vector<sei::PUB_R_ITEM_STRU> vec_item;
	sei::LFO_R_REQ_STRU                    lfo_r_req;

#ifndef  _WINDOWS32
	int retVal = enum_lfo_item_from_hdb(tvBgn, tvEnd, vec_item);
	if (retVal >= 0)
	{
		for (ii = 0; ii < vec_item.size(); ++ii)
		{	// 案例重复判断
			if (read_lfo_prof_from_hdb(vec_item[ii].dir_id, vec_item[ii].data_id, lfo_r_req) >= 0)
				if (tvBgn.tv_sec <= lfo_r_req.beg_time_e.tv_sec - 1 && tvEnd.tv_sec >= lfo_r_req.end_time_e.tv_sec + 1)
				{
					sprintf(m_LogString, "CLFOMonitorObj::SaveLfoCaseByTmd() -> read_lfo_prof_from_hdb(): the Case is exist");
					MessShow(m_LogString);
					return true;
				}
		}
	}
#endif

	tmd::LFO_S_REQ_STRU                   lfo_s_req;

	lfo_s_req.beg_time_e.tv_sec = tvBgn.tv_sec;//  LFO发生时间
	lfo_s_req.beg_time_e.tv_usec = tvBgn.tv_usec;
	lfo_s_req.end_time_e.tv_sec = tvEnd.tv_sec;//  LFO结束时间
	lfo_s_req.end_time_e.tv_usec = tvEnd.tv_usec;

	lfo_s_req.beg_time_r.tv_sec = tvBgn.tv_sec - m_LfoPara.rcd_pre_t;//  开始数据记录的时间
	lfo_s_req.beg_time_r.tv_usec = tvBgn.tv_usec;
	lfo_s_req.end_time_r.tv_sec = tvEnd.tv_sec + m_LfoPara.rcd_aft_t;//  结束数据记录的时间
	lfo_s_req.end_time_r.tv_usec = tvEnd.tv_usec;

	//if (m_SysResSave.obj_id/LFO_DB_BASE == ACLN_DOT_NO)//  标题内容，作为案例名字的一部分
	{
		lfo_s_req.c_title = m_SysResSave.obj_name;
		lfo_s_req.c_title.append("低频振荡");
	}
	//////////////////////////////////////////////////////////////////////////
	// 2008-04-05 added by zhw at yn
	// 2008-04-29 modified by zhw at nj for displaying damp curve (hd)
	// 2008-10-24 南网同步三区，文件太大，封
	if (para_IsShowAnaData == 1)
	{
		lfo_s_req.e_res_d = "[START]\n";
		sprintf(szString, "版本号:%10.1f\n", 2.0);  //版本号
		lfo_s_req.e_res_d.append(szString);
		if (m_SysResSave.obj_id / DB_TABLE_BASE == ACLN_DEVICE_NO)  //线路
		{
			for (ii = 0; ii < m_nLnNum; ii++)
			{
				if (m_SysResSave.obj_id == m_pLnPmu[ii].acln_id)
				{
					if (m_pLnPmu[ii].is_end == MENU_LFO_MENU_YES)
					{
						sprintf(szString, "设备ID号:%10d\n", m_pLnPmu[ii].dot_id_2);  //触发设备ID末端
						lfo_s_req.e_res_d.append(szString);
					}
					else
					{
						sprintf(szString, "设备ID号:%10d\n", m_pLnPmu[ii].dot_id_1);  //触发设备ID首端
						lfo_s_req.e_res_d.append(szString);
					}
					break;
				}
			}
		}
		else                                                  //发电机
		{
			sprintf(szString, "设备ID号:%10d\n", m_SysResSave.obj_id);  //触发设备ID
			lfo_s_req.e_res_d.append(szString);
		}

		// 振荡线路个数(%4d)，振荡机组个数(%4d)，时间间隔(%4d)
		sprintf(szString, "振荡线路个数:%4d\n", m_SysResSave.lfo_line_num);  //振荡线路数
		lfo_s_req.e_res_d.append(szString);
		sprintf(szString, "振荡机组个数:%4d\n", m_SysResSave.act_gen_num);  //振荡机组数
		lfo_s_req.e_res_d.append(szString);
		sprintf(szString, "时间间隔:%4d\n", m_LfoPara.time_leng - m_LfoPara.int_time);  //时间间隔
		lfo_s_req.e_res_d.append(szString);

		//振荡线路信息
		if (m_SysResSave.lfo_line_num > 0)
		{
			for (ii = 0; ii < m_nLnNum; ii++)
			{
				if (m_pLnResSave[ii].lfo_b_t > 0 && m_pLnResSave[ii].lfo_e_t > 0)
				{
					// 设备名称:(%64s)；设备ID:(%10d).
					// 线端ID号:(%10d)
					// 起始时间:
					// 消失时间：
					// 点数: 
					sprintf(szString, "设备名称:%s;设备ID:%10d.\n", m_pLnResSave[ii].obj_name, m_pLnResSave[ii].obj_id);
					lfo_s_req.e_res_d.append(szString);
					if (m_pLnPmu[ii].is_end == MENU_LFO_MENU_YES)
					{
						sprintf(szString, "设备ID:%10d\n", m_pLnPmu[ii].dot_id_2);  //触发设备ID末端
						lfo_s_req.e_res_d.append(szString);
					}
					else
					{
						sprintf(szString, "设备ID:%10d\n", m_pLnPmu[ii].dot_id_1);  //触发设备ID首端
						lfo_s_req.e_res_d.append(szString);
					}
					sprintf(szString, "起始时间:%d\n", m_pLnResSave[ii].lfo_b_t);
					lfo_s_req.e_res_d.append(szString);
					sprintf(szString, "消失时间:%d\n", m_pLnResSave[ii].lfo_e_t);
					lfo_s_req.e_res_d.append(szString);
					sprintf(szString, "点数:%4d\n", m_pLnResSave[ii].mode1_num);
					lfo_s_req.e_res_d.append(szString);
					for (jj = 0; jj < m_pLnResSave[ii].mode1_num; jj++)
					{
						//起始时间(time_t);截止时间(time_t);频率(%8.4f);	阻尼比(%8.4f);幅值(%8.4f)
						sprintf(szString, "%d;%d;%8.4f;%8.4f;%8.4f.\n", m_pLnResSave[ii].mode1[jj].bgn_t, m_pLnResSave[ii].mode1[jj].end_t, \
							m_pLnResSave[ii].mode1[jj].freq, m_pLnResSave[ii].mode1[jj].damp, m_pLnResSave[ii].mode1[jj].ampl);
						lfo_s_req.e_res_d.append(szString);
					}
				}

			}
		}

		//振荡机组信息
		if (m_SysResSave.act_gen_num > 0)
		{
			for (ii = 0; ii < m_nGnNum; ii++)
			{
				if (m_pGnResSave[ii].lfo_b_t > 0 && m_pGnResSave[ii].lfo_e_t > 0)
				{
					// 设备名称:(%64s)；设备ID:(%10d).
					// 起始时间:
					// 消失时间：
					// 点数: 
					sprintf(szString, "设备名称:%s;设备ID:%10d.\n", m_pGnResSave[ii].obj_name, m_pGnResSave[ii].obj_id);
					lfo_s_req.e_res_d.append(szString);
					sprintf(szString, "起始时间:%d\n", m_pGnResSave[ii].lfo_b_t);
					lfo_s_req.e_res_d.append(szString);
					sprintf(szString, "消失时间:%d\n", m_pGnResSave[ii].lfo_e_t);
					lfo_s_req.e_res_d.append(szString);
					sprintf(szString, "点数:%4d\n", m_pGnResSave[ii].mode1_num);
					lfo_s_req.e_res_d.append(szString);
					for (jj = 0; jj < m_pGnResSave[ii].mode1_num; jj++)
					{
						//起始时间(time_t);截止时间(time_t);频率(%8.4f);	阻尼比(%8.4f);幅值(%8.4f)
						sprintf(szString, "%d;%d;%8.4f;%8.4f;%8.4f.\n", m_pGnResSave[ii].mode1[jj].bgn_t, m_pGnResSave[ii].mode1[jj].end_t, \
							m_pGnResSave[ii].mode1[jj].freq, m_pGnResSave[ii].mode1[jj].damp, m_pGnResSave[ii].mode1[jj].ampl);
						lfo_s_req.e_res_d.append(szString);
					}
				}
			}
		}

		sprintf(szString, "[END]\n");  //结束
		lfo_s_req.e_res_d.append(szString);
	}
	//lfo_s_req.e_res_d = "分析结果详细信息";               //  分析结果详细信息,待补充
	//////////////////////////////////////////////////////////////////////////
	//  需增加整体描述
	lfo_s_req.e_res_s = "<p align=\"center\"><b><font size=\"4\">低频振荡在线分析</font></b></p>";

	sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_SysResSave.lfo_b_t));
	sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_SysResSave.lfo_e_t));

	sprintf(szString, "<p align=\"left\">发生时间：  %s&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "
		"消失时间:   %s&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 持续时间:   %s</p>",
		strTime1, strTime2, PT_GetStringOfTimeInterval(m_SysResSave.hold_time * 1000));
	lfo_s_req.e_res_s.append(szString);

	sprintf(szString, "<p align=\"left\">触发设备：  %s&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;振荡频率:   %6.3fHz</p>",
		m_SysResSave.obj_name, m_SysResSave.freq);
	lfo_s_req.e_res_s.append(szString);

	sprintf(szString, "<p align=\"left\">振荡线路数：%d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;振荡机组数: %d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;振荡变压数：%d</p>",
		m_SysResSave.lfo_line_num, m_SysResSave.act_gen_num, m_SysResSave.lfo_tr_num);
	lfo_s_req.e_res_s.append(szString);
	//////////////////////////////////////////////////////////////////////////
	int             nGnNum = 0;
	char            fac_name[64];
	//  告警机组
	if (m_SysResSave.act_gen_num > 0)
	{
		lfo_s_req.e_res_s.append("<p align=\"center\">&nbsp;振荡机组</p>");

		lfo_s_req.e_res_s.append("<div align=\"center\">");
		lfo_s_req.e_res_s.append("<center>");

		lfo_s_req.e_res_s.append("<table border=\"1\" id=\"table1\"><tr>");
		lfo_s_req.e_res_s.append("<td align=\"center\" valign=\"center\"><b>序号</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\" valign=\"center\"><b>线路名称</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>告警时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>消失时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>持续时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>主导频率</b><p><b>(Hz)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>告警前均值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>消失后均值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>最大值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>对应时刻</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>最小值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>对应时刻</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>峰峰值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("</tr>");

		for (ii = 0; ii < m_nGnNum; ii++)
		{
			if (m_pGnResSave[ii].lfo_b_t > 0 && m_pGnResSave[ii].lfo_e_t > 0)
			{
				memset(fac_name, 0, 64);
				lfo_s_req.e_res_s.append("<tr>");

				sprintf(szString, "<td align=\"center\">%d</td>", ++nGnNum);
				lfo_s_req.e_res_s.append(szString);
				for (jj = 0; jj < m_nGnNum; jj++)
				{
					if (m_pGnResSave[ii].obj_id == m_pGnInf[jj].gn_id)
					{
						strcpy(fac_name, m_pGnInf[jj].fac_name);
						break;
					}
				}
				sprintf(szString, "<td>%s%s</td>", fac_name, m_pGnResSave[ii].obj_name);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pGnResSave[ii].lfo_b_t));
				sprintf(szString, "<td>%s</td>", strTime1);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pGnResSave[ii].lfo_e_t));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetStringOfTimeInterval(m_pGnResSave[ii].hold_time * 1000));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pGnResSave[ii].mode1[0].freq);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pGnResSave[ii].b_aver_pqv0[0]);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pGnResSave[ii].e_aver_pqv0[0]);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pGnResSave[ii].max_value.value);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pGnResSave[ii].max_value.time));
				sprintf(szString, "<td>%s</td>", strTime1);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pGnResSave[ii].min_value.value);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pGnResSave[ii].min_value.time));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", (m_pGnResSave[ii].max_min_dif * 2));
				lfo_s_req.e_res_s.append(szString);

				lfo_s_req.e_res_s.append("</tr>");
			}
		}

		lfo_s_req.e_res_s.append("</table>");

		lfo_s_req.e_res_s.append("</div>");
		lfo_s_req.e_res_s.append("</center>");
	}

	int             nLnNum = 0;
	//  告警线路
	if (m_SysResSave.lfo_line_num > 0)
	{
		lfo_s_req.e_res_s.append("<p align=\"center\">&nbsp;振荡线路</p>");

		lfo_s_req.e_res_s.append("<div align=\"center\">");
		lfo_s_req.e_res_s.append("<center>");

		lfo_s_req.e_res_s.append("<table border=\"1\" id=\"table2\"><tr>");
		lfo_s_req.e_res_s.append("<td align=\"center\" valign=\"center\"><b>序号</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\" valign=\"center\"><b>线路名称</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>告警时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>消失时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>持续时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>主导频率</b><p><b>(Hz)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>告警前均值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>消失后均值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>最大值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>对应时刻</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>最小值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>对应时刻</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>峰峰值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("</tr>");

		for (ii = 0; ii < m_nLnNum; ii++)
		{
			if (m_pLnResSave[ii].lfo_b_t > 0 && m_pLnResSave[ii].lfo_e_t > 0)
			{
				lfo_s_req.e_res_s.append("<tr>");

				sprintf(szString, "<td align=\"center\">%d</td>", ++nLnNum);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td>%s</td>", m_pLnResSave[ii].obj_name);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pLnResSave[ii].lfo_b_t));
				sprintf(szString, "<td>%s</td>", strTime1);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pLnResSave[ii].lfo_e_t));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetStringOfTimeInterval(m_pLnResSave[ii].hold_time * 1000));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLnResSave[ii].mode1[0].freq);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLnResSave[ii].b_aver_pqv0[0]);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLnResSave[ii].e_aver_pqv0[0]);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLnResSave[ii].max_value.value);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pLnResSave[ii].max_value.time));
				sprintf(szString, "<td>%s</td>", strTime1);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLnResSave[ii].min_value.value);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pLnResSave[ii].min_value.time));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", (m_pLnResSave[ii].max_min_dif * 2));
				lfo_s_req.e_res_s.append(szString);

				lfo_s_req.e_res_s.append("</tr>");
			}
		}

		lfo_s_req.e_res_s.append("</table>");

		lfo_s_req.e_res_s.append("</div>");
	}

	int             nTrNum = 0;
	//  告警变压器
	if (m_SysResSave.lfo_tr_num > 0)
	{
		lfo_s_req.e_res_s.append("<p align=\"center\">&nbsp;振荡变压器</p>");

		lfo_s_req.e_res_s.append("<div align=\"center\">");
		lfo_s_req.e_res_s.append("<center>");

		lfo_s_req.e_res_s.append("<table border=\"1\" id=\"table2\"><tr>");
		lfo_s_req.e_res_s.append("<td align=\"center\" valign=\"center\"><b>序号</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\" valign=\"center\"><b>变压器名称</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>告警时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>消失时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>持续时间</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>主导频率</b><p><b>(Hz)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>告警前均值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>消失后均值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>最大值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>对应时刻</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>最小值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>对应时刻</b></td>");
		lfo_s_req.e_res_s.append("<td align=\"center\"><b>峰峰值</b><p><b>(MW)</b></td>");
		lfo_s_req.e_res_s.append("</tr>");

		for (ii = 0; ii < m_nTrNum; ii++)
		{
			if (m_pTrResSave[ii].lfo_b_t > 0 && m_pTrResSave[ii].lfo_e_t > 0)
			{
				memset(fac_name, 0, 64);
				lfo_s_req.e_res_s.append("<tr>");

				sprintf(szString, "<td align=\"center\">%d</td>", ++nTrNum);
				lfo_s_req.e_res_s.append(szString);
				for (jj = 0; jj < m_nTrNum; jj++)
				{
					if (m_pTrResSave[ii].obj_id == m_pTrInf[jj].tr_id)
					{
						strcpy(fac_name, m_pTrInf[jj].fac_name);
						break;
					}
				}
				sprintf(szString, "<td>%s%s</td>", fac_name, m_pTrResSave[ii].obj_name);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pTrResSave[ii].lfo_b_t));
				sprintf(szString, "<td>%s</td>", strTime1);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pTrResSave[ii].lfo_e_t));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetStringOfTimeInterval(m_pTrResSave[ii].hold_time * 1000));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pTrResSave[ii].mode1[0].freq);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pTrResSave[ii].b_aver_pqv0[0]);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pTrResSave[ii].e_aver_pqv0[0]);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pTrResSave[ii].max_value.value);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pTrResSave[ii].max_value.time));
				sprintf(szString, "<td>%s</td>", strTime1);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pTrResSave[ii].min_value.value);
				lfo_s_req.e_res_s.append(szString);

				sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pTrResSave[ii].min_value.time));
				sprintf(szString, "<td>%s</td>", strTime2);
				lfo_s_req.e_res_s.append(szString);

				sprintf(szString, "<td align=\"center\">%6.2f</td>", (m_pTrResSave[ii].max_min_dif * 2));
				lfo_s_req.e_res_s.append(szString);

				lfo_s_req.e_res_s.append("</tr>");
			}
		}

		lfo_s_req.e_res_s.append("</table>");

		lfo_s_req.e_res_s.append("</div>");
	}
	//  2008-08-26 参与因子信息
	if (para_IsShowAnaRes == 1)
	{
		char  area_name[64];
		char  NameString[64];
		lfo_s_req.e_res_s.append("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">模式信息</font></b>]</p>");
		for (ii = 0; ii < m_nLfoMode; ii++)
		{
			if (m_pLfoMode[ii].nPartGn <= 0)
			{
				continue;
			}
			sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pLfoMode[ii].time_bgn));
			sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pLfoMode[ii].time_end));
			sprintf(szString, "<p align=\"left\"> %s ～ %s 主要模式信息</p>", strTime1, strTime2);
			lfo_s_req.e_res_s.append(szString);
			sprintf(szString, "<p align=\"left\">      频率=%6.3f  阻尼比=%6.3f</p>",
				//m_pLfoMode[ii].mode_id,
				m_pLfoMode[ii].freq,
				m_pLfoMode[ii].damp);
			lfo_s_req.e_res_s.append(szString);
			if (m_pLfoMode[ii].nPartGn > 0)
			{
				sprintf(szString, "<p align=\"left\">    参与机组数：%d</p>", m_pLfoMode[ii].nPartGn);
				lfo_s_req.e_res_s.append(szString);
				lfo_s_req.e_res_s.append("<table border=\"1\" id=\"table3\"><tr>");
				//lfo_s_req.e_res_s.append("<td align=\"center\" colspan=\"3\"><b>一端机组</b></td>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>机组名称</b></td>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>幅值</b></td>");
				//lfo_s_req.e_res_s.append("<td align=\"center\"><b>初相</b></td>");
				//lfo_s_req.e_res_s.append("<td align=\"center\"><b>实部</b></td>");
				//lfo_s_req.e_res_s.append("<td align=\"center\"><b>虚部</b></td>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>参与因子</b></td>");
				//lfo_s_req.e_res_s.append("<td align=\"center\"><b>能量</b></td>");
				//lfo_s_req.e_res_s.append("<td align=\"center\"><b>频率</b></td>");
				//lfo_s_req.e_res_s.append("<td align=\"center\"><b>阻尼比</b></td>");
				lfo_s_req.e_res_s.append("</tr>");

				for (jj = 0; jj < m_pLfoMode[ii].nPartGn; jj++)
				{
					memset(&area_name, 0, 64 * sizeof(char));
					memset(&NameString, 0, 64 * sizeof(char));
					for (int kk = 0; kk < m_nGnNum; kk++)
					{
						if (m_pLfoMode[ii].PartGn[jj].obj_id == m_pGnInf[kk].gn_id)
						{
							strcpy(area_name, m_pGnInf[kk].area_name);
							strcpy(NameString, m_pGnInf[kk].gn_name);
							break;
						}
					}

					sprintf(szString, "<td align=\"center\">%s</td>", NameString);
					lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartGn[jj].mgnt);
					lfo_s_req.e_res_s.append(szString);
					/*sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartGn[jj].phas*180/PRONY_PI);
					lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartGn[jj].real);
					lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartGn[jj].imag);
					lfo_s_req.e_res_s.append(szString);*/
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartGn[jj].fact);
					lfo_s_req.e_res_s.append(szString);
					/*sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartGn[jj].engy);
					lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartGn[jj].freq);
					lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartGn[jj].damp);
					lfo_s_req.e_res_s.append(szString);*/
					lfo_s_req.e_res_s.append("</tr>");
				}
				lfo_s_req.e_res_s.append("</table>");
				lfo_s_req.e_res_s.append("</div>");
			}

			if (m_pLfoMode[ii].nPartFc > 0)
			{
				sprintf(szString, "<p align=\"left\">    厂站信息：%d</p>", m_pLfoMode[ii].nPartFc);
				lfo_s_req.e_res_s.append(szString);

				lfo_s_req.e_res_s.append("<table border=\"1\" id=\"table4\"><tr>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>厂站名称</b></td>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>角度波动</b></td>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>参与因子</b></td>");
				lfo_s_req.e_res_s.append("</tr>");

				for (jj = 0; jj < m_pLfoMode[ii].nPartFc; jj++)
				{
					memset(&area_name, 0, 64 * sizeof(char));
					memset(&NameString, 0, 64 * sizeof(char));
					for (int kk = 0; kk < m_nFcNum; kk++)
					{
						if (m_pLfoMode[ii].PartFc[jj].obj_id == m_pFcInf[kk].fac_id)
						{
							strcpy(area_name, m_pFcInf[kk].area_name);
							strcpy(NameString, m_pFcInf[kk].fac_name);
							break;
						}
					}
					sprintf(szString, "<td align=\"center\">%s</td>", NameString);
					lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartFc[jj].ampl);
					lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].PartFc[jj].fact);
					lfo_s_req.e_res_s.append(szString);
					lfo_s_req.e_res_s.append("</tr>");
				}
				lfo_s_req.e_res_s.append("</table>");
				lfo_s_req.e_res_s.append("</div>");
			}

			if (m_pLfoMode[ii].nLfoCenter > 0)
			{
				sprintf(szString, "<p align=\"left\">    振荡中心厂站数：%d</p>", m_pLfoMode[ii].nLfoCenter);
				lfo_s_req.e_res_s.append(szString);

				lfo_s_req.e_res_s.append("<table border=\"1\" id=\"table4\"><tr>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>厂站名称</b></td>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>电压波动</b></td>");
				lfo_s_req.e_res_s.append("<td align=\"center\"><b>所属群</b></td>");
				lfo_s_req.e_res_s.append("</tr>");

				for (jj = 0; jj < m_pLfoMode[ii].nLfoCenter; jj++)
				{
					sprintf(szString, "<td align=\"center\">%s</td>", m_pLfoMode[ii].LfoCenter[jj].fac_name);
					lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<td align=\"center\">%6.2f</td>", m_pLfoMode[ii].LfoCenter[jj].v_ampl);
					lfo_s_req.e_res_s.append(szString);
					if (m_pLfoMode[ii].LfoCenter[jj].clus == MENU_LFO_CLUST_RMAIN)
					{
						lfo_s_req.e_res_s.append("<td align=\"center\">余下群</td>");
					}
					else if (m_pLfoMode[ii].LfoCenter[jj].clus == MENU_LFO_CLUST_CRTCL)
					{
						lfo_s_req.e_res_s.append("<td align=\"center\">临界群</td>");
					}
					else if (m_pLfoMode[ii].LfoCenter[jj].clus == MENU_LFO_CLUST_OPOST)
					{
						lfo_s_req.e_res_s.append("<td align=\"center\">相对群</td>");
					}
					else
					{
						lfo_s_req.e_res_s.append("<td align=\"center\">未匹配</td>");
					}
					lfo_s_req.e_res_s.append("</tr>");
				}
				lfo_s_req.e_res_s.append("</table>");
				lfo_s_req.e_res_s.append("</div>");
			}
		}
	}

	// 2008-09-09 by zhw
	// 辅助策略信息
#ifdef _LFO_ASST_DECI_
	if (para_IsShowAsstDeci == 1)
	{
		if (GetAsstDeci() == true)
		{
			lfo_s_req.e_res_s.append("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">辅助决策</font></b>]</p>");
			lfo_s_req.e_res_s.append("<table border=\"1\" id=\"table4\"><tr>");
			lfo_s_req.e_res_s.append("<td align=\"center\"><b>关键设备</b></td>");
			lfo_s_req.e_res_s.append("<td align=\"center\"><b>控制措施描述</b></td>");
			lfo_s_req.e_res_s.append("<td align=\"center\"><b>策略等级</b></td>");
			lfo_s_req.e_res_s.append("</tr>");
			for (ii = 0; ii < m_nAsstDeci; ii++)
			{
				sprintf(szString, "<td align=\"center\">%s</td>", m_pAsstDeci[ii].key_dev_name);
				lfo_s_req.e_res_s.append(szString);
				sprintf(szString, "<td align=\"center\">%s</td>", m_pAsstDeci[ii].ctl_act_dsec);
				lfo_s_req.e_res_s.append(szString);
				if (m_pAsstDeci[ii].deci_type == MENU_LFO_DECI_GRAD1)
				{
					lfo_s_req.e_res_s.append("<td align=\"center\">一级策略</td>");
				}
				else if (m_pAsstDeci[ii].deci_type == MENU_LFO_DECI_GRAD2)
				{
					lfo_s_req.e_res_s.append("<td align=\"center\">二级策略</td>");
				}
				else if (m_pAsstDeci[ii].deci_type == MENU_LFO_DECI_GRAD3)
				{
					lfo_s_req.e_res_s.append("<td align=\"center\">三级策略</td>");
				}
				lfo_s_req.e_res_s.append("</tr>");
			}
			lfo_s_req.e_res_s.append("</table>");
			lfo_s_req.e_res_s.append("</div>");
		}
	}
#endif
	// 2008-06-04 by zhw at nj
	if (m_SysResSave.act_gen_num > 0)
	{
		lfo_s_req.e_res_s.append("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">振荡机组详细信息</font></b>]</p>");

		for (ii = 0; ii < m_nGnNum; ii++)
		{
			if (m_pGnResSave[ii].lfo_b_t > 0 && m_pGnResSave[ii].lfo_e_t > 0)
			{
				//if ((int)(m_pGnResSave[i].obj_id/DB_TABLE_BASE) == ACLN_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pGnResSave[ii].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pGnResSave[ii].lfo_e_t));
					for (jj = 0; jj < m_nGnNum; jj++)
					{
						if (m_pGnResSave[ii].obj_id == m_pGnInf[jj].gn_id)
						{
							strcpy(fac_name, m_pGnInf[jj].fac_name);
							break;
						}
					}
					sprintf(szString, "<p align=\"center\">%s主导模式</p>", m_pGnResSave[ii].obj_name);
					lfo_s_req.e_res_s.append(szString);

					lfo_s_req.e_res_s.append("<div align=\"center\"><table border=\"1\" id=\"table1\">");

					lfo_s_req.e_res_s.append("<tr><td align=\"center\">&nbsp;序号&nbsp; </td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;分析时段&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;幅值(MW)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;频率(Hz)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;阻尼比(%)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;相角(deg)&nbsp;</td></tr>");

					for (jj = 0; jj < m_pGnResSave[ii].mode1_num; jj++)
					{
						sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pGnResSave[ii].mode1[jj].bgn_t));
						sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pGnResSave[ii].mode1[jj].end_t));

						sprintf(szString, "<tr><td align=\"center\">%4d</td><td align=\"center\">%8s&nbsp;-&nbsp;%8s</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td></tr>",
							jj + 1,
							strTime1,
							strTime2,
							m_pGnResSave[ii].mode1[jj].ampl,
							m_pGnResSave[ii].mode1[jj].freq,
							m_pGnResSave[ii].mode1[jj].damp,
							m_pGnResSave[ii].mode1[jj].phase);
						lfo_s_req.e_res_s.append(szString);
					}
					lfo_s_req.e_res_s.append("</table></div>");
				}
			}
		}
	}
	if (m_SysResSave.lfo_line_num > 0)
	{
		lfo_s_req.e_res_s.append("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">振荡线路详细信息</font></b>]</p>");
		//sprintf(szString,"--------------------------------------------------------------------------------------------------------------------------------------------------------------<br>");
		//lfo_s_req.e_res_s.append(szString);
		for (ii = 0; ii < m_nLnNum; ii++)
		{
			if (m_pLnResSave[ii].lfo_b_t > 0 && m_pLnResSave[ii].lfo_e_t > 0)
			{
				//if ((int)(m_pLnResSave[i].obj_id/DB_TABLE_BASE) == ACLN_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pLnResSave[ii].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pLnResSave[ii].lfo_e_t));

					//sprintf(szString,"--------------------------------------------------------------------------------------------------------------------------------------------------------------<br>");					
					//lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<p align=\"center\">%s主导模式</p>", m_pLnResSave[ii].obj_name);
					lfo_s_req.e_res_s.append(szString);

					int LineModeNum = 0;
					LineModeNum = m_pLnResSave[ii].mode1_num;

					lfo_s_req.e_res_s.append("<div align=\"center\"><table border=\"1\" id=\"table2\">");

					lfo_s_req.e_res_s.append("<tr><td align=\"center\">&nbsp;序号&nbsp; </td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;分析时段&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;幅值(MW)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;频率(Hz)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;阻尼比(%)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;相角(deg)&nbsp;</td></tr>");

					for (jj = 0; jj < LineModeNum; jj++)
					{
						sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pLnResSave[ii].mode1[jj].bgn_t));
						sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pLnResSave[ii].mode1[jj].end_t));

						sprintf(szString, "<tr><td align=\"center\">%4d</td><td align=\"center\">%8s&nbsp;-&nbsp;%8s</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td></tr>",
							jj + 1,
							strTime1,
							strTime2,
							m_pLnResSave[ii].mode1[jj].ampl,
							m_pLnResSave[ii].mode1[jj].freq,
							m_pLnResSave[ii].mode1[jj].damp,
							m_pLnResSave[ii].mode1[jj].phase);
						lfo_s_req.e_res_s.append(szString);
					}
					lfo_s_req.e_res_s.append("</table></div>");
				}
			}
		}
	}
	if (m_SysResSave.lfo_tr_num > 0)
	{
		lfo_s_req.e_res_s.append("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">振荡变压器详细信息</font></b>]</p>");
		//sprintf(szString,"--------------------------------------------------------------------------------------------------------------------------------------------------------------<br>");
		//lfo_s_req.e_res_s.append(szString);
		for (ii = 0; ii < m_nTrNum; ii++)
		{
			if (m_pTrResSave[ii].lfo_b_t > 0 && m_pTrResSave[ii].lfo_e_t > 0)
			{
				//if ((int)(m_pLnResSave[i].obj_id/DB_TABLE_BASE) == ACLN_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pTrResSave[ii].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pTrResSave[ii].lfo_e_t));
					for (jj = 0; jj < m_nTrNum; jj++)
					{
						if (m_pTrResSave[ii].obj_id == m_pTrInf[jj].tr_id)
						{
							strcpy(fac_name, m_pTrInf[jj].fac_name);
							break;
						}
					}
					//sprintf(szString,"--------------------------------------------------------------------------------------------------------------------------------------------------------------<br>");					
					//lfo_s_req.e_res_s.append(szString);
					sprintf(szString, "<p align=\"center\">%s%s主导模式</p>", fac_name, m_pTrResSave[ii].obj_name);
					lfo_s_req.e_res_s.append(szString);

					int TrModeNum = 0;
					TrModeNum = m_pTrResSave[ii].mode1_num;

					lfo_s_req.e_res_s.append("<div align=\"center\"><table border=\"1\" id=\"table2\">");

					lfo_s_req.e_res_s.append("<tr><td align=\"center\">&nbsp;序号&nbsp; </td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;分析时段&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;幅值(MW)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;频率(Hz)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;阻尼比(%)&nbsp;</td>");
					lfo_s_req.e_res_s.append("<td align=\"center\">&nbsp;相角(deg)&nbsp;</td></tr>");

					for (jj = 0; jj < TrModeNum; jj++)
					{
						sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pTrResSave[ii].mode1[jj].bgn_t));
						sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pTrResSave[ii].mode1[jj].end_t));

						sprintf(szString, "<tr><td align=\"center\">%4d</td><td align=\"center\">%8s&nbsp;-&nbsp;%8s</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td></tr>",
							jj + 1,
							strTime1,
							strTime2,
							m_pTrResSave[ii].mode1[jj].ampl,
							m_pTrResSave[ii].mode1[jj].freq,
							m_pTrResSave[ii].mode1[jj].damp,
							m_pTrResSave[ii].mode1[jj].phase);
						lfo_s_req.e_res_s.append(szString);
					}
					lfo_s_req.e_res_s.append("</table></div>");
				}
			}
		}
	}
	// 厂站电压波动

	//  save pmu data
	 //  2008-04-06 added by zhw at yn
	 //  para_SaveAllFac == 1 tmd保存所有厂站数据
	 // para_SaveAllFac == 0 tmd 保存告警设备数据
	 //////////////////////////////////////////////////////////////////////////
	 //  Factory 厂站电压测点
#ifdef _WAMAP_
	int kk;
	vector<int> tmpVecCol;
	// UAV UAA UBV UBA UCV UCA V F
	PODB_InitColID(tmpVecCol, BS_DEVICE_NO_TAB);
	for (ii = 0; ii < m_nFcNum; ii++)
	{
		if (para_SaveAllFac == 0) break;  //  2008-04-06 added by zhw at yn
		if (m_pFcPmu[ii].vhisSts == MENU_LFO_DEV_STS_NOYC) continue;

		if (m_pFcPmu[ii].m_bs_vecV.viFilNo[0] >= 0 && m_pFcPmu[ii].m_bs_vecV.viColNo[0] >= 0)
		{
			keyid.record_id = m_pFcPmu[ii].m_bs_id;

			if (tmpVecCol.size() >= VEC_V_NUM)
			{
				// UAV UAA UBV UBA UCV UCA
				for (kk = 0; kk < VEC_V_NUM; kk++)
				{
					keyid.column_id = tmpVecCol[kk];
					lfo_s_req.vec_keyid.push_back(keyid);
				}
			}
		}
		if (m_pFcPmu[ii].v_bs_vecV.viFilNo[0] > 0 && m_pFcPmu[ii].v_bs_vecV.viColNo[0] > 0)
		{
			keyid.record_id = m_pFcPmu[ii].v_bs_id;

			if (tmpVecCol.size() >= VEC_V_NUM)
			{
				// UAV UAA UBV UBA UCV UCA
				for (kk = 0; kk < VEC_V_NUM; kk++)
				{
					keyid.column_id = tmpVecCol[kk];
					lfo_s_req.vec_keyid.push_back(keyid);
				}
			}
		}
	}

	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA F
	PODB_InitColID(tmpVecCol, GN_DEVICE_NO_TAB);
	//  generator 机组测点
	for (ii = 0; ii < m_nGnNum; ii++)
	{
		//  2008-04-06 added by zhw at yn
		if (para_SaveAllFac == 0 && m_pGnResSave[ii].lfo_b_t <= 0 \
			&& m_pGnResSave[ii].lfo_e_t <= 0) continue;

		if (m_pGnPmu[ii].hisSts == MENU_LFO_DEV_STS_NOYC) continue;

		if (m_pGnPmu[ii].vecPwr.viFilNo >= 0 && m_pGnPmu[ii].vecPwr.viColNo >= 0)
		{
			keyid.record_id = m_pGnPmu[ii].gn_id;

			if (tmpVecCol.size() >= VEC_VIP_NUM)
			{
				// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
				for (kk = 0; kk < VEC_VIP_NUM; kk++)
				{
					keyid.column_id = tmpVecCol[kk];
					lfo_s_req.vec_keyid.push_back(keyid);
				}
			}
		}
	}
	//
	//  line 线路测点
	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR F
	PODB_InitColID(tmpVecCol, ACLN_DOT_NO_TAB);
	for (ii = 0; ii < m_nLnNum; ii++)
	{
		//  2008-04-06 added by zhw at yn
		if (para_SaveAllFac == 0 && m_pLnResSave[ii].lfo_b_t <= 0 \
			&& m_pLnResSave[ii].lfo_e_t <= 0) continue;

		if (m_pLnPmu[ii].hisSts == MENU_LFO_DEV_STS_NOYC) continue;

		if (m_pLnPmu[ii].is_end == MENU_LFO_MENU_NO && m_pLnPmu[ii].vecPwr1.viFilNo >= 0 && m_pLnPmu[ii].vecPwr1.viColNo >= 0)
		{
			keyid.record_id = m_pLnPmu[ii].dot_id_1;

			if (tmpVecCol.size() >= 13)
			{
				// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
				for (kk = 0; kk < 13; kk++)
				{
					keyid.column_id = tmpVecCol[kk];
					lfo_s_req.vec_keyid.push_back(keyid);
				}
			}
		}

		if (m_pLnPmu[ii].is_end == MENU_LFO_MENU_YES && m_pLnPmu[ii].vecPwr2.viFilNo >= 0 && m_pLnPmu[ii].vecPwr2.viColNo >= 0)
		{
			keyid.record_id = m_pLnPmu[ii].dot_id_2;

			if (tmpVecCol.size() >= 13)
			{
				// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
				for (kk = 0; kk < 13; kk++)
				{
					keyid.column_id = tmpVecCol[kk];
					lfo_s_req.vec_keyid.push_back(keyid);
				}
			}
		}
	}
	//  变压器测点
	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR F
	PODB_InitColID(tmpVecCol, TRWD_DEVICE_NO_TAB);
	for (ii = 0; ii < m_nTrNum; ii++)
	{
		//  2008-04-06 added by zhw at yn
		if (para_SaveAllFac == 0 && m_pTrResSave[ii].lfo_b_t <= 0 \
			&& m_pTrResSave[ii].lfo_e_t <= 0) continue;

		if (m_pTrPmu[ii].hisSts == MENU_LFO_DEV_STS_NOYC) continue;

		if (m_pTrPmu[ii].vecPwr_high.viFilNo >= 0 && m_pTrPmu[ii].vecPwr_high.viColNo >= 0)
		{
			keyid.record_id = m_pTrPmu[ii].high_wind_id;

			if (tmpVecCol.size() >= 13)
			{
				// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
				for (kk = 0; kk < 13; kk++)
				{
					keyid.column_id = tmpVecCol[kk];
					lfo_s_req.vec_keyid.push_back(keyid);
				}
			}
		}

		if (m_pTrPmu[ii].vecVI_mid.viFilNo >= 0 && m_pTrPmu[ii].vecPwr_mid.viColNo >= 0)
		{
			keyid.record_id = m_pTrPmu[ii].mid_wind_id;
			if (tmpVecCol.size() >= 13)
			{
				// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
				for (kk = 0; kk < 13; kk++)
				{
					keyid.column_id = tmpVecCol[kk];
					lfo_s_req.vec_keyid.push_back(keyid);
				}
			}
		}

		if (m_pTrPmu[ii].vecVI_low.viFilNo >= 0 && m_pTrPmu[ii].vecPwr_low.viColNo >= 0)
		{
			keyid.record_id = m_pTrPmu[ii].low_wind_id;
			if (tmpVecCol.size() >= 13)
			{
				// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
				for (kk = 0; kk < 13; kk++)
				{
					keyid.column_id = tmpVecCol[kk];
					lfo_s_req.vec_keyid.push_back(keyid);
				}
			}
		}
	}
#endif
	//*/
#ifdef _WAMAP_
	sprintf(m_LogString, "SaveLfoCaseByTmd(): 准备调用 send_lfo_save_req()！");
	MessShow(m_LogString);
	//printf( ">>> SaveLfoCaseByTmd()：%s\n", lfo_s_req.e_res_s.c_str());
	std::vector<tmd::TBL_RW_REQ_STRU> vec_tbl_req;
	if (lfo_s_req.vec_keyid.size() > 0)
		if (send_lfo_save_req(m_MsgBus, lfo_s_req, vec_tbl_req) < 0)
		{
			sprintf(m_LogString, "SaveLfoCaseByTmd(): call send_lfo_save_req()  error");
			MessShow(m_LogString);
		}
#endif

	char  fileName[256];
	char  logStr[MAXBUF];
#ifdef _WINDOWS32
	sprintf(fileName, "%s\\res\\testTMDsave.html", m_RunInfDir);
#else
	sprintf(fileName, "%s/res/testTMDsave.html", m_RunInfDir);
#endif
	FILE* fp = fopen(fileName, "w+");
	if (!fp)
	{
		sprintf(m_LogString, "SaveLfoCaseByTmd():SaveResultFileTxt: file open error");
		MessShow(m_LogString);
		return true;
	}

	fprintf(fp, "%s\n", lfo_s_req.e_res_d.c_str()); //2008-04-05 added by zhw at yn
	fprintf(fp, "%s\n", lfo_s_req.e_res_s.c_str());
	if (lfo_s_req.vec_keyid.size() > 0)
	{
		fprintf(fp, "lfo_s_req.vec_keyid.size() = %d\n", lfo_s_req.vec_keyid.size());
		for (jj = 0; jj < lfo_s_req.vec_keyid.size(); ++jj)
		{
			fprintf(fp, "No.[%d]  %d  %d\n", jj + 1, lfo_s_req.vec_keyid[jj].record_id, lfo_s_req.vec_keyid[jj].column_id);
		}
	}

	fclose(fp);
#endif // _D5000
	return true;
}

/**************************************************************
* 功能：给离线程序发送最近一次振荡时间信息
* 参数：
* 返回：bool
***************************************************************/
bool CLfoAnaObj::SendHisLfoMess()
{
	return true;
	// #ifndef _WAMAP_
	//     return true;
	// #else
	// #ifndef  _WINDOWS32
	//     if(PODB_IsOnDuty(g_appNo, g_ctxNo) != 1 )// 判断是否主机，主机发送消息
	//     {
	//         sprintf( m_LogString, "SendHisLfoMess(): 本机不是WAMAP主机,退出消息发送!" ); 
	//         MessShow( m_LogString );
	//         return false;
	//     }
	// #endif
	//     
	// #ifdef  USE_WEEK_WAM_MEM
	//     sprintf( m_LogString, "SendHisLfoMess(): 离线分析退出消息发送!" ); 
	//     WriteLogFile( m_LogString );
	//     return true;
	// #endif
	//     
	//     int retValue ;
	//     char *msgSend ;
	//     int  sendLen ;
	//     
	//     sTHIS_LFO_INFO 	lfoinfo;
	//     
	//     memset((void *)&lfoinfo, 0, sizeof(sTHIS_LFO_INFO));
	//     
	//     lfoinfo.package_head.package_type = LFO_INFO; // 100 ;
	//     lfoinfo.package_head.data_num = 1 ;
	//     lfoinfo.lfo_info.length(1) ;
	//     lfoinfo.lfo_info[0].start_time = m_SysResSave.lfo_b_t;
	//     lfoinfo.lfo_info[0].end_time   = m_SysResSave.lfo_e_t;
	//     
	//     MARSHAL(lfoinfo, msgSend, sendLen);
	//     
	//     retValue = g_pMsgBus->SendMessageToChannel( msgSend, LFO_INFO, sendLen, WAMAP_LFO_REAL_HIS_CTL_CHANNEL );
	//     if( retValue != 1 )
	//     {
	//         sprintf(m_LogString,"Send message to WamLfoAna_his error!");
	//         WriteLogFile(m_LogString);
	//         delete [] msgSend ;
	//         return (false);		
	//     }
	//     else
	//     {
	//         sprintf(m_LogString,"Send message to WamLfoAna_his success!");
	//         WriteLogFile(m_LogString);
	//         delete [] msgSend ;
	//         return (true);		
	//     }
	// #endif
}

//////////////////////////////////////////////////////////////////////////
// 得到辅助决策  2008-09-08 by zhw
bool CLfoAnaObj::GetAsstDeci()
{
#ifdef _WAMAP_
#ifdef _LFO_ASST_DECI_
	//  LFO辅助决策表
	int                 nAsstDeci = 0;
	TAB_LFO_ASST_DECI* pAsstDeci = NULL;
	CBuffer                buf_base;
	int                    TabBufSize = 0;
	int                    ret_code;
	if (g_IDBTableOp.Open(LFO_AP_ID, LFO_ASST_DECI_NO, g_ctxNo) == DB_OK)
	{
		ret_code = g_IDBTableOp.TableGet(TAB_LFO_ASST_DECI_FLD, buf_base);
		if (ret_code > 0)
		{
			TabBufSize = sizeof(TAB_LFO_ASST_DECI);
			nAsstDeci = buf_base.GetLength() / TabBufSize;
			pAsstDeci = (TAB_LFO_ASST_DECI*)calloc(nAsstDeci, sizeof(TAB_LFO_ASST_DECI));
			memset((void*)pAsstDeci, 0, nAsstDeci * sizeof(TAB_LFO_ASST_DECI));
			memcpy((void*)pAsstDeci, (void*)buf_base.GetBufPtr(), nAsstDeci * TabBufSize);
		}
	}
	if (nAsstDeci <= 0)
	{
		sprintf(m_LogString, "GetAsstDeci: LFO_ASST_DECI_NO[%d]  no record ", LFO_ASST_DECI_NO);
		WriteLogFile(m_LogString);

		if (pAsstDeci) { free(pAsstDeci);  pAsstDeci = NULL; }
		return false;
	}

	m_nAsstDeci = nAsstDeci;
	memcpy((void*)m_pAsstDeci, (void*)pAsstDeci, m_nAsstDeci * TabBufSize);

#endif
#endif
	return true;
}

//确定实时曲线对应设备
void CLfoAnaObj::DecideLfoCurveID()
{
#ifdef _WAMAP_
	if (m_RunCtrCom == MENU_LFO_PRO_CTL_SAVE)
	{
		sprintf(m_LogString, "DecideLfoCurveID():: 案例保存不更新，返回！");
		MessShow(m_LogString);
		return;
	}

	int  i, j, ii;
	float max_ampl = 0.0;
	//INT_REC  findLfoCurveID   = 0; 
	INT_REC findGenCurve_id; ZERO_INT_REC(findGenCurve_id);
	INT_REC findAclnCurve_id; ZERO_INT_REC(findAclnCurve_id);
	INT_REC findGenPower_id; ZERO_INT_REC(findGenPower_id); // 2008-06-11 by zhw
	INT_REC findTrCurve_id; ZERO_INT_REC(findTrCurve_id);
	int  tmptableno;
	// #ifndef _WINDOWS32
	// 	int oldtype =0;
	// #endif
		//  数据来自PMU量测   
	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	{
		//用于显示LFO实测振荡曲线
		if (m_SysMon.stat > MENU_LFO_DEV_STS_SCT) //实时曲线可能跳变
		{
			//单独监视设备
			if (!VALID_INT_REC(findGenCurve_id))
			{
				if (m_nGnWarn > 0)
				{
					max_ampl = 0.0;
					for (ii = 0; ii < m_nGnWarn; ii++)
					{
						if (m_pGnWarn[ii].lead_ampl > max_ampl)
						{
							findGenCurve_id = m_pGnWarn[ii].gn_id;
						}
					}
				}
				else
				{
					max_ampl = 0.0;
					for (ii = 0; ii < m_nGnNum; ii++)
					{
						if (m_pGnMon[ii].lead_ampl > max_ampl)
						{
							findGenCurve_id = m_pGnMon[ii].gn_id;
						}
					}
				}
			}
			if (!VALID_INT_REC(findAclnCurve_id))
			{
				if (m_nLnWarn > 0)
				{
					max_ampl = 0.0;
					for (ii = 0; ii < m_nLnWarn; ii++)
					{
						if (m_pLnWarn[ii].lead_ampl > max_ampl)
						{
							findAclnCurve_id = m_pLnWarn[ii].ln_id;
						}
					}
				}
				else
				{
					max_ampl = 0.0;
					for (ii = 0; ii < m_nLnNum; ii++)
					{
						if (m_pLnMon[ii].lead_ampl > max_ampl)
						{
							findAclnCurve_id = m_pLnMon[ii].ln_id;
						}
					}
				}
			}
			if (!VALID_INT_REC(findTrCurve_id))
			{
				if (m_nTrWarn > 0)
				{
					max_ampl = 0.0;
					for (ii = 0; ii < m_nTrWarn; ii++)
					{
						if (m_pTrWarn[ii].lead_ampl > max_ampl)
						{
							findTrCurve_id = m_pTrWarn[ii].tr_id;
						}
					}
				}
				else
				{
					max_ampl = 0.0;
					for (ii = 0; ii < m_nTrNum; ii++)
					{
						if (m_pTrMon[ii].lead_ampl > max_ampl)
						{
							findTrCurve_id = m_pTrMon[ii].tr_id;
						}
					}
				}
			}
			// 查找名称和数据地址,没有找到则显示前一次设备曲线
			if (!VALID_INT_REC(findGenCurve_id))
			{
				m_LfoCurveGenDev.gn_id = findGenCurve_id;
				for (j = 0; j < m_nGnNum; j++)
				{
					if (findGenCurve_id == m_pGnInf[j].gn_id)
					{
						memcpy((void*)&m_LfoCurveGenDev, (void*)&m_pGnPmu[j].gn_id, sizeof(SSO_GN_PMU));
						if (para_ShowFacName == 1)
						{
							sprintf(m_LfoCurveGenDev.gn_name, "%s.%s", m_pGnInf[j].fac_name, m_pGnInf[j].gn_name);
							//strcpy(m_LfoCurveGenDev.gn_name, m_pGnInf[j].fac_name);  // 2008-07-23 by zhw
							//strcat(m_LfoCurveGenDev.gn_name, m_pGnInf[j].gn_name);
						}
						else
						{
							strcpy(m_LfoCurveGenDev.gn_name, m_pGnInf[j].gn_name);  // 2008-07-23 by zhw
						}

						break;
					}
				}
			}
			if (!VALID_INT_REC(findAclnCurve_id))
			{
				m_LfoCurveLineDev.ln_id = findAclnCurve_id;
				for (j = 0; j < m_nLnNum; j++)
				{
					if (findAclnCurve_id == m_pLnInf[j].acln_id)
					{
						memcpy((void*)&m_LfoCurveLineDev, (void*)&m_pLnPmu[j].ln_id, sizeof(SSO_LN_PMU));
						strcpy(m_LfoCurveLineDev.ln_name, m_pLnInf[j].acln_name);

						break;
					}
				}
			}
			if (!VALID_INT_REC(findTrCurve_id))
			{
				m_LfoCurveTrDev.tr_id = findTrCurve_id;
				for (j = 0; j < m_nTrNum; j++)
				{
					if (findTrCurve_id == m_pTrInf[j].tr_id)
					{
						memcpy((void*)&m_LfoCurveTrDev, (void*)&m_pTrPmu[j].tr_id, sizeof(SSO_TR_PMU));
						if (para_ShowFacName == 1)
						{
							sprintf(m_LfoCurveTrDev.tr_name, "%s.%s", m_pTrInf[j].fac_name, m_pTrInf[j].tr_name);
							//strcpy(m_LfoCurveTrDev.tr_name, m_pTrInf[j].fac_name);
							//strcat(m_LfoCurveTrDev.tr_name, m_pTrInf[j].tr_name);
						}
						else
						{
							strcpy(m_LfoCurveTrDev.tr_name, m_pTrInf[j].tr_name);
						}
						break;
					}
				}
			}
		}
		else // ( m_SysMon.stat <= MENU_LFO_DEV_STS_SCT ) 
		{
			INT_REC idx; ZERO_INT_REC(idx);
			// 此处处理，在系统未发生LFO，显示设定的曲线
			vector<TAB_SSO_RT_CURVE_ID> VecLfoRt;
			GetLfoRTCurveId(VecLfoRt);
			idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 7);
			for (j = 0; j < (int)VecLfoRt.size(); j++)
			{
				if (idx == VecLfoRt[j].dev_idx)
				{
					findGenCurve_id = VecLfoRt[j].dev_id;
					break;
				}
			}
			idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 3);
			for (j = 0; j < (int)VecLfoRt.size(); j++)
			{
				if (idx == VecLfoRt[j].dev_idx)
				{
					findAclnCurve_id = VecLfoRt[j].dev_id;
					break;
				}
			}
			idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 8);
			for (j = 0; j < (int)VecLfoRt.size(); j++)
			{
				if (idx == VecLfoRt[j].dev_idx)
				{
					findTrCurve_id = VecLfoRt[j].dev_id;
					break;
				}
			}
			//			idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB,2);
			// 			PODB_TableGetByKey(LFO_RT_CURVE_NO_TAB,idx, "dev_id", (char *)&findGenCurve_id, sizeof(INT_REC));
			// 			idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB,3);
			// 			PODB_TableGetByKey(LFO_RT_CURVE_NO_TAB,idx, "dev_id", (char *)&findAclnCurve_id, sizeof(INT_REC));
			// 			idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB,8);
			// 			PODB_TableGetByKey(LFO_RT_CURVE_NO_TAB,idx, "dev_id", (char *)&findTrCurve_id, sizeof(INT_REC));
			findGenPower_id = findGenCurve_id;

			if (!VALID_INT_REC(m_LfoCurveGenDev.gn_id) || !VALID_INT_REC(findGenCurve_id)) // 初次
			{
				for (j = 0; j < m_nGnNum; j++) // 显示波动最大曲线
				{
					memcpy((void*)&m_LfoCurveGenDev, (void*)&m_pGnPmu[j].gn_id, sizeof(SSO_GN_PMU));
					if (para_ShowFacName == 1) 
					{
						sprintf(m_LfoCurveGenDev.gn_name, "%s.%s",m_pGnInf[j].fac_name, m_pGnInf[j].gn_name);
					}
					else 
					{
						strcpy(m_LfoCurveGenDev.gn_name, m_pGnInf[j].gn_name);  // 2008-07-23 by zhw at ty
					}
					break;
				}
			}
			if (!VALID_INT_REC(m_LfoCurveLineDev.ln_id) || !VALID_INT_REC(findAclnCurve_id)) // 初次
			{
				for (j = 0; j < m_nLnNum; j++) // 显示波动最大曲线
				{
					memcpy((void*)&m_LfoCurveLineDev, (void*)&m_pLnPmu[j].ln_id, sizeof(SSO_LN_PMU));
					strcpy(m_LfoCurveLineDev.ln_name, m_pLnInf[j].acln_name);

					break;
				}
			}
			if (!VALID_INT_REC(m_LfoCurveTrDev.tr_id) || !VALID_INT_REC(findTrCurve_id)) // 初次
			{
				for (j = 0; j < m_nTrNum; j++) // 显示波动最大曲线
				{
					memcpy((void*)&m_LfoCurveTrDev, (void*)&m_pTrPmu[j].tr_id, sizeof(SSO_TR_PMU));
					if (para_ShowFacName == 1) 
					{
						sprintf(m_LfoCurveTrDev.tr_name, "%s.%s", m_pTrInf[j].fac_name, m_pTrInf[j].tr_name);
					}
					else 
					{
						strcpy(m_LfoCurveTrDev.tr_name, m_pTrInf[j].tr_name);
					}
					break;
				}
			}

			// 查找名称和数据地址,没有找到则显示前一次设备曲线
			tmptableno = PODB_GetTabelNoByDevId(findGenCurve_id);
			if (tmptableno == GN_DEVICE_NO_TAB && findGenCurve_id != m_LfoCurveGenDev.gn_id)
			{
				for (j = 0; j < m_nGnNum; j++) // 显示波动最大曲线
				{
					if (findGenCurve_id == m_pGnInf[j].gn_id)
					{
						memcpy((void*)&m_LfoCurveGenDev, (void*)&m_pGnPmu[j].gn_id, sizeof(SSO_GN_PMU));
						if (para_ShowFacName == 1) 
						{
							sprintf(m_LfoCurveGenDev.gn_name, "%s.%s",m_pGnInf[j].fac_name, m_pGnInf[j].gn_name);
						}
						else 
						{
							strcpy(m_LfoCurveGenDev.gn_name, m_pGnInf[j].gn_name);  // 2008-07-23 by zhw at ty																
						}

						break;
					}
				}
			}

			tmptableno = PODB_GetTabelNoByDevId(findAclnCurve_id);
			if (findAclnCurve_id != m_LfoCurveLineDev.ln_id && (tmptableno == ACLN_DEVICE_NO_TAB || tmptableno == ACLN_DOT_NO_TAB))
			{
				for (j = 0; j < m_nLnNum; j++) // 显示波动最大曲线
				{
					if (findAclnCurve_id == m_pLnInf[j].acln_id)
					{
						memcpy((void*)&m_LfoCurveLineDev, (void*)&m_pLnPmu[j].ln_id, sizeof(SSO_LN_PMU));
						strcpy(m_LfoCurveLineDev.ln_name, m_pLnInf[j].acln_name);

						break;
					}
				}
			}

			tmptableno = PODB_GetTabelNoByDevId(findTrCurve_id);
			if (findTrCurve_id != m_LfoCurveTrDev.tr_id && (tmptableno == TR_DEVICE_NO_TAB || tmptableno == TRWD_DEVICE_NO_TAB))
			{
				for (j = 0; j < m_nTrNum; j++) // 显示波动最大曲线
				{
					if (findTrCurve_id == m_pTrInf[j].tr_id)
					{
						memcpy((void*)&m_LfoCurveTrDev, (void*)&m_pTrPmu[j].tr_id, sizeof(SSO_TR_PMU));
						if (para_ShowFacName == 1) 
						{
							sprintf(m_LfoCurveTrDev.tr_name, "%s.%s", m_pTrInf[j].fac_name, m_pTrInf[j].tr_name);
						}
						else 
						{
							strcpy(m_LfoCurveTrDev.tr_name, m_pTrInf[j].tr_name);
						}
						
						break;
					}
				}
			}
		}
		sprintf(m_LogString, "DecideLfoCurveID():gn_id="INT_REC_FMT"[%s] acln_id="INT_REC_FMT"[%s] tr_id="INT_REC_FMT"[%s]!",
			INT_REC_VAL(m_LfoCurveGenDev.gn_id), m_LfoCurveGenDev.gn_name,
			INT_REC_VAL(m_LfoCurveLineDev.ln_id), m_LfoCurveLineDev.ln_name,
			INT_REC_VAL(m_LfoCurveTrDev.tr_id), m_LfoCurveTrDev.tr_name);
		MessShow(m_LogString);
	}
	else //if(m_calPara.dsource >= 2) // 处理本地数据 PMU实时数据ThreadFunc_RealtimeCurveProcess(void * lparm)中处理
	{
		ZERO_INT_REC(findGenCurve_id);
		ZERO_INT_REC(findAclnCurve_id);
		ZERO_INT_REC(findGenPower_id);
		ZERO_INT_REC(findTrCurve_id);
		int   no = 0;
		//int   skipNum = 1;
		float dtime = 0;

		if (m_nSmpStp < 1)
		{
			m_nSmpStp = 1;
		}

		if (m_SysMon.stat > MENU_LFO_DEV_STS_SCT) //实时曲线可能跳变
		{
			//单独监视设备
			if (!VALID_INT_REC(findGenCurve_id))
			{
				if (m_nGnWarn > 0)
				{
					findGenCurve_id = m_pGnWarn[0].gn_id;
					findGenPower_id = m_pGnWarn[0].gn_id;  // 2008-06-11 by zhw
					m_LfoCurveGenDev.gn_id = findGenCurve_id;
					sprintf(m_LogString, "111 m_LfoCurveGenDev.gn_id=["INT_REC_FMT"] ", INT_REC_VAL(m_LfoCurveGenDev.gn_id));
					MessShow(m_LogString);
				}
			}
			if (!VALID_INT_REC(findAclnCurve_id))
			{
				if (m_nLnWarn > 0)
				{
					findAclnCurve_id = m_pLnWarn[0].ln_id;
					m_LfoCurveLineDev.ln_id = findAclnCurve_id;
				}
			}
			if (!VALID_INT_REC(findTrCurve_id))
			{
				if (m_nTrWarn > 0)
				{
					findTrCurve_id = m_pTrWarn[0].tr_id;
					m_LfoCurveTrDev.tr_id = findTrCurve_id;
				}
			}
		}
		else
		{   // 此处处理，在系统未发生LFO，显示设定的曲线
			INT_REC  idx; ZERO_INT_REC(idx);
			vector<TAB_SSO_RT_CURVE_ID> VecLfoRt;
			GetLfoRTCurveId(VecLfoRt);
			idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 7);
			for (j = 0; j < (int)VecLfoRt.size(); j++)
			{
				if (idx == VecLfoRt[j].dev_idx)
				{
					findGenPower_id = VecLfoRt[j].dev_id;
					break;
				}
			}
			idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 3);
			for (j = 0; j < (int)VecLfoRt.size(); j++)
			{
				if (idx == VecLfoRt[j].dev_idx)
				{
					findAclnCurve_id = VecLfoRt[j].dev_id;
					break;
				}
			}
			idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 8);
			for (j = 0; j < (int)VecLfoRt.size(); j++)
			{
				if (idx == VecLfoRt[j].dev_idx)
				{
					findTrCurve_id = VecLfoRt[j].dev_id;
					break;
				}
			}
			// 			idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB,2);
			// 			sprintf(m_LogString,"idx["INT_REC_FMT"],talbeno[%d]",idx,LFO_RT_CURVE_NO_TAB);
			// 			MessShow(m_LogString);
			// 			PODB_TableGetByKey(LFO_RT_CURVE_NO_TAB,idx, "dev_id", (char *)&findGenCurve_id, sizeof(INT_REC));
			// 			idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB,3);
			// 			PODB_TableGetByKey(LFO_RT_CURVE_NO_TAB,idx, "dev_id", (char *)&findAclnCurve_id, sizeof(INT_REC));
			// 			idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB,8);
			// 			PODB_TableGetByKey(LFO_RT_CURVE_NO_TAB,idx, "dev_id", (char *)&findTrCurve_id, sizeof(INT_REC));
			// 			findGenPower_id = findGenCurve_id;
			if (!VALID_INT_REC(m_GenLfoCurve.dev_id) || !VALID_INT_REC(m_GenPowCurve.dev_id)) // 初次
			{
				m_LfoCurveGenDev.gn_id = m_pGnDat[0].gn_id;
				sprintf(m_LogString, "初次 m_LfoCurveGenDev.gn_id=["INT_REC_FMT"]", INT_REC_VAL(m_LfoCurveGenDev.gn_id));
				MessShow(m_LogString);
			}
			if (!VALID_INT_REC(m_AclnLfoCurve.dev_id)) // 初次
			{
				m_LfoCurveLineDev.ln_id = m_pLnDat[0].ln_id;
			}
			if (!VALID_INT_REC(m_TrLfoCurve.dev_id)) // 初次
			{
				m_LfoCurveTrDev.tr_id = m_pTrDat[0].tr_id;
			}
			// 查找名称和数据地址,没有找到则显示前一次设备曲线
			if (PODB_GetTabelNoByDevId(findGenPower_id) == GN_DEVICE_NO_TAB && findGenPower_id != m_LfoCurveGenDev.gn_id)
			{
				m_LfoCurveGenDev.gn_id = findGenPower_id;
				sprintf(m_LogString, "显示前一次设备曲线,m_LfoCurveGenDev.gn_id=["INT_REC_FMT"]", INT_REC_VAL(m_LfoCurveGenDev.gn_id));
				MessShow(m_LogString);
			}
			if (findAclnCurve_id != m_LfoCurveLineDev.ln_id && (PODB_GetTabelNoByDevId(findAclnCurve_id) == ACLN_DEVICE_NO_TAB || PODB_GetTabelNoByDevId(findAclnCurve_id) == ACLN_DOT_NO_TAB))
			{
				m_LfoCurveLineDev.ln_id = findAclnCurve_id;
			}
			if (findTrCurve_id != m_LfoCurveTrDev.tr_id && (PODB_GetTabelNoByDevId(findTrCurve_id) == TR_DEVICE_NO_TAB || PODB_GetTabelNoByDevId(findTrCurve_id) == TRWD_DEVICE_NO_TAB))
			{
				m_LfoCurveTrDev.tr_id = findTrCurve_id;
			}
		}

		m_GenPowCurve.dev_id = m_LfoCurveGenDev.gn_id;
		for (i = 0; i < m_nGnNum; i++)
		{
			if (m_GenPowCurve.dev_id == m_pGnDat[i].gn_id)
			{
				if(para_ShowFacName == 1)
				{
					sprintf(m_GenPowCurve.dev_name, "%s.%s", m_pGnInf[i].fac_name, m_pGnInf[i].gn_name);  //2021-10-24 by lns at jb
				}
				else 
				{
					strcpy(m_GenPowCurve.dev_name, m_pGnInf[i].gn_name);  // 2008-07-23 by zhw at ty
				}

				if (m_nSamples >= LFO_CURVE_MAX_PNT)
				{
					for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
					{
						no = m_nSmpStp * j;
						m_GenPowCurve.seq_cuv_data[j] = m_pGnDat[i].pwr[no];

						//printf(">>>>>>>>>  no=%d  time=%f value=%f !\n",
						//	j,
						//	m_GenLfoCurve.time[i],
						//	m_GenLfoCurve.value[i]);
					}
				}
				else
				{
					for (j = 0; j < LFO_CURVE_MAX_PNT - m_nSamples; j++)
					{
						no = j + m_nSamples;
						m_GenPowCurve.seq_cuv_data[j] = m_GenPowCurve.seq_cuv_data[no];
					}
					for (j = LFO_CURVE_MAX_PNT - m_nSamples; j < LFO_CURVE_MAX_PNT; j++)
					{
						no = m_nSmpStp * (j - (LFO_CURVE_MAX_PNT - m_nSamples));
						m_GenPowCurve.seq_cuv_data[j] = m_pGnDat[i].pwr[no];
					}
				}

				findGenCurve_id = m_pGnDat[i].gn_id;
				break;
			}

		}
		if (!VALID_INT_REC(findGenCurve_id))
		{
			memset((char*)&m_GenLfoCurve, 0, sizeof(TAB_LFO_RT_CURVE));
			m_GenLfoCurve.dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 2);
			for (i = 0; i < LFO_CURVE_MAX_PNT; i++)
			{
				m_GenLfoCurve.seq_cuv_data[i] = DB_PIC_NO_DIS;
			}
		}

		// line data
		m_AclnLfoCurve.dev_id = m_LfoCurveLineDev.ln_id;
		for (i = 0; i < m_nLnNum; i++)
		{
			if (m_AclnLfoCurve.dev_id == m_pLnDat[i].ln_id)
			{
				strcpy(m_AclnLfoCurve.dev_name, m_pLnInf[i].acln_name);
				if (m_nSamples >= LFO_CURVE_MAX_PNT)
				{
					for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
					{
						no = m_nSmpStp * j;
						m_AclnLfoCurve.seq_cuv_data[j] = m_pLnDat[i].pwr[no];
					}
				}
				else
				{
					for (j = 0; j < LFO_CURVE_MAX_PNT - m_nSamples; j++)
					{
						no = j + m_nSamples;
						m_AclnLfoCurve.seq_cuv_data[j] = m_AclnLfoCurve.seq_cuv_data[no];
					}
					for (j = LFO_CURVE_MAX_PNT - m_nSamples; j < LFO_CURVE_MAX_PNT; j++)
					{
						no = m_nSmpStp * (j - (LFO_CURVE_MAX_PNT - m_nSamples));
						m_AclnLfoCurve.seq_cuv_data[j] = m_pLnDat[i].pwr[no];
					}
				}

				findAclnCurve_id = m_pLnDat[i].ln_id;
				break;
			}
		}
		if (!VALID_INT_REC(findAclnCurve_id))
		{
			memset((char*)&m_AclnLfoCurve, 0, sizeof(TAB_LFO_RT_CURVE));
			m_AclnLfoCurve.dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 3);

			for (i = 0; i < LFO_CURVE_MAX_PNT; i++)
			{
				m_AclnLfoCurve.seq_cuv_data[i] = DB_PIC_NO_DIS;
			}
		}

		// tr data
		m_TrLfoCurve.dev_id = m_LfoCurveTrDev.tr_id;
		for (i = 0; i < m_nTrNum; i++)
		{
			if (m_TrLfoCurve.dev_id == m_pTrDat[i].tr_id)
			{
				if(para_ShowFacName == 1)
				{
					sprintf(m_TrLfoCurve.dev_name, "%s.%s", m_pTrInf[i].fac_name, m_pTrInf[i].tr_name);
				}
				else {
					strcpy(m_TrLfoCurve.dev_name, m_pTrInf[i].tr_name);
				}

				if (m_nSamples >= LFO_CURVE_MAX_PNT)
				{
					for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
					{
						no = m_nSmpStp * j;
						m_TrLfoCurve.seq_cuv_data[j] = m_pTrDat[i].pwr[no];
					}
				}
				else
				{
					for (j = 0; j < LFO_CURVE_MAX_PNT - m_nSamples; j++)
					{
						no = j + m_nSamples;
						m_TrLfoCurve.seq_cuv_data[j] = m_TrLfoCurve.seq_cuv_data[no];
					}
					for (j = LFO_CURVE_MAX_PNT - m_nSamples; j < LFO_CURVE_MAX_PNT; j++)
					{
						no = m_nSmpStp * (j - (LFO_CURVE_MAX_PNT - m_nSamples));
						m_TrLfoCurve.seq_cuv_data[j] = m_pTrDat[i].pwr[no];
					}
				}

				findTrCurve_id = m_pTrDat[i].tr_id;
				break;
			}
		}
		if (!VALID_INT_REC(findTrCurve_id))
		{
			memset((char*)&m_TrLfoCurve, 0, sizeof(TAB_LFO_RT_CURVE));
			m_TrLfoCurve.dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 8);

			for (i = 0; i < LFO_CURVE_MAX_PNT; i++)
			{
				m_TrLfoCurve.seq_cuv_data[i] = DB_PIC_NO_DIS;
			}
		}
		sprintf(m_LogString, "DecideLfoCurveID():gn_id="INT_REC_FMT"[%s] acln_id="INT_REC_FMT"[%s] tr_id="INT_REC_FMT"[%s]!",
			INT_REC_VAL(m_GenLfoCurve.dev_id), m_GenLfoCurve.dev_name,
			INT_REC_VAL(m_AclnLfoCurve.dev_id), m_AclnLfoCurve.dev_name,
			INT_REC_VAL(m_TrLfoCurve.dev_id), m_TrLfoCurve.dev_name);
		MessShow(m_LogString);

		//为了图形时间显示连接唯一动态数据
		//	TAB_LFO_RT_CURVE    m_TimeLfoCurve ;                  //  实时曲线对应时间
		if (m_nSamples >= LFO_CURVE_MAX_PNT)
		{
			for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
			{
				no = m_nSmpStp * j;
				m_TimeLfoCurve.seq_cuv_data[j] = m_Matchtime[no] - m_Matchtime[0];
			}
			m_TimeLfoCurve.cuv_stp = (m_Matchtime[1] - m_Matchtime[0]) * m_nSmpStp;
			m_TimeLfoCurve.cuv_sec = (int)(m_TimeLfoCurve.cuv_stp * (LFO_CURVE_MAX_PNT + 1));
			m_TimeLfoCurve.cuv_bgn_sec = m_CurGpsTime - m_TimeLfoCurve.cuv_sec;
			m_TimeLfoCurve.cuv_end_sec = m_CurGpsTime;

			m_GenLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
			m_GenLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
			m_GenLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
			m_GenLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;

			// 2008-06-11 by zhw
			m_GenPowCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
			m_GenPowCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
			m_GenPowCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
			m_GenPowCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;

			m_AclnLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
			m_AclnLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
			m_AclnLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
			m_AclnLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;

			m_TrLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
			m_TrLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
			m_TrLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
			m_TrLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;
		}
		else
		{
			dtime = (m_Matchtime[1] - m_Matchtime[0]) * m_nSmpStp;
			for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
			{
				no = j;
				m_TimeLfoCurve.seq_cuv_data[no] = no * dtime;//0;
			}
			m_TimeLfoCurve.cuv_stp = dtime;
			m_TimeLfoCurve.cuv_sec = (int)(dtime * (LFO_CURVE_MAX_PNT + 1));
			m_TimeLfoCurve.cuv_bgn_sec = m_CurGpsTime - m_TimeLfoCurve.cuv_sec;
			m_TimeLfoCurve.cuv_end_sec = m_CurGpsTime;

			m_GenLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
			m_GenLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
			m_GenLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
			m_GenLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;

			// 2008-06-11 by zhw
			m_GenPowCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
			m_GenPowCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
			m_GenPowCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
			m_GenPowCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;

			m_AclnLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
			m_AclnLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
			m_AclnLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
			m_AclnLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;

			m_TrLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
			m_TrLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
			m_TrLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
			m_TrLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;
		}
	}
#endif
	return;
}

int CLfoAnaObj::GetLfoRTCurveId(vector<TAB_SSO_RT_CURVE_ID>& VecLfoRt)
{
	if (!VecLfoRt.empty())
	{
		VecLfoRt.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_RT_CURVE_NO_TAB, TAB_SSO_RT_CURVE_ID_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(TAB_SSO_RT_CURVE_ID);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO实时曲线表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO实时曲线表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	TAB_SSO_RT_CURVE_ID* pInfo = NULL;
	pInfo = new TAB_SSO_RT_CURVE_ID[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(TAB_SSO_RT_CURVE_ID));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecLfoRt.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

//  实时曲线显示处理线程(刷新实时曲线、断面曲线)
//  从本地获取 m_TimeLfoCurve m_LfoCurveGenDev  m_LfoCurveLineDev
//  从高速缓冲区获取 起始时刻、结束时刻、采样时间、采样频率
void CLfoAnaObj::RealtimeCurveProcess()
{
	//////////////////////////////////////////////////////////////////////////
	int            i, j, no = 0;
	char           LogString[MAXBUF];

	WT_VEC_TIME    lastVecTime;
	time_t         LastRecTime = 0;
	time_t         th_beginTime = 0;
	time_t         th_endTime = 0;
	time_t         th_endTime_old = 0;
#ifndef _D5000
	short          iappno = -10;
	short          ifieldno = -10;
#else
	int            iappno = -10;
	int            ifieldno = -10;
#endif
	int            nRealNum = 0;

	int            th_sam_freq = 0;
	int            th_sam_time = 0;
	int            th_piontNum = 0;
	//int            th_realpiontNum  = 0;

	VEC_DATA_STS   vtmpSts;
	int            findFlag1 = 0;
	int            findFlag2 = 0;
	int            th_movesec = 0;             // 移动秒数
	int            th_movepnt = 0;             // 移动点数

	float          th_step = 0;             // 步长
	//WT_VEC_DATA    tmpData[DATA_NUM_MAX];
	PUB_WT_VEC_DATA* tmpData = NULL;
	WT_VEC_DESC    desc;
	float* th_DataBuffer1 = NULL;  // 数据缓冲
	float* th_DataBuffer2 = NULL;  // 数据缓冲
	float* th_DataBuffer3 = NULL;  // 数据缓冲
	SEQ_BASE_INFO  dataBaseInfo;
	//float          th_max       = 0;
	//float          th_min       = 0;
	//float          th_aver      = 0;
	//int            bufSize      = 0;
	///////////////////////////// calc p by ui //////////////////////////////
	int              ii, jj;
	int              nNeedNum = 0;
	//float            vviData[DATA_NUM_MAX*VEC_VI_NUM];
	//VEC_DATA_STS     vSecSts[DATA_NUM_MAX];
	float* vviData;
	VEC_DATA_STS* vSecSts;
	int              baddata_flag = 0;
	//////////////////////////////////////////////////////////////////////////
	m_TimeLfoCurve.dev_idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 1);
	m_GenLfoCurve.dev_idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 2);
	m_AclnLfoCurve.dev_idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 3);
	m_GenPowCurve.dev_idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 7);
	m_TrLfoCurve.dev_idx = PODB_GetDevIdByKeyID(SSO_RT_CURVE_NO_TAB, 8);
	// 	strcpy(m_TimeLfoCurve.dev_name, "LFO实时曲线时间") ;
	// 	strcpy(m_TimeLfoCurve.unit,     "秒") ;
	// 	strcpy(m_GenLfoCurve.dev_name, "发电机功角曲线") ;
	// 	strcpy(m_GenLfoCurve.unit,     "deg") ;
	// 	strcpy(m_AclnLfoCurve.dev_name, "线路有功曲线") ;
	// 	strcpy(m_AclnLfoCurve.unit,     "MW") ;
	// 	strcpy(m_GenPowCurve.dev_name, "发电机有功曲线") ;
	// 	strcpy(m_GenPowCurve.unit,     "MW") ;
	// 	strcpy(m_TrLfoCurve.dev_name, "变压器有功曲线") ;
	// 	strcpy(m_TrLfoCurve.unit,     "MW") ;
		//////////////////////////////////////////////////////////////////////////
	LastRecTime = 0;
	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)// 处理PMU实时数据
	{
		th_sam_time = LFO_CURVE_TIME_LENG;
		th_sam_freq = (int)(th_sam_time * MAX_FRA_PER_SEC / LFO_CURVE_MAX_PNT);
		th_step = (float)(th_sam_time * 1.0 / LFO_CURVE_MAX_PNT);

		// 获取GPS时标
// 		if ( PWD_GetLastTime(lastVecTime) != 0)
// 		{
// 			sprintf( LogString, "Get GPS time error"); 
// 			MessShow( LogString );
// 			return ;
// 		}
// 		th_endTime   = lastVecTime.csecond - para_nDelaySec;
		th_endTime = m_CurGpsTime - para_nDelaySec;
		th_beginTime = th_endTime - th_sam_time;
		th_piontNum = th_sam_time * MAX_FRA_PER_SEC;

		if (th_endTime_old < 10)
		{
			th_movesec = LFO_CURVE_TIME_LENG;
			th_movepnt = LFO_CURVE_MAX_PNT;// 移动点数
		}
		else
		{
			th_movesec = (th_endTime - th_endTime_old);
			th_movepnt = (int)(th_movesec * LFO_CURVE_MAX_PNT / LFO_CURVE_TIME_LENG + 0.45);// 移动点数
		}
		th_movesec = LFO_CURVE_TIME_LENG;
		th_movepnt = 0;// 移动点数
		if (th_movepnt < 1) th_movepnt = 0;
		th_endTime_old = th_endTime;

		///////////////////////////////// 实时曲线 ///////////////////////////////
		m_TimeLfoCurve.cuv_bgn_sec = th_beginTime;
		m_TimeLfoCurve.cuv_end_sec = th_endTime;
		m_TimeLfoCurve.cuv_sec = th_sam_time;
		m_TimeLfoCurve.cuv_pnt = LFO_CURVE_MAX_PNT;
		m_TimeLfoCurve.cuv_stp = th_step;
		for (i = 0; i < LFO_CURVE_MAX_PNT; i++)
		{
			m_TimeLfoCurve.seq_cuv_data[i] = th_step * i;
		}

		// 曲线数据处理
		if (PODB_GetTabelNoByDevId(m_LfoCurveGenDev.gn_id) == GN_DEVICE_NO_TAB)
		{
			m_GenLfoCurve.dev_id = m_LfoCurveGenDev.gn_id;
			strcpy(m_GenLfoCurve.dev_name, m_LfoCurveGenDev.gn_name);
			m_GenPowCurve.dev_id = m_LfoCurveGenDev.gn_id;
			strcpy(m_GenPowCurve.dev_name, m_LfoCurveGenDev.gn_name);
		}
		if (PODB_GetTabelNoByDevId(m_LfoCurveLineDev.ln_id) == ACLN_DEVICE_NO_TAB || PODB_GetTabelNoByDevId(m_LfoCurveLineDev.ln_id) == ACLN_DOT_NO_TAB)
		{
			m_AclnLfoCurve.dev_id = m_LfoCurveLineDev.ln_id;
			strcpy(m_AclnLfoCurve.dev_name, m_LfoCurveLineDev.ln_name);
		}
		if (PODB_GetTabelNoByDevId(m_LfoCurveTrDev.tr_id) == TR_DEVICE_NO_TAB || PODB_GetTabelNoByDevId(m_LfoCurveTrDev.tr_id) == TRWD_DEVICE_NO_TAB)
		{
			m_TrLfoCurve.dev_id = m_LfoCurveTrDev.tr_id;
			strcpy(m_TrLfoCurve.dev_name, m_LfoCurveTrDev.tr_name);
		}
		// 前移数据
		for (j = 0; j < LFO_CURVE_MAX_PNT - th_movepnt; j++)
		{
			no = j + th_movepnt;
			m_GenLfoCurve.seq_cuv_data[j] = m_GenLfoCurve.seq_cuv_data[no];
			m_AclnLfoCurve.seq_cuv_data[j] = m_AclnLfoCurve.seq_cuv_data[no];
			m_GenPowCurve.seq_cuv_data[j] = m_GenPowCurve.seq_cuv_data[no];  // 2008-06-11 by zhw
			m_TrLfoCurve.seq_cuv_data[j] = m_TrLfoCurve.seq_cuv_data[no];
		}
		for (j = LFO_CURVE_MAX_PNT - th_movepnt; j < LFO_CURVE_MAX_PNT; j++)
		{
			m_GenLfoCurve.seq_cuv_data[j] = 0;
			m_AclnLfoCurve.seq_cuv_data[j] = 0;
			m_GenPowCurve.seq_cuv_data[j] = 0; // 2008-06-11 by zhw
			m_TrLfoCurve.seq_cuv_data[j] = 0;
		}
		// 取新数 (机组计算功角) 
		iappno = -10;
		ifieldno = -10;
		findFlag1 = 0;
		findFlag2 = 0;
		tmpData = (PUB_WT_VEC_DATA*)calloc(DATA_NUM_MAX, sizeof(PUB_WT_VEC_DATA));
		th_DataBuffer1 = (float*)calloc(DATA_NUM_MAX, sizeof(float));

		// 2008-06-11 by zhw 
		// 取发电机有功功率
		iappno = -10;
		ifieldno = -10;
		findFlag2 = 0;
		iappno = m_LfoCurveGenDev.vecAmpl.viFilNo;
		ifieldno = m_LfoCurveGenDev.vecAmpl.viColNo;

		memset((void*)tmpData, 0, DATA_NUM_MAX * sizeof(PUB_WT_VEC_DATA));
		if (iappno >= 0 && ifieldno >= 0)
		{
#ifndef _D5000
			if (PWD_TableGet(iappno, ifieldno, th_endTime - th_movesec, th_endTime, tmpData, nRealNum, desc) == true)
#else
			if (PWD_TableGet(ifieldno, th_endTime - th_movesec, th_endTime, tmpData, nRealNum) == true)
#endif
			{   // 逐秒逐桢修复并统计无效桢
				if (nRealNum == th_movesec * MAX_FRA_PER_SEC)
				{
					memset((void*)&th_DataBuffer1[0], 0, DATA_NUM_MAX * sizeof(float));
					for (no = 0; no < th_movesec; no++)
					{
						memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
						IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[no * MAX_FRA_PER_SEC], th_endTime - th_movesec + no, (float*)&th_DataBuffer1[no * MAX_FRA_PER_SEC], vtmpSts);
					}

					for (j = 0; j < LFO_CURVE_MAX_PNT - th_movepnt; j++)
					{
						no = j * th_sam_freq;
						m_GenPowCurve.seq_cuv_data[j + th_movepnt] = th_DataBuffer1[no];
					}
				}
			}
		}

		// 取新数 (线路有功) 
		{
			iappno = -10;
			ifieldno = -10;
			findFlag1 = 0;
			// 是否末端
			if (m_LfoCurveLineDev.is_end == MENU_LFO_MENU_YES)
			{
				iappno = m_LfoCurveLineDev.vecPwr2.viFilNo;
				ifieldno = m_LfoCurveLineDev.vecPwr2.viColNo;
			}
			else
			{
				iappno = m_LfoCurveLineDev.vecPwr1.viFilNo;
				ifieldno = m_LfoCurveLineDev.vecPwr1.viColNo;
			}

			memset((void*)&th_DataBuffer1[0], 0, DATA_NUM_MAX * sizeof(float));
			memset((void*)tmpData, 0, DATA_NUM_MAX * sizeof(PUB_WT_VEC_DATA));
			if (iappno >= 0 && ifieldno >= 0)
			{
#ifndef _D5000
				if (PWD_TableGet(iappno, ifieldno, th_endTime - th_movesec, th_endTime, tmpData, nRealNum, desc) == true)
#else
				if (PWD_TableGet(ifieldno, th_endTime - th_movesec, th_endTime, tmpData, nRealNum) == true)
#endif
				{   // 逐秒逐桢修复并统计无效桢
					if (nRealNum == th_movesec * MAX_FRA_PER_SEC)
					{
						for (no = 0; no < th_movesec; no++)
						{
							memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
							IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[no * MAX_FRA_PER_SEC], th_endTime - th_movesec + no, (float*)&th_DataBuffer1[no * MAX_FRA_PER_SEC], vtmpSts);
						}
						findFlag1 = 1;
					}
				}
			}
			if (findFlag1 == 1)
			{
				th_DataBuffer2 = (float*)calloc(DATA_NUM_MAX, sizeof(float));
				memset((void*)&th_DataBuffer2[0], 0, DATA_NUM_MAX * sizeof(float));
				DP_MidAveFilter(th_DataBuffer1, th_DataBuffer2, nRealNum, 5);
				for (j = 0; j < LFO_CURVE_MAX_PNT - th_movepnt; j++)
				{
					no = j * th_sam_freq;
					m_AclnLfoCurve.seq_cuv_data[j + th_movepnt] = th_DataBuffer2[no];
				}
				if (th_DataBuffer2) { free(th_DataBuffer2); th_DataBuffer2 = NULL; }
			}
		}

		// 取新数 (变压器有功) 
		{
			iappno = -10;
			ifieldno = -10;
			findFlag1 = 0;
			if (m_LfoCurveTrDev.slct_wind_id == m_LfoCurveTrDev.high_wind_id)
			{
				iappno = m_LfoCurveTrDev.vecPwr_high.viFilNo;
				ifieldno = m_LfoCurveTrDev.vecPwr_high.viColNo;
			}
			else if (m_LfoCurveTrDev.slct_wind_id == m_LfoCurveTrDev.mid_wind_id)
			{
				iappno = m_LfoCurveTrDev.vecPwr_mid.viFilNo;
				ifieldno = m_LfoCurveTrDev.vecPwr_mid.viColNo;
			}
			else if (m_LfoCurveTrDev.slct_wind_id == m_LfoCurveTrDev.low_wind_id)
			{
				iappno = m_LfoCurveTrDev.vecPwr_low.viFilNo;
				ifieldno = m_LfoCurveTrDev.vecPwr_low.viColNo;
			}
			else
			{
				// 报错
			}

			memset((void*)&th_DataBuffer1[0], 0, DATA_NUM_MAX * sizeof(float));
			memset((void*)tmpData, 0, DATA_NUM_MAX * sizeof(PUB_WT_VEC_DATA));
			if (iappno >= 0 && ifieldno >= 0)
			{
#ifndef _D5000
				if (PWD_TableGet(iappno, ifieldno, th_endTime - th_movesec, th_endTime, tmpData, nRealNum, desc) == true)
#else
				if (PWD_TableGet(ifieldno, th_endTime - th_movesec, th_endTime, tmpData, nRealNum) == true)
#endif
				{   // 逐秒逐桢修复并统计无效桢
					if (nRealNum == th_movesec * MAX_FRA_PER_SEC)
					{
						for (no = 0; no < th_movesec; no++)
						{
							memset((void*)&vtmpSts, 0, sizeof(VEC_DATA_STS));
							IWM_PmuDataCheck((PUB_WT_VEC_DATA*)&tmpData[no * MAX_FRA_PER_SEC], th_endTime - th_movesec + no, (float*)&th_DataBuffer1[no * MAX_FRA_PER_SEC], vtmpSts);
						}
						findFlag1 = 1;
					}
				}
			}
			if (findFlag1 == 1)
			{
				th_DataBuffer2 = (float*)calloc(DATA_NUM_MAX, sizeof(float));
				memset((void*)&th_DataBuffer2[0], 0, DATA_NUM_MAX * sizeof(float));
				DP_MidAveFilter(th_DataBuffer1, th_DataBuffer2, nRealNum, 5);
				for (j = 0; j < LFO_CURVE_MAX_PNT - th_movepnt; j++)
				{
					no = j * th_sam_freq;
					m_TrLfoCurve.seq_cuv_data[j + th_movepnt] = th_DataBuffer2[no];
				}
				if (th_DataBuffer2) { free(th_DataBuffer2); th_DataBuffer2 = NULL; }
			}
		}
		if (th_DataBuffer1) { free(th_DataBuffer1); th_DataBuffer1 = NULL; }
		if (tmpData) { free(tmpData); tmpData = NULL; }
	}

	// 数据处理
	DP_CalcDataSeqBaseInfo((float*)&m_GenLfoCurve.seq_cuv_data[0], LFO_CURVE_MAX_PNT, dataBaseInfo);
	m_GenLfoCurve.max_val = dataBaseInfo.max_val;
	m_GenLfoCurve.min_val = dataBaseInfo.min_val;
	m_GenLfoCurve.ave_val = dataBaseInfo.ave_val;
	m_GenLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
	m_GenLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
	m_GenLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
	m_GenLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;
	strcpy(m_GenLfoCurve.unit, " deg ");

	// 2008-06-11 by zhw
	DP_CalcDataSeqBaseInfo((float*)&m_GenPowCurve.seq_cuv_data[0], LFO_CURVE_MAX_PNT, dataBaseInfo);
	m_GenPowCurve.max_val = dataBaseInfo.max_val;
	m_GenPowCurve.min_val = dataBaseInfo.min_val;
	m_GenPowCurve.ave_val = dataBaseInfo.ave_val;
	m_GenPowCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
	m_GenPowCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
	m_GenPowCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
	m_GenPowCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;
	strcpy(m_GenPowCurve.unit, " MW ");

	DP_CalcDataSeqBaseInfo((float*)&m_AclnLfoCurve.seq_cuv_data[0], LFO_CURVE_MAX_PNT, dataBaseInfo);
	m_AclnLfoCurve.max_val = dataBaseInfo.max_val;
	m_AclnLfoCurve.min_val = dataBaseInfo.min_val;
	m_AclnLfoCurve.ave_val = dataBaseInfo.ave_val;
	m_AclnLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
	m_AclnLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
	m_AclnLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
	m_AclnLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;
	strcpy(m_AclnLfoCurve.unit, " MW ");

	DP_CalcDataSeqBaseInfo((float*)&m_TrLfoCurve.seq_cuv_data[0], LFO_CURVE_MAX_PNT, dataBaseInfo);
	m_TrLfoCurve.max_val = dataBaseInfo.max_val;
	m_TrLfoCurve.min_val = dataBaseInfo.min_val;
	m_TrLfoCurve.ave_val = dataBaseInfo.ave_val;
	m_TrLfoCurve.cuv_stp = m_TimeLfoCurve.cuv_stp;
	m_TrLfoCurve.cuv_sec = m_TimeLfoCurve.cuv_sec;
	m_TrLfoCurve.cuv_bgn_sec = m_TimeLfoCurve.cuv_bgn_sec;
	m_TrLfoCurve.cuv_end_sec = m_TimeLfoCurve.cuv_end_sec;
	strcpy(m_TrLfoCurve.unit, " MW ");

	// 写入实时库
	{
		UpdateLfoCurve(m_TimeLfoCurve);
		UpdateLfoCurve(m_GenLfoCurve);
		UpdateLfoCurve(m_AclnLfoCurve);
		UpdateLfoCurve(m_GenPowCurve); // 2008-06-11 by zhw
		UpdateLfoCurve(m_TrLfoCurve);
#ifdef _NUSP
		vector<INT_REC> tmpRecordid;
		for (i = 0; i < LFO_CURVE_MAX_PNT; i++)
		{
			m_SsoCurve2[i].recordid = PODB_GetDevIdByKeyID(SSO_RT_CURVE2_NO_TAB, i+1);
			m_SsoCurve2[i].time_view = m_TimeLfoCurve.seq_cuv_data[i];
			m_SsoCurve2[i].gn_power = m_GenPowCurve.seq_cuv_data[i];
			m_SsoCurve2[i].ln_power = m_AclnLfoCurve.seq_cuv_data[i];
			m_SsoCurve2[i].tr_power = m_TrLfoCurve.seq_cuv_data[i];
			strcpy(m_SsoCurve2[i].gn_name, m_GenPowCurve.dev_name);
			strcpy(m_SsoCurve2[i].ln_name, m_AclnLfoCurve.dev_name);
			strcpy(m_SsoCurve2[i].tr_name, m_TrLfoCurve.dev_name);

			tmpRecordid.push_back(m_SsoCurve2[i].recordid);
		}
		PODB_DeleteRecord(tmpRecordid, SSO_RT_CURVE2_NO_TAB);  //清除前LFO_CURVE_MAX_PNT条记录
		tmpRecordid.clear();
		vector<INT_REC>().swap(tmpRecordid);
		// PODB_TableClear(SSO_RT_CURVE2_NO_TAB);  // 整表清除

		m_SsoCurve2[0].time_point = m_TimeLfoCurve.cuv_bgn_sec;
		m_SsoCurve2[LFO_CURVE_MAX_PNT - 1].time_point = m_TimeLfoCurve.cuv_end_sec;

		int ret_code = PODB_TableWrite((const char*)&m_SsoCurve2[0].recordid, LFO_CURVE_MAX_PNT * sizeof(TAB_SSO_RT_CURVE2), SSO_RT_CURVE2_NO_TAB);
		if (ret_code < 0)
		{
			sprintf(m_LogString, "写SSO实时曲线横向表[%d]失败", SSO_RT_CURVE2_NO_TAB);
			MessShow(m_LogString);
		}
#endif // _NUSP
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 逐桢检查1秒实采量，返回修复数据（有效桢插值修复无效帧）及数据状态
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 
//  参数说明 : iPmudata                 输入实采量
//             tBgnSec                  取数时刻
//             oVecData                 返回数组
//             vecSts                   质量状态                            [OUT]
bool CLfoAnaObj::IWM_PmuDataCheck(PUB_WT_VEC_DATA* iPmudata, time_t tBgnSec, float* oVecData, VEC_DATA_STS& vecSts)
{
#ifdef _D5000
	vecSts.nFraNum = MAX_FRA_PER_SEC;
	PMU_DATA_DESC tmpPmuDesc;
	memset((void*)&tmpPmuDesc, 0, sizeof(PMU_DATA_DESC));
	PWD_PmuDataCheck(iPmudata, MAX_FRA_PER_SEC, tmpPmuDesc);
	if (tmpPmuDesc.nKedong > 0)
	{
		vecSts.nBadNum = vecSts.nBadNum + tmpPmuDesc.nKedong;
	}

	if (tmpPmuDesc.nPmuOff > 0)
	{
		vecSts.nBadNum = MAX_FRA_PER_SEC;
		vecSts.bPmuOff = 1;
	}

	if (tmpPmuDesc.nDataBad > 0)
	{
		vecSts.nBadSeq = tmpPmuDesc.nDataBad;
		vecSts.nBadNum = vecSts.nBadNum + tmpPmuDesc.nDataBad;
	}

	if (tmpPmuDesc.nPmuBad > 0)
	{
		vecSts.nBadNum = vecSts.nBadNum + tmpPmuDesc.nPmuBad;
	}

	if (tmpPmuDesc.nGpsLost > 0)
	{
		vecSts.nBadNum = vecSts.nBadNum + tmpPmuDesc.nGpsLost;
	}

	if (tmpPmuDesc.nDataLost > 0)
	{
		vecSts.nBadNum = vecSts.nBadNum + tmpPmuDesc.nDataLost;
	}

	if (tmpPmuDesc.nDataDead > 0)
	{
		vecSts.nBadNum = vecSts.nBadNum + tmpPmuDesc.nDataDead;
	}

	if (tmpPmuDesc.nTimeDiff > 0)
	{
		vecSts.nBadNum = vecSts.nBadNum + tmpPmuDesc.nTimeDiff;
	}

	// 	if (tmpPmuDesc.nDynSimu > 0 )
	// 	{
	// 		vecSts.nBadNum = vecSts.nBadNum + tmpPmuDesc.nDynSimu;
	// 	}

	int ii;
	for (ii = 0; ii < MAX_FRA_PER_SEC; ii++)
	{
		oVecData[ii] = iPmudata[ii].value;
	}

#else
	int             nBadSeq = 0;
	int             ii;

	if (!iPmudata || !oVecData) return false;

	memset((void*)&vecSts, 0, sizeof(VEC_DATA_STS));

	int iVecBgn = MAX_FRA_PER_SEC;
	for (ii = 0; ii < MAX_FRA_PER_SEC; ++ii)
	{
		if (!((iPmudata[ii].quality & WS_MEAS_STS_DATA_BAD)))
		{
			iVecBgn = ii; break;
		}
	}

	int iVecEnd = -1;
	for (ii = MAX_FRA_PER_SEC - 1; ii > iVecBgn; --ii)// for ( ii = MAX_FRA_PER_SEC - 1; ii >= iVecBgn; --ii ) // 08-01-28 lsf
	{
		if (!(iPmudata[ii].quality & WS_MEAS_STS_DATA_BAD))
		{
			iVecEnd = ii; break;
		}
	}

	vecSts.nFraNum = MAX_FRA_PER_SEC;

	//  设置PMU通道退出信号
	for (ii = 0; ii < MAX_FRA_PER_SEC; ++ii)
	{
		//  2008-10-11 by zhw
		//  拟合值  0XFFFFFFFF
		if ((iPmudata[ii].quality & WS_MEAS_STS_PMU_OFF) && (iPmudata[ii].quality != WS_MEAS_STS_DYN_SIMU))
		{
			vecSts.bPmuOff = 1; break;
		}
	}

	//  逐桢修复并统计无效桢
	for (ii = 0; ii < MAX_FRA_PER_SEC; ++ii)
	{
		oVecData[ii] = iPmudata[ii].value;

		if (iPmudata[ii].quality == WS_MEAS_STS_DYN_SIMU)
		{
			continue;
		}
		//  如果是无效桢
		if (iPmudata[ii].quality & WS_MEAS_STS_DATA_BAD)
		{
			//  如果存在多个有效桢, 则无效桢用同一秒内的有效桢插值替代
			if (iVecBgn < iVecEnd)
			{
				int i1, i2, nn;

				if (ii < iVecBgn)
				{
					for (nn = ii + 1; nn <= iVecEnd; ++nn)
					{
						if (!(iPmudata[nn].quality & WS_MEAS_STS_DATA_BAD)) { i1 = nn; break; }
					}
					for (nn = i1 + 1; nn <= iVecEnd; ++nn)
					{
						if (!(iPmudata[nn].quality & WS_MEAS_STS_DATA_BAD)) { i2 = nn; break; }
					}
				}
				else if (ii > iVecEnd)
				{
					for (nn = ii - 1; nn >= iVecBgn; --nn)
					{
						if (!(iPmudata[nn].quality & WS_MEAS_STS_DATA_BAD)) { i2 = nn; break; }
					}
					for (nn = i2 - 1; nn >= iVecBgn; --nn)
					{
						if (!(iPmudata[nn].quality & WS_MEAS_STS_DATA_BAD)) { i1 = nn; break; }
					}
				}
				else
				{
					for (nn = ii - 1; nn >= iVecBgn; --nn)
					{
						if (!(iPmudata[nn].quality & WS_MEAS_STS_DATA_BAD)) { i1 = nn; break; }
					}
					for (nn = i1 + 1; nn <= iVecEnd; ++nn)
					{
						if (!(iPmudata[nn].quality & WS_MEAS_STS_DATA_BAD)) { i2 = nn; break; }
					}
				}

				oVecData[ii] = iPmudata[i1].value + (iPmudata[i2].value - iPmudata[i1].value) * (ii - i1) / (i2 - i1);
			}
			//  如果存在一个有效桢, 则无效桢用同一秒内的有效桢直接替代
			else if (iVecBgn == iVecEnd)
			{
				oVecData[ii] = iPmudata[iVecBgn].value;
			}

			nBadSeq++;

			vecSts.nBadNum++;
			vecSts.tBadSec = tBgnSec;
			vecSts.iBadFra = ii;
		}
		else if (nBadSeq > 0)
		{
			if (nBadSeq > vecSts.nBadSeq) vecSts.nBadSeq = nBadSeq;

			nBadSeq = 0;
		}
	}

	if (nBadSeq > vecSts.nBadSeq) vecSts.nBadSeq = nBadSeq;
#endif

	return true;
}


//   写LFO告警设备振荡曲线20141008
void CLfoAnaObj::UpdateWarnDevCurve(TAB_WARN_DEV_CURVE WarnCurve)
{
#ifdef _WAMAP_

	int i = 0;

	TAB_WARN_DEV_CURVE1    temp_DevCurve1;
	TAB_WARN_DEV_CURVE2    temp_DevCurve2;

	memset((void*)&temp_DevCurve1, 0, sizeof(TAB_WARN_DEV_CURVE1));
	memset((void*)&temp_DevCurve1, 0, sizeof(TAB_WARN_DEV_CURVE2));

	INT_REC keyrecord = WarnCurve.dev_idx;

	temp_DevCurve1.dev_id = WarnCurve.dev_id;
	sprintf(temp_DevCurve1.dev_name, "%s", WarnCurve.dev_name);
	sprintf(temp_DevCurve1.unit, "%s", WarnCurve.unit);

	temp_DevCurve1.sam_freq = WarnCurve.sam_freq;
	temp_DevCurve1.cuv_bgn_sec = WarnCurve.cuv_bgn_sec;
	temp_DevCurve1.cuv_end_sec = WarnCurve.cuv_end_sec;
	temp_DevCurve1.cuv_sec = WarnCurve.cuv_sec;
	temp_DevCurve1.cuv_pnt = WarnCurve.cuv_pnt;
	temp_DevCurve1.cuv_stp = WarnCurve.cuv_stp;
	// 	temp_DevCurve1.max_val     = WarnCurve.max_val;
	// 	temp_DevCurve1.min_val     = WarnCurve.min_val;
	// 	temp_DevCurve1.ave_val     = WarnCurve.ave_val;

	for (i = 0; i < LFO_CURVE_PNT; i++)
	{
		temp_DevCurve1.seq_cuv_data[i] = WarnCurve.seq_cuv_data[i];
	}
	for (i = 0; i < LFO_CURVE_PNT; i++)
	{
		temp_DevCurve2.seq_cuv_data[i] = WarnCurve.seq_cuv_data[i + LFO_CURVE_PNT];
	}

	// 	PODB_TableModifyByKey((const char *)&keyrecord, TAB_WARN_DEV_CURVE1_FLD, (char *)&temp_DevCurve1.dev_id, sizeof(TAB_WARN_DEV_CURVE1),LFO_DEV_CURVE_TAB);
	// 	PODB_TableModifyByKey((const char *)&keyrecord, TAB_WARN_DEV_CURVE2_FLD, (char *)&temp_DevCurve2.seq_cuv_data[0], sizeof(TAB_WARN_DEV_CURVE2),LFO_DEV_CURVE_TAB);

#endif
	return;
}
