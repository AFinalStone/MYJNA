#include "data_packet.h"
#include<netinet/in.h>
namespace packet{

void DataOutPacket::Write8(int8 b){
	if (cursor_ - buffer_ + sizeof(b) > buffer_length_){
		int32 filled = cursor_ - buffer_;
		buffer_length_ = (int32) (cursor_ - buffer_ + sizeof(b)) * 2;
		buffer_ = (char*)realloc(buffer_,buffer_length_);
		if (buffer_==NULL){
			buffer_length_ = 0;
			Reset();
			return;
		}
		cursor_ = buffer_ + filled;
	}

	*(int8*)cursor_ = b;
	cursor_ += sizeof(b);
}


void DataOutPacket::Write16(int16 w){
	if (cursor_ - buffer_ + sizeof(w) > buffer_length_){
		int32 filled = cursor_ - buffer_;
		buffer_length_ = (int32) (cursor_ - buffer_ + sizeof(w)) * 2;
		buffer_ = (char*)realloc(buffer_,buffer_length_);
		if (buffer_==NULL){
			buffer_length_ = 0;
			Reset();
			return;
		}
		cursor_ = buffer_ + filled;
	}

	*(int16*) cursor_ = w;
	cursor_ += sizeof(w);
}

void DataOutPacket::Write32(int32 dw){
	if (cursor_ - buffer_ + sizeof(dw) > buffer_length_){
		int32 filled = cursor_ - buffer_;
		buffer_length_ = (int32) (cursor_ - buffer_ + sizeof(dw)) * 2;
		buffer_ = (char*)realloc(buffer_,buffer_length_);
		if (buffer_==NULL){
			buffer_length_ = 0;
			Reset();
			return;
		}
		cursor_ = buffer_ + filled;
	}

	*(int32*) cursor_ = dw;
	cursor_ += sizeof(dw);
}

void DataOutPacket::Write64(int64 lw){
	if (cursor_ - buffer_ + sizeof(lw) > buffer_length_){
		int32 filled = cursor_ - buffer_;
		buffer_length_ = (int32) (cursor_ - buffer_ + sizeof(lw)) * 2;
		buffer_ = (char*)realloc(buffer_,buffer_length_);
		if (buffer_==NULL){
			buffer_length_ = 0;
			Reset();
			return;
		}
		cursor_ = buffer_ + filled;
	}

	*(int64*) cursor_ = lw;
	cursor_ += sizeof(lw);

}

void DataOutPacket::WriteData(const char* buf,const int32 len){
	if(cursor_ - buffer_ + len > buffer_length_)
	{
		int32 filled = cursor_ - buffer_;
		buffer_length_ = (int32)(cursor_ - buffer_ + len ) * 2;
		buffer_ =(char*)realloc(buffer_  ,buffer_length_);
		if (buffer_==NULL)
		{
			buffer_length_ = 0;
			Reset();
			return;
		}
		cursor_ = buffer_ + filled;
	}

	memcpy(cursor_, buf, len);
	cursor_ += len;
}

void DataOutPacket::WriteString(const char *str, int32 n){
	int32 len = n + 1;
	if(cursor_ - buffer_ + n + sizeof(len) > buffer_length_){
		int filled = cursor_ - buffer_;
		buffer_length_ = (unsigned int)(cursor_ - buffer_ + n + sizeof(len))*2;
		buffer_ = (char*)realloc(buffer_, buffer_length_);
		if (buffer_==NULL){
			buffer_length_ = 0;
			Reset();
			return;
		}
		cursor_ = buffer_ + filled;
	}

	*this << (int32)len;
	memcpy(cursor_, str, len);
	cursor_[n] = '\0';
	cursor_ += len;
}

DataOutPacket & DataOutPacket::operator <<(int8 b){
	Write8(b);
	return *this;
}

DataOutPacket &DataOutPacket::operator <<(int16 w)
{
	Write16(htons(w));
	return *this;
}

DataOutPacket &DataOutPacket::operator <<(int32 dw)
{
	Write32(htonl(dw));
	return *this;
}

DataOutPacket &DataOutPacket::operator <<(int64 lw)
{
	Write64(htonl64(lw));
	return *this;
}

DataOutPacket &DataOutPacket::operator <<(const char *str)
{
	WriteString(str, (int)strlen(str));
	return *this;
}

DataOutPacket &DataOutPacket::operator <<(const std::string& str)
{
	WriteString(str.c_str(), (int)strlen(str.c_str()));
	return *this;
}

DataOutPacket &DataOutPacket::operator <<(bool b)
{
	Write8(b ? 1 : 0);
	return *this;
}

int16 DataInPacket::Read16()
{
	int16 w = 0;
	if ( data_len_ - (cursor_ - data_) >= sizeof(w))
	{
		w = *(int16 *) cursor_;
		cursor_ += sizeof(w);
	}
	return w;
}

int32 DataInPacket::Read32()
{
	int32 dw = 0;
	if (data_len_ - (cursor_ - data_) >= sizeof(dw))
	{
		dw = *(int32 *) cursor_;
		cursor_ += sizeof(dw);
	}
	return dw;
}

int64 DataInPacket::Read64()
{
	int64 lw = 0;
	if (data_len_ - (cursor_ - data_) >= sizeof(lw))
	{
		lw = *(int64 *) cursor_;
		cursor_ += sizeof(lw);
	}
	return lw;
}

const char* DataInPacket::ReadData(int32 size,int32& n){
	n = (int)(data_len_ - (cursor_ - data_));
	if(n == 0)
	{
		return NULL;
	}
	const char* temp_cursor = cursor_;
	cursor_ += size;
	return temp_cursor;
}

const  char *DataInPacket::ReadData(int &n)
{
	n = (int)(data_len_ - (cursor_ - data_));
	if(n == 0)
	{
		return NULL;
	}
	return cursor_;
}

int8 DataInPacket::Read8()
{
	int8 b = 0;
	if (data_len_ - (cursor_ - data_) >= sizeof(b))
	{
		b = *cursor_++;
	}
	return b;
}

DataInPacket &DataInPacket::operator >>(bool &b)
{
	b = (Read8() != 0);
	return *this;
}

DataInPacket &DataInPacket::operator >>(int8 &b)
{
	b = Read8();
	return *this;
}

DataInPacket &DataInPacket::operator >>(int16 &w)
{
	w = ntohs(Read16());
	return *this;
}

DataInPacket &DataInPacket::operator >> (int32 &dw)
{
	dw = ntohl(Read32());
	return *this;
}

DataInPacket &DataInPacket::operator >>(int64 &lw)
{
	lw = ntohl64(Read64());
	return *this;
}

DataInPacket &DataInPacket::operator >> (const char* &str)
{
	int32 len;
	operator >> (len);
	if (len > 0 && len <= data_len_ - (cursor_ - data_) && !cursor_[len - 1])
	{
		str = (char *)cursor_;
		cursor_ += len;
	}
	else
	{
		str = "";
	}
	return *this;
}

DataInPacket &DataInPacket::operator >> (std::string& str)
{
	int32 len;
	operator >> (len);
	if (len > 0 && len <= data_len_ - (cursor_ - data_) && !cursor_[len - 1])
	{
		str = (char *)cursor_;
		cursor_ += len;
	}
	else
	{
		str = "";
	}
	return *this;
}


DataInPacket &DataInPacket::operator >> (char* str)
{
	const char* cszStr=NULL;
	int32 len;
	operator >> (len);
	if (len > 0 && len <= data_len_ - (cursor_ - data_) && !cursor_[len - 1])
	{
		cszStr = (char *)cursor_;
		cursor_ += len;
	}
	else
	{
		cszStr = "";
	}
	strcpy(str,cszStr);
	return *this;
}

int DataInPacket::Forward(int delta)
{
	int forward_len = cursor_ - data_;
	if(delta > forward_len)
	{
		delta = forward_len;
	}
	cursor_ -= delta;

	return delta;
}

}
