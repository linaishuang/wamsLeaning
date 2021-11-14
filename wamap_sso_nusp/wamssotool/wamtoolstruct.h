#ifndef _WAM_MV_STRUCT_H_
#define _WAM_MV_STRUCT_H_

#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// 宏定义
#ifndef WT_MENU_NO
#define WT_MENU_NO  0
#define WT_MENU_YES 1
#endif

#ifndef WT_MODE_NEW
#define WT_MODE_TEST 1      // 生成设备信息，不修改商用库
#define WT_MODE_NEW  2      // 新增设备
#define WT_MODE_ALL  3      // 设备全部更新
#endif

//////////////////////////////////////////////////////////////////////////
typedef struct type_WT_PARA_INFO
{
	int   nMode;     // 1,新增设备;
	                   // 2,清除原有设备后全部更新;
	int   nCurveMode; // 更新设备曲线表
	float VBase;     // 电压等级，大于该电压等级的设备作为候选设备
	int   IsFacUpdate;  // 是否更新LFO厂站表
	int   IsGenUpdate;  // 是否更新LFO发电机表
	int   IsAclnUpdate; // 是否更新LFO交流线路表
	int   IsTrUpdate;   // 是否更新LFO变压器表
	int   IsVFDevUpdate;  // 是否更新VF动态评估母线表
	int   IsAmGenUpdate;  // 是否更新WAM_AM设备表

// 	int  nBusAll;  // 母线总数
// 	int  nBusPmu;  // 装有PMU母线个数
// 	int  nBusOld;  // VF动态评估母线表中记录个数
// 	int  nBusNew;  // 新增母线个数

	int  nFacAll;  // 厂站总数
	int  nFacPmu;  // 装有PMU厂站个数
	int  nFacOld;  // LFO厂站表中记录个数
	INT_REC MAX_FacOld_idx;  // LFO厂站表中记录最大索引idx
	int  nFacNew;  // 新增厂站个数
	
	int  nGenAll;  // 发电机总数
	int  nGenPmu;  // 装有PMU发电机个数
	int  nGenOld;  // LFO发电机表中记录个数
	INT_REC MAX_GenOld_idx;  // LFO发电机表中记录最大索引idx
	int  nGenNew;  // 新增发电机个数

	int  nAclnAll;  // 交流线路总数
	int  nAclnPmu;  // 装有PMU交流线路个数
	int  nAclnOld;  // LFO交流线路表中记录个数
	INT_REC MAX_AclnOld_idx;  // LFO交流线路表中记录最大索引idx
	int  nAclnNew;  // 新增交流线路个数

	int  nTrAll;  // 变压器总数
	int  nTrPmu;  // 装有PMU变压器个数
	int  nTrOld;  // LFO变压器表中记录个数
	INT_REC MAX_TrOld_idx;  // LFO变压器表中记录最大索引idx
	int  nTrNew;  // 新增变压器个数
	
	int  nVFAll;  // VF设备总数
	int  nVFPmu;  // 装有PMU的设备个数
	int  nVFOld;  // VF设备表中记录个数
	INT_REC MAX_VFOld_idx;  // VF设备表中记录最大索引idx
	int  nVFNew;  // 新增设备个数

	char GenInfoFile[MAXBUF];  // 机组信息文件名
	int  nAMAll;  // AM设备总数
	int  nAMPmu;  // 装有PMU的设备个数
	int  nAMOld;  // AM设备表中记录个数
	INT_REC MAX_AMOld_idx;  // AM设备表中记录最大索引idx
	int  nAMNew;  // 新增设备个数
}WT_PARA_INFO;
// 端点信息
typedef struct  type_WT_DOT_INFO
{
	INT_REC dot_id; // 设备一端ID号
	INT_REC fac_id; // 厂站ID号
	INT_REC area_id; // 区域ID号
	VOL_INT_REC vlty_id; // 电压类型ID号
	float vbase; // 电压等级
	float vtap;  // 分接头电压
	INT_REC tapty_id; /* 分接头类型ID号 */
	float tap_pos; // 分接头位置

	char dot_name[64]; // 设备名称
	char fac_name[64]; // 厂站名称
	char area_name[64]; // 区域名称

	int  IsYcDefine;   // 测点是否可用
	int  nYcFlag[VEC_SSO_NUM];
	int  nYcFilNo[VEC_SSO_NUM];
	int  nYcColNo[VEC_SSO_NUM];
}WT_DOT_INFO;

