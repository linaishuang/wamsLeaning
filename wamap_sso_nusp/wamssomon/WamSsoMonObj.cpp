/******************************************************************************
  ModuleName :   WAMS LFO Analysis frame
  FileName   :   WamLfoAnaObj.cpp
  Description:   WAMS LFO Analysis frame Implemention
  HISTORY :
	  Date        Version       Author                Activities
   ----------------------------------------------------------------------
   09/11/2007       1.0         lsf                    created
   04/29/2008       2.0         zhw              �޸�����ȱ�����ʼʱ��
												 FormLfoMonSaveResult( )
******************************************************************************/
// 
// ����Ԥ������
// ������֯
// ������ҵ���߳̿���
// �����
// ���泡�������ݼ������
// ( ��ȷ��ĸ��ѡȡ����, ��·���˾��в�������ѡȡ���� ) ??????? 
// Ŀǰ��ĸ��ѡȡ��վ�е�ѹ�ȼ���ߵ���ĸ/��ĸ����ָ��ĸ��
//       ��·ѡȡ����һ�ˣ���ָ���߶�
#include    "WamSsoMonObj.h"
#ifdef _WINDOWS32
#include <io.h>                      // _findfirst
#include <process.h>                 // _beginthreadex
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#endif

#include <iostream>
//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֱ���
extern CLfoAnaObj* g_pAnaObj;                         //  ������
CPronyAnalysis* g_pProny;
CLFOAnaByCurve                g_theLfoAna;
extern void g_PubLogMessShow(const char* log, const int logtype = LTINFO);
//////////////////////////////////////////////////////////////////////////
// ����
CLfoAnaObj::CLfoAnaObj()
{
	memset(m_RunInfDir, 0, MAXBUF);
	memset(m_LogString, 0, MAXBUF);
	memset(m_SaveFileDir, 0, MAXBUF);
	memset(m_DataDir, 0, MAXBUF);
	memset(m_DataFileDir, 0, MAXBUF);
	memset(m_DataFile, 0, MAXBUF);
	memset(m_DataFile1, 0, MAXBUF);
	memset(m_DataFile2, 0, MAXBUF);
	memset(m_DataFile3, 0, MAXBUF);
	memset(m_DataFile4, 0, MAXBUF);
#ifdef _WINDOWS32
	sprintf(m_RunInfDir, "..");
	sprintf(m_CasefDir, "..");
	sprintf(m_backslash, "\\");
#else
	sprintf(m_RunInfDir, "../data/wam_sso/mon");
	sprintf(m_CasefDir, "../data/wam/ssocasefile");
	sprintf(m_backslash, "/");
#endif
	sprintf(m_SaveFileDir, "%s%s%s", m_RunInfDir, m_backslash, "save");
	sprintf(m_DataDir, "%s%s%s", m_RunInfDir, m_backslash, "data");
	m_LnPowFileName = "ln_power";
	m_GnPowFileName = "gn_power";
	m_GnAngFileName = "gn_angle";
	m_FacVoltFileName = "fac_volt";
	m_FacAngFileName = "fac_angle";
	m_TrPowFileName = "tr_power";
	m_LfoResFileName = "ModeShape";

	int   ii;
	//  ��־���ʻ������ʼ��
	THRD_InitThreadMutex(m_logmutex, "LogFileMutex");
	//  ʵʱ���߻�����
	THRD_InitThreadMutex(m_RtCurvemutex, "RtCurveMutex");
	//  ���滥����
	THRD_InitThreadMutex(m_Itfcmutex, "ItfcMutex");
	THRD_InitThreadMutex(m_DynFilemutex, "DynFileMutex");

	////  ���ò���(ͨ�������ļ����ģ�������Ӧ���ݿ�)
	para_LfoAnaByPwr = 0;                         //  ����ѡ��    1 ���ʷ���      0 �Ƕȷ���
	para_LfoAnaByEEAC = 0;                         //  ��ʽѡ��    1 EEAC��Ⱥ      0 ��λ�Ƿ�Ⱥ
	para_CalcPQVByUI = 0;                         //  �Ƿ����PQV 1 ͨ��ui����PQV 0 ֱ�ӻ�ȡPQV
	// 2008-04-06 added by zhw at yn
	para_SaveAllFac = 1;                         //  tmd�������г�վ����    1��������    0����澯�豸
	para_CallAllFacDyn = 0;
	//2008-04-14 added by zhw at nj
	//para_TestLnID             = 240000000;                  //  �źŷ������⽻���߶�ID 240000000
	para_nGpsTime = 0;                          //  ���ݰ�������ʱGpsʱ��

	para_nFcMaxMem = 100;                       //  ���ڴ泧վ�����
	para_nGnMaxMem = 200;                       //  ���ڴ���������
	para_nLnMaxMem = 300;                       //  ���ڴ���·�����
	para_nTrMaxMem = 200;                       //  ���ڴ��ѹ�������
	para_nItfcMaxMem = 5;                          //  ���ڴ���������
	para_nItfcMaxDev = 5 * para_nItfcMaxMem;         //  ���ڴ���������·��

	para_nAnaMaxFcNum = 120;//20 ;                        //  ������վ�����
	para_nShwMaxFcNum = 3;                         //  ÿȺ��ʾ��վ�����
	if (para_nAnaMaxFcNum > para_nFcMaxMem) para_nAnaMaxFcNum = para_nFcMaxMem;
	if (para_nShwMaxFcNum > para_nFcMaxMem) para_nShwMaxFcNum = para_nFcMaxMem;

	para_nSysMaxMode = SYS_MODE_MAX; //5 ;         //  ����sysmode��
	para_nCaseMaxMem = para_nAnaMaxFcNum + para_nGnMaxMem + para_nLnMaxMem + para_nTrMaxMem;
	para_nObjMaxMode = (3 * para_nCaseMaxMem);      //  ����objmode��(3*para_nCaseMaxMem)
	para_SaveDataToFile = 0;                          //  ���������ļ���־
	para_DataSaveNum = 15;                         //  �����ļ��������

	para_nAsstDeciMax = 100;                        //  ������������� 2008-09-09 by zhw

	para_SaveXmlFile = 0;                        //  �Ƿ�����XML�ļ�
	if (!m_XmlFileName.empty())
	{
		m_XmlFileName.clear();
	}
	memset(para_XmlFileDir, 0, MAXBUF);               //  ɽ��Ӧ��ϵͳ�ӿ��ļ�Ŀ¼  //2008-06-24 by zhw

	// ����E���Խӿ��ļ�
	para_SaveResFile = 0;
	memset(para_FtpServer, 0, MAXBUF);
	memset(para_FtpUser, 0, MAXBUF);
	memset(para_FtpPsw, 0, MAXBUF);
	memset(para_ResFileName, 0, MAXBUF);
	memset(para_FtpDir, 0, MAXBUF);
	memset(para_FtpCmd, 0, MAXBUF);

	para_DebugInfo = 0;                        //  ������Ϣ  // 2008-07-16 by zhw
	para_Ampl_aver = 0.6;                      //  ����ģʽ��ֵռ���ξ�ֵ��  // 2008-08-12
	para_FcVoltPer = 0.01;                     //  ��վ��ѹԤ�аٷֱ�  // 2008-08-13
	para_VoltAmplPer = 0.6;                      //  2008-08-14
	para_MaxAnglDiff = 300.0;                    //  ����ͻ��ǶȲ�
	para_IsCallDynFile = 0;                        //  �Ƿ��ٻ�100֡�ļ�
	para_IsCallComtrFile = 0;                        //  �Ƿ��ٻ�¼���ļ�
	para_CallDynDelaySec = 60;                      //  �ٻ�100֡�ļ��ӳ�ʱ��
	para_IsShowAnaRes = 0;                        //  TMD�Ƿ���ʾ�������,��������Ϣ
	para_IsShowAsstDeci = 0;                        //  TMD�Ƿ���ʾ��������
	para_IsShowAnaData = 0;
	para_IsFactByClust = 0;
	para_FilePmuInfo = 1;
	para_FileModeTxt = 2;
	para_DebugProny = 1;
	para_UpAppVariable = 0;
	para_FormAsstDeci = 0;
	para_FileType = 1;
	para_UpDevLfoStat = 0;
	para_IsDataCycle = 0;
	para_IsFltSim = 0;
	para_IsWarnIA = 0;
	para_ShowFacName = 1;
	para_WarnAppNo = AP_SCADA;
	para_IsDataValid = 1;

	////  �����ļ�����
	m_DataFileAskPnt = 250;                        //  ���������ļ�Ҫ���ȡ������

	m_FcDataFilePos = 0;                          //  �����ļ���ȡλ��
	m_FcDataFileOffset = 0;                          //  ��������ļ���ȡλ�õ�ƫ����

	m_GnDataFilePos = 0;                          //  �����ļ���ȡλ��
	m_GnDataFileOffset = 0;                          //  ��������ļ���ȡλ�õ�ƫ����

	m_LnDataFilePos = 0;                          //  �����ļ���ȡλ��
	m_LnDataFileOffset = 0;                          //  ��������ļ���ȡλ�õ�ƫ����
	m_TrDataFilePos = 0;
	m_TrDataFileOffset = 0;

	// 2008-06-23 by zhw
	m_FcVolFilePos = 0;                          //  ��վ��ѹ��ȡλ��
	m_FcVolFileOffset = 0;                          //  ��վ��ѹ��ȡƫ����
	m_FcAngFilePos = 0;                          //  ��վ��ѹ��ȡλ��
	m_FcAngFileOffset = 0;                          //  ��վ��ѹ��ȡƫ����

	m_GnPowFilePos = 0;                          //  ������й���ȡλ��
	m_GnPowFileOffset = 0;                          //  ������й���ȡƫ����
	m_GnAngFilePos = 0;                          //  �������Ƕ�ȡλ��
	m_GnAngFileOffset = 0;                          //  �������Ƕ�ȡƫ����

	// 2008-05-11 by zhw at nj �źŷ���������
	m_acdot_id = 0;
	m_isForceSet = 0;
	m_filNo = -1;
	m_colNo = -1;
	////  
	m_reAllocmem = NOALLOCMEM;                 //  �����ڴ�
	m_LfoAnaByPwr = 0;                          //  �������أ�1 �й� 0 �����ǣ�
	m_FormResSaveStat = L_NOT_FORM_RESULTE;         //  ���ϱ���״̬(0 ������ 1�������� 2���� 3ͳ�������)
	m_WarnSendFlag = 0;
	m_CallDynFile = LFO_TMD_DYN_GET_NO;

	////  calc para
	m_LfoPara.mem_max_sec = 20;                        //  �����������
	m_LfoPara.smp_freq = LFO_SMP_FRQ;                //  ����Ƶ��
	m_LfoPara.flt_len = 5;	                        //  �˲�����
	m_LfoPara.flt_coe = 0.020f;	                //  �˲�ϵ��
	m_LfoPara.time_leng = 10;	                    //  �۲�ʱ�䴰��
	m_LfoPara.int_time = 5;	                        //  ��������

	m_LfoPara.freq_min = 10;	                //  Ƶ������ 2008-03-11
	m_LfoPara.freq_max = 90;	                //  Ƶ������
	para_AnaWnapPer = 30.000f;                   //  ������������ٷֱ���ֵ
	m_LfoPara.volt_thre1 = 220;						//��ѹ�ȼ�1	
	m_LfoPara.volt_thre2 = 500;						//��ѹ�ȼ�2

	m_LfoPara.sso_jud_num = 5;	                        //  �жϴ���


	////  disp para
	m_DispCtl.dsource = 0;	                    //  ����Դ

	ZERO_INT_REC(m_DispCtl.sso_fac_id);	                    //  ��Ӧ���⳧վID

	m_DispCtl.is_warn = 1;	                    //  �Ƿ��͸澯
	m_DispCtl.is_samp = 1;	                    //  �Ƿ񴥷�����

	m_DispCtl.show_max = 3;	                    //  ���������ʾ

	////  process para
	memset((void*)&m_ProcCtl, 0, sizeof(PARA_PROC_CTL));
	//m_ProcCtl.proc_ctl   =  0 ;                           //  ���з�ʽ����
	//m_ProcCtl.proc_tim   =  0 ;                           //  ���к�ʱ
	//m_ProcCtl.proc_sts   =  0 ;	                        //  ����״̬

	////  data
	m_CalcStartTime = 0;                         //  ��ǰ�۲촰����ʼGPSʱ��
	m_iCalcOffPos = 0;                         //  ����ƫ��λ��
	m_CurGpsTime = 0;                         //  ��ǰGPSʱ��(��ֹGPSʱ��)
	m_OldGpsTime = 0;                         //  �ϴμ���ʱ��(��ֹGPSʱ��)

	//// data process
	m_nSmpFre = LFO_SMP_FRQ;               //  ����Ƶ��
	m_nSmpStp = (int)(MAX_FRA_PER_SEC / m_nSmpFre); //  �������0.1
	m_nOrigDlt = 0;					        //  ԭʼ��������
	m_nOrigSam = 0;					        //  ԭʼ���������
	m_nSamples = 0;					        //  �����ò��������
	memset((void*)m_Matchtime, 0, sizeof(float) * DATA_NUM_MAX);
	memset((void*)m_ProMatchtime, 0, sizeof(float) * DATA_NUM_MAX);

	m_tNewSec = 0;                         //  ȡ��GPSʱ��(m_CurGpsTime-m_nMovSec) 
	m_nMovSec = 0;                         //  ȡ���ƶ�����
	m_iOffPos = 0;                         //  ȡ��ƫ��λ��

	m_CallHDFile = 0;
	m_HDFileOk = 0;
	////  device para
	m_nFcNum = 0;                             //  LFO��վ����
	m_pFcInf = NULL;                          //  LFO��վ��̬��Ϣ
	m_pFcCtl = NULL;                          //  LFO��վ������Ϣ
	m_pFcPmu = NULL;                          //  LFO��վPMU��Ϣ 
	m_pFcDat = NULL;                          //  LFO��վ��̬����
	m_pFcMon = NULL;                          //  LFO��վ������Ϣ
	m_pFcLfo = NULL;                          //  LFO��վ����Ϣ
	m_nFcProDat = 0;                          //  �账��ĳ�վ
	m_pFcProDat = NULL;                       //  �账��ĳ�վ����
	m_pFcAna = NULL;                       //  LFO��վ������Ϣ

	m_nGnNum = 0;                             //  LFO����̨��    
	m_pGnInf = NULL;                          //  LFO���龲̬��Ϣ
	m_pGnCtl = NULL;                          //  LFO���������Ϣ
	m_pGnPmu = NULL;                          //  LFO����PMU��Ϣ 
	m_pGnDat = NULL;                          //  LFO���鶯̬����
	m_pGnMon = NULL;                          //  LFO���������Ϣ
	m_pGnOsc = NULL;                          //  LFO�����񵴱�־
	m_pGnFact = NULL;                        //  LFO������������� 2008-04-04 added by zhw at yn
	m_pLnFact = NULL;                         //  LFO��·��������  2008-05-21 added by zhw at nj
	m_nGnProDat = 0;                           //  �账��Ļ���
	m_pGnProDat = NULL;                        //  �账��Ļ�������(Ԥ�к�)
	m_nGnWarn = 0;
	sum_WarnGen = 0;
	m_pGnWarn = NULL;
	m_pGnResSave = NULL;
	m_pDevWarn = NULL;

	m_nLnNum = 0;                             //  LFO��·��      
	m_pLnInf = NULL;                          //  LFO��·��̬��Ϣ
	m_pLnCtl = NULL;                          //  LFO��·������Ϣ
	m_pLnPmu = NULL;                          //  LFO��·PMU��Ϣ 
	m_pLnDat = NULL;                          //  LFO��·��̬����
	m_pLnMon = NULL;                          //  LFO��·������Ϣ
	m_pLnOsc = NULL;                          //  LFO��·�񵴱�־
	m_nLnProDat = 0;                           //  �账�����·
	m_pLnProDat = NULL;                        //  �账�����·����(Ԥ�к�)
	m_nLnWarn = 0;
	sum_WarnLn = 0;
	m_pLnWarn = NULL;
	m_pLnResSave = NULL;
	m_DevGnWarn = NULL;							//�澯�豸
	m_DevLnWarn = NULL;							//�澯�豸
	m_DevTrWarn = NULL;							//�澯�豸
	sum_WarnTr = 0;
	sum_DevGnWarn = NULL;
	sum_DevLnWarn = NULL;
	sum_DevTrWarn = NULL;
	sum_pGnDat = NULL;
	sum_pLnDat = NULL;
	sum_pTrDat = NULL;
	////  powersystem info
	m_LfoAmplOverLmt = false; //LFO����(���Խ�ޱ�־) 
	memset(&m_ThisLfoInfo, 0, sizeof(THIS_LFO_INFO));		   //  ���ε�Ƶ����Ϣ
	memset((void*)&m_SysMon, 0, sizeof(SSO_SYS_MON));       //  LFOϵͳ������Ϣ
	memset((void*)&m_SysMon_Old, 0, sizeof(SSO_SYS_MON));   //  LFOϵͳǰһ�ּ�����Ϣ
	memset((void*)&m_SysAna, 0, sizeof(LFO_SYS_ANA));       //  LFOϵͳ������Ϣ
	memset((void*)&m_SysLfo, 0, sizeof(LFO_SYS_LFO));       //  LFOϵͳ����Ϣ

	////  Case info
	m_nCases = 0;                              //  ��������
	m_pCases = NULL;                           //  ������Ϣ

	m_nMonMode = 0;	                           //  mon ģʽ��
	m_pMonMode = NULL;	                       //  mon ģʽ


	// ������ʾLFOʵ��������
	memset((char*)&m_LfoCurveGenDev, 0, sizeof(SSO_GN_PMU));
	// 	m_LfoCurveGenDev = -10 ;
	// 	m_LfoCurveGenDev.col_no  = -10 ;
	memset((char*)&m_LfoCurveLineDev, 0, sizeof(SSO_LN_PMU));
	memset((char*)&m_LfoCurveTrDev, 0, sizeof(SSO_LN_PMU));
	// 	m_LfoCurveLineDev.file_no = -10 ;
	// 	m_LfoCurveLineDev.col_no  = -10 ;   
	memset((char*)&m_GenLfoCurve, 0, sizeof(TAB_LFO_RT_CURVE));
	memset((char*)&m_AclnLfoCurve, 0, sizeof(TAB_LFO_RT_CURVE));
	memset((char*)&m_TimeLfoCurve, 0, sizeof(TAB_LFO_RT_CURVE));
	memset((char*)&m_GenPowCurve, 0, sizeof(TAB_LFO_RT_CURVE));  // 2008-06-11 by zhw
	memset((char*)&m_TrLfoCurve, 0, sizeof(TAB_LFO_RT_CURVE));
	for (ii = 0; ii < LFO_CURVE_MAX_PNT; ii++)
	{
		m_GenLfoCurve.seq_cuv_data[ii] = DB_PIC_NO_DIS;
		m_AclnLfoCurve.seq_cuv_data[ii] = DB_PIC_NO_DIS;
		m_GenPowCurve.seq_cuv_data[ii] = DB_PIC_NO_DIS; // 2008-06-11 by zhw
		m_TrLfoCurve.seq_cuv_data[ii] = DB_PIC_NO_DIS;
	}

	// ģʽ����
	memset((char*)m_pLfoModeCurve, 0, sizeof(PERIOD_LEAD_MODE));

	//  ������Ϣ
// 	m_nItfcNum    = 0;                         //  LFO���������
//     m_pItfcDat    = NULL;                      //  LFO���������Ϣ
//     m_nItfcDevNum = 0;                         //  LFO������·��
//     m_pItfcDevInf = NULL;                      //  LFO������·��Ϣ
//     m_pItfcDevPmu = NULL;                      //  LFO������·PMU��Ϣ
// 	m_pItfcDevDat = NULL;                      //  LFO������·��̬����(Ԥ�����)

	// �����ͺ�������������2 �ӷ���10��
	para_nDelaySec = 2;                        //  ȡ���������Ӻ�����
	para_CalcAngByEA = 0;                      //  �Ƿ���㹦��  2008-05-22 by zhw

	//  ������ģʽ
	m_nAllObjMode = 0;                        //  ������ģʽ����(�޳���ͬģʽ)
	m_pAllObjMode = NULL;                     //  ������ģʽ

	//  δɸѡģʽ����
	m_nAllMode = 0;                        //  δɸѡģʽ����
	m_pAllMode = NULL;                     //  δɸѡģʽ

	//  ģʽ����
	m_nSltMode = 0;                        //  ϵͳģʽ����
	m_pSltMode = NULL;                      //  ϵͳģʽ(��Ⱥϸ��)

	//  LFO�ڼ�ģʽ����
	m_nLfoMode = 0;                        //  LFO�ڼ�ģʽ����
	m_pLfoMode = NULL;                      //  LFO�ڼ�ģʽ
	m_pLfoWds = NULL;                      //  LFO�ڼ���㴰��

	m_nCntrFc = 0;                    //  �����ĳ�վ��
	m_pCntrFc = NULL;                 //  �����ĳ�վ��Ϣ
	m_nCntrLn = 0;                    //  �����ĳ�վ��
	m_pCntrLn = NULL;                 //  LFO��������·
	// �������ƴ�ʩ��Ϣ(���������ĺͲ�������)
	// �����ʣ�����/���⣻��Ϣ������
	// ˵����˫��

	// ˵����������
	g_pProny = new CPronyAnalysis;
#ifdef _LFO_ASST_DECI_
	InitAsstDeci();
#endif
	//////////////////////////////////////////////////////////////////////////
	return;
}

// ����
CLfoAnaObj::~CLfoAnaObj()
{
#ifdef _LFO_ASST_DECI_
	ReleaseAsst();
#endif
	// 	////  �ͷ���־���ʻ�����
	THRD_ReleaseThreadMutex(m_logmutex);
	////  �ͷ�ʵʱ���߻�����
	THRD_ReleaseThreadMutex(m_RtCurvemutex);
	////  �ͷŶ��滥����
	THRD_ReleaseThreadMutex(m_Itfcmutex);
	THRD_ReleaseThreadMutex(m_DynFilemutex);

	if (g_pProny)    delete g_pProny;
	ReleaseMem();

	return;
}

// ���·����ڴ�ǰ���ͷ��ڴ�
void CLfoAnaObj::ReleaseMem()
{
	if (m_pFcInf) { free(m_pFcInf);   m_pFcInf = NULL; }
	if (m_pFcCtl) { free(m_pFcCtl);   m_pFcCtl = NULL; }
	if (m_pFcPmu) { free(m_pFcPmu);   m_pFcPmu = NULL; }
	if (m_pFcDat) { free(m_pFcDat);   m_pFcDat = NULL; }
	if (m_pFcMon) { free(m_pFcMon);   m_pFcMon = NULL; }
	if (m_pFcLfo) { free(m_pFcLfo);   m_pFcLfo = NULL; }
	if (m_pFcProDat) { free(m_pFcProDat);   m_pFcProDat = NULL; }
	if (m_pFcAna) { free(m_pFcAna);   m_pFcAna = NULL; }

	if (m_pGnInf) { free(m_pGnInf);   m_pGnInf = NULL; }
	if (m_pGnCtl) { free(m_pGnCtl);   m_pGnCtl = NULL; }
	if (m_pGnPmu) { free(m_pGnPmu);   m_pGnPmu = NULL; }
	if (m_pGnDat) { free(m_pGnDat);   m_pGnDat = NULL; }
	if (m_pGnMon) { free(m_pGnMon);   m_pGnMon = NULL; }
	if (m_pGnFact) { free(m_pGnFact);   m_pGnFact = NULL; }
	if (m_pGnOsc) { free(m_pGnOsc);   m_pGnOsc = NULL; }
	if (m_pGnProDat) { free(m_pGnProDat); m_pGnProDat = NULL; }
	if (m_pGnWarn) { free(m_pGnWarn);   m_pGnWarn = NULL; }
	if (m_pGnResSave) { free(m_pGnResSave);   m_pGnResSave = NULL; }
	if (m_DevGnWarn) { free(m_DevGnWarn);   m_DevGnWarn = NULL; }
	if (sum_DevGnWarn) { free(sum_DevGnWarn);   sum_DevGnWarn = NULL; }
	if (sum_pGnDat) { free(sum_pGnDat);   sum_pGnDat = NULL; }
	if (sum_pGnInf) { free(sum_pGnInf);   sum_pGnInf = NULL; }
	if (gn_warn_time) { free(gn_warn_time);   gn_warn_time = NULL; }
	if (m_pDevWarn) { free(m_pDevWarn);   m_pDevWarn = NULL; }
	if (m_pDevEveWarn) { free(m_pDevEveWarn);   m_pDevEveWarn = NULL; }
	if (m_pEveFcWarn) { free(m_pEveFcWarn);   m_pEveFcWarn = NULL; }



	if (m_pLnInf) { free(m_pLnInf);   m_pLnInf = NULL; }
	if (m_pLnCtl) { free(m_pLnCtl);   m_pLnCtl = NULL; }
	if (m_pLnPmu) { free(m_pLnPmu);   m_pLnPmu = NULL; }
	if (m_pLnDat) { free(m_pLnDat);   m_pLnDat = NULL; }
	if (m_pLnMon) { free(m_pLnMon);   m_pLnMon = NULL; }
	if (m_pLnFact) { free(m_pLnFact);   m_pLnFact = NULL; }
	if (m_pLnOsc) { free(m_pLnOsc);   m_pLnOsc = NULL; }
	if (m_pLnProDat) { free(m_pLnProDat); m_pLnProDat = NULL; }
	if (m_pLnWarn) { free(m_pLnWarn);   m_pLnWarn = NULL; }
	if (m_pLnResSave) { free(m_pLnResSave);   m_pLnResSave = NULL; }
	if (m_DevLnWarn) { free(m_DevLnWarn);   m_DevLnWarn = NULL; }
	if (sum_DevLnWarn) { free(sum_DevLnWarn);   sum_DevLnWarn = NULL; }
	if (sum_pLnDat) { free(sum_pLnDat);   sum_pLnDat = NULL; }
	if (sum_pLnInf) { free(sum_pLnInf);   sum_pLnInf = NULL; }
	if (ln_warn_time) { free(ln_warn_time);   ln_warn_time = NULL; }

	if (m_pTrInf) { free(m_pTrInf);   m_pTrInf = NULL; }
	if (m_pTrCtl) { free(m_pTrCtl);   m_pTrCtl = NULL; }
	if (m_pTrPmu) { free(m_pTrPmu);   m_pTrPmu = NULL; }
	if (m_pTrDat) { free(m_pTrDat);   m_pTrDat = NULL; }
	if (m_pTrMon) { free(m_pTrMon);   m_pTrMon = NULL; }
	if (m_pTrOsc) { free(m_pTrOsc);   m_pTrOsc = NULL; }
	if (m_pTrProDat) { free(m_pTrProDat); m_pTrProDat = NULL; }
	if (m_pTrWarn) { free(m_pTrWarn);   m_pTrWarn = NULL; }
	if (m_pTrResSave) { free(m_pTrResSave);   m_pTrResSave = NULL; }
	if (m_DevTrWarn) { free(m_DevTrWarn);   m_DevTrWarn = NULL; }
	if (sum_DevTrWarn) { free(sum_DevTrWarn);   sum_DevTrWarn = NULL; }
	if (sum_pTrDat) { free(sum_pTrDat);   sum_pTrDat = NULL; }
	if (sum_pTrInf) { free(sum_pTrInf);   sum_pTrInf = NULL; }
	if (tr_warn_time) { free(tr_warn_time);   tr_warn_time = NULL; }

	// ����������
	if (m_pCntrFc) { free(m_pCntrFc);   m_pCntrFc = NULL; }
	////  Case info
	if (m_pCases) { free(m_pCases);   m_pCases = NULL; }
	if (m_pMonMode) { free(m_pMonMode);   m_pMonMode = NULL; }

	//  ������Ϣ
//     if ( m_pItfcDat )       { free( m_pItfcDat );      m_pItfcDat = NULL; }
//     if ( m_pItfcDevInf )    { free( m_pItfcDevInf );   m_pItfcDevInf = NULL; }
//     if ( m_pItfcDevPmu )    { free( m_pItfcDevPmu );   m_pItfcDevPmu = NULL; }
//     if ( m_pItfcDevDat )    { free( m_pItfcDevDat );   m_pItfcDevDat = NULL; }

	//  ������ģʽ
	if (m_pAllObjMode) { free(m_pAllObjMode);   m_pAllObjMode = NULL; }

	//  δɸѡģʽ
	if (m_pAllMode) { free(m_pAllMode);   m_pAllMode = NULL; }
	//  ģʽ����
	if (m_pSltMode) { free(m_pSltMode);   m_pSltMode = NULL; }

	//  LFO�ڼ�ģʽ����
	if (m_pLfoMode) { free(m_pLfoMode);   m_pLfoMode = NULL; }
	if (m_pLfoWds) { free(m_pLfoWds);   m_pLfoWds = NULL; }

#ifdef _LFO_ASST_DECI_
	if (m_pAsstDeci) { free(m_pAsstDeci);   m_pAsstDeci = NULL; }
#endif
	return;
}

//////////////////////////////// �����ڴ� /////////////////////////////////
// �����ڴ�ռ�
bool CLfoAnaObj::AllocMemforPointer()
{
#ifdef _WAMAP_
	m_nFcNum = PODB_GetTabRecNum(SSO_FAC_NO_TAB);
	m_nGnNum = PODB_GetTabRecNum(SSO_GN_NO_TAB);
	m_nLnNum = PODB_GetTabRecNum(SSO_ACLN_NO_TAB);
	m_nTrNum = PODB_GetTabRecNum(SSO_TR_NO_TAB);
	// 	m_nItfcNum    = PODB_GetTabRecNum( LFO_INTERFACE_NO_TAB );
	// 	m_nItfcDevNum = PODB_GetTabRecNum( LFO_INTERFACE_DEV_NO_TAB );

		// д������ļ�
	if (m_nFcNum > para_nFcMaxMem)  para_nFcMaxMem = m_nFcNum;
	if (m_nGnNum > para_nGnMaxMem)  para_nGnMaxMem = m_nGnNum;
	if (m_nLnNum > para_nLnMaxMem)  para_nLnMaxMem = m_nLnNum;
	if (m_nTrNum > para_nTrMaxMem)  para_nTrMaxMem = m_nTrNum;
	// 	if ( m_nItfcNum > para_nItfcMaxMem )     para_nItfcMaxMem = m_nItfcNum;	
	// 	if ( m_nItfcDevNum > para_nItfcMaxDev )  para_nItfcMaxDev = m_nItfcDevNum;	
#else
	m_nFcNum = para_nFcMaxMem;
	m_nGnNum = para_nGnMaxMem;
	m_nLnNum = para_nLnMaxMem;
	m_nTrNum = para_nTrMaxMem;
	//     m_nItfcNum = para_nItfcMaxMem;
	//     m_nItfcDevNum = para_nItfcMaxDev;
#endif
	para_nCaseMaxMem = para_nAnaMaxFcNum + para_nGnMaxMem + para_nLnMaxMem + para_nTrMaxMem;
	para_nObjMaxMode = (para_nSysMaxMode * para_nCaseMaxMem);
	if (para_nAnaMaxFcNum > para_nFcMaxMem) para_nAnaMaxFcNum = para_nFcMaxMem;
	if (para_nShwMaxFcNum > para_nFcMaxMem) para_nShwMaxFcNum = para_nFcMaxMem;

	sprintf(m_LogString, "AllocMemforPointer(): Alloc memory for all pointer by DB");
	MessShow(m_LogString);
	//////////////////////////////////////////////////////////////////////////
	// para_nFcMaxMem    - Create >> m_pFcInf, m_pFcCtl, m_pFcPmu, m_pFcDat, m_pFcMon, m_pFcLfo, m_pFcProDat
	m_pFcInf = (SSO_FC_INF*)calloc(para_nFcMaxMem, sizeof(SSO_FC_INF));
	m_pFcCtl = (SSO_FC_CTL*)calloc(para_nFcMaxMem, sizeof(SSO_FC_CTL));
	m_pFcPmu = (LFO_FC_PMU*)calloc(para_nFcMaxMem, sizeof(LFO_FC_PMU));
	m_pFcDat = (LFO_FC_DAT*)calloc(para_nFcMaxMem, sizeof(LFO_FC_DAT));
	m_pFcMon = (SSO_FC_MON*)calloc(para_nFcMaxMem, sizeof(SSO_FC_MON));
	m_pFcLfo = (SSO_FC_MON*)calloc(para_nFcMaxMem, sizeof(SSO_FC_MON));
	m_pFcProDat = (LFO_FC_DAT*)calloc(para_nFcMaxMem, sizeof(LFO_FC_DAT));
	m_pFcAna = (LFO_FC_ANA*)calloc(para_nFcMaxMem, sizeof(LFO_FC_ANA));
	m_pFcWarn = (SSO_FC_MON*)calloc(para_nFcMaxMem, sizeof(SSO_FC_MON));

	memset((void*)m_pFcInf, 0, para_nFcMaxMem * sizeof(SSO_FC_INF));
	memset((void*)m_pFcCtl, 0, para_nFcMaxMem * sizeof(SSO_FC_CTL));
	memset((void*)m_pFcPmu, 0, para_nFcMaxMem * sizeof(LFO_FC_PMU));
	memset((void*)m_pFcDat, 0, para_nFcMaxMem * sizeof(LFO_FC_DAT));
	memset((void*)m_pFcMon, 0, para_nFcMaxMem * sizeof(SSO_FC_MON));
	memset((void*)m_pFcLfo, 0, para_nFcMaxMem * sizeof(SSO_FC_MON));
	memset((void*)m_pFcProDat, 0, para_nFcMaxMem * sizeof(LFO_FC_DAT));
	memset((void*)m_pFcAna, 0, para_nFcMaxMem * sizeof(LFO_FC_ANA));
	// para_nGnMaxMem    - Create >> m_pGnInf, m_pGnCtl, m_pGnPmu, m_pGnDat, m_pGnMon, m_pGnOsc
	m_pGnInf = (SSO_GN_INF*)calloc(para_nGnMaxMem, sizeof(SSO_GN_INF));
	m_pGnCtl = (SSO_GN_CTL*)calloc(para_nGnMaxMem, sizeof(SSO_GN_CTL));
	m_pGnPmu = (SSO_GN_PMU*)calloc(para_nGnMaxMem, sizeof(SSO_GN_PMU));
	m_pGnDat = (SSO_GN_DAT*)calloc(para_nGnMaxMem, sizeof(SSO_GN_DAT));
	m_pGnMon = (SSO_GN_MON*)calloc(para_nGnMaxMem, sizeof(SSO_GN_MON));
	m_pGnFact = (OBJ_ACT_FACT*)calloc(para_nGnMaxMem, sizeof(OBJ_ACT_FACT));  //2008-04-04 added by zhw at yn
	m_pGnOsc = (OSCILLAT_FLAG*)calloc(para_nGnMaxMem, sizeof(OSCILLAT_FLAG));
	m_pGnProDat = (SSO_GN_DAT*)calloc(para_nGnMaxMem, sizeof(SSO_GN_DAT));
	m_pGnWarn = (SSO_GN_MON*)calloc(para_nGnMaxMem, sizeof(SSO_GN_MON));
	m_pGnResSave = (LFO_OBJ_RES_SAVE*)calloc(para_nGnMaxMem, sizeof(LFO_OBJ_RES_SAVE));
	m_DevGnWarn = (SSO_GN_MON*)calloc(para_nGnMaxMem, sizeof(SSO_GN_MON));
	sum_DevGnWarn = (SSO_GN_MON*)calloc(para_nGnMaxMem, sizeof(SSO_GN_MON));
	sum_pGnInf = (SSO_GN_INF*)calloc(para_nGnMaxMem, sizeof(SSO_GN_INF));
	sum_pGnDat = (SSO_GN_DAT*)calloc(para_nGnMaxMem, sizeof(SSO_GN_DAT));
	gn_warn_time = (TYPE_GN_WARN_TIME*)calloc(para_nGnMaxMem, sizeof(TYPE_GN_WARN_TIME));
	m_pDevWarn = (SSO_DEV_MON*)calloc((para_nGnMaxMem + para_nLnMaxMem + para_nTrMaxMem), sizeof(SSO_DEV_MON));
	m_pDevEveWarn = (SSO_DEV_MON*)calloc((para_nGnMaxMem + para_nLnMaxMem + para_nTrMaxMem), sizeof(SSO_DEV_MON));
	m_pEveFcWarn = (SSO_FC_MON*)calloc((para_nFcMaxMem), sizeof(SSO_FC_MON));

	memset((void*)m_pGnInf, 0, para_nGnMaxMem * sizeof(SSO_GN_INF));
	memset((void*)m_pGnCtl, 0, para_nGnMaxMem * sizeof(SSO_GN_CTL));
	memset((void*)m_pGnPmu, 0, para_nGnMaxMem * sizeof(SSO_GN_PMU));
	memset((void*)m_pGnDat, 0, para_nGnMaxMem * sizeof(SSO_GN_DAT));
	memset((void*)m_pGnMon, 0, para_nGnMaxMem * sizeof(SSO_GN_MON));
	memset((void*)m_pGnOsc, 0, para_nGnMaxMem * sizeof(OSCILLAT_FLAG));
	memset((void*)m_pGnProDat, 0, para_nGnMaxMem * sizeof(SSO_GN_DAT));
	memset((void*)m_pGnWarn, 0, para_nGnMaxMem * sizeof(SSO_GN_MON));
	memset((void*)m_pGnResSave, 0, para_nGnMaxMem * sizeof(LFO_OBJ_RES_SAVE));
	memset((void*)m_DevGnWarn, 0, para_nGnMaxMem * sizeof(SSO_GN_MON));
	memset((void*)sum_DevGnWarn, 0, para_nGnMaxMem * sizeof(SSO_GN_MON));
	memset((void*)sum_pGnInf, 0, para_nGnMaxMem * sizeof(SSO_GN_INF));
	memset((void*)sum_pGnDat, 0, para_nGnMaxMem * sizeof(SSO_GN_DAT));
	memset((void*)gn_warn_time, 0, para_nGnMaxMem * sizeof(TYPE_GN_WARN_TIME));

	// para_nLnMaxMem    - Create >> m_pLnInf, m_pLnCtl, m_pLnPmu, m_pLnDat, m_pLnMon, m_pLnOsc
	m_pLnInf = (SSO_LN_INF*)calloc(para_nLnMaxMem, sizeof(SSO_LN_INF));
	m_pLnCtl = (SSO_LN_CTL*)calloc(para_nLnMaxMem, sizeof(SSO_LN_CTL));
	m_pLnPmu = (SSO_LN_PMU*)calloc(para_nLnMaxMem, sizeof(SSO_LN_PMU));
	m_pLnDat = (SSO_LN_DAT*)calloc(para_nLnMaxMem, sizeof(SSO_LN_DAT));
	m_pLnMon = (SSO_LN_MON*)calloc(para_nLnMaxMem, sizeof(SSO_LN_MON));
	m_pLnFact = (OBJ_ACT_FACT*)calloc(para_nLnMaxMem, sizeof(OBJ_ACT_FACT));  //2008-05-21 added by zhw at nj
	m_pLnOsc = (OSCILLAT_FLAG*)calloc(para_nLnMaxMem, sizeof(OSCILLAT_FLAG));
	m_pLnProDat = (SSO_LN_DAT*)calloc(para_nLnMaxMem, sizeof(SSO_LN_DAT));
	m_pLnWarn = (SSO_LN_MON*)calloc(para_nLnMaxMem, sizeof(SSO_LN_MON));
	m_pLnResSave = (LFO_OBJ_RES_SAVE*)calloc(para_nLnMaxMem, sizeof(LFO_OBJ_RES_SAVE));
	m_DevLnWarn = (SSO_LN_MON*)calloc(para_nLnMaxMem, sizeof(SSO_LN_MON));
	sum_DevLnWarn = (SSO_LN_MON*)calloc(para_nLnMaxMem, sizeof(SSO_LN_MON));
	sum_pLnInf = (SSO_LN_INF*)calloc(para_nLnMaxMem, sizeof(SSO_LN_INF));
	sum_pLnDat = (SSO_LN_DAT*)calloc(para_nLnMaxMem, sizeof(SSO_LN_DAT));
	ln_warn_time = (TYPE_LN_WARN_TIME*)calloc(para_nLnMaxMem, sizeof(TYPE_LN_WARN_TIME));

	memset((void*)m_pLnInf, 0, para_nLnMaxMem * sizeof(SSO_LN_INF));
	memset((void*)m_pLnCtl, 0, para_nLnMaxMem * sizeof(SSO_LN_CTL));
	memset((void*)m_pLnPmu, 0, para_nLnMaxMem * sizeof(SSO_LN_PMU));
	memset((void*)m_pLnDat, 0, para_nLnMaxMem * sizeof(SSO_LN_DAT));
	memset((void*)m_pLnMon, 0, para_nLnMaxMem * sizeof(SSO_LN_MON));
	memset((void*)m_pLnOsc, 0, para_nLnMaxMem * sizeof(OSCILLAT_FLAG));
	memset((void*)m_pLnProDat, 0, para_nLnMaxMem * sizeof(SSO_LN_DAT));
	memset((void*)m_pLnWarn, 0, para_nLnMaxMem * sizeof(SSO_LN_MON));
	memset((void*)m_pLnResSave, 0, para_nLnMaxMem * sizeof(LFO_OBJ_RES_SAVE));
	memset((void*)m_DevLnWarn, 0, para_nLnMaxMem * sizeof(SSO_LN_MON));
	memset((void*)sum_pLnInf, 0, para_nLnMaxMem * sizeof(SSO_LN_INF));
	memset((void*)sum_DevLnWarn, 0, para_nLnMaxMem * sizeof(SSO_LN_MON));
	memset((void*)sum_pLnDat, 0, para_nLnMaxMem * sizeof(SSO_LN_DAT));
	memset((void*)ln_warn_time, 0, para_nLnMaxMem * sizeof(TYPE_LN_WARN_TIME));

	m_pTrInf = (SSO_TR_INF*)calloc(para_nTrMaxMem, sizeof(SSO_TR_INF));
	m_pTrCtl = (SSO_TR_CTL*)calloc(para_nTrMaxMem, sizeof(SSO_TR_CTL));
	m_pTrPmu = (SSO_TR_PMU*)calloc(para_nTrMaxMem, sizeof(SSO_TR_PMU));
	m_pTrDat = (SSO_TR_DAT*)calloc(para_nTrMaxMem, sizeof(SSO_TR_DAT));
	m_pTrMon = (SSO_TR_MON*)calloc(para_nTrMaxMem, sizeof(SSO_TR_MON));
	m_pTrOsc = (OSCILLAT_FLAG*)calloc(para_nTrMaxMem, sizeof(OSCILLAT_FLAG));
	m_pTrProDat = (SSO_TR_DAT*)calloc(para_nTrMaxMem, sizeof(SSO_TR_DAT));
	m_pTrWarn = (SSO_TR_MON*)calloc(para_nTrMaxMem, sizeof(SSO_TR_MON));
	m_pTrResSave = (LFO_OBJ_RES_SAVE*)calloc(para_nTrMaxMem, sizeof(LFO_OBJ_RES_SAVE));
	m_DevTrWarn = (SSO_TR_MON*)calloc(para_nTrMaxMem, sizeof(SSO_TR_MON));
	sum_DevTrWarn = (SSO_TR_MON*)calloc(para_nTrMaxMem, sizeof(SSO_TR_MON));
	sum_pTrInf = (SSO_TR_INF*)calloc(para_nTrMaxMem, sizeof(SSO_TR_INF));
	sum_pTrDat = (SSO_TR_DAT*)calloc(para_nTrMaxMem, sizeof(SSO_TR_DAT));
	tr_warn_time = (TYPE_TR_WARN_TIME*)calloc(para_nTrMaxMem, sizeof(TYPE_TR_WARN_TIME));

	memset((void*)m_pTrInf, 0, para_nTrMaxMem * sizeof(SSO_TR_INF));
	memset((void*)m_pTrCtl, 0, para_nTrMaxMem * sizeof(SSO_TR_CTL));
	memset((void*)m_pTrPmu, 0, para_nTrMaxMem * sizeof(SSO_TR_PMU));
	memset((void*)m_pTrDat, 0, para_nTrMaxMem * sizeof(SSO_TR_DAT));
	memset((void*)m_pTrMon, 0, para_nTrMaxMem * sizeof(SSO_TR_MON));
	memset((void*)m_pTrOsc, 0, para_nTrMaxMem * sizeof(OSCILLAT_FLAG));
	memset((void*)m_pTrProDat, 0, para_nTrMaxMem * sizeof(SSO_TR_DAT));
	memset((void*)m_pTrWarn, 0, para_nTrMaxMem * sizeof(SSO_TR_MON));
	memset((void*)m_pTrResSave, 0, para_nTrMaxMem * sizeof(LFO_OBJ_RES_SAVE));
	memset((void*)m_DevTrWarn, 0, para_nTrMaxMem * sizeof(SSO_TR_MON));
	memset((void*)sum_DevTrWarn, 0, para_nTrMaxMem * sizeof(SSO_TR_MON));
	memset((void*)sum_pTrInf, 0, para_nTrMaxMem * sizeof(SSO_TR_INF));
	memset((void*)sum_pTrDat, 0, para_nTrMaxMem * sizeof(SSO_TR_DAT));
	memset((void*)tr_warn_time, 0, para_nTrMaxMem * sizeof(TYPE_TR_WARN_TIME));

	// para_nAnaMaxFcNum - Create >>  m_pCntrFc
	m_pCntrFc = (SSO_FC_MON*)calloc(para_nAnaMaxFcNum, sizeof(SSO_FC_MON));
	memset((void*)m_pCntrFc, 0, para_nAnaMaxFcNum * sizeof(SSO_FC_MON));

	// para_nCaseMaxMem  - Create >> m_pCases
	m_pCases = (CASE_TYPE*)calloc(para_nCaseMaxMem, sizeof(CASE_TYPE));
	memset((void*)m_pCases, 0, para_nCaseMaxMem * sizeof(CASE_TYPE));

	m_pMonMode = (PRONY_ANAL_RSLT*)calloc(para_nCaseMaxMem, sizeof(PRONY_ANAL_RSLT));
	memset((void*)m_pMonMode, 0, para_nCaseMaxMem * sizeof(PRONY_ANAL_RSLT));

	m_pFcHDFile = (SSO_HD_FILE_FAC*)calloc(para_nFcMaxMem, sizeof(SSO_HD_FILE_FAC));
	memset((void*)m_pFcHDFile, 0, para_nFcMaxMem * sizeof(SSO_HD_FILE_FAC));
	// para_nItfcMaxMem - Create >> m_pItfcDat
// 	m_pItfcDat = (LFO_INTERFACE *)calloc(para_nItfcMaxMem, sizeof(LFO_INTERFACE));
// 	memset((void *)m_pItfcDat, 0, para_nItfcMaxMem * sizeof(LFO_INTERFACE));

	// para_nItfcMaxDev  - Create >> m_pItfcDevInf, m_pItfcDevPmu, m_pItfcDevDat
	//m_pItfcDevInf = (LFO_INTERFACE_DEV *)calloc(para_nItfcMaxDev, sizeof(LFO_INTERFACE_DEV));
// 	m_pItfcDevPmu = (LFO_LN_PMU *)calloc(para_nItfcMaxDev, sizeof(LFO_LN_PMU));
// 	m_pItfcDevDat = (LFO_LN_DAT *)calloc(para_nItfcMaxDev, sizeof(LFO_LN_DAT));

	//memset((void *)m_pItfcDevInf, 0, para_nItfcMaxDev * sizeof(LFO_INTERFACE_DEV));
// 	memset((void *)m_pItfcDevPmu, 0, para_nItfcMaxDev * sizeof(LFO_LN_PMU));
// 	memset((void *)m_pItfcDevDat, 0, para_nItfcMaxDev * sizeof(LFO_LN_DAT));

	// para_nObjMaxMode  - Create >> m_pAllObjMode
	m_pAllObjMode = (MODE_PART_OBJ*)calloc(para_nObjMaxMode, sizeof(MODE_PART_OBJ));
	memset((void*)m_pAllObjMode, 0, para_nObjMaxMode * sizeof(MODE_PART_OBJ));

	// para_nSysMaxMode  - Create >> m_pAllMode, m_pSltMode
	m_pAllMode = (MODE_CHRCT*)calloc(para_nSysMaxMode, sizeof(MODE_CHRCT));
	m_pSltMode = (MODE_CHRCT*)calloc(para_nSysMaxMode, sizeof(MODE_CHRCT));

	memset((void*)m_pAllMode, 0, para_nSysMaxMode * sizeof(MODE_CHRCT));
	memset((void*)m_pSltMode, 0, para_nSysMaxMode * sizeof(MODE_CHRCT));

	// Period_Mode_Max   - Create >> m_pLfoMode, m_pLfoWds
	m_nLfoMode = 0;                        //  LFO�ڼ�ģʽ����
	m_pLfoMode = (MODE_CHRCT*)calloc(Period_Mode_Max, sizeof(MODE_CHRCT));
	m_pLfoWds = (LFO_CALC_WDS*)calloc(Period_Mode_Max, sizeof(LFO_CALC_WDS));

	memset((void*)m_pLfoMode, 0, Period_Mode_Max * sizeof(MODE_CHRCT));
	memset((void*)m_pLfoWds, 0, Period_Mode_Max * sizeof(LFO_CALC_WDS));
#ifdef _LFO_ASST_DECI_
	m_nAsstDeci = 0;                         // ���������� 2008-09-09
	m_pAsstDeci = (TAB_LFO_ASST_DECI*)calloc(para_nAsstDeciMax, sizeof(TAB_LFO_ASST_DECI));
	memset((void*)m_pAsstDeci, 0, para_nAsstDeciMax * sizeof(TAB_LFO_ASST_DECI));
#endif
	//��ʼ������״̬
	int ii, jj;
	for (ii = 0; ii < para_nFcMaxMem; ii++)
	{
		for (jj = 0; jj < MAX_tIMW_WIN; jj++)
		{
			m_pFcDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_OK;
		}
	}
	for (ii = 0; ii < para_nGnMaxMem; ii++)
	{
		for (jj = 0; jj < MAX_tIMW_WIN; jj++)
		{
			m_pGnDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_OK;
		}
	}
	for (ii = 0; ii < para_nLnMaxMem; ii++)
	{
		for (jj = 0; jj < MAX_tIMW_WIN; jj++)
		{
			m_pLnDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_OK;
		}
	}
	for (ii = 0; ii < para_nTrMaxMem; ii++)
	{
		for (jj = 0; jj < MAX_tIMW_WIN; jj++)
		{
			m_pTrDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_OK;
		}
	}

	sprintf(m_LogString, "AllocMemforPointer(): nFcMaxMem[%d]   nGnMaxMem[%d]    nLnMaxMem[%d]   nItfcMaxMem[%d]  para_nItfcMaxDev[%d]\n nCaseMaxMem[%d] nAnaMaxFcNum[%d] nObjMaxMode[%d] nSysMaxMode[%d]",
		para_nFcMaxMem, para_nGnMaxMem, para_nLnMaxMem, para_nItfcMaxMem, para_nItfcMaxDev,
		para_nCaseMaxMem, para_nAnaMaxFcNum, para_nObjMaxMode, para_nSysMaxMode);
	MessShow(m_LogString);
#ifdef _LFO_ASST_DECI_
	AllocAsstMem();
#endif
	return true;
}

// ��ȡ����������Ϣ
int CLfoAnaObj::ReadDefaultFile()
{
	char  filename[MAXBUF];
	std::string sectionName;
	std::string sTempStr;
	int      TempInt;
#ifdef _WINDOWS32
	sprintf(filename, "%s\\ini\\Default.ini", m_RunInfDir);
#else
	sprintf(filename, "%s/ini/Default.ini", m_RunInfDir);
#endif
	CIniFile iniFile(filename);
	if (iniFile.INI_ReadFile() == false)
	{
		sprintf(m_LogString, "�����ļ���ȡʧ��: %s", filename);
		MessShow(m_LogString);
		return -1;
	}

	sectionName = "Ӧ�ò���";
	g_ctxName = iniFile.INI_GetValue(sectionName, "ctx_name", AC_REALTIME_NAME);
	g_ctxNo = iniFile.INI_GetValueI(sectionName, "ctx_no", AC_REALTIME_NO);
	g_appName = iniFile.INI_GetValue(sectionName, "app_name", AF_WAMS_NAME);
	g_appNo = iniFile.INI_GetValueI(sectionName, "app_no", AP_WAMS);

	sectionName = "�������";
	m_LfoPara.time_leng = iniFile.INI_GetValueI(sectionName, "time_leng", 2);
	m_LfoPara.int_time = iniFile.INI_GetValueI(sectionName, "int_time", 0);
	m_LfoPara.freq_min = iniFile.INI_GetValueFF(sectionName, "freq_min", 10);
	m_LfoPara.freq_max = iniFile.INI_GetValueFF(sectionName, "freq_max", 90);
	para_FcVoltPer = iniFile.INI_GetValueFF(sectionName, "FcVoltPer", 0.0);
	para_Ampl_aver = iniFile.INI_GetValueFF(sectionName, "Ampl_aver", 0.6);
	para_VoltAmplPer = iniFile.INI_GetValueFF(sectionName, "VoltAmplPer", 0.1);
	para_AnaWnapPer = iniFile.INI_GetValueFF(sectionName, "AnaWnapPer", 30.0);
	para_MaxAnglDiff = iniFile.INI_GetValueFF(sectionName, "MaxAnglDiff", 300.0);
	m_LfoPara.sso_jud_num = iniFile.INI_GetValueI(sectionName, "sso_jud_num", 5);
	m_LfoPara.volt_thre1 = iniFile.INI_GetValueFF(sectionName, "volt_thre1", 220);
	m_LfoPara.volt_thre2 = iniFile.INI_GetValueFF(sectionName, "volt_thre2", 500);
	m_LfoPara.gn_swn_ampl = iniFile.INI_GetValueFF(sectionName, "gn_swn_ampl", 10.0);
	m_LfoPara.gn_ewn_ampl = iniFile.INI_GetValueFF(sectionName, "gn_ewn_ampl", 10.0);
	m_LfoPara.ln_swn_ampl1 = iniFile.INI_GetValueFF(sectionName, "ln_swn_ampl1", 10.0);
	m_LfoPara.ln_swn_ampl2 = iniFile.INI_GetValueFF(sectionName, "ln_swn_ampl2", 25.0);
	m_LfoPara.ln_swn_ampl3 = iniFile.INI_GetValueFF(sectionName, "ln_swn_ampl3", 35.0);
	m_LfoPara.ln_ewn_ampl1 = iniFile.INI_GetValueFF(sectionName, "ln_ewn_ampl1", 10.0);
	m_LfoPara.ln_ewn_ampl2 = iniFile.INI_GetValueFF(sectionName, "ln_ewn_ampl2", 25.0);
	m_LfoPara.ln_ewn_ampl3 = iniFile.INI_GetValueFF(sectionName, "ln_ewn_ampl3", 35.0);
	m_LfoPara.tr_swn_ampl1 = iniFile.INI_GetValueFF(sectionName, "tr_swn_ampl1", 10.0);
	m_LfoPara.tr_swn_ampl2 = iniFile.INI_GetValueFF(sectionName, "tr_swn_ampl2", 25.0);
	m_LfoPara.tr_swn_ampl3 = iniFile.INI_GetValueFF(sectionName, "tr_swn_ampl3", 35.0);
	m_LfoPara.tr_ewn_ampl1 = iniFile.INI_GetValueFF(sectionName, "tr_ewn_ampl1", 10.0);
	m_LfoPara.tr_ewn_ampl2 = iniFile.INI_GetValueFF(sectionName, "tr_ewn_ampl2", 25.0);
	m_LfoPara.tr_ewn_ampl3 = iniFile.INI_GetValueFF(sectionName, "tr_ewn_ampl3", 35.0);

	sectionName = "���Ʋ���";
	para_CalcPQVByUI = iniFile.INI_GetValueI(sectionName, "CalcPQVByUI", 0);
	para_LfoAnaByPwr = iniFile.INI_GetValueI(sectionName, "LfoAnaByPwr", 0);
	g_nCalcThrd = iniFile.INI_GetValueI(sectionName, "nCalcThrd", 0);
	para_nDelaySec = iniFile.INI_GetValueI(sectionName, "DelaySec", 2);
	para_UpAppVariable = iniFile.INI_GetValueI(sectionName, "UpAppVariable", 0);
	para_DebugProny = iniFile.INI_GetValueI(sectionName, "DebugProny", 0);
	para_IsCallDynFile = iniFile.INI_GetValueI(sectionName, "IsCallDynFile", 0);
	para_CallDynDelaySec = iniFile.INI_GetValueI(sectionName, "CallDynDelaySec", 35);
	para_CallAllFacDyn = iniFile.INI_GetValueI(sectionName, "CallAllFacDyn", 0);
	para_FormAsstDeci = iniFile.INI_GetValueI(sectionName, "FormAsstDeci", 0);
	para_UpDevLfoStat = iniFile.INI_GetValueI(sectionName, "UpDevLfoStat", 0);
	para_IsDataCycle = iniFile.INI_GetValueI(sectionName, "is_data_cycle", 0);
	para_IsFltSim = iniFile.INI_GetValueI(sectionName, "IsFltSim", 0);
	para_IsWarnIA = iniFile.INI_GetValueI(sectionName, "IsWarnCIA", 0);
	para_ShowFacName = iniFile.INI_GetValueI(sectionName, "ShowFacName", 1);
	para_WarnAppNo = iniFile.INI_GetValueI(sectionName, "WarnAppNo", AP_SCADA);
	para_IsDataValid = iniFile.INI_GetValueI(sectionName, "IsDataValid", 1);
	m_LfoPara.delay_time = iniFile.INI_GetValueFF(sectionName, "Delay_time", 30.0);

	sectionName = "�����ģ";
	para_nFcMaxMem = iniFile.INI_GetValueI(sectionName, "nFcMaxMem", 150);     //  ���ڴ泧վ�����
	para_nGnMaxMem = iniFile.INI_GetValueI(sectionName, "nGnMaxMem", 200);	  //  ���ڴ���������
	para_nLnMaxMem = iniFile.INI_GetValueI(sectionName, "nLnMaxMem", 200);	  //  ���ڴ���·�����
	para_nTrMaxMem = iniFile.INI_GetValueI(sectionName, "nTrMaxMem", 200);     //  ���ڴ��ѹ�������
	para_nItfcMaxMem = iniFile.INI_GetValueI(sectionName, "nItfcMaxMem", 10);	  //  ���ڴ���������
	para_nItfcMaxDev = iniFile.INI_GetValueI(sectionName, "nItfcMaxDev", 50);	  //  ���ڴ���������·��
	para_nAnaMaxFcNum = iniFile.INI_GetValueI(sectionName, "nAnaMaxFcNum", 100);  //  ������վ�����
	para_nCaseMaxMem = iniFile.INI_GetValueI(sectionName, "nCaseMaxMem", 500);
	para_nSysMaxMode = iniFile.INI_GetValueI(sectionName, "nSysMaxMode", 10);
	para_nAsstDeciMax = iniFile.INI_GetValueI(sectionName, "nAsstDeciMax", 100);
	//g_PubLogFileObj.m_LogSaveNum= iniFile.INI_GetValueI(sectionName,"MaxLogSaveNum");

	sectionName = "�������";
	para_nShwMaxFcNum = iniFile.INI_GetValueI(sectionName, "nShwMaxFcNum", 3);
	para_DebugInfo = iniFile.INI_GetValueI(sectionName, "DebugInfo", 0);
	para_SaveDataToFile = iniFile.INI_GetValueI(sectionName, "SaveDataToFile", 0);
	para_DataSaveNum = iniFile.INI_GetValueI(sectionName, "DataSaveNum", 2);
	para_SaveAllFac = iniFile.INI_GetValueI(sectionName, "SaveAllFac", 1);
	para_IsShowAnaRes = iniFile.INI_GetValueI(sectionName, "IsShowAnaRes", 0);
	para_IsShowAnaData = iniFile.INI_GetValueI(sectionName, "IsShowAnaData", 0);
	para_IsShowAsstDeci = iniFile.INI_GetValueI(sectionName, "IsShowAsstDeci", 0);
	para_SaveXmlFile = iniFile.INI_GetValueI(sectionName, "SaveXmlFile", 0);
#ifdef _WINDOWS32
	sprintf(para_XmlFileDir, "%s\\%s", m_RunInfDir, iniFile.INI_GetValue(sectionName, "XmlFileDir", "save").c_str());  // ɽ��Ӧ��ϵͳ�ӿ��ļ�Ŀ¼ // 2008-06-24 by zhw
#else
	sprintf(para_XmlFileDir, "%s/%s", m_RunInfDir, iniFile.INI_GetValue(sectionName, "XmlFileDir", "save").c_str());
#endif
	sprintf(m_HDFileDir, "%s/../../../%s", m_RunInfDir, iniFile.INI_GetValue(sectionName, "HDFileDir", "var/pmu_offline_file/comtr_file").c_str());
	para_SaveResFile = iniFile.INI_GetValueI(sectionName, "SaveResfile", 0);
	sprintf(para_FtpServer, "%s", iniFile.INI_GetValue(sectionName, "FtpServer").c_str());
	sprintf(para_FtpUser, "%s", iniFile.INI_GetValue(sectionName, "FtpUser").c_str());
	sprintf(para_FtpPsw, "%s", iniFile.INI_GetValue(sectionName, "FtpPsw").c_str());
	sprintf(para_ResFileName, "%s", iniFile.INI_GetValue(sectionName, "ResFileName").c_str());
	sprintf(para_FtpDir, "%s", iniFile.INI_GetValue(sectionName, "FtpDir").c_str());
	sprintf(para_FtpCmd, "%s", iniFile.INI_GetValue(sectionName, "FtpCmd").c_str());

	sectionName = "������ʾ";
	m_acdot_id = iniFile.INI_GetValueI(sectionName, "acdot_id");
	m_isForceSet = iniFile.INI_GetValueI(sectionName, "isForceSet");
	m_filNo = iniFile.INI_GetValueI(sectionName, "filNo");
	m_colNo = iniFile.INI_GetValueI(sectionName, "colNo");
	m_DispCtl.dsource = iniFile.INI_GetValueI(sectionName, "DataSource", 0);
	sTempStr = iniFile.INI_GetValue(sectionName, "nGpsTime");
	TempInt = PT_GetSecondByTimeString(sTempStr.c_str());
	if (TempInt < 0)
	{
		sprintf(m_LogString, "ʱ��ת��ʧ��[%s]", sTempStr.c_str());
		MessShow(m_LogString);
	}
	else
	{
		para_nGpsTime = TempInt;
	}

	//  �޸�Ϊ����·��
	sprintf(m_DataFile, "%s", iniFile.INI_GetValue(sectionName, "DataFile").c_str());
	sprintf(m_DataFile1, "%s", iniFile.INI_GetValue(sectionName, "DataFile1").c_str());
	sprintf(m_DataFile2, "%s", iniFile.INI_GetValue(sectionName, "DataFile2").c_str());
	sprintf(m_DataFile3, "%s", iniFile.INI_GetValue(sectionName, "DataFile3").c_str());
	sprintf(m_DataFile4, "%s", iniFile.INI_GetValue(sectionName, "DataFile4").c_str());

	// #ifdef _WINDOWS32
	//     sprintf(m_DataFile,"%s\\%s",m_DataDir, iniFile.INI_GetValue(sectionName,"DataFile").c_str());
	//     sprintf(m_DataFile1,"%s\\%s",m_DataDir,iniFile.INI_GetValue(sectionName,"DataFile1").c_str());
	//     sprintf(m_DataFile2,"%s\\%s",m_DataDir,iniFile.INI_GetValue(sectionName,"DataFile2").c_str());
	//     sprintf(m_DataFile3,"%s\\%s",m_DataDir,iniFile.INI_GetValue(sectionName,"DataFile3").c_str());
	//     sprintf(m_DataFile4,"%s\\%s",m_DataDir,iniFile.INI_GetValue(sectionName,"DataFile4").c_str());
	// #else
	//     sprintf(m_DataFile,"%s/%s",m_DataDir, iniFile.INI_GetValue(sectionName,"DataFile").c_str());
	//     sprintf(m_DataFile1,"%s/%s",m_DataDir,iniFile.INI_GetValue(sectionName,"DataFile1").c_str());
	//     sprintf(m_DataFile2,"%s/%s",m_DataDir,iniFile.INI_GetValue(sectionName,"DataFile2").c_str());
	//     sprintf(m_DataFile3,"%s/%s",m_DataDir,iniFile.INI_GetValue(sectionName,"DataFile3").c_str());
	//     sprintf(m_DataFile4,"%s/%s",m_DataDir,iniFile.INI_GetValue(sectionName,"DataFile4").c_str());
	// #endif

	if (m_DispCtl.dsource == MENU_LFO_DATA_FILE)
	{
		sprintf(m_DataFileDir, "%s", m_DataFile);
	}
	else if (m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE)
	{
		sprintf(m_DataFileDir, "%s", m_DataFile1);
	}
	else if (m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE2)
	{
		sprintf(m_DataFileDir, "%s", m_DataFile2);
	}
	else if (m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE3)
	{
		sprintf(m_DataFileDir, "%s", m_DataFile3);
	}
	else if (m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE4)
	{
		sprintf(m_DataFileDir, "%s", m_DataFile4);
	}

#ifdef _LFO_ASST_DECI_
	ReadAsstIni();
#endif
	return 1;
}
// 2008-06-10 by zhw 
// �����ļ�����
void CLfoAnaObj::WriteDataToFile(int nType, int realNum, float t_start, float t_end, float t_delta, int nOffSec)
{
	//     if( para_SaveDataToFile == 0 )
	//     {
	//         return;
	//     }
	// 
	//     FILE	*data_fp;
	//     char	datafilename[MAXBUF];
	// 
	//     bool	b_fileopenflag=false;
	//     int     ii,jj;
	// 
	//     int     nOffPos  = 0;
	// 	char    strTime1[28], strTime2[28];
	// 
	// #ifdef _WINDOWS32
	//     if( nType == DATA_TYPE_FAC_VOLT )
	//     {
	//         sprintf(datafilename,"%s\\%s.dat",m_SaveFileDir, m_FacVoltFileName.c_str());
	//     }
	//     else if( nType == DATA_TYPE_FAC_ANG )
	//     {
	//         sprintf(datafilename,"%s\\%s.dat",m_SaveFileDir, m_FacAngFileName.c_str());
	//     }
	//     else if( nType == DATA_TYPE_GEN_POW )
	//     {
	//         sprintf(datafilename,"%s\\%s.dat",m_SaveFileDir, m_GnPowFileName.c_str() );
	//     }
	//     else if( nType == DATA_TYPE_GEN_ANG )
	//     {
	//         sprintf(datafilename,"%s\\%s.dat",m_SaveFileDir, m_GnAngFileName.c_str() );
	//     }
	//     else if( nType == DATA_TYPE_ACLN_POW )
	//     {
	//         sprintf(datafilename,"%s\\%s.dat",m_SaveFileDir, m_LnPowFileName.c_str() );
	//     }
	//     else
	//     {
	//         sprintf(m_LogString,"WriteDataToFile():: �������ʹ��� DATA_TYPE= %d", nType );
	//         MessShow(m_LogString);
	//         return;
	//     }
	// #else
	//     if( nType == DATA_TYPE_FAC_VOLT )
	//     {
	//         sprintf(datafilename,"%s/%s.dat",m_SaveFileDir, m_FacVoltFileName.c_str());
	//     }
	//     else if( nType == DATA_TYPE_FAC_ANG )
	//     {
	//         sprintf(datafilename,"%s/%s.dat",m_SaveFileDir, m_FacAngFileName.c_str());
	//     }
	//     else if( nType == DATA_TYPE_GEN_POW )
	//     {
	//         sprintf(datafilename,"%s/%s.dat",m_SaveFileDir, m_GnPowFileName.c_str() );
	//     }
	//     else if( nType == DATA_TYPE_GEN_ANG )
	//     {
	//         sprintf(datafilename,"%s/%s.dat",m_SaveFileDir, m_GnAngFileName.c_str() );
	//     }
	//     else if( nType == DATA_TYPE_ACLN_POW )
	//     {
	//         sprintf(datafilename,"%s/%s.dat",m_SaveFileDir, m_LnPowFileName.c_str() );
	//     }
	//     else
	//     {
	//         sprintf(m_LogString,"WriteDataToFile():: �������ʹ��� DATA_TYPE= %d", nType );
	//         MessShow(m_LogString);
	//         return;
	//     }
	// #endif
	//     
	//     //�򿪴��ڵ������ļ�
	//     if( ( data_fp=fopen(datafilename,"r+") ) == NULL )
	//     {
	//         ////��������ļ������ڣ������´���
	//         if( ( data_fp=fopen(datafilename,"w+") ) == NULL )
	//         {
	//             sprintf(m_LogString,"File[%s] Creating happened error\n",datafilename);
	//             MessShow(m_LogString);
	//         }
	//         else
	//         {
	//             b_fileopenflag=true;
	//         }
	//     }
	//     else
	//     {
	//         b_fileopenflag=true;
	//     }
	//     
	//     if(b_fileopenflag)
	//     {
	//         //�ƶ��ļ�ָ�뵽ĩβ
	//         fseek(data_fp,0,2);
	//         //д��������Ϣ
	//         nOffPos  = nOffSec * MAX_FRA_PER_SEC ;
	//         sprintf( strTime1, "%s", PT_GetTimeStringBySeconds(t_start));
	//         sprintf( strTime2, "%s", PT_GetTimeStringBySeconds(t_end));
	// 
	//         if( nType == DATA_TYPE_FAC_VOLT )
	//         {
	//             fprintf(data_fp,"%s\t%s\t%d\t%d\t%9.2f\t%d\t%d\n",strTime1,strTime2,t_start,t_end,t_delta,m_nFcNum,realNum);
	//             for( ii=0;ii<m_nFcNum;ii++)  //  ��վID��
	//             {
	//                 fprintf(data_fp,""INT_REC_FMT"\t",m_pFcDat[ii].fac_id);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<m_nFcNum;ii++)  //  ��վ����
	//             {
	//                 fprintf(data_fp,"%s\t",m_pFcInf[ii].fac_name);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<realNum;ii++)
	//             {
	//                 for(jj=0;jj<m_nFcNum;jj++)
	//                 {
	//                     fprintf(data_fp,"%9.2f\t",m_pFcDat[jj].u1v[nOffPos+ii]);  //  ��վ�ߵ�ѹ
	//                 }
	//                 fprintf(data_fp,"\n");
	//             }
	//         }
	//         else if( nType == DATA_TYPE_FAC_ANG )
	//         {
	//             fprintf(data_fp,"%s\t%s\t%d\t%d\t%9.2f\t%d\t%d\n",strTime1,strTime2,t_start,t_end,t_delta,m_nFcNum,realNum);
	//             for( ii=0;ii<m_nFcNum;ii++)  //  ��վID��
	//             {
	//                 fprintf(data_fp,""INT_REC_FMT"\t",m_pFcDat[ii].fac_id);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<m_nFcNum;ii++)  //  ��վ����
	//             {
	//                 fprintf(data_fp,"%s\t",m_pFcInf[ii].fac_name);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	//             
	//             for( ii=0;ii<realNum;ii++)
	//             {
	//                 for(jj=0;jj<m_nFcNum;jj++)
	//                 {
	//                     fprintf(data_fp,"%9.2f\t",m_pFcDat[jj].u1a[nOffPos+ii]);  //  ��վ��ѹ���
	//                 }
	//                 fprintf(data_fp,"\n");
	//             }
	//         }
	//         else if( nType == DATA_TYPE_GEN_POW )
	//         {
	//             fprintf(data_fp,"%s\t%s\t%d\t%d\t%9.2f\t%d\t%d\n",strTime1,strTime2,t_start,t_end,t_delta,m_nGnNum,realNum);
	//             for( ii=0;ii<m_nGnNum;ii++)   //  �����ID��
	//             {
	//                 fprintf(data_fp,""INT_REC_FMT"\t",m_pGnDat[ii].gn_id);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<m_nGnNum;ii++)  //  ���������
	//             {
	//                 fprintf(data_fp,"%s\t",m_pGnInf[ii].gn_name);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<m_nGnNum;ii++)  //  �������������
	//             {
	//                 fprintf(data_fp,"%s\t",m_pGnInf[ii].area_name);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<m_nGnNum;ii++)  //  ���������ʱ�䳣��
	//             {
	//                 fprintf(data_fp,"%9.2f\t",m_pGnInf[ii].tj);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<realNum;ii++)
	//             {
	//                 for(jj=0;jj<m_nGnNum;jj++)
	//                 {
	//                     fprintf(data_fp,"%9.2f\t",m_pGnDat[jj].pwr[nOffPos+ii]); //  ������й�
	//                 }
	//                 fprintf(data_fp,"\n");
	//             }
	//         }
	//         else if( nType == DATA_TYPE_GEN_ANG )
	//         {
	//             fprintf(data_fp,"%s\t%s\t%d\t%d\t%9.2f\t%d\t%d\n",strTime1,strTime2,t_start,t_end,t_delta,m_nGnNum,realNum);
	//             for( ii=0;ii<m_nGnNum;ii++)
	//             {
	//                 fprintf(data_fp,""INT_REC_FMT"\t",m_pGnDat[ii].gn_id);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<m_nGnNum;ii++)  //  ���������
	//             {
	//                 fprintf(data_fp,"%s\t",m_pGnInf[ii].gn_name);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	//             
	//             for( ii=0;ii<m_nGnNum;ii++)  //  �������������
	//             {
	//                 fprintf(data_fp,"%s\t",m_pGnInf[ii].area_name);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<m_nGnNum;ii++)  //  ���������ʱ�䳣��
	//             {
	//                 fprintf(data_fp,"%9.2f\t",m_pGnInf[ii].tj);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<realNum;ii++)
	//             {
	//                 for(jj=0;jj<m_nGnNum;jj++)
	//                 {
	//                     fprintf(data_fp,"%9.2f\t",m_pGnDat[jj].ang[nOffPos+ii]);  // ��������
	//                 }
	//                 fprintf(data_fp,"\n");
	//             }
	//         }
	//         else if( nType == DATA_TYPE_ACLN_POW )
	//         {
	//             fprintf(data_fp,"%s\t%s\t%d\t%d\t%9.2f\t%d\t%d\n",strTime1,strTime2,t_start,t_end,t_delta,m_nLnNum,realNum);
	//             for( ii=0;ii<m_nLnNum;ii++)  //  ��·ID��
	//             {
	//                 fprintf(data_fp,""INT_REC_FMT"\t",m_pLnDat[ii].acln_id);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<m_nLnNum;ii++)  //  ��·����
	//             {
	//                 fprintf(data_fp,"%s\t",m_pLnInf[ii].acln_name);
	//             }
	//             
	//             fprintf(data_fp,"\n");
	// 
	//             for( ii=0;ii<realNum;ii++)
	//             {
	//                 for(jj=0;jj<m_nLnNum;jj++)
	//                 {
	//                     fprintf(data_fp,"%9.2f\t",m_pLnDat[jj].pwr[nOffPos+ii]);  //  ��·����
	//                 }
	//                 fprintf(data_fp,"\n");
	//             }
	//         }
	// 
	//         fclose(data_fp);
	//     }
	//     
	//     int rtn=CheckDataFile(datafilename);
	//     //�ж��ļ���С
	//     /*struct stat data_stat;
	//     stat(datafilename,&data_stat); // ��ȡ�Ѵ��ļ���Ϣ
	//     if( data_stat.st_size > DATAFILEMAXLENGTH )//1024*1024*10)
	//     {
	//         char newfilename[FILENAMELEN];
	//         memset(newfilename,NULL,FILENAMELEN);
	//         
	//         char NewAdd_FileName[FILENAMELEN];
	//         char FileIndexName[FILENAMELEN];
	//         char FileDir[FILENAMELEN];
	//         
	//         sprintf(FileDir,"%s",m_SaveFileDir);
	// 
	//         if( nType == DATA_TYPE_FAC_VOLT )
	//         {
	//             sprintf(NewAdd_FileName,"%s", m_FacVoltFileName.c_str());
	//             sprintf(FileIndexName,"%s.idx", m_FacVoltFileName.c_str());
	//         }
	//         else if( nType == DATA_TYPE_FAC_ANG )
	//         {
	//             sprintf(NewAdd_FileName,"%s", m_FacAngFileName.c_str());
	//             sprintf(FileIndexName,"%s.idx", m_FacAngFileName.c_str());
	//         }
	//         else if( nType == DATA_TYPE_GEN_POW )
	//         {
	//             sprintf(NewAdd_FileName,"%s", m_GnPowFileName.c_str());
	//             sprintf(FileIndexName,"%s.idx", m_GnPowFileName.c_str());
	//         }
	//         else if( nType == DATA_TYPE_GEN_ANG )
	//         {
	//             sprintf(NewAdd_FileName,"%s", m_GnAngFileName.c_str());
	//             sprintf(FileIndexName,"%s.idx", m_GnAngFileName.c_str());
	//         }
	//         else if( nType == DATA_TYPE_ACLN_POW )
	//         {
	//             sprintf(NewAdd_FileName,"%s", m_LnPowFileName.c_str());
	//             sprintf(FileIndexName,"%s.idx", m_LnPowFileName.c_str());
	//         }
	//        
	//         int rtn=CheckDataFile(NewAdd_FileName,FileIndexName,FileDir);
	//         
	//     }*/

	return;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : �����ļ�����
//  ������� : �ж�Ŀǰ�����ļ���,�����������para_DataSaveNum,ɾ���������־
//  ��    ע :
//  �� �� ֵ : �ɹ�����0  ʧ�ܷ���ֵС��0
//  ����˵�� : NewAdd_FileName               ���������ļ���
//             FileIndexName                 �����ļ���
//             FileDir                       �����ļ�����Ŀ¼
////////////////////////////////////////////////////////////////////////////////
int CLfoAnaObj::CheckDataFile(const char* fileName)
{
	struct stat data_stat;
	stat(fileName, &data_stat); // ��ȡ�Ѵ��ļ���Ϣ
	if (data_stat.st_size <= DATAFILEMAXLENGTH)//1024*1024*10)
	{
		return 0;
	}

	//NewAdd_FileName,char* FileIndexName,char * FileDir)
	string fileNameSrc;
	string SrcFileDir;
	string SrcFileName;
	string SrcFileSuffix;
	string::size_type tmpIdx;
	char backslash[3];

#ifdef _WINDOWS32
	sprintf(backslash, "%s", "\\");
#else
	sprintf(backslash, "%s", "/");
#endif

	fileNameSrc = fileName;
	// get file Info
	tmpIdx = fileNameSrc.find_last_of(backslash);
	SrcFileDir = fileNameSrc.substr(0, tmpIdx + 1);
	fileNameSrc.erase(0, tmpIdx + 1);
	tmpIdx = fileNameSrc.find_last_of(".");
	SrcFileName = fileNameSrc.substr(0, tmpIdx);
	SrcFileSuffix = fileNameSrc.substr(tmpIdx);

	string NewAddFileName;
	time_t ltime;

	time(&ltime);

	NewAddFileName = SrcFileDir;
	NewAddFileName.append(SrcFileName);
	NewAddFileName.append("_");
	NewAddFileName.append(PT_GetNoSpaceMarkTimeStringBySeconds(ltime));
	NewAddFileName.append(SrcFileSuffix);
	if (rename(fileName, NewAddFileName.c_str()) != 0)
	{
		sprintf(m_LogString, "�������ļ�ʧ��: %s - %s", fileName, NewAddFileName.c_str());
		MessShow(m_LogString);
		return -1;
	}

	char    indexfilename_Abs[MAXBUF];
	char    filedirname[MAXBUF];
	char    indexfilename[MAXBUF];
	char    newsavefilename[MAXBUF];
	sprintf(indexfilename_Abs, "%s%s.idx", SrcFileDir.c_str(), SrcFileName.c_str());
	sprintf(filedirname, "%s", SrcFileDir.c_str());
	sprintf(indexfilename, "%s.idx", SrcFileName.c_str());
	tmpIdx = NewAddFileName.find_last_of(backslash);
	sprintf(newsavefilename, "%s", NewAddFileName.substr(tmpIdx + 1).c_str());

	//�ж������ļ��Ƿ����	
	bool	b_IndexFileExistFlag = false;
	char	readbuf[MAXBUF];
	char	writebuf[MAXBUF];
	int     logfilecount = 0;
	int     acs_file_sts;

#ifdef _WINDOWS32
	acs_file_sts = 0;
#else
	acs_file_sts = F_OK;
#endif
	if (0 == access(indexfilename_Abs, acs_file_sts))
	{
		b_IndexFileExistFlag = true;
	}
	else
	{
		b_IndexFileExistFlag = false;
		sprintf(m_LogString, "CheckDataFile():File[%s] isn't existed!\n", indexfilename);
		MessShow(m_LogString);
	}

	FILE* Index_fp, * tmp_index_fp, * new_index_fp;
	char	tmpindexfilename_Abs[MAXBUF];
	char	newindexfilename_Abs[MAXBUF];
	char	deletefilename_Abs[MAXBUF];
	char	findfilename_Abs[MAXBUF];

	bool	bAddFlag = true;//�ļ����ӵ������ļ���ʶ
	int	    writefilecount = 0;
	string  s_readstr;
	if (b_IndexFileExistFlag)
	{
		if ((Index_fp = fopen(indexfilename_Abs, "r+")) == NULL)
		{
			sprintf(m_LogString, "CheckDataFile():File[%s] opening happened error!\n", indexfilename_Abs);
			MessShow(m_LogString);
			return -10;
		}
		sprintf(tmpindexfilename_Abs, "%s%s%s.tmp", filedirname, backslash, indexfilename);
		if ((tmp_index_fp = fopen(tmpindexfilename_Abs, "w")) == NULL)
		{
			fclose(Index_fp);
			sprintf(m_LogString, "CheckDataFile():File[%s] opening happened error!\n", tmpindexfilename_Abs);
			MessShow(m_LogString);
			return -20;
		}

		//��ȡ��¼��־�ļ�����
		while (1)
		{
			memset(readbuf, 0, MAXBUF);
			s_readstr.erase();
			if (feof(Index_fp)) break;
			fgets(readbuf, MAXBUF, Index_fp);
			s_readstr = TrimLeft(readbuf);
			if (s_readstr.length() > 0)
			{
				memset(readbuf, 0, MAXBUF);
				memcpy(readbuf, s_readstr.c_str(), s_readstr.length() - 1);//ȥ����ֹ��
				s_readstr = readbuf;
				logfilecount = atoi(readbuf);
				break;
			}
		}

		if (logfilecount >= para_DataSaveNum)//����������ļ��Ѵﵽ�����Ŀpara_DataSaveNum��ɾ���������ļ� 2008-05-21 by zhw
		{
			int del_pos = 0;//delete fail filecount;
			for (int i = 0; i < logfilecount; i++)
			{
				while (1)
				{
					memset(readbuf, 0, MAXBUF);
					s_readstr.erase();
					fgets(readbuf, MAXBUF, Index_fp);
					s_readstr = TrimLeft(readbuf);
					if (feof(Index_fp)) break;
					if (s_readstr.length() > 0)
					{
						memset(readbuf, 0, MAXBUF);
						memcpy(readbuf, s_readstr.c_str(), s_readstr.length() - 1);//ȥ����ֹ��
						s_readstr = readbuf;
						break;
					}
				}

				if (s_readstr.length() > 0)
				{
					if (i < (logfilecount - (para_DataSaveNum - 1)))  //2008-05-21 by zhw
					{
						sprintf(deletefilename_Abs, "%s%s%s", filedirname, backslash, s_readstr.c_str());
						//delete the filename
						if (0 != remove(deletefilename_Abs))
						{
							del_pos++;
						}
						else
						{
							sprintf(m_LogString, "CheckDataFile():[%s] deleted error!\n", deletefilename_Abs);
							MessShow(m_LogString);
						}
					}
					else
					{
						if (0 == strcmp(s_readstr.c_str(), newsavefilename))
						{
							bAddFlag = false;
						}
						sprintf(writebuf, "%s\n", s_readstr.c_str());
						fputs(writebuf, tmp_index_fp);
						writefilecount++;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < logfilecount; i++)
			{
				while (1)
				{
					memset(readbuf, 0, MAXBUF);
					s_readstr.erase();
					fgets(readbuf, MAXBUF, Index_fp);
					s_readstr = TrimLeft(readbuf);

					if (feof(Index_fp)) break;
					if (s_readstr.length() > 0)
					{
						memset(readbuf, 0, MAXBUF);
						memcpy(readbuf, s_readstr.c_str(), s_readstr.length() - 1);//ȥ����ֹ��
						s_readstr = readbuf;
						break;
					}
				}

				if (s_readstr.length() > 0)
				{
					if (0 == strcmp(s_readstr.c_str(), newsavefilename))
					{
						bAddFlag = false;
						printf("CheckDataFile():File[%s] is existed!\n", newsavefilename);
					}

					//�ж��ļ��Ƿ����
					sprintf(findfilename_Abs, "%s%s%s", filedirname, backslash, s_readstr.c_str());
					//if( 0 == access( findfilename_Abs,F_OK ) )
#ifdef _WINDOWS32
					acs_file_sts = 0;
#else
					acs_file_sts = F_OK;
#endif
					if (0 == access(findfilename_Abs, acs_file_sts))
					{
						sprintf(writebuf, "%s\n", s_readstr.c_str());
						fputs(writebuf, tmp_index_fp);
						writefilecount++;
						//printf("%sTEMPTEMPTEMPTEMP\n\n\n\n\n",writebuf);
						//fflush(stdout);
					}
					else
					{
						printf("CheckDataFile():File[%s] is not existed!\n", s_readstr.c_str());
					}
				}
			}
		}
		logfilecount = writefilecount;

		if (bAddFlag)
		{
			logfilecount++;
		}
		fclose(Index_fp);
		fclose(tmp_index_fp);

		sprintf(tmpindexfilename_Abs, "%s%s%s.tmp", filedirname, backslash, indexfilename);
		if ((tmp_index_fp = fopen(tmpindexfilename_Abs, "r+")) == NULL)
		{
			printf("CheckDataFile():File[%s] opening happened error!\n", indexfilename_Abs);
			return -30;
		}
		sprintf(newindexfilename_Abs, "%s%s%s.new", filedirname, backslash, indexfilename);
		if ((new_index_fp = fopen(newindexfilename_Abs, "w")) == NULL)
		{
			fclose(tmp_index_fp);
			printf("CheckDataFile():File[%s] creating happened error!\n", newindexfilename_Abs);
			return -40;
		}
		//д���¼�ļ�����
		sprintf(writebuf, "%d\n", logfilecount);
		fputs(writebuf, new_index_fp);
		//д������¼�ļ�����
		while (1)
		{
			memset(readbuf, 0, MAXBUF);
			s_readstr.erase();
			fgets(readbuf, MAXBUF, tmp_index_fp);
			s_readstr = TrimLeft(readbuf);
			if (feof(tmp_index_fp))
			{
				//fputs("\n",new_index_fp);						
				break;
			}
			sprintf(writebuf, "%s", s_readstr.c_str());
			fputs(writebuf, new_index_fp);
		}
		if (bAddFlag)
		{
			sprintf(writebuf, "%s\n", newsavefilename);
			fputs(writebuf, new_index_fp);
		}
		fclose(tmp_index_fp);
		fclose(new_index_fp);

		//return 0;
		if (0 == remove(indexfilename_Abs))
		{
			sprintf(m_LogString, "CheckDataFile():[%s] deleted error!", indexfilename_Abs);
			MessShow(m_LogString);
		}
		if (0 == remove(tmpindexfilename_Abs))
		{
			sprintf(m_LogString, "CheckDataFile():[%s] deleted error!", tmpindexfilename_Abs);
			MessShow(m_LogString);
		}
		//������ת���ļ�
		rename(newindexfilename_Abs, indexfilename_Abs);
	}//indexfile is exist
	else
	{
		if ((Index_fp = fopen(indexfilename_Abs, "w+")) == NULL)
		{
			printf("CheckDataFile():File[%s] creating happened error!\n", indexfilename_Abs);
			return -50;
		}
		logfilecount++;
		//д���¼�ļ�����
		sprintf(writebuf, "%d\n", logfilecount);
		fputs(writebuf, Index_fp);

		//д���¼����ļ�����		
		sprintf(writebuf, "%s\n", newsavefilename);
		fputs(writebuf, Index_fp);

		fclose(Index_fp);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ȥ���ַ�����˵Ŀո�
//  ������� : 
//  ��    ע :
//  �� �� ֵ : �������ַ���
//  ����˵�� : str               �账����ַ���
////////////////////////////////////////////////////////////////////////////////
char* CLfoAnaObj::TrimLeft(char* str)//ȥ���ַ�����˵Ŀո�
{
	char* str_begin = str;
	char* str_end = str + strlen(str);

	while (str_begin < str_end)
	{
		if (NULL == strchr(" \t", *str_begin))
			break;
		str_begin++;
	}

	memmove(str, str_begin, strlen(str_begin) + 1);
	return str;
}

void CLfoAnaObj::MessShow(const char* logstr, const int logtype)
{
	g_PubLogMessShow(logstr, logtype);
	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ����ȡ��ŵ�д��log file
//  ������� :
//  ��    ע :
//  �� �� ֵ : �ɹ�����true
////////////////////////////////////////////////////////////////////////////////
// void CLfoAnaObj::WriteColIDtoLogFile()
// {
// #ifdef _WAMAP_
//     char  logStr[MAXBUF];    
// 
//     sprintf(logStr, "%s", "****    IDB_InitColID()    ****" ); WriteLogFile(logStr);
// 
//     sprintf(logStr,"COL_ID_YC_FHZ = %3d", COL_ID_YC_FHZ); WriteLogFile(logStr);
// 
//     sprintf(logStr,"COL_ID_GN_IAV = %3d  COL_ID_GN_IAA = %3d", COL_ID_GN_IAV, COL_ID_GN_IAA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_GN_IBV = %3d  COL_ID_GN_IBA = %3d", COL_ID_GN_IBV, COL_ID_GN_IBA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_GN_ICV = %3d  COL_ID_GN_ICA = %3d", COL_ID_GN_ICV, COL_ID_GN_ICA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_GN_UAV = %3d  COL_ID_GN_UAA = %3d", COL_ID_GN_UAV, COL_ID_GN_UAA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_GN_UBV = %3d  COL_ID_GN_UBA = %3d", COL_ID_GN_UBV, COL_ID_GN_UBA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_GN_UCV = %3d  COL_ID_GN_UCA = %3d", COL_ID_GN_UCV, COL_ID_GN_UCA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_GN_PWR = %3d  COL_ID_GN_QWR = %3d", COL_ID_GN_PWR, COL_ID_GN_QWR); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_GN_ANG = %3d  COL_ID_GN_ANC = %3d", COL_ID_GN_ANG, COL_ID_GN_ANC); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_GN_EEV = %3d  COL_ID_GN_EEA = %3d", COL_ID_GN_EEV, COL_ID_GN_EEA); WriteLogFile(logStr);
// 
//     sprintf(logStr,"COL_ID_LN_IAV = %3d  COL_ID_LN_IAA = %3d", COL_ID_LN_IAV, COL_ID_LN_IAA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_LN_IBV = %3d  COL_ID_LN_IBA = %3d", COL_ID_LN_IBV, COL_ID_LN_IBA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_LN_ICV = %3d  COL_ID_LN_ICA = %3d", COL_ID_LN_ICV, COL_ID_LN_ICA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_LN_UAV = %3d  COL_ID_LN_UAA = %3d", COL_ID_LN_UAV, COL_ID_LN_UAA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_LN_UBV = %3d  COL_ID_LN_UBA = %3d", COL_ID_LN_UBV, COL_ID_LN_UBA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_LN_UCV = %3d  COL_ID_LN_UCA = %3d", COL_ID_LN_UCV, COL_ID_LN_UCA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_LN_PWR = %3d  COL_ID_LN_QWR = %3d", COL_ID_LN_PWR, COL_ID_LN_QWR); WriteLogFile(logStr);
// 
//     sprintf(logStr,"COL_ID_BS_UAV = %3d  COL_ID_BS_UAA = %3d", COL_ID_BS_UAV, COL_ID_BS_UAA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_BS_UBV = %3d  COL_ID_BS_UBA = %3d", COL_ID_BS_UBV, COL_ID_BS_UBA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_BS_UCV = %3d  COL_ID_BS_UCA = %3d", COL_ID_BS_UCV, COL_ID_BS_UCA); WriteLogFile(logStr);
// 
//     sprintf(logStr,"COL_ID_TR_IAV = %3d  COL_ID_TR_IAA = %3d\n", COL_ID_TR_IAV, COL_ID_TR_IAA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_TR_IBV = %3d  COL_ID_TR_IBA = %3d\n", COL_ID_TR_IBV, COL_ID_TR_IBA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_TR_ICV = %3d  COL_ID_TR_ICA = %3d\n", COL_ID_TR_ICV, COL_ID_TR_ICA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_TR_UAV = %3d  COL_ID_TR_UAA = %3d\n", COL_ID_TR_UAV, COL_ID_TR_UAA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_TR_UBV = %3d  COL_ID_TR_UBA = %3d\n", COL_ID_TR_UBV, COL_ID_TR_UBA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_TR_UCV = %3d  COL_ID_TR_UCA = %3d\n", COL_ID_TR_UCV, COL_ID_TR_UCA); WriteLogFile(logStr);
//     sprintf(logStr,"COL_ID_TR_PWR = %3d  COL_ID_TR_QWR = %3d\n", COL_ID_TR_PWR, COL_ID_TR_QWR); WriteLogFile(logStr);
// 
//     sprintf(logStr,"COL_ID_BS_VAC = %3d", COL_ID_BS_VAC); WriteLogFile(logStr);
// #endif
// 	return ;
// }

//////////////////////////////// ��Ϣ�㼯 /////////////////////////////////
//  ��ȡ������Ϣ
bool CLfoAnaObj::InitPrject()
{
#ifdef _WAMAP_
	// ��վ��Ϣ��
	vector<FAC_TAB_CRE> VecFacTab;
	PODB_GetFacInfo(VecFacTab);
	// ���������
	vector<AREA_TAB_CRE> VecAreaTab;
	PODB_GetAreaInfo(VecAreaTab);
	// WAMSǰ��ң�ⶨ���
	vector<FES_YC_TAB_CRE> VecFesYcTab;
	PODB_GetFesYcInfo(VecFesYcTab);
	// WAMSǰ��ң�Ŷ����
	vector<FES_YX_TAB_CRE> VecFesYxTab;
	PODB_GetFesYxInfo(VecFesYxTab);
	vector<FES_YX_YC> VecFesYxYc;
	PODB_YxYc(VecFesYcTab, VecFesYxTab, VecFesYxYc);
	// WAMSǰ��ͨ�ų�վ��
	PODB_GetFesCOMMFACInfo(m_vCOMMFc);

	// SSO��վ��,ĸ�߱�
	InitFc(VecFacTab, VecAreaTab, VecFesYcTab);
	// SSO������·��,�����߶α�,�����߶ζ˵��
	InitLn(VecFacTab, VecAreaTab, VecFesYxYc);
	// SSO�������,�������
	InitGn(VecFacTab, VecAreaTab, VecFesYxYc);
	// SSO��ѹ����,��ѹ����,��ѹ�������
	InitTr(VecFacTab, VecAreaTab, VecFesYxYc);
#endif
	return true;
}

/*
int CLfoAnaObj::GetFacInfo(vector<FAC_TAB_CRE> &VecFacTab)
{
	if (!VecFacTab.empty())
	{
		VecFacTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(FAC_INFO_NO_TAB,FAC_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(FAC_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"��վ��Ϣ���¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"��վ��Ϣ��[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	FAC_TAB_CRE *pInfo= NULL;
	pInfo = new FAC_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(FAC_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecFacTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaObj::GetAreaInfo(vector<AREA_TAB_CRE> &VecAreaTab)
{
	if (!VecAreaTab.empty())
	{
		VecAreaTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(DIV_INFO_NO_TAB,AREA_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(AREA_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"����������¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"���������[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	AREA_TAB_CRE *pInfo= NULL;
	pInfo = new AREA_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(AREA_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecAreaTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaObj::GetVLTypeInfo(vector<VL_TYPE_TAB_CRE> &VecVLTypeTab)
{
	if (!VecVLTypeTab.empty())
	{
		VecVLTypeTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(VL_TYPE_NO_TAB,VL_TYPE_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(VL_TYPE_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"��ѹ�ȼ����¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"��ѹ�ȼ���[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	VL_TYPE_TAB_CRE *pInfo= NULL;
	pInfo = new VL_TYPE_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(VL_TYPE_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecVLTypeTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaObj::GetFesYcInfo(vector<FES_YC_TAB_CRE> &VecFesYcTab)
{
	if (!VecFesYcTab.empty())
	{
		VecFesYcTab.clear();
	}
#ifdef _WAMAP_
#ifdef _D5000

#else
	CBuffer buf_base;
	if( PODB_TableGet(WAM_FES_YC_DEFINE_NO_TAB,FES_YC_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(FES_YC_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"WAMSǰ��ң�ⶨ����¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"WAMSǰ��ң�ⶨ���[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	FES_YC_TAB_CRE *pInfo= NULL;
	pInfo = new FES_YC_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(FES_YC_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecFesYcTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
#endif
	return 0;
}

int CLfoAnaObj::GetBusInfo(vector<BUS_TAB_CRE> &VecBusTab)
{
	if (!VecBusTab.empty())
	{
		VecBusTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(BS_DEVICE_NO_TAB,BUS_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(BUS_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"ĸ�߱��¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"ĸ�߱�[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	BUS_TAB_CRE *pInfo= NULL;
	pInfo = new BUS_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(BUS_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecBusTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CLfoAnaObj::GetAclnInfo(vector<ACLINE_TAB_CRE> &VecAclnTab)
{
	if (!VecAclnTab.empty())
	{
		VecAclnTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(ACLN_DEVICE_NO_TAB,ACLINE_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(ACLINE_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"�����߶α��¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"�����߶α�[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	ACLINE_TAB_CRE *pInfo= NULL;
	pInfo = new ACLINE_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(ACLINE_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecAclnTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CLfoAnaObj::GetAclnDotInfo(vector<ACLNDOT_TAB_CRE> &VecAclnDotTab)
{
	if (!VecAclnDotTab.empty())
	{
		VecAclnDotTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(ACLN_DOT_NO_TAB,ACLNDOT_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(ACLNDOT_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"�����߶ζ˵���¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"�����߶ζ˵��[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	ACLNDOT_TAB_CRE *pInfo= NULL;
	pInfo = new ACLNDOT_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(ACLNDOT_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecAclnDotTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CLfoAnaObj::GetGenInfo(vector<GEN_TAB_CRE> &VecGenTab)
{
	if (!VecGenTab.empty())
	{
		VecGenTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(GN_DEVICE_NO_TAB,GEN_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(GEN_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"��������¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"�������[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	GEN_TAB_CRE *pInfo= NULL;
	pInfo = new GEN_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(GEN_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecGenTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CLfoAnaObj::GetTrInfo(vector<TR_TAB_CRE> &VecTrTab)
{
	if (!VecTrTab.empty())
	{
		VecTrTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(TR_DEVICE_NO_TAB,TR_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(TR_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"��ѹ�����¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"��ѹ����[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	TR_TAB_CRE *pInfo= NULL;
	pInfo = new TR_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(TR_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecTrTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CLfoAnaObj::GetTrwdInfo(vector<TRWD_TAB_CRE> &VecTrwdTab)
{
	if (!VecTrwdTab.empty())
	{
		VecTrwdTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if( PODB_TableGet(TRWD_DEVICE_NO_TAB,TRWD_TAB_CRE_FLD,buf_base) < 0 )
	{
		return -1;
	}

	int nInfo = buf_base.GetLength()/sizeof(TRWD_TAB_CRE);
	if ( nInfo <=0 )
	{
		sprintf(m_LogString,"��ѹ��������¼Ϊ��[%d]",nInfo);
		MessShow(m_LogString,LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString,"��ѹ�������[%d]",nInfo);
		MessShow(m_LogString,LTDEBUG);
	}

	TRWD_TAB_CRE *pInfo= NULL;
	pInfo = new TRWD_TAB_CRE[nInfo];
	memcpy(pInfo,buf_base.GetBufPtr(),nInfo * sizeof(TRWD_TAB_CRE));

	int i=0;
	for ( i = 0; i<nInfo; i++)
	{
		VecTrwdTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
}*/
int CLfoAnaObj::GetSsoFac(vector<SSO_FC_TAB_CRE>& VecLfoFcTab)
{
	if (!VecLfoFcTab.empty())
	{
		VecLfoFcTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_FAC_NO_TAB, SSO_FC_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_FC_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO��վ���¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO��վ��[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_FC_TAB_CRE* pInfo = NULL;
	pInfo = new SSO_FC_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_FC_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecLfoFcTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CLfoAnaObj::GetSsoAcln(vector<SSO_LN_TAB_CRE>& VecLfoAclnTab)
{
	if (!VecLfoAclnTab.empty())
	{
		VecLfoAclnTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_ACLN_NO_TAB, SSO_LN_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_LN_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO�����߶α��¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO�����߶α�[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_LN_TAB_CRE* pInfo = NULL;
	pInfo = new SSO_LN_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_LN_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecLfoAclnTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CLfoAnaObj::GetSsoGen(vector<SSO_GN_TAB_CRE>& VecLfoGnTab)
{
	if (!VecLfoGnTab.empty())
	{
		VecLfoGnTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_GN_NO_TAB, SSO_GN_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_GN_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO��������¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO�������[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_GN_TAB_CRE* pInfo = NULL;
	pInfo = new SSO_GN_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_GN_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecLfoGnTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CLfoAnaObj::GetSsoTr(vector<SSO_TR_TAB_CRE>& VecLfoTrTab)
{
	if (!VecLfoTrTab.empty())
	{
		VecLfoTrTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_TR_NO_TAB, SSO_TR_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_TR_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO��ѹ�����¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "SSO��ѹ����[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_TR_TAB_CRE* pInfo = NULL;
	pInfo = new SSO_TR_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_TR_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecLfoTrTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaObj::WritePmuInfo(const int devtableno)
{
	if (para_FilePmuInfo == 0)
	{
		return 0;
	}

	string mode;
	if (devtableno == LFO_FAC_NO_TAB)
	{
		mode = "w";
	}
	else
	{
		mode = "a+";
	}

	string tmpfilename = m_RunInfDir;
	tmpfilename = tmpfilename + m_backslash + "res" + m_backslash + "DevicePmuInfo.txt";
	FILE* fileptr = NULL;
	if ((fileptr = fopen(tmpfilename.c_str(), mode.c_str())) == NULL)
	{
		sprintf(m_LogString, " InitFc() Cannot open the file %s", tmpfilename.c_str());
		MessShow(m_LogString);
		return -1;
	}
	int ii, jj;
	if (devtableno == LFO_FAC_NO_TAB)
	{
		fprintf(fileptr, "//**********************LFO��վPMU��Ϣ(������%d)****************************//\n", m_nFcNum);
		for (ii = 0; ii < m_nFcNum; ii++)
		{
			fprintf(fileptr, "fac_id="INT_REC_FMT",		fac_name=%s,		m_bs_id="INT_REC_FMT",		v_bs_id="INT_REC_FMT"\n",
				INT_REC_VAL(m_pFcPmu[ii].fac_id), m_pFcInf[ii].fac_name, INT_REC_VAL(m_pFcPmu[ii].m_bs_id), m_pFcPmu[ii].v_bs_id);
			for (jj = 0; jj < VEC_V_NUM; jj++)
			{
				fprintf(fileptr, "					        FilNo=%d,  ColNo=%d,			FilNo=%d,  ColNo=%d\n",
					m_pFcPmu[ii].m_bs_vecV.viFilNo[jj],
					m_pFcPmu[ii].m_bs_vecV.viColNo[jj],
					m_pFcPmu[ii].v_bs_vecV.viFilNo[jj],
					m_pFcPmu[ii].v_bs_vecV.viColNo[jj]);
			}
		}
	}
	else if (devtableno == LFO_ACLN_NO_TAB)
	{
		fprintf(fileptr, "//*********************LFO��·PMU��Ϣ(������%d)****************************//\n", m_nLnNum);

		for (ii = 0; ii < m_nLnNum; ii++)
		{
			fprintf(fileptr, "acln_id="INT_REC_FMT",	  acln_name=%s,    dot_id_1="INT_REC_FMT",		dot_id_2="INT_REC_FMT"\n", INT_REC_VAL(m_pLnPmu[ii].ln_id), m_pLnInf[ii].acln_name, INT_REC_VAL(m_pLnPmu[ii].dot_id_1), INT_REC_VAL(m_pLnPmu[ii].dot_id_2));
			fprintf(fileptr, "					     Pwr   FilNo=%d,  ColNo=%d,		Pwr  FilNo=%d,  ColNo=%d\n",
				m_pLnPmu[ii].vecAmpl1.viFilNo,
				m_pLnPmu[ii].vecAmpl1.viColNo,
				m_pLnPmu[ii].vecAmpl2.viFilNo,
				m_pLnPmu[ii].vecAmpl2.viColNo);

			//             for(jj=0; jj<VEC_VI_NUM; jj++)
			//             {
			//                 fprintf(fileptr,"					           FilNo=%d,  ColNo=%d,		     FilNo=%d,  ColNo=%d\n",
			//                     m_pLnPmu[ii].vecVI1.viFilNo[jj],
			//                     m_pLnPmu[ii].vecVI1.viColNo[jj],
			//                     m_pLnPmu[ii].vecVI2.viFilNo[jj],
			//                     m_pLnPmu[ii].vecVI2.viColNo[jj]);
			//             }

		}
	}
	else if (devtableno == LFO_GN_NO_TAB)
	{
		fprintf(fileptr, "//*********************LFO����PMU��Ϣ(������%d)****************************//\n", m_nGnNum);
		for (ii = 0; ii < m_nGnNum; ii++)
		{
			fprintf(fileptr, "gn_id="INT_REC_FMT"     gn_name=%s%s\n", INT_REC_VAL(m_pGnPmu[ii].gn_id), m_pGnInf[ii].fac_name, m_pGnInf[ii].gn_name);
			fprintf(fileptr, "					  SSP   FilNo=%d,  ColNo=%d\n", m_pGnPmu[ii].vecAmpl.viFilNo, m_pGnPmu[ii].vecAmpl.viColNo);
			fprintf(fileptr, "					  SSF    FilNo=%d,  ColNo=%d\n", m_pGnPmu[ii].vecFreq.viFilNo, m_pGnPmu[ii].vecFreq.viColNo);
			fprintf(fileptr, "					  SSO    FilNo=%d,  ColNo=%d\n", m_pGnPmu[ii].vecSSO.viFilNo, m_pGnPmu[ii].vecSSO.viColNo);
			// 			for(jj=0; jj<VEC_VI_NUM; jj++)
			// 			{
			// 				fprintf(fileptr,"					        FilNo=%d,  ColNo=%d\n",
			// 					m_pGnPmu[ii].vecVI.viFilNo[jj],
			// 					m_pGnPmu[ii].vecVI.viColNo[jj]);
			// 			}
		}
	}
	else if (devtableno == LFO_TR_NO_TAB)
	{
		fprintf(fileptr, "//*********************LFO��ѹ��PMU��Ϣ(������%d)****************************//\n", m_nTrNum);
		//         for(ii=0; ii<m_nTrNum; ii++)
		//         {
		//             fprintf(fileptr,"tr_id="INT_REC_FMT",	  tr_name=%s,    high_wind_id="INT_REC_FMT",		mid_wind_id="INT_REC_FMT",		low_wind_id="INT_REC_FMT"\n",\
		//                 m_pTrPmu[ii].tr_id,m_pTrPmu[ii].tr_name, m_pTrPmu[ii].high_wind_id,m_pTrPmu[ii].mid_wind_id,m_pTrPmu[ii].low_wind_id);
		//             fprintf(fileptr,"					  hwd_Pwr   FilNo=%d,  ColNo=%d,		mwd_Pwr  FilNo=%d,  ColNo=%d,		lwd_Pwr  FilNo=%d,  ColNo=%d\n",
		//                 m_pTrPmu[ii].vecPwr_high.viFilNo,
		//                 m_pTrPmu[ii].vecPwr_high.viColNo,
		//                 m_pTrPmu[ii].vecPwr_mid.viFilNo,
		//                 m_pTrPmu[ii].vecPwr_mid.viColNo,
		//                 m_pTrPmu[ii].vecPwr_low.viFilNo,
		//                 m_pTrPmu[ii].vecPwr_low.viColNo);
		//             for(jj=0; jj<VEC_VI_NUM; jj++)
		//             {
		//                 fprintf(fileptr,"					           FilNo=%d,  ColNo=%d,		     FilNo=%d,  ColNo=%d,		     FilNo=%d,  ColNo=%d\n",
		//                     m_pTrPmu[ii].vecVI_high.viFilNo[jj],
		//                     m_pTrPmu[ii].vecVI_high.viColNo[jj],
		//                     m_pTrPmu[ii].vecVI_mid.viFilNo[jj],
		//                     m_pTrPmu[ii].vecVI_mid.viColNo[jj],
		//                     m_pTrPmu[ii].vecVI_low.viFilNo[jj],
		//                     m_pTrPmu[ii].vecVI_low.viColNo[jj]);
		//             }
		//         }
	}
	else
	{
		sprintf(m_LogString, "��Ŵ���[%d]", devtableno);
		MessShow(m_LogString);
	}

	fclose(fileptr);

	return 0;
}

int CLfoAnaObj::EstimateObjOscillate()
{
	int ii, jj;
	int kk;
	int sso;
	int warn_num;//������
	RTDB_TIME ssp_time;
	float ssp, ssf;
	// 	m_SysMon.mon_ampl =0;
	// 	m_SysMon.mon_freq =0;
	m_nGnWarn = 0;
	m_nDevWarn = 0;
	memset((void*)m_pGnWarn, 0, para_nGnMaxMem * sizeof(SSO_GN_MON));
	memset((void*)m_pDevWarn, 0, (para_nGnMaxMem + para_nLnMaxMem + para_nTrMaxMem) * sizeof(SSO_DEV_MON));
	// 	sprintf(m_LogString,"���ֻ������[%d]",m_nGnNum);
	// 	MessShow(m_LogString);
	for (ii = 0; ii < m_nGnNum; ii++)//�ж����л�����״̬
	{
		// 		sprintf(m_LogString,"��������״̬[%d]",m_pGnMon[ii].stat_old);
		// 		MessShow(m_LogString);
		m_pGnMon[ii].stat = MENU_LFO_DEV_STS_SCT;//���豸״̬��ʼΪ��ȫ
		sso = 0;
		warn_num = 0;
		ssp_time = 0;
		ssp = 0.0;
		ssf = 0.0;
		for (jj = 0; jj < m_LfoPara.time_leng * MAX_FRA_PER_SEC; jj++)
		{//��ȡ���������� �ж���״̬ ��ȡ������
			if (m_pGnDat[ii].sso[jj] == 1 /* && m_pGnDat[ii].ampl[jj] > m_LfoPara.gn_swn_ampl */)
			{
				warn_num++;
				if (warn_num >= m_LfoPara.sso_jud_num)
				{
					sso = m_pGnDat[ii].sso[jj];
					if (m_pGnDat[ii].ampl[jj] > ssp)
					{
						ssp = m_pGnDat[ii].ampl[jj];
						ssf = m_pGnDat[ii].freq[jj];
						ssp_time = m_CurGpsTime - m_LfoPara.time_leng + int(jj / MAX_FRA_PER_SEC);
					}
				}
			}
			else
			{
				m_pGnMon[ii].lead_ampl = m_pGnDat[ii].ampl[jj];
				m_pGnMon[ii].lead_freq = m_pGnDat[ii].freq[jj];
				warn_num = 0;
			}
		}

		if (sso == 1)
		{
			m_pGnWarn[m_nGnWarn].gn_id = m_pGnDat[ii].gn_id;
			if (para_ShowFacName == 1) {
				sprintf(m_pGnWarn[m_nGnWarn].gn_name, "%s.%s", m_pGnInf[ii].fac_name, m_pGnInf[ii].gn_name);
			}
			else {
				sprintf(m_pGnWarn[m_nGnWarn].gn_name, "%s", m_pGnInf[ii].gn_name);
			}
			m_pGnWarn[m_nGnWarn].fac_id = m_pGnInf[ii].fac_id;
			m_pGnWarn[m_nGnWarn].lead_ampl = ssp;
			m_pGnWarn[m_nGnWarn].lead_freq = ssf;
			m_pGnWarn[m_nGnWarn].stat = MENU_LFO_DEV_STS_WRN;
			m_pGnMon[ii].lead_ampl = ssp;
			m_pGnMon[ii].lead_freq = ssf;
			m_pGnMon[ii].stat = MENU_LFO_DEV_STS_WRN;
			m_pGnMon[ii].if_sso = MENU_LFO_MENU_YES;
			if (m_pGnMon[ii].max_ampl < ssp)
			{
				m_pGnMon[ii].max_ampl = ssp;
				m_pGnMon[ii].lfo_max_ampl_t = ssp_time;
				m_pGnWarn[m_nGnWarn].max_ampl = ssp;
				m_pGnWarn[m_nGnWarn].lfo_max_ampl_t = ssp_time;
			}
			if (m_SysMon.mon_ampl < m_pGnMon[ii].lead_ampl)
			{
				m_SysMon.mon_ampl = m_pGnMon[ii].lead_ampl;
				m_SysMon.mon_freq = m_pGnMon[ii].lead_freq;
			}
			if (m_pGnMon[ii].stat_old == MENU_LFO_DEV_STS_SCT)//���ְ�ȫ�����ֿ�ʼ��
			{
				if (para_ShowFacName == 1) {
					sprintf(m_LogString, "����[%s.%s]��ʼ��,ampl=%f,freq=%f", m_pGnInf[ii].fac_name, m_pGnInf[ii].gn_name, m_SysMon.mon_ampl, m_SysMon.mon_freq);
				}
				else {
					sprintf(m_LogString, "����[%s]��ʼ��,ampl=%f,freq=%f", m_pGnInf[ii].gn_name, m_SysMon.mon_ampl, m_SysMon.mon_freq);
				}
				MessShow(m_LogString);
				m_pGnWarn[m_nGnWarn].sso_bgn = m_CalcStartTime;
				m_pGnMon[ii].sso_bgn = m_CalcStartTime;
				m_pGnWarn[m_nGnWarn].aver_freq1 = ssf;
				m_pGnMon[ii].aver_freq1 = ssf;
			}
			else if (m_pGnMon[ii].stat_old == MENU_LFO_DEV_STS_WRN)//�����񵴣����ֳ�����
			{
				m_pGnWarn[m_nGnWarn].sso_bgn = m_pGnMon[ii].sso_bgn;
				m_pGnWarn[m_nGnWarn].sso_end = m_CurGpsTime;
				m_pGnMon[ii].sso_end = m_CurGpsTime;
				strcpy(m_pGnWarn[m_nGnWarn].sso_time, PT_GetStringOfTimeInterval((m_CurGpsTime - m_pGnWarn[m_nGnWarn].sso_bgn) * 1000));
				strcpy(m_pGnMon[ii].sso_time, PT_GetStringOfTimeInterval((m_CurGpsTime - m_pGnMon[ii].sso_bgn) * 1000));
			}
			m_pDevWarn[m_nDevWarn].dev_id = m_pGnWarn[m_nGnWarn].gn_id;
			m_pDevWarn[m_nDevWarn].fac_id = m_pGnWarn[m_nGnWarn].fac_id;
			m_pDevWarn[m_nDevWarn].sso_bgn = m_pGnWarn[m_nGnWarn].sso_bgn;
			m_pDevWarn[m_nDevWarn].sso_end = m_pGnWarn[m_nGnWarn].sso_end;
			m_pDevWarn[m_nDevWarn].lead_ampl = m_pGnWarn[m_nGnWarn].lead_ampl;
			m_pDevWarn[m_nDevWarn].lead_freq = m_pGnWarn[m_nGnWarn].lead_freq;
			if (IsWarnDev(m_pGnWarn[m_nGnWarn].gn_id, kk) == false)//���豸δͳ�ƹ�
			{
				m_pDevEveWarn[m_nDevEveWarn].dev_id = m_pGnWarn[m_nGnWarn].gn_id;
				m_pDevEveWarn[m_nDevEveWarn].fac_id = m_pGnWarn[m_nGnWarn].fac_id;
				m_pDevEveWarn[m_nDevEveWarn].sso_bgn = m_pGnWarn[m_nGnWarn].sso_bgn;
				m_pDevEveWarn[m_nDevEveWarn].sso_end = m_pGnWarn[m_nGnWarn].sso_end;
				m_pDevEveWarn[m_nDevEveWarn].lead_ampl = ssp;
				m_pDevEveWarn[m_nDevEveWarn].lead_freq = ssf;
				sprintf(m_pDevEveWarn[m_nDevEveWarn].dev_name, "%s", m_pGnWarn[m_nGnWarn].gn_name);
				sprintf(m_pDevEveWarn[m_nDevEveWarn].fac_name, "%s", m_pGnInf[ii].fac_name);
				m_nDevEveWarn++;
			}
			else//��ͳ�ƹ�,�޸��񵴽���ʱ��
			{
				m_pDevEveWarn[kk].sso_end = m_pGnWarn[m_nGnWarn].sso_end;
				if (m_pDevEveWarn[kk].lead_ampl < ssp)
				{
					m_pDevEveWarn[kk].lead_ampl = ssp;
					m_pDevEveWarn[kk].lead_freq = ssf;
				}
			}
			m_nGnWarn++;
			m_nDevWarn++;
			sprintf(m_LogString, "����[%s]״̬[%d],ampl=%f,freq=[%f]", m_pGnMon[ii].gn_name, m_pGnMon[ii].stat, m_pGnMon[ii].lead_ampl, m_pGnMon[ii].lead_freq);
			MessShow(m_LogString);
		}
		m_pGnMon[ii].stat_old = m_pGnMon[ii].stat;
	}
	sprintf(m_LogString, "�񵴻������[%d],m_SysMon.mon_freq=%f", m_nGnWarn, m_SysMon.mon_freq);
	MessShow(m_LogString);

	sprintf(m_LogString, "������·����[%d]", m_nLnNum);
	MessShow(m_LogString);
	m_nLnWarn = 0;
	memset((void*)m_pLnWarn, 0, para_nLnMaxMem * sizeof(SSO_LN_MON));
	for (ii = 0; ii < m_nLnNum; ii++)//�ж�������·��״̬
	{
		sso = 0;
		warn_num = 0;
		ssp_time = 0;
		ssp = 0.0;
		ssf = 0.0;
		m_pLnMon[ii].stat = MENU_LFO_DEV_STS_SCT;//���豸״̬��ʼΪ��ȫ
		for (jj = 0; jj < m_LfoPara.time_leng * MAX_FRA_PER_SEC; jj++)
		{
			if (m_pLnDat[ii].sso[jj] == 1 /*&& m_pLnDat[ii].ampl[jj] > m_LfoPara.ln_swn_ampl1 */)
			{
				warn_num++;
				if (warn_num >= m_LfoPara.sso_jud_num)
				{
					sso = m_pLnDat[ii].sso[jj];
					if (m_pLnDat[ii].ampl[jj] > ssp)
					{
						ssp = m_pLnDat[ii].ampl[jj];
						ssf = m_pLnDat[ii].freq[jj];
						ssp_time = m_CurGpsTime + int(jj / MAX_FRA_PER_SEC);
					}
				}
			}
			else
			{
				m_pLnMon[ii].lead_ampl = m_pLnDat[ii].ampl[jj];
				m_pLnMon[ii].lead_freq = m_pLnDat[ii].freq[jj];
				warn_num = 0;
			}

		}
		if (sso == 1)
		{
			m_pLnWarn[m_nLnWarn].ln_id = m_pLnDat[ii].ln_id;
			sprintf(m_pLnWarn[m_nLnWarn].ln_name, "%s", m_pLnInf[ii].acln_name);
			if (m_pLnCtl[ii].is_end == 0)
			{
				m_pLnWarn[m_nLnWarn].fac_id = m_pLnInf[ii].fac_id_1;
			}
			else
			{
				m_pLnWarn[m_nLnWarn].fac_id = m_pLnInf[ii].fac_id_2;
			}
			m_pLnWarn[m_nLnWarn].lead_ampl = ssp;
			m_pLnWarn[m_nLnWarn].lead_freq = ssf;
			m_pLnWarn[m_nLnWarn].stat = MENU_LFO_DEV_STS_WRN;
			m_pLnMon[ii].lead_ampl = ssp;
			m_pLnMon[ii].lead_freq = ssf;
			m_pLnMon[ii].stat = MENU_LFO_DEV_STS_WRN;
			m_pLnMon[ii].if_sso = MENU_LFO_MENU_YES;
			if (m_pLnMon[ii].max_ampl < ssp)
			{
				m_pLnMon[ii].max_ampl = ssp;
				m_pLnMon[ii].lfo_max_ampl_t = ssp_time;
				m_pLnWarn[m_nLnWarn].max_ampl = ssp;
				m_pLnWarn[m_nLnWarn].lfo_max_ampl_t = ssp_time;
			}
			if (m_SysMon.mon_ampl < m_pLnMon[ii].lead_ampl)
			{
				m_SysMon.mon_ampl = m_pLnMon[ii].lead_ampl;
				m_SysMon.mon_freq = m_pLnMon[ii].lead_freq;
			}
			if (m_pLnMon[ii].stat_old == MENU_LFO_DEV_STS_SCT)//���ְ�ȫ�����ֿ�ʼ��
			{
				m_pLnWarn[m_nLnWarn].sso_bgn = m_CalcStartTime;
				m_pLnMon[ii].sso_bgn = m_CalcStartTime;
				m_pLnWarn[m_nLnWarn].aver_freq1 = ssf;
				m_pLnMon[ii].aver_freq1 = ssf;
			}
			else if (m_pLnMon[ii].stat_old == MENU_LFO_DEV_STS_WRN)//�����񵴣����ֳ�����
			{
				m_pLnWarn[m_nLnWarn].sso_bgn = m_pLnMon[ii].sso_bgn;
				m_pLnWarn[m_nLnWarn].sso_end = m_CurGpsTime;
				m_pLnMon[ii].sso_end = m_CurGpsTime;
				strcpy(m_pLnWarn[m_nLnWarn].sso_time, PT_GetStringOfTimeInterval((m_CurGpsTime - m_pLnWarn[m_nLnWarn].sso_bgn) * 1000));
				strcpy(m_pLnMon[ii].sso_time, PT_GetStringOfTimeInterval((m_CurGpsTime - m_pLnMon[ii].sso_bgn) * 1000));
			}
			m_pDevWarn[m_nDevWarn].dev_id = m_pLnWarn[m_nLnWarn].ln_id;
			m_pDevWarn[m_nDevWarn].fac_id = m_pLnWarn[m_nLnWarn].fac_id;
			m_pDevWarn[m_nDevWarn].sso_bgn = m_pLnWarn[m_nLnWarn].sso_bgn;
			m_pDevWarn[m_nDevWarn].sso_end = m_pLnWarn[m_nLnWarn].sso_end;
			m_pDevWarn[m_nDevWarn].lead_ampl = m_pLnWarn[m_nLnWarn].lead_ampl;
			m_pDevWarn[m_nDevWarn].lead_freq = m_pLnWarn[m_nLnWarn].lead_freq;
			if (IsWarnDev(m_pLnWarn[m_nLnWarn].ln_id, kk) == false)//���豸δͳ�ƹ�
			{
				m_pDevEveWarn[m_nDevEveWarn].dev_id = m_pLnWarn[m_nLnWarn].ln_id;
				m_pDevEveWarn[m_nDevEveWarn].fac_id = m_pLnWarn[m_nLnWarn].fac_id;
				m_pDevEveWarn[m_nDevEveWarn].sso_bgn = m_pLnWarn[m_nLnWarn].sso_bgn;
				m_pDevEveWarn[m_nDevEveWarn].sso_end = m_pLnWarn[m_nLnWarn].sso_end;
				m_pDevEveWarn[m_nDevEveWarn].lead_ampl = ssp;
				m_pDevEveWarn[m_nDevEveWarn].lead_freq = ssf;
				sprintf(m_pDevEveWarn[m_nDevEveWarn].dev_name, "%s", m_pLnWarn[m_nLnWarn].ln_name);
				if (m_pLnCtl[ii].is_end == 0)
				{
					sprintf(m_pDevEveWarn[m_nDevEveWarn].fac_name, "%s", m_pLnInf[ii].fac_name_1);
				}
				else
				{
					sprintf(m_pDevEveWarn[m_nDevEveWarn].fac_name, "%s", m_pLnInf[ii].fac_name_2);
				}
				m_nDevEveWarn++;
			}
			else//��ͳ�ƹ�,�޸��񵴽���ʱ��
			{
				m_pDevEveWarn[kk].sso_end = m_pLnWarn[m_nLnWarn].sso_end;
				if (m_pDevEveWarn[kk].lead_ampl < ssp)
				{
					m_pDevEveWarn[kk].lead_ampl = ssp;
					m_pDevEveWarn[kk].lead_freq = ssf;
				}
			}
			m_nDevWarn++;
			m_nLnWarn++;
			sprintf(m_LogString, "��·[%s]״̬[%d],ampl=%f,freq=[%f]", m_pLnMon[ii].ln_name, m_pLnMon[ii].stat, m_pLnMon[ii].lead_ampl, m_pLnMon[ii].lead_freq);
			MessShow(m_LogString);
		}
		m_pLnMon[ii].stat_old = m_pLnMon[ii].stat;
	}

	sprintf(m_LogString, "����·����[%d],m_SysMon.mon_freq=[%f]", m_nLnWarn, m_SysMon.mon_freq);
	MessShow(m_LogString);

	sprintf(m_LogString, "���ֱ�ѹ������[%d]", m_nTrNum);
	MessShow(m_LogString);
	m_nTrWarn = 0;
	memset((void*)m_pTrWarn, 0, para_nTrMaxMem * sizeof(SSO_TR_MON));
	for (ii = 0; ii < m_nTrNum; ii++)//�ж����б�ѹ����״̬
	{
		sso = 0;
		warn_num = 0;
		ssp_time = 0;
		ssp = 0.0;
		ssf = 0.0;
		m_pTrMon[ii].stat = MENU_LFO_DEV_STS_SCT;//���豸״̬��ʼΪ��ȫ
		for (jj = 0; jj < m_LfoPara.time_leng * MAX_FRA_PER_SEC; jj++)
		{
			// 			sprintf(m_LogString,"��ѹ��[%ld]����[%d],ampl=%f,freq=[%f]",m_pTrMon[ii].tr_id,m_pTrDat[ii].sso[jj],m_pTrDat[ii].ampl[jj],m_pTrDat[ii].freq[jj]);
			// 			MessShow(m_LogString);
			if (m_pTrDat[ii].sso[jj] == 1 /*&& m_pTrDat[ii].ampl[jj] > m_LfoPara.tr_swn_ampl1 */)
			{
				warn_num++;
				if (warn_num >= m_LfoPara.sso_jud_num)
				{
					sso = m_pTrDat[ii].sso[jj];
					if (m_pTrDat[ii].ampl[jj] > ssp)
					{
						ssp = m_pTrDat[ii].ampl[jj];
						ssf = m_pTrDat[ii].freq[jj];
						ssp_time = m_CurGpsTime + int(jj / MAX_FRA_PER_SEC);
					}
				}
			}
			else
			{
				m_pTrMon[ii].lead_ampl = m_pTrDat[ii].ampl[jj];
				m_pTrMon[ii].lead_freq = m_pTrDat[ii].freq[jj];
				warn_num = 0;
			}
		}
		if (sso == 1)
		{
			m_pTrWarn[m_nTrWarn].tr_id = m_pTrDat[ii].tr_id;
			if (para_ShowFacName) {
				sprintf(m_pTrWarn[m_nTrWarn].tr_name, "%s.%s", m_pTrInf[ii].fac_name, m_pTrInf[ii].tr_name);
			}
			else {
				sprintf(m_pTrWarn[m_nTrWarn].tr_name, "%s", m_pTrInf[ii].tr_name);
			}
			m_pTrWarn[m_nTrWarn].fac_id = m_pTrInf[ii].fac_id;
			m_pTrWarn[m_nTrWarn].lead_ampl = ssp;
			m_pTrWarn[m_nTrWarn].lead_freq = ssf;
			m_pTrWarn[m_nTrWarn].stat = MENU_LFO_DEV_STS_WRN;
			m_pTrMon[ii].lead_ampl = ssp;
			m_pTrMon[ii].lead_freq = ssf;
			m_pTrMon[ii].stat = MENU_LFO_DEV_STS_WRN;
			m_pTrMon[ii].if_sso = MENU_LFO_MENU_YES;
			if (m_pTrMon[ii].max_ampl < ssp)
			{
				m_pTrMon[ii].max_ampl = ssp;
				m_pTrMon[ii].lfo_max_ampl_t = ssp_time;
				m_pTrWarn[m_nTrWarn].max_ampl = ssp;
				m_pTrWarn[m_nTrWarn].lfo_max_ampl_t = ssp_time;
			}
			if (m_SysMon.mon_ampl < m_pTrMon[ii].lead_ampl)
			{
				m_SysMon.mon_ampl = m_pTrMon[ii].lead_ampl;
				m_SysMon.mon_freq = m_pTrMon[ii].lead_freq;
				// 				sprintf(m_LogString,"��ѹ��***,m_SysMon.mon_ampl=%f,m_SysMon.mon_freq=[%f]",m_SysMon.mon_ampl,m_SysMon.mon_freq);
				// 				MessShow(m_LogString);
			}
			if (m_pTrMon[ii].stat_old == MENU_LFO_DEV_STS_SCT)//���ְ�ȫ�����ֿ�ʼ��
			{
				m_pTrWarn[m_nTrWarn].sso_bgn = m_CalcStartTime;
				m_pTrMon[ii].sso_bgn = m_CalcStartTime;
				m_pTrWarn[m_nTrWarn].aver_freq1 = ssf;
				m_pTrMon[ii].aver_freq1 = ssf;
			}
			else if (m_pTrMon[ii].stat_old == MENU_LFO_DEV_STS_WRN)//�����񵴣����ֳ�����
			{
				m_pTrWarn[m_nTrWarn].sso_bgn = m_pTrMon[ii].sso_bgn;
				m_pTrWarn[m_nTrWarn].sso_end = m_CurGpsTime;
				m_pTrMon[ii].sso_end = m_CurGpsTime;
				strcpy(m_pTrWarn[m_nTrWarn].sso_time, PT_GetStringOfTimeInterval((m_CurGpsTime - m_pTrWarn[m_nTrWarn].sso_bgn) * 1000));
				strcpy(m_pTrMon[ii].sso_time, PT_GetStringOfTimeInterval((m_CurGpsTime - m_pTrMon[ii].sso_bgn) * 1000));
			}
			m_pDevWarn[m_nDevWarn].dev_id = m_pTrWarn[m_nTrWarn].tr_id;
			m_pDevWarn[m_nDevWarn].fac_id = m_pTrWarn[m_nTrWarn].fac_id;
			m_pDevWarn[m_nDevWarn].sso_bgn = m_pTrWarn[m_nTrWarn].sso_bgn;
			m_pDevWarn[m_nDevWarn].sso_end = m_pTrWarn[m_nTrWarn].sso_end;
			m_pDevWarn[m_nDevWarn].lead_ampl = m_pTrWarn[m_nTrWarn].lead_ampl;
			m_pDevWarn[m_nDevWarn].lead_freq = m_pTrWarn[m_nTrWarn].lead_freq;
			if (IsWarnDev(m_pTrWarn[m_nTrWarn].tr_id, kk) == false)//���豸δͳ�ƹ�
			{
				m_pDevEveWarn[m_nDevEveWarn].dev_id = m_pTrWarn[m_nTrWarn].tr_id;
				m_pDevEveWarn[m_nDevEveWarn].fac_id = m_pTrWarn[m_nTrWarn].fac_id;
				m_pDevEveWarn[m_nDevEveWarn].sso_bgn = m_pTrWarn[m_nTrWarn].sso_bgn;
				m_pDevEveWarn[m_nDevEveWarn].sso_end = m_pTrWarn[m_nTrWarn].sso_end;
				m_pDevEveWarn[m_nDevEveWarn].lead_ampl = ssp;
				m_pDevEveWarn[m_nDevEveWarn].lead_freq = ssf;
				sprintf(m_pDevEveWarn[m_nDevEveWarn].dev_name, "%s", m_pTrWarn[m_nTrWarn].tr_name);
				sprintf(m_pDevEveWarn[m_nDevEveWarn].fac_name, "%s", m_pTrInf[ii].fac_name);
				m_nDevEveWarn++;
			}
			else//��ͳ�ƹ�,�޸��񵴽���ʱ��
			{
				m_pDevEveWarn[kk].sso_end = m_pTrWarn[m_nTrWarn].sso_end;
				if (m_pDevEveWarn[kk].lead_ampl < ssp)
				{
					m_pDevEveWarn[kk].lead_ampl = ssp;
					m_pDevEveWarn[kk].lead_freq = ssf;
				}
			}
			m_nDevWarn++;
			m_nTrWarn++;
			sprintf(m_LogString, "��ѹ��[%s]״̬[%d],ampl=%f,freq=[%f]", m_pTrMon[ii].tr_name, m_pTrMon[ii].stat, m_pTrMon[ii].lead_ampl, m_pTrMon[ii].lead_freq);
			MessShow(m_LogString);
		}
		m_pTrMon[ii].stat_old = m_pTrMon[ii].stat;
	}
	sprintf(m_LogString, "�񵴱�ѹ������[%d],m_SysMon.mon_freq=[%f]", m_nTrWarn, m_SysMon.mon_freq);
	MessShow(m_LogString);
	return m_nGnWarn + m_nLnWarn + m_nTrWarn;
}

int CLfoAnaObj::EstimateSysOscillate()
{
	m_SysMon.if_sso = MENU_SSO_MENU_NO;//��ϵͳ�Ƿ�����Ϊ��
	if (m_SysMon_Old.if_sso == MENU_SSO_MENU_NO || m_SysMon_Old.if_sso == MENU_SSO_MENU_YESTONO)//����ϵͳ���ڰ�ȫ״̬��ƽϢ״̬
	{
		sprintf(m_LogString, "����ϵͳ���ڰ�ȫ״̬��ƽϢ״̬[%d],", m_SysMon_Old.if_sso);
		MessShow(m_LogString);
		//�жϱ����豸״̬
		if (EstimateObjOscillate() > 0)//�����豸��(����)
		{
			float max_ampl = 0;
			int ii;
			m_SysMon.if_sso = MENU_SSO_MENU_NOTOYES;
			for (ii = 0; ii < m_nGnWarn; ii++)
			{
				if (m_pGnWarn[ii].lead_ampl > max_ampl)
				{
					m_SysMon.warn_dev_id = m_pGnWarn[ii].gn_id;
					sprintf(m_SysMon.warn_dev_name, "%s", m_pGnWarn[ii].gn_name);
					m_SysMon.warn_freq = m_pGnWarn[ii].lead_freq;
					max_ampl = m_pGnWarn[ii].lead_ampl;
				}
			}
			for (ii = 0; ii < m_nLnWarn; ii++)
			{
				if (m_pLnWarn[ii].lead_ampl > max_ampl)
				{
					m_SysMon.warn_dev_id = m_pLnWarn[ii].ln_id;
					sprintf(m_SysMon.warn_dev_name, "%s", m_pLnWarn[ii].ln_name);
					m_SysMon.warn_freq = m_pLnWarn[ii].lead_freq;
					max_ampl = m_pLnWarn[ii].lead_ampl;
				}
			}
			for (ii = 0; ii < m_nTrWarn; ii++)
			{
				if (m_pTrWarn[ii].lead_ampl > max_ampl)
				{
					m_SysMon.warn_dev_id = m_pTrWarn[ii].tr_id;
					sprintf(m_SysMon.warn_dev_name, "%s", m_pTrWarn[ii].tr_name);
					m_SysMon.warn_freq = m_pTrWarn[ii].lead_freq;
					max_ampl = m_pTrWarn[ii].lead_ampl;
				}
			}
			// 			if (m_nGnWarn > 0)
			// 			{
			// 				m_SysMon.warn_dev_id = m_pGnWarn[0].gn_id;
			// 				sprintf(m_SysMon.warn_dev_name,"%s",m_pGnWarn[0].gn_name);
			// 				m_SysMon.warn_freq = m_pGnWarn[0].lead_freq;
			// 			} 
			// 			else if (m_nLnWarn > 0)
			// 			{
			// 				m_SysMon.warn_dev_id = m_pLnWarn[0].ln_id;
			// 				sprintf(m_SysMon.warn_dev_name,"%s",m_pLnWarn[0].ln_name);
			// 				m_SysMon.warn_freq = m_pLnWarn[0].lead_freq;
			// 			}
			// 			else if (m_nTrWarn > 0 )
			// 			{
			// 				m_SysMon.warn_dev_id = m_pTrWarn[0].tr_id;
			// 				sprintf(m_SysMon.warn_dev_name,"%s",m_pTrWarn[0].tr_name);
			// 				m_SysMon.warn_freq = m_pTrWarn[0].lead_freq;
			// 			}
			m_SysMon.stat = MENU_LFO_DEV_STS_WRN;
			m_SysMon.sso_bgn = m_CalcStartTime;
			m_SysMon.sso_gn_num = m_nGnWarn;
			m_SysMon.sso_ln_num = m_nLnWarn;
			m_SysMon.sso_tr_num = m_nTrWarn;
			strcpy(m_SysMon.stat_ctx, PT_GetChineseTimeStringBySeconds(m_SysMon.sso_bgn));
			strcat(m_SysMon.stat_ctx, "��⵽��ͬ����");
			if (m_SysMon.start_time <= 0)
			{
				m_SysMon.start_time = m_SysMon.sso_bgn - (m_SysMon.sso_bgn % 60);//�ٻ���ʼʱ��Ϊ��ʼ��ʱ�̵�������
				m_SysMon.end_time = m_SysMon.sso_bgn - (m_SysMon.sso_bgn % 60) + 60;
			}
		}
		else
		{
			m_SysMon.stat = MENU_LFO_DEV_STS_SCT;
			m_SysMon.if_sso = MENU_SSO_MENU_NO;//��ϵͳ��Ϊ��ȫ״̬
			return 0;
		}
	}
	else if (m_SysMon_Old.if_sso == MENU_SSO_MENU_YES || m_SysMon_Old.if_sso == MENU_SSO_MENU_NOTOYES)//���ִ�����״̬������״̬
	{
		sprintf(m_LogString, "���ִ�����״̬������״̬[%d],", m_SysMon_Old.if_sso);
		MessShow(m_LogString);
		//�жϱ����豸״̬
		if (EstimateObjOscillate() > 0) // ���ֳ����澯
		{
			m_SysMon.if_sso = MENU_SSO_MENU_YES;
			m_SysMon.stat = MENU_LFO_DEV_STS_WRN;
			m_SysMon.sso_gn_num = m_nGnWarn;
			m_SysMon.sso_ln_num = m_nLnWarn;
			m_SysMon.sso_tr_num = m_nTrWarn;
			//�����豸�������٣���ͳ�������豸����
			if (m_SysMon.sso_gn_num < m_SysMon_Old.sso_gn_num)
			{
				m_SysMon.sso_gn_num = m_SysMon_Old.sso_gn_num;
			}
			if (m_SysMon.sso_ln_num < m_SysMon_Old.sso_ln_num)
			{
				m_SysMon.sso_ln_num = m_SysMon_Old.sso_ln_num;
			}
			if (m_SysMon.sso_tr_num < m_SysMon_Old.sso_tr_num)
			{
				m_SysMon.sso_tr_num = m_SysMon_Old.sso_tr_num;
			}
			m_SysMon.sso_bgn = m_SysMon_Old.sso_bgn;
			m_SysMon.sso_end = m_CurGpsTime;
			strcpy(m_SysMon.sso_time, PT_GetStringOfTimeInterval((m_SysMon.sso_end - m_SysMon.sso_bgn) * 1000));
			strcpy(m_SysMon.stat_ctx, PT_GetChineseTimeStringBySeconds(m_SysMon.sso_bgn));
			strcat(m_SysMon.stat_ctx, "��⵽��ͬ����");
			if (m_CallHDFile == 1)
			{
				if ((m_CurGpsTime - m_SysMon_Old.end_time) >= 0 && (m_CurGpsTime - m_SysMon_Old.end_time) < 60)//��ǰʱ�䳬���ϴ��ٻ�����δ��ʱ��1����
				{
				}
				else if ((m_CurGpsTime - m_SysMon_Old.end_time) >= 60 && (m_CurGpsTime - m_SysMon_Old.end_time) < 120)//��ǰʱ�䳬���ϴ��ٻ�����ʱ��1���ӣ�δ��2����
				{
					m_SysMon.start_time = m_SysMon_Old.end_time;
					m_SysMon.end_time = m_SysMon.start_time + 60;
				}
				else if ((m_CurGpsTime - m_SysMon_Old.end_time) >= 120)//��ǰʱ�䳬���ϴ��ٻ�����ʱ��2����
				{
					m_SysMon.start_time = m_CurGpsTime - 60 - (m_CurGpsTime % 60);
					m_SysMon.end_time = m_SysMon.start_time + 60;
				}
			}
		}
		else//���ֲ��澯����ƽϢ ���¼�����
		{
			m_SysMon.if_sso = MENU_SSO_MENU_YESTONO;
			m_SysMon.stat = MENU_LFO_DEV_STS_SCT;
			m_SysMon.sso_bgn = m_SysMon_Old.sso_bgn;
			m_SysMon.sso_end = m_CurGpsTime - m_LfoPara.time_leng;
			strcpy(m_SysMon.sso_time, PT_GetStringOfTimeInterval((m_SysMon.sso_end - m_SysMon.sso_bgn) * 1000));
			memset(&m_SysMon.stat_ctx, 0, sizeof(m_SysMon.stat_ctx));
			if (m_SysMon.sso_end > m_SysMon_Old.end_time)
			{
				m_SysMon.start_time = m_SysMon.sso_end - (m_SysMon.sso_end % 60);
				m_SysMon.end_time = m_SysMon.start_time + 60;
			}
		}
	}
	sprintf(m_LogString, "����ϵͳ״̬�ж����[%d],�澯����[%d],�澯��·[%d],�澯��ѹ��[%d],ϵͳ���[%f],ϵͳƵ��[%f]", m_SysMon.if_sso, m_SysMon.sso_gn_num, m_SysMon.sso_ln_num, m_SysMon.sso_tr_num, m_SysMon.mon_ampl, m_SysMon.mon_freq);
	MessShow(m_LogString);
	sprintf(m_LogString, "stat_ctx[%s]", m_SysMon.stat_ctx);
	MessShow(m_LogString);
	memcpy((char*)&m_SysMon_Old, (char*)&m_SysMon, sizeof(SSO_SYS_MON));
	return m_SysMon.if_sso;
}

void CLfoAnaObj::PushCallFac()
{
	int ii, jj;
	m_nFcWarn = 0;
	memset((void*)m_pFcWarn, 0, para_nFcMaxMem * sizeof(SSO_FC_MON));
	// 	for (ii=0 ; ii < m_nDevWarn; ii++)
	// 	{
	// 		sprintf(m_LogString,"�澯��վid[%ld]", m_pDevWarn[ii].fac_id);
	// 		MessShow(m_LogString);
	// 	}
	for (ii = 0; ii < m_nDevWarn; ii++)//���豸�г�վID�ظ�����Ϊ-1
	{
		if (!VALID_INT_REC(m_pDevWarn[ii].fac_id))
		{
			continue;
		}
		for (jj = ii + 1; jj < m_nDevWarn; jj++)
		{
			if (m_pDevWarn[ii].fac_id == m_pDevWarn[jj].fac_id)
			{
				ZERO_INT_REC(m_pDevWarn[jj].fac_id);
				if (m_pDevWarn[ii].sso_bgn > m_pDevWarn[jj].sso_bgn)
				{
					m_pDevWarn[ii].sso_bgn = m_pDevWarn[jj].sso_bgn;
				}
			}
		}
		m_pFcWarn[m_nFcWarn].fac_id = m_pDevWarn[ii].fac_id;
		m_nFcWarn++;
	}
	// 	sprintf(m_LogString,"�澯��վ����[%d],�澯�������[%d],�澯��·����[%d],�澯��ѹ������[%d],�澯�豸����[%d]", m_nFcWarn,m_nGnWarn,m_nLnWarn,m_nTrWarn,m_nDevWarn);
	// 	MessShow(m_LogString);

}

// ��վ��Ϣ(ĸ��ѡȡ��վ�е�ѹ�ȼ���ߵ���ĸ/��ĸ����ָ��ĸ��)
bool CLfoAnaObj::InitFc(vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<FES_YC_TAB_CRE>& VecFesYcTab)
{
#ifdef _WAMAP_
	if (!m_pFcInf || !m_pFcCtl || !m_pFcPmu || !m_pFcMon)
	{
		sprintf(m_LogString, "InitFc(): pointer >m_pFcInf >m_pFcCtl >m_pFcPmu >m_pFcMon no Alloc Memory");
		MessShow(m_LogString);
		return false;
	}

	// LFO��վ��
	vector<SSO_FC_TAB_CRE> VecLfoFcTab;
	if (GetSsoFac(VecLfoFcTab) < 0)
	{
		return false;
	}

	// ĸ�߱�
	vector<BUS_TAB_CRE> VecBusTab;
	if (PODB_GetBusInfo(VecBusTab) < 0)
	{
		return false;
	}

	// ��ѹ���ͱ�
	vector<VL_TYPE_TAB_CRE> VecVLType;
	PODB_GetVLTypeInfo(VecVLType);

	// set >> m_pFcInf, m_pFcCtl, m_pFcPmu
	int ii, jj, kk, find_flag1, find_flag2;
	m_nFcNum = (int)VecLfoFcTab.size();
	for (ii = 0; ii < (int)VecLfoFcTab.size(); ii++)
	{
		m_pFcInf[ii].fac_idx = VecLfoFcTab[ii].fac_idx;
		m_pFcInf[ii].fac_id = VecLfoFcTab[ii].station_id;
		m_pFcInf[ii].vlty_id = VecLfoFcTab[ii].vlty_id;

		m_pFcCtl[ii].station_id = VecLfoFcTab[ii].station_id;
		// 		m_pFcCtl[ii].is_m_bs  = VecLfoFcTab[ii].is_m_bs;

		for (jj = 0; jj < (int)VecFacTab.size(); jj++)
		{
			if (m_pFcInf[ii].fac_id == VecFacTab[jj].fac_id)
			{
				m_pFcInf[ii].area_id = VecFacTab[jj].area_id;
				sprintf(m_pFcInf[ii].fac_name, "%s", VecFacTab[jj].fac_name);
				break;
			}
		}
		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (m_pFcInf[ii].area_id == VecAreaTab[jj].area_id)
			{
				sprintf(m_pFcInf[ii].area_name, "%s", VecAreaTab[jj].area_name);
				break;
			}
		}

		// 		for ( jj = 0; jj<(int)VecVLType.size(); jj++ )
		// 		{
		// 			if (m_pFcInf[ii].vlty_id == VecVLType[jj].vlty_id )
		// 			{
		// 				m_pFcInf[ii].m_bs_vmax = VecVLType[jj].vbase * VOL_UP_LMT_COE;
		// 				m_pFcInf[ii].m_bs_vmin = VecVLType[jj].vbase * VOL_LW_LMT_COE;
		// 				m_pFcInf[ii].v_bs_vmax = VecVLType[jj].vbase * VOL_UP_LMT_COE;
		// 				m_pFcInf[ii].v_bs_vmin = VecVLType[jj].vbase * VOL_LW_LMT_COE;
		// 				break;
		// 			}
		// 		}
		// 
		// 	    find_flag1 = 0;
		// 	    find_flag2 = 0;
		//         //  ĸ��ѡȡ����������
		// 		for( jj = 0; jj < (int)VecBusTab.size(); jj++ )
		// 		{
		// 			if( m_pFcInf[ii].fac_id == VecBusTab[jj].fac_id && m_pFcInf[ii].vlty_id == VecBusTab[jj].vlty_id )
		// 			{
		// 				m_pFcInf[ii].m_bs_id   = VecBusTab[jj].bs_id;
		// 				find_flag1 = 1;
		// 				break;
		// // 				if ( VecBusTab[jj].bs_type == BS_TYPE_ZM )
		// // 				{
		// // 					m_pFcInf[ii].m_bs_id   = VecBusTab[jj].bs_id;
		// // 					find_flag1 = 1;
		// // 				}
		// // 				else if ( VecBusTab[jj].bs_type == BS_TYPE_DM && m_pFcInf[ii].m_bs_id < BS_DEVICE_NO_TAB )
		// // 				{
		// // 					m_pFcInf[ii].m_bs_id   = VecBusTab[jj].bs_id;
		// // 					find_flag1 = 1;
		// // 				}
		// // 				else if ( VecBusTab[jj].bs_type == BS_TYPE_FM)
		// // 				{
		// // 					m_pFcInf[ii].v_bs_id   = VecBusTab[jj].bs_id;
		// // 					find_flag2 = 1;
		// // 				}
		// // 				if ( find_flag1 == 1 && find_flag2 == 1) break;
		// 			}
		// 		}
		// 
		// 		m_pFcPmu[ii].fac_id   = m_pFcInf[ii].fac_id;
		// 		//m_pFcPmu[ii].m_bs_id  = m_pFcInf[ii].m_bs_id;
		// 		//m_pFcPmu[ii].v_bs_id  = m_pFcInf[ii].v_bs_id;
		// 		m_pFcDat[ii].fac_id   = m_pFcInf[ii].fac_id; 
	}

	// 	// UAV UAA UBV UBA UCV UCA V
	// 	vector<int> VecBusCol;
	// 	if(PODB_InitColID(VecBusCol,BS_DEVICE_NO_TAB)<0)
	// 	{
	// 		return false;
	// 	}
	// 
	// 	// set >> m_pFcPmu, m_pFcMon
	//     int     sign_V_FCNom[VEC_V_NUM];   // VEC_V_NUM == 6   
	//     int     sign_V_FCNov[VEC_V_NUM];   // VEC_V_NUM == 6   
	//     int     sign_f_FCNo ;             // 0 �޲�� 1 �в��
	// 	bool      isSetYC    = false;
	// 	bool      isSetYC_m  = false;
	// 	bool      isSetYC_v  = false;
	// 	//bool      isSetYC_f  = false;
	// 	INT_REC tmprecord_id;
	// 	int tmpcolumn_id;
	//     for(ii=0; ii < m_nFcNum; ii++)
	// 	{
	// 		m_pFcPmu[ii].vcurSts = MENU_LFO_DEV_STS_OK;
	// 		m_pFcPmu[ii].vhisSts = MENU_LFO_DEV_STS_OK;
	// 		m_pFcPmu[ii].fcurSts = MENU_LFO_DEV_STS_OK;
	// 		m_pFcPmu[ii].fhisSts = MENU_LFO_DEV_STS_OK;
	// 		m_pFcPmu[ii].vecFHZ.viFilNo = -1;
	// 		m_pFcPmu[ii].vecFHZ.viColNo = -1;
	// 		for ( jj=0; jj<VEC_V_NUM; jj++ )
	// 		{
	// 			m_pFcPmu[ii].m_bs_vecV.viFilNo[jj] = -1;
	// 			m_pFcPmu[ii].m_bs_vecV.viColNo[jj] = -1;
	// 			m_pFcPmu[ii].v_bs_vecV.viFilNo[jj] = -1;
	// 			m_pFcPmu[ii].v_bs_vecV.viColNo[jj] = -1;
	// 		}
	// 
	// 		memset((void *)sign_V_FCNom, 0, VEC_V_NUM * sizeof(int));
	// 		memset((void *)sign_V_FCNov, 0, VEC_V_NUM * sizeof(int));
	// 		sign_f_FCNo  = 0;
	// 		isSetYC_m  = false;
	// 		isSetYC_v  = false;
	// 		//isSetYC_f  = false;
	// 		for(jj=0; jj < (int)VecFesYcTab.size(); jj++)
	// 		{
	// 			if (para_IsFltSim == 1)
	// 			{
	// 				// ����ģ��������
	// 				if (VecFesYcTab[jj].dot_no < 0)
	// 				{
	// 					continue;
	// 				}
	// 			}
	// 			if (VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0 ) continue;
	// 			tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
	// 			tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
	// 			if( tmprecord_id  == m_pFcPmu[ii].m_bs_id )
	// 			{
	// 				// UAV UAA UBV UBA UCV UCA V
	// 				for (kk =0; kk<VEC_V_NUM; kk++)
	// 				{
	// 					if( tmpcolumn_id == VecBusCol[kk] )
	// 					{
	// 						sign_V_FCNom[kk]               = 1;
	// 						m_pFcPmu[ii].m_bs_vecV.viFilNo[kk] = VecFesYcTab[jj].file_no;
	// 						m_pFcPmu[ii].m_bs_vecV.viColNo[kk] = VecFesYcTab[jj].col_no;
	// 					}
	// 				}
	// 					
	// 				for ( kk=0; kk<VEC_V_NUM; kk++ )
	// 				{
	// 					if ( sign_V_FCNom[kk] == 0 )
	// 					{
	// 						break;
	// 					}
	// 				}
	// 				if (kk >= 1 )//ֻ��A���ѹ�����Ч����
	// 				{
	// 					isSetYC_m  = true;
	// 				}
	// 
	// 				if ( isSetYC_m == true && isSetYC_v  == true)
	// 				{
	// 					break ;
	// 				}
	// 				//if ( isSetYC_v == true && isSetYC_f == true)  break ;
	// 			}
	// 				//////////////////////////////////////////////////////////////////////////
	// 			if( tmprecord_id  == m_pFcPmu[ii].v_bs_id )
	// 			{
	// 				// UAV UAA UBV UBA UCV UCA V
	// 				for (kk =0; kk<VEC_V_NUM; kk++)
	// 				{
	// 					if( tmpcolumn_id == VecBusCol[kk] )
	// 					{
	// 						sign_V_FCNov[kk]               = 1;
	// 						m_pFcPmu[ii].v_bs_vecV.viFilNo[kk] = VecFesYcTab[jj].file_no;
	// 						m_pFcPmu[ii].v_bs_vecV.viColNo[kk] = VecFesYcTab[jj].col_no;
	// 					}
	// 				}
	// 				
	// 				for ( kk=0; kk<VEC_V_NUM; kk++ )
	// 				{
	// 					if ( sign_V_FCNov[kk] == 0 )
	// 					{
	// 						break;
	// 					}
	// 				}
	// 				if (kk >= 1 )//ֻ��A���ѹ�����Ч����
	// 				{
	// 					isSetYC_v  = true;
	// 				}
	// 				
	// 				if ( isSetYC_m == true && isSetYC_v  == true)
	// 				{
	// 					break ;
	// 				}
	// 				//if ( isSetYC_m == true && isSetYC_f == true)  break ;
	// 			}
	// 		}
	// 		
	// 		isSetYC    = true;
	// 		if ( isSetYC_m == false && isSetYC_v == false ) // no YC
	// 		{
	// 			m_pFcPmu[ii].vcurSts   = MENU_LFO_DEV_STS_NOYC;
	// 			m_pFcPmu[ii].vhisSts   = MENU_LFO_DEV_STS_NOYC;
	// 			m_pFcMon[ii].stat      = MENU_LFO_DEV_STS_NOYC;
	// 		}
	// 	}

	WritePmuInfo(LFO_FAC_NO_TAB);
#endif	
	return true;
}

// ��·��Ϣ
bool CLfoAnaObj::InitLn(vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<FES_YX_YC>& VecFesYcTab)
{
	map<INT_REC, int> map_acln_id;
	map<INT_REC, int>::iterator it;

#ifdef _WAMAP_
	if (!m_pLnInf || !m_pLnCtl || !m_pLnPmu || !m_pLnMon)
	{
		sprintf(m_LogString, "InitLn(): pointer >m_pLnInf >m_pLnCtl >m_pLnPmu >m_pLnMon no Alloc Memory");
		MessShow(m_LogString);
		return false;
	}

	vector<SSO_LN_TAB_CRE> VecLfoAclnTab;
	if (GetSsoAcln(VecLfoAclnTab) < 0)
	{
		return false;
	}

	vector<ACLINE_TAB_CRE> VecAclnTab;
	if (PODB_GetAclnInfo(VecAclnTab) < 0)
	{
		return false;
	}

	vector<ACLNDOT_TAB_CRE> VecDotTab;
	if (PODB_GetAclnDotInfo(VecDotTab) < 0)
	{
		return false;
	}

	vector<VL_TYPE_TAB_CRE> VecVLTypeTab;
	if (PODB_GetVLTypeInfo(VecVLTypeTab) < 0)
	{
		return false;
	}

	// set >> m_pLnInf, m_pLnCtl
	int ii, jj, kk, find_flag_1, find_flag_2;
	m_nLnNum = (int)VecLfoAclnTab.size();
	for (ii = 0; ii < (int)VecLfoAclnTab.size(); ii++)
	{
		m_pLnInf[ii].acln_idx = VecLfoAclnTab[ii].acln_idx;
		m_pLnInf[ii].acln_id = VecLfoAclnTab[ii].acln_id;
		m_pLnInf[ii].dot_id_1 = VecLfoAclnTab[ii].dot_id_1;
		m_pLnInf[ii].fac_id_1 = VecLfoAclnTab[ii].fac_id_1;
		m_pLnInf[ii].area_id_1 = VecLfoAclnTab[ii].area_id_1;
		m_pLnInf[ii].dot_id_2 = VecLfoAclnTab[ii].dot_id_2;
		m_pLnInf[ii].fac_id_2 = VecLfoAclnTab[ii].fac_id_2;
		m_pLnInf[ii].area_id_2 = VecLfoAclnTab[ii].area_id_2;

		m_pLnCtl[ii].acln_id = VecLfoAclnTab[ii].acln_id;
		m_pLnCtl[ii].is_end = VecLfoAclnTab[ii].is_end;
		for (jj = 0; jj < (int)VecAclnTab.size(); jj++)
		{
			if (m_pLnInf[ii].acln_id == VecAclnTab[jj].acln_id)
			{
				sprintf(m_pLnInf[ii].acln_name, "%s", VecAclnTab[jj].acln_name);
				m_pLnInf[ii].imax1 = VecAclnTab[jj].imax1;
				m_pLnInf[ii].vlty_id = VecAclnTab[jj].vlty_id;
				break;
			}
		}

		for (jj = 0; jj < (int)VecVLTypeTab.size(); jj++)
		{
			if (m_pLnInf[ii].vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				m_pLnInf[ii].vbase = VecVLTypeTab[jj].vbase;
				break;
			}
		}

		find_flag_1 = 0;
		find_flag_2 = 0;
		for (jj = 0; jj < (int)VecFacTab.size(); jj++)
		{
			if (m_pLnInf[ii].fac_id_1 == VecFacTab[jj].fac_id)
			{
				sprintf(m_pLnInf[ii].fac_name_1, "%s", VecFacTab[jj].fac_name);
				find_flag_1 = 1;
			}
			if (m_pLnInf[ii].fac_id_2 == VecFacTab[jj].fac_id)
			{
				sprintf(m_pLnInf[ii].fac_name_2, "%s", VecFacTab[jj].fac_name);
				find_flag_2 = 1;
			}
			if (find_flag_1 == 1 && find_flag_2 == 1)
			{
				break;
			}
		}
		find_flag_1 = 0;
		find_flag_2 = 0;
		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (m_pLnInf[ii].area_id_1 == VecAreaTab[jj].area_id)
			{
				sprintf(m_pLnInf[ii].area_name_1, "%s", VecAreaTab[jj].area_name);
				find_flag_1 = 1;
			}
			if (m_pLnInf[ii].area_id_2 == VecAreaTab[jj].area_id)
			{
				sprintf(m_pLnInf[ii].area_name_2, "%s", VecAreaTab[jj].area_name);
				find_flag_2 = 1;
			}
			if (find_flag_1 == 1 && find_flag_2 == 1)
			{
				break;
			}
		}

		m_pLnPmu[ii].ln_id = m_pLnInf[ii].acln_id;
		sprintf(m_pLnPmu[ii].ln_name, "%s", m_pLnInf[jj].acln_name);
		m_pLnPmu[ii].dot_id_1 = m_pLnInf[ii].dot_id_1;
		m_pLnPmu[ii].dot_id_2 = m_pLnInf[ii].dot_id_2;
		m_pLnDat[ii].ln_id = m_pLnInf[ii].acln_id;
		map_acln_id.insert(map<INT_REC, int>::value_type(m_pLnInf[ii].acln_id, ii));
	}

	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR
	vector<int> VecLnCol;
	if (PODB_InitColID(VecLnCol, ACLN_DOT_NO_TAB) < 0)
	{
		return false;
	}

	// set >> m_pGnPmu, m_pGnMon
	int     sign_VI_FCNo1[VEC_VI_NUM]; // VEC_VI_NUM == 12   
	int     sign_Ampl1, sign_Freq1, sign_STAT1, sign_PWR1;             // �׶� 0 �޲�� 1 �в��
	int     sign_VI_FCNo2[VEC_VI_NUM]; // VEC_VI_NUM == 12   
	int     sign_Ampl2, sign_Freq2, sign_STAT2, sign_PWR2;             // �׶� 0 �޲�� 1 �в��
	bool      isSetYC = false;
	bool      isSetYC_dot1 = false;
	bool      isSetYC_dot2 = false;
	INT_REC tmprecord_id;
	int tmpcolumn_id;
	for (ii = 0; ii < m_nLnNum; ii++)
	{
		m_pLnPmu[ii].curSts = MENU_LFO_DEV_STS_OK;
		m_pLnPmu[ii].hisSts = MENU_LFO_DEV_STS_OK;
		m_pLnPmu[ii].vecAmpl1.viFilNo = -1;
		m_pLnPmu[ii].vecFreq1.viColNo = -1;
		m_pLnPmu[ii].vecSSO1.viColNo = -1;
		m_pLnPmu[ii].vecAmpl2.viFilNo = -1;
		m_pLnPmu[ii].vecFreq2.viColNo = -1;
		m_pLnPmu[ii].vecSSO2.viColNo = -1;

		memset((void*)sign_VI_FCNo1, 0, VEC_VI_NUM * sizeof(int));
		memset((void*)sign_VI_FCNo2, 0, VEC_VI_NUM * sizeof(int));
		sign_Ampl1 = 0;
		sign_Freq1 = 0;
		sign_STAT1 = 0;
		sign_PWR1 = 0;
		sign_Ampl2 = 0;
		sign_Freq2 = 0;
		sign_STAT2 = 0;
		sign_PWR2 = 0;
		isSetYC = true;
		isSetYC_dot1 = false;
		isSetYC_dot2 = false;
		for (jj = 0; jj < (int)VecFesYcTab.size(); jj++)
		{
			if (para_IsFltSim == 1)
			{
				// ����ģ��������
				if (VecFesYcTab[jj].dot_no < 0)
				{
					continue;
				}
			}
			if (VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0) continue;
			tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
			tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
			//////////////////////////////////////////////////////////////////////////
			//  dot_id_1 �׶�
			if (tmprecord_id == m_pLnPmu[ii].dot_id_1)
			{
				if (tmpcolumn_id == VecLnCol[0])//��ֵ���
				{
					sign_Ampl1 = 1;
					m_pLnPmu[ii].vecAmpl1.viFilNo = VecFesYcTab[jj].file_no;
					m_pLnPmu[ii].vecAmpl1.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecLnCol[1])//Ƶ�ʲ��
				{
					sign_Freq1 = 1;
					m_pLnPmu[ii].vecFreq1.viFilNo = VecFesYcTab[jj].file_no;
					m_pLnPmu[ii].vecFreq1.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecLnCol[2])//״̬���
				{
					sign_STAT1 = 1;
					m_pLnPmu[ii].vecSSO1.viFilNo = VecFesYcTab[jj].file_no;
					m_pLnPmu[ii].vecSSO1.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecLnCol[3])//״̬���
				{
					sign_PWR1 = 1;
					m_pLnPmu[ii].vecPwr1.viFilNo = VecFesYcTab[jj].file_no;
					m_pLnPmu[ii].vecPwr1.viColNo = VecFesYcTab[jj].col_no;
				}
				if (sign_Ampl1 == 1 && sign_Freq1 == 1 && sign_STAT1 == 1 && sign_PWR1 == 1)//�ж��׶˷�ֵƵ�ʲ���Ƿ���Ч
				{
					isSetYC_dot1 = true;
				}

				if (isSetYC_dot1 == true && isSetYC_dot2 == true)
				{
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			//  dot_id_2ĩ��
			if (tmprecord_id == m_pLnPmu[ii].dot_id_2)
			{
				if (tmpcolumn_id == VecLnCol[0])//��ֵ���
				{
					sign_Ampl2 = 1;
					m_pLnPmu[ii].vecAmpl2.viFilNo = VecFesYcTab[jj].file_no;
					m_pLnPmu[ii].vecAmpl2.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecLnCol[1])//Ƶ�ʲ��
				{
					sign_Freq2 = 1;
					m_pLnPmu[ii].vecFreq2.viFilNo = VecFesYcTab[jj].file_no;
					m_pLnPmu[ii].vecFreq2.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecLnCol[2])//״̬���
				{
					sign_STAT2 = 1;
					m_pLnPmu[ii].vecSSO2.viFilNo = VecFesYcTab[jj].file_no;
					m_pLnPmu[ii].vecSSO2.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecLnCol[3])//״̬���
				{
					sign_PWR2 = 1;
					m_pLnPmu[ii].vecPwr2.viFilNo = VecFesYcTab[jj].file_no;
					m_pLnPmu[ii].vecPwr2.viColNo = VecFesYcTab[jj].col_no;
				}
				if (sign_Ampl2 == 1 && sign_Freq2 == 1 && sign_STAT2 == 1 && sign_PWR2 == 1)//�ж�ĩ�˷�ֵƵ�ʲ���Ƿ���Ч
				{
					isSetYC_dot2 = true;
				}

				if (isSetYC_dot1 == true && isSetYC_dot2 == true)
				{
					break;
				}
			}
		}


		if (isSetYC_dot1 == false && isSetYC_dot2 == false)
		{
			m_pLnPmu[ii].curSts = MENU_LFO_DEV_STS_NOYC;
			m_pLnPmu[ii].hisSts = MENU_LFO_DEV_STS_NOYC;
			m_pLnMon[ii].stat = MENU_LFO_DEV_STS_NOYC;
			m_pLnMon[ii].stat_val = MENU_LFO_DEV_STS_NOYC;
		}
	}
	for (ii = 0; ii < VecDotTab.size(); ii++)
	{
		it = map_acln_id.begin();
		it = map_acln_id.find(VecDotTab[ii].acln_id);
		if (it != map_acln_id.end())
		{
			g_map_ac_dot.insert(multimap<INT_REC, INT_REC>::value_type(VecDotTab[ii].acln_id, VecDotTab[ii].acln_dot_id));
			g_map_acln_dot.insert(map<INT_REC, int>::value_type(VecDotTab[ii].acln_dot_id, ii));
		}
	}
	// 	m_vAcDot_id.clear();
	// 	m_vAcDot_id.push_back(116812116173783720);
	// 	m_vAcDot_id.push_back(116812116173783721);
	// 	m_vAcDot_id.push_back(116812116173783722);
	// 	m_vAcDot_id.push_back(116812116173783723);
	// 	m_vAcDot_id.push_back(116812116173783724);
	// 	m_vAcDot_id.push_back(116812116173783725);
	// 	m_vAcDot_id.push_back(116812116173783726);
	// 	m_vAcDot_id.push_back(116812116173783727);
	// 	m_vAcDot_id.push_back(116812116173784530);
	// 	m_vAcDot_id.push_back(116812116173784531);
	// 	m_vAcDot_id.push_back(116812116173784532);
	// 	m_vAcDot_id.push_back(116812116173784533);
	// 	m_vAcDot_id.push_back(116812116173783484);
	// 	m_vAcDot_id.push_back(116812116173783485);
	// 	m_vAcDot_id.push_back(116812116173783486);
	// 	m_vAcDot_id.push_back(116812116173783487);
	// 	m_vAcDot_id.push_back(116812116173783413);
	// 	m_vAcDot_id.push_back(116812116173783414);
	// 	m_vAcDot_id.push_back(116812116173783488);
	// 	m_vAcDot_id.push_back(116812116173783489);
	// 	m_vAcDot_id.push_back(116812116173784517);
	// 	m_vAcDot_id.push_back(116812116173784519);
	// 	m_vAcDot_id.push_back(116812115351699843);
	// 	m_vAcDot_id.push_back(116812115351699845);
	// 	m_vAcDot_id.push_back(116812116173784513);
	// 	m_vAcDot_id.push_back(116812116173784515);



	WritePmuInfo(LFO_ACLN_NO_TAB);

#endif	

	return true;
}

// ������Ϣ
bool CLfoAnaObj::InitGn(vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<FES_YX_YC>& VecFesYcTab)
{
#ifdef _WAMAP_
	if (!m_pGnInf || !m_pGnCtl || !m_pGnPmu || !m_pGnMon)
	{
		sprintf(m_LogString, "InitGn(): pointer >m_pGnInf >m_pGnCtl >m_pGnPmu >m_pGnMon no Alloc Memory");
		MessShow(m_LogString);
		return false;
	}

	vector<SSO_GN_TAB_CRE> VecLfoGenTab;
	if (GetSsoGen(VecLfoGenTab) < 0)
	{
		return false;
	}

	vector<GEN_TAB_CRE> VecGenTab;
	if (PODB_GetGenInfo(VecGenTab) < 0)
	{
		return false;
	}

	// set >> m_pGnInf, m_pGnCtl
	int ii, jj, kk;
	m_nGnNum = (int)VecLfoGenTab.size();
	for (ii = 0; ii < (int)VecLfoGenTab.size(); ii++)
	{
		m_pGnInf[ii].gn_idx = VecLfoGenTab[ii].gn_idx;
		m_pGnInf[ii].gn_id = VecLfoGenTab[ii].gn_id;
		m_pGnInf[ii].fac_id = VecLfoGenTab[ii].station_id;
		m_pGnInf[ii].area_id = VecLfoGenTab[ii].area_id;

		m_pGnCtl[ii].gn_id = VecLfoGenTab[ii].gn_id;
		m_pGnCtl[ii].is_monitor = VecLfoGenTab[ii].is_monitor;

		g_map_gn_fac.insert(multimap<INT_REC, int>::value_type(m_pGnInf[ii].fac_id, ii));

		for (jj = 0; jj < (int)VecFacTab.size(); jj++)
		{
			if (m_pGnInf[ii].fac_id == VecFacTab[jj].fac_id)
			{
				sprintf(m_pGnInf[ii].fac_name, "%s", VecFacTab[jj].fac_name);
				break;
			}
		}
		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (m_pGnInf[ii].area_id == VecAreaTab[jj].area_id)
			{
				sprintf(m_pGnInf[ii].area_name, "%s", VecAreaTab[jj].area_name);
				break;
			}
		}

		for (jj = 0; jj < (int)VecGenTab.size(); jj++)
		{
			if (m_pGnInf[ii].gn_id == VecGenTab[jj].gn_id)
			{
				sprintf(m_pGnInf[ii].gn_name, "%s", VecGenTab[jj].gn_name);
#ifndef _NUSP
				m_pGnInf[ii].pnom = VecGenTab[jj].pnom;
#endif // !_NUSP
				m_pGnInf[ii].pmin = VecGenTab[jj].pmin;
				m_pGnInf[ii].pmax = VecGenTab[jj].pmax;
#ifndef _NUSP
				if (m_pGnInf[ii].pmin < m_pGnInf[ii].pnom * 0.1) m_pGnInf[ii].pmin = (float)(m_pGnInf[ii].pnom * 0.1);
				if (m_pGnInf[ii].pmax < m_pGnInf[ii].pnom * 1.0) m_pGnInf[ii].pmax = (float)(m_pGnInf[ii].pnom * 1.0);
#endif // !_NUSP				
				break;
			}
		}

		m_pGnPmu[ii].gn_id = m_pGnInf[ii].gn_id;
		sprintf(m_pGnPmu[ii].gn_name, "%s", m_pGnInf[jj].fac_name);
		strcat(m_pGnPmu[ii].gn_name, m_pGnInf[jj].gn_name);
		m_pGnDat[ii].gn_id = m_pGnInf[ii].gn_id;
	}

	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
	vector<int> VecGenCol;
	if (PODB_InitColID(VecGenCol, GN_DEVICE_NO_TAB) < 0)
	{
		return false;
	}

	// set >> m_pGnPmu, m_pGnMon
	int     sign_VI_FCNo[VEC_VI_NUM]; // VEC_VI_NUM == 12   
	int     sign_Ampl_FCNo;             // 0 �޲�� 1 �в��
	int     sign_Freq_FCNo;
	int     sign_Stat_FCNo;
	int     sign_PWR;
	bool    isSetYC = false;
	INT_REC tmprecord_id;
	int tmpcolumn_id;
	for (ii = 0; ii < m_nGnNum; ii++)
	{
		m_pGnPmu[ii].curSts = MENU_LFO_DEV_STS_OK;
		m_pGnPmu[ii].hisSts = MENU_LFO_DEV_STS_OK;
		m_pGnPmu[ii].vecAmpl.viFilNo = -1;
		m_pGnPmu[ii].vecFreq.viColNo = -1;
		m_pGnPmu[ii].vecSSO.viFilNo = -1;

		memset((void*)sign_VI_FCNo, 0, VEC_VI_NUM * sizeof(int));
		sign_Ampl_FCNo = 0;
		sign_Freq_FCNo = 0;
		sign_Stat_FCNo = 0;
		sign_PWR = 0;
		isSetYC = false;
		for (jj = 0; jj < (int)VecFesYcTab.size(); jj++)
		{
			if (para_IsFltSim == 1)
			{
				// ����ģ��������
				if (VecFesYcTab[jj].dot_no < 0)
				{
					continue;
				}
			}
			if (VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0) continue;
			tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
			tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
			if (tmprecord_id == m_pGnPmu[ii].gn_id)
			{
				if (tmpcolumn_id == VecGenCol[0])
				{
					// ��ֵ���
					sign_Ampl_FCNo = 1;
					m_pGnPmu[ii].vecAmpl.viFilNo = VecFesYcTab[jj].file_no;
					m_pGnPmu[ii].vecAmpl.viColNo = VecFesYcTab[jj].col_no;
				}
				else if (tmpcolumn_id == VecGenCol[1])
				{
					// Ƶ�ʲ��
					sign_Freq_FCNo = 1;
					m_pGnPmu[ii].vecFreq.viFilNo = VecFesYcTab[jj].file_no;
					m_pGnPmu[ii].vecFreq.viColNo = VecFesYcTab[jj].col_no;
				}
				else if (tmpcolumn_id == VecGenCol[2])
				{
					// ״̬λ���
					sign_Stat_FCNo = 1;
					m_pGnPmu[ii].vecSSO.viFilNo = VecFesYcTab[jj].file_no;
					m_pGnPmu[ii].vecSSO.viColNo = VecFesYcTab[jj].col_no;
				}
				else if (tmpcolumn_id == VecGenCol[3])
				{
					// ״̬λ���
					sign_PWR = 1;
					m_pGnPmu[ii].vecPwr.viFilNo = VecFesYcTab[jj].file_no;
					m_pGnPmu[ii].vecPwr.viColNo = VecFesYcTab[jj].col_no;
				}
				if (sign_Ampl_FCNo == 1 && sign_Freq_FCNo == 1 && sign_Stat_FCNo == 1 && sign_PWR == 1)
				{
					isSetYC = true;
					break;
				}
			}
		}

		if (isSetYC == false) // no YC
		{
			m_pGnPmu[ii].curSts = MENU_LFO_DEV_STS_NOYC;
			m_pGnPmu[ii].hisSts = MENU_LFO_DEV_STS_NOYC;
			m_pGnMon[ii].stat = MENU_LFO_DEV_STS_NOYC;
			m_pGnMon[ii].stat_val = MENU_LFO_DEV_STS_NOYC;
		}
	}

	WritePmuInfo(LFO_GN_NO_TAB);

#endif	

	return true;
}

// ������Ϣ(����·���)
bool CLfoAnaObj::InitItfc(vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<FES_YC_TAB_CRE>& VecFesYcTab)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��ѹ����Ϣ
bool CLfoAnaObj::InitTr(vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<FES_YX_YC>& VecFesYcTab)
{
#ifdef _WAMAP_
	if (!m_pTrInf || !m_pTrCtl || !m_pTrPmu || !m_pTrMon)
	{
		sprintf(m_LogString, "InitTr(): pointer >m_pTrInf >m_pTrCtl >m_pTrPmu >m_pTrMon no Alloc Memory");
		MessShow(m_LogString);
		return false;
	}

	vector<SSO_TR_TAB_CRE> VecLfoTrTab;
	if (GetSsoTr(VecLfoTrTab) < 0)
	{
		return false;
	}

	vector<TR_TAB_CRE> VecTrTab;
	if (PODB_GetTrInfo(VecTrTab) < 0)
	{
		return false;
	}

	vector<TRWD_TAB_CRE> VecTrwdTab;
	if (PODB_GetTrwdInfo(VecTrwdTab) < 0)
	{
		return false;
	}

	vector<VL_TYPE_TAB_CRE> VecVLTypeTab;
	if (PODB_GetVLTypeInfo(VecVLTypeTab) < 0)
	{
		return false;
	}

	// set >> m_pTrInf, m_pTrCtl
	int ii, jj, kk, mm;
	float pwr = -1.0f;
	m_nTrNum = (int)VecLfoTrTab.size();
	for (ii = 0; ii < (int)VecLfoTrTab.size(); ii++)
	{
		m_pTrInf[ii].tr_idx = VecLfoTrTab[ii].tr_idx;
		m_pTrInf[ii].tr_id = VecLfoTrTab[ii].tr_id;
		m_pTrInf[ii].high_wind_id = VecLfoTrTab[ii].high_wind_id;
		m_pTrInf[ii].high_vlty_id = VecLfoTrTab[ii].high_vlty_id;
		m_pTrInf[ii].mid_wind_id = VecLfoTrTab[ii].mid_wind_id;
		m_pTrInf[ii].mid_vlty_id = VecLfoTrTab[ii].mid_vlty_id;
		m_pTrInf[ii].low_wind_id = VecLfoTrTab[ii].low_wind_id;
		m_pTrInf[ii].low_vlty_id = VecLfoTrTab[ii].low_vlty_id;
		m_pTrInf[ii].fac_id = VecLfoTrTab[ii].station_id;
		m_pTrInf[ii].area_id = VecLfoTrTab[ii].area_id;

		g_map_tr_fac.insert(multimap<INT_REC, int>::value_type(m_pTrInf[ii].fac_id, ii));

		m_pTrCtl[ii].tr_id = VecLfoTrTab[ii].tr_id;
		m_pTrCtl[ii].is_monitor = VecLfoTrTab[ii].is_monitor;
		//         m_pTrCtl[ii].is_part_ana = VecLfoTrTab[ii].is_part_ana;
		//         m_pTrCtl[ii].is_pow_calc = VecLfoTrTab[ii].is_pow_calc;
		//         m_pTrCtl[ii].mon_wind_slct = VecLfoTrTab[ii].mon_wind_slct;

		for (jj = 0; jj < (int)VecTrTab.size(); jj++)
		{
			if (m_pTrInf[ii].tr_id == VecTrTab[jj].tr_id)
			{
				sprintf(m_pTrInf[ii].tr_name, "%s", VecTrTab[jj].tr_name);
				break;
			}
		}

		for (jj = 0; jj < (int)VecFacTab.size(); jj++)
		{
			if (m_pTrInf[ii].fac_id == VecFacTab[jj].fac_id)
			{
				sprintf(m_pTrInf[ii].fac_name, "%s", VecFacTab[jj].fac_name);
				break;
			}
		}

		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (m_pTrInf[ii].area_id == VecAreaTab[jj].area_id)
			{
				sprintf(m_pTrInf[ii].area_name, "%s", VecAreaTab[jj].area_name);
				break;
			}
		}

		for (jj = 0; jj < (int)VecVLTypeTab.size(); jj++)
		{
			if (m_pTrInf[ii].high_vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				m_pTrInf[ii].vhigh = VecVLTypeTab[jj].vbase;
			}
			if (m_pTrInf[ii].mid_vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				m_pTrInf[ii].vmid = VecVLTypeTab[jj].vbase;
			}
			if (m_pTrInf[ii].low_vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				m_pTrInf[ii].vlow = VecVLTypeTab[jj].vbase;
			}
		}

		for (mm = 0; mm < (int)VecTrwdTab.size(); mm++)  //��ȡ��ѹ������� 20141008
		{
			if (VecTrwdTab[mm].tr_id == m_pTrInf[ii].tr_id)
			{
				pwr = VecTrwdTab[mm].snom;
			}
			if (m_pTrInf[ii].max_pwr < pwr)
			{
				m_pTrInf[ii].max_pwr = pwr;
			}
		}

		m_pTrPmu[ii].tr_id = m_pTrInf[ii].tr_id;
		sprintf(m_pTrPmu[ii].tr_name, "%s", m_pTrInf[ii].tr_name);
		m_pTrPmu[ii].high_wind_id = m_pTrInf[ii].high_wind_id;
		m_pTrPmu[ii].mid_wind_id = m_pTrInf[ii].mid_wind_id;
		m_pTrPmu[ii].low_wind_id = m_pTrInf[ii].low_wind_id;

		m_pTrDat[ii].tr_id = m_pTrInf[ii].tr_id;
	}

	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR
	vector<int> VecTrCol;
	if (PODB_InitColID(VecTrCol, TRWD_DEVICE_NO_TAB) < 0)
	{
		return false;
	}

	// set >> m_pTrPmu, m_pTrMon
	int sign_FCNo_high_Ampl, sign_FCNo_high_Freq, sign_FCNo_high_Stat, sign_FCNo_high_Pwr;
	int sign_FCNo_mid_Ampl, sign_FCNo_mid_Freq, sign_FCNo_mid_Stat, sign_FCNo_mid_Pwr;
	int sign_FCNo_low_Ampl, sign_FCNo_low_Freq, sign_FCNo_low_Stat, sign_FCNo_low_Pwr;
	bool      isSetYC = false;
	bool      isSetYC_high = false;
	bool      isSetYC_mid = false;
	bool      isSetYC_low = false;
	INT_REC tmprecord_id;
	int tmpcolumn_id;
	for (ii = 0; ii < m_nTrNum; ii++)
	{
		m_pTrPmu[ii].curSts = MENU_LFO_DEV_STS_OK;
		m_pTrPmu[ii].hisSts = MENU_LFO_DEV_STS_OK;
		m_pTrPmu[ii].vecAmpl_high.viFilNo = -1;
		m_pTrPmu[ii].vecFreq_high.viColNo = -1;
		m_pTrPmu[ii].vecSSO_high.viFilNo = -1;
		m_pTrPmu[ii].vecAmpl_mid.viColNo = -1;
		m_pTrPmu[ii].vecFreq_mid.viFilNo = -1;
		m_pTrPmu[ii].vecSSO_mid.viColNo = -1;
		m_pTrPmu[ii].vecAmpl_low.viColNo = -1;
		m_pTrPmu[ii].vecFreq_low.viFilNo = -1;
		m_pTrPmu[ii].vecSSO_low.viColNo = -1;

		sign_FCNo_high_Ampl = 0;
		sign_FCNo_high_Freq = 0;
		sign_FCNo_high_Stat = 0;
		sign_FCNo_mid_Ampl = 0;
		sign_FCNo_mid_Freq = 0;
		sign_FCNo_mid_Stat = 0;
		sign_FCNo_low_Ampl = 0;
		sign_FCNo_low_Freq = 0;
		sign_FCNo_low_Stat = 0;

		isSetYC = true;
		isSetYC_high = false;
		isSetYC_mid = false;
		isSetYC_low = false;

		for (jj = 0; jj < (int)VecFesYcTab.size(); jj++)
		{
			if (para_IsFltSim == 1)
			{
				// ����ģ��������
				if (VecFesYcTab[jj].dot_no < 0)
				{
					continue;
				}
			}
			if (VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0) continue;
			tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
			tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
			//  ��ѹ��
			if (tmprecord_id == m_pTrPmu[ii].high_wind_id)
			{
				// ��ֵ����
				if (tmpcolumn_id == VecTrCol[0])
				{
					sign_FCNo_high_Ampl = 1;
					m_pTrPmu[ii].vecAmpl_high.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecAmpl_high.viColNo = VecFesYcTab[jj].col_no;
				}
				// Ƶ������
				if (tmpcolumn_id == VecTrCol[1])
				{
					sign_FCNo_high_Freq = 1;
					m_pTrPmu[ii].vecFreq_high.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecFreq_high.viColNo = VecFesYcTab[jj].col_no;
				}
				// ״̬����
				if (tmpcolumn_id == VecTrCol[2])
				{
					sign_FCNo_high_Stat = 1;
					m_pTrPmu[ii].vecSSO_high.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecSSO_high.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecTrCol[3])
				{
					sign_FCNo_high_Pwr = 1;
					m_pTrPmu[ii].vecPwr_high.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecPwr_high.viColNo = VecFesYcTab[jj].col_no;
				}

				if (sign_FCNo_high_Ampl == 1 && sign_FCNo_high_Freq == 1 && sign_FCNo_high_Stat == 1 && sign_FCNo_high_Pwr == 1)//
				{
					isSetYC_high = true;
					m_pTrPmu[ii].hwd_yc_ok = MENU_LFO_MENU_YES;
				}

				if (isSetYC_high == true && isSetYC_mid == true && isSetYC_low == true)
				{
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			//  ��ѹ��
			if (VALID_INT_REC(m_pTrPmu[ii].mid_wind_id) && tmprecord_id == m_pTrPmu[ii].mid_wind_id)
			{
				// ��ֵ����
				if (tmpcolumn_id == VecTrCol[0])
				{
					sign_FCNo_mid_Ampl = 1;
					m_pTrPmu[ii].vecAmpl_mid.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecAmpl_mid.viColNo = VecFesYcTab[jj].col_no;
				}
				// Ƶ������
				if (tmpcolumn_id == VecTrCol[1])
				{
					sign_FCNo_mid_Freq = 1;
					m_pTrPmu[ii].vecFreq_mid.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecFreq_mid.viColNo = VecFesYcTab[jj].col_no;
				}
				// ״̬����
				if (tmpcolumn_id == VecTrCol[2])
				{
					sign_FCNo_mid_Stat = 1;
					m_pTrPmu[ii].vecSSO_mid.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecSSO_mid.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecTrCol[3])
				{
					sign_FCNo_mid_Pwr = 1;
					m_pTrPmu[ii].vecPwr_mid.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecPwr_mid.viColNo = VecFesYcTab[jj].col_no;
				}
				if (sign_FCNo_mid_Ampl == 1 && sign_FCNo_mid_Freq == 1 && sign_FCNo_mid_Stat == 1 && sign_FCNo_mid_Pwr)//
				{
					isSetYC_mid = true;
					m_pTrPmu[ii].mwd_yc_ok = MENU_LFO_MENU_YES;
				}

				if (isSetYC_high == true && isSetYC_mid == true && isSetYC_low == true)
				{
					break;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			//  ��ѹ��
			if (tmprecord_id == m_pTrPmu[ii].low_wind_id)
			{
				// ��ֵ����
				if (tmpcolumn_id == VecTrCol[0])
				{
					sign_FCNo_low_Ampl = 1;
					m_pTrPmu[ii].vecAmpl_low.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecAmpl_low.viColNo = VecFesYcTab[jj].col_no;
				}
				// Ƶ������
				if (tmpcolumn_id == VecTrCol[1])
				{
					sign_FCNo_low_Freq = 1;
					m_pTrPmu[ii].vecFreq_low.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecFreq_low.viColNo = VecFesYcTab[jj].col_no;
				}
				// ״̬����
				if (tmpcolumn_id == VecTrCol[2])
				{
					sign_FCNo_low_Stat = 1;
					m_pTrPmu[ii].vecSSO_low.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecSSO_low.viColNo = VecFesYcTab[jj].col_no;
				}
				if (tmpcolumn_id == VecTrCol[3])
				{
					sign_FCNo_low_Pwr = 1;
					m_pTrPmu[ii].vecPwr_low.viFilNo = VecFesYcTab[jj].file_no;
					m_pTrPmu[ii].vecPwr_low.viColNo = VecFesYcTab[jj].col_no;
				}
				if (sign_FCNo_low_Ampl == 1 && sign_FCNo_low_Freq == 1 && sign_FCNo_low_Stat == 1 && sign_FCNo_low_Pwr == 1)//
				{
					isSetYC_low = true;
					m_pTrPmu[ii].lwd_yc_ok = MENU_LFO_MENU_YES;
				}
			}

			if (m_pTrInf[ii].trty == THREE_WINDING)
			{
				if (isSetYC_high == true && isSetYC_mid == true && isSetYC_low == true)
				{
					break;
				}
			}
			else if (m_pTrInf[ii].trty == TWO_WINDING)
			{
				if (isSetYC_high == true && isSetYC_low == true)
				{
					break;
				}
			}
		}

		if (isSetYC_high == false && isSetYC_mid == false && isSetYC_low == false) // no YC
		{
			m_pTrPmu[ii].curSts = MENU_LFO_DEV_STS_NOYC;
			m_pTrPmu[ii].hisSts = MENU_LFO_DEV_STS_NOYC;
			m_pTrMon[ii].stat = MENU_LFO_DEV_STS_NOYC;
			m_pTrMon[ii].stat_val = MENU_LFO_DEV_STS_NOYC;
		}
	}

	WritePmuInfo(LFO_TR_NO_TAB);

#endif	

	return true;
}
//////////////////////////////////////////////////////////////////////////
// 2008-05-11 add by zhw
// ����KeyId����Ż�ȡPMU�����ļ����к�
// bool CLfoAnaObj::GetPmuFilColNobyKeyId( KEY_ID_STRU ikeyid, short &filNo, short &colNo )
// {
// #ifdef _WAMAP_
// 	if (ikeyid.record_id <= 1) return false;
// 
// 	//WAMSǰ��ң�ⶨ���
//     CBuffer                buf_base;
//     int                    TabBufSize = 0;
// 
// 	FES_YC_TAB_CRE *       pFesycInf = NULL;
//     int                    nFesycInf = 0;
// 	
//     if ( g_IDBTableOp.Open(LFO_AP_ID, WAM_FES_YC_DEFINE_NO, g_ctxNo)  == DB_OK )
// 	{
// 		if ( g_IDBTableOp.TableGet( FES_YC_TAB_CRE_FLD, buf_base ) > 0 )
// 		{
// 			TabBufSize = sizeof(FES_YC_TAB_CRE);
// 			nFesycInf = buf_base.GetLength()/TabBufSize;
// 			pFesycInf = (FES_YC_TAB_CRE *)calloc(nFesycInf, sizeof(FES_YC_TAB_CRE));
// 			memset((void *)pFesycInf, 0, nFesycInf * sizeof(FES_YC_TAB_CRE));
// 			memcpy((void*)pFesycInf, (void*)buf_base.GetBufPtr(), TabBufSize*nFesycInf);
// 		}
// 	}
//     if( nFesycInf <= 0 )
// 	{
// 		sprintf( m_LogString, "GetPmuFilColNobyKeyId(): WAM_FES_YC_DEFINE_NO[%d] no record ",  WAM_FES_YC_DEFINE_NO ); 
// 		WriteLogFile( m_LogString );
// 
// 		if( pFesycInf )  {free(pFesycInf);  pFesycInf = NULL;}
// 		return false;
// 	}
// 	else
// 	{
// 		for (int ii=0; ii<nFesycInf; ii++)
// 		{
// 			if ( pFesycInf[ii].yc_id.record_id == ikeyid.record_id && pFesycInf[ii].yc_id.column_id == ikeyid.column_id )
// 			{
// 				filNo = pFesycInf[ii].file_no;
// 				colNo = pFesycInf[ii].col_no;
// 				break;
// 			}
// 		}
// 	}
// 
// 	if( pFesycInf )  {free(pFesycInf);  pFesycInf = NULL;}
// #endif
// 	return true ;
// }

// 2008-05-11 add by zhw
// ����������ý�����·��Ϣ���й����ʵ��ļ����к�
void CLfoAnaObj::SetPmuFilColNoForTest(int ac_dot_id, int COL_ID, int nLnNum, SSO_LN_PMU* pLnPmu, int GN_COL_ID, int nGnNum, SSO_GN_PMU* pGnPmu, short ifilNo, short icolNo, int isForceSet)
{
#ifndef _D5000
	if (nLnNum <= 0 || !pLnPmu) return;
	if (ac_dot_id <= 0) return;

	short      filNo = -1;
	short      colNo = -1;
	MY_KEY_ID  Mykeyid;
	int jj;
	sprintf(m_LogString, "ac_dot_id=%d,ifilno=%d,icolno=%d", ac_dot_id, ifilNo, icolNo);
	MessShow(m_LogString);
	for (jj = 0; jj < nLnNum; jj++)
	{
		if (pLnPmu[jj].dot_id_1 == ac_dot_id)
		{
			if (isForceSet == 0)
			{
				Mykeyid.record_id = ac_dot_id;
				Mykeyid.column_id = COL_ID;
				if (PWD_GetAppFieldByKeyId(filNo, colNo, Mykeyid) == true)
				{
					pLnPmu[jj].vecPwr1.viFilNo = filNo;
					pLnPmu[jj].vecPwr1.viColNo = colNo;
					pLnPmu[jj].is_end = MENU_LFO_MENU_NO;
				}
			}
			else
			{
				pLnPmu[jj].vecPwr1.viFilNo = ifilNo;
				pLnPmu[jj].vecPwr1.viColNo = icolNo;
				pLnPmu[jj].is_end = MENU_LFO_MENU_NO;
			}
			sprintf(m_LogString, "SetPmuFilColNoForTest(): Change AC line %s["INT_REC_FMT"]["INT_REC_FMT"] data source to file[%d] col[%d], success!", pLnPmu[jj].acln_name, INT_REC_VAL(pLnPmu[jj].acln_id), INT_REC_VAL(pLnPmu[jj].dot_id_1), pLnPmu[jj].vecPwr1.viFilNo, pLnPmu[jj].vecPwr1.viColNo);
			MessShow(m_LogString);
			break;
		}
		if (pLnPmu[jj].dot_id_2 == ac_dot_id)
		{
			if (isForceSet == 0)
			{
				Mykeyid.record_id = ac_dot_id;
				Mykeyid.column_id = COL_ID;
				if (PWD_GetAppFieldByKeyId(filNo, colNo, Mykeyid) == true)
				{
					pLnPmu[jj].vecPwr2.viFilNo = filNo;
					pLnPmu[jj].vecPwr2.viColNo = colNo;
					pLnPmu[jj].is_end = MENU_LFO_MENU_YES;
				}
			}
			else
			{
				pLnPmu[jj].vecPwr2.viFilNo = ifilNo;
				pLnPmu[jj].vecPwr2.viColNo = icolNo;
				pLnPmu[jj].is_end = MENU_LFO_MENU_YES;
			}
			sprintf(m_LogString, "SetPmuFilColNoForTest(): Change AC line %s["INT_REC_FMT"]["INT_REC_FMT"] data source to file[%d] col[%d], success!", pLnPmu[jj].acln_name, INT_REC_VAL(pLnPmu[jj].acln_id), INT_REC_VAL(pLnPmu[jj].dot_id_2), pLnPmu[jj].vecPwr2.viFilNo, pLnPmu[jj].vecPwr2.viColNo);
			MessShow(m_LogString);
			break;
		}
	}

	for (jj = 0; jj < nGnNum; jj++)
	{
		if (pGnPmu[jj].gn_id == ac_dot_id)
		{
			if (isForceSet == 0)
			{
				Mykeyid.record_id = ac_dot_id;
				Mykeyid.column_id = GN_COL_ID;
				if (PWD_GetAppFieldByKeyId(filNo, colNo, Mykeyid) == true)
				{
					pGnPmu[jj].vecPwr.viFilNo = filNo;
					pGnPmu[jj].vecPwr.viColNo = colNo;
				}
			}
			else
			{
				pGnPmu[jj].vecPwr.viFilNo = ifilNo;
				pGnPmu[jj].vecPwr.viColNo = icolNo;
			}
			sprintf(m_LogString, "SetPmuFilColNoForTest(): Change Gn %s["INT_REC_FMT"] data source to file[%d] col[%d], success!", pGnPmu[jj].gn_name, INT_REC_VAL(pGnPmu[jj].gn_id), pGnPmu[jj].vecPwr.viFilNo, pGnPmu[jj].vecPwr.viColNo);
			MessShow(m_LogString);
			break;
		}
	}
#endif
	return;
}


///////////////////////////////// ���ݴ��� /////////////////////////////////
// ��һ�ּ���ǰ��ʼ��
void CLfoAnaObj::InitialLFOEstimate()
{
	int  ii;
	// ��ǲο���
	//KEY_ID_STRU          m_AngRefID;                        //  �ο����id
	//float                m_AngRefDat[DATA_NUM_MAX];          //  �ο��������
// 	sprintf( m_LogString, "InitialLFOEstimate(): Ӧ�ó�ʼ��!" ); 
// 	MessShow( m_LogString );

	m_CalcStartTime = 0;                         //  ��ǰ�۲촰����ʼGPSʱ�� 
	if (g_ctxNo != PUB_AC_RESEARCH_NO)
	{
		m_CurGpsTime = 0;                         //  ��ǰGPSʱ��(��ֹGPSʱ��)
	}

	m_nOrigDlt = 0;					        //  ԭʼ��������
	m_nOrigSam = 0;					        //  ԭʼ���������
	m_nSamples = 0;					        //  ʵ�ʲ��������
	m_nWarnWdsnum = 0;
	m_fAverFreq = 0.0;
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��ʵʱ����״̬
	for (ii = 0; ii < m_nFcNum; ii++)
	{
		m_pFcCtl[ii].is_monitor = MENU_LFO_MON_YES;
		m_pFcCtl[ii].is_part_ana = MENU_LFO_MON_YES;
		if (m_pFcMon[ii].stat != MENU_LFO_DEV_STS_NOYC)
		{
			memset((void*)&m_pFcMon[ii], 0, sizeof(SSO_FC_MON));
			m_pFcMon[ii].fac_id = m_pFcInf[ii].fac_id;
			m_pFcMon[ii].stat = MENU_LFO_DEV_STS_OK;
			//             m_pFcMon[ii].slct_bus_id = m_pFcInf[ii].m_bs_id;  //2008-04-05 added by zhw at yn
		}
	}
	for (ii = 0; ii < m_nGnNum; ii++)
	{
		m_pGnCtl[ii].is_monitor = MENU_LFO_MON_YES;
		// 		m_pGnCtl[ii].is_part_ana = MENU_LFO_MON_YES;
		if (m_pGnMon[ii].stat != MENU_LFO_DEV_STS_NOYC)
		{
			memset((void*)&m_pGnMon[ii], 0, sizeof(SSO_GN_MON));
			m_pGnMon[ii].gn_id = m_pGnInf[ii].gn_id;
			m_pGnMon[ii].stat = MENU_LFO_DEV_STS_OK;
			m_pGnMon[ii].stat_old = MENU_LFO_DEV_STS_SCT;

			memset((void*)&m_pGnFact[ii], 0, sizeof(OBJ_ACT_FACT));  //2008-04-04 added by zhw at yn
			m_pGnFact[ii].obj_id = m_pGnInf[ii].gn_id;
			m_pGnFact[ii].obj_idx = m_pGnInf[ii].gn_idx;

			//  2008-05-30 by zhw at fj
//             if( m_pGnPmu[ii].ang_flag == 1 ) // �������ǿ���
//             {
//                 m_pGnMon[ii].is_ang_calc = MENU_LFO_MENU_NO;
//             }
//             else if( m_pGnPmu[ii].ang_flag == 2 ) // ���㹦�ǿ���
//             {
//                 m_pGnMon[ii].is_ang_calc = MENU_LFO_MENU_YES;
//             }
//             else if( m_pGnPmu[ii].ang_flag == 3 ) //  ���߶�����
//             {
//                 if( m_pGnCtl[ii].is_agl_calc == MENU_LFO_MENU_YES )  // ���㹦������
//                 {
//                     m_pGnMon[ii].is_ang_calc = MENU_LFO_MENU_YES;
//                     m_pGnPmu[ii].is_ang_calc = MENU_LFO_MENU_YES;
//                 }
//                 else
//                 {
//                     m_pGnMon[ii].is_ang_calc = MENU_LFO_MENU_NO;
//                     m_pGnPmu[ii].is_ang_calc = MENU_LFO_MENU_NO;
//                 }
//             }
		}
	}
	for (ii = 0; ii < m_nLnNum; ii++)
	{
		m_pLnCtl[ii].is_monitor = MENU_LFO_MON_YES;
		// 		m_pLnCtl[ii].is_part_ana = MENU_LFO_MON_YES;

		if (m_pLnMon[ii].stat != MENU_LFO_DEV_STS_NOYC)
		{
			memset((void*)&m_pLnMon[ii], 0, sizeof(SSO_LN_MON));
			m_pLnMon[ii].ln_id = m_pLnInf[ii].acln_id;
			// 			m_pLnMon[ii].vbase  = m_pLnInf[ii].vbase;
			m_pLnMon[ii].stat = MENU_LFO_DEV_STS_OK;
			m_pLnMon[ii].stat_old = MENU_LFO_DEV_STS_SCT;

			memset((void*)&m_pLnFact[ii], 0, sizeof(OBJ_ACT_FACT));
			m_pLnFact[ii].obj_id = m_pLnInf[ii].acln_id;
			m_pLnFact[ii].obj_idx = m_pLnInf[ii].acln_idx;
		}
	}
	for (ii = 0; ii < m_nTrNum; ii++)
	{
		m_pTrCtl[ii].is_monitor = MENU_LFO_MON_YES;
		//         m_pTrCtl[ii].is_part_ana = MENU_LFO_MON_YES;

		if (m_pTrMon[ii].stat != MENU_LFO_DEV_STS_NOYC)
		{
			memset((void*)&m_pTrMon[ii], 0, sizeof(SSO_TR_MON));
			m_pTrMon[ii].high_wind_id = m_pTrInf[ii].high_wind_id;
			m_pTrMon[ii].mid_wind_id = m_pTrInf[ii].mid_wind_id;
			m_pTrMon[ii].low_wind_id = m_pTrInf[ii].low_wind_id;
			//             m_pTrMon[ii].vhigh		   = m_pTrInf[ii].vhigh;
			//             m_pTrMon[ii].vmid		   = m_pTrInf[ii].vmid;
			//             m_pTrMon[ii].vlow		   = m_pTrInf[ii].vlow;
			m_pTrMon[ii].tr_id = m_pTrInf[ii].tr_id;
			m_pTrMon[ii].stat = MENU_LFO_DEV_STS_OK;
			m_pTrMon[ii].stat_old = MENU_LFO_DEV_STS_SCT;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//  ���������ļ������ʼ��Data
	if (m_DispCtl.dsource != MENU_LFO_DATA_RT && m_DispCtl.dsource != MENU_LFO_DATA_RT_TEST)
	{
		for (ii = 0; ii < m_nFcNum; ii++)
		{
			memset((void*)&m_pFcDat[ii], 0, sizeof(LFO_FC_DAT));
			m_pFcDat[ii].fac_id = m_pFcInf[ii].fac_id;
			m_pFcDat[ii].stat = MENU_LFO_DEV_STS_OK;
		}
		for (ii = 0; ii < m_nGnNum; ii++)
		{
			memset((void*)&m_pGnDat[ii], 0, sizeof(SSO_GN_DAT));
			m_pGnDat[ii].gn_id = m_pGnInf[ii].gn_id;
			m_pGnDat[ii].stat = MENU_LFO_DEV_STS_OK;
		}
		for (ii = 0; ii < m_nLnNum; ii++)
		{
			memset((void*)&m_pLnDat[ii], 0, sizeof(SSO_LN_DAT));
			m_pLnDat[ii].ln_id = m_pLnInf[ii].acln_id;
			m_pLnDat[ii].stat = MENU_LFO_DEV_STS_OK;
		}
		for (ii = 0; ii < m_nTrNum; ii++)
		{
			memset((void*)&m_pTrDat[ii], 0, sizeof(SSO_TR_DAT));
			m_pTrDat[ii].tr_id = m_pTrInf[ii].tr_id;
			m_pTrDat[ii].stat = MENU_LFO_DEV_STS_OK;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	////  powersystem info
	long temp_time = 0;
	if (m_nFcHDFile == 0)
	{
		temp_time = m_SysMon.start_time;
	}
	memset((void*)&m_SysMon, 0, sizeof(SSO_SYS_MON));       //  LFOϵͳ������Ϣ
	if (m_nFcHDFile == 0)
	{
		m_SysMon.start_time = temp_time;
		m_SysMon.end_time = temp_time + 60;
	}

	memset((void*)&m_SysMon_Old, 0, sizeof(SSO_SYS_MON));       //  ����LFOϵͳ������Ϣ
	////  Case info
	m_nCases = 0;
	memset((void*)m_pCases, 0, sizeof(CASE_TYPE) * para_nCaseMaxMem);
	m_nMonMode = 0;
	memset((void*)m_pMonMode, 0, sizeof(PRONY_ANAL_RSLT) * para_nCaseMaxMem);
	//  ������ģʽ
	m_nAllObjMode = 0;
	memset((void*)m_pAllObjMode, 0, sizeof(MODE_PART_OBJ) * para_nObjMaxMode);
	//  δɸѡģʽ
	m_nAllMode = 0;
	memset((void*)m_pAllMode, 0, sizeof(MODE_CHRCT) * para_nSysMaxMode);
	//  ģʽ����
	m_nSltMode = 0;
	memset((void*)m_pSltMode, 0, sizeof(MODE_CHRCT) * para_nSysMaxMode);
	//  �账������
	memset((void*)m_ProMatchtime, 0, sizeof(float) * DATA_NUM_MAX);
	m_nFcProDat = 0;
	memset((void*)m_pFcProDat, 0, sizeof(LFO_FC_DAT) * para_nFcMaxMem);
	memset((void*)m_pFcAna, 0, sizeof(LFO_FC_ANA) * para_nFcMaxMem);
	m_nGnProDat = 0;
	memset((void*)m_pGnProDat, 0, sizeof(SSO_GN_DAT) * para_nGnMaxMem);
	m_nLnProDat = 0;
	memset((void*)m_pLnProDat, 0, sizeof(SSO_LN_DAT) * para_nLnMaxMem);
	m_nTrProDat = 0;
	memset((void*)m_pTrProDat, 0, sizeof(SSO_TR_DAT) * para_nTrMaxMem);
	m_nCntrFc = 0;
	memset((void*)m_pCntrFc, 0, sizeof(SSO_FC_MON) * para_nAnaMaxFcNum);
	m_nDevEveWarn = 0;
	memset((void*)m_pDevEveWarn, 0, (para_nGnMaxMem + para_nLnMaxMem + para_nTrMaxMem) * sizeof(SSO_DEV_MON));
	m_nEveFcWarn = 0;
	memset((void*)m_pEveFcWarn, 0, (para_nFcMaxMem) * sizeof(SSO_FC_MON));
	return;
}

//  ������� (���ӷ������ڳ���)
int CLfoAnaObj::CheckCalcCtlPara()
{
	//PARA_LFO_ANA         m_LfoPara;                         //  ������Ϣ
	//PARA_DISP_CTL        m_DispCtl;                         //  �������
	//PARA_PROC_CTL        m_ProcCtl;                         //  ������Ϣ
	int  ii;
	//  �����豸����״̬(δ����)
	for (ii = 0; ii < m_nFcNum; ii++)
	{
		if (m_pFcCtl[ii].is_monitor == MENU_LFO_MENU_NO)  m_pFcMon[ii].stat = MENU_LFO_DEV_STS_NO;
	}
	for (ii = 0; ii < m_nGnNum; ii++)
	{
		if (m_pGnCtl[ii].is_monitor == MENU_LFO_MENU_NO)
		{
			m_pGnMon[ii].stat = MENU_LFO_DEV_STS_NO;
			m_pGnMon[ii].stat_val = MENU_LFO_DEV_STS_NO;
		}
	}
	for (ii = 0; ii < m_nLnNum; ii++)
	{
		if (m_pLnCtl[ii].is_monitor == MENU_LFO_MENU_NO)
		{
			m_pLnMon[ii].stat = MENU_LFO_DEV_STS_NO;
			m_pLnMon[ii].stat_val = MENU_LFO_DEV_STS_NO;
		}
	}
	for (ii = 0; ii < m_nTrNum; ii++)
	{
		if (m_pTrCtl[ii].is_monitor == MENU_LFO_MENU_NO)
		{
			m_pTrMon[ii].stat = MENU_LFO_DEV_STS_NO;
			m_pTrMon[ii].stat_val = MENU_LFO_DEV_STS_NO;
		}
	}

	//  ���������
	PARA_LFO_ANA  temp_LfoPara;
	memset((void*)&temp_LfoPara, 0, sizeof(PARA_LFO_ANA));

	if (GetLfoPara(temp_LfoPara) == 0)
	{
		if (temp_LfoPara.mem_max_sec > 10 && temp_LfoPara.mem_max_sec < 30)
		{
			m_LfoPara.mem_max_sec = temp_LfoPara.mem_max_sec;
		}
		if (temp_LfoPara.freq_min > 0.1 && temp_LfoPara.freq_min < 20)
		{
			m_LfoPara.freq_min = temp_LfoPara.freq_min;
		}
		if (temp_LfoPara.freq_max > 50 && temp_LfoPara.freq_max < 100)
		{
			m_LfoPara.freq_max = temp_LfoPara.freq_max;
		}
		if (temp_LfoPara.smp_freq >= m_LfoPara.freq_max * 3 && temp_LfoPara.smp_freq < 2 * LFO_SMP_FRQ)
		{
			m_LfoPara.smp_freq = temp_LfoPara.smp_freq;
			m_nSmpFre = m_LfoPara.smp_freq;
			m_nSmpStp = (int)(MAX_FRA_PER_SEC / m_nSmpFre); //  �������0.1
		}
		if (temp_LfoPara.flt_len >= 3 && temp_LfoPara.flt_len < m_LfoPara.smp_freq)
		{
			m_LfoPara.flt_len = temp_LfoPara.flt_len;
		}
		if (temp_LfoPara.flt_coe >= 0.01 && temp_LfoPara.flt_coe <= 0.50)
		{
			m_LfoPara.flt_coe = temp_LfoPara.flt_coe;
		}
		if (temp_LfoPara.time_leng >= 0 && temp_LfoPara.time_leng <= 10)
		{
			m_LfoPara.time_leng = temp_LfoPara.time_leng;
		}
		if (temp_LfoPara.int_time >= 0 && temp_LfoPara.int_time <= m_LfoPara.time_leng / 2)
		{
			m_LfoPara.int_time = temp_LfoPara.int_time;
		}
		if (temp_LfoPara.sso_jud_num > 1 && temp_LfoPara.sso_jud_num < 10)
		{
			m_LfoPara.sso_jud_num = temp_LfoPara.sso_jud_num;
		}
		if (temp_LfoPara.volt_thre1 > 0)
		{
			m_LfoPara.volt_thre1 = temp_LfoPara.volt_thre1;
		}
		if (temp_LfoPara.volt_thre2 > 0)
		{
			m_LfoPara.volt_thre2 = temp_LfoPara.volt_thre2;
		}
		//�㶫��Ŀ����
		if (temp_LfoPara.gn_swn_ampl > 0)
		{
			m_LfoPara.gn_swn_ampl = temp_LfoPara.gn_swn_ampl;
		}
		if (temp_LfoPara.gn_ewn_ampl > 0)
		{
			m_LfoPara.gn_ewn_ampl = temp_LfoPara.gn_ewn_ampl;
		}
		if (temp_LfoPara.ln_swn_ampl1 > 0)
		{
			m_LfoPara.ln_swn_ampl1 = temp_LfoPara.ln_swn_ampl1;
		}
		if (temp_LfoPara.ln_swn_ampl2 > 0)
		{
			m_LfoPara.ln_swn_ampl2 = temp_LfoPara.ln_swn_ampl2;
		}
		if (temp_LfoPara.ln_swn_ampl3 > 0)
		{
			m_LfoPara.ln_swn_ampl3 = temp_LfoPara.ln_swn_ampl3;
		}
		if (temp_LfoPara.ln_ewn_ampl1 > 0)
		{
			m_LfoPara.ln_ewn_ampl1 = temp_LfoPara.ln_ewn_ampl1;
		}
		if (temp_LfoPara.ln_ewn_ampl2 > 0)
		{
			m_LfoPara.ln_ewn_ampl2 = temp_LfoPara.ln_ewn_ampl2;
		}
		if (temp_LfoPara.ln_ewn_ampl3 > 0)
		{
			m_LfoPara.ln_ewn_ampl3 = temp_LfoPara.ln_ewn_ampl3;
		}
		if (temp_LfoPara.tr_swn_ampl1 > 0)
		{
			m_LfoPara.tr_swn_ampl1 = temp_LfoPara.tr_swn_ampl1;
		}
		if (temp_LfoPara.tr_swn_ampl2 > 0)
		{
			m_LfoPara.tr_swn_ampl2 = temp_LfoPara.tr_swn_ampl2;
		}
		if (temp_LfoPara.tr_swn_ampl3 > 0)
		{
			m_LfoPara.tr_swn_ampl3 = temp_LfoPara.tr_swn_ampl3;
		}
		if (temp_LfoPara.tr_ewn_ampl1 > 0)
		{
			m_LfoPara.tr_ewn_ampl1 = temp_LfoPara.tr_ewn_ampl1;
		}
		if (temp_LfoPara.tr_ewn_ampl2 > 0)
		{
			m_LfoPara.tr_ewn_ampl2 = temp_LfoPara.tr_ewn_ampl2;
		}
		if (temp_LfoPara.tr_ewn_ampl3 > 0)
		{
			m_LfoPara.tr_ewn_ampl3 = temp_LfoPara.tr_ewn_ampl3;
		}
		sprintf(m_LogString, "�۲�ʱ�䴰��=[%d],����ʱ�䴰��=[%d]", m_LfoPara.time_leng, m_LfoPara.int_time);
		MessShow(m_LogString);

	}

	PARA_DISP_CTL  temp_DispCtl;
	memset((void*)&temp_DispCtl, 0, sizeof(PARA_DISP_CTL));

	if (GetDispCtl(temp_DispCtl) == 0)
	{
		if (temp_DispCtl.dsource != m_DispCtl.dsource)
		{
			m_reAllocmem = REALLOCMEM;
			//////////////////////////////////////////////////////////////////////////
			m_FcDataFilePos = 0;
			m_FcDataFileOffset = 0;
			m_GnDataFilePos = 0;
			m_FcDataFileOffset = 0;
			m_LnDataFilePos = 0;
			m_LnDataFileOffset = 0;
			m_TrDataFilePos = 0;
			m_TrDataFileOffset = 0;
			//m_CurGpsTime = 0;
			m_OldGpsTime = 0;
			m_FcVolFilePos = 0;                          //  ��վ��ѹ��ȡλ��
			m_FcVolFileOffset = 0;                          //  ��վ��ѹ��ȡƫ����
			m_FcAngFilePos = 0;                          //  ��վ��ѹ��ȡλ��
			m_FcAngFileOffset = 0;                          //  ��վ��ѹ��ȡƫ����

			m_GnPowFilePos = 0;                          //  ������й���ȡλ��
			m_GnPowFileOffset = 0;                          //  ������й���ȡƫ����
			m_GnAngFilePos = 0;                          //  �������Ƕ�ȡλ��
			m_GnAngFileOffset = 0;                          //  �������Ƕ�ȡƫ����
			memset(&m_SysResSave, 0, sizeof(LFO_SYS_RES_SAVE));
			m_DispCtl.dsource = temp_DispCtl.dsource;
			// 			if( m_DispCtl.dsource <= MENU_LFO_DATA_RT )
			// 			{
			// #ifdef _WAMAP_
			//                 SetPmuFilColNoForTest( m_acdot_id, COL_ID_LN_PWR, m_nLnNum, m_pLnPmu, COL_ID_GN_PWR, m_nGnNum, m_pGnPmu, m_filNo, m_colNo, 0 );
			// #endif
			// 			}
		}
		if (m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
		{
			// #ifdef _WAMAP_
			//             if ( m_isForceSet == 1 ) // ǿ������
			//             {
			//                 SetPmuFilColNoForTest( m_acdot_id, COL_ID_LN_UAV, m_nLnNum, m_pLnPmu, COL_ID_GN_PWR, m_nGnNum, m_pGnPmu, m_filNo, m_colNo, 1 );
			//             }
			//             else  // ��ǿ������
			//             {
			//                 SetPmuFilColNoForTest( m_acdot_id, COL_ID_LN_UAV, m_nLnNum, m_pLnPmu, COL_ID_GN_PWR, m_nGnNum, m_pGnPmu, m_filNo, m_colNo, 0 );
			//             }
			// #endif
		}
		else
		{
			if (m_DispCtl.dsource == MENU_LFO_DATA_FILE)
			{
				sprintf(m_DataFileDir, "%s", m_DataFile);
			}
			else if (m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE)
			{
				sprintf(m_DataFileDir, "%s", m_DataFile1);
			}
			else if (m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE2)
			{
				sprintf(m_DataFileDir, "%s", m_DataFile2);
			}
			else if (m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE3)
			{
				sprintf(m_DataFileDir, "%s", m_DataFile3);
			}
			else if (m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE4)
			{
				sprintf(m_DataFileDir, "%s", m_DataFile4);
			}
		}

		if (temp_DispCtl.show_max >= 0)
		{
			m_DispCtl.show_max = temp_DispCtl.show_max;
		}
#ifdef _NUSP
		if (PODB_GetTabelNoByDevId(temp_DispCtl.sso_fac_id) == PLANT_INFO_NO_TAB || PODB_GetTabelNoByDevId(temp_DispCtl.sso_fac_id) == SUBSTATION_INFO_NO_TAB)
#else
		if (PODB_GetTabelNoByDevId(temp_DispCtl.sso_fac_id) == FAC_INFO_NO_TAB)
#endif // _NUSP
		{
			m_DispCtl.sso_fac_id = temp_DispCtl.sso_fac_id;
		}

		m_DispCtl.is_warn = temp_DispCtl.is_warn;
		m_DispCtl.is_samp = temp_DispCtl.is_samp;
	}

	//  �����̿��Ʋ���
	//int     GetProcCtl( PARA_PROC_CTL &ProcCtl ) ;  //  LFO��������һ����¼��
	//GetProcCtl( m_ProcCtl ) ;
	return 0;
}

// ��ȡ����
int CLfoAnaObj::HandlingPmuSampleData()
{
	int   i;
	char  strTime1[28], strTime2[28];

	//////////////////////////////////////////////////////////////////////////
	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)  //  ��������PMU����   
	{
		//////////////////////////////////////////////////////////////////////////
		m_FcDataFilePos = 0;                          //  �����ļ���ȡλ��
		m_FcDataFileOffset = 0;                          //  ��������ļ���ȡλ�õ�ƫ����

		m_GnDataFilePos = 0;                          //  �����ļ���ȡλ��
		m_GnDataFileOffset = 0;                          //  ��������ļ���ȡλ�õ�ƫ����

		m_LnDataFilePos = 0;                          //  �����ļ���ȡλ��
		m_LnDataFileOffset = 0;                          //  ��������ļ���ȡλ�õ�ƫ����

		m_TrDataFilePos = 0;
		m_TrDataFileOffset = 0;

		m_FcVolFilePos = 0;                          //  ��վ��ѹ��ȡλ��
		m_FcVolFileOffset = 0;                          //  ��վ��ѹ��ȡƫ����
		m_FcAngFilePos = 0;                          //  ��վ��ѹ��ȡλ��
		m_FcAngFileOffset = 0;                          //  ��վ��ѹ��ȡƫ����

		m_GnPowFilePos = 0;                          //  ������й���ȡλ��
		m_GnPowFileOffset = 0;                          //  ������й���ȡƫ����
		m_GnAngFilePos = 0;                          //  �������Ƕ�ȡλ��
		m_GnAngFileOffset = 0;                          //  �������Ƕ�ȡƫ����
		m_LfoPara.data_pos = 0;
		//////////////////////////////////////////////////////////////////////////

#ifdef _WAMAP_                                                                                                                
		sprintf(m_LogString, "��������WAMS���ݿ�[%d]", m_DispCtl.dsource);
		MessShow(m_LogString);

		int    LastRecTime = 0;
		m_CurGpsTime = 0;

		PWD_GetLastTime(LastRecTime);  //  �õ�PMU���ݱ�����ˢ��ʱ��
		m_CurGpsTime = LastRecTime - para_nDelaySec;                //  ����2�룬ȷ����ȡ��ˢ�º����ݣ�
		if (m_CurGpsTime <= m_OldGpsTime)
		{
			PT_DelayMinSec(1100);
			PWD_GetLastTime(LastRecTime);
			m_CurGpsTime = LastRecTime - para_nDelaySec;
		}
		if (LastRecTime <= 0)
		{
			sprintf(m_LogString, " PMU BUFFER NOT UPDATE! ");
			MessShow(m_LogString);
			for (i = 0; i < m_nFcNum; i++)
			{
				if (m_pFcMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pFcCtl[i].is_monitor == MENU_LFO_MENU_YES)
				{
					m_pFcMon[i].stat = MENU_LFO_DEV_STS_OLD;
				}
			}
			for (i = 0; i < m_nGnNum; i++)
			{
				if (m_pGnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pGnCtl[i].is_monitor == MENU_LFO_MENU_YES)
				{
					m_pGnMon[i].stat = MENU_LFO_DEV_STS_OLD;
					m_pGnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
				}
			}
			for (i = 0; i < m_nLnNum; i++)
			{
				if (m_pLnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pLnCtl[i].is_monitor == MENU_LFO_MENU_YES)
				{
					m_pLnMon[i].stat = MENU_LFO_DEV_STS_OLD;
					m_pLnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
				}
			}

			m_CurGpsTime = -1;
			PT_DelayMinSec(2000);
			return 1;
		}

		if (m_SysMon_Old.stat > MENU_LFO_DEV_STS_SCT)  // ϵͳ����ȫ�����
		{
			if (m_CurGpsTime <= m_OldGpsTime)
			{
				sprintf(m_LogString, " PMU BUFFER NOT UPDATE! ");
				MessShow(m_LogString);
				for (i = 0; i < m_nFcNum; i++)
				{
					if (m_pFcMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pFcCtl[i].is_monitor == MENU_LFO_MENU_YES)
					{
						m_pFcMon[i].stat = MENU_LFO_DEV_STS_OLD;
					}
				}
				for (i = 0; i < m_nGnNum; i++)
				{
					if (m_pGnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pGnCtl[i].is_monitor == MENU_LFO_MENU_YES)
					{
						m_pGnMon[i].stat = MENU_LFO_DEV_STS_OLD;
						m_pGnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
					}
				}
				for (i = 0; i < m_nLnNum; i++)
				{
					if (m_pLnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pLnCtl[i].is_monitor == MENU_LFO_MENU_YES)
					{
						m_pLnMon[i].stat = MENU_LFO_DEV_STS_OLD;
						m_pLnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
					}
				}

				PT_DelayMinSec(2000);
				return 2;
			}
			else
			{
				sprintf(m_LogString, " �ȴ�ʱ��ˢ��,cur=%d,old=%d,leng=%d,int=%d", m_CurGpsTime, m_OldGpsTime, m_LfoPara.time_leng, m_LfoPara.int_time);
				MessShow(m_LogString);
				if (m_CurGpsTime < (m_OldGpsTime + m_LfoPara.time_leng - m_LfoPara.int_time))
				{
					while (m_CurGpsTime < (m_OldGpsTime + m_LfoPara.time_leng - m_LfoPara.int_time))
					{
						PT_DelayMinSec(1000);

						PWD_GetLastTime(LastRecTime);
						if (LastRecTime <= 0)
						{
							sprintf(m_LogString, " PMU BUFFER NOT UPDATE! ");
							MessShow(m_LogString);
							for (i = 0; i < m_nFcNum; i++)
							{
								if (m_pFcMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pFcCtl[i].is_monitor == MENU_LFO_MENU_YES)
								{
									m_pFcMon[i].stat = MENU_LFO_DEV_STS_OLD;
								}
							}
							for (i = 0; i < m_nGnNum; i++)
							{
								if (m_pGnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pGnCtl[i].is_monitor == MENU_LFO_MENU_YES)
								{
									m_pGnMon[i].stat = MENU_LFO_DEV_STS_OLD;
									m_pGnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
								}
							}
							for (i = 0; i < m_nLnNum; i++)
							{
								if (m_pLnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pLnCtl[i].is_monitor == MENU_LFO_MENU_YES)
								{
									m_pLnMon[i].stat = MENU_LFO_DEV_STS_OLD;
									m_pLnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
								}
							}
							m_CurGpsTime = -1;
							return 1;
						}
						m_CurGpsTime = LastRecTime - para_nDelaySec;   // LastRecTime - 1; 
					}
				}
				else
				{
					m_CurGpsTime = (m_OldGpsTime + m_LfoPara.time_leng - m_LfoPara.int_time);
				}
				m_CalcStartTime = m_CurGpsTime - m_LfoPara.time_leng;
			}
		}
		else   // ϵͳ��ȫ�����
		{
			if (m_CurGpsTime > (m_OldGpsTime + m_LfoPara.time_leng + m_LfoPara.time_leng - m_LfoPara.int_time))
			{
				sprintf(m_LogString, " HandlingPmuSampleData(): Calc windows lag behind %d[s], Auto Correct!", ((int)(m_LfoPara.time_leng)));
				MessShow(m_LogString);
			}
			else
			{
				if (m_CurGpsTime <= m_OldGpsTime)
				{
					sprintf(m_LogString, " PMU BUFFER NOT UPDATE! m_OldGpsTime[%d], m_CurGpsTime[%d]", m_OldGpsTime, m_CurGpsTime);
					MessShow(m_LogString);
					for (i = 0; i < m_nFcNum; i++)
					{
						if (m_pFcMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pFcCtl[i].is_monitor == MENU_LFO_MENU_YES)
						{
							m_pFcMon[i].stat = MENU_LFO_DEV_STS_OLD;
							//m_pFcMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
						}
					}
					for (i = 0; i < m_nGnNum; i++)
					{
						if (m_pGnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pGnCtl[i].is_monitor == MENU_LFO_MENU_YES)
						{
							m_pGnMon[i].stat = MENU_LFO_DEV_STS_OLD;
							m_pGnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
						}
					}
					for (i = 0; i < m_nLnNum; i++)
					{
						if (m_pLnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pLnCtl[i].is_monitor == MENU_LFO_MENU_YES)
						{
							m_pLnMon[i].stat = MENU_LFO_DEV_STS_OLD;
							m_pLnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
						}
					}

					PT_DelayMinSec(2000);
					return 2;
				}
				else
				{
					sprintf(m_LogString, " �ȴ�ʱ��ˢ��,cur=%d,old=%d,leng=%d,int=%d", m_CurGpsTime, m_OldGpsTime, m_LfoPara.time_leng, m_LfoPara.int_time);
					MessShow(m_LogString);
					while (m_CurGpsTime < (m_OldGpsTime + m_LfoPara.time_leng - m_LfoPara.int_time))
					{
						PT_DelayMinSec(1000);

						PWD_GetLastTime(LastRecTime);
						if (LastRecTime <= 0)
						{
							sprintf(m_LogString, " PMU BUFFER NOT UPDATE! ");
							MessShow(m_LogString);
							for (i = 0; i < m_nFcNum; i++)
							{
								if (m_pFcMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pFcCtl[i].is_monitor == MENU_LFO_MENU_YES)
								{
									m_pFcMon[i].stat = MENU_LFO_DEV_STS_OLD;
									//m_pFcMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
								}
							}
							for (i = 0; i < m_nGnNum; i++)
							{
								if (m_pGnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pGnCtl[i].is_monitor == MENU_LFO_MENU_YES)
								{
									m_pGnMon[i].stat = MENU_LFO_DEV_STS_OLD;
									m_pGnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
								}
							}
							for (i = 0; i < m_nLnNum; i++)
							{
								if (m_pLnMon[i].stat != MENU_LFO_DEV_STS_NOYC && m_pLnCtl[i].is_monitor == MENU_LFO_MENU_YES)
								{
									m_pLnMon[i].stat = MENU_LFO_DEV_STS_OLD;
									m_pLnMon[i].stat_val = MENU_LFO_DEV_STS_OLD;
								}
							}
							m_CurGpsTime = -1;
							return 1;
						}
						m_CurGpsTime = LastRecTime - para_nDelaySec;   // LastRecTime - 1; 
					}
				}
			}

			m_CalcStartTime = m_CurGpsTime - m_LfoPara.time_leng;
		}
		sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
		sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));

		sprintf(m_LogString, " HandlingPmuSampleData(): StartTime=%s[%d], EndTime=%s[%d] LastRecTime=%s[%d]",
			strTime1, m_CalcStartTime, strTime2, m_CurGpsTime, PT_GetHMSTimeStringBySeconds(LastRecTime), LastRecTime);
		MessShow(m_LogString);

		//////////////////////////////////////////////////////////////////////////
		//  get Pmu measured data (ʵ�������ɵ����߳�ˢ��)
		m_nMovSec = m_LfoPara.time_leng - m_LfoPara.int_time;   //  ȡ���ƶ�����
		m_tNewSec = m_CurGpsTime - m_nMovSec;                   //  ȡ��GPSʱ��(m_CurGpsTime-m_nMovSec) 
		m_nOrigSam = m_LfoPara.time_leng * MAX_FRA_PER_SEC;
		//ƥ��ʱ��
		m_nOrigDlt = (float)(1.0 / MAX_FRA_PER_SEC);
		for (i = 0; i < m_nOrigSam; i++)
		{
			m_Matchtime[i] = m_nOrigDlt * i;
		}

		float t_start = 0;
		float t_end = 0;
		float t_delta = 0;
		GetFcData(t_start, t_end, t_delta, m_DispCtl.dsource);
		GetGnData(t_start, t_end, t_delta, m_DispCtl.dsource);
		GetLnData(t_start, t_end, t_delta, m_DispCtl.dsource);
		GetTrData(t_start, t_end, t_delta, m_DispCtl.dsource);

#endif
	}
	else if (m_DispCtl.dsource == MENU_LFO_DATA_FILE)    // ���ļ�����ͬ������ѭ������
	{
		// �ӱ����ļ�ȡ����
		sprintf(m_LogString, "���ļ�����ͬ������ѭ��������\t[%d  %s]", m_DispCtl.dsource, m_DataFileDir);
		MessShow(m_LogString);

		// �ж��Ƿ�Ϊ�¸�ʽ�ļ�
		string tmpstring = m_DataFileDir;
		tmpstring += m_backslash;
		tmpstring += "pmu_data.csv";
		int   realNum = 0;
		float t_delta = 0.0f;
		int nDataTime = -1;
		if (PF_FindFile(tmpstring))
		{
			// �����¸�ʽ�����ļ�
			sprintf(m_LogString, "���¸�ʽ��ȡ�����ļ�[%s]", tmpstring.c_str());
			MessShow(m_LogString, LTINFO);

			ReadNewData(tmpstring, realNum, t_delta, nDataTime);
		}
		else
		{
			// �������¸�ʽ�ļ������ɸ�ʽ��ȡ
			sprintf(m_LogString, "���ɸ�ʽ��ȡ�����ļ�");
			MessShow(m_LogString, LTINFO);
			//////////////////////////////////////////////////////////////////////////
			// ƽ�ƻ�������
			//////////////////////////////////////////////////////////////////////////
			//  get Pmu measured data (ʵ�������ɵ����߳�ˢ��)
			float t_start = 0;  // ���ж�ʱ�����Ƿ�һ��
			float t_end = 0;
			int   temp_int = 0;
			temp_int = GetFcData(t_start, t_end, t_delta, m_DispCtl.dsource);
			if (temp_int > 0 && t_end > 0.1)
			{
				realNum = temp_int;
				m_Matchtime[realNum - 1] = t_end;
				m_Matchtime[0] = t_start;
			}
			temp_int = 0;
			temp_int = GetGnData(t_start, t_end, t_delta, m_DispCtl.dsource);
			if (temp_int > 0 && t_end > 0.1)
			{
				realNum = temp_int;
				m_Matchtime[realNum - 1] = t_end;
				m_Matchtime[0] = t_start;
			}
			temp_int = 0;
			temp_int = GetLnData(t_start, t_end, t_delta, m_DispCtl.dsource);
			if (temp_int > 0 && t_end > 0.1)
			{
				realNum = temp_int;
				m_Matchtime[realNum - 1] = t_end;
				m_Matchtime[0] = t_start;
			}
			temp_int = 0;
			temp_int = GetTrData(t_start, t_end, t_delta, m_DispCtl.dsource);
			if (temp_int > 0 && t_end > 0.1)
			{
				realNum = temp_int;
				m_Matchtime[realNum - 1] = t_end;
				m_Matchtime[0] = t_start;
			}
		}

		////////////lsf add 2007-05-27
		m_nOrigDlt = (m_Matchtime[realNum - 1] - m_Matchtime[0]) / realNum;
		/*
				if( para_nGpsTime >0 )
				{
					m_OldGpsTime = para_nGpsTime;  // 2008-04-24 added by zhw at nj for nGpsTime
				}*/


		if (m_OldGpsTime <= 0)
		{
			time(&m_CurGpsTime);
			m_CurGpsTime = m_CurGpsTime - 60;

			if (para_nGpsTime > 0)
			{
				m_CurGpsTime = para_nGpsTime + m_LfoPara.time_leng;  // 2008-04-24 added by zhw at nj for nGpsTime
			}
		}
		else
		{
			m_CurGpsTime = m_OldGpsTime - m_LfoPara.int_time + m_LfoPara.time_leng;
		}

		m_CalcStartTime = m_CurGpsTime - (int)(m_Matchtime[realNum - 1] - m_Matchtime[0]); //((int)(m_LfoPara.time_leng/0.04))*0.04;
		//ƥ��ʱ��
		m_nOrigSam = realNum;
		for (i = 0; i < m_nOrigSam; i++)
		{
			m_Matchtime[i] = m_nOrigDlt * i;
		}

		if (realNum <= 0)
		{
			for (i = 0; i < 250; i++)
			{
				m_Matchtime[i] = 0.04 * i;
			}
			return -1;
		}
		sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
		sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));

		sprintf(m_LogString, " HandlingPmuSampleData(): StartTime=%s[%d], EndTime=%s[%d]",
			strTime1, m_CalcStartTime, strTime2, m_CurGpsTime);
		MessShow(m_LogString);
		PT_DelaySecond(2);
	}
	else if (m_DispCtl.dsource >= MENU_LFO_CON_DATA_FILE)  //��ͬһ�ļ���ȡ��������
	{
		// �ӱ����ļ�ȡ����               
		sprintf(m_LogString, "��ͬһ�ļ���ȡ�������ߣ�\t[%d  %s]", m_DispCtl.dsource, m_DataFileDir);
		MessShow(m_LogString);

		m_DataFileAskPnt = LFO_CURVE_MAX_PNT;     //  ���������ļ�Ҫ���ȡ������

		// �ж��Ƿ�Ϊ�¸�ʽ�ļ�
		string tmpstring = m_DataFileDir;
		tmpstring += m_backslash;
		tmpstring += "pmu_data.csv";
		int   realNum = 0;
		float t_delta = 0.0f;
		int nDataTime = -1;
		if (PF_FindFile(tmpstring))
		{
			// �����¸�ʽ�����ļ�
			sprintf(m_LogString, "���¸�ʽ��ȡ�����ļ�[%s]", tmpstring.c_str());
			MessShow(m_LogString, LTINFO);

			ReadNewData(tmpstring, realNum, t_delta, nDataTime);
			sprintf(m_LogString, "���¸�ʽ��ȡ�����ļ����");
			MessShow(m_LogString, LTINFO);
		}
		else
		{
			// �������¸�ʽ�ļ������ɸ�ʽ��ȡ
			sprintf(m_LogString, "���ɸ�ʽ��ȡ�����ļ�");
			MessShow(m_LogString, LTINFO);
			//////////////////////////////////////////////////////////////////////////
			// ƽ�ƻ�������
			//////////////////////////////////////////////////////////////////////////
			//  get Pmu measured data (ʵ�������ɵ����߳�ˢ��)
			float t_start = 0;  // ���ж�ʱ�����Ƿ�һ��
			float t_end = 0;
			int   temp_int = 0;
			temp_int = GetFcData(t_start, t_end, t_delta, m_DispCtl.dsource);
			if (temp_int > 0 && t_end > 0.1)
			{
				realNum = temp_int;
				m_Matchtime[realNum - 1] = t_end;
				m_Matchtime[0] = t_start;
			}
			temp_int = 0;
			temp_int = GetGnData(t_start, t_end, t_delta, m_DispCtl.dsource);
			if (temp_int > 0 && t_end > 0.1)
			{
				realNum = temp_int;
				m_Matchtime[realNum - 1] = t_end;
				m_Matchtime[0] = t_start;
			}
			temp_int = 0;
			temp_int = GetLnData(t_start, t_end, t_delta, m_DispCtl.dsource);
			if (temp_int > 0 && t_end > 0.1)
			{
				realNum = temp_int;
				m_Matchtime[realNum - 1] = t_end;
				m_Matchtime[0] = t_start;
			}
			temp_int = 0;
			temp_int = GetTrData(t_start, t_end, t_delta, m_DispCtl.dsource);
			if (temp_int > 0 && t_end > 0.1)
			{
				realNum = temp_int;
				m_Matchtime[realNum - 1] = t_end;
				m_Matchtime[0] = t_start;
			}
		}

		m_nOrigDlt = (m_Matchtime[realNum - 1] - m_Matchtime[0]) / realNum;
		if (m_OldGpsTime <= 0)
		{
			time(&m_CurGpsTime);
			//m_CurGpsTime = m_CurGpsTime - 600;
			if (para_nGpsTime > 0)
			{
				m_CurGpsTime = para_nGpsTime + m_LfoPara.time_leng;  // 2008-04-24 added by zhw at nj for nGpsTime
			}
		}
		else
		{
			m_CurGpsTime = m_OldGpsTime - m_LfoPara.int_time + m_LfoPara.time_leng;
		}

		//ƥ��ʱ��
		m_nOrigSam = realNum;
		for (i = 0; i < m_nOrigSam; i++)
		{
			m_Matchtime[i] = m_nOrigDlt * i;
		}
		m_CalcStartTime = m_CurGpsTime - ((int)(realNum * t_delta + 0.5));
		PT_DelaySecond(2);

		if (realNum <= 0)
		{
			for (i = 0; i < 250; i++)
			{
				m_Matchtime[i] = 0.04 * i;
			}
			return -1;
		}

		sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
		sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));
		sprintf(m_LogString, " HandlingPmuSampleData(): StartTime=%s[%d], EndTime=%s[%d]",
			strTime1, m_CalcStartTime, strTime2, m_CurGpsTime);
		MessShow(m_LogString);
	}
	else
	{
		sprintf(m_LogString, "HandlingPmuSampleData(): ����Դѡ�����\t%d ", m_DispCtl.dsource);
		MessShow(m_LogString);

		m_CurGpsTime = -1;

		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	return 0;
}

//     int                  m_nFcNum;                          //  LFO��վ����
//     LFO_FC_INF *         m_pFcInf;                          //  LFO��վ��̬��Ϣ
//     LFO_FC_CTL *         m_pFcCtl;                          //  LFO��վ������Ϣ
//     LFO_FC_PMU *         m_pFcPmu;                          //  LFO��վPMU��Ϣ
//     LFO_FC_DAT *         m_pFcDat;                          //  LFO��վ��̬����(Ԥ�����)
//     LFO_FC_MON *         m_pFcMon;                          //  LFO��վ������Ϣ
// ��ȡ��վ���ݣ����ݴ���//������//
int CLfoAnaObj::GetFcData(float& t_start, float& t_end, float& t_delta, int dsource)
{
	if (m_nFcNum < 0 || !m_pFcPmu || !m_pFcDat)  return -1;
	int ii, jj, kk;
	int realNum = 0;
	int newSub, oldSub = 0;

	// ���ݿ�����
	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	{
#ifdef _WAMAP_                                                                                                                
		// ǰ������
		if (m_nMovSec > 0 || m_LfoPara.int_time > 0)
		{
			for (ii = 0; ii < m_nFcNum; ii++)
			{
				if (m_pFcPmu[ii].vhisSts != MENU_LFO_DEV_STS_NOYC)
				{
					for (jj = 0; jj < m_LfoPara.int_time; jj++)
					{
						m_pFcDat[ii].SecSts[jj] = m_pFcDat[ii].SecSts[jj + m_nMovSec];

						newSub = MAX_FRA_PER_SEC * jj;
						oldSub = MAX_FRA_PER_SEC * (jj + m_nMovSec);
						for (kk = 0; kk < MAX_FRA_PER_SEC; kk++)
						{
							m_pFcDat[ii].u1v[newSub + kk] = m_pFcDat[ii].u1v[oldSub + kk];
							m_pFcDat[ii].u1a[newSub + kk] = m_pFcDat[ii].u1a[oldSub + kk];
							m_pFcDat[ii].fhz[newSub + kk] = m_pFcDat[ii].fhz[oldSub + kk];
						}
					}
				}
			}
		}
		// ��ʼ��
		for (jj = m_LfoPara.int_time; jj < m_LfoPara.int_time + m_nMovSec; jj++)
		{
			for (ii = 0; ii < m_nFcNum; ii++)
			{
				m_pFcDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_OK;
				if (m_pFcCtl[ii].is_monitor == MENU_LFO_MENU_NO)  m_pFcDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_NO;
				if (m_pFcPmu[ii].vhisSts == MENU_LFO_DEV_STS_NOYC) m_pFcDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_NOYC;
			}
		}

		// ȡ�����ݣ������
		int checkSec = 0;
		for (ii = 0; ii < m_nFcNum; ii++)
		{
			if (m_pFcPmu[ii].vhisSts != MENU_LFO_DEV_STS_NOYC && m_pFcCtl[ii].is_monitor != MENU_LFO_MENU_NO)
			{
				//2008-04-05 added by zhw at yn
				// slct_bus_id �ѳ�ʼ��Ϊ��ĸ
// 				if (m_pFcPmu[ii].m_bs_vecV.viFilNo[0] >= 0 && m_pFcPmu[ii].v_bs_vecV.viFilNo[0] >= 0 )
//                 {
//                     m_pFcPmu[ii].is_m_bs = m_pFcCtl[ii].is_m_bs;
//                 }
// 				else if (m_pFcPmu[ii].m_bs_vecV.viFilNo[0] >= 0 )
//                 {
//                     m_pFcPmu[ii].is_m_bs = MENU_LFO_MENU_YES;
//                 }
// 				else
//                 {
//                     m_pFcPmu[ii].is_m_bs = MENU_LFO_MENU_NO;
//                     m_pFcMon[ii].slct_bus_id = m_pFcPmu[ii].v_bs_id;
//                 }

				if (GetFcDataFromWAMAP(m_pFcPmu[ii], m_tNewSec, m_nMovSec, m_pFcDat[ii], m_LfoPara.int_time) == false)
				{
					for (jj = 0; jj < m_nMovSec; jj++)
					{
						m_pFcDat[ii].SecSts[jj + m_LfoPara.int_time] = MENU_LFO_DEV_STS_OLD;
					}
				}
				// ���ݼ�� ĸ��ͣ�� // ����ͻ��������쳣����֯����ǰ�ж�
				for (jj = 0; jj < m_nMovSec; jj++)
				{
					checkSec = m_LfoPara.int_time + jj;
					// 2008-06-02 by zhw at nj
					if (m_pFcDat[ii].SecSts[checkSec] == MENU_LFO_DEV_STS_OK)//MENU_LFO_DEV_STS_OFF)
					{
						newSub = checkSec * MAX_FRA_PER_SEC;
						for (kk = 0; kk < MAX_FRA_PER_SEC; kk++)
						{
							if (fabs(m_pFcDat[ii].u1v[newSub + kk]) > 1.0f) break;
						}

						if (kk == MAX_FRA_PER_SEC) m_pFcDat[ii].SecSts[checkSec] = MENU_LFO_DEV_STS_STP;
					}
				}
			}
		}
		realNum = MAX_FRA_PER_SEC * m_nMovSec;
		// 2008-06-11 by zhw
//         WriteDataToFile( DATA_TYPE_FAC_VOLT, realNum, m_tNewSec, m_nMovSec, t_delta, m_LfoPara.int_time );
//         WriteDataToFile( DATA_TYPE_FAC_ANG,  realNum, m_tNewSec, m_nMovSec, t_delta, m_LfoPara.int_time );
#endif
	}
	else if (m_DispCtl.dsource == MENU_LFO_DATA_FILE) // �����ļ�
	{
		// �ӱ����ļ�ȡ����               
		//sprintf( m_LogString, "GetFcData()���������Ա����ļ���ѭ������");
		//MessShow( m_LogString );
		// �����ݶ��� m_pFcDat �У�������״̬ MENU_LFO_DEV_STS_OK
		// m_nFcNum  m_pFcDat
		// �ӱ����ļ�ȡ���� 

//		int   bad_int = 0;
//		float bad_float = 0;
//		char  bad_name[64];
		char filename[MAXBUF];
		memset(filename, 0, MAXBUF);
		//int nTempFcNum;
		int total1 = 0;
		int total2 = 0;
		int tcount = -1;

		TEMPDATA* pTempdata1 = NULL;
		char   buff[MAXBUF];
		string  sTempTime;
		int     TempInt;
		memset(buff, 0, MAXBUF);
		//��ȡ��վ��ѹ��ֵ����

#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_FacVoltFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_FacVoltFileName.c_str());
#endif

		FILE* stream = fopen(filename, "r+");

		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetFcAngle::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				//para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total1);

			pTempdata1 = (TEMPDATA*)calloc(total1, sizeof(TEMPDATA));
			memset((void*)pTempdata1, 0, total1 * sizeof(TEMPDATA));

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata1[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%s", pTempdata1[ii].name);
			}

			if (total1 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				tcount = -1;
				while (feof(stream) == 0)
				{
					tcount++;
					// 2008-05-12 by zhw ѭ����ȡԽ��
					if (tcount >= DATA_NUM_MAX)
					{

						sprintf(m_LogString, "��������Խ�ޣ�%d > %d", tcount, DATA_NUM_MAX);
						MessShow(m_LogString);
						tcount--;
						break;
					}
					fscanf(stream, "%f", &m_Matchtime[tcount]);

					for (ii = 0; ii < total1; ii++)
					{
						fscanf(stream, "%f", &pTempdata1[ii].data1[tcount]);
					}
				}
			}

			fclose(stream);
		}

		if (tcount > 1)
		{
			realNum = tcount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[tcount - 1];
		}

		//��ȡ��վ��ѹ�������
		total2 = 0;
		tcount = -1;
		TEMPDATA* pTempdata2 = NULL;
		memset(filename, 0, MAXBUF);
#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_FacAngFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_FacAngFileName.c_str());
#endif

		stream = fopen(filename, "r+");

		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetFcAngle::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				//para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total2);
			if (total1 != total2)
			{
				sprintf(m_LogString, "��վ��ѹ��ֵ�ļ��͵�ѹ����ļ��ĳ�վ�������ȣ�");
				MessShow(m_LogString);
			}

			//			bad_int   = 0;
			//			bad_float = 0;
			//			memset(bad_name, 0, sizeof(char)*64 );

			pTempdata2 = (TEMPDATA*)calloc(total2, sizeof(TEMPDATA));
			memset((void*)pTempdata2, 0, total2 * sizeof(TEMPDATA));

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata2[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, "%s", pTempdata2[ii].name);
			}

			if (total2 > 0)
			{

				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				tcount = -1;
				while (feof(stream) == 0)
				{
					tcount++;
					// 2008-05-12 by zhw ѭ����ȡԽ��
					if (tcount >= DATA_NUM_MAX)
					{
						sprintf(m_LogString, "��������Խ�ޣ�%d > %d", tcount, DATA_NUM_MAX);
						MessShow(m_LogString);
						tcount--;
						break;
					}
					fscanf(stream, "%f", &m_Matchtime[tcount]);

					for (ii = 0; ii < total2; ii++)
					{
						fscanf(stream, "%f", &pTempdata2[ii].data1[tcount]);
					}
				}
			}

			fclose(stream);
		}
		if (tcount > 1)
		{
			realNum = tcount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[tcount - 1];
		}
#ifdef _WAMAP_
		for (ii = 0; ii < total1; ii++)
		{
			for (jj = 0; jj < m_nFcNum; jj++)
			{
				if (pTempdata1[ii].id == m_pFcInf[jj].fac_id)
				{
					//strcpy(m_pFcInf[jj].fac_name, pTempdata1[ii].name);
					m_pFcCtl[jj].is_monitor = MENU_LFO_MON_YES;
					for (kk = 0; kk < tcount; kk++)
					{
						m_pFcDat[jj].u1v[kk] = pTempdata1[ii].data1[kk];
					}
					break;
				}
			}
		}
		for (ii = 0; ii < total2; ii++)
		{
			for (jj = 0; jj < m_nFcNum; jj++)
			{
				if (pTempdata2[ii].id == m_pFcInf[jj].fac_id)
				{
					//strcpy(m_pFcInf[jj].fac_name, pTempdata2[ii].name);
					m_pFcCtl[jj].is_monitor = MENU_LFO_MON_YES;
					//DP_JumpPointHdl(tcount,m_pFcDat[jj].u1a,pTempdata2[ii].data1,para_MaxAnglDiff);  // 2008-08-15
					for (kk = 0; kk < tcount; kk++)
					{
						m_pFcDat[jj].u1a[kk] = pTempdata2[ii].data1[kk];
					}
					break;
				}
			}
		}
#else
		if (total1 > m_nFcNum)
		{
			total1 = m_nFcNum;
		}
		else
		{
			m_nFcNum = total1;
		}

		for (ii = 0; ii < total1; ii++)
		{
			if (ii > m_nFcNum) break;
			m_pFcInf[ii].fac_id = pTempdata1[ii].id;
			m_pFcDat[ii].fac_id = pTempdata1[ii].id;
			m_pFcCtl[ii].fac_id = pTempdata1[ii].id;
			m_pFcMon[ii].fac_id = pTempdata1[ii].id;
			strcpy(m_pFcInf[ii].fac_name, pTempdata1[ii].name);
			m_pFcCtl[ii].is_monitor = MENU_LFO_MON_YES;
			for (kk = 0; kk < tcount; kk++)
			{
				m_pFcDat[ii].u1v[kk] = pTempdata1[ii].data1[kk];
				m_pFcDat[ii].u1a[kk] = pTempdata2[ii].data1[kk];
			}
		}
#endif
		// 2008-06-11 by zhw
//         if( total1 > 0 )
//         {
//             WriteDataToFile( DATA_TYPE_FAC_VOLT, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }
//         if( total2 > 0 )
//         {
//             WriteDataToFile( DATA_TYPE_FAC_ANG,  realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }

		if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
		if (pTempdata2) { free(pTempdata2);  pTempdata2 = NULL; }

	}
	else if (m_DispCtl.dsource >= MENU_LFO_CON_DATA_FILE) // �����ļ�
	{
		// �ӱ����ļ�ȡ����               
		//sprintf( m_LogString, "GetFcData()���������Ա����ļ�����������");
		//MessShow( m_LogString );

		//��ȡ��վ��ѹ��ֵ����
		char  filename[MAXBUF];
		memset(filename, 0, MAXBUF);
		//int nTempGnNum;
		int total1 = 0;
		int total2 = 0;
		int tcount = -1;

		TEMPDATA* pTempdata1 = NULL;
		int         tempFilePos;
		int         askCount = -1;
		int         overCount = -1;
		char        buff[MAXBUF];
		string      sTempTime;
		int         TempInt;
		memset(buff, 0, MAXBUF);
		//		if(c_SampleTimes > 0)  askCount = LFO_CURVE_MAX_PNT*c_SampleTimes;		

#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_FacVoltFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_FacVoltFileName.c_str());
#endif

		FILE* stream = fopen(filename, "r+");

		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetFcVolt::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total1);

			pTempdata1 = (TEMPDATA*)calloc(total1, sizeof(TEMPDATA));
			memset((void*)pTempdata1, 0, total1 * sizeof(TEMPDATA));

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata1[ii].id));
			}

			fscanf(stream, "%f", &t_delta);
			if (t_delta > ZEROVALUE)
			{
				askCount = (int)(m_LfoPara.time_leng / t_delta + 0.45);
				overCount = (int)(m_LfoPara.int_time / t_delta + 0.45);
			}

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%s", pTempdata1[ii].name);
			}

			if (m_FcVolFilePos > 0)  fseek(stream, m_FcVolFilePos, SEEK_SET);

			if (total1 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				for (kk = 0; kk < askCount; kk++)
				{
					tempFilePos = ftell(stream);
					//�˳�  check for EOF
					if (feof(stream))
					{
						tcount = kk - 1;
						if (tcount < 1)
						{
							sprintf(m_LogString, "���ļ�����tcount<2 [m_FcVoltFilePos=%d; tempFilePos=%d]", m_FcVolFilePos, tempFilePos);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							fclose(stream);
							return -1;
						}
						if (t_delta <= 0)
						{
							sprintf(m_LogString, "���ļ��������ݼ�� t_delta=%f]", t_delta);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							fclose(stream);
							return -1;
						}

						if (tcount < askCount)
						{
							for (jj = 0; jj < (askCount - tcount); jj++)
							{
								m_Matchtime[tcount + jj] = m_Matchtime[tcount + jj - 1] + t_delta;
								for (ii = 0; ii < m_nFcNum; ii++)
								{
									m_pFcDat[ii].u1v[tcount + jj] = m_pFcDat[ii].u1v[tcount - 1];
								}
							}
						}
						askCount = tcount;
						m_FcVolFilePos = ftell(stream);
						//m_FcVolFilePos = tempFilePos-m_FcVolFileOffset;
						break;
					}

					fscanf(stream, "%f", &m_Matchtime[kk]);
					for (ii = 0; ii < total1; ii++)
					{
						fscanf(stream, "%f", &pTempdata1[ii].data1[kk]);
					}

					if (kk == (askCount - overCount) - 1)
					{
						m_FcVolFilePos = ftell(stream);
					}

					if (m_FcVolFileOffset <= 0)  m_FcVolFileOffset = m_FcVolFilePos - tempFilePos;
				}
			}

			fclose(stream);
		}

		//��վ��ѹ���
		tcount = -1;
		total2 = 0;
		tempFilePos = 0;
		askCount = LFO_CURVE_MAX_PNT;

		TEMPDATA* pTempdata2 = NULL;
		// 		if(c_SampleTimes > 0)  askCount = LFO_CURVE_MAX_PNT*c_SampleTimes;		
		memset(filename, 0, MAXBUF);
#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_FacAngFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_FacAngFileName.c_str());
#endif

		stream = fopen(filename, "r+");

		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetFcAngle::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				//para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total2);

			if (total1 != total2)
			{
				sprintf(m_LogString, "��վ��ѹ��ֵ�ļ��͵�ѹ����ļ��ĳ�վ�������ȣ�");
				MessShow(m_LogString);
			}

			pTempdata2 = (TEMPDATA*)calloc(total2, sizeof(TEMPDATA));
			memset((void*)pTempdata2, 0, total2 * sizeof(TEMPDATA));

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata2[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, "%s", pTempdata2[ii].name);
			}

			if (m_FcAngFilePos > 0)  fseek(stream, m_FcAngFilePos, SEEK_SET);

			if (total2 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				for (kk = 0; kk < askCount; kk++)
				{
					tempFilePos = ftell(stream);
					//�˳�  check for EOF
					if (feof(stream))
					{
						tcount = kk - 1;
						if (tcount < 1)
						{
							sprintf(m_LogString, "���ļ�����tcount<2 [m_FcAngFilePos=%d; tempFilePos=%d]", m_FcAngFilePos, tempFilePos);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							if (pTempdata2) { free(pTempdata2);  pTempdata2 = NULL; }
							fclose(stream);
							return -1;
						}
						if (t_delta <= 0)
						{
							sprintf(m_LogString, "���ļ��������ݼ�� t_delta=%f]", t_delta);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							if (pTempdata2) { free(pTempdata2);  pTempdata2 = NULL; }
							fclose(stream);
							return -1;
						}

						if (tcount < askCount)
						{
							for (jj = 0; jj < (askCount - tcount); jj++)
							{
								m_Matchtime[tcount + jj] = m_Matchtime[tcount + jj - 1] + t_delta;
								for (ii = 0; ii < m_nFcNum; ii++)
								{
									m_pFcDat[ii].u1a[tcount + jj] = m_pFcDat[ii].u1a[tcount - 1];
								}
							}
						}
						askCount = tcount;
						m_FcAngFilePos = ftell(stream);
						//m_FcAngFilePos = tempFilePos-m_FcAngFileOffset;
						break;
					}

					fscanf(stream, "%f", &m_Matchtime[kk]);
					for (ii = 0; ii < total2; ii++)
					{
						fscanf(stream, "%f", &pTempdata2[ii].data1[kk]);
					}

					if (kk == (askCount - overCount) - 1)
					{
						m_FcAngFilePos = ftell(stream);
					}

					if (m_FcAngFileOffset <= 0) m_FcAngFileOffset = m_FcAngFilePos - tempFilePos;
				}
			}
			fclose(stream);
		}

		if (askCount > 0)
		{
			realNum = askCount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[askCount - 1];
		}
#ifdef _WAMAP_
		for (ii = 0; ii < total1; ii++)
		{
			for (jj = 0; jj < m_nFcNum; jj++)
			{
				if (pTempdata1[ii].id == m_pFcInf[jj].fac_id)
				{
					//strcpy(m_pFcInf[jj].fac_name, pTempdata1[ii].name);
					m_pFcCtl[jj].is_monitor = MENU_LFO_MON_YES;
					for (kk = 0; kk < askCount; kk++)
					{
						m_pFcDat[jj].u1v[kk] = pTempdata1[ii].data1[kk];
					}
					break;
				}
			}
		}
		for (ii = 0; ii < total2; ii++)
		{
			for (jj = 0; jj < m_nFcNum; jj++)
			{
				if (pTempdata2[ii].id == m_pFcInf[jj].fac_id)
				{
					//strcpy(m_pFcInf[jj].fac_name, pTempdata2[ii].name);
					m_pFcCtl[jj].is_monitor = MENU_LFO_MON_YES;
					m_pFcMon[jj].fac_id = m_pFcInf[jj].fac_id;
					m_pFcMon[jj].stat = MENU_LFO_DEV_STS_OK;
					//DP_JumpPointHdl(askCount,m_pFcDat[jj].u1a,pTempdata2[ii].data1);  // 2008-08-14 by zhw
					for (kk = 0; kk < askCount; kk++)
					{
						m_pFcDat[jj].u1a[kk] = pTempdata2[ii].data1[kk];
					}
					break;
				}
			}
		}
#else
		if (total1 > m_nFcNum)
		{
			total1 = m_nFcNum;
		}
		else
		{
			m_nFcNum = total1;
		}

		for (ii = 0; ii < total1; ii++)
		{
			if (ii > m_nFcNum) break;
			m_pFcInf[ii].fac_id = pTempdata1[ii].id;
			m_pFcDat[ii].fac_id = pTempdata1[ii].id;
			m_pFcCtl[ii].fac_id = pTempdata1[ii].id;
			m_pFcMon[ii].fac_id = pTempdata1[ii].id;
			m_pFcMon[ii].stat = MENU_LFO_DEV_STS_OK;
			strcpy(m_pFcInf[ii].fac_name, pTempdata1[ii].name);
			m_pFcCtl[ii].is_monitor = MENU_LFO_MON_YES;
			for (kk = 0; kk < askCount; kk++)
			{
				m_pFcDat[ii].u1v[kk] = pTempdata1[ii].data1[kk];
				m_pFcDat[ii].u1a[kk] = pTempdata2[ii].data1[kk];
			}
		}
#endif
		// 2008-06-11 by zhw
//         if( total1 > 0 )
//         {
//             WriteDataToFile( DATA_TYPE_FAC_VOLT, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }
//         if( total2 > 0 )
//         {
//             WriteDataToFile( DATA_TYPE_FAC_ANG,  realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }

		if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
		if (pTempdata2) { free(pTempdata2);  pTempdata2 = NULL; }
	}
	else  // m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE2 // �����ļ�
	{
		sprintf(m_LogString, "����Դ����");
		MessShow(m_LogString);
		realNum = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	return realNum;
}

// ��ȡ�������ݣ����ݴ���
int CLfoAnaObj::GetGnData(float& t_start, float& t_end, float& t_delta, int dsource)
{
	if (m_nGnNum < 0 || !m_pGnPmu || !m_pGnDat)  return -1;
	int ii, jj, kk;
	int realNum = 0;
	int newSub, oldSub = 0;

	// ���ݿ�����
	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	{
#ifdef _WAMAP_                                                                                                                
		// ǰ������
		if (m_nMovSec > 0 || m_LfoPara.int_time > 0)
		{
			for (ii = 0; ii < m_nGnNum; ii++)
			{
				if (m_pGnPmu[ii].hisSts != MENU_LFO_DEV_STS_NOYC)
				{
					for (jj = 0; jj < m_LfoPara.int_time; jj++)
					{
						m_pGnDat[ii].SecSts[jj] = m_pGnDat[ii].SecSts[jj + m_nMovSec];

						newSub = MAX_FRA_PER_SEC * jj;
						oldSub = MAX_FRA_PER_SEC * (jj + m_nMovSec);
						for (kk = 0; kk < MAX_FRA_PER_SEC; kk++)
						{
							m_pGnDat[ii].ampl[newSub + kk] = m_pGnDat[ii].ampl[oldSub + kk];
							m_pGnDat[ii].freq[newSub + kk] = m_pGnDat[ii].freq[oldSub + kk];
							m_pGnDat[ii].sso[newSub + kk] = m_pGnDat[ii].sso[oldSub + kk];
						}
					}
				}
			}
		}
		for (jj = m_LfoPara.int_time; jj < m_LfoPara.int_time + m_nMovSec; jj++)
		{
			for (ii = 0; ii < m_nGnNum; ii++)
			{
				m_pGnDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_OK;
				if (m_pGnCtl[ii].is_monitor == MENU_LFO_MENU_NO) m_pGnDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_NO;
				if (m_pGnPmu[ii].hisSts == MENU_LFO_DEV_STS_NOYC) m_pGnDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_NOYC;
			}
		}

		// ȡ�����ݣ������
		int checkSec = 0;
		for (ii = 0; ii < m_nGnNum; ii++)
		{
			// 			sprintf( m_LogString, "����hisSts[%d],is_monitor[%d]",m_pGnPmu[ii].hisSts,m_pGnCtl[ii].is_monitor);
			// 			MessShow( m_LogString );
			if (m_pGnPmu[ii].hisSts != MENU_LFO_DEV_STS_NOYC && m_pGnCtl[ii].is_monitor != MENU_LFO_MENU_NO)
			{
				if (GetGnDataFromWAMAP(m_pGnPmu[ii], m_tNewSec, m_nMovSec, m_pGnDat[ii], m_LfoPara.int_time) == false)
				{
					sprintf(m_LogString, "��ȡ����[%ld]pmu���ݴ���!", m_pGnPmu[ii].gn_id);
					MessShow(m_LogString);
					for (jj = 0; jj < m_nMovSec; jj++)
					{
						m_pGnDat[ii].SecSts[jj + m_LfoPara.int_time] = MENU_LFO_DEV_STS_OLD;
					}
				}
				// 				int nn =999999;
				// 				if (nn > sizeof(m_pGnDat[ii].ampl)/sizeof(float))
				// 				{
				// 					nn = sizeof(m_pGnDat[ii].ampl)/sizeof(float);
				// 				}
				// 				if (nn > sizeof(m_pGnDat[ii].freq)/sizeof(float))
				// 				{
				// 					nn = sizeof(m_pGnDat[ii].freq)/sizeof(float);
				// 				}
				// 				if (nn > sizeof(m_pGnDat[ii].sso)/sizeof(int))
				// 				{
				// 					nn = sizeof(m_pGnDat[ii].sso)/sizeof(int);
				// 				}
				// 				for (int mm=0;mm < nn;mm++)
				// 				{
				// 					sprintf( m_LogString, "����sso[%d],ssp[%f],ssf[%f]",m_pGnDat[ii].sso[mm],m_pGnDat[ii].ampl[mm],m_pGnDat[ii].freq[mm]);
				// 					MessShow( m_LogString );
				// 				}
								// ���ݼ�� ����ͣ�� // ����ͻ��������쳣����֯����ǰ�ж�
				// 				for ( jj =0; jj<m_nMovSec; jj++ )
				// 				{
				// 					checkSec = m_LfoPara.int_time + jj;
				//                     // 2008-05-31 by zhw at fj
				//                     // 2008-06-02 by zhw at nj
				// 					if (m_pGnDat[ii].SecSts[checkSec] == MENU_LFO_DEV_STS_OK)//MENU_LFO_DEV_STS_OFF)
				// 					{
				// 						newSub = checkSec * MAX_FRA_PER_SEC;
				// 						for (kk = 0; kk< MAX_FRA_PER_SEC; kk++)
				// 						{
				// 							if (fabs(m_pGnDat[ii].pwr[newSub+kk]) > 1.0f ) break;
				// 						}
				// 
				// 						if (kk == MAX_FRA_PER_SEC ) m_pGnDat[ii].SecSts[checkSec] = MENU_LFO_DEV_STS_STP;
				// 					}
				// 				}
			}
		}
		realNum = MAX_FRA_PER_SEC * m_nMovSec;
		// 2008-06-11 by zhw 
//         WriteDataToFile( DATA_TYPE_GEN_POW, realNum, m_tNewSec, m_nMovSec, t_delta, m_LfoPara.int_time );
//         WriteDataToFile( DATA_TYPE_GEN_ANG, realNum, m_tNewSec, m_nMovSec, t_delta, m_LfoPara.int_time );
#endif
	}
	else if (m_DispCtl.dsource == MENU_LFO_DATA_FILE) // �����ļ�
	{
		// �ӱ����ļ�ȡ����               
		//sprintf( m_LogString, "GetGnData()���������Ա����ļ���ѭ������");
		//MessShow( m_LogString );
		// �����ݶ��� m_pGnDat �У�������״̬
		// m_pGnDat
		// �ӱ����ļ�ȡ����  

//		int   bad_int = 0;
//		float bad_float = 0;
//		char  bad_name[64];
		char  filename[MAXBUF];
		memset(filename, 0, MAXBUF);
		//int nTempGnNum;
		int total1 = 0;
		int total2 = 0;
		int tcount = -1;

		TEMPDATA* pTempdata1 = NULL;
		char   buff[MAXBUF];
		string sTempTime;
		int    TempInt;
		memset(buff, 0, MAXBUF);
		char  tmpTime[64];

#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_GnPowFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_GnPowFileName.c_str());
#endif

		FILE* stream = fopen(filename, "r+");
		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetGnPower::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total1);

			pTempdata1 = (TEMPDATA*)calloc(total1, sizeof(TEMPDATA));
			memset((void*)pTempdata1, 0, total1 * sizeof(TEMPDATA));

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata1[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%f", &pTempdata1[ii].data);
			}

			fscanf(stream, "%s", tmpTime);

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%s", pTempdata1[ii].name);
			}

			if (total1 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				tcount = -1;
				while (feof(stream) == 0)
				{
					tcount = tcount + 1;
					// 2008-05-12 by zhw ѭ����ȡԽ��
					if (tcount >= DATA_NUM_MAX)
					{
						sprintf(m_LogString, "��������Խ�ޣ�%d > %d", tcount, DATA_NUM_MAX);
						MessShow(m_LogString);
						tcount = tcount - 1;
						break;
					}
					fscanf(stream, "%f", &m_Matchtime[tcount]);

					for (ii = 0; ii < total1; ii++)
					{
						fscanf(stream, "%f", &pTempdata1[ii].data1[tcount]);
					}
				}
			}

			fclose(stream);
		}

		if (tcount > 1)
		{
			realNum = tcount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[tcount - 1];
		}
		// 2008-05-04 only pwr
#ifdef _WAMAP_
		for (ii = 0; ii < total1; ii++)
		{
			for (jj = 0; jj < m_nGnNum; jj++)
			{
				if (pTempdata1[ii].id == m_pGnInf[jj].gn_id)
				{
					//strcpy(m_pGnInf[jj].gn_name, pTempdata1[ii].name);
					//strcpy(m_pGnInf[jj].area_name, pTempdata1[ii].name_1);
//                     m_pGnInf[jj].emws       = pTempdata1[ii].data;
					m_pGnCtl[jj].is_monitor = MENU_LFO_MON_YES;
					for (kk = 0; kk < tcount; kk++)
					{
						m_pGnDat[jj].ampl[kk] = pTempdata1[ii].data1[kk];
					}
					break;
				}
			}
		}
#endif
		//�ӱ����ļ���ȡ���������
		total2 = 0;
		tcount = -1;

		TEMPDATA* pTempdata2 = NULL;
		memset(filename, 0, MAXBUF);
#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_GnAngFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_GnAngFileName.c_str());
#endif

		stream = fopen(filename, "r+");
		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetGnAngle::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total2);
			if (total1 != total2)
			{
				sprintf(m_LogString, "������й��ļ��͹����ļ��ķ�����������ȣ�");
				MessShow(m_LogString);
			}

			pTempdata2 = (TEMPDATA*)calloc(total2, sizeof(TEMPDATA));
			memset((void*)pTempdata2, 0, total2 * sizeof(TEMPDATA));

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata2[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, "%f", &pTempdata2[ii].data);
			}

			fscanf(stream, "%s", tmpTime);

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, "%s", pTempdata2[ii].name);
			}

			if (total2 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				tcount = -1;
				while (feof(stream) == 0)
				{
					tcount = tcount + 1;
					// 2008-05-12 by zhw ѭ����ȡԽ��
					if (tcount >= DATA_NUM_MAX)
					{
						sprintf(m_LogString, "��������Խ�ޣ�%d > %d", tcount, DATA_NUM_MAX);
						MessShow(m_LogString);
						tcount = tcount - 1;
						break;
					}
					fscanf(stream, "%f", &m_Matchtime[tcount]);

					for (ii = 0; ii < total2; ii++)
					{
						fscanf(stream, "%f", &pTempdata2[ii].data1[tcount]);
					}

					// 					if(tcount >0 && m_Matchtime[tcount] == 0)
					// 					{
					// 						tcount--;
					// 						break;
					// 					}
				}
			}
			fclose(stream);
		}

		if (tcount > 1)
		{
			realNum = tcount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[tcount - 1];
		}
#ifdef _WAMAP_
		for (ii = 0; ii < total2; ii++)
		{
			for (jj = 0; jj < m_nGnNum; jj++)
			{
				if (pTempdata2[ii].id == m_pGnInf[jj].gn_id)
				{
					//strcpy(m_pGnInf[jj].gn_name, pTempdata2[ii].name);
					//strcpy(m_pGnInf[jj].area_name, pTempdata2[ii].name_1);
// 					m_pGnInf[jj].emws       = pTempdata2[ii].data;
					m_pGnCtl[jj].is_monitor = MENU_LFO_MON_YES;
					for (kk = 0; kk < tcount; kk++)
					{
						m_pGnDat[jj].freq[kk] = pTempdata2[ii].data1[kk];
					}
					break;
				}
			}
		}
#else
		if (total1 > m_nGnNum)
		{
			total1 = m_nGnNum;
		}
		else
		{
			m_nGnNum = total1;
		}

		for (ii = 0; ii < total1; ii++)
		{
			if (ii > m_nGnNum) break;
			m_pGnInf[ii].gn_id = pTempdata1[ii].id;
			m_pGnDat[ii].gn_id = pTempdata1[ii].id;
			m_pGnCtl[ii].gn_id = pTempdata1[ii].id;
			m_pGnMon[ii].gn_id = pTempdata1[ii].id;
			strcpy(m_pGnInf[ii].gn_name, pTempdata1[ii].name);
			m_pGnInf[ii].emws = pTempdata1[ii].data;
			m_pGnCtl[ii].is_monitor = MENU_LFO_MON_YES;
			for (kk = 0; kk < tcount; kk++)
			{
				m_pGnDat[ii].pwr[kk] = pTempdata1[ii].data1[kk];
				m_pGnDat[ii].ang[kk] = pTempdata2[ii].data1[kk];
			}
		}
#endif
		// 2008-06-11 by zhw 
//         if( total1 > 0)
//         {
//             WriteDataToFile( DATA_TYPE_GEN_POW, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }
// 
//         if( total2 > 0)
//         {
//             WriteDataToFile( DATA_TYPE_GEN_ANG, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }

		if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
		if (pTempdata2) { free(pTempdata2);  pTempdata2 = NULL; }
	}
	else if (m_DispCtl.dsource >= MENU_LFO_CON_DATA_FILE) // �����ļ�
	{
		// �ӱ����ļ�ȡ����               
		//sprintf( m_LogString, "GetGnData()���������Ա����ļ�����������");
		//MessShow( m_LogString );

		//������й�
		char  filename[MAXBUF];
		memset(filename, 0, MAXBUF);
		//int   nTempGnNum;
		int			total1 = 0;
		int			total2 = 0;
		int         tcount = -1;
		int         tempFilePos = 0;

		int         askCount = -1;
		int         overCount = -1;

		TEMPDATA* pTempdata1 = NULL;
		char        buff[MAXBUF];
		string      sTempTime;
		char        tmpTime[64];
		int         TempInt;
		memset(buff, 0, MAXBUF);
		// 		if(c_SampleTimes > 0)  askCount = LFO_CURVE_MAX_PNT*c_SampleTimes;		

#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_GnPowFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_GnPowFileName.c_str());
#endif

		FILE* stream = fopen(filename, "r+");
		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetGnPower::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total1);

			pTempdata1 = (TEMPDATA*)calloc(total1, sizeof(TEMPDATA));
			memset((void*)pTempdata1, 0, total1 * sizeof(TEMPDATA));

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata1[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			if (t_delta > ZEROVALUE)
			{
				askCount = (int)(m_LfoPara.time_leng / t_delta + 0.45);
				overCount = (int)(m_LfoPara.int_time / t_delta + 0.45);
			}

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%f", &pTempdata1[ii].data);
			}

			fscanf(stream, "%s", tmpTime);

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%s", pTempdata1[ii].name);
			}

			if (m_GnPowFilePos > 0)  fseek(stream, m_GnPowFilePos, SEEK_SET);

			if (total1 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				for (kk = 0; kk < askCount; kk++)
				{
					tempFilePos = ftell(stream);
					//�˳�  check for EOF
					if (feof(stream))
					{
						tcount = kk - 1;
						if (tcount < 1)
						{
							sprintf(m_LogString, "���ļ�����tcount<2 [m_GnPowFilePos=%d; tempFilePos=%d]", m_GnPowFilePos, tempFilePos);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							fclose(stream);
							return -1;
						}
						if (t_delta <= 0)
						{
							sprintf(m_LogString, "���ļ��������ݼ�� t_delta=%f]", t_delta);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							fclose(stream);
							return -1;
						}

						if (tcount < askCount)
						{
							for (jj = 0; jj < (askCount - tcount); jj++)
							{
								m_Matchtime[tcount + jj] = m_Matchtime[tcount + jj - 1] + t_delta;
								for (ii = 0; ii < m_nGnNum; ii++)
								{
									// 									m_pGnDat[ii].pwr[tcount+jj] = m_pGnDat[ii].pwr[tcount-1];
								}
							}
						}
						askCount = tcount;
						m_GnPowFilePos = ftell(stream);
						//m_GnPowFilePos = tempFilePos-m_GnPowFileOffset;
						break;
					}

					fscanf(stream, "%f", &m_Matchtime[kk]);
					for (ii = 0; ii < total1; ii++)
					{
						fscanf(stream, "%f", &pTempdata1[ii].data1[kk]);
					}
					if (kk == (askCount - overCount) - 1)
					{
						m_GnPowFilePos = ftell(stream);
					}

					if (m_GnPowFileOffset <= 0) m_GnPowFileOffset = m_GnPowFilePos - tempFilePos;
				}
			}
			fclose(stream);
		}

		//���������
		tcount = -1;
		total2 = 0;
		tempFilePos = 0;
		askCount = -1;

		TEMPDATA* pTempdata2 = NULL;
		// 		if(c_SampleTimes > 0)  askCount = LFO_CURVE_MAX_PNT*c_SampleTimes;		
		memset(filename, 0, MAXBUF);
#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_GnAngFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_GnAngFileName.c_str());
#endif

		stream = fopen(filename, "r+");
		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetGnPower::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total2);

			if (total1 != total2)
			{
				sprintf(m_LogString, "������й��ļ��͹����ļ��ķ�����������ȣ�");
				MessShow(m_LogString);
			}

			pTempdata2 = (TEMPDATA*)calloc(total2, sizeof(TEMPDATA));
			memset((void*)pTempdata2, 0, total2 * sizeof(TEMPDATA));

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata2[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			// 2008-05-12 by zhw
			if (t_delta > ZEROVALUE)
			{
				askCount = (int)(m_LfoPara.time_leng / t_delta + 0.45);
				overCount = (int)(m_LfoPara.int_time / t_delta + 0.45);
			}

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, "%f", &pTempdata2[ii].data);
			}

			fscanf(stream, "%s", tmpTime);

			for (ii = 0; ii < total2; ii++)
			{
				fscanf(stream, "%s", pTempdata2[ii].name);
			}

			if (m_GnAngFilePos > 0)  fseek(stream, m_GnAngFilePos, SEEK_SET);

			if (total2 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				for (kk = 0; kk < askCount; kk++)
				{
					tempFilePos = ftell(stream);
					//�˳�  check for EOF
					if (feof(stream))
					{
						tcount = kk - 1;
						if (tcount < 1)
						{
							sprintf(m_LogString, "���ļ�����tcount<2 [GnAngFilePos=%d; tempFilePos=%d]", m_GnAngFilePos, tempFilePos);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							if (pTempdata2) { free(pTempdata2);  pTempdata2 = NULL; }
							fclose(stream);
							return -1;
						}
						if (t_delta <= 0)
						{
							sprintf(m_LogString, "���ļ��������ݼ�� t_delta=%f]", t_delta);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							if (pTempdata2) { free(pTempdata2);  pTempdata2 = NULL; }
							fclose(stream);
							return -1;
						}

						if (tcount < askCount)
						{
							for (jj = 0; jj < (askCount - tcount); jj++)
							{
								m_Matchtime[tcount + jj] = m_Matchtime[tcount + jj - 1] + t_delta;
								for (ii = 0; ii < m_nGnNum; ii++)
								{
									// 									m_pGnDat[ii].ang[tcount+jj] = m_pGnDat[ii].ang[tcount-1];
								}
							}
						}
						askCount = tcount;
						m_GnAngFilePos = ftell(stream);
						//m_GnAngFilePos = tempFilePos-m_GnAngFileOffset;
						break;
					}

					fscanf(stream, "%f", &m_Matchtime[kk]);
					for (ii = 0; ii < total2; ii++)
					{
						fscanf(stream, "%f", &pTempdata2[ii].data1[kk]);
					}

					if (kk == (askCount - overCount) - 1)
					{
						m_GnAngFilePos = ftell(stream);
					}

					if (m_GnAngFileOffset <= 0) m_GnAngFileOffset = m_GnAngFilePos - tempFilePos;
				}
			}
			fclose(stream);
		}

		if (askCount > 0)
		{
			realNum = askCount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[askCount - 1];
		}
#ifdef _WAMAP_
		for (ii = 0; ii < total1; ii++)
		{
			for (jj = 0; jj < m_nGnNum; jj++)
			{
				if (pTempdata1[ii].id == m_pGnInf[jj].gn_id)
				{
					//strcpy(m_pGnInf[jj].gn_name, pTempdata1[ii].name);
					//strcpy(m_pGnInf[jj].area_name, pTempdata1[ii].name_1);
// 					m_pGnInf[jj].emws       = pTempdata1[ii].data;
					m_pGnCtl[jj].is_monitor = MENU_LFO_MON_YES;
					for (kk = 0; kk < askCount; kk++)
					{
						// 						m_pGnDat[jj].pwr[kk] = pTempdata1[ii].data1[kk];
					}
					break;
				}
			}
		}
		for (ii = 0; ii < total2; ii++)
		{
			for (jj = 0; jj < m_nGnNum; jj++)
			{
				if (pTempdata2[ii].id == m_pGnInf[jj].gn_id)
				{
					//strcpy(m_pGnInf[jj].gn_name, pTempdata2[ii].name);
					//strcpy(m_pGnInf[jj].area_name, pTempdata2[ii].name_1);
// 					m_pGnInf[jj].emws       = pTempdata2[ii].data;
					m_pGnCtl[jj].is_monitor = MENU_LFO_MON_YES;
					m_pGnMon[jj].gn_id = m_pGnInf[jj].gn_id;
					m_pGnMon[jj].stat = MENU_LFO_DEV_STS_OK;
					//DP_JumpPointHdl(askCount,m_pGnDat[jj].ang,pTempdata2[ii].data1);
					for (kk = 0; kk < askCount; kk++)
					{
						// 						m_pGnDat[jj].ang[kk] = pTempdata2[ii].data1[kk];
					}
					break;
				}
			}
		}
#else
		if (total1 > m_nGnNum)
		{
			total1 = m_nGnNum;
		}
		else
		{
			m_nGnNum = total1;
		}

		for (ii = 0; ii < total1; ii++)
		{
			if (ii > m_nGnNum) break;
			m_pGnInf[ii].gn_id = pTempdata1[ii].id;
			m_pGnDat[ii].gn_id = pTempdata1[ii].id;
			m_pGnCtl[ii].gn_id = pTempdata1[ii].id;
			m_pGnMon[ii].gn_id = pTempdata1[ii].id;
			m_pGnMon[ii].stat = MENU_LFO_DEV_STS_OK;
			strcpy(m_pGnInf[ii].gn_name, pTempdata1[ii].name);
			//             m_pGnInf[ii].emws = pTempdata1[ii].data;
			m_pGnCtl[ii].is_monitor = MENU_LFO_MON_YES;
			for (kk = 0; kk < askCount; kk++)
			{
				//                 m_pGnDat[ii].pwr[kk] = pTempdata1[ii].data1[kk];
				//                 m_pGnDat[ii].ang[kk] = pTempdata2[ii].data1[kk]; 
			}
		}
#endif
		// 2008-06-11 by zhw 
//         if( total1 > 0)
//         {
//             WriteDataToFile( DATA_TYPE_GEN_POW, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }
//         if( total2 > 0)
//         {
//             WriteDataToFile( DATA_TYPE_GEN_ANG, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }

		if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
		if (pTempdata2) { free(pTempdata2);  pTempdata2 = NULL; }

	}
	else  // m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE2 // �����ļ�
	{
		sprintf(m_LogString, "����Դ����");
		MessShow(m_LogString);
		realNum = 0;
	}

	return realNum;
}

// ��ȡ��·���ݣ����ݴ���
int CLfoAnaObj::GetLnData(float& t_start, float& t_end, float& t_delta, int dsource)
{
	if (m_nLnNum < 0 || !m_pLnPmu || !m_pLnDat)  return -1;
	int ii, jj, kk;
	int realNum = 0;
	int newSub, oldSub = 0;

	// ���ݿ�����
	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	{
#ifdef _WAMAP_                                                                                                                

		// ǰ������
		if (m_nMovSec > 0 || m_tNewSec > 0)
		{
			for (ii = 0; ii < m_nLnNum; ii++)
			{
				if (m_pLnPmu[ii].hisSts != MENU_LFO_DEV_STS_NOYC)
				{
					for (jj = 0; jj < m_LfoPara.int_time; jj++)
					{
						m_pLnDat[ii].SecSts[jj] = m_pLnDat[ii].SecSts[jj + m_nMovSec];

						newSub = MAX_FRA_PER_SEC * jj;
						oldSub = MAX_FRA_PER_SEC * (jj + m_nMovSec);
						for (kk = 0; kk < MAX_FRA_PER_SEC; kk++)
						{
							// 							m_pLnDat[ii].pwr[newSub+kk] = m_pLnDat[ii].pwr[oldSub+kk];
						}
					}
				}
			}
		}
		for (jj = m_LfoPara.int_time; jj < m_LfoPara.int_time + m_nMovSec; jj++)
		{
			for (ii = 0; ii < m_nLnNum; ii++)
			{
				m_pLnDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_OK;
				if (m_pLnCtl[ii].is_monitor == MENU_LFO_MENU_NO) m_pLnDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_NO;
				if (m_pLnPmu[ii].hisSts == MENU_LFO_DEV_STS_NOYC) m_pLnDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_NOYC;
			}
		}

		// ȡ�����ݣ������
		int checkSec = 0;
		for (ii = 0; ii < m_nLnNum; ii++)
		{
			if (m_pLnPmu[ii].hisSts != MENU_LFO_DEV_STS_NOYC && m_pLnCtl[ii].is_monitor != MENU_LFO_MENU_NO)
			{
				//////////////////////////////////////////////////////////////////////////
				// 2008-04-05 added by zhw at yn
				// slct_dot_id �ѳ�ʼ��Ϊĩ��
				if (m_pLnPmu[ii].vecAmpl1.viFilNo >= 0 && m_pLnPmu[ii].vecAmpl2.viFilNo >= 0)
				{
					// 2008-07-10 by zhw for xb
					if (m_pLnCtl[ii].is_end == MENU_LFO_MENU_YES)
					{
						m_pLnPmu[ii].is_end = MENU_LFO_MENU_YES;
						m_pLnMon[ii].slct_dot_id = m_pLnPmu[ii].dot_id_2;
					}
					else
					{
						m_pLnPmu[ii].is_end = MENU_LFO_MENU_NO;
						m_pLnMon[ii].slct_dot_id = m_pLnPmu[ii].dot_id_1;
					}
				}
				else if (m_pLnPmu[ii].vecAmpl2.viFilNo >= 0)
				{
					m_pLnPmu[ii].is_end = MENU_LFO_MENU_YES;
					m_pLnMon[ii].slct_dot_id = m_pLnPmu[ii].dot_id_2;
				}
				else
				{
					m_pLnPmu[ii].is_end = MENU_LFO_MENU_NO;
					m_pLnMon[ii].slct_dot_id = m_pLnPmu[ii].dot_id_1;
				}
				//////////////////////////////////////////////////////////////////////////

				if (GetLnDataFromWAMAP(m_pLnPmu[ii], m_tNewSec, m_nMovSec, m_pLnDat[ii], m_LfoPara.int_time) == false)
				{
					for (jj = 0; jj < m_nMovSec; jj++)
					{
						m_pLnDat[ii].SecSts[jj + m_LfoPara.int_time] = MENU_LFO_DEV_STS_OLD;
					}
				}
				// ���ݼ�� ��·ͣ�� // ����ͻ��������쳣����֯����ǰ�ж�
// 				for ( jj =0; jj<m_nMovSec; jj++ )
// 				{
// 					checkSec = m_LfoPara.int_time + jj;
// 
//                     // 2008-06-02 by zhw at nj
//                     if (m_pLnDat[ii].SecSts[checkSec] == MENU_LFO_DEV_STS_OK)//MENU_LFO_DEV_STS_OFF)
// 					{
// 						newSub = checkSec * MAX_FRA_PER_SEC;
// 						for (kk = 0; kk< MAX_FRA_PER_SEC; kk++)
// 						{
// 							if (fabs(m_pLnDat[ii].pwr[newSub+kk]) > 1.0f ) break;
// 						}
// 
// 						if (kk == MAX_FRA_PER_SEC ) m_pLnDat[ii].SecSts[checkSec] = MENU_LFO_DEV_STS_STP;
// 					}
// 				}
			}
		}
		realNum = MAX_FRA_PER_SEC * m_nMovSec;
		//  ���ݱ���  2008-06-10 by zhw
		//WriteDataToFile( DATA_TYPE_ACLN_POW, realNum, m_tNewSec, m_nMovSec, t_delta, m_LfoPara.int_time );
#endif
	}
	else if (m_DispCtl.dsource == MENU_LFO_DATA_FILE) // �����ļ�
	{
		// �ӱ����ļ�ȡ����               
		//sprintf( m_LogString, "GetLnData()���������Ա����ļ���ѭ������");
		//MessShow( m_LogString );
		// �����ݶ��� m_pLnDat �У�������״̬
		// m_pLnDat

//		int   bad_int = 0;
//		float bad_float = 0;
//		char  bad_name[64];
		char  filename[MAXBUF];
		memset(filename, 0, MAXBUF);
		//int   nTempLnNum;

		int total1 = 0;
		//int total2 = 0;
		int tcount = -1;

		TEMPDATA* pTempdata1 = NULL;
		char   buff[MAXBUF];
		string  sTempTime;
		int     TempInt;
		memset(buff, 0, MAXBUF);
#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_LnPowFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_LnPowFileName.c_str());
#endif

		FILE* stream = fopen(filename, "r+");
		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetFcVolt::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total1);

			pTempdata1 = (TEMPDATA*)calloc(total1, sizeof(TEMPDATA));
			memset((void*)pTempdata1, 0, total1 * sizeof(TEMPDATA));

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata1[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%s", pTempdata1[ii].name);
			}

			if (total1 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				tcount = -1;
				while (feof(stream) == 0)
				{
					tcount++;
					// 2008-05-12 by zhw ѭ����ȡԽ��
					if (tcount >= DATA_NUM_MAX)
					{
						sprintf(m_LogString, "��������Խ�ޣ�%d > %d", tcount, DATA_NUM_MAX);
						MessShow(m_LogString);
						tcount--;
						break;
					}
					fscanf(stream, "%f", &m_Matchtime[tcount]);

					for (ii = 0; ii < total1; ii++)
					{
						fscanf(stream, "%f", &pTempdata1[ii].data1[tcount]);
					}
				}
			}

			fclose(stream);
		}

		if (tcount > 1)
		{
			realNum = tcount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[tcount - 1];
		}
#ifdef _WAMAP_
		for (ii = 0; ii < total1; ii++)
		{
			for (jj = 0; jj < m_nLnNum; jj++)
			{
				if (pTempdata1[ii].id == m_pLnInf[jj].acln_id)
				{
					strcpy(m_pLnInf[jj].acln_name, pTempdata1[ii].name);
					m_pLnCtl[jj].is_monitor = MENU_LFO_MON_YES;
					for (kk = 0; kk < tcount; kk++)
					{
						// 						m_pLnDat[jj].pwr[kk] = pTempdata1[ii].data1[kk];
					}
					break;
				}
			}
		}
#else
		if (total1 > m_nLnNum)
		{
			total1 = m_nLnNum;
		}
		else
		{
			m_nLnNum = total1;
		}

		for (ii = 0; ii < total1; ii++)
		{
			if (ii > m_nLnNum) break;
			m_pLnInf[ii].acln_id = pTempdata1[ii].id;
			m_pLnDat[ii].acln_id = pTempdata1[ii].id;
			m_pLnCtl[ii].acln_id = pTempdata1[ii].id;
			m_pLnMon[ii].acln_id = pTempdata1[ii].id;
			strcpy(m_pLnInf[ii].acln_name, pTempdata1[ii].name);
			m_pLnCtl[ii].is_monitor = MENU_LFO_MON_YES;
			for (kk = 0; kk < tcount; kk++)
			{
				m_pLnDat[ii].pwr[kk] = pTempdata1[ii].data1[kk];
			}
		}
#endif
		//  ���ݱ���  2008-06-10 by zhw
//         if( total1 > 0)
//         {
//             WriteDataToFile( DATA_TYPE_ACLN_POW, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }

		if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
	}
	else if (m_DispCtl.dsource >= MENU_LFO_CON_DATA_FILE) // �����ļ�
	{
		// �ӱ����ļ�ȡ����               
		//sprintf( m_LogString, "GetLnData()���������Ա����ļ�����������");
		//MessShow( m_LogString );

		//�������й�
		char  filename[MAXBUF];
		memset(filename, 0, MAXBUF);
		//int   nTempLnNum;		
		int			total1 = 0;
		//int			total2 = 0;
		int         tcount = -1;
		int         tempFilePos;
		int         askCount = -1;
		int         overCount = -1;  // ����ʱ�����ݵ��� 2008-06-08 by zhw

		TEMPDATA* pTempdata1 = NULL;
		char    buff[MAXBUF];
		string  sTempTime;
		int     TempInt;
		memset(buff, 0, MAXBUF);
		// 		if(c_SampleTimes > 0)  askCount = LFO_CURVE_MAX_PNT*c_SampleTimes;		

#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_LnPowFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_LnPowFileName.c_str());
#endif

		FILE* stream = fopen(filename, "r+");
		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetLnData::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total1);

			pTempdata1 = (TEMPDATA*)calloc(total1, sizeof(TEMPDATA));
			memset((void*)pTempdata1, 0, total1 * sizeof(TEMPDATA));

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata1[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			if (t_delta > ZEROVALUE)
			{
				askCount = (int)(m_LfoPara.time_leng / t_delta + 0.45);
				overCount = (int)(m_LfoPara.int_time / t_delta + 0.45);
			}

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%s", pTempdata1[ii].name);
			}

			if (m_LnDataFilePos > 0)
			{
				fseek(stream, m_LnDataFilePos, SEEK_SET);
			}
			// 			sprintf(m_LogString,"GetLnData():���ļ�ƫ��λ�ã�m_LnDataFilePos=%d",m_LnDataFilePos);                                               
			// 			MessShow(m_LogString);      

			if (total1 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				for (kk = 0; kk < askCount; kk++)
				{
					tempFilePos = ftell(stream);
					//�˳�  check for EOF
					if (feof(stream))
					{
						tcount = kk - 1;
						if (tcount < 1)
						{
							sprintf(m_LogString, "���ļ�����tcount<2 [m_LnDataFilePos=%d; tempFilePos=%d]", m_LnDataFilePos, tempFilePos);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							fclose(stream);
							return -1;
						}
						if (t_delta <= 0)
						{
							sprintf(m_LogString, "���ļ��������ݼ�� [t_delta=%f]", t_delta);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							fclose(stream);
							return -1;
						}

						if (tcount < askCount)
						{
							for (jj = 0; jj < (askCount - tcount); jj++)
							{
								m_Matchtime[tcount + jj] = m_Matchtime[tcount + jj - 1] + t_delta;
								for (ii = 0; ii < m_nLnNum; ii++)
								{
									// 									m_pLnDat[ii].pwr[tcount+jj] = m_pLnDat[ii].pwr[tcount-1];
								}
							}
						}
						askCount = tcount;
						m_LnDataFilePos = ftell(stream);
						//m_LnDataFilePos = tempFilePos-m_LnDataFileOffset;
						break;
					}

					fscanf(stream, "%f", &m_Matchtime[kk]);
					for (ii = 0; ii < total1; ii++)
					{
						fscanf(stream, "%f", &pTempdata1[ii].data1[kk]);
					}
					//m_LnDataFilePos = ftell(stream);  2008-05-08 by zhw

					if (kk == (askCount - overCount) - 1)  // 2008-05-08 by zhw
					{
						m_LnDataFilePos = ftell(stream);
					}
					if (m_LnDataFileOffset == 0) m_LnDataFileOffset = m_LnDataFilePos - tempFilePos;
				}
			}
			fclose(stream);
		}

		if (askCount > 0)
		{
			realNum = askCount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[askCount - 1];
		}
#ifdef _WAMAP_
		for (ii = 0; ii < total1; ii++)
		{
			for (jj = 0; jj < m_nLnNum; jj++)
			{
				if (pTempdata1[ii].id == m_pLnInf[jj].acln_id)
				{
					//strcpy(m_pLnInf[jj].acln_name, pTempdata1[ii].name);
					m_pLnCtl[jj].is_monitor = MENU_LFO_MON_YES;
					m_pLnMon[jj].ln_id = m_pLnInf[jj].acln_id;
					m_pLnMon[jj].stat = MENU_LFO_DEV_STS_OK;
					if (m_pLnPmu[jj].vecAmpl2.viFilNo >= 0 && m_pLnPmu[jj].vecAmpl2.viFilNo >= 0)
					{
						// 2008-07-10 by zhw for xb
						if (m_pLnCtl[jj].is_end == MENU_LFO_MENU_YES)
						{
							m_pLnPmu[jj].is_end = MENU_LFO_MENU_YES;
							m_pLnMon[jj].slct_dot_id = m_pLnPmu[jj].dot_id_2;
						}
						else
						{
							m_pLnPmu[jj].is_end = MENU_LFO_MENU_NO;
							m_pLnMon[jj].slct_dot_id = m_pLnPmu[jj].dot_id_1;
						}
					}
					else if (m_pLnPmu[jj].vecAmpl2.viFilNo >= 0)
					{
						m_pLnPmu[jj].is_end = MENU_LFO_MENU_YES;
						m_pLnMon[jj].slct_dot_id = m_pLnPmu[jj].dot_id_2;
					}
					else
					{
						m_pLnPmu[jj].is_end = MENU_LFO_MENU_NO;
						m_pLnMon[jj].slct_dot_id = m_pLnPmu[jj].dot_id_1;
					}
					for (kk = 0; kk < askCount; kk++)
					{
						//                         m_pLnDat[jj].pwr[kk] = pTempdata1[ii].data1[kk];
					}
					break;
				}
			}
		}
#else
		if (total1 > m_nLnNum)
		{
			total1 = m_nLnNum;
		}
		else
		{
			m_nLnNum = total1;
		}

		for (ii = 0; ii < total1; ii++)
		{
			if (ii > m_nLnNum)  break;
			m_pLnInf[ii].acln_id = pTempdata1[ii].id;
			m_pLnDat[ii].ln_id = pTempdata1[ii].id;
			m_pLnCtl[ii].acln_id = pTempdata1[ii].id;
			m_pLnMon[ii].ln_id = pTempdata1[ii].id;
			m_pLnMon[ii].stat = MENU_LFO_DEV_STS_OK;
			strcpy(m_pLnInf[ii].acln_name, pTempdata1[ii].name);
			m_pLnCtl[ii].is_monitor = MENU_LFO_MON_YES;
			for (kk = 0; kk < askCount; kk++)
			{
				//                 m_pLnDat[ii].pwr[kk] = pTempdata1[ii].data1[kk];
			}
		}
#endif
		//  ���ݱ���  2008-06-10 by zhw
//         if( total1 > 0)
//         {
//             WriteDataToFile( DATA_TYPE_ACLN_POW, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }

		if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }

	}
	else  // m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE2 // �����ļ�
	{
		sprintf(m_LogString, "����Դ����");
		MessShow(m_LogString);
		realNum = 0;
	}

	return realNum;
}

// ��ȡ��ѹ�����ݣ����ݴ���
int CLfoAnaObj::GetTrData(float& t_start, float& t_end, float& t_delta, int dsource)
{
	if (m_nTrNum < 0 || !m_pTrPmu || !m_pTrDat)  return -1;
	int ii, jj, kk;
	int realNum = 0;
	int newSub, oldSub = 0;

	// ���ݿ�����
	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	{
#ifdef _WAMAP_                                                                                                                

		// ǰ������
		if (m_nMovSec > 0 || m_tNewSec > 0)
		{
			for (ii = 0; ii < m_nTrNum; ii++)
			{
				if (m_pTrPmu[ii].hisSts != MENU_LFO_DEV_STS_NOYC)
				{
					for (jj = 0; jj < m_LfoPara.int_time; jj++)
					{
						m_pTrDat[ii].SecSts[jj] = m_pTrDat[ii].SecSts[jj + m_nMovSec];

						newSub = MAX_FRA_PER_SEC * jj;
						oldSub = MAX_FRA_PER_SEC * (jj + m_nMovSec);
						for (kk = 0; kk < MAX_FRA_PER_SEC; kk++)
						{
							// 							m_pTrDat[ii].pwr[newSub+kk] = m_pTrDat[ii].pwr[oldSub+kk];
						}
					}
				}
			}
		}
		for (jj = m_LfoPara.int_time; jj < m_LfoPara.int_time + m_nMovSec; jj++)
		{
			for (ii = 0; ii < m_nTrNum; ii++)
			{
				m_pTrDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_OK;
				if (m_pTrCtl[ii].is_monitor == MENU_LFO_MENU_NO) m_pTrDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_NO;
				if (m_pTrPmu[ii].hisSts == MENU_LFO_DEV_STS_NOYC) m_pTrDat[ii].SecSts[jj] = MENU_LFO_DEV_STS_NOYC;
			}
		}

		// ȡ�����ݣ������
		int checkSec = 0;
		for (ii = 0; ii < m_nTrNum; ii++)
		{
			if (m_pTrPmu[ii].hisSts != MENU_LFO_DEV_STS_NOYC && m_pTrCtl[ii].is_monitor != MENU_LFO_MENU_NO)
			{
				//                 if( m_pTrCtl[ii].mon_wind_slct == HIGH_WINDING )
				//                 {
				if (m_pTrPmu[ii].hwd_yc_ok == MENU_LFO_MENU_YES)
				{
					m_pTrMon[ii].slct_wind_id = m_pTrPmu[ii].high_wind_id;
					m_pTrPmu[ii].slct_wind_id = m_pTrPmu[ii].high_wind_id;
					//                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[ii].is_pow_calc;
				}
				else if (m_pTrPmu[ii].mwd_yc_ok == MENU_LFO_MENU_YES)
				{
					m_pTrMon[ii].slct_wind_id = m_pTrPmu[ii].mid_wind_id;
					m_pTrPmu[ii].slct_wind_id = m_pTrPmu[ii].mid_wind_id;
					//                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[ii].is_pow_calc;
				}
				else if (m_pTrPmu[ii].lwd_yc_ok == MENU_LFO_MENU_YES)
				{
					m_pTrMon[ii].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					m_pTrPmu[ii].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					//                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
				}
				else
				{
					sprintf(m_LogString, "tr_id=%s["INT_REC_FMT"]��㲻����!", m_pTrPmu[ii].tr_name, INT_REC_VAL(m_pTrPmu[ii].tr_id));
					MessShow(m_LogString);
				}
				//                 }
				//                 else if( m_pTrCtl[ii].slct_wind_id == m_pTrCtl[ii].mid_wind_id )
				//                 {
				//                     if( m_pTrPmu[ii].mwd_yc_ok == MENU_LFO_MENU_YES )
				//                     {
				//                         m_pTrMon[ii].slct_wind_id = m_pTrPmu[ii].mid_wind_id;
				//                         m_pTrPmu[ii].slct_wind_id = m_pTrPmu[ii].mid_wind_id;
				// //                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[ii].is_pow_calc;
				//                     }
				//                     else if( m_pTrPmu[ii].hwd_yc_ok == MENU_LFO_MENU_YES )
				//                     {
				//                         m_pTrMon[ii].slct_wind_id = m_pTrPmu[ii].high_wind_id;
				//                         m_pTrPmu[ii].slct_wind_id = m_pTrPmu[ii].high_wind_id;
				// //                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[ii].is_pow_calc;
				//                     }
				//                     else if( m_pTrPmu[ii].lwd_yc_ok == MENU_LFO_MENU_YES )
				//                     {
				//                         m_pTrMon[ii].slct_wind_id = m_pTrPmu[ii].low_wind_id;
				//                         m_pTrPmu[ii].slct_wind_id = m_pTrPmu[ii].low_wind_id;
				// //                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[ii].is_pow_calc;
				//                     }
				//                     else
				//                     {
				//                         sprintf(m_LogString,"tr_id=%s["INT_REC_FMT"]��㲻����!",m_pTrPmu[ii].tr_name,m_pTrPmu[ii].tr_id);
				//                         MessShow(m_LogString);
				//                     }
				//                 }
				//                 else if( m_pTrCtl[ii].slct_wind_id == m_pTrCtl[ii].low_wind_id )
				//                 {
				//                     if( m_pTrPmu[ii].lwd_yc_ok == MENU_LFO_MENU_YES )
				//                     {
				//                         m_pTrMon[ii].slct_wind_id = m_pTrPmu[ii].low_wind_id;
				//                         m_pTrPmu[ii].slct_wind_id = m_pTrPmu[ii].low_wind_id;
				// //                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[ii].is_pow_calc;
				//                     }
				//                     else if( m_pTrPmu[ii].hwd_yc_ok == MENU_LFO_MENU_YES )
				//                     {
				//                         m_pTrMon[ii].slct_wind_id = m_pTrPmu[ii].high_wind_id;
				//                         m_pTrPmu[ii].slct_wind_id = m_pTrPmu[ii].high_wind_id;
				// //                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[ii].is_pow_calc;
				//                     }
				//                     else if( m_pTrPmu[ii].mwd_yc_ok == MENU_LFO_MENU_YES )
				//                     {
				//                         m_pTrMon[ii].slct_wind_id = m_pTrPmu[ii].mid_wind_id;
				//                         m_pTrPmu[ii].slct_wind_id = m_pTrPmu[ii].mid_wind_id;
				// //                         m_pTrPmu[ii].is_pow_calc = m_pTrCtl[ii].is_pow_calc;
				//                     }
				//                     else
				//                     {
				//                         sprintf(m_LogString,"tr_id=%s["INT_REC_FMT"]��㲻����!",m_pTrPmu[ii].tr_name,m_pTrPmu[ii].tr_id);
				//                         MessShow(m_LogString);
				//                     }
				//                 }

				if (GetTrDataFromWAMAP(m_pTrPmu[ii], m_tNewSec, m_nMovSec, m_pTrDat[ii], m_LfoPara.int_time) == false)
				{
					for (jj = 0; jj < m_nMovSec; jj++)
					{
						m_pTrDat[ii].SecSts[jj + m_LfoPara.int_time] = MENU_LFO_DEV_STS_OLD;
					}
				}
				// 				for ( jj=0; jj< m_nMovSec*MAX_FRA_PER_SEC;jj++)
				// 				{
				// 					sprintf(m_LogString,"tr_id=%s["INT_REC_FMT"]��ȡ����[%d],freq[%f],ampl[%f]!",m_pTrPmu[ii].tr_name,m_pTrPmu[ii].tr_id,m_pTrDat[ii].sso[jj],m_pTrDat[ii].freq[jj],m_pTrDat[ii].ampl[jj]);
				// 					MessShow(m_LogString);
				// 				}
				// 				sprintf(m_LogString,"����ȡ[%d]֡����",sizeof(m_pTrDat[ii].sso)/sizeof(int));
				// 				MessShow(m_LogString);
								// ���ݼ�� ��·ͣ�� // ����ͻ��������쳣����֯����ǰ�ж�
				// 				for ( jj =0; jj<m_nMovSec; jj++ )
				// 				{
				// 					checkSec = m_LfoPara.int_time + jj;
				// 
				//                     // 2008-06-02 by zhw at nj
				//                     if (m_pTrDat[ii].SecSts[checkSec] == MENU_LFO_DEV_STS_OK)//MENU_LFO_DEV_STS_OFF)
				// 					{
				// 						newSub = checkSec * MAX_FRA_PER_SEC;
				// 						for (kk = 0; kk< MAX_FRA_PER_SEC; kk++)
				// 						{
				// 							if (fabs(m_pTrDat[ii].pwr[newSub+kk]) > 1.0f ) break;
				// 						}
				// 
				// 						if (kk == MAX_FRA_PER_SEC ) m_pTrDat[ii].SecSts[checkSec] = MENU_LFO_DEV_STS_STP;
				// 					}
				// 				}
			}
		}
		realNum = MAX_FRA_PER_SEC * m_nMovSec;
		//  ���ݱ���  2008-06-10 by zhw
		//WriteDataToFile( DATA_TYPE_ACLN_POW, realNum, m_tNewSec, m_nMovSec, t_delta, m_LfoPara.int_time );
#endif
	}
	else if (m_DispCtl.dsource == MENU_LFO_DATA_FILE) // �����ļ�
	{
		// �ӱ����ļ�ȡ����               
		//sprintf( m_LogString, "GetTrData()���������Ա����ļ���ѭ������");
		//MessShow( m_LogString );
		// �����ݶ��� m_pTrDat �У�������״̬
		// m_pTrDat

//		int   bad_int = 0;
//		float bad_float = 0;
//		char  bad_name[64];
		char  filename[MAXBUF];
		memset(filename, 0, MAXBUF);
		//int   nTempLnNum;

		int total1 = 0;
		//int total2 = 0;
		int tcount = -1;

		TEMPDATA* pTempdata1 = NULL;
		char        buff[MAXBUF];
		string      sTempTime;
		int         TempInt;
		memset(buff, 0, MAXBUF);

#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_TrPowFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_TrPowFileName.c_str());
#endif

		FILE* stream = fopen(filename, "r+");
		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetGnPower::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total1);

			pTempdata1 = (TEMPDATA*)calloc(total1, sizeof(TEMPDATA));
			memset((void*)pTempdata1, 0, total1 * sizeof(TEMPDATA));

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata1[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%s", pTempdata1[ii].name);
			}

			if (total1 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				tcount = -1;
				while (feof(stream) == 0)
				{
					tcount++;
					// 2008-05-12 by zhw ѭ����ȡԽ��
					if (tcount >= DATA_NUM_MAX)
					{
						sprintf(m_LogString, "��������Խ�ޣ�%d > %d", tcount, DATA_NUM_MAX);
						MessShow(m_LogString);
						tcount--;
						break;
					}
					fscanf(stream, "%f", &m_Matchtime[tcount]);

					for (ii = 0; ii < total1; ii++)
					{
						fscanf(stream, "%f", &pTempdata1[ii].data1[tcount]);
					}
				}
			}

			fclose(stream);
		}

		if (tcount > 1)
		{
			realNum = tcount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[tcount - 1];
		}
#ifdef _WAMAP_
		for (ii = 0; ii < total1; ii++)
		{
			for (jj = 0; jj < m_nTrNum; jj++)
			{
				if (pTempdata1[ii].id == m_pTrInf[jj].tr_id)
				{
					strcpy(m_pTrInf[jj].tr_name, pTempdata1[ii].name);
					m_pTrCtl[jj].is_monitor = MENU_LFO_MON_YES;
					//                     if( m_pTrCtl[jj].mon_wind_slct == HIGH_WINDING )
					{
						if (m_pTrPmu[jj].hwd_yc_ok == MENU_LFO_MENU_YES)
						{
							m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
							m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
							//                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
						}
						else if (m_pTrPmu[jj].mwd_yc_ok == MENU_LFO_MENU_YES)
						{
							m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
							m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
							//                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
						}
						else if (m_pTrPmu[jj].lwd_yc_ok == MENU_LFO_MENU_YES)
						{
							m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
							m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
							//                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
						}

					}
					//                     else if( m_pTrCtl[jj].mon_wind_slct == MID_WINDING )
					//                     {
					//                         if( m_pTrPmu[jj].mwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                         else if( m_pTrPmu[jj].hwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                         else if( m_pTrPmu[jj].lwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                     }
					//                     else if( m_pTrCtl[jj].mon_wind_slct == LOW_WINDING )
					//                     {
					//                         if( m_pTrPmu[jj].lwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                         else if( m_pTrPmu[jj].hwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                         else if( m_pTrPmu[jj].mwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                     }
					for (kk = 0; kk < tcount; kk++)
					{
						// 						m_pTrDat[jj].pwr[kk] = pTempdata1[ii].data1[kk];
					}
					break;
				}
			}
		}
#else
		if (total1 > m_nTrNum)
		{
			total1 = m_nTrNum;
		}
		else
		{
			m_nTrNum = total1;
		}

		for (ii = 0; ii < total1; ii++)
		{
			if (ii > m_nTrNum) break;
			m_pTrInf[ii].tr_id = pTempdata1[ii].id;
			m_pTrDat[ii].tr_id = pTempdata1[ii].id;
			m_pTrCtl[ii].tr_id = pTempdata1[ii].id;
			m_pTrMon[ii].tr_id = pTempdata1[ii].id;
			strcpy(m_pTrInf[ii].tr_name, pTempdata1[ii].name);
			m_pTrCtl[ii].is_monitor = MENU_LFO_MON_YES;
			for (kk = 0; kk < tcount; kk++)
			{
				m_pTrDat[ii].pwr[kk] = pTempdata1[ii].data1[kk];
			}
		}
#endif
		//  ���ݱ���  2008-06-10 by zhw
//         if( total1 > 0)
//         {
//             //WriteDataToFile( DATA_TYPE_ACLN_POW, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }

		if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
	}
	else if (m_DispCtl.dsource >= MENU_LFO_CON_DATA_FILE) // �����ļ�
	{
		// �ӱ����ļ�ȡ����               
		//sprintf( m_LogString, "GetTrData()���������Ա����ļ�����������");
		//MessShow( m_LogString );

		//�������й�
		char  filename[MAXBUF];
		memset(filename, 0, MAXBUF);
		//int   nTempLnNum;		
		int			total1 = 0;
		//int			total2 = 0;
		int         tcount = -1;
		int         tempFilePos;
		int         askCount = -1;
		int         overCount = -1;  // ����ʱ�����ݵ��� 2008-06-08 by zhw

		TEMPDATA* pTempdata1 = NULL;
		char        buff[MAXBUF];
		string      sTempTime;
		int         TempInt;
		memset(buff, 0, MAXBUF);
		// 		if(c_SampleTimes > 0)  askCount = LFO_CURVE_MAX_PNT*c_SampleTimes;		

#ifdef _WINDOWS32
		sprintf(filename, "%s\\%s.txt", m_DataFileDir, m_TrPowFileName.c_str());
#else
		sprintf(filename, "%s/%s.txt", m_DataFileDir, m_TrPowFileName.c_str());
#endif

		FILE* stream = fopen(filename, "r+");
		if (stream != NULL)
		{
			fscanf(stream, "%d", &para_FileType);
			fgets(buff, MAXBUF, stream);
			buff[strlen(buff) - 1] = '\0';
			sTempTime = buff;
			TempInt = PT_GetSecondByTimeString(sTempTime.c_str());
			if (TempInt < 0)
			{
				//                 sprintf( m_LogString, "GetGnPower::mktime failed when transfer nGpsTime! ");
				//                 MessShow( m_LogString );
			}
			else
			{
				para_nGpsTime = TempInt;
			}
			fscanf(stream, "%d", &total1);

			pTempdata1 = (TEMPDATA*)calloc(total1, sizeof(TEMPDATA));
			memset((void*)pTempdata1, 0, total1 * sizeof(TEMPDATA));

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, ""INT_REC_FMT"", INT_REC_VAL(pTempdata1[ii].id));
			}

			fscanf(stream, "%f", &t_delta);

			if (t_delta > ZEROVALUE)
			{
				askCount = (int)(m_LfoPara.time_leng / t_delta + 0.45);
				overCount = (int)(m_LfoPara.int_time / t_delta + 0.45);
			}

			for (ii = 0; ii < total1; ii++)
			{
				fscanf(stream, "%s", pTempdata1[ii].name);
			}

			if (m_TrDataFilePos > 0)
			{
				fseek(stream, m_TrDataFilePos, SEEK_SET);
			}
			// 			sprintf(m_LogString,"GetTrData():���ļ�ƫ��λ�ã�m_TrDataFilePos=%d",m_TrDataFilePos);                                               
			// 			MessShow(m_LogString);      

			if (total1 > 0)
			{
				memset(m_Matchtime, 0, DATA_NUM_MAX * sizeof(float));
				for (kk = 0; kk < askCount; kk++)
				{
					tempFilePos = ftell(stream);
					//�˳�  check for EOF
					if (feof(stream))
					{
						tcount = kk - 1;
						if (tcount < 1)
						{
							sprintf(m_LogString, "���ļ�����tcount<2 [m_TrDataFilePos=%d; tempFilePos=%d]", m_TrDataFilePos, tempFilePos);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							fclose(stream);
							return -1;
						}
						if (t_delta <= 0)
						{
							sprintf(m_LogString, "���ļ��������ݼ�� [t_delta=%f]", t_delta);
							MessShow(m_LogString);
							if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }
							fclose(stream);
							return -1;
						}

						if (tcount < askCount)
						{
							for (jj = 0; jj < (askCount - tcount); jj++)
							{
								m_Matchtime[tcount + jj] = m_Matchtime[tcount + jj - 1] + t_delta;
								for (ii = 0; ii < m_nTrNum; ii++)
								{
									// 									m_pTrDat[ii].pwr[tcount+jj] = m_pTrDat[ii].pwr[tcount-1];
								}
							}
						}
						askCount = tcount;
						m_TrDataFilePos = ftell(stream);
						//m_TrDataFilePos = tempFilePos-m_TrDataFileOffset;
						break;
					}

					fscanf(stream, "%f", &m_Matchtime[kk]);
					for (ii = 0; ii < total1; ii++)
					{
						fscanf(stream, "%f", &pTempdata1[ii].data1[kk]);
					}
					//m_TrDataFilePos = ftell(stream);  2008-05-08 by zhw

					if (kk == (askCount - overCount) - 1)  // 2008-05-08 by zhw
					{
						m_TrDataFilePos = ftell(stream);
					}
					if (m_TrDataFileOffset == 0) m_TrDataFileOffset = m_TrDataFilePos - tempFilePos;
				}
			}
			fclose(stream);
		}

		if (askCount > 0)
		{
			realNum = askCount;
			t_start = m_Matchtime[0];
			t_end = m_Matchtime[askCount - 1];
		}
#ifdef _WAMAP_
		for (ii = 0; ii < total1; ii++)
		{
			for (jj = 0; jj < m_nTrNum; jj++)
			{
				if (pTempdata1[ii].id == m_pTrInf[jj].tr_id)
				{
					//strcpy(m_pLnInf[jj].acln_name, pTempdata1[ii].name);
					m_pTrCtl[jj].is_monitor = MENU_LFO_MON_YES;
					m_pTrMon[jj].tr_id = m_pTrInf[jj].tr_id;
					m_pTrMon[jj].stat = MENU_LFO_DEV_STS_OK;
					//                     if( m_pTrCtl[jj].mon_wind_slct == HIGH_WINDING )
					{
						if (m_pTrPmu[jj].hwd_yc_ok == MENU_LFO_MENU_YES)
						{
							m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
							m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
							//                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
						}
						else if (m_pTrPmu[jj].mwd_yc_ok == MENU_LFO_MENU_YES)
						{
							m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
							m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
							//                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
						}
						else if (m_pTrPmu[jj].lwd_yc_ok == MENU_LFO_MENU_YES)
						{
							m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
							m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
							//                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
						}

					}
					//                     else if( m_pTrCtl[jj].mon_wind_slct == MID_WINDING )
					//                     {
					//                         if( m_pTrPmu[jj].mwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                         else if( m_pTrPmu[jj].hwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                         else if( m_pTrPmu[jj].lwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                     }
					//                     else if( m_pTrCtl[jj].mon_wind_slct == LOW_WINDING )
					//                     {
					//                         if( m_pTrPmu[jj].lwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].low_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                         else if( m_pTrPmu[jj].hwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].high_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                         else if( m_pTrPmu[jj].mwd_yc_ok == MENU_LFO_MENU_YES )
					//                         {
					//                             m_pTrMon[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
					//                             m_pTrPmu[jj].slct_wind_id = m_pTrPmu[jj].mid_wind_id;
					// //                             m_pTrPmu[jj].is_pow_calc = m_pTrCtl[jj].is_pow_calc;
					//                         }
					//                     }
					for (kk = 0; kk < askCount; kk++)
					{
						//                         m_pTrDat[jj].pwr[kk] = pTempdata1[ii].data1[kk];
					}
					break;
				}
			}
		}
#else
		if (total1 > m_nTrNum)
		{
			total1 = m_nTrNum;
		}
		else
		{
			m_nTrNum = total1;
		}

		for (ii = 0; ii < total1; ii++)
		{
			if (ii > m_nTrNum)  break;
			m_pTrInf[ii].tr_id = pTempdata1[ii].id;
			m_pTrDat[ii].tr_id = pTempdata1[ii].id;
			m_pTrCtl[ii].tr_id = pTempdata1[ii].id;
			m_pTrMon[ii].tr_id = pTempdata1[ii].id;
			m_pTrMon[ii].stat = MENU_LFO_DEV_STS_OK;
			strcpy(m_pTrInf[ii].tr_name, pTempdata1[ii].name);
			m_pTrCtl[ii].is_monitor = MENU_LFO_MON_YES;
			for (kk = 0; kk < askCount; kk++)
			{
				//                 m_pTrDat[ii].pwr[kk] = pTempdata1[ii].data1[kk];
			}
		}
#endif
		//  ���ݱ���  2008-06-10 by zhw
//         if( total1 > 0)
//         {
//             //WriteDataToFile( DATA_TYPE_ACLN_POW, realNum, t_start, t_end, t_delta, m_LfoPara.int_time );
//         }

		if (pTempdata1) { free(pTempdata1);  pTempdata1 = NULL; }

	}
	else  // m_DispCtl.dsource == MENU_LFO_CON_DATA_FILE2 // �����ļ�
	{
		sprintf(m_LogString, "����Դ����");
		MessShow(m_LogString);
		realNum = 0;
	}
	return realNum;
}

//  ����������Ԥ����(gn, ln)
//  ����Ԥ����(�����Լ�顢�˲���������Ԥ�С����)
//     int                  m_nSmpFre;                         //  ����Ƶ��
//     int                  m_nSmpStp;                         //  �������
int CLfoAnaObj::FormMonProData()
{
	int             ii, jj, kk;
	//float           ampl_max     = 0;
	//float           ampl_max_rat = 0;
	//int             findFlag     = 0 ;
	float           temp_iBuffer[DATA_NUM_MAX];
	SEQ_BASE_INFO   dataBaseInfo;

	int             ChgTimCoe = (int)(m_nOrigDlt * BAS_FRAME_PERSEC + 0.45);
	if (ChgTimCoe < 1) ChgTimCoe = 1;

	// ���ӻ������ݴ���
	m_nGnProDat = 0;
	for (ii = 0; ii < m_nGnNum; ii++)
	{
		if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
		{
			//  �����㴰������״̬
			m_pGnDat[ii].stat = MENU_LFO_DEV_STS_OK;
			for (jj = m_LfoPara.time_leng - 1; jj >= 0; jj--)
			{
				if (m_pGnDat[ii].SecSts[jj] != MENU_LFO_DEV_STS_OK)
				{
					m_pGnDat[ii].stat = m_pGnDat[ii].SecSts[jj];
					if (para_DebugInfo == 1)
					{
						sprintf(m_LogString, "FormMonProData()::�����["INT_REC_FMT"]\tͨ���˳�[%d,%d]\t��Ч֡��[%d,%d]\t���������Ч֡[%d,%d]\t����״̬ [%d]", \
							INT_REC_VAL(m_pGnDat[ii].gn_id), m_pGnDat[ii].VecBadData[0].viFilNo, m_pGnDat[ii].VecBadData[0].viColNo, \
							m_pGnDat[ii].VecBadData[1].viFilNo, m_pGnDat[ii].VecBadData[1].viColNo, \
							m_pGnDat[ii].VecBadData[2].viFilNo, m_pGnDat[ii].VecBadData[2].viColNo, m_pGnDat[ii].stat);
						MessShow(m_LogString);
					}
					break;
				}
			}
			if (m_pGnDat[ii].stat != MENU_LFO_DEV_STS_OK)
			{
				m_pGnMon[ii].stat = m_pGnDat[ii].stat;
				m_pGnMon[ii].stat_val = m_pGnDat[ii].stat;
				continue;
			}
		}

		//  �����Լ��  // �Ƕ��ڷ������ִ��� // �澯ʱͻ������д���
// 		m_pGnDat[ii].stat = IsGnDatValid( ii, ChgTimCoe, m_LfoPara.gn_wn_ampl );
// 		if ( m_pGnDat[ii].stat != MENU_LFO_DEV_STS_OK )
// 		{
// 			m_pGnMon[ii].stat     = m_pGnDat[ii].stat;
// 			m_pGnMon[ii].stat_val = m_pGnDat[ii].stat;
// 			continue;
// 		}

		//  �˲� ���� ���
// 		memset((void *)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
//         DP_MidAveFilter( m_pGnDat[ii].pwr, temp_iBuffer, m_nOrigSam, 5 );
// 
// 		DP_CalcDataSeqBaseInfo( temp_iBuffer, m_nOrigSam, dataBaseInfo);
// 		m_pGnDat[ii].ave_val       = dataBaseInfo.ave_val; 
// 		m_pGnDat[ii].max_val.value = dataBaseInfo.max_val; 
// 		m_pGnDat[ii].max_val.time  = (int)(dataBaseInfo.max_val_pnt * m_nOrigDlt) + m_CalcStartTime;
// 		m_pGnDat[ii].min_val.value = dataBaseInfo.min_val; 
// 		m_pGnDat[ii].min_val.time  = (int)(dataBaseInfo.min_val_pnt * m_nOrigDlt) + m_CalcStartTime;
// 		m_pGnDat[ii].max_ampl      = dataBaseInfo.max_ampl;
// 		m_pGnDat[ii].max_ampl_rat  = dataBaseInfo.max_ampl_rat;
// 		// set m_pGnMon
// 		m_pGnMon[ii].aver_val     = m_pGnDat[ii].ave_val; 
// 		m_pGnMon[ii].max_val      = m_pGnDat[ii].max_val.value; 
// 		m_pGnMon[ii].max_val_t    = m_pGnDat[ii].max_val.time;
// 		m_pGnMon[ii].min_val      = m_pGnDat[ii].min_val.value; 
// 		m_pGnMon[ii].min_val_t    = m_pGnDat[ii].min_val.time;
// 		m_pGnMon[ii].max_ampl     = m_pGnDat[ii].max_ampl;
// 		m_pGnMon[ii].max_ampl_rat = m_pGnDat[ii].max_ampl_rat;
		//////////////////////////////////////////////////////////////////////////

// 		m_pGnProDat[m_nGnProDat].is_calc = 1;  // 08-02-02
// 		if( CurveLfoPreEstimate( dataBaseInfo, m_LfoPara.gn_wn_ampl*m_LfoPara.fjud_wnap_per/100, 0, 0 )  <= 0 )
// 		{
// 			m_pGnMon[ii].stat     = MENU_LFO_DEV_STS_SCT;
// 			m_pGnMon[ii].stat_val = MENU_LFO_DEV_STS_SCT;
// 
// 			m_pGnProDat[m_nGnProDat].is_calc = 0;  // 08-02-02
// 			//continue;                            // 08-02-02
// 		}
		// �������
// 		m_pGnProDat[m_nGnProDat].gn_id         = m_pGnDat[ii].gn_id;
//  		m_pGnProDat[m_nGnProDat].ave_val       = m_pGnDat[ii].ave_val; 
//  		m_pGnProDat[m_nGnProDat].max_val.value = m_pGnDat[ii].max_val.value; 
//  		m_pGnProDat[m_nGnProDat].max_val.time  = m_pGnDat[ii].max_val.time;
//  		m_pGnProDat[m_nGnProDat].min_val.value = m_pGnDat[ii].min_val.value; 
//  		m_pGnProDat[m_nGnProDat].min_val.time  = m_pGnDat[ii].min_val.time ;
// 		m_pGnProDat[m_nGnProDat].max_ampl      = m_pGnDat[ii].max_ampl;
// 		m_pGnProDat[m_nGnProDat].max_ampl_rat  = m_pGnDat[ii].max_ampl_rat;
// 		for ( kk=0; kk<m_nSamples; kk++)
// 		{
// 			m_pGnProDat[m_nGnProDat].pwr[kk] = temp_iBuffer[m_nSmpStp*kk];
// 		}
		m_pGnProDat[m_nGnProDat].stat = m_pGnDat[ii].stat;
		m_nGnProDat += 1;
	}
	//////////////////////////////////////////////////////////////////////////

	// ������·���ݴ���
	m_nLnProDat = 0;
	for (ii = 0; ii < m_nLnNum; ii++)
	{
		if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
		{
			//  �����㴰������״̬
			m_pLnDat[ii].stat = MENU_LFO_DEV_STS_OK;
			for (jj = m_LfoPara.time_leng - 1; jj >= 0; jj--)
			{
				if (m_pLnDat[ii].SecSts[jj] != MENU_LFO_DEV_STS_OK)
				{
					m_pLnDat[ii].stat = m_pLnDat[ii].SecSts[jj];
					if (para_DebugInfo == 1)
					{
						sprintf(m_LogString, "FormMonProData()::��·["INT_REC_FMT"]\tͨ���˳�[%d,%d]\t��Ч֡��[%d,%d]\t���������Ч֡[%d,%d]\t����״̬[%d]", \
							INT_REC_VAL(m_pLnDat[ii].ln_id), m_pLnDat[ii].VecBadData[0].viFilNo, m_pLnDat[ii].VecBadData[0].viColNo, \
							m_pLnDat[ii].VecBadData[1].viFilNo, m_pLnDat[ii].VecBadData[1].viColNo, \
							m_pLnDat[ii].VecBadData[2].viFilNo, m_pLnDat[ii].VecBadData[2].viColNo, m_pLnDat[ii].stat);
						MessShow(m_LogString);
					}
					break;
				}
			}
			if (m_pLnDat[ii].stat != MENU_LFO_DEV_STS_OK)
			{
				m_pLnMon[ii].stat = m_pLnDat[ii].stat;
				m_pLnMon[ii].stat_val = m_pLnDat[ii].stat;
				continue;
			}
		}

		//  �����Լ��  // �Ƕ��ڷ������ִ��� // �澯ʱͻ������д���
// 		m_pLnDat[ii].stat = IsLnDatValid( ii, ChgTimCoe, m_LfoPara.ln_wn_ampl );
// 		if ( m_pLnDat[ii].stat != MENU_LFO_DEV_STS_OK )
// 		{
// 			m_pLnMon[ii].stat     = m_pLnDat[ii].stat;
// 			m_pLnMon[ii].stat_val = m_pLnDat[ii].stat;
// 			continue;
// 		}

		//  �˲� ���� ���
// 		memset((void *)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
//         DP_MidAveFilter( m_pLnDat[ii].pwr, temp_iBuffer, m_nOrigSam, 5 );
// 
// 		DP_CalcDataSeqBaseInfo( temp_iBuffer, m_nOrigSam, dataBaseInfo);
// 		m_pLnDat[ii].ave_val       = dataBaseInfo.ave_val; 
// 		m_pLnDat[ii].max_val.value = dataBaseInfo.max_val; 
// 		m_pLnDat[ii].max_val.time  = (int)(dataBaseInfo.max_val_pnt * m_nOrigDlt) + m_CalcStartTime;
// 		m_pLnDat[ii].min_val.value = dataBaseInfo.min_val; 
// 		m_pLnDat[ii].min_val.time  = (int)(dataBaseInfo.min_val_pnt * m_nOrigDlt) + m_CalcStartTime;
// 		m_pLnDat[ii].max_ampl      = dataBaseInfo.max_ampl;
// 		m_pLnDat[ii].max_ampl_rat  = dataBaseInfo.max_ampl_rat;
// 		// set m_pGnMon
// 		m_pLnMon[ii].aver_val      = m_pLnDat[ii].ave_val; 
// 		m_pLnMon[ii].max_val       = m_pLnDat[ii].max_val.value; 
// 		m_pLnMon[ii].max_val_t     = m_pLnDat[ii].max_val.time;
// 		m_pLnMon[ii].min_val       = m_pLnDat[ii].min_val.value; 
// 		m_pLnMon[ii].min_val_t     = m_pLnDat[ii].min_val.time;
// 		m_pLnMon[ii].max_ampl      = m_pLnDat[ii].max_ampl;    // 2008-04-09 by zhw at yn
// 		m_pLnMon[ii].max_ampl_rat  = m_pLnDat[ii].max_ampl_rat;    // 2008-04-09 by zhw at yn

// 		if( CurveLfoPreEstimate( dataBaseInfo, m_LfoPara.ln_wn_ampl*m_LfoPara.fjud_wnap_per/100, 0, 0 )  <= 0 )
// 		{
// 			m_pLnMon[ii].stat     = MENU_LFO_DEV_STS_SCT;
// 			m_pLnMon[ii].stat_val = MENU_LFO_DEV_STS_SCT;
// 
// 			continue;
// 		}
		// �������
// 		m_pLnProDat[m_nLnProDat].acln_id       = m_pLnDat[ii].acln_id;
//  		m_pLnProDat[m_nLnProDat].ave_val       = m_pLnDat[ii].ave_val; 
//  		m_pLnProDat[m_nLnProDat].max_val.value = m_pLnDat[ii].max_val.value; 
//  		m_pLnProDat[m_nLnProDat].max_val.time  = m_pLnDat[ii].max_val.time;
//  		m_pLnProDat[m_nLnProDat].min_val.value = m_pLnDat[ii].min_val.value; 
//  		m_pLnProDat[m_nLnProDat].min_val.time  = m_pLnDat[ii].min_val.time ;
// 		m_pLnProDat[m_nLnProDat].max_ampl      = m_pLnDat[ii].max_ampl;
// 		m_pLnProDat[m_nLnProDat].max_ampl_rat  = m_pLnDat[ii].max_ampl_rat;
// 		for ( kk=0; kk<m_nSamples; kk++)
// 		{
// 			m_pLnProDat[m_nLnProDat].pwr[kk] = temp_iBuffer[m_nSmpStp*kk];
// 		}
		m_pLnProDat[m_nLnProDat].stat = m_pLnDat[ii].stat;

		m_nLnProDat += 1;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���ӱ�ѹ����·���ݴ���
	m_nTrProDat = 0;
	for (ii = 0; ii < m_nTrNum; ii++)
	{
		if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
		{
			//  �����㴰������״̬
			m_pTrDat[ii].stat = MENU_LFO_DEV_STS_OK;
			for (jj = m_LfoPara.time_leng - 1; jj >= 0; jj--)
			{
				if (m_pTrDat[ii].SecSts[jj] != MENU_LFO_DEV_STS_OK)
				{
					m_pTrDat[ii].stat = m_pTrDat[ii].SecSts[jj];
					if (para_DebugInfo == 1)
					{
						sprintf(m_LogString, "FormMonProData()::��ѹ��["INT_REC_FMT"]\tͨ���˳�[%d,%d]\t��Ч֡��[%d,%d]\t���������Ч֡[%d,%d]\t����״̬[%d]", \
							INT_REC_VAL(m_pTrDat[ii].tr_id), m_pTrDat[ii].VecBadData[0].viFilNo, m_pTrDat[ii].VecBadData[0].viColNo, \
							m_pTrDat[ii].VecBadData[1].viFilNo, m_pTrDat[ii].VecBadData[1].viColNo, \
							m_pTrDat[ii].VecBadData[2].viFilNo, m_pTrDat[ii].VecBadData[2].viColNo, m_pTrDat[ii].stat);
						MessShow(m_LogString);
					}
					break;
				}
			}
			if (m_pTrDat[ii].stat != MENU_LFO_DEV_STS_OK)
			{
				m_pTrMon[ii].stat = m_pTrDat[ii].stat;
				m_pTrMon[ii].stat_val = m_pTrDat[ii].stat;
				continue;
			}
		}

		//  �����Լ��
//         m_pTrDat[ii].stat = IsTrDatValid( ii, ChgTimCoe, m_LfoPara.tr_wn_ampl );
//         if ( m_pTrDat[ii].stat != MENU_LFO_DEV_STS_OK )
//         {
//             m_pTrMon[ii].stat     = m_pTrDat[ii].stat;
//             m_pTrMon[ii].stat_val = m_pTrDat[ii].stat;
//             continue;
//         }

		//  �˲� ���� ���
//         memset((void *)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
//         DP_MidAveFilter( m_pTrDat[ii].pwr, temp_iBuffer, m_nOrigSam, 5 );

//         DP_CalcDataSeqBaseInfo( temp_iBuffer, m_nOrigSam, dataBaseInfo);
//         m_pTrDat[ii].ave_val       = dataBaseInfo.ave_val; 
//         m_pTrDat[ii].max_val.value = dataBaseInfo.max_val; 
//         m_pTrDat[ii].max_val.time  = (int)(dataBaseInfo.max_val_pnt * m_nOrigDlt) + m_CalcStartTime;
//         m_pTrDat[ii].min_val.value = dataBaseInfo.min_val; 
//         m_pTrDat[ii].min_val.time  = (int)(dataBaseInfo.min_val_pnt * m_nOrigDlt) + m_CalcStartTime;
//         m_pTrDat[ii].max_ampl      = dataBaseInfo.max_ampl;
//         m_pTrDat[ii].max_ampl_rat  = dataBaseInfo.max_ampl_rat;
//         // set m_pGnMon
//         m_pTrMon[ii].aver_val      = m_pTrDat[ii].ave_val; 
//         m_pTrMon[ii].max_val       = m_pTrDat[ii].max_val.value; 
//         m_pTrMon[ii].max_val_t     = m_pTrDat[ii].max_val.time;
//         m_pTrMon[ii].min_val       = m_pTrDat[ii].min_val.value; 
//         m_pTrMon[ii].min_val_t     = m_pTrDat[ii].min_val.time;
//         m_pTrMon[ii].max_ampl      = m_pTrDat[ii].max_ampl;    // 2008-04-09 by zhw at yn
//         m_pTrMon[ii].max_ampl_rat  = m_pTrDat[ii].max_ampl_rat;    // 2008-04-09 by zhw at yn

//         if( CurveLfoPreEstimate( dataBaseInfo, m_LfoPara.tr_wn_ampl*m_LfoPara.fjud_wnap_per/100, 0, 0 )  <= 0 )
//         {
//             m_pTrMon[ii].stat     = MENU_LFO_DEV_STS_SCT;
//             m_pTrMon[ii].stat_val = MENU_LFO_DEV_STS_SCT;
//             
//             continue;
//         }
		// �������
//         m_pTrProDat[m_nTrProDat].tr_id       = m_pTrDat[ii].tr_id;
//         m_pTrProDat[m_nTrProDat].ave_val       = m_pTrDat[ii].ave_val; 
//         m_pTrProDat[m_nTrProDat].max_val.value = m_pTrDat[ii].max_val.value; 
//         m_pTrProDat[m_nTrProDat].max_val.time  = m_pTrDat[ii].max_val.time;
//         m_pTrProDat[m_nTrProDat].min_val.value = m_pTrDat[ii].min_val.value; 
//         m_pTrProDat[m_nTrProDat].min_val.time  = m_pTrDat[ii].min_val.time ;
//         m_pTrProDat[m_nTrProDat].max_ampl      = m_pTrDat[ii].max_ampl;
//         m_pTrProDat[m_nTrProDat].max_ampl_rat  = m_pTrDat[ii].max_ampl_rat;
//         for ( kk=0; kk<m_nSamples; kk++)
//         {
//             m_pTrProDat[m_nTrProDat].pwr[kk] = temp_iBuffer[m_nSmpStp*kk];
//         }
		m_pTrProDat[m_nTrProDat].stat = m_pTrDat[ii].stat;

		m_nTrProDat += 1;
	}

	// 2009-07-08
//     char fileName[MAXBUF];
// #ifdef _WINDOWS32
//     sprintf( fileName, "%s\\res\\%s.txt", m_RunInfDir,"AverValue" );
// #else
//     sprintf( fileName, "%s/res/%s.txt", m_RunInfDir,"AverValue" );
// #endif
//     
//     FILE*   fileptr = NULL;
//     if((fileptr=fopen(fileName,"w+"))==NULL)
//     {
//         sprintf( m_LogString," WriteModeShapeToFile() Cannot open the file %s", fileName);
//         MessShow(m_LogString);
//         return 0;
//     }
// 
//     char strTime1[30];
//     char strTime2[30];
//     char temp_message[1024];
//     float tmpf;
//     sprintf( strTime1, "%s", PT_GetTimeStringBySeconds(m_CalcStartTime));
//     sprintf( strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime)   );
//     fprintf(fileptr,"\n%s\n","----------------------------------------------------------------------------------------------");
//     sprintf( temp_message, " %s �� %s ��Ҫģʽ��Ϣ", strTime1, strTime2 );
//     fprintf(fileptr,"\n%s\n", temp_message);
//     if( m_nGnProDat >0 )
//     {
//         fprintf(fileptr,"%s\n", "�������");
//         fprintf(fileptr,"%s\n", "           �����ID    ���ֵ    ��Сֵ  ���ƽ��   �����Сƽ��");
//         for( ii=0; ii<m_nGnProDat; ii++)
//         {
//             tmpf = (m_pGnProDat[ii].max_val.value + m_pGnProDat[ii].min_val.value)/2.0f;
//             sprintf( temp_message,"                "INT_REC_FMT"  %6.3f  %6.3f  %6.3f  %6.3f",
//                 m_pGnProDat[ii].gn_id,
//                 m_pGnProDat[ii].max_val.value,
//                 m_pGnProDat[ii].min_val.value,
//                 m_pGnProDat[ii].ave_val,
//                 tmpf
//             );
//             fprintf(fileptr,"%s\n", temp_message);
//         }
//     }
//     if( m_nLnProDat >0 )
//     {
//         fprintf(fileptr,"%s\n", "������·��");
//         fprintf(fileptr,"%s\n", "           ������·ID    ���ֵ    ��Сֵ   ���ƽ��   �����Сƽ��");
//         for( ii=0; ii<m_nLnProDat; ii++)
//         {
//             tmpf = (m_pLnProDat[ii].max_val.value + m_pLnProDat[ii].min_val.value)/2.0f;
//             sprintf( temp_message,"                "INT_REC_FMT"  %6.3f  %6.3f  %6.3f  %6.3f",
//                 m_pLnProDat[ii].acln_id,
//                 m_pLnProDat[ii].max_val.value,
//                 m_pLnProDat[ii].min_val.value,
//                 m_pLnProDat[ii].ave_val,
//                 tmpf
//                 );
//             fprintf(fileptr,"%s\n", temp_message);
//         }
//     }
//     if( m_nTrProDat >0 )
//     {
//         fprintf(fileptr,"%s\n", "��ѹ����");
//         fprintf(fileptr,"%s\n", "           ��ѹ��ID    ���ֵ    ��Сֵ   ���ƽ��   �����Сƽ��");
//         for( ii=0; ii<m_nTrProDat; ii++)
//         {
//             tmpf = (m_pTrProDat[ii].max_val.value + m_pTrProDat[ii].min_val.value)/2.0f;
//             sprintf( temp_message,"                "INT_REC_FMT"  %6.3f  %6.3f  %6.3f  %6.3f",
//                 m_pTrProDat[ii].tr_id,
//                 m_pTrProDat[ii].max_val.value,
//                 m_pTrProDat[ii].min_val.value,
//                 m_pTrProDat[ii].ave_val,
//                 tmpf
//                 );
//             fprintf(fileptr,"%s\n", temp_message);
//         }
//     }
//     fclose(fileptr);

	sprintf(m_LogString, "��������Ԥ�������! m_nGnProDat=%d m_nLnProDat=%d m_nTrProDat=%d", m_nGnProDat, m_nLnProDat, m_nTrProDat);
	MessShow(m_LogString);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��վPmudata��Ϣ����ȽϺ��� ampl�ɴ�С
////////////////////////////////////////////////////////////////////////////////
extern "C" int FcPmuDatCompareByAmpl(const void* p1, const void* p2)
{
	if (((LFO_FC_DAT*)p1)->max_ampl != ((LFO_FC_DAT*)p2)->max_ampl)
	{
		return ((LFO_FC_DAT*)p1)->max_ampl < ((LFO_FC_DAT*)p2)->max_ampl ? 1 : -1;
	}

	return ((LFO_FC_DAT*)p1)->ave_val > ((LFO_FC_DAT*)p2)->ave_val ? 1 : -1;
}

// 	int                  m_nFcProDat;                       //  �账��ĳ�վ
// 	LFO_FC_DAT *         m_pFcProDat;                       //  �账��ĳ�վ����((Ԥ�����)
// ��վ���ݴ���(�����Լ�顢�������˲������ProData)
int CLfoAnaObj::FormFcProData()
{
	if (m_nFcNum <= 0 || !m_pFcDat || !m_pFcProDat) return 0;

	int             ii, jj, kk;
	//float           ampl_max     = 0;
	//float           ampl_max_rat = 0;
	//int             findFlag     = 0 ;
	float           temp_iBuffer[DATA_NUM_MAX];
	SEQ_BASE_INFO   dataBaseInfo;

	//
	m_nFcProDat = 0;
	for (ii = 0; ii < m_nFcNum; ii++)
	{
		if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
		{
			//  �����㴰������״̬
			m_pFcDat[ii].stat = MENU_LFO_DEV_STS_OK;
			for (jj = m_LfoPara.time_leng - 1; jj >= 0; jj--)
			{
				if (m_pFcDat[ii].SecSts[jj] != MENU_LFO_DEV_STS_OK)
				{
					m_pFcDat[ii].stat = m_pFcDat[ii].SecSts[jj];
					break;
				}
			}
			if (m_pFcDat[ii].stat != MENU_LFO_DEV_STS_OK)
			{
				m_pFcMon[ii].stat = m_pFcDat[ii].stat;
				continue;
			}
		}

		//  �����Լ��  // �Ƕ����ڴ������
		m_pFcDat[ii].stat = IsFcDatValid(ii);
		if (m_pFcDat[ii].stat != MENU_LFO_DEV_STS_OK)
		{
			m_pFcMon[ii].stat = m_pFcDat[ii].stat;
			continue;
		}

		//  �˲� ���� ���
		memset((void*)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
		DP_MidAveFilter(m_pFcDat[ii].u1v, temp_iBuffer, m_nOrigSam, 5);

		DP_CalcDataSeqBaseInfo(temp_iBuffer, m_nOrigSam, dataBaseInfo);
		m_pFcDat[ii].ave_val = dataBaseInfo.ave_val;
		m_pFcDat[ii].max_val.value = dataBaseInfo.max_val;
		m_pFcDat[ii].max_val.time = (int)(dataBaseInfo.max_val_pnt * m_nOrigDlt) + m_CalcStartTime;
		m_pFcDat[ii].min_val.value = dataBaseInfo.min_val;
		m_pFcDat[ii].min_val.time = (int)(dataBaseInfo.min_val_pnt * m_nOrigDlt) + m_CalcStartTime;
		m_pFcDat[ii].max_ampl = dataBaseInfo.max_ampl;
		m_pFcDat[ii].max_ampl_rat = dataBaseInfo.max_ampl_rat;
		// set m_pGnMon
// 		m_pFcMon[ii].v_aver      = m_pFcDat[ii].ave_val; 
// 		m_pFcMon[ii].v_max       = m_pFcDat[ii].max_val.value; 
// 		m_pFcMon[ii].v_min       = m_pFcDat[ii].min_val.value; 
// 		m_pFcMon[ii].v_ampl      = m_pFcDat[ii].max_ampl;

		// 2008-08-12 by zhw 0.01->0.001
// 		if( CurveLfoPreEstimate( dataBaseInfo, (float)(para_FcVoltPer*m_pFcMon[ii].v_aver), 1, (float)0.01 )  <= 0 )
// 		{
// 			m_pFcMon[ii].stat     = MENU_LFO_DEV_STS_SCT;
// 			//m_pFcMon[ii].stat_val = m_pFcDat[ii].stat;
// 
// 			continue;
// 		}

		// �������
		m_pFcProDat[m_nFcProDat].fac_id = m_pFcDat[ii].fac_id;
		m_pFcProDat[m_nFcProDat].ave_val = m_pFcDat[ii].ave_val;
		m_pFcProDat[m_nFcProDat].max_val.value = m_pFcDat[ii].max_val.value;
		m_pFcProDat[m_nFcProDat].max_val.time = m_pFcDat[ii].max_val.time;
		m_pFcProDat[m_nFcProDat].min_val.value = m_pFcDat[ii].min_val.value;
		m_pFcProDat[m_nFcProDat].min_val.time = m_pFcDat[ii].min_val.time;
		m_pFcProDat[m_nFcProDat].max_ampl = m_pFcDat[ii].max_ampl;
		m_pFcProDat[m_nFcProDat].max_ampl_rat = m_pFcDat[ii].max_ampl_rat;
		for (kk = 0; kk < m_nSamples; kk++)
		{
			m_pFcProDat[m_nFcProDat].u1v[kk] = temp_iBuffer[m_nSmpStp * kk];
		}
		m_pFcProDat[m_nFcProDat].stat = m_pFcDat[ii].stat;

		m_nFcProDat += 1;
	}

	//  ɸѡ��վ(ʹ�ñ�������)
	// sort by m_pFcPmuDat[ii].max_ampl
	qsort((void*)m_pFcProDat, m_nFcProDat, sizeof(LFO_FC_DAT), FcPmuDatCompareByAmpl);

	int  m_nFcProDat_old = m_nFcProDat;
	if (m_nFcProDat > para_nAnaMaxFcNum)  m_nFcProDat = para_nAnaMaxFcNum;

	sprintf(m_LogString, "��վ����Ԥ�������! m_nFcProDat_old=%d m_nFcProDat=%d", m_nFcProDat_old, m_nFcProDat);
	MessShow(m_LogString);

	return 0;
}

// 	// ��ǲο���ѡȡ ��Ԥ��������·�ͻ�����ѡȡ�ο���
//  int                  m_AngRefID;                        //  �ο����id
// 	float                m_AngRefDat[DATA_NUM_MAX];         //  �ο��������
// m_nGnProDat
// m_pGnProDat
// m_nLnProDat
// m_pLnProDat
int CLfoAnaObj::SelectRefAngle()
{
	if (m_nGnProDat < 1 && m_nLnProDat < 1)  return -1;

	int ii, jj;
	memset((void*)m_AngRefDat, 0, DATA_NUM_MAX * sizeof(float));

	if (m_nGnProDat > 0)
	{
		m_AngRefID = m_pGnProDat[0].gn_id;
		// 2008-07-16 by zhw
		for (jj = 0; jj < m_nGnNum; jj++)
		{
			if (m_AngRefID == m_pGnDat[jj].gn_id)
			{
				for (ii = 0; ii < m_nOrigSam; ii++)
				{
					//                     m_AngRefDat[ii] = m_pGnDat[jj].u1a[ii];
				}
				break;
			}
		}
		return 1;
	}

	if (m_nLnProDat > 0)
	{
		m_AngRefID = m_pLnProDat[0].ln_id;
		for (jj = 0; jj < m_nLnNum; jj++)
		{
			if (m_AngRefID == m_pLnDat[jj].ln_id)
			{
				for (ii = 0; ii < m_nOrigSam; ii++)
				{
					// m_AngRefDat[ii] = m_pLnProDat[0].u1a[ii]; 2008-07-16 lsf
//                     m_AngRefDat[ii] = m_pLnDat[jj].u1a[ii];
				}
				break;
			}
		}
		return 1;
	}
	return -1;
}

// 	��ǲο���ѡȡ ������ѡȡ�ο���
// 	float                m_AngRefDat2[DATA_NUM_MAX];         //  �ο��������
// m_nGnNum
// m_pGnDat
int CLfoAnaObj::SelectRefAngle2()
{
	if (m_nGnNum < 1 && !m_pGnDat)  return -1;

	int nn, ii;
	int     temp_nGnData = 0;
	float   temp_iBuffer[DATA_NUM_MAX];
	float   temp_oBuffer[DATA_NUM_MAX];
	memset((void*)m_AngRefDat2, 0, DATA_NUM_MAX * sizeof(float));

	if (m_nGnNum > 0)
	{
		for (nn = 0; nn < m_nGnNum; nn++)
		{
			if (m_pGnDat[nn].stat == MENU_LFO_DEV_STS_OK)
			{
				memset((void*)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
				memset((void*)temp_oBuffer, 0, DATA_NUM_MAX * sizeof(float));

				if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
				{
					// 					DP_CalcAngleDiff( m_nOrigSam, m_pGnDat[nn].ang, m_AngRefDat, temp_iBuffer );    
					DP_MidAveFilter(temp_iBuffer, temp_oBuffer, m_nOrigSam, 5);
				}
				else
				{
					// 					DP_MidAveFilter( m_pGnDat[nn].ang, temp_oBuffer, m_nOrigSam, 5 );
				}
				for (ii = 0; ii < m_nOrigSam; ii++)
				{
					m_AngRefDat2[ii] += temp_oBuffer[ii];
				}
				temp_nGnData += 1;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		memset((void*)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
		for (ii = 0; ii < m_nOrigSam; ii++)
		{
			temp_iBuffer[ii] = m_AngRefDat2[ii];
		}

		memset((void*)m_AngRefDat2, 0, DATA_NUM_MAX * sizeof(float));
		if (temp_nGnData > 0)
		{
			for (ii = 0; ii < m_nSamples; ii++)
			{
				m_AngRefDat2[ii] = temp_iBuffer[m_nSmpStp * ii] / temp_nGnData;
			}
		}
	}

	return 1;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ��վFcProdata��Ϣ����ȽϺ��� stat��С����
////////////////////////////////////////////////////////////////////////////////
extern "C" int FcPmuDatCompareByStat(const void* p1, const void* p2)
{
	if (((LFO_FC_DAT*)p1)->stat != ((LFO_FC_DAT*)p2)->stat)
	{
		return ((LFO_FC_DAT*)p1)->stat > ((LFO_FC_DAT*)p2)->stat ? 1 : -1;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ����GnProdata��Ϣ����ȽϺ��� stat��С����
////////////////////////////////////////////////////////////////////////////////
extern "C" int GnPmuDatCompareByStat(const void* p1, const void* p2)
{
	if (((SSO_GN_DAT*)p1)->stat != ((SSO_GN_DAT*)p2)->stat)
	{
		return ((SSO_GN_DAT*)p1)->stat > ((SSO_GN_DAT*)p2)->stat ? 1 : -1;
	}
	return 0;
}


//  ����������Ԥ����(gn, ln)
//  ����Ԥ����(�����Լ�顢�˲���������Ԥ�С����)
//     int                  m_nSmpFre;                         //  ����Ƶ��
//     int                  m_nSmpStp;                         //  �������
//  int                  m_AngRefID;                        //  �ο����id
// 	float                m_AngRefDat[DATA_NUM_MAX];         //  �ο��������
int CLfoAnaObj::FormAnaProData()
{
	FormFcProData();
	if (m_nGnProDat < 1 && m_nFcProDat < 1)  return -1;      // �޷�������

	int     ii, jj, kk;
	int     temp_ProGnData = 0;
	int     temp_ProFcData = 0;
	float   temp_iBuffer[DATA_NUM_MAX];
	float   temp_oBuffer[DATA_NUM_MAX];

	int     ChgTimCoe = (int)(m_nOrigDlt * BAS_FRAME_PERSEC + 0.45);
	if (ChgTimCoe < 1) ChgTimCoe = 1;
	float   AglMaxChg = G_ANG_MAX_CHG * ChgTimCoe;

	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	{
		/*SelectRefAngle( );
		sprintf(m_LogString,"FormAnaProData(): �ο��ǣ�id=[%d] ! ", m_AngRefID);
		WriteLogFile(m_LogString);*/  // 2008-08-15

		//  ��ȡ��Խ�
		for (jj = 0; jj < m_nGnProDat; jj++)
		{
			// 			m_pGnProDat[jj].is_calc = 0; // 08-02-02 ���÷���״̬
			for (ii = 0; ii < m_nGnNum; ii++)
			{
				if (m_pGnProDat[jj].gn_id == m_pGnDat[ii].gn_id)
				{
					//  ��Խ� �˲� ���� ���
					memset((void*)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
					memset((void*)temp_oBuffer, 0, DATA_NUM_MAX * sizeof(float));
					//DP_CalcAngleDiff( m_nOrigSam, m_pGnDat[ii].ang, m_AngRefDat, temp_iBuffer );    // 2008-08-15
//                     DP_JumpPointHdl(m_nOrigSam,temp_iBuffer,m_pGnDat[ii].ang,para_MaxAnglDiff); 

					DP_MidAveFilter(temp_iBuffer, temp_oBuffer, m_nOrigSam, 5);
					m_pGnProDat[jj].stat = IsAglDatValid(m_nOrigSam, temp_oBuffer, AglMaxChg);

					if (m_pGnProDat[jj].stat == MENU_LFO_DEV_STS_OK)
					{
						for (kk = 0; kk < m_nSamples; kk++)
						{
							// 							m_pGnProDat[jj].ang[kk] = temp_oBuffer[m_nSmpStp*kk];
						}
						//temp_ProGnData += 1; // 08-02-02
					}

					// 08-02-02
// 					if ( m_pGnProDat[jj].max_ampl > m_LfoPara.gn_wn_ampl*para_AnaWnapPer/100)
// 					{
// 						m_pGnProDat[jj].is_calc = 1;
// 					}

					break;
				}
			}
		}
		// sort by m_pGnProDat[jj].stat
		//qsort((void *)m_pGnProDat, m_nGnProDat, sizeof(LFO_GN_DAT), GnPmuDatCompareByStat);// 08-02-02
		//m_nGnProDat = temp_ProGnData;// 08-02-02

		for (jj = 0; jj < m_nFcProDat; jj++)
		{
			for (ii = 0; ii < m_nFcNum; ii++)
			{
				if (m_pFcProDat[jj].fac_id == m_pFcDat[ii].fac_id)
				{
					//  ��Խ� �˲� ���� ���
					memset((void*)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
					memset((void*)temp_oBuffer, 0, DATA_NUM_MAX * sizeof(float));
					//DP_CalcAngleDiff( m_nOrigSam, m_pFcDat[ii].u1a, m_AngRefDat, temp_iBuffer );    
//                     DP_JumpPointHdl(m_nOrigSam,temp_iBuffer,m_pFcDat[ii].u1a,para_MaxAnglDiff);  // 2008-08-15

					DP_MidAveFilter(temp_iBuffer, temp_oBuffer, m_nOrigSam, 5);
					m_pFcProDat[jj].stat = IsAglDatValid(m_nOrigSam, temp_oBuffer, AglMaxChg);

					if (m_pFcProDat[jj].stat == MENU_LFO_DEV_STS_OK)
					{
						for (kk = 0; kk < m_nSamples; kk++)
						{
							// 							m_pFcProDat[jj].u1a[kk] = temp_oBuffer[m_nSmpStp*kk];
						}
						temp_ProFcData += 1;
					}

					break;
				}
			}
		}
		// sort by m_pFcProDat[jj].stat
		qsort((void*)m_pFcProDat, m_nFcProDat, sizeof(LFO_FC_DAT), FcPmuDatCompareByStat);
		m_nFcProDat = temp_ProFcData;
		//////////////////////////////////////////////////////////////////////////
		// 2008-04-05 by zhw at yn
		/*SelectRefAngle2( ); // cankaojiao 08-02-01
		for ( jj=0; jj<m_nGnProDat; jj++ )
		{
			if ( m_pGnProDat[jj].stat != MENU_LFO_DEV_STS_OK ) continue; //��08-02-02

			for ( kk=0; kk<m_nSamples; kk++)
			{
				m_pGnProDat[jj].ang[kk] = m_pGnProDat[jj].ang[kk] - m_AngRefDat2[kk];
			}
		}

		for ( jj=0; jj<m_nFcProDat; jj++ )
		{
			for ( kk=0; kk<m_nSamples; kk++)
			{
				m_pFcProDat[jj].u1a[kk] = m_pFcProDat[jj].u1a[kk] - m_AngRefDat2[kk];
			}
		}*/

		//////////////////////////////////////////////////////////////////////////

	}
	else
	{
		for (jj = 0; jj < m_nGnProDat; jj++)
		{
			// 			m_pGnProDat[jj].is_calc = 0; // 08-02-02 ���÷���״̬
			for (ii = 0; ii < m_nGnNum; ii++)
			{
				if (m_pGnProDat[jj].gn_id == m_pGnDat[ii].gn_id)
				{
					//  �˲� ���� ���
					memset((void*)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
					memset((void*)temp_oBuffer, 0, DATA_NUM_MAX * sizeof(float));
					//DP_CalcAngleDiff( m_nOrigSam, m_pGnDat[ii].ang, m_AngRefDat, temp_iBuffer );
//                     DP_JumpPointHdl(m_nOrigSam,temp_iBuffer,m_pGnDat[ii].ang,para_MaxAnglDiff);

					DP_MidAveFilter(temp_iBuffer, temp_oBuffer, m_nOrigSam, 5);
					m_pGnProDat[jj].stat = IsAglDatValid(m_nOrigSam, temp_oBuffer, AglMaxChg);

					if (m_pGnProDat[jj].stat == MENU_LFO_DEV_STS_OK)
					{
						for (kk = 0; kk < m_nSamples; kk++)
						{
							// 							m_pGnProDat[jj].ang[kk] = temp_oBuffer[m_nSmpStp*kk];
						}
						temp_ProGnData += 1;// 08-02-02
					}

					// 08-02-02
// 					if ( m_pGnProDat[jj].max_ampl > m_LfoPara.gn_wn_ampl*para_AnaWnapPer/100)
// 					{
// 						m_pGnProDat[jj].is_calc = 1;
// 					}
					break;
				}

			}
		}
		// sort by m_pGnProDat[jj].stat
		qsort((void*)m_pGnProDat, m_nGnProDat, sizeof(SSO_GN_DAT), GnPmuDatCompareByStat);// 08-02-02
		m_nGnProDat = temp_ProGnData;// 08-02-02

		for (jj = 0; jj < m_nFcProDat; jj++)
		{
			for (ii = 0; ii < m_nFcNum; ii++)
			{
				if (m_pFcProDat[jj].fac_id == m_pFcDat[ii].fac_id)
				{
					//  ��Խ� �˲� ���� ���
					memset((void*)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
					memset((void*)temp_oBuffer, 0, DATA_NUM_MAX * sizeof(float));
					//DP_CalcAngleDiff( m_nOrigSam, m_pFcDat[ii].u1a, m_AngRefDat, temp_iBuffer );    
//                     DP_JumpPointHdl(m_nOrigSam,temp_iBuffer,m_pFcDat[ii].u1a,para_MaxAnglDiff);

					DP_MidAveFilter(temp_iBuffer, temp_oBuffer, m_nOrigSam, 5);
					m_pFcProDat[jj].stat = IsAglDatValid(m_nOrigSam, temp_oBuffer, AglMaxChg);

					if (m_pFcProDat[jj].stat == MENU_LFO_DEV_STS_OK)
					{
						for (kk = 0; kk < m_nSamples; kk++)
						{
							// 							m_pFcProDat[jj].u1a[kk] = temp_oBuffer[m_nSmpStp*kk];
						}
						temp_ProFcData += 1;
					}

					break;
				}
			}
		}
		// sort by m_pFcProDat[jj].stat
		qsort((void*)m_pFcProDat, m_nFcProDat, sizeof(LFO_FC_DAT), FcPmuDatCompareByStat);
		m_nFcProDat = temp_ProFcData;
	}
	// ���ĽǴ���
	//////////////////////////////////////////////////////////////////////////
	// 2008-04-05 by zhw at yn
	if (para_FileType == FILE_TYPE_DYN)
	{
		SelectRefAngle2(); // cankaojiao 08-02-01
		//////////////////////////////////////////////////////////////////////////
		for (jj = 0; jj < m_nGnProDat; jj++)
		{
			if (m_pGnProDat[jj].stat != MENU_LFO_DEV_STS_OK) continue; //��08-02-02

			for (kk = 0; kk < m_nSamples; kk++)
			{
				//                 m_pGnProDat[jj].ang[kk] = m_pGnProDat[jj].ang[kk] - m_AngRefDat2[kk];
			}
		}

		for (jj = 0; jj < m_nFcProDat; jj++)
		{
			for (kk = 0; kk < m_nSamples; kk++)
			{
				//                 m_pFcProDat[jj].u1a[kk] = m_pFcProDat[jj].u1a[kk] - m_AngRefDat2[kk];
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	sprintf(m_LogString, "��������Ԥ�������! m_nGnProDat=%d m_nFcProDat=%d", m_nGnProDat, m_nFcProDat);
	MessShow(m_LogString);

	return 0;
}

// ��վ������Ч���ж�
// ��վ������Ч���ж�
int CLfoAnaObj::IsFcDatValid(int no)
{
	// 	if (para_IsDataValid == 0)
	// 	{
	// 		return MENU_LFO_DEV_STS_OK;
	// 	}
	// 
	// 	int    ii;
	// 	float  v_max, v_min;
	// 	char   strTime1[28], strTime2[28];
	// 
	// 	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	// 	{
	// 		if (m_pFcPmu[no].is_m_bs == MENU_LFO_MENU_YES )
	// 		{
	// 			v_max = m_pFcInf[no].m_bs_vmax;
	// 			v_min = m_pFcInf[no].m_bs_vmin;	
	// 		}
	// 		else
	// 		{
	// 			v_max = m_pFcInf[no].v_bs_vmax;
	// 			v_min = m_pFcInf[no].v_bs_vmin;	
	// 		}
	// 
	// 		for ( ii=0; ii<m_nOrigSam; ii++ )
	// 		{
	// 			if ( m_pFcDat[no].u1v[ii] < v_min || m_pFcDat[no].u1v[ii] > v_max )
	// 			{
	// 				sprintf( strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
	// 				sprintf( strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));
	// 				
	// 				sprintf(m_LogString,"IsFcDatValid(): Fc[%s] [%s-%s] ��ѹ�쳣��%5.2f [%5.2f , %5.2f]", m_pFcInf[no].fac_name, 
	// 					strTime1, strTime2, m_pFcDat[no].u1v[ii], v_min, v_max );
	// 				MessShow(m_LogString);
	// 				
	// 				return MENU_LFO_DEV_STS_VAL;
	// 			}
	// 		}
	// 	}

	return MENU_LFO_DEV_STS_OK;
}

// ��·������Ч���ж�
int CLfoAnaObj::IsLnDatValid(int no, int ChgTimCoe, float ChgMinVal)
{
	// 	if (para_IsDataValid == 0)
	// 	{
	// 		return MENU_LFO_DEV_STS_OK;
	// 	}
	// 
	// 	int    ii;
	// 	float  ChangeVal = -1.0f, pwr = 0.0f, max_pwr = 5000.0, min_pwr = -5000.0;
	// 	float  MaxChangeLimVal = L_PWR_MAX_CHG * ChgTimCoe;
	// 	char   strTime1[28], strTime2[28];
	// 
	// 	for ( ii=0; ii<m_nOrigSam; ii++ )
	// 	{
	// //  		sprintf(m_LogString,"�����߶�[%s]�й�[%f]��������[%f]��ѹ��ֵ[%f]", m_pLnInf[no].acln_name, m_pLnDat[no].pwr[ii],
	// //  				m_pLnInf[no].imax1, m_pLnInf[no].vbase );
	// //  		MessShow(m_LogString);	//20141008	
	// 		pwr = (m_pLnInf[no].imax1 * m_pLnInf[no].vbase)/1000.0;
	// 		if (pwr > 1.0)
	// 		{
	// 			max_pwr = 2.0 * pwr;
	// 			min_pwr = -2.0 * pwr;
	// 		}
	// //  		sprintf(m_LogString,"�����߶�Pmax=[%f],Pmin=[%f]", m_pLnInf[no].acln_name,max_pwr,min_pwr);
	// //  		MessShow(m_LogString);	
	// 
	// 		if ( m_pLnDat[no].pwr[ii] < min_pwr || m_pLnDat[no].pwr[ii] > max_pwr )
	// 		{
	// 			sprintf( strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
	// 			sprintf( strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));
	// 			
	// 			sprintf(m_LogString,"IsLnDatValid(): Ln[%s] [%s-%s] �й������쳣��%5.2f [%5.2f , %5.2f]", m_pLnInf[no].acln_name, 
	// 				strTime1, strTime2, m_pLnDat[no].pwr[ii], min_pwr, max_pwr );
	// 			MessShow(m_LogString);
	// 			return MENU_LFO_DEV_STS_VAL;
	// 		}
	// 	}
	// 	
	// 	ChangeVal = JudgeTwoValDelta( &m_pLnDat[no].pwr[0], m_nOrigSam, MaxChangeLimVal, ChgMinVal, 1 ) ;
	// 	if( ChangeVal > ZEROVALUE )
	// 	{
	// 		sprintf( strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
	// 		sprintf( strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));
	// 		
	// 		sprintf(m_LogString,"IsLnDatValid(): Ln[%s] [%s-%s] ����ͻ�䣬ͻ������%5.2f", m_pLnInf[no].acln_name, 
	// 			strTime1, strTime2, ChangeVal);
	// 		MessShow(m_LogString);
	// 		
	// 		return MENU_LFO_DEV_STS_BRK;
	// 	}

	return MENU_LFO_DEV_STS_OK;
}

// ����������Ч���жϣ�p Խ�� ͻ�䣩
int CLfoAnaObj::IsGnDatValid(int no, int ChgTimCoe, float ChgMinVal)
{
	// 	if (para_IsDataValid == 0)
	// 	{
	// 		return MENU_LFO_DEV_STS_OK;
	// 	}
	// 
	// 	int    ii;
	// 	float  ChangeVal = -1.0f;
	// 	float  MaxChangeLimVal = L_PWR_MAX_CHG * ChgTimCoe;
	// 	char   strTime1[28], strTime2[28];
	// 
	// 	if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
	// 	{
	// 		if (ChgTimCoe < 1)  ChgTimCoe = 1; 
	// 		float  MaxChangeLimVal = G_PWR_MAX_CHG * m_pGnInf[no].pnom * ChgTimCoe;
	// 
	// 		for ( ii=0; ii<m_nOrigSam; ii++ )
	// 		{
	// 			//if ( m_pGnDat[no].pwr[ii] < m_pGnInf[no].pmin || m_pGnDat[no].pwr[ii] > m_pGnInf[no].pmax )
	// 			if ( m_pGnDat[no].pwr[ii] < -m_pGnInf[no].pmax || m_pGnDat[no].pwr[ii] > PWR_UP_LMT_COE*m_pGnInf[no].pmax )// ���ǳ���
	// 			{
	// 				sprintf( strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
	// 				sprintf( strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));
	// 				
	// 				sprintf(m_LogString,"IsGnDatValid(): Gn[%s%s] [%s-%s] �й������쳣��%5.2f [%5.2f , %5.2f]", m_pGnInf[no].fac_name, m_pGnInf[no].gn_name, 
	// 					strTime1, strTime2, m_pGnDat[no].pwr[ii], -m_pGnInf[no].pmax, PWR_UP_LMT_COE*m_pGnInf[no].pmax );
	// 				MessShow(m_LogString);
	// 				return MENU_LFO_DEV_STS_VAL;
	// 			}
	// 		}
	// 	}
	// 
	// 	ChangeVal = JudgeTwoValDelta( &m_pGnDat[no].pwr[0], m_nOrigSam, MaxChangeLimVal, ChgMinVal, 1 ) ;
	// 	if( ChangeVal > ZEROVALUE )
	// 	{
	// 		sprintf( strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
	// 		sprintf( strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));
	// 		
	// 		sprintf(m_LogString,"IsGnDatValid(): Gn[%s%s] [%s-%s] ����ͻ�䣬ͻ������%5.2f", m_pGnInf[no].fac_name, m_pGnInf[no].gn_name, 
	// 			strTime1, strTime2, ChangeVal);
	// 		MessShow(m_LogString);
	// 		
	// 		return MENU_LFO_DEV_STS_BRK;
	// 	}

	return MENU_LFO_DEV_STS_OK;
}

// �Ƕ���Ч���ж�
int CLfoAnaObj::IsAglDatValid(int nAgl, float* pAgl, float MaxChg)
{
	if (para_IsDataValid == 0)
	{
		return MENU_LFO_DEV_STS_OK;
	}

	int    ii;

	for (ii = 1; ii < nAgl; ii++)
	{
		if (DP_ABS(pAgl[ii] - pAgl[ii - 1]) > MaxChg)
		{
			return MENU_LFO_DEV_STS_BRK;
		}
	}

	return MENU_LFO_DEV_STS_OK;
}

// ��ѹ��������Ч���ж�
int CLfoAnaObj::IsTrDatValid(int no, int ChgTimCoe, float ChgMinVal)
{
	if (para_IsDataValid == 0)
	{
		return MENU_LFO_DEV_STS_OK;
	}

	int    ii;
	float  ChangeVal = -1.0f, pwr = 0.0f, max_pwr = 5000.0, min_pwr = -5000.0;;
	float  MaxChangeLimVal = L_PWR_MAX_CHG * ChgTimCoe;
	char   strTime1[28], strTime2[28];

	for (ii = 0; ii < m_nOrigSam; ii++)
	{
		//  		sprintf(m_LogString,"��ѹ��[%s]�й�[%f]�����[%f]", m_pTrInf[no].tr_name, m_pTrDat[no].pwr[ii],
		//  			m_pTrInf[no].max_pwr );
		//  		MessShow(m_LogString);	//20141008	
		pwr = m_pTrInf[no].max_pwr;
		if (pwr > 1.0)
		{
			max_pwr = 2.0 * pwr;
			min_pwr = -2.0 * pwr;
		}
		// 		sprintf(m_LogString,"��ѹ��Pmax=[%f],Pmin=[%f]", m_pTrInf[no].tr_name,max_pwr,min_pwr);
		//  		MessShow(m_LogString);	
		//         if ( m_pTrDat[no].pwr[ii] < min_pwr || m_pTrDat[no].pwr[ii] > max_pwr )
		//         {
		//             sprintf( strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
		//             sprintf( strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));
		//             
		//             sprintf(m_LogString,"IsTrDatValid(): Tr[%s] [%s-%s] �й������쳣��%5.2f [%5.2f , %5.2f]", m_pTrInf[no].tr_name, 
		//                 strTime1, strTime2, m_pTrDat[no].pwr[ii], min_pwr, max_pwr );
		//             MessShow(m_LogString);
		//             return MENU_LFO_DEV_STS_VAL;
		//         }
	}

	//     ChangeVal = JudgeTwoValDelta( &m_pTrDat[no].pwr[0], m_nOrigSam, MaxChangeLimVal, ChgMinVal, 1 ) ;
	if (ChangeVal > ZEROVALUE)
	{
		sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_CalcStartTime));
		sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));

		sprintf(m_LogString, "IsTrDatValid(): Tr[%s] [%s-%s] ����ͻ�䣬ͻ������%5.2f", m_pTrInf[no].tr_name,
			strTime1, strTime2, ChangeVal);
		MessShow(m_LogString);

		return MENU_LFO_DEV_STS_BRK;
	}

	return MENU_LFO_DEV_STS_OK;
}

// �ж��������ݵ��Ƿ���ͻ��(ͻ�䷵�� fabs(data[i+1] - data[i])���޸�������������㲻ͻ�䷵��-1 )
// �жϷ�����1)����������Բ� > (ǰʮ����Բ�֮��)�� 2) ���ݸ�����������Բ�
float CLfoAnaObj::JudgeTwoValDelta(float* data, int num, float max_change_lim, float delta_lim_val, int algorithm)
{
	int   i, j;
	int   count = 0;
	float delta_lim_calc = 0;
	float delta_lim = 0;

	if (num < 3)  return -1;

	if (algorithm == 1)
	{
		if (num > 10)
		{
			count = 10;
		}
		else
		{
			count = num;
		}

		for (j = 0; j < (count - 1); j++)
		{
			if (DP_ABS(data[num - 1 - j] - data[num - 2 - j]) > max_change_lim)
			{
				return DP_ABS(data[num - 1 - j] - data[num - 2 - j]);
			}
			else
			{
				delta_lim_calc += DP_ABS(data[num - 1 - j] - data[num - 2 - j]);
			}
		}

		for (i = 0; i < (num - 2); i++)
		{
			if (i > 0 && i < count)
			{
				delta_lim_calc = delta_lim_calc - (DP_ABS(data[num - 1 - (count - 1 - i)] - data[num - 2 - (count - 1 - i)])) + (DP_ABS(data[i] - data[i - 1]));
			}
			else if (i >= count)
			{
				delta_lim_calc = delta_lim_calc - (DP_ABS(data[i - count + 1] - data[i - count])) + (DP_ABS(data[i] - data[i - 1]));
			}

			// ��ֹż���Ľ�Сë�̣�����Ϊͻ��
			if (delta_lim_calc < (delta_lim_val))
			{
				delta_lim = (delta_lim_val);
			}
			else
			{
				delta_lim = delta_lim_calc;
			}

			if (DP_ABS(data[i + 1] - data[i]) > max_change_lim)
			{
				return DP_ABS(data[i + 1] - data[i]);
			}
			else
			{
				if (DP_ABS(data[i + 1] - data[i]) > delta_lim)
				{
					if (DP_ABS(data[i + 2] - data[i]) < delta_lim)
					{
						data[i + 1] = (float)((data[i + 2] + data[i]) / 2.0);
					}
					else
					{
						return DP_ABS(data[i + 1] - data[i]);
					}
				}
			}
		}
	}
	else
	{
		delta_lim = delta_lim_val;
		for (i = 0; i < (num - 1); i++)
		{
			if (DP_ABS(data[i + 1] - data[i]) > delta_lim_val)
			{
				return DP_ABS(data[i + 1] - data[i]);
			}
		}
	}


	return -1;
}

//  LFOԤ�ж�((���ڶ���ֵ < �趨�İ�ȫ�ż�ֵ,��Ϊ��ȫ) 0 ��ȫ��1 ��Ҫ����)
//        �������ж��⣬��·���ʣ�����ƽ��ֵ�жϣ���ֹ��·���ز���
//          check_aver = 1 ���ƽ��ֵ =0 �����ƽ��ֵ;   
//          ���ƽ��ֵ��С���趨ֵ(aver_lim) ����Ϊ���ʴ����С�������з��� ���� -1
int CLfoAnaObj::CurveLfoPreEstimate(SEQ_BASE_INFO dataBaseInfo, float ampl_sec_lim, int check_aver, float aver_lim)
{
	if (check_aver == 1)  // ����ֵ
	{
		if (fabs((double)dataBaseInfo.ave_val) < aver_lim)
		{
			return -1;   // ��Ϊ���ʴ����С�������з���
		}
		else
		{
			if (dataBaseInfo.max_ampl > ampl_sec_lim)
			{
				return 1;
			}
			else
			{
				return 0;     //����Ҫ����
			}
		}
	}
	else
	{
		if (dataBaseInfo.max_ampl > ampl_sec_lim)
		{
			return 1;
		}
		else
		{
			return 0;     //����Ҫ����
		}
	}
}

///////////////////////////////// ������� /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  �������� : ģ̬�ֲ���Ϣ����ȽϺ��� ampl��С����
////////////////////////////////////////////////////////////////////////////////
extern "C" int ModeShapeCompareByAmpl(const void* p1, const void* p2)
{
	if (((MODE_PART_OBJ*)p1)->ampl != ((MODE_PART_OBJ*)p2)->ampl)
	{
		return ((MODE_PART_OBJ*)p1)->ampl > ((MODE_PART_OBJ*)p2)->ampl ? 1 : -1;
	}
	return ((MODE_PART_OBJ*)p1)->engy > ((MODE_PART_OBJ*)p2)->engy ? 1 : -1;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ��������� fact�ɴ�С
////////////////////////////////////////////////////////////////////////////////
extern "C" int LfoCenterCompareByFact(const void* p1, const void* p2)
{
	if (((LFO_FC_ANA*)p1)->fact != ((LFO_FC_ANA*)p2)->ampl)
	{
		return ((LFO_FC_ANA*)p1)->fact > ((LFO_FC_ANA*)p2)->fact ? 1 : -1;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ������ʼʱ�� ��С��������  ��ʼʱ����ͬ,���񵴷�ֵ�ɴ�С����
////////////////////////////////////////////////////////////////////////////////
extern "C" int ResSaveCompareByBgnTime(const void* p1, const void* p2)
{
	if (((LFO_OBJ_RES_SAVE*)p1)->lfo_b_t != ((LFO_OBJ_RES_SAVE*)p2)->lfo_b_t)
	{
		return ((LFO_OBJ_RES_SAVE*)p1)->lfo_b_t > ((LFO_OBJ_RES_SAVE*)p2)->lfo_b_t ? 1 : -1;
	}
	return ((LFO_OBJ_RES_SAVE*)p1)->max_ampl > ((LFO_OBJ_RES_SAVE*)p2)->max_ampl ? -1 : 1;
	//return -1;
}
// ������
//  ������֯
//  prony������֯
//  (LFO�ж�)
	//  ���Ԥ�к���·���������ݣ������豸�ж�
	//  ϵͳ״̬�ж�
	//  �����澯��Ϣ
	//  �����豸����ģʽ
//  prony������֯
//  (LFO����)
	//  ���Ԥ�к�վ���������ݣ�ʶ��ģʽ
	//  �����Ҫģʽ������ģ̬
	//  �����������
	//  ʶ��������
	//  ����������
//  (�������)
//  �����֯
void CLfoAnaObj::MainProcess()
{
	int             ii, jj = 0;
	struct  timeb   t_StartTime;
	struct  timeb   t_EndTime;
	float             t_total;
	//int             m_nCountTimes = 0;
	int             t_nowTime = 0;
	//char            temp_message[1024];
	char            fileName[MAXBUF];

	m_wRoleID = MAIN_ROLE;  // �����ƶ������
	InitialLFOEstimate();      // ��ʼ������
	while (true)
	{
		sprintf(m_LogString, "��ʼ��һ�ּ���");
		MessShow(m_LogString, 100);
		t_StartTime = PT_GetNowMilliSeconds();
#ifdef _WINDOWS32
		t_nowTime = PT_GetNowSeconds();
		sprintf(fileName, "%s\\run\\WamSsoAna.RUN", m_RunInfDir);
		FILE* fileptr = NULL;
		if ((fileptr = fopen(fileName, "w+")) == NULL)
		{
			sprintf(m_LogString, " MainProcess() Cannot open the file %s", fileName);
			MessShow(m_LogString);
		}
		else
		{
			fprintf(fileptr, "0\n");

			if (m_wRoleID == MAIN_ROLE)
				fprintf(fileptr, "MAIN\n");
			else if (m_wRoleID == VICE_ROLE)
				fprintf(fileptr, "VICE\n");
			else if (m_wRoleID == CALC_ROLE)
				fprintf(fileptr, "CALC\n");
			else
				fprintf(fileptr, "UNKNOWN\n");

			fprintf(fileptr, "%d", t_nowTime);
			fclose(fileptr);
		}
#endif
		//InitialLFOEstimate();      // ��ʼ������
		//��ȡ�豸������Ϣ
		if (GetFcCtl(m_nFcNum, m_pFcCtl) == 1)  m_reAllocmem = REALLOCMEM;
		if (GetLnCtl(m_nLnNum, m_pLnCtl) == 1)  m_reAllocmem = REALLOCMEM;
		if (GetGnCtl(m_nGnNum, m_pGnCtl) == 1)  m_reAllocmem = REALLOCMEM;
		if (GetTrCtl(m_nTrNum, m_pTrCtl) == 1)  m_reAllocmem = REALLOCMEM;
		//////////////////////////////////////////////////////////////////////////

		if (m_reAllocmem == REALLOCMEM)  // ���·����ڴ�
		{
			if (m_DispCtl.dsource == MENU_LFO_DATA_RT || m_DispCtl.dsource == MENU_LFO_DATA_RT_TEST)
			{
				sprintf(m_LogString, "���ݱ����ģ���ͣ���㣬�������·����ڴ�");
				MessShow(m_LogString);
				ReleaseMem();

				AllocMemforPointer();
				if (InitPrject() == false)
				{
					sprintf(m_LogString, "���·����ڴ�󣬻�ȡ�豸��Ϣʧ�ܣ������˳�����");
					MessShow(m_LogString);
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				InitialLFOEstimate();

				m_reAllocmem = NOALLOCMEM;
				if (GetFcCtl(m_nFcNum, m_pFcCtl) == 1)  m_reAllocmem = REALLOCMEM;
				if (GetLnCtl(m_nLnNum, m_pLnCtl) == 1)  m_reAllocmem = REALLOCMEM;
				if (GetGnCtl(m_nGnNum, m_pGnCtl) == 1)  m_reAllocmem = REALLOCMEM;
				if (GetTrCtl(m_nTrNum, m_pTrCtl) == 1)  m_reAllocmem = REALLOCMEM;

				if (m_reAllocmem == REALLOCMEM)
				{
					sprintf(m_LogString, "���·����ڴ���ٴη������ݱ����ģ��˳�����");
					MessShow(m_LogString);
					break;
				}
			}
		}
		CheckCalcCtlPara();              //  �����Ʋ����������豸����״̬
		//////////////////////////////////////////////////////////////////////////
		if (HandlingPmuSampleData() != 0)  //  ��ȡ���ݣ����������ļ���
		{
			// PMU Buffer not update ʱ����
			// ˢ��״̬
			// 2008-05-20 by zhw
			m_SysMon.stat = MENU_LFO_DEV_STS_OLD;
			//strcpy(m_SysMon.stat_ctx,"����δˢ�£�"); ;
			//UpdateMonSysTable( m_SysMon );
			if (para_IsDataCycle == 1)
			{
				m_FcDataFilePos = 0;
				m_FcDataFileOffset = 0;
				m_GnDataFilePos = 0;
				m_FcDataFileOffset = 0;
				m_LnDataFilePos = 0;
				m_LnDataFileOffset = 0;
				m_TrDataFilePos = 0;
				m_TrDataFileOffset = 0;
				m_FcVolFilePos = 0;                          //  ��վ��ѹ��ȡλ��
				m_FcVolFileOffset = 0;                          //  ��վ��ѹ��ȡƫ����
				m_FcAngFilePos = 0;                          //  ��վ��ѹ��ȡλ��
				m_FcAngFileOffset = 0;                          //  ��վ��ѹ��ȡƫ����
				m_GnPowFilePos = 0;                          //  ������й���ȡλ��
				m_GnPowFileOffset = 0;                          //  ������й���ȡƫ����
				m_GnAngFilePos = 0;                          //  �������Ƕ�ȡλ��
				m_GnAngFileOffset = 0;                          //  �������Ƕ�ȡƫ����
			}
			else
			{
				sprintf(m_LogString, "�����ļ���ȡ���");
				MessShow(m_LogString, LTINFO);
			}
			continue;
		}
		//  ƥ�����ʱ����
		if (m_DispCtl.dsource != MENU_LFO_DATA_RT && m_DispCtl.dsource != MENU_LFO_DATA_RT_TEST)
		{
			m_nSmpStp = (int)(1 / (m_nOrigDlt * m_nSmpFre) + 0.5);

			if (m_nSmpStp < 1)
			{
				m_nSmpStp = 1;
				sprintf(m_LogString, "MainProcess(): �������ݲ���Ƶ�ʹ���[%f]��Ӱ��������! ", m_nOrigDlt);
				MessShow(m_LogString);
				// ���߷����������Զ��˳�
				//break;
			}
		}
		m_nSamples = DP_NumBySample(m_nOrigSam, m_nSmpStp);
		for (ii = 0; ii < m_nSamples; ii++)
		{
			jj = m_nSmpStp * ii;
			m_ProMatchtime[ii] = m_Matchtime[jj];
		}

		//  ����Ԥ�����˲��������������Լ�飩
		FormMonProData();
		//////////////////////////////////////////////////////////////////////////
		//  ����
		EstimateSysOscillate();

		if (m_SysMon.if_sso == MENU_SSO_MENU_NO)//ϵͳ��ȫ
		{
			UpdateMonGnTableStat(m_nGnNum, m_pGnInf, m_pGnMon);
			UpdateMonLnTableStat(m_nLnNum, m_pLnInf, m_pLnMon);
			UpdateMonTrTableStat(m_nTrNum, m_pTrInf, m_pTrMon);
			UpdateMonFcTableStat(m_nFcNum, m_pFcInf, m_nGnNum, m_pGnMon, m_nTrNum, m_pTrMon);
			UpdateMonSysTableStat(m_SysMon);
			FormMonModeCurce();                                     //  �γ�ģʽ����
			UpdateMonModeCurce(m_pLfoModeCurve);
			DecideLfoCurveID();                                   //  ȷ��ʵʱ���߶�Ӧ�豸			
			RealtimeCurveProcess();
			sprintf(m_LogString, " ϵͳ��ȫ,������һ�ַ���");
			MessShow(m_LogString);
		}
		else if (m_SysMon.if_sso == MENU_SSO_MENU_YES || m_SysMon.if_sso == MENU_SSO_MENU_NOTOYES)//��ʼ�񵴻������
		{
			//�γ��񵴳�վ����
			sprintf(m_LogString, " ϵͳ��,�γ��񵴳�վ����");
			MessShow(m_LogString);
			PushCallFac();

			if (m_SysMon.if_sso == MENU_SSO_MENU_NOTOYES)
			{
				//����Ԥ��
				SSO_SYS_MON    tempSendSysStatus;
				memcpy((void*)&tempSendSysStatus, (void*)&m_SysMon, sizeof(SSO_SYS_MON));
				tempSendSysStatus.stat = MENU_LFO_DEV_STS_WRN;

				LFOSendWarnMessage(tempSendSysStatus, tempSendSysStatus.sso_bgn, m_DispCtl.sso_fac_id);
			}
			//���ֽ�����
			UpdateMonInfToWAMAP();
			//SSOʵʱCASE���ڱ����
			SampleHisCase();

		}
		else if (m_SysMon.if_sso == MENU_SSO_MENU_YESTONO)//��ƽϢ
		{
			sprintf(m_LogString, " ��ƽϢ,������");
			MessShow(m_LogString);
			//����Ԥ��
			//������
			UpdateMonInfToWAMAP();
			//SSO������SSO���豸�����
			SampleSsoPara();

			SampleHisDev();

			PushEveFac();

			SaveCaseFile();

			InitialLFOEstimate();//��ʼ������
		}

		if (HDFileOk())
		{
			CallHDFile();
		}


		//���汾�ּ�����Ϣ��Ϊ���ּ���׼��
		m_OldGpsTime = m_CurGpsTime;

	}
	return;
}

// ������
//  ������֯
//  prony������֯
//  (LFO�ж�)
// 	//  ���Ԥ�к���·���������ݣ������豸�ж�
// 	//  ϵͳ״̬�ж�
// 	//  �����澯��Ϣ
// 	//  �����豸����ģʽ
//  prony������֯
//  (LFO����)
// 	//  ���Ԥ�к�վ���������ݣ�ʶ��ģʽ
// 	//  �����Ҫģʽ������ģ̬
// 	//  �����������
// 	//  ʶ��������
// 	//  ����������
//  (�������)
//  �����֯
// void CLfoAnaObj::MainProcess( )
// {
// 	int             ii, jj, kk = 0;
// 	struct  timeb   t_StartTime;
// 	struct  timeb   t_EndTime;
// 	int             t_total;
// 	int             m_nCountTimes = 0;
// 
// 	while(true)
// 	{
// 		MessShow(" MainProcess(): ��ʼһ���µļ���! ");
// 		t_StartTime = TF_GetNowMilliSeconds();
// 
//     	InitialLFOEstimate();      // ��ʼ������
// 
// 		//��ȡ�豸������Ϣ
// 		if( GetFcCtl( m_nFcNum, m_pFcCtl ) == 1 )  m_reAllocmem = REALLOCMEM;
// 		if( GetLnCtl( m_nLnNum, m_pLnCtl ) == 1 )  m_reAllocmem = REALLOCMEM;
// 		if( GetGnCtl( m_nGnNum, m_pGnCtl ) == 1 )  m_reAllocmem = REALLOCMEM;
// 		//////////////////////////////////////////////////////////////////////////
// 		if(m_reAllocmem == REALLOCMEM)  // ���·����ڴ�
// 		{
// 			MessShow(" MainProcess(): ���ݱ����ģ���ͣ���㣬�������·����ڴ�! ");
// 			ReleaseMem();
// 
// 			AllocMemforPointer();
// 			if ( InitPrject() == false )
// 			{
// 				MessShow(" MainProcess(): ���·����ڴ�󣬻�ȡ�豸��Ϣʧ�ܣ������˳�����! ");
// 				break;
// 			}
// 			//////////////////////////////////////////////////////////////////////////
//     		InitialLFOEstimate();
// 
// 			m_reAllocmem = NOALLOCMEM;
// 			if( GetFcCtl( m_nFcNum, m_pFcCtl ) == 1 )  m_reAllocmem = REALLOCMEM;
// 			if( GetLnCtl( m_nLnNum, m_pLnCtl ) == 1 )  m_reAllocmem = REALLOCMEM;
// 			if( GetGnCtl( m_nGnNum, m_pGnCtl ) == 1 )  m_reAllocmem = REALLOCMEM;
// 			
// 			if (m_reAllocmem == NOALLOCMEM)
// 			{
// 				MessShow(" MainProcess(): ���·����ڴ���ٴη������ݱ����ģ��˳�����! ");
// 				break;
// 			}
// 		}
// 		CheckCalcCtlPara( );          // �����Ʋ���
// 		
// 		//////////////////////////////////////////////////////////////////////////
// 		if(HandlingPmuSampleData() != 0)  //  ��ȡ���ݣ����������ļ���
// 		{
// 			// PMU Buffer not update ʱ����
// 			// ˢ��״̬
// 			// continue;
// 		}
// 		//  ƥ�����ʱ����
// 		if(m_DispCtl.dsource != MENU_LFO_DATA_RT && m_DispCtl.dsource != MENU_LFO_DATA_RT_TEST)
// 		{
// 			m_nSmpStp = (int)(1/(m_nOrigDlt*m_nSmpFre) + 0.5);
// 
// 			if ( m_nSmpStp < 1 )
// 			{
// 				m_nSmpStp = 1;
// 				sprintf( m_LogString, "MainProcess(): �������ݲ���Ƶ�ʹ���[%f]��Ӱ��������! ", m_nOrigDlt); 
// 				MessShow( m_LogString );
// 			}
// 		}
// 		m_nSamples = DP_NumBySample( m_nOrigSam, m_nSmpStp ) ;
// 		for ( ii=0; ii<m_nSamples; ii++)
// 		{
// 			jj = m_nSmpStp * ii;
// 			m_ProMatchtime[ii] = m_Matchtime[jj];
// 		}
// 
// 		//  ����Ԥ�����˲��������������Լ�飩
// 		FormMonProData( );
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		// EEAC
// 		//WAMAPDataHandling();
// 		//EEACIdentifyCluster();
// 		//////////////////////////////////////////////////////////////////////////
// 		//  ����
// 		FormMonCaseData();
// 		sprintf( m_LogString, "MainProcess():  Form Monitor Case[%d] ", m_nCases); 
// 		WriteLogFile( m_LogString );
// 
// 		ClacCtlMainProcess(  m_nCases, m_pCases );
// 
// 		MonClacProcess( );
// 
//  		DecideLfoCurveID(  ) ;                                   //  ȷ��ʵʱ���߶�Ӧ�豸
//  		FormMonModeCurce(   );                                   //  �γ�ģʽ����
// 
// 		UpdateMonModeCurce( m_nLfoModeCurve, m_pLfoModeCurve );  //  ����ģʽ����
// 		UpdateMonInfToWAMAP(); // ʵʱ������Ϣ
// 		//unsigned char triEms_value = MENU_NO;
// 		//if ( m_SysMon.stat == MENU_LFO_DEV_STS_WRN ) triEms_value = MENU_YES;
// 		//LFOTriEmsSingle( triEms_value ); 
// 
// 		//����������ʾ���� mon
// 		//////////////////////////////////////////////////////////////////////////
// 		//  �����̴߳���ʵʱ��ʾ����
// 		FormItfcCurce();    // �γɶ�������
// 		UpdateItfcCurce( m_nItfcNum, m_pItfcDat );               //  ���¶�������
// 		//  
// 		//////////////////////////////////////////////////////////////////////////
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		//  ����
// 		//////////////////////////////////////////////////////////////////////////
// 		if (m_SysMon.stat == MENU_LFO_DEV_STS_FWN || m_SysMon.stat == MENU_LFO_DEV_STS_WRN || m_SysMon.stat == MENU_LFO_DEV_STS_WRN2 )
// 		{
// 			//  ����Ԥ������Ҫ�ԽǶȴ��� �������˲��������Լ�飩
// 			FormAnaProData( );
// 			// ��ʼ��
// 			m_nCases      = 0;
// 			memset((void *)m_pCases, 0, sizeof(CASE_TYPE) * para_nCaseMaxMem );
// 
// 			// ��֯����
// 			FormAnaCaseData();
// 			sprintf( m_LogString, "MainProcess():  Form Analysis Case[%d] ", m_nCases); 
// 			WriteLogFile( m_LogString );
// 
// 			ClacCtlMainProcess( m_nCases, m_pCases );
// 
// 			AnaCalcProcess( );
// 
// 			UpdateAnaInfToWAMAP(); // ���߷�����Ϣ
// 		}
// 		//////////////////////////////////////////////////////////////////////////
// 		t_EndTime = TF_GetNowMilliSeconds();
// 		t_total   = TF_CalculateTimeInterval(t_StartTime, t_EndTime);
// 		
// 		int nM,nS,nMs;
// 		char temp_time_str[16];
// 		nM = t_total/60000;
// 		nS = (t_total-nM*60000)/1000;
// 		nMs = t_total-nM*60000-nS*1000;
// 		if(nM >0)
// 		{
// 			sprintf(temp_time_str, "%d��%d��%d����",nM,nS,nMs);
// 		}
// 		else
// 		{
// 			if(nS >0)
// 			{
// 				sprintf(temp_time_str, "%d��%d����",nS,nMs);
// 			}
// 			else
// 			{
// 				sprintf(temp_time_str, "%d����",nMs);
// 			}
// 		}
// 		//strcpy(m_SysStatus.use_time, temp_time_str);
//          
// 
// 		sprintf( m_LogString, "MainProcess():  ���ּ����������ʱ%d��%d��%d����", nM, nS, nMs ); 
// 		MessShow( m_LogString );
// 	}
// 	return ;
// }

// LFO�ж�����
void CLfoAnaObj::MonClacProcess()
{
	//  ���Ԥ�к���·�������ж�
	//  ���ü����豸״̬
	//  ϵͳ״̬�ж�
	//  ��������ģʽ
	//  ���澯��Ϣ��tmd����
// 	ModeResultHandling( m_LfoPara.stor_damp ); // �������м����豸ȫģʽ 
	UpdateMonMdTable(m_nAllObjMode, m_pAllObjMode);

	ExtractLeadMode();       // ��ȡ����ģʽ,���m_pLnMon,m_pGnMon
// 	EstimateObjOscillate();  // �жϼ�������� ���� m_pLnMon��m_pLnOsc�� m_pGnMon��m_pGnOsc��
// 	EstimateSysOscillate();  // �ж�ϵͳ�����(�����澯) m_pLnMon��m_pGnMon

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��վ������� ampl�ɴ�С
////////////////////////////////////////////////////////////////////////////////
extern "C" int FcAnaResCompareByAmpl(const void* p1, const void* p2)
{
	if (((LFO_FC_ANA*)p1)->ampl != ((LFO_FC_ANA*)p2)->ampl)
	{
		return ((LFO_FC_ANA*)p1)->ampl > ((LFO_FC_ANA*)p2)->ampl ? 1 : -1;
	}
	return -1;
}

// LFO��������
void CLfoAnaObj::AnaCalcProcess()
{
	//  ���Ԥ�к�վ���������ݣ�ʶ��ģʽ
	//  �����Ҫģʽ������ģ̬
	//  �����������
	//  ʶ��������
	//  ����������
	//////////////////////////////////////////////////////////////////////////

// 	int  ii, jj, kk;
// 	// filter obj mode :  case -> m_pAllObjMode m_nAllObjMode
// 		LfoClctMainMode();
// 		//WriteAllObjModeToFile( m_nAllObjMode, m_pAllObjMode );
// 		
// 		// identify Gn and Ln of the mode :  m_pAllObjMode -> m_pAllMode m_nAllMode
// 		g_theLfoAna.IdtfPartGnPartLn( m_nAllObjMode, m_pAllObjMode, m_nAllMode, m_pAllMode, m_LfoPara.sm_md_freq, m_LfoPara.sm_md_damp );
// 		g_theLfoAna.CalcModeGnLnEngy( m_nAllMode, m_pAllMode );
// 		
// 		sprintf( m_LogString, "Analysis Case��%d; obj mode��%d; sys mode��%d", m_nCases, m_nAllObjMode, m_nAllMode ); 
// 		MessShow( m_LogString );
// 		//WriteModeShapeToFile( m_nAllMode, m_pAllMode );
// 		
// 		// Calclation modeshape �� CalcPartFactor( int iObjNum, MODE_PART_OBJ *ModeShape );
// 		if ( m_nAllMode > 0 ) 
// 		{
// 			sprintf( m_LogString, "����ģ̬�ֲ�!");
// 			MessShow( m_LogString );
// 			for (ii=0; ii<m_nAllMode; ii++)
// 			{
//                 m_pAllMode[ii].time_bgn = m_CalcStartTime; // 2008-06-04 by zhw at nj
//                 m_pAllMode[ii].time_end = m_CurGpsTime;
// 
// 				if ( m_pAllMode[ii].nPartGn > 0 )
// 				{
// 					if(para_LfoAnaByPwr == 0)
// 					{
// // 						if (para_LfoAnaByEEAC == 1)
// // 						{
// // 							// ���Ӷ�Tj�Ĵ��� �쳣�������
// // 							LfoAnaPartGenPreHandle( m_nCluDataGens, m_genData, m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn );
// // 							if (g_theLfoAna.CalcPartFactor( m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn,  para_LfoAnaByPwr ) == false )
// // 								LfoAnaSetPartFact0(  m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn );
// // 						}
// // 						else
// 						{
// 							// ���Ӷ�Tj�Ĵ��� �쳣�������
// 							LfoAnaPartGenPreHandle( m_nGnNum, m_pGnInf, m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn );
// 							if (g_theLfoAna.CalcPartFactor( m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn,  para_LfoAnaByPwr ) == false )
// 								LfoAnaSetPartFact0(  m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn );
// 						}
// 					}
// 					else
// 						g_theLfoAna.CalcPartFactor( m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn );
// 
// 					// set sign
// 					//g_theLfoAna.JudgPartFactorSign( m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn );
// 				}
// // 				// �ɹ���· //2008-09-24 ��
// // 				if ( m_pAllMode[ii].nPartLn > 0 )
// // 				{
// // 					g_theLfoAna.CalcPartFactor( m_pAllMode[ii].nPartLn, m_pAllMode[ii].PartLn );
// // 					// set sign
// // 					//g_theLfoAna.JudgPartFactorSign( m_pAllMode[ii].nPartLn, m_pAllMode[ii].PartLn );
// // 				}
// 				// ��վ�ֲ�
// 				if ( m_pAllMode[ii].nPartFc > 0 )
// 				{
// 					g_theLfoAna.CalcPartFactor( m_pAllMode[ii].nPartFc, m_pAllMode[ii].PartFc );
// 					// set sign
// 					//g_theLfoAna.JudgPartFactorSign( m_pAllMode[ii].nPartFc, m_pAllMode[ii].PartFc );
// 				}
// 			}
// 		}
// 		
// 		//WriteModeShapeToFile( m_nAllMode, m_pAllMode , 1);
// 		//WriteModeShapeToFile( m_nAllMode, m_pAllMode );  // 2008-06-04 by zhw at nj
// 		
// 		// ģ̬ɸѡ��
// 		m_nSltMode   = 0;
// 		memset((void*)m_pSltMode, 0, sizeof(MODE_CHRCT) * para_nSysMaxMode); 
// 		if ( m_nAllMode > 0 ) 
// 		{
// 			sprintf( m_LogString, "ɸѡģ̬�ֲ�!");
// 			MessShow( m_LogString );
// 			if (m_nAllMode < para_nSysMaxMode) 
// 				m_nSltMode = m_nAllMode;
// 			else
// 				m_nSltMode = para_nSysMaxMode;
// 
// 			memcpy( (void*)m_pSltMode, (void*)m_pAllMode, sizeof(MODE_CHRCT)*m_nSltMode );
// 			for (ii=0; ii<m_nSltMode; ii++)
// 			{
// 				// ��׼ѡȡ��gn��
// 				g_theLfoAna.IdtfCausLfoGn( m_pSltMode[ii].nPartGn, m_pSltMode[ii].PartGn, m_LfoPara.cohcy_angl );
// 				// ��վ�迼���뷢���һ��
// 				if ( m_pSltMode[ii].nPartGn > 0 )
// 				{
// 					// 2008-09-24 g_theLfoAna.IdtfLfoGnCohcyFc( m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc, m_pSltMode[ii].PartGn[0].phas, (float)(m_pSltMode[ii].PartGn[0].phas+PRONY_PI+0.03), m_LfoPara.cohcy_angl );
// 					g_theLfoAna.IdtfLfoGnCohcyFc( m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc, (MODE_PART_OBJ*)&m_pSltMode[ii].PartGn[0].obj_id, m_LfoPara.cohcy_angl );
// 				}
// 				else
// 				{
// 					g_theLfoAna.IdtfCausLfoGn( m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc, m_LfoPara.cohcy_angl );
// 				}
// 				// 2008-09-24 add ͨ��Fc����Ln/Tr����Ⱥ
// 				//////////////////////////////////////////////////////////////////////////
// 				if ( m_pSltMode[ii].nPartFc > 0 )
// 				{
// 					LfoAnaSetLnClustbyFc( m_pSltMode[ii].nPartLn, m_pSltMode[ii].PartLn, m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc );
// 					LfoAnaSetTrClustbyFc( m_pSltMode[ii].nPartTr, m_pSltMode[ii].PartTr, m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc );
// 				}
// 				//////////////////////////////////////////////////////////////////////////
// 				// ͳһ�������ӱ�׼
// 				//////////////////////////////////////////////////////////////////////////
// 				LfoAnaSetGnAmpl( m_pSltMode[ii].nPartGn, m_pSltMode[ii].PartGn );
// 				LfoAnaResetFact( (MODE_CHRCT*)&m_pSltMode[ii].mode_id );
// 				//////////////////////////////////////////////////////////////////////////
// 				// ����ʶ��
// 				//////////////////////////////////////////////////////////////////////////
// 				g_theLfoAna.IdtfLfoCenter( (MODE_CHRCT*)&m_pSltMode[ii].mode_id );
// 				//////////////////////////////////////////////////////////////////////////
// 			}
// 		}
// 		//WriteModeShapeToFile( m_nSltMode, m_pSltMode);  // 2008-06-04 by zhw at nj
// 
// 		//�������ж� ->����ʶ��->��ѹ����
// 	    float MaxVoltAmpl=0.0;
// 		if ( m_nFcProDat > 0 )
// 		{
// 			for( ii=0; ii<m_nFcProDat; ii++)
// 			{
// 				m_pFcAna[ii].fac_id       = m_pFcProDat[ii].fac_id;
// 				for( jj=0; jj< m_nFcNum; jj++)
// 				{
// 					if( m_pFcAna[ii].fac_id == m_pFcInf[jj].fac_id)
// 					{
// 						strcpy(m_pFcAna[ii].fac_name,m_pFcInf[jj].fac_name);
// 						break;
// 					}
// 				}
// 				m_pFcAna[ii].lfo_v_aver   = m_pFcProDat[ii].ave_val; 
// 				m_pFcAna[ii].lfo_v_max    = m_pFcProDat[ii].max_val.value; 
// 				m_pFcAna[ii].lfo_v_min    = m_pFcProDat[ii].min_val.value ; 
// 				m_pFcAna[ii].lfo_v_ampl   = m_pFcProDat[ii].max_ampl; 
// 				
// 				m_pFcAna[ii].lfo_is_cntr  = MENU_LFO_MON_NO; 
// 				m_pFcAna[ii].lfo_is_disp  = MENU_LFO_MON_NO ; 
// 				m_pFcAna[ii].lfo_clus     = MENU_LFO_CLUST_RMAIN; 
// 				// ��ȡ��ѹ�������ֵ
// 				if (m_pFcAna[ii].lfo_v_ampl > MaxVoltAmpl )
// 					MaxVoltAmpl = m_pFcAna[ii].lfo_v_ampl;
// 
// 				// ���������
// 				if ( m_nSltMode > 0 && m_pSltMode[0].nPartFc > 0)
// 				{
// 					for (kk=0; kk<m_pSltMode[0].nPartFc; kk++)
// 					{
// 						if ( m_pFcAna[ii].fac_id == m_pSltMode[0].PartFc[kk].obj_id )
// 						{
// 							m_pFcAna[ii].ampl  = m_pSltMode[0].PartFc[kk].ampl; 
// 							m_pFcAna[ii].freq  = m_pSltMode[0].PartFc[kk].freq; 
// 							m_pFcAna[ii].phas  = m_pSltMode[0].PartFc[kk].phas ; 
// 							m_pFcAna[ii].damp  = m_pSltMode[0].PartFc[kk].damp; 
// 							
// 							m_pFcAna[ii].lfo_clus = m_pSltMode[0].PartFc[kk].clus;
// 							
// 							break;
// 						}
// 					}
// 				}
// 			}
// 
// 			//����
// 			qsort((void *)m_pFcAna, m_nFcProDat , sizeof(LFO_FC_ANA), FcAnaResCompareByAmpl);
// 		}
// 
// 		sprintf( m_LogString, "AnaCalcProcess():��ѹ���ڶ�[%f]����ֵ[%f]�����ĳ�����ֵ[%d]!",MaxVoltAmpl, MaxVoltAmpl*para_VoltAmplPer, para_nShwMaxFcNum);
// 		MessShow( m_LogString );
//  		// �������ж�
// 		MaxVoltAmpl = MaxVoltAmpl*para_VoltAmplPer;
// 		int  tempCntrFcNum = 0;
// 		if ( m_nSltMode > 0 )
// 		{
// 			if ( m_pSltMode[0].nLfoCenter > 0 )
// 			{
// 				for (ii=0; ii<m_pSltMode[0].nLfoCenter; ii++)
// 				{
// 					for (jj=0; jj<m_nFcProDat; jj++ )
// 					{
// 						if ( m_pSltMode[0].LfoCenter[ii].fac_id == m_pFcAna[jj].fac_id )
// 						{
// 							if (m_pFcAna[jj].ampl > MaxVoltAmpl)
// 							{
// 								m_pFcAna[jj].lfo_is_cntr = MENU_LFO_MON_YES;
// 								m_pFcAna[jj].lfo_is_disp = MENU_LFO_MON_YES;
// 								tempCntrFcNum +=1;
// 								break;
// 							}
// 						}
// 					}
// 
// 					if (tempCntrFcNum >= para_nShwMaxFcNum )
// 					{
// 						break; // �ﵽ��ֵ������
// 					}
// 				}
// 			}
// 		}
// 
// 		//
// 		if (tempCntrFcNum > 0)
// 		{
// 			strcpy( m_SysAna.center_desc, "������λ�����³�վ������");
//             for( ii=0; ii< m_nFcProDat; ii++)
//             {
//                 if( m_pFcAna[ii].lfo_is_cntr == MENU_LFO_MON_YES )
//                 {
//                     m_SysAna.center_id = m_pFcAna[ii].fac_id;
//                     sprintf(m_LogString,"%s��",m_pFcAna[ii].fac_name);
//                     strcat( m_SysAna.center_desc,m_LogString);
//                 }
//             }
// 		}
// 		else
// 		{
// 			m_SysAna.center_id = -1;
// 			strcpy( m_SysAna.center_desc, "�޷�ȷ��������!");
// 		}
// 		MessShow(m_SysAna.center_desc,LTINFO);
////////////////////////////2008-09-24/////////////////////////////////
// 		if ( m_nFcProDat > 0 )
// 		{
// 			for( ii=0; ii<m_nFcProDat; ii++)
// 			{
// 				m_pFcAna[ii].fac_id       = m_pFcProDat[ii].fac_id;
// 				for( jj=0; jj< m_nFcNum; jj++)
//                 {
//                     if( m_pFcAna[ii].fac_id == m_pFcInf[jj].fac_id)
//                     {
//                         strcpy(m_pFcAna[ii].fac_name,m_pFcInf[jj].fac_name);
//                         break;
//                     }
//                 }
// 				m_pFcAna[ii].lfo_v_aver   = m_pFcProDat[ii].ave_val; 
// 				m_pFcAna[ii].lfo_v_max    = m_pFcProDat[ii].max_val.value; 
// 				m_pFcAna[ii].lfo_v_min    = m_pFcProDat[ii].min_val.value ; 
// 				m_pFcAna[ii].lfo_v_ampl   = m_pFcProDat[ii].max_ampl; 
// 				
// 				m_pFcAna[ii].lfo_is_cntr  = MENU_LFO_MON_NO; 
// 				m_pFcAna[ii].lfo_is_disp  = MENU_LFO_MON_NO ; 
// 				m_pFcAna[ii].lfo_clus     = MENU_LFO_CLUST_RMAIN; 
// 			}
// 			
// 			if ( m_nSltMode > 0 )
// 			{
// 				if ( m_pSltMode[0].nPartFc > 0 )
// 				{
// 					int                 temp_nSltFc;                 // һ�˳�վ��
// 					MODE_PART_OBJ      *temp_pSltFc;                 // һ�˳�վ
// 					temp_nSltFc = 0;
// 					temp_pSltFc = (MODE_PART_OBJ *)calloc(para_nAnaMaxFcNum, sizeof(MODE_PART_OBJ));
// 					memset((void*)temp_pSltFc, 0, sizeof(MODE_PART_OBJ) * para_nAnaMaxFcNum);
//                     
// 					// m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc
// 					// g_theLfoAna.IdtfLfoCenter( m_pSltMode[0].nPartFc, m_pSltMode[0].PartFc, m_nFcProDat, m_pFcProInf, para_nShwMaxFcNum );
// 					//////////////////////////////////////////////////////////////////////////
// 					// һ�˳�վ
// 					for( ii=0; ii<m_pSltMode[0].nPartFc; ii++)
// 					{
// 						if ( m_pSltMode[0].PartFc[ii].fact > 0 ) 
// 						{
// 							g_theLfoAna.MemCpyModePartObjStru( m_pSltMode[0].PartFc[ii], temp_pSltFc[temp_nSltFc] );
// 							temp_nSltFc += 1;
// 						}
// 					}
// 					if ( temp_nSltFc > 0 )
// 					{
// 						// sort by ampl
// 						qsort((void *)temp_pSltFc, temp_nSltFc, sizeof(MODE_PART_OBJ), ModeShapeCompareByAmpl);
// 						
// 						if ( temp_nSltFc > para_nShwMaxFcNum ) temp_nSltFc = para_nShwMaxFcNum;
// 						for( ii=0; ii<m_nFcProDat; ii++)
// 						{
// 							for( jj=0; jj<temp_nSltFc; jj++)
// 							{
// 								if ( m_pFcAna[ii].fac_id == temp_pSltFc[jj].obj_id )
// 								{
// 									m_pFcAna[ii].ampl  = temp_pSltFc[jj].ampl; 
// 									m_pFcAna[ii].freq  = temp_pSltFc[jj].freq; 
// 									m_pFcAna[ii].phas  = temp_pSltFc[jj].phas ; 
// 									m_pFcAna[ii].damp  = temp_pSltFc[jj].damp; 
// 									
// 									m_pFcAna[ii].lfo_is_cntr  = MENU_LFO_MON_YES; 
// 									m_pFcAna[ii].lfo_is_disp  = MENU_LFO_MON_YES; 
// 									m_pFcAna[ii].lfo_clus     = MENU_LFO_CLUST_CRTCL;                        //  ����Ⱥ
// 									
// 									break;
// 								}
// 							}
// 						}
// 						
// 					}
// 					//////////////////////////////////////////////////////////////////////////
// 					// ��һ�˳�վ
// 					temp_nSltFc   = 0;
// 					memset((void*)temp_pSltFc, 0, sizeof(MODE_PART_OBJ) * para_nAnaMaxFcNum); 
// 					
// 					for( ii=0; ii<m_pSltMode[0].nPartFc; ii++)
// 					{
// 						if ( m_pSltMode[0].PartFc[ii].fact < 0 ) 
// 						{
// 							g_theLfoAna.MemCpyModePartObjStru( m_pSltMode[0].PartFc[ii], temp_pSltFc[temp_nSltFc] );
// 							temp_nSltFc += 1;
// 						}
// 					}
// 					if ( temp_nSltFc > 0 )
// 					{
// 						// sort by ampl
// 						qsort((void *)temp_pSltFc, temp_nSltFc, sizeof(MODE_PART_OBJ), ModeShapeCompareByAmpl);
// 						
// 						if ( temp_nSltFc > para_nShwMaxFcNum ) temp_nSltFc = para_nShwMaxFcNum;
// 						for( ii=0; ii<m_nFcProDat; ii++)
// 						{
// 							for( jj=0; jj<temp_nSltFc; jj++)
// 							{
// 								if ( m_pFcAna[ii].fac_id == temp_pSltFc[jj].obj_id )
// 								{
// 									m_pFcAna[ii].ampl  = temp_pSltFc[jj].ampl; 
// 									m_pFcAna[ii].freq  = temp_pSltFc[jj].freq; 
// 									m_pFcAna[ii].phas  = temp_pSltFc[jj].phas ; 
// 									m_pFcAna[ii].damp  = temp_pSltFc[jj].damp; 
// 									
// 									m_pFcAna[ii].lfo_is_cntr  = MENU_LFO_MON_YES; 
// 									m_pFcAna[ii].lfo_is_disp  = MENU_LFO_MON_YES; 
// 									m_pFcAna[ii].lfo_clus     = MENU_LFO_CLUST_OPOST;                        //  ����Ⱥ
// 									
// 									break;
// 								}
// 							}
// 						}
// 					}
// 
//                     //WriteLfoCenterToFile( m_SysLfo, m_nFcProDat, m_pFcAna );
//                     // 2008-08-14 ���ӵ�ѹ��ֵ����
//                     //  �õ������ĳ�վ�ڵ�ѹ�������ֵ
//                     MaxVoltAmpl = 0.0;
//                     for(ii=0; ii<m_nFcProDat; ii++)
//                     {
//                         if( m_pFcAna[ii].lfo_is_cntr ==MENU_LFO_MON_YES \
//                          && m_pFcAna[ii].lfo_v_ampl > MaxVoltAmpl)
//                         {
//                             MaxVoltAmpl = m_pFcAna[ii].lfo_v_ampl;
//                         }
//                     }
// 
//                     //  ��ѹ��ֵ��С����������ȥ��
//                     for(ii=0; ii<m_nFcProDat; ii++)
//                     {
//                         if( m_pFcAna[ii].lfo_is_cntr ==MENU_LFO_MON_YES \
//                          && m_pFcAna[ii].lfo_v_ampl < para_VoltAmplPer*MaxVoltAmpl)
//                         {
//                             m_pFcAna[ii].lfo_is_cntr = MENU_LFO_MON_NO;
//                             m_pFcAna[ii].lfo_is_disp = MENU_LFO_MON_NO;
//                         }
//                     }
// 					if (temp_pSltFc) { free(temp_pSltFc); temp_pSltFc =NULL; }
// 					
// 					strcpy( m_SysAna.center_desc, "����������λ�����³�վ������");
//                     for( ii=0; ii< m_nFcProDat; ii++)
//                     {
//                         if( m_pFcAna[ii].lfo_is_cntr == MENU_LFO_MON_YES )
//                         {
//                             m_SysAna.center_id = m_pFcAna[ii].fac_id;
//                             sprintf(m_LogString,"%s��",m_pFcAna[ii].fac_name);
//                             strcat( m_SysAna.center_desc,m_LogString);
//                         }
//                     }
// 				}
// 				else
// 				{
// 					m_SysAna.center_id = -1;
// 					strcpy( m_SysAna.center_desc, "�޷�ȷ�������ģ�");						
// 				}
// 			}
// 			else
// 			{
// 				m_SysAna.center_id = -1;
// 				strcpy( m_SysAna.center_desc, "�޷�ȷ�������ģ�");
// 			}
// 		}
// 		else
// 		{
// 			m_SysAna.center_id = -1;
// 			strcpy( m_SysAna.center_desc, "�޷�ȷ�������ģ�");
// 		}
///
//////////////////////////////////////////////////////////////////////////
// 		WriteLfoCenterToFile( m_SysAna, m_nFcProDat, m_pFcAna );

// 		//////////////////////////////////////////////////////////////////////////
// 		// �����������÷���
// 		int    temp_gnNum1       = 0;
// 		float  temp_gnAverAng1   = 0;
// 		int    temp_gnNum2       = 0;
// 		float  temp_gnAverAng2   = 0;
// 		float  temp_cntrAverAng  = 0;
// 		if ( m_nSltMode > 0 ) 
// 		{
// 			for (ii=0; ii<m_nSltMode; ii++)
// 			{
// 				temp_gnNum1       = 0;
// 				temp_gnAverAng1   = 0;
// 				temp_gnNum2       = 0;
// 				temp_gnAverAng2   = 0;
// 				temp_cntrAverAng  = 0;
// 
// 				if ( m_pSltMode[ii].nPartGn <= 0 ) continue;
// 
// 				for ( jj=0; jj<m_pSltMode[ii].nPartGn; jj++)
// 				{
// 					if ( m_pSltMode[ii].PartGn[jj].fact >= 0 ) 
// 					{
// 						for( kk=0; kk<m_nGnProDat; kk++)
// 						{
// 							if ( m_pSltMode[ii].PartGn[jj].obj_id == m_pGnProDat[kk].gn_id ) 
// 							{
// 								temp_gnAverAng1   += m_pGnProDat[kk].ang[0];
// 								temp_gnNum1       += 1;
// 								break;
// 							}
// 						}
// 					}
// 					else if ( m_pSltMode[ii].PartGn[jj].fact < 0 ) 
// 					{
// 						for( kk=0; kk<m_nGnProDat; kk++)
// 						{
// 							if ( m_pSltMode[ii].PartGn[jj].obj_id == m_pGnProDat[kk].gn_id ) 
// 							{
// 								temp_gnAverAng2   += m_pGnProDat[kk].ang[0];
// 								temp_gnNum2       += 1;
// 								break;
// 							}
// 						}
// 					}
// 				}
// 
// 				if ( temp_gnNum1 > 0 && temp_gnNum2 > 0 )
// 				{
// 					temp_gnAverAng1 = temp_gnAverAng1/temp_gnNum1;
// 					temp_gnAverAng2 = temp_gnAverAng2/temp_gnNum2;
// 					if ( temp_gnAverAng1 < temp_gnAverAng2)  // �������÷���
// 					{
// 						g_theLfoAna.ReSetFactorSign( m_pSltMode[ii].nPartGn, m_pSltMode[ii].PartGn );
// 					}
// 
// 					// 2007-11-29 EEAC��ͬȺ�������ӻ�׼��ͬ( m_pSltMode[ii].PartGn���ɴ�С���� )
// 					if ( para_LfoAnaByEEAC == 1 )
// 					{
// 						temp_gnAverAng1 = m_pSltMode[ii].PartGn[0].fact;
// 						temp_gnAverAng2 = m_pSltMode[ii].PartGn[m_pSltMode[ii].nPartGn-1].fact;
// 						if ( temp_gnAverAng1 < 0 ) temp_gnAverAng1 = -temp_gnAverAng1;
// 						if ( temp_gnAverAng2 < 0 ) temp_gnAverAng2 = -temp_gnAverAng2;
// 
// 						for ( jj=0; jj<m_pSltMode[ii].nPartGn; jj++ )
// 						{
// 							if ( m_pSltMode[ii].PartGn[jj].fact >= 0 ) 
// 							{
// 								m_pSltMode[ii].PartGn[jj].fact = m_pSltMode[ii].PartGn[jj].fact/temp_gnAverAng1;
// 							}
// 							else
// 							{
// 								m_pSltMode[ii].PartGn[jj].fact = m_pSltMode[ii].PartGn[jj].fact/temp_gnAverAng2;
// 							}
// 						}
// 						
// 					}
// 				}
// 				else  // �����ͬ�࣬����
// 				{
// 					sprintf( m_LogString, "����������ͬ�࣬������������жϷ���!");
// 					WriteLogFile( m_LogString );
// 				}
// 
// 			}
// 		}
// 		WriteModeShapeToFile( m_nSltMode, m_pSltMode);
//*/
	return;
}

//  ��֯ mon Case
void CLfoAnaObj::FormMonCaseData()
{
	// 	int   ii, jj, kk;
	// 	
	// 	struct timeb time1,time2;
	// 	ftime( &time1);
	// 	ftime( &time2);
	// 
	// 	for( jj=0; jj<m_nLnProDat; jj++ )
	// 	{
	// 		ii = jj;
	// 		m_pCases[ii].m_nID	    = m_pLnProDat[jj].ln_id;
	// 		m_pCases[ii].m_nStatus  = 0;
	// 		m_pCases[ii].m_time     = 0;
	// 		m_pCases[ii].timeStart  = time1;
	// 		m_pCases[ii].timeEnd    = time2;
	// 		m_pCases[ii].m_curve.m_nPoints  = m_nSamples;
	// 		for( kk=0; kk<m_nSamples; kk++)
	// 		{
	// 			m_pCases[ii].m_curve.t_value[kk] = m_ProMatchtime[kk];
	// 			m_pCases[ii].m_curve.y_value[kk] = m_pLnProDat[jj].pwr[kk];
	// 		}
	// 		m_pCases[ii].m_curve.t_length = m_ProMatchtime[m_nSamples-1];
	// 	}
	// 	m_nCases = m_nLnProDat;
	// 
	// 	for( jj=0; jj<m_nGnProDat; jj++ )
	// 	{
	// 		if ( m_pGnProDat[jj].is_calc == 1 ) // 08-02-02
	// 		{
	// 			m_pCases[m_nCases].m_nID	  = m_pGnProDat[jj].gn_id;
	// 			m_pCases[m_nCases].m_nStatus  = 0;
	// 			m_pCases[m_nCases].m_time     = 0;
	// 			m_pCases[m_nCases].timeStart  = time1;
	// 			m_pCases[m_nCases].timeEnd    = time2;
	// 			m_pCases[m_nCases].m_curve.m_nPoints  = m_nSamples;
	// 			for(kk=0; kk<m_nSamples; kk++)
	// 			{
	// 				m_pCases[m_nCases].m_curve.t_value[kk] = m_ProMatchtime[kk];
	// 				m_pCases[m_nCases].m_curve.y_value[kk] = m_pGnProDat[jj].pwr[kk];
	// 			}
	// 			m_pCases[m_nCases].m_curve.t_length = m_ProMatchtime[m_nSamples-1];
	// 			
	// 			m_nCases      += 1;
	// 		}
	// 		if ( m_nCases >= para_nCaseMaxMem )
	// 		{
	// 			sprintf( m_LogString, "FormMonCaseData(): Monitor Case num[%d] Խ��[%d],���Զ���Case��", m_nCases, para_nCaseMaxMem ); 
	// 			MessShow( m_LogString );
	// 			break;
	// 		}
	// 	}
	// 
	//     for( ii=0; ii<m_nTrProDat; ii++ )
	//     {
	//         m_pCases[m_nCases].m_nID	    = m_pTrProDat[ii].tr_id;
	//         m_pCases[m_nCases].m_nStatus  = 0;
	//         m_pCases[m_nCases].m_time     = 0;
	//         m_pCases[m_nCases].timeStart  = time1;
	//         m_pCases[m_nCases].timeEnd    = time2;
	//         m_pCases[m_nCases].m_curve.m_nPoints  = m_nSamples;
	//         for( kk=0; kk<m_nSamples; kk++)
	//         {
	//             m_pCases[m_nCases].m_curve.t_value[kk] = m_ProMatchtime[kk];
	//             m_pCases[m_nCases].m_curve.y_value[kk] = m_pTrProDat[ii].pwr[kk];
	//         }
	//         m_pCases[m_nCases].m_curve.t_length = m_ProMatchtime[m_nSamples-1];
	//         m_nCases = m_nCases+1;
	//         if ( m_nCases >= para_nCaseMaxMem )
	//         {
	//             sprintf( m_LogString, "FormMonCaseData(): Monitor Case num[%d] Խ��[%d],���Զ���Case��", m_nCases, para_nCaseMaxMem ); 
	//             MessShow( m_LogString );
	//             break;
	// 		}
	// 	}
	// 	sprintf( m_LogString, "FormMonCaseData(): Monitor Case num[%d], Ln[%d], Gn[%d], Tr[%d]��", m_nCases, m_nLnProDat, m_nCases-m_nLnProDat-m_nTrProDat, m_nTrProDat ); 
	// 	MessShow( m_LogString );
}

//  ��֯ ana Case
void CLfoAnaObj::FormAnaCaseData()
{
	// 	int jj, kk;
	// 
	// 	struct timeb time1,time2;
	// 	ftime( &time1);
	// 	ftime( &time2);
	// 
	// 	m_nCases = 0;
	// 	for( jj=0; jj<m_nGnProDat; jj++ )
	// 	{
	// 		if ( m_pGnProDat[jj].is_calc == 1 && m_pGnProDat[jj].stat == MENU_LFO_DEV_STS_OK ) // 08-02-02
	// 		{
	// 			m_pCases[m_nCases].m_nID	    = m_pGnProDat[jj].gn_id;
	// 			m_pCases[m_nCases].m_nStatus  = 0;
	// 			m_pCases[m_nCases].m_time     = 0;
	// 			m_pCases[m_nCases].timeStart  = time1;
	// 			m_pCases[m_nCases].timeEnd    = time2;
	// 			m_pCases[m_nCases].m_curve.m_nPoints  = m_nSamples;
	// 			for( kk=0; kk<m_nSamples; kk++)
	// 			{
	// 				m_pCases[m_nCases].m_curve.t_value[kk] = m_ProMatchtime[kk];
	// 				m_pCases[m_nCases].m_curve.y_value[kk] = m_pGnProDat[jj].ang[kk];
	// 			}
	// 			m_pCases[m_nCases].m_curve.t_length = m_ProMatchtime[m_nSamples-1];
	// 
	// 			m_nCases += 1;
	// 		}
	// 	}
	// 
	// 	for( jj=0; jj<m_nFcProDat; jj++ )
	// 	{
	// 		m_pCases[m_nCases].m_nID	  = m_pFcProDat[jj].fac_id;
	// 		m_pCases[m_nCases].m_nStatus  = 0;
	// 		m_pCases[m_nCases].m_time     = 0;
	// 		m_pCases[m_nCases].timeStart  = time1;
	// 		m_pCases[m_nCases].timeEnd    = time2;
	// 		m_pCases[m_nCases].m_curve.m_nPoints  = m_nSamples;
	// 		for(kk=0; kk<m_nSamples; kk++)
	// 		{
	// 			m_pCases[m_nCases].m_curve.t_value[kk] = m_ProMatchtime[kk];
	// 			m_pCases[m_nCases].m_curve.y_value[kk] = m_pFcProDat[jj].u1a[kk];
	// 		}
	// 		m_pCases[m_nCases].m_curve.t_length = m_ProMatchtime[m_nSamples-1];
	// 		m_nCases += 1;
	// 	}
	// 
	// 	sprintf( m_LogString, "FormAnaCaseData():  Form Analysis Case[%d], Fc[%d], Gn[%d] ", m_nCases, m_nFcProDat, m_nCases-m_nFcProDat ); 
	// 	MessShow( m_LogString );
}

// eeac����Ԥ����
void CLfoAnaObj::WAMAPDataHandling()
{

	return;
}

// eeac��Ⱥ
void CLfoAnaObj::EEACIdentifyCluster()
{

	return;
}

// 	m_nAllObjMode = 0 ;
// 	memset((void *)m_pAllObjMode, 0, sizeof(MODE_PART_OBJ) * para_nObjMaxMode );
// 	m_nMonMode      = 0;
// 	memset((void *)m_pMonMode, 0, sizeof(PRONY_ANAL_RSLT) * para_nCaseMaxMem );
// ģʽ�����Ϣ����� m_nAllObjMode  m_pAllObjMode ��
void CLfoAnaObj::ModeResultHandling(float damp_lim)
{
	int ii, jj, kk;
	int dev_type = -1;

	m_nMonMode = 0;
	m_nAllObjMode = 0;
	int tmptableno;
	for (ii = 0; ii < m_nCases; ii++)
	{
		tmptableno = PODB_GetTabelNoByDevId(m_pCases[ii].m_nID);
		if (tmptableno == GN_DEVICE_NO_TAB)
		{
			dev_type = MENU_LFO_DEV_GEN;
			m_pMonMode[m_nMonMode].obj_id = m_pCases[ii].m_nID;
			// ��ȡ�豸����
			for (jj = 0; jj < m_nGnNum; jj++)
			{
				if (m_pMonMode[m_nMonMode].obj_id == m_pGnInf[jj].gn_id)
				{
					sprintf(m_pMonMode[m_nMonMode].obj_name, "%s", m_pGnInf[jj].gn_name);
					break;
				}
			}
		}
		else if (tmptableno == ACLN_DEVICE_NO_TAB || tmptableno == ACLN_DOT_NO_TAB)
		{
			dev_type = MENU_LFO_DEV_LINE;
			m_pMonMode[m_nMonMode].obj_id = m_pCases[ii].m_nID;
			// ��ȡ�豸����
			for (jj = 0; jj < m_nLnNum; jj++)
			{
				if (m_pMonMode[m_nMonMode].obj_id == m_pLnInf[jj].acln_id)
				{
					sprintf(m_pMonMode[m_nMonMode].obj_name, "%s", m_pLnInf[jj].acln_name);
					break;
				}
			}
		}
		else if (tmptableno == TR_DEVICE_NO_TAB || tmptableno == TRWD_DEVICE_NO_TAB)
		{
			dev_type = MENU_LFO_DEV_TR;
			m_pMonMode[m_nMonMode].obj_id = m_pCases[ii].m_nID;
			// ��ȡ�豸����
			for (jj = 0; jj < m_nTrNum; jj++)
			{
				if (m_pMonMode[m_nMonMode].obj_id == m_pTrInf[jj].tr_id)
				{
					sprintf(m_pMonMode[m_nMonMode].obj_name, "%s", m_pTrInf[jj].tr_name);
					break;
				}
			}
		}
		else
			continue;


		m_pMonMode[m_nMonMode].mode_num = m_pCases[ii].m_result.mode_num;

		for (jj = 0; jj < m_pMonMode[m_nMonMode].mode_num; jj++)
		{
			for (kk = 0; kk < 7; kk++)
			{
				m_pMonMode[m_nMonMode].mode[jj][kk] = m_pCases[ii].m_result.mode[jj][kk];
			}
			if (m_pMonMode[m_nMonMode].mode[jj][0] <= damp_lim)
			{
				m_pAllObjMode[m_nAllObjMode].obj_id = m_pMonMode[m_nMonMode].obj_id;
				sprintf(m_pAllObjMode[m_nAllObjMode].obj_name, "%s", m_pMonMode[m_nMonMode].obj_name);
				m_pAllObjMode[m_nAllObjMode].type = dev_type;
				m_pAllObjMode[m_nAllObjMode].damp = m_pMonMode[m_nMonMode].mode[jj][0];
				m_pAllObjMode[m_nAllObjMode].freq = m_pMonMode[m_nMonMode].mode[jj][1];
				m_pAllObjMode[m_nAllObjMode].ampl = m_pMonMode[m_nMonMode].mode[jj][2];
				m_pAllObjMode[m_nAllObjMode].phas = m_pMonMode[m_nMonMode].mode[jj][3];
				m_pAllObjMode[m_nAllObjMode].real = m_pMonMode[m_nMonMode].mode[jj][4]; // ʵ��(˥������)
				m_pAllObjMode[m_nAllObjMode].imag = m_pMonMode[m_nMonMode].mode[jj][5]; // �鲿
				m_pAllObjMode[m_nAllObjMode].engy = m_pMonMode[m_nMonMode].mode[jj][6]; // engy
				m_nAllObjMode += 1;
			}
		}

		m_nMonMode += 1;
	}
	return;
}

// ��ģʽ��ȡ
// ��ȡ����ģʽ,���m_pLnMon,m_pGnMon,m_pTrMon
void CLfoAnaObj::ExtractLeadMode()
{
	int       i, j;
	int       leadmodeSub = 0;
	int tmptableno;
	for (i = 0; i < m_nMonMode; i++)
	{
		tmptableno = PODB_GetTabelNoByDevId(m_pMonMode[i].obj_id);
		if (tmptableno == GN_DEVICE_NO_TAB)
		{
			for (j = 0; j < m_nGnNum; j++)
			{
				if (m_pMonMode[i].obj_id == m_pGnMon[j].gn_id)
				{
					// 					m_pGnMon[j].lead_damp = m_pMonMode[i].mode[leadmodeSub][0];
					m_pGnMon[j].lead_freq = m_pMonMode[i].mode[leadmodeSub][1];
					m_pGnMon[j].lead_ampl = m_pMonMode[i].mode[leadmodeSub][2];
					m_pGnMon[j].lead_phas = m_pMonMode[i].mode[leadmodeSub][3];
					// 					m_pGnMon[j].lead_real = m_pMonMode[i].mode[leadmodeSub][4];
					// 					m_pGnMon[j].lead_imag = m_pMonMode[i].mode[leadmodeSub][5];
					// 					m_pGnMon[j].lead_engy = m_pMonMode[i].mode[leadmodeSub][6];
										//m_pGnMon[j].lead_fail = m_pMonMode[i].mode[leadmodeSub][3];
					break;
				}
			}
		}
		else if (tmptableno == ACLN_DEVICE_NO_TAB || tmptableno == ACLN_DOT_NO_TAB)
		{
			for (j = 0; j < m_nLnNum; j++)
			{
				if (m_pMonMode[i].obj_id == m_pLnMon[j].ln_id)
				{
					// 					m_pLnMon[j].lead_damp = m_pMonMode[i].mode[leadmodeSub][0];
					m_pLnMon[j].lead_freq = m_pMonMode[i].mode[leadmodeSub][1];
					m_pLnMon[j].lead_ampl = m_pMonMode[i].mode[leadmodeSub][2];
					m_pLnMon[j].lead_phas = m_pMonMode[i].mode[leadmodeSub][3];
					// 					m_pLnMon[j].lead_real = m_pMonMode[i].mode[leadmodeSub][4];
					// 					m_pLnMon[j].lead_imag = m_pMonMode[i].mode[leadmodeSub][5];
					// 					m_pLnMon[j].lead_engy = m_pMonMode[i].mode[leadmodeSub][6];
										//m_pLnMon[j].lead_fail = m_pMonMode[i].mode[leadmodeSub][3];
					break;
				}
			}
		}
		else if (tmptableno == TR_DEVICE_NO_TAB || tmptableno == TRWD_DEVICE_NO_TAB)
		{
			for (j = 0; j < m_nTrNum; j++)
			{
				if (m_pMonMode[i].obj_id == m_pTrMon[j].tr_id)
				{
					//                     m_pTrMon[j].lead_damp = m_pMonMode[i].mode[leadmodeSub][0];
					m_pTrMon[j].lead_freq = m_pMonMode[i].mode[leadmodeSub][1];
					m_pTrMon[j].lead_ampl = m_pMonMode[i].mode[leadmodeSub][2];
					m_pTrMon[j].lead_phas = m_pMonMode[i].mode[leadmodeSub][3];
					//                     m_pTrMon[j].lead_real = m_pMonMode[i].mode[leadmodeSub][4];
					//                     m_pTrMon[j].lead_imag = m_pMonMode[i].mode[leadmodeSub][5];
					//                     m_pTrMon[j].lead_engy = m_pMonMode[i].mode[leadmodeSub][6];
										//m_pTrMon[j].lead_fail = m_pMonMode[i].mode[leadmodeSub][3];
					break;
				}
			}
		}
		else
		{
			sprintf(m_LogString, "�豸���ʹ���["INT_REC_FMT"]", INT_REC_VAL(m_pMonMode[i].obj_id));
			MessShow(m_LogString);
			continue;
		}
	}

	return;
}

// �жϼ��Ӷ��������
// �жϼ�������� ���� m_pLnMon��m_pLnOsc�� m_pGnMon��m_pGnOsc��
// void CLfoAnaObj::EstimateObjOscillate( )
// {
// 	int            ii, jj, kk, nn ;
// 	int            checkTimeNum  = 0;
// 	int            checkDataNum  = 0;
// 	float          temp_ampl_max = 0;
// 	float          temp_ampl_min = 0;
// 	int            times         = 0;
//     float          ampl_aver_new = 0;    // 2008-05-08 by zhw
//     float          ampl_aver_old = 0;
//     int            countNum      = 0;
// 
// 	//int            leadmodeSub  = 0;
// 	//float          leadmodeAmpl = 0.0;
// 
// 	LFO_LEAD_MODE  temp_leadMode;       //����ģʽ
// 
// 	//�������л�ʱ��ȡ��������
// 	if(m_CurGpsTime - m_OldGpsTime > m_LfoPara.htime_max) 
// 	{
// 		//�����ǰ�õ���ϵͳ״̬
// 		memset(&m_SysMon_Old,0,sizeof(LFO_SYS_MON));
// 		m_SysMon_Old.stat = MENU_LFO_DEV_STS_SCT;
// 		sprintf(m_LogString, "EstimateObjOscillate(): m_CurGpsTime[%d]-m_OldGpsTime[%d],�����ǰ�õ���ϵͳ״̬��",m_CurGpsTime,m_OldGpsTime);
// 		MessShow(m_LogString);
// 	}
// 
// 	float dtime   = m_ProMatchtime[2] - m_ProMatchtime[1];
// 	for(ii=0; ii<m_nGnNum; ii++)
// 	{
// 		if ( m_pGnMon[ii].stat != MENU_LFO_DEV_STS_OK )
// 		{
// 			memset((void*)&m_pGnOsc[ii], 0, sizeof(OSCILLAT_FLAG) );
// 			m_pGnOsc[ii].obj_id = m_pGnInf[ii].gn_id;
// 			
// 			continue;
// 		}
// 		m_pGnOsc[ii].begin_ampl = m_pGnCtl[ii].lfo_gn_swn;
// 		m_pGnOsc[ii].end_ampl = m_pGnCtl[ii].lfo_gn_ewn;
// 		sprintf(m_LogString,"�����豸�����ֵ[%f]+[%f]",m_pGnOsc[ii].begin_ampl,m_pGnOsc[ii].end_ampl);
// 		MessShow(m_LogString);
// 
// 		memset((void*)&temp_leadMode, 0, sizeof(LFO_LEAD_MODE) ); 
// 
// 		temp_leadMode.obj_id    = m_pGnMon[ii].gn_id;
// 		temp_leadMode.amplitude = m_pGnMon[ii].lead_ampl;
// 		temp_leadMode.frequency = m_pGnMon[ii].lead_freq;
// 		temp_leadMode.damping   = m_pGnMon[ii].lead_damp;
// 		temp_leadMode.phase     = m_pGnMon[ii].lead_phas;
// 		temp_leadMode.aver_ampl      = m_pGnMon[ii].max_ampl;
// 		temp_leadMode.aver_ampl_rat  = m_pGnMon[ii].max_ampl_rat;
// 		temp_leadMode.aver_ampl2     = m_pGnMon[ii].max_ampl;
// 		temp_leadMode.aver_ampl_rat2 = m_pGnMon[ii].max_ampl_rat;
// 		// �޷���Ҫ���ģʽʱ���������Ϊ���ֵ100 
// 		if(m_pGnMon[ii].lead_ampl < ZEROVALUE)
// 		{
// //			temp_leadMode.damping = 100.0;
// 			temp_leadMode.damping = m_LfoPara.max_damp;
// 		}
// 
// 		//��������Ƶ�ʣ����������е�ԭʼƽ���������Ϊ��ֵ���������ԭ�з�ֵ���ܿ�б���߶Է�ֵ�����Ӱ�죩
// 		checkTimeNum  = 1;
// 		checkDataNum  = 0;		
// 		if( temp_leadMode.frequency > 0.001 && dtime>0.0001)
// 			checkDataNum = (int)(1.2*(1.0/temp_leadMode.frequency/dtime + 1)); // 08-02-01 2*(int)
// 		else  // 2008-01-22 lsf ������
// 			checkDataNum = (int)(3*m_nSamples/4); 
// 		if (checkDataNum > m_nSamples ) checkDataNum = m_nSamples;
// 		
// 		checkTimeNum  = (int)(m_nSamples/checkDataNum);
// 
// 		//if( temp_leadMode.frequency > m_LfoPara.freq_min )//// 2008-01-22 lsf ������
// 		{
// 			for(jj=0; jj<m_nGnProDat; jj++)
// 			{
// 				if(temp_leadMode.obj_id == m_pGnProDat[jj].gn_id)
// 				{
// 					temp_leadMode.aver_ampl = 0;
//                     ampl_aver_new = 0;
//                     ampl_aver_old = 0;
//                     countNum      = 0;
// 					for ( nn=0; nn<checkTimeNum; nn++ )
// 					{
// 						temp_ampl_max = m_pGnProDat[jj].pwr[nn*checkDataNum];
// 						temp_ampl_min = temp_ampl_max; 
// 						for( kk=nn*checkDataNum; kk<(nn+1)*checkDataNum; kk++)
// 						{
// 							if(temp_ampl_max < m_pGnProDat[jj].pwr[kk]) temp_ampl_max = m_pGnProDat[jj].pwr[kk];
// 							if(temp_ampl_min > m_pGnProDat[jj].pwr[kk]) temp_ampl_min = m_pGnProDat[jj].pwr[kk];
// 						}
// 
//                         /*ampl_aver_new = (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);    // 2008-05-08 by zhw at nj
//                         if(nn == 0)
//                         {
//                             countNum += 1;
//                             temp_leadMode.aver_ampl += ampl_aver_new;
//                         }
//                         else if( fabs(ampl_aver_new - ampl_aver_old) <= (m_LfoPara.fjud_wnap_per*min(ampl_aver_new,ampl_aver_old)/100) )
//                         {
//                             countNum += 1;
//                             temp_leadMode.aver_ampl += ampl_aver_new;
//                         }
//                         ampl_aver_old = ampl_aver_new;*/
// 
//                         temp_leadMode.aver_ampl += (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);
// 					}
// 					
// 					temp_leadMode.aver_ampl = temp_leadMode.aver_ampl/checkTimeNum;
//                     //temp_leadMode.aver_ampl = temp_leadMode.aver_ampl/countNum;
// 					break;
// 
// //					temp_ampl_max = m_pGnProDat[jj].pwr[m_nSamples-checkDataNum];
// //					temp_ampl_min = temp_ampl_max; 
// //					for( kk=(m_nSamples-checkDataNum+1); kk<m_nSamples; kk++)
// //					{
// //						if(temp_ampl_max < m_pGnProDat[jj].pwr[kk]) temp_ampl_max = m_pGnProDat[jj].pwr[kk];
// //						if(temp_ampl_min > m_pGnProDat[jj].pwr[kk]) temp_ampl_min = m_pGnProDat[jj].pwr[kk];
// //					}
// //					
// //					temp_leadMode.aver_ampl = (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);
// //					break;
// 				}
// 			}
// 		}
// 
//         // 2008-10-22 for nw ���ֵ
//         m_pGnMon[ii].max_min_dif = temp_leadMode.aver_ampl;
// 
// 		//  judge lfo
// 		//�������л�ʱ��ȡ��������
// 		//if(m_CurGpsTime - m_OldGpsTime > m_LfoPara.htime_max) 
// 		if ( m_pGnOsc[ii].LM_damp < 0.005 && m_pGnOsc[ii].LM_frequency < 0.005 )
// 		{
// 			times = 1; // 2008-01-22 
// 			EstimateSingleObjState( &m_pGnOsc[ii], &temp_leadMode, times, m_LfoPara.sm_md_freq );
// 		}
// 		else
// 		{
// 			times = 2; // 2008-01-22 ����бֱ��
// 			EstimateSingleObjState( &m_pGnOsc[ii], &temp_leadMode, times, m_LfoPara.sm_md_freq );
// 		}
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		{  //  stat
// 			if(m_pGnOsc[ii].lfo_warn == 1 )
// 			{
// 				//m_pGnOsc[ii].state = MENU_LFO_DEV_STS_WRN;                       //LFO�澯
// 				// 2008-01-22 lsf 
// 				m_pGnMon[ii].stat = MENU_LFO_DEV_STS_WRN;                   //LFO�澯
// 			}
// 			else
// 			{
// 				if(m_pGnOsc[ii].lackdamp_warn == 1)
// 				{
// 					//m_pGnOsc[ii].state = MENU_LFO_DEV_STS_WRN;                   //LFO�澯(���᲻��)
// 					// 2008-01-22 lsf 
// 					//m_pGnMon[ii].stat = MENU_LFO_DEV_STS_WRN;                   //LFO�澯(���᲻��)
//                     m_pGnMon[ii].stat = MENU_LFO_DEV_STS_SCT;                     //  Ƿ���᲻�澯  // 2008-08-20
// 				}
// 				else
// 				{
// 					if(m_pGnOsc[ii].damp_norm != 1)
// 					{
// 						if(m_pGnOsc[ii].lfo_forewarn == 1)
// 						{
// 							//m_pGnOsc[ii].state = MENU_LFO_DEV_STS_FWN;       //LFOԤ��
// 							// 2008-01-22 lsf 
// 							m_pGnMon[ii].stat = MENU_LFO_DEV_STS_FWN;       //LFOԤ��
// 						}
// 						else
// 						{
// 							//m_pGnOsc[ii].state = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 							// 2008-01-22 lsf 
// 							m_pGnMon[ii].stat = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 						}
// 					}
// 					else
// 					{
// 						if(m_pGnOsc[ii].state == 0 )    //first time
// 						{
// 							//m_pGnOsc[ii].state = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 							// 2008-01-22 lsf 
// 							m_pGnMon[ii].stat = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 						}
// 						else
// 						{
// 							m_pGnMon[ii].stat = m_pGnOsc[ii].state;
// 						}
// 						
// 					}
// 				}
// 			}
// 		}
// 		// ���������ظ澯�ж� (�ڸ澯ǰ�ж�)
// 		//m_pGnMon[ii].stat     = m_pGnOsc[ii].state;
// 		//m_pGnMon[ii].stat_val = m_pGnOsc[ii].state;
// 		// 2008-01-22 lsf 
// 		m_pGnMon[ii].stat_val = m_pGnMon[ii].stat;
// 		m_pGnOsc[ii].state    = m_pGnMon[ii].stat; // ��¼ǰһ��״̬
// 		// ��Ϣ
// 		sprintf(m_LogString, "gn_id="INT_REC_FMT", gn_name=%s, LFO_Ampl_Fir=%f, LFO_Ampl=%f, aver_ampl2=%f, aver_ampl=%f, stat[%d]",
// 			temp_leadMode.obj_id, m_pGnInf[ii].gn_name, m_LfoPara.gn_wn_ampl*m_LfoPara.fjud_wnap_per/100, m_LfoPara.gn_wn_ampl, temp_leadMode.aver_ampl2,temp_leadMode.aver_ampl,m_pGnMon[ii].stat);
// 		MessShow(m_LogString);
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	for(ii=0; ii<m_nLnNum; ii++)
// 	{
// 		if ( m_pLnMon[ii].stat != MENU_LFO_DEV_STS_OK )
// 		{
// 			memset((void*)&m_pLnOsc[ii], 0, sizeof(OSCILLAT_FLAG) ); 
// 			m_pLnOsc[ii].obj_id = m_pLnInf[ii].acln_id;
// 			
// 			continue;
// 		}
// 		m_pLnOsc[ii].begin_ampl = m_pLnCtl[ii].lfo_ln_swn;
// 		m_pLnOsc[ii].end_ampl = m_pLnCtl[ii].lfo_ln_ewn;
// 		sprintf(m_LogString,"��·�豸�����ֵ[%f]+[%f]",m_pLnOsc[ii].begin_ampl,m_pLnOsc[ii].end_ampl);
// 		MessShow(m_LogString);
// 		memset((void*)&temp_leadMode, 0, sizeof(LFO_LEAD_MODE) ); 
// 
// 		temp_leadMode.obj_id    = m_pLnMon[ii].acln_id;
// 		temp_leadMode.amplitude = m_pLnMon[ii].lead_ampl;
// 		temp_leadMode.frequency = m_pLnMon[ii].lead_freq;
// 		temp_leadMode.damping   = m_pLnMon[ii].lead_damp;
// 		temp_leadMode.phase     = m_pLnMon[ii].lead_phas;
// 		temp_leadMode.vbase		= m_pLnMon[ii].vbase;
// 		temp_leadMode.aver_ampl      = m_pLnMon[ii].max_ampl;
// 		temp_leadMode.aver_ampl_rat  = m_pLnMon[ii].max_ampl_rat;
// 		temp_leadMode.aver_ampl2     = m_pLnMon[ii].max_ampl;
// 		temp_leadMode.aver_ampl_rat2 = m_pLnMon[ii].max_ampl_rat;
// 		// �޷���Ҫ���ģʽʱ���������Ϊ���ֵ100 
// 		if(m_pLnMon[ii].lead_ampl < ZEROVALUE)
// 		{
// 			temp_leadMode.damping = 100;
// 		}
// 
// 		//��������Ƶ�ʣ����������е�ԭʼƽ���������Ϊ��ֵ���������ԭ�з�ֵ���ܿ�б���߶Է�ֵ�����Ӱ�죩
// 		checkTimeNum  = 1;
// 		checkDataNum  = 0;		
// 		if( temp_leadMode.frequency > 0.001 && dtime>0.0001)  
// 			checkDataNum = (int)(1.2*(1.0/temp_leadMode.frequency/dtime + 1)); // 08-02-01 2*(int)
// 		else  // 2008-01-22 lsf ������
// 			checkDataNum = (int)(3*m_nSamples/4); 
// 		if (checkDataNum > m_nSamples ) checkDataNum = m_nSamples-1;
// 		
// 		checkTimeNum  = (int)(m_nSamples/checkDataNum);
// 
// 		//if( temp_leadMode.frequency > m_LfoPara.freq_min )//
// 		{
// 			for(jj=0; jj<m_nLnProDat; jj++)
// 			{
// 				if(temp_leadMode.obj_id == m_pLnProDat[jj].acln_id)
// 				{
// 					temp_leadMode.aver_ampl = 0;
//                     ampl_aver_new = 0;
//                     ampl_aver_old = 0;
//                     countNum      = 0;
// 					for ( nn=0; nn<checkTimeNum; nn++ )
// 					{
// 						temp_ampl_max = m_pLnProDat[jj].pwr[nn*checkDataNum];
// 						temp_ampl_min = temp_ampl_max; 
// 						for( kk=nn*checkDataNum; kk<(nn+1)*checkDataNum; kk++)
// 						{
// 							if(temp_ampl_max < m_pLnProDat[jj].pwr[kk]) temp_ampl_max = m_pLnProDat[jj].pwr[kk];
// 							if(temp_ampl_min > m_pLnProDat[jj].pwr[kk]) temp_ampl_min = m_pLnProDat[jj].pwr[kk];
// 						}
// 
//                         /*ampl_aver_new = (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);    // 2008-05-08 by zhw at nj
//                         if(nn == 0)
//                         {
//                             countNum += 1;
//                             temp_leadMode.aver_ampl += ampl_aver_new;
//                         }
//                         else if( fabs(ampl_aver_new - ampl_aver_old) <= (m_LfoPara.fjud_wnap_per*min(ampl_aver_new,ampl_aver_old)/100) )
//                         {
//                             countNum += 1;
//                             temp_leadMode.aver_ampl += ampl_aver_new;
//                         }
//                         ampl_aver_old = ampl_aver_new;*/
// 
// 						temp_leadMode.aver_ampl += (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);
// 					}
// 
// 					temp_leadMode.aver_ampl = temp_leadMode.aver_ampl/checkTimeNum;
//                     //temp_leadMode.aver_ampl = temp_leadMode.aver_ampl/countNum;
// 					break;
// 					
// //					temp_ampl_max = m_pLnProDat[jj].pwr[m_nSamples-checkDataNum];
// //					temp_ampl_min = temp_ampl_max; 
// //					for( kk=(m_nSamples-checkDataNum+1); kk<m_nSamples; kk++)
// //					{
// //						if(temp_ampl_max < m_pLnProDat[jj].pwr[kk]) temp_ampl_max = m_pLnProDat[jj].pwr[kk];
// //						if(temp_ampl_min > m_pLnProDat[jj].pwr[kk]) temp_ampl_min = m_pLnProDat[jj].pwr[kk];
// //					}
// //
// //					temp_leadMode.aver_ampl = (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);
// //					break;
// 				}
// 			}
// 		}
// 
//         // 2008-10-22 for nw ���ֵ
//         m_pLnMon[ii].max_min_dif = temp_leadMode.aver_ampl;
// 		//  judge lfo
// 		//if(m_CurGpsTime - m_OldGpsTime > m_LfoPara.htime_max) 
// 		if ( m_pLnOsc[ii].LM_damp < 0.005 && m_pLnOsc[ii].LM_frequency < 0.005 )
// 		{
// 			times = 1;
// 			EstimateSingleObjState( &m_pLnOsc[ii], &temp_leadMode, times, m_LfoPara.sm_md_freq );
// 		}
// 		else
// 		{
// 			times = 2;
// 			EstimateSingleObjState( &m_pLnOsc[ii], &temp_leadMode, times, m_LfoPara.sm_md_freq );
// 		}
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		{  //  stat
// 			if(m_pLnOsc[ii].lfo_warn == 1 )
// 			{
// 				//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_WRN;                       //LFO�澯
// 				// 2008-01-22 lsf 
// 				m_pLnMon[ii].stat = MENU_LFO_DEV_STS_WRN;                       //LFO�澯
// 			}
// 			else
// 			{
// 				if(m_pLnOsc[ii].lackdamp_warn == 1)
// 				{
// 					//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_WRN;                   //LFO�澯(���᲻��)
// 					// 2008-01-22 lsf 
// 					//m_pLnMon[ii].stat = MENU_LFO_DEV_STS_WRN;                   //LFO�澯(���᲻��)
//                       m_pLnMon[ii].stat = MENU_LFO_DEV_STS_SCT;                   //  Ƿ���᲻�澯 2008-08-20
// 				}
// 				else
// 				{
// 					if(m_pLnOsc[ii].damp_norm != 1)
// 					{
// 						if(m_pLnOsc[ii].lfo_forewarn == 1)
// 						{
// 							//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_FWN;       //LFOԤ��
// 							// 2008-01-22 lsf 
// 							m_pLnMon[ii].stat = MENU_LFO_DEV_STS_FWN;       //LFOԤ��
// 						}
// 						else
// 						{
// 							//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 							// 2008-01-22 lsf 
// 							m_pLnMon[ii].stat = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 						}
// 					}
// 					else
// 					{
// 						if(m_pLnOsc[ii].state == 0 )    //first time
// 						{
// 							//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 							// 2008-01-22 lsf 
// 							m_pLnMon[ii].stat = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 						}
// 						else
// 						{
// 							// 2008-01-22 lsf 
// 							m_pLnMon[ii].stat = m_pLnOsc[ii].state;       //ǰһ��״̬
// 						}
// 						
// 					}
// 				}
// 			}
// 		}
// 		// ���������ظ澯�ж� (�ڸ澯ǰ�ж�)
// 		
// 		//m_pLnMon[ii].stat     = m_pLnOsc[ii].state;
// 		//m_pLnMon[ii].stat_val = m_pLnOsc[ii].state;
// 		// 2008-01-22 lsf 
// 		m_pLnMon[ii].stat_val = m_pLnMon[ii].stat;
// 		m_pLnOsc[ii].state    = m_pLnMon[ii].stat; // ��¼ǰһ��״̬
// 		// ��Ϣ
// 		sprintf(m_LogString, "ln_id="INT_REC_FMT", ln_name=%s, LFO_Ampl_Fir=%f, LFO_Ampl=%f, aver_ampl2=%f, aver_ampl=%f,stat[%d]",
// 			temp_leadMode.obj_id, m_pLnInf[ii].acln_name,m_LfoPara.ln_wn_ampl*m_LfoPara.fjud_wnap_per/100, m_LfoPara.ln_wn_ampl, temp_leadMode.aver_ampl2,temp_leadMode.aver_ampl,m_pLnMon[ii].stat);
// 		MessShow(m_LogString);
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
//     // ��ѹ��
// 	for(ii=0; ii<m_nTrNum; ii++)
// 	{
// 		if ( m_pTrMon[ii].stat != MENU_LFO_DEV_STS_OK )
// 		{
// 			memset((void*)&m_pTrOsc[ii], 0, sizeof(OSCILLAT_FLAG) ); 
// 			m_pTrOsc[ii].obj_id = m_pTrInf[ii].tr_id;
// 			continue;
// 		}
// 		m_pTrOsc[ii].begin_ampl = m_pTrCtl[ii].lfo_tr_swn;
// 		m_pTrOsc[ii].end_ampl = m_pTrCtl[ii].lfo_tr_ewn;
// 		sprintf(m_LogString,"��ѹ���豸�����ֵ[%f]+[%f]",m_pTrOsc[ii].begin_ampl,m_pTrOsc[ii].end_ampl);
// 		MessShow(m_LogString);
// 		memset((void*)&temp_leadMode, 0, sizeof(LFO_LEAD_MODE) ); 
// 
// 		temp_leadMode.obj_id    = m_pTrMon[ii].tr_id;
// 		temp_leadMode.amplitude = m_pTrMon[ii].lead_ampl;
// 		temp_leadMode.frequency = m_pTrMon[ii].lead_freq;
// 		temp_leadMode.damping   = m_pTrMon[ii].lead_damp;
// 		temp_leadMode.phase     = m_pTrMon[ii].lead_phas;
// 		if (m_pTrMon[ii].slct_wind_id == m_pTrMon[ii].high_wind_id)
// 		{
// 			m_pTrMon[ii].vbase = m_pTrMon[ii].vhigh;
// 		} 
// 		else if (m_pTrMon[ii].slct_wind_id == m_pTrMon[ii].mid_wind_id)
// 		{
// 			m_pTrMon[ii].vbase = m_pTrMon[ii].vmid;
// 		}
// 		else
// 		{
// 			m_pTrMon[ii].vbase = m_pTrMon[ii].vlow;
// 		}
// 		temp_leadMode.vbase	         = m_pTrMon[ii].vbase;
// 		temp_leadMode.aver_ampl      = m_pTrMon[ii].max_ampl;
// 		temp_leadMode.aver_ampl_rat  = m_pTrMon[ii].max_ampl_rat;
// 		temp_leadMode.aver_ampl2     = m_pTrMon[ii].max_ampl;
// 		temp_leadMode.aver_ampl_rat2 = m_pTrMon[ii].max_ampl_rat;
// 		// �޷���Ҫ���ģʽʱ���������Ϊ���ֵ100 
// 		if(m_pTrMon[ii].lead_ampl < ZEROVALUE)
// 		{
// 			temp_leadMode.damping = 100;
// 		}
// 
// 		//��������Ƶ�ʣ����������е�ԭʼƽ���������Ϊ��ֵ���������ԭ�з�ֵ���ܿ�б���߶Է�ֵ�����Ӱ�죩
// 		checkTimeNum  = 1;
// 		checkDataNum  = 0;		
// 		if( temp_leadMode.frequency > 0.001 && dtime>0.0001)  
// 			checkDataNum = (int)(1.2*(1.0/temp_leadMode.frequency/dtime + 1)); // 08-02-01 2*(int)
// 		else  // 2008-01-22 lsf ������
// 			checkDataNum = (int)(3*m_nSamples/4); 
// 		if (checkDataNum > m_nSamples ) checkDataNum = m_nSamples-1;
// 		
// 		checkTimeNum  = (int)(m_nSamples/checkDataNum);
// 
// 		//if( temp_leadMode.frequency > m_LfoPara.freq_min )//
// 		{
// 			for(jj=0; jj<m_nTrProDat; jj++)
// 			{
// 				if(temp_leadMode.obj_id == m_pTrProDat[jj].tr_id)
// 				{
// 					temp_leadMode.aver_ampl = 0;
//                     ampl_aver_new = 0;
//                     ampl_aver_old = 0;
//                     countNum      = 0;
// 					for ( nn=0; nn<checkTimeNum; nn++ )
// 					{
// 						temp_ampl_max = m_pTrProDat[jj].pwr[nn*checkDataNum];
// 						temp_ampl_min = temp_ampl_max; 
// 						for( kk=nn*checkDataNum; kk<(nn+1)*checkDataNum; kk++)
// 						{
// 							if(temp_ampl_max < m_pTrProDat[jj].pwr[kk]) temp_ampl_max = m_pTrProDat[jj].pwr[kk];
// 							if(temp_ampl_min > m_pTrProDat[jj].pwr[kk]) temp_ampl_min = m_pTrProDat[jj].pwr[kk];
// 						}
// 
//                         /*ampl_aver_new = (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);    // 2008-05-08 by zhw at nj
//                         if(nn == 0)
//                         {
//                             countNum += 1;
//                             temp_leadMode.aver_ampl += ampl_aver_new;
//                         }
//                         else if( fabs(ampl_aver_new - ampl_aver_old) <= (m_LfoPara.fjud_wnap_per*min(ampl_aver_new,ampl_aver_old)/100) )
//                         {
//                             countNum += 1;
//                             temp_leadMode.aver_ampl += ampl_aver_new;
//                         }
//                         ampl_aver_old = ampl_aver_new;*/
// 
// 						temp_leadMode.aver_ampl += (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);
// 					}
// 
// 					temp_leadMode.aver_ampl = temp_leadMode.aver_ampl/checkTimeNum;
//                     //temp_leadMode.aver_ampl = temp_leadMode.aver_ampl/countNum;
// 					break;
// 					
// //					temp_ampl_max = m_pLnProDat[jj].pwr[m_nSamples-checkDataNum];
// //					temp_ampl_min = temp_ampl_max; 
// //					for( kk=(m_nSamples-checkDataNum+1); kk<m_nSamples; kk++)
// //					{
// //						if(temp_ampl_max < m_pLnProDat[jj].pwr[kk]) temp_ampl_max = m_pLnProDat[jj].pwr[kk];
// //						if(temp_ampl_min > m_pLnProDat[jj].pwr[kk]) temp_ampl_min = m_pLnProDat[jj].pwr[kk];
// //					}
// //
// //					temp_leadMode.aver_ampl = (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);
// //					break;
// 				}
// 			}
// 		}
// 
//         // 2008-10-22 for nw ���ֵ
//         m_pTrMon[ii].max_min_dif = temp_leadMode.aver_ampl;
// 		//  judge lfo
// 		//if(m_CurGpsTime - m_OldGpsTime > m_LfoPara.htime_max) 
// 		if ( m_pTrOsc[ii].LM_damp < 0.005 && m_pTrOsc[ii].LM_frequency < 0.005 )
// 		{
// 			times = 1;
// 			EstimateSingleObjState( &m_pTrOsc[ii], &temp_leadMode, times, m_LfoPara.sm_md_freq );
// 		}
// 		else
// 		{
// 			times = 2;
// 			EstimateSingleObjState( &m_pTrOsc[ii], &temp_leadMode, times, m_LfoPara.sm_md_freq );
// 		}
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		{  //  stat
// 			if(m_pTrOsc[ii].lfo_warn == 1 )
// 			{
// 				//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_WRN;                       //LFO�澯
// 				// 2008-01-22 lsf 
// 				m_pTrMon[ii].stat = MENU_LFO_DEV_STS_WRN;                       //LFO�澯
// 			}
// 			else
// 			{
// 				if(m_pTrOsc[ii].lackdamp_warn == 1)
// 				{
// 					//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_WRN;                   //LFO�澯(���᲻��)
// 					// 2008-01-22 lsf 
// 					//m_pLnMon[ii].stat = MENU_LFO_DEV_STS_WRN;                   //LFO�澯(���᲻��)
//                       m_pTrMon[ii].stat = MENU_LFO_DEV_STS_SCT;                   //  Ƿ���᲻�澯 2008-08-20
// 				}
// 				else
// 				{
// 					if(m_pTrOsc[ii].damp_norm != 1)
// 					{
// 						if(m_pTrOsc[ii].lfo_forewarn == 1)
// 						{
// 							//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_FWN;       //LFOԤ��
// 							// 2008-01-22 lsf 
// 							m_pTrMon[ii].stat = MENU_LFO_DEV_STS_FWN;       //LFOԤ��
// 						}
// 						else
// 						{
// 							//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 							// 2008-01-22 lsf 
// 							m_pTrMon[ii].stat = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 						}
// 					}
// 					else
// 					{
// 						if(m_pTrOsc[ii].state == 0 )    //first time
// 						{
// 							//m_pLnOsc[ii].state = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 							// 2008-01-22 lsf 
// 							m_pTrMon[ii].stat = MENU_LFO_DEV_STS_SCT;       //��ȫ
// 						}
// 						else
// 						{
// 							// 2008-01-22 lsf 
// 							m_pTrMon[ii].stat = m_pTrOsc[ii].state;       //ǰһ��״̬
// 						}
// 						
// 					}
// 				}
// 			}
// 		}
// 		// ���������ظ澯�ж� (�ڸ澯ǰ�ж�)
// 		
// 		//m_pLnMon[ii].stat     = m_pLnOsc[ii].state;
// 		//m_pLnMon[ii].stat_val = m_pLnOsc[ii].state;
// 		// 2008-01-22 lsf 
// 		m_pTrMon[ii].stat_val = m_pTrMon[ii].stat;
// 		m_pTrOsc[ii].state    = m_pTrMon[ii].stat; // ��¼ǰһ��״̬
// 		// ��Ϣ
// 		sprintf(m_LogString, "tr_id="INT_REC_FMT", tr_name=%s, LFO_Ampl_Fir=%f, LFO_Ampl=%f, aver_ampl2=%f, aver_ampl=%f,stat[%d]",
// 			temp_leadMode.obj_id, m_pTrInf[ii].tr_name, m_LfoPara.tr_wn_ampl*m_LfoPara.fjud_wnap_per/100, m_LfoPara.tr_wn_ampl, temp_leadMode.aver_ampl2,temp_leadMode.aver_ampl,m_pTrMon[ii].stat);
// 		MessShow(m_LogString);
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	return ;
// }

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��·������Ϣ����ȽϺ��� stat�ɴ�С,max_ampl�ɴ�С
////////////////////////////////////////////////////////////////////////////////
extern "C" int LnMonCompareByStat(const void* p1, const void* p2)
{
	if (((SSO_LN_MON*)p1)->stat != ((SSO_LN_MON*)p2)->stat)
	{
		return ((SSO_LN_MON*)p1)->stat < ((SSO_LN_MON*)p2)->stat ? 1 : -1;
	}
	return ((SSO_LN_MON*)p1)->lead_ampl < ((SSO_LN_MON*)p2)->lead_ampl ? 1 : -1;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ���������Ϣ����ȽϺ��� stat�ɴ�С,max_ampl�ɴ�С
////////////////////////////////////////////////////////////////////////////////
extern "C" int GnMonCompareByStat(const void* p1, const void* p2)
{
	if (((SSO_GN_MON*)p1)->stat != ((SSO_GN_MON*)p2)->stat)
	{
		return ((SSO_GN_MON*)p1)->stat < ((SSO_GN_MON*)p2)->stat ? 1 : -1;
	}
	return ((SSO_GN_MON*)p1)->lead_ampl < ((SSO_GN_MON*)p2)->lead_ampl ? 1 : -1;
}
////////////////////////////////////////////////////////////////////////////////
//  �������� : ��ѹ��������Ϣ����ȽϺ��� stat�ɴ�С,max_ampl�ɴ�С
////////////////////////////////////////////////////////////////////////////////
extern "C" int TrMonCompareByStat(const void* p1, const void* p2)
{
	if (((SSO_TR_MON*)p1)->stat != ((SSO_TR_MON*)p2)->stat)
	{
		return ((SSO_TR_MON*)p1)->stat < ((SSO_TR_MON*)p2)->stat ? 1 : -1;
	}
	return ((SSO_TR_MON*)p1)->lead_ampl < ((SSO_TR_MON*)p2)->lead_ampl ? 1 : -1;
}
// ϵͳ������Ϣ
// �ж�ϵͳ�������ͳ������·�����飻����ģʽ���������豸��Ϣ����
// void CLfoAnaObj::EstimateSysOscillate( )
// {
// 	sprintf(m_LogString, "EstimateSysOscillate():�ж�ϵͳ�����!");
// 	MessShow(m_LogString);
// 
//     int   ii,jj, kk = 0;
// 	char  DevName[200];                         //
// 	INT_REC   temp_id   = 0;
// 	//char      dev_name[64];
// 	INT_REC   fac_id = 0;
// 	char fac_name[64];
// 	float temp_ampl = 0;
// 
// 	m_nLnWarn = 0;
// 	m_nGnWarn = 0;
//     m_nTrWarn = 0;
// 	memset((void*)m_pLnWarn, 0, para_nLnMaxMem * sizeof(LFO_LN_MON));
// 	memset((void*)m_pGnWarn, 0, para_nGnMaxMem * sizeof(LFO_GN_MON));
//     memset((void*)m_pTrWarn, 0, para_nTrMaxMem * sizeof(LFO_TR_MON));
// 	memset((void*)m_DevLnWarn, 0, para_nLnMaxMem * sizeof(LFO_LN_MON));
// 	memset((void*)m_DevGnWarn, 0, para_nGnMaxMem * sizeof(LFO_GN_MON));
//     memset((void*)m_DevTrWarn, 0, para_nTrMaxMem * sizeof(LFO_TR_MON));
// 	//  ͳ�����豸
// 	for ( ii=0; ii<m_nLnNum; ii++ )
// 	{
// 		memcpy( (void*)&m_pLnWarn[ii], (void*)&m_pLnMon[ii], sizeof(LFO_LN_MON) );
// 		if ( m_pLnMon[ii].stat > MENU_LFO_DEV_STS_SCT )
// 		{
// 			memcpy( (void*)&m_DevLnWarn[m_nLnWarn], (void*)&m_pLnMon[ii], sizeof(LFO_LN_MON) );
// 			m_nLnWarn += 1;
// 		}
// 	}
// 	if ( m_nLnNum > 1 )
// 	{
// 		qsort((void *)m_pLnWarn, m_nLnNum, sizeof(LFO_LN_MON), LnMonCompareByStat);
// 	}
// 	for ( ii=0; ii<m_nGnNum; ii++ )
// 	{
// 		memcpy( (void*)&m_pGnWarn[ii], (void*)&m_pGnMon[ii], sizeof(LFO_GN_MON) );
// 		if ( m_pGnMon[ii].stat > MENU_LFO_DEV_STS_SCT )
// 		{
// 			memcpy( (void*)&m_DevGnWarn[m_nGnWarn], (void*)&m_pGnMon[ii], sizeof(LFO_GN_MON) );
// 			m_nGnWarn += 1;
// 		}
// 	}
// 	if ( m_nGnNum > 1 )
// 	{
// 		qsort((void *)m_pGnWarn, m_nGnNum, sizeof(LFO_GN_MON), GnMonCompareByStat);
// 	}
//     //  ͳ�����豸
//     for ( ii=0; ii<m_nTrNum; ii++ )
//     {
//         memcpy( (void*)&m_pTrWarn[ii], (void*)&m_pTrMon[ii], sizeof(LFO_TR_MON) );
//         if ( m_pTrMon[ii].stat > MENU_LFO_DEV_STS_SCT )
//         {
// 	        memcpy( (void*)&m_DevTrWarn[m_nTrWarn], (void*)&m_pTrMon[ii], sizeof(LFO_TR_MON) );
//             m_nTrWarn += 1;
//         }
//     }
//     if ( m_nTrNum > 1 )
//     {
//         qsort((void *)m_pTrWarn, m_nTrNum, sizeof(LFO_TR_MON), TrMonCompareByStat);
// 	}
// 	//  �����豸״̬�жϵ���״̬
// 	if ( (m_nLnWarn + m_nGnWarn + m_nTrWarn) > 0 )  // ���ڸ澯(������ln gn �Ƚ�)
// 	{
// 		m_SysMon.cur_time     = m_CurGpsTime;
// 		m_SysMon.warn_gn_num  = m_nGnWarn;
// 		m_SysMon.warn_ln_num  = m_nLnWarn;
//         m_SysMon.warn_tr_num  = m_nTrWarn;
//         if( m_nGnWarn > 0)
//         {
//             m_SysMon.mon_gn_id = m_pGnWarn[0].gn_id;
//         }
//         if( m_nLnWarn > 0)
//         {
//             m_SysMon.mon_ln_id = m_pLnWarn[0].acln_id;
//         }
//         if( m_nTrWarn > 0)
//         {
//             m_SysMon.mon_tr_id = m_pTrWarn[0].tr_id;
//         }
// 		if (m_nLnWarn > 0 && m_nGnWarn > 0 ) 
// 		{
// 			m_SysMon.stat    = DP_MAX(m_pGnWarn[0].stat, m_pLnWarn[0].stat);
// 			
// 			m_SysMon.mon_ampl = m_pLnWarn[0].lead_ampl;
// 			m_SysMon.mon_freq = m_pLnWarn[0].lead_freq;
// 			m_SysMon.mon_damp = m_pLnWarn[0].lead_damp;
// 			m_SysMon.mon_phas = m_pLnWarn[0].lead_phas;
// 			m_SysMon.mon_real = m_pLnWarn[0].lead_real;
// 			m_SysMon.mon_imag = m_pLnWarn[0].lead_imag;
// 			m_SysMon.mon_engy = m_pLnWarn[0].lead_engy;
// 			m_SysMon.mon_fail = m_pLnWarn[0].lead_fail;
// 			
// 			m_SysMon.mon_gn_id = m_pGnWarn[0].gn_id;
// 			m_SysMon.mon_ln_id = m_pLnWarn[0].acln_id;
// 
// 			for ( jj=0; jj<m_nLnNum; jj++ )
// 			{
// 				if (m_SysMon.mon_ln_id == m_pLnInf[jj].acln_id )
// 				{
// 					temp_id   = m_SysMon.mon_ln_id;
// 					//sprintf(dev_name,"%s",m_pLnInf[jj].acln_name);
// 					if(m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_1)
// 					{
// 						fac_id = m_pLnInf[jj].fac_id_1;
// 						sprintf(fac_name,"%s",m_pLnInf[jj].fac_name_1);
// 					}
// 					else
// 					{
// 						fac_id = m_pLnInf[jj].fac_id_2;
// 						sprintf(fac_name,"%s",m_pLnInf[jj].fac_name_2);
// 					}
// 					temp_ampl = m_pLnMon[jj].max_ampl;
// 					sprintf(DevName, "%s", m_pLnInf[jj].acln_name);
// 					break;
// 				}
// 					
// 			}
//             
// 			switch( m_SysMon.stat )
// 			{
// 				case MENU_LFO_DEV_STS_FWN:   sprintf(m_SysMon.stat_ctx, "Ԥ���������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 				case MENU_LFO_DEV_STS_WRN:   sprintf(m_SysMon.stat_ctx, "�澯�������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 				case MENU_LFO_DEV_STS_WRN2:  sprintf(m_SysMon.stat_ctx, "���ظ澯�������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 
// 				default :                    strcpy(m_SysMon.stat_ctx, "");                 
// 			}
// 		}
// 		else if (m_nLnWarn > 0)
// 		{
// 			m_SysMon.stat     = m_pLnWarn[0].stat;
// 			
// 			m_SysMon.mon_ampl = m_pLnWarn[0].lead_ampl;
// 			m_SysMon.mon_freq = m_pLnWarn[0].lead_freq;
// 			m_SysMon.mon_damp = m_pLnWarn[0].lead_damp;
// 			m_SysMon.mon_phas = m_pLnWarn[0].lead_phas;
// 			m_SysMon.mon_real = m_pLnWarn[0].lead_real;
// 			m_SysMon.mon_imag = m_pLnWarn[0].lead_imag;
// 			m_SysMon.mon_engy = m_pLnWarn[0].lead_engy;
// 			m_SysMon.mon_fail = m_pLnWarn[0].lead_fail;
// 			
// 			m_SysMon.mon_gn_id = 0;
//             m_SysMon.mon_tr_id = 0;
// 			m_SysMon.mon_ln_id = m_pLnWarn[0].acln_id;
// 			
// 			for ( jj=0; jj<m_nLnNum; jj++ )
// 			{
// 				if (m_SysMon.mon_ln_id == m_pLnInf[jj].acln_id )
// 				{
// 					temp_id   = m_SysMon.mon_ln_id;
// 					//sprintf(dev_name,"%s",m_pLnInf[jj].acln_name);
// 					if(m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_1)
// 					{
// 						fac_id = m_pLnInf[jj].fac_id_1;
// 						sprintf(fac_name,"%s",m_pLnInf[jj].fac_name_1);
// 					}
// 					else
// 					{
// 						fac_id = m_pLnInf[jj].fac_id_2;
// 						sprintf(fac_name,"%s",m_pLnInf[jj].fac_name_2);
// 					}
// 					temp_ampl = m_pLnMon[jj].max_ampl;
// 					sprintf(DevName, "%s", m_pLnInf[jj].acln_name);
// 					break;
// 				}
// 					
// 			}
//             
// 			switch( m_SysMon.stat )
// 			{
// 				case MENU_LFO_DEV_STS_FWN:   sprintf(m_SysMon.stat_ctx, "Ԥ���������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 				case MENU_LFO_DEV_STS_WRN:   sprintf(m_SysMon.stat_ctx, "�澯�������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 				case MENU_LFO_DEV_STS_WRN2:  sprintf(m_SysMon.stat_ctx, "���ظ澯�������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 
// 				default :                    strcpy(m_SysMon.stat_ctx, "");                 
// 			}
// 		}
//         else if (m_nTrWarn > 0)
//         {
//             m_SysMon.stat     = m_pTrWarn[0].stat;
//             
//             m_SysMon.mon_ampl = m_pTrWarn[0].lead_ampl;
//             m_SysMon.mon_freq = m_pTrWarn[0].lead_freq;
//             m_SysMon.mon_damp = m_pTrWarn[0].lead_damp;
//             m_SysMon.mon_phas = m_pTrWarn[0].lead_phas;
//             m_SysMon.mon_real = m_pTrWarn[0].lead_real;
//             m_SysMon.mon_imag = m_pTrWarn[0].lead_imag;
//             m_SysMon.mon_engy = m_pTrWarn[0].lead_engy;
//             m_SysMon.mon_fail = m_pTrWarn[0].lead_fail;
//             
//             m_SysMon.mon_gn_id = 0;
//             m_SysMon.mon_tr_id = m_pTrWarn[0].tr_id;
//             m_SysMon.mon_ln_id = 0;
//             
//             for ( jj=0; jj<m_nTrNum; jj++ )
//             {
//                 if (m_SysMon.mon_tr_id == m_pTrInf[jj].tr_id )
//                 {
//                     temp_id   = m_SysMon.mon_tr_id;
// 					//sprintf(dev_name,"%s",m_pTrInf[jj].tr_name);
// 					fac_id = m_pTrInf[jj].fac_id;
// 					sprintf(fac_name,"%s",m_pTrInf[jj].fac_name);
//                     temp_ampl = m_pTrMon[jj].max_ampl;
// 					if(para_ShowFacName == 1)
// 					{
// 						sprintf(DevName, "%s%s", m_pTrInf[jj].fac_name,m_pTrInf[jj].tr_name);
// 					}
// 					else
// 					{
// 						sprintf(DevName, "%s", m_pTrInf[jj].tr_name);
// 					}
//                     
//                     break;
//                 }
//                 
//             }
//             
//             switch( m_SysMon.stat )
//             {
//  			case MENU_LFO_DEV_STS_FWN:   sprintf(m_SysMon.stat_ctx, "Ԥ���������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 			case MENU_LFO_DEV_STS_WRN:   sprintf(m_SysMon.stat_ctx, "�澯�������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 			case MENU_LFO_DEV_STS_WRN2:  sprintf(m_SysMon.stat_ctx, "���ظ澯�������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 
//             default :                    strcpy(m_SysMon.stat_ctx, "");                 
//             }
// 		}
// 		else
// 		{
// 			m_SysMon.stat     = m_pGnWarn[0].stat;
// 			
// 			m_SysMon.mon_ampl = m_pGnWarn[0].lead_ampl;
// 			m_SysMon.mon_freq = m_pGnWarn[0].lead_freq;
// 			m_SysMon.mon_damp = m_pGnWarn[0].lead_damp;
// 			m_SysMon.mon_phas = m_pGnWarn[0].lead_phas;
// 			m_SysMon.mon_real = m_pGnWarn[0].lead_real;
// 			m_SysMon.mon_imag = m_pGnWarn[0].lead_imag;
// 			m_SysMon.mon_engy = m_pGnWarn[0].lead_engy;
// 			m_SysMon.mon_fail = m_pGnWarn[0].lead_fail;
// 			
// 			m_SysMon.mon_gn_id = m_pGnWarn[0].gn_id;
// 			m_SysMon.mon_ln_id = 0;
// 			for ( jj=0; jj<m_nGnNum; jj++ )
// 			{
// 				if (m_SysMon.mon_gn_id == m_pGnInf[jj].gn_id )
// 				{
// 					temp_id   = m_SysMon.mon_gn_id;
// 					//sprintf(dev_name,"%s",m_pGnInf[jj].gn_name);
// 					fac_id = m_pGnInf[jj].fac_id;
// 					sprintf(fac_name,"%s",m_pGnInf[jj].fac_name);
// 					temp_ampl = m_pGnMon[jj].max_ampl;
// 					if (para_ShowFacName == 1)
// 					{
// 						sprintf(DevName, "%s%s", m_pGnInf[jj].fac_name, m_pGnInf[jj].gn_name );
// 					}
// 					else
// 					{
// 						sprintf(DevName, "%s", m_pGnInf[jj].gn_name );
// 					}
// 					
// 					break;
// 				}
// 					
// 			}
//             // 2008-04-15 modified by zhw at nj  2*temp_ampl -> temp_ampl
// 			switch( m_SysMon.stat )
// 			{
// 				case MENU_LFO_DEV_STS_FWN:   sprintf(m_SysMon.stat_ctx, "Ԥ���������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 				case MENU_LFO_DEV_STS_WRN:   sprintf(m_SysMon.stat_ctx, "�澯�������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 				case MENU_LFO_DEV_STS_WRN2:  sprintf(m_SysMon.stat_ctx, "���ظ澯�������豸[%s],��Ƶ��[%4.2fHz],���ʲ���[%6.2fMW],�����[%6.3f%]",DevName,m_SysMon.mon_freq, 2*temp_ampl,m_SysMon.mon_damp ); break;
// 				default :                    strcpy(m_SysMon.stat_ctx, "");                 
// 			}
// 		}
// 	}
// 	else                                // �޸澯(��Ҫ�����˵�˳��)�����Ƶ���״̬��
// 	{
// 		m_SysMon.cur_time     = m_CurGpsTime;
// 		m_SysMon.warn_gn_num  = 0;
// 		m_SysMon.warn_ln_num  = 0;
//         m_SysMon.warn_tr_num  = 0;
// 		
// 		m_SysMon.mon_ampl = 0;
// 		m_SysMon.mon_freq = 0;
// 		m_SysMon.mon_damp = 0;
// 		m_SysMon.mon_phas = 0;
// 		m_SysMon.mon_real = 0;
// 		m_SysMon.mon_imag = 0;
// 		m_SysMon.mon_engy = 0;
// 		m_SysMon.mon_fail = 0;
// 		
// 		m_SysMon.mon_gn_id = 0;
// 		m_SysMon.mon_ln_id = 0;
//         m_SysMon.mon_tr_id = 0;
// 		if (m_nLnNum > 0 && m_nGnNum > 0 && m_nTrNum > 0) 
// 		{
// 			m_SysMon.stat    = DP_MAX(m_pGnWarn[0].stat, m_pLnWarn[0].stat);
// 			m_SysMon.stat    = DP_MAX(m_SysMon.stat,m_pTrMon[0].stat);
// 		}
// 		else
// 		{
// 			m_SysMon.stat     = MENU_LFO_DEV_STS_SCT;
// 		}
// 	}
// 	m_SysMon.stat_val     = m_SysMon.stat;
// // 	switch( m_SysMon.stat )
// // 	{
// // 		//case MENU_LFO_DEV_STS_OK:    strcpy(m_SysMon.stat_ctx, " ��PMU���ݣ� "); break;
// // 		//case MENU_LFO_DEV_STS_NOYC:  strcpy(m_SysMon.stat_ctx, " ��PMU���ݣ� "); break;
// // 		//case MENU_LFO_DEV_STS_OFF:   strcpy(m_SysMon.stat_ctx, " PMU���ݲ����ã� "); break;
// // 		//case MENU_LFO_DEV_STS_OLD:   strcpy(m_SysMon.stat_ctx, " ϵͳ��ȫ�� "); break;
// // 
// // 		//case MENU_LFO_DEV_STS_BAD:   strcpy(m_SysMon.stat_ctx, ""); break;
// // 		//case MENU_LFO_DEV_STS_NO:    strcpy(m_SysMon.stat_ctx, ""); break;
// // 		//case MENU_LFO_DEV_STS_STP:   strcpy(m_SysMon.stat_ctx, ""); break;
// // 
// // 		//case MENU_LFO_DEV_STS_VAL:   strcpy(m_SysMon.stat_ctx, ""); break;
// // 		//case MENU_LFO_DEV_STS_BRK:   strcpy(m_SysMon.stat_ctx, ""); break;
// // 		//case MENU_LFO_DEV_STS_SCT:   strcpy(m_SysMon.stat_ctx, ""); break;
// // 		case MENU_LFO_DEV_STS_FWN:   sprintf(m_SysMon.stat_ctx, " ��Ƶ�ʣ�%4.2f,�������豸��%s"); break;
// // 		case MENU_LFO_DEV_STS_WRN:   strcpy(m_SysMon.stat_ctx, " ϵͳ���ֵ�Ƶ�񵴣�"); break;
// // 		case MENU_LFO_DEV_STS_WRN2:  strcpy(m_SysMon.stat_ctx, " ϵͳ���ֵ�Ƶ�񵴣�"); break;
// // 		default :                    strcpy(m_SysMon.stat_ctx, "");                 
// // 	}
// 	//
//     //if( m_runStat == true )
// 	{
// 		UpdateMonSysTable( m_SysMon ) ;
// 	}    
// 
// 	sprintf(m_LogString, " %s, m_SysMon.stat =%d��m_SysMon_Old.stat =%d", m_SysMon.stat_ctx, m_SysMon.stat, m_SysMon_Old.stat);
// 	MessShow( m_LogString );
// 				
// 	////////////
// 	//  �澯
// 	int   warn_time = 0;
// 	int   warn_flag = 0;
// 	//////////////////////////////////////////////////////////////////////////
// 	// (����LFO���,�ж�����Ƿ�Խ������ֵ)
// 	//float  tempMaxAmpl = 0;
// 	int tmptableno;
// 	if (m_SysMon.stat == MENU_LFO_DEV_STS_WRN )
// 	{
// 		if (m_LfoAmplOverLmt == false && (m_nLnWarn + m_nGnWarn + m_nTrWarn) > 0)
// 		{
// 			tmptableno = PODB_GetTabelNoByDevId(temp_id);
// 			if (tmptableno == ACLN_DOT_NO_TAB || tmptableno == ACLN_DEVICE_NO_TAB )
// 			{
// 				if ( temp_ampl > m_LfoPara.ln_wn2_ampl ) 
// 				{
// 					m_LfoAmplOverLmt = true;
// 
// 					//�澯��Ϣ
// 					char  temp_time_str[200];
//                     
// 					sprintf(temp_time_str, "��������ֵ[%6.2fMW]",  2*m_LfoPara.ln_wn2_ampl );
// 					strcat(m_SysMon.stat_ctx, temp_time_str);
// 
// 					LFO_SYS_MON    tempSendSysStatus;
// 					memcpy((void*)&tempSendSysStatus, (void*)&m_SysMon, sizeof(LFO_SYS_MON));
// 					tempSendSysStatus.stat = MENU_LFO_DEV_STS_WRN2;
// 					
// 					warn_time = m_SysMon.cur_time - m_LfoPara.time_leng;
// 					LFOSendWarnMessage( tempSendSysStatus, warn_time, m_DispCtl.lfo_fac_id) ;
// 					MessShow( m_SysMon.stat_ctx );
// 					warn_flag ++;
// 				}
// 			}
// 			else if(tmptableno == GN_DEVICE_NO_TAB )
// 			{
// 				if ( temp_ampl > m_LfoPara.gn_wn2_ampl ) 
// 				{
// 					m_LfoAmplOverLmt = true;
// 
// 					//�澯��Ϣ
// 					char  temp_time_str[200];
//                     
// 					sprintf(temp_time_str, "��������ֵ[%6.2fMW]",  2*m_LfoPara.ln_wn2_ampl );
// 					strcat(m_SysMon.stat_ctx, temp_time_str);
// 
// 					LFO_SYS_MON    tempSendSysStatus;
// 					memcpy((void*)&tempSendSysStatus, (void*)&m_SysMon, sizeof(LFO_SYS_MON));
// 					tempSendSysStatus.stat = MENU_LFO_DEV_STS_WRN2;
// 					
// 					warn_time = m_SysMon.cur_time - m_LfoPara.time_leng;
// 					LFOSendWarnMessage( tempSendSysStatus, warn_time, m_DispCtl.lfo_fac_id) ;
// 					MessShow( m_SysMon.stat_ctx );
// 					warn_flag ++;
// 				}
// 			}
//             else if (tmptableno == TR_DEVICE_NO_TAB || tmptableno == TRWD_DEVICE_NO_TAB)
//             {
//                 if ( temp_ampl > m_LfoPara.tr_wn2_ampl ) 
//                 {
//                     m_LfoAmplOverLmt = true;
//                     
//                     //�澯��Ϣ
//                     char  temp_time_str[200];
//                     
//                     sprintf(temp_time_str, "��������ֵ[%6.2fMW]",  2*m_LfoPara.tr_wn2_ampl );
//                     strcat(m_SysMon.stat_ctx, temp_time_str);
//                     
//                     LFO_SYS_MON    tempSendSysStatus;
//                     memcpy((void*)&tempSendSysStatus, (void*)&m_SysMon, sizeof(LFO_SYS_MON));
//                     tempSendSysStatus.stat = MENU_LFO_DEV_STS_WRN2;
//                     
//                     warn_time = m_SysMon.cur_time - m_LfoPara.time_leng;
//                     LFOSendWarnMessage( tempSendSysStatus, warn_time, m_DispCtl.lfo_fac_id) ;
//                     MessShow( m_SysMon.stat_ctx );
// 					warn_flag ++;
//                 }
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (m_LfoAmplOverLmt == true) m_LfoAmplOverLmt = false;
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	
// 	////////////
// 	//�澯 m_SysStatus
// 	if((m_SysMon.stat - m_SysMon_Old.stat) != 0)    
// 	{
// 		if ((m_SysMon_Old.stat >= MENU_LFO_DEV_STS_SCT && m_SysMon_Old.stat <= MENU_LFO_DEV_STS_WRN ))
// 		{
// 			if((m_SysMon.stat >= MENU_LFO_DEV_STS_SCT && m_SysMon.stat <= MENU_LFO_DEV_STS_WRN))
// 			{
// 				if(m_SysMon.stat == MENU_LFO_DEV_STS_WRN)            
// 				{
// 					//�澯��Ϣ
// 					warn_time = m_SysMon.cur_time - m_LfoPara.lfo_jud_num*m_LfoPara.time_leng+(m_LfoPara.lfo_jud_num-1)*m_LfoPara.int_time;//2007-1-20 �������Ϊ�о�����
// 					if (warn_flag == 0)
// 					{
// 						LFOSendWarnMessage( m_SysMon, warn_time, m_DispCtl.lfo_fac_id); 
// 					}
// 					MessShow( m_SysMon.stat_ctx );
// 					
// 					memset(&m_ThisLfoInfo,0,sizeof(THIS_LFO_INFO)); //���ε�Ƶ����Ϣ
// 					m_ThisLfoInfo.start_time = m_CurGpsTime - m_LfoPara.time_leng;
// 					m_ThisLfoInfo.end_time = m_CurGpsTime;
// 					//m_ThisLfoInfo.hold_time = abs(m_ThisLfoInfo.end_time - m_ThisLfoInfo.start_time);
// 					m_ThisLfoInfo.frequency  = m_SysMon.mon_freq;
// 					m_ThisLfoInfo.ampl_max   = temp_ampl;
// 					strcpy( m_ThisLfoInfo.obj_name, DevName );
// 					//2006-5-18
// 					m_ThisLfoInfo.obj_id     = temp_id;
// 					//sprintf(m_ThisLfoInfo.obj_name,"%s",dev_name);
// 					m_ThisLfoInfo.fac_id = fac_id;
// 					sprintf(m_ThisLfoInfo.fac_name,"%s",fac_name);
// 					
// 					// 2007-1-18 ������ȫ���̷������
// 					m_FormResSaveStat = L_FST_FORM_RESULTE; //(0 ������ 1�������� 2���� 3ͳ�������)
// 					m_WarnSendFlag    = 1;
// 					//////////////////////////////////////////////////////////////////////////
// 				}
// 				else if(m_SysMon.stat == MENU_LFO_DEV_STS_FWN) 
// 				{
// 					if(m_SysMon_Old.stat != MENU_LFO_DEV_STS_WRN)     
// 					{
// 						//Ԥ����Ϣ
// 						warn_time = m_SysMon.cur_time - m_LfoPara.lfo_jud_num*m_LfoPara.time_leng+(m_LfoPara.lfo_jud_num-1)*m_LfoPara.int_time;//2007-1-20 �������Ϊ�о�����
// 						if (warn_flag == 0)
// 						{
// 							LFOSendWarnMessage( m_SysMon, warn_time, m_DispCtl.lfo_fac_id) ;
// 						}
// 						MessShow(m_SysMon.stat_ctx);
// 
// 						// 2006-09-13
// 						memset(&m_ThisLfoInfo,0,sizeof(THIS_LFO_INFO)); //���ε�Ƶ����Ϣ
// 						m_ThisLfoInfo.start_time = m_CurGpsTime - m_LfoPara.time_leng;
// 						m_ThisLfoInfo.end_time = m_CurGpsTime;
// 						//m_ThisLfoInfo.hold_time = abs(m_ThisLfoInfo.end_time - m_ThisLfoInfo.start_time);
// 						m_ThisLfoInfo.frequency  = m_SysMon.mon_freq;
// 						m_ThisLfoInfo.ampl_max   = temp_ampl;
// 						strcpy(m_ThisLfoInfo.obj_name, DevName);;
// 						//2006-5-18
// 						m_ThisLfoInfo.obj_id = temp_id;
// 						//sprintf(m_ThisLfoInfo.obj_name,"%s",dev_name);
// 						m_ThisLfoInfo.fac_id = fac_id;
// 						sprintf(m_ThisLfoInfo.fac_name,"%s",fac_name);
// 					}
// 				}
// 				else if(m_SysMon.stat == MENU_LFO_DEV_STS_SCT)   
// 				{
//                     if(m_SysMon_Old.stat == MENU_LFO_DEV_STS_WRN)  // 2008-05-08 �澯-> ��ȫ
//                     {
//                         int hold_time,nM,nS;
//                         char  temp_time_str[64];
// 						// ״̬��ȫ��������ʱ��Ϊ����ʱ��
//                         warn_time = m_CurGpsTime;//2007-1-20 �������Ϊ�о�����
//                         m_ThisLfoInfo.end_time  = m_CurGpsTime - m_LfoPara.time_leng + m_LfoPara.int_time;
//                         hold_time = abs(m_ThisLfoInfo.end_time - m_ThisLfoInfo.start_time);
//                         nM = hold_time/60;
//                         nS = (hold_time - nM*60);
//                         if(nM >0)
//                         {
//                             sprintf(temp_time_str, "%d��%d��",nM,nS);
//                         }
//                         else
//                         {
//                             sprintf(temp_time_str, "%d��", nS);
//                         }
//                         
//                         strcpy(m_ThisLfoInfo.hold_time, temp_time_str);
//                         
//                         strcpy(m_SysMon.stat_ctx, " ��Ƶ�񵴸澯����,");
//                         sprintf(temp_time_str,"����ʱ��[%s]��", m_ThisLfoInfo.hold_time );
//                         strcat(m_SysMon.stat_ctx, temp_time_str);
//                         
//                         LFOSendWarnMessage( m_SysMon, warn_time, m_DispCtl.lfo_fac_id); 
//                         MessShow( m_SysMon.stat_ctx );
//                         
//                         //SendStartRecMessage(1);	//�˳�¼�������ͽ���ʱ��
//                         //writeLfoOnlnModeToWAMAP(0, m_lfoOsciOrder, 1); // ���������ģʽ�б�				
//                         
//                         //////////////////////////////////////////////////////////////////////////
//                         // 2007-1-18 ������ȫ���̷������
//                         m_FormResSaveStat = L_END_FORM_RESULTE;            //(0 ������ 1�������� 2���� 3ͳ�������)
// 						m_WarnSendFlag = 0;
//                         //////////////////////////////////////////////////////////////////////////	
//                     }
// 				}
// 				else
// 				{
// 					if (m_SysMon_Old.stat == MENU_LFO_DEV_STS_WRN || m_SysMon_Old.stat == MENU_LFO_DEV_STS_FWN) 
// 					{
// 						char  temp_time_str[64];
// 						sprintf(temp_time_str,"��Ƶ�񵴸澯�ڼ䣬���ݳ����쳣��");
// 						strcat(m_SysMon.stat_ctx, temp_time_str);
// 						
// 						warn_time = m_SysMon.cur_time;//2007-1-20 �������Ϊ�о�����
// 						LFOSendWarnMessage( m_SysMon, warn_time, m_DispCtl.lfo_fac_id) ;
// 						
// 						sprintf(temp_time_str,"%s��m_SysMon.stat=%d", m_SysMon.stat_ctx,  m_SysMon.stat );
// 						MessShow( temp_time_str );
// 					}
// 				}
// 			}
// 		}
// 		else if (m_SysMon_Old.stat >= MENU_LFO_DEV_STS_NOYC && m_SysMon_Old.stat <= MENU_LFO_DEV_STS_BRK )
// 		{
// 			// �ѷ����񵴣�����״̬�쳣�����˳���־
// 			if (m_WarnSendFlag == 1 && m_SysMon.stat == MENU_LFO_DEV_STS_SCT)
// 			{
// 				m_FormResSaveStat = L_END_FORM_RESULTE;
// 				m_WarnSendFlag =0;
// 			}
// 		}
// 
// 	}
// 		
// // 	if( m_SysMon.stat > MENU_LFO_DEV_STS_SCT )
// // 	{
// // 		if(m_SysMon.mon_ampl > ZEROVALUE && m_SysMon.mon_freq > ZEROVALUE )
// // 		{
// // 			WriteLogFile("������ģʽ����ʷ�⣡");
// // 			//wam_lfo_sample( &m_SysStatus );             // ������ģʽ����ʷ��
// // 			char filename[FILENAMELEN];
// // 			sprintf(filename, "../lfo_log/%d.dat", m_ThisLfoInfo.beg_time);
// // 			SaveLfoCaseDatatoFile(filename, m_CurGpsTime, m_nOrigSam, m_nPmuDataGens, m_genPmuData, m_nPmuDataAclns, m_aclnPmuData );// ����:��LFO�������ݴ����ļ�	
// // 			printf(">>>>>>>>> EstimateSysOscillate(): ����LFO���ݣ�\n");
// // 
// // 		}		
// // 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// 2007-1-18 ������ȫ���̷������
// // 	if(m_FormResSaveStat == L_FST_FORM_RESULTE)//(0 ������ 1�������� 2���� 3ͳ�������)
// // 	{
// // 		InitLfoResultSaveStruct( ) ;
// // 		FormLfoMonSaveResult( ) ;
// // 		m_FormResSaveStat = L_SnD_FORM_RESULTE;
// // 	}
// // 	else if(m_FormResSaveStat == L_SnD_FORM_RESULTE && m_SysMon.stat == MENU_LFO_DEV_STS_WRN)
// // 	{
// // 		FormLfoMonSaveResult( );
// // 	}
// // 	else if(m_FormResSaveStat == L_END_FORM_RESULTE)
// // 	{
// // 		FormLfoMonSaveResult( );
// // 		SaveResultFileTxt( ); 
// // 		m_FormResSaveStat = L_NOT_FORM_RESULTE ;
// // 
// // 		//�������ݿ�LFO�������
// // 		UpdateMonLfoToWAMAP(  );
// // 		SaveLfoCaseByTmd(  );   // ����TMD����������
// // 	}
// 
// 	memcpy((char *)&m_SysMon_Old, (char *)&m_SysMon, sizeof(LFO_SYS_MON));
// 	//////////////////////////////////////////////////////////////////////////
// //*/
//   return ;
// }

// ���߼�����Ϣ(lfo)
void CLfoAnaObj::UpdateMonLfoToWAMAP()
{
	int   ii, jj;
	//float temp_freq = 0;

	UpdateLfoSysTable();
	UpdateLfoGnTable(m_nGnNum, m_pGnInf, m_pGnResSave);
	UpdateLfoLnTable(m_nLnNum, m_pLnInf, m_pLnResSave);
	UpdateLfoTrTable(m_nTrNum, m_pTrInf, m_pTrResSave);
	int                 nDevLeadMode = 0;
	LFO_DEV_LEAD_MODE* pDevLeadMode = NULL; //[para_nObjMaxMode];

	pDevLeadMode = (LFO_DEV_LEAD_MODE*)calloc(para_nObjMaxMode, sizeof(LFO_DEV_LEAD_MODE));
	memset((void*)pDevLeadMode, 0, para_nObjMaxMode * sizeof(LFO_DEV_LEAD_MODE));
	if (m_SysResSave.act_gen_num > 0)
	{
		for (ii = 0; ii < m_nGnNum; ii++)
		{
			if (m_pGnResSave[ii].lfo_b_t > 0)
			{
				for (jj = 0; jj < m_pGnResSave[ii].mode1_num; jj++)
				{
					pDevLeadMode[nDevLeadMode].dev_id = m_pGnResSave[ii].obj_id;
					pDevLeadMode[nDevLeadMode].dev_type = MENU_LFO_DEV_GEN;
					pDevLeadMode[nDevLeadMode].wds_bgn = m_pGnResSave[ii].mode1[jj].bgn_t;
					pDevLeadMode[nDevLeadMode].wds_end = m_pGnResSave[ii].mode1[jj].end_t;
					pDevLeadMode[nDevLeadMode].ampl = m_pGnResSave[ii].mode1[jj].ampl;
					pDevLeadMode[nDevLeadMode].freq = m_pGnResSave[ii].mode1[jj].freq;
					pDevLeadMode[nDevLeadMode].ampl = m_pGnResSave[ii].mode1[jj].damp;
					pDevLeadMode[nDevLeadMode].ampl = m_pGnResSave[ii].mode1[jj].phase;
					pDevLeadMode[nDevLeadMode].real = m_pGnResSave[ii].mode1[jj].real;
					pDevLeadMode[nDevLeadMode].imag = m_pGnResSave[ii].mode1[jj].image;
					pDevLeadMode[nDevLeadMode].engy = 0;
					pDevLeadMode[nDevLeadMode].fail = 0;
					nDevLeadMode += 1;
				}
			}
		}
	}
	if (m_SysResSave.lfo_line_num > 0)
	{
		for (ii = 0; ii < m_nLnNum; ii++)
		{
			if (m_pLnResSave[ii].lfo_b_t > 0)
			{
				for (jj = 0; jj < m_pLnResSave[ii].mode1_num; jj++)
				{
					pDevLeadMode[nDevLeadMode].dev_id = m_pLnResSave[ii].obj_id;
					pDevLeadMode[nDevLeadMode].dev_type = MENU_LFO_DEV_LINE;
					pDevLeadMode[nDevLeadMode].wds_bgn = m_pLnResSave[ii].mode1[jj].bgn_t;
					pDevLeadMode[nDevLeadMode].wds_end = m_pLnResSave[ii].mode1[jj].end_t;
					pDevLeadMode[nDevLeadMode].ampl = m_pLnResSave[ii].mode1[jj].ampl;
					pDevLeadMode[nDevLeadMode].freq = m_pLnResSave[ii].mode1[jj].freq;
					pDevLeadMode[nDevLeadMode].ampl = m_pLnResSave[ii].mode1[jj].damp;
					pDevLeadMode[nDevLeadMode].ampl = m_pLnResSave[ii].mode1[jj].phase;
					pDevLeadMode[nDevLeadMode].real = m_pLnResSave[ii].mode1[jj].real;
					pDevLeadMode[nDevLeadMode].imag = m_pLnResSave[ii].mode1[jj].image;
					pDevLeadMode[nDevLeadMode].engy = 0;
					pDevLeadMode[nDevLeadMode].fail = 0;
					nDevLeadMode += 1;
				}
			}
		}
	}
	if (m_SysResSave.lfo_tr_num > 0)
	{
		for (ii = 0; ii < m_nTrNum; ii++)
		{
			if (m_pTrResSave[ii].lfo_b_t > 0)
			{
				for (jj = 0; jj < m_pTrResSave[ii].mode1_num; jj++)
				{
					pDevLeadMode[nDevLeadMode].dev_id = m_pTrResSave[ii].obj_id;
					pDevLeadMode[nDevLeadMode].dev_type = MENU_LFO_DEV_TR;
					pDevLeadMode[nDevLeadMode].wds_bgn = m_pTrResSave[ii].mode1[jj].bgn_t;
					pDevLeadMode[nDevLeadMode].wds_end = m_pTrResSave[ii].mode1[jj].end_t;
					pDevLeadMode[nDevLeadMode].ampl = m_pTrResSave[ii].mode1[jj].ampl;
					pDevLeadMode[nDevLeadMode].freq = m_pTrResSave[ii].mode1[jj].freq;
					pDevLeadMode[nDevLeadMode].ampl = m_pTrResSave[ii].mode1[jj].damp;
					pDevLeadMode[nDevLeadMode].ampl = m_pTrResSave[ii].mode1[jj].phase;
					pDevLeadMode[nDevLeadMode].real = m_pTrResSave[ii].mode1[jj].real;
					pDevLeadMode[nDevLeadMode].imag = m_pTrResSave[ii].mode1[jj].image;
					pDevLeadMode[nDevLeadMode].engy = 0;
					pDevLeadMode[nDevLeadMode].fail = 0;
					nDevLeadMode += 1;
				}
			}
		}
	}
	if (nDevLeadMode > 0)
	{
		UpdateLfoMdTable(nDevLeadMode, pDevLeadMode);
	}
	if (pDevLeadMode) { free(pDevLeadMode); pDevLeadMode = NULL; }
	return;
}

// �жϵ������Ӷ���
//void CLfoAnaObj::EstimateSingleObjState( )
void CLfoAnaObj::EstimateSingleObjState(OSCILLAT_FLAG* osciFlag, LFO_LEAD_MODE* leadMode, int times, float SameFreqDifVal)
{

	float          LD_Warn_Damping = 3.0;         //��Ƿ����ʱ��������ٽ�ֵ
	int            LD_Times = 2;           //����Ƿ�����оݴ������ڸ�ֵ��Ƿ����澯(>=1)
	float          LFO_Warn_Damping = 0.00001f;     //�е�Ƶ��ʱ��������ٽ�ֵ
	float          Out_LFO_Warn_Damping = 0.5;         //�˳���Ƶ�񵴸澯ʱ��������ٽ�ֵ
	int            LFO_Times = 2;           //�����Ƶ���оݴ������ڸ�ֵ����Ƶ�񵴸澯(>=1)
	float          LFO_Forewarn_Damping = 1.5;         //�е�Ƶ��Ԥ��ʱ��������ٽ�ֵ
	int            LFO_Fore_Times = 2;           //�����Ƶ��Ԥ���оݴ������ڸ�ֵ����Ƶ��Ԥ��(>=1)
	int            DN_Times = 2;           //�������������оݴ������ڸ�ֵ����ʾ��������(>=1)
	float          LFO_Ampl_Per = 0.1f;         //�е�Ƶ��ʱ��������ֵ������ֵ
	float          LFO_Ampl_Per_Fir = 0.1f;         //�е�Ƶ��ʱ��������ֵ������С��ֵ  206-1-18
	float          temp_ampl = 0.0;
	float temp_freq_margin = 2 * SameFreqDifVal;

#ifdef	LFO_DEBUG_

	sprintf(m_LogString, "EstimateSingleObjState() obj_id="INT_REC_FMT" aver_ampl=%f ampl=%f freq=%f damp=%f",
		INT_REC_VAL(leadMode->obj_id),
		leadMode->aver_ampl,
		leadMode->amplitude,
		leadMode->frequency,
		leadMode->damping);
	MessShow(m_LogString);
#endif
	int tmptalbeno = PODB_GetTabelNoByDevId(leadMode->obj_id);
	if (osciFlag->lfo_warn == 0)
	{
		if (osciFlag->begin_ampl > 0.1)
		{
			LFO_Ampl_Per = osciFlag->begin_ampl;
		}
		else
		{
			if (tmptalbeno == ACLN_DOT_NO_TAB || tmptalbeno == ACLN_DEVICE_NO_TAB)
			{
				// 				if (leadMode->vbase<0.001)
				// 				{
				// 					leadMode->vbase = 220.0;
				// 				}
				if (leadMode->vbase < m_LfoPara.volt_thre1)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre1 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.ln_swn_ampl1;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre1 && leadMode->vbase < m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre2 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.ln_swn_ampl2;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre3 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.ln_swn_ampl3;
				}
			}
			else if (tmptalbeno == TR_DEVICE_NO_TAB || tmptalbeno == TRWD_DEVICE_NO_TAB)
			{
				if (leadMode->vbase < m_LfoPara.volt_thre1)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre1 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.tr_swn_ampl1;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre1 && leadMode->vbase < m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre2 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.tr_swn_ampl2;

				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre3 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.tr_swn_ampl3;

				}
			}
			else
			{
				LFO_Ampl_Per = m_LfoPara.gn_swn_ampl;
			}
		}
	}
	else if (osciFlag->lfo_warn == 1)
	{
		if (osciFlag->end_ampl > 0.1)
		{
			LFO_Ampl_Per = osciFlag->end_ampl;
		}
		else
		{
			if (tmptalbeno == ACLN_DOT_NO_TAB || tmptalbeno == ACLN_DEVICE_NO_TAB)
			{
				// 				if (leadMode->vbase<0.001)
				// 				{
				// 					leadMode->vbase = 220.0;
				// 				}
				if (leadMode->vbase < m_LfoPara.volt_thre1)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre1 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.ln_ewn_ampl1;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre1 && leadMode->vbase < m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre2 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.ln_ewn_ampl2;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre3 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.ln_ewn_ampl3;
				}
			}
			else if (tmptalbeno == TR_DEVICE_NO_TAB || tmptalbeno == TRWD_DEVICE_NO_TAB)
			{
				if (leadMode->vbase < m_LfoPara.volt_thre1)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre1 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.tr_ewn_ampl1;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre1 && leadMode->vbase < m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre2 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.tr_ewn_ampl2;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre3 ;//�ٷֱȸ�Ϊ����ֵ
					LFO_Ampl_Per = m_LfoPara.tr_ewn_ampl3;
				}
			}
			else
			{
				LFO_Ampl_Per = m_LfoPara.gn_swn_ampl;
			}
		}
	}
	// 	LFO_Ampl_Per_Fir = LFO_Ampl_Per*m_LfoPara.fjud_wnap_per/100 ;//����ֵ  2006-1-18
	temp_ampl = leadMode->aver_ampl;

	// 	LD_Warn_Damping      = m_LfoPara.lack_damp;
	// 	LFO_Warn_Damping     = m_LfoPara.lfo_damp;
	// 	LFO_Forewarn_Damping = m_LfoPara.flfo_damp;
	Out_LFO_Warn_Damping = LD_Warn_Damping;  //2006-1-18

// 	LD_Times      = m_LfoPara.lfo_jud_num;   
// 	LFO_Fore_Times= m_LfoPara.lfo_jud_num + 1;   
// 	LFO_Times     = m_LfoPara.lfo_jud_num; 

	//���η���
	if (times == 1)
	{
		//�����־λ
		osciFlag->lackdamp_warn = 0;
		osciFlag->times_ldw = 0;
		osciFlag->lfo_forewarn = 0;
		osciFlag->times_lfw = 0;
		osciFlag->lfo_warn = 0;
		osciFlag->times_lw = 0;
		osciFlag->damp_norm = 0;
		osciFlag->times_dn = 0;

		osciFlag->obj_id = leadMode->obj_id;
		osciFlag->LM_frequency = leadMode->frequency;
		osciFlag->LM_damp = leadMode->damping;
		/////////////////////////////////////////////////////////
		// 		//����ģʽʱ ����ԭ����д������㲻�����������Ч��û�з���Ƶ�ʷ�Χ��ģʽ��
		// 		if(temp_leadMode.amplitude <= ZEROVALUE)                       
		// 		{
		// 			//m_ClusOsci[i].LM_damp = 100;
		// 			continue;
		// 		}
		if (osciFlag->LM_frequency < SameFreqDifVal)
			temp_freq_margin = 0;
		else
			temp_freq_margin = (float)fabs(osciFlag->LM_frequency - leadMode->frequency);
		sprintf(m_LogString, "obj_id="INT_REC_FMT", damping=%f, frequency=%f[df=%f],amplitude=%f,phase=%f,aver_ampl=%f[%f]",
			INT_REC_VAL(leadMode->obj_id), leadMode->damping, leadMode->frequency, temp_freq_margin, leadMode->amplitude, leadMode->phase, leadMode->aver_ampl, LFO_Ampl_Per);
		MessShow(m_LogString);
		//��δ���ֻ�ƽϢ����ֵ������
		if (temp_ampl < LFO_Ampl_Per) //leadMode->aver_ampl_rat < LFO_Ampl_Per)  
		{
			// 2008-05-11 by zhw 0.05 0.08
/*
			if(leadMode->damping < LFO_Warn_Damping)
			{
				osciFlag->times_lw += 1;
			}*/

		}
		else  //�����񵴣���ֵ������
		{
			if (leadMode->amplitude < para_Ampl_aver * temp_ampl)
			{
				sprintf(m_LogString, "amplitude=[%6.2f]< [%6.2f]*[%6.2f]", leadMode->amplitude, para_Ampl_aver, temp_ampl);
				MessShow(m_LogString);
			}
			else
			{
				if (leadMode->damping < LFO_Warn_Damping)
				{
					osciFlag->times_lw += 1;

					if (osciFlag->times_lw >= LFO_Times)
					{
						osciFlag->lfo_warn = 1;
						osciFlag->times_lw = 0;
					}
				}
				else if (leadMode->damping >= LFO_Warn_Damping && leadMode->damping < LD_Warn_Damping)
				{
					osciFlag->times_ldw += 1;

					if (osciFlag->times_ldw >= LD_Times)
					{
						osciFlag->lackdamp_warn = 1;
						osciFlag->times_ldw = 0;
					}
				}
				else
				{
					float damp_exp = 0;
					float damp_sign = 0;
					float fcast_ampl = 0;
					damp_sign = (float)(-(leadMode->damping / fabs(leadMode->damping)));
					damp_exp = (float)(damp_sign * fabs(2 * PRONY_PI * leadMode->frequency * leadMode->damping / sqrt((100 - leadMode->damping) * (100 + leadMode->damping))));
					fcast_ampl = (float)(leadMode->amplitude * exp(damp_exp * 2 * m_LfoPara.time_leng));

					osciFlag->times_dn += 1;

					if (osciFlag->times_dn >= DN_Times && fcast_ampl > LFO_Ampl_Per)
					{
						osciFlag->damp_norm = 1;
						osciFlag->times_dn = 0;
					}
				}
			}
		}
	}
	else   //��������
	{
		osciFlag->obj_id = leadMode->obj_id;
		// 2007-04-12 �޸ģ������񵴳��ڣ�osciFlag->LM_frequency����ֵ������temp_freq_margin����Χ���������жϡ����Ӷ�osciFlag->LM_frequency�ж�
		//temp_freq_margin = (float)fabs(osciFlag->LM_frequency - leadMode->frequency);
		//printf(">>>>>>>>> obj_id=%d, damping=%f, frequency=%f[df=%f]\n", leadMode->obj_id, leadMode->damping, leadMode->frequency,temp_freq_margin);
		if (osciFlag->LM_frequency < SameFreqDifVal)
			temp_freq_margin = 0;
		else
			temp_freq_margin = (float)fabs(osciFlag->LM_frequency - leadMode->frequency);
		sprintf(m_LogString, "obj_id="INT_REC_FMT", damping=%f, frequency=%f[df=%f],amplitude=%f,phase=%f,aver_ampl=%f[%f]",
			INT_REC_VAL(leadMode->obj_id), leadMode->damping, leadMode->frequency, temp_freq_margin, leadMode->amplitude, leadMode->phase, leadMode->aver_ampl, LFO_Ampl_Per);
		MessShow(m_LogString);
		//��δ���ֻ�ƽϢ����ֵ������
		// 2008-08-12 by zhw
		if (temp_ampl < LFO_Ampl_Per) //if(leadMode->aver_ampl_rat < LFO_Ampl_Per)  
		{
			// ��ʱ�˳��澯
			if (osciFlag->times_dn > -10)
			{
				osciFlag->times_dn = -11;
			}
			else
			{
				osciFlag->times_dn -= 1;
			}

			if (osciFlag->times_dn < (-10 - LFO_Times) && leadMode->damping > Out_LFO_Warn_Damping)
			{
				//�����־λ
				osciFlag->lackdamp_warn = 0;
				osciFlag->times_ldw = 0;
				osciFlag->damp_norm = 0;
				//osciFlag->times_dn      = 0;
				//osciFlag->lfo_warn      = 0;
				osciFlag->lfo_forewarn = 0;

				// 2005-12-23  ��ռ�����
				osciFlag->times_dn = -10;
			}

			//2006-1-18  ��ֹ�������񵴲��˳�
			if (osciFlag->times_dn < (-10 - LFO_Times) && (temp_ampl < LFO_Ampl_Per_Fir) && (leadMode->damping >= 0))
			{
				//�����־λ
				osciFlag->lackdamp_warn = 0;
				osciFlag->times_ldw = 0;
				osciFlag->damp_norm = 0;
				//osciFlag->times_dn      = 0;
				//osciFlag->lfo_warn      = 0;
				osciFlag->lfo_forewarn = 0;

				// 2005-12-23  ��ռ�����
				osciFlag->times_dn = -10;
			}
			else
			{
				//2006-6-5  ��ֹ�������񵴲��˳�
				if (osciFlag->times_dn < (-10 - (2 * LFO_Times)))
				{
					//�����־λ
					osciFlag->lackdamp_warn = 0;
					osciFlag->times_ldw = 0;
					osciFlag->damp_norm = 0;
					//osciFlag->times_dn      = 0;
					//osciFlag->lfo_warn      = 0;
					osciFlag->lfo_forewarn = 0;

					// 2005-12-23  ��ռ�����
					osciFlag->times_dn = -10;
				}
			}

			if (osciFlag->lfo_warn == 0)
			{
				// 2008-05-11 by zhw 0.05 0.08

				if (temp_freq_margin < SameFreqDifVal)
				{
					sprintf(m_LogString, "��ֵ:%f[%f],Ƶ��:%f[%f],������,������", temp_ampl, LFO_Ampl_Per, temp_freq_margin, SameFreqDifVal);
					MessShow(m_LogString);
					//osciFlag->times_lw += 1;
				}
				else
				{
					osciFlag->times_lw = 0;
				}

			}
			else
			{
				if (leadMode->aver_ampl2 <= LFO_Ampl_Per)
				{
					osciFlag->times_lw += 1;
					if (osciFlag->times_lw > LFO_Times)
					{
						osciFlag->lfo_warn = 0;
						osciFlag->times_lw = 0;
					}
				}
				else
				{
					osciFlag->times_lw = 0;
				}
			}

			//  LFO  Forewarning  
			/*if(osciFlag->lfo_forewarn == 0)
			{
				if(temp_freq_margin<SameFreqDifVal && leadMode->damping <= LFO_Forewarn_Damping && leadMode->damping < osciFlag->LM_damp)
				{
					osciFlag->times_lfw += 1;

					if(osciFlag->times_lfw >= LFO_Fore_Times)
					{
						if(leadMode->damping < 0) //Out_LFO_Warn_Damping
						{
							osciFlag->lfo_forewarn = 1;
							osciFlag->times_lfw    = 0;
						}
					}
				}
				else
				{
					osciFlag->times_lfw = 0;
				}
			}
			else
			{
				if(leadMode->damping > 0)
				{
					osciFlag->times_lfw += 1;
					if(osciFlag->times_lfw >= LFO_Fore_Times)
					{
						osciFlag->lfo_forewarn = 0;
						osciFlag->times_lfw    = 0;
					}
				}
				else
				{
					osciFlag->times_lfw = 0;
				}
			}*/
		}
		else  //�����񵴣���ֵ������
		{
			if (leadMode->amplitude < para_Ampl_aver * temp_ampl)
			{
				// 2008-10-22 by zhw for nw
				osciFlag->times_lw = 0;
				osciFlag->times_lfw = 0;
				sprintf(m_LogString, "amplitude=[%6.2f]< [%6.2f]*[%6.2f],�������㣡", leadMode->amplitude, para_Ampl_aver, temp_ampl);
				MessShow(m_LogString);
			}
			else
			{
				if (osciFlag->times_dn <= -10)
				{
					osciFlag->times_dn = 0;
				}

				//  LFO  Forewarning  �����ѷ���LFO���������ⷢԤ�� 2006-09-24
				if (osciFlag->lfo_warn == 0)
				{
					if (osciFlag->lfo_forewarn == 0)
					{
						if (osciFlag->times_lfw == 0)  // 2008-01-22 lsf ��һ�β��ж�Ƶ��
						{
							if (leadMode->damping <= LFO_Forewarn_Damping && leadMode->damping < osciFlag->LM_damp)  // ??
							{
								osciFlag->times_lfw += 1;

								if (osciFlag->times_lfw >= LFO_Fore_Times)
								{
									// 2006-09-24
									osciFlag->lfo_forewarn = 1;
									osciFlag->times_lfw = 0;
								}
							}
							else
							{
								osciFlag->times_lfw = 0;
							}
						}
						else
						{
							if (temp_freq_margin < SameFreqDifVal && leadMode->damping <= LFO_Forewarn_Damping && leadMode->damping < osciFlag->LM_damp)
							{
								osciFlag->times_lfw += 1;

								if (osciFlag->times_lfw >= LFO_Fore_Times)
								{
									// 2006-09-24
									osciFlag->lfo_forewarn = 1;
									osciFlag->times_lfw = 0;
								}
							}
							else
							{
								osciFlag->times_lfw = 0;
							}
						}
					}
				}
				else
				{
					osciFlag->lfo_forewarn = 0;
					osciFlag->times_lfw = 0;
				}

				//	LFO Warning
				if (osciFlag->lfo_warn == 0)
				{
					//if(temp_freq_margin<SameFreqDifVal && leadMode->damping < LFO_Warn_Damping)
					if (osciFlag->times_lw == 0)  // 2008-01-22 lsf ��һ�β��ж�Ƶ��
					{
						if (leadMode->damping < LFO_Warn_Damping)
						{
							osciFlag->times_lw += 1;

							if (osciFlag->times_lw >= LFO_Times)
							{
								osciFlag->lfo_warn = 1;
								osciFlag->times_lw = 0;
							}
						}
						else
						{
							osciFlag->times_lw = 0;
						}
					}
					else
					{
						if (temp_freq_margin < SameFreqDifVal && leadMode->damping < LFO_Warn_Damping)
						{
							osciFlag->times_lw += 1;

							if (osciFlag->times_lw >= LFO_Times)
							{
								osciFlag->lfo_warn = 1;
								osciFlag->times_lw = 0;
							}
						}
						else
						{
							osciFlag->times_lw = 0;
						}
					}

				}
				else if (osciFlag->lfo_warn == 1)
				{
					if (leadMode->damping >= LFO_Warn_Damping)
					{
						osciFlag->times_lw += 1;

						//if(osciFlag->times_lw >= LFO_Times+2)                
						//{
						//	osciFlag->lfo_warn = 0;
						//	osciFlag->times_lw = 0;
						//}
					}
					else if (leadMode->damping < LFO_Warn_Damping)
					{
						osciFlag->times_lw = 0;
					}
				}

				//  �����ѷ���LFO�����ⷢԤ����Ϣ 2006-09-24
				if (osciFlag->lfo_warn == 1)
				{
					osciFlag->lfo_forewarn = 0;
					osciFlag->times_lfw = 0;
				}

				// Lack damping Warning
				if (osciFlag->lackdamp_warn == 0)
				{
					if (leadMode->damping >= LFO_Warn_Damping && leadMode->damping <= LD_Warn_Damping)
					{
						osciFlag->times_ldw += 1;

						if (osciFlag->times_ldw >= LD_Times)
						{
							osciFlag->lackdamp_warn = 1;
							osciFlag->times_ldw = 0;
						}
					}
					else
					{
						osciFlag->times_ldw = 0;
					}
				}
				else if (osciFlag->lackdamp_warn == 1)
				{
					if (leadMode->damping > LD_Warn_Damping || leadMode->damping < LFO_Warn_Damping)
					{
						osciFlag->times_ldw += 1;

						if (osciFlag->times_ldw >= LD_Times + 2)
						{
							osciFlag->lackdamp_warn = 0;
							osciFlag->times_ldw = 0;
						}
					}
					else
					{
						osciFlag->times_ldw = 0;
					}
				}

				// damping normal
				if (osciFlag->damp_norm == 0)
				{
					if (leadMode->damping >= LD_Warn_Damping && leadMode->damping < 30)
					{
						osciFlag->times_dn += 1;

						if (osciFlag->times_dn >= DN_Times)
						{
							osciFlag->damp_norm = 1;
							osciFlag->times_dn = 0;
						}
					}
					else
					{
						osciFlag->times_dn = 0;
					}
				}
				else
				{
					if (leadMode->damping < LD_Warn_Damping || leadMode->damping >= 6)
					{
						osciFlag->times_dn += 1;

						if (osciFlag->times_dn >= LD_Times) // DN_Times �����᲻�����
						{
							osciFlag->damp_norm = 0;
							osciFlag->times_dn = 0;
						}
					}
					else
					{
						osciFlag->times_dn = 0;
					}

					//if(leadMode->damping == 100 && leadMode->aver_ampl_rat < 1.5*LFO_Ampl_Per)  
					if (leadMode->damping == 100 && temp_ampl < 1.5 * LFO_Ampl_Per)
					{
						osciFlag->damp_norm = 0;
					}
				}
			}

		}

		osciFlag->LM_frequency = leadMode->frequency;
		osciFlag->LM_damp = leadMode->damping;
	}
	//*/
	sprintf(m_LogString, "����������=%d, ������=%d, Ԥ��=%d, �澯=%d, Ƿ�����־=%d, Ԥ����־= %d, �澯��־=%d",
		osciFlag->times_dn, osciFlag->times_ldw, osciFlag->times_lfw, osciFlag->times_lw,
		osciFlag->lackdamp_warn, osciFlag->lfo_forewarn, osciFlag->lfo_warn);
	MessShow(m_LogString);
	return;
}

//	PERIOD_LEAD_MODE      m_pLfoModeCurve[LFO_CURVE_MAX_PNT];//  ����ģʽ����
// �γ�����ģʽ����
void CLfoAnaObj::FormMonModeCurce()
{
	// 2008-05-19 by zhw �������淵��
	if (m_RunCtrCom == MENU_LFO_PRO_CTL_SAVE)
	{
		sprintf(m_LogString, "FormMonModeCurce( ):: �������治���£����أ�");
		MessShow(m_LogString);
		return;
	}

	int  ii, jj;
	int  CalcWdsTime = m_LfoPara.time_leng - m_LfoPara.int_time;

	// �״�����
	if (m_pLfoModeCurve[0].bgn_t <= 0 && m_pLfoModeCurve[0].end_t <= 0)
	{
		sprintf(m_LogString, "�״�����ģʽ����");
		MessShow(m_LogString);
		for (ii = 0; ii < LFO_CURVE_MAX_PNT; ii++)
		{
			jj = LFO_CURVE_MAX_PNT - ii - 2;
			m_pLfoModeCurve[ii].bgn_t = m_CurGpsTime - jj * CalcWdsTime;
			m_pLfoModeCurve[ii].end_t = m_pLfoModeCurve[ii].bgn_t + m_LfoPara.time_leng;
			m_pLfoModeCurve[ii].ampl = 0;
			m_pLfoModeCurve[ii].freq = 0;
			//			m_pLfoModeCurve[ii].damp  = 100.0;
			// 			m_pLfoModeCurve[ii].damp  = m_LfoPara.max_damp;
			m_pLfoModeCurve[ii].phase = 0;
			m_pLfoModeCurve[ii].real = 0;
			m_pLfoModeCurve[ii].image = 0;
		}
	}
	// ǰ��
	for (ii = 0; ii < LFO_CURVE_MAX_PNT - 1; ii++)
	{
		jj = ii + 1;
		m_pLfoModeCurve[ii].bgn_t = m_pLfoModeCurve[jj].bgn_t;
		m_pLfoModeCurve[ii].end_t = m_pLfoModeCurve[jj].end_t;
		m_pLfoModeCurve[ii].ampl = m_pLfoModeCurve[jj].ampl;
		m_pLfoModeCurve[ii].freq = m_pLfoModeCurve[jj].freq;
		m_pLfoModeCurve[ii].damp = m_pLfoModeCurve[jj].damp;
		m_pLfoModeCurve[ii].phase = m_pLfoModeCurve[jj].phase;
		m_pLfoModeCurve[ii].real = m_pLfoModeCurve[jj].real;
		m_pLfoModeCurve[ii].image = m_pLfoModeCurve[jj].image;
	}
	// ���
	m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].end_t = m_CurGpsTime;
	m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].ampl = m_SysMon.mon_ampl;
	m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].freq = m_SysMon.mon_freq;
	sprintf(m_LogString, "����ģʽ����bgn_t=[%ld],end_t=[%ld],ampl=[%f],freq=[%f]",
		m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t, m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].end_t, m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].ampl, m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].freq);
	MessShow(m_LogString);
	// 		if (m_SysMon.mon_freq < ZEROVALUE )
	// 		{
	//             if( m_SysMon.stat >= MENU_LFO_DEV_STS_FWN )
	//             {
	//                 return;
	//             }
	// 			m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].freq  = 0;
	// //			m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].damp  = 100.0;
	// // 			m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].damp  = m_LfoPara.max_damp;
	// 		}
	// 		else
	// 		{
	// // 			m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].freq  = m_SysMon.mon_freq;
	// //     		m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].damp  = m_SysMon.mon_damp;
	// 		}

	return;
}
// ����ʵʱ������Ϣ
void CLfoAnaObj::UpdateMonInfToWAMAP()
{
	if (m_RunCtrCom == MENU_LFO_PRO_CTL_SAVE)
	{
		sprintf(m_LogString, "UpdateMonInfToWAMAP( ):: �������治���£����أ�");
		MessShow(m_LogString);
		return;
	}

	UpdateMonGnTable(m_nGnNum, m_pGnInf, m_pGnMon);
	UpdateMonLnTable(m_nLnNum, m_pLnInf, m_pLnMon);
	//	UpdateLnDevTableLfoStat( m_nLnNum, m_pLnMon );
	UpdateMonTrTable(m_nTrNum, m_pTrInf, m_pTrMon);
	UpdateMonFcTable(m_nFcNum, m_pFcInf, m_nGnNum, m_pGnMon, m_nTrNum, m_pTrMon);
	UpdateMonSysTable(m_SysMon);
	//    UpdateAppVariable(m_SysMon);
	//	UpdateMonModeCurce( m_pLfoModeCurve );
	//    LFOTriEmsSingle( m_SysMon );
	FormMonModeCurce();                                     //  �γ�ģʽ����

	UpdateMonModeCurce(m_pLfoModeCurve);

	DecideLfoCurveID();                                   //  ȷ��ʵʱ���߶�Ӧ�豸

	RealtimeCurveProcess();

	sprintf(m_LogString, "UpdateMonInfToWAMAP()���! ");
	MessShow(m_LogString);
	return;
}


/////////////////////////////////ana����/////////////////////////////////
//  ��������Ҫģʽͳ�� // case result -> m_pAllObjMode
//  int                 m_nAllObjMode; 
//  MODE_PART_OBJ       m_pAllObjMode[3*Obj_Num_Max];
void CLfoAnaObj::LfoClctMainMode()
{

	int ii, jj, kk;

	int nCddtMode = 0;
	MODE_PART_OBJ pCddtMode[MODE_NUM_MAX];
	int nMainMode = 0;
	MODE_PART_OBJ pMainMode[OBJ_MODE_MAX];

	//int dataSize = sizeof(MODE_PART_OBJ);
	int objType = OBJ_TYPE_GN;
	m_nAllObjMode = 0;
	memset((void*)m_pAllObjMode, 0, sizeof(MODE_PART_OBJ) * para_nObjMaxMode);
	int tmptableno;
	char tmpdevname[64];
	for (ii = 0; ii < m_nCases; ii++)
	{
		memset(tmpdevname, 0, sizeof(tmpdevname));
		if (m_pCases[ii].m_result.mode_num > 0)
		{
			tmptableno = PODB_GetTabelNoByDevId(m_pCases[ii].m_nID);
			if (tmptableno == ACLN_DOT_NO_TAB || tmptableno == ACLN_DEVICE_NO_TAB)
			{
				objType = OBJ_TYPE_LN;                      // type
				for (jj = 0; jj < m_nLnNum; jj++)
				{
					if (m_pCases[ii].m_nID == m_pLnInf[jj].acln_id)
					{
						sprintf(tmpdevname, "%s", m_pLnInf[jj].acln_name);
						break;
					}
				}
			}
			else if (tmptableno == GN_DEVICE_NO_TAB)
			{
				objType = OBJ_TYPE_GN;                      // type

				for (jj = 0; jj < m_nGnNum; jj++)
				{
					if (m_pCases[ii].m_nID == m_pGnInf[jj].gn_id)
					{
						sprintf(tmpdevname, "%s", m_pGnInf[jj].gn_name);
						break;
					}
				}
			}
			else if (tmptableno == TR_DEVICE_NO_TAB || tmptableno == TRWD_DEVICE_NO_TAB)
			{
				objType = OBJ_TYPE_TR;                      // type
				for (jj = 0; jj < m_nTrNum; jj++)
				{
					if (m_pCases[ii].m_nID == m_pTrInf[jj].tr_id)
					{
						sprintf(tmpdevname, "%s", m_pTrInf[jj].tr_name);
						break;
					}
				}
			}
#ifdef _NUSP
			else if (tmptableno == PLANT_INFO_NO_TAB || tmptableno == SUBSTATION_INFO_NO_TAB)
#else
			else if (tmptableno == FAC_INFO_NO_TAB)
#endif // _NUSP			
			{
				objType = OBJ_TYPE_FC;                      // type
				for (jj = 0; jj < m_nFcNum; jj++)
				{
					if (m_pCases[ii].m_nID == m_pFcInf[jj].fac_id)
					{
						sprintf(tmpdevname, "%s", m_pFcInf[jj].fac_name);
						break;
					}
				}
			}
			else
			{
				sprintf(m_LogString, "�����ĵ�%d��Case���ͺ�["INT_REC_FMT"]����!", ii, INT_REC_VAL(m_pCases[ii].m_nID));
				MessShow(m_LogString);
				continue;
			}

			// initial pCddtMode and pMainMode
			nCddtMode = 0;
			nMainMode = 0;
			memset((void*)pCddtMode, 0, sizeof(MODE_PART_OBJ) * MODE_NUM_MAX);
			memset((void*)pMainMode, 0, sizeof(MODE_PART_OBJ) * OBJ_MODE_MAX);

			// fill m_pAllObjMode
			nCddtMode = m_pCases[ii].m_result.mode_num;
			//if (nCddtMode > MODE_NUM_MAX ) nCddtMode = MODE_NUM_MAX;
			if (nCddtMode > para_nSysMaxMode) nCddtMode = para_nSysMaxMode;
			for (jj = 0; jj < nCddtMode; jj++)
			{
				pCddtMode[jj].obj_id = m_pCases[ii].m_nID;              // obj_id
				sprintf(pCddtMode[jj].obj_name, "%s", tmpdevname);
				pCddtMode[jj].type = objType;                           // type
				pCddtMode[jj].damp = m_pCases[ii].m_result.mode[jj][0]; // �����
				pCddtMode[jj].freq = m_pCases[ii].m_result.mode[jj][1]; // Ƶ��
				pCddtMode[jj].ampl = m_pCases[ii].m_result.mode[jj][2]; // ���
				pCddtMode[jj].phas = m_pCases[ii].m_result.mode[jj][3]; // ����
				pCddtMode[jj].real = m_pCases[ii].m_result.mode[jj][4]; // ʵ��(˥������)
				pCddtMode[jj].imag = m_pCases[ii].m_result.mode[jj][5]; // �鲿
				pCddtMode[jj].engy = m_pCases[ii].m_result.mode[jj][6]; // engy
			}

			// 			g_theLfoAna.IdtfMainMode( nCddtMode, pCddtMode, nMainMode, pMainMode, m_LfoPara.sm_md_freq, m_LfoPara.sm_md_damp );

			if (nMainMode > 0)
			{
				if (nMainMode >= (OBJ_MODE_MAX + OBJ_MODE_MAX))
				{
					sprintf(m_LogString, "����õ���ObjMode Num[%d] > ObjMode MaxNum[%d],���Զ���Mode!", nMainMode, para_nObjMaxMode);
					MessShow(m_LogString);
					nMainMode = para_nObjMaxMode;
				}
				for (kk = 0; kk < nMainMode; kk++)
				{
					g_theLfoAna.MemCpyModePartObjStru(pMainMode[kk], m_pAllObjMode[m_nAllObjMode]);
					m_nAllObjMode += 1;
				}
			}
		}
	}

	// 	int                  m_nMonMode;	                    //  mon ģʽ��
	// 	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon ģʽ
		//  ͳ����·��Ϣ 2007-12
	for (ii = 0; ii < m_nMonMode; ii++)
	{
		if (m_pMonMode[ii].mode_num > 0)
		{
			memset(tmpdevname, 0, sizeof(tmpdevname));
			tmptableno = PODB_GetTabelNoByDevId(m_pMonMode[ii].obj_id);
			if (tmptableno == ACLN_DOT_NO_TAB || tmptableno == ACLN_DEVICE_NO_TAB)
			{
				objType = OBJ_TYPE_LN;                      // type
				for (jj = 0; jj < m_nLnNum; jj++)
				{
					if (m_pMonMode[ii].obj_id == m_pLnInf[jj].acln_id)
					{
						sprintf(tmpdevname, "%s", m_pLnInf[jj].acln_name);
						break;
					}
				}
			}
			else if (tmptableno == GN_DEVICE_NO_TAB)
			{
				continue; ////objType = OBJ_TYPE_GN ;                      // type
			}
#ifdef _NUSP
			else if (tmptableno == PLANT_INFO_NO_TAB || tmptableno == SUBSTATION_INFO_NO_TAB)
#else
			else if (tmptableno == FAC_INFO_NO_TAB)
#endif // _NUSP		
			{
				continue; ////objType = OBJ_TYPE_FC ;                      // type
			}
			else if (tmptableno == TR_DEVICE_NO_TAB)
			{
				//continue; // 2008-09-24
				objType = OBJ_TYPE_TR;                      // type
				for (jj = 0; jj < m_nTrNum; jj++)
				{
					if (m_pMonMode[ii].obj_id == m_pTrInf[jj].tr_id)
					{
						sprintf(tmpdevname, "%s", m_pTrInf[jj].tr_name);
						break;
					}
				}
			}
			else
			{
				sprintf(m_LogString, "���ӵĵ�%d��Case���ͺ�["INT_REC_FMT"]����!", ii, INT_REC_VAL(m_pMonMode[ii].obj_id));
				MessShow(m_LogString);
				continue;
			}

			// initial pCddtMode and pMainMode
			nCddtMode = 0;
			nMainMode = 0;
			memset((void*)pCddtMode, 0, sizeof(MODE_PART_OBJ) * MODE_NUM_MAX);
			memset((void*)pMainMode, 0, sizeof(MODE_PART_OBJ) * OBJ_MODE_MAX);

			// fill m_pAllObjMode
			nCddtMode = m_pMonMode[ii].mode_num; // .mode_num;
			sprintf(m_pMonMode[ii].obj_name, "%s", tmpdevname);
			if (nCddtMode > para_nSysMaxMode) nCddtMode = para_nSysMaxMode;
			for (jj = 0; jj < nCddtMode; jj++)
			{
				pCddtMode[jj].obj_id = m_pMonMode[ii].obj_id;    // obj_id
				sprintf(pCddtMode[jj].obj_name, "%s", tmpdevname);
				pCddtMode[jj].type = objType;                    // type
				pCddtMode[jj].damp = m_pMonMode[ii].mode[jj][0]; // �����
				pCddtMode[jj].freq = m_pMonMode[ii].mode[jj][1]; // Ƶ��
				pCddtMode[jj].ampl = m_pMonMode[ii].mode[jj][2]; // ���
				pCddtMode[jj].phas = m_pMonMode[ii].mode[jj][3]; // ����
				pCddtMode[jj].real = m_pMonMode[ii].mode[jj][4]; // ʵ��(˥������)
				pCddtMode[jj].imag = m_pMonMode[ii].mode[jj][5]; // �鲿
				pCddtMode[jj].engy = m_pMonMode[ii].mode[jj][6]; // engy
			}

			// 			g_theLfoAna.IdtfMainMode( nCddtMode, pCddtMode, nMainMode, pMainMode, m_LfoPara.sm_md_freq, m_LfoPara.sm_md_damp );

			if (nMainMode > 0)
			{
				if (nMainMode >= (OBJ_MODE_MAX + OBJ_MODE_MAX))
				{
					sprintf(m_LogString, "����õ���ObjMode Num[%d] > ObjMode MaxNum[%d],���Զ���Mode!", nMainMode, para_nObjMaxMode);
					MessShow(m_LogString);
					nMainMode = para_nObjMaxMode;
				}
				for (kk = 0; kk < nMainMode; kk++)
				{
					g_theLfoAna.MemCpyModePartObjStru(pMainMode[kk], m_pAllObjMode[m_nAllObjMode]);
					m_nAllObjMode += 1;
				}
			}
		}
	}

	return;
}


// 
void CLfoAnaObj::LfoAnaEEACIdentifyCluster()
{

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ģʽ���뷢���Ԥ����
//  ������� : �����������ǰ����Tj����factλ
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : nGnNum                  �����ݻ�����[in]
//             pGnData[]               �����ݻ���[in]
//             nPartGn                 ���������[in]
//             pPartGn[]               �������[in]
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaObj::LfoAnaPartGenPreHandle(int nGnNum, SSO_GN_INF* pGnInf, int nPartGn, MODE_PART_OBJ* pPartGn)
{
	if (nPartGn < 0 || !pPartGn || nGnNum <= 0 || !pGnInf) return;

	int  ii, jj;
	for (ii = 0; ii < nPartGn; ii++)
	{
		for (jj = 0; jj < nGnNum; jj++)
		{
			if (pPartGn[ii].obj_id == pGnInf[jj].gn_id)
			{
				// 				pPartGn[ii].fact = pGnInf[jj].emws;

				break;
			}
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : �������豸�Ĳ���������0
//  ������� : �����������ǰ����Tj����factλ������������Ӽ�����󣬽�factλ��0
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : iGnNum                  ������[in]
//             PartGn[]                ���[in]
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaObj::LfoAnaSetPartFact0(int nPartGn, MODE_PART_OBJ* pPartGn)
{
	if (nPartGn < 0 || !pPartGn) return;

	int  ii;
	for (ii = 0; ii < nPartGn; ii++)
	{
		pPartGn[ii].fact = 0;
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ���ݳ�վ������·����Ⱥ
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : nPartLn                  ��·��
//             pPartLn[]                ��·��Ϣ
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaObj::LfoAnaSetLnClustbyFc(int nPartLn, MODE_PART_OBJ* pPartLn, int nPartFc, MODE_PART_OBJ* pPartFc)
{
	if (nPartLn < 0 || !pPartLn) return;
	if (nPartFc < 0 || !pPartFc) return;

	int  ii, jj, kk;
	INT_REC  temp_Fc_id; ZERO_INT_REC(temp_Fc_id);
	for (ii = 0; ii < nPartLn; ii++)
	{
		ZERO_INT_REC(temp_Fc_id);
		for (jj = 0; jj < m_nLnNum; jj++)
		{
			if (pPartLn[ii].obj_id == m_pLnMon[jj].ln_id)
			{
				if (m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_1)
					temp_Fc_id = m_pLnInf[jj].fac_id_1;
				else if (m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_2)
					temp_Fc_id = m_pLnInf[jj].fac_id_2;
				else
					ZERO_INT_REC(temp_Fc_id);
				break;
			}
		}
		for (kk = 0; kk < nPartFc; kk++)
		{
			if (VALID_INT_REC(temp_Fc_id) && temp_Fc_id == pPartFc[kk].obj_id)
			{
				pPartLn[ii].clus = pPartFc[kk].clus;
				break;
			}
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ���ݳ�վ���ñ�ѹ������Ⱥ
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : nPartTr                  ��ѹ����
//             pPartTr[]                ��ѹ����Ϣ
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaObj::LfoAnaSetTrClustbyFc(int nPartTr, MODE_PART_OBJ* pPartTr, int nPartFc, MODE_PART_OBJ* pPartFc)
{
	if (nPartTr < 0 || !pPartTr) return;
	if (nPartFc < 0 || !pPartFc) return;

	int  ii, jj, kk;
	INT_REC temp_Fc_id; ZERO_INT_REC(temp_Fc_id);
	for (ii = 0; ii < nPartTr; ii++)
	{
		ZERO_INT_REC(temp_Fc_id);
		for (jj = 0; jj < m_nTrNum; jj++)
		{
			if (pPartTr[ii].obj_id == m_pTrInf[jj].tr_id)
			{
				temp_Fc_id = m_pTrInf[jj].fac_id;
				break;
			}
		}
		for (kk = 0; kk < nPartFc; kk++)
		{
			if (VALID_INT_REC(temp_Fc_id) && temp_Fc_id == pPartFc[kk].obj_id)
			{
				pPartTr[ii].clus = pPartFc[kk].clus;
				break;
			}
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ���ݼ�����Ϣ�������÷������Ӧģʽ�ķ�ֵ
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : nPartGn                  �������
//             pPartGn[]                �������Ϣ
////////////////////////////////////////////////////////////////////////////////
// 	int                  m_nMonMode;	                    //  mon ģʽ��
// 	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon ģʽ
void CLfoAnaObj::LfoAnaSetGnAmpl(int nPartGn, MODE_PART_OBJ* pPartGn)
{
	if (nPartGn < 0 || !pPartGn)    return;
	if (m_nMonMode < 0 || !m_pMonMode) return;

	int  ii, jj, kk;
	int  tempFindFlag = 0;
	int  tempSubIndex = -1;
	for (ii = 0; ii < nPartGn; ii++)
	{
		tempFindFlag = 0;
		tempSubIndex = -1;
		for (jj = 0; jj < m_nMonMode; jj++)
		{
			if (pPartGn[ii].obj_id == m_pMonMode[jj].obj_id)
			{
				for (kk = 0; kk < m_pMonMode[jj].mode_num; kk++)
				{
					// 					if ( g_theLfoAna.IdtfSameMode( pPartGn[ii].freq, pPartGn[ii].damp, m_pMonMode[jj].mode[kk][1], m_pMonMode[jj].mode[kk][0], m_LfoPara.sm_md_freq, m_LfoPara.sm_md_damp ) == true ) 
					// 					{
					// 						tempFindFlag = 1;
					// 						tempSubIndex = kk;
					// 						break;
					// 					}
				}
				if (tempFindFlag == 1)
				{
					pPartGn[ii].ampl = m_pMonMode[jj].mode[tempSubIndex][2];
				}
				break;
			}
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ͳһ���м���Ԫ����׼������fact
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : pSltMode[]              ģʽ��Ϣ
////////////////////////////////////////////////////////////////////////////////
// 	int                  m_nMonMode;	                    //  mon ģʽ��
// 	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon ģʽ
void CLfoAnaObj::LfoAnaResetFact(MODE_CHRCT* pSltMode)
{
	if (!pSltMode)   return;

	int   ii;
	int   tempMaxType = -1;
	INT_REC  tempMaxObjid; ZERO_INT_REC(tempMaxObjid);
	float tempMaxAmpl = 1.0f;
	for (ii = 0; ii < pSltMode->nPartGn; ii++)
	{
		// 2008-12-11 by zhw �ڷ�Ⱥ���(�ٽ�Ⱥ�����Ⱥ,����������Ⱥ)��ѡ���ֵ�����豸,��Ϊ�������ӻ�׼,
		// �����ƿ���Ϊ��,�������豸��ѡ���ֵ�����豸
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartGn[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		if (tempMaxAmpl < pSltMode->PartGn[ii].ampl)
		{
			tempMaxAmpl = pSltMode->PartGn[ii].ampl;
			tempMaxType = OBJ_TYPE_GN;
			tempMaxObjid = pSltMode->PartGn[ii].obj_id;
		}
	}
	for (ii = 0; ii < pSltMode->nPartLn; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartLn[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		if (tempMaxAmpl < pSltMode->PartLn[ii].ampl)
		{
			tempMaxAmpl = pSltMode->PartLn[ii].ampl;
			tempMaxType = OBJ_TYPE_LN;
			tempMaxObjid = pSltMode->PartLn[ii].obj_id;
		}
	}
	for (ii = 0; ii < pSltMode->nPartTr; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartTr[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		if (tempMaxAmpl < pSltMode->PartTr[ii].ampl)
		{
			tempMaxAmpl = pSltMode->PartTr[ii].ampl;
			tempMaxType = OBJ_TYPE_TR;
			tempMaxObjid = pSltMode->PartTr[ii].obj_id;
		}
	}
	// reset fact
	for (ii = 0; ii < pSltMode->nPartGn; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartGn[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		pSltMode->PartGn[ii].fact = pSltMode->PartGn[ii].ampl / tempMaxAmpl;
	}
	for (ii = 0; ii < pSltMode->nPartLn; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartLn[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		pSltMode->PartLn[ii].fact = pSltMode->PartLn[ii].ampl / tempMaxAmpl;
	}
	for (ii = 0; ii < pSltMode->nPartTr; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartTr[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		pSltMode->PartTr[ii].fact = pSltMode->PartTr[ii].ampl / tempMaxAmpl;
	}
	// ��׼�豸��
	char  tempMaxObjname[64];
	strcpy(tempMaxObjname, "");
	if (tempMaxType == OBJ_TYPE_GN)
	{
		for (ii = 0; ii < m_nGnNum; ii++)
		{
			if (tempMaxObjid == m_pGnInf[ii].gn_id)
			{
				strcpy(tempMaxObjname, m_pGnInf[ii].fac_name);
				strcat(tempMaxObjname, m_pGnInf[ii].gn_name);
				break;
			}
		}
	}
	else if (tempMaxType == OBJ_TYPE_LN)
	{
		for (ii = 0; ii < m_nLnNum; ii++)
		{
			if (tempMaxObjid == m_pLnInf[ii].acln_id)
			{
				strcpy(tempMaxObjname, m_pLnInf[ii].acln_name);
				break;
			}
		}
	}
	else if (tempMaxType == OBJ_TYPE_TR)
	{
		for (ii = 0; ii < m_nTrNum; ii++)
		{
			if (tempMaxObjid == m_pTrInf[ii].tr_id)
			{
				strcpy(tempMaxObjname, m_pTrInf[ii].fac_name);
				strcat(tempMaxObjname, m_pTrInf[ii].tr_name);
				break;
			}
		}
	}
	if (para_DebugInfo == 1)
	{
		sprintf(m_LogString, "fact��׼�豸��["INT_REC_FMT"][%s][%f]��", INT_REC_VAL(tempMaxObjid), tempMaxObjname, tempMaxAmpl);
		MessShow(m_LogString);
	}

	return;
}

// ����pcoa
void CLfoAnaObj::CalcGenClustorPcoi()
{

	return;
}

// ����pcoa
void CLfoAnaObj::CalcGenOpstPcoiCurv()
{

	return;
}


//  m_nSltMode, m_pSltMode
//	memset((void *)m_pFcAna, 0, sizeof(LFO_FC_ANA) * para_nFcMaxMem );
// // ���߷�����Ϣ���
void CLfoAnaObj::UpdateAnaInfToWAMAP()
{

	return;
}

/////////////////////////////////FormResultStruct/////////////////////////////////
// ��ʼ���������ṹ
//      LFO_SYS_RES_SAVE     m_SysResSave;                      //  ϵͳ���
//  	LFO_OBJ_RES_SAVE *   m_pLnResSave;                      //  �����豸���
//  	LFO_OBJ_RES_SAVE *   m_pGnResSave;                      //  �����豸���
//  	LFO_OBJ_RES_SAVE *   m_pTrResSave;                      //  �����豸���
void CLfoAnaObj::InitLfoResultSaveStruct()
{
	// mon
	memset((void*)&m_SysResSave, 0, sizeof(LFO_SYS_RES_SAVE));
	memset((void*)m_pGnResSave, 0, para_nGnMaxMem * sizeof(LFO_OBJ_RES_SAVE));
	memset((void*)m_pLnResSave, 0, para_nLnMaxMem * sizeof(LFO_OBJ_RES_SAVE));
	memset((void*)m_pTrResSave, 0, para_nTrMaxMem * sizeof(LFO_OBJ_RES_SAVE));
	m_nLfoMode = 0;                        //  LFO�ڼ�ģʽ����
	memset((void*)m_pLfoMode, 0, Period_Mode_Max * sizeof(MODE_CHRCT));
	memset((void*)m_pLfoWds, 0, Period_Mode_Max * sizeof(LFO_CALC_WDS));

	int i;
	for (i = 0; i < m_nLnNum; i++)               //��·
	{
		m_pLnResSave[i].obj_id = m_pLnInf[i].acln_id;
		strcpy(m_pLnResSave[i].obj_name, m_pLnInf[i].acln_name);
		if (m_pLnMon[i].slct_dot_id == m_pLnInf[i].dot_id_1)
		{
			m_pLnResSave[i].fac_id = m_pLnInf[i].fac_id_1;
			sprintf(m_pLnResSave[i].fac_name, "%s", m_pLnInf[i].fac_name_1);
		}
		else
		{
			m_pLnResSave[i].fac_id = m_pLnInf[i].fac_id_2;
			sprintf(m_pLnResSave[i].fac_name, "%s", m_pLnInf[i].fac_name_2);
		}

		m_pLnResSave[i].obj_type = MENU_LFO_DEV_LINE;
	}
	for (i = 0; i < m_nGnNum; i++)               //����
	{
		m_pGnResSave[i].obj_id = m_pGnInf[i].gn_id;
		strcpy(m_pGnResSave[i].obj_name, m_pGnInf[i].gn_name);
		m_pGnResSave[i].fac_id = m_pGnInf[i].fac_id;
		sprintf(m_pGnResSave[i].fac_name, "%s", m_pGnInf[i].fac_name);
		m_pGnResSave[i].obj_type = MENU_LFO_DEV_GEN;
	}
	for (i = 0; i < m_nTrNum; i++)               //��ѹ��
	{
		m_pTrResSave[i].obj_id = m_pTrInf[i].tr_id;
		strcpy(m_pTrResSave[i].obj_name, m_pTrInf[i].tr_name);
		m_pTrResSave[i].fac_id = m_pTrInf[i].fac_id;
		sprintf(m_pTrResSave[i].fac_name, "%s", m_pTrInf[i].fac_name);
		m_pTrResSave[i].obj_type = MENU_LFO_DEV_TR;
	}
	return;
}

// ȫ���̷����������
void CLfoAnaObj::FormLfoResultProcess()
{
	if (m_FormResSaveStat == L_FST_FORM_RESULTE)//(0 ������ 1�������� 2���� 3ͳ�������)
	{
		InitLfoResultSaveStruct();
		FormLfoMonSaveResult();
		FormLfoAnaSaveResult();
		UpdateMonLfoToWAMAP();
		SendWarnToIA(L_FST_FORM_RESULTE);
		m_FormResSaveStat = L_SnD_FORM_RESULTE;
	}
	else if (m_FormResSaveStat == L_SnD_FORM_RESULTE && m_SysMon.stat == MENU_LFO_DEV_STS_WRN)
	{
		FormLfoMonSaveResult();
		FormLfoAnaSaveResult();
		UpdateMonLfoToWAMAP();
		SendWarnToIA(L_SnD_FORM_RESULTE);
		//���� Խ���˳�
		if (m_nLfoMode >= (Period_Mode_Max - 1))
		{
			sprintf(m_LogString, "m_nLfoMode=%d,Խ�紦��", m_nLfoMode);
			MessShow(m_LogString, LTINFO);

			SaveResultFileTxt();

			UpdateLfoEvent();

			UpdateHisCase();

			UpdateHisShape();

			SaveLfoCaseByTmd();

			SendHisLfoMess();

			LFOTriggerSample();

			SaveCaseFile();

			m_FormResSaveStat = L_FST_FORM_RESULTE;

			InitLfoResultSaveStruct();

			sprintf(m_LogString, "m_nLfoMode=%d,�����³�ʼ��", m_nLfoMode);
			MessShow(m_LogString, LTINFO);
		}

	}
	else if (m_FormResSaveStat == L_END_FORM_RESULTE)
	{
		FormLfoMonSaveResult();
		FormLfoAnaSaveResult();
		SaveResultFileTxt();
		//�������ݿ�LFO�������
		UpdateMonLfoToWAMAP();

		// �񵴽��������LFO���¼���дʵʱ��,TODO:��¼����һ��ֵ���
		UpdateLfoEvent();

		// ����LFOʵʱCASE���ڱ�
		// дʵʱ�⣬�񵴽������������д�����豸��Ϣ��ֻ�������һ�����豸��
		UpdateHisCase();

		// ����LFOʵʱCASEģ̬ȫ��
		// дʵʱ�⣬�񵴽������������д�����豸��Ϣ��ֻ�������һ�����豸��
		UpdateHisShape();

		SendWarnToIA(L_END_FORM_RESULTE);

		int UpdateFlag = 0;
#ifdef _WAMAP_
#ifndef  _WINDOWS32 
		if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
		{
			UpdateFlag = 0;
			sprintf(m_LogString, "��������WAMAP����,�˳����ֲ���");
			MessShow(m_LogString, LTINFO);
		}
		else
		{
			UpdateFlag = 1;
		}
#endif
#endif
		if (UpdateFlag == 1)
		{
			// ����TMD
			SaveLfoCaseByTmd();
			SendHisLfoMess();

			// LFO����
			LFOTriggerSample();

			// ���ɰ������潻���ļ�
			SaveCaseFile();

			m_FormResSaveStat = L_NOT_FORM_RESULTE;
			// �ٻ�100֡�ļ���־
			// �����߳�
			if (para_IsCallDynFile == 1)
			{
				sprintf(m_LogString, "�����߳��ٻ�100֡�ļ�CallDynFileTMain: %d", para_IsCallDynFile);
				MessShow(m_LogString);
#ifdef _WINDOWS32
				HANDLE hThrd;
				unsigned int threadId;

				hThrd = (HANDLE)_beginthreadex(NULL, 0, CallDynFileTMain, NULL, 0, &threadId);
				if (hThrd == NULL)
				{
					sprintf(m_LogString, "QTSATThread()�̴߳���ʧ��");
					MessShow(m_LogString);
				}
#else
				pthread_t pt = 0;
				pthread_attr_t thd_attr;
				pthread_attr_init(&thd_attr);
				pthread_attr_setdetachstate(&thd_attr, PTHREAD_CREATE_JOINABLE);
				pthread_attr_setstacksize(&thd_attr, 67108864);

				int ret = pthread_create(&pt, &thd_attr, CallDynFileTMain, NULL);
				if (ret != 0)
				{
					sprintf(m_LogString, "QTSATThread()�̴߳���ʧ��");
					MessShow(m_LogString);
				}
				else
				{
					//pthread_join(pt,NULL);
					pthread_attr_destroy(&thd_attr);
				}
#endif
			}
			else
			{
				sprintf(m_LogString, "�������ٻ�100֡�ļ�CallDynFileTMain: %d", para_IsCallDynFile);
				MessShow(m_LogString);
			}
		}
	}

	SendLfoResFile();
	return;
}
// Period_Mode_Max   - Create >> m_pLfoMode, m_pLfoWds
// 	m_nLfoMode   = 0 ;                        //  LFO�ڼ�ģʽ����
// 	memset((void *)m_pLfoMode, 0, Period_Mode_Max * sizeof(MODE_CHRCT));
// 	memset((void *)m_pLfoWds,  0, Period_Mode_Max * sizeof(LFO_CALC_WDS));
// �γ�ana�������ṹ(���㴰����Ϣ��ģ̬ȫ��Ϣ)
void CLfoAnaObj::FormLfoAnaSaveResult()
{
	// 	int            ii, jj ,kk;
	// 	char           LogString[MAXBUF];  //  �ַ���
	// 	LFO_CALC_WDS   temp_CalcWds ;
	// 	memset((void *)&temp_CalcWds, 0, sizeof(LFO_CALC_WDS));
	// 
	// 	if(m_FormResSaveStat == L_FST_FORM_RESULTE) //(0 ������ 1�������� 2���� 3ͳ�������)
	// 	{	// ������㴰����Ϣ
	// 		temp_CalcWds.wds_no       = m_nLfoMode+1;	     
	// 		temp_CalcWds.wds_bgn      = m_CalcStartTime;
	// 		temp_CalcWds.wds_end      = m_CurGpsTime;
	// 		temp_CalcWds.mode_num     = 1;
	// 		temp_CalcWds.real1        = m_DispMode.real;	     
	// 		temp_CalcWds.imag1        = m_DispMode.imag;	     
	// 		temp_CalcWds.freq1        = m_DispMode.freq;	     
	// 		temp_CalcWds.damp1        = m_DispMode.damp;	     
	// 		temp_CalcWds.real2        = m_SysMon.mon_real;	     
	// 		temp_CalcWds.imag2        = m_SysMon.mon_imag;	     
	// 		temp_CalcWds.freq2        = m_SysMon.mon_freq;	     
	// 		temp_CalcWds.damp2        = m_SysMon.mon_damp;	     
	// 		temp_CalcWds.real3        = 0;	     
	// 		temp_CalcWds.imag3        = 0;	     
	// 		temp_CalcWds.freq3        = 0;	     
	// 		temp_CalcWds.damp3        = 0;	     
	// 		temp_CalcWds.is_mode_show = MENU_LFO_MON_YES;	
	// 		if ( m_nSltMode < 0 )
	// 		{
	// 			temp_CalcWds.is_mode_show = MENU_LFO_MON_NO;	
	// 		}
	// 
	// 		temp_CalcWds.occur_time   = m_CalcStartTime;
	// 		if ( m_DispMode.nPartGn > 0 )
	// 		{
	// 			sprintf( LogString, "�������:%d̨,", m_DispMode.nPartGn );
	// 			strcat( temp_CalcWds.content, LogString );
	// 			for ( ii=0; ii<m_nGnNum; ii++ )
	// 			{
	// 				if ( m_pGnInf[ii].gn_id == m_DispMode.PartGn[0].obj_id )
	// 				{
	// 					if (para_ShowFacName == 1)
	// 					{
	// 						sprintf( LogString, "%s%s;", m_pGnInf[ii].fac_name, m_pGnInf[ii].gn_name );
	// 					}
	// 					else
	// 					{
	// 						sprintf( LogString, "%s;", m_pGnInf[ii].gn_name );
	// 					}
	// 					
	// 					strcat(temp_CalcWds.content, LogString );
	// 					break;
	// 				}
	// 			}
	// 
	//             sprintf( m_SysAna.ana_detail, "�������:%d̨,", m_DispMode.nPartGn );
	//             for( ii=0; ii<m_DispMode.nPartGn; ii++ )
	//             {
	//                 for( jj=0; jj<m_nGnNum; jj++)
	//                 {
	//                     if( m_pGnInf[jj].gn_id == m_DispMode.PartGn[ii].obj_id )
	//                     {
	//                         if( strlen( m_SysAna.ana_detail) > 128 ) 
	//                         {
	//                             break;
	//                         }
	// 						if (para_ShowFacName == 1)
	// 						{
	// 							sprintf( LogString, "%s%s;", m_pGnInf[jj].fac_name, m_pGnInf[jj].gn_name );
	// 						}
	// 						else
	// 						{
	// 							sprintf( LogString, "%s;", m_pGnInf[jj].gn_name );
	// 						}
	//                         
	// 					    strcat(m_SysAna.ana_detail, LogString );
	//                     }
	//                 }
	//             }
	//             strcat( m_SysAna.ana_detail, "\n\n");
	// 		}
	// 		else
	// 		{
	// 			strcat(temp_CalcWds.content, "������Χ�ڻ���δ����;" );
	//             strcat(m_SysAna.ana_detail, "������Χ�ڻ���δ����;");
	// 		}
	// 		//  ��·
	// 		if ( m_DispMode.nPartLn > 0 )
	// 		{
	// 			sprintf( LogString, "�ɹ���·:%d��,", m_DispMode.nPartLn );
	// 			strcat(temp_CalcWds.content, LogString );
	// 			for ( ii=0; ii<m_nLnNum; ii++ )
	// 			{
	// 				if ( m_pLnInf[ii].acln_id == m_DispMode.PartLn[0].obj_id )
	// 				{
	// 					sprintf( LogString, "%s;", m_pLnInf[ii].acln_name );
	// 					strcat(temp_CalcWds.content, LogString );
	// 					break;
	// 				}
	// 			}
	// 
	//             /*sprintf( LogString, "�ɹ���·:%d��,", m_DispMode.nPartLn );
	//             strcat( m_SysAna.ana_detail, LogString );
	//             for( ii=0; ii<m_DispMode.nPartLn; ii++ )
	//             {
	//                 for( jj=0; jj<m_nLnNum; jj++)
	//                 {
	//                     if( m_pLnInf[jj].acln_id == m_DispMode.PartLn[ii].obj_id )
	//                     {
	//                         sprintf( LogString, "%s;", m_pGnInf[jj].acln_name );
	//                         strcat(m_SysAna.ana_detail, LogString );
	//                     }
	//                 }
	//             }
	//             strcat( m_SysAna.ana_detail, "\n");*/
	// 		}
	// 		else
	// 		{
	// 			//strcat(temp_CalcWds.content, "" );
	// 		}
	// 		//  ��վ
	// 		if ( m_SysAna.center_id > 0 )
	// 		{
	// 			strcat(temp_CalcWds.content, "���ĳ�վ:" );
	// 			for ( ii=0; ii<m_nFcProDat; ii++ )
	// 			{
	// 				kk = 0;
	// 				if ( m_pFcAna[ii].lfo_is_cntr == MENU_LFO_MON_YES)
	// 				{
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].fac_id = m_pFcAna[ii].fac_id;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].is_cntr = m_pFcAna[ii].lfo_is_cntr;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].v_ampl = m_pFcAna[ii].lfo_v_ampl;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].v_aver = m_pFcAna[ii].lfo_v_aver;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].v_min = m_pFcAna[ii].lfo_v_min;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].clus = m_pFcAna[ii].lfo_clus;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].is_disp = m_pFcAna[ii].lfo_is_disp;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].v_max = m_pFcAna[ii].lfo_v_max;
	// 					for (jj=0; jj<m_nFcNum; jj++ )
	// 					{
	// 						if ( m_pFcAna[ii].fac_id == m_pFcInf[jj].fac_id )
	// 						{
	//                             strcpy(m_DispMode.LfoCenter[m_DispMode.nLfoCenter].fac_name,m_pFcInf[jj].fac_name);
	//                             m_DispMode.LfoCenter[m_DispMode.nLfoCenter].vlty_id = m_pFcInf[jj].vlty_id;
	// 							kk++;
	// 							if (kk == 1)
	// 							{
	// 								sprintf( LogString, "%s", m_pFcInf[jj].fac_name );
	// 							}
	// 							else
	// 							{
	// 								sprintf( LogString, ",%s", m_pFcInf[jj].fac_name );
	// 							}
	// 							strcat(temp_CalcWds.content, LogString );
	// 							break;
	// 						}
	// 					}
	//                     m_DispMode.nLfoCenter = m_DispMode.nLfoCenter + 1;
	// 				}
	// 			}
	// 		}
	// 		else
	// 		{
	// 			strcat(temp_CalcWds.content, "�޷�ȷ��������;" );
	// 		}
	// 
	//         if( m_DispMode.nPartFc > 0)
	//         {
	//             sprintf( LogString, "�ɹ۳�վ:%d��,", m_DispMode.nPartFc );
	//             strcat( m_SysAna.ana_detail, LogString );
	//             for( ii=0; ii<m_DispMode.nPartFc; ii++ )
	//             {
	//                 for( jj=0; jj<m_nFcNum; jj++)
	//                 {
	//                     if( strlen( m_SysAna.ana_detail) > 200 ) 
	//                     {
	//                         break;
	//                     }
	//                     if( m_pFcInf[jj].fac_id == m_DispMode.PartFc[ii].obj_id )
	//                     {
	//                         sprintf( LogString, "%s;", m_pFcInf[jj].fac_name);
	//                         strcat(m_SysAna.ana_detail, LogString );
	//                     }
	//                 }
	//             }
	//             strcat( m_SysAna.ana_detail, "\n");
	//         }
	//         else
	//         {
	//             strcat( m_SysAna.ana_detail, "�޷�ȷ��������!" );
	//         }
	// 		// �ж��Ƿ���ʾ
	// 		temp_CalcWds.is_ctx_show  = MENU_LFO_MON_YES;
	// 		strcpy(temp_CalcWds.center_desc, m_SysAna.center_desc);
	// 		UpdateCasWndsTable( 1, temp_CalcWds ,1) ; // �����¼
	// 
	//         UpdateAnaSysTable( m_SysAna );
	//         
	// 		//////////////////////////////////////////////////////////////////////////	
	// 		// ��¼������Ϣ
	// 		memcpy((void*)&m_pLfoWds[m_nLfoMode], (void*)&temp_CalcWds, sizeof(LFO_CALC_WDS));
	// 		//  ��¼ģ̬��Ϣ
	// 		memcpy((void*)&m_pLfoMode[m_nLfoMode], (void*)&m_DispMode, sizeof(MODE_CHRCT));
	// 		m_pLfoMode[m_nLfoMode].mode_id = m_nLfoMode + 1;
	// 		m_nLfoMode  += 1;
	// 	}
	// 	else if(m_FormResSaveStat == L_SnD_FORM_RESULTE && m_SysMon.stat == MENU_LFO_DEV_STS_WRN)
	// 	{
	// 		temp_CalcWds.wds_no       = m_nLfoMode+1;	     
	// 		temp_CalcWds.wds_bgn      = m_CalcStartTime;
	// 		temp_CalcWds.wds_end      = m_CurGpsTime;
	// 		temp_CalcWds.mode_num     = 1;
	// 		temp_CalcWds.real1        = m_DispMode.real;	     
	// 		temp_CalcWds.imag1        = m_DispMode.imag;	     
	// 		temp_CalcWds.freq1        = m_DispMode.freq;	     
	// 		temp_CalcWds.damp1        = m_DispMode.damp;	     
	// 		temp_CalcWds.real2        = m_SysMon.mon_real;	     
	// 		temp_CalcWds.imag2        = m_SysMon.mon_imag;	     
	// 		temp_CalcWds.freq2        = m_SysMon.mon_freq;	     
	// 		temp_CalcWds.damp2        = m_SysMon.mon_damp;	     
	// 		temp_CalcWds.real3        = 0;	     
	// 		temp_CalcWds.imag3        = 0;	     
	// 		temp_CalcWds.freq3        = 0;	     
	// 		temp_CalcWds.damp3        = 0;	     
	// 		temp_CalcWds.is_mode_show = MENU_LFO_MON_YES;	
	// 		if ( m_nSltMode < 0 )
	// 		{
	// 			temp_CalcWds.is_mode_show = MENU_LFO_MON_NO;	
	// 		}
	// 
	// 		temp_CalcWds.occur_time   = m_CalcStartTime;
	// 		if ( m_DispMode.nPartGn > 0 )
	// 		{
	// 			sprintf( LogString, "�������:%d̨,", m_DispMode.nPartGn );
	// 			strcat(temp_CalcWds.content, LogString );
	// 			for ( ii=0; ii<m_nGnNum; ii++ )
	// 			{
	// 				if ( m_pGnInf[ii].gn_id == m_DispMode.PartGn[0].obj_id )
	// 				{
	// 					if (para_ShowFacName == 1)
	// 					{
	// 						sprintf( LogString, "%s%s;", m_pGnInf[ii].fac_name, m_pGnInf[ii].gn_name );
	// 					}
	// 					else
	// 					{
	// 						sprintf( LogString, "%s;", m_pGnInf[ii].gn_name );
	// 					}
	// 					
	// 					strcat(temp_CalcWds.content, LogString );
	// 					break;
	// 				}
	// 			}
	// 
	//             sprintf( m_SysAna.ana_detail, "�������:%d̨,", m_DispMode.nPartGn );
	//             for( ii=0; ii<m_DispMode.nPartGn; ii++ )
	//             {
	//                 for( jj=0; jj<m_nGnNum; jj++)
	//                 {
	//                     if( strlen( m_SysAna.ana_detail) > 128 ) 
	//                     {
	//                         break;
	//                     }
	//                     if( m_pGnInf[jj].gn_id == m_DispMode.PartGn[ii].obj_id )
	//                     {
	// 						if (para_ShowFacName == 1)
	// 						{
	// 							sprintf( LogString, "%s%s;", m_pGnInf[jj].fac_name, m_pGnInf[jj].gn_name );
	// 						}
	// 						else
	// 						{
	// 							sprintf( LogString, "%s;", m_pGnInf[jj].gn_name );
	// 						}
	//                         
	//                         strcat(m_SysAna.ana_detail, LogString );
	//                     }
	//                 }
	//             }
	//             strcat( m_SysAna.ana_detail, "\n\n");
	// 		}
	// 		else
	// 		{
	// 			strcat(temp_CalcWds.content, "������Χ�ڻ���δ����;" );
	//             strcat(m_SysAna.ana_detail, "������Χ�ڻ���δ����;\n\n");
	// 		}
	// 		//  ��· //2008-04-07 ???
	// 		if ( m_DispMode.nPartLn > 0 )
	// 		{
	// 			sprintf( LogString, "�ɹ���·:%d��,", m_DispMode.nPartLn );
	// 			strcat(temp_CalcWds.content, LogString );
	// 			for ( ii=0; ii<m_nLnNum; ii++ )
	// 			{
	// 				if ( m_pLnInf[ii].acln_id == m_DispMode.PartLn[0].obj_id )
	// 				{
	// 					sprintf( LogString, "%s;", m_pLnInf[ii].acln_name );
	// 					strcat(temp_CalcWds.content, LogString );
	// 					break;
	// 				}
	// 			}
	// 
	//             /*sprintf( LogString, "�ɹ���·:%d��,", m_DispMode.nPartLn );
	//             strcat( m_SysAna.ana_detail, LogString );
	//             for( ii=0; ii<m_DispMode.nPartLn; ii++ )
	//             {
	//                 for( jj=0; jj<m_nLnNum; jj++)
	//                 {
	//                     if( m_pLnInf[jj].acln_id == m_DispMode.PartLn[ii].obj_id )
	//                     {
	//                         sprintf( LogString, "%s;", m_pGnInf[jj].acln_name );
	//                         strcat(m_SysAna.ana_detail, LogString );
	//                     }
	//                 }
	//             }
	//             strcat( m_SysAna.ana_detail, "\n");*/
	// 		}
	// 		else
	// 		{
	// 			//strcat(temp_CalcWds.content, "" );
	// 		}
	//         //  ��վ //2008-09-09 ???
	//         /*if ( m_DispMode.nPartFc > 0 )
	//         {
	//             sprintf( LogString, "�ɹ۳�վ:%d��,", m_DispMode.nPartFc );
	//             strcat(temp_CalcWds.content, LogString );
	//             for ( ii=0; ii<m_nFcNum; ii++ )
	//             {
	//                 if ( m_pFcInf[ii].fac_id == m_DispMode.PartFc[0].obj_id )
	//                 {
	//                     sprintf( LogString, "%s;", m_pFcInf[ii].fac_id );
	//                     strcat(temp_CalcWds.content, LogString );
	//                     break;
	//                 }
	//             }
	//         }
	//         else
	//         {
	//             //strcat(temp_CalcWds.content, "" );
	// 		}*/
	// 		//  ���ĳ�վ
	// 		if ( m_SysAna.center_id > 0 )
	// 		{
	// 			int kkk =0;
	// 			strcat(temp_CalcWds.content, "���ĳ�վ:" );
	// 			for ( ii=0; ii<m_nFcProDat; ii++ )
	// 			{
	// 				kkk =0;
	// 				if ( m_pFcAna[ii].lfo_is_cntr == MENU_LFO_MON_YES)
	// 				{
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].fac_id = m_pFcAna[ii].fac_id;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].is_cntr = m_pFcAna[ii].lfo_is_cntr;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].v_ampl = m_pFcAna[ii].lfo_v_ampl;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].v_aver = m_pFcAna[ii].lfo_v_aver;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].v_min = m_pFcAna[ii].lfo_v_min;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].clus = m_pFcAna[ii].lfo_clus;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].is_disp = m_pFcAna[ii].lfo_is_disp;
	//                     m_DispMode.LfoCenter[m_DispMode.nLfoCenter].v_max = m_pFcAna[ii].lfo_v_max;
	// 					for (jj=0; jj<m_nFcNum; jj++ )
	// 					{
	// 						if ( m_pFcAna[ii].fac_id == m_pFcInf[jj].fac_id )
	// 						{
	//                             strcpy(m_DispMode.LfoCenter[m_DispMode.nLfoCenter].fac_name,m_pFcInf[jj].fac_name);
	//                             m_DispMode.LfoCenter[m_DispMode.nLfoCenter].vlty_id = m_pFcInf[jj].vlty_id;
	// 							kkk++;
	// 							if (kkk == 1)
	// 							{
	// 								sprintf( LogString, "%s", m_pFcInf[jj].fac_name );
	// 							}
	// 							else
	// 							{
	// 								sprintf( LogString, "%s,", m_pFcInf[jj].fac_name );
	// 							}
	// 							strcat(temp_CalcWds.content, LogString );
	// 							break;
	// 						}
	// 					}
	//                     m_DispMode.nLfoCenter = m_DispMode.nLfoCenter + 1;
	// 				}
	// 			}
	// 		}
	// 		else
	// 		{
	// 			strcat(temp_CalcWds.content, "�޷�ȷ��������;" );
	// 		}
	// 
	//         if( m_DispMode.nPartFc > 0)
	//         {
	//             sprintf( LogString, "�ɹ۳�վ:%d��,", m_DispMode.nPartFc );
	//             strcat( m_SysAna.ana_detail, LogString );
	//             for( ii=0; ii<m_DispMode.nPartFc; ii++ )
	//             {
	//                 for( jj=0; jj<m_nFcNum; jj++)
	//                 {
	//                     if( m_pFcInf[jj].fac_id == m_DispMode.PartFc[ii].obj_id )
	//                     {
	//                         if( strlen( m_SysAna.ana_detail) > 200 ) 
	//                         {
	//                             break;
	//                         }
	//                         sprintf( LogString, "%s;", m_pFcInf[jj].fac_name);
	//                         strcat(m_SysAna.ana_detail, LogString );
	//                     }
	//                 }
	//             }
	//   //          strcat( m_SysAna.ana_detail, "\n");
	// 			strcat( m_SysAna.ana_detail, "!");
	//         }
	//         else
	//         {
	// //            strcat( m_SysAna.ana_detail, "�޷�ȷ�������ģ�\n" );
	// 			strcat( m_SysAna.ana_detail, "�޷�ȷ��������!" );
	//         }
	// 		// �ж��Ƿ���ʾ
	// 		if ( m_nLfoMode>=1 && strcmp( temp_CalcWds.content, m_pLfoWds[m_nLfoMode-1].content ) == 0 )
	// 		{
	// 			temp_CalcWds.is_ctx_show  = MENU_LFO_MON_NO;
	// 		}
	// 		else if( m_DispMode.nPartGn > 0 )
	// 		{
	// 			temp_CalcWds.is_ctx_show  = MENU_LFO_MON_YES;
	// 		}
	// 		strcpy(temp_CalcWds.center_desc, m_SysAna.center_desc);
	// 		UpdateCasWndsTable( 1, temp_CalcWds) ; // �����¼
	// 
	//         UpdateAnaSysTable( m_SysAna );
	// 		//////////////////////////////////////////////////////////////////////////	
	// 		// ��¼������Ϣ
	//         memcpy((void*)&m_pLfoWds[m_nLfoMode], (void*)&temp_CalcWds, sizeof(LFO_CALC_WDS));
	//         //  ��¼ģ̬��Ϣ
	//         memcpy((void*)&m_pLfoMode[m_nLfoMode], (void*)&m_DispMode, sizeof(MODE_CHRCT));
	// 		m_pLfoMode[m_nLfoMode].mode_id = m_nLfoMode + 1;
	//         m_nLfoMode  += 1;
	// 		//////////////////////////////////////////////////////////////////////////
	// 	}
	// 	else if(m_FormResSaveStat == L_END_FORM_RESULTE)
	// 	{
	// 		// ana
	// 		//UpdateCasWndsTable( 1, temp_CalcWds, 1) ; // �����¼
	//         UpdateAverFact( m_nLfoMode, m_pLfoMode );   // 2008-06-05 by zhw
	// 		//UpdateCasAllShapeTable( m_nLfoMode, m_pLfoMode ) ;
	// 	}

	return;
}


// �γ�mon�������ṹ
void CLfoAnaObj::FormLfoMonSaveResult()
{
	// 	int        i= 0 ,ii;
	// 	//float      pavAveValue = 0;
	// 	int        LfoLineNum  = 0 ;
	//     int        LfoGenNum   = 0;
	//     int        LfoTrNum    = 0;
	// 
	// 	if(m_FormResSaveStat == L_FST_FORM_RESULTE)//(0 ������ 1�������� 2���� 3ͳ�������)
	// 	{
	// 		//����ϵͳ������Ϣ
	// 		m_SysResSave.lfo_b_t         = m_CurGpsTime - m_LfoPara.time_leng;
	// 		m_SysResSave.lfo_e_t         = m_CurGpsTime;
	// 		m_SysResSave.hold_time = m_SysResSave.lfo_e_t - m_SysResSave.lfo_b_t;
	// 		m_SysResSave.obj_id          = m_ThisLfoInfo.obj_id;
	// 		m_SysResSave.fac_id          = m_ThisLfoInfo.fac_id;
	// 		sprintf(m_SysResSave.fac_name,"%s",m_ThisLfoInfo.fac_name);
	// 		strcpy(m_SysResSave.obj_name,  m_ThisLfoInfo.obj_name);
	// 		m_SysResSave.freq            = m_ThisLfoInfo.frequency;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].end_t = m_CurGpsTime;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].ampl  = m_SysMon.mon_ampl;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].freq  = m_SysMon.mon_freq;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].damp  = m_SysMon.mon_damp;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].phase = m_SysMon.mon_phas;
	// 		m_SysResSave.mode1_num += 1;
	// 		m_SysResSave.max_ampl = 0.0f;
	// 		// ����ƽ��Ƶ��
	// 		m_SysResSave.aver_freq = 0.0f;
	// 		for (ii =0; ii<m_SysResSave.mode1_num; ii++)
	// 		{
	// 			m_SysResSave.aver_freq += m_SysResSave.mode1[m_SysResSave.mode1_num].freq;
	// 		}
	// 		m_SysResSave.aver_freq = m_SysResSave.aver_freq / m_SysResSave.mode1_num;
	// 
	// 		int table_no = PODB_GetTabelNoByDevId(m_SysResSave.obj_id);
	// 		if(table_no == GN_DEVICE_NO_TAB)
	// 		{
	// 			m_SysResSave.obj_type        = MENU_LFO_DEV_GEN;
	// 		}
	// 		else if (table_no == ACLN_DOT_NO_TAB)
	// 		{
	// 			m_SysResSave.obj_type        = MENU_LFO_DEV_LINE;
	// 		}
	// 		else if (table_no == TR_DEVICE_NO_TAB)
	// 		{
	// 			m_SysResSave.obj_type        = MENU_LFO_DEV_TR;
	// 		}
	// 		else
	// 		{
	// 			m_SysResSave.obj_type        = 0;
	// 		}
	// 
	// 		//��������豸��Ϣ
	// 		for(i=0; i<m_nLnNum; i++)               //��·
	// 		{
	// // 			m_pLnResSave[i].obj_id         = m_pLnInf[i].acln_id;
	// // 			strcpy(m_pLnResSave[i].obj_name, m_pLnInf[i].acln_name);
	// // 			if (m_pLnMon[i].slct_dot_id == m_pLnInf[i].dot_id_1)
	// // 			{
	// // 				m_pLnResSave[i].fac_id         = m_pLnInf[i].fac_id_1;
	// // 				sprintf(m_pLnResSave[i].fac_name,"%s",m_pLnInf[i].fac_name_1);
	// // 			}
	// // 			else
	// // 			{
	// // 				m_pLnResSave[i].fac_id         = m_pLnInf[i].fac_id_2;
	// // 				sprintf(m_pLnResSave[i].fac_name,"%s",m_pLnInf[i].fac_name_2);
	// // 			}
	// // 			
	// // 			m_pLnResSave[i].obj_type       = MENU_LFO_DEV_LINE;
	// 			if(m_pLnMon[i].stat >= MENU_LFO_DEV_STS_SCT && m_pLnDat[i].stat == MENU_LFO_DEV_STS_OK )
	// 			{
	// 				m_pLnResSave[i].stat           = m_pLnMon[i].stat;
	//                 m_pLnResSave[i].b_aver_pqv0[0]   = m_pLnMon[i].aver_val;
	//                 m_pLnResSave[i].e_aver_pqv0[0]   = m_pLnMon[i].aver_val;
	// 				
	// 				if(m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 				{
	// 					if (m_pLnResSave[i].lfo_b_t <= 0)
	// 					{
	// 						m_pLnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 					}
	// 					
	// 					m_pLnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 					m_pLnResSave[i].hold_time      = m_pLnResSave[i].lfo_e_t - m_pLnResSave[i].lfo_b_t;
	// 					m_pLnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                     m_pLnResSave[i].b_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	//                     m_pLnResSave[i].e_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	// 					if (m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN )   //freq > 0)
	// 					{
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].ampl  = m_pLnMon[i].lead_ampl;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].freq  = m_pLnMon[i].lead_freq;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].damp  = m_pLnMon[i].lead_damp;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].phase = m_pLnMon[i].lead_phas;
	// 						m_pLnResSave[i].mode1_num += 1;
	// 					}
	//                 }
	//                 else
	//                 {
	//                     m_pLnResSave[i].lfo_b_t        = 0;
	// 					m_pLnResSave[i].lfo_e_t        = 0;
	// 					m_pLnResSave[i].hold_time      = 0;
	// 					m_pLnResSave[i].if_lfo         = MENU_LFO_MON_NO;
	//                 }
	// 
	//                 m_pLnResSave[i].max_value.value  =  m_pLnMon[i].max_val;
	//                 m_pLnResSave[i].max_value.time   =  m_pLnMon[i].max_val_t;
	//                 m_pLnResSave[i].min_value.value  =  m_pLnMon[i].min_val;
	//                 m_pLnResSave[i].min_value.time   =  m_pLnMon[i].min_val_t;
	//                 if( m_pLnMon[i].max_min_dif > m_pLnResSave[i].max_min_dif )
	//                 {
	//                     m_pLnResSave[i].max_min_dif      =  m_pLnMon[i].max_min_dif;  // ���ֵ��������
	//                 }
	// 			}
	// 		}
	// 		for(i=0; i<m_nGnNum; i++)               //����
	// 		{
	// // 			m_pGnResSave[i].obj_id         = m_pGnInf[i].gn_id;
	// // // 				strcpy(m_pGnResSave[i].obj_name, m_pGnInf[i].fac_name);
	// // // 				strcat(m_pGnResSave[i].obj_name, m_pGnInf[i].gn_name);
	// // 			strcpy(m_pGnResSave[i].obj_name, m_pGnInf[i].gn_name);
	// // 			m_pGnResSave[i].fac_id         = m_pGnInf[i].fac_id;
	// // 			sprintf(m_pGnResSave[i].fac_name,"%s",m_pGnInf[i].fac_name);
	// // 			m_pGnResSave[i].obj_type       = MENU_LFO_DEV_GEN;
	// 			if(m_pGnMon[i].stat >= MENU_LFO_DEV_STS_SCT && m_pGnDat[i].stat == MENU_LFO_DEV_STS_OK ) 	
	// 			{
	// 				m_pGnResSave[i].stat           = m_pGnMon[i].stat;
	// 				m_pGnResSave[i].b_aver_pqv0[0] = m_pGnMon[i].aver_val;
	// 				m_pGnResSave[i].e_aver_pqv0[0] = m_pGnMon[i].aver_val;
	// 				
	//                 // 2008-11-13
	// 				if(m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 				{
	// 					if (m_pGnResSave[i].lfo_b_t <=0 )
	// 					{
	// 						m_pGnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 					}
	// 					
	// 					m_pGnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 					m_pGnResSave[i].hold_time      = m_pGnResSave[i].lfo_e_t - m_pGnResSave[i].lfo_b_t;
	// 					m_pGnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                     m_pGnResSave[i].b_aver_pqv1[0] = m_pGnMon[i].aver_val;
	// 				    m_pGnResSave[i].e_aver_pqv1[0] = m_pGnMon[i].aver_val;
	// 					if (m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].ampl  = m_pGnMon[i].lead_ampl;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].freq  = m_pGnMon[i].lead_freq;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].damp  = m_pGnMon[i].lead_damp;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].phase = m_pGnMon[i].lead_phas;
	// 						m_pGnResSave[i].mode1_num += 1;
	// 					}
	//                 }
	//                 else
	//                 {
	//                     m_pGnResSave[i].lfo_b_t        = 0;
	// 					m_pGnResSave[i].lfo_e_t        = 0;
	// 					m_pGnResSave[i].hold_time      = 0;
	// 					m_pGnResSave[i].if_lfo         = MENU_LFO_MON_NO;
	//                 }
	//                 
	//                 m_pGnResSave[i].max_value.value  =  m_pGnMon[i].max_val;
	//                 m_pGnResSave[i].max_value.time   =  m_pGnMon[i].max_val_t;
	//                 m_pGnResSave[i].min_value.value  =  m_pGnMon[i].min_val;
	//                 m_pGnResSave[i].min_value.time   =  m_pGnMon[i].min_val_t;
	//                 if( m_pGnMon[i].max_min_dif > m_pGnResSave[i].max_min_dif )
	//                 {
	//                     m_pGnResSave[i].max_min_dif      =  m_pGnMon[i].max_min_dif;  // ���ֵ��������
	//                 }
	// 			}
	// 		}
	//         for(i=0; i<m_nTrNum; i++)               //��ѹ��
	//         {
	// // 			m_pTrResSave[i].obj_id         = m_pTrInf[i].tr_id;
	// // 			strcpy(m_pTrResSave[i].obj_name, m_pTrInf[i].tr_name);
	// // 			m_pTrResSave[i].fac_id         = m_pTrInf[i].fac_id;
	// // 			sprintf(m_pTrResSave[i].fac_name,"%s",m_pTrInf[i].fac_name);
	// // 			m_pTrResSave[i].obj_type       = MENU_LFO_DEV_TR;
	//             if(m_pTrMon[i].stat >= MENU_LFO_DEV_STS_SCT && m_pTrDat[i].stat == MENU_LFO_DEV_STS_OK )
	//             {
	//                 m_pTrResSave[i].stat           = m_pTrMon[i].stat;
	//                 m_pTrResSave[i].b_aver_pqv0[0] = m_pTrMon[i].aver_val;
	//                 m_pTrResSave[i].e_aver_pqv0[0] = m_pTrMon[i].aver_val;
	//                 
	//                 if(m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	//                 {
	// 					if (m_pTrResSave[i].lfo_b_t >= 0)
	// 					{
	// 						m_pTrResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 					}
	//                     
	// 					m_pTrResSave[i].lfo_e_t        = m_CurGpsTime;
	// 					m_pTrResSave[i].hold_time      = m_pTrResSave[i].lfo_e_t - m_pTrResSave[i].lfo_b_t;
	//                     m_pTrResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                     m_pTrResSave[i].b_aver_pqv1[0] = m_pTrMon[i].aver_val;
	//                     m_pTrResSave[i].e_aver_pqv1[0] = m_pTrMon[i].aver_val;
	// 					if (m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN )   //freq > 0)
	// 					{
	// 						m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 						m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 						m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].ampl  = m_pTrMon[i].lead_ampl;
	// 						m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].freq  = m_pTrMon[i].lead_freq;
	// 						m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].damp  = m_pTrMon[i].lead_damp;
	// 						m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].phase = m_pTrMon[i].lead_phas;
	// 						m_pTrResSave[i].mode1_num += 1;
	// 					}
	//                 }
	//                 else
	//                 {
	//                     m_pTrResSave[i].lfo_b_t        = 0;
	// 					m_pTrResSave[i].lfo_e_t        = 0;
	// 					m_pTrResSave[i].hold_time      = 0;
	//                     m_pTrResSave[i].if_lfo         = MENU_LFO_MON_NO;
	//                 }
	//                 
	//                 m_pTrResSave[i].max_value.value  =  m_pTrMon[i].max_val;
	//                 m_pTrResSave[i].max_value.time   =  m_pTrMon[i].max_val_t;
	//                 m_pTrResSave[i].min_value.value  =  m_pTrMon[i].min_val;
	//                 m_pTrResSave[i].min_value.time   =  m_pTrMon[i].min_val_t;
	//                 if( m_pTrMon[i].max_min_dif > m_pTrResSave[i].max_min_dif )
	//                 {
	//                     m_pTrResSave[i].max_min_dif      =  m_pTrMon[i].max_min_dif;  // ���ֵ��������
	//                 }
	//             }
	// 		}
	// 
	//         //////////////////////////////////////////////////////////////////////////		
	//         //����ϵͳ������Ϣ
	//         m_SysResSave.lfo_e_t         = m_CurGpsTime;
	//         m_SysResSave.hold_time       = m_SysResSave.lfo_e_t - m_SysResSave.lfo_b_t ;
	//         //m_SysResSave.freq            = m_ThisLfoInfo.frequency ; //��δ���		
	//         //ͳ�Ƴ����񵴵ļ�����·����
	//         LfoLineNum  = 0 ;
	//         float temp_value=0.0;
	//         //time_t temp_time;
	//         //memset( (void*)&temp_value,0,sizeof(VALUE_TIME_STRU));
	//         for(i=0; i<m_nLnNum; i++) 
	//         {
	//             if (m_pLnResSave[i].lfo_b_t > 0)
	//             {
	//                 m_pLnResSave[i].max_ampl = fabs(m_pLnResSave[i].max_value.value - m_pLnResSave[i].min_value.value);
	// 				if (m_pLnResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pLnResSave[i].max_ampl;
	// 				}
	//                 LfoLineNum += 1;
	//             }
	//         }
	//         m_SysResSave.lfo_line_num = LfoLineNum;
	//         
	//         LfoGenNum  = 0 ;
	//         for(i=0; i<m_nGnNum; i++) 
	//         {
	//             if (m_pGnResSave[i].lfo_b_t > 0)
	//             {
	//                 m_pGnResSave[i].max_ampl = fabs(m_pGnResSave[i].max_value.value - m_pGnResSave[i].min_value.value);
	// 				if (m_pGnResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pGnResSave[i].max_ampl;
	// 				}
	//                 LfoGenNum += 1;
	//             }
	//         }
	//         m_SysResSave.act_gen_num = LfoGenNum;
	//         
	//         for(i=0; i<m_nTrNum; i++) 
	//         {
	//             if (m_pTrResSave[i].lfo_b_t > 0 )
	//             {
	//                 m_pTrResSave[i].max_ampl = fabs(m_pTrResSave[i].max_value.value - m_pTrResSave[i].min_value.value);
	// 				if (m_pTrResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pTrResSave[i].max_ampl;
	// 				}
	//                 LfoTrNum += 1;
	//             }
	//         }
	//         m_SysResSave.lfo_tr_num = LfoTrNum;
	//         
	//         sprintf(m_SysResSave.lfo_case_des, "%s%s", PT_GetChineseTimeStringBySeconds(m_SysResSave.lfo_b_t), "ϵͳ���ֵ�Ƶ��");
	// 	}
	// 	else if(m_FormResSaveStat == L_SnD_FORM_RESULTE )//(0 ������ 1�������� 2���� 3ͳ�������)
	// 	{
	// 		//����ϵͳ������Ϣ
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].end_t = m_CurGpsTime;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].ampl  = m_SysMon.mon_ampl;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].freq  = m_SysMon.mon_freq;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].damp  = m_SysMon.mon_damp;
	// 		m_SysResSave.mode1[m_SysResSave.mode1_num].phase = m_SysMon.mon_phas;
	// 		m_SysResSave.mode1_num += 1;
	// 
	// 		// ����ƽ��Ƶ��
	// 		m_SysResSave.aver_freq = 0.0f;
	// 		for (ii =0; ii<m_SysResSave.mode1_num; ii++)
	// 		{
	// 			m_SysResSave.aver_freq += m_SysResSave.mode1[m_SysResSave.mode1_num].freq;
	// 		}
	// 		m_SysResSave.aver_freq = m_SysResSave.aver_freq / m_SysResSave.mode1_num;
	// 
	// 		//��������豸��Ϣ
	// 		for(i=0; i<m_nLnNum; i++)               //��·
	// 		{
	// 			//״̬�ж�
	// 			if (((m_pLnMon[i].stat     >= MENU_LFO_DEV_STS_SCT && m_pLnMon[i].stat     <= MENU_LFO_DEV_STS_WRN )) &&
	// 				((m_pLnResSave[i].stat >= MENU_LFO_DEV_STS_SCT && m_pLnResSave[i].stat <= MENU_LFO_DEV_STS_WRN )))
	// 			{
	// 				if(m_pLnResSave[i].stat != m_pLnMon[i].stat )
	// 				{
	// 					m_pLnResSave[i].stat           = m_pLnMon[i].stat;
	// 					if ( m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	//                         // 2008-04-29 by zhw at nj m_ThisLfoInfo.start_time -> m_CurGpsTime - m_LfoPara.time_leng
	// 						if (m_pLnResSave[i].lfo_b_t <= 0)
	// 						{
	// 							m_pLnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 						}
	// 						m_pLnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pLnResSave[i].hold_time      = m_pLnResSave[i].lfo_e_t - m_pLnResSave[i].lfo_b_t;
	// 						m_pLnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	// 						
	// 						m_pLnResSave[i].b_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	// 						
	// 						m_pLnResSave[i].max_value.value  =  m_pLnMon[i].max_val;
	// 						m_pLnResSave[i].max_value.time   =  m_pLnMon[i].max_val_t;
	// 						m_pLnResSave[i].min_value.value  =  m_pLnMon[i].min_val;
	// 						m_pLnResSave[i].min_value.time   =  m_pLnMon[i].min_val_t;
	//                         if( m_pLnMon[i].max_min_dif > m_pLnResSave[i].max_min_dif )
	//                         {
	//                             m_pLnResSave[i].max_min_dif      =  m_pLnMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	// 					}
	// 					else if ( m_pLnMon[i].stat == MENU_LFO_DEV_STS_SCT)
	// 					{
	// 						//m_pLnResSave[i].lfo_e_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 						m_pLnResSave[i].hold_time      = m_pLnResSave[i].lfo_e_t - m_pLnResSave[i].lfo_b_t ;
	// 						
	// 						m_pLnResSave[i].e_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	// 					}
	// 				}
	// 				else
	// 				{
	// 					if ( m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	// 						if (m_pLnResSave[i].lfo_b_t <= 0)
	// 						{
	// 							// ���θ澯
	// 							m_pLnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pLnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 							m_pLnResSave[i].b_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	// 							m_pLnResSave[i].max_value.value  =  m_pLnMon[i].max_val;
	// 							m_pLnResSave[i].max_value.time   =  m_pLnMon[i].max_val_t;
	// 							m_pLnResSave[i].min_value.value  =  m_pLnMon[i].min_val;
	// 							m_pLnResSave[i].min_value.time   =  m_pLnMon[i].min_val_t;
	// 						}
	// 						m_pLnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pLnResSave[i].hold_time      = m_pLnResSave[i].lfo_e_t - m_pLnResSave[i].lfo_b_t;
	// 						m_pLnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                         if( m_pLnMon[i].max_min_dif > m_pLnResSave[i].max_min_dif )
	//                         {
	//                             m_pLnResSave[i].max_min_dif      =  m_pLnMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	// 					}
	// 				}
	// 
	// // 				m_pLnResSave[i].obj_id         = m_pLnInf[i].acln_id;
	// // 				strcpy(m_pLnResSave[i].obj_name, m_pLnInf[i].acln_name);
	// // 				if (m_pLnMon[i].slct_dot_id == m_pLnInf[i].dot_id_1)
	// // 				{
	// // 					m_pLnResSave[i].fac_id         = m_pLnInf[i].fac_id_1;
	// // 					sprintf(m_pLnResSave[i].fac_name,"%s",m_pLnInf[i].fac_name_1);
	// // 				}
	// // 				else
	// // 				{
	// // 					m_pLnResSave[i].fac_id         = m_pLnInf[i].fac_id_2;
	// // 					sprintf(m_pLnResSave[i].fac_name,"%s",m_pLnInf[i].fac_name_2);
	// // 				}
	// // 				m_pLnResSave[i].obj_type       = MENU_LFO_DEV_LINE;
	//                 // 2008-11-13
	// 				//if( m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN ) // 2008-01-22 lsf ��������״̬�ж�
	// 				if( m_pLnMon[i].stat >= MENU_LFO_DEV_STS_SCT  && m_pLnDat[i].stat == MENU_LFO_DEV_STS_OK )
	// 				{
	//                     m_pLnResSave[i].e_aver_pqv0[0]   = m_pLnMon[i].aver_val;
	// 					if (m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN)   //freq > 0)
	// 					{
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].ampl  = m_pLnMon[i].lead_ampl;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].freq  = m_pLnMon[i].lead_freq;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].damp  = m_pLnMon[i].lead_damp;
	// 						m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].phase = m_pLnMon[i].lead_phas;
	// 						m_pLnResSave[i].mode1_num += 1;
	// 					}
	// 
	// 					if (m_pLnResSave[i].max_value.value < m_pLnMon[i].max_val && m_pLnMon[i].max_val_t > 0)
	// 					{
	// 						m_pLnResSave[i].max_value.value  =  m_pLnMon[i].max_val;
	// 						m_pLnResSave[i].max_value.time   =  m_pLnMon[i].max_val_t;
	// 					}
	// 					if (m_pLnResSave[i].min_value.value > m_pLnMon[i].min_val && m_pLnMon[i].min_val_t > 0)
	// 					{
	// 						m_pLnResSave[i].min_value.value  =  m_pLnMon[i].min_val;
	// 						m_pLnResSave[i].min_value.time   =  m_pLnMon[i].min_val_t;
	// 					}
	//                     if( m_pLnMon[i].max_min_dif > m_pLnResSave[i].max_min_dif )
	//                     {
	//                         m_pLnResSave[i].max_min_dif      =  m_pLnMon[i].max_min_dif;  // ���ֵ��������
	//                     }
	// 				}
	// 			}
	// 		}
	// 		//��������豸��Ϣ
	// 		for(i=0; i<m_nGnNum; i++)               //����
	// 		{
	// 			//״̬�ж�
	// 			if (((m_pGnMon[i].stat     >= MENU_LFO_DEV_STS_SCT && m_pGnMon[i].stat     <= MENU_LFO_DEV_STS_WRN )) &&
	// 				((m_pGnResSave[i].stat >= MENU_LFO_DEV_STS_SCT && m_pGnResSave[i].stat <= MENU_LFO_DEV_STS_WRN )))
	// 			{
	// 				if(m_pGnResSave[i].stat != m_pGnMon[i].stat )
	// 				{
	// 					m_pGnResSave[i].stat           = m_pGnMon[i].stat;
	// 					if ( m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	//                         if (m_pGnResSave[i].lfo_b_t <= 0)
	//                         {
	// 							m_pGnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	//                         }
	// 						
	// 						m_pGnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pGnResSave[i].hold_time      = m_pGnResSave[i].lfo_e_t - m_pGnResSave[i].lfo_b_t;
	// 						m_pGnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	// 						
	// 						m_pGnResSave[i].b_aver_pqv1[0]   = m_pGnMon[i].aver_val;
	// 						
	// 						m_pGnResSave[i].max_value.value  =  m_pGnMon[i].max_val;
	// 						m_pGnResSave[i].max_value.time   =  m_pGnMon[i].max_val_t;
	// 						m_pGnResSave[i].min_value.value  =  m_pGnMon[i].min_val;
	// 						m_pGnResSave[i].min_value.time   =  m_pGnMon[i].min_val_t;
	//                         if( m_pGnMon[i].max_min_dif > m_pGnResSave[i].max_min_dif )
	//                         {
	//                             m_pGnResSave[i].max_min_dif      =  m_pGnMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	// 					}
	// 					else if ( m_pGnMon[i].stat == MENU_LFO_DEV_STS_SCT)
	// 					{
	// 						//m_pGnResSave[i].lfo_e_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 						m_pGnResSave[i].hold_time      = m_pGnResSave[i].lfo_e_t - m_pGnResSave[i].lfo_b_t ;
	// 						
	// 						m_pGnResSave[i].e_aver_pqv1[0]   = m_pGnMon[i].aver_val;
	// 					}
	// 				}
	// 				else
	// 				{
	// 					if ( m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	// 						if (m_pGnResSave[i].lfo_b_t <= 0)
	// 						{
	// 							// ���θ澯
	// 							m_pGnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pGnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 							m_pGnResSave[i].b_aver_pqv1[0]   = m_pGnMon[i].aver_val;
	// 							m_pGnResSave[i].max_value.value  =  m_pGnMon[i].max_val;
	// 							m_pGnResSave[i].max_value.time   =  m_pGnMon[i].max_val_t;
	// 							m_pGnResSave[i].min_value.value  =  m_pGnMon[i].min_val;
	// 							m_pGnResSave[i].min_value.time   =  m_pGnMon[i].min_val_t;
	// 						}
	// 						m_pGnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pGnResSave[i].hold_time      = m_pGnResSave[i].lfo_e_t - m_pGnResSave[i].lfo_b_t;
	// 						m_pGnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                         if( m_pGnMon[i].max_min_dif > m_pGnResSave[i].max_min_dif )
	//                         {
	//                             m_pGnResSave[i].max_min_dif      =  m_pGnMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	// 					}
	// 				}
	// 
	// // 				m_pGnResSave[i].obj_id         = m_pGnInf[i].gn_id;
	// // // 				strcpy(m_pGnResSave[i].obj_name, m_pGnInf[i].fac_name);
	// // // 				strcat(m_pGnResSave[i].obj_name, m_pGnInf[i].gn_name);
	// // 				strcpy(m_pGnResSave[i].obj_name, m_pGnInf[i].gn_name);
	// // 				m_pGnResSave[i].fac_id         = m_pGnInf[i].fac_id;
	// // 				sprintf(m_pGnResSave[i].fac_name,"%s",m_pGnInf[i].fac_name);
	// // 				m_pGnResSave[i].obj_type       = MENU_LFO_DEV_GEN;
	// 
	// 				//if( m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN )  // 2008-01-22 lsf ��������״̬�ж�
	// 				if( m_pGnMon[i].stat >= MENU_LFO_DEV_STS_SCT  && m_pGnDat[i].stat == MENU_LFO_DEV_STS_OK )  // 2008-01-22 lsf ��������״̬�ж�
	// 				{
	// 					m_pGnResSave[i].e_aver_pqv0[0]   = m_pGnMon[i].aver_val;
	// 					if (m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)   //freq > 0)
	// 					{
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].ampl  = m_pGnMon[i].lead_ampl;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].freq  = m_pGnMon[i].lead_freq;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].damp  = m_pGnMon[i].lead_damp;
	// 						m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].phase = m_pGnMon[i].lead_phas;
	// 						m_pGnResSave[i].mode1_num += 1;
	// 					}
	// 
	// 					if (m_pGnResSave[i].max_value.value < m_pGnMon[i].max_val && m_pGnMon[i].max_val_t > 0)
	// 					{
	// 						m_pGnResSave[i].max_value.value  =  m_pGnMon[i].max_val;
	// 						m_pGnResSave[i].max_value.time   =  m_pGnMon[i].max_val_t;
	// 					}
	// 					if (m_pGnResSave[i].min_value.value > m_pGnMon[i].min_val && m_pGnMon[i].min_val_t > 0)
	// 					{
	// 						m_pGnResSave[i].min_value.value  =  m_pGnMon[i].min_val;
	// 						m_pGnResSave[i].min_value.time   =  m_pGnMon[i].min_val_t;
	// 					}
	//                     if( m_pGnMon[i].max_min_dif > m_pGnResSave[i].max_min_dif )
	//                     {
	//                         m_pGnResSave[i].max_min_dif      =  m_pGnMon[i].max_min_dif;  // ���ֵ��������
	//                     }
	// 				}
	// 			}
	// 		}
	// 
	//         //��������豸��Ϣ
	//         for(i=0; i<m_nTrNum; i++)               //��·
	//         {
	//             //״̬�ж�
	//             if (((m_pTrMon[i].stat     >= MENU_LFO_DEV_STS_SCT && m_pTrMon[i].stat     <= MENU_LFO_DEV_STS_WRN )) &&
	//                 ((m_pTrResSave[i].stat >= MENU_LFO_DEV_STS_SCT && m_pTrResSave[i].stat <= MENU_LFO_DEV_STS_WRN )))
	//             {
	//                 if(m_pTrResSave[i].stat != m_pTrMon[i].stat )
	//                 {
	//                     m_pTrResSave[i].stat           = m_pTrMon[i].stat;
	//                     if ( m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	//                     {
	//                         if (m_pTrResSave[i].lfo_b_t <= 0)
	//                         {
	// 							m_pTrResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	//                         }
	//                         
	// 						m_pTrResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pTrResSave[i].hold_time      = m_pTrResSave[i].lfo_e_t - m_pTrResSave[i].lfo_b_t;
	//                         m_pTrResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                         
	//                         m_pTrResSave[i].b_aver_pqv1[0]   = m_pTrMon[i].aver_val;
	//                         
	//                         m_pTrResSave[i].max_value.value  =  m_pTrMon[i].max_val;
	//                         m_pTrResSave[i].max_value.time   =  m_pTrMon[i].max_val_t;
	//                         m_pTrResSave[i].min_value.value  =  m_pTrMon[i].min_val;
	//                         m_pTrResSave[i].min_value.time   =  m_pTrMon[i].min_val_t;
	//                         if( m_pTrMon[i].max_min_dif > m_pTrResSave[i].max_min_dif )
	//                         {
	//                             m_pTrResSave[i].max_min_dif      =  m_pTrMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	//                     }
	//                     else if ( m_pTrMon[i].stat == MENU_LFO_DEV_STS_SCT)
	//                     {
	//                         //m_pTrResSave[i].lfo_e_t        = m_CurGpsTime - m_LfoPara.time_leng;
	//                         m_pTrResSave[i].hold_time      = m_pTrResSave[i].lfo_e_t - m_pTrResSave[i].lfo_b_t ;
	// 
	//                         m_pTrResSave[i].e_aver_pqv1[0]   = m_pTrMon[i].aver_val;
	//                     }
	//                 }
	// 				else
	// 				{
	// 					if ( m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	// 						if (m_pTrResSave[i].lfo_b_t <= 0)
	// 						{
	// 							// ���θ澯
	// 							m_pTrResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pTrResSave[i].lfo_e_t        = m_CurGpsTime;
	// 							m_pTrResSave[i].b_aver_pqv1[0]   = m_pTrMon[i].aver_val;
	// 							m_pTrResSave[i].max_value.value  =  m_pTrMon[i].max_val;
	// 							m_pTrResSave[i].max_value.time   =  m_pTrMon[i].max_val_t;
	// 							m_pTrResSave[i].min_value.value  =  m_pTrMon[i].min_val;
	// 							m_pTrResSave[i].min_value.time   =  m_pTrMon[i].min_val_t;
	// 						}
	// 						m_pTrResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pTrResSave[i].hold_time      = m_pTrResSave[i].lfo_e_t - m_pTrResSave[i].lfo_b_t;
	// 						m_pTrResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                         if( m_pTrMon[i].max_min_dif > m_pTrResSave[i].max_min_dif )
	//                         {
	//                             m_pTrResSave[i].max_min_dif      =  m_pTrMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	// 					}
	// 				}
	//                 
	// // 				m_pTrResSave[i].obj_id         = m_pTrInf[i].tr_id;
	// // 				strcpy(m_pTrResSave[i].obj_name, m_pTrInf[i].tr_name);
	// // 				m_pTrResSave[i].fac_id         = m_pTrInf[i].fac_id;
	// // 				sprintf(m_pTrResSave[i].fac_name,"%s",m_pTrInf[i].fac_name);
	// // 				m_pTrResSave[i].obj_type       = MENU_LFO_DEV_TR;
	// 
	//                 //if( m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN ) // 2008-01-22 lsf ��������״̬�ж�
	//                 if( m_pTrMon[i].stat >= MENU_LFO_DEV_STS_SCT  && m_pTrDat[i].stat == MENU_LFO_DEV_STS_OK )
	//                 {
	//                     m_pTrResSave[i].e_aver_pqv0[0]   = m_pTrMon[i].aver_val;
	//                     if (m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN)   //freq > 0)
	//                     {
	//                         m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	//                         m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].end_t = m_CurGpsTime;
	//                         m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].ampl  = m_pTrMon[i].lead_ampl;
	//                         m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].freq  = m_pTrMon[i].lead_freq;
	//                         m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].damp  = m_pTrMon[i].lead_damp;
	//                         m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].phase = m_pTrMon[i].lead_phas;
	//                         m_pTrResSave[i].mode1_num += 1;
	//                     }
	//                     
	//                     if (m_pTrResSave[i].max_value.value < m_pTrMon[i].max_val && m_pTrMon[i].max_val_t > 0)
	//                     {
	//                         m_pTrResSave[i].max_value.value  =  m_pTrMon[i].max_val;
	//                         m_pTrResSave[i].max_value.time   =  m_pTrMon[i].max_val_t;
	//                     }
	//                     if (m_pTrResSave[i].min_value.value > m_pTrMon[i].min_val && m_pTrMon[i].min_val_t > 0)
	//                     {
	//                         m_pTrResSave[i].min_value.value  =  m_pTrMon[i].min_val;
	//                         m_pTrResSave[i].min_value.time   =  m_pTrMon[i].min_val_t;
	//                     }
	//                     if( m_pTrMon[i].max_min_dif > m_pTrResSave[i].max_min_dif )
	//                     {
	//                         m_pTrResSave[i].max_min_dif      =  m_pTrMon[i].max_min_dif;  // ���ֵ��������
	//                     }
	//                 }
	//             }
	// 		}
	//         //////////////////////////////////////////////////////////////////////////		
	//         //����ϵͳ������Ϣ
	//         m_SysResSave.lfo_e_t         = m_CurGpsTime;
	//         m_SysResSave.hold_time       = m_SysResSave.lfo_e_t - m_SysResSave.lfo_b_t ;
	//         //m_SysResSave.freq            = m_ThisLfoInfo.frequency ; //��δ���		
	//         //ͳ�Ƴ����񵴵ļ�����·����
	//         LfoLineNum  = 0 ;
	//         float temp_value=0.0;
	//         //time_t temp_time;
	//         //memset( (void*)&temp_value,0,sizeof(VALUE_TIME_STRU));
	//         for(i=0; i<m_nLnNum; i++) 
	//         {
	//             if (m_pLnResSave[i].lfo_b_t > 0 )
	//             {
	//                 m_pLnResSave[i].max_ampl = fabs(m_pLnResSave[i].max_value.value - m_pLnResSave[i].min_value.value);
	// 				if (m_pLnResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pLnResSave[i].max_ampl;
	// 				}
	//                 LfoLineNum += 1;
	//             }
	//         }
	//         m_SysResSave.lfo_line_num = LfoLineNum;
	//         
	//         LfoGenNum  = 0 ;
	//         for(i=0; i<m_nGnNum; i++) 
	//         {
	//             if (m_pGnResSave[i].lfo_b_t > 0 )
	//             {
	//                 m_pGnResSave[i].max_ampl = fabs(m_pGnResSave[i].max_value.value - m_pGnResSave[i].min_value.value);
	// 				if (m_pGnResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pGnResSave[i].max_ampl;
	// 				}
	//                 LfoGenNum += 1;
	//             }
	//         }
	//         m_SysResSave.act_gen_num = LfoGenNum;
	//         
	//         for(i=0; i<m_nTrNum; i++) 
	//         {
	//             if (m_pTrResSave[i].lfo_b_t > 0 )
	//             {
	//                 m_pTrResSave[i].max_ampl = fabs(m_pTrResSave[i].max_value.value - m_pTrResSave[i].min_value.value);
	// 				if (m_pTrResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pTrResSave[i].max_ampl;
	// 				}
	//                 LfoTrNum += 1;
	//             }
	//         }
	//         m_SysResSave.lfo_tr_num = LfoTrNum;
	//         
	//         sprintf(m_SysResSave.lfo_case_des, "%s%s", PT_GetChineseTimeStringBySeconds(m_SysResSave.lfo_b_t), "ϵͳ���ֵ�Ƶ��");
	// 	}
	// 	else if(m_FormResSaveStat == L_END_FORM_RESULTE)
	// 	{
	// 		//��������豸��Ϣ
	// 		for(i=0; i<m_nLnNum; i++)               //��·
	// 		{
	// // 			m_pLnResSave[i].obj_id         = m_pLnInf[i].acln_id;
	// // 			strcpy(m_pLnResSave[i].obj_name, m_pLnInf[i].acln_name);
	// // 			if (m_pLnMon[i].slct_dot_id == m_pLnInf[i].dot_id_1)
	// // 			{
	// // 				m_pLnResSave[i].fac_id         = m_pLnInf[i].fac_id_1;
	// // 				sprintf(m_pLnResSave[i].fac_name,"%s",m_pLnInf[i].fac_name_1);
	// // 			}
	// // 			else
	// // 			{
	// // 				m_pLnResSave[i].fac_id         = m_pLnInf[i].fac_id_2;
	// // 				sprintf(m_pLnResSave[i].fac_name,"%s",m_pLnInf[i].fac_name_2);
	// // 			}
	// // 			m_pLnResSave[i].obj_type       = MENU_LFO_DEV_LINE;
	// 
	// 			if(m_pLnMon[i].stat >= MENU_LFO_DEV_STS_SCT)
	// 			{
	// 				m_pLnResSave[i].e_aver_pqv0[0]   = m_pLnMon[i].aver_val;
	// 			}
	// 			//״̬�ж�
	// 			if (((m_pLnMon[i].stat     >= MENU_LFO_DEV_STS_SCT && m_pLnMon[i].stat     <= MENU_LFO_DEV_STS_WRN )) &&
	// 				((m_pLnResSave[i].stat >= MENU_LFO_DEV_STS_SCT && m_pLnResSave[i].stat <= MENU_LFO_DEV_STS_WRN )))
	// 			{
	// 				if(m_pLnResSave[i].stat != m_pLnMon[i].stat )
	// 				{
	// 					m_pLnResSave[i].stat           = m_pLnMon[i].stat;
	// 					if ( m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	//                         if (m_pLnResSave[i].lfo_b_t <= 0)
	//                         {
	// 							m_pLnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	//                         }
	// 						
	// 						m_pLnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pLnResSave[i].hold_time      = m_pLnResSave[i].lfo_e_t - m_pLnResSave[i].lfo_b_t;
	// 						m_pLnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	// 						
	// 						m_pLnResSave[i].b_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	// 						if (m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN )   //freq > 0)
	// 						{
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].ampl  = m_pLnMon[i].lead_ampl;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].freq  = m_pLnMon[i].lead_freq;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].damp  = m_pLnMon[i].lead_damp;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].phase = m_pLnMon[i].lead_phas;
	// 							m_pLnResSave[i].mode1_num += 1;
	// 						}
	// 					}
	// 					else if ( m_pLnMon[i].stat == MENU_LFO_DEV_STS_SCT)
	// 					{
	// 						// ��ȫʱ����һ�ֽ���ʱ��Ϊ�񵴽���ʱ��
	// 						//m_pLnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pLnResSave[i].hold_time      = m_pLnResSave[i].lfo_e_t - m_pLnResSave[i].lfo_b_t ;
	// 						
	// 						m_pLnResSave[i].e_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	// 					}
	// 				}
	// 				else
	// 				{
	// 					if ( m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	// 						if (m_pLnResSave[i].lfo_b_t <= 0)
	// 						{
	// 							// ���θ澯
	// 							m_pLnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pLnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 							m_pLnResSave[i].b_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	// 							m_pLnResSave[i].max_value.value  =  m_pLnMon[i].max_val;
	// 							m_pLnResSave[i].max_value.time   =  m_pLnMon[i].max_val_t;
	// 							m_pLnResSave[i].min_value.value  =  m_pLnMon[i].min_val;
	// 							m_pLnResSave[i].min_value.time   =  m_pLnMon[i].min_val_t;
	// 						}
	// 						m_pLnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pLnResSave[i].hold_time      = m_pLnResSave[i].lfo_e_t - m_pLnResSave[i].lfo_b_t;
	// 						m_pLnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                         if( m_pLnMon[i].max_min_dif > m_pLnResSave[i].max_min_dif )
	//                         {
	//                             m_pLnResSave[i].max_min_dif      =  m_pLnMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	// 						if (m_pLnMon[i].stat >= MENU_LFO_DEV_STS_FWN )   //freq > 0)
	// 						{
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].ampl  = m_pLnMon[i].lead_ampl;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].freq  = m_pLnMon[i].lead_freq;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].damp  = m_pLnMon[i].lead_damp;
	// 							m_pLnResSave[i].mode1[m_pLnResSave[i].mode1_num].phase = m_pLnMon[i].lead_phas;
	// 							m_pLnResSave[i].mode1_num += 1;
	// 						}
	// 					}
	// 				}
	//             }
	// //             if ( m_pLnResSave[i].lfo_b_t > 0 && m_pLnResSave[i].stat >= MENU_LFO_DEV_STS_FWN)  //2008-04-01 added by zhw at yn
	// //             {
	// //                 m_pLnResSave[i].lfo_e_t        = Date7_beginTime+Date7_timeLeng;
	// //                 m_pLnResSave[i].hold_time      = m_pLnResSave[i].lfo_e_t - m_pLnResSave[i].lfo_b_t ;
	// //                 
	// //                 m_pLnResSave[i].e_aver_pqv1[0]   = m_pLnMon[i].aver_val;
	// //             }
	//  		}
	// 		//��������豸��Ϣ
	// 		for(i=0; i<m_nGnNum; i++)               //����
	// 		{
	// // 			m_pGnResSave[i].obj_id         = m_pGnInf[i].gn_id;
	// // // 				strcpy(m_pGnResSave[i].obj_name, m_pGnInf[i].fac_name);
	// // // 				strcat(m_pGnResSave[i].obj_name, m_pGnInf[i].gn_name);
	// // 			strcpy(m_pGnResSave[i].obj_name, m_pGnInf[i].gn_name);
	// // 			m_pGnResSave[i].fac_id         = m_pGnInf[i].fac_id;
	// // 			sprintf(m_pGnResSave[i].fac_name,"%s",m_pGnInf[i].fac_name);
	// // 			m_pGnResSave[i].obj_type       = MENU_LFO_DEV_GEN;
	// 			if(m_pGnMon[i].stat >= MENU_LFO_DEV_STS_SCT)
	// 			{
	// 				m_pGnResSave[i].e_aver_pqv0[0]   = m_pGnMon[i].aver_val;
	// 			}
	// 			//״̬�ж�
	// 			if (((m_pGnMon[i].stat     >= MENU_LFO_DEV_STS_SCT && m_pLnMon[i].stat     <= MENU_LFO_DEV_STS_WRN )) &&
	// 				((m_pGnResSave[i].stat >= MENU_LFO_DEV_STS_SCT && m_pGnResSave[i].stat <= MENU_LFO_DEV_STS_WRN )))
	// 			{
	// 				if(m_pGnResSave[i].stat != m_pGnMon[i].stat )
	// 				{
	// 					m_pGnResSave[i].stat           = m_pGnMon[i].stat;
	// 					if ( m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	//                         if (m_pGnResSave[i].lfo_b_t <= 0)
	//                         {
	// 							m_pGnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	//                         }
	// 
	// 						m_pGnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	// 						
	// 						m_pGnResSave[i].b_aver_pqv1[0]   = m_pGnMon[i].aver_val;
	// 						if (m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 						{
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].ampl  = m_pGnMon[i].lead_ampl;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].freq  = m_pGnMon[i].lead_freq;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].damp  = m_pGnMon[i].lead_damp;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].phase = m_pGnMon[i].lead_phas;
	// 							m_pGnResSave[i].mode1_num += 1;
	// 						}
	// 					}
	// 					else if ( m_pGnMon[i].stat == MENU_LFO_DEV_STS_SCT)
	// 					{
	// 						// ��ȫʱ����һ�ֽ���ʱ��Ϊ�񵴽���ʱ��
	// 						//m_pGnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pGnResSave[i].hold_time      = m_pGnResSave[i].lfo_e_t - m_pGnResSave[i].lfo_b_t ;
	// 						
	// 						m_pGnResSave[i].e_aver_pqv1[0]   = m_pGnMon[i].aver_val;
	// 					}
	// 				}
	// 				else
	// 				{
	// 					if ( m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	// 						if (m_pGnResSave[i].lfo_b_t <= 0)
	// 						{
	// 							// ���θ澯
	// 							m_pGnResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pGnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 							m_pGnResSave[i].b_aver_pqv1[0]   = m_pGnMon[i].aver_val;
	// 							m_pGnResSave[i].max_value.value  =  m_pGnMon[i].max_val;
	// 							m_pGnResSave[i].max_value.time   =  m_pGnMon[i].max_val_t;
	// 							m_pGnResSave[i].min_value.value  =  m_pGnMon[i].min_val;
	// 							m_pGnResSave[i].min_value.time   =  m_pGnMon[i].min_val_t;
	// 						}
	// 						m_pGnResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pGnResSave[i].hold_time      = m_pGnResSave[i].lfo_e_t - m_pGnResSave[i].lfo_b_t;
	// 						m_pGnResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                         if( m_pGnMon[i].max_min_dif > m_pGnResSave[i].max_min_dif )
	//                         {
	//                             m_pGnResSave[i].max_min_dif      =  m_pGnMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	// 						if (m_pGnMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 						{
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].ampl  = m_pGnMon[i].lead_ampl;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].freq  = m_pGnMon[i].lead_freq;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].damp  = m_pGnMon[i].lead_damp;
	// 							m_pGnResSave[i].mode1[m_pGnResSave[i].mode1_num].phase = m_pGnMon[i].lead_phas;
	// 							m_pGnResSave[i].mode1_num += 1;
	// 						}
	// 					}
	// 				}
	// 			}
	// //             if ( m_pGnResSave[i].lfo_b_t > 0 && m_pGnResSave[i].stat >= MENU_LFO_DEV_STS_FWN)  //2008-04-01 added by zhw at yn
	// //             {
	// //                 m_pGnResSave[i].lfo_e_t        = Date7_beginTime+Date7_timeLeng;
	// //                 m_pGnResSave[i].hold_time      = m_pGnResSave[i].lfo_e_t - m_pGnResSave[i].lfo_b_t ;
	// //                 
	// //                 m_pGnResSave[i].e_aver_pqv1[0]   = m_pGnMon[i].aver_val;
	// //             }
	// 		}
	//         //��������豸��Ϣ
	//         for(i=0; i<m_nTrNum; i++)               //��ѹ��
	//         {
	// // 			m_pTrResSave[i].obj_id         = m_pTrInf[i].tr_id;
	// // 			strcpy(m_pTrResSave[i].obj_name, m_pTrInf[i].tr_name);
	// // 			m_pTrResSave[i].fac_id         = m_pTrInf[i].fac_id;
	// // 			sprintf(m_pTrResSave[i].fac_name,"%s",m_pTrInf[i].fac_name);
	// // 			m_pTrResSave[i].obj_type       = MENU_LFO_DEV_TR;
	//             if(m_pTrMon[i].stat >= MENU_LFO_DEV_STS_SCT)
	//             {
	//                 m_pTrResSave[i].e_aver_pqv0[0]   = m_pTrMon[i].aver_val;
	//             }
	//             //״̬�ж�
	//             if (((m_pTrMon[i].stat     >= MENU_LFO_DEV_STS_SCT && m_pTrMon[i].stat     <= MENU_LFO_DEV_STS_WRN )) &&
	//                 ((m_pTrResSave[i].stat >= MENU_LFO_DEV_STS_SCT && m_pTrResSave[i].stat <= MENU_LFO_DEV_STS_WRN )))
	//             {
	//                 if(m_pTrResSave[i].stat != m_pTrMon[i].stat )
	//                 {
	//                     m_pTrResSave[i].stat           = m_pTrMon[i].stat;
	//                     if ( m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	//                     {
	//                         if (m_pTrResSave[i].lfo_b_t <= 0)
	//                         {
	// 							m_pTrResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	//                         }
	//                         
	// 						m_pTrResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pTrResSave[i].hold_time      = m_pTrResSave[i].lfo_e_t - m_pTrResSave[i].lfo_b_t;
	//                         m_pTrResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                         
	//                         m_pTrResSave[i].b_aver_pqv1[0]   = m_pTrMon[i].aver_val;
	// 						if (m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN )   //freq > 0)
	// 						{
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].ampl  = m_pTrMon[i].lead_ampl;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].freq  = m_pTrMon[i].lead_freq;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].damp  = m_pTrMon[i].lead_damp;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].phase = m_pTrMon[i].lead_phas;
	// 							m_pTrResSave[i].mode1_num += 1;
	// 						}
	//                     }
	//                     else if ( m_pTrMon[i].stat == MENU_LFO_DEV_STS_SCT)
	//                     {
	// 						// ��ȫʱ����һ�ֽ���ʱ��Ϊ�񵴽���ʱ��
	//                         // m_pTrResSave[i].lfo_e_t        = m_CurGpsTime;
	//                         m_pTrResSave[i].hold_time      = m_pTrResSave[i].lfo_e_t - m_pTrResSave[i].lfo_b_t ;
	//                         
	//                         m_pTrResSave[i].e_aver_pqv1[0]   = m_pTrMon[i].aver_val;
	//                     }
	//                 }
	// 				else
	// 				{
	// 					if ( m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN)
	// 					{
	// 						if (m_pTrResSave[i].lfo_b_t <= 0)
	// 						{
	// 							// ���θ澯
	// 							m_pTrResSave[i].lfo_b_t        = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pTrResSave[i].lfo_e_t        = m_CurGpsTime;
	// 							m_pTrResSave[i].b_aver_pqv1[0]   = m_pTrMon[i].aver_val;
	// 							m_pTrResSave[i].max_value.value  =  m_pTrMon[i].max_val;
	// 							m_pTrResSave[i].max_value.time   =  m_pTrMon[i].max_val_t;
	// 							m_pTrResSave[i].min_value.value  =  m_pTrMon[i].min_val;
	// 							m_pTrResSave[i].min_value.time   =  m_pTrMon[i].min_val_t;
	// 						}
	// 						m_pTrResSave[i].lfo_e_t        = m_CurGpsTime;
	// 						m_pTrResSave[i].hold_time      = m_pTrResSave[i].lfo_e_t - m_pTrResSave[i].lfo_b_t;
	// 						m_pTrResSave[i].if_lfo         = MENU_LFO_MON_YES;
	//                         if( m_pTrMon[i].max_min_dif > m_pTrResSave[i].max_min_dif )
	//                         {
	//                             m_pTrResSave[i].max_min_dif      =  m_pTrMon[i].max_min_dif;  // ���ֵ��������
	//                         }
	// 						if (m_pTrMon[i].stat >= MENU_LFO_DEV_STS_FWN )   //freq > 0)
	// 						{
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].bgn_t = m_CurGpsTime - m_LfoPara.time_leng;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].end_t = m_CurGpsTime;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].ampl  = m_pTrMon[i].lead_ampl;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].freq  = m_pTrMon[i].lead_freq;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].damp  = m_pTrMon[i].lead_damp;
	// 							m_pTrResSave[i].mode1[m_pTrResSave[i].mode1_num].phase = m_pTrMon[i].lead_phas;
	// 							m_pTrResSave[i].mode1_num += 1;
	// 						}
	// 					}
	// 				}
	//                 
	//             }
	// //             if ( m_pTrResSave[i].lfo_b_t > 0 && m_pTrResSave[i].stat >= MENU_LFO_DEV_STS_FWN)  //2008-04-01 added by zhw at yn
	// //             {
	// //                 m_pTrResSave[i].lfo_e_t        = Date7_beginTime+Date7_timeLeng;
	// //                 m_pTrResSave[i].hold_time      = m_pTrResSave[i].lfo_e_t - m_pTrResSave[i].lfo_b_t ;
	// //                 
	// //                 m_pTrResSave[i].e_aver_pqv1[0]   = m_pTrMon[i].aver_val;
	// //             }
	//  		}
	// 		//////////////////////////////////////////////////////////////////////////		
	// 		//����ϵͳ������Ϣ
	// 		// ��ȫʱ����һ�ֽ���ʱ��Ϊ�񵴽���ʱ��
	// 		//m_SysResSave.lfo_e_t         = m_CurGpsTime;
	// 		m_SysResSave.hold_time       = m_SysResSave.lfo_e_t - m_SysResSave.lfo_b_t ;
	// 		//m_SysResSave.freq            = m_ThisLfoInfo.frequency ; //��δ���		
	// 		//ͳ�Ƴ����񵴵ļ�����·����
	// 		LfoLineNum  = 0 ;
	//         float temp_value=0.0;
	//         time_t temp_time;
	//         //memset( (void*)&temp_value,0,sizeof(VALUE_TIME_STRU));
	// 		for(i=0; i<m_nLnNum; i++) 
	// 		{
	// 			if (m_pLnResSave[i].lfo_b_t > 0 )
	// 			{
	//                 //  2008-08-28 �����ֵ,��Сֵ�����ǹ��ʷ���,������ֵ
	//                 if(fabs(m_pLnResSave[i].max_value.value)<fabs(m_pLnResSave[i].min_value.value))
	//                 {
	//                     temp_value = m_pLnResSave[i].max_value.value;
	//                     temp_time  = m_pLnResSave[i].max_value.time;
	//                     m_pLnResSave[i].max_value.value = m_pLnResSave[i].min_value.value;
	//                     m_pLnResSave[i].max_value.time = m_pLnResSave[i].min_value.time;
	//                     m_pLnResSave[i].min_value.value = temp_value;
	//                     m_pLnResSave[i].min_value.time = temp_time;
	//                 }
	//                 m_pLnResSave[i].max_ampl = fabs(m_pLnResSave[i].max_value.value - m_pLnResSave[i].min_value.value);
	// 				if (m_pLnResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pLnResSave[i].max_ampl;
	// 				}
	// 				LfoLineNum += 1;
	// 			}
	// 		}
	// 		m_SysResSave.lfo_line_num = LfoLineNum;
	// 
	// 		LfoGenNum  = 0 ;
	// 		for(i=0; i<m_nGnNum; i++) 
	// 		{
	// 			if (m_pGnResSave[i].lfo_b_t > 0 )
	// 			{
	//                 if(fabs(m_pGnResSave[i].max_value.value)<fabs(m_pGnResSave[i].min_value.value))
	//                 {
	//                     temp_value = m_pGnResSave[i].max_value.value;
	//                     temp_time  = m_pGnResSave[i].max_value.time;
	//                     m_pGnResSave[i].max_value.value = m_pGnResSave[i].min_value.value;
	//                     m_pGnResSave[i].max_value.time = m_pGnResSave[i].min_value.time;
	//                     m_pGnResSave[i].min_value.value = temp_value;
	//                     m_pGnResSave[i].min_value.time = temp_time;
	//                 }
	//                 m_pGnResSave[i].max_ampl = fabs(m_pGnResSave[i].max_value.value - m_pGnResSave[i].min_value.value);
	// 				if (m_pGnResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pGnResSave[i].max_ampl;
	// 				}
	// 				LfoGenNum += 1;
	// 			}
	// 		}
	// 		m_SysResSave.act_gen_num = LfoGenNum;
	// 
	//         for(i=0; i<m_nTrNum; i++) 
	//         {
	//             if (m_pTrResSave[i].lfo_b_t > 0 )
	//             {
	//                 //  2008-08-28 �����ֵ,��Сֵ�����ǹ��ʷ���,������ֵ
	//                 if(fabs(m_pTrResSave[i].max_value.value)<fabs(m_pTrResSave[i].min_value.value))
	//                 {
	//                     temp_value = m_pTrResSave[i].max_value.value;
	//                     temp_time  = m_pTrResSave[i].max_value.time;
	//                     m_pTrResSave[i].max_value.value = m_pTrResSave[i].min_value.value;
	//                     m_pTrResSave[i].max_value.time = m_pTrResSave[i].min_value.time;
	//                     m_pTrResSave[i].min_value.value = temp_value;
	//                     m_pTrResSave[i].min_value.time = temp_time;
	//                 }
	//                 m_pTrResSave[i].max_ampl = fabs(m_pTrResSave[i].max_value.value - m_pTrResSave[i].min_value.value);
	// 				if (m_pTrResSave[i].max_ampl > m_SysResSave.max_ampl)
	// 				{
	// 					m_SysResSave.max_ampl = m_pTrResSave[i].max_ampl;
	// 				}
	//                 LfoTrNum += 1;
	//             }
	//         }
	// 		m_SysResSave.lfo_tr_num = LfoTrNum;
	// 
	// 		sprintf(m_SysResSave.lfo_case_des, "%s%s", PT_GetChineseTimeStringBySeconds(m_SysResSave.lfo_b_t), "ϵͳ���ֵ�Ƶ��");
	// 		// ����ƽ��Ƶ��
	// 		m_SysResSave.aver_freq = 0.0f;
	// 		for (ii =0; ii<m_SysResSave.mode1_num; ii++)
	// 		{
	// 			m_SysResSave.aver_freq += m_SysResSave.mode1[m_SysResSave.mode1_num].freq;
	// 		}
	// 		m_SysResSave.aver_freq = m_SysResSave.aver_freq / m_SysResSave.mode1_num;
	//         //  2008-08-26 ���񵴿�ʼʱ������
	// //         qsort((void *)m_pLnResSave, m_nLnNum, sizeof(LFO_OBJ_RES_SAVE), ResSaveCompareByBgnTime);
	// //         qsort((void *)m_pGnResSave, m_nGnNum,  sizeof(LFO_OBJ_RES_SAVE), ResSaveCompareByBgnTime);
	// //         qsort((void *)m_pTrResSave, m_nTrNum,  sizeof(LFO_OBJ_RES_SAVE), ResSaveCompareByBgnTime);
	// 	}
	// 
	//     WriteMonResult();   //  ɽ��Ӧ��ϵͳ�ӿ��ļ�  // 2008-06-24 by zhw

	return;
}

// ���������ṹд���ļ�
void CLfoAnaObj::SaveResultFileTxt()
{
	int     i, j = 0;
	int     LineModeNum = 0;
	//int     LfoLineNum  = 0 ;
	char    saveFileName[MAXBUF];
	char    strTime1[28], strTime2[28];

#ifndef _WINDOWS32
	i = sprintf(saveFileName, "%s/res/%s", m_RunInfDir, PT_GetChineseTimeStringBySeconds(m_SysResSave.lfo_b_t));
	sprintf(saveFileName + i, "%s.lfo", "ϵͳ�񵴸澯");
#else
	i = sprintf(saveFileName, "%s\\res\\%s", m_RunInfDir, PT_GetChineseTimeStringBySeconds(m_SysResSave.lfo_b_t));
	sprintf(saveFileName + i, "%s.lfo", "ϵͳ�񵴸澯");
#endif

	FILE* fp = fopen(saveFileName, "w+");
	if (!fp)
	{
		sprintf(m_LogString, "SaveResultFileTxt: file open error");
		MessShow(m_LogString);
		return;
	}


	fprintf(fp, "\n[[�������]]\n");
	fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
	fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");

	sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_SysResSave.lfo_b_t));
	sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_SysResSave.lfo_e_t));

	fprintf(fp, "\tLFO�澯ʱ��  = %s   LFO��ʧʱ�� = %s   ����ʱ�� = %s \n",
		strTime1,
		strTime2,
		PT_GetStringOfTimeInterval(m_SysResSave.hold_time * 1000));
	fprintf(fp, "\t�����澯�豸 = %s   ��ʼ��Ƶ�� = %6.3fHz \n", m_SysResSave.obj_name, m_SysResSave.freq);
	fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
	if (m_SysResSave.lfo_line_num > 0)
	{
		fprintf(fp, "\t����·��%d����\n", m_SysResSave.lfo_line_num);

		for (i = 0; i < m_nLnNum; i++)
		{
			if (m_pLnResSave[i].lfo_b_t > 0 && m_pLnResSave[i].lfo_e_t > 0)
			{
				//if ((int)(m_pLnResSave[i].obj_id/DB_TABLE_BASE) == ACLN_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pLnResSave[i].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pLnResSave[i].lfo_e_t));

					fprintf(fp, "  \t%s  "INT_REC_FMT"  �澯ʱ�й���ֵ = %6.2fMW  ��ʧʱ�й���ֵ = %6.2fMW  �񵴷���ʱ�� = %s  ����ʧʱ�� = %s  ���ڶ����� = %6.2fMW \n",
						m_pLnResSave[i].obj_name,
						INT_REC_VAL(m_pLnResSave[i].obj_id),
						m_pLnResSave[i].b_aver_pqv0[0],
						m_pLnResSave[i].e_aver_pqv0[0],
						strTime1,
						strTime2,
						m_pLnResSave[i].max_value.value - m_pLnResSave[i].min_value.value);
				}
			}
		}
	}
	if (m_SysResSave.act_gen_num > 0)
	{
		fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
		fprintf(fp, "\t�񵴷������%d̨��\n", m_SysResSave.act_gen_num);

		for (i = 0; i < m_nGnNum; i++)
		{
			if (m_pGnResSave[i].lfo_b_t > 0 && m_pGnResSave[i].lfo_e_t > 0)
			{
				//if ((int)(m_pGnResSave[i].obj_id/DB_TABLE_BASE) == ACLN_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pGnResSave[i].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pGnResSave[i].lfo_e_t));

					fprintf(fp, "  \t%s  "INT_REC_FMT"  �澯ʱ�й���ֵ = %6.2fMW  ��ʧʱ�й���ֵ = %6.2fMW  �񵴷���ʱ�� = %s  ����ʧʱ�� = %s  ���ڶ����� = %6.2fMW \n",
						m_pGnResSave[i].obj_name,
						INT_REC_VAL(m_pGnResSave[i].obj_id),
						m_pGnResSave[i].b_aver_pqv0[0],
						m_pGnResSave[i].e_aver_pqv0[0],
						strTime1,
						strTime2,
						m_pGnResSave[i].max_value.value - m_pGnResSave[i].min_value.value);
				}
			}
		}
	}

	if (m_SysResSave.lfo_tr_num > 0)
	{
		fprintf(fp, "\t�񵴱�ѹ����%d����\n", m_SysResSave.lfo_tr_num);

		for (i = 0; i < m_nTrNum; i++)
		{
			if (m_pTrResSave[i].lfo_b_t > 0 && m_pTrResSave[i].lfo_e_t > 0)
			{
				//if ((int)(m_pLnResSave[i].obj_id/DB_TABLE_BASE) == ACLN_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pTrResSave[i].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pTrResSave[i].lfo_e_t));

					fprintf(fp, "  \t%s  "INT_REC_FMT"  �澯ʱ�й���ֵ = %6.2fMW  ��ʧʱ�й���ֵ = %6.2fMW  �񵴷���ʱ�� = %s  ����ʧʱ�� = %s  ���ڶ����� = %6.2fMW \n",
						m_pTrResSave[i].obj_name,
						INT_REC_VAL(m_pTrResSave[i].obj_id),
						m_pTrResSave[i].b_aver_pqv0[0],
						m_pTrResSave[i].e_aver_pqv0[0],
						strTime1,
						strTime2,
						m_pTrResSave[i].max_value.value - m_pTrResSave[i].min_value.value);
				}
			}
		}
	}

	if (m_SysResSave.lfo_line_num > 0)
	{
		fprintf(fp, "\n[��ϸ��Ϣ]\n");
		fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
		//fprintf(fp, "[��Ҫ��Ϣ]\n");
		for (i = 0; i < m_nLnNum; i++)
		{
			if (m_pLnResSave[i].lfo_b_t > 0 && m_pLnResSave[i].lfo_e_t > 0)
			{
				//if ((int)(m_pLnResSave[i].obj_id/DB_TABLE_BASE) == ACLN_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pLnResSave[i].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pLnResSave[i].lfo_e_t));

					fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
					fprintf(fp, "\t%s "INT_REC_FMT"  \n", m_pLnResSave[i].obj_name, INT_REC_VAL(m_pLnResSave[i].obj_id));
					fprintf(fp, "\t�񵴷���ʱ��   = %s    ����ʧʱ��   = %s   ����ʱ�� = %s \n", strTime1, strTime2, PT_GetStringOfTimeInterval(m_pLnResSave[i].hold_time * 1000));
					fprintf(fp, "\t��ǰ�й���ֵ = %6.2fMW               ��ʧ���й���ֵ = %6.2fMW \n", m_pLnResSave[i].b_aver_pqv1[0], m_pLnResSave[i].e_aver_pqv1[0]);
					fprintf(fp, "\t�й����ֵ     = %6.2fMW               ʱ��           = %s \n", m_pLnResSave[i].max_value.value, PT_GetTimeStringBySeconds(m_pLnResSave[i].max_value.time));
					fprintf(fp, "\t�й���Сֵ     = %6.2fMW               ʱ��           = %s \n", m_pLnResSave[i].min_value.value, PT_GetTimeStringBySeconds(m_pLnResSave[i].min_value.time));
					fprintf(fp, "\t���ڶ�����   = %6.2fMW \n", m_pLnResSave[i].max_value.value - m_pLnResSave[i].min_value.value);

					fprintf(fp, "\n  ����ģʽ��\n");
					LineModeNum = 0;
					LineModeNum = m_pLnResSave[i].mode1_num;

					fprintf(fp, "%s\n", "  ���         ����ʱ��             ��ֵ(MW)   Ƶ��(Hz)   �����(%)   ���(deg)");
					for (j = 0; j < LineModeNum; j++)
					{
						sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pLnResSave[i].mode1[j].bgn_t));
						sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pLnResSave[i].mode1[j].end_t));

						fprintf(fp, "  %4d  %10s - %8s   %10.2f  %10.2f  %10.2f  %10.2f\n",
							j + 1,
							strTime1,
							strTime2,
							m_pLnResSave[i].mode1[j].ampl,
							m_pLnResSave[i].mode1[j].freq,
							m_pLnResSave[i].mode1[j].damp,
							m_pLnResSave[i].mode1[j].phase);
					}
					fprintf(fp, "\n");
				}
			}
		}
	}
	if (m_SysResSave.act_gen_num > 0)
	{
		//fprintf(fp, "\n[��ϸ��Ϣ]\n");
		fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
		//fprintf(fp, "[��Ҫ��Ϣ]\n");
		for (i = 0; i < m_nGnNum; i++)
		{
			if (m_pGnResSave[i].lfo_b_t > 0 && m_pGnResSave[i].lfo_e_t > 0)
			{
				//if ((int)(m_pGnResSave[i].obj_id/DB_TABLE_BASE) == ACLN_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pGnResSave[i].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pGnResSave[i].lfo_e_t));

					fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
					fprintf(fp, "\t%s  "INT_REC_FMT"  \n", m_pGnResSave[i].obj_name, INT_REC_VAL(m_pGnResSave[i].obj_id));
					fprintf(fp, "\t�񵴷���ʱ��   = %s    ����ʧʱ��   = %s   ����ʱ�� = %s \n", strTime1, strTime2, PT_GetStringOfTimeInterval(m_pGnResSave[i].hold_time * 1000));
					fprintf(fp, "\t��ǰ�й���ֵ = %6.2fMW               ��ʧ���й���ֵ = %6.2fMW \n", m_pGnResSave[i].b_aver_pqv1[0], m_pGnResSave[i].e_aver_pqv1[0]);
					fprintf(fp, "\t�й����ֵ     = %6.2fMW               ʱ��           = %s \n", m_pGnResSave[i].max_value.value, PT_GetTimeStringBySeconds(m_pGnResSave[i].max_value.time));
					fprintf(fp, "\t�й���Сֵ     = %6.2fMW               ʱ��           = %s \n", m_pGnResSave[i].min_value.value, PT_GetTimeStringBySeconds(m_pGnResSave[i].min_value.time));
					fprintf(fp, "\t���ڶ�����   = %6.2fMW \n", m_pGnResSave[i].max_value.value - m_pGnResSave[i].min_value.value);

					fprintf(fp, "\n  ����ģʽ��\n");

					fprintf(fp, "%s\n", "  ���         ����ʱ��             ��ֵ(MW)   Ƶ��(Hz)   �����(%)   ���(deg)");
					for (j = 0; j < m_pGnResSave[i].mode1_num; j++)
					{
						sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pGnResSave[i].mode1[j].bgn_t));
						sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pGnResSave[i].mode1[j].end_t));

						fprintf(fp, "  %4d  %10s - %8s   %10.2f  %10.2f  %10.2f  %10.2f\n",
							j + 1,
							strTime1,
							strTime2,
							m_pGnResSave[i].mode1[j].ampl,
							m_pGnResSave[i].mode1[j].freq,
							m_pGnResSave[i].mode1[j].damp,
							m_pGnResSave[i].mode1[j].phase);
					}
					fprintf(fp, "\n");
				}
			}
		}
	}

	if (m_SysResSave.lfo_tr_num > 0)
	{
		for (i = 0; i < m_nTrNum; i++)
		{
			if (m_pTrResSave[i].lfo_b_t > 0 && m_pTrResSave[i].lfo_e_t > 0)
			{
				//if ((int)(m_pTrResSave[i].obj_id/DB_TABLE_BASE) == ACTr_DOT_NO )
				{
					sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_pTrResSave[i].lfo_b_t));
					sprintf(strTime2, "%s", PT_GetTimeStringBySeconds(m_pTrResSave[i].lfo_e_t));

					fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
					fprintf(fp, "\t%s  "INT_REC_FMT"  \n", m_pTrResSave[i].obj_name, INT_REC_VAL(m_pTrResSave[i].obj_id));
					sprintf(m_LogString, "����ʱ��=%d", m_pTrResSave[i].hold_time);
					MessShow(m_LogString, LTINFO);
					fprintf(fp, "\t�񵴷���ʱ��   = %s    ����ʧʱ��   = %s   ����ʱ�� = %s \n", strTime1, strTime2, PT_GetStringOfTimeInterval(m_pTrResSave[i].hold_time * 1000));
					fprintf(fp, "\t��ǰ�й���ֵ = %6.2fMW               ��ʧ���й���ֵ = %6.2fMW \n", m_pTrResSave[i].b_aver_pqv1[0], m_pTrResSave[i].e_aver_pqv1[0]);
					fprintf(fp, "\t�й����ֵ     = %6.2fMW               ʱ��           = %s \n", m_pTrResSave[i].max_value.value, PT_GetTimeStringBySeconds(m_pTrResSave[i].max_value.time));
					fprintf(fp, "\t�й���Сֵ     = %6.2fMW               ʱ��           = %s \n", m_pTrResSave[i].min_value.value, PT_GetTimeStringBySeconds(m_pTrResSave[i].min_value.time));
					fprintf(fp, "\t���ڶ�����   = %6.2fMW \n", m_pTrResSave[i].max_value.value - m_pTrResSave[i].min_value.value);

					fprintf(fp, "\n  ����ģʽ��\n");
					LineModeNum = 0;
					LineModeNum = m_pTrResSave[i].mode1_num;

					fprintf(fp, "%s\n", "  ���         ����ʱ��             ��ֵ(MW)   Ƶ��(Hz)   �����(%)   ���(deg)");
					for (j = 0; j < LineModeNum; j++)
					{
						sprintf(strTime1, "%s", PT_GetHMSTimeStringBySeconds(m_pTrResSave[i].mode1[j].bgn_t));
						sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_pTrResSave[i].mode1[j].end_t));

						fprintf(fp, "  %4d  %10s - %8s   %10.2f  %10.2f  %10.2f  %10.2f\n",
							j + 1,
							strTime1,
							strTime2,
							m_pTrResSave[i].mode1[j].ampl,
							m_pTrResSave[i].mode1[j].freq,
							m_pTrResSave[i].mode1[j].damp,
							m_pTrResSave[i].mode1[j].phase);
					}
					fprintf(fp, "\n");
				}
			}
		}
	}

	fprintf(fp, "\n[[��������]]\n");
	fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
	fprintf(fp, "%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");
	/*fprintf(fp, "\t�������ڳ���   = %6ds    ����ʱ�䳤��   = %6ds \n", m_LfoPara.time_leng, m_LfoPara.int_time );
	//fprintf(fp, "\tPMUƵ��        = %6d֡/��   ����Ƶ��       = %6d \n", MAX_FRA_PER_SEC, m_calc_para.secsamp);
	fprintf(fp, "\tƵ������       = %6.3fHz   Ƶ������       = %6.3fHz \n",
		m_LfoPara.freq_min, m_LfoPara.freq_max );
	fprintf(fp, "\t�澯����ȶ�ֵ = %6.3f%%    Ԥ������ȶ�ֵ = %6.3f%% \n",
		m_LfoPara.lfo_damp, m_LfoPara.flfo_damp );
	//fprintf(fp, "\������й���ֵ  = %6.3fdeg   ��·�й���ֵ = %6.3MW \n",	m_LfoPara.gn_wn_ampl, m_LfoPara.ln_wn_ampl );*/
	// 2008-05-11 by zhw
	//sprintf(logStr,"//**********************LFO������Ϣ****************************//");
	//fprintf(fileptr,"%s\n", logStr);

	//fprintf(fp,"\t\t�����������\t\t\t=\t%d\n",m_LfoPara.mem_max_sec);

	//fprintf(fp,"\t\t����Ƶ��\t\t\t\t=\t%d\n",m_LfoPara.smp_freq);

//     fprintf(fp,"\t\tԤ������ٷֱ�\t\t\t=\t%f\n",m_LfoPara.fjud_wnap_per);

	//fprintf(fp,"\t\t�˲�����\t\t\t\t=\t%d\n",m_LfoPara.flt_len);

	//fprintf(fp,"\t\t�˲�ϵ��\t\t\t\t=\t%f\n",m_LfoPara.flt_coe);

	fprintf(fp, "\t\t�۲�ʱ�䴰��\t\t\t=\t%d\n", m_LfoPara.time_leng);

	fprintf(fp, "\t\t����Ƶ��\t\t\t=\t%d\n", m_LfoPara.smp_freq);

	fprintf(fp, "\t\t��������\t\t\t\t=\t%d\n", m_LfoPara.int_time);

	fprintf(fp, "\t\tƵ������\t\t\t\t=\t%f\n", m_LfoPara.freq_min);

	fprintf(fp, "\t\tƵ������\t\t\t\t=\t%f\n", m_LfoPara.freq_max);

	//     fprintf(fp,"\t\t����ģʽ������ֵ\t\t=\t%f\n",m_LfoPara.stor_damp);
	//     
	//     fprintf(fp,"\t\t���᲻����ֵ\t\t\t=\t%f\n",m_LfoPara.lack_damp);
	//     
	//     fprintf(fp,"\t\tԤ��������ֵ\t\t\t=\t%f\n",m_LfoPara.flfo_damp);
	//     
	//     fprintf(fp,"\t\t�澯������ֵ\t\t\t=\t%f\n", m_LfoPara.lfo_damp);

	fprintf(fp, "\t\t���������ֵ\t\t\t=\t%f\n", m_LfoPara.gn_swn_ampl);

	//     fprintf(fp,"\t\t����������澯�����ֵ\t=\t%f\n", m_LfoPara.gn_wn2_ampl);

	fprintf(fp, "\t\t��·�����ֵ\t\t\t=\t%f\n", m_LfoPara.ln_swn_ampl1);

	//     fprintf(fp,"\t\t��·�����澯�����ֵ\t=\t%f\n", m_LfoPara.ln_wn2_ampl);

	//     fprintf(fp,"\t\t�жϴ���\t\t\t\t=\t%d\n", m_LfoPara.lfo_jud_num);

		//fprintf(fp,"\t\t��¼��ǰʱ��\t\t\t=\t%d\n", m_LfoPara.rcd_pre_t);

		//fprintf(fp,"\t\t��¼�Ӻ�ʱ��\t\t\t=\t%d\n", m_LfoPara.rcd_aft_t);

		//fprintf(fp,"\t\t�������ʱ��\t\t\t=\t%d\n", m_LfoPara.htime_max);

	//     fprintf(fp,"\t\tͬģƵ�ʲ�\t\t\t\t=\t%f\n", m_LfoPara.sm_md_freq);
	//     
	//     fprintf(fp,"\t\tͬģ�����\t\t\t\t=\t%f\n", m_LfoPara.sm_md_damp);
	//     
	//     fprintf(fp,"\t\tͬ���ǶȲ�\t\t\t\t=\t%f\n", m_LfoPara.cohcy_angl);

	fclose(fp);

	return;
}

//////////////////////////////////////////////////////////////////////////
////////////////////////////////// �ļ���� ///////////////////////////////
//  �������� : ������������ģʽ���д���ļ� AllObjMode.txt  
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : inObjMode                ģʽ����
//             ipObjMode                ģʽ��Ϣ
////////////////////////////////////////////////////////////////////////////////
// void CLfoAnaObj::WriteAllObjModeToFile( const int inObjMode, MODE_PART_OBJ *ipObjMode )
// {
//     if (para_FileModeTxt == 0)
//     {
//         return;
//     }
//     else
//     {
//         if ( !ipObjMode || inObjMode<=0 ) return;
//         
//         int ii;
//         char strTime1[30];
//         char strTime2[30];
//         char fileName[MAXBUF];
//         char temp_message[1024];
//         char NameString[64];
//         
// #ifdef _WINDOWS32
//         sprintf( fileName, "%s\\res\\%s.txt", m_RunInfDir,m_LfoResFileName.c_str() );
// #else
//         sprintf( fileName, "%s/res/%s.txt", m_RunInfDir,m_LfoResFileName.c_str() );
// #endif
//         FILE*   fileptr = NULL;
//         if (para_FileModeTxt == 1)
//         {
//             if((fileptr=fopen(fileName,"a+"))==NULL)
//             {
//                 sprintf( m_LogString," WriteAllObjModeToFile() Cannot open the file %s", fileName);
//                 MessShow(m_LogString);
//                 return ;
//             }
//         }
//         else if (para_FileModeTxt == 2)
//         {
//             if((fileptr=fopen(fileName,"w+"))==NULL)
//             {
//                 sprintf( m_LogString," WriteAllObjModeToFile() Cannot open the file %s", fileName);
//                 MessShow(m_LogString);
//                 return ;
//             }
//         }
//         sprintf( strTime1, "%s", PT_GetTimeStringBySeconds(m_CalcStartTime));
//         sprintf( strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime)   );
//         
//         fprintf(fileptr,"\n%s\n","----------------------------------------------------------------------------------------------");
//         sprintf( temp_message, " %s �� %s ����ģʽ��Ϣ", strTime1, strTime2 );
//         fprintf(fileptr,"\n%s\n", temp_message);
//         
//         if ( inObjMode > 0) 
//         {
//             fprintf(fileptr,"\n%s\n", "                  ����ID     ��ֵ   ����    ʵ��    �鲿    ����   ����   Ƶ��    �����   �豸����");
//         }
//         for( ii=0; ii < inObjMode; ii++)
//         {
//         /*if (ii!=0 ) 
//         {
//         if ( ipObjMode[ii].obj_id != ipObjMode[ii-1].obj_id)
//         {
//         fprintf(fileptr,"\n");
//         }
//         }*/
//             if( ipObjMode[ii].obj_id/DB_TABLE_BASE == GN_DEVICE_NO)
//             {
//                 for ( int kk = 0; kk<m_nGnNum; kk++ )
//                 {
//                     if ( ipObjMode[ii].obj_id == m_pGnInf[kk].gn_id ) 
//                     {
//                         strcpy(NameString,  m_pGnInf[kk].gn_name );
//                         break;
//                     }
//                 }
//             }
//             else if( ipObjMode[ii].obj_id/DB_TABLE_BASE == ACLN_DEVICE_NO )
//             {
//                 for ( int kk = 0; kk<m_nLnNum; kk++ )
//                 {
//                     if ( ipObjMode[ii].obj_id == m_pLnInf[kk].acln_id ) 
//                     {
//                         strcpy(NameString,  m_pLnInf[kk].acln_name );
//                         break;
//                     }
//                 }
//             }
//             else if( ipObjMode[ii].obj_id/DB_TABLE_BASE == FAC_INFO_NO )
//             {
//                 for ( int kk = 0; kk<m_nFcNum; kk++ )
//                 {
//                     if ( ipObjMode[ii].obj_id == m_pFcInf[kk].fac_id ) 
//                     {
//                         strcpy(NameString,  m_pFcInf[kk].fac_name );
//                         break;
//                     }
//                 }
//             }
//             
//             sprintf(temp_message, "                %d  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f    %s",
//                 ipObjMode[ii].obj_id,
//                 ipObjMode[ii].ampl, // .mgnt 
//                 ipObjMode[ii].phas*180/PRONY_PI,
//                 ipObjMode[ii].real,
//                 ipObjMode[ii].imag,
//                 ipObjMode[ii].fact,
//                 ipObjMode[ii].engy,
//                 ipObjMode[ii].freq,
//                 ipObjMode[ii].damp,
//                 NameString);
//             
//             fprintf(fileptr,"%s\n", temp_message);
//             
//         }
//         
//         fclose(fileptr);
//         
//         CheckDataFile(fileName);
//         
//         return;
//     }
// 	return;
// }
////////////////////////////////////////////////////////////////////////////////
//  �������� : ��ģ̬�ֲ����д���ļ� ModeShape.txt  
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : inMode                ģʽ����
//             ipMode                ģʽ��Ϣ
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaObj::WriteModeShapeToFile(const int inMode, MODE_CHRCT* ipMode, unsigned char isShowFac)
{
	if (para_FileModeTxt == 0)
	{
		return;
	}
	else
	{
		if (!ipMode || inMode <= 0) return;

		int  ii, jj;
		char strTime1[30];
		char strTime2[30];
		char fileName[MAXBUF];
		char temp_message[1024];

		sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_CalcStartTime));
		sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));

#ifdef _WINDOWS32
		sprintf(fileName, "%s\\res\\%s.txt", m_RunInfDir, m_LfoResFileName.c_str());
#else
		sprintf(fileName, "%s/res/%s.txt", m_RunInfDir, m_LfoResFileName.c_str());
#endif

		FILE* fileptr = NULL;
		if (para_FileModeTxt == 1)
		{
			if ((fileptr = fopen(fileName, "a+")) == NULL)
			{
				sprintf(m_LogString, " WriteModeShapeToFile() Cannot open the file %s", fileName);
				MessShow(m_LogString);
				return;
			}
		}
		else if (para_FileModeTxt == 2)
		{
			if ((fileptr = fopen(fileName, "w+")) == NULL)
			{
				sprintf(m_LogString, " WriteModeShapeToFile() Cannot open the file %s", fileName);
				MessShow(m_LogString);
				return;
			}
		}
		fprintf(fileptr, "\n%s\n", "----------------------------------------------------------------------------------------------");
		sprintf(temp_message, " %s �� %s ��Ҫģʽ��Ϣ", strTime1, strTime2);
		fprintf(fileptr, "\n%s\n", temp_message);

		fprintf(fileptr, "%s\n", "ϵͳ��Ҫģʽ��");
		fprintf(fileptr, "%s\n", "           ģʽID    Ƶ��   �����   ʵ��   �鲿   engyGn   engyLn");
		for (ii = 0; ii < inMode; ii++)
		{
			sprintf(temp_message, "                %d  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f",
				ipMode[ii].mode_id,
				ipMode[ii].freq,
				ipMode[ii].damp,
				ipMode[ii].real,
				ipMode[ii].imag,
				ipMode[ii].engyGn,
				ipMode[ii].engyLn);
			fprintf(fileptr, "%s\n", temp_message);
		}

		fprintf(fileptr, "%s\n", "ģ̬�ֲ���");
		for (ii = 0; ii < inMode; ii++)
		{
			sprintf(temp_message, "      ģʽID= %d  Ƶ��=%6.3f  �����=%6.3f",
				ipMode[ii].mode_id,
				ipMode[ii].freq,
				ipMode[ii].damp);
			fprintf(fileptr, "\n%s\n", temp_message);

			/*		if ( ipMode[ii].nPartGn > 0)
			{
			fprintf(fileptr,"    �����������%d\n", ipMode[ii].nPartGn);
			fprintf(fileptr,"%s\n", "                  ����ID     ��ֵ   ����    ʵ��    �鲿    ����   ����   Ƶ��    �����");
			for( jj=0; jj < ipMode[ii].nPartGn; jj++)
			{
			sprintf( temp_message,"                %d  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f",
			ipMode[ii].PartGn[jj].obj_id,
			ipMode[ii].PartGn[jj].mgnt,
			ipMode[ii].PartGn[jj].phas*180/PRONY_PI,
			ipMode[ii].PartGn[jj].real,
			ipMode[ii].PartGn[jj].imag,
			ipMode[ii].PartGn[jj].fact,
			ipMode[ii].PartGn[jj].engy,
			ipMode[ii].PartGn[jj].freq,
			ipMode[ii].PartGn[jj].damp);
			fprintf(fileptr,"%s\n", temp_message);
			}
			}
			*/
			//int   area_id = 0;
			char  area_name[64];
			char  GnNameString[64];
			if (ipMode[ii].nPartGn > 0)
			{
				fprintf(fileptr, "    �����������%d\n", ipMode[ii].nPartGn);
				fprintf(fileptr, "%s\n", "                  ����ID     ��ֵ   ����    ʵ��    �鲿    ����   ����   Ƶ��    �����  ����Ⱥ   ��������      С��  ");
				for (jj = 0; jj < ipMode[ii].nPartGn; jj++)
				{
					memset(GnNameString, 0, 64 * sizeof(char));
					memset(area_name, 0, 64 * sizeof(char));
					for (int kk = 0; kk < m_nGnNum; kk++)
					{
						if (ipMode[ii].PartGn[jj].obj_id == m_pGnInf[kk].gn_id)
						{
							strcpy(area_name, m_pGnInf[kk].area_name);
							strcpy(GnNameString, m_pGnInf[kk].gn_name);
							break;
						}
					}

					sprintf(temp_message, "                "INT_REC_FMT"  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %d  %s   %s",
						INT_REC_VAL(ipMode[ii].PartGn[jj].obj_id),
						ipMode[ii].PartGn[jj].ampl,//mgnt,
						ipMode[ii].PartGn[jj].phas * 180 / PRONY_PI,
						ipMode[ii].PartGn[jj].real,
						ipMode[ii].PartGn[jj].imag,
						ipMode[ii].PartGn[jj].fact,
						ipMode[ii].PartGn[jj].engy,
						ipMode[ii].PartGn[jj].freq,
						ipMode[ii].PartGn[jj].damp,
						ipMode[ii].PartGn[jj].clus,
						GnNameString,
						area_name);
					fprintf(fileptr, "%s\n", temp_message);
				}
			}
			if (ipMode[ii].nPartLn > 0)
			{
				fprintf(fileptr, "    �ɹ���·����%d\n", ipMode[ii].nPartLn);
				fprintf(fileptr, "%s\n", "                  ��·ID    ��ֵ    ����    ʵ��    �鲿    ����   ����   Ƶ��    �����  ����Ⱥ    ��·����");
				for (jj = 0; jj < ipMode[ii].nPartLn; jj++)
				{
					memset(GnNameString, 0, sizeof(char));
					for (int kk = 0; kk < m_nLnNum; kk++)
					{
						if (ipMode[ii].PartLn[jj].obj_id == m_pLnInf[kk].acln_id)
						{
							strcpy(GnNameString, m_pLnInf[kk].acln_name);
							break;
						}
					}
					sprintf(temp_message, "                "INT_REC_FMT"  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %d   %s",
						INT_REC_VAL(ipMode[ii].PartLn[jj].obj_id),
						ipMode[ii].PartLn[jj].ampl,//mgnt,
						ipMode[ii].PartLn[jj].phas * 180 / PRONY_PI,
						ipMode[ii].PartLn[jj].real,
						ipMode[ii].PartLn[jj].imag,
						ipMode[ii].PartLn[jj].fact,
						ipMode[ii].PartLn[jj].engy,
						ipMode[ii].PartLn[jj].freq,
						ipMode[ii].PartLn[jj].damp,
						ipMode[ii].PartLn[jj].clus,
						GnNameString);
					fprintf(fileptr, "%s\n", temp_message);
				}
			}

			if (isShowFac == 1)
			{
				if (ipMode[ii].nPartFc > 0)
				{
					fprintf(fileptr, "    ���ĳ�վ����%d\n", ipMode[ii].nPartFc);
					fprintf(fileptr, "%s\n", "                  ��վID    ��ֵ    ����    ʵ��    �鲿    ����   ����   Ƶ��    �����  ����Ⱥ    ��վ����");
					for (jj = 0; jj < ipMode[ii].nPartFc; jj++)
					{
						for (int kk = 0; kk < m_nGnNum; kk++)
						{
							if (ipMode[ii].PartFc[jj].obj_id == m_pFcInf[kk].fac_id)
							{
								strcpy(GnNameString, m_pFcInf[kk].fac_name);
								break;
							}
						}
						sprintf(temp_message, "                "INT_REC_FMT"  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %6.3f  %d    %s",
							INT_REC_VAL(ipMode[ii].PartFc[jj].obj_id),
							ipMode[ii].PartFc[jj].ampl,//mgnt,
							ipMode[ii].PartFc[jj].phas * 180 / PRONY_PI,
							ipMode[ii].PartFc[jj].real,
							ipMode[ii].PartFc[jj].imag,
							ipMode[ii].PartFc[jj].fact,
							ipMode[ii].PartFc[jj].engy,
							ipMode[ii].PartFc[jj].freq,
							ipMode[ii].PartFc[jj].damp,
							ipMode[ii].PartFc[jj].clus,
							GnNameString);
						fprintf(fileptr, "%s\n", temp_message);
					}
				}
			}

		}
		fclose(fileptr);

		//�ж��ļ���С
		CheckDataFile(fileName);

		return;
	}
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ���������ڽ�������д���ļ� LfoCentor.txt  
//  ������� : 
//  ��    ע :
//  �� �� ֵ :
//  ����˵�� : inMode                ģʽ����
//             ipMode                ģʽ��Ϣ
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaObj::WriteLfoCenterToFile(LFO_SYS_ANA SysAna, int nFcProInf, LFO_FC_ANA* pFcProInf)
{
	if (para_FileModeTxt == 0)
	{
		return;
	}
	else
	{
		int ii, jj;
		char strTime1[30];
		char strTime2[30];
		char fileName[MAXBUF];
		char temp_message[1024];

		sprintf(strTime1, "%s", PT_GetTimeStringBySeconds(m_CalcStartTime));
		sprintf(strTime2, "%s", PT_GetHMSTimeStringBySeconds(m_CurGpsTime));

#ifdef _WINDOWS32
		sprintf(fileName, "%s\\res\\%s.txt", m_RunInfDir, m_LfoResFileName.c_str());
#else
		sprintf(fileName, "%s/res/%s.txt", m_RunInfDir, m_LfoResFileName.c_str());
#endif

		FILE* fileptr = NULL;
		if (para_FileModeTxt == 1)
		{
			if ((fileptr = fopen(fileName, "a+")) == NULL)
			{
				sprintf(temp_message, " WriteLfoCenterToFile() Cannot open the file %s", fileName);
				MessShow(temp_message);
				return;
			}
		}
		else if (para_FileModeTxt == 2)
		{
			if ((fileptr = fopen(fileName, "a+")) == NULL)
			{
				sprintf(temp_message, " WriteLfoCenterToFile() Cannot open the file %s", fileName);
				MessShow(temp_message);
				return;
			}
		}
		fprintf(fileptr, "\n%s\n", "----------------------------------------------------------------------------------------------");
		sprintf(temp_message, " %s �� %s %s", strTime1, strTime2, SysAna.center_desc);
		fprintf(fileptr, "\n%s\n", temp_message);

		if (!VALID_INT_REC(SysAna.center_id))
		{
			fclose(fileptr);
			return;
		}

		int nTempData;
		LFO_FC_ANA* pTempData = NULL;
		nTempData = 0;
		pTempData = (LFO_FC_ANA*)calloc(2 * para_nAnaMaxFcNum, sizeof(LFO_FC_ANA));
		memset((void*)pTempData, 0, 2 * para_nShwMaxFcNum * sizeof(LFO_FC_ANA));

		for (ii = 0; ii < nFcProInf; ii++)
		{
			if (pFcProInf[ii].lfo_is_cntr == MENU_LFO_MON_YES)
			{
				pTempData[nTempData].fac_id = pFcProInf[ii].fac_id;
				for (jj = 0; jj < m_nFcNum; jj++)
				{
					if (pTempData[nTempData].fac_id == m_pFcInf[jj].fac_id)
					{
						strcpy(pTempData[nTempData].fac_name, m_pFcInf[jj].fac_name);
						break;
					}
				}

				pTempData[nTempData].lfo_v_ampl = pFcProInf[ii].lfo_v_ampl;
				pTempData[nTempData].lfo_clus = pFcProInf[ii].lfo_clus;
				pTempData[nTempData].fact = pFcProInf[ii].fact;
				nTempData = nTempData + 1;
			}
		}

		//  ��fact����
		qsort((void*)pTempData, nTempData, sizeof(LFO_FC_ANA), LfoCenterCompareByFact);

		fprintf(fileptr, "%s\n", "�����ĳ�վ��");
		fprintf(fileptr, "%s\n", "           ��վID  ��վ����    ��ѹ����   ����Ⱥ");
		for (ii = 0; ii < nTempData; ii++)
		{
			sprintf(temp_message, "                "INT_REC_FMT"  %s  %6.3f  %d",
				INT_REC_VAL(pTempData[ii].fac_id),
				pTempData[ii].fac_name,
				pTempData[ii].lfo_v_ampl,
				pTempData[ii].lfo_clus);
			fprintf(fileptr, "%s\n", temp_message);
		}

		fclose(fileptr);
		CheckDataFile(fileName);

		if (pTempData) { free(pTempData); pTempData = NULL; }
		return;
	}
}
// 2008-06-24 by zhw
//  ɽ��Ӧ��ϵͳ�ӿ��ļ�
void CLfoAnaObj::WriteMonResult()
{
	// #ifdef _WAMAP_
	//     if( para_SaveXmlFile == 0 )
	//     {
	//         return ;
	//     }
	// 
	// #ifndef  _WINDOWS32
	//     if(PODB_IsOnDuty(g_appNo, g_ctxNo) != 1 )// �ж��Ƿ�����������������Ϣ
	//     {
	//         sprintf( m_LogString, "WriteMonResult(): ��������WAMAP����,�˳��ӿ�Ӧ��ϵͳ�ӿ��ļ�����!" ); 
	//         MessShow( m_LogString );
	//         return;
	//     }
	// #endif
	// 
	//     int     ii, jj, kk     = 0 ;
	//     char    SaveFileDir[MAXBUF];
	//     char    SaveFileName[MAXBUF];
	//     char    AlertType[3];
	//     char    STime[28],ETime[28];
	//     KEY_ID_STRU keyid_temp;
	// 
	//     sprintf(SaveFileDir,"%s",para_XmlFileDir);
	//     sprintf(STime,"%s",PT_GetNoSpaceMarkTimeStringBySeconds(m_CurGpsTime-m_LfoPara.time_leng));
	//     sprintf(ETime,"%s",PT_GetNoSpaceMarkTimeStringBySeconds(m_CurGpsTime));
	// #ifndef _WINDOWS32
	//     sprintf(SaveFileName,"%s/WAMS_LFO_%s.xml",SaveFileDir,STime);
	// #else
	//     sprintf(SaveFileName,"%s\\WAMS_LFO_%s.xml",SaveFileDir,STime);
	// #endif
	//     
	//     FILE *  fp = fopen(SaveFileName, "w+");
	//     if ( !fp )
	//     {
	//         sprintf( m_LogString, "WriteMonResult():: file open error");
	//         MessShow( m_LogString );
	//         return;
	//     }
	//     
	//     if(m_FormResSaveStat == L_FST_FORM_RESULTE )
	//     {
	//         sprintf(AlertType,"LSS");
	//     }
	//     else if( m_FormResSaveStat == L_SnD_FORM_RESULTE )
	//     {
	//         sprintf(AlertType,"LSD");
	//     }
	//     else if( m_FormResSaveStat == L_END_FORM_RESULTE )
	//     {
	//         sprintf(AlertType,"LSE");
	//     }
	// 
	//     fprintf(fp,"<?xml version=\"1.0\" encoding=\"GB2312\"?><wams>");
	//     fprintf(fp,"<alert type=\"%s\" rec_count=\"%d\" S_time=\"%s\" E_time=\"%s\" Num_gen=\"%d\" Num_acln=\"%d\">",\
	//                 AlertType,m_nGnWarn+m_nLnWarn,STime,ETime,m_nGnWarn,m_nLnWarn);
	// 
	//     if( m_nGnWarn > 0 )
	//     {
	//         for( ii=0; ii<m_nGnNum; ii++ )
	//         {
	//             if( m_pGnMon[ii].stat > MENU_LFO_DEV_STS_SCT )
	//             {
	//                 keyid_temp.record_id = m_pGnMon[ii].gn_id;
	//                 //keyid_temp.column_id = COL_ID_GN_PWR;
	//                 fprintf(fp,"<rec id=\"%d_%d\" st_id=\"%d\" wave_a=\"%6.2f\" wave_f=\"%6.2f\" wave_r=\"%6.2f\" />",\
	//                 keyid_temp.record_id,keyid_temp.column_id,m_pGnInf[ii].fac_id,m_pGnMon[ii].lead_ampl,m_pGnMon[ii].lead_freq,m_pGnMon[ii].lead_damp);
	//             }
	//         }
	//     }
	//     if( m_nLnWarn > 0 )
	//     {
	//         for( ii=0; ii<m_nLnNum; ii++ )
	//         {
	//             if( m_pLnMon[ii].stat > MENU_LFO_DEV_STS_SCT )
	//             {
	//                 keyid_temp.record_id = m_pLnMon[ii].slct_dot_id;
	//                 //keyid_temp.column_id = COL_ID_LN_PWR;
	//                 fprintf(fp,"<rec id=\"%d_%d\" st_id=\"%d\" wave_a=\"%6.2f\" wave_f=\"%6.2f\" wave_r=\"%6.2f\" />",\
	//                 keyid_temp.record_id,keyid_temp.column_id,m_pLnMon[ii].slct_dot_id,m_pLnMon[ii].lead_ampl,m_pLnMon[ii].lead_freq,m_pLnMon[ii].lead_damp);
	//             }
	//         }
	//     }
	// 
	//     fprintf(fp,"</alert></wams>");
	// 
	//     fclose(fp);
	// 
	//     m_XmlFileName.push_back(SaveFileName);
	//     if( m_FormResSaveStat == L_END_FORM_RESULTE )
	//     {
	//         CreateHisFtpFile(m_XmlFileName);
	//         for(ii=0; ii<m_XmlFileName.size(); ii++)
	//         {
	//             remove(m_XmlFileName[ii].c_str());
	//         }
	// 
	//         if(!m_XmlFileName.empty())
	//         {
	//             m_XmlFileName.clear();
	//         }
	//     }
	// #endif
	return;
}

// ������ �����ļ��ӿ�
void CLfoAnaObj::CreateHisFtpFile(vector<string>& fileName)
{
	FILE* fp;
	fp = fopen("/users/ems/.netrc", "w");
	if (!fp) {
		printf("!!OPEN .NETRC ERROR!");
		return;
	}
	fprintf(fp, "%s\n", "machine cim_ftp login ems password open3000 macdef init");
	fprintf(fp, "%s\n", "bin");
	fprintf(fp, "%s\n", "cd /users/ems/open2000e/wams_lfo_to_yj");
	for (int ii = 0; ii < fileName.size(); ii++)//��ע�⣺fileName�Ǵ��о���·�����ļ�����
	{
		fprintf(fp, "put %s\n", fileName[ii].c_str());
	}
	fprintf(fp, "%s\n\n", "bye");

	fclose(fp);

	char    cmdStr[MAXBUF];
	sprintf(cmdStr, "cp /users/ems/.netrc %s/%s", para_XmlFileDir, "TestFtp.txt");
	MessShow(cmdStr);

#if defined(_UNIX)
	system(cmdStr);

	system("chmod 600 /users/ems/.netrc");
	system("/usr/bin/ftp cim_ftp>/users/ems/cim_ftp.log");
	sprintf(cmdStr, "/usr/bin/ftp cim_ftp>/users/ems/cim_ftp.log");
	MessShow(cmdStr);
#endif
}

void CLfoAnaObj::SendLfoResFile()
{
#ifdef  _WAMAP_
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)// �ж��Ƿ�����������������Ϣ
	{
		sprintf(m_LogString, "SendLfoResFile(): ��������WAMAP����,�˳��ӿ��ļ�����!");
		MessShow(m_LogString);
		return;
	}
#endif
	char    cmdStr[MAXBUF];
	if (para_SaveResFile == 0)
	{
		return;
	}
	else if (para_SaveResFile == 2)
	{
		sprintf(cmdStr, "%s", para_FtpCmd);
		system(cmdStr);
		MessShow(cmdStr);
		return;
	}

	char tmpFileName[MAXBUF];
#ifndef _WINDOWS32
	sprintf(tmpFileName, "%s/%s", m_SaveFileDir, para_ResFileName);
#else
	sprintf(tmpFileName, "%s\\%s", m_SaveFileDir, para_ResFileName);
#endif
	FILE* fp;
	fp = fopen(tmpFileName, "w");
	if (!fp)
	{
		sprintf(m_LogString, "����ļ�����ʧ��[%s]", tmpFileName);
		MessShow(m_LogString);
		return;
	}

	// ��Ҫ��Ϣ
	int tmpstat = -1;
	int ii;
	if (m_SysMon.stat == MENU_LFO_DEV_STS_SCT)
	{
		tmpstat = 1;
	}
	else if (m_SysMon.stat > MENU_LFO_DEV_STS_SCT)
	{
		tmpstat = 2;
	}
	else
	{
		tmpstat = 3;
	}
	fprintf(fp, "<�񵴸�Ҫ��Ϣ>\n");
	fprintf(fp, "@ id   type  rec_count  s_time  e_time   num_gen  num_acln  num_tr\n");
	fprintf(fp, "//��� ��״̬��ʶ  �����豸��  ���ּ��㿪ʼʱ�� ���ۼ������ʱ�� ��Ƶ��  ������� �񵴻������ �񵴽�����·����  �񵴱�ѹ������\n");
	fprintf(fp, "# %d  %d %d %s %s %d %d %d\n", 1, tmpstat, m_SysMon.sso_gn_num + m_SysMon.sso_ln_num + m_SysMon.sso_tr_num,
		PT_GetNoSpaceMarkTimeStringBySeconds(m_CalcStartTime), PT_GetNoSpaceMarkTimeStringBySeconds(m_CurGpsTime),
		m_SysMon.sso_gn_num, m_SysMon.sso_ln_num, m_SysMon.sso_tr_num
	);
	fprintf(fp, "</�񵴸�Ҫ��Ϣ>\n");

	fprintf(fp, "<���豸��Ϣ>\n");
	fprintf(fp, "@id  dev_name  fac_name  dev_type  power  ampl\n");
	fprintf(fp, "//���  �豸���� ��վ���� �豸����  �񵴷�ֵ ��Ƶ��\n");
	int tmpnum = 1;
	char tmpchar[64];
	for (ii = 0; ii < m_nGnNum; ii++)
	{
		if (m_pGnMon[ii].stat > MENU_LFO_DEV_STS_SCT)
		{
			fprintf(fp, "# %d %s %s %d %f %f\n", tmpnum, m_pGnInf[ii].gn_name, m_pGnInf[ii].fac_name, 1, m_pGnMon[ii].lead_ampl, m_pGnMon[ii].lead_freq);
			tmpnum++;
		}
	}

	for (ii = 0; ii < m_nLnNum; ii++)
	{
		if (m_pLnMon[ii].stat > MENU_LFO_DEV_STS_SCT)
		{
			if (m_pLnMon[ii].slct_dot_id == m_pLnInf[ii].dot_id_1)
			{
				sprintf(tmpchar, "%s", m_pLnInf[ii].fac_name_1);
			}
			else
			{
				sprintf(tmpchar, "%s", m_pLnInf[ii].fac_name_2);
			}
			fprintf(fp, "# %d %s %s %d %f %f\n", tmpnum, m_pLnInf[ii].acln_name, tmpchar, 2, m_pLnMon[ii].lead_ampl, m_pLnMon[ii].lead_freq);
			tmpnum++;
		}
	}

	for (ii = 0; ii < m_nTrNum; ii++)
	{
		if (m_pTrMon[ii].stat > MENU_LFO_DEV_STS_SCT)
		{
			fprintf(fp, "# %d %s %s %d %f %f\n", tmpnum, m_pTrInf[ii].tr_name, m_pTrInf[ii].tr_name, 3, m_pTrMon[ii].lead_ampl, m_pTrMon[ii].lead_freq);
			tmpnum++;
		}
	}
	fprintf(fp, "</���豸��Ϣ>\n");

	fprintf(fp, "<����������Ϣ>\n");
	fprintf(fp, "@id   time    desc\n");
	fprintf(fp, "//���  ʱ��   ����������Ϣ\n");

	// ��������
#ifdef _LFO_ASST_DECI_
	if (GetAsstDeci())
	{
		for (ii = 0; ii < m_nAsstDeci; ii++)
		{
			fprintf(fp, "# %d %s %s\n", ii + 1, PT_GetNoSpaceMarkTimeStringBySeconds(m_pAsstDeci[ii].occur_time), m_pAsstDeci[ii].ctl_act_dsec);
		}
	}
#endif
	fprintf(fp, "</����������Ϣ>\n");
	fclose(fp);

	// �����ļ�
	char tmpNetrc[MAXBUF];
#ifndef _WINDOWS32
	sprintf(tmpNetrc, "%s", "/users/ems/.netrc");
#else
	sprintf(tmpNetrc, "%s\\%s", m_SaveFileDir, "netrc.txt");
#endif

	fp = fopen(tmpNetrc, "w");

	if (!fp) {
		sprintf(m_LogString, "%s", "!!OPEN .NETRC ERROR!");
		MessShow(m_LogString);
		return;
	}

	fprintf(fp, "machine %s login %s password %s macdef init\n", para_FtpServer, para_FtpUser, para_FtpPsw);
	fprintf(fp, "%s\n", "bin");
	fprintf(fp, "cd %s\n", para_FtpDir);
	fprintf(fp, "lcd %s\n", m_SaveFileDir);
	fprintf(fp, "put %s\n", para_ResFileName);
	fprintf(fp, "%s\n\n", "bye");

	fclose(fp);

#if defined(_UNIX)
	system("chmod 600 /users/ems/.netrc");
	sprintf(cmdStr, "/usr/bin/ftp %s>%s/%s", para_FtpServer, m_SaveFileDir, "ftp.log");
	system(cmdStr);
	MessShow(cmdStr);
#endif

	// ɾ���ű�
	if (remove(tmpNetrc) == -1)
	{
		sprintf(m_LogString, "ɾ���ű�ʧ��[%s]", tmpNetrc);
		MessShow(m_LogString);
	}
	else
	{
		sprintf(m_LogString, "ɾ���ű��ɹ�[%s]", tmpNetrc);
		MessShow(m_LogString);
	}
	return;
}

int CLfoAnaObj::ReadNewData(const string tmpfilename, int& realNum, float& t_delta, int& nDataTime)
{
	CDataFile df;
	if (df.ReadFileData(tmpfilename.c_str(), m_LfoPara.data_pos, m_LfoPara.time_leng, m_LfoPara.int_time) == false)
	{
		sprintf(m_LogString, "��ȡ�����ļ�ʧ��[%s][%s]", tmpfilename.c_str(), df.GetLastError());
		MessShow(m_LogString, LTERROR);
		PT_DelaySecond(2);
		return -1;
	}

	// ��ȡ����ʱ��
	nDataTime = df.m_DataHead.gps_time;

	int ii, jj, kk, tableno, nColNum, nflag;
	int tmpFcNum = 0, tmpGnNum = 0, tmpLnNum = 0, tmpTrNum = 0;
	float tmpdev_voltbase = 0.0;
	string::size_type idx;
	string tmpdevname, tmpColName;
	vector<double> v2d_data;
	string tmpstring;
	for (ii = 0; ii < (int)df.m_DataHead.vec_devid_s.size(); ii++)
	{
		// 		sprintf(m_LogString,"start[%d]",ii);
		// 		MessShow(m_LogString);
				// ��ȡ�豸ID
		tmpstring = df.m_DataHead.vec_devid_s[ii];
		INT_REC tmpdev_id = ATOI(tmpstring.c_str());

		if ((int)df.m_DataHead.vec_devvolt_s.size() > ii)
		{
			tmpstring.clear();
			tmpstring = df.m_DataHead.vec_devvolt_s[ii];
			tmpdev_voltbase = atof(tmpstring.c_str());
		}
		// �жϲ������
		// ��ȡ�������
		// ȥ��ǰ����"����ʱ��,����"
		tmpstring.clear();
		if (df.GetNumberOfVariables() >= ii + 2)
		{
			df.GetVariableName(ii + 2, tmpstring);
		}

		idx = tmpstring.find_last_of("-");
		// ��ȡ�豸����
		if (idx != string::npos)
		{
			tmpdevname = tmpstring.substr(0, idx);
		}

		// �жϲ������
		tmpColName = tmpstring.substr(idx + 1);
		// ȥ��ÿ��ĩβ��","
		idx = tmpColName.find_first_of(",");
		if (idx != string::npos)
		{
			tmpColName = tmpColName.substr(0, idx);
		}
		if (tmpColName == "00P")
		{
			// �й�
			nColNum = 12;
		}
		else if (tmpColName == "00Q")
		{
			// �޹�
			nColNum = 13;
		}
		else if (tmpColName == "UAV���")
		{
			// ��ѹ���
			nColNum = 1;
		}
		else if (tmpColName == "UAV��ֵ")
		{
			// ��ѹ��ֵ
			nColNum = 0;
		}
		else if (tmpColName == "SSF")
		{
			// ��ѹ��ֵ
			nColNum = 20;
		}
		else if (tmpColName == "SSP")
		{
			// ��ѹ��ֵ
			nColNum = 21;
		}
		else if (tmpColName == "SSO")
		{
			// ��ѹ��ֵ
			nColNum = 22;
		}
		else if (tmpColName == "EEV���")
		{
			// ���������
			nColNum = 16;
		}
		else
		{
			// δ���壬Ĭ��Ϊ�й�
			nColNum = -1;
			//			nColNum = 12;
			sprintf(m_LogString, "�������������[%s]", tmpColName.c_str());
			MessShow(m_LogString, LTERROR);
			continue;
		}

		// ��ȡ�豸����
		if (df.GetData(ii + 2, v2d_data) == -1)
		{
			sprintf(m_LogString, "[%s]["INT_REC_FMT"]��ȡ���ݴ���[%s]",
				tmpstring.c_str(), INT_REC_VAL(tmpdev_id), df.GetLastError());
			MessShow(m_LogString, LTDEBUG);
		}

		// �ж��豸����
		tableno = PODB_GetTabelNoByDevId(tmpdev_id);
		// 		sprintf(m_LogString,"�豸����Ϊ[%d]",tableno);
		// 		MessShow(m_LogString,LTINFO);
#ifdef _NUSP
		if (tableno == PLANT_INFO_NO_TAB || tableno == SUBSTATION_INFO_NO_TAB)
#else
		if (tableno == FAC_INFO_NO_TAB)
#endif // _NUSP		
		{
			// ��վ
#ifdef _WAMAP_
			for (jj = 0; jj < m_nFcNum; jj++)
			{
				if (tmpdev_id == m_pFcInf[jj].fac_id)
				{
					m_pFcCtl[jj].is_monitor = MENU_LFO_MON_YES;
					if (nColNum == 0)
					{
						// ��ѹ��ֵ
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pFcDat[jj].u1v[kk] = (float)v2d_data[kk];
						}
					}
					else if (nColNum == 1)
					{
						// ��ѹ���
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pFcDat[jj].u1a[kk] = (float)v2d_data[kk];
						}
					}

					break;
				}
			}
#else
			// �ж��Ƿ������豸
			nflag = -1;
			for (jj = 0; jj < tmpFcNum; jj++)
			{
				if (tmpdev_id == m_pFcInf[jj].fac_id)
				{
					nflag = jj;
					break;
				}
			}

			if (nflag != -1)
			{
				// �����豸����������
				if (nColNum == 0)
				{
					// ��ѹ��ֵ
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pFcDat[nflag].u1v[kk] = (float)v2d_data[kk];
					}
				}
				else if (nColNum == 1)
				{
					// ��ѹ���
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pFcDat[nflag].u1a[kk] = (float)v2d_data[kk];
					}
				}
			}
			else
			{
				// û���豸����Ҫ����
				m_pFcInf[tmpFcNum].fac_id = tmpdev_id;
				m_pFcDat[tmpFcNum].fac_id = tmpdev_id;
				m_pFcCtl[tmpFcNum].fac_id = tmpdev_id;
				m_pFcMon[tmpFcNum].fac_id = tmpdev_id;
				sprintf(m_pFcInf[tmpFcNum].fac_name, "%s", tmpdevname.c_str());
				m_pFcCtl[tmpFcNum].is_monitor = MENU_LFO_MON_YES;
				if (nColNum == 0)
				{
					// ��ѹ��ֵ
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pFcDat[tmpFcNum].u1v[kk] = (float)v2d_data[kk];
					}
				}
				else if (nColNum == 1)
				{
					// ��ѹ���
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pFcDat[tmpFcNum].u1a[kk] = (float)v2d_data[kk];
					}
				}
				tmpFcNum++;
			}
#endif
		}
		else if (tableno == GN_DEVICE_NO_TAB)
		{
			// �����
#ifdef _WAMAP_
			for (jj = 0; jj < m_nGnNum; jj++)
			{
				if (tmpdev_id == m_pGnInf[jj].gn_id)
				{
					m_pGnCtl[jj].is_monitor = MENU_LFO_MON_YES;
					if (nColNum == 20)
					{
						// 						sprintf(m_LogString,"��������Ϊ����[%ld]��SSF,������[%d]",tmpdev_id,(int)v2d_data.size());
						// 						MessShow(m_LogString,LTINFO);
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pGnDat[jj].freq[kk] = (float)v2d_data[kk];
						}
					}
					else if (nColNum == 21)
					{
						// 						sprintf(m_LogString,"��������Ϊ����[%ld]��SSP,������[%d]",tmpdev_id,(int)v2d_data.size());
						// 						MessShow(m_LogString,LTINFO);
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pGnDat[jj].ampl[kk] = (float)v2d_data[kk];
						}
					}
					else if (nColNum == 22)
					{
						// 						sprintf(m_LogString,"��������Ϊ����[%ld]��SSO,������[%d]",tmpdev_id,(int)v2d_data.size());
						// 						MessShow(m_LogString,LTINFO);
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pGnDat[jj].sso[kk] = (int)v2d_data[kk];
						}
					}
					else if (nColNum == 12)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pGnDat[jj].pwr[kk] = (float)v2d_data[kk];
						}
					}

					break;
				}
			}
#else
			// �ж��Ƿ������豸
			nflag = -1;
			for (jj = 0; jj < tmpGnNum; jj++)
			{
				if (tmpdev_id == m_pGnInf[jj].gn_id)
				{
					nflag = jj;
					break;
				}
			}

			if (nflag != -1)
			{
				sprintf(m_LogString, "�����豸��������");
				MessShow(m_LogString, LTINFO);
				// �����豸����������
				if (nColNum == 12)
				{
					// �й�
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pGnDat[nflag].pwr[kk] = (float)v2d_data[kk];
					}
				}
				else if (nColNum == 16)
				{
					// ����
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pGnDat[nflag].ang[kk] = (float)v2d_data[kk];
					}
				}
			}
			else
			{
				// û���豸����Ҫ����
				sprintf(m_LogString, "û���豸����Ҫ����");
				MessShow(m_LogString, LTINFO);
				m_pGnInf[tmpGnNum].gn_id = tmpdev_id;
				m_pGnDat[tmpGnNum].gn_id = tmpdev_id;
				m_pGnCtl[tmpGnNum].gn_id = tmpdev_id;
				m_pGnMon[tmpGnNum].gn_id = tmpdev_id;
				sprintf(m_pGnInf[tmpGnNum].gn_name, "%s", tmpdevname.c_str());
				m_pGnCtl[tmpGnNum].is_monitor = MENU_LFO_MON_YES;
				if (nColNum == 12)
				{
					// �й�
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pGnDat[tmpGnNum].pwr[kk] = (float)v2d_data[kk];
					}
				}
				else if (nColNum == 16)
				{
					// ����
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pGnDat[tmpGnNum].ang[kk] = (float)v2d_data[kk];
					}
				}
				tmpGnNum++;
			}
#endif
		}
		else if (tableno == ACLN_DEVICE_NO_TAB || tableno == ACLN_DOT_NO_TAB)
		{
			// ������·
#ifdef _WAMAP_
			for (jj = 0; jj < m_nLnNum; jj++)
			{
				if (tmpdev_id == m_pLnInf[jj].acln_id)
				{
					m_pLnCtl[jj].is_monitor = MENU_LFO_MON_YES;
					if (nColNum == 20)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pLnDat[jj].freq[kk] = (float)v2d_data[kk];
						}
					}
					else if (nColNum == 21)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pLnDat[jj].ampl[kk] = (float)v2d_data[kk];
						}
					}
					else if (nColNum == 22)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pLnDat[jj].sso[kk] = (int)v2d_data[kk];
						}
					}
					else if (nColNum == 12)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pLnDat[jj].pwr[kk] = (float)v2d_data[kk];
						}
					}

					break;
				}
			}
#else
			// �ж��Ƿ������豸
			nflag = -1;
			for (jj = 0; jj < tmpLnNum; jj++)
			{
				if (tmpdev_id == m_pLnInf[jj].acln_id)
				{
					nflag = jj;
					break;
				}
			}

			if (nflag != -1)
			{
				// �����豸����������
				if (nColNum == 12)
				{
					// �й�
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pLnDat[nflag].pwr[kk] = (float)v2d_data[kk];
					}
				}
			}
			else
			{
				m_pLnInf[tmpLnNum].acln_id = tmpdev_id;
				m_pLnDat[tmpLnNum].acln_id = tmpdev_id;
				m_pLnCtl[tmpLnNum].acln_id = tmpdev_id;
				m_pLnMon[tmpLnNum].acln_id = tmpdev_id;
				//���ӻ�ȡ��ѹ�ȼ�
				m_pLnInf[tmpLnNum].vbase = tmpdev_voltbase;
				m_pLnMon[tmpLnNum].vbase = tmpdev_voltbase;
				sprintf(m_pLnInf[tmpLnNum].acln_name, "%s", tmpdevname.c_str());
				m_pLnCtl[tmpLnNum].is_monitor = MENU_LFO_MON_YES;
				if (nColNum == 12)
				{
					// �й�
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pLnDat[tmpLnNum].pwr[kk] = (float)v2d_data[kk];
					}
				}
				tmpLnNum++;
			}

#endif
		}
		else if (tableno == TR_DEVICE_NO_TAB || tableno == TRWD_DEVICE_NO_TAB)
		{
			// ��ѹ��
#ifdef _WAMAP_
			for (jj = 0; jj < m_nTrNum; jj++)
			{
				if (tmpdev_id == m_pTrInf[jj].tr_id)
				{
					//					m_pTrCtl[jj].is_monitor = MENU_LFO_MON_YES;
					if (nColNum == 20)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pTrDat[jj].freq[kk] = (float)v2d_data[kk];
						}
					}
					else if (nColNum == 21)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pTrDat[jj].ampl[kk] = (float)v2d_data[kk];
						}
					}
					else if (nColNum == 22)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pTrDat[jj].sso[kk] = (int)v2d_data[kk];
						}
					}
					else if (nColNum == 12)
					{
						for (kk = 0; kk < (int)v2d_data.size(); kk++)
						{
							m_pTrDat[jj].pwr[kk] = (float)v2d_data[kk];
						}
					}

					break;
				}
			}
#else
			// �ж��Ƿ������豸
			nflag = -1;
			for (jj = 0; jj < tmpTrNum; jj++)
			{
				if (tmpdev_id == m_pTrInf[jj].tr_id)
				{
					nflag = jj;
					break;
				}
			}

			if (nflag != -1)
			{
				// �����豸����������
				if (nColNum == 12)
				{
					// �й�
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pTrDat[nflag].pwr[kk] = (float)v2d_data[kk];
					}
				}
			}
			else
			{
				m_pTrInf[tmpTrNum].tr_id = tmpdev_id;
				m_pTrDat[tmpTrNum].tr_id = tmpdev_id;
				m_pTrCtl[tmpTrNum].tr_id = tmpdev_id;
				m_pTrMon[tmpTrNum].tr_id = tmpdev_id;
				m_pTrInf[tmpLnNum].vbase = tmpdev_voltbase;
				m_pTrMon[tmpLnNum].vbase = tmpdev_voltbase;
				sprintf(m_pTrInf[tmpTrNum].tr_name, "%s", tmpdevname.c_str());
				//				m_pTrCtl[tmpTrNum].is_monitor = MENU_LFO_MON_YES;
				if (nColNum == 12)
				{
					// �й�
					for (kk = 0; kk < (int)v2d_data.size(); kk++)
					{
						m_pTrDat[tmpTrNum].pwr[kk] = (float)v2d_data[kk];
					}
				}
				tmpTrNum++;
			}
#endif
		}
		else
		{
			sprintf(m_LogString, "�豸���ʹ���[%d]", tableno);
			MessShow(m_LogString, LTERROR);
		}
		// 		sprintf(m_LogString,"end[%d]",ii);
		// 		MessShow(m_LogString);
	}
#ifndef _WAMAP_
	m_nFcNum = tmpFcNum;
	m_nGnNum = tmpGnNum;
	m_nLnNum = tmpLnNum;
	m_nTrNum = tmpTrNum;
#endif

	// ���ݹ۲촰��ʱ���ʱ��������ʱ������
	realNum = int(m_LfoPara.time_leng * df.m_DataHead.time_sample);
	for (ii = 0; ii < realNum; ii++)
	{
		m_Matchtime[ii] = ii / df.m_DataHead.time_sample;
	}
	t_delta = 1 / df.m_DataHead.time_sample;
	return 0;
}

int CLfoAnaObj::CallHDFile()
{
#ifndef _NUSP
	// �ж�ֵ������
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}

	if (m_nFcWarn == 0)
	{
		return 0;
	}
	Message msg;
	// 	size_t buf_size = 0;
	memset(msg.Msg_buf, 0, sizeof(msg.Msg_buf));
	msg.header.event = 1203;//��Ϣ����
	msg.header.serv = 175;//��Ϣͨ����
	FAD_CALLUP_COMTR_NAME_STRU tmp_CallHDFile;//�ٻ���¼���ļ���Ϣ�ṹ��	

	memset((void*)m_pFcHDFile, 0, para_nFcMaxMem * sizeof(SSO_HD_FILE_FAC));
	m_nFcHDFile = 0;
	for (int ii = 0; ii < m_nFcWarn; ii++)
	{
		if (ii > 10)//ÿ������ٻ�10����վ
		{
			break;
		}
		//����վIDת��Ϊͨ�ų�վID
		for (int kk = 0; kk < m_vCOMMFc.size(); kk++)
		{
			if (m_pFcWarn[ii].fac_id == m_vCOMMFc[kk].st_id)
			{
				m_pFcHDFile[m_nFcHDFile].fac_id = m_vCOMMFc[kk].comm_fac_id;
				// 				m_pFcHDFile[m_nFcHDFile].fac_id = 2392818777017287073;
				m_nFcHDFile++;
				break;
			}
		}
	}

	if (m_nFcHDFile <= 0)
	{
		sprintf(m_LogString, "��ȡͨ�ų�վIDʧ��");
		MessShow(m_LogString, LTINFO);
		return 0;
	}

	// 	m_nFcHDFile = 1;
	// 	if (m_nFcHDFile > 1)
	// 	{
	// 		m_pFcHDFile[0].fac_id = 2392818777017287073;
	// 	}

	for (int jj = 0; jj < m_nFcHDFile; jj++)
	{
		tmp_CallHDFile.fac_seq.push_back(m_pFcHDFile[jj].fac_id);
	}
	tmp_CallHDFile.fac_num = m_nFcHDFile;
	tmp_CallHDFile.start_time = m_SysMon.start_time;
	tmp_CallHDFile.end_time = m_SysMon.start_time;
	tmp_CallHDFile.src_proc_name = g_pAnaObj->m_ProcName;
	// 	sprintf(tmp_CallHDFile.src_proc_name,"%s",g_pAnaObj->m_ProcName);

	char hostname[32];
	gethostname(hostname, 32);
	tmp_CallHDFile.src_host_name = hostname;

	sprintf(m_LogString, "��¼���ļ��ٻ�����[%s],����[%s]", tmp_CallHDFile.src_host_name.c_str(), tmp_CallHDFile.src_proc_name.c_str());
	MessShow(m_LogString, LTINFO);


	sprintf(m_LogString, "��¼���ļ��ٻ�ʱ��[%ld-%ld][%s],�ٻ���վ����[%d]",
		m_SysMon.start_time, m_SysMon.end_time, PT_GetHMSTimeStringBySeconds(m_SysMon.start_time), tmp_CallHDFile.fac_num);
	MessShow(m_LogString, LTINFO);
	for (int kk = 0; kk < tmp_CallHDFile.fac_seq.size(); kk++)
	{
		sprintf(m_LogString, "��¼���ļ��ٻ���վ%d ID[%ld]", kk + 1, tmp_CallHDFile.fac_seq[kk]);
		MessShow(m_LogString, LTINFO);
	}


	char* msg_buf = NULL;
	int buf_size = 0;
	M_CODE(tmp_CallHDFile, msg_buf, buf_size);

	memcpy((void*)(msg.Msg_buf), msg_buf, sizeof(tmp_CallHDFile));

	sprintf(m_LogString, "��¼���ļ��ٻ���Ϣ��ֵ���");
	MessShow(m_LogString, LTINFO);

	if (PODB_messageSend((Message*)&msg, sizeof(tmp_CallHDFile), NULL) < 0)
	{
		sprintf(m_LogString, "��¼���ļ��ٻ���Ϣ����ʧ��");
		MessShow(m_LogString, LTINFO);
	}
	else
	{
		sprintf(m_LogString, "��¼���ļ��ٻ���Ϣ���ͳɹ�");
		MessShow(m_LogString, LTINFO);
		m_CallHDFile = 1;//�����ٻ���Ϣ�ɹ�,���Ƿ������ٻ���¼���ļ���־��Ϊ1
		if (m_SysMon.stat <= MENU_LFO_DEV_STS_SCT)
		{
			sprintf(m_LogString, "��ʱϵͳ��ȫ,m_SysMon.start_time����");
			MessShow(m_LogString, LTINFO);
			m_SysMon.start_time = 0;
			m_SysMon.end_time = 0;
		}
		else
		{
			sprintf(m_LogString, "��ʱϵͳ����ȫ,m_SysMon.start_time������");
			MessShow(m_LogString, LTINFO);
		}
	}
#endif // !_NUSP
	return 0;
}
//���ݵ�ǰʱ�䣬��Ҫ�ٻ���¼���ļ���ʱ����Ѿ��ٻ��ĳ�¼���ļ��Ƿ񷵻أ��жϱ����Ƿ���Խ����ٻ�
bool CLfoAnaObj::HDFileOk()
{
#ifndef _NUSP
	sprintf(m_LogString, "�жϱ����Ƿ���Խ��г�¼���ļ��ٻ�,m_CallHDFile=[%d]", m_CallHDFile);
	MessShow(m_LogString, LTINFO);
	if (m_CallHDFile == 0)//��ʱ��δ���г�¼���ļ��ٻ�
	{
		if (m_nFcWarn <= 0)//�񵴳�վ������û�г�վ
		{
			sprintf(m_LogString, "��ʱ��δ���г�¼���ļ��ٻ�,�񵴳�վ������û�г�վ,��������¼���ļ��ٻ�");
			MessShow(m_LogString, LTINFO);
			return false;
		}
		if ((m_CurGpsTime - m_SysMon.end_time) > m_LfoPara.delay_time)//�ж��ٻ�ʱ���Ƿ���Ч
		{
			sprintf(m_LogString, "��ʱ��δ���г�¼���ļ��ٻ�,�ٻ�ʱ����Ч,������¼���ļ��ٻ�");
			MessShow(m_LogString, LTINFO);
			return true;
		}
		else
		{
			sprintf(m_LogString, "��ʱ��δ���г�¼���ļ��ٻ�,�ٻ�ʱ����Ч,��������¼���ļ��ٻ�,m_CurGpsTime=%ld,end_time=%ld", m_CurGpsTime, m_SysMon.end_time);
			MessShow(m_LogString, LTINFO);
			return false;
		}
	}
	else//��ʱ�Ѿ��г�¼���ļ��ٻ�,�ж��Ƿ��Ƿ��Ѿ��ٻ����
	{
		if (HDFileBack())//�ٻ����ļ��Ѿ�����
		{
			sprintf(m_LogString, "�ϴ��ٻ��ĳ�¼���ļ��Ѿ�����");
			MessShow(m_LogString, LTINFO);
			StartSSOAna();
			if ((m_CurGpsTime - m_SysMon.end_time) > m_LfoPara.delay_time)//�ж��ٻ�ʱ���Ƿ���Ч
			{
				sprintf(m_LogString, "�ٻ�ʱ����Ч,������¼���ļ��ٻ�");
				MessShow(m_LogString, LTINFO);
				return true;
			}
			else
			{
				sprintf(m_LogString, "�ٻ�ʱ����Ч,��������¼���ļ��ٻ�");
				MessShow(m_LogString, LTINFO);
				return false;
			}
		}
		else
		{
			sprintf(m_LogString, "�ϴ��ٻ��ĳ�¼���ļ�δ����,��������¼���ļ��ٻ�");
			MessShow(m_LogString, LTINFO);
			return false;
		}
	}
#endif // !_NUSP
	return true;
}

bool CLfoAnaObj::HDFileBack()
{
#ifndef _NUSP
	Message* m_pMessage;
	Msg_source* m_pMsgSrc;
	m_pMessage = new Message;
	m_pMsgSrc = new Msg_source;
	FAD_TRANS_REPORT_STRUCT HDFileReport;
	int succed_fac_num;//�ٻ��ɹ��ĳ�վ����
	int fail_fac_num;
	long tmp_fac_id;
	PODB_messageReceive(m_pMessage, m_pMsgSrc, false);
	int recvedMsgType = m_pMessage->header.event;
	sprintf(m_LogString, "���յ���Ϣ����%d", recvedMsgType);
	MessShow(m_LogString, LTINFO);
	if (recvedMsgType != 1022)
	{
		delete m_pMsgSrc;
		delete m_pMessage;
		return false;
	}
	else
	{
		vector<string> vfilename;
		char* recvedMsgBuf = NULL;
		char filename[32];
		char hostname[20];
		char procname[20];
		int  procid;
		recvedMsgBuf = m_pMessage->Msg_buf;

		M_DECODE(HDFileReport, (char*)recvedMsgBuf, m_pMessage->header.len);
		//M_DECODE(HDFileReport, (char *)m_pMessage->Msg_buf,m_pMessage->header.len);
		delete m_pMessage;
		delete m_pMsgSrc;

		sprintf(m_LogString, "�ٻ���¼���ļ�������Ϣ,�ļ���:%s,��վID=%ld �ٻ�״̬%d", HDFileReport.filename.c_str(), HDFileReport.fac_id, HDFileReport.type);
		MessShow(m_LogString, LTINFO);
		PS_StringDivde(HDFileReport.filename.c_str(), ".", vfilename);
		if (vfilename.size() > 2)
		{
			sprintf(filename, "%s.%s", vfilename[0].c_str(), vfilename[1].c_str());
		}
		if (vfilename.size() > 3)
		{
			sprintf(hostname, "%s", vfilename[2].c_str());
		}
		if (vfilename.size() > 4)
		{
			sprintf(procname, "%s", vfilename[3].c_str());
		}
		if (vfilename.size() > 5)
		{
			procid = atoi(vfilename[4].c_str());
		}
		// 		if (strcmp(procname,"wamssomon") !=0)
		// 		{
		// 			return false;
		// 		}
		succed_fac_num = 0;
		fail_fac_num = 0;
		tmp_fac_id = HDFileReport.fac_id;
		for (int ii = 0; ii < m_nFcHDFile; ii++)
		{
			sprintf(m_LogString, "m_pFcHDFile[ii].fac_id=[%ld],HDFileReport.fac_id=[%ld],tmp_fac_id=[%ld]", m_pFcHDFile[ii].fac_id, HDFileReport.fac_id, tmp_fac_id);
			MessShow(m_LogString, LTINFO);
			if (m_pFcHDFile[ii].fac_id == HDFileReport.fac_id)
			{
				m_pFcHDFile[ii].stat = HDFileReport.type;
				sprintf(m_pFcHDFile[ii].file_name, "%s", filename);
				sprintf(m_LogString, "��վ[%ld]�ٻ���¼���ļ�״̬[%d]", m_pFcHDFile[ii].fac_id, m_pFcHDFile[ii].stat);
				MessShow(m_LogString, LTINFO);
			}
			else
			{
				MessShow("222222");
			}
			if (m_pFcHDFile[ii].stat == SUCCED)
			{
				succed_fac_num++;
			}
			if (m_pFcHDFile[ii].stat == FAILED)
			{
				sprintf(m_LogString, "��վ[%ld]�ٻ���¼���ļ�ʧ��,�ļ���[%s]", m_pFcHDFile[ii].fac_id, m_pFcHDFile[ii].file_name);
				MessShow(m_LogString, LTINFO);
				fail_fac_num++;
			}
		}
		sprintf(m_LogString, "�ٻ���¼���ļ���վ����[%d],�ɹ�[%d],ʧ��[%d]", m_nFcHDFile, succed_fac_num, fail_fac_num);
		MessShow(m_LogString, LTINFO);
		if ((succed_fac_num + fail_fac_num) == m_nFcHDFile)
		{
			sprintf(m_LogString, "���γ�¼���ļ��ٻ��ɹ�");
			MessShow(m_LogString, LTINFO);
			m_CallHDFile = 0;//��¼���ļ��ٻ��ɹ������Ƿ������ٻ���¼���ļ���־��Ϊ��
			return true;
		}
		else
		{
			return false;
		}
	}
#endif // !_NUSP
	return true;
}

void CLfoAnaObj::StartSSOAna()
{
	SSO_MSG_FILE_STRUCT tmpCase;
	HD_File_Struct	tmpFile;

	sprintf(tmpCase.version, "%s", "v1.0.0");
	tmpCase.start_time = m_SysMon.sso_bgn;
	sprintf(tmpCase.dir, "%s", m_RunInfDir);


	fstream fs;
	char filename[MAXBUF];
	char filenamebas[MAXBUF];
	char Examplefbas[MAXBUF];
	sprintf(filename, "sso_ana.msg");
#ifdef _WINDOWS32
	sprintf(filenamebas, "%s\\res\\%s", m_RunInfDir, filename);
	sprintf(Examplefbas, "%s\\data", m_RunInfDir);
#else
	sprintf(filenamebas, "%s/../ana/run/%s", m_RunInfDir, filename);
	sprintf(Examplefbas, "%s/data", m_RunInfDir);
#endif
	fs.open(filenamebas, ios::out);
	if (fs.fail())
	{
		sprintf(m_LogString, "������־�ļ�����ʧ��[%s]", filenamebas);
		MessShow(m_LogString, LTERROR);
		return;
	}

	fs << "Version = " << tmpCase.version << ";" << endl;
	fs << "Time = " << tmpCase.start_time << ";" << endl;
	fs << "Directory = " << m_HDFileDir << ";" << endl;
	fs << "Facnum  = " << tmpCase.file_num << ";" << endl;
	for (int jj = 0; jj < m_nFcHDFile; jj++)
	{
		memset(&tmpFile, 0, sizeof(HD_File_Struct));
		tmpFile.fac_id = m_pFcHDFile[jj].fac_id;
		sprintf(tmpFile.cfg_name, "%s.cfg", m_pFcHDFile[jj].file_name);
		sprintf(tmpFile.dat_name, "%s.dat", m_pFcHDFile[jj].file_name);
		tmpCase.vfile.push_back(tmpFile);
	}

	for (int ii = 0; ii < tmpCase.vfile.size(); ii++)
	{
		fs << "tmpCase.vfile[ii].fac_id,tmpCase.vfile[ii].cfg_name,tmpCase.vfile[ii].dat_name" << endl;
	}

	fs.close();
	return;
}

//����������豸ID,�ж��豸�ڱ������¼����Ƿ��Ѿ���ͳ�ƹ�
//��δͳ�ƹ�,����false
//��ͳ�ƹ�,��ii��ֵΪͳ�Ƶ��±�
bool CLfoAnaObj::IsWarnDev(INT_REC dev_id, int& ii)
{
	if (m_nDevEveWarn == 0)
	{
		return false;
	}
	for (ii = 0; ii < m_nDevEveWarn; ii++)
	{
		if (dev_id == m_pDevEveWarn[ii].dev_id)
		{
			return true;
		}
	}
	return false;
}

bool CLfoAnaObj::IsWarnFac(INT_REC fac_id, int& ii)
{
	if (m_nEveFcWarn == 0)
	{
		return false;
	}
	for (ii = 0; ii < m_nEveFcWarn; ii++)
	{
		if (fac_id == m_pEveFcWarn[ii].fac_id)
		{
			return true;
		}
	}
	return false;
}

void CLfoAnaObj::PushEveFac()
{
	int kk;
	for (int ii = 0; ii < m_nDevEveWarn; ii++)
	{
		if (IsWarnFac(m_pDevEveWarn[ii].fac_id, kk) == false)
		{
			m_pEveFcWarn[m_nEveFcWarn].fac_id = m_pDevEveWarn[ii].fac_id;
			m_pEveFcWarn[m_nEveFcWarn].sso_bgn = m_pDevEveWarn[ii].sso_bgn;
			m_pEveFcWarn[m_nEveFcWarn].sso_end = m_pDevEveWarn[ii].sso_end;
			sprintf(m_pEveFcWarn[m_nEveFcWarn].fac_name, "%s", m_pDevEveWarn[ii].fac_name);
			m_nEveFcWarn++;
		}
		else
		{
			if (m_pEveFcWarn[kk].sso_bgn > m_pDevEveWarn[ii].sso_bgn)
			{
				m_pEveFcWarn[kk].sso_bgn = m_pDevEveWarn[ii].sso_bgn;
			}
			if (m_pEveFcWarn[kk].sso_end < m_pDevEveWarn[ii].sso_end)
			{
				m_pEveFcWarn[kk].sso_end = m_pDevEveWarn[ii].sso_end;
			}
		}
	}
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!2008-01-18 �������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
// prony����
int PronyAnalysisProcess(CASE_TYPE* case_pointer, CPronyAnalysis* p_theProny)
{
	int     i = 0;
	char    LogString[MAXBUF];                         //  ��־�ļ��ַ���
	struct  timeb   calStartTime;
	struct  timeb   calEndTime;
	PRONY_CALC_PARA PronyPara;
	double mode[100][7], modedata[100][300];
	double y_value[NBSAVX], t_value[NBSAVX];
	memset(mode, 0, 100 * 7 * sizeof(double));
	memset(modedata, 0, 100 * 300 * sizeof(double));
	memset(y_value, 0, NBSAVX * sizeof(double));
	memset(t_value, 0, NBSAVX * sizeof(double));

	PronyPara.if_set_rank = 0;//0;   // 0 �Զ�ȷ�������� 1 ��Ϊ�趨����
	PronyPara.rank = 30;
	PronyPara.queue_type = -2;      // 0 ��ֵ����; -1 Ԥ���ֵ����;-2 Ԥ����������
	PronyPara.delDC = 1;       // �Ƿ�ȥ��ֱ��(0 ��; 1 ��)
	PronyPara.freq_min = 0.02f;   //2008-03-11
	PronyPara.freq_max = 100.0f;  // 3.0f;
	PronyPara.minAbsAmpl = 0.001f;  // 0.00001f;
	PronyPara.stor_lim_ampl = 0;       // 0.08f;
	PronyPara.stor_lim_damp = 100;
	PronyPara.snr_min = 20;      // 30;40;

	//m_calPara
	//////////////////////////////////////////////////////////////////////////
	PronyPara.freq_min = g_pAnaObj->m_LfoPara.freq_min;//0.2f;
	PronyPara.freq_max = g_pAnaObj->m_LfoPara.freq_max;//2.5f;
// 	PronyPara.stor_lim_damp = g_pAnaObj->m_LfoPara.stor_damp;//30; 

	int    mode_num = 0;
	double snr = 0;
	int    rank = 0;
	int    point_num = case_pointer->m_curve.m_nPoints;
	for (i = 0; i < point_num; i++)
	{
		y_value[i] = case_pointer->m_curve.y_value[i];
		t_value[i] = case_pointer->m_curve.t_value[i];
	}

	if (g_pAnaObj->para_DebugProny >= 3)
	{
		FILE* fp = fopen("mode_temp.txt", "a+");
		for (i = 0; i < point_num; i++)
		{
			fprintf(fp, "\t\t%6.2f\t\t%6.2f\n", t_value[i], y_value[i]);
		}
		fclose(fp);
		//             sprintf(LogString, "     %6.2f, %6.2f", 
		//                 //i,
		//                 t_value[i], 
		//                 y_value[i]);
		//             g_pAnaObj->WriteLogFile( LogString );
	}
	ftime(&calStartTime);

	//g_pProny->PronyAnalysis( PronyPara, point_num, y_value, t_value, mode_num, mode, snr, rank );
	p_theProny->PronyAnalysis7(PronyPara, point_num, y_value, t_value, mode_num, mode, snr, rank);

	if (g_pAnaObj->para_DebugProny >= 3)
	{
		sprintf(LogString, " mode_num = %d  rank= %d, snr= %f",
			mode_num,
			rank,
			snr);
		g_pAnaObj->MessShow(LogString);
	}

	case_pointer->m_result.mode_num = mode_num;

	if (mode_num > 0)
	{
		for (i = 0; i < mode_num; i++)
		{
			if (g_pAnaObj->para_DebugProny >= 2)
			{
				if (i == 0)
				{
					//printf("    ��ֵ      Ƶ��       �����     ��λ\n");
					sprintf(LogString, "    ��ֵ      Ƶ��       �����     ��λ\n");
					g_pAnaObj->MessShow(LogString);
				}
				//printf("  %6.2f   %6.2f    %6.2f    %6.2f\n",mode[i][2],mode[i][1],mode[i][0],mode[i][3]);
				sprintf(LogString, "  %6.2f   %6.2f    %6.2f    %6.2f\n", mode[i][2], mode[i][1], mode[i][0], mode[i][3]);
				g_pAnaObj->MessShow(LogString);
			}
			for (int j = 0; j < 7; j++)                            // lsf  2005.01.16
				case_pointer->m_result.mode[i][j] = (float)mode[i][j];
		}
	}
	else if (mode_num == 0)
	{
		if (g_pAnaObj->para_DebugProny >= 1)
		{
			sprintf(LogString, "    û�з���Ҫ�����ģʽ");
			g_pAnaObj->MessShow(LogString);
		}
	}
	else if (mode_num < 0)
	{
		if (g_pAnaObj->para_DebugProny >= 1)
		{
			sprintf(LogString, "    �޿��н�");
			g_pAnaObj->MessShow(LogString);
		}
	}

	ftime(&calEndTime);
	int second = g_pAnaObj->PT_CalculateTimeInterval(calStartTime, calEndTime);

	//if(mode_num>=0)
	//{
	if (g_pAnaObj->para_DebugProny >= 1)
	{
		sprintf(LogString, "case_id="INT_REC_FMT", ��ʱ=%d����", INT_REC_VAL(case_pointer->m_nID), second);
		g_pAnaObj->MessShow(LogString);
	}
	//}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
//      ���ܣ�  ���㴦���̣߳��̶�3��5���̣߳�
//      ������  VOID
//      ���أ�  VOID
////////////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS32
unsigned long WINAPI CalcThreadMain(void* param)
#else
extern "C" {
	void CalcThreadMain(void* param)
#endif
	{
		struct  timeb   t_StartTime;
		struct  timeb   t_EndTime;
		int             t_total;
		int             counter = 0;
		char            LogString[MAXBUF];                         //  ��־�ļ��ַ���
		int             CalcThread_Max_Time = MAX_TOTAL_TIME * 1000;

		CALC_THRD_CASE* pPara = (CALC_THRD_CASE*)param;
		int CalcThreadMain_id = pPara->thrd_id;

#ifndef _WINDOWS32
		//�����߳��ͷ���Դ��־
		int iret = pthread_detach(pthread_self());
		if (iret != 0)
		{
			pthread_exit(NULL);
		}
#endif

		CPronyAnalysis* th_pProny;
		th_pProny = new CPronyAnalysis;

		while (1)
		{
			if (!th_pProny)
			{
				printf(" create CPronyAnalysis pointer fail, Thread will exit!\n");
				break;
			}
			//TF_DelayMinSec(300);
			//TF_DelayMinSec(300);
#ifdef _WINDOWS32
			Sleep(300);
#else
			usleep(300 * 1000);
			//printf(">>> pronycalc_thread call usleep() \n");
#endif


			counter += 1;
			if (counter >= 20)
			{
				sprintf(LogString, "CalcThreadMain[%d] is live!", CalcThreadMain_id);
				g_pAnaObj->MessShow(LogString);
				counter = 0;
			}

			if (g_ProcessExitFlag)     //  �������˳���־
			{
				sprintf(LogString, "CalcThreadMain[%d] g_ProcessExitFlag is true, Thread will exit!", CalcThreadMain_id);
				g_pAnaObj->MessShow(LogString);
				break;
			}

			if (pPara->isStartUp != 1)
			{
				continue;
			}
			else
			{
				if (pPara->isFinish == 1) continue;

				// #ifdef _WINDOWS32
				// 				THRD_LockThreadMutex( pPara->ThrdCaseMutex ); 
				// #else
				// 				int oldtype;
				// 				THRD_LockThreadMutex( pPara->ThrdCaseMutex, oldtype ); 
				// #endif

				if (pPara->nCases < 1)
				{
					sprintf(LogString, "CalcThreadMain[%d] pPara->nCases = %d, error!\n", CalcThreadMain_id, pPara->nCases);
					g_pAnaObj->MessShow(LogString);
				}
				else       // calculation
				{
					sprintf(LogString, "CalcThreadMain[%d] ������һ�ּ���! case��[%d]\n", CalcThreadMain_id, pPara->nCases);
					g_pAnaObj->MessShow(LogString);

					if (pPara->nCases > THRD_CASE_NUM)
					{
						sprintf(LogString, "CalcThreadMain[%d] pPara->nCases[%d] > THRD_CASE_NUM[%d], reset pPara->nCases[%d]!\n", CalcThreadMain_id, pPara->nCases, THRD_CASE_NUM, pPara->nCases);
						g_pAnaObj->MessShow(LogString);

						pPara->nCases = THRD_CASE_NUM;
					}

					t_StartTime = g_pAnaObj->PT_GetNowMilliSeconds();

					for (int ii = 0; ii < pPara->nCases; ii++)
					{
						//�ܼ���ʱ�䳬ʱ�ж�
						t_EndTime = g_pAnaObj->PT_GetNowMilliSeconds();
						t_total = g_pAnaObj->PT_CalculateTimeInterval(t_StartTime, t_EndTime);
						if (t_total > CalcThread_Max_Time) // 60s
						{
							sprintf(LogString, "CalcThreadMain[%d] ����ʱ�䳬ʱ[%ld] - [%ld], use time %ds!", CalcThreadMain_id, t_StartTime.time, t_EndTime.time, t_total / 1000);
							g_pAnaObj->MessShow(LogString);
							break;
						}

						sprintf(LogString, "CalcThreadMain[%d] call PronyAnalysis case_id["INT_REC_FMT"]", CalcThreadMain_id, INT_REC_VAL(pPara->pcases[ii].m_nID));
						g_pAnaObj->MessShow(LogString);

						PronyAnalysisProcess((CASE_TYPE*)&(pPara->pcases[ii]), th_pProny);

						if (g_ProcessExitFlag)     //  �������˳���־
						{
							sprintf(LogString, "CalcThreadMain[%d] g_ProcessExitFlag is true, Thread will exit!\n", CalcThreadMain_id);
							g_pAnaObj->MessShow(LogString);
							break;
						}
					}
					//////////////////////////////////////////////////////////////////////////

#ifdef _WINDOWS32
					THRD_LockThreadMutex(pPara->ThrdCaseMutex);
#else
					int oldtype = 0;
					THRD_LockThreadMutex(pPara->ThrdCaseMutex, oldtype);
#endif

					pPara->isFinish = 1;

#ifdef _WINDOWS32
					THRD_UnLockThreadMutex(pPara->ThrdCaseMutex);
#else
					THRD_UnLockThreadMutex(pPara->ThrdCaseMutex, oldtype);
#endif
					//////////////////////////////////////////////////////////////////////////
					t_EndTime = g_pAnaObj->PT_GetNowMilliSeconds();
					t_total = g_pAnaObj->PT_CalculateTimeInterval(t_StartTime, t_EndTime);

					int   nM, nS, nMs;
					char  temp_time_str[16];
					nM = t_total / 60000;
					nS = (t_total - nM * 60000) / 1000;
					nMs = t_total - nM * 60000 - nS * 1000;
					if (nM > 0)
					{
						sprintf(temp_time_str, "%d��%d��%d����", nM, nS, nMs);
					}
					else
					{
						if (nS > 0)
						{
							sprintf(temp_time_str, "%d��%d����", nS, nMs);
						}
						else
						{
							sprintf(temp_time_str, "%d����", nMs);
						}
					}
					sprintf(LogString, "CalcThreadMain[%d] pPara->nCases = %d use_time = %s\n", CalcThreadMain_id, pPara->nCases, temp_time_str);
					g_pAnaObj->MessShow(LogString);
				}
			}
		}
		if (th_pProny) delete th_pProny;

#ifdef _WINDOWS32
		return 0;
#else
		pthread_exit(NULL);
		return;
	}
#endif
}

// ������ҵ����()
int ClacCtlMainProcess(int nCase, CASE_TYPE* pCase)
{
	if (nCase <= 0 || !pCase) return 0;

	int  ii, jj, nn, mm, ll;
	struct  timeb   t_StartTime;
	struct  timeb   t_EndTime;
	int             t_total;
	struct  timeb   t_NowTime;
	char            LogString[1024];                         //  ��־�ļ��ַ���

	int  nCalcCase = 0;

	t_StartTime = g_pAnaObj->PT_GetNowMilliSeconds();

	for (ii = 0; ii < nCase; ii++)
	{
		pCase[ii].m_nStatus = 0;
	}

	if (g_nCalcThrd < 1)
	{
		sprintf(LogString, "�޼����̣߳����������������̼���!");
		g_pAnaObj->MessShow(LogString);
		for (ii = 0; ii < nCase; ii++)
		{
			//�ܼ���ʱ�䳬ʱ�ж�
			t_EndTime = g_pAnaObj->PT_GetNowMilliSeconds();
			t_total = g_pAnaObj->PT_CalculateTimeInterval(t_StartTime, t_EndTime);
			if (t_total > MAX_TOTAL_TIME * 1000) // 60s // ���Ӻ궨��
			{
				sprintf(LogString, "�ܼ���ʱ�䳬ʱs[%ld] - e[%ld], use time %ds!", t_StartTime.time, t_EndTime.time, t_total / 1000);
				g_pAnaObj->MessShow(LogString);

				break;
			}

			PronyAnalysisProcess((CASE_TYPE*)&pCase[ii], g_pProny);
		}
	}
	else // �����߳�
	{
		while (1)
		{
			//TF_DelayMinSec(50);
			//usleep(200);
#ifdef _WINDOWS32
			Sleep(1000);
#else
			sleep(1);
#endif
			sprintf(LogString, "�����̣���������̼߳���״̬!");
			g_pAnaObj->MessShow(LogString);
			for (ii = 0; ii < g_nCalcThrd; ii++)
			{
				if (g_pCalcThrdCase[ii].isStartUp == 0)
				{
					if (g_pCalcThrdCase[ii].isFinish == 0)
					{
#ifdef _WINDOWS32
						THRD_LockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex);
#else
						int oldtype = 0;
						THRD_LockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex, oldtype);
#endif
						//////////////////////////////////////////////////////////////////////////
						// ������ݣ������̼߳���
						t_NowTime = g_pAnaObj->PT_GetNowMilliSeconds();
						g_pCalcThrdCase[ii].nCases = 0;
						for (nn = 0; nn < nCase; nn++)  // ��ֹλ���޸�
						{
							if (g_pCalcThrdCase[ii].nCases < THRD_CASE_NUM)
							{
								if (pCase[nn].m_nStatus == 0)
								{
									pCase[nn].m_nStatus = 1; // ���ڴ���
									pCase[nn].timeStart = t_NowTime;

									g_pCalcThrdCase[ii].pcases[g_pCalcThrdCase[ii].nCases].m_nID = pCase[nn].m_nID;
									//g_pCalcThrdCase[ii].pcases[g_pCalcThrdCase[ii].nCases].m_nStatus = pCase[nn].m_nStatus;

									g_pCalcThrdCase[ii].pcases[g_pCalcThrdCase[ii].nCases].m_curve.m_nPoints = pCase[nn].m_curve.m_nPoints;
									for (mm = 0; mm < pCase[nn].m_curve.m_nPoints; mm++)
									{
										g_pCalcThrdCase[ii].pcases[g_pCalcThrdCase[ii].nCases].m_curve.t_value[mm] = pCase[nn].m_curve.t_value[mm];
										g_pCalcThrdCase[ii].pcases[g_pCalcThrdCase[ii].nCases].m_curve.y_value[mm] = pCase[nn].m_curve.y_value[mm];
									}

									g_pCalcThrdCase[ii].nCases += 1;
								}
							}
							else
								break;
						}
						if (g_pCalcThrdCase[ii].nCases > 0) g_pCalcThrdCase[ii].isStartUp = 1;
						//////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS32
						THRD_UnLockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex);
#else
						THRD_UnLockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex, oldtype);
#endif
					}
					else //  g_pCalcThrdCase[ii].isFinish == 1
					{
#ifdef _WINDOWS32
						THRD_LockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex);
#else
						int oldtype = 0;
						THRD_LockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex, oldtype);
#endif

						g_pCalcThrdCase[ii].isFinish = 0;

#ifdef _WINDOWS32
						THRD_UnLockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex);
#else
						THRD_UnLockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex, oldtype);
#endif
					}
				}
				else //  g_pCalcThrdCase[ii].isStartUp == 1
				{
					if (g_pCalcThrdCase[ii].isFinish == 0)
					{
						// �߳����ڼ��㣬�жϳ�ʱ
					}
					else //  g_pCalcThrdCase[ii].isFinish == 1
					{
#ifdef _WINDOWS32
						THRD_LockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex);
#else
						int oldtype = 0;
						THRD_LockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex, oldtype);
#endif
						//////////////////////////////////////////////////////////////////////////
						g_pCalcThrdCase[ii].isStartUp = 0;
						g_pCalcThrdCase[ii].isFinish = 0;
						//////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS32
						THRD_UnLockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex);
#else
						THRD_UnLockThreadMutex(g_pCalcThrdCase[ii].ThrdCaseMutex, oldtype);
#endif
						//////////////////////////////////////////////////////////////////////////
						// �̼߳�����������ս��
						t_NowTime = g_pAnaObj->PT_GetNowMilliSeconds();
						// ���
						for (jj = 0; jj < g_pCalcThrdCase[ii].nCases; jj++)
						{
							for (nn = 0; nn < nCase; nn++)
							{
								if (g_pCalcThrdCase[ii].pcases[jj].m_nID == pCase[nn].m_nID)
								{
									pCase[nn].m_nStatus = 2; // �������
									pCase[nn].timeEnd = t_NowTime;
									pCase[nn].m_time = g_pAnaObj->PT_CalculateTimeInterval(pCase[nn].timeStart, pCase[nn].timeEnd);

									pCase[nn].m_result.mode_num = g_pCalcThrdCase[ii].pcases[jj].m_result.mode_num;

									if (g_pCalcThrdCase[ii].pcases[jj].m_result.mode_num > 0)
									{
										for (mm = 0; mm < g_pCalcThrdCase[ii].pcases[jj].m_result.mode_num; mm++)
										{
											for (ll = 0; ll < 7; ll++)
												pCase[nn].m_result.mode[mm][ll] = g_pCalcThrdCase[ii].pcases[jj].m_result.mode[mm][ll];
										}
									}
									break;
								}
							}
						}
						// 						#ifdef _WINDOWS32
						// 							THRD_LockThreadMutex( g_pCalcThrdCase[ii].ThrdCaseMutex ); 
						// 						#else
						// 							int oldtype;
						// 							THRD_LockThreadMutex( g_pCalcThrdCase[ii].ThrdCaseMutex, oldtype ); 
						// 						#endif
						// 						//////////////////////////////////////////////////////////////////////////
						// 						 g_pCalcThrdCase[ii].isFinish = 0;
						// 						//////////////////////////////////////////////////////////////////////////
						// 						#ifdef _WINDOWS32
						// 							THRD_UnLockThreadMutex( g_pCalcThrdCase[ii].ThrdCaseMutex ); 
						// 						#else
						// 							THRD_UnLockThreadMutex( g_pCalcThrdCa

					}
				}
			}
			// ������������Ƿ������
			nCalcCase = 0;
			t_NowTime = g_pAnaObj->PT_GetNowMilliSeconds();
			for (nn = 0; nn < nCase; nn++)
			{
				if (pCase[nn].m_nStatus == 0)
				{
					nCalcCase += 1;
				}
				else if (pCase[nn].m_nStatus == 1)
				{
					t_total = g_pAnaObj->PT_CalculateTimeInterval(pCase[nn].timeStart, t_NowTime);
					if (t_total > MAX_TOTAL_TIME * 1000) // 60s
					{
						pCase[nn].m_nStatus = 0; // ���¼���
					}
					nCalcCase += 1;
				}
			}
			if (nCalcCase == 0) break;
		}
	}
	t_EndTime = g_pAnaObj->PT_GetNowMilliSeconds();
	t_total = g_pAnaObj->PT_CalculateTimeInterval(t_StartTime, t_EndTime);

	int   nM, nS, nMs;
	char  temp_time_str[16];
	nM = t_total / 60000;
	nS = (t_total - nM * 60000) / 1000;
	nMs = t_total - nM * 60000 - nS * 1000;
	if (nM > 0)
	{
		sprintf(temp_time_str, "%d��%d��%d����", nM, nS, nMs);
	}
	else
	{
		if (nS > 0)
		{
			sprintf(temp_time_str, "%d��%d����", nS, nMs);
		}
		else
		{
			sprintf(temp_time_str, "%d����", nMs);
		}
	}
	sprintf(LogString, "ClacCtlMainProcess(): ��������[%d]�����������ʱ%s!", nCase, temp_time_str);
	g_pAnaObj->MessShow(LogString);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  �������� : ��־���ʻ����������ͽ�������
#ifdef _WINDOWS32
	//  �������� : ��ʼ��������
void THRD_InitThreadMutex(HANDLE& mutex, char* mutexName)
{
	mutex = CreateMutex(NULL, FALSE, mutexName);
}
// 	void THRD_InitThreadMutex( HANDLE  &mutex )
// 	{
// 		mutex = CreateMutex(NULL,FALSE,NULL);
// 	}
	//  �������� : �ͷŻ�����
void THRD_ReleaseThreadMutex(HANDLE  mutex)
{
	CloseHandle(mutex);
	mutex = NULL;
}

//  �������� : ������������
void THRD_LockThreadMutex(HANDLE  mutex)
{
	WaitForSingleObject(mutex, INFINITE);
}
//  �������� : �������������
void THRD_UnLockThreadMutex(HANDLE  mutex)
{
	ReleaseMutex(mutex);
}
#else
	//  �������� : AIX�������·�װ�������������
void mypthread_mutex_unlock(void* mymutex)
{
	pthread_mutex_unlock((pthread_mutex_t*)&mymutex);
	return;
}
//  �������� : �������������
void THRD_UnLockThreadMutex(pthread_mutex_t  mutex, int oldtype)
{
	//	pthread_mutex_unlock((pthread_mutex_t * )&mutex);
		//pthread_cleanup_pop(0);  //2008-01-18 sun Unix 
		//pthread_setcanceltype(oldtype, NULL);
		//----// //2008-01-19
	pthread_mutex_unlock((pthread_mutex_t*)&mutex);
	//----// //2008-01-19
	return;
}

//  �������� : ��ʼ��������
void THRD_InitThreadMutex(pthread_mutex_t  mutex, char* mutexName)
{
	//----// //2008-01-19
	int status = 0;
	status = pthread_mutex_init(&mutex, NULL);
	if (status != 0)
	{
		printf("In THRD_InitThreadMutex(), pthread_mutex_init error.\n");
	}
	assert(status == 0);

	//----//
	return;
}
//  �������� : �ͷŻ�����
void THRD_ReleaseThreadMutex(pthread_mutex_t  mutex)
{
	//----// //2008-01-19
	int status = 0;
	status = pthread_mutex_destroy(&mutex);
	if (status != 0)
	{
		printf("In THRD_ReleaseThreadMutex(), pthread_mutex_destroy error...\n");
	}
	//----// //2008-01-19
	return;
}

//  �������� : ������������
void THRD_LockThreadMutex(pthread_mutex_t  mutex, int oldtype)
{
	//	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);
	//	pthread_cleanup_push(mypthread_mutex_unlock, &mutex);
	//	
	//	pthread_mutex_lock((pthread_mutex_t * )&mutex);
		//----// //2008-01-19
	pthread_mutex_lock((pthread_mutex_t*)&mutex);
	//----// //2008-01-19
	return;
}
#endif

//////////////////////////////////////////////////////////////////////////
// �ٻ�100֡�ļ��߳�
#ifdef _WINDOWS32
unsigned __stdcall CallDynFileTMain(void* para)
#else
void* CallDynFileTMain(void*)
#endif
{
#ifdef _WINDOWS32
	THRD_LockThreadMutex(g_pAnaObj->m_DynFilemutex);
#else
	THRD_LockThreadMutex(g_pAnaObj->m_DynFilemutex, 0);
#endif

	g_pAnaObj->CallDynFile();

#ifdef _WINDOWS32
	_endthreadex(0);
#else
	pthread_exit(0);
#endif

#ifdef _WINDOWS32
	THRD_UnLockThreadMutex(g_pAnaObj->m_DynFilemutex);
#else
	THRD_UnLockThreadMutex(g_pAnaObj->m_DynFilemutex, 0);
#endif

#ifdef _WINDOWS32
	return 0;
#else
	return NULL;
#endif
}
