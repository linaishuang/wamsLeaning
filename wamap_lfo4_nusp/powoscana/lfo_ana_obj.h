// lfo_ana_obj.h: interface for the CLfoAnaObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LFO_ANA_OBJ_H__1AB2F642_D76D_4C90_92D6_4724266E2E1C__INCLUDED_)
#define AFX_LFO_ANA_OBJ_H__1AB2F642_D76D_4C90_92D6_4724266E2E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pubwamsdata.h"
#include "pubodbapi.h"
#include "pubfun.h"
#include "lfo_struct.h"
#include "PronyAnalysis.h"
#include "DataFile.h"

#ifdef _D5000
#include "warn_service_message_m.h"
#ifndef _KEDONG
#include "HisdbAccess.h"
#include "FileOpSop.h"
#endif // !_KEDONG
#endif

#ifdef _WAMAP_
#ifndef _D5000
#include "warn_service_message.h"
#include "lfo_s_req_stru.h"
#include "tbl_rw_req_stru.h"
#include "lfo_s_req_tool.h"
#endif
#endif

#include "LFOAnaByCurve.h"

#include "db_api/odb_common.h"
#ifdef _PODB_SAMPLE_
#include "db_def/sample_def_wams.h"
#endif // _PODB_SAMPLE_

#include <string>
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////////
//  ͻ���ж�
const short BAS_FRAME_PERSEC = 100;                 //  ��Сÿ��֡��
const float FHZ_MAX_CHG = 2.0f;                //  Ƶ�����ͻ��
const float G_ANG_MAX_CHG = 3.0f;                //  0.01�빦�����仯
const float G_PWR_MAX_CHG = 0.05f;               //  0.01�빦�����ͻ�䣨�ֵ��
const float L_PWR_MAX_CHG = 35.0f;               //  0.01���й����仯

#ifdef _D5000
#ifndef WAMS_LFO_LPO_WARN_TYPE
#define WAMS_LFO_LPO_WARN_TYPE 211
#endif
#ifndef WAMS_POW_WARN_TYPE
#define WAMS_POW_WARN_TYPE 217
#endif
#ifndef WAMS_LFO_WARN_TYPE
#define WAMS_LFO_WARN_TYPE 210
#endif
#else	
// WAMS_OSC_WARN_TYPE;
#ifndef LPO_RT_CURVE_WAMS
#define LPO_RT_CURVE_WAMS LPO_RT_CURVE_WAMAP
#endif
#ifndef LFO_POW_CASE_WDS_WAMS
#define LFO_POW_CASE_WDS_WAMS LFO_POW_CASE_WDS_WAMAP
#endif
#ifndef LPO_ALL_MODE_WAMS_WAMS
#define LPO_ALL_MODE_WAMS_WAMS LPO_ALL_MODE_WAMS_WAMAP
#endif
#ifndef LPO_ALLDEV_MODE_WAMS_WAMS
#define LPO_ALLDEV_MODE_WAMS_WAMS LPO_ALLDEV_MODE_WAMS_WAMAP
#endif
#ifndef APP_TO_WARN_SERVICE_TYPE
#define APP_TO_WARN_SERVICE_TYPE        2000    //Ӧ�÷����澯����ı�������
#endif
#endif

class CDataCache : public CPubFun
{
public:
	CDataCache();
	virtual ~CDataCache();
public:
	int DataCacheMain();
	void MessShow(const char* log, const int logtype = LTINFO);
private:
	virtual BOOL OnTask(LPVOID lpv);
	virtual BOOL OnTask();
public:
	int m_DebugType;
	char m_backslash[2];
	char m_LogString[2048];
	//    char m_LogString[MAXBUF];
	string m_maindir;
};

