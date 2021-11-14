#ifndef _WAMS_LFO_ANA_ITFC_H_
#define _WAMS_LFO_ANA_ITFC_H_
////  os : #ifndef _WINDOWS32 _SUN _ALPHA

#ifndef _WINDOWS32
	#include <sys/time.h>
	#include <sys/timeb.h>
#endif
//#include  "WamRtCurveDisp.h"

const int  WAMAP_LFO_TMD_CHANNEL = 5680 ;  // channel_id
/******************************************************************************/

/*2007-11-19  grm add */

/******************************************************************************/
///////////////////////////////////参数表结构定义////////////////////////
typedef struct  tab_PARA_PROC_CTL
{
	int mon_ctl;            /* 监视运行控制 */
	unsigned char mon_sts;	/* 监视运行状态 */
	char mon_use_time[20];	/* 监视计算耗时 */
	char mon_run_ctx[64];	/* 监视运行信息 */

}TAB_PARA_PROC_CTL;

#define TAB_PARA_PROC_CTL_FLD  "mon_ctl,mon_sts,mon_use_time,mon_run_ctx"

typedef struct  tab_PARA_SSO_ANA
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
#ifdef _D5000
	float			volt_thre1;								/* 电压等级1 */
	float			volt_thre2;								/* 电压等级2 */
//广东项目增加
	float			gn_swn_ampl;							//	机组开始振荡振幅限值
	float			gn_ewn_ampl;							//	机组结束振荡振幅限值
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
#endif
}TAB_PARA_SSO_ANA;
#ifdef _D5000
#define TAB_PARA_SSO_ANA_FLD "mem_max_sec,smp_freq,flt_len,flt_coe,time_leng,int_time,freq_min,freq_max,sso_jud_num,"\
"volt_thre1,volt_thre2,gn_swn_ampl,gn_ewn_ampl,ln_swn_ampl1,ln_swn_ampl2,ln_swn_ampl3,ln_ewn_ampl1,ln_ewn_ampl2,ln_ewn_ampl3,tr_swn_ampl1,tr_swn_ampl2,tr_swn_ampl3,tr_ewn_ampl1,tr_ewn_ampl2,tr_ewn_ampl3"
#else
#define TAB_PARA_SSO_ANA_FLD "mem_max_sec,smp_freq,flt_len,flt_coe,time_leng,int_time,freq_min,freq_max,sso_jud_num"
#endif

// 输出控制
typedef struct  tab_PARA_DISP_CTL
{
	int             dsource;	                            //  数据源
	float           sso_tim_freq;	                        //  时段区分频差
// 	int             show_max;	                            //  最大允许显示
	INT_REC         sso_fac_id;	                            //  对应虚拟厂站ID
	unsigned char   is_warn;	                            //  是否发送告警
	unsigned char   is_samp;	                            //  是否触发采样
}TAB_PARA_DISP_CTL;
#define TAB_PARA_DISP_CTL_FLD "dsource,sso_tim_freq,sso_fac_id,is_warn,is_samp"

///////////////////////////////////结果表结构定义////////////////////////
// 交流线路振荡信息
typedef struct  tag_LFO_LN_LFO
{
	int             if_lfo;	                                //  是否出现振荡
	float           lfo_bgn_val;	                        //  LFO前均值
	float           lfo_end_val;	                        //  LFO后均值
	RTDB_TIME       slfo_bgn_t;	                            //  自身LFO发生时间
	RTDB_TIME       slfo_end_t;	                            //  自身LFO消失时间
	char            slfo_time[20];	                        //  持续时间
	float           slfo_bgn_val;	                        //  自身LFO前均值
	float           slfo_end_val;	                        //  自身LFO后均值
	float           lfo_max_val;	                        //  LFO最大值
	RTDB_TIME       lfo_max_val_t;	                        //  LFO最大值时间
	float           lfo_min_val;	                        //  LFO最小值
	RTDB_TIME       lfo_min_val_t;	                        //  LFO最小值时间
	float           lfo_max_min_dif;	                    //  LFO最大差值
	float           aver_freq1;	                            //  振荡频率1
	float           aver_freq2;	                            //  振荡频率2
}LFO_LN_LFO;
#define LFO_LN_LFO_FLD "if_lfo,lfo_bgn_val,lfo_end_val,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif,aver_freq1,aver_freq2"

