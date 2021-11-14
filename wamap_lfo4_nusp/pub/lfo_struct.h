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
#define LFO_THREAD_NUM 5  // �����̸߳���
#endif

#ifndef LFO_DATA_SOURCE_FILE
#define	LFO_DATA_SOURCE_FILE	    1	/* ��Ƶ�񵴰��� */
#define	LFO_DATA_SOURCE_REALTTIME	0	/* PMUʵʱ���� */
#endif

#ifndef LFO_SAVE_FLAG_IDEL
#define LFO_SAVE_FLAG_IDEL 0 // ����
#define LFO_SAVE_FLAG_START 1  // ��ʼ��
#define LFO_SAVE_FLAG_HOLD 2 // �񵴳���
#define LFO_SAVE_FLAG_END 3  // �񵴽���
#endif

#define LFO_UNSAVE_WARN 1 // ����ȫ�澯
#define LFO_STSOLD_WARN 2 // ���ݲ�ˢ�¸澯
#define LFO_SAVE_WARN 3 // �澯->��ȫʱ���͸澯

#define MAX_tIMW_WIN        40                    // ���ʱ�䴰��30��
#define DATA_NUM_MAX       (MAX_tIMW_WIN * MAX_FRA_PER_SEC)//  ��������������0.08/40
#define LfoRec_Time_Max  30       //15M
#define Period_Mode_Max  (LfoRec_Time_Max*6)
#define COHCY_ANGL 75.0f
#define LFO_CURVE_MAX_PNT   250       // ʵʱ����������
#define ZEROVALUE  0.0001
#define MULTI_MENU(n)    ((1<<(n-1))) // ��ѡ�˵�
#define CURVE_TIME 60     // ������ʾʱ��
#define CURVE_NUM_PER_SEC 25 //������ʾÿ�����
#define LPO_CURVE_MAX_PNT   CURVE_TIME*CURVE_NUM_PER_SEC       // ʵʱ����������
#define UNDISPLAY_VALUE 999999

//////////////////////////////////////////////////////////////////////////
// �������
typedef struct type_LFO_PARA_STRUCT
{
    // �������
    float freq_min;	/* Ƶ������ */
    float freq_max;	/* Ƶ������ */
    float lfo_freq_min; // ��Ƶ��Ƶ������
    float lfo_freq_max; // ��Ƶ��Ƶ������(��һ����¼��Ч)
    int   time_leng;	/* �۲�ʱ�䴰�� */  // �޸�Ϊfloat
    float time_leng_coe;  // �۲�ʱ�䴰��ϵ��
	float sample_freq;	/* ����Ƶ�� */  // �޸�Ϊfloat
	float sample_freq_coe;  // ����Ƶ��ϵ��
	int   int_time;	/* �������� */  // �޸�Ϊfloat
	float int_time_coe;  // ��������ϵ��
    float gn_wn_ampl;	/* ���������ֵ */
    float gn_wn2_ampl;	/* ��������澯�����ֵ */
    float ln_wn_ampl;	/* ��·�����ֵ */
    float ln_wn2_ampl;	/* ��·�����澯�����ֵ */
    float tr_wn_ampl;	/* ��ѹ�������ֵ */
	float tr_wn2_ampl;	/* ��ѹ�������澯�����ֵ */
	float stor_damp;	/* ����ģʽ������ֵ */
    float lack_damp;	/* ���᲻����ֵ */ // �������������ֵ
	float flfo_damp;	/* Ԥ��������ֵ */
	float lfo_damp;	/* �澯������ֵ */  // ��Ƶ���������ֵ
	float damp_min;  // ��������ֵ(�����Ҫ���ڸ�ֵ����Ч)
    float fjud_wnap_per;	/* Ԥ������ٷֱ� */
    float ana_ampl_per;  // �����Ƕȹ�������ٷֱ�
    int   lfo_jud_num;	/* �жϴ��� */
    int   warn_time_max;  // �澯����ʱ����ֵ(s)
    int   safe_time_max;  // ��ȫ��������ʱ����ֵ(s)
    int   delay_time;  // PMUʱ���ӳ�ʱ��
    float freq_diff;  // ͬģʽƵ�ʲ�
    float phas_diff;  // ͬģʽ�ǶȲ�
    int   cache_windows_num;  // ���洰��������
    float stor_ampl;  // ����ģʽ��ֵ��ֵ
    int   is_mon_lfo;  // �Ƿ���ӵ�Ƶ��;0,�жϹ�����;1,�жϵ�Ƶ�񵴺͹�����
    int   time_leng_min;  // ʱ�䴰����Сֵ
    int   time_leng_max;  // ʱ�䴰�����ֵ
    float sample_freq_min;  // ����Ƶ����Сֵ
    float sample_freq_max;  // ����Ƶ�����ֵ
    int   int_time_min;  // ����������Сֵ
    int   int_time_max;  // �����������ֵ
    float mode_save_amplper;  // ģʽ�����ֵ�ٷֱ�
	int rcd_pre_t;	/* ��¼��ǰʱ�� */
	int rcd_aft_t;	/* ��¼�Ӻ�ʱ�� */
	INT_REC lfo_fac_id;	/* ��Ӧ���⳧վID */
	int is_warn;	/* �Ƿ��͸澯 */
	int is_samp;	/* �Ƿ񴥷����� */
	int is_tmd;	/* �Ƿ񴥷�tmd */
	int is_triems;	/* �Ƿ���EMSת�� */
	int warn_app_no; // �澯����Ӧ�ú�(����������)
	int is_eng;  // �Ƿ���ʾӢ��(������Ŀ)
#ifdef _D5000
	INT_REC ems_keyid;	/* EMSת��KEYID */
#else
	KEY_ID_STRU ems_keyid;	/* EMSת��KEYID */
#endif
	float   cohcy_angl;	                            //  ͬ���ǶȲ�
    float   sh_fact;	//���������ż�ֵ
	float	sh_gn_pow;	//�����ɢ���ʷ�ֵ
	float	sh_ln_pow;	//֧·��ɢ���ʷ�ֵ
	float	sh_phas;	//��λ�ϴ�ֵ
	float   max_damp;//����ȱ���ֵ
    // �������߲���
    // prony��������(����PronyAnalysis.h�нṹ��)
    PRONY_CALC_PARA prony_para;
    // ���Ʋ���
    int  devinfo_source;  // 1,���豸��Ϣ�ļ���ȡ;0,�����ݿ��豸���ȡ;Ĭ��Ϊ1
    int  data_source;  // 0,�Ӷ�̬���ݿ��ȡPMU����;1,�������ļ��ж�ȡPMU����;Ĭ��Ϊ1
    int  is_add_facname;  // 1,����豸��վ����;0,������豸��վ����
    int  is_save_casefile;  // 1,���ɰ����ļ�;0,�����ɰ����ļ�
    int  is_save_devinfo;  // 1,�����豸��Ϣ�ļ�;0,�������豸��Ϣ�ļ�
    int  is_cycle;  // 1,ѭ����ȡ����;0,���ݶ�ȡʧ�ܺ��˳�
    // ����������
    // �������Բ���
    char devfile_name[MAXBUF];  // �豸��Ϣ�ļ�����
    char datafile_name[MAXBUF];  // �����ļ�����
    char devicefile_name[MAXBUF];  // �����豸�ļ�����
    int data_pos;  // ѭ��������ʼλ��
	int data_pos_old; //��ʱ���汾�ּ���ѭ��������ʼ
	int data_pos_osc; //С�Ŷ�����������ʼ��
	int file_start_time;//����ʱ����ʼʱ��
	int file_end_time;//����ʱ�����ʱ��
	int file_now_time;//����ʱ�䵱ǰʱ��
	int data_sample;//�����ļ�����֡��
	// ������ʾ
	int is_lfo_curve;  // �澯ʱ�Ƿ�������߿ؼ���ʾ������;
	int change_data_source;	//�Ƿ�����Դ�л�;
	int change_dev_num;	//�����豸�����Ƿ�仯;
//#ifdef _D5000
 	float			volt_thre1;								/* ��ѹ�ȼ�1 */
 	float			volt_thre2;								/* ��ѹ�ȼ�2 */
	float           ampl_thre1;	                            //  ��·�ͱ�ѹ�������ֵ1
	float           ampl_thre2;	                            //  ��·�ͱ�ѹ�������ֵ2
	float           ampl_thre3;	                            //  ��·�ͱ�ѹ�������ֵ3
	//�㶫��Ŀ����
	float			gn_wn_ampl1;							//	��������������ֵ
	float			ln_wn_ampl1;							//	��·�����������ֵ
	float			tr_wn_ampl1;							//	��ѹ�������������ֵ
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
	
//#endif
	float osc_pow_ampl;	/* С�Ŷ�����ż�ֵ */
	float osc_sec_ampl;	/* С�Ŷ�����ʱ���ż�ֵ */
	int rec_num;		//�ӳ����洰�ڸ��� 
	int if_flit_lfo;	//���ʲ����澯�Ƿ��ų���Ƶ��

	float			lowfreq_min;							//��Ƶ��Ƶ������
	float			lowfreq_max;							//��Ƶ��Ƶ������
	float			smp_freq_coe;							//����Ƶ��ϵ��
	float			warnlfo_time;							//�澯����ʱ����ֵ
	float			safelfo_time;							//��ȫ����ʱ����ֵ
	int				Voltunit_is_kV; //PMU�ɼ���ѹ��λ�Ƿ�ΪkV
	float			para_Ampl_aver;	//����ģʽ��ֵռ���ξ�ֵ�� 
	int             lfo_warn;

//�㶫��Ŀ���Ӹ澯 add by cyj 20170801
    float			gn_lfo_wn_ampl;								/* lfo_���������ֵ */
	float			gn_lfo_wn_ampl1;							//	lfo_��������������ֵ
	float			ln_lfo_swn_ampl1;						  //	lfo_��·��ʼ�������ֵ1  
	float			ln_lfo_swn_ampl2;							//  lfo_��·��ʼ�������ֵ2  
	float			ln_lfo_swn_ampl3;							//  lfo_��·��ʼ�������ֵ3  
	float			ln_lfo_ewn_ampl1;							//  lfo_��·�����������ֵ1  
	float			ln_lfo_ewn_ampl2;							//  lfo_��·�����������ֵ2  
	float			ln_lfo_ewn_ampl3;							//  lfo_��·�����������ֵ3  
	float			tr_lfo_swn_ampl1;							//  lfo_��ѹ����ʼ�������ֵ1
	float			tr_lfo_swn_ampl2;							//  lfo_��ѹ����ʼ�������ֵ2
	float			tr_lfo_swn_ampl3;							//  lfo_��ѹ����ʼ�������ֵ3
	float			tr_lfo_ewn_ampl1;							//  lfo_��ѹ�������������ֵ1
	float			tr_lfo_ewn_ampl2;							//  lfo_��ѹ�������������ֵ2
	float			tr_lfo_ewn_ampl3;							//  lfo_��ѹ�������������ֵ3
// 	vector<float>	vpre_freq;									//  Ԥ�����Ƶ��
	float			pre_freq1;
	float			pre_freq2;
	float			pre_freq3;
	float			pre_freq4;
	float			pre_freq5;
	int				curve_time_60;								//  �Ƿ�Ҫ����ʾ60s���� 1-��ʾ60������ 2-��ʾ20������
	int				thread_mode;								//  Ƶ�λ���ģʽ;0-Ԥ��,1-����Ӧ
	int				mainmode_num;								//  ��Ҫģʽ����
	float			inf_ampl;									//  �������ϵ��
	float			freq_disp;									//��ǰģʽƵ��
	float			sm_md_damp;//ͬģʽ�����
	float			para_VoltAmplPer;//  ��վ��ѹ��ֵ�ٷֱ�
	int				para_LfoAnaByPwr;
	int				para_nShwMaxFcNum;//  ÿȺ��ʾ��վ�����
	float			center_volt_ampl;	//�����ĵ�ѹ������С��ֵ ���ڸ�ֵ�Ż���������								//��ǰģʽƵ��
	int				disp_num;//��������ʾ�豸���� Ĭ����ʾ3��
	float gn_capacity1;	/* ��������1 */
	float gn_capacity2;	/* ��������2 */
	float gn_swn_ampl2;	/* ���鿪ʼ�������ֵ2 */
	float gn_swn_ampl3;	/* ���鿪ʼ�������ֵ3 */
	float gn_ewn_ampl2;	/* ��������������ֵ2 */
	float gn_ewn_ampl3;	/* ��������������ֵ3 */

}LFO_PARA_STRUCT;

