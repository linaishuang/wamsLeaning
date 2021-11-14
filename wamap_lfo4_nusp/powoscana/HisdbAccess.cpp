
#include <system.h>
#include "HisdbAccess.h"
//#include "FastDataMan.h"

//portno端口号，仅D5000有效
#ifndef _PCS9000
HisdbAccess::HisdbAccess(int portno, bool bdebug)
{
	// printf("HisdbAccess构造开始\n");
#ifdef _D5000
    if( portno>0 )
	    sql_sp_client = new sql_sp_client_base( portno );
	else
	    sql_sp_client = new sql_sp_client_base();
#else
	vector<T_Corba_Props> props_all(3);	
	strcpy(props_all[0].prop_name , "ooc.orb.trace.connections");
	strcpy(props_all[0].prop_value, "2");
	strcpy(props_all[1].prop_name , "ooc.orb.policy.connect_timeout");
	strcpy(props_all[1].prop_value, "2000");
	strcpy(props_all[2].prop_name , "ooc.orb.policy.request_timeout");
	strcpy(props_all[2].prop_value, "20000");	
	sql_sp_client = new sql_sp_client_base(AP_DB_SERVICE,1,PORT_DB_SQL_SP,"sql_sp_server",props_all,AC_REALTIME_NO,"alias_sql_sp_server");
#endif

#ifdef _D5000
#ifndef _NUSP
	sql_modify_client = new modify_client_base();
#else
	//
#endif // !_NUSP    
#else
	vector<T_Corba_Props> props_all2(2);  
    strcpy(props_all2[0].prop_name , "ooc.orb.policy.connect_timeout");
    strcpy(props_all2[0].prop_value, "2000");
    strcpy(props_all2[1].prop_name , "ooc.orb.policy.request_timeout");
    strcpy(props_all2[1].prop_value, "200000");
    	
    int m_policy = 1;   
    sql_modify_client = new modify_client_base( AP_DB_SERVICE, m_policy, PORT_DB_MODIFY, "db_modify_server", props_all2, AC_REALTIME_NO);        
#endif

    isDebug = bdebug;
	m_queryerr = 0;
    table_net = new NET_ODB::CTableNet(AP_PUBLIC);
}

HisdbAccess::~HisdbAccess()
{
	if( sql_sp_client ){
		delete sql_sp_client;
	}
	if( table_net ){
	    delete table_net;
	}
#ifndef _NUSP
	if (sql_modify_client) {
		delete sql_modify_client;
	}
#else
	//
#endif // !_NUSP    

}

//执行update,delete
int HisdbAccess::ExecHisBySql( const char * sql )
{
    if( isDebug )
        printf("ExecHisBySql: %s\n", sql);
#ifdef _D5000
	SEQDBErrorStru seq_db_err;
	short ret = sql_sp_client->ExecuteSql( sql, seq_db_err);
#else
	SEQDBErrorStru_var seq_db_err;
	short ret = sql_sp_client->ExecuteSqlEx( sql, seq_db_err, 1);
#endif
	
	if( ret == 0 )
	{
		return ret;
	}
	else
	{
		m_queryerr++;
		return -1;
	}
}

//查询 返回个数
int HisdbAccess::QueryHisBySql(const char * sql )
{
    if( isDebug )
        printf("QueryHisBySql: %s\n", sql);
	SEQOutDataTypeStru out_data_type;
#ifdef _D5000
	SEQDBErrorStru seq_db_err;
	TSelectResultStru result_ptr;
	short ret = sql_sp_client->SelectSql( sql, QUERY_ALL_RESULT, out_data_type, result_ptr, seq_db_err);
	if( ret == 0 )
	{
		printf("result_ptr.data_num=%d\n", result_ptr.data_num);
		return result_ptr.data_num;
	}
#else
	SEQDBErrorStru_var seq_db_err;
	TSelectResultStru_var result_ptr;
	short ret = sql_sp_client->SelectSql( sql, QUERY_ALL_RESULT, out_data_type, result_ptr, seq_db_err);
	if( ret == 0 )
	{
		printf("result_ptr.data_num=%d\n", result_ptr->data_num);
		return result_ptr->data_num;
	}
#endif
	else
	{
		m_queryerr++;
		return -1;
	}
}

