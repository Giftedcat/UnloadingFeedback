# 前言

我们的app有的时候难免会有很多问题，导致用户卸载掉我们的app。而我们作为开发者需要采集用户卸载的原因，来更好的完善我们的app，所以需要在用户卸载后弹出我们的反馈页面，让用户把自己不满意的地方反馈给我们。

本文为ndk学习文章，经本人对4.4，5.1，6.0，7.1的版本测试，该功能仅适用于低版本的Android设备，高版本中子进程会连带父进程一起被杀死，所以仅供学习。

# 效果

![image](https://upload-images.jianshu.io/upload_images/20395467-1b31bb117251858b.gif?imageMogr2/auto-orient/strip)

# 原理解析

主要是通过主进程分叉出子进程循环检查app是否被卸载，当发现app被卸载后打开网页来实现

这里就有人要问了，为什么当app被卸载后子进程还能运行呢

我们可以在代码中加入一些log来观察一下进程编号的关系与变化

先在代码中定义一下宏

```
#define LOG_TAG "unloadingfeedback"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
```

然后在fork出来的子进程循环中打印出进程编号

```
            //获取进程编号
            pid = getpid();
            //获取父进程编号
            ppid = getppid();
            LOGD("progress is Running && pid = %d && ppid = %d", pid, ppid);
```

#### 当app运行的时候

让我们来看下打印出来的日志

![image](https://upload-images.jianshu.io/upload_images/20395467-b7388c77fb925a45.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

进程id为2289，父进程id为2275

为了验证一下，我们使用adb shell，执行一下ps命令，来看一下模拟器当前所有的进程

![image](https://upload-images.jianshu.io/upload_images/20395467-baa577edae552893.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

左边一列是进程id，右边一列是父进程id，可以看到2289的父进程为2275 包名为com.giftecat.unloadingfeedback

#### 当app被杀死的时候

我们还是先来看下日志是怎么打印的

![image](https://upload-images.jianshu.io/upload_images/20395467-aa163447d6686d79.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们的进程id还是2289，但是发现父进程id变成了1，我们再用命令行来看一下

![image](https://upload-images.jianshu.io/upload_images/20395467-d068cb3081a72d6c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

而这个进程id为1的进程，就是我们的系统进程

![image](https://upload-images.jianshu.io/upload_images/20395467-b13c20a1fe872b3c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 结论

使用主进程分叉出一个子进程后，当主进程被杀死后，子进程就会被系统进程托管

可以理解为子进程的父亲被杀死了，变成了孤儿后系统进程便会将他领养

![image](https://upload-images.jianshu.io/upload_images/20395467-de2fa83782dba99f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#### 判断app是否被卸载和打开反馈网页

懂得原理之后事情就变得简单了

我们判断一下ppid是否为1，然后根据包名轮询检查文件是否还在

```
file = fopen("/data/data/com.giftedcat.unloadingfeedback", "r");
if (file == NULL) {
}
```

最后使用am命令打开浏览器

即am start --user 0 -a android.intent.action.VIEW -d http://www.baidu.com

```
execlp("am", "am", "start", "--user", "0", "-a", "android.intent.action.VIEW",
                           "-d", "http://www.baidu.com", NULL);
```


觉得有帮助的话可以给我点个star哦，感谢
