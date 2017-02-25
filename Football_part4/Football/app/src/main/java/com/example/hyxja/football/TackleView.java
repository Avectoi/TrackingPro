package com.example.hyxja.football;

import android.content.Context;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.support.v4.util.Pair;
import android.util.Log;
import android.view.View;

import java.util.List;

/**
 * Created by hyxja on 2016/8/21.
 */
public class TackleView extends View {
    List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> tackleList;
    Context mContext;
    int num = -1;
    public TackleView(Context context) {
        super(context);
        mContext = context;
    }

    public void setData(List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> tackleList){
        this.tackleList = tackleList;
    }

    public void setEmphasizedItem(int num){
        this.num = num;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        Paint smallP = new Paint();
        smallP.setColor(Color.BLUE);
        smallP.setStrokeWidth(20);
        Paint bigP = new Paint();
        bigP.setColor(Color.GREEN);
        bigP.setStrokeWidth(40);
        for(int i=0;i<tackleList.size();i++){
            Rect dest;
            if(i == num){
               canvas.drawPoint(Utils.dip2px(mContext, tackleList.get(i).first.first.first * 1f),
                       Utils.dip2px(mContext, tackleList.get(i).first.first.second * 1f) , bigP);
            }
            else{
                canvas.drawPoint(Utils.dip2px(mContext, tackleList.get(i).first.first.first * 1f),
                        Utils.dip2px(mContext, tackleList.get(i).first.first.second * 1f) , smallP);
            }
        }
    }
}