//查询并返回数据
int HisdbAccess::QueryHisBySql(const char * sql,  int structlen, CBuffer &buf)
{
    if( isDebug )
        printf("SelectSql_2: %s\n", sql);
	int record_num=0, field_num=0;
	char * buf_tmp=NULL;

	SEQOutDataTypeStru out_data_type;
	CSqlResultAlignClient sql_align; //用于数据对齐
	VIndicator null_vec;
#ifdef _D5000
	SEQDBErrorStru seq_db_err;
	TSelectResultStru result_ptr;
	short ret = sql_sp_client->SelectSql( sql, QUERY_ALL_RESULT, out_data_type, result_ptr, seq_db_err);
	if( ret == 0 )
	{
		record_num = result_ptr.data_num;
		field_num = result_ptr.field_num;
		if( record_num > 0 )
		{
			buf_tmp = new char[ record_num * structlen ];
			ret = sql_align.GetAlignResultClient(result_ptr.field_info, result_ptr.data_value_seq, null_vec,
				buf_tmp,  structlen
				) ; 
			buf.SetBuffer( buf_tmp, record_num * structlen);
			delete []buf_tmp;
		}
		printf(">> record_num=%d\n", record_num);
		return record_num; //返回记录数
	}
#else
	SEQDBErrorStru_var seq_db_err;
	TSelectResultStru_var result_ptr;
	//查询
	short ret = sql_sp_client->SelectSql( sql, QUERY_ALL_RESULT, out_data_type, result_ptr, seq_db_err);
	if( ret == 0 )
	{
		record_num = result_ptr->data_num;
		field_num = result_ptr->field_num;
		if( record_num > 0 )
		{
			buf_tmp = new char[ record_num * structlen ];
			ret = sql_align.GetAlignResultClient(result_ptr->field_info, result_ptr->data_value_seq, null_vec,
				buf_tmp,  structlen
				) ; 
			buf.SetBuffer( buf_tmp, record_num * structlen);
			delete []buf_tmp;
		}
		printf(">> record_num=%d\n", record_num);
		return record_num; //返回记录数
	}
#endif
	else
	{
		m_queryerr++;
		return -1; //失败
	}
}

//查询并返回数据
int HisdbAccess::QueryHisBySql(const char * sql,  int structlen, char ** buf)
{
    if( isDebug )
        printf("SelectSql_3: %s\n", sql);
	int record_num=0, field_num=0;

	SEQOutDataTypeStru out_data_type;
	CSqlResultAlignClient sql_align; //用于数据对齐
	VIndicator null_vec;
#ifdef _D5000
	SEQDBErrorStru seq_db_err;
	TSelectResultStru result_ptr;
	short ret = sql_sp_client->SelectSql( sql, QUERY_ALL_RESULT, out_data_type, result_ptr, seq_db_err);
	if( ret == 0 )
	{
		record_num = result_ptr.data_num;
		field_num = result_ptr.field_num;
		if( record_num > 0 )
		{
			*buf = new char[ record_num * structlen ];
			ret = sql_align.GetAlignResultClient(result_ptr.field_info, result_ptr.data_value_seq, null_vec,
				*buf,  structlen ); 
		}
		printf(">> record_num=%d\n", record_num);
		return record_num; //返回记录数
	}
#else
	SEQDBErrorStru_var seq_db_err;
	TSelectResultStru_var result_ptr;
	//查询
	short ret = sql_sp_client->SelectSql( sql, QUERY_ALL_RESULT, out_data_type, result_ptr, seq_db_err);
	if( ret == 0 )
	{
		record_num = result_ptr->data_num;
		field_num = result_ptr->field_num;
		if( record_num > 0 )
		{
			*buf = new char[ record_num * structlen ];
			ret = sql_align.GetAlignResultClient(result_ptr->field_info, result_ptr->data_value_seq, null_vec,
				*buf,  structlen ) ; 
		}
		printf(">> record_num=%d\n", record_num);
		return record_num; //返回记录数
	}
#endif
	else
	{
		m_queryerr++;
		return -1; //失败
	}
}

