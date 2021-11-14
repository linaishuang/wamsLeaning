#ifndef _WAM_MV_STRUCT_H_
#define _WAM_MV_STRUCT_H_

#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// �궨��
#ifndef WT_MENU_NO
#define WT_MENU_NO  0
#define WT_MENU_YES 1
#endif

#ifndef WT_MODE_NEW
#define WT_MODE_TEST 1      // �����豸��Ϣ�����޸����ÿ�
#define WT_MODE_NEW  2      // �����豸
#define WT_MODE_ALL  3      // �豸ȫ������
#endif

//////////////////////////////////////////////////////////////////////////
typedef struct type_WT_PARA_INFO
{
	int   nMode;     // 1,�����豸;
	                   // 2,���ԭ���豸��ȫ������;
	int   nCurveMode; // �����豸���߱�
	float VBase;     // ��ѹ�ȼ������ڸõ�ѹ�ȼ����豸��Ϊ��ѡ�豸
	int   IsFacUpdate;  // �Ƿ����LFO��վ��
	int   IsGenUpdate;  // �Ƿ����LFO�������
	int   IsAclnUpdate; // �Ƿ����LFO������·��
	int   IsTrUpdate;   // �Ƿ����LFO��ѹ����
	int   IsVFDevUpdate;  // �Ƿ����VF��̬����ĸ�߱�
	int   IsAmGenUpdate;  // �Ƿ����WAM_AM�豸��

// 	int  nBusAll;  // ĸ������
// 	int  nBusPmu;  // װ��PMUĸ�߸���
// 	int  nBusOld;  // VF��̬����ĸ�߱��м�¼����
// 	int  nBusNew;  // ����ĸ�߸���

	int  nFacAll;  // ��վ����
	int  nFacPmu;  // װ��PMU��վ����
	int  nFacOld;  // LFO��վ���м�¼����
	INT_REC MAX_FacOld_idx;  // LFO��վ���м�¼�������idx
	int  nFacNew;  // ������վ����
	
	int  nGenAll;  // ���������
	int  nGenPmu;  // װ��PMU���������
	int  nGenOld;  // LFO��������м�¼����
	INT_REC MAX_GenOld_idx;  // LFO��������м�¼�������idx
	int  nGenNew;  // �������������

	int  nAclnAll;  // ������·����
	int  nAclnPmu;  // װ��PMU������·����
	int  nAclnOld;  // LFO������·���м�¼����
	INT_REC MAX_AclnOld_idx;  // LFO������·���м�¼�������idx
	int  nAclnNew;  // ����������·����

	int  nTrAll;  // ��ѹ������
	int  nTrPmu;  // װ��PMU��ѹ������
	int  nTrOld;  // LFO��ѹ�����м�¼����
	INT_REC MAX_TrOld_idx;  // LFO��ѹ�����м�¼�������idx
	int  nTrNew;  // ������ѹ������
	
	int  nVFAll;  // VF�豸����
	int  nVFPmu;  // װ��PMU���豸����
	int  nVFOld;  // VF�豸���м�¼����
	INT_REC MAX_VFOld_idx;  // VF�豸���м�¼�������idx
	int  nVFNew;  // �����豸����

	char GenInfoFile[MAXBUF];  // ������Ϣ�ļ���
	int  nAMAll;  // AM�豸����
	int  nAMPmu;  // װ��PMU���豸����
	int  nAMOld;  // AM�豸���м�¼����
	INT_REC MAX_AMOld_idx;  // AM�豸���м�¼�������idx
	int  nAMNew;  // �����豸����
}WT_PARA_INFO;
// �˵���Ϣ
typedef struct  type_WT_DOT_INFO
{
	INT_REC dot_id; // �豸һ��ID��
	INT_REC fac_id; // ��վID��
	INT_REC area_id; // ����ID��
	VOL_INT_REC vlty_id; // ��ѹ����ID��
	float vbase; // ��ѹ�ȼ�
	float vtap;  // �ֽ�ͷ��ѹ
	INT_REC tapty_id; /* �ֽ�ͷ����ID�� */
	float tap_pos; // �ֽ�ͷλ��

	char dot_name[64]; // �豸����
	char fac_name[64]; // ��վ����
	char area_name[64]; // ��������

	int  IsYcDefine;   // ����Ƿ����
	int  nYcFlag[VEC_SSO_NUM];
	int  nYcFilNo[VEC_SSO_NUM];
	int  nYcColNo[VEC_SSO_NUM];
}WT_DOT_INFO;

