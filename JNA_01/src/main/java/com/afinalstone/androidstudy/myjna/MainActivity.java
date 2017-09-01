package com.afinalstone.androidstudy.myjna;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HashMap;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.ByteBufInputStream;

import static io.netty.buffer.Unpooled.buffer;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }



    public void onClickPacketStream(View view) {
        getData();
    }

    public void onClickUnpackStream(View view) {
        Clibrary.INSTANTCE.SetKey(22222);;
        int key =  Clibrary.INSTANTCE.GetKey();;
        System.out.println("key:"+key);
    }


    public void getData() {
        HashMap<String, Object> map = new HashMap<>();
        map.put("phone", "15267163669");
        map.put("pwd", "123456");
        byte type = 4;
        Short operateCode = 77;
        try {
            SocketDataPacket socketDataPacket = new SocketDataPacket(operateCode, type, map);
            writeSerializable(socketDataPacket);
            System.out.print("加密成功");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
    }

    public void writeSerializable( SocketDataPacket socketDataPacket ) {
        byte[] dataBody = socketDataPacket.getDataBody();
        ByteBuffer byteBuffer = ByteBuffer.allocate(26+dataBody.length);
        byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
        byteBuffer.putShort(socketDataPacket.getPacketLength());
        byteBuffer.put(socketDataPacket.getIsZipEncrypt());
        byteBuffer.put(socketDataPacket.getType());
        byteBuffer.putShort(socketDataPacket.getSignature());
        byteBuffer.putShort(socketDataPacket.getOperateCode());
        byteBuffer.putShort(socketDataPacket.getDataLength());
        byteBuffer.putInt(socketDataPacket.getTimestamp());
        byteBuffer.putLong(socketDataPacket.getSessionId());
        byteBuffer.putInt(socketDataPacket.getRequestId());
        byteBuffer.put(dataBody);
        try {
            byte[] bytes_01 = byteBuffer.array();
            String result_01 = new String(bytes_01,"UTF-8");
            System.out.println("原始数据长度："+bytes_01.length);
            System.out.println("原始数据：result_01："+result_01);
            //加密处理
            byte[] bytes_02 = socketDataPacketPacketStream(bytes_01);

            ByteBuf buffer_02 = buffer(bytes_02.length);
            buffer_02.writeBytes(bytes_02);

            ByteBufInputStream in_02 = new ByteBufInputStream(buffer_02);
            socketDataUnpackStream(in_02);


        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    private void socketDataUnpackStream(ByteBufInputStream byteBufInputStream) throws IOException {
        //解析数据
        byte[] bytes_decode = socketDataUnpackStream(InputStreamTOByte(byteBufInputStream));
        String result_01 = new String(bytes_decode,"UTF-8");
        System.out.println("解密后的數據："+result_01);

        ByteBuf buffer  = buffer(bytes_decode.length);
        buffer.writeBytes(bytes_decode);
        ByteBuf inputStream = buffer.order( ByteOrder.LITTLE_ENDIAN);

        SocketDataPacket SocketDataPacket_result = new SocketDataPacket();

        SocketDataPacket_result.setPacketLength(inputStream.readShort());
        SocketDataPacket_result.setIsZipEncrypt(inputStream.readByte());
        SocketDataPacket_result.setType(inputStream.readByte());
        SocketDataPacket_result.setSignature(inputStream.readShort());
        SocketDataPacket_result.setOperateCode(inputStream.readShort());
        SocketDataPacket_result.setDataLength(inputStream.readShort());
        SocketDataPacket_result.setTimestamp(inputStream.readInt());
        SocketDataPacket_result.setSessionId(inputStream.readLong());
        SocketDataPacket_result.setRequestId(buffer.readInt());


    }

    public byte[] InputStreamTOByte(InputStream in) throws IOException{
        ByteArrayOutputStream outStream = new ByteArrayOutputStream();
        byte[] data = new byte[1024*4];
        int count = -1;
        while((count = in.read(data,0,4096)) != -1)
            outStream.write(data, 0, count);
        data = null;
        return outStream.toByteArray();
    }

    public byte [] socketDataPacketPacketStream(byte [] bytes) throws IOException, ClassNotFoundException {
        int length = 64;
        Memory pointer = new Memory(bytes.length);
        pointer.write(0,bytes,0,length);
        PointerByReference out_stream = new PointerByReference(Pointer.NULL);;
        IntByReference out_stream_length = new IntByReference(0);
        int flag =  Clibrary.INSTANTCE.PacketStream(pointer, length ,out_stream, out_stream_length);;
        int length_result = out_stream_length.getValue();
        System.out.println("加密之后的长度："+length_result);
        System.out.println("加密是否成功："+flag);

        byte [] bytes_result = out_stream.getValue().getByteArray(0,length_result);
        return bytes_result;
    }

    public byte [] socketDataUnpackStream(byte [] bytes){
        int length = bytes.length;
        Memory pointer = new Memory(length);
        pointer.write(0,bytes,0,length);

        PointerByReference out_stream = new PointerByReference(Pointer.NULL);;
        IntByReference out_stream_length = new IntByReference(0);

        int flag =  Clibrary.INSTANTCE.UnpackStream(pointer, length ,out_stream, out_stream_length);;
        int length_result = out_stream_length.getValue();
        System.out.println("解密之后的长度："+length_result);
        System.out.println("解密是否成功："+flag);

        byte [] bytes_result = out_stream.getValue().getByteArray(0,length_result);
        return bytes_result;
    }

}
