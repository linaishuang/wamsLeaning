/******************************************************************************
  ModuleName :   WAMS LFO Analysis main function
  FileName   :   WamLfoAna.cpp
  Description:   Defines the entry point for the LFO Analysis application.
  HISTORY :    	
      Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   09/11/2007       1.0         lsf                    created
   2011/05/10      v3.1.0       zhw                    D5000��ֲ
   2011/05/26      v3.1.1       zhw                    ���ӳ�վID(_ADDFACID_)
   2011/06/23      v3.1.1.1     zhw                    ����WAMSӦ��ģ��澯��Ϣ��
   2011/08/23      v3.1.2       zhw                    ����D5000PMU���ݽӿ�
   ���Ӻ궨��_WAMS_APP_WARN����Ӧ�ñ�����
   ��ȡPMUʵʱ���ݵ���
   2012/01/31      v3.1.3       zhw                    �񵴽��������LFO���¼���
   2012/05/17      v3.1.3.1     zhw                    �������־�������,����pubfun,pubodbapi
   2012/07/11      v3.1.3.2     zhw                    ���������ݸ�ʽ
   2012/07/25      v3.1.4       zhw                    ���Ӳ���,���Ӱ�������ӿ�
   2012/07/27      v3.1.4.1     zhw                    �޸�����·��Ϊ����·��
   2012/08/16      v3.1.4.2     zhw                    �����Ƿ����ģ��������para_IsFltSim
   2012/08/21      v3.1.4.3     zhw                    �������ۺ����ܸ澯(�ƶ�)������Ϣ
   2012/08/28      v3.1.5       zhw                    �澯����
   2012/09/20      v3.1.5.1     zhw                    ��ȡ�ɸ�ʽ�ļ�ʱ��������
   2012/09/22      v3.1.5.2     zhw                    ������Ƶ�񵴰�����־�ļ��񵴱�־����
   2012/10/16      v3.1.6       zhw                    �澯�ַ�������ֵ��200����
   WAMSǰ��ң�ⶨ����WAMS_FESӦ�ö�ȡ
   ���ӳ�վ�����Ƿ���ʾ����
   ���Ӹ澯Ӧ�ò���
   ����·��ͨ��������������
   2012/11/30      v3.1.6.1     zhw                    ���¾ɸ�ʽ�ļ���ȡ��ʽ����ӽ�������
   ����ģʽ��Ӧʱ��
   UpdateHisCase�����в����LFOʵʱCASE���ڱ����ݣ��޸�Ϊ׷��
   2012/12/19      v3.1.7       zhw                    ȥ����Ϣ����
   2012/12/21      v3.1.7       zhw                    SAFE_DELETE����ΪSAFE_DELETE_ARRAY
   2013/01/09      v3.1.7       zhw                    �޸Ĳ����͸澯��ʽ
   2013/02/21      v3.1.8       zhw                    �޸Ŀƶ���Ϣ�ӿڷ�Ⱥ(group_num)Ĭ��ֵ
   2013/03/07      v3.1.8.1     zhw                    �ж�ǰ��������
   2013/04/22      v3.1.9       cyj                    PC493,����Խ������Զ���գ���������
   2013/04/24      v3.1.9_1     cyj                    GJ363,���ݵ�ѹ�ȼ����������ֵ
   2013/10/11      v3.1.10_1    cyj					   PC0619,�����ӿڶ����ȫ�ֱ���
   2014/01/28      v3.1.11_1    cyj					   ��������������Ƿ����ݼ��,�ſ�������й��쳣���
   2014/02/18      v3.1.12_1    cyj					   �ڴ�����δ�ͷ��޸�
   2014/05/07      v3.1.13_1    cyj					   pc_0755�񵴱�ѹ���������޸�
													   GJ_0631���ֻ���й�������
******************************************************************************/
#include "WamSsoMonObj.h"
#include "ThreadFunc.h"
#include <string.h> 
// #ifndef _WINDOWS32
// 	#include "../lib/ThreadFunc.h"                       //  CThreadFuncs::startThread()
// #else
// 	#include "..\lib\ThreadFunc.h"                       //  CThreadFuncs::startThread()
// #endif

