//**************************************
// Generated by mcpp translator
// Version 1.2
//**************************************

#include"hd_file_message_m.h"
#include<assert.h>
void
FAD_CALLUP_COMTR_NAME_STRU::__write(MLang::OutputStream&__os)const
{
	size_t len_0=fac_seq.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		__os.write_long_array(&fac_seq[0],len_0);
		break;
	}
	__os.write_short(fac_num);
	__os.write_ulong(start_time);
	__os.write_ulong(end_time);
	__os.write_string(src_host_name.c_str());
	__os.write_string(src_proc_name.c_str());
}
void
FAD_CALLUP_COMTR_NAME_STRU::__read(MLang::InputStream&__is)
{
	size_t len_0=__is.read_uint();
	fac_seq.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		__is.read_long_array(&fac_seq[0],len_0);
		break;
	}
	fac_num=__is.read_short();
	start_time=__is.read_ulong();
	end_time=__is.read_ulong();
	src_host_name=__is.read_string();
	src_proc_name=__is.read_string();
}
FAD_CALLUP_COMTR_NAME_STRU::FAD_CALLUP_COMTR_NAME_STRU()
{
}
FAD_CALLUP_COMTR_NAME_STRU::FAD_CALLUP_COMTR_NAME_STRU(const FAD_CALLUP_COMTR_NAME_STRU&__val)
:fac_seq(__val.fac_seq),fac_num(__val.fac_num),start_time(__val.start_time),end_time(__val.end_time),src_host_name(__val.src_host_name),src_proc_name(__val.src_proc_name)
{

}
FAD_CALLUP_COMTR_NAME_STRU&
FAD_CALLUP_COMTR_NAME_STRU::operator=(const FAD_CALLUP_COMTR_NAME_STRU&__val)
{
	if(this == &__val)
		return *this;
	fac_seq=__val.fac_seq;
	fac_num=__val.fac_num;
	start_time=__val.start_time;
	end_time=__val.end_time;
	src_host_name=__val.src_host_name;
	src_proc_name=__val.src_proc_name;

	return *this;
}
void
FAD_TRANS_REPORT_STRUCT::__write(MLang::OutputStream&__os)const
{
	__os.write_long(fac_id);
	__os.write_short(type);
	__os.write_string(filename.c_str());
	__os.write_int(total_frame);
	__os.write_int(current_frame);
}
void
FAD_TRANS_REPORT_STRUCT::__read(MLang::InputStream&__is)
{
	fac_id=__is.read_long();
	type=__is.read_short();
	filename=__is.read_string();
	total_frame=__is.read_int();
	current_frame=__is.read_int();
}
FAD_TRANS_REPORT_STRUCT::FAD_TRANS_REPORT_STRUCT()
{
}
FAD_TRANS_REPORT_STRUCT::FAD_TRANS_REPORT_STRUCT(const FAD_TRANS_REPORT_STRUCT&__val)
:fac_id(__val.fac_id),type(__val.type),filename(__val.filename),total_frame(__val.total_frame),current_frame(__val.current_frame)
{

}
FAD_TRANS_REPORT_STRUCT&
FAD_TRANS_REPORT_STRUCT::operator=(const FAD_TRANS_REPORT_STRUCT&__val)
{
	if(this == &__val)
		return *this;
	fac_id=__val.fac_id;
	type=__val.type;
	filename=__val.filename;
	total_frame=__val.total_frame;
	current_frame=__val.current_frame;

	return *this;
}
