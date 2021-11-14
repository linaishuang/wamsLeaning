
////计算类公用结构定义： 
//  公用宏定义
//  多线程计算控制结构
//
#ifndef _WAMS_LFO_STRUCT_H_
#define _WAMS_LFO_STRUCT_H_
////  os : #ifndef _WINDOWS32 _SUN _ALPHA

//#include "wamap_struct.h"            // MAX_FRA_PER_SEC    2006-08-04 
#include "PronyAnalysis.h"                     //  PRONY_MAX_MODE
// #ifndef _WINDOWS32
// 	#include <sys/time.h>                        //  time_t, tm, localtime(), mktime()
// 	#include <sys/timeb.h>                       //  timeb, ftime()
//     //#include "../publicfunction/ItfcWamMemApi.h"
//     #include "../lib/PronyAnalysis.h"                     //  PRONY_MAX_MODE    
// #else
//     #include "..\lib\PronyAnalysis.h"                     //  PRONY_MAX_MODE 
//     //#include "..\publicfunction\ItfcWamMemApi.h"   
// #endif


//  宏定义
/******************************************************************************/
#define MAX_tIMW_WIN        40                    // 最大时间窗口30秒
#define DATA_NUM_MAX       (MAX_tIMW_WIN * MAX_FRA_PER_SEC)//  分析数据最大点数0.08/40
#define LFO_SMP_FRQ		    12                    //  采样频率
#define REF_ANG_MAX_PNT	    (2*DATA_NUM_MAX)       //  参考角预选数据最大点数0.08
#define MODE_NUM_MAX        PRONY_MAX_MODE
/******************************************************************************/
//  宏定义
#define MAX_TOTAL_TIME      30                     //  最大允许计算时间(s)

// 2007-01-19 save result
#define LfoRec_Time_Max  30       //15M
#define Period_Mode_Max  (LfoRec_Time_Max*6)
/******************************************************************************/
//////////////////////////////////////////////////////////////////////////
// #define VEC_V_NUM                       6                   //  三相电压变量数 (UA UB UC)
// #define VEC_VI_NUM                      12                  //  三相电流电压变量数 (IA IB IC UA UB UC)

// #define STA_PMU_GK_OFF                  0X01                //  PMU通道退出  //CStatusOp.h STATE_YC_GK_OFF
// #define STA_PMU_NOT_CHANGE              0X02                //  PMU数据不刷新//CStatusOp.h STATE_YC_NOT_CHANGE
// #define STA_PMU_BAD_DATA                0X03                //  PMU数据不可用//(STA_PMU_GK_OFF && STA_PMU_NOT_CHANGE)// 070313 是否可用STA_PMU_NOT_CHANGE）代替STA_PMU_BAD_DATA
// #define SAT_PMU_SIMU_DATA               0XFFFFFFFF          //  拟合值  2008-10-11 by zhw

#define  LFO_CURVE_CYC       3         // 实时曲线刷新周期(s)
#define  LFO_CURVE_TIME_LENG 20        // 实时曲线窗口长度
#define  LFO_CURVE_MAX_PNT   250       // 实时曲线最大点数
#define  LFO_CURVE_PNT		 125       // TableNet对个数限制，需要分开

#define TIME2STR 
//////////////////////////////////////////////////////////////////////////
typedef struct tag_VEC_YC_FCNO
{
    int           viFilNo;                                //  
    int           viColNo;
}VEC_YC_FCNO;                                               //  遥测相量文件号列号

typedef struct tag_VEC_V_FCNO
{
    int           viFilNo[VEC_V_NUM];                     //  (UA UB UC)
    int           viColNo[VEC_V_NUM];
}VEC_V_FCNO;                                                //  三相电压相量文件号列号

typedef struct tag_VEC_VI_FCNO
{
    int           viFilNo[VEC_VI_NUM];                    //  (IA IB IC UA UB UC)
    int           viColNo[VEC_VI_NUM];
}VEC_VI_FCNO;                                               //  三相电流电压相量文件号列号

typedef struct tag_VEC_DATA_STS
{
    int             nFraNum;                                //  总帧数
    int             nBadNum;                                //  无效桢数
    int             nBadSeq;                                //  最大连续无效桢数
    time_t          tBadSec;                                //  最近无效时间
    int             iBadFra;                                //  最近无效桢号
    unsigned char   bPmuOff;                                //  PMU通道退出信号
    VEC_YC_FCNO     VecBadData[3];                          //  无效帧文件号  // 2008-07-16
}VEC_DATA_STS;                                              //  高速缓存区动态数据质量状态
/////////////////////////////////// 参数信息 //////////////////////////////
typedef struct  tag_PARA_LFO_ANA
{
	int             mem_max_sec;                            //  缓冲最大秒数
	int             smp_freq;	                            //  采样频率
	int             flt_len;	                            //  滤波窗宽
	float           flt_coe;	                            //  滤波系数
	int             time_leng;	                            //  观察时间窗口
	int             int_time;	                            //  交叠窗口
	float           freq_min;	                            //  频率下限
	float           freq_max;	                            //  频率上限
	int             sso_jud_num;	                        //  判断次数

	int             data_pos;                               //  记录新数据文件读取指针


	float			volt_thre1;								//	电压等级1
	float			volt_thre2;								//	电压等级2
	float			gn_swn_ampl ;							//	机组开始振荡振幅限值   
	float			gn_ewn_ampl ;							//	机组结束振荡振幅限值   
	float			ln_swn_ampl1;							//	线路开始振荡振幅限值1  
	float			ln_swn_ampl2;						  //	线路开始振荡振幅限值2  
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

	float			delay_time;								//  召唤长录波文件延时

}PARA_LFO_ANA;                        

// 输出控制
typedef struct  tag_PARA_DISP_CTL
{
	int             dsource;	                            //  数据源
	float           sso_tim_freq;	                        //  时段区分频差
	INT_REC         sso_fac_id;	                            //  对应虚拟厂站ID
	unsigned char   is_warn;	                            //  是否发送告警
	unsigned char   is_samp;	                            //  是否触发采样
	int             show_max;	                            //  最大允许显示
}PARA_DISP_CTL;

// 进程信息
typedef struct tag_PARA_PROC_CTL
{
	int              proc_ctl;                              //  运行方式控制
	int              proc_tim;                              //  运行耗时
	unsigned char    proc_sts;	                            //  运行状态
	char             proc_ctim[20];	                        //  运行耗时char
	char             proc_ctx[64];	                        //  运行信息
}PARA_PROC_CTL;

// 数值时刻
typedef struct  tag_VALUE_TIME_STRU
{
	float           value;                                  //  数值
	//struct timeval	time;                                   //  时刻
	time_t       	time;                                    //  时刻
}VALUE_TIME_STRU;

