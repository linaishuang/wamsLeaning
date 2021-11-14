//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	�ļ���:	wrdb_client_api.h

	������:	�Ϲ� Copyright (c) 2021 �����ķ��̱��Զ����ɷ����޹�˾

	��  ��:	2021-03-18

	��  ��:	WAMSʱ��ʵʱ��API�ӿ����ݽṹ����������

	��  ��:	1.0
			
	�޸���: 

	��  ��: 

	��  ��: WRDBΪ Wams Realtime Database ����д
*************************************************************************/
//////////////////////////////////////////////////////////////////////////

#ifndef __WRDB_API_H_
#define __WRDB_API_H_

#ifdef _WIN32
#ifdef WRDB_API_EXPORTS
#define WRDB_DLL_ENTRY __declspec(dllexport)
#else
#define WRDB_DLL_ENTRY __declspec(dllimport)
#endif
#else
#define WRDB_DLL_ENTRY
#endif

#if defined (__x86_64)||(__x86_64__)||(__amd64)||(__x86_64__)||(_M_X64)
	#ifndef __int64
		#define __int64 long
	#endif
#else 
	#ifndef __int64
		#define __int64 long long
	#endif
#endif//#if defined (__x86_64)||(__x86_64__)||(__amd64)||(__x86_64__)||(_M_X64)