//////////////////////////////////////////////////////////////////////////
// �豸��ʼ���ṹ����wamtoolstruct.h�����ݽṹ(WT_DEV_INFO)
//////////////////////////////////////////////////////////////////////////
// �豸������Ϣ
typedef struct  type_LFO_DEV_BASE
{
//  �豸��Ϣ
	INT_REC dev_idx;  // �豸����
	INT_REC dev_id;  // �豸ID��
	INT_REC fac_id;  // ��վID��
	INT_REC area_id;  // ����ID��
	char dev_name[64];  // �豸����
	char fac_name[64]; // ��վ����
	char area_name[64]; // ��������
	int dev_type;  // �豸����
	float ampl_limit_s;//��ʼ�����ֵ
	float ampl_limit_e;//���������ֵ
#ifdef _CHG_AMPL
	int is_chg_ampl;	/* �Ƿ���㶯̬�����ֵ */
	float chg_ampl_per;	/* ��̬�����ֵ�ٷֱ� */
#endif
//  ѡ��˵���Ϣ
    INT_REC dot_id; // �豸һ��ID��
    INT_REC dot_fac_id; // ��վID��
    INT_REC dot_area_id; // ����ID��
    
    char dot_dev_name[64]; // �豸����
    char dot_fac_name[64]; // ��վ����
    char dot_area_name[64]; // ��������
//  PMU�����Ϣ
    int  IsYcDefine;   // ����Ƿ����
    int  nYcFlag[VEC_VIP_NUM];
    int  nYcFilNo[VEC_VIP_NUM];
    int  nYcColNo[VEC_VIP_NUM];
// 	char yc_name[VEC_VIP_NUM][64]; // �������
}LFO_DEV_BASE;

// ����ͳ����Ϣ
typedef struct  type_LFO_CURVE_INFO
{
	float           max_val;                                //  ���ֵ
	RTDB_TIME         max_val_t;                              //  ���ֵʱ��
	float           min_val;                                //  ��Сֵ
	RTDB_TIME         min_val_t;                              //  ��Сֵʱ��
    float           max_ampl;                               //  ����ֵ
	float           aver_val;                               //  ��ֵ
	float           max_ampl_rat;                           //  ��󲨶���(������ֵ/��ֵ) 
}LFO_CURVE_INFO;

// ʵʱģʽ�ṹ
typedef struct  type_LFO_MODE_INFO
{
	RTDB_TIME         start_time;
	RTDB_TIME         end_time;
	float           ampl;                              //  ���
	float           freq;                              //  Ƶ��
	float           damp;                              //  �����
	float           phas;                              //  ����
	float           real;                              //  ʵ��
	float           imag;                              //  �鲿
	float           engy;                              //  ����
	float           engy_pre_time;                     //  ��λʱ������
	float           fail;                              //  ˥��
	float           aver_val;                          //  ƽ�����
	int             group;                             // ��Ⱥ��Ϣ
    float           fact;                             // ��������
	float			pow;								// ��ɢ����
	int				is_source;							// �Ƿ���Դ(ʵʱ����)
	int				eve_source;							// �Ƿ���Դ(���¼�)
	char			yc_name[64];						// �������
	char			in_or_out[8];						//���������
	int				stat;								//״̬
// 	float			vec_pow[250];						//ͳ�Ƹ����ں�ɢ����
// 	int				pow_num;							//ͳ�ƺ�ɢ���ʴ�����
}LFO_MODE_INFO;
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

// �豸��Ϣ�ṹ��
typedef struct  type_LFO_DEV_INFO
{
    LFO_DEV_BASE dev;  // �豸������Ϣ
    WT_DEV_INFO dev_info;  // �豸��̬��Ϣ
    // ���Ʋ���
//     int is_monitor;  // �Ƿ����;δ����(MENU_LFO_DEV_STS_NO	6);����(MENU_LFO_DEV_STS_OK 1)
//     int is_prony;  // �Ƿ����Prony����
    // PMU����(vector<float>)
	vector<float> v2d_data;  // ����(�й�),������
	vector<float> ana_data;  // ����(����),������
	vector<float> back_data;  // ����
// 	vector<float> back_ana;  // ����
   // ����/��ȫ״̬
    int stat;
	int stat_old;  // ��һ��״̬
	int save_flag;
    int stat_prony;// �Ƿ�ͨ��Ԥ��(�Ƿ���Ҫ�����ķ���)
    int stat_source;// �Ƿ�ͨ��Ԥ��(�Ƿ���Ҫ��Դ����)
	int is_sm_md;//�Ƿ�ͬ������Զ��棩
	char intline_des[128];//�����������
    LFO_CURVE_INFO curve;  // ����ͳ��
    LFO_CURVE_INFO curve_ana;  // ����ͳ��
    vector<LFO_MODE_INFO> vec_mode;  // ʵʱģʽ(�������ģʽ������)
    LFO_MODE_INFO mode;  // ����ģʽ(ʵʱģʽ�е��������ģʽ)
    LFO_MODE_INFO mode_shape;  // ģ̬��������������ģʽ��Ӧ�Ĳ����豸�а������豸��ʵʱģʽ���������ӣ��豸��Ⱥ
    vector<LFO_MODE_INFO> vec_lead_mode;  // �����񵴹���������ģʽ
	OSCILLAT_FLAG	osciFlag;//���жϽṹ
//     // �жϸ澯״̬ʱ��
//     int start_time_warn;
// 	int end_time_warn;
// 	int hold_time_warn;
	// �жϵ�Ƶ��״̬ʱ��
// 	RTDB_TIME start_time_lfo;
// 	RTDB_TIME end_time_lfo;
// 	int hold_time_lfo;
    // ���߳����о���/��ȫʱ��
    // ���߳���ͳ�������񵴹���ʱ��
	RTDB_TIME start_time;
	RTDB_TIME end_time;
	int hold_time;

//С�Ŷ�״̬��С�Ŷ�ͳ��ʱ��
//     int osc_stat;
// 	int osc_stat_old;  // ��һ��״̬
// 	int osc_save_flag;
//     RTDB_TIME osc_start_time;
//     RTDB_TIME osc_end_time;
// 	int osc_hold_time;

	int sourceflag;//�Ƿ�Ϊ��Դ�������֧·
	int judge_num; //����澯����

	unsigned char lfo_is_cntr;

}LFO_DEV_INFO;


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
typedef struct type_LFO_INTLINE_INFO
{
	INT_REC recordid;
	INT_REC inf_no;
	char inf_name[64];
	int eletype;
	INT_REC line_no;
	char line_name[64];
	int onoff;
	int mpoint;	/* ��� */
	INT_REC stat_no;	/* ��㳧վ */
	float ampl_limit_s;
	float ampl_limit_e;
    LFO_CURVE_INFO curve;  // ����ͳ��
    vector<LFO_MODE_INFO> vec_mode;  // ʵʱģʽ(�������ģʽ������)
    LFO_MODE_INFO mode;  // ����ģʽ(ʵʱģʽ�е��������ģʽ)
    LFO_MODE_INFO mode_shape;  // ģ̬��������������ģʽ��Ӧ�Ĳ����豸�а������豸��ʵʱģʽ���������ӣ��豸��Ⱥ
    vector<LFO_MODE_INFO> vec_lead_mode;  // �����񵴹���������ģʽ
	LFO_DEV_INFO dev_info;
}LFO_INTLINE_INFO;
//////////////////////////////////////////////////////////////////////////
// �豸ģʽ�ṹ(LFO_DEV_MODE)
typedef struct  type_LFO_DEV_MODE
{
    LFO_DEV_BASE dev;  // �豸������Ϣ
    LFO_MODE_INFO mode;  // ģʽ��Ϣ
}LFO_DEV_MODE;

//////////////////////////////////////////////////////////////////////////
// Prony�����ṹ
typedef struct  type_LFO_PRONY_INFO
{
    LFO_DEV_BASE dev;  // �豸������Ϣ
    // PMU����(vector<float>)
    vector<float> v2d_data;
    vector<LFO_MODE_INFO> vec_mode;  // ʵʱģʽ(�������ģʽ������)
}LFO_PRONY_INFO;

