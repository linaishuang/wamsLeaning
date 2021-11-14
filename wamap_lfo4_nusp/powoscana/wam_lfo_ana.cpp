/******************************************************************************
  ModuleName :   WAMS LFO Analysis main function
  FileName   :   wam_lfo_ana.cpp
  Description:   Defines the entry point for the LFO Analysis application.
  HISTORY :
	  Date        Version       Author                Activities
   ----------------------------------------------------------------------
   2011/08/03      v4.0.0.0     zhw                    创建
   2011/09/05      v4.0.0.0     zhw                    保存所有大于门槛值的告警模式
   2012/01/20      v4.0.0.1     zhw                    每轮计算均刷新EMS告警开关(防止不复位)
   2012/02/02      v4.0.1       zhw                    更新pubodbapi,pubfun
   TODO:告警开始时间修正(窗口信息缓存，设备信息也需缓存)
   2012/03/19      v4.0.2       zhw                    TODO:增加100帧文件召唤
   每个时段输出幅值最大的模式;最大值、最小值时刻不对应
   2012/03/27
   设备无数据时，不显示振荡曲线;从文件读取数据时支持交叠窗口;
   支持读取文件中时间;
   2012/07/11      v4.0.2.1     zhw                    增加数据文件新格式支持
   2012/08/30      v4.0.2.2     zhw                    更新pubodbapi,修正日志输出错误
   添加清空结构体函数,增加日志个数到10
   设备直接判断安全时，清空设备时间
   修改告警接口
   2012/11/11      v4.0.2.3     zhw                    更新PronyAnalysis
   运行路径通过参数传入
   修改默认参数
   保存模式时增加阻尼比判断
   结果文件中输出参数信息
   2013/04/01      v4.0.2.4     zhw                    增加输出英文(伊朗)
   2013/12/05      v4.0.3_1     cyj                    修改每轮分析时间获取方式
   2013/12/05      v4.0.4_1     cyj                    南网PMU扩容升级修改
******************************************************************************/
#include "lfo_ana_obj.h"
//#include "LFOAnaByCurve.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////

CLfoAnaObj* g_pAnaObj = NULL;
#ifdef _D5000
//CFileOpSop g_sop_fileop;		//lns_20210811
#endif
//CLFOAnaByCurve g_lfoAnabycurve;

int   g_appNo = AP_WAMS;
string g_appName = AF_WAMS_NAME;
int   g_ctxNo = AC_REALTIME_NO;
string g_ctxName = AC_REALTIME_NAME;
char  g_LogStr[MAXBUF];
char  g_ProName[] = "powoscana";
int g_Thread_nun;
int b_end_proc;//是否退出程序
string g_dir;
#ifdef _D5000
char  g_Version[] = "v2.0.3_2";
#else
char  g_Version[] = "v4.0.6_1";
#endif
//保存文件位置与本轮结束时间关系
long g_pos_threadid;//需要记录文件位置的线程ID

//////////////////////////////////////////////////////////////////////////
void MainExit(int iSig);
bool MainInit(int argc, char* argv[]);
void g_PubLogMessShow(const char* log, const int logtype = LTINFO);
//////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		if (0 == strcmp(argv[1], "-v"))
		{
			char tmpver[128];
			//开发人员提供名称、版本、功能等信息
			cout << "==================================================================" << endl;
			cout << "||程序名称:电网功率振荡在线监视与分析" << endl;
			cout << "||英文名称:" << g_ProName << endl;
			cout << "||版本  号:" << g_Version << endl;
			cout << "||创建时间:2014-12-16" << endl;

			cout << "||启动参数:进程名 主目录" << endl;
			cout << "||功能说明:功率振荡在线监视分析、扰动源定位" << endl;
			cout << "||开发人员:应用软件开发部" << endl;
			cout << "||最近维护时间:2021-09-24" << endl;
			cout << "||最近修订内容:支持新一代系统" << endl;
			cout << "||维护人员:应用软件开发部" << endl;
			sprintf(tmpver, "||备    注:%s Ver %s build at %s %s", g_ProName, g_Version, __DATE__, __TIME__);
			cout << tmpver << endl;
			cout << "==================================================================" << endl;
			exit(0);
		}
	}
	printf("进程初始化\n");
	if (MainInit(argc, argv) == false) MainExit(-1);

	sprintf(g_LogStr, "********%s Ver %s build at %s %s********", g_ProName, g_Version, __DATE__, __TIME__);
	g_PubLogMessShow(g_LogStr);

	sprintf(g_LogStr, "启动参数：%d", argc);
	g_PubLogMessShow(g_LogStr);
	for (int ii = 0; ii < argc; ++ii)
	{
		sprintf(g_LogStr, "%s", argv[ii]);
		g_PubLogMessShow(g_LogStr);
	}

	//int retcode = g_pAnaObj->MainProc();
