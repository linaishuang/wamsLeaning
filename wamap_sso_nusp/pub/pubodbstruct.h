/**************************************************************************

   pubodbstruct.h: struct for the CPubOdbApi class.
  
   HISTORY :    	
   Date        Version       Author                Activities      
  ----------------------------------------------------------------------
   2011/04/02   v1.0.0       zhw                   ����
   OPEN3000��D5000��Ŷ�Ӧ����
   2011/04/16   v1.0.1       zhw   ���WAMSǰ�ñ�궨�����MV��ѹ��������
   2011/05/04   v1.0.2       zhw   ����WAMSӦ��D5000���
   �ƶ�ͷ�ļ�����Ӳ��ң����Ϣ���WAMSǰ��ң�ⶨ���ṹ����
   2011/08/23   v1.0.3       zhw   ����D5000wams����ң����Ϣ��(8006)
   2011/10/30   v1.0.4       zhw   ��������,�޸�WFESң�ⶨ���
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
#define 			CUR_AREA_NO       				990102 // ��ʱ����
#endif // _NUSP

//======== �������� ======//
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

//===== ���� =====//��Ϊ���ڲ�ID
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
	#define E_VALUE_GN            "w_eea_val"  // ������ڵ���
	#define E_ANGLE_GN            "w_eep_val"  // ������ڵ������
	#define GN_ANGLE_GN           "w_dep_val"   // ��������� ���ڲ�IDΪ373
	#define GN_ANGLE_CAL_GN       "w_dmp_val"  // ��������㹦�� ���ڲ�IDΪ377
	#define V_A_VALUE_DF1         "w_va_mag"   // �����߶ζ˵���ѹ��ֵ
	#define V_A_ANGLE_DF1         "w_va_ang"
	#define V_B_VALUE_DF1         "w_vb_mag"
	#define V_B_ANGLE_DF1         "w_vb_ang"
	#define V_C_VALUE_DF1         "w_vc_mag"
	#define V_C_ANGLE_DF1         "w_vc_ang"
	#define I_A_ANGLE_DF1         "w_ia_ang"
	#define I_B_ANGLE_DF1         "w_ib_ang"
	#define I_C_ANGLE_DF1         "w_ic_ang"
	#define F_VALUE_DF            "w_f_val" // Ƶ��
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
	#define E_VALUE_GN            "e_value"  // ������ڵ���
	#define E_ANGLE_GN            "e_angle"  // ������ڵ������
	#define GN_ANGLE_GN           "gn_angle"   // ���������
	#define GN_ANGLE_CAL_GN       "gn_angle_cal"  // ��������㹦��
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
	#define F_VALUE_DF            "frequency" // Ƶ��
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
// ��Ŷ���
#ifdef _NUSP
	#define VL_TYPE_NO_TAB RT_DIC_VOLTAGETYPE_NO // ��ѹ���ͱ� 10000
	#define TERMINAL_NO_TAB RT_SINGLETERM_NO // ����Ԫ���� 102027
	#define DIV_INFO_NO_TAB RT_CON_PWRGRID_NO // ����������Ϣ 101
	#define PLANT_INFO_NO_TAB RT_CON_PLANT_NO // ���糧ʵʱҵ��� 111
	#define SUBSTATION_INFO_NO_TAB RT_CON_SUBSTATION_NO // ���վ������Ϣ 112
	#define CONVERSUBSTATION_INFO_NO_TAB RT_CON_CONVERSUBSTATION_NO // ����վʵʱҵ��� 113
	#define BAY_INFO_NO_TAB RT_CON_BAY_NO // ���ʵʱҵ��� 121
	#define BRK_DEVICE_NO_TAB RT_DEV_BREAKER_NO // ��·��ʵʱҵ��� 1321
	#define DISCR_DEVICE_NO_TAB RT_DEV_DIS_NO // ���뿪��ʵʱҵ��� 1322
	#define GDDISCR_DEVICE_NO_TAB RT_DEV_GROUNDDIS_NO // �ӵص�բʵʱҵ��� 1323
	#define BS_DEVICE_NO_TAB RT_DEV_BUSBAR_NO // ĸ��ʵʱҵ��� 1301
	#define LINE_DEVICE_NO_TAB RT_DEV_TLINE_NO // T���߱� 1204
	#define ACLN_DEVICE_NO_TAB RT_DEV_ACLINE_NO // ������·������Ϣ 1201
	#define ACLN_DOT_NO_TAB RT_DEV_ACLINEEND_NO // �����߶˻�����Ϣ 1210
	#define TR_DEVICE_NO_TAB RT_DEV_PWRTRANSFM_NO // ��ѹ��������Ϣ 1311
	#define TRWD_DEVICE_NO_TAB RT_DEV_TRANSFMWD_NO // ��ѹ�����������Ϣ 1312
	#define TAP_TYPE_NO_TAB TAPTYPE_NO // ��ѹ����ͷ���ͱ� 140916
	#define GN_DEVICE_NO_TAB RT_DEV_GENERATOR_NO // �����������Ϣ 1101
	#define LD_DEVICE_NO_TAB RT_CON_LOAD_NO // ���ɻ�����Ϣ 408
	#define R_DEVICE_NO_TAB RT_DEV_SHUNTREACTOR_NO // �����翹��������Ϣ 1401
	#define C_DEVICE_NO_TAB RT_DEV_SHUNTCAPACITOR_NO // ����������������Ϣ 1402
	#define DCSYSTEM_INFO_NO_TAB RT_CON_HVDCSYS_NO // ֱ�����ϵͳʵʱҵ��� 150
	#define DCCONV_DEVICE_NO_TAB RT_CON_CONVERTER_NO // ������ʵʱҵ��� 154
	#define DCLN_DOT_NO_TAB RT_DEV_DCLINEEND_NO // ֱ���߶�ʵʱҵ��� 1211
	#define DCLN_PIECE_NO_TAB RT_DEV_DCLINE_NO // ֱ����·������Ϣ 1206
	#define LFO_PARA_NO_TAB LFO_PARA_NO // LFO������ 143000
	#define LFO_FAC_NO_TAB LFO_FAC_NO // LFO��վ�� 143001
	#define LFO_GN_NO_TAB LFO_GN_NO // LFO������� 143002
	#define LFO_ACLN_NO_TAB LFO_ACLN_NO // LFO������·�� 143003
	#define LFO_RT_CURVE_NO_TAB LFO_RT_CURVE_NO // LFOʵʱ���߱� 143004
	#define LFO_ALLDEV_MODE_NO_TAB LFO_ALLDEV_MODE_NO // LFOʵʱ�豸ģʽȫ�� 143005
	#define LFO_DEV_LEADMODE_NO_TAB LFO_DEV_LEADMODE_NO // LFOʵʱ�豸����ģʽ�� 143006
	#define LFO_WDS_SHAPE_NO_TAB LFO_WDS_SHAPE_NO // LFOʵʱģ̬�� 143007
	#define LFO_CASE_WDS_NO_TAB LFO_CASE_WDS_NO // LFOʵʱCASE���ڱ� 143008
	#define LFO_CASE_ALLSHAPE_NO_TAB LFO_CASE_ALLSHAPE_NO // LFOʵʱCASEģ̬ȫ�� 143009
	#define LFO_TR_NO_TAB LFO_TR_NO // LFO��ѹ���� 143010
	#define LFO_INTERFACE_NO_TAB LFO_INTERFACE_NO // LFO���涨��� 143011
	#define LFO_INTERFACE_DEV_NO_TAB LFO_INTERFACE_DEV_NO // LFO������ɱ� 143012
	#define LFO_ASST_DECI_NO_TAB LFO_ASST_DECI_NO // LFO�������߱� 143013
	#define LFO_DECI_INDEX_NO_TAB LFO_DECI_INDEX_NO // LFO���������� 143014
	#define LFO_CTL_DECI_NO_TAB LFO_CTL_DECI_NO // LFO���Ա� 143015
	#define LFO_LPO_PARA_NO_TAB LFO_LPO_PARA_NO // LFO_LPO������ 143016
	#define LFO_LPO_EVE_NO_TAB LFO_LPO_EVE_NO // LFO_LPO��Ϣ�� 143017
	#define LFO_LPO_CASE_NO_TAB LFO_LPO_CASE_NO // LFO_LPO��ϸ��Ϣ�� 143018
	#define LFO_CTL_ACT_NO_TAB LFO_CTL_ACT_NO // LFO���ƶ���� 143019
	#define LFO_DEV_CURVE_TAB LFO_DEV_CURVE_NO //LFO�豸�����߱� 143020
	#define LFO_INTFACE_NO_TAB LFO_INTFACE_NO	//LFO���������ȱ� 143024
	#define LFO_INTLINE_NO_TAB LFO_INTLINE_NO	//LFO������������֧·��ϵ�� 143025
	#define PQ_DA_CTL_PARA_NO_TAB PQ_DA_CTL_PARA_NO // VF��̬���������� 143050
	#define PQ_DA_BUS_NO_TAB PQ_DA_BUS_NO // VF��̬����ĸ�߱� 143051
	#define PQ_DA_STAT_NO_TAB PQ_DA_STAT_NO // VF��̬����״̬�� 143052
	#define PQ_DA_VOLT_INFO_NO_TAB PQ_DA_VOLT_INFO_NO // VF��̬������ѹ��Ϣ�� 143053
	#define PQ_DA_FREQ_INFO_NO_TAB PQ_DA_FREQ_INFO_NO // VF��̬����Ƶ����Ϣ�� 143054
	#define PQ_DA_MAR_NO_TAB PQ_DA_MAR_NO // ��̬Ʒ������ԣ�ȱ� 143055
	#define LFO_WARN_NO_TAB LFO_WARN_NO // LFO���Ӹ澯�� 8690
	#define LFO_LPO_WARN_NO_TAB LFO_LPO_WARN_NO // LFO_LPO�澯�� 8691
	#define PQ_DA_VOLT_WARN_NO_TAB PQ_DA_VOLT_WARN_NO // ��̬Ʒ��������ѹ�澯�� 8695
	#define PQ_DA_FREQ_WARN_NO_TAB PQ_DA_FREQ_WARN_NO // ��̬Ʒ������Ƶ�ʸ澯�� 8696
	#define WAM_MV_PARA_NO_TAB WAM_MV_PARA_NO // WAM_MV������ 8560
	#define WAM_MV_ACLN_NO_TAB WAM_MV_ACLN_NO // WAM_MV������·�� 8561
	#define WAM_MV_TR_NO_TAB WAM_MV_TR_NO // WAM_MV��ѹ���� 8562
	#define WAM_MV_DEVCUV_NO_TAB WAM_MV_DEVCUV_NO // WAM_MV�豸���߱� 8563
	#define WAM_MV_CURVE_NO_TAB WAM_MV_CURVE_NO // WAM_MVʵʱ���߱� 8564
	#define	WAM_AM_PARA_NO_TAB 1966 // WAM_AM_PARA_NO //	1966	/* WAM_AM������ */
	#define	WAM_AM_DEV_NO_TAB 1967 // WAM_AM_DEV_NO  //	1967	/* WAM_AM�豸�� */
	#define	WAM_AM_CURVE_NO_TAB 1968 // WAM_AM_CURVE_NO	//1968	/* WAM_AM���߱� */
	#define LFO_MODE_INF_NO_TAB DSA_LFO_MODE_INF_NO // ʵ����ģʽ�� 140316
	#define LFO_DEV_INF_NO_TAB DSA_LFO_DEV_INF_NO // ʵ�����豸�� 9197
	#define DSA_SSATPMUCOMP_NO_TAB DSA_SSATPMUCOMP_NO // ��Ƶ��С�Ŷ�����ƥ���� 9198
	#define WAMS_OSC_INFO_NO_TAB WAMS_OSC_INFO_NO // WAMS��Ƶ����Ϣ�� 8229
	#define DSA_N1OUTAGE_NO_TAB DSA_N1OUTAGE_NO // EACCS�豸����ѡ�� 9281
	#define DSA_N1RESULT_NO_TAB DSA_N1RESULT_NO // EACCS��EMS���Ͻ���Ƚ� 9282
#ifdef _KEDONG
	#define WAM_FES_YC_DEFINE_NO_TAB 105606 // WAMS_RECV_ANALOG_NEW  // wams����ң����Ϣ�� 105606
	#define WAM_FES_YX_DEFINE_NO_TAB 105607 // WAMS_RECV_POINT_NEW   // wams����ң����Ϣ�� 105607
#else
	#define WAM_FES_YC_DEFINE_NO_TAB SG_WAM_FES_YC_DEFINE_RT_NO // WFESң�ⶨ��� 104066	
	#define WAM_FES_YX_DEFINE_NO_TAB SG_WAM_FES_YX_DEFINE_RT_NO // WFESң�Ŷ���� 104065
	#define WAM_COMM_FAC_INFO_NO SG_WAM_COMM_FAC_INFO_RT_NO // WFESͨѶ��վ�� 104062
        #define WAM_CHANNEL_INFO_NO_TAB SG_WAM_CHANNEL_INFO_RT_NO // WFESͨ���� 104063
