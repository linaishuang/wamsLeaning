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
///////////////////////////////////������ṹ����////////////////////////
typedef struct  tab_PARA_PROC_CTL
{
	int mon_ctl;            /* �������п��� */
	unsigned char mon_sts;	/* ��������״̬ */
	char mon_use_time[20];	/* ���Ӽ����ʱ */
	char mon_run_ctx[64];	/* ����������Ϣ */

}TAB_PARA_PROC_CTL;

#define TAB_PARA_PROC_CTL_FLD  "mon_ctl,mon_sts,mon_use_time,mon_run_ctx"

typedef struct  tab_PARA_SSO_ANA
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
#ifdef _D5000
	float			volt_thre1;								/* ��ѹ�ȼ�1 */
	float			volt_thre2;								/* ��ѹ�ȼ�2 */
//�㶫��Ŀ����
	float			gn_swn_ampl;							//	���鿪ʼ�������ֵ
	float			gn_ewn_ampl;							//	��������������ֵ
	float			ln_swn_ampl1;						    //	��·��ʼ�������ֵ1  
	float			ln_swn_ampl2;							//  ��·��ʼ�������ֵ2  
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
#endif
}TAB_PARA_SSO_ANA;
#ifdef _D5000
#define TAB_PARA_SSO_ANA_FLD "mem_max_sec,smp_freq,flt_len,flt_coe,time_leng,int_time,freq_min,freq_max,sso_jud_num,"\
"volt_thre1,volt_thre2,gn_swn_ampl,gn_ewn_ampl,ln_swn_ampl1,ln_swn_ampl2,ln_swn_ampl3,ln_ewn_ampl1,ln_ewn_ampl2,ln_ewn_ampl3,tr_swn_ampl1,tr_swn_ampl2,tr_swn_ampl3,tr_ewn_ampl1,tr_ewn_ampl2,tr_ewn_ampl3"
#else
#define TAB_PARA_SSO_ANA_FLD "mem_max_sec,smp_freq,flt_len,flt_coe,time_leng,int_time,freq_min,freq_max,sso_jud_num"
#endif

// �������
typedef struct  tab_PARA_DISP_CTL
{
	int             dsource;	                            //  ����Դ
	float           sso_tim_freq;	                        //  ʱ������Ƶ��
// 	int             show_max;	                            //  ���������ʾ
	INT_REC         sso_fac_id;	                            //  ��Ӧ���⳧վID
	unsigned char   is_warn;	                            //  �Ƿ��͸澯
	unsigned char   is_samp;	                            //  �Ƿ񴥷�����
}TAB_PARA_DISP_CTL;
#define TAB_PARA_DISP_CTL_FLD "dsource,sso_tim_freq,sso_fac_id,is_warn,is_samp"

///////////////////////////////////�����ṹ����////////////////////////
// ������·����Ϣ
typedef struct  tag_LFO_LN_LFO
{
	int             if_lfo;	                                //  �Ƿ������
	float           lfo_bgn_val;	                        //  LFOǰ��ֵ
	float           lfo_end_val;	                        //  LFO���ֵ
	RTDB_TIME       slfo_bgn_t;	                            //  ����LFO����ʱ��
	RTDB_TIME       slfo_end_t;	                            //  ����LFO��ʧʱ��
	char            slfo_time[20];	                        //  ����ʱ��
	float           slfo_bgn_val;	                        //  ����LFOǰ��ֵ
	float           slfo_end_val;	                        //  ����LFO���ֵ
	float           lfo_max_val;	                        //  LFO���ֵ
	RTDB_TIME       lfo_max_val_t;	                        //  LFO���ֵʱ��
	float           lfo_min_val;	                        //  LFO��Сֵ
	RTDB_TIME       lfo_min_val_t;	                        //  LFO��Сֵʱ��
	float           lfo_max_min_dif;	                    //  LFO����ֵ
	float           aver_freq1;	                            //  ��Ƶ��1
	float           aver_freq2;	                            //  ��Ƶ��2
}LFO_LN_LFO;
#define LFO_LN_LFO_FLD "if_lfo,lfo_bgn_val,lfo_end_val,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif,aver_freq1,aver_freq2"

