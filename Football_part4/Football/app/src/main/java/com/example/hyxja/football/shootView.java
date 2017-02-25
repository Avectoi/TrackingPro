package com.example.hyxja.football;

import android.content.Context;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.support.v4.util.Pair;
import android.view.View;

import java.util.List;

/**
 * Created by hyxja on 2016/8/21.
 */
public class shootView extends View {
    List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> goalList, savedList, missList, PostList;
    Context mContext;
    int status = -1;
    int num = -1;
    public shootView(Context context) {
        super(context);
        mContext = context;
    }

    public void setData(List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> goalList,
                        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> savedList,
                        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> missList,
                        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> PostList){
        this.goalList = goalList;
        this.missList = missList;
        this.PostList = PostList;
        this.savedList = savedList;
    }

    public void setEmphasizedItem(int status, int num){
        this.status = status;
        this.num = num;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        Paint goalP = new Paint();
        Paint savedP = new Paint();
        Paint missP = new Paint();
        Paint postP = new Paint();
        goalP.setColor(Color.GREEN);
        goalP.setStrokeWidth(20);
        savedP.setColor(Color.RED);
        savedP.setStrokeWidth(20);
        missP.setColor(Color.YELLOW);
        missP.setStrokeWidth(20);
        postP.setColor(Color.BLUE);
        postP.setStrokeWidth(20);
        for(int i=0;i<goalList.size();i++){
            Rect dest;
            if(status == 1 && i == num){
                dest = new Rect(Utils.dip2px(mContext, goalList.get(i).first.first.first * 1f) - 40,
                        Utils.dip2px(mContext, goalList.get(i).first.first.second * 1f) - 40,
                        Utils.dip2px(mContext, goalList.get(i).first.first.first * 1f) + 40,
                        Utils.dip2px(mContext, goalList.get(i).first.first.second * 1f) + 40);
            }
            else{
                dest = new Rect(Utils.dip2px(mContext, goalList.get(i).first.first.first * 1f) - 25,
                        Utils.dip2px(mContext, goalList.get(i).first.first.second * 1f) - 25,
                        Utils.dip2px(mContext, goalList.get(i).first.first.first * 1f) + 25,
                        Utils.dip2px(mContext, goalList.get(i).first.first.second * 1f) + 25);
            }
            canvas.drawBitmap(BitmapFactory.decodeResource(getResources(), R.mipmap.goal), null, dest ,goalP);
        }
        for(int i=0;i<savedList.size();i++){
            Rect dest;
            if(status == 2 && num == i){
                dest = new Rect(Utils.dip2px(mContext, savedList.get(i).first.first.first*1f)-40,
                        Utils.dip2px(mContext,savedList.get(i).first.first.second*1f)-40,
                        Utils.dip2px(mContext, savedList.get(i).first.first.first*1f)+40,
                        Utils.dip2px(mContext,savedList.get(i).first.first.second*1f)+40);
            }
            else {
                dest = new Rect(Utils.dip2px(mContext, savedList.get(i).first.first.first*1f)-25,
                        Utils.dip2px(mContext,savedList.get(i).first.first.second*1f)-25,
                        Utils.dip2px(mContext, savedList.get(i).first.first.first*1f)+25,
                        Utils.dip2px(mContext,savedList.get(i).first.first.second*1f)+25);
            }
            canvas.drawBitmap(BitmapFactory.decodeResource(getResources(), R.mipmap.save), null, dest ,goalP);
        }
        for(int i=0;i<missList.size();i++){
            Rect dest;
            if(status == 3 && num == i) {
                dest = new Rect(Utils.dip2px(mContext, missList.get(i).first.first.first * 1f) - 40,
                        Utils.dip2px(mContext, missList.get(i).first.first.second * 1f) -40,
                        Utils.dip2px(mContext, missList.get(i).first.first.first * 1f) + 40,
                        Utils.dip2px(mContext, missList.get(i).first.first.second * 1f) +40);
            }
            else {
                dest = new Rect(Utils.dip2px(mContext, missList.get(i).first.first.first * 1f) - 25,
                        Utils.dip2px(mContext, missList.get(i).first.first.second * 1f) - 25,
                        Utils.dip2px(mContext, missList.get(i).first.first.first * 1f) + 25,
                        Utils.dip2px(mContext, missList.get(i).first.first.second * 1f) + 25);
            }
            canvas.drawBitmap(BitmapFactory.decodeResource(getResources(), R.mipmap.miss), null, dest ,goalP);
        }
        for(int i=0;i<PostList.size();i++){
            Rect dest ;
            if(status == 4 && num == i) {
                dest = new Rect(Utils.dip2px(mContext, PostList.get(i).first.first.first * 1f) - 40,
                        Utils.dip2px(mContext, PostList.get(i).first.first.second * 1f) - 40,
                        Utils.dip2px(mContext, PostList.get(i).first.first.first * 1f)  + 40,
                        Utils.dip2px(mContext, PostList.get(i).first.first.second * 1f) + 40);
            }
            else {
                dest = new Rect(Utils.dip2px(mContext, PostList.get(i).first.first.first * 1f) - 25,
                        Utils.dip2px(mContext, PostList.get(i).first.first.second * 1f) - 25,
                        Utils.dip2px(mContext, PostList.get(i).first.first.first * 1f) + 25,
                        Utils.dip2px(mContext, PostList.get(i).first.first.second * 1f) + 25);
            }
            canvas.drawBitmap(BitmapFactory.decodeResource(getResources(), R.mipmap.post), null, dest ,goalP);        }
    }
}
