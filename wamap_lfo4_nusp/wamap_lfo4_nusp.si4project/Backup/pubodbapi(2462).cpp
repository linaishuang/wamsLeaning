// pubodbapi.cpp: implementation of the CPubOdbApi class.
//
//////////////////////////////////////////////////////////////////////

#include "pubodbapi.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPubOdbApi::CPubOdbApi()
{
	// printf("CPubOdbApi构造开始\n");
}

CPubOdbApi::~CPubOdbApi()
{
#ifdef _WAMAP_
#ifdef _D5000
	// 若未进行初始化,是否能够退出??
	// PODB_messageExit();
#else
#ifdef _PODB_MSGBUS_
	PODB_ReleaseMsgBus();
#endif // _PODB_MSGBUS_
#endif // _D5000
#endif // _WAMAP_
}

//////////////////////////////////////////////////////////////////////////
int CPubOdbApi::PODB_TableGet(const int tableno, const std::string deffields, CBuffer& buf_base, const int appno, const int contextno)
{
	int ret = -1;
	if (appno == AP_WAMS_FES) {
		ret = m_IDBTable_wams_fes.Open(appno, tableno, contextno);
	}
	else 
	{
		ret = m_IDBTable.Open(appno, tableno, contextno);
	}
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (appno == AP_WAMS_FES)
	{
		ret = m_IDBTable_wams_fes.TableGet(deffields.c_str(), buf_base);
	}
	else
	{
		ret = m_IDBTable.TableGet(deffields.c_str(), buf_base);
	}
	// 过滤记录为空错误
	if (ret < 0)
	{
		sprintf(m_LogString, "读取表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_TableGet(const int tableno, CBuffer& buf_base, const int appno, const int contextno)
{
	int ret = -1;
	if (appno == AP_WAMS_FES)
	{
		ret = m_IDBTable_wams_fes.Open(appno, tableno, contextno);
	}
	else 
	{
		ret = m_IDBTable.Open(appno, tableno, contextno);
	}
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (appno == AP_WAMS_FES)
	{
		ret = m_IDBTable_wams_fes.TableGet(buf_base);
	}
	else
	{
		ret = m_IDBTable.TableGet(buf_base);
	}
	// 过滤记录为空错误
	if (ret < 0)
	{
		sprintf(m_LogString, "读取表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_GetTabRecNum(const int tableno, const int appno, const int contextno)
{
	short           nFieldNum = 0;
	int             nRecordNum = 0, nRecordSize = 0, ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ret = m_IDBTable.GetTablePara(nFieldNum, nRecordNum, nRecordSize);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "获取表记录失败,ret[%d],appno[%d],tabno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return nRecordNum;
}

int CPubOdbApi::PODB_GetFieldNoByName(const int tableno, const char* field_name, const int ret_value, const int appno /*= g_appNo*/, const int contextno /*= g_ctxNo*/)
{
	int tmpFieldNo = ret_value, ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,置默认值,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return tmpFieldNo;
	}

	ret = m_IDBTable.GetFieldNoByName(tmpFieldNo, field_name);
	if (ret <= 0)
	{
		sprintf(m_LogString, "获取表域号失败,置默认值,ret[%d],appno[%d],tabno[%d],ctxno[%d],field_name[%s]", ret, appno, tableno, contextno, field_name);
		MessShow(m_LogString, LTERROR);
		return tmpFieldNo;
	}

	return tmpFieldNo;
}

// ??待调试
// int CPubOdbApi::PODB_TableGetByKey(const int tableno,INT_REC &key_ptr, const char* field_name, char* field_buf_ptr, const int buf_size,const int appno, const int contextno)
// {
//     if ( m_IDBTable.Open(appno, tableno, contextno) != DB_OK )
// 	{
// 		sprintf(m_logstr,"打开表失败,appno[%d],tabno[%d],ctxno[%d]",appno, tableno, contextno);
// 		MessShow(m_logstr,LTERROR);
// 		return -1;
// 	}
// 	
// 	if ( m_IDBTable.TableGetByKey((const char *)key_ptr,field_name,field_buf_ptr,buf_size) != DB_OK )
// 	{
// 		sprintf(m_logstr,"获取表指定记录域失败,key_ptr["INT_REC_FMT"],field_name[%s],buf_size[%d],appno[%d],tabno[%d],ctxno[%d]",(const char *)key_ptr,field_name, buf_size,appno, tableno, contextno);
// 		MessShow(m_logstr,LTERROR);
// 		return -1;
// 	}
// 	return 0;
// }

int CPubOdbApi::PODB_TableWrite(const char* pTabBuf, const int TabBufSize, const int tableno, const int appno, const int contextno)
{
	int ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ret = m_IDBTable.TableWrite(pTabBuf, TabBufSize);
	if (ret < 0)
	{
		sprintf(m_LogString, "写表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d],ret[%d]", ret, appno, tableno, contextno, ret);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_TableModify(const char* field_name, const char* field_buf_ptr, const int buf_size, const int tableno, const int appno, const int contextno)
{
	int ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ret = m_IDBTable.TableModify(field_name, field_buf_ptr, buf_size);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "修改表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_TableModifyByKey(const char* key_ptr, const char* field_name, const char* field_buf_ptr, const int buf_size, const int tableno, const int appno /* =g_appNo */, const int contextno /* = g_ctxNo */)
{
	int ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ret = m_IDBTable.TableModifyByKey(key_ptr, field_name, field_buf_ptr, buf_size);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "修改表失败,ret[%d],key_ptr["INT_REC_FMT"],field_name[%s],appno[%d],tableno[%d],ctxno[%d]", ret, key_ptr, field_name, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_TableModifyByKey(const char* key_ptr, const int field_no, const char* field_buf_ptr, const int buf_size, const int tableno, const int appno, const int contextno)
{
	int ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ret = m_IDBTable.TableModifyByKey(key_ptr, field_no, field_buf_ptr, buf_size);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "修改表失败,ret[%d],key_ptr["INT_REC_FMT"],appno[%d],tableno[%d],ctxno[%d]", ret, key_ptr, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_DeleteRecord(const INT_REC& key_id, const int tableno, const int appno, const int contextno)
{
	int ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	ret = m_IDBTable.DeleteRecord((const char*)&key_id);
	if (ret < DB_OK)   //新一代环境正确的结果返回值是1，d5000环境正确的返回值是0 ，错误的返回值都是-xxxx
	{
		sprintf(m_LogString, "删除记录失败,ret[%d],key_id["INT_REC_FMT"],appno[%d],tableno[%d],ctxno[%d]", ret, INT_REC_VAL(key_id), appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_DeleteRecord(const vector<INT_REC>& key_id, const int tableno, const int appno, const int contextno)
{
	int ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "打开表失败,ret[%d],appno[%d],tableno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	for (int i = 0; i < (int)key_id.size(); i++)
	{
		ret = m_IDBTable.DeleteRecord((const char*)&key_id.at(i));
		if (ret < DB_OK)   //新一代环境正确的结果返回值是1，d5000环境正确的返回值是0 ，错误的返回值都是-xxxx
		{
			sprintf(m_LogString, "删除记录失败,ret[%d],key_id["INT_REC_FMT"],appno[%d],tableno[%d],ctxno[%d]", ret, INT_REC_VAL(key_id.at(i)), appno, tableno, contextno);
			MessShow(m_LogString, LTERROR);
			return -1;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
int  CPubOdbApi::PODB_ExecuteMultiSql(const std::vector<string>& vecsqlstr, const int appno, const int contextno)
{
	int ii, ret_code, sql_num;
#ifdef _D5000
#ifdef _KEDONG
#ifdef _WAMSTOOL
	char sql[5000]={0};
	CHisDbOp hisop(1);
	sql_num = (int)vecsqlstr.size();
	for (ii=0;ii<sql_num;ii++)
	{
		sprintf(sql, "%s", vecsqlstr[ii].c_str());
		if(hisop.ExecSql(sql)<0)
			MessShow("ExecuteMultiSql error", LTERROR);
	}
#endif // _WAMSTOOL
#else
	StrSeq               seq_sql_str;
	sql_sp_client_base* pSqlSPClient = NULL;
	SEQExecMultiResultStru seq_exec_multi_result;
	memset((void*)&seq_exec_multi_result, 0, sizeof(SEQExecMultiResultStru));
	pSqlSPClient = new sql_sp_client_base();
	if (pSqlSPClient == NULL)
	{
		MessShow("sql_sp_client_base 初始化失败", LTERROR);
		return -1;
	}
	sql_num = vecsqlstr.size();
	seq_sql_str.length(sql_num);
	for (ii = 0; ii < sql_num; ii++)
	{
		seq_sql_str[ii] = vecsqlstr[ii].c_str();
	}
	ret_code = pSqlSPClient->ExecuteMultiSql(sql_num, seq_sql_str, seq_exec_multi_result);
	if (ret_code != 0)
	{
		MessShow("ExecuteMultiSql error", LTERROR);
	}

	SAFE_DELETE(pSqlSPClient);
	memset((void*)&seq_exec_multi_result, 0, sizeof(SEQExecMultiResultStru));
#endif // _KEDONG
#else
	vector<T_Corba_Props> props_all;
	T_Corba_Props         tmp_props_all;
	sql_sp_client_base* pSqlSPClient = NULL;
	StrSeq                 seq_sql_str;
	SEQExecMultiResultStru_var seq_exec_multi_result;
	memset((void*)&seq_exec_multi_result, 0, sizeof(SEQExecMultiResultStru_var));

	sprintf(tmp_props_all.prop_name, "ooc.orb.policy.connect_timeout");
	sprintf(tmp_props_all.prop_value, "2000");
	props_all.push_back(tmp_props_all);
	sprintf(tmp_props_all.prop_name, "ooc.orb.policy.request_timeout");
	sprintf(tmp_props_all.prop_value, "20000");
	props_all.push_back(tmp_props_all);

	pSqlSPClient = new sql_sp_client_base(AP_DB_SERVICE, 1, PORT_DB_SQL_SP, "sql_sp_server", props_all, AC_REALTIME_NO, "alias_sql_sp_server");

	if (pSqlSPClient == NULL)
	{
		MessShow("sql_sp_client_base 初始化失败", LTERROR);
		return -1;
	}

	sql_num = vecsqlstr.size();
	seq_sql_str.length(sql_num);
	for (ii = 0; ii < sql_num; ii++)
	{
		seq_sql_str[ii] = (const ::CORBA::String_var)CORBA::string_dup(vecsqlstr[ii].c_str());
	}

	ret_code = pSqlSPClient->ExecuteMultiSql(sql_num, seq_sql_str, seq_exec_multi_result);
	if (ret_code != 0)
	{
		MessShow("ExecuteMultiSql error", LTERROR);
	}

	props_all.clear();
	SAFE_DELETE(pSqlSPClient);
	memset((void*)&seq_exec_multi_result, 0, sizeof(SEQExecMultiResultStru_var));
#endif
	return 0;
}

int CPubOdbApi::PODB_TableClearSql(const string tablename)
{
	string sqlstr = "delete ";
	sqlstr = sqlstr + tablename;
	vector<string> vecsql;
	vecsql.push_back(sqlstr);
	if (PODB_ExecuteMultiSql(vecsql) < 0)
	{
		sprintf(m_LogString, "表清空失败: [%s]", sqlstr.c_str());
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "表清空成功: [%s]", sqlstr.c_str());
		MessShow(m_LogString, LTINFO);
	}

	return 0;
}

int CPubOdbApi::PODB_ModifyTableBySqls(const std::vector<string>& vecsqlstr, const int appno, const int contextno)
{
	int ii, ret_code = 0, sql_num = 0;
	if (vecsqlstr.empty())
	{
		return 0;
	}
#ifdef _NUSP
	ret_code = PODB_ExecuteMultiSql(vecsqlstr, appno, contextno);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "修改商用库失败");
		MessShow(m_LogString, LTERROR);
	}
#else
#ifdef _D5000
	modify_client_base* modify_client = NULL;
	TSqlModifyRequest           sql_modify_request;
	SEQSqlModifyAnswer		    sql_modify_answer;
	modify_client = new modify_client_base();
	if (modify_client == NULL)
	{
		MessShow("modify_client_base 初始化失败", LTERROR);
		return -1;
	}
	sql_num = (int)vecsqlstr.size();
	sql_modify_request.app_no = appno;
	sql_modify_request.seq_sql_request.length(sql_num);
	for (ii = 0; ii < (int)vecsqlstr.size(); ii++)
	{
		sql_modify_request.seq_sql_request[ii] = (const ::MLang::STRING)MLang::string_dup(vecsqlstr[ii].c_str());
		// 		sprintf(m_LogString,"seq_sql:[%s]",sql_modify_request.seq_sql_request[ii].c_str());
		// 		MessShow(m_LogString);
	}

	ret_code = modify_client->ModifyTableBySqls(sql_modify_request, sql_modify_answer);
	for (ii = 0; ii < (int)sql_modify_answer.length(); ii++)
	{
		if (sql_modify_answer[ii].success_flag != 0)
		{
			sprintf(m_LogString, "修改商用库失败,table[%d],optype[%d],flag[%d]",
				sql_modify_answer[ii].table_no, sql_modify_answer[ii].op_type, sql_modify_answer[ii].success_flag);
			MessShow(m_LogString, LTERROR);
		}
	}
	SAFE_DELETE(modify_client);
#else
	vector<T_Corba_Props>       props_all;
	modify_client_base* modify_client = NULL;
	TSqlModifyRequest           sql_modify_request;
	SEQSqlModifyAnswer_var      sql_modify_answer;
	sql_num = (int)vecsqlstr.size();
	sql_modify_request.app_no = appno;
	sql_modify_request.seq_sql_request.length(sql_num);
	for (ii = 0; ii < (int)vecsqlstr.size(); ii++)
	{
		sql_modify_request.seq_sql_request[ii] = (const ::CORBA::String_var)CORBA::string_dup(vecsqlstr[ii].c_str());
	}

	T_Corba_Props  tmp_props_all;
	sprintf(tmp_props_all.prop_name, "ooc.orb.policy.connect_timeout");
	sprintf(tmp_props_all.prop_value, "2000");
	props_all.push_back(tmp_props_all);
	sprintf(tmp_props_all.prop_name, "ooc.orb.policy.request_timeout");
	sprintf(tmp_props_all.prop_value, "20000");
	props_all.push_back(tmp_props_all);

	modify_client = new modify_client_base(AP_DB_SERVICE, 1, PORT_DB_MODIFY, "db_modify_server", props_all, contextno);
	if (modify_client == NULL)
	{
		MessShow("modify_client_base 初始化失败", LTERROR);
		return -1;
	}

	ret_code = modify_client->ModifyTableBySqls(sql_modify_request, sql_modify_answer);
	props_all.clear();
	SAFE_DELETE(modify_client);
	if (ret_code != 0)
	{
		sprintf(m_LogString, "商用库修改错误[%d]", ret_code);
		MessShow(m_LogString, LTERROR);
	}
#endif // _D5000
#endif // _NUSP
	return ret_code;
}

int CPubOdbApi::PODB_ModifyTableBySqlsp(const std::vector<string>& vecsqlstr, const int appno, const int contextno)
{
	if (vecsqlstr.empty())
	{
		return 0;
	}
	char sql[5000];
#ifdef _D5000
#ifdef _KEDONG
#ifdef _WAMSTOOL
	CHisDbOp hisop(1);
	for (int ii=0;ii<(int)vecsqlstr.size();ii++)
	{
		sprintf(sql, "%s", vecsqlstr[ii].c_str());
		if(hisop.ExecSql(sql)<0)
		{
			sprintf(m_LogString, "执行SQL[%s]失败", sql);
			MessShow(m_LogString);
		}
	}
#endif // _WAMSTOOL
#else
	sql_sp_client_base* sql_sp_client;
	sql_sp_client = new sql_sp_client_base();
	SEQDBErrorStru          seq_db_err;
	int ii, retcode = 0;
	for (ii = 0; ii < (int)vecsqlstr.size(); ii++)
	{
		sprintf(sql, "%s", vecsqlstr[ii].c_str());
		retcode = sql_sp_client->ExecuteSql(sql, seq_db_err);
		if (retcode == 0)
		{
			sprintf(m_LogString, "执行SQL[%s]成功", sql);
			MessShow(m_LogString);
		}
		else
		{
			sprintf(m_LogString, "执行SQL[%s]失败", sql);
			MessShow(m_LogString);
		}
	}
	if (sql_sp_client)		delete sql_sp_client;
#endif // _KEDONG
#endif // _D5000
}
int CPubOdbApi::PODB_GetFacIDByFacName(INT_REC& fac_id, const char* fac_name, const int appno, const int contextno)
{
	int ret = -1;
#ifdef _KEDONG
	m_ODBApi.SetAppNo(g_ctxName, g_ctxNo, g_appName, appno);
#else
	m_ODBApi.SetAppNo(appno, contextno);
#endif // _KEDONG
	ret = m_ODBApi.GetFacIDByFacName(fac_id, fac_name);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "根据厂站名称获取厂站ID错误[%s]", fac_name);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_GetFacNameByFacID(char* fac_name, const INT_REC fac_id, const int appno, const int contextno)
{
	int ret = -1;
#ifdef _KEDONG
	m_ODBApi.SetAppNo(g_ctxName, g_ctxNo, g_appName, appno);
#else
	m_ODBApi.SetAppNo(appno, contextno);
	ret = m_ODBApi.GetFacNameByFacID(fac_name, fac_id);  // 冀北科东环境接口形参类型有问题
#endif // _KEDONG
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "根据厂站ID获取厂站名称错误["INT_REC_FMT"]", INT_REC_VAL(fac_id));
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_GetNameStringByID(const INT_REC reference_id, std::string& ref_string, const int appno, const int contextno)
{
	if (!ref_string.empty())
		ref_string.clear();

	int ret = -1;
#ifdef _KEDONG
	m_ODBApi.SetAppNo(g_ctxName, g_ctxNo, g_appName, appno);
#else
	m_ODBApi.SetAppNo(appno, contextno);
#endif //_KEDONG
#ifdef _NUSP
	ret = m_ODBApi.GetNameStringByRtdbKeyID(reference_id, ref_string);
#else
	ret = m_ODBApi.GetNameStringByID(reference_id, ref_string);
#endif // _NUSP
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "根据ID获取名称错误["INT_REC_FMT"]", INT_REC_VAL(reference_id));
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_GetNameStringByID(const std::vector<INT_REC>& vec_refid, std::vector<std::string>& vec_ref_string, const int appno, const int contextno)
{
	int ret = -1;
#ifdef _KEDONG
	m_ODBApi.SetAppNo(g_ctxName, g_ctxNo, g_appName, appno);
#else
	m_ODBApi.SetAppNo(appno, contextno);
#endif // _KEDONG
#ifdef _NUSP
	ret = m_ODBApi.GetNameStringByRtdbKeyID(vec_refid, vec_ref_string);
#else
	ret = m_ODBApi.GetNameStringByID(vec_refid, vec_ref_string);
#endif // _NUSP
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "根据ID获取名称错误");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return 0;
}

int CPubOdbApi::PODB_TableClear(const int tableno, const int appno, const int contextno)
{
	short           nFieldNum = 0;
	int             nRecordNum = 0, nRecordSize = 0, ret = -1;
	ret = m_IDBTable.Open(appno, tableno, contextno);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "表打开失败:ret[%d],tab[%d],app[%d],ctx[%d]", ret, tableno, appno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}
#ifndef _KEDONG
	ret = m_IDBTable.GetTablePara(nFieldNum, nRecordNum, nRecordSize);
	if (ret != DB_OK)
	{
		sprintf(m_LogString, "获取表记录失败,ret[%d],appno[%d],tabno[%d],ctxno[%d]", ret, appno, tableno, contextno);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	if (nRecordNum > 0)
#endif // !_KEDONG
	{
		ret = m_IDBTable.TableClear();
		if (ret != DB_OK)
		{
			sprintf(m_LogString, "表清空失败:ret[%d],tab[%d],app[%d],ctx[%d]", ret, tableno, appno, contextno);
			MessShow(m_LogString, LTERROR);
			return -1;
		}
	}

	return 0;
}

int CPubOdbApi::PODB_IsOnDuty(const int app_id, const int ctx_no)
{
	if (m_ServMan.IsOnDuty(app_id, ctx_no) == 1)
	{
		sprintf(m_LogString, "本机为主机,应用[%d][%d]", app_id, ctx_no);
		MessShow(m_LogString, LTINFO);
		return 1;
	}
	else
	{
		sprintf(m_LogString, "本机非主机,应用[%d][%d]", app_id, ctx_no);
		MessShow(m_LogString, LTINFO);
		return 0;
	}
}

int CPubOdbApi::PODB_GetHostName(string& hostname, const int app_id, const int ctx_no)
{
	char tmphostname[MAXBUF];
	memset(tmphostname, 0, sizeof(tmphostname));

#ifdef _NUSP
	int ret = m_ServMan.RequestSubscn(tmphostname, app_id);
#else
	int ret = m_ServMan.RequestService(app_id, 1, tmphostname, ctx_no)
#endif // _NUSP

	if (ret > 0)
	{
		sprintf(m_LogString, "获取主机名称成功[%s],应用[%d][%d]", tmphostname, app_id, ctx_no);
		MessShow(m_LogString, LTINFO);
		hostname = tmphostname;
		return 1;
	}
	else
	{
		sprintf(m_LogString, "获取主机名称失败[%s],应用[%d][%d]", tmphostname, app_id, ctx_no);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
}

int CPubOdbApi::PODB_GetHostName(char* hostname, const int bufsize, const int app_id, const int ctx_no)
{
	char tmphostname[MAXBUF];
	memset(tmphostname, 0, sizeof(tmphostname));
	memset(hostname, 0, bufsize);
#ifdef _NUSP
	int ret = m_ServMan.RequestSubscn(tmphostname, app_id);
#else
	int ret = m_ServMan.RequestService(app_id, 1, tmphostname, ctx_no)
#endif // _NUSP

	if (ret > 0)
	{
		sprintf(m_LogString, "获取主机名称成功[%s],应用[%d][%d]", tmphostname, app_id, ctx_no);
		MessShow(m_LogString, LTINFO);
		strncpy(hostname, tmphostname, bufsize);
		return 1;
	}
	else
	{
		sprintf(m_LogString, "获取主机名称失败[%s],应用[%d][%d]", tmphostname, app_id, ctx_no);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
}


#ifdef _D5000
int CPubOdbApi::PODB_ProcRegister(char* context_name, char* app_name, char* proc_name)
{
#ifdef _NUSP
	NS_SYSADMIN::CProcCommon	prcm;
#else
	proc_invocation     prcm;
#endif // _NUSP

	int ret = prcm.proc_init(context_name, app_name, proc_name);
	if (ret < 0)
	{
		sprintf(m_LogString, "%s 进程注册失败!", proc_name);
		MessShow(m_LogString, LTINFO);
		return -1;;
	}
	else
	{
		sprintf(m_LogString, "%s 进程注册成功!", proc_name);
		MessShow(m_LogString, LTINFO);
		return 0;
	}
}
#else
int CPubOdbApi::PODB_ProcRegister(const int app_id, const string register_name, const short mon_cycle, const short context /* = 0 */)
{
	if (m_SamMan.ProcRegister(app_id, register_name.c_str(), mon_cycle, context) == 1)
	{
		sprintf(m_LogString, "进程注册成功[%s],应用[%d][%d]", register_name.c_str(), mon_cycle, context);
		MessShow(m_LogString, LTINFO);
		return -1;
	}
	else
	{
		sprintf(m_LogString, "进程注册失败[%s],应用[%d][%d]", register_name.c_str(), mon_cycle, context);
		MessShow(m_LogString, LTINFO);
		return 0;
	}
}
#endif

int CPubOdbApi::PODB_InitColID(vector<int>& VecCol, const int tableno)
{
	if (!VecCol.empty())
	{
		vector<int>().swap(VecCol);
		VecCol.clear();
	}
	sprintf(m_LogString, "初始化域号[%d]", tableno);
	MessShow(m_LogString, LTDEBUG);
#ifdef _WAMAP_
	if (tableno == GN_DEVICE_NO_TAB)
	{
		// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR QWR ANG ANC EV EA F
		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB, V_A_VALUE_DF1, GN_v_a_value));
		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB, V_A_ANGLE_DF1, GN_v_a_angle));
		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB, P_VAL, GN_p));
		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB, Q_VAL, GN_q));
		//VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB, F_VALUE_DF, GN_f_value));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,V_B_VALUE_DF1,228));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,V_B_ANGLE_DF1,224));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,V_C_VALUE_DF1,229));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,V_C_ANGLE_DF1,231));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,I_A_VALUE_DF,120));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,I_A_ANGLE_DF1,233));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,I_B_VALUE_DF,130));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,I_B_ANGLE_DF1,235));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,I_C_VALUE_DF,140));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,I_C_ANGLE_DF1,237));
		
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,GN_ANGLE_CAL_GN,304));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,E_VALUE_GN,294));
		// 		VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,E_ANGLE_GN,296));
				// 从厂站频率里面获取设备频率
		// 		VecCol.push_back(PODB_GetFieldNoByName(FAC_INFO_NO_TAB,F_VALUE_DF,133));
				//VecCol.push_back(PODB_GetFieldNoByName(GN_DEVICE_NO_TAB,F_VALUE_DF));
		if ((int)VecCol.size() < VEC_VIP_NUM)
		{
			sprintf(m_LogString, "列号个数错误[%d]", (int)VecCol.size());
			MessShow(m_LogString, LTERROR);
			return -1;
		}
	}
	else if (tableno == ACLN_DOT_NO_TAB)
	{
		// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR F
		VecCol.push_back(PODB_GetFieldNoByName(ACLN_DOT_NO_TAB, V_A_VALUE_DF1, ACLN_v_a_value));   //根据域名查找,需区分D5000和OPEN3000,后续改进
		VecCol.push_back(PODB_GetFieldNoByName(ACLN_DOT_NO_TAB, V_A_ANGLE_DF1, ACLN_v_a_angle));
		VecCol.push_back(PODB_GetFieldNoByName(ACLN_DOT_NO_TAB, P_VAL, ACLN_p));
		VecCol.push_back(PODB_GetFieldNoByName(ACLN_DOT_NO_TAB, Q_VAL, ACLN_q));
		//VecCol.push_back(PODB_GetFieldNoByName(ACLN_DOT_NO_TAB, F_VALUE_DF, ACLN_f_value));
		//VecCol.push_back(PODB_GetFieldNoByName(PLANT_INFO_NO_TAB, F_VALUE_DF, 68));			// 获取电厂频率域号
		//VecCol.push_back(PODB_GetFieldNoByName(SUBSTATION_INFO_NO_TAB, F_VALUE_DF, 42));	// 获取变电站频率域号
		// 从厂站频率里面获取设备频率
		//VecCol.push_back(PODB_GetFieldNoByName(FAC_INFO_NO_TAB, F_VALUE_DF, 133));
		//VecCol.push_back(PODB_GetFieldNoByName(ACLN_DOT_NO_TAB,F_VALUE_DF));
		if ((int)VecCol.size() < VEC_VIP_NUM)
		{
			sprintf(m_LogString, "列号个数错误[%d]", (int)VecCol.size());
			MessShow(m_LogString, LTERROR);
			return -1;
		}
	}
	else if (tableno == TRWD_DEVICE_NO_TAB)
	{
		// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR F
		
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,V_B_VALUE_DF1,183));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,V_B_ANGLE_DF1,189));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,V_C_VALUE_DF1,185));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,V_C_ANGLE_DF1,191));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,I_A_VALUE_DF,90));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,I_A_ANGLE_DF,95));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,I_B_VALUE_DF,100));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,I_B_ANGLE_DF,105));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,I_C_VALUE_DF,110));
		// 		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB,I_C_ANGLE_DF,115));
		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB, V_A_VALUE_DF1, TRWD_v_a_value));
		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB, V_A_ANGLE_DF1, TRWD_v_a_angle));
		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB, P_VAL, TRWD_p));
		VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB, Q_VAL, TRWD_q));
		//VecCol.push_back(PODB_GetFieldNoByName(TRWD_DEVICE_NO_TAB, F_VALUE_DF, TRWD_f_value));
		// 从厂站频率里面获取设备频率
		//VecCol.push_back(PODB_GetFieldNoByName(PLANT_INFO_NO_TAB, F_VALUE_DF, 68));			// 获取电厂频率域号
		//VecCol.push_back(PODB_GetFieldNoByName(SUBSTATION_INFO_NO_TAB, F_VALUE_DF, 42));	// 获取变电站频率域号
		// 从厂站频率里面获取设备频率
		//VecCol.push_back(PODB_GetFieldNoByName(FAC_INFO_NO_TAB, F_VALUE_DF, 133));
		if ((int)VecCol.size() < VEC_VIP_NUM)
		{
			sprintf(m_LogString, "列号个数错误[%d]", (int)VecCol.size());
			MessShow(m_LogString, LTERROR);
			return -1;
		}
	}
	else if (tableno == BS_DEVICE_NO_TAB)
	{
		// UAV UAA UBV UBA UCV UCA V F
// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,"i_a_value"));
// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,"i_a_angle"));
// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,"i_b_value"));
// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,"i_b_angle"));
// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,"i_c_value"));
// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,"i_c_angle"));
		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB, V_A_VALUE_DF, BS_v_a_value));
		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB, V_A_ANGLE_DF1, BS_v_a_angle));
		// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,V_B_VALUE_DF,90));
		// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,V_B_ANGLE_DF1,182));
		// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,V_C_VALUE_DF,100));
		// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,V_C_ANGLE_DF1,184));
		// 		VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,V_VAL,30));
				// 从厂站频率里面获取设备频率
				//VecCol.push_back(PODB_GetFieldNoByName(BS_DEVICE_NO_TAB,F_VALUE_DF));
		// 		VecCol.push_back(PODB_GetFieldNoByName(FAC_INFO_NO_TAB,F_VALUE_DF,133));
		if ((int)VecCol.size() < VEC_V_NUM)
		{
			sprintf(m_LogString, "列号个数错误[%d]", (int)VecCol.size());
			MessShow(m_LogString, LTERROR);
			return -1;
		}
	}
	else
	{
		sprintf(m_LogString, "表号错误[%d]", tableno);
		MessShow(m_LogString, LTERROR);
	}

