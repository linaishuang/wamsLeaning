/**************************************************************************

   pubodbstruct.h: struct for the CPubOdbApi class.
  
   HISTORY :    	
   Date        Version       Author                Activities      
  ----------------------------------------------------------------------
   2011/04/02   v1.0.0       zhw                   创建
   OPEN3000和D5000表号对应整理
   2011/04/16   v1.0.1       zhw   添加WAMS前置表宏定义更新MV变压器表名称
   2011/05/04   v1.0.2       zhw   更新WAMS应用D5000表号
   移动头文件，添加测点遥测信息表和WAMS前置遥测定义表结构定义
   2011/08/23   v1.0.3       zhw   更新D5000wams接收遥测信息表(8006)
   2011/10/30   v1.0.4       zhw   更新域名,修改WFES遥测定义表
**************************************************************************/

#if !defined(_PUB_ODB_STRUCT_H_INCLUDED_)
#define _PUB_ODB_STRUCT_H_INCLUDED_

//////////////////////////////////////////////////////////////////////////
#ifdef _KEDONG
#include "db_api/odb_net_m.h"
#else
#include "db_api/odb_struct.h"
#endif // _KEDONG
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////////
#ifndef _WAMSDATA_
const	int		MAX_FRA_PER_SEC =	25;
#else
#ifdef _SIFANG
const	int		MAX_FRA_PER_SEC =	50;
#endif // _SIFANG
#endif
#ifndef AP_WAMS
#ifdef _D5000
#define         		AP_WAMS         				700000
#else
#define         		AP_WAMS         				32000
#endif
#endif

#ifdef AP_WAMS_SSO
#define				AP_WAMS_SSO					703200
#endif

#ifndef AP_WAMAP
#define         		AP_WAMAP         				AP_WAMS
#endif

#ifndef AP_WAMS_FES
#define 			AP_WAMS_FES					702100
#endif

#ifndef AF_WAMAP
#define				AF_WAMAP					AF_WAMS
#endif

#ifndef AF_WAMS_NAME
#define				AF_WAMS_NAME					"wams"
#endif

#ifndef AF_WAMS_NAME
#define				AF_WAMS_SSO_NAME				"wams_sso"
#endif

#ifndef AP_EACCS
#define AP_EACCS  AP_DSA
#endif

#ifndef PUB_AC_RESEARCH_NO
#ifdef _D5000
#define PUB_AC_RESEARCH_NO 4
#else
#define PUB_AC_RESEARCH_NO AC_RESEARCH1_NO
#endif
#endif
#ifdef _NUSP
#define 			CUR_AREA_NO       				990102 // 临时定义
#endif // _NUSP

//======== 数据类型 ======//
#ifdef _NUSP
#define		C_TYPE_ID                       		C_DATATYPE_LONG
#define     	MOD_FID_NM(f_n)                 		f_n
#define     	c_id                            		c_long
#define		INT_REC_VAL(id)					(id).FormStringID().c_str()
#define		ATOI(id)					RTDB_KEY_STRU(string(id))
#define		ITOA(id)					(id).FormStringID().c_str()
#define		ITOS(id)					(id).FormStringID()
#define     	a_to_d						atol
#define     	INT_REC						RTDB_KEY_STRU
#define     	INT_REC_FMT					"%s"
#define		ZERO_INT_REC(id)			 	((id) = RTDB_KEY_STRU())
#define		VALID_INT_REC(id)			 	((id) != RTDB_KEY_STRU())
#define		RTDB_TIME					long
#define		RTDB_TIME_FMT					"%ld"
#define		VOL_INT_REC					int
#define		VOL_INT_REC_FMT					"%d"
#else
#ifdef _D5000
#define     	C_TYPE_ID                       		C_DATATYPE_LONG
#define     	MOD_FID_NM(f_n)                 		f_n
#define     	c_id                            		c_long
#define		ZERO_INT_REC(id)				((id) = 0)
#define		VALID_INT_REC(id)				((id) != 0)
//  typedef     MODIFY_ANSWER                   		MODIFY_ANSWER_var;
	#ifdef _WINDOWS32
		#define     	a_to_d                          _atoi64
		#define     	INT_REC				__int64
		#define     	INT_REC_FMT			"%I64d"
		#define		INT_REC_VAL(id)			(id)
		#define		RTDB_TIME			__int64
		#define		RTDB_TIME_FMT			"%I64d"
		#define		VOL_INT_REC			__int64
		#define		VOL_INT_REC_FMT			"%I64d"
	#else
		#define		ATOI(id)			atol(id)
		#define     	a_to_d                          atol
		#define     	INT_REC				long
		#define     	INT_REC_FMT			"%ld"
		#define		INT_REC_VAL(id)			(id)
		#define		RTDB_TIME			long
		#define		RTDB_TIME_FMT			"%ld"
		#define		VOL_INT_REC			long
		#define		VOL_INT_REC_FMT			"%ld"
	#endif //_WINDOWS32
#else
#define     	C_TYPE_ID                       		C_DATATYPE_INT
#define     	MOD_FID_NM(f_n)                 		CORBA::string_dup(f_n)
#define     	c_id                            		c_int
#define     	a_to_d                          		atoi
#define		ATOI(id)					atoi(id)
#define     	INT_REC                                 	int
#define     	INT_REC_FMT  "%d"
#define		INT_REC_VAL(id)					(id)
#define		RTDB_TIME					int
#define		RTDB_TIME_FMT					"%d"
#define		ZERO_INT_REC(id)				((id) = 0)
#define		VALID_INT_REC(id)				((id) != 0)
#define		VOL_INT_REC					int
#define		VOL_INT_REC_FMT					"%d"
#endif // _D5000
#endif // _NUSP
#ifdef _KEDONG
#define		TABLE_NO_T		cmnUint32
#define		COLUMN_NO_T		cmnUint32
#define         ARA_SER_NO_T            cmnUint64
#endif

//===== 域名 =====//均为域内部ID
#ifdef _D5000
	#define V_A_VALUE_DF          "v_a_value"
	#define V_A_ANGLE_DF          "v_a_angle"
	#define V_B_VALUE_DF          "v_b_value"
	#define V_B_ANGLE_DF          "v_b_angle"
	#define V_C_VALUE_DF          "v_c_value"
	#define V_C_ANGLE_DF          "v_c_angle"
	#define I_A_VALUE_DF          "i_a_value"
	#define I_A_ANGLE_DF          "i_a_angle"
	#define I_B_VALUE_DF          "i_b_value"
	#define I_B_ANGLE_DF          "i_b_angle"
	#define I_C_VALUE_DF          "i_c_value"
	#define I_C_ANGLE_DF          "i_c_angle"
	#define E_VALUE_GN            "w_eea_val"  // 发电机内电势
	#define E_ANGLE_GN            "w_eep_val"  // 发电机内电势相角
	#define GN_ANGLE_GN           "w_dep_val"   // 发电机功角 域内部ID为373
	#define GN_ANGLE_CAL_GN       "w_dmp_val"  // 发电机计算功角 域内部ID为377
	#define V_A_VALUE_DF1         "w_va_mag"   // 交流线段端点表电压幅值
	#define V_A_ANGLE_DF1         "w_va_ang"
	#define V_B_VALUE_DF1         "w_vb_mag"
	#define V_B_ANGLE_DF1         "w_vb_ang"
	#define V_C_VALUE_DF1         "w_vc_mag"
	#define V_C_ANGLE_DF1         "w_vc_ang"
	#define I_A_ANGLE_DF1         "w_ia_ang"
	#define I_B_ANGLE_DF1         "w_ib_ang"
	#define I_C_ANGLE_DF1         "w_ic_ang"
	#define F_VALUE_DF            "w_f_val" // 频率
	#define V_VAL                 "v"
	#define P_VAL                 "p"
	#define Q_VAL                 "q"
	#define DC_P_VAL              "pac"
	#define DC_Q_VAL              "qac" 
	#define SSF                   "w_ssf_val"
	#define SSP                   "w_ssp_val"
#ifdef _NUSP
	#define SSO                   "w_sso_stat"
#else
	#define SSO                   "w_sso_val"
#endif // _NUSP

#else  // _D5000
	#define V_A_VALUE_DF          "v_a_value"
	#define V_A_ANGLE_DF          "v_a_angle"
	#define V_B_VALUE_DF          "v_b_value"
	#define V_B_ANGLE_DF          "v_b_angle"
	#define V_C_VALUE_DF          "v_c_value"
	#define V_C_ANGLE_DF          "v_c_angle"
	#define I_A_VALUE_DF          "i_a_value"
	#define I_A_ANGLE_DF          "i_a_angle"
	#define I_B_VALUE_DF          "i_b_value"
	#define I_B_ANGLE_DF          "i_b_angle"
	#define I_C_VALUE_DF          "i_c_value"
	#define I_C_ANGLE_DF          "i_c_angle"
	#define E_VALUE_GN            "e_value"  // 发电机内电势
	#define E_ANGLE_GN            "e_angle"  // 发电机内电势相角
	#define GN_ANGLE_GN           "gn_angle"   // 发电机功角
	#define GN_ANGLE_CAL_GN       "gn_angle_cal"  // 发电机计算功角
	#define V_A_VALUE_DF1         "v_a_value"
	#define V_A_ANGLE_DF1         "v_a_angle"
	#define V_B_VALUE_DF1         "v_b_value"
	#define V_B_ANGLE_DF1         "v_b_angle"
	#define V_C_VALUE_DF1         "v_c_value"
	#define V_C_ANGLE_DF1         "v_c_angle"
	#define I_A_VALUE_DF1         "i_a_value"
	#define I_A_ANGLE_DF1         "i_a_angle"
	#define I_B_VALUE_DF1         "i_b_value"
	#define I_B_ANGLE_DF1         "i_b_angle"
	#define I_C_VALUE_DF1         "i_c_value"
	#define I_C_ANGLE_DF1         "i_c_angle"
	#define F_VALUE_DF            "frequency" // 频率
	#define V_VAL                 "v_value"
	#define P_VAL                 "p_value"
	#define Q_VAL                 "q_value"
	#define DC_P_VAL              "acp_value"
	#define DC_Q_VAL              "acq_value"
	#define SSF                   "w_ssf_val"
	#define SSP                   "w_ssp_val"
	#define SSO                   "w_sso_val"
	#endif