// ��������Ϣ
typedef struct  tag_LFO_GN_LFO
{
	int             if_lfo;	                                //  �Ƿ������
	float           lfo_bgn_val;	                        //  LFOǰ��ֵ
	float           lfo_end_val;	                        //  LFO���ֵ
	RTDB_TIME       slfo_bgn_t;	                            //  ����LFO����ʱ��
	RTDB_TIME       slfo_end_t;	                            //  ����LFO��ʧʱ��
	char            slfo_time[20];	                        //  ����ʱ��
	float           slfo_bgn_val;	                        //  ����LFOǰ��ֵ
	float           slfo_end_val;	                        //  ����LFO���ֵ
	float           lfo_max_val;	                        //  LFO���ֵ
	RTDB_TIME       lfo_max_val_t;	                        //  LFO���ֵʱ��
	float           lfo_min_val;	                        //  LFO��Сֵ
	RTDB_TIME       lfo_min_val_t;	                        //  LFO��Сֵʱ��
	float           lfo_max_min_dif;	                    //  LFO����ֵ
	float           aver_freq1;	                            //  ��Ƶ��1
	float           aver_freq2;	                            //  ��Ƶ��2
}LFO_GN_LFO;
#define LFO_GN_LFO_FLD "if_lfo,lfo_bgn_val,lfo_end_val,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif,aver_freq1,aver_freq2"

// ��ѹ������Ϣ
typedef struct  tag_LFO_TR_LFO
{
    unsigned char   if_lfo;	                                //  �Ƿ������
    float           lfo_bgn_val;	                        //  LFOǰ��ֵ
    float           lfo_end_val;	                        //  LFO���ֵ
	RTDB_TIME       slfo_bgn_t;	                            //  ����LFO����ʱ��
	RTDB_TIME       slfo_end_t;	                            //  ����LFO��ʧʱ��
    char            slfo_time[20];	                        //  ����ʱ��
    float           slfo_bgn_val;	                        //  ����LFOǰ��ֵ
    float           slfo_end_val;	                        //  ����LFO���ֵ
    float           lfo_max_val;	                        //  LFO���ֵ
    RTDB_TIME       lfo_max_val_t;	                        //  LFO���ֵʱ��
    float           lfo_min_val;	                        //  LFO��Сֵ
	RTDB_TIME       lfo_min_val_t;	                        //  LFO��Сֵʱ��
    float           lfo_max_min_dif;	                    //  LFO����ֵ
    float           aver_freq1;	                            //  ��Ƶ��1
    float           aver_freq2;	                            //  ��Ƶ��2
}LFO_TR_LFO;
#define LFO_TR_LFO_FLD "if_lfo,lfo_bgn_val,lfo_end_val,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif,aver_freq1,aver_freq2"


// ��վ������Ϣ
typedef struct tab_LFO_FC_MON
{
// 	int             fac_id;                                 //  ��վID
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	float lead_ampl;	/* ��� */
	int lead_amplclr;	/* �����ɫ */
	float lead_freq;	/* Ƶ�� */
	int lead_freqclr;	/* Ƶ����ɫ */
	int fac_stat;	/* ��վ״̬ */
	int sso_mon;	/* �Ƿ�ɹ� */
}TAB_LFO_FC_MON;
#define TAB_LFO_FC_MON_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,fac_stat,sso_mon"
// ��վ������Ϣ״̬
typedef struct tab_LFO_FC_MON_STAT
{
	// 	int             fac_id;                                 //  ��վID
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	float lead_ampl;	/* ��� */
	int lead_amplclr;	/* �����ɫ */
	float lead_freq;	/* Ƶ�� */
	int lead_freqclr;	/* Ƶ����ɫ */
	int fac_stat;	/* ��վ״̬ */
	int sso_mon;	/* �Ƿ�ɹ� */
}TAB_LFO_FC_MON_STAT;
#define TAB_LFO_FC_MON_STAT_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,fac_stat,sso_mon"


