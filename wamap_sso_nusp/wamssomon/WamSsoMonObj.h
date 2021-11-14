/******************************************************************************
  ModuleName :   WAMS LFO Analysis frame
  FileName   :   WamLfoAnaObj.cpp
  Description:   WAMS LFO Analysis frame Implemention
  HISTORY :    	
      Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   09/11/2007       1.0         lsf                    created
******************************************************************************/
//VC6.0���ã�Project-->Settings...-->C/C++ -->General -->Preprocessors definitions: _WINDOWS32
//			 Project-->Settings...-->C/C++ -->Code Generation -->Use run-time library: Debug Multithreaded DLL
//	         Project-->Settings...-->Link  -->General -->Object/library modules --> Ws2_32.lib

//����ƽ̨��1) TableNet �ӿڣ����¿��¼���������ƣ�Ŀǰ���ֳ���250float����
//			2) ���̳߳��򣬴�������TableNetʵ�壬������һ���߳���ʹ�ú󣬲ſ��ã�
// ���ö��壺 
// �궨��
// ��������
//   �ӿں���(���롢����ӿڡ��澯���͡�����������tmd�ӿ�)
//   ������֯����(����Ԥ����)
//   ������ҵ���ƺ���
#ifndef _WAM_LFO_ANA_OBJ_H_
#define _WAM_LFO_ANA_OBJ_H_

#include "pubodbapi.h"
#include "pubfun.h"
#include "pubwamsdata.h"
#include "DataFile.h"
// #include  "PubWamAppFunc.h"               //  WAMSӦ�ù��ú���
#include  "WamLfoStruct.h"                                  //  LFO���ӷ����ṹ
//#include  "WamRtCurveDisp.h"                                //  ������ʾ�ṹ
#include  "WamSsoMonObjItfc.h"                              //  ���ݿ⽻���ṹ WamSsoMonObjItfc.h
#include  "LFOAnaByCurve.h"                          //  LFO�����㷨����
#include "PubDataProcess.h"

#ifdef _PODB_SAMPLE_
#include "db_def/sample_def_wams.h"
#endif

// #define _LFO_ASST_DECI_                                  //  ���ɸ�������
#ifdef _LFO_ASST_DECI_
#include "WamLfoAsstObj.h"
#endif
#include <vector>
#include <string>
//  �궨��
/******************************************************************************/
//#define     g_appNo                  AP_WAMAP             // 

// case state
#define L_No_PROC        0         // δ����
#define L_PROC           1         // ���ڴ�����
#define L_FINISH         2         // �������

// estimate state
#define L_EST_FIRST      1          // ����
#define L_EST_SECOND     2          // �ǳ���

// data state
#define L_VALID_DATA     1          // ��Ч���� 
#define L_BAD_DATA       2          // ������

// memory state
#define NOALLOCMEM       0          // �����·���
#define REALLOCMEM       1          // ���·���

// Form-Resulte state//(0 ������ 1�������� 2���� 3ͳ�������)
#define L_NOT_FORM_RESULTE       0          // ������
#define L_FST_FORM_RESULTE       1          // ��������
#define L_SnD_FORM_RESULTE       2          // ����
#define L_END_FORM_RESULTE       3          // ͳ�������

// ������������  2008-06-11 by zhw
#define DATAFILEMAXLENGTH  (1024*1024*10)   //  �����ļ���󳤶�

#define DATA_TYPE_FAC_VOLT       0          //  ��վ�ߵ�ѹ
#define DATA_TYPE_FAC_ANG        1          //  ��վ���
#define DATA_TYPE_GEN_POW        2          //  ������й�
#define DATA_TYPE_GEN_ANG        3          //  ���������
#define DATA_TYPE_ACLN_POW       4          //  ��·�й�

//  �ٻ�100֡�ļ�
#define LFO_TMD_DYN_GET_NO       0          //  �ٻ�100֡�ļ�
#define LFO_TMD_DYN_GET_YES      1

#define  FILE_TYPE_SIM    0
#define  FILE_TYPE_DYN    1

#define DB_PIC_NO_DIS                 0                     //  ���治��ʾ��ֵ

#define	MAIN_ROLE			0
#define	VICE_ROLE			1
#define CALC_ROLE			2

#define    NBSAVX	          400                           // point

/*ĸ�����*/
#ifndef BS_TYPE_DM
#define	BS_TYPE_DM	0	/* ��ĸ */
#define	BS_TYPE_ZM	1	/* ��ĸ */
#define	BS_TYPE_FM	2	/* ��ĸ */
#define	BS_TYPE_PM	3	/* ��ĸ */
#endif

#ifndef WAMS_SSO_WARN_TYPE
#define WAMS_SSO_WARN_TYPE 210
#endif

#ifndef AP_WAMS_SSO
#define AP_WAMS_SSO 703200
#endif

#define CONNECT_FES 1 //���ӵ�ǰ��
#define CONNECT_PMU 2 //���ӵ�PMU
#define IN_TRANS    3 //������
#define SUCCED      4 //�ٻ��ɹ�
#define FAILED      5 //�ٻ�ʧ��

#define     MULTI_MENU(n)    ((1<<(n-1))) // ��ѡ�˵�

/******************************************************************************/
//  ����Ԥ���������������
//  Ƶ��Ԥ����
const float FHZ_MAX_VAL              =  53.0f;               //  Ƶ�������ֵ
const float FHZ_MIN_VAL              =  47.0f;               //  Ƶ�������ֵ
//  ��ѹԤ����
const float VOL_UP_LMT_COE           =  1.3f;                //  ��������ѹ����ϵ������ѹ���ͻ�ֵ��
const float VOL_LW_LMT_COE           =  0.7f;                //  ��������ѹ����ϵ��
//  ����Ԥ����
const float PWR_UP_LMT_COE           =  2.0f;                //  ��������������ϵ�����ֵ��
const float PWR_LW_LMT_COE           = -1.0f;                //  ��������������ϵ��

//  ͻ���ж�
const short BAS_FRAME_PERSEC         =  100;                 //  ��Сÿ��֡��
const float FHZ_MAX_CHG              =  2.0f;                //  Ƶ�����ͻ��
const float G_ANG_MAX_CHG            =  3.0f;                //  0.01�빦�����仯
const float G_PWR_MAX_CHG            =  0.05f;               //  0.01�빦�����ͻ�䣨�ֵ��
const float L_PWR_MAX_CHG            =  30.0f;               //  0.01���й����仯

//  �궨��
#define ZEROVALUE  0.0001

/******************************************************************************/

// ��̬��Ϣ
//////////////////////////////////////////////////////////////////////////
#define PMU_SET_INF_FCNO_NO   0                             //  ���ļ����к�
#define PMU_SET_INF_FCNO_OK   1                             //  ����
#define PMU_SET_INF_FCNO_ER   3                             //  �ļ����кŴ���


// ң����Ϣ����
/******************************************************************************/
// ��վң����Ϣ
typedef struct tag_LFO_FC_PMU
{
    INT_REC         fac_id;                                 //  ��վID��
    INT_REC         m_bs_id;                                //  ĸ��ID��
	VEC_V_FCNO      m_bs_vecV;                              //  �������� (UA UB UC)
    INT_REC         v_bs_id;                                //  ĸ��ID��
	VEC_V_FCNO      v_bs_vecV;                              //  �������� (UA UB UC)
    VEC_YC_FCNO     vecFHZ;                                 //  ��վƵ��
    unsigned char   vcurSts;                                //  ��ѹ��ǰ״̬
    unsigned char   vhisSts;                                //  ��ѹ��ʷ״̬
    unsigned char   fcurSts;                                //  Ƶ�ʵ�ǰ״̬
    unsigned char   fhisSts;                                //  Ƶ����ʷ״̬
	unsigned char   is_m_bs;                                //  ��ĸ����
} LFO_FC_PMU;

// ����ң����Ϣ
typedef struct tag_SSO_GN_PMU
{
	INT_REC         gn_id;      //  ����ID��
	char            gn_name[64];   //  �������� 
	VEC_YC_FCNO     vecAmpl;   //  �񵴷�ֵ����
	VEC_YC_FCNO     vecFreq;    //  ��Ƶ������
	VEC_YC_FCNO     vecSSO;    //  ��״̬����
	VEC_YC_FCNO     vecPwr;    //  ��״̬����
    unsigned char   curSts;                                 //  ��ǰ״̬
    unsigned char   hisSts;                                 //  ��ʷ״̬
}SSO_GN_PMU;

