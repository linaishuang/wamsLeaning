//////////////////////////////////////////////////////////////////////////
//
// wamlfostruct.h:
//
//////////////////////////////////////////////////////////////////////////
#ifndef _WAM_LFO_STRUCT_H_
#define _WAM_LFO_STRUCT_H_

#include "PronyAnalysis.h"  // PRONY_CALC_PARA
#include "wamtoolstruct.h"  // WT_DOT_INFO,WT_DEV_INFO

#include <algorithm>
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////////
#ifndef LFO_THREAD_NUM
#define LFO_THREAD_NUM 5  // 监视线程个数
#endif

#ifndef LFO_DATA_SOURCE_FILE
#define	LFO_DATA_SOURCE_FILE	    1	/* 低频振荡案例 */
#define	LFO_DATA_SOURCE_REALTTIME	0	/* PMU实时数据 */
#endif

#ifndef LFO_SAVE_FLAG_IDEL
#define LFO_SAVE_FLAG_IDEL 0 // 空闲
#define LFO_SAVE_FLAG_START 1  // 开始振荡
#define LFO_SAVE_FLAG_HOLD 2 // 振荡持续
#define LFO_SAVE_FLAG_END 3  // 振荡结束
#endif

#define LFO_UNSAVE_WARN 1 // 不安全告警
#define LFO_STSOLD_WARN 2 // 数据不刷新告警
#define LFO_SAVE_WARN 3 // 告警->安全时发送告警

#define MAX_tIMW_WIN        40                    // 最大时间窗口30秒
#define DATA_NUM_MAX       (MAX_tIMW_WIN * MAX_FRA_PER_SEC)//  分析数据最大点数0.08/40
#define LfoRec_Time_Max  30       //15M
#define Period_Mode_Max  (LfoRec_Time_Max*6)
#define COHCY_ANGL 75.0f
#define LFO_CURVE_MAX_PNT   250       // 实时曲线最大点数
#define ZEROVALUE  0.0001
#define MULTI_MENU(n)    ((1<<(n-1))) // 多选菜单
#define CURVE_TIME 60     // 曲线显示时间
#define CURVE_NUM_PER_SEC 25 //曲线显示每秒点数
#define LPO_CURVE_MAX_PNT   CURVE_TIME*CURVE_NUM_PER_SEC       // 实时曲线最大点数
#define UNDISPLAY_VALUE 999999

//////////////////////////////////////////////////////////////////////////
// 计算参数
typedef struct type_LFO_PARA_STRUCT
{
    // 计算参数
    float freq_min;	/* 频率下限 */
    float freq_max;	/* 频率上限 */
    float lfo_freq_min; // 低频振荡频率下限
    float lfo_freq_max; // 低频振荡频率上限(第一条记录有效)
    int   time_leng;	/* 观察时间窗口 */  // 修改为float
    float time_leng_coe;  // 观察时间窗口系数
	float sample_freq;	/* 采样频率 */  // 修改为float
	float sample_freq_coe;  // 采样频率系数
	int   int_time;	/* 交叠窗口 */  // 修改为float
	float int_time_coe;  // 交叠窗口系数
    float gn_wn_ampl;	/* 机组振幅限值 */
    float gn_wn2_ampl;	/* 机组紧急告警振幅限值 */
    float ln_wn_ampl;	/* 线路振幅限值 */
    float ln_wn2_ampl;	/* 线路紧急告警振幅限值 */
    float tr_wn_ampl;	/* 变压器振幅限值 */
	float tr_wn2_ampl;	/* 变压器紧急告警振幅限值 */
	float stor_damp;	/* 保存模式阻尼限值 */
    float lack_damp;	/* 阻尼不足限值 */ // 功率振荡阻尼比限值
	float flfo_damp;	/* 预警阻尼限值 */
	float lfo_damp;	/* 告警阻尼限值 */  // 低频振荡阻尼比限值
	float damp_min;  // 阻尼比最大值(阻尼比要大于该值才有效)
    float fjud_wnap_per;	/* 预判振幅百分比 */
    float ana_ampl_per;  // 分析角度功率振幅百分比
    int   lfo_jud_num;	/* 判断次数 */
    int   warn_time_max;  // 告警持续时间限值(s)
    int   safe_time_max;  // 安全持续持续时间限值(s)
    int   delay_time;  // PMU时刻延迟时间
    float freq_diff;  // 同模式频率差
    float phas_diff;  // 同模式角度差
    int   cache_windows_num;  // 缓存窗口最大个数
    float stor_ampl;  // 保存模式幅值限值
    int   is_mon_lfo;  // 是否监视低频振荡;0,判断功率振荡;1,判断低频振荡和功率振荡
    int   time_leng_min;  // 时间窗口最小值
    int   time_leng_max;  // 时间窗口最大值
    float sample_freq_min;  // 采样频率最小值
    float sample_freq_max;  // 采样频率最大值
    int   int_time_min;  // 交叠窗口最小值
    int   int_time_max;  // 交叠窗口最大值
    float mode_save_amplper;  // 模式保存幅值百分比
	int rcd_pre_t;	/* 记录超前时间 */
	int rcd_aft_t;	/* 记录延后时间 */
	INT_REC lfo_fac_id;	/* 对应虚拟厂站ID */
	int is_warn;	/* 是否发送告警 */
	int is_samp;	/* 是否触发采样 */
	int is_tmd;	/* 是否触发tmd */
	int is_triems;	/* 是否向EMS转发 */
	int warn_app_no; // 告警发送应用号(检索主机名)
	int is_eng;  // 是否显示英文(伊朗项目)
#ifdef _D5000
	INT_REC ems_keyid;	/* EMS转发KEYID */
#else
	KEY_ID_STRU ems_keyid;	/* EMS转发KEYID */
#endif
	float   cohcy_angl;	                            //  同调角度差
    float   sh_fact;	//参与因子门槛值
	float	sh_gn_pow;	//机组耗散功率阀值
	float	sh_ln_pow;	//支路耗散功率阀值
	float	sh_phas;	//相位较大阀值
	float   max_damp;//阻尼比饱和值
    // 辅助决策参数
    // prony分析参数(采用PronyAnalysis.h中结构体)
    PRONY_CALC_PARA prony_para;
    // 控制参数
    int  devinfo_source;  // 1,从设备信息文件读取;0,从数据库设备表读取;默认为1
    int  data_source;  // 0,从动态数据库读取PMU数据;1,从数据文件中读取PMU数据;默认为1
    int  is_add_facname;  // 1,添加设备厂站名称;0,不添加设备厂站名称
    int  is_save_casefile;  // 1,生成案例文件;0,不生成案例文件
    int  is_save_devinfo;  // 1,生成设备信息文件;0,不生成设备信息文件
    int  is_cycle;  // 1,循环读取数据;0,数据读取失败后退出
    // 结果保存参数
    // 案例测试参数
    char devfile_name[MAXBUF];  // 设备信息文件名称
    char datafile_name[MAXBUF];  // 数据文件名称
    char devicefile_name[MAXBUF];  // 监视设备文件名称
    int data_pos;  // 循环数据起始位置
	int data_pos_old; //临时保存本轮计算循环数据起始
	int data_pos_osc; //小扰动保存数据起始点
	int file_start_time;//案例时间起始时刻
	int file_end_time;//案例时间结束时刻
	int file_now_time;//案例时间当前时刻
	int data_sample;//案例文件数据帧数
	// 曲线显示
	int is_lfo_curve;  // 告警时是否调用曲线控件显示振荡曲线;
	int change_data_source;	//是否数据源切换;
	int change_dev_num;	//监视设备数量是否变化;
//#ifdef _D5000
 	float			volt_thre1;								/* 电压等级1 */
 	float			volt_thre2;								/* 电压等级2 */
	float           ampl_thre1;	                            //  线路和变压器振幅限值1
	float           ampl_thre2;	                            //  线路和变压器振幅限值2
	float           ampl_thre3;	                            //  线路和变压器振幅限值3
	//广东项目增加
	float			gn_wn_ampl1;							//	机组结束振荡振幅限值
	float			ln_wn_ampl1;							//	线路结束振荡振幅限值
	float			tr_wn_ampl1;							//	变压器结束振荡振幅限值
	float			ln_swn_ampl1;						    //	线路开始振荡振幅限值1  
	float			ln_swn_ampl2;							//  线路开始振荡振幅限值2  
	float			ln_swn_ampl3;							//  线路开始振荡振幅限值3  
	float			ln_ewn_ampl1;							//  线路结束振荡振幅限值1  
	float			ln_ewn_ampl2;							//  线路结束振荡振幅限值2  
	float			ln_ewn_ampl3;							//  线路结束振荡振幅限值3  
	float			tr_swn_ampl1;							//  变压器开始振荡振幅限值1
	float			tr_swn_ampl2;							//  变压器开始振荡振幅限值2
	float			tr_swn_ampl3;							//  变压器开始振荡振幅限值3
	float			tr_ewn_ampl1;							//  变压器结束振荡振幅限值1
	float			tr_ewn_ampl2;							//  变压器结束振荡振幅限值2
	float			tr_ewn_ampl3;							//  变压器结束振荡振幅限值3
	
//#endif
	float osc_pow_ampl;	/* 小扰动振幅门槛值 */
	float osc_sec_ampl;	/* 小扰动持续时间门槛值 */
	int rec_num;		//延长保存窗口个数 
	int if_flit_lfo;	//功率波动告警是否排除低频振荡

	float			lowfreq_min;							//低频振荡频率下限
	float			lowfreq_max;							//低频振荡频率上限
	float			smp_freq_coe;							//采样频率系数
	float			warnlfo_time;							//告警持续时间限值
	float			safelfo_time;							//安全持续时间限值
	int				Voltunit_is_kV; //PMU采集电压单位是否为kV
	float			para_Ampl_aver;	//主导模式幅值占波形均值比 
	int             lfo_warn;

//广东项目增加告警 add by cyj 20170801
    float			gn_lfo_wn_ampl;								/* lfo_机组振幅限值 */
	float			gn_lfo_wn_ampl1;							//	lfo_机组结束振荡振幅限值
	float			ln_lfo_swn_ampl1;						  //	lfo_线路开始振荡振幅限值1  
	float			ln_lfo_swn_ampl2;							//  lfo_线路开始振荡振幅限值2  
	float			ln_lfo_swn_ampl3;							//  lfo_线路开始振荡振幅限值3  
	float			ln_lfo_ewn_ampl1;							//  lfo_线路结束振荡振幅限值1  
	float			ln_lfo_ewn_ampl2;							//  lfo_线路结束振荡振幅限值2  
	float			ln_lfo_ewn_ampl3;							//  lfo_线路结束振荡振幅限值3  
	float			tr_lfo_swn_ampl1;							//  lfo_变压器开始振荡振幅限值1
	float			tr_lfo_swn_ampl2;							//  lfo_变压器开始振荡振幅限值2
	float			tr_lfo_swn_ampl3;							//  lfo_变压器开始振荡振幅限值3
	float			tr_lfo_ewn_ampl1;							//  lfo_变压器结束振荡振幅限值1
	float			tr_lfo_ewn_ampl2;							//  lfo_变压器结束振荡振幅限值2
	float			tr_lfo_ewn_ampl3;							//  lfo_变压器结束振荡振幅限值3
// 	vector<float>	vpre_freq;									//  预设监视频段
	float			pre_freq1;
	float			pre_freq2;
	float			pre_freq3;
	float			pre_freq4;
	float			pre_freq5;
	int				curve_time_60;								//  是否要求显示60s曲线 1-显示60秒曲线 2-显示20秒曲线
	int				thread_mode;								//  频段划分模式;0-预设,1-自适应
	int				mainmode_num;								//  主要模式个数
	float			inf_ampl;									//  断面振幅系数
	float			freq_disp;									//当前模式频率
	float			sm_md_damp;//同模式阻尼差
	float			para_VoltAmplPer;//  厂站电压幅值百分比
	int				para_LfoAnaByPwr;
	int				para_nShwMaxFcNum;//  每群显示厂站最大数
	float			center_volt_ampl;	//振荡中心电压波动最小限值 大于该值才会判振荡中心								//当前模式频率
	int				disp_num;//主画面显示设备个数 默认显示3个
	float gn_capacity1;	/* 机组容量1 */
	float gn_capacity2;	/* 机组容量2 */
	float gn_swn_ampl2;	/* 机组开始振荡振幅限值2 */
	float gn_swn_ampl3;	/* 机组开始振荡振幅限值3 */
	float gn_ewn_ampl2;	/* 机组结束振荡振幅限值2 */
	float gn_ewn_ampl3;	/* 机组结束振荡振幅限值3 */

}LFO_PARA_STRUCT;

