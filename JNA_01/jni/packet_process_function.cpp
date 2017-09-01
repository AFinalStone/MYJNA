//  Copyright (c) 2016-2017 The KID Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry


#include "packet_process_function.h"
#include "comm_head.h"
#include "packet_process_assist.h"
#include "data_packet.h"


#include <list>
#include <string>

extern "C"{
//明文->密文
bool PacketStream(const void *in_stream, int32 in_stream_length,
                  void **out_stream, int32* out_stream_length) {
    int32 temp;
    
    
    
    //1.读取明文包信息
    packet::DataInPacket in(
                            reinterpret_cast<char*>(const_cast<void*>(in_stream)), in_stream_length);
    int16 packet_length = in.Read16();
    int8 is_zip_encrypt = in.Read8();
    //  is_zip_encrypt = (net::TEA<<4) + net::ZIP_TYPE; //高4位加密类型  低4位压缩类型  TODO:一定加密, 选择性压缩
    is_zip_encrypt = (net::TEA<<4); //高4位加密类型  低4位压缩类型  TODO:一定加密, 选择性压缩
    
    int8 type = in.Read8();
    uint16 signature = in.Read16();//原包中签名
    int16 operate_code = in.Read16();
    int16 data_length = in.Read16();
    int32 timestamp = in.Read32();
    int64 session_id = in.Read64();
    int32 reserved = in.Read32();
    std::string body_stream = in.ReadData(data_length, temp);
    //LOG_ERROR2("PacketStream packet length[%d],is_zip_encrypt[%d]", packet_length, is_zip_encrypt);
    
    //加密算法会补全8 位，导致checksum 结果变化，提前补足
    int32 packetBody_length = PACKET_HEAD_LENGTH + body_stream.length() - sizeof(int16) - sizeof(int8);
    if (packetBody_length%8 != 0) {
        packetBody_length = (packetBody_length/8 + 1) * 8 + sizeof(int16) + sizeof(int8) - PACKET_HEAD_LENGTH;
    } else {
        packetBody_length = body_stream.length();
    }
    //LOG_DEBUG2("PacketStream packetBody_length:%d, content:%s", packetBody_length, body_stream.c_str());
    
    //1.包大小改变，重组明文包
    data_length = packetBody_length;
    packet_length = data_length + PACKET_HEAD_LENGTH;
    packet::DataOutPacket out(false, packet_length);
    out.Write16(packet_length);
    out.Write8(is_zip_encrypt);
    out.Write8(type);
    out.Write16(0);//signature
    out.Write16(operate_code);
    out.Write16(data_length);
    out.Write32(timestamp);
    out.Write64(session_id);
    out.Write32(reserved);
    out.WriteData(body_stream.c_str(), packetBody_length);
    
    
    //2.对明文包checksum，写入signature
    char* packetBody_stream = reinterpret_cast<char *>(const_cast<char *>(out.GetData()));
    if(!net::checksum(packetBody_stream, signature)) {
        //LOG_ERROR("PacketStream checksum failure.");
    }
    out.Reset();
    out.Skip( sizeof(int16) + sizeof(int8) + sizeof(int8) );
    out.Write16(signature);
    //LOG_DEBUG2("PacketStream packet_length[%d] is_zip_encrypt[%d] signature [%d]",
    //              packet_length, is_zip_encrypt, signature);
    
    //4.加密
    char* encrypt_packetBody = \
    reinterpret_cast<char *>(const_cast<char *>(out.GetData())) + sizeof(int16) + sizeof(int8);
    //LOG_ERROR2("====befor encrypt str[%s]", encrypt_packetBody);
    int32 temp_data_len = packet_length - sizeof(int16) - sizeof(int8);
    packetBody_stream = net::str_en_8byte(encrypt_packetBody, temp_data_len, 16, packetBody_length);//Don't forget free memory
    //LOG_ERROR2("====after encrypt str[%s]", packetBody_stream);
    //LOG_DEBUG2("====encrypt in len[%d], return length[%d]", temp_data_len, packetBody_length);
    
    
    //3.暂时都不压缩 TODO: 包体小的时候不压缩，节省时间
    /*  unsigned char* zipData;
     unsigned char* zip_packetBody = reinterpret_cast<unsigned char *>(packetBody_stream);
     MZip* zip_handle = new MZip();//TODO: create once
     unsigned long zipSize = zip_handle->ZipData(zip_packetBody, packetBody_length, &zipData);
     delete zip_handle;
     zip_handle = NULL;
     LOG_DEBUG2("after zip, data size[%d]", zipSize);
     
     packetBody_stream = reinterpret_cast<char *>(zipData);
     packetBody_length = zipSize;
     LOG_ERROR2("after zip, body length[%d]", packetBody_length);
     */
    
    //5.组密文包
    packet_length = packetBody_length + sizeof(int16) + sizeof(int8);
    packet::DataOutPacket out_packet(false, packet_length);
    out_packet.Write16(packet_length);
    out_packet.Write8(is_zip_encrypt);
    out_packet.WriteData(packetBody_stream, packetBody_length);
    
    
    
    *out_stream = reinterpret_cast<void *>(const_cast<char *>(out_packet.GetData()));
    *out_stream_length = packet_length;
    //LOG_DEBUG2("PacketStream after packet length[%d]", packet_length);
    
    if (packetBody_stream) {
        delete packetBody_stream;
        packetBody_stream = NULL;
    }
    
    return true;
}
//密文->明文
bool UnpackStream(const void *in_stream, int32 in_stream_length,
                  void **out_stream, int32* out_stream_length) {
    int32 temp;
    packet::DataInPacket in(
                            reinterpret_cast<char*>(const_cast<void*>(in_stream)), in_stream_length);
    int16 packet_length = in.Read16();
    int8 is_zip_encrypt = in.Read8();
    //LOG_ERROR2("UnpackStream packet length[%d],is_zip_encrypt[%d]", packet_length, is_zip_encrypt);
    
    char* body_stream;
    int32 body_length = packet_length - sizeof(int16) - sizeof(int8);
    //1.判断是否需要解压
    if((is_zip_encrypt & 0xF) > 0) {
        unsigned char* zip_packetBody = \
        reinterpret_cast<unsigned char*>(const_cast<char *>(in.ReadData(temp)));
        unsigned char* unzipData;
        net::MUnZip* unzip_handle = new net::MUnZip();//TODO:Create once
        body_length = unzip_handle->UnZipData(zip_packetBody, body_length, &unzipData);
        body_stream = reinterpret_cast<char *>(unzipData);
        delete unzip_handle;
        unzip_handle = NULL;
    } else {
        body_stream = reinterpret_cast<char *>(const_cast<char *>(in.ReadData(temp)));
    }
    //LOG_DEBUG2("UnpackStream after unzip body length[%d]", body_length);
    
    //2.判断是否需要解密
    if(((is_zip_encrypt>>4) & 0xF) > 0) {
        char* decrypt_packetBody = reinterpret_cast<char *>(body_stream);
        //LOG_ERROR2("====before decrypt str[%s]", decrypt_packetBody);
        body_stream = net::str_de_8byte(decrypt_packetBody, body_length, 16);//Don't forget free memory
        //LOG_ERROR2("====after decrypt str[%s]", body_stream);
    } else {
        //body_stream = reinterpret_cast<char *>(const_cast<char *>(in.ReadData(temp)));
        //body_length = len - sizeof(int16) - sizeof(int8);
    }
    //LOG_DEBUG2("UnpackStream after decrypt body length[%d]", body_length);
    
    
    //3.解包
    packet::DataInPacket in_body_stream(reinterpret_cast<char*>(body_stream), body_length);
    int8 type = in_body_stream.Read8();
    uint16 signature = in_body_stream.Read16();//原包中签名
    /*  int16 operate_code = in_body_stream.Read16();
     int16 data_length = in_body_stream.Read16();
     int32 timestamp = in_body_stream.Read32();
     int64 session_id = in_body_stream.Read64();
     int32 reserved = in_body_stream.Read32();
     std::string body_string = in_body_stream.ReadData(data_length, temp);
     LOG_ERROR2("===test type[%d], signature[%d], operate_code[%d], data_length[%d],timestamp[%d], \
     session_id[%ld],reserved[%d], body_string[%s]", type,signature,operate_code,data_length,
     timestamp,session_id,reserved, body_string.c_str());
     */
    
    //4.重新组明文包
    packet_length = body_length + sizeof(int16) + sizeof(int8);
    packet::DataOutPacket out_packet(false, packet_length);
    out_packet.Write16(packet_length);
    out_packet.Write8(is_zip_encrypt);
    out_packet.WriteData(body_stream, body_length);
    out_packet.Reset();
    out_packet.Skip( sizeof(int16) + sizeof(int8) + sizeof(int8) );
    out_packet.Write16(0);//签名重置为0
    
    
    //5.checksum 校验
    uint16 my_signature;
    char* checksum_packetBody = reinterpret_cast<char *>(const_cast<char *>(out_packet.GetData()));
    net::checksum(checksum_packetBody, my_signature);
    //LOG_DEBUG2("Original signature[%d], myself signature[%d]", signature, my_signature);
    if(my_signature != signature) {
        //LOG_ERROR2("Original signature[%d], myself signature[%d], it's not correct packet.", signature, my_signature);
        //goto UNPAKBACK;
        //return false; TODO:为了测试, 兼容老包
    }
    
    
    //6.返回明文
    *out_stream = reinterpret_cast<void *>(const_cast<char *>(out_packet.GetData()));
    *out_stream_length = packet_length;
    //LOG_DEBUG2("PacketStream after packet length[%d]", packet_length);
    
    
    if (((is_zip_encrypt>>4) & 0xF) > 0 && body_stream) {
        delete body_stream;
        body_stream = NULL;
    }
    return true;
}
}