#endif // _KEDONG
	#define VALUE_INFO_NO_TAB VALUE_INFO_NO // ���ң����Ϣ�� ����
	#define WAMS_APP_WARN_INFO_NO_TAB WAMS_APP_WARN_INFO_NO // WAMSӦ��ģ��澯��Ϣ�� 223382
	#define SSO_PARA_NO_TAB SSO_PARA_NO // SSO������ 
	#define SSO_FAC_NO_TAB SSO_FAC_NO // SSO��վ�� 
	#define SSO_GN_NO_TAB SSO_GN_NO // SSO������� 
	#define SSO_ACLN_NO_TAB SSO_ACLN_NO // SSO������·�� 
	#define SSO_TR_NO_TAB SSO_TR_NO // SSO��ѹ����
	#define SSO_DEV_NO_TAB SSO_DEV_NO //SSO���豸��
	#define SSO_CASE_WDS_NO_TAB SSO_CASE_WDS_NO  //SSOʵʱCASE���ڱ�
	#define SSO_RT_CURVE_NO_TAB SSO_RT_CURVE_NO // SSOʵʱ���߱�
	#define SSO_RT_CURVE2_NO_TAB SSO_RT_CURVE2_NO //SSOʵʱ���ߺ����
#elif _D5000
	#define VL_TYPE_NO_TAB BASEVOLTAGE_NO // ��ѹ���ͱ� 401
	#define TERMINAL_NO_TAB SINGLETERM_NO // ����Ԫ���� 421
	#define CMPNY_INFO_NO_TAB COMPANY_NO // ������˾�� 403
	#define DIV_INFO_NO_TAB SUBCONTROLAREA_NO // ����� 404
	#define FAC_INFO_NO_TAB SUBSTATION_NO // ��վ�� 405
	#define VL_LEVEL_NO_TAB VOLTAGELEVEL_NO // ��ѹ�ȼ��� 402
	#define BAY_INFO_NO_TAB BAY_NO // ����� 406
	#define BRK_DEVICE_NO_TAB BREAKER_NO // ��·���� 407
	#define DISCR_DEVICE_NO_TAB DISCONNECTOR_NO // ��բ�� 408
	#define GDDISCR_DEVICE_NO_TAB GROUNDDISCONNECTOR_NO // �ӵص�բ�� 409
	#define BS_DEVICE_NO_TAB BUSBARSECTION_NO // ĸ�߱� 410
	#define LINE_DEVICE_NO_TAB ACLINE_NO // T������·�� 413
	#define ACLN_DEVICE_NO_TAB ACLINESEGMENT_NO // �����߶α� 414
	#define ACLN_DOT_NO_TAB ACLINEEND_NO // �����߶ζ˵�� 415
	#define TR_DEVICE_NO_TAB POWERTRANSFORMER_NO // ��ѹ���� 416
	#define TRWD_DEVICE_NO_TAB TRANSFORMERWINDING_NO // ��ѹ������� 417
	#define TAP_TYPE_NO_TAB TAPTYPE_NO // ��ѹ����ͷ���ͱ� 418
	#define GN_DEVICE_NO_TAB GENERATINGUNIT_NO // �������� 411
	#define LD_DEVICE_NO_TAB ENERGYCONSUMER_NO // ���ɱ� 412
	#define RC_DEVICE_NO_TAB COMPENSATOR_P_NO // �ݿ����� 419
	#define DCSYSTEM_INFO_NO_TAB DCSYSTEM_NO // ֱ��ϵͳ�� 424
	#define DCCONV_DEVICE_NO_TAB RECTIFIERINVERTER_NO // �������� 425
	#define DCLN_DOT_NO_TAB DCLINEEND_NO // ֱ���߶˱� 427
	#define DCLN_PIECE_NO_TAB DCLINESEGMENT_NO // ֱ���߶α� 426
	#define DCGROUND_TERM_NO_TAB DCGROUNDTERM_NO // ֱ���ӵض˱� 428
	#define ISLANDANAABUS_NO_TAB DSA_ISLANDANAABUS_NO // �������ĸ�� 9319
	#define STCASE_TMP_NO_TAB DSA_STCASE_TMP_NO // ���㷽����Դ�� 9125
	#define ZONELFCASE_RES_NO_TAB DSA_ZONELFCASE_RES_NO // ������ʼ������������ 9135
	#define LFEMGSTATICS_NO_TAB DSA_LFEMGSTATICS_NO // ��̬��������ͳ�ƽ���� 9257
	#define EMGSTATICS_NO_TAB DSA_EMGSTATICS_NO // ��������ͳ�ƽ�� 9258
	#define PVCURVERESULT_NO_TAB DSA_PVCURVERESULT_NO // PV����ȫ����� 9235
	#define ELE_GRPUNIT_NO_TAB DSA_ELE_GRPUNIT_NO // �豸����ɱ� 9053
	#define ELE_GRP_NO_TAB DSA_ELE_GRP_NO // �豸��� 9052
	#define TEMPERATURERATE_NO_TAB DSA_TEMPERATURERATE_NO // �¶�ϵ���� 9064
	#define ISLANDMARGINRESULT_NO_TAB DSA_ISLANDMARGINRESULT_NO // ������������� 9323
	#define CONTGRP_NO_TAB DSA_CONTGRP_NO // ������� 9031
	#define CCTCONT_RESULT_NO_TAB DSA_CCTCONT_RESULT_NO // CCT���Ͻ���� 9236
	#define INTLINE_RES_NO_TAB DSA_INTLINE_RES_NO // �ؼ������֧·��ϵ�� 9202
	#define INTERFACE_RES_NO_TAB DSA_INTERFACE_RES_NO // �ؼ������ 9203
	#define LFCASEOVLDECI_NO_TAB DSA_LFCASEOVLDECI_NO // ��ʼ���ش�ʩ����� 9253
	#define STCASEOVLDECI_NO_TAB DSA_STCASEOVLDECI_NO // Ԥ����ش�ʩ����� 9254
	#define ISLANDINTCONT_NO_TAB DSA_ISLANDINTCONT_NO // �������Ͻ���� 9320
	#define CASE_RESULT_STATICS_NO_TAB DSA_CASE_RESULT_STATICS_NO // ��ȫ�Է���ͳ����Ϣ�� 9393
	#define FREQINTERFACE_NO_TAB DSA_FREQINTERFACE_NO // ����Ƶ�ʶ�������� 9321
	#define FREQINTLINE_NO_TAB DSA_FREQINTLINE_NO // �������Ⱥ�֧·��ϵ�� 9322
	#define UFV_ACTION_NO_TAB DSA_UFV_ACTION_NO // ��Ƶ��ѹ����װ�ö����� 9324
	#define GENPROC_ACTION_NO_TAB DSA_GENPROC_ACTION_NO // ��������������� 9325
	#define PRIMEINTCONT_NO_TAB DSA_PRIMEINTCONT_NO // �ؼ��������� 9201
	#define CONT_ISLAND_NO_TAB DSA_CONT_ISLAND_NO // ���Ϻ����˵��� 9313
	#define SCPCDMBUSRESULT_NO_TAB DSA_SCPCDMBUSRESULT_NO // ĸ�߶�·�����������߽�� 9297
	#define SCLNPCDMRESULT_NO_TAB DSA_SCLNPCDMRESULT_NO // ��·��·�����������߽�� 9298
	#define SCHEMETYPE_NO_TAB DSA_SCHEMETYPE_NO // ���ռƻ������ 9391
	#define STINTFACELN_NO_TAB DSA_STINTFACELN_NO // ���޶�������·���� 9241
	#define STINTERFACE_NO_TAB DSA_STINTERFACE_NO // EACCS���������Ƚ���� 9228
	#define SENRESULT_NO_TAB DSA_SENRESULT_NO // �����ȼ����� 9333
	#define ISLANDBUS_NO_TAB DSA_ISLANDBUS_NO // ���˵�ĸ�� 9311
	#define ISLANDRESULT_NO_TAB DSA_ISLANDRESULT_NO // ���˷������ 9312
	#define LEADSRHELE_NO_TAB DSA_LEADSRHELE_NO // �����豸����ǰ��ȽϽ���� 9332
	#define LEADSRHGENRESULT_NO_TAB DSA_LEADSRHGENRESULT_NO // ����������н�� 9334
	#define DCCTRLPAR_NO_TAB DSA_DCCTRLPAR_NO // ֱ������ϵͳ 9027
	#define SCSET_NO_TAB DSA_SCSET_NO // ��·������������ 9291
	#define SCBUSRESULT_NO_TAB DSA_SCBUSRESULT_NO // ĸ�߶�·������ 9292
	#define SCLNRESULT_NO_TAB DSA_SCLNRESULT_NO // ��·��·������ 9293
	#define SCBUSADJRESULT_NO_TAB DSA_SCBUSADJRESULT_NO // ĸ�߶�·����Ԫ�������� 9294
	#define SCLNSADJRESULT_NO_TAB DSA_SCLNSADJRESULT_NO // ��·��·����Ԫ�������� 9295
	#define SCBREAKRESULT_NO_TAB DSA_SCBREAKRESULT_NO // ��·����·������� 9296
	#define TIMEPROC_NO_TAB DSA_TIMEPROC_NO // ��ʱ�������� 9430
	#define EACCSINTPV_NO_TAB DSA_EACCSINTPV_NO // ����������PV���߽���� 9234
	#define PVCURVE_NO_TAB DSA_PVCURVE_NO // PV���߽���� 9233
	#define SIMUINTERFACE_NO_TAB DSA_SIMUINTERFACE_NO // ����EACCS���������ȱ� 9416
	#define LPSCASE_RESULT_NO_TAB DSA_LPSCASE_RESULT_NO // ������ڹ��ϼ���� 9335
	#define LPSCONT_RESULT_NO_TAB DSA_LPSCONT_RESULT_NO // ������ڹ���������� 9336
	#define LPSCCGEN_RES_NO_TAB DSA_LPSCCGEN_RES_NO // �����Ⱥ�б� 9337
	#define CONTDECIDISP_NO_TAB DSA_CONTDECIDISP_NO // ���ϴ�ʩ��ʾ�� 9259
	#define THDUFV_ACTION_NO_TAB DSA_THDUFV_ACTION_NO // ���������ߵ�Ƶ��ѹ����װ�ö����� 9364
	#define THDGENPROC_ACTION_NO_TAB DSA_THDGENPROC_ACTION_NO // ���������߷�������������� 9365
	#define THDLINEPROC_ACTION_NO_TAB DSA_THDLINEPROC_ACTION_NO // ������������·���������� 9366
	#define EACCSMARLMT_NO_TAB DSA_EACCSMARLMT_NO // ϵͳ��ֵ�� 9067
	#define THDCONT_RESULT_NO_TAB DSA_THDCONT_RESULT_NO // ����������У�˽�� 9412
	#define ZONE_INFO_NO_TAB DSA_ZONE_INFO_NO // С�� 9001
	#define CONT_FILTER_RULE_NO_TAB DSA_CONT_FILTER_RULE_NO // ����ɸѡ����� 9055
	#define SIMUSYSPARA_NO_TAB DSA_SIMUSYSPARA_NO // ����ϵͳ�������� 9411
	#define SSATMODE_NO_TAB DSA_SSATMODE_NO // С�Ŷ�ģʽ������� 9191
	#define SSATMODESHAPE_NO_TAB DSA_SSATMODESHAPE_NO // С�Ŷ�����ģ̬��� 9192
	#define SIMUCURVE_NO_TAB DSA_SIMUCURVE_NO // �������߽���� 9413
	#define SIMUCURVEDISP_NO_TAB DSA_SIMUCURVEDISP_NO // ����������ʾ�� 9414
	#define PCDMSTAINFO_NO_TAB DSA_PCDMSTAINFO_NO // Ԥ������ͳ����Ϣ 9273
	#define EACCSINTORDER_NO_TAB DSA_EACCSINTORDER_NO // WAMAP��������������� 9229
	#define EACCSCIRCLE_NO_TAB DSA_EACCSCIRCLE_NO // ��ϻ��� 9341
	#define EACCSCIRCLELN_NO_TAB DSA_EACCSCIRCLELN_NO // ��ϻ���·��� 9342
	#define EACCSCIRCLERULE_NO_TAB DSA_EACCSCIRCLERULE_NO // ��ϻ������� 9343
	#define CURDISPRECORD_NO_TAB DSA_CURDISPRECORD_NO // ��ǰ��¼�� 9445
	#define PCP_PROCESSINFO_NO_TAB DSA_PCP_PROCESSINFO_NO // ���ӽ�����Ϣ�� 9444
	#define PCP_NETWORKINFO_NO_TAB DSA_PCP_NETWORKINFO_NO // ����״̬��Ϣ�� 9443
	#define LINEPROC_ACTION_NO_TAB DSA_LINEPROC_ACTION_NO // ��·���������� 9326
	#define EACCSECDONLINE_NO_TAB DSA_EACCSECDONLINE_NO // ����װ�����߲��� 9261
	#define ACTDEVICEINFO_NO_TAB DSA_ACTDEVICEINFO_NO // ����װ�ò������ñ� 9260
	#define PCP_CPULOADINFO_NO_TAB DSA_PCP_CPULOADINFO_NO // CPU���ڴ渺�ɱ� 9442
	#define PCP_DISKINFO_NO_TAB DSA_PCP_DISKINFO_NO // ���̿ռ���Ϣ�� 9441
	#define DSAS_PROCESSINFO_NO_TAB DSA_DSAS_PROCESSINFO_NO // ����Ԥ��������Ϣ�� 9401
	#define DSAS_RUNINFO_NO_TAB DSA_DSAS_RUNINFO_NO // ����Ԥ��������Ϣ�� 9402
	#define CONT_RESULT_SIMU_NO_TAB DSA_CONT_RESULT_SIMU_NO // �������������� 9417
	#define CASE_EXCHANGE_INFO_NO_TAB DSA_CASE_EXCHANGE_INFO_NO // CASE������Ϣ�� 9429
	#define SSAMARCURVE_NO_TAB DSA_SSAMARCURVE_NO // ��̬��ȫԣ������ 9381
	#define SSAMODE_NO_TAB DSA_SSAMODE_NO // ģʽ������� 9211
	#define SSAMODEFAC_NO_TAB DSA_SSAMODEFAC_NO // ģʽ�������� 9212
	#define TRANSFERMODE_NO_TAB DSA_TRANSFERMODE_NO // ����ģʽ 9223
	#define UFUVSHEDLOAD_NO_TAB DSA_UFUVSHEDLOAD_NO // UFUV���ؽ�� 9367
	#define LEADSRHRULE_NO_TAB DSA_LEADSRHRULE_NO // ������༫���������� 9331
	#define LOADGROUP_NO_TAB DSA_LOADGROUP_NO // ���������ݱ� 9002
	#define LOADGRP_LOAD_NO_TAB DSA_LOADGRP_LOAD_NO // ������͸��ɹ�ϵ�� 9003
	#define MUTUAL_DEVICE_NO_TAB DSA_MUTUAL_DEVICE_NO // ���򻥸� 9004
	#define UFV_DEVICE_NO_TAB DSA_UFV_DEVICE_NO // ��Ƶ��ѹ������װ�� 9361
	#define RU_DEVICE_NO_TAB DSA_RU_DEVICE_NO // ��Ƶ��ѹ��·�Ͽ����� 9362
	#define GENPROT_DEVICE_NO_TAB DSA_GENPROT_DEVICE_NO // ����������� 9363
	#define SCSDEV_NO_TAB DSA_SCSDEV_NO // �ȿ�װ�ö���� 9083
	#define SCSSYS_NO_TAB DSA_SCSSYS_NO // �ȿ�ϵͳ����� 9081
	#define GEN_LIB_NO_TAB DSA_GEN_LIB_NO // �������̬���� 9011
	#define AVRE_LIB_NO_TAB DSA_AVRE_LIB_NO // ����E��ģ�Ͳ����� 9012
	#define AVRF_LIB_NO_TAB DSA_AVRF_LIB_NO // ����F��ģ�Ͳ����� 9013
	#define AVRF2_LIB_NO_TAB DSA_AVRF2_LIB_NO // ����F����ģ�Ͳ����� 9014
	#define GOV_LIB_NO_TAB DSA_GOV_LIB_NO // ��������ԭ����ϵͳģ�ͱ� 9016
	#define PSS_LIB_NO_TAB DSA_PSS_LIB_NO // ����ϵͳ�ȶ���S�� 9015
	#define GENTG_LIB_NO_TAB DSA_GENTG_LIB_NO // �繦��ģ�Ͳ��� 9017
	#define GENGV_LIB_NO_TAB DSA_GENGV_LIB_NO // ����ģ�Ͳ��� 9018
	#define LOAD_LIB_NO_TAB DSA_LOAD_LIB_NO // ����ģ�Ͳ��� 9020
	#define MOTO_LIB_NO_TAB DSA_MOTO_LIB_NO // ��Ӧ�綯����̬���� 9021
	#define DC_LIB_NO_TAB DSA_DC_LIB_NO // ֱ������ģ�Ͳ����� 9025
	#define DCMOD_LIB_NO_TAB DSA_DCMOD_LIB_NO // ֱ������ģ�Ͳ����� 9026
	#define GENGB_LIB_NO_TAB DSA_GENGB_LIB_NO // ���ؽǿ���ϵͳģ�Ͳ��� 9019
	#define RZ_LIB_NO_TAB DSA_RZ_LIB_NO // ��·�̶�����ģ�Ͳ��� 9022
	#define SVC_LIB_NO_TAB DSA_SVC_LIB_NO // ��ֹ�޹����������� 9023
	#define DECISIONCOST_NO_TAB DSA_DECISIONCOST_NO // ��ʩ���۱� 9272
	#define MONITORLMTDEF_NO_TAB DSA_MONITORLMTDEF_NO // ������ֵ�� 9066
	#define ADJCONSTRAIN_NO_TAB DSA_ADJCONSTRAIN_NO // ����Լ���� 9225
	#define DEVICE_MONITOR_NO_TAB DSA_DEVICE_MONITOR_NO // ����Ԫ���� 9061
	#define VOLT_DEVIATION_NO_TAB DSA_VOLT_DEVIATION_NO // �����ѹ��Ԫ�� 9062
	#define FREQ_DEVIATION_NO_TAB DSA_FREQ_DEVIATION_NO // ����Ƶ�ʶ�Ԫ�� 9063
	#define PRE_DECISION_NO_TAB DSA_PRE_DECISION_NO // Ԥ�����������ռ� 9271
	#define LIMIT_DEFINATION_NO_TAB DSA_LIMIT_DEFINATION_NO // ��ֵ�����б� 9065
	#define MATCH_INTERFACE_NO_TAB DSA_MATCH_INTERFACE_NO // ƥ������ 9111
	#define MATCH_AREALINE_NO_TAB DSA_MATCH_AREALINE_NO // ƥ�������ɱ� 9112
	#define NODE_TYDEF_NO_TAB DSA_NODE_TYDEF_NO // �ڵ����ͱ� 9113
	#define INTERFACE_NO_TAB DSA_INTERFACE_NO // WAMAP���������ȱ� 9221
	#define INTLINE_NO_TAB DSA_INTLINE_NO // WAMAP���������Ⱥ�֧·��ϵ�� 9222
	#define LMT_DECISION_NO_TAB DSA_LMT_DECISION_NO // ���޹��������ռ� 9224
	#define PCP_MANAGENODEINFO_NO_TAB DSA_PCP_MANAGENODEINFO_NO // ����ڵ�������Ϣ�� 9421
	#define PCP_COMPUTENODEINFO_NO_TAB DSA_PCP_COMPUTENODEINFO_NO // ����ڵ�������Ϣ�� 9422
	#define PCP_COMMUNICATENODEINFO_NO_TAB DSA_PCP_COMMUNICATENODEINFO_NO // ͨ�Žڵ�������Ϣ�� 9423
	#define PCP_ALARMINFO_NO_TAB DSA_PCP_ALARMINFO_NO // �澯��Ϣ�� 9424
	#define PCP_DOMAININFO_NO_TAB DSA_PCP_DOMAININFO_NO // ��������Ϣ�� 9425
	#define PCP_NODEPROCESSINFO_NO_TAB DSA_PCP_NODEPROCESSINFO_NO // ��ɫӦ�ý���������Ϣ�� 9426
	#define CONTINT_NO_TAB DSA_CONTINT_NO // ���Ϻ����ȹ�ϵ�� 9226
	#define PCP_NODERUNSTATUSINFO_NO_TAB DSA_PCP_NODERUNSTATUSINFO_NO // �ڵ�����״̬������Ϣ�� 9427
	#define PCP_CLUSTERNODEINFO_NO_TAB DSA_PCP_CLUSTERNODEINFO_NO // ��Ⱥ�ڵ���Ϣ�� 9428
	#define THDACTCONT_RESULT_NO_TAB DSA_THDACTCONT_RESULT_NO // ��������������ͳ�ƽ���� 9278
	#define LFPCDMSENELE_RES_NO_TAB DSA_LFPCDMSENELE_RES_NO // У�����������Ƚ�� 9279
	#define PCDMSENELE_RES_NO_TAB DSA_PCDMSENELE_RES_NO // Ԥ�����������Ƚ�� 9280
	#define BL_EVENT_NO_TAB DSA_BL_EVENT_NO // ĸ�߼�֧·�¼��� 9034
	#define EMG_DECITION_NO_TAB DSA_EMG_DECITION_NO // ���������л����и��ɴ�ʩ 9035
	#define GENEVENT_NO_TAB DSA_GENEVENT_NO // ��������� 9036
	#define DC_EVENT_NO_TAB DSA_DC_EVENT_NO // ֱ���߹��� 9037
	#define SSATMODECOMP_NO_TAB DSA_SSATMODECOMP_NO // С�Ŷ���������ǰ��Ƚ� 9193
	#define SSATPREADJUST_RES_NO_TAB DSA_SSATPREADJUST_RES_NO // С���Ÿ������߱� 9194
	#define SSATDAMPGEN_RES_NO_TAB DSA_SSATDAMPGEN_RES_NO // С������������Ⱥ 9195
	#define LFPCDMSTATICS_NO_TAB DSA_LFPCDMSTATICS_NO // ��ʼ������������ͳ�ƽ�� 9275
	#define LFPREADJUST_RES_NO_TAB DSA_LFPREADJUST_RES_NO // ��̬�������ߵ����б� 9276
	#define FAULT_AUTOGEN_NO_TAB DSA_FAULT_AUTOGEN_NO // ���߹������ɲ��� 9051
	#define HISFAULT_RULE_NO_TAB DSA_HISFAULT_RULE_NO // ��ʷ�������ɹ��� 9056
	#define SIMUCURVEAUTORUN_NO_TAB DSA_SIMUCURVEAUTORUN_NO // ��������ʵʱ�� 9415
	#define FAULT_TIMET_NO_TAB DSA_FAULT_TIMET_NO // ����ʱ��� 9054
	#define CONTINGENCY_NO_TAB DSA_CONTINGENCY_NO // ������ 9032
	#define CONT_EVENT_NO_TAB DSA_CONT_EVENT_NO // �������¼���ϵ�� 9033
	#define CONT_DECITION_NO_TAB DSA_CONT_DECITION_NO // ���Ϻͺ�ѡ��ʩ����ϵ�� 9252
	#define LFCASE_NO_TAB DSA_LFCASE_NO // ��ʼ������������� 9131
	#define LFCASE_RES_NO_TAB DSA_LFCASE_RES_NO // ��ʼ������������ 9132
	#define LFCASE_PARA_NO_TAB DSA_LFCASE_PARA_NO // ����������� 9123
	#define LFOVERELE_RES_NO_TAB DSA_LFOVERELE_RES_NO // ��ʼ����Ԫ���������б� 9133
	#define SENELE_RES_NO_TAB DSA_SENELE_RES_NO // ��̬Ԫ�������ȷ��� 9134
	#define PREADJUST_RES_NO_TAB DSA_PREADJUST_RES_NO // Ԥ�����ƹ��ʵ����б� 9274
	#define DECITIONGRP_NO_TAB DSA_DECITIONGRP_NO // ��ѡ��ʩ��� 9251
	#define LFEMG_RES_NO_TAB DSA_LFEMG_RES_NO // ��̬�������Ʋ��Խ�� 9256
	#define LINEFLOW_COMP_NO_TAB DSA_LINEFLOW_COMP_NO // ��·�����Ƚ��б� 9161
	#define BUSVOLT_COMP_NO_TAB DSA_BUSVOLT_COMP_NO // ĸ�ߵ�ѹ�Ƚ��б� 9162
	#define MATCH_RESULT_NO_TAB DSA_MATCH_RESULT_NO // ƥ������ 9163
	#define LMTSTATICS_NO_TAB DSA_LMTSTATICS_NO // ���޹���ͳ�ƽ�� 9232
	#define INTADJLIST_NO_TAB DSA_INTADJLIST_NO // ���湦�ʵ����б� 9230
	#define STCASE_NO_TAB DSA_STCASE_NO // ���㷽������� 9122
	#define STCASE_PARA_NO_TAB DSA_STCASE_PARA_NO // ��ȫ�Է���������Ϣ 9124
	#define CONT_PARA_NO_TAB DSA_CONT_PARA_NO // ����������Ϣ 9227
	#define STCASE_CONT_NO_TAB DSA_STCASE_CONT_NO // �ȶ����㷽���͹��Ϲ�ϵ�� 9237
	#define STCASETSK_NO_TAB DSA_STCASETSK_NO // ���㷽������� 9121
	#define CASE_RESULT_NO_TAB DSA_CASE_RESULT_NO // ���ڹ��ϼ������ 9171
	#define CONT_RESULT_NO_TAB DSA_CONT_RESULT_NO // ���ڹ���������� 9172
	#define EMGSENELE_RES_NO_TAB DSA_EMGSENELE_RES_NO // �������������Ƚ�� 9238
	#define PCDMOVERELE_RES_NO_TAB DSA_PCDMOVERELE_RES_NO // Ԥ������ǰ��Ԫ���������б� 9239
	#define LFPCDMOVERELE_RES_NO_TAB DSA_LFPCDMOVERELE_RES_NO // У������ǰ��Ԫ���������б� 9240
	#define FAKEVB_RES_NO_TAB DSA_FAKEVB_RES_NO // ��ѹ����ĸ���б� 9175
	#define FAKEFB_RES_NO_TAB DSA_FAKEFB_RES_NO // Ƶ�ʱ���ĸ���б� 9176
	#define CCGEN_RES_NO_TAB DSA_CCGEN_RES_NO // ��Ⱥ�б� 9174
	#define DAMPMODE_RES_NO_TAB DSA_DAMPMODE_RES_NO // ��ģʽ�� 9177
	#define DAMPGEN_RES_NO_TAB DSA_DAMPGEN_RES_NO // ��������Ⱥ 9178
	#define EMG_RES_NO_TAB DSA_EMG_RES_NO // ���Ʋ���������� 9255
	#define OVERELE_RES_NO_TAB DSA_OVERELE_RES_NO // Ԫ���������б� 9173
	#define CONTSENELE_RES_NO_TAB DSA_CONTSENELE_RES_NO // Խ��Ԫ�������ȷ������ 9231
	#define CONCONT_RES_NO_TAB DSA_CONCONT_RES_NO // �ɿز��ɿع����б� 9277
	#define MARGINCURVE_NO_TAB DSA_MARGINCURVE_NO // ԣ������ 9382
	#define PFRESULTCOMPARE_NO_TAB DSA_PFRESULTCOMPARE_NO // ����ǰ�����Ƚ� 9153
	#define CASE_RESULT2_NO_TAB DSA_CASE_RESULT2_NO // ���ڹ��ϼ�������� 9179
	#define MODEL_PARA_BIAS_NO_TAB DSAMODEL_PARA_BIAS_NO // ����ת���� 9394
	#define SCS_SETVALUE_RESULT_NO_TAB DSA_SCS_SETVALUE_RESULT_NO // �ȿ�ϵͳ��ֵ������ 9098
	#define SCS_SETVALUE_ADJ_RESULT_NO_TAB DSA_SCS_SETVALUE_ADJ_RESULT_NO // �ȿ�ϵͳ��ֵ���������� 9099
	#define LOSS_STATICS_NO_TAB DSA_LOSS_STATICS_NO // ����ǰ������ȽϽ���� 9262
	#define LFCASE_CMPNY_NO_TAB DSA_LFCASE_CMPNY_NO // ��˾����ͳ�� 9152
	#define LFCASEDCLN_NO_TAB DSA_LFCASEDCLN_NO // ֱ����·���� 9151
	#define LFCASE_AREA_NO_TAB DSA_LFCASE_AREA_NO // ��������Ϣ 9141
	#define LFCASE_STAT_NO_TAB DSA_LFCASE_STAT_NO // ��վ���� 9142
	#define LFCASEBUS_NO_TAB DSA_LFCASEBUS_NO // ĸ�ߵ�ѹ��� 9144
	#define LFCASELN_NO_TAB DSA_LFCASELN_NO // ��·���� 9143
	#define LFCASETRANSFORMER_NO_TAB DSA_LFCASETRANSFORMER_NO // ��ѹ������ 9145
	#define LFCASEWIND_NO_TAB DSA_LFCASEWIND_NO // ���鳱�� 9146
	#define LFCASECAP_NO_TAB DSA_LFCASECAP_NO // �ݿ���������� 9147
	#define LFCASEGEN_NO_TAB DSA_LFCASEGEN_NO // ���糱����� 9148
	#define LFCASEINT_NO_TAB DSA_LFCASEINT_NO // ������泱�� 9150
	#define LFCASEINTLN_NO_TAB DSA_LFCASEINTLN_NO // ������·���� 9149
	#define SCSFAC_NO_TAB DSA_SCSFAC_NO // �ȿ�վ��� 9082
	#define SCSPLATE_NO_TAB DSA_SCSPLATE_NO // �ȿ�װ��ѹ��� 9084
	#define SCSINF_NO_TAB DSA_SCSINF_NO // �ȿ�װ�ö���� 9085
	#define SCSINFUNIT_NO_TAB DSA_SCSINFUNIT_NO // �ȿ�װ�ö�����ɱ� 9086
	#define SCSOPMODE_NO_TAB DSA_SCSOPMODE_NO // ���з�ʽ�� 9087
	#define SCSSETVAL_NO_TAB DSA_SCSSETVAL_NO // ��ֵ�� 9090
	#define CALFORMULA_NO_TAB DSA_CALFORMULA_NO // ��ʽ�� 9091
	#define SCSOFFDECI_NO_TAB DSA_SCSOFFDECI_NO // �ȿ�����վ���Ա� 9092
	#define SCSSEARCHRESULT_NO_TAB DSA_SCSSEARCHRESULT_NO // ���߲���������� 9100
	#define BACKOFFDECI_NO_TAB DSA_BACKOFFDECI_NO // �����Ͳ��Ա� 9093
	#define SCSCTRLDECI_NO_TAB DSA_SCSCTRLDECI_NO // ���Ʋ��Ա� 9094
	#define SCSACTGRP_NO_TAB DSA_SCSACTGRP_NO // �ȿض����� 9095
	#define SCSDECI_NO_TAB DSA_SCSDECI_NO // �ȿ�ִ�в��Ա� 9096
	#define SCSORDER_NO_TAB DSA_SCSORDER_NO // �ȿض����ռ� 9097
	#define SCSJUDGEUNIT_NO_TAB DSA_SCSJUDGEUNIT_NO // �ȿ�װ���оݵ�Ԫ�� 9089
	#define SCSJUDGE_NO_TAB DSA_SCSJUDGE_NO // �ȿ�װ���оݱ� 9088
	#define BATS_JUDGE_NO_TAB DSA_BATS_JUDGE_NO // ����Ͷװ�ö����оݱ� 9351
	#define BATSCASE_NO_TAB DSA_BATSCASE_NO // ����ͶУ�˷��� 9352
	#define BATS_ACTION_NO_TAB DSA_BATS_ACTION_NO // ����Ͷװ�ö����� 9353
	#define FLORESULTFILE_NO_TAB DSA_FLORESULTFILE_NO // ��������ļ���Ϣ�� 9354
	#define LFSTAANGRLT_NO_TAB DSA_LFSTAANGRLT_NO // ��̬�����ȶ������ 9355
	#define CDQPFRESULTCOMPARE_NO_TAB DSA_CDQPFRESULTCOMPARE_NO // ������Ԥ��ǰ�����Ƚ� 9356
	#define CDQGENERALMES_NO_TAB DSA_CDQGENERALMES_NO // �����ڸ�Ҫ��Ϣ�� 9357
	#define LFO_PARA_NO_TAB LFO_PARA_NO // LFO������ 8600
	#define LFO_FAC_NO_TAB LFO_FAC_NO // LFO��վ�� 8601
	#define LFO_GN_NO_TAB LFO_GN_NO // LFO������� 8602
	#define LFO_ACLN_NO_TAB LFO_ACLN_NO // LFO������·�� 8603
	#define LFO_RT_CURVE_NO_TAB LFO_RT_CURVE_NO // LFOʵʱ���߱� 8604
	#define LFO_ALLDEV_MODE_NO_TAB LFO_ALLDEV_MODE_NO // LFOʵʱ�豸ģʽȫ�� 8605
	#define LFO_DEV_LEADMODE_NO_TAB LFO_DEV_LEADMODE_NO // LFOʵʱ�豸����ģʽ�� 8606
	#define LFO_WDS_SHAPE_NO_TAB LFO_WDS_SHAPE_NO // LFOʵʱģ̬�� 8607
	#define LFO_CASE_WDS_NO_TAB LFO_CASE_WDS_NO // LFOʵʱCASE���ڱ� 8608
	#define LFO_CASE_ALLSHAPE_NO_TAB LFO_CASE_ALLSHAPE_NO // LFOʵʱCASEģ̬ȫ�� 8609
	#define LFO_TR_NO_TAB LFO_TR_NO // LFO��ѹ���� 8610
	#define LFO_INTERFACE_NO_TAB LFO_INTERFACE_NO // LFO���涨��� 8611
	#define LFO_INTERFACE_DEV_NO_TAB LFO_INTERFACE_DEV_NO // LFO������ɱ� 8612
	#define LFO_ASST_DECI_NO_TAB LFO_ASST_DECI_NO // LFO�������߱� 8613
	#define LFO_DECI_INDEX_NO_TAB LFO_DECI_INDEX_NO // LFO���������� 8614
	#define LFO_CTL_DECI_NO_TAB LFO_CTL_DECI_NO // LFO���Ա� 8615
	#define LFO_LPO_PARA_NO_TAB LFO_LPO_PARA_NO // LFO_LPO������ 8616
	#define LFO_LPO_EVE_NO_TAB LFO_LPO_EVE_NO // LFO_LPO��Ϣ�� 8617
	#define LFO_LPO_CASE_NO_TAB LFO_LPO_CASE_NO // LFO_LPO��ϸ��Ϣ�� 8618
	#define LFO_CTL_ACT_NO_TAB LFO_CTL_ACT_NO // LFO���ƶ���� 8619
	#define LFO_DEV_CURVE_TAB LFO_DEV_CURVE_NO //LFO�豸�����߱� 8620
	#define LFO_WARN_NO_TAB LFO_WARN_NO // LFO���Ӹ澯�� 8690
	#define LFO_LPO_WARN_NO_TAB LFO_LPO_WARN_NO // LFO_LPO�澯�� 8691
	#define PQ_DA_CTL_PARA_NO_TAB PQ_DA_CTL_PARA_NO // VF��̬���������� 8650
	#define PQ_DA_BUS_NO_TAB PQ_DA_BUS_NO // VF��̬����ĸ�߱� 8651
	#define PQ_DA_STAT_NO_TAB PQ_DA_STAT_NO // VF��̬����״̬�� 8652
	#define PQ_DA_VOLT_INFO_NO_TAB PQ_DA_VOLT_INFO_NO // VF��̬������ѹ��Ϣ�� 8653
	#define PQ_DA_FREQ_INFO_NO_TAB PQ_DA_FREQ_INFO_NO // VF��̬����Ƶ����Ϣ�� 8654
	#define PQ_DA_MAR_NO_TAB PQ_DA_MAR_NO // ��̬Ʒ������ԣ�ȱ� 8655
	#define PQ_DA_VOLT_WARN_NO_TAB PQ_DA_VOLT_WARN_NO // ��̬Ʒ��������ѹ�澯�� 8695
	#define PQ_DA_FREQ_WARN_NO_TAB PQ_DA_FREQ_WARN_NO // ��̬Ʒ������Ƶ�ʸ澯�� 8696
	#define WAM_MV_PARA_NO_TAB WAM_MV_PARA_NO // WAM_MV������ 8560
	#define WAM_MV_ACLN_NO_TAB WAM_MV_ACLN_NO // WAM_MV������·�� 8561
	#define WAM_MV_TR_NO_TAB WAM_MV_TR_NO // WAM_MV��ѹ���� 8562
	#define WAM_MV_DEVCUV_NO_TAB WAM_MV_DEVCUV_NO // WAM_MV�豸���߱� 8563
	#define WAM_MV_CURVE_NO_TAB WAM_MV_CURVE_NO // WAM_MVʵʱ���߱� 8564
	#define SSO_RT_CURVE_NO_TAB SSO_RT_CURVE_NO // LFOʵʱ���߱� 8665
	#define	WAM_AM_PARA_NO_TAB 1966 // WAM_AM_PARA_NO //	1966	/* WAM_AM������ */
	#define	WAM_AM_DEV_NO_TAB 1967 // WAM_AM_DEV_NO  //	1967	/* WAM_AM�豸�� */
	#define	WAM_AM_CURVE_NO_TAB 1968 // WAM_AM_CURVE_NO	//1968	/* WAM_AM���߱� */
	#define LFO_MODE_INF_NO_TAB DSA_LFO_MODE_INF_NO // ʵ����ģʽ�� 9196
	#define LFO_DEV_INF_NO_TAB DSA_LFO_DEV_INF_NO // ʵ�����豸�� 9197
	#define DSA_SSATPMUCOMP_NO_TAB DSA_SSATPMUCOMP_NO // ��Ƶ��С�Ŷ�����ƥ���� 9198
	#define WAMS_OSC_INFO_NO_TAB WAMS_OSC_INFO_NO // WAMS��Ƶ����Ϣ�� 8229
	#define DSA_N1OUTAGE_NO_TAB DSA_N1OUTAGE_NO // EACCS�豸����ѡ�� 9281
	#define DSA_N1RESULT_NO_TAB DSA_N1RESULT_NO // EACCS��EMS���Ͻ���Ƚ� 9282