// ȫ�ֱ�������
/******************************************************************************/
// API �ӿ�
// #ifndef USE_DB_NET_API
// 	ODB::CTableOp                 th_TableOp;            //  ʵʱ�Ȿ�ؽӿ�
//  	ODB::CTableOp                 g_IDBTableOp;          //  ʵʱ�Ȿ�ؽӿ�
// #else
// 	NET_ODB::CTableNet            th_TableOp;            //  ʵʱ������ӿ�
//  	NET_ODB::CTableNet            g_IDBTableOp;          //  ʵʱ������ӿ�
// #endif
///////////////////////////////////////////////////////////////////////////
CLfoAnaObj      *      g_pAnaObj  ;                         //  ������

int   g_appNo = AP_WAMS;
string  g_appName = AF_WAMS_NAME;
int   g_ctxNo = AC_REALTIME_NO;
string  g_ctxName = AC_REALTIME_NAME;
char  g_LogStr[MAXBUF];
char g_ProName[] = "wamssomon";
char g_ProVersion[] = "v1.0.1_1";

//�����̺߳���
bool                   g_ProcessExitFlag ;//= true;                   //  �����˳���־
int                    g_nCalcThrd       ;//= 1;                      //  �̸߳���
CALC_THRD_CASE *       g_pCalcThrdCase   ;//= NULL ;                  //  �߳�case
//////////////////////////////////////////////////////////////////////////
void g_PubLogMessShow(const char *log, const int logtype=LTINFO);
/******************************************************************************/

extern "C" void LfoAna_ExitInstance(int iSig)
{
#ifdef  _WAMAP_
#ifdef _PODB_MSGBUS_
#ifdef _D5000
	g_pAnaObj->PODB_messageExit(-1);
#endif
#endif
#endif
	g_ProcessExitFlag   =  true;     //  �����˳���־
	g_pAnaObj->PT_DelaySecond(1);
	
 	g_pAnaObj->ReleaseMem();

	SAFE_DELETE(g_pAnaObj);
	//////////////////////////////////////////////////////////////////////////
	for (int kk = 0; kk < g_nCalcThrd; kk++  )
	{
		THRD_ReleaseThreadMutex( g_pCalcThrdCase[kk].ThrdCaseMutex );
	}
	if (g_pCalcThrdCase)  delete g_pCalcThrdCase;
	//////////////////////////////////////////////////////////////////////////

    //exit(0);
}

bool LfoAna_InitInstance(int argc, char * argv[])
{
	g_pAnaObj = new CLfoAnaObj;

	if (argc >=2)
	{
		sprintf(g_pAnaObj->m_ProcName,"%s",argv[0]);
		sprintf(g_pAnaObj->m_RunInfDir,"%s",argv[1]);
	}

	// ��ʼ����־
	string tmpString = g_pAnaObj->m_RunInfDir;
	tmpString = tmpString + g_pAnaObj->m_backslash + "ini" + g_pAnaObj->m_backslash +  "Default.ini";
	CIniFile iniFile(tmpString);
	int tmpDebugType = -1;
	if( iniFile.INI_ReadFile() == true )
	{
		tmpDebugType = iniFile.INI_GetValueI("���Ʋ���","DebugType",-1);
	}
	
	tmpString = g_pAnaObj->m_RunInfDir;
	tmpString = tmpString + g_pAnaObj->m_backslash + "log" + g_pAnaObj->m_backslash +  g_ProName + ".log";
	g_pAnaObj->PLF_InitLogFile(tmpString);
	if (tmpDebugType >= 0)
	{
		g_pAnaObj->PLF_setLogType(tmpDebugType);
	}
	sprintf(g_LogStr,"��־�ļ���[%s]",tmpString.c_str());
	g_PubLogMessShow(g_LogStr,LTDEBUG);
	printf("[%s]\n",g_pAnaObj->m_backslash);

	// ����ע��
#ifdef  _WAMAP_ 
#ifdef _D5000
	g_pAnaObj->PODB_ProcRegister(AC_REALTIME_NAME,AF_WAMS_NAME,g_ProName);
#ifndef _NUSP
#ifdef _PODB_MSGBUS_
	g_pAnaObj->PODB_messageInit(AC_REALTIME_NAME,AF_WAMS_NAME,g_ProName);
	g_pAnaObj->PODB_messageSubscribe(175,AC_REALTIME_NAME);
#endif // _PODB_MSGBUS_
#endif // !_NUSP
#else
	g_pAnaObj->PODB_ProcRegister(g_appNo, g_ProName, 0 , g_ctxNo);
#ifdef _PODB_MSGBUS_
	g_pAnaObj->PODB_InitMsgBus(g_ProName,g_ProName,1);
#endif // _PODB_MSGBUS_
#endif // _D5000
#endif // _WAMAP_
	return true;
}

