// pubwamsdata.cpp: implementation of the pubwamsdata class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "pubwamsdata.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPubWamsData::CPubWamsData()
{
#ifdef _WAMSDATA_
#ifndef _SIFANG
#ifndef _D5000
#ifdef  USE_NET_WAM_MEM
	#ifdef  USE_WEEK_WAM_MEM
		m_pWamsData = new CNetWamapDatabase(WEEKDATA);
	#else
		m_pWamsData = new CNetWamapDatabase(CALDATA);
	#endif
#else
	#ifdef  USE_WEEK_WAM_MEM
		m_pWamsData = new CWamapWeekDatabase(WEEKDATA);
	#else
		m_pWamsData = new CWamapDatabase(CALDATA);
	#endif
#endif

#else // #ifndef _D5000

#ifdef  USE_NET_WAM_MEM
    #ifdef  USE_WEEK_WAM_MEM
        m_pWamsData = new CNetWamapDatabase("localhost",WEEKDATA);  // 历史库网络接口
		printf("localhost,WEEKDATA\n");
    #else
        m_pWamsData = new CNetWamapDatabase("localhost",CALDATA);  // 实时库网络接口
		printf("localhost,CALDATA\n");
    #endif
#else
    #ifdef  USE_WEEK_WAM_MEM
        m_pWamsData = new CWamapDatabase("localhost",HIS_DATA);  // 历史库本地接口
		printf("localhost,HIS_DATA\n");
    #else
        m_pWamsData = new CWamapDatabase("localhost",PMU_DATA);  // 实时库本地接口
		printf("localhost,PMU_DATA\n");
    #endif
#endif
#endif  // #ifndef _D5000
#endif // !_SIFANG
#endif

	printf("CPubWamsData构造结束\n");

}

CPubWamsData::~CPubWamsData()
{
#ifdef _WAMSDATA_
#ifndef _SIFANG
	if (m_pWamsData)
	{
		delete m_pWamsData;
		m_pWamsData = NULL;
	}
#endif // !_SIFANG
#endif
}

void CPubWamsData::MessShow(const char *log, const int logtype)
{
	std::cout<<">>>"<<log<<std::endl;
	return;
}
//////////////////////////////////////////////////////////////////////////
bool CPubWamsData::PWD_GetStatus()
{
#ifdef _WAMSDATA_
#ifndef _SIFANG
	if (m_pWamsData==NULL)
	{
		sprintf(m_LogStr,"缓存区接口未初始化");
		MessShow(m_LogStr,LTERROR);
		return false;
	}
	return (m_pWamsData->GetStatus());
#endif // !_SIFANG
#endif
	return true;
}

bool CPubWamsData::PWD_TableGet(WT_VEC_TIME &ntime,int type)
{
	memset((void*)&ntime,0,sizeof(WT_VEC_TIME));
#ifdef _WAMSDATA_
#ifndef _SIFANG
	if (PWD_GetStatus()==false)
	{
		sprintf(m_LogStr,"缓存区接口调用错误");
		MessShow(m_LogStr,LTERROR);
		return false;
	}
#ifndef _D5000
	bool ret_code = m_pWamsData->TableGet(ntime,type);
#else
	bool ret_code = m_pWamsData->TableGet(ntime);
#endif
	if (ret_code==false)
	{
		sprintf(m_LogStr,"获取缓存区时间失败[%s]",m_pWamsData->GetError());
		MessShow(m_LogStr,LTERROR);
	}
	return ret_code;
#endif // !_SIFANG
#endif // _WAMSDATA_
	return true;
}