//////////////////////////////////////////////////////////////////////////
class CLfoAnaBase : public CPubFun, public CPubWamsData, public CPronyAnalysis, public CPubOdbApi
{
public:
	CLfoAnaBase();
	virtual ~CLfoAnaBase();
public:
	int  MonitorMainProc();
	int  Exit();
	int  Error(const int nret);
	void MessShow(const char* log, const int logtype = LTINFO);
public:
	int InitProc();
	int GetMonDev();
	int GetLfoPara(LFO_PARA_STRUCT& LfoPara, const int mode = 0);
	int GetModeFreq(float& freq);//��LFO������mon_fail2����ȡ��ǰģʽ��Ƶ��
	int CheckPara(LFO_PARA_STRUCT& LfoPara, const int logtype = 0);
	int CalcPara(LFO_PARA_STRUCT& LfoPara);
	int GetDevCtlInfo(vector<LFO_DEV_INFO>& VecDev);
	int GetFacCtlInfo(vector<LFO_DEV_INFO>& VecMonDev);
	int GetGenCtlInfo(vector<LFO_DEV_INFO>& VecMonDev);
	int GetLineCtlInfo(vector<LFO_DEV_INFO>& VecMonDev);
	int GetTrCtlInfo(vector<LFO_DEV_INFO>& VecMonDev);

	int GetData();
	int ShowProMem(char* keyword);
	int ShowPMUTime(LFO_SYS_INFO& MonSys);
	int GetPMUData(vector<LFO_DEV_INFO> VecMonDev);
	int JudgeDataStat(vector<LFO_DEV_INFO> VecMonDev);
	int PMUDataHandle(vector<LFO_DEV_INFO> VecMonDev);
	int GetTimeFromFile(LFO_SYS_INFO& MonSys, CDataFile& df);
	int GetTimeFromDB(LFO_SYS_INFO& MonSys);
	int GetDataFromFile(vector<LFO_DEV_INFO>& VecMonDev, CDataFile& df);
	int GetDataFromDB(LFO_SYS_INFO& MonSys, vector<LFO_DEV_INFO>& VecMonDev);
	int FormDataFileName(string& tmpfilename);
	int GetDevDataFromDB(RTDB_TIME StartTime, RTDB_TIME EndTime, int ordernum, LFO_DEV_INFO& MonDev);

	int DataPretreat();//��������Ԥ����
	int AnaPretreat();//��������Ԥ����
	int DataInfoStatistic(vector<LFO_DEV_INFO>& VecMonDev, const int mode);
	int UnitAng(vector<float>& vdata);
	float UnitAng2(float fdata);
	int SelectRefAngle2(vector<LFO_DEV_INFO>& VecMonDev);
	int GetCurveInfo(vector<float>& VecData, LFO_CURVE_INFO& CurveInfo);
	int DataFilter(vector<LFO_DEV_INFO>& VecMonDev, const int mode);
	int Euler_ValFilter(int nn, double x[], double t[], double y[]);
	void DP_MidAveFilter(const double* iSrcData, double* oFltData, int nDataLen, int nFiltLen);  // ��ֵƽ���˲�
	int DataSample(vector<LFO_DEV_INFO>& VecMonDev, const int mode);
	int LfoPreEstimate(vector<LFO_DEV_INFO>& VecMonDev);
	int IsDataValid(vector<LFO_DEV_INFO>& VecMonDev);
	int IntfaceAmplLimit(vector<LFO_DEV_INFO>& VecMonDev);
	//  �ж��������ݵ��Ƿ���ͻ�䡣��ͻ�䷵��-1, ������1)����������Բ� > (ǰʮ����Բ�֮��)��2) ����������Բ� > ����������ֵ
	float  JudgeTwoValDelta(vector<float>& VecData, int num, float max_change_lim, float delta_lim_val, int algorithm);

	int PronyAnaMain();
	int FormPronyData(vector<LFO_DEV_INFO> VecMonDev, vector<LFO_PRONY_INFO>& VecCase, const int mode);
	int PronyProcess(vector<LFO_PRONY_INFO>& VecCase);
	int GetDevMode(vector<LFO_PRONY_INFO> VecCase, vector<LFO_DEV_INFO>& VecMonDev);
	int GetAnaMode(vector<LFO_PRONY_INFO> VecCase, vector<LFO_DEV_INFO>& VecMonDev);
	int GetIntMode(vector<LFO_DEV_INFO>& VecMonDev);
	int FormMonResult();
	int SysModeSum(vector<LFO_DEV_INFO> VecMonDev, vector<LFO_MODE_SHAPE>& VecModeShape);
	int CalcPartFact(vector<LFO_MODE_SHAPE>& VecModeShape);
	int GetDevCluster(vector<LFO_MODE_SHAPE>& VecModeShape);
	int JudgeLfoCenter(vector<LFO_MODE_SHAPE>& VecModeShape);
	int GetDevModeShape(vector<LFO_MODE_SHAPE> VecModeShape, vector<LFO_DEV_INFO>& VecMonDev);
	int JudgeDevLfoStat(vector<LFO_DEV_INFO>& VecMonDev);
	int JudgeDevOscStat(vector<LFO_DEV_INFO>& VecMonDev);
	int PronyAna();

