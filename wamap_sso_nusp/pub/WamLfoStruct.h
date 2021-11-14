
////�����๫�ýṹ���壺 
//  ���ú궨��
//  ���̼߳�����ƽṹ
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


//  �궨��
/******************************************************************************/
#define MAX_tIMW_WIN        40                    // ���ʱ�䴰��30��
#define DATA_NUM_MAX       (MAX_tIMW_WIN * MAX_FRA_PER_SEC)//  ��������������0.08/40
#define LFO_SMP_FRQ		    12                    //  ����Ƶ��
#define REF_ANG_MAX_PNT	    (2*DATA_NUM_MAX)       //  �ο���Ԥѡ����������0.08
#define MODE_NUM_MAX        PRONY_MAX_MODE
/******************************************************************************/
//  �궨��
#define MAX_TOTAL_TIME      30                     //  ����������ʱ��(s)

// 2007-01-19 save result
#define LfoRec_Time_Max  30       //15M
#define Period_Mode_Max  (LfoRec_Time_Max*6)
/******************************************************************************/
//////////////////////////////////////////////////////////////////////////
// #define VEC_V_NUM                       6                   //  �����ѹ������ (UA UB UC)
// #define VEC_VI_NUM                      12                  //  ���������ѹ������ (IA IB IC UA UB UC)

// #define STA_PMU_GK_OFF                  0X01                //  PMUͨ���˳�  //CStatusOp.h STATE_YC_GK_OFF
// #define STA_PMU_NOT_CHANGE              0X02                //  PMU���ݲ�ˢ��//CStatusOp.h STATE_YC_NOT_CHANGE
// #define STA_PMU_BAD_DATA                0X03                //  PMU���ݲ�����//(STA_PMU_GK_OFF && STA_PMU_NOT_CHANGE)// 070313 �Ƿ����STA_PMU_NOT_CHANGE������STA_PMU_BAD_DATA
// #define SAT_PMU_SIMU_DATA               0XFFFFFFFF          //  ���ֵ  2008-10-11 by zhw

#define  LFO_CURVE_CYC       3         // ʵʱ����ˢ������(s)
#define  LFO_CURVE_TIME_LENG 20        // ʵʱ���ߴ��ڳ���
#define  LFO_CURVE_MAX_PNT   250       // ʵʱ����������
#define  LFO_CURVE_PNT		 125       // TableNet�Ը������ƣ���Ҫ�ֿ�

#define TIME2STR 
//////////////////////////////////////////////////////////////////////////
typedef struct tag_VEC_YC_FCNO
{
    int           viFilNo;                                //  
    int           viColNo;
}VEC_YC_FCNO;                                               //  ң�������ļ����к�

typedef struct tag_VEC_V_FCNO
{
    int           viFilNo[VEC_V_NUM];                     //  (UA UB UC)
    int           viColNo[VEC_V_NUM];
}VEC_V_FCNO;                                                //  �����ѹ�����ļ����к�

typedef struct tag_VEC_VI_FCNO
{
    int           viFilNo[VEC_VI_NUM];                    //  (IA IB IC UA UB UC)
    int           viColNo[VEC_VI_NUM];
}VEC_VI_FCNO;                                               //  ���������ѹ�����ļ����к�

typedef struct tag_VEC_DATA_STS
{
    int             nFraNum;                                //  ��֡��
    int             nBadNum;                                //  ��Ч����
    int             nBadSeq;                                //  ���������Ч����
    time_t          tBadSec;                                //  �����Чʱ��
    int             iBadFra;                                //  �����Ч���
    unsigned char   bPmuOff;                                //  PMUͨ���˳��ź�
    VEC_YC_FCNO     VecBadData[3];                          //  ��Ч֡�ļ���  // 2008-07-16
}VEC_DATA_STS;                                              //  ���ٻ�������̬��������״̬
/////////////////////////////////// ������Ϣ //////////////////////////////
typedef struct  tag_PARA_LFO_ANA
{
	int             mem_max_sec;                            //  �����������
	int             smp_freq;	                            //  ����Ƶ��
	int             flt_len;	                            //  �˲�����
	float           flt_coe;	                            //  �˲�ϵ��
	int             time_leng;	                            //  �۲�ʱ�䴰��
	int             int_time;	                            //  ��������
	float           freq_min;	                            //  Ƶ������
	float           freq_max;	                            //  Ƶ������
	int             sso_jud_num;	                        //  �жϴ���

	int             data_pos;                               //  ��¼�������ļ���ȡָ��


	float			volt_thre1;								//	��ѹ�ȼ�1
	float			volt_thre2;								//	��ѹ�ȼ�2
	float			gn_swn_ampl ;							//	���鿪ʼ�������ֵ   
	float			gn_ewn_ampl ;							//	��������������ֵ   
	float			ln_swn_ampl1;							//	��·��ʼ�������ֵ1  
	float			ln_swn_ampl2;						  //	��·��ʼ�������ֵ2  
	float			ln_swn_ampl3;							//  ��·��ʼ�������ֵ3  
	float			ln_ewn_ampl1;							//  ��·�����������ֵ1  
	float			ln_ewn_ampl2;							//  ��·�����������ֵ2  
	float			ln_ewn_ampl3;							//  ��·�����������ֵ3  
	float			tr_swn_ampl1;							//  ��ѹ����ʼ�������ֵ1
	float			tr_swn_ampl2;							//  ��ѹ����ʼ�������ֵ2
	float			tr_swn_ampl3;							//  ��ѹ����ʼ�������ֵ3
	float			tr_ewn_ampl1;							//  ��ѹ�������������ֵ1
	float			tr_ewn_ampl2;							//  ��ѹ�������������ֵ2
	float			tr_ewn_ampl3;							//  ��ѹ�������������ֵ3

	float			delay_time;								//  �ٻ���¼���ļ���ʱ

}PARA_LFO_ANA;                        

// �������
typedef struct  tag_PARA_DISP_CTL
{
	int             dsource;	                            //  ����Դ
	float           sso_tim_freq;	                        //  ʱ������Ƶ��
	INT_REC         sso_fac_id;	                            //  ��Ӧ���⳧վID
	unsigned char   is_warn;	                            //  �Ƿ��͸澯
	unsigned char   is_samp;	                            //  �Ƿ񴥷�����
	int             show_max;	                            //  ���������ʾ
}PARA_DISP_CTL;

// ������Ϣ
typedef struct tag_PARA_PROC_CTL
{
	int              proc_ctl;                              //  ���з�ʽ����
	int              proc_tim;                              //  ���к�ʱ
	unsigned char    proc_sts;	                            //  ����״̬
	char             proc_ctim[20];	                        //  ���к�ʱchar
	char             proc_ctx[64];	                        //  ������Ϣ
}PARA_PROC_CTL;

// ��ֵʱ��
typedef struct  tag_VALUE_TIME_STRU
{
	float           value;                                  //  ��ֵ
	//struct timeval	time;                                   //  ʱ��
	time_t       	time;                                    //  ʱ��
}VALUE_TIME_STRU;

/////////////////////////////////// ������Ϣ //////////////////////////////
// ���龲̬��Ϣ
typedef struct tag_SSO_GN_INF
{
    INT_REC         gn_idx;    //  ��������
	char            gn_name[64];  //  ��������
    INT_REC         gn_id;     //  ����ID��
    INT_REC         fac_id;     //  ��վID��
    char            fac_name[64];  //  ��վ����
	INT_REC         area_id;	      //  ����ID��
	char            area_name[64];	   //  ��������
	float           snom;           //  ����ʣ�MVA��
    float           pnom;         //  �������MW��
	float           pmin;	         //  �й�����
	float           pmax;	         //  �й�����
	float           qmin;	         //  �޹�����
	float           qmax;	         //  �޹�����
} SSO_GN_INF;


// ���������Ϣ
typedef struct tag_SSO_GN_CTL
{
    INT_REC         gn_id;                                  //  ����ID��
	unsigned char   is_monitor;                             //  �Ƿ����
// 	unsigned char   is_part_ana;                            //  �Ƿ�������
// 	unsigned char   is_agl_calc;                            //  ���㹦������
	float			sso_gn_swn;								//	��Ƶ�񵴿�ʼ�����ֵ
	float			sso_gn_ewn;								//	��Ƶ�񵴽��������ֵ
} SSO_GN_CTL;
#define SSO_GN_CTL_FLD "gn_id,is_monitor,sso_gn_swn,sso_gn_ewn"

