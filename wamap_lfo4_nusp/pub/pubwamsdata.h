/**************************************************************************
  
   pubodbapi.h: interface for the CPubOdbApi class.
  
   HISTORY :    	
   Date        Version       Author                Activities      
      ----------------------------------------------------------------------
   2011/07/11   v1.0.0        zhw                  创建
   2011/08/23   v1.0.1        zhw                  D-5000时间序列数据库重新封装
   2012/05/17   v1.0.2        zhw                  修正警告内容
**************************************************************************/

#if !defined(AFX_PUBWAMSDATA_H__3E58ABCC_2110_4778_81B8_A4CBF8029544__INCLUDED_)
#define AFX_PUBWAMSDATA_H__3E58ABCC_2110_4778_81B8_A4CBF8029544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #ifndef _WAMSDATA_
// #define _WAMSDATA_
// #endif

#ifdef _WAMSDATA_
#ifdef _SIFANG
#include "wrdb_pnapi.h"
#else
#include "WamapDBLib.h"
#endif // _SIFANG
#endif
#include <time.h>
#include <vector>
#include <iostream>
#include <system.h>  //获取应用参数 AP_WAMS AF_WAMS_NAME
#include <sam_service.h>  //获取应用所在主机host_name
#include <sam_common.h> 

using namespace std;

#ifdef AP_WAMS
#define		AP_WAMS			700000
#endif // AP_WAMS

#ifdef AF_WAMS_NAME
#define		AF_WAMS_NAME			"wams"
#endif // AF_WAMS_NAME

//////////////////////////////////////////////////////////////
extern int g_appNo;
extern string g_appName;
extern int g_ctxNo;
extern string g_ctxName;
//////////////////////////////////////////////////////////////

#ifndef MAXBUF
#define MAXBUF 512
#endif

#ifndef LTDEBUG
#define LTDEBUG 0
#define LTINFO  1
#define LTDEV   2
#define LTWARN  3
#define LTERROR 4
#endif

#ifdef _D5000
#ifndef WS_MEAS_STS_OK
#define WS_MEAS_STS_OK              0x00                //  正常
#define WS_MEAS_STS_PMU_OFF         0x01                //  通道退出(前置通讯中断)
#define WS_MEAS_STS_DATA_BAD        0x02                //  PMU数据异常(PMU上送质量位)
#define WS_MEAS_STS_PMU_BAD         0x04                //  PMU装置异常(PMU上送质量位)
#define WS_MEAS_STS_GPS_LOST        0x08                //  GPS失步(PMU上送质量位)
#define WS_MEAS_STS_DATA_LOST       0x10                //  PMU数据丢帧(前置判定)
#define WS_MEAS_STS_DATA_DEAD       0x20                //  死数据(前置判定)
#define WS_MEAS_STS_TIME_DIFF       0x40                //  PMU数据时差较大(前置判定)
#endif
#ifndef WS_MEAS_STS_MANU_BAD
#define WS_MEAS_STS_MANU_BAD        0x80                //  人工设定异常数据(前置判定)
#endif
#define WS_MEAS_STS_DYN_SIMU        -99                 //  动态数据拟合值(PI、eDNA等动态数据库)(未使用)
#define WS_MEAS_STS_KEDONG          -1                  //  科东前置
#else
#ifndef WS_MEAS_STS_OK
#define WS_MEAS_STS_OK              0x08                //  正常
#define WS_MEAS_STS_PMU_OFF         0x01                //  通道退出(前置通讯中断)
#define WS_MEAS_STS_DATA_BAD        0x02                //  数据不可用(未细化的质量位)
#define WS_MEAS_STS_PMU_BAD         0x00                //  PMU装置异常(PMU上送质量位)(未使用)
#define WS_MEAS_STS_GPS_LOST        0x00                //  GPS失步(PMU上送质量位)(未使用)
#define WS_MEAS_STS_DATA_LOST       0x00                //  PMU数据丢帧(前置判定)(未使用)
#define WS_MEAS_STS_DATA_DEAD       0x00                //  死数据(前置判定)(未使用)
#define WS_MEAS_STS_TIME_DIFF       0x00                //  PMU数据时差较大(前置判定)(未使用)
#define WS_MEAS_STS_DYN_SIMU        -1                  //  动态数据拟合值(PI、eDNA等动态数据库)
#define WS_MEAS_STS_KEDONG          -99                 //  科东前置
#endif
#endif
//////////////////////////////////////////////////////////////////////////
typedef struct type_PMU_DATA_DESC
{
    int  nPmuOff;  //  通道退出(前置通讯中断)
    int  nDataBad;  // PMU数据异常(PMU上送质量位)
    int  nPmuBad;  //  PMU装置异常(PMU上送质量位)
	int  nGpsLost;  // GPS失步(PMU上送质量位)
	int  nDataLost;  // PMU数据丢帧(前置判定)
	int  nDataDead;  // 死数据(前置判定)
	int  nTimeDiff;  // PMU数据时差较大(前置判定)
	int  nManu_Bad;  // 人工设定异常数据(前置判定)
    int  nDynSimu;  // 动态数据拟合值(PI、eDNA等动态数据库)(可用)
    int  nKedong;  // 科东前置,质量位为-1
	int  nAll;  // 异常点总数
	int  bCon;  // 是否有连续异常点标志
	int  begin_ok;  // 第一个正常数据位置
	int  end_ok;  // 最后一个正常数据位置
}PMU_DATA_DESC;