//数据源切换
	int retcode;
	while (true)
	{
		b_end_proc = 0;
		retcode = g_pAnaObj->MainProc();
		if (retcode != 0) break;
	}

	sprintf(g_LogStr, "main函数即将退出,retcode=%d", retcode);
	g_PubLogMessShow(g_LogStr);
	if (retcode == 0)
	{
		MainExit(0);
	}
	else
	{
		MainExit(-1);
	}

	return 0;
}

/******************************************************************************/
bool MainInit(int argc, char* argv[])
{
	g_pAnaObj = new CLfoAnaObj;
	if (g_pAnaObj == NULL)
	{
		std::cout << "g_pAnaObj类实例化失败" << std::endl;
		return false;
	}
	printf("g_pAnaObj类实例化成功");
	// 主目录
#ifdef _WINDOWS32
	g_pAnaObj->m_maindir = "..";
	sprintf(m_CasefDir, "..");
#else
	g_pAnaObj->m_maindir = "../data/wam_poa";
	sprintf(g_pAnaObj->m_CasefDir, "../data/wam/casefile");
#endif
	if (argc >= 2)
	{
		//		sprintf(g_dir,"%s",argv[1]);
		g_dir = argv[1];
	}
	string tmpString;
	if (argc >= 2)
	{
		// 运行路径通过参数传入
		tmpString = argv[1];
		g_pAnaObj->PS_StringTrimAll(tmpString);
		// 检查路径是否存在
		if (g_pAnaObj->PF_FindDir(tmpString) == false)
		{
			std::cout << "输入路径错误" << std::endl;
		}
		else
		{
			g_pAnaObj->m_maindir = tmpString;
		}
	}

	// 初始化日志
	tmpString = g_pAnaObj->m_maindir;
	tmpString = tmpString + g_pAnaObj->m_backslash + "ini" + g_pAnaObj->m_backslash + g_ProName + ".ini";
	CIniFile iniFile(tmpString);
	int tmpDebugType = -1;
	if (iniFile.INI_ReadFile() == true)
	{
		tmpDebugType = iniFile.INI_GetValueI("计算参数", "debug_type", -1);
	}

	tmpString = g_pAnaObj->m_maindir;
	tmpString = tmpString + g_pAnaObj->m_backslash + "log" + g_pAnaObj->m_backslash + g_ProName + ".log";
	g_pAnaObj->PLF_InitLogFile(tmpString);
	if (tmpDebugType >= 0)
	{
		g_pAnaObj->PLF_setLogType(tmpDebugType);
	}
	sprintf(g_LogStr, "日志文件名[%s]", tmpString.c_str());
	g_PubLogMessShow(g_LogStr, LTDEBUG);
	sprintf(g_LogStr, "进程名[%s],主目录[%s]", g_ProName, g_pAnaObj->m_maindir.c_str());
	g_PubLogMessShow(g_LogStr);
	// 	if (g_pAnaObj->CheckProcessRunByName(g_ProName,g_pAnaObj->m_maindir.c_str())>0)
	// 	{
	// 		sprintf(g_LogStr,"已有同名进程在运行,本进程退出");
	// 		g_PubLogMessShow(g_LogStr,LTDEBUG);
	// 		return false;
	// 	}


		// 进程注册
#ifdef  _WAMAP_ 

#ifdef _D5000
	// D5000不通过订阅消息通道发送告警
	g_pAnaObj->PODB_ProcRegister(AC_REALTIME_NAME, AF_WAMS_NAME, g_ProName);
#ifndef _NUSP
	g_pAnaObj->PODB_messageInit(AC_REALTIME_NAME, AF_WAMS_NAME, g_ProName);
#endif // !_NUSP
#else
	g_pAnaObj->PODB_ProcRegister(g_appNo, g_ProName, 0, g_ctxNo);
	g_pAnaObj->PODB_InitMsgBus(g_ProName, g_ProName, 1);
#endif  // _D5000
#endif

	return true;
}

void MainExit(int iSig)
{
	if (iSig != 0)
	{
		g_pAnaObj->Error(iSig);//异常中止
	}
	else
	{
		g_pAnaObj->Exit();
	}

	// 	SAFE_DELETE(g_pAnaObj);

	exit(iSig);
}

void g_PubLogMessShow(const char* log, const int logtype)
{
	g_pAnaObj->PLF_MessShow(log, logtype);
}