/////////////////////////////////// 机组信息 //////////////////////////////
// 机组静态信息
typedef struct tag_SSO_GN_INF
{
    INT_REC         gn_idx;    //  机组索引
	char            gn_name[64];  //  机组名称
    INT_REC         gn_id;     //  机组ID号
    INT_REC         fac_id;     //  厂站ID号
    char            fac_name[64];  //  厂站名称
	INT_REC         area_id;	      //  区域ID号
	char            area_name[64];	   //  区域名称
	float           snom;           //  额定功率（MVA）
    float           pnom;         //  额定出力（MW）
	float           pmin;	         //  有功下限
	float           pmax;	         //  有功上限
	float           qmin;	         //  无功下限
	float           qmax;	         //  无功上限
} SSO_GN_INF;


// 机组控制信息
typedef struct tag_SSO_GN_CTL
{
    INT_REC         gn_id;                                  //  机组ID号
	unsigned char   is_monitor;                             //  是否监视
// 	unsigned char   is_part_ana;                            //  是否参与分析
// 	unsigned char   is_agl_calc;                            //  计算功角优先
	float			sso_gn_swn;								//	低频振荡开始振幅限值
	float			sso_gn_ewn;								//	低频振荡结束振幅限值
} SSO_GN_CTL;
#define SSO_GN_CTL_FLD "gn_id,is_monitor,sso_gn_swn,sso_gn_ewn"

// 发电机数据
typedef struct tag_SSO_GN_DAT
{
	INT_REC         gn_id;  //  机组ID号
	float           ampl[DATA_NUM_MAX]; //  幅值
	float           freq[DATA_NUM_MAX];  //  频率
	float           pwr[DATA_NUM_MAX];  //  有功
	int				sso[DATA_NUM_MAX]; //  状态
	int             SecSts[MAX_tIMW_WIN]; //  每秒量测状态
    VEC_YC_FCNO     VecBadData[3];                             //  量测不可用文件号  // 2008-07-16
	int             stat;                   //  数据质量
}SSO_GN_DAT;

// 机组监视信息
typedef struct tag_SSO_GN_MON
{
	INT_REC         gn_id;  //  机组ID号
	INT_REC         fac_id;    //  厂站ID号
	char            gn_name[64];    //  机组名称
	int             stat;      //  状态
	int             stat_val;   //  状态值
	float           lead_ampl; //  振幅
	float           lead_freq;  //  频率
	float           lead_phas;  //  初相
	int				if_sso;
	RTDB_TIME        sso_bgn;	   //  SSO发生时间
	RTDB_TIME        sso_end;	   //  SSO消失时间
	char            sso_time[20];	//  持续时间
	int             stat_old;      //  上轮状态
	float           max_ampl;  //  最大幅值
	RTDB_TIME         lfo_max_ampl_t;	   //  最大幅值时间
	float           aver_freq1;  //  振荡频率1
	int sso_mon;	/* 是否可观 */
} SSO_GN_MON;
#define LFO_GN_MON_FLD "gn_id,stat,stat_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,is_ang_calc"

///////////////////////////////////////////////////////////////////////////
// 交流线路静态信息
typedef struct tag_SSO_LN_INF
{
  INT_REC         acln_idx;    //  线路索引
	char            acln_name[64];    //  线路名称
  INT_REC         acln_id;       //  线路ID号
	INT_REC         dot_id_1;	    //  交流线段首端
	INT_REC         fac_id_1;	    //  厂站ID号
	INT_REC         area_id_1;	    //  区域ID号
  char            fac_name_1[64];  //  厂站名称
	char            area_name_1[64];	 //  区域名称
	INT_REC         dot_id_2;	    //  交流线段末端
	INT_REC         fac_id_2;	    //  厂站ID号
	INT_REC         area_id_2;	    //  区域ID号
  char            fac_name_2[64];  //  厂站名称
	char            area_name_2[64];	 //  区域名称
	float           imax1;	      //  电流正常限或电流上限(额定电流)
	VOL_INT_REC     vlty_id;	  //  电压类型ID号
	float			vbase;			/* 电压基值 */
} SSO_LN_INF;

// 交流线路控制信息
typedef struct tag_SSO_LN_CTL
{
    INT_REC         acln_id;                                //  线路ID号
	unsigned char   is_monitor;                             //  是否监视
// 	unsigned char   is_part_ana;                            //  是否参与分析
	unsigned char   is_end;                                 //  末端优先
	float			sso_ln_swn;								//	低频振荡开始振幅限值
	float			sso_ln_ewn;								//	低频振荡结束振幅限值
} SSO_LN_CTL;
#define SSO_LN_CTL_FLD "acln_id,is_monitor,is_end,sso_ln_swn,sso_ln_ewn"

// 线路数据（有功+电压相量）
typedef struct tag_SSO_LN_DAT
{
	INT_REC         ln_id;  //  线路ID号
	float           ampl[DATA_NUM_MAX]; //  幅值
	float           freq[DATA_NUM_MAX];  //  频率
	float           pwr[DATA_NUM_MAX];  //  有功
	int				sso[DATA_NUM_MAX]; //  状态
	int             SecSts[MAX_tIMW_WIN]; //  每秒量测状态
	int             stat;                   //  数据质量
    VEC_YC_FCNO     VecBadData[3];                             //  数据不可用文件号  // 2008-07-16 by zhw
}SSO_LN_DAT;

// 交流线路监视信息
typedef struct tag_SSO_LN_MON
{
	INT_REC         ln_id;  //  线路ID号
	INT_REC         fac_id;    //  厂站ID号
	char            ln_name[64];    //  线路名称
	int             stat;      //  状态
	int             stat_val;   //  状态值
	float           lead_ampl; //  振幅
	float           lead_freq;  //  频率
	float           lead_phas;  //  初相
	RTDB_TIME         sso_bgn;	   //  SSO发生时间
	RTDB_TIME         sso_end;	   //  SSO消失时间
	char            sso_time[20];	//  持续时间
	INT_REC             slct_dot_id;                            //  选定线端ID  2008-04-05 added by zhw at yn
	int             stat_old;      //  上轮状态
	int				if_sso;
	float           max_ampl;  //  最大幅值
	RTDB_TIME         lfo_max_ampl_t;	   //  最大幅值时间
	float           aver_freq1;  //  振荡频率1
	int sso_mon;	/* 是否可观 */
} SSO_LN_MON;
#define TAB_LFO_AAST_LN_MON_FLD "acln_id,stat,stat_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,slct_dot_id"
///////////////////////////////////////////////////////////////////////////
// 变压器静态信息
typedef struct tag_SSO_TR_INF
{
	INT_REC         tr_idx;  //  变压器索引
	char            tr_name[64];   //  变压器名称
	INT_REC         tr_id;       //  变压器ID号
	INT_REC         fac_id;    //  厂站ID号
	char            fac_name[64]; //  厂站名称
	INT_REC         area_id;    //  区域ID号
	char            area_name[64]; //  区域名称
	unsigned char   trty;         //  绕组类型
	unsigned char   tr_type;      //  变压器类型
	INT_REC high_wind_id;	/* 高压侧绕组ID号 */
	VOL_INT_REC high_vlty_id;   // 高压侧电压类型ID号
	INT_REC mid_wind_id;	/* 中压侧绕组ID号 */
	VOL_INT_REC mid_vlty_id;   // 中压侧电压类型ID号
	INT_REC low_wind_id;	/* 低压侧绕组ID号 */
	VOL_INT_REC low_vlty_id;   // 低压侧电压类型ID号
	float			vhigh;		/* 高压侧电压基值 */
	float			vmid;		/* 中压侧电压基值 */
	float			vlow;		/* 低压侧电压基值 */
	float			vbase;		/* 电压基值 */
	float			max_pwr;		// 有功上限
} SSO_TR_INF;