// 设备信息
typedef struct  type_WT_DEV_INFO
{
	INT_REC dev_id;  // 设备ID号
	INT_REC fac_id;  // 厂站ID号
	INT_REC area_id;  // 区域ID号
	char dev_name[64];  // 设备名称
	char fac_name[64]; // 厂站名称
	char area_name[64]; // 区域名称
	char bpa_name[64];  // BPA名称
	int dev_type;  // 设备类型
	VOL_INT_REC maxvlty_id;  // 最高电压等级ID
	float maxvbase;  // 最高电压等级
	int sim_curve_idx;  // 仿真曲线顺序号

	// 变压器
	int trty;	// 绕组类型
	int tr_type;	// 变压器类型
	float temperature;  // 当前温度
	// 母线频率测点
	INT_REC freq_yc_id;  // 测点ID号(测点遥测信息表)
	INT_REC volt_per_id;  // 电压允许偏移组
	INT_REC freq_per_id;  // 频率允许偏移组
	
	int is_old;  // 已存在
	int vflag;   // 电压等级标志
	int yc_flag;  // 测点标志;  0,三端均无效;1,一端有效;2,二端有效;
	                        // 3,一二端均有效;
	                        // 4,三端有效;  5,一三端有效; 6,一二三端均有效
	int is_add;  // 是否增加设备
	WT_DOT_INFO dot1; // 一端设备信息(厂站电压等级最高母线,发电机,交流线路首端,变压器高压侧,母线)
	WT_DOT_INFO dot2; // 二端设备信息(交流线路末端,变压器低压侧)
	WT_DOT_INFO dot3; // 三端设备信息(变压器中压侧)
}WT_DEV_INFO;

// SSO厂站表
typedef struct type_SSO_FC_TAB_CRE
{
	INT_REC fac_idx;	/* 厂站索引 */
	INT_REC station_id;	/* 厂站ID号 */
	INT_REC area_id;	/* 区域ID号 */
	unsigned char fac_type;	/* 厂站类型 */
	VOL_INT_REC vlty_id;	/* 电压等级 */
	unsigned char is_monitor;	/* 是否监视 */
	unsigned char is_part_ana;	/* 是否参与分析 */
	unsigned char is_m_bs;	/* 正母优先 */
} SSO_FC_TAB_CRE;
#define SSO_FC_TAB_CRE_FLD "fac_idx,station_id,area_id,fac_type,vlty_id,is_monitor,is_part_ana,is_m_bs"
#ifdef _NUSP
#define SSO_FC_TAB_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',"VOL_INT_REC_FMT",%d,%d,%d,%d)"
#define SSO_FC_TAB_CRE_VAL(val) INT_REC_VAL(val.fac_idx),INT_REC_VAL(val.station_id),INT_REC_VAL(val.area_id),val.fac_type,val.vlty_id,val.is_monitor,val.is_part_ana,val.is_m_bs
#else
#define SSO_FC_TAB_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,"INT_REC_FMT",%d,%d,%d)"
#define SSO_FC_TAB_CRE_VAL(val) val.fac_idx,val.station_id,val.area_id,val.fac_type,val.vlty_id,val.is_monitor,val.is_part_ana,val.is_m_bs
#endif // _NUSP