#ifndef _D5000
bool CPubWamsData::PWD_GetAppFieldByKeyId(short &appNo,short &fieldNo,const MY_KEY_ID &keyid)
{
#ifdef _WAMSDATA_
    if (PWD_GetStatus()==false)
    {
        sprintf(m_LogStr,"缓存区接口调用错误");
        MessShow(m_LogStr,LTERROR);
        return false;
    }
    if (m_pWamsData->GetAppFieldByKeyId(appNo,fieldNo,keyid)== false)
    {
        sprintf(m_LogStr,"获取keyid失败[%s]",m_pWamsData->GetError());
        MessShow(m_LogStr,LTERROR);
	return false;
    }
#endif
    return true;
}
bool CPubWamsData::PWD_GetKeyIdByAppField(MY_KEY_ID &keyid,const int &appNo,const short &fieldNo)
{
#ifdef _WAMSDATA_
    if (PWD_GetStatus()==false)
    {
        sprintf(m_LogStr,"缓存区接口调用错误");
        MessShow(m_LogStr,LTERROR);
        return false;
    }
    if (m_pWamsData->GetKeyIdByAppField(keyid,appNo,fieldNo)== false)
    {
        sprintf(m_LogStr,"获取appno,fieldno失败[%s]",m_pWamsData->GetError());
        MessShow(m_LogStr,LTERROR);
	return false;
    }
#endif
    return true;
}
bool CPubWamsData::PWD_TableGet(const char* keyPtr,time_t start,time_t end,vector<WT_VEC_TIME> &ntime,vector<PUB_WT_VEC_DATA> &vdata,int type) 
{
#ifdef _WAMSDATA_
	if (PWD_GetStatus()==false)
	{
		sprintf(m_LogStr,"缓存区接口调用错误");
		MessShow(m_LogStr,LTERROR);
		return false;
	}
	
	bool ret_code = m_pWamsData->TableGet(keyPtr,start,end,ntime,vdata,type);
	if (ret_code==false)
	{
		sprintf(m_LogStr,"获取PMU数据失败[%s],keyptr[%s],start[%d],end[%d]",m_pWamsData->GetError(),keyPtr,start,end);
		MessShow(m_LogStr,LTERROR);
		return false;
	}
#endif
	return true;
}
bool CPubWamsData::PWD_TableGet(const short &appNo,const short &fieldNo,time_t start,time_t end,PUB_WT_VEC_DATA *ptr,int &num)
{
#ifdef _WAMSDATA_
	if (PWD_GetStatus()==false)
	{
		sprintf(m_LogStr,"缓存区接口调用错误");
		MessShow(m_LogStr,LTERROR);
		return false;
	}
	
	bool ret_code = m_pWamsData->TableGet(appNo,fieldNo,start,end,ptr,num);
	if (ret_code==false)
	{
		sprintf(m_LogStr,"获取PMU数据失败[%s],appno[%d],fieldno[%d],start[%d],end[%d]",m_pWamsData->GetError(),appNo,fieldNo,start,end);
		MessShow(m_LogStr,LTERROR);
		return false;
	}
#endif
	return true;
}
bool CPubWamsData::PWD_TableGet(const char* keyPtr,time_t start,time_t end,PUB_WT_VEC_DATA *ptr,int &num)
{
#ifdef _WAMSDATA_
	if (PWD_GetStatus()==false)
	{
		sprintf(m_LogStr,"缓存区接口调用错误");
		MessShow(m_LogStr,LTERROR);
		return false;
	}
	
	bool ret_code = m_pWamsData->TableGet(keyPtr,start,end,ptr,num);
	if (ret_code==false)
	{
		sprintf(m_LogStr,"获取PMU数据失败[%s],keyptr[%s],start[%d],end[%d]",m_pWamsData->GetError(),keyPtr,start,end);
		MessShow(m_LogStr,LTERROR);
		return false;
	}
#endif
	return true;
}
bool CPubWamsData::PWD_TableGet(const short &appNo,const short &fieldNo,time_t start,time_t end,PUB_WT_VEC_DATA *ptr,int &num,WT_VEC_DESC &desc)
{
#ifdef _WAMSDATA_
	if (PWD_GetStatus()==false)
	{
		sprintf(m_LogStr,"缓存区接口调用错误");
		MessShow(m_LogStr,LTERROR);
		return false;
	}
	
	bool ret_code = m_pWamsData->TableGet(appNo,fieldNo,start,end,ptr,num,desc);
	if (ret_code==false)
	{
		sprintf(m_LogStr,"获取PMU数据失败[%s],appno[%d],fieldno[%d],start[%d],end[%d]",m_pWamsData->GetError(),appNo,fieldNo,start,end);
		MessShow(m_LogStr,LTERROR);
		return false;
	}
#endif
	return true;
}
// bool CPubWamsData::PWD_TableGet(const short &appNo,const short &fieldNo,time_t start,time_t end,float *ptr,int &num,WT_VEC_DESC &desc)
// {
// #ifdef _WAMSDATA_
// 	if (PWD_GetStatus()==false)
// 	{
// 		sprintf(m_LogStr,"缓存区接口调用错误");
// 		MessShow(m_LogStr,LTERROR);
// 		return false;
// 	}
// 	
// 	vector<WT_VEC_DATA> 
// 	bool ret_code = m_pWamsData->TableGet(appNo,fieldNo,start,end,ptr,num,desc);
// 	if (ret_code==false)
// 	{
// 		sprintf(m_LogStr,"获取PMU数据失败[%s],appno[%d],fieldno[%d],start[%d],end[%d]",m_pWamsData->GetError(),appNo,fieldNo,start,end);
// 		MessShow(m_LogStr,LTERROR);
// 	}
// 	return ret_code;
// #else
// 	return true;
// #endif
// }
bool CPubWamsData::PWD_TableGet(const char* keyPtr,time_t start,time_t end,PUB_WT_VEC_DATA *ptr,int &num,WT_VEC_DESC &desc)
{
#ifdef _WAMSDATA_
	if (PWD_GetStatus()==false)
	{
		sprintf(m_LogStr,"缓存区接口调用错误");
		MessShow(m_LogStr,LTERROR);
		return false;
	}
	
	bool ret_code = m_pWamsData->TableGet(keyPtr,start,end,ptr,num,desc);
	if (ret_code==false)
	{
		sprintf(m_LogStr,"获取PMU数据失败[%s],keyptr[%s],start[%d],end[%d]",m_pWamsData->GetError(),keyPtr,start,end);
		MessShow(m_LogStr,LTERROR);
		return false;
	}
#endif
	return true;
}
#else // !_D5000
bool CPubWamsData::PWD_TableGet(const int &addr,const WT_VEC_TIME &ntime,WT_VEC_DATA &vdata)
{
#ifdef _WAMSDATA_
#ifndef _SIFANG
	if (PWD_GetStatus() == false)
	{
		sprintf(m_LogStr, "缓存区接口调用错误");
		MessShow(m_LogStr, LTERROR);
		return false;
	}

	bool ret_code = m_pWamsData->TableGet(addr, ntime, vdata);
	if (ret_code == false)
	{
		sprintf(m_LogStr, "获取PMU数据失败[%s],addr[%d]", m_pWamsData->GetError(), addr);
		MessShow(m_LogStr, LTERROR);
		return false;
	}
#endif // !_SIFANG
#endif // _WAMSDATA_
	return true;
}
bool CPubWamsData::PWD_TableGet(const int &addr,const time_t &start,const time_t &end,WT_VEC_VALUE *pData,int &num,int interval)
{
#ifdef _WAMSDATA_
#ifdef _SIFANG
	string hostname;
	WRDB_DESCR wrdb_descr;
	// 根据应用号查找应用所在主机名
	PWD_GetHostName(hostname, AP_WAMS);
	// 打开实时库
	int retCode = WRDB_Open(hostname.c_str(), &wrdb_descr, g_ctxNo);
	if (retCode < 0) {
		sprintf(m_LogStr, "打开时序实时库失败");
		MessShow(m_LogStr, LTERROR);
		WRDB_Close(&wrdb_descr);
		return false;
	}
	
	retCode = WRDB_Read(&wrdb_descr, addr, start, end, pData, &num);
	if (retCode < 0)
	{
		int ntime_secs = PWD_GetLastTime();
		sprintf(m_LogStr, "retCode[%d],获取PMU数据失败[%s],addr[%d],start[%ld],end[%ld],num[%d]----->ntime_secs[%d]", retCode, WRDB_StrError(&wrdb_descr),addr, start, end, num, ntime_secs);
		MessShow(m_LogStr, LTERROR);
		WRDB_Close(&wrdb_descr);
		return false;
	}
	/*else {
		sprintf(m_LogStr, "retCode[%d],获取PMU数据成功,addr[%d],start[%ld],end[%ld],num[%d]",retCode, addr, start, end, num);
		MessShow(m_LogStr);
		WRDB_Close(&wrdb_descr);
		for(int i=0;i<100;i++)
		{
			sprintf(m_LogStr, "lns_test pData[i].value------->[%d]",pData[i].value);
			MessShow(m_LogStr);
		}
		return true;
	}*/
#else // _SIFANG
	if (PWD_GetStatus() == false)
	{
		sprintf(m_LogStr, "缓存区接口调用错误");
		MessShow(m_LogStr, LTERROR);
		return false;
	}

	bool ret_code = m_pWamsData->TableGet(addr, start, end, pData, num, interval);
	if (ret_code == false)
	{
		sprintf(m_LogStr, "获取PMU数据失败[%s],addr[%d],start[%ld],end[%ld]", m_pWamsData->GetError(), addr, start, end);
		MessShow(m_LogStr, LTERROR);
		return ret_code;
	}	
#endif // _SIFANG
#endif
	return true;
}
#endif  // !_D5000

