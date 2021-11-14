//Created by mcpp v1.0.
#ifndef __WARN_SERVICE_MESSAGE_M_H__
#define __WARN_SERVICE_MESSAGE_M_H__

#include"mcode/mvector.h"
#include"mcode/mstring.h"
#include"mcode/mstream.h"
#include"mcode/mclient.h"
#include"public_m.h"


typedef MLang::VECTOR<UDataValue> SEQ_FIELD_VALUE;
struct ONE_WARN_MESSAGE_STRU
{
	int	warn_type;
	int	app_no;
	MLang::Long	node_id;
	unsigned char	is_restrain;
	unsigned char	op_type;
	int	sound_table_key_field_order_no;
	int	reservered_1;
	int	reservered_2;
	SEQ_FIELD_VALUE	seq_field_info;
	ONE_WARN_MESSAGE_STRU();
	ONE_WARN_MESSAGE_STRU(const ONE_WARN_MESSAGE_STRU&);
	ONE_WARN_MESSAGE_STRU&operator=(const ONE_WARN_MESSAGE_STRU&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<ONE_WARN_MESSAGE_STRU> SEQ_WARN_MESSAGE_TYPE;
struct APP_TO_WARN_SERVICE_MESSAGE_STRU
{
	MLang::Long	warn_num;
	SEQ_WARN_MESSAGE_TYPE	seq_warn_message;
	APP_TO_WARN_SERVICE_MESSAGE_STRU();
	APP_TO_WARN_SERVICE_MESSAGE_STRU(const APP_TO_WARN_SERVICE_MESSAGE_STRU&);
	APP_TO_WARN_SERVICE_MESSAGE_STRU&operator=(const APP_TO_WARN_SERVICE_MESSAGE_STRU&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
struct ONE_WARN_DW_MESSAGE_STRU
{
	TKeyID	yx_id;
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<ONE_WARN_DW_MESSAGE_STRU> SEQ_DW_MESSAGE_TYPE;
struct WARN_DW_SERVICE_MESSAGE_STRU
{
	unsigned char	is_from_warn_window;
	int	dw_num;
	SEQ_DW_MESSAGE_TYPE	seq_dw_warn_message;
	WARN_DW_SERVICE_MESSAGE_STRU();
	WARN_DW_SERVICE_MESSAGE_STRU(const WARN_DW_SERVICE_MESSAGE_STRU&);
	WARN_DW_SERVICE_MESSAGE_STRU&operator=(const WARN_DW_SERVICE_MESSAGE_STRU&);
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
struct scadainfo_to_warn
{
	unsigned char	raise_flag;
	int	res1;
	int	res2;
	void __write(MLang::OutputStream&__os)const;
	void __read(MLang::InputStream&__is);
};
#endif