// 机组振荡信息
typedef struct  tag_LFO_GN_LFO
{
	int             if_lfo;	                                //  是否出现振荡
	float           lfo_bgn_val;	                        //  LFO前均值
	float           lfo_end_val;	                        //  LFO后均值
	RTDB_TIME       slfo_bgn_t;	                            //  自身LFO发生时间
	RTDB_TIME       slfo_end_t;	                            //  自身LFO消失时间
	char            slfo_time[20];	                        //  持续时间
	float           slfo_bgn_val;	                        //  自身LFO前均值
	float           slfo_end_val;	                        //  自身LFO后均值
	float           lfo_max_val;	                        //  LFO最大值
	RTDB_TIME       lfo_max_val_t;	                        //  LFO最大值时间
	float           lfo_min_val;	                        //  LFO最小值
	RTDB_TIME       lfo_min_val_t;	                        //  LFO最小值时间
	float           lfo_max_min_dif;	                    //  LFO最大差值
	float           aver_freq1;	                            //  振荡频率1
	float           aver_freq2;	                            //  振荡频率2
}LFO_GN_LFO;
#define LFO_GN_LFO_FLD "if_lfo,lfo_bgn_val,lfo_end_val,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif,aver_freq1,aver_freq2"

// 变压器振荡信息
typedef struct  tag_LFO_TR_LFO
{
    unsigned char   if_lfo;	                                //  是否出现振荡
    float           lfo_bgn_val;	                        //  LFO前均值
    float           lfo_end_val;	                        //  LFO后均值
	RTDB_TIME       slfo_bgn_t;	                            //  自身LFO发生时间
	RTDB_TIME       slfo_end_t;	                            //  自身LFO消失时间
    char            slfo_time[20];	                        //  持续时间
    float           slfo_bgn_val;	                        //  自身LFO前均值
    float           slfo_end_val;	                        //  自身LFO后均值
    float           lfo_max_val;	                        //  LFO最大值
    RTDB_TIME       lfo_max_val_t;	                        //  LFO最大值时间
    float           lfo_min_val;	                        //  LFO最小值
	RTDB_TIME       lfo_min_val_t;	                        //  LFO最小值时间
    float           lfo_max_min_dif;	                    //  LFO最大差值
    float           aver_freq1;	                            //  振荡频率1
    float           aver_freq2;	                            //  振荡频率2
}LFO_TR_LFO;
#define LFO_TR_LFO_FLD "if_lfo,lfo_bgn_val,lfo_end_val,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif,aver_freq1,aver_freq2"


// 厂站监视信息
typedef struct tab_LFO_FC_MON
{
// 	int             fac_id;                                 //  厂站ID
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	float lead_ampl;	/* 振幅 */
	int lead_amplclr;	/* 振幅颜色 */
	float lead_freq;	/* 频率 */
	int lead_freqclr;	/* 频率颜色 */
	int fac_stat;	/* 厂站状态 */
	int sso_mon;	/* 是否可观 */
}TAB_LFO_FC_MON;
#define TAB_LFO_FC_MON_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,fac_stat,sso_mon"
// 厂站监视信息状态
typedef struct tab_LFO_FC_MON_STAT
{
	// 	int             fac_id;                                 //  厂站ID
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	float lead_ampl;	/* 振幅 */
	int lead_amplclr;	/* 振幅颜色 */
	float lead_freq;	/* 频率 */
	int lead_freqclr;	/* 频率颜色 */
	int fac_stat;	/* 厂站状态 */
	int sso_mon;	/* 是否可观 */
}TAB_LFO_FC_MON_STAT;
#define TAB_LFO_FC_MON_STAT_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,fac_stat,sso_mon"