// 变压器控制信息
typedef struct tag_SSO_TR_CTL
{
    INT_REC         tr_id;                                  //  变压器ID号
    unsigned char   is_monitor;                             //  是否监视
//     unsigned char   is_part_ana;                            //  是否参与分析
//     unsigned char   mon_wind_slct;                          //  监视绕组
//     unsigned char   is_pow_calc;                            //  功率是否计算值
	float			sso_tr_swn;								//	低频振荡开始振幅限值
	float			sso_tr_ewn;								//	低频振荡结束振幅限值
} SSO_TR_CTL;
#define SSO_TR_CTL_FLD "tr_id,is_monitor,sso_tr_swn,sso_tr_ewn"

// 变压器数据（有功+电压相量）
typedef struct tag_SSO_TR_DAT
{
	INT_REC         tr_id;  //  机组ID号
	float           ampl[DATA_NUM_MAX]; //  幅值
	float           freq[DATA_NUM_MAX];  //  频率
	float           pwr[DATA_NUM_MAX];  //  有功
	int				sso[DATA_NUM_MAX]; //  状态
	int             SecSts[MAX_tIMW_WIN]; //  每秒量测状态
	int             stat;                   //  数据质量
    VEC_YC_FCNO     VecBadData[3];                          //  数据不可用文件号  // 2008-07-16 by zhw
}SSO_TR_DAT;

// 变压器监视信息
typedef struct tag_SSO_TR_MON
{
    INT_REC         tr_id;  //  线路ID号
	INT_REC         fac_id;    //  厂站ID号
	char            tr_name[64];    //  变压器名称
	int             stat;      //  状态
	int             stat_val;   //  状态值
	float           lead_ampl; //  振幅
	float           lead_freq;  //  频率
	float           lead_phas;  //  初相
	RTDB_TIME         sso_bgn;	   //  SSO发生时间
	RTDB_TIME         sso_end;	   //  SSO消失时间
	char            sso_time[20];	//  持续时间
    INT_REC			high_wind_id;							/* 高压侧绕组ID号 */
    INT_REC			mid_wind_id;							/* 中压侧绕组ID号 */
    INT_REC			low_wind_id;							/* 低压侧绕组ID号 */
    INT_REC         slct_wind_id;                           //  选定选定绕组ID号
	int             stat_old;      //  上轮状态
	int				if_sso;
	float           max_ampl;  //  最大幅值
	RTDB_TIME         lfo_max_ampl_t;	   //  最大幅值时间
	float           aver_freq1;  //  振荡频率1
	int sso_mon;	/* 是否可观 */
} SSO_TR_MON;

///////////////////////////////////////////////////////////////////////////
// 厂站静态信息
typedef struct tag_SSO_FC_INF
{
	INT_REC         fac_idx;  //  厂站索引
	INT_REC         fac_id;   //  厂站ID
	char            fac_name[64];   //  厂站名称
	INT_REC         area_id;	      //  区域ID号
	char            area_name[64];	   //  区域名称
	VOL_INT_REC         vlty_id;      //  最高电压等级

} SSO_FC_INF;


// 厂站控制信息
typedef struct tag_SSO_FC_CTL
{
	INT_REC         station_id;                                 //  厂站ID
	unsigned char   is_monitor;                             //  是否监视
	unsigned char   is_part_ana;                            //  是否参与分析
// 	unsigned char   is_set_bus;                             //  是否指定母线
//     INT_REC         set_bus_id;                             //  指定母线ID
// 	unsigned char   is_m_bs;                                //  正母优先
// 	unsigned char   is_set_fyc;                             //  是否指定频率测点
//  int             set_fyc_id;                             //  指定频率测点ID
} SSO_FC_CTL;
#define SSO_FC_CTL_FLD "station_id,is_monitor,is_part_ana"

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
} LFO_FC_DAT;

// 厂站监视信息
typedef struct tag_SSO_FC_MON
{
	INT_REC         fac_id;  //  厂站ID号
	int             stat;      //  状态
	char            fac_name[64];                           //  厂站名称
	int             stat_val;   //  状态值
	RTDB_TIME         sso_bgn;	   //  SSO发生时间
	RTDB_TIME         sso_end;	   //  SSO消失时间
	char            sso_time[20];	//  持续时间
} SSO_FC_MON;
//#define LFO_FC_MON_FLD "stat,v_aver,v_max,v_min,v_ampl,is_cntr,is_disp,clus,slct_bus_id"

// 厂站振荡信息
// typedef struct tag_LFO_FC_LFO
// {
// 	int             stat;                                       //  厂站ID
// 	float           lfo_v_aver;                                 //  电压均值
// 	float           lfo_v_max;                                  //  电压最大值
// 	float           lfo_v_min;                                  //  电压最小值
// 	float           lfo_v_ampl;                                 //  电压摆动
// 	float           lfo_f_aver;                                 //  频率均值
// 	float           lfo_f_max;                                  //  频率最大值
// 	float           lfo_f_min;                                  //  频率最小值
// 	float           lfo_f_ampl;                                 //  频率摆动
// 	unsigned char   lfo_is_cntr;                                //  是否中心
// 	unsigned char   lfo_is_disp;                                //  是否显示
// 	unsigned char   lfo_clus;                                   //  所属群
// } LFO_FC_LFO;
// #define LFO_FC_LFO_FLD "stat,lfo_v_aver,lfo_v_max,lfo_v_min,lfo_v_ampl,lfo_f_aver,lfo_f_max,lfo_f_min,lfo_f_ampl,lfo_is_cntr,lfo_is_disp,lfo_clus"
//////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////

// 系统监视信息
typedef struct  tag_SSO_SYS_MON
{
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	char            stat_ctx[200];	//  振荡案例描述
	float			mon_ampl;
	float			mon_freq;
	RTDB_TIME         sso_bgn;	   //  SSO发生时间
	RTDB_TIME         sso_end;	   //  SSO消失时间
	char            sso_time[20];	//  持续时间
	INT_REC         warn_dev_id; //  触发告警设备ID
	char            warn_dev_name[64]; //  触发告警设备
	float           warn_freq;	       //  触发告警频率
	int             sso_ln_num;	   //  SSO线路数
	int             sso_gn_num;	   //  SSO发电机数
	int             sso_tr_num;     //  LFO变压器数

	int             if_sso;	   //  是否出现振荡
	RTDB_TIME			start_time;//长录波文件召唤开始时间
	RTDB_TIME			end_time;//长录波文件召唤结束时间
} SSO_SYS_MON;

