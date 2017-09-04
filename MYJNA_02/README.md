
#### 首先是check.cpp文件：

```C
#include "check.h"
#include <string.h>
extern "C"{

bool checksum(const char* src_data, unsigned short&  check_ret) {
    bool ret = true;

    do {
        const int16_t* opt_data = reinterpret_cast<const int16_t*>(src_data);
        if (opt_data == NULL || src_data == NULL) {
            ret = false;
            /*TODO*/
            // warn_log();
            break;
        }
        int32_t accu_sum = 0;
        int32_t data_len = strlen(src_data);

        while (data_len > 1) {
            accu_sum += *(opt_data);
            opt_data++;
            data_len = data_len - 2;

            if (accu_sum & 0x80000000) {
                accu_sum = (accu_sum >> 16) + (accu_sum & 0xFFFF);
            }
        }

        if (data_len == 1) {
            accu_sum += *(reinterpret_cast<const int8_t*>(opt_data));
        }

        while (accu_sum >> 16) {
            accu_sum = (accu_sum >> 16) + (accu_sum & 0xFFFF);
        }
        check_ret = 0xFFFF & accu_sum;
        
//        check_ret = (accu_sum == 0xFFFF)?~accu_sum:accu_sum;

//        check_ret = (accu_sum == 0xFFFF)?accu_sum:~accu_sum;
    }while(0);

    return ret;
}
}

```

#### 然后是check.h文件

```h
#ifndef TASK_CHECKSUM_H
#define TASK_CHECKSUM_H

typedef signed char int8_t;
typedef short int  int16_t;
typedef int        int32_t;


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis          checksum加密算法
 *
 * @Param src_data    被加密的字符串 
 *
 * @Param check_ret   int16_t类型的加密结果
 *
 * @Returns           加密是否成功
 */
/* ----------------------------------------------------------------------------*/

extern "C" bool checksum(const char* src_data, unsigned short& check_ret);

#endif
```

编译C++代码，生成libcheck.so文件，把他们添加到已经准备好的jniLibs文件夹中，然后创建按一个CLibrary类：

```C++
//继承Library，用于加载库文件
public interface Clibrary extends Library {
    //加载libhello.so链接库
    Clibrary INSTANTCE = (Clibrary) Native.loadLibrary("check", Clibrary.class);
    //此方法为链接库中的方法
//    checksum(const char* src_data, unsigned short&  check_ret)
    int checksum(String src_data, IntByReference  check_ret);
}
```

MainActivity

```java
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

```