// ���������
typedef struct tag_SSO_GN_DAT
{
	INT_REC         gn_id;  //  ����ID��
	float           ampl[DATA_NUM_MAX]; //  ��ֵ
	float           freq[DATA_NUM_MAX];  //  Ƶ��
	float           pwr[DATA_NUM_MAX];  //  �й�
	int				sso[DATA_NUM_MAX]; //  ״̬
	int             SecSts[MAX_tIMW_WIN]; //  ÿ������״̬
    VEC_YC_FCNO     VecBadData[3];                             //  ���ⲻ�����ļ���  // 2008-07-16
	int             stat;                   //  ��������
}SSO_GN_DAT;

// ���������Ϣ
typedef struct tag_SSO_GN_MON
{
	INT_REC         gn_id;  //  ����ID��
	INT_REC         fac_id;    //  ��վID��
	char            gn_name[64];    //  ��������
	int             stat;      //  ״̬
	int             stat_val;   //  ״ֵ̬
	float           lead_ampl; //  ���
	float           lead_freq;  //  Ƶ��
	float           lead_phas;  //  ����
	int				if_sso;
	RTDB_TIME        sso_bgn;	   //  SSO����ʱ��
	RTDB_TIME        sso_end;	   //  SSO��ʧʱ��
	char            sso_time[20];	//  ����ʱ��
	int             stat_old;      //  ����״̬
	float           max_ampl;  //  ����ֵ
	RTDB_TIME         lfo_max_ampl_t;	   //  ����ֵʱ��
	float           aver_freq1;  //  ��Ƶ��1
	int sso_mon;	/* �Ƿ�ɹ� */
} SSO_GN_MON;
#define LFO_GN_MON_FLD "gn_id,stat,stat_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,is_ang_calc"

///////////////////////////////////////////////////////////////////////////
// ������·��̬��Ϣ
typedef struct tag_SSO_LN_INF
{
  INT_REC         acln_idx;    //  ��·����
	char            acln_name[64];    //  ��·����
  INT_REC         acln_id;       //  ��·ID��
	INT_REC         dot_id_1;	    //  �����߶��׶�
	INT_REC         fac_id_1;	    //  ��վID��
	INT_REC         area_id_1;	    //  ����ID��
  char            fac_name_1[64];  //  ��վ����
	char            area_name_1[64];	 //  ��������
	INT_REC         dot_id_2;	    //  �����߶�ĩ��
	INT_REC         fac_id_2;	    //  ��վID��
	INT_REC         area_id_2;	    //  ����ID��
  char            fac_name_2[64];  //  ��վ����
	char            area_name_2[64];	 //  ��������
	float           imax1;	      //  ���������޻��������(�����)
	VOL_INT_REC     vlty_id;	  //  ��ѹ����ID��
	float			vbase;			/* ��ѹ��ֵ */
} SSO_LN_INF;

// ������·������Ϣ
typedef struct tag_SSO_LN_CTL
{
    INT_REC         acln_id;                                //  ��·ID��
	unsigned char   is_monitor;                             //  �Ƿ����
// 	unsigned char   is_part_ana;                            //  �Ƿ�������
	unsigned char   is_end;                                 //  ĩ������
	float			sso_ln_swn;								//	��Ƶ�񵴿�ʼ�����ֵ
	float			sso_ln_ewn;								//	��Ƶ�񵴽��������ֵ
} SSO_LN_CTL;
#define SSO_LN_CTL_FLD "acln_id,is_monitor,is_end,sso_ln_swn,sso_ln_ewn"

// ��·���ݣ��й�+��ѹ������
typedef struct tag_SSO_LN_DAT
{
	INT_REC         ln_id;  //  ��·ID��
	float           ampl[DATA_NUM_MAX]; //  ��ֵ
	float           freq[DATA_NUM_MAX];  //  Ƶ��
	float           pwr[DATA_NUM_MAX];  //  �й�
	int				sso[DATA_NUM_MAX]; //  ״̬
	int             SecSts[MAX_tIMW_WIN]; //  ÿ������״̬
	int             stat;                   //  ��������
    VEC_YC_FCNO     VecBadData[3];                             //  ���ݲ������ļ���  // 2008-07-16 by zhw
}SSO_LN_DAT;

// ������·������Ϣ
typedef struct tag_SSO_LN_MON
{
	INT_REC         ln_id;  //  ��·ID��
	INT_REC         fac_id;    //  ��վID��
	char            ln_name[64];    //  ��·����
	int             stat;      //  ״̬
	int             stat_val;   //  ״ֵ̬
	float           lead_ampl; //  ���
	float           lead_freq;  //  Ƶ��
	float           lead_phas;  //  ����
	RTDB_TIME         sso_bgn;	   //  SSO����ʱ��
	RTDB_TIME         sso_end;	   //  SSO��ʧʱ��
	char            sso_time[20];	//  ����ʱ��
	INT_REC             slct_dot_id;                            //  ѡ���߶�ID  2008-04-05 added by zhw at yn
	int             stat_old;      //  ����״̬
	int				if_sso;
	float           max_ampl;  //  ����ֵ
	RTDB_TIME         lfo_max_ampl_t;	   //  ����ֵʱ��
	float           aver_freq1;  //  ��Ƶ��1
	int sso_mon;	/* �Ƿ�ɹ� */
} SSO_LN_MON;
#define TAB_LFO_AAST_LN_MON_FLD "acln_id,stat,stat_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,slct_dot_id"
///////////////////////////////////////////////////////////////////////////
// ��ѹ����̬��Ϣ
typedef struct tag_SSO_TR_INF
{
	INT_REC         tr_idx;  //  ��ѹ������
	char            tr_name[64];   //  ��ѹ������
	INT_REC         tr_id;       //  ��ѹ��ID��
	INT_REC         fac_id;    //  ��վID��
	char            fac_name[64]; //  ��վ����
	INT_REC         area_id;    //  ����ID��
	char            area_name[64]; //  ��������
	unsigned char   trty;         //  ��������
	unsigned char   tr_type;      //  ��ѹ������
	INT_REC high_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC high_vlty_id;   // ��ѹ���ѹ����ID��
	INT_REC mid_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC mid_vlty_id;   // ��ѹ���ѹ����ID��
	INT_REC low_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC low_vlty_id;   // ��ѹ���ѹ����ID��
	float			vhigh;		/* ��ѹ���ѹ��ֵ */
	float			vmid;		/* ��ѹ���ѹ��ֵ */
	float			vlow;		/* ��ѹ���ѹ��ֵ */
	float			vbase;		/* ��ѹ��ֵ */
	float			max_pwr;		// �й�����
} SSO_TR_INF;


// ��ѹ��������Ϣ
typedef struct tag_SSO_TR_CTL
{
    INT_REC         tr_id;                                  //  ��ѹ��ID��
    unsigned char   is_monitor;                             //  �Ƿ����
//     unsigned char   is_part_ana;                            //  �Ƿ�������
//     unsigned char   mon_wind_slct;                          //  ��������
//     unsigned char   is_pow_calc;                            //  �����Ƿ����ֵ
	float			sso_tr_swn;								//	��Ƶ�񵴿�ʼ�����ֵ
	float			sso_tr_ewn;								//	��Ƶ�񵴽��������ֵ
} SSO_TR_CTL;
#define SSO_TR_CTL_FLD "tr_id,is_monitor,sso_tr_swn,sso_tr_ewn"

// ��ѹ�����ݣ��й�+��ѹ������
typedef struct tag_SSO_TR_DAT
{
	INT_REC         tr_id;  //  ����ID��
	float           ampl[DATA_NUM_MAX]; //  ��ֵ
	float           freq[DATA_NUM_MAX];  //  Ƶ��
	float           pwr[DATA_NUM_MAX];  //  �й�
	int				sso[DATA_NUM_MAX]; //  ״̬
	int             SecSts[MAX_tIMW_WIN]; //  ÿ������״̬
	int             stat;                   //  ��������
    VEC_YC_FCNO     VecBadData[3];                          //  ���ݲ������ļ���  // 2008-07-16 by zhw
}SSO_TR_DAT;

