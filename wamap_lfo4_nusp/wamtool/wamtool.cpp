/******************************************************************************
  ModuleName :   wammvpi tool
  FileName   :   wammvpitool.cpp
  Description:   
  HISTORY :    	
      Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   2011/06/01      1.0.0         zhw                    创建
   2011/10/20      1.0.1         zhw                    增加厂站，发电机，交流线路端点和变压器绕组监视设备
   2012/05/17     v1.0.2         zhw                    更新接口函数,从厂站中获取设备频率
   TODO:排序查找提高速度
   2012/06/19     v1.0.3         zhw                    lfo更新参数表中运行信息
   2012/07/26     v1.0.4         zhw                    增加更新wam_am设备表(安徽)
   2012/10/30     v1.0.5         zhw                    WAMS前置遥测定义表从WAMS_FES应用读取
   2017/06/21     v1.0.6         cyj                    PC1207
******************************************************************************/

#include "wamtoolobj.h"
#include <stdlib.h>
using namespace std;

CWamToolObj *g_pWamToolObj= NULL;//  MVTOOL类定义
int   g_appNo = AP_WAMS;
string g_appName = AF_WAMS_NAME;
int   g_ctxNo = AC_REALTIME_NO;
string g_ctxName = AC_REALTIME_NAME;
string g_ProName  = "wamlfotool";  // 程序名称
string g_Version  = "1.0.7_1";  // 程序版本号
char g_LogStr[MAXBUF];
#ifdef _WINDOWS32
static string G_SLASH 			= "\\";
#else
static string G_SLASH 			= "/";
#endif
void g_PubLogMessShow(const char *log, const int logtype=LTINFO);
struct  timeb   g_StartTime;
struct  timeb   g_EndTime;

void WamExit(int iSig);
bool WamInit(int argc, char * argv[]);