//////////////////////////////////////////////////////////////////////////
// ģ̬�����ṹ
typedef struct  type_LFO_MODE_SHAPE
{
    LFO_MODE_INFO mode;  // ����ģʽ(���ģʽ����������)
    vector<LFO_DEV_MODE> vec_dev;  // ���ģʽ�Ĳ����豸(������Ϣ��ʵʱģʽ���������ӣ��豸��Ⱥ)
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
// ϵͳ״̬�ṹ��
typedef struct  type_LFO_SYS_INFO
{
	RTDB_TIME window_id;  // ����ID
	float freq_min;  // ����Ƶ�ʶ�����
	float freq_max;  // ����Ƶ�ʶ�����
    int gps_time;  // PMU����ʱ��
    int gps_time_old;  // ��һ�ַ���PMU����ʱ��
	RTDB_TIME start_time;  // ���ַ�����ʼʱ��
	RTDB_TIME end_time;  // ���ַ�������ʱ��
    float sample_org;  // ԭʼ���ݲ������
    // ��ȫ״̬
    int stat;
	int stat_old;
	char stat_desc[200];  // ��������Ϣ
	char lfo_desc[200];  // ����������Ϣ
    int save_flag;
	int lfo_warn_flag;   //�������¼����Ƿ��Ѿ����͵�Ƶ�񵴸澯��־
	int lfo_type;//��Ƶ������ 1��ʾ��Ƶ�� 2��ʾ������ʲ���
    INT_REC         center_id;	                            //  ������
    char center_desc[128];                                  //  ����������

    // ���豸����������ģʽ�в����豸����
//     LFO_MODE_INFO lead_mode;  // ����ģʽ
//     LFO_DEV_BASE lead_dev;  // ������������豸
	LFO_DEV_INFO lead_dev;  // ��������豸���豸ģʽ������
	LFO_DEV_INFO lead_gen;  // ������Ļ��飬�豸ģʽ������
    vector<LFO_DEV_INFO> vec_dev;  // �������豸��Ϣ
    vector<LFO_DEV_INFO> cen_dev;  // �������豸��Ϣ
    vector<LFO_MODE_SHAPE> vec_modeshape;  // ����ģ̬������Ϣ
    vector<LFO_MODE_SHAPE> vec_mainmode;  // ��Ҫģʽ
	int lfo_gn_num; // �񵴻������
	int lfo_ln_num;  // ����·����
	int lfo_tr_num;  // �񵴱�ѹ������
	int lfo_inf_num;  // �񵴱�ѹ������
	char lfos_gn_name[64];	/* ��Դ�������� */
	int lfos_gn_num;	/* ��Դ������Ŀ */
	int lfos_ln_num;	/* ��Դ��·��Ŀ */
	int lfos_tr_num;	/* ��Դ��ѹ����Ŀ */
	char lfos_des[128];	/* ��Դ���� */
	float progress; //�������
	int file_end;//�����ļ��Ƿ������־
	int warn_thread;//�澯�̺߳�
}LFO_SYS_INFO;
//////////////////////////////////////////////////////////////////////////
// ÿ���������ڽṹ
typedef struct  type_LFO_WINDOW_INFO
{
// 1)	����ID
	INT_REC window_id;
    vector<LFO_SYS_INFO> vec_sys;  // 3)	ϵͳ״̬�ṹ(���Ƶ�ʶζ����ڸ澯)
}LFO_WINDOW_INFO;
//////////////////////////////////////////////////////////////////////////
// ���¼��ṹ
typedef struct  type_LFO_CASE_INFO
{
// 1)	�¼�ID�����ڸ���
	RTDB_TIME event_id;
	int stat;  // ��Ƶ�񵴻�����
	char case_desc[128];  // ������
	int lfo_gn_num; // �񵴻������
	int lfo_ln_num;  // ����·����
	int lfo_tr_num;  // �񵴱�ѹ������
	int lfo_inf_num;  // �񵴱�ѹ������
    vector<LFO_SYS_INFO> vec_windows;  // 2)	ÿ�����ڷ������
// 3)	��ͳ�ƣ��񵴿�ʼʱ�䣬�񵴽���ʱ�䣬�񵴳���ʱ�䣬ƽ��Ƶ�ʣ���С����ȣ������澯�豸��
	RTDB_TIME start_time;
	RTDB_TIME end_time;
    int hold_time;
	LFO_MODE_INFO lead_mode;
	vector<LFO_MODE_SHAPE> vec_mainmode;
    LFO_DEV_INFO warn_dev;
    vector<LFO_DEV_INFO> vec_dev;  // 4)	���豸ͳ�ƣ������豸ͳ�Ʊ������豸��ʼʱ�䣬����ʱ�䣬����ʱ�䣬����ͳ�ƣ�, ���豸ģʽ�������豸ͳ��ÿ���������ڵ�����ģʽ��
    vector<LFO_DEV_INFO> cen_dev;  // 4)	������ͳ�ƣ������豸ͳ�Ʊ������豸��ʼʱ�䣬����ʱ�䣬����ʱ�䣬����ͳ�ƣ�, ���豸ģʽ�������豸ͳ��ÿ���������ڵ�����ģʽ��
    vector<LFO_DEV_INFO> sou_dev;  // 4)	��Դͳ�ƣ������豸ͳ�Ʊ������豸��ʼʱ�䣬����ʱ�䣬����ʱ�䣬����ͳ�ƣ�, ���豸ģʽ�������豸ͳ��ÿ���������ڵ�����ģʽ��
//    vector<LFO_MODE_SHAPE> vec_modeshape;  // 5)	ÿ����������ģʽģ̬���
	int is_lfo_source;  /* �Ƿ�ǿ���� */
	char lfos_gn_name[64];	/* ��Դ�������� */
	int lfos_gn_num;	/* ��Դ������Ŀ */
	int lfos_ln_num;	/* ��Դ��·��Ŀ */
	int lfos_tr_num;	/* ��Դ��ѹ����Ŀ */
	char lfos_des[128];	/* ��Դ���� */
	char center_desc[128]; /* ���������� */
	float progress; //�������
	int lfo_modle;	/* ��ģʽ */
	int lfo_type;	/* ������ */
	char type_desc[64]; //����������
	char lfo_ana_des[128]; //�񵴷�������
	char asst_desc[128]; //������������
}LFO_CASE_INFO;
//////////////////////////////////////////////////////////////////////////
// �߳����ݱ���ṹ
typedef struct type_LFO_THREAD_INFO
{
    vector<LFO_DEV_INFO> VecDev;  // �̵߳��豸��Ϣ����
    //vector<LFO_MODE_SHAPE> VecModeShape;  // �̵߳�ģ̬��Ϣ����
    LFO_SYS_INFO MonSys; // �̵߳�ϵͳ��Ϣ����
// 	LFO_SYS_INFO oscMonSys; // �̵߳�ϵͳ��Ϣ����
}LFO_THREAD_INFO;

typedef struct type_LFO_CASE_FILE_DEV
{
	INT_REC fac_id;  // ��վID
	INT_REC dev_id;  // �豸ID(�˵�)
	INT_REC dev_id1;  // �豸ID
	int column_id;  // ��ID
#ifndef _D5000
	KEY_ID_STRU yc_id;
#else
	INT_REC yc_id;  // ң��ID
#endif
	int file_no;  // �ļ���
	int col_no;  // �к�
	int is_warn;  // �Ƿ����豸
}LFO_CASE_FILE_DEV;
//////////////////////////////////////////////////////////////////////////
// ��������ṹ
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
	int lfo_modle;	/* ��ģʽ */
	int lfo_type;	/* ������ */
	char type_desc[64];//����������
	vector<LFO_CASE_FILE_DEV> vecdev;
}LFO_CASE_FILE_STRUCT;
//////////////////////////////////////////////////////////////////////////
// �豸״̬�жϽṹ
typedef struct type_LFO_DEV_STAT
{
	int warn_type; // 0,�а�ȫ;1,�и澯

	float freq;  // Ƶ��
	float ampl;  // ��ֵ
	float damp;  // �����
	int   start_time;  // ��ʼʱ��
	int   end_time;  // ����ʱ��
	int   hold_time;  // ����ʱ��
	int   ana_start_time;  // ���ַ�����ʼʱ��
	int   ana_end_time;  // ���ַ�������ʱ��
	
	float freq_min;  // Ƶ����Сֵ
	float freq_max;  // Ƶ�����ֵ
	float ampl_thre;  // ��ֵ�ż�ֵ
	float damp_thre;  // ������ż�ֵ
	float damp_min;  // �������Сֵ
	int   time_max;  // ����ʱ���ż�ֵ
	
	int   stat;  // ״̬;0,����������;1,��������

	float lfofreq_min;  // ��Ƶ��Ƶ����Сֵ
	float lfofreq_max;  // ��Ƶ��Ƶ�����ֵ
	float lfodamp_thre;  // ��Ƶ��������ż�ֵ
	float aver_ampl;
}LFO_DEV_STAT;
//////////////////////////////////////////////////////////////////////////
// ͳ����������豸��Ϣ
typedef struct type_LPO_DEV_INFO
{
//	INT_REC                 dev_idx;              //�豸����        
	INT_REC                 dev_id;               //�豸ID          
	INT_REC                 fac_id_1;             //�׶˳�վID��    
	INT_REC                 area_id_1;            //�׶�����ID��    
	INT_REC                 fac_id_2;             //ĩ�˳�վID��    
	INT_REC                 area_id_2;             //ĩ������ID��    
	char                    dev_name[64];         //�豸��          
	char                    fac_name_1[64];       //�׶˳�վ��      
	char                    fac_name_2[64];       //ĩ�˳�վ��      
	char                    area_name_1[64];      //�׶�������      
	char                    area_name_2[64];      //ĩ��������      
	INT_REC                 dev_type;             //�豸����        
	unsigned char           is_monitor;           //�Ƿ����        
	unsigned char	          is_part_ana;          //�Ƿ�������    
	float                   lead_ampl;            //���            
	float                   lead_freq;            //Ƶ��            
	float                   lead_damp;            //�����          
	float                   lead_phas;            //����            
	float                   lead_real;            //ʵ��            
	float                   lead_imag;            //�鲿            
	float                   lead_engy;            //����            
	int                     if_lfo;               //�Ƿ������    
	RTDB_TIME                 slfo_bgn_t;           //����LFO����ʱ�� 
	RTDB_TIME                 slfo_end_t;           //����LFO��ʧʱ�� 
	char                    slfo_time[20];        //����ʱ��        
	char                    file_dir[256];        //��·�����ļ�����
	vector<float>          v2d_data;
}LPO_DEV_INFO;
#define LPODEVINFOINSERT "dev_id,fac_id_1,area_id_1,fac_id_2,area_id_2,dev_name,fac_name_1,fac_name_2,area_name_1,area_name_2,dev_type,is_monitor,is_part_ana,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,if_lfo,slfo_bgn_t,slfo_end_t,slfo_time,file_dir"
//////////////////////////////////////////////////////////////////////////
// LFO�������ȡ�ṹ
typedef struct type_LFO_PARA_TABLE_READ
{
//	int record_id;	/* ��¼ID */
	INT_REC record_id;	/* ��¼ID */
	int dsource;	/* ����Դ */
	int mem_max_sec;	/* ����������� */
	int smp_freq;	/* ����Ƶ�� */
	float fjud_wnap_per;	/* Ԥ������ٷֱ� */
	int flt_len;	/* �˲����� */
	float flt_coe;	/* �˲�ϵ�� */
	int time_leng;	/* �۲�ʱ�䴰�� */
	int int_time;	/* �������� */
	float freq_min;	/* Ƶ������ */
	float freq_max;	/* Ƶ������ */
	float stor_damp;	/* ����ģʽ������ֵ */
	float lack_damp;	/* ���᲻����ֵ */
	float flfo_damp;	/* Ԥ��������ֵ */
	float lfo_damp;	/* �澯������ֵ */ //��Ƶ��
	float gn_wn_ampl;	/* ���������ֵ */
	float gn_wn2_ampl;	/* �����澯�����ֵ */
	float ln_wn_ampl;	/* ��·�����ֵ */
	float ln_wn2_ampl;	/* �����澯�����ֵ */
	int lfo_jud_num;	/* �жϴ��� */
	float lfo_tim_freq;	/* ʱ������Ƶ�� */
	int rcd_pre_t;	/* ��¼��ǰʱ�� */
	int rcd_aft_t;	/* ��¼�Ӻ�ʱ�� */
	int htime_max;	/* �������ʱ�� */
	int show_max;	/* ���������ʾ */
	INT_REC lfo_fac_id;	/* ��Ӧ���⳧վID */
	float sm_md_freq;	/* ͬģƵ�ʲ� */
	float sm_md_damp;	/* ͬģ����� */
	float cohcy_angl;	/* ͬ���ǶȲ� */

	unsigned char is_warn;	/* �Ƿ��͸澯 */
	unsigned char is_samp;	/* �Ƿ񴥷����� */
	unsigned char is_tmd;	/* �Ƿ񴥷�tmd */
	unsigned char is_statestm;	/* �Ƿ�����״̬���� */
	RTDB_TIME his_bgn;	/* ��ѯ��ʼʱ�� */
	RTDB_TIME his_end;	/* ��ѯ��ֹʱ�� */

	int mon_ctl;	/* �������п��� */
	unsigned char is_triems;	/* �Ƿ���EMSת�� */
#ifdef _D5000
	INT_REC ems_keyid;	/* EMSת��KEYID */
#else
	KEY_ID_STRU ems_keyid;	/* EMSת��KEYID */
#endif
	float tr_wn_ampl;	/* ��ѹ�������ֵ */
	float tr_wn2_ampl;	/* �����澯�����ֵ */

	float lowfreq_min;	/* ��Ƶ��Ƶ������ */  // float sh_gn_fact;	/* ������������ż� */
	float lowfreq_max;	/* ��Ƶ��Ƶ������ */  // float sh_gn_ampl;	/* ��������ֵ�ż� */
	float time_leng_coe;	/* �۲�ʱ�䴰��ϵ�� */  // float sh_gn_num;	/* �����������ż� */
	float smp_freq_coe;	/* ����Ƶ��ϵ�� */  // float sh_ln_fact;	/* �ɹ���·�����ż� */
	float int_time_coe;	/* ��������ϵ�� */  // float sh_ln_ampl;	/* �ɹ���·��ֵ�ż� */
	int warnlfo_time;	/* �澯����ʱ����ֵ */  // int htime_max;	/* �������ʱ�� */
	int safelfo_time;	/* ��ȫ����ʱ����ֵ */  // int show_max;	/* ���������ʾ */
	float sh_fact;	/* �������ӷ�ֵ */
	float sh_phas;	/* ��ǳ�ǰ��ֵ */
	float sh_gn_pow;	/* �����ɢ���ʷ�ֵ */
	float sh_ln_pow;	/* ֧·��ɢ���ʷ�ֵ */

	float			volt_thre1;								/* ��ѹ�ȼ�1 */
	float			volt_thre2;								/* ��ѹ�ȼ�2 */
	float           ampl_thre1;	                            //  ��·�ͱ�ѹ�������ֵ1
	float           ampl_thre2;	                            //  ��·�ͱ�ѹ�������ֵ2
	float           ampl_thre3;	                            //  ��·�ͱ�ѹ�������ֵ3
	//�㶫��Ŀ����
	float			gn_wn_ampl1;							//	��������������ֵ
	float			ln_wn_ampl1;							//	��·�����������ֵ
	float			tr_wn_ampl1;							//	��ѹ�������������ֵ
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
	float osc_pow_ampl;	/* С�Ŷ�����ż�ֵ */
	float osc_sec_ampl;	/* С�Ŷ�����ʱ���ż�ֵ */

	float gn_capacity1;	/* ��������1 */
	float gn_capacity2;	/* ��������2 */
	float gn_swn_ampl2;	/* ���鿪ʼ�������ֵ2 */
	float gn_swn_ampl3;	/* ���鿪ʼ�������ֵ3 */
	float gn_ewn_ampl2;	/* ��������������ֵ2 */
	float gn_ewn_ampl3;	/* ��������������ֵ3 */
	int thread_mode;	/* ��Ƶ�η�ʽ */
	float pre_freq1;	/* Ԥ��Ƶ�ʶ�1 */
	float pre_freq2;	/* Ԥ��Ƶ�ʶ�2 */
	float pre_freq3;	/* Ԥ��Ƶ�ʶ�3 */
	float pre_freq4;	/* Ԥ��Ƶ�ʶ�4 */
	float pre_freq5;	/* Ԥ��Ƶ�ʶ�5 */
	float phas_diff;	/* ����ͬ����λ�ǶȲ� */
	int mainmode_num;	/* չʾģʽ���� */
	float inf_ampl;	/* �������ϵ�� */
	float center_volt_ampl;	/* �����ĵ�ѹ������ֵ */
	float para_voltamplper;	/* ��վ��ѹ��ֵϵ�� */
	int para_nshwmaxfcnum;	/* ��������ʾ��೧վ�� */
	float para_ampl_aver;	/* ����ģʽ��ֵ/��������ϵ�� */
	float max_damp;	/* ����ȱ���ֵ */
	int disp_num;	/* �����ϴ��豸��ʾ���� */
	int rec_num;	/* ���永��ǰ�ƴ��ڸ��� */
	int voltunit_is_kv;	/* ��ѹ��λ�Ƿ�Ϊǧ�� */
	int is_cycle;	/* �Ƿ�ѭ����ȡ���� */
	int is_lfo_curve;	/* �Ƿ�������߿ؼ� */
	int warn_app_no;	/* ���͸澯Ӧ�ú� */
	int is_save_casefile;	/* �Ƿ����ɰ����ļ� */
	int is_save_devinfo;	/* �Ƿ������豸��Ϣ�ļ� */
	int is_add_facname;	/* �Ƿ���ӳ�վ�� */
	int is_eng;	/* �Ƿ�Ӣ�İ� */

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
// ����ԭ����������
//#define LFO_PARA_TABLE_READ_FLD "record_id,dsource,mem_max_sec,smp_freq,fjud_wnap_per,flt_len,flt_coe,time_leng,int_time,freq_min,"\
//    	"freq_max,stor_damp,lack_damp,flfo_damp,lfo_damp,gn_wn_ampl,gn_wn2_ampl,ln_wn_ampl,ln_wn2_ampl,lfo_jud_num,lfo_tim_freq,rcd_pre_t,rcd_aft_t,"\
//			"htime_max,show_max,lfo_fac_id,sm_md_freq,sm_md_damp,cohcy_angl,is_warn,is_samp,is_tmd,is_statestm,his_bgn,his_end,is_triems,ems_keyid,"\
//    	"tr_wn_ampl,tr_wn2_ampl,sh_gn_fact,sh_gn_ampl,sh_gn_num,sh_ln_fact,sh_ln_ampl,htime_max,show_max"
////#endif
// LFO������д��ṹ
typedef struct type_LFO_PARA_TABLE_MONWRITE
{
	//int record_id;	/* ��¼ID */
	//int run_mode;	/* ���з�ʽ */
	
	RTDB_TIME cur_time;	/* ��ǰʱ�� */
	int stat;	/* ״̬ */
	int stat_val;	/* ״ֵ̬ */
	char stat_ctx[200];	/* ״̬��Ϣ */

// 	RTDB_TIME lfo_bgn2;	/* LFO����ʱ��2 */
// 	char lfo_time2[20];	/* ����ʱ��2 */
// 	int lfo_modle2;	/* ��ģʽ2 */
// 	int lfo_type2;	/* ������2 */
// 	int is_lfo_source2;	/* �Ƿ�ǿ����2 */
	char lfos_des[128];	/* ��Դ����2 */
// 	char lfo_ana_des2[200];	/* �񵴷�������2 */
	
	
//	char case_type_des[128];	/* ���������� */
}LFO_PARA_TABLE_MONWRITE;
#define LFO_PARA_TABLE_MONWRITE_FLD "cur_time,stat2,stat_val2,stat_ctx2,lfos_des"

typedef struct type_LFO_PARA_TABLE_WRITE
{
	//int record_id;	/* ��¼ID */
	//int run_mode;	/* ���з�ʽ */

// 	int cur_time;	/* ��ǰʱ�� */
// 	int stat;	/* ״̬ */
// 	int stat_val;	/* ״ֵ̬ */
// 	char stat_ctx[200];	/* ״̬��Ϣ */

	char case_des[128];	/* �񵴰������� */
	RTDB_TIME lfo_bgn;	/* LFO����ʱ�� */
	RTDB_TIME lfo_end;	/* LFO��ʧʱ�� */
	INT_REC warn_dev_id2;	/* �����澯�豸ID */
	char warn_dev_name2[64];	/* �����澯�豸 */
	float warn_freq;	/* �����澯Ƶ�� */
	float aver_freq;	/* ����ƽ��Ƶ�� */
	int lfo_ln_num;	/* LFO��·�� */
	int lfo_gn_num;	/* LFO������� */
	char center_desc[128];	/* ���������� */
	int mode_num2;	/* ģʽ��2 */
	char center_desc2[128];	/* ����������2 */
	float lead_freq2;	/* Ƶ��2 */
	float lead_damp2;	/* �����2 */
	char case_des2[128];	/* �񵴰�������2 */
	RTDB_TIME lfo_bgn2;	/* LFO����ʱ��2 */
	char lfo_time2[20];	/* ����ʱ��2 */
	int lfo_tr_num;	/* LFO��ѹ���� */
	char mon_use_time[20];	/* ���Ӽ����ʱ */

	char lfos_gn_name[64];	/* ��Դ�������� */
	int lfos_gn_num;	/* ��Դ������Ŀ */
	int lfos_ln_num;	/* ��Դ��·��Ŀ */
	int lfos_tr_num;	/* ��Դ��ѹ����Ŀ */
// 	char lfos_des[128];	/* ��Դ���� */
	int lfo_inf_num;
// 	char lfo_ana_des[200];	/* �񵴷������� */
	char lfo_ana_des2[200];	/* �񵴷�������2 */
// 	int lfo_modle;	/* ��ģʽ */
// 	int lfo_type;	/* ������ */
// 	int is_lfo_source;	/* �Ƿ�ǿ���� */
	int lfo_modle2;	/* ��ģʽ2 */
	int lfo_type2;	/* ������2 */
	int is_lfo_source2;	/* �Ƿ�ǿ����2 */
	float warn_damp;	/* �澯����� */
	char lfos_des2[128];	/* ��Դ����2 */
// 	char lfo_bgn_des[64];	/* LFO����ʱ������ */
// 	char lfo_end_des[64];	/* LFO��ʧʱ������ */
	char lfo_bgn_des2[64];	/* LFO����ʱ������2 */
	char lfo_end_des2[64];	/* LFO��ʧʱ������2 */
// 	char is_source_des[8];	/* �Ƿ�ǿ�������� */
	char is_source_des2[8];	/* �Ƿ�ǿ��������2 */

	//char case_type_des[128];	/* ���������� */
}LFO_PARA_TABLE_WRITE;
//#define LFO_PARA_TABLE_WRITE_FLD "case_des,lfo_bgn,lfo_end,lfo_time,warn_dev_id,warn_dev_name,warn_freq,aver_freq,lfo_ln_num,lfo_gn_num,lfo_tr_num,case_type_des"
#define LFO_PARA_TABLE_WRITE_FLD "case_des,lfo_bgn,lfo_end,warn_dev_id2,warn_dev_name2,warn_freq,aver_freq,lfo_ln_num,lfo_gn_num,center_desc,mode_num2,center_desc2,lead_freq2,lead_damp2,case_des2,lfo_bgn2,lfo_time2,lfo_tr_num,mon_use_time,lfos_gn_name,lfos_gn_num,lfos_ln_num,lfos_tr_num,lfo_inf_num,lfo_ana_des2,lfo_modle2,lfo_type2,is_lfo_source2,warn_damp,lfos_des2,lfo_bgn_des2,lfo_end_des2,is_source_des2"

typedef struct type_LFO_PARA_TABLE_CLEAR
{
	char lfo_time[20];	/* ����ʱ�� */
	INT_REC warn_dev_id;	/* �����澯�豸ID */
	char warn_dev_name[64];	/* �����澯�豸 */
	int stat2;	/* ״̬2 */
	int stat_val2;	/* ״ֵ̬2 */
	char stat_ctx2[200];	/* ״̬��Ϣ2 */
	float mon_fail2;/* mon˥��2 */
	float lead_freq2;	/* Ƶ��2 */
	float lead_damp2;	/* �����2 */
	char case_des2[128];	/* �񵴰�������2 */
	RTDB_TIME lfo_bgn2;	/* LFO����ʱ��2 */
	char lfo_time2[20];	/* ����ʱ��2 */
	INT_REC warn_dev_id2;	/* �����澯�豸ID2 */
	char warn_dev_name2[64];	/* �����澯�豸2 */
	char center_desc2[128];	/* ����������2 */
	
	int lfo_modle2;	/* ��ģʽ2 */
	int lfo_type2;	/* ������2 */
	int is_lfo_source2;	/* �Ƿ�ǿ����2 */
	char lfos_des2[128];	/* ��Դ����2 */
	
	char lfo_bgn_des2[64];	/* LFO����ʱ������2 */
	char lfo_end_des2[64];	/* LFO��ʧʱ������2 */
	char is_source_des2[8];	/* �Ƿ�ǿ��������2 */
	//char case_type_des[128];	/* ���������� */
	char lfo_ana_des[200];	/* �񵴷������� */
	float lead_freq;	/* Ƶ��2 */
	float lead_damp;	/* �����2 */
	
	// 	char lfos_des[128];	/* ��Դ���� */
	int lfo_modle;	/* ��ģʽ */
	int lfo_type;	/* ������ */
	int is_lfo_source;	/* �Ƿ�ǿ���� */
	char lfo_bgn_des[64];	/* LFO����ʱ������ */
	char lfo_end_des[64];	/* LFO��ʧʱ������ */
	char is_source_des[8];	/* �Ƿ�ǿ�������� */
	char mode_source_des[8];	/* ��ǰģʽ�Ƿ�ǿ�������� */
	char mode_ana_des[200];	/* ��ǰģʽ�񵴷������� */

}LFO_PARA_TABLE_CLEAR;
#define LFO_PARA_TABLE_CLEAR_FLD "lfo_time,warn_dev_id,warn_dev_name,stat2,stat_val2,stat_ctx2,mon_fail2,lead_freq2,lead_damp2,case_des2,lfo_bgn2,lfo_time2,warn_dev_id2,warn_dev_name2,center_desc2,lfo_modle2,lfo_type2,is_lfo_source2,lfos_des2,lfo_bgn_des2,lfo_end_des2,is_source_des2,"\
								 "lfo_ana_des,lead_freq,lead_damp,lfo_modle,lfo_type,is_lfo_source,lfo_bgn_des,lfo_end_des,is_source_des,mode_source_des,mode_ana_des"

// LFO��������ȡ�ṹ������wamtoolstruct.h�нṹLFO_GN_TAB_CRE��
// LFO�������ʵʱ��Ϣд��ṹ
typedef struct type_LFO_GN_TAB_MONWRITE
{
	//int gn_idx;	/* ��������� */
	
	int stat;	/* ״̬2 */
	int stat_val;	/* ״ֵ̬ */
	
	float lead_ampl;	/* ��� */
	float lead_freq;	/* Ƶ�� */
	float lead_damp;	/* ����� */
	float lead_phas;	/* ���� */
	float lead_real;	/* ʵ�� */
	float lead_imag;	/* �鲿 */
	float lead_engy;	/* ���� */
	float lead_fail;	/* ˥�� */
	float aver_val;	/* ��ֵ */
	float max_val;	/* ���ֵ */
	RTDB_TIME max_val_t;	/* ���ֵʱ�� */
	float min_val;	/* ��Сֵ */
	RTDB_TIME min_val_t;	/* ��Сֵʱ�� */
	float max_min_dif;	/* ���ڶ� */
	float fact;	/* ��������2 */
	int fact_sts;	/* ��������״̬2 */
//  	float lfo_max_min_dif;	/* LFO���ڶ� */	
// 	float fact;	/* �������� */
// 	int is_lfo_source;	/* �Ƿ��Ŷ�Դ */
	float lead_gen_pow;	/* ��ɢ���� */
// 	int fact_sts;	/* ��������״̬ */
// 	int if_lfo;	/* �Ƿ������ */
// 	// 	float lfo_bgn_val;	/* LFOǰ��ֵ */
// 	// 	float lfo_end_val;	/* LFO���ֵ */
// 	int slfo_bgn_t;	/* ����LFO����ʱ�� */
// 	int slfo_end_t;	/* ����LFO��ʧʱ�� */
// 	char slfo_time[20];	/* ����ʱ�� */
// 	float slfo_bgn_val;	/* ����LFOǰ��ֵ */
// 	float slfo_end_val;	/* ����LFO���ֵ */
// 	float lfo_max_val;	/* LFO���ֵ */
// 	int lfo_max_val_t;	/* LFO���ֵʱ�� */
// 	float lfo_min_val;	/* LFO��Сֵ */
// 	int lfo_min_val_t;	/* LFO��Сֵʱ�� */

	
	unsigned char clus;	/* ����Ⱥ */
	int is_lfo_source;	/* �Ƿ��Ŷ�Դ */

// 	float max_ampl;	/* ����ֵ */
// 	float max_ampl_rat;	/* ��󲨶��� */
// 	unsigned char is_ang_calc;	/* �����Ƿ����ֵ */

//	float aver_fact;	/* ��������ƽ��ֵ */
}LFO_GN_TAB_MONWRITE;
// #define LFO_GN_TAB_MONWRITE_FLD "stat,stat_val,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,"\
//                                 "max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,is_ang_calc"
#define LFO_GN_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,max_min_dif2,fact2,fact_sts2,lead_gen_pow,clus,is_lfo_source"

typedef struct type_LFO_GN_TAB_WRITE
{
	//int gn_idx;	/* ��������� */

// 	int stat;	/* ״̬ */
// 	int stat_val;	/* ״ֵ̬ */
// 
//  	float lead_ampl;	/* ���2 */
//  	float lead_freq;	/* Ƶ��2 */
//  	float lead_damp;	/* �����2 */
//  	float lead_phas;	/* ����2 */
//  	float lead_real;	/* ʵ��2 */
//  	float lead_imag;	/* �鲿2 */
//  	float lead_engy;	/* ����2 */
// 	float lead_fail;	/* ˥�� */
// 	float aver_val;	/* ��ֵ */
// 	float max_val;	/* ���ֵ */
// 	int max_val_t;	/* ���ֵʱ�� */
// 	float min_val;	/* ��Сֵ */
// 	int min_val_t;	/* ��Сֵʱ�� */
// 	float max_min_dif;	/* ���ڶ� */

// 	float fact;	/* ��������2 */
	int fact_sts;	/* ��������״̬2 */
	int if_lfo;	/* �Ƿ������2 */
// 	float lfo_bgn_val;	/* LFOǰ��ֵ2 */
// 	float lfo_end_val;	/* LFO���ֵ2 */
	RTDB_TIME slfo_bgn_t;	/* ����LFO����ʱ��2 */
	RTDB_TIME slfo_end_t;	/* ����LFO��ʧʱ��2 */
	char lfo_time[20];	/* ����ʱ��2 */
	float slfo_bgn_val;	/* ����LFOǰ��ֵ2 */
	float slfo_end_val;	/* ����LFO���ֵ2 */
	float lfo_max_val;	/* ���ֵ2 */
	RTDB_TIME lfo_max_val_t;	/* ���ֵʱ��2 */
	float lfo_min_val;	/* ��Сֵ2 */
	RTDB_TIME lfo_min_val_t;	/* ��Сֵʱ��2 */
	float lfo_max_min_dif;	/* ���ڶ�2 */
// 	int is_lfo_source;	/* �Ƿ��Ŷ�Դ2 */
	int is_lfo_source2;	/* �Ƿ��Ŷ�Դ2 */
	float lead_gen_pow;	/* ��ɢ����2 */
	unsigned char lfo_is_disp2;	/* �Ƿ���ʾ */
// 	unsigned char clus;	/* ����Ⱥ */
// 	float max_ampl;	/* ����ֵ */
// 	float max_ampl_rat;	/* ��󲨶��� */
//	unsigned char is_ang_calc;	/* �����Ƿ����ֵ */
	//float aver_fact;	/* ��������ƽ��ֵ */
}LFO_GN_TAB_WRITE;
#define LFO_GN_TAB_WRITE_FLD "fact_sts2,if_lfo2,slfo_bgn_t2,slfo_end_t2,lfo_time2,slfo_bgn_val2,slfo_end_val2,lfo_max_val2,lfo_max_val_t2,lfo_min_val2,lfo_min_val_t2,lfo_max_min_dif2,is_lfo_source2,lead_gen_pow2,lfo_is_disp2"


typedef struct type_LFO_FC_TAB_WRITE
{
	float v_max;	/* ��ѹ���ֵ2 */
	float v_min;	/* ��ѹ��Сֵ2 */
	float v_ampl;	/* ��ѹ�ڶ�2 */
	unsigned char is_cntr;	/* �Ƿ�����2 */
	unsigned char is_disp;	/* �Ƿ���ʾ2 */
	float lfo_v_max;	/* lfo��ѹ���ֵ */
	float lfo_v_min;	/* lfo��ѹ��Сֵ */
	float lfo_v_ampl;	/* lfo��ѹ�ڶ� */
	unsigned char lfo_is_cntr;	/* lfo�Ƿ����� */
	unsigned char lfo_is_disp;	/* lfo�Ƿ���ʾ */
	unsigned char lfo_clus;	/* lfo����Ⱥ */
}LFO_FC_TAB_WRITE;
#define LFO_FC_TAB_WRITE_FLD "v_max2,v_min2,v_ampl2,is_cntr2,is_disp2,lfo_v_max2,lfo_v_min2,lfo_v_ampl2,lfo_is_cntr2,lfo_is_disp2,lfo_clus2"


// LFO�����߶α��ȡ�ṹ������wamtoolstruct.h�нṹLFO_LN_TAB_CRE��
// LFO�����߶α�д��ṹ
typedef struct type_LFO_LN_TAB_MONWRITE
{
	//int acln_idx;	/* �����߶����� */
	
	int stat;	/* ״̬ */
	int stat_val;	/* ״ֵ̬ */
	
	float lead_ampl;	/* ��� */
	float lead_freq;	/* Ƶ�� */
	float lead_damp;	/* ����� */
	float lead_phas;	/* ���� */
	float lead_real;	/* ʵ�� */
	float lead_imag;	/* �鲿 */
	float lead_engy;	/* ���� */
	float lead_fail;	/* ˥�� */
	float aver_val;	/* ��ֵ */
	float max_val;	/* ���ֵ */
	RTDB_TIME max_val_t;	/* ���ֵʱ�� */
	float min_val;	/* ��Сֵ */
	RTDB_TIME min_val_t;	/* ��Сֵʱ�� */
	float max_min_dif;	/* ���ڶ� */
// 	int if_lfo;	/* �Ƿ������ */
// 	
// 	int slfo_bgn_t;	/* ����LFO����ʱ�� */
// 	int slfo_end_t;	/* ����LFO��ʧʱ�� */
// 	char slfo_time[20];	/* ����ʱ�� */
// 	float slfo_bgn_val;	/* ����LFOǰ��ֵ */
// 	float slfo_end_val;	/* ����LFO���ֵ */
// 	float lfo_max_val;	/* LFO���ֵ */
// 	int lfo_max_val_t;	/* LFO���ֵʱ�� */
// 	float lfo_min_val;	/* LFO��Сֵ */
// 	int lfo_min_val_t;	/* LFO��Сֵʱ�� */
//  	float lfo_max_min_dif;	/* LFO���ڶ� */
	
// 	float max_ampl;	/* ����ֵ */
// 	float max_ampl_rat;	/* ��󲨶��� */
// 	int slct_dot_id;	/* ѡ���߶�ID */
// 	float fact;	/* �������� */
// 	int fact_sts;	/* ��������״̬ */
	int istie_source;	/* �Ƿ�����Ŷ�Դ2 */
	float lead_line_pow;	/* ��ɢ����2 */
	float fact;	/* ��������2 */
	char yc_name[64];	/* �������2 */
	char in_or_out[8];	/* ����������2 */

}LFO_LN_TAB_MONWRITE;
//#define LFO_LN_TAB_MONWRITE_FLD "stat,stat_val,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,slct_dot_id"
//#define LFO_LN_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,lfo_max_min_dif2,istie_source2,lead_line_pow2,fact2,yc_name2,in_or_out2"
#define LFO_LN_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,max_min_dif2,istie_source,lead_line_pow,fact,yc_name,in_or_out"

typedef struct type_LFO_LN_TAB_WRITE
{
	//int acln_idx;	/* �����߶����� */

// 	int stat;	/* ״̬ */
// 	int stat_val;	/* ״ֵ̬ */
// 
// 	float lead_ampl;	/* ���2 */
// 	float lead_freq;	/* Ƶ��2 */
// 	float lead_damp;	/* �����2 */
// 	float lead_phas;	/* ����2 */
// 	float lead_real;	/* ʵ��2 */
// 	float lead_imag;	/* �鲿2 */
// 	float lead_engy;	/* ����2 */
// 	float lead_fail;	/* ˥�� */
// 	float aver_val;	/* ��ֵ */
// 	float max_val;	/* ���ֵ */
// 	int max_val_t;	/* ���ֵʱ�� */
// 	float min_val;	/* ��Сֵ */
// 	int min_val_t;	/* ��Сֵʱ�� */
// 	float max_min_dif;	/* ���ڶ� */
	int if_lfo;	/* �Ƿ������ */

	RTDB_TIME slfo_bgn_t;	/* ����LFO����ʱ�� */
	RTDB_TIME slfo_end_t;	/* ����LFO��ʧʱ�� */
	char lfo_time[20];	/* ����ʱ�� */
	float slfo_bgn_val;	/* ����LFOǰ��ֵ */
	float slfo_end_val;	/* ����LFO���ֵ */
	float lfo_max_val;	/* LFO���ֵ */
	RTDB_TIME lfo_max_val_t;	/* LFO���ֵʱ�� */
	float lfo_min_val;	/* LFO��Сֵ */
	RTDB_TIME lfo_min_val_t;	/* LFO��Сֵʱ�� */
	float lfo_max_min_dif;	/* LFO���ڶ� */
// 	int istie_source;	/* �Ƿ�����Ŷ�Դ */
	int istie_source2;	/* �Ƿ�����Ŷ�Դ2 */
	float lead_line_pow;	/* ��ɢ����2 */
	int lfo_is_disp2;	/* lfo�Ƿ���ʾ2 */
//	int slct_dot_id;	/* ѡ���߶�ID */
// 	float fact;	/* ��������2 */
// 	int fact_sts;	/* ��������״̬2 */
// 	char yc_name[64];	/* ������� */
// 	char in_or_out[8];	/* ���������� */
}LFO_LN_TAB_WRITE;
//#define LFO_LN_TAB_WRITE_FLD "if_lfo,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif"
#define LFO_LN_TAB_WRITE_FLD "if_lfo2,slfo_bgn_t2,slfo_end_t2,lfo_time2,slfo_bgn_val2,slfo_end_val2,lfo_max_val2,lfo_max_val_t2,lfo_min_val2,lfo_min_val_t2,lfo_max_min_dif2,istie_source2,lead_line_pow,lfo_is_disp2"

// LFO��ѹ�����ȡ�ṹ������wamtoolstruct.h�нṹLFO_TR_TAB_CRE��
typedef struct type_LFO_TR_TAB_MONWRITE
{
	//int tr_idx;	/* ��ѹ������ */
	
// 	float fact;	/* �������� */
// 	int fact_sts;	/* ��������״̬ */
	int stat;	/* ״̬ */
	int stat_val;	/* ״ֵ̬ */
	
	float lead_ampl;	/* ��� */
	float lead_freq;	/* Ƶ�� */
	float lead_damp;	/* ����� */
	float lead_phas;	/* ���� */
	float lead_real;	/* ʵ�� */
	float lead_imag;	/* �鲿 */
	float lead_engy;	/* ���� */
 	float lead_fail;	/* ˥�� */
	float aver_val;	/* ��ֵ */
	float max_val;	/* ���ֵ */
	RTDB_TIME max_val_t;	/* ���ֵʱ�� */
	float min_val;	/* ��Сֵ */
	RTDB_TIME min_val_t;	/* ��Сֵʱ�� */
	float max_min_dif;	/* ���ڶ� */
// 	float max_ampl;	/* ����ֵ */
// 	float max_ampl_rat;	/* ��󲨶��� */
// 	unsigned char if_lfo;	/* �Ƿ������ */
// 	
// 	int slfo_bgn_t;	/* ����LFO����ʱ�� */
// 	int slfo_end_t;	/* ����LFO��ʧʱ�� */
// 	char slfo_time[20];	/* ����ʱ�� */
// 	float slfo_bgn_val;	/* ����LFOǰ��ֵ */
// 	float slfo_end_val;	/* ����LFO���ֵ */
// 	float lfo_max_val;	/* LFO���ֵ */
// 	int lfo_max_val_t;	/* LFO���ֵʱ�� */
// 	float lfo_min_val;	/* LFO��Сֵ */
// 	int lfo_min_val_t;	/* LFO��Сֵʱ�� */
//  	float lfo_max_min_dif;	/* LFO���ڶ� */
	int istie_source;	/* �Ƿ�����Ŷ�Դ2 */
	float lead_tr_pow;	/* ��ɢ����2 */
	float fact;	/* ��������2 */
	char yc_name[64];	/* �������2 */
	char in_or_out[8];	/* ����������2 */	
}LFO_TR_TAB_MONWRITE;
//#define LFO_TR_TAB_MONWRITE_FLD "stat,stat_val,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat"
//#define LFO_TR_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,lfo_max_min_dif2,istie_source2,lead_tr_pow2,fact2,yc_name2,in_or_out2"
#define LFO_TR_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy2,lead_fail2,aver_val2,max_val2,max_val_t2,min_val2,min_val_t2,max_min_dif2,istie_source,lead_tr_pow,fact,yc_name,in_or_out"

// LFO���������ȱ��ȡ�ṹ
typedef struct type_LFO_INT_TAB_MONWRITE
{
	int stat2;	/* ״̬2 */
	int stat_val2;	/* ״ֵ̬2 */
	float lead_ampl2;	/* ���2 */
	float lead_freq2;	/* Ƶ��2 */
	float lead_damp2;	/* �����2 */
	float lead_phas2;	/* ����2 */
	float lead_real2;	/* ʵ��2 */
	float lead_imag2;	/* �鲿2 */
	float lead_engy2;	/* ����2 */
	float lead_fail2;	/* ˥��2 */
	float aver_val2;	/* ��ֵ2 */
	unsigned char is_same_mod2;     /* �Ƿ�ͬ��2 */
// 	char intline_des2[256]; /* �����������2 */
	float max_min_dif2;	/* ���ڶ� */
}LFO_INT_TAB_MONWRITE;
#define LFO_INT_TAB_MONWRITE_FLD "stat2,stat_val2,lead_ampl2,lead_freq2,lead_damp2,lead_phas2,lead_real2,lead_imag2,lead_engy,lead_fail2,aver_val2,is_same_mod2,max_min_dif2"

// LFO��ѹ����д��ṹ
typedef struct type_LFO_TR_TAB_WRITE
{
	//int tr_idx;	/* ��ѹ������ */

// 	float fact;	/* �������� */
// 	int fact_sts;	/* ��������״̬ */
// 	int stat;	/* ״̬ */
// 	int stat_val;	/* ״ֵ̬ */

// 	float lead_ampl;	/* ���2 */
// 	float lead_freq;	/* Ƶ��2 */
// 	float lead_damp;	/* �����2 */
// 	float lead_phas;	/* ����2 */
// 	float lead_real;	/* ʵ��2 */
// 	float lead_imag;	/* �鲿2 */
// 	float lead_engy;	/* ����2 */
// 	float lead_fail;	/* ˥�� */
// 	float aver_val;	/* ��ֵ */
// 	float max_val;	/* ���ֵ */
// 	int max_val_t;	/* ���ֵʱ�� */
// 	float min_val;	/* ��Сֵ */
// 	int min_val_t;	/* ��Сֵʱ�� */
// 	float max_min_dif;	/* ���ڶ� */
// 	float max_ampl;	/* ����ֵ */
// 	float max_ampl_rat;	/* ��󲨶��� */
	unsigned char if_lfo;	/* �Ƿ������ */

	RTDB_TIME slfo_bgn_t;	/* ����LFO����ʱ�� */
	RTDB_TIME slfo_end_t;	/* ����LFO��ʧʱ�� */
	char lfo_time[20];	/* ����ʱ�� */
	float slfo_bgn_val;	/* ����LFOǰ��ֵ */
	float slfo_end_val;	/* ����LFO���ֵ */
	float lfo_max_val;	/* LFO���ֵ */
	RTDB_TIME lfo_max_val_t;	/* LFO���ֵʱ�� */
	float lfo_min_val;	/* LFO��Сֵ */
	RTDB_TIME lfo_min_val_t;	/* LFO��Сֵʱ�� */
	float lfo_max_min_dif;	/* LFO���ڶ� */

// 	int istie_source;	/* �Ƿ�����Ŷ�Դ */
	int istie_source2;	/* �Ƿ�����Ŷ�Դ2 */
	int lfo_is_disp2;	/* lfo�Ƿ���ʾ2 */
// 	float lead_tr_pow;	/* ��ɢ���� */
// 	float fact;	/* ��������2 */
// 	int fact_sts;	/* ��������״̬2 */
// 	char yc_name[64];	/* �������2 */
// 	char in_or_out[8];	/* ����������2 */
}LFO_TR_TAB_WRITE;
//#define LFO_TR_TAB_WRITE_FLD "fact,fact_sts,if_lfo,slfo_bgn_t,slfo_end_t,slfo_time,slfo_bgn_val,slfo_end_val,lfo_max_val,lfo_max_val_t,lfo_min_val,lfo_min_val_t,lfo_max_min_dif"
#define LFO_TR_TAB_WRITE_FLD "if_lfo2,slfo_bgn_t2,slfo_end_t2,lfo_time2,slfo_bgn_val2,slfo_end_val2,lfo_max_val2,lfo_max_val_t2,lfo_min_val2,lfo_min_val_t2,lfo_max_min_dif2,istie_source2,lfo_is_disp2"

// LFO���������ȱ�д��ṹ
typedef struct type_LFO_INF_TAB_WRITE
{
// 	float lead_ampl2;	/* ���2 */
// 	float lead_freq2;	/* Ƶ��2 */
// 	float lead_damp2;	/* �����2 */
// 	float lead_phas2;	/* ����2 */
// 	float lead_real2;	/* ʵ��2 */
// 	float lead_imag2;	/* �鲿2 */
// 	float lead_engy2;	/* ����2 */
// 	float lead_fail2;	/* ˥��2 */
// 	float aver_val2;	/* ��ֵ2 */
	int if_lfo2;	/* �Ƿ������2 */
	RTDB_TIME lfo_bgn_t2;	/* LFO����ʱ��2 */
	RTDB_TIME lfo_end_t2;	/* LFO��ʧʱ��2 */
	char lfo_time2[20];	/* ����ʱ��2 */
	float lfo_max_val2;	/* LFO���ֵ2 */
	RTDB_TIME lfo_max_val_t2;	/* LFO���ֵʱ��2 */
	float lfo_min_val2;	/* LFO��Сֵ2 */
	RTDB_TIME lfo_min_val_t2;	/* LFO��Сֵʱ��2 */
	char intline_des2[256]; /* �����������2 */
	unsigned char lfo_is_disp2;	/* �Ƿ���ʾ */
}LFO_INF_TAB_WRITE;
#define LFO_INF_TAB_WRITE_FLD "if_lfo2,lfo_bgn_t2,lfo_end_t2,lfo_time2,lfo_max_val2,lfo_max_val_t2,lfo_min_val2,lfo_min_val_t2,intline_des2,lfo_is_disp2"

// LFO���¼���д��ṹ
typedef struct type_LFO_CASE_INFO_WRITE
{
	INT_REC record_id;	/* ��¼ID */
	INT_REC eve_id;	/* �¼�ID */
	INT_REC win_id;	/* ���ں� */
	RTDB_TIME analy_bgn;	/* ������ʼʱ�� */
	RTDB_TIME analy_end;	/* ������ֹʱ�� */
	float freq_max;	/* Ƶ������ */
	float freq_min;	/* Ƶ������ */
	int stat;	/* ϵͳ״̬ */
	int show_gn_num;	/* �񵴷�������� */
	int show_tr_num;	/* �񵴱�ѹ������ */
	int show_ln_num;	/* ����·���� */
	float freq;	/* ����ģʽƵ�� */
	float ampl;	/* ��� */
	float damp;	/* ����� */
	int dev_num;	/* �����豸���� */
	int is_show;	/* �Ƿ���ʾ */
	char case_des[128];	/* �񵴰������� */
}LFO_CASE_INFO_WRITE;


// LFOʵʱCASE���ڱ�д��ṹ
typedef struct type_LFO_WINDOWS_WRITE
{
// 	int wds_id;	/* ����ID */
// 
// 	int eve_id;	/* �¼�ID */
// 	int win_id;	/* ���ں� */
// 	int analy_bgn;	/* ������ʼʱ�� */
// 	int analy_end;	/* ������ֹʱ�� */
// 	int is_show;	/* �Ƿ���ʾ */
// 
// 	int lfodev_id;	/* ���豸ID */
// 	int fac_id;	/* ��վID */
// 	int dev_type;	/* �豸���� */
// 	float freq;	/* Ƶ�� */
// 	float ampl;	/* ��� */
// 	float damp;	/* ����� */
// 	float aver_val;	/* ƽ��ֵ */
// 	float max_ampl_rat;	/* ��󲨶��� */
	INT_REC wds_id;	/* ����ID */
	int wds_no;	/* ������� */
	RTDB_TIME wds_bgn;	/* ��ʼʱ�� */
	RTDB_TIME wds_end;	/* ����ʱ�� */
	int mode_num;	/* ģʽ�� */
	float real1;	/* ʵ��1 */
	float imag1;	/* �鲿1 */
	float freq1;	/* Ƶ��1 */
	float damp1;	/* �����1 */
	float real2;	/* ʵ��2 */
	float imag2;	/* �鲿2 */
	float freq2;	/* Ƶ��2 */
	float damp2;	/* �����2 */
	float real3;	/* ʵ��3 */
	float imag3;	/* �鲿3 */
	float freq3;	/* Ƶ��3 */
	float damp3;	/* �����3 */
	int is_mode_show;	/* ģʽ�Ƿ���ʾ */
	RTDB_TIME occur_time;	/* ����ʱ�� */
	char content[255];	/* ��ϸ��Ϣ���� */
	int is_ctx_show;	/* ��Ϣ�Ƿ���ʾ */
	char center_desc[128];	/* ���������� */
	char lastt[64];	/* ����ʱ�� */
	float freq_max;	/* Ƶ������ */
	float freq_min;	/* Ƶ������ */
	INT_REC eve_id;	/* �¼�ID */
	INT_REC win_id;	/* ���ں� */
	RTDB_TIME analy_bgn;	/* ������ʼʱ�� */
	RTDB_TIME analy_end;	/* ������ֹʱ�� */
	int is_show;	/* �Ƿ���ʾ */
	float rphas;	/* ���� */
	INT_REC lfodev_id;	/* ���豸ID */
	INT_REC fac_id;	/* ��վID */
	int dev_type;	/* �豸���� */
	float freq;	/* Ƶ�� */
	float ampl;	/* ��� */
	float damp;	/* ����� */
	float aver_val;	/* ƽ��ֵ */
	float max_ampl_rat;	/* ��󲨶��� */
}LFO_WINDOWS_WRITE;

// LFOʵʱCASE���ڱ�д��ṹ
typedef struct type_POW_WDS_WRITE
{
	INT_REC wds_id;	/* ����ID */
	int wds_no;	/* ������� */
	RTDB_TIME wds_bgn;	/* ��ʼʱ�� */
	RTDB_TIME wds_end;	/* ����ʱ�� */
	int mode_num;	/* ģʽ�� */
	float real1;	/* ʵ��1 */
	float imag1;	/* �鲿1 */
	float freq1;	/* Ƶ��1 */
	float damp1;	/* �����1 */
	float real2;	/* ʵ��2 */
	float imag2;	/* �鲿2 */
	float freq2;	/* Ƶ��2 */
	float damp2;	/* �����2 */
	float real3;	/* ʵ��3 */
	float imag3;	/* �鲿3 */
	float freq3;	/* Ƶ��3 */
	float damp3;	/* �����3 */
	int is_mode_show;	/* ģʽ�Ƿ���ʾ */
	RTDB_TIME occur_time;	/* ����ʱ�� */
	char content[255];	/* ��ϸ��Ϣ���� */
	int is_ctx_show;	/* ��Ϣ�Ƿ���ʾ */
	char center_desc[128];	/* ���������� */
	char lastt[64];	/* ����ʱ�� */
	float freq_max;	/* Ƶ������ */
	float freq_min;	/* Ƶ������ */
	INT_REC eve_id;	/* �¼�ID */
	INT_REC win_id;	/* ���ں� */
	RTDB_TIME analy_bgn;	/* ������ʼʱ�� */
	RTDB_TIME analy_end;	/* ������ֹʱ�� */
	int is_show;	/* �Ƿ���ʾ */
	float rphas;	/* ���� */
	INT_REC lfodev_id;	/* ���豸ID */
	INT_REC fac_id;	/* ��վID */
	int dev_type;	/* �豸���� */
	float freq;	/* Ƶ�� */
	float ampl;	/* ��� */
	float damp;	/* ����� */
	float aver_val;	/* ƽ��ֵ */
	float max_ampl_rat;	/* ��󲨶��� */
	char lfo_time[64];	/* ����ʱ�� */
	float lfo_max_val;	/* LFO���ֵ */
	RTDB_TIME lfo_max_val_t;	/* LFO���ֵʱ�� */
	float lfo_min_val;	/* LFO��Сֵ */
	RTDB_TIME lfo_min_val_t;	/* LFO��Сֵʱ�� */
	char lfodev_name[64];	/* ���豸���� */
	char fac_name[64];	/* ��վ���� */
}POW_WDS_WRITE;
// LFOʵʱCASEģ̬ȫ��д��ṹ
typedef struct type_LFO_MODESHAPE_WRITE
{
	INT_REC record_id;	/* ��¼�� */

	float rreall;	/* ����ֵʵ�� */
	float rimag;	/* ����ֵ�鲿 */
	float rmgnt;	/* ����ֵ��ֵ */
	float rphas;	/* ����ֵ��� */
	float fact;	/* �������� */
	int fact_sts2;	/* ��������״̬ */
	int dev_id;	/* �豸ID */
	char dev_name[64];	/* �豸���� */
	int dev_type;	/* �豸���� */
	int fac_id;	/* ������վ */
	int area_id;	/* �������� */

	unsigned char clus;	/* ����Ⱥ */
	float freq;	/* Ƶ�� */
	float damp;	/* ����� */
	float ampl;	/* ��� */
	float engy;	/* ���� */
	float aver_val;	/* ƽ��ֵ */
	float max_ampl;	/* ����ֵ */
	float max_ampl_rat;	/* ��󲨶��� */
	char last[64];	/* ����ʱ�� */
	float freq_max;	/* Ƶ������ */
	float freq_min;	/* Ƶ������ */
	INT_REC eve_id;	/* �¼�ID */
	INT_REC win_id;	/* ���ں� */
	RTDB_TIME analy_bgn;	/* ������ʼʱ�� */
	RTDB_TIME analy_end;	/* ������ֹʱ�� */
	int is_show;	/* �Ƿ���ʾ */
	float iniphas;	/* ���� */
}LFO_MODESHAPE_WRITE;

//////////////////////////////////////////////////////////////////////////
typedef struct tab_lfo_rt_curve1
{
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
	float seq_cuv_data[125];	/* seq_cuv_data���� */
}TAB_LFO_RT_CURVE1;
#define TAB_LFO_RT_CURVE1_FLD "dev_name,unit,sam_freq,cuv_bgn_sec,cuv_end_sec,cuv_sec,cuv_pnt,cuv_stp,max_val,min_val,ave_val,seq001,seq002,seq003,seq004,seq005,seq006,seq007,seq008,seq009,seq010,seq011,seq012,seq013,seq014,seq015,seq016,seq017,seq018,seq019,seq020,seq021,seq022,seq023,seq024,seq025,seq026,seq027,seq028,seq029,seq030,seq031,seq032,seq033,seq034,seq035,seq036,seq037,seq038,seq039,seq040,seq041,seq042,seq043,seq044,seq045,seq046,seq047,seq048,seq049,seq050,seq051,seq052,seq053,seq054,seq055,seq056,seq057,seq058,seq059,seq060,seq061,seq062,seq063,seq064,seq065,seq066,seq067,seq068,seq069,seq070,seq071,seq072,seq073,seq074,seq075,seq076,seq077,seq078,seq079,seq080,seq081,seq082,seq083,seq084,seq085,seq086,seq087,seq088,seq089,seq090,seq091,seq092,seq093,seq094,seq095,seq096,seq097,seq098,seq099,seq100,seq101,seq102,seq103,seq104,seq105,seq106,seq107,seq108,seq109,seq110,seq111,seq112,seq113,seq114,seq115,seq116,seq117,seq118,seq119,seq120,seq121,seq122,seq123,seq124,seq125"

typedef struct tab_lfo_rt_curve2
{
	float seq_cuv_data[125];	/* seq_cuv_data���� */
}TAB_LFO_RT_CURVE2;
#define TAB_LFO_RT_CURVE2_FLD "seq126,seq127,seq128,seq129,seq130,seq131,seq132,seq133,seq134,seq135,seq136,seq137,seq138,seq139,seq140,seq141,seq142,seq143,seq144,seq145,seq146,seq147,seq148,seq149,seq150,seq151,seq152,seq153,seq154,seq155,seq156,seq157,seq158,seq159,seq160,seq161,seq162,seq163,seq164,seq165,seq166,seq167,seq168,seq169,seq170,seq171,seq172,seq173,seq174,seq175,seq176,seq177,seq178,seq179,seq180,seq181,seq182,seq183,seq184,seq185,seq186,seq187,seq188,seq189,seq190,seq191,seq192,seq193,seq194,seq195,seq196,seq197,seq198,seq199,seq200,seq201,seq202,seq203,seq204,seq205,seq206,seq207,seq208,seq209,seq210,seq211,seq212,seq213,seq214,seq215,seq216,seq217,seq218,seq219,seq220,seq221,seq222,seq223,seq224,seq225,seq226,seq227,seq228,seq229,seq230,seq231,seq232,seq233,seq234,seq235,seq236,seq237,seq238,seq239,seq240,seq241,seq242,seq243,seq244,seq245,seq246,seq247,seq248,seq249,seq250"

typedef struct tab_lfo_rt_curve_id
{
	INT_REC dev_idx;	/* �豸����ID */
	INT_REC dev_id;	/* ����ID */
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
}VEC_YC_FCNO;                                               //  ң�������ļ����к�
// ��ֵʱ��
typedef struct  tag_VALUE_TIME_STRU
{
    float           value;                                  //  ��ֵ
    //struct timeval	time;                                   //  ʱ��
    time_t       	time;                                    //  ʱ��
}VALUE_TIME_STRU;


/////////////////////////////////// ������Ϣ //////////////////////////////
// ���龲̬��Ϣ
typedef struct tag_LFO_GN_INF
{
    INT_REC         gn_idx;                                 //  ��������
	char            gn_name[64];                            //  ��������
    INT_REC         gn_id;                                  //  ����ID��
    INT_REC         fac_id;                                 //  ��վID��
    char            fac_name[64];                           //  ��վ����
	INT_REC         area_id;	                            //  ����ID��
	char            area_name[64];	                        //  ��������
	float           snom;                                   //  ����ʣ�MVA��
    float           pnom;                                   //  �������MW��
	float           pmin;	                                //  �й�����
	float           pmax;	                                //  �й�����
	float           qmin;	                                //  �޹�����
	float           qmax;	                                //  �޹�����
	float           tj;	                                    //  ����ʱ�䳣��
    float           emws;                                   //  ���鶯�ܣ�MWo�룩tj*snom
} LFO_GN_INF;

// ���������Ϣ
typedef struct tag_LFO_GN_CTL
{
    INT_REC         gn_id;                                  //  ����ID��
	unsigned char   is_monitor;                             //  �Ƿ����
	unsigned char   is_part_ana;                            //  �Ƿ�������
	unsigned char   is_agl_calc;                            //  ���㹦������
	float			lfo_gn_swn;								//	��Ƶ�񵴿�ʼ�����ֵ
	float			lfo_gn_ewn;								//	��Ƶ�񵴽��������ֵ
} LFO_GN_CTL;
#define LFO_GN_CTL_FLD "gn_id,is_monitor,is_part_ana,is_agl_calc,lfo_gn_swn,lfo_gn_ewn"

// ���������
typedef struct tag_LFO_GN_DAT
{
    INT_REC         gn_id;                                  //  ����ID��
	float           ang[DATA_NUM_MAX];                      //  ����
	float           pwr[DATA_NUM_MAX];                      //  �й�
    float           u1a[DATA_NUM_MAX];                      //  ��ѹ���
	
	int             SecSts[MAX_tIMW_WIN];                   //  ÿ������״̬
	int             stat;                                   //  ��������״̬(�����ſ���)
    VEC_YC_FCNO     VecBadData[3];                             //  ���ⲻ�����ļ���  // 2008-07-16
	
	int             is_calc;                                //  �Ƿ���Ҫ���� 08-02-02
	VALUE_TIME_STRU max_val;                                //  ���ֵ
	VALUE_TIME_STRU min_val;                                //  ��Сֵ
	float           ave_val;                                //  ��ֵ
    float           max_ampl;                               //  ���ڶ�
	float           max_ampl_rat;                           //  ��󲨶���(������ֵ/��ֵ)
	float           max_ampl_ran;							//  ��󲨶�����(������ֵ/���)
}LFO_GN_DAT;

// ���������Ϣ
typedef struct tag_LFO_GN_MON
{
    INT_REC         gn_id;                                  //  ����ID��
	int             stat;                                   //  ״̬
	int             stat_val;                               //  ״ֵ̬
	INT_REC         damp_grad;                              //  ����ȼ�
	int             damp_grad_val;                          //  ����ȼ�ֵ
	INT_REC         mode_rcd_id;                            //  ���豸ģʽȫ�����׼�¼ID��
	int             mode_rcd_num;	                        //  ���豸ģʽȫ���еļ�¼����
	float           lead_ampl;                              //  ���
	float           lead_freq;                              //  Ƶ��
	float           lead_damp;                              //  �����
	float           lead_phas;                              //  ����
	float           lead_real;                              //  ʵ��
	float           lead_imag;                              //  �鲿
	float           lead_engy;                              //  ����
	float           lead_fail;                              //  ˥��
	float           aver_val;                               //  ��ֵ
	float           max_val;                                //  ���ֵ
	RTDB_TIME         max_val_t;                              //  ���ֵʱ��
	float           min_val;                                //  ��Сֵ
	RTDB_TIME         min_val_t;                              //  ��Сֵʱ��
	float           max_min_dif;                            //  ���ڶ������ֵ������
    float           max_ampl;                               //  ����ֵ
	float           max_ampl_rat;                           //  ��󲨶���(������ֵ/��ֵ)
    unsigned char   is_ang_calc;                            //  �����Ƿ����ֵ  2008-05-22 by zhw
} LFO_GN_MON;
#define LFO_GN_MON_FLD "gn_id,stat,stat_val,damp_grad,damp_grad_val,mode_rcd_id,mode_rcd_num,lead_ampl,lead_freq,lead_damp,lead_phas,lead_real,lead_imag,lead_engy,lead_fail,aver_val,max_val,max_val_t,min_val,min_val_t,max_min_dif,max_ampl,max_ampl_rat,is_ang_calc"

// ����ң����Ϣ
typedef struct tag_LFO_GN_PMU
{
    INT_REC         gn_id;                                  //  ����ID��
	char            gn_name[64];                            //  ��������// 2008-01-08
    VEC_YC_FCNO     vecPwr;                                 //  �����й�
	
    VEC_YC_FCNO     vecAng;                                 //  ���鹦��
    VEC_YC_FCNO     vecAnc;                                 //  ������㹦��
    VEC_YC_FCNO     vecEv;                                  //  �����ڵ���
    VEC_YC_FCNO     vecEa;                                  //  �����ڵ������
    unsigned char   curSts;                                 //  ��ǰ״̬
    unsigned char   hisSts;                                 //  ��ʷ״̬
	unsigned char   is_agl_calc;                            //  ���㹦������
    int             ang_flag;                               //  1: ���ǲ���ֵ����  2: ���Ǽ���ֵ����  3: ���ǲ���ֵ�ͼ���ֵ������  //2008-05-29 by zhw
    unsigned char   is_ang_calc;                            //  �����Ƿ����ֵ  2008-05-21 by zhw at fj
} LFO_GN_PMU;

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
	float           max_ampl_ran;   //  ��󲨶�����(������ֵ/���)
} LFO_FC_DAT;
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
typedef struct type_Gn_warn_time
{
	time_t m_GnStartTime; 
	time_t m_GnEndTime; 
}TYPE_GN_WARN_TIME;