#ifdef __cplusplus
extern "C" { 
#endif

#include "stdlib.h"
#include "string.h"

	//���ݵ����ö���
	typedef struct _WRDB_RecordConfig
	{
		int iRecordID;			// ����ID(-1Ϊ��Ч),��IDҲ��Ϊ��recordArr���±ꡣ
		int iDatasetID;			// �������ݼ���ID
		int iIndex;				// ��ѹ���ļ��е�����λ��
		int iType;				// �������ͣ���ѭWAMSʵʱ���ݿ���������Ͷ���)
		__int64 recv_ana_id;	// ��Ӧd5000����������ID��
		//char codeStr[32];		// WAMSʱ�����п���Ӣ�ı�ʶ
		char szDesc[64];		// ��������
		char szTagName[32];		// ��վ����ǩ�� [11/16/2010 xiexiaodong]
	} WRDB_RecordConfig;


	//���ݿ����ģʽ
	enum  ACCESS_MODE_WRDB	 
	{
		ACCESS_MODE_READONLY_WRDB			=	0,		//ֻ��������ֻ��ģʽ���ɷ���ȫ����������ݼ���
		ACCESS_MODE_WRITEONLY_WRDB			=	1,		//ֻд������ֻдģʽ��ֻ�ܷ���ָ�������ݼ���
		ACCESS_MODE_WRITEANDREAD_WRDB		=	2,		//��д�����ڶ�дģʽ��ֻ�ܷ���ָ�������ݼ���
	};


	//ʱ��ʵʱ���������
	struct  WRDB_DESCR
	{
		void*   lpHandle;				//���ʾ��
		int		iMode;					//���ʱ�־
		WRDB_DESCR()
		{
			lpHandle = NULL;
			iMode = -1;
		}
	};

	//WAMSʱ�����п�ʱ��ṹ���壨��ʵʱ���е�WRDB_Time���ݣ�����ǿ��ת����
	struct WRDB_Time
	{
		int secs;
		int usecs;
	};

	//WAMSʱ�����п�����ֵ�㶨�壨��ʵʱ���е�WRDB_Value�ȼۣ�����ǿ��ת����
	struct WRDB_Value
	{
		WRDB_Time time;
		float value;
		char status;
	};




/*********************************************************************
��  �ܣ�	��ʱ��ʵʱ��ӿ�
��  ����	descr:		ʱ��ʵʱ����ʶ����ָ��
flag:		����ģʽ��ֻ����ֻд����д
szDomain:	������������
����ֵ��	0 : �����ɹ�
		-1: ����ʧ��
��  ����	�ڷ���ʱ��ʵʱ��֮ǰ���е���
			server_ip:���������IP�����ʱ�����дNULL����
			descr	: ������������ָ��
			flag	: ��ѯģʽ
			szDomain: ������������δ���ã�
					: NULL		��ʾ���ʱ�����
					: "��ϵͳ"	��ʾ������
					: "XXXXX"	��ʾ�����������
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Open(char * server_ip, WRDB_DESCR *descr, int flag, const char* szDomain=0);

/*********************************************************************
��  �ܣ�	�ر�ʱ��ʵʱ��ӿ�
��  ����	pDescr:		ʱ��ʵʱ����ʶ����ָ��	
����ֵ��	��
��  ����	�ڽ�������ʱ��ʵʱ������
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Close(WRDB_DESCR *pDescr);


/*********************************************************************
��  �ܣ�	���ʱ��ʵʱ��ӿڵ�״̬
��  ����	pDescr:		ʱ��ʵʱ����ʶ����ָ��	
����ֵ��	
			0 : �ر�
			1 : ��
��  ����	�ڷ���ʱ��ʵʱ������֮ǰ���е���
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_IsOpen(WRDB_DESCR *pDescr);


/*********************************************************************
��  �ܣ�	д�����ݼ�¼
��  ����	descr:		ʱ��ʵʱ����ʶ����ָ��	
iRecordID:	����iID
iBeginSOC:	���ݵĿ�ʼʱ��(��)
iBeginUSEC:	���ݵĿ�ʼʱ��(΢��)
iEndSOC:	���ݵĽ���ʱ��(��)
iEndUSEC:	���ݵĽ���ʱ��(΢��)
pBuffer:	���ݴ洢����
iLength:	���ݳ���
pFlag:		���ݱ�־�洢����
iFlagLength:���ݱ�־�洢���򳤶�
szDomain	:	��������
			:	NULL		��ʾ���ʱ�����
			:	"��ϵͳ"	��ʾ������
			:	"XXXXX"	��ʾ�����������

����ֵ��	0	:�ɹ�
			-1	:ʧ��
��  ����	�ڴ򿪷��ʽӿں�,���Ե���.
			ʱ������Ϊǰ�պ�
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Write(	WRDB_DESCR *descr,
							  int iRecordID,			
							  int iBeginSOC,		
							  int iEndSOC,
							  int iInterval,
							  const char* pBuffer,
							  int iLength,
							  const char* pFlag, 
							  int iFlagLength,
							  const char* szDomain = 0);



/*********************************************************************
��  �ܣ�	��ȡ���ݼ�¼
��  ����	descr:		ʱ��ʵʱ����ʶ����ָ��	
			iRecordID:	����iID
			iBeginSOC:	���ݵĿ�ʼʱ��(��)
			iEndSOC:	���ݵĽ���ʱ��(��)
			iInterval:	��ѯ���ݵ�ʱ������΢�룩
			pBuffer:	���ݴ洢��������ֵ��float�ͣ���ʱ��˳������
			iLength:	���ݴ洢���򳤶ȣ�����-�洢���򳤶ȣ� ����-ʵ����Ч���ݵĳ��ȣ�
			pFlag:		���ݱ�־�洢��������ֵ��char�ͣ���ʱ��˳������
			iFlagLength:���ݱ�־�洢���򳤶� ����-�洢���򳤶ȣ� ����-ʵ����Ч�����ĳ��ȣ�
			szDomain	:	��������
						:	NULL		��ʾ���ʱ�����
						:	"��ϵͳ"	��ʾ������
						:	"XXXXX"	��ʾ�����������

����ֵ��	0	:�ɹ�
			-1	:ʧ�ܣ�ע�⣬�������洢���򳤶Ȳ��㣬����-1��
��  ����	�ڴ򿪷��ʽӿں�,���Ե���.
			��ѯʱ������Ϊǰ�պ�,
			
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Read(	WRDB_DESCR *descr,
							 int iRecordID,			//����iID	 
							 int iBeginSOC,			//��ʼʱ��(��)
							 int iEndSOC,			//����ʱ��(��)
							 WRDB_Value* pValue,	//�����������
							 int* iCount,			//������ݸ���
							 int iInterval = 20000,	//��ѯ���ݵ�ʱ������΢�룩
							 const char* szDomain = 0);



/*********************************************************************
��  �ܣ�	д�����ݼ�¼
��  ����	pDescr:		ʱ��ʵʱ����ʶ����ָ��	
iRecordID:	����iID
pValue:		д�����ݻ�����
iCount:		д�����ݵĸ���
����ֵ��	0	:�ɹ�
-1	:ʧ��
��  ����	�ڴ򿪷��ʽӿں�,���Ե���.
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_WriteValue(	WRDB_DESCR *pDescr,
								   int iRecordID,			
								   const WRDB_Value* pValue,
								   const int& iCount,
								   const char* szDomain = 0);



/*********************************************************************
��  �ܣ�	��ȡ���ݼ�¼
��  ����	pDescr		:	ʱ��ʵʱ����ʶ����ָ��	
			iRecordID	:	����iID
			iBeginSOC	:	���ݵĿ�ʼʱ��(��)
			iEndSOC		:	���ݵĽ���ʱ��(��)
			iInterval	:	��ѯ���ݵ�ʱ������΢�룩
			pValue		:	��ȡ���ݵĻ�����
			iCount		:	��ȡ���ݵĸ���
			szDomain	:	��������
						:	NULL		��ʾ���ʱ�����
						:	"��ϵͳ"	��ʾ������
						:	"XXXXX"	��ʾ�����������

����ֵ��	0	:�ɹ�
			-1	:ʧ�ܣ�ע�⣬�������洢���򳤶Ȳ��㣬����-1��
��  ����	�ڴ򿪷��ʽӿں�,���Ե���.
			1��ʱ������Ϊǰ�պ�,
			2���ӿڻ�Դ���Ĳ������м�飬������ݴ洢�����Ȳ��ܱ���ȫ����ѯ�������ݣ����������᷵��ʧ�ܡ�
			3) �紫���pValue=NULL�������ӿڻᰴ�ռ���ĳ��ȷ��仺����������iCount�з��س��ȡ�
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_ReadValue(	WRDB_DESCR *pDescr,
							  int iRecordID,
							  int iBeginSOC,
							  int iEndSOC,
							  int iInterval,
							  WRDB_Value* pValue,
							  int& iCount,
							  const char* szDomain = 0);



/*********************************************************************
��  �ܣ�	�����¼
��  ����	pDescr		:	ʱ��ʵʱ����ʶ����ָ��	
			pRecordConfig:	���ݼ�¼��ָ��
����ֵ��	0 ���ɹ�
			-1������
��  ����	�����¼�Ľṹ�У�����Ҫ���õ��ֶ��У�
			1��RecordID		��¼ID�ţ������ڶ������Ч��Χ��ȡֵ��
			2��codeStr		���Ӣ�ı�ʶ������ʶ�ڿ���ӦΪΨһ������
			3��szDesc		��������
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Insert(WRDB_DESCR *pDescr,
							   const WRDB_RecordConfig* pRecordConfig,
							   const char* szDomain = 0);		


/*********************************************************************
��  �ܣ�	ɾ����¼
��  ����	pDescr:			ʱ��ʵʱ����ʶ����ָ��	
			iRecordID: ��ɾ�����ݼ�¼��ID��
����ֵ��	0���ɹ�
			-1������
��  ����	
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Delete(WRDB_DESCR *pDescr,
							   int iRecordID,
							   const char* szDomain = 0);		


/*********************************************************************
��  �ܣ�	��ȡ����������
��  ����	pDescr:			ʱ��ʵʱ����ʶ����ָ��	

����ֵ��	�������
��  ����	�ڶ�д����ʧ�ܺ����
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_Error(WRDB_DESCR *pDescr);		

/*********************************************************************
��  �ܣ�	��ȡ�����������
��  ����	pDescr:			ʱ��ʵʱ����ʶ����ָ��	
����ֵ��	��������
��  ����	�ڶ�д����ʧ�ܺ����
*********************************************************************/
WRDB_DLL_ENTRY const char* WRDB_StrError(WRDB_DESCR *pDescr);		


/*********************************************************************
��  �ܣ�	��ȡ����˵����ݼ�ID
��  ����	pDescr:	 ��ʷ����ʶ����ָ��	
����ֵ��	���Ի�����˵����ݼ�ID��
��  ����	����������ֻд�Ͷ�дģʽ��Ч����д����ֻ��д��ָ�������ݼ��С�
			����ֻ��ģʽ�����壬����ֻ��ģʽ�£�������ܹ����ʵ�ȫ�������ݼ�������֪������˹������ݼ���ID��
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_GetDatasetID(WRDB_DESCR *pDescr);

/*********************************************************************
��  �ܣ�	��ȡ��¼�б�
��  ����	pDescr:		in	��ʷ����ʶ����ָ��	
			pConfig:	out	���������Ϣ����ָ��
			iCount:		in  ���������Ϣ����ά��
						out ����������õ�����
����ֵ��	0	���ɹ�
			-1	��ʧ��
��  ����	
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_GetRecordList(WRDB_DESCR *pDescr,
									  WRDB_RecordConfig* pConfig,
									  int& iCount,
									  const char* szDomain = 0);



/*********************************************************************
��  �ܣ�	��ȡ��ϵͳ����
��  ����	pDescr:		in	ʱ��ʵʱ����ʶ����ָ��	
			szDomain:	out	����ϵͳ����
����ֵ��	0	���ɹ�
			-1	��ʧ��
��  ����	
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_LocalDomainName(WRDB_DESCR *pDescr,char* szDomain);


/*********************************************************************
��  �ܣ�	��ȡʱ��������¼�
��  ����	pDescr:		in	ʱ��ʵʱ����ʶ����ָ��	
		latest_time:	out	ʱ�������ʱ��
����ֵ��	0	���ɹ�
		-1	��ʧ��
��  ����	
*********************************************************************/
WRDB_DLL_ENTRY int WRDB_ReadLastTime(WRDB_DESCR *pDescr, WRDB_Time & latest_time);


#ifdef __cplusplus
}
#endif

#endif // !defined(AFX_WRDB_H__0B25F7A1_D615_4294_968D_D11C8B475E9B__INCLUDED_)