//////////////////////////////////////////////////////////////////////////
// 设备初始化结构采用wamtoolstruct.h中数据结构(WT_DEV_INFO)
//////////////////////////////////////////////////////////////////////////
// 设备基本信息
typedef struct  type_LFO_DEV_BASE
{
//  设备信息
	INT_REC dev_idx;  // 设备索引
	INT_REC dev_id;  // 设备ID号
	INT_REC fac_id;  // 厂站ID号
	INT_REC area_id;  // 区域ID号
	char dev_name[64];  // 设备名称
	char fac_name[64]; // 厂站名称
	char area_name[64]; // 区域名称
	int dev_type;  // 设备类型
	float ampl_limit_s;//开始振幅限值
	float ampl_limit_e;//结束振幅限值
#ifdef _CHG_AMPL
	int is_chg_ampl;	/* 是否计算动态振幅限值 */
	float chg_ampl_per;	/* 动态振幅限值百分比 */
#endif
//  选择端点信息
    INT_REC dot_id; // 设备一端ID号
    INT_REC dot_fac_id; // 厂站ID号
    INT_REC dot_area_id; // 区域ID号
    
    char dot_dev_name[64]; // 设备名称
    char dot_fac_name[64]; // 厂站名称
    char dot_area_name[64]; // 区域名称
//  PMU测点信息
    int  IsYcDefine;   // 测点是否可用
    int  nYcFlag[VEC_VIP_NUM];
    int  nYcFilNo[VEC_VIP_NUM];
    int  nYcColNo[VEC_VIP_NUM];
// 	char yc_name[VEC_VIP_NUM][64]; // 测点名称
}LFO_DEV_BASE;

// 波形统计信息
typedef struct  type_LFO_CURVE_INFO
{
	float           max_val;                                //  最大值
	RTDB_TIME         max_val_t;                              //  最大值时间
	float           min_val;                                //  最小值
	RTDB_TIME         min_val_t;                              //  最小值时间
    float           max_ampl;                               //  最大幅值
	float           aver_val;                               //  均值
	float           max_ampl_rat;                           //  最大波动率(波动幅值/均值) 
}LFO_CURVE_INFO;

// 实时模式结构
typedef struct  type_LFO_MODE_INFO
{
	RTDB_TIME         start_time;
	RTDB_TIME         end_time;
	float           ampl;                              //  振幅
	float           freq;                              //  频率
	float           damp;                              //  阻尼比
	float           phas;                              //  初相
	float           real;                              //  实部
	float           imag;                              //  虚部
	float           engy;                              //  能量
	float           engy_pre_time;                     //  单位时间能量
	float           fail;                              //  衰减
	float           aver_val;                          //  平均振幅
	int             group;                             // 分群信息
    float           fact;                             // 参与因子
	float			pow;								// 耗散功率
	int				is_source;							// 是否振荡源(实时窗口)
	int				eve_source;							// 是否振荡源(振荡事件)
	char			yc_name[64];						// 测点名称
	char			in_or_out[8];						//流入或流出
	int				stat;								//状态
// 	float			vec_pow[250];						//统计各窗口耗散功率
// 	int				pow_num;							//统计耗散功率窗口数
}LFO_MODE_INFO;
// LFO监视warning sign
typedef struct tag_OSCILLAT_FLAG
{
	INT_REC  obj_id;               //监视对象ID号
    int      lackdamp_warn;        //欠阻尼告警标志（=1 欠阻尼，=0 正常）
    int      times_ldw;            //满足欠阻尼判据次数
    int      lfo_forewarn;         //LFO预警标志   （=1 LFO预警，=0 正常）
    int      times_lfw;            //满足LFO预警判据次数
    int      lfo_warn;             //LFO告警负阻尼标志（=1 LFO告警，=0 正常）
    int      times_lw;             //满足告警次数
	int      damp_norm;            //阻尼正常
    int      times_dn;             //满足负阻尼正常次数
	float    LM_frequency;         //主导模式频率
	float    LM_damp;              //主导模式阻尼比
	int      state;                //前一轮状态
	//广东项目增加
	float    begin_ampl;		   //设备振荡开始振幅限值
	float    end_ampl;             //设备振荡结束振幅限值
}OSCILLAT_FLAG;

// 设备信息结构体
typedef struct  type_LFO_DEV_INFO
{
    LFO_DEV_BASE dev;  // 设备基本信息
    WT_DEV_INFO dev_info;  // 设备静态信息
    // 控制参数
//     int is_monitor;  // 是否监视;未监视(MENU_LFO_DEV_STS_NO	6);监视(MENU_LFO_DEV_STS_OK 1)
//     int is_prony;  // 是否进行Prony分析
    // PMU数据(vector<float>)
	vector<float> v2d_data;  // 数据(有功),监视用
	vector<float> ana_data;  // 数据(功角),分析用
	vector<float> back_data;  // 数据
// 	vector<float> back_ana;  // 数据
   // 数据/安全状态
    int stat;
	int stat_old;  // 上一轮状态
	int save_flag;
    int stat_prony;// 是否通过预判(是否需要振荡中心分析)
    int stat_source;// 是否通过预判(是否需要振荡源分析)
	int is_sm_md;//是否同调（针对断面）
	char intline_des[128];//断面组成描述
    LFO_CURVE_INFO curve;  // 波形统计
    LFO_CURVE_INFO curve_ana;  // 波形统计
    vector<LFO_MODE_INFO> vec_mode;  // 实时模式(包含多个模式并排序)
    LFO_MODE_INFO mode;  // 主导模式(实时模式中的能量最大模式)
    LFO_MODE_INFO mode_shape;  // 模态分析中能量最大的模式对应的参与设备中包含该设备的实时模式，参与因子，设备分群
    vector<LFO_MODE_INFO> vec_lead_mode;  // 整个振荡过程中主导模式
	OSCILLAT_FLAG	osciFlag;//振荡判断结构
//     // 判断告警状态时间
//     int start_time_warn;
// 	int end_time_warn;
// 	int hold_time_warn;
	// 判断低频振荡状态时间
// 	RTDB_TIME start_time_lfo;
// 	RTDB_TIME end_time_lfo;
// 	int hold_time_lfo;
    // 子线程中判据振荡/安全时间
    // 主线程中统计整个振荡过程时间
	RTDB_TIME start_time;
	RTDB_TIME end_time;
	int hold_time;

//小扰动状态、小扰动统计时间
//     int osc_stat;
// 	int osc_stat_old;  // 上一轮状态
// 	int osc_save_flag;
//     RTDB_TIME osc_start_time;
//     RTDB_TIME osc_end_time;
// 	int osc_hold_time;

	int sourceflag;//是否为振荡源及其关联支路
	int judge_num; //满足告警次数

	unsigned char lfo_is_cntr;

}LFO_DEV_INFO;


// LFO监视主导模式
typedef struct tag_LFO_LEAD_MODE               
{
	INT_REC  obj_id;               //监视对象ID号(临界群ID号为负)
	float    damping;              //振荡模式阻尼比
	float    frequency;            //振荡模式频率
	float    amplitude;            //振荡模式振幅
	float    phase;                //振荡模式初相
	float    aver_ampl;            //波形平均振幅
	float    aver_ampl_rat;        //波形平均振幅与均值的比值
	float    aver_ampl2;           //波形平均振幅2
	float    aver_ampl_rat2;       //波形平均振幅与均值的比值2
	float	 vbase;					//电压基值
}LFO_LEAD_MODE;
typedef struct type_LFO_INTLINE_INFO
{
	INT_REC recordid;
	INT_REC inf_no;
	char inf_name[64];
	int eletype;
	INT_REC line_no;
	char line_name[64];
	int onoff;
	int mpoint;	/* 测点 */
	INT_REC stat_no;	/* 测点厂站 */
	float ampl_limit_s;
	float ampl_limit_e;
    LFO_CURVE_INFO curve;  // 波形统计
    vector<LFO_MODE_INFO> vec_mode;  // 实时模式(包含多个模式并排序)
    LFO_MODE_INFO mode;  // 主导模式(实时模式中的能量最大模式)
    LFO_MODE_INFO mode_shape;  // 模态分析中能量最大的模式对应的参与设备中包含该设备的实时模式，参与因子，设备分群
    vector<LFO_MODE_INFO> vec_lead_mode;  // 整个振荡过程中主导模式
	LFO_DEV_INFO dev_info;
}LFO_INTLINE_INFO;
//////////////////////////////////////////////////////////////////////////
// 设备模式结构(LFO_DEV_MODE)
typedef struct  type_LFO_DEV_MODE
{
    LFO_DEV_BASE dev;  // 设备基本信息
    LFO_MODE_INFO mode;  // 模式信息
}LFO_DEV_MODE;

//////////////////////////////////////////////////////////////////////////
// Prony分析结构
typedef struct  type_LFO_PRONY_INFO
{
    LFO_DEV_BASE dev;  // 设备基本信息
    // PMU数据(vector<float>)
    vector<float> v2d_data;
    vector<LFO_MODE_INFO> vec_mode;  // 实时模式(包含多个模式并排序)
}LFO_PRONY_INFO;

//////////////////////////////////////////////////////////////////////////
// 模态分析结构
typedef struct  type_LFO_MODE_SHAPE
{
    LFO_MODE_INFO mode;  // 汇总模式(多个模式按能量排序)
    vector<LFO_DEV_MODE> vec_dev;  // 多个模式的参与设备(基本信息，实时模式，参与因子，设备分群)
	int is_lfo_source;
	int is_center;
	char lfo_ana_des[200];
	char is_source_des[8];
	int gn_num;
	int line_num;
	int	tr_mun;
	char lfo_type[20];
}LFO_MODE_SHAPE;