#ifndef PODB_DATATYPE_INT
#define PODB_DATATYPE_INT    0
#define PODB_DATATYPE_FLOAT  1
#define PODB_DATATYPE_STRING 2
#define PODB_DATATYPE_TIME   3
#define PODB_DATATYPE_LONG   4
#endif
//////////////////////////////////////////////////////////////////////////
// 表号定义
#ifdef _NUSP
	#define VL_TYPE_NO_TAB RT_DIC_VOLTAGETYPE_NO // 电压类型表 10000
	#define TERMINAL_NO_TAB RT_SINGLETERM_NO // 单端元件表 102027
	#define DIV_INFO_NO_TAB RT_CON_PWRGRID_NO // 电网基本信息 101
	#define PLANT_INFO_NO_TAB RT_CON_PLANT_NO // 发电厂实时业务表 111
	#define SUBSTATION_INFO_NO_TAB RT_CON_SUBSTATION_NO // 变电站基本信息 112
	#define CONVERSUBSTATION_INFO_NO_TAB RT_CON_CONVERSUBSTATION_NO // 换流站实时业务表 113
	#define BAY_INFO_NO_TAB RT_CON_BAY_NO // 间隔实时业务表 121
	#define BRK_DEVICE_NO_TAB RT_DEV_BREAKER_NO // 断路器实时业务表 1321
	#define DISCR_DEVICE_NO_TAB RT_DEV_DIS_NO // 隔离开关实时业务表 1322
	#define GDDISCR_DEVICE_NO_TAB RT_DEV_GROUNDDIS_NO // 接地刀闸实时业务表 1323
	#define BS_DEVICE_NO_TAB RT_DEV_BUSBAR_NO // 母线实时业务表 1301
	#define LINE_DEVICE_NO_TAB RT_DEV_TLINE_NO // T接线表 1204
	#define ACLN_DEVICE_NO_TAB RT_DEV_ACLINE_NO // 交流线路基本信息 1201
	#define ACLN_DOT_NO_TAB RT_DEV_ACLINEEND_NO // 交流线端基本信息 1210
	#define TR_DEVICE_NO_TAB RT_DEV_PWRTRANSFM_NO // 变压器基本信息 1311
	#define TRWD_DEVICE_NO_TAB RT_DEV_TRANSFMWD_NO // 变压器绕组基本信息 1312
	#define TAP_TYPE_NO_TAB TAPTYPE_NO // 变压器抽头类型表 140916
	#define GN_DEVICE_NO_TAB RT_DEV_GENERATOR_NO // 发电机基本信息 1101
	#define LD_DEVICE_NO_TAB RT_CON_LOAD_NO // 负荷基本信息 408
	#define R_DEVICE_NO_TAB RT_DEV_SHUNTREACTOR_NO // 并联电抗器基本信息 1401
	#define C_DEVICE_NO_TAB RT_DEV_SHUNTCAPACITOR_NO // 并联电容器基本信息 1402
	#define DCSYSTEM_INFO_NO_TAB RT_CON_HVDCSYS_NO // 直流输电系统实时业务表 150
	#define DCCONV_DEVICE_NO_TAB RT_CON_CONVERTER_NO // 换流器实时业务表 154
	#define DCLN_DOT_NO_TAB RT_DEV_DCLINEEND_NO // 直流线端实时业务表 1211
	#define DCLN_PIECE_NO_TAB RT_DEV_DCLINE_NO // 直流线路基本信息 1206
	#define LFO_PARA_NO_TAB LFO_PARA_NO // LFO参数表 143000
	#define LFO_FAC_NO_TAB LFO_FAC_NO // LFO厂站表 143001
	#define LFO_GN_NO_TAB LFO_GN_NO // LFO发电机表 143002
	#define LFO_ACLN_NO_TAB LFO_ACLN_NO // LFO交流线路表 143003
	#define LFO_RT_CURVE_NO_TAB LFO_RT_CURVE_NO // LFO实时曲线表 143004
	#define LFO_ALLDEV_MODE_NO_TAB LFO_ALLDEV_MODE_NO // LFO实时设备模式全表 143005
	#define LFO_DEV_LEADMODE_NO_TAB LFO_DEV_LEADMODE_NO // LFO实时设备主导模式表 143006
	#define LFO_WDS_SHAPE_NO_TAB LFO_WDS_SHAPE_NO // LFO实时模态表 143007
	#define LFO_CASE_WDS_NO_TAB LFO_CASE_WDS_NO // LFO实时CASE窗口表 143008
	#define LFO_CASE_ALLSHAPE_NO_TAB LFO_CASE_ALLSHAPE_NO // LFO实时CASE模态全表 143009
	#define LFO_TR_NO_TAB LFO_TR_NO // LFO变压器表 143010
	#define LFO_INTERFACE_NO_TAB LFO_INTERFACE_NO // LFO断面定义表 143011
	#define LFO_INTERFACE_DEV_NO_TAB LFO_INTERFACE_DEV_NO // LFO断面组成表 143012
	#define LFO_ASST_DECI_NO_TAB LFO_ASST_DECI_NO // LFO辅助决策表 143013
	#define LFO_DECI_INDEX_NO_TAB LFO_DECI_INDEX_NO // LFO策略索引表 143014
	#define LFO_CTL_DECI_NO_TAB LFO_CTL_DECI_NO // LFO策略表 143015
	#define LFO_LPO_PARA_NO_TAB LFO_LPO_PARA_NO // LFO_LPO参数表 143016
	#define LFO_LPO_EVE_NO_TAB LFO_LPO_EVE_NO // LFO_LPO信息表 143017
	#define LFO_LPO_CASE_NO_TAB LFO_LPO_CASE_NO // LFO_LPO详细信息表 143018
	#define LFO_CTL_ACT_NO_TAB LFO_CTL_ACT_NO // LFO控制对象表 143019
	#define LFO_DEV_CURVE_TAB LFO_DEV_CURVE_NO //LFO设备振荡曲线表 143020
	#define LFO_INTFACE_NO_TAB LFO_INTFACE_NO	//LFO联络线走廊表 143024
	#define LFO_INTLINE_NO_TAB LFO_INTLINE_NO	//LFO联络线走廊与支路关系表 143025
	#define PQ_DA_CTL_PARA_NO_TAB PQ_DA_CTL_PARA_NO // VF动态评估参数表 143050
	#define PQ_DA_BUS_NO_TAB PQ_DA_BUS_NO // VF动态评估母线表 143051
	#define PQ_DA_STAT_NO_TAB PQ_DA_STAT_NO // VF动态评估状态表 143052
	#define PQ_DA_VOLT_INFO_NO_TAB PQ_DA_VOLT_INFO_NO // VF动态评估电压信息表 143053
	#define PQ_DA_FREQ_INFO_NO_TAB PQ_DA_FREQ_INFO_NO // VF动态评估频率信息表 143054
	#define PQ_DA_MAR_NO_TAB PQ_DA_MAR_NO // 动态品质评估裕度表 143055
	#define LFO_WARN_NO_TAB LFO_WARN_NO // LFO监视告警表 8690
	#define LFO_LPO_WARN_NO_TAB LFO_LPO_WARN_NO // LFO_LPO告警表 8691
	#define PQ_DA_VOLT_WARN_NO_TAB PQ_DA_VOLT_WARN_NO // 动态品质评估电压告警表 8695
	#define PQ_DA_FREQ_WARN_NO_TAB PQ_DA_FREQ_WARN_NO // 动态品质评估频率告警表 8696
	#define WAM_MV_PARA_NO_TAB WAM_MV_PARA_NO // WAM_MV参数表 8560
	#define WAM_MV_ACLN_NO_TAB WAM_MV_ACLN_NO // WAM_MV交流线路表 8561
	#define WAM_MV_TR_NO_TAB WAM_MV_TR_NO // WAM_MV变压器表 8562
	#define WAM_MV_DEVCUV_NO_TAB WAM_MV_DEVCUV_NO // WAM_MV设备曲线表 8563
	#define WAM_MV_CURVE_NO_TAB WAM_MV_CURVE_NO // WAM_MV实时曲线表 8564
	#define	WAM_AM_PARA_NO_TAB 1966 // WAM_AM_PARA_NO //	1966	/* WAM_AM参数表 */
	#define	WAM_AM_DEV_NO_TAB 1967 // WAM_AM_DEV_NO  //	1967	/* WAM_AM设备表 */
	#define	WAM_AM_CURVE_NO_TAB 1968 // WAM_AM_CURVE_NO	//1968	/* WAM_AM曲线表 */
	#define LFO_MODE_INF_NO_TAB DSA_LFO_MODE_INF_NO // 实测振荡模式表 140316
	#define LFO_DEV_INF_NO_TAB DSA_LFO_DEV_INF_NO // 实测振荡设备表 9197
	#define DSA_SSATPMUCOMP_NO_TAB DSA_SSATPMUCOMP_NO // 低频振荡小扰动分析匹配结果 9198
	#define WAMS_OSC_INFO_NO_TAB WAMS_OSC_INFO_NO // WAMS低频振荡信息表 8229
	#define DSA_N1OUTAGE_NO_TAB DSA_N1OUTAGE_NO // EACCS设备开断选择 9281
	#define DSA_N1RESULT_NO_TAB DSA_N1RESULT_NO // EACCS和EMS开断结果比较 9282
#ifdef _KEDONG
	#define WAM_FES_YC_DEFINE_NO_TAB 105606 // WAMS_RECV_ANALOG_NEW  // wams接受遥测信息表 105606
	#define WAM_FES_YX_DEFINE_NO_TAB 105607 // WAMS_RECV_POINT_NEW   // wams接受遥信信息表 105607
#else
	#define WAM_FES_YC_DEFINE_NO_TAB SG_WAM_FES_YC_DEFINE_RT_NO // WFES遥测定义表 104066	
	#define WAM_FES_YX_DEFINE_NO_TAB SG_WAM_FES_YX_DEFINE_RT_NO // WFES遥信定义表 104065
	#define WAM_COMM_FAC_INFO_NO SG_WAM_COMM_FAC_INFO_RT_NO // WFES通讯厂站表 104062
        #define WAM_CHANNEL_INFO_NO_TAB SG_WAM_CHANNEL_INFO_RT_NO // WFES通道表 104063
#endif // _KEDONG
	#define VALUE_INFO_NO_TAB VALUE_INFO_NO // 测点遥测信息表 待定
	#define WAMS_APP_WARN_INFO_NO_TAB WAMS_APP_WARN_INFO_NO // WAMS应用模块告警信息表 223382
	#define SSO_PARA_NO_TAB SSO_PARA_NO // SSO参数表 
	#define SSO_FAC_NO_TAB SSO_FAC_NO // SSO厂站表 
	#define SSO_GN_NO_TAB SSO_GN_NO // SSO发电机表 
	#define SSO_ACLN_NO_TAB SSO_ACLN_NO // SSO交流线路表 
	#define SSO_TR_NO_TAB SSO_TR_NO // SSO变压器表
	#define SSO_DEV_NO_TAB SSO_DEV_NO //SSO振荡设备表
	#define SSO_CASE_WDS_NO_TAB SSO_CASE_WDS_NO  //SSO实时CASE窗口表
	#define SSO_RT_CURVE_NO_TAB SSO_RT_CURVE_NO // SSO实时曲线表
	#define SSO_RT_CURVE2_NO_TAB SSO_RT_CURVE2_NO //SSO实时曲线横向表