typedef struct  tag_CURVE_DATA
{
	float           t_GPS;                                  //  ��վID
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
	int         m_nStatus;    // 0 δ���� 1 ���ڴ����� 2 �������
	int         m_time;       // �����ʱ�䳤��,����
	CURVE_DATA  m_curve;      // ��������
	MODE_RESULT m_result;     // ������
	int         m_nClusterID;
	int			m_type;
	char		m_name[64]; 
	struct timeb timeStart;  // ��ʼ����ʱ��
	struct timeb timeEnd;    // ��������ʱ��
	
}CASE_TYPE;
// Prony�������	
typedef struct tag_PRONY_ANAL_RSLT
{
	INT_REC  obj_id;               //���Ӷ���ID��(�ٽ�ȺID��Ϊ��)
	char     obj_name[64];        //���Ӷ�������(200701)
    int      mode_num;             //��ģʽ����
    float    mode[MODE_NUM_MAX][8];//��ģʽ   (����Ԥ���ֵ)
}PRONY_ANAL_RSLT;

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
// �ƶ�ƽ̨�ۺ����ܸ澯���豸��Ϣ
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
// �ƶ�ƽ̨�ۺ����ܸ澯�ṹ��
typedef struct type_LOW_FRE_OSCI_INFO_TO_IA_STRU
{
	INT_REC search_id;  // ������(�豸ID)
	time_t occur_time;  // ����ʱ��
	short msec;  // ����
	RTDB_TIME end_time;  // ����ʱ��
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


#endif