//////////////////////////////////////////////////////////////////////////
// 系统状态结构体
typedef struct  type_LFO_SYS_INFO
{
	RTDB_TIME window_id;  // 窗口ID
	float freq_min;  // 分析频率段下限
	float freq_max;  // 分析频率段上限
    int gps_time;  // PMU最新时刻
    int gps_time_old;  // 上一轮分析PMU最新时刻
	RTDB_TIME start_time;  // 本轮分析开始时间
	RTDB_TIME end_time;  // 本轮分析结束时间
    float sample_org;  // 原始数据采样间隔
    // 安全状态
    int stat;
	int stat_old;
	char stat_desc[200];  // 振荡描述信息
	char lfo_desc[200];  // 分析描述信息
    int save_flag;
	int lfo_warn_flag;   //本次振荡事件中是否已经发送低频振荡告警标志
	int lfo_type;//低频振荡类型 1表示低频振荡 2表示大幅功率波动
    INT_REC         center_id;	                            //  振荡中心
    char center_desc[128];                                  //  振荡中心描述

    // 振荡设备个数，主导模式中参与设备个数
//     LFO_MODE_INFO lead_mode;  // 主导模式
//     LFO_DEV_BASE lead_dev;  // 参与因子最大设备
	LFO_DEV_INFO lead_dev;  // 振幅最大的设备，设备模式及波形
	LFO_DEV_INFO lead_gen;  // 振幅最大的机组，设备模式及波形
    vector<LFO_DEV_INFO> vec_dev;  // 所有振荡设备信息
    vector<LFO_DEV_INFO> cen_dev;  // 振荡中心设备信息
    vector<LFO_MODE_SHAPE> vec_modeshape;  // 所有模态分析信息
    vector<LFO_MODE_SHAPE> vec_mainmode;  // 主要模式
	int lfo_gn_num; // 振荡机组个数
	int lfo_ln_num;  // 振荡线路个数
	int lfo_tr_num;  // 振荡变压器个数
	int lfo_inf_num;  // 振荡变压器个数
	char lfos_gn_name[64];	/* 振荡源机组名称 */
	int lfos_gn_num;	/* 振荡源机组数目 */
	int lfos_ln_num;	/* 振荡源线路数目 */
	int lfos_tr_num;	/* 振荡源变压器数目 */
	char lfos_des[128];	/* 振荡源描述 */
	float progress; //计算进度
	int file_end;//案例文件是否结束标志
	int warn_thread;//告警线程号
}LFO_SYS_INFO;
//////////////////////////////////////////////////////////////////////////
// 每个分析窗口结构
typedef struct  type_LFO_WINDOW_INFO
{
// 1)	窗口ID
	INT_REC window_id;
    vector<LFO_SYS_INFO> vec_sys;  // 3)	系统状态结构(多个频率段都存在告警)
}LFO_WINDOW_INFO;
//////////////////////////////////////////////////////////////////////////
// 振荡事件结构
typedef struct  type_LFO_CASE_INFO
{
// 1)	事件ID，窗口个数
	RTDB_TIME event_id;
	int stat;  // 低频振荡或功率振荡
	char case_desc[128];  // 振荡描述
	int lfo_gn_num; // 振荡机组个数
	int lfo_ln_num;  // 振荡线路个数
	int lfo_tr_num;  // 振荡变压器个数
	int lfo_inf_num;  // 振荡变压器个数
    vector<LFO_SYS_INFO> vec_windows;  // 2)	每个窗口分析结果
// 3)	振荡统计（振荡开始时间，振荡结束时间，振荡持续时间，平均频率，最小阻尼比，触发告警设备）
	RTDB_TIME start_time;
	RTDB_TIME end_time;
    int hold_time;
	LFO_MODE_INFO lead_mode;
	vector<LFO_MODE_SHAPE> vec_mainmode;
    LFO_DEV_INFO warn_dev;
    vector<LFO_DEV_INFO> vec_dev;  // 4)	振荡设备统计（按振荡设备统计本次振荡设备开始时间，结束时间，持续时间，波形统计）, 振荡设备模式（按振荡设备统计每个分析窗口的主导模式）
    vector<LFO_DEV_INFO> cen_dev;  // 4)	振荡中心统计（按振荡设备统计本次振荡设备开始时间，结束时间，持续时间，波形统计）, 振荡设备模式（按振荡设备统计每个分析窗口的主导模式）
    vector<LFO_DEV_INFO> sou_dev;  // 4)	振荡源统计（按振荡设备统计本次振荡设备开始时间，结束时间，持续时间，波形统计）, 振荡设备模式（按振荡设备统计每个分析窗口的主导模式）
//    vector<LFO_MODE_SHAPE> vec_modeshape;  // 5)	每个窗口主导模式模态结果
	int is_lfo_source;  /* 是否强迫振荡 */
	char lfos_gn_name[64];	/* 振荡源机组名称 */
	int lfos_gn_num;	/* 振荡源机组数目 */
	int lfos_ln_num;	/* 振荡源线路数目 */
	int lfos_tr_num;	/* 振荡源变压器数目 */
	char lfos_des[128];	/* 振荡源描述 */
	char center_desc[128]; /* 振荡中心描述 */
	float progress; //计算进度
	int lfo_modle;	/* 振荡模式 */
	int lfo_type;	/* 振荡类型 */
	char type_desc[64]; //振荡类型描述
	char lfo_ana_des[128]; //振荡分析描述
	char asst_desc[128]; //辅助决策描述
}LFO_CASE_INFO;
//////////////////////////////////////////////////////////////////////////
// 线程数据保存结构
typedef struct type_LFO_THREAD_INFO
{
    vector<LFO_DEV_INFO> VecDev;  // 线程的设备信息保存
    //vector<LFO_MODE_SHAPE> VecModeShape;  // 线程的模态信息保存
    LFO_SYS_INFO MonSys; // 线程的系统信息保存
// 	LFO_SYS_INFO oscMonSys; // 线程的系统信息保存
}LFO_THREAD_INFO;

typedef struct type_LFO_CASE_FILE_DEV
{
	INT_REC fac_id;  // 厂站ID
	INT_REC dev_id;  // 设备ID(端点)
	INT_REC dev_id1;  // 设备ID
	int column_id;  // 域ID
#ifndef _D5000
	KEY_ID_STRU yc_id;
#else
	INT_REC yc_id;  // 遥测ID
#endif
	int file_no;  // 文件号
	int col_no;  // 列号
	int is_warn;  // 是否振荡设备
}LFO_CASE_FILE_DEV;
//////////////////////////////////////////////////////////////////////////
// 案例报告结构
typedef struct type_LFO_CASE_FILE_STRUCT
{
	char version[10];  // 版本号
	char time[20];  // 标志文件生成时间
	char system[64];  // 系统说明
	RTDB_TIME eve_id;  // 事件ID
	RTDB_TIME start_time;  // 振荡起始时间
	RTDB_TIME end_time;  // 振荡截止时间
	RTDB_TIME hold_time;  // 持续时间
	RTDB_TIME Data_Save_start_time; //数据保存起始时间
	RTDB_TIME Data_Save_end_time; //数据保存起始时间
	char case_des[128];  // 振荡事件描述
	INT_REC warn_dev_id;  // 触发告警设备ID
	char warn_dev_name[64];  // 触发告警设备名
	float warn_freq;  // 触发告警频率
	float max_ampl;  // 振荡最大幅值
	int lfo_modle;	/* 振荡模式 */
	int lfo_type;	/* 振荡类型 */
	char type_desc[64];//振荡类型描述
	vector<LFO_CASE_FILE_DEV> vecdev;
}LFO_CASE_FILE_STRUCT;
//////////////////////////////////////////////////////////////////////////
// 设备状态判断结构
typedef struct type_LFO_DEV_STAT
{
	int warn_type; // 0,判安全;1,判告警

	float freq;  // 频率
	float ampl;  // 幅值
	float damp;  // 阻尼比
	int   start_time;  // 开始时间
	int   end_time;  // 结束时间
	int   hold_time;  // 持续时间
	int   ana_start_time;  // 本轮分析开始时间
	int   ana_end_time;  // 本轮分析结束时间
	
	float freq_min;  // 频率最小值
	float freq_max;  // 频率最大值
	float ampl_thre;  // 幅值门槛值
	float damp_thre;  // 阻尼比门槛值
	float damp_min;  // 阻尼比最小值
	int   time_max;  // 持续时间门槛值
	
	int   stat;  // 状态;0,不满足条件;1,满足条件

	float lfofreq_min;  // 低频振荡频率最小值
	float lfofreq_max;  // 低频振荡频率最大值
	float lfodamp_thre;  // 低频振荡阻尼比门槛值
	float aver_ampl;
}LFO_DEV_STAT;
//////////////////////////////////////////////////////////////////////////
// 统计意义的振荡设备信息
typedef struct type_LPO_DEV_INFO
{
//	INT_REC                 dev_idx;              //设备索引        
	INT_REC                 dev_id;               //设备ID          
	INT_REC                 fac_id_1;             //首端厂站ID号    
	INT_REC                 area_id_1;            //首端区域ID号    
	INT_REC                 fac_id_2;             //末端厂站ID号    
	INT_REC                 area_id_2;             //末端区域ID号    
	char                    dev_name[64];         //设备名          
	char                    fac_name_1[64];       //首端厂站名      
	char                    fac_name_2[64];       //末端厂站名      
	char                    area_name_1[64];      //首端区域名      
	char                    area_name_2[64];      //末端区域名      
	INT_REC                 dev_type;             //设备类型        
	unsigned char           is_monitor;           //是否监视        
	unsigned char	          is_part_ana;          //是否参与分析    
	float                   lead_ampl;            //振幅            
	float                   lead_freq;            //频率            
	float                   lead_damp;            //阻尼比          
	float                   lead_phas;            //初相            
	float                   lead_real;            //实部            
	float                   lead_imag;            //虚部            
	float                   lead_engy;            //能量            
	int                     if_lfo;               //是否出现振荡    
	RTDB_TIME                 slfo_bgn_t;           //自身LFO发生时间 
	RTDB_TIME                 slfo_end_t;           //自身LFO消失时间 
	char                    slfo_time[20];        //持续时间        
	char                    file_dir[256];        //带路径的文件名称
	vector<float>          v2d_data;
}LPO_DEV_INFO;
#define LPODEVINFOINSERT "dev_id,fac_id_1,area_id_1,fac_id_2,area_id_2,dev_name,fac_name_1,fac_name_2,area_name_1,area_name_2,dev_type,is_monitor,is_part_ana,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,if_lfo,slfo_bgn_t,slfo_end_t,slfo_time,file_dir"
//////////////////////////////////////////////////////////////////////////
// LFO参数表读取结构
typedef struct type_LFO_PARA_TABLE_READ
{
//	int record_id;	/* 记录ID */
	INT_REC record_id;	/* 记录ID */
	int dsource;	/* 数据源 */
	int mem_max_sec;	/* 缓冲最大秒数 */
	int smp_freq;	/* 采样频率 */
	float fjud_wnap_per;	/* 预判振幅百分比 */
	int flt_len;	/* 滤波窗宽 */
	float flt_coe;	/* 滤波系数 */
	int time_leng;	/* 观察时间窗口 */
	int int_time;	/* 交叠窗口 */
	float freq_min;	/* 频率下限 */
	float freq_max;	/* 频率上限 */
	float stor_damp;	/* 保存模式阻尼限值 */
	float lack_damp;	/* 阻尼不足限值 */
	float flfo_damp;	/* 预警阻尼限值 */
	float lfo_damp;	/* 告警阻尼限值 */ //低频振荡
	float gn_wn_ampl;	/* 机组振幅限值 */
	float gn_wn2_ampl;	/* 紧急告警振幅限值 */
	float ln_wn_ampl;	/* 线路振幅限值 */
	float ln_wn2_ampl;	/* 紧急告警振幅限值 */
	int lfo_jud_num;	/* 判断次数 */
	float lfo_tim_freq;	/* 时段区分频差 */
	int rcd_pre_t;	/* 记录超前时间 */
	int rcd_aft_t;	/* 记录延后时间 */
	int htime_max;	/* 最大允许时间 */
	int show_max;	/* 最大允许显示 */
	INT_REC lfo_fac_id;	/* 对应虚拟厂站ID */
	float sm_md_freq;	/* 同模频率差 */
	float sm_md_damp;	/* 同模阻尼差 */
	float cohcy_angl;	/* 同调角度差 */

	unsigned char is_warn;	/* 是否发送告警 */
	unsigned char is_samp;	/* 是否触发采样 */
	unsigned char is_tmd;	/* 是否触发tmd */
	unsigned char is_statestm;	/* 是否启动状态估计 */
	RTDB_TIME his_bgn;	/* 查询起始时刻 */
	RTDB_TIME his_end;	/* 查询截止时刻 */

	int mon_ctl;	/* 监视运行控制 */
	unsigned char is_triems;	/* 是否向EMS转发 */
#ifdef _D5000
	INT_REC ems_keyid;	/* EMS转发KEYID */
#else
	KEY_ID_STRU ems_keyid;	/* EMS转发KEYID */
#endif
	float tr_wn_ampl;	/* 变压器振幅限值 */
	float tr_wn2_ampl;	/* 紧急告警振幅限值 */

	float lowfreq_min;	/* 低频振荡频率下限 */  // float sh_gn_fact;	/* 参与机组因子门槛 */
	float lowfreq_max;	/* 低频振荡频率上限 */  // float sh_gn_ampl;	/* 参与机组幅值门槛 */
	float time_leng_coe;	/* 观察时间窗口系数 */  // float sh_gn_num;	/* 参与机组个数门槛 */
	float smp_freq_coe;	/* 采样频率系数 */  // float sh_ln_fact;	/* 可观线路因子门槛 */
	float int_time_coe;	/* 交叠窗口系数 */  // float sh_ln_ampl;	/* 可观线路幅值门槛 */
	int warnlfo_time;	/* 告警持续时间限值 */  // int htime_max;	/* 最大允许时间 */
	int safelfo_time;	/* 安全持续时间限值 */  // int show_max;	/* 最大允许显示 */
	float sh_fact;	/* 参与因子阀值 */
	float sh_phas;	/* 相角超前阀值 */
	float sh_gn_pow;	/* 机组耗散功率阀值 */
	float sh_ln_pow;	/* 支路耗散功率阀值 */

	float			volt_thre1;								/* 电压等级1 */
	float			volt_thre2;								/* 电压等级2 */
	float           ampl_thre1;	                            //  线路和变压器振幅限值1
	float           ampl_thre2;	                            //  线路和变压器振幅限值2
	float           ampl_thre3;	                            //  线路和变压器振幅限值3
	//广东项目增加
	float			gn_wn_ampl1;							//	机组结束振荡振幅限值
	float			ln_wn_ampl1;							//	线路结束振荡振幅限值
	float			tr_wn_ampl1;							//	变压器结束振荡振幅限值
	float			ln_swn_ampl1;						    //	线路开始振荡振幅限值1  
	float			ln_swn_ampl2;							//  线路开始振荡振幅限值2  
	float			ln_swn_ampl3;							//  线路开始振荡振幅限值3  
	float			ln_ewn_ampl1;							//  线路结束振荡振幅限值1  
	float			ln_ewn_ampl2;							//  线路结束振荡振幅限值2  
	float			ln_ewn_ampl3;							//  线路结束振荡振幅限值3  
	float			tr_swn_ampl1;							//  变压器开始振荡振幅限值1
	float			tr_swn_ampl2;							//  变压器开始振荡振幅限值2
	float			tr_swn_ampl3;							//  变压器开始振荡振幅限值3
	float			tr_ewn_ampl1;							//  变压器结束振荡振幅限值1
	float			tr_ewn_ampl2;							//  变压器结束振荡振幅限值2
	float			tr_ewn_ampl3;							//  变压器结束振荡振幅限值3
	float osc_pow_ampl;	/* 小扰动振幅门槛值 */
	float osc_sec_ampl;	/* 小扰动持续时间门槛值 */

	float gn_capacity1;	/* 机组容量1 */
	float gn_capacity2;	/* 机组容量2 */
	float gn_swn_ampl2;	/* 机组开始振荡振幅限值2 */
	float gn_swn_ampl3;	/* 机组开始振荡振幅限值3 */
	float gn_ewn_ampl2;	/* 机组结束振荡振幅限值2 */
	float gn_ewn_ampl3;	/* 机组结束振荡振幅限值3 */
	int thread_mode;	/* 分频段方式 */
	float pre_freq1;	/* 预设频率段1 */
	float pre_freq2;	/* 预设频率段2 */
	float pre_freq3;	/* 预设频率段3 */
	float pre_freq4;	/* 预设频率段4 */
	float pre_freq5;	/* 预设频率段5 */
	float phas_diff;	/* 断面同调相位角度差 */
	int mainmode_num;	/* 展示模式个数 */
	float inf_ampl;	/* 断面振幅系数 */
	float center_volt_ampl;	/* 振荡中心电压波动限值 */
	float para_voltamplper;	/* 厂站电压幅值系数 */
	int para_nshwmaxfcnum;	/* 振荡中心显示最多厂站数 */
	float para_ampl_aver;	/* 主导模式幅值/能量过滤系数 */
	float max_damp;	/* 阻尼比饱和值 */
	int disp_num;	/* 波动较大设备显示数量 */
	int rec_num;	/* 保存案例前移窗口个数 */
	int voltunit_is_kv;	/* 电压单位是否为千伏 */
	int is_cycle;	/* 是否循环读取案例 */
	int is_lfo_curve;	/* 是否调用曲线控件 */
	int warn_app_no;	/* 发送告警应用号 */
	int is_save_casefile;	/* 是否生成案例文件 */
	int is_save_devinfo;	/* 是否生成设备信息文件 */
	int is_add_facname;	/* 是否添加厂站名 */
	int is_eng;	/* 是否英文版 */

}LFO_PARA_TABLE_READ;
#define LFO_PARA_TABLE_READ_FLD "record_id,dsource,mem_max_sec,smp_freq,fjud_wnap_per,flt_len,flt_coe,time_leng,int_time,freq_min,"\
         "freq_max,stor_damp,lack_damp,flfo_damp,lfo_damp,gn_wn_ampl,gn_wn2_ampl,ln_wn_ampl,ln_wn2_ampl,lfo_jud_num,lfo_tim_freq,rcd_pre_t,rcd_aft_t,"\
         "htime_max,show_max,lfo_fac_id,sm_md_freq,sm_md_damp,cohcy_angl,is_warn,is_samp,is_tmd,is_statestm,his_bgn,his_end,mon_ctl,is_triems,ems_keyid,"\
         "tr_wn_ampl,tr_wn2_ampl,lowfreq_min,lowfreq_max,time_leng_coe,smp_freq_coe,int_time_coe,warnlfo_time,safelfo_time,sh_fact,sh_phas,sh_gn_pow,sh_ln_pow,"\
		 "volt_thre1,volt_thre2,ampl_thre1,ampl_thre2,ampl_thre3,gn_wn_ampl1,ln_wn_ampl1,tr_wn_ampl1,ln_swn_ampl1,ln_swn_ampl2,ln_swn_ampl3,ln_ewn_ampl1,ln_ewn_ampl2,"\
		 "ln_ewn_ampl3,tr_swn_ampl1,tr_swn_ampl2,tr_swn_ampl3,tr_ewn_ampl1,tr_ewn_ampl2,tr_ewn_ampl3,osc_pow_ampl,osc_sec_ampl,"\
		 "gn_capacity1,gn_capacity2,gn_swn_ampl2,gn_swn_ampl3,gn_ewn_ampl2,gn_ewn_ampl3,thread_mode,pre_freq1,pre_freq2,pre_freq3,pre_freq4,pre_freq5,"\
		 "phas_diff,mainmode_num,inf_ampl,center_volt_ampl,para_voltamplper,para_nshwmaxfcnum,para_ampl_aver,max_damp,disp_num,rec_num,voltunit_is_kv,is_cycle,is_lfo_curve,warn_app_no,is_save_casefile,is_save_devinfo,is_add_facname,is_eng"
