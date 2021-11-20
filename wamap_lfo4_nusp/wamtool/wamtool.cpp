/******************************************************************************
  ModuleName :   wammvpi tool
  FileName   :   wammvpitool.cpp
  Description:   
  HISTORY :    	
      Date        Version       Author                Activities      
   ----------------------------------------------------------------------
   2011/06/01      1.0.0         zhw                    ����
   2011/10/20      1.0.1         zhw                    ���ӳ�վ���������������·�˵�ͱ�ѹ����������豸
   2012/05/17     v1.0.2         zhw                    ���½ӿں���,�ӳ�վ�л�ȡ�豸Ƶ��
   TODO:�����������ٶ�
   2012/06/19     v1.0.3         zhw                    lfo���²�������������Ϣ
   2012/07/26     v1.0.4         zhw                    ���Ӹ���wam_am�豸��(����)
   2012/10/30     v1.0.5         zhw                    WAMSǰ��ң�ⶨ����WAMS_FESӦ�ö�ȡ
   2017/06/21     v1.0.6         cyj                    PC1207
******************************************************************************/

#include "wamtoolobj.h"
#include <stdlib.h>
using namespace std;

CWamToolObj *g_pWamToolObj= NULL;//  MVTOOL�ඨ��
int   g_appNo = AP_WAMS;
string g_appName = AF_WAMS_NAME;
int   g_ctxNo = AC_REALTIME_NO;
string g_ctxName = AC_REALTIME_NAME;
string g_ProName  = "wamlfotool";  // ��������
string g_Version  = "1.0.7_1";  // ����汾��
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
			//������Ա�ṩ���ơ��汾�����ܵ���Ϣ
	    		cout<<"||��������:WAMS�����豸ά������"<<endl;
	    		cout<<"||Ӣ������:"<<g_ProName<<endl;
	    		cout<<"||�汾  ��:"<<g_Version<<endl;
	    		cout<<"||����ʱ��:2011-06-01"<<endl;  
			cout<<"||��������:wamlfotool -ctl new -volt 330 lfo/vf"<<endl;    
			cout<<"||����˵��:��Ƶ�����߼��ӷ����͵�ѹƵ�ʶ�̬�����豸ά��"<<endl; 
			cout<<"||������Ա:Ӧ�����������"<<endl;                
			cout<<"||���ά��ʱ��:2021-07-06"<<endl; 
	    		cout<<"||����޶�����:��������ظ��豸��¼����"<<endl; 
	    		cout<<"||ά����Ա:Ӧ�����������"<<endl;
			sprintf(tmpver,"||��    ע:%s Ver %s build at %s %s",g_ProName.c_str(), g_Version.c_str(), __DATE__, __TIME__);
			cout<<tmpver<<endl;
			exit(0);
		}
	}
	
	if ( WamInit(argc, argv) == false ) WamExit(-1);

	sprintf(g_LogStr,"********%s Ver %s build at %s %s********",g_ProName.c_str(), g_Version.c_str(), __DATE__, __TIME__);
	g_PubLogMessShow(g_LogStr);
	
	sprintf(g_LogStr,"����������%d",argc);
	g_PubLogMessShow(g_LogStr);
	for ( int ii = 0; ii < argc; ++ii )
	{
		sprintf(g_LogStr,"%s",argv[ii]);
		g_PubLogMessShow(g_LogStr);
	}

	if (argc<5)
	{
		sprintf(g_LogStr,"��������");
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
			sprintf(g_LogStr,"��������");
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
			sprintf(g_LogStr,"��ѹ�ȼ�Խ��[%f]",g_pWamToolObj->m_WTPara.VBase);
			g_PubLogMessShow(g_LogStr,LTERROR);
			WamExit(-3);
		}
	}

	if (argc>5)
	{
		// �ж�����ģʽ
		tmpstring = argv[5];
		g_pWamToolObj->PS_StringTrimAll(tmpstring);
		if(tmpstring == "lfo")
		{
			// lfo: ά����Ƶ���豸��
			g_pWamToolObj->m_WTPara.IsFacUpdate= 1;
			g_pWamToolObj->m_WTPara.IsAclnUpdate = 1;
			g_pWamToolObj->m_WTPara.IsGenUpdate = 1;
			g_pWamToolObj->m_WTPara.IsTrUpdate = 1;
			g_pWamToolObj->m_WTPara.IsVFDevUpdate = 0;
		}
		else if (tmpstring == "vf")
		{
			// vf: ά����ѹƵ���豸��
			g_pWamToolObj->m_WTPara.IsFacUpdate= 0;
			g_pWamToolObj->m_WTPara.IsAclnUpdate = 0;
			g_pWamToolObj->m_WTPara.IsGenUpdate = 0;
			g_pWamToolObj->m_WTPara.IsTrUpdate = 0;
			g_pWamToolObj->m_WTPara.IsVFDevUpdate = 1;
		}
		else if (tmpstring == "am")
		{
			// am: ά��WAM_AM�豸��
			g_pWamToolObj->m_WTPara.IsAmGenUpdate = 1;
		}
		else
		{
			sprintf(g_LogStr,"��������[%s]",tmpstring.c_str());
			g_PubLogMessShow(g_LogStr,LTERROR);
		}
	}
	else
	{
		// Ĭ��ͬʱά����Ƶ�񵴺͵�ѹƵ���豸��
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
		sprintf(g_LogStr, "�����ļ�������[%s]", filename.c_str());
		g_PubLogMessShow(g_LogStr, LTERROR);
		return -1;
	}

	string sectionName = "���Ʋ���";
	int tmpDebugType = iniFile.INI_GetValueI("���Ʋ���","DebugType",-1);
	
	filename = g_pWamToolObj->m_maindir + g_pWamToolObj->m_backslash + "log" + g_pWamToolObj->m_backslash +  g_ProName + ".log";
	g_pWamToolObj->PLF_InitLogFile(filename);
	if (tmpDebugType >= 0)
	{
		g_pWamToolObj->PLF_setLogType(tmpDebugType);
	}
	sprintf(g_LogStr,"��־�ļ���[%s]",filename.c_str());
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
			g_PubLogMessShow("�����ʽ: wamlfotool -ctl new -volt 500 lfo");
			g_PubLogMessShow(" -ctl:����ģʽ:");
			g_PubLogMessShow("  new: ����������;all:����LFO�豸��;test:���Գ���,ֻ����д��");
			g_PubLogMessShow(" -volt:��ѹ�ȼ�:");
			g_PubLogMessShow("  500: ��ӵ�ѹ�ȼ�Ϊ500KV���ϵ���·;��Χ[0,2000]");
			g_PubLogMessShow("  lfo: ά����Ƶ���豸��");
			g_PubLogMessShow("  vf: ά����ѹƵ���豸��");
		}
		g_pWamToolObj->Error(iSig);//�쳣��ֹ
	}
	else
	{
// #ifdef _D5000
// 		sprintf(str_cmd,"%s","echo yes|down_load wams 8601 8602 8603 8610");
// 		int rtn_value = system(str_cmd);
// 		if (rtn_value != 0)
// 		{
// 			sprintf(g_LogStr,"��װ8601 8602 8603 8610��ʧ��");
// 		    g_PubLogMessShow(g_LogStr);
// 		}
// 		else
// 		{
// 			sprintf(g_LogStr,"��װ8601 8602 8603 8610��ɹ�");
// 		    g_PubLogMessShow(g_LogStr);
// 		}
// #endif
		g_pWamToolObj->Exit();
	}

	//д��־�ļ�
	char filename[256];
	sprintf(filename,"%s%s%s",g_pWamToolObj->g_MainDir,g_pWamToolObj->m_backslash, "wamtool_calc.sta");
	sprintf(g_LogStr,"��־�ļ�wamtool_calc.staȫ·��%s",filename);
	g_PubLogMessShow(g_LogStr);
	FILE * fp;
	fp=fopen(filename,"w");
	if (fp==NULL)
	{
		sprintf(g_LogStr,"���󣺴�����־�ļ�wamtool_calc.staʧ��");
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
	sprintf(g_LogStr,"�ܼ���ʱ��%f ��",tmpTime);
	g_PubLogMessShow(g_LogStr);

	SAFE_DELETE(g_pWamToolObj);
	
	exit(iSig);
}

void g_PubLogMessShow(const char *log, const int logtype)
{
	g_pWamToolObj->PLF_MessShow(log,logtype);
}