int CPubWamsData::PWD_GetLastTime(WT_VEC_TIME &wttime)
{
	memset((void*)&wttime, 0, sizeof(WT_VEC_TIME));
#ifdef _SIFANG
	string hostname;
	WRDB_DESCR wrdb_descr;
	// 根据应用号查找应用所在主机名
	PWD_GetHostName(hostname, AP_WAMS);
	// 打开实时库
	int retCode = WRDB_Open(hostname.c_str(), &wrdb_descr, g_ctxNo);
	if (retCode < 0) {
		sprintf(m_LogStr, "打开时序实时库失败");
		MessShow(m_LogStr, LTERROR);
		WRDB_Close(&wrdb_descr);
		return -1;
	}

	if (WRDB_ReadLastTime(&wrdb_descr, wttime) < 0) {
		sprintf(m_LogStr, "获取当前时间失败");
		MessShow(m_LogStr, LTERROR);
		WRDB_Close(&wrdb_descr);
		return -1;
	}
	WRDB_Close(&wrdb_descr);
#else
	if( PWD_TableGet(wttime) == false )
	{
		sprintf(m_LogStr,"获取当前时间失败");
		MessShow(m_LogStr,LTERROR);
		return -1;
	}
#endif // _SIFANG	
	return 0;
}

int CPubWamsData::PWD_GetLastTime()
{
	WT_VEC_TIME ntime;
	memset((void*)&ntime, 0, sizeof(WT_VEC_TIME));
#ifdef _SIFANG
	string hostname;
	WRDB_DESCR wrdb_descr;
	// 根据应用号查找应用所在主机名
	PWD_GetHostName(hostname, AP_WAMS);
	sprintf(m_LogStr,"%s",hostname);
	MessShow(m_LogStr);
	// 打开实时库
	int retCode = WRDB_Open(hostname.c_str(), &wrdb_descr, g_ctxNo);
	if (retCode < 0) {
		sprintf(m_LogStr, "打开时序实时库失败");
		MessShow(m_LogStr, LTERROR);
		WRDB_Close(&wrdb_descr);
	       	return -1;
	}

	if (WRDB_ReadLastTime(&wrdb_descr, ntime) < 0) {
		sprintf(m_LogStr, "获取当前时间失败");
		MessShow(m_LogStr, LTERROR);
		WRDB_Close(&wrdb_descr);
		return -1;
	}
	WRDB_Close(&wrdb_descr);
	return (ntime.secs);
#else	
	if (PWD_TableGet(ntime) == false)
	{
		sprintf(m_LogStr, "获取当前时间失败");
		MessShow(m_LogStr, LTERROR);
		return -1;
	}
	return (ntime.csecond);
#endif // _SIFANG	
}

