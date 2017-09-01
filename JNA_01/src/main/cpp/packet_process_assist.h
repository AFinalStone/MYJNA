//  Copyright (c) 2016-2017 The KID Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_PUB_LOGIC_NET_CHECKSUM_H_
#define SWP_PUB_LOGIC_NET_CHECKSUM_H_

#include <string>
#include "zlib.h"
#include "basic/basictypes.h"


#ifdef __ANDROID__
#ifdef __cplusplus
extern "C"{
#endif

unsigned int GetKey();
void SetKey(unsigned int key);
unsigned int CreateKey();

#ifdef __cplusplus
}
#endif
#endif

namespace net {

typedef signed char int8_t;
typedef short int  int16_t;
typedef int        int32_t;
typedef unsigned char       BYTE;

enum zip_type {
	NO_ZIP_TYPE = 0,
	ZIP_TYPE
};

enum encrypt_type {
	NO_ENCRYPT_TYPE = 0,
	TEA
};

#define ENCODE_ZLIB_MIN_COMP_SIZE (17)

bool checksum(const char* src_data, uint16&  check_ret);

#ifndef __ANDROID__

unsigned int GetKey();
void SetKey(unsigned int key);
unsigned int CreateKey();

#endif

//jiami
char* str_en_8byte(char *clearmsg, int len, int rounds, int& len1);
//jiemi
char* str_de_8byte(char *encryptedmsg, int len, int rounds);

//fengzhuang
std::string str_en(std::string a, int rounds);

std::string str_de(std::string strEn, int rounds);

enum
{
	ZIP_TYPE_NONE = 0,
	ZIP_TYPE_ZLIB
};

class MZip
{
public:
	MZip(void);
	virtual ~MZip(void);

	unsigned long ZipData(const unsigned char* pUnZipData, unsigned long ulUnZipLen, unsigned char** ppZipData);
	void EnableCompress(bool bEnable);
	int Zip(const unsigned char* pUnZipData, unsigned long ulUnZipLen, unsigned char* pZipData, unsigned long& ulZipLen);

public:
	z_stream*         zlibStream_;
	unsigned long     ulTotalZipLen_;
	bool		      bCompress_;
};

class MUnZip
{
public:
	MUnZip(void);
	virtual ~MUnZip(void);

	unsigned long UnZipData(const unsigned char* pZipData, unsigned long ulZipLen, unsigned char** ppUnZipData);
	int UnZip(const unsigned char* pZipData, unsigned long ulZipLen, unsigned char** ppUnZipData, unsigned long* ulUnZipLen, int iRecursion=0);
public:
	z_stream * zlibStream_;
	unsigned long unZipLen_;
};


} // namespace net

#endif