#elif _D5000
	#define VL_TYPE_NO_TAB BASEVOLTAGE_NO // 电压类型表 401
	#define TERMINAL_NO_TAB SINGLETERM_NO // 单端元件表 421
	#define CMPNY_INFO_NO_TAB COMPANY_NO // 电力公司表 403
	#define DIV_INFO_NO_TAB SUBCONTROLAREA_NO // 区域表 404
	#define FAC_INFO_NO_TAB SUBSTATION_NO // 厂站表 405
	#define VL_LEVEL_NO_TAB VOLTAGELEVEL_NO // 电压等级表 402
	#define BAY_INFO_NO_TAB BAY_NO // 间隔表 406
	#define BRK_DEVICE_NO_TAB BREAKER_NO // 断路器表 407
	#define DISCR_DEVICE_NO_TAB DISCONNECTOR_NO // 刀闸表 408
	#define GDDISCR_DEVICE_NO_TAB GROUNDDISCONNECTOR_NO // 接地刀闸表 409
	#define BS_DEVICE_NO_TAB BUSBARSECTION_NO // 母线表 410
	#define LINE_DEVICE_NO_TAB ACLINE_NO // T交流线路表 413
	#define ACLN_DEVICE_NO_TAB ACLINESEGMENT_NO // 交流线段表 414
	#define ACLN_DOT_NO_TAB ACLINEEND_NO // 交流线段端点表 415
	#define TR_DEVICE_NO_TAB POWERTRANSFORMER_NO // 变压器表 416
	#define TRWD_DEVICE_NO_TAB TRANSFORMERWINDING_NO // 变压器绕组表 417
	#define TAP_TYPE_NO_TAB TAPTYPE_NO // 变压器抽头类型表 418
	#define GN_DEVICE_NO_TAB GENERATINGUNIT_NO // 发电机组表 411
	#define LD_DEVICE_NO_TAB ENERGYCONSUMER_NO // 负荷表 412
	#define RC_DEVICE_NO_TAB COMPENSATOR_P_NO // 容抗器表 419
	#define DCSYSTEM_INFO_NO_TAB DCSYSTEM_NO // 直流系统表 424
	#define DCCONV_DEVICE_NO_TAB RECTIFIERINVERTER_NO // 换流器表 425
	#define DCLN_DOT_NO_TAB DCLINEEND_NO // 直流线端表 427
	#define DCLN_PIECE_NO_TAB DCLINESEGMENT_NO // 直流线段表 426
	#define DCGROUND_TERM_NO_TAB DCGROUNDTERM_NO // 直流接地端表 428
	#define ISLANDANAABUS_NO_TAB DSA_ISLANDANAABUS_NO // 孤网组成母线 9319
	#define STCASE_TMP_NO_TAB DSA_STCASE_TMP_NO // 计算方案来源表 9125
	#define ZONELFCASE_RES_NO_TAB DSA_ZONELFCASE_RES_NO // 分区初始工况计算结果表 9135
	#define LFEMGSTATICS_NO_TAB DSA_LFEMGSTATICS_NO // 基态紧急控制统计结果表 9257
	#define EMGSTATICS_NO_TAB DSA_EMGSTATICS_NO // 紧急控制统计结果 9258
	#define PVCURVERESULT_NO_TAB DSA_PVCURVERESULT_NO // PV曲线全结果表 9235
	#define ELE_GRPUNIT_NO_TAB DSA_ELE_GRPUNIT_NO // 设备组组成表 9053
	#define ELE_GRP_NO_TAB DSA_ELE_GRP_NO // 设备组表 9052
	#define TEMPERATURERATE_NO_TAB DSA_TEMPERATURERATE_NO // 温度系数表 9064
	#define ISLANDMARGINRESULT_NO_TAB DSA_ISLANDMARGINRESULT_NO // 孤网评估结果表 9323
	#define CONTGRP_NO_TAB DSA_CONTGRP_NO // 算例组表 9031
	#define CCTCONT_RESULT_NO_TAB DSA_CCTCONT_RESULT_NO // CCT故障结果表 9236
	#define INTLINE_RES_NO_TAB DSA_INTLINE_RES_NO // 关键断面和支路关系表 9202
	#define INTERFACE_RES_NO_TAB DSA_INTERFACE_RES_NO // 关键断面表 9203
	#define LFCASEOVLDECI_NO_TAB DSA_LFCASEOVLDECI_NO // 初始过载措施结果表 9253
	#define STCASEOVLDECI_NO_TAB DSA_STCASEOVLDECI_NO // 预想过载措施结果表 9254
	#define ISLANDINTCONT_NO_TAB DSA_ISLANDINTCONT_NO // 孤网故障结果表 9320
	#define CASE_RESULT_STATICS_NO_TAB DSA_CASE_RESULT_STATICS_NO // 安全性分析统计信息表 9393
	#define FREQINTERFACE_NO_TAB DSA_FREQINTERFACE_NO // 孤网频率断面输出表 9321
	#define FREQINTLINE_NO_TAB DSA_FREQINTLINE_NO // 孤网走廊和支路关系表 9322
	#define UFV_ACTION_NO_TAB DSA_UFV_ACTION_NO // 低频低压减载装置动作表 9324
	#define GENPROC_ACTION_NO_TAB DSA_GENPROC_ACTION_NO // 发电机保护动作表 9325
	#define PRIMEINTCONT_NO_TAB DSA_PRIMEINTCONT_NO // 关键断面结果表 9201
	#define CONT_ISLAND_NO_TAB DSA_CONT_ISLAND_NO // 故障后拓扑岛表 9313
	#define SCPCDMBUSRESULT_NO_TAB DSA_SCPCDMBUSRESULT_NO // 母线短路电流辅助决策结果 9297
	#define SCLNPCDMRESULT_NO_TAB DSA_SCLNPCDMRESULT_NO // 线路短路电流辅助决策结果 9298
	#define SCHEMETYPE_NO_TAB DSA_SCHEMETYPE_NO // 周日计划结果表 9391
	#define STINTFACELN_NO_TAB DSA_STINTFACELN_NO // 极限断面络线路潮流 9241
	#define STINTERFACE_NO_TAB DSA_STINTERFACE_NO // EACCS联络线走廊结果表 9228
	#define SENRESULT_NO_TAB DSA_SENRESULT_NO // 灵敏度计算结果 9333
	#define ISLANDBUS_NO_TAB DSA_ISLANDBUS_NO // 拓扑岛母线 9311
	#define ISLANDRESULT_NO_TAB DSA_ISLANDRESULT_NO // 拓扑分析结果 9312
	#define LEADSRHELE_NO_TAB DSA_LEADSRHELE_NO // 被控设备控制前后比较结果表 9332
	#define LEADSRHGENRESULT_NO_TAB DSA_LEADSRHGENRESULT_NO // 机组进相运行结果 9334
	#define DCCTRLPAR_NO_TAB DSA_DCCTRLPAR_NO // 直流控制系统 9027
	#define SCSET_NO_TAB DSA_SCSET_NO // 短路电流计算设置 9291
	#define SCBUSRESULT_NO_TAB DSA_SCBUSRESULT_NO // 母线短路计算结果 9292
	#define SCLNRESULT_NO_TAB DSA_SCLNRESULT_NO // 线路短路计算结果 9293
	#define SCBUSADJRESULT_NO_TAB DSA_SCBUSADJRESULT_NO // 母线短路相邻元件计算结果 9294
	#define SCLNSADJRESULT_NO_TAB DSA_SCLNSADJRESULT_NO // 线路短路相邻元件计算结果 9295
	#define SCBREAKRESULT_NO_TAB DSA_SCBREAKRESULT_NO // 断路器短路电流结果 9296
	#define TIMEPROC_NO_TAB DSA_TIMEPROC_NO // 定时进程设置 9430
	#define EACCSINTPV_NO_TAB DSA_EACCSINTPV_NO // 联络线走廊PV曲线结果表 9234
	#define PVCURVE_NO_TAB DSA_PVCURVE_NO // PV曲线结果表 9233
	#define SIMUINTERFACE_NO_TAB DSA_SIMUINTERFACE_NO // 仿真EACCS联络线走廊表 9416
	#define LPSCASE_RESULT_NO_TAB DSA_LPSCASE_RESULT_NO // 进相基于故障集结果 9335
	#define LPSCONT_RESULT_NO_TAB DSA_LPSCONT_RESULT_NO // 进相基于故障算例结果 9336
	#define LPSCCGEN_RES_NO_TAB DSA_LPSCCGEN_RES_NO // 进相机群列表 9337
	#define CONTDECIDISP_NO_TAB DSA_CONTDECIDISP_NO // 故障措施显示表 9259
	#define THDUFV_ACTION_NO_TAB DSA_THDUFV_ACTION_NO // 第三道防线低频低压减载装置动作表 9364
	#define THDGENPROC_ACTION_NO_TAB DSA_THDGENPROC_ACTION_NO // 第三道防线发电机保护动作表 9365
	#define THDLINEPROC_ACTION_NO_TAB DSA_THDLINEPROC_ACTION_NO // 第三道防线线路保护动作表 9366
	#define EACCSMARLMT_NO_TAB DSA_EACCSMARLMT_NO // 系统阈值表 9067
	#define THDCONT_RESULT_NO_TAB DSA_THDCONT_RESULT_NO // 第三道防线校核结果 9412
	#define ZONE_INFO_NO_TAB DSA_ZONE_INFO_NO // 小区 9001
	#define CONT_FILTER_RULE_NO_TAB DSA_CONT_FILTER_RULE_NO // 故障筛选规则表 9055
	#define SIMUSYSPARA_NO_TAB DSA_SIMUSYSPARA_NO // 仿真系统参数设置 9411
	#define SSATMODE_NO_TAB DSA_SSATMODE_NO // 小扰动模式分析结果 9191
	#define SSATMODESHAPE_NO_TAB DSA_SSATMODESHAPE_NO // 小扰动分析模态结果 9192
	#define SIMUCURVE_NO_TAB DSA_SIMUCURVE_NO // 仿真曲线结果表 9413
	#define SIMUCURVEDISP_NO_TAB DSA_SIMUCURVEDISP_NO // 仿真曲线显示表 9414
	#define PCDMSTAINFO_NO_TAB DSA_PCDMSTAINFO_NO // 预防控制统计信息 9273
	#define EACCSINTORDER_NO_TAB DSA_EACCSINTORDER_NO // WAMAP联络线走廊排序表 9229
	#define EACCSCIRCLE_NO_TAB DSA_EACCSCIRCLE_NO // 解合环组 9341
	#define EACCSCIRCLELN_NO_TAB DSA_EACCSCIRCLELN_NO // 解合环线路组成 9342
	#define EACCSCIRCLERULE_NO_TAB DSA_EACCSCIRCLERULE_NO // 解合环方案表 9343
	#define CURDISPRECORD_NO_TAB DSA_CURDISPRECORD_NO // 当前记录表 9445
	#define PCP_PROCESSINFO_NO_TAB DSA_PCP_PROCESSINFO_NO // 监视进程信息表 9444
	#define PCP_NETWORKINFO_NO_TAB DSA_PCP_NETWORKINFO_NO // 网络状态信息表 9443
	#define LINEPROC_ACTION_NO_TAB DSA_LINEPROC_ACTION_NO // 线路保护动作表 9326
	#define EACCSECDONLINE_NO_TAB DSA_EACCSECDONLINE_NO // 安控装置在线策略 9261
	#define ACTDEVICEINFO_NO_TAB DSA_ACTDEVICEINFO_NO // 安控装置参数设置表 9260
	#define PCP_CPULOADINFO_NO_TAB DSA_PCP_CPULOADINFO_NO // CPU及内存负荷表 9442
	#define PCP_DISKINFO_NO_TAB DSA_PCP_DISKINFO_NO // 磁盘空间信息表 9441
	#define DSAS_PROCESSINFO_NO_TAB DSA_DSAS_PROCESSINFO_NO // 调度预案进程信息表 9401
	#define DSAS_RUNINFO_NO_TAB DSA_DSAS_RUNINFO_NO // 调度预案运行信息表 9402
	#define CONT_RESULT_SIMU_NO_TAB DSA_CONT_RESULT_SIMU_NO // 仿真故障算例结果 9417
	#define CASE_EXCHANGE_INFO_NO_TAB DSA_CASE_EXCHANGE_INFO_NO // CASE交互信息表 9429
	#define SSAMARCURVE_NO_TAB DSA_SSAMARCURVE_NO // 静态安全裕度曲线 9381
	#define SSAMODE_NO_TAB DSA_SSAMODE_NO // 模式分析结果 9211
	#define SSAMODEFAC_NO_TAB DSA_SSAMODEFAC_NO // 模式参与因子 9212
	#define TRANSFERMODE_NO_TAB DSA_TRANSFERMODE_NO // 传输模式 9223
	#define UFUVSHEDLOAD_NO_TAB DSA_UFUVSHEDLOAD_NO // UFUV减载结果 9367
	#define LEADSRHRULE_NO_TAB DSA_LEADSRHRULE_NO // 机组进相极限搜索规则 9331
	#define LOADGROUP_NO_TAB DSA_LOADGROUP_NO // 负荷组数据表 9002
	#define LOADGRP_LOAD_NO_TAB DSA_LOADGRP_LOAD_NO // 负荷组和负荷关系表 9003
	#define MUTUAL_DEVICE_NO_TAB DSA_MUTUAL_DEVICE_NO // 零序互感 9004
	#define UFV_DEVICE_NO_TAB DSA_UFV_DEVICE_NO // 低频低压减负荷装置 9361
	#define RU_DEVICE_NO_TAB DSA_RU_DEVICE_NO // 低频低压线路断开保护 9362
	#define GENPROT_DEVICE_NO_TAB DSA_GENPROT_DEVICE_NO // 发电机保护表 9363
	#define SCSDEV_NO_TAB DSA_SCSDEV_NO // 稳控装置定义表 9083
	#define SCSSYS_NO_TAB DSA_SCSSYS_NO // 稳控系统定义表 9081
	#define GEN_LIB_NO_TAB DSA_GEN_LIB_NO // 发电机动态参数 9011
	#define AVRE_LIB_NO_TAB DSA_AVRE_LIB_NO // 励磁E型模型参数表 9012
	#define AVRF_LIB_NO_TAB DSA_AVRF_LIB_NO // 励磁F型模型参数表 9013
	#define AVRF2_LIB_NO_TAB DSA_AVRF2_LIB_NO // 励磁F新型模型参数表 9014
	#define GOV_LIB_NO_TAB DSA_GOV_LIB_NO // 调速器和原动机系统模型表 9016
	#define PSS_LIB_NO_TAB DSA_PSS_LIB_NO // 电力系统稳定器S型 9015
	#define GENTG_LIB_NO_TAB DSA_GENTG_LIB_NO // 风功率模型参数 9017
	#define GENGV_LIB_NO_TAB DSA_GENGV_LIB_NO // 风速模型参数 9018
	#define LOAD_LIB_NO_TAB DSA_LOAD_LIB_NO // 负荷模型参数 9020
	#define MOTO_LIB_NO_TAB DSA_MOTO_LIB_NO // 感应电动机动态参数 9021
	#define DC_LIB_NO_TAB DSA_DC_LIB_NO // 直流控制模型参数表 9025
	#define DCMOD_LIB_NO_TAB DSA_DCMOD_LIB_NO // 直流调制模型参数表 9026
	#define GENGB_LIB_NO_TAB DSA_GENGB_LIB_NO // 桨矩角控制系统模型参数 9019
	#define RZ_LIB_NO_TAB DSA_RZ_LIB_NO // 线路固定串补模型参数 9022
	#define SVC_LIB_NO_TAB DSA_SVC_LIB_NO // 静止无功补偿器参数 9023
	#define DECISIONCOST_NO_TAB DSA_DECISIONCOST_NO // 措施代价表 9272
	#define MONITORLMTDEF_NO_TAB DSA_MONITORLMTDEF_NO // 考察限值表 9066
	#define ADJCONSTRAIN_NO_TAB DSA_ADJCONSTRAIN_NO // 调整约束表 9225
	#define DEVICE_MONITOR_NO_TAB DSA_DEVICE_MONITOR_NO // 考察元件表 9061
	#define VOLT_DEVIATION_NO_TAB DSA_VOLT_DEVIATION_NO // 考察电压二元表 9062
	#define FREQ_DEVIATION_NO_TAB DSA_FREQ_DEVIATION_NO // 考察频率二元表 9063
	#define PRE_DECISION_NO_TAB DSA_PRE_DECISION_NO // 预防控制搜索空间 9271
	#define LIMIT_DEFINATION_NO_TAB DSA_LIMIT_DEFINATION_NO // 限值定义列表 9065
	#define MATCH_INTERFACE_NO_TAB DSA_MATCH_INTERFACE_NO // 匹配断面表 9111
	#define MATCH_AREALINE_NO_TAB DSA_MATCH_AREALINE_NO // 匹配断面组成表 9112
	#define NODE_TYDEF_NO_TAB DSA_NODE_TYDEF_NO // 节点类型表 9113
	#define INTERFACE_NO_TAB DSA_INTERFACE_NO // WAMAP联络线走廊表 9221
	#define INTLINE_NO_TAB DSA_INTLINE_NO // WAMAP联络线走廊和支路关系表 9222
	#define LMT_DECISION_NO_TAB DSA_LMT_DECISION_NO // 极限功率搜索空间 9224
	#define PCP_MANAGENODEINFO_NO_TAB DSA_PCP_MANAGENODEINFO_NO // 管理节点运行信息表 9421
	#define PCP_COMPUTENODEINFO_NO_TAB DSA_PCP_COMPUTENODEINFO_NO // 计算节点运行信息表 9422
	#define PCP_COMMUNICATENODEINFO_NO_TAB DSA_PCP_COMMUNICATENODEINFO_NO // 通信节点运行信息表 9423
	#define PCP_ALARMINFO_NO_TAB DSA_PCP_ALARMINFO_NO // 告警信息表 9424
	#define PCP_DOMAININFO_NO_TAB DSA_PCP_DOMAININFO_NO // 工作域信息表 9425
	#define PCP_NODEPROCESSINFO_NO_TAB DSA_PCP_NODEPROCESSINFO_NO // 角色应用进程运行信息表 9426
	#define CONTINT_NO_TAB DSA_CONTINT_NO // 故障和走廊关系表 9226
	#define PCP_NODERUNSTATUSINFO_NO_TAB DSA_PCP_NODERUNSTATUSINFO_NO // 节点运行状态监视信息表 9427
	#define PCP_CLUSTERNODEINFO_NO_TAB DSA_PCP_CLUSTERNODEINFO_NO // 机群节点信息表 9428
	#define THDACTCONT_RESULT_NO_TAB DSA_THDACTCONT_RESULT_NO // 第三道防线算例统计结果表 9278
	#define LFPCDMSENELE_RES_NO_TAB DSA_LFPCDMSENELE_RES_NO // 校正控制灵敏度结果 9279
	#define PCDMSENELE_RES_NO_TAB DSA_PCDMSENELE_RES_NO // 预防控制灵敏度结果 9280
	#define BL_EVENT_NO_TAB DSA_BL_EVENT_NO // 母线及支路事件表 9034
	#define EMG_DECITION_NO_TAB DSA_EMG_DECITION_NO // 紧急控制切机或切负荷措施 9035
	#define GENEVENT_NO_TAB DSA_GENEVENT_NO // 发电机故障 9036
	#define DC_EVENT_NO_TAB DSA_DC_EVENT_NO // 直流线故障 9037
	#define SSATMODECOMP_NO_TAB DSA_SSATMODECOMP_NO // 小扰动分析控制前后比较 9193
	#define SSATPREADJUST_RES_NO_TAB DSA_SSATPREADJUST_RES_NO // 小干扰辅助决策表 9194
	#define SSATDAMPGEN_RES_NO_TAB DSA_SSATDAMPGEN_RES_NO // 小干扰振荡主导机群 9195
	#define LFPCDMSTATICS_NO_TAB DSA_LFPCDMSTATICS_NO // 初始工况辅助决策统计结果 9275
	#define LFPREADJUST_RES_NO_TAB DSA_LFPREADJUST_RES_NO // 基态辅助决策调整列表 9276
	#define FAULT_AUTOGEN_NO_TAB DSA_FAULT_AUTOGEN_NO // 在线故障生成策略 9051
	#define HISFAULT_RULE_NO_TAB DSA_HISFAULT_RULE_NO // 历史故障生成规则 9056
	#define SIMUCURVEAUTORUN_NO_TAB DSA_SIMUCURVEAUTORUN_NO // 仿真曲线实时表 9415
	#define FAULT_TIMET_NO_TAB DSA_FAULT_TIMET_NO // 故障时序表 9054
	#define CONTINGENCY_NO_TAB DSA_CONTINGENCY_NO // 算例表 9032
	#define CONT_EVENT_NO_TAB DSA_CONT_EVENT_NO // 算例和事件关系表 9033
	#define CONT_DECITION_NO_TAB DSA_CONT_DECITION_NO // 故障和候选措施集关系表 9252
	#define LFCASE_NO_TAB DSA_LFCASE_NO // 初始工况计算任务表 9131
	#define LFCASE_RES_NO_TAB DSA_LFCASE_RES_NO // 初始工况计算结果表 9132
	#define LFCASE_PARA_NO_TAB DSA_LFCASE_PARA_NO // 潮流计算参数 9123
	#define LFOVERELE_RES_NO_TAB DSA_LFOVERELE_RES_NO // 初始工况元件负载率列表 9133
	#define SENELE_RES_NO_TAB DSA_SENELE_RES_NO // 基态元件灵敏度分析 9134
	#define PREADJUST_RES_NO_TAB DSA_PREADJUST_RES_NO // 预防控制功率调整列表 9274
	#define DECITIONGRP_NO_TAB DSA_DECITIONGRP_NO // 候选措施组表 9251
	#define LFEMG_RES_NO_TAB DSA_LFEMG_RES_NO // 基态紧急控制策略结果 9256
	#define LINEFLOW_COMP_NO_TAB DSA_LINEFLOW_COMP_NO // 线路潮流比较列表 9161
	#define BUSVOLT_COMP_NO_TAB DSA_BUSVOLT_COMP_NO // 母线电压比较列表 9162
	#define MATCH_RESULT_NO_TAB DSA_MATCH_RESULT_NO // 匹配结果表 9163
	#define LMTSTATICS_NO_TAB DSA_LMTSTATICS_NO // 极限功率统计结果 9232
	#define INTADJLIST_NO_TAB DSA_INTADJLIST_NO // 断面功率调整列表 9230
	#define STCASE_NO_TAB DSA_STCASE_NO // 计算方案结果表 9122
	#define STCASE_PARA_NO_TAB DSA_STCASE_PARA_NO // 安全性分析参数信息 9124
	#define CONT_PARA_NO_TAB DSA_CONT_PARA_NO // 故障设置信息 9227
	#define STCASE_CONT_NO_TAB DSA_STCASE_CONT_NO // 稳定计算方案和故障关系表 9237
	#define STCASETSK_NO_TAB DSA_STCASETSK_NO // 计算方案输入表 9121
	#define CASE_RESULT_NO_TAB DSA_CASE_RESULT_NO // 基于故障集输出量 9171
	#define CONT_RESULT_NO_TAB DSA_CONT_RESULT_NO // 基于故障算例结果 9172
	#define EMGSENELE_RES_NO_TAB DSA_EMGSENELE_RES_NO // 紧急控制灵敏度结果 9238
	#define PCDMOVERELE_RES_NO_TAB DSA_PCDMOVERELE_RES_NO // 预防控制前后元件负载率列表 9239
	#define LFPCDMOVERELE_RES_NO_TAB DSA_LFPCDMOVERELE_RES_NO // 校正控制前后元件负载率列表 9240
	#define FAKEVB_RES_NO_TAB DSA_FAKEVB_RES_NO // 电压薄弱母线列表 9175
	#define FAKEFB_RES_NO_TAB DSA_FAKEFB_RES_NO // 频率薄弱母线列表 9176
	#define CCGEN_RES_NO_TAB DSA_CCGEN_RES_NO // 机群列表 9174
	#define DAMPMODE_RES_NO_TAB DSA_DAMPMODE_RES_NO // 振荡模式表 9177
	#define DAMPGEN_RES_NO_TAB DSA_DAMPGEN_RES_NO // 振荡主导机群 9178
	#define EMG_RES_NO_TAB DSA_EMG_RES_NO // 控制策略搜索结果 9255
	#define OVERELE_RES_NO_TAB DSA_OVERELE_RES_NO // 元件负载率列表 9173
	#define CONTSENELE_RES_NO_TAB DSA_CONTSENELE_RES_NO // 越限元件灵敏度分析结果 9231
	#define CONCONT_RES_NO_TAB DSA_CONCONT_RES_NO // 可控不可控故障列表 9277
	#define MARGINCURVE_NO_TAB DSA_MARGINCURVE_NO // 裕度曲线 9382
	#define PFRESULTCOMPARE_NO_TAB DSA_PFRESULTCOMPARE_NO // 操作前后潮流比较 9153
	#define CASE_RESULT2_NO_TAB DSA_CASE_RESULT2_NO // 基于故障集输出量二 9179
	#define MODEL_PARA_BIAS_NO_TAB DSAMODEL_PARA_BIAS_NO // 数据转换表 9394
	#define SCS_SETVALUE_RESULT_NO_TAB DSA_SCS_SETVALUE_RESULT_NO // 稳控系统定值计算结果 9098
	#define SCS_SETVALUE_ADJ_RESULT_NO_TAB DSA_SCS_SETVALUE_ADJ_RESULT_NO // 稳控系统定值计算调整结果 9099
	#define LOSS_STATICS_NO_TAB DSA_LOSS_STATICS_NO // 调整前后网损比较结果表 9262
	#define LFCASE_CMPNY_NO_TAB DSA_LFCASE_CMPNY_NO // 公司潮流统计 9152
	#define LFCASEDCLN_NO_TAB DSA_LFCASEDCLN_NO // 直流线路潮流 9151
	#define LFCASE_AREA_NO_TAB DSA_LFCASE_AREA_NO // 区域潮流信息 9141
	#define LFCASE_STAT_NO_TAB DSA_LFCASE_STAT_NO // 厂站潮流 9142
	#define LFCASEBUS_NO_TAB DSA_LFCASEBUS_NO // 母线电压结果 9144
	#define LFCASELN_NO_TAB DSA_LFCASELN_NO // 线路潮流 9143
	#define LFCASETRANSFORMER_NO_TAB DSA_LFCASETRANSFORMER_NO // 变压器潮流 9145
	#define LFCASEWIND_NO_TAB DSA_LFCASEWIND_NO // 绕组潮流 9146
	#define LFCASECAP_NO_TAB DSA_LFCASECAP_NO // 容抗器潮流结果 9147
	#define LFCASEGEN_NO_TAB DSA_LFCASEGEN_NO // 发电潮流结果 9148
	#define LFCASEINT_NO_TAB DSA_LFCASEINT_NO // 联络断面潮流 9150
	#define LFCASEINTLN_NO_TAB DSA_LFCASEINTLN_NO // 联络线路潮流 9149
	#define SCSFAC_NO_TAB DSA_SCSFAC_NO // 稳控站点表 9082
	#define SCSPLATE_NO_TAB DSA_SCSPLATE_NO // 稳控装置压板表 9084
	#define SCSINF_NO_TAB DSA_SCSINF_NO // 稳控装置断面表 9085
	#define SCSINFUNIT_NO_TAB DSA_SCSINFUNIT_NO // 稳控装置断面组成表 9086
	#define SCSOPMODE_NO_TAB DSA_SCSOPMODE_NO // 运行方式表 9087
	#define SCSSETVAL_NO_TAB DSA_SCSSETVAL_NO // 定值表 9090
	#define CALFORMULA_NO_TAB DSA_CALFORMULA_NO // 公式表 9091
	#define SCSOFFDECI_NO_TAB DSA_SCSOFFDECI_NO // 稳控主子站策略表 9092
	#define SCSSEARCHRESULT_NO_TAB DSA_SCSSEARCHRESULT_NO // 离线策略搜索结果 9100
	#define BACKOFFDECI_NO_TAB DSA_BACKOFFDECI_NO // 反馈型策略表 9093
	#define SCSCTRLDECI_NO_TAB DSA_SCSCTRLDECI_NO // 控制策略表 9094
	#define SCSACTGRP_NO_TAB DSA_SCSACTGRP_NO // 稳控动作组 9095
	#define SCSDECI_NO_TAB DSA_SCSDECI_NO // 稳控执行策略表 9096
	#define SCSORDER_NO_TAB DSA_SCSORDER_NO // 稳控动作空间 9097
	#define SCSJUDGEUNIT_NO_TAB DSA_SCSJUDGEUNIT_NO // 稳控装置判据单元表 9089
	#define SCSJUDGE_NO_TAB DSA_SCSJUDGE_NO // 稳控装置判据表 9088
	#define BATS_JUDGE_NO_TAB DSA_BATS_JUDGE_NO // 备自投装置动作判据表 9351
	#define BATSCASE_NO_TAB DSA_BATSCASE_NO // 备自投校核方案 9352
	#define BATS_ACTION_NO_TAB DSA_BATS_ACTION_NO // 备自投装置动作表 9353
	#define FLORESULTFILE_NO_TAB DSA_FLORESULTFILE_NO // 潮流结果文件信息表 9354
	#define LFSTAANGRLT_NO_TAB DSA_LFSTAANGRLT_NO // 静态功角稳定结果表 9355
	#define CDQPFRESULTCOMPARE_NO_TAB DSA_CDQPFRESULTCOMPARE_NO // 超短期预测前后潮流比较 9356
	#define CDQGENERALMES_NO_TAB DSA_CDQGENERALMES_NO // 超短期概要信息表 9357
	#define LFO_PARA_NO_TAB LFO_PARA_NO // LFO参数表 8600
	#define LFO_FAC_NO_TAB LFO_FAC_NO // LFO厂站表 8601
	#define LFO_GN_NO_TAB LFO_GN_NO // LFO发电机表 8602
	#define LFO_ACLN_NO_TAB LFO_ACLN_NO // LFO交流线路表 8603
	#define LFO_RT_CURVE_NO_TAB LFO_RT_CURVE_NO // LFO实时曲线表 8604
	#define LFO_ALLDEV_MODE_NO_TAB LFO_ALLDEV_MODE_NO // LFO实时设备模式全表 8605
	#define LFO_DEV_LEADMODE_NO_TAB LFO_DEV_LEADMODE_NO // LFO实时设备主导模式表 8606
	#define LFO_WDS_SHAPE_NO_TAB LFO_WDS_SHAPE_NO // LFO实时模态表 8607
	#define LFO_CASE_WDS_NO_TAB LFO_CASE_WDS_NO // LFO实时CASE窗口表 8608
	#define LFO_CASE_ALLSHAPE_NO_TAB LFO_CASE_ALLSHAPE_NO // LFO实时CASE模态全表 8609
	#define LFO_TR_NO_TAB LFO_TR_NO // LFO变压器表 8610
	#define LFO_INTERFACE_NO_TAB LFO_INTERFACE_NO // LFO断面定义表 8611
	#define LFO_INTERFACE_DEV_NO_TAB LFO_INTERFACE_DEV_NO // LFO断面组成表 8612
	#define LFO_ASST_DECI_NO_TAB LFO_ASST_DECI_NO // LFO辅助决策表 8613
	#define LFO_DECI_INDEX_NO_TAB LFO_DECI_INDEX_NO // LFO策略索引表 8614
	#define LFO_CTL_DECI_NO_TAB LFO_CTL_DECI_NO // LFO策略表 8615
	#define LFO_LPO_PARA_NO_TAB LFO_LPO_PARA_NO // LFO_LPO参数表 8616
	#define LFO_LPO_EVE_NO_TAB LFO_LPO_EVE_NO // LFO_LPO信息表 8617
	#define LFO_LPO_CASE_NO_TAB LFO_LPO_CASE_NO // LFO_LPO详细信息表 8618
	#define LFO_CTL_ACT_NO_TAB LFO_CTL_ACT_NO // LFO控制对象表 8619
	#define LFO_DEV_CURVE_TAB LFO_DEV_CURVE_NO //LFO设备振荡曲线表 8620
	#define LFO_WARN_NO_TAB LFO_WARN_NO // LFO监视告警表 8690
	#define LFO_LPO_WARN_NO_TAB LFO_LPO_WARN_NO // LFO_LPO告警表 8691
	#define PQ_DA_CTL_PARA_NO_TAB PQ_DA_CTL_PARA_NO // VF动态评估参数表 8650
	#define PQ_DA_BUS_NO_TAB PQ_DA_BUS_NO // VF动态评估母线表 8651
	#define PQ_DA_STAT_NO_TAB PQ_DA_STAT_NO // VF动态评估状态表 8652
	#define PQ_DA_VOLT_INFO_NO_TAB PQ_DA_VOLT_INFO_NO // VF动态评估电压信息表 8653
	#define PQ_DA_FREQ_INFO_NO_TAB PQ_DA_FREQ_INFO_NO // VF动态评估频率信息表 8654
	#define PQ_DA_MAR_NO_TAB PQ_DA_MAR_NO // 动态品质评估裕度表 8655
	#define PQ_DA_VOLT_WARN_NO_TAB PQ_DA_VOLT_WARN_NO // 动态品质评估电压告警表 8695
	#define PQ_DA_FREQ_WARN_NO_TAB PQ_DA_FREQ_WARN_NO // 动态品质评估频率告警表 8696
	#define WAM_MV_PARA_NO_TAB WAM_MV_PARA_NO // WAM_MV参数表 8560
	#define WAM_MV_ACLN_NO_TAB WAM_MV_ACLN_NO // WAM_MV交流线路表 8561
	#define WAM_MV_TR_NO_TAB WAM_MV_TR_NO // WAM_MV变压器表 8562
	#define WAM_MV_DEVCUV_NO_TAB WAM_MV_DEVCUV_NO // WAM_MV设备曲线表 8563
	#define WAM_MV_CURVE_NO_TAB WAM_MV_CURVE_NO // WAM_MV实时曲线表 8564
	#define SSO_RT_CURVE_NO_TAB SSO_RT_CURVE_NO // LFO实时曲线表 8665
	#define	WAM_AM_PARA_NO_TAB 1966 // WAM_AM_PARA_NO //	1966	/* WAM_AM参数表 */
	#define	WAM_AM_DEV_NO_TAB 1967 // WAM_AM_DEV_NO  //	1967	/* WAM_AM设备表 */
	#define	WAM_AM_CURVE_NO_TAB 1968 // WAM_AM_CURVE_NO	//1968	/* WAM_AM曲线表 */
	#define LFO_MODE_INF_NO_TAB DSA_LFO_MODE_INF_NO // 实测振荡模式表 9196
	#define LFO_DEV_INF_NO_TAB DSA_LFO_DEV_INF_NO // 实测振荡设备表 9197
	#define DSA_SSATPMUCOMP_NO_TAB DSA_SSATPMUCOMP_NO // 低频振荡小扰动分析匹配结果 9198
	#define WAMS_OSC_INFO_NO_TAB WAMS_OSC_INFO_NO // WAMS低频振荡信息表 8229
	#define DSA_N1OUTAGE_NO_TAB DSA_N1OUTAGE_NO // EACCS设备开断选择 9281
	#define DSA_N1RESULT_NO_TAB DSA_N1RESULT_NO // EACCS和EMS开断结果比较 9282
