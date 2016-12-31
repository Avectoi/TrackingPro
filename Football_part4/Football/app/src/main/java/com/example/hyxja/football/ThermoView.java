package com.example.hyxja.football;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.support.v4.util.Pair;
import android.view.View;

import java.util.List;

/**
 * Created by hyxja on 2016/10/19.
 */
public class ThermoView extends View {
    Context mContext;
    int team;
    List<List<Pair<Float,Float>>> posList;

    public ThermoView(Context context) {
        super(context);
        mContext = context;
    }

    public void setDataList(List<List<Pair<Float,Float>>> posList){
        this.posList = posList;
    }

    public void setTeam(int team){
        this.team = team;
    }
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        Paint P = new Paint();
        P.setColor(getResources().getColor(R.color.tranparentYellow));
        Paint fastP = new Paint();
        fastP.setColor(getResources().getColor(R.color.tranparentRed));
        for(int i=0;i<posList.size();i++){
            if(team == -1) {
                if(i==0||i==1||i==2||i==3||i==4||i==5||i==10||i==11) {
                    for (int j = 1; j < posList.get(i).size(); j++) {
                        double speed = Math.sqrt((posList.get(i).get(j).first-posList.get(i).get(j-1).first)*(posList.get(i).get(j).first-posList.get(i).get(j-1).first)*(105/800)*(105/800)
                                +(posList.get(i).get(j).second-posList.get(i).get(j-1).second)*(posList.get(i).get(j).second-posList.get(i).get(j-1).second)*70/400*70/400)/0.04;
                        if(speed < 19.8/3.6) {
                            canvas.drawCircle(Utils.dip2px(mContext, posList.get(i).get(j).first * 1f),
                                    Utils.dip2px(mContext, (500 - posList.get(i).get(j).second) * 1f), 30, P);
                        }
                        else {
                            canvas.drawCircle(Utils.dip2px(mContext, posList.get(i).get(j).first * 1f),
                                    Utils.dip2px(mContext, (500 - posList.get(i).get(j).second) * 1f), 30, fastP);
                        }
                    }
                }
            }
            else if(team == -2){
                if(i==6||i==7||i==9||i==12||i==13||i==14||i==15||i==16) {
                    for (int j = 1; j < posList.get(i).size(); j++) {
                        double speed = Math.sqrt((posList.get(i).get(j).first-posList.get(i).get(j-1).first)*(posList.get(i).get(j).first-posList.get(i).get(j-1).first)*(105/800)*(105/800)
                                +(posList.get(i).get(j).second-posList.get(i).get(j-1).second)*(posList.get(i).get(j).second-posList.get(i).get(j-1).second)*70/400*70/400)/0.04;
                        if(speed < 19.8/3.6) {
                            canvas.drawCircle(Utils.dip2px(mContext, posList.get(i).get(j).first * 1f),
                                    Utils.dip2px(mContext, (500 - posList.get(i).get(j).second) * 1f), 30, P);
                        }
                        else {
                            canvas.drawCircle(Utils.dip2px(mContext, posList.get(i).get(j).first * 1f),
                                    Utils.dip2px(mContext, (500 - posList.get(i).get(j).second) * 1f), 30, fastP);
                        }
                    }
                }
            }
            else{
                if(i == team) {
                    for (int j = 1; j < posList.get(i).size(); j++) {
                        double speed = Math.sqrt((posList.get(i).get(j).first-posList.get(i).get(j-1).first)*(posList.get(i).get(j).first-posList.get(i).get(j-1).first)*(105/800)*(105/800)
                                +(posList.get(i).get(j).second-posList.get(i).get(j-1).second)*(posList.get(i).get(j).second-posList.get(i).get(j-1).second)*70/400*70/400)/0.04;
                        if(speed < 19.8/3.6) {
                            canvas.drawCircle(Utils.dip2px(mContext, posList.get(i).get(j).first * 1f),
                                    Utils.dip2px(mContext, (500 - posList.get(i).get(j).second) * 1f), 30, P);
                        }
                        else {
                            canvas.drawCircle(Utils.dip2px(mContext, posList.get(i).get(j).first * 1f),
                                    Utils.dip2px(mContext, (500 - posList.get(i).get(j).second) * 1f), 30, fastP);
                        }
                    }
                }
            }
        }
    }
}
