package com.example.hyxja.football;

import android.content.Intent;
import android.graphics.Color;
import android.support.v4.util.Pair;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.widget.TextView;

import com.github.mikephil.charting.charts.PieChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.PieData;
import com.github.mikephil.charting.data.PieDataSet;

import java.util.ArrayList;
import java.util.List;

public class ShootDetailActivity extends AppCompatActivity {
    private int goalNum, shootNum;
    private PieChart pieChart;
    private TextView smallBoxLeft, smallBoxRight, bigBoxLeft, bigBoxMiddle, bigBoxRight,
            farLeft, left, right, farRight, far;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_shoot_detail);
        Intent intent = getIntent();
        setPieChart(intent);
        findViews();
        int team = intent.getExtras().getInt("team");
        boolean isTeam = intent.getExtras().getBoolean("isTeam");
        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> goalList;
        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> missList;
        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> postList;
        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> savedList;
        DbDataFetcher dbDataFetcher = new DbDataFetcher(this);
        if(isTeam){
            goalList = dbDataFetcher.getOneEventOfTeamList(16, team-1);
            missList = dbDataFetcher.getOneEventOfTeamList(13, team-1);
            postList = dbDataFetcher.getOneEventOfTeamList(14, team-1);
            savedList = dbDataFetcher.getOneEventOfTeamList(62, team-1);
        }
        else {
            goalList = dbDataFetcher.getOneEventOfPlayerList(16, team-2);
            missList = dbDataFetcher.getOneEventOfPlayerList(13, team-2);
            postList = dbDataFetcher.getOneEventOfPlayerList(14, team-2);
            savedList = dbDataFetcher.getOneEventOfPlayerList(62, team-2);
        }
        List<List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>>> shootList = new ArrayList<>();
        shootList.add(goalList);
        shootList.add(missList);
        shootList.add(postList);
        shootList.add(savedList);
        int smallBoxLeftNum = 0;
        int smallBoxRightNum = 0;
        int bigBoxLeftNum = 0;
        int bigBoxMiddleNum = 0;
        int bigBoxRightNum = 0;
        int farLeftNum = 0;
        int farRightNum = 0;
        int leftNum = 0;
        int rightNum = 0;
        int farNum = 0;
        if(team ==1 || team ==3 || team==4 || team ==5||team==6||team==7||team==8||team==13||team==14){
            for(int i=0;i<shootList.size();i++) {
                for (int j = 0; j < shootList.get(i).size(); j++) {
                    if (shootList.get(i).get(j).first.first.first < 400) {
                        farNum++;
                    } else if (shootList.get(i).get(j).first.first.first < 650) {
                        if (shootList.get(i).get(j).first.first.second > 417) {
                            farLeftNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 250) {
                            leftNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 83) {
                            rightNum++;
                        } else {
                            farRightNum++;
                        }
                    } else if (shootList.get(i).get(j).first.first.first < 750) {
                        if (shootList.get(i).get(j).first.first.second > 417) {
                            farLeftNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 250) {
                            bigBoxLeftNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 83) {
                            bigBoxRightNum++;
                        } else {
                            farRightNum++;
                        }
                    } else {
                        if (shootList.get(i).get(j).first.first.second > 417) {
                            farLeftNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 333) {
                            bigBoxLeftNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 250) {
                            smallBoxLeftNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 167) {
                            smallBoxRightNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 83) {
                            bigBoxRightNum++;
                        } else {
                            farRightNum++;
                        }
                    }
                }
            }
        }
        else{
            for(int i=0;i<shootList.size();i++) {
                for (int j = 0; j < shootList.get(i).size(); j++) {
                    if (shootList.get(i).get(j).first.first.first > 400) {
                        farNum++;
                    } else if (shootList.get(i).get(j).first.first.first > 150) {
                        if (shootList.get(i).get(j).first.first.second > 417) {
                            farRightNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 250) {
                            rightNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 83) {
                            leftNum++;
                        } else {
                            farLeftNum++;
                        }
                    } else if (shootList.get(i).get(j).first.first.first >50) {
                        if (shootList.get(i).get(j).first.first.second > 417) {
                            farRightNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 250) {
                            bigBoxRightNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 83) {
                            bigBoxLeftNum++;
                        } else {
                            farLeftNum++;
                        }
                    } else {
                        if (shootList.get(i).get(j).first.first.second > 417) {
                            farRightNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 333) {
                            bigBoxRightNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 250) {
                            smallBoxRightNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 167) {
                            smallBoxLeftNum++;
                        } else if (shootList.get(i).get(j).first.first.second > 83) {
                            bigBoxLeftNum++;
                        } else {
                            farLeftNum++;
                        }
                    }
                }
            }
        }
        far.setText(farNum+"");
        farLeft.setText(farLeftNum+"");
        farRight.setText(farRightNum+"");
        left.setText(leftNum+"");
        right.setText(rightNum+"");
        bigBoxLeft.setText(bigBoxLeftNum+"");
        bigBoxRight.setText(bigBoxRightNum+"");
        bigBoxMiddle.setText(bigBoxMiddleNum+"");
        smallBoxLeft.setText(smallBoxLeftNum+"");
        smallBoxRight.setText(smallBoxRightNum+"");
        dbDataFetcher.closeDataBase();
    }

    private void findViews() {
        smallBoxLeft = (TextView)findViewById(R.id.small_box_left);
        smallBoxRight = (TextView)findViewById(R.id.small_box_right);
        bigBoxLeft = (TextView)findViewById(R.id.big_box_left);
        bigBoxMiddle = (TextView)findViewById(R.id.big_box_middle);
        bigBoxRight = (TextView)findViewById(R.id.big_box_right);
        farLeft = (TextView)findViewById(R.id.far_left);
        left = (TextView)findViewById(R.id.left);
        right = (TextView)findViewById(R.id.right);
        farRight = (TextView)findViewById(R.id.far_right);
        far = (TextView)findViewById(R.id.far);
    }

    private void setPieChart(Intent intent) {
        goalNum = intent.getIntExtra("goalNum", 0);
        shootNum = intent.getIntExtra("shootNum",0);
        pieChart = (PieChart)findViewById(R.id.shoot_pie_chart);
        List<String> name = new ArrayList<String>();
        name.add("goal");
        name.add("not goal");
        List<Entry> dataList = new ArrayList<>();
        int goalPercent = 0;
        if(shootNum != 0){
            goalPercent = goalNum*100/shootNum;
        }
        int notGoalPercent = 100-goalPercent;
        dataList.add(new Entry(goalPercent,0));
        dataList.add(new Entry(notGoalPercent,1));
        PieDataSet pieDataSet = new PieDataSet(dataList, "goal/shoot");
        ArrayList<Integer> colors = new ArrayList<>();
        colors.add(Color.rgb(205, 205, 205));
        colors.add(Color.rgb(114, 188, 223));
        pieDataSet.setColors(colors);
        DisplayMetrics metrics = getResources().getDisplayMetrics();
        float px = 5 * (metrics.densityDpi / 160f);
        pieDataSet.setSelectionShift(px);
        pieDataSet.setSliceSpace(0f);
        PieData pieData = new PieData(name,pieDataSet);
        //pieChart.setHoleRadius(60f);  //半径
        //pieChart.setTransparentCircleRadius(64f); // 半透明圈
        pieChart.setDescription("goal/shoot");
        pieChart.setDrawHoleEnabled(true);
        pieChart.setUsePercentValues(true);
        pieChart.setData(pieData);
        Legend mLegend = pieChart.getLegend();  //设置比例图
        mLegend.setPosition(Legend.LegendPosition.RIGHT_OF_CHART);  //最右边显示
//      mLegend.setForm(LegendForm.LINE);  //设置比例图的形状，默认是方形
        mLegend.setXEntrySpace(7f);
        mLegend.setYEntrySpace(5f);
    }
}
