JNA(Java Native Access)框架是一个开源的Java框架，是SUN公司主导开发的，建立在经典的JNI的基础之上的一个框架。使用JNI调用共享类库（.dll/.so文件）是非常麻烦的事情，既需要编写java代码，又要编写C语言的代理方法，这其中需要很多数据类型的转换，是让人非常头痛。JNA框架就是为了解决这些问题和繁琐的事情而开发的，它提供一组Java工具类用于在运行期动态访问系统本地共享类库而不需要编写任何Native/JNI代码。开发人员只要在一个java接口中描述目标native library的函数与结构，JNA将自动实现Java接口到native function的映射，大大降低了Java调用本体共享库的开发难度。JNA与.NET平台上的P/Invoke机制一样简单和方便。

你只需要下载一个jar包，就可以使用JNA的强大功能方便地调用动态链接库中的C函数。
下载地址是：https://github.com/twall/jna

一、JNA实战笔记汇总<一> 简单认识JNA|成功调用JNA

假设有一个动态链接库: CnblogsJna.so，

在app下面建立一個jni文件夾，添加库函数文件sayhello.c：

```C
#include "sayhello.h"

int sayHello(){
    printf("Hello World!");
    return 1;
}

```

以及头文件sayhello.h：

```C
#include <stdio.h>
int sayHello();
```

其他就是配置我们编译C/C++代码的环境了，如果不懂可以参考这篇文章：[NDK学习笔记<一> 初步认识JNI|成功搭建NDK开发环境](http://blog.csdn.net/abc6368765/article/details/64438213)

成功配置我们的C/C++编译环境后，在lib目录下面成功编译出我们需要的.so文件。然后把jna.jar和相关的.so文件添加到项目
中，和JNA相关的jna.jar和.so库文件可以在这里下载：[JNA开发的jna.jar以及.so库文件](http://download.csdn.net/download/abc6368765/9963681)

到这里位置，整个项目结构是这样的



http://www.cnblogs.com/gulvzhe/archive/2012/06/29/2569333.html