#ifdef _KEDONG_
    #define WAM_FES_YC_DEFINE_NO_TAB WAMS_RECV_ANALOG_NEW_NO // wams接收遥测信息表 8006
#else
    #define WAM_FES_YC_DEFINE_NO_TAB WAM_FES_YC_DEFINE_NO // WFES遥测定义表 8505
#endif
	#define WAM_FES_YX_DEFINE_NO_TAB WAM_FES_YX_DEFINE_NO // WFES遥测定义表 8504
	#define WAM_CHANNEL_INFO_NO_TAB WAM_CHANNEL_INFO_NO // WAMS通道表 待定
	#define VALUE_INFO_NO_TAB VALUE_INFO_NO // 测点遥测信息表 待定
    #define WAMS_APP_WARN_INFO_NO_TAB WAMS_APP_WARN_INFO_NO // WAMS应用模块告警信息表 8414
	#define PRE_PVSSAMODE_NO_TAB DSA_PRE_PVSSAMODE_NO  // 预想故障鼻点电压模式分析结果 716
	#define PRE_PVSSAMODEFAC_NO_TAB DSA_EACCS_PRE_PVSSAMODEFAC_NO  // 预想故障鼻点模式参与因子表 717
	#define EME_EACCSINTPV_NO_TAB DSA_EACCS_EACCSINTPV_NO // 联络线走廊PV曲线结果表 724
	#define EME_PVCURVE_NO_TAB DSA_EACCS_PVCURVE_NO // PV曲线结果表 723
	#define	EACCS_PVCURVERESULT_NO_TAB DSA_EACCS_PVCURVERESULT_NO  //	725	/* PV曲线全结果表 */
	#define	EACCS_EME_EACCSINTPV_NO_TAB  DSA_EACCS_EME_EACCSINTPV_NO  //	719	/* 基态联络线走廊PV曲线结果表 */
	#define	EACCS_EME_PVCURVE_NO_TAB DSA_EACCS_EME_PVCURVE_NO  //	718	/* 基态PV曲线结果表 */
	#define	EACCS_EME_PVCURVERESULT_NO_TAB DSA_EACCS_EME_PVCURVERESULT_NO  //	720	/* 基态PV曲线全结果表 */
	#define EME_PVSSAMODE_NO_TAB DSA_EACCS_EME_PVSSAMODE_NO // 基态鼻点电压模式分析结果表 721
	#define EME_PVSSAMODEFAC_NO_TAB DSA_EACCS_EME_PVSSAMODEFAC_NO  // 基态鼻点模式参与因子表 722
	#define	SCMS_SYSTEM_INFO_NO_TAB SCMS_SYSTEM_INFO_NO	//9458	/* SCMS系统信息表 */
	#define	DUTYDECI_NO_TAB DSA_DUTYDECI_NO	// 9467	/* 稳控系统当值策略表 */
	#define SSO_PARA_NO_TAB SSO_PARA_NO // SSO参数表 
	#define SSO_FAC_NO_TAB SSO_FAC_NO // SSO厂站表 
	#define SSO_GN_NO_TAB SSO_GN_NO // SSO发电机表 
	#define SSO_ACLN_NO_TAB SSO_ACLN_NO // SSO交流线路表 
	#define SSO_TR_NO_TAB SSO_TR_NO // SSO变压器表
	#define SSO_DEV_NO_TAB SSO_DEV_NO //SSO振荡设备表
	#define SSO_CASE_WDS_NO_TAB SSO_CASE_WDS_NO  //SSO实时CASE窗口表
