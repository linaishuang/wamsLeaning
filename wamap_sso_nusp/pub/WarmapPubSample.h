/******************************************************************************
    模  块:  
    说  明:  Warmap结果采样
    文件名:  WarmapPubSample.h

    时间          版本       作者           修改内容
  ----------------------------------------------------------------------
    2009/10/17    V1.0       zhf            初始版
******************************************************************************/

#ifndef _WARMAP_PUB_SAMPLE_INC_
#define _WARMAP_PUB_SAMPLE_INC_

#ifdef _WAMAP_
#ifdef _D5000
   //#include "trigger_sample_m.h"
   #include "msg_api/message_inv.h"
   #include "msg_api/messageheader.h" 
   #include "message_channel.h"
//消息类型
   #include "message_type.h"
   #include "db_com/trigger_sample_m.h"
   #include "db_com/trigger_sample_service_interface.h"
#else
   #include "db_com/trigger_sample.h"
#endif
static int s_nAppNo;
static int s_nRecIdx;
static int s_nItemIdx;
static int s_nTableNo;
static int s_nBufSize;
#ifndef _D5000
static char * s_pMsgBuffer;
static char   s_szHostName[64];
static TSEQSampleProcessRequest  s_tSampleProcess;
static TSampleProcessRequest  *  s_pSampleRequest;

#else
static   Message msg;
static   char *mes_content;
static   int mes_len;
static   SEQSampleProcessRequest in_para;
static   trigger_sample_service_interface tri_client;
#endif

//////////////////////////////////////////////////////////////////////////
#ifndef _D5000
// #define Sample_Multi_Init(nAppNo, nTabNo, nType, nItemNum, nOccurTime, v) \
//     s_nAppNo = nAppNo; \
//     s_tSampleProcess.seq_length = v.size(); \
//     s_tSampleProcess.seq_sample_process_request.length(v.size()); \
//     for(s_nRecIdx=0; s_nRecIdx<v.size(); s_nRecIdx++) \
//     { \
//         s_pSampleRequest = &s_tSampleProcess.seq_sample_process_request[s_nRecIdx]; \
//         s_nItemIdx = -1; \
//         s_nTableNo = nTabNo; \
//         s_pSampleRequest->app_no      = nAppNo; \
//         s_pSampleRequest->sample_type = nType; \
//         s_pSampleRequest->sample_mode = OFFERDATA_SAMPLE_MODE; \
//         s_pSampleRequest->occur_time  = nOccurTime; \
//         s_pSampleRequest->flag        = 0; \
//         s_pSampleRequest->reserve_1   = 0; \
//         s_pSampleRequest->reserve_2   = 0; \
//         s_pSampleRequest->keyid_num   = nItemNum; \
//         s_pSampleRequest->seq_keyid_value.length(nItemNum);
// 
// #define SAMPLE_MULTI_ITEM_INT(nColId, val) \
//         s_nItemIdx++;\
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.record_id = TABLE_MULT * s_nTableNo; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.column_id = nColId; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_datatype  = SAMPLE_DATATYPE_INT; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_value.c_int(val); 
// 
// #define SAMPLE_MULTI_ITEM_FLT(nColId, val) \
//         s_nItemIdx++;\
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.record_id = TABLE_MULT * s_nTableNo; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.column_id = nColId; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_datatype  = SAMPLE_DATATYPE_FLOAT; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_value.c_float(val); 
// 
// #define SAMPLE_MULTI_ITEM_STR(nColId, val) \
//         s_nItemIdx++;\
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.record_id = TABLE_MULT * s_nTableNo; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.column_id = nColId; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_datatype  = SAMPLE_DATATYPE_STRING; \
//         s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_value.c_string((const ::CORBA::String_var)CORBA::string_dup(val));
// 
// #define Sample_Multi_End() \
//     } \
//     s_nBufSize = 0; \
//     s_pMsgBuffer = NULL; \
//     GetAppHostName(s_nAppNo, s_szHostName, 64); \
//     MARSHAL(s_tSampleProcess, s_pMsgBuffer, s_nBufSize); \
//     if (g_pMsgBus->SendMessageToHost(s_pMsgBuffer, DB_TRIGGER_SAMPLE_TYPE, s_nBufSize, s_szHostName, "TRIGGER_SAMPLE_SERVER") < 0)\
//     { \
//         ShowMsg(LOG_INFO_WARN, "采样 (%d,%d) 失败!!", s_nAppNo, s_nTableNo); \
//     } \
//     DEL_ARR_PTR(s_pMsgBuffer);
#else
#define Sample_Multi_Init(nAppNo, nTabNo, nType, nItemNum, nOccurTime, v) \
    s_nAppNo = nAppNo; \
    s_nTableNo = nTabNo; \
    in_para.length(v.size());\
    for(s_nRecIdx=0; s_nRecIdx<v.size(); s_nRecIdx++) \
    { \
    	  s_nItemIdx = -1; \
   			in_para[s_nRecIdx].sample_type = nType;\
				in_para[s_nRecIdx].app_no = nAppNo;\
				in_para[s_nRecIdx].sample_mode = OFFERDATA_SAMPLE_MODE;\  
				in_para[s_nRecIdx].occur_time = nOccurTime;\
				in_para[s_nRecIdx].keyid_num = nItemNum;\
				in_para[s_nRecIdx].flag = 0;\
				in_para[s_nRecIdx].seq_keyid_value.length(nItemNum);
				
#define SAMPLE_MULTI_ITEM_INT(nColId, val) \
        s_nItemIdx++;\
        in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid=MF_CalcRecId(s_nTableNo, nColId, 0); \
   			in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid_datatype=0; \
    		in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid_value.c_long(val);\
    		
#define SAMPLE_MULTI_ITEM_FLT(nColId, val) \
        s_nItemIdx++;\
        in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid=MF_CalcRecId(s_nTableNo, nColId, 0); \
        in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid_datatype=1; \
        in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid_value.c_float(val);
        
#define SAMPLE_MULTI_ITEM_STR(nColId, val) \
        s_nItemIdx++;\
        in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid=MF_CalcRecId(s_nTableNo, nColId, 0); \
  		  in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid_datatype=2; \
   			in_para[s_nRecIdx].seq_keyid_value[s_nItemIdx].keyid_value.c_string(val);
   			
#define Sample_Multi_End() \
    } \
	  tri_client.PutTriggerSampleOneWay(in_para);