// �豸��Ϣ
typedef struct  type_WT_DEV_INFO
{
	INT_REC dev_id;  // �豸ID��
	INT_REC fac_id;  // ��վID��
	INT_REC area_id;  // ����ID��
	char dev_name[64];  // �豸����
	char fac_name[64]; // ��վ����
	char area_name[64]; // ��������
	char bpa_name[64];  // BPA����
	int dev_type;  // �豸����
	VOL_INT_REC maxvlty_id;  // ��ߵ�ѹ�ȼ�ID
	float maxvbase;  // ��ߵ�ѹ�ȼ�
	int sim_curve_idx;  // ��������˳���

	// ��ѹ��
	int trty;	// ��������
	int tr_type;	// ��ѹ������
	float temperature;  // ��ǰ�¶�
	// ĸ��Ƶ�ʲ��
	INT_REC freq_yc_id;  // ���ID��(���ң����Ϣ��)
	INT_REC volt_per_id;  // ��ѹ����ƫ����
	INT_REC freq_per_id;  // Ƶ������ƫ����
	
	int is_old;  // �Ѵ���
	int vflag;   // ��ѹ�ȼ���־
	int yc_flag;  // ����־;  0,���˾���Ч;1,һ����Ч;2,������Ч;
	                        // 3,һ���˾���Ч;
	                        // 4,������Ч;  5,һ������Ч; 6,һ�����˾���Ч
	int is_add;  // �Ƿ������豸
	WT_DOT_INFO dot1; // һ���豸��Ϣ(��վ��ѹ�ȼ����ĸ��,�����,������·�׶�,��ѹ����ѹ��,ĸ��)
	WT_DOT_INFO dot2; // �����豸��Ϣ(������·ĩ��,��ѹ����ѹ��)
	WT_DOT_INFO dot3; // �����豸��Ϣ(��ѹ����ѹ��)
}WT_DEV_INFO;

// SSO��վ��
typedef struct type_SSO_FC_TAB_CRE
{
	INT_REC fac_idx;	/* ��վ���� */
	INT_REC station_id;	/* ��վID�� */
	INT_REC area_id;	/* ����ID�� */
	unsigned char fac_type;	/* ��վ���� */
	VOL_INT_REC vlty_id;	/* ��ѹ�ȼ� */
	unsigned char is_monitor;	/* �Ƿ���� */
	unsigned char is_part_ana;	/* �Ƿ������� */
	unsigned char is_m_bs;	/* ��ĸ���� */
} SSO_FC_TAB_CRE;
#define SSO_FC_TAB_CRE_FLD "fac_idx,station_id,area_id,fac_type,vlty_id,is_monitor,is_part_ana,is_m_bs"
#ifdef _NUSP
#define SSO_FC_TAB_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',"VOL_INT_REC_FMT",%d,%d,%d,%d)"
#define SSO_FC_TAB_CRE_VAL(val) INT_REC_VAL(val.fac_idx),INT_REC_VAL(val.station_id),INT_REC_VAL(val.area_id),val.fac_type,val.vlty_id,val.is_monitor,val.is_part_ana,val.is_m_bs
#else
#define SSO_FC_TAB_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,"INT_REC_FMT",%d,%d,%d)"
#define SSO_FC_TAB_CRE_VAL(val) val.fac_idx,val.station_id,val.area_id,val.fac_type,val.vlty_id,val.is_monitor,val.is_part_ana,val.is_m_bs
#endif // _NUSP