// SSO发电机表
typedef struct type_SSO_GN_TAB_CRE
{
	INT_REC gn_idx;	/* 发电机索引 */
	INT_REC gn_id;	/* 发电机ID号 */
	INT_REC station_id;	/* 厂站ID号 */
	INT_REC area_id;	/* 区域ID号 */
	unsigned char is_monitor;	/* 是否监视 */
	char gn_name[64];  //	发电机名称
	char fac_name[64];  //	厂站名称
	char area_name[64];  //	区域名称
	unsigned char is_part_ana;	/* 是否参与分析 */
} SSO_GN_TAB_CRE;
#define SSO_GN_TAB_CRE_FLD "gn_idx,gn_id,station_id,area_id,is_monitor,gn_name,fac_name,area_name,is_part_ana"
#ifdef _NUSP
#define SSO_GN_TAB_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',%d,'%s','%s','%s',%d)"
#define SSO_GN_TAB_CRE_VAL(val) INT_REC_VAL(val.gn_idx),INT_REC_VAL(val.gn_id),INT_REC_VAL(val.station_id),INT_REC_VAL(val.area_id),val.is_monitor,val.gn_name,val.fac_name,val.area_name,val.is_part_ana
#endif
#define SSO_GN_TAB_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,'%s','%s','%s',%d)"
#define SSO_GN_TAB_CRE_VAL(val) val.gn_idx,val.gn_id,val.station_id,val.area_id,val.is_monitor,val.gn_name,val.fac_name,val.area_name,val.is_part_ana
#endif // _NUSP


// SSO交流线路表
typedef struct type_SSO_LN_TAB_CRE
{
	INT_REC acln_idx;	/* 交流线段索引 */
	INT_REC acln_id;	/* 交流线路ID号 */
	INT_REC dot_id_1;	/* 交流线段首端 */
	INT_REC fac_id_1;	/* 厂站ID号 */
	INT_REC area_id_1;	/* 区域ID号 */
	INT_REC dot_id_2;	/* 交流线段末端 */
	INT_REC fac_id_2;	/* 厂站ID号 */
	INT_REC area_id_2;	/* 区域ID号 */
	unsigned char is_monitor;	/* 是否监视 */
	unsigned char is_end;	/* 末端优先 */
	VOL_INT_REC vlty_id;	/* 电压类型ID号 */
	char acln_name[64];    //	线路名称
	char fac_name_1[64];   //	首端厂站名称
	char fac_name_2[64];   //	末端厂站名称
	char area_name_1[64];  //	首端区域名称
	char area_name_2[64];  //	末端区域名称
	unsigned char is_part_ana;	/* 是否参与分析 */
} SSO_LN_TAB_CRE;
#define SSO_LN_TAB_CRE_FLD "acln_idx,acln_id,dot_id_1,fac_id_1,area_id_1,dot_id_2,fac_id_2,area_id_2,is_monitor,is_end,vlty_id,acln_name,fac_name_1,fac_name_2,area_name_1,area_name_2,is_part_ana"
#ifdef _NUSP
#define SSO_LN_TAB_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',%d,%d,"VOL_INT_REC_FMT",'%s','%s','%s','%s','%s',%d)"
#define SSO_LN_TAB_CRE_VAL(val) INT_REC_VAL(val.acln_idx),INT_REC_VAL(val.acln_id),INT_REC_VAL(val.dot_id_1),INT_REC_VAL(val.fac_id_1),INT_REC_VAL(val.area_id_1),INT_REC_VAL(val.dot_id_2),INT_REC_VAL(val.fac_id_2),INT_REC_VAL(val.area_id_2),val.is_monitor,val.is_end,val.vlty_id,val.acln_name,val.fac_name_1,val.fac_name_2,val.area_name_1,val.area_name_2,val.is_part_ana
#else
#define SSO_LN_TAB_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,%d,"INT_REC_FMT",'%s','%s','%s','%s','%s',%d)"
#define SSO_LN_TAB_CRE_VAL(val) val.acln_idx,val.acln_id,val.dot_id_1,val.fac_id_1,val.area_id_1,val.dot_id_2,val.fac_id_2,val.area_id_2,val.is_monitor,val.is_end,val.vlty_id,val.acln_name,val.fac_name_1,val.fac_name_2,val.area_name_1,val.area_name_2,val.is_part_ana
#endif // _NUSP