#ifdef _KEDONG_
    #define WAM_FES_YC_DEFINE_NO_TAB WAMS_RECV_ANALOG_NEW_NO // wams����ң����Ϣ�� 8006
#else
    #define WAM_FES_YC_DEFINE_NO_TAB WAM_FES_YC_DEFINE_NO // WFESң�ⶨ��� 8505
#endif
	#define WAM_FES_YX_DEFINE_NO_TAB WAM_FES_YX_DEFINE_NO // WFESң�ⶨ��� 8504
	#define WAM_CHANNEL_INFO_NO_TAB WAM_CHANNEL_INFO_NO // WAMSͨ���� ����
	#define VALUE_INFO_NO_TAB VALUE_INFO_NO // ���ң����Ϣ�� ����
    #define WAMS_APP_WARN_INFO_NO_TAB WAMS_APP_WARN_INFO_NO // WAMSӦ��ģ��澯��Ϣ�� 8414
	#define PRE_PVSSAMODE_NO_TAB DSA_PRE_PVSSAMODE_NO  // Ԥ����ϱǵ��ѹģʽ������� 716
	#define PRE_PVSSAMODEFAC_NO_TAB DSA_EACCS_PRE_PVSSAMODEFAC_NO  // Ԥ����ϱǵ�ģʽ�������ӱ� 717
	#define EME_EACCSINTPV_NO_TAB DSA_EACCS_EACCSINTPV_NO // ����������PV���߽���� 724
	#define EME_PVCURVE_NO_TAB DSA_EACCS_PVCURVE_NO // PV���߽���� 723
	#define	EACCS_PVCURVERESULT_NO_TAB DSA_EACCS_PVCURVERESULT_NO  //	725	/* PV����ȫ����� */
	#define	EACCS_EME_EACCSINTPV_NO_TAB  DSA_EACCS_EME_EACCSINTPV_NO  //	719	/* ��̬����������PV���߽���� */
	#define	EACCS_EME_PVCURVE_NO_TAB DSA_EACCS_EME_PVCURVE_NO  //	718	/* ��̬PV���߽���� */
	#define	EACCS_EME_PVCURVERESULT_NO_TAB DSA_EACCS_EME_PVCURVERESULT_NO  //	720	/* ��̬PV����ȫ����� */
	#define EME_PVSSAMODE_NO_TAB DSA_EACCS_EME_PVSSAMODE_NO // ��̬�ǵ��ѹģʽ��������� 721
	#define EME_PVSSAMODEFAC_NO_TAB DSA_EACCS_EME_PVSSAMODEFAC_NO  // ��̬�ǵ�ģʽ�������ӱ� 722
	#define	SCMS_SYSTEM_INFO_NO_TAB SCMS_SYSTEM_INFO_NO	//9458	/* SCMSϵͳ��Ϣ�� */
	#define	DUTYDECI_NO_TAB DSA_DUTYDECI_NO	// 9467	/* �ȿ�ϵͳ��ֵ���Ա� */
	#define SSO_PARA_NO_TAB SSO_PARA_NO // SSO������ 
	#define SSO_FAC_NO_TAB SSO_FAC_NO // SSO��վ�� 
	#define SSO_GN_NO_TAB SSO_GN_NO // SSO������� 
	#define SSO_ACLN_NO_TAB SSO_ACLN_NO // SSO������·�� 
	#define SSO_TR_NO_TAB SSO_TR_NO // SSO��ѹ����
	#define SSO_DEV_NO_TAB SSO_DEV_NO //SSO���豸��
	#define SSO_CASE_WDS_NO_TAB SSO_CASE_WDS_NO  //SSOʵʱCASE���ڱ�
#else
	#define VL_TYPE_NO_TAB VL_TYPE_NO // ��ѹ���ͱ� 200
	#define TERMINAL_NO_TAB TERMINAL_NO // �ն˱� 205
	#define CMPNY_INFO_NO_TAB CMPNY_INFO_NO // ��˾��Ϣ�� 207
	#define DIV_INFO_NO_TAB DIV_INFO_NO // ��������� 209
	#define FAC_INFO_NO_TAB FAC_INFO_NO // ��վ��Ϣ�� 210
	#define VL_LEVEL_NO_TAB VL_LEVEL_NO // ��ѹ�ȼ��� 211
	#define BAY_INFO_NO_TAB BAY_INFO_NO // �����Ϣ�� 212
	#define BRK_DEVICE_NO_TAB BRK_DEVICE_NO // ��·����Ϣ�� 220
	#define DISCR_DEVICE_NO_TAB DISCR_DEVICE_NO // ��բ�� 221
	#define GDDISCR_DEVICE_NO_TAB GDDISCR_DEVICE_NO // �ӵص�բ�� 222
	#define BS_DEVICE_NO_TAB BS_DEVICE_NO // ĸ�߱� 230
	#define LINE_DEVICE_NO_TAB LINE_DEVICE_NO // ��·�� 239
	#define ACLN_DEVICE_NO_TAB ACLN_DEVICE_NO // �����߶α� 240
	#define ACLN_DOT_NO_TAB ACLN_DOT_NO // �����߶ζ˵�� 242
	#define TR_DEVICE_NO_TAB TR_DEVICE_NO // ��ѹ���� 250
	#define TRWD_DEVICE_NO_TAB TRWD_DEVICE_NO // ��ѹ������� 251
	#define TAP_TYPE_NO_TAB TAP_TYPE_NO // ��ѹ���ֽ�ͷ���ͱ� 252
	#define GN_DEVICE_NO_TAB GN_DEVICE_NO // ������� 260
	#define LD_DEVICE_NO_TAB LD_DEVICE_NO // ���ɱ� 270
	#define RC_DEVICE_NO_TAB RC_DEVICE_NO // �ݿ����� 275
	#define DCSYSTEM_INFO_NO_TAB DCSYSTEM_INFO_NO // ֱ��ϵͳ�� 320
	#define DCCONV_DEVICE_NO_TAB DCCONV_DEVICE_NO // ֱ�������˶���� 321
	#define DCLN_DOT_NO_TAB DCLN_DOT_NO // ֱ���߶˱� 325
	#define DCLN_PIECE_NO_TAB DCLN_PIECE_NO // ֱ�����߱� 326
	#define DCGROUND_TERM_NO_TAB DCGROUND_TERM_NO // ֱ���ӵض� 329
	#define ISLANDANAABUS_NO_TAB ISLANDANAABUS_NO // �������ĸ�� 823
	#define STCASE_TMP_NO_TAB STCASE_TMP_NO // ���㷽����Դ�� 824
	#define ZONELFCASE_RES_NO_TAB ZONELFCASE_RES_NO // ������ʼ������������ 825
	#define LFEMGSTATICS_NO_TAB LFEMGSTATICS_NO // ��̬��������ͳ�ƽ���� 826
	#define EMGSTATICS_NO_TAB EMGSTATICS_NO // ��������ͳ�ƽ�� 827
	#define PVCURVERESULT_NO_TAB PVCURVERESULT_NO // PV����ȫ����� 828
	#define ELE_GRPUNIT_NO_TAB ELE_GRPUNIT_NO // �豸����ɱ� 829
	#define ELE_GRP_NO_TAB ELE_GRP_NO // �豸��� 830
	#define TEMPERATURERATE_NO_TAB TEMPERATURERATE_NO // �¶�ϵ���� 831
	#define ISLANDMARGINRESULT_NO_TAB ISLANDMARGINRESULT_NO // ������������� 832
	#define CONTGRP_NO_TAB CONTGRP_NO // ������� 833
	#define CCTCONT_RESULT_NO_TAB CCTCONT_RESULT_NO // CCT���Ͻ���� 834
	#define INTLINE_RES_NO_TAB INTLINE_RES_NO // �ؼ������֧·��ϵ�� 835
	#define INTERFACE_RES_NO_TAB INTERFACE_RES_NO // �ؼ������ 836
	#define LFCASEOVLDECI_NO_TAB LFCASEOVLDECI_NO // ��ʼ���ش�ʩ����� 837
	#define STCASEOVLDECI_NO_TAB STCASEOVLDECI_NO // Ԥ����ش�ʩ����� 838
	#define ISLANDINTCONT_NO_TAB ISLANDINTCONT_NO // �������Ͻ���� 839
	#define CASE_RESULT_STATICS_NO_TAB CASE_RESULT_STATICS_NO // ��ȫ�Է���ͳ����Ϣ�� 840
	#define FREQINTERFACE_NO_TAB FREQINTERFACE_NO // ����Ƶ�ʶ�������� 841
	#define FREQINTLINE_NO_TAB FREQINTLINE_NO // �������Ⱥ�֧·��ϵ�� 842
	#define UFV_ACTION_NO_TAB UFV_ACTION_NO // ��Ƶ��ѹ����װ�ö����� 843
	#define GENPROC_ACTION_NO_TAB GENPROC_ACTION_NO // ��������������� 844
	#define PRIMEINTCONT_NO_TAB PRIMEINTCONT_NO // �ؼ��������� 845
	#define CONT_ISLAND_NO_TAB CONT_ISLAND_NO // ���Ϻ����˵��� 847
	#define SCPCDMBUSRESULT_NO_TAB SCPCDMBUSRESULT_NO // ĸ�߶�·�����������߽�� 848
	#define SCLNPCDMRESULT_NO_TAB SCLNPCDMRESULT_NO // ��·��·�����������߽�� 849
	#define SCHEMETYPE_NO_TAB SCHEMETYPE_NO // ���ռƻ������ 850
	#define STINTFACELN_NO_TAB STINTFACELN_NO // ���޶�������·���� 851
	#define STINTERFACE_NO_TAB STINTERFACE_NO // EACCS���������Ƚ���� 852
	#define SENRESULT_NO_TAB SENRESULT_NO // �����ȼ����� 853
	#define ISLANDBUS_NO_TAB ISLANDBUS_NO // ���˵�ĸ�� 855
	#define ISLANDRESULT_NO_TAB ISLANDRESULT_NO // ���˷������ 856
	#define LEADSRHELE_NO_TAB LEADSRHELE_NO // �����豸����ǰ��ȽϽ���� 858
	#define LEADSRHGENRESULT_NO_TAB LEADSRHGENRESULT_NO // ����������н�� 859
	#define DCCTRLPAR_NO_TAB DCCTRLPAR_NO // ֱ������ϵͳ 860
	#define SCSET_NO_TAB SCSET_NO // ��·������������ 861
	#define SCBUSRESULT_NO_TAB SCBUSRESULT_NO // ĸ�߶�·������ 862
	#define SCLNRESULT_NO_TAB SCLNRESULT_NO // ��·��·������ 863
	#define SCBUSADJRESULT_NO_TAB SCBUSADJRESULT_NO // ĸ�߶�·����Ԫ�������� 864
	#define SCLNSADJRESULT_NO_TAB SCLNSADJRESULT_NO // ��·��·����Ԫ�������� 865
	#define SCBREAKRESULT_NO_TAB SCBREAKRESULT_NO // ��·����·������� 866
	#define TIMEPROC_NO_TAB TIMEPROC_NO // ��ʱ�������� 867
	#define EACCSINTPV_NO_TAB EACCSINTPV_NO // ����������PV����� 868
	#define PVCURVE_NO_TAB PVCURVE_NO // PV���߽���� 869
	#define SIMUINTERFACE_NO_TAB SIMUINTERFACE_NO // ����EACCS���������ȱ� 870
	#define LPSCASE_RESULT_NO_TAB LPSCASE_RESULT_NO // ������ڹ��ϼ���� 871
	#define LPSCONT_RESULT_NO_TAB LPSCONT_RESULT_NO // ������ڹ���������� 872
	#define LPSCCGEN_RES_NO_TAB LPSCCGEN_RES_NO // �����Ⱥ�б� 873
	#define CONTDECIDISP_NO_TAB CONTDECIDISP_NO // ���ϴ�ʩ��ʾ�� 874
	#define THDUFV_ACTION_NO_TAB THDUFV_ACTION_NO // ���������ߵ�Ƶ��ѹ����װ�ö����� 875
	#define THDGENPROC_ACTION_NO_TAB THDGENPROC_ACTION_NO // ���������߷�������������� 876
	#define THDLINEPROC_ACTION_NO_TAB THDLINEPROC_ACTION_NO // ������������·���������� 877
	#define EACCSMARLMT_NO_TAB EACCSMARLMT_NO // ϵͳ��ֵ�� 878
	#define THDCONT_RESULT_NO_TAB THDCONT_RESULT_NO // ����������У�˽�� 879
	#define ZONE_INFO_NO_TAB ZONE_INFO_NO // С�� 880
	#define CONT_FILTER_RULE_NO_TAB CONT_FILTER_RULE_NO // ����ɸѡ����� 892
	#define SIMUSYSPARA_NO_TAB SIMUSYSPARA_NO // ����ϵͳ�������� 893
	#define SSATMODE_NO_TAB SSATMODE_NO // С�Ŷ�ģʽ������� 894
	#define SSATMODESHAPE_NO_TAB SSATMODESHAPE_NO // С�Ŷ�����ģ̬��� 895
	#define SIMUCURVE_NO_TAB SIMUCURVE_NO // �������߽���� 896
	#define SIMUCURVEDISP_NO_TAB SIMUCURVEDISP_NO // ����������ʾ�� 897
	#define PCDMSTAINFO_NO_TAB PCDMSTAINFO_NO // Ԥ������ͳ����Ϣ 898
	#define EACCSINTORDER_NO_TAB EACCSINTORDER_NO // WAMAP��������������� 899
	#define EACCSCIRCLE_NO_TAB EACCSCIRCLE_NO // ��ϻ��� 900
	#define EACCSCIRCLELN_NO_TAB EACCSCIRCLELN_NO // ��ϻ���·��� 901
	#define EACCSCIRCLERULE_NO_TAB EACCSCIRCLERULE_NO // ��ϻ������� 902
	#define CURDISPRECORD_NO_TAB CURDISPRECORD_NO // ��ǰ��¼�� 903
	#define PCP_PROCESSINFO_NO_TAB PCP_PROCESSINFO_NO // ���ӽ�����Ϣ�� 904
	#define PCP_NETWORKINFO_NO_TAB PCP_NETWORKINFO_NO // WARMAP����״̬��Ϣ�� 905
	#define LINEPROC_ACTION_NO_TAB LINEPROC_ACTION_NO // ��·���������� 906
	#define EACCSECDONLINE_NO_TAB EACCSECDONLINE_NO // ����װ�����߲��� 907
	#define ACTDEVICEINFO_NO_TAB ACTDEVICEINFO_NO // ����װ�ò������ñ� 908
	#define PCP_CPULOADINFO_NO_TAB PCP_CPULOADINFO_NO // WARMAP_CPU���ڴ渺�ɱ� 909
	#define PCP_DISKINFO_NO_TAB PCP_DISKINFO_NO // WARMAP���̿ռ���Ϣ�� 910
	#define DSAS_PROCESSINFO_NO_TAB DSAS_PROCESSINFO_NO // ����Ԥ��������Ϣ�� 911
	#define DSAS_RUNINFO_NO_TAB DSAS_RUNINFO_NO // ����Ԥ��������Ϣ�� 912
	#define CONT_RESULT_SIMU_NO_TAB CONT_RESULT_SIMU_NO // �������������� 913
	#define CASE_EXCHANGE_INFO_NO_TAB CASE_EXCHANGE_INFO_NO // CASE������Ϣ�� 914
	#define SSAMARCURVE_NO_TAB SSAMARCURVE_NO // ��̬��ȫԣ������ 916
	#define SSAMODE_NO_TAB SSAMODE_NO // ģʽ������� 917
	#define SSAMODEFAC_NO_TAB SSAMODEFAC_NO // ģʽ�������� 918
	#define TRANSFERMODE_NO_TAB TRANSFERMODE_NO // ����ģʽ 919
	#define UFUVSHEDLOAD_NO_TAB UFUVSHEDLOAD_NO // UFUV���ؽ�� 932
	#define LEADSRHRULE_NO_TAB LEADSRHRULE_NO // ������༫���������� 951
	#define LOADGROUP_NO_TAB LOADGROUP_NO // ���������ݱ� 952
	#define LOADGRP_LOAD_NO_TAB LOADGRP_LOAD_NO // ������͸��ɹ�ϵ�� 953
	#define MUTUAL_DEVICE_NO_TAB MUTUAL_DEVICE_NO // ���򻥸� 954
	#define UFV_DEVICE_NO_TAB UFV_DEVICE_NO // ��Ƶ��ѹ������װ�� 955
	#define RU_DEVICE_NO_TAB RU_DEVICE_NO // ��Ƶ��ѹ��·�Ͽ����� 956
	#define GENPROT_DEVICE_NO_TAB GENPROT_DEVICE_NO // ����������� 957
	#define SCSDEV_NO_TAB SCSDEV_NO // �ȿ�װ�ö���� 958
	#define SCSSYS_NO_TAB SCSSYS_NO // �ȿ�ϵͳ����� 959
	#define GEN_LIB_NO_TAB GEN_LIB_NO // �������̬���� 961
	#define AVRE_LIB_NO_TAB AVRE_LIB_NO // ����E��ģ�Ͳ����� 962
	#define AVRF_LIB_NO_TAB AVRF_LIB_NO // ����F��ģ�Ͳ����� 963
	#define AVRF2_LIB_NO_TAB AVRF2_LIB_NO // ����F����ģ�Ͳ����� 964
	#define GOV_LIB_NO_TAB GOV_LIB_NO // ��������ԭ����ϵͳģ�ͱ� 965
	#define PSS_LIB_NO_TAB PSS_LIB_NO // ����ϵͳ�ȶ���S�� 966
	#define GENTG_LIB_NO_TAB GENTG_LIB_NO // �繦��ģ�Ͳ��� 967
	#define GENGV_LIB_NO_TAB GENGV_LIB_NO // ����ģ�Ͳ��� 968
	#define LOAD_LIB_NO_TAB LOAD_LIB_NO // ����ģ�Ͳ��� 969
	#define MOTO_LIB_NO_TAB MOTO_LIB_NO // ��Ӧ�綯����̬���� 970
	#define DC_LIB_NO_TAB DC_LIB_NO // ֱ������ģ�Ͳ����� 971
	#define DCMOD_LIB_NO_TAB DCMOD_LIB_NO // ֱ������ģ�Ͳ����� 972
	#define GENGB_LIB_NO_TAB GENGB_LIB_NO // ���ؽǿ���ϵͳģ�Ͳ��� 973
	#define RZ_LIB_NO_TAB RZ_LIB_NO // ��·�̶�����ģ�Ͳ��� 974
	#define SVC_LIB_NO_TAB SVC_LIB_NO // ��ֹ�޹����������� 975
	#define DECISIONCOST_NO_TAB DECISIONCOST_NO // ��ʩ���۱� 977
	#define MONITORLMTDEF_NO_TAB MONITORLMTDEF_NO // ������ֵ�� 978
	#define ADJCONSTRAIN_NO_TAB ADJCONSTRAIN_NO // ����Լ���� 979
	#define DEVICE_MONITOR_NO_TAB DEVICE_MONITOR_NO // ����Ԫ���� 980
	#define VOLT_DEVIATION_NO_TAB VOLT_DEVIATION_NO // �����ѹ��Ԫ�� 981
	#define FREQ_DEVIATION_NO_TAB FREQ_DEVIATION_NO // ����Ƶ�ʶ�Ԫ�� 982
	#define PRE_DECISION_NO_TAB PRE_DECISION_NO // Ԥ�����������ռ� 983
	#define LIMIT_DEFINATION_NO_TAB LIMIT_DEFINATION_NO // ��ֵ�����б� 984
	#define MATCH_INTERFACE_NO_TAB MATCH_INTERFACE_NO // ƥ������ 985
	#define MATCH_AREALINE_NO_TAB MATCH_AREALINE_NO // ƥ�������ɱ� 986
	#define NODE_TYDEF_NO_TAB NODE_TYDEF_NO // �ڵ����ͱ� 987
	#define INTERFACE_NO_TAB INTERFACE_NO // WAMAP���������ȱ� 988
	#define INTLINE_NO_TAB INTLINE_NO // WAMAP���������Ⱥ�֧·��ϵ�� 989
	#define LMT_DECISION_NO_TAB LMT_DECISION_NO // ���޹��������ռ� 990
	#define PCP_MANAGENODEINFO_NO_TAB PCP_MANAGENODEINFO_NO // ����ڵ�������Ϣ�� 991
	#define PCP_COMPUTENODEINFO_NO_TAB PCP_COMPUTENODEINFO_NO // ����ڵ�������Ϣ�� 992
	#define PCP_COMMUNICATENODEINFO_NO_TAB PCP_COMMUNICATENODEINFO_NO // ͨ�Žڵ�������Ϣ�� 993
	#define PCP_ALARMINFO_NO_TAB PCP_ALARMINFO_NO // �澯��Ϣ�� 994
	#define PCP_DOMAININFO_NO_TAB PCP_DOMAININFO_NO // ��������Ϣ�� 995
	#define PCP_NODEPROCESSINFO_NO_TAB PCP_NODEPROCESSINFO_NO // ��ɫӦ�ý���������Ϣ�� 996
	#define CONTINT_NO_TAB CONTINT_NO // ���Ϻ����ȹ�ϵ�� 997
	#define PCP_NODERUNSTATUSINFO_NO_TAB PCP_NODERUNSTATUSINFO_NO // �ڵ�����״̬������Ϣ�� 998
	#define PCP_CLUSTERNODEINFO_NO_TAB PCP_CLUSTERNODEINFO_NO // ��Ⱥ�ڵ���Ϣ�� 999
	#define THDACTCONT_RESULT_NO_TAB THDACTCONT_RESULT_NO // ��������������ͳ�ƽ���� 1748
	#define LFPCDMSENELE_RES_NO_TAB LFPCDMSENELE_RES_NO // У�����������Ƚ�� 1749
	#define PCDMSENELE_RES_NO_TAB PCDMSENELE_RES_NO // Ԥ�����������Ƚ�� 1750
	#define BL_EVENT_NO_TAB BL_EVENT_NO // ĸ�߼�֧·�¼��� 1751
	#define EMG_DECITION_NO_TAB EMG_DECITION_NO // ���������л����и��ɴ�ʩ 1752
	#define GENEVENT_NO_TAB GENEVENT_NO // ��������� 1753
	#define DC_EVENT_NO_TAB DC_EVENT_NO // ֱ���߹��� 1754
	#define SSATMODECOMP_NO_TAB SSATMODECOMP_NO // С�Ŷ���������ǰ��Ƚ� 1755
	#define SSATPREADJUST_RES_NO_TAB SSATPREADJUST_RES_NO // С���Ÿ������߱� 1756
	#define SSATDAMPGEN_RES_NO_TAB SSATDAMPGEN_RES_NO // С������������Ⱥ 1757
	#define LFPCDMSTATICS_NO_TAB LFPCDMSTATICS_NO // ��ʼ������������ͳ�ƽ�� 1758
	#define LFPREADJUST_RES_NO_TAB LFPREADJUST_RES_NO // ��̬�������ߵ����б� 1759
	#define FAULT_AUTOGEN_NO_TAB FAULT_AUTOGEN_NO // ���߹������ɲ��� 1760
	#define HISFAULT_RULE_NO_TAB HISFAULT_RULE_NO // ��ʷ�������ɹ��� 1761
	#define SIMUCURVEAUTORUN_NO_TAB SIMUCURVEAUTORUN_NO // ��������ʵʱ�� 1762
	#define FAULT_TIMET_NO_TAB FAULT_TIMET_NO // ����ʱ��� 1763
	#define CONTINGENCY_NO_TAB CONTINGENCY_NO // ������ 1764
	#define CONT_EVENT_NO_TAB CONT_EVENT_NO // �������¼���ϵ�� 1765
	#define CONT_DECITION_NO_TAB CONT_DECITION_NO // ���Ϻͺ�ѡ��ʩ����ϵ�� 1766
	#define LFCASE_NO_TAB LFCASE_NO // ��ʼ������������� 1767
	#define LFCASE_RES_NO_TAB LFCASE_RES_NO // ��ʼ������������ 1768
	#define LFCASE_PARA_NO_TAB LFCASE_PARA_NO // ����������� 1769
	#define LFOVERELE_RES_NO_TAB LFOVERELE_RES_NO // ��ʼ����Ԫ���������б� 1770
	#define SENELE_RES_NO_TAB SENELE_RES_NO // ��̬Ԫ�������ȷ��� 1771
	#define PREADJUST_RES_NO_TAB PREADJUST_RES_NO // Ԥ�����ƹ��ʵ����б� 1772
	#define DECITIONGRP_NO_TAB DECITIONGRP_NO // ��ѡ��ʩ��� 1773
	#define LFEMG_RES_NO_TAB LFEMG_RES_NO // ��̬�������Ʋ��Խ�� 1774
	#define LINEFLOW_COMP_NO_TAB LINEFLOW_COMP_NO // ��·�����Ƚ��б� 1775
	#define BUSVOLT_COMP_NO_TAB BUSVOLT_COMP_NO // ĸ�ߵ�ѹ�Ƚ��б� 1776
	#define MATCH_RESULT_NO_TAB MATCH_RESULT_NO // ƥ������ 1777
	#define LMTSTATICS_NO_TAB LMTSTATICS_NO // ���޹���ͳ�ƽ�� 1778
	#define INTADJLIST_NO_TAB INTADJLIST_NO // ���湦�ʵ����б� 1779
	#define STCASE_NO_TAB STCASE_NO // ���㷽������� 1780
	#define STCASE_PARA_NO_TAB STCASE_PARA_NO // ��ȫ�Է���������Ϣ 1781
	#define CONT_PARA_NO_TAB CONT_PARA_NO // ����������Ϣ 1782
	#define STCASE_CONT_NO_TAB STCASE_CONT_NO // �ȶ����㷽���͹��Ϲ�ϵ�� 1783
	#define STCASETSK_NO_TAB STCASETSK_NO // ���㷽������� 1784
	#define CASE_RESULT_NO_TAB CASE_RESULT_NO // ���ڹ��ϼ������ 1785
	#define CONT_RESULT_NO_TAB CONT_RESULT_NO // ���ڹ���������� 1786
	#define EMGSENELE_RES_NO_TAB EMGSENELE_RES_NO // �������������Ƚ�� 1787
	#define PCDMOVERELE_RES_NO_TAB PCDMOVERELE_RES_NO // Ԥ������ǰ��Ԫ���������б� 1788
	#define LFPCDMOVERELE_RES_NO_TAB LFPCDMOVERELE_RES_NO // У������ǰ��Ԫ���������б� 1789
	#define FAKEVB_RES_NO_TAB FAKEVB_RES_NO // ��ѹ����ĸ���б� 1790
	#define FAKEFB_RES_NO_TAB FAKEFB_RES_NO // Ƶ�ʱ���ĸ���б� 1791
	#define CCGEN_RES_NO_TAB CCGEN_RES_NO // ��Ⱥ�б� 1792
	#define DAMPMODE_RES_NO_TAB DAMPMODE_RES_NO // ��ģʽ�� 1793
	#define DAMPGEN_RES_NO_TAB DAMPGEN_RES_NO // ��������Ⱥ 1794
	#define EMG_RES_NO_TAB EMG_RES_NO // ���Ʋ���������� 1795
	#define OVERELE_RES_NO_TAB OVERELE_RES_NO // Ԫ���������б� 1796
	#define CONTSENELE_RES_NO_TAB CONTSENELE_RES_NO // Խ��Ԫ�������ȷ������ 1797
	#define CONCONT_RES_NO_TAB CONCONT_RES_NO // �ɿز��ɿع����б� 1798
	#define MARGINCURVE_NO_TAB MARGINCURVE_NO // ԣ������ 1799
	#define PFRESULTCOMPARE_NO_TAB PFRESULTCOMPARE_NO // ����ǰ�����Ƚ� 2012
	#define CASE_RESULT2_NO_TAB CASE_RESULT2_NO // ���ڹ��ϼ�������� 2013
	#define MODEL_PARA_BIAS_NO_TAB MODEL_PARA_BIAS_NO // ����ת���� 2014
	#define SCS_SETVALUE_RESULT_NO_TAB SCS_SETVALUE_RESULT_NO // �ȿ�ϵͳ��ֵ������ 2015
	#define SCS_SETVALUE_ADJ_RESULT_NO_TAB SCS_SETVALUE_ADJ_RESULT_NO // �ȿ�ϵͳ��ֵ���������� 2016
	#define LOSS_STATICS_NO_TAB LOSS_STATICS_NO // ����ǰ������ȽϽ���� 2017
	#define LFCASE_CMPNY_NO_TAB LFCASE_CMPNY_NO // ��˾����ͳ�� 2018
	#define LFCASEDCLN_NO_TAB LFCASEDCLN_NO // ֱ����·���� 2019
	#define LFCASE_AREA_NO_TAB LFCASE_AREA_NO // ������ 2020
	#define LFCASE_STAT_NO_TAB LFCASE_STAT_NO // ��վ���� 2021
	#define LFCASEBUS_NO_TAB LFCASEBUS_NO // ĸ�ߵ�ѹ��� 2022
	#define LFCASELN_NO_TAB LFCASELN_NO // ��·���� 2023
	#define LFCASETRANSFORMER_NO_TAB LFCASETRANSFORMER_NO // ��ѹ������ 2024
	#define LFCASEWIND_NO_TAB LFCASEWIND_NO // ���鳱�� 2025
	#define LFCASECAP_NO_TAB LFCASECAP_NO // �ݿ���������� 2026
	#define LFCASEGEN_NO_TAB LFCASEGEN_NO // ���糱����� 2027
	#define LFCASEINT_NO_TAB LFCASEINT_NO // ���泱����� 2028
	#define LFCASEINTLN_NO_TAB LFCASEINTLN_NO // ������·���� 2029
	#define SCSFAC_NO_TAB SCSFAC_NO // �ȿ�վ��� 2030
	#define SCSPLATE_NO_TAB SCSPLATE_NO // �ȿ�װ��ѹ��� 2031
	#define SCSINF_NO_TAB SCSINF_NO // �ȿ�װ�ö���� 2032
	#define SCSINFUNIT_NO_TAB SCSINFUNIT_NO // �ȿ�װ�ö�����ɱ� 2033
	#define SCSOPMODE_NO_TAB SCSOPMODE_NO // ���з�ʽ�� 2034
	#define SCSSETVAL_NO_TAB SCSSETVAL_NO // ��ֵ�� 2036
	#define CALFORMULA_NO_TAB CALFORMULA_NO // ��ʽ�� 2037
	#define SCSOFFDECI_NO_TAB SCSOFFDECI_NO // �ȿ�����վ���Ա� 2038
	#define SCSSEARCHRESULT_NO_TAB SCSSEARCHRESULT_NO // ���߲���������� 2039
	#define BACKOFFDECI_NO_TAB BACKOFFDECI_NO // �����Ͳ��Ա� 2040
	#define SCSCTRLDECI_NO_TAB SCSCTRLDECI_NO // ���Ʋ��Ա� 2041
	#define SCSACTGRP_NO_TAB SCSACTGRP_NO // �ȿض����� 2042
	#define SCSDECI_NO_TAB SCSDECI_NO // �ȿ�ִ�в��Ա� 2043
	#define SCSORDER_NO_TAB SCSORDER_NO // �ȿض����ռ� 2044
	#define SCSJUDGEUNIT_NO_TAB SCSJUDGEUNIT_NO // �ȿ�װ���оݵ�Ԫ�� 2045
	#define SCSJUDGE_NO_TAB SCSJUDGE_NO // �ȿ�װ���оݱ� 2046
	#define BATS_JUDGE_NO_TAB BATS_JUDGE_NO // ����Ͷװ�ö����оݱ� 2047
	#define BATSCASE_NO_TAB BATSCASE_NO // ����ͶУ�˷��� 2048
	#define BATS_ACTION_NO_TAB BATS_ACTION_NO // ����Ͷװ�ö����� 2049
	#define FLORESULTFILE_NO_TAB FLORESULTFILE_NO // ��������ļ���Ϣ�� 2050
	#define LFSTAANGRLT_NO_TAB LFSTAANGRLT_NO // ��̬�����ȶ������ 2051
	#define CDQPFRESULTCOMPARE_NO_TAB CDQPFRESULTCOMPARE_NO // ������Ԥ��ǰ�����Ƚ� 2052
	#define CDQGENERALMES_NO_TAB CDQGENERALMES_NO // �����ڸ�Ҫ��Ϣ�� 2053
	#define LFO_PARA_NO_TAB LFO_PARA_NO // LFO������ 1930
	#define LFO_FAC_NO_TAB LFO_FAC_NO // LFO��վ�� 1931
	#define LFO_GN_NO_TAB LFO_GN_NO // LFO������� 1932
	#define LFO_ACLN_NO_TAB LFO_ACLN_NO // LFO������·�� 1933
	#define LFO_RT_CURVE_NO_TAB LFO_RT_CURVE_NO // LFOʵʱ���߱� 1934
	#define LFO_ALLDEV_MODE_NO_TAB LFO_ALLDEV_MODE_NO // LFOʵʱ�豸ģʽȫ�� 1935
	#define LFO_DEV_LEADMODE_NO_TAB LFO_DEV_LEADMODE_NO // LFOʵʱ�豸����ģʽ�� 1936
	#define LFO_WDS_SHAPE_NO_TAB LFO_WDS_SHAPE_NO // LFOʵʱģ̬�� 1937
	#define LFO_CASE_WDS_NO_TAB LFO_CASE_WDS_NO // LFOʵʱCASE���ڱ� 1938
	#define LFO_CASE_ALLSHAPE_NO_TAB LFO_CASE_ALLSHAPE_NO // LFOʵʱCASEģ̬ȫ�� 1939
	#define LFO_TR_NO_TAB LFO_TR_NO // LFO��ѹ���� 1940
	#define LFO_INTERFACE_NO_TAB LFO_INTERFACE_NO // LFO���涨��� 1941
	#define LFO_INTERFACE_DEV_NO_TAB LFO_INTERFACE_DEV_NO // LFO������ɱ� 1942
	#define LFO_ASST_DECI_NO_TAB LFO_ASST_DECI_NO // LFO�������߱� 1943
	#define LFO_DECI_INDEX_NO_TAB LFO_DECI_INDEX_NO // LFO���������� 1944
	#define LFO_CTL_DECI_NO_TAB LFO_CTL_DECI_NO // LFO���Ա� 1945
	#define LFO_LPO_PARA_NO_TAB LFO_LPO_PARA_NO // LFO_LPO������ 1946
	#define LFO_LPO_EVE_NO_TAB LFO_LPO_EVE_NO // LFO_LPO��Ϣ�� 1947
	#define LFO_LPO_CASE_NO_TAB LFO_LPO_CASE_NO // LFO_LPO��ϸ��Ϣ�� 1948
	#define LFO_CTL_ACT_NO_TAB LFO_CTL_ACT_NO // LFO���ƶ���� 1949
	#define LFO_WARN_NO_TAB LFO_WARN_NO // LFO���Ӹ澯�� 2315
	#define LFO_LPO_WARN_NO_TAB LFO_LPO_WARN_NO // LFO_LPO�澯�� 2316
	#define PQ_DA_CTL_PARA_NO_TAB PQ_DA_CTL_PARA_NO // VF��̬���������� 1950
	#define PQ_DA_BUS_NO_TAB PQ_DA_BUS_NO // VF��̬����ĸ�߱� 1951
	#define PQ_DA_STAT_NO_TAB PQ_DA_STAT_NO // VF��̬����״̬�� 1952
	#define PQ_DA_VOLT_INFO_NO_TAB PQ_DA_VOLT_INFO_NO // VF��̬������ѹ��Ϣ�� 1954
	#define PQ_DA_FREQ_INFO_NO_TAB PQ_DA_FREQ_INFO_NO // VF��̬����Ƶ����Ϣ�� 1955
	#define PQ_DA_MAR_NO_TAB PQ_DA_MAR_NO // ��̬Ʒ������ԣ�ȱ� 1956
	#define PQ_DA_VOLT_WARN_NO_TAB PQ_DA_VOLT_WARN_NO // ��̬Ʒ��������ѹ�澯�� 2313
	#define PQ_DA_FREQ_WARN_NO_TAB PQ_DA_FREQ_WARN_NO // ��̬Ʒ������Ƶ�ʸ澯�� 2314
	#define WAM_MV_PARA_NO_TAB WAM_MV_PARA_NO // WAM_MV������ 1960
	#define WAM_MV_ACLN_NO_TAB WAM_MV_ACLN_NO // WAM_MV������·�� 1961
	#define WAM_MV_TR_NO_TAB WAM_MV_TR_NO // WAM_MV��ѹ���� 1962
	#define WAM_MV_DEVCUV_NO_TAB WAM_MV_DEVCUV_NO // WAM_MV�豸���߱� 1963
	#define WAM_MV_CURVE_NO_TAB WAM_MV_CURVE_NO // WAM_MVʵʱ���߱� 1964
	#define	WAM_AM_PARA_NO_TAB 1966 //WAM_AM_PARA_NO //	1966	/* WAM_AM������ */
	#define	WAM_AM_DEV_NO_TAB 1967 //WAM_AM_DEV_NO  //	1967	/* WAM_AM�豸�� */
	#define	WAM_AM_CURVE_NO_TAB 1968  //WAM_AM_CURVE_NO	//1968	/* WAM_AM���߱� */
	#define LFO_MODE_INF_NO_TAB LFO_MODE_INF_NO // ʵ����ģʽ�� ����
	#define LFO_DEV_INF_NO_TAB LFO_DEV_INF_NO // ʵ�����豸�� ����
	#define DSA_SSATPMUCOMP_NO_TAB DSA_SSATPMUCOMP_NO // ��Ƶ��С�Ŷ�����ƥ���� ����
	#define WAMS_OSC_INFO_NO_TAB WAMS_OSC_INFO_NO // WAMS��Ƶ����Ϣ�� ����
	#define DSA_N1OUTAGE_NO_TAB DSA_N1OUTAGE_NO // EACCS�豸����ѡ�� ����
	#define DSA_N1RESULT_NO_TAB DSA_N1RESULT_NO // EACCS��EMS���Ͻ���Ƚ� ����
	#define WAM_FES_YC_DEFINE_NO_TAB WAM_FES_YC_DEFINE_NO // WAMSǰ��ң�ⶨ��� 1813
	#define WAM_CHANNEL_INFO_NO_TAB WAM_CHANNEL_INFO_NO // WAMSͨ���� 612
	#define VALUE_INFO_NO_TAB VALUE_INFO_NO // ���ң����Ϣ�� 307
    #define WAMS_APP_WARN_INFO_NO_TAB APP_VARIABLE_NO // Ӧ�ñ����� 190
	#define PRE_PVSSAMODE_NO_TAB PRE_PVSSAMODE_NO  // Ԥ����ϱǵ��ѹģʽ������� 716
	#define PRE_PVSSAMODEFAC_NO_TAB PRE_PVSSAMODEFAC_NO  // Ԥ����ϱǵ�ģʽ�������ӱ� 717
	#define	PVCURVERESULT_NO_TAB PVCURVERESULT_NO  //	828	/* PV����ȫ����� */
	#define	EME_EACCSINTPV_NO_TAB  EME_EACCSINTPV_NO  //	719	/* ��̬����������PV���߽���� */
	#define	EME_PVCURVE_NO_TAB EME_PVCURVE_NO  //	718	/* ��̬PV���߽���� */
	#define	EME_PVCURVERESULT_NO_TAB EME_PVCURVERESULT_NO  //	720	/* ��̬PV����ȫ����� */
	#define EME_PVSSAMODE_NO_TAB EME_PVSSAMODE_NO // ��̬�ǵ��ѹģʽ��������� 721
	#define EME_PVSSAMODEFAC_NO_TAB EME_PVSSAMODEFAC_NO  // ��̬�ǵ�ģʽ�������ӱ� 722
	#define	SCMS_SYSTEM_INFO_NO_TAB SCMS_SYSTEM_INFO_NO	//9458	/* SCMSϵͳ��Ϣ�� */
	#define	DUTYDECI_NO_TAB DSA_DUTYDECI_NO	// 9467	/* �ȿ�ϵͳ��ֵ���Ա� */
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
// ������Ϣ�ṹ
typedef struct type_TRIGGER_SAMPLE_BASE
{
	INT_REC keyid;  // ID��
	int record_id;  // ��¼��(open3000)
	int column_id;  // ���(open3000)
	int vtype;  // ֵ����
	INT_REC vint;  // ����ֵ
	float vfloat;  // ����ֵ
	char vstring[128];  // �ַ���ֵ(ֻ֧��128λ�ַ�)
}TRIGGER_SAMPLE_BASE;