///////////////////////////////////////////////////////////////////////////


int main(int argc, char * argv[])
{
    if (argc == 2)
    {
	    if (0 == strcmp(argv[1], "-v"))
	    {
    		    char tmpver[128];
		    //������Ա�ṩ���ơ��汾�����ܵ���Ϣ
		    cout<<"||��������:��ͬ�������߼���"<<endl;
		    cout<<"||Ӣ������:"<<g_ProName<<endl;
		    cout<<"||�汾  ��:"<<g_ProVersion<<endl;
		    cout<<"||����ʱ��:2017-04-11"<<endl;
		    cout<<"||��������:������ ��Ŀ¼"<<endl;    
		    cout<<"||����˵��:��ͬ�������߼���"<<endl; 
		    cout<<"||������Ա:Ӧ�����������"<<endl;                
		    cout<<"||���ά��ʱ��:2017-05-12"<<endl; 
		    cout<<"||����޶�����:�¿���"<<endl; 
		    cout<<"||ά����Ա:Ӧ�����������"<<endl;
		    sprintf(tmpver,"||��    ע:%s Ver %s build at %s %s",g_ProName, g_ProVersion, __DATE__, __TIME__);
		    cout<<tmpver<<endl;
		    exit(0);
	    }
    }

    if ( LfoAna_InitInstance(argc,argv) == false ) 
    {
	    LfoAna_ExitInstance(0);
	    return 0;
    }

	sprintf(g_LogStr,"**********%s Ver %s build at %s %s**********", g_ProName, g_ProVersion, __DATE__, __TIME__);
	g_PubLogMessShow(g_LogStr,100);
	g_pAnaObj->ReadDefaultFile();
	g_pAnaObj->AllocMemforPointer();
	if ( g_pAnaObj->InitPrject() == false)
	{
		LfoAna_ExitInstance(0);
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	// ʹ��һ���߳� th_TableOp,�����ʵ������
	//int thrednum = Thred_NetTableGetRecNum(LFO_AP_ID, LFO_FAC_NO);
	//  �����߳�
	g_ProcessExitFlag = false;                 //  �����˳���־
	if (g_nCalcThrd < 0) g_nCalcThrd = 0;
	//g_nCalcThrd       = 6 ;
	
	g_pCalcThrdCase = (CALC_THRD_CASE *)calloc(g_nCalcThrd, sizeof(CALC_THRD_CASE));
	memset( (void *)g_pCalcThrdCase, 0, sizeof(CALC_THRD_CASE) * g_nCalcThrd);
	for (int kk = 0; kk < g_nCalcThrd; kk++)
	{
		g_pCalcThrdCase[kk].thrd_id = kk+1;
		sprintf( g_LogStr, "CalcMutex%d", g_pCalcThrdCase[kk].thrd_id);
		THRD_InitThreadMutex( g_pCalcThrdCase[kk].ThrdCaseMutex, g_LogStr);
		sprintf(g_LogStr, "���������߳�[%d]!", g_pCalcThrdCase[kk].thrd_id);
		g_PubLogMessShow(g_LogStr);
		CThreadFuncs::startThread(CalcThreadMain, (void *)&g_pCalcThrdCase[kk]);
		g_pAnaObj->PT_DelaySecond(1);
	}
	//////////////////////////////////////////////////////////////////////////
	//  ��������ˢ���߳�
// 	int ThreadFunc_RealtimeCurve = 0 ;
// 	CThreadFuncs::startThread( ThreadFunc_RealtimeCurveProcess, (void *)&ThreadFunc_RealtimeCurve);
	//////////////////////////////////////////////////////////////////////////

	g_pAnaObj->MainProcess();
	LfoAna_ExitInstance(0);
	return 0;
}

void g_PubLogMessShow(const char *log, const int logtype)
{
	g_pAnaObj->PLF_MessShow(log,logtype);
}