int main(int argc, char * argv[])
{
	if (argc == 2)
	{  
		if (0 == strcmp(argv[1], "-v"))
		{
			char tmpver[128];
			//开发人员提供名称、版本、功能等信息
	    		cout<<"||程序名称:WAMS功能设备维护工具"<<endl;
	    		cout<<"||英文名称:"<<g_ProName<<endl;
	    		cout<<"||版本  号:"<<g_Version<<endl;
	    		cout<<"||创建时间:2011-06-01"<<endl;  
			cout<<"||启动参数:wamlfotool -ctl new -volt 330 lfo/vf"<<endl;    
			cout<<"||功能说明:低频振荡在线监视分析和电压频率动态评估设备维护"<<endl; 
			cout<<"||开发人员:应用软件开发部"<<endl;                
			cout<<"||最近维护时间:2021-07-06"<<endl; 
	    		cout<<"||最近修订内容:解决生成重复设备记录问题"<<endl; 
	    		cout<<"||维护人员:应用软件开发部"<<endl;
			sprintf(tmpver,"||备    注:%s Ver %s build at %s %s",g_ProName.c_str(), g_Version.c_str(), __DATE__, __TIME__);
			cout<<tmpver<<endl;
			exit(0);
		}
	}
	
	if ( WamInit(argc, argv) == false ) WamExit(-1);

	sprintf(g_LogStr,"********%s Ver %s build at %s %s********",g_ProName.c_str(), g_Version.c_str(), __DATE__, __TIME__);
	g_PubLogMessShow(g_LogStr);
	
	sprintf(g_LogStr,"启动参数：%d",argc);
	g_PubLogMessShow(g_LogStr);
	for ( int ii = 0; ii < argc; ++ii )
	{
		sprintf(g_LogStr,"%s",argv[ii]);
		g_PubLogMessShow(g_LogStr);
	}

	if (argc<5)
	{
		sprintf(g_LogStr,"参数错误");
		g_PubLogMessShow(g_LogStr,LTERROR);
		WamExit(-1);
	}

	string tmpstring;
	tmpstring = argv[1];

	g_pWamToolObj->PS_StringTrimAll(tmpstring);
	if (tmpstring == "-ctl")
	{
		tmpstring = argv[2];
		g_pWamToolObj->PS_StringTrimAll(tmpstring);
		g_pWamToolObj->PS_StringMakeLower(tmpstring);
		if (tmpstring == "test")
		{
			g_pWamToolObj->m_WTPara.nMode = WT_MODE_TEST;
		}
		else if (tmpstring == "new")
		{
			g_pWamToolObj->m_WTPara.nMode = WT_MODE_NEW;
		}
		else if (tmpstring == "all")
		{
			g_pWamToolObj->m_WTPara.nMode = WT_MODE_ALL;
		}
		else
		{
			sprintf(g_LogStr,"参数错误");
			g_PubLogMessShow(g_LogStr,LTERROR);
			WamExit(-2);
		}
	}

	tmpstring = argv[3];
	g_pWamToolObj->PS_StringTrimAll(tmpstring);
	if (tmpstring == "-volt")
	{
		tmpstring = argv[4];
		g_pWamToolObj->PS_StringTrimAll(tmpstring);
		g_pWamToolObj->PS_StringMakeLower(tmpstring);
		g_pWamToolObj->m_WTPara.VBase = (float)atof(tmpstring.c_str());
		if (g_pWamToolObj->m_WTPara.VBase<0.0 || g_pWamToolObj->m_WTPara.VBase>2000.0)
		{
			sprintf(g_LogStr,"电压等级越限[%f]",g_pWamToolObj->m_WTPara.VBase);
			g_PubLogMessShow(g_LogStr,LTERROR);
			WamExit(-3);
		}
	}

	if (argc>5)
	{
		// 判断运行模式
		tmpstring = argv[5];
		g_pWamToolObj->PS_StringTrimAll(tmpstring);
		if(tmpstring == "lfo")
		{
			// lfo: 维护低频振荡设备表
			g_pWamToolObj->m_WTPara.IsFacUpdate= 1;
			g_pWamToolObj->m_WTPara.IsAclnUpdate = 1;
			g_pWamToolObj->m_WTPara.IsGenUpdate = 1;
			g_pWamToolObj->m_WTPara.IsTrUpdate = 1;
			g_pWamToolObj->m_WTPara.IsVFDevUpdate = 0;
		}
		else if (tmpstring == "vf")
		{
			// vf: 维护电压频率设备表
			g_pWamToolObj->m_WTPara.IsFacUpdate= 0;
			g_pWamToolObj->m_WTPara.IsAclnUpdate = 0;
			g_pWamToolObj->m_WTPara.IsGenUpdate = 0;
			g_pWamToolObj->m_WTPara.IsTrUpdate = 0;
			g_pWamToolObj->m_WTPara.IsVFDevUpdate = 1;
		}
		else if (tmpstring == "am")
		{
			// am: 维护WAM_AM设备表
			g_pWamToolObj->m_WTPara.IsAmGenUpdate = 1;
		}
		else
		{
			sprintf(g_LogStr,"参数错误[%s]",tmpstring.c_str());
			g_PubLogMessShow(g_LogStr,LTERROR);
		}
	}
	else
	{
		// 默认同时维护低频振荡和电压频率设备表
		g_pWamToolObj->m_WTPara.IsFacUpdate= 1;
		g_pWamToolObj->m_WTPara.IsAclnUpdate = 1;
		g_pWamToolObj->m_WTPara.IsGenUpdate = 1;
		g_pWamToolObj->m_WTPara.IsTrUpdate = 1;
		g_pWamToolObj->m_WTPara.IsVFDevUpdate = 1;
	}
	
	int retcode = g_pWamToolObj->WamToolMain();
	if( retcode == 0 ) {
		WamExit(0);
	}
	else {
		WamExit(-5);
	}
	return 0;
}