// ��ѹ��������Ϣ
typedef struct tag_SSO_TR_MON
{
    INT_REC         tr_id;  //  ��·ID��
	INT_REC         fac_id;    //  ��վID��
	char            tr_name[64];    //  ��ѹ������
	int             stat;      //  ״̬
	int             stat_val;   //  ״ֵ̬
	float           lead_ampl; //  ���
	float           lead_freq;  //  Ƶ��
	float           lead_phas;  //  ����
	RTDB_TIME         sso_bgn;	   //  SSO����ʱ��
	RTDB_TIME         sso_end;	   //  SSO��ʧʱ��
	char            sso_time[20];	//  ����ʱ��
    INT_REC			high_wind_id;							/* ��ѹ������ID�� */
    INT_REC			mid_wind_id;							/* ��ѹ������ID�� */
    INT_REC			low_wind_id;							/* ��ѹ������ID�� */
    INT_REC         slct_wind_id;                           //  ѡ��ѡ������ID��
	int             stat_old;      //  ����״̬
	int				if_sso;
	float           max_ampl;  //  ����ֵ
	RTDB_TIME         lfo_max_ampl_t;	   //  ����ֵʱ��
	float           aver_freq1;  //  ��Ƶ��1
	int sso_mon;	/* �Ƿ�ɹ� */
} SSO_TR_MON;

///////////////////////////////////////////////////////////////////////////
// ��վ��̬��Ϣ
typedef struct tag_SSO_FC_INF
{
	INT_REC         fac_idx;  //  ��վ����
	INT_REC         fac_id;   //  ��վID
	char            fac_name[64];   //  ��վ����
	INT_REC         area_id;	      //  ����ID��
	char            area_name[64];	   //  ��������
	VOL_INT_REC         vlty_id;      //  ��ߵ�ѹ�ȼ�

} SSO_FC_INF;


// ��վ������Ϣ
typedef struct tag_SSO_FC_CTL
{
	INT_REC         station_id;                                 //  ��վID
	unsigned char   is_monitor;                             //  �Ƿ����
	unsigned char   is_part_ana;                            //  �Ƿ�������
// 	unsigned char   is_set_bus;                             //  �Ƿ�ָ��ĸ��
//     INT_REC         set_bus_id;                             //  ָ��ĸ��ID
// 	unsigned char   is_m_bs;                                //  ��ĸ����
// 	unsigned char   is_set_fyc;                             //  �Ƿ�ָ��Ƶ�ʲ��
//  int             set_fyc_id;                             //  ָ��Ƶ�ʲ��ID
} SSO_FC_CTL;
#define SSO_FC_CTL_FLD "station_id,is_monitor,is_part_ana"

// ��վ��̬����
typedef struct tag_LFO_FC_DAT
{
	INT_REC         fac_id;                                 //  ��վID
	float           u1v[DATA_NUM_MAX];                      //  ��ѹ��ֵ
	float           u1a[DATA_NUM_MAX];                      //  ��ѹ���
	float           fhz[DATA_NUM_MAX];                      //  Ƶ��
	int             SecSts[MAX_tIMW_WIN];                   //  ÿ������״̬
	int             stat;                                   //  ��������״̬(�����ſ���)
    VEC_YC_FCNO     VecBadData[3];                             //  ���ݲ������ļ���  // 2008-07-16 by zhw
	VALUE_TIME_STRU max_val;                                //  ���ֵ
	VALUE_TIME_STRU min_val;                                //  ��Сֵ
	float           ave_val;                                //  ��ֵ
    float           max_ampl;                               //  ���ڶ�
	float           max_ampl_rat;                           //  ��󲨶���(������ֵ/��ֵ)
} LFO_FC_DAT;

// ��վ������Ϣ
typedef struct tag_SSO_FC_MON
{
	INT_REC         fac_id;  //  ��վID��
	int             stat;      //  ״̬
	char            fac_name[64];                           //  ��վ����
	int             stat_val;   //  ״ֵ̬
	RTDB_TIME         sso_bgn;	   //  SSO����ʱ��
	RTDB_TIME         sso_end;	   //  SSO��ʧʱ��
	char            sso_time[20];	//  ����ʱ��
} SSO_FC_MON;
//#define LFO_FC_MON_FLD "stat,v_aver,v_max,v_min,v_ampl,is_cntr,is_disp,clus,slct_bus_id"

// ��վ����Ϣ
// typedef struct tag_LFO_FC_LFO
// {
// 	int             stat;                                       //  ��վID
// 	float           lfo_v_aver;                                 //  ��ѹ��ֵ
// 	float           lfo_v_max;                                  //  ��ѹ���ֵ
// 	float           lfo_v_min;                                  //  ��ѹ��Сֵ
// 	float           lfo_v_ampl;                                 //  ��ѹ�ڶ�
// 	float           lfo_f_aver;                                 //  Ƶ�ʾ�ֵ
// 	float           lfo_f_max;                                  //  Ƶ�����ֵ
// 	float           lfo_f_min;                                  //  Ƶ����Сֵ
// 	float           lfo_f_ampl;                                 //  Ƶ�ʰڶ�
// 	unsigned char   lfo_is_cntr;                                //  �Ƿ�����
// 	unsigned char   lfo_is_disp;                                //  �Ƿ���ʾ
// 	unsigned char   lfo_clus;                                   //  ����Ⱥ
// } LFO_FC_LFO;
// #define LFO_FC_LFO_FLD "stat,lfo_v_aver,lfo_v_max,lfo_v_min,lfo_v_ampl,lfo_f_aver,lfo_f_max,lfo_f_min,lfo_f_ampl,lfo_is_cntr,lfo_is_disp,lfo_clus"
//////////////////////////////////////////////////////////////////////////
// ��վ����Ϣ
typedef struct tag_LFO_FC_ANA
{
	INT_REC         fac_id;                                 //  ��վID
	char            fac_name[64];                           //  ��վ����
	int             stat;                                       //  ��վID
	float           lfo_v_aver;                                 //  ��ѹ��ֵ
	float           lfo_v_max;                                  //  ��ѹ���ֵ
	float           lfo_v_min;                                  //  ��ѹ��Сֵ
	float           lfo_v_ampl;                                 //  ��ѹ�ڶ�
	float           lfo_f_aver;                                 //  Ƶ�ʾ�ֵ
	float           lfo_f_max;                                  //  Ƶ�����ֵ
	float           lfo_f_min;                                  //  Ƶ����Сֵ
	float           lfo_f_ampl;                                 //  Ƶ�ʰڶ�
	unsigned char   lfo_is_cntr;                                //  �Ƿ�����
	unsigned char   lfo_is_disp;                                //  �Ƿ���ʾ
	unsigned char   lfo_clus;                                   //  ����Ⱥ
	float             damp;                                 //�����
	float             freq;                                 //Ƶ��
	float             ampl;                                 //���
	float             phas;                                 //����
	float             mgnt;                                 //����
	float             real;                                 //ʵ��  // 
	float             imag;                                 //�鲿
	float             fact;                                 //��������
	float             engy;                                 //����
} LFO_FC_ANA;
///////////////////////////////////////////////////////////////////////////

// ϵͳ������Ϣ
typedef struct  tag_SSO_SYS_MON
{
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	char            stat_ctx[200];	//  �񵴰�������
	float			mon_ampl;
	float			mon_freq;
	RTDB_TIME         sso_bgn;	   //  SSO����ʱ��
	RTDB_TIME         sso_end;	   //  SSO��ʧʱ��
	char            sso_time[20];	//  ����ʱ��
	INT_REC         warn_dev_id; //  �����澯�豸ID
	char            warn_dev_name[64]; //  �����澯�豸
	float           warn_freq;	       //  �����澯Ƶ��
	int             sso_ln_num;	   //  SSO��·��
	int             sso_gn_num;	   //  SSO�������
	int             sso_tr_num;     //  LFO��ѹ����

	int             if_sso;	   //  �Ƿ������
	RTDB_TIME			start_time;//��¼���ļ��ٻ���ʼʱ��
	RTDB_TIME			end_time;//��¼���ļ��ٻ�����ʱ��
} SSO_SYS_MON;