// #define LFO_PARA_TABLE_READ_FLD "record_id,dsource,mem_max_sec,smp_freq,fjud_wnap_per,flt_len,flt_coe,time_leng,int_time,freq_min,"\
// 	"freq_max,stor_damp,lack_damp,flfo_damp,lfo_damp,gn_wn_ampl,gn_wn2_ampl,ln_wn_ampl,ln_wn2_ampl,lfo_jud_num,lfo_tim_freq,rcd_pre_t,rcd_aft_t,"\
// 	"htime_max,show_max,lfo_fac_id,sm_md_freq,sm_md_damp,cohcy_angl,is_warn,is_samp,is_tmd,is_statestm,his_bgn,his_end,is_triems,ems_keyid,"\
//     "tr_wn_ampl,tr_wn2_ampl,lowfreq_min,lowfreq_max,time_leng_coe,smp_freq_coe,int_time_coe,warnlfo_time,safelfo_time,sh_fact,sh_phas,sh_gn_pow,sh_ln_pow"
//#else
// 借用原参数表中域
//#define LFO_PARA_TABLE_READ_FLD "record_id,dsource,mem_max_sec,smp_freq,fjud_wnap_per,flt_len,flt_coe,time_leng,int_time,freq_min,"\
//    	"freq_max,stor_damp,lack_damp,flfo_damp,lfo_damp,gn_wn_ampl,gn_wn2_ampl,ln_wn_ampl,ln_wn2_ampl,lfo_jud_num,lfo_tim_freq,rcd_pre_t,rcd_aft_t,"\
//			"htime_max,show_max,lfo_fac_id,sm_md_freq,sm_md_damp,cohcy_angl,is_warn,is_samp,is_tmd,is_statestm,his_bgn,his_end,is_triems,ems_keyid,"\
//    	"tr_wn_ampl,tr_wn2_ampl,sh_gn_fact,sh_gn_ampl,sh_gn_num,sh_ln_fact,sh_ln_ampl,htime_max,show_max"
////#endif
// LFO参数表写入结构
typedef struct type_LFO_PARA_TABLE_MONWRITE
{
	//int record_id;	/* 记录ID */
	//int run_mode;	/* 运行方式 */
	
	RTDB_TIME cur_time;	/* 当前时间 */
	int stat;	/* 状态 */
	int stat_val;	/* 状态值 */
	char stat_ctx[200];	/* 状态信息 */

// 	RTDB_TIME lfo_bgn2;	/* LFO发生时间2 */
// 	char lfo_time2[20];	/* 持续时间2 */
// 	int lfo_modle2;	/* 振荡模式2 */
// 	int lfo_type2;	/* 振荡类型2 */
// 	int is_lfo_source2;	/* 是否强迫振荡2 */
	char lfos_des[128];	/* 振荡源描述2 */
// 	char lfo_ana_des2[200];	/* 振荡分析描述2 */
	
	
//	char case_type_des[128];	/* 振荡类型描述 */
}LFO_PARA_TABLE_MONWRITE;
#define LFO_PARA_TABLE_MONWRITE_FLD "cur_time,stat2,stat_val2,stat_ctx2,lfos_des"

typedef struct type_LFO_PARA_TABLE_WRITE
{
	//int record_id;	/* 记录ID */
	//int run_mode;	/* 运行方式 */

// 	int cur_time;	/* 当前时间 */
// 	int stat;	/* 状态 */
// 	int stat_val;	/* 状态值 */
// 	char stat_ctx[200];	/* 状态信息 */

	char case_des[128];	/* 振荡案例描述 */
	RTDB_TIME lfo_bgn;	/* LFO发生时间 */
	RTDB_TIME lfo_end;	/* LFO消失时间 */
	INT_REC warn_dev_id2;	/* 触发告警设备ID */
	char warn_dev_name2[64];	/* 触发告警设备 */
	float warn_freq;	/* 触发告警频率 */
	float aver_freq;	/* 主导平均频率 */
	int lfo_ln_num;	/* LFO线路数 */
	int lfo_gn_num;	/* LFO发电机数 */
	char center_desc[128];	/* 振荡中心描述 */
	int mode_num2;	/* 模式数2 */
	char center_desc2[128];	/* 振荡中心描述2 */
	float lead_freq2;	/* 频率2 */
	float lead_damp2;	/* 阻尼比2 */
	char case_des2[128];	/* 振荡案例描述2 */
	RTDB_TIME lfo_bgn2;	/* LFO发生时间2 */
	char lfo_time2[20];	/* 持续时间2 */
	int lfo_tr_num;	/* LFO变压器数 */
	char mon_use_time[20];	/* 监视计算耗时 */

	char lfos_gn_name[64];	/* 振荡源机组名称 */
	int lfos_gn_num;	/* 振荡源机组数目 */
	int lfos_ln_num;	/* 振荡源线路数目 */
	int lfos_tr_num;	/* 振荡源变压器数目 */
// 	char lfos_des[128];	/* 振荡源描述 */
	int lfo_inf_num;
// 	char lfo_ana_des[200];	/* 振荡分析描述 */
	char lfo_ana_des2[200];	/* 振荡分析描述2 */
// 	int lfo_modle;	/* 振荡模式 */
// 	int lfo_type;	/* 振荡类型 */
// 	int is_lfo_source;	/* 是否强迫振荡 */
	int lfo_modle2;	/* 振荡模式2 */
	int lfo_type2;	/* 振荡类型2 */
	int is_lfo_source2;	/* 是否强迫振荡2 */
	float warn_damp;	/* 告警阻尼比 */
	char lfos_des2[128];	/* 振荡源描述2 */
// 	char lfo_bgn_des[64];	/* LFO发生时间描述 */
// 	char lfo_end_des[64];	/* LFO消失时间描述 */
	char lfo_bgn_des2[64];	/* LFO发生时间描述2 */
	char lfo_end_des2[64];	/* LFO消失时间描述2 */
// 	char is_source_des[8];	/* 是否强迫振荡描述 */
	char is_source_des2[8];	/* 是否强迫振荡描述2 */

	//char case_type_des[128];	/* 振荡类型描述 */
}LFO_PARA_TABLE_WRITE;
//#define LFO_PARA_TABLE_WRITE_FLD "case_des,lfo_bgn,lfo_end,lfo_time,warn_dev_id,warn_dev_name,warn_freq,aver_freq,lfo_ln_num,lfo_gn_num,lfo_tr_num,case_type_des"
#define LFO_PARA_TABLE_WRITE_FLD "case_des,lfo_bgn,lfo_end,warn_dev_id2,warn_dev_name2,warn_freq,aver_freq,lfo_ln_num,lfo_gn_num,center_desc,mode_num2,center_desc2,lead_freq2,lead_damp2,case_des2,lfo_bgn2,lfo_time2,lfo_tr_num,mon_use_time,lfos_gn_name,lfos_gn_num,lfos_ln_num,lfos_tr_num,lfo_inf_num,lfo_ana_des2,lfo_modle2,lfo_type2,is_lfo_source2,warn_damp,lfos_des2,lfo_bgn_des2,lfo_end_des2,is_source_des2"

