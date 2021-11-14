// mv =toolobj.cpp: implementation of the CMvToolObj class.
//
//////////////////////////////////////////////////////////////////////

#include "wamtoolobj.h"
extern void g_PubLogMessShow(const char* log, const int logtype = LTINFO);
//////////////////////////////////////////////////////////////////////////
bool g_SortGnByIdx(WT_DEV_INFO gen1, WT_DEV_INFO gen2)
{
	// gen1ǰһ��,gen2��һ��,false����,true������
	if (gen1.sim_curve_idx > gen2.sim_curve_idx)
	{
		return false;
	}
	else
	{
		return true;
	}
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWamToolObj::CWamToolObj()
{
	InitPara();
}

CWamToolObj::~CWamToolObj()
{

}

void CWamToolObj::InitPara()
{
#ifdef _WINDOWS32
	m_maindir = "..";
	sprintf(m_backslash, "\\");
#else
	char m_Path[128];
	strcpy(m_Path, getenv("HOME"));
	string m_homedir = m_Path;
	m_maindir = m_homedir + "/data/wam_sso/mon";
	string cts_maindir = m_homedir + "/wams_bin";
	strcpy(g_MainDir, cts_maindir.c_str());
	sprintf(m_backslash, "/");
#endif
	memset(m_LogString, 0, sizeof(m_LogString));
	m_DebugType = LTINFO;

	memset((void*)&m_WTPara, 0, sizeof(WT_PARA_INFO));
	m_WTPara.nMode = WT_MODE_ALL;
	m_WTPara.VBase = 220.0f;
	m_WTPara.IsFacUpdate = 0;
	m_WTPara.IsGenUpdate = 0;
	m_WTPara.IsAclnUpdate = 0;
	m_WTPara.IsTrUpdate = 0;
	m_WTPara.IsVFDevUpdate = 1;
	m_WTPara.IsVFDevUpdate = 0;
	m_WTPara.IsVFDevUpdate = 0;
	m_WTPara.IsAmGenUpdate = 0;
	sprintf(m_WTPara.GenInfoFile, "%s", "gen_info.csv");
	return;
}

void CWamToolObj::MessShow(const char* log, const int logtype)
{
	g_PubLogMessShow(log, logtype);

	return;
}

void CWamToolObj::Error(const int nret)
{
	if (nret == -2)
	{
		MessShow("ʹ��˵��:");
		MessShow("�����ʽ: wammvtool -ctl new -volt 220");
		MessShow("-ctl���Ʋ���˵��:");
		MessShow("    new: ����������");
		MessShow("    all: ɾ��ԭ�豸��,���¸����豸��");
		MessShow("-volt���Ʋ���˵��:");
		MessShow("    220: ��ӵ�ѹ�ȼ�Ϊ220kv���ϵ���·; ��Χ[100,1000]");
		MessShow("lfo:ά����Ƶ���豸��");
		MessShow("vf :ά����ѹƵ���豸��");
		MessShow("am :ά��AM�豸��");
		MessShow("Ĭ��ͬʱά����Ƶ�񵴺͵�ѹƵ���豸��");
	}
	else if (nret == -5)
	{
		sprintf(m_LogString, "���ݱ�ά��ʧ��");
		MessShow(m_LogString, LTERROR);

		//WriteToolInf("���ݱ�ά��ʧ��");
	}

	return;
}

void CWamToolObj::Exit()
{
	time_t tmpTime = PT_GetNowSeconds();

	sprintf(m_LogString, "%s ���ݱ�ά�����", PT_GetTimeStringBySeconds(tmpTime));
	MessShow(m_LogString);
	//WriteToolInf(m_LogString);
	return;
}

//////////////////////////////////////////////////////////////////////////
int CWamToolObj::WamToolMain()
{
	string tmpstring = m_maindir + m_backslash + "ini" + m_backslash + g_ProName + ".ini";
	if (ReadIniFile(tmpstring, m_WTPara) == false)
	{
		MessShow("����Ĭ�ϲ���", LTERROR);
	}

	if (CheckPara(m_WTPara) == false)
	{
		return -1;
	}

	if (InitProject() < 0)
	{
		return -1;
	}

	// LFO�������,��LFO�������־
	if (m_WTPara.IsGenUpdate == 1)
	{
		GetLfoGen(m_VecGenInfo);
		// �õ�ѹ�ȼ���־,�Ƿ�������־
		SetAddFlag(m_VecGenInfo, SSO_GN_NO_TAB);
		//WriteDevFile(m_VecGenInfo, SSO_GN_NO_TAB);
		// ���ӷ����
		WriteDevNew(m_VecGenInfo, SSO_GN_NO_TAB);
	}

	// LFO������·��,��LFO������·��־
	if (m_WTPara.IsAclnUpdate == 1)
	{
		MessShow("������·");
		GetLfoAcln(m_VecAclnInfo);
		MessShow("��ȡSSO��·�����");
		// �õ�ѹ�ȼ���־,�Ƿ�������־
		SetAddFlag(m_VecAclnInfo, SSO_ACLN_NO_TAB);
		//WriteDevFile(m_VecAclnInfo, SSO_ACLN_NO_TAB);
		// ���ӽ�����·
		WriteDevNew(m_VecAclnInfo, SSO_ACLN_NO_TAB);
	}

	// LFO��ѹ����,��LFO��ѹ����־
	if (m_WTPara.IsTrUpdate == 1)
	{
		MessShow("�����ѹ��");
		GetLfoTr(m_VecTrInfo);
		MessShow("��ȡSSO��ѹ�������");
		// �õ�ѹ�ȼ���־,�Ƿ�������־
		SetAddFlag(m_VecTrInfo, SSO_TR_NO_TAB);
		//WriteDevFile(m_VecTrInfo, SSO_TR_NO_TAB);
		// ���ӱ�ѹ��
		WriteDevNew(m_VecTrInfo, SSO_TR_NO_TAB);
	}
	// LFO��վ��,��LFO��վ��־
	if (m_WTPara.IsFacUpdate == 1)
	{
		GetLfoFac(m_VecFacInfo);
		// �õ�ѹ�ȼ���־,�Ƿ�������־
		SetAddFlag(m_VecFacInfo, SSO_FAC_NO_TAB);
		//WriteDevFile(m_VecFacInfo, SSO_FAC_NO_TAB);
		// ���ӳ�վ
		WriteDevNew(m_VecFacInfo, SSO_FAC_NO_TAB);
	}

	// VF��̬����ĸ�߱�,��VFĸ�߱�־
	if (m_WTPara.IsVFDevUpdate == 1)
	{
		GetVFBus(m_VecVFDevInfo);
		// �õ�ѹ�ȼ���־,�Ƿ�������־
		SetAddFlag(m_VecVFDevInfo, PQ_DA_BUS_NO_TAB);
		//WriteDevFile(m_VecVFDevInfo, PQ_DA_BUS_NO_TAB);
		// ����VF��̬����ĸ��
		WriteDevNew(m_VecVFDevInfo, PQ_DA_BUS_NO_TAB);
	}

	// WAM_AM�豸��
	if (m_WTPara.IsAmGenUpdate == 1)
	{
		GetAMGen(m_VecAmGenInfo);
		// ���Ƿ�������־
		SetAddFlag(m_VecAmGenInfo, WAM_AM_DEV_NO_TAB);
		//WriteDevFile(m_VecAmGenInfo, WAM_AM_DEV_NO_TAB);
		// �����豸
		WriteDevNew(m_VecAmGenInfo, WAM_AM_DEV_NO_TAB);
	}

	if (m_WTPara.IsGenUpdate == 1 || m_WTPara.IsAclnUpdate == 1 || m_WTPara.IsTrUpdate == 1)
	{
		sprintf(m_LogString, "���ݱ�ά�����,������վ%d��,������·%d��,�����%d̨,��ѹ��%d̨",
			m_WTPara.nFacNew, m_WTPara.nAclnNew, m_WTPara.nGenNew, m_WTPara.nTrNew);
		MessShow(m_LogString, LTINFO);
		// WriteToolInf(m_LogString);
	}
	else if (m_WTPara.IsVFDevUpdate == 1)
	{
		sprintf(m_LogString, "���ݱ�ά�����,�����豸[%d]", m_WTPara.nVFNew);
		MessShow(m_LogString, LTINFO);
	}
	else if (m_WTPara.IsAmGenUpdate == 1)
	{
		sprintf(m_LogString, "AM���ݱ�ά�����,����[%d]", m_WTPara.nAMNew);
		MessShow(m_LogString, LTINFO);
	}

	return 0;
}


bool CWamToolObj::ReadIniFile(const string IniFileName, WT_PARA_INFO& tmpPara)
{
	CIniFile iniFile(IniFileName);
	if (iniFile.INI_ReadFile() == false)
	{
		sprintf(m_LogString, "�����ļ���ȡʧ��[%s]", IniFileName.c_str());
		MessShow(m_LogString, LTERROR);
		return false;
	}

	string sectionName = "Ӧ�ò���";
	g_ctxName = iniFile.INI_GetValue(sectionName, "ctx_name", AC_REALTIME_NAME);
	g_ctxNo = iniFile.INI_GetValueI(sectionName, "ctx_no", AC_REALTIME_NO);
	g_appName = iniFile.INI_GetValue(sectionName, "app_name", AF_WAMS_NAME);
	g_appNo = iniFile.INI_GetValueI(sectionName, "app_no", AP_WAMS);

	sectionName = "���Ʋ���";
	//    m_DebugType = iniFile.INI_GetValueI(sectionName,"DebugType",LTINFO);
	// 	tmpPara.nMode = iniFile.INI_GetValueI(sectionName,"nMode",WT_MODE_ALL);
	// 	tmpPara.VBase = iniFile.INI_GetValueFF(sectionName,"VBase",220.0);
	// 	tmpPara.IsFacUpdate =iniFile.INI_GetValueI(sectionName,"IsFacUpdate",0);
	// 	tmpPara.IsGenUpdate  =iniFile.INI_GetValueI(sectionName,"IsGenUpdate",0);
	// 	tmpPara.IsAclnUpdate=iniFile.INI_GetValueI(sectionName,"IsAclnUpdate",0);
	// 	tmpPara.IsTrUpdate  =iniFile.INI_GetValueI(sectionName,"IsTrUpdate",0);
	// 	tmpPara.IsVFDevUpdate  =iniFile.INI_GetValueI(sectionName,"IsVFBusUpdate",1);
	sprintf(tmpPara.GenInfoFile, "%s", iniFile.INI_GetValue(sectionName, "GenInfoFile", "gen_info.csv").c_str());
	return true;
}

bool CWamToolObj::CheckPara(WT_PARA_INFO& tmpPara)
{
	bool bFlag = true;
	if (tmpPara.nMode == WT_MODE_NEW)
	{
		sprintf(m_LogString, "�����豸[%d]", tmpPara.nMode);
		MessShow(m_LogString, LTDEBUG);
	}
	else if (tmpPara.nMode == WT_MODE_ALL)
	{
		sprintf(m_LogString, "����ȫ���豸[%d]", tmpPara.nMode);
		MessShow(m_LogString, LTDEBUG);
	}
	else if (tmpPara.nMode == WT_MODE_TEST)
	{
		sprintf(m_LogString, "���Գ���[%d]", tmpPara.nMode);
		MessShow(m_LogString, LTDEBUG);
	}
	else
	{
		sprintf(m_LogString, "���Ʋ�������[%d]", tmpPara.nMode);
		MessShow(m_LogString, LTDEBUG);
		bFlag = false;
	}

	if (tmpPara.VBase < 0.0f || tmpPara.VBase > 1000.0f)
	{
		sprintf(m_LogString, "��ѹ��������[%.2f]", tmpPara.VBase);
		MessShow(m_LogString, LTDEBUG);
		bFlag = false;
	}
	else
	{
		sprintf(m_LogString, "��ѹ����[%.2f]", tmpPara.VBase);
		MessShow(m_LogString, LTDEBUG);
	}

	return bFlag;
}

//////////////////////////////////////////////////////////////////////////
int CWamToolObj::InitProject()
{
#ifdef _WAMAP_
	// ��վ��Ϣ��
	vector<FAC_TAB_CRE> VecFacTab;
	if (PODB_GetFacInfo(VecFacTab) < 0)
	{
		return -1;
	}

	// ���������
	vector<AREA_TAB_CRE> VecAreaTab;
	if (PODB_GetAreaInfo(VecAreaTab) < 0)
	{
		return -1;
	}
	// WAMSǰ��ң�ⶨ���
	vector<FES_YC_TAB_CRE> VecFesYcTab;
	if (PODB_GetFesYcInfo(VecFesYcTab) < 0)
	{
		return -1;
	}
	// WAMSǰ��ң�Ŷ����
	vector<FES_YX_TAB_CRE> VecFesYxTab;
	if (PODB_GetFesYxInfo(VecFesYxTab))
	{
		return -1;
	}
	vector<FES_YX_YC> VecFesYxYc;
	PODB_YxYc(VecFesYcTab, VecFesYxTab, VecFesYxYc);
	for (int ii = 0; ii < VecFesYxYc.size(); ii++)
	{
		g_map_colno.insert(multimap<INT_REC, int>::value_type(PODB_GetRecordId(VecFesYxYc[ii].yc_id), ii));
	}


	// ��ѹ���ͱ�
	vector<VL_TYPE_TAB_CRE> VecVLTypeTab;
	if (PODB_GetVLTypeInfo(VecVLTypeTab) < 0)
	{
		return -1;
	}

	// �����
	//if (m_WTPara.IsGenUpdate == 1)
	{
		if (InitGenInfo(m_VecGenInfo, VecFacTab, VecAreaTab, VecVLTypeTab) < 0)
		{
			return -1;
		}
		SetYcFlag(m_VecGenInfo, VecFesYxYc, SSO_GN_NO_TAB);
	}

	// ������·
	//if (m_WTPara.IsAclnUpdate == 1)
	{
		if (InitAclnInfo(m_VecAclnInfo, VecFacTab, VecAreaTab, VecVLTypeTab) < 0)
		{
			return -1;
		}
		SetYcFlag(m_VecAclnInfo, VecFesYxYc, SSO_ACLN_NO_TAB);
	}

	// ��ѹ��
	//if (m_WTPara.IsTrUpdate == 1)
	{
		if (InitTrInfo(m_VecTrInfo, VecFacTab, VecAreaTab, VecVLTypeTab) < 0)
		{
			return -1;
		}

		SetYcFlag(m_VecTrInfo, VecFesYxYc, SSO_TR_NO_TAB);
	}

	// ��վ��Ϣ
	//if (m_WTPara.IsFacUpdate == 1)
	{
		if (InitFacInfo(m_VecFacInfo, VecFacTab, VecAreaTab, VecVLTypeTab, m_VecGenInfo, m_VecAclnInfo, m_VecTrInfo) < 0)
		{
			return -1;
		}
		// �ò��ң���־
	}

	// VF��̬�����豸��Ϣ
	if (m_WTPara.IsVFDevUpdate == 1)
	{
		if (InitVFDevInfo(m_VecVFDevInfo) < 0)
		{
			return -1;
		}
	}

	// WAM_AM�豸����Ϣ
	if (m_WTPara.IsAmGenUpdate == 1)
	{
		if (InitAMGenInfo(m_VecAmGenInfo) < 0)
		{
			return -1;
		}
	}
#endif
	return 0;
}

// int CWamToolObj::InitBusInfo(vector<WT_DEV_INFO> &VecDevInfo,vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<VL_TYPE_TAB_CRE> &VecVLTypeTab)
// {
// 	if (!VecDevInfo.empty())
// 	{
// 		VecDevInfo.clear();
// 	}
// 
// 	// ĸ�߱�
// 	vector<BUS_TAB_CRE> VecBusTab;
// 	if (PODB_GetBusInfo(VecBusTab)<0)
// 	{
// 		return -1;
// 	}
// 
// 	// ���ң����Ϣ��
// 	vector<VALUEINFO_TAB_CRE> VecValueInfoTab;
// 	if (PODB_GetValueInfo(VecValueInfoTab)<0)
// 	{
// 		return -1;
// 	}
// 
// 	// VF��̬����������,��ȡĬ�϶�Ԫ�����
// 	vector<PQ_DA_CTL_PARA_TAB_CRE> VecVFPara;
// 	if (GetVFPara(VecVFPara)<0)
// 	{
// 		return -1;
// 	}
// 	INT_REC tmp_dft_v_para = 0;
// 	INT_REC tmp_dft_f_para = 0;
// 	if (!VecVFPara.empty())
// 	{
// 		tmp_dft_f_para = VecVFPara[0].dft_f_para;
// 		tmp_dft_v_para = VecVFPara[0].dft_v_para;
// 	}
// 
// 	int ii;
// 	WT_DEV_INFO tmpDevInfo;
// 	for ( ii =0; ii<(int)VecBusTab.size(); ii++ )
// 	{
// 		// ĸ����Ϣ
// 		memset((void *)&tmpDevInfo,0,sizeof(WT_DEV_INFO));
// 		tmpDevInfo.dev_id = VecBusTab[ii].bs_id;
// 		tmpDevInfo.fac_id = VecBusTab[ii].fac_id;
// 		tmpDevInfo.maxvlty_id = VecBusTab[ii].vlty_id;
// 		tmpDevInfo.dev_type = VecBusTab[ii].bs_type;
// 		tmpDevInfo.volt_per_id = tmp_dft_v_para;
// 		tmpDevInfo.freq_per_id = tmp_dft_f_para;
// 		sprintf(tmpDevInfo.dev_name,"%s",VecBusTab[ii].bs_name);
// 		
// 		VecDevInfo.push_back(tmpDevInfo);
// 	}
// 	m_WTPara.nBusAll = (int)VecDevInfo.size();
// 
// 	// ��䳧վ����,����ID
// 	GetFacName(VecDevInfo,VecFacTab);
// 
// 	// �����������
// 	GetAreaName(VecDevInfo,VecAreaTab);
// 
// 	// ����ѹ�ȼ�
// 	GetVBase(VecDevInfo,VecVLTypeTab);
// 
// 	// ���Ƶ�ʲ����Ϣ
// #ifdef _D5000
// 
// #else
// 	int jj,nFlag;
// 	for ( ii = 0; ii<(int)VecDevInfo.size(); ii++)
// 	{
// 		nFlag = 0;
// 		for ( jj = 0; jj<(int)VecValueInfoTab.size(); jj++)
// 		{
// 			if (VecDevInfo[ii].fac_id == VecValueInfoTab[jj].fac_id)
// 			{
// 				nFlag = 1;
// 				VecDevInfo[ii].freq_yc_id = VecValueInfoTab[jj].value_id;
// 				break;
// 			}
// 		}
// 		if (nFlag ==0)
// 		{
// 			sprintf(m_LogString,"["INT_REC_FMT"][%s]δ�ҵ���ӦƵ�ʲ��["INT_REC_FMT"]",VecDevInfo[ii].dev_id,VecDevInfo[ii].dev_name,VecDevInfo[ii].fac_id);
// 			MessShow(m_LogString,LTERROR);
// 		}
// 	}
// #endif
// 
// 	// ���˵���Ϣ
// 	for ( ii = 0; ii<(int)VecDevInfo.size(); ii++)
// 	{
// 		VecDevInfo[ii].dot1.dot_id = VecDevInfo[ii].dev_id;
// 		VecDevInfo[ii].dot1.fac_id = VecDevInfo[ii].fac_id;
// 		VecDevInfo[ii].dot1.area_id = VecDevInfo[ii].area_id;
// 		VecDevInfo[ii].dot1.vlty_id = VecDevInfo[ii].maxvlty_id;
// 		VecDevInfo[ii].dot1.vbase = VecDevInfo[ii].maxvbase;
// 		sprintf(VecDevInfo[ii].dot1.dot_name,"%s",VecDevInfo[ii].dev_name);
// 		sprintf(VecDevInfo[ii].dot1.fac_name,"%s",VecDevInfo[ii].fac_name);
// 		sprintf(VecDevInfo[ii].dot1.area_name,"%s",VecDevInfo[ii].area_name);
// 	}
// 
// 	return 0;
// }

int CWamToolObj::GetVFBus(vector<WT_DEV_INFO>& VecDevInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(PQ_DA_BUS_NO_TAB, PQ_DA_BUS_TAB_CRE_FLD, buf_base) < 0)
	{
		sprintf(m_LogString, "��ȡVF��̬����ĸ�߱�ʧ��");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(PQ_DA_BUS_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "VF��̬����ĸ�߱��¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "VF��̬����ĸ�߱�[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	PQ_DA_BUS_TAB_CRE* pInfo = NULL;
	pInfo = new PQ_DA_BUS_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(PQ_DA_BUS_TAB_CRE));

	// ���Ѵ��ڱ�־
	m_WTPara.nVFOld = nInfo;
	int ii, jj;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < nInfo; jj++)
		{
			// bus_id��Ӧ��վ��ߵ�ѹ�ȼ�ĸ�ߣ�������������߶ζ˵�ͱ�ѹ������
			if (VecDevInfo[ii].dot1.dot_id == pInfo[jj].bus_id)
			{
				VecDevInfo[ii].is_old = 1;
				break;
			}
			else if (VecDevInfo[ii].dot2.dot_id == pInfo[jj].bus_id)
			{
				VecDevInfo[ii].is_old = 1;
				break;
			}
			else if (VecDevInfo[ii].dot3.dot_id == pInfo[jj].bus_id)
			{
				VecDevInfo[ii].is_old = 1;
				break;
			}
		}
	}

	SAFE_DELETE(pInfo);
#endif
	return 0;
}