	int IsWarn(LFO_MODE_INFO& Mode, LFO_PARA_STRUCT& Para, const int devtype);
	int IsSafe(LFO_MODE_INFO& Mode, LFO_PARA_STRUCT& Para, const int devtype);
	int GetLfoDevStat(LFO_DEV_STAT& tmpDevStat);
	int SearchAsstDeci();
	int JudgeSysLfoStat(vector<LFO_DEV_INFO> VecMonDev, vector<LFO_MODE_SHAPE> VecModeShape, LFO_SYS_INFO& MonSys, LFO_SYS_INFO& oscMonSys);
	//yjj ����
	int LocateLfoSource(vector<LFO_DEV_INFO>& VecMonDev, LFO_SYS_INFO& MonSys);
	void    LfoAnaSetLnClustbyFc(int nPartLn, MODE_PART_OBJ* pPartLn, int nPartFc, MODE_PART_OBJ* pPartFc, vector<LFO_DEV_INFO>& VecMonDev);
	void    LfoAnaSetTrClustbyFc(int nPartTr, MODE_PART_OBJ* pPartTr, int nPartFc, MODE_PART_OBJ* pPartFc, vector<LFO_DEV_INFO>& VecMonDev);
	void	EstimateSingleObjState(OSCILLAT_FLAG* osciFlag, LFO_LEAD_MODE* leadMode, int times, float SameFreqDifVal);