#else
	#define VL_TYPE_NO_TAB VL_TYPE_NO // 电压类型表 200
	#define TERMINAL_NO_TAB TERMINAL_NO // 终端表 205
	#define CMPNY_INFO_NO_TAB CMPNY_INFO_NO // 公司信息表 207
	#define DIV_INFO_NO_TAB DIV_INFO_NO // 行政区域表 209
	#define FAC_INFO_NO_TAB FAC_INFO_NO // 厂站信息表 210
	#define VL_LEVEL_NO_TAB VL_LEVEL_NO // 电压等级表 211
	#define BAY_INFO_NO_TAB BAY_INFO_NO // 间隔信息表 212
	#define BRK_DEVICE_NO_TAB BRK_DEVICE_NO // 断路器信息表 220
	#define DISCR_DEVICE_NO_TAB DISCR_DEVICE_NO // 刀闸表 221
	#define GDDISCR_DEVICE_NO_TAB GDDISCR_DEVICE_NO // 接地刀闸表 222
	#define BS_DEVICE_NO_TAB BS_DEVICE_NO // 母线表 230
	#define LINE_DEVICE_NO_TAB LINE_DEVICE_NO // 线路表 239
	#define ACLN_DEVICE_NO_TAB ACLN_DEVICE_NO // 交流线段表 240
	#define ACLN_DOT_NO_TAB ACLN_DOT_NO // 交流线段端点表 242
	#define TR_DEVICE_NO_TAB TR_DEVICE_NO // 变压器表 250
	#define TRWD_DEVICE_NO_TAB TRWD_DEVICE_NO // 变压器绕组表 251
	#define TAP_TYPE_NO_TAB TAP_TYPE_NO // 变压器分接头类型表 252
	#define GN_DEVICE_NO_TAB GN_DEVICE_NO // 发电机表 260
	#define LD_DEVICE_NO_TAB LD_DEVICE_NO // 负荷表 270
	#define RC_DEVICE_NO_TAB RC_DEVICE_NO // 容抗器表 275
	#define DCSYSTEM_INFO_NO_TAB DCSYSTEM_INFO_NO // 直流系统表 320
	#define DCCONV_DEVICE_NO_TAB DCCONV_DEVICE_NO // 直流换流端定义表 321
	#define DCLN_DOT_NO_TAB DCLN_DOT_NO // 直流线端表 325
	#define DCLN_PIECE_NO_TAB DCLN_PIECE_NO // 直流导线表 326
	#define DCGROUND_TERM_NO_TAB DCGROUND_TERM_NO // 直流接地端 329
	#define ISLANDANAABUS_NO_TAB ISLANDANAABUS_NO // 孤网组成母线 823
	#define STCASE_TMP_NO_TAB STCASE_TMP_NO // 计算方案来源表 824
	#define ZONELFCASE_RES_NO_TAB ZONELFCASE_RES_NO // 分区初始工况计算结果表 825
	#define LFEMGSTATICS_NO_TAB LFEMGSTATICS_NO // 基态紧急控制统计结果表 826
	#define EMGSTATICS_NO_TAB EMGSTATICS_NO // 紧急控制统计结果 827
	#define PVCURVERESULT_NO_TAB PVCURVERESULT_NO // PV曲线全结果表 828
	#define ELE_GRPUNIT_NO_TAB ELE_GRPUNIT_NO // 设备组组成表 829
	#define ELE_GRP_NO_TAB ELE_GRP_NO // 设备组表 830
	#define TEMPERATURERATE_NO_TAB TEMPERATURERATE_NO // 温度系数表 831
	#define ISLANDMARGINRESULT_NO_TAB ISLANDMARGINRESULT_NO // 孤网评估结果表 832
	#define CONTGRP_NO_TAB CONTGRP_NO // 算例组表 833
	#define CCTCONT_RESULT_NO_TAB CCTCONT_RESULT_NO // CCT故障结果表 834
	#define INTLINE_RES_NO_TAB INTLINE_RES_NO // 关键断面和支路关系表 835
	#define INTERFACE_RES_NO_TAB INTERFACE_RES_NO // 关键断面表 836
	#define LFCASEOVLDECI_NO_TAB LFCASEOVLDECI_NO // 初始过载措施结果表 837
	#define STCASEOVLDECI_NO_TAB STCASEOVLDECI_NO // 预想过载措施结果表 838
	#define ISLANDINTCONT_NO_TAB ISLANDINTCONT_NO // 孤网故障结果表 839
	#define CASE_RESULT_STATICS_NO_TAB CASE_RESULT_STATICS_NO // 安全性分析统计信息表 840
	#define FREQINTERFACE_NO_TAB FREQINTERFACE_NO // 孤网频率断面输出表 841
	#define FREQINTLINE_NO_TAB FREQINTLINE_NO // 孤网走廊和支路关系表 842
	#define UFV_ACTION_NO_TAB UFV_ACTION_NO // 低频低压减载装置动作表 843
	#define GENPROC_ACTION_NO_TAB GENPROC_ACTION_NO // 发电机保护动作表 844
	#define PRIMEINTCONT_NO_TAB PRIMEINTCONT_NO // 关键断面结果表 845
	#define CONT_ISLAND_NO_TAB CONT_ISLAND_NO // 故障后拓扑岛表 847
	#define SCPCDMBUSRESULT_NO_TAB SCPCDMBUSRESULT_NO // 母线短路电流辅助决策结果 848
	#define SCLNPCDMRESULT_NO_TAB SCLNPCDMRESULT_NO // 线路短路电流辅助决策结果 849
	#define SCHEMETYPE_NO_TAB SCHEMETYPE_NO // 周日计划结果表 850
	#define STINTFACELN_NO_TAB STINTFACELN_NO // 极限断面络线路潮流 851
	#define STINTERFACE_NO_TAB STINTERFACE_NO // EACCS联络线走廊结果表 852
	#define SENRESULT_NO_TAB SENRESULT_NO // 灵敏度计算结果 853
	#define ISLANDBUS_NO_TAB ISLANDBUS_NO // 拓扑岛母线 855
	#define ISLANDRESULT_NO_TAB ISLANDRESULT_NO // 拓扑分析结果 856
	#define LEADSRHELE_NO_TAB LEADSRHELE_NO // 被控设备控制前后比较结果表 858
	#define LEADSRHGENRESULT_NO_TAB LEADSRHGENRESULT_NO // 机组进相运行结果 859
	#define DCCTRLPAR_NO_TAB DCCTRLPAR_NO // 直流控制系统 860
	#define SCSET_NO_TAB SCSET_NO // 短路电流计算设置 861
	#define SCBUSRESULT_NO_TAB SCBUSRESULT_NO // 母线短路计算结果 862
	#define SCLNRESULT_NO_TAB SCLNRESULT_NO // 线路短路计算结果 863
	#define SCBUSADJRESULT_NO_TAB SCBUSADJRESULT_NO // 母线短路相邻元件计算结果 864
	#define SCLNSADJRESULT_NO_TAB SCLNSADJRESULT_NO // 线路短路相邻元件计算结果 865
	#define SCBREAKRESULT_NO_TAB SCBREAKRESULT_NO // 断路器短路电流结果 866
	#define TIMEPROC_NO_TAB TIMEPROC_NO // 定时进程设置 867
	#define EACCSINTPV_NO_TAB EACCSINTPV_NO // 联络线走廊PV结果表 868
	#define PVCURVE_NO_TAB PVCURVE_NO // PV曲线结果表 869
	#define SIMUINTERFACE_NO_TAB SIMUINTERFACE_NO // 仿真EACCS联络线走廊表 870
	#define LPSCASE_RESULT_NO_TAB LPSCASE_RESULT_NO // 进相基于故障集结果 871
	#define LPSCONT_RESULT_NO_TAB LPSCONT_RESULT_NO // 进相基于故障算例结果 872
	#define LPSCCGEN_RES_NO_TAB LPSCCGEN_RES_NO // 进相机群列表 873
	#define CONTDECIDISP_NO_TAB CONTDECIDISP_NO // 故障措施显示表 874
	#define THDUFV_ACTION_NO_TAB THDUFV_ACTION_NO // 第三道防线低频低压减载装置动作表 875
	#define THDGENPROC_ACTION_NO_TAB THDGENPROC_ACTION_NO // 第三道防线发电机保护动作表 876
	#define THDLINEPROC_ACTION_NO_TAB THDLINEPROC_ACTION_NO // 第三道防线线路保护动作表 877
	#define EACCSMARLMT_NO_TAB EACCSMARLMT_NO // 系统阈值表 878
	#define THDCONT_RESULT_NO_TAB THDCONT_RESULT_NO // 第三道防线校核结果 879
	#define ZONE_INFO_NO_TAB ZONE_INFO_NO // 小区 880
	#define CONT_FILTER_RULE_NO_TAB CONT_FILTER_RULE_NO // 故障筛选规则表 892
	#define SIMUSYSPARA_NO_TAB SIMUSYSPARA_NO // 仿真系统参数设置 893
	#define SSATMODE_NO_TAB SSATMODE_NO // 小扰动模式分析结果 894
	#define SSATMODESHAPE_NO_TAB SSATMODESHAPE_NO // 小扰动分析模态结果 895
	#define SIMUCURVE_NO_TAB SIMUCURVE_NO // 仿真曲线结果表 896
	#define SIMUCURVEDISP_NO_TAB SIMUCURVEDISP_NO // 仿真曲线显示表 897
	#define PCDMSTAINFO_NO_TAB PCDMSTAINFO_NO // 预防控制统计信息 898
	#define EACCSINTORDER_NO_TAB EACCSINTORDER_NO // WAMAP联络线走廊排序表 899
	#define EACCSCIRCLE_NO_TAB EACCSCIRCLE_NO // 解合环组 900
	#define EACCSCIRCLELN_NO_TAB EACCSCIRCLELN_NO // 解合环线路组成 901
	#define EACCSCIRCLERULE_NO_TAB EACCSCIRCLERULE_NO // 解合环方案表 902
	#define CURDISPRECORD_NO_TAB CURDISPRECORD_NO // 当前记录表 903
	#define PCP_PROCESSINFO_NO_TAB PCP_PROCESSINFO_NO // 监视进程信息表 904
	#define PCP_NETWORKINFO_NO_TAB PCP_NETWORKINFO_NO // WARMAP网络状态信息表 905
	#define LINEPROC_ACTION_NO_TAB LINEPROC_ACTION_NO // 线路保护动作表 906
	#define EACCSECDONLINE_NO_TAB EACCSECDONLINE_NO // 安控装置在线策略 907
	#define ACTDEVICEINFO_NO_TAB ACTDEVICEINFO_NO // 安控装置参数设置表 908
	#define PCP_CPULOADINFO_NO_TAB PCP_CPULOADINFO_NO // WARMAP_CPU及内存负荷表 909
	#define PCP_DISKINFO_NO_TAB PCP_DISKINFO_NO // WARMAP磁盘空间信息表 910
	#define DSAS_PROCESSINFO_NO_TAB DSAS_PROCESSINFO_NO // 调度预案进程信息表 911
	#define DSAS_RUNINFO_NO_TAB DSAS_RUNINFO_NO // 调度预案运行信息表 912
	#define CONT_RESULT_SIMU_NO_TAB CONT_RESULT_SIMU_NO // 仿真故障算例结果 913
	#define CASE_EXCHANGE_INFO_NO_TAB CASE_EXCHANGE_INFO_NO // CASE交互信息表 914
	#define SSAMARCURVE_NO_TAB SSAMARCURVE_NO // 静态安全裕度曲线 916
	#define SSAMODE_NO_TAB SSAMODE_NO // 模式分析结果 917
	#define SSAMODEFAC_NO_TAB SSAMODEFAC_NO // 模式参与因子 918
	#define TRANSFERMODE_NO_TAB TRANSFERMODE_NO // 传输模式 919
	#define UFUVSHEDLOAD_NO_TAB UFUVSHEDLOAD_NO // UFUV减载结果 932
	#define LEADSRHRULE_NO_TAB LEADSRHRULE_NO // 机组进相极限搜索规则 951
	#define LOADGROUP_NO_TAB LOADGROUP_NO // 负荷组数据表 952
	#define LOADGRP_LOAD_NO_TAB LOADGRP_LOAD_NO // 负荷组和负荷关系表 953
	#define MUTUAL_DEVICE_NO_TAB MUTUAL_DEVICE_NO // 零序互感 954
	#define UFV_DEVICE_NO_TAB UFV_DEVICE_NO // 低频低压减负荷装置 955
	#define RU_DEVICE_NO_TAB RU_DEVICE_NO // 低频低压线路断开保护 956
	#define GENPROT_DEVICE_NO_TAB GENPROT_DEVICE_NO // 发电机保护表 957
	#define SCSDEV_NO_TAB SCSDEV_NO // 稳控装置定义表 958
	#define SCSSYS_NO_TAB SCSSYS_NO // 稳控系统定义表 959
	#define GEN_LIB_NO_TAB GEN_LIB_NO // 发电机动态参数 961
	#define AVRE_LIB_NO_TAB AVRE_LIB_NO // 励磁E型模型参数表 962
	#define AVRF_LIB_NO_TAB AVRF_LIB_NO // 励磁F型模型参数表 963
	#define AVRF2_LIB_NO_TAB AVRF2_LIB_NO // 励磁F新型模型参数表 964
	#define GOV_LIB_NO_TAB GOV_LIB_NO // 调速器和原动机系统模型表 965
	#define PSS_LIB_NO_TAB PSS_LIB_NO // 电力系统稳定器S型 966
	#define GENTG_LIB_NO_TAB GENTG_LIB_NO // 风功率模型参数 967
	#define GENGV_LIB_NO_TAB GENGV_LIB_NO // 风速模型参数 968
	#define LOAD_LIB_NO_TAB LOAD_LIB_NO // 负荷模型参数 969
	#define MOTO_LIB_NO_TAB MOTO_LIB_NO // 感应电动机动态参数 970
	#define DC_LIB_NO_TAB DC_LIB_NO // 直流控制模型参数表 971
	#define DCMOD_LIB_NO_TAB DCMOD_LIB_NO // 直流调制模型参数表 972
	#define GENGB_LIB_NO_TAB GENGB_LIB_NO // 桨矩角控制系统模型参数 973
	#define RZ_LIB_NO_TAB RZ_LIB_NO // 线路固定串补模型参数 974
	#define SVC_LIB_NO_TAB SVC_LIB_NO // 静止无功补偿器参数 975
	#define DECISIONCOST_NO_TAB DECISIONCOST_NO // 措施代价表 977
	#define MONITORLMTDEF_NO_TAB MONITORLMTDEF_NO // 考察限值表 978
	#define ADJCONSTRAIN_NO_TAB ADJCONSTRAIN_NO // 调整约束表 979
	#define DEVICE_MONITOR_NO_TAB DEVICE_MONITOR_NO // 考察元件表 980
	#define VOLT_DEVIATION_NO_TAB VOLT_DEVIATION_NO // 考察电压二元表 981
	#define FREQ_DEVIATION_NO_TAB FREQ_DEVIATION_NO // 考察频率二元表 982
	#define PRE_DECISION_NO_TAB PRE_DECISION_NO // 预防控制搜索空间 983
	#define LIMIT_DEFINATION_NO_TAB LIMIT_DEFINATION_NO // 限值定义列表 984
	#define MATCH_INTERFACE_NO_TAB MATCH_INTERFACE_NO // 匹配断面表 985
	#define MATCH_AREALINE_NO_TAB MATCH_AREALINE_NO // 匹配断面组成表 986
	#define NODE_TYDEF_NO_TAB NODE_TYDEF_NO // 节点类型表 987
	#define INTERFACE_NO_TAB INTERFACE_NO // WAMAP联络线走廊表 988
	#define INTLINE_NO_TAB INTLINE_NO // WAMAP联络线走廊和支路关系表 989
	#define LMT_DECISION_NO_TAB LMT_DECISION_NO // 极限功率搜索空间 990
	#define PCP_MANAGENODEINFO_NO_TAB PCP_MANAGENODEINFO_NO // 管理节点运行信息表 991
	#define PCP_COMPUTENODEINFO_NO_TAB PCP_COMPUTENODEINFO_NO // 计算节点运行信息表 992
	#define PCP_COMMUNICATENODEINFO_NO_TAB PCP_COMMUNICATENODEINFO_NO // 通信节点运行信息表 993
	#define PCP_ALARMINFO_NO_TAB PCP_ALARMINFO_NO // 告警信息表 994
	#define PCP_DOMAININFO_NO_TAB PCP_DOMAININFO_NO // 工作域信息表 995
	#define PCP_NODEPROCESSINFO_NO_TAB PCP_NODEPROCESSINFO_NO // 角色应用进程运行信息表 996
	#define CONTINT_NO_TAB CONTINT_NO // 故障和走廊关系表 997
	#define PCP_NODERUNSTATUSINFO_NO_TAB PCP_NODERUNSTATUSINFO_NO // 节点运行状态监视信息表 998
	#define PCP_CLUSTERNODEINFO_NO_TAB PCP_CLUSTERNODEINFO_NO // 机群节点信息表 999
	#define THDACTCONT_RESULT_NO_TAB THDACTCONT_RESULT_NO // 第三道防线算例统计结果表 1748
	#define LFPCDMSENELE_RES_NO_TAB LFPCDMSENELE_RES_NO // 校正控制灵敏度结果 1749
	#define PCDMSENELE_RES_NO_TAB PCDMSENELE_RES_NO // 预防控制灵敏度结果 1750
	#define BL_EVENT_NO_TAB BL_EVENT_NO // 母线及支路事件表 1751
	#define EMG_DECITION_NO_TAB EMG_DECITION_NO // 紧急控制切机或切负荷措施 1752
	#define GENEVENT_NO_TAB GENEVENT_NO // 发电机故障 1753
	#define DC_EVENT_NO_TAB DC_EVENT_NO // 直流线故障 1754
	#define SSATMODECOMP_NO_TAB SSATMODECOMP_NO // 小扰动分析控制前后比较 1755
	#define SSATPREADJUST_RES_NO_TAB SSATPREADJUST_RES_NO // 小干扰辅助决策表 1756
	#define SSATDAMPGEN_RES_NO_TAB SSATDAMPGEN_RES_NO // 小干扰振荡主导机群 1757
	#define LFPCDMSTATICS_NO_TAB LFPCDMSTATICS_NO // 初始工况辅助决策统计结果 1758
	#define LFPREADJUST_RES_NO_TAB LFPREADJUST_RES_NO // 基态辅助决策调整列表 1759
	#define FAULT_AUTOGEN_NO_TAB FAULT_AUTOGEN_NO // 在线故障生成策略 1760
	#define HISFAULT_RULE_NO_TAB HISFAULT_RULE_NO // 历史故障生成规则 1761
	#define SIMUCURVEAUTORUN_NO_TAB SIMUCURVEAUTORUN_NO // 仿真曲线实时表 1762
	#define FAULT_TIMET_NO_TAB FAULT_TIMET_NO // 故障时序表 1763
	#define CONTINGENCY_NO_TAB CONTINGENCY_NO // 算例表 1764
	#define CONT_EVENT_NO_TAB CONT_EVENT_NO // 算例和事件关系表 1765
	#define CONT_DECITION_NO_TAB CONT_DECITION_NO // 故障和候选措施集关系表 1766
	#define LFCASE_NO_TAB LFCASE_NO // 初始工况计算任务表 1767
	#define LFCASE_RES_NO_TAB LFCASE_RES_NO // 初始工况计算结果表 1768
	#define LFCASE_PARA_NO_TAB LFCASE_PARA_NO // 潮流计算参数 1769
	#define LFOVERELE_RES_NO_TAB LFOVERELE_RES_NO // 初始工况元件负载率列表 1770
	#define SENELE_RES_NO_TAB SENELE_RES_NO // 基态元件灵敏度分析 1771
	#define PREADJUST_RES_NO_TAB PREADJUST_RES_NO // 预防控制功率调整列表 1772
	#define DECITIONGRP_NO_TAB DECITIONGRP_NO // 候选措施组表 1773
	#define LFEMG_RES_NO_TAB LFEMG_RES_NO // 基态紧急控制策略结果 1774
	#define LINEFLOW_COMP_NO_TAB LINEFLOW_COMP_NO // 线路潮流比较列表 1775
	#define BUSVOLT_COMP_NO_TAB BUSVOLT_COMP_NO // 母线电压比较列表 1776
	#define MATCH_RESULT_NO_TAB MATCH_RESULT_NO // 匹配结果表 1777
	#define LMTSTATICS_NO_TAB LMTSTATICS_NO // 极限功率统计结果 1778
	#define INTADJLIST_NO_TAB INTADJLIST_NO // 断面功率调整列表 1779
	#define STCASE_NO_TAB STCASE_NO // 计算方案结果表 1780
	#define STCASE_PARA_NO_TAB STCASE_PARA_NO // 安全性分析参数信息 1781
	#define CONT_PARA_NO_TAB CONT_PARA_NO // 故障设置信息 1782
	#define STCASE_CONT_NO_TAB STCASE_CONT_NO // 稳定计算方案和故障关系表 1783
	#define STCASETSK_NO_TAB STCASETSK_NO // 计算方案输入表 1784
	#define CASE_RESULT_NO_TAB CASE_RESULT_NO // 基于故障集输出量 1785
	#define CONT_RESULT_NO_TAB CONT_RESULT_NO // 基于故障算例结果 1786
	#define EMGSENELE_RES_NO_TAB EMGSENELE_RES_NO // 紧急控制灵敏度结果 1787
	#define PCDMOVERELE_RES_NO_TAB PCDMOVERELE_RES_NO // 预防控制前后元件负载率列表 1788
	#define LFPCDMOVERELE_RES_NO_TAB LFPCDMOVERELE_RES_NO // 校正控制前后元件负载率列表 1789
	#define FAKEVB_RES_NO_TAB FAKEVB_RES_NO // 电压薄弱母线列表 1790
	#define FAKEFB_RES_NO_TAB FAKEFB_RES_NO // 频率薄弱母线列表 1791
	#define CCGEN_RES_NO_TAB CCGEN_RES_NO // 机群列表 1792
	#define DAMPMODE_RES_NO_TAB DAMPMODE_RES_NO // 振荡模式表 1793
	#define DAMPGEN_RES_NO_TAB DAMPGEN_RES_NO // 振荡主导机群 1794
	#define EMG_RES_NO_TAB EMG_RES_NO // 控制策略搜索结果 1795
	#define OVERELE_RES_NO_TAB OVERELE_RES_NO // 元件负载率列表 1796
	#define CONTSENELE_RES_NO_TAB CONTSENELE_RES_NO // 越限元件灵敏度分析结果 1797
	#define CONCONT_RES_NO_TAB CONCONT_RES_NO // 可控不可控故障列表 1798
	#define MARGINCURVE_NO_TAB MARGINCURVE_NO // 裕度曲线 1799
	#define PFRESULTCOMPARE_NO_TAB PFRESULTCOMPARE_NO // 操作前后潮流比较 2012
	#define CASE_RESULT2_NO_TAB CASE_RESULT2_NO // 基于故障集输出量二 2013
	#define MODEL_PARA_BIAS_NO_TAB MODEL_PARA_BIAS_NO // 数据转换表 2014
	#define SCS_SETVALUE_RESULT_NO_TAB SCS_SETVALUE_RESULT_NO // 稳控系统定值计算结果 2015
	#define SCS_SETVALUE_ADJ_RESULT_NO_TAB SCS_SETVALUE_ADJ_RESULT_NO // 稳控系统定值计算调整结果 2016
	#define LOSS_STATICS_NO_TAB LOSS_STATICS_NO // 调整前后网损比较结果表 2017
	#define LFCASE_CMPNY_NO_TAB LFCASE_CMPNY_NO // 公司潮流统计 2018
	#define LFCASEDCLN_NO_TAB LFCASEDCLN_NO // 直流线路潮流 2019
	#define LFCASE_AREA_NO_TAB LFCASE_AREA_NO // 区域潮流 2020
	#define LFCASE_STAT_NO_TAB LFCASE_STAT_NO // 厂站潮流 2021
	#define LFCASEBUS_NO_TAB LFCASEBUS_NO // 母线电压结果 2022
	#define LFCASELN_NO_TAB LFCASELN_NO // 线路潮流 2023
	#define LFCASETRANSFORMER_NO_TAB LFCASETRANSFORMER_NO // 变压器潮流 2024
	#define LFCASEWIND_NO_TAB LFCASEWIND_NO // 绕组潮流 2025
	#define LFCASECAP_NO_TAB LFCASECAP_NO // 容抗器潮流结果 2026
	#define LFCASEGEN_NO_TAB LFCASEGEN_NO // 发电潮流结果 2027
	#define LFCASEINT_NO_TAB LFCASEINT_NO // 断面潮流结果 2028
	#define LFCASEINTLN_NO_TAB LFCASEINTLN_NO // 联络线路潮流 2029
	#define SCSFAC_NO_TAB SCSFAC_NO // 稳控站点表 2030
	#define SCSPLATE_NO_TAB SCSPLATE_NO // 稳控装置压板表 2031
	#define SCSINF_NO_TAB SCSINF_NO // 稳控装置断面表 2032
	#define SCSINFUNIT_NO_TAB SCSINFUNIT_NO // 稳控装置断面组成表 2033
	#define SCSOPMODE_NO_TAB SCSOPMODE_NO // 运行方式表 2034
	#define SCSSETVAL_NO_TAB SCSSETVAL_NO // 定值表 2036
	#define CALFORMULA_NO_TAB CALFORMULA_NO // 公式表 2037
	#define SCSOFFDECI_NO_TAB SCSOFFDECI_NO // 稳控主子站策略表 2038
	#define SCSSEARCHRESULT_NO_TAB SCSSEARCHRESULT_NO // 离线策略搜索结果 2039
	#define BACKOFFDECI_NO_TAB BACKOFFDECI_NO // 反馈型策略表 2040
	#define SCSCTRLDECI_NO_TAB SCSCTRLDECI_NO // 控制策略表 2041
	#define SCSACTGRP_NO_TAB SCSACTGRP_NO // 稳控动作组 2042
	#define SCSDECI_NO_TAB SCSDECI_NO // 稳控执行策略表 2043
	#define SCSORDER_NO_TAB SCSORDER_NO // 稳控动作空间 2044
	#define SCSJUDGEUNIT_NO_TAB SCSJUDGEUNIT_NO // 稳控装置判据单元表 2045
	#define SCSJUDGE_NO_TAB SCSJUDGE_NO // 稳控装置判据表 2046
	#define BATS_JUDGE_NO_TAB BATS_JUDGE_NO // 备自投装置动作判据表 2047
	#define BATSCASE_NO_TAB BATSCASE_NO // 备自投校核方案 2048
	#define BATS_ACTION_NO_TAB BATS_ACTION_NO // 备自投装置动作表 2049
	#define FLORESULTFILE_NO_TAB FLORESULTFILE_NO // 潮流结果文件信息表 2050
	#define LFSTAANGRLT_NO_TAB LFSTAANGRLT_NO // 静态功角稳定结果表 2051
	#define CDQPFRESULTCOMPARE_NO_TAB CDQPFRESULTCOMPARE_NO // 超短期预测前后潮流比较 2052
	#define CDQGENERALMES_NO_TAB CDQGENERALMES_NO // 超短期概要信息表 2053
	#define LFO_PARA_NO_TAB LFO_PARA_NO // LFO参数表 1930
	#define LFO_FAC_NO_TAB LFO_FAC_NO // LFO厂站表 1931
	#define LFO_GN_NO_TAB LFO_GN_NO // LFO发电机表 1932
	#define LFO_ACLN_NO_TAB LFO_ACLN_NO // LFO交流线路表 1933
	#define LFO_RT_CURVE_NO_TAB LFO_RT_CURVE_NO // LFO实时曲线表 1934
	#define LFO_ALLDEV_MODE_NO_TAB LFO_ALLDEV_MODE_NO // LFO实时设备模式全表 1935
	#define LFO_DEV_LEADMODE_NO_TAB LFO_DEV_LEADMODE_NO // LFO实时设备主导模式表 1936
	#define LFO_WDS_SHAPE_NO_TAB LFO_WDS_SHAPE_NO // LFO实时模态表 1937
	#define LFO_CASE_WDS_NO_TAB LFO_CASE_WDS_NO // LFO实时CASE窗口表 1938
	#define LFO_CASE_ALLSHAPE_NO_TAB LFO_CASE_ALLSHAPE_NO // LFO实时CASE模态全表 1939
	#define LFO_TR_NO_TAB LFO_TR_NO // LFO变压器表 1940
	#define LFO_INTERFACE_NO_TAB LFO_INTERFACE_NO // LFO断面定义表 1941
	#define LFO_INTERFACE_DEV_NO_TAB LFO_INTERFACE_DEV_NO // LFO断面组成表 1942
	#define LFO_ASST_DECI_NO_TAB LFO_ASST_DECI_NO // LFO辅助决策表 1943
	#define LFO_DECI_INDEX_NO_TAB LFO_DECI_INDEX_NO // LFO策略索引表 1944
	#define LFO_CTL_DECI_NO_TAB LFO_CTL_DECI_NO // LFO策略表 1945
	#define LFO_LPO_PARA_NO_TAB LFO_LPO_PARA_NO // LFO_LPO参数表 1946
	#define LFO_LPO_EVE_NO_TAB LFO_LPO_EVE_NO // LFO_LPO信息表 1947
	#define LFO_LPO_CASE_NO_TAB LFO_LPO_CASE_NO // LFO_LPO详细信息表 1948
	#define LFO_CTL_ACT_NO_TAB LFO_CTL_ACT_NO // LFO控制对象表 1949
	#define LFO_WARN_NO_TAB LFO_WARN_NO // LFO监视告警表 2315
	#define LFO_LPO_WARN_NO_TAB LFO_LPO_WARN_NO // LFO_LPO告警表 2316
	#define PQ_DA_CTL_PARA_NO_TAB PQ_DA_CTL_PARA_NO // VF动态评估参数表 1950
	#define PQ_DA_BUS_NO_TAB PQ_DA_BUS_NO // VF动态评估母线表 1951
	#define PQ_DA_STAT_NO_TAB PQ_DA_STAT_NO // VF动态评估状态表 1952
	#define PQ_DA_VOLT_INFO_NO_TAB PQ_DA_VOLT_INFO_NO // VF动态评估电压信息表 1954
	#define PQ_DA_FREQ_INFO_NO_TAB PQ_DA_FREQ_INFO_NO // VF动态评估频率信息表 1955
	#define PQ_DA_MAR_NO_TAB PQ_DA_MAR_NO // 动态品质评估裕度表 1956
	#define PQ_DA_VOLT_WARN_NO_TAB PQ_DA_VOLT_WARN_NO // 动态品质评估电压告警表 2313
	#define PQ_DA_FREQ_WARN_NO_TAB PQ_DA_FREQ_WARN_NO // 动态品质评估频率告警表 2314
	#define WAM_MV_PARA_NO_TAB WAM_MV_PARA_NO // WAM_MV参数表 1960
	#define WAM_MV_ACLN_NO_TAB WAM_MV_ACLN_NO // WAM_MV交流线路表 1961
	#define WAM_MV_TR_NO_TAB WAM_MV_TR_NO // WAM_MV变压器表 1962
	#define WAM_MV_DEVCUV_NO_TAB WAM_MV_DEVCUV_NO // WAM_MV设备曲线表 1963
	#define WAM_MV_CURVE_NO_TAB WAM_MV_CURVE_NO // WAM_MV实时曲线表 1964
	#define	WAM_AM_PARA_NO_TAB 1966 //WAM_AM_PARA_NO //	1966	/* WAM_AM参数表 */
	#define	WAM_AM_DEV_NO_TAB 1967 //WAM_AM_DEV_NO  //	1967	/* WAM_AM设备表 */
	#define	WAM_AM_CURVE_NO_TAB 1968  //WAM_AM_CURVE_NO	//1968	/* WAM_AM曲线表 */
	#define LFO_MODE_INF_NO_TAB LFO_MODE_INF_NO // 实测振荡模式表 待定
	#define LFO_DEV_INF_NO_TAB LFO_DEV_INF_NO // 实测振荡设备表 待定
	#define DSA_SSATPMUCOMP_NO_TAB DSA_SSATPMUCOMP_NO // 低频振荡小扰动分析匹配结果 待定
	#define WAMS_OSC_INFO_NO_TAB WAMS_OSC_INFO_NO // WAMS低频振荡信息表 待定
	#define DSA_N1OUTAGE_NO_TAB DSA_N1OUTAGE_NO // EACCS设备开断选择 待定
	#define DSA_N1RESULT_NO_TAB DSA_N1RESULT_NO // EACCS和EMS开断结果比较 待定
	#define WAM_FES_YC_DEFINE_NO_TAB WAM_FES_YC_DEFINE_NO // WAMS前置遥测定义表 1813
	#define WAM_CHANNEL_INFO_NO_TAB WAM_CHANNEL_INFO_NO // WAMS通道表 612
	#define VALUE_INFO_NO_TAB VALUE_INFO_NO // 测点遥测信息表 307
    #define WAMS_APP_WARN_INFO_NO_TAB APP_VARIABLE_NO // 应用变量表 190
	#define PRE_PVSSAMODE_NO_TAB PRE_PVSSAMODE_NO  // 预想故障鼻点电压模式分析结果 716
	#define PRE_PVSSAMODEFAC_NO_TAB PRE_PVSSAMODEFAC_NO  // 预想故障鼻点模式参与因子表 717
	#define	PVCURVERESULT_NO_TAB PVCURVERESULT_NO  //	828	/* PV曲线全结果表 */
	#define	EME_EACCSINTPV_NO_TAB  EME_EACCSINTPV_NO  //	719	/* 基态联络线走廊PV曲线结果表 */
	#define	EME_PVCURVE_NO_TAB EME_PVCURVE_NO  //	718	/* 基态PV曲线结果表 */
	#define	EME_PVCURVERESULT_NO_TAB EME_PVCURVERESULT_NO  //	720	/* 基态PV曲线全结果表 */
	#define EME_PVSSAMODE_NO_TAB EME_PVSSAMODE_NO // 基态鼻点电压模式分析结果表 721
	#define EME_PVSSAMODEFAC_NO_TAB EME_PVSSAMODEFAC_NO  // 基态鼻点模式参与因子表 722
	#define	SCMS_SYSTEM_INFO_NO_TAB SCMS_SYSTEM_INFO_NO	//9458	/* SCMS系统信息表 */
	#define	DUTYDECI_NO_TAB DSA_DUTYDECI_NO	// 9467	/* 稳控系统当值策略表 */