// typedef struct tag_VEC_DATA_STS
// {
//     int             nFraNum;                                //  总帧数
//     int             nBadNum;                                //  无效桢数
//     int             nBadSeq;                                //  最大连续无效桢数
//     time_t          tBadSec;                                //  最近无效时间
//     short           iBadFra;                                //  最近无效桢号
//     unsigned char   bPmuOff;                                //  PMU通道退出信号
//     VEC_YC_FCNO     VecBadData[3];                          //  无效帧文件号  // 2008-07-16
// }VEC_DATA_STS;                                              //  高速缓存区动态数据质量状态

#ifdef _WAMSDATA_
#ifdef _SIFANG
#define WT_VEC_TIME struct WRDB_Time
#define WT_VEC_VALUE struct WRDB_Value
typedef struct {
	float			value; //向量数值
	short			quality; //质量标记	
} WT_VEC_DATA;
typedef struct {
    short           pmu_type                ; //pmu 类型用于标示是PMU数据还是ADX3000数据
	int             record_id               ; //向量ID
	short           column_id               ; //向量ID
	char            desc[64]                ; //向量描述
} WT_VEC_DESC ;
#endif // _SIFANG
#else // _WAMSDATA_
typedef struct _MY_KEY_ID {
	int 	record_id 	;
	short	column_id 	;
} MY_KEY_ID ;
typedef struct {
	float			value 			; //向量数值
	short			quality			; //质量标记	
} WT_VEC_DATA ;

typedef struct {
	int			csecond			; //世纪秒
	short		fracsec			; //秒等份
} WT_VEC_TIME ;

typedef struct {
	short		pmu_type		; //pmu 类型用于标示是PMU数据还是ADX3000数据
	int		record_id		; //向量ID
	short		column_id		; //向量ID
	char		desc[64]		; //向量描述	
} WT_VEC_DESC ;

//WAMS时间序列库数据值点定义
typedef struct {
    WT_VEC_TIME time      ;
    float     value       ;
    char      status      ;
} WT_VEC_VALUE ;
#endif // _WAMSDATA_

#ifdef _D5000
    #define     PUB_WT_VEC_DATA                         WT_VEC_VALUE
#else
    #define     PUB_WT_VEC_DATA                         WT_VEC_DATA
#endif

