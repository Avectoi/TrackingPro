package com.example.hyxja.football;

import android.content.Context;
import android.content.Intent;
import android.support.v4.util.Pair;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.RelativeLayout;

import java.util.List;

public class ThermActivity extends AppCompatActivity {
    ThermoView thermoView;
    Context mContext;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_therm);
        Intent intent = getIntent();
        int team = intent.getExtras().getInt("team");
        mContext = this;
        thermoView = new ThermoView(this);
        DbDataFetcher dbDataFetcher = new DbDataFetcher(mContext);
        List<List<Pair<Float,Float>>> posList = dbDataFetcher.getPlayerPosList();
        dbDataFetcher.closeDataBase();
        thermoView.setDataList(posList);
        thermoView.setTeam(team);
        RelativeLayout thermView = (RelativeLayout)findViewById(R.id.therm_view);
        if(thermView != null){
            thermView.addView(thermoView);
        }
    }
}