int CWamToolObj::GetVFPara(vector<PQ_DA_CTL_PARA_TAB_CRE>& VecVFPara)
{
	if (!VecVFPara.empty())
	{
		VecVFPara.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(PQ_DA_CTL_PARA_NO_TAB, PQ_DA_CTL_PARA_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(PQ_DA_CTL_PARA_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "VF��̬�����������¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "VF��̬����������[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	PQ_DA_CTL_PARA_TAB_CRE* pInfo = NULL;
	pInfo = new PQ_DA_CTL_PARA_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(PQ_DA_CTL_PARA_TAB_CRE));

	int ii = 0;
	for (ii = 0; ii < nInfo; ii++)
	{
		VecVFPara.push_back(pInfo[ii]);
	}

	SAFE_DELETE(pInfo);
#endif
	return 0;
}

int CWamToolObj::InitFacInfo(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab, vector<WT_DEV_INFO>& VecSSOGen, vector<WT_DEV_INFO>& VecSSOLine, vector<WT_DEV_INFO>& VecSSOTr)
{
	if (!VecDevInfo.empty())
	{
		vector<WT_DEV_INFO>().swap(VecDevInfo);
		VecDevInfo.clear();
	}
#ifdef _WAMAP_

	int ii, jj, kk, nFlag;
	vector<INT_REC> fac_id;//�в��ĳ�վID
	map<INT_REC, int>::iterator it;
	for (ii = 0; ii < VecSSOGen.size(); ii++)
	{
		if (VecSSOGen[ii].yc_flag == 1)
		{
			fac_id.push_back(VecSSOGen[ii].fac_id);
		}
	}
	for (ii = 0; ii < VecSSOLine.size(); ii++)
	{
		if (VecSSOLine[ii].dot1.IsYcDefine == WT_MENU_YES)
		{
			fac_id.push_back(VecSSOLine[ii].dot1.fac_id);
		}
		if (VecSSOLine[ii].dot2.IsYcDefine == WT_MENU_YES)
		{
			fac_id.push_back(VecSSOLine[ii].dot2.fac_id);
		}
	}
	for (ii = 0; ii < VecSSOTr.size(); ii++)
	{
		if (VecSSOTr[ii].yc_flag == 6 || VecSSOTr[ii].yc_flag == 3 || VecSSOTr[ii].yc_flag == 1)//6-�������в�� 3-˫�����в�� 1-�������в��
		{
			fac_id.push_back(VecSSOTr[ii].fac_id);
		}
	}
	sort(fac_id.begin(), fac_id.end());//����
	fac_id.erase(unique(fac_id.begin(), fac_id.end()), fac_id.end());//ȥ���ظ���¼

	for (ii = 0; ii < fac_id.size(); ii++)
	{
		g_map_facid.insert(map<INT_REC, int>::value_type(fac_id[ii], ii));
	}

	WT_DEV_INFO tmpDevInfo;
	for (ii = 0; ii < (int)VecFacTab.size(); ii++)
	{
		// ��վ��Ϣ
		memset((void*)&tmpDevInfo, 0, sizeof(WT_DEV_INFO));
		tmpDevInfo.dev_id = VecFacTab[ii].fac_id;
		tmpDevInfo.fac_id = VecFacTab[ii].fac_id;
		tmpDevInfo.area_id = VecFacTab[ii].area_id;
		tmpDevInfo.dev_type = MENU_LFO_DEV_FAC;
		sprintf(tmpDevInfo.dev_name, "%s", VecFacTab[ii].fac_name);
		sprintf(tmpDevInfo.fac_name, "%s", VecFacTab[ii].fac_name);

		VecDevInfo.push_back(tmpDevInfo);
	}

	// �����������
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nFlag = 0;
		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (VecDevInfo[ii].area_id == VecAreaTab[jj].area_id)
			{
				nFlag = 1;
				sprintf(VecDevInfo[ii].area_name, "%s", VecAreaTab[jj].area_name);
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "δ�ҵ���Ӧ����["INT_REC_FMT"][%s]["INT_REC_FMT"]", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].area_id));
			MessShow(m_LogString, LTDEBUG);
		}
	}

	// ����ѹ�ȼ�
// 	for ( ii =0; ii<(int)VecDevInfo.size(); ii++)
// 	{
// 		for ( jj =0; jj<(int)VecVLTypeTab.size(); jj++)
// 		{
// 			if (VecDevInfo[ii].maxvlty_id == VecVLTypeTab[jj].vlty_id)
// 			{
// 				VecDevInfo[ii].maxvbase = VecVLTypeTab[jj].vbase;
// 				break;
// 			}
// 		}
// 	}
// 
	m_WTPara.nFacAll = (int)VecDevInfo.size();
	m_WTPara.nFacPmu = 0;
	for (ii = 0; ii < VecDevInfo.size(); ii++)
	{
		it = g_map_facid.begin();
		it = g_map_facid.find(VecDevInfo[ii].fac_id);
		if (it != g_map_facid.end())
		{
			VecDevInfo[ii].yc_flag = 1;
			m_WTPara.nFacPmu++;
			sprintf(m_LogString, "��վ[%s]�в��,��ѹ�ȼ�[%f]", VecDevInfo[ii].fac_name, VecDevInfo[ii].maxvbase);
			MessShow(m_LogString);
			continue;
		}
	}
	sprintf(m_LogString, "��վ����[%d],�в�㳧վ��[%d]", m_WTPara.nFacAll, m_WTPara.nFacPmu);
	MessShow(m_LogString);

#endif
	return 0;
}

int CWamToolObj::GetLfoFac(vector<WT_DEV_INFO>& VecDevInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_FAC_NO_TAB, SSO_FC_TAB_CRE_FLD, buf_base) < 0)
	{
		sprintf(m_LogString, "��ȡSSO��վ��ʧ��");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	int nInfo = buf_base.GetLength() / sizeof(SSO_FC_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO��վ���¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "SSO��վ��[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_FC_TAB_CRE* pInfo = NULL;
	pInfo = new SSO_FC_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_FC_TAB_CRE));

	// ���Ѵ��ڱ�־
	m_WTPara.nFacOld = nInfo;
	int ii, jj;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < nInfo; jj++)
		{
			// 			sprintf(m_LogString,"��yjj��ID[%ld],ID[%ld]",pInfo[jj].fac_id,VecDevInfo[ii].dev_id);
			// 			MessShow(m_LogString,LTERROR);
			if (VecDevInfo[ii].dev_id == pInfo[jj].station_id)
			{
				VecDevInfo[ii].is_old = 1;
				break;
			}
		}
	}

	SAFE_DELETE(pInfo);
#endif

	return 0;
}

int CWamToolObj::InitGenInfo(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab)
{
	if (!VecDevInfo.empty())
	{
		vector<WT_DEV_INFO>().swap(VecDevInfo);
		VecDevInfo.clear();
	}
#ifdef _WAMAP_
	// �������
	vector<GEN_TAB_CRE> VecGenTab;
	if (PODB_GetGenInfo(VecGenTab) < 0)
	{
		return false;
	}

	// ��䷢�����Ϣ
	int ii, jj;
	WT_DEV_INFO tmpDevInfo;
	for (ii = 0; ii < (int)VecGenTab.size(); ii++)
	{
		memset((void*)&tmpDevInfo, 0, sizeof(WT_DEV_INFO));
		tmpDevInfo.dev_id = VecGenTab[ii].gn_id;
		tmpDevInfo.fac_id = VecGenTab[ii].fac_id;
		tmpDevInfo.maxvlty_id = VecGenTab[ii].vlty_id;
		sprintf(tmpDevInfo.dev_name, "%s", VecGenTab[ii].gn_name);

		VecDevInfo.push_back(tmpDevInfo);
	}

	m_WTPara.nGenAll = (int)VecDevInfo.size();

	// ��䳧վ����
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < (int)VecFacTab.size(); jj++)
		{
			if (VecDevInfo[ii].fac_id == VecFacTab[jj].fac_id)
			{
				VecDevInfo[ii].area_id = VecFacTab[jj].area_id;
				sprintf(VecDevInfo[ii].fac_name, "%s", VecFacTab[jj].fac_name);
				break;
			}
		}
	}

	// �����������
	int nFlag;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nFlag = 0;
		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (VecDevInfo[ii].area_id == VecAreaTab[jj].area_id)
			{
				nFlag = 1;
				sprintf(VecDevInfo[ii].area_name, "%s", VecAreaTab[jj].area_name);
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "δ�ҵ���Ӧ����["INT_REC_FMT"][%s]["INT_REC_FMT"]", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].area_id));
			MessShow(m_LogString, LTDEBUG);
		}
	}

	// ����ѹ�ȼ�
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < (int)VecVLTypeTab.size(); jj++)
		{
			if (VecDevInfo[ii].maxvlty_id == VecVLTypeTab[jj].vlty_id)
			{
				VecDevInfo[ii].maxvbase = VecVLTypeTab[jj].vbase;
				break;
			}
		}
	}

	// ���˵���Ϣ
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		VecDevInfo[ii].dot1.dot_id = VecDevInfo[ii].dev_id;
		VecDevInfo[ii].dot1.fac_id = VecDevInfo[ii].fac_id;
		VecDevInfo[ii].dot1.area_id = VecDevInfo[ii].area_id;
		VecDevInfo[ii].dot1.vlty_id = VecDevInfo[ii].maxvlty_id;
		VecDevInfo[ii].dot1.vbase = VecDevInfo[ii].maxvbase;
		sprintf(VecDevInfo[ii].dot1.dot_name, "%s", VecDevInfo[ii].dev_name);
		sprintf(VecDevInfo[ii].dot1.fac_name, "%s", VecDevInfo[ii].fac_name);
		sprintf(VecDevInfo[ii].dot1.area_name, "%s", VecDevInfo[ii].area_name);
	}
#endif
	return 0;
}

int CWamToolObj::GetLfoGen(vector<WT_DEV_INFO>& VecDevInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_GN_NO_TAB, SSO_GN_TAB_CRE_FLD, buf_base) < 0)
	{
		sprintf(m_LogString, "��ȡSSO�������ʧ��");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_GN_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO��������¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "SSO�������[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_GN_TAB_CRE* pInfo = NULL;
	pInfo = new SSO_GN_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_GN_TAB_CRE));

	// ���Ѵ��ڱ�־
	int ii, jj;
	m_WTPara.nGenOld = nInfo;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < nInfo; jj++)
		{
			if (VecDevInfo[ii].dev_id == pInfo[jj].gn_id)
			{
				VecDevInfo[ii].is_old = 1;
				break;
			}
		}
	}

	SAFE_DELETE(pInfo);
#endif
	return 0;
}