// ϵͳ����Ϣ
typedef struct  tag_SSO_SYS_SSO
{
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	char            stat_ctx[200];	//  �񵴰�������
	int warn_gn_num;	/* �澯������ */
	int warn_ln_num;	/* �澯��·�� */
	int warn_tr_num;	/* �澯��ѹ���� */
	float lead_ampl;	/* ��� */
	float			lead_freq;     // Ƶ��
	RTDB_TIME         sso_bgn;	   //  SSO����ʱ��
	RTDB_TIME         sso_end;	   //  SSO��ʧʱ��
	char            sso_time[20];	//  ����ʱ��
	INT_REC         warn_dev_id; //  �����澯�豸ID
	char            warn_dev_name[64]; //  �����澯�豸
	float           warn_freq;	       //  �����澯Ƶ��
	int             sso_ln_num;	   //  SSO��·��
	int             sso_gn_num;	   //  SSO�������
	int             sso_tr_num;     //  LFO��ѹ����
	int is_sso;	/* �Ƿ��� */
} SSO_SYS_SSO;
#define SSO_SYS_MON_FLD "stat,stat_val,stat_ctx,warn_gn_num,warn_ln_num,warn_tr_num,lead_ampl,lead_freq,sso_bgn,sso_end,sso_time,warn_dev_id,warn_dev_name,warn_freq,sso_ln_num,sso_gn_num,sso_tr_num,is_sso"

// ϵͳ����Ϣ
typedef struct  tag_LFO_SYS_LFO
{
//	int             if_lfo;	                                //  �Ƿ������  /*2007-11-30 ע��*/
	char            case_des[128];	                        //  �񵴰�������
	RTDB_TIME         lfo_bgn;	                            //  LFO����ʱ��
	RTDB_TIME         lfo_end;	                            //  LFO��ʧʱ��
	char            lfo_time[20];	                        //  ����ʱ��
	INT_REC         warn_dev_id;	                        //  �����澯�豸ID
	char            warn_dev_name[64];	                    //  �����澯�豸
	float           warn_freq;	                            //  �����澯Ƶ��
	float           aver_freq;	                            //  ����ƽ��Ƶ��
	int             lfo_ln_num;	                            //  LFO��·��
	int             lfo_gn_num;	                            //  LFO�������
    int             lfo_tr_num;                             //  LFO��ѹ����
	//int             center_id;	                            //  ������
	//char            center_desc[128];	                    //  ����������
    //int his_bgn;	/* ��ѯ��ʼʱ�� */  // 2008-10-13 by zhw �����о�ʱ��Ϊ���һ�ε�Ƶ��ʱ��
	//int his_end;	/* ��ѯ��ֹʱ�� */
#ifdef _ADDFACID_
	INT_REC warn_dev_facid;  // �����澯�豸STID
#endif
	float aver_freq2;	/* ����ƽ��Ƶ��2 */
} LFO_SYS_LFO;
#ifdef _ADDFACID_
#define LFO_SYS_LFO_FLD "case_des,lfo_bgn,lfo_end,lfo_time,warn_dev_id,warn_dev_name,warn_freq,aver_freq,lfo_ln_num,lfo_gn_num,lfo_tr_num,warn_dev_facid,aver_freq2"
#else
#define LFO_SYS_LFO_FLD "case_des,lfo_bgn,lfo_end,lfo_time,warn_dev_id,warn_dev_name,warn_freq,aver_freq,lfo_ln_num,lfo_gn_num,lfo_tr_num,aver_freq2"
#endif

// �豸������Ϣ
typedef struct tag_SSO_DEV_MON
{
    INT_REC         dev_id;  //  �豸ID��
	INT_REC         fac_id;    //  ��վID��
	int				dev_type; // �豸����
	int             stat;      //  ״̬
	int             stat_val;   //  ״ֵ̬
	float           lead_ampl; //  ���
	float           lead_freq;  //  Ƶ��
	float           lead_phas;  //  ����
	RTDB_TIME         sso_bgn;	   //  SSO����ʱ��
	RTDB_TIME         sso_end;	   //  SSO��ʧʱ��
	char            sso_time[20];	//  ����ʱ��
	char			dev_name[64];
	char			fac_name[64];
	int             stat_old;      //  ����״̬
} SSO_DEV_MON;


// ϵͳ������Ϣ
typedef struct  tag_LFO_SYS_ANA
{
	RTDB_TIME         wds_bgn;	                            //  ��ʼʱ��
	RTDB_TIME         wds_end;	                            //  ����ʱ��
	int             mode_num;	                            //  ģʽ��
	INT_REC         dom_gn_id;	                            //  ��Ҫ����ID
	INT_REC         dom_ln_id;	                            //  ��Ҫ��·ID
	float           lead_ampl;	                            //  ���
	float           lead_freq;	                            //  Ƶ��
	float           lead_damp;	                            //  �����
	float           lead_phas;	                            //  ����
	float           lead_real;	                            //  ʵ��
	float           lead_imag;	                            //  �鲿
	float           lead_engy;	                            //  ����
	float           lead_fail;	                            //  ˥��
	int             show_gn_num;	                        //  ���������
	int             show_ln_num;	                        //  ����·��       
    INT_REC         center_id;	                            //  ������
	char            center_desc[128];	                    //  ����������
    char            ana_detail[255];                        //  ������Ϣ
} LFO_SYS_ANA;
#define LFO_SYS_ANA_FLD "wds_bgn,wds_end,mode_num,dom_gn_id,dom_ln_id,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,show_gn_num,show_ln_num,center_id,center_desc,ana_detail"

///////////////////////////////////////////////////////////////////////////
// LFO����warning sign
typedef struct tag_OSCILLAT_FLAG
{
	INT_REC  obj_id;               //���Ӷ���ID��
    int      lackdamp_warn;        //Ƿ����澯��־��=1 Ƿ���ᣬ=0 ������
    int      times_ldw;            //����Ƿ�����оݴ���
    int      lfo_forewarn;         //LFOԤ����־   ��=1 LFOԤ����=0 ������
    int      times_lfw;            //����LFOԤ���оݴ���
    int      lfo_warn;             //LFO�澯�������־��=1 LFO�澯��=0 ������
    int      times_lw;             //����澯����
	int      damp_norm;            //��������
    int      times_dn;             //���㸺������������
	float    LM_frequency;         //����ģʽƵ��
	float    LM_damp;              //����ģʽ�����
	int      state;                //ǰһ��״̬
//�㶫��Ŀ����
	float    begin_ampl;		   //�豸�񵴿�ʼ�����ֵ
	float    end_ampl;             //�豸�񵴽��������ֵ
}OSCILLAT_FLAG;

// LFO��������ģʽ
typedef struct tag_LFO_LEAD_MODE               
{
	INT_REC  obj_id;               //���Ӷ���ID��(�ٽ�ȺID��Ϊ��)
	float    damping;              //��ģʽ�����
	float    frequency;            //��ģʽƵ��
	float    amplitude;            //��ģʽ���
	float    phase;                //��ģʽ����
	float    aver_ampl;            //����ƽ�����
	float    aver_ampl_rat;        //����ƽ��������ֵ�ı�ֵ
	float    aver_ampl2;           //����ƽ�����2
	float    aver_ampl_rat2;       //����ƽ��������ֵ�ı�ֵ2
	float	 vbase;					//��ѹ��ֵ
}LFO_LEAD_MODE;

// Prony�������	
typedef struct tag_PRONY_ANAL_RSLT
{
 	INT_REC  obj_id;               //���Ӷ���ID��(�ٽ�ȺID��Ϊ��)
	char     obj_name[64];        //���Ӷ�������(200701)
    int      mode_num;             //��ģʽ����
    float    mode[MODE_NUM_MAX][8];//��ģʽ   (����Ԥ���ֵ)
}PRONY_ANAL_RSLT;

// LFO����Ϣͳ��
struct THIS_LFO_INFO
{
	int      start_time;
	int      end_time;
	INT_REC  obj_id;               // �ٽ�ȺΪ -1        
	char     obj_name[64];
	INT_REC  fac_id;
	char     fac_name[64];
	float    frequency;
	float    ampl_max;
	float    ampl_min;
	char     hold_time[20];
};

///////////////////////////////////////////////////////////////////////////
// �ο���ѡȡ����
typedef struct tag_LFO_REF_ANG_DAT
{
    INT_REC         dev_id;                                 //  ��·ID��
 	float           time[REF_ANG_MAX_PNT];                  //  ʱ��
 	float           ang1[REF_ANG_MAX_PNT];                  //  ���1
    float           ang2[REF_ANG_MAX_PNT];                  //  ���2
	int             SecSts[MAX_tIMW_WIN];                   //  ÿ������״̬
	int             stat;                                   //  ��������״̬(�����ſ���)
} LFO_REF_ANG_DAT;

