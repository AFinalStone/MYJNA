package com.afinalstone.androidstudy.myjna;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

//继承Library，用于加载库文件
    public interface Clibrary extends Library {
        //加载libhello.so链接库
        Clibrary INSTANTCE = (Clibrary) Native.loadLibrary("test", Clibrary.class);
        //此方法为链接库中的方法

        int GetKey();

        void SetKey( int key);


        //明文->密文
        int PacketStream(Pointer in_stream, int in_stream_length,
                         PointerByReference out_stream, IntByReference out_stream_length);

        //密文->明文
        int UnpackStream(Pointer in_stream, int in_stream_length,
                         PointerByReference out_stream, IntByReference out_stream_length);
    }