int CWamToolObj::InitAclnInfo(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab)
{
	if (!VecDevInfo.empty())
	{
		vector<WT_DEV_INFO>().swap(VecDevInfo);
		VecDevInfo.clear();
	}
#ifdef _WAMAP_
	// �����߶α�
	vector<ACLINE_TAB_CRE> VecAclnTab;
	if (PODB_GetAclnInfo(VecAclnTab) < 0)
	{
		return false;
	}

	// �����߶ζ˵��
	vector<ACLNDOT_TAB_CRE> VecAclnDotTab;
	if (PODB_GetAclnDotInfo(VecAclnDotTab) < 0)
	{
		return false;
	}

	// ��佻����·��Ϣ
	int ii, jj, nfalg1, nflag2;
	WT_DEV_INFO tmpDevInfo;
	for (ii = 0; ii < (int)VecAclnTab.size(); ii++)
	{
		memset((void*)&tmpDevInfo, 0, sizeof(WT_DEV_INFO));
		tmpDevInfo.dev_id = VecAclnTab[ii].acln_id;
		tmpDevInfo.maxvlty_id = VecAclnTab[ii].vlty_id;
		tmpDevInfo.dot1.fac_id = VecAclnTab[ii].fac_id_s;
		tmpDevInfo.dot2.fac_id = VecAclnTab[ii].fac_id_e;
		sprintf(tmpDevInfo.dev_name, "%s", VecAclnTab[ii].acln_name);
		VecDevInfo.push_back(tmpDevInfo);
	}

	m_WTPara.nAclnAll = (int)VecDevInfo.size();

	// ��佻���߶ζ˵�
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nfalg1 = 0;
		nflag2 = 0;
		for (jj = 0; jj < (int)VecAclnDotTab.size(); jj++)
		{
			if (VecDevInfo[ii].dev_id == VecAclnDotTab[jj].acln_id)
			{
				if (VecDevInfo[ii].dot1.fac_id == VecAclnDotTab[jj].fac_id)
				{
					VecDevInfo[ii].dot1.dot_id = VecAclnDotTab[jj].acln_dot_id;
					sprintf(VecDevInfo[ii].dot1.dot_name, "%s", VecAclnDotTab[jj].acln_dot_name);
					nfalg1 = 1;
				}
				else if (VecDevInfo[ii].dot2.fac_id == VecAclnDotTab[jj].fac_id)
				{
					VecDevInfo[ii].dot2.dot_id = VecAclnDotTab[jj].acln_dot_id;
					sprintf(VecDevInfo[ii].dot2.dot_name, "%s", VecAclnDotTab[jj].acln_dot_name);
					nflag2 = 1;
				}
			}

			if ((nfalg1 == 1) && (nflag2 == 1))
			{
				break;
			}
		}
	}

	// ��䳧վ��Ϣ
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nfalg1 = 0;
		nflag2 = 0;
		for (jj = 0; jj < (int)VecFacTab.size(); jj++)
		{
			if (VecDevInfo[ii].dot1.fac_id == VecFacTab[jj].fac_id)
			{
				VecDevInfo[ii].dot1.area_id = VecFacTab[jj].area_id;
				sprintf(VecDevInfo[ii].dot1.fac_name, "%s", VecFacTab[jj].fac_name);
				nfalg1 = 1;
			}
			if (VecDevInfo[ii].dot2.fac_id == VecFacTab[jj].fac_id)
			{
				VecDevInfo[ii].dot2.area_id = VecFacTab[jj].area_id;
				sprintf(VecDevInfo[ii].dot2.fac_name, "%s", VecFacTab[jj].fac_name);
				nflag2 = 1;
			}
			if ((nfalg1 == 1) && (nflag2 == 1))
			{
				break;
			}
		}
	}

	// ���������Ϣ
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nfalg1 = 0;
		nflag2 = 0;
		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (VecDevInfo[ii].dot1.area_id == VecAreaTab[jj].area_id)
			{
				sprintf(VecDevInfo[ii].dot1.area_name, "%s", VecAreaTab[jj].area_name);
				nfalg1 = 1;
			}
			if (VecDevInfo[ii].dot2.area_id == VecFacTab[jj].area_id)
			{
				sprintf(VecDevInfo[ii].dot2.area_name, "%s", VecAreaTab[jj].area_name);
				nflag2 = 1;
			}
			if ((nfalg1 == 1) && (nflag2 == 1))
			{
				break;
			}
		}
	}

	// ����ѹ�ȼ�
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < (int)VecVLTypeTab.size(); jj++)
		{
			if (VecDevInfo[ii].maxvlty_id == VecVLTypeTab[jj].vlty_id)
			{
				VecDevInfo[ii].maxvbase = VecVLTypeTab[jj].vbase;
				VecDevInfo[ii].dot1.vbase = VecVLTypeTab[jj].vbase;
				VecDevInfo[ii].dot2.vbase = VecVLTypeTab[jj].vbase;
				break;
			}
		}
	}
#endif
	return 0;
}

int CWamToolObj::GetLfoAcln(vector<WT_DEV_INFO>& VecDevInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_ACLN_NO_TAB, SSO_LN_TAB_CRE_FLD, buf_base) < 0)
	{
		sprintf(m_LogString, "��ȡSSO������·��ʧ��");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_LN_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO������·��¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "SSO������·��[%d]", nInfo);
		MessShow(m_LogString);
	}

	SSO_LN_TAB_CRE* pInfo = NULL;
	pInfo = new SSO_LN_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_LN_TAB_CRE));

	// ���Ѵ��ڱ�־
	int ii, jj;
	m_WTPara.nAclnOld = nInfo;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < nInfo; jj++)
		{
			if (VecDevInfo[ii].dev_id == pInfo[jj].acln_id)
			{
				VecDevInfo[ii].is_old = 1;
				break;
			}
		}
	}

	SAFE_DELETE(pInfo);
#endif
	return 0;
}

int CWamToolObj::InitTrInfo(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab)
{
	if (!VecDevInfo.empty())
	{
		vector<WT_DEV_INFO>().swap(VecDevInfo);
		VecDevInfo.clear();
	}
#ifdef _WAMAP_
	// ��ѹ����
	vector<TR_TAB_CRE> VecTrTab;
	if (PODB_GetTrInfo(VecTrTab) < 0)
	{
		return -1;
	}

	// ��ѹ�������
	vector<TRWD_TAB_CRE> VecTrWdTab;
	if (PODB_GetTrwdInfo(VecTrWdTab) < 0)
	{
		return -1;
	}

	// ����ѹ����Ϣ
	int ii, jj;
	WT_DEV_INFO tmpDevInfo;
	for (ii = 0; ii < (int)VecTrTab.size(); ii++)
	{
		memset((void*)&tmpDevInfo, 0, sizeof(WT_DEV_INFO));
		tmpDevInfo.dev_id = VecTrTab[ii].tr_id;
		tmpDevInfo.fac_id = VecTrTab[ii].fac_id;
		tmpDevInfo.trty = VecTrTab[ii].trty;
		// 		tmpDevInfo.tr_type = VecTrTab[ii].tr_type;
		tmpDevInfo.dot1.fac_id = VecTrTab[ii].fac_id;
		tmpDevInfo.dot2.fac_id = VecTrTab[ii].fac_id;
		tmpDevInfo.dot3.fac_id = VecTrTab[ii].fac_id;
		sprintf(tmpDevInfo.dev_name, "%s", VecTrTab[ii].tr_name);
		VecDevInfo.push_back(tmpDevInfo);
	}

	m_WTPara.nTrAll = (int)VecDevInfo.size();

	// ���������Ϣ
	int nflag1, nflag2, nflag3;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nflag1 = 0;
		nflag2 = 0;
		nflag3 = 0;
		for (jj = 0; jj < (int)VecTrWdTab.size(); jj++)
		{
			if (VecDevInfo[ii].dev_id == VecTrWdTab[jj].tr_id)
			{
				if (VecTrWdTab[jj].trwd_type == HIGH_WINDING)
				{
					VecDevInfo[ii].dot1.dot_id = VecTrWdTab[jj].trwd_id;
					VecDevInfo[ii].dot1.vlty_id = VecTrWdTab[jj].vlty_id;
					sprintf(VecDevInfo[ii].dot1.dot_name, "%s", VecTrWdTab[jj].trwd_name);
					nflag1 = 1;
				}
				else if (VecTrWdTab[jj].trwd_type == MID_WINDING)
				{
					VecDevInfo[ii].dot3.dot_id = VecTrWdTab[jj].trwd_id;
					VecDevInfo[ii].dot3.vlty_id = VecTrWdTab[jj].vlty_id;
					sprintf(VecDevInfo[ii].dot3.dot_name, "%s", VecTrWdTab[jj].trwd_name);
					nflag3 = 1;
				}
				else if (VecTrWdTab[jj].trwd_type == LOW_WINDING)
				{
					VecDevInfo[ii].dot2.dot_id = VecTrWdTab[jj].trwd_id;
					VecDevInfo[ii].dot2.vlty_id = VecTrWdTab[jj].vlty_id;
					sprintf(VecDevInfo[ii].dot2.dot_name, "%s", VecTrWdTab[jj].trwd_name);
					nflag2 = 1;
				}

				if (VecDevInfo[ii].dev_type == THREE_WINDING)
				{
					if ((nflag1 == 1) && (nflag2 == 1) && (nflag3 == 1))
					{
						break;
					}
				}
				else if (VecDevInfo[ii].dev_type == TWO_WINDING)
				{
					if ((nflag1 == 1) && (nflag2 == 1))
					{
						break;
					}
				}
			}
		}
	}

	// ��䳧վ��Ϣ
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nflag1 = 0;
		for (jj = 0; jj < (int)VecFacTab.size(); jj++)
		{
			if (VecDevInfo[ii].fac_id == VecFacTab[jj].fac_id)
			{
				nflag1 = 1;
				VecDevInfo[ii].area_id = VecFacTab[jj].area_id;
				VecDevInfo[ii].dot1.area_id = VecFacTab[jj].area_id;
				VecDevInfo[ii].dot2.area_id = VecFacTab[jj].area_id;
				VecDevInfo[ii].dot3.area_id = VecFacTab[jj].area_id;
				sprintf(VecDevInfo[ii].fac_name, "%s", VecFacTab[jj].fac_name);
				sprintf(VecDevInfo[ii].dot1.fac_name, "%s", VecFacTab[jj].fac_name);
				sprintf(VecDevInfo[ii].dot2.fac_name, "%s", VecFacTab[jj].fac_name);
				sprintf(VecDevInfo[ii].dot3.fac_name, "%s", VecFacTab[jj].fac_name);

				break;
			}
		}
		if (nflag1 == 0)
		{
			sprintf(m_LogString, " δ�ҵ���Ӧ��վ["INT_REC_FMT"][%s],["INT_REC_FMT"]", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].fac_id));
			MessShow(m_LogString, LTDEBUG);
		}
	}

	// ���������Ϣ
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (VecDevInfo[ii].area_id == VecAreaTab[jj].area_id)
			{
				sprintf(VecDevInfo[ii].area_name, "%s", VecAreaTab[jj].area_name);
				sprintf(VecDevInfo[ii].dot1.area_name, "%s", VecAreaTab[jj].area_name);
				sprintf(VecDevInfo[ii].dot2.area_name, "%s", VecAreaTab[jj].area_name);
				sprintf(VecDevInfo[ii].dot3.area_name, "%s", VecAreaTab[jj].area_name);
				break;
			}
		}
	}

	// ����ѹ�ȼ�
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nflag1 = 0;
		nflag2 = 0;
		nflag3 = 0;
		for (jj = 0; jj < (int)VecVLTypeTab.size(); jj++)
		{
			if (VecDevInfo[ii].dot1.vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				VecDevInfo[ii].dot1.vbase = VecVLTypeTab[jj].vbase;
				nflag1 = 1;;
			}
			else if (VecDevInfo[ii].dot2.vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				VecDevInfo[ii].dot2.vbase = VecVLTypeTab[jj].vbase;
				nflag2 = 1;;
			}
			else if (VecDevInfo[ii].dot3.vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				VecDevInfo[ii].dot3.vbase = VecVLTypeTab[jj].vbase;
				nflag3 = 1;;
			}

			if (VecDevInfo[ii].dev_type == THREE_WINDING)
			{
				if ((nflag1 == 1) && (nflag2 == 1) && (nflag3 == 1))
				{
					break;
				}
			}
			else if (VecDevInfo[ii].dev_type == TWO_WINDING)
			{
				if ((nflag1 == 1) && (nflag2 == 1))
				{
					break;
				}
			}
		}
	}

	// ��ȡ��ߵ�ѹ�ȼ�
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		VecDevInfo[ii].maxvbase = 0.0f;
		if (VecDevInfo[ii].dot1.vbase > VecDevInfo[ii].maxvbase)
		{
			VecDevInfo[ii].maxvbase = VecDevInfo[ii].dot1.vbase;
			VecDevInfo[ii].maxvlty_id = VecDevInfo[ii].dot1.vlty_id;
		}
		if (VecDevInfo[ii].dot2.vbase > VecDevInfo[ii].maxvbase)
		{
			VecDevInfo[ii].maxvbase = VecDevInfo[ii].dot2.vbase;
			VecDevInfo[ii].maxvlty_id = VecDevInfo[ii].dot2.vlty_id;
		}
		if (VecDevInfo[ii].dot3.vbase > VecDevInfo[ii].maxvbase)
		{
			VecDevInfo[ii].maxvbase = VecDevInfo[ii].dot3.vbase;
			VecDevInfo[ii].maxvlty_id = VecDevInfo[ii].dot3.vlty_id;
		}
	}

#endif
	return 0;
}

int CWamToolObj::InitVFDevInfo(vector<WT_DEV_INFO>& VecDevInfo)
{
	if (!VecDevInfo.empty())
	{
		vector<WT_DEV_INFO>().swap(VecDevInfo);
		VecDevInfo.clear();
	}

	// ��װ��PMU���ĳ�վ���������������·�ͱ�ѹ����Ϣ��䵽VF�����豸��Ϣ�ṹ����
	int ii;
	for (ii = 0; ii < (int)m_VecFacInfo.size(); ii++)
	{
		if (m_VecFacInfo[ii].yc_flag == 1)
		{
			VecDevInfo.push_back(m_VecFacInfo[ii]);
		}
	}

	for (ii = 0; ii < (int)m_VecGenInfo.size(); ii++)
	{
		if (m_VecGenInfo[ii].yc_flag == 1)
		{
			VecDevInfo.push_back(m_VecGenInfo[ii]);
		}
	}

	for (ii = 0; ii < (int)m_VecAclnInfo.size(); ii++)
	{
		if (m_VecAclnInfo[ii].yc_flag >= 1)
		{
			VecDevInfo.push_back(m_VecAclnInfo[ii]);
		}
	}

	for (ii = 0; ii < (int)m_VecTrInfo.size(); ii++)
	{
		if (m_VecTrInfo[ii].yc_flag >= 1)
		{
			VecDevInfo.push_back(m_VecTrInfo[ii]);
		}
	}

	m_WTPara.nVFAll = m_WTPara.nFacAll + m_WTPara.nGenAll + m_WTPara.nAclnAll + m_WTPara.nTrAll;
	m_WTPara.nVFPmu = m_WTPara.nFacPmu + m_WTPara.nGenPmu + m_WTPara.nAclnPmu + m_WTPara.nTrPmu;
	sprintf(m_LogString, "VF�����豸:��վ[%d],�����[%d],������·[%d],��ѹ��[%d],����[%d]",
		m_WTPara.nFacPmu, m_WTPara.nGenPmu, m_WTPara.nAclnPmu, m_WTPara.nTrPmu, m_WTPara.nVFPmu);
	MessShow(m_LogString, LTINFO);

	return 0;
}

int CWamToolObj::GetLfoTr(vector<WT_DEV_INFO>& VecDevInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(SSO_TR_NO_TAB, SSO_TR_TAB_CRE_FLD, buf_base) < 0)
	{
		sprintf(m_LogString, "��ȡSSO��ѹ����ʧ��");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(SSO_TR_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "SSO��ѹ����¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "SSO��ѹ����[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	SSO_TR_TAB_CRE* pInfo = NULL;
	pInfo = new SSO_TR_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(SSO_TR_TAB_CRE));

	// ���Ѵ��ڱ�־
	int ii, jj;
	m_WTPara.nTrOld = nInfo;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < nInfo; jj++)
		{
			if (VecDevInfo[ii].dev_id == pInfo[jj].tr_id)
			{
				VecDevInfo[ii].is_old = 1;
				break;
			}
		}
	}

	SAFE_DELETE(pInfo);
#endif
	return 0;
}

int CWamToolObj::GetFacName(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab)
{
	// ��䳧վ����
	int ii, jj, nFlag;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nFlag = 0;
		for (jj = 0; jj < (int)VecFacTab.size(); jj++)
		{
			if (VecDevInfo[ii].fac_id == VecFacTab[jj].fac_id)
			{
				nFlag = 1;
				VecDevInfo[ii].area_id = VecFacTab[jj].area_id;
				sprintf(VecDevInfo[ii].fac_name, "%s", VecFacTab[jj].fac_name);
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "["INT_REC_FMT"][%s]δ�ҵ���Ӧ��վ["INT_REC_FMT"]", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].fac_id));
			MessShow(m_LogString, LTERROR);
		}
	}
	return 0;
}