// ������·ң����Ϣ
typedef struct tag_SSO_LN_PMU
{
	INT_REC         ln_id;      //  ��·ID��
	char            ln_name[64];   //  ��·���� 
	INT_REC         dot_id_1;   //  �׶˶˵�ID��
	VEC_YC_FCNO     vecAmpl1;   //  �׶��񵴷�ֵ����
	VEC_YC_FCNO     vecFreq1;    //  �׶���Ƶ������
	VEC_YC_FCNO     vecSSO1;    //  �׶���״̬����
	VEC_YC_FCNO     vecPwr1;    //  �׶��й�����
	INT_REC         dot_id_2;   //  ĩ�˶˵�ID��
	VEC_YC_FCNO     vecAmpl2;   //  ĩ�׶��񵴷�ֵ����
	VEC_YC_FCNO     vecFreq2;    //  ĩ����Ƶ������
	VEC_YC_FCNO     vecSSO2;    //  ĩ����״̬����
	VEC_YC_FCNO     vecPwr2;    //  ĩ���й�����
    unsigned char   curSts;                                 //  ��ǰ״̬
    unsigned char   hisSts;                                 //  ��ʷ״̬
	unsigned char   is_end;                                 //  ĩ������
}SSO_LN_PMU;

// ��ѹ��ң����Ϣ
typedef struct tag_SSO_TR_PMU
{
	INT_REC         tr_id;      //  ��ѹ��ID��
	char            tr_name[64];   //  ��ѹ������ 
	INT_REC         high_wind_id;   //  ��ѹ����ѹ��ID��
	VEC_YC_FCNO     vecAmpl_high;   //  ��ѹ���񵴷�ֵ����
	VEC_YC_FCNO     vecFreq_high;    //  ��ѹ����Ƶ������
	VEC_YC_FCNO     vecSSO_high;    //  ��ѹ����״̬����
	VEC_YC_FCNO     vecPwr_high;    //  ��ѹ����״̬����
	INT_REC         mid_wind_id;   //  ��ѹ����ѹ��ID��
	VEC_YC_FCNO     vecAmpl_mid;   //  ��ѹ���񵴷�ֵ����
	VEC_YC_FCNO     vecFreq_mid;    //  ��ѹ����Ƶ������
	VEC_YC_FCNO     vecSSO_mid;    //  ��ѹ����״̬����
	VEC_YC_FCNO     vecPwr_mid;    //  ��ѹ����״̬����
	INT_REC         low_wind_id;   //  ��ѹ����ѹ��ID��
	VEC_YC_FCNO     vecAmpl_low;   //  ��ѹ���񵴷�ֵ����
	VEC_YC_FCNO     vecFreq_low;    //  ��ѹ����Ƶ������
	VEC_YC_FCNO     vecSSO_low;    //  ��ѹ����״̬����
	VEC_YC_FCNO     vecPwr_low;    //  ��ѹ����״̬����
    unsigned char   curSts;                                 //  ��ǰ״̬
    unsigned char   hisSts;                                 //  ��ʷ״̬
    INT_REC         slct_wind_id;                           //  ѡ������ID
    unsigned char   hwd_yc_ok;                              //  ��ѹ����OK
    unsigned char   mwd_yc_ok;                              //  ��ѹ����OK
    unsigned char   lwd_yc_ok;                              //  ��ѹ����OK
}SSO_TR_PMU;

//��¼���ļ���Ϣ
typedef struct tag_HD_File_Struct
{
	INT_REC fac_id;//��վ����
	char cfg_name[64];//�����ļ���
	char dat_name[64];//�����ļ���
}HD_File_Struct;
//����SSO������Ϣ�ļ��ṹ
typedef struct tag_SSO_MSG_FILE_STRUCT
{
	char version[10];  // �汾��
	char dir[64];  // ¼���ļ��洢Ŀ¼
	RTDB_TIME start_time;  // ����ʼʱ��
	int file_num;//¼���ļ���Ŀ
	vector<HD_File_Struct> vfile;//��¼���ļ�
} SSO_MSG_FILE_STRUCT;
//�ٻ���¼���ļ��ĳ�վ��Ϣ
typedef struct tag_SSO_HD_FILE_FAC
{
	INT_REC fac_id;
	int stat;
	char file_name[40];
} SSO_HD_FILE_FAC;

/******************************************************************************/

//  case���м���궨��
/******************************************************************************/
#define CALC_THRD_NUM                   2                    //  ���м����߳���
#define THRD_CASE_NUM                   20//10                   //  �����߳�Case��
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
////  ���м���case
typedef struct  tag_CALC_THRD_CASE
{	
#ifdef _WINDOWS32
	HANDLE          ThrdCaseMutex;                          //  �̻߳�����
#else
	pthread_mutex_t ThrdCaseMutex;                          //  �̻߳�����
#endif
	int             thrd_id;                                //  �߳�id
	int             isStartUp;                              //  �Ƿ��������㣨��������λ 0 ��     1 ������
	int             isFinish;                               //  �Ƿ����    ���߳���λ   0 δ��� 1 ��ɣ�
	int             nCases;                                 //  �߳�case��
	CASE_TYPE       pcases[THRD_CASE_NUM];                  //  �߳�case
}CALC_THRD_CASE;
/******************************************************************************/

//  �ⲿ�궨��
/******************************************************************************/
//////////////////////////////////////////////////////////////////////////
extern  bool                   g_ProcessExitFlag ;                  //  �����˳���־
extern  int                    g_nCalcThrd       ;                  //  �̸߳���
extern  CALC_THRD_CASE *       g_pCalcThrdCase   ;                  //  �߳�case
////////////////////////////////////////////////////////////////////////////////
////  thread���ʻ����������ͽ�������
#ifdef _WINDOWS32
	extern "C" void    THRD_LockThreadMutex(    HANDLE  mutex ); 
	extern "C" void    THRD_UnLockThreadMutex(  HANDLE  mutex ); 
	extern "C" void    THRD_InitThreadMutex(    HANDLE  &mutex, char * mutexName ); 
	extern "C" void    THRD_ReleaseThreadMutex( HANDLE  mutex ); 

    extern "C" int     ClacCtlMainProcess( int nCase,  CASE_TYPE * pCase );

	extern "C" unsigned long WINAPI CalcThreadMain(void *param);
	extern unsigned __stdcall CallDynFileTMain(void *para);

	extern "C" unsigned long WINAPI ThreadFunc_RealtimeCurveProcess (void *lparm);
	extern "C" int     Thred_NetTableGetRecNum(int iAppNo, int iTableNo );
	extern "C" void    Thred_UpdateLfoCurve( TAB_LFO_RT_CURVE RtCurve  ) ;

	extern "C" unsigned long WINAPI ThreadFunc_MessageProcess(void *lparm);
#else
	extern "C" void    mypthread_mutex_unlock(void* mymutex);  //  AIX�������·�װ�������������
	extern "C" void    THRD_LockThreadMutex(    pthread_mutex_t  mutex, int oldtype ); 
	extern "C" void    THRD_UnLockThreadMutex(    pthread_mutex_t  mutex, int oldtype );
	//extern "C" void    THRD_UnLockThreadMutex(  pthread_mutex_t  mutex, int oldtype ); 
	extern "C" void    THRD_InitThreadMutex(    pthread_mutex_t  mutex, char * mutexName ); 
	extern "C" void    THRD_ReleaseThreadMutex( pthread_mutex_t  mutex ); 

    extern "C" int     ClacCtlMainProcess( int nCase,  CASE_TYPE * pCase );
	extern "C" void    CalcThreadMain(void *param);
	extern void *CallDynFileTMain(void *);

	extern "C" void    ThreadFunc_RealtimeCurveProcess (void *lparm);
	extern "C" int     Thred_NetTableGetRecNum(int iAppNo, int iTableNo );
	extern "C" void    Thred_UpdateLfoCurve( TAB_LFO_RT_CURVE RtCurve  ) ;

	extern "C" void    ThreadFunc_MessageProcess(void *lparm);
#endif
////////////////////////////////////////////////////////////////////////////////



