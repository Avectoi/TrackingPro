package com.example.hyxja.football;

import android.content.Context;
import android.content.Intent;
import android.support.v4.util.Pair;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public class ShootActivity extends AppCompatActivity {
    shootView shootView1;
    Context mContext;
    TextView detailText;
    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_shoot);
        RelativeLayout shootView = (RelativeLayout)findViewById(R.id.shoot_rl);
        mContext = this;
        DbDataFetcher dbDataFetcher = new DbDataFetcher(this);
        RecyclerView shootRv;
        shootRv = (RecyclerView)findViewById(R.id.shoot_rv);
        detailText = (TextView)findViewById(R.id.shoot_detail_text);
        final Intent intent = getIntent();
        final int team = intent.getExtras().getInt("team");
        final boolean isTeam = intent.getExtras().getBoolean("isTeam");
        List goalList, goalTimeList;
        List missList, missTimeList;
        List postList, postTimeList;
        List savedList, savedTimeList;
        if(isTeam){
            goalList = dbDataFetcher.getOneEventOfTeamList(16, team-1);
            goalTimeList = dbDataFetcher.getEventTimeList();
            missList = dbDataFetcher.getOneEventOfTeamList(13, team-1);
            missTimeList = dbDataFetcher.getEventTimeList();
            postList = dbDataFetcher.getOneEventOfTeamList(14, team-1);
            postTimeList = dbDataFetcher.getEventTimeList();
            savedList = dbDataFetcher.getOneEventOfTeamList(62, team-1);
            savedTimeList = dbDataFetcher.getEventTimeList();
        }
        else {
            goalList = dbDataFetcher.getOneEventOfPlayerList(16, team-2);
            goalTimeList = dbDataFetcher.getEventTimeList();
            missList = dbDataFetcher.getOneEventOfPlayerList(13, team-2);
            missTimeList = dbDataFetcher.getEventTimeList();
            postList = dbDataFetcher.getOneEventOfPlayerList(14, team-2);
            postTimeList = dbDataFetcher.getEventTimeList();
            savedList = dbDataFetcher.getOneEventOfPlayerList(62, team-2);
            savedTimeList = dbDataFetcher.getEventTimeList();
        }
        shootView1 = new shootView(this);
        shootView1.setData(goalList,savedList,missList,postList);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        if (shootView != null) {
            shootView.addView(shootView1, layoutParams);
        }
        ShootAdapter shootAdapter = new ShootAdapter();
        List<List<Integer>> shootDataList = getShootDataList(goalList,savedList,missList,postList,goalTimeList, savedTimeList, missTimeList, postTimeList);
        final int goalNum;
        if(shootDataList.size()>0){
            goalNum = shootDataList.get(0).size();
        }
        else{
            goalNum=0;
        }
        int shootNumTemp = 0;
        for(int i=0;i<shootDataList.size();i++){
            shootNumTemp += shootDataList.get(i).size();
        }
        final int shootNum = shootNumTemp;
        detailText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent1 = new Intent(ShootActivity.this, ShootDetailActivity.class);
                intent1.putExtra("goalNum",goalNum);
                intent1.putExtra("shootNum",shootNum);
                intent1.putExtra("team",team);
                intent1.putExtra("isTeam", isTeam);
                startActivity(intent1);
            }
        });
        shootAdapter.setDataList(shootDataList);
        shootRv.setLayoutManager(new LinearLayoutManager(this));
        shootRv.setAdapter(shootAdapter);
        dbDataFetcher.closeDataBase();
    }

    List<List<Integer>> getShootDataList(List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> goalList,
                                         List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> savedList,
                                         List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> missList,
                                         List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> postList,
                                         List<Integer> goalTimeList, List<Integer> savedTimeList,
                                         List<Integer> missTimeList, List<Integer> postTimeList){
        List<List<Integer>> list = new ArrayList<>();
        int goalIt = 0; int savedIt = 0; int missIt = 0; int postIt = 0;
        int goalTime, savedTime, missTime, postTime;
        while (true){
            if(goalIt < goalTimeList.size()){
                goalTime = goalTimeList.get(goalIt);
            }
            else {
                goalTime = -1;
            }
            if(savedIt < savedTimeList.size()){
                savedTime = savedTimeList.get(savedIt);
            }
            else {
                savedTime = -1;
            }
            if(missIt < missTimeList.size()){
                missTime = missTimeList.get(missIt);
            }
            else {
                missTime = -1;
            }
            if(postIt < postTimeList.size()){
                postTime = postTimeList.get(postIt);
            }
            else {
                postTime = -1;
            }
            Log.e("fuck", "times:  "+goalTime+"  "+savedTime+"  "+missTime+"   "+postTime);
            int nextKind = -1;
            int nextTime = -1;
            if(goalTime != -1){
                nextKind = 1;
                nextTime = goalTime;
            }
            if(savedTime != -1 && (savedTime < nextTime || nextTime ==-1)){
                nextKind = 2;
                nextTime = savedTime;
            }
            if(missTime != -1 && (missTime < nextTime || nextTime ==-1)){
                nextKind = 3;
                nextTime = missTime;
            }
            if(postTime != -1 && (postTime < nextTime || nextTime ==-1)){
                nextKind = 4;
                nextTime = postTime;
            }
            if(nextTime == -1){
                break;
            }
            else{
                List<Integer> data = new ArrayList<>();
                if(nextKind == 1){
                    data.add(goalList.get(goalIt).second.first);
                    data.add(1);
                    data.add(goalTime);
                    data.add(goalIt);
                    goalIt++;
                }
                else if(nextKind == 2){
                    data.add(savedList.get(savedIt).second.first);
                    data.add(2);
                    data.add(savedTime);
                    data.add(savedIt);
                    savedIt++;
                }
                else if(nextKind == 3){
                    data.add(missList.get(missIt).second.first);
                    data.add(3);
                    data.add(missTime);
                    data.add(missIt);
                    missIt++;
                }
                else if(nextKind == 4){
                    data.add(postList.get(postIt).second.first);
                    data.add(4);
                    data.add(postTime);
                    data.add(postIt);
                    postIt++;
                }
                list.add(data);
            }
        }
        return  list;
    }

    class ShootAdapter extends RecyclerView.Adapter<ShootAdapter.MyViewHolder> {
        private List<List<Integer>> dataList;
        @Override
        public MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new MyViewHolder(LayoutInflater.from(mContext).inflate(R.layout.shoot_item_view, parent, false),
                    new shootViewHolderClicks() {
                        @Override
                        public void onFollowStatusChange(int position) {
                            if(position > 0) {
                                shootView1.setEmphasizedItem(dataList.get(position - 1).get(1), dataList.get(position - 1).get(3));
                                shootView1.invalidate();
                            }
                        }
                    });
        }

        public void setDataList(List<List<Integer>> dataList){
            this.dataList = dataList;
        }

        @Override
        public void onBindViewHolder(MyViewHolder holder, int position) {
            if(position == 0){
                holder.timeText.setText("time");
                holder.statusText.setText("status");
                holder.playerText.setText("player");
            }
            else{
                holder.timeText.setText(""+dataList.get(position-1).get(2));
                int status = dataList.get(position-1).get(1);
                String string = null;
                if(status == 1){
                    string = "goal";
                }
                else if(status == 2){
                    string = "saved";
                }
                else if(status == 4){
                    string = "post";
                }
                else {
                    string = "missed";
                }
                holder.statusText.setText(string);
                holder.playerText.setText(""+dataList.get(position-1).get(0));
            }
        }

        @Override
        public int getItemCount() {
            return dataList.size()+1;
        }

        class MyViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
            TextView playerText, statusText, timeText;
            shootViewHolderClicks mListener;

            public MyViewHolder(View view, shootViewHolderClicks listener) {
                super(view);
                playerText = (TextView) view.findViewById(R.id.player_name);
                statusText = (TextView) view.findViewById(R.id.result);
                timeText = (TextView) view.findViewById(R.id.time);
                mListener = listener;
                view.setOnClickListener(this);
            }

            @Override
            public void onClick(View v) {
                mListener.onFollowStatusChange(getLayoutPosition());
            }
        }
    }

    private interface shootViewHolderClicks {
        void onFollowStatusChange(int position);
    }
}
