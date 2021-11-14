#include"warn_service_message_m.h"
#include<assert.h>
void
ONE_WARN_MESSAGE_STRU::__write(MLang::OutputStream&__os)const
{
	__os.write_int(warn_type);
	__os.write_int(app_no);
	__os.write_long(node_id);
	__os.write_uchar(is_restrain);
	__os.write_uchar(op_type);
	__os.write_int(sound_table_key_field_order_no);
	__os.write_int(reservered_1);
	__os.write_int(reservered_2);
	size_t len_0=seq_field_info.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_field_info[i_0].__write(__os);
	}
}
void
ONE_WARN_MESSAGE_STRU::__read(MLang::InputStream&__is)
{
	warn_type=__is.read_int();
	app_no=__is.read_int();
	node_id=__is.read_long();
	is_restrain=__is.read_uchar();
	op_type=__is.read_uchar();
	sound_table_key_field_order_no=__is.read_int();
	reservered_1=__is.read_int();
	reservered_2=__is.read_int();
	size_t len_0=__is.read_uint();
	seq_field_info.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_field_info[i_0].__read(__is);
	}
}
ONE_WARN_MESSAGE_STRU::ONE_WARN_MESSAGE_STRU()
{
}
ONE_WARN_MESSAGE_STRU::ONE_WARN_MESSAGE_STRU(const ONE_WARN_MESSAGE_STRU&__val)
:warn_type(__val.warn_type),app_no(__val.app_no),node_id(__val.node_id),is_restrain(__val.is_restrain),op_type(__val.op_type),sound_table_key_field_order_no(__val.sound_table_key_field_order_no),reservered_1(__val.reservered_1),reservered_2(__val.reservered_2),seq_field_info(__val.seq_field_info)
{

}
ONE_WARN_MESSAGE_STRU&
ONE_WARN_MESSAGE_STRU::operator=(const ONE_WARN_MESSAGE_STRU&__val)
{
	if(this == &__val)
		return *this;
	warn_type=__val.warn_type;
	app_no=__val.app_no;
	node_id=__val.node_id;
	is_restrain=__val.is_restrain;
	op_type=__val.op_type;
	sound_table_key_field_order_no=__val.sound_table_key_field_order_no;
	reservered_1=__val.reservered_1;
	reservered_2=__val.reservered_2;
	seq_field_info=__val.seq_field_info;

	return *this;
}
void
APP_TO_WARN_SERVICE_MESSAGE_STRU::__write(MLang::OutputStream&__os)const
{
	__os.write_long(warn_num);
	size_t len_0=seq_warn_message.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_warn_message[i_0].__write(__os);
	}
}
void
APP_TO_WARN_SERVICE_MESSAGE_STRU::__read(MLang::InputStream&__is)
{
	warn_num=__is.read_long();
	size_t len_0=__is.read_uint();
	seq_warn_message.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_warn_message[i_0].__read(__is);
	}
}
APP_TO_WARN_SERVICE_MESSAGE_STRU::APP_TO_WARN_SERVICE_MESSAGE_STRU()
{
}
APP_TO_WARN_SERVICE_MESSAGE_STRU::APP_TO_WARN_SERVICE_MESSAGE_STRU(const APP_TO_WARN_SERVICE_MESSAGE_STRU&__val)
:warn_num(__val.warn_num),seq_warn_message(__val.seq_warn_message)
{

}
APP_TO_WARN_SERVICE_MESSAGE_STRU&
APP_TO_WARN_SERVICE_MESSAGE_STRU::operator=(const APP_TO_WARN_SERVICE_MESSAGE_STRU&__val)
{
	if(this == &__val)
		return *this;
	warn_num=__val.warn_num;
	seq_warn_message=__val.seq_warn_message;

	return *this;
}
void
ONE_WARN_DW_MESSAGE_STRU::__write(MLang::OutputStream&__os)const
{
	__os.write_long(yx_id);
}
void
ONE_WARN_DW_MESSAGE_STRU::__read(MLang::InputStream&__is)
{
	yx_id=__is.read_long();
}
void
WARN_DW_SERVICE_MESSAGE_STRU::__write(MLang::OutputStream&__os)const
{
	__os.write_uchar(is_from_warn_window);
	__os.write_int(dw_num);
	size_t len_0=seq_dw_warn_message.size();
	__os.write_uint(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_dw_warn_message[i_0].__write(__os);
	}
}
void
WARN_DW_SERVICE_MESSAGE_STRU::__read(MLang::InputStream&__is)
{
	is_from_warn_window=__is.read_uchar();
	dw_num=__is.read_int();
	size_t len_0=__is.read_uint();
	seq_dw_warn_message.resize(len_0);
	for(size_t i_0=0;i_0<len_0;i_0++)
	{
		seq_dw_warn_message[i_0].__read(__is);
	}
}
WARN_DW_SERVICE_MESSAGE_STRU::WARN_DW_SERVICE_MESSAGE_STRU()
{
}
WARN_DW_SERVICE_MESSAGE_STRU::WARN_DW_SERVICE_MESSAGE_STRU(const WARN_DW_SERVICE_MESSAGE_STRU&__val)
:is_from_warn_window(__val.is_from_warn_window),dw_num(__val.dw_num),seq_dw_warn_message(__val.seq_dw_warn_message)
{

}
WARN_DW_SERVICE_MESSAGE_STRU&
WARN_DW_SERVICE_MESSAGE_STRU::operator=(const WARN_DW_SERVICE_MESSAGE_STRU&__val)
{
	if(this == &__val)
		return *this;
	is_from_warn_window=__val.is_from_warn_window;
	dw_num=__val.dw_num;
	seq_dw_warn_message=__val.seq_dw_warn_message;

	return *this;
}
void
scadainfo_to_warn::__write(MLang::OutputStream&__os)const
{
	__os.write_uchar(raise_flag);
	__os.write_int(res1);
	__os.write_int(res2);
}
void
scadainfo_to_warn::__read(MLang::InputStream&__is)
{
	raise_flag=__is.read_uchar();
	res1=__is.read_int();
	res2=__is.read_int();
}