#endif

//////////////////////////////////////////////////////////////////////////
// 非域级采样
//////////////////////////////////////////////////////////////////////////
#ifndef _D5000
// #define Sample_Init(nAppNo, nTabNo, nType, nItemNum, nOccurTime) \
//     s_nAppNo = nAppNo; \
//     s_tSampleProcess.seq_length = 1; \
//     s_tSampleProcess.seq_sample_process_request.length(1); \
//     s_pSampleRequest = &s_tSampleProcess.seq_sample_process_request[0]; \
//     s_nItemIdx = -1; \
//     s_nTableNo = nTabNo; \
//     s_pSampleRequest->app_no      = nAppNo; \
//     s_pSampleRequest->sample_type = nType; \
//     s_pSampleRequest->sample_mode = OFFERDATA_SAMPLE_MODE; \
//     s_pSampleRequest->occur_time  = nOccurTime; \
//     s_pSampleRequest->flag        = 0; \
//     s_pSampleRequest->reserve_1   = 0; \
//     s_pSampleRequest->reserve_2   = 0; \
//     s_pSampleRequest->keyid_num   = nItemNum; \
//     s_pSampleRequest->seq_keyid_value.length(nItemNum);
// 
// #define SAMPLE_ITEM_INT(nColId, val) \
//     s_nItemIdx++;\
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.record_id = TABLE_MULT * s_nTableNo + 1; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.column_id = nColId; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_datatype  = SAMPLE_DATATYPE_INT; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_value.c_int(val); 
// 
// #define SAMPLE_ITEM_FLT(nColId, val) \
//     s_nItemIdx++;\
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.record_id = TABLE_MULT * s_nTableNo + 1; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.column_id = nColId; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_datatype  = SAMPLE_DATATYPE_FLOAT; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_value.c_float(val); 
// 
// #define SAMPLE_ITEM_STR(nColId, val) \
//     s_nItemIdx++;\
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.record_id = TABLE_MULT * s_nTableNo + 1; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid.column_id = nColId; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_datatype  = SAMPLE_DATATYPE_STRING; \
//     s_pSampleRequest->seq_keyid_value[s_nItemIdx].keyid_value.c_string((const ::CORBA::String_var)CORBA::string_dup(val));
// 
// #define Sample_End() \
//     s_nBufSize = 0; \
//     s_pMsgBuffer = NULL; \
//     GetAppHostName(s_nAppNo, s_szHostName, 64); \
//     MARSHAL(s_tSampleProcess, s_pMsgBuffer, s_nBufSize); \
//     if (g_pMsgBus->SendMessageToHost(s_pMsgBuffer, DB_TRIGGER_SAMPLE_TYPE, s_nBufSize, s_szHostName, "TRIGGER_SAMPLE_SERVER") < 0)\
//     { \
//         ShowMsg(LOG_INFO_WARN, "采样 (%d) 失败!!", s_nTableNo); \
//     } \
//     DEL_ARR_PTR(s_pMsgBuffer);
#else
#define Sample_Init(nAppNo, nTabNo, nType, nItemNum, nOccurTime) \
  s_nAppNo = nAppNo; \
  s_nItemIdx=-1;\
  s_nTableNo = nTabNo; \
  in_para.length(1); \
	in_para[0].sample_type = nType; \
	in_para[0].app_no = nAppNo; \
	in_para[0].sample_mode = OFFERDATA_SAMPLE_MODE; \    
	in_para[0].occur_time = time(NULL); \
	in_para[0].keyid_num = nItemNum; \
	in_para[0].flag = 0; \
	in_para[0].seq_keyid_value.length(nItemNum);
 
#define SAMPLE_ITEM_INT(nColId, val) \
    s_nItemIdx++;\
    in_para[0].seq_keyid_value[s_nItemIdx].keyid=MF_CalcRecId(s_nTableNo, nColId, 1); \
    in_para[0].seq_keyid_value[s_nItemIdx].keyid_datatype=0; \
    in_para[0].seq_keyid_value[s_nItemIdx].keyid_value.c_long(val);

#define SAMPLE_ITEM_FLT(nColId, val) \
    s_nItemIdx++;\
    in_para[0].seq_keyid_value[s_nItemIdx].keyid=MF_CalcRecId(s_nTableNo, nColId, 1); \
    in_para[0].seq_keyid_value[s_nItemIdx].keyid_datatype=1; \
    in_para[0].seq_keyid_value[s_nItemIdx].keyid_value.c_float(val);
 
#define SAMPLE_ITEM_STR(nColId, val) \
    s_nItemIdx++;\
    in_para[0].seq_keyid_value[s_nItemIdx].keyid=MF_CalcRecId(s_nTableNo, nColId, 1); \
    in_para[0].seq_keyid_value[s_nItemIdx].keyid_datatype=2; \
    in_para[0].seq_keyid_value[s_nItemIdx].keyid_value.c_string(val);
 
#define Sample_End() \
 	  tri_client.PutTriggerSampleOneWay(in_para);

#endif
#endif

#endif