// SSO变压器表
typedef struct type_SSO_TR_TAB_CRE
{
    INT_REC tr_idx;	/* 变压器索引 */
    INT_REC tr_id;	/* 变压器ID号 */
    INT_REC station_id;	/* 厂站ID号 */
    INT_REC area_id; /* 区域ID号 */
    unsigned char trty;	/* 绕组类型 */  // 三绕组变，两绕组变
	unsigned char tr_type;	/* 变压器类型 */  // 主变，启动变，厂用变
	INT_REC high_wind_id;	/* 高压侧绕组ID号 */
	VOL_INT_REC high_vlty_id;	/* 高压侧电压类型ID号 */
	INT_REC mid_wind_id;	/* 中压侧绕组ID号 */
	VOL_INT_REC mid_vlty_id;	/* 中压侧电压类型ID号 */
	INT_REC low_wind_id;	/* 低压侧绕组ID号 */
	VOL_INT_REC low_vlty_id;	/* 低压侧电压类型ID号 */
	INT_REC slct_wind_id;	/* 选定绕组ID号 */
    unsigned char is_monitor;	/* 是否监视 */
    unsigned char   hwd_yc_ok;                              //  高压侧测点OK
    unsigned char   mwd_yc_ok;                              //  中压侧测点OK
    unsigned char   lwd_yc_ok;                              //  低压侧测点OK
	char tr_name[64];    //	发电机名称
	char fac_name[64];   //	厂站名称
	char area_name[64];  //	区域名称
    unsigned char is_part_ana;	/* 是否参与分析 */
} SSO_TR_TAB_CRE;
#define SSO_TR_TAB_CRE_FLD "tr_idx,tr_id,station_id,area_id,trty,tr_type,high_wind_id,high_vlty_id,mid_wind_id,mid_vlty_id,low_wind_id,low_vlty_id,slct_wind_id,is_monitor,hwd_yc_ok,mwd_yc_ok,lwd_yc_ok,tr_name,fac_name,area_name,is_part_ana"
#ifdef _NUSP
#define SSO_TR_TAB_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',%d,%d,'"INT_REC_FMT"',"VOL_INT_REC_FMT",'"INT_REC_FMT"',"VOL_INT_REC_FMT",'"INT_REC_FMT"',"VOL_INT_REC_FMT",'"INT_REC_FMT"',%d,%d,%d,%d,'%s','%s','%s',%d)"
#define SSO_TR_TAB_CRE_VAL(val) INT_REC_VAL(val.tr_idx),INT_REC_VAL(val.tr_id),INT_REC_VAL(val.station_id),INT_REC_VAL(val.area_id),val.trty,val.tr_type,INT_REC_VAL(val.high_wind_id),val.high_vlty_id,INT_REC_VAL(val.mid_wind_id),val.mid_vlty_id,INT_REC_VAL(val.low_wind_id),val.low_vlty_id,INT_REC_VAL(val.slct_wind_id),val.is_monitor,val.hwd_yc_ok,val.mwd_yc_ok,val.lwd_yc_ok,val.tr_name,val.fac_name,val.area_name,val.is_part_ana
#else
#define SSO_TR_TAB_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,%d,"INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,%d,%d,%d,'%s','%s','%s',%d)"
#define SSO_TR_TAB_CRE_VAL(val) val.tr_idx,val.tr_id,val.station_id,val.area_id,val.trty,val.tr_type,val.high_wind_id,val.high_vlty_id,val.mid_wind_id,val.mid_vlty_id,val.low_wind_id,val.low_vlty_id,val.slct_wind_id,val.is_monitor,val.hwd_yc_ok,val.mwd_yc_ok,val.lwd_yc_ok,val.tr_name,val.fac_name,val.area_name,val.is_part_ana
#endif // _NUSP