#endif

#ifdef _WINDOWS32
struct KEY_STRU
{
	int key;
	short field_id;
	short table_no;
};
#endif
//////////////////////////////////////////////////////////////////////////
// 采样信息结构
typedef struct type_TRIGGER_SAMPLE_BASE
{
	INT_REC keyid;  // ID号
	int record_id;  // 记录号(open3000)
	int column_id;  // 域号(open3000)
	int vtype;  // 值类型
	INT_REC vint;  // 整形值
	float vfloat;  // 浮点值
	char vstring[128];  // 字符串值(只支持128位字符)
}TRIGGER_SAMPLE_BASE;

typedef struct type_TRIGGER_SAMPLE_DEFINE
{
	int tableno; // 表号
	int sampleno;  // 采样号
	int appno;  // 应用号
	int mode;  // 0,域级采样;1,非域级采样
	RTDB_TIME occur_time;  // 数据时刻
	vector<TRIGGER_SAMPLE_BASE> vecsmp;
}TRIGGER_SAMPLE_DEFINE;
//////////////////////////////////////////////////////////////////////////
//告警信息结构
typedef struct type_WARN_MESSAGE_BASE
{
	int vtype;  // 值类型
	RTDB_TIME vtime;  // 时间
#ifdef _D5000
	INT_REC vlong;  // 长整形值
#endif
	int vint;  // 整形值
	float vfloat;  // 浮点值
	char vstring[200];  // 字符串值(只支持200位字符)
}WARN_MESSAGE_BASE;

