package com.giftedcat.unloadingfeedback.application;

import android.app.Application;

public class MainApplication extends Application {

    static {
        System.loadLibrary("native-lib");
    }

}
