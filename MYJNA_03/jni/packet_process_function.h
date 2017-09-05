//  Copyright (c) 2016-2017 The KID Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_PUB_NET_PACKET_PROCESS_FUNCTION_H_
#define SWP_PUB_NET_PACKET_PROCESS_FUNCTION_H_

#include "basictypes.h"


//明文->密文
extern "C" bool PacketStream(const void *in_stream, int32 in_stream_length,
                    void **out_stream, int32* out_stream_length);

//密文->明文
extern "C" bool UnpackStream(const void *in_stream, int32 in_stream_length,
                    void **out_stream, int32* out_stream_length);
#endif /* SWP_PUB_NET_PACKET_PROCESS_FUNCTION_H_ */
