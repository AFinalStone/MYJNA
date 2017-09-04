package com.afinalstone.androidstudy.myjna_02;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

//继承Library，用于加载库文件
public interface Clibrary extends Library {
    //加载libhello.so链接库
    Clibrary INSTANTCE = (Clibrary) Native.loadLibrary("check", Clibrary.class);
    //此方法为链接库中的方法
//    checksum(const char* src_data, unsigned short&  check_ret)
    int checksum(String src_data, IntByReference  check_ret);

}