//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_NET_COMM_HEAD_H_
#define SWP_NET_COMM_HEAD_H_

#include <list>
#include <string>

#include "basictypes.h"
//#include "logic/base_values.h"

enum PACKET_TYPE {
  ERROR_TYPE = 0,
  USER_TYPE = 1,
  CHAT_TYPE = 2,
  USERS_TYPE = 3,
  QUOTATIONS_TYPE = 4,
  TRADES_TYPE = 5,
  HISTORY_TYPE = 6,
  PAY_TYPE = 7,
  BANK_TYPE = 8,
  IM_TYPE = 9,
  INFO_TYPE = 10,
  MARKET_TYPE = 11,
  COMMENTS_TYPE = 12,
  SEARCH_TYPE = 13,
  FLASH_TYPE = 14,
  CIRCLE_TYPE = 15,
  STARSIDE_TYPE = 16,
  NETFLASH_TYPE = 17
};


enum PRS {
    NOZIP_AND_NOENCRYPT = 0,
    ZIP_AND_NOENCRYPT = 1,
    NOZIP_AND_ENCRYPT = 2,
    ZIP_AND_ENCRYPT = 3
};

enum operatorcode {
  HEART_PACKET = 0x64

};

enum NETTYPE {
  SOCKET = 1,
  HTTP = 2
};

//  packet_length 长度为原始数据长度
struct PacketHead{
    int16 packet_length;
    int8  is_zip_encrypt;
    int8  type;
    int16 signature;
    int16 operate_code;
    int16 data_length;
    int32 timestamp;
    int64 session_id;
    int32 reserved;
};

//struct PacketControl : public PacketHead {
//  base_logic::DictionaryValue* body_;
//};

#define PACKET_HEAD_LENGTH (sizeof(int16) * 4 + sizeof(int8) * 2 + \
    sizeof(int32) * 2 + sizeof(int64))

#endif /*SWP_NET_COMM_HEAD_H_*/