//�ٻ���¼���ļ��ṹ
// typedef struct tag_FAD_CALLUP_COMTR_NAME_STRU 
// {
// 	vector<long>   	fac_seq; //�������ļ���ͨѶ��վid
// 	short fac_num; //�������ļ���ͨѶ��վ��Ŀ������fac_seq.size()
// 	unsigned long  	start_time; //����ʼʱ��
// 	unsigned long  	end_time; //�������ʱ��
// 	string  src_host_name; //����������Ϣ�ĳ������ڻ�������
// 	string  src_proc_name;// ����������Ϣ�ĳ�������
// } FAD_CALLUP_COMTR_NAME_STRU;

// typedef struct tag_FAD_TRANS_REPORT_STRUCT 
// {
// 	long fac_id; //¼���ļ�����ͨѶ��վid
// 	short type; //1-connect fes  2-connect pmu 3-in trans
// 	// 4-suceed  5- failed
// 	string filename; //¼���ļ�����
// 	int total_frame; //¼���ļ��ܰ���
// 	int current_frame; //¼���ļ��ѽ��հ���
// } FAD_TRANS_REPORT_STRUCT;

// ��ģʽ
////  mode participation object
//typedef struct  lfo_MODE_PART_OBJ
//{
//	int               obj_id;                               //�豸ID
//	//	char              obj_name[64];                         //�豸����
//	unsigned char     type;                                 //����(OBJ_TYPE_GN; OBJ_TYPE_LN)
//	float             damp;                                 //�����
//	float             freq;                                 //Ƶ��
//	float             ampl;                                 //���
//	float             phas;                                 //����
//	float             mgnt;                                 //����
//	float             real;                                 //ʵ��  // 
//	float             imag;                                 //�鲿
//	float             fact;                                 //��������
//	float             engy;                                 //����
//}MODE_PART_OBJ;
//
////  mode characteristics
//typedef struct  lfo_MODE_CHRCT
//{
//	int               mode_id;                              //ģʽID
//	unsigned char     type;                                 //����(=0 local; =1 interarea)
//	
//	float             real;                                 //ʵ��(aver)
//	float             imag;                                 //�鲿(aver)
//	float             freq;                                 //Ƶ��(aver)
//	float             damp;                                 //�����(aver)
//
//	float             engyGn;                               //��������(aver) // 2007-08-10 lsf add
//	float             engyLn;                               //��·����(aver) // 2007-08-10 lsf add
//	
//	int               nPartGn;                              //���������
//	MODE_PART_OBJ     PartGn[MODE_MAXGN];                   //����ģ̬
//	int               nPartLn;                              //�ɹ���·��
//	MODE_PART_OBJ     PartLn[MODE_MAXLN];                   //��·ģ̬
//	
//}MODE_CHRCT;
//
// ģ̬
// ��������
typedef struct  type_OBJ_ACT_FACT
{
    INT_REC         obj_idx;                                //  �豸������  2008-04-03 added by zhw at yn
	INT_REC         obj_id;                                 //  �豸ID(�豸ָ��·/�����)
	float           fact;                                   //  ��������
	int             fact_sts;                               //  ��������״̬
	unsigned char   clus;                                   //  ����Ⱥ
    float           aver_fact;                              //  ��������ƽ��ֵ 2008-06-05 by zhw
    int             num_fact;                               //  ����������Ӵ���
}OBJ_ACT_FACT;
#define TYPE_OBJ_ACT_FACT "fact,fact_sts,clus"
#define TYPE_OBJ_AVER_FACT "aver_fact"
///////////////////////////////////////////////////////////////////////////

// �������ƴ�ʩ
	// ����ͼ��ʾ�����ġ����볧վ�������ʣ��������⣩
	// ���ݷ�����������ӣ���������
	// ������������·��  �������з�ʽ
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////// ���м��� //////////////////////////////
///////////////////////////  CALCULATION STRUCT  ////////////////////////
// curve data
typedef struct  tag_CURVE_DATA
{
	float           t_GPS;                                  //  ��վID
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
	int         m_nStatus;    // 0 δ���� 1 ���ڴ����� 2 �������
	int         m_time;       // �����ʱ�䳤��,����
	CURVE_DATA  m_curve;      // ��������
	MODE_RESULT m_result;     // ������
	int         m_nClusterID;
	struct timeb timeStart;  // ��ʼ����ʱ��
	struct timeb timeEnd;    // ��������ʱ��
	
}CASE_TYPE;

///////////////////////////////////////////////////////////////////////////
//����ʱ�ζ�Ӧ����ģʽ
typedef struct  type_period_lead_mode
{
	time_t   bgn_t;           //��ʼʱ��
	time_t   end_t;           //��ֹʱ��
	float    ampl;            //���
	float    freq;            //Ƶ��
	float    damp;            //�����
	float    phase;           //����
	float    real;            //ʵ��
	float    image;           //�鲿
}PERIOD_LEAD_MODE;

// save result
//�񵴹���ϵͳ������
typedef struct  type_lfo_sys_res_save
{
	time_t                  lfo_b_t;                //��ʼʱ��
	time_t                  lfo_e_t;                //����ʱ��
	int                     hold_time;              //����ʱ��
	float                   freq;                   //�����澯��Ƶ��
	float                   aver_freq;              //ƽ��Ƶ��
	INT_REC                 obj_id;                 //����Ԫ��
	char                    obj_name[64];           //�豸����
	int                     obj_type;               //����Ԫ�����ͣ���·�����������Ⱥ��
	INT_REC                 fac_id;                 //��վID
	char                    fac_name[64];           //��վ����
//	ACT_GEN_INFO	        obj_ActGen;             //��ϸ��Ϣ�����������Ⱥ��
    int                     lfo_line_num;	        //����·��
    int                     act_gen_num;	        //���뷢�����
    int                     lfo_tr_num;             //�񵴱�ѹ����
    char                    lfo_case_des[128];	    //�񵴰�������
    float                   max_ampl;               //������ֵ
//	ACT_GEN_INFO	        ActGen;                 //���鼰��������
	int                     mode1_num;
	PERIOD_LEAD_MODE        mode1[Period_Mode_Max]; //����ģʽ1
// 	int                     mode2_num;
// 	PERIOD_LEAD_MODE        mode2[Period_Mode_Max]; //����ģʽ2
// 	int                     mode3_num;
// 	PERIOD_LEAD_MODE        mode3[Period_Mode_Max]; //����ģʽ3
}LFO_SYS_RES_SAVE;

//�񵴹��̱�����Ԫ�����
typedef struct  type_lfo_obj_res_save
{
	INT_REC                 obj_id;                 //�豸ID��(�豸ָ��·/�����)
	char                    obj_name[64];           //�豸����
	INT_REC                 fac_id;                 //��վ����
	char                    fac_name[64];           //��վ����
// 	int                     ref_obj_id;             //�ο��豸ID��(��Է�����������ǣ�)
// 	char                    ref_obj_name[64];       //�ο��豸����
	unsigned char           obj_type;               //�豸����(��·/�����)
	float                   b_aver_pqv0[3];         //��ʼƽ��pqv(ϵͳlfo)
	float                   e_aver_pqv0[3];         //����ƽ��pqv
	int                     stat;                   //�豸״̬
	int                     if_lfo;	                //�Ƿ������
	time_t                  lfo_b_t;                //��ʼʱ��
	time_t                  lfo_e_t;                //����ʱ��
	int                     hold_time;              //����ʱ��
	float                   b_aver_pqv1[3];         //��ʼƽ��pqv(����lfo)
	float                   e_aver_pqv1[3];         //����ƽ��pqv
	VALUE_TIME_STRU         max_value;              //���ֵ
	VALUE_TIME_STRU         min_value;              //��Сֵ
    float                   max_ampl;               //���ڶ�(��Ȳ�)
	int                     mode1_num;
	PERIOD_LEAD_MODE        mode1[Period_Mode_Max]; //����ģʽ1
// 	int                     mode2_num;
// 	PERIOD_LEAD_MODE        mode2[Period_Mode_Max]; //����ģʽ2
// 	int                     mode3_num;
// 	PERIOD_LEAD_MODE        mode3[Period_Mode_Max]; //����ģʽ3
    float                   max_min_dif;            //���ֵ���������£�
}LFO_OBJ_RES_SAVE;