typedef struct type_LFO_PARA_TABLE_CLEAR
{
	char lfo_time[20];	/* 持续时间 */
	INT_REC warn_dev_id;	/* 触发告警设备ID */
	char warn_dev_name[64];	/* 触发告警设备 */
	int stat2;	/* 状态2 */
	int stat_val2;	/* 状态值2 */
	char stat_ctx2[200];	/* 状态信息2 */
	float mon_fail2;/* mon衰减2 */
	float lead_freq2;	/* 频率2 */
	float lead_damp2;	/* 阻尼比2 */
	char case_des2[128];	/* 振荡案例描述2 */
	RTDB_TIME lfo_bgn2;	/* LFO发生时间2 */
	char lfo_time2[20];	/* 持续时间2 */
	INT_REC warn_dev_id2;	/* 触发告警设备ID2 */
	char warn_dev_name2[64];	/* 触发告警设备2 */
	char center_desc2[128];	/* 振荡中心描述2 */
	
	int lfo_modle2;	/* 振荡模式2 */
	int lfo_type2;	/* 振荡类型2 */
	int is_lfo_source2;	/* 是否强迫振荡2 */
	char lfos_des2[128];	/* 振荡源描述2 */
	
	char lfo_bgn_des2[64];	/* LFO发生时间描述2 */
	char lfo_end_des2[64];	/* LFO消失时间描述2 */
	char is_source_des2[8];	/* 是否强迫振荡描述2 */
	//char case_type_des[128];	/* 振荡类型描述 */
	char lfo_ana_des[200];	/* 振荡分析描述 */
	float lead_freq;	/* 频率2 */
	float lead_damp;	/* 阻尼比2 */
	
	// 	char lfos_des[128];	/* 振荡源描述 */
	int lfo_modle;	/* 振荡模式 */
	int lfo_type;	/* 振荡类型 */
	int is_lfo_source;	/* 是否强迫振荡 */
	char lfo_bgn_des[64];	/* LFO发生时间描述 */
	char lfo_end_des[64];	/* LFO消失时间描述 */
	char is_source_des[8];	/* 是否强迫振荡描述 */
	char mode_source_des[8];	/* 当前模式是否强迫振荡描述 */
	char mode_ana_des[200];	/* 当前模式振荡分析描述 */

}LFO_PARA_TABLE_CLEAR;
#define LFO_PARA_TABLE_CLEAR_FLD "lfo_time,warn_dev_id,warn_dev_name,stat2,stat_val2,stat_ctx2,mon_fail2,lead_freq2,lead_damp2,case_des2,lfo_bgn2,lfo_time2,warn_dev_id2,warn_dev_name2,center_desc2,lfo_modle2,lfo_type2,is_lfo_source2,lfos_des2,lfo_bgn_des2,lfo_end_des2,is_source_des2,"\
								 "lfo_ana_des,lead_freq,lead_damp,lfo_modle,lfo_type,is_lfo_source,lfo_bgn_des,lfo_end_des,is_source_des,mode_source_des,mode_ana_des"

// LFO发电机表读取结构（采用wamtoolstruct.h中结构LFO_GN_TAB_CRE）
// LFO发电机表实时信息写入结构
typedef struct type_LFO_GN_TAB_MONWRITE
{
	//int gn_idx;	/* 发电机索引 */
	
	int stat;	/* 状态2 */
	int stat_val;	/* 状态值 */
	
	float lead_ampl;	/* 振幅 */
	float lead_freq;	/* 频率 */
	float lead_damp;	/* 阻尼比 */
	float lead_phas;	/* 初相 */
	float lead_real;	/* 实部 */
	float lead_imag;	/* 虚部 */
	float lead_engy;	/* 能量 */
	float lead_fail;	/* 衰减 */
	float aver_val;	/* 均值 */
	float max_val;	/* 最大值 */
	RTDB_TIME max_val_t;	/* 最大值时间 */
	float min_val;	/* 最小值 */
	RTDB_TIME min_val_t;	/* 最小值时间 */
	float max_min_dif;	/* 最大摆动 */
	float fact;	/* 参与因子2 */
	int fact_sts;	/* 参与因子状态2 */
//  	float lfo_max_min_dif;	/* LFO最大摆动 */	
// 	float fact;	/* 参与因子 */
// 	int is_lfo_source;	/* 是否扰动源 */
	float lead_gen_pow;	/* 耗散功率 */
// 	int fact_sts;	/* 参与因子状态 */
// 	int if_lfo;	/* 是否出现振荡 */
// 	// 	float lfo_bgn_val;	/* LFO前均值 */
// 	// 	float lfo_end_val;	/* LFO后均值 */
// 	int slfo_bgn_t;	/* 自身LFO发生时间 */
// 	int slfo_end_t;	/* 自身LFO消失时间 */
// 	char slfo_time[20];	/* 持续时间 */
// 	float slfo_bgn_val;	/* 自身LFO前均值 */
// 	float slfo_end_val;	/* 自身LFO后均值 */
// 	float lfo_max_val;	/* LFO最大值 */
// 	int lfo_max_val_t;	/* LFO最大值时间 */
// 	float lfo_min_val;	/* LFO最小值 */
// 	int lfo_min_val_t;	/* LFO最小值时间 */

	
	unsigned char clus;	/* 所属群 */
	int is_lfo_source;	/* 是否扰动源 */

// 	float max_ampl;	/* 最大幅值 */
// 	float max_ampl_rat;	/* 最大波动率 */
// 	unsigned char is_ang_calc;	/* 功角是否计算值 */

//	float aver_fact;	/* 参与因子平均值 */
}LFO_GN_TAB_MONWRITE;
// #define LFO_GN_TAB_MONWRITE_FLD "stat,stat_val,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,"\
//                                 "max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,is_ang_calc"
#define LFO_GN_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,max_min_dif2,fact2,fact_sts2,lead_gen_pow,clus,is_lfo_source"

typedef struct type_LFO_GN_TAB_WRITE
{
	//int gn_idx;	/* 发电机索引 */

// 	int stat;	/* 状态 */
// 	int stat_val;	/* 状态值 */
// 
//  	float lead_ampl;	/* 振幅2 */
//  	float lead_freq;	/* 频率2 */
//  	float lead_damp;	/* 阻尼比2 */
//  	float lead_phas;	/* 初相2 */
//  	float lead_real;	/* 实部2 */
//  	float lead_imag;	/* 虚部2 */
//  	float lead_engy;	/* 能量2 */
// 	float lead_fail;	/* 衰减 */
// 	float aver_val;	/* 均值 */
// 	float max_val;	/* 最大值 */
// 	int max_val_t;	/* 最大值时间 */
// 	float min_val;	/* 最小值 */
// 	int min_val_t;	/* 最小值时间 */
// 	float max_min_dif;	/* 最大摆动 */

// 	float fact;	/* 参与因子2 */
	int fact_sts;	/* 参与因子状态2 */
	int if_lfo;	/* 是否出现振荡2 */
// 	float lfo_bgn_val;	/* LFO前均值2 */
// 	float lfo_end_val;	/* LFO后均值2 */
	RTDB_TIME slfo_bgn_t;	/* 自身LFO发生时间2 */
	RTDB_TIME slfo_end_t;	/* 自身LFO消失时间2 */
	char lfo_time[20];	/* 持续时间2 */
	float slfo_bgn_val;	/* 自身LFO前均值2 */
	float slfo_end_val;	/* 自身LFO后均值2 */
	float lfo_max_val;	/* 最大值2 */
	RTDB_TIME lfo_max_val_t;	/* 最大值时间2 */
	float lfo_min_val;	/* 最小值2 */
	RTDB_TIME lfo_min_val_t;	/* 最小值时间2 */
	float lfo_max_min_dif;	/* 最大摆动2 */
// 	int is_lfo_source;	/* 是否扰动源2 */
	int is_lfo_source2;	/* 是否扰动源2 */
	float lead_gen_pow;	/* 耗散功率2 */
	unsigned char lfo_is_disp2;	/* 是否显示 */
// 	unsigned char clus;	/* 所属群 */
// 	float max_ampl;	/* 最大幅值 */
// 	float max_ampl_rat;	/* 最大波动率 */
//	unsigned char is_ang_calc;	/* 功角是否计算值 */
	//float aver_fact;	/* 参与因子平均值 */
}LFO_GN_TAB_WRITE;
#define LFO_GN_TAB_WRITE_FLD "fact_sts2,if_lfo2,slfo_bgn_t2,slfo_end_t2,lfo_time2,slfo_bgn_val2,slfo_end_val2,lfo_max_val2,lfo_max_val_t2,lfo_min_val2,lfo_min_val_t2,lfo_max_min_dif2,is_lfo_source2,lead_gen_pow2,lfo_is_disp2"


typedef struct type_LFO_FC_TAB_WRITE
{
	float v_max;	/* 电压最大值2 */
	float v_min;	/* 电压最小值2 */
	float v_ampl;	/* 电压摆动2 */
	unsigned char is_cntr;	/* 是否中心2 */
	unsigned char is_disp;	/* 是否显示2 */
	float lfo_v_max;	/* lfo电压最大值 */
	float lfo_v_min;	/* lfo电压最小值 */
	float lfo_v_ampl;	/* lfo电压摆动 */
	unsigned char lfo_is_cntr;	/* lfo是否中心 */
	unsigned char lfo_is_disp;	/* lfo是否显示 */
	unsigned char lfo_clus;	/* lfo所属群 */
}LFO_FC_TAB_WRITE;
#define LFO_FC_TAB_WRITE_FLD "v_max2,v_min2,v_ampl2,is_cntr2,is_disp2,lfo_v_max2,lfo_v_min2,lfo_v_ampl2,lfo_is_cntr2,lfo_is_disp2,lfo_clus2"


// LFO交流线段表读取结构（采用wamtoolstruct.h中结构LFO_LN_TAB_CRE）
// LFO交流线段表写入结构
typedef struct type_LFO_LN_TAB_MONWRITE
{
	//int acln_idx;	/* 交流线段索引 */
	
	int stat;	/* 状态 */
	int stat_val;	/* 状态值 */
	
	float lead_ampl;	/* 振幅 */
	float lead_freq;	/* 频率 */
	float lead_damp;	/* 阻尼比 */
	float lead_phas;	/* 初相 */
	float lead_real;	/* 实部 */
	float lead_imag;	/* 虚部 */
	float lead_engy;	/* 能量 */
	float lead_fail;	/* 衰减 */
	float aver_val;	/* 均值 */
	float max_val;	/* 最大值 */
	RTDB_TIME max_val_t;	/* 最大值时间 */
	float min_val;	/* 最小值 */
	RTDB_TIME min_val_t;	/* 最小值时间 */
	float max_min_dif;	/* 最大摆动 */
// 	int if_lfo;	/* 是否出现振荡 */
// 	
// 	int slfo_bgn_t;	/* 自身LFO发生时间 */
// 	int slfo_end_t;	/* 自身LFO消失时间 */
// 	char slfo_time[20];	/* 持续时间 */
// 	float slfo_bgn_val;	/* 自身LFO前均值 */
// 	float slfo_end_val;	/* 自身LFO后均值 */
// 	float lfo_max_val;	/* LFO最大值 */
// 	int lfo_max_val_t;	/* LFO最大值时间 */
// 	float lfo_min_val;	/* LFO最小值 */
// 	int lfo_min_val_t;	/* LFO最小值时间 */
//  	float lfo_max_min_dif;	/* LFO最大摆动 */
	
// 	float max_ampl;	/* 最大幅值 */
// 	float max_ampl_rat;	/* 最大波动率 */
// 	int slct_dot_id;	/* 选定线端ID */
// 	float fact;	/* 参与因子 */
// 	int fact_sts;	/* 参与因子状态 */
	int istie_source;	/* 是否关联扰动源2 */
	float lead_line_pow;	/* 耗散功率2 */
	float fact;	/* 参与因子2 */
	char yc_name[64];	/* 测点名称2 */
	char in_or_out[8];	/* 流出或流入2 */

}LFO_LN_TAB_MONWRITE;
//#define LFO_LN_TAB_MONWRITE_FLD "stat,stat_val,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,slct_dot_id"
//#define LFO_LN_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,lfo_max_min_dif2,istie_source2,lead_line_pow2,fact2,yc_name2,in_or_out2"
#define LFO_LN_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,max_min_dif2,istie_source,lead_line_pow,fact,yc_name,in_or_out"

