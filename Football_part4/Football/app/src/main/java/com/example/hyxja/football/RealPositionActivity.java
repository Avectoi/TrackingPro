package com.example.hyxja.football;

import android.content.Intent;
import android.support.v4.util.Pair;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import java.util.ArrayList;
import java.util.List;

public class RealPositionActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_real_position);
        RelativeLayout positionLayout = (RelativeLayout)findViewById(R.id.position_view);
        DbDataFetcher dbDataFetcher = new DbDataFetcher(this);
        Intent intent = getIntent();
        int team = intent.getExtras().getInt("team");
        List<List<Pair<Float,Float>>> posList = dbDataFetcher.getPlayerPosList();
        List<List<Pair<Float,Float>>> targetList = new ArrayList<>();
        if(team == -1){
            for(int i=0;i<posList.size();i++){
                if(i==0||i==1||i==2||i==3||i==4||i==5||i==10||i==11){
                    targetList.add(posList.get(i));
                }
            }
        }
        else if(team == -2){
            for(int i=0;i<posList.size();i++){
                if(i==6||i==7||i==9||i==12||i==13||i==14||i==15||i==16){
                    targetList.add(posList.get(i));
                }
            }
        }
        RealPositionView positionView = new RealPositionView(this);
        positionView.setAllPosList(targetList);
        positionView.setTeam(-team);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        if (positionLayout != null) {
            positionLayout.addView(positionView, layoutParams);
        }
    }
}
