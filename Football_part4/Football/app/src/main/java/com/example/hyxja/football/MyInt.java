package com.example.hyxja.football;

/**
 * Created by hyxja on 2016/7/25.
 */
public class MyInt{
    private int pos;

    MyInt(int pos){
        this.pos = pos;
    }

    public void setValue(int pos){
        this.pos = pos;
    }
    public int getValue(){
        return pos;
    }
}