typedef struct type_LFO_LN_TAB_WRITE
{
	//int acln_idx;	/* 交流线段索引 */

// 	int stat;	/* 状态 */
// 	int stat_val;	/* 状态值 */
// 
// 	float lead_ampl;	/* 振幅2 */
// 	float lead_freq;	/* 频率2 */
// 	float lead_damp;	/* 阻尼比2 */
// 	float lead_phas;	/* 初相2 */
// 	float lead_real;	/* 实部2 */
// 	float lead_imag;	/* 虚部2 */
// 	float lead_engy;	/* 能量2 */
// 	float lead_fail;	/* 衰减 */
// 	float aver_val;	/* 均值 */
// 	float max_val;	/* 最大值 */
// 	int max_val_t;	/* 最大值时间 */
// 	float min_val;	/* 最小值 */
// 	int min_val_t;	/* 最小值时间 */
// 	float max_min_dif;	/* 最大摆动 */
	int if_lfo;	/* 是否出现振荡 */

	RTDB_TIME slfo_bgn_t;	/* 自身LFO发生时间 */
	RTDB_TIME slfo_end_t;	/* 自身LFO消失时间 */
	char lfo_time[20];	/* 持续时间 */
	float slfo_bgn_val;	/* 自身LFO前均值 */
	float slfo_end_val;	/* 自身LFO后均值 */
	float lfo_max_val;	/* LFO最大值 */
	RTDB_TIME lfo_max_val_t;	/* LFO最大值时间 */
	float lfo_min_val;	/* LFO最小值 */
	RTDB_TIME lfo_min_val_t;	/* LFO最小值时间 */
	float lfo_max_min_dif;	/* LFO最大摆动 */
// 	int istie_source;	/* 是否关联扰动源 */
	int istie_source2;	/* 是否关联扰动源2 */
	float lead_line_pow;	/* 耗散功率2 */
	int lfo_is_disp2;	/* lfo是否显示2 */
//	int slct_dot_id;	/* 选定线端ID */
// 	float fact;	/* 参与因子2 */
// 	int fact_sts;	/* 参与因子状态2 */
// 	char yc_name[64];	/* 测点名称 */
// 	char in_or_out[8];	/* 流出或流入 */
}LFO_LN_TAB_WRITE;
//#define LFO_LN_TAB_WRITE_FLD "if_lfo,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif"
#define LFO_LN_TAB_WRITE_FLD "if_lfo2,slfo_bgn_t2,slfo_end_t2,lfo_time2,slfo_bgn_val2,slfo_end_val2,lfo_max_val2,lfo_max_val_t2,lfo_min_val2,lfo_min_val_t2,lfo_max_min_dif2,istie_source2,lead_line_pow,lfo_is_disp2"

// LFO变压器表读取结构（采用wamtoolstruct.h中结构LFO_TR_TAB_CRE）
typedef struct type_LFO_TR_TAB_MONWRITE
{
	//int tr_idx;	/* 变压器索引 */
	
// 	float fact;	/* 参与因子 */
// 	int fact_sts;	/* 参与因子状态 */
	int stat;	/* 状态 */
	int stat_val;	/* 状态值 */
	
	float lead_ampl;	/* 振幅 */
	float lead_freq;	/* 频率 */
	float lead_damp;	/* 阻尼比 */
	float lead_phas;	/* 初相 */
	float lead_real;	/* 实部 */
	float lead_imag;	/* 虚部 */
	float lead_engy;	/* 能量 */
 	float lead_fail;	/* 衰减 */
	float aver_val;	/* 均值 */
	float max_val;	/* 最大值 */
	RTDB_TIME max_val_t;	/* 最大值时间 */
	float min_val;	/* 最小值 */
	RTDB_TIME min_val_t;	/* 最小值时间 */
	float max_min_dif;	/* 最大摆动 */
// 	float max_ampl;	/* 最大幅值 */
// 	float max_ampl_rat;	/* 最大波动率 */
// 	unsigned char if_lfo;	/* 是否出现振荡 */
// 	
// 	int slfo_bgn_t;	/* 自身LFO发生时间 */
// 	int slfo_end_t;	/* 自身LFO消失时间 */
// 	char slfo_time[20];	/* 持续时间 */
// 	float slfo_bgn_val;	/* 自身LFO前均值 */
// 	float slfo_end_val;	/* 自身LFO后均值 */
// 	float lfo_max_val;	/* LFO最大值 */
// 	int lfo_max_val_t;	/* LFO最大值时间 */
// 	float lfo_min_val;	/* LFO最小值 */
// 	int lfo_min_val_t;	/* LFO最小值时间 */
//  	float lfo_max_min_dif;	/* LFO最大摆动 */
	int istie_source;	/* 是否关联扰动源2 */
	float lead_tr_pow;	/* 耗散功率2 */
	float fact;	/* 参与因子2 */
	char yc_name[64];	/* 测点名称2 */
	char in_or_out[8];	/* 流出或流入2 */	
}LFO_TR_TAB_MONWRITE;
//#define LFO_TR_TAB_MONWRITE_FLD "stat,stat_val,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat"
//#define LFO_TR_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,lfo_max_min_dif2,istie_source2,lead_tr_pow2,fact2,yc_name2,in_or_out2"
#define LFO_TR_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,max_min_dif2,istie_source,lead_tr_pow,fact,yc_name,in_or_out"

// LFO联络线走廊表读取结构
typedef struct type_LFO_INT_TAB_MONWRITE
{
	int stat2;	/* 状态2 */
	int stat_val2;	/* 状态值2 */
	float lead_ampl2;	/* 振幅2 */
	float lead_freq2;	/* 频率2 */
	float lead_damp2;	/* 阻尼比2 */
	float lead_phas2;	/* 初相2 */
	float lead_real2;	/* 实部2 */
	float lead_imag2;	/* 虚部2 */
	float lead_engy2;	/* 能量2 */
	float lead_fail2;	/* 衰减2 */
	float aver_val2;	/* 均值2 */
	unsigned char is_same_mod2;     /* 是否同调2 */
// 	char intline_des2[256]; /* 断面组成描述2 */
	float max_min_dif2;	/* 最大摆动 */
}LFO_INT_TAB_MONWRITE;
#define LFO_INT_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy,lead_fail2,aver_val2,is_same_mod2,max_min_dif2"

// LFO变压器表写入结构
typedef struct type_LFO_TR_TAB_WRITE
{
	//int tr_idx;	/* 变压器索引 */

// 	float fact;	/* 参与因子 */
// 	int fact_sts;	/* 参与因子状态 */
// 	int stat;	/* 状态 */
// 	int stat_val;	/* 状态值 */

// 	float lead_ampl;	/* 振幅2 */
// 	float lead_freq;	/* 频率2 */
// 	float lead_damp;	/* 阻尼比2 */
// 	float lead_phas;	/* 初相2 */
// 	float lead_real;	/* 实部2 */
// 	float lead_imag;	/* 虚部2 */
// 	float lead_engy;	/* 能量2 */
// 	float lead_fail;	/* 衰减 */
// 	float aver_val;	/* 均值 */
// 	float max_val;	/* 最大值 */
// 	int max_val_t;	/* 最大值时间 */
// 	float min_val;	/* 最小值 */
// 	int min_val_t;	/* 最小值时间 */
// 	float max_min_dif;	/* 最大摆动 */
// 	float max_ampl;	/* 最大幅值 */
// 	float max_ampl_rat;	/* 最大波动率 */
	unsigned char if_lfo;	/* 是否出现振荡 */

	RTDB_TIME slfo_bgn_t;	/* 自身LFO发生时间 */
	RTDB_TIME slfo_end_t;	/* 自身LFO消失时间 */
	char lfo_time[20];	/* 持续时间 */
	float slfo_bgn_val;	/* 自身LFO前均值 */
	float slfo_end_val;	/* 自身LFO后均值 */
	float lfo_max_val;	/* LFO最大值 */
	RTDB_TIME lfo_max_val_t;	/* LFO最大值时间 */
	float lfo_min_val;	/* LFO最小值 */
	RTDB_TIME lfo_min_val_t;	/* LFO最小值时间 */
	float lfo_max_min_dif;	/* LFO最大摆动 */

// 	int istie_source;	/* 是否关联扰动源 */
	int istie_source2;	/* 是否关联扰动源2 */
	int lfo_is_disp2;	/* lfo是否显示2 */
// 	float lead_tr_pow;	/* 耗散功率 */
// 	float fact;	/* 参与因子2 */
// 	int fact_sts;	/* 参与因子状态2 */
// 	char yc_name[64];	/* 测点名称2 */
// 	char in_or_out[8];	/* 流出或流入2 */
}LFO_TR_TAB_WRITE;
//#define LFO_TR_TAB_WRITE_FLD "fact,fact_sts,if_lfo,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif"
#define LFO_TR_TAB_WRITE_FLD "if_lfo2,slfo_bgn_t2,slfo_end_t2,lfo_time2,slfo_bgn_val2,slfo_end_val2,lfo_max_val2,lfo_max_val_t2,lfo_min_val2,lfo_min_val_t2,lfo_max_min_dif2,istie_source2,lfo_is_disp2"

// LFO联络线走廊表写入结构
typedef struct type_LFO_INF_TAB_WRITE
{
// 	float lead_ampl2;	/* 振幅2 */
// 	float lead_freq2;	/* 频率2 */
// 	float lead_damp2;	/* 阻尼比2 */
// 	float lead_phas2;	/* 初相2 */
// 	float lead_real2;	/* 实部2 */
// 	float lead_imag2;	/* 虚部2 */
// 	float lead_engy2;	/* 能量2 */
// 	float lead_fail2;	/* 衰减2 */
// 	float aver_val2;	/* 均值2 */
	int if_lfo2;	/* 是否出现振荡2 */
	RTDB_TIME lfo_bgn_t2;	/* LFO发生时间2 */
	RTDB_TIME lfo_end_t2;	/* LFO消失时间2 */
	char lfo_time2[20];	/* 持续时间2 */
	float lfo_max_val2;	/* LFO最大值2 */
	RTDB_TIME lfo_max_val_t2;	/* LFO最大值时间2 */
	float lfo_min_val2;	/* LFO最小值2 */
	RTDB_TIME lfo_min_val_t2;	/* LFO最小值时间2 */
	char intline_des2[256]; /* 断面组成描述2 */
	unsigned char lfo_is_disp2;	/* 是否显示 */
}LFO_INF_TAB_WRITE;
#define LFO_INF_TAB_WRITE_FLD "if_lfo2,lfo_bgn_t2,lfo_end_t2,lfo_time2,lfo_max_val2,lfo_max_val_t2,lfo_min_val2,lfo_min_val_t2,intline_des2,lfo_is_disp2"