#endif // _WAMAP_
	return 0;
}

int CPubOdbApi::PODB_IsFlagAll(int* nFlag, const int nFlagNum)
{
	int ii = 0;
	for (ii = 0; ii < nFlagNum; ii++)
	{
		if (nFlag[ii] == 0)
		{
			break;
		}
	}

	return ii;
}
//////////////////////////////////////////////////////////////////////////
// 基本设备信息
int CPubOdbApi::PODB_GetFacInfo(vector<FAC_TAB_CRE>& VecFacTab)
{
	if (!VecFacTab.empty())
	{
		VecFacTab.clear();
	}
#ifdef _WAMAP_
#ifdef _NUSP
	CBuffer buf_base_1;
	//	发电厂
	if (PODB_TableGet(PLANT_INFO_NO_TAB, PLANT_TAB_CRE_FLD, buf_base_1) < 0)
	{
		return -1;
	}

	int nInfo = buf_base_1.GetLength() / sizeof(PLANT_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "发电厂信息表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "发电厂信息表[%d]", nInfo);
		MessShow(m_LogString);
	}

	FAC_TAB_CRE* pInfo_1 = NULL;
	pInfo_1 = new FAC_TAB_CRE[nInfo];
	memcpy(pInfo_1, buf_base_1.GetBufPtr(), nInfo * sizeof(FAC_TAB_CRE));
	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecFacTab.push_back(pInfo_1[i]);
	}
	SAFE_DELETE_ARRAY(pInfo_1);

	//	变电站
	CBuffer buf_base;
	if (PODB_TableGet(SUBSTATION_INFO_NO_TAB, SUBSTATION_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	nInfo = buf_base.GetLength() / sizeof(SUBSTATION_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "变电站信息表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "变电站信息表[%d]", nInfo);
		MessShow(m_LogString);
	}

	FAC_TAB_CRE* pInfo = NULL;
	pInfo = new FAC_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(FAC_TAB_CRE));
	for (i = 0; i < nInfo; i++)
	{
		VecFacTab.push_back(pInfo[i]);
	}

#else // _NUSP

	CBuffer buf_base;
	if (PODB_TableGet(FAC_INFO_NO_TAB, FAC_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(FAC_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "厂站信息表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "厂站信息表[%d]", nInfo);
		MessShow(m_LogString);
	}

	FAC_TAB_CRE* pInfo = NULL;
	pInfo = new FAC_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(FAC_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecFacTab.push_back(pInfo[i]);
	}
#endif // _NUSP		
	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CPubOdbApi::PODB_GetAreaInfo(vector<AREA_TAB_CRE>& VecAreaTab)
{
	if (!VecAreaTab.empty())
	{
		VecAreaTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(DIV_INFO_NO_TAB, AREA_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(AREA_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "行政区域表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "行政区域表[%d]", nInfo);
		MessShow(m_LogString);
	}

	AREA_TAB_CRE* pInfo = NULL;
	pInfo = new AREA_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(AREA_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecAreaTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CPubOdbApi::PODB_GetVLTypeInfo(vector<VL_TYPE_TAB_CRE>& VecVLTypeTab)
{
	if (!VecVLTypeTab.empty())
	{
		VecVLTypeTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(VL_TYPE_NO_TAB, VL_TYPE_TAB_CRE_FLD, buf_base) < 0)
	{
		sprintf(m_LogString, "读取电压等级表[%d]失败", VL_TYPE_NO_TAB);
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(VL_TYPE_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "电压等级表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "电压等级表[%d]", nInfo);
		MessShow(m_LogString);
	}

	VL_TYPE_TAB_CRE* pInfo = NULL;
	pInfo = new VL_TYPE_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(VL_TYPE_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecVLTypeTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CPubOdbApi::PODB_GetFesYcInfo(vector<FES_YC_TAB_CRE>& VecFesYcTab)
{
	if (!VecFesYcTab.empty())
	{
		VecFesYcTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(WAM_FES_YC_DEFINE_NO_TAB, FES_YC_TAB_CRE_FLD, buf_base, AP_WAMS_FES) < 0) // AP_WAMS_FES
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(FES_YC_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "WAMS前置遥测定义表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "WAMS前置遥测定义表[%d]", nInfo);
		MessShow(m_LogString, LTDEBUG);
	}

	FES_YC_TAB_CRE* pInfo = NULL;
	pInfo = new FES_YC_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(FES_YC_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		if (pInfo[i].col_no < 0)
		{
			continue;
		}
		VecFesYcTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CPubOdbApi::PODB_GetBusInfo(vector<BUS_TAB_CRE>& VecBusTab)
{
	if (!VecBusTab.empty())
	{
		VecBusTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(BS_DEVICE_NO_TAB, BUS_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(BUS_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "母线表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "母线表[%d]", nInfo);
		MessShow(m_LogString);
	}

	BUS_TAB_CRE* pInfo = NULL;
	pInfo = new BUS_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(BUS_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecBusTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CPubOdbApi::PODB_GetAclnInfo(vector<ACLINE_TAB_CRE>& VecAclnTab)
{
	if (!VecAclnTab.empty())
	{
		VecAclnTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(ACLN_DEVICE_NO_TAB, ACLINE_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(ACLINE_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "交流线段表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "交流线段表[%d]", nInfo);
		MessShow(m_LogString);
	}

	ACLINE_TAB_CRE* pInfo = NULL;
	pInfo = new ACLINE_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(ACLINE_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecAclnTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CPubOdbApi::PODB_GetAclnDotInfo(vector<ACLNDOT_TAB_CRE>& VecAclnDotTab)
{
	if (!VecAclnDotTab.empty())
	{
		VecAclnDotTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(ACLN_DOT_NO_TAB, ACLNDOT_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(ACLNDOT_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "交流线段端点表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "交流线段端点表[%d]", nInfo);
		MessShow(m_LogString);
	}

	ACLNDOT_TAB_CRE* pInfo = NULL;
	pInfo = new ACLNDOT_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(ACLNDOT_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecAclnDotTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CPubOdbApi::PODB_GetGenInfo(vector<GEN_TAB_CRE>& VecGenTab)
{
	if (!VecGenTab.empty())
	{
		VecGenTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(GN_DEVICE_NO_TAB, GEN_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(GEN_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "发电机表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "发电机表[%d]", nInfo);
		MessShow(m_LogString);
	}

	GEN_TAB_CRE* pInfo = NULL;
	pInfo = new GEN_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(GEN_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecGenTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}

int CPubOdbApi::PODB_GetTrInfo(vector<TR_TAB_CRE>& VecTrTab)
{
	if (!VecTrTab.empty())
	{
		VecTrTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(TR_DEVICE_NO_TAB, TR_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(TR_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "变压器表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "变压器表[%d]", nInfo);
		MessShow(m_LogString);
	}

	TR_TAB_CRE* pInfo = NULL;
	pInfo = new TR_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(TR_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecTrTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CPubOdbApi::PODB_GetTrwdInfo(vector<TRWD_TAB_CRE>& VecTrwdTab)
{
	if (!VecTrwdTab.empty())
	{
		VecTrwdTab.clear();
	}
#ifdef _WAMAP_
	CBuffer buf_base;
	if (PODB_TableGet(TRWD_DEVICE_NO_TAB, TRWD_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(TRWD_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "变压器绕组表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "变压器绕组表[%d]", nInfo);
		MessShow(m_LogString);
	}

	TRWD_TAB_CRE* pInfo = NULL;
	pInfo = new TRWD_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(TRWD_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecTrwdTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif
	return 0;
}
int CPubOdbApi::PODB_GetValueInfo(vector<VALUEINFO_TAB_CRE>& VecValueInfoTab)
{
	if (!VecValueInfoTab.empty())
	{
		VecValueInfoTab.clear();
	}
#ifdef _WAMAP_
#ifdef _D5000

#else
	CBuffer buf_base;
	if (PODB_TableGet(VALUE_INFO_NO_TAB, VALUEINFO_TAB_CRE_FLD, buf_base) < 0)
	{
		return -1;
	}

	int nInfo = buf_base.GetLength() / sizeof(VALUEINFO_TAB_CRE);
	if (nInfo <= 0)
	{
		sprintf(m_LogString, "测点遥测信息表记录为空[%d]", nInfo);
		MessShow(m_LogString, LTERROR);
		return 0;
	}
	else
	{
		sprintf(m_LogString, "测点遥测信息表[%d]", nInfo);
		MessShow(m_LogString);
	}

	VALUEINFO_TAB_CRE* pInfo = NULL;
	pInfo = new VALUEINFO_TAB_CRE[nInfo];
	memcpy(pInfo, buf_base.GetBufPtr(), nInfo * sizeof(VALUEINFO_TAB_CRE));

	int i = 0;
	for (i = 0; i < nInfo; i++)
	{
		VecValueInfoTab.push_back(pInfo[i]);
	}

	SAFE_DELETE_ARRAY(pInfo);
#endif  // _D5000
#endif  // _WAMAP_
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void CPubOdbApi::MessShow(const char* log, const int logtype)
{
	cout << ">>>" << log << endl;
	return;
}

//////////////////////////////////////////////////////////////////////////
//根据KEY_ID_STRU keyid获取表号，记录号，域号，设备ID号
void CPubOdbApi::PODB_ParseKeyid(const KEY_ID_STRU keyid, int& table_no, int& rec_id, int& column_id, INT_REC& devid)
{
#ifndef _NUSP
	table_no = 0;
	rec_id = 0;
	column_id = 0;
	devid = 0;

#ifdef _D5000
	KEY_STRU  buf;
	memcpy(&buf, &keyid, sizeof(KEY_STRU));
	table_no = buf.table_no;
	rec_id = buf.key;
	column_id = buf.field_id;

	KEY_STRU  buf_2;
	buf_2.table_no = table_no;
	buf_2.key = rec_id;
	buf_2.field_id = 0;
	memcpy(&devid, &buf_2, sizeof(INT_REC));
#else
	table_no = keyid.record_id / DB_TABLE_BASE;
	rec_id = keyid.record_id - table_no * DB_TABLE_BASE;
	column_id = keyid.column_id;

	devid = keyid.record_id;
#endif // _D5000
#endif // !_NUSP
	return;
}
// 解析记录号
void CPubOdbApi::PODB_ParseKeyid(const INT_REC keyid, int& table_no, int& area_no, int& serial_no, int& column_no)
{
#ifdef _NUSP
	table_no = (TABLE_NO_T)keyid.GetTableID();
	ARA_SER_NO_T areaSerialNo = keyid.GetAreaSerialNo();
	column_no = (COLUMN_NO_T)keyid.GetColumnNo();

	long minAreaSerialNo10 = 1000000;        // 用于判断记录ID长度是否为10位
	long minAreaSerialNo16 = 1000000000000;  // 用于判断记录ID长度是否为16位
	if (areaSerialNo < minAreaSerialNo10)    // 记录ID为 10 位, 无记录序号, 20210126
	{
		area_no = areaSerialNo;
		serial_no = 0;  // 序号
	}
	else if (areaSerialNo < minAreaSerialNo16)    // 记录ID为 16 位, 记录序号只有4位, 20210126
	{
		area_no = areaSerialNo / 10000;   // 区域号
		serial_no = areaSerialNo % 10000;   // 序号
	}
	else
	{
		area_no = areaSerialNo / 100000000;   // 区域号
		serial_no = areaSerialNo % 100000000;  // 序号
	}
#endif // _NUSP
	return;
}
// 将记录ID中记录序号增加 n
void CPubOdbApi::IncrRtdbId(INT_REC& id, int no = 1)
{
#ifdef _NUSP
	int table_no = 0;
	int area_no = 0;
	int serial_no = 0;
	int column_no = 0;
	PODB_ParseKeyid(id, table_no, area_no, serial_no, column_no);
	serial_no += no;
	ARA_SER_NO_T area_serial_no = area_no;
	area_serial_no = area_serial_no * 100000000 + serial_no;
	id.create_key(table_no, area_serial_no, column_no);
#else
	id += no;
#endif // _NUSP
	return;
}
// 将记录ID中记录序号减少 n
void CPubOdbApi::DescRtdbId(INT_REC& id, int no = 1)
{
#ifdef _NUSP
	int table_no = 0;
	int area_no = 0;
	int serial_no = 0;
	int column_no = 0;
	PODB_ParseKeyid(id, table_no, area_no, serial_no, column_no);
	serial_no -= no;
	ARA_SER_NO_T area_serial_no = area_no;
	area_serial_no = area_serial_no * 100000000 + serial_no;
	id.create_key(table_no, area_serial_no, column_no);
#else
	id -= no;
#endif // _NUSP
	return;
}
#ifdef _NUSP
//根据INT_REC keyid获取设备ID号
INT_REC CPubOdbApi::PODB_GetRecordId(const INT_REC keyid)
{
	INT_REC devid;
	ZERO_INT_REC(devid);
	devid.create_key(keyid.GetTableID(), keyid.GetAreaSerialNo(), 0);
	return devid;
}

//根据INT_REC keyid获取域号
int CPubOdbApi::PODB_GetColumnId(const INT_REC keyid)
{
	int column_id = 0;
	column_id = (int)keyid.GetColumnNo();
	return column_id;
}
#else // _NUSP
#ifdef _D5000
//根据KEY_ID_STRU keyid获取设备ID号
INT_REC CPubOdbApi::PODB_GetRecordId(const long keyid)
{
	INT_REC devid = 0;
	KEY_STRU  buf;
	memcpy(&buf, &keyid, sizeof(KEY_STRU));
	buf.field_id = 0;
	memcpy(&devid, &buf, sizeof(INT_REC));
	return devid;
}

//根据KEY_ID_STRU keyid获取域号
int CPubOdbApi::PODB_GetColumnId(const long keyid)
{
	int column_id = 0;
	KEY_STRU  buf;
	memcpy(&buf, &keyid, sizeof(KEY_STRU));
	column_id = buf.field_id;
	return column_id;
}

#else // _D5000
//根据KEY_ID_STRU keyid获取设备ID号
INT_REC CPubOdbApi::PODB_GetRecordId(const KEY_ID_STRU keyid)
{
	INT_REC devid = 0;
	devid = keyid.record_id;
	return devid;
}

//根据KEY_ID_STRU keyid获取域号
int CPubOdbApi::PODB_GetColumnId(const KEY_ID_STRU keyid)
{
	int column_id = 0;
	column_id = keyid.column_id;
	return column_id;
}
#endif // _D5000
#endif // _NUSP

//根据设备ID获取表号
int CPubOdbApi::PODB_GetTabelNoByDevId(const INT_REC dev_id)
{
	int table_no = -1;
#ifdef _NUSP
	table_no = (int)dev_id.GetTableID();
#else // _NUSP
#ifdef _D5000
	KEY_STRU  buf;
	memcpy(&buf, &dev_id, sizeof(KEY_STRU));
	table_no = buf.table_no;

	if (table_no <= 0)
	{
		table_no = dev_id / DB_TABLE_BASE;
	}
#else // _D5000
	table_no = dev_id / DB_TABLE_BASE;
#endif // _D5000
#endif // _NUSP	
	return table_no;
}

//根据表号，记录号，域号获取KEY_ID_STRU keyid
void CPubOdbApi::PODB_SetKeyid(const int table_no, const int rec_id, const int column_id, KEY_ID_STRU& keyid)
{
#ifdef _NUSP
	//新一代未使用
#else
#ifdef _D5000
	KEY_STRU  buf;
	buf.table_no = table_no;
	buf.key = rec_id;
	buf.field_id = column_id;
	memcpy(&keyid, &buf, sizeof(KEY_ID_STRU));

#else
	keyid.record_id = table_no * DB_TABLE_BASE + rec_id;
	keyid.column_id = column_id;
#endif // _D5000
#endif // _NUSP	
	return;
}

//根据设备ID、域号获取KEY_ID_STRU keyid
void CPubOdbApi::PODB_SetKeyid(const INT_REC dev_id, const int column_id, KEY_ID_STRU& keyid)
{
#ifdef _NUSP
	//新一代未使用
#else
#ifdef _D5000
	KEY_STRU  buf;
	memcpy(&buf, &dev_id, sizeof(KEY_STRU));
	int table_no = buf.table_no;
	int rec_id = buf.key;
	int col_id = column_id;
	PODB_SetKeyid(table_no, rec_id, col_id, keyid);
#else
	keyid.record_id = dev_id;
	keyid.column_id = column_id;
#endif // _D5000
#endif // _NUSP	
	return;
}

//根据表号，记录号获取设备ID
void CPubOdbApi::PODB_GetDevIdByKeyID(const int& table_no, const int& rec_id, INT_REC& devid)
{
	ZERO_INT_REC(devid);
#ifdef _NUSP	
	int column_no = 0;
	ARA_SER_NO_T area_serial_no = CUR_AREA_NO;
	area_serial_no = area_serial_no * 100000000 + rec_id;
	devid.create_key(table_no, area_serial_no, column_no);
#else
#ifdef _D5000
	KEY_STRU  buf;
	buf.table_no = table_no;
	buf.key = rec_id;
	buf.field_id = 0;
	memcpy(&devid, &buf, sizeof(INT_REC));
#else
	devid = table_no * DB_TABLE_BASE + rec_id;
#endif // _D5000
#endif // _NUSP	
	return;
}

//改变记录号中的表号(区域号、记录号不变)
INT_REC CPubOdbApi::PODB_ChangeDevId(const int table_no, const INT_REC rec_id)
{
	INT_REC devid;
	ZERO_INT_REC(devid);
	int tableno = 0;
	int recid = 0;
	int columnid = 0;
#ifdef _NUSP
	devid.create_key(table_no, rec_id.GetAreaSerialNo(), 0);
#else
#ifdef _D5000
	KEY_STRU  buf;
	memcpy(&buf, &rec_id, sizeof(KEY_STRU));
	buf.table_no = table_no;
	buf.field_id = 0;
	memcpy(&devid, &buf, sizeof(INT_REC));

#else
	tableno = rec_id / DB_TABLE_BASE;
	recid = rec_id - table_no * DB_TABLE_BASE;
	devid = table_no * DB_TABLE_BASE + recid;

#endif
#endif // _NUSP

	return devid;
}

INT_REC CPubOdbApi::PODB_GetDevIdByKeyID(const int& table_no, const int& rec_id)
{
	INT_REC devid;
	ZERO_INT_REC(devid);
#ifdef _NUSP
	int column_no = 0;
	ARA_SER_NO_T area_serial_no = CUR_AREA_NO;
	area_serial_no = area_serial_no * 100000000 + rec_id;
	devid.create_key(table_no, area_serial_no, column_no);
#else
	#ifdef _D5000
		KEY_STRU  buf;
		buf.table_no = table_no;
		buf.key = rec_id;
		buf.field_id = 0;
		memcpy(&devid, &buf, sizeof(INT_REC));
	#else
		devid = table_no * DB_TABLE_BASE + rec_id;
	#endif // _D5000
#endif // _NUSP	
	return devid;
}

INT_REC CPubOdbApi::PODB_GetDevId(const int table_no, const int rec_id, const int field_id)
{
	INT_REC devid;
	ZERO_INT_REC(devid);
#ifdef _NUSP	
	int column_no = field_id;
	ARA_SER_NO_T area_serial_no = CUR_AREA_NO;
	area_serial_no = area_serial_no * 100000000 + rec_id;
	devid.create_key(table_no, area_serial_no, column_no);
#else
	#ifdef _D5000
		KEY_STRU  buf;
		buf.table_no = table_no;
		buf.key = rec_id;
		buf.field_id = field_id;
		memcpy(&devid, &buf, sizeof(INT_REC));
	#else
		devid = table_no * DB_TABLE_BASE + rec_id;
	#endif // _D5000
#endif // _NUSP	
	return devid;
}

//获取系统主目录
void CPubOdbApi::PODB_GetHomePath(string& home_path)
{
	char* ptr_env = NULL;
	char home_str[256];

	home_path = "";

	ptr_env = getenv("OPEN2000E_HOME");

	if (ptr_env == NULL)
	{
		printf("=== Err: genenv(OPEN2000E_HOME) ===");
		return;
	}
	else
	{
		printf("=== HOME_PATH = %s\n", ptr_env);
	}

	memset(home_str, 0, sizeof(home_str));

	strcpy(home_str, ptr_env);

#ifndef _WINDOWS32
	home_path = string(home_str) + string("/");
#else
	home_path = string(home_str) + string("\\");
#endif // !_WINDOWS32

	return;
}

//获取Sys目录
void CPubOdbApi::PODB_GetSysPath(string& sys_path)
{
	string home_path;

	PODB_GetHomePath(home_path);

	sys_path = "";

	if (!home_path.empty())
	{
#ifdef _D5000

#ifndef _WINDOWS32
		sys_path = home_path + string("conf/");
#else
		sys_path = home_path + string("conf\\");
#endif // !_WINDOWS32

#else

#ifndef _WINDOWS32
		sys_path = home_path + string("sys/");
#else
		sys_path = home_path + string("sys\\");
#endif // !_WINDOWS32

#endif // _D5000
	}

	return;
}

//计算某数的N次方
int CPubOdbApi::PODB_GetVarNsquare(int var, int n)
{
	int i, nsquare;
	nsquare = 1;

	for (i = 0; i < n; i++)
	{
		nsquare *= var;
	}

	return nsquare;
}

string CPubOdbApi::PODB_GetYcName(const int nVIP)
{
	string ColName;
	// UAV UAA UBV UBA UCV UCA IAV IAA IBV IBA ICB ICA PWR QWR ANG ANC EV EA F
	switch (nVIP)
	{
	case 0:
		ColName = "UAV";
		break;
	case 1:
		ColName = "UAA";
		break;
	case 2:
		ColName = "UBV";
		break;
	case 3:
		ColName = "UBA";
		break;
	case 4:
		ColName = "UCV";
		break;
	case 5:
		ColName = "UCA";
		break;
	case 6:
		ColName = "IAV";
		break;
	case 7:
		ColName = "IAA";
		break;
	case 8:
		ColName = "IBV";
		break;
	case 9:
		ColName = "IBA";
		break;
	case 10:
		ColName = "ICV";
		break;
	case 11:
		ColName = "ICA";
		break;
	case 12:
		ColName = "PWR";
		break;
	case 13:
		ColName = "QWR";
		break;
	case 14:
		ColName = "ANG";
		break;
	case 15:
		ColName = "ANC";
		break;
	case 16:
		ColName = "EV";
		break;
	case 17:
		ColName = "EA";
		break;
	case 18:
		ColName = "F";
		break;
	default:
		ColName = "ERR";
		break;
	}

	return ColName;
}
//////////////////////////////////////////////////////////////////////////
// 消息封装
#ifdef _PODB_MSGBUS_
#ifdef _D5000
// D5000下消息为结构体,初始化需要context_name(态名),app_name(应用名),ProcName(进程名)
int CPubOdbApi::PODB_messageInit(char* context_name, char* app_name, char* proc_name)
{
	int ret_code = 0;
	ret_code = m_MsgBus.messageInit(context_name, app_name, proc_name);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "初始化消息失败[%s][%s][%s][%d]", context_name, app_name, proc_name, ret_code);
		MessShow(m_LogString, LTERROR);
	}
	return ret_code;
}
int CPubOdbApi::PODB_messageExit(int proc_key)
{
	return (m_MsgBus.messageExit(proc_key));
}
int CPubOdbApi::PODB_messageSend(Message* messageP, int messageLength, Msg_destination* msg_dst_p)
{
	int ret_code = m_MsgBus.messageSend(messageP, messageLength, msg_dst_p);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "消息发送失败");
		MessShow(m_LogString, LTERROR);
	}
	return ret_code;
}
int CPubOdbApi::PODB_messageReceive(Message* messageP, Msg_source* msg_src_p, int sync)
{
	int ret_code = m_MsgBus.messageReceive(messageP, msg_src_p, sync);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "消息接收失败");
		MessShow(m_LogString, LTERROR);
	}
	return ret_code;
}
int CPubOdbApi::PODB_messageSubscribe(short set_id, char* context_name)
{
	int ret_code = m_MsgBus.messageSubscribe(set_id, context_name);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "消息订阅失败");
		MessShow(m_LogString, LTERROR);
	}
	return ret_code;
}
int CPubOdbApi::PODB_messageUnSubscribe(short set_id, char* context_name)
{
	int ret_code = m_MsgBus.messageUnSubscribe(set_id, context_name);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "消息退订失败");
		MessShow(m_LogString, LTERROR);
	}
	return ret_code;
}

#else  // #ifdef _D5000
// OPEN3000下消息为指针,初始化需要ProcName(进程名),ThreadName(线程名)
int CPubOdbApi::PODB_InitMsgBus(const string proc_name, const string thread_name, const int persist, const int context, const int system)
{
	int ret_code = 0;
	if (m_MsgBus)
	{
		m_MsgBus->release();
		m_MsgBus = NULL;
	}
	CMsgBusFactory* m_pMsgBusFactory = NULL;
	getmsgbusfactory(&m_pMsgBusFactory);
	if (m_pMsgBusFactory == NULL)
	{
		sprintf(m_LogString, "初始化MsgBusFactory失败");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	m_MsgBus = m_pMsgBusFactory->MsgBusFactory(proc_name.c_str(), thread_name.c_str(), persist, context, system);
	if (m_MsgBus == NULL)
	{
		sprintf(m_LogString, "初始化消息实例失败");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	m_pMsgBusFactory->release();
	m_pMsgBusFactory = NULL;
	return ret_code;
}

int CPubOdbApi::PODB_ReleaseMsgBus()
{
	if (m_MsgBus)
	{
		m_MsgBus->release();
		m_MsgBus = NULL;
	}
	return 0;
}

int CPubOdbApi::PODB_SendMessageToHost(const char* message, const int msg_type, const int length, const char* host_name, const char* process_name, const char* instance_name)
{
	if (m_MsgBus == NULL)
	{
		sprintf(m_LogString, "消息未初始化");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int ret_code = m_MsgBus->SendMessageToHost(message, msg_type, length, host_name, instance_name);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "消息发送失败,host[%s],msgtype[%d],process[%s]", host_name, msg_type, process_name);
		MessShow(m_LogString, LTERROR);
	}
	return ret_code;
}

int CPubOdbApi::PODB_SendMessageToChannel(const char* message, const int msg_type, const int length, const int channel_id, const char* process_name, const char* instance_name)
{
	if (m_MsgBus == NULL)
	{
		sprintf(m_LogString, "消息未初始化");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int ret_code = m_MsgBus->SendMessageToChannel(message, msg_type, length, channel_id, process_name);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "消息发送失败,channel[%d],msgtype[%d],process[%s]", channel_id, msg_type, process_name);
		MessShow(m_LogString, LTERROR);
	}
	return ret_code;
}

int CPubOdbApi::PODB_JoinChannel(const int channel_id)
{
	if (m_MsgBus == NULL)
	{
		sprintf(m_LogString, "消息未初始化");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	int ret_code = m_MsgBus->JoinChannel(channel_id);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "加入通道号失败[%d]", channel_id);
		MessShow(m_LogString, LTERROR);
	}
	return ret_code;
}

int CPubOdbApi::PODB_RecvMessage(struct MESSAGE_HEADER* msg_header, ::CBuffer& buf_base)
{
	if (m_MsgBus == NULL)
	{
		sprintf(m_LogString, "消息未初始化");
		MessShow(m_LogString, LTERROR);
		return -1;
	}

	return (m_MsgBus->RecvMessage(msg_header, buf_base));
}
#endif // _D5000
#endif  // _PODB_MSGBUS_
//////////////////////////////////////////////////////////////////////////
#ifdef _PODB_SAMPLE_
int CPubOdbApi::PODB_ClearSample(vector<TRIGGER_SAMPLE_DEFINE>& vectrisample)
{
	if (!vectrisample.empty())
	{
		int ii;
		for (ii = 0; ii < (int)vectrisample.size(); ii++)
		{
			PODB_ClearSample(vectrisample[ii]);
		}
		SAFE_SWAP(vectrisample, TRIGGER_SAMPLE_DEFINE);
	}
	return 0;
}

int CPubOdbApi::PODB_ClearSample(TRIGGER_SAMPLE_DEFINE& trisample)
{
	trisample.tableno = 0;
	trisample.sampleno = 0;
	trisample.mode = 0;
	trisample.appno = 0;
	if (!trisample.vecsmp.empty())
	{
		SAFE_SWAP(trisample.vecsmp, TRIGGER_SAMPLE_BASE);
	}
	return 0;
}
// col_id 域内部ID号
int CPubOdbApi::PODB_SmpBaseINT(TRIGGER_SAMPLE_DEFINE& tmpSample, const int col_id, const INT_REC vint)
{
	TRIGGER_SAMPLE_BASE tmpBase;
	memset((void*)&tmpBase, 0, sizeof(TRIGGER_SAMPLE_BASE));
	tmpBase.keyid = PODB_GetDevId(tmpSample.tableno, tmpSample.mode, col_id);
	tmpBase.record_id = PODB_GetDevIdByKeyID(tmpSample.tableno, tmpSample.mode);
	tmpBase.column_id = col_id;
	tmpBase.vtype = PODB_DATATYPE_INT;
	tmpBase.vint = vint;
	tmpSample.vecsmp.push_back(tmpBase);
	return 0;
}
int CPubOdbApi::PODB_SmpBaseFLT(TRIGGER_SAMPLE_DEFINE& tmpSample, const int col_id, const float vfloat)
{
	TRIGGER_SAMPLE_BASE tmpBase;
	memset((void*)&tmpBase, 0, sizeof(TRIGGER_SAMPLE_BASE));
	tmpBase.keyid = PODB_GetDevId(tmpSample.tableno, tmpSample.mode, col_id);
	tmpBase.record_id = PODB_GetDevIdByKeyID(tmpSample.tableno, tmpSample.mode);
	tmpBase.column_id = col_id;
	tmpBase.vtype = PODB_DATATYPE_FLOAT;
	tmpBase.vfloat = vfloat;
	tmpSample.vecsmp.push_back(tmpBase);
	return 0;
}
int CPubOdbApi::PODB_SmpBaseSTR(TRIGGER_SAMPLE_DEFINE& tmpSample, const int col_id, const string vstring)
{
	TRIGGER_SAMPLE_BASE tmpBase;
	memset((void*)&tmpBase, 0, sizeof(TRIGGER_SAMPLE_BASE));
	tmpBase.keyid = PODB_GetDevId(tmpSample.tableno, tmpSample.mode, col_id);
	tmpBase.record_id = PODB_GetDevIdByKeyID(tmpSample.tableno, tmpSample.mode);
	tmpBase.column_id = col_id;
	tmpBase.vtype = PODB_DATATYPE_STRING;
	if (vstring.length() < 128)
	{
		sprintf(tmpBase.vstring, "%s", vstring.c_str());
	}
	else
	{
		// 采样只能保存128位字符,保留末尾截止符,截取前127位
		sprintf(tmpBase.vstring, "%s", vstring.substr(0, 126).c_str());
	}
	tmpSample.vecsmp.push_back(tmpBase);
	return 0;
}
int CPubOdbApi::PODB_TriggerSample(vector<TRIGGER_SAMPLE_DEFINE>& vectrisample)
{
	int ret_code = 0, ii = 0, nNum = 0, nFlag = 0;
	if (vectrisample.empty())
	{
		sprintf(m_LogString, "采样记录数为空[%d]", (int)vectrisample.size());
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		// 检查参数
		nFlag = 0;
		nNum = (int)vectrisample.front().vecsmp.size();
		for (ii = 0; ii < (int)vectrisample.size(); ii++)
		{
			if (vectrisample[ii].tableno <= 0)
			{
				nFlag++;
				sprintf(m_LogString, "采样表号错误[%d][%d]", ii + 1, vectrisample[ii].tableno);
				MessShow(m_LogString, LTERROR);
			}

			if (vectrisample[ii].sampleno <= 0)
			{
				nFlag++;
				sprintf(m_LogString, "采样号错误[%d][%d]", ii + 1, vectrisample[ii].sampleno);
				MessShow(m_LogString, LTERROR);
			}

			if (vectrisample[ii].appno <= 0)
			{
				nFlag++;
				sprintf(m_LogString, "采样应用号错误[%d][%d]", ii + 1, vectrisample[ii].appno);
				MessShow(m_LogString, LTERROR);
			}

			if (vectrisample[ii].mode != 0 && vectrisample[ii].mode != 1)
			{
				nFlag++;
				sprintf(m_LogString, "域级或非域级采样类型错误[%d][%d]", ii + 1, vectrisample[ii].mode);
				MessShow(m_LogString, LTERROR);
			}

			if (vectrisample[ii].occur_time < 0)
			{
				nFlag++;
				sprintf(m_LogString, "采样时间错误[%d][%d]", ii + 1, vectrisample[ii].occur_time);
				MessShow(m_LogString, LTERROR);
			}

			if ((int)vectrisample[ii].vecsmp.size() != nNum)
			{
				nFlag++;
				sprintf(m_LogString, "采样域个数错误[%d][%d]", ii + 1, nNum);
				MessShow(m_LogString, LTERROR);
			}
		}
	}

	if (nFlag > 0)
	{
		return -1;
	}

#ifdef _D5000
	int jj;
	tmptsmp.length((int)vectrisample.size());
	for (ii = 0; ii < (int)vectrisample.size(); ii++)
	{
		tmptsmp[ii].sample_type = vectrisample[ii].sampleno;
		tmptsmp[ii].app_no = vectrisample[ii].appno;
		tmptsmp[ii].sample_mode = OFFERDATA_SAMPLE_MODE;
		tmptsmp[ii].occur_time = vectrisample[ii].occur_time;
		tmptsmp[ii].keyid_num = (int)vectrisample[ii].vecsmp.size();
		tmptsmp[ii].flag = 0;
		tmptsmp[ii].seq_keyid_value.length(tmptsmp[ii].keyid_num);
		for (jj = 0; jj < (int)vectrisample[ii].vecsmp.size(); jj++)
		{
			tmptsmp[ii].seq_keyid_value[jj].keyid = vectrisample[ii].vecsmp[jj].keyid;
			tmptsmp[ii].seq_keyid_value[jj].keyid_datatype = vectrisample[ii].vecsmp[jj].vtype;
			if (vectrisample[ii].vecsmp[jj].vtype == PODB_DATATYPE_INT)
			{
				// 整形
				tmptsmp[ii].seq_keyid_value[jj].keyid_value.c_long(vectrisample[ii].vecsmp[jj].vint);
			}
			else if (vectrisample[ii].vecsmp[jj].vtype == PODB_DATATYPE_FLOAT)
			{
				// 浮点型
				tmptsmp[ii].seq_keyid_value[jj].keyid_value.c_float(vectrisample[ii].vecsmp[jj].vfloat);
			}
			else if (vectrisample[ii].vecsmp[jj].vtype == PODB_DATATYPE_STRING)
			{
				// 字符串
				tmptsmp[ii].seq_keyid_value[jj].keyid_value.c_string(vectrisample[ii].vecsmp[jj].vstring);
			}
			else
			{
				// 值类型错误
				sprintf(m_LogString, "采样值类型错误[%d]", vectrisample[ii].vecsmp[jj].vtype);
				MessShow(m_LogString, LTERROR);
			}
		}
	}

	ret_code = tri_client.PutTriggerSampleOneWay(tmptsmp);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "采样发送失败[%d][%d]", ret_code, (int)vectrisample.size());
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "采样发送成功[%d][%d]", ret_code, (int)vectrisample.size());
		MessShow(m_LogString, LTINFO);
	}
#else
	TSEQSampleProcessRequest  tmptsmp;
	int jj, tmpappno;
	tmptsmp.seq_length = (int)vectrisample.size();
	tmptsmp.seq_sample_process_request.length((int)vectrisample.size());
	for (ii = 0; ii < (int)vectrisample.size(); ii++)
	{
		tmptsmp.seq_sample_process_request[ii].sample_type = vectrisample[ii].sampleno;
		tmptsmp.seq_sample_process_request[ii].app_no = vectrisample[ii].appno;
		tmpappno = vectrisample[ii].appno;
		tmptsmp.seq_sample_process_request[ii].sample_mode = OFFERDATA_SAMPLE_MODE;
		tmptsmp.seq_sample_process_request[ii].occur_time = vectrisample[ii].occur_time;
		tmptsmp.seq_sample_process_request[ii].keyid_num = (int)vectrisample[ii].vecsmp.size();
		tmptsmp.seq_sample_process_request[ii].flag = 0;
		tmptsmp.seq_sample_process_request[ii].reserve_1 = 0;
		tmptsmp.seq_sample_process_request[ii].reserve_2 = 0;
		tmptsmp.seq_sample_process_request[ii].seq_keyid_value.length((int)vectrisample[ii].vecsmp.size());
		for (jj = 0; jj < (int)vectrisample[ii].vecsmp.size(); jj++)
		{
			tmptsmp.seq_sample_process_request[ii].seq_keyid_value[jj].keyid.record_id = vectrisample[ii].vecsmp[jj].record_id;
			tmptsmp.seq_sample_process_request[ii].seq_keyid_value[jj].keyid.column_id = vectrisample[ii].vecsmp[jj].column_id;
			tmptsmp.seq_sample_process_request[ii].seq_keyid_value[jj].keyid_datatype = vectrisample[ii].vecsmp[jj].vtype;
			if (vectrisample[ii].vecsmp[jj].vtype == PODB_DATATYPE_INT)
			{
				// 整形
				tmptsmp.seq_sample_process_request[ii].seq_keyid_value[jj].keyid_value.c_int(vectrisample[ii].vecsmp[jj].vint);
			}
			else if (vectrisample[ii].vecsmp[jj].vtype == PODB_DATATYPE_FLOAT)
			{
				// 浮点型
				tmptsmp.seq_sample_process_request[ii].seq_keyid_value[jj].keyid_value.c_float(vectrisample[ii].vecsmp[jj].vfloat);
			}
			else if (vectrisample[ii].vecsmp[jj].vtype == PODB_DATATYPE_STRING)
			{
				// 字符串
				tmptsmp.seq_sample_process_request[ii].seq_keyid_value[jj].keyid_value.c_string((const ::CORBA::String_var)CORBA::string_dup(vectrisample[ii].vecsmp[jj].vstring));
			}
			else
			{
				// 值类型错误
				sprintf(m_LogString, "采样值类型错误[%d]", vectrisample[ii].vecsmp[jj].vtype);
				MessShow(m_LogString, LTERROR);
			}
		}
	}

	// 获取主机名
	string tmphostname;
	if (PODB_GetHostName(tmphostname, tmpappno) < 0)
	{
		return -1;
	}

	char* msg_buf = NULL;
	int msg_size = 0;
	MARSHAL(tmptsmp, msg_buf, msg_size);
	ret_code = PODB_SendMessageToHost(msg_buf, DB_TRIGGER_SAMPLE_TYPE, msg_size, tmphostname.c_str(), "powoscana", "TRIGGER_SAMPLE_SERVER");
	if (ret_code < 0)
	{
		sprintf(m_LogString, "采样发送失败[%s],msgtype[%d]", tmphostname.c_str(), DB_TRIGGER_SAMPLE_TYPE);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "采样发送成功[%s],msgtype[%d]", tmphostname.c_str(), DB_TRIGGER_SAMPLE_TYPE);
		MessShow(m_LogString, LTINFO);
	}
	SAFE_DELETE(msg_buf);
#endif

	return ret_code;
}

int CPubOdbApi::PODB_TriggerSample(TRIGGER_SAMPLE_DEFINE& trisample)
{
	return 0;
}

#endif  // _PODB_SAMPLE_
//////////////////////////////////////////////////////////////////////////
#ifdef _PODB_WARN_
int CPubOdbApi::PODB_ClearWarnMsg(vector<WARN_MESSAGE_STRUCT>& tmpVecWarnMsg)
{
	if (!tmpVecWarnMsg.empty())
	{
		int ii;
		for (ii = 0; ii < (int)tmpVecWarnMsg.size(); ii++)
		{
			PODB_ClearWarnMsg(tmpVecWarnMsg[ii]);
		}
		SAFE_SWAP(tmpVecWarnMsg, WARN_MESSAGE_STRUCT);
	}
	return 0;
}

int CPubOdbApi::PODB_ClearWarnMsg(WARN_MESSAGE_STRUCT& tmpWarnMsg)
{
	tmpWarnMsg.appno = 0;
	tmpWarnMsg.warntype = 0;
	if (!tmpWarnMsg.vecwarn.empty())
	{
		SAFE_SWAP(tmpWarnMsg.vecwarn, WARN_MESSAGE_BASE);
	}
	return 0;
}

int CPubOdbApi::PODB_WarnMsgBaseINT(WARN_MESSAGE_STRUCT& tmpWarnMsg, const int vint)
{
	WARN_MESSAGE_BASE tmpWarnBase;
	memset((void*)&tmpWarnBase, 0, sizeof(WARN_MESSAGE_BASE));
	tmpWarnBase.vtype = PODB_DATATYPE_INT;
	tmpWarnBase.vint = vint;
	tmpWarnMsg.vecwarn.push_back(tmpWarnBase);
	return 0;
}

int CPubOdbApi::PODB_WarnMsgBaseTime(WARN_MESSAGE_STRUCT& tmpWarnMsg, const RTDB_TIME vtime)
{
	WARN_MESSAGE_BASE tmpWarnBase;
	memset((void*)&tmpWarnBase, 0, sizeof(WARN_MESSAGE_BASE));
	tmpWarnBase.vtype = PODB_DATATYPE_TIME;
	tmpWarnBase.vtime = vtime;
	tmpWarnMsg.vecwarn.push_back(tmpWarnBase);
	return 0;
}

#ifdef _D5000
int CPubOdbApi::PODB_WarnMsgBaseLONG(WARN_MESSAGE_STRUCT& tmpWarnMsg, const INT_REC vlong)
{
	WARN_MESSAGE_BASE tmpWarnBase;
	memset((void*)&tmpWarnBase, 0, sizeof(WARN_MESSAGE_BASE));
	tmpWarnBase.vtype = PODB_DATATYPE_LONG;
	tmpWarnBase.vlong = vlong;
	tmpWarnMsg.vecwarn.push_back(tmpWarnBase);
	return 0;
}
#endif

int CPubOdbApi::PODB_WarnMsgBaseSTRING(WARN_MESSAGE_STRUCT& tmpWarnMsg, const string vstring)
{
	WARN_MESSAGE_BASE tmpWarnBase;
	memset((void*)&tmpWarnBase, 0, sizeof(WARN_MESSAGE_BASE));
	tmpWarnBase.vtype = PODB_DATATYPE_STRING;
	if (vstring.length() < 200)
	{
		sprintf(tmpWarnBase.vstring, "%s", vstring.c_str());
	}
	else
	{
		// 告警只能保存200位字符,保留末尾截止符,截取前199位
		sprintf(tmpWarnBase.vstring, "%s", vstring.substr(0, 198).c_str());
	}
	tmpWarnMsg.vecwarn.push_back(tmpWarnBase);
	return 0;
}

int CPubOdbApi::PODB_SendWarnMessage(vector<WARN_MESSAGE_STRUCT>& tmpVecWarnMsg, const int appno, const int contextno)
{
	// appno指定发送告警信息的应用主机
	// tmpVecWarnMsg[ii].appno指定告警定义所属应用
	int ret_code = 0, ii = 0, nNum = 0, nFlag = 0;
	if (tmpVecWarnMsg.empty())
	{
		sprintf(m_LogString, "告警信息个数为空");
		MessShow(m_LogString, LTERROR);
		return -1;
	}
	else
	{
		// 检查告警信息
		nFlag = 0;
		nNum = (int)tmpVecWarnMsg.front().vecwarn.size();
		for (ii = 0; ii < (int)tmpVecWarnMsg.size(); ii++)
		{
			if (tmpVecWarnMsg[ii].warntype < 0)
			{
				nFlag++;
				sprintf(m_LogString, "告警类型错误[%d][%d]", ii + 1, tmpVecWarnMsg[ii].warntype);
				MessShow(m_LogString, LTERROR);
			}

			if (tmpVecWarnMsg[ii].appno < 0)
			{
				nFlag++;
				sprintf(m_LogString, "告警应用号错误[%d][%d]", ii + 1, tmpVecWarnMsg[ii].appno);
				MessShow(m_LogString, LTERROR);
			}

			if (tmpVecWarnMsg[ii].vecwarn.size() != nNum)
			{
				nFlag++;
				sprintf(m_LogString, "告警域个数错误[%d][%d]", ii + 1, nNum);
				MessShow(m_LogString, LTERROR);
			}
		}
	}

	if (nFlag > 0)
	{
		return -1;
	}

#ifdef _WAMAP_
	int jj;
	APP_TO_WARN_SERVICE_MESSAGE_STRU tmpMsg;
	tmpMsg.warn_num = (int)tmpVecWarnMsg.size();
	tmpMsg.seq_warn_message.length(tmpMsg.warn_num);
	for (ii = 0; ii < (int)tmpVecWarnMsg.size(); ii++)
	{
		tmpMsg.seq_warn_message[ii].warn_type = tmpVecWarnMsg[ii].warntype;
		tmpMsg.seq_warn_message[ii].app_no = tmpVecWarnMsg[ii].appno;
		tmpMsg.seq_warn_message[ii].node_id = 1;
		tmpMsg.seq_warn_message[ii].op_type = 0;
		tmpMsg.seq_warn_message[ii].is_restrain = 0;
		//tmpMsg.seq_warn_message[ii].sound_table_key_field_order_no = -1;
		tmpMsg.seq_warn_message[ii].reservered_1 = 0;
		tmpMsg.seq_warn_message[ii].reservered_2 = 0;
		tmpMsg.seq_warn_message[ii].seq_field_info.length((int)tmpVecWarnMsg[ii].vecwarn.size());

		for (jj = 0; jj < (int)tmpVecWarnMsg[ii].vecwarn.size(); jj++)
		{
			if (tmpVecWarnMsg[ii].vecwarn[jj].vtype == PODB_DATATYPE_TIME)
			{
				tmpMsg.seq_warn_message[ii].seq_field_info[jj].c_time(tmpVecWarnMsg[ii].vecwarn[jj].vtime);
			}
#ifdef _D5000
			else if (tmpVecWarnMsg[ii].vecwarn[jj].vtype == PODB_DATATYPE_LONG)
			{
				tmpMsg.seq_warn_message[ii].seq_field_info[jj].c_long(tmpVecWarnMsg[ii].vecwarn[jj].vlong);
			}
#endif
			else if (tmpVecWarnMsg[ii].vecwarn[jj].vtype == PODB_DATATYPE_INT)
			{
				tmpMsg.seq_warn_message[ii].seq_field_info[jj].c_int(tmpVecWarnMsg[ii].vecwarn[jj].vint);
			}
			else if (tmpVecWarnMsg[ii].vecwarn[jj].vtype == PODB_DATATYPE_STRING)
			{
				tmpMsg.seq_warn_message[ii].seq_field_info[jj].c_string(tmpVecWarnMsg[ii].vecwarn[jj].vstring);
			}
			else
			{
				// 值类型错误
				sprintf(m_LogString, "值类型错误[%d]", tmpVecWarnMsg[ii].vecwarn[jj].vtype);
				MessShow(m_LogString, LTERROR);
			}
		}
	}

	string hostname;
	if (PODB_GetHostName(hostname, appno, contextno) != 1)
	{
		return 0;
	}
	// 	sprintf(m_LogString,"hostname=[%s]",hostname);
	// 		MessShow(m_LogString,LTERROR);	
	char* msg_buf = NULL;
	int buf_size = 0;
#ifdef _D5000
	Message msg;
	Msg_destination* pp_tcp = new Msg_destination;
	M_CODE(tmpMsg, msg_buf, buf_size);
	msg.header.serv = APP_TO_WARN_CHANNEL;
	msg.header.event = APP_TO_WARN_SERVICE_TYPE;
	memcpy(msg.Msg_buf, msg_buf, buf_size);
	if (buf_size > MAX_MSGBUF_LEN)
	{
		sprintf(m_LogString, "告警消息长度越限[%d]", buf_size);
		MessShow(m_LogString, LTERROR);
		SAFE_DELETE(msg_buf);
		return -1;
	}
	memset(pp_tcp, 0, sizeof(Msg_destination));
	strcpy(pp_tcp->host_name, hostname.c_str());
	//strcpy(pp_tcp->context_name,AC_REALTIME_NAME);
	//strcpy(pp_tcp->app_name,AF_PUBLIC_NAME);
	//strcpy(pp_tcp->proc_name,"warnserver");
	pp_tcp->status = 0;
	//pp_tcp->sock = 0;
	//ret_code = PODB_messageSend((Message  *)&msg, buf_size, pp_tcp);
	ret_code = PODB_messageSend((Message*)&msg, buf_size, pp_tcp);
	SAFE_DELETE(pp_tcp);
	SAFE_DELETE(msg_buf);
	if (ret_code <= 0)
	{
		sprintf(m_LogString, "告警发送失败[%s],msgtype[%d],channel=[%d],size=[%d]", hostname.c_str(), APP_TO_WARN_SERVICE_TYPE, APP_TO_WARN_CHANNEL, buf_size);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "告警发送成功[%s],msgtype[%d],channel=[%d],size=[%d]", hostname.c_str(), APP_TO_WARN_SERVICE_TYPE, APP_TO_WARN_CHANNEL, buf_size);
		MessShow(m_LogString, LTINFO);
	}
#else
	MARSHAL(tmpMsg, msg_buf, buf_size);
	if (buf_size > MAX_MSGBUF_LEN)
	{
		sprintf(m_LogString, "告警消息长度越限[%d]", buf_size);
		MessShow(m_LogString, LTERROR);
		SAFE_DELETE(msg_buf);
		return -1;
	}
	ret_code = PODB_SendMessageToHost(msg_buf, APP_TO_WARN_SERVICE_TYPE, buf_size, hostname.c_str(), "powoscana", "WARN_SERVICE");
	SAFE_DELETE(msg_buf);
	if (ret_code < 0)
	{
		sprintf(m_LogString, "告警发送失败[%s],msgtype[%d]", hostname.c_str(), APP_TO_WARN_SERVICE_TYPE);
		MessShow(m_LogString, LTERROR);
	}
	else
	{
		sprintf(m_LogString, "告警发送成功[%s],msgtype[%d]", hostname.c_str(), APP_TO_WARN_SERVICE_TYPE);
		MessShow(m_LogString, LTINFO);
	}
#endif  // _D5000
#endif  // _WAMAP_
	return ret_code;
}
#endif _PODB_WARN_