	//��������
	void    AnaCalcProcess();                               //  LFO��������
	void    LfoClctMainMode(); // case result   -> m_pAllObjMode	
	void    LfoAnaPartGenPreHandle(int nGnNum, LFO_GN_INF* pGnInf, int nPartGn, MODE_PART_OBJ* pPartGn);
	void    LfoAnaSetLnClustbyFc(int nPartLn, MODE_PART_OBJ* pPartLn, int nPartFc, MODE_PART_OBJ* pPartFc);
	void    LfoAnaSetTrClustbyFc(int nPartTr, MODE_PART_OBJ* pPartTr, int nPartFc, MODE_PART_OBJ* pPartFc);
	void    LfoAnaSetGnAmpl(int nPartGn, MODE_PART_OBJ* pPartGn);
	void    LfoAnaResetFact(MODE_CHRCT* pSltMode);
	int UpdateStatOld(vector<LFO_DEV_INFO>& VecMonDev, LFO_SYS_INFO& MonSys, LFO_SYS_INFO& oscMonSys);
	int WritePmuData(const char* dir_fln, tab_lfo_pmu_data& mm_PMUData);
	int WritePmuSqlData(LPO_DEV_INFO  VecLPODEVINFO);
	//	int ExcuteFileUpnLoad(const char* locdir, vector<string> vec_files, const char * srvdir);
	int ExcuteFileUpnLoad(const char* locdir, string vec_files, const char* srvdir);
public:
	int ReadLfoPara(vector<LFO_PARA_TABLE_READ>& VecLfoPara);
	int GetLfoFac(vector<LFO_FC_TAB_CRE>& VecLfoFacInfo);
	int GetLfoGen(vector<LFO_GN_TAB_CRE>& VecLfoGenInfo);
	int GetLfoLine(vector<LFO_LN_TAB_CRE>& VecLfoLineInfo);
	int GetLfoTr(vector<LFO_TR_TAB_CRE>& VecLfoTrInfo);
	int GetLfoIntface(vector<LFO_INT_TAB_CRE>& VecLfoIntInfo);
	int GetLfoIntline(vector<LFO_INTLINE_TAB_CRE>& VecLfoIntlineInfo);
public:
	int ClearLfoDev(vector<LFO_DEV_INFO>& tmpVecDev);
	int ClearLfoDev(LFO_DEV_INFO& tmpLfoDev);
	int ClearLfoIntline(vector<LFO_INTLINE_INFO>& tmpVecIntline);
	int ClearLfoIntline(LFO_INTLINE_INFO& tmpLfoIntline);
	int ClearLfoModeShape(vector<LFO_MODE_SHAPE>& tmpVecMode);
	int ClearLfoModeShape(LFO_MODE_SHAPE& tmpModeShape);
	int ClearLfoSysInfo(vector<LFO_SYS_INFO>& tmpVecSys);
	int ClearLfoSysInfo(LFO_SYS_INFO& tmpLfoSys);
	int ClearLfoModeInfo(LFO_MODE_INFO& tmpLfoMode);
	int ClearLfoModeInfo(vector<LFO_MODE_INFO>& tmpVecMode);
	int ClearLfoPronyInfo(vector<LFO_PRONY_INFO>& tmpVecProny);
	int ClearLfoPronyInfo(LFO_PRONY_INFO& tmpLfoProny);
	int ClearLfoDevMode(LFO_DEV_MODE& tmpDevMode);
	int ClearLfoDevMode(vector<LFO_DEV_MODE>& tmpVecDevMode);
	int ClearLfoPronyInfo1(vector<LFO_PRONY_INFO>& tmpVecProny);
	int CopyLfoDev(vector<LFO_DEV_INFO>& ObjVecDev, vector<LFO_DEV_INFO>& SrcVecDev);//�����豸��Ϣvector
	int CopyLfoDev(LFO_DEV_INFO& ObjLfoDev, LFO_DEV_INFO& SrcLfoDev);//�����豸��Ϣ
	int CopyLfoSysInfo(vector<LFO_SYS_INFO>& ObjVecSys, vector<LFO_SYS_INFO>& SrcVecSys);//����ϵͳ��Ϣvector
	int CopyLfoSysInfo(LFO_SYS_INFO& ObjLfoSys, LFO_SYS_INFO& SrcLfoSys);//����ϵͳ��Ϣ
	int CopyLfoModeShape(vector<LFO_MODE_SHAPE>& ObjVecMode, vector<LFO_MODE_SHAPE>& SrcVecMode);//����ģʽ��Ϣvector
	int CopyLfoModeShape(LFO_MODE_SHAPE& ObjModeShape, LFO_MODE_SHAPE& SrcModeShape);//����ģʽ��Ϣ
	int CopyLfoIntline(vector<LFO_INTLINE_INFO>& ObjVecIntline, vector<LFO_INTLINE_INFO>& SrcVecIntline);//����֧·��Ϣvector
	int CopyLfoIntline(LFO_INTLINE_INFO& ObjLfoIntline, LFO_INTLINE_INFO& SrcLfoIntline);//����֧·��Ϣ
	int CopyLfoModeInfo(LFO_MODE_INFO& ObjLfoMode, LFO_MODE_INFO& SrcLfoMode);
	int CopyLfoModeInfo(vector<LFO_MODE_INFO>& ObjVecMode, vector<LFO_MODE_INFO>& SrcVecMode);
	int CopyLfoDevBase(LFO_DEV_BASE& ObjDevBase, LFO_DEV_BASE& SrcDevBase);
	int CopyLfoDotInfo(WT_DOT_INFO& ObjDotInfo, WT_DOT_INFO& SrcDotInfo);
	int CopyLfoWtInfo(WT_DEV_INFO& ObjWtInfo, WT_DEV_INFO& SrcWtInfo);
private:
	virtual BOOL OnTask(LPVOID lpv);
	virtual BOOL OnTask();
public:
	int m_DebugType;
	char m_backslash[2];
	char m_LogString[2048];
	//    char m_LogString[MAXBUF];
	string m_maindir;
	char   m_CasefDir[MAXBUF];                         //  �����ļ�Ŀ¼
public:
	int m_ThreadIdx;  // �̱߳��
	LFO_PARA_STRUCT m_LfoPara;
	//     1)	�豸��̬��Ϣ
	//    vector<WT_DEV_INFO> m_VecDev;  // ���г�վ�����������·�ͱ�ѹ������̬��Ϣ��Ϊ��ʱ����)
	//     2)	�豸������Ϣ
	vector<LFO_DEV_INFO> m_VecDev;  // ���ӳ�վ�����������·�ͱ�ѹ����������
//     3)	ģ̬�������
	vector<LFO_MODE_SHAPE> m_VecModeShape;
	//     4)	ϵͳ״̬
	LFO_SYS_INFO m_MonSys;
	//     5)	ϵͳС�Ŷ�״̬
	LFO_SYS_INFO m_oscMonSys;
	//     6)	���������Ϣ
	vector<LFO_INTLINE_INFO> m_VecIntline;  // �������
public:
	int                  para_nFcMaxMem;                    //  ���ڴ泧վ�����
	int                  para_nGnMaxMem;                    //  ���ڴ���������
	int                  para_nLnMaxMem;                    //  ���ڴ���·�����
	int                  para_nTrMaxMem;                    //  ���ڴ��ѹ�������
	int					 para_nObjMaxMode;                  //  ����objmode��(3*para_nCaseMaxMem)
	int					 para_nCaseMaxMem;					//  �������� (para_nAnaMaxFcNum+para_nGnMaxMem+para_nLnMaxMem+para_nTrMaxMem)
	int					 para_nAnaMaxFcNum;                 //  ������վ�����
	int					 para_nSysMaxMode;			        //  ����sysmode��

public:
	// 	MODE_CHRCT           m_DispMode;


public:
	int                  m_nGnNum;                          //  LFO����̨��
	LFO_GN_INF* m_pGnInf;                          //  LFO���龲̬��Ϣ
//     LFO_GN_CTL *         m_pGnCtl;                          //  LFO���������Ϣ
//     LFO_GN_PMU *         m_pGnPmu;                          //  LFO����PMU��Ϣ
//     LFO_GN_DAT *         m_pGnDat;                          //  LFO���鶯̬����
//     LFO_GN_MON *         m_pGnMon;                          //  LFO���������Ϣ
// 	OSCILLAT_FLAG *      m_pGnOsc;                          //  LFO�����񵴱�־
//     OBJ_ACT_FACT *       m_pGnFact;                        //  LFO�����������  2008-04-04 added by zhw at yn
// 	int                  m_nGnProDat;                       //  �账��Ļ���
// 	LFO_GN_DAT *         m_pGnProDat;                       //  �账��Ļ�������(Ԥ�����)
// 	int                  m_nGnWarn;
// 	int					 sum_WarnGen;
// 	LFO_GN_MON *         m_pGnWarn;
// 	LFO_OBJ_RES_SAVE *   m_pGnResSave;                      //  �����豸���
// 	LFO_GN_MON *         m_DevGnWarn;							//  �澯�豸���
// 	LFO_GN_MON *         sum_DevGnWarn;
//     LFO_GN_DAT *         sum_pGnDat;                          //  LFO���鶯̬����
	LFO_GN_INF* sum_pGnInf;
	// 	TYPE_GN_WARN_TIME *  gn_warn_time;