/* VF动态评估母线表 */
typedef struct type_PQ_DA_BUS_TAB_CRE
{
#ifdef _NUSP
	INT_REC bus_idx;	/* 考察母线索引 */
#endif // _NUSP		
	INT_REC bus_id;	/* 考察母线ID号 */
	INT_REC fac_id;	/* 厂站ID号 */
	INT_REC area_id;	/* 区域ID */
	int volt_monitor;	/* 电压监视   */
	INT_REC volt_per_id;	/* 电压允许偏移组 */
	int freq_monitor;	/* 频率监视 */
	INT_REC freq_per_id;	/* 频率允许偏移组 */
	INT_REC freq_yc_id;	/* 频率测点ID */
	float vbase;	/* 电压基准 */
}PQ_DA_BUS_TAB_CRE;
#ifdef _NUSP
#define PQ_DA_BUS_TAB_CRE_FLD "bus_idx,bus_id,fac_id,area_id,volt_monitor,volt_per_id,freq_monitor,freq_per_id,freq_yc_id,vbase"
#define PQ_DA_BUS_TAB_CRE_FMT "INSERT INTO %s (%s)\nVALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',%d,'"INT_REC_FMT"',%d,'"INT_REC_FMT"','"INT_REC_FMT"',%f)"
#define PQ_DA_BUS_TAB_CRE_VAL(val) INT_REC_VAL(val.bus_idx),INT_REC_VAL(val.bus_id),INT_REC_VAL(val.fac_id),INT_REC_VAL(val.area_id),val.volt_monitor,INT_REC_VAL(val.volt_per_id),val.freq_monitor,INT_REC_VAL(val.freq_per_id),INT_REC_VAL(val.freq_yc_id),val.vbase
#else
#define PQ_DA_BUS_TAB_CRE_FLD "bus_id,fac_id,area_id,volt_monitor,volt_per_id,freq_monitor,freq_per_id,freq_yc_id,vbase"
#define PQ_DA_BUS_TAB_CRE_FMT "INSERT %s (%s)\nVALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,"INT_REC_FMT",%d,"INT_REC_FMT","INT_REC_FMT",%f)"
#define PQ_DA_BUS_TAB_CRE_VAL(val) INT_REC_VAL(val.bus_id),INT_REC_VAL(val.fac_id),INT_REC_VAL(val.area_id),val.volt_monitor,INT_REC_VAL(val.volt_per_id),val.freq_monitor,INT_REC_VAL(val.freq_per_id),INT_REC_VAL(val.freq_yc_id),val.vbase
#endif // _NUSP

/* VF动态评估参数表 */
typedef struct type_PQ_DA_CTL_PARA_TAB_CRE
{
	INT_REC record_id;	/* 记录ID */
	INT_REC dft_v_para;	/* 缺省电压二元参数组 */
	INT_REC dft_f_para;	/* 缺省频率二元参数组 */
}PQ_DA_CTL_PARA_TAB_CRE;

#define PQ_DA_CTL_PARA_TAB_CRE_FLD "record_id,dft_v_para,dft_f_para"

//机组信息结构体
typedef struct tag_GN_INFO_BASE
{
	INT_REC	dev_idx;  // 序号
	INT_REC	dev_id;  //	设备ID
	INT_REC	fac_id;  //	厂站ID
	INT_REC	area_id;  // 区域ID
	char dev_name[64];  //	设备名称
	char fac_name[64];  //	厂站名称
	char area_name[64];  //	区域名称
	int	dev_type;  //	设备类型
	float	r_value;  //	电阻值
	float	x_value;  //	电抗值
	int	is_monitor;  //	是否监视
	int	is_calc_ang;  //	是否推算功角
	char bpaname[64];  //	BPA名称
	int	sim_curve_idx;  //	仿真曲线顺序号
	int	stat;  //	状态
	int stat_value;  // 状态值
	char stat_desc[128];  // 状态描述
	float	angle_value;  //	功角平均值
	float	power_value;  //	功率平均值
	int	group;  //	所属群
	int fn[VEC_ALL_NUM];  // 测点定义:UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA QWR U1V U1A I1V I1A
	int cn[VEC_ALL_NUM];
}GN_INFO_BASE;

