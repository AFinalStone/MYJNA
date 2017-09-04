package com.afinalstone.androidstudy.myjna_02;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.ptr.PointerByReference;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClick(View view){
        IntByReference check_ret = new IntByReference();
        int flag = Clibrary.INSTANTCE.checksum("123",check_ret);
        Log.d("MainActivity","checksum的返回标记:"+flag);
        Log.d("MainActivity","checksum的返回结果:"+check_ret.getValue());

    }
}