	int                  m_nCases;                          //  ��������
	CASE_TYPE* m_pCases;                          //  ������Ϣ
// 	int                  m_nMonMode;	                    //  mon ģʽ��
// 	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon ģʽ
	int                  para_IsFactByClust;                  //  �ڷ�Ⱥ�����ѡ���ֵ�����豸,��Ϊ�������ӵĻ�׼



	CDataFile			 m_df;								//��ȡ�����ļ�

};

class CLfoAnaObj : public CLfoAnaBase
{
public:
	CLfoAnaObj();
	virtual ~CLfoAnaObj();
public:
	int MainProc();
public:
	int InitPara();
	int ReadIniFile(const string filename, LFO_PARA_STRUCT& LfoPara);
	int ReadFreqRange(LFO_PARA_STRUCT& LfoPara);
	int InitDevInfo(vector<LFO_DEV_INFO>& VecDev);
	int GetDevFromDB(vector<LFO_DEV_INFO>& VecDev);
	int GetDevFromFile(vector<LFO_DEV_INFO>& VecDev);
	int GetDevice(const string filename, vector<long>& VecDevID, vector<LFO_INTLINE_INFO>& VecINTLINE);//���߹��ܻ�ȡ�����豸��Ϣ
	int SaveDevInfo(vector<LFO_DEV_INFO>& VecDev);
	int ThreadInit();
	int StartThread();
	int StartDataCacheTh();
	int StartMonitorTh();
	int ThreadCallBack(vector<LFO_DEV_INFO>& VecDev, vector<LFO_MODE_SHAPE>& VecModeShape, LFO_SYS_INFO& MonSys, LFO_PARA_STRUCT& LfoPara, const int threadidx, LFO_SYS_INFO& oscMonSys);
	int ThreadCallBack_ERR(LFO_SYS_INFO& MonSys, LFO_PARA_STRUCT& LfoPara, const int threadidx, const int is_warn);
	//int UpdateThreadInfo(const int threadidx);
	int UpdateMonDev(vector<LFO_DEV_INFO>& VecDev, LFO_SYS_INFO& MonSys, const int threadidx);
	int UpdateOscMonDev(vector<LFO_DEV_INFO>& VecDev, LFO_SYS_INFO& MonSys, const int threadidx);

