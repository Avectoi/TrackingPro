package com.example.hyxja.football;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.util.Pair;
import android.support.v7.app.AppCompatActivity;
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

public class TackleActivity extends AppCompatActivity {
    TackleView tackleView;
    Context mContext;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tackle);
        RelativeLayout tackleView1 = (RelativeLayout)findViewById(R.id.tackle_rl);
        mContext = this;
        DbDataFetcher dbDataFetcher = new DbDataFetcher(this);
        RecyclerView tackleRv;
        tackleRv = (RecyclerView)findViewById(R.id.tackle_rv);
        Intent intent = getIntent();
        int team = intent.getExtras().getInt("team");
        boolean isTeam = intent.getExtras().getBoolean("isTeam");
        int type = intent.getExtras().getInt("type");
        List tackeList, tackleTimeList;
        if(isTeam){
            tackeList = dbDataFetcher.getOneEventOfTeamList(type, team-1);
            tackleTimeList = dbDataFetcher.getEventTimeList();
        }
        else {
            tackeList = dbDataFetcher.getOneEventOfPlayerList(type, team-2);
            tackleTimeList = dbDataFetcher.getEventTimeList();
        }
        tackleView = new TackleView(this);
        tackleView.setData(tackeList);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        if (tackleView1 != null) {
            tackleView1.addView(tackleView, layoutParams);
        }
        ShootAdapter shootAdapter = new ShootAdapter();
        List shootDataList = getShootDataList(tackeList, tackleTimeList);
        shootAdapter.setDataList(shootDataList);
        tackleRv.setLayoutManager(new LinearLayoutManager(this));
        tackleRv.setAdapter(shootAdapter);
        dbDataFetcher.closeDataBase();
    }

    List<Pair<Integer, Integer>> getShootDataList(List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> tackleList,
                                         List<Integer> tackleTimeList){
        List<Pair<Integer, Integer>> list = new ArrayList<>();
        for(int i=0;i<tackleTimeList.size();i++){
            Integer player = tackleList.get(i).second.first;
            Integer time = tackleTimeList.get(i);
            Pair<Integer, Integer> data = new Pair<>(player, time);
            list.add(data);
        }
        return  list;
    }

    class ShootAdapter extends RecyclerView.Adapter<ShootAdapter.MyViewHolder> {
        private List<Pair<Integer, Integer>> dataList;
        @Override
        public MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new MyViewHolder(LayoutInflater.from(mContext).inflate(R.layout.tackle_item_view, parent, false),
                    new shootViewHolderClicks() {
                        @Override
                        public void onFollowStatusChange(int position) {
                            if(position > 0) {
                                tackleView.setEmphasizedItem(position-1);
                                tackleView.invalidate();
                            }
                        }
                    });
        }

        public void setDataList(List<Pair<Integer, Integer>> dataList){
            this.dataList = dataList;
        }

        @Override
        public void onBindViewHolder(MyViewHolder holder, int position) {
            if(position == 0){
                holder.timeText.setText("time");
                holder.playerText.setText("player");
            }
            else {
                holder.timeText.setText(""+dataList.get(position-1).second);
                holder.playerText.setText(""+dataList.get(position-1).first);
            }
        }

        @Override
        public int getItemCount() {
            return dataList.size()+1;
        }

        class MyViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
            TextView playerText, timeText;
            shootViewHolderClicks mListener;

            public MyViewHolder(View view, shootViewHolderClicks listener) {
                super(view);
                playerText = (TextView) view.findViewById(R.id.player_name);
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
