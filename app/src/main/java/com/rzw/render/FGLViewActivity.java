package com.rzw.render;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.rzw.R;

public class FGLViewActivity extends AppCompatActivity {
    private FGLView mGLView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fglview);

        mGLView= (FGLView) findViewById(R.id.mGLView);
        mGLView.setShape(Triangle.class);


    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }
}