// 系统振荡信息
typedef struct  tag_SSO_SYS_SSO
{
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	char            stat_ctx[200];	//  振荡案例描述
	int warn_gn_num;	/* 告警机组数 */
	int warn_ln_num;	/* 告警线路数 */
	int warn_tr_num;	/* 告警变压器数 */
	float lead_ampl;	/* 振幅 */
	float			lead_freq;     // 频率
	RTDB_TIME         sso_bgn;	   //  SSO发生时间
	RTDB_TIME         sso_end;	   //  SSO消失时间
	char            sso_time[20];	//  持续时间
	INT_REC         warn_dev_id; //  触发告警设备ID
	char            warn_dev_name[64]; //  触发告警设备
	float           warn_freq;	       //  触发告警频率
	int             sso_ln_num;	   //  SSO线路数
	int             sso_gn_num;	   //  SSO发电机数
	int             sso_tr_num;     //  LFO变压器数
	int is_sso;	/* 是否振荡 */
} SSO_SYS_SSO;
#define SSO_SYS_MON_FLD "stat,stat_val,stat_ctx,warn_gn_num,warn_ln_num,warn_tr_num,lead_ampl,lead_freq,sso_bgn,sso_end,sso_time,warn_dev_id,warn_dev_name,warn_freq,sso_ln_num,sso_gn_num,sso_tr_num,is_sso"

// 系统振荡信息
typedef struct  tag_LFO_SYS_LFO
{
//	int             if_lfo;	                                //  是否出现振荡  /*2007-11-30 注释*/
	char            case_des[128];	                        //  振荡案例描述
	RTDB_TIME         lfo_bgn;	                            //  LFO发生时间
	RTDB_TIME         lfo_end;	                            //  LFO消失时间
	char            lfo_time[20];	                        //  持续时间
	INT_REC         warn_dev_id;	                        //  触发告警设备ID
	char            warn_dev_name[64];	                    //  触发告警设备
	float           warn_freq;	                            //  触发告警频率
	float           aver_freq;	                            //  主导平均频率
	int             lfo_ln_num;	                            //  LFO线路数
	int             lfo_gn_num;	                            //  LFO发电机数
    int             lfo_tr_num;                             //  LFO变压器数
	//int             center_id;	                            //  振荡中心
	//char            center_desc[128];	                    //  振荡中心描述
    //int his_bgn;	/* 查询起始时刻 */  // 2008-10-13 by zhw 离线研究时刻为最近一次低频振荡时刻
	//int his_end;	/* 查询截止时刻 */
#ifdef _ADDFACID_
	INT_REC warn_dev_facid;  // 触发告警设备STID
#endif
	float aver_freq2;	/* 主导平均频率2 */
} LFO_SYS_LFO;
#ifdef _ADDFACID_
#define LFO_SYS_LFO_FLD "case_des,lfo_bgn,lfo_end,lfo_time,warn_dev_id,warn_dev_name,warn_freq,aver_freq,lfo_ln_num,lfo_gn_num,lfo_tr_num,warn_dev_facid,aver_freq2"
#else
#define LFO_SYS_LFO_FLD "case_des,lfo_bgn,lfo_end,lfo_time,warn_dev_id,warn_dev_name,warn_freq,aver_freq,lfo_ln_num,lfo_gn_num,lfo_tr_num,aver_freq2"
#endif

// 设备监视信息
typedef struct tag_SSO_DEV_MON
{
    INT_REC         dev_id;  //  设备ID号
	INT_REC         fac_id;    //  厂站ID号
	int				dev_type; // 设备类型
	int             stat;      //  状态
	int             stat_val;   //  状态值
	float           lead_ampl; //  振幅
	float           lead_freq;  //  频率
	float           lead_phas;  //  初相
	RTDB_TIME         sso_bgn;	   //  SSO发生时间
	RTDB_TIME         sso_end;	   //  SSO消失时间
	char            sso_time[20];	//  持续时间
	char			dev_name[64];
	char			fac_name[64];
	int             stat_old;      //  上轮状态
} SSO_DEV_MON;


// 系统分析信息
typedef struct  tag_LFO_SYS_ANA
{
	RTDB_TIME         wds_bgn;	                            //  起始时刻
	RTDB_TIME         wds_end;	                            //  截至时刻
	int             mode_num;	                            //  模式数
	INT_REC         dom_gn_id;	                            //  重要机组ID
	INT_REC         dom_ln_id;	                            //  重要线路ID
	float           lead_ampl;	                            //  振幅
	float           lead_freq;	                            //  频率
	float           lead_damp;	                            //  阻尼比
	float           lead_phas;	                            //  初相
	float           lead_real;	                            //  实部
	float           lead_imag;	                            //  虚部
	float           lead_engy;	                            //  能量
	float           lead_fail;	                            //  衰减
	int             show_gn_num;	                        //  参与机组数
	int             show_ln_num;	                        //  振荡线路数       
    INT_REC         center_id;	                            //  振荡中心
	char            center_desc[128];	                    //  振荡中心描述
    char            ana_detail[255];                        //  分析信息
} LFO_SYS_ANA;
#define LFO_SYS_ANA_FLD "wds_bgn,wds_end,mode_num,dom_gn_id,dom_ln_id,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,show_gn_num,show_ln_num,center_id,center_desc,ana_detail"

///////////////////////////////////////////////////////////////////////////
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

// Prony分析结果	
typedef struct tag_PRONY_ANAL_RSLT
{
 	INT_REC  obj_id;               //监视对象ID号(临界群ID号为负)
	char     obj_name[64];        //监视对象名称(200701)
    int      mode_num;             //振荡模式个数
    float    mode[MODE_NUM_MAX][8];//振荡模式   (增加预测幅值)
}PRONY_ANAL_RSLT;

// LFO振荡信息统计
struct THIS_LFO_INFO
{
	int      start_time;
	int      end_time;
	INT_REC  obj_id;               // 临界群为 -1        
	char     obj_name[64];
	INT_REC  fac_id;
	char     fac_name[64];
	float    frequency;
	float    ampl_max;
	float    ampl_min;
	char     hold_time[20];
};

///////////////////////////////////////////////////////////////////////////
// 参考角选取数据
typedef struct tag_LFO_REF_ANG_DAT
{
    INT_REC         dev_id;                                 //  线路ID号
 	float           time[REF_ANG_MAX_PNT];                  //  时刻
 	float           ang1[REF_ANG_MAX_PNT];                  //  相角1
    float           ang2[REF_ANG_MAX_PNT];                  //  相角2
	int             SecSts[MAX_tIMW_WIN];                   //  每秒量测状态
	int             stat;                                   //  计算数据状态(正常才可用)
} LFO_REF_ANG_DAT;