//////////////////////////////////////////////////////////////////////////
class CPubWamsData  
{
public:
	CPubWamsData();
	virtual ~CPubWamsData();
	virtual void MessShow(const char *log, const int logtype = LTINFO);
public:
	bool PWD_GetStatus();
	bool PWD_TableGet(WT_VEC_TIME &ntime,int type=0);
#ifndef _D5000
    bool PWD_GetAppFieldByKeyId(short &appNo,short &fieldNo,const MY_KEY_ID &keyid);
	bool PWD_GetKeyIdByAppField(MY_KEY_ID &keyid,const int &appNo,const short &fieldNo);
	bool PWD_TableGet(const char* keyPtr,time_t start,time_t end,vector<WT_VEC_TIME> &ntime,vector<PUB_WT_VEC_DATA> &vdata,int type=0) ;
	bool PWD_TableGet(const short &appNo,const short &fieldNo,time_t start,time_t end,PUB_WT_VEC_DATA *ptr,int &num);
	bool PWD_TableGet(const char* keyPtr,time_t start,time_t end,PUB_WT_VEC_DATA *ptr,int &num);
	bool PWD_TableGet(const short &appNo,const short &fieldNo,time_t start,time_t end,PUB_WT_VEC_DATA *ptr,int &num,WT_VEC_DESC &desc);
	//bool PWD_TableGet(const short &appNo,const short &fieldNo,time_t start,time_t end,float *ptr,int &num,WT_VEC_DESC &desc);
	bool PWD_TableGet(const char* keyPtr,time_t start,time_t end,PUB_WT_VEC_DATA *ptr,int &num,WT_VEC_DESC &desc);
#else
	bool PWD_TableGet(const int &addr,const WT_VEC_TIME &ntime,WT_VEC_DATA &vdata);  
	bool PWD_TableGet(const int &addr,const time_t &start,const time_t &end,WT_VEC_VALUE *pData,int &num,int interval=0);
#endif
public:
	int  PWD_GetLastTime();
	int  PWD_GetLastTime(int &ntime);
	int  PWD_GetLastTime(WT_VEC_TIME &wttime);
	bool PWD_PmuDataCheck(PUB_WT_VEC_DATA *iPmudata, const int nPmudata, PMU_DATA_DESC &vecSts);
	bool PWD_PmuDataCheck(PUB_WT_VEC_DATA *iPmudata, const int nPmudata, PMU_DATA_DESC &vecSts, int &bad_num, int &bad_con);
	//bool PWD_PmuDataCheck(WT_VEC_DATA *iPmudata, time_t tBgnSec, float * oVecData, VEC_DATA_STS &vecSts);

private:
	int  PWD_GetHostName(string& hostname, const int app_id = g_appNo, const int ctx_no = g_ctxNo); // 获取主机名

private:
	char m_LogStr[MAXBUF];
#ifdef _SIFANG
	CServicesManage     m_ServMan;  //lns20211030可以使用
#endif // _SIFANG
#ifdef _WAMSDATA_
#ifndef _SIFANG
#ifdef  USE_NET_WAM_MEM
	#ifdef USE_WEEK_WAM_MEM
		CNetWamapDatabase  *m_pWamsData; //  高速缓存区历史数据网络接口
	#else
		CNetWamapDatabase  *m_pWamsData; //  高速缓存区实时数据网络接口
	#endif
#else
	#ifdef USE_WEEK_WAM_MEM
		CWamapDatabase     *m_pWamsData; //  高速缓存区历史数据本地接口
	#else
		CWamapDatabase     *m_pWamsData; //  高速缓存区实时数据本地接口
	#endif
#endif // USE_NET_WAM_MEM
#endif // !_SIFANG
#endif // _WAMSDATA_
};

#endif // !defined(AFX_PUBWAMSDATA_H__3E58ABCC_2110_4778_81B8_A4CBF8029544__INCLUDED_)
