package com.attrsc.nativecallback;

/**
 * Created by Shuai
 * 28/02/2019.
 */

public interface JNIListener {
//    void onFrame(byte[] data, int width, int height, int rotate);
    void notify(int what, int arg1, int arg2, Object object);
}
