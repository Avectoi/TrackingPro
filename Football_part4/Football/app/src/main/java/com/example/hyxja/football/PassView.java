package com.example.hyxja.football;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.support.v4.util.Pair;
import android.util.Log;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.MultiAutoCompleteTextView;

import java.util.List;

/**
 * Created by hyxja on 2016/8/18.
 */
public class PassView extends View {
    int emphasizedId = -1;
    Context mContext;
    List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> dataList;
    public PassView(Context context) {
        super(context);
        mContext = context;
    }

    public void setDataList(List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> dataList){
        this.dataList = dataList;
    }

    public void setEmphasizeItem(int id){
        emphasizedId = id;
    }
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        Paint p = new Paint();
        p.setColor(Color.GREEN);
        p.setStrokeWidth(5);
        Paint bigP = new Paint();
        bigP.setColor(Color.RED);
        bigP.setStrokeWidth(10);
        Log.e("fuck","datalistsize：　"+dataList.size());
        for(int i=0;i<dataList.size();i++){
            if(i == emphasizedId){
                canvas.drawLine(Utils.dip2px(mContext, dataList.get(i).first.first.first * 1f), Utils.dip2px(mContext, dataList.get(i).first.first.second * 1f),
                        Utils.dip2px(mContext, dataList.get(i).first.second.first * 1f), Utils.dip2px(mContext, dataList.get(i).first.second.second * 1f), bigP);
            }
            else {
                canvas.drawLine(Utils.dip2px(mContext, dataList.get(i).first.first.first * 1f), Utils.dip2px(mContext, dataList.get(i).first.first.second * 1f),
                        Utils.dip2px(mContext, dataList.get(i).first.second.first * 1f), Utils.dip2px(mContext, dataList.get(i).first.second.second * 1f), p);
            }
        }
    }
}