int CWamToolObj::GetAreaName(vector<WT_DEV_INFO>& VecDevInfo, vector<AREA_TAB_CRE>& VecAreaTab)
{
	// ������������
	int ii, jj, nFlag;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nFlag = 0;
		for (jj = 0; jj < (int)VecAreaTab.size(); jj++)
		{
			if (VecDevInfo[ii].area_id == VecAreaTab[jj].area_id)
			{
				nFlag = 1;
				sprintf(VecDevInfo[ii].area_name, "%s", VecAreaTab[jj].area_name);
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "["INT_REC_FMT"][%s]δ�ҵ���Ӧ����["INT_REC_FMT"]", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].area_id));
			MessShow(m_LogString, LTERROR);
		}
	}

	return 0;
}

int CWamToolObj::GetVBase(vector<WT_DEV_INFO>& VecDevInfo, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab)
{
	// ����ѹ�ȼ�
	int ii, jj, nFlag;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nFlag = 0;
		for (jj = 0; jj < (int)VecVLTypeTab.size(); jj++)
		{
			if (VecDevInfo[ii].maxvlty_id == VecVLTypeTab[jj].vlty_id)
			{
				nFlag = 1;
				VecDevInfo[ii].maxvbase = VecVLTypeTab[jj].vbase;
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "["INT_REC_FMT"][%s]δ�ҵ���Ӧ��ѹ�ȼ�["VOL_INT_REC_FMT"]", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name, VecDevInfo[ii].maxvlty_id);
			MessShow(m_LogString, LTERROR);
		}
	}
	return 0;
}

int CWamToolObj::SetYcFlag(vector<WT_DEV_INFO>& VecDevInfo, vector<FES_YX_YC>& VecFesYcTab, const int tableno)
{
	int ii, jj, kk, nNum;
	INT_REC tmprecord_id;
	int tmpcolumn_id;
	pair<multimap<INT_REC, int>::iterator, multimap<INT_REC, int>::iterator> range;
	multimap<INT_REC, int>::iterator it;
	if (tableno == SSO_FAC_NO_TAB)
	{
		// ���WAMSǰ��ң����Ϣ
		// UAV UAA UBV UBA UCV UCA V F
#ifdef _WAMAP_
		if (PODB_InitColID(m_VecColBus, BS_DEVICE_NO_TAB) < 0)
		{
			return false;
		}
#endif
		m_WTPara.nFacPmu = 0;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_NO;
			VecDevInfo[ii].dot2.IsYcDefine = WT_MENU_NO;
			VecDevInfo[ii].dot3.IsYcDefine = WT_MENU_NO;
			for (jj = 0; jj < VEC_VIP_NUM; jj++)
			{
				VecDevInfo[ii].dot1.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot1.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot1.nYcColNo[jj] = -1;
				VecDevInfo[ii].dot2.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot2.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot2.nYcColNo[jj] = -1;
				VecDevInfo[ii].dot3.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot3.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot3.nYcColNo[jj] = -1;
			}
			if (!VALID_INT_REC(VecDevInfo[ii].dot1.dot_id))
			{
				sprintf(m_LogString, ""INT_REC_FMT",%s��Ϣ��ȫ", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}

			range = g_map_colno.equal_range(VecDevInfo[ii].dot1.dot_id);

			for (it = range.first; it != range.second; ++it)
			{
				if (VecFesYcTab[it->second].file_no < 0 || VecFesYcTab[it->second].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[it->second].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[it->second].yc_id);
				if (VecDevInfo[ii].dot1.dot_id == tmprecord_id)
				{
					// UAV UAA UBV UBA UCV UCA
					for (kk = 0; kk < VEC_V_NUM; kk++)
					{
						if (tmpcolumn_id == m_VecColBus[kk])
						{
							VecDevInfo[ii].dot1.nYcFlag[kk] = WT_MENU_YES;
							VecDevInfo[ii].dot1.nYcFilNo[kk] = VecFesYcTab[it->second].file_no;
							VecDevInfo[ii].dot1.nYcColNo[kk] = VecFesYcTab[it->second].col_no;
						}
					}
					// F
//					if (tmpcolumn_id == m_VecColBus[7])
//					{
//						VecDevInfo[ii].dot1.nYcFlag[7] = WT_MENU_YES;
//						VecDevInfo[ii].dot1.nYcFilNo[7] = VecFesYcTab[jj].file_no;
//						VecDevInfo[ii].dot1.nYcColNo[7] = VecFesYcTab[jj].col_no;
//					}
				}

				if (PODB_IsFlagAll(VecDevInfo[ii].dot1.nYcFlag, VEC_V_NUM) == VEC_V_NUM)
				{
					VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_YES;
					break;
				}
			}
			if (VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_NO)
			{
				sprintf(m_LogString, "δ�ҵ���Ӧ�����Ϣ������Ϣ��ȫ["INT_REC_FMT"][%s]", INT_REC_VAL(VecDevInfo[ii].dot1.dot_id), VecDevInfo[ii].dot1.dot_name);
				MessShow(m_LogString, LTERROR);
			}
			else
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 1;
				m_WTPara.nFacPmu++;
			}
		}
	}
	else if (tableno == SSO_GN_NO_TAB)
	{
		// ���WAMSǰ��ң����Ϣ
		// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA F
#ifdef _WAMAP_
		if (PODB_InitColID(m_VecColGen, GN_DEVICE_NO_TAB) < 0)
		{
			return false;
		}
#endif
		m_WTPara.nGenPmu = 0;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_NO;
			VecDevInfo[ii].dot2.IsYcDefine = WT_MENU_NO;
			VecDevInfo[ii].dot3.IsYcDefine = WT_MENU_NO;
			for (jj = 0; jj < VEC_SSO_NUM; jj++)
			{
				VecDevInfo[ii].dot1.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot1.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot1.nYcColNo[jj] = -1;
				VecDevInfo[ii].dot2.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot2.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot2.nYcColNo[jj] = -1;
				VecDevInfo[ii].dot3.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot3.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot3.nYcColNo[jj] = -1;
			}

			if (!VALID_INT_REC(VecDevInfo[ii].dot1.dot_id))
			{
				sprintf(m_LogString, ""INT_REC_FMT",%s��Ϣ��ȫ", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}

			range = g_map_colno.equal_range(VecDevInfo[ii].dot1.dot_id);

			for (it = range.first; it != range.second; ++it)
			{
				if (VecFesYcTab[it->second].file_no < 0 || VecFesYcTab[it->second].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[it->second].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[it->second].yc_id);
				if (VecDevInfo[ii].dot1.dot_id == tmprecord_id)
				{
					// SSO SSF SSP  PWR 
					for (kk = 0; kk < VEC_SSO_NUM; kk++)
					{
						if (tmpcolumn_id == m_VecColGen[kk])
						{
							VecDevInfo[ii].dot1.nYcFlag[kk] = WT_MENU_YES;
							VecDevInfo[ii].dot1.nYcFilNo[kk] = VecFesYcTab[it->second].file_no;
							VecDevInfo[ii].dot1.nYcColNo[kk] = VecFesYcTab[it->second].col_no;
						}
					}
				}
				nNum = PODB_IsFlagAll(VecDevInfo[ii].dot1.nYcFlag, VEC_SSO_NUM);
				if (nNum >= VEC_SSO_NUM)
				{
					VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_YES;
				}
				if (nNum == VEC_SSO_NUM)
				{
					break;
				}
			}
			if (VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_NO)
			{
				sprintf(m_LogString, "δ�ҵ���Ӧ�����Ϣ������Ϣ��ȫ["INT_REC_FMT"][%s]", INT_REC_VAL(VecDevInfo[ii].dot1.dot_id), VecDevInfo[ii].dot1.dot_name);
				MessShow(m_LogString, LTERROR);
			}
			else
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 1;
				m_WTPara.nGenPmu++;
			}
		}
	}
	else if (tableno == SSO_ACLN_NO_TAB)
	{
		// ���WAMSǰ��ң����Ϣ
		// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR F
#ifdef _WAMAP_
		if (PODB_InitColID(m_VecColLn, ACLN_DOT_NO_TAB) < 0)
		{
			return false;
		}
#endif
		m_WTPara.nAclnPmu = 0;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			if (VecDevInfo[ii].maxvbase < m_WTPara.VBase)
			{
				continue;
			}
			VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_NO;
			VecDevInfo[ii].dot2.IsYcDefine = WT_MENU_NO;
			VecDevInfo[ii].dot3.IsYcDefine = WT_MENU_NO;
			for (jj = 0; jj < VEC_SSO_NUM; jj++)
			{
				VecDevInfo[ii].dot1.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot1.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot1.nYcColNo[jj] = -1;
				VecDevInfo[ii].dot2.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot2.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot2.nYcColNo[jj] = -1;
				VecDevInfo[ii].dot3.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot3.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot3.nYcColNo[jj] = -1;
			}

			if (!VALID_INT_REC(VecDevInfo[ii].dot1.dot_id) && !VALID_INT_REC(VecDevInfo[ii].dot2.dot_id))
			{
				sprintf(m_LogString, ""INT_REC_FMT",%s��Ϣ��ȫ", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}

			range = g_map_colno.equal_range(VecDevInfo[ii].dot1.dot_id);

			for (it = range.first; it != range.second; ++it)
			{
				if (VecFesYcTab[it->second].file_no < 0 || VecFesYcTab[it->second].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[it->second].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[it->second].yc_id);
				if (VecDevInfo[ii].dot1.dot_id == tmprecord_id)
				{
					// SSO SSP SSF PWR
					for (kk = 0; kk < VEC_SSO_NUM; kk++)
					{
						if (tmpcolumn_id == m_VecColLn[kk])
						{
							VecDevInfo[ii].dot1.nYcFlag[kk] = WT_MENU_YES;
							VecDevInfo[ii].dot1.nYcFilNo[kk] = VecFesYcTab[it->second].file_no;
							VecDevInfo[ii].dot1.nYcColNo[kk] = VecFesYcTab[it->second].col_no;
						}
					}
				}
				if (PODB_IsFlagAll(VecDevInfo[ii].dot1.nYcFlag, VEC_SSO_NUM) == VEC_SSO_NUM)
				{
					VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_YES;
					break;
				}
			}

			range = g_map_colno.equal_range(VecDevInfo[ii].dot2.dot_id);

			for (it = range.first; it != range.second; ++it)
			{
				if (VecFesYcTab[it->second].file_no < 0 || VecFesYcTab[it->second].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[it->second].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[it->second].yc_id);
				if (VecDevInfo[ii].dot2.dot_id == tmprecord_id)
				{
					// SSO SSP SSF PWR
					for (kk = 0; kk < VEC_SSO_NUM; kk++)
					{
						if (tmpcolumn_id == m_VecColLn[kk])
						{
							VecDevInfo[ii].dot2.nYcFlag[kk] = WT_MENU_YES;
							VecDevInfo[ii].dot2.nYcFilNo[kk] = VecFesYcTab[it->second].file_no;
							VecDevInfo[ii].dot2.nYcColNo[kk] = VecFesYcTab[it->second].col_no;
						}
					}
				}
				if (VecDevInfo[ii].dot2.dot_id == tmprecord_id)
				{
					//  SSO SSP SSF PWR
					for (kk = 0; kk < VEC_SSO_NUM; kk++)
					{
						if (tmpcolumn_id == m_VecColLn[kk])
						{
							VecDevInfo[ii].dot2.nYcFlag[kk] = WT_MENU_YES;
							VecDevInfo[ii].dot2.nYcFilNo[kk] = VecFesYcTab[it->second].file_no;
							VecDevInfo[ii].dot2.nYcColNo[kk] = VecFesYcTab[it->second].col_no;
						}
					}
				}
				if (PODB_IsFlagAll(VecDevInfo[ii].dot2.nYcFlag, VEC_SSO_NUM) == VEC_SSO_NUM)
				{
					VecDevInfo[ii].dot2.IsYcDefine = WT_MENU_YES;
					break;
				}
			}
			if ((VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_YES) && (VecDevInfo[ii].dot2.IsYcDefine == WT_MENU_YES))
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 2;
				m_WTPara.nAclnPmu++;
			}
			else if (VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_YES)
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 1;
				m_WTPara.nAclnPmu++;
			}
			else if (VecDevInfo[ii].dot2.IsYcDefine == WT_MENU_YES)
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 1;
				m_WTPara.nAclnPmu++;
			}
			else
			{
				sprintf(m_LogString, "δ�ҵ���Ӧ�����Ϣ������Ϣ��ȫ["INT_REC_FMT"][%s]", INT_REC_VAL(VecDevInfo[ii].dot1.dot_id), VecDevInfo[ii].dot1.dot_name);
				MessShow(m_LogString, LTERROR);
			}
		}
	}
	else if (tableno == SSO_TR_NO_TAB)
	{
		// ���WAMSǰ��ң����Ϣ
		// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR F
#ifdef _WAMAP_
		if (PODB_InitColID(m_VecColTr, TRWD_DEVICE_NO_TAB) < 0)
		{
			return false;
		}
#endif
		m_WTPara.nTrPmu = 0;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			if (VecDevInfo[ii].maxvbase < m_WTPara.VBase)
			{
				continue;
			}
			VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_NO;
			VecDevInfo[ii].dot2.IsYcDefine = WT_MENU_NO;
			VecDevInfo[ii].dot3.IsYcDefine = WT_MENU_NO;
			for (jj = 0; jj < VEC_SSO_NUM; jj++)
			{
				VecDevInfo[ii].dot1.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot1.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot1.nYcColNo[jj] = -1;
				VecDevInfo[ii].dot2.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot2.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot2.nYcColNo[jj] = -1;
				VecDevInfo[ii].dot3.nYcFlag[jj] = WT_MENU_NO;
				VecDevInfo[ii].dot3.nYcFilNo[jj] = -1;
				VecDevInfo[ii].dot3.nYcColNo[jj] = -1;
			}

			if (!VALID_INT_REC(VecDevInfo[ii].dot1.dot_id))
			{
				sprintf(m_LogString, ""INT_REC_FMT",%s��Ϣ��ȫ", INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}

			range = g_map_colno.equal_range(VecDevInfo[ii].dot1.dot_id);
			for (it = range.first; it != range.second; ++it)
			{
				if (VecFesYcTab[it->second].file_no < 0 || VecFesYcTab[it->second].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[it->second].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[it->second].yc_id);
				if (VecDevInfo[ii].dot1.dot_id == tmprecord_id)
				{
					// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR
					for (kk = 0; kk < VEC_SSO_NUM; kk++)
					{
						if (tmpcolumn_id == m_VecColTr[kk])
						{
							VecDevInfo[ii].dot1.nYcFlag[kk] = WT_MENU_YES;
							VecDevInfo[ii].dot1.nYcFilNo[kk] = VecFesYcTab[it->second].file_no;
							VecDevInfo[ii].dot1.nYcColNo[kk] = VecFesYcTab[it->second].col_no;
						}
					}
				}
				if (PODB_IsFlagAll(VecDevInfo[ii].dot1.nYcFlag, VEC_SSO_NUM) == VEC_SSO_NUM)
				{
					VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_YES;
					break;
				}
			}

			range = g_map_colno.equal_range(VecDevInfo[ii].dot2.dot_id);
			for (it = range.first; it != range.second; ++it)
			{
				if (VecFesYcTab[it->second].file_no < 0 || VecFesYcTab[it->second].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[it->second].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[it->second].yc_id);
				if (VecDevInfo[ii].dot2.dot_id == tmprecord_id)
				{
					// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR
					for (kk = 0; kk < VEC_SSO_NUM; kk++)
					{
						if (tmpcolumn_id == m_VecColTr[kk])
						{
							VecDevInfo[ii].dot2.nYcFlag[kk] = WT_MENU_YES;
							VecDevInfo[ii].dot2.nYcFilNo[kk] = VecFesYcTab[it->second].file_no;
							VecDevInfo[ii].dot2.nYcColNo[kk] = VecFesYcTab[it->second].col_no;
						}
					}
				}
				if (PODB_IsFlagAll(VecDevInfo[ii].dot2.nYcFlag, VEC_SSO_NUM) == VEC_SSO_NUM)
				{
					VecDevInfo[ii].dot2.IsYcDefine = WT_MENU_YES;
					break;
				}
			}

			range = g_map_colno.equal_range(VecDevInfo[ii].dot3.dot_id);
			for (it = range.first; it != range.second; ++it)
			{
				if (VecFesYcTab[it->second].file_no < 0 || VecFesYcTab[it->second].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[it->second].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[it->second].yc_id);
				if (VecDevInfo[ii].dot3.dot_id == tmprecord_id)
				{
					// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR
					for (kk = 0; kk < VEC_SSO_NUM; kk++)
					{
						if (tmpcolumn_id == m_VecColTr[kk])
						{
							VecDevInfo[ii].dot3.nYcFlag[kk] = WT_MENU_YES;
							VecDevInfo[ii].dot3.nYcFilNo[kk] = VecFesYcTab[it->second].file_no;
							VecDevInfo[ii].dot3.nYcColNo[kk] = VecFesYcTab[it->second].col_no;
						}
					}
				}
				if (PODB_IsFlagAll(VecDevInfo[ii].dot3.nYcFlag, VEC_SSO_NUM) == VEC_SSO_NUM)
				{
					VecDevInfo[ii].dot3.IsYcDefine = WT_MENU_YES;
					break;
				}
			}

			if ((VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_YES) && (VecDevInfo[ii].dot2.IsYcDefine == WT_MENU_YES) && (VecDevInfo[ii].dot3.IsYcDefine == WT_MENU_YES))
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 6;
				m_WTPara.nTrPmu++;
			}
			else if ((VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_YES) && (VecDevInfo[ii].dot2.IsYcDefine == WT_MENU_YES))
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 3;
				m_WTPara.nTrPmu++;
			}
			else if (VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_YES)
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 1;
				m_WTPara.nTrPmu++;
			}
			else if (VecDevInfo[ii].dot2.IsYcDefine == WT_MENU_YES)
			{
				// �ò���־
				VecDevInfo[ii].yc_flag = 1;
				m_WTPara.nTrPmu++;
			}
			else
			{
				sprintf(m_LogString, "δ�ҵ���Ӧ�����Ϣ������Ϣ��ȫ["INT_REC_FMT"][%s]", INT_REC_VAL(VecDevInfo[ii].dot1.dot_id), VecDevInfo[ii].dot1.dot_name);
				MessShow(m_LogString, LTERROR);
			}
		}
	}
	// 	else if (tableno == PQ_DA_BUS_NO_TAB)
	// 	{
	// 		// ���WAMSǰ��ң����Ϣ
	// 		// UAV UAA UBV UBA UCV UCA V F
	// 		if(PODB_InitColID(m_VecColBus,BS_DEVICE_NO_TAB)<0)
	// 		{
	// 			return false;
	// 		}
	// 		m_WTPara.nBusPmu = 0;
	// 		for( ii=0; ii<(int)VecDevInfo.size(); ii++ )
	// 		{
	// 			VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_NO;
	// 			VecDevInfo[ii].dot2.IsYcDefine = WT_MENU_NO;
	// 			VecDevInfo[ii].dot3.IsYcDefine = WT_MENU_NO;
	// 			for( jj=0; jj<VEC_VIP_NUM; jj++)
	// 			{
	// 				VecDevInfo[ii].dot1.nYcFlag[jj]  = WT_MENU_NO;
	// 				VecDevInfo[ii].dot1.nYcFilNo[jj] = -1;
	// 				VecDevInfo[ii].dot1.nYcColNo[jj] = -1;
	// 				VecDevInfo[ii].dot2.nYcFlag[jj]  = WT_MENU_NO;
	// 				VecDevInfo[ii].dot2.nYcFilNo[jj] = -1;
	// 				VecDevInfo[ii].dot2.nYcColNo[jj] = -1;
	// 				VecDevInfo[ii].dot3.nYcFlag[jj]  = WT_MENU_NO;
	// 				VecDevInfo[ii].dot3.nYcFilNo[jj] = -1;
	// 				VecDevInfo[ii].dot3.nYcColNo[jj] = -1;
	// 			}
	// 
	// 			if (VecDevInfo[ii].dot1.dot_id<=0)
	// 			{
	// 				sprintf(m_LogString,""INT_REC_FMT",%s��Ϣ��ȫ",VecDevInfo[ii].dev_id,VecDevInfo[ii].dev_name);
	// 				MessShow(m_LogString,LTINFO);
	// 				continue;
	// 			}
	// 
	// 			for( jj=0; jj<(int)VecFesYcTab.size(); jj++ )
	// 			{
	// 				if( VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0 )
	// 				{
	// 					continue;
	// 				}
	// 				tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
	// 				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
	// 				if( VecDevInfo[ii].dot1.dot_id == tmprecord_id )
	// 				{
	// 					// UAV UAA UBV UBA UCV UCA V
	// 					for (kk =0; kk<7; kk++)
	// 					{
	// 						if( tmpcolumn_id == m_VecColBus[kk] )
	// 						{
	// 							VecDevInfo[ii].dot1.nYcFlag[kk] = WT_MENU_YES;
	// 							VecDevInfo[ii].dot1.nYcFilNo[kk]= VecFesYcTab[jj].file_no;
	// 							VecDevInfo[ii].dot1.nYcColNo[kk]= VecFesYcTab[jj].col_no;
	// 						}
	// 					}
	// 				}
	// 				if (PODB_IsFlagAll(VecDevInfo[ii].dot1.nYcFlag,7)==7)
	// 				{
	// 					VecDevInfo[ii].dot1.IsYcDefine = WT_MENU_YES;
	// 					break;
	// 				}
	// 			}
	// 			if ((VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_YES) &&
	// 				(VecDevInfo[ii].freq_yc_id > 0))
	// 			{
	// 				// �ò���־
	// 				VecDevInfo[ii].yc_flag = 1;
	// 				m_WTPara.nBusPmu ++;
	// 			}
	// 			else
	// 			{
	// 				sprintf(m_LogString,"δ�ҵ���Ӧ�����Ϣ������Ϣ��ȫ["INT_REC_FMT"][%s]",VecDevInfo[ii].dot1.dot_id,VecDevInfo[ii].dot1.dot_name);
	// 				MessShow(m_LogString,LTERROR);
	// 			}
	// 		}
	// 	}
	else
	{
		sprintf(m_LogString, "�豸���ʹ���[%d]", tableno);
		MessShow(m_LogString, LTERROR);
	}
	return 0;
}