	int UpdateMonSys(LFO_SYS_INFO MonSysThread, LFO_SYS_INFO MonSys);
	int CacheWindowsData(LFO_SYS_INFO MonSys, vector<LFO_SYS_INFO>& VecLfoWindow);
	int CacheOscWindowsData(LFO_SYS_INFO MonSys, vector<LFO_SYS_INFO>& VecLfoWindow);
	int FormLfoCaseResult(vector<LFO_SYS_INFO>& VecLfoWindow, LFO_CASE_INFO& LfoCase, const int save_flag);
	int FormOscCaseResult(vector<LFO_SYS_INFO>& VecLfoWindow, LFO_CASE_INFO& LfoCase, const int save_flag);
	//�������
	int SampleLfoPara(LFO_CASE_INFO& LfoCase);//burst_sample_data_0451����
	int SampleHisEve(LFO_CASE_INFO& LfoCase);//burst_sample_data_0448����
	int SampleHisCase(LFO_CASE_INFO& LfoCase);//burst_sample_data_0449����
	int SampleHisShape(LFO_CASE_INFO& LfoCase);//burst_sample_data_0450����
	int SampleHisMode(LFO_CASE_INFO& LfoCase);//burst_sample_data_0500 burst_sample_data_0501����



	int SaveMonResult(vector<LFO_DEV_INFO>& VecMonDev, LFO_SYS_INFO& MonSys);
	int SaveLfoResult(LFO_CASE_INFO& LfoCase);
	int SendWarnMessage(LFO_SYS_INFO& MonSys, int type, int end_time, int hold_time, int if_lfo);
	int SendWarnToIA(LFO_SYS_INFO& MonSys, const int mode);  // ���ۺ����ܸ澯(�ƶ�)������Ϣ
	int ShowWamsCurve(LFO_SYS_INFO& MonSys);
	int JurgeLFO(LFO_SYS_INFO& MonSys, int stat); //�ж��Ƿ������Ƶ�������ֵ
	int SetEmsStat(LFO_SYS_INFO& MonSys);

	int UpdateLfoMonPara(LFO_SYS_INFO& MonSys);
	int UpdateMonGnTable(vector<LFO_DEV_INFO>& VecMonDev);
	int UpdateMonLnTable(vector<LFO_DEV_INFO>& VecMonDev);
	int UpdateMonTrTable(vector<LFO_DEV_INFO>& VecMonDev);
	int UpdateMonIntTable(vector<LFO_DEV_INFO>& VecMonDev);
	int UpdateWDSTable(LFO_CASE_INFO& MonSys);
	int UpdateAnaGnTable(vector<LFO_DEV_INFO>& VecMonDev);

	int UpdateMonGnDisp(vector<LFO_DEV_INFO>& VecMonDev);
	int UpdateMonLnDisp(vector<LFO_DEV_INFO>& VecMonDev);
	int UpdateMonTrDisp(vector<LFO_DEV_INFO>& VecMonDev);
	int UpdateMonIntDisp(vector<LFO_DEV_INFO>& VecMonDev);

	int SaveLfoCurve(vector<LFO_DEV_INFO>& VecMonDev, LFO_SYS_INFO& MonSys);
	int UpdateLfoCurve(LFO_DEV_INFO& tmpLfoDev);
	int SavePowCurve(LFO_CASE_INFO& LfoCase);//���º�ɢ��������
	int UpdatePowCurve(LFO_DEV_INFO& tmpLfoDev);

	int UpdateLfoGnTable(LFO_CASE_INFO& LfoCase, int save_flag);
	int UpdateLfoFcTable(LFO_CASE_INFO& LfoCase, int save_flag);
	int UpdateLfoLnTable(LFO_CASE_INFO& LfoCase, int save_flag);
	int UpdateLfoTrTable(LFO_CASE_INFO& LfoCase, int save_flag);
	int UpdateLfoIntTable(LFO_CASE_INFO& LfoCase, int save_flag);
	// 	int UpdateCaseWdsTable(LFO_CASE_INFO &LfoCase);//����LPO���ʲ������ڱ�

	int UpdateLfoParaTable(LFO_CASE_INFO& LfoCase);
	int ClearLfoParaTable(LFO_CASE_INFO& LfoCase);
	int UpdateLfoCaseTable(LFO_CASE_INFO& LfoCase);
	int UpdateLfoWindowsTable(LFO_CASE_INFO& LfoCase);
	int UpdateLfoModeShape(LFO_CASE_INFO& LfoCase);

	int SaveLfoCaseFile(LFO_CASE_INFO& LfoCase);
	int SaveCaseCurve(LFO_CASE_INFO& LfoCase);
	int SaveOscReult(LFO_CASE_INFO& LfoCase);
	int SaveCaseTmd(LFO_CASE_INFO& LfoCase);
	int TmdAddDevInfo(vector<LFO_DEV_INFO>& VecDev, vector<string>& VecString, int devtype);
	int TmdAddModeInfo(vector<LFO_DEV_INFO>& VecDev, vector<string>& VecString, int devtype);
	int TmdAddKeyId(vector<LFO_DEV_INFO>& VecDev, vector<KEY_ID_STRU>& VecKeyID, int devtype);

	void    FormMonModeCurce();                             //  �γ�ģʽ����
	void    UpdateMonModeCurce(PERIOD_LEAD_MODE* pLfoModeCurve);                //  LFOʵʱ���߱�ģʽ���ߣ�
	void    UpdateLfoCurve(TAB_LFO_RT_CURVE RtCurve);                            //  LFOʵʱ���߱���¼1��2��3��

	int main_RunStatus;     //  ����״̬ 0�ȴ�  1���� 
	int main_RunCtrCom;         //  ���߷������з�ʽ����
private:
	int AddLfoFc(vector<LFO_DEV_INFO>& VecMonDev);
	int AddLfoGen(vector<LFO_DEV_INFO>& VecMonDev);
	int AddLfoLine(vector<LFO_DEV_INFO>& VecMonDev);
	int AddLfoTr(vector<LFO_DEV_INFO>& VecMonDev);
	int AddLfoIntface(vector<LFO_DEV_INFO>& VecMonDev);
	int AddLfoIntLine(vector<LFO_INTLINE_INFO>& VecINTLINE);

	int SetFacInfo(vector<LFO_DEV_INFO>& VecMonDev);
	int SetAreaInfo(vector<LFO_DEV_INFO>& VecMonDev);
	int SetVlTypeInfo(vector<LFO_DEV_INFO>& VecMonDev);
	int SetFesYcInfo(vector<LFO_DEV_INFO>& VecMonDev);
	int SetDevInfo(vector<LFO_DEV_INFO>& VecMonDev);
private:
	int ClearLfoCase(LFO_CASE_INFO& tmpLfoCase);
	int ClearCaseFile(LFO_CASE_FILE_STRUCT& tmpCaseFile);
private:
	virtual BOOL OnTask(LPVOID lpv);
	virtual BOOL OnTask();
private:
	CDataCache* m_pDataThread;
	CLfoAnaBase* m_pLfoThread;

	LFO_THREAD_INFO* m_pLfoThreadInfo;  // �߳���Ϣ���棬���̼߳�����ɺ���¶�Ӧ�ṹ

//     2.9	ÿ���������ڽṹ(LFO_SYS_INFO)
	vector<LFO_SYS_INFO> m_VecLfoWindow;
	//     2.10	���¼��ṹ(LFO_CASE_INFO)
	LFO_CASE_INFO m_LfoCase;

	//     2.11	С�Ŷ�ͳ��ÿ���������ڽṹ(LFO_SYS_INFO)
	vector<LFO_SYS_INFO> m_VecOscWindow;
	//     2.12	С�Ŷ�ͳ���¼��ṹ(LFO_CASE_INFO)
	LFO_CASE_INFO m_oscLfoCase;

	PERIOD_LEAD_MODE     m_pLfoModeCurve[LFO_CURVE_MAX_PNT];//  ����ģʽ����
	LPO_RT_CURVE_WAMS	 m_pLfoCurve[LPO_CURVE_MAX_PNT];

};

#endif // !defined(AFX_LFO_ANA_OBJ_H__1AB2F642_D76D_4C90_92D6_4724266E2E1C__INCLUDED_)