// ���㴰��
typedef struct tag_LFO_CALC_WDS
{
	INT_REC wds_id;
	int wds_no;	                                            //  �������
	RTDB_TIME wds_bgn;                                            //  ��ʼʱ��
	RTDB_TIME wds_end;                                            //  ����ʱ��
	int mode_num;	                                        //  ģʽ��
	float real1;	                                        //  ʵ��1
	float imag1;	                                        //  �鲿1
	float freq1;	                                        //  Ƶ��1
	float damp1;	                                        //  �����1
	float real2;	                                        //  ʵ��2
	float imag2;	                                        //  �鲿2
	float freq2;	                                        //  Ƶ��2
	float damp2;	                                        //  �����2
	float real3;	                                        //  ʵ��3
	float imag3;	                                        //  �鲿3
	float freq3;	                                        //  Ƶ��3
	float damp3;	                                        //  �����3
	int is_mode_show;	                                    //  ģʽ�Ƿ���ʾ
	RTDB_TIME occur_time;	                                        //  ����ʱ��
	char content[255];	                                    //  ��ϸ��Ϣ����
	int is_ctx_show;	                                    //  ��Ϣ�Ƿ���ʾ
    char center_desc[128];                                  //  ����������
}LFO_CALC_WDS;

/* LFOʵʱ���߱� */ // db_struct_wamap.h
typedef struct tab_lfo_rt_curve
{
	INT_REC dev_idx;	/* �豸����ID */
	INT_REC dev_id;	/* ����ID */
	char dev_name[128];	/* �������� */
	char unit[8];	/* ������λ */
	int sam_freq;	/* ����Ƶ�� */
	RTDB_TIME cuv_bgn_sec;	/* ��ʼʱ�� */
	RTDB_TIME cuv_end_sec;	/* ����ʱ�� */
	int cuv_sec;	/* ʱ�� */
	int cuv_pnt;	/* ���� */
	float cuv_stp;	/* ���� */
	float max_val;	/* ���ֵ */
	float min_val;	/* ��Сֵ */
	float ave_val;	/* ƽ��ֵ */
	float seq_cuv_data[250];	/* seq_cuv_data���� */
}TAB_LFO_RT_CURVE;

#define TAB_LFO_RT_CURVE_FLD "dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,max_val,min_val,ave_val,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125," \
"seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"
typedef struct tab_lfo_rt_curve1
{
	//INT_REC dev_id;	/* ����ID */
	char dev_name[128];	/* �������� */
	char unit[8];	/* ������λ */
	int sam_freq;	/* ����Ƶ�� */
	RTDB_TIME cuv_bgn_sec;	/* ��ʼʱ�� */
	RTDB_TIME cuv_end_sec;	/* ����ʱ�� */
	int cuv_sec;	/* ʱ�� */
	int cuv_pnt;	/* ���� */
	float cuv_stp;	/* ���� */
// 	float max_val;	/* ���ֵ */
// 	float min_val;	/* ��Сֵ */
// 	float ave_val;	/* ƽ��ֵ */
	float seq_cuv_data[125];	/* seq_cuv_data���� */
}TAB_LFO_RT_CURVE1;
#define TAB_LFO_RT_CURVE1_FLD "dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20,s21,s22,s23,s24,s25,s26,s27,s28,s29,s30,s31,s32,s33,s34,s35,s36,s37,s38,s39,s40,s41,s42,s43,s44,s45,s46,s47,s48,s49,s50,s51,s52,s53,s54,s55,s56,s57,s58,s59,s60,s61,s62,s63,s64,s65,s66,s67,s68,s69,s70,s71,s72,s73,s74,s75,s76,s77,s78,s79,s80,s81,s82,s83,s84,s85,s86,s87,s88,s89,s90,s91,s92,s93,s94,s95,s96,s97,s98,s99,s100,s101,s102,s103,s104,s105,s106,s107,s108,s109,s110,s111,s112,s113,s114,s115,s116,s117,s118,s119,s120,s121,s122,s123,s124,s125"
typedef struct tab_lfo_rt_curve2
{
	float seq_cuv_data[125];	/* seq_cuv_data���� */
}TAB_LFO_RT_CURVE2;
#define TAB_LFO_RT_CURVE2_FLD "s126,s127,s128,s129,s130,s131,s132,s133,s134,s135,s136,s137,s138,s139,s140,s141,s142,s143,s144,s145,s146,s147,s148,s149,s150,s151,s152,s153,s154,s155,s156,s157,s158,s159,s160,s161,s162,s163,s164,s165,s166,s167,s168,s169,s170,s171,s172,s173,s174,s175,s176,s177,s178,s179,s180,s181,s182,s183,s184,s185,s186,s187,s188,s189,s190,s191,s192,s193,s194,s195,s196,s197,s198,s199,s200,s201,s202,s203,s204,s205,s206,s207,s208,s209,s210,s211,s212,s213,s214,s215,s216,s217,s218,s219,s220,s221,s222,s223,s224,s225,s226,s227,s228,s229,s230,s231,s232,s233,s234,s235,s236,s237,s238,s239,s240,s241,s242,s243,s244,s245,s246,s247,s248,s249,s250"

typedef struct tab_sso_rt_curve_id
{
	INT_REC dev_idx;	/* �豸����ID */
	INT_REC dev_id;	/* ����ID */
}TAB_SSO_RT_CURVE_ID;
#define TAB_SSO_RT_CURVE_ID_FLD "dev_idx,dev_id"

/* SSOʵʱ���ߺ���� */
#ifdef _NUSP
typedef struct tab_SSO_rt_curve2
{
	INT_REC recordid;	/* ��¼ID */
	RTDB_TIME time_point;	/* ʱ�� */
	float time_view;	/* ��ʾʱ�� */
	int time_sec;	/* ����ʱ�� */
	char time_des[8];	/* ʱ������ */
	char gn_name[64];	/* ��������� */
	float gn_power;	/* ������й����� */
	float gn_ssp_val;	/* �����г����ֵ */
	float gn_ssf_val;	/* �����г��Ƶ�� */
	char ln_name[64];	/* ������·���� */
	float ln_power;	/* ������·�й����� */
	float ln_ssp_val;	/* ������·г����ֵ */
	float ln_ssf_val;	/* ������·г��Ƶ�� */
	char tr_name[64];	/* ��ѹ������ */
	float tr_power;	/* ��ѹ���й����� */
	float tr_ssp_val;	/* ��ѹ��г����ֵ */
	float tr_ssf_val;	/* ��ѹ��г��Ƶ�� */
	float lead_freq;	/* ����ģʽƵ������ */
	float lead_damp;	/* ����ģʽ��������� */
}TAB_SSO_RT_CURVE2;
#define TAB_SSO_RT_CURVE2_FLD "recordid,time_point,time_view,time_sec,time_des,gn_name,gn_power,gn_ssp_val,gn_ssf_val,ln_name,ln_power,ln_ssp_val,ln_ssf_val,tr_name,tr_power,tr_ssp_val,tr_ssf_val,lead_freq,lead_damp"
#endif // _NUSP

/* LFO�豸�����߱� */ //20141008
typedef struct tab_warn_dev_curve
{
	INT_REC dev_idx;	/* �豸����ID */
	INT_REC dev_id;	/* ����ID */
	char dev_name[128];	/* �������� */
	char unit[8];	/* ������λ */
	int sam_freq;	/* ����Ƶ�� */
	RTDB_TIME cuv_bgn_sec;	/* ��ʼʱ�� */
	RTDB_TIME cuv_end_sec;	/* ����ʱ�� */
	int cuv_sec;	/* ʱ�� */
	int cuv_pnt;	/* ���� */
	float cuv_stp;	/* ���� */
//	float max_val;	/* ���ֵ */
//	float min_val;	/* ��Сֵ */
//	float ave_val;	/* ƽ��ֵ */
	float seq_cuv_data[250];	/* seq_cuv_data���� */
}TAB_WARN_DEV_CURVE;
#define TAB_WARN_DEV_CURVE_FLD "dev_id,dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125," \
"seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"
typedef struct tab_warn_dev_curve1
{
	INT_REC dev_id;	/* ����ID */
	char dev_name[128];	/* �������� */
	char unit[8];	/* ������λ */
	int sam_freq;	/* ����Ƶ�� */
	RTDB_TIME cuv_bgn_sec;	/* ��ʼʱ�� */
	RTDB_TIME cuv_end_sec;	/* ����ʱ�� */
	int cuv_sec;	/* ʱ�� */
	int cuv_pnt;	/* ���� */
	float cuv_stp;	/* ���� */
// 	float max_val;	/* ���ֵ */
// 	float min_val;	/* ��Сֵ */
// 	float ave_val;	/* ƽ��ֵ */
	float seq_cuv_data[125];	/* seq_cuv_data���� */
}TAB_WARN_DEV_CURVE1;
#define TAB_WARN_DEV_CURVE1_FLD "dev_id,dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125"
typedef struct tab_warn_dev_curve2
{
	float seq_cuv_data[125];	/* seq_cuv_data���� */
}TAB_WARN_DEV_CURVE2;
#define TAB_WARN_DEV_CURVE2_FLD "seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"


