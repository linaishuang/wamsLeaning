//**************************************
// Generated by mcpp translator
// Version 1.2
//**************************************

#include"trigger_sample_m.h"
#include<assert.h>
USampleDataValue::USampleDataValue()
:__u_init(false),__u_index(0)
{
	memset(&__u_val,0,sizeof(__u_val));
}
USampleDataValue::~USampleDataValue()
{
	__clear();
}
USampleDataValue::USampleDataValue(const USampleDataValue&__val)
:__u_init(false),__u_index(0)
{
	*this=__val;
}
short
USampleDataValue::_d()const
{
	return __u_index;
}
USampleDataValue&
USampleDataValue::operator=(const USampleDataValue&__val)
{
	if(this == &__val)
		return *this;
	__clear();
	if(!__val.__u_init)
		return *this;
	__u_init=true;
	__u_index=__val.__u_index;
	switch(__u_index)
	{
	case(SAMPLE_DATATYPE_LONG):
	{
		__u_val.c_long=__val.__u_val.c_long;
		break;
	}
	case(SAMPLE_DATATYPE_FLOAT):
	{
		__u_val.c_float=__val.__u_val.c_float;
		break;
	}
	case(SAMPLE_DATATYPE_STRING):
	{
		__u_val.c_string=new char[strlen(__val.__u_val.c_string)+1];
		strcpy(__u_val.c_string,__val.__u_val.c_string);
		break;
	}
	default:
	{
		assert(0);
		break;
	}
	}//end switch
	return *this;
}
void
USampleDataValue::__write(MLang::OutputStream&__os)const
{
	__os.write_ushort(__u_index);
	switch(__u_index)
	{
	case(SAMPLE_DATATYPE_LONG):
	{
		__os.write_long(__u_val.c_long);
		break;
	}
	case(SAMPLE_DATATYPE_FLOAT):
	{
		__os.write_float(__u_val.c_float);
		break;
	}
	case(SAMPLE_DATATYPE_STRING):
	{
		__os.write_string(__u_val.c_string);
		break;
	}
	default:
	{
		assert(0);
		break;
	}
	}
}
void
USampleDataValue::__read(MLang::InputStream&__is)
{
	__clear();
	__u_init=true;
	__u_index=__is.read_ushort();
	switch(__u_index)
	{
	case(SAMPLE_DATATYPE_LONG):
	{
		__u_val.c_long = __is.read_long();
		break;
	}
	case(SAMPLE_DATATYPE_FLOAT):
	{
		__u_val.c_float = __is.read_float();
		break;
	}
	case(SAMPLE_DATATYPE_STRING):
	{
		const char*p_0=__is.read_string();
		__u_val.c_string=new char[strlen(p_0)+1];
		strcpy(__u_val.c_string,p_0);
		break;
	}
	default:
	{
		assert(0);
		break;
	}
	}
}
void
USampleDataValue::__clear()
{
	if(!__u_init)
		return;
	switch(__u_index)
	{
	case(SAMPLE_DATATYPE_LONG):
	{
		break;
	}
	case(SAMPLE_DATATYPE_FLOAT):
	{
		break;
	}
	case(SAMPLE_DATATYPE_STRING):
	{
		if(__u_val.c_string!=NULL)
		{
			delete []__u_val.c_string;
			__u_val.c_string=NULL;
		}
		break;
	}
	default:
	{
		assert(0);
		break;
	}
	}
}
void
USampleDataValue::c_long(const MLang::Long __v)
{
	__clear();
	__u_init=true;
	__u_index=SAMPLE_DATATYPE_LONG;
	__u_val.c_long=__v;
}
void
USampleDataValue::c_float(const float __v)
{
	__clear();
	__u_init=true;
	__u_index=SAMPLE_DATATYPE_FLOAT;
	__u_val.c_float=__v;
}
void
USampleDataValue::c_string(const char*__v)
{
	__clear();
	__u_init=true;
	__u_index=SAMPLE_DATATYPE_STRING;
	__u_val.c_string= new char[strlen(__v)+1];
	strcpy(__u_val.c_string,__v);
}
void 
USampleDataValue::c_string(const MLang::STRING&__v)
{
	__clear();
	__u_init=true;
	__u_index=SAMPLE_DATATYPE_STRING;
	__u_val.c_string= new char[__v.size()+1];
	strcpy(__u_val.c_string,__v.c_str());
}
const MLang::Long
USampleDataValue::c_long()const
{
	assert(__u_init && __u_index==SAMPLE_DATATYPE_LONG);
	return __u_val.c_long;
}
const float
USampleDataValue::c_float()const
{
	assert(__u_init && __u_index==SAMPLE_DATATYPE_FLOAT);
	return __u_val.c_float;
}
const char*
USampleDataValue::c_string()const
{
	assert(__u_init && __u_index==SAMPLE_DATATYPE_STRING);
	return __u_val.c_string;
}