// 机组监视信息
typedef struct tab_SSO_GN_MON
{
//     int             gn_id;                                  //  机组ID号
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	INT_REC         mode_rcd_id;                            //  在设备模式全表中首记录ID号
	int             mode_rcd_num;	                        //  在设备模式全表中的记录个数
	float           lead_ampl;                              //  振幅
	int lead_amplclr;	/* 振幅颜色 */
	float           lead_freq;                              //  频率
	int lead_freqclr;	/* 频率颜色 */
	float           lead_damp;                              //  阻尼比
	float           lead_phas;                              //  初相
	float           lead_real;                              //  实部
	float           lead_imag;                              //  虚部
	float           lead_engy;                              //  能量
	float           lead_fail;                              //  衰减
	float           aver_val;                               //  均值
	int				if_sso;									/* 是否出现振荡 */
	RTDB_TIME		sso_bgn_t;								/* SSO发生时间 */
	RTDB_TIME		sso_end_t;								/* SSO消失时间 */
	char			sso_time[20];							/* 持续时间 */
    float           max_ampl;                               //  最大幅值
	RTDB_TIME		lfo_max_ampl_t;							//  最大幅值时间 
	float			aver_freq1;								//	振荡频率1 
	int sso_mon;	/* 是否可观 */
}TAB_SSO_GN_MON;
#define TAB_SSO_GN_MON_FLD "stat,stat_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,if_sso,sso_bgn_t,sso_end_t,sso_time,max_ampl,lfo_max_ampl_t,aver_freq1,sso_mon"


// 机组监视信息状态
typedef struct tab_SSO_GN_MON_STAT
{
	//     int             gn_id;                                  //  机组ID号
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	float           lead_ampl;                              //  振幅
	int lead_amplclr;	/* 振幅颜色 */
	float           lead_freq;                              //  频率
	int lead_freqclr;	/* 频率颜色 */
}TAB_SSO_GN_MON_STAT;
#define TAB_SSO_GN_MON_STAT_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr"

// 交流线路监视信息
typedef struct tab_SSO_LN_MON
{
//     int             acln_id;                                //  线路ID号
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	INT_REC         mode_rcd_id;                            //  在设备模式全表中首记录ID号
	int             mode_rcd_num;	                        //  在设备模式全表中的记录个数
	float           lead_ampl;                              //  振幅
	int lead_amplclr;	/* 振幅颜色 */
	float           lead_freq;                              //  频率
	int lead_freqclr;	/* 频率颜色 */
	float           lead_damp;                              //  阻尼比
	float           lead_phas;                              //  初相
	float           lead_real;                              //  实部
	float           lead_imag;                              //  虚部
	float           lead_engy;                              //  能量
	float           lead_fail;                              //  衰减
	int				if_sso;									/*  是否出现振荡 */
	RTDB_TIME		sso_bgn_t;								/*  SSO发生时间 */
	RTDB_TIME		sso_end_t;								/*  SSO消失时间 */
	char			sso_time[20];							/*  持续时间 */
    float           max_ampl;                               //  最大幅值
	RTDB_TIME		lfo_max_ampl_t;							/*  最大幅值时间 */
    INT_REC         slct_dot_id;                            //  选定线端ID  2008-04-05 added by zhw at yn
	float			aver_freq1;								/*  振荡频率1 */
	int sso_mon;	/* 是否可观 */
}TAB_SSO_LN_MON;
#define TAB_SSO_LN_MON_FLD "stat,stat_val,"\
"mode_rcd_id,mode_rcd_num,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,lead_damp,lead_phas,lead_real,"\
"lead_imag,lead_engy,lead_fail,if_sso,sso_bgn_t,sso_end_t,sso_time,"\
"max_ampl,lfo_max_ampl_t,slct_dot_id,aver_freq1,sso_mon"