int CWamToolObj::SetAddFlag(vector<WT_DEV_INFO>& VecDevInfo, const int tableno)
{
	// �õ�ѹ��־
	int ii;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		// ������ͳ�վ���е�ѹ�ȼ�
		if (VecDevInfo[ii].dev_type == TOOL_DEV_TYPE_GEN || VecDevInfo[ii].dev_type == MENU_LFO_DEV_FAC)
		{
			VecDevInfo[ii].vflag = 1;
		}
		else
		{
			if (VecDevInfo[ii].maxvbase >= m_WTPara.VBase)
			{
				VecDevInfo[ii].vflag = 1;
			}
		}
	}

	// ���Ƿ����ӱ�־
	int devnum = 0;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		// 		sprintf(m_LogString,"��yjj��SetAddFlag���豸["INT_REC_FMT"]V[%f]is_old=[%d],vflag=[%d],yc_flag=[%d]",VecDevInfo[ii].dev_id,VecDevInfo[ii].maxvbase,VecDevInfo[ii].is_old,VecDevInfo[ii].vflag,VecDevInfo[ii].yc_flag);
		// 		MessShow(m_LogString,LTERROR);
		if (m_WTPara.nMode == WT_MODE_NEW || m_WTPara.nMode == WT_MODE_TEST)
		{
			if ((VecDevInfo[ii].is_old == 0) && (VecDevInfo[ii].vflag >= 1) && (VecDevInfo[ii].yc_flag >= 1))
			{
				VecDevInfo[ii].is_add = 1;
				devnum++;
			}
		}
		else if (m_WTPara.nMode == WT_MODE_ALL)
		{
			if ((VecDevInfo[ii].vflag >= 1) && (VecDevInfo[ii].yc_flag >= 1))
			{
				VecDevInfo[ii].is_add = 1;
				devnum++;
			}
		}
	}

	if (tableno == SSO_FAC_NO_TAB)
	{
		m_WTPara.nFacNew = devnum;
	}
	else if (tableno == SSO_GN_NO_TAB)
	{
		m_WTPara.nGenNew = devnum;
	}
	else if (tableno == SSO_ACLN_NO_TAB)
	{
		m_WTPara.nAclnNew = devnum;
	}
	else if (tableno == SSO_TR_NO_TAB)
	{
		m_WTPara.nTrNew = devnum;
	}
	else if (tableno == PQ_DA_BUS_NO_TAB)
	{
		m_WTPara.nVFNew = devnum;
	}
	else if (tableno == WAM_AM_DEV_NO_TAB)
	{
		m_WTPara.nAMNew = devnum;
	}
	else
	{
		sprintf(m_LogString, "�豸���ʹ���[%d]", tableno);
		MessShow(m_LogString, LTERROR);
	}
	return 0;
}

int CWamToolObj::WriteDevNew(vector<WT_DEV_INFO>& VecDevInfo, const int tableno)
{
	if (VecDevInfo.empty())
	{
		return 0;
	}

	int ii = 0;
	int add_num = 0;
	INT_REC tmpIdx;
	vector<string> VecSql;
	string tablename;
	char SqlBuffer[4096];
	memset(SqlBuffer, 0, sizeof(SqlBuffer));
	if (tableno == SSO_FAC_NO_TAB)
	{
		MessShow("����վ");
		if (m_WTPara.nMode <= WT_MODE_NEW)
		{
			// ����
			//tmpIdx = PODB_GetDevIdByKeyID(tableno, m_WTPara.nFacOld + 1);			
			if (VALID_INT_REC(m_WTPara.MAX_FacOld_idx))
			{
				tmpIdx = m_WTPara.MAX_FacOld_idx;		//lns  Ӧ���Ǳ�������¼����idx + 1
				IncrRtdbId(tmpIdx);
			}
			else
			{
				tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
			}
		}
		else
		{
			tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
		}

		vector<SSO_FC_TAB_CRE> VecLfoFacW;
		SSO_FC_TAB_CRE tmpLfoFacW;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			if (VecDevInfo[ii].is_add == WT_MENU_NO)
			{
				continue;
			}
			if (VecDevInfo[ii].yc_flag != 1)
			{
				continue;
			}

			memset((void*)&tmpLfoFacW, 0, sizeof(SSO_FC_TAB_CRE));
#ifdef _NUSP
			tmpLfoFacW.fac_idx = PODB_ChangeDevId(SSO_FAC_NO_TAB, VecDevInfo[ii].dev_id);
#endif // _NUSP
			tmpLfoFacW.station_id = VecDevInfo[ii].dev_id;
			tmpLfoFacW.area_id = VecDevInfo[ii].area_id;
			tmpLfoFacW.fac_type = VecDevInfo[ii].dev_type;
			tmpLfoFacW.vlty_id = VecDevInfo[ii].maxvlty_id;
			tmpLfoFacW.is_monitor = WT_MENU_YES;
			tmpLfoFacW.is_part_ana = WT_MENU_YES;
			// 			tmpLfoFacW.is_set_bus = WT_MENU_NO;
			// 			tmpLfoFacW.set_bus_id = VecDevInfo[ii].dot1.dot_id;
			// 			tmpLfoFacW.clus = MENU_LFO_CLUST_RMAIN;
			tmpLfoFacW.is_m_bs = MENU_LFO_MENU_YES;

			VecLfoFacW.push_back(tmpLfoFacW);
		}
		add_num = (int)VecLfoFacW.size();
		if (add_num <= 0) return 0;

		for (ii = 0; ii < (int)VecLfoFacW.size(); ii++)
		{
			sprintf(m_LogString, "������վ%d:"INT_REC_FMT"", ii + 1, INT_REC_VAL(VecLfoFacW[ii].station_id));
			MessShow(m_LogString, LTDEBUG);
		}

		// д���ÿ�
		tablename = "sso_fac";
		for (ii = 0; ii < (int)VecLfoFacW.size(); ii++)
		{
			sprintf(SqlBuffer, ""SSO_FC_TAB_CRE_FMT"",
				tablename.c_str(),
				SSO_FC_TAB_CRE_FLD,
				SSO_FC_TAB_CRE_VAL(VecLfoFacW[ii]));
			VecSql.push_back(SqlBuffer);
		}
#ifdef _NUSP
		SSO_FAC_WAMS* add_fac_info = NULL;
		add_fac_info = new SSO_FAC_WAMS[add_num];
		memset((void*)add_fac_info, 0, add_num * sizeof(SSO_FAC_WAMS));
		for (ii = 0; ii < add_num; ii++)
		{
			add_fac_info[ii].fac_idx = PODB_ChangeDevId(SSO_FAC_NO_TAB, VecLfoFacW[add_num - 1 - ii].station_id);
			add_fac_info[ii].station_id = VecLfoFacW[add_num - 1 - ii].station_id;
			add_fac_info[ii].area_id = VecLfoFacW[add_num - 1 - ii].area_id;
			add_fac_info[ii].fac_type = VecLfoFacW[add_num - 1 - ii].fac_type;
			add_fac_info[ii].vlty_id = VecLfoFacW[add_num - 1 - ii].vlty_id;
			add_fac_info[ii].is_monitor = VecLfoFacW[add_num - 1 - ii].is_monitor;
			add_fac_info[ii].is_part_ana = VecLfoFacW[add_num - 1 - ii].is_part_ana;
			// 			add_fac_info[ii].is_set_bus = VecLfoFacW[add_num - 1 - ii].is_set_bus;
			// 			add_fac_info[ii].set_bus_id = VecLfoFacW[add_num - 1 - ii].set_bus_id;
			// 			add_fac_info[ii].clus = VecLfoFacW[add_num - 1 - ii].clus;
			add_fac_info[ii].is_m_bs = VecLfoFacW[add_num - 1 - ii].is_m_bs;
		}
		if (m_WTPara.nMode == WT_MODE_ALL)
		{
			PODB_TableClear(tableno);
		}
		int ret_code = PODB_TableWrite((const char*)&add_fac_info[0].fac_idx, add_num * sizeof(SSO_FAC_WAMS), tableno);
		if (ret_code < 0)
		{
			sprintf(m_LogString, "дSSO��վ��[app��%d��table��%d]ʧ�ܣ���¼��[%d]\n", g_appNo, tableno, add_num);
			MessShow(m_LogString);
		}
		SAFE_DELETE_ARRAY(add_fac_info);
#endif // _NUSP
	}
	else if (tableno == SSO_GN_NO_TAB)
	{
		if (m_WTPara.nMode <= WT_MODE_NEW)
		{
			// ����
//			tmpIdx = PODB_GetDevIdByKeyID(tableno, m_WTPara.nGenOld + 1);  
			if (VALID_INT_REC(m_WTPara.MAX_GenOld_idx))
			{
				tmpIdx = m_WTPara.MAX_GenOld_idx;		//lns  Ӧ���Ǳ�������¼����idx + 1
				IncrRtdbId(tmpIdx);
			}
			else
			{
				tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
			}
		}
		else
		{
			tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
		}

		vector<SSO_GN_TAB_CRE> VecLfoGenW;
		SSO_GN_TAB_CRE tmpLfoGenW;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			if (VecDevInfo[ii].is_add == WT_MENU_NO)
			{
				continue;
			}

			memset((void*)&tmpLfoGenW, 0, sizeof(SSO_GN_TAB_CRE));
#ifdef _NUSP
			tmpLfoGenW.gn_idx = PODB_ChangeDevId(SSO_GN_NO_TAB, VecDevInfo[ii].dev_id);
#endif // _NUSP			
			tmpLfoGenW.gn_id = VecDevInfo[ii].dev_id;
			tmpLfoGenW.station_id = VecDevInfo[ii].fac_id;
			tmpLfoGenW.area_id = VecDevInfo[ii].area_id;
			tmpLfoGenW.is_monitor = WT_MENU_YES;
			tmpLfoGenW.is_part_ana = WT_MENU_YES;
			// 			tmpLfoGenW.clus = MENU_LFO_CLUST_RMAIN;
			//�㶫��Ŀ����
			//sprintf( m_LogString, "���������[%s][%s][%s]", VecDevInfo[ii].dev_name, VecDevInfo[ii].fac_name, VecDevInfo[ii].area_name ); 
			//MessShow( m_LogString );
			sprintf(tmpLfoGenW.gn_name, "%s", VecDevInfo[ii].dev_name);
			sprintf(tmpLfoGenW.fac_name, "%s", VecDevInfo[ii].fac_name);
			sprintf(tmpLfoGenW.area_name, "%s", VecDevInfo[ii].area_name);
			// MessShow("��ӡ���������");
			// sprintf( m_LogString, "���������[%s][%s][%s]", tmpLfoGenW.gn_name, tmpLfoGenW.fac_name, tmpLfoGenW.area_name ); 
			// MessShow( m_LogString );

			VecLfoGenW.push_back(tmpLfoGenW);
		}
		add_num = (int)VecLfoGenW.size();
		if (add_num <= 0) return 0;

		MessShow("�����������");
		//for (ii = 0; ii < (int)VecLfoGenW.size(); ii++)
		//{
		//	sprintf(m_LogString, "%d,"INT_REC_FMT"", ii + 1, INT_REC_VAL(VecLfoGenW[ii].gn_id));
		//	MessShow(m_LogString, LTDEBUG);
		//}

		// д���ÿ�
		tablename = "sso_gn";
		MessShow("��ӡSqlBuffer��");
		for (ii = 0; ii < (int)VecLfoGenW.size(); ii++)
		{
			sprintf(SqlBuffer, ""SSO_GN_TAB_CRE_FMT"",
				tablename.c_str(),
				SSO_GN_TAB_CRE_FLD,
				SSO_GN_TAB_CRE_VAL(VecLfoGenW[ii]));
			VecSql.push_back(SqlBuffer);

			sprintf(m_LogString, "%s", SqlBuffer);
			MessShow(m_LogString, LTDEBUG);
		}
