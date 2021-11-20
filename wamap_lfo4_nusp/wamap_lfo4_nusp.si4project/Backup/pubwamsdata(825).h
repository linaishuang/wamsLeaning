/**************************************************************************
  
   pubodbapi.h: interface for the CPubOdbApi class.
  
   HISTORY :    	
   Date        Version       Author                Activities      
      ----------------------------------------------------------------------
   2011/07/11   v1.0.0        zhw                  ����
   2011/08/23   v1.0.1        zhw                  D-5000ʱ���������ݿ����·�װ
   2012/05/17   v1.0.2        zhw                  ������������
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
#include <system.h>  //��ȡӦ�ò��� AP_WAMS AF_WAMS_NAME
#include <sam_service.h>  //��ȡӦ����������host_name
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
#define WS_MEAS_STS_OK              0x00                //  ����
#define WS_MEAS_STS_PMU_OFF         0x01                //  ͨ���˳�(ǰ��ͨѶ�ж�)
#define WS_MEAS_STS_DATA_BAD        0x02                //  PMU�����쳣(PMU��������λ)
#define WS_MEAS_STS_PMU_BAD         0x04                //  PMUװ���쳣(PMU��������λ)
#define WS_MEAS_STS_GPS_LOST        0x08                //  GPSʧ��(PMU��������λ)
#define WS_MEAS_STS_DATA_LOST       0x10                //  PMU���ݶ�֡(ǰ���ж�)
#define WS_MEAS_STS_DATA_DEAD       0x20                //  ������(ǰ���ж�)
#define WS_MEAS_STS_TIME_DIFF       0x40                //  PMU����ʱ��ϴ�(ǰ���ж�)
#endif
#ifndef WS_MEAS_STS_MANU_BAD
#define WS_MEAS_STS_MANU_BAD        0x80                //  �˹��趨�쳣����(ǰ���ж�)
#endif
#define WS_MEAS_STS_DYN_SIMU        -99                 //  ��̬�������ֵ(PI��eDNA�ȶ�̬���ݿ�)(δʹ��)
#define WS_MEAS_STS_KEDONG          -1                  //  �ƶ�ǰ��
#else
#ifndef WS_MEAS_STS_OK
#define WS_MEAS_STS_OK              0x08                //  ����
#define WS_MEAS_STS_PMU_OFF         0x01                //  ͨ���˳�(ǰ��ͨѶ�ж�)
#define WS_MEAS_STS_DATA_BAD        0x02                //  ���ݲ�����(δϸ��������λ)
#define WS_MEAS_STS_PMU_BAD         0x00                //  PMUװ���쳣(PMU��������λ)(δʹ��)
#define WS_MEAS_STS_GPS_LOST        0x00                //  GPSʧ��(PMU��������λ)(δʹ��)
#define WS_MEAS_STS_DATA_LOST       0x00                //  PMU���ݶ�֡(ǰ���ж�)(δʹ��)
#define WS_MEAS_STS_DATA_DEAD       0x00                //  ������(ǰ���ж�)(δʹ��)
#define WS_MEAS_STS_TIME_DIFF       0x00                //  PMU����ʱ��ϴ�(ǰ���ж�)(δʹ��)
#define WS_MEAS_STS_DYN_SIMU        -1                  //  ��̬�������ֵ(PI��eDNA�ȶ�̬���ݿ�)
#define WS_MEAS_STS_KEDONG          -99                 //  �ƶ�ǰ��
#endif
#endif
//////////////////////////////////////////////////////////////////////////
typedef struct type_PMU_DATA_DESC
{
    int  nPmuOff;  //  ͨ���˳�(ǰ��ͨѶ�ж�)
    int  nDataBad;  // PMU�����쳣(PMU��������λ)
    int  nPmuBad;  //  PMUװ���쳣(PMU��������λ)
	int  nGpsLost;  // GPSʧ��(PMU��������λ)
	int  nDataLost;  // PMU���ݶ�֡(ǰ���ж�)
	int  nDataDead;  // ������(ǰ���ж�)
	int  nTimeDiff;  // PMU����ʱ��ϴ�(ǰ���ж�)
	int  nManu_Bad;  // �˹��趨�쳣����(ǰ���ж�)
    int  nDynSimu;  // ��̬�������ֵ(PI��eDNA�ȶ�̬���ݿ�)(����)
    int  nKedong;  // �ƶ�ǰ��,����λΪ-1
	int  nAll;  // �쳣������
	int  bCon;  // �Ƿ��������쳣���־
	int  begin_ok;  // ��һ����������λ��
	int  end_ok;  // ���һ����������λ��
}PMU_DATA_DESC;

// typedef struct tag_VEC_DATA_STS
// {
//     int             nFraNum;                                //  ��֡��
//     int             nBadNum;                                //  ��Ч����
//     int             nBadSeq;                                //  ���������Ч����
//     time_t          tBadSec;                                //  �����Чʱ��
//     short           iBadFra;                                //  �����Ч���
//     unsigned char   bPmuOff;                                //  PMUͨ���˳��ź�
//     VEC_YC_FCNO     VecBadData[3];                          //  ��Ч֡�ļ���  // 2008-07-16
// }VEC_DATA_STS;                                              //  ���ٻ�������̬��������״̬

#ifdef _WAMSDATA_
#ifdef _SIFANG
#define WT_VEC_TIME struct WRDB_Time
#define WT_VEC_VALUE struct WRDB_Value
typedef struct {
	float			value; //������ֵ
	short			quality; //�������	
} WT_VEC_DATA;
typedef struct {
    short           pmu_type                ; //pmu �������ڱ�ʾ��PMU���ݻ���ADX3000����
	int             record_id               ; //����ID
	short           column_id               ; //����ID
	char            desc[64]                ; //��������
} WT_VEC_DESC ;
#endif // _SIFANG
#else // _WAMSDATA_
typedef struct _MY_KEY_ID {
	int 	record_id 	;
	short	column_id 	;
} MY_KEY_ID ;
typedef struct {
	float			value 			; //������ֵ
	short			quality			; //�������	
} WT_VEC_DATA ;

typedef struct {
	int			csecond			; //������
	short		fracsec			; //��ȷ�
} WT_VEC_TIME ;

typedef struct {
	short		pmu_type		; //pmu �������ڱ�ʾ��PMU���ݻ���ADX3000����
	int		record_id		; //����ID
	short		column_id		; //����ID
	char		desc[64]		; //��������	
} WT_VEC_DESC ;

//WAMSʱ�����п�����ֵ�㶨��
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
	int  PWD_GetHostName(string& hostname, const int app_id = g_appNo, const int ctx_no = g_ctxNo); // ��ȡ������

private:
	char m_LogStr[MAXBUF];
#ifdef _SIFANG
	CServicesManage     m_ServMan;  //lns20211030����ʹ��
#endif // _SIFANG
#ifdef _WAMSDATA_
#ifndef _SIFANG
#ifdef  USE_NET_WAM_MEM
	#ifdef USE_WEEK_WAM_MEM
		CNetWamapDatabase  *m_pWamsData; //  ���ٻ�������ʷ��������ӿ�
	#else
		CNetWamapDatabase  *m_pWamsData; //  ���ٻ�����ʵʱ��������ӿ�
	#endif
#else
	#ifdef USE_WEEK_WAM_MEM
		CWamapDatabase     *m_pWamsData; //  ���ٻ�������ʷ���ݱ��ؽӿ�
	#else
		CWamapDatabase     *m_pWamsData; //  ���ٻ�����ʵʱ���ݱ��ؽӿ�
	#endif
#endif // USE_NET_WAM_MEM
#endif // !_SIFANG
#endif // _WAMSDATA_
};

#endif // !defined(AFX_PUBWAMSDATA_H__3E58ABCC_2110_4778_81B8_A4CBF8029544__INCLUDED_)
