//////////////////////////////////////////////////////////////////////////
//
// mvtoolobj.h: interface for the CMvToolObj class.
// 2011/06/02  TODO:D5000系统下需完善测点判断
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVTOOLOBJ_H__EC0796FF_452E_4E8A_A03E_7AD99FA52079__INCLUDED_)
#define AFX_MVTOOLOBJ_H__EC0796FF_452E_4E8A_A03E_7AD99FA52079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <set>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "pubodbapi.h"
#include "pubfun.h"
#include "wamtoolstruct.h"

extern string g_ProName;  // 程序名称

//////////////////////////////////////////////////////////////////////////
class CWamToolObj : public CPubFun, public CPubOdbApi
{
public:
	CWamToolObj();
	virtual ~CWamToolObj();
public:
	int  WamToolMain();
	void Exit();
	void Error(const int nret);
	void MessShow(const char* log, const int logtype = LTINFO);
public:
	int m_DebugType;
	char m_backslash[2];
	char m_LogString[MAXBUF];
	string m_maindir;
	char g_MainDir[MAXBUF];
	WT_PARA_INFO m_WTPara;
private:
	void InitPara();   // 初始参数
	bool ReadIniFile(const string IniFileName, WT_PARA_INFO& tmpPara);
	bool CheckPara(WT_PARA_INFO& tmpPara);   // 检查参数
	int  InitProject();

	//int  InitBusInfo(vector<WT_DEV_INFO> &VecDevInfo,vector<FAC_TAB_CRE> &VecFacTab,vector<AREA_TAB_CRE> &VecAreaTab,vector<VL_TYPE_TAB_CRE> &VecVLTypeTab);
	int  InitVFDevInfo(vector<WT_DEV_INFO>& VecDevInfo);
	int  GetVFBus(vector<WT_DEV_INFO>& VecDevInfo);
	int  GetVFPara(vector<PQ_DA_CTL_PARA_TAB_CRE>& VecVFPara);

	int  InitFacInfo(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab, vector<WT_DEV_INFO>& VecSSOGen, vector<WT_DEV_INFO>& VecSSOLine, vector<WT_DEV_INFO>& VecSSOTr);
	int  GetLfoFac(vector<WT_DEV_INFO>& VecDevInfo);

	int  InitGenInfo(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab);
	int  GetLfoGen(vector<WT_DEV_INFO>& VecDevInfo);

	int  InitAclnInfo(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab);
	int  GetLfoAcln(vector<WT_DEV_INFO>& VecDevInfo);

	int  InitTrInfo(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab, vector<AREA_TAB_CRE>& VecAreaTab, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab);
	int  GetLfoTr(vector<WT_DEV_INFO>& VecDevInfo);

	int  InitAMGenInfo(vector<WT_DEV_INFO>& VecDevInfo);
	int  GetAMGen(vector<WT_DEV_INFO>& VecDevInfo);
	int  ReadAMBpaName(const string tmpFileName, vector<GN_INFO_BASE>& VecGenBase);

	int  SetYcFlag(vector<WT_DEV_INFO>& VecDevInfo, vector<FES_YX_YC>& VecFesYcTab, const int tableno);
	int  SetAddFlag(vector<WT_DEV_INFO>& VecDevInfo, const int tableno);
	int  WriteDevNew(vector<WT_DEV_INFO>& VecDevInfo, const int tableno);
	int  GetFacName(vector<WT_DEV_INFO>& VecDevInfo, vector<FAC_TAB_CRE>& VecFacTab);
	int  GetAreaName(vector<WT_DEV_INFO>& VecDevInfo, vector<AREA_TAB_CRE>& VecAreaTab);
	int  GetVBase(vector<WT_DEV_INFO>& VecDevInfo, vector<VL_TYPE_TAB_CRE>& VecVLTypeTab);
	int  WriteDevFile(vector<WT_DEV_INFO>& VecDevInfo, const int tableno);
	int  WriteToolInf(const string ToolInf);
private:
	vector<WT_DEV_INFO> m_VecVFDevInfo;
	vector<WT_DEV_INFO> m_VecFacInfo;
	vector<WT_DEV_INFO> m_VecAclnInfo;
	vector<WT_DEV_INFO> m_VecGenInfo;
	vector<WT_DEV_INFO> m_VecTrInfo;
	vector<WT_DEV_INFO> m_VecAmGenInfo;
	vector<int> m_VecColBus;
	vector<int> m_VecColGen;
	vector<int> m_VecColLn;
	vector<int> m_VecColTr;

	map<INT_REC, int>					g_map_facid;//有测点的厂站MAP
	multimap<INT_REC, int>				g_map_colno;//采样文件列号map,存放设备ID
};

#endif // !defined(AFX_MVTOOLOBJ_H__EC0796FF_452E_4E8A_A03E_7AD99FA52079__INCLUDED_)