typedef struct type_WARN_MESSAGE_STRUCT
{
	int warntype; // 告警号
	int appno;  // 应用号
	vector<WARN_MESSAGE_BASE> vecwarn;
}WARN_MESSAGE_STRUCT;
//////////////////////////////////////////////////////////////////////////
// 公共设备表
/* 电压类型表 */
typedef struct type_VL_TYPE_TAB_CRE
{
	VOL_INT_REC vlty_id;	/* 电压类型ID号 */
#ifdef _NUSP
#ifdef _KEDONG
	char vlty_name[64];     /* 电压类型名称 */
#else
	char vlty_name[128];	/* 电压类型名称 */
#endif // _KEDONG
#else
	char vlty_name[64];	/* 电压类型名称 */
#endif // _NUSP
	float vbase;	/* 电压基值 */
}VL_TYPE_TAB_CRE;
#ifndef _D5000
#define VL_TYPE_TAB_CRE_FLD "vlty_id,vlty_name,vbase"
#else // _D5000
	#ifdef _NUSP
		#ifdef _KEDONG
			#define VL_TYPE_TAB_CRE_FLD "id,name,nomvol"
		#else
			#define VL_TYPE_TAB_CRE_FLD "code,name,nomvol"
		#endif // _KEDONG
	#else
		#define VL_TYPE_TAB_CRE_FLD "id,name,nomvol"
	#endif // _NUSP