// SSO�������
typedef struct type_SSO_GN_TAB_CRE
{
	INT_REC gn_idx;	/* ��������� */
	INT_REC gn_id;	/* �����ID�� */
	INT_REC station_id;	/* ��վID�� */
	INT_REC area_id;	/* ����ID�� */
	unsigned char is_monitor;	/* �Ƿ���� */
	char gn_name[64];  //	���������
	char fac_name[64];  //	��վ����
	char area_name[64];  //	��������
	unsigned char is_part_ana;	/* �Ƿ������� */
} SSO_GN_TAB_CRE;
#define SSO_GN_TAB_CRE_FLD "gn_idx,gn_id,station_id,area_id,is_monitor,gn_name,fac_name,area_name,is_part_ana"
#ifdef _NUSP
#define SSO_GN_TAB_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',%d,'%s','%s','%s',%d)"
#define SSO_GN_TAB_CRE_VAL(val) INT_REC_VAL(val.gn_idx),INT_REC_VAL(val.gn_id),INT_REC_VAL(val.station_id),INT_REC_VAL(val.area_id),val.is_monitor,val.gn_name,val.fac_name,val.area_name,val.is_part_ana
#endif
#define SSO_GN_TAB_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,'%s','%s','%s',%d)"
#define SSO_GN_TAB_CRE_VAL(val) val.gn_idx,val.gn_id,val.station_id,val.area_id,val.is_monitor,val.gn_name,val.fac_name,val.area_name,val.is_part_ana
#endif // _NUSP


// SSO������·��
typedef struct type_SSO_LN_TAB_CRE
{
	INT_REC acln_idx;	/* �����߶����� */
	INT_REC acln_id;	/* ������·ID�� */
	INT_REC dot_id_1;	/* �����߶��׶� */
	INT_REC fac_id_1;	/* ��վID�� */
	INT_REC area_id_1;	/* ����ID�� */
	INT_REC dot_id_2;	/* �����߶�ĩ�� */
	INT_REC fac_id_2;	/* ��վID�� */
	INT_REC area_id_2;	/* ����ID�� */
	unsigned char is_monitor;	/* �Ƿ���� */
	unsigned char is_end;	/* ĩ������ */
	VOL_INT_REC vlty_id;	/* ��ѹ����ID�� */
	char acln_name[64];    //	��·����
	char fac_name_1[64];   //	�׶˳�վ����
	char fac_name_2[64];   //	ĩ�˳�վ����
	char area_name_1[64];  //	�׶���������
	char area_name_2[64];  //	ĩ����������
	unsigned char is_part_ana;	/* �Ƿ������� */
} SSO_LN_TAB_CRE;
#define SSO_LN_TAB_CRE_FLD "acln_idx,acln_id,dot_id_1,fac_id_1,area_id_1,dot_id_2,fac_id_2,area_id_2,is_monitor,is_end,vlty_id,acln_name,fac_name_1,fac_name_2,area_name_1,area_name_2,is_part_ana"
#ifdef _NUSP
#define SSO_LN_TAB_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',%d,%d,"VOL_INT_REC_FMT",'%s','%s','%s','%s','%s',%d)"
#define SSO_LN_TAB_CRE_VAL(val) INT_REC_VAL(val.acln_idx),INT_REC_VAL(val.acln_id),INT_REC_VAL(val.dot_id_1),INT_REC_VAL(val.fac_id_1),INT_REC_VAL(val.area_id_1),INT_REC_VAL(val.dot_id_2),INT_REC_VAL(val.fac_id_2),INT_REC_VAL(val.area_id_2),val.is_monitor,val.is_end,val.vlty_id,val.acln_name,val.fac_name_1,val.fac_name_2,val.area_name_1,val.area_name_2,val.is_part_ana
#else
#define SSO_LN_TAB_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,%d,"INT_REC_FMT",'%s','%s','%s','%s','%s',%d)"
#define SSO_LN_TAB_CRE_VAL(val) val.acln_idx,val.acln_id,val.dot_id_1,val.fac_id_1,val.area_id_1,val.dot_id_2,val.fac_id_2,val.area_id_2,val.is_monitor,val.is_end,val.vlty_id,val.acln_name,val.fac_name_1,val.fac_name_2,val.area_name_1,val.area_name_2,val.is_part_ana
#endif // _NUSP