#ifdef _NUSP
		SSO_GN_WAMS* add_gen_info = NULL;
		add_gen_info = new SSO_GN_WAMS[add_num];
		memset((void*)add_gen_info, 0, add_num * sizeof(SSO_GN_WAMS));
		for (ii = 0; ii < add_num; ii++)
		{
			add_gen_info[ii].gn_idx = PODB_ChangeDevId(SSO_GN_NO_TAB, VecLfoGenW[add_num - 1 - ii].gn_id);
			add_gen_info[ii].gn_id = VecLfoGenW[add_num - 1 - ii].gn_id;
			add_gen_info[ii].station_id = VecLfoGenW[add_num - 1 - ii].station_id;
			add_gen_info[ii].area_id = VecLfoGenW[add_num - 1 - ii].area_id;
			add_gen_info[ii].is_monitor = VecLfoGenW[add_num - 1 - ii].is_monitor;
			add_gen_info[ii].is_part_ana = VecLfoGenW[add_num - 1 - ii].is_part_ana;
			// 			add_gen_info[ii].clus = VecLfoGenW[add_num - 1 - ii].clus;
			sprintf(add_gen_info[ii].gn_name, "%s", VecLfoGenW[add_num - 1 - ii].gn_name);
			sprintf(add_gen_info[ii].fac_name, "%s", VecLfoGenW[add_num - 1 - ii].fac_name);
			sprintf(add_gen_info[ii].area_name, "%s", VecLfoGenW[add_num - 1 - ii].area_name);
		}
		if (m_WTPara.nMode == WT_MODE_ALL)
		{
			PODB_TableClear(tableno);
		}
		int ret_code = PODB_TableWrite((const char*)&add_gen_info[0].gn_idx, add_num * sizeof(SSO_GN_WAMS), tableno);
		if (ret_code < 0)
		{
			sprintf(m_LogString, "дSSO�������[app��%d��table��%d]ʧ�ܣ���¼��[%d]\n", g_appNo, tableno, add_num);
			MessShow(m_LogString);
		}
		SAFE_DELETE_ARRAY(add_gen_info);
#endif // _NUSP
	}
	else if (tableno == SSO_ACLN_NO_TAB)
	{
		if (m_WTPara.nMode <= WT_MODE_NEW)
		{
			// ����
			//tmpIdx = PODB_GetDevIdByKeyID(tableno, m_WTPara.nAclnOld + 1);
			if (VALID_INT_REC(m_WTPara.MAX_AclnOld_idx))
			{
				tmpIdx = m_WTPara.MAX_AclnOld_idx;		//lns  Ӧ���Ǳ�������¼����idx + 1
				IncrRtdbId(tmpIdx);
			}
			else
			{
				tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
			}
		}
		else
		{
			tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
		}

		vector<SSO_LN_TAB_CRE> VecLfoLnW;
		SSO_LN_TAB_CRE tmpLfoLnW;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			if (VecDevInfo[ii].is_add == WT_MENU_NO)
			{
				continue;
			}

			memset((void*)&tmpLfoLnW, 0, sizeof(SSO_LN_TAB_CRE));
#ifdef _NUSP
			tmpLfoLnW.acln_idx = PODB_ChangeDevId(SSO_ACLN_NO_TAB, VecDevInfo[ii].dev_id);
#endif // _NUSP
			tmpLfoLnW.acln_id = VecDevInfo[ii].dev_id;
			tmpLfoLnW.dot_id_1 = VecDevInfo[ii].dot1.dot_id;
			tmpLfoLnW.fac_id_1 = VecDevInfo[ii].dot1.fac_id;
			tmpLfoLnW.area_id_1 = VecDevInfo[ii].dot1.area_id;
			tmpLfoLnW.dot_id_2 = VecDevInfo[ii].dot2.dot_id;
			tmpLfoLnW.fac_id_2 = VecDevInfo[ii].dot2.fac_id;
			tmpLfoLnW.area_id_2 = VecDevInfo[ii].dot2.area_id;
			tmpLfoLnW.is_monitor = WT_MENU_YES;
			tmpLfoLnW.is_part_ana = WT_MENU_YES;
			if (VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_YES)
			{
				tmpLfoLnW.is_end = WT_MENU_NO;
			}
			else
			{
				tmpLfoLnW.is_end = WT_MENU_YES;
			}
			tmpLfoLnW.vlty_id = VecDevInfo[ii].maxvlty_id;
			//�㶫��Ŀ����
			sprintf(tmpLfoLnW.acln_name, "%s", VecDevInfo[ii].dev_name);
			sprintf(tmpLfoLnW.fac_name_1, "%s", VecDevInfo[ii].dot1.fac_name);
			sprintf(tmpLfoLnW.fac_name_2, "%s", VecDevInfo[ii].dot2.fac_name);
			sprintf(tmpLfoLnW.area_name_1, "%s", VecDevInfo[ii].dot1.area_name);
			sprintf(tmpLfoLnW.area_name_2, "%s", VecDevInfo[ii].dot2.area_name);

			VecLfoLnW.push_back(tmpLfoLnW);
		}
		add_num = (int)VecLfoLnW.size();
		if (add_num <= 0) return 0;

		sprintf(m_LogString, "����������·[%d]", (int)VecLfoLnW.size());
		MessShow(m_LogString, LTINFO);

		// д���ÿ�
		tablename = "sso_acln";
		for (ii = 0; ii < (int)VecLfoLnW.size(); ii++)
		{
			sprintf(SqlBuffer, ""SSO_LN_TAB_CRE_FMT"",
				tablename.c_str(),
				SSO_LN_TAB_CRE_FLD,
				SSO_LN_TAB_CRE_VAL(VecLfoLnW[ii]));
			VecSql.push_back(SqlBuffer);
			// 			sprintf(m_LogString,"%s",SqlBuffer);
			// 			MessShow(m_LogString,LTINFO);
		}
#ifdef _NUSP
		SSO_ACLN_WAMS* add_acln_info = NULL;
		add_acln_info = new SSO_ACLN_WAMS[add_num];
		memset((void*)add_acln_info, 0, add_num * sizeof(SSO_ACLN_WAMS));
		for (ii = 0; ii < add_num; ii++)
		{
			add_acln_info[ii].acln_idx = PODB_ChangeDevId(SSO_ACLN_NO_TAB, VecLfoLnW[add_num - 1 - ii].acln_id);
			add_acln_info[ii].acln_id = VecLfoLnW[add_num - 1 - ii].acln_id;
			add_acln_info[ii].dot_id_1 = VecLfoLnW[add_num - 1 - ii].dot_id_1;
			add_acln_info[ii].fac_id_1 = VecLfoLnW[add_num - 1 - ii].fac_id_1;
			add_acln_info[ii].area_id_1 = VecLfoLnW[add_num - 1 - ii].area_id_1;
			add_acln_info[ii].dot_id_2 = VecLfoLnW[add_num - 1 - ii].dot_id_2;
			add_acln_info[ii].fac_id_2 = VecLfoLnW[add_num - 1 - ii].fac_id_2;
			add_acln_info[ii].area_id_2 = VecLfoLnW[add_num - 1 - ii].area_id_2;
			add_acln_info[ii].is_monitor = VecLfoLnW[add_num - 1 - ii].is_monitor;
			add_acln_info[ii].is_part_ana = VecLfoLnW[add_num - 1 - ii].is_part_ana;
			add_acln_info[ii].is_end = VecLfoLnW[add_num - 1 - ii].is_end;
			add_acln_info[ii].vlty_id = VecLfoLnW[add_num - 1 - ii].vlty_id;
			sprintf(add_acln_info[ii].acln_name, "%s", VecLfoLnW[add_num - 1 - ii].acln_name);
			sprintf(add_acln_info[ii].fac_name_1, "%s", VecLfoLnW[add_num - 1 - ii].fac_name_1);
			sprintf(add_acln_info[ii].fac_name_2, "%s", VecLfoLnW[add_num - 1 - ii].fac_name_2);
			sprintf(add_acln_info[ii].area_name_1, "%s", VecLfoLnW[add_num - 1 - ii].area_name_1);
			sprintf(add_acln_info[ii].area_name_2, "%s", VecLfoLnW[add_num - 1 - ii].area_name_2);
		}
		if (m_WTPara.nMode == WT_MODE_ALL)
		{
			PODB_TableClear(tableno);
		}
		int ret_code = PODB_TableWrite((const char*)&add_acln_info[0].acln_idx, add_num * sizeof(SSO_ACLN_WAMS), tableno);
		if (ret_code < 0)
		{
			sprintf(m_LogString, "дSSO������·��[app��%d��table��%d]ʧ�ܣ���¼��[%d]\n", g_appNo, tableno, add_num);
			MessShow(m_LogString);
		}
		else
		{
			sprintf(m_LogString, "дSSO������·��[app��%d��table��%d]�ɹ�����¼��[%d]\n", g_appNo, tableno, add_num);
			MessShow(m_LogString);
		}
		SAFE_DELETE_ARRAY(add_acln_info);
#endif // _NUSP
	}
	else if (tableno == SSO_TR_NO_TAB)
	{
		if (m_WTPara.nMode <= WT_MODE_NEW)
		{
			// ����
			//tmpIdx = PODB_GetDevIdByKeyID(tableno, m_WTPara.nTrOld + 1);			
			if (VALID_INT_REC(m_WTPara.MAX_TrOld_idx))
			{
				tmpIdx = m_WTPara.MAX_TrOld_idx;		//lns  Ӧ���Ǳ�������¼����idx + 1
				IncrRtdbId(tmpIdx);
			}
			else
			{
				tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
			}
		}
		else
		{
			tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
		}

		vector<SSO_TR_TAB_CRE> VecLfoTrW;
		SSO_TR_TAB_CRE tmpLfoTrW;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			if (VecDevInfo[ii].is_add == WT_MENU_NO)
			{
				continue;
			}

			memset((void*)&tmpLfoTrW, 0, sizeof(SSO_TR_TAB_CRE));
#ifdef _NUSP
			tmpLfoTrW.tr_idx = PODB_ChangeDevId(SSO_TR_NO_TAB, VecDevInfo[ii].dev_id);
#endif // _NUSP
			tmpLfoTrW.tr_id = VecDevInfo[ii].dev_id;
			tmpLfoTrW.station_id = VecDevInfo[ii].fac_id;
			tmpLfoTrW.area_id = VecDevInfo[ii].area_id;
			tmpLfoTrW.trty = VecDevInfo[ii].trty;
			tmpLfoTrW.tr_type = VecDevInfo[ii].dev_type;
			tmpLfoTrW.high_wind_id = VecDevInfo[ii].dot1.dot_id;
			tmpLfoTrW.high_vlty_id = VecDevInfo[ii].dot1.vlty_id;
			tmpLfoTrW.mid_wind_id = VecDevInfo[ii].dot3.dot_id;
			tmpLfoTrW.mid_vlty_id = VecDevInfo[ii].dot3.vlty_id;
			tmpLfoTrW.low_wind_id = VecDevInfo[ii].dot2.dot_id;
			tmpLfoTrW.low_vlty_id = VecDevInfo[ii].dot2.vlty_id;
			tmpLfoTrW.slct_wind_id = VecDevInfo[ii].dot1.dot_id;
			tmpLfoTrW.is_monitor = WT_MENU_YES;
			tmpLfoTrW.is_part_ana = WT_MENU_YES;
			tmpLfoTrW.hwd_yc_ok = VecDevInfo[ii].dot1.IsYcDefine;
			tmpLfoTrW.mwd_yc_ok = VecDevInfo[ii].dot3.IsYcDefine;
			tmpLfoTrW.lwd_yc_ok = VecDevInfo[ii].dot2.IsYcDefine;
			//�㶫��Ŀ����
			sprintf(tmpLfoTrW.tr_name, "%s", VecDevInfo[ii].dev_name);
			sprintf(tmpLfoTrW.fac_name, "%s", VecDevInfo[ii].fac_name);
			sprintf(tmpLfoTrW.area_name, "%s", VecDevInfo[ii].area_name);

			VecLfoTrW.push_back(tmpLfoTrW);
		}
		add_num = (int)VecLfoTrW.size();
		if (add_num <= 0) return 0;

		sprintf(m_LogString, "������ѹ��:[%d]", VecLfoTrW.size());
		MessShow(m_LogString, LTINFO);

		// д���ÿ�
		tablename = "sso_tr";
		for (ii = 0; ii < (int)VecLfoTrW.size(); ii++)
		{
			sprintf(SqlBuffer, ""SSO_TR_TAB_CRE_FMT"",
				tablename.c_str(),
				SSO_TR_TAB_CRE_FLD,
				SSO_TR_TAB_CRE_VAL(VecLfoTrW[ii]));
			VecSql.push_back(SqlBuffer);
		}
#ifdef _NUSP
		SSO_TR_WAMS* add_tr_info = NULL;
		add_tr_info = new SSO_TR_WAMS[add_num];
		memset((void*)add_tr_info, 0, add_num * sizeof(SSO_TR_WAMS));
		for (ii = 0; ii < add_num; ii++)
		{
			add_tr_info[ii].tr_idx = PODB_ChangeDevId(SSO_TR_NO_TAB, VecLfoTrW[add_num - 1 - ii].tr_id);
			add_tr_info[ii].tr_id = VecLfoTrW[add_num - 1 - ii].tr_id;
			add_tr_info[ii].station_id = VecLfoTrW[add_num - 1 - ii].station_id;
			add_tr_info[ii].area_id = VecLfoTrW[add_num - 1 - ii].area_id;
			add_tr_info[ii].trty = VecLfoTrW[add_num - 1 - ii].trty;
			// 			add_tr_info[ii].tr_type = VecLfoTrW[add_num - 1 - ii].tr_type;
			add_tr_info[ii].high_wind_id = VecLfoTrW[add_num - 1 - ii].high_wind_id;
			add_tr_info[ii].high_vlty_id = VecLfoTrW[add_num - 1 - ii].high_vlty_id;
			add_tr_info[ii].mid_wind_id = VecLfoTrW[add_num - 1 - ii].mid_wind_id;
			add_tr_info[ii].mid_vlty_id = VecLfoTrW[add_num - 1 - ii].mid_vlty_id;
			add_tr_info[ii].low_wind_id = VecLfoTrW[add_num - 1 - ii].low_wind_id;
			add_tr_info[ii].low_vlty_id = VecLfoTrW[add_num - 1 - ii].low_vlty_id;
			add_tr_info[ii].slct_wind_id = VecLfoTrW[add_num - 1 - ii].slct_wind_id;
			// 			add_tr_info[ii].mon_wind_slct = VecLfoTrW[add_num - 1 - ii].mon_wind_slct;
			add_tr_info[ii].is_monitor = VecLfoTrW[add_num - 1 - ii].is_monitor;
			add_tr_info[ii].is_part_ana = VecLfoTrW[add_num - 1 - ii].is_part_ana;
			add_tr_info[ii].hwd_yc_ok = VecLfoTrW[add_num - 1 - ii].hwd_yc_ok;
			add_tr_info[ii].mwd_yc_ok = VecLfoTrW[add_num - 1 - ii].mwd_yc_ok;
			add_tr_info[ii].lwd_yc_ok = VecLfoTrW[add_num - 1 - ii].lwd_yc_ok;
			//�㶫��Ŀ����
			sprintf(add_tr_info[ii].tr_name, "%s", VecLfoTrW[add_num - 1 - ii].tr_name);
			sprintf(add_tr_info[ii].fac_name, "%s", VecLfoTrW[add_num - 1 - ii].fac_name);
			sprintf(add_tr_info[ii].area_name, "%s", VecLfoTrW[add_num - 1 - ii].area_name);
		}
		if (m_WTPara.nMode == WT_MODE_ALL)
		{
			PODB_TableClear(tableno);
		}
		int ret_code = PODB_TableWrite((const char*)&add_tr_info[0].tr_idx, add_num * sizeof(SSO_TR_WAMS), tableno);
		if (ret_code < 0)
		{
			sprintf(m_LogString, "дSSO��ѹ����[app��%d��table��%d]ʧ�ܣ���¼��[%d]\n", g_appNo, tableno, add_num);
			MessShow(m_LogString);
		}
		else
		{
			sprintf(m_LogString, "дSSO��ѹ����[app��%d��table��%d]�ɹ�����¼��[%d]\n", g_appNo, tableno, add_num);
			MessShow(m_LogString);
		}
		SAFE_DELETE_ARRAY(add_tr_info);