typedef struct tab_SSO_LN_MON_STAT
{
	//     int             acln_id;                                //  线路ID号
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
	float           lead_ampl;                              //  振幅
	int lead_amplclr;	/* 振幅颜色 */
	float           lead_freq;                              //  频率
	int lead_freqclr;	/* 频率颜色 */
}TAB_SSO_LN_MON_STAT;
#define TAB_SSO_LN_MON_STAT_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr"

// 交流线路监视信息状态
typedef struct tab_SSO_LN_STAT
{
	//     int             acln_id;                                //  线路ID号
	int             stat;                                   //  状态
	int             stat_val;                               //  状态值
}TAB_SSO_LN_STAT;
#define TAB_SSO_LN_STAT_FLD "stat,stat_val,"\


/*
#define TAB_LFO_LN_MON_FLD "pwr_val,stat,stat_val,damp_grad,damp_grad_val,\
mode_rcd_id,mode_rcd_num,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,\
lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,\
max_ampl,max_ampl_rat"*/

// 变压器监视信息
typedef struct tab_SSO_TR_MON
{
    int             stat;                                   //  状态
    int             stat_val;                               //  状态值
    INT_REC         mode_rcd_id;                            //  在设备模式全表中首记录ID号
    int             mode_rcd_num;	                        //  在设备模式全表中的记录个数
    float           lead_ampl;                              //  振幅
	int				lead_amplclr;							/* 振幅颜色 */
	float           lead_freq;                              //  频率
	int				lead_freqclr;							/* 频率颜色 */
    float           lead_damp;                              //  阻尼比
    float           lead_phas;                              //  初相
    float           lead_real;                              //  实部
    float           lead_imag;                              //  虚部
    float           lead_engy;                              //  能量
    float           lead_fail;                              //  衰减
    float           max_ampl;                               //  最大幅值
	RTDB_TIME		lfo_max_ampl_t;							/*最大幅值时间 */
	unsigned char	if_sso;									/* 是否出现振荡 */
	RTDB_TIME		sso_bgn_t;								/* SSO发生时间 */
	RTDB_TIME		sso_end_t;								/* SSO消失时间 */
	char			sso_time[20];							/* 持续时间 */
	float			aver_freq1;								/* 振荡频率1 */
	int sso_mon;	/* 是否可观 */
}TAB_SSO_TR_MON;
#define TAB_SSO_TR_MON_FLD "stat,stat_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,lead_damp,lead_phas,lead_real,"\
    "lead_imag,lead_engy,lead_fail,max_ampl,lfo_max_ampl_t,if_sso,sso_bgn_t,sso_end_t,sso_time,aver_freq1,sso_mon"

typedef struct tab_SSO_TR_MON_STAT
{
    int             stat;                                   //  状态
    int             stat_val;                               //  状态值
    float           lead_ampl;                              //  振幅
	int				lead_amplclr;							/* 振幅颜色 */
	float           lead_freq;                              //  频率
	int				lead_freqclr;							/* 频率颜色 */
}TAB_SSO_TR_MON_STAT;
#define TAB_SSO_TR_MON_STAT_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr"

/*  LFO实时设备模式全表    2007-12-3 grm修改  */
typedef struct tab_LFO_DEVICE_MODE
{
	INT_REC record_id;	/* 记录ID */
	INT_REC dev_id;	/* 设备ID */
	int dev_type;	/* 设备类型 */
	unsigned char is_show;	/* 是否显示 */
	float ampl;	/* 振幅 */
	float freq;	/* 频率 */
	float damp;	/* 阻尼比 */
	float phas;	/* 初相 */
	float real;	/* 实部 */
	float imag;	/* 虚部 */
	float engy;	/* 能量 */
	float fail;	/* 衰减 */

}TAB_LFO_DEVICE_MODE;
#define TAB_LFO_DEVICE_MODE_FLD "record_id,dev_id,dev_type,ampl,freq,damp,phas,real,imag,engy,fail"