// SSO��ѹ����
typedef struct type_SSO_TR_TAB_CRE
{
    INT_REC tr_idx;	/* ��ѹ������ */
    INT_REC tr_id;	/* ��ѹ��ID�� */
    INT_REC station_id;	/* ��վID�� */
    INT_REC area_id; /* ����ID�� */
    unsigned char trty;	/* �������� */  // ������䣬�������
	unsigned char tr_type;	/* ��ѹ������ */  // ���䣬�����䣬���ñ�
	INT_REC high_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC high_vlty_id;	/* ��ѹ���ѹ����ID�� */
	INT_REC mid_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC mid_vlty_id;	/* ��ѹ���ѹ����ID�� */
	INT_REC low_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC low_vlty_id;	/* ��ѹ���ѹ����ID�� */
	INT_REC slct_wind_id;	/* ѡ������ID�� */
    unsigned char is_monitor;	/* �Ƿ���� */
    unsigned char   hwd_yc_ok;                              //  ��ѹ����OK
    unsigned char   mwd_yc_ok;                              //  ��ѹ����OK
    unsigned char   lwd_yc_ok;                              //  ��ѹ����OK
	char tr_name[64];    //	���������
	char fac_name[64];   //	��վ����
	char area_name[64];  //	��������
    unsigned char is_part_ana;	/* �Ƿ������� */
} SSO_TR_TAB_CRE;
#define SSO_TR_TAB_CRE_FLD "tr_idx,tr_id,station_id,area_id,trty,tr_type,high_wind_id,high_vlty_id,mid_wind_id,mid_vlty_id,low_wind_id,low_vlty_id,slct_wind_id,is_monitor,hwd_yc_ok,mwd_yc_ok,lwd_yc_ok,tr_name,fac_name,area_name,is_part_ana"
#ifdef _NUSP
#define SSO_TR_TAB_CRE_FMT "INSERT INTO %s (%s) VALUES ('"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"','"INT_REC_FMT"',%d,%d,'"INT_REC_FMT"',"VOL_INT_REC_FMT",'"INT_REC_FMT"',"VOL_INT_REC_FMT",'"INT_REC_FMT"',"VOL_INT_REC_FMT",'"INT_REC_FMT"',%d,%d,%d,%d,'%s','%s','%s',%d)"
#define SSO_TR_TAB_CRE_VAL(val) INT_REC_VAL(val.tr_idx),INT_REC_VAL(val.tr_id),INT_REC_VAL(val.station_id),INT_REC_VAL(val.area_id),val.trty,val.tr_type,INT_REC_VAL(val.high_wind_id),val.high_vlty_id,INT_REC_VAL(val.mid_wind_id),val.mid_vlty_id,INT_REC_VAL(val.low_wind_id),val.low_vlty_id,INT_REC_VAL(val.slct_wind_id),val.is_monitor,val.hwd_yc_ok,val.mwd_yc_ok,val.lwd_yc_ok,val.tr_name,val.fac_name,val.area_name,val.is_part_ana
#else
#define SSO_TR_TAB_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,%d,"INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%d,%d,%d,%d,'%s','%s','%s',%d)"
#define SSO_TR_TAB_CRE_VAL(val) val.tr_idx,val.tr_id,val.station_id,val.area_id,val.trty,val.tr_type,val.high_wind_id,val.high_vlty_id,val.mid_wind_id,val.mid_vlty_id,val.low_wind_id,val.low_vlty_id,val.slct_wind_id,val.is_monitor,val.hwd_yc_ok,val.mwd_yc_ok,val.lwd_yc_ok,val.tr_name,val.fac_name,val.area_name,val.is_part_ana
#endif // _NUSP

