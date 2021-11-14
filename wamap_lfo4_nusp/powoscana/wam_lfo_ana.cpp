/******************************************************************************
  ModuleName :   WAMS LFO Analysis main function
  FileName   :   wam_lfo_ana.cpp
  Description:   Defines the entry point for the LFO Analysis application.
  HISTORY :
	  Date        Version       Author                Activities
   ----------------------------------------------------------------------
   2011/08/03      v4.0.0.0     zhw                    ����
   2011/09/05      v4.0.0.0     zhw                    �������д����ż�ֵ�ĸ澯ģʽ
   2012/01/20      v4.0.0.1     zhw                    ÿ�ּ����ˢ��EMS�澯����(��ֹ����λ)
   2012/02/02      v4.0.1       zhw                    ����pubodbapi,pubfun
   TODO:�澯��ʼʱ������(������Ϣ���棬�豸��ϢҲ�軺��)
   2012/03/19      v4.0.2       zhw                    TODO:����100֡�ļ��ٻ�
   ÿ��ʱ�������ֵ����ģʽ;���ֵ����Сֵʱ�̲���Ӧ
   2012/03/27
   �豸������ʱ������ʾ������;���ļ���ȡ����ʱ֧�ֽ�������;
   ֧�ֶ�ȡ�ļ���ʱ��;
   2012/07/11      v4.0.2.1     zhw                    ���������ļ��¸�ʽ֧��
   2012/08/30      v4.0.2.2     zhw                    ����pubodbapi,������־�������
   �����սṹ�庯��,������־������10
   �豸ֱ���жϰ�ȫʱ������豸ʱ��
   �޸ĸ澯�ӿ�
   2012/11/11      v4.0.2.3     zhw                    ����PronyAnalysis
   ����·��ͨ����������
   �޸�Ĭ�ϲ���
   ����ģʽʱ����������ж�
   ����ļ������������Ϣ
   2013/04/01      v4.0.2.4     zhw                    �������Ӣ��(����)
   2013/12/05      v4.0.3_1     cyj                    �޸�ÿ�ַ���ʱ���ȡ��ʽ
   2013/12/05      v4.0.4_1     cyj                    ����PMU���������޸�
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
int b_end_proc;//�Ƿ��˳�����
string g_dir;
#ifdef _D5000
char  g_Version[] = "v2.0.3_2";
#else
char  g_Version[] = "v4.0.6_1";
#endif
//�����ļ�λ���뱾�ֽ���ʱ���ϵ
long g_pos_threadid;//��Ҫ��¼�ļ�λ�õ��߳�ID

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
			//������Ա�ṩ���ơ��汾�����ܵ���Ϣ
			cout << "==================================================================" << endl;
			cout << "||��������:�������������߼��������" << endl;
			cout << "||Ӣ������:" << g_ProName << endl;
			cout << "||�汾  ��:" << g_Version << endl;
			cout << "||����ʱ��:2014-12-16" << endl;

			cout << "||��������:������ ��Ŀ¼" << endl;
			cout << "||����˵��:���������߼��ӷ������Ŷ�Դ��λ" << endl;
			cout << "||������Ա:Ӧ�����������" << endl;
			cout << "||���ά��ʱ��:2021-09-24" << endl;
			cout << "||����޶�����:֧����һ��ϵͳ" << endl;
			cout << "||ά����Ա:Ӧ�����������" << endl;
			sprintf(tmpver, "||��    ע:%s Ver %s build at %s %s", g_ProName, g_Version, __DATE__, __TIME__);
			cout << tmpver << endl;
			cout << "==================================================================" << endl;
			exit(0);
		}
	}
	printf("���̳�ʼ��\n");
	if (MainInit(argc, argv) == false) MainExit(-1);

	sprintf(g_LogStr, "********%s Ver %s build at %s %s********", g_ProName, g_Version, __DATE__, __TIME__);
	g_PubLogMessShow(g_LogStr);

	sprintf(g_LogStr, "����������%d", argc);
	g_PubLogMessShow(g_LogStr);
	for (int ii = 0; ii < argc; ++ii)
	{
		sprintf(g_LogStr, "%s", argv[ii]);
		g_PubLogMessShow(g_LogStr);
	}

	//int retcode = g_pAnaObj->MainProc();
//����Դ�л�
	int retcode;
	while (true)
	{
		b_end_proc = 0;
		retcode = g_pAnaObj->MainProc();
		if (retcode != 0) break;
	}

	sprintf(g_LogStr, "main���������˳�,retcode=%d", retcode);
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
		std::cout << "g_pAnaObj��ʵ����ʧ��" << std::endl;
		return false;
	}
	printf("g_pAnaObj��ʵ�����ɹ�");
	// ��Ŀ¼
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
		// ����·��ͨ����������
		tmpString = argv[1];
		g_pAnaObj->PS_StringTrimAll(tmpString);
		// ���·���Ƿ����
		if (g_pAnaObj->PF_FindDir(tmpString) == false)
		{
			std::cout << "����·������" << std::endl;
		}
		else
		{
			g_pAnaObj->m_maindir = tmpString;
		}
	}

	// ��ʼ����־
	tmpString = g_pAnaObj->m_maindir;
	tmpString = tmpString + g_pAnaObj->m_backslash + "ini" + g_pAnaObj->m_backslash + g_ProName + ".ini";
	CIniFile iniFile(tmpString);
	int tmpDebugType = -1;
	if (iniFile.INI_ReadFile() == true)
	{
		tmpDebugType = iniFile.INI_GetValueI("�������", "debug_type", -1);
	}

	tmpString = g_pAnaObj->m_maindir;
	tmpString = tmpString + g_pAnaObj->m_backslash + "log" + g_pAnaObj->m_backslash + g_ProName + ".log";
	g_pAnaObj->PLF_InitLogFile(tmpString);
	if (tmpDebugType >= 0)
	{
		g_pAnaObj->PLF_setLogType(tmpDebugType);
	}
	sprintf(g_LogStr, "��־�ļ���[%s]", tmpString.c_str());
	g_PubLogMessShow(g_LogStr, LTDEBUG);
	sprintf(g_LogStr, "������[%s],��Ŀ¼[%s]", g_ProName, g_pAnaObj->m_maindir.c_str());
	g_PubLogMessShow(g_LogStr);
	// 	if (g_pAnaObj->CheckProcessRunByName(g_ProName,g_pAnaObj->m_maindir.c_str())>0)
	// 	{
	// 		sprintf(g_LogStr,"����ͬ������������,�������˳�");
	// 		g_PubLogMessShow(g_LogStr,LTDEBUG);
	// 		return false;
	// 	}


		// ����ע��
#ifdef  _WAMAP_ 

#ifdef _D5000
	// D5000��ͨ��������Ϣͨ�����͸澯
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
		g_pAnaObj->Error(iSig);//�쳣��ֹ
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