/* LFO实时设备主导模式表 */
typedef struct tab_LFO_DEV_LEAD_MODE
{
	INT_REC record_id;	/* 记录ID */
	INT_REC wds_no;	/* 窗口序号 */
	RTDB_TIME wds_bgn;	/* 起始时刻 */
	RTDB_TIME wds_end;	/* 截至时刻 */
	INT_REC dev_id;	/* 设备ID */
	int dev_type;	/* 设备类型 */
	unsigned char is_show;	/* 是否显示 */
	float ampl;	/* 振幅 */
	float freq;	/* 频率 */
	float damp;	/* 阻尼比 */
	float phas;	/* 初相 */
	float real;	/* 实部 */
	float imag;	/* 虚部 */
	float engy;	/* 能量 */
	float fail;	/* 衰减 */
}TAB_LFO_DEV_LEAD_MODE;
#define TAB_LFO_DEV_LEAD_MODE_FLD "record_id,wds_no,wds_bgn,wds_end,dev_id,dev_type,is_show,ampl,freq,damp,phas,real,imag,engy,fail"


/* LFO设备模式  */
// typedef struct tag_LFO_DEVICE_MODE
// {
// 	INT_REC dev_id;	/* 设备ID */
// 	int dev_type;	/* 设备类型 */
// 	unsigned char is_show;	/* 是否显示 */
// 	float ampl;	/* 振幅 */
// 	float freq;	/* 频率 */
// 	float damp;	/* 阻尼比 */
// 	float phas;	/* 初相 */
// 	float real;	/* 实部 */
// 	float imag;	/* 虚部 */
// 	float engy;	/* 能量 */
// 	float fail;	/* 衰减 */
// }LFO_DEVICE_MODE;
// #define LFO_DEVICE_MODE_FLD "dev_id,dev_type,ampl,freq,damp,phas,real,imag,engy,fail"

/* LFO实时设备主导模式表 */
typedef struct tag_LFO_DEV_LEAD_MODE
{
	INT_REC wds_no;	/* 窗口序号 */
	RTDB_TIME wds_bgn;	/* 起始时刻 */
	RTDB_TIME wds_end;	/* 截至时刻 */
	INT_REC dev_id;	/* 设备ID */
	int dev_type;	/* 设备类型 */
	unsigned char is_show;	/* 是否显示 */
	float ampl;	/* 振幅 */
	float freq;	/* 频率 */
	float damp;	/* 阻尼比 */
	float phas;	/* 初相 */
	float real;	/* 实部 */
	float imag;	/* 虚部 */
	float engy;	/* 能量 */
	float fail;	/* 衰减 */
}LFO_DEV_LEAD_MODE;
#define LFO_DEV_LEAD_MODE_FLD "wds_no,wds_bgn,wds_end,dev_id,dev_type,is_show,ampl,freq,damp,phas,real,imag,engy,fail"

/* LFO实时模态表 */
// typedef struct tag_LFO_WDS_SHAPE
// {
// 	INT_REC wdcase_no;	/* CASE号 */
// 	int app_no;	/* 功能号 */
// 	int mode_no;	/* 模式号 */
// 	int shape_no;	/* 模态序号 */
// 	float rreall;	/* 特征值实部 */
// 	float rimag;	/* 特征值虚部 */
// 	float rmgnt;	/* 特征值幅值 */
// 	float rphas;	/* 特征值相角 */
// 	float fact;	/* 参与因子 */
// 	int fact_sts2;	/* 参与因子状态 */
// 	int dev_id;	/* 设备ID */
// 	char dev_name[64];	/* 设备名称 */
// 	int dev_type;	/* 设备类型 */
// 	int fac_id;	/* 所属厂站 */
// 	int area_id;	/* 所属区域 */
// }LFO_WDS_SHAPE;
// 
// #define LFO_WDS_SHAPE_FLD "record_id,wdcase_no,app_no,mode_no,shape_no,rreall,rimag,rmgnt,rphas,fact,fact_sts2,dev_id,dev_name,dev_type,fac_id,area_id"

