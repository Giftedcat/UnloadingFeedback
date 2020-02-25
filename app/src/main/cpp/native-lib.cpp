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

    //获取分叉处的进程编号
    int pid = fork();
    //父进程编号
    int ppid;
    FILE *file;
    if (pid == 0) {
        //进程编号为0，说明是子进程
        LOGD("child progress pid = %d", pid);
        while (1) {
            sleep(2);
            //获取父进程编号
            ppid = getppid();
            LOGD("child progress ppid = %d", ppid);
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
    } else if (pid > 0) {
        //进程编号大于0，说明是父进程
        while (1) {
            sleep(2);
            LOGD("parent Progress pid = %d", pid);
        }
    } else{
        LOGD("parent Progress pid < 0");
    }

}