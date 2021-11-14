/******************************************************************************
  ModuleName :   WAMS LFO Analysis main function
  FileName   :   WamLfoAna.cpp
  Description:   Defines the entry point for the LFO Analysis application.
  HISTORY :    	
      Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   09/11/2007       1.0         lsf                    created
   2011/05/10      v3.1.0       zhw                    D5000移植
   2011/05/26      v3.1.1       zhw                    增加厂站ID(_ADDFACID_)
   2011/06/23      v3.1.1.1     zhw                    更新WAMS应用模块告警信息表
   2011/08/23      v3.1.2       zhw                    更新D5000PMU数据接口
   增加宏定义_WAMS_APP_WARN屏蔽应用变量表
   获取PMU实时数据调试
   2012/01/31      v3.1.3       zhw                    振荡结束后更新LFO振荡事件表
   2012/05/17      v3.1.3.1     zhw                    修正部分警告内容,更新pubfun,pubodbapi
   2012/07/11      v3.1.3.2     zhw                    增加新数据格式
   2012/07/25      v3.1.4       zhw                    增加采样,增加案例保存接口
   2012/07/27      v3.1.4.1     zhw                    修改数据路径为绝对路径
   2012/08/16      v3.1.4.2     zhw                    增加是否过滤模拟量开关para_IsFltSim
   2012/08/21      v3.1.4.3     zhw                    增加向综合智能告警(科东)发送消息
   2012/08/28      v3.1.5       zhw                    告警联调
   2012/09/20      v3.1.5.1     zhw                    读取旧格式文件时间间隔错误
   2012/09/22      v3.1.5.2     zhw                    修正低频振荡案例标志文件振荡标志错误
   2012/10/16      v3.1.6       zhw                    告警字符长度限值在200以内
   WAMS前置遥测定义表从WAMS_FES应用读取
   增加厂站名称是否显示开关
   增加告警应用参数
   运行路径通过启动参数传入
   2012/11/30      v3.1.6.1     zhw                    更新旧格式文件读取方式，添加交叠窗口
   更新模式对应时间
   UpdateHisCase函数中不清空LFO实时CASE窗口表内容，修改为追加
   2012/12/19      v3.1.7       zhw                    去掉消息订阅
   2012/12/21      v3.1.7       zhw                    SAFE_DELETE修正为SAFE_DELETE_ARRAY
   2013/01/09      v3.1.7       zhw                    修改采样和告警方式
   2013/02/21      v3.1.8       zhw                    修改科东消息接口分群(group_num)默认值
   2013/03/07      v3.1.8.1     zhw                    判断前置质量码
   2013/04/22      v3.1.9       cyj                    PC493,数组越界程序自动清空，并保存结果
   2013/04/24      v3.1.9_1     cyj                    GJ363,根据电压等级设置振幅限值
   2013/10/11      v3.1.10_1    cyj					   PC0619,采样接口定义成全局变量
   2014/01/28      v3.1.11_1    cyj					   根据配置项决定是否数据检查,放开发电机有功异常检查
   2014/02/18      v3.1.12_1    cyj					   内存分配后未释放修改
   2014/05/07      v3.1.13_1    cyj					   pc_0755振荡变压器数不对修改
													   GJ_0631测点只判有功、功角
******************************************************************************/
#include "WamSsoMonObj.h"
#include "ThreadFunc.h"
#include <string.h> 
// #ifndef _WINDOWS32
// 	#include "../lib/ThreadFunc.h"                       //  CThreadFuncs::startThread()
// #else
// 	#include "..\lib\ThreadFunc.h"                       //  CThreadFuncs::startThread()
// #endif

// 全局变量定义
/******************************************************************************/
// API 接口
// #ifndef USE_DB_NET_API
// 	ODB::CTableOp                 th_TableOp;            //  实时库本地接口
//  	ODB::CTableOp                 g_IDBTableOp;          //  实时库本地接口
// #else
// 	NET_ODB::CTableNet            th_TableOp;            //  实时库网络接口
//  	NET_ODB::CTableNet            g_IDBTableOp;          //  实时库网络接口
// #endif
///////////////////////////////////////////////////////////////////////////
CLfoAnaObj      *      g_pAnaObj  ;                         //  计算类

