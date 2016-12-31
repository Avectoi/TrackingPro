package com.example.hyxja.football;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.List;

public class PlayerActivity extends AppCompatActivity {
    Context mContext;
    List<Integer> mData;
    int team, position, playerId;
    double playerLoad;
    RecyclerView recyclerView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);
        mContext = this;
        DbDataFetcher dbDataFetcher = new DbDataFetcher(mContext);
        Intent intent = getIntent();
        team = intent.getExtras().getInt("team");
        position = intent.getExtras().getInt("pos");
        mData = dbDataFetcher.getCountData((position-1)+11*(team-1)+2);
        MyAdapter adapter = new MyAdapter();
        if(team == 1) {
            playerId = Constants.TEAM1PLAYERID[(position - 1)];
            adapter.setTeam(Constants.TEAM1PLAYERID[(position - 1)]);
        }
        else{
            playerId = Constants.TEAM2PLAYERID[(position-1)];
            adapter.setTeam(Constants.TEAM2PLAYERID[(position-1)]);
        }
        adapter.setDataList(mData);
        PlayerLoad pl = new PlayerLoad();
        pl.setPostList(dbDataFetcher.getPlayerPosList().get(playerId));
        playerLoad = pl.getPlayerLoad();
        recyclerView = (RecyclerView)findViewById(R.id.player_rv);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        recyclerView.setAdapter(adapter);
        dbDataFetcher.closeDataBase();
    }

    class MyAdapter extends RecyclerView.Adapter<MyAdapter.MyViewHolder> {
        private List<Integer> dataList;
        private int team;
        @Override
        public MyViewHolder onCreateViewHolder(final ViewGroup parent, int viewType) {
            return new MyViewHolder(LayoutInflater.from(mContext).inflate(R.layout.team_data_view, parent, false),
                    new MyViewHolderClicks() {
                        @Override
                        public void onFollowStatusChange(int position) {
                            if (position == 0) {
                                Intent intent = new Intent(PlayerActivity.this, PassActivity.class);
                                intent.putExtra("team",team);
                                intent.putExtra("isTeam", false);
                                startActivity(intent);
                            }
                            else if(position == 12){
                                Intent intent = new Intent(PlayerActivity.this,ThermActivity.class);
                                intent.putExtra("team", team);
                                startActivity(intent);
                            }
                            else if(position == 11){
                                Intent intent = new Intent(PlayerActivity.this, ShootActivity.class);
                                intent.putExtra("team",team);
                                intent.putExtra("isTeam", false);
                                startActivity(intent);
                            }
                            else if(position == 1 || position == 3){
                                Intent intent = new Intent(PlayerActivity.this, TackleActivity.class);
                                int type;
                                if(position == 1){
                                    type = 7;
                                }
                                else {
                                    type = 8;
                                }
                                intent.putExtra("type", type);
                                intent.putExtra("team",team);
                                intent.putExtra("isTeam", false);
                                startActivity(intent);
                            }
                        }
                    });
        }

        public void setDataList(List<Integer> dataList){
            this.dataList = dataList;
        }

        public void setTeam(int team){
            this.team = team;
        }

        @Override
        public void onBindViewHolder(MyViewHolder holder, int position) {
            if(position == 12){
                holder.titleText.setText("therm chart");
                holder.dataText.setVisibility(View.INVISIBLE);
                holder.moreText.setVisibility(View.VISIBLE);
            }
            else if(position == 13){
                holder.titleText.setText("playerLoad");
                holder.dataText.setVisibility(View.VISIBLE);
                holder.dataText.setText(""+(int)playerLoad);
                holder.moreText.setVisibility(View.INVISIBLE);
            }
            else {
                holder.titleText.setText(Constants.dataArray[position]);
                holder.dataText.setText("" + dataList.get(position));
                holder.dataText.setVisibility(View.VISIBLE);
                holder.moreText.setVisibility(View.VISIBLE);
            }
        }

        @Override
        public int getItemCount() {
            return dataList.size()+1;
        }

        class MyViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
            TextView titleText, dataText, moreText;
            MyViewHolderClicks mListener;

            public MyViewHolder(View view, MyViewHolderClicks listener) {
                super(view);
                titleText = (TextView) view.findViewById(R.id.text_title);
                dataText = (TextView) view.findViewById(R.id.text_data);
                moreText = (TextView) view.findViewById(R.id.text_more);
                mListener = listener;
                moreText.setOnClickListener(this);
            }

            @Override
            public void onClick(View v) {
                mListener.onFollowStatusChange(getLayoutPosition());
            }
        }
    }

    private interface MyViewHolderClicks {
        void onFollowStatusChange(int position);
    }
}
