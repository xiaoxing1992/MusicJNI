package com.rzplayer;

public class RzPlayer {

    private String url;

    private MediaErrorListener mediaErrorListener;
    private JfOnPreparedListener jfOnPreparedListener;

    public void setJfOnPreparedListener(JfOnPreparedListener jfOnPreparedListener) {
        this.jfOnPreparedListener = jfOnPreparedListener;
    }

    public void setMediaErrorListener(MediaErrorListener mediaErrorListener) {
        this.mediaErrorListener = mediaErrorListener;
    }

    static {
        System.loadLibrary("avutil-55");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
        System.loadLibrary("postproc-54");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public void setDataResource(String filePath) {
        this.url = filePath;
    }

    public void onErrorListener(int code, String msg) {
        if (mediaErrorListener != null) {
            mediaErrorListener.onError(code, msg);
        }
    }

    public void onPrepared() {
        if (url == null) {
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                jniPrepared(url);
            }
        }).start();
    }

    public void onCallPrepared(){
        if (jfOnPreparedListener != null)
        {
            jfOnPreparedListener.onPrepared();
        }
    }

    public native void jniPrepared(String url);

    public void start() {
        if (url == null) {
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_start();
            }
        }).start();
    }

    public native void n_start();
}