class CLfoAnaObj : public CPubFun, public CPubOdbApi, public CPubWamsData
{

public:
	////  ���ò������£�
    int                  para_nFcMaxMem;                    //  ���ڴ泧վ�����
    int                  para_nGnMaxMem;                    //  ���ڴ���������
    int                  para_nLnMaxMem;                    //  ���ڴ���·�����
    int                  para_nTrMaxMem;                    //  ���ڴ��ѹ�������
	int                  para_nAnaMaxFcNum;                 //  ������վ�����
    int                  para_nItfcMaxMem;                  //  ���ڴ���������
	int                  para_nItfcMaxDev;                  //  ���ڴ���������·��
	int                  para_nShwMaxFcNum;                 //  ÿȺ��ʾ��վ�����
	// ���Ӱٷֱȿ���
	int                  para_nCaseMaxMem;                  //  �������� (para_nAnaMaxFcNum+para_nGnMaxMem+para_nLnMaxMem+para_nTrMaxMem)
	int                  para_LfoAnaByPwr;                  //  ����ѡ��
	int                  para_LfoAnaByEEAC;                 //  ��ʽѡ��
    int                  para_nSysMaxMode;                  //  ����sysmode��
    int                  para_nObjMaxMode;                  //  ����objmode��(3*para_nCaseMaxMem)
	// ��ʾ���߿���
    int                  para_nDisplyCurve;                 //  ���ڴ�������
	// �����ͺ����������Ӻͷ�����5s��
    int                  para_nDelaySec;                    //  ȡ���������Ӻ�����
	int                  para_CalcPQVByUI;                  //  �Ƿ����PQVѡ��
    int                  para_CalcAngByEA;                  //  �Ƿ���㹦��  2008-05-22 by zhw
    // 2008-04-06 added by zhw at yn
    int                  para_SaveAllFac;                   //  tmd�Ƿ񱣴����г�վ����
    int                  para_CallAllFacDyn;                //  �Ƿ��ٻ����г�վ�ļ�
    time_t               para_nGpsTime;                     //  ���ݰ�������ʱGpsʱ��
    int                  para_SaveDataToFile;                 //  ���ݱ��濪�� 2008-06-10 by zhw
    int                  para_DataSaveNum;                    //  �����ļ��������
    int                  para_nAsstDeciMax;                   //  �������������  2008-09-09
    int                  para_SaveXmlFile;                    //  �Ƿ�����XML�ļ�
    char                 para_XmlFileDir[MAXBUF];          //  ɽ��Ӧ��ϵͳ�ӿ��ļ�Ŀ¼
    vector<string>       m_XmlFileName;                         //  �����ļ�����ftp����ʱʹ��

	// ����E���Ը�ʽ�ӿ��ļ�
	int                  para_SaveResFile;                    // 1,�����ļ�
	char                 para_FtpServer[MAXBUF];                      // FTP������IP
	char                 para_FtpUser[MAXBUF];           // FTP�������û���
	char                 para_FtpPsw[MAXBUF];			  // ����
	char                 para_ResFileName[MAXBUF];       // ����ļ���
	char                 para_FtpDir[MAXBUF];            // FTPĿ¼
	char                 para_FtpCmd[MAXBUF];            // ������
    int                  para_DebugInfo;                      //  ������Ϣ����  // 2008-07-16
    float                para_Ampl_aver;                      //  ���бֱ�����⣬����ģʽ��ֵռ���ξ�ֵ��  // 2008-08-12
    float                para_FcVoltPer;                      //  ��վ��ѹԤ�аٷֱ�  // 2008-08-13
    float                para_VoltAmplPer;                    //  ��վ��ѹ��ֵ�ٷֱ�  // 2008-08-14
    float                para_MaxAnglDiff;                    //  ����ͻ��ǶȲ�
    int                  para_IsCallDynFile;                  //  �Ƿ��ٻ�100֡�ļ�
    int                  para_IsCallComtrFile;                //  �Ƿ��ٻ�¼���ļ�
    int                  para_CallDynDelaySec;                //  �ٻ�100֡�ļ��ӳ�ʱ��
    int                  para_IsShowAnaRes;                   //  TMD�Ƿ���ʾ�������,��������Ϣ
    int                  para_IsShowAsstDeci;                 //  TMD�Ƿ���ʾ��������
    int                  para_IsShowAnaData;                  //  TMD�Ƿ���ʾ�豸��㣬Ƶ�ʣ��������Ϣ
    int                  para_IsFactByClust;                  //  �ڷ�Ⱥ�����ѡ���ֵ�����豸,��Ϊ�������ӵĻ�׼
    int                  para_UpAppVariable;                  //  1,WAMSӦ��ģ��澯��Ϣ��;0,������
    int                  para_FormAsstDeci;                   //  �γɸ�������(�ϲ�)
	int                  para_UpDevLfoStat;                   //  ���¹����豸��LFO״̬
	int                  para_IsDataCycle;                    //  1,ѭ����ȡ����;0,��ѭ����ȡ����
public:
    int                  para_FilePmuInfo;                   //  DevicePmuInfo.txt �豸�����Ϣ
    // 0�������ļ�; 1�����ļ���׷������; 2���Ǵ����ļ�
    int                  para_FileModeTxt;                   //  AllObjMode.txt ModeShape.txt LfoCentor.txt
    // 0�����; 1��������Ҫ��Ϣ; 2���ģʽ��Ϣ; 3�������
    int                  para_DebugProny;                    //  PRONY����ѡ��
    int                  para_IsFltSim;                      // �Ƿ����ģ����;1,����;0,������;Ĭ��ֵΪ0
    int                  para_IsWarnIA;                     // �Ƿ����ۺ����ܸ澯������Ϣ(�ƶ�);1,����;0,������
    int                  para_ShowFacName;                   // �Ƿ���ʾ��վ����;1,��ʾ;0,����ʾ;Ĭ��Ϊ1
    int                  para_WarnAppNo;                     // d5000���͸澯��Ӧ��;700000
    int                  para_IsDataValid;                     // �Ƿ��ж�����״̬;1,�ж�;0,���ж�;Ĭ��ֵΪ0
public:
	// ���ݴ������
    int                  m_nSmpFre;                         //  ����Ƶ��
    int                  m_nSmpStp;                         //  �������
	time_t               m_tNewSec;                         //  ȡ��GPSʱ��(m_CurGpsTime-m_nMovSec) 
    int                  m_nMovSec;                         //  ȡ���ƶ�����
    int                  m_iOffPos;                         //  ȡ��ƫ��λ��
	time_t               m_CalcStartTime;                   //  ������ʼGPSʱ��(m_CurGpsTime-time_leng) 
    int                  m_iCalcOffPos;                     //  ����ƫ��λ��
	time_t               m_CurGpsTime;                      //  ��ǰGPSʱ��(��ֹGPSʱ��)
	time_t               m_OldGpsTime;                      //  �ϴ�GPSʱ��(��ֹGPSʱ��)
public:
	float                m_nOrigDlt;					    //  ԭʼ��������
	int                  m_nOrigSam;					    //  ԭʼ���������
	float                m_Matchtime[DATA_NUM_MAX];         //  ƥ��ʱ������
	int                  m_nSamples ;					    //  �����ò��������
	int                  m_nWarnWdsnum ;					//  �񵴴�����
	float				 m_fAverFreq;						//	ƽ��Ƶ��
	float                m_ProMatchtime[DATA_NUM_MAX];      //  ������ƥ��ʱ��
public:
	int                  m_setParaFlag;                     //  ��������ļ�
	int                  m_reAllocmem;                      //  �����ڴ�
	int                  m_LfoAnaByPwr;                     //  �������أ�1 �й� 0 �����ǣ�

public:
	int                  m_DataFileAskPnt;                  //  ���������ļ�Ҫ���ȡ������
	int                  m_FcDataFilePos;                   //  �����ļ���ȡλ��
	int                  m_FcDataFileOffset;                //  ��������ļ���ȡλ�õ�ƫ����
	int                  m_FcDataFileAskPnt;                //  ���������ļ�Ҫ���ȡ������
	int                  m_GnDataFilePos;                   //  �����ļ���ȡλ��
	int                  m_GnDataFileOffset;                //  ��������ļ���ȡλ�õ�ƫ����
	int                  m_GnDataFileAskPnt;                //  ���������ļ�Ҫ���ȡ������
	int                  m_LnDataFilePos;                   //  �����ļ���ȡλ��
	int                  m_LnDataFileOffset;                //  ��������ļ���ȡλ�õ�ƫ����
	int                  m_LnDataFileAskPnt;                //  ���������ļ�Ҫ���ȡ������
    int                  m_TrDataFilePos;                   //  �����ļ���ȡλ��
    int                  m_TrDataFileOffset;                //  ��������ļ���ȡλ�õ�ƫ����
	int                  m_TrDataFileAskPnt;                //  ���������ļ�Ҫ���ȡ������
    int                  m_FcVolFilePos;                    //  ��վ��ѹ��ȡλ��  //fac_volt.txt
    int                  m_FcAngFilePos;                    //  ��վ��Ƕ�ȡλ��  //fac_angle.txt
    int                  m_FcVolFileOffset;                 //  ��վ��ѹ��ȡλ�õ�ƫ����
    int                  m_FcAngFileOffset;                 //  ��վ��Ƕ�ȡλ�õ�ƫ����
    int                  m_GnPowFilePos;                    //  ������й���ȡλ��  //gn_power.txt
    int                  m_GnAngFilePos;                    //  ��������Ƕ�ȡλ��  //gn_angle.txt
    int                  m_GnPowFileOffset;                 //  ������й���ȡλ�õ�ƫ����
    int                  m_GnAngFileOffset;                 //  ��������Ƕ�ȡλ�õ�ƫ����
    int                  m_CallHDFile;                      //  �Ƿ������ٻ���¼���ļ���־
	int                  m_HDFileOk;						//  ��¼���ļ��ٻ���ɱ�־

public:
    PARA_LFO_ANA         m_LfoPara;                         //  ������Ϣ
	float                para_AnaWnapPer;                 //  ����������ֵ
    PARA_DISP_CTL        m_DispCtl;                         //  �������
    PARA_PROC_CTL        m_ProcCtl;                         //  ������Ϣ
public:
    int                  m_nFcNum;                          //  LFO��վ����
    SSO_FC_INF *         m_pFcInf;                          //  LFO��վ��̬��Ϣ
    SSO_FC_CTL *         m_pFcCtl;                          //  LFO��վ������Ϣ
    LFO_FC_PMU *         m_pFcPmu;                          //  LFO��վPMU��Ϣ
    LFO_FC_DAT *         m_pFcDat;                          //  LFO��վ��̬����
    SSO_FC_MON *         m_pFcMon;                          //  LFO��վ������Ϣ
    SSO_FC_MON *         m_pFcLfo;                          //  LFO��վ����Ϣ
	int                  m_nFcProDat;                       //  �账��ĳ�վ
	LFO_FC_DAT *         m_pFcProDat;                       //  �账��ĳ�վ����((Ԥ�����)
    LFO_FC_ANA *         m_pFcAna;                          //  LFO��վ������Ϣ
	int					 m_nFcWarn;                         //  ���ڸ澯��վ��
    SSO_FC_MON *         m_pFcWarn;                         //  ���ڸ澯��վ��Ϣ
	int					 m_nEveFcWarn;                      //  �¼��澯��վ��
    SSO_FC_MON *         m_pEveFcWarn;                      //  �¼��澯��վ��Ϣ
	int					 m_nFcHDFile;						//  �ٻ���¼���ļ���վ����
	SSO_HD_FILE_FAC	*	 m_pFcHDFile;						//  �ٻ���¼���ļ���վ��Ϣ
	vector<FES_COMM_TAB_CRE> m_vCOMMFc;						//  ǰ��ͨѶ��վ��
public:
    int                  m_nGnNum;                          //  LFO����̨��
    SSO_GN_INF *         m_pGnInf;                          //  LFO���龲̬��Ϣ
    SSO_GN_CTL *         m_pGnCtl;                          //  LFO���������Ϣ
    SSO_GN_PMU *         m_pGnPmu;                          //  LFO����PMU��Ϣ
    SSO_GN_DAT *         m_pGnDat;                          //  LFO���鶯̬����
    SSO_GN_MON *         m_pGnMon;                          //  LFO���������Ϣ
	OSCILLAT_FLAG *      m_pGnOsc;                          //  LFO�����񵴱�־
    OBJ_ACT_FACT *       m_pGnFact;                        //  LFO�����������  2008-04-04 added by zhw at yn
	int                  m_nGnProDat;                       //  �账��Ļ���
	SSO_GN_DAT *         m_pGnProDat;                       //  �账��Ļ�������(Ԥ�����)
	int                  m_nGnWarn;
	int					 sum_WarnGen;
	SSO_GN_MON *         m_pGnWarn;
 	LFO_OBJ_RES_SAVE *   m_pGnResSave;                      //  �����豸���
	SSO_GN_MON *         m_DevGnWarn;							//  �澯�豸���
	SSO_GN_MON *         sum_DevGnWarn;
    SSO_GN_DAT *         sum_pGnDat;                          //  LFO���鶯̬����
    SSO_GN_INF *         sum_pGnInf;      
	TYPE_GN_WARN_TIME *  gn_warn_time;
	multimap<INT_REC, int>				g_map_gn_fac;//�������վID

public:
    int                  m_nLnNum;                          //  LFO��·��
    SSO_LN_INF *         m_pLnInf;                          //  LFO��·��̬��Ϣ
    SSO_LN_CTL *         m_pLnCtl;                          //  LFO��·������Ϣ
    SSO_LN_PMU *         m_pLnPmu;                          //  LFO��·PMU��Ϣ
	SSO_LN_DAT *         m_pLnDat;                          //  LFO��·��̬����
    SSO_LN_MON *         m_pLnMon;                          //  LFO��·������Ϣ
	OSCILLAT_FLAG *      m_pLnOsc;                          //  LFO��·�񵴱�־
    OBJ_ACT_FACT *       m_pLnFact;                        //  LFO��·��������  2008-05-21 added by zhw at nj
	int                  m_nLnProDat;                       //  �账�����·
	SSO_LN_DAT *         m_pLnProDat;                       //  �账�����·����(Ԥ�����)
	int                  m_nLnWarn;
	int					 sum_WarnLn;
	SSO_LN_MON *         m_pLnWarn;
 	LFO_OBJ_RES_SAVE *   m_pLnResSave;                      //  �����豸���
	SSO_LN_MON *         m_DevLnWarn;							//  �澯�豸���
	SSO_LN_MON *		 sum_DevLnWarn;
    SSO_LN_DAT *         sum_pLnDat;      
    SSO_LN_INF *         sum_pLnInf;    
	TYPE_LN_WARN_TIME *  ln_warn_time;
	multimap<INT_REC, INT_REC>				g_map_ac_dot;//�����߶ζ˵�id
	map<INT_REC, int>				g_map_acln_dot;//�����߶ζ˵�id
	vector<INT_REC>		 m_vAcDot_id; // ������ʾ����Ҫ��Ϊ�Ķ˵�

public:
    int                  m_nTrNum;                          //  LFO��ѹ����
    SSO_TR_INF *         m_pTrInf;                          //  LFO��ѹ����̬��Ϣ
    SSO_TR_CTL *         m_pTrCtl;                          //  LFO��ѹ��������Ϣ
    SSO_TR_PMU *         m_pTrPmu;                          //  LFO��ѹ��PMU��Ϣ
    SSO_TR_DAT *         m_pTrDat;                          //  LFO��ѹ����̬����
    SSO_TR_MON *         m_pTrMon;                          //  LFO��ѹ��������Ϣ
    OSCILLAT_FLAG *      m_pTrOsc;                          //  LFO��ѹ���񵴱�־
    int                  m_nTrProDat;                       //  �账��ı�ѹ��
    SSO_TR_DAT *         m_pTrProDat;                       //  �账��ı�ѹ������(Ԥ�����)
    int                  m_nTrWarn;
	int					 sum_WarnTr;
    SSO_TR_MON *         m_pTrWarn;
 	LFO_OBJ_RES_SAVE *   m_pTrResSave;                      //  ���ӱ�ѹ�����
	SSO_TR_MON *         m_DevTrWarn;							//  �澯�豸���
	SSO_TR_MON *		 sum_DevTrWarn;
    SSO_TR_DAT *         sum_pTrDat; 
    SSO_TR_INF *         sum_pTrInf;   
	TYPE_TR_WARN_TIME *  tr_warn_time;
	multimap<INT_REC, int>				g_map_tr_fac;//��ѹ����վID

public:
	bool                 m_LfoAmplOverLmt;                  //  LFOʱ�����Խ�ޱ�־
    SSO_SYS_MON          m_SysMon;                          //  LFOϵͳ������Ϣ
    SSO_SYS_MON          m_SysMon_Old;                      //  LFOϵͳǰһ�ּ�����Ϣ
    LFO_SYS_LFO          m_SysLfo;                          //  LFOϵͳ����Ϣ
    LFO_SYS_ANA          m_SysAna;                          //  LFOϵͳ������Ϣ
	int					 m_nDevWarn;						//  �������豸����
	SSO_DEV_MON	*		 m_pDevWarn;						//	�������豸��Ϣ
	int					 m_nDevEveWarn;						//  �¼����豸����
	SSO_DEV_MON	*		 m_pDevEveWarn;						//	�¼����豸��Ϣ
	vector<FAD_TRANS_REPORT_STRUCT> m_vTranReport;			//  �ٻ�¼���ļ����ر�־
public:
	//////////////////////////////////////////////////////////////////////////
	int                  m_nCases;                          //  ��������
	CASE_TYPE *          m_pCases;                          //  ������Ϣ
	//int                  m_nCalcThrd;                       //  �̸߳���
	//CALC_THRD_CASE *     m_pCalcThrdCase;                   //  �߳�case
	int                  m_wRoleID;                         //  �ڵ��ɫ
	//////////////////////////////////////////////////////////////////////////
public:
	int                  m_nMonMode;	                    //  mon ģʽ��
	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon ģʽ
	///////////////////////////////������ʾ///////////////////////////////
public:
	// ������ʾLFOʵ��������
	#ifdef _WINDOWS32
		HANDLE            m_RtCurvemutex;                   //  ʵʱ���߻�����
		HANDLE            m_DynFilemutex;                   //  �ٻ�100֡�ļ�������
	#else
		pthread_mutex_t   m_RtCurvemutex;                   //  ʵʱ���߻�����
		pthread_mutex_t   m_DynFilemutex;
	#endif
	SSO_GN_PMU            m_LfoCurveGenDev ;                //  ʵʱ���߶�Ӧ�����
	SSO_LN_PMU            m_LfoCurveLineDev;                //  ʵʱ���߶�Ӧ������
    SSO_TR_PMU            m_LfoCurveTrDev;                  //  ʵʱ���߶�Ӧ��ѹ��
// 	RTIME_CUR_DEV         m_LfoCurveGenDev ;                //  ʵʱ���߶�Ӧ�����
// 	RTIME_CUR_DEV         m_LfoCurveLineDev;                //  ʵʱ���߶�Ӧ������
	TAB_LFO_RT_CURVE      m_GenLfoCurve  ;                  //  �������Ӧʵʱ����(����)
	TAB_LFO_RT_CURVE      m_AclnLfoCurve ;                  //  �����߶�Ӧʵʱ����(�й�)
	TAB_LFO_RT_CURVE      m_TimeLfoCurve ;                  //  ʵʱ���߶�Ӧʱ��
    TAB_LFO_RT_CURVE      m_GenPowCurve  ;                  //  �������Ӧʵʱ����(�й�)  2008-06-11 by zhw
    TAB_LFO_RT_CURVE      m_TrLfoCurve   ;                  //  ��ѹ��ʵʱ����(�й�)
	// ������ʾLFOʵ����ģʽ����
	PERIOD_LEAD_MODE      m_pLfoModeCurve[LFO_CURVE_MAX_PNT];//  ����ģʽ����
	TAB_SSO_RT_CURVE2	  m_SsoCurve2[LFO_CURVE_MAX_PNT];  // ����SSOʵʱ���ߺ����
public:
	#ifdef _WINDOWS32
		HANDLE           m_Itfcmutex;                       //  ���滥����
	#else
		pthread_mutex_t  m_Itfcmutex;                       //  ���滥����
	#endif
    //int                  m_nItfcNum;                        //  LFO���������
    //LFO_INTERFACE *      m_pItfcDat;                        //  LFO���������Ϣ
    //int                  m_nItfcDevNum;                     //  LFO������·��
    //LFO_INTERFACE_DEV *  m_pItfcDevInf;                     //  LFO������·��Ϣ
    //SSO_LN_PMU *         m_pItfcDevPmu;                     //  LFO������·PMU��Ϣ
	//LFO_LN_DAT *         m_pItfcDevDat;                     //  LFO������·��̬����(Ԥ�����)
////////////////////////////////����//////////////////////////////////////
public:
	//LFO_REF_ANG_DAT
	// ��ǲο���ѡȡ
    INT_REC              m_AngRefID;                        //  �ο����id
	float                m_AngRefDat[DATA_NUM_MAX];         //  �ο��������
	float                m_AngRefDat2[DATA_NUM_MAX];        //  �ο��������
public:
	int                  m_nAllObjMode;                     //  ������ģʽ����(�޳���ͬģʽ)
	MODE_PART_OBJ *      m_pAllObjMode;                     //  ������ģʽ
	int                  m_nAllMode;                        //  δɸѡģʽ����
	MODE_CHRCT *         m_pAllMode;                        //  δɸѡģʽ
	int                  m_nSltMode;                        //  ϵͳģʽ����
	MODE_CHRCT *         m_pSltMode;                        //  ϵͳģʽ(��Ⱥϸ��)
	MODE_CHRCT           m_DispMode;
	//GEN_LEAD_CLUS *      m_GnLeadClus;
	//���� m_pSltMode �еĻ����򲻲������ģʽ
	int                  m_nLfoMode;                        //  LFO�ڼ�ģʽ����
	MODE_CHRCT *         m_pLfoMode;                        //  LFO�ڼ�ģʽ
    LFO_CALC_WDS *       m_pLfoWds;                         //  LFO�ڼ���㴰��
// ���������� 
public:
	//////////////////////////////////////////////////////////////////////////
	int                 m_nCntrFc;                         //  �����ĳ�վ��
	SSO_FC_MON *        m_pCntrFc;                         //  �����ĳ�վ��Ϣ
	int                 m_nCntrLn;                         //  �����ĳ�վ��
    LFO_CNTR_LN *       m_pCntrLn;                         //  LFO��������·
public:
// �������ƴ�ʩ��Ϣ(���������ĺͲ�������)
#ifdef _LFO_ASST_DECI_
    int                 m_nAsstDeci;                       //  ����������
    TAB_LFO_ASST_DECI * m_pAsstDeci;
#endif
	// �����ʣ�����/���⣻��Ϣ������
	// ˵����˫��
	// ˵����������
//////////////////////////////////////////////////////////////////////////
	
//////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS32
	HANDLE          m_logmutex;                             //  ��־���ʻ�����
#else
	pthread_mutex_t m_logmutex;                             //  ��־���ʻ�����
#endif
	char    m_backslash[2];
	char    m_ProcName[20];                         //  ������ϢĿ¼
	char    m_RunInfDir[MAXBUF];                         //  ������ϢĿ¼
	char    m_CasefDir[MAXBUF];                         //  �����ļ�Ŀ¼
	char    m_LogString[MAXBUF];                         //  ��־�ļ��ַ���
	void    MessShow( const char * logstr ,const int logtype = LTINFO);
	//void    WriteColIDtoLogFile() ;
public:
    // 2008-06-10 by zhw at nj
    char   m_SaveFileDir[MAXBUF];
	char   m_DataDir[MAXBUF];
    char   m_DataFileDir[MAXBUF];
    char   m_HDFileDir[MAXBUF];//��¼���ļ�Ŀ¼ $D5000_HOME/var/pmu_offline_file/comtr_file
    string   m_LnPowFileName;
    string   m_FacVoltFileName;
    string   m_FacAngFileName;
    string   m_GnPowFileName;
    string   m_GnAngFileName;
    string   m_TrPowFileName;
    string   m_LfoResFileName;
	// �����ļ�Ŀ¼�� 2008-09-26
	char   m_DataFile[MAXBUF];
	char   m_DataFile1[MAXBUF];
	char   m_DataFile2[MAXBUF];
	char   m_DataFile3[MAXBUF];
	char   m_DataFile4[MAXBUF];
    void   WriteDataToFile( int nType, int realNum, float t_start, float t_end, float t_delta, int nOffSec );  // ���ݱ���
	int    CheckDataFile(const char* fileName);    //�����ļ�������	
	char*  TrimLeft(char* str);                               //  ȥ���ַ�����˵Ŀո�
public:
	 CLfoAnaObj();
	~CLfoAnaObj();
public:
	int     ReadDefaultFile();                              //  ����������Ϣ
	bool    AllocMemforPointer();                           //  �����ڴ�ռ�
    void    ReleaseMem();                                   //  �ͷ��ڴ�
public:
    bool    InitPrject();                                         //  ��ȡ������Ϣ
    bool    InitFc(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YC_TAB_CRE> &VecFesYcTab);  //  ��վ��Ϣ
    bool    InitLn(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YX_YC> &VecFesYcTab);  //  ��·��Ϣ
    bool    InitGn(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YX_YC> &VecFesYcTab);  //  ������Ϣ
    bool    InitItfc(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YC_TAB_CRE> &VecFesYcTab);  //  ������Ϣ
    bool    InitTr(vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<FES_YX_YC> &VecFesYcTab);  //  ��ѹ����Ϣ
//     int     GetFacInfo(vector<FAC_TAB_CRE> &VecFacTab);
// 	int     GetAreaInfo(vector<AREA_TAB_CRE> &VecAreaTab);
// 	int     GetVLTypeInfo(vector<VL_TYPE_TAB_CRE> &VecVLTypeTab);
// 	int     GetFesYcInfo(vector<FES_YC_TAB_CRE> &VecFesYcTab);
// 	int     GetBusInfo(vector<BUS_TAB_CRE> &VecBusTab);
// 	int     GetAclnInfo(vector<ACLINE_TAB_CRE> &VecAclnTab);
// 	int     GetAclnDotInfo(vector<ACLNDOT_TAB_CRE> &VecAclnDotTab);
// 	int     GetGenInfo(vector<GEN_TAB_CRE> &VecGenTab);
// 	int     GetTrInfo(vector<TR_TAB_CRE> &VecTrTab);
// 	int     GetTrwdInfo(vector<TRWD_TAB_CRE> &VecTrwdTab);
	int     GetSsoFac(vector<SSO_FC_TAB_CRE> &VecLfoFcTab);
	int     GetSsoAcln(vector<SSO_LN_TAB_CRE> &VecLfoAclnTab);
	int     GetSsoGen(vector<SSO_GN_TAB_CRE> &VecLfoGnTab);
	int     GetSsoTr(vector<SSO_TR_TAB_CRE> &VecLfoTrTab);
	int     WritePmuInfo(const int devtableno);
	int     GetLfoRTCurveId(vector<TAB_SSO_RT_CURVE_ID> &VecLfoRt);
public:
	void    InitialLFOEstimate();                           //  ÿ�ּ����ʼ������
    int     CheckCalcCtlPara( ) ;                           //  �������
public:
	int     HandlingPmuSampleData();                        //  ��ȡ���ݣ����������ļ���
	int     GetFcData( float &t_start, float &t_end, float &t_delta, int dsource = 0 );//  ��ȡ��վ����
    int     GetLnData( float &t_start, float &t_end, float &t_delta, int dsource = 0 );//  ��ȡ��·����
    int     GetGnData( float &t_start, float &t_end, float &t_delta, int dsource = 0 );//  ��ȡ��������
    int     GetTrData( float &t_start, float &t_end, float &t_delta, int dsource = 0 );//  ��ȡ��ѹ������
    int     para_FileType;                                  //  �ļ����� 0:SIM 1:DYN
public:
	int     FormFcProData(  );                              //  ��վ���ݴ���
    int     FormLnProData(  );                              //  ��·���ݴ���
    int     FormGnProData(  );                              //  �������ݴ���
public:
	void    MainProcess();
	int     FormMonProData();                               //  mon���ݴ���
	void    FormMonCaseData();
	void    MonClacProcess();                               //  LFO�ж�����
	int     EstimateObjOscillate();							//  �豸��ͬ�����ж�
	int     EstimateSysOscillate();							//  ϵͳ��ͬ�����ж�
	void	PushCallFac();									//	�γ��ٻ���վ����
	void	PushEveFac();									//	ͳ�����¼��е��񵴳�վ
	bool    IsWarnDev( INT_REC dev_id , int &ii);									//	�ж����豸�ڱ������¼����Ƿ��Ѿ�ͳ�ƹ�
	bool    IsWarnFac( INT_REC fac_id , int &ii);									//	�ж����豸�ĳ�վ�Ƿ�ͳ�ƹ�
	int     SelectRefAngle( ); 
	int     SelectRefAngle2( ); 
	int     FormAnaProData( );                              //  ana���ݴ���
	void    FormAnaCaseData( );
	void    AnaCalcProcess();                               //  LFO��������
	int     GetVarNsquare(int var, int n);
public:
	//  eeac
	void    WAMAPDataHandling();
    void    EEACIdentifyCluster();
public:
	//  prony
//	int     PronyAnalysisProcess( CASE_TYPE * case_pointer );
//	void    ClacCtlMainProcess( int nCases,  CASE_TYPE * pCase ); // ������ҵ����()
public:
	//  mon
	void    ModeResultHandling( float damp_lim );           //  ģʽ�����Ϣ
	void    ExtractLeadMode();                              //
//	void    EstimateObjOscillate();                         //  �жϼ��Ӷ���(�ٽ�Ⱥ)�����
//	void    EstimateSysOscillate();                         //  �ж�ϵͳ�����(ˢ��ϵͳ״̬�������澯)
    void    EstimateSingleObjState( OSCILLAT_FLAG  *osciFlag, LFO_LEAD_MODE *leadMode, int times, float SameFreqDifVal );
	void    UpdateMonLfoToWAMAP();                          // ���߼�����Ϣ
public:
	//  ana
	void    LfoClctMainMode(); // case result   -> m_pAllObjMode	
    void    LfoAnaEEACIdentifyCluster();
	void    LfoAnaPartGenPreHandle( int nGnNum, SSO_GN_INF *pGnInf, int nPartGn, MODE_PART_OBJ *pPartGn);
	void    LfoAnaSetPartFact0(   int nPartGn, MODE_PART_OBJ *pPartGn );
	//      2008-09-24 lsf add ͳһ�������ӻ�׼
	void    LfoAnaSetLnClustbyFc( int nPartLn, MODE_PART_OBJ *pPartLn, int nPartFc, MODE_PART_OBJ *pPartFc );
	void    LfoAnaSetTrClustbyFc( int nPartTr, MODE_PART_OBJ *pPartTr, int nPartFc, MODE_PART_OBJ *pPartFc );
	void    LfoAnaSetGnAmpl(      int nPartGn, MODE_PART_OBJ *pPartGn );
	void    LfoAnaResetFact(      MODE_CHRCT *pSltMode );
	//////////////////////////////////////////////////////////////////////////
	void    CalcGenClustorPcoi();
	void    CalcGenOpstPcoiCurv();
	void    UpdateAnaInfToWAMAP();                          // ���߷�����Ϣ
public:
	void    DecideLfoCurveID() ;                            //  ȷ��ʵʱ���߶�Ӧ�豸
	void    FormMonModeCurce();                             //  �γ�ģʽ����
	void    UpdateMonInfToWAMAP();                          //  ʵʱ������Ϣ	
	void    FormItfcCurce();                                //  �γɶ�������
	void	UpdateLfoWarnDev( );	//	���豸����
public:
	THIS_LFO_INFO        m_ThisLfoInfo;                     //  ���ε�Ƶ����Ϣ(LFO��ʧ��ͳ��) 
	int                  m_FormResSaveStat;                 //  ���ϱ���״̬
	int                  m_WarnSendFlag;                    //  �澯����
	LFO_SYS_RES_SAVE     m_SysResSave ;                     //  ϵͳ���
    int                  m_CallDynFile;                     //  �ٻ�100֡�ļ���־
	//  FormLfoSaveResult
	void    FormLfoResultProcess( );
    void    FormAsstDeci();                                 // �γɸ�������
    void    InitLfoResultSaveStruct( ) ;
	void    FormLfoMonSaveResult( ) ;
	void    FormLfoAnaSaveResult( );
	void    SaveResultFileTxt( ) ; 
    void    WriteMonResult( );                            //  ɽ��Ӧ��ϵͳ�ӿ��ļ�  // 2008-06-24 by zhw
    void    CreateHisFtpFile(vector<string> &fileName);
	void    SendLfoResFile();                             // ����E���Ը�ʽ�ӿ��ļ�
    int     CallHDFile();               //  �ٻ���¼���ļ�  //  2008-08-19
	bool	HDFileOk();					//	�ж��Ƿ���Կ�ʼ�ٻ���¼���ļ�
	bool	HDFileBack();				//	�ж��ٻ��ĳ�¼���ļ��Ƿ��Ѿ�����
	void	StartSSOAna();				//	����sso��ϸ������
public:
	//  FAT
    int     m_acdot_id;
    int     m_isForceSet;
    short   m_filNo;
	short   m_colNo;
    //bool    GetPmuFilColNobyKeyId( KEY_ID_STRU ikeyid, short &filNo, short &colNo );// ����KeyId����Ż�ȡPMU�����ļ����к�
    void SetPmuFilColNoForTest( int ac_dot_id, int COL_ID, int nLnNum, SSO_LN_PMU * pLnPmu, int GN_COL_ID, int nGnNum, SSO_GN_PMU * pGnPmu,short ifilNo, short icolNo, int isForceSet );
	/////////////////////////////////�ӿڲ���/////////////////////////////////
public:
	//  ʵʱ��
	 // ����KeyId����Ż�ȡPMU�����ļ����к�
	//int     GetFileColumNoByKeyID( int fileno, int colno, MY_KEY_ID Mykeyid );
   //  ���ж��������أ�(0 ����, -1 ȡ��ʧ��, 1 ��¼������ )
    int     GetFcCtl( int num, SSO_FC_CTL *pFcCtl ) ;  //  LFO��վ��
    int     GetLnCtl( int num, SSO_LN_CTL *pLnCtl ) ;  //  LFO������·��
    int     GetGnCtl( int num, SSO_GN_CTL *pGnCtl ) ;  //  LFO�������
    int     GetTrCtl( int num, SSO_TR_CTL *pTrCtl ) ;  //  LFO��ѹ����
    int     GetLfoPara( PARA_LFO_ANA  &LfoPara ) ;  //  LFO��������һ����¼��
    int     GetDispCtl( PARA_DISP_CTL &DispCtl ) ;  //  LFO��������һ����¼��
    int     GetProcCtl( PARA_PROC_CTL &ProcCtl ) ;  //  LFO��������һ����¼��
	void    UpdateMonFcTable( int num, SSO_FC_INF *pfc_inf, int num_gn, SSO_GN_MON *pgn_mon,int num_tr, SSO_TR_MON *ptr_mon) ;   //  LFO��վ��
	void    UpdateMonFcTableStat( int num, SSO_FC_INF *pfc_inf, int num_gn, SSO_GN_MON *pgn_mon,int num_tr, SSO_TR_MON *ptr_mon) ;   //  LFO��վ��
	void    UpdateMonGnTable( int num, SSO_GN_INF *pgn_inf, SSO_GN_MON *pgn_mon ) ;   //  LFO�������
	void    UpdateMonGnTableStat( int num, SSO_GN_INF *pgn_inf, SSO_GN_MON *pgn_mon ) ;   //  LFO�������״̬
	void    UpdateonGnTableStat( int num, SSO_GN_INF *pgn_inf, SSO_GN_MON *pgn_mon ) ;   //  LFO�������
	void    UpdateMonLnTable( int num, SSO_LN_INF *pln_inf, SSO_LN_MON *pln_mon ) ;   //  LFO������·��״̬
	void    UpdateMonLnTableStat( int num, SSO_LN_INF *pln_inf, SSO_LN_MON *pln_mon ) ;   //  LFO������·��״̬
    void    UpdateMonTrTable( int num, SSO_TR_INF *ptr_inf, SSO_TR_MON *ptr_mon ) ;   //  LFO��ѹ����
    void    UpdateMonTrTableStat( int num, SSO_TR_INF *ptr_inf, SSO_TR_MON *ptr_mon ) ;   //  LFO��ѹ����
	void    UpdateMonMdTable( int num, MODE_PART_OBJ *pobj_mode );                    //  ʵʱ�豸ģʽȫ��
	void    UpdateMonSysTable( SSO_SYS_MON sys_mon ) ;                                //  LFO��������һ����¼��
	void    UpdateMonSysTableStat( SSO_SYS_MON sys_mon ) ;                                //  LFO��������һ����¼��
    int     UpdateAppVariable(SSO_SYS_MON sys_mon) ;                                  // ����WAMSӦ��ģ��澯��Ϣ��
	void    UpdateFcInfTableLfoStat(  int num, SSO_FC_MON *pfc_mon  ) ;               //  ��վ��Ϣ��LFO״̬
	void    UpdateGnDevTableLfoStat(  MODE_CHRCT &DispMode) ;                                             //  �������LFO״̬
	void    UpdateLnDevTableLfoStat( int num, SSO_LN_MON *pln_mon );                  //  �����߶α�LFO״̬
	void    UpdateLfoFcTable( int num, SSO_FC_INF *pfc_inf, SSO_FC_MON *pfc_lfo ) ;   //  lfo���
	void    UpdateLfoGnTable( int num, SSO_GN_INF *pgn_inf, LFO_OBJ_RES_SAVE *pgn_lfo ) ;
	void    UpdateLfoLnTable( int num, SSO_LN_INF *pln_inf, LFO_OBJ_RES_SAVE *pln_lfo ) ;
    void    UpdateLfoTrTable( int num, SSO_TR_INF *ptr_inf, LFO_OBJ_RES_SAVE *ptr_lfo ) ;
	void    UpdateLfoMdTable( int num, LFO_DEV_LEAD_MODE *pobj_mode );                //  ʵʱ�豸����ģʽ��
	void    UpdateLfoSysTable( ) ;                                //  LFO��������һ����¼��
    void    UpdateLfoHisTime( LFO_HIS_TIME his_time );
	// ana
	//void    UpdateAnaFcTable( int num, SSO_FC_INF *fc_inf, SSO_FC_MON *fc_stat ) ;  //  ana���
	//void    UpdateAnaGnTable( int num, SSO_GN_INF *gn_inf, LFO_GN_LFO *gn_stat ) ;
	//void    UpdateAnaLnTable( int num, SSO_LN_INF *ln_inf, LFO_LN_LFO *ln_stat ) ;
	void    UpdateAnaSysTable( LFO_SYS_ANA sys_ana ) ;                                //  LFO��������һ����¼��
	void    UpdateAnaCntrTable( int num, SSO_FC_INF *pfc_inf, SSO_FC_MON *pcntr_fc ); //  LFO��վ�����ֳ�վ��
	void    UpdateAnaShapTable( int num, MODE_CHRCT *pModeShap ) ;                    //  LFOʵʱģ̬��
    void    UpdateGnFact(int num, MODE_CHRCT *pModeShap );                                     //  LFO�������������� 2008-04-03 added by zhw at yn
    void    UpdateLnFact(int num, MODE_CHRCT *pModeShap );                                     //  LFO��·�������� 2008-05-21 added by zhw at nj
    void    UpdateAverFact( int num, MODE_CHRCT *pModeShap );                                  //  ��������ƽ��ֵ��� 2008-06-05 by zhw
	void    UpdateCasWndsTable( int num, LFO_CALC_WDS CalcWds, int is_clear = 0) ;    //  LFOʵʱcase���ڱ�
	void    UpdateCasAllShapeTable( int num, MODE_CHRCT *pModeShap ) ;                //  LFOʵʱcaseģ̬ȫ��
	//  ����������ʾ
	void    UpdateLfoCurve( TAB_LFO_RT_CURVE RtCurve  ) ;                            //  LFOʵʱ���߱���¼1��2��3��
	void    UpdateMonModeCurce(  PERIOD_LEAD_MODE * pLfoModeCurve ) ;                //  LFOʵʱ���߱���¼4��5��6��
	//void    UpdateItfcCurce(    int num, LFO_INTERFACE *pItfcCurve ) ;               //  LFO���涨���

	//  ��ϸ����������չʾ�澯�豸��������20141008
	void	UpdateWarnDevCurve( TAB_WARN_DEV_CURVE WarnCurve);
	void    RealtimeCurveProcess();
	//  ������
	//bool    GetLastTimeFromWAMAP(time_t *recordtime);                                //  ��ȡPMU���ݱ������һ��ˢ��ʱ��
	bool    GetFcDataFromWAMAP( LFO_FC_PMU iFcPmu, time_t tBgnSec, int nVecSec, LFO_FC_DAT &oFcDat, int nOffSec );       //��ȡ��վPMU��������
	bool    GetGnDataFromWAMAP( SSO_GN_PMU iGnPmu, time_t tBgnSec, int nVecSec, SSO_GN_DAT &oGnDat, int nOffSec );       //��ȡ����PMU��������
	bool    GetLnDataFromWAMAP( SSO_LN_PMU iLnPmu, time_t tBgnSec, int nVecSec, SSO_LN_DAT &oLnDat, int nOffSec );       //��ȡ��·PMU��������
    bool    GetTrDataFromWAMAP( SSO_TR_PMU iTrPmu, time_t tBgnSec, int nVecSec, SSO_TR_DAT &oTrDat, int nOffSec );       //��ȡ��·PMU��������
    bool    IWM_PmuDataCheck( PUB_WT_VEC_DATA *iPmudata, time_t tBgnSec, float * oVecData, VEC_DATA_STS &vecSts);
	//  ��Ϣ
	//int     IsOnDuty( int app_id, short ctx_no = AC_REALTIME_NO );
	//int     GetAppHostName( int app_id, char * host_name, short ctx_no = AC_REALTIME_NO );
    //  ��ȡ��������
    bool    GetAsstDeci(); // 2008-09-09 by zhw
    int     LFOSendWarnMessage(const SSO_SYS_MON status_info, int warn_time, INT_REC factory_id) ;
	int     LFOTriggerSample();
	int     SampleSsoPara();
	int     SampleHisDev();
	int     SampleHisCase();
	int     SampleHisShape();
	bool    SaveLfoCaseByTmd(  );        //���ͱ��� LFO case��Ϣ������TMD��¼PMU����
    int     LFOTriEmsSingle( SSO_SYS_MON sys_mon );         // ��EMSת��LFO��Ϣ  2008-05-31 by zhw at fj
    int     CallDynFile();               //  �ٻ�100֡�ļ�  //  2008-08-19
    int     IsDynFileExist( int fac_id, int bgn_t, int end_t );
    int     IsTrFileExist( int fac_id, int bgn_t, int end_t );
    int     FormDynFile(int fac_id,int beg_sec,int end_sec,vector<string> &dynVec);
    void    GetPmuType(string idcode,short &pmuType);
    void    GetDynTimeStr(int beg_sec,int end_sec,vector<string> &dynTimeVec);
    int     CallTrFile();
    bool    SendHisLfoMess();      //  �����߳�����Ϣ���������һ����ʱ��
    int     UpdateLfoEvent();  // ����LFO���¼���
    int     UpdateHisCase();  // ����LFOʵʱCASE���ڱ�
    int     UpdateHisShape();  // LFOʵʱCASEģ̬ȫ��
    int     SaveCaseFile();
	int     ReadNewData(const string tmpfilename, int &realNum, float &t_delta, int &nDataTime);
	int     SendWarnToIA(const int mode);  // ���ۺ����ܸ澯(�ƶ�)������Ϣ
	/////////////////////////////////���ݴ���/////////////////////////////////
public:
    bool    IsFcStsValid(const VEC_DATA_STS &vecSts)
    {
        return ( vecSts.nBadNum <= MAX_FRA_PER_SEC / 5 );
    }
	