/* LFO���涨��� */
// typedef struct tab_LFO_INTERFACE
// {
// 	int   itfc_id;	/* ����ID�� */
// 	char  itfc_name[64];	/* �������� */
// 	
// 	unsigned char is_monitor;	/* �Ƿ���� */
// 	int sam_freq;	/* ����Ƶ�� */
// 	int cuv_bgn_sec;	/* ��ʼʱ�� */
// 	int cuv_end_sec;	/* ����ʱ�� */
// 	int cuv_sec;	/* ʱ�� */
// 	int cuv_pnt;	/* ���� */
// 	float cuv_stp;	/* ���� */
// 	float aver_val;	/* ��ֵ */
// 	float max_val;	/* ���ֵ */
// 	int max_val_t;	/* ���ֵʱ�� */
// 	float min_val;	/* ��Сֵ */
// 	int min_val_t;	/* ��Сֵʱ�� */
// 	float max_min_dif;	/* ���ڶ� */
// 	float seq_cuv_data[250];	/* seq_cuv_data���� */
// }LFO_INTERFACE;
// #define TAB_LFO_INTERFACE_DEF_FLD "sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125," \
// "seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"

/* LFO������ɱ� */
// typedef struct tab_LFO_INTERFACE_DEV
// {
// 	int dev_idx;	/* �豸���� */
// 	int itfc_id;	/* ����ID�� */
// 	int dev_id;	/* �豸ID */  //(���ý�����·��)
// 	int dev_type;	/* �豸���� */
// 	int is_valid;	/* �Ƿ���Ч */
// 	int is_invert;	/* �Ƿ���ȡ�� */
// 	int stat;	/* ״̬ */
// 	int stat_val;	/* ״ֵ̬ */
// }LFO_INTERFACE_DEV;

typedef struct type_SSO_FC_TAB_CRE
{
    INT_REC         fac_idx;                                //  ��վ����
	INT_REC         station_id;                                 //  ��վID
	INT_REC         area_id;                                //  ����ID��
	unsigned char   fac_type;                               //  ��վ����
	VOL_INT_REC     vlty_id;                                //  ��ߵ�ѹ�ȼ�
	
} SSO_FC_TAB_CRE;
#define SSO_FC_TAB_CRE_FLD "fac_idx,station_id,area_id,fac_type,vlty_id"
// SSO�������
typedef struct type_SSO_GN_TAB_CRE
{
    INT_REC             gn_idx;                                 //  ��������
    INT_REC             gn_id;                                  //  ����ID��
    INT_REC             station_id;                                 //  ��վID��
	INT_REC             area_id;	                            //  ����ID��
	unsigned char       is_monitor;	                        //  ���㹦������
} SSO_GN_TAB_CRE;
#define SSO_GN_TAB_CRE_FLD "gn_idx,gn_id,station_id,area_id,is_monitor"
// SSO������·��
typedef struct type_SSO_LN_TAB_CRE
{
    INT_REC             acln_idx;                               //  ��·����
    INT_REC             acln_id;                                //  ��·ID��
	INT_REC             dot_id_1;	                            //  �����߶��׶�
	INT_REC             fac_id_1;	                            //  ��վID��
	INT_REC             area_id_1;	                            //  ����ID��
	INT_REC             dot_id_2;	                            //  �����߶�ĩ��
	INT_REC             fac_id_2;	                            //  ��վID��
	INT_REC             area_id_2;	                            //  ����ID��
	unsigned char       is_end;		                            //  ĩ������
} SSO_LN_TAB_CRE;
#define SSO_LN_TAB_CRE_FLD "acln_idx,acln_id,dot_id_1,fac_id_1,area_id_1,dot_id_2,fac_id_2,area_id_2,is_end"

// SSO��ѹ����
typedef struct type_SSO_TR_TAB_CRE
{
    INT_REC tr_idx;  // ��ѹ������
    INT_REC tr_id;	/* ��ѹ��ID�� */
    INT_REC station_id;	/* ��վID�� */
    INT_REC area_id; // ����ID��
	
    unsigned char trty;	/* �������� */ // ������䡢�������
	unsigned char tr_type;	/* ��ѹ������ */ // ���䡢�����䡢���ñ�
	INT_REC high_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC high_vlty_id;   // ��ѹ���ѹ����ID��	
	INT_REC mid_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC mid_vlty_id;   // ��ѹ���ѹ����ID��	
	INT_REC low_wind_id;	/* ��ѹ������ID�� */
	VOL_INT_REC low_vlty_id;   // ��ѹ���ѹ����ID��
	INT_REC slct_wind_id;	/* ѡ������ID�� */
//    unsigned char is_pow_calc;	/* �����Ƿ����ֵ */
    unsigned char is_monitor;	/* �Ƿ���� */
//    unsigned char is_part_ana;	/* �Ƿ������� */
//	unsigned char mon_wind_slct;	/* �������� */
	
} SSO_TR_TAB_CRE;
#define SSO_TR_TAB_CRE_FLD "tr_idx,tr_id,station_id,area_id,trty,tr_type,high_wind_id,high_vlty_id,mid_wind_id,mid_vlty_id,low_wind_id,low_vlty_id,slct_wind_id,is_monitor"

// LFO���¼���
typedef struct type_LFO_LPO_EVE_CRE
{
	INT_REC record_id;	/* ��¼ID */
	INT_REC eve_id;  // �¼�ID����ʼ��ʱ�̣�
	INT_REC win_id;  // ���ںţ���ǰʱ�̣�
	RTDB_TIME analy_bgn;  // ������ʼʱ��
	RTDB_TIME analy_end;  // ������ֹʱ��
	float freq_max;  // Ƶ������
	float freq_min;  // Ƶ������
	int stat;  // ϵͳ״̬
	int show_gn_num;  // �񵴷��������
	int show_tr_num;  // �񵴱�ѹ������
	int show_ln_num;  // ����·����
	float freq;  // ����ģʽƵ��
	float ampl;  // ���
	float damp;  // �����
	int   dev_num;  // �����豸����
	unsigned char   is_show;  // �Ƿ���ʾ
	char case_des[128];  // ����������
	RTDB_TIME ana_his_bgn;  // ���߷�����ʼʱ��
	RTDB_TIME ana_his_end;  // ���߷�����ֹʱ��
	INT_REC warn_dev_id;	// �����澯�豸ID
	char warn_dev_name[64];  // �����澯�豸��
	char lfo_time[64];  // ����ʱ��
}LFO_LPO_EVE_CRE;
#define LFO_LPO_EVE_CRE_FLD "eve_id,win_id,analy_bgn,analy_end,freq_max,freq_min,stat,show_gn_num,show_tr_num,show_ln_num,freq,ampl,damp,dev_num,is_show,case_des,ana_his_bgn,ana_his_end,warn_dev_id,warn_dev_name,lfo_time"
#define LFO_LPO_EVE_CRE_FMT "INSERT %s (%s) VALUES ("INT_REC_FMT",to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),%f,%f,%d,%d,%d,%d,%f,%f,%f,%d,%d,'%s',"INT_REC_FMT",'%s','%s')"
//#define LFO_LPO_EVE_CRE_VAL(val) val.eve_id,val.win_id,val.analy_bgn,val.analy_end,val.ana_his_bgn,val.ana_his_end,val.freq_max,val.freq_min,val.stat,val.show_gn_num,val.show_tr_num,val.show_ln_num,val.freq,val.ampl,val.damp,val.dev_num,val.is_show,val.case_des,val.warn_dev_id,val.warn_dev_name,val.lfo_time
#define LFO_LPO_EVE_CRE_VAL(val) val.freq_max,val.freq_min,val.stat,val.show_gn_num,val.show_tr_num,val.show_ln_num,val.freq,val.ampl,val.damp,val.dev_num,val.is_show,val.case_des,val.warn_dev_id,val.warn_dev_name,val.lfo_time