/* VF��̬����ĸ�߱� */
typedef struct type_PQ_DA_BUS_TAB_CRE
{
#ifdef _NUSP
	INT_REC bus_idx;	/* ����ĸ������ */
#endif // _NUSP		
	INT_REC bus_id;	/* ����ĸ��ID�� */
	INT_REC fac_id;	/* ��վID�� */
	INT_REC area_id;	/* ����ID */
	int volt_monitor;	/* ��ѹ����   */
	INT_REC volt_per_id;	/* ��ѹ����ƫ���� */
	int freq_monitor;	/* Ƶ�ʼ��� */
	INT_REC freq_per_id;	/* Ƶ������ƫ���� */
	INT_REC freq_yc_id;	/* Ƶ�ʲ��ID */
	float vbase;	/* ��ѹ��׼ */
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

/* VF��̬���������� */
typedef struct type_PQ_DA_CTL_PARA_TAB_CRE
{
	INT_REC record_id;	/* ��¼ID */
	INT_REC dft_v_para;	/* ȱʡ��ѹ��Ԫ������ */
	INT_REC dft_f_para;	/* ȱʡƵ�ʶ�Ԫ������ */
}PQ_DA_CTL_PARA_TAB_CRE;

#define PQ_DA_CTL_PARA_TAB_CRE_FLD "record_id,dft_v_para,dft_f_para"

//������Ϣ�ṹ��
typedef struct tag_GN_INFO_BASE
{
	INT_REC	dev_idx;  // ���
	INT_REC	dev_id;  //	�豸ID
	INT_REC	fac_id;  //	��վID
	INT_REC	area_id;  // ����ID
	char dev_name[64];  //	�豸����
	char fac_name[64];  //	��վ����
	char area_name[64];  //	��������
	int	dev_type;  //	�豸����
	float	r_value;  //	����ֵ
	float	x_value;  //	�翹ֵ
	int	is_monitor;  //	�Ƿ����
	int	is_calc_ang;  //	�Ƿ����㹦��
	char bpaname[64];  //	BPA����
	int	sim_curve_idx;  //	��������˳���
	int	stat;  //	״̬
	int stat_value;  // ״ֵ̬
	char stat_desc[128];  // ״̬����
	float	angle_value;  //	����ƽ��ֵ
	float	power_value;  //	����ƽ��ֵ
	int	group;  //	����Ⱥ
	int fn[VEC_ALL_NUM];  // ��㶨��:UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR ANG ANC EV EA QWR U1V U1A I1V I1A
	int cn[VEC_ALL_NUM];
}GN_INFO_BASE;

/* WAM_AM�豸�� */
typedef struct typde_WAM_AM_DEV_CRE
{
#ifdef _NUSP
	INT_REC dev_idx;	/* �豸���� */
#endif // _NUSP	
	INT_REC dev_id;	/* �豸ID */
	INT_REC fac_id;	/* ��վID */
	INT_REC area_id;	/* ����ID */
	VOL_INT_REC vlty_id;	/* ��ѹ�ȼ�ID */
	char dev_name[64];	/* �豸���� */
	char fac_name[64];	/* ��վ���� */
	char area_name[64];	/* �������� */
	int dev_type;	/* �豸���� */
	float r_value;	/* ����ֵ */
	float x_value;	/* �翹ֵ */
	int is_monitor;	/* �Ƿ���� */
	int is_calc_ang;	/* �Ƿ����㹦�� */
	char bpaname[64];	/* �豸BPA���� */
	int sim_curve_idx;	/* ��������˳��� */
	int stat;	/* ״̬ */
	int stat_value;	/* ״ֵ̬ */
	char stat_desc[128];	/* ״̬���� */
	float angle_value;	/* ����ƽ��ֵ */
	float power_value;	/* ����ƽ��ֵ */
	int group_id;	/* ����Ⱥ */
	int fn1;	/* �ļ���1 */
	int fn2;	/* �ļ���2 */
	int fn3;	/* �ļ���3 */
	int fn4;	/* �ļ���4 */
	int fn5;	/* �ļ���5 */
	int fn6;	/* �ļ���6 */
	int fn7;	/* �ļ���7 */
	int fn8;	/* �ļ���8 */
	int fn9;	/* �ļ���9 */
	int fn10;	/* �ļ���10 */
	int fn11;	/* �ļ���11 */
	int fn12;	/* �ļ���12 */
	int fn13;	/* �ļ���13 */
	int fn14;	/* �ļ���14 */
	int fn15;	/* �ļ���15 */
	int fn16;	/* �ļ���16 */
	int fn17;	/* �ļ���17 */
	int fn18;	/* �ļ���18 */
	int fn19;	/* �ļ���19 */
	int fn20;	/* �ļ���20 */
	int fn21;	/* �ļ���21 */
	int fn22;	/* �ļ���22 */
	int cn1;	/* �к�1 */
	int cn2;	/* �к�2 */
	int cn3;	/* �к�3 */
	int cn4;	/* �к�4 */
	int cn5;	/* �к�5 */
	int cn6;	/* �к�6 */
	int cn7;	/* �к�7 */
	int cn8;	/* �к�8 */
	int cn9;	/* �к�9 */
	int cn10;	/* �к�10 */
	int cn11;	/* �к�11 */
	int cn12;	/* �к�12 */
	int cn13;	/* �к�13 */
	int cn14;	/* �к�14 */
	int cn15;	/* �к�15 */
	int cn16;	/* �к�16 */
	int cn17;	/* �к�17 */
	int cn18;	/* �к�18 */
	int cn19;	/* �к�19 */
	int cn20;	/* �к�20 */
	int cn21;	/* �к�21 */
	int cn22;	/* �к�22 */
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