/******************************************************************************/
bool WamInit(int argc, char * argv[])
{
	g_pWamToolObj    = new CWamToolObj;
	string tmpCurDir = argv[0];
	string::size_type idx;
	idx = tmpCurDir.find_last_of(g_pWamToolObj->m_backslash);
	if( idx != string::npos )
	{
		tmpCurDir.erase(idx);
	}
	else
	{
		tmpCurDir = ".";
	}

	string filename;
	filename = g_pWamToolObj->m_maindir + g_pWamToolObj->m_backslash + "ini" + g_pWamToolObj->m_backslash +  g_ProName + ".ini";
	CIniFile iniFile(filename);
	if (iniFile.INI_ReadFile() == false)
	{
		sprintf(g_LogStr, "配置文件不存在[%s]", filename.c_str());
		g_PubLogMessShow(g_LogStr, LTERROR);
		return -1;
	}

	string sectionName = "控制参数";
	int tmpDebugType = iniFile.INI_GetValueI("控制参数","DebugType",-1);
	
	filename = g_pWamToolObj->m_maindir + g_pWamToolObj->m_backslash + "log" + g_pWamToolObj->m_backslash +  g_ProName + ".log";
	g_pWamToolObj->PLF_InitLogFile(filename);
	if (tmpDebugType >= 0)
	{
		g_pWamToolObj->PLF_setLogType(tmpDebugType);
	}
	sprintf(g_LogStr,"日志文件名[%s]",filename.c_str());
	g_PubLogMessShow(g_LogStr,LTDEBUG);
	
	g_StartTime = g_pWamToolObj->PT_GetNowMilliSeconds();
	g_EndTime   = g_pWamToolObj->PT_GetNowMilliSeconds();
	
	return true;
}

void WamExit(int iSig)
{
	char str_cmd[80];
	memset(str_cmd, 0, 80);
	if(iSig != 0 )
	{
		if (iSig>=-3)
		{
			g_PubLogMessShow("命令格式: wamlfotool -ctl new -volt 500 lfo");
			g_PubLogMessShow(" -ctl:运行模式:");
			g_PubLogMessShow("  new: 添加新增测点;all:更新LFO设备表;test:测试程序,只读表不写表");
			g_PubLogMessShow(" -volt:电压等级:");
			g_PubLogMessShow("  500: 添加电压等级为500KV以上的线路;范围[0,2000]");
			g_PubLogMessShow("  lfo: 维护低频振荡设备表");
			g_PubLogMessShow("  vf: 维护电压频率设备表");
		}
		g_pWamToolObj->Error(iSig);//异常中止
	}
	else
	{
// #ifdef _D5000
// 		sprintf(str_cmd,"%s","echo yes|down_load wams 8601 8602 8603 8610");
// 		int rtn_value = system(str_cmd);
// 		if (rtn_value != 0)
// 		{
// 			sprintf(g_LogStr,"下装8601 8602 8603 8610表失败");
// 		    g_PubLogMessShow(g_LogStr);
// 		}
// 		else
// 		{
// 			sprintf(g_LogStr,"下装8601 8602 8603 8610表成功");
// 		    g_PubLogMessShow(g_LogStr);
// 		}
// #endif
		g_pWamToolObj->Exit();
	}

	//写标志文件
	char filename[256];
	sprintf(filename,"%s%s%s",g_pWamToolObj->g_MainDir,g_pWamToolObj->m_backslash, "wamtool_calc.sta");
	sprintf(g_LogStr,"标志文件wamtool_calc.sta全路径%s",filename);
	g_PubLogMessShow(g_LogStr);
	FILE * fp;
	fp=fopen(filename,"w");
	if (fp==NULL)
	{
		sprintf(g_LogStr,"错误：创建标志文件wamtool_calc.sta失败");
		g_PubLogMessShow(g_LogStr);
		return;
	}
	char txt[256];
	memset(txt,'\0',256);
	if (iSig==0)
	{
		strcpy(txt,"0");
	}
	else
	{
		strcpy(txt,"-1");	
	}
	fprintf(fp,"%s",txt);
	fclose(fp);
	
	g_EndTime = g_pWamToolObj->PT_GetNowMilliSeconds();
	float tmpTime = (g_pWamToolObj->PT_CalculateTimeInterval(g_StartTime,g_EndTime))/1000.0f;
	sprintf(g_LogStr,"总计用时：%f 秒",tmpTime);
	g_PubLogMessShow(g_LogStr);

	SAFE_DELETE(g_pWamToolObj);
	
	exit(iSig);
}

void g_PubLogMessShow(const char *log, const int logtype)
{
	g_pWamToolObj->PLF_MessShow(log,logtype);
}