/* LFOʵʱCASE���ڱ� */
typedef struct type_LFO_CASE_WDS_CRE
{
	INT_REC wds_id;	/* ����ID */
	INT_REC eve_id;	/* �¼�ID */
	INT_REC win_id;	/* ���ں� */
	RTDB_TIME analy_bgn;	/* ������ʼʱ�� */
	RTDB_TIME analy_end;	/* ������ֹʱ�� */
	float rphas;  /* ���� */
	INT_REC lfodev_id;	/* ���豸ID */
	INT_REC fac_id;	/* ��վID */
	int dev_type;	/* �豸���� */
	float freq;	/* Ƶ�� */
	float ampl;	/* ��� */
	float damp;	/* ����� */
	float aver_val;	/* ƽ��ֵ */
	float max_ampl_rat;	/* ���ʲ���ֵ */
	char lfo_time[64];  /* ����ʱ�� */
	float lfo_max_val;  /* LFO���ֵ */
	RTDB_TIME lfo_max_val_t;  /* LFO���ֵʱ�� */
	float lfo_min_val;  /* LFO��Сֵ */
	RTDB_TIME lfo_min_val_t;  /* LFO��Сֵʱ�� */
}LFO_CASE_WDS_CRE;

#define LFO_CASE_WDS_CRE_FLD "eve_id,win_id,analy_bgn,analy_end,rphas,lfodev_id,fac_id,dev_type,freq,ampl,damp,aver_val,max_ampl_rat,lfo_time,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t"
#define LFO_CASE_WDS_CRE_FMT "INSERT %s (%s) VALUES (to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),to_date('%s','YYYY-MM-DD HH24:MI:SS'),%f,"INT_REC_FMT","INT_REC_FMT",%d,%f,%f,%f,%f,%f,'%s',%f,to_date('%s','YYYY-MM-DD HH24:MI:SS'),%f,to_date('%s','YYYY-MM-DD HH24:MI:SS'))"
//#define LFO_CASE_WDS_CRE_VAL(val) val.eve_id,val.win_id,val.analy_bgn,val.analy_end,val.rphas,val.lfodev_id,val.fac_id,val.dev_type,val.freq,val.ampl,val.damp,val.aver_val,val.max_ampl,val.lfo_time,val.lfo_max_val,val.lfo_max_val_t,val.lfo_min_val,val.lfo_min_val_t
#define LFO_CASE_WDS_CRE_VAL(val) val.rphas,val.lfodev_id,val.fac_id,val.dev_type,val.freq,val.ampl,val.damp,val.aver_val,val.max_ampl,val.lfo_time,val.lfo_max_val


/* LFOʵʱCASEģ̬ȫ�� */
typedef struct type_LFO_CASE_ALLSHAPE_CRE
{
	INT_REC record_id;	/* ��¼�� */
	float fact;	/* �������� */
	INT_REC dev_id;	/* �豸ID */
	char dev_name[64];	/* �豸���� */
	int dev_type;	/* �豸���� */
	INT_REC fac_id;	/* ������վ */
	INT_REC area_id;	/* �������� */
	unsigned char clus;	/* ����Ⱥ */
	float freq;	/* Ƶ�� */
	float damp;	/* ����� */
	float ampl;	/* ��� */
	float engy;	/* ���� */
	float aver_val;	/* ƽ��ֵ */
	float max_ampl_rat;	/* ��󲨶��� */
	float freq_max;	/* Ƶ������ */
	float freq_min;	/* Ƶ������ */
	INT_REC eve_id;	/* �¼�ID */
	INT_REC win_id;	/* ���ں� */
	INT_REC analy_bgn;	/* ������ʼʱ�� */
	INT_REC analy_end;	/* ������ֹʱ�� */
	float iniphas;	/* ���� */
}LFO_CASE_ALLSHAPE_CRE;

#define LFO_CASE_ALLSHAPE_CRE_FLD "fact,dev_id,dev_name,dev_type,fac_id,area_id,clus,freq,damp,ampl,engy,aver_val,max_ampl_rat,freq_max,freq_min,eve_id,win_id,analy_bgn,analy_end,iniphas"
#define LFO_CASE_ALLSHAPE_CRE_FMT "INSERT %s (%s) VALUES (%f,"INT_REC_FMT",'%s',%d,"INT_REC_FMT","INT_REC_FMT",%d,%f,%f,%f,%f,%f,%f,%f,%f,"INT_REC_FMT","INT_REC_FMT","INT_REC_FMT","INT_REC_FMT",%f)"
#define LFO_CASE_ALLSHAPE_CRE_VAL(val) val.fact,val.dev_id,val.dev_name,val.dev_type,val.fac_id,val.area_id,val.clus,val.freq,val.damp,val.ampl,val.engy,val.aver_val,val.max_ampl_rat,val.freq_max,val.freq_min,val.eve_id,val.win_id,val.analy_bgn,val.analy_end,val.iniphas


typedef struct type_LFO_CASE_FILE_DEV
{
	INT_REC fac_id;  // ��վID
	INT_REC dev_id;  // �豸ID(�˵�)
	INT_REC dev_id1;  // �豸ID
	int column_id;  // ��ID
#ifndef _D5000
	KEY_ID_STRU yc_id;
#else
	long yc_id;  // ң��ID
#endif
	int file_no;  // �ļ���
	int col_no;  // �к�
	int is_warn;  // �Ƿ����豸
}LFO_CASE_FILE_DEV;

typedef struct type_LFO_CASE_FILE_STRUCT
{
	char version[10];  // �汾��
	char time[20];  // ��־�ļ�����ʱ��
	char system[64];  // ϵͳ˵��
	RTDB_TIME eve_id;  // �¼�ID
	RTDB_TIME start_time;  // ����ʼʱ��
	RTDB_TIME end_time;  // �񵴽�ֹʱ��
	RTDB_TIME hold_time;  // ����ʱ��
	RTDB_TIME Data_Save_start_time; //���ݱ�����ʼʱ��
	RTDB_TIME Data_Save_end_time; //���ݱ�����ʼʱ��
	char case_des[128];  // ���¼�����
	INT_REC warn_dev_id;  // �����澯�豸ID
	char warn_dev_name[64];  // �����澯�豸��
	float warn_freq;  // �����澯Ƶ��
	float max_ampl;  // ������ֵ
	vector<LFO_CASE_FILE_DEV> vecdev;
}LFO_CASE_FILE_STRUCT;

//////////////////////////////////////////////////////////////////////////
#ifndef LOW_FRE_OSCI_INFO_TO_IA
#define LOW_FRE_OSCI_INFO_TO_IA 504
#endif

// ���豸��Ϣ
typedef struct type_OSCI_DEV_INFO_STRU
{
	INT_REC osci_id;  // ������*1000
	INT_REC key_id;  // �豸ID
	int group_num;  // ������Ⱥ1/2
    char   group_name[64];          //������Ⱥ
	char descr[64];  // �豸����
	float damp_rate;  // �����
	float oscillation_fre;  // ��Ƶ��
	float oscillation_value;  // �񵴷�ֵ
	float phase_angle;  // ��λ
}OSCI_DEV_INFO_STRU;

typedef struct type_LOW_FRE_OSCI_INFO_TO_IA_STRU
{
	INT_REC search_id;  // ������(�豸ID)
	time_t occur_time;  // ����ʱ��
	short msec;  // ����
	long end_time;  // ����ʱ��
	INT_REC dev_id;  // �����豸(ͨ����id,����WAMS����)
	char dev_descr[64];  // �����豸����
	INT_REC st_id;  // �����豸������վid
	int app_id;  // ���ͽ�������Ӧ��id
	short criterion_num;  // ���豸��Ϣ����OSCI_DEV_INFO�ĸ���
	INT_REC osci_id;  // ��Դ���״���id��������0��1����������Դ�Ƿ��Դ��ڣ�
	char status;  // ��״̬����ʼ��������������
	float damp_rate;  // �����
	float oscillation_fre;  // ��Ƶ��
	float oscillation_value;  // �񵴷�ֵ
	float phase_angle;  // ��λ
	short continus_flag;  // ������־��0�״Σ�1������
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
