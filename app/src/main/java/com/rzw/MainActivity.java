package com.rzw;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.rzplayer.JfOnPreparedListener;
import com.rzplayer.RzPlayer;

public class MainActivity extends AppCompatActivity {


    private RzPlayer rzPlayer;
    private String filePath = Environment.getExternalStorageDirectory() + "/test1.mp3";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        rzPlayer = new RzPlayer();
        // Example of a call to a native method
        Button bt_play = findViewById(R.id.bt_play);

        if(filePath!=null){
            rzPlayer.setDataResource(filePath);
        }

        bt_play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(rzPlayer!=null){
                    rzPlayer.onPrepared();
                }
            }
        });

        rzPlayer.setJfOnPreparedListener(new JfOnPreparedListener() {
            @Override
            public void onPrepared() {
                Log.e("ssssssssssssssssss","解码器已准备好");
                rzPlayer.start();
            }
        });

    }

}
