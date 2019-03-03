package com.attrsc.nativecallback;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {


    private NativeHelper helper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
//        helper = new NativeHelper();
//
//        helper._init();
//        helper._setListener(new JNIListener() {
//            @Override
//            public void notify(int what, int arg1, int arg2, Object object) {
//                byte[] data = (byte[])object;
//                Log.d("debug", "message what = " + what +",\n" +
//                        "arg1 = " + arg1 + ",\n" +
//                        "arg2 = " + arg2 + ",\n" +
//                        "len = " + data.length + ",\n" +
//                        "object = " + Arrays.toString(data));
//            }
//        });
//        helper._start();
        helper = new NativeHelper();
        tv.setText(helper._helloFromJni());
        helper._initConsumerProducer();
        helper._setListenerConsumerProducer(new JNIListener() {
            @Override
            public void notify(int what, int arg1, int arg2, Object object) {
                byte[] data = (byte[])object;
                Log.d("debug", "message what = " + what + ",\n" +
                        "arg1 = " + arg1 + ",\n" +
                        "arg2 = " + arg2 + ",\n" +
                        "len = " + data.length + ",\n" +
                        "object = " + Arrays.toString(data));

            }
        });

        helper._startConsumerProducer();

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    @Override
    protected void onDestroy() {
        super.onDestroy();
//        helper._release();
        helper._releaseConsumerProducer();
    }
}