/* LFO实时模态表 */
typedef struct tab_LFO_WDS_SHAPE
{
	INT_REC record_id;	/* 记录号 */
	int wdcase_no;	/* CASE号 */
	int app_no;	/* 功能号 */
	int mode_no;	/* 模式号 */
	int shape_no;	/* 模态序号 */
	float rreall;	/* 特征值实部 */
	float rimag;	/* 特征值虚部 */
	float rmgnt;	/* 特征值幅值 */
	float rphas;	/* 特征值相角 */
	float fact;	/* 参与因子 */
	int fact_sts2;	/* 参与因子状态 */
	INT_REC dev_id;	/* 设备ID */
	char dev_name[64];	/* 设备名称 */
	int dev_type;	/* 设备类型 */
	INT_REC fac_id;	/* 所属厂站 */
	INT_REC area_id;	/* 所属区域 */
    float freq; /* 频率 */
    float damp; /* 阻尼比 */
    float ampl; /* 振幅 */
    float engy; /* 能量 */
    float max_ampl; /* 最大幅值 */
    float max_ampl_rat; /* 最大波动率 */
    unsigned char clus; /* 所属群 */
    float aver_val; /* 平均值 */
}TAB_LFO_WDS_SHAPE;
#define TAB_LFO_WDS_SHAPE_FLD "record_id,wdcase_no,app_no,mode_no,shape_no,rreall,rimag,rmgnt,rphas,fact,fact_sts2,dev_id,dev_name,dev_type,fac_id,area_id,freq,damp,ampl,engy,max_ampl,max_ampl_rat,clus,aver_val"

/* LFO实时CASE模态全表 */
typedef struct tab_lfo_case_allshape
{
	INT_REC record_id;	/* 记录号 */
	int wds_no;	/* 窗口号 */
	int app_no;	/* 功能号 */
	int mode_no;	/* 模式号 */
	int shape_no;	/* 模态序号 */
	float rreall;	/* 特征值实部 */
	float rimag;	/* 特征值虚部 */
	float rmgnt;	/* 特征值幅值 */
	float rphas;	/* 特征值相角 */
	float fact;	/* 参与因子 */
	int fact_sts2;	/* 参与因子状态 */
	INT_REC dev_id;	/* 设备ID */
	char dev_name[64];	/* 设备名称 */
	int dev_type;	/* 设备类型 */
	INT_REC fac_id;	/* 所属厂站 */
	INT_REC area_id;	/* 所属区域 */
	unsigned char is_prim;	/* 是否主导模式 */
	unsigned char is_curt;	/* 是否当前模式 */
    unsigned char clus;	/* 所属群 */
    float freq;	/* 频率 */
    float damp;	/* 阻尼比 */
    float ampl;	/* 振幅 */
	float engy;	/* 能量 */
    float aver_val; /* 平均值 */
    float max_ampl; /* 最大幅值 */
    float max_ampl_rat; /* 最大波动率 */
}TAB_LFO_CASE_ALLSHAPE;
#define TAB_LFO_CASE_ALLSHAPE_FLD "record_id,wds_no,app_no,mode_no,shape_no,rreall,rimag,rmgnt,rphas,fact,fact_sts2,dev_id,dev_name,dev_type,fac_id,area_id,is_prim,is_curt,clus,freq,damp,ampl,engy,aver_val,max_ampl,max_ampl_rat"

