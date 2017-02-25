package com.example.hyxja.football;


import android.support.v4.util.Pair;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by hyxja on 2016/10/26.
 */
public class PlayerLoad {
    private List<Pair<Float,Float>> postList;
    private double playerLoad = 0;
    public void setPostList(List<Pair<Float,Float>> postList){
        this.postList = postList;
        calcPlayerLoad();
    }
    private void calcPlayerLoad(){
        List<Double> speedList = new ArrayList<>();
        List<Double> accelerateList = new ArrayList<>();
        for(int i = 1;i<postList.size();i++){
            double speed = Math.sqrt((postList.get(i).first-postList.get(i-1).first)*(postList.get(i).first-postList.get(i-1).first)*(105/800)*(105/800)
                    +(postList.get(i).second-postList.get(i-1).second)*(postList.get(i).second-postList.get(i-1).second)*70/400*70/400)/0.04;
            speedList.add(speed);
        }
        for(int i=1;i<speedList.size();i++){
            double accelerate = (speedList.get(i)-speedList.get(i-1))/0.04;
            accelerateList.add(accelerate);
        }
        for (int i=1;i<accelerateList.size();i++){
            playerLoad = playerLoad+Math.abs(accelerateList.get(i)-accelerateList.get(i-1));
        }
    }
    public double getPlayerLoad(){
        return playerLoad;
    }
}