#endif // _NUSP
	}
	else if (tableno == PQ_DA_BUS_NO_TAB)
	{
		if (m_WTPara.nMode <= WT_MODE_NEW)
		{
			// ����
			//tmpIdx = PODB_GetDevIdByKeyID(PQ_DA_BUS_NO_TAB, m_WTPara.nVFOld + 1);
			if (VALID_INT_REC(m_WTPara.MAX_VFOld_idx))
			{
				tmpIdx = m_WTPara.MAX_VFOld_idx;		//lns  Ӧ���Ǳ�������¼����idx + 1
				IncrRtdbId(tmpIdx);
			}
			else
			{
				tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
			}
		}
		else
		{
			tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
		}

		// VF��̬����������,��ȡĬ�϶�Ԫ�����
		vector<PQ_DA_CTL_PARA_TAB_CRE> VecVFPara;
		if (GetVFPara(VecVFPara) < 0)
		{
			return -1;
		}
		INT_REC tmp_dft_v_para; ZERO_INT_REC(tmp_dft_v_para);
		INT_REC tmp_dft_f_para; ZERO_INT_REC(tmp_dft_f_para);
		if (!VecVFPara.empty())
		{
			tmp_dft_f_para = VecVFPara[0].dft_f_para;
			tmp_dft_v_para = VecVFPara[0].dft_v_para;
		}

		vector<PQ_DA_BUS_TAB_CRE> VecVFBus;
		PQ_DA_BUS_TAB_CRE tmpVFBus;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			if (VecDevInfo[ii].is_add == WT_MENU_NO)
			{
				continue;
			}

			memset((void*)&tmpVFBus, 0, sizeof(PQ_DA_BUS_TAB_CRE));
			// ����в��Ķ˵�
			if (VecDevInfo[ii].dot1.IsYcDefine == WT_MENU_YES)
			{
				tmpVFBus.bus_id = VecDevInfo[ii].dot1.dot_id;
				tmpVFBus.fac_id = VecDevInfo[ii].dot1.fac_id;
				tmpVFBus.area_id = VecDevInfo[ii].dot1.area_id;
			}
			else if (VecDevInfo[ii].dot2.IsYcDefine == WT_MENU_YES)
			{
				tmpVFBus.bus_id = VecDevInfo[ii].dot2.dot_id;
				tmpVFBus.fac_id = VecDevInfo[ii].dot2.fac_id;
				tmpVFBus.area_id = VecDevInfo[ii].dot2.area_id;
			}
			else if (VecDevInfo[ii].dot3.IsYcDefine == WT_MENU_YES)
			{
				tmpVFBus.bus_id = VecDevInfo[ii].dot3.dot_id;
				tmpVFBus.fac_id = VecDevInfo[ii].dot3.fac_id;
				tmpVFBus.area_id = VecDevInfo[ii].dot3.area_id;
			}
#ifdef _NUSP
			tmpVFBus.bus_idx = PODB_ChangeDevId(PQ_DA_BUS_NO_TAB, tmpVFBus.bus_id);
#endif // _NUSP
			tmpVFBus.volt_monitor = WT_MENU_YES;
			tmpVFBus.volt_per_id = tmp_dft_v_para;
			tmpVFBus.freq_monitor = WT_MENU_YES;
			tmpVFBus.freq_per_id = tmp_dft_f_para;
			tmpVFBus.freq_yc_id = VecDevInfo[ii].freq_yc_id;
			tmpVFBus.vbase = VecDevInfo[ii].maxvbase;

			VecVFBus.push_back(tmpVFBus);
		}
		add_num = (int)VecVFBus.size();
		if (add_num <= 0) return 0;

		MessShow("����ĸ�ߣ�");

		// д���ÿ�
		tablename = "pq_da_bus";
		for (ii = 0; ii < (int)VecVFBus.size(); ii++)
		{
			sprintf(SqlBuffer, ""PQ_DA_BUS_TAB_CRE_FMT"",
				tablename.c_str(),
				PQ_DA_BUS_TAB_CRE_FLD,
				PQ_DA_BUS_TAB_CRE_VAL(VecVFBus[ii]));
			VecSql.push_back(SqlBuffer);
		}
#ifdef _NUSP
		PQ_DA_BUS_WAMS* add_VFBus_info = NULL;
		add_VFBus_info = new PQ_DA_BUS_WAMS[add_num];
		memset((void*)add_VFBus_info, 0, add_num * sizeof(PQ_DA_BUS_WAMS));
		for (ii = 0; ii < add_num; ii++)
		{
			add_VFBus_info[ii].bus_idx = PODB_ChangeDevId(PQ_DA_BUS_NO_TAB, VecVFBus[add_num - 1 - ii].bus_id);
			add_VFBus_info[ii].bus_id = VecVFBus[add_num - 1 - ii].bus_id;
			add_VFBus_info[ii].fac_id = VecVFBus[add_num - 1 - ii].fac_id;
			add_VFBus_info[ii].area_id = VecVFBus[add_num - 1 - ii].area_id;
			add_VFBus_info[ii].volt_monitor = VecVFBus[add_num - 1 - ii].volt_monitor;
			add_VFBus_info[ii].volt_per_id = VecVFBus[add_num - 1 - ii].volt_per_id;
			add_VFBus_info[ii].freq_monitor = VecVFBus[add_num - 1 - ii].freq_monitor;
			add_VFBus_info[ii].freq_per_id = VecVFBus[add_num - 1 - ii].freq_per_id;
			add_VFBus_info[ii].freq_yc_id = VecVFBus[add_num - 1 - ii].freq_yc_id;
			add_VFBus_info[ii].vbase = VecVFBus[add_num - 1 - ii].vbase;
		}
		if (m_WTPara.nMode == WT_MODE_ALL)
		{
			PODB_TableClear(tableno);
		}
		int ret_code = PODB_TableWrite((const char*)&add_VFBus_info[0].bus_idx, add_num * sizeof(PQ_DA_BUS_WAMS), tableno);
		if (ret_code < 0)
		{
			sprintf(m_LogString, "дVF��̬����ĸ�߱�[app��%d��table��%d]ʧ�ܣ���¼��[%d]\n", g_appNo, tableno, add_num);
			MessShow(m_LogString);
		}
		SAFE_DELETE_ARRAY(add_VFBus_info);
