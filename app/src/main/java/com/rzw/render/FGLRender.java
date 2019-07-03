package com.rzw.render;

import android.opengl.GLES20;
import android.view.View;

import java.lang.reflect.Constructor;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class FGLRender extends Shape {

    private Shape shape;
    private Class<? extends Shape> clazz = Triangle.class;

    public FGLRender(View mView) {
        super(mView);
    }

    public void setShape(Class<? extends Shape> shape) {
        this.clazz = shape;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        GLES20.glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //设置背景颜色

        try {
            Constructor<? extends Shape> constructor = clazz.getDeclaredConstructor(View.class);
            constructor.setAccessible(true);
            shape = constructor.newInstance(mView);


        } catch (Exception e) {
            e.printStackTrace();
            shape = new Triangle(mView);
        }

        shape.onSurfaceCreated(gl, config);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);

        shape.onSurfaceChanged(gl, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT); //清除深度缓冲与颜色缓冲
        shape.onDrawFrame(gl);
    }
}