int CPubWamsData::PWD_GetLastTime(int &ntime)
{
	ntime = 0;
	WT_VEC_TIME wttime;
	memset((void *)&wttime,0,sizeof(WT_VEC_TIME));
#ifdef _SIFANG
	string hostname;
	WRDB_DESCR wrdb_descr;
	// 根据应用号查找应用所在主机名
	PWD_GetHostName(hostname, AP_WAMS);  
	// 打开实时库
	int retCode = WRDB_Open(hostname.c_str(), &wrdb_descr, g_ctxNo);
	if (retCode < 0) {
		sprintf(m_LogStr, "打开时序实时库失败");
		MessShow(m_LogStr, LTERROR);
		WRDB_Close(&wrdb_descr);
		return -1;
	}

	if (WRDB_ReadLastTime(&wrdb_descr, wttime) < 0) {
		sprintf(m_LogStr, "获取当前时间失败");
		MessShow(m_LogStr, LTERROR);
		WRDB_Close(&wrdb_descr);
		return -1;
	}
	WRDB_Close(&wrdb_descr);
	ntime = wttime.secs;
#else
	if (PWD_TableGet(wttime) == false)
	{
		sprintf(m_LogStr, "获取当前时间失败");
		MessShow(m_LogStr, LTERROR);
		return -1;
	}
	ntime = wttime.csecond;
#endif // _SIFANG
	return 0;
}

 // 获取主机名