// ���������Ϣ
typedef struct tab_SSO_GN_MON
{
//     int             gn_id;                                  //  ����ID��
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	INT_REC         mode_rcd_id;                            //  ���豸ģʽȫ�����׼�¼ID��
	int             mode_rcd_num;	                        //  ���豸ģʽȫ���еļ�¼����
	float           lead_ampl;                              //  ���
	int lead_amplclr;	/* �����ɫ */
	float           lead_freq;                              //  Ƶ��
	int lead_freqclr;	/* Ƶ����ɫ */
	float           lead_damp;                              //  �����
	float           lead_phas;                              //  ����
	float           lead_real;                              //  ʵ��
	float           lead_imag;                              //  �鲿
	float           lead_engy;                              //  ����
	float           lead_fail;                              //  ˥��
	float           aver_val;                               //  ��ֵ
	int				if_sso;									/* �Ƿ������ */
	RTDB_TIME		sso_bgn_t;								/* SSO����ʱ�� */
	RTDB_TIME		sso_end_t;								/* SSO��ʧʱ�� */
	char			sso_time[20];							/* ����ʱ�� */
    float           max_ampl;                               //  ����ֵ
	RTDB_TIME		lfo_max_ampl_t;							//  ����ֵʱ�� 
	float			aver_freq1;								//	��Ƶ��1 
	int sso_mon;	/* �Ƿ�ɹ� */
}TAB_SSO_GN_MON;
#define TAB_SSO_GN_MON_FLD "stat,stat_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,if_sso,sso_bgn_t,sso_end_t,sso_time,max_ampl,lfo_max_ampl_t,aver_freq1,sso_mon"


// ���������Ϣ״̬
typedef struct tab_SSO_GN_MON_STAT
{
	//     int             gn_id;                                  //  ����ID��
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	float           lead_ampl;                              //  ���
	int lead_amplclr;	/* �����ɫ */
	float           lead_freq;                              //  Ƶ��
	int lead_freqclr;	/* Ƶ����ɫ */
}TAB_SSO_GN_MON_STAT;
#define TAB_SSO_GN_MON_STAT_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr"

// ������·������Ϣ
typedef struct tab_SSO_LN_MON
{
//     int             acln_id;                                //  ��·ID��
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	INT_REC         mode_rcd_id;                            //  ���豸ģʽȫ�����׼�¼ID��
	int             mode_rcd_num;	                        //  ���豸ģʽȫ���еļ�¼����
	float           lead_ampl;                              //  ���
	int lead_amplclr;	/* �����ɫ */
	float           lead_freq;                              //  Ƶ��
	int lead_freqclr;	/* Ƶ����ɫ */
	float           lead_damp;                              //  �����
	float           lead_phas;                              //  ����
	float           lead_real;                              //  ʵ��
	float           lead_imag;                              //  �鲿
	float           lead_engy;                              //  ����
	float           lead_fail;                              //  ˥��
	int				if_sso;									/*  �Ƿ������ */
	RTDB_TIME		sso_bgn_t;								/*  SSO����ʱ�� */
	RTDB_TIME		sso_end_t;								/*  SSO��ʧʱ�� */
	char			sso_time[20];							/*  ����ʱ�� */
    float           max_ampl;                               //  ����ֵ
	RTDB_TIME		lfo_max_ampl_t;							/*  ����ֵʱ�� */
    INT_REC         slct_dot_id;                            //  ѡ���߶�ID  2008-04-05 added by zhw at yn
	float			aver_freq1;								/*  ��Ƶ��1 */
	int sso_mon;	/* �Ƿ�ɹ� */
}TAB_SSO_LN_MON;
#define TAB_SSO_LN_MON_FLD "stat,stat_val,"\
"mode_rcd_id,mode_rcd_num,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,lead_damp,lead_phas,lead_real,"\
"lead_imag,lead_engy,lead_fail,if_sso,sso_bgn_t,sso_end_t,sso_time,"\
"max_ampl,lfo_max_ampl_t,slct_dot_id,aver_freq1,sso_mon"