// LFO辅助决策表
typedef struct tab_LFO_ASST_DECI
{
    INT_REC             record_id;		                        //  记录号
	RTDB_TIME           occur_time;		                        //  时间
    INT_REC             key_dev_id;		                        //  关键设备
    char            key_dev_name[64];		                //  关键设备名
    int             key_dev_type;		                    //  关键设备类型
    float           lead_freq;		                        //  振荡频率
    float           lead_damp;		                        //  阻尼比
    float           lead_ampl;		                        //  振荡幅值
    char            lfo_time[64];		                    //  持续时间
    int             deci_type;		                        //  策略等级
    char            ctl_act_dsec[128];		                //  控制措施描述
    INT_REC             dev_id;		                            //  设备号
    char            dev_name[64];		                    //  设备名
    int             dev_type;		                        //  设备类型
    float           dev_value;		                        //  当前均值
    int             dev_act_type;		                    //  调整类型
    float           dev_change;		                        //  调整量
    int             is_show;		                        //  是否显示
    int             is_flow_pstv;		                    //  是否潮流为正 // 2008-04-04 
    float           key_dev_aver;          	                //  有功均值 // 2008-04-04 
    float           key_dev_ampl;	             	        //  有功波动值 // 2008-04-04 
    float           dev_ampl;                               //  最大幅值 // 2008-04-04 
    float           dev_ampl_rat;                           //  最大波动率(波动幅值/均值) // 2008-04-04 
    int             deci_sur;		                        //  策略来源(0 分析结果 1 离线策略) // 2008-04-04 
}TAB_LFO_ASST_DECI;
#define TAB_LFO_ASST_DECI_FLD "record_id,occur_time,key_dev_id,key_dev_name,key_dev_type,lead_freq,lead_damp,lead_ampl,lfo_time,deci_type," \
"ctl_act_dsec,dev_id,dev_name,dev_type,dev_value,dev_act_type,dev_change,is_show,is_flow_pstv,key_dev_aver,key_dev_ampl,dev_ampl,dev_ampl_rat,deci_sur"

// typedef struct tagWAM_CHANNLE_INFO_STRU
// {
//     int               chan_id;
//     int               comm_fac_id;
//     int               fac_id;
//     short             proto_type;
//     unsigned char     chan_type;
//     char              idcode[9];
//     char              stn[17];
// }WAM_CHANNEL_INFO_STRU;
// #define TAB_WAM_CHANNEL_INFO_FLD "chan_id,comm_fac_id,fac_id,proto_type,chan_type,idcode,stn"

// typedef struct tagDynFileStru
// {
//     int               fac_id;
//     int               beg_sec;
//     int               end_sec;
// }DynFileStru;


typedef struct tagLFO_HIS_TIME
{
    int               his_bgn;
    int               his_end;
}LFO_HIS_TIME;
#define TAB_LFO_HIS_TIME "his_bgn,his_end"
/******************************************************************************/
//更新厂站参与因子、所属群、是否中心
typedef struct tab_fac_info_wamap
{
    float         lfo_fact;             // lfo参与因子
    unsigned char lfo_clus;			    //lfo所属群//default_uc1;
    unsigned char is_cntr;			    //lfo是否中心//default_uc2;
}TAB_FAC_INFO_WAMAP;
#define TAB_FAC_INFO_WAMAP_FLD "lfo_fact,lfo_clus,is_cntr"

//更新线路"是否振荡"
typedef struct tab_acln_device_wamap
{
    unsigned char if_lfo;			 //lfo是否振荡default_uc1;
}TAB_ACLN_DEVICE_WAMAP;
#define  TAB_ACLN_DEVICE_WAMAP_FLD "if_lfo"

//更新发电机参与因子、所属群
typedef struct tab_gen_device_wamap
{
    float         lfo_fact;          // lfo参与因子default_f3
    unsigned char lfo_clus;			 //lfo是否振荡default_uc1;
}TAB_GEN_DEVICE_WAMAP;
#define  TAB_GEN_DEVICE_WAMAP_FLD "lfo_fact,lfo_clus"
/******************************************************************************/
//读数据文件时，存放临时数据
typedef struct TempData
{
	INT_REC	  id;
	char  name[64];
	char  name_1[64];
	float data;
	float data1[DATA_NUM_MAX];
	float data2[DATA_NUM_MAX];
}TEMPDATA;

