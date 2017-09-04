package com.afinalstone.androidstudy.myjna_01;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }

    public void onClick(View view){
        int result = Clibrary.INSTANTCE.sayHello();
        Log.d("MainActivity","sayHello的放回结果:"+result);
    }

}