typedef struct tab_SSO_LN_MON_STAT
{
	//     int             acln_id;                                //  ��·ID��
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	float           lead_ampl;                              //  ���
	int lead_amplclr;	/* �����ɫ */
	float           lead_freq;                              //  Ƶ��
	int lead_freqclr;	/* Ƶ����ɫ */
}TAB_SSO_LN_MON_STAT;
#define TAB_SSO_LN_MON_STAT_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr"

// ������·������Ϣ״̬
typedef struct tab_SSO_LN_STAT
{
	//     int             acln_id;                                //  ��·ID��
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
}TAB_SSO_LN_STAT;
#define TAB_SSO_LN_STAT_FLD "stat,stat_val,"\


/*
#define TAB_LFO_LN_MON_FLD "pwr_val,stat,stat_val,damp_grad,damp_grad_val,\
mode_rcd_id,mode_rcd_num,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,\
lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,\
max_ampl,max_ampl_rat"*/

// ��ѹ��������Ϣ
typedef struct tab_SSO_TR_MON
{
    int             stat;                                   //  ״̬
    int             stat_val;                               //  ״ֵ̬
    INT_REC         mode_rcd_id;                            //  ���豸ģʽȫ�����׼�¼ID��
    int             mode_rcd_num;	                        //  ���豸ģʽȫ���еļ�¼����
    float           lead_ampl;                              //  ���
	int				lead_amplclr;							/* �����ɫ */
	float           lead_freq;                              //  Ƶ��
	int				lead_freqclr;							/* Ƶ����ɫ */
    float           lead_damp;                              //  �����
    float           lead_phas;                              //  ����
    float           lead_real;                              //  ʵ��
    float           lead_imag;                              //  �鲿
    float           lead_engy;                              //  ����
    float           lead_fail;                              //  ˥��
    float           max_ampl;                               //  ����ֵ
	RTDB_TIME		lfo_max_ampl_t;							/*����ֵʱ�� */
	unsigned char	if_sso;									/* �Ƿ������ */
	RTDB_TIME		sso_bgn_t;								/* SSO����ʱ�� */
	RTDB_TIME		sso_end_t;								/* SSO��ʧʱ�� */
	char			sso_time[20];							/* ����ʱ�� */
	float			aver_freq1;								/* ��Ƶ��1 */
	int sso_mon;	/* �Ƿ�ɹ� */
}TAB_SSO_TR_MON;
#define TAB_SSO_TR_MON_FLD "stat,stat_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_amplclr,lead_freq,lead_freqclr,lead_damp,lead_phas,lead_real,"\
    "lead_imag,lead_engy,lead_fail,max_ampl,lfo_max_ampl_t,if_sso,sso_bgn_t,sso_end_t,sso_time,aver_freq1,sso_mon"

typedef struct tab_SSO_TR_MON_STAT
{
    int             stat;                                   //  ״̬
    int             stat_val;                               //  ״ֵ̬
    float           lead_ampl;                              //  ���
	int				lead_amplclr;							/* �����ɫ */
	float           lead_freq;                              //  Ƶ��
	int				lead_freqclr;							/* Ƶ����ɫ */
}TAB_SSO_TR_MON_STAT;
#define TAB_SSO_TR_MON_STAT_FLD "stat,stat_val,lead_ampl,lead_amplclr,lead_freq,lead_freqclr"