    bool    IsLnStsValid(const VEC_DATA_STS &vecSts)
    {
        return ( vecSts.nBadNum <= MAX_FRA_PER_SEC / 5 );
    }
	
    bool    IsGnStsValid(const VEC_DATA_STS &vecSts)
    {
        return ( vecSts.nBadNum <= MAX_FRA_PER_SEC / 5 );
    }
	
    bool    IsTrStsValid(const VEC_DATA_STS &vecSts)
    {
        return ( vecSts.nBadNum <= MAX_FRA_PER_SEC / 5 );
    }

    int     IsFcDatValid( int no );
    int     IsLnDatValid( int no, int ChgTimCoe, float ChgMinVal );
    int     IsGnDatValid( int no, int ChgTimCoe, float ChgMinVal );
	int     IsAglDatValid( int nAgl, float * pAgl, float MaxChg );
    int     IsTrDatValid( int no, int ChgTimCoe, float ChgMinVal );
	
	//  �ж��������ݵ��Ƿ���ͻ�䡣��ͻ�䷵��-1, ������1)����������Բ� > (ǰʮ����Բ�֮��)��2) ����������Բ� > ����������ֵ
    float   JudgeTwoValDelta(float *data, int num, float max_change_lim, float delta_lim_val, int algorithm);
	int     CurveLfoPreEstimate( SEQ_BASE_INFO dataBaseInfo, float ampl_sec_lim, int check_aver, float aver_lim  );
	//////////////////////////////// ��վ�������ݷ��� /////////////////////////////////
public:
	int                  Date7_RunStatus;     //  ����״̬ 0�ȴ�  1���� 
	int                  m_RunCtrCom;         //  ���߷������з�ʽ����
    char                 m_szLfoMsg[256];     //  ������Ϣ
	int                  Date7_beginTime ;    //  ��ʼʱ��
	int                  Date7_timeLeng ;     //  Ҫ�����ʱ�䳤��
	int                  Date7_acln_id;       //  Ҫ������ĵ�����·
    int  Date7GetTimePara( int &begintime, int &endtime, int &MemMaxDate);
    int  Date7TimeParaCheck( int begintime, int endtime, int MemMaxDate);
    bool Date7LfoMsgCtxToWAMAP(char * Msg_Ctx, int if_firt = 0);
    int  Data7HandlingPmuSampleData();
    void Data7MainProcess( );	
	void Date7RealtimeCurveProcess( time_t endtime );//ˢ��ʵ������
	/////////////////////////////////�������/////////////////////////////////
public:
	//  ana
// 	void    WriteGnLeadClusToFile(  );
// 	void    WriteModeShapeToFile( );
//     void    WriteCurveBaseInfoToFile( );
    //void    WriteAllObjModeToFile( const int inObjMode, MODE_PART_OBJ *ipObjMode );
	void    WriteModeShapeToFile( const int inMode, MODE_CHRCT *ipMode, unsigned char isShowFac = 1  );
	void	WriteLfoCenterToFile( LFO_SYS_ANA SysAna, int nFcProInf, LFO_FC_ANA *pFcProInf);
private:
#ifdef _LFO_ASST_DECI_
    PARA_LFO_ASST        m_LfoAsstPara;                     //  �������Բ���
    LFO_ASST_PREFIX      m_LfoAsstfix;                      //  ��������ǰ��׺
    LFO_KEY_DEV          m_AsstDeciStat;
	LFO_KEY_DEV          m_AsstDeciStatold;                 // 2008-09-26
    int                  m_nAsstDeciInf;                    //  LFO�������Ը���
    LFO_ASST_DECI *      m_pAsstDeciInf;                    //  LFO����������Ϣ
    int                  m_nAsstDeciInfOld;                 //  LFO�������Ը���
    LFO_ASST_DECI *      m_pAsstDeciInfOld;                 //  LFO����������ʷ��Ϣ
    int             m_nCtlDevNum;
    LFO_CTL_DEV*    m_pCtlDevId;
    int             m_nCtlDevNumOld;
    LFO_CTL_DEV*    m_pCtlDevIdOld;
    int                  m_nDeciIndxInf;                    //  LFO������������
    LFO_DECI_INDEX *     m_pDeciIndxInf;                    //  LFO����������Ϣ
    