// LFO振荡事件表写入结构
typedef struct type_LFO_CASE_INFO_WRITE
{
	INT_REC record_id;	/* 记录ID */
	INT_REC eve_id;	/* 事件ID */
	INT_REC win_id;	/* 窗口号 */
	RTDB_TIME analy_bgn;	/* 分析起始时间 */
	RTDB_TIME analy_end;	/* 分析截止时间 */
	float freq_max;	/* 频率上限 */
	float freq_min;	/* 频率下限 */
	int stat;	/* 系统状态 */
	int show_gn_num;	/* 振荡发电机个数 */
	int show_tr_num;	/* 振荡变压器个数 */
	int show_ln_num;	/* 振荡线路个数 */
	float freq;	/* 主导模式频率 */
	float ampl;	/* 振幅 */
	float damp;	/* 阻尼比 */
	int dev_num;	/* 参与设备个数 */
	int is_show;	/* 是否显示 */
	char case_des[128];	/* 振荡案例描述 */
}LFO_CASE_INFO_WRITE;


// LFO实时CASE窗口表写入结构
typedef struct type_LFO_WINDOWS_WRITE
{
// 	int wds_id;	/* 窗口ID */
// 
// 	int eve_id;	/* 事件ID */
// 	int win_id;	/* 窗口号 */
// 	int analy_bgn;	/* 分析起始时间 */
// 	int analy_end;	/* 分析截止时间 */
// 	int is_show;	/* 是否显示 */
// 
// 	int lfodev_id;	/* 振荡设备ID */
// 	int fac_id;	/* 厂站ID */
// 	int dev_type;	/* 设备类型 */
// 	float freq;	/* 频率 */
// 	float ampl;	/* 振幅 */
// 	float damp;	/* 阻尼比 */
// 	float aver_val;	/* 平均值 */
// 	float max_ampl_rat;	/* 最大波动率 */
	INT_REC wds_id;	/* 窗口ID */
	int wds_no;	/* 窗口序号 */
	RTDB_TIME wds_bgn;	/* 起始时刻 */
	RTDB_TIME wds_end;	/* 截至时刻 */
	int mode_num;	/* 模式数 */
	float real1;	/* 实部1 */
	float imag1;	/* 虚部1 */
	float freq1;	/* 频率1 */
	float damp1;	/* 阻尼比1 */
	float real2;	/* 实部2 */
	float imag2;	/* 虚部2 */
	float freq2;	/* 频率2 */
	float damp2;	/* 阻尼比2 */
	float real3;	/* 实部3 */
	float imag3;	/* 虚部3 */
	float freq3;	/* 频率3 */
	float damp3;	/* 阻尼比3 */
	int is_mode_show;	/* 模式是否显示 */
	RTDB_TIME occur_time;	/* 发生时间 */
	char content[255];	/* 详细信息描述 */
	int is_ctx_show;	/* 信息是否显示 */
	char center_desc[128];	/* 振荡中心描述 */
	char lastt[64];	/* 持续时间 */
	float freq_max;	/* 频率上限 */
	float freq_min;	/* 频率下限 */
	INT_REC eve_id;	/* 事件ID */
	INT_REC win_id;	/* 窗口号 */
	RTDB_TIME analy_bgn;	/* 分析起始时间 */
	RTDB_TIME analy_end;	/* 分析截止时间 */
	int is_show;	/* 是否显示 */
	float rphas;	/* 初相 */
	INT_REC lfodev_id;	/* 振荡设备ID */
	INT_REC fac_id;	/* 厂站ID */
	int dev_type;	/* 设备类型 */
	float freq;	/* 频率 */
	float ampl;	/* 振幅 */
	float damp;	/* 阻尼比 */
	float aver_val;	/* 平均值 */
	float max_ampl_rat;	/* 最大波动率 */
}LFO_WINDOWS_WRITE;

// LFO实时CASE窗口表写入结构
typedef struct type_POW_WDS_WRITE
{
	INT_REC wds_id;	/* 窗口ID */
	int wds_no;	/* 窗口序号 */
	RTDB_TIME wds_bgn;	/* 起始时刻 */
	RTDB_TIME wds_end;	/* 截至时刻 */
	int mode_num;	/* 模式数 */
	float real1;	/* 实部1 */
	float imag1;	/* 虚部1 */
	float freq1;	/* 频率1 */
	float damp1;	/* 阻尼比1 */
	float real2;	/* 实部2 */
	float imag2;	/* 虚部2 */
	float freq2;	/* 频率2 */
	float damp2;	/* 阻尼比2 */
	float real3;	/* 实部3 */
	float imag3;	/* 虚部3 */
	float freq3;	/* 频率3 */
	float damp3;	/* 阻尼比3 */
	int is_mode_show;	/* 模式是否显示 */
	RTDB_TIME occur_time;	/* 发生时间 */
	char content[255];	/* 详细信息描述 */
	int is_ctx_show;	/* 信息是否显示 */
	char center_desc[128];	/* 振荡中心描述 */
	char lastt[64];	/* 持续时间 */
	float freq_max;	/* 频率上限 */
	float freq_min;	/* 频率下限 */
	INT_REC eve_id;	/* 事件ID */
	INT_REC win_id;	/* 窗口号 */
	RTDB_TIME analy_bgn;	/* 分析起始时间 */
	RTDB_TIME analy_end;	/* 分析截止时间 */
	int is_show;	/* 是否显示 */
	float rphas;	/* 初相 */
	INT_REC lfodev_id;	/* 振荡设备ID */
	INT_REC fac_id;	/* 厂站ID */
	int dev_type;	/* 设备类型 */
	float freq;	/* 频率 */
	float ampl;	/* 振幅 */
	float damp;	/* 阻尼比 */
	float aver_val;	/* 平均值 */
	float max_ampl_rat;	/* 最大波动率 */
	char lfo_time[64];	/* 持续时间 */
	float lfo_max_val;	/* LFO最大值 */
	RTDB_TIME lfo_max_val_t;	/* LFO最大值时间 */
	float lfo_min_val;	/* LFO最小值 */
	RTDB_TIME lfo_min_val_t;	/* LFO最小值时间 */
	char lfodev_name[64];	/* 振荡设备名称 */
	char fac_name[64];	/* 厂站名称 */
}POW_WDS_WRITE;
// LFO实时CASE模态全表写入结构
typedef struct type_LFO_MODESHAPE_WRITE
{
	INT_REC record_id;	/* 记录号 */

	float rreall;	/* 特征值实部 */
	float rimag;	/* 特征值虚部 */
	float rmgnt;	/* 特征值幅值 */
	float rphas;	/* 特征值相角 */
	float fact;	/* 参与因子 */
	int fact_sts2;	/* 参与因子状态 */
	int dev_id;	/* 设备ID */
	char dev_name[64];	/* 设备名称 */
	int dev_type;	/* 设备类型 */
	int fac_id;	/* 所属厂站 */
	int area_id;	/* 所属区域 */

	unsigned char clus;	/* 所属群 */
	float freq;	/* 频率 */
	float damp;	/* 阻尼比 */
	float ampl;	/* 振幅 */
	float engy;	/* 能量 */
	float aver_val;	/* 平均值 */
	float max_ampl;	/* 最大幅值 */
	float max_ampl_rat;	/* 最大波动率 */
	char last[64];	/* 持续时间 */
	float freq_max;	/* 频率上限 */
	float freq_min;	/* 频率下限 */
	INT_REC eve_id;	/* 事件ID */
	INT_REC win_id;	/* 窗口号 */
	RTDB_TIME analy_bgn;	/* 分析起始时间 */
	RTDB_TIME analy_end;	/* 分析截止时间 */
	int is_show;	/* 是否显示 */
	float iniphas;	/* 初相 */
}LFO_MODESHAPE_WRITE;

//////////////////////////////////////////////////////////////////////////
typedef struct tab_lfo_rt_curve1
{
	char dev_name[128];	/* 曲线名称 */
	char unit[8];	/* 变量单位 */
	int sam_freq;	/* 采样频率 */
	RTDB_TIME cuv_bgn_sec;	/* 起始时刻 */
	RTDB_TIME cuv_end_sec;	/* 结束时刻 */
	int cuv_sec;	/* 时间 */
	int cuv_pnt;	/* 点数 */
	float cuv_stp;	/* 步长 */
	float max_val;	/* 最大值 */
	float min_val;	/* 最小值 */
	float ave_val;	/* 平均值 */
	float seq_cuv_data[125];	/* seq_cuv_data数组 */
}TAB_LFO_RT_CURVE1;
#define TAB_LFO_RT_CURVE1_FLD "dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,max_val,min_val,ave_val,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125"

typedef struct tab_lfo_rt_curve2
{
	float seq_cuv_data[125];	/* seq_cuv_data数组 */
}TAB_LFO_RT_CURVE2;
#define TAB_LFO_RT_CURVE2_FLD "seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"

typedef struct tab_lfo_rt_curve_id
{
	INT_REC dev_idx;	/* 设备索引ID */
	INT_REC dev_id;	/* 曲线ID */
}TAB_LFO_RT_CURVE_ID;
#define TAB_LFO_RT_CURVE_ID_FLD "dev_idx,dev_id"

//////////////////////////////////////////////////////////////////////////
typedef struct tab_lfo_pmu_row
{
	RTDB_TIME         begin_time;
	RTDB_TIME             end_time;
	vector<float>	vec_value;
};

typedef struct tab_lfo_pmu_header 
{
	RTDB_TIME			begin_time;
	RTDB_TIME			end_time;
	vector<INT_REC> vec_eleid;
	vector<string>  vec_elename;
};

typedef struct tab_lfo_pmu_data
{
	tab_lfo_pmu_header		head;
	vector<tab_lfo_pmu_row> vec_row;
}TAB_LFO_PMU_DATA; 

typedef struct tag_VEC_YC_FCNO
{
    int           viFilNo;                                //  
    int           viColNo;
}VEC_YC_FCNO;                                               //  遥测相量文件号列号
// 数值时刻
typedef struct  tag_VALUE_TIME_STRU
{
    float           value;                                  //  数值
    //struct timeval	time;                                   //  时刻
    time_t       	time;                                    //  时刻
}VALUE_TIME_STRU;


/////////////////////////////////// 机组信息 //////////////////////////////
// 机组静态信息
typedef struct tag_LFO_GN_INF
{
    INT_REC         gn_idx;                                 //  机组索引
	char            gn_name[64];                            //  机组名称
    INT_REC         gn_id;                                  //  机组ID号
    INT_REC         fac_id;                                 //  厂站ID号
    char            fac_name[64];                           //  厂站名称
	INT_REC         area_id;	                            //  区域ID号
	char            area_name[64];	                        //  区域名称
	float           snom;                                   //  额定功率（MVA）
    float           pnom;                                   //  额定出力（MW）
	float           pmin;	                                //  有功下限
	float           pmax;	                                //  有功上限
	float           qmin;	                                //  无功下限
	float           qmax;	                                //  无功上限
	float           tj;	                                    //  惯性时间常数
    float           emws;                                   //  机组动能（MWo秒）tj*snom
} LFO_GN_INF;

// 机组控制信息
typedef struct tag_LFO_GN_CTL
{
    INT_REC         gn_id;                                  //  机组ID号
	unsigned char   is_monitor;                             //  是否监视
	unsigned char   is_part_ana;                            //  是否参与分析
	unsigned char   is_agl_calc;                            //  计算功角优先
	float			lfo_gn_swn;								//	低频振荡开始振幅限值
	float			lfo_gn_ewn;								//	低频振荡结束振幅限值
} LFO_GN_CTL;
#define LFO_GN_CTL_FLD "gn_id,is_monitor,is_part_ana,is_agl_calc,lfo_gn_swn,lfo_gn_ewn"

// 发电机数据
typedef struct tag_LFO_GN_DAT
{
    INT_REC         gn_id;                                  //  机组ID号
	float           ang[DATA_NUM_MAX];                      //  功角
	float           pwr[DATA_NUM_MAX];                      //  有功
    float           u1a[DATA_NUM_MAX];                      //  电压相角
	
	int             SecSts[MAX_tIMW_WIN];                   //  每秒量测状态
	int             stat;                                   //  计算数据状态(正常才可用)
    VEC_YC_FCNO     VecBadData[3];                             //  量测不可用文件号  // 2008-07-16
	
	int             is_calc;                                //  是否需要计算 08-02-02
	VALUE_TIME_STRU max_val;                                //  最大值
	VALUE_TIME_STRU min_val;                                //  最小值
	float           ave_val;                                //  均值
    float           max_ampl;                               //  最大摆动
	float           max_ampl_rat;                           //  最大波动率(波动幅值/均值)
	float           max_ampl_ran;							//  最大波动幅度(波动幅值/振幅)
}LFO_GN_DAT;