//召唤长录波文件结构
// typedef struct tag_FAD_CALLUP_COMTR_NAME_STRU 
// {
// 	vector<long>   	fac_seq; //被请求文件的通讯厂站id
// 	short fac_num; //被请求文件的通讯厂站数目，等于fac_seq.size()
// 	unsigned long  	start_time; //请求开始时间
// 	unsigned long  	end_time; //请求结束时间
// 	string  src_host_name; //发送请求消息的程序所在机器名称
// 	string  src_proc_name;// 发送请求消息的程序名称
// } FAD_CALLUP_COMTR_NAME_STRU;

// typedef struct tag_FAD_TRANS_REPORT_STRUCT 
// {
// 	long fac_id; //录波文件所属通讯厂站id
// 	short type; //1-connect fes  2-connect pmu 3-in trans
// 	// 4-suceed  5- failed
// 	string filename; //录波文件名称
// 	int total_frame; //录波文件总包数
// 	int current_frame; //录波文件已接收包数
// } FAD_TRANS_REPORT_STRUCT;

// 振荡模式
////  mode participation object
//typedef struct  lfo_MODE_PART_OBJ
//{
//	int               obj_id;                               //设备ID
//	//	char              obj_name[64];                         //设备名称
//	unsigned char     type;                                 //类型(OBJ_TYPE_GN; OBJ_TYPE_LN)
//	float             damp;                                 //阻尼比
//	float             freq;                                 //频率
//	float             ampl;                                 //振幅
//	float             phas;                                 //初相
//	float             mgnt;                                 //量级
//	float             real;                                 //实部  // 
//	float             imag;                                 //虚部
//	float             fact;                                 //参与因子
//	float             engy;                                 //能量
//}MODE_PART_OBJ;
//
////  mode characteristics
//typedef struct  lfo_MODE_CHRCT
//{
//	int               mode_id;                              //模式ID
//	unsigned char     type;                                 //类型(=0 local; =1 interarea)
//	
//	float             real;                                 //实部(aver)
//	float             imag;                                 //虚部(aver)
//	float             freq;                                 //频率(aver)
//	float             damp;                                 //阻尼比(aver)
//
//	float             engyGn;                               //机组能量(aver) // 2007-08-10 lsf add
//	float             engyLn;                               //线路能量(aver) // 2007-08-10 lsf add
//	
//	int               nPartGn;                              //参与机组数
//	MODE_PART_OBJ     PartGn[MODE_MAXGN];                   //机组模态
//	int               nPartLn;                              //可观线路数
//	MODE_PART_OBJ     PartLn[MODE_MAXLN];                   //线路模态
//	
//}MODE_CHRCT;
//
// 模态
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
///////////////////////////////////////////////////////////////////////////

// 辅助控制措施
	// 地理图显示振荡中心、参与厂站、振荡性质（区内区外）
	// 根据发电机参与因子，调整出力
	// 根据振荡中心线路，  调整运行方式
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////// 并行计算 //////////////////////////////
///////////////////////////  CALCULATION STRUCT  ////////////////////////
// curve data
typedef struct  tag_CURVE_DATA
{
	float           t_GPS;                                  //  厂站ID
	int             m_nPoints;
	float           t_length;
	float           y_value[DATA_NUM_MAX];
	float           t_value[DATA_NUM_MAX];
}CURVE_DATA;
// mode result
typedef struct  tag_MODE_RESULT
{
	float            t_GPS;
	int		         mode_num;
	float	         mode[MODE_NUM_MAX][8];       
	bool	         modeStatus[MODE_NUM_MAX];
}MODE_RESULT;
// case result
typedef struct  tag_CASE_TYPE
{
	INT_REC     m_nID;
	int         m_nStatus;    // 0 未处理 1 正在处理中 2 处理完毕
	int         m_time;       // 处理的时间长度,毫秒
	CURVE_DATA  m_curve;      // 曲线数据
	MODE_RESULT m_result;     // 计算结果
	int         m_nClusterID;
	struct timeb timeStart;  // 开始计算时刻
	struct timeb timeEnd;    // 结束计算时刻
	
}CASE_TYPE;

///////////////////////////////////////////////////////////////////////////
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

// save result
//振荡过程系统整体结果
typedef struct  type_lfo_sys_res_save
{
	time_t                  lfo_b_t;                //开始时刻
	time_t                  lfo_e_t;                //结束时刻
	int                     hold_time;              //持续时间
	float                   freq;                   //触发告警振荡频率
	float                   aver_freq;              //平均频率
	INT_REC                 obj_id;                 //触发元件
	char                    obj_name[64];           //设备名称
	int                     obj_type;               //触发元件类型（线路、发电机、机群）
	INT_REC                 fac_id;                 //厂站ID
	char                    fac_name[64];           //厂站名称
//	ACT_GEN_INFO	        obj_ActGen;             //详细信息（发电机、机群）
    int                     lfo_line_num;	        //振荡线路数
    int                     act_gen_num;	        //参与发电机数
    int                     lfo_tr_num;             //振荡变压器数
    char                    lfo_case_des[128];	    //振荡案例描述
    float                   max_ampl;               //振荡最大幅值
//	ACT_GEN_INFO	        ActGen;                 //机组及参与因子
	int                     mode1_num;
	PERIOD_LEAD_MODE        mode1[Period_Mode_Max]; //主导模式1
// 	int                     mode2_num;
// 	PERIOD_LEAD_MODE        mode2[Period_Mode_Max]; //主导模式2
// 	int                     mode3_num;
// 	PERIOD_LEAD_MODE        mode3[Period_Mode_Max]; //主导模式3
}LFO_SYS_RES_SAVE;

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

// 计算窗口
typedef struct tag_LFO_CALC_WDS
{
	INT_REC wds_id;
	int wds_no;	                                            //  窗口序号
	RTDB_TIME wds_bgn;                                            //  起始时刻
	RTDB_TIME wds_end;                                            //  截至时刻
	int mode_num;	                                        //  模式数
	float real1;	                                        //  实部1
	float imag1;	                                        //  虚部1
	float freq1;	                                        //  频率1
	float damp1;	                                        //  阻尼比1
	float real2;	                                        //  实部2
	float imag2;	                                        //  虚部2
	float freq2;	                                        //  频率2
	float damp2;	                                        //  阻尼比2
	float real3;	                                        //  实部3
	float imag3;	                                        //  虚部3
	float freq3;	                                        //  频率3
	float damp3;	                                        //  阻尼比3
	int is_mode_show;	                                    //  模式是否显示
	RTDB_TIME occur_time;	                                        //  发生时间
	char content[255];	                                    //  详细信息描述
	int is_ctx_show;	                                    //  信息是否显示
    char center_desc[128];                                  //  振荡中心描述
}LFO_CALC_WDS;

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