    int                  m_nCtlDeciInf;                     //  LFO���Ը���
    LFO_CTL_DECI   *     m_pCtlDeciInf;                     //  LFO������Ϣ
    
    int                  m_nDeciActInf;                     //  LFO���ƶ������
    LFO_CTL_ACT *        m_pDeciActInf;                     //  LFO���ƶ�����Ϣ

    int  InitAsstDeci();
    int  ReleaseAsst();
    int  AllocAsstMem();
    int  ReadAsstIni();
    int  EstimatLfoDeciRank();
    int  SechLfoDeciKeyDev();
    int  LfoDeciFromAnaRelt();
    int  StringCat(char* strSrc, const char* strAdd, int stringlen);
    void MemCpyDeciInfStru( LFO_ASST_DECI iDeciInf, LFO_ASST_DECI &oDeciInf );
    int  IsNewAsstDeciInfo( int nDeciInf, LFO_ASST_DECI *pDeciInf, LFO_ASST_DECI tempDeciInf );

    void UpdateAsstDeciTable( int DeciNum, LFO_ASST_DECI *DeciInf, int is_clear = 0 );
    void UpdateDevTable( int nDevNum, LFO_CTL_DEV * pDevId );
    int  LfoDeciTriggerSample( int DeciNum, LFO_ASST_DECI *DeciInf );
    void WriteAsstDeciToFile();
#endif
};

#endif