#endif // _NUSP
	}
	else if (tableno == WAM_AM_DEV_NO_TAB)
	{
		if (m_WTPara.nMode <= WT_MODE_NEW)
		{
			// ����
			//tmpIdx = PODB_GetDevIdByKeyID(tableno, m_WTPara.nAMOld + 1);
			if (VALID_INT_REC(m_WTPara.MAX_AMOld_idx))
			{
				tmpIdx = m_WTPara.MAX_AMOld_idx;		//lns  Ӧ���Ǳ�������¼����idx + 1
				IncrRtdbId(tmpIdx);
			}
			else
			{
				tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
			}
		}
		else
		{
			tmpIdx = PODB_GetDevIdByKeyID(tableno, 1);
		}

		vector<WAM_AM_DEV_CRE> VecAmGen;
		WAM_AM_DEV_CRE tmpAmGen;
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			if (VecDevInfo[ii].is_add == WT_MENU_NO)
			{
				continue;
			}

			memset((void*)&tmpAmGen, 0, sizeof(WAM_AM_DEV_CRE));
#ifdef _NUSP
			tmpAmGen.dev_idx = PODB_ChangeDevId(WAM_AM_DEV_NO_TAB, VecDevInfo[ii].dev_id);
#endif // _NUSP
			tmpAmGen.dev_id = VecDevInfo[ii].dev_id;
			tmpAmGen.fac_id = VecDevInfo[ii].fac_id;
			tmpAmGen.area_id = VecDevInfo[ii].area_id;
			tmpAmGen.vlty_id = VecDevInfo[ii].dot1.vlty_id;
			sprintf(tmpAmGen.dev_name, "%s", VecDevInfo[ii].dev_name);
			sprintf(tmpAmGen.fac_name, "%s", VecDevInfo[ii].fac_name);
			sprintf(tmpAmGen.area_name, "%s", VecDevInfo[ii].area_name);
			tmpAmGen.is_monitor = 1;
			sprintf(tmpAmGen.bpaname, "%s", VecDevInfo[ii].bpa_name);
			tmpAmGen.sim_curve_idx = VecDevInfo[ii].sim_curve_idx;
			//#define VEC_VIP_NUM 17 // UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
			tmpAmGen.fn1 = VecDevInfo[ii].dot1.nYcFilNo[0];
			tmpAmGen.cn1 = VecDevInfo[ii].dot1.nYcColNo[0];
			tmpAmGen.fn2 = VecDevInfo[ii].dot1.nYcFilNo[1];
			tmpAmGen.cn2 = VecDevInfo[ii].dot1.nYcColNo[1];
			tmpAmGen.fn3 = VecDevInfo[ii].dot1.nYcFilNo[2];
			tmpAmGen.cn3 = VecDevInfo[ii].dot1.nYcColNo[2];
			tmpAmGen.fn4 = VecDevInfo[ii].dot1.nYcFilNo[3];
			tmpAmGen.cn4 = VecDevInfo[ii].dot1.nYcColNo[3];
			tmpAmGen.fn5 = VecDevInfo[ii].dot1.nYcFilNo[4];
			tmpAmGen.cn5 = VecDevInfo[ii].dot1.nYcColNo[4];
			tmpAmGen.fn6 = VecDevInfo[ii].dot1.nYcFilNo[5];
			tmpAmGen.cn6 = VecDevInfo[ii].dot1.nYcColNo[5];
			tmpAmGen.fn7 = VecDevInfo[ii].dot1.nYcFilNo[6];
			tmpAmGen.cn7 = VecDevInfo[ii].dot1.nYcColNo[6];
			tmpAmGen.fn8 = VecDevInfo[ii].dot1.nYcFilNo[7];
			tmpAmGen.cn8 = VecDevInfo[ii].dot1.nYcColNo[7];
			tmpAmGen.fn9 = VecDevInfo[ii].dot1.nYcFilNo[8];
			tmpAmGen.cn9 = VecDevInfo[ii].dot1.nYcColNo[8];
			tmpAmGen.fn10 = VecDevInfo[ii].dot1.nYcFilNo[9];
			tmpAmGen.cn10 = VecDevInfo[ii].dot1.nYcColNo[9];
			tmpAmGen.fn11 = VecDevInfo[ii].dot1.nYcFilNo[10];
			tmpAmGen.cn11 = VecDevInfo[ii].dot1.nYcColNo[10];
			tmpAmGen.fn12 = VecDevInfo[ii].dot1.nYcFilNo[11];
			tmpAmGen.cn12 = VecDevInfo[ii].dot1.nYcColNo[11];
			tmpAmGen.fn13 = VecDevInfo[ii].dot1.nYcFilNo[12];
			tmpAmGen.cn13 = VecDevInfo[ii].dot1.nYcColNo[12];
			tmpAmGen.fn14 = VecDevInfo[ii].dot1.nYcFilNo[13];
			tmpAmGen.cn14 = VecDevInfo[ii].dot1.nYcColNo[13];
			tmpAmGen.fn15 = VecDevInfo[ii].dot1.nYcFilNo[14];
			tmpAmGen.cn15 = VecDevInfo[ii].dot1.nYcColNo[14];
			tmpAmGen.fn16 = VecDevInfo[ii].dot1.nYcFilNo[15];
			tmpAmGen.cn16 = VecDevInfo[ii].dot1.nYcColNo[15];
			tmpAmGen.fn17 = VecDevInfo[ii].dot1.nYcFilNo[16];
			tmpAmGen.cn17 = VecDevInfo[ii].dot1.nYcColNo[16];

			VecAmGen.push_back(tmpAmGen);
		}

		if ((int)VecAmGen.size() <= 0) return 0;

		MessShow("����AM�������");

		// д���ÿ�
		tablename = "wam_am_dev";
		for (ii = 0; ii < (int)VecAmGen.size(); ii++)
		{
			sprintf(SqlBuffer, ""WAM_AM_DEV_CRE_FMT"",
				tablename.c_str(),
				WAM_AM_DEV_CRE_FLD,
				WAM_AM_DEV_CRE_VAL(VecAmGen[ii]));
			VecSql.push_back(SqlBuffer);
		}
	}
	else
	{
		sprintf(m_LogString, "�豸���ʹ���[%d]", tableno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	MessShow("׼��sql���", LTINFO);
	for (ii = 0; ii < (int)VecSql.size(); ii++)
	{
		MessShow(VecSql[ii].c_str(), LTDEBUG);
	}

	if (m_WTPara.nMode == WT_MODE_TEST)
	{
		MessShow("����ģʽ�����޸����ÿ�");
	}
	else if (m_WTPara.nMode == WT_MODE_ALL)
	{
		// ȫ��д�룬�����
		MessShow("ȫ��д��, ����豸��");
#ifdef _WAMAP_
		PODB_TableClearSql(tablename);
#endif
		MessShow("ȫ��д��, �޸����ÿ�", LTINFO);
#ifdef _WAMAP_
		PODB_ModifyTableBySqls(VecSql);
#endif
		//PODB_ExecuteMultiSql(VecSql);
	}
	else if (m_WTPara.nMode == WT_MODE_NEW)
	{
		// д�������豸
		MessShow("�����豸, �޸����ÿ�", LTINFO);
#ifdef _WAMAP_
		PODB_ModifyTableBySqls(VecSql);
#endif
	}

	return 0;
}

int CWamToolObj::WriteDevFile(vector<WT_DEV_INFO>& VecDevInfo, const int tableno)
{
	int ii, jj;
	time_t    CurTime;
	time(&CurTime);

	sprintf(m_LogString, "ά�����ݱ�ʱ��: %s", PT_GetChineseTimeStringBySeconds(CurTime));
	MessShow(m_LogString);

	if (tableno == SSO_FAC_NO_TAB)
	{
		sprintf(m_LogString, "��վ��Ϣ-���[%d]-��վ[%d]-PMU��վ[%d]-LFO��վ[%d]-����[%d]", SSO_FAC_NO_TAB, m_WTPara.nFacAll, m_WTPara.nFacPmu, m_WTPara.nFacOld, m_WTPara.nFacNew);
		MessShow(m_LogString);
		sprintf(m_LogString, "���\t\t��վ����\t\tĸ��\t\t��ѹ��־\t\t����־\t\t�Ƿ����б�־\t\t�Ƿ�����");
		MessShow(m_LogString);
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			sprintf(m_LogString, "%d\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d\t\t%d\t\t%d",
				ii + 1, VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dot1.dot_name, INT_REC_VAL(VecDevInfo[ii].dot1.dot_id),
				VecDevInfo[ii].vflag, VecDevInfo[ii].yc_flag, VecDevInfo[ii].is_old, VecDevInfo[ii].is_add);
			MessShow(m_LogString, LTDEV);
			sprintf(m_LogString, "NAME\t\tFilNo\t\tColNo");
			MessShow(m_LogString, LTDEV);
			for (jj = 0; jj < VEC_SSO_NUM; jj++)
			{
				sprintf(m_LogString, "%s\t\t\t\t%d\t\t\t\t%d",
					PODB_GetYcName(jj).c_str(), VecDevInfo[ii].dot1.nYcFilNo[jj], VecDevInfo[ii].dot1.nYcColNo[jj]);
				MessShow(m_LogString, LTDEV);
			}
		}
	}
	else if (tableno == SSO_GN_NO_TAB)
	{
		sprintf(m_LogString, "�������Ϣ-���[%d]-�����[%d]-PMU�����[%d]-LFO�����[%d]-����[%d]", SSO_GN_NO_TAB, m_WTPara.nGenAll, m_WTPara.nGenPmu, m_WTPara.nGenOld, m_WTPara.nGenNew);
		MessShow(m_LogString);
		sprintf(m_LogString, "���\t\t\t\t���������\t\t��վ����\t\t����\t\t��ѹ��־\t\t����־\t\t�Ƿ����б�־\t\t�Ƿ�����");
		MessShow(m_LogString);
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			sprintf(m_LogString, "%d\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d\t\t%d\t\t%d",
				ii + 1, VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].fac_name, INT_REC_VAL(VecDevInfo[ii].fac_id), VecDevInfo[ii].area_name, INT_REC_VAL(VecDevInfo[ii].area_id),
				VecDevInfo[ii].vflag, VecDevInfo[ii].yc_flag, VecDevInfo[ii].is_old, VecDevInfo[ii].is_add);
			MessShow(m_LogString, LTDEV);
			sprintf(m_LogString, "NAME\t\tFilNo\t\tColNo");
			MessShow(m_LogString, LTDEV);
			for (jj = 0; jj < VEC_SSO_NUM; jj++)
			{
				sprintf(m_LogString, "%s\t\t\t\t%d\t\t\t\t%d",
					PODB_GetYcName(jj).c_str(), VecDevInfo[ii].dot1.nYcFilNo[jj], VecDevInfo[ii].dot1.nYcColNo[jj]);
				MessShow(m_LogString, LTDEV);
			}
		}
	}
	else if (tableno == SSO_ACLN_NO_TAB)
	{
		sprintf(m_LogString, "������·��Ϣ-���[%d]-������·[%d]-PMU������·[%d]-LFO������·[%d]-����[%d]", SSO_ACLN_NO_TAB, m_WTPara.nAclnAll, m_WTPara.nAclnPmu, m_WTPara.nAclnOld, m_WTPara.nAclnNew);
		MessShow(m_LogString);
		sprintf(m_LogString, "���\t\t\t\t������·����\t\t�׶�\t\tĩ��\t\t��ѹ��־\t\t����־\t\t�Ƿ����б�־\t\t�Ƿ�����");
		MessShow(m_LogString);
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			sprintf(m_LogString, "%d\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d\t\t%d\t\t%d",
				ii + 1, VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dot1.fac_name, INT_REC_VAL(VecDevInfo[ii].dot1.dot_id), VecDevInfo[ii].dot2.fac_name, INT_REC_VAL(VecDevInfo[ii].dot2.dot_id),
				VecDevInfo[ii].vflag, VecDevInfo[ii].yc_flag, VecDevInfo[ii].is_old, VecDevInfo[ii].is_add);
			MessShow(m_LogString, LTDEV);
			sprintf(m_LogString, "NAME\t\tFilNo1\t\tColNo1\t\tFilNo2\t\tColNo2");
			MessShow(m_LogString, LTDEV);
			for (jj = 0; jj < VEC_SSO_NUM; jj++)
			{
				sprintf(m_LogString, "%s\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d",
					PODB_GetYcName(jj).c_str(), VecDevInfo[ii].dot1.nYcFilNo[jj], VecDevInfo[ii].dot1.nYcColNo[jj],
					VecDevInfo[ii].dot2.nYcFilNo[jj], VecDevInfo[ii].dot2.nYcColNo[jj]);
				MessShow(m_LogString, LTDEV);
			}
		}
	}
	else if (tableno == SSO_TR_NO_TAB)
	{
		sprintf(m_LogString, "��ѹ����Ϣ-���[%d]-��ѹ��[%d]-PMU��ѹ��[%d]-LFO��ѹ��[%d]-����[%d]", SSO_TR_NO_TAB, m_WTPara.nTrAll, m_WTPara.nTrPmu, m_WTPara.nTrOld, m_WTPara.nTrNew);
		MessShow(m_LogString);
		sprintf(m_LogString, "���\t\t\t\t��ѹ������\t\t��ѹ��\t\t��ѹ��\t\t��ѹ��\t\t��ѹ��־\t\t����־\t\t�Ƿ����б�־\t\t�Ƿ�����");
		MessShow(m_LogString);
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			sprintf(m_LogString, "%d\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d\t\t%d\t\t%d",
				ii + 1, VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].dot1.dot_name, INT_REC_VAL(VecDevInfo[ii].dot1.dot_id), VecDevInfo[ii].dot2.dot_name, INT_REC_VAL(VecDevInfo[ii].dot2.dot_id),
				VecDevInfo[ii].dot3.dot_name, INT_REC_VAL(VecDevInfo[ii].dot3.dot_id), VecDevInfo[ii].vflag, VecDevInfo[ii].yc_flag, VecDevInfo[ii].is_old, VecDevInfo[ii].is_add);
			MessShow(m_LogString, LTDEV);
			sprintf(m_LogString, "NAME\t\tFilNo1\t\tColNo1\t\tFilNo2\t\tColNo2\t\tFilNo3\t\tColNo3");
			MessShow(m_LogString, LTDEV);
			for (jj = 0; jj < VEC_SSO_NUM; jj++)
			{
				sprintf(m_LogString, "%s\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d",
					PODB_GetYcName(jj).c_str(), VecDevInfo[ii].dot1.nYcFilNo[jj], VecDevInfo[ii].dot1.nYcColNo[jj],
					VecDevInfo[ii].dot2.nYcFilNo[jj], VecDevInfo[ii].dot2.nYcColNo[jj],
					VecDevInfo[ii].dot3.nYcFilNo[jj], VecDevInfo[ii].dot3.nYcColNo[jj]);
				MessShow(m_LogString, LTDEV);
			}
		}
	}
	else if (tableno == PQ_DA_BUS_NO_TAB)
	{
		sprintf(m_LogString, "ĸ����Ϣ-���[%d]-ĸ��[%d]-PMUĸ��[%d]-VFĸ��[%d]-����[%d]", PQ_DA_BUS_NO_TAB, m_WTPara.nVFAll, m_WTPara.nVFPmu, m_WTPara.nVFOld, m_WTPara.nVFNew);
		MessShow(m_LogString);
		sprintf(m_LogString, "���	ĸ������	��վ                               ��ѹ��־		����־		�Ƿ����б�־		�Ƿ�����");
		MessShow(m_LogString);
		for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
		{
			sprintf(m_LogString, "%-5d    %s["INT_REC_FMT"]    %s["INT_REC_FMT"]    %d    %d    %d    %d",
				ii + 1, VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].dev_id), VecDevInfo[ii].fac_name, INT_REC_VAL(VecDevInfo[ii].fac_id), VecDevInfo[ii].vflag, VecDevInfo[ii].yc_flag, VecDevInfo[ii].is_old, VecDevInfo[ii].is_add);
			MessShow(m_LogString, LTDEV);
			sprintf(m_LogString, "NAME    FilNo1    ColNo1  FHZ");
			MessShow(m_LogString, LTDEV);
			for (jj = 0; jj < VEC_V_NUM; jj++)
			{
				sprintf(m_LogString, "%s    %d    %d    "INT_REC_FMT"",
					PODB_GetYcName(jj).c_str(), VecDevInfo[ii].dot1.nYcFilNo[jj], VecDevInfo[ii].dot1.nYcColNo[jj], INT_REC_VAL(VecDevInfo[ii].freq_yc_id));
				MessShow(m_LogString, LTDEV);
			}
		}
	}
	else
	{
		sprintf(m_LogString, "�豸���ʹ���[%d]", tableno);
		MessShow(m_LogString, LTERROR);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// ���ݿ����
int CWamToolObj::WriteToolInf(const string ToolInf)
{
	if (m_WTPara.nMode == WT_MODE_TEST)
	{
		return 0;
	}
	if (m_WTPara.IsAclnUpdate == 1 ||
		m_WTPara.IsGenUpdate == 1 ||
		m_WTPara.IsAclnUpdate == 1 ||
		m_WTPara.IsTrUpdate == 1)
	{
#ifdef _WAMAP_
		int idx = PODB_GetTabRecNum(SSO_PARA_NO_TAB);
		INT_REC recordid = PODB_GetDevIdByKeyID(SSO_PARA_NO_TAB, 1);
		char ana_run_ctx[64];
		char LogString[1024];
		sprintf(ana_run_ctx, "%s", ToolInf.c_str());
		sprintf(LogString, "WriteToolInf���²�����[%s],���[%d],��¼ID["INT_REC_FMT"]��¼��[%d]", ana_run_ctx, SSO_PARA_NO_TAB, INT_REC_VAL(recordid), idx);
		MessShow(LogString);

		PODB_TableModifyByKey((const char*)&recordid, "ana_run_ctx", (char*)&ana_run_ctx, sizeof(ana_run_ctx), SSO_PARA_NO_TAB);
#endif
	}

	return 0;
}

int CWamToolObj::InitAMGenInfo(vector<WT_DEV_INFO>& VecDevInfo)
{
	if (!VecDevInfo.empty())
	{
		vector<WT_DEV_INFO>().swap(VecDevInfo);
		VecDevInfo.clear();
	}

	int ii, jj;
	for (ii = 0; ii < (int)m_VecGenInfo.size(); ii++)
	{
		if (m_VecGenInfo[ii].yc_flag == 1)
		{
			VecDevInfo.push_back(m_VecGenInfo[ii]);
		}
	}

	m_WTPara.nAMAll = m_WTPara.nGenAll;
	m_WTPara.nAMPmu = m_WTPara.nGenPmu;
	sprintf(m_LogString, "AM�����[%d]", m_WTPara.nAMPmu);
	MessShow(m_LogString, LTINFO);

	// ����豸BPA���ƺ���������
	vector<GN_INFO_BASE> VecGenBase;
	string tmpFile = m_WTPara.GenInfoFile;
	if (ReadAMBpaName(tmpFile, VecGenBase) < 0)
	{
		return 0;
	}

	int nFlag;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		nFlag = 0;
		VecDevInfo[ii].sim_curve_idx = 9999;
		for (jj = 0; jj < (int)VecGenBase.size(); jj++)
		{
			if (VecDevInfo[ii].dev_id == VecGenBase[jj].dev_id)
			{
				nFlag = 1;
				sprintf(VecDevInfo[ii].bpa_name, "%s", VecGenBase[jj].bpaname);
				VecDevInfo[ii].sim_curve_idx = VecGenBase[jj].sim_curve_idx;
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "δ�ҵ��豸BPA��[%s]["INT_REC_FMT"]", VecDevInfo[ii].dev_name, INT_REC_VAL(VecDevInfo[ii].dev_id));
			MessShow(m_LogString, LTINFO);
		}
	}

	// ��������������
// 	if (!VecDevInfo.empty())
// 	{
// 		sort(VecDevInfo.begin(),VecDevInfo.end(),g_SortGnByIdx);
// 	}
	return 0;
}

int CWamToolObj::GetAMGen(vector<WT_DEV_INFO>& VecDevInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(WAM_AM_DEV_NO_TAB, WAM_AM_DEV_CRE_FLD, buf_base) < 0)
	{
		sprintf(m_LogString, "��ȡSSO�������ʧ��");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(WAM_AM_DEV_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "WAM_AM�豸���¼Ϊ��[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "WAM_AM�豸��[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	WAM_AM_DEV_CRE* pInfo = NULL;
	pInfo = new WAM_AM_DEV_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(WAM_AM_DEV_CRE));

	// ���Ѵ��ڱ�־
	int ii, jj;
	m_WTPara.nAMOld = nInfo;
	for (ii = 0; ii < (int)VecDevInfo.size(); ii++)
	{
		for (jj = 0; jj < nInfo; jj++)
		{
			if (VecDevInfo[ii].dev_id == pInfo[jj].dev_id)
			{
				VecDevInfo[ii].is_old = 1;
				break;
			}
		}
	}

#ifdef _NUSP
	INT_REC MAX_AMIdxTemp = pInfo[0].dev_idx;
	for (ii = 0; ii < nInfo; ii++)
	{
		if (MAX_AMIdxTemp < pInfo[ii].dev_idx)
		{
			MAX_AMIdxTemp = pInfo[ii].dev_idx;
		}
	}
	m_WTPara.MAX_AMOld_idx = MAX_AMIdxTemp;
#endif // _NUSP
	
	SAFE_DELETE(pInfo);
#endif

	return 0;
}

int CWamToolObj::ReadAMBpaName(const string tmpFileName, vector<GN_INFO_BASE>& VecGenBase)
{
	if (!VecGenBase.empty())
	{
		vector<GN_INFO_BASE>().swap(VecGenBase);
		VecGenBase.clear();
	}

	sprintf(m_LogString, "��ȡ������Ϣ�ļ�[%s]", tmpFileName.c_str());
	MessShow(m_LogString, LTINFO);
	fstream fDataFile;
	fDataFile.open(tmpFileName.c_str(), ios::in);
	if (fDataFile.fail())
	{
		sprintf(m_LogString, "�ļ���ʧ��[%s]", tmpFileName.c_str());
		MessShow(m_LogString);
		return -1;
	}

	string strline, tmpstr;
	vector<string> vecstr;
	stringstream ss;
	GN_INFO_BASE tmpGenInfo;

	// ��ȡ��һ�б�ͷ
	int nColNum = 0;
	getline(fDataFile, strline);
	nColNum++;
	// �ӵڶ��п�ʼ��ȡ
	int ii;
	while (getline(fDataFile, strline))
	{
		nColNum++;
		vector<string>().swap(vecstr);
		vecstr.clear();
		ss.clear();
		ss.str(strline);
		while (getline(ss, tmpstr, ','))
		{
			vecstr.push_back(tmpstr);
		}

		if (vecstr.size() < 64)
		{
			sprintf(m_LogString, "������Ϣ�ļ���[%d]�д���[%d]", nColNum, vecstr.size());
			MessShow(m_LogString, LTERROR);
			continue;
		}

		memset((void*)&tmpGenInfo, 0, sizeof(GN_INFO_BASE));
		tmpGenInfo.dev_idx = ATOI(vecstr[0].c_str());
		tmpGenInfo.dev_id = ATOI(vecstr[1].c_str());
		tmpGenInfo.fac_id = ATOI(vecstr[2].c_str());
		tmpGenInfo.area_id = ATOI(vecstr[3].c_str());
		sprintf(tmpGenInfo.dev_name, "%s", vecstr[4].c_str());
		sprintf(tmpGenInfo.fac_name, "%s", vecstr[5].c_str());
		sprintf(tmpGenInfo.area_name, "%s", vecstr[6].c_str());
		tmpGenInfo.dev_type = atoi(vecstr[7].c_str());
		tmpGenInfo.r_value = (float)atof(vecstr[8].c_str());
		tmpGenInfo.x_value = (float)atof(vecstr[9].c_str());
		tmpGenInfo.is_monitor = atoi(vecstr[10].c_str());
		tmpGenInfo.is_calc_ang = atoi(vecstr[11].c_str());
		sprintf(tmpGenInfo.bpaname, "%s", vecstr[12].c_str());
		tmpGenInfo.sim_curve_idx = atoi(vecstr[13].c_str());
		tmpGenInfo.stat = atoi(vecstr[14].c_str());
		tmpGenInfo.stat_value = atoi(vecstr[15].c_str());
		sprintf(tmpGenInfo.stat_desc, "%s", vecstr[16].c_str());
		tmpGenInfo.angle_value = (float)atof(vecstr[17].c_str());
		tmpGenInfo.power_value = (float)atof(vecstr[18].c_str());
		tmpGenInfo.group = atoi(vecstr[19].c_str());
		for (ii = 0; ii < VEC_ALL_NUM; ii++)
		{
			tmpGenInfo.fn[ii] = atoi(vecstr[20 + ii].c_str());
		}
		for (ii = 0; ii < VEC_ALL_NUM; ii++)
		{
			tmpGenInfo.cn[ii] = atoi(vecstr[42 + ii].c_str());
		}

		VecGenBase.push_back(tmpGenInfo);
	}

	fDataFile.close();
	return 0;
}