/* WAM_AM设备表 */
typedef struct typde_WAM_AM_DEV_CRE
{
#ifdef _NUSP
	INT_REC dev_idx;	/* 设备索引 */
#endif // _NUSP	
	INT_REC dev_id;	/* 设备ID */
	INT_REC fac_id;	/* 厂站ID */
	INT_REC area_id;	/* 区域ID */
	VOL_INT_REC vlty_id;	/* 电压等级ID */
	char dev_name[64];	/* 设备名称 */
	char fac_name[64];	/* 厂站名称 */
	char area_name[64];	/* 区域名称 */
	int dev_type;	/* 设备类型 */
	float r_value;	/* 电阻值 */
	float x_value;	/* 电抗值 */
	int is_monitor;	/* 是否监视 */
	int is_calc_ang;	/* 是否推算功角 */
	char bpaname[64];	/* 设备BPA名称 */
	int sim_curve_idx;	/* 仿真曲线顺序号 */
	int stat;	/* 状态 */
	int stat_value;	/* 状态值 */
	char stat_desc[128];	/* 状态描述 */
	float angle_value;	/* 功角平均值 */
	float power_value;	/* 功率平均值 */
	int group_id;	/* 所属群 */
	int fn1;	/* 文件号1 */
	int fn2;	/* 文件号2 */
	int fn3;	/* 文件号3 */
	int fn4;	/* 文件号4 */
	int fn5;	/* 文件号5 */
	int fn6;	/* 文件号6 */
	int fn7;	/* 文件号7 */
	int fn8;	/* 文件号8 */
	int fn9;	/* 文件号9 */
	int fn10;	/* 文件号10 */
	int fn11;	/* 文件号11 */
	int fn12;	/* 文件号12 */
	int fn13;	/* 文件号13 */
	int fn14;	/* 文件号14 */
	int fn15;	/* 文件号15 */
	int fn16;	/* 文件号16 */
	int fn17;	/* 文件号17 */
	int fn18;	/* 文件号18 */
	int fn19;	/* 文件号19 */
	int fn20;	/* 文件号20 */
	int fn21;	/* 文件号21 */
	int fn22;	/* 文件号22 */
	int cn1;	/* 列号1 */
	int cn2;	/* 列号2 */
	int cn3;	/* 列号3 */
	int cn4;	/* 列号4 */
	int cn5;	/* 列号5 */
	int cn6;	/* 列号6 */
	int cn7;	/* 列号7 */
	int cn8;	/* 列号8 */
	int cn9;	/* 列号9 */
	int cn10;	/* 列号10 */
	int cn11;	/* 列号11 */
	int cn12;	/* 列号12 */
	int cn13;	/* 列号13 */
	int cn14;	/* 列号14 */
	int cn15;	/* 列号15 */
	int cn16;	/* 列号16 */
	int cn17;	/* 列号17 */
	int cn18;	/* 列号18 */
	int cn19;	/* 列号19 */
	int cn20;	/* 列号20 */
	int cn21;	/* 列号21 */
	int cn22;	/* 列号22 */
}WAM_AM_DEV_CRE;
#ifdef _NUSP
#define WAM_AM_DEV_CRE_FLD "dev_idx,dev_id,fac_id,area_id,vlty_id,dev_name,fac_name,area_name,dev_type,r_value,x_value,is_monitor,is_calc_ang,bpaname,sim_curve_idx,stat,stat_value,stat_desc,angle_value,power_value,group_id,fn1,fn2,fn3,fn4,fn5,fn6,fn7,fn8,fn9,fn10,fn11,fn12,fn13,fn14,fn15,fn16,fn17,fn18,fn19,fn20,fn21,fn22,cn1,cn2,cn3,cn4,cn5,cn6,cn7,cn8,cn9,cn10,cn11,cn12,cn13,cn14,cn15,cn16,cn17,cn18,cn19,cn20,cn21,cn22"
#define WAM_AM_DEV_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',"VOL_INT_REC_FMT",'%s','%s','%s',%d,%f,%f,%d,%d,'%s',%d,%d,%d,'%s',%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"
#define WAM_AM_DEV_CRE_VAL(val) INT_REC_VAL(val.dev_idx),INT_REC_VAL(val.dev_id),INT_REC_VAL(val.fac_id),INT_REC_VAL(val.area_id),val.vlty_id,val.dev_name,val.fac_name,val.area_name,val.dev_type,val.r_value,val.x_value,val.is_monitor,val.is_calc_ang,val.bpaname,val.sim_curve_idx,val.stat,val.stat_value,val.stat_desc,val.angle_value,val.power_value,val.group_id,val.fn1,val.fn2,val.fn3,val.fn4,val.fn5,val.fn6,val.fn7,val.fn8,val.fn9,val.fn10,val.fn11,val.fn12,val.fn13,val.fn14,val.fn15,val.fn16,val.fn17,val.fn18,val.fn19,val.fn20,val.fn21,val.fn22,val.cn1,val.cn2,val.cn3,val.cn4,val.cn5,val.cn6,val.cn7,val.cn8,val.cn9,val.cn10,val.cn11,val.cn12,val.cn13,val.cn14,val.cn15,val.cn16,val.cn17,val.cn18,val.cn19,val.cn20,val.cn21,val.cn22
#else
#define WAM_AM_DEV_CRE_FLD "dev_id,fac_id,area_id,vlty_id,dev_name,fac_name,area_name,dev_type,r_value,x_value,is_monitor,is_calc_ang,bpaname,sim_curve_idx,stat,stat_value,stat_desc,angle_value,power_value,group_id,fn1,fn2,fn3,fn4,fn5,fn6,fn7,fn8,fn9,fn10,fn11,fn12,fn13,fn14,fn15,fn16,fn17,fn18,fn19,fn20,fn21,fn22,cn1,cn2,cn3,cn4,cn5,cn6,cn7,cn8,cn9,cn10,cn11,cn12,cn13,cn14,cn15,cn16,cn17,cn18,cn19,cn20,cn21,cn22"
#define WAM_AM_DEV_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",'%s','%s','%s',%d,%f,%f,%d,%d,'%s',%d,%d,%d,'%s',%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)"
#define WAM_AM_DEV_CRE_VAL(val) INT_REC_VAL(val.dev_id),INT_REC_VAL(val.fac_id),INT_REC_VAL(val.area_id),val.vlty_id,val.dev_name,val.fac_name,val.area_name,val.dev_type,val.r_value,val.x_value,val.is_monitor,val.is_calc_ang,val.bpaname,val.sim_curve_idx,val.stat,val.stat_value,val.stat_desc,val.angle_value,val.power_value,val.group_id,val.fn1,val.fn2,val.fn3,val.fn4,val.fn5,val.fn6,val.fn7,val.fn8,val.fn9,val.fn10,val.fn11,val.fn12,val.fn13,val.fn14,val.fn15,val.fn16,val.fn17,val.fn18,val.fn19,val.fn20,val.fn21,val.fn22,val.cn1,val.cn2,val.cn3,val.cn4,val.cn5,val.cn6,val.cn7,val.cn8,val.cn9,val.cn10,val.cn11,val.cn12,val.cn13,val.cn14,val.cn15,val.cn16,val.cn17,val.cn18,val.cn19,val.cn20,val.cn21,val.cn22
#endif // _NUSP
