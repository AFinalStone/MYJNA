#ifndef MIG_FM_PUBLIC_PROTOCOL_DATA_PACKET_H__
#define MIG_FM_PUBLIC_PROTOCOL_DATA_PACKET_H__

#include <stdio.h>
#include <string>
#include "basictypes.h"

namespace packet{

#define htonl64(x) \
	((((x) & 0xff00000000000000ull) >> 56)				      \
	| (((x) & 0x00ff000000000000ull) >> 40)				      \
	| (((x) & 0x0000ff0000000000ull) >> 24)				      \
	| (((x) & 0x000000ff00000000ull) >> 8)				      \
	| (((x) & 0x00000000ff000000ull) << 8)				      \
	| (((x) & 0x0000000000ff0000ull) << 24)				      \
	| (((x) & 0x000000000000ff00ull) << 40)				      \
	| (((x) & 0x00000000000000ffull) << 56))

#define ntohl64 htonl64

class DataOutPacket
{
public:
	DataOutPacket(bool auto_delete = true,int len = 1024)
		:buffer_(NULL)
		,buffer_length_(len)
		,auto_delete_(auto_delete){
			buffer_ = (char*)malloc(buffer_length_);
			memset(buffer_,'0',buffer_length_);
			if (buffer_==NULL)
				buffer_length_ = 0;
			Reset();

	}

	virtual ~DataOutPacket(void){
		if (buffer_ && auto_delete_){
			free(buffer_);
			buffer_ = NULL;
		}
	}

	const char* GetData(){
		return buffer_;
	}

	int32 GetLength(){
		int32 n;
		n = (int32) (cursor_ - buffer_);
		return n;
	}

	void Reset(){
		cursor_ = buffer_;
	}
public:
	void Write8(int8 b);
	void Write16(int16 w);
	void Write32(int32 dw);
	void Write64(int64 lw);
	void WriteData(const char* buf,const int32 len);
	void WriteString(const char *str, int32 n);

	DataOutPacket &operator <<(int8 b);
	DataOutPacket &operator <<(int16 w);
	DataOutPacket &operator <<(int32 dw);
	DataOutPacket &operator <<(int64 lw);
	DataOutPacket &operator <<(const char *str);
	DataOutPacket &operator <<(const std::string& str);
	DataOutPacket &operator <<(bool b);


	inline const char* Skip (int delta){
		cursor_ += delta;
		return (cursor_ - delta);
	}

	inline const char* SetCursor(const char* cur){
		const char* old_cur = cursor_;
		cursor_ = const_cast<char*>(cur);
		return old_cur;
	}
protected:
	char*    buffer_;
	int32    buffer_length_;
	char*    cursor_;
	bool     auto_delete_;
};


class DataInPacket{
public:
	DataInPacket(const char* d,int32 n){
		cursor_ = data_ =const_cast<char*>(d);
		init_len_ = data_len_ = n;
	}

	DataInPacket &operator >>(int8 &b);
	DataInPacket &operator >>(int16 &w);
	DataInPacket &operator >>(int32 &dw);
	DataInPacket &operator >>(int64 &lw);
	DataInPacket &operator >>(const char* &str);
	DataInPacket &operator >>(char* str);
	DataInPacket &operator >>(std::string& str);
	DataInPacket &operator >>(bool &b);

	char* GetCursor() const {return cursor_;}
	int32 GetDataLen() const {return data_len_;}

    void Reset(){
        cursor_ = data_;
        data_len_ = init_len_;
    }

	int8 Read8();
	int64 Read64();
	int32 Read32();
	int16 Read16();
	const  char *ReadData(int32 &n);
	const  char *ReadData(int32 size,int32& n);

	int32 Forward(int32 delta);

	inline const int32 GetLength(){
		return data_len_;
	}

protected:
	char* data_;
	char* cursor_;
	int32 data_len_;
	int32 init_len_;
};
}
#endif