int   g_appNo = AP_WAMS;
string  g_appName = AF_WAMS_NAME;
int   g_ctxNo = AC_REALTIME_NO;
string  g_ctxName = AC_REALTIME_NAME;
char  g_LogStr[MAXBUF];
char g_ProName[] = "wamssomon";
char g_ProVersion[] = "v1.0.1_1";

//声明线程函数
bool                   g_ProcessExitFlag ;//= true;                   //  进程退出标志
int                    g_nCalcThrd       ;//= 1;                      //  线程个数
CALC_THRD_CASE *       g_pCalcThrdCase   ;//= NULL ;                  //  线程case
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
	g_ProcessExitFlag   =  true;     //  进程退出标志
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

	// 初始化日志
	string tmpString = g_pAnaObj->m_RunInfDir;
	tmpString = tmpString + g_pAnaObj->m_backslash + "ini" + g_pAnaObj->m_backslash +  "Default.ini";
	CIniFile iniFile(tmpString);
	int tmpDebugType = -1;
	if( iniFile.INI_ReadFile() == true )
	{
		tmpDebugType = iniFile.INI_GetValueI("控制参数","DebugType",-1);
	}
	
	tmpString = g_pAnaObj->m_RunInfDir;
	tmpString = tmpString + g_pAnaObj->m_backslash + "log" + g_pAnaObj->m_backslash +  g_ProName + ".log";
	g_pAnaObj->PLF_InitLogFile(tmpString);
	if (tmpDebugType >= 0)
	{
		g_pAnaObj->PLF_setLogType(tmpDebugType);
	}
	sprintf(g_LogStr,"日志文件名[%s]",tmpString.c_str());
	g_PubLogMessShow(g_LogStr,LTDEBUG);
	printf("[%s]\n",g_pAnaObj->m_backslash);

	// 进程注册
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
		    //开发人员提供名称、版本、功能等信息
		    cout<<"||程序名称:次同步振荡在线监视"<<endl;
		    cout<<"||英文名称:"<<g_ProName<<endl;
		    cout<<"||版本  号:"<<g_ProVersion<<endl;
		    cout<<"||创建时间:2017-04-11"<<endl;
		    cout<<"||启动参数:进程名 主目录"<<endl;    
		    cout<<"||功能说明:次同步振荡在线监视"<<endl; 
		    cout<<"||开发人员:应用软件开发部"<<endl;                
		    cout<<"||最近维护时间:2017-05-12"<<endl; 
		    cout<<"||最近修订内容:新开发"<<endl; 
		    cout<<"||维护人员:应用软件开发部"<<endl;
		    sprintf(tmpver,"||备    注:%s Ver %s build at %s %s",g_ProName, g_ProVersion, __DATE__, __TIME__);
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
	// 使用一次线程 th_TableOp,解决多实体问题
	//int thrednum = Thred_NetTableGetRecNum(LFO_AP_ID, LFO_FAC_NO);
	//  创建线程
	g_ProcessExitFlag = false;                 //  进程退出标志
	if (g_nCalcThrd < 0) g_nCalcThrd = 0;
	//g_nCalcThrd       = 6 ;
	
	g_pCalcThrdCase = (CALC_THRD_CASE *)calloc(g_nCalcThrd, sizeof(CALC_THRD_CASE));
	memset( (void *)g_pCalcThrdCase, 0, sizeof(CALC_THRD_CASE) * g_nCalcThrd);
	for (int kk = 0; kk < g_nCalcThrd; kk++)
	{
		g_pCalcThrdCase[kk].thrd_id = kk+1;
		sprintf( g_LogStr, "CalcMutex%d", g_pCalcThrdCase[kk].thrd_id);
		THRD_InitThreadMutex( g_pCalcThrdCase[kk].ThrdCaseMutex, g_LogStr);
		sprintf(g_LogStr, "创建计算线程[%d]!", g_pCalcThrdCase[kk].thrd_id);
		g_PubLogMessShow(g_LogStr);
		CThreadFuncs::startThread(CalcThreadMain, (void *)&g_pCalcThrdCase[kk]);
		g_pAnaObj->PT_DelaySecond(1);
	}
	//////////////////////////////////////////////////////////////////////////
	//  创建曲线刷新线程
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

