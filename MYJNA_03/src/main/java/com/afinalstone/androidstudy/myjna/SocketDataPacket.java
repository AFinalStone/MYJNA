package com.afinalstone.androidstudy.myjna;


import com.sun.jna.Library;
import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

import org.json.JSONObject;

import java.io.IOException;
import java.io.OutputStream;
import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.HashMap;

import io.netty.buffer.ByteBufInputStream;

/**
 * Created by yaowang on 2017/2/18.
 * Change by yaolei on 2017-8-23 15:03:19  Socket数据包对象
 */

public class SocketDataPacket implements Serializable {

    private Short packetLength = 24;
    private byte isZipEncrypt = 0;
    private byte type = 0;
    private Short signature = 0;
    private Short operateCode = 0;
    private Short dataLength = 0;
    //包发送的时间
    private int timestamp = 99;

    //SocketAPIRequestManager对象自身维持一个sessionID自增对象，
    // 一般是通过getSessionId把该对象的自增sessionID赋值给这里的sessionId，以此來保证sessionID的唯一性
    private Long sessionId = 8877L;

    private int requestId = 0;

    private byte[] dataBody;


    public SocketDataPacket() {

    }

    /**
     * @param operateCode 具体的操作行为编码，
     * @param type
     * @param dataBody
     */
    public SocketDataPacket(short operateCode, byte type, byte[] dataBody){
        this.operateCode = operateCode;
        this.type = type;
        this.dataBody = dataBody;
        this.packetLength = (short)(24 + dataBody.length);
        this.dataLength = (short)dataBody.length;
    }

    public SocketDataPacket(short operateCode, byte type, String jsonBody) throws UnsupportedEncodingException {

        this(operateCode,type,jsonBody.getBytes("UTF-8"));
        System.out.println("jsonBody:"+jsonBody);
    }

    public SocketDataPacket(short operateCode, byte type, JSONObject jsonObject) throws UnsupportedEncodingException {
        this(operateCode,type,jsonObject.toString());
    }


    public SocketDataPacket(short operateCode, byte type, HashMap<String,Object> map) throws UnsupportedEncodingException {
       this(operateCode,type,new JSONObject(map));
    }


    public Short getPacketLength() {
        return packetLength;
    }

    public void setPacketLength(Short packetLength) {
        this.packetLength = packetLength;
    }

    public byte getIsZipEncrypt() {
        return isZipEncrypt;
    }

    public void setIsZipEncrypt(byte isZipEncrypt) {
        this.isZipEncrypt = isZipEncrypt;
    }

    public byte getType() {
        return type;
    }

    public void setType(byte type) {
        this.type = type;
    }

    public Short getSignature() {
        return signature;
    }

    public void setSignature(Short signature) {
        this.signature = signature;
    }

    public Short getOperateCode() {
        return operateCode;
    }

    public void setOperateCode(Short operateCode) {
        this.operateCode = operateCode;
    }

    public Short getDataLength() {
        return dataLength;
    }

    public void setDataLength(Short dataLength) {
        this.dataLength = dataLength;
    }

    public int getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(int timestamp) {
        this.timestamp = timestamp;
    }

    public Long getSessionId() {
        return sessionId;
    }

    public void setSessionId(Long sessionId) {
        this.sessionId = sessionId;
    }

    public int getRequestId() {
        return requestId;
    }

    public void setRequestId(int requestId) {
        this.requestId = requestId;
    }

    public byte[] getDataBody() {
        return dataBody;
    }

    public void setDataBody(byte[] dataBody) {
        this.dataBody = dataBody;
    }

    public boolean writeSerializable(OutputStream outputStream) throws IOException {
        ByteBuffer byteBuffer = ByteBuffer.allocate(26+dataBody.length);
        byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
        byteBuffer.putShort(packetLength);
        byteBuffer.put(isZipEncrypt);
        byteBuffer.put(type);
        byteBuffer.putShort(signature);
        byteBuffer.putShort(operateCode);
        byteBuffer.putShort(dataLength);
        byteBuffer.putInt(timestamp);
        byteBuffer.putLong(sessionId);
        byteBuffer.putInt(requestId);
        byteBuffer.put(dataBody);
        outputStream.write(byteBuffer.array());

        return true;
    }




    public boolean readSerializable(ByteBufInputStream in) throws IOException {


        packetLength = in.readShort();
        isZipEncrypt = in.readByte();
        type = in.readByte();
        signature = in.readShort();
        operateCode = in.readShort();
        dataLength = in.readShort();
        timestamp = in.readInt();
        sessionId = in.readLong();
        requestId = in.readInt();
        if( dataLength > packetLength - 26  ) {
            return false;
        }
        dataBody = new byte[dataLength];

        in.read(dataBody);
        return true;
    }

    @Override
    public String toString() {
        return "SocketDataPacket{" +
                "packetLength=" + packetLength +
                ", isZipEncrypt=" + isZipEncrypt +
                ", type=" + type +
                ", signature=" + signature +
                ", operateCode=" + operateCode +
                ", dataLength=" + dataLength +
                ", timestamp=" + timestamp +
                ", sessionId=" + sessionId +
                ", requestId=" + requestId +
                ", dataBody=" + Arrays.toString(dataBody) +
                '}';
    }
}