#define TAB_LFO_RT_CURVE_FLD "dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,max_val,min_val,ave_val,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125," \
"seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"
typedef struct tab_lfo_rt_curve1
{
	//INT_REC dev_id;	/* 曲线ID */
	char dev_name[128];	/* 曲线名称 */
	char unit[8];	/* 变量单位 */
	int sam_freq;	/* 采样频率 */
	RTDB_TIME cuv_bgn_sec;	/* 起始时刻 */
	RTDB_TIME cuv_end_sec;	/* 结束时刻 */
	int cuv_sec;	/* 时间 */
	int cuv_pnt;	/* 点数 */
	float cuv_stp;	/* 步长 */
// 	float max_val;	/* 最大值 */
// 	float min_val;	/* 最小值 */
// 	float ave_val;	/* 平均值 */
	float seq_cuv_data[125];	/* seq_cuv_data数组 */
}TAB_LFO_RT_CURVE1;
#define TAB_LFO_RT_CURVE1_FLD "dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20,s21,s22,s23,s24,s25,s26,s27,s28,s29,s30,s31,s32,s33,s34,s35,s36,s37,s38,s39,s40,s41,s42,s43,s44,s45,s46,s47,s48,s49,s50,s51,s52,s53,s54,s55,s56,s57,s58,s59,s60,s61,s62,s63,s64,s65,s66,s67,s68,s69,s70,s71,s72,s73,s74,s75,s76,s77,s78,s79,s80,s81,s82,s83,s84,s85,s86,s87,s88,s89,s90,s91,s92,s93,s94,s95,s96,s97,s98,s99,s100,s101,s102,s103,s104,s105,s106,s107,s108,s109,s110,s111,s112,s113,s114,s115,s116,s117,s118,s119,s120,s121,s122,s123,s124,s125"
typedef struct tab_lfo_rt_curve2
{
	float seq_cuv_data[125];	/* seq_cuv_data数组 */
}TAB_LFO_RT_CURVE2;
#define TAB_LFO_RT_CURVE2_FLD "s126,s127,s128,s129,s130,s131,s132,s133,s134,s135,s136,s137,s138,s139,s140,s141,s142,s143,s144,s145,s146,s147,s148,s149,s150,s151,s152,s153,s154,s155,s156,s157,s158,s159,s160,s161,s162,s163,s164,s165,s166,s167,s168,s169,s170,s171,s172,s173,s174,s175,s176,s177,s178,s179,s180,s181,s182,s183,s184,s185,s186,s187,s188,s189,s190,s191,s192,s193,s194,s195,s196,s197,s198,s199,s200,s201,s202,s203,s204,s205,s206,s207,s208,s209,s210,s211,s212,s213,s214,s215,s216,s217,s218,s219,s220,s221,s222,s223,s224,s225,s226,s227,s228,s229,s230,s231,s232,s233,s234,s235,s236,s237,s238,s239,s240,s241,s242,s243,s244,s245,s246,s247,s248,s249,s250"

typedef struct tab_sso_rt_curve_id
{
	INT_REC dev_idx;	/* 设备索引ID */
	INT_REC dev_id;	/* 曲线ID */
}TAB_SSO_RT_CURVE_ID;
#define TAB_SSO_RT_CURVE_ID_FLD "dev_idx,dev_id"

/* SSO实时曲线横向表 */
#ifdef _NUSP
typedef struct tab_SSO_rt_curve2
{
	INT_REC recordid;	/* 记录ID */
	RTDB_TIME time_point;	/* 时间 */
	float time_view;	/* 显示时间 */
	int time_sec;	/* 整秒时间 */
	char time_des[8];	/* 时间描述 */
	char gn_name[64];	/* 发电机名称 */
	float gn_power;	/* 发电机有功曲线 */
	float gn_ssp_val;	/* 发电机谐波幅值 */
	float gn_ssf_val;	/* 发电机谐波频率 */
	char ln_name[64];	/* 交流线路名称 */
	float ln_power;	/* 交流线路有功曲线 */
	float ln_ssp_val;	/* 交流线路谐波幅值 */
	float ln_ssf_val;	/* 交流线路谐波频率 */
	char tr_name[64];	/* 变压器名称 */
	float tr_power;	/* 变压器有功曲线 */
	float tr_ssp_val;	/* 变压器谐波幅值 */
	float tr_ssf_val;	/* 变压器谐波频率 */
	float lead_freq;	/* 主导模式频率曲线 */
	float lead_damp;	/* 主导模式阻尼比曲线 */
}TAB_SSO_RT_CURVE2;
#define TAB_SSO_RT_CURVE2_FLD "recordid,time_point,time_view,time_sec,time_des,gn_name,gn_power,gn_ssp_val,gn_ssf_val,ln_name,ln_power,ln_ssp_val,ln_ssf_val,tr_name,tr_power,tr_ssp_val,tr_ssf_val,lead_freq,lead_damp"
#endif // _NUSP

/* LFO设备振荡曲线表 */ //20141008
typedef struct tab_warn_dev_curve
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
//	float max_val;	/* 最大值 */
//	float min_val;	/* 最小值 */
//	float ave_val;	/* 平均值 */
	float seq_cuv_data[250];	/* seq_cuv_data数组 */
}TAB_WARN_DEV_CURVE;
#define TAB_WARN_DEV_CURVE_FLD "dev_id,dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125," \
"seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"
typedef struct tab_warn_dev_curve1
{
	INT_REC dev_id;	/* 曲线ID */
	char dev_name[128];	/* 曲线名称 */
	char unit[8];	/* 变量单位 */
	int sam_freq;	/* 采样频率 */
	RTDB_TIME cuv_bgn_sec;	/* 起始时刻 */
	RTDB_TIME cuv_end_sec;	/* 结束时刻 */
	int cuv_sec;	/* 时间 */
	int cuv_pnt;	/* 点数 */
	float cuv_stp;	/* 步长 */
// 	float max_val;	/* 最大值 */
// 	float min_val;	/* 最小值 */
// 	float ave_val;	/* 平均值 */
	float seq_cuv_data[125];	/* seq_cuv_data数组 */
}TAB_WARN_DEV_CURVE1;
#define TAB_WARN_DEV_CURVE1_FLD "dev_id,dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125"
typedef struct tab_warn_dev_curve2
{
	float seq_cuv_data[125];	/* seq_cuv_data数组 */
}TAB_WARN_DEV_CURVE2;
#define TAB_WARN_DEV_CURVE2_FLD "seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"