/*  LFOʵʱ�豸ģʽȫ��    2007-12-3 grm�޸�  */
typedef struct tab_LFO_DEVICE_MODE
{
	INT_REC record_id;	/* ��¼ID */
	INT_REC dev_id;	/* �豸ID */
	int dev_type;	/* �豸���� */
	unsigned char is_show;	/* �Ƿ���ʾ */
	float ampl;	/* ��� */
	float freq;	/* Ƶ�� */
	float damp;	/* ����� */
	float phas;	/* ���� */
	float real;	/* ʵ�� */
	float imag;	/* �鲿 */
	float engy;	/* ���� */
	float fail;	/* ˥�� */

}TAB_LFO_DEVICE_MODE;
#define TAB_LFO_DEVICE_MODE_FLD "record_id,dev_id,dev_type,ampl,freq,damp,phas,real,imag,engy,fail"

/* LFOʵʱ�豸����ģʽ�� */
typedef struct tab_LFO_DEV_LEAD_MODE
{
	INT_REC record_id;	/* ��¼ID */
	INT_REC wds_no;	/* ������� */
	RTDB_TIME wds_bgn;	/* ��ʼʱ�� */
	RTDB_TIME wds_end;	/* ����ʱ�� */
	INT_REC dev_id;	/* �豸ID */
	int dev_type;	/* �豸���� */
	unsigned char is_show;	/* �Ƿ���ʾ */
	float ampl;	/* ��� */
	float freq;	/* Ƶ�� */
	float damp;	/* ����� */
	float phas;	/* ���� */
	float real;	/* ʵ�� */
	float imag;	/* �鲿 */
	float engy;	/* ���� */
	float fail;	/* ˥�� */
}TAB_LFO_DEV_LEAD_MODE;
#define TAB_LFO_DEV_LEAD_MODE_FLD "record_id,wds_no,wds_bgn,wds_end,dev_id,dev_type,is_show,ampl,freq,damp,phas,real,imag,engy,fail"


/* LFO�豸ģʽ  */
// typedef struct tag_LFO_DEVICE_MODE
// {
// 	INT_REC dev_id;	/* �豸ID */
// 	int dev_type;	/* �豸���� */
// 	unsigned char is_show;	/* �Ƿ���ʾ */
// 	float ampl;	/* ��� */
// 	float freq;	/* Ƶ�� */
// 	float damp;	/* ����� */
// 	float phas;	/* ���� */
// 	float real;	/* ʵ�� */
// 	float imag;	/* �鲿 */
// 	float engy;	/* ���� */
// 	float fail;	/* ˥�� */
// }LFO_DEVICE_MODE;
// #define LFO_DEVICE_MODE_FLD "dev_id,dev_type,ampl,freq,damp,phas,real,imag,engy,fail"

/* LFOʵʱ�豸����ģʽ�� */
typedef struct tag_LFO_DEV_LEAD_MODE
{
	INT_REC wds_no;	/* ������� */
	RTDB_TIME wds_bgn;	/* ��ʼʱ�� */
	RTDB_TIME wds_end;	/* ����ʱ�� */
	INT_REC dev_id;	/* �豸ID */
	int dev_type;	/* �豸���� */
	unsigned char is_show;	/* �Ƿ���ʾ */
	float ampl;	/* ��� */
	float freq;	/* Ƶ�� */
	float damp;	/* ����� */
	float phas;	/* ���� */
	float real;	/* ʵ�� */
	float imag;	/* �鲿 */
	float engy;	/* ���� */
	float fail;	/* ˥�� */
}LFO_DEV_LEAD_MODE;
#define LFO_DEV_LEAD_MODE_FLD "wds_no,wds_bgn,wds_end,dev_id,dev_type,is_show,ampl,freq,damp,phas,real,imag,engy,fail"

/* LFOʵʱģ̬�� */
// typedef struct tag_LFO_WDS_SHAPE
// {
// 	INT_REC wdcase_no;	/* CASE�� */
// 	int app_no;	/* ���ܺ� */
// 	int mode_no;	/* ģʽ�� */
// 	int shape_no;	/* ģ̬��� */
// 	float rreall;	/* ����ֵʵ�� */
// 	float rimag;	/* ����ֵ�鲿 */
// 	float rmgnt;	/* ����ֵ��ֵ */
// 	float rphas;	/* ����ֵ��� */
// 	float fact;	/* �������� */
// 	int fact_sts2;	/* ��������״̬ */
// 	int dev_id;	/* �豸ID */
// 	char dev_name[64];	/* �豸���� */
// 	int dev_type;	/* �豸���� */
// 	int fac_id;	/* ������վ */
// 	int area_id;	/* �������� */
// }LFO_WDS_SHAPE;
// 
// #define LFO_WDS_SHAPE_FLD "record_id,wdcase_no,app_no,mode_no,shape_no,rreall,rimag,rmgnt,rphas,fact,fact_sts2,dev_id,dev_name,dev_type,fac_id,area_id"

