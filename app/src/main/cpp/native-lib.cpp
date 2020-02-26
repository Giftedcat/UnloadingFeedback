#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <string>
#include <unistd.h>
#include <android/log.h>

#define LOG_TAG "unloadingfeedback"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT void JNICALL
Java_com_giftedcat_unloadingfeedback_utils_JniUtil_fork(JNIEnv *env, jclass type) {

    //获取分叉出来的进程编号
    int id = fork();
    //父进程编号
    int ppid;
    //进程编号
    int pid;
    FILE *file;
    if (id == 0) {
        //分叉出来的进程编号为0，说明已经无法再分叉了，是子进程
        while (1) {
            sleep(2);
            //获取进程编号
            pid = getpid();
            //获取父进程编号
            ppid = getppid();
            LOGD("progress is Running && pid = %d && ppid = %d", pid, ppid);
            //父进程编号为1，说明父进程已经被杀死，这时子进程的父进程就变成了系统进程，即为1
            if (ppid == 1) {
                //根据包名获取该app的文件
                file = fopen("/data/data/com.giftedcat.unloadingfeedback", "r");
                //文件的NULL，说明该包名的app已经被卸载
                if (file == NULL) {
                    //使用am命令打开浏览器，让用户填写卸载反馈
                    execlp("am", "am", "start", "--user", "0", "-a", "android.intent.action.VIEW",
                           "-d", "http://www.baidu.com", NULL);
                }
            }
        }
    } else if (id > 0){
        // 分叉出来的编号大于0，说明是主进程
    }

}