/* 应用变量表 */
// typedef struct app_variable_lfo
// {
//     //int var_id;	/* ID */
//     //int virtual_time;	/* 虚拟时间 */  // PMU时间，罗为刷新
//     int milli_sec;	/* 毫秒数 */
//     int model_time;	/* 模型时间 */
//     char message[200];	/* 最新消息 */  // 振荡信息
//     int data_status;	/* 数据状态 */  // 正常/数据不刷新
//     int app_status;	/* 应用状态 */      // 正常/越限/越中间限/越最高限  低频振荡只使用正常/越中间限
//     int acln_limit_1;	/* 线路电流越限个数，对应最低越限门槛 */
//     int acln_limit_2;	/* 线路电流越限个数，对应中间越限门槛 */
//     int acln_limit_3;	/* 线路电流越限个数，对应最高越限门槛 */
//     int tr_limit_1;	/* 变压器功率越限个数，对应最低越限门槛 */
//     int tr_limit_2;	/* 变压器功率越限个数，对应中间越限门槛 */
//     int tr_limit_3;	/* 变压器功率越限个数，对应最高越限门槛 */
//     int bus_low_limit_1;	/* 母线电压越下限个数，对应最低越限门槛 */
//     int bus_low_limit_2;	/* 母线电压越下限个数，对应中间越限门槛 */  // 告警线路个数
//     int bus_low_limit_3;	/* 母线电压越下限个数，对应最高越限门槛 */
//     int bus_up_limit_1;	/* 母线电压越上限个数，对应最低越限门槛 */
//     int bus_up_limit_2;	/* 母线电压越上限个数，对应中间越限门槛 */      // 告警发电机个数
//     int bus_up_limit_3;	/* 母线电压越上限个数，对应最高越限门槛 */
//     int bgp_limit_1;	/* 断面越限个数，对应最低越限门槛 */
//     int bgp_limit_2;	/* 断面越限个数，对应中间越限门槛 */
//     int bgp_limit_3;	/* 断面越限个数，对应最高越限门槛 */
//     //int data_refresh_time;	/* 数据刷新时间 */
// }APP_VARIABLE_LFO;
// 
// #define APP_VARIABLE_LFO_DEFFIELDS "milli_sec,model_time,message,data_status,app_status,acln_limit_1,acln_limit_2,acln_limit_3,tr_limit_1,tr_limit_2,tr_limit_3,bus_low_limit_1,bus_low_limit_2,bus_low_limit_3,bus_up_limit_1,bus_up_limit_2,bus_up_limit_3,bgp_limit_1,bgp_limit_2,bgp_limit_3"

/* WAMS应用模块告警信息表 */
typedef struct wams_app_warn_info_wams_lfo
{
//    INT_REC record_id;	/* 记录ID */
    unsigned char warn_level;	/* 告警等级 */
    char app_info[128];	/* 应用概要信息 */
//    unsigned char app_status_change;	/* 应用状态变化位 */
	RTDB_TIME refresh_tm;	/* 刷新时间 */
//    char graph_name[64];	/* 关联画面 */
    int app_info_1;	/* 面板数据1 */
    char app_info_desc_1[64];	/* 面板数据1描述 */
    int app_info_2;	/* 面板数据2 */
    char app_info_desc_2[64];	/* 面板数据2描述 */
    int app_info_3;	/* 面板数据3 */
    char app_info_desc_3[64];	/* 面板数据3描述 */
//     int app_info_4;	/* 面板数据4 */
//     char app_info_desc_4[64];	/* 面板数据4描述 */
//     int app_info_5;	/* 面板数据5 */
//     char app_info_desc_5[64];	/* 面板数据5描述 */
//     int app_info_6;	/* 面板数据6 */
//     char app_info_desc_6[64];	/* 面板数据6描述 */
    unsigned char is_popup_theme;	/* 是否告警推主题 */
//    int app_module_id;	/* 应用模块名称 */
}WAMS_APP_WARN_INFO_WAMS_LFO;

#define WAMS_APP_WARN_INFO_WAMS_LFO_DEFFIELDS "warn_level,app_info,refresh_tm,app_info_1,app_info_desc_1,app_info_2,app_info_desc_2,app_info_3,app_info_desc_3,is_popup_theme"


/******************************************************************************/


#endif