// 机组监视信息
typedef struct tag_LFO_GN_MON
{
    INT_REC         gn_id;                                  //  机组ID号
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	INT_REC         damp_grad;                              //  阻尼等级
	int             damp_grad_val;                          //  阻尼等级值
	INT_REC         mode_rcd_id;                            //  在设备模式全表中首记录ID号
	int             mode_rcd_num;	                        //  在设备模式全表中的记录个数
	float           lead_ampl;                              //  振幅
	float           lead_freq;                              //  频率
	float           lead_damp;                              //  阻尼比
	float           lead_phas;                              //  初相
	float           lead_real;                              //  实部
	float           lead_imag;                              //  虚部
	float           lead_engy;                              //  能量
	float           lead_fail;                              //  衰减
	float           aver_val;                               //  均值
	float           max_val;                                //  最大值
	RTDB_TIME         max_val_t;                              //  最大值时间
	float           min_val;                                //  最小值
	RTDB_TIME         min_val_t;                              //  最小值时间
	float           max_min_dif;                            //  最大摆动（峰峰值南网）
    float           max_ampl;                               //  最大幅值
	float           max_ampl_rat;                           //  最大波动率(波动幅值/均值)
    unsigned char   is_ang_calc;                            //  功角是否计算值  2008-05-22 by zhw
} LFO_GN_MON;
#define LFO_GN_MON_FLD "gn_id,stat,stat_val,damp_grad,damp_grad_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,is_ang_calc"

// 机组遥测信息
typedef struct tag_LFO_GN_PMU
{
    INT_REC         gn_id;                                  //  机组ID号
	char            gn_name[64];                            //  机组名称// 2008-01-08
    VEC_YC_FCNO     vecPwr;                                 //  机组有功
	
    VEC_YC_FCNO     vecAng;                                 //  机组功角
    VEC_YC_FCNO     vecAnc;                                 //  机组计算功角
    VEC_YC_FCNO     vecEv;                                  //  机组内电势
    VEC_YC_FCNO     vecEa;                                  //  机组内电势相角
    unsigned char   curSts;                                 //  当前状态
    unsigned char   hisSts;                                 //  历史状态
	unsigned char   is_agl_calc;                            //  计算功角优先
    int             ang_flag;                               //  1: 功角测量值可用  2: 功角计算值可用  3: 功角测量值和计算值都可用  //2008-05-29 by zhw
    unsigned char   is_ang_calc;                            //  功角是否计算值  2008-05-21 by zhw at fj
} LFO_GN_PMU;

// 参与因子
typedef struct  type_OBJ_ACT_FACT
{
    INT_REC         obj_idx;                                //  设备索引号  2008-04-03 added by zhw at yn
	INT_REC         obj_id;                                 //  设备ID(设备指线路/发电机)
	float           fact;                                   //  参与因子
	int             fact_sts;                               //  参与因子状态
	unsigned char   clus;                                   //  所属群
    float           aver_fact;                              //  参与因子平均值 2008-06-05 by zhw
    int             num_fact;                               //  计算参与因子次数
}OBJ_ACT_FACT;
#define TYPE_OBJ_ACT_FACT "fact,fact_sts,clus"
#define TYPE_OBJ_AVER_FACT "aver_fact"

//分析时段对应主导模式
typedef struct  type_period_lead_mode
{
    time_t   bgn_t;           //开始时刻
    time_t   end_t;           //截止时刻
    float    ampl;            //振幅
    float    freq;            //频率
    float    damp;            //阻尼比
    float    phase;           //初相
    float    real;            //实部
    float    image;           //虚部
}PERIOD_LEAD_MODE;

// 厂站动态数据
typedef struct tag_LFO_FC_DAT
{
	INT_REC         fac_id;                                 //  厂站ID
	float           u1v[DATA_NUM_MAX];                      //  电压幅值
	float           u1a[DATA_NUM_MAX];                      //  电压相角
	float           fhz[DATA_NUM_MAX];                      //  频率
	int             SecSts[MAX_tIMW_WIN];                   //  每秒量测状态
	int             stat;                                   //  计算数据状态(正常才可用)
    VEC_YC_FCNO     VecBadData[3];                             //  数据不可用文件号  // 2008-07-16 by zhw
	VALUE_TIME_STRU max_val;                                //  最大值
	VALUE_TIME_STRU min_val;                                //  最小值
	float           ave_val;                                //  均值
    float           max_ampl;                               //  最大摆动
	float           max_ampl_rat;                           //  最大波动率(波动幅值/均值)
	float           max_ampl_ran;   //  最大波动幅度(波动幅值/振幅)
} LFO_FC_DAT;
// 厂站振荡信息
typedef struct tag_LFO_FC_ANA
{
	INT_REC         fac_id;                                 //  厂站ID
	char            fac_name[64];                           //  厂站名称
	int             stat;                                       //  厂站ID
	float           lfo_v_aver;                                 //  电压均值
	float           lfo_v_max;                                  //  电压最大值
	float           lfo_v_min;                                  //  电压最小值
	float           lfo_v_ampl;                                 //  电压摆动
	float           lfo_f_aver;                                 //  频率均值
	float           lfo_f_max;                                  //  频率最大值
	float           lfo_f_min;                                  //  频率最小值
	float           lfo_f_ampl;                                 //  频率摆动
	unsigned char   lfo_is_cntr;                                //  是否中心
	unsigned char   lfo_is_disp;                                //  是否显示
	unsigned char   lfo_clus;                                   //  所属群
	float             damp;                                 //阻尼比
	float             freq;                                 //频率
	float             ampl;                                 //振幅
	float             phas;                                 //初相
	float             mgnt;                                 //量级
	float             real;                                 //实部  // 
	float             imag;                                 //虚部
	float             fact;                                 //参与因子
	float             engy;                                 //能量
} LFO_FC_ANA;


//振荡过程被监视元件结果
typedef struct  type_lfo_obj_res_save
{
	INT_REC                 obj_id;                 //设备ID号(设备指线路/发电机)
	char                    obj_name[64];           //设备名称
	INT_REC                 fac_id;                 //厂站名称
	char                    fac_name[64];           //厂站名称
	// 	int                     ref_obj_id;             //参考设备ID号(针对发电机或（相对相角）)
	// 	char                    ref_obj_name[64];       //参考设备名称
	unsigned char           obj_type;               //设备类型(线路/发电机)
	float                   b_aver_pqv0[3];         //初始平均pqv(系统lfo)
	float                   e_aver_pqv0[3];         //结束平均pqv
	int                     stat;                   //设备状态
	int                     if_lfo;	                //是否出现振荡
	time_t                  lfo_b_t;                //开始时刻
	time_t                  lfo_e_t;                //结束时刻
	int                     hold_time;              //持续时间
	float                   b_aver_pqv1[3];         //初始平均pqv(自身lfo)
	float                   e_aver_pqv1[3];         //结束平均pqv
	VALUE_TIME_STRU         max_value;              //最大值
	VALUE_TIME_STRU         min_value;              //最小值
    float                   max_ampl;               //最大摆动(峰谷差)
	int                     mode1_num;
	PERIOD_LEAD_MODE        mode1[Period_Mode_Max]; //主导模式1
	// 	int                     mode2_num;
	// 	PERIOD_LEAD_MODE        mode2[Period_Mode_Max]; //主导模式2
	// 	int                     mode3_num;
	// 	PERIOD_LEAD_MODE        mode3[Period_Mode_Max]; //主导模式3
    float                   max_min_dif;            //峰峰值（南网更新）
}LFO_OBJ_RES_SAVE;
typedef struct type_Gn_warn_time
{
	time_t m_GnStartTime; 
	time_t m_GnEndTime; 
}TYPE_GN_WARN_TIME;

typedef struct  tag_CURVE_DATA
{
	float           t_GPS;                                  //  厂站ID
	int             m_nPoints;
	float           t_length;
	float           y_value[DATA_NUM_MAX];
	float           t_value[DATA_NUM_MAX];
}CURVE_DATA;

typedef struct  tag_MODE_RESULT
{
	float            t_GPS;
	int		         mode_num;
	float	         mode[MODE_NUM_MAX][8];       
	bool	         modeStatus[MODE_NUM_MAX];
}MODE_RESULT;

typedef struct  tag_CASE_TYPE
{
	INT_REC     m_nID;
	int         m_nStatus;    // 0 未处理 1 正在处理中 2 处理完毕
	int         m_time;       // 处理的时间长度,毫秒
	CURVE_DATA  m_curve;      // 曲线数据
	MODE_RESULT m_result;     // 计算结果
	int         m_nClusterID;
	int			m_type;
	char		m_name[64]; 
	struct timeb timeStart;  // 开始计算时刻
	struct timeb timeEnd;    // 结束计算时刻
	
}CASE_TYPE;
// Prony分析结果	
typedef struct tag_PRONY_ANAL_RSLT
{
	INT_REC  obj_id;               //监视对象ID号(临界群ID号为负)
	char     obj_name[64];        //监视对象名称(200701)
    int      mode_num;             //振荡模式个数
    float    mode[MODE_NUM_MAX][8];//振荡模式   (增加预测幅值)
}PRONY_ANAL_RSLT;

/* LFO实时曲线表 */ // db_struct_wamap.h
typedef struct tab_lfo_rt_curve
{
	INT_REC dev_idx;	/* 设备索引ID */
	INT_REC dev_id;	/* 曲线ID */
	char dev_name[128];	/* 曲线名称 */
	char unit[8];	/* 变量单位 */
	int sam_freq;	/* 采样频率 */
	RTDB_TIME cuv_bgn_sec;	/* 起始时刻 */
	RTDB_TIME cuv_end_sec;	/* 结束时刻 */
	int cuv_sec;	/* 时间 */
	int cuv_pnt;	/* 点数 */
	float cuv_stp;	/* 步长 */
	float max_val;	/* 最大值 */
	float min_val;	/* 最小值 */
	float ave_val;	/* 平均值 */
	float seq_cuv_data[250];	/* seq_cuv_data数组 */
}TAB_LFO_RT_CURVE;
// 科东平台综合智能告警振荡设备信息
typedef struct type_OSCI_DEV_INFO_STRU
{
	INT_REC osci_id;  // 振荡因子*1000
	INT_REC key_id;  // 设备ID
	int group_num;  // 所属的群1/2
    char   group_name[64];          //所属的群
	char descr[64];  // 设备描述
	float damp_rate;  // 阻尼比
	float oscillation_fre;  // 振荡频率
	float oscillation_value;  // 振荡幅值
	float phase_angle;  // 相位
}OSCI_DEV_INFO_STRU;
// 科东平台综合智能告警结构体
typedef struct type_LOW_FRE_OSCI_INFO_TO_IA_STRU
{
	INT_REC search_id;  // 振荡中心(设备ID)
	time_t occur_time;  // 故障时间
	short msec;  // 毫秒
	RTDB_TIME end_time;  // 结束时间
	INT_REC dev_id;  // 故障设备(通过此id,调阅WAMS曲线)
	char dev_descr[64];  // 故障设备描述
	INT_REC st_id;  // 故障设备所属厂站id
	int app_id;  // 发送进程所属应用id
	short criterion_num;  // 振荡设备信息个数OSCI_DEV_INFO的个数
	INT_REC osci_id;  // 振荡源（首次填id，后续填0、1，表明该振荡源是否仍存在）
	char status;  // 振荡状态（开始、持续、结束）
	float damp_rate;  // 阻尼比
	float oscillation_fre;  // 振荡频率
	float oscillation_value;  // 振荡幅值
	float phase_angle;  // 相位
	short continus_flag;  // 续传标志（0首次，1续传）
}LOW_FRE_OSCI_INFO_TO_IA_STRU;


#endif