/* LFOʵʱģ̬�� */
typedef struct tab_LFO_WDS_SHAPE
{
	INT_REC record_id;	/* ��¼�� */
	int wdcase_no;	/* CASE�� */
	int app_no;	/* ���ܺ� */
	int mode_no;	/* ģʽ�� */
	int shape_no;	/* ģ̬��� */
	float rreall;	/* ����ֵʵ�� */
	float rimag;	/* ����ֵ�鲿 */
	float rmgnt;	/* ����ֵ��ֵ */
	float rphas;	/* ����ֵ��� */
	float fact;	/* �������� */
	int fact_sts2;	/* ��������״̬ */
	INT_REC dev_id;	/* �豸ID */
	char dev_name[64];	/* �豸���� */
	int dev_type;	/* �豸���� */
	INT_REC fac_id;	/* ������վ */
	INT_REC area_id;	/* �������� */
    float freq; /* Ƶ�� */
    float damp; /* ����� */
    float ampl; /* ��� */
    float engy; /* ���� */
    float max_ampl; /* ����ֵ */
    float max_ampl_rat; /* ��󲨶��� */
    unsigned char clus; /* ����Ⱥ */
    float aver_val; /* ƽ��ֵ */
}TAB_LFO_WDS_SHAPE;
#define TAB_LFO_WDS_SHAPE_FLD "record_id,wdcase_no,app_no,mode_no,shape_no,rreall,rimag,rmgnt,rphas,fact,fact_sts2,dev_id,dev_name,dev_type,fac_id,area_id,freq,damp,ampl,engy,max_ampl,max_ampl_rat,clus,aver_val"

/* LFOʵʱCASEģ̬ȫ�� */
typedef struct tab_lfo_case_allshape
{
	INT_REC record_id;	/* ��¼�� */
	int wds_no;	/* ���ں� */
	int app_no;	/* ���ܺ� */
	int mode_no;	/* ģʽ�� */
	int shape_no;	/* ģ̬��� */
	float rreall;	/* ����ֵʵ�� */
	float rimag;	/* ����ֵ�鲿 */
	float rmgnt;	/* ����ֵ��ֵ */
	float rphas;	/* ����ֵ��� */
	float fact;	/* �������� */
	int fact_sts2;	/* ��������״̬ */
	INT_REC dev_id;	/* �豸ID */
	char dev_name[64];	/* �豸���� */
	int dev_type;	/* �豸���� */
	INT_REC fac_id;	/* ������վ */
	INT_REC area_id;	/* �������� */
	unsigned char is_prim;	/* �Ƿ�����ģʽ */
	unsigned char is_curt;	/* �Ƿ�ǰģʽ */
    unsigned char clus;	/* ����Ⱥ */
    float freq;	/* Ƶ�� */
    float damp;	/* ����� */
    float ampl;	/* ��� */
	float engy;	/* ���� */
    float aver_val; /* ƽ��ֵ */
    float max_ampl; /* ����ֵ */
    float max_ampl_rat; /* ��󲨶��� */
}TAB_LFO_CASE_ALLSHAPE;
#define TAB_LFO_CASE_ALLSHAPE_FLD "record_id,wds_no,app_no,mode_no,shape_no,rreall,rimag,rmgnt,rphas,fact,fact_sts2,dev_id,dev_name,dev_type,fac_id,area_id,is_prim,is_curt,clus,freq,damp,ampl,engy,aver_val,max_ampl,max_ampl_rat"

