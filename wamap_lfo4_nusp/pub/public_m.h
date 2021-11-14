//**************************************
// Generated by mcpp translator
// Version 1.2
//**************************************

#ifndef __PUBLIC_M_H__
#define __PUBLIC_M_H__

#include"mcode/mvector.h"
#include"mcode/mstring.h"
#include"mcode/mstream.h"
#include"mcode/mclient.h"


typedef MLang::VECTOR<MLang::STRING> StrSeq;
typedef MLang::VECTOR<MLang::Long> LongSeq;
typedef MLang::VECTOR<short> ShortSeq;
typedef MLang::VECTOR<char> CharSeq;
const short C_DATATYPE_DEFAULT = 0;
const short C_DATATYPE_STRING = 1;
const short C_DATATYPE_UCHAR = 2;
const short C_DATATYPE_SHORT = 3;
const short C_DATATYPE_INT = 4;
const short C_DATATYPE_DATETIME = 5;
const short C_DATATYPE_FLOAT = 6;
const short C_DATATYPE_DOUBLE = 7;
const short C_DATATYPE_KEYID = 8;
const short C_DATATYPE_BINARY = 9;
const short C_DATATYPE_TEXT = 10;
const short C_DATATYPE_IMAGE = 11;
const short C_DATATYPE_APPKEYID = 12;
const short C_DATATYPE_APPID = 13;
const short C_DATATYPE_UINT = 14;
const short C_DATATYPE_LONG = 15;
typedef MLang::Long TKeyID;
struct TAppKeyID
{
    int	app_id;
    TKeyID	key_id;
    void __write(MLang::OutputStream&__os)const;
    void __read(MLang::InputStream&__is);
};
typedef TAppKeyID TAppID;
struct UDataValue
{
private:
    union
    {
        char*	c_string;
        unsigned char	c_uchar;
        short	c_short;
        int	c_int;
        MLang::Long	c_time;
        float	c_float;
        double	c_double;
        TKeyID	c_keyid;
        CharSeq*	c_binary;
        CharSeq*	c_text;
        CharSeq*	c_image;
        TAppKeyID	c_appkeyid;
        TAppID	c_appid;
        char	c_default;
        unsigned int	c_uint;
        MLang::Long	c_long;
    } __u_val;
    bool __u_init;
    unsigned short __u_index;
public:
    UDataValue();
    ~UDataValue();
    UDataValue(const UDataValue&);
    UDataValue&operator=(const UDataValue&);
    short _d()const;
    void __clear();
    void __write(MLang::OutputStream&__os)const;
    void __read(MLang::InputStream&__is);
    void c_string(const char*);
    void	c_string(const MLang::STRING&);
    const char*c_string()const;
    void c_uchar(const unsigned char);
    const unsigned char	c_uchar()const;
    void c_short(const short);
    const short	c_short()const;
    void c_int(const int);
    const int	c_int()const;
    void c_time(const MLang::Long);
    const MLang::Long	c_time()const;
    void c_float(const float);
    const float	c_float()const;
    void c_double(const double);
    const double	c_double()const;
    void c_keyid(const TKeyID);
    const TKeyID	c_keyid()const;
    void c_binary(const CharSeq&);
    const CharSeq&c_binary()const;
    CharSeq&c_binary();
    void c_text(const CharSeq&);
    const CharSeq&c_text()const;
    CharSeq&c_text();
    void c_image(const CharSeq&);
    const CharSeq&c_image()const;
    CharSeq&c_image();
    void	c_appkeyid(const TAppKeyID&);
    const	TAppKeyID&c_appkeyid()const;
    TAppKeyID&c_appkeyid();
    void	c_appid(const TAppID&);
    const	TAppID&c_appid()const;
    TAppID&c_appid();
    void c_default(const char);
    const char	c_default()const;
    void c_uint(const unsigned int);
    const unsigned int	c_uint()const;
    void c_long(const MLang::Long);
    const MLang::Long	c_long()const;
};
typedef MLang::VECTOR<UDataValue> SEQDataValue;
struct TDBErrorStru
{
    unsigned int	error_no;
    MLang::STRING	error_msg;
    TDBErrorStru();
    TDBErrorStru(const TDBErrorStru&);
    TDBErrorStru&operator=(const TDBErrorStru&);
    void __write(MLang::OutputStream&__os)const;
    void __read(MLang::InputStream&__is);
};
typedef MLang::VECTOR<TDBErrorStru> SEQDBErrorStru;
#endif