#endif // _D5000

// 发电机表
typedef struct type_GEN_TAB_CRE
{
	INT_REC        gn_id;                                       //  发电机ID号
	char           gn_name[64];                                 //  发电机名称   
	INT_REC        fac_id;                                      //  厂站ID号
	VOL_INT_REC   vlty_id;	/* 电压类型ID号 */
#ifdef _NUSP
	int		gn_type; /* 发电机类型 */
#else
	unsigned char gn_type;	/* 发电机类型 */
#endif // _NUSP
	float pmax;	/* 机组有功上限 */
	float pmin;	/* 机组有功下限 */
#ifdef _NUSP
	//float pnom;	/* 机组额定出力 */
#else
	float pnom;	/* 机组额定出力 */
#endif // _NUSP	

}GEN_TAB_CRE;
#ifndef _D5000                         
#define GEN_TAB_CRE_FLD "gn_id,gn_name,fac_id,vlty_id,gn_type,pmax,pmin,pnom"
#else
	#ifdef _NUSP
		#define GEN_TAB_CRE_FLD "id,name,st_id,voltage_type,gn_type,p_max,p_min"
	#else
		#define GEN_TAB_CRE_FLD "id,name,st_id,bv_id,gn_type,pmax,pmin,pnom"
	#endif // _NUSP
#endif // _D5000

// 交流线段端点表
typedef struct type_ACLNDOT_TAB_CRE
{
	INT_REC        acln_dot_id;                                 //  交流线段端点ID号
#ifdef _NUSP
#ifdef _KEDONG
	char           acln_dot_name[64];                           //  交流线段端点名称
#else
	char		   acln_dot_name[128];                           //  交流线段端点名称
#endif // _KEDONG
#else
	char		   acln_dot_name[64];                           //  交流线段端点名称
#endif // _NUSP	
	INT_REC        fac_id;                                      //  厂站ID号
	INT_REC        acln_id;                                     //  所属交流线段ID号
}ACLNDOT_TAB_CRE;
#ifndef _D5000
#define ACLNDOT_TAB_CRE_FLD "acln_dot_id,acln_dot_name,fac_id,acln_id"
#else
	#ifdef _NUSP
		#ifdef _KEDONG
			#define ACLNDOT_TAB_CRE_FLD "id,name,st_id,aline_id"
		#else
			#define ACLNDOT_TAB_CRE_FLD "id,name,st_id,line_id"
		#endif // _KEDONG
	#else
		#define ACLNDOT_TAB_CRE_FLD "id,name,st_id,aclnseg_id"
	#endif // _NUSP
#endif // _D5000

// 交流线段表
typedef struct type_ACLINE_TAB_CRE
{
	INT_REC        acln_id;                                     //  交流线段ID号
	char           acln_name[64];                               //  交流线段名称
	INT_REC        fac_id_s;                                    //  一端厂站ID号
	INT_REC        fac_id_e;                                    //  二端厂站ID号
	VOL_INT_REC    vlty_id;	                                   //  电压类型ID号
	float			imax1;									/* 电流上限 */
}ACLINE_TAB_CRE;
#ifndef _D5000
#define ACLINE_TAB_CRE_FLD "acln_id,acln_name,fac_id_s,fac_id_e,vlty_id,imax1"
#else
	#ifdef _NUSP
		#define ACLINE_TAB_CRE_FLD "id,name,start_st_id,end_st_id,voltage_type,imax"
	#else
		#define ACLINE_TAB_CRE_FLD "id,name,ist_id,jst_id,bv_id,amprating"
	#endif // _NUSP
#endif // _D5000

// 母线表
typedef struct  type_BUS_TAB_CRE
{
	INT_REC           bs_id;                                    //  母线ID
	char              bs_name[64];                              //  母线名称
	INT_REC           fac_id;                                   //  厂站ID
	VOL_INT_REC       vlty_id;	                                   //  电压类型ID号
	int               bs_type;                                  //  母线类型
}BUS_TAB_CRE;
#ifndef _D5000
#define BUS_TAB_CRE_FLD "bs_id,bs_name,fac_id,vlty_id,bs_type"
#else
	#ifdef _NUSP
		#define BUS_TAB_CRE_FLD "id,name,st_id,voltage_type,busbartype"
	#else
		#define BUS_TAB_CRE_FLD "id,name,st_id,bv_id,bs_type"
	#endif // _NUSP
#endif // _D5000

// 厂站信息表
#ifdef _NUSP
// 发电厂基本信息表
typedef struct type_PLANT_TAB_CRE
{
	INT_REC        fac_id;                                      //  电厂ID
	char           fac_name[64];                                //  发电厂名称
	VOL_INT_REC    vlty_id;					//  电压等级
	INT_REC        area_id;                                     //  接入电网
	int            fac_type;                                    //  厂站类型
}PLANT_TAB_CRE;
#ifdef _KEDONG
#define PLANT_TAB_CRE_FLD "id,name,max_voltage_type,connective_pg_id,plant_type"
#else
#define PLANT_TAB_CRE_FLD "id,name,voltage_type,pg_id,st_type"
#endif // _KEDONG
// 变电站基本信息表
typedef struct type_SUBSTATION_TAB_CRE
{
	INT_REC        fac_id;                                      //  变电站ID
	char           fac_name[64];                                //  变电站名称
	VOL_INT_REC    vlty_id;					//  交流最高电压等级
	INT_REC        area_id;                                     //  接入电网
	int            fac_type;                                    //  厂站类型
}SUBSTATION_TAB_CRE;
#ifdef _KEDONG
#define SUBSTATION_TAB_CRE_FLD "id,name,top_ac_voltage_type,pg_id,type"
#else
#define SUBSTATION_TAB_CRE_FLD "id,name,voltage_type,pg_id,st_type"
#endif // _KEDONG

typedef struct type_FAC_TAB_CRE
{
	INT_REC        fac_id;                                      //  厂站ID号
	char           fac_name[64];                                //  厂站名称
	VOL_INT_REC    vlty_id;                                       /* 最高电压类型ID */
	INT_REC        area_id;                                     //  区域ID
	int            fac_type;                                    //  厂站类型
}FAC_TAB_CRE;
#define FAC_TAB_CRE_FLD "fac_id,fac_name,vlty_id,pg_id,fac_type"
#else
typedef struct type_FAC_TAB_CRE
{
	INT_REC        fac_id;                                      //  厂站ID号
	char           fac_name[64];                                //  厂站名称
	VOL_INT_REC    vlty_id;                                       /* 最高电压类型ID */
	INT_REC        area_id;                                     //  区域ID
	int            fac_type;                                    //  厂站类型
}FAC_TAB_CRE;
#ifndef _D5000
#define FAC_TAB_CRE_FLD "fac_id,fac_name,vlty_id,area_id,fac_type"
#else
#define FAC_TAB_CRE_FLD "id,name,bv_id,subarea_id,st_type"
#endif // _D5000
#endif // _NUSP

//行政区域表  /* 电网基本信息 */
typedef struct  type_AREA_TAB_CRE
{
	INT_REC        area_id;                                     //  区域ID
#ifdef _NUSP
#ifdef _KEDONG
	char		area_name[64];					// 区域名称
#else
	char           area_name[128];                               //  区域名称
#endif // _KEDONG
#else
	char           area_name[64];                               //  区域名称
#endif // _NUSP	
}AREA_TAB_CRE;
#ifndef _D5000
#define AREA_TAB_CRE_FLD "area_id,area_name"
#else
	#ifdef _NUSP
		#define AREA_TAB_CRE_FLD "id,name"
	#else
		#define AREA_TAB_CRE_FLD "id,name"
	#endif // _NUSP
#endif // _D5000

/* 变压器表 */
typedef struct type_TR_TAB_CRE
{
	INT_REC tr_id;	/* 变压器ID号 */
	char tr_name[64];	/* 变压器名称 */
	INT_REC fac_id;	/* 厂站ID号 */
#ifdef _NUSP
	int trty;                              /* 绕组类型 */
	//unsigned char tr_type;		/* 变压器类型 */
#else
	unsigned char trty;                   /* 绕组类型 */
	unsigned char tr_type;	/* 变压器类型 */
#endif // _NUSP		
}TR_TAB_CRE;
#ifndef _D5000
#define TR_TAB_CRE_FLD "tr_id,tr_name,fac_id,trty,tr_type"
#else
	#ifdef _NUSP
		#define TR_TAB_CRE_FLD "id,name,st_id,wind_type"
	#else
		#define TR_TAB_CRE_FLD "id,name,st_id,wind_type,tr_type"
	#endif // _NUSP	
#endif // _D5000

/* 变压器绕组表 */
typedef struct type_TRWD_TAB_CRE
{
	INT_REC trwd_id;	/* 变压器绕组ID号 */
	char trwd_name[64];	/* 变压器绕组名称 */
	INT_REC fac_id;	/* 厂站ID号 */
	INT_REC tr_id;	/* 变压器ID号 */
	VOL_INT_REC vlty_id;	/* 电压类型ID号 */
#ifdef _NUSP
	//INT_REC vlle_id;	/* 电压等级ID号 */
	int trwd_type;        /* 变压器绕组类型 */
#else
	INT_REC vlle_id;	/* 电压等级ID号 */
	unsigned char trwd_type;        /* 变压器绕组类型 */
#endif // _NUSP		
	float	snom;		/* 绕组正常额定功率 */
}TRWD_TAB_CRE;
#ifndef _D5000
#define TRWD_TAB_CRE_FLD "trwd_id,trwd_name,fac_id,tr_id,vlty_id,vlle_id,trwd_type,snom"
#else
	#ifdef _NUSP
		#define TRWD_TAB_CRE_FLD "id,name,st_id,transfm_id,voltage_type,wind_type,mvanom"
	#else
		#define TRWD_TAB_CRE_FLD "id,name,st_id,tr_id,bv_id,vl_id,wind_type,mvanom"
	#endif // _NUSP		
#endif // _D5000

/* 测点遥测信息表 */
typedef struct type_VALUEINFO_TAB_CRE
{
	INT_REC value_id;	/* 测点ID号 */
	INT_REC fac_id;	/* 厂站ID号 */
	INT_REC value_name[64];	/* 测点名称 */
	unsigned char value_type;	/* 类型 */
}VALUEINFO_TAB_CRE;
#ifndef _D5000
#define VALUEINFO_TAB_CRE_FLD "value_id,fac_id,value_name,value_type"
#else  //D5000没有读取这个表
#define VALUEINFO_TAB_CRE_FLD "value_id,fac_id,value_name,value_type"
#endif

// WAMS前置遥测定义表
#ifndef _D5000
typedef struct  type_FES_YC_TAB_CRE
{
	KEY_ID_STRU     yc_id;               //遥测ID
	int		fac_id;		     /* 厂站ID */
	short           dot_no;              //点号
	int             file_no;             //采样存储文件号
	int             col_no;              //采样文件列号
}FES_YC_TAB_CRE;
#define FES_YC_TAB_CRE_FLD "yc_id,fac_id,dot_no,file_no,col_no"

#else

#ifdef _KEDONG
typedef struct  type_FES_YC_TAB_CRE
{
    INT_REC     yc_id;               //数据点名
    INT_REC     fac_id;              //厂站名
    int			dot_no;				// 数据点号
    int			file_no;             // PMU测点实时库地址
    int			col_no;              // 备用(PMU测点实时库地址)
}FES_YC_TAB_CRE;
#define FES_YC_TAB_CRE_FLD "psid,st_id,localno,record_id,record_id"
// WAMS前置遥信定义表
typedef struct  type_FES_YX_TAB_CRE
{
	INT_REC         st_id;                /* 厂站ID */
	INT_REC         yx_id;               //遥信ID
	int				dot_no;              //点号
	int             file_no;             //采样存储文件号
	int             col_no;              //采样文件列号
}FES_YX_TAB_CRE;
#define FES_YX_TAB_CRE_FLD "st_id,psid,localno,record_id,record_id"
#else
typedef struct  type_FES_YC_TAB_CRE
{
	INT_REC			yc_id;				//遥测ID
	INT_REC			fac_id;				//厂站ID
	short			dot_no;				//点号
	int				file_no;			//采样存储文件号
	int				col_no;				//采样文件列号
}FES_YC_TAB_CRE;
#define FES_YC_TAB_CRE_FLD "yc_id,st_id,dot_no,file_no,col_no"
// WAMS前置遥信定义表
typedef struct  type_FES_YX_TAB_CRE
{
	INT_REC         st_id;				 /* 厂站ID */
	INT_REC         yx_id;               //遥信ID
	short           dot_no;              //点号
	int             file_no;             //采样存储文件号
	int             col_no;              //采样文件列号
}FES_YX_TAB_CRE;
#define FES_YX_TAB_CRE_FLD "st_id,yx_id,dot_no,file_no,col_no"
#endif // _KEDONG

// WAMS前置遥测遥信合并结构体
typedef struct  type_FES_YX_YC
{
	INT_REC				st_id;			  /* 厂站ID */
   	INT_REC				yc_id;             //遥测遥信合并INT
    short				dot_no;              //点号
	int					file_no;             //采样存储文件号
    int					col_no;              //采样文件列号
}FES_YX_YC;

// WAMS前置通讯厂站
typedef struct  type_FES_COMM
{
	INT_REC comm_fac_id;	/* 通讯厂站ID */
	INT_REC st_id;	/* 厂站ID */
	char fac_name[64];	/* 通讯厂站名称 */
}FES_COMM_TAB_CRE;
#define FES_COMM_TAB_CRE_FLD "comm_fac_id,st_id,fac_name"

#endif // !_D5000
//////////////////////////////////////////////////////////////////////////
// 故障表

#endif