// LFO�������߱�
typedef struct tab_LFO_ASST_DECI
{
    INT_REC             record_id;		                        //  ��¼��
	RTDB_TIME           occur_time;		                        //  ʱ��
    INT_REC             key_dev_id;		                        //  �ؼ��豸
    char            key_dev_name[64];		                //  �ؼ��豸��
    int             key_dev_type;		                    //  �ؼ��豸����
    float           lead_freq;		                        //  ��Ƶ��
    float           lead_damp;		                        //  �����
    float           lead_ampl;		                        //  �񵴷�ֵ
    char            lfo_time[64];		                    //  ����ʱ��
    int             deci_type;		                        //  ���Եȼ�
    char            ctl_act_dsec[128];		                //  ���ƴ�ʩ����
    INT_REC             dev_id;		                            //  �豸��
    char            dev_name[64];		                    //  �豸��
    int             dev_type;		                        //  �豸����
    float           dev_value;		                        //  ��ǰ��ֵ
    int             dev_act_type;		                    //  ��������
    float           dev_change;		                        //  ������
    int             is_show;		                        //  �Ƿ���ʾ
    int             is_flow_pstv;		                    //  �Ƿ���Ϊ�� // 2008-04-04 
    float           key_dev_aver;          	                //  �й���ֵ // 2008-04-04 
    float           key_dev_ampl;	             	        //  �й�����ֵ // 2008-04-04 
    float           dev_ampl;                               //  ����ֵ // 2008-04-04 
    float           dev_ampl_rat;                           //  ��󲨶���(������ֵ/��ֵ) // 2008-04-04 
    int             deci_sur;		                        //  ������Դ(0 ������� 1 ���߲���) // 2008-04-04 
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
//���³�վ�������ӡ�����Ⱥ���Ƿ�����
typedef struct tab_fac_info_wamap
{
    float         lfo_fact;             // lfo��������
    unsigned char lfo_clus;			    //lfo����Ⱥ//default_uc1;
    unsigned char is_cntr;			    //lfo�Ƿ�����//default_uc2;
}TAB_FAC_INFO_WAMAP;
#define TAB_FAC_INFO_WAMAP_FLD "lfo_fact,lfo_clus,is_cntr"

//������·"�Ƿ���"
typedef struct tab_acln_device_wamap
{
    unsigned char if_lfo;			 //lfo�Ƿ���default_uc1;
}TAB_ACLN_DEVICE_WAMAP;
#define  TAB_ACLN_DEVICE_WAMAP_FLD "if_lfo"

//���·�����������ӡ�����Ⱥ
typedef struct tab_gen_device_wamap
{
    float         lfo_fact;          // lfo��������default_f3
    unsigned char lfo_clus;			 //lfo�Ƿ���default_uc1;
}TAB_GEN_DEVICE_WAMAP;
#define  TAB_GEN_DEVICE_WAMAP_FLD "lfo_fact,lfo_clus"
/******************************************************************************/
//�������ļ�ʱ�������ʱ����
typedef struct TempData
{
	INT_REC	  id;
	char  name[64];
	char  name_1[64];
	float data;
	float data1[DATA_NUM_MAX];
	float data2[DATA_NUM_MAX];
}TEMPDATA;

/* Ӧ�ñ����� */
// typedef struct app_variable_lfo
// {
//     //int var_id;	/* ID */
//     //int virtual_time;	/* ����ʱ�� */  // PMUʱ�䣬��Ϊˢ��
//     int milli_sec;	/* ������ */
//     int model_time;	/* ģ��ʱ�� */
//     char message[200];	/* ������Ϣ */  // ����Ϣ
//     int data_status;	/* ����״̬ */  // ����/���ݲ�ˢ��
//     int app_status;	/* Ӧ��״̬ */      // ����/Խ��/Խ�м���/Խ�����  ��Ƶ��ֻʹ������/Խ�м���
//     int acln_limit_1;	/* ��·����Խ�޸�������Ӧ���Խ���ż� */
//     int acln_limit_2;	/* ��·����Խ�޸�������Ӧ�м�Խ���ż� */
//     int acln_limit_3;	/* ��·����Խ�޸�������Ӧ���Խ���ż� */
//     int tr_limit_1;	/* ��ѹ������Խ�޸�������Ӧ���Խ���ż� */
//     int tr_limit_2;	/* ��ѹ������Խ�޸�������Ӧ�м�Խ���ż� */
//     int tr_limit_3;	/* ��ѹ������Խ�޸�������Ӧ���Խ���ż� */
//     int bus_low_limit_1;	/* ĸ�ߵ�ѹԽ���޸�������Ӧ���Խ���ż� */
//     int bus_low_limit_2;	/* ĸ�ߵ�ѹԽ���޸�������Ӧ�м�Խ���ż� */  // �澯��·����
//     int bus_low_limit_3;	/* ĸ�ߵ�ѹԽ���޸�������Ӧ���Խ���ż� */
//     int bus_up_limit_1;	/* ĸ�ߵ�ѹԽ���޸�������Ӧ���Խ���ż� */
//     int bus_up_limit_2;	/* ĸ�ߵ�ѹԽ���޸�������Ӧ�м�Խ���ż� */      // �澯���������
//     int bus_up_limit_3;	/* ĸ�ߵ�ѹԽ���޸�������Ӧ���Խ���ż� */
//     int bgp_limit_1;	/* ����Խ�޸�������Ӧ���Խ���ż� */
//     int bgp_limit_2;	/* ����Խ�޸�������Ӧ�м�Խ���ż� */
//     int bgp_limit_3;	/* ����Խ�޸�������Ӧ���Խ���ż� */
//     //int data_refresh_time;	/* ����ˢ��ʱ�� */
// }APP_VARIABLE_LFO;
// 
// #define APP_VARIABLE_LFO_DEFFIELDS "milli_sec,model_time,message,data_status,app_status,acln_limit_1,acln_limit_2,acln_limit_3,tr_limit_1,tr_limit_2,tr_limit_3,bus_low_limit_1,bus_low_limit_2,bus_low_limit_3,bus_up_limit_1,bus_up_limit_2,bus_up_limit_3,bgp_limit_1,bgp_limit_2,bgp_limit_3"

/* WAMSӦ��ģ��澯��Ϣ�� */
typedef struct wams_app_warn_info_wams_lfo
{
//    INT_REC record_id;	/* ��¼ID */
    unsigned char warn_level;	/* �澯�ȼ� */
    char app_info[128];	/* Ӧ�ø�Ҫ��Ϣ */
//    unsigned char app_status_change;	/* Ӧ��״̬�仯λ */
	RTDB_TIME refresh_tm;	/* ˢ��ʱ�� */
//    char graph_name[64];	/* �������� */
    int app_info_1;	/* �������1 */
    char app_info_desc_1[64];	/* �������1���� */
    int app_info_2;	/* �������2 */
    char app_info_desc_2[64];	/* �������2���� */
    int app_info_3;	/* �������3 */
    char app_info_desc_3[64];	/* �������3���� */
//     int app_info_4;	/* �������4 */
//     char app_info_desc_4[64];	/* �������4���� */
//     int app_info_5;	/* �������5 */
//     char app_info_desc_5[64];	/* �������5���� */
//     int app_info_6;	/* �������6 */
//     char app_info_desc_6[64];	/* �������6���� */
    unsigned char is_popup_theme;	/* �Ƿ�澯������ */
//    int app_module_id;	/* Ӧ��ģ������ */
}WAMS_APP_WARN_INFO_WAMS_LFO;

#define WAMS_APP_WARN_INFO_WAMS_LFO_DEFFIELDS "warn_level,app_info,refresh_tm,app_info_1,app_info_desc_1,app_info_2,app_info_desc_2,app_info_3,app_info_desc_3,is_popup_theme"


/******************************************************************************/


#endif