/* LFO断面定义表 */
// typedef struct tab_LFO_INTERFACE
// {
// 	int   itfc_id;	/* 断面ID号 */
// 	char  itfc_name[64];	/* 断面名称 */
// 	
// 	unsigned char is_monitor;	/* 是否监视 */
// 	int sam_freq;	/* 采样频率 */
// 	int cuv_bgn_sec;	/* 起始时刻 */
// 	int cuv_end_sec;	/* 结束时刻 */
// 	int cuv_sec;	/* 时间 */
// 	int cuv_pnt;	/* 点数 */
// 	float cuv_stp;	/* 步长 */
// 	float aver_val;	/* 均值 */
// 	float max_val;	/* 最大值 */
// 	int max_val_t;	/* 最大值时间 */
// 	float min_val;	/* 最小值 */
// 	int min_val_t;	/* 最小值时间 */
// 	float max_min_dif;	/* 最大摆动 */
// 	float seq_cuv_data[250];	/* seq_cuv_data数组 */
// }LFO_INTERFACE;
// #define TAB_LFO_INTERFACE_DEF_FLD "sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125," \
// "seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"

/* LFO断面组成表 */
// typedef struct tab_LFO_INTERFACE_DEV
// {
// 	int dev_idx;	/* 设备索引 */
// 	int itfc_id;	/* 断面ID号 */
// 	int dev_id;	/* 设备ID */  //(引用交流线路表)
// 	int dev_type;	/* 设备类型 */
// 	int is_valid;	/* 是否有效 */
// 	int is_invert;	/* 是否方向取反 */
// 	int stat;	/* 状态 */
// 	int stat_val;	/* 状态值 */
// }LFO_INTERFACE_DEV;

typedef struct type_SSO_FC_TAB_CRE
{
    INT_REC         fac_idx;                                //  厂站索引
	INT_REC         station_id;                                 //  厂站ID
	INT_REC         area_id;                                //  区域ID号
	unsigned char   fac_type;                               //  厂站类型
	VOL_INT_REC     vlty_id;                                //  最高电压等级
	
} SSO_FC_TAB_CRE;
#define SSO_FC_TAB_CRE_FLD "fac_idx,station_id,area_id,fac_type,vlty_id"
// SSO发电机表
typedef struct type_SSO_GN_TAB_CRE
{
    INT_REC             gn_idx;                                 //  机组索引
    INT_REC             gn_id;                                  //  机组ID号
    INT_REC             station_id;                                 //  厂站ID号
	INT_REC             area_id;	                            //  区域ID号
	unsigned char       is_monitor;	                        //  计算功角优先
} SSO_GN_TAB_CRE;
#define SSO_GN_TAB_CRE_FLD "gn_idx,gn_id,station_id,area_id,is_monitor"
// SSO交流线路表
typedef struct type_SSO_LN_TAB_CRE
{
    INT_REC             acln_idx;                               //  线路索引
    INT_REC             acln_id;                                //  线路ID号
	INT_REC             dot_id_1;	                            //  交流线段首端
	INT_REC             fac_id_1;	                            //  厂站ID号
	INT_REC             area_id_1;	                            //  区域ID号
	INT_REC             dot_id_2;	                            //  交流线段末端
	INT_REC             fac_id_2;	                            //  厂站ID号
	INT_REC             area_id_2;	                            //  区域ID号
	unsigned char       is_end;		                            //  末端优先
} SSO_LN_TAB_CRE;
#define SSO_LN_TAB_CRE_FLD "acln_idx,acln_id,dot_id_1,fac_id_1,area_id_1,dot_id_2,fac_id_2,area_id_2,is_end"

// SSO变压器表
typedef struct type_SSO_TR_TAB_CRE
{
    INT_REC tr_idx;  // 变压器索引
    INT_REC tr_id;	/* 变压器ID号 */
    INT_REC station_id;	/* 厂站ID号 */
    INT_REC area_id; // 区域ID号
	
    unsigned char trty;	/* 绕组类型 */ // 三绕组变、两绕组变
	unsigned char tr_type;	/* 变压器类型 */ // 主变、启动变、厂用变
	INT_REC high_wind_id;	/* 高压侧绕组ID号 */
	VOL_INT_REC high_vlty_id;   // 高压侧电压类型ID号	
	INT_REC mid_wind_id;	/* 中压侧绕组ID号 */
	VOL_INT_REC mid_vlty_id;   // 中压侧电压类型ID号	
	INT_REC low_wind_id;	/* 低压侧绕组ID号 */
	VOL_INT_REC low_vlty_id;   // 低压侧电压类型ID号
	INT_REC slct_wind_id;	/* 选定绕组ID号 */
//    unsigned char is_pow_calc;	/* 功率是否计算值 */
    unsigned char is_monitor;	/* 是否监视 */
//    unsigned char is_part_ana;	/* 是否参与分析 */
//	unsigned char mon_wind_slct;	/* 监视绕组 */
	
} SSO_TR_TAB_CRE;
#define SSO_TR_TAB_CRE_FLD "tr_idx,tr_id,station_id,area_id,trty,tr_type,high_wind_id,high_vlty_id,mid_wind_id,mid_vlty_id,low_wind_id,low_vlty_id,slct_wind_id,is_monitor"

// LFO振荡事件表
typedef struct type_LFO_LPO_EVE_CRE
{
	INT_REC record_id;	/* 记录ID */
	INT_REC eve_id;  // 事件ID（开始振荡时刻）
	INT_REC win_id;  // 窗口号（当前时刻）
	RTDB_TIME analy_bgn;  // 分析起始时间
	RTDB_TIME analy_end;  // 分析截止时间
	float freq_max;  // 频率上限
	float freq_min;  // 频率下限
	int stat;  // 系统状态
	int show_gn_num;  // 振荡发电机个数
	int show_tr_num;  // 振荡变压器个数
	int show_ln_num;  // 振荡线路个数
	float freq;  // 主导模式频率
	float ampl;  // 振幅
	float damp;  // 阻尼比
	int   dev_num;  // 参与设备个数
	unsigned char   is_show;  // 是否显示
	char case_des[128];  // 振荡类型描述
	RTDB_TIME ana_his_bgn;  // 离线分析起始时间
	RTDB_TIME ana_his_end;  // 离线分析截止时间
	INT_REC warn_dev_id;	// 触发告警设备ID
	char warn_dev_name[64];  // 触发告警设备名
	char lfo_time[64];  // 持续时间
}LFO_LPO_EVE_CRE;
#define LFO_LPO_EVE_CRE_FLD "eve_id,win_id,analy_bgn,analy_end,freq_max,freq_min,stat,show_gn_num,show_tr_num,show_ln_num,freq,ampl,damp,dev_num,is_show,case_des,ana_his_bgn,ana_his_end,warn_dev_id,warn_dev_name,lfo_time"
#define LFO_LPO_EVE_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT",to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),%f,%f,%d,%d,%d,%d,%f,%f,%f,%d,%d,'%s',"INT_REC_FMT",'%s','%s')"
//#define LFO_LPO_EVE_CRE_VAL(val) val.eve_id,val.win_id,val.analy_bgn,val.analy_end,val.ana_his_bgn,val.ana_his_end,val.freq_max,val.freq_min,val.stat,val.show_gn_num,val.show_tr_num,val.show_ln_num,val.freq,val.ampl,val.damp,val.dev_num,val.is_show,val.case_des,val.warn_dev_id,val.warn_dev_name,val.lfo_time
#define LFO_LPO_EVE_CRE_VAL(val) val.freq_max,val.freq_min,val.stat,val.show_gn_num,val.show_tr_num,val.show_ln_num,val.freq,val.ampl,val.damp,val.dev_num,val.is_show,val.case_des,val.warn_dev_id,val.warn_dev_name,val.lfo_time