typedef struct type_TRIGGER_SAMPLE_DEFINE
{
	int tableno; // ���
	int sampleno;  // ������
	int appno;  // Ӧ�ú�
	int mode;  // 0,�򼶲���;1,���򼶲���
	RTDB_TIME occur_time;  // ����ʱ��
	vector<TRIGGER_SAMPLE_BASE> vecsmp;
}TRIGGER_SAMPLE_DEFINE;
//////////////////////////////////////////////////////////////////////////
//�澯��Ϣ�ṹ
typedef struct type_WARN_MESSAGE_BASE
{
	int vtype;  // ֵ����
	RTDB_TIME vtime;  // ʱ��
#ifdef _D5000
	INT_REC vlong;  // ������ֵ
#endif
	int vint;  // ����ֵ
	float vfloat;  // ����ֵ
	char vstring[200];  // �ַ���ֵ(ֻ֧��200λ�ַ�)
}WARN_MESSAGE_BASE;

typedef struct type_WARN_MESSAGE_STRUCT
{
	int warntype; // �澯��
	int appno;  // Ӧ�ú�
	vector<WARN_MESSAGE_BASE> vecwarn;
}WARN_MESSAGE_STRUCT;
//////////////////////////////////////////////////////////////////////////
// �����豸��
/* ��ѹ���ͱ� */
typedef struct type_VL_TYPE_TAB_CRE
{
	VOL_INT_REC vlty_id;	/* ��ѹ����ID�� */
#ifdef _NUSP
#ifdef _KEDONG
	char vlty_name[64];     /* ��ѹ�������� */
#else
	char vlty_name[128];	/* ��ѹ�������� */
#endif // _KEDONG
#else
	char vlty_name[64];	/* ��ѹ�������� */
#endif // _NUSP
	float vbase;	/* ��ѹ��ֵ */
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

// �������
typedef struct type_GEN_TAB_CRE
{
	INT_REC        gn_id;                                       //  �����ID��
	char           gn_name[64];                                 //  ���������   
	INT_REC        fac_id;                                      //  ��վID��
	VOL_INT_REC   vlty_id;	/* ��ѹ����ID�� */
#ifdef _NUSP
	int		gn_type; /* ��������� */
#else
	unsigned char gn_type;	/* ��������� */
#endif // _NUSP
	float pmax;	/* �����й����� */
	float pmin;	/* �����й����� */
#ifdef _NUSP
	//float pnom;	/* �������� */
#else
	float pnom;	/* �������� */
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

// �����߶ζ˵��
typedef struct type_ACLNDOT_TAB_CRE
{
	INT_REC        acln_dot_id;                                 //  �����߶ζ˵�ID��
#ifdef _NUSP
#ifdef _KEDONG
	char           acln_dot_name[64];                           //  �����߶ζ˵�����
#else
	char		   acln_dot_name[128];                           //  �����߶ζ˵�����
#endif // _KEDONG
#else
	char		   acln_dot_name[64];                           //  �����߶ζ˵�����
#endif // _NUSP	
	INT_REC        fac_id;                                      //  ��վID��
	INT_REC        acln_id;                                     //  ���������߶�ID��
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

// �����߶α�
typedef struct type_ACLINE_TAB_CRE
{
	INT_REC        acln_id;                                     //  �����߶�ID��
	char           acln_name[64];                               //  �����߶�����
	INT_REC        fac_id_s;                                    //  һ�˳�վID��
	INT_REC        fac_id_e;                                    //  ���˳�վID��
	VOL_INT_REC    vlty_id;	                                   //  ��ѹ����ID��
	float			imax1;									/* �������� */
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

// ĸ�߱�
typedef struct  type_BUS_TAB_CRE
{
	INT_REC           bs_id;                                    //  ĸ��ID
	char              bs_name[64];                              //  ĸ������
	INT_REC           fac_id;                                   //  ��վID
	VOL_INT_REC       vlty_id;	                                   //  ��ѹ����ID��
	int               bs_type;                                  //  ĸ������
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

// ��վ��Ϣ��
#ifdef _NUSP
// ���糧������Ϣ��
typedef struct type_PLANT_TAB_CRE
{
	INT_REC        fac_id;                                      //  �糧ID
	char           fac_name[64];                                //  ���糧����
	VOL_INT_REC    vlty_id;					//  ��ѹ�ȼ�
	INT_REC        area_id;                                     //  �������
	int            fac_type;                                    //  ��վ����
}PLANT_TAB_CRE;
#ifdef _KEDONG
#define PLANT_TAB_CRE_FLD "id,name,max_voltage_type,connective_pg_id,plant_type"
#else
#define PLANT_TAB_CRE_FLD "id,name,voltage_type,pg_id,st_type"
#endif // _KEDONG
// ���վ������Ϣ��
typedef struct type_SUBSTATION_TAB_CRE
{
	INT_REC        fac_id;                                      //  ���վID
	char           fac_name[64];                                //  ���վ����
	VOL_INT_REC    vlty_id;					//  ������ߵ�ѹ�ȼ�
	INT_REC        area_id;                                     //  �������
	int            fac_type;                                    //  ��վ����
}SUBSTATION_TAB_CRE;
#ifdef _KEDONG
#define SUBSTATION_TAB_CRE_FLD "id,name,top_ac_voltage_type,pg_id,type"
#else
#define SUBSTATION_TAB_CRE_FLD "id,name,voltage_type,pg_id,st_type"
#endif // _KEDONG

typedef struct type_FAC_TAB_CRE
{
	INT_REC        fac_id;                                      //  ��վID��
	char           fac_name[64];                                //  ��վ����
	VOL_INT_REC    vlty_id;                                       /* ��ߵ�ѹ����ID */
	INT_REC        area_id;                                     //  ����ID
	int            fac_type;                                    //  ��վ����
}FAC_TAB_CRE;
#define FAC_TAB_CRE_FLD "fac_id,fac_name,vlty_id,pg_id,fac_type"
#else
typedef struct type_FAC_TAB_CRE
{
	INT_REC        fac_id;                                      //  ��վID��
	char           fac_name[64];                                //  ��վ����
	VOL_INT_REC    vlty_id;                                       /* ��ߵ�ѹ����ID */
	INT_REC        area_id;                                     //  ����ID
	int            fac_type;                                    //  ��վ����
}FAC_TAB_CRE;
#ifndef _D5000
#define FAC_TAB_CRE_FLD "fac_id,fac_name,vlty_id,area_id,fac_type"
#else
#define FAC_TAB_CRE_FLD "id,name,bv_id,subarea_id,st_type"
#endif // _D5000
#endif // _NUSP

//���������  /* ����������Ϣ */
typedef struct  type_AREA_TAB_CRE
{
	INT_REC        area_id;                                     //  ����ID
#ifdef _NUSP
#ifdef _KEDONG
	char		area_name[64];					// ��������
#else
	char           area_name[128];                               //  ��������
#endif // _KEDONG
#else
	char           area_name[64];                               //  ��������
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

/* ��ѹ���� */
typedef struct type_TR_TAB_CRE
{
	INT_REC tr_id;	/* ��ѹ��ID�� */
	char tr_name[64];	/* ��ѹ������ */
	INT_REC fac_id;	/* ��վID�� */
#ifdef _NUSP
	int trty;                              /* �������� */
	//unsigned char tr_type;		/* ��ѹ������ */
#else
	unsigned char trty;                   /* �������� */
	unsigned char tr_type;	/* ��ѹ������ */
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

/* ��ѹ������� */
typedef struct type_TRWD_TAB_CRE
{
	INT_REC trwd_id;	/* ��ѹ������ID�� */
	char trwd_name[64];	/* ��ѹ���������� */
	INT_REC fac_id;	/* ��վID�� */
	INT_REC tr_id;	/* ��ѹ��ID�� */
	VOL_INT_REC vlty_id;	/* ��ѹ����ID�� */
#ifdef _NUSP
	//INT_REC vlle_id;	/* ��ѹ�ȼ�ID�� */
	int trwd_type;        /* ��ѹ���������� */
#else
	INT_REC vlle_id;	/* ��ѹ�ȼ�ID�� */
	unsigned char trwd_type;        /* ��ѹ���������� */
#endif // _NUSP		
	float	snom;		/* ������������� */
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

/* ���ң����Ϣ�� */
typedef struct type_VALUEINFO_TAB_CRE
{
	INT_REC value_id;	/* ���ID�� */
	INT_REC fac_id;	/* ��վID�� */
	INT_REC value_name[64];	/* ������� */
	unsigned char value_type;	/* ���� */
}VALUEINFO_TAB_CRE;
#ifndef _D5000
#define VALUEINFO_TAB_CRE_FLD "value_id,fac_id,value_name,value_type"
#else  //D5000û�ж�ȡ�����
#define VALUEINFO_TAB_CRE_FLD "value_id,fac_id,value_name,value_type"
#endif

// WAMSǰ��ң�ⶨ���
#ifndef _D5000
typedef struct  type_FES_YC_TAB_CRE
{
	KEY_ID_STRU     yc_id;               //ң��ID
	int		fac_id;		     /* ��վID */
	short           dot_no;              //���
	int             file_no;             //�����洢�ļ���
	int             col_no;              //�����ļ��к�
}FES_YC_TAB_CRE;
#define FES_YC_TAB_CRE_FLD "yc_id,fac_id,dot_no,file_no,col_no"

#else

#ifdef _KEDONG
typedef struct  type_FES_YC_TAB_CRE
{
    INT_REC     yc_id;               //���ݵ���
    INT_REC     fac_id;              //��վ��
    int			dot_no;				// ���ݵ��
    int			file_no;             // PMU���ʵʱ���ַ
    int			col_no;              // ����(PMU���ʵʱ���ַ)
}FES_YC_TAB_CRE;
#define FES_YC_TAB_CRE_FLD "psid,st_id,localno,record_id,record_id"
// WAMSǰ��ң�Ŷ����
typedef struct  type_FES_YX_TAB_CRE
{
	INT_REC         st_id;                /* ��վID */
	INT_REC         yx_id;               //ң��ID
	int				dot_no;              //���
	int             file_no;             //�����洢�ļ���
	int             col_no;              //�����ļ��к�
}FES_YX_TAB_CRE;
#define FES_YX_TAB_CRE_FLD "st_id,psid,localno,record_id,record_id"
#else
typedef struct  type_FES_YC_TAB_CRE
{
	INT_REC			yc_id;				//ң��ID
	INT_REC			fac_id;				//��վID
	short			dot_no;				//���
	int				file_no;			//�����洢�ļ���
	int				col_no;				//�����ļ��к�
}FES_YC_TAB_CRE;
#define FES_YC_TAB_CRE_FLD "yc_id,st_id,dot_no,file_no,col_no"
// WAMSǰ��ң�Ŷ����
typedef struct  type_FES_YX_TAB_CRE
{
	INT_REC         st_id;				 /* ��վID */
	INT_REC         yx_id;               //ң��ID
	short           dot_no;              //���
	int             file_no;             //�����洢�ļ���
	int             col_no;              //�����ļ��к�
}FES_YX_TAB_CRE;
#define FES_YX_TAB_CRE_FLD "st_id,yx_id,dot_no,file_no,col_no"
#endif // _KEDONG

// WAMSǰ��ң��ң�źϲ��ṹ��
typedef struct  type_FES_YX_YC
{
	INT_REC				st_id;			  /* ��վID */
   	INT_REC				yc_id;             //ң��ң�źϲ�INT
    short				dot_no;              //���
	int					file_no;             //�����洢�ļ���
    int					col_no;              //�����ļ��к�
}FES_YX_YC;

// WAMSǰ��ͨѶ��վ
typedef struct  type_FES_COMM
{
	INT_REC comm_fac_id;	/* ͨѶ��վID */
	INT_REC st_id;	/* ��վID */
	char fac_name[64];	/* ͨѶ��վ���� */
}FES_COMM_TAB_CRE;
#define FES_COMM_TAB_CRE_FLD "comm_fac_id,st_id,fac_name"

#endif // !_D5000
//////////////////////////////////////////////////////////////////////////
// ���ϱ�

#endif
