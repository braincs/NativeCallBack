package com.attrsc.nativecallback;

/**
 * Created by Shuai
 * 28/02/2019.
 */

public class NativeHelper {

    static {
        System.loadLibrary("native-lib");
    }

    public native String _helloFromJni();
    public native void _setListener(JNIListener listener);
    public native void _start();
    public native void _init();
    public native void _release();



    //test queue via C&S
    public native void _initConsumerProducer();
    public native void _startConsumerProducer();
    public native void _setListenerConsumerProducer(JNIListener listener);
    public native void _releaseConsumerProducer();

//    public native void _postImage(byte[] data, int w, int h);
}