int CPubWamsData::PWD_GetHostName(string& hostname, const int app_id /*= g_appNo*/, const int ctx_no /*= g_ctxNo*/)
{
#ifdef _SIFANG
	char tmphostname[128];
	memset(tmphostname, 0, sizeof(tmphostname));
	if (m_ServMan.RequestService(app_id, 1, tmphostname, ctx_no) > 0)
	{
		hostname = tmphostname;
		sprintf(m_LogStr, "获取主机名称成功[%s],应用[%d][%d]", hostname.c_str(), app_id, ctx_no);
		MessShow(m_LogStr, LTINFO);
		return 1;
	}
	else
	{
		sprintf(m_LogStr, "获取主机名称失败,应用[%d][%d]", app_id, ctx_no);
		MessShow(m_LogStr, LTERROR);
	}
#endif // _SIFANG
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 检查数据状态
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 
//  参数说明 : iPmudata                 输入实采量
//             nPmudata                 数据个数
//             vecSts                   质量状态                            [OUT]
bool CPubWamsData::PWD_PmuDataCheck( PUB_WT_VEC_DATA *iPmudata, const int nPmudata, PMU_DATA_DESC &vecSts)
{
    memset((void *)&vecSts, 0, sizeof(PMU_DATA_DESC));
	vecSts.begin_ok = -1;
    if ( !iPmudata ) return false;
    int ii;
	bool bad_before = false;//上一个点是否异常
	bool bad_now = false;//
	bool bad_two = false;//是否连续两个点异常

    for ( ii = 0; ii < nPmudata; ++ii )
    {
		bad_now = false;
#ifdef _D5000
		// 统计拟合值个数
        if ( iPmudata[ii].status == WS_MEAS_STS_DYN_SIMU )
        {
            vecSts.nDynSimu ++;
        }
		// 科东前置，质量位为-1
		else if (iPmudata[ii].status == WS_MEAS_STS_KEDONG )
		{
			vecSts.nKedong ++;
		}
		// 统计通道退出个数
        else if ( iPmudata[ii].status & WS_MEAS_STS_PMU_OFF)
        { 
			vecSts.nPmuOff ++;
			bad_now = true;
			if (bad_before == true)
			{
				bad_two = true;
			}
		}
		// 统计数据不可用个数
        else if ( iPmudata[ii].status & WS_MEAS_STS_DATA_BAD)
        { 
			vecSts.nDataBad ++;
			bad_now = true;
			if (bad_before == true)
			{
				bad_two = true;
			}
		}
		// 统计PMU装置异常个数
		else if ( iPmudata[ii].status & WS_MEAS_STS_PMU_BAD )
        {
            vecSts.nPmuBad ++;
			bad_now = true;
			if (bad_before == true)
			{
				bad_two = true;
			}
        }
		// 统计GPS失步个数
		else if ( iPmudata[ii].status & WS_MEAS_STS_GPS_LOST )
        {
            vecSts.nGpsLost ++;
			bad_now = true;
			if (bad_before == true)
			{
				bad_two = true;
			}
        }
		// 统计PMU数据丢帧个数
		else if ( iPmudata[ii].status & WS_MEAS_STS_DATA_LOST )
        {
            vecSts.nDataLost ++;
			bad_now = true;
			if (bad_before == true)
			{
				bad_two = true;
			}
        }
		// 统计死数据个数
		else if ( iPmudata[ii].status & WS_MEAS_STS_DATA_DEAD )
        {
            vecSts.nDataDead ++;
			bad_now = true;
			if (bad_before == true)
			{
				bad_two = true;
			}
        }
		// 统计PMU数据时差较大个数
		else if ( iPmudata[ii].status & WS_MEAS_STS_TIME_DIFF )
        {
            vecSts.nTimeDiff ++;
			bad_now = true;
			if (bad_before == true)
			{
				bad_two = true;
			}
        }
		// 统计人工设定异常数据个数
		else if ( iPmudata[ii].status & WS_MEAS_STS_MANU_BAD )
        {
            vecSts.nManu_Bad ++;
			bad_now = true;
			if (bad_before == true)
			{
				bad_two = true;
			}
		}
		if (bad_now == true) 
		{
			iPmudata[ii].status = WS_MEAS_STS_DATA_BAD;//将异常质量码统一置为WS_MEAS_STS_DATA_BAD
		}
		else
		{
			vecSts.end_ok = ii;
			if (vecSts.begin_ok < 0)
			{
				vecSts.begin_ok = ii;
			}
		}
		
#else
		// 统计拟合值个数
        if ( iPmudata[ii].quality == WS_MEAS_STS_DYN_SIMU )
        {
            vecSts.nDynSimu ++;
        }
		// 科东前置，质量位为-1
		else if (iPmudata[ii].quality == WS_MEAS_STS_KEDONG )
		{
			vecSts.nKedong ++;
		}
		// 统计通道退出个数
        else if ( iPmudata[ii].quality & WS_MEAS_STS_PMU_OFF)
        { 
			vecSts.nPmuOff ++;
		}
		// 统计数据不可用个数
        else if ( iPmudata[ii].quality & WS_MEAS_STS_DATA_BAD)
        { 
			vecSts.nDataBad ++;
		}
		// 统计PMU装置异常个数
		else if ( iPmudata[ii].quality & WS_MEAS_STS_PMU_BAD )
        {
            vecSts.nPmuBad ++;
        }
		// 统计GPS失步个数
		else if ( iPmudata[ii].quality & WS_MEAS_STS_GPS_LOST )
        {
            vecSts.nGpsLost ++;
        }
		// 统计PMU数据丢帧个数
		else if ( iPmudata[ii].quality & WS_MEAS_STS_DATA_LOST )
        {
            vecSts.nDataLost ++;
        }
		// 统计死数据个数
		else if ( iPmudata[ii].quality & WS_MEAS_STS_DATA_DEAD )
        {
            vecSts.nDataDead ++;
        }
		// 统计PMU数据时差较大个数
		else if ( iPmudata[ii].quality & WS_MEAS_STS_TIME_DIFF )
        {
            vecSts.nTimeDiff ++;
        }
#endif
		if (bad_two == true)
		{
			vecSts.bCon = 1;
		} 
		else
		{
			vecSts.bCon = 0;
		}

		vecSts.nAll = vecSts.nTimeDiff + vecSts.nDataDead + vecSts.nDataLost + vecSts.nGpsLost + vecSts.nPmuBad + vecSts.nDataBad + vecSts.nPmuOff;
    }


    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 逐桢检查1秒实采量，返回修复数据（有效桢插值修复无效帧）及数据状态
//  处理过程 : 
//  备    注 : 
//  返 回 值 : 
//  参数说明 : iPmudata                 输入实采量
//             tBgnSec                  取数时刻
//             oVecData                 返回数组
//             vecSts                   质量状态                            [OUT]
// bool  CPubWamsData::PWD_PmuDataCheck( WT_VEC_DATA *iPmudata, time_t tBgnSec, float * oVecData, VEC_DATA_STS &vecSts)
// {
//     int             nBadSeq = 0;
//     int             ii;
// 
//     if ( !iPmudata ||  !oVecData) return false;
// 
//     memset((void *)&vecSts, 0, sizeof(VEC_DATA_STS));
// 
//     int iVecBgn = MAX_FRA_PER_SEC;
//     for ( ii = 0; ii < MAX_FRA_PER_SEC; ++ii )
//     {
//         if ( !( (iPmudata[ii].quality & STA_PMU_BAD_DATA) ) )
//         { iVecBgn = ii; break; }
//     }
// 
//     int iVecEnd = -1;
//     for ( ii = MAX_FRA_PER_SEC - 1; ii > iVecBgn; --ii )// for ( ii = MAX_FRA_PER_SEC - 1; ii >= iVecBgn; --ii ) // 08-01-28 lsf
//     {
//         if ( !( iPmudata[ii].quality & STA_PMU_BAD_DATA) )
//         { iVecEnd = ii; break; }
//     }
// 
//     vecSts.nFraNum = MAX_FRA_PER_SEC;
// 
//     //  设置PMU通道退出信号
//     for ( ii = 0; ii < MAX_FRA_PER_SEC; ++ii )
//     {
//         //  2008-10-11 by zhw
//         //  拟合值  0XFFFFFFFF
//         if ( (iPmudata[ii].quality & STA_PMU_GK_OFF) && (iPmudata[ii].quality != SAT_PMU_SIMU_DATA) )
//         { vecSts.bPmuOff = 1; break; }
//     }
// 
//     //  逐桢修复并统计无效桢
//     for ( ii = 0; ii < MAX_FRA_PER_SEC; ++ii )
//     {
//         oVecData[ii] = iPmudata[ii].value;
// 
//         if ( iPmudata[ii].quality == SAT_PMU_SIMU_DATA )
//         {
//             continue;
//         }
//         //  如果是无效桢
//         if ( iPmudata[ii].quality & STA_PMU_BAD_DATA )
//         {
//             //  如果存在多个有效桢, 则无效桢用同一秒内的有效桢插值替代
//             if ( iVecBgn < iVecEnd )
//             {
//                 int i1, i2, nn;
// 
//                 if ( ii < iVecBgn )
//                 {
//                     for ( nn = ii + 1; nn <= iVecEnd; ++nn )
//                     {
//                         if ( !( iPmudata[nn].quality & STA_PMU_BAD_DATA ) ) { i1 = nn; break; }
//                     }
//                     for ( nn = i1 + 1; nn <= iVecEnd; ++nn )
//                     {
//                         if ( !( iPmudata[nn].quality & STA_PMU_BAD_DATA ) ) { i2 = nn; break; }
//                     }
//                 }
//                 else if ( ii > iVecEnd )
//                 {
//                     for ( nn = ii - 1; nn >= iVecBgn; --nn )
//                     {
//                         if ( !( iPmudata[nn].quality & STA_PMU_BAD_DATA ) ) { i2 = nn; break; }
//                     }
//                     for ( nn = i2 - 1; nn >= iVecBgn; --nn )
//                     {
//                         if ( !( iPmudata[nn].quality & STA_PMU_BAD_DATA ) ) { i1 = nn; break; }
//                     }
//                 }
//                 else
//                 {
//                     for ( nn = ii - 1; nn >= iVecBgn; --nn )
//                     {
//                         if ( !( iPmudata[nn].quality & STA_PMU_BAD_DATA ) ) { i1 = nn; break; }
//                     }
//                     for ( nn = i1 + 1; nn <= iVecEnd; ++nn )
//                     {
//                         if ( !( iPmudata[nn].quality & STA_PMU_BAD_DATA ) ) { i2 = nn; break; }
//                     }
//                 }
// 
//                 oVecData[ii] = iPmudata[i1].value + ( iPmudata[i2].value - iPmudata[i1].value ) * ( ii - i1 ) / ( i2 - i1 );
//             }
//             //  如果存在一个有效桢, 则无效桢用同一秒内的有效桢直接替代
//             else if ( iVecBgn == iVecEnd )
//             {
//                 oVecData[ii] = iPmudata[iVecBgn].value;
//             }
// 
//             nBadSeq ++;
// 
//             vecSts.nBadNum ++;
//             vecSts.tBadSec = tBgnSec;
//             vecSts.iBadFra = ii;
//         }
//         else if ( nBadSeq > 0 )
//         {
//             if ( nBadSeq > vecSts.nBadSeq ) vecSts.nBadSeq = nBadSeq;
// 
//             nBadSeq = 0;
//         }
//     }
// 
//     if ( nBadSeq > vecSts.nBadSeq ) vecSts.nBadSeq = nBadSeq;
// 
//     return true;
// }