/* LFO实时CASE窗口表 */
typedef struct type_LFO_CASE_WDS_CRE
{
	INT_REC wds_id;	/* 窗口ID */
	INT_REC eve_id;	/* 事件ID */
	INT_REC win_id;	/* 窗口号 */
	RTDB_TIME analy_bgn;	/* 分析起始时间 */
	RTDB_TIME analy_end;	/* 分析截止时间 */
	float rphas;  /* 初相 */
	INT_REC lfodev_id;	/* 振荡设备ID */
	INT_REC fac_id;	/* 厂站ID */
	int dev_type;	/* 设备类型 */
	float freq;	/* 频率 */
	float ampl;	/* 振幅 */
	float damp;	/* 阻尼比 */
	float aver_val;	/* 平均值 */
	float max_ampl_rat;	/* 功率波动值 */
	char lfo_time[64];  /* 持续时间 */
	float lfo_max_val;  /* LFO最大值 */
	RTDB_TIME lfo_max_val_t;  /* LFO最大值时间 */
	float lfo_min_val;  /* LFO最小值 */
	RTDB_TIME lfo_min_val_t;  /* LFO最小值时间 */
}LFO_CASE_WDS_CRE;

#define LFO_CASE_WDS_CRE_FLD "eve_id,win_id,analy_bgn,analy_end,rphas,lfodev_id,fac_id,dev_type,freq,ampl,damp,aver_val,max_ampl_rat,lfo_time,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t"
#define LFO_CASE_WDS_CRE_FMT "INSERT %s (%s) VALUES (to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),%f,"INT_REC_FMT","INT_REC_FMT",%d,%f,%f,%f,%f,%f,'%s',%f,to_date('%s','YYYY-MM-DD HH24:MI:SS'),%f,to_date('%s','YYYY-MM-DD HH24:MI:SS'))"
//#define LFO_CASE_WDS_CRE_VAL(val) val.eve_id,val.win_id,val.analy_bgn,val.analy_end,val.rphas,val.lfodev_id,val.fac_id,val.dev_type,val.freq,val.ampl,val.damp,val.aver_val,val.max_ampl,val.lfo_time,val.lfo_max_val,val.lfo_max_val_t,val.lfo_min_val,val.lfo_min_val_t
#define LFO_CASE_WDS_CRE_VAL(val) val.rphas,val.lfodev_id,val.fac_id,val.dev_type,val.freq,val.ampl,val.damp,val.aver_val,val.max_ampl,val.lfo_time,val.lfo_max_val


/* LFO实时CASE模态全表 */
typedef struct type_LFO_CASE_ALLSHAPE_CRE
{
	INT_REC record_id;	/* 记录号 */
	float fact;	/* 参与因子 */
	INT_REC dev_id;	/* 设备ID */
	char dev_name[64];	/* 设备名称 */
	int dev_type;	/* 设备类型 */
	INT_REC fac_id;	/* 所属厂站 */
	INT_REC area_id;	/* 所属区域 */
	unsigned char clus;	/* 所属群 */
	float freq;	/* 频率 */
	float damp;	/* 阻尼比 */
	float ampl;	/* 振幅 */
	float engy;	/* 能量 */
	float aver_val;	/* 平均值 */
	float max_ampl_rat;	/* 最大波动率 */
	float freq_max;	/* 频率上限 */
	float freq_min;	/* 频率下限 */
	INT_REC eve_id;	/* 事件ID */
	INT_REC win_id;	/* 窗口号 */
	INT_REC analy_bgn;	/* 分析起始时间 */
	INT_REC analy_end;	/* 分析截止时间 */
	float iniphas;	/* 初相 */
}LFO_CASE_ALLSHAPE_CRE;

#define LFO_CASE_ALLSHAPE_CRE_FLD "fact,dev_id,dev_name,dev_type,fac_id,area_id,clus,freq,damp,ampl,engy,aver_val,max_ampl_rat,freq_max,freq_min,eve_id,win_id,analy_bgn,analy_end,iniphas"
#define LFO_CASE_ALLSHAPE_CRE_FMT "INSERT %s (%s) VALUES (%f,"INT_REC_FMT",'%s',%d,"INT_REC_FMT","INT_REC_FMT",%d,%f,%f,%f,%f,%f,%f,%f,%f,"INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%f)"
#define LFO_CASE_ALLSHAPE_CRE_VAL(val) val.fact,val.dev_id,val.dev_name,val.dev_type,val.fac_id,val.area_id,val.clus,val.freq,val.damp,val.ampl,val.engy,val.aver_val,val.max_ampl_rat,val.freq_max,val.freq_min,val.eve_id,val.win_id,val.analy_bgn,val.analy_end,val.iniphas


typedef struct type_LFO_CASE_FILE_DEV
{
	INT_REC fac_id;  // 厂站ID
	INT_REC dev_id;  // 设备ID(端点)
	INT_REC dev_id1;  // 设备ID
	int column_id;  // 域ID
#ifndef _D5000
	KEY_ID_STRU yc_id;
#else
	long yc_id;  // 遥测ID
#endif
	int file_no;  // 文件号
	int col_no;  // 列号
	int is_warn;  // 是否振荡设备
}LFO_CASE_FILE_DEV;

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
	vector<LFO_CASE_FILE_DEV> vecdev;
}LFO_CASE_FILE_STRUCT;

//////////////////////////////////////////////////////////////////////////
#ifndef LOW_FRE_OSCI_INFO_TO_IA
#define LOW_FRE_OSCI_INFO_TO_IA 504
#endif

// 振荡设备信息
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

typedef struct type_LOW_FRE_OSCI_INFO_TO_IA_STRU
{
	INT_REC search_id;  // 振荡中心(设备ID)
	time_t occur_time;  // 故障时间
	short msec;  // 毫秒
	long end_time;  // 结束时间
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

typedef struct type_Gn_warn_time
{
	time_t m_GnStartTime; 
	time_t m_GnEndTime; 
}TYPE_GN_WARN_TIME;
typedef struct type_ln_warn_time
{
	time_t m_LnStartTime; 
	time_t m_LnEndTime; 
}TYPE_LN_WARN_TIME;
typedef struct type_Tr_warn_time
{
	time_t m_TrStartTime; 
	time_t m_TrEndTime; 
}TYPE_TR_WARN_TIME;

#endif
