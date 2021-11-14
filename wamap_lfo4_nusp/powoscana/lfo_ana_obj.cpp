// lfo_ana_obj.cpp: implementation of the CLfoAnaObj class.
//
//////////////////////////////////////////////////////////////////////

#include "lfo_ana_obj.h"
#include "LFOAnaByCurve.h"
#include <math.h>


extern CLfoAnaObj* g_pAnaObj;
#ifdef _D5000
// extern 	CFileOpSop g_sop_fileop;  //lns_20210811
#endif

CLFOAnaByCurve g_lfoAnabycurve;
CLFOAnaByCurve                g_theLfoAna;

extern char g_ProName[];
extern char g_Version[];
extern string g_dir;
extern void g_PubLogMessShow(const char* log, const int logtype = LTINFO);
extern int g_Thread_nun;
extern int b_end_proc;
int file_end_num = 0;//案例文件读取完毕次数
struct Pos_Time
{
	int pos;
	int start_time;
};
vector<Pos_Time> g_vec_pos_time;
extern long g_pos_threadid;//需要记录文件位置的线程ID

/*HisdbAccess hisdb_access(D5000_HISDB_PORT);*/
//////////////////////////////////////////////////////////////////////////
bool g_SortModeByEny(LFO_MODE_INFO mode1, LFO_MODE_INFO mode2)
{
	if (mode1.engy > mode2.engy)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool g_SortDevModeByEngy(LFO_DEV_MODE mode1, LFO_DEV_MODE mode2)
{
	if (mode1.mode.engy > mode2.mode.engy)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool g_SortDevByMaxampl(LFO_DEV_INFO mode1, LFO_DEV_INFO mode2)
{
	if (mode1.curve.max_ampl > mode2.curve.max_ampl)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool g_SortModeShapeByEngy(LFO_MODE_SHAPE mode1, LFO_MODE_SHAPE mode2)
{
	if (mode1.mode.stat != mode2.mode.stat)
	{
		if (mode1.mode.stat > mode2.mode.stat)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (mode1.mode.engy > mode2.mode.engy)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
//振荡源设备按平均耗散功率由大到小排序
bool g_SortDevByPow(LFO_DEV_INFO dev1, LFO_DEV_INFO dev2)
{
	if (dev1.mode.pow > dev2.mode.pow)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool g_SortModeShapeBystatEngy(LFO_MODE_SHAPE mode1, LFO_MODE_SHAPE mode2)
{
	if (mode1.mode.engy_pre_time > mode2.mode.engy_pre_time)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//模式按阻尼比由小到大排序
bool g_SortModeShapeByDamp(LFO_MODE_SHAPE mode1, LFO_MODE_SHAPE mode2)
{
	if (mode1.mode.damp < mode2.mode.damp)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool g_SortDevByEngy(LFO_DEV_INFO dev1, LFO_DEV_INFO dev2)
{
	if (dev1.mode.engy > dev2.mode.engy)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool g_SortIntlineModeModeByEngy(LFO_MODE_INFO mode1, LFO_MODE_INFO mode2)
{
	if (mode1.engy > mode2.engy)
	{
		return true;
	}
	else
	{
		return false;
	}
}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 厂站分析结果 ampl由大到小
////////////////////////////////////////////////////////////////////////////////
extern "C" int FcAnaResCompareByAmpl(const void* p1, const void* p2)
{
	if (((LFO_FC_ANA*)p1)->lfo_v_ampl != ((LFO_FC_ANA*)p2)->lfo_v_ampl)
	{
		return ((LFO_FC_ANA*)p1)->lfo_v_ampl > ((LFO_FC_ANA*)p2)->lfo_v_ampl ? 1 : -1;
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 排序比较函数 engy由大到小  engy相等则ampl由大到小
////////////////////////////////////////////////////////////////////////////////
extern "C" int g_IntlineModeCompareByEngy(const void* p1, const void* p2)
{
	if (((LFO_MODE_INFO*)p1)->engy != ((LFO_MODE_INFO*)p2)->engy)
	{
		return ((LFO_MODE_INFO*)p1)->engy < ((LFO_MODE_INFO*)p2)->engy ? 1 : -1;
	}

	return ((PRONY_MODE_CHARA*)p1)->ampl < ((PRONY_MODE_CHARA*)p2)->ampl ? 1 : -1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLfoAnaBase::CLfoAnaBase()
{

#ifdef _WINDOWS32
	sprintf(m_backslash, "\\");
#else
	sprintf(m_backslash, "/");
#endif
	m_maindir = "..";
	m_DebugType = LTDEBUG;

	// 初始化参数
	m_DebugType = LTDEBUG;
	m_LfoPara.freq_min = 0.03f;
	m_LfoPara.freq_max = 8.0f;
	m_LfoPara.lfo_freq_min = 0.2f;
	m_LfoPara.lfo_freq_max = 2.5f;
	m_LfoPara.time_leng = 10;
	m_LfoPara.time_leng_coe = 2.0f;
	m_LfoPara.sample_freq = 6.25f;
	m_LfoPara.sample_freq_coe = 5.0f;
	m_LfoPara.int_time = 5;
	m_LfoPara.int_time_coe = 0.5f;
	m_LfoPara.gn_wn_ampl = 10.0f;
	m_LfoPara.gn_wn2_ampl = 15.0f;
	m_LfoPara.ln_wn_ampl = 25.0f;
	m_LfoPara.ln_wn2_ampl = 50.0f;
	m_LfoPara.tr_wn_ampl = 25.0f;
	m_LfoPara.tr_wn2_ampl = 50.0f;
	m_LfoPara.stor_damp = 30.0f;
	m_LfoPara.lack_damp = 5.0f;
	m_LfoPara.flfo_damp = 5.0f;
	m_LfoPara.lfo_damp = 3.0f;
	m_LfoPara.damp_min = -20.0f;
	m_LfoPara.fjud_wnap_per = 60.0f;
	m_LfoPara.ana_ampl_per = 30.0f;
	m_LfoPara.lfo_jud_num = 2;
	m_LfoPara.warn_time_max = 25;
	m_LfoPara.safe_time_max = 25;
	m_LfoPara.delay_time = 2;
	m_LfoPara.freq_diff = 0.05f;
	m_LfoPara.phas_diff = 15.0f;
	m_LfoPara.cache_windows_num = 10;
	sprintf(m_LfoPara.devfile_name, "%s", "DeviceInfo.xml");
	sprintf(m_LfoPara.datafile_name, "%s", "Datafile.csv");
	sprintf(m_LfoPara.devicefile_name, "%s", "device.txt");
	m_LfoPara.stor_ampl = 0.1f;
	m_LfoPara.data_pos = 0;
	m_LfoPara.data_pos_old = 0;
	m_LfoPara.data_pos_osc = 0;
	m_LfoPara.data_source = 0;
	m_LfoPara.devinfo_source = 0;
	m_LfoPara.is_mon_lfo = 0;
	m_LfoPara.time_leng_min = 5;  // 时间窗口最小值
	m_LfoPara.time_leng_max = 40;  // 时间窗口最大值
	m_LfoPara.sample_freq_min = 6.25;  // 采样频率最小值
	m_LfoPara.sample_freq_max = 25.0;  // 采样频率最大值
	m_LfoPara.int_time_min = 0;  // 交叠窗口最小值
	m_LfoPara.int_time_max = 40;  // 交叠窗口最大值
	m_LfoPara.mode_save_amplper = 30.0;
	m_LfoPara.is_add_facname = 1;
	m_LfoPara.is_save_casefile = 1;
	m_LfoPara.is_save_devinfo = 1;
	m_LfoPara.is_cycle = 1;
	m_LfoPara.is_lfo_curve = 1;
	m_LfoPara.rcd_aft_t = 30;
	m_LfoPara.rcd_aft_t = 30;
	m_LfoPara.is_warn = 1;
	m_LfoPara.is_tmd = 1;
	m_LfoPara.is_triems = 0;
	m_LfoPara.warn_app_no = AP_SCADA;
	m_LfoPara.is_eng = 0;
	m_LfoPara.cohcy_angl = COHCY_ANGL;	//  同调角度差
	m_LfoPara.sh_fact = 0.1f;	//  参与因子阀值
	m_LfoPara.sh_gn_pow = 0.5f;	//机组耗散功率阀值
	m_LfoPara.sh_ln_pow = 1.5f;//支路耗散功率阀值
	m_LfoPara.sh_phas = 30; //相位超前阀值
	m_LfoPara.volt_thre1 = 220;						//电压等级1	
	m_LfoPara.volt_thre2 = 500;						//电压等级2
	m_LfoPara.ampl_thre1 = 20.0;
	m_LfoPara.ampl_thre2 = 25.0;
	m_LfoPara.ampl_thre1 = 20.0;
	m_LfoPara.ampl_thre2 = 25.0;
	m_LfoPara.ampl_thre3 = 35.0;
	m_LfoPara.gn_wn_ampl1 = 10.0;
	m_LfoPara.ln_wn_ampl1 = 25.0;
	m_LfoPara.tr_wn_ampl1 = 25.0;
	m_LfoPara.ln_swn_ampl1 = 20.0;
	m_LfoPara.ln_swn_ampl2 = 25.0;
	m_LfoPara.ln_swn_ampl3 = 35.0;
	m_LfoPara.ln_ewn_ampl1 = 20.0;
	m_LfoPara.ln_ewn_ampl2 = 25.0;
	m_LfoPara.ln_ewn_ampl3 = 35.0;
	m_LfoPara.tr_swn_ampl1 = 20.0;
	m_LfoPara.tr_swn_ampl2 = 25.0;
	m_LfoPara.tr_swn_ampl3 = 35.0;
	m_LfoPara.tr_ewn_ampl1 = 20.0;
	m_LfoPara.tr_ewn_ampl2 = 25.0;
	m_LfoPara.tr_ewn_ampl3 = 35.0;
	m_LfoPara.osc_pow_ampl = 10.0;
	m_LfoPara.osc_sec_ampl = 10.0;
	m_LfoPara.rec_num = 2;
	m_LfoPara.if_flit_lfo = 1;
	m_LfoPara.Voltunit_is_kV = 1;
	m_LfoPara.pre_freq1 = 0.05;
	m_LfoPara.pre_freq2 = 0.1;
	m_LfoPara.pre_freq3 = 0.2;
	m_LfoPara.pre_freq4 = 2.5;
	m_LfoPara.pre_freq5 = 8.0;
	m_LfoPara.curve_time_60 = 0;
	m_LfoPara.para_Ampl_aver = 0.6;
	m_LfoPara.thread_mode = 0;
	m_LfoPara.mainmode_num = 3;
	m_LfoPara.inf_ampl = 0.8;
	m_LfoPara.para_VoltAmplPer = 0.6;
	m_LfoPara.para_LfoAnaByPwr = 1;
	m_LfoPara.para_nShwMaxFcNum = 5;
	m_LfoPara.center_volt_ampl = 10.0;
	m_LfoPara.disp_num = 3;
	m_LfoPara.gn_capacity1 = 200.0;
	m_LfoPara.gn_capacity2 = 600.0;
	m_LfoPara.gn_swn_ampl2 = 10.0;
	m_LfoPara.gn_swn_ampl3 = 12.0;
	m_LfoPara.gn_ewn_ampl2 = 10.0;
	m_LfoPara.gn_ewn_ampl3 = 12.0;

	memset((void*)&m_LfoPara.prony_para, 0, sizeof(PRONY_CALC_PARA));
	m_LfoPara.prony_para.if_set_rank = 0;//0;   // 0 自动确定阶数； 1 人为设定阶数
	m_LfoPara.prony_para.rank = 30;
	m_LfoPara.prony_para.queue_type = -2;      // 0 幅值排序; -1 预测幅值排序;-2 预测能量排序
	m_LfoPara.prony_para.delDC = 1;       // 是否去除直流(0 否; 1 是)
	m_LfoPara.prony_para.freq_min = 0.02f;   //2008-03-11
	m_LfoPara.prony_para.freq_max = 100.0f;  // 3.0f;
	m_LfoPara.prony_para.minAbsAmpl = 0.001f;  // 0.00001f;
	m_LfoPara.prony_para.stor_lim_ampl = 0;       // 0.08f;
	m_LfoPara.prony_para.stor_lim_damp = 100;
	m_LfoPara.prony_para.snr_min = 20;      // 30;40;
	m_LfoPara.prony_para.eng_time = 2.0f;
	m_LfoPara.prony_para.max_iter = 3;
	m_LfoPara.prony_para.is_filter = 0;

	printf("CLfoAnaBase构造结束\n");

}

CLfoAnaBase::~CLfoAnaBase()
{
}

BOOL CLfoAnaBase::OnTask(LPVOID lpv)
{
	sprintf(m_LogString, "OnTask( LPVOID lpv )-MonitorMainProc");
	MessShow(m_LogString, LTINFO);
	MonitorMainProc();
	return TRUE;
}

BOOL CLfoAnaBase::OnTask()
{
	sprintf(m_LogString, "OnTask-MonitorMainProc");
	MessShow(m_LogString, LTINFO);
	MonitorMainProc();
	return TRUE;
}

int CLfoAnaBase::Error(const int nret)
{

	return 0;
}

int CLfoAnaBase::Exit()
{
	return 0;
}

void CLfoAnaBase::MessShow(const char* logstr, const int logtype)
{
	PLF_MessShow(logstr, logtype);
	return;
}
//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    MonitorMainProc
// FullName:  CLfoAnaBase::MonitorMainProc
// Access:    public 
// Returns:   int
// Qualifier:
// 监视线程主程序
//************************************
int CLfoAnaBase::MonitorMainProc()
{
	struct  timeb   t_StartTime;
	struct  timeb   t_EndTime;
	int getdata_err = 0;//获取数据失败后是否告警标志
	int readdata_err = 0;
	int readdata_err_num = 0;//连续读数据失败次数
	sprintf(m_LogString, "********%s Ver %s build at %s %s 监视线程[%ld]启动********", g_ProName, g_Version, __DATE__, __TIME__, GetId());
	MessShow(m_LogString, LTINFO);

	// 主循环
	while (true)
	{
		sprintf(m_LogString, "开始一轮新的计算<%lu>", GetId());
		MessShow(m_LogString, LTINFO);
		//20180817

		ShowProMem("开始一轮新的计算");
		readdata_err = 0;
		t_StartTime = PT_GetNowMilliSeconds();
		t_EndTime = t_StartTime;
		//     5.1	初始化
		// TODO:设备信息变化时重新初始化
		if (InitProc() < 0)
		{
			continue;
		}

		ShowProMem("InitProc");
		m_LfoPara.change_dev_num = 0;
		m_LfoPara.change_data_source = 0;
		//     5.2	获取计算参数,设备控制参数
		if (GetMonDev() < 0)
		{
			continue;
		}

		if (m_LfoPara.change_data_source == 1)
		{
			//数据源切换,退出子线程
			sprintf(m_LogString, "数据源切换,退出子线程");
			MessShow(m_LogString, LTINFO);
			break;
		}
		if (m_LfoPara.change_dev_num == 1 && m_MonSys.stat_old <= MENU_LFO_DEV_STS_SCT)
		{//设备表数量变化且系统未处于告警状态下,退出子线程
			sprintf(m_LogString, "设备表数量变化,当前未告警,退出线程");
			MessShow(m_LogString, LTINFO);
			break;
		}
		if (LFO_DATA_SOURCE_REALTTIME != m_LfoPara.data_source && m_MonSys.file_end == 1)
		{
			sprintf(m_LogString, "案例数据读取完毕");
			MessShow(m_LogString, LTINFO);
			PT_DelayMinSec(1000);
			continue;
		}

		//     5.3	获取数据
#ifdef _SIFANG
		g_pAnaObj->m_mutex.Lock();  //lns at jb
		int lns_GetData = GetData();
		g_pAnaObj->m_mutex.Unlock();  //lns at jb
		if (lns_GetData < 0)
#else
		if (GetData() < 0)
#endif // _SIFANG
		{
			readdata_err_num++;//读数据失败时，加1
			if (LFO_DATA_SOURCE_REALTTIME == m_LfoPara.data_source && readdata_err_num > 5)
			{
				sprintf(m_LogString, "读取PMU实时数据次数达到%d次,线程即将退出", readdata_err_num);
				MessShow(m_LogString, LTERROR);
				b_end_proc = 1;
				break;
			}
			else
			{
				InitProc();
			}

			for (int ii = 0; ii < (int)m_VecDev.size(); ii++)
			{//获取数据失败时，初始化判断状态结构体osciFlag
				memset((void*)&m_VecDev[ii].osciFlag, 0, sizeof(OSCILLAT_FLAG));
				m_VecDev[ii].osciFlag.obj_id = m_VecDev[ii].dev.dev_id;
			}

			readdata_err = 1;
			if (getdata_err == 0)//首次出现获取数据失败时，置标志为1
			{
				getdata_err = 1;
			}
			// 			g_pAnaObj->m_mutex.Lock();
			// 			if (g_pAnaObj->ThreadCallBack_ERR(m_MonSys,m_LfoPara,m_ThreadIdx,getdata_err)<0)
			// 			{
			// 				sprintf(m_LogString,"线程回调函数错误");	
			// 				MessShow(m_LogString,LTERROR);
			// 				continue;
			// 			}
			getdata_err = 2;//运行完ThreadCallBack_ERR后将标志置为2，只首次出现获取数据失败时告警
// 			g_pAnaObj->m_mutex.Unlock();
			PT_DelayMinSec(1000);
			ShowProMem("文件读完");
			//			continue;
		}
		//		getdata_err = 0;//获取数据成功时，将标志置为0
		//  		sprintf(m_LogString,"readdata_err=[%d]",readdata_err);
		//  		MessShow(m_LogString,LTINFO);
		ShowProMem("GetData之后");
		if (0 == readdata_err)
		{
			readdata_err_num = 0;//读数据成功时,复位为0
			//     5.4	数据预处理
			if (DataPretreat() < 0)
			{
				continue;
			}
			ShowProMem("数据预处理");
			//     5.5	Prony分析主线程
			if (PronyAnaMain() < 0)
			{
				continue;
			}
			ShowProMem("Prony分析主线程");
			//     5.6	监视信息处理
			if (FormMonResult() < 0)
			{
				continue;
			}
			ShowProMem("监视信息处理");

			if (m_MonSys.stat >= MENU_LFO_DEV_STS_WRN)
			{
				sprintf(m_LogString, "开始振荡源定位");
				MessShow(m_LogString);
				if (LocateLfoSource(m_VecDev, m_MonSys) < 0)
				{
					//        return -1;
				}
				ShowProMem("振荡源定位");
				if (AnaPretreat() < 0)
				{
					continue;
				}

				ShowProMem("AnaPretreat");
				m_nCases = 0;
				m_pCases = (CASE_TYPE*)calloc(para_nCaseMaxMem, sizeof(CASE_TYPE));
				memset((void*)m_pCases, 0, para_nCaseMaxMem * sizeof(CASE_TYPE));

				ShowProMem("申请m_pCases内存");

				if (PronyAna() < 0)
				{
					if (m_pCases) { free(m_pCases);   m_pCases = NULL; }

					continue;
				}
				ShowProMem("PronyAna");

				AnaCalcProcess();

				if (m_pCases) { free(m_pCases);   m_pCases = NULL; }
				ShowProMem("AnaCalcProcess");
			}
			m_df.ClearData();
		}
		// 		sprintf(m_LogString,"【yjj】线程回调函数开始,进入UpdateStatOld[%d][%d]",m_MonSys.stat_old,m_MonSys.stat);
		// 		MessShow(m_LogString,LTINFO);
				// 下面函数转移到CLfoAnaObj类中

		g_pAnaObj->m_mutex.Lock();
		sprintf(m_LogString, "线程[%d],设备数[%d],模式数[%d],主导设备[%s],阻尼比[%f]", m_ThreadIdx + 1, m_MonSys.vec_dev.size(), m_MonSys.vec_modeshape.size(), m_MonSys.lead_dev.dev.dev_name, m_MonSys.lead_dev.mode.damp);
		MessShow(m_LogString, LTERROR);
		ShowProMem("线程回调");
		if (g_pAnaObj->ThreadCallBack(m_VecDev, m_VecModeShape, m_MonSys, m_LfoPara, m_ThreadIdx, m_oscMonSys) < 0)
		{
			sprintf(m_LogString, "线程回调函数错误");
			MessShow(m_LogString, LTERROR);
			continue;
		}
		getdata_err = 0;//获取数据成功时，将标志置为0
		g_pAnaObj->m_mutex.Unlock();
		// 保存本轮状态
// 	sprintf(m_LogString,"【yjj】线程回调函数结束2,进入UpdateStatOld[%d][%d]",m_MonSys.stat_old,m_MonSys.stat);
// 	MessShow(m_LogString,LTINFO);

		UpdateStatOld(m_VecDev, m_MonSys, m_oscMonSys);
		t_EndTime = PT_GetNowMilliSeconds();
		sprintf(m_LogString, "本轮计算耗时[%.3f]秒", PT_CalculateTimeInterval(t_StartTime, t_EndTime) / 1000.0f);
		MessShow(m_LogString, LTINFO);

		//		sprintf(m_LogString,"【yjj】退出UpdateStatOld");
		//		MessShow(m_LogString,LTINFO);
	}
	sprintf(m_LogString, "监视线程[%ld]退出", GetId());
	MessShow(m_LogString, LTINFO);
	g_PubLogMessShow(m_LogString, LTINFO);
	return 0;
}

//************************************
// Method:    InitProc
// FullName:  CLfoAnaBase::InitProc
// Access:    public 
// Returns:   int
// Qualifier:
// 清空监视信息结构体(LFO_DEV_INFO m_VecMonFac, m_VecMonGen, m_VecMonLine, m_VecMonTr)
// 根据控制参数确定设备基本信息 (LFO_DEV_BASE)。
// 输入：LFO设备表中控制参数，设备初始化信息(vector<WT_DEV_INFO> m_VecFac, m_VecGen, m_VecLine, m_VecTr)，vector<LFO_DEV_INFO> m_VecMonDev;
// 输出：填充vector<LFO_DEV_INFO> m_VecMonDev基本信息(LFO_DEV_BASE)结构
//************************************
int CLfoAnaBase::InitProc()
{
	// 清空监视信息结构体
	int ii, tmpstat;
	// 	LFO_DEV_BASE tmpdev;  // 设备基本信息
	//  WT_DEV_INFO tmp_dev_info;  // 设备静态信息
	// 	int tmp_start_time_lfo;
	// 	int tmp_end_time_lfo;
	// 	int tmp_hold_time_lfo;
	//  int tmp_start_time;
	//  int tmp_end_time;
	// 	int tmp_hold_time;

	for (ii = 0; ii < m_VecDev.size(); ii++)
	{
		m_VecDev[ii].stat = MENU_LFO_DEV_STS_OK;
		if (m_VecDev[ii].stat_old <= 0)
		{
			m_VecDev[ii].stat_old = MENU_LFO_DEV_STS_SCT;
		}
		if (!m_VecDev[ii].v2d_data.empty())
		{
			vector<float>().swap(m_VecDev[ii].v2d_data);
			m_VecDev[ii].v2d_data.clear();
			vector<float>().swap(m_VecDev[ii].v2d_data);
		}

		if (!m_VecDev[ii].ana_data.empty())
		{
			vector<float>().swap(m_VecDev[ii].ana_data);
			m_VecDev[ii].ana_data.clear();
			vector<float>().swap(m_VecDev[ii].ana_data);
		}

		if (!m_VecDev[ii].back_data.empty())
		{
			vector<float>().swap(m_VecDev[ii].back_data);
			m_VecDev[ii].back_data.clear();
			vector<float>().swap(m_VecDev[ii].back_data);
		}

		memset((void*)&m_VecDev[ii].curve, 0, sizeof(LFO_CURVE_INFO));
		memset((void*)&m_VecDev[ii].curve_ana, 0, sizeof(LFO_CURVE_INFO));
		// 		memset((void *)&m_VecDev[ii].osciFlag,0,sizeof(OSCILLAT_FLAG));		

		if (!m_VecDev[ii].vec_mode.empty())
		{
			ClearLfoModeInfo(m_VecDev[ii].vec_mode);
		}

		ClearLfoModeInfo(m_VecDev[ii].mode);
		ClearLfoModeInfo(m_VecDev[ii].mode_shape);

		if (!m_VecDev[ii].vec_lead_mode.empty())
		{
			ClearLfoModeInfo(m_VecDev[ii].vec_lead_mode);
		}
	}
	// 	LFO_DEV_INFO tmpDevInfo;
	// 	vector<LFO_DEV_INFO> tmpVecDevInfo;
	// 	tmpVecDevInfo.reserve(m_VecDev.size());
	// 	for ( ii = 0;ii < m_VecDev.size();ii++)
	// 	{
	// 		tmpVecDevInfo.push_back(m_VecDev[ii]);
	// 	}
	// 	if (!m_VecDev.empty())
	// 	{
	// 		ClearLfoDev(m_VecDev);
	// 	}
	// 
	// 	for ( ii =0; ii<(int)tmpVecDevInfo.size(); ii++)
	// 	{
	// 		ClearLfoDev(tmpDevInfo);
	// 		tmpDevInfo.dev = tmpVecDevInfo[ii].dev;
	// 		tmpDevInfo.dev_info = tmpVecDevInfo[ii].dev_info;
	// 		tmpDevInfo.stat = MENU_LFO_DEV_STS_OK;
	// // 		tmpDevInfo.osc_stat = MENU_LFO_DEV_STS_OK;
	// 		if (tmpVecDevInfo[ii].stat_old <=0)
	// 		{
	// 			tmpDevInfo.stat_old = MENU_LFO_DEV_STS_SCT;
	// 		}
	// 		else
	// 		{
	// 			tmpDevInfo.stat_old = tmpVecDevInfo[ii].stat_old;
	// 		}
	// // 		tmpDevInfo.start_time_lfo = tmpVecDevInfo[ii].start_time_lfo;
	// // 		tmpDevInfo.end_time_lfo = tmpVecDevInfo[ii].end_time_lfo;
	// // 		tmpDevInfo.hold_time_lfo = tmpVecDevInfo[ii].hold_time_lfo;
	// 		tmpDevInfo.start_time = tmpVecDevInfo[ii].start_time;
	// 		tmpDevInfo.end_time = tmpVecDevInfo[ii].end_time;
	// 		tmpDevInfo.hold_time = tmpVecDevInfo[ii].hold_time;
	// 
	// 		tmpDevInfo.judge_num = tmpVecDevInfo[ii].judge_num;//20160116
	// 
	// // 		if (tmpVecDevInfo[ii].osc_stat_old <=0)
	// // 		{
	// // 			tmpDevInfo.osc_stat_old = MENU_LFO_DEV_STS_SCT;
	// // 		}
	// // 		else
	// // 		{
	// // 			tmpDevInfo.osc_stat_old = tmpVecDevInfo[ii].osc_stat_old;
	// // 		}
	// // 		tmpDevInfo.osc_start_time = tmpVecDevInfo[ii].osc_start_time;
	// // 		tmpDevInfo.osc_end_time = tmpVecDevInfo[ii].osc_end_time;
	// // 		tmpDevInfo.osc_hold_time = tmpVecDevInfo[ii].osc_hold_time;
	// 		tmpDevInfo.osciFlag = tmpVecDevInfo[ii].osciFlag;
	// 		m_VecDev.push_back(tmpDevInfo);
	// 	}
	// 
	// 	ClearLfoDev(tmpDevInfo);
	// 	ClearLfoDev(tmpVecDevInfo);

	for (ii = 0; ii < m_VecIntline.size(); ii++)
	{
		if (!m_VecIntline[ii].vec_mode.empty())
		{
			ClearLfoModeInfo(m_VecIntline[ii].vec_mode);
		}
		if (!m_VecIntline[ii].vec_lead_mode.empty())
		{
			ClearLfoModeInfo(m_VecIntline[ii].vec_lead_mode);
		}
	}

	// 清空系统状态结构
	// 保存系统上一轮状态
	int tmp_gps_time, tmp_save_flag, file_end;
	tmp_gps_time = m_MonSys.gps_time_old;
	tmpstat = m_MonSys.stat_old;
	tmp_save_flag = m_MonSys.save_flag;
	file_end = m_MonSys.file_end;
	ClearLfoSysInfo(m_MonSys);
	// 保存上一轮状态
	if (tmpstat <= 0)
	{
		// 初次进入初始化
		m_MonSys.gps_time_old = 0;
		m_MonSys.stat_old = MENU_LFO_DEV_STS_SCT;
		m_MonSys.save_flag = LFO_SAVE_FLAG_IDEL;
	}
	else
	{
		m_MonSys.gps_time_old = tmp_gps_time;
		m_MonSys.stat_old = tmpstat;
		m_MonSys.save_flag = tmp_save_flag;
	}
	m_MonSys.freq_min = m_LfoPara.freq_min;
	m_MonSys.freq_max = m_LfoPara.freq_max;
	m_MonSys.sample_org = MAX_FRA_PER_SEC;
	m_MonSys.file_end = file_end;

	sprintf(m_LogString, "保存系统本轮状态[%d],上一轮状态[%d]", m_MonSys.stat, m_MonSys.stat_old);
	MessShow(m_LogString, LTINFO);

	// 清空系统小扰动状态结构
	// 保存系统小扰动上一轮状态
	tmp_gps_time = m_oscMonSys.gps_time_old;
	tmpstat = m_oscMonSys.stat_old;
	tmp_save_flag = m_oscMonSys.save_flag;
	ClearLfoSysInfo(m_oscMonSys);
	// 保存上一轮状态
	if (tmpstat <= 0)
	{
		// 初次进入初始化
		m_oscMonSys.gps_time_old = 0;
		m_oscMonSys.stat_old = MENU_LFO_DEV_STS_SCT;
		m_oscMonSys.save_flag = LFO_SAVE_FLAG_IDEL;
	}
	else
	{
		m_oscMonSys.gps_time_old = tmp_gps_time;
		m_oscMonSys.stat_old = tmpstat;
		m_oscMonSys.save_flag = tmp_save_flag;
	}
	// 清空模态信息
	ClearLfoModeShape(m_VecModeShape);

	return 0;
}

int CLfoAnaBase::GetMonDev()
{
	if (GetLfoPara(m_LfoPara, 1) < 0)
	{
		sprintf(m_LogString, "参数表读取失败,采用默认参数");
		MessShow(m_LogString, LTERROR);
	}

	// 计算参数输出
	CalcPara(m_LfoPara);

	if (CheckPara(m_LfoPara, -1) < 0)
	{
		sprintf(m_LogString, "参数检查错误,采用默认参数");
		MessShow(m_LogString, LTERROR);
	}
	if (m_LfoPara.change_data_source == 1)
	{
		// 		 if (g_pAnaObj->InitDevInfo(m_VecDev)<0)
		// 	     {
		// 			 sprintf(m_LogString,"设备静态信息读取错误");
		// 			 MessShow(m_LogString,LTERROR);
		// 			 return -1;
		// 	     }
	}

	if (GetDevCtlInfo(m_VecDev) < 0)
	{
		sprintf(m_LogString, "获取设备控制信息失败");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	return 0;
}

int CLfoAnaBase::GetData()
{
	// 	sprintf(m_LogString,"[yjj]GetData PMU数据源[%d]",m_LfoPara.data_source);
	// 	MessShow(m_LogString,LTINFO);
	
	if (LFO_DATA_SOURCE_REALTTIME == m_LfoPara.data_source)
	{
		//g_pAnaObj->m_mutex.Lock();  //lns at jb
		if (GetTimeFromDB(m_MonSys) < 0)
		{
			sprintf(m_LogString, "PMU时间获取失败");
			MessShow(m_LogString, LTERROR);
			return -1;
		}
		else
		{
			m_oscMonSys.start_time = m_MonSys.start_time;
			m_oscMonSys.end_time = m_MonSys.end_time;
			m_oscMonSys.sample_org = m_MonSys.sample_org;
		}

		if (GetDataFromDB(m_MonSys, m_VecDev) < 0)
		{
			sprintf(m_LogString, "PMU数据获取失败");
			MessShow(m_LogString, LTERROR);
			return -1;
		}
		//g_pAnaObj->m_mutex.Unlock();  //lns at jb
	}
	else
	{
		if (m_LfoPara.is_cycle == 0 && m_LfoPara.data_pos < 0)
		{
			// 不循环，案例数据读取完毕
			sprintf(m_LogString, "案例数据读取完毕");
			MessShow(m_LogString, LTINFO);
			return -1;
		}
		if (m_MonSys.file_end == 1)
		{
			sprintf(m_LogString, "案例数据读取完毕");
			MessShow(m_LogString, LTINFO);
			return -1;
		}
		// 从数据文件中读取PMU数据
		{
			// 			CDataFile df;
			string tmpfilename;
			Pos_Time tmp_pos_time;
			FormDataFileName(tmpfilename);
			// 			g_pAnaObj->m_mutex.Lock();
			sprintf(m_LogString, "读取设备数据[%s]", tmpfilename.c_str());
			MessShow(m_LogString, LTINFO);

			m_LfoPara.data_pos_old = m_LfoPara.data_pos;

			//  		sprintf(m_LogString,"[yjj]设备点数old[%d],now[%d]",m_LfoPara.data_pos_old,m_LfoPara.data_pos);
			//  		MessShow(m_LogString,LTINFO);
// 			m_df.ClearData();
			ShowProMem("读文件之前");
			if (m_df.ReadFileData(tmpfilename.c_str(), m_LfoPara.data_pos, m_LfoPara.time_leng, m_LfoPara.int_time) == false)
			{
				sprintf(m_LogString, "读取数据文件失败[%s][%s],文件位置[%d]", tmpfilename.c_str(), m_df.GetLastError(), m_LfoPara.data_pos);
				MessShow(m_LogString, LTERROR);
				ShowProMem("读文件之后");
				// 				ShowProMem("读数据之后");
				m_df.ClearData();
				m_MonSys.file_end = 1;
				m_LfoPara.data_pos = -1;
				// 				g_pAnaObj->m_mutex.Unlock();
				return -1;
			}
			m_LfoPara.file_start_time = m_df.m_DataHead.start_time;
			m_LfoPara.file_end_time = m_df.m_DataHead.end_time;
			m_LfoPara.data_sample = m_df.m_DataHead.time_sample;
			m_MonSys.file_end = 0;
			sprintf(m_LogString, "读取数据文件位置[%d],上轮位置[%d]", m_LfoPara.data_pos, m_LfoPara.data_pos_old);
			MessShow(m_LogString, LTERROR);
			ShowProMem("读文件之后");
			// 			g_pAnaObj->m_mutex.Unlock();

						// 		sprintf(m_LogString,"[yjj]设备点数更新[%d]",m_LfoPara.data_pos);
						//		MessShow(m_LogString,LTINFO);

						// 		if( df.WriteFileData(".//datafile_test.csv") == false )
						// 		{
						// 			sprintf(m_LogString,"输出文件失败");
						// 			MessShow(m_LogString,LTERROR);
						// 		}

			if (GetTimeFromFile(m_MonSys, m_df) < 0)
			{
				sprintf(m_LogString, "PMU时间获取失败");
				MessShow(m_LogString, LTERROR);
				// 				m_df.ClearData();
				return -1;
			}
			else
			{
				m_oscMonSys.start_time = m_MonSys.start_time;
				m_oscMonSys.end_time = m_MonSys.end_time;
				m_oscMonSys.sample_org = m_MonSys.sample_org;
				if (g_pos_threadid == GetId())
				{
					tmp_pos_time.pos = m_LfoPara.data_pos_old;
					tmp_pos_time.start_time = m_MonSys.start_time;
					g_vec_pos_time.push_back(tmp_pos_time);
				}
			}
			if (GetDataFromFile(m_VecDev, m_df) < 0)
			{
				sprintf(m_LogString, "PMU数据获取失败");
				MessShow(m_LogString, LTERROR);
				// 				m_df.ClearData();
				return -1;
			}
			sprintf(m_LogString, "[yjj]读取设备数据结束,文件位置[%d]!", m_LfoPara.data_pos);
			MessShow(m_LogString, LTINFO);
			// 			m_df.ClearData();

		}
		// 		ShowProMem("读数据之后");
	}

	sprintf(m_LogString, "开始计算断面有功");
	MessShow(m_LogString, LTINFO);
	//获取所有支路的有功
	for (int ii = 0; ii < m_VecIntline.size(); ii++)
	{
		sprintf(m_LogString, "支路[%s]id["INT_REC_FMT"]", m_VecIntline[ii].line_name, INT_REC_VAL(m_VecIntline[ii].line_no));
		MessShow(m_LogString);
		m_VecIntline[ii].dev_info.v2d_data.clear();
		for (int jj = 0; jj < m_VecDev.size(); jj++)
		{
			if (m_VecIntline[ii].line_no == m_VecDev[jj].dev.dev_id)
			{
				ClearLfoDev(m_VecIntline[ii].dev_info);
				CopyLfoDev(m_VecIntline[ii].dev_info, m_VecDev[jj]);
				if (!m_VecDev[jj].v2d_data.empty())
				{
					m_VecIntline[ii].dev_info.v2d_data.assign(m_VecDev[jj].v2d_data.begin(), m_VecDev[jj].v2d_data.end());
				}
				// 				m_VecIntline[ii].dev_info = m_VecDev[jj];
				// 				for (int kk= 0;kk<m_VecIntline[ii].dev_info.v2d_data.size();kk++)
				// 				{
				// 					sprintf(m_LogString,"支路[%s]数据[%f]",m_VecIntline[ii].line_name,m_VecIntline[ii].dev_info.v2d_data[kk]);
				// 					MessShow(m_LogString);
				// 				}
				break;
			}
		}
	}
	// 	ShowProMem("获取支路有功");
		//断面所有支路有功叠加获得断面有功
	for (int ii = 0; ii < m_VecDev.size(); ii++)
	{
		if (m_VecDev[ii].dev.dev_type != MENU_LFO_DEV_ITFC)
		{
			continue;
		}
		for (int jj = 0; jj < m_VecIntline.size(); jj++)
		{
			//			sprintf(m_LogString, "支路ID["INT_REC_FMT"],断面ID["INT_REC_FMT"]", INT_REC_VAL(m_VecIntline[jj].inf_no), INT_REC_VAL(m_VecDev[ii].dev.dev_id));
			// 			MessShow(m_LogString);
			if (m_VecIntline[jj].inf_no == m_VecDev[ii].dev.dev_id)
			{
				if (m_VecIntline[jj].dev_info.v2d_data.empty())
				{
					continue;
				}
				m_VecDev[ii].v2d_data.reserve(m_VecIntline[jj].dev_info.v2d_data.size());
				if (m_VecDev[ii].v2d_data.empty())
				{
					if (LFO_DATA_SOURCE_REALTTIME == m_LfoPara.data_source)
					{//实时数据 根据测点厂站 判断支路有功是否需要取相反数
//						if (m_VecIntline[jj].stat_no == m_VecIntline[jj].dev_info.dev.dot_fac_id)
						{
							for (int kk = 0; kk < m_VecIntline[jj].dev_info.v2d_data.size(); kk++)
							{
								m_VecDev[ii].v2d_data.push_back(fabs(m_VecIntline[jj].dev_info.v2d_data[kk]));
							}
						}
						//						else
						//						{
						//							for (int kk = 0;kk<m_VecIntline[jj].dev_info.v2d_data.size();kk++)
						//							{
						//								m_VecDev[ii].v2d_data.push_back(-m_VecIntline[jj].dev_info.v2d_data[kk]);
						//							}
						//						}
					}
					else
					{
						for (int kk = 0; kk < m_VecIntline[jj].dev_info.v2d_data.size(); kk++)
						{
							m_VecDev[ii].v2d_data.push_back(m_VecIntline[jj].dev_info.v2d_data[kk]);
						}
					}
				}
				else
				{
					if (LFO_DATA_SOURCE_REALTTIME == m_LfoPara.data_source)
					{
						//						if (m_VecIntline[jj].stat_no == m_VecIntline[jj].dev_info.dev.dot_fac_id)
						{
							for (int kk = 0; kk < m_VecDev[ii].v2d_data.size(); kk++)
							{
								m_VecDev[ii].v2d_data[kk] += fabs(m_VecIntline[jj].dev_info.v2d_data[kk]);
							}
						}
						//						else
						//						{
						//							for (int kk = 0;kk<m_VecDev[ii].v2d_data.size();kk++)
						//							{
						//								m_VecDev[ii].v2d_data[kk] += -m_VecIntline[jj].dev_info.v2d_data[kk];
						//							}
						//						}
					}
					else
					{
						for (int kk = 0; kk < m_VecDev[ii].v2d_data.size(); kk++)
						{
							m_VecDev[ii].v2d_data[kk] += m_VecIntline[jj].dev_info.v2d_data[kk];
						}
					}
				}
			}
		}
		if (!m_VecDev[ii].v2d_data.empty())
		{
			m_VecDev[ii].stat = MENU_LFO_DEV_STS_OK;
		}

		// 		for (int kk= 0;kk<m_VecDev[ii].v2d_data.size();kk++)
		// 		{
		// 			sprintf(m_LogString,"断面[%s]数据[%f]",m_VecDev[ii].dev.dev_name,m_VecDev[ii].v2d_data[kk]);
		// 			MessShow(m_LogString);
		// 		}
	}
	// 	ShowProMem();

	//     if (JudgeDataStat(m_VecDev)<0)
	//     {
	//         sprintf(m_LogString,"判断设备数据状态错误");
	//         MessShow(m_LogString,LTERROR);
	//         return -1;
	//     }
	//	sprintf(m_LogString,"[yjj]PMU数据处理");
	 //       MessShow(m_LogString,LTERROR);
	//     if (PMUDataHandle(m_VecDev)<0)
	//     {
	//         sprintf(m_LogString,"PMU数据处理错误");
	//         MessShow(m_LogString,LTERROR);
	//         return -1;
	//     }

	return 0;
}

int CLfoAnaBase::FormDataFileName(string& tmpfilename)
{
	char tmpchar[MAXBUF];
	sprintf(tmpchar, "_%d", m_ThreadIdx + 1);
	tmpfilename = m_LfoPara.datafile_name;
	string::size_type idx = tmpfilename.find_last_of(".");
	if (idx != string::npos)
	{
		tmpfilename = tmpfilename.substr(0, idx) + tmpchar + tmpfilename.substr(idx);
	}
	return 0;
}

int CLfoAnaBase::DataPretreat()
{
	//yjj add   	
	if (DataInfoStatistic(m_VecDev, 1) < 0)
	{
		sprintf(m_LogString, "估计曲线波动错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (IsDataValid(m_VecDev) < 0)
	{
		sprintf(m_LogString, "数据检查错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (DataFilter(m_VecDev, 1) < 0)
	{
		sprintf(m_LogString, "数据滤波错误");
		MessShow(m_LogString, LTERROR);
		//   return -1;
	}

	if (DataSample(m_VecDev, 1) < 0)
	{
		sprintf(m_LogString, "数据采样错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (LfoPreEstimate(m_VecDev) < 0)
	{
		sprintf(m_LogString, "根据波形预判错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (IntfaceAmplLimit(m_VecDev) < 0)
	{
		sprintf(m_LogString, "断面振幅计算错误");
		MessShow(m_LogString, LTERROR);
		//         return -1;
	}

	return 0;
}

int CLfoAnaBase::PronyAnaMain()
{
	// Prony分析结构
	vector<LFO_PRONY_INFO> tmpVecCase;

	//	for (int ii =0; ii<(int)m_VecDev.size(); ii++ )
	//	{
	//		int cts_int = m_VecDev[ii].v2d_data.size();
	//		sprintf(m_LogString,"[yjj]设备["INT_REC_FMT"],数据点数[%d]",INT_REC_VAL(m_VecDev[ii].dev.dev_id),cts_int);
	//		MessShow(m_LogString,LTERROR);
	//	}

	if (FormPronyData(m_VecDev, tmpVecCase, 1) < 0)
	{
		sprintf(m_LogString, "形成计算数据错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	sprintf(m_LogString, "本轮监视需进行Prony分析设备数[%d]", tmpVecCase.size());
	MessShow(m_LogString);
	if (PronyProcess(tmpVecCase) < 0)
	{
		sprintf(m_LogString, "进行Prony分析错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (GetDevMode(tmpVecCase, m_VecDev) < 0)
	{
		sprintf(m_LogString, "获取设备对应模式错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ClearLfoPronyInfo1(tmpVecCase);

	if (GetIntMode(m_VecDev) < 0)
	{
		sprintf(m_LogString, "获取断面对应模式错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CLfoAnaBase::FormMonResult()
{
	if (SysModeSum(m_VecDev, m_VecModeShape) < 0)
	{
		sprintf(m_LogString, "模式汇总错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (CalcPartFact(m_VecModeShape) < 0)		//空函数
	{
		sprintf(m_LogString, "计算参与程度错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (GetDevCluster(m_VecModeShape) < 0)	//空函数
	{
		sprintf(m_LogString, "进行设备分群错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (JudgeLfoCenter(m_VecModeShape) < 0)	//空函数
	{
		sprintf(m_LogString, "判断振荡中心错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (GetDevModeShape(m_VecModeShape, m_VecDev) < 0)
	{
		sprintf(m_LogString, "填充设备参与因子和设备分群信息错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	//     if (CalcPartFact(m_VecDev)<0)
	//     {
	//         sprintf(m_LogString,"计算参与程度错误");
	//         MessShow(m_LogString,LTERROR);
	//         return -1;
	//     }

	if (JudgeDevLfoStat(m_VecDev) < 0)
	{
		sprintf(m_LogString, "判断监视设备状态错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	//     if (JudgeDevOscStat(m_VecDev)<0)
	//     {
	//         sprintf(m_LogString,"判断监视设备小扰动状态错误");
	//         MessShow(m_LogString,LTERROR);    
	//     }
	//开始振荡源定位


	if (SearchAsstDeci() < 0)		//空函数
	{
		sprintf(m_LogString, "搜索辅助决策错误");
		MessShow(m_LogString, LTERROR);
		//return -1;
	}

	if (JudgeSysLfoStat(m_VecDev, m_VecModeShape, m_MonSys, m_oscMonSys) < 0)
	{
		sprintf(m_LogString, "判断系统状态错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}


	return 0;
}


//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    GetLfoPara
// FullName:  CLfoAnaBase::GetLfoPara
// Access:    public 
// Returns:   int
// Qualifier:
// 输入：参数结构体(LFO_PARA_STRUCT m_LfoPara)
// 输出：填充参数结构体(LFO_PARA_STRUCT m_LfoPara)
// 返回值：0，正常；1，异常
//************************************
int CLfoAnaBase::GetLfoPara(LFO_PARA_STRUCT& LfoPara, const int mode)
{
	vector<LFO_PARA_TABLE_READ> VecLfoPara;
	ReadLfoPara(VecLfoPara);

	if (VecLfoPara.empty())
	{
		return 0;
	}

	// 获取第一条记录
	// 原低频振荡参数表第二条记录
	int LfoParaIdx = 1;
	if ((int)VecLfoPara.size() <= LfoParaIdx)
	{
		sprintf(m_LogString, "参数表记录个数错误[%d]", (int)VecLfoPara.size());
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (mode == 0)
	{
		// 初始读取更新频率范围，计算过程中保持频率范围不变

		LfoPara.freq_min = VecLfoPara[LfoParaIdx].freq_min;
		LfoPara.freq_max = VecLfoPara[LfoParaIdx].freq_max;
		// 		sprintf(m_LogString,"[yjj]频率下限1[%f]，上限[%f]",VecLfoPara[LfoParaIdx].freq_min,VecLfoPara[LfoParaIdx].freq_max);
		// 		MessShow(m_LogString,LTINFO);		
		//		LfoPara.lfo_freq_min = VecLfoPara[LfoParaIdx].lowfreq_min;
		//		LfoPara.lfo_freq_max = VecLfoPara[LfoParaIdx].lowfreq_max;
		if (1 == LfoPara.if_flit_lfo)
		{
			LfoPara.lfo_freq_min = VecLfoPara[LfoParaIdx - 1].freq_min;	//取第一条记录参数 低频振荡频率下限
			LfoPara.lfo_freq_max = VecLfoPara[LfoParaIdx - 1].freq_max;	//取第一条记录参数 低频振荡频率上限
			LfoPara.lfo_damp = VecLfoPara[LfoParaIdx - 1].lfo_damp;
			// 			sprintf(m_LogString,"[yjj]功率波动频率范围[%f--%f],低频振荡频率范围[%f--%f]",LfoPara.freq_min,LfoPara.freq_max,LfoPara.lfo_freq_min,LfoPara.lfo_freq_max);
			// 			MessShow(m_LogString,LTINFO);
		}
		else
		{
			LfoPara.lfo_freq_min = VecLfoPara[LfoParaIdx].lowfreq_min;
			LfoPara.lfo_freq_max = VecLfoPara[LfoParaIdx].lowfreq_max;
			LfoPara.lfo_damp = VecLfoPara[LfoParaIdx].lfo_damp;
		}
	}
	LfoPara.change_data_source = 0;
	if (LfoPara.data_source != VecLfoPara[LfoParaIdx].dsource)
	{
		// 数据源切换
		LfoPara.data_pos = 0;
		LfoPara.change_data_source = 1;
		sprintf(m_LogString, "原数据源[%d]!=当前数据源[%d]", LfoPara.data_source, VecLfoPara[LfoParaIdx].dsource);
		MessShow(m_LogString, LTERROR);
	}
	//用于Prony计算
	LfoPara.prony_para.freq_min = LfoPara.freq_min;
	LfoPara.prony_para.freq_max = LfoPara.freq_max;
	LfoPara.data_source = VecLfoPara[LfoParaIdx].dsource;
	//LfoPara.time_leng = VecLfoPara[LfoParaIdx].time_leng;
	LfoPara.time_leng_coe = VecLfoPara[LfoParaIdx].time_leng_coe;
	//LfoPara.sample_freq = VecLfoPara[LfoParaIdx].smp_freq;
	LfoPara.sample_freq_coe = VecLfoPara[LfoParaIdx].smp_freq_coe;
	//LfoPara.int_time = VecLfoPara[LfoParaIdx].int_time;
	LfoPara.int_time_coe = VecLfoPara[LfoParaIdx].int_time_coe;
	LfoPara.gn_wn_ampl = VecLfoPara[LfoParaIdx].gn_wn_ampl;
	LfoPara.gn_wn2_ampl = VecLfoPara[LfoParaIdx].gn_wn2_ampl;
	LfoPara.ln_wn_ampl = VecLfoPara[LfoParaIdx].ln_wn_ampl;
	LfoPara.ln_wn2_ampl = VecLfoPara[LfoParaIdx].ln_wn2_ampl;
	LfoPara.tr_wn_ampl = VecLfoPara[LfoParaIdx].tr_wn_ampl;
	LfoPara.tr_wn2_ampl = VecLfoPara[LfoParaIdx].tr_wn2_ampl;
	//LfoPara.stor_ampl = VecLfoPara[LfoParaIdx].stor_ampl;
	LfoPara.lack_damp = VecLfoPara[LfoParaIdx].lack_damp;
	LfoPara.flfo_damp = VecLfoPara[LfoParaIdx].flfo_damp;
	LfoPara.fjud_wnap_per = VecLfoPara[LfoParaIdx].fjud_wnap_per;
	LfoPara.lfo_jud_num = VecLfoPara[LfoParaIdx].lfo_jud_num;
	LfoPara.warn_time_max = VecLfoPara[LfoParaIdx].warnlfo_time;
	LfoPara.safe_time_max = VecLfoPara[LfoParaIdx].safelfo_time;
	//LfoPara.delay_time = VecLfoPara[LfoParaIdx].delay_time;
	LfoPara.freq_diff = VecLfoPara[LfoParaIdx].sm_md_freq;
	//LfoPara.cache_windows_num = 
	//LfoPara.stor_ampl = VecLfoPara[LfoParaIdx].
	//LfoPara.is_mon_lfo = 
	//LfoPara.time_leng_min = VecLfoPara[LfoParaIdx].time_leng_min;
	//LfoPara.time_leng_max = VecLfoPara[LfoParaIdx]
	//LfoPara.sample_freq_min = VecLfoPara[LfoParaIdx].sample_freq_min;
	//LfoPara.sample_freq_max = VecLfoPara[LfoParaIdx].sample_freq_max;
	//LfoPara.int_time_min = VecLfoPara[LfoParaIdx]
	//LfoPara.int_time_max = VecLfoPara[LfoParaIdx]
	//LfoPara.mode_save_amplper =
	LfoPara.rcd_pre_t = VecLfoPara[LfoParaIdx].rcd_pre_t;	/* 记录超前时间 */
	LfoPara.rcd_aft_t = VecLfoPara[LfoParaIdx].rcd_aft_t;	/* 记录延后时间 */
	LfoPara.lfo_fac_id = VecLfoPara[LfoParaIdx].lfo_fac_id;	/* 对应虚拟厂站ID */
	LfoPara.is_warn = VecLfoPara[LfoParaIdx].is_warn;	/* 是否发送告警 */
	LfoPara.is_samp = VecLfoPara[LfoParaIdx].is_samp;	/* 是否触发采样 */
	LfoPara.is_tmd = VecLfoPara[LfoParaIdx].is_tmd;	/* 是否触发tmd */
	LfoPara.is_triems = VecLfoPara[LfoParaIdx].is_triems;	/* 是否向EMS转发 */
#ifndef  _WINDOWS32	
	LfoPara.ems_keyid = VecLfoPara[LfoParaIdx].ems_keyid;	/* EMS转发KEYID */
#endif
	LfoPara.volt_thre1 = VecLfoPara[LfoParaIdx].volt_thre1;
	LfoPara.volt_thre2 = VecLfoPara[LfoParaIdx].volt_thre2;
	LfoPara.ampl_thre1 = VecLfoPara[LfoParaIdx].ampl_thre1;
	LfoPara.ampl_thre2 = VecLfoPara[LfoParaIdx].ampl_thre2;
	LfoPara.ampl_thre3 = VecLfoPara[LfoParaIdx].ampl_thre3;
	LfoPara.gn_wn_ampl1 = VecLfoPara[LfoParaIdx].gn_wn_ampl1;
	LfoPara.ln_wn_ampl1 = VecLfoPara[LfoParaIdx].ln_wn_ampl1;
	LfoPara.tr_wn_ampl1 = VecLfoPara[LfoParaIdx].tr_wn_ampl1;
	LfoPara.ln_swn_ampl1 = VecLfoPara[LfoParaIdx].ln_swn_ampl1;
	LfoPara.ln_swn_ampl2 = VecLfoPara[LfoParaIdx].ln_swn_ampl2;
	LfoPara.ln_swn_ampl3 = VecLfoPara[LfoParaIdx].ln_swn_ampl3;
	LfoPara.ln_ewn_ampl1 = VecLfoPara[LfoParaIdx].ln_ewn_ampl1;
	LfoPara.ln_ewn_ampl2 = VecLfoPara[LfoParaIdx].ln_ewn_ampl2;
	LfoPara.ln_ewn_ampl3 = VecLfoPara[LfoParaIdx].ln_ewn_ampl3;
	LfoPara.tr_swn_ampl1 = VecLfoPara[LfoParaIdx].tr_swn_ampl1;
	LfoPara.tr_swn_ampl2 = VecLfoPara[LfoParaIdx].tr_swn_ampl2;
	LfoPara.tr_swn_ampl3 = VecLfoPara[LfoParaIdx].tr_swn_ampl3;
	LfoPara.tr_ewn_ampl1 = VecLfoPara[LfoParaIdx].tr_ewn_ampl1;
	LfoPara.tr_ewn_ampl2 = VecLfoPara[LfoParaIdx].tr_ewn_ampl2;
	LfoPara.tr_ewn_ampl3 = VecLfoPara[LfoParaIdx].tr_ewn_ampl3;
	LfoPara.gn_swn_ampl2 = VecLfoPara[LfoParaIdx].gn_swn_ampl2;
	LfoPara.gn_swn_ampl3 = VecLfoPara[LfoParaIdx].gn_swn_ampl3;
	LfoPara.gn_ewn_ampl2 = VecLfoPara[LfoParaIdx].gn_ewn_ampl2;
	LfoPara.gn_ewn_ampl3 = VecLfoPara[LfoParaIdx].gn_ewn_ampl3;

	LfoPara.sh_fact = VecLfoPara[LfoParaIdx].sh_fact;
	LfoPara.sh_gn_pow = VecLfoPara[LfoParaIdx].sh_gn_pow;
	LfoPara.sh_ln_pow = VecLfoPara[LfoParaIdx].sh_ln_pow;
	LfoPara.sh_phas = VecLfoPara[LfoParaIdx].sh_phas;
	LfoPara.osc_pow_ampl = VecLfoPara[LfoParaIdx].osc_pow_ampl;
	LfoPara.osc_sec_ampl = VecLfoPara[LfoParaIdx].osc_sec_ampl;

	//广东项目新增告警 add by cyj 20170801	
	LfoPara.gn_lfo_wn_ampl = VecLfoPara[LfoParaIdx - 1].gn_wn_ampl;
	LfoPara.gn_lfo_wn_ampl1 = VecLfoPara[LfoParaIdx - 1].gn_wn_ampl1;
	LfoPara.ln_lfo_swn_ampl1 = VecLfoPara[LfoParaIdx - 1].ln_swn_ampl1;
	LfoPara.ln_lfo_swn_ampl2 = VecLfoPara[LfoParaIdx - 1].ln_swn_ampl2;
	LfoPara.ln_lfo_swn_ampl3 = VecLfoPara[LfoParaIdx - 1].ln_swn_ampl3;
	LfoPara.ln_lfo_ewn_ampl1 = VecLfoPara[LfoParaIdx - 1].ln_ewn_ampl1;
	LfoPara.ln_lfo_ewn_ampl2 = VecLfoPara[LfoParaIdx - 1].ln_ewn_ampl2;
	LfoPara.ln_lfo_ewn_ampl3 = VecLfoPara[LfoParaIdx - 1].ln_ewn_ampl3;
	LfoPara.tr_lfo_swn_ampl1 = VecLfoPara[LfoParaIdx - 1].tr_swn_ampl1;
	LfoPara.tr_lfo_swn_ampl2 = VecLfoPara[LfoParaIdx - 1].tr_swn_ampl2;
	LfoPara.tr_lfo_swn_ampl3 = VecLfoPara[LfoParaIdx - 1].tr_swn_ampl3;
	LfoPara.tr_lfo_ewn_ampl1 = VecLfoPara[LfoParaIdx - 1].tr_ewn_ampl1;
	LfoPara.tr_lfo_ewn_ampl2 = VecLfoPara[LfoParaIdx - 1].tr_ewn_ampl2;
	LfoPara.tr_lfo_ewn_ampl3 = VecLfoPara[LfoParaIdx - 1].tr_ewn_ampl3;
	LfoPara.gn_capacity1 = VecLfoPara[LfoParaIdx].gn_capacity1;
	LfoPara.gn_capacity2 = VecLfoPara[LfoParaIdx].gn_capacity2;
	LfoPara.gn_swn_ampl2 = VecLfoPara[LfoParaIdx].gn_swn_ampl2;
	LfoPara.gn_swn_ampl3 = VecLfoPara[LfoParaIdx].gn_swn_ampl3;
	LfoPara.gn_ewn_ampl2 = VecLfoPara[LfoParaIdx].gn_ewn_ampl2;
	LfoPara.gn_ewn_ampl3 = VecLfoPara[LfoParaIdx].gn_ewn_ampl3;
	LfoPara.thread_mode = VecLfoPara[LfoParaIdx].thread_mode;
	LfoPara.pre_freq1 = VecLfoPara[LfoParaIdx].pre_freq1;
	LfoPara.pre_freq2 = VecLfoPara[LfoParaIdx].pre_freq2;
	LfoPara.pre_freq3 = VecLfoPara[LfoParaIdx].pre_freq3;
	LfoPara.pre_freq4 = VecLfoPara[LfoParaIdx].pre_freq4;
	LfoPara.pre_freq5 = VecLfoPara[LfoParaIdx].pre_freq5;
	LfoPara.phas_diff = VecLfoPara[LfoParaIdx].phas_diff;
	LfoPara.mainmode_num = VecLfoPara[LfoParaIdx].mainmode_num;
	LfoPara.inf_ampl = VecLfoPara[LfoParaIdx].inf_ampl;
	LfoPara.center_volt_ampl = VecLfoPara[LfoParaIdx].center_volt_ampl;
	LfoPara.para_VoltAmplPer = VecLfoPara[LfoParaIdx].para_voltamplper;
	LfoPara.para_nShwMaxFcNum = VecLfoPara[LfoParaIdx].para_nshwmaxfcnum;
	LfoPara.para_Ampl_aver = VecLfoPara[LfoParaIdx].para_ampl_aver;
	LfoPara.max_damp = VecLfoPara[LfoParaIdx].max_damp;
	LfoPara.disp_num = VecLfoPara[LfoParaIdx].disp_num;
	LfoPara.rec_num = VecLfoPara[LfoParaIdx].rec_num;
	LfoPara.Voltunit_is_kV = VecLfoPara[LfoParaIdx].voltunit_is_kv;
	LfoPara.is_cycle = VecLfoPara[LfoParaIdx].is_cycle;
	LfoPara.is_lfo_curve = VecLfoPara[LfoParaIdx].is_lfo_curve;
	LfoPara.warn_app_no = VecLfoPara[LfoParaIdx].warn_app_no;
	LfoPara.is_save_casefile = VecLfoPara[LfoParaIdx].is_save_casefile;
	LfoPara.is_save_devinfo = VecLfoPara[LfoParaIdx].is_save_devinfo;
	LfoPara.is_add_facname = VecLfoPara[LfoParaIdx].is_add_facname;
	LfoPara.is_eng = VecLfoPara[LfoParaIdx].is_eng;


	//检查低频振荡结束限值是否合理
	if (LfoPara.gn_lfo_wn_ampl1 < LfoPara.gn_wn_ampl1)
	{
		sprintf(m_LogString, "机组低频振荡结束振幅限值[%.2f]小于功率波动结束振幅限值,请修改。程序已自动修正为功率波动结束振幅限值", LfoPara.gn_lfo_wn_ampl1);
		MessShow(m_LogString, LTERROR);
		LfoPara.gn_lfo_wn_ampl1 = LfoPara.gn_wn_ampl1;
	}
	if (LfoPara.ln_lfo_ewn_ampl1 < LfoPara.ln_ewn_ampl1)
	{
		sprintf(m_LogString, "线路低频振荡结束振幅限值1[%.2f]小于功率波动结束振幅限值1,请修改。程序已自动修正为功率波动结束振幅限值1", LfoPara.ln_lfo_ewn_ampl1);
		MessShow(m_LogString, LTERROR);
		LfoPara.ln_lfo_ewn_ampl1 = LfoPara.ln_ewn_ampl1;
	}
	if (LfoPara.ln_lfo_ewn_ampl2 < LfoPara.ln_ewn_ampl2)
	{
		sprintf(m_LogString, "线路低频振荡结束振幅限值2[%.2f]小于功率波动结束振幅限值2,请修改。程序已自动修正为功率波动结束振幅限值2", LfoPara.ln_lfo_ewn_ampl2);
		MessShow(m_LogString, LTERROR);
		LfoPara.ln_lfo_ewn_ampl2 = LfoPara.ln_ewn_ampl2;
	}
	if (LfoPara.ln_lfo_ewn_ampl3 < LfoPara.ln_ewn_ampl3)
	{
		sprintf(m_LogString, "线路低频振荡结束振幅限值3[%.2f]小于功率波动结束振幅限值3,请修改。程序已自动修正为功率波动结束振幅限值1", LfoPara.ln_lfo_ewn_ampl3);
		MessShow(m_LogString, LTERROR);
		LfoPara.ln_lfo_ewn_ampl3 = LfoPara.ln_ewn_ampl3;
	}
	if (LfoPara.tr_lfo_ewn_ampl1 < LfoPara.tr_ewn_ampl1)
	{
		sprintf(m_LogString, "变压器低频振荡结束振幅限值1[%.2f]小于功率波动结束振幅限值1,请修改。程序已自动修正为功率波动结束振幅限值1", LfoPara.tr_lfo_ewn_ampl1);
		MessShow(m_LogString, LTERROR);
		LfoPara.tr_lfo_ewn_ampl1 = LfoPara.tr_ewn_ampl1;
	}
	if (LfoPara.tr_lfo_ewn_ampl2 < LfoPara.tr_ewn_ampl2)
	{
		sprintf(m_LogString, "变压器低频振荡结束振幅限值2[%.2f]小于功率波动结束振幅限值1,请修改。程序已自动修正为功率波动结束振幅限值2", LfoPara.tr_lfo_ewn_ampl2);
		MessShow(m_LogString, LTERROR);
		LfoPara.tr_lfo_ewn_ampl2 = LfoPara.tr_ewn_ampl2;
	}
	if (LfoPara.tr_lfo_ewn_ampl3 < LfoPara.tr_ewn_ampl3)
	{
		sprintf(m_LogString, "变压器低频振荡结束振幅限值3[%.2f]小于功率波动结束振幅限值1,请修改。程序已自动修正为功率波动结束振幅限值3", LfoPara.tr_lfo_ewn_ampl3);
		MessShow(m_LogString, LTERROR);
		LfoPara.tr_lfo_ewn_ampl3 = LfoPara.tr_ewn_ampl3;
	}

	sprintf(m_LogString, "读取参数校验:振荡中心显示厂站数[%d],阻尼比饱和值[%f].断面系数[%f],机组容量1[%f]", LfoPara.para_nShwMaxFcNum, LfoPara.max_damp, LfoPara.inf_ampl, LfoPara.gn_capacity1);
	MessShow(m_LogString);
	// 	sprintf(m_LogString,"线路低频振荡振幅限值1:开始[%f],结束[%f]",LfoPara.ln_lfo_swn_ampl1,LfoPara.ln_lfo_ewn_ampl1);
	// 	MessShow(m_LogString);
	// 	sprintf(m_LogString,"线路低频振荡振幅限值2:开始[%f],结束[%f]",LfoPara.ln_lfo_swn_ampl2,LfoPara.ln_lfo_ewn_ampl2);
	// 	MessShow(m_LogString);
	// 	sprintf(m_LogString,"线路低频振荡振幅限值3:开始[%f],结束[%f]",LfoPara.ln_lfo_swn_ampl3,LfoPara.ln_lfo_ewn_ampl3);
	// 	MessShow(m_LogString);
	// 	sprintf(m_LogString,"变压器低频振荡振幅限值1:开始[%f],结束[%f]",LfoPara.tr_lfo_swn_ampl1,LfoPara.tr_lfo_ewn_ampl1);
	// 	MessShow(m_LogString);
	// 	sprintf(m_LogString,"变压器低频振荡振幅限值2:开始[%f],结束[%f]",LfoPara.tr_lfo_swn_ampl2,LfoPara.tr_lfo_ewn_ampl2);
	// 	MessShow(m_LogString);
	// 	sprintf(m_LogString,"变压器低频振荡振幅限值3:开始[%f],结束[%f]",LfoPara.tr_lfo_swn_ampl3,LfoPara.tr_lfo_ewn_ampl3);
	// 	MessShow(m_LogString);


	return 0;
}


int CLfoAnaBase::GetModeFreq(float& freq)
{
	CBuffer buf_base;
	if (PODB_TableGet(LFO_PARA_NO_TAB, "mon_fail2", buf_base) < 0)
	{
		sprintf(m_LogString, "读取LFO参数表mon_fail2域失败");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	int nInfo = buf_base.GetLength() / sizeof(float);
	if (nInfo < 2)
	{
		sprintf(m_LogString, "LFO参数表为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "LFO参数表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	float* pInfo = NULL;
	pInfo = new float[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(float));

	freq = pInfo[1];

	return 0;
}

//************************************
// Method:    CheckPara
// FullName:  CLfoAnaObj::CheckPara
// Access:    public 
// Returns:   int
// Qualifier:
// 输入：参数结构体(LFO_PARA_STRUCT m_LfoPara)
// 输出：参数结构体(LFO_PARA_STRUCT m_LfoPara)
// 返回值：0，正常；1，参数越限
//************************************
int CLfoAnaBase::CheckPara(LFO_PARA_STRUCT& LfoPara, const int logtype)
{
	int nFlag = 0;
	float temp_f = 0.0f;
	if (logtype >= 1)
	{
		// 主函数中判频率
		if (LfoPara.freq_min < 0.03f || LfoPara.freq_min > 0.5f)
		{
			//			nFlag ++;
			sprintf(m_LogString, "频率下限[%.2f]超出(0.03~0.5),请修改。程序已自动修正为0.03Hz", LfoPara.freq_min);
			MessShow(m_LogString, LTERROR);
			LfoPara.freq_min = 0.03;
		}
		else
		{
			sprintf(m_LogString, "频率下限[%.2f]", LfoPara.freq_min);
			MessShow(m_LogString, logtype);
		}
		if (LfoPara.freq_max < 1.0f || LfoPara.freq_max > 8.0f)
		{
			//			nFlag ++;
			sprintf(m_LogString, "频率上限[%.2f]超出(1.0~8.0),请修改。程序自动修正为8.0Hz", LfoPara.freq_max);
			MessShow(m_LogString, LTERROR);
			LfoPara.freq_max = 8.0;
		}
		else
		{
			sprintf(m_LogString, "频率上限[%.2f]", LfoPara.freq_max);
			MessShow(m_LogString, logtype);
		}
		if (LfoPara.freq_max <= LfoPara.freq_min)
		{
			//			nFlag ++;
			LfoPara.freq_min = 0.1;
			LfoPara.freq_max = 5.0;
			sprintf(m_LogString, "频率上下限错误max[%.2f]<=min[%.2f],请修改。程序自动修正为[0.1~5.0]", LfoPara.freq_max, LfoPara.freq_min);
			MessShow(m_LogString, LTERROR);
		}

		if (LfoPara.lfo_freq_min < 0.1f || LfoPara.lfo_freq_min > 0.5f)
		{
			//			nFlag ++;
			LfoPara.lfo_freq_min = 0.2;
			sprintf(m_LogString, "低频振荡频率下限[%.2f]超出(0.1~0.5),请修改。程序自动修正为0.2Hz", LfoPara.lfo_freq_min);
			MessShow(m_LogString, LTERROR);
		}
		else
		{
			sprintf(m_LogString, "低频振荡频率下限[%.2f]", LfoPara.lfo_freq_min);
			MessShow(m_LogString, logtype);
		}
		if (LfoPara.lfo_freq_max < 1.0f || LfoPara.lfo_freq_max > 8.0f)
		{
			//			nFlag ++;
			LfoPara.lfo_freq_max = 2.5;
			sprintf(m_LogString, "低频振荡频率上限[%.2f]超出(1.0~5.0),请修改。程序自动修正为2.5Hz", LfoPara.lfo_freq_max);
			MessShow(m_LogString, LTERROR);
		}
		else
		{
			sprintf(m_LogString, "低频振荡频率上限[%.2f]", LfoPara.lfo_freq_max);
			MessShow(m_LogString, logtype);
		}
		if (LfoPara.lfo_freq_max <= LfoPara.lfo_freq_min)
		{
			//			nFlag ++;
			LfoPara.lfo_freq_min = 0.2;
			LfoPara.lfo_freq_max = 2.5;
			sprintf(m_LogString, "低频振荡频率上下限max[%.2f]<=min[%.2f],请修改。程序自动修正为[0.2~2.5]", LfoPara.lfo_freq_max, LfoPara.lfo_freq_min);
			MessShow(m_LogString, LTERROR);
		}
		// 		if (LfoPara.lfo_freq_min <= LfoPara.freq_min )
		// 		{
		// //			nFlag ++;
		// 			LfoPara.freq_min = 0.1;
		// 			LfoPara.lfo_freq_min = 0.2;
		// 			sprintf(m_LogString,"频率上下限lfo_min[%.2f]<=min[%.2f],请修改。程序自动修正为lfo_min=0.2Hz,min=0.1Hz",LfoPara.lfo_freq_min,LfoPara.freq_min);
		// 			MessShow(m_LogString,LTERROR);
		// 		}
		// 		if (LfoPara.lfo_freq_max >= LfoPara.freq_max )
		// 		{
		// //			nFlag ++;
		// 			LfoPara.freq_max = 2.5;
		// 			LfoPara.lfo_freq_max = 5.0;
		// 			sprintf(m_LogString,"频率上下限lfo_max[%.2f]>=max[%.2f],请修改。程序自动修正为lfo_max=2.5Hz,max=5.0Hz",LfoPara.lfo_freq_max,LfoPara.freq_max);
		// 			MessShow(m_LogString,LTERROR);
		// 		}
	}
	if (LfoPara.time_leng_coe < 1.0f || LfoPara.time_leng_coe > 10.0f)
	{
		//		nFlag ++;
		LfoPara.time_leng_coe = 2.0;
		sprintf(m_LogString, "观察时间窗口系数[%.2f]超出(1.0~10.0),请修改。程序自动修正为2", LfoPara.time_leng_coe);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "观察时间窗口系数[%.2f]", LfoPara.time_leng_coe);
		MessShow(m_LogString, logtype);
	}
	if (LfoPara.time_leng_min > LfoPara.time_leng_max)
	{
		//		nFlag ++;
		temp_f = LfoPara.time_leng_min;
		LfoPara.time_leng_min = LfoPara.time_leng_max;
		LfoPara.time_leng_max = temp_f;
		sprintf(m_LogString, "窗口时间长度范围错误(%d~%d),程序自动修正窗口长度", LfoPara.time_leng_min, LfoPara.time_leng_max);
		MessShow(m_LogString, LTERROR);
	}
	if (LfoPara.time_leng < LfoPara.time_leng_min || LfoPara.time_leng > LfoPara.time_leng_max)
	{
		//		nFlag ++;
		sprintf(m_LogString, "窗口时间长度[%d]超出(%d~%d),请修改。", LfoPara.time_leng, LfoPara.time_leng_min, LfoPara.time_leng_max);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "窗口时间长度[%d](%d~%d)", LfoPara.time_leng, LfoPara.time_leng_min, LfoPara.time_leng_max);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.sample_freq_coe < 3.0f || LfoPara.sample_freq_coe > 20.0f)
	{
		//		nFlag ++;
		LfoPara.sample_freq_coe = 3.0f;
		sprintf(m_LogString, "采样频率系数[%.2f]超出(3.0~20.0),请修改。程序自动修正为3.0", LfoPara.sample_freq_coe);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "采样频率系数[%.2f]", LfoPara.sample_freq_coe);
		MessShow(m_LogString, logtype);
	}
	if (LfoPara.sample_freq_min > LfoPara.sample_freq_max)
	{
		//		nFlag ++;
		temp_f = LfoPara.sample_freq_min;
		LfoPara.sample_freq_min = LfoPara.sample_freq_max;
		LfoPara.sample_freq_max = temp_f;
		sprintf(m_LogString, "采样频率范围错误(%.2f~%.2f),请修改。程序自动修正", LfoPara.sample_freq_min, LfoPara.sample_freq_max);
		MessShow(m_LogString, LTERROR);
	}
	if (LfoPara.sample_freq < LfoPara.sample_freq_min || LfoPara.sample_freq > LfoPara.sample_freq_max)
	{
		//		nFlag ++;
		LfoPara.sample_freq = LfoPara.sample_freq_min;
		sprintf(m_LogString, "采样频率[%.2f]超出(%.2f~%.2f),请修改。程序自动修正", LfoPara.sample_freq, LfoPara.sample_freq_min, LfoPara.sample_freq_max);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "采样频率[%.2f](%.2f~%.2f)", LfoPara.sample_freq, LfoPara.sample_freq_min, LfoPara.sample_freq_max);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.int_time_coe < 0.0f || LfoPara.int_time_coe > 1.0f)
	{
		//		nFlag ++;
		LfoPara.int_time_coe = 0.5;
		sprintf(m_LogString, "交叠窗口系数[%.2f]超出(0.0~1.0),请修改。程序自动修正为0.5", LfoPara.int_time_coe);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "交叠窗口系数[%.2f]", LfoPara.int_time_coe);
		MessShow(m_LogString, logtype);
	}
	if (LfoPara.int_time_min > LfoPara.int_time_max)
	{
		//		nFlag ++;
		temp_f = LfoPara.int_time_min;
		LfoPara.int_time_min = LfoPara.int_time_max;
		LfoPara.int_time_max = temp_f;
		sprintf(m_LogString, "交叠窗口范围错误(%d~%d),请修改。程序自动修正", LfoPara.int_time_min, LfoPara.int_time_max);
		MessShow(m_LogString, LTERROR);
	}
	if (LfoPara.int_time < LfoPara.int_time_min || LfoPara.int_time > LfoPara.int_time_max)
	{
		//		nFlag ++;
		LfoPara.int_time = LfoPara.int_time_min;
		sprintf(m_LogString, "交叠窗口[%d]超出(%d~%d),请修改。程序自动修正[%d]", LfoPara.int_time, LfoPara.int_time_min, LfoPara.int_time_max, LfoPara.int_time);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "交叠窗口[%d](%d~%d)", LfoPara.int_time, LfoPara.int_time_min, LfoPara.int_time_max);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.gn_wn_ampl < 0.0f || LfoPara.gn_wn_ampl > 100.0f)
	{
		//		nFlag ++;
		LfoPara.gn_wn_ampl = 10.0;
		sprintf(m_LogString, "机组振幅限值[%.2f]超出(0.0~100.0),请修改。程序自动修正为10.0", LfoPara.gn_wn_ampl);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "机组振幅限值[%.2f]", LfoPara.gn_wn_ampl);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.ln_wn_ampl < 0.0f || LfoPara.ln_wn_ampl > 100.0f)
	{
		//		nFlag ++;
		LfoPara.ln_wn_ampl = 25.0;
		sprintf(m_LogString, "线路振幅限值[%.2f]超出(0.0~100.0),请修改。程序自动修正为25.0", LfoPara.ln_wn_ampl);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "线路振幅限值[%.2f]", LfoPara.ln_wn_ampl);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.tr_wn_ampl < 0.0f || LfoPara.tr_wn_ampl > 100.0f)
	{
		//		nFlag ++;
		LfoPara.tr_wn_ampl = 25.0;
		sprintf(m_LogString, "变压器振幅限值[%.2f]超出(0.0~100.0),请修改。程序自动修正为25.0", LfoPara.tr_wn_ampl);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "变压器振幅限值[%.2f]", LfoPara.tr_wn_ampl);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.stor_damp < 5.0f || LfoPara.stor_damp > 100.0f)
	{
		//		nFlag ++;
		LfoPara.stor_damp = 30.0;
		sprintf(m_LogString, "保存模式阻尼限值[%.2f]超出(5.0~100.0),请修改。程序自动修正为30.0", LfoPara.stor_damp);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "保存模式阻尼限值[%.2f]", LfoPara.stor_damp);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.lack_damp < -100.0f || LfoPara.lack_damp > 100.0f)
	{
		//		nFlag ++;
		LfoPara.lack_damp = 5.0;
		sprintf(m_LogString, "阻尼不足限值[%.2f]超出(-100.0~100.0),请修改。程序自动修正为5.0", LfoPara.lack_damp);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "阻尼不足限值[%.2f]", LfoPara.lack_damp);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.lfo_damp < -100.0f || LfoPara.lfo_damp > 100.0f)
	{
		//		nFlag ++;
		LfoPara.lfo_damp = 3.0;
		sprintf(m_LogString, "告警阻尼限值[%.2f]超出(-100.0~100.0),请修改。程序自动修正为5.0", LfoPara.lfo_damp);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "告警阻尼限值[%.2f]", LfoPara.lfo_damp);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.damp_min > 0)
	{
		//		nFlag ++;
		LfoPara.damp_min = -20.0;
		sprintf(m_LogString, "最小阻尼限值[%.2f]超出(-100.0~0.0),请修改。程序自动修正为-20.0", LfoPara.damp_min);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "最小阻尼限值[%.2f]", LfoPara.damp_min);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.fjud_wnap_per < 0.0f || LfoPara.fjud_wnap_per > 100.0f)
	{
		//		nFlag ++;
		LfoPara.fjud_wnap_per = 60.0;
		sprintf(m_LogString, "预判振幅百分比[%.2f]超出(0.0~100.0),请修改。程序自动修正为60.0", LfoPara.fjud_wnap_per);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "预判振幅百分比[%.2f]", LfoPara.fjud_wnap_per);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.ana_ampl_per < 0.0f || LfoPara.ana_ampl_per > 100.0f)
	{
		//		nFlag ++;
		LfoPara.ana_ampl_per = 30.0;
		sprintf(m_LogString, "分析角度功率振幅百分比[%.2f]超出(0.0~100.0),请修改。程序自动修正为30.0", LfoPara.ana_ampl_per);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "分析角度功率振幅百分比[%.2f]", LfoPara.ana_ampl_per);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.warn_time_max < 1 || LfoPara.warn_time_max > 60)
	{
		//		nFlag ++;
		LfoPara.warn_time_max = 20;
		sprintf(m_LogString, "告警持续时间限值[%d]超出(1~60),请修改。程序自动修正为20", LfoPara.warn_time_max);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "告警持续时间限值[%d]", LfoPara.warn_time_max);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.safe_time_max < 1 || LfoPara.safe_time_max > 60)
	{
		//		nFlag ++;
		LfoPara.safe_time_max = 20;
		sprintf(m_LogString, "安全持续持续时间限值[%d]超出(1~60),请修改。程序自动修正为20.0", LfoPara.safe_time_max);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "安全持续持续时间限值[%d]", LfoPara.safe_time_max);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.delay_time < 0 || LfoPara.delay_time > 60)
	{
		//		nFlag ++;
		LfoPara.delay_time = 2;
		sprintf(m_LogString, "PMU时刻延迟时间超出[%d](0~60),请修改。程序自动修正为2", LfoPara.delay_time);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "PMU时刻延迟时间[%d]", LfoPara.delay_time);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.freq_diff < 0.0f || LfoPara.freq_diff > 5.0f)
	{
		//		nFlag ++;
		LfoPara.freq_diff = 0.05;
		sprintf(m_LogString, "同模式频率差[%.2f]超出(0.0~5.0),请修改。程序自动修正为0.05", LfoPara.freq_diff);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "同模式频率差[%.2f]", LfoPara.freq_diff);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.cache_windows_num < 2 || LfoPara.cache_windows_num > 50)
	{
		//		nFlag ++;
		LfoPara.cache_windows_num = 10;
		sprintf(m_LogString, "缓存窗口最大个数[%d]超出(2~50),请修改。程序自动修正为10", LfoPara.cache_windows_num);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "缓存窗口最大个数[%d]", LfoPara.cache_windows_num);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.mode_save_amplper < 0.0f || LfoPara.mode_save_amplper > 100.0f)
	{
		//		nFlag ++;
		LfoPara.mode_save_amplper = 30.0;
		sprintf(m_LogString, "模式保存幅值百分比[%.2f]超出(0.0~100.0),请修改。程序自动修正为30.0", LfoPara.mode_save_amplper);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "模式保存幅值百分比[%.2f]", LfoPara.mode_save_amplper);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.rcd_pre_t < 0 || LfoPara.rcd_pre_t > 100.0f)
	{
		//		nFlag ++;
		LfoPara.rcd_pre_t = 30;
		sprintf(m_LogString, "记录超前时间[%d]超出(0.0~100.0),请修改。程序自动修正为30", LfoPara.rcd_pre_t);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "记录超前时间[%d]", LfoPara.rcd_pre_t);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.rcd_aft_t < 0 || LfoPara.rcd_aft_t > 100.0f)
	{
		//		nFlag ++;
		LfoPara.rcd_aft_t = 30;
		sprintf(m_LogString, "记录延后时间[%d]超出(0.0~100.0),请修改。程序自动修正为30", LfoPara.rcd_aft_t);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "记录延后时间[%d]", LfoPara.rcd_aft_t);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.is_warn != 0 && LfoPara.is_warn != 1)
	{
		//		nFlag ++;
		LfoPara.is_warn = 1;
		sprintf(m_LogString, "是否发送告警错误[%d],请修改。程序自动修正为1", LfoPara.is_warn);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "是否发送告警[%d]", LfoPara.is_warn);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.is_tmd != 0 && LfoPara.is_tmd != 1)
	{
		//		nFlag ++;
		LfoPara.is_tmd = 1;
		sprintf(m_LogString, "是否触发tmd错误[%d],请修改。程序自动修正为1", LfoPara.is_tmd);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "是否触发tmd[%d]", LfoPara.is_tmd);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.is_triems != 0 && LfoPara.is_triems != 1)
	{
		//		nFlag ++;
		LfoPara.is_triems = 1;
		sprintf(m_LogString, "是否向EMS转发错误[%d],请修改。程序自动修正为1", LfoPara.is_triems);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "是否向EMS转发[%d]", LfoPara.is_triems);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.devinfo_source < 0 || LfoPara.devinfo_source > 10)
	{
		//		nFlag ++;
		LfoPara.devinfo_source = 0;
		sprintf(m_LogString, "设备信息源错误[%d],请修改。程序自动修正为0", LfoPara.devinfo_source);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "设备信息源[%d]", LfoPara.devinfo_source);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.data_source < 0 || LfoPara.data_source > 10)
	{
		//		nFlag ++;
		LfoPara.data_source = 0;
		sprintf(m_LogString, "数据源错误[%d],请修改。程序自动修正为0", LfoPara.data_source);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "数据源[%d]", LfoPara.data_source);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.is_save_casefile != 0 && LfoPara.is_save_casefile != 1)
	{
		//		nFlag ++;
		LfoPara.is_save_casefile = 1;
		sprintf(m_LogString, "生成案例文件错误[%d],请修改。程序自动修正为1", LfoPara.is_save_casefile);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "生成案例文件[%d]", LfoPara.is_save_casefile);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.is_save_devinfo != 0 && LfoPara.is_save_devinfo != 1)
	{
		//		nFlag ++;
		LfoPara.is_save_devinfo = 1;
		sprintf(m_LogString, "生成设备信息错误[%d],请修改。程序自动修正为1", LfoPara.is_save_devinfo);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "生成设备信息[%d]", LfoPara.is_save_devinfo);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.is_cycle != 0 && LfoPara.is_cycle != 1)
	{
		//		nFlag ++;
		LfoPara.is_cycle = 1;
		sprintf(m_LogString, "循环读取数据错误[%d],请修改。程序自动修正为1", LfoPara.is_cycle);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "循环读取数据[%d]", LfoPara.is_cycle);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.is_lfo_curve != 0 && LfoPara.is_lfo_curve != 1)
	{
		//		nFlag ++;
		LfoPara.is_lfo_curve = 1;
		sprintf(m_LogString, "调用曲线控件错误[%d],请修改。程序自动修正为1", LfoPara.is_lfo_curve);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "调用曲线控件[%d]", LfoPara.is_lfo_curve);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.warn_app_no <= 0)
	{
		//		nFlag ++;
		sprintf(m_LogString, "发送告警应用号错误[%d],请修改。", LfoPara.warn_app_no);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "发送告警应用号[%d]", LfoPara.warn_app_no);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.is_eng != 0 && LfoPara.is_eng != 1)
	{
		//		nFlag ++;
		LfoPara.is_eng = 0;
		sprintf(m_LogString, "是否显示英文错误[%d],请修改。程序自动修正为0", LfoPara.is_eng);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "是否显示英文[%d]", LfoPara.is_eng);
		MessShow(m_LogString, logtype);
	}
	if (LfoPara.sh_fact >= 0)
	{
		sprintf(m_LogString, "参与因子阀值[%f]", LfoPara.sh_fact);
		MessShow(m_LogString, logtype);
	}
	if (LfoPara.sh_gn_pow >= 0)
	{
		sprintf(m_LogString, "机组耗散功率阀值[%f]", LfoPara.sh_gn_pow);
		MessShow(m_LogString, logtype);
	}
	if (LfoPara.sh_ln_pow >= 0)
	{
		sprintf(m_LogString, "支路耗散功率阀值[%f]", LfoPara.sh_ln_pow);
		MessShow(m_LogString, logtype);
	}

	if (LfoPara.cohcy_angl <= 0 || LfoPara.cohcy_angl >= 90)
	{
		sprintf(m_LogString, "同调角度差[%f]有误,自动修正成", LfoPara.sh_phas);
		MessShow(m_LogString, logtype);
		LfoPara.cohcy_angl = COHCY_ANGL;
	}
	if (LfoPara.sh_phas >= 0 && LfoPara.sh_phas <= 90)
	{
		//		LfoPara.sh_phas = (float)(LfoPara.sh_phas*PRONY_PI/180);
		sprintf(m_LogString, "相角超前阀值[%f]", LfoPara.sh_phas);
		MessShow(m_LogString, logtype);
	}
	if (LfoPara.osc_pow_ampl >= 0)
	{
		sprintf(m_LogString, "小扰动振幅门槛值[%f]", LfoPara.osc_pow_ampl);
		MessShow(m_LogString, logtype);
	}
	if (LfoPara.osc_sec_ampl > 10.0)
	{
		LfoPara.osc_sec_ampl = 10.0;
		sprintf(m_LogString, "小扰动持续时间门槛值[%f],请修改。程序自动修正为10.0", LfoPara.osc_sec_ampl);
		MessShow(m_LogString, logtype);
	}
	else
	{
		//		nFlag ++;
		sprintf(m_LogString, "小扰动持续时间门槛值为[%f]", LfoPara.osc_sec_ampl);
		MessShow(m_LogString, logtype);
	}
	//广东项目增加
	if (LfoPara.gn_wn_ampl1 > 0)
	{

	}
	if (LfoPara.ln_wn_ampl1 > 0)
	{

	}
	if (LfoPara.tr_wn_ampl1 > 0)
	{

	}
	if (LfoPara.ln_swn_ampl1 > 0)
	{

	}
	if (LfoPara.ln_swn_ampl2 > 0)
	{

	}
	if (LfoPara.ln_swn_ampl3 > 0)
	{

	}
	if (LfoPara.ln_ewn_ampl1 > 0)
	{

	}
	if (LfoPara.ln_ewn_ampl2 > 0)
	{

	}
	if (LfoPara.ln_ewn_ampl3 > 0)
	{

	}
	if (LfoPara.tr_swn_ampl1 > 0)
	{

	}
	if (LfoPara.tr_swn_ampl2 > 0)
	{

	}
	if (LfoPara.tr_swn_ampl3 > 0)
	{

	}
	if (LfoPara.tr_ewn_ampl1 > 0)
	{

	}
	if (LfoPara.tr_ewn_ampl2 > 0)
	{

	}
	if (LfoPara.tr_ewn_ampl3 > 0)
	{

	}

	if (LfoPara.rec_num > 0)
	{

	}
	if (LfoPara.if_flit_lfo != 1 || LfoPara.if_flit_lfo != 0)
	{

	}
	if (nFlag > 0)
	{
		return -1;
	}
	return 0;
}

int CLfoAnaBase::CalcPara(LFO_PARA_STRUCT& LfoPara)
{
	if (LfoPara.thread_mode == 0)
	{
		// 观察时间窗口
		LfoPara.time_leng = (int)(LfoPara.time_leng_coe / LfoPara.freq_min + 0.5f);

		if (LfoPara.time_leng < LfoPara.time_leng_min)
		{
			LfoPara.time_leng = LfoPara.time_leng_min;
		}
		if (LfoPara.time_leng > LfoPara.time_leng_min && LfoPara.time_leng < 10)
		{
			LfoPara.time_leng = 10;
		}
		if (LfoPara.time_leng > 10 && LfoPara.time_leng < 20)
		{
			LfoPara.time_leng = 20;
		}
		if (LfoPara.time_leng > 20 && LfoPara.time_leng < LfoPara.time_leng_max)
		{
			LfoPara.time_leng = LfoPara.time_leng_max;
		}
		if (LfoPara.time_leng > LfoPara.time_leng_max)
		{
			LfoPara.time_leng = LfoPara.time_leng_max;
		}

		// 采样频率
		LfoPara.sample_freq = LfoPara.sample_freq_coe * LfoPara.freq_max;
		if (LfoPara.sample_freq < LfoPara.sample_freq_min)
		{
			LfoPara.sample_freq = LfoPara.sample_freq_min;
		}
		if (LfoPara.sample_freq > LfoPara.sample_freq_max)
		{
			LfoPara.sample_freq = LfoPara.sample_freq_max;
		}

		// 交叠窗口
		LfoPara.int_time = (int)(LfoPara.int_time_coe * LfoPara.time_leng);
		if (LfoPara.time_leng <= LfoPara.time_leng_min)
		{
			// 窗口长度小于5秒,不交叠
			LfoPara.int_time = 0;
		}
		if (LfoPara.int_time < LfoPara.int_time_min)
		{
			LfoPara.int_time = LfoPara.int_time_min;
		}
		if (LfoPara.int_time > LfoPara.int_time_max)
		{
			LfoPara.int_time = LfoPara.int_time_max;
		}
	}
	else if (LfoPara.thread_mode == 1)
	{
		// 观察时间窗口
		LfoPara.time_leng = (int)(LfoPara.time_leng_coe / LfoPara.freq_min + 0.5f);
		if (LfoPara.time_leng < LfoPara.time_leng_min)
		{
			LfoPara.time_leng = LfoPara.time_leng_min;
		}
		if (LfoPara.time_leng > LfoPara.time_leng_max)
		{
			LfoPara.time_leng = LfoPara.time_leng_max;
		}

		// 采样频率
		LfoPara.sample_freq = LfoPara.sample_freq_coe * LfoPara.freq_max;
		if (LfoPara.sample_freq < 2.5)
		{
			LfoPara.sample_freq = 2.5;
		}
		else if (LfoPara.sample_freq > 2.5 && LfoPara.sample_freq < 3.125)
		{
			LfoPara.sample_freq = 3.125;
		}
		else if (LfoPara.sample_freq > 3.125 && LfoPara.sample_freq < 5)
		{
			LfoPara.sample_freq = 5;
		}
		else if (LfoPara.sample_freq > 5 && LfoPara.sample_freq < 6.125)
		{
			LfoPara.sample_freq = 6.125;
		}
		else if (LfoPara.sample_freq > 6.125 && LfoPara.sample_freq < 12.5)
		{
			LfoPara.sample_freq = 12.5;
		}
		else if (LfoPara.sample_freq > 12.5 && LfoPara.sample_freq < 25)
		{
			LfoPara.sample_freq = 25;
		}
		else if (LfoPara.sample_freq > LfoPara.sample_freq_max)
		{
			LfoPara.sample_freq = LfoPara.sample_freq_max;
		}

		// 交叠窗口
		LfoPara.int_time = (int)(LfoPara.int_time_coe * LfoPara.time_leng);
		if (LfoPara.time_leng <= LfoPara.time_leng_min)
		{
			// 窗口长度小于5秒,不交叠
			LfoPara.int_time = 0;
		}
		if (LfoPara.int_time < LfoPara.int_time_min)
		{
			LfoPara.int_time = LfoPara.int_time_min;
		}
		if (LfoPara.int_time > LfoPara.int_time_max)
		{
			LfoPara.int_time = LfoPara.int_time_max;
		}
	}


	sprintf(m_LogString, "freq[%.2f~%.2f],time_leng[%d](%.2f)(%d~%d),sample_freq[%.3f](%.3f)(%.3f~%.3f),int_time[%d](%.2f)(%d~%d)",
		LfoPara.freq_min, LfoPara.freq_max, LfoPara.time_leng, LfoPara.time_leng_coe, LfoPara.time_leng_min, LfoPara.time_leng_max,
		LfoPara.sample_freq, LfoPara.sample_freq_coe, LfoPara.sample_freq_min, LfoPara.sample_freq_max,
		LfoPara.int_time, LfoPara.int_time_coe, LfoPara.int_time_min, LfoPara.int_time_max);
	MessShow(m_LogString, LTINFO);

	return 0;
}

//************************************
// Method:    GetDevCtlInfo
// FullName:  CLfoAnaBase::GetDevCtlInfo
// Access:    public 
// Returns:   int
// Qualifier:
// 对监视的设备，填充设备基本信息，PMU测点信息。
// 输入：LFO设备表中控制参数，设备信息vector<LFO_DEV_INFO> m_VecMonDev
// 输出：填充vector<LFO_DEV_INFO> m_VecMonDev控制信息和数据状态
// 若设备未监视，状态置为(#define	MENU_LFO_DEV_STS_NO	6	/* 未监视 */)
//************************************
int CLfoAnaBase::GetDevCtlInfo(vector<LFO_DEV_INFO>& VecDev)
{
	int ii, nFlag;
	// 	sprintf(m_LogString,"[yjj]GetDevCtlInfo获取设备个数[%d],数据源[%d]",(int)VecDev.size(),m_LfoPara.data_source);
	// 	MessShow(m_LogString,LTERROR);
		//LFO_DEV_INFO tmpDevInfo;
	if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
	{
		// 若从数据库读取设备信息，根据控制标志确定是否监视。
		nFlag = 0;
		// 获取厂站控制信息
		// 		if (GetFacCtlInfo(VecDev)<0)
		// 		{
		// 			nFlag --;
		// 			sprintf(m_LogString,"获取厂站控制信息失败");
		// 			MessShow(m_LogString,LTERROR);
		// 		}

		// 获取发电机控制信息
		if (GetGenCtlInfo(VecDev) < 0)
		{
			nFlag--;
			sprintf(m_LogString, "获取发电机控制信息失败");
			MessShow(m_LogString, LTERROR);
		}

		// 获取交流线路控制信息
		if (GetLineCtlInfo(VecDev) < 0)
		{
			nFlag--;
			sprintf(m_LogString, "获取交流线路控制信息失败");
			MessShow(m_LogString, LTERROR);
		}

		// 获取变压器控制信息
		if (GetTrCtlInfo(VecDev) < 0)
		{
			nFlag--;
			sprintf(m_LogString, "获取交流线路控制信息失败");
			MessShow(m_LogString, LTERROR);
		}

		if (nFlag < 0)
		{
			return -1;
		}
	}
	else
	{
		// 若从文件读取设备信息，将匹配名称的设备置为监视，其他设备置为未监视。
		// 读取数据文件中设备名称
		// 设备名称匹配的设为监视
		for (ii = 0; ii < (int)VecDev.size(); ii++)
		{
			VecDev[ii].stat = MENU_LFO_DEV_STS_OK;
			// 			VecDev[ii].osc_stat = MENU_LFO_DEV_STS_OK;
			//读文件时获取设备是否监视信息?????????
			// 			if (VecDev[ii].dev.dev_id == 116530640525984342)
			// 			{
			// 				VecDev[ii].stat = MENU_LFO_DEV_STS_NO;
			// 				VecDev[ii].osc_stat = MENU_LFO_DEV_STS_NO;
			// 			}
						// #define	MENU_LFO_DEV_STS_NOYC	2	/* 无测点 */
			// 			if (VecDev[ii].stat == MENU_LFO_DEV_STS_NOYC)
			// 			{
			// 				continue;
			// 			}
			// 			nFlag = 0;
			// 			tmp_yc_name = VecDev[ii].dev.yc_name[12];
			// 			for ( jj =0; jj<(int)data_devname.size(); jj++)
			// 			{
			// 				if (tmp_yc_name == data_devname[jj])
			// 				{
			// 					nFlag = 1;
			// 					// 若设备为监视标志，状态置为(#define	MENU_LFO_DEV_STS_OK	1	/* 正常 */)
			// 					VecDev[ii].stat = MENU_LFO_DEV_STS_OK;
			// 				}
			// 			}
			// 			if (nFlag == 0)
			// 			{
			// 				// 若设备未监视，状态置为(#define	MENU_LFO_DEV_STS_NO	6	/* 未监视 */)
			// 				VecDev[ii].stat = MENU_LFO_DEV_STS_NO;
			// 				sprintf(m_LogString,"设备未监视");
			// 				MessShow(m_LogString,LTINFO);
			// 			}
		}
	}
	// 	sprintf(m_LogString,"[yjj]111GetDevCtlInfo获取设备个数[%d],数据源[%d]",(int)VecDev.size(),m_LfoPara.data_source);
	// 	MessShow(m_LogString,LTERROR);
	return 0;
}

//************************************
// Method:    GetFacCtlInfo
// FullName:  CLfoAnaBase::GetFacCtlInfo
// Access:    public 
// Returns:   int
// Qualifier:
// 若从文件读取设备信息，将所有设备置为监视。
// 若从数据库读取设备信息，根据控制标志确定是否监视。
// 对监视的设备，填充设备基本信息。
//************************************
int CLfoAnaBase::GetFacCtlInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	// 读取厂站监视标志
	vector<INT_REC> vec_fac;
	// 将监视标志赋值给设备信息
	int ii, jj, nFlag;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		// #define	MENU_LFO_DEV_STS_NOYC	2	/* 无测点 */
		if (VecMonDev[ii].stat == MENU_LFO_DEV_STS_NOYC)
		{
			continue;
		}
		nFlag = 0;
		for (jj = 0; jj < (int)vec_fac.size(); jj++)
		{
			if (VecMonDev[ii].dev.dev_id == vec_fac[jj])
			{
				nFlag = 1;
				//if (vec_fac[jj].is_monitor)
				{
				}

				// 根据选择开关确定分析端点，更新选择端点ID和PMU配置信息
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "未找到监视设备控制信息");
			MessShow(m_LogString, LTERROR);
		}
	}

#endif

	return 0;
}

int CLfoAnaBase::GetGenCtlInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	// 读取发电机监视标志
	vector<LFO_GN_TAB_CRE> VecLfoGen;
	GetLfoGen(VecLfoGen);
	int temf = 0;
	// 将监视标志赋值给设备信息
	int ii, jj, nFlag;
	for (jj = 0; jj < (int)VecLfoGen.size(); jj++)
	{
		nFlag = 0;
		for (ii = 0; ii < (int)VecMonDev.size(); ii++)
		{
			// 			if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN)
			// 			{
			// 				continue;
			// 			}
			if (VecMonDev[ii].dev.dev_id == VecLfoGen[jj].gn_id)
			{
				nFlag = 1;
				VecMonDev[ii].dev.ampl_limit_s = VecLfoGen[jj].lfo_gn_swn;
				VecMonDev[ii].dev.ampl_limit_e = VecLfoGen[jj].lfo_gn_ewn;
#ifdef _CHG_AMPL
				VecMonDev[ii].dev.is_chg_ampl = VecLfoGen[jj].is_chg_ampl;
				VecMonDev[ii].dev.chg_ampl_per = VecLfoGen[jj].chg_ampl_per;
#endif
				if (VecLfoGen[jj].is_monitor == MENU_LFO_MENU_NO)
				{
					VecMonDev[ii].stat = MENU_LFO_DEV_STS_NO;
					// 					VecMonDev[ii].osc_stat = MENU_LFO_DEV_STS_NO;
				}
				// TODO:根据选择开关确定分析端点，更新选择端点ID和PMU配置信息
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "设备表中出现新增发电机["INT_REC_FMT"]", INT_REC_VAL(VecLfoGen[jj].gn_id));
			MessShow(m_LogString, LTERROR);
			m_LfoPara.change_dev_num = 1;
		}
		// 		sprintf(m_LogString,"[yjj]机组设备["INT_REC_FMT",%s]状态[%d][%d][%d]",INT_REC_VAL(VecMonDev[ii].dev.dev_id),VecMonDev[ii].dev.dev_name,temf,VecMonDev[ii].stat,VecMonDev[ii].stat_old);
		// 		MessShow(m_LogString,LTERROR);
	}

#endif

	return 0;
}

int CLfoAnaBase::GetLineCtlInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	// 读取LFO交流线路监视标志
	vector<LFO_LN_TAB_CRE> VecLfoLine;
	GetLfoLine(VecLfoLine);
	int temf = 0;
	// 将监视标志赋值给设备信息
	int ii, jj, nFlag;
	for (jj = 0; jj < (int)VecLfoLine.size(); jj++)
	{
		nFlag = 0;
		for (ii = 0; ii < (int)VecMonDev.size(); ii++)
		{
			// 			if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_LINE)
			// 			{
			// 				continue;
			// 			}
			if (VecMonDev[ii].dev.dev_id == VecLfoLine[jj].acln_id)
			{
				nFlag = 1;
				VecMonDev[ii].dev.ampl_limit_s = VecLfoLine[jj].lfo_ln_swn;
				VecMonDev[ii].dev.ampl_limit_e = VecLfoLine[jj].lfo_ln_ewn;
#ifdef _CHG_AMPL
				VecMonDev[ii].dev.is_chg_ampl = VecLfoLine[jj].is_chg_ampl;
				VecMonDev[ii].dev.chg_ampl_per = VecLfoLine[jj].chg_ampl_per;
#endif
				if (VecLfoLine[jj].is_monitor == MENU_LFO_MENU_NO)
				{
					VecMonDev[ii].stat = MENU_LFO_DEV_STS_NO;
					// 					VecMonDev[ii].osc_stat = MENU_LFO_DEV_STS_NO;
				}
				// 根据选择开关确定分析端点，更新选择端点ID和PMU配置信息
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "设备表中出现新增线路["INT_REC_FMT"]", INT_REC_VAL(VecLfoLine[jj].acln_id));
			MessShow(m_LogString, LTERROR);
			m_LfoPara.change_dev_num = 1;
		}
		// 		sprintf(m_LogString,"[yjj]线路设备["INT_REC_FMT",%s]状态[%d][%d][%d]",INT_REC_VAL(VecMonDev[ii].dev.dev_id),VecMonDev[ii].dev.dev_name,temf,VecMonDev[ii].stat,VecMonDev[ii].stat_old);
		// 		MessShow(m_LogString,LTERROR);
	}

#endif

	return 0;
}

int CLfoAnaBase::GetTrCtlInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	// 读取LFO变压器监视标志
	vector<LFO_TR_TAB_CRE> VecLfoTr;
	GetLfoTr(VecLfoTr);

	// 将监视标志赋值给设备信息
	int ii, jj, nFlag;
	for (jj = 0; jj < (int)VecLfoTr.size(); jj++)
	{
		nFlag = 0;
		for (ii = 0; ii < (int)VecMonDev.size(); ii++)
		{
			// 			if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_TR)
			// 			{
			// 				continue;
			// 			}
			if (VecMonDev[ii].dev.dev_id == VecLfoTr[jj].tr_id)
			{
				nFlag = 1;
				VecMonDev[ii].dev.ampl_limit_s = VecLfoTr[jj].lfo_tr_swn;
				VecMonDev[ii].dev.ampl_limit_e = VecLfoTr[jj].lfo_tr_ewn;
#ifdef _CHG_AMPL
				VecMonDev[ii].dev.is_chg_ampl = VecLfoTr[jj].is_chg_ampl;
				VecMonDev[ii].dev.chg_ampl_per = VecLfoTr[jj].chg_ampl_per;
#endif
				if (VecLfoTr[jj].is_monitor == MENU_LFO_MENU_NO)
				{
					VecMonDev[ii].stat = MENU_LFO_DEV_STS_NO;
					// 					VecMonDev[ii].osc_stat = MENU_LFO_DEV_STS_NO;
				}
				// 根据选择开关确定分析端点，更新选择端点ID和PMU配置信息
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "设备表中出现新增变压器["INT_REC_FMT"]", INT_REC_VAL(VecLfoTr[jj].tr_id));
			MessShow(m_LogString, LTERROR);
			m_LfoPara.change_dev_num = 1;
		}
	}

#endif

	return 0;
}
//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    GetPMUTime
// FullName:  CLfoAnaBase::GetPMUTime
// Access:    public 
// Returns:   int
// Qualifier:
// 若从动态数据库获取PMU时间，可调用PMU数据缓存接口。
// 若从文件获取数据，则从文件读取该数据时间，测试时间列第一行数据指定该数据时间，第二行数据指定采样间隔时间。
// 输入：数据源参数，系统状态结构(LFO_SYS_INFO m_MonSys)
// 输出：填充系统状态结构(LFO_SYS_INFO m_MonSys)中PMU最新时刻m_CurGpsTime，分析开始时间m_StartTime，分析结束时间m_EndTime
//************************************
int CLfoAnaBase::ShowPMUTime(LFO_SYS_INFO& MonSys)
{
	//     if (1 == m_LfoPara.data_source)
	//     {
	//         GetTimeFromDB(MonSys);
	//     }
	//     else
	//     {
	//         GetTimeFromFile(MonSys);
	//     }
	char tmptime1[10], tmptime2[10], tmptime3[10];
	sprintf(tmptime1, "%s", PT_GetHMSTimeStringBySeconds(MonSys.gps_time));
	sprintf(tmptime2, "%s", PT_GetHMSTimeStringBySeconds(MonSys.start_time));
	sprintf(tmptime3, "%s", PT_GetHMSTimeStringBySeconds(MonSys.end_time));
	sprintf(m_LogString, "PMU最新时间[%d][%s],本轮分析时间["RTDB_TIME_FMT"~"RTDB_TIME_FMT"][%s~%s]",
		MonSys.gps_time, tmptime1, MonSys.start_time, MonSys.end_time, tmptime2, tmptime3);
	MessShow(m_LogString, LTINFO);
	return 0;
}

int CLfoAnaBase::GetTimeFromFile(LFO_SYS_INFO& MonSys, CDataFile& df)
{
	// 从文件读取数据，分析开始时刻从最新时刻开始向后计算
	// 初次启动
	if (MonSys.gps_time_old <= 0)
	{
		// 若数据文件中指定时间为零，则采用当前时间(当前时间减去延迟)
		if (df.m_DataHead.start_time <= 0)
		{
			MonSys.gps_time = PT_GetNowSeconds() - 60;
		}
		else
		{
			// 否则采用数据文件中指定时间
			MonSys.gps_time = df.m_DataHead.start_time;
		}
	}
	else
	{
		// 循环读取数据，则根据上一次最新时刻计算本次最新时刻
		MonSys.gps_time = MonSys.gps_time_old + m_LfoPara.time_leng - m_LfoPara.int_time;
	}

	if (MonSys.gps_time <= 0)
	{
		sprintf(m_LogString, "GPS时间错误[%d]", MonSys.gps_time);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	MonSys.start_time = MonSys.gps_time;
	MonSys.end_time = MonSys.gps_time + m_LfoPara.time_leng;
	MonSys.sample_org = df.m_DataHead.time_sample;

	//计算进度
	if (df.m_DataHead.start_time > 0 && df.m_DataHead.end_time > 0)
	{
		MonSys.progress = float((MonSys.end_time - df.m_DataHead.start_time)) / float((df.m_DataHead.end_time - df.m_DataHead.start_time)) * 100;
	}

	if (MonSys.start_time <= 0 || MonSys.end_time <= 0 || MonSys.end_time < MonSys.start_time)
	{
		sprintf(m_LogString, "分析时间错误["RTDB_TIME_FMT"~"RTDB_TIME_FMT"]", MonSys.start_time, MonSys.end_time);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ShowPMUTime(MonSys);

	return 0;
}

int CLfoAnaBase::GetTimeFromDB(LFO_SYS_INFO& MonSys)
{
	int i;
	int	LastRecTime;
	// 从数据库读取数据，分析开始时刻从最新时刻开始向前计算
	// 初次启动
	if (MonSys.gps_time_old <= 0)
	{
		// 当前时间减去延迟
		MonSys.gps_time = PWD_GetLastTime() - m_LfoPara.delay_time;
	}
	else
	{
		// 循环读取数据，则根据上一次最新时刻计算本次最新时刻
// 		MonSys.gps_time = MonSys.gps_time_old + m_LfoPara.time_leng - m_LfoPara.int_time;
// 		int tmp_gps_time = PWD_GetLastTime() - m_LfoPara.delay_time;
// 		if (MonSys.gps_time > tmp_gps_time)
// 		{
// 			// 若时间未更新，则循环等待
// 			while(true)
// 			{
// 				Sleep(1000);
// 				tmp_gps_time = PWD_GetLastTime() - m_LfoPara.delay_time;
// 				if (MonSys.gps_time < tmp_gps_time )
// 				{
// 					break;
// 				}
// 			}
// 		}
// 		if (MonSys.gps_time <=0 )
// 		{
// 			sprintf(m_LogString,"GPS时间错误[%d]",MonSys.gps_time);
// 			MessShow(m_LogString,LTERROR);
// 			return -1;
// 		}
		//修改为和低频振荡相同的处理方式，由最新的PMU时刻计算本次最新时刻
		MonSys.gps_time = PWD_GetLastTime() - m_LfoPara.delay_time;
		if (MonSys.gps_time <= MonSys.gps_time_old)
		{
			PT_DelayMinSec(1100);
			//			PT_DelayMinSec(500);
			MonSys.gps_time = PWD_GetLastTime() - m_LfoPara.delay_time;
		}
		if (MonSys.gps_time <= 0)
		{
			sprintf(m_LogString, " PMU BUFFER NOT UPDATE! ");
			MessShow(m_LogString);
			for (i = 0; i < m_VecDev.size(); i++)
			{
				if (m_VecDev[i].stat != MENU_LFO_DEV_STS_NOYC)
				{
					m_VecDev[i].stat = MENU_LFO_DEV_STS_OLD;
					// 					m_VecDev[i].osc_stat     = MENU_LFO_DEV_STS_OLD;
				}
			}

			MonSys.gps_time = -1;
			PT_DelayMinSec(2000);
			return -1;
		}
		if (MonSys.stat_old > MENU_LFO_DEV_STS_SCT)  // 系统不安全情况下
		{
			if (MonSys.gps_time <= MonSys.gps_time_old)
			{
				sprintf(m_LogString, " PMU BUFFER NOT UPDATE! ");
				MessShow(m_LogString);
				for (i = 0; i < m_VecDev.size(); i++)
				{
					if (m_VecDev[i].stat != MENU_LFO_DEV_STS_NOYC)
					{
						m_VecDev[i].stat = MENU_LFO_DEV_STS_OLD;
						// 						m_VecDev[i].osc_stat     = MENU_LFO_DEV_STS_OLD;
					}
				}
				PT_DelayMinSec(2000);
				//	PT_DelayMinSec(500);
				return -1;
			}
			else
			{
				sprintf(m_LogString, " 等待时间刷新1,cur=%d,old=%d,leng=%d,int=%d", MonSys.gps_time, MonSys.gps_time_old, m_LfoPara.time_leng, m_LfoPara.int_time);
				MessShow(m_LogString);
				if (MonSys.gps_time < (MonSys.gps_time_old + m_LfoPara.time_leng - m_LfoPara.int_time))
				{
					while (MonSys.gps_time < (MonSys.gps_time_old + m_LfoPara.time_leng - m_LfoPara.int_time))
					{
						PT_DelayMinSec(1000);
						//	PT_DelayMinSec(500); 
						LastRecTime = PWD_GetLastTime();

						if (LastRecTime <= 0)
						{
							sprintf(m_LogString, " PMU BUFFER NOT UPDATE! ");
							MessShow(m_LogString);
							for (i = 0; i < m_VecDev.size(); i++)
							{
								if (m_VecDev[i].stat != MENU_LFO_DEV_STS_NOYC)
								{
									m_VecDev[i].stat = MENU_LFO_DEV_STS_OLD;
									// 									m_VecDev[i].osc_stat     = MENU_LFO_DEV_STS_OLD;
								}
							}
							MonSys.gps_time = -1;
							return -1;
						}
						MonSys.gps_time = LastRecTime - m_LfoPara.delay_time;   // LastRecTime - 1; 
					}
				}
				else
				{
					MonSys.gps_time = (MonSys.gps_time_old + m_LfoPara.time_leng - m_LfoPara.int_time);
				}
			}
		}
		else   // 系统安全情况下
		{
			if (MonSys.gps_time > (MonSys.gps_time_old + m_LfoPara.time_leng + m_LfoPara.time_leng - m_LfoPara.int_time))
			{
				sprintf(m_LogString, " HandlingPmuSampleData(): Calc windows lag behind %d[s], Auto Correct!", ((int)(m_LfoPara.time_leng)));
				MessShow(m_LogString);
			}
			else
			{
				if (MonSys.gps_time <= MonSys.gps_time_old)
				{
					for (i = 0; i < m_VecDev.size(); i++)
					{
						sprintf(m_LogString, " PMU BUFFER NOT UPDATE! m_OldGpsTime[%d], m_CurGpsTime[%d]", MonSys.gps_time_old, MonSys.gps_time);
						MessShow(m_LogString);

						if (m_VecDev[i].stat != MENU_LFO_DEV_STS_NOYC)
						{
							m_VecDev[i].stat = MENU_LFO_DEV_STS_OLD;
							// 							m_VecDev[i].osc_stat     = MENU_LFO_DEV_STS_OLD;
						}
					}

					PT_DelayMinSec(2000);
					//PT_DelayMinSec(500);
					return -1;
				}
				else
				{
					sprintf(m_LogString, " 等待时间刷新2,cur=%d,old=%d,leng=%d,int=%d", MonSys.gps_time, MonSys.gps_time_old, m_LfoPara.time_leng, m_LfoPara.int_time);
					MessShow(m_LogString);
					while (MonSys.gps_time < (MonSys.gps_time_old + m_LfoPara.time_leng - m_LfoPara.int_time))
					{
						PT_DelayMinSec(1000);
						//PT_DelayMinSec(500); 

						PWD_GetLastTime(LastRecTime);
						if (LastRecTime <= 0)
						{
							sprintf(m_LogString, " PMU BUFFER NOT UPDATE! ");
							MessShow(m_LogString);
							for (i = 0; i < m_VecDev.size(); i++)
							{
								if (m_VecDev[i].stat != MENU_LFO_DEV_STS_NOYC)
								{
									m_VecDev[i].stat = MENU_LFO_DEV_STS_OLD;
									// 									m_VecDev[i].osc_stat     = MENU_LFO_DEV_STS_OLD;
								}
							}
							MonSys.gps_time = -1;
							return -1;
						}
						MonSys.gps_time = LastRecTime - m_LfoPara.delay_time;   // 高速缓存区存储数据有延时 
					}
				}
			}
		}
	}


	MonSys.start_time = MonSys.gps_time - m_LfoPara.time_leng;
	MonSys.end_time = MonSys.gps_time;
	MonSys.gps_time_old = MonSys.gps_time;

	if (MonSys.start_time <= 0 || MonSys.end_time <= 0 || MonSys.end_time < MonSys.start_time)
	{
		sprintf(m_LogString, "分析时间错误["RTDB_TIME_FMT"~"RTDB_TIME_FMT"]", MonSys.start_time, MonSys.end_time);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ShowPMUTime(MonSys);

	return 0;
}

//************************************
// Method:    GetPMUData
// FullName:  CLfoAnaBase::GetPMUData
// Access:    public 
// Returns:   int
// Qualifier:
// 若设备信息从数据库中读取，而PMU数据从文件中读取，需先读取数据目录的设备信息文件，获取变量名，
// 再读取数据文件，根据设备信息文件中变量名与数据文件中数据对应。
// 若设备信息从文件读取，已获取变量名，可直接读取数据文件。
// 若从动态数据库中获取PMU数据，直接根据文件号和列号调用接口。
// 调用接口函数获取设备PMU数据，去掉未监视的设备
// 输入：数据源参数，设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)
// 输出：填充设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)中PMU数据
//************************************
int CLfoAnaBase::GetPMUData(vector<LFO_DEV_INFO> VecMonDev)
{
	//     if (1 == m_LfoPara.data_source)
	//     {
	//         GetDataFromDB(VecMonDev);
	//     }
	//     else
	//     {
	//         GetDataFromFile(VecMonDev);
	//     }

	return 0;
}

int CLfoAnaBase::GetDataFromFile(vector<LFO_DEV_INFO>& VecMonDev, CDataFile& df)
{
	// 从文件获取数据，直接幅值给v2d_data,不判断数据状态
	// 读取数据目录中数据文件
	// CDataFile df(".\\voltdata.csv");
	// 填充对应设备数据
	// 根据设备测量量名称（厂站名称+设备名+测量量名）匹配对应数据
//     vector<float> vData;
//     int iSize = df.GetData("时间", vData);
//     填充vector<LFO_DEV_INFO> m_VecMonDev	

	int ii, jj, tableno, nColNum, nFlag = 0;
	string::size_type idx;
	string tmpdevname, tmpColName;
	string tmpstring;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		nColNum = -1;
		if (VecMonDev[ii].stat != MENU_LFO_DEV_STS_OK)
		{
			continue;
		}

		// 		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
		// 		{
		// 			// 暂不处理厂站
		// 			VecMonDev[ii].stat = MENU_LFO_DEV_STS_BAD;
		// 			continue;
		// 		}


				//修改读取数据模块
		for (jj = 0; jj < (int)df.m_DataHead.vec_devid_s.size(); jj++)
		{
			// 获取设备ID
// 			tmpstring = df.m_DataHead.vec_devid_s[jj];
			INT_REC tmpdev_id = df.m_DataHead.vec_devid_s[jj];
			if (tmpdev_id == VecMonDev[ii].dev.dev_id)
			{
				// 去掉前两列"日期时间,毫秒"
				//				tmpstring.clear();
				if (df.GetNumberOfVariables() >= jj + 2)
				{
					df.GetVariableName(jj + 2, tmpstring);
				}
				idx = tmpstring.find_last_of("-");
				// 获取设备名称
				if (idx != string::npos)
				{
					tmpdevname = tmpstring.substr(0, idx);
				}
				// 判断测点类型
				tmpColName = -1;//测点类型赋初值
				tmpColName = tmpstring.substr(idx + 1);
				// 去掉每行末尾的","
				idx = tmpColName.find_first_of(",");
				if (idx != string::npos)
				{
					tmpColName = tmpColName.substr(0, idx);
				}
				if (tmpColName == "00P")
				{
					// 有功
					nColNum = 2;
				}
				else if (tmpColName == "00Q")
				{
					// 无功
					nColNum = 3;
				}
				else if (tmpColName == "UAV相角")
				{
					// 电压相角
					nColNum = 1;
				}
				else if (tmpColName == "UAV幅值")
				{
					// 电压幅值
					nColNum = 0;
				}
				else if (tmpColName == "EEV相角")
				{
					// 发电机功角
					nColNum = 16;
				}
				else
				{
					// 未定义，默认为无效
					nColNum = -1;
				}

				if (nColNum == 2)
				{
					// 获取设备数据
// 					if (jj%1000 == 0 && jj >= 1000)
// 					{
// 						sprintf(m_LogString,"jj=%d,v2d_data.size()=%d,v2d_data.capacity=%d",
// 							jj,VecMonDev[ii].v2d_data.size(),VecMonDev[ii].v2d_data.capacity());
// 						MessShow(m_LogString,LTDEBUG);
// 					}
					if (df.GetData(jj + 2, VecMonDev[ii].v2d_data) == -1)
					{
						sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
							tmpstring.c_str(), INT_REC_VAL(tmpdev_id), df.GetLastError());
						MessShow(m_LogString, LTDEBUG);
					}
#ifdef _LFO_HIS_LIB_
					if (df.m_DataHead.vec_devvolt_s.size() > jj)
					{
						VecMonDev[ii].dev_info.maxvbase = atof(df.m_DataHead.vec_devvolt_s[jj].c_str());
						// 						sprintf(m_LogString,"[%s]maxvbase=[%f]",tmpstring.c_str(),VecMonDev[ii].dev_info.maxvbase);
						// 						MessShow(m_LogString);
					}
#endif
					continue;
					// 					if (VecMonDev[ii].dev.dev_id == 115686216317272154)
					// 					{
					// 						for (int kk = 0;kk<VecMonDev[ii].v2d_data.size();kk++)
					// 						{
					// 							sprintf(m_LogString,"[%s]有功功率=[%f]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].v2d_data[kk]);
					// 							MessShow(m_LogString);
					// 						}
					// 					}
				}
				if (nColNum == 0)
				{
					if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_FAC)
					{
						continue;
					}
					// 获取设备数据
					VecMonDev[ii].v2d_data.clear();
					if (df.GetData(jj + 2, VecMonDev[ii].v2d_data) == -1)
					{
						sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
							tmpstring.c_str(), INT_REC_VAL(tmpdev_id), df.GetLastError());
						MessShow(m_LogString, LTDEBUG);
					}
					continue;
					// 					sprintf(m_LogString,"[%s]电压相角数据[%d]",tmpstring.c_str(),VecMonDev[ii].v2d_data.size());
					// 					MessShow(m_LogString);
					// 					for (int kk = 0;kk<VecMonDev[ii].v2d_data.size();kk++)
					// 					{
					// 						sprintf(m_LogString,"[%f]",VecMonDev[ii].v2d_data[kk]);
					// 						MessShow(m_LogString);
					// 					}
				}
				if (nColNum == 1)
				{
					// 					if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_FAC)
					// 					{
					// 						continue;
					// 					}
										// 获取设备数据
					VecMonDev[ii].ana_data.clear();
					if (df.GetData(jj + 2, VecMonDev[ii].ana_data) == -1)
					{
						sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
							tmpstring.c_str(), INT_REC_VAL(tmpdev_id), df.GetLastError());
						MessShow(m_LogString, LTDEBUG);
					}
					continue;
					// 					sprintf(m_LogString,"[%s]电压相角数据[%d]",tmpstring.c_str(),VecMonDev[ii].v2d_data.size());
					// 					MessShow(m_LogString);
					// 					for (int kk = 0;kk<VecMonDev[ii].v2d_data.size();kk++)
					// 					{
					// 						sprintf(m_LogString,"[%f]",VecMonDev[ii].v2d_data[kk]);
					// 						MessShow(m_LogString);
					// 					}
				}
			}
		}
		usleep(5);
		// 	for ( ii =0; ii<(int)VecMonDev.size(); ii++ )
		// 	{
		// 		sprintf(m_LogString,"[yjj]GetDataFromFile设备状态[%d]",VecMonDev[ii].stat);
		// 		MessShow(m_LogString,LTERROR);
		// 		if (VecMonDev[ii].stat != MENU_LFO_DEV_STS_OK)
		// 		{
		// 			continue;
		// 		}
		// 		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
		// 		{
		// 			// 暂不处理厂站
		// 			VecMonDev[ii].stat = MENU_LFO_DEV_STS_BAD;
		// 			continue;
		// 		}
		//  		sprintf(m_LogString,"[yjj]开始进入函数GetData");
		//  		MessShow(m_LogString,LTERROR);
		// 		if( df.GetData(VecMonDev[ii].dev.yc_name[12], VecMonDev[ii].v2d_data) == -1)
		// 		{
		// 			nFlag ++;
		// 			// 置数据状态
		// 			VecMonDev[ii].stat = MENU_LFO_DEV_STS_BAD;
		// 			sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
		// 				VecMonDev[ii].dev.dev_name,VecMonDev[ii].dev.dev_id,VecMonDev[ii].dev.yc_name[12],df.GetLastError());
		// 			MessShow(m_LogString,LTDEBUG);
		// 		}
		// 		sprintf(m_LogString,"[yjj]GetDataFromFile设备状态1[%d]",VecMonDev[ii].stat);
		// 		MessShow(m_LogString,LTERROR);
		// 		else
		// 		{
		// 			sprintf(m_LogString,"%s[%d]数据[%d]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].dev.dev_id,(int)VecMonDev[ii].v2d_data.size());
		// 			MessShow(m_LogString,LTDEBUG);
		// 			for (jj =0; jj<(int)VecMonDev[ii].v2d_data.size(); jj++)
		// 			{
		// 				sprintf(m_LogString,"%d,%f",jj+1,VecMonDev[ii].v2d_data[jj]);
		// 				MessShow(m_LogString,LTDEBUG);
		// 			}
		// 		}

		// 		sprintf(m_LogString,"设备[%s]数据[%d][%d]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].v2d_data.size(),VecMonDev[ii].ana_data.size());
		// 		MessShow(m_LogString,LTERROR);
	}

	if (nFlag >= (int)VecMonDev.size())
	{
		sprintf(m_LogString, "所有设备数据错误[%d]", nFlag);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("GetDataFromFile-end");
	return 0;
}

int CLfoAnaBase::GetDataFromDB(LFO_SYS_INFO& MonSys, vector<LFO_DEV_INFO>& VecMonDev)
{
	// PWD_TableGet
	// 填充vector<LFO_DEV_INFO> m_VecMonDev
	// 从数据库获取PMU数据时，采用临时变量vector<WT_VEC_DATA>，包含数据和质量位。判断数据状态后，将状态为正常的设备数据保存到v2d_data中。
	int ii;
	int ordernum, anaordernum, orderfac;
	ordernum = 2;//有功
	anaordernum = 1;//功角/角度
	orderfac = 0;//电压
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].stat == MENU_LFO_DEV_STS_OK)
		{
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
			{//厂站先获取电压再获取角度
				GetDevDataFromDB(MonSys.start_time, MonSys.end_time, orderfac, VecMonDev[ii]);
				GetDevDataFromDB(MonSys.start_time, MonSys.end_time, anaordernum, VecMonDev[ii]);
				continue;
			}
			GetDevDataFromDB(MonSys.start_time, MonSys.end_time, ordernum, VecMonDev[ii]);
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{//机组再次读取功角数据
				GetDevDataFromDB(MonSys.start_time, MonSys.end_time, anaordernum, VecMonDev[ii]);
			}
		}
		else
		{
			sprintf(m_LogString, "设备[%s]数据错误状态异常[%d]", VecMonDev[ii].dev_info.dev_name, VecMonDev[ii].stat);
			MessShow(m_LogString, LTERROR);
		}
	}

	return 0;
}

//************************************
// Method:    JudgeDataStat
// FullName:  CLfoAnaBase::JudgeDataStat
// Access:    public 
// Returns:   int
// Qualifier:
// 根据PMU数据判断设备数据状态，去掉未监视的设备
// 输入：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)
// 输出：填充设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)中PMU数据状态标志
// 量测异常：数据不在最大最小值间。
// 量测突变：相邻两个数据点间数值差大于门槛值。
// 若数据正常，将安全状态初始化为(#define	MENU_LFO_DEV_STS_SCT	10	/* 安全 */)
//************************************
int CLfoAnaBase::JudgeDataStat(vector<LFO_DEV_INFO> VecMonDev)
{
	// 	int ii;
	// 	PWD_DATA_DESC tmp_data_desc;
	//     for ( ii =0; ii<(int)VecMonDev.size(); ii++ )
	//     {
	// 		// 去掉未监视的设备
	// 		if (VecMonDev[ii].stat == MENU_LFO_DEV_STS_NO)
	// 		{
	// 			continue;
	// 		}
	// 		
	// 		memset((void *)&tmp_data_desc,0,sizeof(PWD_DATA_DESC));
	// 		PWD_PmuDataCheck(VecMonDev[ii].v2d_data.at(0),tmp_data_desc);
	// 		if (tmp_data_desc.nPmuOff > 0 )
	// 		{
	// 			// #define	MENU_LFO_DEV_STS_OFF	3	/* 通道退出 */
	// 			VecMonDev[ii].data_stat = MENU_LFO_DEV_STS_OFF;
	// 		}
	// 		else if (tmp_data_desc.nNotChange > 0)
	// 		{
	// 			// #define	MENU_LFO_DEV_STS_OLD	4	/* 数据不刷新 */
	// 			VecMonDev[ii].data_stat = MENU_LFO_DEV_STS_OLD;
	// 		}
	// 		else if (tmp_data_desc.nBadNum > 3)
	// 		{
	// 			// #define	MENU_LFO_DEV_STS_BAD	5	/* 量测不可用 */
	// 			VecMonDev[ii].data_stat = MENU_LFO_DEV_STS_BAD;
	// 		}
	// 		else if ()
	// 		{
	// 			// #define	MENU_LFO_DEV_STS_VAL	8	/* 量测异常 */
	// 			VecMonDev[ii].data_stat = MENU_LFO_DEV_STS_VAL;
	// 		}
	// 		else if ()
	// 		{
	// 			// #define	MENU_LFO_DEV_STS_BRK	9	/* 量测突变 *
	// 			VecMonDev[ii].data_stat = MENU_LFO_DEV_STS_BRK;
	// 		}
	// 		else
	// 		{
	// 			// #define	MENU_LFO_DEV_STS_SCT	10	/* 安全 */
	// 			VecMonDev[ii].data_stat = MENU_LFO_DEV_STS_SCT;
	// 		}
	//     }
	return 0;
}

//************************************
// Method:    PMUDataHandle
// FullName:  CLfoAnaBase::PMUDataHandle
// Access:    public 
// Returns:   int
// Qualifier:
// 计算参数，对状态为安全的设备，根据控制参数确定是否根据三相电压电流计算有功
// 输入：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)
// 输出：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)
//************************************
int CLfoAnaBase::PMUDataHandle(vector<LFO_DEV_INFO> VecMonDev)
{
	//	int ii;
	// 	for ( ii = 0; ii<(int)VecMonDev.size(); ii++ )
	// 	{
	// 		if (VecMonDev[ii].stat == MENU_LFO_DEV_STS_SCT)
	// 		{
	// 
	// 		}
	// 	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    DataInfoStatistic
// FullName:  CLfoAnaBase::DataInfoStatistic
// Access:    public 
// Returns:   int
// Qualifier:
// 根据PMU数据估计波形信息。
// 输入：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev) PMU数据(vector<float>)
// 输出：填充设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)中波形统计(LFO_CURVE_INFO)
//************************************
int CLfoAnaBase::DataInfoStatistic(vector<LFO_DEV_INFO>& VecMonDev, const int mode)
{
	int ii, ret;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (mode == 1)
		{
			if (VecMonDev[ii].stat == MENU_LFO_DEV_STS_OK)
			{
				GetCurveInfo(VecMonDev[ii].v2d_data, VecMonDev[ii].curve);
				VecMonDev[ii].back_data.assign(VecMonDev[ii].v2d_data.begin(), VecMonDev[ii].v2d_data.end());
			}
		}
		else if (mode == 2)
		{
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN || VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
			{
				// 				UnitAng(VecMonDev[ii].ana_data);
				ret = GetCurveInfo(VecMonDev[ii].ana_data, VecMonDev[ii].curve_ana);
				// 				VecMonDev[ii].back_ana.assign(VecMonDev[ii].ana_data.begin(), VecMonDev[ii].ana_data.end()); 
				// 				if (ret == -1)
				// 				{
				// 					sprintf(m_LogString,"[%s]数据为空,GetCurveInfo错误",VecMonDev[ii].dev.dev_name);
				// 					MessShow(m_LogString,LTINFO);
				// 				}
				// 				if (ret == 0)
				// 				{
				// 					sprintf(m_LogString,"[%s]最大摆动[%f]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].curve_ana.max_ampl);
				// 					MessShow(m_LogString,LTINFO);
				// 				}
			}
		}
	}
	return 0;
}

int CLfoAnaBase::GetCurveInfo(vector<float>& VecData, LFO_CURVE_INFO& CurveInfo)
{
	memset((void*)&CurveInfo, 0, sizeof(LFO_CURVE_INFO));
	if (VecData.empty())
	{
		return -1;
	}

	int ii, max_idx = 0, min_idx = 0;
	CurveInfo.max_val = (float)VecData.front();
	CurveInfo.min_val = (float)VecData.front();
	for (ii = 0; ii < (int)VecData.size(); ii++)
	{
		CurveInfo.aver_val += (float)VecData[ii];
		if (VecData[ii] > CurveInfo.max_val)
		{
			CurveInfo.max_val = (float)VecData[ii];
			max_idx = ii;
		}

		if (VecData[ii] < CurveInfo.min_val)
		{
			CurveInfo.min_val = (float)VecData[ii];
			min_idx = ii;
		}
	}

	CurveInfo.aver_val = CurveInfo.aver_val / (int)VecData.size();
	CurveInfo.max_ampl = (float)fabs(CurveInfo.max_val - CurveInfo.min_val);
	if (CurveInfo.aver_val != 0)
	{
		CurveInfo.max_ampl_rat = (float)(CurveInfo.max_ampl / fabs(CurveInfo.aver_val));
	}

	// 根据索引查找最大值时间
	float timeinter = 1.0;
	if (m_MonSys.sample_org > 0.0)
	{
		timeinter = (float)(1.0 / m_MonSys.sample_org);
	}

	CurveInfo.max_val_t = m_MonSys.start_time + (int)(max_idx * timeinter);
	CurveInfo.min_val_t = m_MonSys.start_time + (int)(min_idx * timeinter);

	return 0;
}

//************************************
// Method:    DataFilter
// FullName:  CLfoAnaBase::DataFilter
// Access:    public 
// Returns:   int
// Qualifier:
// 采用滤波算法去掉不平滑的数据。
// 输入：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev) PMU数据(vector<float>)
// 输出：滤波后PMU数据(vector<float>)
// TODO:滤波算法确定
//************************************
int CLfoAnaBase::DataFilter(vector<LFO_DEV_INFO>& VecMonDev, const int mode)
{
	double y_value[PRONY_DATA_MAX], t_value[PRONY_DATA_MAX];
	int ii, jj, kk, nDataNum, Filter_select;
	float tt;
	double* datayp = NULL;
	Filter_select = 1;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].stat != MENU_LFO_DEV_STS_OK)
		{
			continue;
		}

		memset(y_value, 0, PRONY_DATA_MAX * sizeof(double));
		memset(t_value, 0, PRONY_DATA_MAX * sizeof(double));
		if (mode == 1)
		{
			nDataNum = VecMonDev[ii].v2d_data.size();
		}
		else if (mode == 2)
		{
			nDataNum = VecMonDev[ii].ana_data.size();
		}

		datayp = (double*)calloc(nDataNum, sizeof(double));
		memset((void*)datayp, 0, nDataNum * sizeof(double));
		if (nDataNum > PRONY_DATA_MAX)
		{
			sprintf(m_LogString, "数据点数过多截取[%d]个,[%d]>[%d]", PRONY_DATA_MAX, nDataNum, PRONY_DATA_MAX);
			MessShow(m_LogString, LTINFO);
			nDataNum = PRONY_DATA_MAX;
		}
		if (nDataNum < 5)
		{
			sprintf(m_LogString, "数据错误,数据点数[%d]", nDataNum);
			MessShow(m_LogString, LTINFO);
			if (datayp != NULL) { free(datayp); datayp = NULL; }
			return -1;
		}
		for (jj = 0; jj < nDataNum; jj++)
		{
			t_value[jj] = jj / m_LfoPara.sample_freq;  // 根据采样周期计算时间间隔
			if (mode == 1)
			{
				y_value[jj] = VecMonDev[ii].v2d_data[jj];
			}
			else if (mode == 2)
			{
				y_value[jj] = VecMonDev[ii].ana_data[jj];
			}
			//			sprintf(m_LogString,"机组有功曲线：%.6f",y_value[jj]);
			//			MessShow(m_LogString);
		}
		if (Filter_select == 0)
		{
			tt = t_value[4] - t_value[3];
			if (int ret = Euler_ValFilter(nDataNum, y_value, t_value, datayp) > 0)
			{
				sprintf(m_LogString, "一阶惯性滤波处理异常!");
				MessShow(m_LogString, LTINFO);
				if (datayp != NULL) { free(datayp); datayp = NULL; }
				return -1;
			}
		}
		else
		{
			//			sprintf(m_LogString,"采用中值滤波方法!");
			//			MessShow(m_LogString,LTINFO);
			DP_MidAveFilter(y_value, datayp, nDataNum, 5);
		}
		for (kk = 0; kk < nDataNum; kk++)
		{
			if (mode == 1)
			{
				VecMonDev[ii].v2d_data[kk] = datayp[kk];
			}
			else if (mode == 2)
			{
				VecMonDev[ii].ana_data[kk] = datayp[kk];
			}
		}
		if (datayp != NULL) { free(datayp); datayp = NULL; }
	}
	if (datayp != NULL) { free(datayp); datayp = NULL; }
}
int CLfoAnaBase::Euler_ValFilter(int np, double xp[], double tp[], double yp[])
{
	double deltt, const_t, temp1, temp2;

	const_t = 0.03;
	deltt = tp[4] - tp[3];
	if (deltt < 0.0001)
	{
		return 1;
	}
	yp[0] = xp[0];
	if ((2 * const_t + deltt) < 0.0001)
	{
		return 1;
	}
	temp1 = (2 * const_t - deltt) / (2 * const_t + deltt);
	temp2 = deltt / (2 * const_t + deltt);
	for (int k = 1; k < np; k++)
	{
		yp[k] = temp1 * yp[k - 1] + temp2 * (xp[k - 1] + xp[k]);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 中值平均滤波器 ---- 防脉冲干扰平均滤波器
//  处理过程 : 连续采样N个数据, 去掉一个最大值和一个最小值, 计算N-2个数据的平均值, N取: 3~14
//  备    注 :
//  返 回 值 :
//  参数说明 : iSrcData                 原始数据
//             oFltData                 滤波数据[OUT]
//             nDataLen                 数据长度
//             nFiltLen                 滤波窗宽
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaBase::DP_MidAveFilter(const double* iSrcData, double* oFltData, int nDataLen, int nFiltLen)
{
	// 	memcpy(oFltData,iSrcData,nDataLen*sizeof(float));
	// 	return;
	int             nHalfLen = (nFiltLen - 1) / 2;        //  滤波窗半宽
	double* pfData = NULL;                        //  滤波窗数据
	double           fTmp;
	int             nValidLen;
	int             idx, ii, jj;

	nFiltLen = nHalfLen * 2 + 1;
	nValidLen = nFiltLen - 2;
	if (!iSrcData || !oFltData || nHalfLen < 1 || nDataLen < nFiltLen) return;

	pfData = (double*)calloc(nFiltLen, sizeof(double));
	if (!pfData) return;

	//  源信号前后两端半窗口宽度数据保持不变
	for (idx = 0; idx < nHalfLen; ++idx) oFltData[idx] = iSrcData[idx];
	for (idx = nDataLen - nHalfLen; idx < nDataLen; ++idx) oFltData[idx] = iSrcData[idx];

	//  以每一个点为中心
	for (idx = nDataLen - nHalfLen - 1; idx >= nHalfLen; --idx)
	{
		//  以idx为中心, 选取2*n+1个数据data[idx-n],...,data[idx],...,data[idx+n]
		for (ii = 0; ii < nFiltLen; ++ii) pfData[ii] = iSrcData[idx - nHalfLen + ii];

		//  直接插入排序
		for (ii = 1; ii < nFiltLen; ++ii)
		{
			//  大于等于有序区中所有值, 则保留原位, 否则进行排序
			if (pfData[ii] >= pfData[ii - 1]) continue;

			//  保存副本
			fTmp = pfData[ii];

			//  从右向左在有序区中查找插入位置, 将关键字大的记录后移一个位置
			for (jj = ii - 1; jj >= 0 && fTmp < pfData[jj]; --jj) pfData[jj + 1] = pfData[jj];

			//  插入到正确的位置上
			pfData[jj + 1] = fTmp;
		}

		oFltData[idx] = 0.0f;
		for (jj = nValidLen; jj > 0; --jj) oFltData[idx] += pfData[jj];
		oFltData[idx] /= nValidLen;
	}

	free(pfData);
}

//************************************
// Method:    DataSample
// FullName:  CLfoAnaBase::DataSample
// Access:    public 
// Returns:   int
// Qualifier:
// 根据采样参数对原始数据进行采样。
// 输入：采样参数，设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev) PMU数据(vector<float>)
// 输出：采样后PMU数据(vector<float>)
//************************************
int CLfoAnaBase::DataSample(vector<LFO_DEV_INFO>& VecMonDev, const int mode)
{
	int ii, jj;
	int nSample = 0, nNum = 0;
	// 采样间隔点数
	nSample = (int)(m_MonSys.sample_org / m_LfoPara.sample_freq + 0.5);

	if (nSample < 1)
	{
		nSample = 1;
		nNum = (int)(m_LfoPara.time_leng * m_MonSys.sample_org);
	}
	else
	{
		// 采样后数据个数
		nNum = (int)((m_LfoPara.time_leng * m_MonSys.sample_org - (nSample - 1)) / nSample + 1);
	}
	sprintf(m_LogString, "采样后点数[%d],nSample[%d],forg[%.2f],freq[%.2f],采样类型[%d]", nNum, nSample, m_MonSys.sample_org, m_LfoPara.sample_freq, mode);
	MessShow(m_LogString, LTINFO);

	vector<float> tmp_vec_data;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].stat != MENU_LFO_DEV_STS_OK && mode == 1)
		{
			// 			sprintf(m_LogString,"状态位不对,不采样");
			// 			MessShow(m_LogString,LTINFO);
			continue;
		}
		if (mode == 2)
		{//分析模式,机组和厂站采样
			if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN && VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_FAC)
			{
				// 				sprintf(m_LogString,"设备类型错误,不采样");
				// 				MessShow(m_LogString,LTINFO);
				continue;
			}
		}

		if (nSample == 1)
		{
			// 不采样
			continue;
		}

		if (VecMonDev[ii].v2d_data.empty() && mode == 1)
		{
			// 数据为空
// 			sprintf(m_LogString,"v2d_data为空,不采样");
// 			MessShow(m_LogString,LTINFO);
			continue;
		}
		if (VecMonDev[ii].ana_data.empty() && mode == 2)
		{
			// 数据为空
// 			sprintf(m_LogString,"ana_data为空,不采样");
// 			MessShow(m_LogString,LTINFO);
			continue;
		}
		if (mode == 1)
		{
			if (VecMonDev[ii].v2d_data.size() <= nNum)
			{
				// 				sprintf(m_LogString,"监视数据个数[%d]<=采样后个数[%d],不采样",VecMonDev[ii].v2d_data.size(),nNum);
				// 				MessShow(m_LogString,LTINFO);
				continue;
			}
		}
		if (mode == 2)
		{
			if (VecMonDev[ii].ana_data.size() <= nNum)
			{
				// 				sprintf(m_LogString,"分析数据个数[%d]<=采样后个数[%d],不采样",VecMonDev[ii].ana_data.size(),nNum);
				// 				MessShow(m_LogString,LTINFO);
				continue;
			}
		}
		// 采样
		if (!tmp_vec_data.empty())
		{
			vector<float>().swap(tmp_vec_data);
			tmp_vec_data.clear();
			vector<float>().swap(tmp_vec_data);
		}
		// 		sprintf(m_LogString,"监视数据个数[%d],分析数据个数[%d]",VecMonDev[ii].v2d_data.size(),VecMonDev[ii].ana_data.size());
		// 		MessShow(m_LogString,LTINFO);

		for (jj = 0; jj < nNum; jj++)
		{
			// 			sprintf(m_LogString,"%f",VecMonDev[ii].v2d_data[jj*nSample]);
			// 			MessShow(m_LogString,LTDEBUG);
			if (mode == 1)
			{
				tmp_vec_data.push_back(VecMonDev[ii].v2d_data[jj * nSample]);
			}
			else if (mode == 2)
			{
				tmp_vec_data.push_back(VecMonDev[ii].ana_data[jj * nSample]);
			}
		}

		if (mode == 1)
		{
			vector<float>().swap(VecMonDev[ii].v2d_data);
			VecMonDev[ii].v2d_data.clear();
			VecMonDev[ii].v2d_data.assign(tmp_vec_data.begin(), tmp_vec_data.end());
		}
		else if (mode == 2)
		{
			vector<float>().swap(VecMonDev[ii].ana_data);
			VecMonDev[ii].ana_data.clear();
			VecMonDev[ii].ana_data.assign(tmp_vec_data.begin(), tmp_vec_data.end());
			// 			sprintf(m_LogString,"[%s]采样后分析数据点数[%d]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].ana_data.size());
			// 			MessShow(m_LogString,LTINFO);
			// 			for (jj = 0;jj < VecMonDev[ii].ana_data.size();jj++)
			// 			{
			// 				if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			// 				{
			// 					sprintf(m_LogString,"[%s]采样后分析数据[%f]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].ana_data[jj]);
			// 					MessShow(m_LogString,LTINFO);
			// 				}
			// 			}
		}

	}
	sprintf(m_LogString, "数据采样结束");
	MessShow(m_LogString, LTINFO);

	return 0;
}

//************************************
// Method:    LfoPreEstimate
// FullName:  CLfoAnaBase::LfoPreEstimate
// Access:    public 
// Returns:   int
// Qualifier:
// 根据波形振幅预判，将振荡幅值大与门槛值的设备进行Prony分析，振荡小的设备不予分析。
// 输入：计算参数中幅值预判参数，设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev) 波形统计(LFO_CURVE_INFO)
// 输出：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)置是否计算标志。
// TODO:开始振荡后降低分析门槛值
//************************************
int CLfoAnaBase::LfoPreEstimate(vector<LFO_DEV_INFO>& VecMonDev)
{
	int ii;
	float tmpAmplPre = 0.0f;
	//	MessShow("预判:",LTDEBUG);
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].stat != MENU_LFO_DEV_STS_OK)
		{
			memset(&VecMonDev[ii].osciFlag, 0, sizeof(OSCILLAT_FLAG));
			memset(&VecMonDev[ii].curve, 0, sizeof(LFO_CURVE_INFO));
			continue;
		}
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
		{
			sprintf(m_LogString, "[%s]最大摆动[%f]", VecMonDev[ii].dev.dev_name, VecMonDev[ii].curve.max_ampl);
			MessShow(m_LogString);
			continue;
		}
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
		{
			continue;
		}

		// 		tmpAmplPre = 0.0f;
		// 		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
		// 		{
		// 			tmpAmplPre = m_LfoPara.fjud_wnap_per * m_LfoPara.gn_wn_ampl / 100.0f;
		// 		}
		// 		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
		// 		{
		// 			tmpAmplPre = m_LfoPara.fjud_wnap_per * m_LfoPara.ln_wn_ampl / 100.0f;
		// 		}
		// 		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
		// 		{
		// 			tmpAmplPre = m_LfoPara.fjud_wnap_per * m_LfoPara.tr_wn_ampl / 100.0f;
		// 		}
		// 		else
		// 		{
		// 			sprintf(m_LogString,"设备类型错误");
		// 			MessShow(m_LogString,LTERROR);
		// 		}
				// 预判幅值为平均值,与波形比较时需乘2
		tmpAmplPre = m_LfoPara.fjud_wnap_per * VecMonDev[ii].dev.ampl_limit_s / 100.0f;
		tmpAmplPre = 2.0f * tmpAmplPre;
		for (int jj = 0; jj < m_VecIntline.size(); jj++)
		{
			if (m_VecIntline[jj].line_no == VecMonDev[ii].dev.dev_id)
			{
				m_VecIntline[jj].dev_info = VecMonDev[ii];
				tmpAmplPre = tmpAmplPre * 0.8;
				m_VecIntline[jj].ampl_limit_s = VecMonDev[ii].dev.ampl_limit_s;
				m_VecIntline[jj].ampl_limit_e = VecMonDev[ii].dev.ampl_limit_e;
			}
		}
		if (VecMonDev[ii].curve.max_ampl > tmpAmplPre)
		{
			// 需要进行Prony分析
			VecMonDev[ii].stat = MENU_LFO_DEV_STS_OK;
			VecMonDev[ii].stat_prony = MENU_LFO_DEV_STS_OK;
			// 			sprintf(m_LogString,"[%s]["INT_REC_FMT"],max_ampl[%.3f],pre[%.3f],stat[%d],stat_old[%d]",
			// 				VecMonDev[ii].dev.dev_name,VecMonDev[ii].dev.dev_id,VecMonDev[ii].curve.max_ampl,tmpAmplPre,VecMonDev[ii].stat,VecMonDev[ii].stat_old);
			// 			MessShow(m_LogString,LTDEBUG);
		}
		else
		{
			// 状态直接置为安全
			VecMonDev[ii].stat = MENU_LFO_DEV_STS_SCT;
			VecMonDev[ii].stat_prony = MENU_LFO_DEV_STS_OLD;
			// 清空设备时间
			VecMonDev[ii].start_time = 0;
			VecMonDev[ii].end_time = 0;
			VecMonDev[ii].hold_time = 0;
			memset(&VecMonDev[ii].osciFlag, 0, sizeof(OSCILLAT_FLAG));
		}
	}
	return 0;
}

//************************************

//************************************
int CLfoAnaBase::IsDataValid(vector<LFO_DEV_INFO>& VecMonDev)
{
	MessShow("数据检查");
	int   ii, kk, num;
	float pwr = 0.0f, max_pwr = 5000.0, min_pwr = -5000.0, tem_pwr = 0.0f, ChangeVal = -1.0f, ChgMinVal = 0.0;
	float pmax = 0;

	int   ChgTimCoe = (int)(m_MonSys.sample_org / m_LfoPara.sample_freq + 0.5);
	float  MaxChangeLimVal = L_PWR_MAX_CHG * ChgTimCoe;

	if (ChgTimCoe < 1) ChgTimCoe = 1;

	//	MessShow("数据有效性检查:",LTDEBUG);
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].stat != MENU_LFO_DEV_STS_OK)
		{
			continue;
		}
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
		{
			tem_pwr = VecMonDev[ii].dev_info.pmax;
			if (tem_pwr > 1.0)
			{
				max_pwr = 1.5 * tem_pwr;
				min_pwr = -1.5 * tem_pwr;
			}
			ChgMinVal = m_LfoPara.gn_wn_ampl;
#ifdef _LFO_HIS_LIB_
			pmax = VecMonDev[ii].dev_info.maxvbase;//离线文件中,电压等级存放了机组容量
#else
			pmax = VecMonDev[ii].dev_info.pmax;
#endif
#ifdef _GUANGDONG
			if (VecMonDev[ii].dev_info.maxvbase < m_LfoPara.volt_thre1)
			{
				ChgMinVal = m_LfoPara.gn_wn_ampl;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.gn_wn_ampl;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.gn_wn_ampl1;
			}
			else if (VecMonDev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre1 && VecMonDev[ii].dev_info.maxvbase < m_LfoPara.volt_thre2)
			{
				ChgMinVal = m_LfoPara.gn_swn_ampl2;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.gn_swn_ampl2;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.gn_ewn_ampl2;
			}
			else if (VecMonDev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre2)
			{
				ChgMinVal = m_LfoPara.gn_swn_ampl3;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.gn_swn_ampl3;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.gn_ewn_ampl3;
			}
			sprintf(m_LogString, "机组[%s]电压等级[%f],振幅限值[%f]", VecMonDev[ii].dev.dev_name, VecMonDev[ii].dev_info.maxvbase, VecMonDev[ii].dev.ampl_limit_s);
			MessShow(m_LogString);
#else
			if (pmax < m_LfoPara.gn_capacity1)
			{
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.gn_wn_ampl;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.gn_wn_ampl1;
			}
			else if (pmax >= m_LfoPara.gn_capacity1 && pmax < m_LfoPara.gn_capacity2)
			{
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.gn_swn_ampl2;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.gn_ewn_ampl2;
			}
			else if (pmax >= m_LfoPara.gn_capacity2)
			{
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.gn_swn_ampl3;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.gn_ewn_ampl3;
			}
#endif
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
		{
			tem_pwr = (VecMonDev[ii].dev_info.imax1 * VecMonDev[ii].dev_info.maxvbase) / 1000.0;
			if (tem_pwr > 1.0)
			{
				max_pwr = 2.0 * tem_pwr;
				min_pwr = -2.0 * tem_pwr;
			}
			ChgMinVal = m_LfoPara.ln_wn_ampl;
			if (VecMonDev[ii].dev_info.maxvbase < m_LfoPara.volt_thre1)
			{
				ChgMinVal = m_LfoPara.ln_swn_ampl1;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.ln_swn_ampl1;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.ln_ewn_ampl1;
			}
			else if (VecMonDev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre1 && VecMonDev[ii].dev_info.maxvbase < m_LfoPara.volt_thre2)
			{
				ChgMinVal = m_LfoPara.ln_swn_ampl2;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.ln_swn_ampl2;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.ln_ewn_ampl2;
			}
			else if (VecMonDev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre2)
			{
				ChgMinVal = m_LfoPara.ln_swn_ampl3;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.ln_swn_ampl3;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.ln_ewn_ampl3;
			}
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
		{
			tem_pwr = VecMonDev[ii].dev_info.max_pwr;
			if (tem_pwr > 1.0)
			{
				max_pwr = 2.0 * tem_pwr;
				min_pwr = -2.0 * tem_pwr;
			}
			if (VecMonDev[ii].dev_info.maxvbase < m_LfoPara.volt_thre1)
			{
				ChgMinVal = m_LfoPara.tr_swn_ampl1;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.tr_swn_ampl1;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.tr_swn_ampl1;
			}
			else if (VecMonDev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre1 && VecMonDev[ii].dev_info.maxvbase < m_LfoPara.volt_thre2)
			{
				ChgMinVal = m_LfoPara.tr_swn_ampl2;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.tr_swn_ampl2;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.tr_swn_ampl2;
			}
			else if (VecMonDev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre2)
			{
				ChgMinVal = m_LfoPara.tr_swn_ampl3;
				VecMonDev[ii].dev.ampl_limit_s = m_LfoPara.tr_swn_ampl3;
				VecMonDev[ii].dev.ampl_limit_e = m_LfoPara.tr_swn_ampl3;
			}

		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
		{
			continue;
		}
		else
		{
			continue;
			sprintf(m_LogString, "设备类型错误");
			MessShow(m_LogString, LTERROR);
		}
#ifdef _CHG_AMPL

		if (VecMonDev[ii].dev.is_chg_ampl == MENU_YES)
		{
			VecMonDev[ii].dev.ampl_limit_s = DP_MAX(DP_ABS(VecMonDev[ii].curve.max_val), DP_ABS(VecMonDev[ii].curve.min_val)) * VecMonDev[ii].dev.chg_ampl_per / 100.0f;
			VecMonDev[ii].dev.ampl_limit_e = VecMonDev[ii].dev.ampl_limit_s;
			sprintf(m_LogString, "设备[%s]采用动态振幅限值,最大值[%f],百分比[%f],动态振幅限值[%f]", VecMonDev[ii].dev.dev_name, DP_MAX(DP_ABS(VecMonDev[ii].curve.max_val), DP_ABS(VecMonDev[ii].curve.min_val)), VecMonDev[ii].dev.chg_ampl_per, VecMonDev[ii].dev.ampl_limit_s);
			MessShow(m_LogString);
		}
#endif
		// 		sprintf(m_LogString,"设备[%s]["INT_REC_FMT"]振幅限值[%f][%f]",VecMonDev[ii].dev.dev_name,INT_REC_VAL(VecMonDev[ii].dev.dev_id),VecMonDev[ii].dev.ampl_limit_s,VecMonDev[ii].dev.ampl_limit_e);
		// 		MessShow(m_LogString);
		num = (int)VecMonDev[ii].v2d_data.size();
		for (int kk = 0; kk < num; kk++)
		{
			pwr = VecMonDev[ii].v2d_data[kk];
			if (pwr<min_pwr || pwr>max_pwr)
			{
#ifdef _GUANGDONG
				sprintf(m_LogString, "IsDataValid(): Dev[%s%s]有功功率异常:%5.2f [%5.2f , %5.2f]", VecMonDev[ii].dev_info.fac_name, VecMonDev[ii].dev_info.dev_name, pwr, min_pwr, max_pwr);
#else
				sprintf(m_LogString, "IsDataValid(): Dev[%s]有功功率异常:%5.2f [%5.2f , %5.2f]", VecMonDev[ii].dev_info.dev_name, pwr, min_pwr, max_pwr);
#endif
				MessShow(m_LogString);
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_VAL;
				break;
			}
		}

		ChangeVal = JudgeTwoValDelta(VecMonDev[ii].v2d_data, num, MaxChangeLimVal, ChgMinVal, 1);
		if (ChangeVal > 0.0001)
		{
			// 			sprintf(m_LogString,"IsDataValid()返回值[%f]: Dev[%s]存在突变，突变量：[%5.2f]>[%5.2f]", ChangeVal,VecMonDev[ii].dev_info.dev_name, ChangeVal,MaxChangeLimVal);
			// 			MessShow(m_LogString);
			VecMonDev[ii].stat = MENU_LFO_DEV_STS_BRK;
			//			return MENU_LFO_DEV_STS_BRK;
		}
	}

	return 0;
}

// 判断相邻数据点是否有突变(突变返回 fabs(data[i+1] - data[i])，修复；如果连续两点不突变返回-1 )
// 判断方法：1)相邻两点绝对差 > (前十点绝对差之和)； 2) 根据给定的允许绝对差
float CLfoAnaBase::JudgeTwoValDelta(vector<float>& VecData, int num, float max_change_lim, float delta_lim_val, int algorithm)
{
	int   i, j;
	int   count = 0;
	float delta_lim_calc = 0;
	float delta_lim = 0;

	if (VecData.empty())
	{
		return 0;
	}
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
		{//统计前十个点的误差和 赋值给delta_lim_calc
			if (fabs(VecData[j + 1] - VecData[j]) > max_change_lim)
			{
				sprintf(m_LogString, "111VecData[%d]=%f,VecData[%d]=%f,差值大于max_change_lim[%f]", j + 1, VecData[j + 1], j, VecData[j], max_change_lim);
				MessShow(m_LogString);
				return fabs(VecData[j + 1] - VecData[j]);
			}
			else
			{
				delta_lim_calc += fabs(VecData[j + 1] - VecData[j]);
			}
		}

		for (i = 0; i < (num - 2); i++)
		{
			if (i == 0)
			{//第1个点,统计第2个点至第11个点误差和（前10个点的误差和 - 前两个点误差 + 第11个点和第10个点误差）
				delta_lim_calc = delta_lim_calc - (fabs(VecData[i + 1] - VecData[i])) + (fabs(VecData[i + count + 1] - VecData[i + count]));
			}
			else if (i > 0 && i < count)
			{//第2点到第10个点,统计第1个点至第11点的误差和-第i+1个点与第i个点的误差
				delta_lim_calc = delta_lim_calc - (fabs(VecData[i + 1] - VecData[i])) + (fabs(VecData[i] - VecData[i - 1]));
			}
			else if (i >= count)
			{//从第11个点开始,统计前10点的误差和
				delta_lim_calc = delta_lim_calc - (fabs(VecData[i - count + 1] - VecData[i - count])) + (fabs(VecData[i] - VecData[i - 1]));
			}

			// 防止偶尔的较小毛刺，被判为突变
			if (delta_lim_calc < (delta_lim_val))
			{
				delta_lim = (delta_lim_val);
			}
			else
			{
				delta_lim = delta_lim_calc;
			}

			if (fabs(VecData[i + 1] - VecData[i]) > max_change_lim)
			{
				sprintf(m_LogString, "222VecData[%d]=%f,VecData[%d]=%f,大于max_change_lim[%f]", i + 1, VecData[i + 1], i, VecData[i], max_change_lim);
				MessShow(m_LogString);
				return fabs(VecData[i + 1] - VecData[i]);
			}
			else
			{
				if (fabs(VecData[i + 1] - VecData[i]) > delta_lim)
				{
					if (fabs(VecData[i + 2] - VecData[i]) < delta_lim)
					{
						VecData[i + 1] = (float)((VecData[i + 2] + VecData[i]) / 2.0);
					}
					else
					{
						sprintf(m_LogString, "333VecData[%d]=%f,VecData[%d]=%f,大于delta_lim[%f]", i + 1, VecData[i + 1], i, VecData[i], delta_lim);
						MessShow(m_LogString);
						return fabs(VecData[i + 1] - VecData[i]);
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
			if (fabs(VecData[i + 1] - VecData[i]) > delta_lim_val)
			{
				sprintf(m_LogString, "444VecData[%d]=%f,VecData[%d]=%f,大于delta_lim_val[%f]", i + 1, VecData[i + 1], i, VecData[i], delta_lim_val);
				MessShow(m_LogString);
				return fabs(VecData[i + 1] - VecData[i]);
			}
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    IntfaceAmplLimit
// FullName:  CLfoAnaBase::IntfaceAmplLimit
// Access:    public 
// Returns:   int
// Qualifier:
// 计算输电断面振幅限值和断面组成设备描述
// 输入：支路振幅限值
// 输出：输电断面振幅限值和断面组成设备描述
// 
//************************************
int CLfoAnaBase::IntfaceAmplLimit(vector<LFO_DEV_INFO>& VecMonDev)
{
	// 	sprintf(m_LogString,"计算断面振幅");
	// 	MessShow(m_LogString);
	string intline_des;
	for (int ii = 0; ii < VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_ITFC)
		{
			continue;
		}
		VecMonDev[ii].dev.ampl_limit_s = 0;
		VecMonDev[ii].dev.ampl_limit_e = 0;
		memset((void*)VecMonDev[ii].intline_des, 0, sizeof(VecMonDev[ii].intline_des));
		intline_des.clear();
		for (int jj = 0; jj < m_VecIntline.size(); jj++)
		{
			if (VecMonDev[ii].dev.dev_id == m_VecIntline[jj].inf_no)
			{
				if (fabs(m_VecIntline[jj].ampl_limit_s) < 0.001)
				{
					continue;
				}
				VecMonDev[ii].dev.ampl_limit_s += m_VecIntline[jj].ampl_limit_s;
				VecMonDev[ii].dev.ampl_limit_e += m_VecIntline[jj].ampl_limit_e;
				intline_des += m_VecIntline[jj].line_name;
				intline_des += ";";
			}
		}
		VecMonDev[ii].dev.ampl_limit_s = VecMonDev[ii].dev.ampl_limit_s * m_LfoPara.inf_ampl;
		VecMonDev[ii].dev.ampl_limit_e = VecMonDev[ii].dev.ampl_limit_e * m_LfoPara.inf_ampl;
		sprintf(VecMonDev[ii].intline_des, "%s", intline_des.c_str());
		sprintf(m_LogString, "断面[%s]组成设备[%s]开始振荡振幅[%f],结束振荡振幅[%f]", VecMonDev[ii].dev.dev_name, VecMonDev[ii].intline_des, VecMonDev[ii].dev.ampl_limit_s, VecMonDev[ii].dev.ampl_limit_e);
		MessShow(m_LogString);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    FormPronyData
// FullName:  CLfoAnaBase::FormPronyData
// Access:    public 
// Returns:   int
// Qualifier:
// 将满足预判条件的设备数据幅值给Prony分析的数据结构。
// 输入：是否计算标志，设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev) PMU数据(vector<float>)
// 输出：Prony分析数据结构(vector<LFO_PRONY_INFO>)，填充PMU数据。
// 清空设备信息PMU数据结构
//************************************
int CLfoAnaBase::FormPronyData(vector<LFO_DEV_INFO> VecMonDev, vector<LFO_PRONY_INFO>& VecCase, const int mode)
{
	int ii;
	ClearLfoPronyInfo(VecCase);
	LFO_PRONY_INFO tmpCase;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].stat == MENU_LFO_DEV_STS_OK && mode == 1)
		{
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
			{
				continue;
			}
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
			{
				continue;
			}
			ClearLfoPronyInfo(tmpCase);
			// 			tmpCase.dev = VecMonDev[ii].dev;
			CopyLfoDevBase(tmpCase.dev, VecMonDev[ii].dev);
			tmpCase.v2d_data.assign(VecMonDev[ii].v2d_data.begin(), VecMonDev[ii].v2d_data.end());
			vector<float>().swap(VecMonDev[ii].v2d_data);
			VecMonDev[ii].v2d_data.clear();
			vector<float>().swap(VecMonDev[ii].v2d_data);

			VecCase.push_back(tmpCase);
		}
		else if (mode == 2)
		{
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				if (VecMonDev[ii].stat_prony != MENU_LFO_DEV_STS_OK)
				{
					continue;
				}
			}
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				continue;
			}
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				continue;
			}
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
			{
				continue;
			}

			m_pCases[m_nCases].m_nID = VecMonDev[ii].dev.dev_id;
			m_pCases[m_nCases].m_type = VecMonDev[ii].dev.dev_type;
			sprintf(m_pCases[m_nCases].m_name, "%s", VecMonDev[ii].dev.dev_name);
			ClearLfoPronyInfo(tmpCase);
			// 			tmpCase.dev = VecMonDev[ii].dev;
			CopyLfoDevBase(tmpCase.dev, VecMonDev[ii].dev);
			tmpCase.v2d_data.assign(VecMonDev[ii].ana_data.begin(), VecMonDev[ii].ana_data.end());
			vector<float>().swap(VecMonDev[ii].ana_data);
			VecMonDev[ii].ana_data.clear();
			vector<float>().swap(VecMonDev[ii].ana_data);
			VecCase.push_back(tmpCase);
			sprintf(m_LogString, "设备[%s]参与分析,类型[%d],数据个数[%d]", VecMonDev[ii].dev.dev_name, VecMonDev[ii].dev.dev_type, tmpCase.v2d_data.size());
			MessShow(m_LogString);
			m_nCases++;
			if (m_nCases == para_nCaseMaxMem)
			{
				sprintf(m_LogString, "m_pCase数量越界");
				MessShow(m_LogString);
				break;
			}
		}
	}
	return 0;
}

//************************************
// Method:    PronyProcess
// FullName:  CLfoAnaBase::PronyProcess
// Access:    public 
// Returns:   int
// Qualifier:
// 调用PronyAnalysis7()函数进行分析，保存分析结果。
// 输入：保存模式参数，Prony分析数据结构(vector<LFO_PRONY_INFO>) PMU数据(vector<float>)
// 输出：Prony分析数据结构(vector<LFO_PRONY_INFO>)实时模式(包含多个模式并排序)(vector<LFO_MODE_INFO>)
//************************************
int CLfoAnaBase::PronyProcess(vector<LFO_PRONY_INFO>& VecCase)
{
	struct  timeb   tStartTime, tEndTime, tPronyStart, tPronyEnd;
	int tTotalTime = 0;
	int nDataNum, mode_num, rank;
	double snr = 0.0;
	double mode[PRONY_MAX_MODE][7];
	double y_value[PRONY_DATA_MAX], t_value[PRONY_DATA_MAX];
	tStartTime = PT_GetNowMilliSeconds();
	int ii, jj;
	LFO_MODE_INFO tmpMode;
	// 	int tempint6 = (int)VecCase.size();
	for (ii = 0; ii < (int)VecCase.size(); ii++)
	{
		tEndTime = PT_GetNowMilliSeconds();
		tTotalTime = PT_CalculateTimeInterval(tStartTime, tEndTime);
		//总计算时间超时判断
// 		if ( tTotalTime > PRONY_MAX_TIME )
// 		{
// 			sprintf(m_LogString,"计算超时:[%d]s",tTotalTime/1000);
// 			MessShow(m_LogString,LTERROR);
// 			//break;
// 		}

		memset(mode, 0, PRONY_MAX_MODE * 7 * sizeof(double));
		memset(y_value, 0, PRONY_DATA_MAX * sizeof(double));
		memset(t_value, 0, PRONY_DATA_MAX * sizeof(double));
		nDataNum = VecCase[ii].v2d_data.size();
		if (nDataNum > PRONY_DATA_MAX)
		{
			sprintf(m_LogString, "数据点数过多截取[%d]个,[%d]>[%d]", PRONY_DATA_MAX, nDataNum, PRONY_DATA_MAX);
			MessShow(m_LogString);
			nDataNum = PRONY_DATA_MAX;
		}

		for (jj = 0; jj < nDataNum; jj++)
		{
			t_value[jj] = jj / m_LfoPara.sample_freq;  // 根据采样周期计算时间间隔
			y_value[jj] = VecCase[ii].v2d_data[jj];
			// 			if (VecCase[ii].dev.dev_id== 116530641247404034 || VecCase[ii].dev.dev_id== 116530641247404083)
			// 			{
			 				/*sprintf(m_LogString,"【yjj1111】曲线：%.6f ,时间[%f]",y_value[jj],t_value[jj] );
			 				MessShow(m_LogString);*/
			// 			}
		}
		// 	sprintf(m_LogString,"【yjj1111】开始进入PronyAnalysis7");
		//  	MessShow(m_LogString);		

		tPronyStart = PT_GetNowMilliSeconds();
		mode_num = 0;
		snr = 0.0;
		rank = 0;

		//  		sprintf(m_LogString,"[提示：]本轮PronyAnalysis分析频率范围[%f][%f]",m_LfoPara.prony_para.freq_min,m_LfoPara.prony_para.freq_max);
		// 	 	MessShow(m_LogString);

		// 		g_pAnaObj->m_mutex.Lock();
		PronyAnalysis7(m_LfoPara.prony_para, nDataNum, y_value, t_value, mode_num, mode, snr, rank);
		// 		g_pAnaObj->m_mutex.Unlock();

		// 	sprintf(m_LogString,"【yjj1111】退出PronyAnalysis7");
		// 	MessShow(m_LogString);			

		tPronyEnd = PT_GetNowMilliSeconds();
		tTotalTime = PT_CalculateTimeInterval(tPronyStart, tPronyEnd);

		if (mode_num == 0)
		{
			sprintf(m_LogString, "没有符合要求的振荡模式,case_id=["INT_REC_FMT"][%s],rank=[%d],snr=[%f],nDataNum=[%d]", INT_REC_VAL(VecCase[ii].dev.dev_id), VecCase[ii].dev.dev_name, rank, snr, nDataNum);
			MessShow(m_LogString, LTINFO);
		}
		else if (mode_num < 0)
		{
			sprintf(m_LogString, "无可行解,case_id=["INT_REC_FMT"]", INT_REC_VAL(VecCase[ii].dev.dev_id));
			MessShow(m_LogString, LTINFO);
		}
		else
		{
			ClearLfoModeInfo(VecCase[ii].vec_mode);

			sprintf(m_LogString, "[%s]["INT_REC_FMT"]模式[%d]:", VecCase[ii].dev.dev_name, INT_REC_VAL(VecCase[ii].dev.dev_id), mode_num);
			MessShow(m_LogString, LTINFO);
			for (jj = 0; jj < mode_num; jj++)
			{
				ClearLfoModeInfo(tmpMode);
				tmpMode.damp = (float)mode[jj][0] / 100;//百分比转为实际值
				tmpMode.freq = (float)mode[jj][1];
				tmpMode.ampl = (float)mode[jj][2];
				tmpMode.phas = (float)(mode[jj][3] / RADBASE);
				tmpMode.real = (float)mode[jj][4];
				tmpMode.imag = (float)mode[jj][5];
				tmpMode.engy = (float)mode[jj][6];
				tmpMode.start_time = m_MonSys.start_time;
				tmpMode.end_time = m_MonSys.end_time;
				tmpMode.engy_pre_time = (float)(tmpMode.engy / m_LfoPara.time_leng);

				if (tmpMode.damp < 0 && fabs(tmpMode.damp) < 0.0005)//阻尼比-0.000处理
				{
					tmpMode.damp = 0.0;
				}
				VecCase[ii].vec_mode.push_back(tmpMode);

				sprintf(m_LogString, "%d,fdaprie[%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f]",
					jj + 1, tmpMode.freq, tmpMode.damp, tmpMode.ampl, tmpMode.phas, tmpMode.real, tmpMode.imag, tmpMode.engy, tmpMode.engy_pre_time);
				MessShow(m_LogString, LTINFO);
			}

			if (VecCase[ii].vec_mode.empty())
			{
				sprintf(m_LogString, "没有该频率段的模式["INT_REC_FMT"]", INT_REC_VAL(VecCase[ii].dev.dev_id));
				MessShow(m_LogString, LTERROR);
			}
			else
			{
				// 按能量排序
				sort(VecCase[ii].vec_mode.begin(), VecCase[ii].vec_mode.end(), g_SortModeByEny);
			}
		}
		usleep(5);
	}

	return 0;
}

//************************************
// Method:    GetDevMode
// FullName:  CLfoAnaBase::GetDevMode
// Access:    public 
// Returns:   int
// Qualifier:
// 将Prony分析数据结构中的模式信息赋值给对应设备的模式信息。此时清空全局变量Prony分析数据结构。
// 输入：Prony分析数据结构(vector<LFO_PRONY_INFO>)实时模式(包含多个模式并排序)(vector<LFO_MODE_INFO>)
// 输出：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev) 实时模式（包含多个模式并排序）(vector<LFO_MODE_INFO>)，主导模式（实时模式中的能量最大模式）
//************************************
int CLfoAnaBase::GetDevMode(vector<LFO_PRONY_INFO> VecCase, vector<LFO_DEV_INFO>& VecMonDev)
{
	int ii, jj, kk;
	int checkTimeNum = 0;
	int checkDataNum = 0;
	float tmpAmpl = 0.0f, tmpWarnAmpl = 0.0f;
	float dtime = 0.0;
	float          temp_ampl_max = 0;
	float          temp_ampl_min = 0;
	int            times = 0;
	float          ampl_aver_new = 0;    // 2008-05-08 by zhw
	float          ampl_aver_old = 0;
	int            countNum = 0;

	int                  m_nSmpFre;                         //  采样频率
	int                  m_nSmpStp;                         //  采样间隔
	float                m_nOrigDlt;					    //  原始采样点间隔
	int                  m_nOrigSam;					    //  原始采样点个数
	float                m_Matchtime[PRONY_DATA_MAX];         //  匹配时刻数据
	int                  m_nSamples;					    //  计算用采样点个数
	float                m_ProMatchtime[PRONY_DATA_MAX];      //  计算用匹配时刻	
	LFO_MODE_INFO tmpModeInfo;
	memset((void*)m_Matchtime, 0, sizeof(float) * PRONY_DATA_MAX);
	memset((void*)m_ProMatchtime, 0, sizeof(float) * PRONY_DATA_MAX);
	m_nSmpFre = m_LfoPara.sample_freq;
	m_nOrigSam = m_LfoPara.time_leng * m_MonSys.sample_org;
	m_nSmpStp = (int)(m_MonSys.sample_org / m_nSmpFre + 0.5); //  采样间隔0.1

	if (m_nSmpStp < 1)
	{
		m_nSmpStp = 1;
		m_nSamples = m_nOrigSam;
	}
	else
	{
		// 采样后数据个数
		m_nSamples = (int)((m_nOrigSam - (m_nSmpStp - 1)) / m_nSmpStp + 1);
	}
	//匹配时间
	m_nOrigDlt = (float)(1.0 / MAX_FRA_PER_SEC);
	for (ii = 0; ii < m_nOrigSam; ii++)
	{
		m_Matchtime[ii] = m_nOrigDlt * ii;
	}

	for (ii = 0; ii < m_nSamples; ii++)
	{
		jj = m_nSmpStp * ii;
		m_ProMatchtime[ii] = m_Matchtime[jj];
	}

	dtime = m_ProMatchtime[2] - m_ProMatchtime[1];

	for (ii = 0; ii < (int)VecCase.size(); ii++)
	{
		for (jj = 0; jj < (int)VecMonDev.size(); jj++)
		{
			if (VecMonDev[jj].dev.dev_type == MENU_LFO_DEV_ITFC)
			{
				continue;
			}
			if (VecMonDev[jj].dev.dev_id == VecCase[ii].dev.dev_id)
			{
				if (!VecCase[ii].vec_mode.empty())
				{
					// 保存幅值大于幅值告警门槛值的模式
// 					if (VecMonDev[jj].dev.dev_type == MENU_LFO_DEV_GEN)
// 					{
// 						tmpWarnAmpl = m_LfoPara.gn_wn_ampl;
// 					}
// 					else if (VecMonDev[jj].dev.dev_type == MENU_LFO_DEV_LINE)
// 					{
// 						tmpWarnAmpl = m_LfoPara.ln_wn_ampl;
// 					}
// 					else if (VecMonDev[jj].dev.dev_type == MENU_LFO_DEV_TR)
// 					{
// 						tmpWarnAmpl = m_LfoPara.tr_wn_ampl;
// 					}
// 					else
// 					{
// 						// 设备类型未知,置较大值
// 						tmpWarnAmpl = 9999.0f;
// 					}
					tmpWarnAmpl = VecMonDev[jj].dev.ampl_limit_s * m_LfoPara.mode_save_amplper / 100.0f;

					// 筛选频率范围内且大于保存幅值门槛值的模式
					// 增加阻尼比判断条件
					tmpAmpl = VecCase[ii].vec_mode.front().ampl;
					tmpAmpl = VecCase[ii].vec_mode.front().ampl * m_LfoPara.mode_save_amplper / 100.0f;
					// 					sprintf(m_LogString,"%s["INT_REC_FMT"]tmpAmpl=%f,tmpWarnAmpl=%f,ampl_limit=%f,mode_save_amplper=%f",VecCase[ii].dev.dev_name,INT_REC_VAL(VecCase[ii].dev.dev_id),tmpAmpl,tmpWarnAmpl,VecMonDev[jj].dev.ampl_limit_s,m_LfoPara.mode_save_amplper);
					// 					MessShow(m_LogString,LTINFO);

					// 					int aaa = (int)VecCase[ii].vec_mode.size();
					// 					sprintf(m_LogString,"[yjj]GetDevMode:[%f],[%f],模式数[%d]",tmpWarnAmpl,tmpAmpl,aaa);
					// 					MessShow(m_LogString,LTINFO);
					for (kk = 0; kk < (int)VecCase[ii].vec_mode.size(); kk++)
					{
						//yjj 此处存在疑问，暂时去掉阻尼比条件
						// 						if ((VecCase[ii].vec_mode[kk].freq >= m_LfoPara.freq_min) && 
						// 							(VecCase[ii].vec_mode[kk].freq <= m_LfoPara.freq_max) &&
						// 							(VecCase[ii].vec_mode[kk].damp <= m_LfoPara.lack_damp)&&
						// 							(VecCase[ii].vec_mode[kk].damp >= m_LfoPara.damp_min) &&
						// 							(VecCase[ii].vec_mode[kk].ampl >= tmpAmpl))
						if ((VecCase[ii].vec_mode[kk].freq >= m_LfoPara.freq_min) &&
							(VecCase[ii].vec_mode[kk].freq <= m_LfoPara.freq_max) &&
							(VecCase[ii].vec_mode[kk].ampl >= tmpWarnAmpl) &&
							(VecCase[ii].vec_mode[kk].ampl >= tmpAmpl))
						{
							CopyLfoModeInfo(tmpModeInfo, VecCase[ii].vec_mode[kk]);
							VecMonDev[jj].vec_mode.push_back(tmpModeInfo);
						}
						// 						if ((VecCase[ii].vec_mode[kk].freq >= m_LfoPara.freq_min) && 
						// 							(VecCase[ii].vec_mode[kk].freq <= m_LfoPara.freq_max) &&
						// 							(VecCase[ii].vec_mode[kk].damp <= m_LfoPara.lack_damp)&&
						// 							(VecCase[ii].vec_mode[kk].damp >= m_LfoPara.damp_min) &&
						// 							(VecCase[ii].vec_mode[kk].ampl >= tmpWarnAmpl))
						if ((VecCase[ii].vec_mode[kk].freq >= m_LfoPara.freq_min) &&
							(VecCase[ii].vec_mode[kk].freq <= m_LfoPara.freq_max) &&
							(VecCase[ii].vec_mode[kk].ampl >= tmpWarnAmpl) &&
							(VecCase[ii].vec_mode[kk].ampl >= tmpAmpl))
						{
							CopyLfoModeInfo(tmpModeInfo, VecCase[ii].vec_mode[kk]);
							VecMonDev[jj].vec_lead_mode.push_back(tmpModeInfo);
						}
					}
					if (!VecMonDev[jj].vec_mode.empty())
					{
						// 模式已按能量排序
						// sort(VecMonDev[jj].vec_mode.begin(),VecMonDev[jj].vec_mode.end(),g_SortModeByEny);
						// 将能量最大的模式作为主导模式
						VecMonDev[jj].mode = VecMonDev[jj].vec_mode.front();

						//根据主导频率，计算周期中的原始平均振幅，作为幅值条件，替代原有幅值（避开斜曲线对幅值计算的影响）
						checkTimeNum = 1;
						checkDataNum = 0;

						if (VecMonDev[jj].mode.freq > 0.001 && dtime > 0.0001)
							checkDataNum = (int)(1.2 * (1.0 / VecMonDev[jj].mode.freq / dtime + 1));
						else
							checkDataNum = (int)(3 * m_nSamples / 4);
						if (checkDataNum > m_nSamples) checkDataNum = m_nSamples;
						checkTimeNum = (int)(m_nSamples / checkDataNum);

						VecMonDev[jj].mode.aver_val = 0.0;
						ampl_aver_new = 0;
						ampl_aver_old = 0;
						countNum = 0;
						for (int nn = 0; nn < checkTimeNum; nn++)
						{
							temp_ampl_max = VecCase[ii].v2d_data[nn * checkDataNum];
							temp_ampl_min = temp_ampl_max;
							for (kk = nn * checkDataNum; kk < (nn + 1) * checkDataNum; kk++)
							{
								if (temp_ampl_max < VecCase[ii].v2d_data[kk]) temp_ampl_max = VecCase[ii].v2d_data[kk];
								if (temp_ampl_min > VecCase[ii].v2d_data[kk]) temp_ampl_min = VecCase[ii].v2d_data[kk];
							}

							VecMonDev[jj].mode.aver_val += (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);
						}

						VecMonDev[jj].mode.aver_val = VecMonDev[jj].mode.aver_val / checkTimeNum;
						// 填充时间
// 						VecMonDev[jj].mode.start_time = m_MonSys.start_time;
// 						VecMonDev[jj].mode.end_time = m_MonSys.end_time;
						//若该设备属于某一输电断面 则记录下其振荡模式
						for (kk = 0; kk < (int)VecMonDev[jj].vec_mode.size(); kk++)
						{
							for (int mm = 0; mm < m_VecIntline.size(); mm++)
							{
								if (m_VecIntline[mm].line_no == VecMonDev[jj].dev.dev_id)
								{
									m_VecIntline[mm].vec_mode.push_back(VecMonDev[jj].vec_mode[kk]);
									memcpy(&m_VecIntline[mm].curve, &VecMonDev[jj].curve, sizeof(LFO_CURVE_INFO));
								}
							}
						}
					}
					else
					{
						sprintf(m_LogString, "%s["INT_REC_FMT"]没有该频率段的模式[%f~%f],[%f,%f]", VecMonDev[jj].dev.dev_name, INT_REC_VAL(VecMonDev[jj].dev.dev_id),
							m_LfoPara.freq_min, m_LfoPara.freq_max, m_LfoPara.lack_damp, tmpAmpl);
						MessShow(m_LogString, LTERROR);
					}

					// 					for (kk =0; kk<(int)VecMonDev[jj].vec_lead_mode.size(); kk++)
					// 					{
					// 						sprintf(m_LogString,"%s["INT_REC_FMT"]lead_mode_fdape[%f,%f,%f,%f,%f]",VecMonDev[jj].dev.dev_name,INT_REC_VAL(VecMonDev[jj].dev.dev_id),
					// 							VecMonDev[jj].vec_lead_mode[kk].freq,VecMonDev[jj].vec_lead_mode[kk].damp,VecMonDev[jj].vec_lead_mode[kk].ampl,VecMonDev[jj].vec_lead_mode[kk].phas,VecMonDev[jj].vec_lead_mode[kk].engy);
					// 						MessShow(m_LogString,LTDEBUG);
					// 					}
				}
				break;
			}
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    GetIntMode
// FullName:  CLfoAnaBase::GetIntMode
// Access:    public 
// Returns:   int
// Qualifier:
// 统计输电断面的模式。
//************************************
int CLfoAnaBase::GetIntMode(vector<LFO_DEV_INFO>& VecMonDev)
{
	LFO_MODE_INFO tmp_lead_mode;
	float max_engy = -999999.0;
	int line_num;//断面支路数
	int same_line_num;//断面同调支路数
	vector<LFO_INTLINE_INFO> tmp_VecIntline;
	for (int mm = 0; mm < m_VecIntline.size(); mm++)
	{
		//所有模式按能量排序
		sort(m_VecIntline[mm].vec_mode.begin(), m_VecIntline[mm].vec_mode.end(), g_SortIntlineModeModeByEngy);
		sprintf(m_LogString, "支路%s["INT_REC_FMT"]模式数[%d]", m_VecIntline[mm].line_name, INT_REC_VAL(m_VecIntline[mm].line_no), m_VecIntline[mm].vec_mode.size());
		MessShow(m_LogString, LTINFO);
	}
	for (int ii = 0; ii < VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_ITFC)
		{
			continue;
		}
		memset(&tmp_lead_mode, 0, sizeof(LFO_MODE_INFO));
		memset(&VecMonDev[ii].mode, 0, sizeof(LFO_MODE_INFO));
		line_num = 0;
		for (int mm = 0; mm < m_VecIntline.size(); mm++)
		{
			if (fabs(m_VecIntline[mm].ampl_limit_s) < 0.001)
			{
				continue;
			}
			if (m_VecIntline[mm].inf_no == VecMonDev[ii].dev.dev_id)
			{
				tmp_VecIntline.push_back(m_VecIntline[mm]);

				//line_num ++;//统计断面支路
				for (int jj = 0; jj < m_VecIntline[mm].vec_mode.size(); jj++)
				{
					//获取能量最大的模式
					if (m_VecIntline[mm].vec_mode[jj].engy > max_engy)
					{
						max_engy = m_VecIntline[mm].vec_mode[jj].engy;
						memcpy(&tmp_lead_mode, &m_VecIntline[mm].vec_mode[jj], sizeof(LFO_MODE_INFO));
					}
				}
			}
		}
		sprintf(m_LogString, "%s["INT_REC_FMT"]主导模式频率[%.3f]初相位[%.2f]阻尼比[%.3f]", VecMonDev[ii].dev.dev_name, INT_REC_VAL(VecMonDev[ii].dev.dev_id), tmp_lead_mode.freq, tmp_lead_mode.phas, tmp_lead_mode.damp);
		MessShow(m_LogString, LTINFO);
		same_line_num = 0;
		for (int mm = 0; mm < tmp_VecIntline.size(); mm++)
		{
			for (int jj = 0; jj < tmp_VecIntline[mm].vec_mode.size(); jj++)
			{
				//与能量最大的模式进行同调判断
				if (fabs(tmp_VecIntline[mm].vec_mode[jj].freq - tmp_lead_mode.freq) < m_LfoPara.freq_diff
					&& (fabs(tmp_VecIntline[mm].vec_mode[jj].phas - tmp_lead_mode.phas) < m_LfoPara.phas_diff || fabs(fabs(tmp_VecIntline[mm].vec_mode[jj].phas - tmp_lead_mode.phas) - 180) < m_LfoPara.phas_diff))
				{
					same_line_num++;//统计同调支路
					VecMonDev[ii].mode.ampl += tmp_VecIntline[mm].vec_mode[jj].ampl;
					VecMonDev[ii].mode.aver_val += tmp_VecIntline[mm].vec_mode[jj].aver_val;
					VecMonDev[ii].mode.damp += tmp_VecIntline[mm].vec_mode[jj].damp;
					VecMonDev[ii].mode.engy += tmp_VecIntline[mm].vec_mode[jj].engy;
					VecMonDev[ii].mode.phas += tmp_VecIntline[mm].vec_mode[jj].phas;
					break;
				}
			}
		}
		VecMonDev[ii].is_sm_md = MENU_LFO_MENU_NO;
		if (same_line_num == tmp_VecIntline.size())
		{
			//断面同调
			VecMonDev[ii].is_sm_md = MENU_LFO_MENU_YES;
			VecMonDev[ii].mode.freq = tmp_lead_mode.freq;
			VecMonDev[ii].mode.damp = VecMonDev[ii].mode.damp / same_line_num;
			VecMonDev[ii].mode.aver_val = VecMonDev[ii].mode.aver_val / same_line_num;
			VecMonDev[ii].mode.phas = VecMonDev[ii].mode.phas / same_line_num;

			sprintf(m_LogString, "%s["INT_REC_FMT"]同调,频率[%f],阻尼比[%f],振幅[%f],均值[%f]", VecMonDev[ii].dev.dev_name, INT_REC_VAL(VecMonDev[ii].dev.dev_id), VecMonDev[ii].mode.freq, VecMonDev[ii].mode.damp, VecMonDev[ii].mode.ampl, VecMonDev[ii].curve.aver_val);
			MessShow(m_LogString, LTINFO);
		}
		else
		{
			//断面非同调
			sprintf(m_LogString, "%s["INT_REC_FMT"]非同调", VecMonDev[ii].dev.dev_name, INT_REC_VAL(VecMonDev[ii].dev.dev_id));
			MessShow(m_LogString, LTINFO);
			VecMonDev[ii].is_sm_md = MENU_LFO_MENU_NO;
			VecMonDev[ii].mode.freq = tmp_lead_mode.freq;
			VecMonDev[ii].mode.damp = VecMonDev[ii].mode.damp / same_line_num;
			VecMonDev[ii].mode.aver_val = VecMonDev[ii].mode.aver_val / same_line_num;
			VecMonDev[ii].mode.phas = VecMonDev[ii].mode.phas / same_line_num;
		}
	}
	for (int mm = 0; mm < m_VecIntline.size(); mm++)
	{
		vector<LFO_MODE_INFO>().swap(m_VecIntline[mm].vec_mode);
		m_VecIntline[mm].vec_mode.clear();
		m_VecIntline[mm].ampl_limit_s = 0.0;
		m_VecIntline[mm].ampl_limit_e = 0.0;
		// 		sprintf(m_LogString,"支路%s["INT_REC_FMT"]清空后模式数[%d]",m_VecIntline[mm].line_name,INT_REC_VAL(m_VecIntline[mm].line_no),m_VecIntline[mm].vec_mode.size());
		// 		MessShow(m_LogString,LTINFO);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    SysModeSum
// FullName:  CLfoAnaBase::SysModeSum
// Access:    public 
// Returns:   int
// Qualifier:
// 将各个设备模式信息按相同模式汇总，并按能量排序。
// 同一模式，阻尼处理??需完善
// 所有模式的平均频率为主导模式频率
// 所有模式中平均阻尼为主导模式阻尼
// 所有模式的幅值和为主导模式的幅值
// 所有模式的能量和为主导模式的能量
// 求取平均值时去掉一个最大值,去掉一个最小值后取平均
// 输入：模式相同判断参数（频率差，阻尼差），设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev) 实时模式（包含多个模式并排序）(vector<LFO_MODE_INFO>)
// 输出：模态分析结构(vector<LFO_MODE_SHAPE>)汇总模式（多个模式按能量排序）(LFO_MODE_INFO)，多个模式的参与设备填充实时模式信息
// vector< LFO_DEV_MODE>
//************************************
int CLfoAnaBase::SysModeSum(vector<LFO_DEV_INFO> VecMonDev, vector<LFO_MODE_SHAPE>& VecModeShape)
{
	ClearLfoModeShape(VecModeShape);
	int nFindFlag, ii, jj, kk;
	LFO_MODE_SHAPE tmpSysMode;
	LFO_DEV_MODE tmpDevInfo;
	// 每个设备的主导模式按频率汇总
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].mode.freq <= 0.01f || VecMonDev[ii].mode.engy < 1.0f)
		{
			// 没有该频率段模式
			continue;
		}
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
		{
			// 断面暂不统计为可观设备
			continue;
		}
		nFindFlag = -1;
		for (kk = 0; kk < (int)VecModeShape.size(); kk++)
		{
			if (fabs(VecMonDev[ii].mode.freq - VecModeShape[kk].mode.freq) <= m_LfoPara.freq_diff)
			{
				// 同一模式，记录ID
				nFindFlag = kk;
				break;
			}
		}

		if (nFindFlag < 0)
		{
			// 没有相同模式，增加
			ClearLfoModeShape(tmpSysMode);
			memset((void*)&tmpDevInfo, 0, sizeof(LFO_DEV_MODE));
			CopyLfoModeInfo(tmpSysMode.mode, VecMonDev[ii].mode);
			CopyLfoDevBase(tmpDevInfo.dev, VecMonDev[ii].dev);
			CopyLfoModeInfo(tmpDevInfo.mode, VecMonDev[ii].mode);
			// 			tmpSysMode.mode = VecMonDev[ii].mode;
			// 			tmpDevInfo.dev = VecMonDev[ii].dev;
			// 			tmpDevInfo.mode = VecMonDev[ii].mode;
			tmpSysMode.vec_dev.push_back(tmpDevInfo);
			VecModeShape.push_back(tmpSysMode);
		}
		else
		{
			// 同一模式，记录参与设备
			memset((void*)&tmpDevInfo, 0, sizeof(LFO_DEV_MODE));
			CopyLfoDevBase(tmpDevInfo.dev, VecMonDev[ii].dev);
			CopyLfoModeInfo(tmpDevInfo.mode, VecMonDev[ii].mode);
			// 			tmpDevInfo.dev = VecMonDev[ii].dev;
			// 			tmpDevInfo.mode = VecMonDev[ii].mode;
			VecModeShape[nFindFlag].vec_dev.push_back(tmpDevInfo);
		}
	}
	// 	for ( ii = 0; ii<(int)VecMonDev.size(); ii++ )
	// 	{
	// 		for ( jj = 0; jj<(int)VecMonDev[ii].vec_mode.size(); jj++ )
	// 		{
	// 			nFindFlag = -1;
	// 			for ( kk = 0; kk<(int)VecModeShape.size(); kk++ )
	// 			{
	// 				if ( fabs(VecMonDev[ii].vec_mode[jj].freq - VecModeShape[kk].mode.freq ) <= m_LfoPara.freq_diff )
	// 				{
	// 					// 同一模式，记录ID
	// 					nFindFlag = kk;
	// 					break;
	// 				}
	// 			}
	// 			
	// 			if ( nFindFlag < 0 )
	// 			{
	// 				// 没有相同模式，增加
	// 				memset((void *)&tmpSysMode,0,sizeof(LFO_MODE_SHAPE));
	// 				memset((void *)&tmpDevInfo,0,sizeof(LFO_DEV_MODE));
	// 				tmpSysMode.mode = VecMonDev[ii].vec_mode[jj];
	// 				tmpDevInfo.dev = VecMonDev[ii].dev;
	// 				tmpDevInfo.mode = VecMonDev[ii].vec_mode[jj];
	// 				tmpSysMode.vec_dev.push_back(tmpDevInfo);
	// 				VecModeShape.push_back(tmpSysMode);
	// 			}
	// 			else
	// 			{
	// 				// 同一模式，记录参与设备
	// 				memset((void *)&tmpDevInfo,0,sizeof(LFO_DEV_MODE));
	// 				tmpDevInfo.dev = VecMonDev[ii].dev;
	// 				tmpDevInfo.mode = VecMonDev[ii].vec_mode[jj];
	// 				VecModeShape[nFindFlag].vec_dev.push_back(tmpDevInfo);
	// 			}
	// 		}
	// 	}

	if (!VecModeShape.empty())
	{
		// 同一模式，阻尼处理??需完善
		// 所有模式的平均频率为主导模式频率
		// 所有模式中平均阻尼为主导模式阻尼
		// 所有模式的幅值和为主导模式的幅值
		// 所有模式的能量和为主导模式的能量
		// 求取平均值时去掉一个最大值,去掉一个最小值后取平均
		int tmpNum = 0;
		float tmpFreq, tmpDamp, tmpAmpl, tmpEngy, tmpEngyPer, tmpAmplMax;
		float tmpDampMax, tmpDampMin;

		sprintf(m_LogString, "模式数[%d]", VecModeShape.size());
		MessShow(m_LogString, LTINFO);
		for (ii = 0; ii < (int)VecModeShape.size(); ii++)
		{
			tmpNum = VecModeShape[ii].vec_dev.size();
			tmpFreq = 0.0f;
			tmpDamp = 0.0f;
			tmpAmpl = 0.0f;
			tmpEngy = 0.0f;
			tmpEngyPer = 0.0f;
			tmpAmplMax = 0.0f;

			if (!VecModeShape[ii].vec_dev.empty())
			{
				tmpDampMax = VecModeShape[ii].vec_dev.front().mode.damp;
			}
			else
			{
				tmpDampMax = 0.0f;
			}

			tmpDampMin = tmpDampMax;

			for (jj = 0; jj < (int)VecModeShape[ii].vec_dev.size(); jj++)
			{
				if (tmpAmplMax < VecModeShape[ii].vec_dev[jj].mode.ampl)
				{
					tmpAmplMax = VecModeShape[ii].vec_dev[jj].mode.ampl;
					tmpDamp = VecModeShape[ii].vec_dev[jj].mode.damp;
					tmpFreq = VecModeShape[ii].vec_dev[jj].mode.freq;
				}
				tmpAmpl += VecModeShape[ii].vec_dev[jj].mode.ampl;
				tmpEngy += VecModeShape[ii].vec_dev[jj].mode.engy;
				tmpEngyPer += VecModeShape[ii].vec_dev[jj].mode.engy_pre_time;
				if (VecModeShape[ii].vec_dev[jj].mode.damp > tmpDampMax)
				{
					tmpDampMax = VecModeShape[ii].vec_dev[jj].mode.damp;
				}
				if (VecModeShape[ii].vec_dev[jj].mode.damp < tmpDampMin)
				{
					tmpDampMin = VecModeShape[ii].vec_dev[jj].mode.damp;
				}
				sprintf(m_LogString, "模式[%d]可观设备[%d][%s]频率[%f]振幅[%f],阻尼比[%f]", ii + 1, jj + 1, VecModeShape[ii].vec_dev[jj].dev.dev_name,
					VecModeShape[ii].vec_dev[jj].mode.freq, VecModeShape[ii].vec_dev[jj].mode.ampl, VecModeShape[ii].vec_dev[jj].mode.damp);
				MessShow(m_LogString, LTINFO);
			}

			// 			if ( tmpNum > 0 && tmpNum <=2 )
			// 			{
			// 				VecModeShape[ii].mode.damp = tmpDamp / tmpNum;
			// 			}
			// 			else if ( tmpNum >= 3 )
			// 			{
			// 				tmpDamp = tmpDamp - tmpDampMax - tmpDampMin;
			// 				VecModeShape[ii].mode.damp = tmpDamp / (tmpNum-2);
			// 			}

			VecModeShape[ii].mode.freq = tmpFreq;
			VecModeShape[ii].mode.damp = tmpDamp;
			VecModeShape[ii].mode.ampl = tmpAmpl;
			VecModeShape[ii].mode.engy = tmpEngy;
			VecModeShape[ii].mode.engy_pre_time = tmpEngyPer;

			// 按能量由大到小排序机组模式
			sort(VecModeShape[ii].vec_dev.begin(), VecModeShape[ii].vec_dev.end(), g_SortDevModeByEngy);
			sprintf(m_LogString, "模式[%d]频率[%f]振幅[%f],阻尼比[%f],能量[%f],单位时间能量[%f],设备数[%d]", ii + 1, VecModeShape[ii].mode.freq, VecModeShape[ii].mode.ampl, VecModeShape[ii].mode.damp, VecModeShape[ii].mode.engy, VecModeShape[ii].mode.engy_pre_time, VecModeShape[ii].vec_dev.size());
			MessShow(m_LogString, LTINFO);
		}

		// 按能量由大到小排序主导模式
		sort(VecModeShape.begin(), VecModeShape.end(), g_SortModeShapeByEngy);
		// 		sprintf(m_LogString,"该模式频率[%f]振幅[%f]能量[%f]设备数[%d]",VecModeShape[0].mode.freq,VecModeShape[0].mode.ampl,VecModeShape[0].mode.engy,VecModeShape[0].vec_dev.size());
		// 		MessShow(m_LogString,LTINFO);
	}

	return 0;
}

//************************************
// Method:    CalcPartFact
// FullName:  CLfoAnaBase::CalcPartFact
// Access:    public 
// Returns:   int
// Qualifier:
// 参与因子估计值等于幅值的平方除以惯性时间常数。
// 输入：模态分析结构(vector<LFO_MODE_SHAPE>)
// 输出：模态分析结构(vector<LFO_MODE_SHAPE>)多个模式的参与设备，填充参与因子信息vector< LFO_DEV_MODE>
//************************************
int CLfoAnaBase::CalcPartFact(vector<LFO_MODE_SHAPE>& VecMonDev)
{
	// 	int   ii,jj;
	// 	float fMaxEngy_gn = 0;
	// 	float fMaxAmpl_gn = 0;
	// 	float fMaxEngy_ln = 0;
	// 	float fMaxAmpl_ln = 0;
	// 	float fMaxEngy_tr = 0;
	// 	float fMaxAmpl_tr = 0;
	// 
	// 	if (VecMonDev.empty())
	// 	{
	// 		return 0;
	// 	}
	// 
	// 	for ( ii = 0; ii<(int)VecMonDev.size(); ii++ )
	// 	{		
	// 		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
	// 		{
	// 			if (fMaxEngy_gn < VecMonDev[ii].mode_shape.engy) { fMaxEngy_gn = VecMonDev[ii].mode_shape.engy; }
	// 			if (fMaxAmpl_gn < VecMonDev[ii].mode_shape.ampl) { fMaxAmpl_gn = VecMonDev[ii].mode_shape.ampl; }
	// 		}
	// 		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
	// 		{
	// 			if (fMaxEngy_ln < VecMonDev[ii].mode_shape.engy) { fMaxEngy_ln = VecMonDev[ii].mode_shape.engy; }
	// 			if (fMaxAmpl_ln < VecMonDev[ii].mode_shape.ampl) { fMaxAmpl_ln = VecMonDev[ii].mode_shape.ampl; }
	// 		}
	// 		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
	// 		{
	// 			if (fMaxEngy_tr < VecMonDev[ii].mode_shape.engy) { fMaxEngy_tr = VecMonDev[ii].mode_shape.engy; }
	// 			if (fMaxAmpl_tr < VecMonDev[ii].mode_shape.ampl) { fMaxAmpl_tr = VecMonDev[ii].mode_shape.ampl; }
	// 		}
	// 	}
	// 
	// 	for (ii = 0; ii<(int)VecMonDev.size(); ii++) 
	// 	{
	// 		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
	// 		{
	// 			VecMonDev[ii].mode_shape.fact = VecMonDev[ii].mode_shape.ampl / fMaxAmpl_gn;
	// 		}
	// 		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
	// 		{
	// 			VecMonDev[ii].mode_shape.fact = VecMonDev[ii].mode_shape.ampl / fMaxAmpl_ln;
	// 		}
	// 		else if(VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
	// 		{
	// 			VecMonDev[ii].mode_shape.fact = VecMonDev[ii].mode_shape.ampl / fMaxAmpl_tr;		
	// 		}
	// 	}

	//	qsort((void *)ModeShape, iObjNum, sizeof(MODE_PART_OBJ), ModeShapeCompareByFact);

	return 0;
}

//************************************
// Method:    GetDevCluster
// FullName:  CLfoAnaBase::GetDevCluster
// Access:    public 
// Returns:   int
// Qualifier:
// 以参与因子最大的设备相位为基准，按设定角度值划分角度区间（90度），确定设备所属群。
// 输入：模态分析结构(vector<LFO_MODE_SHAPE>)
// 输出：模态分析结构(vector<LFO_MODE_SHAPE>)多个模式的参与设备，填充设备分群标志vector< LFO_DEV_MODE>
// 与参与因子最大设备在同一群设备为临界群。
// 与参与因子最大设备相位相反的群为相对群。
// 其余角度空间为余下群。
//************************************
int CLfoAnaBase::GetDevCluster(vector<LFO_MODE_SHAPE>& VecModeShape)
{
	return 0;
}

//************************************
// Method:    JudgeLfoCenter
// FullName:  CLfoAnaBase::JudgeLfoCenter
// Access:    public 
// Returns:   int
// Qualifier:
// TODO:暂不开发
//************************************
int CLfoAnaBase::JudgeLfoCenter(vector<LFO_MODE_SHAPE>& VecModeShape)
{
	return 0;
}

//************************************
// Method:    GetDevModeShape
// FullName:  CLfoAnaBase::GetDevModeShape
// Access:    public 
// Returns:   int
// Qualifier:
// 将模态分析结果中主导模式的参与设备信息填充到设备结构中。
// 输入：模态分析结构(vector<LFO_MODE_SHAPE>)，设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)
// 输出：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)，填充数据/安全状态，振荡统计（开始时间，结束时间，持续时间）
//************************************
int CLfoAnaBase::GetDevModeShape(vector<LFO_MODE_SHAPE> VecModeShape, vector<LFO_DEV_INFO>& VecMonDev)
{
	if (VecModeShape.empty())
	{
		return 0;
	}
	int ii, jj;
	// 填充设备模态信息
	for (ii = 0; ii < (int)VecModeShape.front().vec_dev.size(); ii++)
	{
		for (jj = 0; jj < (int)VecMonDev.size(); jj++)
		{
			if (VecMonDev[jj].stat == MENU_LFO_DEV_STS_OK && VecMonDev[jj].dev.dev_id == VecModeShape.front().vec_dev[ii].dev.dev_id)
			{
				CopyLfoModeInfo(VecMonDev[jj].mode_shape, VecModeShape.front().vec_dev[ii].mode);
				// 				VecMonDev[jj].mode_shape = VecModeShape.front().vec_dev[ii].mode;
				break;
			}
		}
	}
	return 0;
}

//************************************
// Method:    JudgeDevLfoStat
// FullName:  CLfoAnaBase::JudgeDevLfoStat
// Access:    public 
// Returns:   int
// Qualifier:
// 判断设备安全状态。
// 输入：幅值，频率，阻尼比，持续时间参数。设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)，主导模式（实时模式中的能量最大模式）(LFO_MODE_INFO)，振荡统计（开始时间，结束时间，持续时间）
// 输出：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)，满足判据标志，满足时间标志，安全状态，更新振荡统计（振荡开始时间，结束时间，持续时间），安全持续时间
// 告警判据：
// (频率>频率下限)&&(频率<频率上限)&&
//  (模式幅值>幅值门槛值)&&
//  (阻尼<阻尼比限值)&&
//  满足上面三个条件后再判断是否满足持续时间。从满足上面三个条件时开始计时。
//  (持续时间>告警持续时间限值) 
//  安全判据：
//  (频率>频率下限)&&(频率<频率上限)&&
//  (模式幅值<幅值门槛值)&&
//  (阻尼>阻尼比限值)&&
//  (持续时间>安全持续时间限值)
//************************************
int CLfoAnaBase::JudgeDevLfoStat(vector<LFO_DEV_INFO>& VecMonDev)
{
	sprintf(m_LogString, "判断设备振荡状态");
	MessShow(m_LogString, LTINFO);
	float          LD_Warn_Damping = 3.0;         //判欠阻尼时，阻尼比临界值
	int            LD_Times = 2;           //满足欠阻尼判据次数大于该值，欠阻尼告警(>=1)
	float          LFO_Warn_Damping = 0.00001f;     //判低频振荡时，阻尼比临界值
	float          Out_LFO_Warn_Damping = 0.5;         //退出低频振荡告警时，阻尼比临界值
	int            LFO_Times = 2;           //满足低频振荡判据次数大于该值，低频振荡告警(>=1)
	float          LFO_Forewarn_Damping = 1.5;         //判低频振荡预警时，阻尼比临界值
	int            LFO_Fore_Times = 2;           //满足低频振荡预警判据次数大于该值，低频振荡预警(>=1)
	int            DN_Times = 2;           //满足阻尼正常判据次数大于该值，显示阻尼正常(>=1)
	float          LFO_Ampl_Per = 0.1f;         //判低频振荡时，振幅与均值比例限值
	float          LFO_Ampl_Per_Fir = 0.1f;         //判低频振荡时，振幅与均值比例最小限值  206-1-18
	float          temp_ampl = 0.0;
	float		   temp_freq_margin = 2 * m_LfoPara.freq_diff;	//同模频率差
	int			   ii, jj, lfo_stat, oscFlag, nn, kk;
	int            checkTimeNum = 0;
	int            checkDataNum = 0;
	float          temp_ampl_max = 0;
	float          temp_ampl_min = 0;
	int            times = 0;
	float          ampl_aver_new = 0;    // 2008-05-08 by zhw
	float          ampl_aver_old = 0;
	int            countNum = 0;
	float          m_nSmpFre, m_Matchtime[PRONY_DATA_MAX];         //  匹配时刻数据
	int            m_nSamples, m_nOrigDlt, m_nSmpStp, m_nOrigSam;					    //  计算用采样点个数
// 	float          m_ProMatchtime[PRONY_DATA_MAX];      //  计算用匹配时刻		
	LFO_DEV_STAT	tmpDevStat;
	LFO_LEAD_MODE	temp_leadMode;
	// 	memset( (void *)m_Matchtime, 0, sizeof(float) * PRONY_DATA_MAX );
	// 	memset( (void *)m_ProMatchtime, 0, sizeof(float) * PRONY_DATA_MAX );
	memset((void*)&temp_leadMode, 0, sizeof(LFO_LEAD_MODE));
	m_nOrigDlt = (float)(1.0 / MAX_FRA_PER_SEC);
	m_nOrigSam = m_LfoPara.time_leng * m_MonSys.sample_org;
	m_nSmpFre = m_LfoPara.sample_freq;
	m_nSmpStp = (int)(m_MonSys.sample_org / m_nSmpFre + 0.5); //  采样间隔0.1
// 	for( ii=0; ii < m_nOrigSam; ii++)
// 	{
// 		m_Matchtime[ii] =  m_nOrigDlt*ii;
// 	} 

// 	for ( ii=0; ii<m_nSamples; ii++)
// 	{
// 		jj = m_nSmpStp * ii;
// 		m_ProMatchtime[ii] = m_Matchtime[jj];
// 	}
	float dtime;

	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].stat != MENU_LFO_DEV_STS_OK)
		{
			memset((void*)&VecMonDev[ii].osciFlag, 0, sizeof(OSCILLAT_FLAG));
			VecMonDev[ii].osciFlag.obj_id = VecMonDev[ii].dev.dev_id;
			continue;
		}
		if (VecMonDev[ii].vec_mode.empty() && VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_ITFC)
		{
			memset((void*)&VecMonDev[ii].osciFlag, 0, sizeof(OSCILLAT_FLAG));
			VecMonDev[ii].osciFlag.obj_id = VecMonDev[ii].dev.dev_id;
			continue;
		}
		// 根据设备类型获取幅值门槛值
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
		{
			VecMonDev[ii].osciFlag.begin_ampl = VecMonDev[ii].dev.ampl_limit_s;
			VecMonDev[ii].osciFlag.end_ampl = VecMonDev[ii].dev.ampl_limit_e;
			sprintf(m_LogString, "机组[%s]容量[%f],振幅限值[%f]", VecMonDev[ii].dev.dev_name, VecMonDev[ii].dev_info.pmax, VecMonDev[ii].dev.ampl_limit_e);
			MessShow(m_LogString, LTINFO);
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
		{
			VecMonDev[ii].osciFlag.begin_ampl = VecMonDev[ii].dev.ampl_limit_s;
			VecMonDev[ii].osciFlag.end_ampl = VecMonDev[ii].dev.ampl_limit_e;
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
		{
			VecMonDev[ii].osciFlag.begin_ampl = VecMonDev[ii].dev.ampl_limit_s;
			VecMonDev[ii].osciFlag.end_ampl = VecMonDev[ii].dev.ampl_limit_e;
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
		{
			if (VecMonDev[ii].is_sm_md == MENU_LFO_MENU_YES)
			{
				sprintf(m_LogString, "断面[%s]同调,进行振荡判断", VecMonDev[ii].dev.dev_name);
				MessShow(m_LogString, LTINFO);
				VecMonDev[ii].osciFlag.begin_ampl = VecMonDev[ii].dev.ampl_limit_s;
				VecMonDev[ii].osciFlag.end_ampl = VecMonDev[ii].dev.ampl_limit_e;
			}
			else if (VecMonDev[ii].is_sm_md == MENU_LFO_MENU_NO)
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_SCT;
				memset((void*)&VecMonDev[ii].osciFlag, 0, sizeof(OSCILLAT_FLAG));
				sprintf(m_LogString, "断面[%s]非同调,不进行振荡判断", VecMonDev[ii].dev.dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
		{
			continue;
		}
		else
		{
			sprintf(m_LogString, "设备类型错误[%d]", VecMonDev[ii].dev.dev_type);
			MessShow(m_LogString, LTERROR);
			continue;
		}
		temp_leadMode.vbase = VecMonDev[ii].dev_info.maxvbase;
		temp_leadMode.obj_id = VecMonDev[ii].dev.dev_id;
		temp_leadMode.amplitude = VecMonDev[ii].mode.ampl;
		temp_leadMode.frequency = VecMonDev[ii].mode.freq;
		temp_leadMode.damping = VecMonDev[ii].mode.damp;
		temp_leadMode.phase = VecMonDev[ii].mode.phas;
		temp_leadMode.aver_ampl = VecMonDev[ii].mode.aver_val;
		temp_leadMode.aver_ampl_rat = 0.0;  //冗余信息
		temp_leadMode.aver_ampl2 = VecMonDev[ii].curve.max_ampl / 2;  //冗余信息
		temp_leadMode.aver_ampl_rat2 = 0.0;  //冗余信息

// 无符合要求的模式时，置阻尼比为最大值100 
		if (temp_leadMode.amplitude < 0.0001)
		{
			temp_leadMode.damping = 100.0;
		}

		//根据主导频率，计算周期中的原始平均振幅，作为幅值条件，替代原有幅值（避开斜曲线对幅值计算的影响）
		// 		sprintf(m_LogString,"开始计算平均振幅");
		// 		MessShow(m_LogString);
		if (VecMonDev[ii].v2d_data.size() != 0)
		{
			m_nSamples = VecMonDev[ii].v2d_data.size();
			// 			sprintf(m_LogString,"m_nSamples=[%d],size()=%d,m_LfoPara.time_leng = %d",m_nSamples,VecMonDev[ii].v2d_data.size(),m_LfoPara.time_leng);
			// 			MessShow(m_LogString);
			dtime = float(m_LfoPara.time_leng) / float(VecMonDev[ii].v2d_data.size());
			// 			sprintf(m_LogString,"dtime=[%f]",dtime);
			// 			MessShow(m_LogString);
			checkTimeNum = 1;
			checkDataNum = 0;
			if (temp_leadMode.frequency > 0.001 && dtime > 0.0001)
				checkDataNum = (int)(1.2 * (1.0 / temp_leadMode.frequency / dtime + 1));
			else
				checkDataNum = (int)(3 * m_nSamples / 4);
			// 			sprintf(m_LogString,"checkDataNum=[%d]",checkDataNum);
			// 			MessShow(m_LogString);
			if (checkDataNum > m_nSamples) checkDataNum = m_nSamples;

			checkTimeNum = (int)(m_nSamples / checkDataNum);

			// 			sprintf(m_LogString,"checkTimeNum=[%d]",checkTimeNum);
			// 			MessShow(m_LogString);
			temp_leadMode.aver_ampl = 0;
			ampl_aver_new = 0;
			ampl_aver_old = 0;
			countNum = 0;
			// 			sprintf(m_LogString,"checkTimeNum=[%d]checkDataNum=[%d],size=[%d]",checkTimeNum,checkDataNum,VecMonDev[ii].v2d_data.size());
			// 			MessShow(m_LogString);
			for (nn = 0; nn < checkTimeNum; nn++)
			{
				temp_ampl_max = VecMonDev[ii].v2d_data[nn * checkDataNum];
				temp_ampl_min = temp_ampl_max;
				for (kk = nn * checkDataNum; kk < (nn + 1) * checkDataNum; kk++)
				{
					if (temp_ampl_max < VecMonDev[ii].v2d_data[kk]) temp_ampl_max = VecMonDev[ii].v2d_data[kk];
					if (temp_ampl_min > VecMonDev[ii].v2d_data[kk]) temp_ampl_min = VecMonDev[ii].v2d_data[kk];
				}
				temp_leadMode.aver_ampl += (float)(fabs(temp_ampl_max - temp_ampl_min) / 2.0);
			}

			temp_leadMode.aver_ampl = temp_leadMode.aver_ampl / checkTimeNum;
		}
		else
		{
			temp_leadMode.aver_ampl = temp_leadMode.amplitude;
		}

		sprintf(m_LogString, "设备最大振幅[%f]平均振幅[%f],阻尼比[%f],频率[%f]", temp_leadMode.aver_ampl2, temp_leadMode.aver_ampl, VecMonDev[ii].osciFlag.LM_damp, VecMonDev[ii].osciFlag.LM_frequency);
		MessShow(m_LogString);
		if (VecMonDev[ii].osciFlag.LM_damp < 0.005 && VecMonDev[ii].osciFlag.LM_frequency < 0.005)
		{
			times = 1;
			EstimateSingleObjState(&VecMonDev[ii].osciFlag, &temp_leadMode, times, m_LfoPara.freq_diff);
		}
		else
		{
			times = 2;
			EstimateSingleObjState(&VecMonDev[ii].osciFlag, &temp_leadMode, times, m_LfoPara.freq_diff);
		}
		// 		sprintf(m_LogString,"设备状态[%d]",VecMonDev[ii].osciFlag.lfo_warn);
		// 		MessShow(m_LogString);

		if (VecMonDev[ii].osciFlag.lfo_warn == 1)
		{
			VecMonDev[ii].stat = MENU_LFO_DEV_STS_WRN;                   //LFO告警
		}
		else
		{
			if (VecMonDev[ii].osciFlag.lackdamp_warn == 1)
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_SCT;                     //  欠阻尼不告警  // 2008-08-20
			}
			else
			{
				if (VecMonDev[ii].osciFlag.damp_norm != 1)
				{
					if (VecMonDev[ii].osciFlag.lfo_forewarn == 1)
					{
						VecMonDev[ii].stat = MENU_LFO_DEV_STS_FWN;       //LFO预警
					}
					else
					{
						VecMonDev[ii].stat = MENU_LFO_DEV_STS_SCT;       //安全
					}
				}
				else
				{
					if (VecMonDev[ii].osciFlag.state == 0)    //first time
					{
						VecMonDev[ii].stat = MENU_LFO_DEV_STS_SCT;       //安全
					}
					else
					{
						VecMonDev[ii].stat = VecMonDev[ii].osciFlag.state;
					}

				}
			}
		}
	}
	return 0;

}
// 判断单个监视对象
void CLfoAnaBase::EstimateSingleObjState(OSCILLAT_FLAG* osciFlag, LFO_LEAD_MODE* leadMode, int times, float SameFreqDifVal)
{

	float          LD_Warn_Damping = 3.0;         //判欠阻尼时，阻尼比临界值
	int            LD_Times = 2;           //满足欠阻尼判据次数大于该值，欠阻尼告警(>=1)
	float          LFO_Warn_Damping = 0.00001f;     //判低频振荡时，阻尼比临界值
	float          Out_LFO_Warn_Damping = 0.5;         //退出低频振荡告警时，阻尼比临界值
	int            LFO_Times = 2;           //满足低频振荡判据次数大于该值，低频振荡告警(>=1)
	float          LFO_Forewarn_Damping = 1.5;         //判低频振荡预警时，阻尼比临界值
	int            LFO_Fore_Times = 2;           //满足低频振荡预警判据次数大于该值，低频振荡预警(>=1)
	int            DN_Times = 2;           //满足阻尼正常判据次数大于该值，显示阻尼正常(>=1)
	float          LFO_Ampl_Per = 0.1f;         //判低频振荡时，振幅与均值比例限值
	float          LFO_Ampl_Per_Fir = 0.1f;         //判低频振荡时，振幅与均值比例最小限值  206-1-18
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
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre1 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.ln_swn_ampl1;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre1 && leadMode->vbase < m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre2 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.ln_swn_ampl2;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre3 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.ln_swn_ampl3;
				}
			}
			else if (tmptalbeno == TR_DEVICE_NO_TAB || tmptalbeno == TRWD_DEVICE_NO_TAB)
			{
				if (leadMode->vbase < m_LfoPara.volt_thre1)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre1 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.tr_swn_ampl1;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre1 && leadMode->vbase < m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre2 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.tr_swn_ampl2;

				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre3 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.tr_swn_ampl3;

				}
			}
			else
			{
				LFO_Ampl_Per = m_LfoPara.gn_wn_ampl;
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
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre1 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.ln_ewn_ampl1;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre1 && leadMode->vbase < m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre2 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.ln_ewn_ampl2;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre3 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.ln_ewn_ampl3;
				}
			}
			else if (tmptalbeno == TR_DEVICE_NO_TAB || tmptalbeno == TRWD_DEVICE_NO_TAB)
			{
				if (leadMode->vbase < m_LfoPara.volt_thre1)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre1 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.tr_ewn_ampl1;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre1 && leadMode->vbase < m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre2 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.tr_ewn_ampl2;
				}
				else if (leadMode->vbase >= m_LfoPara.volt_thre2)
				{
					//			LFO_Ampl_Per     = m_LfoPara.ampl_thre3 ;//百分比改为绝对值
					LFO_Ampl_Per = m_LfoPara.tr_ewn_ampl3;
				}
			}
			else
			{
				LFO_Ampl_Per = m_LfoPara.gn_wn_ampl1;
			}
		}
	}
	LFO_Ampl_Per_Fir = LFO_Ampl_Per * m_LfoPara.fjud_wnap_per / 100;//绝对值  2006-1-18
	temp_ampl = leadMode->aver_ampl;

	LD_Warn_Damping = m_LfoPara.lack_damp;
	LFO_Warn_Damping = m_LfoPara.lfo_damp;
	LFO_Forewarn_Damping = m_LfoPara.flfo_damp;
	Out_LFO_Warn_Damping = LD_Warn_Damping;  //2006-1-18

	LD_Times = m_LfoPara.lfo_jud_num;
	LFO_Fore_Times = m_LfoPara.lfo_jud_num + 1;
	LFO_Times = m_LfoPara.lfo_jud_num;

	//初次分析
	if (times == 1)
	{
		//清除标志位
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
		// 		//无振荡模式时 分清原因进行处理（计算不收敛、拟合无效、没有符合频率范围的模式）
		// 		if(temp_leadMode.amplitude <= ZEROVALUE)                       
		// 		{
		// 			//m_ClusOsci[i].LM_damp = 100;
		// 			continue;
		// 		}
		if (osciFlag->LM_frequency < SameFreqDifVal)
			temp_freq_margin = 0;
		else
			temp_freq_margin = (float)fabs(osciFlag->LM_frequency - leadMode->frequency);
		sprintf(m_LogString, "obj_id="INT_REC_FMT", 阻尼比damping=%f, 频率frequency=%f[df=%f],主导模式振幅amplitude=%f,phase=%f,平均振幅aver_ampl=%f[%f]",
			INT_REC_VAL(leadMode->obj_id), leadMode->damping, leadMode->frequency, temp_freq_margin, leadMode->amplitude, leadMode->phase, leadMode->aver_ampl, LFO_Ampl_Per);
		MessShow(m_LogString);
		//振荡未出现或平息（幅值条件）
		if (temp_ampl < LFO_Ampl_Per) //leadMode->aver_ampl_rat < LFO_Ampl_Per)  
		{
			// 2008-05-11 by zhw 0.05 0.08
/*
			if(leadMode->damping < LFO_Warn_Damping)
			{
				osciFlag->times_lw += 1;
			}*/

		}
		else  //出现振荡（幅值条件）
		{
			if (leadMode->amplitude < m_LfoPara.para_Ampl_aver * temp_ampl)
			{
				sprintf(m_LogString, "amplitude=[%6.2f]< [%6.2f]*[%6.2f]", leadMode->amplitude, m_LfoPara.para_Ampl_aver, temp_ampl);
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
	else   //正常运行
	{
		osciFlag->obj_id = leadMode->obj_id;
		// 2007-04-12 修改，避免振荡初期，osciFlag->LM_frequency无数值，导致temp_freq_margin超范围，不进行判断。增加对osciFlag->LM_frequency判断
		//temp_freq_margin = (float)fabs(osciFlag->LM_frequency - leadMode->frequency);
		//printf(">>>>>>>>> obj_id=%d, damping=%f, frequency=%f[df=%f]\n", leadMode->obj_id, leadMode->damping, leadMode->frequency,temp_freq_margin);
		if (osciFlag->LM_frequency < SameFreqDifVal)
			temp_freq_margin = 0;
		else
			temp_freq_margin = (float)fabs(osciFlag->LM_frequency - leadMode->frequency);
		sprintf(m_LogString, "obj_id="INT_REC_FMT", 阻尼比damping=%f, 频率frequency=%f[df=%f],主导模式振幅amplitude=%f,phase=%f,平均振幅aver_ampl=%f[%f]",
			INT_REC_VAL(leadMode->obj_id), leadMode->damping, leadMode->frequency, temp_freq_margin, leadMode->amplitude, leadMode->phase, leadMode->aver_ampl, LFO_Ampl_Per);
		MessShow(m_LogString);
		//振荡未出现或平息（幅值条件）
		// 2008-08-12 by zhw
		if (temp_ampl < LFO_Ampl_Per) //if(leadMode->aver_ampl_rat < LFO_Ampl_Per)  
		{
			// 延时退出告警
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
				//清除标志位
				osciFlag->lackdamp_warn = 0;
				osciFlag->times_ldw = 0;
				osciFlag->damp_norm = 0;
				//osciFlag->times_dn      = 0;
				//osciFlag->lfo_warn      = 0;
				osciFlag->lfo_forewarn = 0;

				// 2005-12-23  清空计数器
				osciFlag->times_dn = -10;
			}

			//2006-1-18  防止低阻尼振荡不退出
			if (osciFlag->times_dn < (-10 - LFO_Times) && (temp_ampl < LFO_Ampl_Per_Fir) && (leadMode->damping >= 0))
			{
				//清除标志位
				osciFlag->lackdamp_warn = 0;
				osciFlag->times_ldw = 0;
				osciFlag->damp_norm = 0;
				//osciFlag->times_dn      = 0;
				//osciFlag->lfo_warn      = 0;
				osciFlag->lfo_forewarn = 0;

				// 2005-12-23  清空计数器
				osciFlag->times_dn = -10;
			}
			else
			{
				//2006-6-5  防止低阻尼振荡不退出
				if (osciFlag->times_dn < (-10 - (2 * LFO_Times)))
				{
					//清除标志位
					osciFlag->lackdamp_warn = 0;
					osciFlag->times_ldw = 0;
					osciFlag->damp_norm = 0;
					//osciFlag->times_dn      = 0;
					//osciFlag->lfo_warn      = 0;
					osciFlag->lfo_forewarn = 0;

					// 2005-12-23  清空计数器
					osciFlag->times_dn = -10;
				}
			}

			if (osciFlag->lfo_warn == 0)
			{
				// 2008-05-11 by zhw 0.05 0.08

				if (temp_freq_margin < SameFreqDifVal)
				{
					sprintf(m_LogString, "幅值:%f[%f],频差:%f[%f],窗口数不计数,不清零", temp_ampl, LFO_Ampl_Per, temp_freq_margin, SameFreqDifVal);
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
		else  //出现振荡（幅值条件）
		{
			if (leadMode->amplitude < m_LfoPara.para_Ampl_aver * temp_ampl)
			{
				// 2008-10-22 by zhw for nw
				osciFlag->times_lw = 0;
				osciFlag->times_lfw = 0;
				sprintf(m_LogString, "主导模式振幅amplitude=[%6.2f]< [%6.2f]*[%6.2f](主导模式幅值占波形均值比系数 * 平均振幅),窗口次数清零！", leadMode->amplitude, m_LfoPara.para_Ampl_aver, temp_ampl);
				MessShow(m_LogString);
			}
			else
			{
				if (osciFlag->times_dn <= -10)
				{
					osciFlag->times_dn = 0;
				}

				//  LFO  Forewarning  考虑已发生LFO，怎样避免发预警 2006-09-24
				if (osciFlag->lfo_warn == 0)
				{
					if (osciFlag->lfo_forewarn == 0)
					{
						if (osciFlag->times_lfw == 0)  // 2008-01-22 lsf 第一次不判断频差
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
					if (osciFlag->times_lw == 0)  // 2008-01-22 lsf 第一次不判断频差
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

				//  考虑已发生LFO，避免发预警信息 2006-09-24
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

						if (osciFlag->times_dn >= LD_Times) // DN_Times 与阻尼不足配合
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
	sprintf(m_LogString, "负阻尼正常=%d, 负阻尼=%d, 连续满足预警条件窗口数=%d, 连续满足告警条件窗口数=%d, 欠阻尼标志=%d, 预警标志= %d, 告警标志=%d",
		osciFlag->times_dn, osciFlag->times_ldw, osciFlag->times_lfw, osciFlag->times_lw,
		osciFlag->lackdamp_warn, osciFlag->lfo_forewarn, osciFlag->lfo_warn);
	MessShow(m_LogString);
	return;
}


//************************************
// 判断设备小扰动安全状态。
//************************************
int CLfoAnaBase::JudgeDevOscStat(vector<LFO_DEV_INFO>& VecMonDev)
{
	// 	int ii,oscFlag;
	// 	for (ii =0; ii<(int)VecMonDev.size(); ii++)
	// 	{
	// // 		sprintf(m_LogString,"【yjj1】JudgeDevOscStat状态osc_stat_old[%d],osc_stat[%d],平均振幅[%f]",VecMonDev[ii].osc_stat_old,VecMonDev[ii].osc_stat,VecMonDev[ii].mode.ampl );
	// //   	MessShow(m_LogString,LTERROR);
	// 		
	// 		if (VecMonDev[ii].osc_stat != MENU_LFO_DEV_STS_OK)
	// 		{
	// 			continue;
	// 		}
	// 
	// //  		sprintf(m_LogString,"【yjj1】设备开始时间["INT_REC_FMT"],结束时间["INT_REC_FMT"],持续开始["INT_REC_FMT"]持续结束["INT_REC_FMT"]",VecMonDev[ii].start_time,VecMonDev[ii].end_time,VecMonDev[ii].osc_start_time,VecMonDev[ii].osc_end_time );
	// //   		MessShow(m_LogString,LTERROR);	
	// //判断小扰动统计状态
	// 		if (VecMonDev[ii].osc_stat_old == MENU_LFO_DEV_STS_SCT)
	// 		{
	// 	//		if ((VecMonDev[ii].mode.aver_val-m_LfoPara.osc_pow_ampl)>0.001 )
	// 			if ((VecMonDev[ii].mode.ampl > m_LfoPara.para_Ampl_aver * VecMonDev[ii].mode.aver_val)
	// 				 && (VecMonDev[ii].mode.ampl-m_LfoPara.osc_pow_ampl)>0.001 )
	// 			{
	// 				VecMonDev[ii].osc_stat = MENU_LFO_DEV_STS_WRN;
	// 				oscFlag = 1;
	// 			}
	// 			else
	// 			{
	// 				VecMonDev[ii].osc_stat = MENU_LFO_DEV_STS_SCT;
	// 				oscFlag = 0;
	// 			}
	// 			if (oscFlag == 1)
	// 			{
	// 				// 更新小扰动持续时间
	// 				if (VecMonDev[ii].osc_start_time <=0)
	// 				{
	// 					// 初次满足小扰动条件
	// 					VecMonDev[ii].osc_start_time = VecMonDev[ii].start_time;
	// 					VecMonDev[ii].osc_end_time   = VecMonDev[ii].end_time;
	// 				}
	// 				else
	// 				{
	// 					// 非初次满足小扰动条件，更新小扰动统计时间
	// 					if (VecMonDev[ii].start_time < VecMonDev[ii].osc_start_time)
	// 					{
	// 						VecMonDev[ii].osc_start_time = VecMonDev[ii].start_time;
	// 					}
	// 					if (VecMonDev[ii].end_time > VecMonDev[ii].osc_end_time)
	// 					{
	// 						VecMonDev[ii].osc_end_time = VecMonDev[ii].end_time;
	// 					}
	// 				}
	// 				// 计算持续时间
	// 				VecMonDev[ii].hold_time = VecMonDev[ii].osc_end_time - VecMonDev[ii].osc_start_time;
	// 			}
	// 			else
	// 			{
	// 				// 不满足条件状态持续，时间清零
	// 				VecMonDev[ii].osc_start_time = 0;
	// 				VecMonDev[ii].osc_end_time   = 0;
	// 				VecMonDev[ii].osc_hold_time  = 0;
	// 			}
	// 		}
	// 		else if (VecMonDev[ii].osc_stat_old >= MENU_LFO_DEV_STS_WRN)
	// 		{
	// 			if ((VecMonDev[ii].mode.ampl > m_LfoPara.para_Ampl_aver * VecMonDev[ii].mode.aver_val)
	// 				&& (VecMonDev[ii].mode.ampl-m_LfoPara.osc_pow_ampl)>0.001 )
	// 			{
	// 				VecMonDev[ii].osc_stat = MENU_LFO_DEV_STS_WRN;
	// 				oscFlag = 1;
	// 			}
	// 			else
	// 			{
	// 				VecMonDev[ii].osc_stat = MENU_LFO_DEV_STS_SCT;
	// 				oscFlag = 0;
	// 			}
	// 			if (oscFlag == 0)
	// 			{
	// 				// 更新小扰动持续时间
	// 				if (VecMonDev[ii].osc_start_time <=0)
	// 				{
	// 					// 初次满足小扰动条件
	// 					VecMonDev[ii].osc_start_time = VecMonDev[ii].start_time;
	// 					VecMonDev[ii].osc_end_time   = VecMonDev[ii].end_time;
	// 				}
	// 				else
	// 				{
	// 					// 非初次满足小扰动条件，更新小扰动统计时间
	// 					if (VecMonDev[ii].start_time < VecMonDev[ii].osc_start_time)
	// 					{
	// 						VecMonDev[ii].osc_start_time = VecMonDev[ii].start_time;
	// 					}
	// 					if (VecMonDev[ii].end_time > VecMonDev[ii].osc_end_time)
	// 					{
	// 						VecMonDev[ii].osc_end_time = VecMonDev[ii].end_time;
	// 					}
	// 				}
	// 				// 计算持续时间
	// 				VecMonDev[ii].hold_time = VecMonDev[ii].osc_end_time - VecMonDev[ii].osc_start_time;
	// 			}		
	// 		}
	// //  		sprintf(m_LogString,"【yjj2】JudgeDevOscStat状态osc_stat_old[%d],osc_stat[%d],平均振幅[%f]",VecMonDev[ii].osc_stat_old,VecMonDev[ii].osc_stat,VecMonDev[ii].mode.ampl );
	// //  		MessShow(m_LogString,LTERROR);	
	// // 		sprintf(m_LogString,"【yjj2】设备开始时间["INT_REC_FMT"],结束时间["INT_REC_FMT"],持续开始["INT_REC_FMT"]持续结束["INT_REC_FMT"]",VecMonDev[ii].start_time,VecMonDev[ii].end_time,VecMonDev[ii].osc_start_time,VecMonDev[ii].osc_end_time );
	// //  		MessShow(m_LogString,LTERROR);	
	// 	}
	// 	// 清空数据
	// 	for (ii =0; ii<(int)VecMonDev.size(); ii++ )
	// 	{
	// 		// 		vector<float> bak_data;
	// 		// 		bak_data.clear();
	// 		// 		bak_data.insert(bak_data.end(),VecMonDev[ii].v2d_data.begin(),VecMonDev[ii].v2d_data.end());				
	// 		// 		VecMonDev[ii].back_data.push_back(bak_data);
	// 		//		VecMonDev[ii].back_data.insert(VecMonDev[ii].back_data.end(),VecMonDev[ii].v2d_data.begin(),VecMonDev[ii].v2d_data.end());
	// //		if (VecMonDev[ii].osc_stat >= MENU_LFO_DEV_STS_WRN)
	// //		{
	// //			for ( int jjkk =0; jjkk<(int)VecMonDev[ii].v2d_data.size(); jjkk++)
	// //			{
	// //				sprintf(m_LogString,"提示2：有功功率[ %f ]",VecMonDev[ii].v2d_data[jjkk]);
	// //				MessShow(m_LogString,LTINFO);
	// //			}
	// //		}		
	// 		if (!VecMonDev[ii].v2d_data.empty())
	// 		{
	// 			vector<float>().swap(VecMonDev[ii].v2d_data);
	// 			VecMonDev[ii].v2d_data.clear();
	// 		}
	// 	}
	return 0;
}
//************************************
// Method:    IsWarn
// FullName:  CLfoAnaBase::IsWarn
// Access:    public 
// Returns:   int
// Qualifier:
// (频率>频率下限)&&(频率<频率上限)&&
//  (模式幅值>幅值门槛值)&&
//  (阻尼<阻尼比限值)&&
//************************************
int CLfoAnaBase::IsWarn(LFO_MODE_INFO& Mode, LFO_PARA_STRUCT& Para, const int devtype)
{
	int nFlag = 0;
	float tmpAmpl = 0.0f;
	if (devtype == MENU_LFO_DEV_GEN)
	{
		tmpAmpl = Para.gn_wn_ampl;
	}
	else if (devtype == MENU_LFO_DEV_LINE)
	{
		tmpAmpl = Para.ln_wn_ampl;
	}
	else if (devtype == MENU_LFO_DEV_TR)
	{
		tmpAmpl = Para.tr_wn_ampl;
	}
	else
	{
		sprintf(m_LogString, "设备类型错误[%d]", devtype);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	// 频率范围在模式保存时已经过滤
	if (Mode.freq > Para.freq_min &&
		Mode.freq < Para.freq_max &&
		Mode.ampl > tmpAmpl &&
		Mode.damp < Para.lfo_damp)
	{
		// 满足振荡条件
		nFlag = 1;
	}

	sprintf(m_LogString, "告警判断：频率[%f](%f~%f),幅值[%f](%f),阻尼比[%f](%f),是否振荡[%d]",
		Mode.freq, Para.freq_min, Para.freq_max, Mode.ampl, tmpAmpl, Mode.damp, Para.lfo_damp, nFlag);
	MessShow(m_LogString, LTINFO);
	return nFlag;
}

//************************************
// Method:    IsSafe
// FullName:  CLfoAnaBase::IsSafe
// Access:    public 
// Returns:   int
// Qualifier:
//  (频率>频率下限)&&(频率<频率上限)&&
//  (模式幅值<幅值门槛值)&&
//  (阻尼>阻尼比限值)&&
//************************************
int CLfoAnaBase::IsSafe(LFO_MODE_INFO& Mode, LFO_PARA_STRUCT& Para, const int devtype)
{
	int nFlag = 0;
	float tmpAmpl = 0.0f;
	if (devtype == MENU_LFO_DEV_GEN)
	{
		tmpAmpl = Para.gn_wn_ampl;
	}
	else if (devtype == MENU_LFO_DEV_LINE)
	{
		tmpAmpl = Para.ln_wn_ampl;
	}
	else if (devtype == MENU_LFO_DEV_TR)
	{
		tmpAmpl = Para.tr_wn_ampl;
	}
	else
	{
		sprintf(m_LogString, "设备类型错误[%d]", devtype);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	// 频率范围在模式保存时已经过滤
	if (Mode.freq > Para.freq_min &&
		Mode.freq < Para.freq_max &&
		Mode.ampl < tmpAmpl &&
		Mode.damp > Para.lfo_damp)
	{
		// 满足安全条件
		nFlag = 1;
	}

	sprintf(m_LogString, "安全判断：频率[%f](%f~%f),幅值[%f](%f),阻尼比[%f](%f),是否安全[%d]",
		Mode.freq, Para.freq_min, Para.freq_max, Mode.ampl, tmpAmpl, Mode.damp, Para.lfo_damp, nFlag);
	MessShow(m_LogString, LTINFO);
	return nFlag;
}

//************************************
// Method:    SearchAsstDeci
// FullName:  CLfoAnaBase::SearchAsstDeci
// Access:    public 
// Returns:   int
// Qualifier:
// TODO:暂不开发
//************************************
int CLfoAnaBase::SearchAsstDeci()
{
	return 0;
}

//************************************
// Method:    JudgeSysLfoStat
// FullName:  CLfoAnaBase::JudgeSysLfoStat
// Access:    public 
// Returns:   int
// Qualifier:
// 判断系统安全状态，若有一个设备振荡，则系统振荡；若全部设备均安全，则系统安全。
// 输入：设备信息结果(vector<LFO_DEV_INFO> m_VecMonDev)，安全状态，更新振荡统计（振荡开始时间，结束时间，持续时间）
// 输出：系统状态结构体(LFO_SYS_INFO)，安全状态，振荡设备个数，主导模式中参与设备个数，振荡设备信息结构（所有振荡设备）vector< LFO_DEV_INFO >，模态分析结构（所有模式汇总）vector<LFO_MODE_SHAPE>
// 振荡开始时，清空振荡事件结构体。
//************************************
int CLfoAnaBase::JudgeSysLfoStat(vector<LFO_DEV_INFO> VecMonDev, vector<LFO_MODE_SHAPE> VecModeShape, LFO_SYS_INFO& MonSys, LFO_SYS_INFO& oscMonSys)
{
	int ii, tmpstat = 0;
	// 保存振荡设备
	MonSys.lfo_gn_num = 0;
	MonSys.lfo_ln_num = 0;
	MonSys.lfo_tr_num = 0;
	MonSys.lfo_inf_num = 0;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].stat >= MENU_LFO_DEV_STS_WRN)
		{
			if (VecMonDev[ii].stat > tmpstat)
			{
				// 查找最大值,若有低频振荡告警,则系统状态置为低频振荡告警
				tmpstat = VecMonDev[ii].stat;
			}
			MonSys.vec_dev.push_back(VecMonDev[ii]);
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				MonSys.lfo_gn_num++;
			}
			else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				MonSys.lfo_ln_num++;
			}
			else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				MonSys.lfo_tr_num++;
			}
			else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
			{
				MonSys.lfo_inf_num++;
			}
		}
	}

	if (!MonSys.vec_dev.empty())
	{
		sort(MonSys.vec_dev.begin(), MonSys.vec_dev.end(), g_SortDevByEngy);
		// 存在振荡设备，置系统状态为告警
		MonSys.stat = tmpstat;
		MonSys.window_id = MonSys.start_time;
		int lead_gen_idx = -1;
		memset(&MonSys.lead_gen, 0, sizeof(LFO_DEV_INFO));
		// 增加设备振荡时间
		for (ii = 0; ii < (int)MonSys.vec_dev.size(); ii++)
		{
			MonSys.vec_dev[ii].start_time = MonSys.start_time;
			MonSys.vec_dev[ii].end_time = MonSys.end_time;
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN && lead_gen_idx == -1)
			{
				lead_gen_idx = ii;
			}
		}
		// 		MonSys.lead_dev = MonSys.vec_dev.front();
		CopyLfoDev(MonSys.lead_dev, MonSys.vec_dev.front());
		if (MonSys.lead_dev.dev.dev_type != MENU_LFO_DEV_GEN && lead_gen_idx != -1)
		{
			CopyLfoDev(MonSys.lead_gen, MonSys.vec_dev[lead_gen_idx]);
		}
		// 		sprintf(m_LogString,"lead_dev:[%s]频率[%f],幅值[%f],阻尼比[%f]",
		// 			MonSys.lead_dev.dev.dev_name,MonSys.lead_dev.mode.freq,MonSys.lead_dev.mode.ampl,MonSys.lead_dev.mode.damp);
		// 		MessShow(m_LogString,LTINFO);
				// 保存本轮模态结果
		if (!VecModeShape.empty())
		{
			// 			MonSys.lead_mode = VecModeShape.front().mode;
			// 			MonSys.lead_dev = VecModeShape.front().vec_dev.front().dev;
			for (ii = 0; ii < (int)VecModeShape.size(); ii++)
			{
				if (fabs(VecModeShape[ii].mode.freq - MonSys.lead_dev.mode.freq) < m_LfoPara.freq_diff)
				{
					VecModeShape[ii].mode.stat = MENU_LFO_DEV_STS_WRN;
				}
			}
			// 			MonSys.vec_modeshape = VecModeShape;
			CopyLfoModeShape(MonSys.vec_modeshape, VecModeShape);
		}
	}
	else
	{
		// 不存在振荡设备，系统安全
		MonSys.stat = MENU_LFO_DEV_STS_SCT;
		MonSys.window_id = MonSys.start_time;
		sprintf(m_LogString, "不存在振荡设备，系统安全!");
		MessShow(m_LogString, LTINFO);
		if (!VecModeShape.empty())
		{
			// 			MonSys.vec_modeshape = VecModeShape;
			CopyLfoModeShape(MonSys.vec_modeshape, VecModeShape);
			// 			// 			MonSys.lead_mode = VecModeShape.front().mode;
			// 			// 			MonSys.lead_dev = VecModeShape.front().vec_dev.front().dev;
			// 			for (ii =0; ii<(int)VecModeShape.size(); ii++)
			// 			{
			// 				if (fabs(VecModeShape[ii].mode.freq - MonSys.lead_dev.mode.freq) <  m_LfoPara.freq_diff )
			// 				{
			// 					VecModeShape[ii].mode.stat = MENU_LFO_DEV_STS_WRN;
			// 				}
			// 			}
		}

	}
	sprintf(m_LogString, "系统状态[%d],振荡设备个数[%d],机组[%d],线路[%d],变压器[%d],断面[%d],模式个数[%d]", MonSys.stat, (int)MonSys.vec_dev.size(), MonSys.lfo_gn_num, MonSys.lfo_ln_num, MonSys.lfo_tr_num, MonSys.lfo_inf_num, MonSys.vec_modeshape.size());
	MessShow(m_LogString, LTINFO);

	// 	// 保存小扰动统计设备
	// 	tmpstat = 0;
	// 	for ( ii =0; ii<(int)VecMonDev.size(); ii++ )
	// 	{
	// 		if (VecMonDev[ii].osc_stat >= MENU_LFO_DEV_STS_WRN)
	// 		{
	// 			if (VecMonDev[ii].osc_stat > tmpstat)
	// 			{
	// 				// 查找最大值,若有小扰动统计告警,则系统状态置为小扰动统计告警
	// 				tmpstat = VecMonDev[ii].osc_stat;
	// 			}
	// 			oscMonSys.vec_dev.push_back(VecMonDev[ii]);
	// 		}
	// 		
	// 	}
	// 	
	// 	if (!oscMonSys.vec_dev.empty())
	// 	{
	// 		sort(oscMonSys.vec_dev.begin(),oscMonSys.vec_dev.end(),g_SortDevByEngy);
	// 		// 存在小扰动统计设备，置系统状态为告警
	// 		oscMonSys.stat = tmpstat;
	// 		oscMonSys.window_id = oscMonSys.start_time;
	// 	sprintf(m_LogString,"本轮小扰动分析开始时间["RTDB_TIME_FMT"],结束时间["RTDB_TIME_FMT"]",oscMonSys.start_time,oscMonSys.end_time);
	// 	MessShow(m_LogString,LTINFO);
	// 		
	// 		// 增加设备小扰动统计时间
	// 		for (ii =0; ii<(int)oscMonSys.vec_dev.size(); ii++)
	// 		{
	// 			oscMonSys.vec_dev[ii].start_time = oscMonSys.start_time;
	// 			oscMonSys.vec_dev[ii].end_time = oscMonSys.end_time;
	// 		}
	// 		oscMonSys.lead_dev = oscMonSys.vec_dev.front();
	// 		// 保存本轮模态结果
	// 		if (!VecModeShape.empty())
	// 		{
	// 			oscMonSys.vec_modeshape = VecModeShape;
	// 			// 			MonSys.lead_mode = VecModeShape.front().mode;
	// 			// 			MonSys.lead_dev = VecModeShape.front().vec_dev.front().dev;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		// 不存在振荡设备，系统安全
	// 		oscMonSys.stat = MENU_LFO_DEV_STS_SCT;
	// 		oscMonSys.window_id = MonSys.start_time;
	// 		sprintf(m_LogString,"不存在小扰动统计设备，系统安全!");
	// 		MessShow(m_LogString,LTINFO);
	// 	}
	// 	sprintf(m_LogString,"小扰动统计系统状态[%d],小扰动设备个数[%d]",oscMonSys.stat,(int)oscMonSys.vec_dev.size());
	// 	MessShow(m_LogString,LTINFO);
	return 0;
}
#ifndef _LFO_HIS_LIB_
int CLfoAnaBase::ExcuteFileUpnLoad(const char* locdir, string vec_files, const char* srvdir)
{
	int ret = 0;
/* lns_20210811

#ifdef _D5000
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
	char logstr[MAXBUF * 2] = { 0 };
	char srvhost[64] = { 0 };
	if (g_sop_fileop.GetFileHost(srvhost) == false)
	{
		sprintf(logstr, "上传SOP流程文件:获取文件服务主机名[%s]失败", srvhost);
		MessShow(logstr, LTINFO);
		//setSopError(logstr);
		return -1;
	}
	else
	{
		sprintf(logstr, "上传SOP流程文件:获取文件服务主机名[%s]成功", srvhost);
		MessShow(logstr, LTINFO);
	}

	int filesize = 0;
	char locfile[512] = { 0 };
	char srvfile[512] = { 0 };
	//    for(int i=0; i<(int)vec_files.size(); i++)
	//     for(int i=0; i<1; i++)
	//     {
	// 		sprintf(locfile, "%s%s%s",locdir,m_backslash,vec_files[i].c_str() );
	//      sprintf(srvfile, "%s%s%s",srvdir,m_backslash,vec_files[i].c_str() );
	sprintf(locfile, "%s%s%s", locdir, m_backslash, vec_files.c_str());
	sprintf(srvfile, "%s%s%s", srvdir, m_backslash, vec_files.c_str());

	if (g_sop_fileop.Upload(srvhost, srvfile, locfile, filesize) < 0)
	{
		ret--;
		sprintf(logstr, "上传SOP流程文件失败:[%s]=>[%s][%s]", locfile, srvhost, srvfile);
		MessShow(logstr, LTINFO);
		//setSopError(logstr);
	}
	else
	{
		sprintf(logstr, "上传SOP流程文件成功:[%s]=>[%s][%s]", locfile, srvhost, srvfile);
		MessShow(logstr, LTINFO);
	}
	//    }
#endif
*/
	return ret;

}
#endif

#ifndef _LFO_HIS_LIB_
int CLfoAnaBase::WritePmuSqlData(LPO_DEV_INFO VecLPODEVINFO) //added 20141212
{
	int db_ret = -1;
/* lns_20210811

#ifdef _D5000
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
	char sql_update[MAXBUF * 8];
	memset(sql_update, 0, MAXBUF * 8);
	char tm_bg[32];
	memset(sql_update, 0, 32);
	char tm_end[32];
	memset(sql_update, 0, 32);
	char m_Logstr[MAXBUF * 8];
	memset(m_Logstr, 0, MAXBUF * 8);
	sprintf(tm_bg, PT_GetTimeStringBySeconds(VecLPODEVINFO.slfo_bgn_t));
	sprintf(tm_end, PT_GetTimeStringBySeconds(VecLPODEVINFO.slfo_end_t));
	sprintf(sql_update, "insert into dsa_wams_powosc_dev(%s) values ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",'%s','%s','%s','%s','%s',"INT_REC_FMT",%d,%d,%f,%f,%f,%f,%f,%f,%f,%d,TO_DATE('%s','YYYY-MM-DD HH24:MI:SS'),TO_DATE('%s','YYYY-MM-DD HH24:MI:SS'),'%s','%s')",
		LPODEVINFOINSERT, INT_REC_VAL(VecLPODEVINFO.dev_id), INT_REC_VAL(VecLPODEVINFO.fac_id_1), INT_REC_VAL(VecLPODEVINFO.area_id_1),
		INT_REC_VAL(VecLPODEVINFO.fac_id_2), INT_REC_VAL(VecLPODEVINFO.area_id_2), VecLPODEVINFO.dev_name, VecLPODEVINFO.fac_name_1, VecLPODEVINFO.fac_name_2,
		VecLPODEVINFO.area_name_1, VecLPODEVINFO.area_name_2, INT_REC_VAL(VecLPODEVINFO.dev_type), VecLPODEVINFO.is_monitor, VecLPODEVINFO.is_part_ana,
		VecLPODEVINFO.lead_ampl, VecLPODEVINFO.lead_freq, VecLPODEVINFO.lead_damp, VecLPODEVINFO.lead_phas, VecLPODEVINFO.lead_real,
		VecLPODEVINFO.lead_imag, VecLPODEVINFO.lead_engy, VecLPODEVINFO.if_lfo, tm_bg, tm_end,
		VecLPODEVINFO.slfo_time, VecLPODEVINFO.file_dir);

	sprintf(m_Logstr, "写记录[%s]", sql_update);
	MessShow(m_Logstr, LTINFO);
	HisdbAccess hisdb_access(D5000_HISDB_PORT);
	db_ret = hisdb_access.ExecHisBySql(sql_update);
	if (db_ret < 0)
	{
		sprintf(m_LogString, "写记录失败[%d]", db_ret);
		MessShow(m_LogString, LTINFO);
	}
	else
	{
		sprintf(m_LogString, "写记录成功[%d]", db_ret);
		MessShow(m_LogString, LTINFO);
	}
#endif

*/  //lns_20210811
	return db_ret;
}
#endif

int CLfoAnaBase::WritePmuData(const char* mainstaresultfile, tab_lfo_pmu_data& mm_PMUData)
{
/* lns_20210811
	int i, j;
	FILE* fp;
	char begin_time[32];
	char end_time[32];

	if ((fp = fopen(mainstaresultfile, "w+")) == NULL)
	{
		sprintf(m_LogString, "open file[%s] error!", mainstaresultfile);
		MessShow(m_LogString, LTINFO);
		fclose(fp);
		return -1;
	}

	sprintf(begin_time, PT_GetTimeStringBySeconds(mm_PMUData.head.begin_time));
	sprintf(end_time, PT_GetTimeStringBySeconds(mm_PMUData.head.end_time));
	fprintf(fp, "%s,%s", begin_time, end_time);
	for (i = 0; i < mm_PMUData.head.vec_eleid.size(); i++)
	{
		fprintf(fp, ","INT_REC_FMT, INT_REC_VAL(mm_PMUData.head.vec_eleid[i]));
	}
	fprintf(fp, "\n");

	fprintf(fp, "%s", "日期时间,毫秒");
	for (i = 0; i < mm_PMUData.head.vec_elename.size(); i++)
	{
		fprintf(fp, ",%s", mm_PMUData.head.vec_elename[i].c_str());
	}
	fprintf(fp, "\n");

	for (i = 0; i < mm_PMUData.vec_row.size(); i++)
	{
		sprintf(begin_time, PT_GetTimeStringBySeconds(mm_PMUData.vec_row[i].begin_time));
		fprintf(fp, "%s,%d", begin_time, mm_PMUData.vec_row[i].end_time);
		for (j = 0; j < mm_PMUData.vec_row[i].vec_value.size(); j++)
		{
			fprintf(fp, ",%f", mm_PMUData.vec_row[i].vec_value[j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
*/  //lns_20210811

	return 0;
}

int CLfoAnaBase::UpdateStatOld(vector<LFO_DEV_INFO>& VecMonDev, LFO_SYS_INFO& MonSys, LFO_SYS_INFO& oscMonSys)
{
	int ii;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		VecMonDev[ii].stat_old = VecMonDev[ii].stat;
		// 		VecMonDev[ii].osc_stat_old = VecMonDev[ii].osc_stat;
	}
	MonSys.gps_time_old = MonSys.gps_time;
	MonSys.stat_old = MonSys.stat;

	oscMonSys.gps_time_old = oscMonSys.gps_time;
	oscMonSys.stat_old = oscMonSys.stat;
	return 0;
}

int CLfoAnaBase::GetLfoDevStat(LFO_DEV_STAT& tmpDevStat)
{
	// 判断是否满足条件
//	sprintf(m_LogString,"[yjj]判断设备状态[%d],频率%f[%f,%f],振幅%f[%f],阻尼比%f[%f]",tmpDevStat.warn_type,tmpDevStat.freq,tmpDevStat.freq_min,tmpDevStat.freq_max,tmpDevStat.ampl,tmpDevStat.ampl_thre,tmpDevStat.damp,tmpDevStat.damp_thre);
//	MessShow(m_LogString,LTINFO);
	sprintf(m_LogString, "主导模式振幅=[%6.2f],占比[%6.2f],平均振幅[%6.2f]", tmpDevStat.ampl, m_LfoPara.para_Ampl_aver, tmpDevStat.aver_ampl);
	MessShow(m_LogString);

	int nFlag = 0;
	if (tmpDevStat.warn_type == 1)
	{
		// 是否满足告警条件
		if (tmpDevStat.ampl < m_LfoPara.para_Ampl_aver * tmpDevStat.aver_ampl)
		{
			sprintf(m_LogString, "主导模式振幅=[%6.2f]<占比[%6.2f]*平均振幅[%6.2f]", tmpDevStat.ampl, m_LfoPara.para_Ampl_aver, tmpDevStat.aver_ampl);
			MessShow(m_LogString);
		}
		else
		{
			if (tmpDevStat.freq >= tmpDevStat.freq_min &&
				tmpDevStat.freq <= tmpDevStat.freq_max &&
				tmpDevStat.ampl > tmpDevStat.ampl_thre &&
				tmpDevStat.damp < tmpDevStat.damp_thre)
			{
				nFlag = 1;   //不安全
			}
			else
			{
				nFlag = 0;   //安全
			}
			if (1 == m_LfoPara.if_flit_lfo)
			{
				if (tmpDevStat.freq >= tmpDevStat.lfofreq_min &&
					tmpDevStat.freq <= tmpDevStat.lfofreq_max &&
					tmpDevStat.ampl > tmpDevStat.ampl_thre &&
					tmpDevStat.damp < tmpDevStat.lfodamp_thre)
				{
					nFlag = 0;
					sprintf(m_LogString, "属于低频振荡,功率波动排除低频振荡时该设备置为安全状态");
					MessShow(m_LogString, LTINFO);
				}
			}
		}
	}
	else
	{
		// 是否满足安全条件
// 		if (tmpDevStat.freq >= tmpDevStat.freq_min &&
// 			tmpDevStat.freq <= tmpDevStat.freq_max &&
// 			tmpDevStat.ampl <= tmpDevStat.ampl_thre &&
// 			tmpDevStat.damp >= tmpDevStat.damp_thre)
		if (tmpDevStat.ampl <= tmpDevStat.ampl_thre)
		{
			nFlag = 1;  //安全
		}
		else
		{
			nFlag = 0;  //不安全
		}

		if (1 == m_LfoPara.if_flit_lfo)
		{
			if (tmpDevStat.freq >= tmpDevStat.lfofreq_min &&
				tmpDevStat.freq <= tmpDevStat.lfofreq_max &&
				tmpDevStat.ampl > tmpDevStat.ampl_thre &&
				tmpDevStat.damp < tmpDevStat.lfodamp_thre)
			{
				nFlag = 1;
				sprintf(m_LogString, "属于低频振荡,功率波动排除低频振荡时该设备置为安全状态");
				MessShow(m_LogString, LTINFO);
			}
		}
		if (tmpDevStat.ampl < m_LfoPara.para_Ampl_aver * tmpDevStat.aver_ampl)
		{
			nFlag = 1;
			sprintf(m_LogString, "主导模式振幅=[%6.2f]<占比[%6.2f]*平均振幅[%6.2f],该设备置为安全状态", tmpDevStat.ampl, m_LfoPara.para_Ampl_aver, tmpDevStat.aver_ampl);
			MessShow(m_LogString);
		}
	}

	sprintf(m_LogString, "nFlag=[%d],tmpDevStat.start_time=[%d]", nFlag, tmpDevStat.start_time);
	MessShow(m_LogString);
	if (nFlag == 1)
	{
		// 更新振荡持续时间
		if (tmpDevStat.start_time <= 0)
		{
			// 初次满足振荡条件
			tmpDevStat.start_time = tmpDevStat.ana_start_time;
			tmpDevStat.end_time = tmpDevStat.ana_end_time;
		}
		else
		{
			// 非初次满足振荡条件，更新振荡时间
			if (tmpDevStat.ana_start_time < tmpDevStat.start_time)
			{
				tmpDevStat.start_time = tmpDevStat.ana_start_time;
			}
			if (tmpDevStat.ana_end_time > tmpDevStat.end_time)
			{
				tmpDevStat.end_time = tmpDevStat.ana_end_time;
			}
		}
		// 计算持续时间
		tmpDevStat.hold_time = tmpDevStat.end_time - tmpDevStat.start_time;

		if ((tmpDevStat.hold_time >= tmpDevStat.time_max) || ((float)fabs(tmpDevStat.freq) <= 0.001))
		{
			// 置满足条件标志
			tmpDevStat.stat = 1;
			// 清空安全时间 ??
			tmpDevStat.start_time = 0;
			tmpDevStat.end_time = 0;
			tmpDevStat.hold_time = 0;
		}
		else
		{
			// 不满足条件,本轮时间累加不清空
			tmpDevStat.stat = 0;
		}
	}
	else
	{
		// 不满足条件状态持续，时间清零
		tmpDevStat.start_time = 0;
		tmpDevStat.end_time = 0;
		tmpDevStat.hold_time = 0;
		// 置不满足条件标志
		tmpDevStat.stat = 0;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
int CLfoAnaBase::ReadLfoPara(vector<LFO_PARA_TABLE_READ>& VecLfoPara)
{
	if (!VecLfoPara.empty())
	{
		vector<LFO_PARA_TABLE_READ>().swap(VecLfoPara);
		VecLfoPara.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	sprintf(m_LogString, "LFO参数表的表号[%d]", LFO_PARA_NO_TAB);
	MessShow(m_LogString, LTINFO);
	if (PODB_TableGet(LFO_PARA_NO_TAB, LFO_PARA_TABLE_READ_FLD, buf_base) < 0)
	{
		return -1;
	}
	int nInfo = buf_base.GetLength() / sizeof(LFO_PARA_TABLE_READ);

	// 	sprintf(m_LogString,"LFO参数表记录大小[%d][%d]",buf_base.GetLength(),sizeof(LFO_PARA_TABLE_READ));
	// 	MessShow(m_LogString,LTERROR);


	if (nInfo <= 0)
	{
		sprintf(m_LogString, "LFO参数表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "LFO参数表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}
	LFO_PARA_TABLE_READ* pInfo = NULL;
	pInfo = new LFO_PARA_TABLE_READ[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(LFO_PARA_TABLE_READ));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecLfoPara.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}


int CLfoAnaBase::GetLfoFac(vector<LFO_FC_TAB_CRE>& VecLfoFacInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(LFO_FAC_NO_TAB, LFO_FC_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(LFO_FC_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "LFO厂站表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "LFO厂站表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	LFO_FC_TAB_CRE* pInfo = NULL;
	pInfo = new LFO_FC_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(LFO_FC_TAB_CRE));

	int ii;
	for (ii = 0; ii < nInfo; ii++)
	{
		VecLfoFacInfo.push_back(pInfo[ii]);
	}
	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaBase::GetLfoGen(vector<LFO_GN_TAB_CRE>& VecLfoGenInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(LFO_GN_NO_TAB, LFO_GN_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(LFO_GN_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "LFO发电机表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "LFO发电机表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	LFO_GN_TAB_CRE* pInfo = NULL;
	pInfo = new LFO_GN_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(LFO_GN_TAB_CRE));

	int ii;
	for (ii = 0; ii < nInfo; ii++)
	{
		VecLfoGenInfo.push_back(pInfo[ii]);
	}
	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaBase::GetLfoLine(vector<LFO_LN_TAB_CRE>& VecLfoLineInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(LFO_ACLN_NO_TAB, LFO_LN_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(LFO_LN_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "LFO交流线段表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "LFO交流线段表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	LFO_LN_TAB_CRE* pInfo = NULL;
	pInfo = new LFO_LN_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(LFO_LN_TAB_CRE));

	int ii;
	for (ii = 0; ii < nInfo; ii++)
	{
		VecLfoLineInfo.push_back(pInfo[ii]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaBase::GetLfoTr(vector<LFO_TR_TAB_CRE>& VecLfoTrInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(LFO_TR_NO_TAB, LFO_TR_TAB_CRE_FLD, buf_base) < 0)
	{
		sprintf(m_LogString, "读取LFO变压器表失败");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(LFO_TR_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "LFO变压器表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "LFO变压器表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	LFO_TR_TAB_CRE* pInfo = NULL;
	pInfo = new LFO_TR_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(LFO_TR_TAB_CRE));

	int ii;
	for (ii = 0; ii < nInfo; ii++)
	{
		VecLfoTrInfo.push_back(pInfo[ii]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaBase::GetLfoIntface(vector<LFO_INT_TAB_CRE>& VecLfoIntInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(LFO_INTFACE_NO_TAB, LFO_INT_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(LFO_INT_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "LFO联络线走廊表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "LFO联络线走廊表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	LFO_INT_TAB_CRE* pInfo = NULL;
	pInfo = new LFO_INT_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(LFO_INT_TAB_CRE));

	int ii;
	for (ii = 0; ii < nInfo; ii++)
	{
		VecLfoIntInfo.push_back(pInfo[ii]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaBase::GetLfoIntline(vector<LFO_INTLINE_TAB_CRE>& VecLfoIntlineInfo)
{
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(LFO_INTLINE_NO_TAB, LFO_INTLINE_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(LFO_INTLINE_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "LFO联络线走廊与支路表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "LFO联络线走廊与支路表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	LFO_INTLINE_TAB_CRE* pInfo = NULL;
	pInfo = new LFO_INTLINE_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(LFO_INTLINE_TAB_CRE));

	int ii;
	for (ii = 0; ii < nInfo; ii++)
	{
		VecLfoIntlineInfo.push_back(pInfo[ii]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CLfoAnaBase::GetDevDataFromDB(RTDB_TIME StartTime, RTDB_TIME EndTime, int ordernum, LFO_DEV_INFO& MonDev)
{
	int iikk, nRealNum;
	int temp_nYcFilNo, temp_nYcColNo;
	int nNeedNum;
	nNeedNum = (EndTime - StartTime) * MAX_FRA_PER_SEC;
	if (nNeedNum <= 0)
	{
		sprintf(m_LogString, "时间段错误["RTDB_TIME_FMT","RTDB_TIME_FMT",%d]need[%d]", StartTime, EndTime, MAX_FRA_PER_SEC, nNeedNum);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	if (MonDev.stat != MENU_LFO_DEV_STS_OK)
	{
		return 0;
	}

	PUB_WT_VEC_DATA* pData = new PUB_WT_VEC_DATA[nNeedNum];
	PMU_DATA_DESC tmpDataDesc;
	WT_VEC_DESC    desc;
	nRealNum = 0;
	memset(pData, 0, nNeedNum * sizeof(PUB_WT_VEC_DATA));
	temp_nYcFilNo = MonDev.dev.nYcFilNo[ordernum];
	temp_nYcColNo = MonDev.dev.nYcColNo[ordernum];
#ifndef _D5000
	// 	if (PWD_TableGet(temp_nYcFilNo,temp_nYcColNo,StartTime,EndTime,pData,nRealNum,desc) == false )	
	// #else
	// 	sprintf(m_LogString,"【yjj】000000000000000");
	// 	MessShow(m_LogString,LTERROR);
	if (PWD_TableGet(temp_nYcFilNo, temp_nYcColNo, StartTime, EndTime, pData, nRealNum) == false)
#else
	// 	sprintf(m_LogString,"【yjj】111111111111");
	// 	MessShow(m_LogString,LTERROR);
	if (PWD_TableGet(temp_nYcColNo, StartTime, EndTime, pData, nRealNum) == false)
#endif

	{
		sprintf(m_LogString, "获取数据失败,dev["INT_REC_FMT",%s],YcNo[%d,%d],Time["RTDB_TIME_FMT","RTDB_TIME_FMT"]", INT_REC_VAL(MonDev.dev.dev_id), MonDev.dev.dev_name,
			MonDev.dev.nYcFilNo[ordernum], MonDev.dev.nYcColNo[ordernum], StartTime, EndTime);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
		SAFE_DELETE_ARRAY(pData);
		return 0;
	}
	/*printf("%d  <----->  %d",nNeedNum,nRealNum);
	FILE* fp;
	char tmpFileName[128];
	sprintf(tmpFileName,"%s","../data/wam_poa/res/pmu_data_tmp.csv");
	if((fp=fopen(tmpFileName,"a+")) == NULL)
	{
		printf("文件[%s]打开失败",tmpFileName);
		return -1;
	}

	fprintf(fp,"序号,日期,测点,值,状态");
	for (int i=0;i<nRealNum;i++)
	{
		fprintf(fp,"%d,%d.%06d,%d,%.2f,%s\n",i,pData[i].time.secs,pData[i].time.usecs,temp_nYcColNo,pData[i].value,pData[i].status);
	}
	fclose(fp);*/

	if (nRealNum != nNeedNum)
	{
		sprintf(m_LogString, "获取数据个数错误,dev["INT_REC_FMT",%s],YcNo[%d,%d],Time["RTDB_TIME_FMT","RTDB_TIME_FMT"],Need[%d],Real[%d]", INT_REC_VAL(MonDev.dev.dev_id), MonDev.dev.dev_name,
			MonDev.dev.nYcFilNo[ordernum], MonDev.dev.nYcColNo[ordernum], StartTime, EndTime, nNeedNum, nRealNum);
		MessShow(m_LogString, LTDEBUG);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
		SAFE_DELETE_ARRAY(pData);
		return 0;
	}
	// 检查数据状态
	memset((void*)&tmpDataDesc, 0, sizeof(PMU_DATA_DESC));
	if (PWD_PmuDataCheck(pData, nRealNum, tmpDataDesc) == false)
	{
		sprintf(m_LogString, "数据状态错误,dev["INT_REC_FMT",%s],YcNo[%d,%d],Time["RTDB_TIME_FMT","RTDB_TIME_FMT"]", INT_REC_VAL(MonDev.dev.dev_id), MonDev.dev.dev_name,
			MonDev.dev.nYcFilNo[ordernum], MonDev.dev.nYcColNo[ordernum], StartTime, EndTime);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
		SAFE_DELETE_ARRAY(pData);
		return 0;
	}

	// 判断数据状态
	if (tmpDataDesc.nPmuOff > 0)
	{
		sprintf(m_LogString, "[%s]数据状态异常提示:通道退出!", MonDev.dev.dev_name);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_OFF;
	}
	else if (tmpDataDesc.nDataBad > 0)
	{
		sprintf(m_LogString, "[%s]数据状态异常提示:量测不可用1!", MonDev.dev.dev_name);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
	}
	else if (tmpDataDesc.nPmuBad > 0)
	{
		sprintf(m_LogString, "[%s]数据状态异常提示:量测不可用2!", MonDev.dev.dev_name);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
	}
	else if (tmpDataDesc.nGpsLost > 0)
	{
		sprintf(m_LogString, "[%s]数据状态异常提示:量测不可用3!", MonDev.dev.dev_name);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
	}
	else if (tmpDataDesc.nDataLost > 0)
	{
		sprintf(m_LogString, "[%s]数据状态异常提示:量测不可用4!", MonDev.dev.dev_name);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
	}
	else if (tmpDataDesc.nDataDead > 0)
	{
		sprintf(m_LogString, "[%s]数据状态异常提示:量测不可用5!", MonDev.dev.dev_name);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
	}
	else if (tmpDataDesc.nTimeDiff > 0)
	{
		sprintf(m_LogString, "[%s]数据状态异常提示:量测不可用6!", MonDev.dev.dev_name);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
	}
	else if (tmpDataDesc.nDynSimu > 0)
	{
		sprintf(m_LogString, "[%s]数据状态异常提示:量测不可用3!", MonDev.dev.dev_name);
		MessShow(m_LogString, LTERROR);
		MonDev.stat = MENU_LFO_DEV_STS_BAD;
	}

	// 正常数据赋值
	if (ordernum == 2 || ordernum == 0)
	{
		if (MonDev.stat == MENU_LFO_DEV_STS_OK)
		{
			for (iikk = 0; iikk < nRealNum; iikk++)
			{
				MonDev.v2d_data.push_back((float)pData[iikk].value);
			}
		}
		else
		{
			float temp_value = 999999.0;
			for (iikk = 0; iikk < nRealNum; iikk++)
			{
				MonDev.v2d_data.push_back((float)temp_value);
			}
			sprintf(m_LogString, "[提示:][%s]异常数据赋值!", MonDev.dev.dev_name);
			MessShow(m_LogString, LTERROR);
		}
	}
	else if (ordernum == 1)
	{
		if (MonDev.stat == MENU_LFO_DEV_STS_OK)
		{
			for (iikk = 0; iikk < nRealNum; iikk++)
			{
				MonDev.ana_data.push_back((float)pData[iikk].value);
			}
		}
		else
		{
			float temp_value = 999999.0;
			for (iikk = 0; iikk < nRealNum; iikk++)
			{
				MonDev.ana_data.push_back((float)temp_value);
			}
			sprintf(m_LogString, "[提示:][%s]异常数据赋值!", MonDev.dev.dev_name);
			MessShow(m_LogString, LTERROR);
		}
	}


	SAFE_DELETE_ARRAY(pData);
	return 0;
}


int CLfoAnaBase::ClearLfoIntline(vector<LFO_INTLINE_INFO>& tmpVecIntline)
{
	if (!tmpVecIntline.empty())
	{
		for (int ii = 0; ii < tmpVecIntline.size(); ii++)
		{
			ClearLfoIntline(tmpVecIntline[ii]);
		}
		tmpVecIntline.clear();
		vector<LFO_INTLINE_INFO>().swap(tmpVecIntline);
	}
	return 0;
}

int CLfoAnaBase::ClearLfoIntline(LFO_INTLINE_INFO& tmpIntline)
{
	ZERO_INT_REC(tmpIntline.inf_no);
	ZERO_INT_REC(tmpIntline.recordid);
	ZERO_INT_REC(tmpIntline.line_no);
	ZERO_INT_REC(tmpIntline.stat_no);
	tmpIntline.eletype = 0;
	tmpIntline.onoff = 0;
	tmpIntline.mpoint = 0;
	tmpIntline.ampl_limit_s = 0;
	tmpIntline.ampl_limit_e = 0;
	memset(&tmpIntline.inf_name, 0, sizeof(tmpIntline.inf_name));
	memset(&tmpIntline.line_name, 0, sizeof(tmpIntline.line_name));

	memset(&tmpIntline.curve, 0, sizeof(LFO_CURVE_INFO));
	ClearLfoDev(tmpIntline.dev_info);
	ClearLfoModeInfo(tmpIntline.mode);
	ClearLfoModeInfo(tmpIntline.mode_shape);
	if (!tmpIntline.vec_mode.empty())
	{
		ClearLfoModeInfo(tmpIntline.vec_mode);
	}
	if (!tmpIntline.vec_lead_mode.empty())
	{
		ClearLfoModeInfo(tmpIntline.vec_lead_mode);
	}
	return 0;
}


int CLfoAnaBase::CopyLfoIntline(LFO_INTLINE_INFO& ObjLfoIntline, LFO_INTLINE_INFO& SrcLfoIntline)
{
	ClearLfoIntline(ObjLfoIntline);
	ObjLfoIntline.recordid = SrcLfoIntline.recordid;
	ObjLfoIntline.inf_no = SrcLfoIntline.inf_no;
	ObjLfoIntline.line_no = SrcLfoIntline.line_no;
	ObjLfoIntline.eletype = SrcLfoIntline.eletype;
	ObjLfoIntline.onoff = SrcLfoIntline.onoff;
	ObjLfoIntline.mpoint = SrcLfoIntline.mpoint;
	ObjLfoIntline.stat_no = SrcLfoIntline.stat_no;
	ObjLfoIntline.ampl_limit_s = SrcLfoIntline.ampl_limit_s;
	ObjLfoIntline.ampl_limit_e = SrcLfoIntline.ampl_limit_e;
	sprintf(ObjLfoIntline.inf_name, "%s", SrcLfoIntline.inf_name);
	sprintf(ObjLfoIntline.line_name, "%s", SrcLfoIntline.line_name);

	memcpy(&ObjLfoIntline.curve, &SrcLfoIntline.curve, sizeof(LFO_CURVE_INFO));
	memcpy(&ObjLfoIntline.mode, &SrcLfoIntline.mode, sizeof(LFO_MODE_INFO));
	memcpy(&ObjLfoIntline.mode_shape, &SrcLfoIntline.mode_shape, sizeof(LFO_MODE_INFO));

	CopyLfoDev(ObjLfoIntline.dev_info, SrcLfoIntline.dev_info);

}

int CLfoAnaBase::CopyLfoIntline(vector<LFO_INTLINE_INFO>& ObjVecIntline, vector<LFO_INTLINE_INFO>& SrcVecIntline)
{
	if (!SrcVecIntline.empty())
	{
		LFO_INTLINE_INFO tmpIntlineInfo;
		ClearLfoIntline(ObjVecIntline);
		for (int ii = 0; ii < SrcVecIntline.size(); ii++)
		{
			ClearLfoIntline(tmpIntlineInfo);
			CopyLfoIntline(tmpIntlineInfo, SrcVecIntline[ii]);
			ObjVecIntline.push_back(tmpIntlineInfo);
		}
	}
	return 0;
}

int CLfoAnaBase::CopyLfoDev(vector<LFO_DEV_INFO>& ObjVecDev, vector<LFO_DEV_INFO>& SrcVecDev)
{
	if (!SrcVecDev.empty())
	{
		LFO_DEV_INFO tmpDevInfo;
		ClearLfoDev(ObjVecDev);
		for (int ii = 0; ii < SrcVecDev.size(); ii++)
		{
			ClearLfoDev(tmpDevInfo);
			CopyLfoDev(tmpDevInfo, SrcVecDev[ii]);
			ObjVecDev.push_back(tmpDevInfo);
		}
	}
	return 0;
}

int CLfoAnaBase::CopyLfoDev(LFO_DEV_INFO& ObjLfoDev, LFO_DEV_INFO& SrcLfoDev)
{
	int ii;
	LFO_MODE_INFO tmpModeInfo;
	ClearLfoDev(ObjLfoDev);

	ObjLfoDev.stat = SrcLfoDev.stat;
	ObjLfoDev.stat_old = SrcLfoDev.stat_old;
	ObjLfoDev.save_flag = SrcLfoDev.save_flag;
	ObjLfoDev.stat_prony = SrcLfoDev.stat_prony;
	ObjLfoDev.stat_source = SrcLfoDev.stat_source;
	ObjLfoDev.start_time = SrcLfoDev.start_time;
	ObjLfoDev.end_time = SrcLfoDev.end_time;
	ObjLfoDev.hold_time = SrcLfoDev.hold_time;
	ObjLfoDev.sourceflag = SrcLfoDev.sourceflag;
	ObjLfoDev.judge_num = SrcLfoDev.judge_num;
	ObjLfoDev.lfo_is_cntr = SrcLfoDev.lfo_is_cntr;
	sprintf(ObjLfoDev.intline_des, "%s", SrcLfoDev.intline_des);

	memcpy(&ObjLfoDev.dev, &SrcLfoDev.dev, sizeof(LFO_DEV_BASE));
	memcpy(&ObjLfoDev.dev_info, &SrcLfoDev.dev_info, sizeof(WT_DEV_INFO));
	memcpy(&ObjLfoDev.curve, &SrcLfoDev.curve, sizeof(LFO_CURVE_INFO));
	memcpy(&ObjLfoDev.curve_ana, &SrcLfoDev.curve_ana, sizeof(LFO_CURVE_INFO));
	memcpy(&ObjLfoDev.mode, &SrcLfoDev.mode, sizeof(LFO_MODE_INFO));
	memcpy(&ObjLfoDev.mode_shape, &SrcLfoDev.mode_shape, sizeof(LFO_MODE_INFO));
	for (ii = 0; ii < SrcLfoDev.vec_mode.size(); ii++)
	{
		ClearLfoModeInfo(tmpModeInfo);
		// 		memcpy(&tmpModeInfo,&SrcLfoDev.vec_mode[ii],sizeof(LFO_MODE_INFO));
		ObjLfoDev.vec_mode.push_back(SrcLfoDev.vec_mode[ii]);
	}

	for (ii = 0; ii < SrcLfoDev.vec_lead_mode.size(); ii++)
	{
		ClearLfoModeInfo(tmpModeInfo);
		// 		memcpy(&tmpModeInfo,&SrcLfoDev.vec_lead_mode[ii],sizeof(LFO_MODE_INFO));
		ObjLfoDev.vec_lead_mode.push_back(SrcLfoDev.vec_lead_mode[ii]);
	}
	// 	sprintf(m_LogString,"[%s]v2d_data.capacity()=%d,ana_data.capacity()=%d,vec_mode.capacity()=%d",ObjLfoDev.dev.dev_name,ObjLfoDev.v2d_data.capacity(),ObjLfoDev.v2d_data.capacity(),ObjLfoDev.vec_mode.capacity());
	// 	MessShow(m_LogString,LTERROR);

	return 0;
}

int CLfoAnaBase::ClearLfoDev(vector<LFO_DEV_INFO>& tmpVecDev)
{
	if (!tmpVecDev.empty())
	{
		// 		sprintf(m_LogString,"进入ClearLfoDev,大小【%d】",(int)tmpVecDev.size());
		// 		MessShow(m_LogString,LTERROR);
		for (int ii = 0; ii < (int)tmpVecDev.size(); ii++)
		{
			ClearLfoDev(tmpVecDev[ii]);
		}
		tmpVecDev.clear();
		vector<LFO_DEV_INFO>().swap(tmpVecDev);
	}
	return 0;
}

int CLfoAnaBase::ClearLfoDev(LFO_DEV_INFO& tmpLfoDev)
{
	memset((void*)&tmpLfoDev.dev, 0, sizeof(LFO_DEV_BASE));
	memset((void*)&tmpLfoDev.dev_info, 0, sizeof(WT_DEV_INFO));
	memset((void*)&tmpLfoDev.osciFlag, 0, sizeof(OSCILLAT_FLAG));

	// 	sprintf(m_LogString,"v2d_data=[%d]",tmpLfoDev.v2d_data.size());
	// 	MessShow(m_LogString);
	if (!tmpLfoDev.v2d_data.empty())
	{
		// 		vector<float>().swap(tmpLfoDev.v2d_data);
		tmpLfoDev.v2d_data.clear();
		vector<float>().swap(tmpLfoDev.v2d_data);
	}

	if (!tmpLfoDev.ana_data.empty())
	{
		// 		vector<float>().swap(tmpLfoDev.ana_data);
		tmpLfoDev.ana_data.clear();
		vector<float>().swap(tmpLfoDev.ana_data);
	}

	if (!tmpLfoDev.back_data.empty())
	{
		// 		vector<float>().swap(tmpLfoDev.back_data);
		tmpLfoDev.back_data.clear();
		vector<float>().swap(tmpLfoDev.back_data);
	}

	tmpLfoDev.stat = 0;
	tmpLfoDev.stat_old = 0;
	tmpLfoDev.save_flag = 0;
	memset((void*)&tmpLfoDev.curve, 0, sizeof(LFO_CURVE_INFO));
	memset((void*)&tmpLfoDev.curve_ana, 0, sizeof(LFO_CURVE_INFO));

	if (!tmpLfoDev.vec_mode.empty())
	{
		// 		ClearLfoModeInfo(tmpLfoDev.vec_mode);
		tmpLfoDev.vec_mode.clear();
	}

	ClearLfoModeInfo(tmpLfoDev.mode);
	ClearLfoModeInfo(tmpLfoDev.mode_shape);

	if (!tmpLfoDev.vec_lead_mode.empty())
	{
		// 		ClearLfoModeInfo(tmpLfoDev.vec_lead_mode);
		tmpLfoDev.vec_lead_mode.clear();
	}
	// 	tmpLfoDev.start_time_lfo = 0;
	// 	tmpLfoDev.end_time_lfo = 0;
	// 	tmpLfoDev.hold_time_lfo = 0;

	tmpLfoDev.start_time = 0;
	tmpLfoDev.end_time = 0;
	tmpLfoDev.hold_time = 0;

	// 	tmpLfoDev.osc_stat = 0;
	// 	tmpLfoDev.osc_stat_old = 0;
	// 	tmpLfoDev.osc_save_flag = 0;
	// 	tmpLfoDev.osc_start_time = 0;
	// 	tmpLfoDev.osc_end_time = 0;
	// 	tmpLfoDev.osc_hold_time = 0;
	tmpLfoDev.judge_num = 0;
	return 0;
}

int CLfoAnaBase::ClearLfoModeShape(vector<LFO_MODE_SHAPE>& tmpVecMode)
{
	if (!tmpVecMode.empty())
	{
		// 		sprintf(m_LogString,"进入ClearLfoModeShape,大小【%d】",(int)tmpVecMode.size());
		// 		MessShow(m_LogString,LTERROR);
		for (int ii = 0; ii < (int)tmpVecMode.size(); ii++)
		{
			ClearLfoModeShape(tmpVecMode[ii]);
		}
		// 		vector<LFO_MODE_SHAPE>().swap(tmpVecMode);
		tmpVecMode.clear();
		vector<LFO_MODE_SHAPE>().swap(tmpVecMode);
	}
	return 0;
}

int CLfoAnaBase::ClearLfoModeShape(LFO_MODE_SHAPE& tmpModeShape)
{
	ClearLfoModeInfo(tmpModeShape.mode);
	if (!tmpModeShape.vec_dev.empty())
	{
		ClearLfoDevMode(tmpModeShape.vec_dev);
	}
	tmpModeShape.is_center = 0;
	tmpModeShape.is_lfo_source = 0;
	tmpModeShape.gn_num = 0;
	tmpModeShape.line_num = 0;
	tmpModeShape.tr_mun = 0;
	sprintf(tmpModeShape.is_source_des, "");
	sprintf(tmpModeShape.lfo_ana_des, "");
	sprintf(tmpModeShape.lfo_type, "");

	return 0;
}


int CLfoAnaBase::CopyLfoModeShape(vector<LFO_MODE_SHAPE>& ObjVecMode, vector<LFO_MODE_SHAPE>& SrcVecMode)
{
	if (!SrcVecMode.empty())
	{
		LFO_MODE_SHAPE tmpModeShape;
		ClearLfoModeShape(ObjVecMode);
		for (int ii = 0; ii < SrcVecMode.size(); ii++)
		{
			ClearLfoModeShape(tmpModeShape);
			CopyLfoModeShape(tmpModeShape, SrcVecMode[ii]);
			ObjVecMode.push_back(tmpModeShape);
		}
	}
	return 0;
}

int CLfoAnaBase::CopyLfoModeShape(LFO_MODE_SHAPE& ObjModeShape, LFO_MODE_SHAPE& SrcModeShape)
{
	LFO_DEV_MODE tmpDevMode;
	ClearLfoModeShape(ObjModeShape);
	memcpy(&ObjModeShape.mode, &SrcModeShape.mode, sizeof(LFO_MODE_INFO));
	ObjModeShape.is_center = SrcModeShape.is_center;
	ObjModeShape.is_lfo_source = SrcModeShape.is_lfo_source;
	sprintf(ObjModeShape.is_source_des, "%s", SrcModeShape.is_source_des);
	sprintf(ObjModeShape.lfo_ana_des, "%s", SrcModeShape.lfo_ana_des);
	for (int ii = 0; ii < SrcModeShape.vec_dev.size(); ii++)
	{
		ClearLfoDevMode(tmpDevMode);
		memcpy(&tmpDevMode.dev, &SrcModeShape.vec_dev[ii].dev, sizeof(LFO_DEV_BASE));
		memcpy(&tmpDevMode.mode, &SrcModeShape.vec_dev[ii].mode, sizeof(LFO_MODE_INFO));
		ObjModeShape.vec_dev.push_back(tmpDevMode);
	}
	return 0;
}


int CLfoAnaBase::CopyLfoSysInfo(LFO_SYS_INFO& ObjLfoSys, LFO_SYS_INFO& SrcLfoSys)
{
	ClearLfoSysInfo(ObjLfoSys);
	memcpy(&ObjLfoSys.lead_dev, &SrcLfoSys.lead_dev, sizeof(LFO_DEV_INFO));
	memcpy(&ObjLfoSys.lead_gen, &SrcLfoSys.lead_gen, sizeof(LFO_DEV_INFO));
	CopyLfoDev(ObjLfoSys.vec_dev, SrcLfoSys.vec_dev);
	CopyLfoDev(ObjLfoSys.cen_dev, SrcLfoSys.cen_dev);
	CopyLfoModeShape(ObjLfoSys.vec_mainmode, SrcLfoSys.vec_mainmode);
	CopyLfoModeShape(ObjLfoSys.vec_modeshape, SrcLfoSys.vec_modeshape);

	ObjLfoSys.window_id = SrcLfoSys.window_id;
	ObjLfoSys.freq_min = SrcLfoSys.freq_min;
	ObjLfoSys.freq_max = SrcLfoSys.freq_max;
	ObjLfoSys.gps_time = SrcLfoSys.gps_time;
	ObjLfoSys.gps_time_old = SrcLfoSys.gps_time_old;
	ObjLfoSys.start_time = SrcLfoSys.start_time;
	ObjLfoSys.end_time = SrcLfoSys.end_time;
	ObjLfoSys.sample_org = SrcLfoSys.sample_org;
	ObjLfoSys.stat = SrcLfoSys.stat;
	ObjLfoSys.stat_old = SrcLfoSys.stat_old;
	ObjLfoSys.lfo_warn_flag = SrcLfoSys.lfo_warn_flag;
	ObjLfoSys.lfo_type = SrcLfoSys.lfo_type;
	ObjLfoSys.center_id = SrcLfoSys.center_id;
	ObjLfoSys.lfo_gn_num = SrcLfoSys.lfo_gn_num;
	ObjLfoSys.lfo_ln_num = SrcLfoSys.lfo_ln_num;
	ObjLfoSys.lfo_tr_num = SrcLfoSys.lfo_tr_num;
	ObjLfoSys.lfo_inf_num = SrcLfoSys.lfo_inf_num;
	ObjLfoSys.lfos_gn_num = SrcLfoSys.lfos_gn_num;
	ObjLfoSys.lfos_ln_num = SrcLfoSys.lfos_ln_num;
	ObjLfoSys.lfos_tr_num = SrcLfoSys.lfos_tr_num;
	ObjLfoSys.progress = SrcLfoSys.progress;
	ObjLfoSys.file_end = SrcLfoSys.file_end;
	ObjLfoSys.warn_thread = SrcLfoSys.warn_thread;
	sprintf(ObjLfoSys.stat_desc, "%s", SrcLfoSys.stat_desc);
	sprintf(ObjLfoSys.lfo_desc, "%s", SrcLfoSys.lfo_desc);
	sprintf(ObjLfoSys.center_desc, "%s", SrcLfoSys.center_desc);
	sprintf(ObjLfoSys.lfos_gn_name, "%s", SrcLfoSys.lfos_gn_name);
	sprintf(ObjLfoSys.lfos_des, "%s", SrcLfoSys.lfos_des);

	return 0;
}

int CLfoAnaBase::CopyLfoSysInfo(vector<LFO_SYS_INFO>& ObjVecSys, vector<LFO_SYS_INFO>& SrcVecSys)
{
	if (!SrcVecSys.empty())
	{
		LFO_SYS_INFO tmpSysInfo;
		ClearLfoSysInfo(ObjVecSys);
		for (int ii = 0; ii < SrcVecSys.size(); ii++)
		{
			ClearLfoSysInfo(tmpSysInfo);
			CopyLfoSysInfo(tmpSysInfo, SrcVecSys[ii]);
			ObjVecSys.push_back(tmpSysInfo);
		}
	}
	return 0;
}


int CLfoAnaBase::CopyLfoDevBase(LFO_DEV_BASE& ObjDevBase, LFO_DEV_BASE& SrcDevBase)
{
	int ii = 0;
	memset(&ObjDevBase, 0, sizeof(LFO_DEV_BASE));

	ObjDevBase.dev_idx = SrcDevBase.dev_idx;
	ObjDevBase.dev_id = SrcDevBase.dev_id;
	ObjDevBase.fac_id = SrcDevBase.fac_id;
	ObjDevBase.area_id = SrcDevBase.area_id;
	ObjDevBase.dev_type = SrcDevBase.dev_type;
	ObjDevBase.ampl_limit_s = SrcDevBase.ampl_limit_s;
	ObjDevBase.ampl_limit_e = SrcDevBase.ampl_limit_e;
	ObjDevBase.dot_id = SrcDevBase.dot_id;
	ObjDevBase.dot_fac_id = SrcDevBase.dot_fac_id;
	ObjDevBase.dot_area_id = SrcDevBase.dot_area_id;
	ObjDevBase.IsYcDefine = SrcDevBase.IsYcDefine;
	sprintf(ObjDevBase.dev_name, "%s", SrcDevBase.dev_name);
	sprintf(ObjDevBase.fac_name, "%s", SrcDevBase.fac_name);
	sprintf(ObjDevBase.area_name, "%s", SrcDevBase.area_name);
	sprintf(ObjDevBase.dot_dev_name, "%s", SrcDevBase.dot_dev_name);
	sprintf(ObjDevBase.dot_fac_name, "%s", SrcDevBase.dot_fac_name);
	sprintf(ObjDevBase.dot_area_name, "%s", SrcDevBase.dot_area_name);

	for (ii = 0; ii < VEC_VIP_NUM; ii++)
	{
		ObjDevBase.nYcFlag[ii] = SrcDevBase.nYcFlag[ii];
		ObjDevBase.nYcFilNo[ii] = SrcDevBase.nYcFilNo[ii];
		ObjDevBase.nYcColNo[ii] = SrcDevBase.nYcColNo[ii];
	}
	return 0;
}


int CLfoAnaBase::CopyLfoDotInfo(WT_DOT_INFO& ObjDotInfo, WT_DOT_INFO& SrcDotInfo)
{
	memset(&ObjDotInfo, 0, sizeof(WT_DOT_INFO));

	// 	sprintf(m_LogString,"%s-CopyLfoDotInfo",SrcDotInfo.dot_name);
	// 	MessShow(m_LogString);
	ObjDotInfo.dot_id = SrcDotInfo.dot_id;
	ObjDotInfo.fac_id = SrcDotInfo.fac_id;
	ObjDotInfo.vlty_id = SrcDotInfo.vlty_id;
	ObjDotInfo.IsYcDefine = SrcDotInfo.IsYcDefine;
	sprintf(ObjDotInfo.dot_name, "%s", SrcDotInfo.dot_name);
	sprintf(ObjDotInfo.fac_name, "%s", SrcDotInfo.fac_name);
	for (int ii = 0; ii < VEC_VIP_NUM; ii++)
	{
		ObjDotInfo.nYcFlag[ii] = SrcDotInfo.nYcFlag[ii];
		ObjDotInfo.nYcFilNo[ii] = SrcDotInfo.nYcFilNo[ii];
		ObjDotInfo.nYcColNo[ii] = SrcDotInfo.nYcColNo[ii];
	}

	return 0;
}



int CLfoAnaBase::CopyLfoWtInfo(WT_DEV_INFO& ObjWtInfo, WT_DEV_INFO& SrcWtInfo)
{
	// 	sprintf(m_LogString,"%s-CopyLfoWtInfo",SrcWtInfo.dev_name);
	// 	MessShow(m_LogString);
	memset(&ObjWtInfo, 0, sizeof(WT_DEV_INFO));

	ObjWtInfo.dev_idx = SrcWtInfo.dev_idx;
	ObjWtInfo.dev_id = SrcWtInfo.dev_id;
	ObjWtInfo.fac_id = SrcWtInfo.fac_id;
	ObjWtInfo.area_id = SrcWtInfo.area_id;
	sprintf(ObjWtInfo.dev_name, "%s", SrcWtInfo.dev_name);
	sprintf(ObjWtInfo.fac_name, "%s", SrcWtInfo.fac_name);
	sprintf(ObjWtInfo.area_name, "%s", SrcWtInfo.area_name);
	ObjWtInfo.dev_type = SrcWtInfo.dev_type;
	ObjWtInfo.maxvlty_id = SrcWtInfo.maxvlty_id;
	ObjWtInfo.maxvbase = SrcWtInfo.maxvbase;
	ObjWtInfo.pmax = SrcWtInfo.pmax;
	ObjWtInfo.imax1 = SrcWtInfo.imax1;
	ObjWtInfo.max_pwr = SrcWtInfo.max_pwr;
	ObjWtInfo.is_old = SrcWtInfo.is_old;
	ObjWtInfo.vflag = SrcWtInfo.vflag;
	ObjWtInfo.yc_flag = SrcWtInfo.yc_flag;
	ObjWtInfo.is_add = SrcWtInfo.is_add;
	CopyLfoDotInfo(ObjWtInfo.dot1, SrcWtInfo.dot1);
	CopyLfoDotInfo(ObjWtInfo.dot2, SrcWtInfo.dot2);
	CopyLfoDotInfo(ObjWtInfo.dot3, SrcWtInfo.dot3);

	return 0;
}

int CLfoAnaBase::ClearLfoSysInfo(LFO_SYS_INFO& tmpLfoSys)
{
	ZERO_INT_REC(tmpLfoSys.center_id);
	tmpLfoSys.window_id = 0;
	tmpLfoSys.freq_min = 0.0f;
	tmpLfoSys.freq_max = 0.0f;
	tmpLfoSys.gps_time = 0;
	tmpLfoSys.gps_time_old = 0;
	tmpLfoSys.start_time = 0;
	tmpLfoSys.end_time = 0;
	tmpLfoSys.sample_org = 0;
	tmpLfoSys.stat = 0;
	tmpLfoSys.stat_old = 0;
	tmpLfoSys.lfo_warn_flag = 0;
	tmpLfoSys.lfo_type = 0;
	tmpLfoSys.lfo_gn_num = 0;
	tmpLfoSys.lfo_ln_num = 0;
	tmpLfoSys.lfo_tr_num = 0;
	tmpLfoSys.lfo_inf_num = 0;
	tmpLfoSys.lfos_gn_num = 0;
	tmpLfoSys.lfos_ln_num = 0;
	tmpLfoSys.lfos_tr_num = 0;
	tmpLfoSys.warn_thread = 0;

	memset(tmpLfoSys.stat_desc, 0, sizeof(tmpLfoSys.stat_desc));
	memset(tmpLfoSys.lfo_desc, 0, sizeof(tmpLfoSys.lfo_desc));
	memset(tmpLfoSys.lfos_des, 0, sizeof(tmpLfoSys.lfos_des));
	memset(tmpLfoSys.lfos_gn_name, 0, sizeof(tmpLfoSys.lfos_gn_name));
	memset(tmpLfoSys.center_desc, 0, sizeof(tmpLfoSys.center_desc));
	tmpLfoSys.save_flag = 0;
	// 	sprintf(m_LogString,"lead_dev=[%s]",tmpLfoSys.lead_dev.dev.dev_name);
	// 	MessShow(m_LogString);
	ClearLfoDev(tmpLfoSys.lead_dev);
	if (!tmpLfoSys.vec_dev.empty())
	{
		ClearLfoDev(tmpLfoSys.vec_dev);
	}
	if (!tmpLfoSys.cen_dev.empty())
	{
		ClearLfoDev(tmpLfoSys.cen_dev);
	}

	if (!tmpLfoSys.vec_modeshape.empty())
	{
		ClearLfoModeShape(tmpLfoSys.vec_modeshape);
	}
	if (!tmpLfoSys.vec_mainmode.empty())
	{
		ClearLfoModeShape(tmpLfoSys.vec_mainmode);
	}


	return 0;
}

int CLfoAnaBase::ClearLfoSysInfo(vector<LFO_SYS_INFO>& tmpVecSys)
{
	if (!tmpVecSys.empty())
	{
		for (int ii = 0; ii < (int)tmpVecSys.size(); ii++)
		{
			ClearLfoSysInfo(tmpVecSys[ii]);
		}
		// 		vector<LFO_SYS_INFO>().swap(tmpVecSys);
		tmpVecSys.clear();
		vector<LFO_SYS_INFO>().swap(tmpVecSys);
	}
	return 0;
}

int CLfoAnaBase::CopyLfoModeInfo(LFO_MODE_INFO& ObjLfoMode, LFO_MODE_INFO& SrcLfoMode)
{
	memset(&ObjLfoMode, 0, sizeof(LFO_MODE_INFO));
	ObjLfoMode.start_time = SrcLfoMode.start_time;
	ObjLfoMode.end_time = SrcLfoMode.end_time;
	ObjLfoMode.ampl = SrcLfoMode.ampl;
	ObjLfoMode.freq = SrcLfoMode.freq;
	ObjLfoMode.damp = SrcLfoMode.damp;
	ObjLfoMode.phas = SrcLfoMode.phas;
	ObjLfoMode.real = SrcLfoMode.real;
	ObjLfoMode.imag = SrcLfoMode.imag;
	ObjLfoMode.engy = SrcLfoMode.engy;
	ObjLfoMode.engy_pre_time = SrcLfoMode.engy_pre_time;
	ObjLfoMode.fail = SrcLfoMode.fail;
	ObjLfoMode.aver_val = SrcLfoMode.aver_val;
	ObjLfoMode.group = SrcLfoMode.group;
	ObjLfoMode.fact = SrcLfoMode.fact;
	ObjLfoMode.pow = SrcLfoMode.pow;
	ObjLfoMode.is_source = SrcLfoMode.is_source;
	ObjLfoMode.eve_source = SrcLfoMode.eve_source;
	ObjLfoMode.stat = SrcLfoMode.stat;
	sprintf(ObjLfoMode.in_or_out, "%s", SrcLfoMode.in_or_out);
	return 0;
}

int CLfoAnaBase::CopyLfoModeInfo(vector<LFO_MODE_INFO>& ObjVecMode, vector<LFO_MODE_INFO>& SrcVecMode)
{
	if (!SrcVecMode.empty())
	{
		LFO_MODE_INFO tmpModeInfo;
		for (int ii = 0; ii < SrcVecMode.size(); ii++)
		{
			ClearLfoModeInfo(tmpModeInfo);
			CopyLfoModeInfo(tmpModeInfo, SrcVecMode[ii]);
			ObjVecMode.push_back(tmpModeInfo);
		}
	}
	return 0;
}


int CLfoAnaBase::ClearLfoModeInfo(LFO_MODE_INFO& tmpLfoMode)
{
	memset((void*)&tmpLfoMode, 0, sizeof(LFO_MODE_INFO));
	tmpLfoMode.damp = m_LfoPara.max_damp;
	return 0;
}

int CLfoAnaBase::ClearLfoModeInfo(vector<LFO_MODE_INFO>& tmpVecMode)
{
	if (!tmpVecMode.empty())
	{
		// 		vector<LFO_MODE_INFO>().swap(tmpVecMode);
		tmpVecMode.clear();
		vector<LFO_MODE_INFO>().swap(tmpVecMode);
	}
	return 0;
}

int CLfoAnaBase::ClearLfoPronyInfo(LFO_PRONY_INFO& tmpLfoProny)
{
	memset((void*)&tmpLfoProny.dev, 0, sizeof(LFO_DEV_BASE));
	if (!tmpLfoProny.v2d_data.empty())
	{
		// 		vector<float>().swap(tmpLfoProny.v2d_data);
		tmpLfoProny.v2d_data.clear();
		vector<float>().swap(tmpLfoProny.v2d_data);
	}

	ClearLfoModeInfo(tmpLfoProny.vec_mode);
	return 0;
}
int CLfoAnaBase::ClearLfoPronyInfo(vector<LFO_PRONY_INFO>& tmpVecProny)
{
	if (!tmpVecProny.empty())
	{
		for (int ii = 0; ii < (int)tmpVecProny.size(); ii++)
		{
			ClearLfoPronyInfo(tmpVecProny[ii]);
		}
	}
	return 0;
}
int CLfoAnaBase::ClearLfoPronyInfo1(vector<LFO_PRONY_INFO>& tmpVecProny)
{
	if (!tmpVecProny.empty())
	{
		for (int ii = 0; ii < (int)tmpVecProny.size(); ii++)
		{
			ClearLfoPronyInfo(tmpVecProny[ii]);
		}
		// 		vector<LFO_PRONY_INFO>().swap(tmpVecProny);
		tmpVecProny.clear();
		vector<LFO_PRONY_INFO>().swap(tmpVecProny);
	}
	return 0;
}

int CLfoAnaBase::ClearLfoDevMode(LFO_DEV_MODE& tmpDevMode)
{
	memset((void*)&tmpDevMode.dev, 0, sizeof(LFO_DEV_BASE));
	memset((void*)&tmpDevMode.mode, 0, sizeof(LFO_MODE_INFO));
	return 0;
}

int CLfoAnaBase::ClearLfoDevMode(vector<LFO_DEV_MODE>& tmpVecDevMode)
{
	if (!tmpVecDevMode.empty())
	{
		// 		vector<LFO_DEV_MODE>().swap(tmpVecDevMode);
		tmpVecDevMode.clear();
		vector<LFO_DEV_MODE>().swap(tmpVecDevMode);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
CLfoAnaObj::CLfoAnaObj()
{
	ClearLfoCase(m_LfoCase);
	ClearLfoCase(m_oscLfoCase);
	InitPara();
	printf("CLfoAnaObj构造结束\n");
}

CLfoAnaObj::~CLfoAnaObj()
{
	ClearLfoSysInfo(m_VecLfoWindow);
	ClearLfoCase(m_LfoCase);
	ClearLfoSysInfo(m_VecOscWindow);
	ClearLfoCase(m_oscLfoCase);
	int ii;
	for (ii = 0; ii < LFO_THREAD_NUM; ii++)
	{
		ClearLfoDev(m_pLfoThread[ii].m_VecDev);
		ClearLfoModeShape(m_pLfoThread[ii].m_VecModeShape);
		ClearLfoSysInfo(m_pLfoThread[ii].m_MonSys);
		ClearLfoDev(m_pLfoThreadInfo[ii].VecDev);
		ClearLfoSysInfo(m_pLfoThreadInfo[ii].MonSys);
	}

	SAFE_DELETE(m_pDataThread);
	SAFE_DELETE_ARRAY(m_pLfoThread);
	SAFE_DELETE_ARRAY(m_pLfoThreadInfo);
}

int CLfoAnaObj::InitPara()
{
	// 初始化数据缓存类
	m_pDataThread = new CDataCache;
	// 初始化监视线程类(4个频率段)
	m_pLfoThread = new CLfoAnaBase[LFO_THREAD_NUM];
	m_pLfoThreadInfo = new LFO_THREAD_INFO[LFO_THREAD_NUM];
	memset((char*)m_pLfoModeCurve, 0, sizeof(PERIOD_LEAD_MODE));
	memset((char*)m_pLfoCurve, 0, sizeof(LPO_RT_CURVE_WAMS));

	int ii;
	for (ii = 0; ii < LFO_THREAD_NUM; ii++)
	{
		ClearLfoDev(m_pLfoThread[ii].m_VecDev);
		ClearLfoModeShape(m_pLfoThread[ii].m_VecModeShape);
		ClearLfoSysInfo(m_pLfoThread[ii].m_MonSys);
		ClearLfoDev(m_pLfoThreadInfo[ii].VecDev);
		ClearLfoSysInfo(m_pLfoThreadInfo[ii].MonSys);

		ClearLfoSysInfo(m_pLfoThread[ii].m_oscMonSys);
		// 		ClearLfoSysInfo(m_pLfoThreadInfo[ii].oscMonSys);
	}

	// 	sprintf(m_LogString,"[yjj000]初始化监视线程类的状态[%d]",m_MonSys.stat_old);
	// 	cout<<m_LogString<<endl;
		//初始化主线程类
	ClearLfoDev(m_VecDev);
	ClearLfoModeShape(m_VecModeShape);
	ClearLfoSysInfo(m_MonSys);

	ClearLfoSysInfo(m_oscMonSys);
	InitProc();

	// 	sprintf(m_LogString,"[yjj111]初始化监视线程类的状态[%d]",m_MonSys.stat_old);
	// 	cout<<m_LogString<<endl;

	return 0;
}

int CLfoAnaObj::MainProc()
{
	// 1)	读取配置参数
#ifdef _LFO_HIS_LIB_
	string tmpfilename = m_maindir + m_backslash + "ini" + m_backslash + g_ProName + "_offline.ini";
#else
	string tmpfilename = m_maindir + m_backslash + "ini" + m_backslash + g_ProName + ".ini";
#endif
	if (ReadIniFile(tmpfilename, m_LfoPara) < 0)
	{
		sprintf(m_LogString, "采用默认参数");
		MessShow(m_LogString, LTERROR);
	}

	if (GetLfoPara(m_LfoPara, 0) < 0)
	{
		sprintf(m_LogString, "频率范围读取失败,采用默认参数");
		MessShow(m_LogString, LTERROR);
	}
	// 2)	参数检查
	if (CheckPara(m_LfoPara, LTINFO) < 0)
	{
		sprintf(m_LogString, "参数错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	// 3)	读取设备静态信息
	if (InitDevInfo(m_VecDev) < 0)
	{
		sprintf(m_LogString, "设备静态信息读取错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	//    for(int mm =0;mm<m_VecDev.size();mm++)
	//    {
	//    	sprintf(m_LogString,"静态信息展示[%s][%d][%d][%d]",m_VecDev[mm].dev.dev_name,m_VecDev[mm].dev.dev_id,m_VecDev[mm].dev.nYcFilNo[12],m_VecDev[mm].dev.nYcColNo[12]);
	//        MessShow(m_LogString,LTINFO);
	//    }
		// 4)	将设备静态信息保存到文件
	if (SaveDevInfo(m_VecDev) < 0)
	{
		sprintf(m_LogString, "设备静态信息文件生成错误");
		MessShow(m_LogString, LTERROR);
		// return -1;
	}
	// 5)	线程结构体初始化
	if (ThreadInit() < 0)
	{
		sprintf(m_LogString, "线程结构体初始化失败");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	// 6)	启动线程
	if (StartThread() < 0)
	{
		sprintf(m_LogString, "线程启动失败");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	// 7)    主线程循环等待，置主线程状态标志，判断子线程状态
	// 线程主函数完成后会等待下一个任务，此时状态为ThreadStateWaiting
	// 通过ThreadStateWaiting状态判断主函数是否结束，而不是通过ThreadStateDown状态判断
	// 线程析构时会调用结束函数终止线程

	int ii, nFlag = 0;
	while (true)
	{
		nFlag = 0;
		//         if( m_pDataThread->ThreadState() == ThreadStateWaiting )
		//         {
		// 			MessShow("m_pDataThread->ThreadState() == ThreadStateWaiting");
		//             nFlag ++;
		//         }

		for (ii = 0; ii < g_Thread_nun; ii++)
		{
			if (m_pLfoThread[ii].ThreadState() == ThreadStateWaiting)
			{
				sprintf(m_LogString, "m_pLfoThread[%d].ThreadState() == ThreadStateWaiting", ii);
				MessShow(m_LogString, LTINFO);
				nFlag++;
			}
		}

		// 		sprintf(m_LogString,"nFlag=%d,g_Thread_nun=%d",nFlag,g_Thread_nun);
		// 		MessShow(m_LogString,LTINFO);
		if (nFlag >= g_Thread_nun)
		{
			break;
		}
		PT_DelayMinSec(2000);
	}

	// 显式终止线程
	m_pDataThread->Stop();
	for (ii = 0; ii < g_Thread_nun; ii++)
	{
		m_pLfoThread[ii].Stop();
	}
	if (b_end_proc == 1)
	{
		sprintf(m_LogString, "主进程退出,程序退出");
		MessShow(m_LogString, LTINFO);
		return -1;
	}

	sprintf(m_LogString, "主进程退出");
	MessShow(m_LogString, LTINFO);
	return 0;
}

BOOL CLfoAnaObj::OnTask(LPVOID lpv)
{
	return TRUE;
}

BOOL CLfoAnaObj::OnTask()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    ReadIniFile
// FullName:  CLfoAnaObj::ReadIniFile
// Access:    public 
// Returns:   int
// Qualifier:
// 输入：配置参数文件名filename，参数结构体(LFO_PARA_STRUCT m_LfoPara)
// 输出：参数结构体(LFO_PARA_STRUCT m_LfoPara)
// 返回值：0，正常；1，异常
//************************************
int CLfoAnaObj::ReadIniFile(const string filename, LFO_PARA_STRUCT& LfoPara)
{
	if (PF_FindFile(filename) == false)
	{
		sprintf(m_LogString, "配置文件不存在[%s]", filename.c_str());
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	CIniFile iniFile(filename);
	if (iniFile.INI_ReadFile() == false)
	{
		sprintf(m_LogString, "配置文件读取失败: %s", filename.c_str());
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	string sectionName = "应用参数";
	g_ctxName = iniFile.INI_GetValue(sectionName, "ctx_name", AC_REALTIME_NAME);
	g_ctxNo = iniFile.INI_GetValueI(sectionName, "ctx_no", AC_REALTIME_NO);
	g_appName = iniFile.INI_GetValue(sectionName, "app_name", AF_WAMS_NAME);
	g_appNo = iniFile.INI_GetValueI(sectionName, "app_no", AP_WAMS);

	sectionName = "计算参数";
	m_DebugType = iniFile.INI_GetValueI(sectionName, "debug_type", 0);
	LfoPara.freq_min = iniFile.INI_GetValueFF(sectionName, "freq_min", 0.05);
	LfoPara.freq_max = iniFile.INI_GetValueFF(sectionName, "freq_max", 8.0);
	LfoPara.lfo_freq_min = iniFile.INI_GetValueFF(sectionName, "lowfreq_min", 0.2);
	LfoPara.lfo_freq_max = iniFile.INI_GetValueFF(sectionName, "lowfreq_max", 2.5);

	LfoPara.gn_wn_ampl = iniFile.INI_GetValueFF(sectionName, "gn_wn_ampl", 8.0);
	LfoPara.gn_swn_ampl2 = iniFile.INI_GetValueFF(sectionName, "gn_wn_ampl2", 10.0);
	LfoPara.gn_swn_ampl3 = iniFile.INI_GetValueFF(sectionName, "gn_wn_ampl3", 12.0);
	LfoPara.gn_wn_ampl1 = iniFile.INI_GetValueFF(sectionName, "gn_wn_ampl1", 8.0);
	LfoPara.gn_ewn_ampl2 = iniFile.INI_GetValueFF(sectionName, "gn_ewn_ampl2", 10.0);
	LfoPara.gn_ewn_ampl3 = iniFile.INI_GetValueFF(sectionName, "gn_ewn_ampl3", 12.0);
	LfoPara.ln_swn_ampl1 = iniFile.INI_GetValueFF(sectionName, "ln_swn_ampl1", 20.0);
	LfoPara.ln_swn_ampl2 = iniFile.INI_GetValueFF(sectionName, "ln_swn_ampl2", 25.0);
	LfoPara.ln_swn_ampl3 = iniFile.INI_GetValueFF(sectionName, "ln_swn_ampl3", 35.0);
	LfoPara.ln_ewn_ampl1 = iniFile.INI_GetValueFF(sectionName, "ln_ewn_ampl1", 20.0);
	LfoPara.ln_ewn_ampl2 = iniFile.INI_GetValueFF(sectionName, "ln_ewn_ampl2", 25.0);
	LfoPara.ln_ewn_ampl3 = iniFile.INI_GetValueFF(sectionName, "ln_ewn_ampl3", 35.0);
	LfoPara.tr_swn_ampl1 = iniFile.INI_GetValueFF(sectionName, "tr_swn_ampl1", 20.0);
	LfoPara.tr_swn_ampl2 = iniFile.INI_GetValueFF(sectionName, "tr_swn_ampl2", 25.0);
	LfoPara.tr_swn_ampl3 = iniFile.INI_GetValueFF(sectionName, "tr_swn_ampl3", 35.0);
	LfoPara.tr_ewn_ampl1 = iniFile.INI_GetValueFF(sectionName, "tr_ewn_ampl1", 20.0);
	LfoPara.tr_ewn_ampl2 = iniFile.INI_GetValueFF(sectionName, "tr_ewn_ampl2", 25.0);
	LfoPara.tr_ewn_ampl3 = iniFile.INI_GetValueFF(sectionName, "tr_ewn_ampl3", 35.0);
	LfoPara.volt_thre1 = iniFile.INI_GetValueFF(sectionName, "volt_thre1", 220.0);
	LfoPara.volt_thre2 = iniFile.INI_GetValueFF(sectionName, "volt_thre2", 500.0);
	LfoPara.gn_capacity1 = iniFile.INI_GetValueFF(sectionName, "gn_capacity1", 200);
	LfoPara.gn_capacity2 = iniFile.INI_GetValueFF(sectionName, "gn_capacity2", 600);

	LfoPara.fjud_wnap_per = iniFile.INI_GetValueFF(sectionName, "fjud_wnap_per", 60.0);
	LfoPara.ana_ampl_per = iniFile.INI_GetValueFF(sectionName, "ana_ampl_per", 30.0);
	LfoPara.lfo_jud_num = iniFile.INI_GetValueI(sectionName, "lfo_jud_num", 2);

	LfoPara.thread_mode = iniFile.INI_GetValueI(sectionName, "thread_mode", 0);
	LfoPara.pre_freq1 = iniFile.INI_GetValueFF(sectionName, "pre_freq1", 0.05);
	LfoPara.pre_freq2 = iniFile.INI_GetValueFF(sectionName, "pre_freq2", 0.1);
	LfoPara.pre_freq3 = iniFile.INI_GetValueFF(sectionName, "pre_freq3", 0.2);
	LfoPara.pre_freq4 = iniFile.INI_GetValueFF(sectionName, "pre_freq4", 2.5);
	LfoPara.pre_freq5 = iniFile.INI_GetValueFF(sectionName, "pre_freq5", 8);

	LfoPara.time_leng = iniFile.INI_GetValueI(sectionName, "time_leng", 10);
	LfoPara.time_leng_coe = iniFile.INI_GetValueFF(sectionName, "time_leng_coe", 2.0);
	LfoPara.sample_freq = iniFile.INI_GetValueFF(sectionName, "sample_freq", 12.5);
	LfoPara.sample_freq_coe = iniFile.INI_GetValueFF(sectionName, "sample_freq_coe", 5.0);
	LfoPara.int_time = iniFile.INI_GetValueI(sectionName, "int_time", 5);
	LfoPara.int_time_coe = iniFile.INI_GetValueFF(sectionName, "int_time_coe", 0.5);
	LfoPara.time_leng_min = iniFile.INI_GetValueI(sectionName, "time_leng_min", 5);  // 时间窗口最小值
	LfoPara.time_leng_max = iniFile.INI_GetValueI(sectionName, "time_leng_max", 40);  // 时间窗口最大值
	LfoPara.sample_freq_min = iniFile.INI_GetValueFF(sectionName, "sample_freq_min", 6.25);  // 采样频率最小值
	LfoPara.sample_freq_max = iniFile.INI_GetValueFF(sectionName, "sample_freq_max", 25.0);  // 采样频率最大值
	LfoPara.int_time_min = iniFile.INI_GetValueI(sectionName, "int_time_min", 0);  // 交叠窗口最小值
	LfoPara.int_time_max = iniFile.INI_GetValueI(sectionName, "int_time_max", 40);  // 交叠窗口最大值
	LfoPara.delay_time = iniFile.INI_GetValueI(sectionName, "delay_time", 2);
	LfoPara.freq_diff = iniFile.INI_GetValueFF(sectionName, "freq_diff", 0.05);
	LfoPara.phas_diff = iniFile.INI_GetValueFF(sectionName, "phas_diff", 15.0);
	LfoPara.cohcy_angl = iniFile.INI_GetValueFF(sectionName, "cohcy_angl", COHCY_ANGL);
	LfoPara.is_warn = iniFile.INI_GetValueI(sectionName, "is_warn", 1);
	LfoPara.sh_fact = iniFile.INI_GetValueFF(sectionName, "sh_fact", 0.01);
	LfoPara.sh_gn_pow = iniFile.INI_GetValueFF(sectionName, "sh_gn_pow", 0.5);
	LfoPara.sh_ln_pow = iniFile.INI_GetValueFF(sectionName, "sh_ln_pow", 1.5);
	LfoPara.sh_phas = iniFile.INI_GetValueFF(sectionName, "sh_phas", 30);
	LfoPara.mainmode_num = iniFile.INI_GetValueI(sectionName, "mainmode_num", 3);
	LfoPara.inf_ampl = iniFile.INI_GetValueFF(sectionName, "inf_ampl", 1.0);
	LfoPara.center_volt_ampl = iniFile.INI_GetValueFF(sectionName, "center_volt_ampl", 10.0);
	LfoPara.para_VoltAmplPer = iniFile.INI_GetValueFF(sectionName, "para_VoltAmplPer", 0.6);
	LfoPara.para_nShwMaxFcNum = iniFile.INI_GetValueI(sectionName, "para_nShwMaxFcNum", 5);
	LfoPara.para_Ampl_aver = iniFile.INI_GetValueFF(sectionName, "Ampl_aver", 0.6);
	LfoPara.max_damp = iniFile.INI_GetValueFF(sectionName, "max_damp", 0.2);
	LfoPara.disp_num = iniFile.INI_GetValueI(sectionName, "disp_num", 3);

	LfoPara.rec_num = iniFile.INI_GetValueI(sectionName, "rec_num", 2);
	LfoPara.Voltunit_is_kV = iniFile.INI_GetValueI(sectionName, "Voltunit_is_kV", 1);
	LfoPara.if_flit_lfo = iniFile.INI_GetValueI(sectionName, "if_flit_lfo", 0);
	LfoPara.lfo_warn = iniFile.INI_GetValueI(sectionName, "lfo_warn", 0);
	LfoPara.curve_time_60 = iniFile.INI_GetValueI(sectionName, "curve_time_60", 0);
	LfoPara.sm_md_damp = iniFile.INI_GetValueFF(sectionName, "sm_md_damp", 100.0);
	LfoPara.para_LfoAnaByPwr = iniFile.INI_GetValueI(sectionName, "para_LfoAnaByPwr", 1);
	LfoPara.warn_app_no = iniFile.INI_GetValueI(sectionName, "warn_app_no", AP_SCADA);
	LfoPara.is_cycle = iniFile.INI_GetValueI(sectionName, "is_cycle", 0);
	LfoPara.is_save_casefile = iniFile.INI_GetValueI(sectionName, "is_save_casefile", 1);
	LfoPara.is_save_devinfo = iniFile.INI_GetValueI(sectionName, "is_save_devinfo", 1);
	LfoPara.is_add_facname = iniFile.INI_GetValueI(sectionName, "is_add_facname", 1);
	LfoPara.is_eng = iniFile.INI_GetValueI(sectionName, "is_eng", 0);

	LfoPara.stor_damp = iniFile.INI_GetValueFF(sectionName, "stor_damp", 30.0);
	LfoPara.lack_damp = iniFile.INI_GetValueFF(sectionName, "lack_damp", 5.0);
	LfoPara.flfo_damp = iniFile.INI_GetValueFF(sectionName, "flfo_damp", 5.0);
	LfoPara.lfo_damp = iniFile.INI_GetValueFF(sectionName, "lfo_damp", 3.0);
	LfoPara.damp_min = iniFile.INI_GetValueFF(sectionName, "damp_min", -20.0);
	LfoPara.warn_time_max = iniFile.INI_GetValueI(sectionName, "warn_time_max", 20);
	LfoPara.safe_time_max = iniFile.INI_GetValueI(sectionName, "safe_time_max", 20);
	LfoPara.cache_windows_num = iniFile.INI_GetValueI(sectionName, "cache_windows_num", 10);
	sprintf(LfoPara.devfile_name, "%s", iniFile.INI_GetValue(sectionName, "devfile_name", "DeviceInfo.xml").c_str());
	sprintf(LfoPara.datafile_name, "%s", iniFile.INI_GetValue(sectionName, "datafile_name", "Datafile.csv").c_str());
	sprintf(LfoPara.devicefile_name, "%s", iniFile.INI_GetValue(sectionName, "devicefile_name", "device.txt").c_str());
	LfoPara.stor_ampl = iniFile.INI_GetValueFF(sectionName, "stor_ampl", 0.1);
	LfoPara.data_source = iniFile.INI_GetValueI(sectionName, "data_source", 0);
	LfoPara.devinfo_source = iniFile.INI_GetValueI(sectionName, "devinfo_source", 0);
	LfoPara.is_mon_lfo = iniFile.INI_GetValueI(sectionName, "is_mon_lfo", 1);
	LfoPara.mode_save_amplper = iniFile.INI_GetValueFF(sectionName, "mode_save_amplper", 30.0);
	LfoPara.is_lfo_curve = iniFile.INI_GetValueI(sectionName, "is_lfo_curve", 1);
	LfoPara.rcd_pre_t = iniFile.INI_GetValueI(sectionName, "rcd_pre_t", 30);
	LfoPara.rcd_aft_t = iniFile.INI_GetValueI(sectionName, "rcd_aft_t", 30);
	LfoPara.is_tmd = iniFile.INI_GetValueI(sectionName, "is_tmd", 1);
	LfoPara.is_triems = iniFile.INI_GetValueI(sectionName, "is_triems", 1);


	LfoPara.osc_pow_ampl = iniFile.INI_GetValueFF(sectionName, "osc_pow_ampl", 10.0);
	LfoPara.osc_sec_ampl = iniFile.INI_GetValueFF(sectionName, "osc_sec_ampl", 10.0);


	// 	sprintf(m_LogString,"是否发送低频振荡告警标志: %d",LfoPara.lfo_warn);
	// 	MessShow(m_LogString,LTINFO);
	// 	sprintf(m_LogString,"是否显示60秒曲线标志: %d",LfoPara.curve_time_60);
	// 	MessShow(m_LogString,LTINFO);
	// 	sprintf(m_LogString,"频段划分模式标志: %d",LfoPara.thread_mode);
	// 	MessShow(m_LogString,LTINFO);

	sectionName = "PRONY分析参数";
	// Prony分析参数
	LfoPara.prony_para.if_set_rank = iniFile.INI_GetValueI(sectionName, "if_set_rank", 0);//0;   // 0 自动确定阶数； 1 人为设定阶数
	LfoPara.prony_para.rank = iniFile.INI_GetValueI(sectionName, "rank", 30);
	LfoPara.prony_para.queue_type = iniFile.INI_GetValueI(sectionName, "queue_type", -2);      // 0 幅值排序; -1 预测幅值排序;-2 预测能量排序
	LfoPara.prony_para.delDC = iniFile.INI_GetValueI(sectionName, "delDC", 1);       // 是否去除直流(0 否; 1 是)
	LfoPara.prony_para.freq_min = iniFile.INI_GetValueFF(sectionName, "freq_min", 0.02);   //2008-03-11
	LfoPara.prony_para.freq_max = iniFile.INI_GetValueFF(sectionName, "freq_max", 100.0);  // 3.0f;
	LfoPara.prony_para.minAbsAmpl = iniFile.INI_GetValueFF(sectionName, "minAbsAmpl", 0.001);  // 0.00001f;
	LfoPara.prony_para.stor_lim_ampl = iniFile.INI_GetValueFF(sectionName, "stor_lim_ampl", 0.0);       // 0.08f;
	LfoPara.prony_para.stor_lim_damp = iniFile.INI_GetValueFF(sectionName, "stor_lim_damp", 100.0);
	LfoPara.prony_para.snr_min = iniFile.INI_GetValueFF(sectionName, "snr_min", 20.0);      // 30;40;
	LfoPara.prony_para.eng_time = iniFile.INI_GetValueFF(sectionName, "eng_time", 3.0);
	LfoPara.prony_para.max_iter = iniFile.INI_GetValueI(sectionName, "max_iter", 3);
	LfoPara.prony_para.is_filter = iniFile.INI_GetValueI(sectionName, "is_filter", 0);
	sprintf(m_LogString, "queue_type= %d,max_iter=%d,rank=%d", LfoPara.prony_para.queue_type, LfoPara.prony_para.max_iter, LfoPara.prony_para.rank);
	MessShow(m_LogString, LTINFO);
	return 0;
}

//************************************
// Method:    ReadFreqRange
// FullName:  CLfoAnaObj::ReadFreqRange
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: LFO_PARA_STRUCT & LfoPara
// 从LFO参数表第1条记录读取频率范围
//************************************
int CLfoAnaObj::ReadFreqRange(LFO_PARA_STRUCT& LfoPara)
{
	return 0;
}

//************************************
// Method:    InitDevInfo
// FullName:  CLfoAnaObj::InitDevInfo
// Access:    public 
// Returns:   int
// Qualifier:
// 输入：设备信息开关参数，厂站，发电机，交流线路，变压器结构体(vector< LFO_DEV_INFO > m_VecDev)，或设备静态信息文件。
// 输出：填充厂站，发电机，交流线路，变压器结构体(vector< LFO_DEV_INFO > m_VecDev)基本设备信息
// 返回值：0，正常；1，异常
//************************************
int CLfoAnaObj::InitDevInfo(vector<LFO_DEV_INFO>& VecDev)
{
	ClearLfoDev(VecDev);
	//     若设备信息从文件读取，则读取数据文件目录下的设备信息文件。
	//     若设备信息从数据库读取，则读取LFO设备表中设备，并初始化。

	if (LFO_DATA_SOURCE_REALTTIME == m_LfoPara.data_source)
	{
		GetDevFromDB(VecDev);
	}
	else
	{
		sprintf(m_LogString, "[提示：]开始从数据文件读取数据");
		MessShow(m_LogString, LTERROR);
		GetDevFromFile(VecDev);
	}
	return 0;
}

int CLfoAnaObj::GetDevFromFile(vector<LFO_DEV_INFO>& VecDev)
{
	//*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_
	vector<LFO_DEV_INFO> Cts_VecDev;
	vector<LFO_INTLINE_INFO> Cts_VecIntline;
	// LFO厂站表
	if (AddLfoFc(Cts_VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO厂站失败");
		MessShow(m_LogString, LTERROR);
	}

	// LFO发电机表
	if (AddLfoGen(Cts_VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO发电机失败");
		MessShow(m_LogString, LTERROR);
	}

	// LFO交流线路表
	if (AddLfoLine(Cts_VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO交流线路表失败");
		MessShow(m_LogString, LTERROR);
	}

	// LFO变压器表
	if (AddLfoTr(Cts_VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO变压器表失败");
		MessShow(m_LogString, LTERROR);
	}

	if (AddLfoIntface(VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO联络线走廊表失败");
		MessShow(m_LogString, LTERROR);
	}

	if (AddLfoIntLine(m_VecIntline) < 0)
	{
		sprintf(m_LogString, "初始化LFO联络线走廊与支路表失败");
		MessShow(m_LogString, LTERROR);
	}

	if (SetFacInfo(Cts_VecDev) < 0)
	{
		sprintf(m_LogString, "填充厂站信息失败");
		MessShow(m_LogString, LTERROR);
	}

	if (SetAreaInfo(Cts_VecDev) < 0)
	{
		sprintf(m_LogString, "填充区域信息失败");
		MessShow(m_LogString, LTERROR);
	}
	// 填充PMU测点标志
	if (SetFesYcInfo(Cts_VecDev) < 0)
	{
		sprintf(m_LogString, "获取PMU测点信息失败");
		MessShow(m_LogString, LTERROR);
	}
	if (SetDevInfo(Cts_VecDev) < 0)
	{
		sprintf(m_LogString, "填充设备基本信息失败");
		MessShow(m_LogString, LTERROR);
	}
	//*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_

	sprintf(m_LogString, "读取设备信息文件[%s]", m_LfoPara.datafile_name);
	MessShow(m_LogString, LTINFO);

	string tmpstring = m_LfoPara.datafile_name;
	CDataFile df;
	int ngen, nline, ntr, ninf, nfac;
	ngen = 0;
	nline = 0;
	ntr = 0;
	nfac = 0;
	if (df.ReadFileHead(tmpstring.c_str()) == false)
	{
		sprintf(m_LogString, "读取设备信息失败[%s]", tmpstring.c_str());
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	// 	df.m_DataHead.is_old_file = 0;
	m_LfoPara.data_sample = m_df.m_DataHead.time_sample;
	sprintf(m_LogString, "文件格式[%d],设备数量[%d],采样间隔[%d]", df.m_DataHead.is_old_file, df.m_DataHead.vec_devid_s.size(), m_LfoPara.data_sample);
	MessShow(m_LogString);

	// 填充设备静态信息
	int ii, jj, nColNum, tableno, nFlag;
	LFO_DEV_INFO tmpDevInfo;
	string::size_type idx;
	string tmpColName;
	for (ii = 0; ii < (int)df.m_DataHead.vec_devid_s.size(); ii++)
	{
		// 		sprintf(m_LogString,"读案例id=%s %ld",df.m_DataHead.vec_devid_s[ii].c_str(),atol(df.m_DataHead.vec_devid_s[ii].c_str()));
		// 		MessShow(m_LogString);
		ClearLfoDev(tmpDevInfo);
#ifndef _NUSP
		if (df.m_DataHead.is_old_file == 1)
		{
			// 旧格式
// 			sprintf(m_LogString,"数据文件旧格式");
// 			MessShow(m_LogString,LTINFO);
			// 获取设备ID
			tmpstring = df.m_DataHead.vec_devid_s[ii];

			idx = tmpstring.find_last_of("_");
			if (idx != string::npos)
			{
				tmpDevInfo.dev.dev_id = ATOI(tmpstring.substr(0, idx).c_str());
				tmpDevInfo.dev_info.dev_id = tmpDevInfo.dev.dev_id;
			}
			tmpstring.erase(0, idx + 1);
			nColNum = atoi(tmpstring.c_str());

			for (int mm = 0; mm < (int)Cts_VecDev.size(); mm++)
			{
				if (Cts_VecDev[mm].dev_info.dev_id == tmpDevInfo.dev.dev_id)
				{
					tmpDevInfo.dev.dev_idx = Cts_VecDev[mm].dev_info.dev_idx;
					tmpDevInfo.dev.fac_id = Cts_VecDev[mm].dev_info.fac_id;
					tmpDevInfo.dev.area_id = Cts_VecDev[mm].dev_info.area_id;
#ifdef _CHG_AMPL
					tmpDevInfo.dev.chg_ampl_per = Cts_VecDev[mm].dev.chg_ampl_per;
					tmpDevInfo.dev.is_chg_ampl = Cts_VecDev[mm].dev.is_chg_ampl;
#endif
					sprintf(tmpDevInfo.dev.fac_name, "%s", Cts_VecDev[mm].dev_info.dot1.fac_name);
					sprintf(tmpDevInfo.dev.dot_fac_name, "%s", Cts_VecDev[mm].dev_info.dot2.fac_name);
					// 					sprintf(tmpDevInfo.dev.area_name,"%s",Cts_VecDev[mm].dev_info.dot1.area_name);
					// 					sprintf(tmpDevInfo.dev.dot_area_name,"%s",Cts_VecDev[mm].dev_info.dot2.area_name);
					sprintf(tmpDevInfo.dev.dot_dev_name, "%s", Cts_VecDev[mm].dev.dot_dev_name);
					//20150922 add
					tmpDevInfo.dev_info.maxvbase = Cts_VecDev[mm].dev_info.maxvbase;
					tmpDevInfo.dev_info.pmax = Cts_VecDev[mm].dev_info.pmax;
					// 					tmpDevInfo.dev_info.imax1 = Cts_VecDev[mm].dev_info.imax1;   
					// 					tmpDevInfo.dev_info.max_pwr=Cts_VecDev[mm].dev_info.max_pwr;  
				}

			}

			// 获取设备名称
			// 去掉第一列时间
			tmpstring.clear();
			if (df.GetNumberOfVariables() >= ii + 1)
			{
				df.GetVariableName(ii + 1, tmpstring);
			}
			// 获取测点名称
// 			if (nColNum >=0 && nColNum<=VEC_VIP_NUM)
// 			{
// 				sprintf(tmpDevInfo.dev.yc_name[nColNum],"%s",tmpstring.c_str());
// 			}
// 			else
// 			{
// 				sprintf(m_LogString,"测点序号错误[%d]",nColNum);
// 				MessShow(m_LogString,LTERROR);
// 			}

			idx = tmpstring.find_last_of("_");
			if (idx != string::npos)
			{
				sprintf(tmpDevInfo.dev.dev_name, "%s", tmpstring.substr(0, idx).c_str());
			}
		}
		else
#endif // !_NUSP
		{
			// 新格式
			// 获取设备ID
// 			tmpstring = df.m_DataHead.vec_devid_s[ii];
			tmpDevInfo.dev.dev_id = df.m_DataHead.vec_devid_s[ii];
			tmpDevInfo.dev_info.dev_id = tmpDevInfo.dev.dev_id;
			// 			sprintf(m_LogString,"设备ID=["INT_REC_FMT"]",INT_REC_VAL(tmpDevInfo.dev.dev_id));
			// 			MessShow(m_LogString,LTINFO);

			for (int mm = 0; mm < (int)Cts_VecDev.size(); mm++)
			{
				if (Cts_VecDev[mm].dev_info.dev_id == tmpDevInfo.dev.dev_id)
				{
					tmpDevInfo.dev.dev_idx = Cts_VecDev[mm].dev_info.dev_idx;
					tmpDevInfo.dev.fac_id = Cts_VecDev[mm].dev_info.fac_id;
					tmpDevInfo.dev.area_id = Cts_VecDev[mm].dev_info.area_id;
					tmpDevInfo.dev.dot_id = Cts_VecDev[mm].dev.dot_id;
#ifdef _CHG_AMPL
					tmpDevInfo.dev.chg_ampl_per = Cts_VecDev[mm].dev.chg_ampl_per;
					tmpDevInfo.dev.is_chg_ampl = Cts_VecDev[mm].dev.is_chg_ampl;
#endif
					sprintf(tmpDevInfo.dev.fac_name, "%s", Cts_VecDev[mm].dev_info.dot1.fac_name);
					sprintf(tmpDevInfo.dev.dot_fac_name, "%s", Cts_VecDev[mm].dev_info.dot2.fac_name);
					// 					sprintf(tmpDevInfo.dev.area_name,"%s",Cts_VecDev[mm].dev_info.dot1.area_name);
					// 					sprintf(tmpDevInfo.dev.dot_area_name,"%s",Cts_VecDev[mm].dev_info.dot2.area_name);

					sprintf(tmpDevInfo.dev.dot_dev_name, "%s", Cts_VecDev[mm].dev.dot_dev_name);
					//20150922 add
					tmpDevInfo.dev_info.maxvbase = Cts_VecDev[mm].dev_info.maxvbase;
					tmpDevInfo.dev_info.pmax = Cts_VecDev[mm].dev_info.pmax;
					tmpDevInfo.dev_info.imax1 = Cts_VecDev[mm].dev_info.imax1;
					tmpDevInfo.dev_info.max_pwr = Cts_VecDev[mm].dev_info.max_pwr;
				}
			}
			sprintf(m_LogString, "[yjj]GetDevFromFile设备ID["INT_REC_FMT"],索引["INT_REC_FMT"]", INT_REC_VAL(tmpDevInfo.dev.dev_id), INT_REC_VAL(tmpDevInfo.dev.dev_idx));
			MessShow(m_LogString, LTINFO);
			// 获取测点名称
			// 去掉前两列"日期时间,毫秒"
			tmpstring.clear();
			if (df.GetNumberOfVariables() >= ii + 2)
			{
				df.GetVariableName(ii + 2, tmpstring);
			}

			idx = tmpstring.find_last_of("-");
			// 获取设备名称
			if (idx != string::npos)
			{
				sprintf(tmpDevInfo.dev.dev_name, "%s", tmpstring.substr(0, idx).c_str());
			}

			// 判断测点类型
			tmpColName = tmpstring.substr(idx + 1);
			if (tmpColName == "00P")
			{
				// 有功
				nColNum = 2;
			}
			else if (tmpColName == "00Q")
			{
				// 无功
				nColNum = 3;
			}
			else if (tmpColName == "UAV相角")
			{
				// 电压相角
				nColNum = 1;
			}
			else if (tmpColName == "UAV幅值")
			{
				// 电压幅值
				nColNum = 0;
			}
			else if (tmpColName == "EEV相角")
			{
				// 发电机功角
				nColNum = 16;
			}
			else
			{
				// 未定义，默认为无效
				nColNum = -1;
			}

			// 			if (nColNum >=0 && nColNum<=VEC_VIP_NUM)
			// 			{
			// 				sprintf(tmpDevInfo.dev.yc_name[nColNum],"%s",tmpstring.c_str());
			// 			}
		}

		// 获取设备类型
		tableno = PODB_GetTabelNoByDevId(tmpDevInfo.dev.dev_id);
#ifdef _NUSP
		if (tableno == SUBSTATION_INFO_NO_TAB || tableno == PLANT_INFO_NO_TAB || tableno == BS_DEVICE_NO_TAB)
#else
		if (tableno == FAC_INFO_NO_TAB || tableno == BS_DEVICE_NO_TAB)
#endif // _NUSP		
		{
			tmpDevInfo.dev.dev_type = MENU_LFO_DEV_FAC;
			tmpDevInfo.dev_info.dev_type = MENU_LFO_DEV_FAC;
			// 			sprintf(m_LogString,"该设备类型为厂站[%d]",tmpDevInfo.dev.dev_type);
			// 			MessShow(m_LogString);
		}
		else if (tableno == GN_DEVICE_NO_TAB)
		{
			tmpDevInfo.dev.dev_type = MENU_LFO_DEV_GEN;
			tmpDevInfo.dev.dot_id = tmpDevInfo.dev.dev_id;
			sprintf(tmpDevInfo.dev.dot_dev_name, "%s", tmpDevInfo.dev.dev_name);
			// 			sprintf(m_LogString,"该设备类型为发电机[%d]",tmpDevInfo.dev.dev_type);
			// 			MessShow(m_LogString);
		}
		else if (tableno == ACLN_DEVICE_NO_TAB || tableno == ACLN_DOT_NO_TAB)
		{
			tmpDevInfo.dev.dev_type = MENU_LFO_DEV_LINE;
			// 			sprintf(m_LogString,"该设备类型为线路[%d]",tmpDevInfo.dev.dev_type);
			// 			MessShow(m_LogString);
		}
		else if (tableno == TR_DEVICE_NO_TAB)
		{
			tmpDevInfo.dev.dev_type = MENU_LFO_DEV_TR;
			// 			sprintf(m_LogString,"该设备类型为变压器[%d]",tmpDevInfo.dev.dev_type);
			// 			MessShow(m_LogString);
		}
		else
		{
			sprintf(m_LogString, "[%s]设备类型错误[%d]", tmpDevInfo.dev.dev_name, tableno);
			MessShow(m_LogString, LTERROR);
		}

		// 判断是否相同设备
		nFlag = 0;
		for (jj = 0; jj < (int)VecDev.size(); jj++)
		{
			if (tmpDevInfo.dev.dev_id == VecDev[jj].dev.dev_id)
			{
				// 相同设备,保存测点名称
				nFlag = 1;
				// 				if (nColNum >=0 && nColNum<=VEC_VIP_NUM)
				// 				{
				// 					sprintf(VecDev[jj].dev.yc_name[nColNum],"%s",tmpDevInfo.dev.yc_name[nColNum]);
				// 				}
			}
		}

		if (nFlag == 0)
		{
			// 没有相同设备,新增
			tmpDevInfo.dev_info.dev_type = tmpDevInfo.dev.dev_type;
			tmpDevInfo.dev_info.dot1.dot_id = tmpDevInfo.dev.dev_id;
			sprintf(tmpDevInfo.dev_info.dot1.dot_name, "%s", tmpDevInfo.dev.dev_name);
			// 			if (nColNum >=0 && nColNum<=VEC_VIP_NUM)
			// 			{
			// 				sprintf(tmpDevInfo.dev_info.dot1.yc_name,"%s",tmpDevInfo.dev.yc_name[nColNum]);
			// 			}
			tmpDevInfo.dev_info.dev_id = tmpDevInfo.dev.dev_id;
			sprintf(tmpDevInfo.dev_info.dev_name, "%s", tmpDevInfo.dev.dev_name);
			tmpDevInfo.dev_info.yc_flag = 1;
			// 			sprintf(m_LogString,"设备[%s]端点ID[%ld]",tmpDevInfo.dev.dev_name,tmpDevInfo.dev.dot_id);
			// 			MessShow(m_LogString);
			VecDev.push_back(tmpDevInfo);
		}
	}
#ifdef _LFO_HIS_LIB_
	vector<long> devid;
	devid.clear();
	string tmpstring = m_LfoPara.devicefile_name;
	vector<LFO_DEV_INFO> tmpvecdev;
	LFO_DEV_INFO tmpdev;
	GetDevice(tmpstring, devid, m_VecIntline);
	if (!devid.empty())
	{//监视设备文件中没有监视设备，默认所有设备都监视
		tmpvecdev.clear();
		map<long, int> map_devid;  //存放元件ID
		map<long, int>::iterator it;
		for (int ii = 0; ii < devid.size(); ii++)
		{
			map_devid.insert(map<long, int>::value_type(devid[ii], ii));
		}
		for (int ii = 0; ii < VecDev.size(); ii++)
		{
			it = map_devid.find(VecDev[ii].dev.dev_id);
			if (it != map_devid.end())
			{//监视设备文件中含有本设备
				tmpvecdev.push_back(VecDev[ii]);
			}
			else
			{//监视设备文件中不含有本设备,从VecDev中去掉

			}
		}
		VecDev.clear();
		CopyLfoDev(VecDev, tmpvecdev);
	}
	//从断面支路信息中获取断面信息
	tmpvecdev.clear();
	bool bfind;
	for (int ii = 0; ii < m_VecIntline.size(); ii++)
	{
		bfind = false;
		for (int jj = 0; jj < tmpvecdev.size(); jj++)
		{
			if (m_VecIntline[ii].inf_no == tmpvecdev[jj].dev.dev_id)
			{
				bfind = true;
				break;
			}
		}
		if (!bfind)
		{
			ClearLfoDev(tmpdev);
			tmpdev.dev.dev_id = m_VecIntline[ii].inf_no;
			sprintf(tmpdev.dev.dev_name, "%s", m_VecIntline[ii].inf_name);
			tmpvecdev.push_back(tmpdev);
		}
	}
	for (int ii = 0; ii < tmpvecdev.size(); ii++)
	{
		VecDev.push_back(tmpvecdev[ii]);
	}
#endif
	for (int ii = 0; ii < m_VecIntline.size(); ii++)
	{
		for (int jj = 0; jj < VecDev.size(); jj++)
		{
			if (m_VecIntline[ii].line_no == VecDev[jj].dev.dev_id)
			{
				m_VecIntline[ii].dev_info = VecDev[jj];
				break;
			}
		}
	}
	df.ClearData();
	return 0;
}


int CLfoAnaObj::GetDevice(const string filename, vector<long>& VecDevID, vector<LFO_INTLINE_INFO>& VecINTLINE)
{

	FILE* fp = NULL;
	vector<string>  vdevice;//保存监视设备信息
	vector<string> vintline;
	vector<string> vtxt;
	vector<string> vDataVal;
	LFO_INTLINE_INFO tmpintline;
	vdevice.clear();
	vintline.clear();
	vtxt.clear();

	if ((fp = fopen(filename.c_str(), "r+t")) == NULL)
	{
		sprintf(m_LogString, "监视设备文件[%s]不存在", filename.c_str());
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	string strLine;
	char   szLine[1024];
	int line = 0;
	int device_bgn, device_end, intline_bgn, intline_end;
	device_bgn = 0;
	device_end = 0;
	intline_bgn = 0;
	intline_end = 0;
	while (!feof(fp))
	{
		memset(szLine, 0, 1024 * sizeof(char));
		fgets(szLine, 1023, fp);
		PS_CharTrimLeft(szLine);
		PS_CharTrimRight(szLine);
		strLine = std::string(szLine);
		vtxt.push_back(strLine);
		line++;
	}
	fclose(fp);

	for (int ii = 0; ii < vtxt.size(); ii++)
	{
		if (vtxt.at(ii).find("[监视设备信息]") != std::string::npos)
		{
			vdevice.assign(vtxt.begin() + ii, vdevice.end());
		}
		if (vtxt.at(ii).find("[断面支路信息]") != std::string::npos)
		{
			vintline.assign(vtxt.begin() + ii, vdevice.end());
			break;
		}
	}
	//读取监视设备信息
	VecDevID.clear();
	for (int ii = 0; ii < vdevice.size(); ii++)
	{
		if (vdevice.at(ii).find("[/监视设备信息]") != std::string::npos)
		{
			break;
		}
		if (ii == 0)//第一行读取监视设备数量
		{
			continue;
		}
		vDataVal.clear();
		PS_StringDivde(vdevice.at(ii), ",", vDataVal);
		if (vDataVal.size() < 2) continue;
		VecDevID.push_back(atol(vDataVal.at(0).c_str()));
	}
	//读取断面支路信息
	for (int ii = 0; ii < vintline.size(); ii++)
	{
		if (vintline.at(ii).find("[/断面支路信息]") != std::string::npos)
		{
			break;
		}
		if (ii == 0)//第一行读取断面支路数量
		{
			continue;
		}
		vDataVal.clear();
		PS_StringDivde(vdevice.at(ii), ",", vDataVal);
		if (vDataVal.size() < 8) continue;
		tmpintline.inf_no = ATOI(vDataVal.at(0).c_str());
		sprintf(tmpintline.inf_name, "%s", vDataVal.at(1).c_str());
		tmpintline.eletype = atoi(vDataVal.at(2).c_str());
		tmpintline.line_no = ATOI(vDataVal.at(3).c_str());
		sprintf(tmpintline.line_name, "%s", vDataVal.at(4).c_str());
		tmpintline.mpoint = atoi(vDataVal.at(5).c_str());
		tmpintline.stat_no = ATOI(vDataVal.at(6).c_str());
		tmpintline.onoff = atoi(vDataVal.at(7).c_str());
		VecINTLINE.push_back(tmpintline);
	}
	return 0;
}

int CLfoAnaObj::GetDevFromDB(vector<LFO_DEV_INFO>& VecDev)
{
	// LFO厂站表
	if (AddLfoFc(VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO厂站表失败");
		MessShow(m_LogString, LTERROR);
	}

	// LFO发电机表
	if (AddLfoGen(VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO发电机失败");
		MessShow(m_LogString, LTERROR);
	}

	// LFO交流线路表
	if (AddLfoLine(VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO交流线路表失败");
		MessShow(m_LogString, LTERROR);
	}

	// LFO变压器表
	if (AddLfoTr(VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO变压器表失败");
		MessShow(m_LogString, LTERROR);
	}

	// LFO联络线走廊表
	if (AddLfoIntface(VecDev) < 0)
	{
		sprintf(m_LogString, "初始化LFO联络线走廊表失败");
		MessShow(m_LogString, LTERROR);
	}

	// LFO联络线走廊与支路表
	if (AddLfoIntLine(m_VecIntline) < 0)
	{
		sprintf(m_LogString, "初始化LFO联络线走廊与支路表失败");
		MessShow(m_LogString, LTERROR);
	}

	if (SetFacInfo(VecDev) < 0)
	{
		sprintf(m_LogString, "填充厂站信息失败");
		MessShow(m_LogString, LTERROR);
	}

	if (SetAreaInfo(VecDev) < 0)
	{
		sprintf(m_LogString, "填充区域信息失败");
		MessShow(m_LogString, LTERROR);
	}

	if (SetVlTypeInfo(VecDev) < 0)
	{
		sprintf(m_LogString, "填充电压等级信息失败");
		MessShow(m_LogString, LTERROR);
	}

	// 填充PMU测点标志
	if (SetFesYcInfo(VecDev) < 0)
	{
		sprintf(m_LogString, "获取PMU测点信息失败");
		MessShow(m_LogString, LTERROR);
	}

	if (SetDevInfo(VecDev) < 0)
	{
		sprintf(m_LogString, "填充设备基本信息失败");
		MessShow(m_LogString, LTERROR);
	}

	for (int ii = 0; ii < m_VecIntline.size(); ii++)
	{
		for (int jj = 0; jj < VecDev.size(); jj++)
		{
			if (m_VecIntline[ii].line_no == VecDev[jj].dev.dev_id)
			{
				m_VecIntline[ii].dev_info = VecDev[jj];
				break;
			}
		}
	}


	return 0;
}

//************************************
// Method:    SaveDevInfo
// FullName:  CLfoAnaObj::SaveDevInfo
// Access:    public 
// Returns:   int
// Qualifier:
// 以XML文件格式保存设备信息，保存到RES目录下。
// 输入：固定文件名，厂站m_VecFac，发电机m_VecGen，交流线路m_VecLine，变压器m_VecTr结构体(vector<WT_DEV_INFO>)
// 输出：设备静态信息文件。
// 返回值：0，正常；1，异常
// TODO:设备静态信息文件格式待定
//************************************
int CLfoAnaObj::SaveDevInfo(vector<LFO_DEV_INFO>& VecDev)
{
	if (m_LfoPara.is_save_devinfo == 0)
	{
		return 0;
	}

	int ii, jj;
	time_t    CurTime;
	time(&CurTime);

	string tmpfilename = m_maindir + m_backslash + "res" + m_backslash + "DeviceInfo.txt";
	fstream fs;
	fs.open(tmpfilename.c_str(), std::ios::out);
	if (fs.fail())
	{
		sprintf(m_LogString, "设备信息文件生成失败[%s]", tmpfilename.c_str());
		MessShow(m_LogString, LTERROR);
		fs.close();
		return 0;
	}

	sprintf(m_LogString, "设备信息更新时间: %s[%d]", PT_GetChineseTimeStringBySeconds(CurTime), (int)VecDev.size());
	//MessShow(m_LogString);
	fs << m_LogString << std::endl;

	for (ii = 0; ii < (int)VecDev.size(); ii++)
	{
		if (VecDev[ii].dev_info.dev_type == MENU_LFO_DEV_GEN)
		{
			if (m_LfoPara.is_add_facname == 1)
			{
				sprintf(m_LogString, "%d\t\t%s%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d",
					ii + 1, VecDev[ii].dev_info.fac_name, VecDev[ii].dev_info.dev_name, INT_REC_VAL(VecDev[ii].dev_info.dev_id), VecDev[ii].dev_info.fac_name, INT_REC_VAL(VecDev[ii].dev_info.fac_id),
					VecDev[ii].dev_info.vflag, VecDev[ii].dev_info.yc_flag);
			}
			else
			{
				sprintf(m_LogString, "%d\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d",
					ii + 1, VecDev[ii].dev_info.dev_name, INT_REC_VAL(VecDev[ii].dev_info.dev_id), VecDev[ii].dev_info.fac_name, INT_REC_VAL(VecDev[ii].dev_info.fac_id),
					VecDev[ii].dev_info.vflag, VecDev[ii].dev_info.yc_flag);
			}
			//fs<<m_LogString<<endl;
			sprintf(m_LogString, "%d\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d",
				ii + 1, VecDev[ii].dev_info.dev_name, INT_REC_VAL(VecDev[ii].dev_info.dev_id), VecDev[ii].dev_info.fac_name, INT_REC_VAL(VecDev[ii].dev_info.fac_id),
				VecDev[ii].dev_info.vflag, VecDev[ii].dev_info.yc_flag);
			//MessShow(m_LogString,LTDEV);
			fs << m_LogString << endl;
			sprintf(m_LogString, "NAME\t\tFilNo\t\tColNo");
			//MessShow(m_LogString,LTDEV);
			fs << m_LogString << endl;
			for (jj = 0; jj < VEC_VIP_NUM; jj++)
			{
				sprintf(m_LogString, "%s\t\t\t\t%d\t\t\t\t%d",
					PODB_GetYcName(jj).c_str(), VecDev[ii].dev_info.dot1.nYcFilNo[jj], VecDev[ii].dev_info.dot1.nYcColNo[jj]);
				//MessShow(m_LogString,LTDEV);
				fs << m_LogString << endl;
			}
		}
		else if (VecDev[ii].dev_info.dev_type == MENU_LFO_DEV_LINE)
		{
			sprintf(m_LogString, "%d\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d\t\t%d\t\t%d",
				ii + 1, VecDev[ii].dev_info.dev_name, INT_REC_VAL(VecDev[ii].dev_info.dev_id), VecDev[ii].dev_info.dot1.fac_name, INT_REC_VAL(VecDev[ii].dev_info.dot1.dot_id), VecDev[ii].dev_info.dot2.fac_name, INT_REC_VAL(VecDev[ii].dev_info.dot2.dot_id),
				VecDev[ii].dev_info.vflag, VecDev[ii].dev_info.yc_flag, VecDev[ii].dev_info.is_old, VecDev[ii].dev_info.is_add);
			//MessShow(m_LogString,LTDEV);
			fs << m_LogString << endl;
			sprintf(m_LogString, "NAME\t\tFilNo1\t\tColNo1\t\tFilNo2\t\tColNo2");
			//MessShow(m_LogString,LTDEV);
			fs << m_LogString << endl;
			for (jj = 0; jj < 13; jj++)
			{
				sprintf(m_LogString, "%s\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d",
					PODB_GetYcName(jj).c_str(), VecDev[ii].dev_info.dot1.nYcFilNo[jj], VecDev[ii].dev_info.dot1.nYcColNo[jj],
					VecDev[ii].dev_info.dot2.nYcFilNo[jj], VecDev[ii].dev_info.dot2.nYcColNo[jj]);
				//MessShow(m_LogString,LTDEV);
				fs << m_LogString << endl;
			}
		}
		else if (VecDev[ii].dev_info.dev_type == MENU_LFO_DEV_TR)
		{
			if (m_LfoPara.is_add_facname == 1)
			{
				sprintf(m_LogString, "%d\t\t%s%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d\t\t%d\t\t%d",
					ii + 1, VecDev[ii].dev_info.fac_name, VecDev[ii].dev_info.dev_name, INT_REC_VAL(VecDev[ii].dev_info.dev_id), VecDev[ii].dev_info.dot1.dot_name, INT_REC_VAL(VecDev[ii].dev_info.dot1.dot_id), VecDev[ii].dev_info.dot2.dot_name, INT_REC_VAL(VecDev[ii].dev_info.dot2.dot_id),
					VecDev[ii].dev_info.dot3.dot_name, INT_REC_VAL(VecDev[ii].dev_info.dot3.dot_id), VecDev[ii].dev_info.vflag, VecDev[ii].dev_info.yc_flag, VecDev[ii].dev_info.is_old, VecDev[ii].dev_info.is_add);
			}
			else
			{
				sprintf(m_LogString, "%d\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%s["INT_REC_FMT"]\t\t%d\t\t%d\t\t%d\t\t%d",
					ii + 1, VecDev[ii].dev_info.dev_name, INT_REC_VAL(VecDev[ii].dev_info.dev_id), VecDev[ii].dev_info.dot1.dot_name, INT_REC_VAL(VecDev[ii].dev_info.dot1.dot_id), VecDev[ii].dev_info.dot2.dot_name, INT_REC_VAL(VecDev[ii].dev_info.dot2.dot_id),
					VecDev[ii].dev_info.dot3.dot_name, INT_REC_VAL(VecDev[ii].dev_info.dot3.dot_id), VecDev[ii].dev_info.vflag, VecDev[ii].dev_info.yc_flag, VecDev[ii].dev_info.is_old, VecDev[ii].dev_info.is_add);
			}
			//MessShow(m_LogString,LTDEV);
			fs << m_LogString << endl;
			sprintf(m_LogString, "NAME\t\tFilNo1\t\tColNo1\t\tFilNo2\t\tColNo2\t\tFilNo3\t\tColNo3");
			//MessShow(m_LogString,LTDEV);
			fs << m_LogString << endl;
			for (jj = 0; jj < 13; jj++)
			{
				sprintf(m_LogString, "%s\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d",
					PODB_GetYcName(jj).c_str(), VecDev[ii].dev_info.dot1.nYcFilNo[jj], VecDev[ii].dev_info.dot1.nYcColNo[jj],
					VecDev[ii].dev_info.dot2.nYcFilNo[jj], VecDev[ii].dev_info.dot2.nYcColNo[jj],
					VecDev[ii].dev_info.dot3.nYcFilNo[jj], VecDev[ii].dev_info.dot3.nYcColNo[jj]);
				//MessShow(m_LogString,LTDEV);
				fs << m_LogString << endl;
			}
		}
	}

	fs.close();

	return 0;
}

//************************************
// Method:    ThreadInit
// FullName:  CLfoAnaObj::ThreadInit
// Access:    public 
// Returns:   int
// Qualifier:
// 将设备静态信息幅值给各个线程的设备静态信息。
// 填充线程参数。线程编号，线程日志初始化。
// 输入：设备静态信息，各个线程结构体
// 输出：各个线程结构体
//************************************
int CLfoAnaObj::ThreadInit()
{
	int ii;
	string tmpString;
	vector<float> vfreq;
	vector<float> vprefreq;
	float tmp_freq;
	int n_min, n_max;
	// 数据线程初始化
	sprintf(m_LogString, "%s_data.log", g_ProName);
	tmpString = g_pAnaObj->m_maindir + g_pAnaObj->m_backslash + "log" + g_pAnaObj->m_backslash + m_LogString;
	m_pDataThread->PLF_InitLogFile(tmpString, MAXFILESIZE, MAXBACKUPNUM);
	m_pDataThread->PLF_setLogType(m_DebugType);
	sprintf(m_LogString, "数据缓存线程日志文件名[%s]", tmpString.c_str());
	MessShow(m_LogString, LTDEBUG);
	g_Thread_nun = 1;
	//根据监视频段和预设频段确定监视线程数
	if (m_LfoPara.thread_mode == 0)
	{
		vprefreq.push_back(m_LfoPara.pre_freq1);
		vprefreq.push_back(m_LfoPara.pre_freq2);
		vprefreq.push_back(m_LfoPara.pre_freq3);
		vprefreq.push_back(m_LfoPara.pre_freq4);
		vprefreq.push_back(m_LfoPara.pre_freq5);
		for (ii = 0; ii < vprefreq.size(); ii++)
		{
			if (ii < vprefreq.size() - 1)
			{
				if (m_LfoPara.freq_min >= vprefreq[ii] && m_LfoPara.freq_min < vprefreq[ii + 1])
				{
					n_min = ii;
				}
			}
			if (ii > 1)
			{
				if (m_LfoPara.freq_max > vprefreq[ii - 1] && m_LfoPara.freq_max <= vprefreq[ii])
				{
					n_max = ii;
				}
			}
		}

		g_Thread_nun = n_max - n_min;
	}
	else if (m_LfoPara.thread_mode == 1)
	{
		tmp_freq = 3 * m_LfoPara.freq_min;
		while (true)
		{
			if (tmp_freq < m_LfoPara.freq_max)
			{
				vfreq.push_back(tmp_freq);
				tmp_freq = (tmp_freq - 0.01) * 3;
			}
			else
			{
				vfreq.push_back(m_LfoPara.freq_max);
				break;
			}
		}
		g_Thread_nun = vfreq.size();
	}
	sprintf(m_LogString, "监视频率段为[%f]~[%f],分为[%d]个线程", m_LfoPara.freq_min, m_LfoPara.freq_max, g_Thread_nun);
	MessShow(m_LogString, LTDEBUG);
	if (g_Thread_nun > LFO_THREAD_NUM)
	{
		g_Thread_nun = LFO_THREAD_NUM;
	}

	// 监视线程初始化
	for (ii = 0; ii < g_Thread_nun; ii++)
	{
		// 线程日志初始化
		sprintf(m_LogString, "%s_%d.log", g_ProName, ii + 1);
		tmpString = g_pAnaObj->m_maindir + g_pAnaObj->m_backslash + "log" + g_pAnaObj->m_backslash + m_LogString;
		m_pLfoThread[ii].PLF_InitLogFile(tmpString, MAXFILESIZE, MAXBACKUPNUM);
		m_pLfoThread[ii].PLF_setLogType(m_DebugType);
		sprintf(m_LogString, "线程[%d]日志文件名[%s]", ii + 1, tmpString.c_str());
		MessShow(m_LogString, LTDEBUG);
		// 线程参数
		m_pLfoThread[ii].m_ThreadIdx = ii;
		memset((void*)&m_pLfoThread[ii].m_LfoPara, 0, sizeof(LFO_PARA_STRUCT));
		m_pLfoThread[ii].m_LfoPara = m_LfoPara;
		// 设备静态信息
		ClearLfoDev(m_pLfoThread[ii].m_VecDev);
		// 		m_pLfoThread[ii].m_VecDev = m_VecDev;
		CopyLfoDev(m_pLfoThread[ii].m_VecDev, m_VecDev);
		// 		m_pLfoThread[ii].m_VecIntline = m_VecIntline;
		ClearLfoIntline(m_pLfoThread[ii].m_VecIntline);
		CopyLfoIntline(m_pLfoThread[ii].m_VecIntline, m_VecIntline);

		// 使用一次线程TableOp,解决多实体问题
#ifdef _WAMAP_
		sprintf(m_LogString, "解决多实体问题,LFO参数表个数[%d]", m_pLfoThread[ii].PODB_GetTabRecNum(LFO_PARA_NO_TAB));
		MessShow(m_LogString, LTINFO);
#endif
		if (m_LfoPara.thread_mode == 0)
		{
			if (ii == 0)
			{
				m_pLfoThread[ii].m_LfoPara.freq_min = m_LfoPara.freq_min;
			}
			else
			{
				m_pLfoThread[ii].m_LfoPara.freq_min = vprefreq[ii + n_min];
			}
			if (ii == g_Thread_nun - 1)
			{
				m_pLfoThread[ii].m_LfoPara.freq_max = m_LfoPara.freq_max;
			}
			else
			{
				m_pLfoThread[ii].m_LfoPara.freq_max = vprefreq[ii + n_min + 1];
			}
		}
		else if (m_LfoPara.thread_mode == 1)
		{
			if (ii == 0)
			{
				m_pLfoThread[ii].m_LfoPara.freq_min = m_LfoPara.freq_min;
			}
			else
			{
				m_pLfoThread[ii].m_LfoPara.freq_min = vfreq[ii - 1] - 0.01;
			}
			m_pLfoThread[ii].m_LfoPara.freq_max = vfreq[ii];

		}
		sprintf(m_LogString, "该线程监视频段为[%f]~[%f]", m_pLfoThread[ii].m_LfoPara.freq_min, m_pLfoThread[ii].m_LfoPara.freq_max);
		MessShow(m_LogString, LTINFO);
	}

	// 计算各个线程参数
// 	if (LFO_THREAD_NUM==3)
// 	{
// 		m_pLfoThread[0].m_LfoPara.freq_min = m_LfoPara.lfo_freq_max;
// 		m_pLfoThread[0].m_LfoPara.freq_max = m_LfoPara.freq_max;
// // 		m_pLfoThread[0].m_LfoPara.time_leng = 20.0f;
// // 		m_pLfoThread[0].m_LfoPara.sample_freq = 6.25f;
// // 		m_pLfoThread[0].m_LfoPara.int_time = 10.0f;
// //		m_pLfoThread[0].m_LfoPara.is_mon_lfo = 0;
// 		
// 		m_pLfoThread[1].m_LfoPara.freq_min = m_LfoPara.freq_min;
// 		m_pLfoThread[1].m_LfoPara.freq_max = m_LfoPara.lfo_freq_min;
// // 		m_pLfoThread[1].m_LfoPara.time_leng = 10.0f;
// // 		m_pLfoThread[1].m_LfoPara.sample_freq = 12.5f;
// // 		m_pLfoThread[1].m_LfoPara.int_time = 5.0f;
// //		m_pLfoThread[1].m_LfoPara.is_mon_lfo = 0;
// 		
// 		m_pLfoThread[2].m_LfoPara.freq_min = m_LfoPara.lfo_freq_min;	//低频振荡频段
// 		m_pLfoThread[2].m_LfoPara.freq_max = m_LfoPara.lfo_freq_max;
// // 		m_pLfoThread[2].m_LfoPara.time_leng = 5.0f;
// // 		m_pLfoThread[2].m_LfoPara.sample_freq = 25.0f;
// // 		m_pLfoThread[2].m_LfoPara.int_time = 0.0f;
// //		m_pLfoThread[2].m_LfoPara.is_mon_lfo = 0;
// 		
// 	}
// 	else if (LFO_THREAD_NUM==1)
// 	{
// 		m_pLfoThread[0].m_LfoPara.freq_min = m_LfoPara.lfo_freq_min;
// 		m_pLfoThread[0].m_LfoPara.freq_max = m_LfoPara.lfo_freq_max;
// 	}

	return 0;
}

//************************************
// Method:    StartThread
// FullName:  CLfoAnaObj::StartThread
// Access:    public 
// Returns:   int
// Qualifier:
// 启动数据缓存线程和监视线程
// 输入：数据缓存线程状态结构，数据缓存类结构
// 输出：数据缓存线程状态结构
//************************************
int CLfoAnaObj::StartThread()
{
	int nFlag = 0;
	// 启动数据缓存线程
//     if (StartDataCacheTh()<0)
//     {
//         nFlag = -1;
//         sprintf(m_LogString,"启动数据缓存线程失败");
//         MessShow(m_LogString,LTERROR);
//     }
// 
	// 启动监视线程
	if (StartMonitorTh() < 0)
	{
		nFlag = -1;
		sprintf(m_LogString, "启动数据缓存线程失败");
		MessShow(m_LogString, LTERROR);
	}

	if (nFlag < 0)
	{
		return -1;
	}
	return 0;
}

//************************************
// Method:    StartDataCacheTh
// FullName:  CLfoAnaObj::StartDataCacheTh
// Access:    public 
// Returns:   int
// Qualifier:
// 启动数据缓存线程
// 输入：数据缓存线程状态结构，数据缓存类结构
// 输出：数据缓存线程状态结构
// TODO:暂不开发
//************************************
int CLfoAnaObj::StartDataCacheTh()
{
	m_pDataThread->Start();
	if (m_pDataThread->PingThread(500))
	{
		sprintf(m_LogString, "数据缓存线程[%ld]启动成功", m_pDataThread->GetId());
		MessShow(m_LogString, LTINFO);
		m_pDataThread->Event();
		Sleep(500);
	}
	else
	{
		sprintf(m_LogString, "数据缓存线程启动失败");
		MessShow(m_LogString, LTINFO);
		return -1;
	}

	return 0;
}

//************************************
// Method:    StartMonitorTh
// FullName:  CLfoAnaObj::StartMonitorTh
// Access:    public 
// Returns:   int
// Qualifier:
// 启动监视线程MonitorThread()
// 输入：监视线程状态结构，监视线程类结构
// 输出：监视线程状态结构
//************************************
int CLfoAnaObj::StartMonitorTh()
{
	int ii, nFlag = 0;
	for (ii = 0; ii < g_Thread_nun; ii++)
	{
		m_pLfoThread[ii].Start();
		if (m_pLfoThread[ii].PingThread(500))
		{
			if (ii == (g_Thread_nun - 1))
			{//记录最高频段的线程ID，该线程读文件时需记录文件位置
				g_pos_threadid = m_pLfoThread[ii].GetId();
			}
			sprintf(m_LogString, "监视线程[%ld]启动成功", m_pLfoThread[ii].GetId());
			MessShow(m_LogString, LTINFO);
			m_pLfoThread[ii].Event();
			Sleep(500);
		}
		else
		{
			nFlag++;
			sprintf(m_LogString, "监视线程启动失败");
			MessShow(m_LogString, LTINFO);
		}
	}

	if (nFlag > 0)
	{
		return -1;
	}

	return 0;
}

//************************************
// Method:    ThreadCallBack
// FullName:  CLfoAnaObj::ThreadCallBack
// Access:    public 
// Returns:   int
// Qualifier:
// 线程回调函数
//************************************
int CLfoAnaObj::ThreadCallBack(vector<LFO_DEV_INFO>& VecDev, vector<LFO_MODE_SHAPE>& VecModeShape, LFO_SYS_INFO& MonSys, LFO_PARA_STRUCT& LfoPara, const int threadidx, LFO_SYS_INFO& oscMonSys)
{
	sprintf(m_LogString, "线程回调%d", threadidx + 1);
	MessShow(m_LogString);
	char file1[MAXBUF], flie2[MAXBUF];
	int casefileno;
	ShowProMem("回调开始");
	int lfo = -1;
	m_pLfoThread[threadidx].m_mutex.Lock();
	ClearLfoDev(m_pLfoThreadInfo[threadidx].VecDev);
	ClearLfoDev(m_pLfoThreadInfo[threadidx].MonSys.vec_dev);
	ClearLfoDev(m_pLfoThreadInfo[threadidx].MonSys.cen_dev);
	ClearLfoModeShape(m_pLfoThreadInfo[threadidx].MonSys.vec_modeshape);
	ClearLfoSysInfo(m_pLfoThreadInfo[threadidx].MonSys);
	ShowProMem("清空结构");

	// 	ClearLfoDev(m_pLfoThreadInfo[threadidx].oscMonSys.vec_dev);
	// 	ClearLfoModeShape(m_pLfoThreadInfo[threadidx].oscMonSys.vec_modeshape);
	// 	ClearLfoSysInfo(m_pLfoThreadInfo[threadidx].oscMonSys);

		// 更新主线程中该线程对应的设备状态，系统状态
	// 	m_pLfoThreadInfo[threadidx].VecDev = VecDev; ///???????????
	CopyLfoDev(m_pLfoThreadInfo[threadidx].VecDev, VecDev);
	//m_pLfoThreadInfo[threadidx].VecModeShape = VecModeShape;
// 	m_pLfoThreadInfo[threadidx].MonSys = MonSys;
	CopyLfoSysInfo(m_pLfoThreadInfo[threadidx].MonSys, MonSys);
	// 	m_pLfoThreadInfo[threadidx].MonSys.file_end = MonSys.file_end;

	// 	m_pLfoThreadInfo[threadidx].oscMonSys = oscMonSys;
	ShowProMem("结构赋值");

	//m_pLfoThreadInfo[threadidx].m_LfoPara = LfoPara;
	// 案例数据曲线显示
	m_LfoPara.data_pos = LfoPara.data_pos;
	m_LfoPara.file_start_time = LfoPara.file_start_time;
	m_LfoPara.file_end_time = LfoPara.file_end_time;
	// 	sprintf(m_LogString,"线程[%d]设备总数[%d]",threadidx+1,m_pLfoThreadInfo[threadidx].VecDev.size());
	// 	MessShow(m_LogString);
	sprintf(m_LogString, "线程[%d]模式数[%d],设备数[%d],主导设备[%s]", threadidx + 1, m_pLfoThreadInfo[threadidx].MonSys.vec_dev.size(), m_pLfoThreadInfo[threadidx].MonSys.vec_modeshape.size(), m_pLfoThreadInfo[threadidx].MonSys.lead_dev.dev.dev_name);
	MessShow(m_LogString);
	m_pLfoThread[threadidx].m_mutex.Unlock();


	InitProc();

	ShowProMem("InitProc");
	// 更新参数
	if (GetLfoPara(m_LfoPara, 1) < 0)
	{
		sprintf(m_LogString, "参数表读取失败,采用默认参数");
		MessShow(m_LogString, LTERROR);
	}
	ShowProMem("GetLfoPara");


	if (CheckPara(m_LfoPara, -1) < 0)
	{
		sprintf(m_LogString, "参数检查错误,采用默认参数");
		MessShow(m_LogString, LTERROR);
	}
	ShowProMem("CheckPara");

	// 更新主线程中汇总状态
	if (UpdateMonDev(m_VecDev, m_MonSys, threadidx) < 0)
	{
		sprintf(m_LogString, "更新主线程中振荡设备状态失败");
		MessShow(m_LogString, LTERROR);
		// return -1;
	}
	ShowProMem("UpdateMonDev");
	// 	for (int kkk = 0;kkk< m_VecDev.size();kkk++)
	// 	{
	// 		if (m_VecDev[kkk].dev.dev_type != MENU_LFO_DEV_ITFC)
	// 		{
	// 			sprintf(m_LogString,"线程[%d][%s]设备是否振荡源[%d]",threadidx+1,m_VecDev[kkk].dev.dev_name,m_VecDev[kkk].mode.is_source);
	// 			MessShow(m_LogString,LTERROR);
	// 		}
	// 	}
		// 更新主线程中小扰动统计汇总状态
	// 	if (UpdateOscMonDev(m_VecDev,m_oscMonSys,threadidx)<0)
	// 	{
	// 		sprintf(m_LogString,"更新主线程中小扰动统计设备状态失败");
	// 		MessShow(m_LogString,LTERROR);
	// 		// return -1;
	// 	}

		// 更新EMS告警开关
	SetEmsStat(m_MonSys);

	// 根据系统状态置结果保存标志
	m_MonSys.save_flag = LFO_SAVE_FLAG_IDEL;
	if (m_MonSys.stat_old <= MENU_LFO_DEV_STS_SCT
		&& m_MonSys.stat >= MENU_LFO_DEV_STS_WRN)
	{
		sprintf(m_LogString, "开始振荡========");
		MessShow(m_LogString, LTINFO);
		// 安全->告警,振荡开始
		m_MonSys.save_flag = LFO_SAVE_FLAG_START;
		// 发送告警消息
		SendWarnMessage(m_MonSys, LFO_UNSAVE_WARN, 0, 0, 0);
		SendWarnToIA(m_MonSys, m_MonSys.save_flag);
		if (m_LfoPara.lfo_warn == 1)
		{
			m_MonSys.lfo_warn_flag = 0;
			lfo = JurgeLFO(m_MonSys, m_MonSys.lfo_warn_flag);
			if (lfo == 1)
			{
				m_MonSys.lfo_type = 1;
				sprintf(m_LogString, "满足低频振荡判据,发送低频振荡告警217,%f", MonSys.lead_dev.mode.freq);
				MessShow(m_LogString, LTINFO);
				SendWarnMessage(m_MonSys, LFO_UNSAVE_WARN, 0, 0, 1);					
				m_MonSys.lfo_warn_flag = 1;
			}
			else if (lfo == 2)
			{
				m_MonSys.lfo_type = 2;
				sprintf(m_LogString, "满足大幅功率波动判据,发送大幅功率波动告警217,%f", MonSys.lead_dev.mode.freq);
				MessShow(m_LogString, LTINFO);
				SendWarnMessage(m_MonSys, LFO_UNSAVE_WARN, 0, 0, 1);				
				m_MonSys.lfo_warn_flag = 1;
			}
		}

		// 调用曲线控件显示振荡曲线
		ShowWamsCurve(m_MonSys);
	}
	else if (m_MonSys.stat_old >= MENU_LFO_DEV_STS_WRN && m_MonSys.stat >= MENU_LFO_DEV_STS_WRN)
	{
		sprintf(m_LogString, "持续振荡========");
		MessShow(m_LogString, LTINFO);
		// 告警->告警,振荡持续
		m_MonSys.save_flag = LFO_SAVE_FLAG_HOLD;
		if (m_LfoPara.lfo_warn == 1)
		{
			if (m_MonSys.lfo_warn_flag == 0)
			{
				lfo = JurgeLFO(m_MonSys, m_MonSys.lfo_warn_flag);
				if (lfo == 1)
				{
					m_MonSys.lfo_type = 1;
					sprintf(m_LogString, "满足低频振荡判据,发送低频振荡告警217");
					MessShow(m_LogString, LTINFO);
					SendWarnMessage(m_MonSys, LFO_UNSAVE_WARN, 0, 0, 1);	
					m_MonSys.lfo_warn_flag = 1;
				}
				else if (lfo == 2)
				{
					m_MonSys.lfo_type = 2;
					sprintf(m_LogString, "满足大幅功率波动判据,发送大幅功率波动告警217");
					MessShow(m_LogString, LTINFO);
					SendWarnMessage(m_MonSys, LFO_UNSAVE_WARN, 0, 0, 1);	
					m_MonSys.lfo_warn_flag = 1;
				}
			}
			if (m_MonSys.lfo_warn_flag == 1)
			{
				if (JurgeLFO(m_MonSys, m_MonSys.lfo_warn_flag) == 2)
				{
					sprintf(m_LogString, "低频振荡告警结束,发送低频振荡结束告警217");
					MessShow(m_LogString, LTINFO);
					SendWarnMessage(m_MonSys, LFO_SAVE_WARN, m_LfoCase.end_time, 0, 1);						
					m_MonSys.lfo_type = 0;
					m_MonSys.lfo_warn_flag = 2;
				}
			}
		}
	}
	else if (m_MonSys.stat_old >= MENU_LFO_DEV_STS_WRN && m_MonSys.stat <= MENU_LFO_DEV_STS_SCT)
	{
		sprintf(m_LogString, "结束振荡========");
		MessShow(m_LogString, LTINFO);
		// 告警->安全,振荡结束
		if (m_MonSys.lfo_warn_flag == 1)//振荡过程中发送过低频振荡开始告警,但是未发送过结束告警
		{
			sprintf(m_LogString, "结束振荡,补充发送低频振荡结束告警217");
			MessShow(m_LogString, LTINFO);
			SendWarnMessage(m_MonSys, LFO_SAVE_WARN, m_LfoCase.end_time, 0, 1);
		}
		m_MonSys.lfo_type = 0;
		m_MonSys.lfo_warn_flag = 0;
		m_MonSys.save_flag = LFO_SAVE_FLAG_END;
		SendWarnToIA(m_MonSys, m_MonSys.save_flag);
		// 		m_MonSys.warn_thread = threadidx;//结束振荡时,需刷新曲线
	}
	else if (m_MonSys.stat_old <= MENU_LFO_DEV_STS_SCT && m_MonSys.stat <= MENU_LFO_DEV_STS_SCT)
	{
		// 安全->安全,空闲
		m_MonSys.save_flag = LFO_SAVE_FLAG_IDEL;
		m_MonSys.lfo_warn_flag = 0;
		sprintf(m_LogString, "未振荡========");
		MessShow(m_LogString, LTINFO);

	}
	else
	{
		// 标志错误
		m_MonSys.save_flag = LFO_SAVE_FLAG_IDEL;
	}

	m_MonSys.stat_old = m_MonSys.stat;

	if (CacheWindowsData(m_MonSys, m_VecLfoWindow) < 0)
	{
		sprintf(m_LogString, "缓存每个窗口分析结果错误");
		MessShow(m_LogString, LTERROR);
	}
	ShowProMem("CacheWindowsData");

	// 	if (CacheOscWindowsData(m_oscMonSys,m_VecOscWindow)<0)
	// 	{
	// 		sprintf(m_LogString,"缓存小扰动统计每个窗口分析结果错误");
	// 		MessShow(m_LogString,LTERROR);
	// 	}

		//     5.7	过程信息处理
	//	sprintf(m_LogString,"[yjj]振荡标志1[%d]["INT_REC_FMT"]",m_MonSys.save_flag,m_LfoCase.start_time);
	//	MessShow(m_LogString,LTERROR);	
	if (FormLfoCaseResult(m_VecLfoWindow, m_LfoCase, m_MonSys.save_flag) < 0)
	{
		return -1;
	}

#ifndef _LFO_HIS_LIB_
	//	sprintf(m_LogString,"[yjj]振荡标志2[%d]["INT_REC_FMT"]",m_MonSys.save_flag,m_LfoCase.start_time);
	//	MessShow(m_LogString,LTERROR);	
	// 	sprintf(m_LogString,"小扰动统计系统状态[%d][%d]",m_oscMonSys.stat,m_oscMonSys.stat_old);
	// 	MessShow(m_LogString,LTERROR);
	// // 根据系统小扰动统计状态置结果保存标志
	//     m_oscMonSys.save_flag = LFO_SAVE_FLAG_IDEL;
	//     if (m_oscMonSys.stat_old == MENU_LFO_DEV_STS_SCT
	//         && m_oscMonSys.stat >= MENU_LFO_DEV_STS_WRN)
	//     {
	//         // 安全->告警,小扰动开始
	// 		sprintf(m_LogString,"小扰动统计开始======");
	// 		MessShow(m_LogString,LTERROR);
	//         m_oscMonSys.save_flag = LFO_SAVE_FLAG_START;
	//     }
	//     else if (m_oscMonSys.stat_old >= MENU_LFO_DEV_STS_WRN
	//         && m_oscMonSys.stat >= MENU_LFO_DEV_STS_WRN)
	//     {
	// 		sprintf(m_LogString,"小扰动统计持续======");
	// 		MessShow(m_LogString,LTERROR);
	//         // 告警->告警,小扰动统计持续
	//         m_oscMonSys.save_flag = LFO_SAVE_FLAG_HOLD;
	//     }
	//     else if (m_oscMonSys.stat_old >= MENU_LFO_DEV_STS_WRN
	//         && m_oscMonSys.stat <= MENU_LFO_DEV_STS_SCT)
	//     {
	// 		// 告警->安全,振荡结束
	// 		sprintf(m_LogString,"小扰动统计结束======");
	// 		MessShow(m_LogString,LTERROR);
	//         m_oscMonSys.save_flag = LFO_SAVE_FLAG_END;
	//     }
	//     else if (m_oscMonSys.stat_old == MENU_LFO_DEV_STS_SCT
	//         && m_oscMonSys.stat == MENU_LFO_DEV_STS_SCT)
	//     {
	//         // 安全->安全,空闲
	// 		sprintf(m_LogString,"未进行小扰动统计======");
	// 		MessShow(m_LogString,LTERROR);
	//         m_oscMonSys.save_flag = LFO_SAVE_FLAG_IDEL;
	//     }
	//     else
	//     {
	//         // 标志错误
	//         m_oscMonSys.save_flag = LFO_SAVE_FLAG_IDEL;
	//     }

	// 	m_oscMonSys.stat_old = m_oscMonSys.stat;
	// 	if (FormOscCaseResult(m_VecOscWindow,m_oscLfoCase,m_oscMonSys.save_flag)<0)
	// 	{
	// 		sprintf(m_LogString,"形成小扰动统计结果错误");
	// 		MessShow(m_LogString,LTERROR);
	// 		//		return -1;
	// 	}

		// 低频振荡频率段或当前告警频率段更新设备曲线
	sprintf(m_LogString, "当前系统状态[%d],刷新曲线线程[%d],本次回调线程[%d]", m_MonSys.stat, m_MonSys.warn_thread + 1, threadidx + 1);
	MessShow(m_LogString);
	if (threadidx == m_MonSys.warn_thread)
	{
		if (SaveLfoCurve(m_VecDev, m_MonSys) < 0)
		{
			sprintf(m_LogString, "更新LPO实时曲线表失败");
			MessShow(m_LogString, LTERROR);
		}
		else
		{
			//     5.8	实时结果保存
			if (SaveMonResult(m_VecDev, m_MonSys) < 0)
			{
				return -1;
			}
			// 			FormMonModeCurce(); 
			// 			UpdateMonModeCurce( m_pLfoModeCurve );
		}
		ShowProMem("刷新曲线");

		// 		if (SavePowCurve(m_LfoCase)<0)
		// 		{
		// 			sprintf(m_LogString,"更新耗散功率曲线失败");
		// 			MessShow(m_LogString,LTERROR);
		// 		}
	}
	// 过程信息保存
	if (SaveLfoResult(m_LfoCase) < 0)
	{
	}
	ShowProMem("SaveLfoResult");

	// 	m_MonSys.file_end = 1;
	// 	for (int ii = 0;ii < g_Thread_nun;ii++)
	// 	{
	// 		m_MonSys.file_end = m_MonSys.file_end * m_pLfoThreadInfo[ii].MonSys.file_end;
	// 	}
	if (m_MonSys.file_end == 1 && m_LfoPara.data_source != LFO_DATA_SOURCE_REALTTIME)
	{
		sprintf(m_LogString, "案例文件读取完毕");
		MessShow(m_LogString);
		file_end_num++;
		casefileno = 0;

		casefileno = file_end_num % 3;

		sprintf(file1, "%s%sdata%send.fin",
			m_maindir.c_str(), m_backslash, m_backslash);
		sprintf(m_LogString, "案例结束标志文件[%s]", file1);
		MessShow(m_LogString, LTINFO);
		fstream fs;
		fs.open(file1, std::ios::out);
		if (fs.fail())
		{
			fs.close();
			sprintf(m_LogString, "打开案例结束标志文件[%s]失败", file1);
			MessShow(m_LogString, LTINFO);
		}
		else
		{
			fs << "案例读取结束" << endl;
			fs.close();
		}

		// 		sprintf(m_LogString,"替换文件 THREAD<%ld>",m_pLfoThread[threadidx].GetId()); 		
		// 		PT_DelaySecond(5);
		ShowProMem("替换文件");
		int times = 0;
		while (true)
		{
			if (access(file1, 0) == 0)
			{
				sprintf(m_LogString, "案例结束标志文件[%s]仍在,等待拷贝文件", file1);
				MessShow(m_LogString, LTINFO);
				PT_DelaySecond(2);
				times++;
				if (times >= 15)
				{
					times = 0;
					remove(file1);
					break;
				}
				else
				{
					continue;
				}
			}
			else
			{
				sprintf(m_LogString, "案例结束标志文件[%s]不在,拷贝文件完成", file1);
				MessShow(m_LogString, LTINFO);
				break;
			}
		}
		for (int ii = 0; ii < g_Thread_nun; ii++)
		{
			m_pLfoThread[ii].m_MonSys.file_end = 0;
			m_pLfoThreadInfo[ii].MonSys.file_end = 0;
		}
		memset((char*)m_pLfoCurve, 0, sizeof(LPO_RT_CURVE_WAMS));
		g_vec_pos_time.clear();
		vector<Pos_Time>().swap(g_vec_pos_time);
		m_LfoPara.data_pos_old = 0;
	}
	ShowProMem("回调结束");
#endif

	// 	if (m_MonSys.save_flag == LFO_SAVE_FLAG_HOLD || m_MonSys.save_flag == LFO_SAVE_FLAG_START)
	// 	{//振荡期间,将机组分群结果入库
	// 		UpdateAnaGnTable(m_VecDev);
	// 	}
	// 
	return 0;
}

// int CLfoAnaObj::UpdateThreadInfo( const int threadidx )
// {
// 	m_pLfoThreadInfo[threadidx]
// 	return 0;
// }

//************************************
// Method:    UpdateMonDev
// FullName:  CLfoAnaObj::UpdateMonDev
// Access:    public 
// Returns:   int
// Qualifier:
// 根据各个线程设备状态，更新主进程中设备状态
// 同一时刻，只有更新一个线程状态
// TODO:若多个线程同时告警，如何处理？
// 
//************************************
int CLfoAnaObj::UpdateMonDev(vector<LFO_DEV_INFO>& VecDev, LFO_SYS_INFO& MonSys, const int threadidx)
{
	int ii, jj, kk, ll, nFlag;
	int file_end = 1;
	MonSys.file_end = 0;

	// 对系统状态，若有一个频率段系统状态为告警，则汇总后系统状态即为告警。当所有频率段都为安全时，汇总后系统状态为安全。
	for (ii = 0; ii < g_Thread_nun; ii++)
	{
		// 对监视设备信息，若有一个频率段设备状态为告警，则汇总后该设备状态为告警。当所有频率段都为安全时，汇总后设备状态为安全。
// 		sprintf(m_LogString,"[yjj]UpdateMonDev进程[%d],设备数[%d],VecDev.size()=[%d]",ii,m_pLfoThreadInfo[ii].VecDev.size(),(int)VecDev.size());
// 		MessShow(m_LogString,LTINFO);
		for (jj = 0; jj < (int)VecDev.size(); jj++)
		{
			for (kk = 0; kk < (int)m_pLfoThreadInfo[ii].VecDev.size(); kk++)
			{
				if (VecDev[jj].dev.dev_id == m_pLfoThreadInfo[ii].VecDev[kk].dev.dev_id)
				{
					if (m_pLfoThreadInfo[ii].VecDev[kk].stat > VecDev[jj].stat)
					{
						VecDev[jj].stat = m_pLfoThreadInfo[ii].VecDev[kk].stat;
						if (VecDev[jj].dev.dev_type == MENU_LFO_DEV_ITFC)
						{
							VecDev[jj].is_sm_md = m_pLfoThreadInfo[ii].VecDev[kk].is_sm_md;
							sprintf(VecDev[jj].intline_des, "%s", m_pLfoThreadInfo[ii].VecDev[kk].intline_des);
							// 							sprintf(m_LogString,"线程[%d][%s]组成设备[%s]是否同调[%d]",ii+1,VecDev[jj].dev.dev_name,VecDev[jj].intline_des,VecDev[jj].is_sm_md);
							// 							MessShow(m_LogString,LTINFO);
						}
					}
					//  					sprintf(m_LogString,"[yjj]UpdateMonDev能量[%f],[%f]",VecDev[jj].mode.engy,m_pLfoThreadInfo[ii].VecDev[kk].mode.engy);
					//  					MessShow(m_LogString,LTINFO);
					if (VecDev[jj].mode.engy < m_pLfoThreadInfo[ii].VecDev[kk].mode.engy)
					{
						// 若该设备存在多个频率段告警，则保存能量最大的模式。
						CopyLfoModeInfo(VecDev[jj].mode, m_pLfoThreadInfo[ii].VecDev[kk].mode);
						// 						VecDev[jj].mode = m_pLfoThreadInfo[ii].VecDev[kk].mode;
					}

					if (VecDev[jj].curve.max_ampl < m_pLfoThreadInfo[ii].VecDev[kk].curve.max_ampl)
					{
						// 对功率波动值，取各频率段功率波动值中的最大值。
						VecDev[jj].curve = m_pLfoThreadInfo[ii].VecDev[kk].curve;
					}
					// 同一设备模式取并集,非空即增加
					if (!m_pLfoThreadInfo[ii].VecDev[kk].vec_mode.empty())
					{
						for (ll = 0; ll < (int)m_pLfoThreadInfo[ii].VecDev[kk].vec_mode.size(); ll++)
						{
							VecDev[jj].vec_mode.push_back(m_pLfoThreadInfo[ii].VecDev[kk].vec_mode[ll]);
						}
					}
					// 同一设备告警模式取并集
					if (!m_pLfoThreadInfo[ii].VecDev[kk].vec_lead_mode.empty())
					{
						for (ll = 0; ll < (int)m_pLfoThreadInfo[ii].VecDev[kk].vec_lead_mode.size(); ll++)
						{
							VecDev[jj].vec_lead_mode.push_back(m_pLfoThreadInfo[ii].VecDev[kk].vec_lead_mode[ll]);
						}
					}
					break;
				}
			}
		}

		if (m_pLfoThreadInfo[0].MonSys.stat >= MENU_LFO_DEV_STS_WRN
			&& m_pLfoThreadInfo[1].MonSys.stat >= MENU_LFO_DEV_STS_WRN
			&& m_pLfoThreadInfo[2].MonSys.stat >= MENU_LFO_DEV_STS_WRN
			&& m_pLfoThreadInfo[3].MonSys.stat >= MENU_LFO_DEV_STS_WRN)
		{
			// 三频段均告警,取最近更新状态
			MonSys.start_time = m_pLfoThreadInfo[threadidx].MonSys.start_time;
			MonSys.end_time = m_pLfoThreadInfo[threadidx].MonSys.end_time;
			MonSys.gps_time = m_pLfoThreadInfo[threadidx].MonSys.gps_time;
			MonSys.window_id = m_pLfoThreadInfo[threadidx].MonSys.window_id;
			MonSys.freq_min = m_pLfoThreadInfo[threadidx].MonSys.freq_min;
			MonSys.freq_max = m_pLfoThreadInfo[threadidx].MonSys.freq_max;
			sprintf(m_LogString, "线程[%d]lead_dev[%s],阻尼比[%f],系统lead_dev[%s],阻尼比[%f]", ii + 1, m_pLfoThreadInfo[threadidx].MonSys.lead_dev.dev.dev_name, m_pLfoThreadInfo[threadidx].MonSys.lead_dev.mode.damp, MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.damp);
			MessShow(m_LogString, LTINFO);
			if (m_pLfoThreadInfo[threadidx].MonSys.lead_dev.mode.damp < MonSys.lead_dev.mode.damp)
			{
				CopyLfoDev(MonSys.lead_dev, m_pLfoThreadInfo[threadidx].MonSys.lead_dev);
				CopyLfoDev(MonSys.lead_gen, m_pLfoThreadInfo[threadidx].MonSys.lead_gen);
			}
			MonSys.stat = m_pLfoThreadInfo[threadidx].MonSys.stat;
			MonSys.lfo_ln_num = m_pLfoThreadInfo[threadidx].MonSys.lfo_ln_num;
			MonSys.lfo_gn_num = m_pLfoThreadInfo[threadidx].MonSys.lfo_gn_num;
			MonSys.lfo_tr_num = m_pLfoThreadInfo[threadidx].MonSys.lfo_tr_num;
			MonSys.lfo_inf_num = m_pLfoThreadInfo[threadidx].MonSys.lfo_inf_num;
			if (strlen(m_pLfoThreadInfo[ii].MonSys.center_desc) != 0)
			{
				sprintf(MonSys.center_desc, "%s", m_pLfoThreadInfo[threadidx].MonSys.center_desc);
			}
			if (strlen(m_pLfoThreadInfo[ii].MonSys.lfos_des) != 0)
			{
				sprintf(MonSys.lfos_des, "%s", m_pLfoThreadInfo[threadidx].MonSys.lfos_des);
			}
			if (g_Thread_nun == 1)
			{
				MonSys.warn_thread = 0;
			}
			else
			{
				MonSys.warn_thread = 2;//所有线程段都告警 使用0.2~2.5Hz线程
			}
		}
		else
		{
			// TODO:2段告警暂不考虑
			// 1段告警或四段均安全，取状态最大段
			// 若四个频率段均安全,则取低频振荡频率段
			if (m_pLfoThreadInfo[ii].MonSys.stat >= MonSys.stat)
			{
				MonSys.start_time = m_pLfoThreadInfo[ii].MonSys.start_time;
				MonSys.end_time = m_pLfoThreadInfo[ii].MonSys.end_time;
				MonSys.gps_time = m_pLfoThreadInfo[ii].MonSys.gps_time;
				MonSys.window_id = m_pLfoThreadInfo[ii].MonSys.window_id;
				MonSys.freq_min = m_pLfoThreadInfo[ii].MonSys.freq_min;
				MonSys.freq_max = m_pLfoThreadInfo[ii].MonSys.freq_max;
				sprintf(m_LogString, "线程[%d]lead_dev[%s],阻尼比[%f],系统lead_dev[%s],阻尼比[%f]", ii + 1, m_pLfoThreadInfo[ii].MonSys.lead_dev.dev.dev_name, m_pLfoThreadInfo[ii].MonSys.lead_dev.mode.damp, MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.damp);
				MessShow(m_LogString, LTINFO);
				if (m_pLfoThreadInfo[ii].MonSys.lead_dev.mode.damp < MonSys.lead_dev.mode.damp)
				{
					CopyLfoDev(MonSys.lead_dev, m_pLfoThreadInfo[ii].MonSys.lead_dev);
					CopyLfoDev(MonSys.lead_gen, m_pLfoThreadInfo[ii].MonSys.lead_gen);
					MonSys.warn_thread = ii;
				}

				MonSys.stat = m_pLfoThreadInfo[ii].MonSys.stat;
				MonSys.lfo_ln_num = m_pLfoThreadInfo[ii].MonSys.lfo_ln_num;
				MonSys.lfo_gn_num = m_pLfoThreadInfo[ii].MonSys.lfo_gn_num;
				MonSys.lfo_tr_num = m_pLfoThreadInfo[ii].MonSys.lfo_tr_num;
				MonSys.lfo_inf_num = m_pLfoThreadInfo[ii].MonSys.lfo_inf_num;
				if (strlen(m_pLfoThreadInfo[ii].MonSys.center_desc) != 0)
				{
					sprintf(MonSys.center_desc, "%s", m_pLfoThreadInfo[ii].MonSys.center_desc);
				}
				if (strlen(m_pLfoThreadInfo[ii].MonSys.lfos_des) != 0)
				{
					sprintf(MonSys.lfos_des, "%s", m_pLfoThreadInfo[ii].MonSys.lfos_des);
				}
			}
			if (MonSys.stat <= MENU_LFO_DEV_STS_SCT)
			{
				if (g_Thread_nun == 1)
				{
					MonSys.warn_thread = 0;
				}
				else if (g_Thread_nun == 2)
				{
					MonSys.warn_thread = 1;
				}
				else
				{
					MonSys.warn_thread = 2;
				}
			}
		}

		// 振荡设备取并集，设备可重复
// 		sprintf(m_LogString,"线程[%d],设备数[%d]",ii+1,m_pLfoThreadInfo[ii].MonSys.vec_dev.size());
// 		MessShow(m_LogString,LTINFO);
		for (jj = 0; jj < (int)m_pLfoThreadInfo[ii].MonSys.vec_dev.size(); jj++)
		{
			nFlag = 0;
			for (kk = 0; kk < (int)MonSys.vec_dev.size(); kk++)
			{
				// 找到对应设备
				if (MonSys.vec_dev[kk].dev.dev_id == m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].dev.dev_id)
				{
					// 波形取最大值
					if (MonSys.vec_dev[kk].curve.max_ampl < m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].curve.max_ampl)
					{
						MonSys.vec_dev[kk].curve = m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].curve;
					}
					// 取能量最大模式
					if (MonSys.vec_dev[kk].mode.engy < m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode.engy)
					{
						sprintf(m_LogString, "线程[%d],设备[%s]能量[%f]较大,是否振荡源[%d],参与因子[%f]", ii + 1, m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].dev.dev_name, m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode.engy, m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode.is_source, m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode.fact);
						MessShow(m_LogString, LTINFO);
						CopyLfoModeInfo(MonSys.vec_dev[kk].mode, m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode);
						// 						MonSys.vec_dev[kk].mode = m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode;
					}
					// 同一设备模式取并集,非空即增加
					if (!m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_mode.empty())
					{
						for (ll = 0; ll < (int)m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_mode.size(); ll++)
						{
							MonSys.vec_dev[kk].vec_mode.push_back(m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_mode[ll]);
						}
					}
					// 同一设备告警模式取并集
					if (!m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_lead_mode.empty())
					{
						for (ll = 0; ll < (int)m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_lead_mode.size(); ll++)
						{
							MonSys.vec_dev[kk].vec_lead_mode.push_back(m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_lead_mode[ll]);
						}
					}
					nFlag = 1;
					break;
				}
			}
			if (nFlag == 0)
			{
				// 未找到设备，新增
				MonSys.vec_dev.push_back(m_pLfoThreadInfo[ii].MonSys.vec_dev[jj]);
				// 				sprintf(m_LogString,"线程[%d],新增设备设备[%s]能量[%f]较大,是否振荡源[%d]",ii+1,m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].dev.dev_name,m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode.engy,m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode.is_source);
				// 				MessShow(m_LogString,LTINFO);
			}
		}


		for (jj = 0; jj < (int)m_pLfoThreadInfo[ii].MonSys.cen_dev.size(); jj++)
		{
			if (m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].dev.dev_type != MENU_LFO_DEV_FAC)
			{
				continue;
			}
			nFlag = 0;
			for (kk = 0; kk < (int)MonSys.cen_dev.size(); kk++)
			{
				if (MonSys.cen_dev[kk].dev.dev_type != MENU_LFO_DEV_FAC)
				{
					continue;
				}
				// 找到对应设备
				if (MonSys.cen_dev[kk].dev.dev_id == m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].dev.dev_id)
				{
					// 波形取最大值
					if (MonSys.cen_dev[kk].curve.max_ampl < m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].curve.max_ampl)
					{
						MonSys.cen_dev[kk].curve = m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].curve_ana;
					}
					// 取能量最大模式
					if (MonSys.cen_dev[kk].mode.engy < m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].mode.engy)
					{
						CopyLfoModeInfo(MonSys.cen_dev[kk].mode, m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].mode);
						// 						MonSys.cen_dev[kk].mode = m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].mode;
					}
					// 同一设备模式取并集,非空即增加
					if (!m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].vec_mode.empty())
					{
						for (ll = 0; ll < (int)m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].vec_mode.size(); ll++)
						{
							MonSys.cen_dev[kk].vec_mode.push_back(m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].vec_mode[ll]);
						}
					}
					// 同一设备告警模式取并集
					if (!m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].vec_lead_mode.empty())
					{
						for (ll = 0; ll < (int)m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].vec_lead_mode.size(); ll++)
						{
							MonSys.cen_dev[kk].vec_lead_mode.push_back(m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].vec_lead_mode[ll]);
						}
					}
					nFlag = 1;
					break;
				}
			}
			if (nFlag == 0)
			{
				// 未找到设备，新增
				MonSys.cen_dev.push_back(m_pLfoThreadInfo[ii].MonSys.cen_dev[jj]);
				// 				sprintf(m_LogString,"线程[%d],新增振荡中心设备[%s]",ii+1,m_pLfoThreadInfo[ii].MonSys.cen_dev[jj].dev.dev_name);
				// 				MessShow(m_LogString,LTINFO);
			}
		}

		// 振荡模态取并集,频率段不同，模式定不同，直接取并集
// 		sprintf(m_LogString,"线程[%d],模式数[%d]",ii+1,m_pLfoThreadInfo[ii].MonSys.vec_modeshape.size());
// 		MessShow(m_LogString,LTINFO);
		for (jj = 0; jj < (int)m_pLfoThreadInfo[ii].MonSys.vec_modeshape.size(); jj++)
		{
			sprintf(m_LogString, "合并模式频率[%f],振幅[%f],能量[%f],状态[%d]", m_pLfoThreadInfo[ii].MonSys.vec_modeshape[jj].mode.freq, m_pLfoThreadInfo[ii].MonSys.vec_modeshape[jj].mode.ampl, m_pLfoThreadInfo[ii].MonSys.vec_modeshape[jj].mode.engy, m_pLfoThreadInfo[ii].MonSys.vec_modeshape[jj].mode.stat);
			MessShow(m_LogString, LTINFO);
			MonSys.vec_modeshape.push_back(m_pLfoThreadInfo[ii].MonSys.vec_modeshape[jj]);
		}

		//只要有一个线程的file_end为零,则系统的file_end置为零;所有线程的file_end都为1,则系统的file_end置为1;
// 		sprintf(m_LogString,"线程[%d],文件标志[%d]",ii+1,m_pLfoThreadInfo[ii].MonSys.file_end);
// 		MessShow(m_LogString,LTINFO);
		file_end = file_end * m_pLfoThreadInfo[ii].MonSys.file_end;
		// 		// 若三个频率段均安全,则取第三个频率段的结果(默认将低频振荡频率段作为第三个频率段)
		// 		if (m_pLfoThreadInfo[ii].MonSys.stat >= MonSys.stat)
		// 		{
		// 			if (m_pLfoThreadInfo[0].MonSys.stat >= MENU_LFO_DEV_STS_WRN 
		// 				&& m_pLfoThreadInfo[1].MonSys.stat >= MENU_LFO_DEV_STS_WRN 
		// 				&& m_pLfoThreadInfo[2].MonSys.stat >= MENU_LFO_DEV_STS_WRN )
		// 			{
		// 				// 三频段均告警,取最近更新状态
		// 				MonSys.start_time = m_pLfoThreadInfo[threadidx].MonSys.start_time;
		// 				MonSys.end_time = m_pLfoThreadInfo[threadidx].MonSys.end_time;
		// 				MonSys.gps_time = m_pLfoThreadInfo[threadidx].MonSys.gps_time;
		// 				MonSys.window_id = m_pLfoThreadInfo[threadidx].MonSys.window_id;
		// 				MonSys.freq_min = m_pLfoThreadInfo[threadidx].MonSys.freq_min;
		// 				MonSys.freq_max = m_pLfoThreadInfo[threadidx].MonSys.freq_max;
		// 				MonSys.lead_dev = m_pLfoThreadInfo[threadidx].MonSys.lead_dev;
		// 			}
		// 			else
		// 			{
		// 				// TODO:2段告警暂不考虑
		// 				// 1段告警，状态最大段
		// 				MonSys.start_time = m_pLfoThreadInfo[ii].MonSys.start_time;
		// 				MonSys.end_time = m_pLfoThreadInfo[ii].MonSys.end_time;
		// 				MonSys.gps_time = m_pLfoThreadInfo[ii].MonSys.gps_time;
		// 				MonSys.window_id = m_pLfoThreadInfo[ii].MonSys.window_id;
		// 				MonSys.freq_min = m_pLfoThreadInfo[ii].MonSys.freq_min;
		// 				MonSys.freq_max = m_pLfoThreadInfo[ii].MonSys.freq_max;
		// 				MonSys.lead_dev = m_pLfoThreadInfo[ii].MonSys.lead_dev;
		// 			}
		// 	
		// 			// 取最大状态
		// 			if (m_pLfoThreadInfo[ii].MonSys.stat > MonSys.stat)
		// 			{
		// 				MonSys.stat = m_pLfoThreadInfo[ii].MonSys.stat;
		// 			}
		// 			
		// 			// 振荡设备取并集，设备可重复
		// 			for (jj=0; jj<(int)m_pLfoThreadInfo[ii].MonSys.vec_dev.size(); jj++)
		// 			{
		// 				nFlag = 0;
		// 				for (kk =0; kk<(int)MonSys.vec_dev.size(); kk++)
		// 				{
		// 					// 找到对应设备
		// 					if (MonSys.vec_dev[kk].dev.dev_id == m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].dev.dev_id)
		// 					{
		// 						// 波形取最大值
		// 						if (MonSys.vec_dev[kk].curve.max_ampl < m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].curve.max_ampl)
		// 						{
		// 							MonSys.vec_dev[kk].curve = m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].curve;
		// 						}
		// 						// 取能量最大模式
		// 						if (MonSys.vec_dev[kk].mode.engy < m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode.engy)
		// 						{
		// 							MonSys.vec_dev[kk].mode = m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].mode;
		// 						}
		// 						// 同一设备模式取并集,非空即增加
		// 						if (!m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_mode.empty())
		// 						{
		// 							for (ll =0; ll<(int)m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_mode.size(); ll++)
		// 							{
		// 								MonSys.vec_dev[kk].vec_mode.push_back(m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_mode[ll]);
		// 							}
		// 						}
		// 						// 同一设备告警模式取并集
		// 						if (!m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_lead_mode.empty())
		// 						{
		// 							for (ll =0; ll<(int)m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_lead_mode.size(); ll++)
		// 							{
		// 								MonSys.vec_dev[kk].vec_lead_mode.push_back(m_pLfoThreadInfo[ii].MonSys.vec_dev[jj].vec_lead_mode[ll]);
		// 							}
		// 						}
		// 						nFlag =1;
		// 						break;
		// 					}
		// 				}
		// 				if (nFlag == 0)
		// 				{
		// 					// 未找到设备，新增
		// 					MonSys.vec_dev.push_back(m_pLfoThreadInfo[ii].MonSys.vec_dev[jj]);
		// 				}
		// 			}
		// 
		// 			// 振荡模态取并集,频率段不同，模式定不同，直接取并集
		// 			for (jj=0; jj<(int)m_pLfoThreadInfo[ii].MonSys.vec_modeshape.size(); jj++)
		// 			{
		// 				MonSys.vec_modeshape.push_back(m_pLfoThreadInfo[ii].MonSys.vec_modeshape[jj]);
		// 			}
		// 		}
	}
	sprintf(m_LogString, "选定lead_dev[%s],阻尼比[%f],曲线刷新线程[%d]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.damp, MonSys.warn_thread + 1);
	MessShow(m_LogString, LTINFO);
	MonSys.file_end = file_end;

	// 振荡设备按振幅由大到小排序
	if (!MonSys.vec_dev.empty())
	{
		sort(MonSys.vec_dev.begin(), MonSys.vec_dev.end(), g_SortDevByEngy);
		// 将振幅最大设备作为主导设备
// 		MonSys.lead_dev = MonSys.vec_dev.front();
		// 统计各类振荡设备数量
		MonSys.lfo_gn_num = 0;
		MonSys.lfo_tr_num = 0;
		MonSys.lfo_ln_num = 0;
		MonSys.lfo_inf_num = 0;
		for (ii = 0; ii < (int)MonSys.vec_dev.size(); ii++)
		{
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				MonSys.lfo_gn_num++;
			}
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				MonSys.lfo_ln_num++;
			}
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				MonSys.lfo_tr_num++;
			}
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
			{
				MonSys.lfo_inf_num++;
			}
		}
		if (m_LfoPara.is_add_facname == 1)
		{
			if (MonSys.lead_dev.dev.dev_type == MENU_LFO_DEV_GEN)
			{
				// 增加厂站名称
				if (m_LfoPara.is_eng == 1)
				{
					sprintf(MonSys.stat_desc, "device[%s%s],frequency[%5.3fHz],oscillation power[%6.3fMW]", MonSys.lead_dev.dev.fac_name, MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl);
				}
				else
				{
					sprintf(MonSys.stat_desc, "波动幅度最大设备[%s%s],振荡频率[%5.3fHz],功率波动幅度[%6.2fMW],阻尼比[%6.3f]", MonSys.lead_dev.dev.fac_name, MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_dev.mode.damp);
				}
			}
			else if (MonSys.lead_dev.dev.dev_type == MENU_LFO_DEV_TR)
			{
				if (VALID_INT_REC(MonSys.lead_gen.dev.dev_id))			
				{//有机组但机组不是最严重设备
					// 增加厂站名称
					if (m_LfoPara.is_eng == 1)
					{
						sprintf(MonSys.stat_desc, "device[%s%s],frequency[%5.3fHz],oscillation power[%6.2fMW], generator[%s%s] power[%6.2fMW]", MonSys.lead_dev.dev.fac_name, MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_gen.dev.fac_name, MonSys.lead_gen.dev.dev_name, MonSys.lead_gen.curve.max_ampl);
					}
					else
					{
						sprintf(MonSys.stat_desc, "波动幅度最大设备[%s%s],振荡频率[%5.3fHz],功率波动值[%6.2fMW],阻尼比[%6.2f%],波动幅度最大机组[%s%s],功率波动值[%f]", MonSys.lead_dev.dev.fac_name, MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_dev.mode.damp, MonSys.lead_gen.dev.fac_name, MonSys.lead_gen.dev.dev_name, MonSys.lead_gen.curve.max_ampl);
					}
				}
				else
				{
					// 增加厂站名称
					if (m_LfoPara.is_eng == 1)
					{
						sprintf(MonSys.stat_desc, "device[%s%s],frequency[%5.3fHz],oscillation power[%6.2fMW]", MonSys.lead_dev.dev.fac_name, MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl);
					}
					else
					{
						sprintf(MonSys.stat_desc, "波动幅度最大设备[%s%s],振荡频率[%5.3fHz],功率波动值[%6.2fMW],阻尼比[%6.2f%]", MonSys.lead_dev.dev.fac_name, MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_dev.mode.damp);
					}
				}
			}
			else
			{
				// 线路不加厂站名称
				if (VALID_INT_REC(MonSys.lead_gen.dev.dev_id))		
				{
					// 线路不加厂站名称
					if (m_LfoPara.is_eng == 1)
					{
						sprintf(MonSys.stat_desc, "device[%s],frequency[%5.3fHz],oscillation power[%6.2fMW], generator[%s] power[%6.2fMW]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_gen.dev.dev_name, MonSys.lead_gen.curve.max_ampl);
					}
					else
					{
						sprintf(MonSys.stat_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动值[%6.2fMW],阻尼比[%6.2f%],波动幅度最大机组[%s],功率波动值[%f]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_dev.mode.damp, MonSys.lead_gen.dev.dev_name, MonSys.lead_gen.curve.max_ampl);
					}
				}
				else
				{
					// 线路不加厂站名称
					if (m_LfoPara.is_eng == 1)
					{
						sprintf(MonSys.stat_desc, "device[%s],frequency[%5.3fHz],oscillation power[%6.2fMW]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl);
					}
					else
					{
						sprintf(MonSys.stat_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动值[%6.2fMW],阻尼比[%6.2f%]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_dev.mode.damp);
					}
				}
			}
		}
		else
		{
			if (VALID_INT_REC(MonSys.lead_gen.dev.dev_id))	
			{
				if (m_LfoPara.is_eng == 1)
				{
					sprintf(MonSys.stat_desc, "device[%s],frequency[%5.3fHz],oscillation power[%6.2fMW], generator[%s] power[%6.2fMW]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_gen.dev.dev_name, MonSys.lead_gen.curve.max_ampl);
				}
				else
				{
					sprintf(MonSys.stat_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动值[%6.2fMW],阻尼比[%6.2f%],波动幅度最大机组[%s],功率波动值[%f]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_dev.mode.damp, MonSys.lead_gen.dev.dev_name, MonSys.lead_gen.curve.max_ampl);
				}
			}
			else
			{
				if (m_LfoPara.is_eng == 1)
				{
					sprintf(MonSys.stat_desc, "device[%s],frequency[%5.3fHz],oscillation power[%6.2fMW]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl);
				}
				else
				{
					sprintf(MonSys.stat_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动值[%6.2fMW],阻尼比[%6.2f%]", MonSys.lead_dev.dev.dev_name, MonSys.lead_dev.mode.freq, MonSys.lead_dev.curve.max_ampl, MonSys.lead_dev.mode.damp);
				}
			}
		}
	}

	// 振荡模态按能量由大到小排序
	MonSys.vec_mainmode.clear();
	if (!MonSys.vec_modeshape.empty())
	{
		sort(MonSys.vec_modeshape.begin(), MonSys.vec_modeshape.end(), g_SortModeShapeBystatEngy);//先按状态，再按能量排序
		for (jj = 0; jj < MonSys.vec_modeshape.size(); jj++)
		{
			sprintf(m_LogString, "汇总后模式频率[%f],振幅[%f],能量[%f],单位能量[%f]", MonSys.vec_modeshape[jj].mode.freq, MonSys.vec_modeshape[jj].mode.ampl, MonSys.vec_modeshape[jj].mode.engy, MonSys.vec_modeshape[jj].mode.engy_pre_time);
			MessShow(m_LogString, LTINFO);
			if (MonSys.vec_mainmode.size() >= m_LfoPara.mainmode_num)
			{
				break;
			}
			if (MonSys.vec_modeshape[jj].mode.stat <= MENU_LFO_DEV_STS_SCT)
			{
				if (MonSys.vec_modeshape[jj].mode.engy_pre_time < MonSys.vec_modeshape[0].mode.engy_pre_time * m_LfoPara.mode_save_amplper)
				{
					continue;
				}
			}
			MonSys.vec_mainmode.push_back(MonSys.vec_modeshape[jj]);
		}
	}

	sprintf(m_LogString, "汇总后设备信息[%d]", (int)VecDev.size());
	MessShow(m_LogString, LTINFO);
	// 	for (ii =0; ii<(int)VecDev.size(); ii++)
	// 	{
	// 		sprintf(m_LogString,"%s["INT_REC_FMT"]fdapri[%.3f,%.3f,%.3f,%.3f]振幅[%.2f]状态[%d]",
	// 			VecDev[ii].dev.dev_name,VecDev[ii].dev.dev_id,VecDev[ii].mode.freq,VecDev[ii].mode.damp,VecDev[ii].mode.ampl,
	// 			VecDev[ii].mode.phas,VecDev[ii].curve.max_ampl,VecDev[ii].stat);
	// 		MessShow(m_LogString,LTDEBUG);
	// 	}

	char tmptime1[10], tmptime2[10];
	sprintf(m_LogString, "汇总后窗口信息");
	MessShow(m_LogString, LTINFO);
	sprintf(tmptime1, "%s", PT_GetHMSTimeStringBySeconds(MonSys.start_time));
	sprintf(tmptime2, "%s", PT_GetHMSTimeStringBySeconds(MonSys.end_time));
	sprintf(m_LogString, "windows_id["RTDB_TIME_FMT"]freq[%.3f,%.3f]time["RTDB_TIME_FMT"~"RTDB_TIME_FMT"][%s~%s]fdap[%.3f,%.3f,%.3f,%.3f]stat[%d]desc[%s]",
		MonSys.window_id, MonSys.freq_min, MonSys.freq_max, MonSys.start_time, MonSys.end_time, tmptime1, tmptime2,
		MonSys.lead_dev.mode.freq, MonSys.lead_dev.mode.damp, MonSys.lead_dev.mode.ampl, MonSys.lead_dev.mode.phas, MonSys.stat, MonSys.stat_desc);
	MessShow(m_LogString, LTINFO);
	sprintf(m_LogString, "汇总后振荡设备数:[%d],汇总后模式数:[%d],主要模式数:[%d]", MonSys.vec_dev.size(), MonSys.vec_modeshape.size(), MonSys.vec_mainmode.size());
	MessShow(m_LogString, LTINFO);
	// 	for (ii =0; ii<(int)MonSys.vec_dev.size(); ii++)
	// 	{
	// 		sprintf(m_LogString,"%s["INT_REC_FMT"]fdap[%.3f,%.3f,%.3f,%.3f]振幅[%.2f]状态[%d]",
	// 			MonSys.vec_dev[ii].dev.dev_name,MonSys.vec_dev[ii].dev.dev_id,MonSys.vec_dev[ii].mode.freq,MonSys.vec_dev[ii].mode.damp,
	// 			MonSys.vec_dev[ii].mode.ampl,MonSys.vec_dev[ii].mode.phas,MonSys.vec_dev[ii].curve.max_ampl,MonSys.vec_dev[ii].stat);
	// 		MessShow(m_LogString,LTINFO);
	// 		for (jj =0; jj<(int)MonSys.vec_dev[ii].vec_lead_mode.size(); jj++)
	// 		{
	// 			sprintf(m_LogString,"%d,fdap[%.3f,%.3f,%.3f,%.3f]",jj+1,MonSys.vec_dev[ii].vec_lead_mode[jj].freq,MonSys.vec_dev[ii].vec_lead_mode[jj].damp,
	// 				MonSys.vec_dev[ii].vec_lead_mode[jj].ampl,MonSys.vec_dev[ii].vec_lead_mode[jj].phas);
	// 			MessShow(m_LogString,LTINFO);
	// 		}
	// 	}
	// 	sprintf(m_LogString,"汇总后模态:[%d]",MonSys.vec_modeshape.size());
	// 	MessShow(m_LogString,LTINFO);
	// 	for (ii =0; ii<(int)MonSys.vec_modeshape.size(); ii++)
	// 	{
	// 		sprintf(m_LogString,"fdap[%f,%f,%f,%f]",MonSys.vec_modeshape[ii].mode.freq,MonSys.vec_modeshape[ii].mode.damp,
	// 			MonSys.vec_modeshape[ii].mode.ampl,MonSys.vec_modeshape[ii].mode.phas);
	// 		MessShow(m_LogString,LTINFO);
	// 		for ( jj =0; jj<(int)MonSys.vec_modeshape[ii].vec_dev.size(); jj++)
	// 		{
	// 			sprintf(m_LogString,"%s[%d],fdap[%f,%f,%f,%f]",MonSys.vec_modeshape[ii].vec_dev[jj].dev.dev_name,MonSys.vec_modeshape[ii].vec_dev[jj].dev.dev_id,
	// 				MonSys.vec_modeshape[ii].vec_dev[jj].mode.freq,MonSys.vec_modeshape[ii].vec_dev[jj].mode.damp,MonSys.vec_modeshape[ii].vec_dev[jj].mode.ampl,MonSys.vec_modeshape[ii].vec_dev[jj].mode.phas);
	// 			MessShow(m_LogString,LTINFO);
	// 		}
	// 	}

	return 0;
}

int CLfoAnaObj::UpdateMonSys(LFO_SYS_INFO MonSysThread, LFO_SYS_INFO MonSys)
{
	return 0;
}

//************************************
// Method:    FormLfoCaseResult
// FullName:  CLfoAnaObj::FormLfoCaseResult
// Access:    public 
// Returns:   int
// Qualifier:
// 从振荡开始时刻，到振荡截止时刻，保存振荡事件的每个窗口分析结果。
// 输入：每个分析窗口结构(LFO_WINDOW_INFO)，事件ID，窗口ID，分析频率段，振荡开始时间，振荡结束时间，振荡持续时间，平均频率，最小阻尼比，触发告警设备，系统状态结构vector<LFO_SYS_INFO>
// 输出：振荡事件结构(LFO_CASE_INFO)，事件ID，窗口个数
// 每个窗口分析结果vector< LFO_WINDOW_INFO>
// 振荡统计（振荡开始时间，振荡结束时间，振荡持续时间，平均频率，最小阻尼比，触发告警设备）
// 振荡设备统计（按振荡设备统计本次振荡设备开始时间，结束时间，持续时间，波形统计）, 振荡设备模式（按振荡设备统计每个分析窗口的主导模式）vector< LFO_DEV_INFO >
// 每个窗口主导模式模态结果vector<LFO_MODE_SHAPE>
//************************************
int CLfoAnaObj::FormLfoCaseResult(vector<LFO_SYS_INFO>& VecLfoWindow, LFO_CASE_INFO& LfoCase, const int save_flag)
{

	int ii, jj, nFlag, nFlag2;
	int warn_mode, warn_type, blfo, bslfo, bhlfo;
	warn_mode = 0;
	warn_type = 0;
	blfo = 0;
	bslfo = 0;
	bhlfo = 0;
	LFO_SYS_INFO tmpWindows;
	LFO_DEV_INFO tmpDev;
	ClearLfoSysInfo(tmpWindows);
	ClearLfoDev(tmpDev);
	if (!VecLfoWindow.empty())
	{
		tmpWindows = VecLfoWindow.back();
	}
	else
	{
		return 0;
	}


	if (save_flag == LFO_SAVE_FLAG_IDEL)
	{
#ifdef _LFO_HIS_LIB_
		ClearLfoCase(tempLfoCase);
		tempLfoCase.progress = tmpWindows.progress;
		if (g_IsWarn != 1)
		{
			PowAnaDoing(tempLfoCase);
		}
#endif
		return 0;
	}
	else if (save_flag == LFO_SAVE_FLAG_START)
	{
		// 清空变量
		sprintf(m_LogString, "初始化LFOCASE");
		MessShow(m_LogString, LTINFO);
		ClearLfoCase(LfoCase);
		// 本轮开始振荡时间减去告警判断时间
		LfoCase.event_id = tmpWindows.start_time;
		LfoCase.start_time = tmpWindows.start_time;
		LfoCase.end_time = tmpWindows.end_time;
		LfoCase.hold_time = LfoCase.end_time - LfoCase.start_time;
		LfoCase.stat = tmpWindows.stat;
		//  		sprintf(m_LogString,"[yjj]设备开始振荡的起始点old[%d],new[%d]",m_LfoPara.data_pos_old,m_LfoPara.data_pos);
		//  		MessShow(m_LogString,LTINFO);
		// 		sprintf(m_LogString,"[yjj]窗口开始时间["INT_REC_FMT"],结束时间["INT_REC_FMT"]",tmpWindows.start_time,tmpWindows.end_time);
		// 		MessShow(m_LogString,LTINFO);
		// 		sprintf(m_LogString,"[yjj]窗口事件ID["INT_REC_FMT"]",LfoCase.event_id);
		// 		MessShow(m_LogString,LTINFO);

				// 保存在start_time前的窗口信息,TODO:待完善
		// 		for ( ii =0; ii<(int)VecLfoWindow.size(); ii++ )
		// 		{
		// 			if (VecLfoWindow[ii].window_id >= LfoCase.event_id)
		// 			{
		// 				LfoCase.vec_windows.push_back(VecLfoWindow[ii]);
		// 			}
		// 		}
		// 		// 暂只保存本轮窗口信息
		// 		if (!VecLfoWindow.empty())
		// 		{
		// 			LfoCase.vec_windows.push_back(tmpWindows);
		// 		}

				// 去除重复的窗口内容
		nFlag = 0;
		for (ii = 0; ii < (int)LfoCase.vec_windows.size(); ii++)
		{
			//			sprintf(m_LogString,"[yjj2]开始振荡窗口ID["INT_REC_FMT"],已有窗口["INT_REC_FMT"]",tmpWindows.window_id,LfoCase.vec_windows[ii].window_id);
			//			MessShow(m_LogString,LTINFO);
			if (tmpWindows.window_id == LfoCase.vec_windows[ii].window_id)
			{
				nFlag = 1;
				break;
			}
		}
		if (nFlag == 0)
		{
			// 添加未保存的窗口
			LfoCase.vec_windows.push_back(tmpWindows);

			// 记录事件中的最大振幅 取各窗口中主导设备的阻尼比最低值为事件最低阻尼比 并取对应的频率为事件频率
			for (ii = 0; ii < (int)LfoCase.vec_windows.size(); ii++)
			{
				if (LfoCase.vec_windows[ii].lead_dev.mode.damp < LfoCase.lead_mode.damp)
				{
					LfoCase.lead_mode.damp = LfoCase.vec_windows[ii].lead_dev.mode.damp;
					LfoCase.lead_mode.freq = LfoCase.vec_windows[ii].lead_dev.mode.freq;
				}
				if (LfoCase.vec_windows[ii].lead_dev.mode.ampl > LfoCase.lead_mode.ampl)
				{
					LfoCase.lead_mode.ampl = LfoCase.vec_windows[ii].lead_dev.mode.ampl;
				}
			}

			// 保存窗口信息中的振荡设备
			for (ii = 0; ii < (int)tmpWindows.vec_dev.size(); ii++)
			{
				nFlag = 0;
				for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
				{
					if (tmpWindows.vec_dev[ii].dev.dev_id == LfoCase.vec_dev[jj].dev.dev_id)
					{
						nFlag = 1;
						// 更新波形信息
						if (tmpWindows.vec_dev[ii].curve.max_ampl > LfoCase.vec_dev[jj].curve.max_ampl)
						{
							LfoCase.vec_dev[jj].curve = tmpWindows.vec_dev[ii].curve;
						}
						// 更新开始时间
						if (LfoCase.vec_dev[jj].start_time <= 0)
						{
							LfoCase.vec_dev[jj].start_time = tmpWindows.start_time;
						}
						// 更新结束时间
						if (tmpWindows.end_time > LfoCase.vec_dev[jj].end_time)
						{
							LfoCase.vec_dev[jj].end_time = tmpWindows.end_time;
						}
						// 更新持续时间
						LfoCase.vec_dev[jj].hold_time = LfoCase.vec_dev[jj].end_time - LfoCase.vec_dev[jj].start_time;
						// 添加模式信息,主导模式取并集
// 						for (kk=0; kk<(int)tmpWindows.vec_dev[ii].vec_lead_mode.size(); kk++)
// 						{
// 							LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].vec_lead_mode[kk]);
// 						}
						if (LfoCase.vec_dev[jj].mode.eve_source > tmpWindows.vec_dev[ii].mode.is_source)
						{
							tmpWindows.vec_dev[ii].mode.eve_source = LfoCase.vec_dev[jj].mode.eve_source;
						}
						CopyLfoModeInfo(LfoCase.vec_dev[jj].mode, tmpWindows.vec_dev[ii].mode);
						// 						LfoCase.vec_dev[jj].mode = tmpWindows.vec_dev[ii].mode;
												// 2012-03-20 只保存主导模式
												// 不管该设备是否告警，均保存本次分析的主导模式
												//if(!tmpWindows.vec_dev[ii].vec_lead_mode.empty())
						//						if(!tmpWindows.vec_dev[ii].mode.empty())
						// 						{
												//	LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.lead_dev.mode);
						LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].mode);
						//						}
						sprintf(m_LogString, "[提示]新增设备0["INT_REC_FMT"],振幅[%f],耗散功率[%f][%f],是否振荡源[%d]", INT_REC_VAL(LfoCase.vec_dev[jj].dev.dev_id), LfoCase.vec_dev[jj].mode.ampl, LfoCase.vec_dev[jj].mode.pow, tmpWindows.lead_dev.mode.pow, LfoCase.vec_dev[jj].mode.is_source);
						MessShow(m_LogString, LTINFO);
						break;
					}
				}
				if (nFlag == 0)
				{
					sprintf(m_LogString, "[提示]新增设备1["INT_REC_FMT"],振幅[%f],耗散功率[%f],是否振荡源[%d]", INT_REC_VAL(tmpWindows.vec_dev[ii].dev.dev_id), tmpWindows.vec_dev[ii].mode.ampl, tmpWindows.vec_dev[ii].mode.pow, tmpWindows.vec_dev[ii].mode.is_source);
					MessShow(m_LogString, LTINFO);
					// 新增设备
					//memset((void *)&tmpDev,0,sizeof(LFO_DEV_INFO));
					ClearLfoDev(tmpDev);
					tmpDev = tmpWindows.vec_dev[ii];
					tmpDev.start_time = tmpWindows.start_time;
					tmpDev.end_time = tmpWindows.end_time;
					tmpDev.hold_time = tmpDev.end_time - tmpDev.start_time;

					if (!tmpDev.vec_lead_mode.empty())
					{
						ClearLfoModeInfo(tmpDev.vec_lead_mode);
					}
					tmpDev.vec_lead_mode.push_back(tmpDev.mode);//存储该设备各分析窗口的主导模式信息

					LfoCase.vec_dev.push_back(tmpDev);
				}
			}
			// 记录事件中最低阻尼比及其对应的频率
// 			for ( ii =0; ii<(int)LfoCase.vec_dev.size(); ii++ )
// 			{
// 				if (LfoCase.vec_dev[ii].mode.damp < LfoCase.lead_mode.damp)
// 				{
// 					LfoCase.lead_mode.damp = LfoCase.vec_dev[ii].mode.damp;//振荡事件中最低阻尼比
// 					LfoCase.lead_mode.freq = LfoCase.vec_dev[ii].mode.freq;//振荡事件中最低阻尼比对应频率
// 				}
// 			}
			// 保存窗口信息中的振荡中心
			for (ii = 0; ii < (int)tmpWindows.cen_dev.size(); ii++)
			{
				nFlag = 0;
				for (jj = 0; jj < (int)LfoCase.cen_dev.size(); jj++)
				{
					if (tmpWindows.cen_dev[ii].dev.dev_id == LfoCase.cen_dev[jj].dev.dev_id)
					{
						nFlag = 1;
						// 更新波形信息
						if (tmpWindows.cen_dev[ii].curve.max_ampl > LfoCase.cen_dev[jj].curve.max_ampl)
						{
							LfoCase.cen_dev[jj].curve = tmpWindows.cen_dev[ii].curve;
						}
						// 更新开始时间
						if (LfoCase.cen_dev[jj].start_time <= 0)
						{
							LfoCase.cen_dev[jj].start_time = tmpWindows.start_time;
						}
						// 更新结束时间
						if (tmpWindows.end_time > LfoCase.cen_dev[jj].end_time)
						{
							LfoCase.cen_dev[jj].end_time = tmpWindows.end_time;
						}
						// 更新持续时间
						LfoCase.cen_dev[jj].hold_time = LfoCase.cen_dev[jj].end_time - LfoCase.cen_dev[jj].start_time;
						// 添加模式信息,主导模式取并集
						// 						for (kk=0; kk<(int)tmpWindows.vec_dev[ii].vec_lead_mode.size(); kk++)
						// 						{
						// 							LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].vec_lead_mode[kk]);
						// 						}
// 						LfoCase.cen_dev[jj].mode = tmpWindows.cen_dev[ii].mode;
						CopyLfoModeInfo(LfoCase.cen_dev[jj].mode, tmpWindows.cen_dev[ii].mode);
						// 2012-03-20 只保存主导模式
						// 不管该设备是否告警，均保存本次分析的主导模式
						//if(!tmpWindows.vec_dev[ii].vec_lead_mode.empty())
						//						if(!tmpWindows.vec_dev[ii].mode.empty())
						// 						{
						//	LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.lead_dev.mode);
						LfoCase.cen_dev[jj].vec_lead_mode.push_back(tmpWindows.cen_dev[ii].mode);
						//						}
						sprintf(m_LogString, "[提示]新增厂站0["INT_REC_FMT"],振幅[%f],是否振荡中心[%d]", INT_REC_VAL(LfoCase.cen_dev[jj].dev.dev_id), LfoCase.cen_dev[jj].mode.ampl, LfoCase.cen_dev[jj].lfo_is_cntr);
						MessShow(m_LogString, LTINFO);
						break;
					}
				}
				if (nFlag == 0)
				{
					sprintf(m_LogString, "[提示]新增厂站1["INT_REC_FMT"],振幅[%f],是否振荡中心[%d]", INT_REC_VAL(tmpWindows.cen_dev[ii].dev.dev_id), tmpWindows.cen_dev[ii].mode.ampl, tmpWindows.cen_dev[ii].lfo_is_cntr);
					MessShow(m_LogString, LTINFO);
					// 新增设备
					//memset((void *)&tmpDev,0,sizeof(LFO_DEV_INFO));
					ClearLfoDev(tmpDev);
					tmpDev = tmpWindows.cen_dev[ii];
					tmpDev.start_time = tmpWindows.start_time;
					tmpDev.end_time = tmpWindows.end_time;
					tmpDev.hold_time = tmpDev.end_time - tmpDev.start_time;

					if (!tmpDev.vec_lead_mode.empty())
					{
						ClearLfoModeInfo(tmpDev.vec_lead_mode);
					}
					tmpDev.vec_lead_mode.push_back(tmpDev.mode);//存储该设备各分析窗口的主导模式信息

					LfoCase.cen_dev.push_back(tmpDev);
				}
			}
			if (LfoCase.cen_dev.size() > 0)
			{
				// 				strcpy( LfoCase.center_desc, "振荡中心位于以下厂站附近：");
				sprintf(LfoCase.center_desc, "");
				for (ii = 0; ii < LfoCase.cen_dev.size(); ii++)
				{
					sprintf(m_LogString, "%s ", LfoCase.cen_dev[ii].dev.dev_name);
					strcat(LfoCase.center_desc, m_LogString);
				}
			}
			else
			{
				sprintf(LfoCase.center_desc, "");
			}
			sprintf(m_LogString, "本次振荡事件振荡中心描述:%s", LfoCase.center_desc);
			MessShow(m_LogString, LTINFO);
			//保存振荡模式
			sprintf(m_LogString, "本轮窗口模式数目[%d]", tmpWindows.vec_mainmode.size());
			MessShow(m_LogString, LTINFO);
			for (ii = 0; ii < tmpWindows.vec_mainmode.size(); ii++)
			{
				sprintf(m_LogString, "本轮模式[%f]设备个数[%d]", tmpWindows.vec_mainmode[ii].mode.freq, tmpWindows.vec_mainmode[ii].vec_dev.size());
				MessShow(m_LogString, LTINFO);
				nFlag = 0;
				for (jj = 0; jj < LfoCase.vec_mainmode.size(); jj++)
				{
					if (fabs(tmpWindows.vec_mainmode[ii].mode.freq - LfoCase.vec_mainmode[jj].mode.freq) < m_LfoPara.freq_diff)
					{
						nFlag = 1;
						LfoCase.vec_mainmode[jj].mode.end_time = tmpWindows.end_time;
						sprintf(m_LogString, "[提示]更新模式频率[%f],开始时间[%d],结束时间[%d]", LfoCase.vec_mainmode[jj].mode.freq, LfoCase.vec_mainmode[jj].mode.start_time, LfoCase.vec_mainmode[jj].mode.end_time);
						MessShow(m_LogString, LTINFO);
						if (LfoCase.vec_mainmode[jj].is_lfo_source < tmpWindows.vec_mainmode[ii].is_lfo_source)
						{
							LfoCase.vec_mainmode[jj].is_lfo_source = tmpWindows.vec_mainmode[ii].is_lfo_source;
							sprintf(LfoCase.vec_mainmode[jj].lfo_ana_des, "%s", tmpWindows.vec_mainmode[ii].is_lfo_source);
						}
						if (LfoCase.vec_mainmode[jj].is_center < tmpWindows.vec_mainmode[ii].is_center)
						{
							LfoCase.vec_mainmode[jj].is_center = tmpWindows.vec_mainmode[ii].is_center;
							sprintf(LfoCase.vec_mainmode[jj].lfo_ana_des, "%s", tmpWindows.vec_mainmode[ii].lfo_ana_des);
						}
						if (LfoCase.vec_mainmode[jj].mode.damp > tmpWindows.vec_mainmode[ii].mode.damp)
						{
							LfoCase.vec_mainmode[jj].mode.damp = tmpWindows.vec_mainmode[ii].mode.damp;
							LfoCase.vec_mainmode[jj].mode.freq = tmpWindows.vec_mainmode[ii].mode.freq;
						}

						for (int kk = 0; kk < tmpWindows.vec_mainmode[ii].vec_dev.size(); kk++)
						{
							nFlag2 = 0;
							for (int ll = 0; ll < LfoCase.vec_mainmode[jj].vec_dev.size(); ll++)
							{
								if (tmpWindows.vec_mainmode[ii].vec_dev[kk].dev.dev_id == LfoCase.vec_mainmode[jj].vec_dev[ll].dev.dev_id)
								{
									nFlag2 = 1;
									if (LfoCase.vec_mainmode[jj].vec_dev[ll].mode.ampl < tmpWindows.vec_mainmode[ii].vec_dev[kk].mode.ampl)
									{
										LfoCase.vec_mainmode[jj].vec_dev[ll].mode.ampl = tmpWindows.vec_mainmode[ii].vec_dev[kk].mode.ampl;
									}
									if (LfoCase.vec_mainmode[jj].vec_dev[ll].mode.is_source < tmpWindows.vec_mainmode[ii].vec_dev[kk].mode.is_source)
									{
										LfoCase.vec_mainmode[jj].vec_dev[ll].mode.is_source = tmpWindows.vec_mainmode[ii].vec_dev[kk].mode.is_source;
									}
									sprintf(m_LogString, "更新参与设备[%s],振荡幅值[%f],是否振荡源标志[%d]", LfoCase.vec_mainmode[jj].vec_dev[ll].dev.dev_name, LfoCase.vec_mainmode[jj].vec_dev[ll].mode.ampl, LfoCase.vec_mainmode[jj].vec_dev[ll].mode.is_source);
									MessShow(m_LogString, LTINFO);
									break;
								}
							}
							if (nFlag2 == 0)
							{
								LfoCase.vec_mainmode[jj].vec_dev.push_back(tmpWindows.vec_mainmode[ii].vec_dev[kk]);
							}
						}
					}
					break;
				}
				if (nFlag == 0)
				{
					tmpWindows.vec_mainmode[ii].mode.start_time = tmpWindows.start_time;
					tmpWindows.vec_mainmode[ii].mode.end_time = tmpWindows.end_time;
					if (tmpWindows.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_min)
					{
						sprintf(tmpWindows.vec_mainmode[ii].lfo_type, "%s", "超低频振荡");
					}
					else if (tmpWindows.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_min && tmpWindows.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_max)
					{
						sprintf(tmpWindows.vec_mainmode[ii].lfo_type, "%s", "低频振荡");
					}
					else if (tmpWindows.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_max)
					{
						sprintf(tmpWindows.vec_mainmode[ii].lfo_type, "%s", "高频振荡");
					}
					sprintf(m_LogString, "[提示]新增模式频率[%f],开始时间[%d],结束时间[%d]", tmpWindows.vec_mainmode[ii].mode.freq, tmpWindows.vec_mainmode[ii].mode.start_time, tmpWindows.vec_mainmode[ii].mode.end_time);
					MessShow(m_LogString, LTINFO);
					LfoCase.vec_mainmode.push_back(tmpWindows.vec_mainmode[ii]);
				}
			}
			sprintf(m_LogString, "[提示]统计后模式数目[%d]", LfoCase.vec_mainmode.size());
			MessShow(m_LogString, LTINFO);
			sort(LfoCase.vec_mainmode.begin(), LfoCase.vec_mainmode.end(), g_SortModeShapeByDamp);
			for (jj = 0; jj < LfoCase.vec_mainmode.size(); jj++)
			{
				LfoCase.vec_mainmode[jj].gn_num = 0;
				LfoCase.vec_mainmode[jj].line_num = 0;
				LfoCase.vec_mainmode[jj].tr_mun = 0;
				for (ii = 0; ii < LfoCase.vec_mainmode[jj].vec_dev.size(); ii++)
				{
					if (LfoCase.vec_mainmode[jj].vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
					{
						LfoCase.vec_mainmode[jj].gn_num++;
					}
					if (LfoCase.vec_mainmode[jj].vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
					{
						LfoCase.vec_mainmode[jj].line_num++;
					}
					if (LfoCase.vec_mainmode[jj].vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
					{
						LfoCase.vec_mainmode[jj].tr_mun++;
					}
				}
			}
		}

		if (LfoCase.stat == MENU_LFO_DEV_STS_WRN)
		{
			if (m_LfoPara.is_eng == 1)
			{
				sprintf(LfoCase.case_desc, "%s power oscillation happening", PT_GetTimeStringBySeconds(LfoCase.start_time));
			}
			else
			{
#ifdef _GUANGDONG
				sprintf(LfoCase.case_desc, "%s %s%s发生振荡,频率:%.3fHz", PT_GetTimeStringBySeconds(LfoCase.start_time), LfoCase.warn_dev.dev.fac_name, LfoCase.warn_dev.dev.dev_name, LfoCase.lead_mode.freq);
#else
				sprintf(LfoCase.case_desc, "%s %s发生振荡,频率:%.3fHz", PT_GetTimeStringBySeconds(LfoCase.start_time), LfoCase.warn_dev.dev.dev_name, LfoCase.lead_mode.freq);
#endif
			}
		}


		// 首先告警设备
		if (!LfoCase.vec_dev.empty())
		{
			sort(LfoCase.vec_dev.begin(), LfoCase.vec_dev.end(), g_SortDevByEngy);
			CopyLfoDev(LfoCase.warn_dev, LfoCase.vec_dev.front());
			// 			LfoCase.warn_dev = LfoCase.vec_dev.front();
		}
		LfoCase.lfo_gn_num = 0;
		LfoCase.lfo_ln_num = 0;
		LfoCase.lfo_tr_num = 0;
		LfoCase.lfo_inf_num = 0;

		int temp_num = 0;
		temp_num = (int)LfoCase.vec_dev.size();
		sprintf(m_LogString, "[提示]1振荡设备数目[%d]", temp_num);
		MessShow(m_LogString, LTINFO);

		LfoCase.lfos_gn_num = 0;
		LfoCase.lfos_ln_num = 0;
		LfoCase.lfos_tr_num = 0;
		sprintf(LfoCase.lfos_gn_name, " ");
		for (ii = 0; ii < (int)LfoCase.vec_dev.size(); ii++)
		{
			// 			sprintf(m_LogString,"[提示]1是否振荡源[%d],设备类型[%d]",LfoCase.vec_dev[ii].mode.is_source,LfoCase.vec_dev[ii].dev.dev_type);
			// 			MessShow(m_LogString,LTINFO);			
			if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				LfoCase.lfo_gn_num++;
				if (LfoCase.vec_dev[ii].mode.is_source == 1 || LfoCase.vec_dev[ii].mode.eve_source)
				{
					LfoCase.lfos_gn_num++;
					sprintf(LfoCase.lfos_gn_name, "%s", LfoCase.vec_dev[ii].dev.dev_name);
					LfoCase.vec_dev[ii].sourceflag = 1;
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				LfoCase.lfo_ln_num++;
				if (LfoCase.vec_dev[ii].mode.is_source == 1 || LfoCase.vec_dev[ii].mode.eve_source)
				{
					LfoCase.lfos_ln_num++;
					LfoCase.vec_dev[ii].sourceflag = 1;
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				LfoCase.lfo_tr_num++;
				if (LfoCase.vec_dev[ii].mode.is_source == 1 || LfoCase.vec_dev[ii].mode.eve_source)
				{
					LfoCase.lfos_tr_num++;
					LfoCase.vec_dev[ii].sourceflag = 1;
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
			{
				LfoCase.lfo_inf_num++;
			}
			// 			if (LfoCase.vec_dev[ii].mode.pow_num < 250)
			// 			{
			// 				if (LfoCase.vec_dev[ii].mode.is_source == 1)
			// 				{
			// 					sprintf(m_LogString,"1-111111");
			// 					MessShow(m_LogString,LTINFO);			
			// 					LfoCase.vec_dev[ii].mode.vec_pow[LfoCase.vec_dev[ii].mode.pow_num] = LfoCase.vec_dev[ii].mode.pow;
			// 					LfoCase.vec_dev[ii].mode.pow_num ++;
			// 					// 				nFlag = 0;
			// 					// 				for (jj=0;jj<LfoCase.sou_dev.size();jj++)
			// 					// 				{
			// 					// 					if (LfoCase.vec_dev[ii].dev.dev_id == LfoCase.sou_dev[jj].dev.dev_id)
			// 					// 					{
			// 					// 						nFlag = 1;
			// 					// 						LfoCase.sou_dev[jj].mode.vec_pow.push_back(LfoCase.vec_dev[ii].mode.pow);
			// 					// 						break;
			// 					// 					}
			// 					// 					if (nFlag == 0)
			// 					// 					{
			// 					// 						LfoCase.vec_dev[ii].mode.vec_pow.push_back(LfoCase.vec_dev[ii].mode.pow);
			// 					// 						LfoCase.sou_dev.push_back(LfoCase.vec_dev[ii]);
			// 					// 					}
			// 					// 				}
			// 				}
			// 				else
			// 				{
			// 					sprintf(m_LogString,"1-222222");
			// 					MessShow(m_LogString,LTINFO);			
			// 					LfoCase.vec_dev[ii].mode.vec_pow[LfoCase.vec_dev[ii].mode.pow_num]= 999999;
			// 					LfoCase.vec_dev[ii].mode.pow_num ++;
			// 				}
			// 				sprintf(m_LogString,"1-333333");
			// 				MessShow(m_LogString,LTINFO);			
			// 			}
		}
		if (LfoCase.lfos_gn_num > 0)
		{
			sprintf(LfoCase.lfos_des, "%s", LfoCase.lfos_gn_name);
		}
		else if (LfoCase.lfos_ln_num > 0 || LfoCase.lfos_tr_num > 0)
		{
			sprintf(LfoCase.lfos_des, "找到振荡源相关支路");
		}
		else
		{
			sprintf(LfoCase.lfos_des, "");
		}
		if ((LfoCase.lfos_gn_num + LfoCase.lfos_ln_num + LfoCase.lfos_tr_num) > 0)
		{
			LfoCase.is_lfo_source = MENU_LFO_MON_YES;
		}
		else
		{
			LfoCase.is_lfo_source = MENU_LFO_MON_NO;
		}
		sprintf(m_LogString, "[提示1]振荡源机组[%s],线路数目[%d],变压器数目[%d]", LfoCase.lfos_gn_name, LfoCase.lfos_ln_num, LfoCase.lfos_tr_num);
		MessShow(m_LogString, LTINFO);

		if (!LfoCase.vec_mainmode.empty())
		{
			for (int ii = 0; ii < LfoCase.vec_mainmode.size(); ii++)
			{
				if (LfoCase.vec_mainmode[ii].mode.stat >= MENU_LFO_DEV_STS_WRN)
				{
					warn_mode++;
					if (LfoCase.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_min)
					{
						if (bslfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_SLOW - 1);
						}
						bslfo = 1;
					}
					else if (LfoCase.vec_mainmode[ii].mode.freq > m_LfoPara.lfo_freq_max)
					{
						if (bhlfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_HIGH - 1);
						}
						bhlfo = 1;
					}
					else if (LfoCase.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_min && LfoCase.vec_mainmode[ii].mode.freq <= m_LfoPara.lfo_freq_max)
					{
						if (blfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_LOW - 1);
						}
						blfo = 1;
					}
				}
			}
		}
		if (warn_mode > 1)
		{
			LfoCase.lfo_modle = MENU_LFO_MULTI;
			sprintf(LfoCase.type_desc, "%s", "多模式");
		}
		else if (warn_mode == 1)
		{
			LfoCase.lfo_modle = MENU_LFO_SINGLE;
			sprintf(LfoCase.type_desc, "%s", "单模式");
		}
		else if (warn_mode < 1)
		{
			LfoCase.lfo_modle = 0;
		}
		LfoCase.lfo_type = warn_type;
		if (bslfo == 1)
		{
			strcat(LfoCase.type_desc, "超低频振荡 ");
		}
		if (blfo == 1)
		{
			strcat(LfoCase.type_desc, "低频振荡 ");
		}
		if (bhlfo == 1)
		{
			strcat(LfoCase.type_desc, "高频振荡 ");
		}
#ifdef _LFO_HIS_LIB_
		PowAnaDoing(LfoCase);
		sprintf(m_LogString, "[提示：]计算进度[%f]", LfoCase.progress);
		MessShow(m_LogString, LTINFO);
#endif
		// 保存主导模式
//		LfoCase.vec_modeshape = tmpMonSys.vec_modeshape;
// 		if (!tmpMonSys.vec_modeshape.empty())
// 		{
//             for ( ii =0; ii<(int)tmpMonSys.vec_modeshape.size(); ii++)
//             {
//                 LfoCase.vec_modeshape.push_back(tmpMonSys.vec_modeshape[ii]);
//             }	
// 		}
	}
	else if (save_flag == LFO_SAVE_FLAG_HOLD)
	{
		// 		sprintf(m_LogString,"[yjj]更新窗口开始时间["INT_REC_FMT" "INT_REC_FMT"]，结束时间["INT_REC_FMT" "INT_REC_FMT"]",tmpWindows.start_time,LfoCase.start_time,tmpWindows.end_time,LfoCase.end_time);
		// 		MessShow(m_LogString,LTINFO);
				// 更新振荡开始时间
		if (tmpWindows.start_time < LfoCase.start_time)
		{
			LfoCase.start_time = tmpWindows.start_time;
		}
		// 更新振荡结束时间
		if (tmpWindows.end_time > LfoCase.end_time)
		{
			LfoCase.end_time = tmpWindows.end_time;
		}
		LfoCase.hold_time = LfoCase.end_time - LfoCase.start_time;
		LfoCase.stat = tmpWindows.stat;
		//LfoCase.lead_mode = tmpWindows.lead_mode;

		// 暂只保存本轮窗口信息
// 		if (!VecLfoWindow.empty())
// 		{
// 			LfoCase.vec_windows.push_back(tmpWindows);
// 		}

		// 去除重复的窗口内容
		nFlag = 0;
		for (ii = 0; ii < (int)LfoCase.vec_windows.size(); ii++)
		{
			if (tmpWindows.window_id == LfoCase.vec_windows[ii].window_id)
			{
				sprintf(m_LogString, "[yjj2]持续振荡窗口ID["RTDB_TIME_FMT"]重复,该窗口振荡设备个数%d,已有窗口振荡设备个数%d", tmpWindows.window_id, tmpWindows.vec_dev.size(), LfoCase.vec_windows[ii].vec_dev.size());
				MessShow(m_LogString, LTINFO);
				nFlag = 1;
				break;
			}
		}

		if (nFlag == 0)
		{
			LfoCase.vec_windows.push_back(tmpWindows);

			// 记录事件中的最大振幅 取各窗口中主导设备的阻尼比最低值为事件最低阻尼比 并取对应的频率为事件频率
			for (ii = 0; ii < (int)LfoCase.vec_windows.size(); ii++)
			{
				if (LfoCase.vec_windows[ii].lead_dev.mode.damp < LfoCase.lead_mode.damp)
				{
					LfoCase.lead_mode.damp = LfoCase.vec_windows[ii].lead_dev.mode.damp;
					LfoCase.lead_mode.freq = LfoCase.vec_windows[ii].lead_dev.mode.freq;
				}
				if (LfoCase.vec_windows[ii].lead_dev.mode.ampl > LfoCase.lead_mode.ampl)
				{
					LfoCase.lead_mode.ampl = LfoCase.vec_windows[ii].lead_dev.mode.ampl;
				}
			}

			// 保存窗口信息中的振荡设备
			sprintf(m_LogString, "窗口ID["RTDB_TIME_FMT"],振荡设备个数%d", tmpWindows.window_id, tmpWindows.vec_dev.size());
			MessShow(m_LogString, LTINFO);
			for (ii = 0; ii < (int)tmpWindows.vec_dev.size(); ii++)
			{
				nFlag = 0;
				for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
				{
					if (tmpWindows.vec_dev[ii].dev.dev_id == LfoCase.vec_dev[jj].dev.dev_id)
					{
						nFlag = 1;
						// 更新波形信息
						if (tmpWindows.vec_dev[ii].curve.max_val > LfoCase.vec_dev[jj].curve.max_val)
						{
							LfoCase.vec_dev[jj].curve.max_val = tmpWindows.vec_dev[ii].curve.max_val;
							LfoCase.vec_dev[jj].curve.max_val_t = tmpWindows.vec_dev[ii].curve.max_val_t;
						}
						if (tmpWindows.vec_dev[ii].curve.min_val < LfoCase.vec_dev[jj].curve.min_val)
						{
							LfoCase.vec_dev[jj].curve.min_val = tmpWindows.vec_dev[ii].curve.min_val;
							LfoCase.vec_dev[jj].curve.min_val_t = tmpWindows.vec_dev[ii].curve.min_val_t;
						}
						LfoCase.vec_dev[jj].curve.max_ampl = LfoCase.vec_dev[jj].curve.max_val - LfoCase.vec_dev[jj].curve.min_val;
						// 更新开始时间
						if (LfoCase.vec_dev[jj].start_time <= 0)
						{
							LfoCase.vec_dev[jj].start_time = tmpWindows.start_time;
						}
						// 更新结束时间
						if (tmpWindows.end_time > LfoCase.vec_dev[jj].end_time)
						{
							LfoCase.vec_dev[jj].end_time = tmpWindows.end_time;
						}
						// 更新持续时间
						LfoCase.vec_dev[jj].hold_time = LfoCase.vec_dev[jj].end_time - LfoCase.vec_dev[jj].start_time;
						// 						sprintf(m_LogString,"[yjj]11["INT_REC_FMT"]，开始时间["RTDB_TIME_FMT"],结束时间["RTDB_TIME_FMT"],持续时间[%d]", INT_REC_VAL(LfoCase.vec_dev[jj].dev.dev_id),LfoCase.vec_dev[jj].start_time,LfoCase.vec_dev[jj].end_time,LfoCase.vec_dev[jj].hold_time);
						// 						MessShow(m_LogString,LTINFO);						

												// 添加模式信息,主导模式取并集
						// 						for (kk=0; kk<(int)tmpWindows.vec_dev[ii].vec_lead_mode.size(); kk++)
						// 						{
						// 							LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].vec_lead_mode[kk]);
						// 						}
						if (LfoCase.vec_dev[jj].mode.eve_source > tmpWindows.vec_dev[ii].mode.is_source)
						{
							tmpWindows.vec_dev[ii].mode.eve_source = LfoCase.vec_dev[jj].mode.eve_source;
						}
						CopyLfoModeInfo(LfoCase.vec_dev[jj].mode, tmpWindows.vec_dev[ii].mode);
						// 						LfoCase.vec_dev[jj].mode = tmpWindows.vec_dev[ii].mode;
												// 2012-03-20 只保存主导模式
												//if(!tmpWindows.vec_dev[ii].vec_lead_mode.empty())
						//						if(!tmpWindows.vec_dev[ii].mode.empty())
						//						{
													//	LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.lead_dev.mode);
						LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].mode);
						//						}
						//  		sprintf(m_LogString,"[yjj]新增设备2["INT_REC_FMT"]，振幅[%f],耗散功率[%f][%f]",INT_REC_VAL(LfoCase.vec_dev[jj].dev.dev_id),tmpWindows.lead_dev.mode.ampl,LfoCase.vec_dev[jj].mode.pow,tmpWindows.lead_dev.mode.pow);
						//  		MessShow(m_LogString,LTINFO);
						break;
					}
				}
				if (nFlag == 0)
				{
					sprintf(m_LogString, "[提示]新增设备3["INT_REC_FMT"],振幅[%f],耗散功率[%f]", INT_REC_VAL(tmpWindows.vec_dev[ii].dev.dev_id), tmpWindows.vec_dev[ii].mode.ampl, tmpWindows.vec_dev[ii].mode.pow);
					MessShow(m_LogString, LTINFO);
					// 新增设备
					//memset((void *)&tmpDev,0,sizeof(LFO_DEV_INFO));
					ClearLfoDev(tmpDev);
					CopyLfoDev(tmpDev, tmpWindows.vec_dev[ii]);
					tmpDev.start_time = tmpWindows.start_time;
					tmpDev.end_time = tmpWindows.end_time;
					tmpDev.hold_time = tmpDev.end_time - tmpDev.start_time;

					if (!tmpDev.vec_lead_mode.empty())
					{
						ClearLfoModeInfo(tmpDev.vec_lead_mode);
					}
					tmpDev.vec_lead_mode.push_back(tmpDev.mode);//存储该设备各分析窗口的主导模式信息
				//	LfoCase.vec_dev.push_back(tmpWindows.vec_dev[ii]);
					LfoCase.vec_dev.push_back(tmpDev);
				}
			}

			// 记录事件中最低阻尼比及其对应的频率
// 			for ( ii =0; ii<(int)LfoCase.vec_dev.size(); ii++ )
// 			{
// 				if (LfoCase.vec_dev[ii].mode.damp < LfoCase.lead_mode.damp)
// 				{
// 					LfoCase.lead_mode.damp = LfoCase.vec_dev[ii].mode.damp;//振荡事件中最低阻尼比
// 					LfoCase.lead_mode.freq = LfoCase.vec_dev[ii].mode.freq;//振荡事件中最低阻尼比对应频率
// 				}
// 			}
			// 保存窗口信息中的振荡中心
			for (ii = 0; ii < (int)tmpWindows.cen_dev.size(); ii++)
			{
				nFlag = 0;
				for (jj = 0; jj < (int)LfoCase.cen_dev.size(); jj++)
				{
					if (tmpWindows.cen_dev[ii].dev.dev_id == LfoCase.cen_dev[jj].dev.dev_id)
					{
						nFlag = 1;
						// 更新波形信息
						if (tmpWindows.cen_dev[ii].curve.max_ampl > LfoCase.cen_dev[jj].curve.max_ampl)
						{
							LfoCase.cen_dev[jj].curve = tmpWindows.cen_dev[ii].curve;
						}
						// 更新开始时间
						if (LfoCase.cen_dev[jj].start_time <= 0)
						{
							LfoCase.cen_dev[jj].start_time = tmpWindows.start_time;
						}
						// 更新结束时间
						if (tmpWindows.end_time > LfoCase.cen_dev[jj].end_time)
						{
							LfoCase.cen_dev[jj].end_time = tmpWindows.end_time;
						}
						// 更新持续时间
						LfoCase.cen_dev[jj].hold_time = LfoCase.cen_dev[jj].end_time - LfoCase.cen_dev[jj].start_time;
						// 添加模式信息,主导模式取并集
						// 						for (kk=0; kk<(int)tmpWindows.vec_dev[ii].vec_lead_mode.size(); kk++)
						// 						{
						// 							LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].vec_lead_mode[kk]);
						// 						}
// 						LfoCase.cen_dev[jj].mode = tmpWindows.cen_dev[ii].mode;
						CopyLfoModeInfo(LfoCase.cen_dev[jj].mode, tmpWindows.cen_dev[ii].mode);
						// 2012-03-20 只保存主导模式
						// 不管该设备是否告警，均保存本次分析的主导模式
						//if(!tmpWindows.vec_dev[ii].vec_lead_mode.empty())
						//						if(!tmpWindows.vec_dev[ii].mode.empty())
						// 						{
						//	LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.lead_dev.mode);
						LfoCase.cen_dev[jj].vec_lead_mode.push_back(tmpWindows.cen_dev[ii].mode);
						//						}
						sprintf(m_LogString, "[提示]新增厂站0["INT_REC_FMT"],振幅[%f],是否振荡中心[%d]", INT_REC_VAL(LfoCase.cen_dev[jj].dev.dev_id), LfoCase.cen_dev[jj].mode.ampl, LfoCase.cen_dev[jj].lfo_is_cntr);
						MessShow(m_LogString, LTINFO);
						break;
					}
				}
				if (nFlag == 0)
				{
					sprintf(m_LogString, "[提示]新增厂站1["INT_REC_FMT"],振幅[%f],是否振荡中心[%d]", INT_REC_VAL(tmpWindows.cen_dev[ii].dev.dev_id), tmpWindows.cen_dev[ii].mode.ampl, tmpWindows.cen_dev[ii].lfo_is_cntr);
					MessShow(m_LogString, LTINFO);
					// 新增设备
					//memset((void *)&tmpDev,0,sizeof(LFO_DEV_INFO));
					ClearLfoDev(tmpDev);
					tmpDev = tmpWindows.cen_dev[ii];
					tmpDev.start_time = tmpWindows.start_time;
					tmpDev.end_time = tmpWindows.end_time;
					tmpDev.hold_time = tmpDev.end_time - tmpDev.start_time;

					if (!tmpDev.vec_lead_mode.empty())
					{
						ClearLfoModeInfo(tmpDev.vec_lead_mode);
					}
					tmpDev.vec_lead_mode.push_back(tmpDev.mode);//存储该设备各分析窗口的主导模式信息

					LfoCase.cen_dev.push_back(tmpDev);
				}
			}
			if (LfoCase.cen_dev.size() > 0)
			{
				// 				strcpy( LfoCase.center_desc, "振荡中心位于以下厂站附近：");
				sprintf(LfoCase.center_desc, "");
				for (ii = 0; ii < LfoCase.cen_dev.size(); ii++)
				{
					sprintf(m_LogString, "%s ", LfoCase.cen_dev[ii].dev.dev_name);
					strcat(LfoCase.center_desc, m_LogString);
				}
				sprintf(m_LogString, "本次振荡事件振荡中心描述:%s", LfoCase.center_desc);
				MessShow(m_LogString, LTINFO);
			}
			else
			{
				sprintf(LfoCase.center_desc, "");
			}
			sprintf(m_LogString, "本次振荡事件振荡中心描述:%s", LfoCase.center_desc);
			MessShow(m_LogString, LTINFO);
			//保存振荡模式
			sprintf(m_LogString, "本轮窗口模式数目[%d]", tmpWindows.vec_mainmode.size());
			MessShow(m_LogString, LTINFO);
			for (ii = 0; ii < tmpWindows.vec_mainmode.size(); ii++)
			{
				sprintf(m_LogString, "本轮模式[%f]设备个数[%d]", tmpWindows.vec_mainmode[ii].mode.freq, tmpWindows.vec_mainmode[ii].vec_dev.size());
				MessShow(m_LogString, LTINFO);
				nFlag = 0;
				for (jj = 0; jj < LfoCase.vec_mainmode.size(); jj++)
				{
					if (fabs(tmpWindows.vec_mainmode[ii].mode.freq - LfoCase.vec_mainmode[jj].mode.freq) < m_LfoPara.freq_diff)
					{
						nFlag = 1;
						LfoCase.vec_mainmode[jj].mode.end_time = tmpWindows.end_time;;
						sprintf(m_LogString, "[提示]更新模式频率[%f],开始时间[%d],结束时间[%d]", LfoCase.vec_mainmode[jj].mode.freq, LfoCase.vec_mainmode[jj].mode.start_time, LfoCase.vec_mainmode[jj].mode.end_time);
						MessShow(m_LogString, LTINFO);
						if (LfoCase.vec_mainmode[jj].is_lfo_source < tmpWindows.vec_mainmode[ii].is_lfo_source)
						{
							LfoCase.vec_mainmode[jj].is_lfo_source = tmpWindows.vec_mainmode[ii].is_lfo_source;
							sprintf(LfoCase.vec_mainmode[jj].lfo_ana_des, "%s", tmpWindows.vec_mainmode[ii].lfo_ana_des);
						}
						if (LfoCase.vec_mainmode[jj].is_center < tmpWindows.vec_mainmode[ii].is_center)
						{
							LfoCase.vec_mainmode[jj].is_center = tmpWindows.vec_mainmode[ii].is_center;
							sprintf(LfoCase.vec_mainmode[jj].lfo_ana_des, "振荡中心:%s", tmpWindows.vec_mainmode[ii].lfo_ana_des);
						}
						if (LfoCase.vec_mainmode[jj].mode.damp > tmpWindows.vec_mainmode[ii].mode.damp)
						{
							LfoCase.vec_mainmode[jj].mode.damp = tmpWindows.vec_mainmode[ii].mode.damp;
							LfoCase.vec_mainmode[jj].mode.freq = tmpWindows.vec_mainmode[ii].mode.freq;
						}
						for (int kk = 0; kk < tmpWindows.vec_mainmode[ii].vec_dev.size(); kk++)
						{
							nFlag2 = 0;
							for (int ll = 0; ll < LfoCase.vec_mainmode[jj].vec_dev.size(); ll++)
							{
								if (tmpWindows.vec_mainmode[ii].vec_dev[kk].dev.dev_id == LfoCase.vec_mainmode[jj].vec_dev[ll].dev.dev_id)
								{
									if (LfoCase.vec_mainmode[jj].vec_dev[ll].mode.ampl < tmpWindows.vec_mainmode[ii].vec_dev[kk].mode.ampl)
									{
										LfoCase.vec_mainmode[jj].vec_dev[ll].mode.ampl = tmpWindows.vec_mainmode[ii].vec_dev[kk].mode.ampl;
									}
									if (LfoCase.vec_mainmode[jj].vec_dev[ll].mode.is_source < tmpWindows.vec_mainmode[ii].vec_dev[kk].mode.is_source)
									{
										LfoCase.vec_mainmode[jj].vec_dev[ll].mode.is_source = tmpWindows.vec_mainmode[ii].vec_dev[kk].mode.is_source;
									}
									sprintf(m_LogString, "更新参与设备[%s],振荡幅值[%f],是否振荡源标志[%d]", LfoCase.vec_mainmode[jj].vec_dev[ll].dev.dev_name, LfoCase.vec_mainmode[jj].vec_dev[ll].mode.ampl, LfoCase.vec_mainmode[jj].vec_dev[ll].mode.is_source);
									MessShow(m_LogString, LTINFO);
									nFlag2 = 1;
									break;
								}
							}
							if (nFlag2 == 0)
							{
								LfoCase.vec_mainmode[jj].vec_dev.push_back(tmpWindows.vec_mainmode[ii].vec_dev[kk]);
							}
						}
						break;
					}
				}
				if (nFlag == 0)
				{
					tmpWindows.vec_mainmode[ii].mode.start_time = tmpWindows.start_time;
					tmpWindows.vec_mainmode[ii].mode.end_time = tmpWindows.end_time;
					sprintf(m_LogString, "[提示]新增模式频率[%f],开始时间["RTDB_TIME_FMT"],结束时间["RTDB_TIME_FMT"]", tmpWindows.vec_mainmode[ii].mode.freq, tmpWindows.vec_mainmode[ii].mode.start_time, tmpWindows.vec_mainmode[ii].mode.end_time);
					MessShow(m_LogString, LTINFO);
					if (tmpWindows.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_min)
					{
						sprintf(tmpWindows.vec_mainmode[ii].lfo_type, "%s", "超低频振荡");
					}
					else if (tmpWindows.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_min && tmpWindows.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_max)
					{
						sprintf(tmpWindows.vec_mainmode[ii].lfo_type, "%s", "低频振荡");
					}
					else if (tmpWindows.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_max)
					{
						sprintf(tmpWindows.vec_mainmode[ii].lfo_type, "%s", "高频振荡");
					}
					LfoCase.vec_mainmode.push_back(tmpWindows.vec_mainmode[ii]);
				}
			}
			sprintf(m_LogString, "[提示]统计后模式数目[%d]", LfoCase.vec_mainmode.size());
			MessShow(m_LogString, LTINFO);
			sort(LfoCase.vec_mainmode.begin(), LfoCase.vec_mainmode.end(), g_SortModeShapeByDamp);
			for (jj = 0; jj < LfoCase.vec_mainmode.size(); jj++)
			{
				LfoCase.vec_mainmode[jj].gn_num = 0;
				LfoCase.vec_mainmode[jj].line_num = 0;
				LfoCase.vec_mainmode[jj].tr_mun = 0;
				for (ii = 0; ii < LfoCase.vec_mainmode[jj].vec_dev.size(); ii++)
				{
					if (LfoCase.vec_mainmode[jj].vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
					{
						LfoCase.vec_mainmode[jj].gn_num++;
					}
					if (LfoCase.vec_mainmode[jj].vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
					{
						LfoCase.vec_mainmode[jj].line_num++;
					}
					if (LfoCase.vec_mainmode[jj].vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
					{
						LfoCase.vec_mainmode[jj].tr_mun++;
					}
				}
			}
		}
		if (LfoCase.stat >= MENU_LFO_DEV_STS_WRN)
		{
			if (m_LfoPara.is_eng == 1)
			{
				sprintf(LfoCase.case_desc, "%s power oscillation happening", PT_GetTimeStringBySeconds(LfoCase.start_time));
			}
			else
			{
#ifdef _GUANGDONG
				sprintf(LfoCase.case_desc, "%s %s%s发生振荡,频率:%.3fHz", PT_GetTimeStringBySeconds(LfoCase.start_time), LfoCase.warn_dev.dev.fac_name, LfoCase.warn_dev.dev.dev_name, LfoCase.lead_mode.freq);
#else
				sprintf(LfoCase.case_desc, "%s %s发生振荡,频率:%.3fHz", PT_GetTimeStringBySeconds(LfoCase.start_time), LfoCase.warn_dev.dev.dev_name, LfoCase.lead_mode.freq);
#endif
			}
		}

		LfoCase.lfo_gn_num = 0;
		LfoCase.lfo_ln_num = 0;
		LfoCase.lfo_tr_num = 0;
		LfoCase.lfo_inf_num = 0;
		LfoCase.lfos_gn_num = 0;
		LfoCase.lfos_ln_num = 0;
		LfoCase.lfos_tr_num = 0;
		sprintf(LfoCase.lfos_gn_name, " ");
		for (ii = 0; ii < (int)LfoCase.vec_dev.size(); ii++)
		{
			// 			sprintf(m_LogString,"[提示]2是否振荡源[%d],设备类型[%d]",LfoCase.vec_dev[ii].mode.is_source,LfoCase.vec_dev[ii].dev.dev_type);
			// 			MessShow(m_LogString,LTINFO);			
			if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				LfoCase.lfo_gn_num++;
				if (LfoCase.vec_dev[ii].mode.is_source == 1 || LfoCase.vec_dev[ii].mode.eve_source)
				{
					LfoCase.vec_dev[ii].sourceflag = 1;
					LfoCase.lfos_gn_num++;
					sprintf(LfoCase.lfos_gn_name, "%s", LfoCase.vec_dev[ii].dev.dev_name);
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				LfoCase.lfo_ln_num++;
				if (LfoCase.vec_dev[ii].mode.is_source == 1 || LfoCase.vec_dev[ii].mode.eve_source)
				{
					LfoCase.vec_dev[ii].sourceflag = 1;
					LfoCase.lfos_ln_num++;
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				LfoCase.lfo_tr_num++;
				if (LfoCase.vec_dev[ii].mode.is_source == 1 || LfoCase.vec_dev[ii].mode.eve_source)
				{
					LfoCase.vec_dev[ii].sourceflag = 1;
					LfoCase.lfos_tr_num++;
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
			{
				LfoCase.lfo_inf_num++;
			}
			// 			if (LfoCase.vec_dev[ii].mode.pow_num < 250)
			// 			{
			// 				if (LfoCase.vec_dev[ii].mode.is_source == 1)
			// 				{
			// 					LfoCase.vec_dev[ii].mode.vec_pow[LfoCase.vec_dev[ii].mode.pow_num] = LfoCase.vec_dev[ii].mode.pow;
			// 					LfoCase.vec_dev[ii].mode.pow_num ++;
			// 					// 			nFlag = 0;
			// 					// 			for (jj=0;jj<LfoCase.sou_dev.size();jj++)
			// 					// 			{
			// 					// 				if (LfoCase.vec_dev[ii].dev.dev_id == LfoCase.sou_dev[jj].dev.dev_id)
			// 					// 				{
			// 					// 					nFlag = 1;
			// 					// 					LfoCase.sou_dev[jj].mode.vec_pow.push_back(LfoCase.vec_dev[ii].mode.pow);
			// 					// 					break;
			// 					// 				}
			// 					// 				if (nFlag == 0)
			// 					// 				{
			// 					// 					LfoCase.vec_dev[ii].mode.vec_pow.push_back(LfoCase.vec_dev[ii].mode.pow);
			// 					// 					LfoCase.sou_dev.push_back(LfoCase.vec_dev[ii]);
			// 					// 				}
			// 					// 			}
			// 				}
			// 				else
			// 				{
			// 					LfoCase.vec_dev[ii].mode.vec_pow[LfoCase.vec_dev[ii].mode.pow_num] = 999999;
			// 					LfoCase.vec_dev[ii].mode.pow_num ++;
			// 				}
			// 			}
		}
		if (LfoCase.lfos_gn_num > 0)
		{
			sprintf(LfoCase.lfos_des, "%s", LfoCase.lfos_gn_name);
		}
		else if (LfoCase.lfos_ln_num > 0 || LfoCase.lfos_tr_num > 0)
		{
			sprintf(LfoCase.lfos_des, "找到振荡源相关支路");
		}
		else
		{
			sprintf(LfoCase.lfos_des, "");
		}
		if ((LfoCase.lfos_gn_num + LfoCase.lfos_ln_num + LfoCase.lfos_tr_num) > 0)
		{
			LfoCase.is_lfo_source = MENU_LFO_MON_YES;
		}
		else
		{
			LfoCase.is_lfo_source = MENU_LFO_MON_NO;
		}
		sprintf(m_LogString, "[提示2]振荡源机组[%s],线路数目[%d],变压器数目[%d]", LfoCase.lfos_gn_name, LfoCase.lfos_ln_num, LfoCase.lfos_tr_num);
		MessShow(m_LogString, LTINFO);
		if (!LfoCase.vec_mainmode.empty())
		{
			for (int ii = 0; ii < LfoCase.vec_mainmode.size(); ii++)
			{
				if (LfoCase.vec_mainmode[ii].mode.stat >= MENU_LFO_DEV_STS_WRN)
				{
					warn_mode++;
					if (LfoCase.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_min)
					{
						if (bslfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_SLOW - 1);
						}
						bslfo = 1;
					}
					else if (LfoCase.vec_mainmode[ii].mode.freq > m_LfoPara.lfo_freq_max)
					{
						if (bhlfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_HIGH - 1);
						}
						bhlfo = 1;
					}
					else if (LfoCase.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_min && LfoCase.vec_mainmode[ii].mode.freq <= m_LfoPara.lfo_freq_max)
					{
						if (blfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_LOW - 1);
						}
						blfo = 1;
					}
				}
			}
		}
		if (warn_mode > 1)
		{
			LfoCase.lfo_modle = MENU_LFO_MULTI;
			sprintf(LfoCase.type_desc, "%s", "多模式");
		}
		else if (warn_mode == 1)
		{
			LfoCase.lfo_modle = MENU_LFO_SINGLE;
			sprintf(LfoCase.type_desc, "%s", "单模式");
		}
		else if (warn_mode < 1)
		{
			LfoCase.lfo_modle = 0;
		}
		LfoCase.lfo_type = warn_type;
		if (bslfo == 1)
		{
			strcat(LfoCase.type_desc, "超低频振荡 ");
		}
		if (blfo == 1)
		{
			strcat(LfoCase.type_desc, "低频振荡 ");
		}
		if (bhlfo == 1)
		{
			strcat(LfoCase.type_desc, "高频振荡 ");
		}
#ifdef _LFO_HIS_LIB_
		PowAnaDoing(LfoCase);
		sprintf(m_LogString, "[提示：]计算进度[%f]", LfoCase.progress);
		MessShow(m_LogString, LTINFO);
#endif	
		// 		}
	}
	else if (save_flag == LFO_SAVE_FLAG_END)
	{
		// 更新振荡开始时间
// 		sprintf(m_LogString,"[提示]更新窗口开始时间11["INT_REC_FMT" "INT_REC_FMT"]，结束时间["INT_REC_FMT" "INT_REC_FMT"]",tmpWindows.start_time,LfoCase.start_time,tmpWindows.end_time,LfoCase.end_time);
// 		MessShow(m_LogString,LTINFO);
//  		if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
//  		{
// 			if ( tmpWindows.start_time < LfoCase.start_time)
// 			{
// 				LfoCase.start_time = tmpWindows.start_time;
// 			}
// 		}
		// 本轮安全，不更新振荡结束时间
// 		if ( tmpMonSys.end_time > LfoCase.end_time)
// 		{
// 			LfoCase.end_time = tmpMonSys.end_time;
// 		}
		ShowProMem("振荡结束");

		LfoCase.hold_time = LfoCase.end_time - LfoCase.start_time;
		LfoCase.stat = tmpWindows.stat;
		if (LfoCase.stat >= MENU_LFO_DEV_STS_WRN)
		{
			if (m_LfoPara.is_eng == 1)
			{
				sprintf(LfoCase.case_desc, "%s power oscillation happening", PT_GetTimeStringBySeconds(LfoCase.start_time));
			}
			else
			{
#ifdef _GUANGDONG
				sprintf(LfoCase.case_desc, "%s %s%s发生振荡,频率:%.3fHz", PT_GetTimeStringBySeconds(LfoCase.start_time), LfoCase.warn_dev.dev.fac_name, LfoCase.warn_dev.dev.dev_name, LfoCase.lead_mode.freq);
#else
				sprintf(LfoCase.case_desc, "%s %s发生振荡,频率:%.3fHz", PT_GetTimeStringBySeconds(LfoCase.start_time), LfoCase.warn_dev.dev.dev_name, LfoCase.lead_mode.freq);
#endif
			}
		}

		LfoCase.lfo_gn_num = 0;
		LfoCase.lfo_ln_num = 0;
		LfoCase.lfo_tr_num = 0;
		LfoCase.lfo_inf_num = 0;
		LfoCase.lfos_gn_num = 0;
		LfoCase.lfos_ln_num = 0;
		LfoCase.lfos_tr_num = 0;
		sprintf(LfoCase.lfos_gn_name, " ");
		for (ii = 0; ii < (int)LfoCase.vec_dev.size(); ii++)
		{
			sprintf(m_LogString, "[提示]1是否振荡源[%d],设备类型[%d]", LfoCase.vec_dev[ii].mode.is_source, LfoCase.vec_dev[ii].dev.dev_type);
			MessShow(m_LogString, LTINFO);
			if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				LfoCase.lfo_gn_num++;
				if (LfoCase.vec_dev[ii].sourceflag == 1)
				{
					LfoCase.lfos_gn_num++;
					sprintf(LfoCase.lfos_gn_name, "%s", LfoCase.vec_dev[ii].dev.dev_name);
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				LfoCase.lfo_ln_num++;
				if (LfoCase.vec_dev[ii].sourceflag == 1)
				{
					LfoCase.lfos_ln_num++;
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				LfoCase.lfo_tr_num++;
				if (LfoCase.vec_dev[ii].sourceflag == 1)
				{
					LfoCase.lfos_tr_num++;
				}
			}
			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
			{
				LfoCase.lfo_inf_num++;
			}
			// 			if (LfoCase.vec_dev[ii].mode.pow_num < 250)
			// 			{
			// 				if (LfoCase.vec_dev[ii].mode.is_source == 1)
			// 				{
			// 					LfoCase.vec_dev[ii].mode.vec_pow[LfoCase.vec_dev[ii].mode.pow_num] = LfoCase.vec_dev[ii].mode.pow;
			// 					LfoCase.vec_dev[ii].mode.pow_num ++;
			// 					// 			nFlag = 0;
			// 					// 			for (jj=0;jj<LfoCase.sou_dev.size();jj++)
			// 					// 			{
			// 					// 				if (LfoCase.vec_dev[ii].dev.dev_id == LfoCase.sou_dev[jj].dev.dev_id)
			// 					// 				{
			// 					// 					nFlag = 1;
			// 					// 					LfoCase.sou_dev[jj].mode.vec_pow.push_back(LfoCase.vec_dev[ii].mode.pow);
			// 					// 					break;
			// 					// 				}
			// 					// 				if (nFlag == 0)
			// 					// 				{
			// 					// 					LfoCase.vec_dev[ii].mode.vec_pow.push_back(LfoCase.vec_dev[ii].mode.pow);
			// 					// 					LfoCase.sou_dev.push_back(LfoCase.vec_dev[ii]);
			// 					// 				}
			// 					// 			}
			// 				}
			// 				else
			// 				{
			// 					LfoCase.vec_dev[ii].mode.vec_pow[LfoCase.vec_dev[ii].mode.pow_num] = 999999;
			// 					LfoCase.vec_dev[ii].mode.pow_num ++;
			// 				}
			// 			}
		}
		if (LfoCase.lfos_gn_num > 0)
		{
			sprintf(LfoCase.lfos_des, "%s", LfoCase.lfos_gn_name);
		}
		else if (LfoCase.lfos_ln_num > 0 || LfoCase.lfos_tr_num > 0)
		{
			sprintf(LfoCase.lfos_des, "找到振荡源相关支路");
		}
		else
		{
			sprintf(LfoCase.lfos_des, "");
		}
		if ((LfoCase.lfos_gn_num + LfoCase.lfos_ln_num + LfoCase.lfos_tr_num) > 0)
		{
			LfoCase.is_lfo_source = MENU_LFO_MON_YES;
		}
		else
		{
			LfoCase.is_lfo_source = MENU_LFO_MON_NO;
		}
		sprintf(m_LogString, "[提示3]振荡源机组[%s],线路数目[%d],变压器数目[%d]", LfoCase.lfos_gn_name, LfoCase.lfos_ln_num, LfoCase.lfos_tr_num);
		MessShow(m_LogString, LTINFO);
		if (!LfoCase.vec_mainmode.empty())
		{
			for (int ii = 0; ii < LfoCase.vec_mainmode.size(); ii++)
			{
				if (LfoCase.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_min)
				{
					sprintf(LfoCase.vec_mainmode[ii].lfo_type, "超低频振荡");
				}
				else if (LfoCase.vec_mainmode[ii].mode.freq > m_LfoPara.lfo_freq_max)
				{
					sprintf(LfoCase.vec_mainmode[ii].lfo_type, "高频振荡");
				}
				else
				{
					sprintf(LfoCase.vec_mainmode[ii].lfo_type, "低频振荡");
				}
				LfoCase.vec_mainmode[ii].gn_num = 0;
				LfoCase.vec_mainmode[ii].line_num = 0;
				LfoCase.vec_mainmode[ii].tr_mun = 0;
				for (int jj = 0; jj < LfoCase.vec_mainmode[ii].vec_dev.size(); jj++)
				{
					if (LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_type == MENU_LFO_DEV_GEN)
					{
						LfoCase.vec_mainmode[ii].gn_num++;
					}
					if (LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_type == MENU_LFO_DEV_LINE)
					{
						LfoCase.vec_mainmode[ii].line_num++;
					}
					if (LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_type == MENU_LFO_DEV_TR)
					{
						LfoCase.vec_mainmode[ii].tr_mun++;
					}
				}
				if (LfoCase.vec_mainmode[ii].mode.stat >= MENU_LFO_DEV_STS_WRN)
				{
					warn_mode++;
					if (LfoCase.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_min)
					{
						if (bslfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_SLOW - 1);
						}
						bslfo = 1;
					}
					else if (LfoCase.vec_mainmode[ii].mode.freq > m_LfoPara.lfo_freq_max)
					{
						if (bhlfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_HIGH - 1);
						}
						bhlfo = 1;
					}
					else if (LfoCase.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_min && LfoCase.vec_mainmode[ii].mode.freq <= m_LfoPara.lfo_freq_max)
					{
						if (blfo == 0)
						{
							warn_type += PODB_GetVarNsquare(2, MENU_LFO_LOW - 1);
						}
						blfo = 1;
					}
				}
			}
		}
		if (warn_mode > 1)
		{
			LfoCase.lfo_modle = MENU_LFO_MULTI;
			sprintf(LfoCase.type_desc, "%s", "多模式");
		}
		else if (warn_mode == 1)
		{
			LfoCase.lfo_modle = MENU_LFO_SINGLE;
			sprintf(LfoCase.type_desc, "%s", "单模式");
		}
		else if (warn_mode < 1)
		{
			LfoCase.lfo_modle = 0;
		}
		LfoCase.lfo_type = warn_type;
		if (bslfo == 1)
		{
			strcat(LfoCase.type_desc, "超低频振荡 ");
		}
		if (blfo == 1)
		{
			strcat(LfoCase.type_desc, "低频振荡 ");
		}
		if (bhlfo == 1)
		{
			strcat(LfoCase.type_desc, "高频振荡 ");
		}
		if (LfoCase.lfos_gn_num + LfoCase.lfos_ln_num + LfoCase.lfos_tr_num > 0)
		{
			sprintf(LfoCase.lfo_ana_des, "%s", LfoCase.lfos_des);
		}
		else
		{
			sprintf(LfoCase.lfo_ana_des, "%s", LfoCase.center_desc);
		}
		ShowProMem("模式类型赋值");

		//LfoCase.lead_mode = tmpMonSys.lead_mode;
// 		if (!tmpMonSys.vec_mode.empty())
// 		{
// 			// TODO:阻尼比取平均值，是否可取最小值？
// 			LfoCase.aver_freq = (LfoCase.aver_freq + tmpMonSys.vec_mode.front().freq)/2.0f;
// 			LfoCase.aver_damp = (LfoCase.aver_damp + tmpMonSys.vec_mode.front().damp)/2.0f;
// 		}

		// 暂只保存本轮窗口信息
// 		if (!VecLfoWindow.empty())
// 		{
// 			LfoCase.vec_windows.push_back(tmpMonSys);
// 		}

		// 计算平均频率
// 		LfoCase.lead_mode.freq = 0.0f;
// 		for ( ii =0; ii<(int)LfoCase.vec_windows.size(); ii++ )
// 		{
// 			LfoCase.lead_mode.freq += LfoCase.vec_windows[ii].lead_mode.freq;
// 			if (LfoCase.vec_windows[ii].lead_mode.damp < LfoCase.lead_mode.damp)
// 			{
// 				LfoCase.lead_mode.damp = LfoCase.vec_windows[ii].lead_mode.damp;
// 			}
// 		}
// 		if (!LfoCase.vec_windows.empty())
// 		{
// 			LfoCase.lead_mode.freq = LfoCase.lead_mode.freq / (int)LfoCase.vec_windows.size();
// 		}
// 		

		// 保存窗口信息中的振荡设备
// 		for ( ii =0; ii<(int)tmpWindows.vec_dev.size(); ii++)
// 		{
// 			nFlag = 0;
// 			for (jj =0; jj<(int)LfoCase.vec_dev.size(); jj++ )
// 			{
// 				if (tmpWindows.vec_dev[ii].dev.dev_id == LfoCase.vec_dev[jj].dev.dev_id)
// 				{
// 					nFlag = 1;
// 					// 更新波形信息
// 					if (tmpWindows.vec_dev[ii].curve.max_val > LfoCase.vec_dev[jj].curve.max_val)
// 					{
// 						LfoCase.vec_dev[jj].curve.max_val = tmpWindows.vec_dev[ii].curve.max_val;
// 						LfoCase.vec_dev[jj].curve.max_val_t = tmpWindows.vec_dev[ii].curve.max_val_t;
// 					}
// 					if (tmpWindows.vec_dev[ii].curve.min_val < LfoCase.vec_dev[jj].curve.min_val)
// 					{
// 						LfoCase.vec_dev[jj].curve.min_val = tmpWindows.vec_dev[ii].curve.min_val;
// 						LfoCase.vec_dev[jj].curve.min_val_t = tmpWindows.vec_dev[ii].curve.min_val_t;
// 					}
// 					// 更新开始时间
// 					if (tmpWindows.vec_dev[ii].start_time < LfoCase.vec_dev[jj].start_time)
// 					{
// 						LfoCase.vec_dev[jj].start_time = tmpWindows.vec_dev[ii].start_time;
// 					}
// 					// 更新结束时间
// 					if (tmpWindows.vec_dev[ii].end_time > LfoCase.vec_dev[jj].end_time)
// 					{
// 						LfoCase.vec_dev[jj].end_time = tmpWindows.vec_dev[ii].end_time;
// 					}
// 					// 更新持续时间
// 					LfoCase.vec_dev[jj].hold_time = LfoCase.vec_dev[jj].end_time - LfoCase.vec_dev[jj].start_time;
// 					sprintf(m_LogString,"[yjj]22["INT_REC_FMT"]，开始时间["INT_REC_FMT"],结束时间["INT_REC_FMT"],持续时间[%d]", LfoCase.vec_dev[jj].dev.dev_id,LfoCase.vec_dev[jj].start_time,LfoCase.vec_dev[jj].end_time,LfoCase.vec_dev[jj].hold_time);
// 						MessShow(m_LogString,LTINFO);	 
// 					// 添加模式信息
// 					LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].mode);
// 					break;
// 				}
// 			}
// 			if (nFlag == 0)
// 			{
// 				// 新增设备
// 				memset((void *)&tmpDev,0,sizeof(LFO_DEV_INFO));
// 				tmpDev = tmpWindows.vec_dev[ii];
// 				tmpDev.start_time = tmpWindows.start_time;
// 				tmpDev.end_time = tmpWindows.end_time;
// 				tmpDev.hold_time = tmpDev.end_time - tmpDev.start_time;
// 				tmpDev.vec_lead_mode.push_back(tmpWindows.vec_dev[ii].mode);
// 				LfoCase.vec_dev.push_back(tmpWindows.vec_dev[ii]);
// 			}
// 		}

		// 保存主导模式
// 		if (!m_VecModeShape.empty())
// 		{
//             for ( ii =0; ii<(int)m_VecModeShape.size(); ii++)
//             {
//                 LfoCase.vec_modeshape.push_back(m_VecModeShape[ii]);
//             }
// 		}

// 		sprintf(m_LogString,"[提示]事件ID["INT_REC_FMT"]",INT_REC_VAL(LfoCase.event_id));
// 		MessShow(m_LogString,LTINFO);
		int end_time = LfoCase.end_time;
		int hold_time = LfoCase.hold_time;
		// 发送告警消息
		SendWarnMessage(m_MonSys, LFO_SAVE_WARN, end_time, hold_time, 0);	
		ShowProMem("发送振荡结束告警消息");

		//振荡事件采样
#ifdef _PODB_SAMPLE_
		sprintf(m_LogString, "振荡事件采样");
		MessShow(m_LogString, LTINFO);
		SampleLfoPara(LfoCase);

		SampleHisEve(LfoCase);

		SampleHisCase(LfoCase);

		SampleHisShape(LfoCase);

		SampleHisMode(LfoCase);
		ShowProMem("振荡事件采样");
#endif		
		SaveLfoCaseFile(LfoCase);

		// 生成案例保存交互文件		
		SaveCaseCurve(LfoCase);

#ifdef _LFO_HIS_LIB_
		g_pAnaObj->main_RunCtrCom = MENU_LFO_PRO_CTL_STOP;
		g_IsWarn = 1;
		sprintf(m_LogString, "功率波动案例分析结束,振荡结束,调用交互函数,传回振荡结果");
		MessShow(m_LogString, LTINFO);
		sprintf(m_LogString, "功率波动事件发生时间[%s],消失时间[%s],振荡设备个数[%d]", PT_GetTimeStringBySeconds(LfoCase.start_time), PT_GetTimeStringBySeconds(LfoCase.end_time), (int)LfoCase.vec_dev.size());
		MessShow(m_LogString, LTINFO);
		PowAnaEnd(LfoCase);
#endif	

#ifndef  _WINDOWS32		
		// 保存案例到三态数据
		SaveCaseTmd(LfoCase);
#endif
		// 清空LFO参数表中不需要显示的域,更新最新一次振荡事件
		sprintf(m_LogString, "清空LFO参数表中不需要显示的域,更新最新一次振荡事件");
		MessShow(m_LogString, LTINFO);
		ClearLfoParaTable(LfoCase);
		// 振荡结束时更新振荡
		if (UpdateWDSTable(LfoCase) < 0)
		{
			sprintf(m_LogString, "更新LFO实时设备模式全表或LFO实时设备主导模式表错误");
			MessShow(m_LogString, LTERROR);
		}

		if (UpdateLfoGnTable(LfoCase, LFO_SAVE_FLAG_END) < 0)
		{
			sprintf(m_LogString, "更新LFO发电机表错误");
			MessShow(m_LogString, LTERROR);
		}

		if (UpdateLfoLnTable(LfoCase, LFO_SAVE_FLAG_END) < 0)
		{
			sprintf(m_LogString, "更新LFO交流线段表错误");
			MessShow(m_LogString, LTERROR);
		}

		if (UpdateLfoTrTable(LfoCase, LFO_SAVE_FLAG_END) < 0)
		{
			sprintf(m_LogString, "更新LFO变压器表错误");
			MessShow(m_LogString, LTERROR);
		}

		if (UpdateLfoIntTable(LfoCase, LFO_SAVE_FLAG_END) < 0)
		{
			sprintf(m_LogString, "更新LFO联络线走廊表错误");
			MessShow(m_LogString, LTERROR);
		}

		// 清空变量
		sprintf(m_LogString, "清空LFOCASE");
		MessShow(m_LogString, LTINFO);
		ClearLfoCase(LfoCase);
		ShowProMem("清空LFOCASE");
	}
	else
	{
		sprintf(m_LogString, "标志错误[%d]", save_flag);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CLfoAnaObj::SaveMonResult(vector<LFO_DEV_INFO>& VecMonDev, LFO_SYS_INFO& MonSys)
{
#ifndef _LFO_HIS_LIB_
	if (UpdateLfoMonPara(MonSys) < 0)
	{
		sprintf(m_LogString, "更新LFO参数表监视信息错误");
		MessShow(m_LogString, LTERROR);
	}
	if (UpdateMonGnTable(VecMonDev) < 0)
	{
		sprintf(m_LogString, "更新LFO发电机表监视信息错误");
		MessShow(m_LogString, LTERROR);
	}

	if (UpdateMonLnTable(VecMonDev) < 0)
	{
		sprintf(m_LogString, "更新LFO交流线路表监视信息错误");
		MessShow(m_LogString, LTERROR);
	}

	if (UpdateMonTrTable(VecMonDev) < 0)
	{
		sprintf(m_LogString, "更新LFO变压器表监视信息错误");
		MessShow(m_LogString, LTERROR);
	}

	if (UpdateMonIntTable(VecMonDev) < 0)
	{
		sprintf(m_LogString, "更新LFO联络新走廊表监视信息错误");
		MessShow(m_LogString, LTERROR);
	}

#endif
	return 0;
}


//************************************
// Method:    CacheWindowsData
// FullName:  CLfoAnaObj::CacheWindowsData
// Access:    public 
// Returns:   int
// Qualifier:
// 缓存每个分析窗口结果，超过给定值就删除旧结果，添加新结果。若同一时刻多个频率段都存在告警，则保存多个频率段系统状态结果。
// 输入：系统状态结构体(LFO_SYS_INFO)，安全状态，振荡设备个数，主导模式中参与设备个数，振荡设备信息结构（所有振荡设备）vector< LFO_DEV_INFO >，模态分析结构（所有模式汇总）vector<LFO_MODE_SHAPE>
// 输出：每个分析窗口结构(LFO_WINDOW_INFO)，事件ID，窗口ID，分析频率段，振荡开始时间，振荡结束时间，振荡持续时间，平均频率，最小阻尼比，触发告警设备，系统状态结构vector<LFO_SYS_INFO>
//************************************
int CLfoAnaObj::CacheWindowsData(LFO_SYS_INFO MonSys, vector<LFO_SYS_INFO>& VecLfoWindow)
{
	// 	if ((int)VecLfoWindow.size()>m_LfoPara.cache_windows_num)
	// 	{
	// 		vector<LFO_SYS_INFO>().swap(VecLfoWindow);
	// 		VecLfoWindow.erase(VecLfoWindow.begin());
	// // 		sprintf(m_LogString,"delete window_num=%d",(int)VecLfoWindow.size());
	// // 		MessShow(m_LogString,LTINFO);
	// 	}

	ClearLfoSysInfo(VecLfoWindow);

	VecLfoWindow.push_back(MonSys);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    UpdateMonDevTable
// FullName:  CLfoAnaObj::UpdateMonDevTable
// Access:    public 
// Returns:   int
// Qualifier:
// 输入：设备信息结构体(LFO_DEV_INFO)，数据/安全状态，主导模式（实时模式中的能量最大模式）(LFO_MODE_INFO)，模态分析中能量最大的模式对应的参与设备中包含该设备的实时模式，参与因子，设备分群(LFO_MODE_INFO)
// 输出：更新设备表
//************************************
// int CLfoAnaObj::UpdateMonDevTable(vector<LFO_DEV_INFO> VecMonDev)
// {
//     return 0;
// }

//************************************
// Method:    UpdateLfoParaTable
// FullName:  CLfoAnaObj::UpdateLfoParaTable
// Access:    public 
// Returns:   int
// Qualifier:
// 输入：振荡事件结构(LFO_CASE_INFO)，振荡开始时间，振荡结束时间，振荡持续时间，平均频率，最小阻尼比，触发告警设备
//************************************
int CLfoAnaObj::UpdateLfoParaTable(LFO_CASE_INFO& LfoCase)
{
	if (LfoCase.start_time <= 0)
	{
		sprintf(m_LogString, "LfoCase为空,没有振荡事件");
		MessShow(m_LogString);
		return 0;
	}
#ifdef _WAMAP_
	LFO_PARA_TABLE_WRITE tmpLfoParaInfo;
	int warn_mode = 0;
	int warn_type = 0;
	int bslfo = 0;
	int blfo = 0;
	int bhlfo = 0;
	memset((void*)&tmpLfoParaInfo, 0, sizeof(LFO_PARA_TABLE_WRITE));
	sprintf(tmpLfoParaInfo.case_des, "%s", LfoCase.case_desc);
	tmpLfoParaInfo.lfo_bgn = LfoCase.start_time;
	// 	sprintf(tmpLfoParaInfo.lfo_bgn_des,"%s",PT_GetChineseTimeStringBySeconds(LfoCase.start_time));//用字符串显示开始时间
	tmpLfoParaInfo.lfo_bgn2 = LfoCase.start_time;
	sprintf(tmpLfoParaInfo.lfo_bgn_des2, "%s", PT_GetChineseTimeStringBySeconds(LfoCase.start_time));//用字符串显示开始时间2
	tmpLfoParaInfo.lfo_end = LfoCase.end_time;
	// 	sprintf(tmpLfoParaInfo.lfo_end_des,"%s",PT_GetChineseTimeStringBySeconds(LfoCase.end_time));//用字符串显示开始时间2
	sprintf(tmpLfoParaInfo.lfo_end_des2, "%s", PT_GetChineseTimeStringBySeconds(LfoCase.end_time));//用字符串显示开始时间2
	sprintf(tmpLfoParaInfo.mon_use_time, "%s", PT_GetStringOfTimeInterval(LfoCase.hold_time * 1000));
	sprintf(tmpLfoParaInfo.lfo_time2, "%s", PT_GetStringOfTimeInterval(LfoCase.hold_time * 1000));
	tmpLfoParaInfo.warn_dev_id2 = LfoCase.warn_dev.dev.dev_id;
	if (m_LfoPara.is_add_facname == 1)
	{
		sprintf(tmpLfoParaInfo.warn_dev_name2, "%s%s", LfoCase.warn_dev.dev.fac_name, LfoCase.warn_dev.dev.dev_name);
	}
	else
	{
		sprintf(tmpLfoParaInfo.warn_dev_name2, "%s", LfoCase.warn_dev.dev.dev_name);
	}

	tmpLfoParaInfo.warn_freq = LfoCase.lead_mode.freq;
	tmpLfoParaInfo.warn_damp = LfoCase.lead_mode.damp;
	tmpLfoParaInfo.aver_freq = LfoCase.lead_mode.freq;
	tmpLfoParaInfo.lead_freq2 = LfoCase.vec_windows.back().lead_dev.mode.freq;
	tmpLfoParaInfo.lead_damp2 = LfoCase.vec_windows.back().lead_dev.mode.damp;
	tmpLfoParaInfo.lfo_gn_num = LfoCase.lfo_gn_num;
	tmpLfoParaInfo.lfo_ln_num = LfoCase.lfo_ln_num;
	tmpLfoParaInfo.lfo_tr_num = LfoCase.lfo_tr_num;
	tmpLfoParaInfo.lfo_inf_num = LfoCase.lfo_inf_num;

	sprintf(tmpLfoParaInfo.lfos_gn_name, "%s", LfoCase.lfos_gn_name);	/* 振荡源机组名称 */
	tmpLfoParaInfo.lfos_gn_num = LfoCase.lfos_gn_num;	/* 振荡源机组数目 */
	tmpLfoParaInfo.lfos_ln_num = LfoCase.lfos_ln_num;	/* 振荡源线路数目 */
	tmpLfoParaInfo.lfos_tr_num = LfoCase.lfos_tr_num;	/* 振荡源变压器数目 */
// 	sprintf(tmpLfoParaInfo.lfos_des,"%s",LfoCase.lfos_des);	/* 振荡源描述 */
	sprintf(tmpLfoParaInfo.lfos_des2, "%s", LfoCase.lfos_des);	/* 振荡源描述 */
	tmpLfoParaInfo.mode_num2 = LfoCase.vec_mainmode.size();
	sprintf(tmpLfoParaInfo.center_desc2, "%s", LfoCase.center_desc);	/* 振荡中心描述2 */
	sprintf(tmpLfoParaInfo.center_desc, "%s", LfoCase.vec_windows.back().center_desc);	/* 振荡中心描述 */
	if ((tmpLfoParaInfo.lfos_gn_num + tmpLfoParaInfo.lfos_ln_num + tmpLfoParaInfo.lfos_tr_num) > 0)
	{
		// 		tmpLfoParaInfo.is_lfo_source = MENU_LFO_MENU_YES;
		tmpLfoParaInfo.is_lfo_source2 = MENU_LFO_MENU_YES;
		// 		sprintf(tmpLfoParaInfo.is_source_des,"√");
		sprintf(tmpLfoParaInfo.is_source_des2, "√");
		sprintf(tmpLfoParaInfo.lfo_ana_des2, "%s;%s", tmpLfoParaInfo.lfos_des2, tmpLfoParaInfo.center_desc2);	// 分析描述使用振荡源描述
// 		sprintf(tmpLfoParaInfo.lfo_ana_des,"振荡源:%s",tmpLfoParaInfo.lfos_des2);	// 分析描述使用振荡源描述
	}
	else
	{
		// 		tmpLfoParaInfo.is_lfo_source = MENU_LFO_MENU_NO;
		tmpLfoParaInfo.is_lfo_source2 = MENU_LFO_MENU_NO;
		// 		sprintf(tmpLfoParaInfo.is_source_des,"×");
		sprintf(tmpLfoParaInfo.is_source_des2, "×");
		sprintf(tmpLfoParaInfo.lfo_ana_des2, "%s;%s", tmpLfoParaInfo.center_desc2, tmpLfoParaInfo.lfos_des2);	//分析描述使用振荡中心描述
// 		if (strlen(tmpLfoParaInfo.center_desc) != 0)
// 		{
// 			sprintf(tmpLfoParaInfo.lfo_ana_des,"振荡中心:%s",tmpLfoParaInfo.center_desc);	//分析描述使用振荡中心描述
// 		}
// 		else
// 		{
// 			sprintf(tmpLfoParaInfo.lfo_ana_des,"%s",tmpLfoParaInfo.center_desc);	//分析描述使用振荡中心描述
// 		}
	}

	if (!LfoCase.vec_mainmode.empty())
	{
		for (int ii = 0; ii < LfoCase.vec_mainmode.size(); ii++)
		{
			if (LfoCase.vec_mainmode[ii].mode.stat >= MENU_LFO_DEV_STS_WRN)
			{
				warn_mode++;
				if (LfoCase.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_min)
				{
					if (bslfo == 0)
					{
						warn_type += PODB_GetVarNsquare(2, MENU_LFO_SLOW - 1);
					}
					bslfo = 1;
				}
				else if (LfoCase.vec_mainmode[ii].mode.freq > m_LfoPara.lfo_freq_max)
				{
					if (bhlfo == 0)
					{
						warn_type += PODB_GetVarNsquare(2, MENU_LFO_HIGH - 1);
					}
					bhlfo = 1;
				}
				else if (LfoCase.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_min && LfoCase.vec_mainmode[ii].mode.freq <= m_LfoPara.lfo_freq_max)
				{
					if (blfo == 0)
					{
						warn_type += PODB_GetVarNsquare(2, MENU_LFO_LOW - 1);
					}
					blfo = 1;
				}
			}
		}
	}
	if (warn_mode > 1)
	{
		tmpLfoParaInfo.lfo_modle2 = MENU_LFO_MULTI;
		sprintf(tmpLfoParaInfo.case_des2, "%s", "多模式");
	}
	else if (warn_mode == 1)
	{
		tmpLfoParaInfo.lfo_modle2 = MENU_LFO_SINGLE;
		sprintf(tmpLfoParaInfo.case_des2, "%s", "单模式");
	}
	else if (warn_mode < 1)
	{
		tmpLfoParaInfo.lfo_modle2 = 0;
	}
	// 	tmpLfoParaInfo.lfo_modle2 = tmpLfoParaInfo.lfo_modle;
	// 	tmpLfoParaInfo.lfo_type = warn_type;
	tmpLfoParaInfo.lfo_type2 = warn_type;
	if (blfo == 1)
	{
		strcat(tmpLfoParaInfo.case_des2, "低频振荡 ");
	}
	if (bslfo == 1)
	{
		strcat(tmpLfoParaInfo.case_des2, "超低频振荡 ");
	}
	if (bhlfo == 1)
	{
		strcat(tmpLfoParaInfo.case_des2, "高频振荡 ");
	}



	sprintf(m_LogString, "振荡中心描述[%s]振荡源描述[%s],模式个数[%d]", tmpLfoParaInfo.center_desc2, tmpLfoParaInfo.lfos_des2, tmpLfoParaInfo.mode_num2);
	MessShow(m_LogString);

	// 	if (LfoCase.stat == MENU_LFO_DEV_STS_WRN)
	// 	{
	// 		sprintf(tmpLfoParaInfo.case_type_des,"%s","功率波动");
	// 	}
	// 	else if (LfoCase.stat == MENU_LFO_DEV_STS_WRN2)
	// 	{
	// 		sprintf(tmpLfoParaInfo.case_type_des,"%s","低频振荡");
	// 	}

	INT_REC record_id = PODB_GetDevIdByKeyID(LFO_PARA_NO_TAB, 2);
	PODB_TableModifyByKey((const char*)&record_id, LFO_PARA_TABLE_WRITE_FLD, (const char*)&tmpLfoParaInfo, sizeof(LFO_PARA_TABLE_WRITE), LFO_PARA_NO_TAB);

#endif
	return 0;
}
//振荡结束时,清空LFO参数表部分域,更新最新一次振荡事件信息
int CLfoAnaObj::ClearLfoParaTable(LFO_CASE_INFO& LfoCase)
{
#ifdef _WAMAP_

	//清空LFO参数表部分域
	LFO_PARA_TABLE_CLEAR tmpLfoParaInfo;
	memset((void*)&tmpLfoParaInfo, 0, sizeof(LFO_PARA_TABLE_CLEAR));
	tmpLfoParaInfo.lfo_bgn2 = 0;
	tmpLfoParaInfo.lead_freq2 = UNDISPLAY_VALUE;
	tmpLfoParaInfo.lead_damp2 = UNDISPLAY_VALUE;
	sprintf(tmpLfoParaInfo.lfo_bgn_des2, "");//借域显示开始时间2
	sprintf(tmpLfoParaInfo.lfo_end_des2, "");//借域显示开始时间2
	sprintf(tmpLfoParaInfo.is_source_des2, "");//借域显示开始时间2
	sprintf(tmpLfoParaInfo.warn_dev_name2, "");//借域显示开始时间2
	tmpLfoParaInfo.lfo_modle2 = MENU_LFO_NOMODE;
	tmpLfoParaInfo.lfo_type2 = PODB_GetVarNsquare(2, MENU_LFO_SAFE - 1);
	tmpLfoParaInfo.is_lfo_source2 = 0;
	tmpLfoParaInfo.stat2 = MENU_LFO_DEV_STS_SCT;
	tmpLfoParaInfo.stat_val2 = 2;
	sprintf(tmpLfoParaInfo.stat_ctx2, "");
	//更新最新一次振荡事件信息
	sprintf(tmpLfoParaInfo.lfo_bgn_des, "%s", PT_GetChineseTimeStringBySeconds(LfoCase.start_time));//用字符串显示开始时间
	sprintf(tmpLfoParaInfo.lfo_end_des, "%s", PT_GetChineseTimeStringBySeconds(LfoCase.end_time));//用字符串显示开始时间
	sprintf(tmpLfoParaInfo.lfo_time, "%s", PT_GetStringOfTimeInterval(LfoCase.hold_time * 1000));
	if (m_LfoPara.is_add_facname == 1)
	{
		sprintf(tmpLfoParaInfo.warn_dev_name, "%s%s", LfoCase.warn_dev.dev.fac_name, LfoCase.warn_dev.dev.dev_name);
	}
	else
	{
		sprintf(tmpLfoParaInfo.warn_dev_name, "%s", LfoCase.warn_dev.dev.dev_name);
	}
	tmpLfoParaInfo.warn_dev_id = LfoCase.warn_dev.dev.dev_id;
	tmpLfoParaInfo.lfo_modle = LfoCase.lfo_modle;
	tmpLfoParaInfo.lfo_type = LfoCase.lfo_type;
	tmpLfoParaInfo.lead_freq = LfoCase.lead_mode.freq;
	tmpLfoParaInfo.lead_damp = LfoCase.lead_mode.damp;
	if ((LfoCase.lfos_gn_num + LfoCase.lfos_ln_num + LfoCase.lfos_tr_num) > 0)
	{
		tmpLfoParaInfo.is_lfo_source = MENU_LFO_MENU_YES;
		sprintf(tmpLfoParaInfo.is_source_des, "√");
		sprintf(tmpLfoParaInfo.lfo_ana_des, "振荡源:%s", LfoCase.lfos_des);	// 分析描述使用振荡源描述
	}
	else
	{
		tmpLfoParaInfo.is_lfo_source = MENU_LFO_MENU_NO;
		sprintf(tmpLfoParaInfo.is_source_des, "×");
		if (strlen(LfoCase.center_desc) != 0)
		{
			sprintf(tmpLfoParaInfo.lfo_ana_des, "振荡中心:%s", LfoCase.center_desc);	//分析描述使用振荡中心描述
		}
		else
		{
			sprintf(tmpLfoParaInfo.lfo_ana_des, "%s", LfoCase.center_desc);	//分析描述使用振荡中心描述
		}
	}

	if (!LfoCase.vec_mainmode.empty())
	{
		tmpLfoParaInfo.mon_fail2 = LfoCase.vec_mainmode[0].mode.freq;
		sprintf(tmpLfoParaInfo.mode_ana_des, "%s", LfoCase.vec_mainmode[0].lfo_ana_des);
		if (LfoCase.vec_mainmode[0].is_lfo_source == 1)
		{
			sprintf(tmpLfoParaInfo.mode_source_des, "√");
		}
		else
		{
			sprintf(tmpLfoParaInfo.mode_source_des, "×");
		}
	}
	INT_REC record_id = PODB_GetDevIdByKeyID(LFO_PARA_NO_TAB, 2);
	PODB_TableModifyByKey((const char*)&record_id, LFO_PARA_TABLE_CLEAR_FLD, (const char*)&tmpLfoParaInfo, sizeof(LFO_PARA_TABLE_CLEAR), LFO_PARA_NO_TAB);

#endif
	return 0;
}

int CLfoAnaObj::UpdateLfoFcTable(LFO_CASE_INFO& LfoCase, int save_flag)
{
	// 	if (LfoCase.cen_dev.empty())
	// 	{
	// 		return 0;
	// 	}
#ifdef _WAMAP_
	// 振荡设备填写振荡信息，非振荡设备填写空值
	vector<LFO_FC_TAB_CRE> tmpVecLfoFacInfo;
	GetLfoFac(tmpVecLfoFacInfo);
	if (tmpVecLfoFacInfo.empty())
	{
		return 0;
	}
	int ii, jj, nFlag;
	type_LFO_FC_TAB_WRITE tmpLfoDev;

	sprintf(m_LogString, "LfoCase.cen_dev.size()=[%d]", LfoCase.cen_dev.size());
	MessShow(m_LogString);
	for (ii = 0; ii < (int)tmpVecLfoFacInfo.size(); ii++)
	{
		nFlag = 0;
		memset((void*)&tmpLfoDev, 0, sizeof(type_LFO_FC_TAB_WRITE));
		for (jj = 0; jj < (int)LfoCase.cen_dev.size(); jj++)
		{
			sprintf(m_LogString, "UpdateLfoFcTable设备[%s]类型[%d]id["INT_REC_FMT"]", LfoCase.cen_dev[jj].dev.dev_name, LfoCase.cen_dev[jj].dev.dev_type, INT_REC_VAL(LfoCase.cen_dev[jj].dev.dev_id));
			MessShow(m_LogString);
			if (LfoCase.cen_dev[jj].dev.dev_type != MENU_LFO_DEV_FAC)
			{
				continue;
			}
			if (tmpVecLfoFacInfo[ii].fac_id == LfoCase.cen_dev[jj].dev.dev_id || tmpVecLfoFacInfo[ii].set_bus_id == LfoCase.cen_dev[jj].dev.dev_id)
			{

				tmpLfoDev.v_max = LfoCase.cen_dev[jj].curve.max_val;
				tmpLfoDev.v_min = LfoCase.cen_dev[jj].curve.min_val;
				tmpLfoDev.v_ampl = LfoCase.cen_dev[jj].curve.max_ampl;
				tmpLfoDev.lfo_v_max = LfoCase.cen_dev[jj].curve.max_val;
				tmpLfoDev.lfo_v_min = LfoCase.cen_dev[jj].curve.min_val;
				tmpLfoDev.lfo_v_ampl = LfoCase.cen_dev[jj].curve.max_ampl;

				tmpLfoDev.lfo_is_cntr = LfoCase.cen_dev[jj].lfo_is_cntr;
				tmpLfoDev.is_disp = LfoCase.cen_dev[jj].lfo_is_cntr;
				tmpLfoDev.is_cntr = LfoCase.cen_dev[jj].lfo_is_cntr;
				tmpLfoDev.lfo_is_disp = MENU_LFO_MON_YES;
				// 				tmpLfoDev.lfo_clus = LfoCase.cen_dev[jj].lfo_clus;

				nFlag = 1;
				sprintf(m_LogString, "2写LFO厂站表第[%d]个设备索引["INT_REC_FMT"],是否振荡中心[%d]", ii, INT_REC_VAL(tmpVecLfoFacInfo[ii].fac_idx), tmpLfoDev.lfo_is_cntr);
				MessShow(m_LogString);
				break;
			}
		}

		if (nFlag == 0)
		{
			// 设备未振荡
			tmpLfoDev.lfo_is_disp = MENU_LFO_MON_NO;
			tmpLfoDev.is_disp = MENU_LFO_MON_NO;
		}
		PODB_TableModifyByKey((const char*)&tmpVecLfoFacInfo[ii].fac_idx, LFO_FC_TAB_WRITE_FLD, (const char*)&tmpLfoDev, sizeof(LFO_FC_TAB_WRITE), LFO_FAC_NO_TAB);
	}
#endif
	return 0;
}

int CLfoAnaObj::UpdateLfoGnTable(LFO_CASE_INFO& LfoCase, int save_flag)
{
	if (LfoCase.vec_dev.empty())
	{
		return 0;
	}
#ifdef _WAMAP_
	// 振荡设备填写振荡信息，非振荡设备填写空值
	vector<LFO_GN_TAB_CRE> tmpVecLfoGenInfo;
	GetLfoGen(tmpVecLfoGenInfo);
	if (tmpVecLfoGenInfo.empty())
	{
		return 0;
	}
	int ii, jj, nFlag;
	LFO_GN_TAB_WRITE tmpLfoDev;
	// 	sprintf(m_LogString, "[提示]LFO发电机表设备数[%d]振荡设备数[%d]",(int)tmpVecLfoGenInfo.size(),(int)LfoCase.vec_dev.size());
	// 	MessShow(m_LogString);
	for (ii = 0; ii < (int)tmpVecLfoGenInfo.size(); ii++)
	{
		// 		if (tmpVecLfoGenInfo[ii].lfo_is_disp2 == MENU_LFO_MENU_YES && save_flag != LFO_SAVE_FLAG_END)
		// 		{//上次振荡设备,本次振荡过程中暂不更新,待振荡结束时再更新
		// 			sprintf(m_LogString, "["INT_REC_FMT"]为上次振荡发电机,本次振荡过程中暂不更新",INT_REC_VAL(tmpVecLfoGenInfo[ii].gn_idx));
		// 			MessShow(m_LogString);
		// 			continue;
		// 		}
		nFlag = 0;
		memset((void*)&tmpLfoDev, 0, sizeof(LFO_GN_TAB_WRITE));
		for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
		{
			//sprintf(m_LogString, "[提示44]UpdateLfoGnTable设备[%s]类型[%d]",LfoCase.vec_dev[jj].dev.dev_name,LfoCase.vec_dev[jj].dev.dev_type);
			//MessShow(m_LogString);
	//   	sprintf(m_LogString, "[提示]LFO发电机表设备ID["INT_REC_FMT"]=["INT_REC_FMT"]",INT_REC_VAL(tmpVecLfoGenInfo[ii].gn_id),INT_REC_VAL(LfoCase.vec_dev[jj].dev.dev_id));
	//		MessShow(m_LogString);
			if (LfoCase.vec_dev[jj].dev.dev_type != MENU_LFO_DEV_GEN)
			{
				continue;
			}
			if (tmpVecLfoGenInfo[ii].gn_id == LfoCase.vec_dev[jj].dev.dev_id)
			{
				//tmpLfoDev.fact = LfoCase.vec_dev[jj].
				//tmpLfoDev.fact_sts
				tmpLfoDev.if_lfo = MENU_LFO_MENU_YES;
				tmpLfoDev.slfo_bgn_t = LfoCase.vec_dev[jj].start_time;
				tmpLfoDev.slfo_end_t = LfoCase.vec_dev[jj].end_time;

				LfoCase.vec_dev[jj].hold_time = tmpLfoDev.slfo_end_t - tmpLfoDev.slfo_bgn_t;
				sprintf(tmpLfoDev.lfo_time, "%s", PT_GetStringOfTimeInterval(LfoCase.vec_dev[jj].hold_time * 1000));
				//tmpLfoDev.slfo_bgn_val = LfoCase.vec_dev[jj]
				//tmpLfoDev.slfo_end_val = LfoCase.vec_dev[jj]
				tmpLfoDev.lfo_max_val = LfoCase.vec_dev[jj].curve.max_val;
				tmpLfoDev.lfo_max_val_t = LfoCase.vec_dev[jj].curve.max_val_t;
				tmpLfoDev.lfo_min_val = LfoCase.vec_dev[jj].curve.min_val;
				tmpLfoDev.lfo_min_val_t = LfoCase.vec_dev[jj].curve.min_val_t;
				tmpLfoDev.lfo_max_min_dif = LfoCase.vec_dev[jj].curve.max_ampl;

				// 				tmpLfoDev.lead_ampl = LfoCase.vec_dev[jj].mode.ampl;
				// 				tmpLfoDev.lead_freq = LfoCase.vec_dev[jj].mode.freq;
				// 				tmpLfoDev.lead_damp = LfoCase.vec_dev[jj].mode.damp;
				// 				tmpLfoDev.lead_phas = LfoCase.vec_dev[jj].mode.phas;
				// 
				// 				tmpLfoDev.lead_real = LfoCase.vec_dev[jj].mode.real;
				// 				tmpLfoDev.lead_imag = LfoCase.vec_dev[jj].mode.imag;
				// 				tmpLfoDev.lead_engy = LfoCase.vec_dev[jj].mode.engy;

				// 				tmpLfoDev.fact = LfoCase.vec_dev[jj].mode.fact;
				// 				tmpLfoDev.clus = LfoCase.vec_dev[jj].mode.group;
				// 				tmpLfoDev.lead_gen_pow = LfoCase.vec_dev[jj].mode.pow;
				// 				tmpLfoDev.is_lfo_source = LfoCase.vec_dev[jj].mode.is_source;
				tmpLfoDev.is_lfo_source2 = LfoCase.vec_dev[jj].mode.eve_source;

				if (save_flag != LFO_SAVE_FLAG_END)
				{//振荡未结束时,is_disp不变
					tmpLfoDev.lfo_is_disp2 = tmpVecLfoGenInfo[ii].lfo_is_disp2;
				}
				else
				{//振荡结束,is_disp改为是
					tmpLfoDev.lfo_is_disp2 = MENU_LFO_MENU_YES;
					if (m_LfoPara.data_source != LFO_DATA_SOURCE_REALTTIME)
					{
						tmpLfoDev.if_lfo = MENU_LFO_MENU_NO;
					}
				}

				sprintf(m_LogString, "1写LFO发电机表第[%d]个设备索引["INT_REC_FMT"],名称[%s],最大摆动[%f],最大值[%f],最小值[%f],是否振荡源(事件)[%d],是否振荡源(窗口)[%d]", ii, INT_REC_VAL(tmpVecLfoGenInfo[ii].gn_idx), LfoCase.vec_dev[jj].dev.dev_name, tmpLfoDev.lfo_max_min_dif, tmpLfoDev.lfo_max_val, tmpLfoDev.lfo_min_val, tmpLfoDev.is_lfo_source2, LfoCase.vec_dev[jj].mode.is_source);
				MessShow(m_LogString);
				nFlag = 1;
				break;
			}
		}

		if (nFlag == 0)
		{
			// 设备未振荡
			tmpLfoDev.if_lfo = MENU_LFO_MENU_NO;
			if (save_flag != LFO_SAVE_FLAG_END)
			{//振荡未结束时,is_disp不变
				tmpLfoDev.lfo_is_disp2 = tmpVecLfoGenInfo[ii].lfo_is_disp2;
			}
			else
			{//振荡结束,is_disp改为否
				tmpLfoDev.lfo_is_disp2 = MENU_LFO_MENU_NO;
			}
		}
		//  		sprintf(m_LogString, "2写LFO发电机表第[%d]个设备索引["INT_REC_FMT"],是否显示[%d]",ii,INT_REC_VAL(tmpVecLfoGenInfo[ii].gn_idx),tmpLfoDev.lfo_is_disp2);
		//  		MessShow(m_LogString);
		PODB_TableModifyByKey((const char*)&tmpVecLfoGenInfo[ii].gn_idx, LFO_GN_TAB_WRITE_FLD, (const char*)&tmpLfoDev, sizeof(LFO_GN_TAB_WRITE), LFO_GN_NO_TAB);
	}
#endif
	return 0;
}

int CLfoAnaObj::UpdateLfoLnTable(LFO_CASE_INFO& LfoCase, int save_flag)
{

	if (LfoCase.vec_dev.empty())
	{
		return 0;
	}
#ifdef _WAMAP_
	// 振荡设备填写振荡信息，非振荡设备填写空值
	vector<LFO_LN_TAB_CRE> tmpVecLfoLineInfo;
	GetLfoLine(tmpVecLfoLineInfo);

	if (tmpVecLfoLineInfo.empty())
	{
		MessShow("tmpVecLfoLineInfo.empty()");
		return 0;
	}

	int ii, jj, nFlag;
	LFO_LN_TAB_WRITE tmpLfoDev;
	for (ii = 0; ii < (int)tmpVecLfoLineInfo.size(); ii++)
	{
		// 		if (tmpVecLfoLineInfo[ii].lfo_is_disp2 == MENU_LFO_MENU_YES && save_flag != LFO_SAVE_FLAG_END)
		// 		{//上次振荡设备,本次振荡过程中暂不更新,待振荡结束时再更新
		// 			continue;
		// 		}
		nFlag = 0;
		memset((void*)&tmpLfoDev, 0, sizeof(LFO_LN_TAB_WRITE));
		for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
		{
			if (LfoCase.vec_dev[jj].dev.dev_type != MENU_LFO_DEV_LINE)
			{
				continue;
			}
			if (tmpVecLfoLineInfo[ii].acln_id == LfoCase.vec_dev[jj].dev.dev_id)
			{
				//tmpLfoDev.fact = LfoCase.vec_dev[jj].
				//tmpLfoDev.fact_sts
				tmpLfoDev.if_lfo = MENU_LFO_MENU_YES;
				tmpLfoDev.slfo_bgn_t = LfoCase.vec_dev[jj].start_time;
				tmpLfoDev.slfo_end_t = LfoCase.vec_dev[jj].end_time;
				LfoCase.vec_dev[jj].hold_time = tmpLfoDev.slfo_end_t - tmpLfoDev.slfo_bgn_t;
				sprintf(tmpLfoDev.lfo_time, "%s", PT_GetStringOfTimeInterval(LfoCase.vec_dev[jj].hold_time * 1000));
				//tmpLfoDev.slfo_bgn_val = LfoCase.vec_dev[jj]
				//tmpLfoDev.slfo_end_val = LfoCase.vec_dev[jj]
				tmpLfoDev.lfo_max_val = LfoCase.vec_dev[jj].curve.max_val;
				tmpLfoDev.lfo_max_val_t = LfoCase.vec_dev[jj].curve.max_val_t;
				tmpLfoDev.lfo_min_val = LfoCase.vec_dev[jj].curve.min_val;
				tmpLfoDev.lfo_min_val_t = LfoCase.vec_dev[jj].curve.min_val_t;
				tmpLfoDev.lfo_max_min_dif = LfoCase.vec_dev[jj].curve.max_ampl;

				// 				tmpLfoDev.lead_ampl = LfoCase.vec_dev[jj].mode.ampl;
				// 				tmpLfoDev.lead_freq = LfoCase.vec_dev[jj].mode.freq;
				// 				tmpLfoDev.lead_damp = LfoCase.vec_dev[jj].mode.damp;
				// 				tmpLfoDev.lead_phas = LfoCase.vec_dev[jj].mode.phas;
				// 				
				// 				tmpLfoDev.lead_real = LfoCase.vec_dev[jj].mode.real;
				// 				tmpLfoDev.lead_imag = LfoCase.vec_dev[jj].mode.imag;
				// 				tmpLfoDev.lead_engy = LfoCase.vec_dev[jj].mode.engy;
				// 				
				// 				tmpLfoDev.fact = LfoCase.vec_dev[jj].mode.fact;
				// 				tmpLfoDev.lead_line_pow = LfoCase.vec_dev[jj].mode.pow;
				// 				tmpLfoDev.istie_source = LfoCase.vec_dev[jj].mode.is_source;
				tmpLfoDev.istie_source2 = LfoCase.vec_dev[jj].mode.eve_source;
				if (save_flag != LFO_SAVE_FLAG_END)
				{//振荡未结束时,is_disp不变
					tmpLfoDev.lfo_is_disp2 = tmpVecLfoLineInfo[ii].lfo_is_disp2;
				}
				else
				{//振荡结束,is_disp改为是
					tmpLfoDev.lfo_is_disp2 = MENU_LFO_MENU_YES;
					if (m_LfoPara.data_source != LFO_DATA_SOURCE_REALTTIME)
					{
						tmpLfoDev.if_lfo = MENU_LFO_MENU_NO;
					}
				}

				// 				sprintf(tmpLfoDev.yc_name,"%s",LfoCase.vec_dev[jj].dev.dot_dev_name);
				// 				sprintf(tmpLfoDev.in_or_out,"%s",LfoCase.vec_dev[jj].mode.in_or_out);

				// 				sprintf(m_LogString, "写LFO交流线路表设备["INT_REC_FMT"]持续时间[%s]",INT_REC_VAL(tmpVecLfoLineInfo[ii].acln_id),tmpLfoDev.lfo_time);
				// 		 		MessShow(m_LogString);	
				nFlag = 1;
				break;
			}
		}
		if (nFlag == 0)
		{
			// 设备未振荡
			tmpLfoDev.if_lfo = MENU_LFO_MENU_NO;
			if (save_flag != LFO_SAVE_FLAG_END)
			{//振荡未结束时,is_disp不变
				tmpLfoDev.lfo_is_disp2 = tmpVecLfoLineInfo[ii].lfo_is_disp2;
			}
			else
			{//振荡结束,is_disp改为否
				tmpLfoDev.lfo_is_disp2 = MENU_LFO_MENU_NO;
			}
		}
		PODB_TableModifyByKey((const char*)&tmpVecLfoLineInfo[ii].acln_idx, LFO_LN_TAB_WRITE_FLD, (const char*)&tmpLfoDev, sizeof(LFO_LN_TAB_WRITE), LFO_ACLN_NO_TAB);

	}

#endif
	return 0;
}

int CLfoAnaObj::UpdateLfoTrTable(LFO_CASE_INFO& LfoCase, int save_flag)
{
	if (LfoCase.vec_dev.empty())
	{
		return 0;
	}
#ifdef _WAMAP_
	// 振荡设备填写振荡信息，非振荡设备填写空值
	vector<LFO_TR_TAB_CRE> tmpVecLfoTrInfo;
	GetLfoTr(tmpVecLfoTrInfo);

	if (tmpVecLfoTrInfo.empty())
	{
		return 0;
	}

	int ii, jj, nFlag;
	LFO_TR_TAB_WRITE tmpLfoDev;
	for (ii = 0; ii < (int)tmpVecLfoTrInfo.size(); ii++)
	{
		// 		if (tmpVecLfoTrInfo[ii].lfo_is_disp2 == MENU_LFO_MENU_YES && save_flag != LFO_SAVE_FLAG_END)
		// 		{//上次振荡设备,本次振荡过程中暂不更新,待振荡结束时再更新
		// 			continue;
		// 		}
		nFlag = 0;
		memset((void*)&tmpLfoDev, 0, sizeof(LFO_TR_TAB_WRITE));
		for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
		{
			if (LfoCase.vec_dev[jj].dev.dev_type != MENU_LFO_DEV_TR)
			{
				continue;
			}
			if (tmpVecLfoTrInfo[ii].tr_id == LfoCase.vec_dev[jj].dev.dev_id)
			{
				//tmpLfoDev.fact = LfoCase.vec_dev[jj].
				//tmpLfoDev.fact_sts
				tmpLfoDev.if_lfo = MENU_LFO_MENU_YES;
				tmpLfoDev.slfo_bgn_t = LfoCase.vec_dev[jj].start_time;
				tmpLfoDev.slfo_end_t = LfoCase.vec_dev[jj].end_time;
				LfoCase.vec_dev[jj].hold_time = tmpLfoDev.slfo_end_t - tmpLfoDev.slfo_bgn_t;
				sprintf(tmpLfoDev.lfo_time, "%s", PT_GetStringOfTimeInterval(LfoCase.vec_dev[jj].hold_time * 1000));
				//tmpLfoDev.slfo_bgn_val = LfoCase.vec_dev[jj]
				//tmpLfoDev.slfo_end_val = LfoCase.vec_dev[jj]
				tmpLfoDev.lfo_max_val = LfoCase.vec_dev[jj].curve.max_val;
				tmpLfoDev.lfo_max_val_t = LfoCase.vec_dev[jj].curve.max_val_t;
				tmpLfoDev.lfo_min_val = LfoCase.vec_dev[jj].curve.min_val;
				tmpLfoDev.lfo_min_val_t = LfoCase.vec_dev[jj].curve.min_val_t;
				tmpLfoDev.lfo_max_min_dif = LfoCase.vec_dev[jj].curve.max_ampl;

				// 				tmpLfoDev.lead_ampl = LfoCase.vec_dev[jj].mode.ampl;
				// 				tmpLfoDev.lead_freq = LfoCase.vec_dev[jj].mode.freq;
				// 				tmpLfoDev.lead_damp = LfoCase.vec_dev[jj].mode.damp;
				// 				tmpLfoDev.lead_phas = LfoCase.vec_dev[jj].mode.phas;
				// 				
				// 				tmpLfoDev.lead_real = LfoCase.vec_dev[jj].mode.real;
				// 				tmpLfoDev.lead_imag = LfoCase.vec_dev[jj].mode.imag;
				// 				tmpLfoDev.lead_engy = LfoCase.vec_dev[jj].mode.engy;
				// 				
				// 				tmpLfoDev.fact = LfoCase.vec_dev[jj].mode.fact;
				// 				tmpLfoDev.lead_tr_pow = LfoCase.vec_dev[jj].mode.pow;
				// 				tmpLfoDev.istie_source = LfoCase.vec_dev[jj].mode.is_source;
				tmpLfoDev.istie_source2 = LfoCase.vec_dev[jj].mode.eve_source;

				if (save_flag != LFO_SAVE_FLAG_END)
				{//振荡未结束时,is_disp不变
					tmpLfoDev.lfo_is_disp2 = tmpVecLfoTrInfo[ii].lfo_is_disp2;
				}
				else
				{//振荡结束,is_disp改为是
					tmpLfoDev.lfo_is_disp2 = MENU_LFO_MENU_YES;
					if (m_LfoPara.data_source != LFO_DATA_SOURCE_REALTTIME)
					{
						tmpLfoDev.if_lfo = MENU_LFO_MENU_NO;
					}
				}


				// 				sprintf(tmpLfoDev.yc_name,"%s",LfoCase.vec_dev[jj].dev.dot_dev_name);
				// 				sprintf(tmpLfoDev.in_or_out,"%s",LfoCase.vec_dev[jj].mode.in_or_out);

				nFlag = 1;
				break;
			}
		}
		if (nFlag == 0)
		{
			// 设备未振荡
			tmpLfoDev.if_lfo = MENU_LFO_MENU_NO;
			if (save_flag != LFO_SAVE_FLAG_END)
			{//振荡未结束时,is_disp不变
				tmpLfoDev.lfo_is_disp2 = tmpVecLfoTrInfo[ii].lfo_is_disp2;
			}
			else
			{//振荡结束,is_disp改为否
				tmpLfoDev.lfo_is_disp2 = MENU_LFO_MENU_NO;
			}
		}

		PODB_TableModifyByKey((const char*)&tmpVecLfoTrInfo[ii].tr_idx, LFO_TR_TAB_WRITE_FLD, (const char*)&tmpLfoDev, sizeof(LFO_TR_TAB_WRITE), LFO_TR_NO_TAB);

	}
#endif
	return 0;
}


int CLfoAnaObj::UpdateLfoIntTable(LFO_CASE_INFO& LfoCase, int save_flag)
{
	if (LfoCase.vec_dev.empty())
	{
		return 0;
	}
#ifdef _WAMAP_
	// 振荡设备填写振荡信息，非振荡设备填写空值
	vector<LFO_INT_TAB_CRE> tmpVecLfoIntInfo;
	GetLfoIntface(tmpVecLfoIntInfo);

	if (tmpVecLfoIntInfo.empty())
	{
		return 0;
	}

	int ii, jj, nFlag;
	LFO_INF_TAB_WRITE tmpLfoDev;
	for (ii = 0; ii < (int)tmpVecLfoIntInfo.size(); ii++)
	{
		// 		if (tmpVecLfoIntInfo[ii].lfo_is_disp2 == MENU_LFO_MENU_YES && save_flag != LFO_SAVE_FLAG_END)
		// 		{//上次振荡设备,本次振荡过程中暂不更新,待振荡结束时再更新
		// 			continue;
		// 		}
		nFlag = 0;
		memset((void*)&tmpLfoDev, 0, sizeof(LFO_INF_TAB_WRITE));
		for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
		{
			if (LfoCase.vec_dev[jj].dev.dev_type != MENU_LFO_DEV_ITFC)
			{
				continue;
			}
			if (tmpVecLfoIntInfo[ii].inf_idx == LfoCase.vec_dev[jj].dev.dev_id)
			{
				//tmpLfoDev.fact = LfoCase.vec_dev[jj].
				//tmpLfoDev.fact_sts
				tmpLfoDev.if_lfo2 = MENU_LFO_MENU_YES;
				tmpLfoDev.lfo_bgn_t2 = LfoCase.vec_dev[jj].start_time;
				tmpLfoDev.lfo_end_t2 = LfoCase.vec_dev[jj].end_time;
				LfoCase.vec_dev[jj].hold_time = tmpLfoDev.lfo_end_t2 - tmpLfoDev.lfo_bgn_t2;
				sprintf(tmpLfoDev.lfo_time2, "%s", PT_GetStringOfTimeInterval(LfoCase.vec_dev[jj].hold_time * 1000));
				sprintf(tmpLfoDev.intline_des2, "%s", LfoCase.vec_dev[jj].intline_des);
				tmpLfoDev.lfo_max_val2 = LfoCase.vec_dev[jj].curve.max_val;
				tmpLfoDev.lfo_max_val_t2 = LfoCase.vec_dev[jj].curve.max_val_t;
				tmpLfoDev.lfo_min_val2 = LfoCase.vec_dev[jj].curve.min_val;
				tmpLfoDev.lfo_min_val_t2 = LfoCase.vec_dev[jj].curve.min_val_t;

				if (save_flag != LFO_SAVE_FLAG_END)
				{//振荡未结束时,is_disp不变
					tmpLfoDev.lfo_is_disp2 = tmpVecLfoIntInfo[ii].lfo_is_disp2;
				}
				else
				{//振荡结束,is_disp改为是
					tmpLfoDev.lfo_is_disp2 = MENU_LFO_MENU_YES;
				}


				//tmpLfoDev.slfo_bgn_val = LfoCase.vec_dev[jj]
				//tmpLfoDev.slfo_end_val = LfoCase.vec_dev[jj]

// 				tmpLfoDev.lead_ampl2 = LfoCase.vec_dev[jj].mode.ampl;
// 				tmpLfoDev.lead_freq2 = LfoCase.vec_dev[jj].mode.freq;
// 				tmpLfoDev.lead_damp2 = LfoCase.vec_dev[jj].mode.damp;
// 				tmpLfoDev.lead_phas2 = LfoCase.vec_dev[jj].mode.phas;
// 				
// 				tmpLfoDev.lead_real2 = LfoCase.vec_dev[jj].mode.real;
// 				tmpLfoDev.lead_imag2 = LfoCase.vec_dev[jj].mode.imag;
// 				tmpLfoDev.lead_engy2 = LfoCase.vec_dev[jj].mode.engy;

// 				sprintf(m_LogString, "断面[%s]["INT_REC_FMT"]振幅[%f]持续时间[%s]",tmpVecLfoIntInfo[ii].inf_name,tmpVecLfoIntInfo[ii].inf_idx,tmpLfoDev.lead_ampl2,tmpLfoDev.lfo_time2);
// 				MessShow(m_LogString);	
// 				
				nFlag = 1;
				break;
			}
		}
		if (nFlag == 0)
		{
			// 设备未振荡
			tmpLfoDev.if_lfo2 = MENU_LFO_MENU_NO;
			if (save_flag != LFO_SAVE_FLAG_END)
			{//振荡未结束时,is_disp不变
				tmpLfoDev.lfo_is_disp2 = tmpVecLfoIntInfo[ii].lfo_is_disp2;
			}
			else
			{//振荡结束,is_disp改为否
				tmpLfoDev.lfo_is_disp2 = MENU_LFO_MENU_NO;
			}
		}

		PODB_TableModifyByKey((const char*)&tmpVecLfoIntInfo[ii].inf_idx, LFO_INF_TAB_WRITE_FLD, (const char*)&tmpLfoDev, sizeof(LFO_INF_TAB_WRITE), LFO_INTFACE_NO_TAB);

	}
#endif
	return 0;

}


int CLfoAnaObj::UpdateLfoCaseTable(LFO_CASE_INFO& LfoCase)
{
#ifdef _WAMAP_

#endif
	return 0;
}

int CLfoAnaObj::UpdateLfoWindowsTable(LFO_CASE_INFO& LfoCase)
{
	if (LfoCase.vec_windows.empty())
	{
		return 0;
	}
#ifdef _WAMAP_
	int ii;
	int nWindowsInfo = (int)LfoCase.vec_windows.size();
	LFO_POW_CASE_WDS_WAMS* pWindowsInfo = NULL;
	pWindowsInfo = new LFO_POW_CASE_WDS_WAMS[nWindowsInfo];
	memset(pWindowsInfo, 0, nWindowsInfo * sizeof(LFO_POW_CASE_WDS_WAMS));
	for (ii = 0; ii < (int)LfoCase.vec_windows.size(); ii++)
	{
		pWindowsInfo[ii].wds_id = PODB_GetDevIdByKeyID(LFO_POW_CASE_WDS_NO, ii + 1);
		pWindowsInfo[ii].win_id = LfoCase.vec_windows[ii].window_id;
		pWindowsInfo[ii].eve_id = LfoCase.event_id;
		pWindowsInfo[ii].analy_bgn = LfoCase.vec_windows[ii].start_time;
		pWindowsInfo[ii].analy_end = LfoCase.vec_windows[ii].end_time;
		pWindowsInfo[ii].is_show = MENU_LFO_MENU_YES;
		pWindowsInfo[ii].lfodev_id = LfoCase.vec_windows[ii].lead_dev.dev.dev_id;
		pWindowsInfo[ii].fac_id = LfoCase.vec_windows[ii].lead_dev.dev.fac_id;
		pWindowsInfo[ii].dev_type = LfoCase.vec_windows[ii].lead_dev.dev.dev_type;
		pWindowsInfo[ii].freq = LfoCase.vec_windows[ii].lead_dev.mode.freq;
		pWindowsInfo[ii].ampl = LfoCase.vec_windows[ii].lead_dev.mode.ampl;
		pWindowsInfo[ii].damp = LfoCase.vec_windows[ii].lead_dev.mode.damp;
		sprintf(pWindowsInfo[ii].center_desc, "%s", LfoCase.vec_windows[ii].center_desc);

		//pWindowsInfo[ii].aver_val = LfoCase.vec_windows[ii]
		//pWindowsInfo[ii].max_ampl_rat = LfoCase.vec_windows[ii].
	}
	// 清空原表
	PODB_TableClear(LFO_POW_CASE_WDS_NO);
	int ret_code = PODB_TableWrite((const char*)&pWindowsInfo[0].wds_id, nWindowsInfo * sizeof(LFO_POW_CASE_WDS_WAMS), LFO_POW_CASE_WDS_NO);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "写LPO功率波动窗口表[%d]失败", LFO_POW_CASE_WDS_NO);
		MessShow(m_LogString);
	}

	SAFE_DELETE_ARRAY(pWindowsInfo);
#endif
	return 0;
}

int CLfoAnaObj::UpdateLfoModeShape(LFO_CASE_INFO& LfoCase)
{
	return 0;
}
//************************************
// Method:    UpdateLfoDevTable
// FullName:  CLfoAnaObj::UpdateLfoDevTable
// Access:    public 
// Returns:   int
// Qualifier:
// 输入：系统状态结构vector<LFO_SYS_INFO>，振荡设备统计（按振荡设备统计本次振荡设备开始时间，结束时间，持续时间，波形统计）, 振荡设备模式（按振荡设备统计每个分析窗口的主导模式）vector< LFO_DEV_INFO >
//************************************
// int CLfoAnaObj::UpdateLfoDevTable(LFO_SYS_INFO MonSys)
// {
//     return 0;
// }

//************************************
// Method:    UpdateModeShapeTable
// FullName:  CLfoAnaObj::UpdateModeShapeTable
// Access:    public 
// Returns:   int
// Qualifier:
// 输入：系统状态结构vector<LFO_SYS_INFO>，每个窗口主导模式模态结果vector<LFO_MODE_SHAPE>
//************************************
// int CLfoAnaObj::UpdateModeShapeTable(LFO_SYS_INFO MonSys)
// {
//     return 0;
// }

//////////////////////////////////////////////////////////////////////////
//************************************
// Method:    SaveLfoCaseFile
// FullName:  CLfoAnaObj::SaveLfoCaseFile
// Access:    public 
// Returns:   int
// Qualifier:
// 振荡结束后保存本次振荡文件：
// 输入：振荡事件结构(LFO_CASE_INFO)，振荡统计（振荡开始时间，振荡结束时间，振荡持续时间，平均频率，最小阻尼比，触发告警设备）
// 振荡事件结构(LFO_CASE_INFO)，振荡设备统计（按振荡设备统计本次振荡设备开始时间，结束时间，持续时间，波形统计）, 振荡设备模式（按振荡设备统计每个分析窗口的主导模式）vector< LFO_DEV_INFO >
// 振荡事件结构(LFO_CASE_INFO)，每个窗口主导模式模态结果vector<LFO_MODE_SHAPE>
// 振荡结束后，清空振荡事件结构体。
//************************************
int CLfoAnaObj::SaveLfoCaseFile(LFO_CASE_INFO& LfoCase)
{
	if (m_LfoPara.is_save_casefile == 0)
	{
		return 0;
	}

	char tmpfilename[MAXBUF];
	sprintf(tmpfilename, "%s%sres%s%s告警.txt",
		m_maindir.c_str(), m_backslash, m_backslash, PT_GetChineseTimeStringBySeconds(LfoCase.start_time));
	sprintf(m_LogString, "保存结果文件[%s]", tmpfilename);
	MessShow(m_LogString, LTINFO);
	fstream fs;
	fs.open(tmpfilename, std::ios::out);
	if (fs.fail())
	{
		fs.close();
		return -1;
	}

	fs << "功率振荡在线监视" << endl;
	fs << "发生时间:" << PT_GetTimeStringBySeconds(LfoCase.start_time) << "(" << LfoCase.start_time << ")" << "  ";
	fs << "消失时间:" << PT_GetTimeStringBySeconds(LfoCase.end_time) << "(" << LfoCase.end_time << ")" << "  ";
	fs << "持续时间:" << PT_GetStringOfTimeInterval(LfoCase.hold_time * 1000) << endl;
	fs << "首先告警设备:" << LfoCase.warn_dev.dev.dev_name << "  " << "所属厂站:" << LfoCase.warn_dev.dev.fac_name << "  " << endl;
	fs << "触发告警频率:" << LfoCase.warn_dev.mode.freq << "Hz" << endl;
	fs << "振荡平均频率:" << LfoCase.lead_mode.freq << "Hz" << endl;
	fs << "振荡源机组名称:" << LfoCase.lfos_gn_name << endl;
	fs << "振荡源描述信息:" << LfoCase.lfos_des << endl;
	fs << "振荡中心描述信息:" << LfoCase.center_desc << endl;
	fs << "振荡类型:" << LfoCase.type_desc << endl;

	fs << "[振荡设备]" << endl;
	fs << "振荡设备个数:" << (int)LfoCase.vec_dev.size() << endl;
	int ii, jj, kk;
	char tmptime1[20], tmptime2[20];
	for (ii = 0; ii < (int)LfoCase.vec_dev.size(); ii++)
	{
		if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
		{
			continue;
		}
		LfoCase.vec_dev[ii].hold_time = LfoCase.vec_dev[ii].end_time - LfoCase.vec_dev[ii].start_time;
		fs << "设备名称=" << LfoCase.vec_dev[ii].dev.dev_name << "  " << "所属厂站=" << LfoCase.vec_dev[ii].dev.fac_name << "  ";
		fs << "振荡发生时刻=" << PT_GetTimeStringBySeconds(LfoCase.vec_dev[ii].start_time) << "(" << LfoCase.vec_dev[ii].start_time << ")" << "  ";
		fs << "消失时刻=" << PT_GetTimeStringBySeconds(LfoCase.vec_dev[ii].end_time) << "(" << LfoCase.vec_dev[ii].end_time << ")" << "  ";
		fs << "持续时间=" << PT_GetStringOfTimeInterval(LfoCase.vec_dev[ii].hold_time * 1000) << "  ";
		fs << "最大值=" << LfoCase.vec_dev[ii].curve.max_val << "  ";
		fs << "最大值时刻=" << PT_GetTimeStringBySeconds(LfoCase.vec_dev[ii].curve.max_val_t) << "(" << LfoCase.vec_dev[ii].curve.max_val_t << ")" << "  ";
		fs << "最小值=" << LfoCase.vec_dev[ii].curve.max_val << "  ";
		fs << "最小值时刻=" << PT_GetTimeStringBySeconds(LfoCase.vec_dev[ii].curve.min_val_t) << "(" << LfoCase.vec_dev[ii].curve.min_val_t << ")" << "  ";
		fs << "最大波动=" << LfoCase.vec_dev[ii].curve.max_ampl << endl;
		fs << "序号         分析时段                       幅值(MW)   频率(Hz)   阻尼比(%)   相角(deg)  耗散功率(MW) 参与因子  是否振荡源  " << endl;
		for (jj = 0; jj < (int)LfoCase.vec_dev[ii].vec_lead_mode.size(); jj++)
		{
			sprintf(tmptime1, "%s", PT_GetHMSTimeStringBySeconds(LfoCase.vec_dev[ii].vec_lead_mode[jj].start_time));
			sprintf(tmptime2, "%s", PT_GetHMSTimeStringBySeconds(LfoCase.vec_dev[ii].vec_lead_mode[jj].end_time));
			fs << jj + 1 << "  ";
			fs << tmptime1 << "~";
			fs << tmptime2 << "(";
			fs << LfoCase.vec_dev[ii].vec_lead_mode[jj].start_time << "~";
			fs << LfoCase.vec_dev[ii].vec_lead_mode[jj].end_time << ") ";
			fs << LfoCase.vec_dev[ii].vec_lead_mode[jj].ampl << "  ";
			fs << LfoCase.vec_dev[ii].vec_lead_mode[jj].freq << "  ";
			fs << LfoCase.vec_dev[ii].vec_lead_mode[jj].damp << "  ";
			fs << LfoCase.vec_dev[ii].vec_lead_mode[jj].phas << "  ";
			fs << LfoCase.vec_dev[ii].vec_lead_mode[jj].pow << "  ";
			fs << LfoCase.vec_dev[ii].vec_lead_mode[jj].fact << "  ";
			if (LfoCase.vec_dev[ii].vec_lead_mode[jj].is_source == 1)
			{
				fs << "是" << endl;
			}
			else
			{
				fs << "否" << endl;
			}
		}
	}

	fs << "[系统模式信息]" << endl;
	for (ii = 0; ii < (int)LfoCase.vec_windows.size(); ii++)
	{
		fs << LfoCase.vec_windows[ii].start_time << "~" << LfoCase.vec_windows[ii].end_time << "主导模式：" << endl;
		for (jj = 0; jj < (int)LfoCase.vec_windows[ii].vec_modeshape.size(); jj++)
		{
			fs << "序号=" << jj + 1 << "  ";
			fs << "频率=" << LfoCase.vec_windows[ii].vec_modeshape[jj].mode.freq << "  ";
			fs << "阻尼比=" << LfoCase.vec_windows[ii].vec_modeshape[jj].mode.damp << "  ";
			fs << "幅值=" << LfoCase.vec_windows[ii].vec_modeshape[jj].mode.ampl << "  ";
			fs << "初相=" << LfoCase.vec_windows[ii].vec_modeshape[jj].mode.phas << "  ";
			fs << "振荡中心分析描述=" << LfoCase.vec_windows[ii].center_desc << "  ";
			fs << "振荡机组数=" << LfoCase.vec_windows[ii].lfo_gn_num << "  ";
			fs << "振荡线路数=" << LfoCase.vec_windows[ii].lfo_ln_num << "  ";
			fs << "振荡变压器数=" << LfoCase.vec_windows[ii].lfo_tr_num << "  ";
			fs << "振荡断面数=" << LfoCase.vec_windows[ii].lfo_inf_num << "  ";
			fs << "振荡设备数=" << (int)LfoCase.vec_windows[ii].vec_modeshape[jj].vec_dev.size() << endl;
			fs << "  参与设备    幅值    初相    频率    阻尼比" << endl;
			for (kk = 0; kk < (int)LfoCase.vec_windows[ii].vec_modeshape[jj].vec_dev.size(); kk++)
			{
				fs << kk + 1 << "  ";
				fs << LfoCase.vec_windows[ii].vec_modeshape[jj].vec_dev[kk].dev.dev_name << "  ";
				fs << LfoCase.vec_windows[ii].vec_modeshape[jj].vec_dev[kk].mode.ampl << "  ";
				fs << LfoCase.vec_windows[ii].vec_modeshape[jj].vec_dev[kk].mode.phas << "  ";
				fs << LfoCase.vec_windows[ii].vec_modeshape[jj].vec_dev[kk].mode.freq << "  ";
				fs << LfoCase.vec_windows[ii].vec_modeshape[jj].vec_dev[kk].mode.damp << endl;
			}
		}
	}

	fs << "[计算参数]" << endl;
	fs << "频率下限=" << m_LfoPara.freq_min << endl;
	fs << "频率上限=" << m_LfoPara.freq_max << endl;
	fs << "低频振荡频率下限=" << m_LfoPara.lfo_freq_min << endl;
	fs << "低频振荡频率上限=" << m_LfoPara.lfo_freq_max << endl;
	fs << "观察时间窗口=" << m_LfoPara.time_leng << endl;
	fs << "观察时间窗口系数=" << m_LfoPara.time_leng_coe << endl;
	fs << "采样频率=" << m_LfoPara.sample_freq << endl;
	fs << "采样频率系数=" << m_LfoPara.sample_freq_coe << endl;
	fs << "交叠窗口=" << m_LfoPara.int_time << endl;
	fs << "交叠窗口系数=" << m_LfoPara.int_time_coe << endl;
	fs << "机组振幅限值=" << m_LfoPara.gn_wn_ampl << endl;
	fs << "线路振幅限值=" << m_LfoPara.ln_wn_ampl << endl;
	fs << "变压器振幅限值=" << m_LfoPara.tr_wn_ampl << endl;
	fs << "功率振荡阻尼比限值=" << m_LfoPara.lack_damp << endl;
	fs << "低频振荡阻尼比限值=" << m_LfoPara.lfo_damp << endl;
	fs << "最小阻尼比限值=" << m_LfoPara.damp_min << endl;
	fs << "预判振幅百分比=" << m_LfoPara.fjud_wnap_per << endl;
	fs << "告警持续时间限值(s)=" << m_LfoPara.warn_time_max << endl;
	fs << "安全持续持续时间限值(s)=" << m_LfoPara.safe_time_max << endl;
	fs << "PMU时刻延迟时间=" << m_LfoPara.delay_time << endl;
	fs << "同模式频率差=" << m_LfoPara.freq_diff << endl;
	fs << "同模式角度差=" << m_LfoPara.phas_diff << endl;
	fs << "缓存窗口最大个数=" << m_LfoPara.cache_windows_num << endl;
	//fs<<"保存模式幅值限值="<<m_LfoPara.stor_ampl<<endl;
	fs << "是否监视低频振荡=" << m_LfoPara.is_mon_lfo << endl;
	fs << "时间窗口最小值=" << m_LfoPara.time_leng_min << endl;
	fs << "采样频率最小值=" << m_LfoPara.sample_freq_min << endl;
	fs << "采样频率最大值=" << m_LfoPara.sample_freq_max << endl;
	fs << "交叠窗口最小值=" << m_LfoPara.int_time_min << endl;
	fs << "交叠窗口最大值=" << m_LfoPara.int_time_max << endl;
	fs << "模式保存幅值百分比=" << m_LfoPara.mode_save_amplper << endl;
	fs << "是否发送告警=" << m_LfoPara.is_warn << endl;
	fs << "是否触发tmd=" << m_LfoPara.is_tmd << endl;
	fs << "是否向EMS转发=" << m_LfoPara.is_triems << endl;
	fs << "告警发送应用号=" << m_LfoPara.warn_app_no << endl;
	fs << "是否调用曲线控件=" << m_LfoPara.is_lfo_curve << endl;
	fs << "devinfo_source=" << m_LfoPara.devinfo_source << endl;
	fs << "data_source=" << m_LfoPara.data_source << endl;
	fs << "添加设备厂站名称=" << m_LfoPara.is_add_facname << endl;
	fs << "生成案例文件=" << m_LfoPara.is_save_casefile << endl;
	fs << "生成设备信息文件=" << m_LfoPara.is_save_devinfo << endl;
	fs << "循环读取数据=" << m_LfoPara.is_cycle << endl;
	fs << "queue_type=" << m_LfoPara.prony_para.queue_type << endl;
	fs << "delDC=" << m_LfoPara.prony_para.delDC << endl;
	fs << "freq_min=" << m_LfoPara.prony_para.freq_min << endl;
	fs << "freq_max=" << m_LfoPara.prony_para.freq_max << endl;
	fs << "minAbsAmpl=" << m_LfoPara.prony_para.minAbsAmpl << endl;
	fs << "stor_lim_ampl=" << m_LfoPara.prony_para.stor_lim_ampl << endl;
	fs << "stor_lim_damp=" << m_LfoPara.prony_para.stor_lim_damp << endl;
	fs << "snr_min=" << m_LfoPara.prony_para.snr_min << endl;
	fs << "show_log=" << m_LfoPara.prony_para.show_log << endl;
	fs << "eng_time=" << m_LfoPara.prony_para.eng_time << endl;
	fs << "max_iter=" << m_LfoPara.prony_para.max_iter << endl;
	fs << "is_filter=" << m_LfoPara.prony_para.is_filter << endl;
	fs << "Voltunit_is_kV=" << m_LfoPara.Voltunit_is_kV << endl;
	fs << "Ampl_aver=" << m_LfoPara.para_Ampl_aver << endl;
	fs.close();

	return 0;
}
int CLfoAnaObj::SaveCaseCurve(LFO_CASE_INFO& LfoCase)
{
#ifndef _LFO_HIS_LIB_
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
	// 在res目录下面生成带时标的标志文件,然后拷贝到统一目录下
	LFO_CASE_FILE_STRUCT tmpCase;
	ClearCaseFile(tmpCase);
	// 	memset((void *)&tmpCase,0,sizeof(LFO_CASE_FILE_STRUCT));	
	if (!tmpCase.vecdev.empty())
	{
		tmpCase.vecdev.clear();
	}
	sprintf(tmpCase.version, "%s", "v1.0.0");
	sprintf(tmpCase.time, "%s", PT_GetTimeStringBySeconds(PT_GetNowSeconds()));
	sprintf(tmpCase.system, "%s", "电网功率振荡在线监视");
	tmpCase.eve_id = LfoCase.event_id;
	tmpCase.start_time = LfoCase.start_time;
	tmpCase.end_time = LfoCase.end_time;
	tmpCase.hold_time = LfoCase.hold_time;
	tmpCase.Data_Save_start_time = tmpCase.start_time - m_LfoPara.rec_num * (m_LfoPara.time_leng - m_LfoPara.int_time);
	tmpCase.Data_Save_end_time = tmpCase.end_time + m_LfoPara.rec_num * m_LfoPara.time_leng;
	tmpCase.lfo_modle = LfoCase.lfo_modle;
	tmpCase.lfo_type = LfoCase.lfo_type;
	sprintf(tmpCase.type_desc, "%s", LfoCase.type_desc);

	if (abs(tmpCase.end_time - tmpCase.start_time) > 1800)
	{
		return 0;
	}
	if ((tmpCase.start_time - tmpCase.Data_Save_start_time) < 0.0001)
	{
		tmpCase.Data_Save_start_time = tmpCase.start_time;
	}
	if ((tmpCase.end_time - tmpCase.Data_Save_end_time) > 0.0001)
	{
		tmpCase.Data_Save_end_time = tmpCase.end_time;
	}

	tmpCase.warn_dev_id = LfoCase.warn_dev.dev.dev_id;
#ifdef _GUANGDONG
	sprintf(tmpCase.case_des, "%s %s%s发生振荡,频率:%.3fHz", PT_GetTimeStringBySeconds(LfoCase.start_time), LfoCase.warn_dev.dev.fac_name, LfoCase.warn_dev.dev.dev_name, LfoCase.lead_mode.freq);
	sprintf(tmpCase.warn_dev_name, "%s%s", LfoCase.warn_dev.dev.fac_name, LfoCase.warn_dev.dev.dev_name);
#else
	sprintf(tmpCase.case_des, "%s %s发生振荡,频率:%.3fHz", PT_GetTimeStringBySeconds(LfoCase.start_time), LfoCase.warn_dev.dev.dev_name, LfoCase.lead_mode.freq);
	sprintf(tmpCase.warn_dev_name, "%s", LfoCase.warn_dev.dev.dev_name);
#endif
	tmpCase.warn_freq = LfoCase.lead_mode.freq;
	tmpCase.max_ampl = LfoCase.lead_mode.ampl;

	int ii, jj, table_no;
	LFO_CASE_FILE_DEV tmpdev;
	vector<FES_YC_TAB_CRE> tmpVecYc;
#ifdef _WAMAP_
	PODB_GetFesYcInfo(tmpVecYc);
#endif
	for (ii = 0; ii < (int)tmpVecYc.size(); ii++)
	{
		if (tmpVecYc[ii].file_no < 0 || tmpVecYc[ii].col_no < 0)
		{
			continue;
		}
		memset((void*)&tmpdev, 0, sizeof(LFO_CASE_FILE_DEV));
		tmpdev.fac_id = tmpVecYc[ii].fac_id;
		tmpdev.dev_id = PODB_GetRecordId(tmpVecYc[ii].yc_id);
		tmpdev.column_id = PODB_GetColumnId(tmpVecYc[ii].yc_id);
		tmpdev.yc_id = tmpVecYc[ii].yc_id;
		tmpdev.file_no = tmpVecYc[ii].file_no;
		tmpdev.col_no = tmpVecYc[ii].col_no;
		table_no = PODB_GetTabelNoByDevId(tmpdev.dev_id);
#ifdef _NUSP
		if (table_no == BS_DEVICE_NO_TAB || table_no == SUBSTATION_INFO_NO_TAB || table_no == PLANT_INFO_NO_TAB)
#else
		if (table_no == BS_DEVICE_NO_TAB || table_no == FAC_INFO_NO_TAB)
#endif // _NUSP		
		{
			tmpdev.dev_id1 = tmpdev.dev_id;
			tmpdev.is_warn = 0;
			for (jj = 0; jj < (int)LfoCase.cen_dev.size(); jj++)
			{
				if (tmpdev.dev_id == LfoCase.cen_dev[jj].dev.dev_id || tmpdev.dev_id == LfoCase.cen_dev[jj].dev_info.dot1.dot_id || tmpdev.dev_id == LfoCase.cen_dev[jj].dev_info.dot2.dot_id)
				{
					tmpdev.dev_id1 = LfoCase.cen_dev[jj].dev.dev_id;
					tmpdev.is_warn = 1;
					break;
				}
			}
		}
		else if (table_no == GN_DEVICE_NO_TAB)
		{
			tmpdev.dev_id1 = tmpdev.dev_id;
			tmpdev.is_warn = 0;
			if (VALID_INT_REC(tmpdev.dev_id1))		
			{
				for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
				{
					if (tmpdev.dev_id1 == LfoCase.vec_dev[jj].dev.dev_id)
					{
						tmpdev.is_warn = 1;
						break;
					}
				}
			}
		}
		else if (table_no == ACLN_DOT_NO_TAB || table_no == ACLN_DEVICE_NO_TAB)
		{
			// 根据线路端点查找线路ID
			tmpdev.dev_id1 = tmpdev.dev_id;
			tmpdev.is_warn = 0;
			for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
			{
				if (tmpdev.dev_id == LfoCase.vec_dev[jj].dev.dev_id || tmpdev.dev_id == LfoCase.vec_dev[jj].dev.dot_id)
				{
					tmpdev.dev_id1 = LfoCase.vec_dev[jj].dev.dev_id;
					tmpdev.is_warn = 1;
					break;
				}
			}
		}
		else if (table_no == TR_DEVICE_NO_TAB || table_no == TRWD_DEVICE_NO_TAB)
		{
			// 根据变压器绕组查找变压器ID
			tmpdev.dev_id1 = tmpdev.dev_id;
			tmpdev.is_warn = 0;
			for (jj = 0; jj < (int)LfoCase.vec_dev.size(); jj++)
			{
				if (tmpdev.dev_id == LfoCase.vec_dev[jj].dev.dev_id || tmpdev.dev_id == LfoCase.vec_dev[jj].dev.dot_id)
				{
					tmpdev.dev_id1 = LfoCase.vec_dev[jj].dev.dev_id;
					tmpdev.is_warn = 1;
					break;
				}
			}
		}
		else
		{
			//sprintf(m_LogString,"根据表号判断设备类型错误[%d]",table_no);
			//MessShow(m_LogString,LTERROR);
		}

		tmpCase.vecdev.push_back(tmpdev);
	}

	fstream fs;
	char filename[MAXBUF];
	char filenamebas[MAXBUF];
	char Examplefbas[MAXBUF];
	sprintf(filename, "casefile_2_%s.txt", PT_GetNoSpaceMarkTimeStringBySeconds(PT_GetNowSeconds()));
#ifdef _WINDOWS32
	sprintf(filenamebas, "%s\\res\\%s", m_maindir.c_str(), filename);
	sprintf(Examplefbas, "%s\\data", m_maindir.c_str());
#else
	sprintf(filenamebas, "%s/res/%s", m_maindir.c_str(), filename);
	sprintf(Examplefbas, "%s/data", m_maindir.c_str());
#endif
	sprintf(m_LogString, "数据源[%d]", m_LfoPara.data_source);
	MessShow(m_LogString, LTERROR);

	fs.open(filenamebas, ios::out);
	if (fs.fail())
	{
		sprintf(m_LogString, "案例标志文件生成失败[%s]", filenamebas);
		MessShow(m_LogString, LTERROR);
		fs.close();
		return -1;
	}

	fs << "[START]" << endl;
	fs << "[版本信息]" << endl;
	fs << "Version = " << tmpCase.version << ";" << endl;
	fs << "Time = " << tmpCase.time << ";" << endl;
	fs << "System = " << tmpCase.system << ";" << endl;
	fs << "[/版本信息]" << endl;

	fs << "[概要信息]" << endl;
	fs << "案例类型 = 3" << ";" << endl;

	if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
	{
	}
	else
	{
		fs << "演示案例目录 = " << Examplefbas << ";" << endl;
	}

	fs << "事件ID = " << tmpCase.eve_id << ";" << endl;
	fs << "起始时间 = " << tmpCase.start_time << ";" << endl;
	fs << "截止时间 = " << tmpCase.end_time << ";" << endl;
	fs << "持续时间 = " << tmpCase.hold_time << ";" << endl;
	fs << "数据保存起始时间 = " << tmpCase.Data_Save_start_time << ";" << endl;
	fs << "数据保存结束时间 = " << tmpCase.Data_Save_end_time << ";" << endl;
	fs << "事件描述 = " << tmpCase.case_des << ";" << endl;
	fs << "触发告警设备ID = " << INT_REC_VAL(tmpCase.warn_dev_id) << ";" << endl;
	fs << "触发告警设备名 = " << tmpCase.warn_dev_name << ";" << endl;
	fs << "触发告警频率 = " << tmpCase.warn_freq << ";" << endl;
	fs << "振荡最大幅值 = " << tmpCase.max_ampl << ";" << endl;
	fs << "振荡模式 = " << tmpCase.lfo_modle << ";" << endl;
	fs << "振荡类型 = " << tmpCase.lfo_type << ";" << endl;
	fs << "振荡类型描述 = " << tmpCase.type_desc << ";" << endl;
	fs << "[/概要信息]" << endl;

	fs << "[参数信息]" << endl;
	fs << "freq_min = " << m_LfoPara.freq_min << ";" << endl; /* 频率下限 */
	fs << "freq_max = " << m_LfoPara.freq_max << ";" << endl; /* 频率上限  */
	fs << "lowfreq_min = " << m_LfoPara.lfo_freq_min << ";" << endl; /* 频率下限 */
	fs << "lowfreq_max = " << m_LfoPara.lfo_freq_max << ";" << endl; /* 频率上限  */
	fs << "stor_damp = " << m_LfoPara.stor_damp << ";" << endl; /* 保存模式阻尼限值 */
	fs << "lack_damp = " << m_LfoPara.lack_damp << ";" << endl; /* 阻尼不足限值  */
	fs << "flfo_damp = " << m_LfoPara.flfo_damp << ";" << endl; /* 预警阻尼限值  */
	fs << "lfo_damp = " << m_LfoPara.lfo_damp << ";" << endl; /* 告警阻尼限值 */
	fs << "gn_wn_ampl = " << m_LfoPara.gn_swn_ampl2 << ";" << endl; /* 机组振幅限值1  */
	fs << "gn_wn_ampl2 = " << m_LfoPara.gn_swn_ampl2 << ";" << endl; /* 机组振幅限值2 */
	fs << "gn_wn_ampl3 = " << m_LfoPara.gn_swn_ampl2 << ";" << endl; /* 机组振幅限值3 */
	fs << "gn_wn_ampl1 = " << m_LfoPara.gn_ewn_ampl2 << ";" << endl; /* 机组结束振荡振幅限值  */
	fs << "gn_ewn_ampl2 = " << m_LfoPara.gn_ewn_ampl2 << ";" << endl; /* 机组结束振荡振幅限值2 */
	fs << "gn_ewn_ampl3 = " << m_LfoPara.gn_ewn_ampl2 << ";" << endl; /* 机组结束振荡振幅限值3  */
	fs << "ln_swn_ampl1 = " << m_LfoPara.ln_swn_ampl1 << ";" << endl; /* 线路开始振荡振幅限值1 */
	fs << "ln_swn_ampl2 = " << m_LfoPara.ln_swn_ampl2 << ";" << endl; /* 线路开始振荡振幅限值2  */
	fs << "ln_swn_ampl3 = " << m_LfoPara.ln_swn_ampl3 << ";" << endl; /* 线路开始振荡振幅限值3  */
	fs << "ln_ewn_ampl1 = " << m_LfoPara.ln_ewn_ampl1 << ";" << endl; /* 线路结束振荡振幅限值1 */
	fs << "ln_ewn_ampl2 = " << m_LfoPara.ln_ewn_ampl2 << ";" << endl; /* 线路结束振荡振幅限值2  */
	fs << "ln_ewn_ampl3 = " << m_LfoPara.ln_ewn_ampl3 << ";" << endl; /* 线路结束振荡振幅限值3 */
	fs << "tr_swn_ampl1 = " << m_LfoPara.tr_swn_ampl1 << ";" << endl; /* 变压器开始振荡振幅限值1  */
	fs << "tr_swn_ampl2 = " << m_LfoPara.tr_swn_ampl2 << ";" << endl; /* 变压器开始振荡振幅限值2  */
	fs << "tr_swn_ampl3 = " << m_LfoPara.tr_swn_ampl3 << ";" << endl; /* 变压器开始振荡振幅限值3 */
	fs << "tr_ewn_ampl1 = " << m_LfoPara.tr_ewn_ampl1 << ";" << endl; /* 变压器结束振荡振幅限值1  */
	fs << "tr_ewn_ampl2 = " << m_LfoPara.tr_ewn_ampl2 << ";" << endl; /* 变压器结束振荡振幅限值2 */
	fs << "tr_ewn_ampl3 = " << m_LfoPara.tr_ewn_ampl3 << ";" << endl; /* 变压器结束振荡振幅限值3  */
	fs << "volt_thre1 = " << m_LfoPara.volt_thre1 << ";" << endl; /* 电压等级1  */
	fs << "volt_thre2 = " << m_LfoPara.volt_thre2 << ";" << endl; /* 电压等级2 */
	fs << "gn_capacity1 = " << m_LfoPara.gn_capacity1 << ";" << endl; /* 机组容量1  */
	fs << "gn_capacity2 = " << m_LfoPara.gn_capacity2 << ";" << endl; /* 机组容量2 */
	fs << "fjud_wnap_per = " << m_LfoPara.fjud_wnap_per << ";" << endl; /* 预判振幅百分比  */
	fs << "lfo_jud_num = " << m_LfoPara.lfo_jud_num << ";" << endl; /* 判断次数 */
	fs << "rcd_pre_t = " << m_LfoPara.rcd_pre_t << ";" << endl; /* 记录超前时间  */
	fs << "rcd_aft_t = " << m_LfoPara.rcd_aft_t << ";" << endl; /* 记录延后时间 */
	fs << "tr_wn_ampl = " << m_LfoPara.tr_wn_ampl << ";" << endl; /* 线路振幅限值  */
	fs << "tr_wn2_ampl = " << m_LfoPara.tr_wn2_ampl << ";" << endl; /* 变压器振幅限值2  */
	fs << "time_leng = " << m_LfoPara.time_leng << ";" << endl; /* 观察时间窗口 */
	fs << "time_leng_coe = " << m_LfoPara.time_leng_coe << ";" << endl; /* 观察时间窗口系数 */
	fs << "sample_freq = " << m_LfoPara.sample_freq << ";" << endl; /* 采样频率  */
	fs << "sample_freq_coe = " << m_LfoPara.sample_freq_coe << ";" << endl; /* 采样频率系数  */
	fs << "int_time = " << m_LfoPara.int_time << ";" << endl; /* 交叠窗口  */
	fs << "int_time_coe = " << m_LfoPara.int_time_coe << ";" << endl; /* 交叠窗口系数  */
	fs << "warnlfo_time = " << m_LfoPara.warn_time_max << ";" << endl; /* 告警持续时间限值 */
	fs << "safelfo_time = " << m_LfoPara.safe_time_max << ";" << endl; /* 安全持续时间限值  */
	fs << "time_leng_max = " << m_LfoPara.time_leng_max << ";" << endl; /* 最大观察时间窗口 */
	fs << "time_leng_min = " << m_LfoPara.time_leng_min << ";" << endl; /* 最小观察时间窗口  */
	fs << "sample_freq_max = " << m_LfoPara.sample_freq_max << ";" << endl; /* 采样频率最大值 */
	fs << "sample_freq_min = " << m_LfoPara.sample_freq_min << ";" << endl; /* 采样频率最小值  */
	fs << "sh_fact = " << m_LfoPara.sh_fact << ";" << endl; /* 参与因子阀值  */
	fs << "sh_phas = " << m_LfoPara.sh_phas << ";" << endl; /* 相角超前阀值 */
	fs << "sh_gn_pow = " << m_LfoPara.sh_gn_pow << ";" << endl; /* 机组耗散功率阀值  */
	fs << "sh_ln_pow = " << m_LfoPara.sh_ln_pow << ";" << endl; /* 支路耗散功率阀值 */
	fs << "osc_pow_ampl = " << m_LfoPara.osc_pow_ampl << ";" << endl; /* 小扰动振幅门槛值  */
	fs << "osc_sec_ampl = " << m_LfoPara.osc_sec_ampl << ";" << endl; /* 小扰动持续时间门槛值  */
	fs << "if_flit_lfo = " << m_LfoPara.if_flit_lfo << ";" << endl; /* 是否过滤低频振荡 */
	fs << "Voltunit_is_kV = " << m_LfoPara.Voltunit_is_kV << ";" << endl; /* PMU采集电压单位是否为kV */
	fs << "Ampl_aver = " << m_LfoPara.para_Ampl_aver << ";" << endl; /* 解决斜直线问题，主导模式幅值占波形均值比 */
	fs << "thread_mode = " << m_LfoPara.thread_mode << ";" << endl; /* 分频段模式 */
	fs << "mainmode_num = " << m_LfoPara.mainmode_num << ";" << endl; /* 主要模式数量 */
	fs << "inf_ampl = " << m_LfoPara.inf_ampl << ";" << endl; /* 断面振幅系数 */
	fs << "sm_md_damp = " << m_LfoPara.sm_md_damp << ";" << endl; /* 同模式阻尼差 */
	fs << "para_VoltAmplPer = " << m_LfoPara.para_VoltAmplPer << ";" << endl; /* 厂站电压幅值百分比 */
	fs << "para_LfoAnaByPwr = " << m_LfoPara.para_LfoAnaByPwr << ";" << endl; /* 解决斜直线问题，主导模式幅值占波形均值比 */
	fs << "para_nShwMaxFcNum = " << m_LfoPara.para_nShwMaxFcNum << ";" << endl; /* 每群最大厂站数 */
	fs << "max_damp = " << m_LfoPara.max_damp << ";" << endl; /* 阻尼比饱和值 */
	fs << "center_volt_ampl = " << m_LfoPara.center_volt_ampl << endl;
	fs << "disp_num = " << m_LfoPara.disp_num << endl;
	fs << "pre_freq1 = " << m_LfoPara.pre_freq1 << endl;
	fs << "pre_freq2 = " << m_LfoPara.pre_freq2 << endl;
	fs << "pre_freq3 = " << m_LfoPara.pre_freq3 << endl;
	fs << "pre_freq4 = " << m_LfoPara.pre_freq4 << endl;
	fs << "pre_freq5 = " << m_LfoPara.pre_freq5 << endl;
	fs << "[/参数信息]" << endl;

	fs << "[测点信息]" << endl;
	for (ii = 0; ii < (int)tmpCase.vecdev.size(); ii++)
	{
		fs << INT_REC_VAL(tmpCase.vecdev[ii].fac_id) << " ";
		fs << INT_REC_VAL(tmpCase.vecdev[ii].dev_id1) << " ";			//设备ID
		fs << tmpCase.vecdev[ii].column_id << " ";
#ifdef _D5000
		fs << INT_REC_VAL(tmpCase.vecdev[ii].yc_id) << " ";
#else
		fs << tmpCase.vecdev[ii].dev_id << tmpCase.vecdev[ii].column_id << " ";
#endif
		fs << tmpCase.vecdev[ii].file_no << " ";
		fs << tmpCase.vecdev[ii].col_no << " ";
		fs << tmpCase.vecdev[ii].is_warn << ";" << endl;
	}
	fs << "[/测点信息]" << endl;


	fs << "[断面支路信息]" << endl;
	fs << "断面支路个数 = " << m_VecIntline.size() << endl;
	for (ii = 0; ii < m_VecIntline.size(); ii++)
	{
		fs << INT_REC_VAL(m_VecIntline[ii].inf_no) << ",";
		fs << m_VecIntline[ii].inf_name << ",";
		fs << m_VecIntline[ii].eletype << ",";
		fs << INT_REC_VAL(m_VecIntline[ii].line_no) << ",";
		fs << m_VecIntline[ii].line_name << ",";
		fs << m_VecIntline[ii].mpoint << ",";
		fs << INT_REC_VAL(m_VecIntline[ii].stat_no) << ",";
		fs << m_VecIntline[ii].onoff << ";" << endl;
	}
	fs << "[/断面支路信息]" << endl;

	fs << "[END]" << endl;

	fs.close();
#ifndef _LFO_HIS_LIB_	
	// 将文件从res目录拷贝到run目录下
	char filenmaeabs1[MAXBUF];
#ifdef _WINDOWS32
	sprintf(filenmaeabs1, "%s\\%s", m_CasefDir, filename);
#else
	sprintf(filenmaeabs1, "%s/%s", m_CasefDir, filename);
#endif
	sprintf(m_LogString, "案例标志文件[%s]", filenmaeabs1);
	MessShow(m_LogString, LTINFO);
	PF_CopyFileM(filenamebas, filenmaeabs1);

	//	PF_RemoveFile( filenamebas ); // 删除文件
#endif
#endif
	return 0;
}
#ifndef _WINDOWS32
#ifdef _D5000
#ifdef _NUSP

#define  GetKeyIdInfo(yc_id, key, columnid) \
{ \
INT_REC id;\
memcpy(&id, &yc_id, sizeof(INT_REC));\
key.create_key(yc_id.GetTableID(), yc_id.GetAreaSerialNo(),0); \
columnid = (int)yc_id.GetColumnNo(); \
}

#else

#define  GetKeyIdInfo(yc_id, key, columnid) \
{ \
KEY_STRU_AREA keyStru; \
memcpy(&keyStru, &yc_id, sizeof(KEY_STRU_AREA)); \
key = keyStru.key; \
columnid = keyStru.field_id; \
}

#endif // _NUSP
#endif // _D5000
#endif // _WINDOWS32

int CLfoAnaObj::SaveCaseTmd(LFO_CASE_INFO& LfoCase)
{
	char tmpString[1024], tmptime1[20], tmptime2[20];
	vector<string> VecLfoString;
	// 案例概要
	sprintf(tmpString, "%s", "<p align=\"center\"><b><font size=\"4\">功率振荡在线监视</font></b></p>");
	VecLfoString.push_back(tmpString);
	// 时间
	sprintf(tmptime1, "%s", PT_GetTimeStringBySeconds(LfoCase.start_time));
	sprintf(tmptime2, "%s", PT_GetTimeStringBySeconds(LfoCase.end_time));
	sprintf(tmpString, "<p align=\"left\">发生时间:  %s&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; "
		"消失时间:   %s&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 持续时间:   %s</p>",
		tmptime1, tmptime2,
		PT_GetStringOfTimeInterval(LfoCase.hold_time * 1000));
	VecLfoString.push_back(tmpString);
	// 触发设备
	sprintf(tmpString, "<p align=\"left\">触发设备:  %s%s&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
		"触发告警频率:   %6.3fHz&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;平均频率:   %6.3fHz</p>",
		LfoCase.warn_dev.dev.fac_name, LfoCase.warn_dev.dev.dev_name, LfoCase.warn_dev.mode.freq, LfoCase.lead_mode.freq);
	VecLfoString.push_back(tmpString);
	// 振荡设备数
	sprintf(tmpString, "<p align=\"left\">振荡机组数: %d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
		"振荡线路数: %d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;振荡变压数: %d</p>",
		LfoCase.lfo_gn_num, LfoCase.lfo_ln_num, LfoCase.lfo_tr_num);
	VecLfoString.push_back(tmpString);

	// 振荡机组
	if (LfoCase.lfo_gn_num > 0)
	{
		TmdAddDevInfo(LfoCase.vec_dev, VecLfoString, MENU_LFO_DEV_GEN);
	}
	// 振荡线路
	if (LfoCase.lfo_ln_num > 0)
	{
		TmdAddDevInfo(LfoCase.vec_dev, VecLfoString, MENU_LFO_DEV_LINE);
	}
	// 振荡变压器
	if (LfoCase.lfo_tr_num > 0)
	{
		TmdAddDevInfo(LfoCase.vec_dev, VecLfoString, MENU_LFO_DEV_TR);
	}

	// 振荡机组模式
	if (LfoCase.lfo_gn_num > 0)
	{
		TmdAddModeInfo(LfoCase.vec_dev, VecLfoString, MENU_LFO_DEV_GEN);
	}
	// 振荡线路模式
	if (LfoCase.lfo_ln_num > 0)
	{
		TmdAddModeInfo(LfoCase.vec_dev, VecLfoString, MENU_LFO_DEV_LINE);
	}
	// 振荡变压器模式
	if (LfoCase.lfo_tr_num > 0)
	{
		TmdAddModeInfo(LfoCase.vec_dev, VecLfoString, MENU_LFO_DEV_TR);
	}

	// 监视设备的测点
// 	vector<KEY_ID_STRU> VecKeyID;
// 	TmdAddKeyId(m_VecDev,VecKeyID,MENU_LFO_DEV_GEN);
// 	TmdAddKeyId(m_VecDev,VecKeyID,MENU_LFO_DEV_LINE);
// 	TmdAddKeyId(m_VecDev,VecKeyID,MENU_LFO_DEV_TR);
// 
// 	sprintf(m_LogString,"VecKeyID[%d]",(int)VecKeyID.size());
// 	MessShow(m_LogString,LTINFO);
	// 从遥测定义表中获取全部测点
	vector<FES_YC_TAB_CRE> VecFesYcTab;
#ifdef _WAMAP_
	PODB_GetFesYcInfo(VecFesYcTab);
#endif

	// 生成消息文件
	string tmpFileName = m_maindir + m_backslash + "res" + m_backslash + "testTMDsave.html";
	sprintf(m_LogString, "保存三态数据结果[%s]", tmpFileName.c_str());
	MessShow(m_LogString, LTINFO);
	fstream fs;
	fs.open(tmpFileName.c_str(), std::ios::out);
	if (fs.fail())
	{
		fs.close();
		return -1;
	}
	int ii;
	for (ii = 0; ii < (int)VecLfoString.size(); ii++)
	{
		fs << VecLfoString[ii].c_str() << endl;
	}
	for (ii = 0; ii < (int)VecFesYcTab.size(); ii++)
	{
		if (ii == 0)
		{
			fs << "record_id    column_id" << endl;
		}
		if (VecFesYcTab[ii].file_no >= 0 && VecFesYcTab[ii].col_no >= 0)
		{
#ifdef _NUSP
			INT_REC key;
			int colid;
#else
			int key;
			short colid;
#endif // _NUSP

#ifndef  _WINDOWS32	
#ifdef _D5000
			GetKeyIdInfo(VecFesYcTab[ii].yc_id, key, colid);
#endif
#endif	
			fs << INT_REC_VAL(key) << "  " << colid << endl;
			//fs<<VecFesYcTab[ii].yc_id.record_id<<"  "<<VecFesYcTab[ii].yc_id.column_id<<endl;
		}
	}

	fs.close();

#ifdef _WAMAP_
	if (m_LfoPara.is_tmd == MENU_LFO_MENU_NO)
	{
		sprintf(m_LogString, "是否触发三态数据[%d],退出", m_LfoPara.is_tmd);
		MessShow(m_LogString, LTINFO);
		return 0;
	}

#ifndef _LFO_HIS_LIB_
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
#endif
	// TODO:判断案例是否已存在

	// 填充三态数据结构
#ifndef _D5000
// 	tmd::LFO_S_REQ_STRU                  lfo_s_req;
// 	lfo_s_req.beg_time_e.tv_sec   = LfoCase.start_time; //  LFO发生时间
// 	lfo_s_req.beg_time_e.tv_usec  = 0;
// 	lfo_s_req.end_time_e.tv_sec   = LfoCase.end_time;  //  LFO结束时间
// 	lfo_s_req.end_time_e.tv_usec  = 0;
// 	lfo_s_req.beg_time_r.tv_sec   = LfoCase.start_time - m_LfoPara.rcd_pre_t;  //  开始数据记录的时间
// 	lfo_s_req.beg_time_r.tv_usec  = 0;
// 	lfo_s_req.end_time_r.tv_sec   = LfoCase.end_time + m_LfoPara.rcd_aft_t;  //  结束数据记录的时间
// 	lfo_s_req.end_time_r.tv_usec  = 0;
// 
// 	// 标题
// 	lfo_s_req.c_title = LfoCase.warn_dev.dev.dev_name;
// 	lfo_s_req.c_title.append("功率振荡");
// 
// 	// 填充简明信息
// 	for ( ii =0; ii<(int)VecLfoString.size(); ii++)
// 	{
// 		lfo_s_req.e_res_s.append(VecLfoString[ii]);
// 	}
// 	// 填充测点ID
// 	for ( ii=0; ii<(int)VecFesYcTab.size(); ii++)
// 	{
// 		if (VecFesYcTab[ii].file_no >=0 && VecFesYcTab[ii].col_no >=0)
// 		{
// 			lfo_s_req.vec_keyid.push_back(VecFesYcTab[ii].yc_id);
// 		}
// 	}
// 
// 	// 发送TMD消息
// 	std::vector<tmd::TBL_RW_REQ_STRU> vec_tbl_req;
// 	if(tmd::send_lfo_save_req(m_MsgBus, lfo_s_req, vec_tbl_req)<0) 
// 	{
// 		sprintf( m_LogString, "三态数据发送消息失败" ); 
// 		MessShow( m_LogString,LTERROR );
// 	}
#endif
#endif
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int CLfoAnaObj::AddLfoFc(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	vector<LFO_FC_TAB_CRE> VecLfoFcTab;
	GetLfoFac(VecLfoFcTab);

	vector<FAC_TAB_CRE> VecFacInfo;
	PODB_GetFacInfo(VecFacInfo);


	// 获取母线名称
	vector<BUS_TAB_CRE> VecBusTab;
	if (PODB_GetBusInfo(VecBusTab) < 0)
	{
		return false;
	}

	int i, jj, nFlag, kk, nFlag2;
	LFO_DEV_INFO tmpLfoDevInfo;
	WT_DEV_INFO  tmpDevInfo;
	for (i = 0; i < (int)VecLfoFcTab.size(); i++)
	{
		ClearLfoDev(tmpLfoDevInfo);
		memset((void*)&tmpDevInfo, 0, sizeof(WT_DEV_INFO));
		tmpDevInfo.dev_idx = VecLfoFcTab[i].fac_idx;
		tmpDevInfo.dev_id = VecLfoFcTab[i].fac_id;
		tmpDevInfo.fac_id = VecLfoFcTab[i].fac_id;
		tmpDevInfo.area_id = VecLfoFcTab[i].area_id;
		tmpDevInfo.dev_type = MENU_LFO_DEV_FAC;

		nFlag = 0;
		for (jj = 0; jj < (int)VecFacInfo.size(); jj++)
		{
			if (tmpDevInfo.dev_id == VecFacInfo[jj].fac_id)
			{
				sprintf(tmpDevInfo.dev_name, "%s", VecFacInfo[jj].fac_name);
				nFlag = 1;
				// 				sprintf(m_LogString,"[yjj]获取机组名称["INT_REC_FMT"]",INT_REC_VAL(tmpDevInfo.dev_id));
				// 				MessShow(m_LogString,LTERROR);
				break;
			}

		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "未找到对厂站名称["INT_REC_FMT"]", INT_REC_VAL(tmpDevInfo.dev_id));
			MessShow(m_LogString, LTERROR);
		}

		nFlag2 = 0;
		for (jj = 0; jj < VecBusTab.size(); jj++)
		{
			if (VecLfoFcTab[i].set_bus_id == VecBusTab[jj].bs_id)
			{
				tmpDevInfo.dot1.dot_id = VecBusTab[jj].bs_id;
				tmpDevInfo.dot1.fac_id = tmpDevInfo.fac_id;
				// 				tmpDevInfo.dot1.area_id = tmpDevInfo.area_id;
				sprintf(tmpDevInfo.dot1.dot_name, "%s", VecBusTab[jj].bs_name);
				sprintf(tmpDevInfo.dot1.fac_name, "%s", tmpDevInfo.dev_name);
				nFlag2 = 1;//首选母线已找到
				sprintf(m_LogString, "厂站[%s]对应母线[%s]", tmpDevInfo.dev_name, VecBusTab[jj].bs_name);
				MessShow(m_LogString);
				break;
			}
		}
		if (nFlag2 == 0)
		{
			sprintf(m_LogString, "厂站[%s]未找到对应母线", tmpDevInfo.dev_name);
			MessShow(m_LogString, LTERROR);

			ZERO_INT_REC(tmpDevInfo.dot1.dot_id);
			ZERO_INT_REC(tmpDevInfo.dot1.fac_id);
			//ZERO_INT_REC(tmpDevInfo.dot1.area_id);
		}
		// 		tmpLfoDevInfo.dev_info = tmpDevInfo;
		CopyLfoWtInfo(tmpLfoDevInfo.dev_info, tmpDevInfo);

		VecMonDev.push_back(tmpLfoDevInfo);
	}

#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int CLfoAnaObj::AddLfoGen(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	vector<LFO_GN_TAB_CRE> VecLfoGen;
	GetLfoGen(VecLfoGen);

	// 获取发电机名称
	vector<GEN_TAB_CRE> VecGenInfo;
	PODB_GetGenInfo(VecGenInfo);

	vector<FAC_TAB_CRE> VecFacInfo;
	PODB_GetFacInfo(VecFacInfo);

	vector<VL_TYPE_TAB_CRE> VecVLTypeTab;
	if (PODB_GetVLTypeInfo(VecVLTypeTab) < 0)
	{
		sprintf(m_LogString, "读取电压等级表[%d]获取电压信息失败1", VL_TYPE_NO_TAB);
		MessShow(m_LogString, LTERROR);
		//		return false;
	}

	int i, jj, nFlag, kk;
	VOL_INT_REC tmp_vlty_id;

	LFO_DEV_INFO tmpLfoDevInfo;
	WT_DEV_INFO  tmpDevInfo;
	for (i = 0; i < (int)VecLfoGen.size(); i++)
	{
		ClearLfoDev(tmpLfoDevInfo);
		memset((void*)&tmpDevInfo, 0, sizeof(WT_DEV_INFO));
		tmpDevInfo.dev_idx = VecLfoGen[i].gn_idx;
		tmpDevInfo.dev_id = VecLfoGen[i].gn_id;
		tmpDevInfo.fac_id = VecLfoGen[i].fac_id;
		tmpDevInfo.area_id = VecLfoGen[i].area_id;
		tmpDevInfo.dev_type = MENU_LFO_DEV_GEN;

		nFlag = 0;
		tmp_vlty_id = 0;
		for (jj = 0; jj < (int)VecGenInfo.size(); jj++)
		{
			if (tmpDevInfo.dev_id == VecGenInfo[jj].gn_id)
			{
				sprintf(tmpDevInfo.dev_name, "%s", VecGenInfo[jj].gn_name);
				tmpDevInfo.pmax = VecGenInfo[jj].pmax;
				nFlag = 1;
				// 				sprintf(m_LogString,"[yjj]获取机组名称["INT_REC_FMT"]",INT_REC_VAL(tmpDevInfo.dev_id));
				// 				MessShow(m_LogString,LTERROR);
				for (kk = 0; kk < VecFacInfo.size(); kk++)
				{
					if (tmpDevInfo.fac_id == VecFacInfo[kk].fac_id)
					{
						tmp_vlty_id = VecFacInfo[kk].vlty_id;
						break;
					}
				}
				for (kk = 0; kk < (int)VecVLTypeTab.size(); kk++)
				{
					if (tmp_vlty_id == VecVLTypeTab[kk].vlty_id)
					{
						// 						sprintf(m_LogString,"[yjj]获取机组名称["INT_REC_FMT"][%f]",INT_REC_VAL(tmpDevInfo.dev_id),VecVLTypeTab[kk].vbase);
						// 						MessShow(m_LogString,LTERROR);
						tmpDevInfo.maxvbase = VecVLTypeTab[kk].vbase;
						break;
					}
				}
				break;
			}

		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "未找到对应发电机名称["INT_REC_FMT"]", INT_REC_VAL(tmpDevInfo.dev_id));
			MessShow(m_LogString, LTERROR);
		}

		tmpDevInfo.dot1.dot_id = tmpDevInfo.dev_id;
		tmpDevInfo.dot1.fac_id = tmpDevInfo.fac_id;
		// 		tmpDevInfo.dot1.area_id = tmpDevInfo.area_id;
		sprintf(tmpDevInfo.dot1.dot_name, "%s", tmpDevInfo.dev_name);

		// 		tmpLfoDevInfo.dev_info = tmpDevInfo;
		CopyLfoWtInfo(tmpLfoDevInfo.dev_info, tmpDevInfo);
#ifdef _CHG_AMPL
		tmpLfoDevInfo.dev.is_chg_ampl = VecLfoGen[i].is_chg_ampl;
		tmpLfoDevInfo.dev.chg_ampl_per = VecLfoGen[i].chg_ampl_per;
#endif
		VecMonDev.push_back(tmpLfoDevInfo);
	}


#endif
	return 0;
}

int CLfoAnaObj::SetFacInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	vector<FAC_TAB_CRE> VecFacInfo;
	PODB_GetFacInfo(VecFacInfo);

	int ii, jj, nFlag1, nFlag2;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_GEN)
		{
			nFlag1 = 0;
			for (jj = 0; jj < (int)VecFacInfo.size(); jj++)
			{
				if (VecMonDev[ii].dev_info.fac_id == VecFacInfo[jj].fac_id)
				{
					nFlag1 = 1;
					sprintf(VecMonDev[ii].dev_info.fac_name, "%s", VecFacInfo[jj].fac_name);
					break;
				}
			}
			if (nFlag1 == 0)
			{
				sprintf(m_LogString, "未找到对应厂站名称["INT_REC_FMT"]", INT_REC_VAL(VecMonDev[ii].dev_info.fac_id));
				MessShow(m_LogString, LTERROR);
				sprintf(VecMonDev[ii].dev_info.dot1.fac_name, "%s", "未匹配");
			}
			else
			{
				sprintf(VecMonDev[ii].dev_info.dot1.fac_name, "%s", VecMonDev[ii].dev_info.fac_name);
			}
		}
		else if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_LINE)
		{
			nFlag1 = 0;
			nFlag2 = 0;
			for (jj = 0; jj < (int)VecFacInfo.size(); jj++)
			{
				if (VecMonDev[ii].dev_info.dot1.fac_id == VecFacInfo[jj].fac_id)
				{
					nFlag1 = 1;
					sprintf(VecMonDev[ii].dev_info.dot1.fac_name, "%s", VecFacInfo[jj].fac_name);
				}
				if (VecMonDev[ii].dev_info.dot2.fac_id == VecFacInfo[jj].fac_id)
				{
					nFlag2 = 1;
					sprintf(VecMonDev[ii].dev_info.dot2.fac_name, "%s", VecFacInfo[jj].fac_name);
				}
				if (nFlag1 == 1 && nFlag2 == 1)
				{
					break;
				}
			}
			if (nFlag1 == 0)
			{
				sprintf(m_LogString, "未找到对应厂站名称["INT_REC_FMT"]", INT_REC_VAL(VecMonDev[ii].dev_info.dot1.fac_id));
				MessShow(m_LogString, LTERROR);
				sprintf(VecMonDev[ii].dev_info.dot1.fac_name, "%s", "未匹配");
			}
			if (nFlag2 == 0)
			{
				sprintf(m_LogString, "未找到对应厂站名称["INT_REC_FMT"]", INT_REC_VAL(VecMonDev[ii].dev_info.dot2.fac_id));
				MessShow(m_LogString, LTERROR);
				sprintf(VecMonDev[ii].dev_info.dot2.fac_name, "%s", "未匹配");
			}
		}
		else if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_TR)
		{
			nFlag1 = 0;
			for (jj = 0; jj < (int)VecFacInfo.size(); jj++)
			{
				if (VecMonDev[ii].dev_info.fac_id == VecFacInfo[jj].fac_id)
				{
					nFlag1 = 1;
					sprintf(VecMonDev[ii].dev_info.fac_name, "%s", VecFacInfo[jj].fac_name);
					sprintf(VecMonDev[ii].dev_info.dot1.fac_name, "%s", VecFacInfo[jj].fac_name);
					sprintf(VecMonDev[ii].dev_info.dot2.fac_name, "%s", VecFacInfo[jj].fac_name);
					sprintf(VecMonDev[ii].dev_info.dot3.fac_name, "%s", VecFacInfo[jj].fac_name);
					break;
				}
			}
			if (nFlag1 == 0)
			{
				sprintf(m_LogString, "未找到对应厂站名称["INT_REC_FMT"]", INT_REC_VAL(VecMonDev[ii].dev_info.fac_id));
				MessShow(m_LogString, LTERROR);
				sprintf(VecMonDev[ii].dev_info.fac_name, "%s", "未匹配");
				sprintf(VecMonDev[ii].dev_info.dot1.fac_name, "%s", "未匹配");
				sprintf(VecMonDev[ii].dev_info.dot1.fac_name, "%s", "未匹配");
				sprintf(VecMonDev[ii].dev_info.dot1.fac_name, "%s", "未匹配");
			}
		}
	}
#endif
	return 0;
}

int CLfoAnaObj::SetAreaInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	vector<AREA_TAB_CRE> VecAreaInfo;
	PODB_GetAreaInfo(VecAreaInfo);

	int ii, jj, nFlag1, nFlag2;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_GEN)
		{
			nFlag1 = 0;
			for (jj = 0; jj < (int)VecAreaInfo.size(); jj++)
			{
				if (VecMonDev[ii].dev_info.area_id == VecAreaInfo[jj].area_id)
				{
					nFlag1 = 1;
					sprintf(VecMonDev[ii].dev_info.area_name, "%s", VecAreaInfo[jj].area_name);
					break;
				}
			}
			if (nFlag1 == 0)
			{
				sprintf(m_LogString, "未找到对应区域名称["INT_REC_FMT"]", INT_REC_VAL(VecMonDev[ii].dev_info.area_id));
				MessShow(m_LogString, LTERROR);
				sprintf(VecMonDev[ii].dev_info.area_name, "%s", "未匹配");
			}
			else
			{
				// 				sprintf(VecMonDev[ii].dev_info.dot1.area_name,"%s",VecMonDev[ii].dev_info.area_name);
			}
		}
		else if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_LINE)
		{
			// 			nFlag1 = 0;
			// 			nFlag2 = 0;
			// 			for (jj = 0; jj<(int)VecAreaInfo.size(); jj++)
			// 			{
			// 				if (VecMonDev[ii].dev_info.dot1.area_id == VecAreaInfo[jj].area_id)
			// 				{
			// 					nFlag1 =1;
			// 					sprintf(VecMonDev[ii].dev_info.dot1.area_name,"%s",VecAreaInfo[jj].area_name);
			// 				}
			// 				else if (VecMonDev[ii].dev_info.dot2.area_id == VecAreaInfo[jj].area_id)
			// 				{
			// 					nFlag2 =1;
			// 					sprintf(VecMonDev[ii].dev_info.dot2.area_name,"%s",VecAreaInfo[jj].area_name);
			// 				}
			// 				if (nFlag1 == 1 && nFlag2 == 1)
			// 				{
			// 					break;
			// 				}
			// 			}
			// 			if (nFlag1 ==0)
			// 			{
			// 				sprintf(m_LogString,"未找到对应区域名称["INT_REC_FMT"]",VecMonDev[ii].dev_info.dot1.area_id);
			// 				MessShow(m_LogString,LTERROR);
			// 				sprintf(VecMonDev[ii].dev_info.dot1.area_name,"%s","未匹配");
			// 			}
			// 			if (nFlag2 ==0)
			// 			{
			// 				sprintf(m_LogString,"未找到对应区域名称["INT_REC_FMT"]",VecMonDev[ii].dev_info.dot2.area_id);
			// 				MessShow(m_LogString,LTERROR);
			// 				sprintf(VecMonDev[ii].dev_info.dot2.area_name,"%s","未匹配");
			// 			}
		}
		else if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_TR)
		{
			nFlag1 = 0;
			for (jj = 0; jj < (int)VecAreaInfo.size(); jj++)
			{
				if (VecMonDev[ii].dev_info.area_id == VecAreaInfo[jj].area_id)
				{
					nFlag1 = 1;
					sprintf(VecMonDev[ii].dev_info.area_name, "%s", VecAreaInfo[jj].area_name);
					// 					sprintf(VecMonDev[ii].dev_info.dot1.area_name,"%s",VecAreaInfo[jj].area_name);
					// 					sprintf(VecMonDev[ii].dev_info.dot2.area_name,"%s",VecAreaInfo[jj].area_name);
					// 					sprintf(VecMonDev[ii].dev_info.dot3.area_name,"%s",VecAreaInfo[jj].area_name);
					break;
				}
			}
			if (nFlag1 == 0)
			{
				// 				sprintf(m_LogString,"未找到对应区域名称["INT_REC_FMT"]",VecMonDev[ii].dev_info.dot1.area_id);
				// 				MessShow(m_LogString,LTERROR);
				sprintf(VecMonDev[ii].dev_info.area_name, "%s", "未匹配");
				// 				sprintf(VecMonDev[ii].dev_info.dot1.area_name,"%s","未匹配");
				// 				sprintf(VecMonDev[ii].dev_info.dot2.area_name,"%s","未匹配");
				// 				sprintf(VecMonDev[ii].dev_info.dot3.area_name,"%s","未匹配");
			}
		}
	}
#endif
	return 0;
}

int CLfoAnaObj::SetVlTypeInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
	//  	vector<VL_TYPE_TAB_CRE> VecVltypeInfo;
	//  	PODB_GetVLTypeInfo(VecVltypeInfo);
	// 	
	// 	int ii,jj,nFlag;
	// 	for (ii =0; ii<(int)VecMonDev.size(); ii++)
	// 	{
	// 		if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_GEN)
	// 		{
	// 			nFlag = 0;
	// 			for (jj = 0; jj<(int)VecVltypeInfo.size(); jj++)
	// 			{
	// 				if (VecMonDev[ii].dev_info.maxvlty_id == VecVltypeInfo[jj].vlty_id)
	// 				{
	// 					nFlag = 1;
	// 					sprintf(VecMonDev[ii].dev_info.area_name,"%s",VecVltypeInfo[jj].area_name);
	// 					break;
	// 				}
	// 			}
	// 			if (nFlag == 0)
	// 			{
	// 				sprintf(m_LogString,"未找到对应区域名称[%d]",VecMonDev[ii].dev_info.area_id);
	// 				MessShow(m_LogString,LTERROR);
	// 			}
	// 			else
	// 			{
	// 				sprintf(VecMonDev[ii].dev_info.dot1.area_name,"%s",VecMonDev[ii].dev_info.area_name);
	// 			}
	// 		}
	// 	}
	return 0;
}

int CLfoAnaObj::SetFesYcInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	vector<FES_YC_TAB_CRE> VecFesYcTab;
	if (PODB_GetFesYcInfo(VecFesYcTab) < 0)
	{
		return 0;
	}

	int ii, jj, kk, nNum;
	INT_REC tmprecord_id;
	int tmpcolumn_id;
	vector<int> m_VecColGen;
	// UAV UAA PWR ANG ANC 
	PODB_InitColID(m_VecColGen, GN_DEVICE_NO_TAB);

	vector<int> m_VecColLn;
	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR
	if (PODB_InitColID(m_VecColLn, ACLN_DOT_NO_TAB) < 0)
	{
		//return false;
	}

	vector<int> m_VecColTr;
	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR
	if (PODB_InitColID(m_VecColTr, TRWD_DEVICE_NO_TAB) < 0)
	{
		//return false;
	}

	vector<int> m_VecColFac;
	if (PODB_InitColID(m_VecColFac, BS_DEVICE_NO_TAB) < 0)
	{

	}

	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_GEN)
		{
			// 填充WAMS前置遥测信息
			if (m_VecColGen.empty())
			{
				continue;
			}
			VecMonDev[ii].dev_info.dot1.IsYcDefine = WT_MENU_NO;
			VecMonDev[ii].dev_info.dot2.IsYcDefine = WT_MENU_NO;
			VecMonDev[ii].dev_info.dot3.IsYcDefine = WT_MENU_NO;
			for (jj = 0; jj < VEC_VIP_NUM; jj++)
			{
				VecMonDev[ii].dev_info.dot1.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot1.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot1.nYcColNo[jj] = -1;
				VecMonDev[ii].dev_info.dot2.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot2.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot2.nYcColNo[jj] = -1;
				VecMonDev[ii].dev_info.dot3.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot3.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot3.nYcColNo[jj] = -1;
			}
			if (!VALID_INT_REC(VecMonDev[ii].dev_info.dot1.dot_id))
			{
				sprintf(m_LogString, ""INT_REC_FMT",%s信息不全", INT_REC_VAL(VecMonDev[ii].dev_info.dev_id), VecMonDev[ii].dev_info.dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}

			for (jj = 0; jj < (int)VecFesYcTab.size(); jj++)
			{
				if (VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
				if (VecMonDev[ii].dev_info.dot1.dot_id == tmprecord_id)
				{
					// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
					for (kk = 0; kk < m_VecColGen.size(); kk++)
					{
						if (tmpcolumn_id == m_VecColGen[kk])
						{
							VecMonDev[ii].dev_info.dot1.nYcFlag[kk] = WT_MENU_YES;
							VecMonDev[ii].dev_info.dot1.nYcFilNo[kk] = VecFesYcTab[jj].file_no;
							VecMonDev[ii].dev_info.dot1.nYcColNo[kk] = VecFesYcTab[jj].col_no;
						}
					}
				}
				nNum = PODB_IsFlagAll(VecMonDev[ii].dev_info.dot1.nYcFlag, VEC_VIP_NUM);
				if (VecMonDev[ii].dev_info.dot1.nYcFlag[2] >= 0)
				{
					VecMonDev[ii].dev_info.dot1.IsYcDefine = WT_MENU_YES;
				}
				if (nNum == VEC_VIP_NUM)
				{
					break;
				}
			}
			if (VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_NO)
			{
				sprintf(m_LogString, "未找到对应测点信息或测点信息不全["INT_REC_FMT"][%s]", INT_REC_VAL(VecMonDev[ii].dev_info.dot1.dot_id), VecMonDev[ii].dev_info.dot1.dot_name);
				MessShow(m_LogString, LTERROR);
			}
			else
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 1;
			}
		}
		else if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_LINE)
		{
			// 填充WAMS前置遥测信息
			VecMonDev[ii].dev_info.dot1.IsYcDefine = WT_MENU_NO;
			VecMonDev[ii].dev_info.dot2.IsYcDefine = WT_MENU_NO;
			VecMonDev[ii].dev_info.dot3.IsYcDefine = WT_MENU_NO;
			for (jj = 0; jj < VEC_VIP_NUM; jj++)
			{
				VecMonDev[ii].dev_info.dot1.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot1.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot1.nYcColNo[jj] = -1;
				VecMonDev[ii].dev_info.dot2.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot2.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot2.nYcColNo[jj] = -1;
				VecMonDev[ii].dev_info.dot3.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot3.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot3.nYcColNo[jj] = -1;
			}
			if (!VALID_INT_REC(VecMonDev[ii].dev_info.dot1.dot_id) && !VALID_INT_REC(VecMonDev[ii].dev_info.dot2.dot_id))
			{
				sprintf(m_LogString, ""INT_REC_FMT",%s信息不全", INT_REC_VAL(VecMonDev[ii].dev_info.dev_id), VecMonDev[ii].dev_info.dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}

			for (jj = 0; jj < (int)VecFesYcTab.size(); jj++)
			{
				if (VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
				if (VecMonDev[ii].dev_info.dot1.dot_id == tmprecord_id)
				{
					// UAV UAA  PWR Q F
					for (kk = 0; kk < m_VecColLn.size(); kk++)
					{
						if (tmpcolumn_id == m_VecColLn[kk])
						{
							VecMonDev[ii].dev_info.dot1.nYcFlag[kk] = WT_MENU_YES;
							VecMonDev[ii].dev_info.dot1.nYcFilNo[kk] = VecFesYcTab[jj].file_no;
							VecMonDev[ii].dev_info.dot1.nYcColNo[kk] = VecFesYcTab[jj].col_no;
						}
					}
				}
				if (VecMonDev[ii].dev_info.dot2.dot_id == tmprecord_id)
				{
					// UAV UAA  PWR Q F
					for (kk = 0; kk < m_VecColLn.size(); kk++)
					{
						if (tmpcolumn_id == m_VecColLn[kk])
						{
							VecMonDev[ii].dev_info.dot2.nYcFlag[kk] = WT_MENU_YES;
							VecMonDev[ii].dev_info.dot2.nYcFilNo[kk] = VecFesYcTab[jj].file_no;
							VecMonDev[ii].dev_info.dot2.nYcColNo[kk] = VecFesYcTab[jj].col_no;
						}
					}
				}
				//				if (PODB_IsFlagAll1(VecMonDev[ii].dev_info.dot1.nYcFlag,13)==13)
				if (VecMonDev[ii].dev_info.dot1.nYcFlag[2] > 0)
				{
					VecMonDev[ii].dev_info.dot1.IsYcDefine = WT_MENU_YES;
				}
				//				if (PODB_IsFlagAll1(VecMonDev[ii].dev_info.dot2.nYcFlag,13)==13)
				if (VecMonDev[ii].dev_info.dot2.nYcFlag[2] > 0)
				{
					VecMonDev[ii].dev_info.dot2.IsYcDefine = WT_MENU_YES;
				}
				if ((VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES) && (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES))
				{
					break;
				}
			}
			if ((VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES) && (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES))
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 2;
			}
			else if (VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES)
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 1;
			}
			else if (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES)
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 1;
			}
			else
			{
				sprintf(m_LogString, "未找到对应测点信息或测点信息不全["INT_REC_FMT"][%s]", INT_REC_VAL(VecMonDev[ii].dev_info.dot1.dot_id), VecMonDev[ii].dev_info.dot1.dot_name);
				MessShow(m_LogString, LTERROR);
			}
		}
		else if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_TR)
		{
			VecMonDev[ii].dev_info.dot1.IsYcDefine = WT_MENU_NO;
			VecMonDev[ii].dev_info.dot2.IsYcDefine = WT_MENU_NO;
			VecMonDev[ii].dev_info.dot3.IsYcDefine = WT_MENU_NO;
			for (jj = 0; jj < VEC_VIP_NUM; jj++)
			{
				VecMonDev[ii].dev_info.dot1.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot1.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot1.nYcColNo[jj] = -1;
				VecMonDev[ii].dev_info.dot2.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot2.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot2.nYcColNo[jj] = -1;
				VecMonDev[ii].dev_info.dot3.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot3.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot3.nYcColNo[jj] = -1;
			}

			if (!VALID_INT_REC(VecMonDev[ii].dev_info.dot1.dot_id))
			{
				sprintf(m_LogString, ""INT_REC_FMT",%s信息不全", INT_REC_VAL(VecMonDev[ii].dev_info.dev_id), VecMonDev[ii].dev_info.dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}

			for (jj = 0; jj < (int)VecFesYcTab.size(); jj++)
			{
				if (VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
				if (VecMonDev[ii].dev_info.dot1.dot_id == tmprecord_id)
				{
					// UAV UAA  PWR Q F
					for (kk = 0; kk < m_VecColTr.size(); kk++)
					{
						if (tmpcolumn_id == m_VecColTr[kk])
						{
							VecMonDev[ii].dev_info.dot1.nYcFlag[kk] = WT_MENU_YES;
							VecMonDev[ii].dev_info.dot1.nYcFilNo[kk] = VecFesYcTab[jj].file_no;
							VecMonDev[ii].dev_info.dot1.nYcColNo[kk] = VecFesYcTab[jj].col_no;
						}
					}
				}
				if (VecMonDev[ii].dev_info.dot2.dot_id == tmprecord_id)
				{
					// UAV UAA  PWR Q F
					for (kk = 0; kk < m_VecColTr.size(); kk++)
					{
						if (tmpcolumn_id == m_VecColTr[kk])
						{
							VecMonDev[ii].dev_info.dot2.nYcFlag[kk] = WT_MENU_YES;
							VecMonDev[ii].dev_info.dot2.nYcFilNo[kk] = VecFesYcTab[jj].file_no;
							VecMonDev[ii].dev_info.dot2.nYcColNo[kk] = VecFesYcTab[jj].col_no;
						}
					}
				}
				if (VecMonDev[ii].dev_info.dot3.dot_id == tmprecord_id)
				{
					// UAV UAA  PWR Q F
					for (kk = 0; kk < m_VecColTr.size(); kk++)
					{
						if (tmpcolumn_id == m_VecColTr[kk])
						{
							VecMonDev[ii].dev_info.dot3.nYcFlag[kk] = WT_MENU_YES;
							VecMonDev[ii].dev_info.dot3.nYcFilNo[kk] = VecFesYcTab[jj].file_no;
							VecMonDev[ii].dev_info.dot3.nYcColNo[kk] = VecFesYcTab[jj].col_no;
						}
					}
				}
				//				if (PODB_IsFlagAll1(VecMonDev[ii].dev_info.dot1.nYcFlag,13)==13)
				if (VecMonDev[ii].dev_info.dot1.nYcFlag[2] >= 0)
				{
					VecMonDev[ii].dev_info.dot1.IsYcDefine = WT_MENU_YES;
				}
				//				if (PODB_IsFlagAll1(VecMonDev[ii].dev_info.dot2.nYcFlag,13)==13)
				if (VecMonDev[ii].dev_info.dot2.nYcFlag[2] >= 0)
				{
					VecMonDev[ii].dev_info.dot2.IsYcDefine = WT_MENU_YES;
				}
				//				if (PODB_IsFlagAll1(VecMonDev[ii].dev_info.dot3.nYcFlag,13)==13)
				if (VecMonDev[ii].dev_info.dot3.nYcFlag[2] >= 0)
				{
					VecMonDev[ii].dev_info.dot3.IsYcDefine = WT_MENU_YES;
				}

				if (VecMonDev[ii].dev_info.dev_type == THREE_WINDING)
				{
					if ((VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES) && (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES) && (VecMonDev[ii].dev_info.dot3.IsYcDefine == WT_MENU_YES))
					{
						break;
					}
				}
				else if (VecMonDev[ii].dev_info.dev_type == TWO_WINDING)
				{
					if ((VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES) && (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES))
					{
						break;
					}
				}
			}
			if ((VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES) && (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES) && (VecMonDev[ii].dev_info.dot3.IsYcDefine == WT_MENU_YES))
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 6;
			}
			else if ((VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES) && (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES))
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 3;
			}
			else if (VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES)
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 1;
			}
			else if (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES)
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 1;
			}
			else
			{
				sprintf(m_LogString, "未找到对应测点信息或测点信息不全["INT_REC_FMT"][%s]", INT_REC_VAL(VecMonDev[ii].dev_info.dot1.dot_id), VecMonDev[ii].dev_info.dot1.dot_name);
				MessShow(m_LogString, LTERROR);
			}
		}
		else if (VecMonDev[ii].dev_info.dev_type == MENU_LFO_DEV_FAC)
		{
			// 填充WAMS前置遥测信息
			if (m_VecColGen.empty())
			{
				continue;
			}
			VecMonDev[ii].dev_info.dot1.IsYcDefine = WT_MENU_NO;
			VecMonDev[ii].dev_info.dot2.IsYcDefine = WT_MENU_NO;
			VecMonDev[ii].dev_info.dot3.IsYcDefine = WT_MENU_NO;
			for (jj = 0; jj < VEC_VIP_NUM; jj++)
			{
				VecMonDev[ii].dev_info.dot1.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot1.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot1.nYcColNo[jj] = -1;
				VecMonDev[ii].dev_info.dot2.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot2.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot2.nYcColNo[jj] = -1;
				VecMonDev[ii].dev_info.dot3.nYcFlag[jj] = WT_MENU_NO;
				VecMonDev[ii].dev_info.dot3.nYcFilNo[jj] = -1;
				VecMonDev[ii].dev_info.dot3.nYcColNo[jj] = -1;
			}

			if (!VALID_INT_REC(VecMonDev[ii].dev_info.dot1.dot_id))
			{
				sprintf(m_LogString, ""INT_REC_FMT",%s信息不全", INT_REC_VAL(VecMonDev[ii].dev_info.dev_id), VecMonDev[ii].dev_info.dev_name);
				MessShow(m_LogString, LTINFO);
				continue;
			}

			for (jj = 0; jj < (int)VecFesYcTab.size(); jj++)
			{
				if (VecFesYcTab[jj].file_no < 0 || VecFesYcTab[jj].col_no < 0)
				{
					continue;
				}
				tmprecord_id = PODB_GetRecordId(VecFesYcTab[jj].yc_id);
				tmpcolumn_id = PODB_GetColumnId(VecFesYcTab[jj].yc_id);
				if (VecMonDev[ii].dev_info.dot1.dot_id == tmprecord_id)
				{
					// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA
					for (kk = 0; kk < m_VecColFac.size(); kk++)
					{
						if (tmpcolumn_id == m_VecColFac[kk])
						{
							VecMonDev[ii].dev_info.dot1.nYcFlag[kk] = WT_MENU_YES;
							VecMonDev[ii].dev_info.dot1.nYcFilNo[kk] = VecFesYcTab[jj].file_no;
							VecMonDev[ii].dev_info.dot1.nYcColNo[kk] = VecFesYcTab[jj].col_no;
						}
					}
				}
				nNum = PODB_IsFlagAll(VecMonDev[ii].dev_info.dot1.nYcFlag, m_VecColFac.size());
				if (VecMonDev[ii].dev_info.dot1.nYcFlag[0] >= 0)
				{
					VecMonDev[ii].dev_info.dot1.IsYcDefine = WT_MENU_YES;
				}
				if (nNum == m_VecColFac.size())
				{
					break;
				}
			}
			if (VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_NO)
			{
				sprintf(m_LogString, "未找到对应测点信息或测点信息不全["INT_REC_FMT"][%s]", INT_REC_VAL(VecMonDev[ii].dev_info.dot1.dot_id), VecMonDev[ii].dev_info.dot1.dot_name);
				MessShow(m_LogString, LTERROR);
			}
			else
			{
				// 置测点标志
				VecMonDev[ii].dev_info.yc_flag = 1;
			}
		}
	}
#endif
	return 0;
}

int CLfoAnaObj::SetDevInfo(vector<LFO_DEV_INFO>& VecMonDev)
{
	int ii, jj;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
		{//断面不需要填充基本信息
			continue;
		}
		VecMonDev[ii].dev.dev_idx = VecMonDev[ii].dev_info.dev_idx;
		VecMonDev[ii].dev.dev_id = VecMonDev[ii].dev_info.dev_id;
		VecMonDev[ii].dev.fac_id = VecMonDev[ii].dev_info.fac_id;
		VecMonDev[ii].dev.area_id = VecMonDev[ii].dev_info.area_id;
		sprintf(VecMonDev[ii].dev.dev_name, "%s", VecMonDev[ii].dev_info.dev_name);
		sprintf(VecMonDev[ii].dev.fac_name, "%s", VecMonDev[ii].dev_info.fac_name);
		sprintf(VecMonDev[ii].dev.area_name, "%s", VecMonDev[ii].dev_info.area_name);
		VecMonDev[ii].dev.dev_type = VecMonDev[ii].dev_info.dev_type;

		// 根据测点情况选择端点
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
		{
			VecMonDev[ii].dev.dot_id = VecMonDev[ii].dev_info.dot1.dot_id;
			VecMonDev[ii].dev.dot_fac_id = VecMonDev[ii].dev_info.dot1.fac_id;
			// 			VecMonDev[ii].dev.dot_area_id = VecMonDev[ii].dev_info.dot1.area_id;
			sprintf(VecMonDev[ii].dev.dot_dev_name, "%s", VecMonDev[ii].dev_info.dot1.dot_name);
			sprintf(VecMonDev[ii].dev.dot_fac_name, "%s", VecMonDev[ii].dev_info.dot1.fac_name);
			// 			sprintf(VecMonDev[ii].dev.dot_area_name,"%s",VecMonDev[ii].dev_info.dot1.area_name);
			VecMonDev[ii].dev.IsYcDefine = VecMonDev[ii].dev_info.dot1.IsYcDefine;
			for (jj = 0; jj < VEC_VIP_NUM; jj++)
			{
				VecMonDev[ii].dev.nYcFlag[jj] = VecMonDev[ii].dev_info.dot1.nYcFlag[jj];
				VecMonDev[ii].dev.nYcFilNo[jj] = VecMonDev[ii].dev_info.dot1.nYcFilNo[jj];
				VecMonDev[ii].dev.nYcColNo[jj] = VecMonDev[ii].dev_info.dot1.nYcColNo[jj];
			}

			if (VecMonDev[ii].dev_info.yc_flag <= 0)
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_NOYC;
				continue;
			}
			else
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_OK;
			}
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
		{
			if (VecMonDev[ii].dev_info.yc_flag <= 0 && m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_NOYC;
				sprintf(m_LogString, "线路名称[%s]无测点", VecMonDev[ii].dev.dev_name);
				MessShow(m_LogString, LTERROR);
				continue;
			}
			else
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_OK;
			}
			if (VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES)
			{
				// 选首端
				VecMonDev[ii].dev.dot_id = VecMonDev[ii].dev_info.dot1.dot_id;
				VecMonDev[ii].dev.dot_fac_id = VecMonDev[ii].dev_info.dot1.fac_id;
				// 				VecMonDev[ii].dev.dot_area_id = VecMonDev[ii].dev_info.dot1.area_id;
				sprintf(VecMonDev[ii].dev.dot_dev_name, "%s", VecMonDev[ii].dev_info.dot1.dot_name);
				sprintf(VecMonDev[ii].dev.dot_fac_name, "%s", VecMonDev[ii].dev_info.dot1.fac_name);
				// 				sprintf(VecMonDev[ii].dev.dot_area_name,"%s",VecMonDev[ii].dev_info.dot1.area_name);
				sprintf(VecMonDev[ii].dev.fac_name, "%s", VecMonDev[ii].dev_info.dot1.fac_name);
				VecMonDev[ii].dev.IsYcDefine = VecMonDev[ii].dev_info.dot1.IsYcDefine;
				for (jj = 0; jj < VEC_VIP_NUM; jj++)
				{
					VecMonDev[ii].dev.nYcFlag[jj] = VecMonDev[ii].dev_info.dot1.nYcFlag[jj];
					VecMonDev[ii].dev.nYcFilNo[jj] = VecMonDev[ii].dev_info.dot1.nYcFilNo[jj];
					VecMonDev[ii].dev.nYcColNo[jj] = VecMonDev[ii].dev_info.dot1.nYcColNo[jj];
				}
			}
			else
			{
				// 选末端
				VecMonDev[ii].dev.dot_id = VecMonDev[ii].dev_info.dot2.dot_id;
				VecMonDev[ii].dev.dot_fac_id = VecMonDev[ii].dev_info.dot2.fac_id;
				// 				VecMonDev[ii].dev.dot_area_id = VecMonDev[ii].dev_info.dot2.area_id;
				sprintf(VecMonDev[ii].dev.dot_dev_name, "%s", VecMonDev[ii].dev_info.dot2.dot_name);
				sprintf(VecMonDev[ii].dev.dot_fac_name, "%s", VecMonDev[ii].dev_info.dot2.fac_name);
				// 				sprintf(VecMonDev[ii].dev.dot_area_name,"%s",VecMonDev[ii].dev_info.dot2.area_name);
				sprintf(VecMonDev[ii].dev.fac_name, "%s", VecMonDev[ii].dev_info.dot2.fac_name);
				VecMonDev[ii].dev.IsYcDefine = VecMonDev[ii].dev_info.dot2.IsYcDefine;
				for (jj = 0; jj < VEC_VIP_NUM; jj++)
				{
					VecMonDev[ii].dev.nYcFlag[jj] = VecMonDev[ii].dev_info.dot2.nYcFlag[jj];
					VecMonDev[ii].dev.nYcFilNo[jj] = VecMonDev[ii].dev_info.dot2.nYcFilNo[jj];
					VecMonDev[ii].dev.nYcColNo[jj] = VecMonDev[ii].dev_info.dot2.nYcColNo[jj];
				}
			}
			// 			sprintf(m_LogString,"线路名称[%s],选定端点[%s]["INT_REC_FMT"]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].dev.dot_dev_name,INT_REC_VAL(VecMonDev[ii].dev.dot_id));
			// 			MessShow(m_LogString,LTERROR);
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
		{
			if (VecMonDev[ii].dev_info.yc_flag <= 0 && m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_NOYC;
				continue;
			}
			else
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_OK;
			}

			if (VecMonDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES)
			{
				// 选高压绕组
				VecMonDev[ii].dev.dot_id = VecMonDev[ii].dev_info.dot1.dot_id;
				VecMonDev[ii].dev.dot_fac_id = VecMonDev[ii].dev_info.dot1.fac_id;
				// 				VecMonDev[ii].dev.dot_area_id = VecMonDev[ii].dev_info.dot1.area_id;
				sprintf(VecMonDev[ii].dev.dot_dev_name, "%s", VecMonDev[ii].dev_info.dot1.dot_name);
				sprintf(VecMonDev[ii].dev.dot_fac_name, "%s", VecMonDev[ii].dev_info.dot1.fac_name);
				// 				sprintf(VecMonDev[ii].dev.dot_area_name,"%s",VecMonDev[ii].dev_info.dot1.area_name);
				VecMonDev[ii].dev.IsYcDefine = VecMonDev[ii].dev_info.dot1.IsYcDefine;
				for (jj = 0; jj < VEC_VIP_NUM; jj++)
				{
					VecMonDev[ii].dev.nYcFlag[jj] = VecMonDev[ii].dev_info.dot1.nYcFlag[jj];
					VecMonDev[ii].dev.nYcFilNo[jj] = VecMonDev[ii].dev_info.dot1.nYcFilNo[jj];
					VecMonDev[ii].dev.nYcColNo[jj] = VecMonDev[ii].dev_info.dot1.nYcColNo[jj];
				}
			}
			else if (VecMonDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES)
			{
				// 选低压绕组
				VecMonDev[ii].dev.dot_id = VecMonDev[ii].dev_info.dot2.dot_id;
				VecMonDev[ii].dev.dot_fac_id = VecMonDev[ii].dev_info.dot2.fac_id;
				// 				VecMonDev[ii].dev.dot_area_id = VecMonDev[ii].dev_info.dot2.area_id;
				sprintf(VecMonDev[ii].dev.dot_dev_name, "%s", VecMonDev[ii].dev_info.dot2.dot_name);
				sprintf(VecMonDev[ii].dev.dot_fac_name, "%s", VecMonDev[ii].dev_info.dot2.fac_name);
				// 				sprintf(VecMonDev[ii].dev.dot_area_name,"%s",VecMonDev[ii].dev_info.dot2.area_name);
				VecMonDev[ii].dev.IsYcDefine = VecMonDev[ii].dev_info.dot2.IsYcDefine;
				for (jj = 0; jj < VEC_VIP_NUM; jj++)
				{
					VecMonDev[ii].dev.nYcFlag[jj] = VecMonDev[ii].dev_info.dot2.nYcFlag[jj];
					VecMonDev[ii].dev.nYcFilNo[jj] = VecMonDev[ii].dev_info.dot2.nYcFilNo[jj];
					VecMonDev[ii].dev.nYcColNo[jj] = VecMonDev[ii].dev_info.dot2.nYcColNo[jj];
				}
			}
			else if (VecMonDev[ii].dev_info.dot3.IsYcDefine == WT_MENU_YES)
			{
				// 选中压绕组
				VecMonDev[ii].dev.dot_id = VecMonDev[ii].dev_info.dot3.dot_id;
				VecMonDev[ii].dev.dot_fac_id = VecMonDev[ii].dev_info.dot3.fac_id;
				// 				VecMonDev[ii].dev.dot_area_id = VecMonDev[ii].dev_info.dot3.area_id;
				sprintf(VecMonDev[ii].dev.dot_dev_name, "%s", VecMonDev[ii].dev_info.dot3.dot_name);
				sprintf(VecMonDev[ii].dev.dot_fac_name, "%s", VecMonDev[ii].dev_info.dot3.fac_name);
				// 				sprintf(VecMonDev[ii].dev.dot_area_name,"%s",VecMonDev[ii].dev_info.dot3.area_name);
				VecMonDev[ii].dev.IsYcDefine = VecMonDev[ii].dev_info.dot3.IsYcDefine;
				for (jj = 0; jj < VEC_VIP_NUM; jj++)
				{
					VecMonDev[ii].dev.nYcFlag[jj] = VecMonDev[ii].dev_info.dot3.nYcFlag[jj];
					VecMonDev[ii].dev.nYcFilNo[jj] = VecMonDev[ii].dev_info.dot3.nYcFilNo[jj];
					VecMonDev[ii].dev.nYcColNo[jj] = VecMonDev[ii].dev_info.dot3.nYcColNo[jj];
				}
			}
			// 			sprintf(m_LogString,"变压器名称[%s],选定绕组[%s]["INT_REC_FMT"]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].dev.dot_dev_name,INT_REC_VAL(VecMonDev[ii].dev.dot_id));
			// 			MessShow(m_LogString,LTERROR);
		}
		else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
		{
			VecMonDev[ii].dev.dot_id = VecMonDev[ii].dev_info.dot1.dot_id;
			VecMonDev[ii].dev.dot_fac_id = VecMonDev[ii].dev_info.dot1.fac_id;
			// 			VecMonDev[ii].dev.dot_area_id = VecMonDev[ii].dev_info.dot1.area_id;
			sprintf(VecMonDev[ii].dev.dot_dev_name, "%s", VecMonDev[ii].dev_info.dot1.dot_name);
			sprintf(VecMonDev[ii].dev.dot_fac_name, "%s", VecMonDev[ii].dev_info.dot1.fac_name);
			// 			sprintf(VecMonDev[ii].dev.dot_area_name,"%s",VecMonDev[ii].dev_info.dot1.area_name);
			VecMonDev[ii].dev.IsYcDefine = VecMonDev[ii].dev_info.dot1.IsYcDefine;

			for (jj = 0; jj < VEC_VU_NUM; jj++)
			{
				VecMonDev[ii].dev.nYcFlag[jj] = VecMonDev[ii].dev_info.dot1.nYcFlag[jj];
				VecMonDev[ii].dev.nYcFilNo[jj] = VecMonDev[ii].dev_info.dot1.nYcFilNo[jj];
				VecMonDev[ii].dev.nYcColNo[jj] = VecMonDev[ii].dev_info.dot1.nYcColNo[jj];
			}
			if (VecMonDev[ii].dev_info.yc_flag <= 0 && m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_NOYC;
				sprintf(m_LogString, "厂站静态信息:名称[%s],ID["INT_REC_FMT"],无测点", VecMonDev[ii].dev.dev_name, INT_REC_VAL(VecMonDev[ii].dev.dev_id));
				MessShow(m_LogString, LTERROR);
				continue;
			}
			else
			{
				VecMonDev[ii].stat = MENU_LFO_DEV_STS_OK;
			}
			// 			sprintf(m_LogString,"厂站静态信息:名称[%s],ID[%ld],测点[%d]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].dev.dev_id,VecMonDev[ii].dev.nYcColNo[0]);
			// 			MessShow(m_LogString,LTERROR);
		}
		// 默认取dot1测点		
// 		VecMonDev[ii].dev.dev_type = VecMonDev[ii].dev_info.dev_type;
// 		VecMonDev[ii].dev.dot_id = VecMonDev[ii].dev_info.dot1.dot_id;
// 		VecMonDev[ii].dev.dot_fac_id = VecMonDev[ii].dev_info.dot1.fac_id;
// 		VecMonDev[ii].dev.dot_area_id = VecMonDev[ii].dev_info.dot1.area_id;
// 		sprintf(VecMonDev[ii].dev.dot_dev_name,"%s",VecMonDev[ii].dev_info.dot1.dot_name);
// 		sprintf(VecMonDev[ii].dev.dot_fac_name,"%s",VecMonDev[ii].dev_info.dot1.fac_name);
// 		sprintf(VecMonDev[ii].dev.dot_area_name,"%s",VecMonDev[ii].dev_info.dot1.fac_name);
// 		VecMonDev[ii].dev.IsYcDefine = VecMonDev[ii].dev_info.dot1.IsYcDefine;
// 		for (jj =0; jj<VEC_VIP_NUM;jj++)
// 		{
// 			VecMonDev[ii].dev.nYcFlag[jj] = VecMonDev[ii].dev_info.dot1.nYcFlag[jj];
// 			VecMonDev[ii].dev.nYcFilNo[jj] = VecMonDev[ii].dev_info.dot1.nYcFilNo[jj];
// 			VecMonDev[ii].dev.nYcColNo[jj] = VecMonDev[ii].dev_info.dot1.nYcColNo[jj];
// 		}
// 		
// 		if (VecMonDev[ii].dev_info.yc_flag <=0)
// 		{
// 			VecMonDev[ii].stat = MENU_LFO_DEV_STS_NOYC;
// 		}
// 		else
// 		{
// 			VecMonDev[ii].stat = MENU_LFO_DEV_STS_OK;
// 		}
		// 生成测点名称(厂站名称_设备名_测量量名)
// 		for (jj =0; jj<VEC_VIP_NUM;jj++)
// 		{
// 			sprintf(VecMonDev[ii].dev.yc_name[jj],"%s_%s_%s",VecMonDev[ii].dev.fac_name,VecMonDev[ii].dev.dev_name,PODB_GetYcName(jj).c_str());
// 		}
	}

	return 0;
}

int CLfoAnaObj::AddLfoLine(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	vector<LFO_LN_TAB_CRE> VecLfoLine;
	GetLfoLine(VecLfoLine);

	vector<ACLINE_TAB_CRE> VecLineInfo;
	PODB_GetAclnInfo(VecLineInfo);

	vector<ACLNDOT_TAB_CRE> VecAclnDotInfo;
	PODB_GetAclnDotInfo(VecAclnDotInfo);

	vector<VL_TYPE_TAB_CRE> VecVLTypeTab;
	if (PODB_GetVLTypeInfo(VecVLTypeTab) < 0)
	{
		sprintf(m_LogString, "读取电压等级表[%d]获取电压信息失败2", VL_TYPE_NO_TAB);
		MessShow(m_LogString, LTERROR);
		//		return false;
	}

	int i, jj, nFlag;
	LFO_DEV_INFO tmpLfoDevInfo;
	WT_DEV_INFO  tmpDevInfo;
	for (i = 0; i < (int)VecLfoLine.size(); i++)
	{
		ClearLfoDev(tmpLfoDevInfo);
		memset((void*)&tmpDevInfo, 0, sizeof(WT_DEV_INFO));
		tmpDevInfo.dev_idx = VecLfoLine[i].acln_idx;
		tmpDevInfo.dev_id = VecLfoLine[i].acln_id;
		tmpDevInfo.maxvlty_id = VecLfoLine[i].vlty_id;
		tmpDevInfo.dev_type = MENU_LFO_DEV_LINE;

		nFlag = 0;
		for (jj = 0; jj < (int)VecLineInfo.size(); jj++)
		{
			if (tmpDevInfo.dev_id == VecLineInfo[jj].acln_id)
			{
				sprintf(tmpDevInfo.dev_name, "%s", VecLineInfo[jj].acln_name);
				tmpDevInfo.imax1 = VecLineInfo[jj].imax1;
				nFlag = 1;
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "未找到对应交流线路名称["INT_REC_FMT"]", INT_REC_VAL(tmpDevInfo.dev_id));
			MessShow(m_LogString, LTERROR);
		}

		for (jj = 0; jj < (int)VecVLTypeTab.size(); jj++)
		{
			if (VecLfoLine[i].vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				// 				sprintf(m_LogString,"[yjj]线路名称["INT_REC_FMT"][%f]",INT_REC_VAL(tmpDevInfo.dev_id),VecVLTypeTab[jj].vbase);
				// 				MessShow(m_LogString,LTERROR);
				tmpDevInfo.maxvbase = VecVLTypeTab[jj].vbase;
				break;
			}
		}

		tmpDevInfo.dot1.dot_id = VecLfoLine[i].dot_id_1;
		tmpDevInfo.dot1.fac_id = VecLfoLine[i].fac_id_1;
		// 		tmpDevInfo.dot1.area_id = VecLfoLine[i].area_id_1;

		tmpDevInfo.dot2.dot_id = VecLfoLine[i].dot_id_2;
		tmpDevInfo.dot2.fac_id = VecLfoLine[i].fac_id_2;
		// 		tmpDevInfo.dot2.area_id = VecLfoLine[i].area_id_2;
		nFlag = 0;
		for (jj = 0; jj < (int)VecAclnDotInfo.size(); jj++)
		{
			if (tmpDevInfo.dot1.dot_id == VecAclnDotInfo[jj].acln_dot_id)
			{
				nFlag++;
				sprintf(tmpDevInfo.dot1.dot_name, "%s", VecAclnDotInfo[jj].acln_dot_name);
				//  				sprintf(m_LogString,"[yjj]线路首端名称["INT_REC_FMT"][%s]",tmpDevInfo.dot1.dot_id,tmpDevInfo.dot1.dot_name);
				//  				MessShow(m_LogString);
			}
			else if (tmpDevInfo.dot2.dot_id == VecAclnDotInfo[jj].acln_dot_id)
			{
				nFlag++;
				sprintf(tmpDevInfo.dot2.dot_name, "%s", VecAclnDotInfo[jj].acln_dot_name);
				// 				sprintf(m_LogString,"[yjj]线路末端名称["INT_REC_FMT"][%s]",tmpDevInfo.dot2.dot_id,tmpDevInfo.dot2.dot_name);
				//  				MessShow(m_LogString);
			}
			if (nFlag >= 2)
			{
				break;
			}
		}
		// 		sprintf(m_LogString,"[yjj]线路端点数[%d]",nFlag);
		// 		MessShow(m_LogString);
		if (nFlag < 2)
		{
			sprintf(m_LogString, "未找到对应端点名称");
			MessShow(m_LogString, LTERROR);
		}

		// 		tmpLfoDevInfo.dev_info = tmpDevInfo;
		CopyLfoWtInfo(tmpLfoDevInfo.dev_info, tmpDevInfo);
#ifdef _CHG_AMPL
		tmpLfoDevInfo.dev.is_chg_ampl = VecLfoLine[i].is_chg_ampl;
		tmpLfoDevInfo.dev.chg_ampl_per = VecLfoLine[i].chg_ampl_per;
#endif		
		VecMonDev.push_back(tmpLfoDevInfo);
	}

#endif
	return 0;
}

int CLfoAnaObj::AddLfoTr(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	vector<LFO_TR_TAB_CRE> VecLfoTr;
	GetLfoTr(VecLfoTr);

	vector<TR_TAB_CRE> VecTrInfo;
	PODB_GetTrInfo(VecTrInfo);

	vector<TRWD_TAB_CRE> VecTrWdInfo;
	PODB_GetTrwdInfo(VecTrWdInfo);

	vector<VL_TYPE_TAB_CRE> VecVLTypeTab;
	if (PODB_GetVLTypeInfo(VecVLTypeTab) < 0)
	{
		sprintf(m_LogString, "读取电压等级表[%d]获取电压信息失败3", VL_TYPE_NO_TAB);
		MessShow(m_LogString, LTERROR);
		//		return false;
	}

	int i, jj, nFlag;
	float pwr = -1.0f;
	LFO_DEV_INFO tmpLfoDevInfo;
	WT_DEV_INFO  tmpDevInfo;
	for (i = 0; i < (int)VecLfoTr.size(); i++)
	{
		ClearLfoDev(tmpLfoDevInfo);
		memset((void*)&tmpDevInfo, 0, sizeof(WT_DEV_INFO));
		tmpDevInfo.dev_idx = VecLfoTr[i].tr_idx;
		tmpDevInfo.dev_id = VecLfoTr[i].tr_id;
		tmpDevInfo.fac_id = VecLfoTr[i].fac_id;
		tmpDevInfo.area_id = VecLfoTr[i].area_id;
		tmpDevInfo.dev_type = MENU_LFO_DEV_TR;

		nFlag = 0;
		for (jj = 0; jj < (int)VecTrInfo.size(); jj++)
		{
			if (tmpDevInfo.dev_id == VecTrInfo[jj].tr_id)
			{
				sprintf(tmpDevInfo.dev_name, "%s", VecTrInfo[jj].tr_name);
				nFlag = 1;
				break;
			}
		}
		if (nFlag == 0)
		{
			sprintf(m_LogString, "未找到对应变压器名称["INT_REC_FMT"]", INT_REC_VAL(tmpDevInfo.dev_id));
			MessShow(m_LogString, LTERROR);
		}

		tmpDevInfo.dot1.dot_id = VecLfoTr[i].high_wind_id;
		tmpDevInfo.dot1.fac_id = VecLfoTr[i].fac_id;
		// 		tmpDevInfo.dot1.area_id = VecLfoTr[i].area_id;
		tmpDevInfo.dot1.vlty_id = VecLfoTr[i].high_vlty_id;

		tmpDevInfo.dot2.dot_id = VecLfoTr[i].low_wind_id;
		tmpDevInfo.dot2.fac_id = VecLfoTr[i].fac_id;
		// 		tmpDevInfo.dot2.area_id = VecLfoTr[i].area_id;
		tmpDevInfo.dot2.vlty_id = VecLfoTr[i].low_vlty_id;

		tmpDevInfo.dot3.dot_id = VecLfoTr[i].mid_wind_id;
		tmpDevInfo.dot3.fac_id = VecLfoTr[i].fac_id;
		// 		tmpDevInfo.dot3.area_id = VecLfoTr[i].area_id;
		tmpDevInfo.dot3.vlty_id = VecLfoTr[i].mid_vlty_id;

		nFlag = 0;
		for (jj = 0; jj < (int)VecTrWdInfo.size(); jj++)
		{
			if (tmpDevInfo.dot1.dot_id == VecTrWdInfo[jj].trwd_id)
			{
				nFlag++;
				sprintf(tmpDevInfo.dot1.dot_name, "%s", VecTrWdInfo[jj].trwd_name);
			}
			else if (tmpDevInfo.dot2.dot_id == VecTrWdInfo[jj].trwd_id)
			{
				nFlag++;
				sprintf(tmpDevInfo.dot2.dot_name, "%s", VecTrWdInfo[jj].trwd_name);
			}
			else if (tmpDevInfo.dot3.dot_id == VecTrWdInfo[jj].trwd_id)
			{
				nFlag++;
				sprintf(tmpDevInfo.dot3.dot_name, "%s", VecTrWdInfo[jj].trwd_name);
			}
			if (nFlag >= 3)
			{
				break;
			}
		}

		for (jj = 0; jj < (int)VecTrWdInfo.size(); jj++)
		{
			if (VecTrWdInfo[jj].tr_id == VecLfoTr[i].tr_id)
			{
				pwr = VecTrWdInfo[jj].snom;
			}
			if (tmpDevInfo.max_pwr < pwr)
			{
				tmpDevInfo.max_pwr = pwr;
			}
		}

		for (jj = 0; jj < (int)VecVLTypeTab.size(); jj++)
		{
			if (VecLfoTr[i].high_vlty_id == VecVLTypeTab[jj].vlty_id)
			{
				tmpDevInfo.maxvbase = VecVLTypeTab[jj].vbase;
			}
			// 			if( VecLfoTr[i].mid_vlty_id == VecVLTypeTab[jj].vlty_id )
			// 			{
			// 				VecLfoTr[i].vmid   = VecVLTypeTab[jj].vbase;
			// 			}
			// 			if( VecLfoTr[i].low_vlty_id == VecVLTypeTab[jj].vlty_id )
			// 			{
			// 				VecLfoTr[i].vlow   = VecVLTypeTab[jj].vbase;
			// 			}
		}

		if (nFlag < 3)
		{
			sprintf(m_LogString, "变压器[%s]未找到对应绕组名称,nFlag=%d", tmpDevInfo.dev_name, nFlag);
			MessShow(m_LogString, LTERROR);
		}

		// 		tmpLfoDevInfo.dev_info = tmpDevInfo;
		CopyLfoWtInfo(tmpLfoDevInfo.dev_info, tmpDevInfo);
#ifdef _CHG_AMPL
		tmpLfoDevInfo.dev.is_chg_ampl = VecLfoTr[i].is_chg_ampl;
		tmpLfoDevInfo.dev.chg_ampl_per = VecLfoTr[i].chg_ampl_per;
#endif

		VecMonDev.push_back(tmpLfoDevInfo);
	}

#endif
	return 0;
}

//读取输电断面信息
int CLfoAnaObj::AddLfoIntface(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	vector<LFO_INT_TAB_CRE> VecLfoInt;
	GetLfoIntface(VecLfoInt);
	int i, jj, nFlag;
	float pwr = -1.0f;
	LFO_DEV_INFO tmpLfoDevInfo;
	LFO_DEV_BASE  tmpDevInfo;
	for (i = 0; i < (int)VecLfoInt.size(); i++)
	{
		if (VecLfoInt[i].is_monitor == MENU_LFO_MON_NO)
		{
			continue;
		}
		ClearLfoDev(tmpLfoDevInfo);
		memset((void*)&tmpDevInfo, 0, sizeof(LFO_DEV_BASE));
		tmpDevInfo.dev_idx = VecLfoInt[i].inf_idx;
		tmpDevInfo.dev_id = VecLfoInt[i].inf_idx;
		tmpDevInfo.dev_type = MENU_LFO_DEV_ITFC;
		sprintf(tmpDevInfo.dev_name, "%s", VecLfoInt[i].inf_name);


		// 		tmpLfoDevInfo.dev = tmpDevInfo;
		CopyLfoDevBase(tmpLfoDevInfo.dev, tmpDevInfo);

		sprintf(m_LogString, "断面名称[%s]["INT_REC_FMT"]", tmpLfoDevInfo.dev.dev_name, INT_REC_VAL(tmpLfoDevInfo.dev.dev_id));
		MessShow(m_LogString, LTINFO);
		VecMonDev.push_back(tmpLfoDevInfo);
	}
#endif
	return 0;
}

int CLfoAnaObj::AddLfoIntLine(vector<LFO_INTLINE_INFO>& VecINTLINE)
{
	vector<LFO_INTLINE_TAB_CRE> VecLfoIntline;
	GetLfoIntline(VecLfoIntline);
	int i, jj, nFlag;
	float pwr = -1.0f;
	LFO_INTLINE_INFO tmpLfoINTlineInfo;
	for (i = 0; i < (int)VecLfoIntline.size(); i++)
	{
		// 		if (VecLfoInt[i].is_monitor == MENU_LFO_MON_NO)
		// 		{
		// 			continue;
		// 		}
		// 		ClearLfoDev(tmpLfoINTlineInfo);
		tmpLfoINTlineInfo.recordid = VecLfoIntline[i].recordid;
		tmpLfoINTlineInfo.inf_no = VecLfoIntline[i].inf_no;
		tmpLfoINTlineInfo.eletype = VecLfoIntline[i].eletype;
		tmpLfoINTlineInfo.line_no = VecLfoIntline[i].line_no;
		tmpLfoINTlineInfo.mpoint = VecLfoIntline[i].mpoint;
		tmpLfoINTlineInfo.stat_no = VecLfoIntline[i].stat_no;
		tmpLfoINTlineInfo.onoff = VecLfoIntline[i].onoff;

		tmpLfoINTlineInfo.ampl_limit_s = 0.0;
		tmpLfoINTlineInfo.ampl_limit_e = 0.0;
		memset((void*)&tmpLfoINTlineInfo.curve, 0, sizeof(LFO_CURVE_INFO));
		ClearLfoDev(tmpLfoINTlineInfo.dev_info);
		ClearLfoModeInfo(tmpLfoINTlineInfo.mode);
		ClearLfoModeInfo(tmpLfoINTlineInfo.mode_shape);
		ClearLfoModeInfo(tmpLfoINTlineInfo.vec_mode);

		sprintf(tmpLfoINTlineInfo.inf_name, "%s", VecLfoIntline[i].inf_name);
		sprintf(tmpLfoINTlineInfo.line_name, "%s", VecLfoIntline[i].line_name);
		sprintf(m_LogString, "断面名称[%s]["INT_REC_FMT"]支路名称[%s]["INT_REC_FMT"],测点厂站["INT_REC_FMT"]", tmpLfoINTlineInfo.inf_name, INT_REC_VAL(tmpLfoINTlineInfo.inf_no), tmpLfoINTlineInfo.line_name, INT_REC_VAL(tmpLfoINTlineInfo.line_no), INT_REC_VAL(tmpLfoINTlineInfo.stat_no));
		MessShow(m_LogString, LTINFO);



		VecINTLINE.push_back(tmpLfoINTlineInfo);
	}
}
#ifndef _LFO_HIS_LIB_
int CLfoAnaObj::UpdateMonGnTable(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	int ii;
	LFO_GN_TAB_MONWRITE tmpMonDev;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN)
		{
			continue;
		}
		memset((void*)&tmpMonDev, 0, sizeof(LFO_GN_TAB_MONWRITE));
		tmpMonDev.stat = PODB_GetVarNsquare(2, VecMonDev[ii].stat - 1);
		tmpMonDev.stat_val = PODB_GetVarNsquare(2, VecMonDev[ii].stat - 1);

		//   		sprintf(m_LogString,"[yjj]UpdateMonGnTable设备状态[%d]!",tmpMonDev.stat);
		//   		MessShow(m_LogString,LTINFO);
		tmpMonDev.lead_ampl = VecMonDev[ii].mode.ampl;
		tmpMonDev.lead_freq = VecMonDev[ii].mode.freq;
		tmpMonDev.lead_damp = VecMonDev[ii].mode.damp;
		tmpMonDev.lead_phas = VecMonDev[ii].mode.phas;
		//tmpMonDev.lead_real = VecMonDev[ii].mode.real;
		//tmpMonDev.lead_imag = VecMonDev[ii].mode.imag;
		tmpMonDev.lead_engy = VecMonDev[ii].mode.engy;
		//tmpMonDev.lead_fail = VecMonDev[ii].mode.fail;
		tmpMonDev.aver_val = VecMonDev[ii].curve.aver_val;
		tmpMonDev.max_val = VecMonDev[ii].curve.max_val;
		tmpMonDev.max_val_t = VecMonDev[ii].curve.max_val_t;
		tmpMonDev.min_val = VecMonDev[ii].curve.min_val;
		tmpMonDev.min_val_t = VecMonDev[ii].curve.min_val_t;
		tmpMonDev.max_min_dif = VecMonDev[ii].curve.max_ampl;
		tmpMonDev.fact = VecMonDev[ii].mode.fact;
		tmpMonDev.clus = VecMonDev[ii].mode.group;

		// 		tmpMonDev.fact = VecMonDev[ii].mode.fact;
		tmpMonDev.is_lfo_source = VecMonDev[ii].mode.is_source;
		tmpMonDev.lead_gen_pow = VecMonDev[ii].mode.pow;
		//  		sprintf(m_LogString,"[yjj]修改LFO发电机表参与因子[%f],振幅[%f],是否扰动源[%d],最大值[%f]!",tmpMonDev.fact,tmpMonDev.lead_ampl,tmpMonDev.is_lfo_source,tmpMonDev.max_val);
		//  		MessShow(m_LogString,LTINFO);
		// 		tmpMonDev.max_ampl = VecMonDev[ii].curve.max_ampl;
		// 		tmpMonDev.max_ampl_rat = VecMonDev[ii].curve.max_ampl_rat;
				//tmpMonDev.is_ang_calc = VecMonDev[ii].is_ang_calc;
		//    		sprintf(m_LogString, "更新LFO发电机表[%s]状态[%d],功率振幅[%f],主导模式振幅[%f],参与因子[%f],所属群[%d]",VecMonDev[ii].dev.dev_name,VecMonDev[ii].stat,tmpMonDev.max_min_dif,tmpMonDev.lead_ampl,tmpMonDev.fact,tmpMonDev.fact);
		//    		MessShow(m_LogString);
		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, LFO_GN_TAB_MONWRITE_FLD, (const char*)&tmpMonDev.stat, sizeof(LFO_GN_TAB_MONWRITE), LFO_GN_NO_TAB);
	}
#endif
	// 	sprintf(m_LogString,"修改LFO发电机表[%d]结束!",LFO_GN_NO_TAB);
	// 	MessShow(m_LogString,LTINFO);
	return 0;
}

int CLfoAnaObj::UpdateMonGnDisp(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	int ii;
	unsigned char is_disp2;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN)
		{
			continue;
		}
		if (ii < m_LfoPara.disp_num)
		{
			is_disp2 = MENU_LFO_MENU_YES;
			sprintf(m_LogString, "["INT_REC_FMT"][%s]需要显示,功率波动[%f]", INT_REC_VAL(VecMonDev[ii].dev.dev_idx), VecMonDev[ii].dev.dev_name, VecMonDev[ii].curve.max_ampl);
			MessShow(m_LogString);
		}
		else
		{
			is_disp2 = MENU_LFO_MENU_NO;
		}
		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, "is_disp2", (const char*)&is_disp2, sizeof(is_disp2), LFO_GN_NO_TAB);
	}
#endif
	return 0;
}

int CLfoAnaObj::UpdateAnaGnTable(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	int ii;
	// 	int clus;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN)
		{
			continue;
		}
		// 		clus = VecMonDev[ii].mode.group;

		sprintf(m_LogString, "1写LFO发电机表第[%d]个设备[%s]所属群[%d],参与因子[%f]", ii, VecMonDev[ii].dev.dev_name, VecMonDev[ii].mode.group, VecMonDev[ii].mode.fact);
		MessShow(m_LogString);
		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, "clus,fact2", (const char*)&VecMonDev[ii].mode.group, sizeof(int) + sizeof(float), LFO_GN_NO_TAB);
	}
#endif
	// 	sprintf(m_LogString,"修改LFO发电机表[%d]结束!",LFO_GN_NO_TAB);
	// 	MessShow(m_LogString,LTINFO);
	return 0;
}


int CLfoAnaObj::UpdateMonLnTable(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	int ii;
	LFO_LN_TAB_MONWRITE tmpMonDev;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_LINE)
		{
			continue;
		}
		memset((void*)&tmpMonDev, 0, sizeof(LFO_LN_TAB_MONWRITE));
		tmpMonDev.stat = PODB_GetVarNsquare(2, VecMonDev[ii].stat - 1);
		tmpMonDev.stat_val = PODB_GetVarNsquare(2, VecMonDev[ii].stat - 1);
		tmpMonDev.lead_ampl = VecMonDev[ii].mode.ampl;
		tmpMonDev.lead_freq = VecMonDev[ii].mode.freq;
		tmpMonDev.lead_damp = VecMonDev[ii].mode.damp;
		tmpMonDev.lead_phas = VecMonDev[ii].mode.phas;
		tmpMonDev.lead_real = VecMonDev[ii].mode.real;
		tmpMonDev.lead_imag = VecMonDev[ii].mode.imag;
		tmpMonDev.lead_engy = VecMonDev[ii].mode.engy;
		tmpMonDev.lead_fail = VecMonDev[ii].mode.fail;
		tmpMonDev.aver_val = VecMonDev[ii].curve.aver_val;
		tmpMonDev.max_val = VecMonDev[ii].curve.max_val;
		tmpMonDev.max_val_t = VecMonDev[ii].curve.max_val_t;
		tmpMonDev.min_val = VecMonDev[ii].curve.min_val;
		tmpMonDev.min_val_t = VecMonDev[ii].curve.min_val_t;
		tmpMonDev.max_min_dif = VecMonDev[ii].curve.max_ampl;
		// 		tmpMonDev.max_ampl = VecMonDev[ii].curve.max_ampl;
		// 		tmpMonDev.max_ampl_rat = VecMonDev[ii].curve.max_ampl_rat;
				//tmpMonDev.slct_dot_id = VecMonDev[ii].slct_dot_id;

		tmpMonDev.istie_source = VecMonDev[ii].mode.is_source;
		tmpMonDev.lead_line_pow = VecMonDev[ii].mode.pow;
		tmpMonDev.fact = VecMonDev[ii].mode.fact;
		//  		sprintf(m_LogString,"更新LFO线路表[%s]状态[%d],振幅[%f]!",VecMonDev[ii].dev.dev_name,VecMonDev[ii].stat,tmpMonDev.max_min_dif);
		//  		MessShow(m_LogString,LTINFO);
		sprintf(tmpMonDev.yc_name, "%s", VecMonDev[ii].mode.yc_name);
		sprintf(tmpMonDev.in_or_out, "%s", VecMonDev[ii].mode.in_or_out);

		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, LFO_LN_TAB_MONWRITE_FLD, (const char*)&tmpMonDev.stat, sizeof(LFO_LN_TAB_MONWRITE), LFO_ACLN_NO_TAB);
	}
#endif
	return 0;
}

int CLfoAnaObj::UpdateMonLnDisp(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	int ii;
	int is_disp2;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_LINE)
		{
			continue;
		}
		if (ii < m_LfoPara.disp_num)
		{
			is_disp2 = MENU_LFO_MENU_YES;
			sprintf(m_LogString, "[%s]需要显示,功率波动[%f]", VecMonDev[ii].dev.dev_name, VecMonDev[ii].curve.max_ampl);
			MessShow(m_LogString);
		}
		else
		{
			is_disp2 = MENU_LFO_MENU_NO;
		}
		sprintf(m_LogString, "VecMonDev[ii].dev.dev_idx["INT_REC_FMT"]", INT_REC_VAL(VecMonDev[ii].dev.dev_idx));
		MessShow(m_LogString, LTERROR);
		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, "is_disp2", (const char*)&is_disp2, sizeof(is_disp2), LFO_ACLN_NO_TAB);
	}
#endif
	return 0;
}

int CLfoAnaObj::UpdateMonTrTable(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	int ii;
	LFO_TR_TAB_MONWRITE tmpMonDev;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_TR)
		{
			continue;
		}
		memset((void*)&tmpMonDev, 0, sizeof(LFO_TR_TAB_MONWRITE));
		tmpMonDev.stat = PODB_GetVarNsquare(2, VecMonDev[ii].stat - 1);
		tmpMonDev.stat_val = PODB_GetVarNsquare(2, VecMonDev[ii].stat - 1);
		tmpMonDev.lead_ampl = VecMonDev[ii].mode.ampl;
		tmpMonDev.lead_freq = VecMonDev[ii].mode.freq;
		tmpMonDev.lead_damp = VecMonDev[ii].mode.damp;
		tmpMonDev.lead_phas = VecMonDev[ii].mode.phas;
		tmpMonDev.lead_real = VecMonDev[ii].mode.real;
		tmpMonDev.lead_imag = VecMonDev[ii].mode.imag;
		tmpMonDev.lead_engy = VecMonDev[ii].mode.engy;
		tmpMonDev.lead_fail = VecMonDev[ii].mode.fail;
		tmpMonDev.aver_val = VecMonDev[ii].curve.aver_val;
		tmpMonDev.max_val = VecMonDev[ii].curve.max_val;
		tmpMonDev.max_val_t = VecMonDev[ii].curve.max_val_t;
		tmpMonDev.min_val = VecMonDev[ii].curve.min_val;
		tmpMonDev.min_val_t = VecMonDev[ii].curve.min_val_t;
		tmpMonDev.max_min_dif = VecMonDev[ii].curve.max_ampl;
		// 		tmpMonDev.max_ampl = VecMonDev[ii].curve.max_ampl;
		// 		tmpMonDev.max_ampl_rat = VecMonDev[ii].curve.max_ampl_rat;
				//tmpMonDev.slct_dot_id = VecMonDev[ii].slct_dot_id;
		//   		sprintf(m_LogString,"更新LFO变压器表[%s],idx=["INT_REC_FMT"],状态[%d],功率波动[%f]",VecMonDev[ii].dev.dev_name,INT_REC_VAL(VecMonDev[ii].dev.dev_idx),VecMonDev[ii].stat,tmpMonDev.max_min_dif);
		//   		MessShow(m_LogString,LTINFO);

		tmpMonDev.istie_source = VecMonDev[ii].mode.is_source;
		tmpMonDev.lead_tr_pow = VecMonDev[ii].mode.pow;
		tmpMonDev.fact = VecMonDev[ii].mode.fact;
		sprintf(tmpMonDev.in_or_out, "%s", VecMonDev[ii].mode.in_or_out);
		sprintf(tmpMonDev.yc_name, "%s", VecMonDev[ii].mode.yc_name);

		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, LFO_TR_TAB_MONWRITE_FLD, (const char*)&tmpMonDev.stat, sizeof(LFO_TR_TAB_MONWRITE), LFO_TR_NO_TAB);
	}

#endif
	return 0;
}

int CLfoAnaObj::UpdateMonTrDisp(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	int ii;
	int is_disp2;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_TR)
		{
			continue;
		}
		if (ii < m_LfoPara.disp_num)
		{
			is_disp2 = MENU_LFO_MENU_YES;
			sprintf(m_LogString, "[%s]需要显示,功率波动[%f]", VecMonDev[ii].dev.dev_name, VecMonDev[ii].curve.max_ampl);
			MessShow(m_LogString);
		}
		else
		{
			is_disp2 = MENU_LFO_MENU_NO;
		}
		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, "is_disp2", (const char*)&is_disp2, sizeof(is_disp2), LFO_TR_NO_TAB);
	}
#endif
	return 0;
}

int CLfoAnaObj::UpdateMonIntTable(vector<LFO_DEV_INFO>& VecMonDev)
{
	sprintf(m_LogString, "更新断面结果");
	MessShow(m_LogString, LTINFO);
#ifdef _WAMAP_
	int ii;
	LFO_INT_TAB_MONWRITE tmpMonDev;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_ITFC)
		{
			continue;
		}
		memset((void*)&tmpMonDev, 0, sizeof(LFO_INT_TAB_MONWRITE));
		tmpMonDev.stat2 = PODB_GetVarNsquare(2, VecMonDev[ii].stat - 1);
		tmpMonDev.stat_val2 = PODB_GetVarNsquare(2, VecMonDev[ii].stat - 1);
		tmpMonDev.lead_ampl2 = VecMonDev[ii].mode.ampl;
		tmpMonDev.lead_freq2 = VecMonDev[ii].mode.freq;
		tmpMonDev.lead_damp2 = VecMonDev[ii].mode.damp;
		tmpMonDev.lead_phas2 = VecMonDev[ii].mode.phas;
		if (VecMonDev[ii].is_sm_md == MENU_LFO_MENU_YES)
		{
			tmpMonDev.is_same_mod2 = MENU_LFO_MENU_YES;
		}
		else
		{
			tmpMonDev.is_same_mod2 = MENU_LFO_MENU_NO;
		}
		tmpMonDev.max_min_dif2 = VecMonDev[ii].curve.max_ampl;
		// 		sprintf(tmpMonDev.intline_des2,"%s",VecMonDev[ii].intline_des);
		sprintf(m_LogString, "[%s]是否同调=%d,振幅[%f],最大摆动[%f]", VecMonDev[ii].dev.dev_name, tmpMonDev.is_same_mod2, tmpMonDev.lead_ampl2, tmpMonDev.max_min_dif2);
		MessShow(m_LogString, LTINFO);

		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, LFO_INT_TAB_MONWRITE_FLD, (const char*)&tmpMonDev.stat2, sizeof(LFO_INT_TAB_MONWRITE), LFO_INTFACE_NO_TAB);
	}

#endif
	return 0;

}

int CLfoAnaObj::UpdateMonIntDisp(vector<LFO_DEV_INFO>& VecMonDev)
{
#ifdef _WAMAP_
	int ii;
	int is_disp2;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_ITFC)
		{
			continue;
		}
		if (ii < m_LfoPara.disp_num)
		{
			is_disp2 = MENU_LFO_MENU_YES;
			sprintf(m_LogString, "[%s]需要显示,功率波动[%f]", VecMonDev[ii].dev.dev_name, VecMonDev[ii].curve.max_ampl);
			MessShow(m_LogString);
		}
		else
		{
			is_disp2 = MENU_LFO_MENU_NO;
		}
		PODB_TableModifyByKey((const char*)&VecMonDev[ii].dev.dev_idx, "is_disp2", (const char*)&is_disp2, sizeof(is_disp2), LFO_INTFACE_NO_TAB);
	}
#endif
	return 0;
}

//多模式入库
int CLfoAnaObj::UpdateWDSTable(LFO_CASE_INFO& LfoCase)
{
	if (LfoCase.vec_mainmode.size() <= 0)
	{
		sprintf(m_LogString, "本轮没有模式");
		MessShow(m_LogString);
		return 0;
	}
	int ii, jj;
	float freq_disp = 0.0;
	LPO_ALL_MODE_WAMS_WAMS tmpAllmode;
	LPO_ALLDEV_MODE_WAMS_WAMS tmpDevmode;
	vector<LPO_ALL_MODE_WAMS_WAMS> tmpVecAllmode;
	vector<LPO_ALLDEV_MODE_WAMS_WAMS> tmpVecDevmode;
	sprintf(m_LogString, "本轮模式数[%d]", LfoCase.vec_mainmode.size());
	MessShow(m_LogString);
	for (ii = 0; ii < LfoCase.vec_mainmode.size(); ii++)
	{
		memset((void*)&tmpAllmode, 0, sizeof(LPO_ALL_MODE_WAMS_WAMS));
		tmpAllmode.record_id = PODB_GetDevIdByKeyID(LPO_ALL_MODE_WAMS_NO, tmpVecAllmode.size() + 1);
		if (ii == 0)
		{
			tmpAllmode.is_show = MENU_LFO_MENU_YES;
		}
		tmpAllmode.freq = LfoCase.vec_mainmode[ii].mode.freq;
		tmpAllmode.ampl = LfoCase.vec_mainmode[ii].mode.ampl;
		if (tmpAllmode.freq < m_LfoPara.lfo_freq_min)
		{
			sprintf(tmpAllmode.mode_des, "%s", "超低频振荡");
		}
		else if (tmpAllmode.freq > m_LfoPara.lfo_freq_max)
		{
			sprintf(tmpAllmode.mode_des, "%s", "高频振荡");
		}
		else
		{
			sprintf(tmpAllmode.mode_des, "%s", "低频振荡");
		}
		tmpAllmode.damp = LfoCase.vec_mainmode[ii].mode.damp;
		tmpAllmode.lfo_bgn = LfoCase.vec_mainmode[ii].mode.start_time;
		tmpAllmode.lfo_end = LfoCase.vec_mainmode[ii].mode.end_time;
		tmpAllmode.is_lfo_source = LfoCase.vec_mainmode[ii].is_lfo_source;
		sprintf(tmpAllmode.lfo_ana_des, "%s", LfoCase.vec_mainmode[ii].lfo_ana_des);
		if (tmpAllmode.is_lfo_source == 1)
		{
			sprintf(tmpAllmode.is_source_des, "√");
		}
		else
		{
			sprintf(tmpAllmode.is_source_des, "×");
		}
		sprintf(tmpAllmode.lfo_time, "%s", PT_GetStringOfTimeInterval((tmpAllmode.lfo_end - tmpAllmode.lfo_bgn) * 1000));
		for (jj = 0; jj < LfoCase.vec_mainmode[ii].vec_dev.size(); jj++)
		{
			memset((void*)&tmpDevmode, 0, sizeof(LPO_ALLDEV_MODE_WAMS_WAMS));
			tmpDevmode.record_id = PODB_GetDevIdByKeyID(LPO_ALLDEV_MODE_WAMS_NO, tmpVecDevmode.size() + 1);
			tmpDevmode.freq = LfoCase.vec_mainmode[ii].vec_dev[jj].mode.freq;
			tmpDevmode.ampl = LfoCase.vec_mainmode[ii].vec_dev[jj].mode.ampl;
			tmpDevmode.damp = LfoCase.vec_mainmode[ii].vec_dev[jj].mode.damp;
			tmpDevmode.mode_id = tmpAllmode.record_id;
			tmpDevmode.dev_type = LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_type;
			tmpDevmode.dev_id = LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_id;
			tmpDevmode.is_source = LfoCase.vec_mainmode[ii].vec_dev[jj].mode.is_source;
			sprintf(tmpDevmode.dev_name, "%s", LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_name);
			sprintf(tmpDevmode.yc_name, "%s", LfoCase.vec_mainmode[ii].vec_dev[jj].mode.yc_name);
			if (tmpDevmode.dev_type == MENU_LFO_DEV_GEN)
			{
				tmpAllmode.gn_num++;
			}
			else if (tmpDevmode.dev_type == MENU_LFO_DEV_LINE)
			{
				tmpAllmode.line_num++;
			}
			else if (tmpDevmode.dev_type == MENU_LFO_DEV_TR)
			{
				tmpAllmode.tr_mun++;
			}
			if (ii == 0)
			{
				tmpDevmode.is_show = MENU_LFO_MENU_YES;
			}
			else
			{
				tmpDevmode.is_show = MENU_LFO_MENU_NO;
			}
			tmpVecDevmode.push_back(tmpDevmode);
		}
		tmpVecAllmode.push_back(tmpAllmode);
	}
	// 清空原表
	PODB_TableClear(LPO_ALL_MODE_WAMS_NO);
	int ret_code = PODB_TableWrite((const char*)&tmpVecAllmode[0].record_id, tmpVecAllmode.size() * sizeof(LPO_ALL_MODE_WAMS_WAMS), LPO_ALL_MODE_WAMS_NO);
	vector<LPO_ALL_MODE_WAMS_WAMS>().swap(tmpVecAllmode);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "写LPO实时模式全表[%d]失败", LPO_ALL_MODE_WAMS_NO);
		MessShow(m_LogString);
		return -1;
	}
	// 清空原表
	PODB_TableClear(LPO_ALLDEV_MODE_WAMS_NO);
	ret_code = PODB_TableWrite((const char*)&tmpVecDevmode[0].record_id, tmpVecDevmode.size() * sizeof(LPO_ALLDEV_MODE_WAMS_WAMS), LPO_ALLDEV_MODE_WAMS_NO);
	vector<LPO_ALLDEV_MODE_WAMS_WAMS>().swap(tmpVecDevmode);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "写LPO实时设备模式全表[%d]失败", LPO_ALLDEV_MODE_WAMS_NO);
		MessShow(m_LogString);
		return -1;
	}

	return 0;

}


int CLfoAnaObj::SaveLfoResult(LFO_CASE_INFO& LfoCase)
{

	if (UpdateLfoFcTable(LfoCase, LFO_SAVE_FLAG_HOLD) < 0)
	{
		sprintf(m_LogString, "更新LFO厂站表错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("UpdateLfoFcTable");
	if (UpdateLfoGnTable(LfoCase, LFO_SAVE_FLAG_HOLD) < 0)
	{
		sprintf(m_LogString, "更新LFO发电机表错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("UpdateLfoGnTable");

	if (UpdateLfoLnTable(LfoCase, LFO_SAVE_FLAG_HOLD) < 0)
	{
		sprintf(m_LogString, "更新LFO交流线段表错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("UpdateLfoLnTable");

	if (UpdateLfoTrTable(LfoCase, LFO_SAVE_FLAG_HOLD) < 0)
	{
		sprintf(m_LogString, "更新LFO变压器表错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("UpdateLfoTrTable");

	if (UpdateLfoIntTable(LfoCase, LFO_SAVE_FLAG_HOLD) < 0)
	{
		sprintf(m_LogString, "更新LFO联络线走廊表错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("UpdateLfoIntTable");

	if (UpdateLfoParaTable(LfoCase) < 0)
	{
		sprintf(m_LogString, "更新LFO参数表错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("UpdateLfoParaTable");
	//         sprintf(m_LogString,"[yjj]更新LFO振荡事件表");
	//         MessShow(m_LogString,LTERROR);	
	if (UpdateLfoCaseTable(LfoCase) < 0)
	{
		sprintf(m_LogString, "更新LFO振荡事件表错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("UpdateLfoCaseTable");

	//	sprintf(m_LogString,"[yjj]更新LFO振荡事件表结束");
	//         MessShow(m_LogString,LTERROR);		
	if (UpdateLfoWindowsTable(LfoCase) < 0)
	{
		sprintf(m_LogString, "更新LPO功率波动窗口表错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	ShowProMem("UpdateLfoWindowsTable");
	// 	
	//     if (UpdateLfoModeShape(LfoCase)<0)
	//     {
	//         sprintf(m_LogString,"更新LFO实时CASE模态全表错误");
	//         MessShow(m_LogString,LTERROR);
	//         return -1;
	//     }
	return 0;
}

int CLfoAnaObj::UpdateLfoMonPara(LFO_SYS_INFO& MonSys)
{
#ifdef _WAMAP_
	LFO_PARA_TABLE_MONWRITE tmpLfoParaMon;
	int warn_mode = 0;
	int warn_type = 0;
	memset((void*)&tmpLfoParaMon, 0, sizeof(LFO_PARA_TABLE_MONWRITE));
	tmpLfoParaMon.cur_time = MonSys.start_time;
	tmpLfoParaMon.stat = MonSys.stat;
	if (tmpLfoParaMon.stat < MENU_LFO_DEV_STS_SCT)
	{
		tmpLfoParaMon.stat_val = 1;
	}
	if (tmpLfoParaMon.stat == MENU_LFO_DEV_STS_SCT)
	{
		tmpLfoParaMon.stat_val = 2;
	}
	if (tmpLfoParaMon.stat > MENU_LFO_DEV_STS_FWN)
	{
		tmpLfoParaMon.stat_val = 3;
	}
	// 	tmpLfoParaMon.stat_val = PODB_GetVarNsquare(2,MonSys.stat-1);
	sprintf(tmpLfoParaMon.stat_ctx, "%s", MonSys.stat_desc);
	if (tmpLfoParaMon.stat >= MENU_LFO_DEV_STS_WRN)
	{
		// 		if ((MonSys.lfos_gn_num + MonSys.lfos_ln_num + MonSys.lfos_tr_num) > 0)
		// 		{
		// 			tmpLfoParaMon.is_lfo_source2 = MENU_LFO_MENU_YES;
		sprintf(tmpLfoParaMon.lfos_des, "%s", MonSys.lfos_des);
		// 		}
		// 		else
		// 		{
		// 			tmpLfoParaMon.is_lfo_source2 = MENU_LFO_MENU_NO;
		// 		}
		// 		if (!MonSys.vec_mainmode.empty())
		// 		{
		// 			for (int ii = 0; ii < MonSys.vec_mainmode.size();ii++)
		// 			{
		// 				if (MonSys.vec_mainmode[ii].mode.stat >= MENU_LFO_DEV_STS_WRN)
		// 				{
		// 					warn_mode++;
		// 					if (MonSys.vec_mainmode[ii].mode.freq < m_LfoPara.lfo_freq_min)
		// 					{
		// 						warn_type += PODB_GetVarNsquare(2,MENU_LFO_SLOW-1);
		// 					}
		// 					else if (MonSys.vec_mainmode[ii].mode.freq > m_LfoPara.lfo_freq_max)
		// 					{
		// 						warn_type += PODB_GetVarNsquare(2,MENU_LFO_HIGH-1);
		// 					}
		// 					else if (MonSys.vec_mainmode[ii].mode.freq >= m_LfoPara.lfo_freq_min && MonSys.vec_mainmode[ii].mode.freq <= m_LfoPara.lfo_freq_max)
		// 					{
		// 						warn_type += PODB_GetVarNsquare(2,MENU_LFO_LOW-1);
		// 					}
		// 				}
		// 			}
		// 		}
		// 		if (warn_mode > 1)
		// 		{
		// 			tmpLfoParaMon.lfo_modle2 = MENU_LFO_MULTI;
		// 		}
		// 		else if (warn_mode == 1)
		// 		{
		// 			tmpLfoParaMon.lfo_modle2 = MENU_LFO_SINGLE;
		// 		}
		// 		else if (warn_mode < 1)
		// 		{
		// 			tmpLfoParaMon.lfo_modle2 = 0;
		// 		}
		// 		tmpLfoParaMon.lfo_type2 = warn_type;
		// 		sprintf(tmpLfoParaMon.lfo_ana_des2,"%s",MonSys.center_desc);
	}
	// 	else
	// 	{
	// 		tmpLfoParaMon.is_lfo_source2 = 0;
	// 		tmpLfoParaMon.lfo_type2 = 0;
	// 		tmpLfoParaMon.lfo_modle2 = 0;
	// 	}
	if (tmpLfoParaMon.stat == 0)
	{
		tmpLfoParaMon.stat = MENU_LFO_DEV_STS_SCT;
	}

	INT_REC record_id = PODB_GetDevIdByKeyID(LFO_PARA_NO_TAB, 2);
	sprintf(m_LogString, "获取LFO参数表第2条记录的记录号为["INT_REC_FMT"],系统状态[%d],振荡源分析[%s]]", INT_REC_VAL(record_id), MonSys.stat, tmpLfoParaMon.lfos_des);
	MessShow(m_LogString, LTINFO);
	PODB_TableModifyByKey((const char*)&record_id, LFO_PARA_TABLE_MONWRITE_FLD, (const char*)&tmpLfoParaMon, sizeof(LFO_PARA_TABLE_MONWRITE), LFO_PARA_NO_TAB);
#endif
	return 0;
}
#endif

int CLfoAnaObj::SaveLfoCurve(vector<LFO_DEV_INFO>& VecMonDev, LFO_SYS_INFO& MonSys)
{
#ifdef _WAMAP_
	int ii, jj;
	int ordernum = 2;
	int nSample = 0, nNum = 0;
	LFO_DEV_INFO tmpLfoGen, tmpLfoLine, tmpLfoTr, tmpLfoInf, tmpLfoTime;
	vector<LFO_DEV_INFO> tmpvecGen, tmpvecLine, tmpvecInf, tmpvecTr, tmpvecIntLine;
	float tmpfreq[CURVE_NUM_PER_SEC * CURVE_TIME];
	float tmpdamp[CURVE_NUM_PER_SEC * CURVE_TIME];

	ClearLfoDev(tmpLfoGen);
	sprintf(tmpLfoGen.dev.dev_name, "%s", "未匹配");
	tmpLfoGen.dev.dev_type = MENU_LFO_DEV_GEN;
	ClearLfoDev(tmpLfoLine);
	sprintf(tmpLfoLine.dev.dev_name, "%s", "未匹配");
	tmpLfoLine.dev.dev_type = MENU_LFO_DEV_LINE;
	ClearLfoDev(tmpLfoInf);
	sprintf(tmpLfoInf.dev.dev_name, "%s", "未匹配");
	tmpLfoInf.dev.dev_type = MENU_LFO_DEV_ITFC;
	ClearLfoDev(tmpLfoTr);
	sprintf(tmpLfoTr.dev.dev_name, "%s", "未匹配");
	tmpLfoTr.dev.dev_type = MENU_LFO_DEV_TR;
	ClearLfoDev(tmpLfoTime);
	sprintf(tmpLfoTime.dev.dev_name, "%s", "功率振荡曲线时间");
	tmpLfoTime.dev.dev_type = 999;
	// 时间轴
// 	if (m_LfoPara.curve_time_60 == 1)
// 	{
// 		tmpLfoTime.start_time = MonSys.end_time - 60;
// 		tmpLfoGen.start_time = MonSys.end_time - 60;
// 		tmpLfoLine.start_time = MonSys.end_time - 60;
// 		tmpLfoTr.start_time = MonSys.end_time - 60;
// 		tmpLfoInf.start_time = MonSys.end_time - 60;
// 	} 
// 	else
// 	{
// 		tmpLfoTime.start_time = MonSys.end_time - 20;
// 		tmpLfoGen.start_time = MonSys.end_time - 20;
// 		tmpLfoLine.start_time = MonSys.end_time - 20;
// 		tmpLfoTr.start_time = MonSys.end_time - 20;
// 		tmpLfoInf.start_time = MonSys.end_time - 20;
// 	}
	tmpLfoTime.start_time = MonSys.end_time - CURVE_TIME;
	tmpLfoGen.start_time = MonSys.end_time - CURVE_TIME;
	tmpLfoLine.start_time = MonSys.end_time - CURVE_TIME;
	tmpLfoTr.start_time = MonSys.end_time - CURVE_TIME;
	tmpLfoInf.start_time = MonSys.end_time - CURVE_TIME;
	tmpLfoTime.end_time = MonSys.end_time;
	tmpLfoGen.end_time = MonSys.end_time;
	tmpLfoLine.end_time = MonSys.end_time;
	tmpLfoTr.end_time = MonSys.end_time;
	tmpLfoInf.end_time = MonSys.end_time;
	int time_leng = tmpLfoTime.end_time - tmpLfoTime.start_time;
	float dtime = float(CURVE_NUM_PER_SEC);
	dtime = float(1 / dtime);
	sprintf(m_LogString, "start_time = "RTDB_TIME_FMT",end_time = "RTDB_TIME_FMT",dtime = %f", tmpLfoTime.start_time, tmpLfoTime.end_time, dtime);
	MessShow(m_LogString, LTINFO);
	for (ii = 0; ii < CURVE_TIME * CURVE_NUM_PER_SEC; ii++)
	{//60*25
		tmpLfoTime.v2d_data.push_back(dtime * (ii + 1));
		// 		sprintf(m_LogString,"数据[%d] = %f",ii+1,dtime*(ii+1));
		// 		MessShow(m_LogString,LTINFO);
	}

	// 确定显示设备
	// 显示告警设备曲线
	// 没有告警设备，显示指定设备曲线
	// 没有指定设备，显示波动幅值最大的设备曲线
	// 暂只显示波动最大设备
	// 统计波动最大的三个设备
	ClearLfoDev(tmpvecGen);
	ClearLfoDev(tmpvecLine);
	ClearLfoDev(tmpvecTr);
	ClearLfoDev(tmpvecInf);
	ClearLfoDev(tmpvecIntLine);
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		// 		if (VecMonDev[ii].stat < MENU_LFO_DEV_STS_SCT)
		// 		{
		// 			continue;
		// 		}
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
		{
			tmpvecGen.push_back(VecMonDev[ii]);
			if (VecMonDev[ii].curve.max_ampl >= tmpLfoGen.curve.max_ampl)
			{
				tmpLfoGen = VecMonDev[ii];
			}
		}
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
		{
			tmpvecLine.push_back(VecMonDev[ii]);
			if (VecMonDev[ii].curve.max_ampl >= tmpLfoLine.curve.max_ampl)
			{
				tmpLfoLine = VecMonDev[ii];
			}
		}
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
		{
			tmpvecTr.push_back(VecMonDev[ii]);
			if (VecMonDev[ii].curve.max_ampl >= tmpLfoTr.curve.max_ampl)
			{
				tmpLfoTr = VecMonDev[ii];
			}
		}
		if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_ITFC)
		{
			tmpvecInf.push_back(VecMonDev[ii]);
			if (VecMonDev[ii].curve.max_ampl >= tmpLfoInf.curve.max_ampl)
			{
				tmpLfoInf = VecMonDev[ii];
			}
		}
	}
	sprintf(m_LogString, "支路总数[%d]", m_VecIntline.size());
	MessShow(m_LogString, LTINFO);
	for (int ii = 0; ii < m_VecIntline.size(); ii++)
	{
		sprintf(m_LogString, "断面id["INT_REC_FMT"],支路ID["INT_REC_FMT"]", INT_REC_VAL(tmpLfoInf.dev.dev_id), INT_REC_VAL(m_VecIntline[ii].line_no));
		MessShow(m_LogString, LTINFO);
		if (tmpLfoInf.dev.dev_id == m_VecIntline[ii].inf_no)
		{
			tmpvecIntLine.push_back(m_VecIntline[ii].dev_info);
		}
	}
	sort(tmpvecGen.begin(), tmpvecGen.end(), g_SortDevByMaxampl);
	sort(tmpvecLine.begin(), tmpvecLine.end(), g_SortDevByMaxampl);
	sort(tmpvecTr.begin(), tmpvecTr.end(), g_SortDevByMaxampl);
	sort(tmpvecInf.begin(), tmpvecInf.end(), g_SortDevByMaxampl);




	sprintf(m_LogString, "曲线:发电机[%s]["INT_REC_FMT"][%d]交流线路[%s]["INT_REC_FMT"][%d]变压器[%s]["INT_REC_FMT"][%d]time["RTDB_TIME_FMT"~"RTDB_TIME_FMT"]", tmpLfoGen.dev.dev_name, INT_REC_VAL(tmpLfoGen.dev.dev_id), tmpLfoGen.stat,
		tmpLfoLine.dev.dev_name, INT_REC_VAL(tmpLfoLine.dev.dev_id), tmpLfoLine.stat, tmpLfoTr.dev.dev_name, INT_REC_VAL(tmpLfoTr.dev.dev_id), tmpLfoTr.stat, MonSys.start_time, MonSys.end_time);
	MessShow(m_LogString, LTINFO);

	// 获取设备数据
	if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
	{
		tmpLfoGen.stat = MENU_LFO_DEV_STS_OK;
		tmpLfoLine.stat = MENU_LFO_DEV_STS_OK;
		tmpLfoTr.stat = MENU_LFO_DEV_STS_OK;
		GetDevDataFromDB(MonSys.start_time, MonSys.end_time, ordernum, tmpLfoGen);
		GetDevDataFromDB(MonSys.start_time, MonSys.end_time, ordernum, tmpLfoLine);
		GetDevDataFromDB(MonSys.start_time, MonSys.end_time, ordernum, tmpLfoTr);
		for (ii = 0; ii < tmpvecIntLine.size(); ii++)
		{
			GetDevDataFromDB(MonSys.start_time, MonSys.end_time, ordernum, tmpvecIntLine[ii]);

			for (jj = 0; jj < tmpvecIntLine[ii].v2d_data.size(); jj++)
			{
				if (jj >= tmpLfoInf.v2d_data.size())
				{
					tmpLfoInf.v2d_data.push_back(fabs(tmpvecIntLine[ii].v2d_data[jj]));
				}
				else
				{
					tmpLfoInf.v2d_data[jj] += fabs(tmpvecIntLine[ii].v2d_data[jj]);
				}
			}
		}
		nSample = (int)(MAX_FRA_PER_SEC / CURVE_NUM_PER_SEC + 0.5);
	}
	else
	{
		// 从数据文件中读取PMU数据
// 		ShowProMem("刷新曲线读文件");
// 		sprintf(m_LogString,"读取数据文件[%s],起始[%d],窗口[%d],交叠[%d]",m_LfoPara.datafile_name,m_LfoPara.data_pos_old,m_LfoPara.time_leng,m_LfoPara.int_time);
// 		MessShow(m_LogString,LTINFO);

		CDataFile df;
		int pos = -1;
		df.ClearData();
		if (tmpLfoTime.end_time - m_LfoPara.file_start_time <= 60)
		{//本轮结束时间-案例开始时间<60 读取从案例开头到本轮结束时间的数据
			pos = -1;
			tmpLfoTime.start_time = m_LfoPara.file_start_time;
			sprintf(m_LogString, "未超过60秒,读取数据文件[%s],起始[%d],窗口["RTDB_TIME_FMT"],交叠[0]", m_LfoPara.datafile_name, pos, tmpLfoTime.end_time - m_LfoPara.file_start_time);
			MessShow(m_LogString, LTINFO);
			if (df.ReadFileData(m_LfoPara.datafile_name, pos, (tmpLfoTime.end_time - m_LfoPara.file_start_time), 0) == false)
			{
				sprintf(m_LogString, "读取数据文件失败[%s][%s]", m_LfoPara.datafile_name, df.GetLastError());
				MessShow(m_LogString, LTERROR);
				df.ClearData();
				return -1;
			}
		}
		else
		{//本轮结束时间-案例开始时间>60 读取本轮结束时间前60秒的数据
			//获取本轮结束前60秒对应的文件位置
			for (int ii = 0; ii < g_vec_pos_time.size(); ii++)
			{
				if (g_vec_pos_time[ii].start_time == (tmpLfoTime.end_time - 60))
				{
					pos = g_vec_pos_time[ii].pos;
					break;
				}
			}
			sprintf(m_LogString, "已超过60秒,读取数据文件[%s],起始[%d],窗口[60],交叠[0]", m_LfoPara.datafile_name, pos, tmpLfoTime.end_time - m_LfoPara.file_start_time);
			MessShow(m_LogString, LTINFO);
			if (df.ReadFileData(m_LfoPara.datafile_name, pos, 60, 0) == false)
			{
				sprintf(m_LogString, "读取数据文件失败[%s][%s]", m_LfoPara.datafile_name, df.GetLastError());
				MessShow(m_LogString, LTERROR);
				df.ClearData();
				return -1;
			}
		}

		sprintf(m_LogString, "曲线数据读取完毕");
		MessShow(m_LogString, LTINFO);
		nSample = (int)(df.m_DataHead.time_sample / CURVE_NUM_PER_SEC + 0.5);

		// 		if (m_LfoPara.curve_time_60 == 1)
		// 		{
		// 			if (df.ReadFileData(m_LfoPara.datafile_name,m_LfoPara.data_pos_old,60,50) == false)
		// 			{
		// 				sprintf(m_LogString,"读取数据文件失败[%s][%s]",m_LfoPara.datafile_name,df.GetLastError());
		// 				MessShow(m_LogString,LTERROR);
		// 				df.ClearData();
		// 				return -1;
		// 			}
		// 		} 
		// 		else
		// 		{
		// 			if (df.ReadFileData(m_LfoPara.datafile_name,m_LfoPara.data_pos_old,m_LfoPara.time_leng,m_LfoPara.int_time) == false)
		// 			{
		// 				sprintf(m_LogString,"读取数据文件失败[%s][%s]",m_LfoPara.datafile_name,df.GetLastError());
		// 				MessShow(m_LogString,LTERROR);
		// 				df.ClearData();
		// 				return -1;
		// 			}
		// 		}
		// 		ShowProMem("刷新曲线文件后");
		int dii, djj, tableno, nColNum;
		string::size_type idx;
		string tmpdevname, tmpColName;
		string tmpstring;

		if (VALID_INT_REC(tmpLfoGen.dev.dev_id))	
		{
			//修改读取数据模块
			for (djj = 0; djj < (int)df.m_DataHead.vec_devid_s.size(); djj++)
			{
				// 获取设备ID
// 				tmpstring = df.m_DataHead.vec_devid_s[djj];
				INT_REC tmpdev_id = df.m_DataHead.vec_devid_s[djj];
				if (tmpdev_id == tmpLfoGen.dev.dev_id)
				{
					// 去掉前两列"日期时间,毫秒"
					//				tmpstring.clear();
					if (df.GetNumberOfVariables() >= djj + 2)
					{
						df.GetVariableName(djj + 2, tmpstring);
					}
					idx = tmpstring.find_last_of("-");
					// 获取设备名称
					if (idx != string::npos)
					{
						tmpdevname = tmpstring.substr(0, idx);
					}
					// 判断测点类型
					tmpColName = tmpstring.substr(idx + 1);
					// 去掉每行末尾的","
					idx = tmpColName.find_first_of(",");
					if (idx != string::npos)
					{
						tmpColName = tmpColName.substr(0, idx);
					}
					if (tmpColName == "00P")
					{
						// 有功
						nColNum = 2;
					}
					else if (tmpColName == "00Q")
					{
						// 无功
						nColNum = 3;
					}
					else if (tmpColName == "UAV相角")
					{
						// 电压相角
						nColNum = 1;
					}
					else if (tmpColName == "UAV幅值")
					{
						// 电压幅值
						nColNum = 0;
					}
					else if (tmpColName == "EEV相角")
					{
						// 发电机功角
						nColNum = 16;
					}
					else
					{
						// 未定义，默认为无效
						nColNum = -1;
					}

					if (nColNum == ordernum)
					{
						// 获取设备数据
						if (df.GetData(djj + 2, tmpLfoGen.v2d_data) == -1)
						{
							sprintf(m_LogString, "[%s]读取数据错误[%s]",
								tmpstring.c_str(), df.GetLastError());
							MessShow(m_LogString, LTDEBUG);
							break;
						}
						else
						{
							break;
						}
					}
				}
			}
			// 			if( df.GetData(tmpLfoGen.dev.yc_name[12], tmpLfoGen.v2d_data) == -1)
			// 			{
			// 				// 置数据状态
			// 				tmpLfoGen.stat = MENU_LFO_DEV_STS_BAD;
			// 				sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
			// 					tmpLfoGen.dev.dev_name,tmpLfoGen.dev.dev_id,tmpLfoGen.dev.yc_name[12],df.GetLastError());
			// 				MessShow(m_LogString,LTERROR);
			// 			}
		}

		// 		ShowProMem("发电机");
		if (VALID_INT_REC(tmpLfoLine.dev.dev_id))	
		{
			//修改读取数据模块
			for (djj = 0; djj < df.m_DataHead.vec_devid_s.size(); djj++)
			{
				// 获取设备ID
// 				tmpstring = df.m_DataHead.vec_devid_s[djj];
				INT_REC tmpdev_id = df.m_DataHead.vec_devid_s[djj];
				if (tmpdev_id == tmpLfoLine.dev.dev_id)
				{
					// 去掉前两列"日期时间,毫秒"
					//				tmpstring.clear();
					if (df.GetNumberOfVariables() >= djj + 2)
					{
						df.GetVariableName(djj + 2, tmpstring);
					}
					// 					sprintf(m_LogString,"找到线路曲线[%s]",tmpstring.c_str());
					// 					MessShow(m_LogString);

					idx = tmpstring.find_last_of("-");
					// 获取设备名称
					if (idx != string::npos)
					{
						tmpdevname = tmpstring.substr(0, idx);
					}
					// 判断测点类型
					tmpColName = tmpstring.substr(idx + 1);
					// 去掉每行末尾的","
					idx = tmpColName.find_first_of(",");
					if (idx != string::npos)
					{
						tmpColName = tmpColName.substr(0, idx);
					}
					if (tmpColName == "00P")
					{
						// 有功
						nColNum = 2;
					}
					else if (tmpColName == "00Q")
					{
						// 无功
						nColNum = 3;
					}
					else if (tmpColName == "UAV相角")
					{
						// 电压相角
						nColNum = 1;
					}
					else if (tmpColName == "UAV幅值")
					{
						// 电压幅值
						nColNum = 0;
					}
					else if (tmpColName == "EEV相角")
					{
						// 发电机功角
						nColNum = 16;
					}
					else
					{
						// 未定义，默认为无效
						nColNum = -1;
					}

					if (nColNum == ordernum)
					{
						// 获取设备数据
						if (df.GetData(djj + 2, tmpLfoLine.v2d_data) == -1)
						{
							sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
								tmpstring.c_str(), INT_REC_VAL(tmpdev_id), df.GetLastError());
							MessShow(m_LogString);
							break;
						}
						else
						{
							break;
						}
					}
				}
			}
			// 			if( df.GetData(tmpLfoLine.dev.yc_name[12], tmpLfoLine.v2d_data) == -1)
			// 			{
			// 				// 置数据状态
			// 				tmpLfoLine.stat = MENU_LFO_DEV_STS_BAD;
			// 				sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
			// 					tmpLfoLine.dev.dev_name,tmpLfoLine.dev.dev_id,tmpLfoLine.dev.yc_name[12],df.GetLastError());
			// 				MessShow(m_LogString,LTERROR);
			// 			}
		}
		// 		ShowProMem("线路");
		if (VALID_INT_REC(tmpLfoTr.dev.dev_id))	
		{
			//修改读取数据模块
			for (djj = 0; djj < (int)df.m_DataHead.vec_devid_s.size(); djj++)
			{
				// 获取设备ID
// 				tmpstring = df.m_DataHead.vec_devid_s[djj];
				INT_REC tmpdev_id = df.m_DataHead.vec_devid_s[djj];
				if (tmpdev_id == tmpLfoTr.dev.dev_id)
				{
					// 去掉前两列"日期时间,毫秒"
					//				tmpstring.clear();
					if (df.GetNumberOfVariables() >= djj + 2)
					{
						df.GetVariableName(djj + 2, tmpstring);
					}
					idx = tmpstring.find_last_of("-");
					// 获取设备名称
					if (idx != string::npos)
					{
						tmpdevname = tmpstring.substr(0, idx);
					}
					// 判断测点类型
					tmpColName = tmpstring.substr(idx + 1);
					// 去掉每行末尾的","
					idx = tmpColName.find_first_of(",");
					if (idx != string::npos)
					{
						tmpColName = tmpColName.substr(0, idx);
					}
					if (tmpColName == "00P")
					{
						// 有功
						nColNum = 2;
					}
					else if (tmpColName == "00Q")
					{
						// 无功
						nColNum = 3;
					}
					else if (tmpColName == "UAV相角")
					{
						// 电压相角
						nColNum = 1;
					}
					else if (tmpColName == "UAV幅值")
					{
						// 电压幅值
						nColNum = 0;
					}
					else if (tmpColName == "EEV相角")
					{
						// 发电机功角
						nColNum = 16;
					}
					else
					{
						// 未定义，默认为无效
						nColNum = -1;
					}

					if (nColNum == ordernum)
					{
						// 获取设备数据
						if (df.GetData(djj + 2, tmpLfoTr.v2d_data) == -1)
						{
							sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
								tmpstring.c_str(), INT_REC_VAL(tmpdev_id), df.GetLastError());
							MessShow(m_LogString, LTDEBUG);
							break;
						}
						else
						{
							break;
						}
					}
				}
			}
			// 			if( df.GetData(tmpLfoTr.dev.yc_name[12], tmpLfoTr.v2d_data) == -1)
			// 			{
			// 				// 置数据状态
			// 				tmpLfoTr.stat = MENU_LFO_DEV_STS_BAD;
			// 				sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
			// 					tmpLfoTr.dev.dev_name,tmpLfoTr.dev.dev_id,tmpLfoTr.dev.yc_name[12],df.GetLastError());
			// 				MessShow(m_LogString,LTERROR);
			// 			}
		}
		// 		ShowProMem("变压器");
		if (VALID_INT_REC(tmpLfoInf.dev.dev_id))		
		{
			sprintf(m_LogString, "获取曲线显示断面数据,断面组成支路数%d", tmpvecIntLine.size());
			MessShow(m_LogString);
			for (int ii = 0; ii < tmpvecIntLine.size(); ii++)
			{
				tmpvecIntLine[ii].v2d_data.clear();
				sprintf(m_LogString, "找到断面组成支路[%s]", tmpvecIntLine[ii].dev.dev_name);
				MessShow(m_LogString);
				for (djj = 0; djj < df.m_DataHead.vec_devid_s.size(); djj++)
				{
					// 获取设备ID
// 					tmpstring = df.m_DataHead.vec_devid_s[djj];
					INT_REC tmpdev_id = df.m_DataHead.vec_devid_s[djj];
					if (tmpdev_id == tmpvecIntLine[ii].dev.dev_id)
					{
						// 去掉前两列"日期时间,毫秒"
						//				tmpstring.clear();
						if (df.GetNumberOfVariables() >= djj + 2)
						{
							df.GetVariableName(djj + 2, tmpstring);
						}
						idx = tmpstring.find_last_of("-");
						// 获取设备名称
						if (idx != string::npos)
						{
							tmpdevname = tmpstring.substr(0, idx);
						}
						// 判断测点类型
						tmpColName = tmpstring.substr(idx + 1);
						// 去掉每行末尾的","
						idx = tmpColName.find_first_of(",");
						if (idx != string::npos)
						{
							tmpColName = tmpColName.substr(0, idx);
						}
						if (tmpColName == "00P")
						{
							// 有功
							nColNum = 2;
						}
						else if (tmpColName == "00Q")
						{
							// 无功
							nColNum = 3;
						}
						else if (tmpColName == "UAV相角")
						{
							// 电压相角
							nColNum = 1;
						}
						else if (tmpColName == "UAV幅值")
						{
							// 电压幅值
							nColNum = 0;
						}
						else if (tmpColName == "EEV相角")
						{
							// 发电机功角
							nColNum = 16;
						}
						else
						{
							// 未定义，默认为无效
							nColNum = -1;
						}

						if (nColNum == ordernum)
						{
							// 获取设备数据
							if (df.GetData(djj + 2, tmpvecIntLine[ii].v2d_data) == -1)
							{
								sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
									tmpstring.c_str(), INT_REC_VAL(tmpdev_id), df.GetLastError());
								MessShow(m_LogString, LTDEBUG);
							}
							else
							{
								break;
							}
						}
					}
				}
				for (jj = 0; jj < tmpvecIntLine[ii].v2d_data.size(); jj++)
				{
					if (tmpvecIntLine[ii].v2d_data.empty())
					{
						continue;
					}
					if (ii == 0)
					{
						tmpLfoInf.v2d_data.push_back(tmpvecIntLine[ii].v2d_data[jj]);
					}
					else
					{
						tmpLfoInf.v2d_data[jj] += tmpvecIntLine[ii].v2d_data[jj];
					}
				}
			}
		}
		// 		ShowProMem("断面");
		df.ClearData();
	}

	// 数据采样
	if (nSample < 1)
	{
		nSample = 1;
	}
	// 采样后数据个数
	nNum = CURVE_TIME * CURVE_NUM_PER_SEC;

	sprintf(m_LogString, "曲线展示采样后点数[%d],采样间隔[%d]", nNum, nSample);
	MessShow(m_LogString, LTINFO);

	vector<float> tmp_vec_data;
	if (!tmpLfoGen.v2d_data.empty())
	{
		if (!tmp_vec_data.empty())
		{
			tmp_vec_data.clear();
		}
		for (jj = 0; jj < nNum && jj * nSample < tmpLfoGen.v2d_data.size(); jj++)
		{
			tmp_vec_data.push_back(tmpLfoGen.v2d_data[jj * nSample]);
		}

		vector<float>().swap(tmpLfoGen.v2d_data);
		tmpLfoGen.v2d_data.clear();
		tmpLfoGen.v2d_data.assign(tmp_vec_data.begin(), tmp_vec_data.end());
	}

	if (!tmpLfoLine.v2d_data.empty())
	{
		if (!tmp_vec_data.empty())
		{
			tmp_vec_data.clear();
		}
		for (jj = 0; jj < nNum && jj * nSample < tmpLfoLine.v2d_data.size(); jj++)
		{
			tmp_vec_data.push_back(tmpLfoLine.v2d_data[jj * nSample]);
		}

		vector<float>().swap(tmpLfoLine.v2d_data);
		tmpLfoLine.v2d_data.clear();
		tmpLfoLine.v2d_data.assign(tmp_vec_data.begin(), tmp_vec_data.end());
	}

	if (!tmpLfoTr.v2d_data.empty())
	{
		if (!tmp_vec_data.empty())
		{
			tmp_vec_data.clear();
		}
		for (jj = 0; jj < nNum && jj * nSample < tmpLfoTr.v2d_data.size(); jj++)
		{
			tmp_vec_data.push_back(tmpLfoTr.v2d_data[jj * nSample]);
		}

		vector<float>().swap(tmpLfoTr.v2d_data);
		tmpLfoTr.v2d_data.clear();
		tmpLfoTr.v2d_data.assign(tmp_vec_data.begin(), tmp_vec_data.end());
	}

	if (!tmpLfoInf.v2d_data.empty())
	{
		if (!tmp_vec_data.empty())
		{
			tmp_vec_data.clear();
		}
		for (jj = 0; jj < nNum && jj * nSample < tmpLfoInf.v2d_data.size(); jj++)
		{
			tmp_vec_data.push_back(tmpLfoInf.v2d_data[jj * nSample]);
		}

		vector<float>().swap(tmpLfoInf.v2d_data);
		tmpLfoInf.v2d_data.clear();
		tmpLfoInf.v2d_data.assign(tmp_vec_data.begin(), tmp_vec_data.end());
	}

	bool b_curve = true;
	if (m_pLfoCurve[0].time_point <= 0)
	{//初次启动
		for (int ii = 0; ii < (tmpLfoTime.end_time - tmpLfoTime.start_time) * CURVE_NUM_PER_SEC; ii++)
		{
			m_pLfoCurve[ii].recordid = PODB_GetDevIdByKeyID(LPO_RT_CURVE_NO, ii + 1);
			m_pLfoCurve[ii].time_sec = int(dtime * (ii + 1));
			m_pLfoCurve[ii].time_point = tmpLfoTime.start_time + m_pLfoCurve[ii].time_sec;
			m_pLfoCurve[ii].time_view = dtime * (ii + 1);
			sprintf(m_pLfoCurve[ii].time_des, "%d", m_pLfoCurve[ii].time_sec);
#ifdef _GUANGDONG
			sprintf(m_pLfoCurve[ii].gn_name, "%s%s", tmpLfoGen.dev.fac_name, tmpLfoGen.dev.dev_name);
			sprintf(m_pLfoCurve[ii].ln_name, "%s%s", tmpLfoLine.dev.fac_name, tmpLfoLine.dev.dev_name);
			sprintf(m_pLfoCurve[ii].int_name, "%s%s", tmpLfoInf.dev.fac_name, tmpLfoInf.dev.dev_name);
			sprintf(m_pLfoCurve[ii].tr_name, "%s%s", tmpLfoTr.dev.fac_name, tmpLfoTr.dev.dev_name);
#else
			sprintf(m_pLfoCurve[ii].gn_name, "%s", tmpLfoGen.dev.dev_name);
			sprintf(m_pLfoCurve[ii].ln_name, "%s", tmpLfoLine.dev.dev_name);
			sprintf(m_pLfoCurve[ii].int_name, "%s", tmpLfoInf.dev.dev_name);
			sprintf(m_pLfoCurve[ii].tr_name, "%s", tmpLfoTr.dev.dev_name);
#endif
			if (tmpLfoGen.v2d_data.size() > ii)
			{
				m_pLfoCurve[ii].gn_power = tmpLfoGen.v2d_data[ii];
			}
			if (tmpLfoLine.v2d_data.size() > ii)
			{
				m_pLfoCurve[ii].ln_power = tmpLfoLine.v2d_data[ii];
			}
			if (tmpLfoInf.v2d_data.size() > ii)
			{
				m_pLfoCurve[ii].int_power = tmpLfoInf.v2d_data[ii];
			}
			if (tmpLfoTr.v2d_data.size() > ii)
			{
				m_pLfoCurve[ii].tr_power = tmpLfoTr.v2d_data[ii];
			}
			if (m_MonSys.lead_dev.mode.freq < ZEROVALUE)
			{
				m_pLfoCurve[ii].lead_freq = 0.0;
				m_pLfoCurve[ii].lead_damp = m_LfoPara.max_damp;
			}
			else
			{
				m_pLfoCurve[ii].lead_freq = m_MonSys.lead_dev.mode.freq;
				m_pLfoCurve[ii].lead_damp = m_MonSys.lead_dev.mode.damp;
			}

		}
		if (tmpLfoTime.end_time - tmpLfoTime.start_time <= 60)
		{//使用案例文件时,第一个窗口可能不足60s,会走此分支,将不足60s的点置为999999
			int jj = (tmpLfoTime.end_time - tmpLfoTime.start_time) * CURVE_NUM_PER_SEC;
			sprintf(m_LogString, "曲线不足60秒,从第[%d]条记录开始补数据", jj);
			MessShow(m_LogString);
			for (int ii = jj; ii < CURVE_TIME * CURVE_NUM_PER_SEC; ii++)
			{
				m_pLfoCurve[ii].recordid = PODB_GetDevIdByKeyID(LPO_RT_CURVE_NO, ii + 1);
				m_pLfoCurve[ii].time_sec = int(dtime * (ii + 1));
				m_pLfoCurve[ii].time_point = tmpLfoTime.start_time + m_pLfoCurve[ii].time_sec;
				m_pLfoCurve[ii].time_view = dtime * (ii + 1);
				sprintf(m_pLfoCurve[ii].time_des, "%d", m_pLfoCurve[ii].time_sec);
				m_pLfoCurve[ii].gn_power = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].ln_power = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].int_power = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].tr_power = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].lead_freq = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].lead_damp = UNDISPLAY_VALUE;
			}
		}
		else
		{
		}
	}
	else
	{
		if (tmpLfoTime.end_time <= m_LfoPara.file_now_time)
		{//本轮时间小于曲线最新时间,此时不刷新程序
			b_curve = false;
			sprintf(m_LogString, "本轮时间小于曲线最新时间,不刷新程序");
			MessShow(m_LogString);
		}
		else if (m_LfoPara.data_source == MENU_LFO_DATA_RT || (m_LfoPara.data_source != MENU_LFO_DATA_RT && (tmpLfoTime.end_time - m_LfoPara.file_start_time) > 60))
		{//非初次启动 使用实时数据 模式结果需要前移 时间和有功照常赋值
			int move_num = (tmpLfoTime.end_time - m_LfoPara.file_now_time) * CURVE_NUM_PER_SEC;//计算需要前移的点数
			for (int ii = 0; ii < CURVE_TIME * CURVE_NUM_PER_SEC; ii++)
			{
				m_pLfoCurve[ii].recordid = PODB_GetDevIdByKeyID(LPO_RT_CURVE_NO, ii + 1);
				m_pLfoCurve[ii].time_sec = int(dtime * (ii + 1));
				m_pLfoCurve[ii].time_view = dtime * (ii + 1);
				m_pLfoCurve[ii].time_point = tmpLfoTime.start_time + m_pLfoCurve[ii].time_sec;
				sprintf(m_pLfoCurve[ii].time_des, "%d", m_pLfoCurve[ii].time_sec);
#ifdef _GUANGDONG
				sprintf(m_pLfoCurve[ii].gn_name, "%s%s", tmpLfoGen.dev.fac_name, tmpLfoGen.dev.dev_name);
				sprintf(m_pLfoCurve[ii].ln_name, "%s%s", tmpLfoLine.dev.fac_name, tmpLfoLine.dev.dev_name);
				sprintf(m_pLfoCurve[ii].int_name, "%s%s", tmpLfoInf.dev.fac_name, tmpLfoInf.dev.dev_name);
				sprintf(m_pLfoCurve[ii].tr_name, "%s%s", tmpLfoTr.dev.fac_name, tmpLfoTr.dev.dev_name);
#else
				sprintf(m_pLfoCurve[ii].gn_name, "%s", tmpLfoGen.dev.dev_name);
				sprintf(m_pLfoCurve[ii].ln_name, "%s", tmpLfoLine.dev.dev_name);
				sprintf(m_pLfoCurve[ii].int_name, "%s", tmpLfoInf.dev.dev_name);
				sprintf(m_pLfoCurve[ii].tr_name, "%s", tmpLfoTr.dev.dev_name);
#endif
				if (tmpLfoGen.v2d_data.size() > ii)
				{
					m_pLfoCurve[ii].gn_power = tmpLfoGen.v2d_data[ii];
				}
				if (tmpLfoLine.v2d_data.size() > ii)
				{
					m_pLfoCurve[ii].ln_power = tmpLfoLine.v2d_data[ii];
				}
				if (tmpLfoInf.v2d_data.size() > ii)
				{
					m_pLfoCurve[ii].int_power = tmpLfoInf.v2d_data[ii];
				}
				if (tmpLfoTr.v2d_data.size() > ii)
				{
					m_pLfoCurve[ii].tr_power = tmpLfoTr.v2d_data[ii];
				}
				if (ii < (CURVE_TIME * CURVE_NUM_PER_SEC - move_num))
				{//模式前移
					m_pLfoCurve[ii].lead_freq = m_pLfoCurve[ii + move_num].lead_freq;
					m_pLfoCurve[ii].lead_damp = m_pLfoCurve[ii + move_num].lead_damp;
				}
				else
				{
					if (m_MonSys.lead_dev.mode.freq < ZEROVALUE)
					{
						m_pLfoCurve[ii].lead_freq = 0.0;
						m_pLfoCurve[ii].lead_damp = m_LfoPara.max_damp;
					}
					else
					{
						m_pLfoCurve[ii].lead_freq = m_MonSys.lead_dev.mode.freq;
						m_pLfoCurve[ii].lead_damp = m_MonSys.lead_dev.mode.damp;
					}
				}
			}
		}
		else if (m_LfoPara.data_source != MENU_LFO_DATA_RT && (tmpLfoTime.end_time - m_LfoPara.file_start_time) <= 60)
		{//使用案例数据 读取文件未超过60秒 有功结果全覆盖 已有的模式结果不变 新增的模式结果紧接着已有结果写 不足60秒的补充999999
			for (int ii = 0; ii < (tmpLfoTime.end_time - tmpLfoTime.start_time) * CURVE_NUM_PER_SEC; ii++)
			{
				m_pLfoCurve[ii].time_sec = int(dtime * (ii + 1));
				m_pLfoCurve[ii].recordid = PODB_GetDevIdByKeyID(LPO_RT_CURVE_NO, ii + 1);
				m_pLfoCurve[ii].time_point = tmpLfoTime.start_time + m_pLfoCurve[ii].time_sec;
				m_pLfoCurve[ii].time_view = dtime * (ii + 1);
				sprintf(m_pLfoCurve[ii].time_des, "%d", m_pLfoCurve[ii].time_sec);
				sprintf(m_pLfoCurve[ii].gn_name, "%s", tmpLfoGen.dev.dev_name);
				sprintf(m_pLfoCurve[ii].ln_name, "%s", tmpLfoLine.dev.dev_name);
				sprintf(m_pLfoCurve[ii].int_name, "%s", tmpLfoInf.dev.dev_name);
				sprintf(m_pLfoCurve[ii].tr_name, "%s", tmpLfoTr.dev.dev_name);
				if (tmpLfoGen.v2d_data.size() > ii)
				{
					m_pLfoCurve[ii].gn_power = tmpLfoGen.v2d_data[ii];
				}
				if (tmpLfoLine.v2d_data.size() > ii)
				{
					m_pLfoCurve[ii].ln_power = tmpLfoLine.v2d_data[ii];
				}
				if (tmpLfoInf.v2d_data.size() > ii)
				{
					m_pLfoCurve[ii].int_power = tmpLfoInf.v2d_data[ii];
				}
				if (tmpLfoTr.v2d_data.size() > ii)
				{
					m_pLfoCurve[ii].tr_power = tmpLfoTr.v2d_data[ii];
				}
				//模式结果为999999的记录替换为本轮结果
				if (m_MonSys.lead_dev.mode.freq < ZEROVALUE)
				{
					if (fabs(m_pLfoCurve[ii].lead_freq - UNDISPLAY_VALUE) < 0.1)
					{
						m_pLfoCurve[ii].lead_freq = 0.0;
					}
					if (fabs(m_pLfoCurve[ii].lead_damp - UNDISPLAY_VALUE) < 0.1)
					{
						m_pLfoCurve[ii].lead_damp = m_LfoPara.max_damp;
					}
				}
				else
				{
					if (fabs(m_pLfoCurve[ii].lead_freq - UNDISPLAY_VALUE) < 0.1)
					{
						m_pLfoCurve[ii].lead_freq = m_MonSys.lead_dev.mode.freq;
					}
					if (fabs(m_pLfoCurve[ii].lead_damp - UNDISPLAY_VALUE) < 0.1)
					{
						m_pLfoCurve[ii].lead_damp = m_MonSys.lead_dev.mode.damp;
					}
				}
			}
			int jj = (tmpLfoTime.end_time - tmpLfoTime.start_time) * CURVE_NUM_PER_SEC;
			sprintf(m_LogString, "曲线不足60秒,从第[%d]条记录开始补数据", jj);
			MessShow(m_LogString);
			for (int ii = jj; ii < CURVE_TIME * CURVE_NUM_PER_SEC; ii++)
			{
				m_pLfoCurve[ii].recordid = PODB_GetDevIdByKeyID(LPO_RT_CURVE_NO, ii + 1);
				m_pLfoCurve[ii].time_sec = int(dtime * (ii + 1));
				m_pLfoCurve[ii].time_view = dtime * (ii + 1);
				m_pLfoCurve[ii].time_point = tmpLfoTime.start_time + m_pLfoCurve[ii].time_sec;
				sprintf(m_pLfoCurve[ii].time_des, "%d", m_pLfoCurve[ii].time_sec);
				m_pLfoCurve[ii].gn_power = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].ln_power = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].int_power = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].tr_power = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].lead_freq = UNDISPLAY_VALUE;
				m_pLfoCurve[ii].lead_damp = UNDISPLAY_VALUE;
			}
		}
	}

	//处理模式曲线,将1500个点抽样成60点（画面只展示60个点）
	for (int ii = 0; ii < CURVE_TIME * CURVE_NUM_PER_SEC; ii++)
	{//先缓存模式曲线
		tmpfreq[ii] = m_pLfoCurve[ii].lead_freq;
		tmpdamp[ii] = m_pLfoCurve[ii].lead_damp;
		m_pLfoCurve[ii].lead_freq = UNDISPLAY_VALUE;
		m_pLfoCurve[ii].lead_damp = UNDISPLAY_VALUE;
	}
	m_pLfoCurve[0].lead_freq = tmpfreq[0];
	m_pLfoCurve[0].lead_damp = tmpdamp[0];
	for (int ii = 1; ii <= CURVE_TIME; ii++)
	{//抽样模式曲线
		m_pLfoCurve[ii].lead_freq = tmpfreq[(ii - 1) * CURVE_NUM_PER_SEC];
		m_pLfoCurve[ii].lead_damp = tmpdamp[(ii - 1) * CURVE_NUM_PER_SEC];
	}

	// 更新LFO实时曲线表
// 	UpdateLfoCurve(tmpLfoTime);
// 	UpdateLfoCurve(tmpLfoGen);
// 	UpdateLfoCurve(tmpLfoLine);
// 	UpdateLfoCurve(tmpLfoTr);
// 	UpdateLfoCurve(tmpLfoInf);

	m_LfoPara.file_now_time = tmpLfoTime.end_time;

	UpdateMonGnDisp(tmpvecGen);
	UpdateMonLnDisp(tmpvecLine);
	UpdateMonTrDisp(tmpvecTr);
	UpdateMonIntDisp(tmpvecInf);

	ClearLfoDev(tmpLfoTime);
	ClearLfoDev(tmpLfoGen);
	ClearLfoDev(tmpLfoLine);
	ClearLfoDev(tmpLfoTr);
	ClearLfoDev(tmpLfoInf);

	if (b_curve == true)
	{
		vector <INT_REC> tmpRecordid;
		for(int ii = 0;ii < CURVE_TIME * CURVE_NUM_PER_SEC; ii++)
			tmpRecordid.push_back(m_pLfoCurve[ii].recordid);
		PODB_DeleteRecord(tmpRecordid,LPO_RT_CURVE_NO);
		tmpRecordid.clear();
		vector<INT_REC>().swap(tmpRecordid);
		int ret_code = PODB_TableWrite((const char*)&m_pLfoCurve[0].recordid, CURVE_TIME * CURVE_NUM_PER_SEC * sizeof(LPO_RT_CURVE_WAMS), LPO_RT_CURVE_NO);
		if (ret_code < 0)
		{
			sprintf(m_LogString, "写LPO实时曲线表[%d]失败", LPO_RT_CURVE_NO);
			MessShow(m_LogString);
		}
	}

	for (int ii = 0; ii < CURVE_TIME * CURVE_NUM_PER_SEC; ii++)
	{//模式曲线还原
		m_pLfoCurve[ii].lead_freq = tmpfreq[ii];
		m_pLfoCurve[ii].lead_damp = tmpdamp[ii];
	}
#endif
	return 0;
}
//更新有功曲线
int CLfoAnaObj::UpdateLfoCurve(LFO_DEV_INFO& tmpLfoDev)
{
#ifdef _WAMAP_
	int ii, nDataNum1, nDataNum2;
	TAB_LFO_RT_CURVE1 tmpCurve1;
	TAB_LFO_RT_CURVE2 tmpCurve2;
	memset((void*)&tmpCurve1, 0, sizeof(TAB_LFO_RT_CURVE1));
	memset((void*)&tmpCurve2, 0, sizeof(TAB_LFO_RT_CURVE2));

	if (m_LfoPara.is_add_facname == 1)
	{
		if (tmpLfoDev.dev.dev_type == MENU_LFO_DEV_GEN || tmpLfoDev.dev.dev_type == MENU_LFO_DEV_TR)
		{
			// 发电机和变压器设备增加厂站名称
			sprintf(tmpCurve1.dev_name, "%s%s", tmpLfoDev.dev.fac_name, tmpLfoDev.dev.dev_name);
		}
		else
		{
			// 交流线路不增加厂站名称
			sprintf(tmpCurve1.dev_name, "%s", tmpLfoDev.dev.dev_name);
		}
	}
	else
	{
		sprintf(tmpCurve1.dev_name, "%s", tmpLfoDev.dev.dev_name);
	}

	sprintf(tmpCurve1.unit, "%s", "MW");
	//tmpCurve1.sam_freq = 
	tmpCurve1.cuv_bgn_sec = tmpLfoDev.start_time;
	tmpCurve1.cuv_end_sec = tmpLfoDev.end_time;
	//tmpCurve1.cuv_sec = 
	//tmpCurve1.cuv_pnt =
	//tmpCurve1.cuv_stp =
	//tmpCurve1.max_val =
	//tmpCurve1.min_val =
	//tmpCurve1.ave_val =
	int sample_freq = int(tmpLfoDev.v2d_data.size() / 250);
	sprintf(m_LogString, "原始曲线点数[%d],采样频率[%d]", tmpLfoDev.v2d_data.size(), sample_freq);
	MessShow(m_LogString);
	if ((int)tmpLfoDev.v2d_data.size() > 125)
	{
		nDataNum1 = 125;
	}
	else
	{
		nDataNum1 = (int)tmpLfoDev.v2d_data.size();
	}
	for (ii = 0; ii < nDataNum1; ii++)
	{
		tmpCurve1.seq_cuv_data[ii] = (float)tmpLfoDev.v2d_data[ii * sample_freq];
	}

	if ((int)tmpLfoDev.v2d_data.size() - 125 > 125)
	{
		nDataNum2 = 125;
	}
	else
	{
		nDataNum2 = (int)tmpLfoDev.v2d_data.size() - 125;
	}
	for (ii = 0; ii < nDataNum2; ii++)
	{
		tmpCurve2.seq_cuv_data[ii] = (float)tmpLfoDev.v2d_data[(125 + ii) * sample_freq];
	}

	INT_REC dev_idx;
	if (tmpLfoDev.dev.dev_type == MENU_LFO_DEV_GEN)
	{
		dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 10);
	}
	else if (tmpLfoDev.dev.dev_type == MENU_LFO_DEV_LINE)
	{
		dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 11);
	}
	else if (tmpLfoDev.dev.dev_type == MENU_LFO_DEV_TR)
	{
		dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 12);
	}
	else if (tmpLfoDev.dev.dev_type == MENU_LFO_DEV_ITFC)
	{
		dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 13);
	}
	else if (tmpLfoDev.dev.dev_type == 999)
	{
		dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 9);
	}
	else
	{
		sprintf(m_LogString, "曲线类型错误[%d]", tmpLfoDev.dev.dev_type);
		MessShow(m_LogString, LTERROR);
		return 0;
	}

	PODB_TableModifyByKey((const char*)&dev_idx, TAB_LFO_RT_CURVE1_FLD, (const char*)&tmpCurve1, sizeof(TAB_LFO_RT_CURVE1), LFO_RT_CURVE_NO_TAB);
	PODB_TableModifyByKey((const char*)&dev_idx, TAB_LFO_RT_CURVE2_FLD, (const char*)&tmpCurve2, sizeof(TAB_LFO_RT_CURVE2), LFO_RT_CURVE_NO_TAB);
#endif
	return 0;
}
// 形成主导模式曲线
void CLfoAnaObj::FormMonModeCurce()
{
	int  ii, jj;
	int  CalcWdsTime = 10;
	// 	if (m_LfoPara.curve_time_60 == 1)
	// 	{
	// 		CalcWdsTime = 60;
	// 	} 
	// 	else
	// 	{
	// 		CalcWdsTime = 20;
	// 	}

		// 首次启动
	if ((m_pLfoModeCurve[0].bgn_t <= 0 && m_pLfoModeCurve[0].end_t <= 0))
	{
		for (ii = 0; ii < LFO_CURVE_MAX_PNT; ii++)
		{
			jj = LFO_CURVE_MAX_PNT - ii - 1;
			m_pLfoModeCurve[ii].bgn_t = m_MonSys.start_time - jj * CalcWdsTime;
			m_pLfoModeCurve[ii].end_t = m_MonSys.end_time - jj * CalcWdsTime;
			m_pLfoModeCurve[ii].ampl = 0;
			m_pLfoModeCurve[ii].freq = 0;
			//			m_pLfoModeCurve[ii].damp  = 100.0;
			m_pLfoModeCurve[ii].damp = m_LfoPara.max_damp;
			m_pLfoModeCurve[ii].phase = 0;
			m_pLfoModeCurve[ii].real = 0;
			m_pLfoModeCurve[ii].image = 0;
			// 			sprintf(m_LogString,"首次[%d]bgn_t=[%ld]",ii+1,m_pLfoModeCurve[ii].bgn_t);
			// 			MessShow(m_LogString);

		}
	}
	if (m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].end_t >= m_MonSys.end_time)
	{
		return;
	}
	// 前移
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
		// 		sprintf(m_LogString,"前移[%d]bgn_t=[%ld]",ii+1,m_pLfoModeCurve[ii].bgn_t);
		// 		MessShow(m_LogString);
	}
	// 填充
	m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t = m_MonSys.start_time;
	m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].end_t = m_MonSys.end_time;
	m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].ampl = m_MonSys.lead_dev.mode.damp;
	sprintf(m_LogString, "填充[%d]bgn_t=[%ld],end_t=[%ld]", LFO_CURVE_MAX_PNT, m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t, m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].end_t);
	MessShow(m_LogString);
	if (m_MonSys.lead_dev.mode.freq < ZEROVALUE)
	{
		if (m_MonSys.stat >= MENU_LFO_DEV_STS_FWN)
		{
			return;
		}
		m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].freq = 0;
		//			m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].damp  = 100.0;
		m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].damp = m_LfoPara.max_damp;
	}
	else
	{
		m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].freq = m_MonSys.lead_dev.mode.freq;
		m_pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].damp = m_MonSys.lead_dev.mode.damp;
	}
	// 	m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].phase = m_SysMon.mon_phas;
	// 	m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].real  = m_SysMon.mon_real;
	// 	m_pLfoModeCurve[LFO_CURVE_MAX_PNT-1].image = m_SysMon.mon_imag;

	return;
}
//  LFO实时曲线表（记录4、5、6）
void CLfoAnaObj::UpdateMonModeCurce(PERIOD_LEAD_MODE* pLfoModeCurve)
{
#ifdef _WAMAP_

	TAB_LFO_RT_CURVE  LfoModeCurve;
	// 	SEQ_BASE_INFO	  dataBaseInfo ;
	int i, j;

	for (i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			LfoModeCurve.dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 14);
			ZERO_INT_REC(LfoModeCurve.dev_id);			
			sprintf(LfoModeCurve.dev_name, "%s", "模式曲线时间");
			sprintf(LfoModeCurve.unit, "%s", "秒");
			LfoModeCurve.sam_freq = 0;
			LfoModeCurve.cuv_bgn_sec = pLfoModeCurve[0].end_t;
			LfoModeCurve.cuv_end_sec = pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].end_t;
			LfoModeCurve.cuv_sec = LfoModeCurve.cuv_end_sec - LfoModeCurve.cuv_bgn_sec;
			LfoModeCurve.cuv_pnt = LFO_CURVE_MAX_PNT;
			LfoModeCurve.cuv_stp = (float)((LfoModeCurve.cuv_sec * 1.0) / LfoModeCurve.cuv_pnt);
			LfoModeCurve.max_val = 0.0;
			LfoModeCurve.min_val = 0.0;
			LfoModeCurve.ave_val = 0.0;

			for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
			{
				LfoModeCurve.seq_cuv_data[j] = (pLfoModeCurve[j].bgn_t - pLfoModeCurve[0].bgn_t) * 1.0;
			}

			UpdateLfoCurve(LfoModeCurve);
		}

		if (i == 1)
		{
			LfoModeCurve.dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 16);
			ZERO_INT_REC(LfoModeCurve.dev_id);
			sprintf(LfoModeCurve.dev_name, "%s", "频率曲线");
			sprintf(LfoModeCurve.unit, "%s", "Hz");
			LfoModeCurve.sam_freq = 0;
			LfoModeCurve.cuv_bgn_sec = pLfoModeCurve[0].bgn_t;
			LfoModeCurve.cuv_end_sec = pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t;
			LfoModeCurve.cuv_sec = LfoModeCurve.cuv_end_sec - LfoModeCurve.cuv_bgn_sec;
			LfoModeCurve.cuv_pnt = LFO_CURVE_MAX_PNT;
			LfoModeCurve.cuv_stp = (float)((LfoModeCurve.cuv_sec * 1.0) / LfoModeCurve.cuv_pnt);

			for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
			{
				LfoModeCurve.seq_cuv_data[j] = pLfoModeCurve[j].freq;
			}

			// 			DP_CalcDataSeqBaseInfo( (float*)&LfoModeCurve.seq_cuv_data[0], LFO_CURVE_MAX_PNT, dataBaseInfo);
			// 			LfoModeCurve.max_val = dataBaseInfo.max_val;
			// 			LfoModeCurve.min_val = dataBaseInfo.min_val;
			// 			LfoModeCurve.ave_val = dataBaseInfo.ave_val;
			// 			
			UpdateLfoCurve(LfoModeCurve);
		}

		if (i == 2)
		{
			LfoModeCurve.dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB, 15);
			ZERO_INT_REC(LfoModeCurve.dev_id);
			sprintf(LfoModeCurve.dev_name, "%s", "阻尼比曲线");
			sprintf(LfoModeCurve.unit, "%s", "%");
			LfoModeCurve.sam_freq = 0;
			LfoModeCurve.cuv_bgn_sec = pLfoModeCurve[0].bgn_t;
			LfoModeCurve.cuv_end_sec = pLfoModeCurve[LFO_CURVE_MAX_PNT - 1].bgn_t;
			LfoModeCurve.cuv_sec = LfoModeCurve.cuv_end_sec - LfoModeCurve.cuv_bgn_sec;
			LfoModeCurve.cuv_pnt = LFO_CURVE_MAX_PNT;
			LfoModeCurve.cuv_stp = (float)((LfoModeCurve.cuv_sec * 1.0) / LfoModeCurve.cuv_pnt);

			for (j = 0; j < LFO_CURVE_MAX_PNT; j++)
			{
				LfoModeCurve.seq_cuv_data[j] = pLfoModeCurve[j].damp;
			}

			// 			DP_CalcDataSeqBaseInfo( (float*)&LfoModeCurve.seq_cuv_data[0], LFO_CURVE_MAX_PNT, dataBaseInfo);
			// 			LfoModeCurve.max_val = dataBaseInfo.max_val;
			// 			LfoModeCurve.min_val = dataBaseInfo.min_val;
			// 			LfoModeCurve.ave_val = dataBaseInfo.ave_val;

			UpdateLfoCurve(LfoModeCurve);
		}
	}

#endif
	return;
}
//   写LFO曲线
void CLfoAnaObj::UpdateLfoCurve(TAB_LFO_RT_CURVE RtCurve)
{
#ifdef _WAMAP_

	int i = 0;

	TAB_LFO_RT_CURVE1    temp_RtCurve1;
	TAB_LFO_RT_CURVE2    temp_RtCurve2;

	memset((void*)&temp_RtCurve1, 0, sizeof(TAB_LFO_RT_CURVE1));
	memset((void*)&temp_RtCurve2, 0, sizeof(TAB_LFO_RT_CURVE2));

	INT_REC keyrecord = RtCurve.dev_idx;
	// 
	// 	if ( g_IDBTableOp.Open( LFO_AP_ID, LFO_RT_CURVE_NO, g_ctxNo ) != DB_OK )  
	// 	{
	// 		sprintf(m_LogString, "when open app: %d  table: %d,  error",LFO_AP_ID,LFO_RT_CURVE_NO);
	// 		WriteLogFile(m_LogString);
	// 		return;
	// 	}

	//temp_RtCurve1.dev_id      = RtCurve.dev_id;
	sprintf(temp_RtCurve1.dev_name, "%s", RtCurve.dev_name);
	sprintf(temp_RtCurve1.unit, "%s", RtCurve.unit);

	temp_RtCurve1.sam_freq = RtCurve.sam_freq;
	temp_RtCurve1.cuv_bgn_sec = RtCurve.cuv_bgn_sec;
	temp_RtCurve1.cuv_end_sec = RtCurve.cuv_end_sec;
	temp_RtCurve1.cuv_sec = RtCurve.cuv_sec;
	temp_RtCurve1.cuv_pnt = RtCurve.cuv_pnt;
	temp_RtCurve1.cuv_stp = RtCurve.cuv_stp;
	temp_RtCurve1.max_val = RtCurve.max_val;
	temp_RtCurve1.min_val = RtCurve.min_val;
	temp_RtCurve1.ave_val = RtCurve.ave_val;

	for (i = 0; i < 125; i++)
	{
		temp_RtCurve1.seq_cuv_data[i] = RtCurve.seq_cuv_data[i];
	}
	for (i = 0; i < 125; i++)
	{
		temp_RtCurve2.seq_cuv_data[i] = RtCurve.seq_cuv_data[i + 125];
	}

	PODB_TableModifyByKey((const char*)&keyrecord, TAB_LFO_RT_CURVE1_FLD, (char*)&temp_RtCurve1.dev_name, sizeof(TAB_LFO_RT_CURVE1), LFO_RT_CURVE_NO_TAB);
	PODB_TableModifyByKey((const char*)&keyrecord, TAB_LFO_RT_CURVE2_FLD, (char*)&temp_RtCurve2.seq_cuv_data[0], sizeof(TAB_LFO_RT_CURVE2), LFO_RT_CURVE_NO_TAB);
#endif
	return;
}

//更新耗散功率曲线
int CLfoAnaObj::SavePowCurve(LFO_CASE_INFO& LfoCase)
{
	// 	sprintf(m_LogString,"耗散功率曲线入库");
	// 	MessShow(m_LogString);
	// 
	// 	int ii,jj;
	// 	int max_wd = -1 ;//最大窗口数
	// 	int max_wd2 = -1 ;//最大窗口数
	// 	INT_REC dev_idx;
	// 	float aver_pow;
	// 	vector<LFO_DEV_INFO> tmpVecCurve;
	// 	vector<LFO_DEV_INFO> tmpDev;
	// 	LFO_DEV_INFO tmpCurve;
	// 	TAB_LFO_RT_CURVE1 tmpCurve1;
	// 	TAB_LFO_RT_CURVE2 tmpCurve2;
	// 	memset((void *)&tmpCurve1,0,sizeof(TAB_LFO_RT_CURVE1));
	// 	memset((void *)&tmpCurve2,0,sizeof(TAB_LFO_RT_CURVE2));
	// 
	// 
	// 	tmpDev.clear();
	// 	for (ii=0;ii<LfoCase.vec_dev.size();ii++)
	// 	{
	// 		if (LfoCase.vec_dev[ii].mode.is_source == MENU_LFO_MENU_YES)
	// 		{
	// 			tmpDev.push_back(LfoCase.vec_dev[ii]);
	// 			if (LfoCase.vec_dev[ii].mode.pow_num>max_wd)
	// 			{
	// 				max_wd = LfoCase.vec_dev[ii].mode.pow_num;
	// 			}
	// 			continue;
	// 		}
	// 	}
	// 	max_wd2 = max_wd;
	// 	if (max_wd > 250)
	// 	{
	// 		max_wd = 250;
	// 	}
	// 	if (tmpDev.size() == 0)
	// 	{
	// 		sprintf(m_LogString,"没有振荡源设备");
	// 		MessShow(m_LogString);
	// 		return 0;
	// 	}
	// 	sprintf(m_LogString,"振荡源设备数%d,最大窗口数[%d]",tmpDev.size(),max_wd);
	// 	MessShow(m_LogString);
	// 	for (ii=0;ii<tmpDev.size();ii++)
	// 	{
	// 		aver_pow = 0.0;
	// 		for (jj=0;jj<tmpDev[ii].mode.pow_num;jj++)
	// 		{
	// 			aver_pow += tmpDev[ii].mode.vec_pow[jj];	
	// 		}
	// 		tmpDev[ii].mode.pow = float(aver_pow/tmpDev[ii].mode.pow_num);//计算平均耗散功率
	// 		sprintf(m_LogString,"振荡源设备[%s],窗口数[%d],平均耗散功率[%f]",tmpDev[ii].dev.dev_name,tmpDev[ii].mode.pow_num,tmpDev[ii].mode.pow);
	// 		MessShow(m_LogString);
	// 	}
	// 
	// 	sort(tmpDev.begin(),tmpDev.end(),g_SortDevByPow);
	// 
	// 	sprintf(tmpCurve1.unit,"%s","sec");
	// 
	// 	if (max_wd <= 125)
	// 	{
	// 		for (ii=0;ii<max_wd;ii++)
	// 		{
	// 			tmpCurve1.seq_cuv_data[ii] = (float)10.0*ii;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		for (ii=0;ii<125;ii++)
	// 		{
	// 			tmpCurve1.seq_cuv_data[ii] = (float)10.0*ii;
	// 		}
	// 		for (ii=125;ii<max_wd;ii++)
	// 		{
	// 			tmpCurve2.seq_cuv_data[ii-125] = (float)10.0*ii;
	// 		}
	// 	}
	// 	dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB,20);
	// 
	// 	PODB_TableModifyByKey((const char *)&dev_idx, TAB_LFO_RT_CURVE1_FLD, (const char *)&tmpCurve1, sizeof(TAB_LFO_RT_CURVE1),LFO_RT_CURVE_NO_TAB);
	// 	PODB_TableModifyByKey((const char *)&dev_idx, TAB_LFO_RT_CURVE2_FLD, (const char *)&tmpCurve2, sizeof(TAB_LFO_RT_CURVE2),LFO_RT_CURVE_NO_TAB);
	// 
	// 
	// 	for (ii=0;ii<tmpDev.size();ii++)
	// 	{
	// 		memset((void *)&tmpCurve1,0,sizeof(TAB_LFO_RT_CURVE1));
	// 		memset((void *)&tmpCurve2,0,sizeof(TAB_LFO_RT_CURVE2));
	// 
	// 		sprintf(tmpCurve1.unit,"%s","MW");
	// 		sprintf(tmpCurve1.dev_name,"%s",tmpDev[ii].dev.dev_name);
	// 		dev_idx = PODB_GetDevIdByKeyID(LFO_RT_CURVE_NO_TAB,21+ii);
	// 
	// 		if (tmpDev[ii].mode.pow_num <= 125)
	// 		{
	// 			for (jj=0;jj<tmpDev[ii].mode.pow_num;jj++ )
	// 			{
	// 				tmpCurve1.seq_cuv_data[jj] = tmpDev[ii].mode.vec_pow[jj];
	// 			}
	// 		} 
	// 		else if (tmpDev[ii].mode.pow_num <= 250)
	// 		{
	// 			for (jj=0;jj<tmpDev[ii].mode.pow_num;jj++ )
	// 			{
	// 				if (jj<125)
	// 				{
	// 					tmpCurve1.seq_cuv_data[jj] = tmpDev[ii].mode.vec_pow[jj];
	// 				}
	// 				if (jj>=125)
	// 				{
	// 					tmpCurve2.seq_cuv_data[jj-125] = tmpDev[ii].mode.vec_pow[jj];
	// 				}
	// 			}
	// 		}
	// 		else if (tmpDev[ii].mode.pow_num > 250)
	// 		{
	// 			for (jj = 0;jj < 250;jj++)
	// 			{
	// 				if (jj < 125)
	// 				{
	// 					tmpCurve1.seq_cuv_data[jj] = tmpDev[ii].mode.vec_pow[jj+tmpDev[ii].mode.pow_num-250];
	// 				}
	// 				if (jj >= 125)
	// 				{
	// 					tmpCurve2.seq_cuv_data[jj-125] = tmpDev[ii].mode.vec_pow[jj+tmpDev[ii].mode.pow_num-250];
	// 				}
	// 			}
	// 		}
	// 		PODB_TableModifyByKey((const char *)&dev_idx, TAB_LFO_RT_CURVE1_FLD, (const char *)&tmpCurve1, sizeof(TAB_LFO_RT_CURVE1),LFO_RT_CURVE_NO_TAB);
	// 		PODB_TableModifyByKey((const char *)&dev_idx, TAB_LFO_RT_CURVE2_FLD, (const char *)&tmpCurve2, sizeof(TAB_LFO_RT_CURVE2),LFO_RT_CURVE_NO_TAB);
	// 
	// 	}
	// 
	// 

	return 0;

}


int CLfoAnaObj::UpdatePowCurve(LFO_DEV_INFO& tmpLfoDev)
{
	return 0;
}


int CLfoAnaObj::SendWarnMessage(LFO_SYS_INFO& MonSys, int type, int end_time, int hold_time, int if_lfo)
{
#ifndef _NUSP
#ifdef _WAMAP_
	if (m_LfoPara.is_warn == MENU_LFO_MENU_NO)
	{
		sprintf(m_LogString, "是否发送告警[%d],退出", m_LfoPara.is_warn);
		MessShow(m_LogString, LTINFO);
		return 0;
	}

	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
	int warn_type = -1;//低频振荡210 超低频211 中高频 211

	APP_TO_WARN_SERVICE_MESSAGE_STRU tmpMsg;
	tmpMsg.warn_num = 1;
	tmpMsg.seq_warn_message.length(1);
	string tmpstring;
	//yjj	string hostname;
	char hostname[50];
	int ntime;
	if (if_lfo == 0)//原有的功率波动告警流程
	{
		if (type == LFO_UNSAVE_WARN)
		{
			ntime = MonSys.start_time;
			if (m_LfoPara.is_eng == 1)
			{
				tmpstring = PT_GetTimeStringBySeconds(ntime);
				tmpstring += " power oscillation happening,";
			}
			else
			{
				//			tmpstring = PT_GetChineseTimeStringBySeconds(ntime);
				if (MonSys.lead_dev.mode.freq < m_LfoPara.lfo_freq_min)
				{
					warn_type = WAMS_LFO_LPO_WARN_TYPE;
					tmpstring = PT_GetTimeStringBySeconds(ntime);
					tmpstring += " 超低频振荡告警,";
				}
				else if (MonSys.lead_dev.mode.freq > m_LfoPara.lfo_freq_max)
				{
					warn_type = WAMS_LFO_LPO_WARN_TYPE;
					tmpstring = PT_GetTimeStringBySeconds(ntime);
					tmpstring += " 中高频振荡告警,";
				}
				else if (MonSys.lead_dev.mode.freq >= m_LfoPara.lfo_freq_min && MonSys.lead_dev.mode.freq <= m_LfoPara.lfo_freq_max)
				{
					warn_type = WAMS_LFO_WARN_TYPE;
					tmpstring = PT_GetTimeStringBySeconds(ntime);
					tmpstring += " 低频振荡告警,";
				}

			}

			tmpstring += MonSys.stat_desc;

			//yjj		if (PODB_GetHostName(hostname,m_LfoPara.warn_app_no)!=1)
			if (PODB_GetHostName(hostname, sizeof(hostname), m_LfoPara.warn_app_no, g_ctxNo) != 1)
			{
				return 0;
			}
			// 			if (m_LfoPara.data_source != LFO_DATA_SOURCE_REALTTIME)
			// 			{
			// 				while(true)
			// 				{
			// 					if (ntime >= PT_GetNowSeconds())
			// 					{
			// 						PT_DelayMinSec(1000);
			// 					}
			// 					else
			// 					{
			// 						break;
			// 					}
			// 				}
			// 			}
			sprintf(m_LogString, "振荡告警:host[%s],fac_id["INT_REC_FMT"],desc[%s]", hostname, INT_REC_VAL(m_LfoPara.lfo_fac_id), tmpstring.c_str());
			MessShow(m_LogString, LTINFO);
		}
		else if (type == LFO_STSOLD_WARN)
		{
			ntime = MonSys.gps_time;
			tmpstring = PT_GetTimeStringBySeconds(ntime);
			tmpstring += "PMU数据不刷新";
			ZERO_INT_REC(m_LfoPara.lfo_fac_id);		
			sprintf(m_LogString, "PMU数据不刷新告警");
			MessShow(m_LogString, LTINFO);
			//yjj		if (PODB_GetHostName(hostname,m_LfoPara.warn_app_no)!=1)
			if (PODB_GetHostName(hostname, sizeof(hostname), m_LfoPara.warn_app_no, g_ctxNo) != 1)
			{
				return 0;
			}
		}
		else if (type == LFO_SAVE_WARN)
		{
			ntime = end_time;
			int nM, nS;
			char  temp_time_str[64], statdes[200];
			nM = hold_time / 60;
			nS = (hold_time - nM * 60);
			if (nM > 0)
			{
				sprintf(temp_time_str, "%d分%d秒", nM, nS);
			}
			else
			{
				sprintf(temp_time_str, "%d秒", nS);
			}
			sprintf(statdes, " 振荡告警结束,持续时间%s。", temp_time_str);

			tmpstring = PT_GetTimeStringBySeconds(ntime);

			tmpstring += statdes;
			ZERO_INT_REC(m_LfoPara.lfo_fac_id);	
			if (PODB_GetHostName(hostname, sizeof(hostname), m_LfoPara.warn_app_no, g_ctxNo) != 1)
			{
				return 0;
			}
			sprintf(m_LogString, "振荡告警结束:host[%s],fac_id["INT_REC_FMT"],desc[%s]", hostname, INT_REC_VAL(m_LfoPara.lfo_fac_id), tmpstring.c_str());
			MessShow(m_LogString, LTINFO);
		}
	}
	else if (if_lfo == 1)
	{
		if (type == LFO_UNSAVE_WARN)
		{
			ntime = MonSys.start_time;
			tmpstring = PT_GetTimeStringBySeconds(ntime);

			if (m_MonSys.lfo_type == 1)//小于0.2Hz 发低频振荡告警，大于0.2Hz 发大幅功率波动告警
			{
				tmpstring += " 低频振荡告警,";
			}
			else if (m_MonSys.lfo_type == 2)
			{
				tmpstring += " 大幅功率波动告警,";
			}

			tmpstring += MonSys.lfo_desc;

			//yjj		if (PODB_GetHostName(hostname,m_LfoPara.warn_app_no)!=1)
			if (PODB_GetHostName(hostname, sizeof(hostname), m_LfoPara.warn_app_no, g_ctxNo) != 1)
			{
				return 0;
			}
			sprintf(m_LogString, "低频振荡告警:host[%s],fac_id["INT_REC_FMT"],desc[%s]", hostname, INT_REC_VAL(m_LfoPara.lfo_fac_id), tmpstring.c_str());
			MessShow(m_LogString, LTINFO);
		}
		else if (type == LFO_SAVE_WARN)
		{
			ntime = end_time;
			tmpstring = PT_GetTimeStringBySeconds(ntime);
			if (m_MonSys.lfo_type == 1)//小于0.2Hz 发低频振荡告警，大于0.2Hz 发大幅功率波动告警
			{
				tmpstring += " 低频振荡结束。";
			}
			else if (m_MonSys.lfo_type == 2)
			{
				tmpstring += " 大幅功率波动结束。";
			}

			if (PODB_GetHostName(hostname, sizeof(hostname), m_LfoPara.warn_app_no, g_ctxNo) != 1)
			{
				return 0;
			}
			sprintf(m_LogString, "低频振荡结束:host[%s],fac_id["INT_REC_FMT"],desc[%s]", hostname, INT_REC_VAL(m_LfoPara.lfo_fac_id), tmpstring.c_str());
			MessShow(m_LogString, LTINFO);
		}

	}

#ifdef _D5000
	if (if_lfo == 0)
	{
		tmpMsg.seq_warn_message[0].warn_type = warn_type;
	}
	else if (if_lfo == 1)
	{
		tmpMsg.seq_warn_message[0].warn_type = WAMS_LFO_LPO_WARN_TYPE;
	}
#else	
	tmpMsg.seq_warn_message[0].warn_type = WAMS_LFO_WARN_TYPE;
#endif
	tmpMsg.seq_warn_message[0].app_no = g_appNo;
	tmpMsg.seq_warn_message[0].node_id = 0;
	tmpMsg.seq_warn_message[0].op_type = 0;
	tmpMsg.seq_warn_message[0].is_restrain = 0;
	tmpMsg.seq_warn_message[0].sound_table_key_field_order_no = -1;
	tmpMsg.seq_warn_message[0].reservered_1 = 0;
	tmpMsg.seq_warn_message[0].reservered_2 = 0;
	tmpMsg.seq_warn_message[0].seq_field_info.length(5);
	tmpMsg.seq_warn_message[0].seq_field_info[0].c_time(ntime);
	// 	tmpMsg.seq_warn_message[0].seq_field_info[1].c_int(MonSys.stat);
	// 	tmpMsg.seq_warn_message[0].seq_field_info[2].c_string((const char *)tmpstring.c_str());
	// #ifdef _D5000
	// 	tmpMsg.seq_warn_message[0].seq_field_info[3].c_long(m_LfoPara.lfo_fac_id);
	// 	tmpMsg.seq_warn_message[0].seq_field_info[4].c_long(0);
	// #else
	// 	tmpMsg.seq_warn_message[0].seq_field_info[3].c_int(m_LfoPara.lfo_fac_id);
	// 	tmpMsg.seq_warn_message[0].seq_field_info[4].c_int(0);
	// #endif
#ifdef _D5000
	tmpMsg.seq_warn_message[0].seq_field_info[1].c_long(0);
	tmpMsg.seq_warn_message[0].seq_field_info[2].c_long(m_LfoPara.lfo_fac_id);
	tmpMsg.seq_warn_message[0].seq_field_info[3].c_int(MonSys.stat);
	tmpMsg.seq_warn_message[0].seq_field_info[4].c_string((const char*)tmpstring.c_str());
#else
	tmpMsg.seq_warn_message[0].seq_field_info[1].c_int(MonSys.stat);
	tmpMsg.seq_warn_message[0].seq_field_info[2].c_string((const char*)tmpstring.c_str());
	tmpMsg.seq_warn_message[0].seq_field_info[3].c_int(m_LfoPara.lfo_fac_id);
	tmpMsg.seq_warn_message[0].seq_field_info[4].c_int(0);
#endif
	// 		sprintf(m_LogString,"[yjj]告警类型[%d],状态[%d]",tmpMsg.seq_warn_message[0].warn_type,MonSys.stat);
	// 		MessShow(m_LogString,LTINFO );


	char* msg_buf = NULL;
	int buf_size = 0, ret_code = 0;
#ifdef _D5000
	Message msg;
	Msg_destination* pp_tcp = new Msg_destination;
	M_CODE(tmpMsg, msg_buf, buf_size);
	msg.header.serv = APP_TO_WARN_CHANNEL;
	msg.header.event = APP_TO_WARN_SERVICE_TYPE;
	memcpy(msg.Msg_buf, msg_buf, buf_size);
	strcpy(pp_tcp->host_name, hostname);
	// 	strcpy(pp_tcp->context_name,AC_REALTIME_NAME);
	// 	strcpy(pp_tcp->app_name,AF_WAMS_NAME);
	// 	strcpy(pp_tcp->proc_name,"warnserver");
	pp_tcp->status = 0;
	//	pp_tcp->sock = 0;
	ret_code = PODB_messageSend((Message*)&msg, buf_size, pp_tcp);

	/*#ifdef _D5000
	vector<WARN_MESSAGE_STRUCT> tmpVecWarn;
	WARN_MESSAGE_STRUCT tmpWarn;

	tmpWarn.appno = g_appNo;
	tmpWarn.warntype = WAMS_LFO_WARN_TYPE;
#ifdef _D5000
	tmpWarn.warntype = WAMS_LFO_LPO_WARN_TYPE;
#else
	tmpWarn.warntype = WAMS_OSC_WARN_TYPE;
#endif
	PODB_WarnMsgBaseTime(tmpWarn,ntime);
	PODB_WarnMsgBaseINT(tmpWarn,MonSys.stat);
	PODB_WarnMsgBaseSTRING(tmpWarn,tmpstring);
#ifdef _D5000
	PODB_WarnMsgBaseLONG(tmpWarn,m_LfoPara.lfo_fac_id);
#else
	PODB_WarnMsgBaseINT(tmpWarn,m_LfoPara.lfo_fac_id);
#endif
	tmpVecWarn.push_back(tmpWarn);
#ifdef _PODB_WARN_
	PODB_SendWarnMessage(tmpVecWarn,g_appNo);
#endif
	ret_code = PODB_SendWarnMessage(tmpVecWarn,para_WarnAppNo);
*/
	SAFE_DELETE(pp_tcp);
	SAFE_DELETE(msg_buf);
	if (ret_code <= 0)
	{
		sprintf(m_LogString, "告警发送失败[%s],msgtype[%d]", hostname, APP_TO_WARN_SERVICE_TYPE);
		MessShow(m_LogString);
	}
	else
	{
		sprintf(m_LogString, "告警发送成功[%s],msgtype[%d]", hostname, APP_TO_WARN_SERVICE_TYPE);
		MessShow(m_LogString);
	}
#else
	MARSHAL(tmpMsg, msg_buf, buf_size);
	ret_code = PODB_SendMessageToHost(msg_buf, APP_TO_WARN_SERVICE_TYPE, buf_size, hostname, "powoscana", "WARN_SERVICE");
	SAFE_DELETE(msg_buf);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "告警发送失败[%s],msgtype[%d]", hostname, APP_TO_WARN_SERVICE_TYPE);
		MessShow(m_LogString);
	}
	else
	{
		sprintf(m_LogString, "告警发送成功[%s],msgtype[%d]", hostname, APP_TO_WARN_SERVICE_TYPE);
		MessShow(m_LogString);
	}
#endif  // _D5000
#endif  // _WAMAP
#endif // !_NUSP
	return 0;
}


//向科东平台发送告警

int CLfoAnaObj::SendWarnToIA(LFO_SYS_INFO& MonSys, const int mode)
{
#ifndef _NUSP
	// 填充告警结构体
	LOW_FRE_OSCI_INFO_TO_IA_STRU tmpLFOIA;
	memset((void*)&tmpLFOIA, 0, sizeof(LOW_FRE_OSCI_INFO_TO_IA_STRU));
	tmpLFOIA.search_id = MonSys.lead_dev.dev.fac_id;
	tmpLFOIA.msec = 0;
	tmpLFOIA.dev_id = MonSys.lead_dev.dev.dev_id;
	tmpLFOIA.st_id = MonSys.lead_dev.dev.fac_id;
	tmpLFOIA.app_id = g_appNo;
	tmpLFOIA.criterion_num = MonSys.vec_dev.size();
	tmpLFOIA.damp_rate = MonSys.lead_dev.mode.damp;
	tmpLFOIA.oscillation_fre = MonSys.lead_dev.mode.freq;
#ifdef _KEDONG_
	if (tmpLFOIA.oscillation_fre < m_LfoPara.lfo_freq_min)
	{
		sprintf(tmpLFOIA.dev_descr, "发生超低频振荡,频率%.2f", tmpLFOIA.oscillation_fre);
	}
	else if (tmpLFOIA.oscillation_fre > m_LfoPara.lfo_freq_max)
	{
		sprintf(tmpLFOIA.dev_descr, "发生高频振荡,频率%.2f", tmpLFOIA.oscillation_fre);
	}
	else if (tmpLFOIA.oscillation_fre >= m_LfoPara.lfo_freq_min && tmpLFOIA.oscillation_fre <= m_LfoPara.lfo_freq_max)
	{
		sprintf(tmpLFOIA.dev_descr, "发生低频振荡,频率%.2f", tmpLFOIA.oscillation_fre);
	}
#else
	sprintf(tmpLFOIA.dev_descr, "%s", MonSys.lead_dev.dev.dev_name);
#endif
	if (mode == LFO_UNSAVE_WARN)
	{
		tmpLFOIA.occur_time = MonSys.start_time;
		tmpLFOIA.osci_id = MonSys.lead_dev.dev.dev_id;
		tmpLFOIA.status = 1;
	}
	else if (mode == LFO_SAVE_FLAG_HOLD)
	{
		tmpLFOIA.osci_id = 1;
		tmpLFOIA.status = 2;
	}
	else if (mode == LFO_SAVE_FLAG_END)
	{
		tmpLFOIA.osci_id = 1;
		tmpLFOIA.status = 3;
		tmpLFOIA.end_time = MonSys.end_time;
		sprintf(tmpLFOIA.dev_descr, "功率振荡结束");
	}


	sprintf(m_LogString, "综合智能告警内容:[%s]", tmpLFOIA.dev_descr);
	MessShow(m_LogString);
	tmpLFOIA.oscillation_value = MonSys.lead_dev.mode.ampl;
	tmpLFOIA.phase_angle = MonSys.lead_dev.mode.phas;

	tmpLFOIA.continus_flag = 0;

	OSCI_DEV_INFO_STRU tmpDev;
	vector<OSCI_DEV_INFO_STRU> tmpVecDev;
	int ii;
	for (ii = 0; ii < MonSys.vec_dev.size(); ii++)
	{
		memset((void*)&tmpDev, 0, sizeof(OSCI_DEV_INFO_STRU));
		//tmpDev.osci_id = m_pGnResSave[ii].obj_id;
		tmpDev.key_id = MonSys.vec_dev[ii].dev.dev_id;
		// 机组分群默认置为1
		tmpDev.group_num = 1;
		sprintf(tmpDev.descr, "%s", MonSys.vec_dev[ii].dev.dev_name);
		tmpDev.damp_rate = MonSys.vec_dev[ii].mode.damp;
		tmpDev.oscillation_fre = MonSys.vec_dev[ii].mode.freq;
		tmpDev.oscillation_value = MonSys.vec_dev[ii].mode.ampl;
		tmpDev.phase_angle = MonSys.vec_dev[ii].mode.phas;

		tmpVecDev.push_back(tmpDev);
	}
	// 	for ( ii =0; ii<m_nLnNum; ii++ )
	// 	{
	// 		if (m_pLnResSave[ii].lfo_b_t > 0)
	// 		{
	// 			memset((void *)&tmpDev,0,sizeof(OSCI_DEV_INFO_STRU));
	// 			//tmpDev.osci_id = m_pGnResSave[ii].obj_id;
	// 			tmpDev.key_id = m_pLnResSave[ii].obj_id;
	// 			// 线路分群默认置为0
	// 			tmpDev.group_num = 0;
	// 			sprintf(tmpDev.descr,"%s",m_pLnResSave[ii].obj_name);
	// 			if (m_pLnResSave[ii].mode1_num >= 1)
	// 			{
	// 				tmpDev.damp_rate = m_pLnResSave[ii].mode1[m_pLnResSave[ii].mode1_num-1].damp;
	// 				tmpDev.oscillation_fre = m_pLnResSave[ii].mode1[m_pLnResSave[ii].mode1_num-1].freq;
	// 				tmpDev.oscillation_value = m_pLnResSave[ii].mode1[m_pLnResSave[ii].mode1_num-1].ampl;
	// 				tmpDev.phase_angle = m_pLnResSave[ii].mode1[m_pLnResSave[ii].mode1_num-1].phase;
	// 			}
	// 			
	// 			tmpVecDev.push_back(tmpDev);
	// 		}
	// 	}
	// 	for ( ii =0; ii<m_nTrNum; ii++ )
	// 	{
	// 		if (m_pTrResSave[ii].lfo_b_t > 0)
	// 		{
	// 			memset((void *)&tmpDev,0,sizeof(OSCI_DEV_INFO_STRU));
	// 			//tmpDev.osci_id = m_pGnResSave[ii].obj_id;
	// 			tmpDev.key_id = m_pTrResSave[ii].obj_id;
	// 			// 变压器分群默认置为0
	// 			tmpDev.group_num = 0;
	// 			sprintf(tmpDev.descr,"%s",m_pTrResSave[ii].obj_name);
	// 			if (m_pTrResSave[ii].mode1_num >= 1)
	// 			{
	// 				tmpDev.damp_rate = m_pTrResSave[ii].mode1[m_pTrResSave[ii].mode1_num-1].damp;
	// 				tmpDev.oscillation_fre = m_pTrResSave[ii].mode1[m_pTrResSave[ii].mode1_num-1].freq;
	// 				tmpDev.oscillation_value = m_pTrResSave[ii].mode1[m_pTrResSave[ii].mode1_num-1].ampl;
	// 				tmpDev.phase_angle = m_pTrResSave[ii].mode1[m_pTrResSave[ii].mode1_num-1].phase;
	// 			}
	// 			
	// 			tmpVecDev.push_back(tmpDev);
	// 		}
	// 	}

	tmpLFOIA.criterion_num = (int)tmpVecDev.size();

	sprintf(m_LogString, "search_id["INT_REC_FMT"],occur_time[%d],end_time[%d],dev_id["INT_REC_FMT"],dev_descr[%s]",
		INT_REC_VAL(tmpLFOIA.search_id), tmpLFOIA.occur_time, tmpLFOIA.end_time, INT_REC_VAL(tmpLFOIA.dev_id), tmpLFOIA.dev_descr);
	MessShow(m_LogString, LTDEBUG);
	sprintf(m_LogString, "st_id["INT_REC_FMT"],app_id[%d],criterion_num[%d],osci_id["INT_REC_FMT"],status[%d]",
		INT_REC_VAL(tmpLFOIA.st_id), tmpLFOIA.app_id, tmpLFOIA.criterion_num, INT_REC_VAL(tmpLFOIA.osci_id), tmpLFOIA.status);
	MessShow(m_LogString, LTDEBUG);
	sprintf(m_LogString, "damp_rate[%f],oscillation_fre[%f],oscillation_value[%f],phase_angle[%f],continus_flag[%d]",
		tmpLFOIA.damp_rate, tmpLFOIA.oscillation_fre, tmpLFOIA.oscillation_value, tmpLFOIA.phase_angle, tmpLFOIA.continus_flag);
	MessShow(m_LogString, LTDEBUG);
	sprintf(m_LogString, "no,osci_id,key_id,group_num,group_name,descr,damp_rate,oscillation_fre,oscillation_value,phase_angle");
	MessShow(m_LogString, LTDEBUG);
	for (ii = 0; ii < (int)tmpVecDev.size(); ii++)
	{
		sprintf(m_LogString, "%d,"INT_REC_FMT","INT_REC_FMT",%d,%s,%s,%f,%f,%f,%f", ii + 1,
			INT_REC_VAL(tmpVecDev[ii].osci_id), INT_REC_VAL(tmpVecDev[ii].key_id), tmpVecDev[ii].group_num, tmpVecDev[ii].group_name,
			tmpVecDev[ii].descr, tmpVecDev[ii].damp_rate, tmpVecDev[ii].oscillation_fre, tmpVecDev[ii].oscillation_value, tmpVecDev[ii].phase_angle);
		MessShow(m_LogString, LTDEBUG);
	}

#ifdef _WAMAP_
	sprintf(m_LogString, "向综合智能告警发送告警消息");
	MessShow(m_LogString);
	// 判断值班主机
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
#ifdef _D5000
	Message msg;
	size_t buf_size = 0;
	memset(msg.Msg_buf, 0, sizeof(msg.Msg_buf));
	msg.header.event = LOW_FRE_OSCI_INFO_TO_IA;
	msg.header.serv = CH_TRANS_EVENT;

	memcpy((void*)(msg.Msg_buf + buf_size), (void*)&tmpLFOIA, sizeof(LOW_FRE_OSCI_INFO_TO_IA_STRU));
	buf_size += sizeof(LOW_FRE_OSCI_INFO_TO_IA_STRU);

	for (ii = 0; ii < (int)tmpVecDev.size(); ii++)
	{
		if ((buf_size + sizeof(OSCI_DEV_INFO_STRU)) >= MAX_MSGBUF_LEN)
		{
			sprintf(m_LogString, "消息内存越界[%ld]>[%d],截取前[%ld]", (buf_size + sizeof(OSCI_DEV_INFO_STRU)), MAX_MSGBUF_LEN, buf_size);
			MessShow(m_LogString, LTERROR);
			break;
		}

		OSCI_DEV_INFO_STRU& tmpDevStruct = *((OSCI_DEV_INFO_STRU*)(msg.Msg_buf + buf_size));
		tmpDevStruct = tmpVecDev[ii];
		buf_size += sizeof(OSCI_DEV_INFO_STRU);
	}

	if (PODB_messageSend((Message*)&msg, buf_size, NULL) < 0)
	{
		sprintf(m_LogString, "综合智能告警电网振荡告警消息发送失败");
		MessShow(m_LogString, LTINFO);
	}
	else
	{
		sprintf(m_LogString, "综合智能告警电网振荡告警消息发送成功");
		MessShow(m_LogString, LTINFO);
	}
#endif
#endif
#endif // !_NUSP
	return 0;

}

int CLfoAnaObj::SetEmsStat(LFO_SYS_INFO& MonSys)
{
#ifdef _WAMAP_
	if (m_LfoPara.is_triems == MENU_LFO_MENU_NO)
	{
		sprintf(m_LogString, "是否向EMS发送告警[%d],退出", m_LfoPara.is_triems);
		MessShow(m_LogString, LTINFO);
		return 0;
	}
#ifdef _NUSP
	INT_REC key;
	int colid;
#else
	INT_REC key;
	short colid;
#endif // _NUSP

#ifdef _D5000
	GetKeyIdInfo(m_LfoPara.ems_keyid, key, colid);
#endif

	if (!VALID_INT_REC(key))
	{
		sprintf(m_LogString, "EMS触发keyid错误["INT_REC_FMT"]", INT_REC_VAL(key));
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	//	if (m_LfoPara.ems_keyid.record_id<=0)
	//	{
	//		sprintf(m_LogString,"EMS触发keyid错误[%d]",m_LfoPara.ems_keyid.record_id);
	//		MessShow( m_LogString,LTERROR);
	//		return -1;
	//	}

#ifndef _LFO_HIS_LIB_
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return -1;
	}
#endif
	unsigned char triEms_value = 1;  // 南网开关状态，安全状态下合[1]，告警状态下分[0]
	int table_no = PODB_GetTabelNoByDevId(key);
	//    int table_no = PODB_GetTabelNoByDevId(m_LfoPara.ems_keyid.record_id);
	if (MonSys.stat >= MENU_LFO_DEV_STS_WRN)
	{
		triEms_value = 0;
		//		sprintf( m_LogString, "向EMS发送告警: table[%d]record[%d]column[%d]",
		//			table_no, m_LfoPara.ems_keyid.record_id, m_LfoPara.ems_keyid.column_id ); 
		sprintf(m_LogString, "向EMS发送告警: table[%d]record["INT_REC_FMT"]column[%d]",
			table_no, INT_REC_VAL(key), colid);
		MessShow(m_LogString, LTDEBUG);
	}

	//	int ret_code = PODB_TableModifyByKey((const char *)&m_LfoPara.ems_keyid.record_id, m_LfoPara.ems_keyid.column_id, (const char *)&triEms_value, sizeof(triEms_value), table_no);
	int ret_code = PODB_TableModifyByKey((const char*)&key, colid, (const char*)&triEms_value, sizeof(triEms_value), table_no);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "更新EMS开关状态错误table[%d]record["INT_REC_FMT"]column[%d]",
			table_no, INT_REC_VAL(key), colid);
		//        sprintf(m_LogString, "更新EMS开关状态错误table[%d]record[%d]column[%d]",
		//            table_no, m_LfoPara.ems_keyid.record_id, m_LfoPara.ems_keyid.column_id ) ;
		MessShow(m_LogString);
		return -1;
	}

#endif
	return 0;
}

int CLfoAnaObj::TmdAddDevInfo(vector<LFO_DEV_INFO>& VecDev, vector<string>& VecString, int devtype)
{
	if (devtype == MENU_LFO_DEV_GEN)
	{
		VecString.push_back("<p align=\"center\">&nbsp;振荡机组</p>");
	}
	else if (devtype == MENU_LFO_DEV_LINE)
	{
		VecString.push_back("<p align=\"center\">&nbsp;振荡线路</p>");
	}
	else if (devtype == MENU_LFO_DEV_TR)
	{
		VecString.push_back("<p align=\"center\">&nbsp;振荡变压器</p>");
	}
	else
	{
		VecString.push_back("<p align=\"center\">&nbsp;振荡设备</p>");
	}

	VecString.push_back("<div align=\"center\">");
	VecString.push_back("<center>");

	// 表头
	VecString.push_back("<table border=\"1\" id=\"table1\"><tr>");
	VecString.push_back("<td align=\"center\" valign=\"center\"><b>序号</b></td>");
	VecString.push_back("<td align=\"center\" valign=\"center\"><b>设备名称</b></td>");
	VecString.push_back("<td align=\"center\"><b>告警时间</b></td>");
	VecString.push_back("<td align=\"center\"><b>消失时间</b></td>");
	VecString.push_back("<td align=\"center\"><b>持续时间</b></td>");
	VecString.push_back("<td align=\"center\"><b>平均频率</b><p><b>(Hz)</b></td>");
	VecString.push_back("<td align=\"center\"><b>最大值</b><p><b>(MW)</b></td>");
	VecString.push_back("<td align=\"center\"><b>对应时刻</b></td>");
	VecString.push_back("<td align=\"center\"><b>最小值</b><p><b>(MW)</b></td>");
	VecString.push_back("<td align=\"center\"><b>对应时刻</b></td>");
	VecString.push_back("<td align=\"center\"><b>峰峰值</b><p><b>(MW)</b></td>");
	VecString.push_back("</tr>");

	int ii, jj, nDevNum = 0;
	char tmpString[1024];
	float aver_freq;
	for (ii = 0; ii < (int)VecDev.size(); ii++)
	{
		if (VecDev[ii].dev.dev_type != devtype)
		{
			continue;
		}

		VecString.push_back("<tr>");
		// 序号
		sprintf(tmpString, "<td align=\"center\">%d</td>", ++nDevNum);
		VecString.push_back(tmpString);
		// 设备名称
		if (devtype == MENU_LFO_DEV_GEN || devtype == MENU_LFO_DEV_TR)
		{
			sprintf(tmpString, "<td>%s%s</td>", VecDev[ii].dev.fac_name, VecDev[ii].dev.dev_name);
		}
		else
		{
			sprintf(tmpString, "<td>%s</td>", VecDev[ii].dev.dev_name);
		}
		VecString.push_back(tmpString);

		// 告警时间				
		sprintf(tmpString, "<td>%s</td>", PT_GetHMSTimeStringBySeconds(VecDev[ii].start_time));
		VecString.push_back(tmpString);
		// 消失时间
		sprintf(tmpString, "<td>%s</td>", PT_GetHMSTimeStringBySeconds(VecDev[ii].end_time));
		VecString.push_back(tmpString);
		// 持续时间
		sprintf(tmpString, "<td>%s</td>", PT_GetStringOfTimeInterval(VecDev[ii].hold_time * 1000));
		VecString.push_back(tmpString);
		// 平均频率
		aver_freq = 0.0f;
		if (!VecDev[ii].vec_lead_mode.empty())
		{
			for (jj = 0; jj < (int)VecDev[ii].vec_lead_mode.size(); jj++)
			{
				aver_freq += VecDev[ii].vec_lead_mode[jj].freq;
			}
			aver_freq = aver_freq / (int)VecDev[ii].vec_lead_mode.size();
		}
		sprintf(tmpString, "<td align=\"center\">%6.3f</td>", aver_freq);
		VecString.push_back(tmpString);
		// 最大值
		sprintf(tmpString, "<td align=\"center\">%6.2f</td>", VecDev[ii].curve.max_val);
		VecString.push_back(tmpString);
		// 最大值时刻				
		sprintf(tmpString, "<td>%s</td>", PT_GetHMSTimeStringBySeconds(VecDev[ii].curve.max_val_t));
		VecString.push_back(tmpString);
		// 最小值
		sprintf(tmpString, "<td align=\"center\">%6.2f</td>", VecDev[ii].curve.min_val);
		VecString.push_back(tmpString);
		// 最小值时刻				
		sprintf(tmpString, "<td>%s</td>", PT_GetHMSTimeStringBySeconds(VecDev[ii].curve.min_val_t));
		VecString.push_back(tmpString);
		// 峰峰值
		sprintf(tmpString, "<td align=\"center\">%6.2f</td>", (VecDev[ii].curve.max_val - VecDev[ii].curve.min_val));
		VecString.push_back(tmpString);

		VecString.push_back("</tr>");
	}

	VecString.push_back("</table>");
	VecString.push_back("</div>");
	VecString.push_back("</center>");

	return 0;
}

int CLfoAnaObj::TmdAddModeInfo(vector<LFO_DEV_INFO>& VecDev, vector<string>& VecString, int devtype)
{
	if (devtype == MENU_LFO_DEV_GEN)
	{
		VecString.push_back("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">振荡机组详细信息</font></b>]</p>");
	}
	else if (devtype == MENU_LFO_DEV_LINE)
	{
		VecString.push_back("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">振荡线路详细信息</font></b>]</p>");
	}
	else if (devtype == MENU_LFO_DEV_TR)
	{
		VecString.push_back("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">振荡变压器详细信息</font></b>]</p>");
	}
	else
	{
		VecString.push_back("<hr><p>　</p><p align=\"left\">[<b><font size=\"4\">振荡设备详细信息</font></b>]</p>");
	}

	int ii, jj;
	char tmpString[1024], tmptime1[20], tmptime2[20];
	for (ii = 0; ii < (int)VecDev.size(); ii++)
	{
		if (VecDev[ii].dev.dev_type != devtype)
		{
			continue;
		}

		sprintf(tmpString, "<p align=\"center\">%s%s主导模式</p>", VecDev[ii].dev.fac_name, VecDev[ii].dev.dev_name);
		VecString.push_back(tmpString);

		VecString.push_back("<div align=\"center\"><table border=\"1\" id=\"table1\">");
		VecString.push_back("<tr><td align=\"center\">&nbsp;序号&nbsp; </td>");
		VecString.push_back("<td align=\"center\">&nbsp;分析时段&nbsp;</td>");
		VecString.push_back("<td align=\"center\">&nbsp;主导模式幅值(MW)&nbsp;</td>");
		VecString.push_back("<td align=\"center\">&nbsp;频率(Hz)&nbsp;</td>");
		VecString.push_back("<td align=\"center\">&nbsp;阻尼比(%)&nbsp;</td>");
		VecString.push_back("<td align=\"center\">&nbsp;相角(deg)&nbsp;</td>");

		for (jj = 0; jj < (int)VecDev[ii].vec_lead_mode.size(); jj++)
		{
			sprintf(tmptime1, "%s", PT_GetHMSTimeStringBySeconds(VecDev[ii].vec_lead_mode[jj].start_time));
			sprintf(tmptime2, "%s", PT_GetHMSTimeStringBySeconds(VecDev[ii].vec_lead_mode[jj].end_time));
			sprintf(tmpString, "<tr><td align=\"center\">%4d</td><td align=\"center\">%8s&nbsp;-&nbsp;%8s</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.3f</td><td align=\"center\">%10.2f</td><td align=\"center\">%10.2f</td></tr>",
				jj + 1,
				tmptime1,
				tmptime2,
				VecDev[ii].vec_lead_mode[jj].ampl,
				VecDev[ii].vec_lead_mode[jj].freq,
				VecDev[ii].vec_lead_mode[jj].damp,
				VecDev[ii].vec_lead_mode[jj].phas);
			VecString.push_back(tmpString);
		}
		VecString.push_back("</table></div>");
	}
	return 0;
}

int CLfoAnaObj::TmdAddKeyId(vector<LFO_DEV_INFO>& VecDev, vector<KEY_ID_STRU>& VecKeyID, int devtype)
{
#ifndef _D5000
#ifdef _WAMAP_
	// 填充测点信息
	vector<int> VecColDev;
	if (devtype == MENU_LFO_DEV_GEN)
	{
		PODB_InitColID(VecColDev, GN_DEVICE_NO_TAB);
	}
	else if (devtype == MENU_LFO_DEV_LINE)
	{
		PODB_InitColID(VecColDev, ACLN_DOT_NO_TAB);
	}
	else if (devtype == MENU_LFO_DEV_TR)
	{
		PODB_InitColID(VecColDev, ACLN_DOT_NO_TAB);
	}

	// 保存全网设备数据
	KEY_ID_STRU  tmpkeyid;
	int ii, jj;
	for (ii = 0; ii < (int)VecDev.size(); ii++)
	{
		memset((void*)&tmpkeyid, 0, sizeof(KEY_ID_STRU));
		tmpkeyid.record_id = VecDev[ii].dev.dev_id;
		if (VecDev[ii].dev.dev_type == devtype)
		{
			for (jj = 0; jj < (int)VecColDev.size(); jj++)
			{
				if (VecColDev[jj] >= 0)
				{
					tmpkeyid.column_id = VecColDev[jj];
					VecKeyID.push_back(tmpkeyid);
				}
			}
		}
	}
#endif
#endif
	return 0;
}

int CLfoAnaObj::ShowWamsCurve(LFO_SYS_INFO& MonSys)
{
	if (m_LfoPara.is_lfo_curve == 0)
	{
		sprintf(m_LogString, "不调用曲线控件[%d]", m_LfoPara.is_lfo_curve);
		MessShow(m_LogString, LTINFO);
		return 0;
	}

	int DevPowCol = 0;
#ifdef _WAMAP_
#ifndef _LFO_HIS_LIB_
	if (PODB_IsOnDuty(g_appNo, g_ctxNo) != 1)
	{
		return 0;
	}
#endif
	int table_no = PODB_GetTabelNoByDevId(MonSys.lead_dev.dev.dot_id);
	if (table_no == GN_DEVICE_NO_TAB)
	{
		DevPowCol = PODB_GetFieldNoByName(GN_DEVICE_NO_TAB, P_VAL, GN_p);
	}
	else if (table_no == ACLN_DOT_NO_TAB)
	{
		DevPowCol = PODB_GetFieldNoByName(ACLN_DOT_NO_TAB, P_VAL, ACLN_p);
	}
	else if (table_no == TRWD_DEVICE_NO_TAB)
	{
		DevPowCol = PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB, P_VAL, TRWD_p);
	}
	else
	{
		sprintf(m_LogString, "表号错误[%d],dev["INT_REC_FMT"]", table_no, INT_REC_VAL(MonSys.lead_dev.dev.dev_id));
		MessShow(m_LogString, LTERROR);
		return -1;
	}
#endif
	// WamsCurve  -send -devname 设备名称 -app_key_id app_no/key_id/col_id -start 开始时间  -end 结束时间
	char tmpString[1024];
	sprintf(tmpString, "WamsCurve -send -devname %s%s -app_key_id %d/"INT_REC_FMT"/%d &",
		MonSys.lead_dev.dev.dot_fac_name, MonSys.lead_dev.dev.dot_dev_name, g_appNo, INT_REC_VAL(MonSys.lead_dev.dev.dot_id), DevPowCol
	);
	sprintf(m_LogString, "显示曲线:%s", tmpString);
	MessShow(m_LogString, LTINFO);
	system(tmpString);

	return 0;
}

int CLfoAnaObj::ClearLfoCase(LFO_CASE_INFO& tmpLfoCase)
{
	tmpLfoCase.event_id = 0;
	tmpLfoCase.stat = 0;
	memset(tmpLfoCase.case_desc, 0, sizeof(tmpLfoCase.case_desc));
	tmpLfoCase.lfo_gn_num = 0;
	tmpLfoCase.lfo_ln_num = 0;
	tmpLfoCase.lfo_tr_num = 0;
	tmpLfoCase.lfo_inf_num = 0;
	if (!tmpLfoCase.vec_windows.empty())
	{
		ClearLfoSysInfo(tmpLfoCase.vec_windows);
	}
	tmpLfoCase.start_time = 0;
	tmpLfoCase.end_time = 0;
	tmpLfoCase.hold_time = 0;
	ClearLfoModeInfo(tmpLfoCase.lead_mode);
	ClearLfoDev(tmpLfoCase.warn_dev);
	if (!tmpLfoCase.vec_dev.empty())
	{
		ClearLfoDev(tmpLfoCase.vec_dev);
	}
	if (!tmpLfoCase.cen_dev.empty())
	{
		ClearLfoDev(tmpLfoCase.cen_dev);
	}
	if (!tmpLfoCase.sou_dev.empty())
	{
		ClearLfoDev(tmpLfoCase.sou_dev);
	}
	if (!tmpLfoCase.vec_mainmode.empty())
	{
		ClearLfoModeShape(tmpLfoCase.vec_mainmode);
	}

	memset(tmpLfoCase.lfos_gn_name, 0, sizeof(tmpLfoCase.lfos_gn_name));
	tmpLfoCase.lfos_gn_num = 0;	/* 振荡源机组数目 */
	tmpLfoCase.lfos_ln_num = 0;	/* 振荡源线路数目 */
	tmpLfoCase.lfos_tr_num = 0;	/* 振荡源变压器数目 */
	tmpLfoCase.lfo_modle = 0;
	tmpLfoCase.lfo_type = 0;
	tmpLfoCase.is_lfo_source = 0;
	memset(tmpLfoCase.lfos_des, 0, sizeof(tmpLfoCase.lfos_des));
	memset(tmpLfoCase.lfo_ana_des, 0, sizeof(tmpLfoCase.lfo_ana_des));
	memset(tmpLfoCase.center_desc, 0, sizeof(tmpLfoCase.center_desc));
	memset(tmpLfoCase.type_desc, 0, sizeof(tmpLfoCase.type_desc));
	memset(tmpLfoCase.asst_desc, 0, sizeof(tmpLfoCase.asst_desc));
	return 0;
}

int CLfoAnaObj::ClearCaseFile(LFO_CASE_FILE_STRUCT& tmpCaseFile)
{
	tmpCaseFile.eve_id = 0;  // 事件ID
	tmpCaseFile.start_time = 0;  // 振荡起始时间
	tmpCaseFile.end_time = 0;  // 振荡截止时间
	tmpCaseFile.hold_time = 0;  // 持续时间
	tmpCaseFile.Data_Save_start_time = 0; //数据保存起始时间
	tmpCaseFile.Data_Save_end_time = 0; //数据保存起始时间
	ZERO_INT_REC(tmpCaseFile.warn_dev_id);  // 触发告警设备ID
	tmpCaseFile.warn_freq = 0;  // 触发告警频率
	tmpCaseFile.max_ampl = 0;  // 振荡最大幅值
	tmpCaseFile.lfo_modle = 0;	/* 振荡模式 */
	tmpCaseFile.lfo_type = 0;	/* 振荡类型 */
	memset(tmpCaseFile.version, 0, sizeof(tmpCaseFile.version));
	memset(tmpCaseFile.time, 0, sizeof(tmpCaseFile.time));
	memset(tmpCaseFile.system, 0, sizeof(tmpCaseFile.system));
	memset(tmpCaseFile.case_des, 0, sizeof(tmpCaseFile.case_des));
	memset(tmpCaseFile.warn_dev_name, 0, sizeof(tmpCaseFile.warn_dev_name));
	memset(tmpCaseFile.type_desc, 0, sizeof(tmpCaseFile.type_desc));
	vector<LFO_CASE_FILE_DEV>().swap(tmpCaseFile.vecdev);
	tmpCaseFile.vecdev.clear();

	return 0;
}

int CLfoAnaObj::ThreadCallBack_ERR(LFO_SYS_INFO& MonSys, LFO_PARA_STRUCT& LfoPara, const int threadidx, const int is_warn)
{
	MonSys.stat = MENU_LFO_DEV_STS_OLD;
	// 更新参数
	if (GetLfoPara(m_LfoPara, 1) < 0)
	{
		sprintf(m_LogString, "参数表读取失败,采用默认参数");
		MessShow(m_LogString, LTERROR);
	}

	if (CheckPara(m_LfoPara, -1) < 0)
	{
		sprintf(m_LogString, "参数检查错误,采用默认参数");
		MessShow(m_LogString, LTERROR);
	}

	if (is_warn == 1)
	{
		// 更新EMS告警开关
		SetEmsStat(m_MonSys);
		// 发送告警消息
//yjj临时修改
//		SendWarnMessage(MonSys,LFO_STSOLD_WARN);
	}

	//     实时结果结果保存
// 	sprintf(m_LogString,"[yjj]第二次调用函数UpdateLfoMonPara");
//     MessShow(m_LogString,LTERROR);
//  	if (UpdateLfoMonPara(MonSys)<0)
//  	{
//          sprintf(m_LogString,"更新LFO参数表监视信息错误");
//          MessShow(m_LogString,LTERROR);
//  	}

	return 0;
}


int CLfoAnaBase::LocateLfoSource(vector<LFO_DEV_INFO>& VecMonDev, LFO_SYS_INFO& MonSys)
{
	int  ii, jj;
	int                  nAllObjMode = 0;
	int                  nAllMode = 0;
	int				     nSltMode = 0;
	MODE_PART_OBJ* pAllObjMode;
	MODE_CHRCT* pAllMode;
	MODE_CHRCT* pSltMode;

	pAllObjMode = NULL;
	pAllMode = NULL;
	pSltMode = NULL;

	int para_LfoAnaByPwr = 0;

	float tmp_leadmode_freq;//系统主导模式频率,用于同模式判断

	if (!MonSys.vec_modeshape.empty())
	{
		tmp_leadmode_freq = MonSys.vec_modeshape[0].mode.freq;
		sprintf(m_LogString, "主导模式频率[%f]", tmp_leadmode_freq);
		MessShow(m_LogString);
	}

	para_nFcMaxMem = MODE_MAXFC;                       //  开内存厂站最大数
	para_nGnMaxMem = MODE_MAXGN;                       //  开内存机组最大数
	para_nLnMaxMem = MODE_MAXLN;                       //  开内存线路最大数
	para_nTrMaxMem = MODE_MAXLN;                       //  开内存变压器最大数
	para_nSysMaxMode = SYS_MODE_MAX;		      	       //  开内sysmode数
	para_nCaseMaxMem = para_nFcMaxMem + para_nGnMaxMem + para_nLnMaxMem + para_nTrMaxMem;
	para_nObjMaxMode = (para_nSysMaxMode * para_nCaseMaxMem);

	pAllObjMode = (MODE_PART_OBJ*)calloc(para_nObjMaxMode, sizeof(MODE_PART_OBJ));
	memset((void*)pAllObjMode, 0, para_nObjMaxMode * sizeof(MODE_PART_OBJ));

	pAllMode = (MODE_CHRCT*)calloc(para_nSysMaxMode, sizeof(MODE_CHRCT));
	memset((void*)pAllMode, 0, para_nSysMaxMode * sizeof(MODE_CHRCT));

	pSltMode = (MODE_CHRCT*)calloc(para_nSysMaxMode, sizeof(MODE_CHRCT));
	memset((void*)pSltMode, 0, sizeof(MODE_CHRCT) * para_nSysMaxMode);
	ShowProMem("振荡源申请内存");


	vector<LFO_DEV_INFO> cts_VecDev;
	ClearLfoDev(cts_VecDev);
	//		int tempint = (int)cts_VecDev.size();
	// 		sprintf(m_LogString,"【yjj】cts_VecDev大小1[%d]",tempint);
	// 		MessShow(m_LogString);

	LFO_DEV_INFO tmpLfoGn, tmpLfoLn, tmpLfoTr, temp_lfodev;
	ClearLfoDev(tmpLfoGn);
	//	sprintf(tmpLfoGn.dev.dev_name,"%s","未匹配");
	tmpLfoGn.dev.dev_type = MENU_LFO_DEV_GEN;
	ClearLfoDev(tmpLfoLn);
	//	sprintf(tmpLfoLn.dev.dev_name,"%s","未匹配");
	tmpLfoLn.dev.dev_type = MENU_LFO_DEV_LINE;
	ClearLfoDev(tmpLfoTr);
	//	sprintf(tmpLfoTr.dev.dev_name,"%s","未匹配");
	ClearLfoDev(temp_lfodev);
	tmpLfoTr.dev.dev_type = MENU_LFO_DEV_TR;
	vector<LFO_PRONY_INFO> tmpVecCase;
	ClearLfoPronyInfo1(tmpVecCase);

	RTDB_TIME StartTime = m_MonSys.start_time;
	RTDB_TIME EndTime = m_MonSys.end_time;
	int ordernum;
	int nNeedNum;
	nNeedNum = (EndTime - StartTime) * MAX_FRA_PER_SEC;
	int bak_kkk;
	// 	PUB_WT_VEC_DATA *pData = new PUB_WT_VEC_DATA[nNeedNum];
	// 	memset(pData,0,nNeedNum*sizeof(PUB_WT_VEC_DATA));
	float temp_p_ampl, temp_p_phas, temp_q_ampl, temp_q_phas, temp_V_ampl, temp_V_phas, temp_A_ampl, temp_A_phas;
	float delt_t, temp_pow, th0;
	int prony_flag = 0;

	// 	CDataFile df;
	// 	string tmpfilename;

	temp_p_ampl = 0.0;
	temp_p_phas = 0.0;
	temp_q_ampl = 0.0;
	temp_q_phas = 0.0;
	temp_V_ampl = 0.0;
	temp_V_phas = 0.0;
	temp_A_ampl = 0.0;
	temp_A_phas = 0.0;
	delt_t = 0.0;
	temp_pow = 0.0;
	int dii, djj, tableno, nColNum;
	string::size_type idx;
	string tmpdevname, tmpColName;
	string tmpstring;
	if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
	{
		//
	}
	else
	{
		//LocateLfoSource从数据文件中读取PMU数据
// 		FormDataFileName(tmpfilename);
// 		sprintf(m_LogString,"LocateLfoSource读取设备数据[%s],起始[%d]",tmpfilename.c_str(),m_LfoPara.data_pos_old);
// 		MessShow(m_LogString,LTINFO);			
// 		ShowProMem("LocateLfoSource-file");
// 		if (df.ReadFileData(tmpfilename.c_str(),m_LfoPara.data_pos_old,m_LfoPara.time_leng,m_LfoPara.int_time) == false)
// 		{
// 			sprintf(m_LogString,"读取数据文件失败[%s][%s]",tmpfilename.c_str(),df.GetLastError());
// 			MessShow(m_LogString,LTERROR);
// 
// 			SAFE_DELETE_ARRAY(pData);
// 			if ( pAllObjMode )     { free( pAllObjMode );   pAllObjMode = NULL; }
// 			if ( pAllMode )     { free( pAllMode );   pAllMode = NULL; }
// 			if ( pSltMode )     { free( pSltMode );   pSltMode = NULL; }
// 			df.ClearData();
// 
// 			return -1;
// 		}
// 		ShowProMem("LocateLfoSource-end");
	}

	int nsamemode;
	for (ii = 0; ii < (int)VecMonDev.size(); ii++)
	{
		// 		sprintf(m_LogString,"[yjj]提示1:起止时间=["INT_REC_FMT"]["INT_REC_FMT"],持续时间=[%d]!",VecMonDev[ii].start_time,VecMonDev[ii].end_time,VecMonDev[ii].hold_time_lfo);
		//  	MessShow(m_LogString,LTINFO);
		if (VecMonDev[ii].stat_prony == MENU_LFO_DEV_STS_OK)
		{
			if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				nsamemode = 0;
				for (int l = 0; l < VecMonDev[ii].vec_mode.size(); l++)
				{
					if (fabs(tmp_leadmode_freq - VecMonDev[ii].vec_mode[l].freq) > m_LfoPara.freq_diff)
					{//不是同模式,不做振荡源判断
						sprintf(m_LogString, "设备[%s]主导模式频率[%f],与系统主导模式频率[%f]之差大于[%f],不进行振荡源分析", VecMonDev[ii].dev.dev_name, VecMonDev[ii].vec_mode[l].freq, tmp_leadmode_freq, m_LfoPara.freq_diff);
						MessShow(m_LogString, LTERROR);
						continue;
					}
					else
					{
						if (nAllObjMode == para_nObjMaxMode)
						{
							break;
						}
						pAllObjMode[nAllObjMode].obj_id = VecMonDev[ii].dev.dev_id;
						sprintf(pAllObjMode[nAllObjMode].obj_name, "%s", VecMonDev[ii].dev.dev_name);
						pAllObjMode[nAllObjMode].type = VecMonDev[ii].dev.dev_type;
						pAllObjMode[nAllObjMode].damp = VecMonDev[ii].vec_mode[l].damp;
						pAllObjMode[nAllObjMode].freq = VecMonDev[ii].vec_mode[l].freq;
						pAllObjMode[nAllObjMode].ampl = VecMonDev[ii].vec_mode[l].ampl;
						pAllObjMode[nAllObjMode].phas = VecMonDev[ii].vec_mode[l].phas;
						pAllObjMode[nAllObjMode].real = VecMonDev[ii].vec_mode[l].real;
						pAllObjMode[nAllObjMode].imag = VecMonDev[ii].vec_mode[l].imag;
						pAllObjMode[nAllObjMode].engy = VecMonDev[ii].vec_mode[l].engy;
						nAllObjMode += 1;
						nsamemode = 1;
						break;
					}
				}
				if (nsamemode == 0)
				{
					sprintf(m_LogString, "设备[%s]有功功率prony分析模式中不含系统主导模式[%f],不进行振荡源分析", VecMonDev[ii].dev.dev_name, tmp_leadmode_freq);
					MessShow(m_LogString, LTERROR);
					continue;
				}
				//	prony_flag=0;
			}
			else if (VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_LINE || VecMonDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				// 获取设备电压相角数据
				tmpLfoLn = VecMonDev[ii];
				vector<float>().swap(tmpLfoLn.v2d_data);
				ordernum = 1;
				ClearLfoDev(temp_lfodev);
				ClearLfoDev(cts_VecDev);

				tmpLfoLn.stat = MENU_LFO_DEV_STS_OK;
				if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
				{
					GetDevDataFromDB(StartTime, EndTime, ordernum, tmpLfoLn);
				}
				else
				{
					if (VALID_INT_REC(tmpLfoLn.dev.dev_id))			
					{
						//修改读取数据模块
						for (djj = 0; djj < (int)m_df.m_DataHead.vec_devid_s.size(); djj++)
						{
							if (djj % 100 == 0)
							{
								usleep(5);
							}
							// 获取设备ID
// 							tmpstring = m_df.m_DataHead.vec_devid_s[djj];
							INT_REC tmpdev_id = m_df.m_DataHead.vec_devid_s[djj];
							if (tmpdev_id == tmpLfoLn.dev.dev_id)
							{
								// 去掉前两列"日期时间,毫秒"
								//				tmpstring.clear();
								if (m_df.GetNumberOfVariables() >= djj + 2)
								{
									m_df.GetVariableName(djj + 2, tmpstring);
								}
								idx = tmpstring.find_last_of("-");
								// 获取设备名称
								if (idx != string::npos)
								{
									tmpdevname = tmpstring.substr(0, idx);
								}
								// 判断测点类型
								tmpColName = tmpstring.substr(idx + 1);
								// 去掉每行末尾的","
								idx = tmpColName.find_first_of(",");
								if (idx != string::npos)
								{
									tmpColName = tmpColName.substr(0, idx);
								}
								if (tmpColName == "00P")
								{
									// 有功
									nColNum = 2;
								}
								else if (tmpColName == "00Q")
								{
									// 无功
									nColNum = 3;
								}
								else if (tmpColName == "UAV相角")
								{
									// 电压相角
									nColNum = 1;
								}
								else if (tmpColName == "UAV幅值")
								{
									// 电压幅值
									nColNum = 0;
								}
								else if (tmpColName == "EEV相角")
								{
									// 发电机功角
									nColNum = 16;
								}
								else
								{
									// 未定义，默认为无效
									sprintf(m_LogString, "设备[%s]参数域名错误[%s]", VecMonDev[ii].dev.dev_name, tmpColName.c_str());
									MessShow(m_LogString, LTERROR);
									nColNum = -1;
								}

								if (nColNum == ordernum)
								{
									// 获取设备数据
									if (m_df.GetData(djj + 2, tmpLfoLn.v2d_data) == -1)
									{
										sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
											tmpstring.c_str(), INT_REC_VAL(tmpdev_id), m_df.GetLastError());
										MessShow(m_LogString, LTDEBUG);
									}
									else
									{
										break;
									}
								}
							}
						}
						// 						if( m_df.GetData(tmpLfoLn.dev.yc_name[ordernum], tmpLfoLn.v2d_data) == -1)
						// 						{
						// 							// 置数据状态
						// 							tmpLfoGn.stat = MENU_LFO_DEV_STS_BAD;
						// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
						// 								tmpLfoLn.dev.dev_name,tmpLfoLn.dev.dev_id,tmpLfoLn.dev.yc_name[ordernum],m_df.GetLastError());
						// 							MessShow(m_LogString,LTERROR);
						// 						}
					}
				}

				temp_lfodev = tmpLfoLn;
				UnitAng(temp_lfodev.v2d_data);
				for (int kk = 0; kk < (int)temp_lfodev.v2d_data.size(); kk++)
				{
					delt_t = float(1.0 / MAX_FRA_PER_SEC); //单位s
					if (kk == 0)
					{
						th0 = 2.0 * temp_lfodev.v2d_data[1] - temp_lfodev.v2d_data[2];
						tmpLfoLn.v2d_data[kk] = (UnitAng2((temp_lfodev.v2d_data[1] - th0)) * PRONY_PI) / (delt_t * 180.0);  //单位rad/s			
					}
					else
					{
						tmpLfoLn.v2d_data[kk] = (UnitAng2((temp_lfodev.v2d_data[kk] - temp_lfodev.v2d_data[kk - 1])) * PRONY_PI) / (delt_t * 180.0);  //单位rad/s
						//tmpLfoLn.v2d_data[kk]=tmpLfoLn.v2d_data[kk-1];//近似取值			
					}
				}
				// 				for (int kk=0;kk<(int)tmpLfoLn.v2d_data.size();kk++)
				// 				{
				//  					int tempint = (int)tmpLfoLn.v2d_data.size();
				//  					sprintf(m_LogString,"线路[%s]角速度[%d]曲线：%.6f",tmpLfoLn.dev.dev_name,tempint,tmpLfoLn.v2d_data[kk]);
				//  					MessShow(m_LogString);
				//  				}				
				cts_VecDev.push_back(tmpLfoLn);
				if (DataFilter(cts_VecDev, 1) < 0)
				{
					sprintf(m_LogString, "数据滤波错误");
					MessShow(m_LogString, LTERROR);
					//	return -1;
				}
				if (DataSample(cts_VecDev, 1) < 0)
				{
					sprintf(m_LogString, "数据[支路角速度]采样错误");
					MessShow(m_LogString, LTERROR);
					// return -1;
				}
				//				IsDataValid(cts_VecDev); //数据合理性检查和突变检查

								/*vector<LFO_PRONY_INFO> tmpVecCase;*/
				ClearLfoPronyInfo1(tmpVecCase);
				if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
				{
					sprintf(m_LogString, "形成支路角速度计算数据错误");
					MessShow(m_LogString, LTERROR);
					// 					SAFE_DELETE_ARRAY(pData);
					if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
					if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
					if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
					m_df.ClearData();
					return -1;
				}
				if (PronyProcess(tmpVecCase) < 0)
				{
					sprintf(m_LogString, "进行支路角速度Prony分析错误");
					MessShow(m_LogString, LTERROR);
					// 					SAFE_DELETE_ARRAY(pData);
					if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
					if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
					if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
					m_df.ClearData();
					return -1;
				}
				for (int k = 0; k < (int)tmpVecCase.size(); k++)
				{
					if (tmpLfoLn.dev.dev_id == tmpVecCase[k].dev.dev_id)
					{
						if (!tmpVecCase[k].vec_mode.empty())
						{
							for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
							{
								if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
								{//不是同模式,不做振荡源判断
// 									sprintf(m_LogString,"设备[%s]角速度频率[%f],与主导模式频率[%f]之差大于[%f],不进行振荡源分析",VecMonDev[ii].dev.dev_name,tmpVecCase[k].vec_mode[l].freq,tmp_leadmode_freq,m_LfoPara.freq_diff);
// 									MessShow(m_LogString,LTERROR);
									continue;
								}
								else
								{
									if (nAllObjMode == para_nObjMaxMode)
									{
										break;
									}
									pAllObjMode[nAllObjMode].obj_id = VecMonDev[ii].dev.dev_id;
									sprintf(pAllObjMode[nAllObjMode].obj_name, "%s", VecMonDev[ii].dev.dev_name);
									pAllObjMode[nAllObjMode].type = VecMonDev[ii].dev.dev_type;
									pAllObjMode[nAllObjMode].damp = tmpVecCase[k].vec_mode.front().damp;
									pAllObjMode[nAllObjMode].freq = tmpVecCase[k].vec_mode.front().freq;
									pAllObjMode[nAllObjMode].ampl = tmpVecCase[k].vec_mode.front().ampl;
									pAllObjMode[nAllObjMode].phas = tmpVecCase[k].vec_mode.front().phas;
									pAllObjMode[nAllObjMode].real = tmpVecCase[k].vec_mode.front().real;
									pAllObjMode[nAllObjMode].imag = tmpVecCase[k].vec_mode.front().imag;
									pAllObjMode[nAllObjMode].engy = tmpVecCase[k].vec_mode.front().engy;
									nAllObjMode += 1;
									break;
								}
							}
							//	temp_A_ampl = tmpVecCase[k].vec_mode.front().ampl;
							//	temp_A_phas = tmpVecCase[k].vec_mode.front().phas;
							//	prony_flag=0;
						}
						else
						{
							sprintf(m_LogString, "Prony算法未分析出支路[%s]角速度符合要求的振荡模式,该支路不进行振荡源分析", tmpLfoLn.dev.dev_name);
							MessShow(m_LogString);
							//	prony_flag=1;
						}
					}
				}

			}
			sprintf(m_LogString, "第[%d]个设备[%s],类型[%d],状态[%d],振幅[%f],初相[%f],能量[%f],参与因子[%f].....", ii + 1, VecMonDev[ii].dev.dev_name, VecMonDev[ii].dev.dev_type, VecMonDev[ii].stat, VecMonDev[ii].mode.ampl, VecMonDev[ii].mode.phas, VecMonDev[ii].mode.engy, VecMonDev[ii].mode.fact);
			MessShow(m_LogString);
		}
	}
	ShowProMem("线路角速度");

	if (nAllObjMode > 0)
	{
		sprintf(m_LogString, "振荡源分析设备数目nAllObjMode=%d,开始进行振荡源定位......", nAllObjMode);
		MessShow(m_LogString);
		for (ii = 0; ii < nAllObjMode; ii++)
		{
			sprintf(m_LogString, "振荡源分析设备[%s],选取模式频率为[%f]", pAllObjMode[ii].obj_name, pAllObjMode[ii].freq);
			MessShow(m_LogString);
		}
	}
	else
	{
		sprintf(m_LogString, "振荡源分析设备数目nAllObjMode=%d,未挑选出需要分析振荡源的设备", nAllObjMode);
		MessShow(m_LogString);
	}
	g_lfoAnabycurve.IdtfPartGnLnTr(nAllObjMode, pAllObjMode, nAllMode, pAllMode);

	if (nAllObjMode > 0)
	{
		sprintf(m_LogString, "提示：振荡源分析设备数目nAllMode=%d", nAllMode);
		MessShow(m_LogString);
	}
	if (nAllMode > 0)
	{
		sprintf(m_LogString, "计算参与因子!");
		MessShow(m_LogString);
		// 		for (ii=0; ii<nAllMode; ii++)
		// 		{
		ii = 0;
		if (pAllMode[ii].nPartGn > 0)
		{
			if (para_LfoAnaByPwr == 0)
			{
				if (g_lfoAnabycurve.CalcPartFactor(pAllMode[ii].nPartGn, pAllMode[ii].PartGn, para_LfoAnaByPwr) == false)
					g_lfoAnabycurve.LfoAnaSetPartFact0(pAllMode[ii].nPartGn, pAllMode[ii].PartGn);
			}
			else
			{
				g_lfoAnabycurve.CalcPartFactor(pAllMode[ii].nPartGn, pAllMode[ii].PartGn);
			}
		}

		if (pAllMode[ii].nPartLn > 0)
		{
			if (para_LfoAnaByPwr == 0)
			{
				if (g_lfoAnabycurve.CalcPartFactor(pAllMode[ii].nPartLn, pAllMode[ii].PartLn, para_LfoAnaByPwr) == false)
					g_lfoAnabycurve.LfoAnaSetPartFact0(pAllMode[ii].nPartLn, pAllMode[ii].PartLn);
			}
			else
				g_lfoAnabycurve.CalcPartFactor(pAllMode[ii].nPartLn, pAllMode[ii].PartLn);
		}

		if (pAllMode[ii].nPartTr > 0)
		{
			if (para_LfoAnaByPwr == 0)
			{
				if (g_lfoAnabycurve.CalcPartFactor(pAllMode[ii].nPartTr, pAllMode[ii].PartTr, para_LfoAnaByPwr) == false)
					g_lfoAnabycurve.LfoAnaSetPartFact0(pAllMode[ii].nPartTr, pAllMode[ii].PartTr);
			}
			else
				g_lfoAnabycurve.CalcPartFactor(pAllMode[ii].nPartTr, pAllMode[ii].PartTr);
		}

		if (pAllMode[ii].nPartFc > 0)
		{
			g_lfoAnabycurve.CalcPartFactor(pAllMode[ii].nPartFc, pAllMode[ii].PartFc);
		}
		//		}
	}
	else
	{
		sprintf(m_LogString, "没有振荡源分析设备!");
		MessShow(m_LogString);
		// 		SAFE_DELETE_ARRAY(pData);
		if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
		if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
		if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
		m_df.ClearData();
		return 0;
	}
	ShowProMem("计算参与因子");

	// 模态筛选：
	if (nAllMode > 0)
	{
		//根据参与因子过滤设备
		g_lfoAnabycurve.filteDevByPartfact(nAllMode, pAllMode, nSltMode, pSltMode, m_LfoPara.sh_fact);
		for (jj = 0; jj < pSltMode[0].nPartLn; jj++)
		{
			m_VecDev[ii].mode.fact = pSltMode[0].PartLn[jj].fact;
			// 			sprintf( m_LogString, "[yjj0]线路[%s]参与因子=%f",pSltMode[0].PartLn[jj].obj_name,pSltMode[0].PartLn[jj].fact);
			// 			MessShow( m_LogString );
		}
		sprintf(m_LogString, "根据参与因子过滤设备，剩余设备数目=%d", nSltMode);
		MessShow(m_LogString);
		if (nSltMode > 0)
		{
			sprintf(m_LogString, "对剩余设备进行分群处理");
			MessShow(m_LogString);
			//		for (ii=0; ii<nSltMode; ii++)
			//		{
			ii = 0;
			// 基准选取（gn）
			g_lfoAnabycurve.IdtfCausLfoGn(pSltMode[ii].nPartGn, pSltMode[ii].PartGn, m_LfoPara.cohcy_angl, 0);
			g_lfoAnabycurve.IdtfCausLfoGn(pSltMode[ii].nPartLn, pSltMode[ii].PartLn, m_LfoPara.cohcy_angl, 0);
			g_lfoAnabycurve.IdtfCausLfoGn(pSltMode[ii].nPartTr, pSltMode[ii].PartTr, m_LfoPara.cohcy_angl, 0);
			// 暂不考虑厂站分群与发电机一致，暂不根据厂站进行线路和变压器分群
			// 			if ( pSltMode[ii].nPartGn > 0 )
			// 			{
			// 				g_lfoAnabycurve.IdtfLfoGnCohcyFc( pSltMode[ii].nPartFc, pSltMode[ii].PartFc, (MODE_PART_OBJ*)&pSltMode[ii].PartGn[0].obj_id, m_LfoPara.cohcy_angl );
			// 			}
			// 			else
			// 			{
			// 				g_lfoAnabycurve.IdtfCausLfoGn( pSltMode[ii].nPartFc, pSltMode[ii].PartFc, m_LfoPara.cohcy_angl );
			// 			}
			// 			// 通过Fc划分Ln/Tr所属群
			// 			if ( pSltMode[ii].nPartFc > 0 )
			// 			{
			// 				LfoAnaSetLnClustbyFc( pSltMode[ii].nPartLn, pSltMode[ii].PartLn, pSltMode[ii].nPartFc, pSltMode[ii].PartFc,VecMonDev );
			// 				LfoAnaSetTrClustbyFc( pSltMode[ii].nPartTr, pSltMode[ii].PartTr, pSltMode[ii].nPartFc, pSltMode[ii].PartFc,VecMonDev );
			// 			}
			//		}
		}
	}
	ShowProMem("模态筛选");
	for (jj = 0; jj < pSltMode[0].nPartLn; jj++)
	{
		m_VecDev[ii].mode.fact = pSltMode[0].PartLn[jj].fact;
		// 		sprintf( m_LogString, "[yjj1]线路[%s]参与因子=%f",pSltMode[0].PartLn[jj].obj_name,pSltMode[0].PartLn[jj].fact);
		// 		MessShow( m_LogString );
	}
	//获取机组、线路和变压器电压幅值、相角、无功功率
	if (nSltMode > 0)
	{

		// 		sprintf(m_LogString,"yjj开始时间["INT_REC_FMT"]结束时间["INT_REC_FMT"]频率[%d]",StartTime,EndTime,MAX_FRA_PER_SEC);
		// 			MessShow(m_LogString,LTINFO);

		for (jj = 0; jj < pSltMode[0].nPartLn; jj++)
		{
			m_VecDev[ii].mode.fact = pSltMode[0].PartLn[jj].fact;
			// 			sprintf( m_LogString, "[yjj11]线路[%s]参与因子=%f",pSltMode[0].PartLn[jj].obj_name,pSltMode[0].PartLn[jj].fact);
			// 			MessShow( m_LogString );
		}
		// 		sprintf(m_LogString,"1111111111111111111");
		// 		MessShow(m_LogString,LTERROR);		
		//		if(m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
		//		{	
		for (ii = 0; ii < pSltMode[0].nPartGn; ii++)
		{
			// 				sprintf(m_LogString,"22222222222222222222");
			// 				MessShow(m_LogString,LTERROR);		
			prony_flag = 0;
			bak_kkk = ii;
			for (jj = 0; jj < (int)VecMonDev.size(); jj++)
			{
				if (pSltMode[0].PartGn[ii].obj_id == VecMonDev[jj].dev.dev_id)
				{
					tmpLfoGn = VecMonDev[jj];
					vector<float>().swap(tmpLfoGn.v2d_data);
					break;
				}
			}

			// 				sprintf(m_LogString,"3333333333333333333333");
			// 				MessShow(m_LogString,LTERROR);

			ordernum = 0;
			ClearLfoDev(cts_VecDev);

			int tempint = (int)cts_VecDev.size();
			// 				sprintf(m_LogString,"【yjj】cts_VecDev大小2[%d]",tempint);
			// 				MessShow(m_LogString);			
			tmpLfoGn.stat = MENU_LFO_DEV_STS_OK;
			if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				GetDevDataFromDB(StartTime, EndTime, ordernum, tmpLfoGn);
			}
			else
			{
				// 					sprintf(m_LogString,"4444444444444444444444444");
				// 					MessShow(m_LogString,LTERROR);
				if (VALID_INT_REC(tmpLfoGn.dev.dev_id))
				{
					//修改读取数据模块
					for (djj = 0; djj < (int)m_df.m_DataHead.vec_devid_s.size(); djj++)
					{
						// 获取设备ID
// 							tmpstring = m_df.m_DataHead.vec_devid_s[djj];
						INT_REC tmpdev_id = m_df.m_DataHead.vec_devid_s[djj];
						if (tmpdev_id == tmpLfoGn.dev.dev_id)
						{
							// 去掉前两列"日期时间,毫秒"
							//				tmpstring.clear();
							if (m_df.GetNumberOfVariables() >= djj + 2)
							{
								m_df.GetVariableName(djj + 2, tmpstring);
							}
							idx = tmpstring.find_last_of("-");
							// 获取设备名称
							if (idx != string::npos)
							{
								tmpdevname = tmpstring.substr(0, idx);
							}
							// 判断测点类型
							tmpColName = tmpstring.substr(idx + 1);
							// 去掉每行末尾的","
							idx = tmpColName.find_first_of(",");
							if (idx != string::npos)
							{
								tmpColName = tmpColName.substr(0, idx);
							}
							//								sprintf(m_LogString,"[yjj]发电机电压幅值tmpColName=[%s]",tmpColName.c_str());
							//								MessShow(m_LogString,LTERROR);	
							if (tmpColName == "00P")
							{
								// 有功
								nColNum = 2;
							}
							else if (tmpColName == "00Q")
							{
								// 无功
								nColNum = 3;
							}
							else if (tmpColName == "UAV相角")
							{
								// 电压相角
								nColNum = 1;
							}
							else if (tmpColName == "UAV幅值")
							{
								// 电压幅值
								nColNum = 0;
							}
							else if (tmpColName == "EEV相角")
							{
								// 发电机功角
								nColNum = 16;
							}
							else
							{
								// 未定义，默认为无效
								nColNum = -1;
							}
							//								sprintf(m_LogString,"[yjj]发电机电压幅值数据nColNum=[%d],ordernum=[%d]",nColNum,ordernum);
							//								MessShow(m_LogString,LTERROR);
							if (nColNum == ordernum)
							{
								// 获取设备数据
								sprintf(m_LogString, "获取发电机电压幅值数据,nColNum=%d", nColNum);
								MessShow(m_LogString);
								if (m_df.GetData(djj + 2, tmpLfoGn.v2d_data) == -1)
								{
									sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
										tmpstring.c_str(), INT_REC_VAL(tmpdev_id), m_df.GetLastError());
									MessShow(m_LogString, LTDEBUG);
								}
								else
								{
									// 										if (tmpLfoGn.dev.dev_id == 115686216317272154)
									// 										{
									// 											for (int kk = 0;kk<tmpLfoGn.v2d_data.size();kk++)
									// 											{
									// 												sprintf(m_LogString,"[%s]电压幅值=[%f]",tmpLfoGn.dev.dev_name,tmpLfoGn.v2d_data[kk]);
									// 												MessShow(m_LogString);
									// 											}
									// 										}
									break;
								}
							}
							else
							{
								sprintf(m_LogString, "未获取发电机电压幅值数据,nColNum=%d", nColNum);
								MessShow(m_LogString, LTERROR);
								continue;
							}
						}
					}
					// 						if( m_df.GetData(tmpLfoGn.dev.yc_name[ordernum], tmpLfoGn.v2d_data) == -1)
					// 						{
					// 							// 置数据状态
					// 							tmpLfoGn.stat = MENU_LFO_DEV_STS_BAD;
					// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
					// 								tmpLfoGn.dev.dev_name,tmpLfoGn.dev.dev_id,tmpLfoGn.dev.yc_name[ordernum],m_df.GetLastError());
					// 							MessShow(m_LogString,LTERROR);
					// 						}
					//  						sprintf(m_LogString,"5555555555555555555555");
					//  						MessShow(m_LogString,LTERROR);
				}

			}
			//PMU实时数据采用电压标幺值
			float temp_Gnf = tmpLfoGn.dev_info.maxvbase;
			//  				sprintf(m_LogString,"[yjj]机组["INT_REC_FMT"]电压等级为[%f]",tmpLfoGn.dev_info.dev_id,temp_Gnf);
			//   				MessShow(m_LogString,LTERROR);
			if (temp_Gnf > 0.1)
			{
				for (int kk = 0; kk < (int)tmpLfoGn.v2d_data.size(); kk++)
				{
					if (m_LfoPara.Voltunit_is_kV == 1)
					{
						tmpLfoGn.v2d_data[kk] = tmpLfoGn.v2d_data[kk] / (temp_Gnf);
					}
					else
					{
						tmpLfoGn.v2d_data[kk] = tmpLfoGn.v2d_data[kk] / (temp_Gnf * 1000.0);
					}
				}
			}
			else
			{
				for (int kk = 0; kk < (int)tmpLfoGn.v2d_data.size(); kk++)
				{
					if (m_LfoPara.Voltunit_is_kV == 1)
					{
						tmpLfoGn.v2d_data[kk] = tmpLfoGn.v2d_data[kk];
					}
					else
					{
						tmpLfoGn.v2d_data[kk] = tmpLfoGn.v2d_data[kk] / 1000.0;
					}
				}
			}
			//  				sprintf(m_LogString,"666666666666666666666");
			//  				MessShow(m_LogString,LTERROR);
			cts_VecDev.push_back(tmpLfoGn);
			if (DataFilter(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据滤波错误");
				MessShow(m_LogString, LTERROR);
				//	return -1;
			}
			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[机组电压标幺值]采样错误");
				MessShow(m_LogString, LTERROR);
				//	return -1;
			}
			// 				int tempint1 = (int)cts_VecDev.size();
			// 				sprintf(m_LogString,"【yjj】cts_VecDev大小3[%d]",tempint1);
			// 				MessShow(m_LogString);

							/*vector<LFO_PRONY_INFO> tmpVecCase;*/
			ClearLfoPronyInfo1(tmpVecCase);
			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成机组电压计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			//  				sprintf(m_LogString,"77777777777777777777777");
			//  				MessShow(m_LogString,LTERROR);
			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行机组电压Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			//  				sprintf(m_LogString,"88888888888888888888888");
			//  				MessShow(m_LogString,LTERROR);
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoGn.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						temp_V_ampl = tmpVecCase[k].vec_mode.front().ampl;
						temp_V_phas = tmpVecCase[k].vec_mode.front().phas;
						//		prony_flag = 0;
					}
					else
					{
						temp_V_ampl = 0.0;
						temp_V_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出机组[%s]电压对数符合要求的振荡模式,prony_flag++", tmpLfoGn.dev.dev_name);
						MessShow(m_LogString);
					}
				}
			}
			// 				sprintf(m_LogString,"999999999999999999999999999");
			//  				MessShow(m_LogString,LTERROR);
			//				temp_V_ampl = tmpVecCase[0].vec_mode.ampl;
			//				temp_V_phas = tmpVecCase[0].vec_mode.phas;

			ClearLfoPronyInfo1(tmpVecCase);
			//  				sprintf(m_LogString,"000000000000000000000000");
			//  				MessShow(m_LogString,LTERROR);
			ordernum = 1;
			ClearLfoDev(temp_lfodev);
			ClearLfoDev(cts_VecDev);

			// 				int tempint4 = (int)cts_VecDev.size();
			// 				sprintf(m_LogString,"【yjj】cts_VecDev大小4[%d]",tempint4);
			// 				MessShow(m_LogString);

			tmpLfoGn.stat = MENU_LFO_DEV_STS_OK;
			vector<float>().swap(tmpLfoGn.v2d_data);
			if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				GetDevDataFromDB(StartTime, EndTime, ordernum, tmpLfoGn);
			}
			else
			{
				if (VALID_INT_REC(tmpLfoGn.dev.dev_id))	
				{
					// 						sprintf(m_LogString,"aaaaaaaaaaaaaaaaaaaaaaa");
					// 						MessShow(m_LogString,LTERROR);
											//修改读取数据模块
					for (djj = 0; djj < (int)m_df.m_DataHead.vec_devid_s.size(); djj++)
					{
						// 获取设备ID
// 							tmpstring = m_df.m_DataHead.vec_devid_s[djj];
						INT_REC tmpdev_id = m_df.m_DataHead.vec_devid_s[djj];
						if (tmpdev_id == tmpLfoGn.dev.dev_id)
						{
							// 去掉前两列"日期时间,毫秒"
							//				tmpstring.clear();
							if (m_df.GetNumberOfVariables() >= djj + 2)
							{
								m_df.GetVariableName(djj + 2, tmpstring);
							}
							idx = tmpstring.find_last_of("-");
							// 获取设备名称
							if (idx != string::npos)
							{
								tmpdevname = tmpstring.substr(0, idx);
							}
							// 判断测点类型
							tmpColName = tmpstring.substr(idx + 1);
							// 去掉每行末尾的","
							idx = tmpColName.find_first_of(",");
							if (idx != string::npos)
							{
								tmpColName = tmpColName.substr(0, idx);
							}
							if (tmpColName == "00P")
							{
								// 有功
								nColNum = 2;
							}
							else if (tmpColName == "00Q")
							{
								// 无功
								nColNum = 3;
							}
							else if (tmpColName == "UAV相角")
							{
								// 电压相角
								nColNum = 1;
							}
							else if (tmpColName == "UAV幅值")
							{
								// 电压幅值
								nColNum = 0;
							}
							else if (tmpColName == "EEV相角")
							{
								// 发电机功角
								nColNum = 16;
							}
							else
							{
								// 未定义，默认为无效
								nColNum = -1;
							}

							if (nColNum == ordernum)
							{
								// 获取设备数据
								if (m_df.GetData(djj + 2, tmpLfoGn.v2d_data) == -1)
								{
									sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
										tmpstring.c_str(), INT_REC_VAL(tmpdev_id), m_df.GetLastError());
									MessShow(m_LogString, LTDEBUG);
									//	prony_flag ++;
								}
								else
								{
									// 										if (tmpLfoGn.dev.dev_id == 115686216317272154)
									// 										{
									// 											for (int kk = 0;kk<tmpLfoGn.v2d_data.size();kk++)
									// 											{
									// 												sprintf(m_LogString,"[%s]电压相角=[%f]",tmpLfoGn.dev.dev_name,tmpLfoGn.v2d_data[kk]);
									// 												MessShow(m_LogString);
									// 											}
									// 										}
									break;
								}
							}
							else
							{
								// 									sprintf(m_LogString,"未获取发电机电压相角数据,nColNum=%d",nColNum);
								// 									MessShow(m_LogString,LTERROR);
																//	prony_flag ++;
								continue;
							}
						}
					}
					// 						if( m_df.GetData(tmpLfoGn.dev.yc_name[ordernum], tmpLfoGn.v2d_data) == -1)
					// 						{
					// 							// 置数据状态
					// 							tmpLfoGn.stat = MENU_LFO_DEV_STS_BAD;
					// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
					// 								tmpLfoGn.dev.dev_name,tmpLfoGn.dev.dev_id,tmpLfoGn.dev.yc_name[ordernum],m_df.GetLastError());
					// 							MessShow(m_LogString,LTERROR);
					// 						}
				}
			}
			temp_lfodev = tmpLfoGn;
			// 				for (int kk=1;kk<(int)temp_lfodev.v2d_data.size();kk++)
			// 				{	
			// 					sprintf(m_LogString,"机组角速度曲线：%.6f,MAX_FRA_PER_SEC=%d",temp_lfodev.v2d_data[kk-1],MAX_FRA_PER_SEC);
			// 					MessShow(m_LogString);
			// 				}
			UnitAng(temp_lfodev.v2d_data);
			for (int kk = 0; kk < (int)temp_lfodev.v2d_data.size(); kk++)
			{
				delt_t = float(1.0 / MAX_FRA_PER_SEC); //单位s
				if (kk == 0)
				{
					// tmpLfoGn.v2d_data[kk]=0.0;  //单位rad/s	
					th0 = 2.0 * temp_lfodev.v2d_data[1] - temp_lfodev.v2d_data[2];
					tmpLfoGn.v2d_data[kk] = ((temp_lfodev.v2d_data[1] - th0) * PRONY_PI) / (delt_t * 180.0);  //单位rad/s			
				}
				else
				{
					tmpLfoGn.v2d_data[kk] = ((temp_lfodev.v2d_data[kk] - temp_lfodev.v2d_data[kk - 1]) * PRONY_PI) / (delt_t * 180.0);  //单位rad/s
//				  	 tmpLfoGn.v2d_data[kk]=tmpLfoGn.v2d_data[kk-1];//近似取值										
				}

			}
			// 				sprintf(m_LogString,"delt_t=%.6f,MAX_FRA_PER_SEC=%d",delt_t,MAX_FRA_PER_SEC);
			// 				MessShow(m_LogString);
			// 				for (int kk=0;kk<(int)tmpLfoGn.v2d_data.size();kk++)
			// 				{				
			// 					int tempint = (int)tmpLfoGn.v2d_data.size();
			// 					sprintf(m_LogString,"机组角速度[%d]曲线：%.6f",tempint,tmpLfoGn.v2d_data[kk]);
			// 					MessShow(m_LogString);
			// 				}

			cts_VecDev.push_back(tmpLfoGn);
			if (DataFilter(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据滤波错误");
				MessShow(m_LogString, LTERROR);
				//		return -1;
			}
			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据采样错误");
				MessShow(m_LogString, LTERROR);
				//	return -1;
			}
			// 				sprintf(m_LogString,"bbbbbbbbbbbbbbbbbbbbbbbbb");
			// 				MessShow(m_LogString,LTERROR);
			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成机组相角计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			// 				sprintf(m_LogString,"ddddddddddddddddddddddddddddd");
			// 				MessShow(m_LogString,LTERROR);
			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行机组相角Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoGn.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						nsamemode = 0;
						for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
						{
							if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
							{
								continue;
							}
							else
							{
								temp_A_ampl = tmpVecCase[k].vec_mode[l].ampl;
								temp_A_phas = tmpVecCase[k].vec_mode[l].phas;
								nsamemode = 1;
								break;
							}
						}
						if (nsamemode == 0)
						{
							temp_A_ampl = 0.0;
							temp_A_phas = 0.0;
							prony_flag++;
							sprintf(m_LogString, "Prony算法分析出机组[%s]角速度振荡模式与主导模式不同调,prony_flag++", tmpLfoGn.dev.dev_name);
							MessShow(m_LogString);
						}
					}
					else
					{
						temp_A_ampl = 0.0;
						temp_A_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出机组[%s]角速度符合要求的振荡模式,prony_flag++", tmpLfoGn.dev.dev_name);
						MessShow(m_LogString);
					}
				}
			}
			//				temp_A_ampl = tmpVecCase[0].vec_mode.ampl;
			//				temp_A_phas = tmpVecCase[0].vec_mode.phas;
			ClearLfoPronyInfo1(tmpVecCase);
			//				sprintf(m_LogString,"ccccccccccccccccccccccccc");
			//				MessShow(m_LogString,LTERROR);
			// 				temp_p_ampl = pSltMode[0].PartGn[bak_kkk].ampl;
			// 				temp_p_phas = pSltMode[0].PartGn[bak_kkk].phas;
			tmpLfoGn.v2d_data = tmpLfoGn.back_data;
			// 				for (int kk=0;kk<(int)tmpLfoGn.v2d_data.size();kk++)
			// 				{
			// 					int tempint = (int)tmpLfoGn.v2d_data.size();
			// 					sprintf(m_LogString,"机组有功功率[%d]曲线：%.6f",tempint,tmpLfoGn.v2d_data[kk]);
			// 					MessShow(m_LogString);
			// 				}
			ClearLfoDev(cts_VecDev);
			cts_VecDev.push_back(tmpLfoGn);
			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[机组有功功率]采样错误");
				MessShow(m_LogString, LTERROR);
				// return -1;
			}

			ClearLfoPronyInfo1(tmpVecCase);
			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成机组有功计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行机组有功功率Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoGn.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						nsamemode = 0;
						for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
						{
							if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
							{
								continue;
							}
							else
							{
								temp_p_ampl = tmpVecCase[k].vec_mode[l].ampl;
								temp_p_phas = tmpVecCase[k].vec_mode[l].phas;
								nsamemode = 1;
								break;
							}
						}
						if (nsamemode == 0)
						{
							temp_p_ampl = 0.0;
							temp_p_phas = 0.0;
							prony_flag++;
							sprintf(m_LogString, "Prony算法分析出机组[%s]有功功率振荡模式与主导模式不同调,prony_flag++", tmpLfoGn.dev.dev_name);
							MessShow(m_LogString);
						}
						//		prony_flag = 0;
					}
					else
					{
						temp_p_ampl = 0.0;
						temp_p_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出机组[%s]有功功率符合要求的振荡模式,prony_flag++", tmpLfoGn.dev.dev_name);
						MessShow(m_LogString);
					}

				}
			}
			if (prony_flag == 0)
			{
				temp_pow = temp_p_ampl * temp_A_ampl * cos((temp_p_phas - temp_A_phas) * PRONY_PI / 180) - 150.0 * temp_V_ampl * temp_A_ampl * cos((temp_V_phas - temp_A_phas) * PRONY_PI / 180);
				pSltMode[0].PartGn[bak_kkk].pow = temp_pow; //机组耗散功率
			}
			else
			{
				temp_pow = 0.0;
				pSltMode[0].PartGn[bak_kkk].pow = temp_pow; //机组耗散功率
			}

			sprintf(m_LogString, "机组有功振幅[%f]初相角[%f],角度振幅[%f]相角[%f],电压振幅[%f]相角[%f]", temp_p_ampl, temp_p_phas, temp_A_ampl, temp_A_phas, temp_V_ampl, temp_V_phas);
			MessShow(m_LogString);
			sprintf(m_LogString, "机组[%s]耗散功率=%f,prony_flag=%d", tmpLfoGn.dev.dev_name, temp_pow, prony_flag);
			MessShow(m_LogString);
		}

		sprintf(m_LogString, "参与因子过滤后的线路数目[%d]", pSltMode[0].nPartLn);
		MessShow(m_LogString);

		for (ii = 0; ii < pSltMode[0].nPartLn; ii++)
		{
			sprintf(m_LogString, "第[%d]条线路名称[%s]", ii + 1, pSltMode[0].PartLn[ii].obj_name);
			MessShow(m_LogString);
			prony_flag = 0;
			bak_kkk = ii;
			for (jj = 0; jj < (int)VecMonDev.size(); jj++)
			{
				if (pSltMode[0].PartLn[ii].obj_id == VecMonDev[jj].dev.dev_id)
				{
					tmpLfoLn = VecMonDev[jj];
					break;
				}
			}
			// 				sprintf(m_LogString,"【yjj33333333333333333333】");
			 //				MessShow(m_LogString);
			ordernum = 0;
			ClearLfoDev(temp_lfodev);

			//				sprintf(m_LogString,"【yjj22222222222222222222222222】");
			//				MessShow(m_LogString);

			ClearLfoDev(cts_VecDev);
			//				int tempint6 = (int)cts_VecDev.size();
			//				sprintf(m_LogString,"【yjj】cts_VecDev大小6[%d]",tempint6);
			//				MessShow(m_LogString);

			tmpLfoLn.stat = MENU_LFO_DEV_STS_OK;
			vector<float>().swap(tmpLfoLn.v2d_data);
			if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				GetDevDataFromDB(StartTime, EndTime, ordernum, tmpLfoLn);
			}
			else
			{
				if (VALID_INT_REC(tmpLfoLn.dev.dev_id))
				{
					//						sprintf(m_LogString,"aaaaaaaaaaaaaaaaaaaaaaa");
					//						MessShow(m_LogString,LTERROR);
											//修改读取数据模块
					for (djj = 0; djj < (int)m_df.m_DataHead.vec_devid_s.size(); djj++)
					{
						// 							if (djj%100 == 0)
						// 							{
						// 								usleep(5);
						// 							}
													// 获取设备ID
						// 							tmpstring = m_df.m_DataHead.vec_devid_s[djj];
						INT_REC tmpdev_id = m_df.m_DataHead.vec_devid_s[djj];
						if (tmpdev_id == tmpLfoLn.dev.dev_id)
						{
							// 去掉前两列"日期时间,毫秒"
							//				tmpstring.clear();
							if (m_df.GetNumberOfVariables() >= djj + 2)
							{
								m_df.GetVariableName(djj + 2, tmpstring);
							}
							idx = tmpstring.find_last_of("-");
							// 获取设备名称
							if (idx != string::npos)
							{
								tmpdevname = tmpstring.substr(0, idx);
							}
							// 判断测点类型
							tmpColName = tmpstring.substr(idx + 1);
							// 去掉每行末尾的","
							idx = tmpColName.find_first_of(",");
							if (idx != string::npos)
							{
								tmpColName = tmpColName.substr(0, idx);
							}
							if (tmpColName == "00P")
							{
								// 有功
								nColNum = 2;
							}
							else if (tmpColName == "00Q")
							{
								// 无功
								nColNum = 3;
							}
							else if (tmpColName == "UAV相角")
							{
								// 电压相角
								nColNum = 1;
							}
							else if (tmpColName == "UAV幅值")
							{
								// 电压幅值
								nColNum = 0;
							}
							else if (tmpColName == "EEV相角")
							{
								// 发电机功角
								nColNum = 16;
							}
							else
							{
								// 未定义，默认为无效
								nColNum = -1;
							}

							if (nColNum == ordernum)
							{
								// 获取设备数据
								if (m_df.GetData(djj + 2, tmpLfoLn.v2d_data) == -1)
								{
									sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
										tmpstring.c_str(), INT_REC_VAL(tmpdev_id), m_df.GetLastError());
									MessShow(m_LogString, LTDEBUG);
									//	prony_flag ++;
								}
								else
								{
									break;
								}
							}
							else
							{
								// 									sprintf(m_LogString,"未获取线路电压数据");
								// 									MessShow(m_LogString,LTERROR);
								// 									prony_flag ++;
								continue;
							}
						}
					}
					// 						if( df.GetData(tmpLfoLn.dev.yc_name[ordernum], tmpLfoLn.v2d_data) == -1)
					// 						{
					// 							// 置数据状态
					// 							tmpLfoGn.stat = MENU_LFO_DEV_STS_BAD;
					// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
					// 								tmpLfoLn.dev.dev_name,tmpLfoLn.dev.dev_id,tmpLfoLn.dev.yc_name[ordernum],m_df.GetLastError());
					// 							MessShow(m_LogString,LTERROR);
					// 						}
				}
			}
			//PMU实时数据采用电压标幺值
			float temp_lnf = tmpLfoLn.dev_info.maxvbase;
			// 				sprintf(m_LogString,"[yjj]线路["INT_REC_FMT"]电压等级为[%f]",tmpLfoLn.dev_info.dev_id,temp_lnf);
			//  				MessShow(m_LogString,LTERROR);

			if (temp_lnf > 0.1)
			{
				for (int kk = 0; kk < (int)tmpLfoLn.v2d_data.size(); kk++)
				{
					if (m_LfoPara.Voltunit_is_kV == 1)
					{
						tmpLfoLn.v2d_data[kk] = tmpLfoLn.v2d_data[kk] / temp_lnf;
					}
					else
					{
						tmpLfoLn.v2d_data[kk] = tmpLfoLn.v2d_data[kk] / (temp_lnf * 1000.0);
					}
				}
			}
			else
			{
				for (int kk = 0; kk < (int)tmpLfoLn.v2d_data.size(); kk++)
				{
					if (m_LfoPara.Voltunit_is_kV == 1)
					{
						tmpLfoLn.v2d_data[kk] = tmpLfoLn.v2d_data[kk];
					}
					else
					{
						tmpLfoLn.v2d_data[kk] = tmpLfoLn.v2d_data[kk] / 1000.0;
					}
				}
			}
			// 				for (int kk=0;kk<(int)tmpLfoLn.v2d_data.size();kk++)
			// 				{				
			// 					int tempint = (int)tmpLfoLn.v2d_data.size();
			// 					sprintf(m_LogString,"线路[%s]电压幅值[%d]曲线：%.6f",tmpLfoLn.dev.dev_name,tempint,tmpLfoLn.v2d_data[kk]);
			// 					MessShow(m_LogString);
			// 					
			// 				}				
			for (int kk = 0; kk < (int)tmpLfoLn.v2d_data.size(); kk++)
			{
				float tempU;
				tempU = tmpLfoLn.v2d_data[kk];
				tmpLfoLn.v2d_data[kk] = log(tempU);

				// 					int tempint = (int)tmpLfoLn.v2d_data.size();
				// 					sprintf(m_LogString,"线路[%s]电压对数[%d]曲线：%.6f",tmpLfoLn.dev.dev_name,tempint,tmpLfoLn.v2d_data[kk]);
				// 					MessShow(m_LogString);

			}

			cts_VecDev.push_back(tmpLfoLn);
			if (DataFilter(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据滤波错误");
				MessShow(m_LogString, LTERROR);
				//					return -1;
			}
			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[线路电压]采样错误");
				MessShow(m_LogString, LTERROR);
				// return -1;
			}
			/*vector<LFO_PRONY_INFO> tmpVecCase;*/
			ClearLfoPronyInfo1(tmpVecCase);
			// 
			// 				tempint6 = (int)tmpVecCase.size();
			// 				sprintf(m_LogString,"【yjj】tmpVecCase大小1[%d]",tempint6);
			// 				MessShow(m_LogString);

			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成线路电压计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}

			// 				tempint6 = (int)tmpVecCase.size();
			// 				sprintf(m_LogString,"【yjj】tmpVecCase大小2[%d]",tempint6);
			// 				MessShow(m_LogString);

			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行线路电压Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			// 				sprintf(m_LogString,"【yjj0000000000000000000】");
			// 				MessShow(m_LogString);
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoLn.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						nsamemode = 0;
						for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
						{
							if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
							{
								continue;
							}
							else
							{
								temp_V_ampl = tmpVecCase[k].vec_mode[l].ampl;
								temp_V_phas = tmpVecCase[k].vec_mode[l].phas;
								nsamemode = 1;
								break;
							}
						}
						if (nsamemode == 0)
						{
							temp_V_ampl = 0.0;
							temp_V_phas = 0.0;
							prony_flag++;
							sprintf(m_LogString, "Prony算法分析出线路[%s]电压振荡模式与主导模式不同,prony_flag++", tmpLfoLn.dev.dev_name);
							MessShow(m_LogString);
						}
					}
					else
					{
						temp_V_ampl = 0.0;
						temp_V_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出线路[%s]电压幅值符合要求的振荡模式,prony_flag++", tmpLfoLn.dev.dev_name);
						MessShow(m_LogString);
					}
				}
			}
			// 				sprintf(m_LogString,"【yjj11111111111111111111111】");
			// 				MessShow(m_LogString);
			// 				temp_V_ampl = tmpVecCase[0].vec_mode.ampl;
			// 				temp_V_phas = tmpVecCase[0].vec_mode.phas;

			ClearLfoPronyInfo1(tmpVecCase);

			// 				ordernum = 1;
			// 				ClearLfoDev(temp_lfodev);
			// 				ClearLfoDev(cts_VecDev);
			// 
			// // 				int tempint9 = (int)cts_VecDev.size();
			// // 				sprintf(m_LogString,"【yjj】cts_VecDev大小8[%d]",tempint9);
			// // 				MessShow(m_LogString);
			// 
			// 				tmpLfoLn.stat = MENU_LFO_DEV_STS_OK;
			// 				if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			// 				{
			// 					GetDevDataFromDB(StartTime,EndTime,ordernum,tmpLfoLn);
			// 				}
			// 				else
			// 				{
			// 					if (tmpLfoLn.dev.dev_id >0 )
			// 					{
			// 						if( m_df.GetData(tmpLfoLn.dev.yc_name[ordernum], tmpLfoLn.v2d_data) == -1)
			// 						{
			// 							// 置数据状态
			// 							tmpLfoGn.stat = MENU_LFO_DEV_STS_BAD;
			// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
			// 								tmpLfoLn.dev.dev_name,tmpLfoLn.dev.dev_id,tmpLfoLn.dev.yc_name[ordernum],m_df.GetLastError());
			// 							MessShow(m_LogString,LTERROR);
			// 						}
			// 					}
			// 				}
			// 
			// 				temp_lfodev = tmpLfoLn;
			// 				for (int kk=1;kk<(int)temp_lfodev.v2d_data.size();kk++)
			// 				{
			// 					delt_t = float(1.0/MAX_FRA_PER_SEC); //单位s
			// 					tmpLfoLn.v2d_data[kk-1]=((temp_lfodev.v2d_data[kk]-temp_lfodev.v2d_data[kk-1])*PRONY_PI)/(delt_t*180.0);  //单位rad/s
			// 					tmpLfoLn.v2d_data[kk]=tmpLfoLn.v2d_data[kk-1];//近似取值
			// 				}
			// // 				for (int kk=0;kk<(int)tmpLfoLn.v2d_data.size();kk++)
			// // 				{
			// // 					int tempint = (int)tmpLfoLn.v2d_data.size();
			// // 					sprintf(m_LogString,"线路[%s]角速度[%d]曲线：%.6f",tmpLfoLn.dev.dev_name,tempint,tmpLfoLn.v2d_data[kk]);
			// // 					MessShow(m_LogString);
			// // 				}				
			// 				cts_VecDev.push_back(tmpLfoLn);
			// // 				int tempint = (int)cts_VecDev.size();
			// // 				sprintf(m_LogString,"【yjj】cts_VecDev大小9[%d]",tempint);
			// // 				MessShow(m_LogString);
			// 				/*vector<LFO_PRONY_INFO> tmpVecCase;*/
			// 				ClearLfoPronyInfo1(tmpVecCase);
			// 				if (FormPronyData(cts_VecDev,tmpVecCase)<0)
			// 				{
			// 					sprintf(m_LogString,"形成线路角速度计算数据错误");
			// 					MessShow(m_LogString,LTERROR);
			// 					return -1;
			// 				}				
			// 				if (PronyProcess(tmpVecCase)<0)
			// 				{
			// 					sprintf(m_LogString,"进行线路角速度Prony分析错误");
			// 					MessShow(m_LogString,LTERROR);
			// 					return -1;
			// 				}
			// 				for (int k =0; k<(int)tmpVecCase.size(); k++ )
			// 				{
			// 					if (tmpLfoLn.dev.dev_id == tmpVecCase[k].dev.dev_id)
			// 					{
			// 						if (!tmpVecCase[k].vec_mode.empty())
			// 						{
			// 							temp_A_ampl = tmpVecCase[k].vec_mode.front().ampl;
			// 							temp_A_phas = tmpVecCase[k].vec_mode.front().phas;
			// 						//	prony_flag = 0;
			// 						}
			// 						else
			// 						{
			// 							prony_flag ++;
			// 							sprintf( m_LogString, "Prony算法未分析出线路[%s]有功功率符合要求的振荡模式!",tmpLfoLn.dev.dev_name);
			// 							MessShow( m_LogString );							
			// 						}
			// 					}
			// 				}

			if (tmpLfoLn.dev.dev_id == pSltMode[0].PartLn[bak_kkk].obj_id)
			{
				temp_A_ampl = pSltMode[0].PartLn[bak_kkk].ampl;
				temp_A_phas = pSltMode[0].PartLn[bak_kkk].phas;
				//	prony_flag = 0;
			}
			else
			{
				temp_A_ampl = 0.0;
				temp_A_phas = 0.0;
				prony_flag++;
				sprintf(m_LogString, "未获取线路[%s]角速度符合要求的振荡模式,prony_flag++", tmpLfoLn.dev.dev_name);
				MessShow(m_LogString);
			}

			ClearLfoPronyInfo1(tmpVecCase);

			ordernum = 3;
			ClearLfoDev(temp_lfodev);
			ClearLfoDev(cts_VecDev);

			// 				int tempint11 = (int)cts_VecDev.size();
			// 				sprintf(m_LogString,"【yjj】cts_VecDev大小10[%d]",tempint11);
			// 				MessShow(m_LogString);
			tmpLfoLn.stat = MENU_LFO_DEV_STS_OK;
			vector<float>().swap(tmpLfoLn.v2d_data);
			if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				GetDevDataFromDB(StartTime, EndTime, ordernum, tmpLfoLn);
			}
			else
			{
				if (VALID_INT_REC(tmpLfoLn.dev.dev_id))
				{
					//修改读取数据模块
					for (djj = 0; djj < (int)m_df.m_DataHead.vec_devid_s.size(); djj++)
					{
						// 							if (djj%100 == 0)
						// 							{
						// 								usleep(5);
						// 							}
													// 获取设备ID
						// 							tmpstring = m_df.m_DataHead.vec_devid_s[djj];
						INT_REC tmpdev_id = m_df.m_DataHead.vec_devid_s[djj];
						if (tmpdev_id == tmpLfoLn.dev.dev_id)
						{
							// 去掉前两列"日期时间,毫秒"
							//				tmpstring.clear();
							if (m_df.GetNumberOfVariables() >= djj + 2)
							{
								m_df.GetVariableName(djj + 2, tmpstring);
							}
							idx = tmpstring.find_last_of("-");
							// 获取设备名称
							if (idx != string::npos)
							{
								tmpdevname = tmpstring.substr(0, idx);
							}
							// 判断测点类型
							tmpColName = tmpstring.substr(idx + 1);
							// 去掉每行末尾的","
							idx = tmpColName.find_first_of(",");
							if (idx != string::npos)
							{
								tmpColName = tmpColName.substr(0, idx);
							}
							if (tmpColName == "00P")
							{
								// 有功
								nColNum = 2;
							}
							else if (tmpColName == "00Q")
							{
								// 无功
								nColNum = 3;
							}
							else if (tmpColName == "UAV相角")
							{
								// 电压相角
								nColNum = 1;
							}
							else if (tmpColName == "UAV幅值")
							{
								// 电压幅值
								nColNum = 0;
							}
							else if (tmpColName == "EEV相角")
							{
								// 发电机功角
								nColNum = 16;
							}
							else
							{
								// 未定义，默认为无效
								nColNum = -1;
							}

							if (nColNum == ordernum)
							{
								// 获取设备数据
								if (m_df.GetData(djj + 2, tmpLfoLn.v2d_data) == -1)
								{
									sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
										tmpstring.c_str(), INT_REC_VAL(tmpdev_id), m_df.GetLastError());
									//prony_flag ++;
								}
								else
								{
									break;
								}
							}
							else
							{
								// 									sprintf(m_LogString,"未获取线路无功数据,tmpColName=[%s]",tmpColName.c_str());
								// 									MessShow(m_LogString,LTERROR);
								// 									prony_flag ++;
								continue;
							}
						}
					}
					// 						if( m_df.GetData(tmpLfoLn.dev.yc_name[ordernum], tmpLfoLn.v2d_data) == -1)
					// 						{
					// 							// 置数据状态
					// 							tmpLfoGn.stat = MENU_LFO_DEV_STS_BAD;
					// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
					// 								tmpLfoLn.dev.dev_name,tmpLfoLn.dev.dev_id,tmpLfoLn.dev.yc_name[ordernum],m_df.GetLastError());
					// 							MessShow(m_LogString,LTERROR);
					// 						}
				}
			}
			cts_VecDev.push_back(tmpLfoLn);
			if (DataFilter(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[线路无功]滤波错误");
				MessShow(m_LogString, LTERROR);
				//	return -1;
			}
			// 				for (int kk=0;kk<(int)tmpLfoLn.v2d_data.size();kk++)
			// 				{
			// 					int tempint = (int)tmpLfoLn.v2d_data.size();
			// 					sprintf(m_LogString,"线路[%s]无功[%d]曲线：%.6f",tmpLfoLn.dev.dev_name,tempint,tmpLfoLn.v2d_data[kk]);
			// 					MessShow(m_LogString);
			// 				}
			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[线路无功]采样错误");
				MessShow(m_LogString, LTERROR);
				// return -1;
			}
			/*vector<LFO_PRONY_INFO> tmpVecCase;*/
			ClearLfoPronyInfo1(tmpVecCase);
			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成线路无功计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			// 				sprintf(m_LogString,"线路11111111111111");
			// 				MessShow(m_LogString,LTERROR);	
			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行线路无功Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			// 				sprintf(m_LogString,"线路222222222222222");
			// 				MessShow(m_LogString,LTERROR);	
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoLn.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						nsamemode = 0;
						for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
						{
							if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
							{
								continue;
							}
							else
							{
								temp_q_ampl = tmpVecCase[k].vec_mode[l].ampl;
								temp_q_phas = tmpVecCase[k].vec_mode[l].phas;
								nsamemode = 1;
								break;
							}
						}
						if (nsamemode == 0)
						{
							temp_q_ampl = 0.0;
							temp_q_phas = 0.0;
							prony_flag++;
							sprintf(m_LogString, "Prony算法分析出线路[%s]无功功率振荡模式与主导模式不同,prony_flag++", tmpLfoGn.dev.dev_name);
							MessShow(m_LogString);
						}
					}
					else
					{
						temp_q_ampl = 0.0;
						temp_q_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出线路[%s]无功功率符合要求的振荡模式,prony_flag++", tmpLfoLn.dev.dev_name);
						MessShow(m_LogString);
					}
				}
			}
			// 				sprintf(m_LogString,"线路33333333333333333333");
			// 				MessShow(m_LogString,LTERROR);	
			// 				temp_q_ampl = tmpVecCase[0].vec_mode.ampl;
			// 				temp_q_phas = tmpVecCase[0].vec_mode.phas;
			ClearLfoPronyInfo1(tmpVecCase);
			// 				sprintf(m_LogString,"线路444444444444444");
			// 				MessShow(m_LogString,LTERROR);	
			// 				temp_p_ampl = pSltMode[0].PartLn[bak_kkk].ampl;
			// 				temp_p_phas = pSltMode[0].PartLn[bak_kkk].phas;
			tmpLfoLn.v2d_data = tmpLfoLn.back_data;
			// 				for (int kk=0;kk<(int)tmpLfoLn.v2d_data.size();kk++)
			// 				{
			// 					int tempint = (int)tmpLfoLn.v2d_data.size();
			// 					sprintf(m_LogString,"线路[%s]有功功率[%d]曲线：%.6f",tmpLfoLn.dev.dev_name,tempint,tmpLfoLn.v2d_data[kk]);
			// 					MessShow(m_LogString);
			// 				}
			// 				sprintf(m_LogString,"线路555555555555555555");
			// 				MessShow(m_LogString,LTERROR);	

			ClearLfoPronyInfo1(tmpVecCase);
			ClearLfoDev(cts_VecDev);
			cts_VecDev.push_back(tmpLfoLn);
			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[线路有功功率]采样错误");
				MessShow(m_LogString, LTERROR);
				// return -1;
			}
			// 				sprintf(m_LogString,"线路666666666666666666");
			// 				MessShow(m_LogString,LTERROR);	
			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成线路有功计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}

			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行线路有功功率Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			// 				sprintf(m_LogString,"线路777777777777777777");
			// 				MessShow(m_LogString,LTERROR);	
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoLn.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						nsamemode = 0;
						for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
						{
							if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
							{
								continue;
							}
							else
							{
								temp_p_ampl = tmpVecCase[k].vec_mode[l].ampl;
								temp_p_phas = tmpVecCase[k].vec_mode[l].phas;
								nsamemode = 1;
								break;
							}
						}
						if (nsamemode == 0)
						{
							temp_p_ampl = 0.0;
							temp_p_phas = 0.0;
							prony_flag++;
							sprintf(m_LogString, "Prony算法分析出线路[%s]有功功率振荡模式与主导模式不同,prony_flag++", tmpLfoGn.dev.dev_name);
							MessShow(m_LogString);
						}
					}
					else
					{
						temp_p_ampl = 0.0;
						temp_p_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出线路[%s]有功功率符合要求的振荡模式,prony_flag++", tmpLfoLn.dev.dev_name);
						MessShow(m_LogString);
					}
				}
			}
			if (prony_flag == 0)
			{
				temp_pow = temp_p_ampl * temp_A_ampl * cos((temp_p_phas - temp_A_phas) * PRONY_PI / 180.) + 5.0 * temp_q_ampl * temp_V_ampl * cos((temp_q_phas - temp_V_phas + 90) * PRONY_PI / 180.);
				pSltMode[0].PartLn[bak_kkk].pow = temp_pow; //线路耗散功率
			}
			else
			{
				pSltMode[0].PartLn[bak_kkk].pow = 0.0; //线路耗散功率
			}


			sprintf(m_LogString, "线路有功振幅[%f]初相角[%f],无功振幅[%f]初相角[%f],角度振幅[%f]相角[%f],电压振幅[%f]相角[%f]", temp_p_ampl, temp_p_phas, temp_q_ampl, temp_q_phas, temp_A_ampl, temp_A_phas, temp_V_ampl, temp_V_phas);
			MessShow(m_LogString);

			sprintf(m_LogString, "线路[%s]所属群[%d],耗散功率=%f,prony_flag=%d", tmpLfoLn.dev.dev_name, pSltMode[0].PartLn[bak_kkk].clus, pSltMode[0].PartLn[bak_kkk].pow, prony_flag);
			MessShow(m_LogString);
		}


		for (jj = 0; jj < pSltMode[0].nPartLn; jj++)
		{
			m_VecDev[ii].mode.fact = pSltMode[0].PartLn[jj].fact;
			// 				sprintf( m_LogString, "[yjj2]线路[%s]参与因子=%f",pSltMode[0].PartLn[jj].obj_name,pSltMode[0].PartLn[jj].fact);
			// 				MessShow( m_LogString );
		}

		for (ii = 0; ii < pSltMode[0].nPartTr; ii++)
		{
			// 				break;//振荡源定位不考虑变压器
			prony_flag = 0;
			bak_kkk = ii;
			for (jj = 0; jj < (int)VecMonDev.size(); jj++)
			{
				if (pSltMode[0].PartTr[ii].obj_id == VecMonDev[jj].dev.dev_id)
				{
					tmpLfoTr = VecMonDev[jj];
					break;
				}
			}

			ordernum = 0;
			ClearLfoDev(temp_lfodev);
			ClearLfoDev(cts_VecDev);
			tmpLfoTr.stat = MENU_LFO_DEV_STS_OK;
			vector<float>().swap(tmpLfoTr.v2d_data);
			if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				GetDevDataFromDB(StartTime, EndTime, ordernum, tmpLfoTr);
			}
			else
			{
				if (VALID_INT_REC(tmpLfoTr.dev.dev_id))
				{
					//修改读取数据模块
					for (djj = 0; djj < (int)m_df.m_DataHead.vec_devid_s.size(); djj++)
					{
						// 获取设备ID
// 							tmpstring = m_m_df.m_DataHead.vec_devid_s[djj];
						INT_REC tmpdev_id = m_df.m_DataHead.vec_devid_s[djj];
						if (tmpdev_id == tmpLfoTr.dev.dev_id)
						{
							// 去掉前两列"日期时间,毫秒"
							//				tmpstring.clear();
							if (m_df.GetNumberOfVariables() >= djj + 2)
							{
								m_df.GetVariableName(djj + 2, tmpstring);
							}
							idx = tmpstring.find_last_of("-");
							// 获取设备名称
							if (idx != string::npos)
							{
								tmpdevname = tmpstring.substr(0, idx);
							}
							// 判断测点类型
							tmpColName = tmpstring.substr(idx + 1);
							// 去掉每行末尾的","
							idx = tmpColName.find_first_of(",");
							if (idx != string::npos)
							{
								tmpColName = tmpColName.substr(0, idx);
							}
							if (tmpColName == "00P")
							{
								// 有功
								nColNum = 2;
							}
							else if (tmpColName == "00Q")
							{
								// 无功
								nColNum = 3;
							}
							else if (tmpColName == "UAV相角")
							{
								// 电压相角
								nColNum = 1;
							}
							else if (tmpColName == "UAV幅值")
							{
								// 电压幅值
								nColNum = 0;
							}
							else if (tmpColName == "EEV相角")
							{
								// 发电机功角
								nColNum = 16;
							}
							else
							{
								// 未定义，默认为无效
								nColNum = -1;
							}

							if (nColNum == ordernum)
							{
								// 获取设备数据
								if (m_df.GetData(djj + 2, tmpLfoTr.v2d_data) == -1)
								{
									sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
										tmpstring.c_str(), INT_REC_VAL(tmpdev_id), m_df.GetLastError());
									MessShow(m_LogString, LTDEBUG);
									//										prony_flag ++;
								}
								else
								{
									break;
								}
							}
							else
							{
								sprintf(m_LogString, "未获取变压器电压数据");
								MessShow(m_LogString, LTERROR);
								// 									prony_flag ++;
								continue;
							}
						}
					}

					// 						if( df.GetData(tmpLfoTr.dev.yc_name[ordernum], tmpLfoTr.v2d_data) == -1)
					// 						{
					// 							// 置数据状态
					// 							tmpLfoTr.stat = MENU_LFO_DEV_STS_BAD;
					// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
					// 								tmpLfoTr.dev.dev_name,tmpLfoTr.dev.dev_id,tmpLfoTr.dev.yc_name[ordernum],df.GetLastError());
					// 							MessShow(m_LogString,LTERROR);
					// 						}
				}
			}
			float temp_Trf = tmpLfoTr.dev_info.maxvbase;
			if (temp_Trf > 0.1)
			{
				for (int kk = 0; kk < (int)tmpLfoTr.v2d_data.size(); kk++)
				{
					if (m_LfoPara.Voltunit_is_kV == 1)
					{
						tmpLfoTr.v2d_data[kk] = tmpLfoTr.v2d_data[kk] / temp_Trf;
					}
					else
					{
						tmpLfoTr.v2d_data[kk] = tmpLfoTr.v2d_data[kk] / (temp_Trf * 1000.0);
					}
				}
			}
			else
			{
				for (int kk = 0; kk < (int)tmpLfoTr.v2d_data.size(); kk++)
				{
					if (m_LfoPara.Voltunit_is_kV == 1)
					{
						tmpLfoTr.v2d_data[kk] = tmpLfoTr.v2d_data[kk];
					}
					else
					{
						tmpLfoTr.v2d_data[kk] = tmpLfoTr.v2d_data[kk] / 1000.0;
					}
				}
			}
			for (int kk = 0; kk < (int)tmpLfoTr.v2d_data.size(); kk++)
			{
				float tempU;
				tempU = tmpLfoTr.v2d_data[kk];
				tmpLfoTr.v2d_data[kk] = log(tempU);
			}
			cts_VecDev.push_back(tmpLfoTr);
			if (DataFilter(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据滤波错误");
				MessShow(m_LogString, LTERROR);
				//		return -1;
			}
			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[变压器电压]采样错误");
				MessShow(m_LogString, LTERROR);
				// return -1;
			}
			/*vector<LFO_PRONY_INFO> tmpVecCase;*/
			ClearLfoPronyInfo1(tmpVecCase);
			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成变压器电压计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行变压器电压Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoTr.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						nsamemode = 0;
						for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
						{
							if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
							{
								continue;
							}
							else
							{
								temp_V_ampl = tmpVecCase[k].vec_mode[l].ampl;
								temp_V_phas = tmpVecCase[k].vec_mode[l].phas;
								nsamemode = 1;
								break;
							}
						}
						if (nsamemode == 0)
						{
							temp_V_ampl = 0.0;
							temp_V_phas = 0.0;
							prony_flag++;
							sprintf(m_LogString, "Prony算法分析出变压器[%s]电压幅值振荡模式与主导模式不同,prony_flag++", tmpLfoGn.dev.dev_name);
							MessShow(m_LogString);
						}
					}
					else
					{
						temp_V_ampl = 0.0;
						temp_V_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出变压器[%s]电压幅值符合要求的振荡模式,prony_flag++", tmpLfoTr.dev.dev_name);
						MessShow(m_LogString);
					}
				}
			}

			ClearLfoPronyInfo1(tmpVecCase);

			// 				ordernum = 1;
			// 				ClearLfoDev(temp_lfodev);
			// 				ClearLfoDev(cts_VecDev);
			// 				tmpLfoGn.stat = MENU_LFO_DEV_STS_OK;
			// 				if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			// 				{
			// 					GetDevDataFromDB(StartTime,EndTime,ordernum,tmpLfoTr);
			// 				}
			// 				else
			// 				{
			// 					if (tmpLfoTr.dev.dev_id >0 )
			// 					{
			// 						if( m_df.GetData(tmpLfoTr.dev.yc_name[ordernum], tmpLfoTr.v2d_data) == -1)
			// 						{
			// 							// 置数据状态
			// 							tmpLfoTr.stat = MENU_LFO_DEV_STS_BAD;
			// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
			// 								tmpLfoTr.dev.dev_name,tmpLfoTr.dev.dev_id,tmpLfoTr.dev.yc_name[ordernum],m_df.GetLastError());
			// 							MessShow(m_LogString,LTERROR);
			// 						}
			// 					}
			// 				}
			// 				temp_lfodev = tmpLfoTr;
			// 				for (int kk=1;kk<(int)temp_lfodev.v2d_data.size();kk++)
			// 				{
			// 					delt_t = float(1.0/MAX_FRA_PER_SEC); //单位s
			// 					tmpLfoTr.v2d_data[kk-1]=((temp_lfodev.v2d_data[kk]-temp_lfodev.v2d_data[kk-1])*PRONY_PI)/(delt_t*180.);  //单位rad/s
			// 					tmpLfoTr.v2d_data[kk]=tmpLfoTr.v2d_data[kk-1];//近似取值
			// 				}
			// 				cts_VecDev.push_back(tmpLfoTr);
			// 				/*vector<LFO_PRONY_INFO> tmpVecCase;*/
			// 				ClearLfoPronyInfo1(tmpVecCase);
			// 				if (FormPronyData(cts_VecDev,tmpVecCase)<0)
			// 				{
			// 					sprintf(m_LogString,"形成变压器相角计算数据错误");
			// 					MessShow(m_LogString,LTERROR);
			// 					return -1;
			// 				}				
			// 				if (PronyProcess(tmpVecCase)<0)
			// 				{
			// 					sprintf(m_LogString,"进行变压器相角Prony分析错误");
			// 					MessShow(m_LogString,LTERROR);
			// 					return -1;
			// 				}
			// 				for (int k = 0; k<(int)tmpVecCase.size(); k++ )
			// 				{
			// 					if (tmpLfoTr.dev.dev_id == tmpVecCase[k].dev.dev_id)
			// 					{
			// 						if (!tmpVecCase[k].vec_mode.empty())
			// 						{
			// 							temp_A_ampl = tmpVecCase[k].vec_mode.front().ampl;
			// 							temp_A_phas = tmpVecCase[k].vec_mode.front().phas;
			// 						//	prony_flag = 0;
			// 						}
			// 						else
			// 						{
			// 							prony_flag = ++;
			// 							sprintf( m_LogString, "Prony算法未分析出变压器[%s]角速度符合要求的振荡模式!",tmpLfoTr.dev.dev_name);
			// 							MessShow( m_LogString );							
			// 						}
			// 					}
			// 				}					
			if (tmpLfoTr.dev.dev_id == pSltMode[0].PartTr[bak_kkk].obj_id)
			{
				temp_A_ampl = pSltMode[0].PartTr[bak_kkk].ampl;
				temp_A_phas = pSltMode[0].PartTr[bak_kkk].phas;
				//		prony_flag = 0;
			}
			else
			{
				temp_A_ampl = 0.0;
				temp_A_phas = 0.0;
				prony_flag++;
				sprintf(m_LogString, "未获取变压器[%s]角速度符合要求的振荡模式,prony_flag++", tmpLfoTr.dev.dev_name);
				MessShow(m_LogString);
			}
			ClearLfoPronyInfo1(tmpVecCase);

			ordernum = 3;
			ClearLfoDev(temp_lfodev);
			ClearLfoDev(cts_VecDev);
			tmpLfoGn.stat = MENU_LFO_DEV_STS_OK;
			vector<float>().swap(tmpLfoGn.v2d_data);
			if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
			{
				GetDevDataFromDB(StartTime, EndTime, ordernum, tmpLfoTr);
			}
			else
			{
				if (VALID_INT_REC(tmpLfoTr.dev.dev_id))
				{
					//修改读取数据模块
					for (djj = 0; djj < (int)m_df.m_DataHead.vec_devid_s.size(); djj++)
					{
						// 获取设备ID
// 							tmpstring = m_m_df.m_DataHead.vec_devid_s[djj];
						INT_REC tmpdev_id = m_df.m_DataHead.vec_devid_s[djj];
						if (tmpdev_id == tmpLfoTr.dev.dev_id)
						{
							// 去掉前两列"日期时间,毫秒"
							//				tmpstring.clear();
							if (m_df.GetNumberOfVariables() >= djj + 2)
							{
								m_df.GetVariableName(djj + 2, tmpstring);
							}
							idx = tmpstring.find_last_of("-");
							// 获取设备名称
							if (idx != string::npos)
							{
								tmpdevname = tmpstring.substr(0, idx);
							}
							// 判断测点类型
							tmpColName = tmpstring.substr(idx + 1);
							// 去掉每行末尾的","
							idx = tmpColName.find_first_of(",");
							if (idx != string::npos)
							{
								tmpColName = tmpColName.substr(0, idx);
							}
							if (tmpColName == "00P")
							{
								// 有功
								nColNum = 2;
							}
							else if (tmpColName == "00Q")
							{
								// 无功
								nColNum = 3;
							}
							else if (tmpColName == "UAV相角")
							{
								// 电压相角
								nColNum = 1;
							}
							else if (tmpColName == "UAV幅值")
							{
								// 电压幅值
								nColNum = 0;
							}
							else if (tmpColName == "EEV相角")
							{
								// 发电机功角
								nColNum = 16;
							}
							else
							{
								// 未定义，默认为无效
								nColNum = -1;
							}

							if (nColNum == ordernum)
							{
								// 获取设备数据
								if (m_df.GetData(djj + 2, tmpLfoTr.v2d_data) == -1)
								{
									sprintf(m_LogString, "[%s]["INT_REC_FMT"]读取数据错误[%s]",
										tmpstring.c_str(), INT_REC_VAL(tmpdev_id), m_df.GetLastError());
									MessShow(m_LogString, LTDEBUG);
									//										prony_flag ++;
								}
								else
								{
									break;
								}
							}
							else
							{
								// 									sprintf(m_LogString,"未获取变压器无功数据");
								// 									MessShow(m_LogString,LTERROR);
								// 									prony_flag ++;
								continue;
							}
						}
					}
					// 						if( m_df.GetData(tmpLfoTr.dev.yc_name[ordernum], tmpLfoTr.v2d_data) == -1)
					// 						{
					// 							// 置数据状态
					// 							tmpLfoTr.stat = MENU_LFO_DEV_STS_BAD;
					// 							sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取[%s]数据错误[%s]",
					// 								tmpLfoTr.dev.dev_name,tmpLfoTr.dev.dev_id,tmpLfoTr.dev.yc_name[ordernum],m_df.GetLastError());
					// 							MessShow(m_LogString,LTERROR);
					// 						}
				}
			}
			cts_VecDev.push_back(tmpLfoTr);
			if (DataFilter(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[变压器无功]滤波错误");
				MessShow(m_LogString, LTERROR);
				//	return -1;
			}
			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[变压器无功]采样错误");
				MessShow(m_LogString, LTERROR);
				// return -1;
			}
			/*vector<LFO_PRONY_INFO> tmpVecCase;*/
			ClearLfoPronyInfo1(tmpVecCase);
			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成变压器无功计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行变压器无功Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoTr.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						nsamemode = 0;
						for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
						{
							if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
							{
								continue;
							}
							else
							{
								temp_q_ampl = tmpVecCase[k].vec_mode[l].ampl;
								temp_q_phas = tmpVecCase[k].vec_mode[l].phas;
								nsamemode = 1;
								break;
							}
						}
						if (nsamemode == 0)
						{
							temp_q_ampl = 0.0;
							temp_q_phas = 0.0;
							prony_flag++;
							sprintf(m_LogString, "Prony算法分析出变压器[%s]无功功率振荡模式与主导模式不同,prony_flag++", tmpLfoGn.dev.dev_name);
							MessShow(m_LogString);
						}
					}
					else
					{
						temp_q_ampl = 0.0;
						temp_q_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出变压器[%s]无功功率符合要求的振荡模式,prony_flag++", tmpLfoTr.dev.dev_name);
						MessShow(m_LogString);
					}
				}
			}

			ClearLfoDev(cts_VecDev);
			//				cts_VecDev.push_back(tmpLfoTr);	

			ClearLfoPronyInfo1(tmpVecCase);
			tmpLfoTr.v2d_data = tmpLfoTr.back_data;

			cts_VecDev.push_back(tmpLfoTr);
			if (DataFilter(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据滤波错误");
				MessShow(m_LogString, LTERROR);
				//	return -1;
			}
			// 				for (int kk=0;kk<(int)tmpLfoTr.v2d_data.size();kk++)
			//  				{
			//  					int tempint = (int)tmpLfoTr.v2d_data.size();
			//  					sprintf(m_LogString,"变压器[%s]有功功率[%d]曲线：%.6f",tmpLfoTr.dev.dev_name,tempint,tmpLfoTr.v2d_data[kk]);
			//  					MessShow(m_LogString);
			//  				}

			if (DataSample(cts_VecDev, 1) < 0)
			{
				sprintf(m_LogString, "数据[变压器有功]采样错误");
				MessShow(m_LogString, LTERROR);
				// return -1;
			}

			if (FormPronyData(cts_VecDev, tmpVecCase, 1) < 0)
			{
				sprintf(m_LogString, "形成变压器有功计算数据错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			if (PronyProcess(tmpVecCase) < 0)
			{
				sprintf(m_LogString, "进行变压器有功功率Prony分析错误");
				MessShow(m_LogString, LTERROR);
				// 					SAFE_DELETE_ARRAY(pData);
				if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
				if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
				if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
				m_df.ClearData();
				return -1;
			}
			for (int k = 0; k < (int)tmpVecCase.size(); k++)
			{
				if (tmpLfoTr.dev.dev_id == tmpVecCase[k].dev.dev_id)
				{
					if (!tmpVecCase[k].vec_mode.empty())
					{
						nsamemode = 0;
						for (int l = 0; l < tmpVecCase[k].vec_mode.size(); l++)
						{
							if (fabs(tmp_leadmode_freq - tmpVecCase[k].vec_mode[l].freq) > m_LfoPara.freq_diff)
							{
								continue;
							}
							else
							{
								temp_p_ampl = tmpVecCase[k].vec_mode[l].ampl;
								temp_p_phas = tmpVecCase[k].vec_mode[l].phas;
								nsamemode = 1;
								break;
							}
						}
						if (nsamemode == 0)
						{
							temp_p_ampl = 0.0;
							temp_p_phas = 0.0;
							prony_flag++;
							sprintf(m_LogString, "Prony算法分析出变压器[%s]有功功率振荡模式与主导模式不同,prony_flag++", tmpLfoGn.dev.dev_name);
							MessShow(m_LogString);
						}
					}
					else
					{
						temp_p_ampl = 0.0;
						temp_p_phas = 0.0;
						prony_flag++;
						sprintf(m_LogString, "Prony算法未分析出变压器[%s]有功功率符合要求的振荡模式,prony_flag++", tmpLfoTr.dev.dev_name);
						MessShow(m_LogString);
					}
				}
			}

			if (prony_flag == 0)
			{
				temp_pow = temp_p_ampl * temp_A_ampl * cos((temp_p_phas - temp_A_phas) * PRONY_PI / 180.) + 5.0 * temp_q_ampl * temp_V_ampl * cos((temp_V_phas - temp_A_phas + 90) * PRONY_PI / 180.);
				pSltMode[0].PartTr[bak_kkk].pow = temp_pow; //变压器耗散功率
			}
			else
			{
				temp_pow = 0.0;
				pSltMode[0].PartTr[bak_kkk].pow = 0.0; //变压器耗散功率
			}

			sprintf(m_LogString, "变压器有功振幅[%f]初相角[%f],无功振幅[%f]初相角[%f],角度振幅[%f]相角[%f],电压振幅[%f]相角[%f]", temp_p_ampl, temp_p_phas, temp_q_ampl, temp_q_phas, temp_A_ampl, temp_A_phas, temp_V_ampl, temp_V_phas);
			MessShow(m_LogString);
			sprintf(m_LogString, "变压器[%s]耗散功率=%f,prony_flag=%d", tmpLfoTr.dev.dev_name, temp_pow, prony_flag);
			MessShow(m_LogString);
		}

		// 		}
		// 		else
		// 		{
		// 		// 从数据文件中读取PMU数据
		// 		}
		int source_gnnum = 0, source_gnnum1 = 0;
		int source_lnnum = 0, source_lnnum1 = 0;
		int source_trnum = 0, source_trnum1 = 0;
		int source_gnflag[MODE_MAXGN];
		int source_gnflag1[MODE_MAXGN];
		int source_lnflag[MODE_MAXLN];
		int source_lnflag1[MODE_MAXLN];
		int source_trflag[MODE_MAXLN];
		int source_trflag1[MODE_MAXLN];
		int gn_flag, ln_flag, tr_flag, sflag1, sflag2;
		unsigned char     source_clus;
		//振荡源机组
		jj = 0;
		gn_flag = 0;
		for (ii = 0; ii < pSltMode[0].nPartGn; ii++)
		{
			sprintf(m_LogString, "振荡源机组耗散功率:%f[%f]!", pSltMode[0].PartGn[ii].pow, m_LfoPara.sh_gn_pow);
			MessShow(m_LogString, LTINFO);
			if ((pSltMode[0].PartGn[ii].pow - m_LfoPara.sh_gn_pow) > 0.001)
			{
				source_gnnum = source_gnnum + 1;
				source_gnflag[jj] = ii;
				jj = jj + 1;
			}
		}
		sprintf(m_LogString, "source_gnnum:%d!", source_gnnum);
		MessShow(m_LogString, LTINFO);
		if (source_gnnum > 0)
		{
			source_clus = pSltMode[0].PartGn[0].clus;
			jj = 0;
			for (ii = 0; ii < pSltMode[0].nPartGn; ii++)
			{
				sprintf(m_LogString, "振荡源机组所属群[%d],初相角:%f[%f]!", pSltMode[0].PartGn[ii].clus, source_clus, pSltMode[0].PartGn[ii].phas, m_LfoPara.sh_phas);
				MessShow(m_LogString, LTINFO);
				if (pSltMode[0].PartGn[ii].clus == source_clus && fabs(pSltMode[0].PartGn[ii].phas) > m_LfoPara.sh_phas)
				{
					source_gnnum1 = source_gnnum1 + 1;
					source_gnflag1[jj] = ii;
					jj = jj + 1;
				}
			}
			for (ii = 0; ii < source_gnnum; ii++)
			{
				sflag1 = source_gnflag[ii];
				for (jj = 0; jj < source_gnnum1; jj++)
				{
					sflag2 = source_gnflag1[jj];
					if (sflag1 == sflag2)
					{
						pSltMode[0].PartGn[sflag1].is_source = 1;
						sprintf(m_LogString, "找到振荡源机组:%s!", pSltMode[0].PartGn[sflag1].obj_name);
						MessShow(m_LogString, LTINFO);
						gn_flag = 1;
						break;
					}
				}
			}
		}
		else
		{
			gn_flag = 0;
		}
		if (gn_flag == 0)
		{
			sprintf(m_LogString, "未找到振荡源机组!");
			MessShow(m_LogString, LTINFO);
		}

		//关联振荡源线路
		jj = 0;
		ln_flag = 0;
		for (ii = 0; ii < pSltMode[0].nPartLn; ii++)
		{
			//			sprintf(m_LogString,"【yjj】线路耗散功率[%f],阀值[%f]!",pSltMode[0].PartLn[ii].pow,m_LfoPara.sh_ln_pow);
			//			MessShow(m_LogString,LTINFO);
			if ((fabs(pSltMode[0].PartLn[ii].pow) - m_LfoPara.sh_ln_pow) > 0.0001)
			{
				source_lnnum = source_lnnum + 1;
				source_lnflag[jj] = ii;
				jj = jj + 1;
			}
		}
		if (source_lnnum > 0)
		{
			int cts_lnflag = source_lnflag[0];
			float max_phas = 0.0;
			source_clus = pSltMode[0].PartLn[cts_lnflag].clus;
			jj = 0;
			for (ii = 0; ii < pSltMode[0].nPartLn; ii++)
			{
				if (pSltMode[0].PartLn[ii].clus == source_clus)
				{
					source_lnnum1 = source_lnnum1 + 1;
					source_lnflag1[jj] = ii;
					jj = jj + 1;

					float fff = 0;
					fff = fabs(pSltMode[0].PartLn[ii].phas) - max_phas;
					//	sprintf(m_LogString,"【yjj】相角[%f]-[%f]==[%f]!",pSltMode[0].PartLn[ii].phas,max_phas,fff);
					// 	MessShow(m_LogString,LTINFO);
					if (fff > 0.001)
					{
						max_phas = pSltMode[0].PartLn[ii].phas;
					}
				}
				// 				sprintf(m_LogString,"【yjj】振荡源所属群[%d][%d],相位[%f][%f],阀值[%f]!",source_clus,pSltMode[0].PartLn[ii].clus,max_phas,pSltMode[0].PartLn[ii].phas,m_LfoPara.sh_phas);
				// 				MessShow(m_LogString,LTINFO);
			}
			for (ii = 0; ii < source_lnnum; ii++)
			{
				sflag1 = source_lnflag[ii];
				for (jj = 0; jj < source_lnnum1; jj++)
				{
					sflag2 = source_lnflag1[jj];
					//	sprintf(m_LogString,"【yjj】相角1[%f][%f],[%d][%d]!",pSltMode[0].PartLn[sflag1].phas,max_phas,sflag1,sflag2);
					//	MessShow(m_LogString,LTINFO);	

					if (sflag1 == sflag2 && fabs(pSltMode[0].PartLn[sflag1].phas - max_phas) < m_LfoPara.sh_phas)
					{
						pSltMode[0].PartLn[sflag1].is_source = 1;
						sprintf(m_LogString, "找到关联振荡源线路:%s!", pSltMode[0].PartLn[sflag1].obj_name);
						MessShow(m_LogString, LTINFO);
						ln_flag = 1;
						break;
					}
				}
			}
		}
		else
		{
			ln_flag = 0;
		}
		if (ln_flag == 0)
		{
			sprintf(m_LogString, "未找到关联振荡源线路!");
			MessShow(m_LogString, LTINFO);
		}

		//关联振荡源变压器
		jj = 0;
		tr_flag = 0;
		for (ii = 0; ii < pSltMode[0].nPartTr; ii++)
		{
			if ((fabs(pSltMode[0].PartTr[ii].pow) - m_LfoPara.sh_ln_pow) > 0.001)
			{
				source_trnum = source_trnum + 1;
				source_trflag[jj] = ii;
				jj = jj + 1;
			}
		}
		if (source_trnum > 0)
		{
			int cts_trflag = source_trflag[0];
			float max_phas = 0.0;
			source_clus = pSltMode[0].PartTr[cts_trflag].clus;
			jj = 0;
			for (ii = 0; ii < pSltMode[0].nPartTr; ii++)
			{
				if (pSltMode[0].PartTr[ii].clus == source_clus)
				{
					source_trnum1 = source_trnum1 + 1;
					source_trflag1[jj] = ii;
					jj = jj + 1;
					if ((fabs(pSltMode[0].PartTr[ii].phas) - max_phas) > 0.001)
					{
						max_phas = pSltMode[0].PartTr[ii].phas;
					}
				}
			}
			for (ii = 0; ii < source_trnum; ii++)
			{
				sflag1 = source_trflag[ii];	//耗散功率较大

				for (jj = 0; jj < source_trnum1; jj++)
				{
					sflag2 = source_trflag1[jj];
					if (sflag1 == sflag2 && fabs(pSltMode[0].PartTr[sflag1].phas - max_phas) < m_LfoPara.sh_phas)
					{
						pSltMode[0].PartTr[sflag1].is_source = 1;
						sprintf(m_LogString, "找到关联振荡源变压器:%s!", pSltMode[0].PartTr[sflag1].obj_name);
						MessShow(m_LogString, LTINFO);
						tr_flag = 1;
						break;
					}
				}
			}
		}
		else
		{
			tr_flag = 0;
		}
		if (tr_flag == 0)
		{
			sprintf(m_LogString, "未找到关联振荡源变压器!");
			MessShow(m_LogString, LTINFO);
		}
		ClearLfoDev(temp_lfodev);
		ClearLfoDev(cts_VecDev);
	}
	ShowProMem("统计振荡源");

	// 统计振荡源信息到结构体m_VecDev中
	int nflag;
	m_MonSys.lfos_gn_num = 0;
	m_MonSys.lfos_ln_num = 0;
	m_MonSys.lfos_tr_num = 0;
	for (ii = 0; ii < (int)m_VecDev.size(); ii++)
	{
		// 		sprintf(m_LogString,"[yjj]m_VecDev设备个数[%d],机组个数[%d]设备类型[%d],是否告警[%d]!",(int)m_VecDev.size(),pSltMode[0].nPartGn,m_VecDev[ii].dev.dev_type,m_VecDev[ii].stat);
		//  		MessShow(m_LogString,LTINFO);
		// 		if(m_VecDev[ii].stat>=MENU_LFO_DEV_STS_WRN)
		{
			if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				for (jj = 0; jj < pSltMode[0].nPartGn; jj++)
				{
					//  					sprintf(m_LogString,"[yjj]机组[%s],耗散功率1[%f],扰动源设备ID["INT_REC_FMT"],ID["INT_REC_FMT"]!",pSltMode[0].PartGn[jj].obj_name,pSltMode[0].PartGn[jj].pow,pSltMode[0].PartGn[jj].obj_id,m_VecDev[ii].dev.dev_id);
					//  					MessShow(m_LogString,LTINFO);		

					if (m_VecDev[ii].dev.dev_id == pSltMode[0].PartGn[jj].obj_id)
					{
						m_VecDev[ii].mode.is_source = pSltMode[0].PartGn[jj].is_source;
						m_VecDev[ii].mode.pow = pSltMode[0].PartGn[jj].pow;
						m_VecDev[ii].mode.fact = pSltMode[0].PartGn[jj].fact;

						sprintf(m_LogString, "扰动源机组[%s],耗散功率[%f],是否振荡源标志[%d]!", m_VecDev[ii].dev.dev_name, m_VecDev[ii].mode.pow, m_VecDev[ii].mode.is_source);
						MessShow(m_LogString, LTINFO);
					}
				}
			}

			if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				for (jj = 0; jj < pSltMode[0].nPartLn; jj++)
				{
					if (m_VecDev[ii].dev.dev_id == pSltMode[0].PartLn[jj].obj_id)
					{
						m_VecDev[ii].mode.is_source = pSltMode[0].PartLn[jj].is_source;
						m_VecDev[ii].mode.pow = pSltMode[0].PartLn[jj].pow;
						m_VecDev[ii].mode.fact = pSltMode[0].PartLn[jj].fact;

						if (m_VecDev[ii].mode.pow > 0.01)
						{
							sprintf(m_VecDev[ii].mode.in_or_out, "%s", "流出");
							if (m_VecDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES)
							{//测点为首端 靠近振荡源厂站为末端厂站
								sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev_info.dot2.fac_name);
							}
							else
							{//测点为末端 靠近振荡源厂站为首端厂站
								sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev_info.dot1.fac_name);
							}
						}
						else
						{
							sprintf(m_VecDev[ii].mode.in_or_out, "%s", "流入");
							sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev.dot_fac_name);

						}

						// 						if ( m_VecDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES )
						// 						{
						// 							sprintf(m_VecDev[ii].mode.yc_name,"%s",m_VecDev[ii].dev_info.dot1.dot_name);
						// 						}
						// 						else if( m_VecDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES )
						// 						{
						// 							sprintf(m_VecDev[ii].mode.yc_name,"%s",m_VecDev[ii].dev_info.dot2.dot_name);
						// 						}
						// 						sprintf(m_VecDev[ii].mode.yc_name,"%s",m_VecDev[ii].dev.dot_dev_name);

						sprintf(m_LogString, "线路[%s],[%s],参与因子[%f],是否关联振荡源[%d]!", m_VecDev[ii].dev.dev_name, m_VecDev[ii].mode.in_or_out, m_VecDev[ii].mode.fact, m_VecDev[ii].mode.is_source);
						MessShow(m_LogString, LTINFO);

					}
				}
			}

			if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				for (jj = 0; jj < pSltMode[0].nPartTr; jj++)
				{
					if (m_VecDev[ii].dev.dev_id == pSltMode[0].PartTr[jj].obj_id)
					{
						m_VecDev[ii].mode.is_source = pSltMode[0].PartTr[jj].is_source;
						m_VecDev[ii].mode.pow = pSltMode[0].PartTr[jj].pow;
						m_VecDev[ii].mode.fact = pSltMode[0].PartTr[jj].fact;

						if (m_VecDev[ii].mode.pow > 0.01)
						{
							sprintf(m_VecDev[ii].mode.in_or_out, "%s", "流出");
							if (m_VecDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES)
							{//测点绕组为高压侧 靠近振荡源绕组为低压侧绕组
								sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev_info.dot2.dot_name);
							}
							else if (m_VecDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES)
							{//测点绕组为低压侧 靠近振荡源绕组为高压侧绕组
								sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev_info.dot1.dot_name);
							}
							else if (m_VecDev[ii].dev_info.dot3.IsYcDefine == WT_MENU_YES)
							{//测点绕组为中压侧 靠近振荡源绕组为高压侧绕组
								sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev_info.dot1.dot_name);
							}
						}
						else
						{
							sprintf(m_VecDev[ii].mode.in_or_out, "%s", "流入");
							if (m_VecDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES)
							{//测点绕组为高压侧 靠近振荡源绕组为高压侧绕组
								sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev_info.dot1.dot_name);
							}
							else if (m_VecDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES)
							{//测点绕组为低压侧 靠近振荡源绕组为低压侧绕组
								sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev_info.dot2.dot_name);
							}
							else if (m_VecDev[ii].dev_info.dot3.IsYcDefine == WT_MENU_YES)
							{//测点绕组为中压侧 靠近振荡源绕组为中压侧绕组
								sprintf(m_VecDev[ii].mode.yc_name, "%s", m_VecDev[ii].dev_info.dot3.dot_name);
							}
						}

						// 						if ( m_VecDev[ii].dev_info.dot1.IsYcDefine == WT_MENU_YES )
						// 						{
						// 							sprintf(m_VecDev[ii].mode.yc_name,"%s",m_VecDev[ii].dev_info.dot1.dot_name);
						// 						}
						// 						else if( m_VecDev[ii].dev_info.dot2.IsYcDefine == WT_MENU_YES )
						// 						{
						// 							sprintf(m_VecDev[ii].mode.yc_name,"%s",m_VecDev[ii].dev_info.dot2.dot_name);
						// 						}
						// 						else if(m_VecDev[ii].dev_info.dot3.IsYcDefine == WT_MENU_YES)
						// 						{
						// 							sprintf(m_VecDev[ii].mode.yc_name,"%s",m_VecDev[ii].dev_info.dot3.dot_name);
						// 						}
						// 						sprintf(m_VecDev[ii].mode.yc_name,"%s",m_VecDev[ii].dev.dot_dev_name);
						sprintf(m_LogString, "变压器[%s],[%s],参与因子[%f]!", m_VecDev[ii].dev.dev_name, m_VecDev[ii].mode.in_or_out, m_VecDev[ii].mode.fact);
						MessShow(m_LogString, LTINFO);
					}
				}
			}
			//将振荡源设备更新至m_MonSys
			nflag = 0;
			if (m_VecDev[ii].mode.is_source == 1)
			{
				if (m_VecDev[ii].mode.is_source > m_VecDev[ii].mode.eve_source)
				{
					m_VecDev[ii].mode.eve_source = m_VecDev[ii].mode.is_source;
				}
				for (int kk = 0; kk < m_MonSys.vec_dev.size(); kk++)
				{
					if (m_VecDev[ii].dev.dev_id == m_MonSys.vec_dev[kk].dev.dev_id)
					{
						m_MonSys.vec_dev[kk].mode.is_source = m_VecDev[ii].mode.is_source;
						if (m_MonSys.vec_dev[kk].mode.is_source > m_MonSys.vec_dev[kk].mode.eve_source)
						{
							m_MonSys.vec_dev[kk].mode.eve_source = m_MonSys.vec_dev[kk].mode.is_source;
						}
						m_MonSys.vec_dev[kk].mode.pow = m_VecDev[ii].mode.pow;
						m_MonSys.vec_dev[kk].mode.fact = m_VecDev[ii].mode.fact;
						// 						m_MonSys.vec_dev[kk].mode.vec_pow.push_back(m_MonSys.vec_dev[kk].mode.pow);

						if (m_VecDev[ii].mode.pow > 0.01)
						{
							sprintf(m_MonSys.vec_dev[kk].mode.in_or_out, "%s", "流出");
						}
						else
						{
							sprintf(m_MonSys.vec_dev[kk].mode.in_or_out, "%s", "流入");
						}

						nflag = 1;
						break;
					}
				}
				if (nflag == 0)
				{
					sprintf(m_LogString, "振荡源新增设备[%s]至m_MonSys.vec_dev", m_VecDev[ii].dev.dev_name);
					MessShow(m_LogString, LTINFO);
					m_MonSys.vec_dev.push_back(m_VecDev[ii]);
				}
				if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
				{
					sprintf(m_MonSys.lfos_gn_name, "%s", m_VecDev[ii].dev.dev_name);
					m_MonSys.lfos_gn_num++;
				}
				else if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
				{
					m_MonSys.lfos_ln_num++;
				}
				else if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
				{
					m_MonSys.lfos_tr_num++;
				}
			}
		}
	}
	if (m_MonSys.lfos_gn_num > 0)
	{
		sprintf(m_MonSys.lfos_des, "振荡源:%s", m_MonSys.lfos_gn_name);
	}
	else if (m_MonSys.lfos_ln_num > 0 || m_MonSys.lfos_tr_num > 0)
	{
		sprintf(m_MonSys.lfos_des, "找到振荡源相关支路");
	}
	else
	{
		sprintf(m_MonSys.lfos_des, "");
	}
	//更新模式信息中是否强迫振荡及振荡源描述
	if (!MonSys.vec_modeshape.empty())
	{
		if ((m_MonSys.lfos_gn_num + m_MonSys.lfos_ln_num + m_MonSys.lfos_tr_num) > 0)
		{//找到振荡源
			MonSys.vec_modeshape[0].is_lfo_source = 1;
			sprintf(MonSys.vec_modeshape[0].lfo_ana_des, "%s", m_MonSys.lfos_des);
		}
		else
		{//未找到振荡源
			MonSys.vec_modeshape[0].is_lfo_source = 0;
			sprintf(MonSys.vec_modeshape[0].lfo_ana_des, "");
		}
		sprintf(m_LogString, "[%f]Hz振荡模式是否振荡源标志[%d]", MonSys.vec_modeshape[0].mode.freq, MonSys.vec_modeshape[0].is_lfo_source);
		MessShow(m_LogString);
		for (ii = 0; ii < MonSys.vec_modeshape[0].vec_dev.size(); ii++)
		{
			for (jj = 0; jj < MonSys.vec_dev.size(); jj++)
			{
				if (MonSys.vec_modeshape[0].vec_dev[ii].dev.dev_id == MonSys.vec_dev[jj].dev.dev_id)
				{
					CopyLfoModeInfo(MonSys.vec_modeshape[0].vec_dev[ii].mode, m_MonSys.vec_dev[jj].mode);
					sprintf(m_LogString, "[%f]Hz振荡模式可观设备[%s]是否振荡源标志[%d]", MonSys.vec_modeshape[0].mode.freq, MonSys.vec_modeshape[0].vec_dev[ii].dev.dev_name, MonSys.vec_modeshape[0].vec_dev[ii].mode.is_source);
					MessShow(m_LogString);
					break;
				}
			}
		}
	}

	if (pAllObjMode) { free(pAllObjMode);   pAllObjMode = NULL; }
	if (pAllMode) { free(pAllMode);   pAllMode = NULL; }
	if (pSltMode) { free(pSltMode);   pSltMode = NULL; }
	m_df.ClearData();
	sprintf(m_LogString, "提示:振荡源定位结束,[%s]", m_MonSys.lfos_des);
	MessShow(m_LogString);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 根据厂站设置线路所属群
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : nPartLn                  线路数
//             pPartLn[]                线路信息
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaBase::LfoAnaSetLnClustbyFc(int nPartLn, MODE_PART_OBJ* pPartLn, int nPartFc, MODE_PART_OBJ* pPartFc, vector<LFO_DEV_INFO>& VecMonDev)
{
	if (nPartLn < 0 || !pPartLn) return;
	if (nPartFc < 0 || !pPartFc) return;

	int  ii, jj, kk;
	INT_REC temp_Fc_id;
	for (ii = 0; ii < nPartLn; ii++)
	{
		ZERO_INT_REC(temp_Fc_id);
		for (jj = 0; jj < (int)VecMonDev.size(); jj++)
		{
			if (pPartLn[ii].obj_id == VecMonDev[jj].dev_info.dot1.dot_id)
			{
				//				if (m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_1)
				temp_Fc_id = VecMonDev[jj].dev_info.fac_id;
				// 				else if (m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_2)
				// 					temp_Fc_id = m_pLnInf[jj].fac_id_2;
				// 				else
				// 					temp_Fc_id = 0;
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
//  函数功能 : 根据厂站设置线路所属群
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : nPartLn                  线路数
//             pPartLn[]                线路信息
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaBase::LfoAnaSetLnClustbyFc(int nPartLn, MODE_PART_OBJ* pPartLn, int nPartFc, MODE_PART_OBJ* pPartFc)
{
	if (nPartLn < 0 || !pPartLn) return;
	if (nPartFc < 0 || !pPartFc) return;

	int  ii, jj, kk;
	int  temp_Fc_id = 0;
	for (ii = 0; ii < nPartLn; ii++)
	{
		// 		temp_Fc_id = 0;
		// 		for ( jj=0; jj<m_nLnNum; jj++ )
		// 		{
		// 			if ( pPartLn[ii].obj_id == m_pLnMon[jj].acln_id )
		// 			{
		// 				if (m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_1)
		// 					temp_Fc_id = m_pLnInf[jj].fac_id_1;
		// 				else if (m_pLnMon[jj].slct_dot_id == m_pLnInf[jj].dot_id_2)
		// 					temp_Fc_id = m_pLnInf[jj].fac_id_2;
		// 				else
		// 					temp_Fc_id = 0;
		// 				break;
		// 			}
		// 		}
		// 		for ( kk=0; kk<nPartFc; kk++ )
		// 		{
		// 			if ( temp_Fc_id > 1 && temp_Fc_id == pPartFc[kk].obj_id )
		// 			{
		// 				pPartLn[ii].clus = pPartFc[kk].clus;
		// 				break;
		// 			}
		// 		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 根据厂站设置变压器所属群
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : nPartTr                  变压器数
//             pPartTr[]                变压器信息
////////////////////////////////////////////////////////////////////////////////
void CLfoAnaBase::LfoAnaSetTrClustbyFc(int nPartTr, MODE_PART_OBJ* pPartTr, int nPartFc, MODE_PART_OBJ* pPartFc)
{
	if (nPartTr < 0 || !pPartTr) return;
	if (nPartFc < 0 || !pPartFc) return;

	int  ii, jj, kk;
	int  temp_Fc_id = 0;
	for (ii = 0; ii < nPartTr; ii++)
	{
		temp_Fc_id = 0;
		// 		for ( jj=0; jj<(int)VecMonDev.size(); jj++ )
		// 		{
		// 			if ( pPartTr[ii].obj_id == VecMonDev[jj].dev_info.dev_id)
		// 			{
		// 				temp_Fc_id = VecMonDev[jj].dev_info.fac_id;
		// 				break;
		// 			}
		// 		}
		// 		for ( kk=0; kk<nPartFc; kk++ )
		// 		{
		// 			if ( temp_Fc_id > 1 && temp_Fc_id == pPartFc[kk].obj_id )
		// 			{
		// 				pPartTr[ii].clus = pPartFc[kk].clus;
		// 				break;
		// 			}
		// 		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

CDataCache::CDataCache()
{
#ifdef _WINDOWS32
	sprintf(m_backslash, "\\");
#else
	sprintf(m_backslash, "/");
#endif
	m_maindir = "..";
	m_DebugType = LTDEBUG;
}

CDataCache::~CDataCache()
{

}

int CDataCache::DataCacheMain()
{
	sprintf(m_LogString, "********%s Ver %s build at %s %s 数据线程[%ld]启动********", g_ProName, g_Version, __DATE__, __TIME__, GetId());
	MessShow(m_LogString, LTINFO);
	//     1)	数据缓存内存申请
	//         2)	获取新数据，判断数据状态，GetCacheData()
	//         3)	添加新数据AddCacheData()
	//         4)	删除旧数据DeleteCacheData()
	//         
	//         1)	数据获取接口，DataGet()
	//         2)	最新时间获取接口，GetNewTime()
	//         
//         1)	内存释放
	Sleep(5000);
	sprintf(m_LogString, "数据线程[%ld]退出", GetId());
	MessShow(m_LogString, LTINFO);
	g_PubLogMessShow(m_LogString, LTINFO);
	return 0;
}

BOOL CDataCache::OnTask(LPVOID lpv)
{
	DataCacheMain();
	return TRUE;
}

BOOL CDataCache::OnTask()
{
	DataCacheMain();
	return TRUE;
}

void CDataCache::MessShow(const char* log, const int logtype /*= LTINFO*/)
{
	PLF_MessShow(log, logtype);
	return;
}


int CLfoAnaObj::FormOscCaseResult(vector<LFO_SYS_INFO>& VecLfoWindow, LFO_CASE_INFO& LfoCase, const int save_flag)
{

	// 	int ii,jj,nFlag;
	// 	LFO_SYS_INFO tmpWindows;
	// 	LFO_DEV_INFO tmpDev;
	// 	ClearLfoSysInfo(tmpWindows);
	// 	ClearLfoDev(tmpDev);
	// 	if (!VecLfoWindow.empty())
	// 	{
	// 		tmpWindows = VecLfoWindow.back();
	// 	}
	// 	else
	// 	{
	// 		return 0;
	// 	}
	// 
	// 
	//     if (save_flag == LFO_SAVE_FLAG_IDEL)
	//     {
	//         return 0;
	//     }
	//     else if (save_flag == LFO_SAVE_FLAG_START)
	//     {
	//         // 清空变量
	//         sprintf(m_LogString,"初始化LFOOSCCASE");
	// 		MessShow(m_LogString,LTINFO);
	// 		ClearLfoCase(LfoCase);
	// 		// 本轮开始振荡时间减去告警判断时间
	// 		LfoCase.event_id = tmpWindows.start_time;
	// 		LfoCase.start_time = tmpWindows.start_time;
	// 		LfoCase.end_time = tmpWindows.end_time;
	// 		LfoCase.hold_time = LfoCase.end_time - LfoCase.start_time;
	// 		LfoCase.stat = tmpWindows.stat;
	// 		m_LfoPara.data_pos_osc = m_LfoPara.data_pos_old;
	//  		sprintf(m_LogString,"小扰动开始,窗口时间=系统时间,["INT_REC_FMT"]["INT_REC_FMT"]",tmpWindows.start_time,tmpWindows.end_time);
	//  		MessShow(m_LogString,LTINFO);
	// 		if (LfoCase.stat >= MENU_LFO_DEV_STS_WRN)
	// 		{
	// 			sprintf(LfoCase.case_desc,"%s开始告警,系统出现小扰动统计",PT_GetChineseTimeStringBySeconds(LfoCase.start_time));
	// 		}
	// 
	// 		LfoCase.lfo_gn_num = 0;
	// 		LfoCase.lfo_ln_num = 0;
	// 		LfoCase.lfo_tr_num = 0;
	// 		for (ii =0; ii<(int)LfoCase.vec_dev.size(); ii++)
	// 		{
	// 			if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
	// 			{
	// 				LfoCase.lfo_gn_num ++;
	// 			}
	// 			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
	// 			{
	// 				LfoCase.lfo_ln_num ++;
	// 			}
	// 			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
	// 			{
	// 				LfoCase.lfo_tr_num ++;
	// 			}
	// 		}
	// 
	// 		// 去除重复的窗口内容
	// 		nFlag = 0;
	// 		for (ii =0; ii<(int)LfoCase.vec_windows.size(); ii++)
	// 		{
	// 			if (tmpWindows.window_id == LfoCase.vec_windows[ii].window_id)
	// 			{
	// 				nFlag =1;
	// 				break;
	// 			}
	// 		}
	// 		if (nFlag == 0)
	// 		{
	// 			// 添加未保存的窗口
	// 			LfoCase.vec_windows.push_back(tmpWindows);
	// 			
	// 			// 计算平均频率
	// 			LfoCase.lead_mode.freq = 0.0f;
	// 			for ( ii =0; ii<(int)LfoCase.vec_windows.size(); ii++ )
	// 			{
	// 				LfoCase.lead_mode.freq += LfoCase.vec_windows[ii].lead_dev.mode.freq;
	// 				if (LfoCase.vec_windows[ii].lead_dev.mode.damp < LfoCase.lead_mode.damp)
	// 				{
	// 					LfoCase.lead_mode.damp = LfoCase.vec_windows[ii].lead_dev.mode.damp;
	// 				}
	// 				if (LfoCase.vec_windows[ii].lead_dev.mode.ampl > LfoCase.lead_mode.ampl)
	// 				{
	// 					LfoCase.lead_mode.ampl = LfoCase.vec_windows[ii].lead_dev.mode.ampl;
	// 				}
	// 			}
	// 			if (!LfoCase.vec_windows.empty())
	// 			{
	// 				LfoCase.lead_mode.freq = LfoCase.lead_mode.freq / (int)LfoCase.vec_windows.size();
	// 			}
	// 			
	// 			// 保存窗口信息中的振荡设备
	// 			for ( ii =0; ii<(int)tmpWindows.vec_dev.size(); ii++)
	// 			{
	// 				nFlag = 0;
	// 				for (jj =0; jj<(int)LfoCase.vec_dev.size(); jj++ )
	// 				{
	// 					if (tmpWindows.vec_dev[ii].dev.dev_id == LfoCase.vec_dev[jj].dev.dev_id)
	// 					{
	// 						nFlag = 1;
	// 						// 更新波形信息
	// 						if (tmpWindows.vec_dev[ii].curve.max_ampl > LfoCase.vec_dev[jj].curve.max_ampl)
	// 						{
	// 							LfoCase.vec_dev[jj].curve = tmpWindows.vec_dev[ii].curve;
	// 						}
	// // 						// 更新开始时间
	// // 						if (LfoCase.vec_dev[jj].osc_start_time <= 0)
	// // 						{
	// // 							LfoCase.vec_dev[jj].osc_start_time = tmpWindows.start_time;
	// // 						}
	// // 						// 更新结束时间
	// // 						if (tmpWindows.end_time > LfoCase.vec_dev[jj].osc_end_time)
	// // 						{
	// // 							LfoCase.vec_dev[jj].osc_end_time = tmpWindows.end_time;
	// // 						}
	// // 						sprintf(m_LogString,"[yjj]小扰动开始,更新已告警小扰动设备开始时间["INT_REC_FMT"],结束时间["INT_REC_FMT"]",LfoCase.vec_dev[jj].osc_start_time,LfoCase.vec_dev[jj].osc_end_time);
	// // 						MessShow(m_LogString,LTINFO);
	// 						// 更新持续时间
	// // 						LfoCase.vec_dev[jj].osc_hold_time = LfoCase.vec_dev[jj].osc_end_time - LfoCase.vec_dev[jj].osc_start_time;
	// 						// 添加模式信息,主导模式取并集
	// // 						for (kk=0; kk<(int)tmpWindows.vec_dev[ii].vec_lead_mode.size(); kk++)
	// // 						{
	// // 							LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].vec_lead_mode[kk]);
	// // 						}
	// 						// 2012-03-20 只保存主导模式
	// 						// 不管该设备是否告警，均保存本次分析的主导模式
	// // 						if(!tmpWindows.vec_dev[ii].vec_lead_mode.empty())
	// // 						{
	// 							LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.lead_dev.mode);
	// //						}
	// // 		sprintf(m_LogString,"[yjj]新增设备0["INT_REC_FMT"]，振幅[%f]",LfoCase.vec_dev[jj].dev.dev_id,LfoCase.vec_dev[jj].mode.ampl);
	// // 		MessShow(m_LogString,LTINFO);
	// 						break;
	// 					}
	// 				}
	// 				if (nFlag == 0)
	// 				{
	// 					// 新增设备
	// 					//memset((void *)&tmpDev,0,sizeof(LFO_DEV_INFO));
	// 					tmpDev = tmpWindows.vec_dev[ii];
	// // 					tmpDev.osc_start_time = tmpWindows.start_time;
	// // 					tmpDev.osc_end_time = tmpWindows.end_time;
	// // 					tmpDev.osc_hold_time = tmpDev.end_time - tmpDev.start_time;
	// 					LfoCase.vec_dev.push_back(tmpDev);
	// // 					sprintf(m_LogString,"[yjj]小扰动开始,新增设备["INT_REC_FMT"],["INT_REC_FMT"]",tmpDev.osc_start_time,tmpDev.osc_end_time);
	// // 					MessShow(m_LogString,LTINFO);
	// 				}
	// 			}
	// 		}
	// 
	// 		// 首先告警设备
	// 		if (!LfoCase.vec_dev.empty())
	// 		{
	// 			sort(LfoCase.vec_dev.begin(),LfoCase.vec_dev.end(),g_SortDevByEngy);
	// 			LfoCase.warn_dev = LfoCase.vec_dev.front();
	// 		}
	//     }
	//     else if (save_flag == LFO_SAVE_FLAG_HOLD)
	//     {
	// 		sprintf(m_LogString,"小扰动持续,窗口时间["INT_REC_FMT"]["INT_REC_FMT"]",tmpWindows.start_time,tmpWindows.end_time);
	//  		MessShow(m_LogString,LTINFO);
	// 		// 更新振荡开始时间
	// 		if ( tmpWindows.start_time < LfoCase.start_time)
	// 		{
	// 			LfoCase.start_time = tmpWindows.start_time;
	// 		}
	// 		// 更新振荡结束时间
	// 		if ( tmpWindows.end_time > LfoCase.end_time)
	// 		{
	// 			LfoCase.end_time = tmpWindows.end_time;
	// 		}
	// 		sprintf(m_LogString,"小扰动持续,系统时间["INT_REC_FMT"]["INT_REC_FMT"]",LfoCase.start_time,LfoCase.end_time);
	//  		MessShow(m_LogString,LTINFO);
	// 		LfoCase.hold_time = LfoCase.end_time - LfoCase.start_time;
	// 		LfoCase.stat = tmpWindows.stat;
	// 		if (LfoCase.stat >= MENU_LFO_DEV_STS_WRN)
	// 		{
	// 			sprintf(LfoCase.case_desc,"%s持续告警,系统出现小扰动统计",PT_GetChineseTimeStringBySeconds(LfoCase.start_time));
	// 		}
	// 		
	// 		LfoCase.lfo_gn_num = 0;
	// 		LfoCase.lfo_ln_num = 0;
	// 		LfoCase.lfo_tr_num = 0;
	// 		for (ii =0; ii<(int)LfoCase.vec_dev.size(); ii++)
	// 		{
	// 			if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
	// 			{
	// 				LfoCase.lfo_gn_num ++;
	// 			}
	// 			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
	// 			{
	// 				LfoCase.lfo_ln_num ++;
	// 			}
	// 			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
	// 			{
	// 				LfoCase.lfo_tr_num ++;
	// 			}
	// 		}
	// 
	// 		// 去除重复的窗口内容
	// 		nFlag = 0;
	// 		for (ii =0; ii<(int)LfoCase.vec_windows.size(); ii++)
	// 		{
	// 			if (tmpWindows.window_id == LfoCase.vec_windows[ii].window_id)
	// 			{
	// 				nFlag =1;
	// 				break;
	// 			}
	// 		}
	// 		
	// 		if (nFlag == 0)
	// 		{
	// 			LfoCase.vec_windows.push_back(tmpWindows);
	// 
	// 			// 计算平均频率
	// 			LfoCase.lead_mode.freq = 0.0f;
	// 			for ( ii =0; ii<(int)LfoCase.vec_windows.size(); ii++ )
	// 			{
	// 				LfoCase.lead_mode.freq += LfoCase.vec_windows[ii].lead_dev.mode.freq;
	// 				if (LfoCase.vec_windows[ii].lead_dev.mode.damp < LfoCase.lead_mode.damp)
	// 				{
	// 					LfoCase.lead_mode.damp = LfoCase.vec_windows[ii].lead_dev.mode.damp;
	// 				}
	// 				if (LfoCase.vec_windows[ii].lead_dev.mode.ampl > LfoCase.lead_mode.ampl)
	// 				{
	// 					LfoCase.lead_mode.ampl = LfoCase.vec_windows[ii].lead_dev.mode.ampl;
	// 				}
	// 			}
	// 			if (!LfoCase.vec_windows.empty())
	// 			{
	// 				LfoCase.lead_mode.freq = LfoCase.lead_mode.freq / (int)LfoCase.vec_windows.size();
	// 			}
	// 			
	// 			// 保存窗口信息中的振荡设备
	// 			for ( ii =0; ii<(int)tmpWindows.vec_dev.size(); ii++)
	// 			{
	// 				nFlag = 0;
	// 				for (jj =0; jj<(int)LfoCase.vec_dev.size(); jj++ )
	// 				{
	// 					if (tmpWindows.vec_dev[ii].dev.dev_id == LfoCase.vec_dev[jj].dev.dev_id)
	// 					{
	// 						nFlag = 1;
	// 						// 更新波形信息
	// 						if (tmpWindows.vec_dev[ii].curve.max_val > LfoCase.vec_dev[jj].curve.max_val)
	// 						{
	// 							LfoCase.vec_dev[jj].curve.max_val = tmpWindows.vec_dev[ii].curve.max_val;
	// 							LfoCase.vec_dev[jj].curve.max_val_t = tmpWindows.vec_dev[ii].curve.max_val_t;
	// 						}
	// 						if (tmpWindows.vec_dev[ii].curve.min_val < LfoCase.vec_dev[jj].curve.min_val)
	// 						{
	// 							LfoCase.vec_dev[jj].curve.min_val = tmpWindows.vec_dev[ii].curve.min_val;
	// 							LfoCase.vec_dev[jj].curve.min_val_t = tmpWindows.vec_dev[ii].curve.min_val_t;
	// 						}
	// 						LfoCase.vec_dev[jj].curve.max_ampl = LfoCase.vec_dev[jj].curve.max_val - LfoCase.vec_dev[jj].curve.min_val;
	// 						// 更新开始时间
	// 						if (LfoCase.vec_dev[jj].osc_start_time <= 0)
	// 						{
	// 							LfoCase.vec_dev[jj].osc_start_time = tmpWindows.start_time;
	// 						}
	// 						// 更新结束时间
	// 						if (tmpWindows.end_time > LfoCase.vec_dev[jj].osc_end_time)
	// 						{
	// 							LfoCase.vec_dev[jj].osc_end_time = tmpWindows.end_time;
	// 						}
	// 						// 更新持续时间
	// 						LfoCase.vec_dev[jj].osc_hold_time = LfoCase.vec_dev[jj].osc_end_time - LfoCase.vec_dev[jj].osc_start_time;
	// 		sprintf(m_LogString,"小扰动持续,已告警设备开始时间["INT_REC_FMT"]结束时间["INT_REC_FMT"]",LfoCase.vec_dev[jj].osc_start_time,LfoCase.vec_dev[jj].osc_end_time);
	//  		MessShow(m_LogString,LTINFO);						
	// 
	// 						// 添加模式信息,主导模式取并集
	// // 						for (kk=0; kk<(int)tmpWindows.vec_dev[ii].vec_lead_mode.size(); kk++)
	// // 						{
	// // 							LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.vec_dev[ii].vec_lead_mode[kk]);
	// // 						}
	// 						// 2012-03-20 只保存主导模式
	// // 						if(!tmpWindows.vec_dev[ii].vec_lead_mode.empty())
	// // 						{
	// 							LfoCase.vec_dev[jj].vec_lead_mode.push_back(tmpWindows.lead_dev.mode);
	// //						}
	// // 		sprintf(m_LogString,"[yjj]新增设备2["INT_REC_FMT"]，振幅[%f]",LfoCase.vec_dev[jj].dev.dev_id,LfoCase.vec_dev[jj].mode.ampl);
	// // 		MessShow(m_LogString,LTINFO);
	// 						break;
	// 					}
	// 				}
	// 				if (nFlag == 0)
	// 				{
	// // 		sprintf(m_LogString,"[yjj]新增设备3["INT_REC_FMT"]，振幅[%f]",tmpWindows.vec_dev[ii].dev.dev_id,tmpWindows.vec_dev[ii].mode.ampl);
	// // 		MessShow(m_LogString,LTINFO);
	// 					// 新增设备
	// 					//memset((void *)&tmpDev,0,sizeof(LFO_DEV_INFO));	
	// 					tmpDev = tmpWindows.vec_dev[ii];
	// 					tmpDev.osc_start_time = tmpWindows.start_time;
	// 					tmpDev.osc_end_time = tmpWindows.end_time;
	// 					tmpDev.osc_hold_time = tmpDev.end_time - tmpDev.start_time;
	// 					LfoCase.vec_dev.push_back(tmpWindows.vec_dev[ii]);
	// 				}
	// 			}
	// 		}
	//     }
	//     else if (save_flag == LFO_SAVE_FLAG_END)
	//     {
	// 		// 更新振荡开始时间
	// 		sprintf(m_LogString,"[提示]小扰动结束,窗口时间["INT_REC_FMT"]["INT_REC_FMT"]",tmpWindows.start_time,tmpWindows.end_time);
	//  		MessShow(m_LogString,LTINFO);
	//  		if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
	//  		{
	// 			if ( tmpWindows.start_time < LfoCase.start_time)
	// 			{
	// 				LfoCase.start_time = tmpWindows.start_time;
	// 			}
	// 		}
	// 		sprintf(m_LogString,"[提示]小扰动结束,系统时间["INT_REC_FMT"]["INT_REC_FMT"]",LfoCase.start_time,LfoCase.end_time);
	//  		MessShow(m_LogString,LTINFO);
	// 		LfoCase.hold_time = LfoCase.end_time - LfoCase.start_time;
	// 		LfoCase.stat = tmpWindows.stat;
	// 		if (LfoCase.stat >= MENU_LFO_DEV_STS_WRN)
	// 		{
	// 			sprintf(LfoCase.case_desc,"%s结束告警,系统出现小扰动统计",PT_GetChineseTimeStringBySeconds(LfoCase.start_time));
	// 		}
	// 		
	// 		LfoCase.lfo_gn_num = 0;
	// 		LfoCase.lfo_ln_num = 0;
	// 		LfoCase.lfo_tr_num = 0;
	// 		for (ii =0; ii<(int)LfoCase.vec_dev.size(); ii++)
	// 		{
	// 			if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
	// 			{
	// 				LfoCase.lfo_gn_num ++;
	// 			}
	// 			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
	// 			{
	// 				LfoCase.lfo_ln_num ++;
	// 			}
	// 			else if (LfoCase.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
	// 			{
	// 				LfoCase.lfo_tr_num ++;
	// 			}
	// 		}
	// 
	// 		sprintf(m_LogString,"[提示]小扰动统计结束["INT_REC_FMT"]",LfoCase.event_id);
	// 		MessShow(m_LogString,LTINFO);
	// // 生成小扰动统计信息		
	// 		SaveOscReult(LfoCase);
	// // 清空变量
	// 		sprintf(m_LogString,"清空LFOCASE");
	// 		MessShow(m_LogString,LTINFO);
	// 		ClearLfoCase(LfoCase);
	//     }
	//     else
	//     {
	// 		sprintf(m_LogString,"标志错误[%d]",save_flag);
	// 		MessShow(m_LogString,LTERROR);
	// 		return -1;
	//     }

	return 0;
}

//************************************
int CLfoAnaObj::UpdateOscMonDev(vector<LFO_DEV_INFO>& VecDev, LFO_SYS_INFO& oscMonSys, const int threadidx)
{
	// 	int ii,jj,kk,ll,nFlag;
	// 	
	// 	// 对系统状态，若有一个频率段系统小扰动统计状态为告警，则汇总后系统状态即为告警。当所有频率段都为安全时，汇总后系统状态为安全。
	// 	for (ii =0; ii<g_Thread_nun; ii++)
	// 	{
	// 		// 对监视设备信息，若有一个频率段设备状态为告警，则汇总后该设备状态为告警。当所有频率段都为安全时，汇总后设备状态为安全。
	// 		for (jj =0; jj<(int)VecDev.size(); jj++)
	// 		{
	// 			for (kk =0; kk<(int)m_pLfoThreadInfo[ii].VecDev.size(); kk++)
	// 			{
	// 				if (VecDev[jj].dev.dev_id == m_pLfoThreadInfo[ii].VecDev[kk].dev.dev_id)
	// 				{
	// 					if (m_pLfoThreadInfo[ii].VecDev[kk].osc_stat > VecDev[jj].osc_stat)
	// 					{
	// 						VecDev[jj].osc_stat = m_pLfoThreadInfo[ii].VecDev[kk].osc_stat;
	// 					}
	// 					if (VecDev[jj].mode.engy < m_pLfoThreadInfo[ii].VecDev[kk].mode.engy)
	// 					{
	// 						// 若该设备存在多个频率段告警，则保存能量最大的模式。
	// 						VecDev[jj].mode = m_pLfoThreadInfo[ii].VecDev[kk].mode;
	// 					}
	// 
	// 					if (VecDev[jj].curve.max_ampl < m_pLfoThreadInfo[ii].VecDev[kk].curve.max_ampl)
	// 					{
	// 						// 对功率波动值，取各频率段功率波动值中的最大值。
	// 						VecDev[jj].curve = m_pLfoThreadInfo[ii].VecDev[kk].curve;
	// 					}
	// 					// 同一设备模式取并集,非空即增加
	// 					if (!m_pLfoThreadInfo[ii].VecDev[kk].vec_mode.empty())
	// 					{
	// 						for (ll =0; ll<(int)m_pLfoThreadInfo[ii].VecDev[kk].vec_mode.size(); ll++)
	// 						{
	// 							VecDev[jj].vec_mode.push_back(m_pLfoThreadInfo[ii].VecDev[kk].vec_mode[ll]);
	// 						}
	// 					}
	// 					// 同一设备告警模式取并集
	// 					if (!m_pLfoThreadInfo[ii].VecDev[kk].vec_lead_mode.empty())
	// 					{
	// 						for (ll =0; ll<(int)m_pLfoThreadInfo[ii].VecDev[kk].vec_lead_mode.size(); ll++)
	// 						{
	// 							VecDev[jj].vec_lead_mode.push_back(m_pLfoThreadInfo[ii].VecDev[kk].vec_lead_mode[ll]);
	// 						}
	// 					}
	// 					break;
	// 				}
	// 			}
	// 		}
	// 
	// 		if (m_pLfoThreadInfo[0].oscMonSys.stat >= MENU_LFO_DEV_STS_WRN 
	// 			&& m_pLfoThreadInfo[1].oscMonSys.stat >= MENU_LFO_DEV_STS_WRN 
	// 			&& m_pLfoThreadInfo[2].oscMonSys.stat >= MENU_LFO_DEV_STS_WRN )
	// 		{
	// 			// 三频段均告警,取最近更新状态
	// 			oscMonSys.start_time = m_pLfoThreadInfo[threadidx].oscMonSys.start_time;
	// 			oscMonSys.end_time = m_pLfoThreadInfo[threadidx].oscMonSys.end_time;
	// 			oscMonSys.gps_time = m_pLfoThreadInfo[threadidx].oscMonSys.gps_time;
	// 			oscMonSys.window_id = m_pLfoThreadInfo[threadidx].oscMonSys.window_id;
	// 			oscMonSys.freq_min = m_pLfoThreadInfo[threadidx].oscMonSys.freq_min;
	// 			oscMonSys.freq_max = m_pLfoThreadInfo[threadidx].oscMonSys.freq_max;
	// 			oscMonSys.lead_dev = m_pLfoThreadInfo[threadidx].oscMonSys.lead_dev;
	// 			oscMonSys.stat = m_pLfoThreadInfo[threadidx].oscMonSys.stat;
	// 		}
	// 		else
	// 		{
	// 			// TODO:2段告警暂不考虑
	// 			// 1段告警或三段均安全，取状态最大段
	// 			// 若三个频率段均安全,则取第三个频率段的结果(默认将低频振荡频率段作为第三个频率段)
	// 			if (m_pLfoThreadInfo[ii].oscMonSys.stat >= oscMonSys.stat)
	// 			{
	// 				oscMonSys.start_time = m_pLfoThreadInfo[ii].oscMonSys.start_time;
	// 				oscMonSys.end_time = m_pLfoThreadInfo[ii].oscMonSys.end_time;
	// 				oscMonSys.gps_time = m_pLfoThreadInfo[ii].oscMonSys.gps_time;
	// 				oscMonSys.window_id = m_pLfoThreadInfo[ii].oscMonSys.window_id;
	// 				oscMonSys.freq_min = m_pLfoThreadInfo[ii].oscMonSys.freq_min;
	// 				oscMonSys.freq_max = m_pLfoThreadInfo[ii].oscMonSys.freq_max;
	// 				oscMonSys.lead_dev = m_pLfoThreadInfo[ii].oscMonSys.lead_dev;
	// 				oscMonSys.stat = m_pLfoThreadInfo[ii].oscMonSys.stat;
	// 			}
	// 		}
	// 
	// 		// 振荡设备取并集，设备可重复
	// 		for (jj=0; jj<(int)m_pLfoThreadInfo[ii].oscMonSys.vec_dev.size(); jj++)
	// 		{
	// 			nFlag = 0;
	// 			for (kk =0; kk<(int)oscMonSys.vec_dev.size(); kk++)
	// 			{
	// 				// 找到对应设备
	// 				if (oscMonSys.vec_dev[kk].dev.dev_id == m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].dev.dev_id)
	// 				{
	// 					// 波形取最大值
	// 					if (oscMonSys.vec_dev[kk].curve.max_ampl < m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].curve.max_ampl)
	// 					{
	// 						oscMonSys.vec_dev[kk].curve = m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].curve;
	// 					}
	// 					// 取能量最大模式
	// 					if (oscMonSys.vec_dev[kk].mode.engy < m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].mode.engy)
	// 					{
	// 						oscMonSys.vec_dev[kk].mode = m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].mode;
	// 					}
	// 					// 同一设备模式取并集,非空即增加
	// 					if (!m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].vec_mode.empty())
	// 					{
	// 						for (ll =0; ll<(int)m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].vec_mode.size(); ll++)
	// 						{
	// 							oscMonSys.vec_dev[kk].vec_mode.push_back(m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].vec_mode[ll]);
	// 						}
	// 					}
	// 					// 同一设备告警模式取并集
	// 					if (!m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].vec_lead_mode.empty())
	// 					{
	// 						for (ll =0; ll<(int)m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].vec_lead_mode.size(); ll++)
	// 						{
	// 							oscMonSys.vec_dev[kk].vec_lead_mode.push_back(m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj].vec_lead_mode[ll]);
	// 						}
	// 					}
	// 					nFlag =1;
	// 					break;
	// 				}
	// 			}
	// 			if (nFlag == 0)
	// 			{
	// 				// 未找到设备，新增
	// 				oscMonSys.vec_dev.push_back(m_pLfoThreadInfo[ii].oscMonSys.vec_dev[jj]);
	// 			}
	// 		}
	// 		
	// 		// 振荡模态取并集,频率段不同，模式定不同，直接取并集
	// 		for (jj=0; jj<(int)m_pLfoThreadInfo[ii].oscMonSys.vec_modeshape.size(); jj++)
	// 		{
	// 			oscMonSys.vec_modeshape.push_back(m_pLfoThreadInfo[ii].oscMonSys.vec_modeshape[jj]);
	// 		}
	// 	}
	// 
	// 	// 小扰动统计设备按振幅由大到小排序
	// 	if (!oscMonSys.vec_dev.empty())
	// 	{
	// 		sort(oscMonSys.vec_dev.begin(),oscMonSys.vec_dev.end(),g_SortDevByEngy);
	// 	}
	// 
	// 	// 振荡模态按能量由大到小排序
	// 	if (!oscMonSys.vec_modeshape.empty())
	// 	{
	// 		sort(oscMonSys.vec_modeshape.begin(),oscMonSys.vec_modeshape.end(),g_SortModeShapeByEngy);
	// 	}
	return 0;
}
//************************************
int CLfoAnaObj::CacheOscWindowsData(LFO_SYS_INFO MonSys, vector<LFO_SYS_INFO>& VecLfoWindow)
{
	ClearLfoSysInfo(VecLfoWindow);
	VecLfoWindow.push_back(MonSys);
	return 0;
}

int CLfoAnaObj::SaveOscReult(LFO_CASE_INFO& LfoCase)
{
	//按照事件存储统计意义上的振荡设备信息
	// 	int ii;
	// 	LPO_DEV_INFO tmpLPODEVINFO;
	// 	if (!tmpLPODEVINFO.v2d_data.empty())
	// 	{
	// 		vector<float>().swap(tmpLPODEVINFO.v2d_data);
	// 		tmpLPODEVINFO.v2d_data.clear();
	// 	}
	// 	memset((void *)&tmpLPODEVINFO,0,sizeof(LPO_DEV_INFO));
	// 	vector<LPO_DEV_INFO> LPODEVINFO;
	// 	if ( !LPODEVINFO.empty() )
	// 	{
	// 		for ( ii = 0; ii<(int)LPODEVINFO.size(); ii++)
	// 		{
	// 			if ( !LPODEVINFO[ii].v2d_data.empty() )
	// 			{
	// 				vector<float>().swap(LPODEVINFO[ii].v2d_data);
	// 				LPODEVINFO[ii].v2d_data.clear();
	// 			}
	// 		}
	// 		LPODEVINFO.clear();
	// 	}
	// 
	// 	int dev_num=0;
	// 	int ordernum=0;
	// 	char filename[MAXBUF];
	// 	char filenamebas[MAXBUF];
	// 	char localdir[MAXBUF];
	// 	char servdir[MAXBUF];
	// 	tab_lfo_pmu_row tmp_pmu_row;
	// 	if (!tmp_pmu_row.vec_value.empty())
	// 	{
	// 		vector<float>().swap(tmp_pmu_row.vec_value);
	// 		tmp_pmu_row.vec_value.clear();
	// 	}
	// 	memset((void *)&tmp_pmu_row,0,sizeof(tab_lfo_pmu_row));
	// 	time_t tsec = PT_GetNowSeconds();
	// 	INT_REC StartTime = LfoCase.start_time;
	// 	INT_REC EndTime = LfoCase.end_time;
	// 	sprintf(filename,"psave_%s.csv",PT_GetNoSpaceMarkTimeStringBySeconds(tsec));
	// 	sprintf(m_LogString,"提示:本轮获取数据文件名称[%s],系统开始时间["INT_REC_FMT"]结束时间["INT_REC_FMT"]!",filename,StartTime,EndTime);
	// 	MessShow(m_LogString,LTINFO);
	// #ifdef _WINDOWS32
	// 	sprintf( filenamebas, "%s\\res\\%s", g_dir.c_str(),filename );
	// #else
	// 	sprintf( filenamebas, "%s/res/%s", g_dir.c_str(),filename );
	// #endif
	// 	int int1=(int)LfoCase.vec_dev.size();
	//   	sprintf(m_LogString,"提示:小扰动统计设备数目=[%d]!",int1);
	//   	MessShow(m_LogString,LTINFO);
	// 	float ampl_temp=0.0;
	// 	float time_temp=0.0;
	// 	float time_file=0.0;
	// 	for ( int ii =0; ii<(int)LfoCase.vec_dev.size(); ii++ )
	// 	{
	//  		ampl_temp = LfoCase.vec_dev[ii].mode.ampl;
	// 		time_temp = LfoCase.vec_dev[ii].osc_end_time-LfoCase.vec_dev[ii].osc_start_time;
	// // 		sprintf(m_LogString,"提示:该设备小扰动振幅=[%f][%f],时间=["INT_REC_FMT"]-["INT_REC_FMT"]=[%d],[%f]!",ampl_temp,m_LfoPara.osc_pow_ampl,LfoCase.vec_dev[ii].osc_start_time,LfoCase.vec_dev[ii].osc_end_time,LfoCase.vec_dev[ii].osc_hold_time,m_LfoPara.osc_sec_ampl);
	// //  	MessShow(m_LogString,LTINFO);	
	// 		if ( (ampl_temp - m_LfoPara.osc_pow_ampl)>0.01 && (time_temp - m_LfoPara.osc_sec_ampl)>0.001 )
	// 		{
	// 			dev_num = dev_num+1;
	// // 		  	sprintf(m_LogString,"提示:dev_idx=["INT_REC_FMT"],fac_id=["INT_REC_FMT"],fac_name=[%s]!\n",VecMonDev[ii].dev.dev_idx,VecMonDev[ii].dev_info.fac_id,VecMonDev[ii].dev_info.fac_name);
	// //          MessShow(m_LogString,LTINFO);
	// //			tmpLPODEVINFO.dev_idx = VecMonDev[ii].dev.dev_idx;
	// 			tmpLPODEVINFO.dev_id = LfoCase.vec_dev[ii].dev.dev_id;
	// 			tmpLPODEVINFO.fac_id_1 = LfoCase.vec_dev[ii].dev.fac_id;
	// 			tmpLPODEVINFO.area_id_1 = LfoCase.vec_dev[ii].dev.area_id; 			
	// 			sprintf(tmpLPODEVINFO.dev_name,"%s",LfoCase.vec_dev[ii].dev.dev_name);   
	// 			sprintf(tmpLPODEVINFO.fac_name_1,"%s",LfoCase.vec_dev[ii].dev.fac_name); 
	// 			sprintf(tmpLPODEVINFO.area_name_1,"%s",LfoCase.vec_dev[ii].dev.area_name);
	// 			sprintf(tmpLPODEVINFO.fac_name_2,"%s",LfoCase.vec_dev[ii].dev.dot_fac_name); 
	// 			sprintf(tmpLPODEVINFO.area_name_2,"%s",LfoCase.vec_dev[ii].dev.dot_area_name);
	// 			tmpLPODEVINFO.dev_type = LfoCase.vec_dev[ii].dev.dev_type;     
	// // 			tmpLPODEVINFO[dev_num].is_monitor = VecMonDev[ii].is_monitor;     
	// // 			tmpLPODEVINFO[dev_num].is_part_ana = VecMonDev[ii].is_prony; 
	// 			tmpLPODEVINFO.lead_ampl = LfoCase.vec_dev[ii].mode.ampl;      
	// //			tmpLPODEVINFO.lead_ampl = LfoCase.vec_dev[ii].mode.aver_val;
	// 			tmpLPODEVINFO.lead_freq = LfoCase.vec_dev[ii].mode.freq;  
	// 			tmpLPODEVINFO.lead_damp = LfoCase.vec_dev[ii].mode.damp;     
	// 			tmpLPODEVINFO.lead_phas = LfoCase.vec_dev[ii].mode.phas;     
	// 			tmpLPODEVINFO.lead_real = LfoCase.vec_dev[ii].mode.real;     
	// 			tmpLPODEVINFO.lead_imag = LfoCase.vec_dev[ii].mode.imag;     
	// 			tmpLPODEVINFO.lead_engy = LfoCase.vec_dev[ii].mode.engy;     
	// 			tmpLPODEVINFO.if_lfo    = LfoCase.vec_dev[ii].stat;    
	// 			tmpLPODEVINFO.slfo_bgn_t = LfoCase.vec_dev[ii].osc_start_time;     
	// 			tmpLPODEVINFO.slfo_end_t = LfoCase.vec_dev[ii].osc_end_time;    
	// 			sprintf(tmpLPODEVINFO.file_dir,"var/dsa_wams/curve/%s",filename);   			
	// 			strcpy(tmpLPODEVINFO.slfo_time, PT_GetStringOfTimeInterval(LfoCase.vec_dev[ii].osc_hold_time*1000));
	// 			printf("提示=[%d]!\n",LfoCase.vec_dev[ii].back_data.size());			
	// //			tmpLPODEVINFO.v2d_data = VecMonDev[ii].back_data;
	// //			获取设备数据
	// //			sprintf(m_LogString,"提示:小干扰获取设备时间=["INT_REC_FMT"]-["INT_REC_FMT"]",m_oscMonSys.start_time,m_oscMonSys.end_time);
	// 			sprintf(m_LogString,"提示:小干扰获取设备["INT_REC_FMT"]数据时间=["INT_REC_FMT"]-["INT_REC_FMT"]",tmpLPODEVINFO.dev_id,StartTime, EndTime);
	//    			MessShow(m_LogString,LTINFO);
	// 			ordernum = 12;
	// 			LfoCase.vec_dev[ii].stat = MENU_LFO_DEV_STS_OK;
	// 			LfoCase.vec_dev[ii].v2d_data.clear();
	// 			if(m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
	// 			{
	// 				// 从数据库中读取PMU数据
	// 				sprintf(m_LogString,"小扰动统计读取数据库获取["INT_REC_FMT"]设备的PMU数据",LfoCase.vec_dev[ii].dev.dev_id);
	// 				MessShow(m_LogString,LTINFO);				
	// 				GetDevDataFromDB(StartTime,EndTime,ordernum,LfoCase.vec_dev[ii]);
	// // 				for ( int jjkk =0; jjkk<(int)LfoCase.vec_dev[ii].v2d_data.size(); jjkk++)
	// // 				{
	// // 					sprintf(m_LogString,"提示1：有功功率[ %8.2f ]",LfoCase.vec_dev[ii].v2d_data[jjkk]);
	// // 					MessShow(m_LogString,LTINFO);
	// // 				}
	// 			}
	// 			else
	// 			{
	// 				// 从数据文件中读取PMU数据
	// 				CDataFile df;
	// 				time_file = EndTime-StartTime;
	// 				sprintf(m_LogString,"小扰动统计模块读取数据文件[%s],起始[%d],时间长度[%f]",m_LfoPara.datafile_name,m_LfoPara.data_pos_osc,time_file);
	// 				MessShow(m_LogString,LTINFO);
	// 				if (df.ReadFileData(m_LfoPara.datafile_name,m_LfoPara.data_pos_osc,time_file,m_LfoPara.int_time) == false)
	// 				{
	// 					sprintf(m_LogString,"小扰动统计模块读取数据文件失败[%s][%s]",m_LfoPara.datafile_name,df.GetLastError());
	// 					MessShow(m_LogString,LTERROR);
	// 					return -1;
	// 				}
	// 				int dii,djj,tableno,nColNum;
	// 				string::size_type idx;
	// 				string tmpdevname,tmpColName;
	// 				string tmpstring;		
	// 				if (LfoCase.vec_dev[ii].dev.dev_id >0 )
	// 				{
	// 					//修改读取数据模块
	// 					for (djj=0; djj<(int)df.m_DataHead.vec_devid_s.size();djj++)
	// 					{
	// 						// 获取设备ID
	// 						tmpstring = df.m_DataHead.vec_devid_s[djj];
	// 						INT_REC tmpdev_id = a_to_d(tmpstring.c_str());
	// 						if (tmpdev_id == LfoCase.vec_dev[ii].dev.dev_id)
	// 						{
	// 							// 去掉前两列"日期时间,毫秒"
	// 							//tmpstring.clear();
	// 							if (df.GetNumberOfVariables()>= djj+2)
	// 							{
	// 								df.GetVariableName(djj+2,tmpstring);
	// 							}
	// 							idx = tmpstring.find_last_of("-");
	// 							// 获取设备名称
	// 							if (idx != string::npos)
	// 							{
	// 								tmpdevname = tmpstring.substr(0,idx);
	// 							}
	// 							// 判断测点类型
	// 							tmpColName = tmpstring.substr(idx+1);
	// 							// 去掉每行末尾的","
	// 							idx = tmpColName.find_first_of(",");
	// 							if (idx != string::npos)
	// 							{
	// 								tmpColName = tmpColName.substr(0,idx);
	// 							}
	// 							if (tmpColName == "00P")
	// 							{
	// 								// 有功
	// 								nColNum = 12;
	// 							}
	// 							else if (tmpColName == "00Q")
	// 							{
	// 								// 无功
	// 								nColNum = 13;
	// 							}
	// 							else if (tmpColName == "UAV相角")
	// 							{
	// 								// 电压相角
	// 								nColNum = 1;
	// 							}
	// 							else if (tmpColName == "UAV幅值")
	// 							{
	// 								// 电压幅值
	// 								nColNum = 0;
	// 							}
	// 							else if (tmpColName == "EEV相角")
	// 							{
	// 								// 发电机功角
	// 								nColNum = 16;
	// 							}
	// 							else
	// 							{
	// 								// 未定义，默认为无效
	// 								nColNum = -1;
	// 							}
	// 							
	// 							if (nColNum == ordernum)
	// 							{			
	// 								// 获取设备数据
	// 								if( df.GetData(djj+2,LfoCase.vec_dev[ii].v2d_data) == -1)
	// 								{
	// 									sprintf(m_LogString,"[%s]["INT_REC_FMT"]读取数据错误[%s]",
	// 										tmpstring.c_str(),tmpdev_id,df.GetLastError());
	// 									MessShow(m_LogString,LTDEBUG);
	// 								}
	// 								else
	// 								{
	// 									break;
	// 								}
	// 							}
	// 						}
	// 					}
	// 				}
	// 			}
	// 			if (!tmpLPODEVINFO.v2d_data.empty())
	// 			{
	// 				vector<float>().swap(tmpLPODEVINFO.v2d_data);
	// 				tmpLPODEVINFO.v2d_data.clear();
	// 			}
	// //			tmpLPODEVINFO.v2d_data.insert(tmpLPODEVINFO.v2d_data.end(),LfoCase.vec_dev[ii].v2d_data.begin(),LfoCase.vec_dev[ii].v2d_data.end());				
	// 			tmpLPODEVINFO.v2d_data.assign(LfoCase.vec_dev[ii].v2d_data.begin(), LfoCase.vec_dev[ii].v2d_data.end()); 
	// 			LPODEVINFO.push_back(tmpLPODEVINFO);
	// 			//added 写商用库
	//  			if(WritePmuSqlData(tmpLPODEVINFO)<0)
	//  			{
	// 				return 0;
	//  			}
	// 			//end added			 
	// 		}
	// 	}
	// 
	// 	string file_name;
	// //	file_name.clear();
	// 	int t_time;
	// 	int cts_time;
	// 	INT_REC cts_btime;
	// 	tab_lfo_pmu_data m_PMUData;
	// 	m_PMUData.head.begin_time = 0;
	// 	m_PMUData.head.end_time = 0;
	// 	if (!m_PMUData.head.vec_eleid.empty())
	// 	{
	// 		m_PMUData.head.vec_eleid.clear();
	// 	}
	// 	if (!m_PMUData.head.vec_elename.empty())
	// 	{
	// 		m_PMUData.head.vec_elename.clear();
	// 	}
	// 
	// 	for ( ii = 0; ii<(int)m_PMUData.vec_row.size(); ii++)
	// 	{	
	// 		m_PMUData.vec_row[ii].begin_time = 0;
	// 		m_PMUData.vec_row[ii].end_time = 0;
	// 
	// 		if (!m_PMUData.vec_row[ii].vec_value.empty())
	// 		{
	// 			vector<float>().swap(m_PMUData.vec_row[ii].vec_value);
	// 			m_PMUData.vec_row[ii].vec_value.clear();
	// 		}
	// 	}
	// /*	memset((void *)&m_PMUData,0,sizeof(LPO_DEV_INFO));*/
	// 
	// 	for (int iik=0; iik<(int)LPODEVINFO.size(); iik++)
	// 	{
	// 		sprintf(m_LogString,"写入PMU数据文件设备数目[%d],存储数据点数[%d],MAX_FRA_PER_SEC=[%d]!",LPODEVINFO.size(),LPODEVINFO[iik].v2d_data.size(),MAX_FRA_PER_SEC);
	// 		MessShow(m_LogString,LTINFO);
	// 	}
	// 
	// 	if ((int)LPODEVINFO.size()>0)
	// 	{
	// 		m_PMUData.head.begin_time = LfoCase.start_time;
	// 		m_PMUData.head.end_time = LfoCase.end_time;
	// 		m_PMUData.head.vec_eleid.clear();
	// 		m_PMUData.head.vec_elename.clear();
	// 		m_PMUData.vec_row.clear();
	// 		for ( ii=0; ii<(int)LPODEVINFO.size(); ii++ )
	// 		{
	// 			m_PMUData.head.vec_eleid.push_back(LPODEVINFO[ii].dev_id);
	// 			m_PMUData.head.vec_elename.push_back(LPODEVINFO[ii].dev_name);
	// 		}
	// 		for ( ii = 0; ii<(int)LPODEVINFO[0].v2d_data.size(); ii++)
	// 		{
	// 			//t_time = ii * MAX_FRA_PER_SEC;
	// 			//tmp_pmu_row.begin_time = m_PMUData.head.begin_time + t_time / 1000; 
	// 			//tmp_pmu_row.end_time   = t_time % 1000;
	// 			t_time = ii * (1.0/MAX_FRA_PER_SEC)*1000;
	// 			cts_btime = m_PMUData.head.begin_time + t_time / 1000;
	// 			tmp_pmu_row.begin_time = cts_btime; 
	// 			cts_time = t_time % 1000;
	// 			tmp_pmu_row.end_time = cts_time;
	// 			tmp_pmu_row.vec_value.clear();
	// 			for (int jj=0; jj<(int)LPODEVINFO.size(); jj++ )
	// 			{
	// 				if(ii<LPODEVINFO[jj].v2d_data.size())tmp_pmu_row.vec_value.push_back(LPODEVINFO[jj].v2d_data[ii]);
	// 			}
	// 			m_PMUData.vec_row.push_back(tmp_pmu_row);
	// 		}
	// //		sprintf(m_LogString,"yjj00启动函数WritePmuData!");
	// //		MessShow(m_LogString,LTINFO);		
	//  		if (WritePmuData( filenamebas,m_PMUData )<0)
	//  		{
	//  			sprintf(m_LogString,"写PMU数据文件失败");
	//  			MessShow(m_LogString,LTINFO);
	// 			return 0;
	//  		}
	// //		sprintf(m_LogString,"yjj00结束函数WritePmuData!");
	// //		MessShow(m_LogString,LTINFO);		
	// 		//added 上传文件至文件服务器
	// 	#ifdef _WINDOWS32
	// 		sprintf( localdir, "%s\\res\\", g_dir.c_str() );
	// 		sprintf( servdir, "%s\\", "var/dsa_wams/curve" );
	// 	#else
	// 		sprintf( localdir, "%s/res", g_dir.c_str());
	// 		sprintf( servdir, "%s", "var/dsa_wams/curve" );
	// 	#endif
	// //		file_name.push_back(filename);
	// 		file_name = filename; 
	// 		if (ExcuteFileUpnLoad(localdir,file_name,servdir)!= 0)//括号内参数请填写：本地文件路径,文件名称的集合，服务端文件路径
	// 		{
	// 			sprintf(m_LogString,"上传文件至文件服务器失败");
	// 			MessShow(m_LogString,LTINFO);
	// 			return 0;
	// 		}	
	// 		else
	// 		{
	// 			PF_RemoveFile(filenamebas); //删除文件
	// 		}
	// 		//end added
	// 	}
	return 0;
}

int CLfoAnaObj::JurgeLFO(LFO_SYS_INFO& MonSys, int stat)
{
	int if_lfo, lfo_end = 0;
	float max_ampl = 0.0;
	if (stat == 0)//判断是否需要发送低频振荡开始告警
	{
		for (int ii = 0; ii < MonSys.vec_dev.size(); ii++)
		{
			// 			sprintf(m_LogString,"该设备振幅为[%f]",MonSys.vec_dev[ii].mode.ampl);
			// 			MessShow(m_LogString,LTINFO);
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.gn_lfo_wn_ampl)
				{
					if (MonSys.vec_dev[ii].mode.ampl > max_ampl)
					{
						if (MonSys.vec_dev[ii].mode.freq <= m_LfoPara.lfo_freq_min)//频率小于0.2Hz，发低频振荡告警，大于0.2Hz，发大幅功率波动告警
						{
							sprintf(m_LogString, "该设备频率为[%f]<[%f]", MonSys.vec_dev[ii].mode.freq, m_LfoPara.lfo_freq_min);
							MessShow(m_LogString, LTINFO);
							if_lfo = 1;
						}
						else
						{
							sprintf(m_LogString, "该设备频率为[%f]>[%f]", MonSys.vec_dev[ii].mode.freq, m_LfoPara.lfo_freq_min);
							MessShow(m_LogString, LTINFO);
							if_lfo = 2;
						}
						max_ampl = MonSys.vec_dev[ii].mode.ampl;
						sprintf(MonSys.lfo_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动幅度[%6.2fMW],阻尼比[%6.3f]", MonSys.vec_dev[ii].dev.dev_name, MonSys.vec_dev[ii].mode.freq, MonSys.vec_dev[ii].curve.max_ampl, MonSys.vec_dev[ii].mode.damp);
					}
				}
				else
				{
					continue;
				}
			}
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				if (MonSys.vec_dev[ii].dev_info.maxvbase < m_LfoPara.volt_thre1)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.ln_lfo_swn_ampl1)
					{
						if (MonSys.vec_dev[ii].mode.ampl > max_ampl)
						{
							if (MonSys.vec_dev[ii].mode.freq <= m_LfoPara.lowfreq_min)//频率小于0.2Hz，发低频振荡告警，大于0.2Hz，发大幅功率波动告警
							{
								if_lfo = 1;
							}
							else
							{
								if_lfo = 2;
							}
							max_ampl = MonSys.vec_dev[ii].mode.ampl;
							sprintf(MonSys.lfo_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动幅度[%6.2fMW],阻尼比[%6.3f]", MonSys.vec_dev[ii].dev.dev_name, MonSys.vec_dev[ii].mode.freq, MonSys.vec_dev[ii].curve.max_ampl, MonSys.vec_dev[ii].mode.damp);
						}
					}
					else
					{
						continue;
					}
				}
				if (MonSys.vec_dev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre1 && MonSys.vec_dev[ii].dev_info.maxvbase < m_LfoPara.volt_thre2)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.ln_lfo_swn_ampl2)
					{
						if (MonSys.vec_dev[ii].mode.ampl > max_ampl)
						{
							if (MonSys.vec_dev[ii].mode.freq <= m_LfoPara.lfo_freq_min)//频率小于0.2Hz，发低频振荡告警，大于0.2Hz，发大幅功率波动告警
							{
								if_lfo = 1;
							}
							else
							{
								if_lfo = 2;
							}
							max_ampl = MonSys.vec_dev[ii].mode.ampl;
							sprintf(MonSys.lfo_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动幅度[%6.2fMW],阻尼比[%6.3f]", MonSys.vec_dev[ii].dev.dev_name, MonSys.vec_dev[ii].mode.freq, MonSys.vec_dev[ii].curve.max_ampl, MonSys.vec_dev[ii].mode.damp);
						}
					}
					else
					{
						continue;
					}
				}
				if (MonSys.vec_dev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre2)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.ln_lfo_swn_ampl3)
					{
						if (MonSys.vec_dev[ii].mode.ampl > max_ampl)
						{
							if (MonSys.vec_dev[ii].mode.freq <= m_LfoPara.lfo_freq_min)//频率小于0.2Hz，发低频振荡告警，大于0.2Hz，发大幅功率波动告警
							{
								if_lfo = 1;
							}
							else
							{
								if_lfo = 2;
							}
							max_ampl = MonSys.vec_dev[ii].mode.ampl;
							sprintf(MonSys.lfo_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动幅度[%6.2fMW],阻尼比[%6.3f]", MonSys.vec_dev[ii].dev.dev_name, MonSys.vec_dev[ii].mode.freq, MonSys.vec_dev[ii].curve.max_ampl, MonSys.vec_dev[ii].mode.damp);
						}
					}
					else
					{
						continue;
					}
				}
			}
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				if (MonSys.vec_dev[ii].dev_info.maxvbase < m_LfoPara.volt_thre1)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.tr_lfo_swn_ampl1)
					{
						if (MonSys.vec_dev[ii].mode.ampl > max_ampl)
						{
							if (MonSys.vec_dev[ii].mode.freq <= m_LfoPara.lfo_freq_min)//频率小于0.2Hz，发低频振荡告警，大于0.2Hz，发大幅功率波动告警
							{
								if_lfo = 1;
							}
							else
							{
								if_lfo = 2;
							}
							max_ampl = MonSys.vec_dev[ii].mode.ampl;
							sprintf(MonSys.lfo_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动幅度[%6.2fMW],阻尼比[%6.3f]", MonSys.vec_dev[ii].dev.dev_name, MonSys.vec_dev[ii].mode.freq, MonSys.vec_dev[ii].curve.max_ampl, MonSys.vec_dev[ii].mode.damp);
						}
					}
					else
					{
						continue;
					}
				}
				if (MonSys.vec_dev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre1 && MonSys.vec_dev[ii].dev_info.maxvbase < m_LfoPara.volt_thre2)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.tr_lfo_swn_ampl2)
					{
						if (MonSys.vec_dev[ii].mode.ampl > max_ampl)
						{
							if (MonSys.vec_dev[ii].mode.freq <= m_LfoPara.lfo_freq_min)//频率小于0.2Hz，发低频振荡告警，大于0.2Hz，发大幅功率波动告警
							{
								if_lfo = 1;
							}
							else
							{
								if_lfo = 2;
							}
							max_ampl = MonSys.vec_dev[ii].mode.ampl;
							sprintf(MonSys.lfo_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动幅度[%6.2fMW],阻尼比[%6.3f]", MonSys.vec_dev[ii].dev.dev_name, MonSys.vec_dev[ii].mode.freq, MonSys.vec_dev[ii].curve.max_ampl, MonSys.vec_dev[ii].mode.damp);
						}
					}
					else
					{
						continue;
					}
				}
				if (MonSys.vec_dev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre2)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.tr_lfo_swn_ampl3)
					{
						if (MonSys.vec_dev[ii].mode.ampl > max_ampl)
						{
							if (MonSys.vec_dev[ii].mode.freq <= m_LfoPara.lfo_freq_min)//频率小于0.2Hz，发低频振荡告警，大于0.2Hz，发大幅功率波动告警
							{
								if_lfo = 1;
							}
							else
							{
								if_lfo = 2;
							}
							max_ampl = MonSys.vec_dev[ii].mode.ampl;
							sprintf(MonSys.lfo_desc, "波动幅度最大设备[%s],振荡频率[%5.3fHz],功率波动幅度[%6.2fMW],阻尼比[%6.3f]", MonSys.vec_dev[ii].dev.dev_name, MonSys.vec_dev[ii].mode.freq, MonSys.vec_dev[ii].curve.max_ampl, MonSys.vec_dev[ii].mode.damp);
						}
					}
					else
					{
						continue;
					}
				}
			}
		}
		return if_lfo;
	}

	if (stat == 1)//判断是否需要发送低频振荡结束告警
	{
		lfo_end = 3;//先将lfo_end置为3,若有一个设备振幅大于结束限值,则将lfo_end置为0
		for (int ii = 0; ii < MonSys.vec_dev.size(); ii++)
		{
			// 			sprintf(m_LogString,"该设备振幅为[%f]",MonSys.vec_dev[ii].mode.ampl);
			// 			MessShow(m_LogString,LTINFO);
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
			{
				if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.gn_lfo_wn_ampl1)
				{
					lfo_end = 0;
					continue;
				}
				else
				{
					continue;
				}
			}
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
			{
				if (MonSys.vec_dev[ii].dev_info.maxvbase < m_LfoPara.volt_thre1)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.ln_lfo_ewn_ampl1)
					{
						lfo_end = 0;
						continue;
					}
					else
					{
						continue;
					}
				}
				if (MonSys.vec_dev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre1 && MonSys.vec_dev[ii].dev_info.maxvbase < m_LfoPara.volt_thre2)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.ln_lfo_ewn_ampl2)
					{
						lfo_end = 0;
						continue;
					}
					else
					{
						continue;
					}
				}
				if (MonSys.vec_dev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre2)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.ln_lfo_ewn_ampl3)
					{
						lfo_end = 0;
						continue;
					}
					else
					{
						continue;
					}
				}
			}
			if (MonSys.vec_dev[ii].dev.dev_type == MENU_LFO_DEV_TR)
			{
				if (MonSys.vec_dev[ii].dev_info.maxvbase < m_LfoPara.volt_thre1)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.tr_lfo_ewn_ampl1)
					{
						lfo_end = 0;
						continue;
					}
					else
					{
						continue;
					}
				}
				if (MonSys.vec_dev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre1 && MonSys.vec_dev[ii].dev_info.maxvbase < m_LfoPara.volt_thre2)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.tr_lfo_ewn_ampl2)
					{
						lfo_end = 0;
						continue;
					}
					else
					{
						continue;
					}
				}
				if (MonSys.vec_dev[ii].dev_info.maxvbase >= m_LfoPara.volt_thre2)
				{
					if (MonSys.vec_dev[ii].mode.ampl > m_LfoPara.tr_lfo_ewn_ampl3)
					{
						lfo_end = 0;
						continue;
					}
					else
					{
						continue;
					}
				}
			}
		}
		return lfo_end;
	}
	if (stat == 2)//本次振荡事件已经发送过开始和结束告警,直接返回0
	{
		return 0;
	}
	return 0;
}

int CLfoAnaObj::SampleLfoPara(LFO_CASE_INFO& LfoCase)
{
#ifndef _LFO_HIS_LIB_
#ifdef _PODB_SAMPLE_
	sprintf(m_LogString, "LFO参数表采样");
	MessShow(m_LogString, LTINFO);
	int ii;
	His_POW_HIS_DECI tmpLfoHis;
	vector<His_POW_HIS_DECI> tmpVecLfoHis;
	for (ii = 0; ii < LfoCase.vec_windows.size(); ii++)
	{
		memset((void*)&tmpLfoHis, 0, sizeof(His_POW_HIS_DECI));


		tmpLfoHis.record_id = PODB_GetDevIdByKeyID(LFO_PARA_NO_TAB, tmpVecLfoHis.size() + 1);
		tmpLfoHis.wds_bgn = LfoCase.vec_windows[ii].start_time;
		tmpLfoHis.wds_end = LfoCase.vec_windows[ii].end_time;
		tmpLfoHis.mode_num = LfoCase.vec_windows[ii].vec_modeshape.size();
		tmpLfoHis.lead_ampl = LfoCase.vec_windows[ii].lead_dev.mode.ampl;
		tmpLfoHis.lead_freq = LfoCase.vec_windows[ii].lead_dev.mode.freq;
		tmpLfoHis.lead_damp = LfoCase.vec_windows[ii].lead_dev.mode.damp;
		tmpLfoHis.lead_phas = LfoCase.vec_windows[ii].lead_dev.mode.phas;
		tmpLfoHis.lead_real = LfoCase.vec_windows[ii].lead_dev.mode.real;
		tmpLfoHis.lead_imag = LfoCase.vec_windows[ii].lead_dev.mode.imag;
		tmpLfoHis.lead_engy = LfoCase.vec_windows[ii].lead_dev.mode.engy;
		tmpLfoHis.lead_fail = LfoCase.vec_windows[ii].lead_dev.mode.fail;
		tmpLfoHis.lfo_bgn = LfoCase.start_time;
		tmpLfoHis.lfo_end = LfoCase.end_time;
		tmpLfoHis.warn_dev_id = LfoCase.warn_dev.dev.dev_id;
		tmpLfoHis.lfo_gn_num = LfoCase.vec_windows[ii].lfo_gn_num;
		tmpLfoHis.lfo_ln_num = LfoCase.vec_windows[ii].lfo_ln_num;
		tmpLfoHis.lfo_tr_num = LfoCase.vec_windows[ii].lfo_tr_num;
		tmpLfoHis.lfos_gn_num = LfoCase.vec_windows[ii].lfos_gn_num;
		tmpLfoHis.lfos_ln_num = LfoCase.vec_windows[ii].lfos_ln_num;
		tmpLfoHis.lfos_tr_num = LfoCase.vec_windows[ii].lfos_tr_num;
		tmpLfoHis.lfo_inf_num = LfoCase.vec_windows[ii].lfo_inf_num;
		if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
		{
			tmpLfoHis.save_type = 0;
		}
		else
		{
			tmpLfoHis.save_type = 1;
		}

		sprintf(tmpLfoHis.lfo_time, "%s", PT_GetStringOfTimeInterval(LfoCase.hold_time * 1000));
		sprintf(tmpLfoHis.warn_dev_name, "%s", LfoCase.warn_dev.dev.dev_name);
		sprintf(tmpLfoHis.lfos_gn_name, "%s", LfoCase.vec_windows[ii].lfos_gn_name);
		// 		sprintf(tmpLfoHis.lfos_des,"%s",LfoCase.hold_time);
		sprintf(tmpLfoHis.lfo_ana_des2, "%s", LfoCase.vec_windows[ii].center_desc);
		sprintf(tmpLfoHis.lfos_des, "%s", LfoCase.vec_windows[ii].lfos_des);

		tmpVecLfoHis.push_back(tmpLfoHis);
	}

	sprintf(m_LogString, "LFO参数表采样,记录数[%d]", tmpVecLfoHis.size());
	MessShow(m_LogString, LTINFO);
	vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
	TRIGGER_SAMPLE_DEFINE tmpTrigger;
	for (ii = 0; ii < tmpVecLfoHis.size(); ii++)
	{
		PODB_ClearSample(tmpTrigger);
		tmpTrigger.appno = g_appNo;
		tmpTrigger.mode = 0;
		tmpTrigger.occur_time = LfoCase.start_time;
		tmpTrigger.sampleno = HIS_POW_HIS_DECI_NO;
		tmpTrigger.tableno = LFO_PARA_NO_TAB;

		PODB_SmpBaseINT(tmpTrigger, 1, tmpVecLfoHis[ii].record_id);               //1   LONG[8]         记录ID                                     int_001
		PODB_SmpBaseINT(tmpTrigger, 59, tmpVecLfoHis[ii].wds_bgn);                 //59  DATETIME[8]     起始时刻                                   int_002
		PODB_SmpBaseINT(tmpTrigger, 60, tmpVecLfoHis[ii].wds_end);                 //60  DATETIME[8]     截至时刻                                   int_003
		PODB_SmpBaseINT(tmpTrigger, 61, tmpVecLfoHis[ii].mode_num);                //61  INT[4]          模式数                                     int_004
		PODB_SmpBaseFLT(tmpTrigger, 64, tmpVecLfoHis[ii].lead_ampl);                 //64  FLOAT[4]        振幅                                       float_001
		PODB_SmpBaseFLT(tmpTrigger, 65, tmpVecLfoHis[ii].lead_freq);                 //65  FLOAT[4]        频率                                       float_002
		PODB_SmpBaseFLT(tmpTrigger, 66, tmpVecLfoHis[ii].lead_damp);                 //66  FLOAT[4]        阻尼比                                     float_003
		PODB_SmpBaseFLT(tmpTrigger, 67, tmpVecLfoHis[ii].lead_phas);                 //67  FLOAT[4]        初相                                       float_004
		PODB_SmpBaseFLT(tmpTrigger, 68, tmpVecLfoHis[ii].lead_real);                 //68  FLOAT[4]        实部                                       float_005
		PODB_SmpBaseFLT(tmpTrigger, 69, tmpVecLfoHis[ii].lead_imag);                 //69  FLOAT[4]        虚部                                       float_006
		PODB_SmpBaseFLT(tmpTrigger, 70, tmpVecLfoHis[ii].lead_engy);                 //70  FLOAT[4]        能量                                       float_007
		PODB_SmpBaseFLT(tmpTrigger, 71, tmpVecLfoHis[ii].lead_fail);                 //71  FLOAT[4]        衰减                                       float_008
		PODB_SmpBaseSTR(tmpTrigger, 74, tmpVecLfoHis[ii].case_des);              //74  STRING[128]     振荡案例描述                               string_001
		PODB_SmpBaseINT(tmpTrigger, 75, tmpVecLfoHis[ii].lfo_bgn);                 //75  DATETIME[8]     LFO发生时间                                int_005
		PODB_SmpBaseINT(tmpTrigger, 76, tmpVecLfoHis[ii].lfo_end);                 //76  DATETIME[8]     LFO消失时间                                int_006
		PODB_SmpBaseSTR(tmpTrigger, 77, tmpVecLfoHis[ii].lfo_time);              //77  STRING[20]      持续时间                                   string_002
		PODB_SmpBaseINT(tmpTrigger, 78, tmpVecLfoHis[ii].warn_dev_id);             //78  LONG[8]         触发告警设备ID                             int_007
		PODB_SmpBaseSTR(tmpTrigger, 79, tmpVecLfoHis[ii].warn_dev_name);         //79  STRING[64]      触发告警设备                               string_003
		PODB_SmpBaseFLT(tmpTrigger, 80, tmpVecLfoHis[ii].warn_freq);                 //80  FLOAT[4]        触发告警频率                               float_009
		PODB_SmpBaseFLT(tmpTrigger, 81, tmpVecLfoHis[ii].aver_freq);                 //81  FLOAT[4]        主导平均频率                               float_010
		PODB_SmpBaseINT(tmpTrigger, 82, tmpVecLfoHis[ii].lfo_ln_num);              //82  INT[4]          LFO线路数                                  int_008
		PODB_SmpBaseINT(tmpTrigger, 83, tmpVecLfoHis[ii].lfo_gn_num);              //83  INT[4]          LFO发电机数                                int_009
		PODB_SmpBaseINT(tmpTrigger, 156, tmpVecLfoHis[ii].lfo_tr_num);              //156 INT[4]          LFO变压器数                                int_010
		PODB_SmpBaseSTR(tmpTrigger, 167, tmpVecLfoHis[ii].case_type_des);         //167 STRING[128]     振荡类型描述                               string_004
		PODB_SmpBaseINT(tmpTrigger, 170, tmpVecLfoHis[ii].save_type);               //170 INT[4]          案例类型                                   int_011
		PODB_SmpBaseSTR(tmpTrigger, 195, tmpVecLfoHis[ii].lfos_gn_name);          //195 STRING[64]      振荡源机组名称                             string_005
		PODB_SmpBaseINT(tmpTrigger, 196, tmpVecLfoHis[ii].lfos_gn_num);             //196 INT[4]          振荡源机组数目                             int_012
		PODB_SmpBaseINT(tmpTrigger, 197, tmpVecLfoHis[ii].lfos_ln_num);             //197 INT[4]          振荡源线路数目                             int_013
		PODB_SmpBaseINT(tmpTrigger, 198, tmpVecLfoHis[ii].lfos_tr_num);             //198 INT[4]          振荡源变压器数目                           int_014
		PODB_SmpBaseSTR(tmpTrigger, 199, tmpVecLfoHis[ii].lfos_des);              //199 STRING[128]     振荡源描述                                 string_006
		PODB_SmpBaseINT(tmpTrigger, 202, tmpVecLfoHis[ii].lfo_inf_num);             //202 INT[4]          LFO断面数                                  int_015
		PODB_SmpBaseSTR(tmpTrigger, 206, tmpVecLfoHis[ii].lfo_ana_des2);          //206 STRING[200]     振荡分析描述2                              string_007
		PODB_SmpBaseSTR(tmpTrigger, 207, tmpVecLfoHis[ii].asst_desc2);            //207 STRING[128]     辅助决策描述2                              string_008

		tmpVecTrigger.push_back(tmpTrigger);
	}
	PODB_TriggerSample(tmpVecTrigger);
#endif
#endif
	return 0;
}

int CLfoAnaObj::SampleHisEve(LFO_CASE_INFO& LfoCase)
{
#ifndef _LFO_HIS_LIB_
#ifdef _PODB_SAMPLE_
	His_POW_HIS_EVE tmpHisEve;
	memset((void*)&tmpHisEve, 0, sizeof(His_POW_HIS_EVE));

	tmpHisEve.eve_id = LfoCase.start_time;
	tmpHisEve.win_id = LfoCase.start_time;
	tmpHisEve.analy_bgn = LfoCase.start_time;
	tmpHisEve.analy_end = LfoCase.end_time;
	tmpHisEve.freq_max = m_LfoPara.freq_max;
	tmpHisEve.freq_min = m_LfoPara.freq_min;
	tmpHisEve.stat = LfoCase.stat;
	tmpHisEve.show_gn_num = LfoCase.lfo_gn_num;
	tmpHisEve.show_tr_num = LfoCase.lfo_tr_num;
	tmpHisEve.show_ln_num = LfoCase.lfo_ln_num;
	tmpHisEve.freq = LfoCase.lead_mode.freq;
	tmpHisEve.ampl = LfoCase.lead_mode.ampl;
	tmpHisEve.damp = LfoCase.lead_mode.damp;
	tmpHisEve.dev_num = LfoCase.lfo_gn_num + LfoCase.lfo_ln_num + LfoCase.lfo_tr_num;
	tmpHisEve.warn_dev_id = LfoCase.warn_dev.dev.dev_id;
	tmpHisEve.warn_dev_facid = LfoCase.warn_dev.dev.fac_id;
	tmpHisEve.lfos_gn_num = LfoCase.lfos_gn_num;
	tmpHisEve.lfos_ln_num = LfoCase.lfos_ln_num;
	tmpHisEve.lfos_tr_num = LfoCase.lfos_tr_num;
	tmpHisEve.show_int_num = LfoCase.lfo_inf_num;
	tmpHisEve.lfo_modle = LfoCase.lfo_modle;
	tmpHisEve.lfo_type = LfoCase.lfo_type;
	if (tmpHisEve.lfos_gn_num + tmpHisEve.lfos_ln_num + tmpHisEve.lfos_tr_num > 0)
	{
		tmpHisEve.is_lfo_source = MENU_LFO_MON_YES;
	}
	else
	{
		tmpHisEve.is_lfo_source = MENU_LFO_MON_NO;
	}

	sprintf(tmpHisEve.case_des, "%s", LfoCase.case_desc);
	sprintf(tmpHisEve.warn_dev_name, "%s", LfoCase.warn_dev.dev.dev_name);
	sprintf(tmpHisEve.lfo_time, "%s", PT_GetStringOfTimeInterval(LfoCase.hold_time * 1000));
	sprintf(tmpHisEve.warn_dev_facname, "%s", LfoCase.warn_dev.dev.fac_name);
	sprintf(tmpHisEve.lfos_gn_name, "%s", LfoCase.lfos_gn_name);
	sprintf(tmpHisEve.type_desc, "%s", LfoCase.type_desc);
	sprintf(tmpHisEve.lfo_ana_des, "%s", LfoCase.lfo_ana_des);
	if (strlen(LfoCase.center_desc) != 0)
	{
		sprintf(tmpHisEve.center_desc, "振荡中心:%s", LfoCase.center_desc);
	}
	if (m_LfoPara.data_source == LFO_DATA_SOURCE_REALTTIME)
	{
		tmpHisEve.save_type = 0;
	}
	else
	{
		tmpHisEve.save_type = 1;
	}
	sprintf(tmpHisEve.lfos_des, "%s", LfoCase.lfos_des);

	vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
	TRIGGER_SAMPLE_DEFINE tmpTrigger;
	PODB_ClearSample(tmpTrigger);
	tmpVecTrigger.clear();
	tmpTrigger.appno = g_appNo;
	tmpTrigger.mode = 1;//只采一条记录 非域级采样 表级采样 
	tmpTrigger.occur_time = LfoCase.start_time;
	tmpTrigger.sampleno = HIS_POW_HIS_EVE_NO;
	tmpTrigger.tableno = LFO_POW_EVE_NO;

	PODB_SmpBaseINT(tmpTrigger, 1, tmpHisEve.record_id);               //1   LONG[8]         记录ID                                     int_001
	PODB_SmpBaseINT(tmpTrigger, 2, tmpHisEve.eve_id);                  //2   DATETIME[4]     事件ID                                     int_002
	PODB_SmpBaseINT(tmpTrigger, 3, tmpHisEve.win_id);                  //3   DATETIME[4]     窗口号                                     int_003
	PODB_SmpBaseINT(tmpTrigger, 4, tmpHisEve.analy_bgn);               //4   DATETIME[4]     分析起始时间                               int_004
	PODB_SmpBaseINT(tmpTrigger, 5, tmpHisEve.analy_end);               //5   DATETIME[4]     分析截止时间                               int_005
	PODB_SmpBaseFLT(tmpTrigger, 6, tmpHisEve.freq_max);                  //6   FLOAT[4]        频率上限                                   float_001
	PODB_SmpBaseFLT(tmpTrigger, 7, tmpHisEve.freq_min);                  //7   FLOAT[4]        频率下限                                   float_002
	PODB_SmpBaseINT(tmpTrigger, 8, tmpHisEve.stat);                    //8   INT[4]          系统状态                                   int_006
	PODB_SmpBaseINT(tmpTrigger, 9, tmpHisEve.show_gn_num);             //9   INT[4]          振荡发电机个数                             int_007
	PODB_SmpBaseINT(tmpTrigger, 10, tmpHisEve.show_tr_num);             //10  INT[4]          振荡变压器个数                             int_008
	PODB_SmpBaseINT(tmpTrigger, 11, tmpHisEve.show_ln_num);             //11  INT[4]          振荡线路个数                               int_009
	PODB_SmpBaseFLT(tmpTrigger, 12, tmpHisEve.freq);                      //12  FLOAT[4]        主导模式频率                               float_003
	PODB_SmpBaseFLT(tmpTrigger, 13, tmpHisEve.ampl);                      //13  FLOAT[4]        振幅                                       float_004
	PODB_SmpBaseFLT(tmpTrigger, 14, tmpHisEve.damp);                      //14  FLOAT[4]        阻尼比                                     float_005
	PODB_SmpBaseINT(tmpTrigger, 15, tmpHisEve.dev_num);                 //15  INT[4]          参与设备个数                               int_010
	PODB_SmpBaseINT(tmpTrigger, 16, tmpHisEve.is_show);                 //16  INT[4]          是否显示                                   int_011
	PODB_SmpBaseSTR(tmpTrigger, 17, tmpHisEve.case_des);              //17  STRING[128]     振荡案例描述                               string_001
	PODB_SmpBaseINT(tmpTrigger, 18, tmpHisEve.ana_his_bgn);             //18  DATETIME[8]     离线分析起始时间                           int_012
	PODB_SmpBaseINT(tmpTrigger, 19, tmpHisEve.ana_his_end);             //19  DATETIME[8]     离线分析截止时间                           int_013
	PODB_SmpBaseINT(tmpTrigger, 20, tmpHisEve.warn_dev_id);             //20  LONG[8]         触发告警设备ID                             int_014
	PODB_SmpBaseINT(tmpTrigger, 21, tmpHisEve.warn_dev_facid);          //21  LONG[8]         触发告警设备厂站ID                         int_015
	PODB_SmpBaseSTR(tmpTrigger, 22, tmpHisEve.warn_dev_name);         //22  STRING[64]      触发告警设备名                             string_002
	PODB_SmpBaseSTR(tmpTrigger, 23, tmpHisEve.lfo_time);              //23  STRING[64]      持续时间                                   string_003
	PODB_SmpBaseSTR(tmpTrigger, 24, tmpHisEve.warn_dev_facname);      //24  STRING[64]      触发告警设备厂站                           string_004
	PODB_SmpBaseINT(tmpTrigger, 25, tmpHisEve.save_type);               //25  INT[4]          案例类型                                   int_016
	PODB_SmpBaseSTR(tmpTrigger, 26, tmpHisEve.lfos_gn_name);          //26  STRING[64]      振荡源机组名称                             string_005
	PODB_SmpBaseINT(tmpTrigger, 27, tmpHisEve.lfos_gn_num);             //27  INT[4]          振荡源机组数目                             int_017
	PODB_SmpBaseINT(tmpTrigger, 28, tmpHisEve.lfos_ln_num);             //28  INT[4]          振荡源线路数目                             int_018
	PODB_SmpBaseINT(tmpTrigger, 29, tmpHisEve.lfos_tr_num);             //29  INT[4]          振荡源变压器数目                           int_019
	PODB_SmpBaseINT(tmpTrigger, 30, tmpHisEve.show_int_num);            //30  INT[4]          振荡断面个数                               int_020
	PODB_SmpBaseINT(tmpTrigger, 31, tmpHisEve.lfo_modle);            //31  INT[4]          振荡模式                               int_021
	PODB_SmpBaseINT(tmpTrigger, 32, tmpHisEve.lfo_type);            //32  INT[4]          振荡类型                               int_022
	PODB_SmpBaseSTR(tmpTrigger, 33, tmpHisEve.type_desc);          //33  STRING[64]      振荡类型描述                             string_006
	PODB_SmpBaseSTR(tmpTrigger, 34, tmpHisEve.lfo_ana_des);          //34  STRING[128]      振荡分析描述                             string_007
	PODB_SmpBaseSTR(tmpTrigger, 35, tmpHisEve.center_desc);          //35  STRING[128]     振荡中心描述                               string_008
	PODB_SmpBaseSTR(tmpTrigger, 36, tmpHisEve.lfos_des);          //36  STRING[128]     振荡源描述                                 string_009
	PODB_SmpBaseSTR(tmpTrigger, 37, tmpHisEve.asst_desc);          //37  STRING[128]     辅助决策描述                               string_010
	PODB_SmpBaseINT(tmpTrigger, 38, tmpHisEve.is_lfo_source);            //38  INT[4]          是否振荡源                                 int_023

	tmpVecTrigger.push_back(tmpTrigger);
	sprintf(m_LogString, "LPO功率波动事件表采样,记录数[%d],occur_time="RTDB_TIME_FMT",eve_id="INT_REC_FMT"", tmpVecTrigger.size(), tmpTrigger.occur_time, INT_REC_VAL(tmpHisEve.eve_id));
	MessShow(m_LogString, LTINFO);

	PODB_TriggerSample(tmpVecTrigger);
#endif
#endif
	return 0;
}

int CLfoAnaObj::SampleHisCase(LFO_CASE_INFO& LfoCase)
{
#ifndef _LFO_HIS_LIB_
#ifdef _PODB_SAMPLE_
	int ii;
	His_POW_HIS_CASE tmpHisCase;
	vector<His_POW_HIS_CASE> tmpVecHisCase;

	for (ii = 0; ii < LfoCase.vec_dev.size(); ii++)
	{
		memset((void*)&tmpHisCase, 0, sizeof(His_POW_HIS_CASE));

		tmpHisCase.wds_id = PODB_GetDevIdByKeyID(LFO_POW_CASE_WDS_NO, tmpVecHisCase.size());
		tmpHisCase.eve_id = LfoCase.start_time;
		tmpHisCase.win_id = LfoCase.start_time;
		tmpHisCase.analy_bgn = LfoCase.vec_dev[ii].start_time;
		tmpHisCase.analy_end = LfoCase.vec_dev[ii].end_time;
		tmpHisCase.lfodev_id = LfoCase.vec_dev[ii].dev.dev_id;
		tmpHisCase.fac_id = LfoCase.vec_dev[ii].dev.fac_id;
		tmpHisCase.dev_type = LfoCase.vec_dev[ii].dev.dev_type;
		tmpHisCase.freq = LfoCase.vec_dev[ii].mode.freq;
		tmpHisCase.ampl = LfoCase.vec_dev[ii].mode.ampl;
		tmpHisCase.damp = LfoCase.vec_dev[ii].mode.damp;
		tmpHisCase.aver_val = LfoCase.vec_dev[ii].curve.aver_val;
		tmpHisCase.max_ampl_rat = LfoCase.vec_dev[ii].curve.max_ampl_rat;
		tmpHisCase.lfo_max_val = LfoCase.vec_dev[ii].curve.max_val;
		tmpHisCase.lfo_max_val_t = LfoCase.vec_dev[ii].curve.max_val_t;
		tmpHisCase.lfo_min_val = LfoCase.vec_dev[ii].curve.min_val;
		tmpHisCase.lfo_min_val_t = LfoCase.vec_dev[ii].curve.min_val_t;
		tmpHisCase.is_lfo_source = LfoCase.vec_dev[ii].mode.is_source;
		tmpHisCase.lead_pow = LfoCase.vec_dev[ii].mode.pow;
		sprintf(tmpHisCase.lfo_time, "%s", PT_GetStringOfTimeInterval(LfoCase.vec_dev[ii].hold_time * 1000));
		sprintf(tmpHisCase.lfodev_name, "%s", LfoCase.vec_dev[ii].dev.dev_name);
		sprintf(tmpHisCase.fac_name, "%s", LfoCase.vec_dev[ii].dev.fac_name);
		sprintf(tmpHisCase.yc_name, "%s", LfoCase.vec_dev[ii].mode.yc_name);

		tmpVecHisCase.push_back(tmpHisCase);
	}

	sprintf(m_LogString, "LPO功率波动窗口表采样,记录数[%d]", tmpVecHisCase.size());
	MessShow(m_LogString, LTINFO);
	vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
	TRIGGER_SAMPLE_DEFINE tmpTrigger;
	for (ii = 0; ii < tmpVecHisCase.size(); ii++)
	{
		PODB_ClearSample(tmpTrigger);
		tmpTrigger.appno = g_appNo;
		tmpTrigger.mode = 0;
		tmpTrigger.occur_time = LfoCase.start_time;
		tmpTrigger.sampleno = HIS_POW_HIS_CASE_NO;
		tmpTrigger.tableno = LFO_POW_CASE_WDS_NO;

		PODB_SmpBaseINT(tmpTrigger, 1, tmpVecHisCase[ii].wds_id);                  //1   LONG[8]         窗口ID                                     int_001
		PODB_SmpBaseFLT(tmpTrigger, 24, tmpVecHisCase[ii].freq_max);                  //24  PODB_SmpBaseFLT(tmpTrigger,[4]        频率上限                                   PODB_SmpBaseFLT(tmpTrigger,_001
		PODB_SmpBaseFLT(tmpTrigger, 25, tmpVecHisCase[ii].freq_min);                  //25  PODB_SmpBaseFLT(tmpTrigger,[4]        频率下限                                   PODB_SmpBaseFLT(tmpTrigger,_002
		PODB_SmpBaseINT(tmpTrigger, 26, tmpVecHisCase[ii].eve_id);                  //26  DATETIME[4]     事件ID                                     int_002
		PODB_SmpBaseINT(tmpTrigger, 27, tmpVecHisCase[ii].win_id);                  //27  DATETIME[4]     窗口号                                     int_003
		PODB_SmpBaseINT(tmpTrigger, 28, tmpVecHisCase[ii].analy_bgn);               //28  DATETIME[4]     分析起始时间                               int_004
		PODB_SmpBaseINT(tmpTrigger, 29, tmpVecHisCase[ii].analy_end);               //29  DATETIME[4]     分析截止时间                               int_005
		PODB_SmpBaseFLT(tmpTrigger, 31, tmpVecHisCase[ii].rphas);                     //31  PODB_SmpBaseFLT(tmpTrigger,[4]        初相                                       PODB_SmpBaseFLT(tmpTrigger,_006
		PODB_SmpBaseINT(tmpTrigger, 32, tmpVecHisCase[ii].lfodev_id);               //32  LONG[8]         振荡设备ID                                 int_006
		PODB_SmpBaseINT(tmpTrigger, 33, tmpVecHisCase[ii].fac_id);                  //33  LONG[8]         厂站ID                                     int_007
		PODB_SmpBaseINT(tmpTrigger, 34, tmpVecHisCase[ii].dev_type);                //34  INT[4]          设备类型                                   int_008
		PODB_SmpBaseFLT(tmpTrigger, 35, tmpVecHisCase[ii].freq);                      //35  PODB_SmpBaseFLT(tmpTrigger,[4]        频率                                       PODB_SmpBaseFLT(tmpTrigger,_003
		PODB_SmpBaseFLT(tmpTrigger, 36, tmpVecHisCase[ii].ampl);                      //36  PODB_SmpBaseFLT(tmpTrigger,[4]        振幅                                       PODB_SmpBaseFLT(tmpTrigger,_004
		PODB_SmpBaseFLT(tmpTrigger, 37, tmpVecHisCase[ii].damp);                      //37  PODB_SmpBaseFLT(tmpTrigger,[4]        阻尼比                                     PODB_SmpBaseFLT(tmpTrigger,_005
		PODB_SmpBaseFLT(tmpTrigger, 38, tmpVecHisCase[ii].aver_val);                  //38  PODB_SmpBaseFLT(tmpTrigger,[4]        平均值                                     PODB_SmpBaseFLT(tmpTrigger,_007
		PODB_SmpBaseFLT(tmpTrigger, 39, tmpVecHisCase[ii].max_ampl_rat);              //39  PODB_SmpBaseFLT(tmpTrigger,[4]        最大波动率                                 PODB_SmpBaseFLT(tmpTrigger,_008
		PODB_SmpBaseSTR(tmpTrigger, 40, tmpVecHisCase[ii].lfo_time);              //40  STRING[64]      持续时间                                   string_001
		PODB_SmpBaseFLT(tmpTrigger, 41, tmpVecHisCase[ii].lfo_max_val);               //41  PODB_SmpBaseFLT(tmpTrigger,[4]        LFO最大值                                  PODB_SmpBaseFLT(tmpTrigger,_009
		PODB_SmpBaseINT(tmpTrigger, 42, tmpVecHisCase[ii].lfo_max_val_t);           //42  DATETIME[8]     LFO最大值时间                              int_009
		PODB_SmpBaseFLT(tmpTrigger, 43, tmpVecHisCase[ii].lfo_min_val);               //43  PODB_SmpBaseFLT(tmpTrigger,[4]        LFO最小值                                  PODB_SmpBaseFLT(tmpTrigger,_010
		PODB_SmpBaseINT(tmpTrigger, 44, tmpVecHisCase[ii].lfo_min_val_t);           //44  DATETIME[8]     LFO最小值时间                              int_010
		PODB_SmpBaseSTR(tmpTrigger, 45, tmpVecHisCase[ii].lfodev_name);           //45  STRING[64]      振荡设备名称                               string_002
		PODB_SmpBaseSTR(tmpTrigger, 46, tmpVecHisCase[ii].fac_name);              //46  STRING[64]      厂站名称                                   string_003
		PODB_SmpBaseINT(tmpTrigger, 47, tmpVecHisCase[ii].is_lfo_source);           //47  INT[4]          是否振荡源                                 int_011
		PODB_SmpBaseFLT(tmpTrigger, 48, tmpVecHisCase[ii].lead_pow);                  //48  PODB_SmpBaseFLT(tmpTrigger,[4]        耗散功率                                   PODB_SmpBaseFLT(tmpTrigger,_011
		PODB_SmpBaseSTR(tmpTrigger, 49, tmpVecHisCase[ii].yc_name);              //49  STRING[64]      测点名称                                   string_004

		tmpVecTrigger.push_back(tmpTrigger);
	}

	PODB_TriggerSample(tmpVecTrigger);
#endif
#endif
	return 0;
}

int CLfoAnaObj::SampleHisShape(LFO_CASE_INFO& LfoCase)
{
#ifndef _LFO_HIS_LIB_
#ifdef _PODB_SAMPLE_
	int ii, jj;
	His_POW_HIS_SLLSHAPE tmpHisShape;
	vector<His_POW_HIS_SLLSHAPE> tmpVecHisShape;

	for (ii = 0; ii < LfoCase.vec_windows.size(); ii++)
	{
		for (jj = 0; jj < LfoCase.vec_windows[ii].vec_dev.size(); jj++)
		{
			memset((void*)&tmpHisShape, 0, sizeof(His_POW_HIS_SLLSHAPE));

			tmpHisShape.record_id = PODB_GetDevIdByKeyID(LFO_POW_WDS_SHAPE_NO, tmpVecHisShape.size() + 1);
			tmpHisShape.dev_id = LfoCase.vec_windows[ii].vec_dev[jj].dev.dev_id;
			tmpHisShape.dev_type = LfoCase.vec_windows[ii].vec_dev[jj].dev.dev_type;
			tmpHisShape.fac_id = LfoCase.vec_windows[ii].vec_dev[jj].dev.fac_id;
			tmpHisShape.area_id = LfoCase.vec_windows[ii].vec_dev[jj].dev.area_id;
			tmpHisShape.freq = LfoCase.vec_windows[ii].vec_dev[jj].mode.freq;
			tmpHisShape.damp = LfoCase.vec_windows[ii].vec_dev[jj].mode.damp;
			tmpHisShape.ampl = LfoCase.vec_windows[ii].vec_dev[jj].mode.ampl;
			tmpHisShape.engy = LfoCase.vec_windows[ii].vec_dev[jj].mode.engy;
			tmpHisShape.fact = LfoCase.vec_windows[ii].vec_dev[jj].mode.fact;
			tmpHisShape.clus = LfoCase.vec_windows[ii].vec_dev[jj].mode.group;
			tmpHisShape.eve_id = LfoCase.start_time;
			tmpHisShape.win_id = LfoCase.start_time;
			tmpHisShape.analy_bgn = LfoCase.vec_windows[ii].start_time;
			tmpHisShape.analy_end = LfoCase.vec_windows[ii].end_time;
			if (tmpHisShape.dev_type == MENU_LFO_DEV_ITFC)
			{
				tmpHisShape.is_lfo_source = LfoCase.vec_windows[ii].vec_dev[jj].is_sm_md;
				sprintf(tmpHisShape.fac_name, "%s", LfoCase.vec_windows[ii].vec_dev[jj].intline_des);
			}
			else
			{
				tmpHisShape.is_lfo_source = LfoCase.vec_windows[ii].vec_dev[jj].mode.is_source;
				sprintf(tmpHisShape.fac_name, "%s", LfoCase.vec_windows[ii].vec_dev[jj].dev.fac_name);
			}
			tmpHisShape.lead_pow = LfoCase.vec_windows[ii].vec_dev[jj].mode.pow;
			sprintf(tmpHisShape.dev_name, "%s", LfoCase.vec_windows[ii].vec_dev[jj].dev.dev_name);
			sprintf(tmpHisShape.area_name, "%s", LfoCase.vec_windows[ii].vec_dev[jj].dev.area_name);

			tmpVecHisShape.push_back(tmpHisShape);
		}
		for (jj = 0; jj < LfoCase.vec_windows[ii].cen_dev.size(); jj++)
		{
			memset((void*)&tmpHisShape, 0, sizeof(His_POW_HIS_SLLSHAPE));

			tmpHisShape.record_id = PODB_GetDevIdByKeyID(LFO_POW_WDS_SHAPE_NO, tmpVecHisShape.size() + 1);
			tmpHisShape.dev_id = LfoCase.vec_windows[ii].cen_dev[jj].dev.dev_id;
			tmpHisShape.dev_type = LfoCase.vec_windows[ii].cen_dev[jj].dev.dev_type;
			tmpHisShape.fac_id = LfoCase.vec_windows[ii].cen_dev[jj].dev.fac_id;
			tmpHisShape.area_id = LfoCase.vec_windows[ii].cen_dev[jj].dev.area_id;
			tmpHisShape.freq = LfoCase.vec_windows[ii].cen_dev[jj].mode.freq;
			tmpHisShape.damp = LfoCase.vec_windows[ii].cen_dev[jj].mode.damp;
			tmpHisShape.ampl = LfoCase.vec_windows[ii].cen_dev[jj].mode.ampl;
			tmpHisShape.engy = LfoCase.vec_windows[ii].cen_dev[jj].mode.engy;
			tmpHisShape.max_ampl = LfoCase.vec_windows[ii].cen_dev[jj].curve.max_ampl;
			tmpHisShape.min_volt = LfoCase.vec_windows[ii].cen_dev[jj].curve.min_val;
			tmpHisShape.eve_id = LfoCase.start_time;
			tmpHisShape.win_id = LfoCase.start_time;
			tmpHisShape.analy_bgn = LfoCase.vec_windows[ii].start_time;
			tmpHisShape.analy_end = LfoCase.vec_windows[ii].end_time;
			sprintf(tmpHisShape.dev_name, "%s", LfoCase.vec_windows[ii].cen_dev[jj].dev.dev_name);
			sprintf(tmpHisShape.area_name, "%s", LfoCase.vec_windows[ii].cen_dev[jj].dev.area_name);

			tmpVecHisShape.push_back(tmpHisShape);
		}
	}

	sprintf(m_LogString, "LPO功率波动模态全表采样,记录数[%d]", tmpVecHisShape.size());
	MessShow(m_LogString, LTINFO);
	vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
	TRIGGER_SAMPLE_DEFINE tmpTrigger;
	for (ii = 0; ii < tmpVecHisShape.size(); ii++)
	{
		PODB_ClearSample(tmpTrigger);
		tmpTrigger.appno = g_appNo;
		tmpTrigger.mode = 0;
		tmpTrigger.occur_time = LfoCase.start_time;
		tmpTrigger.sampleno = HIS_POW_HIS_SLLSHAPE_NO;
		tmpTrigger.tableno = LFO_POW_WDS_SHAPE_NO;

		PODB_SmpBaseINT(tmpTrigger, 1, tmpVecHisShape[ii].record_id);               //1   LONG[8]         记录号                                     int_001
		PODB_SmpBaseFLT(tmpTrigger, 10, tmpVecHisShape[ii].fact);                      //10  FLOAT[4]        参与因子                                   float_012
		PODB_SmpBaseINT(tmpTrigger, 12, tmpVecHisShape[ii].dev_id);                  //12  LONG[8]         设备ID                                     int_002
		PODB_SmpBaseSTR(tmpTrigger, 13, tmpVecHisShape[ii].dev_name);              //13  STRING[64]      设备名称                                   string_001
		PODB_SmpBaseINT(tmpTrigger, 14, tmpVecHisShape[ii].dev_type);                //14  INT[4]          设备类型                                   int_003
		PODB_SmpBaseINT(tmpTrigger, 15, tmpVecHisShape[ii].fac_id);                  //15  LONG[8]         所属厂站                                   int_004
		PODB_SmpBaseINT(tmpTrigger, 16, tmpVecHisShape[ii].area_id);                 //16  LONG[8]         所属区域                                   int_005
		PODB_SmpBaseINT(tmpTrigger, 19, tmpVecHisShape[ii].clus);                    //19  UPODB_SmpBaseSTR(tmpTrigger,[1]        所属群                                     int_006
		PODB_SmpBaseFLT(tmpTrigger, 20, tmpVecHisShape[ii].freq);                      //20  PODB_SmpBaseFLT(tmpTrigger,[4]        频率                                       PODB_SmpBaseFLT(tmpTrigger,_002
		PODB_SmpBaseFLT(tmpTrigger, 21, tmpVecHisShape[ii].damp);                      //21  PODB_SmpBaseFLT(tmpTrigger,[4]        阻尼比                                     PODB_SmpBaseFLT(tmpTrigger,_003
		PODB_SmpBaseFLT(tmpTrigger, 22, tmpVecHisShape[ii].ampl);                      //22  PODB_SmpBaseFLT(tmpTrigger,[4]        振幅                                       PODB_SmpBaseFLT(tmpTrigger,_004
		PODB_SmpBaseFLT(tmpTrigger, 23, tmpVecHisShape[ii].engy);                      //23  PODB_SmpBaseFLT(tmpTrigger,[4]        能量                                       PODB_SmpBaseFLT(tmpTrigger,_005
		PODB_SmpBaseFLT(tmpTrigger, 24, tmpVecHisShape[ii].aver_val);                  //24  PODB_SmpBaseFLT(tmpTrigger,[4]        平均值                                     PODB_SmpBaseFLT(tmpTrigger,_006
		PODB_SmpBaseFLT(tmpTrigger, 25, tmpVecHisShape[ii].max_ampl);                  //25  PODB_SmpBaseFLT(tmpTrigger,[4]        最大幅值                                   PODB_SmpBaseFLT(tmpTrigger,_007
		PODB_SmpBaseFLT(tmpTrigger, 28, tmpVecHisShape[ii].freq_max);                  //28  PODB_SmpBaseFLT(tmpTrigger,[4]        频率上限                                   PODB_SmpBaseFLT(tmpTrigger,_008
		PODB_SmpBaseFLT(tmpTrigger, 29, tmpVecHisShape[ii].freq_min);                  //29  PODB_SmpBaseFLT(tmpTrigger,[4]        频率下限                                   PODB_SmpBaseFLT(tmpTrigger,_009
		PODB_SmpBaseINT(tmpTrigger, 30, tmpVecHisShape[ii].eve_id);                  //30  DATETIME[4]     事件ID                                     int_007
		PODB_SmpBaseINT(tmpTrigger, 31, tmpVecHisShape[ii].win_id);                  //31  DATETIME[4]     窗口号                                     int_008
		PODB_SmpBaseINT(tmpTrigger, 32, tmpVecHisShape[ii].analy_bgn);               //32  DATETIME[4]     分析起始时间                               int_009
		PODB_SmpBaseINT(tmpTrigger, 33, tmpVecHisShape[ii].analy_end);               //33  DATETIME[4]     分析截止时间                               int_010
		PODB_SmpBaseFLT(tmpTrigger, 35, tmpVecHisShape[ii].iniphas);                   //35  PODB_SmpBaseFLT(tmpTrigger,[4]        初相                                       PODB_SmpBaseFLT(tmpTrigger,_010
		PODB_SmpBaseSTR(tmpTrigger, 36, tmpVecHisShape[ii].fac_name);              //36  STRING[64]      厂站名称                                   string_002
		PODB_SmpBaseSTR(tmpTrigger, 37, tmpVecHisShape[ii].area_name);             //37  STRING[64]      区域名称                                   string_003
		PODB_SmpBaseINT(tmpTrigger, 38, tmpVecHisShape[ii].is_lfo_source);           //38  INT[4]          是否振荡源                                 int_011
		PODB_SmpBaseFLT(tmpTrigger, 39, tmpVecHisShape[ii].lead_pow);                  //39  PODB_SmpBaseFLT(tmpTrigger,[4]        耗散功率                                   PODB_SmpBaseFLT(tmpTrigger,_010
		PODB_SmpBaseFLT(tmpTrigger, 40, tmpVecHisShape[ii].min_volt);                  //40  FLOAT[4]        最低电压                                   float_013
		tmpVecTrigger.push_back(tmpTrigger);
	}

	PODB_TriggerSample(tmpVecTrigger);
#endif
#endif
	return 0;
}

int CLfoAnaObj::SampleHisMode(LFO_CASE_INFO& LfoCase)
{
#ifndef _LFO_HIS_LIB_
#ifdef _PODB_SAMPLE_
	int ii, jj;
	His_LPO_ALL_MODE_WAMS tmpAllmode;
	vector<His_LPO_ALL_MODE_WAMS> tmpVecAllmode;

	His_LPO_ALLDEV_MODE_WAMS tmpDevmode;
	vector<His_LPO_ALLDEV_MODE_WAMS> tmpVecDevmode;

	for (ii = 0; ii < LfoCase.vec_mainmode.size(); ii++)
	{
		sprintf(m_LogString, "参与机组[%d],可观线路[%d],可观变压器[%d]", LfoCase.vec_mainmode[ii].gn_num, LfoCase.vec_mainmode[ii].line_num, LfoCase.vec_mainmode[ii].tr_mun);
		MessShow(m_LogString, LTINFO);
		memset((void*)&tmpAllmode, 0, sizeof(His_LPO_ALL_MODE_WAMS));
		tmpAllmode.record_id = PODB_GetDevIdByKeyID(LPO_ALL_MODE_WAMS_NO, tmpVecAllmode.size() + 1);
		if (ii == 0)
		{
			tmpAllmode.is_show = MENU_LFO_MENU_YES;
		}
		tmpAllmode.freq = LfoCase.vec_mainmode[ii].mode.freq;
		tmpAllmode.ampl = LfoCase.vec_mainmode[ii].mode.ampl;
		if (tmpAllmode.freq < m_LfoPara.lfo_freq_min)
		{
			sprintf(tmpAllmode.mode_des, "%s", "超低频振荡");
		}
		else if (tmpAllmode.freq > m_LfoPara.lfo_freq_max)
		{
			sprintf(tmpAllmode.mode_des, "%s", "高频振荡");
		}
		else
		{
			sprintf(tmpAllmode.mode_des, "%s", "低频振荡");
		}
		tmpAllmode.damp = LfoCase.vec_mainmode[ii].mode.damp;
		tmpAllmode.lfo_bgn = LfoCase.vec_mainmode[ii].mode.start_time;
		tmpAllmode.lfo_end = LfoCase.vec_mainmode[ii].mode.end_time;
		tmpAllmode.is_lfo_source = LfoCase.vec_mainmode[ii].is_lfo_source;
		tmpAllmode.eve_id = LfoCase.start_time;
		sprintf(tmpAllmode.lfo_ana_des, "%s", LfoCase.vec_mainmode[ii].lfo_ana_des);
		if (tmpAllmode.is_lfo_source == 1)
		{
			sprintf(tmpAllmode.is_source_des, "√");
		}
		else
		{
			sprintf(tmpAllmode.is_source_des, "×");
		}
		sprintf(tmpAllmode.lfo_time, "%s", PT_GetStringOfTimeInterval((tmpAllmode.lfo_end - tmpAllmode.lfo_bgn) * 1000));
		for (jj = 0; jj < LfoCase.vec_mainmode[ii].vec_dev.size(); jj++)
		{
			memset((void*)&tmpDevmode, 0, sizeof(His_LPO_ALLDEV_MODE_WAMS));
			tmpDevmode.record_id = PODB_GetDevIdByKeyID(LPO_ALLDEV_MODE_WAMS_NO, tmpVecDevmode.size() + 1);
			tmpDevmode.freq = LfoCase.vec_mainmode[ii].vec_dev[jj].mode.freq;
			tmpDevmode.ampl = LfoCase.vec_mainmode[ii].vec_dev[jj].mode.ampl;
			tmpDevmode.damp = LfoCase.vec_mainmode[ii].vec_dev[jj].mode.damp;
			tmpDevmode.mode_id = tmpAllmode.record_id;
			tmpDevmode.eve_id = LfoCase.start_time;
			tmpDevmode.dev_type = LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_type;
			tmpDevmode.dev_id = LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_id;
			tmpDevmode.is_source = LfoCase.vec_mainmode[ii].vec_dev[jj].mode.is_source;
			sprintf(tmpDevmode.dev_name, "%s", LfoCase.vec_mainmode[ii].vec_dev[jj].dev.dev_name);
			sprintf(tmpDevmode.yc_name, "%s", LfoCase.vec_mainmode[ii].vec_dev[jj].mode.yc_name);
			if (tmpDevmode.dev_type == MENU_LFO_DEV_GEN)
			{
				tmpAllmode.gn_num++;
			}
			else if (tmpDevmode.dev_type == MENU_LFO_DEV_LINE)
			{
				tmpAllmode.line_num++;
			}
			else if (tmpDevmode.dev_type == MENU_LFO_DEV_TR)
			{
				tmpAllmode.tr_mun++;
			}
			tmpVecDevmode.push_back(tmpDevmode);
		}
		LfoCase.vec_mainmode[ii].gn_num = tmpAllmode.gn_num;
		LfoCase.vec_mainmode[ii].line_num = tmpAllmode.line_num;
		LfoCase.vec_mainmode[ii].tr_mun = tmpAllmode.tr_mun;
		tmpVecAllmode.push_back(tmpAllmode);
	}

	vector<TRIGGER_SAMPLE_DEFINE> tmpVecTrigger;
	TRIGGER_SAMPLE_DEFINE tmpTrigger;

	sprintf(m_LogString, "LPO实时模式全表采样,记录数[%d]", tmpVecAllmode.size());
	MessShow(m_LogString, LTINFO);
	for (ii = 0; ii < tmpVecAllmode.size(); ii++)
	{
		PODB_ClearSample(tmpTrigger);
		tmpTrigger.appno = g_appNo;
		tmpTrigger.mode = 0;
		tmpTrigger.occur_time = LfoCase.start_time;
		tmpTrigger.sampleno = HIS_LPO_ALL_MODE_WAMS_NO;
		tmpTrigger.tableno = LPO_ALL_MODE_WAMS_NO;

		PODB_SmpBaseINT(tmpTrigger, 1, tmpVecAllmode[ii].record_id);               //1   LONG[8]         记录ID                                     int_001
		PODB_SmpBaseINT(tmpTrigger, 2, tmpVecAllmode[ii].gn_num);                  //2   INT[4]          参与机组数                                 int_002
		PODB_SmpBaseINT(tmpTrigger, 3, tmpVecAllmode[ii].line_num);                //3   INT[4]          可观支路数                                 int_003
		PODB_SmpBaseINT(tmpTrigger, 4, tmpVecAllmode[ii].is_show);                 //4   UCHAR[1]        是否显示                                   int_004
		PODB_SmpBaseFLT(tmpTrigger, 5, tmpVecAllmode[ii].ampl);                      //5   FLOAT[4]        振幅                                       float_001
		PODB_SmpBaseFLT(tmpTrigger, 6, tmpVecAllmode[ii].freq);                      //6   FLOAT[4]        频率                                       float_002
		PODB_SmpBaseFLT(tmpTrigger, 7, tmpVecAllmode[ii].damp);                      //7   FLOAT[4]        阻尼比                                     float_003
		PODB_SmpBaseFLT(tmpTrigger, 8, tmpVecAllmode[ii].phas);                      //8   FLOAT[4]        初相                                       float_004
		PODB_SmpBaseFLT(tmpTrigger, 9, tmpVecAllmode[ii].real_part);                 //9   FLOAT[4]        实部                                       float_005
		PODB_SmpBaseFLT(tmpTrigger, 10, tmpVecAllmode[ii].image_part);                //10  FLOAT[4]        虚部                                       float_006
		PODB_SmpBaseFLT(tmpTrigger, 11, tmpVecAllmode[ii].engy);                      //11  FLOAT[4]        能量                                       float_007
		PODB_SmpBaseFLT(tmpTrigger, 12, tmpVecAllmode[ii].mon_fail);                  //12  FLOAT[4]        衰减                                       float_008
		PODB_SmpBaseINT(tmpTrigger, 13, tmpVecAllmode[ii].mode_no);                 //13  INT[4]          模式号                                     int_005
		PODB_SmpBaseSTR(tmpTrigger, 14, tmpVecAllmode[ii].mode_des);              //14  STRING[128]     模式描述                                   string_001
		PODB_SmpBaseFLT(tmpTrigger, 15, tmpVecAllmode[ii].start_freq);                //15  FLOAT[4]        起振频率                                   float_009
		PODB_SmpBaseFLT(tmpTrigger, 16, tmpVecAllmode[ii].aver_freq);                 //16  FLOAT[4]        平均频率                                   float_010
		PODB_SmpBaseFLT(tmpTrigger, 17, tmpVecAllmode[ii].disp_freq);                 //17  FLOAT[4]        显示频率                                   float_011
		PODB_SmpBaseINT(tmpTrigger, 18, tmpVecAllmode[ii].lfo_bgn);                 //18  DATETIME[8]     发生时间                                   int_006
		PODB_SmpBaseINT(tmpTrigger, 19, tmpVecAllmode[ii].lfo_end);                 //19  DATETIME[8]     消失时间                                   int_007
		PODB_SmpBaseSTR(tmpTrigger, 20, tmpVecAllmode[ii].lfo_time);              //20  STRING[20]      持续时间                                   string_002
		PODB_SmpBaseSTR(tmpTrigger, 21, tmpVecAllmode[ii].warn_dev_name);         //21  STRING[64]      触发告警设备                               string_003
		PODB_SmpBaseINT(tmpTrigger, 22, tmpVecAllmode[ii].tr_mun);                  //22  INT[4]          可观变压器数                               int_008
		PODB_SmpBaseINT(tmpTrigger, 23, tmpVecAllmode[ii].inf_num);                 //23  INT[4]          可观断面数                                 int_009
		PODB_SmpBaseINT(tmpTrigger, 24, tmpVecAllmode[ii].if_lfo);                  //24  INT[4]          是否告警                                   int_010
		PODB_SmpBaseINT(tmpTrigger, 25, tmpVecAllmode[ii].warn_gn);                 //25  INT[4]          告警机组数                                 int_011
		PODB_SmpBaseINT(tmpTrigger, 26, tmpVecAllmode[ii].warn_ln);                 //26  INT[4]          告警线路数                                 int_012
		PODB_SmpBaseINT(tmpTrigger, 27, tmpVecAllmode[ii].warn_tr);                 //27  INT[4]          告警变压器数                               int_013
		PODB_SmpBaseINT(tmpTrigger, 28, tmpVecAllmode[ii].warn_inf);                //28  INT[4]          告警断面数                                 int_014
		PODB_SmpBaseINT(tmpTrigger, 29, tmpVecAllmode[ii].is_lfo_source);           //29  INT[4]          是否强迫振荡                               int_015
		PODB_SmpBaseSTR(tmpTrigger, 30, tmpVecAllmode[ii].is_source_des);         //30  STRING[8]       是否强迫振荡描述                           string_004
		PODB_SmpBaseSTR(tmpTrigger, 31, tmpVecAllmode[ii].lfo_ana_des);           //31  STRING[200]     振荡分析描述                               string_005
		PODB_SmpBaseINT(tmpTrigger, 32, tmpVecAllmode[ii].lfo_type);                //32  INT[4]          振荡类型                                   int_016
		PODB_SmpBaseSTR(tmpTrigger, 33, tmpVecAllmode[ii].lfo_type_des);          //33  STRING[32]      振荡类型描述                               string_006
		PODB_SmpBaseINT(tmpTrigger, 34, tmpVecAllmode[ii].eve_id);                //34  DATETIME[8]     事件ID                                     int_017
		tmpVecTrigger.push_back(tmpTrigger);
	}

	PODB_TriggerSample(tmpVecTrigger);

	sprintf(m_LogString, "LPO实时设备模式全表采样,记录数[%d]", tmpVecDevmode.size());
	MessShow(m_LogString, LTINFO);
	PODB_ClearSample(tmpVecTrigger);
	for (ii = 0; ii < tmpVecDevmode.size(); ii++)
	{
		PODB_ClearSample(tmpTrigger);
		tmpTrigger.appno = g_appNo;
		tmpTrigger.mode = 0;
		tmpTrigger.occur_time = LfoCase.start_time;
		tmpTrigger.sampleno = HIS_LPO_ALLDEV_MODE_WAMS_NO;
		tmpTrigger.tableno = LPO_ALLDEV_MODE_WAMS_NO;

		PODB_SmpBaseINT(tmpTrigger, 1, tmpVecDevmode[ii].record_id);               //1   LONG[8]         记录ID                                     int_001
		PODB_SmpBaseINT(tmpTrigger, 2, tmpVecDevmode[ii].dev_id);                  //2   LONG[8]         设备ID                                     int_002
		PODB_SmpBaseSTR(tmpTrigger, 3, tmpVecDevmode[ii].dev_name);              //3   STRING[64]      设备名称                                   string_001
		PODB_SmpBaseINT(tmpTrigger, 4, tmpVecDevmode[ii].dev_type);                //4   INT[4]          设备类型                                   int_003
		PODB_SmpBaseINT(tmpTrigger, 5, tmpVecDevmode[ii].is_show);                 //5   UCHAR[1]        是否显示                                   int_004
		PODB_SmpBaseFLT(tmpTrigger, 6, tmpVecDevmode[ii].ampl);                      //6   FLOAT[4]        振幅                                       float_001
		PODB_SmpBaseFLT(tmpTrigger, 7, tmpVecDevmode[ii].freq);                      //7   FLOAT[4]        频率                                       float_002
		PODB_SmpBaseFLT(tmpTrigger, 8, tmpVecDevmode[ii].damp);                      //8   FLOAT[4]        阻尼比                                     float_003
		PODB_SmpBaseFLT(tmpTrigger, 9, tmpVecDevmode[ii].phas);                      //9   FLOAT[4]        初相                                       float_004
		PODB_SmpBaseFLT(tmpTrigger, 10, tmpVecDevmode[ii].real_part);                 //10  FLOAT[4]        实部                                       float_005
		PODB_SmpBaseFLT(tmpTrigger, 11, tmpVecDevmode[ii].image_part);                //11  FLOAT[4]        虚部                                       float_006
		PODB_SmpBaseFLT(tmpTrigger, 12, tmpVecDevmode[ii].engy);                      //12  FLOAT[4]        能量                                       float_007
		PODB_SmpBaseFLT(tmpTrigger, 13, tmpVecDevmode[ii].mon_fail);                  //13  FLOAT[4]        衰减                                       float_008
		PODB_SmpBaseINT(tmpTrigger, 14, tmpVecDevmode[ii].mode_id);                 //14  LONG[8]         模式号                                     int_005
		PODB_SmpBaseFLT(tmpTrigger, 15, tmpVecDevmode[ii].start_freq);                //15  FLOAT[4]        起振频率                                   float_009
		PODB_SmpBaseFLT(tmpTrigger, 16, tmpVecDevmode[ii].aver_freq);                 //16  FLOAT[4]        平均频率                                   float_010
		PODB_SmpBaseFLT(tmpTrigger, 17, tmpVecDevmode[ii].disp_freq);                 //17  FLOAT[4]        显示频率                                   float_011
		PODB_SmpBaseFLT(tmpTrigger, 18, tmpVecDevmode[ii].max_ampl);                  //18  FLOAT[4]        最大振幅                                   float_012
		PODB_SmpBaseFLT(tmpTrigger, 19, tmpVecDevmode[ii].max_min_dif);               //19  FLOAT[4]        最大摆动                                   float_013
		PODB_SmpBaseINT(tmpTrigger, 20, tmpVecDevmode[ii].stat);                    //20  INT[4]          状态                                       int_006
		PODB_SmpBaseINT(tmpTrigger, 21, tmpVecDevmode[ii].stat_val);                //21  INT[4]          状态值                                     int_007
		PODB_SmpBaseINT(tmpTrigger, 22, tmpVecDevmode[ii].stat2);                   //22  INT[4]          状态2                                      int_008
		PODB_SmpBaseINT(tmpTrigger, 23, tmpVecDevmode[ii].stat_val2);               //23  INT[4]          状态值2                                    int_009
		PODB_SmpBaseINT(tmpTrigger, 24, tmpVecDevmode[ii].if_lfo);                  //24  INT[4]          是否出现振荡                               int_010
		PODB_SmpBaseINT(tmpTrigger, 25, tmpVecDevmode[ii].if_lfo2);                 //25  INT[4]          是否出现振荡2                              int_011
		PODB_SmpBaseINT(tmpTrigger, 26, tmpVecDevmode[ii].is_source);               //26  INT[4]          是否振荡源                                 int_012
		PODB_SmpBaseINT(tmpTrigger, 27, tmpVecDevmode[ii].is_source2);              //27  INT[4]          是否振荡源2                                int_013
		PODB_SmpBaseSTR(tmpTrigger, 28, tmpVecDevmode[ii].yc_name);               //28  STRING[64]      测点名称                                   string_002
		PODB_SmpBaseINT(tmpTrigger, 29, tmpVecDevmode[ii].eve_id);              //29  DATETIME[8]     事件ID                                     int_014
		tmpVecTrigger.push_back(tmpTrigger);
	}
	PODB_TriggerSample(tmpVecTrigger);
#endif
#endif
	return 0;
}


int CLfoAnaBase::ShowProMem(char* keyword)   //查看虚拟内存
{
#ifdef _MEM_
	char buffer[1024] = { 0 };
	char command[1024];
	string strData;
	sprintf(command, "ps -aux|grep  %s", "powoscana");
	FILE* fp = popen(command, "r");
	while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL)
	{
		strData = buffer;
		if (strData.find(g_pAnaObj->m_maindir) != std::string::npos)
		{
			sprintf(m_LogString, "%s-%s", keyword, buffer);
			MessShow(m_LogString, LTINFO);
		}
	}
	pclose(fp);
#endif
	return 0;
}

void CLfoAnaBase::AnaCalcProcess()
{
	//  针对预判后厂站、机组数据，识别模式
	//  针对主要模式，计算模态
	//  计算参与因子
	//  识别振荡中心
	//  整理分析结果
	//////////////////////////////////////////////////////////////////////////

	int  ii, jj, kk;
	float center_volt_ampl = m_LfoPara.center_volt_ampl;
	float sm_md_damp = m_LfoPara.sm_md_damp;
	float para_VoltAmplPer = m_LfoPara.para_VoltAmplPer;
	int para_LfoAnaByPwr = m_LfoPara.para_LfoAnaByPwr;
	int para_nShwMaxFcNum = m_LfoPara.para_nShwMaxFcNum;

	sprintf(m_LogString, "sm_md_damp=[%f] para_VoltAmplPer=[%f] para_LfoAnaByPwr=[%d] para_nShwMaxFcNum=[%d] center_volt_ampl = [%f]", sm_md_damp, para_VoltAmplPer, para_LfoAnaByPwr, para_nShwMaxFcNum, center_volt_ampl);
	MessShow(m_LogString);

	int                  m_nAllMode = 0;                        //  未筛选模式总数
	MODE_CHRCT* m_pAllMode = NULL;                        //  未筛选模式
	int                  m_nSltMode = 0;                        //  系统模式总数
	MODE_CHRCT* m_pSltMode = NULL;                        //  系统模式(分群细化)
	int                  m_nFcProDat = 0;                       //  需处理的厂站
	int                  m_nAllObjMode = 0;                     //  单曲线模式总数(剔除相同模式)
	MODE_PART_OBJ* m_pAllObjMode = NULL;                     //  单曲线模式
	// 	LFO_FC_DAT *         m_pFcProDat;                       //  需处理的厂站数据((预处理后)
	LFO_FC_ANA* m_pFcAna = NULL;                          //  LFO厂站分析信息

	para_nFcMaxMem = MODE_MAXFC;                       //  开内存厂站最大数
	para_nGnMaxMem = MODE_MAXGN;                       //  开内存机组最大数
	para_nLnMaxMem = MODE_MAXLN;                       //  开内存线路最大数
	para_nTrMaxMem = MODE_MAXLN;                       //  开内存变压器最大数
	para_nSysMaxMode = SYS_MODE_MAX;		      	       //  开内sysmode数
	para_nCaseMaxMem = para_nFcMaxMem + para_nGnMaxMem + para_nLnMaxMem + para_nTrMaxMem;
	para_nObjMaxMode = (para_nSysMaxMode * para_nCaseMaxMem);

	m_nAllObjMode = 0;
	m_pAllObjMode = (MODE_PART_OBJ*)calloc(para_nObjMaxMode, sizeof(MODE_PART_OBJ));
	memset((void*)m_pAllObjMode, 0, sizeof(MODE_PART_OBJ) * para_nObjMaxMode);
	ShowProMem("申请m_pAllObjMode内存");
	// 	LfoClctMainMode();


	int nCddtMode = 0;
	MODE_PART_OBJ pCddtMode[MODE_NUM_MAX];
	int nMainMode = 0;
	MODE_PART_OBJ pMainMode[OBJ_MODE_MAX];

	//int dataSize = sizeof(MODE_PART_OBJ);
	int objType = OBJ_TYPE_GN;
	int tmptableno;
	char tmpdevname[64];

	for (ii = 0; ii < m_nCases; ii++)
	{
		memset(tmpdevname, 0, sizeof(tmpdevname));
		// 		sprintf( m_LogString, "mode_num=%d!", m_pCases[ii].m_result.mode_num); 
		// 		MessShow( m_LogString );
		if (m_pCases[ii].m_result.mode_num > 0)
		{

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
				sprintf(pCddtMode[jj].obj_name, "%s", m_pCases[ii].m_name);
				if (m_pCases[ii].m_type == MENU_LFO_DEV_GEN)
				{
					pCddtMode[jj].type = OBJ_TYPE_GN;                    // type
				}
				if (m_pCases[ii].m_type == MENU_LFO_DEV_FAC)
				{
					pCddtMode[jj].type = OBJ_TYPE_FC;                    // type
				}
				pCddtMode[jj].damp = m_pCases[ii].m_result.mode[jj][0]; // 阻尼比
				pCddtMode[jj].freq = m_pCases[ii].m_result.mode[jj][1]; // 频率
				pCddtMode[jj].ampl = m_pCases[ii].m_result.mode[jj][2]; // 振幅
				pCddtMode[jj].phas = m_pCases[ii].m_result.mode[jj][3]; // 初相
				pCddtMode[jj].real = m_pCases[ii].m_result.mode[jj][4]; // 实部(衰减因子)
				pCddtMode[jj].imag = m_pCases[ii].m_result.mode[jj][5]; // 虚部
				pCddtMode[jj].engy = m_pCases[ii].m_result.mode[jj][6]; // engy
			}

			g_theLfoAna.IdtfMainMode(nCddtMode, pCddtMode, nMainMode, pMainMode, m_LfoPara.freq_diff, m_LfoPara.sm_md_damp);

			// 			sprintf( m_LogString, "nMainMode=[%d]", nMainMode ); 
			// 			MessShow( m_LogString );
			if (nMainMode > 0)
			{
				if (nMainMode >= (OBJ_MODE_MAX + OBJ_MODE_MAX))
				{
					sprintf(m_LogString, "计算得到的ObjMode Num[%d] > ObjMode MaxNum[%d],忽略多余Mode!", nMainMode, para_nObjMaxMode);
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
	// 	sprintf( m_LogString, "m_nAllObjMode=[%d]", m_nAllObjMode ); 
	// 	MessShow( m_LogString );

	// 	int                  m_nMonMode;	                    //  mon 模式数
	// 	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon 模式
		//  统计线路信息 2007-12
	sprintf(m_LogString, "添加支路候选模式");
	MessShow(m_LogString);
	for (ii = 0; ii < m_VecDev.size(); ii++)
	{
		if (m_VecDev[ii].dev.dev_type != MENU_LFO_DEV_LINE && m_VecDev[ii].dev.dev_type != MENU_LFO_DEV_TR)
		{
			continue;
		}
		if (m_VecDev[ii].vec_mode.size() > 0)
		{

			// initial pCddtMode and pMainMode
			nCddtMode = 0;
			nMainMode = 0;
			memset((void*)pCddtMode, 0, sizeof(MODE_PART_OBJ) * MODE_NUM_MAX);
			memset((void*)pMainMode, 0, sizeof(MODE_PART_OBJ) * OBJ_MODE_MAX);

			// fill m_pAllObjMode
			nCddtMode = m_VecDev[ii].vec_mode.size(); // .mode_num;
			if (nCddtMode > para_nSysMaxMode) nCddtMode = para_nSysMaxMode;
			for (jj = 0; jj < nCddtMode; jj++)
			{
				pCddtMode[jj].obj_id = m_VecDev[ii].dev.dev_id;    // obj_id
				sprintf(pCddtMode[jj].obj_name, "%s", m_VecDev[ii].dev.dev_name);
				if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
				{
					pCddtMode[jj].type = OBJ_TYPE_LN;                    // type
				}
				if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
				{
					pCddtMode[jj].type = OBJ_TYPE_TR;                    // type
				}
				pCddtMode[jj].damp = m_VecDev[ii].vec_mode[jj].damp; // 阻尼比
				pCddtMode[jj].freq = m_VecDev[ii].vec_mode[jj].freq; // 频率
				pCddtMode[jj].ampl = m_VecDev[ii].vec_mode[jj].ampl; // 振幅
				pCddtMode[jj].phas = m_VecDev[ii].vec_mode[jj].phas; // 初相
				pCddtMode[jj].real = m_VecDev[ii].vec_mode[jj].real; // 实部(衰减因子)
				pCddtMode[jj].imag = m_VecDev[ii].vec_mode[jj].imag; // 虚部
				pCddtMode[jj].engy = m_VecDev[ii].vec_mode[jj].engy; // engy
			}

			g_theLfoAna.IdtfMainMode(nCddtMode, pCddtMode, nMainMode, pMainMode, m_LfoPara.freq_diff, m_LfoPara.sm_md_damp);

			// 			sprintf( m_LogString, "计算得到的ObjMode Num[%d]", nMainMode ); 
			// 			MessShow( m_LogString );
			if (nMainMode > 0)
			{
				if (nMainMode >= (OBJ_MODE_MAX + OBJ_MODE_MAX))
				{
					sprintf(m_LogString, "计算得到的ObjMode Num[%d] > ObjMode MaxNum[%d],忽略多余Mode!", nMainMode, para_nObjMaxMode);
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

	ShowProMem("LfoClctMainMode");
	m_nAllMode = 0;
	m_pAllMode = (MODE_CHRCT*)calloc(para_nSysMaxMode, sizeof(MODE_CHRCT));	// filter obj mode :  case -> m_pAllObjMode m_nAllObjMode
	memset((void*)m_pAllMode, 0, para_nSysMaxMode * sizeof(MODE_CHRCT));
	ShowProMem("申请m_pAllMode内存");

	//WriteAllObjModeToFile( m_nAllObjMode, m_pAllObjMode );

	// identify Gn and Ln of the mode :  m_pAllObjMode -> m_pAllMode m_nAllMode
// 		for (ii = 0; ii < m_nAllObjMode;ii++)
// 		{
// 			sprintf( m_LogString, "候选模式[%d]:id=%ld,name=%s,type=%d,damp=%f,freq=%f,ampl=%f,phas=%f,mgnt=%f,real=%f,imag=%f,fact=%f,engy=%f",
// 				ii+1,
// 				m_pAllObjMode[ii].obj_id,
// 				m_pAllObjMode[ii].obj_name,
// 				m_pAllObjMode[ii].type,
// 				m_pAllObjMode[ii].damp,
// 				m_pAllObjMode[ii].freq,
// 				m_pAllObjMode[ii].ampl,
// 				m_pAllObjMode[ii].phas,
// 				m_pAllObjMode[ii].mgnt,
// 				m_pAllObjMode[ii].real,
// 				m_pAllObjMode[ii].imag,
// 				m_pAllObjMode[ii].fact,
// 				m_pAllObjMode[ii].engy); 
// 			MessShow( m_LogString );
// 		}
	g_theLfoAna.IdtfPartGnPartLn(m_nAllObjMode, m_pAllObjMode, m_nAllMode, m_pAllMode, m_LfoPara.freq_diff, sm_md_damp);

	if (m_pAllObjMode != NULL) { free(m_pAllObjMode);	m_pAllObjMode = NULL; }
	ShowProMem("IdtfPartGnPartLn");
	sprintf(m_LogString, "m_nAllMode=%d", m_nAllMode);
	MessShow(m_LogString);


	g_theLfoAna.CalcModeGnLnEngy(m_nAllMode, m_pAllMode);
	for (ii = 0; ii < m_nAllMode; ii++)
	{
		sprintf(m_LogString, "筛选后模式[%d]:type=%d,damp=%f,freq=%f,engyLN=%f,engyGN=%f",
			ii + 1,
			m_pAllMode[ii].type,
			m_pAllMode[ii].damp,
			m_pAllMode[ii].freq,
			m_pAllMode[ii].engyLn,
			m_pAllMode[ii].engyGn);
		MessShow(m_LogString);
	}

	ShowProMem("CalcModeGnLnEngy");
	sprintf(m_LogString, "Analysis Case：%d; obj mode：%d; sys mode：%d", m_nCases, m_nAllObjMode, m_nAllMode);
	MessShow(m_LogString);
	//WriteModeShapeToFile( m_nAllMode, m_pAllMode );

	// Calclation modeshape ： CalcPartFactor( int iObjNum, MODE_PART_OBJ *ModeShape );
	if (m_nAllMode > 0)
	{
		sprintf(m_LogString, "计算模态分布!");
		MessShow(m_LogString);
		for (ii = 0; ii < m_nAllMode; ii++)
		{
			//                 m_pAllMode[ii].time_bgn = m_CalcStartTime; // 2008-06-04 by zhw at nj
			//                 m_pAllMode[ii].time_end = m_CurGpsTime;

			if (m_pAllMode[ii].nPartGn > 0)
			{
				if (para_LfoAnaByPwr == 0)
				{
					// 增加对Tj的处理 异常情况出口
					LfoAnaPartGenPreHandle(m_nGnNum, m_pGnInf, m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn);
					if (g_theLfoAna.CalcPartFactor(m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn, para_LfoAnaByPwr) == false)
						g_theLfoAna.LfoAnaSetPartFact0(m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn);
				}
				else
					g_theLfoAna.CalcPartFactor(m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn);

				// set sign
				//g_theLfoAna.JudgPartFactorSign( m_pAllMode[ii].nPartGn, m_pAllMode[ii].PartGn );
			}
			// 				// 可观线路 //2008-09-24 封
			// 				if ( m_pAllMode[ii].nPartLn > 0 )
			// 				{
			// 					g_theLfoAna.CalcPartFactor( m_pAllMode[ii].nPartLn, m_pAllMode[ii].PartLn );
			// 					// set sign
			// 					//g_theLfoAna.JudgPartFactorSign( m_pAllMode[ii].nPartLn, m_pAllMode[ii].PartLn );
			// 				}
							// 厂站分布
			if (m_pAllMode[ii].nPartFc > 0)
			{
				g_theLfoAna.CalcPartFactor(m_pAllMode[ii].nPartFc, m_pAllMode[ii].PartFc);
				// set sign
				//g_theLfoAna.JudgPartFactorSign( m_pAllMode[ii].nPartFc, m_pAllMode[ii].PartFc );
			}
		}
	}
	ShowProMem("计算模态分布");
	m_nSltMode = 0;
	m_pSltMode = (MODE_CHRCT*)calloc(para_nSysMaxMode, sizeof(MODE_CHRCT));
	memset((void*)m_pSltMode, 0, sizeof(MODE_CHRCT) * para_nSysMaxMode);
	ShowProMem("申请m_pSltMode内存");

	//WriteModeShapeToFile( m_nAllMode, m_pAllMode , 1);
	//WriteModeShapeToFile( m_nAllMode, m_pAllMode );  // 2008-06-04 by zhw at nj

	// 模态筛选：
	if (m_nAllMode > 0)
	{
		sprintf(m_LogString, "筛选模态分布,m_nAllMode=%d,m_LfoPara.cohcy_angl=%f", m_nAllMode, m_LfoPara.cohcy_angl);
		MessShow(m_LogString);
		if (m_nAllMode < para_nSysMaxMode)
			m_nSltMode = m_nAllMode;
		else
			m_nSltMode = para_nSysMaxMode;

		memcpy((void*)m_pSltMode, (void*)m_pAllMode, sizeof(MODE_CHRCT) * m_nSltMode);
		for (ii = 0; ii < m_nSltMode; ii++)
		{
			ShowProMem("开始循环");
			// 基准选取（gn）
			g_theLfoAna.IdtfCausLfoGn(m_pSltMode[ii].nPartGn, m_pSltMode[ii].PartGn, m_LfoPara.cohcy_angl, 0);
			// 厂站需考虑与发电机一致
			if (m_pSltMode[ii].nPartGn > 0)
			{
				sprintf(m_LogString, "nPartGn>0,基准设备[%s]基准角=[%f],所属群[%d]", m_pSltMode[ii].PartGn[0].obj_name, m_pSltMode[ii].PartGn[0].phas, m_pSltMode[ii].PartGn[0].clus);
				MessShow(m_LogString);
				// 2008-09-24 g_theLfoAna.IdtfLfoGnCohcyFc( m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc, m_pSltMode[ii].PartGn[0].phas, (float)(m_pSltMode[ii].PartGn[0].phas+PRONY_PI+0.03), m_LfoPara.cohcy_angl );
				g_theLfoAna.IdtfLfoGnCohcyFc(m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc, (MODE_PART_OBJ*)&m_pSltMode[ii].PartGn[0].obj_id, m_LfoPara.cohcy_angl, 0);
			}
			else
			{
				sprintf(m_LogString, "nPartGn=0,m_pSltMode[ii].PartFc[0].phas=%f", m_pSltMode[ii].PartFc[0].phas);
				MessShow(m_LogString);
				g_theLfoAna.IdtfCausLfoGn(m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc, m_LfoPara.cohcy_angl, 0);
			}
			ShowProMem("基准选取");
			// 2008-09-24 add 通过Fc划分Ln/Tr所属群
			//////////////////////////////////////////////////////////////////////////
			sprintf(m_LogString, "该模式频率[%f],参与厂站数nPartFc=%d", m_pSltMode[ii].freq, m_pSltMode[ii].nPartFc);
			MessShow(m_LogString);
			if (m_pSltMode[ii].nPartFc > 0)
			{
				// 					for (jj = 0;jj < m_pSltMode[ii].nPartFc; jj++)
				// 					{
				// 						sprintf( m_LogString, "设备[%s]所属群[%d],初相角[%f]",m_pSltMode[ii].PartFc[jj].obj_name,m_pSltMode[ii].PartFc[jj].clus,m_pSltMode[ii].PartFc[jj].phas);
				// 						MessShow( m_LogString );
				// 					}
				LfoAnaSetLnClustbyFc(m_pSltMode[ii].nPartLn, m_pSltMode[ii].PartLn, m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc);
				LfoAnaSetTrClustbyFc(m_pSltMode[ii].nPartTr, m_pSltMode[ii].PartTr, m_pSltMode[ii].nPartFc, m_pSltMode[ii].PartFc);
			}
			ShowProMem("分群");
			//////////////////////////////////////////////////////////////////////////
			// 统一参与因子标准
			//////////////////////////////////////////////////////////////////////////
			LfoAnaSetGnAmpl(m_pSltMode[ii].nPartGn, m_pSltMode[ii].PartGn);
			LfoAnaResetFact((MODE_CHRCT*)&m_pSltMode[ii].mode_id);
			ShowProMem("统一参与因子标准");
			//////////////////////////////////////////////////////////////////////////
			// 中心识别
			//////////////////////////////////////////////////////////////////////////
			g_theLfoAna.IdtfLfoCenter((MODE_CHRCT*)&m_pSltMode[ii].mode_id);
			ShowProMem("中心识别");
			//////////////////////////////////////////////////////////////////////////
		}
	}
	if (m_pAllMode != NULL) { free(m_pAllMode);	m_pAllMode = NULL; }
	ShowProMem("筛选模态分布");

	//WriteModeShapeToFile( m_nSltMode, m_pSltMode);  // 2008-06-04 by zhw at nj

	//振荡中心判断 ->中心识别->电压过滤

	m_pFcAna = (LFO_FC_ANA*)calloc(para_nFcMaxMem, sizeof(LFO_FC_ANA));
	memset((void*)m_pFcAna, 0, para_nFcMaxMem * sizeof(LFO_FC_ANA));
	ShowProMem("申请m_pFcAna内存");
	float MaxVoltAmpl = 0.0;
	m_nFcProDat = 0;
	for (ii = 0; ii < m_VecDev.size(); ii++)
	{
		if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_FAC)
		{
			m_pFcAna[m_nFcProDat].fac_id = m_VecDev[ii].dev.dev_id;
			strcpy(m_pFcAna[m_nFcProDat].fac_name, m_VecDev[ii].dev.dev_name);
			m_pFcAna[m_nFcProDat].lfo_v_aver = m_VecDev[ii].curve.aver_val;
			m_pFcAna[m_nFcProDat].lfo_v_max = m_VecDev[ii].curve.max_val;
			m_pFcAna[m_nFcProDat].lfo_v_min = m_VecDev[ii].curve.min_val;
			m_pFcAna[m_nFcProDat].lfo_v_ampl = m_VecDev[ii].curve.max_ampl;

			m_pFcAna[m_nFcProDat].lfo_is_cntr = MENU_LFO_MON_NO;
			m_pFcAna[m_nFcProDat].lfo_is_disp = MENU_LFO_MON_NO;
			m_pFcAna[m_nFcProDat].lfo_clus = MENU_LFO_CLUST_RMAIN;
			// 获取电压波动最大值
			if (m_pFcAna[m_nFcProDat].lfo_v_ampl > MaxVoltAmpl)
				MaxVoltAmpl = m_pFcAna[m_nFcProDat].lfo_v_ampl;

			// 填充分析结果
			if (m_nSltMode > 0 && m_pSltMode[0].nPartFc > 0)
			{
				for (kk = 0; kk < m_pSltMode[0].nPartFc; kk++)
				{
					if (m_pFcAna[m_nFcProDat].fac_id == m_pSltMode[0].PartFc[kk].obj_id)
					{
						m_pFcAna[m_nFcProDat].ampl = m_pSltMode[0].PartFc[kk].ampl;
						m_pFcAna[m_nFcProDat].freq = m_pSltMode[0].PartFc[kk].freq;
						m_pFcAna[m_nFcProDat].phas = m_pSltMode[0].PartFc[kk].phas;
						m_pFcAna[m_nFcProDat].damp = m_pSltMode[0].PartFc[kk].damp;

						m_pFcAna[m_nFcProDat].lfo_clus = m_pSltMode[0].PartFc[kk].clus;

						sprintf(m_LogString, "[%s]["INT_REC_FMT"]电压最大摆动[%f],频率[%f],初相角[%f],所属群[%d]", m_VecDev[ii].dev.dev_name, INT_REC_VAL(m_pFcAna[m_nFcProDat].fac_id), m_pFcAna[m_nFcProDat].lfo_v_ampl, m_pFcAna[m_nFcProDat].freq, m_pFcAna[m_nFcProDat].phas, m_pFcAna[m_nFcProDat].lfo_clus);
						MessShow(m_LogString);
						break;
					}
				}
			}
			m_nFcProDat++;
		}
		if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_GEN)
		{
			for (kk = 0; kk < m_pSltMode[0].nPartGn; kk++)
			{
				if (m_VecDev[ii].dev.dev_id == m_pSltMode[0].PartGn[kk].obj_id)
				{
					m_VecDev[ii].mode.fact = m_pSltMode[0].PartGn[kk].fact;
					m_VecDev[ii].mode.group = m_pSltMode[0].PartGn[kk].clus;
					// 						m_VecDev[ii].mode.ampl = m_pSltMode[0].PartGn[kk].ampl;
					m_VecDev[ii].mode.freq = m_pSltMode[0].PartGn[kk].freq;
					sprintf(m_LogString, "[%s]参与因子[%f],所属群[%d],初相角[%f],振幅[%f]", m_VecDev[ii].dev.dev_name, m_VecDev[ii].mode.fact, m_pSltMode[0].PartGn[kk].clus, m_pSltMode[0].PartGn[kk].phas, m_VecDev[ii].mode.ampl);
					MessShow(m_LogString);
					break;
				}
			}
			for (kk = 0; kk < m_MonSys.vec_dev.size(); kk++)
			{
				if (m_MonSys.vec_dev[kk].dev.dev_id == m_VecDev[ii].dev.dev_id)
				{
					m_MonSys.vec_dev[kk].mode.fact = m_VecDev[ii].mode.fact;
					m_MonSys.vec_dev[kk].mode.group = m_VecDev[ii].mode.group;
					// 						m_MonSys.vec_dev[kk].mode.ampl = m_VecDev[ii].mode.ampl;
					m_MonSys.vec_dev[kk].mode.freq = m_VecDev[ii].mode.freq;
					break;
				}
			}
		}
	}

	//排序
	qsort((void*)m_pFcAna, m_nFcProDat, sizeof(LFO_FC_ANA), FcAnaResCompareByAmpl);

	sprintf(m_LogString, "AnaCalcProcess():电压最大摆动[%f]，限值[%f]，中心厂数限值[%d],m_nSltMode=%d!", MaxVoltAmpl, MaxVoltAmpl * para_VoltAmplPer, m_LfoPara.para_nShwMaxFcNum, m_nSltMode);
	MessShow(m_LogString);
	ShowProMem("电压最大摆动");
	// 振荡中心判断
	MaxVoltAmpl = MaxVoltAmpl * para_VoltAmplPer;
	if (MaxVoltAmpl < center_volt_ampl)
	{
		MaxVoltAmpl = center_volt_ampl;
	}
	int  tempCntrFcNum = 0;
	if (m_nSltMode > 0)
	{
		sprintf(m_LogString, "m_pSltMode[0].nPartFc=[%d]!", m_pSltMode[0].nPartFc);
		MessShow(m_LogString);
		if (m_pSltMode[0].nPartFc > 0)
		{
			for (ii = 0; ii < m_pSltMode[0].nPartFc; ii++)
			{
				// 					sprintf( m_LogString, "厂站[%ld][%s]判断中心",m_pSltMode[0].PartFc[ii].obj_id,m_pSltMode[0].PartFc[ii].obj_name);
				// 					MessShow( m_LogString );
				for (jj = 0; jj < m_nFcProDat; jj++)
				{
					if (m_pSltMode[0].PartFc[ii].obj_id == m_pFcAna[jj].fac_id)
					{
						sprintf(m_LogString, "厂站["INT_REC_FMT"]lfo_v_ampl=%f,MaxVoltAmpl=%f!", INT_REC_VAL(m_pFcAna[jj].fac_id), m_pFcAna[jj].lfo_v_ampl, MaxVoltAmpl);
						MessShow(m_LogString);
						if (m_pFcAna[jj].lfo_v_ampl > MaxVoltAmpl)
						{
							m_pFcAna[jj].lfo_is_cntr = MENU_LFO_MON_YES;
							m_pFcAna[jj].lfo_is_disp = MENU_LFO_MON_YES;
							tempCntrFcNum += 1;
							break;
						}
					}
				}

				if (tempCntrFcNum >= para_nShwMaxFcNum)
				{
					break; // 达到限值，跳出
				}
			}
		}
	}
	ShowProMem("振荡中心判断");

	//
	if (tempCntrFcNum > 0)
	{
		LFO_DEV_MODE tmpDevMode;
		// 			strcpy( m_MonSys.center_desc, "振荡中心位于以下厂站附近：");
		sprintf(m_MonSys.center_desc, "");
		for (ii = 0; ii < m_nFcProDat; ii++)
		{
			if (m_pFcAna[ii].lfo_is_cntr == MENU_LFO_MON_YES)
			{
				m_MonSys.center_id = m_pFcAna[ii].fac_id;
				sprintf(m_LogString, "%s ", m_pFcAna[ii].fac_name);
				strcat(m_MonSys.center_desc, m_LogString);
				for (jj = 0; jj < m_VecDev.size(); jj++)
				{
					if (m_pFcAna[ii].fac_id == m_VecDev[jj].dev.dev_id)
					{
						m_VecDev[jj].lfo_is_cntr = MENU_LFO_MON_YES;
						m_MonSys.cen_dev.push_back(m_VecDev[jj]);
						break;
					}
				}
			}
		}
		if (!m_MonSys.vec_modeshape.empty())
		{
			m_MonSys.vec_modeshape[0].is_center = 1;
			sprintf(m_MonSys.vec_modeshape[0].lfo_ana_des, "%s", m_MonSys.center_desc);
			sprintf(m_LogString, "[%fHz]振荡模式是否振荡中心标志[%d],描述[%s]", m_MonSys.vec_modeshape[0].mode.freq, m_MonSys.vec_modeshape[0].is_center, m_MonSys.vec_modeshape[0].lfo_ana_des);
			MessShow(m_LogString);
			//将振荡中心厂站加入该模式的可观设备中
			for (ii = 0; ii < m_MonSys.cen_dev.size(); ii++)
			{
				ClearLfoDevMode(tmpDevMode);
				tmpDevMode.dev.dev_id = m_MonSys.cen_dev[ii].dev.dev_id;
				tmpDevMode.dev.dev_type = MENU_LFO_DEV_FAC;
				sprintf(tmpDevMode.dev.dev_name, "%s", m_MonSys.cen_dev[ii].dev.dev_name);
				tmpDevMode.mode.ampl = m_MonSys.cen_dev[ii].curve.max_ampl;//用该模式的频率 表示该厂站的频率
				tmpDevMode.mode.freq = m_MonSys.vec_modeshape[0].mode.freq;
				tmpDevMode.mode.is_source = MENU_LFO_MENU_YES;//表示是否振荡中心
				m_MonSys.vec_modeshape[0].vec_dev.push_back(tmpDevMode);
			}
		}
	}
	else
	{
		ZERO_INT_REC(m_MonSys.center_id);
		strcpy(m_MonSys.center_desc, "");
		if (!m_MonSys.vec_modeshape.empty())
		{
			m_MonSys.vec_modeshape[0].is_center = 0;
		}

	}
	MessShow(m_MonSys.center_desc, LTINFO);

	if (m_pFcAna != NULL) { free(m_pFcAna);   m_pFcAna = NULL; }
	if (m_pSltMode != NULL) { free(m_pSltMode);	m_pSltMode = NULL; }
	if (m_pAllMode != NULL) { free(m_pAllMode);	m_pAllMode = NULL; }
	if (m_pAllObjMode != NULL) { free(m_pAllObjMode);	m_pAllObjMode = NULL; }


	// 		WriteLfoCenterToFile( m_SysAna, m_nFcProDat, m_pFcAna );
	// 
	// 		WriteModeShapeToFile( m_nSltMode, m_pSltMode);
	//*/
	return;
}

void CLfoAnaBase::LfoClctMainMode()
{/*
	int ii, jj, kk ;
	float sm_md_damp = 100.0;

	int nCddtMode = 0;
	MODE_PART_OBJ pCddtMode[MODE_NUM_MAX];
	int nMainMode = 0;
	MODE_PART_OBJ pMainMode[OBJ_MODE_MAX];

	//int dataSize = sizeof(MODE_PART_OBJ);
	int objType  = OBJ_TYPE_GN;
	m_nAllObjMode = 0 ;
	m_pAllObjMode = (MODE_PART_OBJ *)calloc(para_nObjMaxMode, sizeof(MODE_PART_OBJ));
	memset((void *)m_pAllObjMode, 0, sizeof(MODE_PART_OBJ) * para_nObjMaxMode );
	int tmptableno;
	char tmpdevname[64];
	sprintf( m_LogString, "m_pAllObjMode-size=%d!", para_nObjMaxMode*sizeof(MODE_PART_OBJ));
	MessShow( m_LogString );

	for( ii=0;ii<m_nCases;ii++)
	{
		memset(tmpdevname,0,sizeof(tmpdevname));
		sprintf( m_LogString, "mode_num=%d!", m_pCases[ii].m_result.mode_num);
		MessShow( m_LogString );
		if (m_pCases[ii].m_result.mode_num > 0)
		{
// 			tmptableno = PODB_GetTabelNoByDevId(m_pCases[ii].m_nID);
// 			if( tmptableno == ACLN_DOT_NO_TAB || tmptableno == ACLN_DEVICE_NO_TAB )
// 			{
// 				objType = OBJ_TYPE_LN ;                      // type
// 				for (jj =0;jj<m_nLnNum; jj++)
// 				{
// 					if (m_pCases[ii].m_nID == m_pLnInf[jj].acln_id)
// 					{
// 						sprintf(tmpdevname,"%s",m_pLnInf[jj].acln_name);
// 						break;
// 					}
// 				}
// 			}
// 			else if(tmptableno == GN_DEVICE_NO_TAB)
// 			{
// 				objType = OBJ_TYPE_GN ;                      // type
//
// 				for (jj =0;jj<m_nGnNum; jj++)
// 				{
// 					if (m_pCases[ii].m_nID == m_pGnInf[jj].gn_id)
// 					{
// 						sprintf(tmpdevname,"%s",m_pGnInf[jj].gn_name);
// 						break;
// 					}
// 				}
// 			}
// 			else if(tmptableno == TR_DEVICE_NO_TAB || tmptableno == TRWD_DEVICE_NO_TAB)
// 			{
// 				objType = OBJ_TYPE_TR ;                      // type
// 				for (jj =0;jj<m_nTrNum; jj++)
// 				{
// 					if (m_pCases[ii].m_nID == m_pTrInf[jj].tr_id)
// 					{
// 						sprintf(tmpdevname,"%s",m_pTrInf[jj].tr_name);
// 						break;
// 					}
// 				}
// 			}
// 			else if(tmptableno == FAC_INFO_NO_TAB)
// 			{
// 				objType = OBJ_TYPE_FC ;                      // type
// 				for (jj =0;jj<m_nFcNum; jj++)
// 				{
// 					if (m_pCases[ii].m_nID == m_pFcInf[jj].fac_id)
// 					{
// 						sprintf(tmpdevname,"%s",m_pFcInf[jj].fac_name);
// 						break;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				sprintf( m_LogString, "分析的第%d个Case类型号["INT_REC_FMT"]错误!", ii, m_pCases[ii].m_nID );
// 				MessShow( m_LogString );
// 				continue;
// 			}

			// initial pCddtMode and pMainMode
			nCddtMode = 0;
			nMainMode = 0;
			memset( (void*)pCddtMode, 0, sizeof(MODE_PART_OBJ) * MODE_NUM_MAX );
			memset( (void*)pMainMode, 0, sizeof(MODE_PART_OBJ) * OBJ_MODE_MAX );

			// fill m_pAllObjMode
			nCddtMode = m_pCases[ii].m_result.mode_num;
			//if (nCddtMode > MODE_NUM_MAX ) nCddtMode = MODE_NUM_MAX;
			if (nCddtMode > para_nSysMaxMode ) nCddtMode = para_nSysMaxMode;
			for (jj=0; jj<nCddtMode; jj++)
			{
				pCddtMode[jj].obj_id = m_pCases[ii].m_nID ;              // obj_id
				sprintf(pCddtMode[jj].obj_name,"%s",m_pCases[ii].m_name);
				if (m_pCases[ii].m_type == MENU_LFO_DEV_GEN)
				{
					pCddtMode[jj].type = OBJ_TYPE_GN ;                    // type
				}
				if (m_pCases[ii].m_type == MENU_LFO_DEV_FAC)
				{
					pCddtMode[jj].type = OBJ_TYPE_FC ;                    // type
				}
				pCddtMode[jj].damp = m_pCases[ii].m_result.mode[jj][0] ; // 阻尼比
				pCddtMode[jj].freq = m_pCases[ii].m_result.mode[jj][1] ; // 频率
				pCddtMode[jj].ampl = m_pCases[ii].m_result.mode[jj][2] ; // 振幅
				pCddtMode[jj].phas = m_pCases[ii].m_result.mode[jj][3] ; // 初相
				pCddtMode[jj].real = m_pCases[ii].m_result.mode[jj][4] ; // 实部(衰减因子)
				pCddtMode[jj].imag = m_pCases[ii].m_result.mode[jj][5] ; // 虚部
				pCddtMode[jj].engy = m_pCases[ii].m_result.mode[jj][6] ; // engy
			}

			g_theLfoAna.IdtfMainMode( nCddtMode, pCddtMode, nMainMode, pMainMode, m_LfoPara.freq_diff, m_LfoPara.sm_md_damp );

// 			sprintf( m_LogString, "nMainMode=[%d]", nMainMode );
// 			MessShow( m_LogString );
			if ( nMainMode > 0 )
			{
				if ( nMainMode >= (OBJ_MODE_MAX+OBJ_MODE_MAX) )
				{
					sprintf( m_LogString, "计算得到的ObjMode Num[%d] > ObjMode MaxNum[%d],忽略多余Mode!", nMainMode, para_nObjMaxMode );
					MessShow( m_LogString );
					nMainMode = para_nObjMaxMode;
				}
				for ( kk=0; kk<nMainMode; kk++)
				{
					g_theLfoAna.MemCpyModePartObjStru( pMainMode[kk], m_pAllObjMode[m_nAllObjMode] );
					m_nAllObjMode +=1;
				}
			}
		}
	}
// 	sprintf( m_LogString, "m_nAllObjMode=[%d]", m_nAllObjMode );
// 	MessShow( m_LogString );

// 	int                  m_nMonMode;	                    //  mon 模式数
// 	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon 模式
	//  统计线路信息 2007-12
	sprintf( m_LogString, "添加支路候选模式" );
	MessShow( m_LogString );
	for( ii=0;ii<m_VecDev.size();ii++)
	{
		if (m_VecDev[ii].dev.dev_type != MENU_LFO_DEV_LINE && m_VecDev[ii].dev.dev_type != MENU_LFO_DEV_TR)
		{
			continue;
		}
		if (m_VecDev[ii].vec_mode.size() > 0)
		{

			// initial pCddtMode and pMainMode
			nCddtMode = 0;
			nMainMode = 0;
			memset( (void*)pCddtMode, 0, sizeof(MODE_PART_OBJ) * MODE_NUM_MAX );
			memset( (void*)pMainMode, 0, sizeof(MODE_PART_OBJ) * OBJ_MODE_MAX );

			// fill m_pAllObjMode
			nCddtMode = m_VecDev[ii].vec_mode.size(); // .mode_num;
			if (nCddtMode > para_nSysMaxMode ) nCddtMode = para_nSysMaxMode;
			for (jj=0; jj<nCddtMode; jj++)
			{
				pCddtMode[jj].obj_id = m_VecDev[ii].dev.dev_id ;    // obj_id
				sprintf(pCddtMode[jj].obj_name,"%s",m_VecDev[ii].dev.dev_name);
				if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_LINE)
				{
					pCddtMode[jj].type = OBJ_TYPE_LN ;                    // type
				}
				if (m_VecDev[ii].dev.dev_type == MENU_LFO_DEV_TR)
				{
					pCddtMode[jj].type = OBJ_TYPE_TR ;                    // type
				}
				pCddtMode[jj].damp = m_VecDev[ii].vec_mode[jj].damp ; // 阻尼比
				pCddtMode[jj].freq = m_VecDev[ii].vec_mode[jj].freq ; // 频率
				pCddtMode[jj].ampl = m_VecDev[ii].vec_mode[jj].ampl ; // 振幅
				pCddtMode[jj].phas = m_VecDev[ii].vec_mode[jj].phas ; // 初相
				pCddtMode[jj].real = m_VecDev[ii].vec_mode[jj].real ; // 实部(衰减因子)
				pCddtMode[jj].imag = m_VecDev[ii].vec_mode[jj].imag ; // 虚部
				pCddtMode[jj].engy = m_VecDev[ii].vec_mode[jj].engy ; // engy
			}

			g_theLfoAna.IdtfMainMode( nCddtMode, pCddtMode, nMainMode, pMainMode, m_LfoPara.freq_diff, m_LfoPara.sm_md_damp );

// 			sprintf( m_LogString, "计算得到的ObjMode Num[%d]", nMainMode );
// 			MessShow( m_LogString );
			if ( nMainMode > 0 )
			{
				if ( nMainMode >= (OBJ_MODE_MAX+OBJ_MODE_MAX) )
				{
					sprintf( m_LogString, "计算得到的ObjMode Num[%d] > ObjMode MaxNum[%d],忽略多余Mode!", nMainMode, para_nObjMaxMode );
					MessShow( m_LogString );
					nMainMode = para_nObjMaxMode;
				}
				for ( kk=0; kk<nMainMode; kk++)
				{
					g_theLfoAna.MemCpyModePartObjStru( pMainMode[kk], m_pAllObjMode[m_nAllObjMode] );
					m_nAllObjMode +=1;
				}
			}
		}
	}*/
	return;
}

void CLfoAnaBase::LfoAnaPartGenPreHandle(int nGnNum, LFO_GN_INF* pGnInf, int nPartGn, MODE_PART_OBJ* pPartGn)
{
	if (nPartGn < 0 || !pPartGn || nGnNum <= 0 || !pGnInf) return;

	int  ii, jj;
	for (ii = 0; ii < nPartGn; ii++)
	{
		for (jj = 0; jj < nGnNum; jj++)
		{
			if (pPartGn[ii].obj_id == pGnInf[jj].gn_id)
			{
				pPartGn[ii].fact = pGnInf[jj].emws;

				break;
			}
		}
	}

	return;

}

////////////////////////////////////////////////////////////////////////////////
//  函数功能 : 根据监视信息重新设置发电机对应模式的幅值
//  处理过程 : 
//  备    注 :
//  返 回 值 :
//  参数说明 : nPartGn                  发电机数
//             pPartGn[]                发电机信息
////////////////////////////////////////////////////////////////////////////////
// 	int                  m_nMonMode;	                    //  mon 模式数
// 	PRONY_ANAL_RSLT *    m_pMonMode;	                    //  mon 模式
void CLfoAnaBase::LfoAnaSetGnAmpl(int nPartGn, MODE_PART_OBJ* pPartGn)
{
	if (nPartGn < 0 || !pPartGn)    return;

	int  ii, jj, kk;
	int  tempFindFlag = 0;
	int  tempSubIndex = -1;
	float sm_md_damp = 100.0;
	// 	sprintf(m_LogString,"发电机设置对应模式振幅,该模式发电机数%d",nPartGn);
	// 	MessShow(m_LogString,LTERROR);
	for (ii = 0; ii < nPartGn; ii++)
	{
		// 		sprintf(m_LogString,"发电机[%s]开始设置对应模式振幅",pPartGn[ii].obj_name);
		// 		MessShow(m_LogString,LTERROR);
		tempFindFlag = 0;
		tempSubIndex = -1;
		for (jj = 0; jj < m_VecDev.size(); jj++)
		{
			if (pPartGn[ii].obj_id == m_VecDev[jj].dev.dev_id)
			{
				// 				sprintf(m_LogString,"发电机[%s]找到监视结果",pPartGn[ii].obj_name);
				// 				MessShow(m_LogString,LTERROR);
				for (kk = 0; kk < m_VecDev[jj].vec_mode.size(); kk++)
				{
					if (g_theLfoAna.IdtfSameMode(pPartGn[ii].freq, pPartGn[ii].damp, m_VecDev[jj].vec_mode[kk].freq, m_VecDev[jj].vec_mode[kk].damp, m_LfoPara.freq_diff, m_LfoPara.sm_md_damp) == true)
					{
						tempFindFlag = 1;
						tempSubIndex = kk;
						break;
					}
				}
				if (tempFindFlag == 1)
				{
					pPartGn[ii].ampl = m_VecDev[jj].vec_mode[tempSubIndex].ampl;
					// 					sprintf(m_LogString,"发电机[%s]找到对应模式,重新设置对应模式振幅[%f]",pPartGn[ii].obj_name,pPartGn[ii].ampl);
					// 					MessShow(m_LogString,LTERROR);
				}
				break;
			}
		}
	}

	return;
}

void CLfoAnaBase::LfoAnaResetFact(MODE_CHRCT* pSltMode)
{
	if (!pSltMode)   return;

	int   ii;
	int   tempMaxType = -1;
	INT_REC   tempMaxObjid;	ZERO_INT_REC(tempMaxObjid);
	float tempMaxAmpl_gen = 1.0f;
	float tempMaxAmpl_ln_tr = 1.0f;
	para_IsFactByClust = MENU_LFO_MENU_YES;
	for (ii = 0; ii < pSltMode->nPartGn; ii++)
	{
		// 2008-12-11 by zhw 在分群结果(临界群和相对群,不包括余下群)中选择幅值最大的设备,做为参与因子基准,
		// 若控制开关为否,在所有设备中选择幅值最大的设备
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartGn[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		if (tempMaxAmpl_gen < pSltMode->PartGn[ii].ampl)
		{
			tempMaxAmpl_gen = pSltMode->PartGn[ii].ampl;
			tempMaxType = OBJ_TYPE_GN;
			tempMaxObjid = pSltMode->PartGn[ii].obj_id;
		}
	}
	for (ii = 0; ii < pSltMode->nPartLn; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartLn[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		if (tempMaxAmpl_ln_tr < pSltMode->PartLn[ii].ampl)
		{
			tempMaxAmpl_ln_tr = pSltMode->PartLn[ii].ampl;
			tempMaxType = OBJ_TYPE_LN;
			tempMaxObjid = pSltMode->PartLn[ii].obj_id;
		}
	}
	for (ii = 0; ii < pSltMode->nPartTr; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartTr[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		if (tempMaxAmpl_ln_tr < pSltMode->PartTr[ii].ampl)
		{
			tempMaxAmpl_ln_tr = pSltMode->PartTr[ii].ampl;
			tempMaxType = OBJ_TYPE_TR;
			tempMaxObjid = pSltMode->PartTr[ii].obj_id;
		}
	}
	// reset fact
	for (ii = 0; ii < pSltMode->nPartGn; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartGn[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		pSltMode->PartGn[ii].fact = pSltMode->PartGn[ii].ampl / tempMaxAmpl_gen;
		// 		if (pSltMode->PartGn[ii].clus == MENU_LFO_CLUST_OPOST)//相对群参与因子置为负数
		// 		{
		// 			pSltMode->PartGn[ii].fact = pSltMode->PartGn[ii].fact * -1;
		// 		}
	}
	for (ii = 0; ii < pSltMode->nPartLn; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartLn[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		pSltMode->PartLn[ii].fact = pSltMode->PartLn[ii].ampl / tempMaxAmpl_ln_tr;
	}
	for (ii = 0; ii < pSltMode->nPartTr; ii++)
	{
		if (para_IsFactByClust == MENU_LFO_MENU_YES && pSltMode->PartTr[ii].clus == MENU_LFO_CLUST_RMAIN) continue;
		pSltMode->PartTr[ii].fact = pSltMode->PartTr[ii].ampl / tempMaxAmpl_ln_tr;
	}
	// 基准设备名
// 	char  tempMaxObjname[64];
// 	strcpy( tempMaxObjname, "" );
// 	if (tempMaxType  == OBJ_TYPE_GN)
// 	{
// 		for (ii=0; ii<m_nGnNum; ii++)
// 		{
// 			if (tempMaxObjid == m_pGnInf[ii].gn_id)
// 			{
// 				strcpy( tempMaxObjname, m_pGnInf[ii].fac_name );
// 				strcat( tempMaxObjname, m_pGnInf[ii].gn_name );
// 				break;
// 			}
// 		}
// 	}
// 	else if (tempMaxType  == OBJ_TYPE_LN)
// 	{
// 		for (ii=0; ii<m_nLnNum; ii++)
// 		{
// 			if (tempMaxObjid == m_pLnInf[ii].acln_id)
// 			{
// 				strcpy( tempMaxObjname, m_pLnInf[ii].acln_name );
// 				break;
// 			}
// 		}
// 	}
// 	else if (tempMaxType  == OBJ_TYPE_TR)
// 	{
// 		for (ii=0; ii<m_nTrNum; ii++)
// 		{
// 			if (tempMaxObjid == m_pTrInf[ii].tr_id)
// 			{
// 				strcpy( tempMaxObjname, m_pTrInf[ii].fac_name );
// 				strcat( tempMaxObjname, m_pTrInf[ii].tr_name );
// 				break;
// 			}
// 		}
// 	}

	return;
}

int CLfoAnaBase::AnaPretreat()
{
	if (SelectRefAngle2(m_VecDev) < 0)
	{
		sprintf(m_LogString, "角度转化错误");
		MessShow(m_LogString, LTERROR);
	}
	if (DataInfoStatistic(m_VecDev, 2) < 0)
	{
		sprintf(m_LogString, "估计曲线波动错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (DataFilter(m_VecDev, 2) < 0)
	{
		sprintf(m_LogString, "数据滤波错误");
		MessShow(m_LogString, LTERROR);
		//   return -1;
	}

	if (DataSample(m_VecDev, 2) < 0)
	{
		sprintf(m_LogString, "数据采样错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	// 	if (LfoPreEstimate(m_VecDev)<0)
	//     {
	//         sprintf(m_LogString,"根据波形预判错误");
	//         MessShow(m_LogString,LTERROR);
	//         return -1;
	//     }
	// 	

	return 0;
}

int CLfoAnaBase::PronyAna()
{
	vector<LFO_PRONY_INFO> tmpVecCase;

	//	for (int ii =0; ii<(int)m_VecDev.size(); ii++ )
	//	{
	//		int cts_int = m_VecDev[ii].v2d_data.size();
	//		sprintf(m_LogString,"[yjj]设备["INT_REC_FMT"],数据点数[%d]",m_VecDev[ii].dev.dev_id,cts_int);
	//		MessShow(m_LogString,LTERROR);
	//	}

	if (FormPronyData(m_VecDev, tmpVecCase, 2) < 0)
	{
		sprintf(m_LogString, "形成计算数据错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (PronyProcess(tmpVecCase) < 0)
	{
		sprintf(m_LogString, "进行Prony分析错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	sprintf(m_LogString, "分析Prony结束");
	MessShow(m_LogString);

	if (GetAnaMode(tmpVecCase, m_VecDev) < 0)
	{
		sprintf(m_LogString, "获取分析模式错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	sprintf(m_LogString, "分析获取模式结束");
	MessShow(m_LogString);

	ClearLfoPronyInfo1(tmpVecCase);

	return 0;
}

/////////////////////////////////ana部分/////////////////////////////////
//  获取ana分析的模式 填充m_nCases,m_pCases中的mode,mode_num
//   
//  MODE_PART_OBJ       m_pAllObjMode[3*Obj_Num_Max];
int CLfoAnaBase::GetAnaMode(vector<LFO_PRONY_INFO> VecCase, vector<LFO_DEV_INFO>& VecMonDev)
{
	int ii, jj, kk;
	float tmpengy = 0;

	for (ii = 0; ii < (int)VecCase.size(); ii++)
	{
		if (VecCase[ii].vec_mode.size() > 0)
		{
			tmpengy = VecCase[ii].vec_mode[0].engy;
		}		
		for (jj = 0; jj < m_nCases; jj++)
		{
			// 			if (VecMonDev[jj].dev.dev_type != MENU_LFO_DEV_GEN)
			// 			{
			// 				continue;
			// 			}
			if (m_pCases[jj].m_nID == VecCase[ii].dev.dev_id)
			{
				if (!VecCase[ii].vec_mode.empty())
				{
					for (kk = 0; kk < (int)VecCase[ii].vec_mode.size(); kk++)
					{
						if (VecCase[ii].vec_mode[kk].engy < tmpengy * m_LfoPara.mode_save_amplper / 100)
						{
							continue;
						}
						m_pCases[jj].m_result.mode[kk][0] = VecCase[ii].vec_mode[kk].damp;                    // 阻尼比百分数
						m_pCases[jj].m_result.mode[kk][1] = VecCase[ii].vec_mode[kk].freq;                    // 频率
						m_pCases[jj].m_result.mode[kk][2] = VecCase[ii].vec_mode[kk].ampl;                    // 振幅
						m_pCases[jj].m_result.mode[kk][3] = VecCase[ii].vec_mode[kk].phas;                    // 初相
						m_pCases[jj].m_result.mode[kk][4] = VecCase[ii].vec_mode[kk].real;                    // 实部(衰减因子)
						m_pCases[jj].m_result.mode[kk][5] = VecCase[ii].vec_mode[kk].imag;                    // 虚部
						m_pCases[jj].m_result.mode[kk][6] = VecCase[ii].vec_mode[kk].engy;                    // engy
						m_pCases[jj].m_result.mode_num++;
					}
				}
				break;
			}
		}
	}
	return 0;
}

int CLfoAnaBase::UnitAng(vector<float>& vdata)
{
	for (int ii = 0; ii < vdata.size(); ii++)
	{
		if (vdata[ii] < -180)
		{
			vdata[ii] = vdata[ii] + 360;
		}
		else if (vdata[ii] > 180)
		{
			vdata[ii] = vdata[ii] - 360;
		}
	}
	return 0;
}

float CLfoAnaBase::UnitAng2(float fdata)
{
	if (fdata < -180)
	{
		return (fdata + 360);
	}
	else if (fdata > 180)
	{
		return (fdata - 360);
	}
	else
	{
		return fdata;
	}
}



//将绝对角度转化为相对角度
int CLfoAnaBase::SelectRefAngle2(vector<LFO_DEV_INFO>& VecMonDev)
{
	if (VecMonDev.size() <= 0)
	{
		return -1;
	}
	sprintf(m_LogString, "角度转化");
	MessShow(m_LogString);
	int ii, jj;
	int temp_nGnData = 0;
	int data_size = 0;
	float   temp_iBuffer[DATA_NUM_MAX];
	float   temp_oBuffer[DATA_NUM_MAX];
	float   tmpval;

	vector<float> tmp_vbaseangle;//基准角度

	memset((void*)temp_iBuffer, 0, DATA_NUM_MAX * sizeof(float));
	memset((void*)temp_oBuffer, 0, DATA_NUM_MAX * sizeof(float));
	tmp_vbaseangle.clear();

	//选取一个有数据的机组 作为角度基准
	for (ii = 0; ii < VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN)
		{
			continue;
		}
		if (!VecMonDev[ii].ana_data.empty())
		{
			// 			tmp_vbaseangle = VecMonDev[ii].ana_data;
			tmp_vbaseangle.assign(VecMonDev[ii].ana_data.begin(), VecMonDev[ii].ana_data.end());
			data_size = VecMonDev[ii].ana_data.size();
			sprintf(m_LogString, "选取[%s]角度数据作为参考角,tmp_vbaseangle[0]=%f,数据个数=%d", VecMonDev[ii].dev.dev_name, tmp_vbaseangle[0], tmp_vbaseangle.size());
			MessShow(m_LogString);
			// 			for (jj = 0;jj<tmp_vbaseangle.size();jj++)
			// 			{
			// 				sprintf(m_LogString,"tmp_vbaseangle[%d]=%f",jj+1,tmp_vbaseangle[jj]);
			// 				MessShow(m_LogString);
			// 			}
			break;
		}
	}

	//所有角度数据减去基准数据，得到相对角度
	for (ii = 0; ii < VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN && VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_FAC)
		{
			continue;
		}
		if (VecMonDev[ii].ana_data.empty())
		{
			continue;
		}
		// 		sprintf(m_LogString,"设备[%s]计算相对值,size=%d",VecMonDev[ii].dev.dev_name,VecMonDev[ii].ana_data.size());
		// 		MessShow(m_LogString);
		for (int jj = 0; jj < VecMonDev[ii].ana_data.size(); jj++)
		{
			tmpval = VecMonDev[ii].ana_data[jj];
			VecMonDev[ii].ana_data[jj] = tmpval - tmp_vbaseangle[jj];
			//统一到-180~180之间
			if (VecMonDev[ii].ana_data[jj] > 180)
			{
				VecMonDev[ii].ana_data[jj] = VecMonDev[ii].ana_data[jj] - 360;
			}
			if (VecMonDev[ii].ana_data[jj] < -180)
			{
				VecMonDev[ii].ana_data[jj] = VecMonDev[ii].ana_data[jj] + 360;
			}
			// 			sprintf(m_LogString,"原始值[%f],参考值[%f],相对值[%f]",tmpval,tmp_vbaseangle[jj],VecMonDev[ii].ana_data[jj]);
			// 			MessShow(m_LogString);
		}
	}


	//求取机组相对值曲线的平均值曲线
	for (ii = 0; ii < VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN)
		{
			continue;
		}
		if (VecMonDev[ii].ana_data.empty())
		{
			continue;
		}
		for (int jj = 0; jj < VecMonDev[ii].ana_data.size(); jj++)
		{
			temp_iBuffer[jj] += VecMonDev[ii].ana_data[jj];
		}
		temp_nGnData += 1;
	}
	sprintf(m_LogString, "参考角设备数%d,数据个数%d", temp_nGnData, data_size);
	MessShow(m_LogString);
	for (int jj = 0; jj < data_size; jj++)
	{
		temp_iBuffer[jj] = (float)(temp_iBuffer[jj] / temp_nGnData);
		// 		sprintf(m_LogString,"参考角度值[%f]",temp_iBuffer[jj]);
		// 		MessShow(m_LogString);
	}
	//所有角度的相对值减去平均值
	for (ii = 0; ii < VecMonDev.size(); ii++)
	{
		if (VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_GEN && VecMonDev[ii].dev.dev_type != MENU_LFO_DEV_FAC)
		{
			continue;
		}
		for (int jj = 0; jj < VecMonDev[ii].ana_data.size(); jj++)
		{
			VecMonDev[ii].ana_data[jj] = VecMonDev[ii].ana_data[jj] - temp_iBuffer[jj];
		}
	}
	return 0;
}