void
TSampleKeyIDInfo::__write(MLang::OutputStream&__os)const
{
	__os.write_long(keyid);
	__os.write_short(keyid_datatype);
	__os.write_uchar(is_key);
}
void
TSampleKeyIDInfo::__read(MLang::InputStream&__is)
{
	keyid=__is.read_long();
	keyid_datatype=__is.read_short();
	is_key=__is.read_uchar();
}
void
TSampleDefineRequest::__write(MLang::OutputStream&__os)const
{
	__os.write_int(sample_type);
	__os.write_int(app_no);
}
void
TSampleDefineRequest::__read(MLang::InputStream&__is)
{
	sample_type=__is.read_int();
	app_no=__is.read_int();
}
void
TSEQSampleDefineRequest::__write(MLang::OutputStream&__os)const
{
	__os.write_int(seq_length);
	size_t len_0=seq_sample_define_request.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_sample_define_request[i_0].__write(__os);
	}
}
void
TSEQSampleDefineRequest::__read(MLang::InputStream&__is)
{
	seq_length=__is.read_int();
	size_t len_0=__is.read_uint();
	seq_sample_define_request.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_sample_define_request[i_0].__read(__is);
	}
}
TSEQSampleDefineRequest::TSEQSampleDefineRequest()
{
}
TSEQSampleDefineRequest::TSEQSampleDefineRequest(const TSEQSampleDefineRequest&__val)
:seq_length(__val.seq_length),seq_sample_define_request(__val.seq_sample_define_request)
{

}
TSEQSampleDefineRequest&
TSEQSampleDefineRequest::operator=(const TSEQSampleDefineRequest&__val)
{
	if(this == &__val)
		return *this;
	seq_length=__val.seq_length;
	seq_sample_define_request=__val.seq_sample_define_request;

	return *this;
}
void
TSampleDefineAnswer::__write(MLang::OutputStream&__os)const
{
	__os.write_int(sample_type);
	__os.write_int(app_no);
	__os.write_bool(is_success);
	error_info.__write(__os);
	__os.write_short(keyid_num);
	size_t len_0=seq_keyid_info.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_keyid_info[i_0].__write(__os);
	}
}
void
TSampleDefineAnswer::__read(MLang::InputStream&__is)
{
	sample_type=__is.read_int();
	app_no=__is.read_int();
	is_success=__is.read_bool();
	error_info.__read(__is);
	keyid_num=__is.read_short();
	size_t len_0=__is.read_uint();
	seq_keyid_info.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_keyid_info[i_0].__read(__is);
	}
}
TSampleDefineAnswer::TSampleDefineAnswer()
{
}
TSampleDefineAnswer::TSampleDefineAnswer(const TSampleDefineAnswer&__val)
:sample_type(__val.sample_type),app_no(__val.app_no),is_success(__val.is_success),error_info(__val.error_info),keyid_num(__val.keyid_num),seq_keyid_info(__val.seq_keyid_info)
{

}
TSampleDefineAnswer&
TSampleDefineAnswer::operator=(const TSampleDefineAnswer&__val)
{
	if(this == &__val)
		return *this;
	sample_type=__val.sample_type;
	app_no=__val.app_no;
	is_success=__val.is_success;
error_info=__val.error_info;
	keyid_num=__val.keyid_num;
	seq_keyid_info=__val.seq_keyid_info;

	return *this;
}
void
TSEQSampleDefineAnswer::__write(MLang::OutputStream&__os)const
{
	__os.write_int(seq_length);
	size_t len_0=seq_sample_define_answer.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_sample_define_answer[i_0].__write(__os);
	}
}
void
TSEQSampleDefineAnswer::__read(MLang::InputStream&__is)
{
	seq_length=__is.read_int();
	size_t len_0=__is.read_uint();
	seq_sample_define_answer.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_sample_define_answer[i_0].__read(__is);
	}
}
TSEQSampleDefineAnswer::TSEQSampleDefineAnswer()
{
}
TSEQSampleDefineAnswer::TSEQSampleDefineAnswer(const TSEQSampleDefineAnswer&__val)
:seq_length(__val.seq_length),seq_sample_define_answer(__val.seq_sample_define_answer)
{

}
TSEQSampleDefineAnswer&
TSEQSampleDefineAnswer::operator=(const TSEQSampleDefineAnswer&__val)
{
	if(this == &__val)
		return *this;
	seq_length=__val.seq_length;
	seq_sample_define_answer=__val.seq_sample_define_answer;

	return *this;
}
void
TSampleKeyIDValue::__write(MLang::OutputStream&__os)const
{
	__os.write_long(keyid);
	__os.write_short(keyid_datatype);
	keyid_value.__write(__os);
}
void
TSampleKeyIDValue::__read(MLang::InputStream&__is)
{
	keyid=__is.read_long();
	keyid_datatype=__is.read_short();
	keyid_value.__read(__is);
}
void
TSampleProcessRequest::__write(MLang::OutputStream&__os)const
{
	__os.write_int(sample_type);
	__os.write_int(app_no);
	__os.write_uchar(sample_mode);
	__os.write_int(occur_time);
	__os.write_int(flag);
	__os.write_int(reserve_1);
	__os.write_int(reserve_2);
	__os.write_short(keyid_num);
	size_t len_0=seq_keyid_value.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_keyid_value[i_0].__write(__os);
	}
}
void
TSampleProcessRequest::__read(MLang::InputStream&__is)
{
	sample_type=__is.read_int();
	app_no=__is.read_int();
	sample_mode=__is.read_uchar();
	occur_time=__is.read_int();
	flag=__is.read_int();
	reserve_1=__is.read_int();
	reserve_2=__is.read_int();
	keyid_num=__is.read_short();
	size_t len_0=__is.read_uint();
	seq_keyid_value.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_keyid_value[i_0].__read(__is);
	}
}
TSampleProcessRequest::TSampleProcessRequest()
{
}
TSampleProcessRequest::TSampleProcessRequest(const TSampleProcessRequest&__val)
:sample_type(__val.sample_type),app_no(__val.app_no),sample_mode(__val.sample_mode),occur_time(__val.occur_time),flag(__val.flag),reserve_1(__val.reserve_1),reserve_2(__val.reserve_2),keyid_num(__val.keyid_num),seq_keyid_value(__val.seq_keyid_value)
{

}
TSampleProcessRequest&
TSampleProcessRequest::operator=(const TSampleProcessRequest&__val)
{
	if(this == &__val)
		return *this;
	sample_type=__val.sample_type;
	app_no=__val.app_no;
	sample_mode=__val.sample_mode;
	occur_time=__val.occur_time;
	flag=__val.flag;
	reserve_1=__val.reserve_1;
	reserve_2=__val.reserve_2;
	keyid_num=__val.keyid_num;
	seq_keyid_value=__val.seq_keyid_value;

	return *this;
}
void
TSEQSampleProcessRequest::__write(MLang::OutputStream&__os)const
{
	__os.write_int(seq_length);
	size_t len_0=seq_sample_process_request.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_sample_process_request[i_0].__write(__os);
	}
}
void
TSEQSampleProcessRequest::__read(MLang::InputStream&__is)
{
	seq_length=__is.read_int();
	size_t len_0=__is.read_uint();
	seq_sample_process_request.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_sample_process_request[i_0].__read(__is);
	}
}
TSEQSampleProcessRequest::TSEQSampleProcessRequest()
{
}
TSEQSampleProcessRequest::TSEQSampleProcessRequest(const TSEQSampleProcessRequest&__val)
:seq_length(__val.seq_length),seq_sample_process_request(__val.seq_sample_process_request)
{

}
TSEQSampleProcessRequest&
TSEQSampleProcessRequest::operator=(const TSEQSampleProcessRequest&__val)
{
	if(this == &__val)
		return *this;
	seq_length=__val.seq_length;
	seq_sample_process_request=__val.seq_sample_process_request;

	return *this;
}
void
TSampleProcessInfo::__write(MLang::OutputStream&__os)const
{
	__os.write_long(keyid);
	__os.write_short(keyid_datatype);
	keyid_value.__write(__os);
	__os.write_bool(is_valid);
	__os.write_string(invalid_string.c_str());
}
void
TSampleProcessInfo::__read(MLang::InputStream&__is)
{
	keyid=__is.read_long();
	keyid_datatype=__is.read_short();
	keyid_value.__read(__is);
	is_valid=__is.read_bool();
	invalid_string=__is.read_string();
}
TSampleProcessInfo::TSampleProcessInfo()
{
}
TSampleProcessInfo::TSampleProcessInfo(const TSampleProcessInfo&__val)
:keyid(__val.keyid),keyid_datatype(__val.keyid_datatype),keyid_value(__val.keyid_value),is_valid(__val.is_valid),invalid_string(__val.invalid_string)
{

}
TSampleProcessInfo&
TSampleProcessInfo::operator=(const TSampleProcessInfo&__val)
{
	if(this == &__val)
		return *this;
	keyid=__val.keyid;
	keyid_datatype=__val.keyid_datatype;
keyid_value=__val.keyid_value;
	is_valid=__val.is_valid;
	invalid_string=__val.invalid_string;

	return *this;
}
void
TSampleProcessAnswer::__write(MLang::OutputStream&__os)const
{
	__os.write_int(sample_type);
	__os.write_int(app_no);
	__os.write_uchar(sample_mode);
	__os.write_int(occur_time);
	__os.write_short(keyid_num);
	size_t len_0=seq_process_info.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_process_info[i_0].__write(__os);
	}
	__os.write_bool(is_success);
	error_info.__write(__os);
}
void
TSampleProcessAnswer::__read(MLang::InputStream&__is)
{
	sample_type=__is.read_int();
	app_no=__is.read_int();
	sample_mode=__is.read_uchar();
	occur_time=__is.read_int();
	keyid_num=__is.read_short();
	size_t len_0=__is.read_uint();
	seq_process_info.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_process_info[i_0].__read(__is);
	}
	is_success=__is.read_bool();
	error_info.__read(__is);
}
TSampleProcessAnswer::TSampleProcessAnswer()
{
}
TSampleProcessAnswer::TSampleProcessAnswer(const TSampleProcessAnswer&__val)
:sample_type(__val.sample_type),app_no(__val.app_no),sample_mode(__val.sample_mode),occur_time(__val.occur_time),keyid_num(__val.keyid_num),seq_process_info(__val.seq_process_info),is_success(__val.is_success),error_info(__val.error_info)
{

}
TSampleProcessAnswer&
TSampleProcessAnswer::operator=(const TSampleProcessAnswer&__val)
{
	if(this == &__val)
		return *this;
	sample_type=__val.sample_type;
	app_no=__val.app_no;
	sample_mode=__val.sample_mode;
	occur_time=__val.occur_time;
	keyid_num=__val.keyid_num;
	seq_process_info=__val.seq_process_info;
	is_success=__val.is_success;
error_info=__val.error_info;

	return *this;
}
void
TSEQSampleProcessAnswer::__write(MLang::OutputStream&__os)const
{
	__os.write_int(seq_length);
	size_t len_0=seq_sample_process_answer.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_sample_process_answer[i_0].__write(__os);
	}
}
void
TSEQSampleProcessAnswer::__read(MLang::InputStream&__is)
{
	seq_length=__is.read_int();
	size_t len_0=__is.read_uint();
	seq_sample_process_answer.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_sample_process_answer[i_0].__read(__is);
	}
}
TSEQSampleProcessAnswer::TSEQSampleProcessAnswer()
{
}
TSEQSampleProcessAnswer::TSEQSampleProcessAnswer(const TSEQSampleProcessAnswer&__val)
:seq_length(__val.seq_length),seq_sample_process_answer(__val.seq_sample_process_answer)
{

}
TSEQSampleProcessAnswer&
TSEQSampleProcessAnswer::operator=(const TSEQSampleProcessAnswer&__val)
{
	if(this == &__val)
		return *this;
	seq_length=__val.seq_length;
	seq_sample_process_answer=__val.seq_sample_process_answer;

	return *this;
}