////////////////////实时库操作////////////////////////
//读取整表
int HisdbAccess::QueryRtDB(const int appid, const int tableid, const char * fieldstr, CBuffer &buf)
{
    int num = 0;
	int rt_ret = table_net->Open( appid, tableid );
	if( rt_ret != DB_OK )
	{
		printf("[error] 打开实时库失败! app=%d, table=%d\n", appid, tableid );
		return -1;
	}
	num = table_net->TableGet(fieldstr, buf);    
    #ifdef _OPEN3000
    if( num == -2435 )
        num = 0;
    #endif
    if( num < 0 ){
        printf("[error] 读取实时库失败! app=%d, table=%d, fields=%s\n", appid, tableid, fieldstr );
    }    
    return num;
}

//读取表-根据Sql语句，SQL语句不支持 in()
int HisdbAccess::QueryRtDBBySql(const int appid, const int tableid, const char * sqlstr, CBuffer &buf)
{
    int num = 0;
	int rt_ret = table_net->Open( appid, tableid );
	if( rt_ret != DB_OK )
	{
		printf("[error] 打开实时库失败! app=%d, table=%d\n", appid, tableid );
		return -1;
	}
	num = table_net->SqlGet(sqlstr, buf);    
    #ifdef _OPEN3000
    if( num == -2435 )
        num = 0;
    #endif
    if( num < 0 ){
        printf("[error] 读取实时库失败! app=%d, table=%d, sql=%s\n", appid, tableid, sqlstr );
    }
    return num;
}

//////////////////////////////////////////////////////:q
//商用库，实时库同步执行接口
//delay_ms 执行完毕后延时毫秒
#ifndef _NUSP
int HisdbAccess::ExecRHDBBySql(const char * sql, const int appid, const int delay_ms)
{
    int i_rhdb_ex_rtn=0; //返回值
    short                       corba_ret;

	TSqlModifyRequest           sql_modify_request;
	sql_modify_request.seq_sql_request.length(1); 	
    sql_modify_request.app_no = appid; 
    
#if defined(_D5000)
    SEQSqlModifyAnswer		    sql_modify_answer;
    SEQDBErrorStru          seq_db_err;
    sql_modify_request.seq_sql_request[0] = sql;
#else
    SEQSqlModifyAnswer_var      sql_modify_answer;  
    SEQDBErrorStru_var      seq_db_err;
    sql_modify_request.seq_sql_request[0] = CORBA::string_dup(sql);
#endif			 

    corba_ret = sql_modify_client->ModifyTableBySqls(sql_modify_request,sql_modify_answer);	
    if (corba_ret == 0 )   
    {
        if( isDebug )
            printf("ExecRHDBBySql: %s\n", sql);    
    }
    else 
    {
        if( isDebug )
            printf("ExecRHDBBySql: Error!! ret=%d\n", corba_ret);
	    i_rhdb_ex_rtn=-1; //命令执行返回值	
    }
    if( i_rhdb_ex_rtn >= 0 )  
    {
        if( sql_modify_answer[0].success_flag != 0)
        {
            if( isDebug )
                printf("ExecRHDBBySql: Error!! table(%d),type(%d),flag(%d),err(%s)\n", 
                    sql_modify_answer[0].table_no, sql_modify_answer[0].op_type, 
                    sql_modify_answer[0].success_flag,sql_modify_answer[0].syntax_err_msg); 
    		i_rhdb_ex_rtn = -2;
        }
        else
        {
            if( isDebug )
                printf("ExecRHDBBySql: Success!!\n");
            printf(">>>ID=%lu____\n", sql_modify_answer[0].key_value);
        }
        //print_sql_answer_report(sql_modify_answer);
    }
    //延迟
    if( delay_ms > 0 )
    {
        int ms = int( delay_ms/4.0f * 1000);
        usleep( ms ); 
        usleep( ms );
        usleep( ms ); 
        usleep( ms ); 
    }
    return i_rhdb_ex_rtn;
}
#else
	//
#endif // !_NUSP  

#endif