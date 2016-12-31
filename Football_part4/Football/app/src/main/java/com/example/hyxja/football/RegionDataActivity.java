package com.example.hyxja.football;

import android.app.DownloadManager;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Environment;
import android.support.v4.util.Pair;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import org.eclipse.egit.github.core.Team;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class RegionDataActivity extends AppCompatActivity {
    private RecyclerView team1DataRV, team2DataRV, team1RV, team2RV;
    LinearLayout overViewLl;
    private List<Integer> mData1, mData2;
    private List<List<Integer>>team1Data, team2Data;
    private TextView overviewText, team1Text, team2Text;
    private Context mContext;
    private int region;
    private DbDataFetcher dbDataFetcher;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_region_data);
        mContext = this;
        Intent intent = getIntent();
        region = intent.getExtras().getInt("region");
        findViews();
        dbDataFetcher = new DbDataFetcher(mContext);
        setOverview(dbDataFetcher);
        setClickListener();
        dbDataFetcher.closeDataBase();
    }

    private void setClickListener() {
        overviewText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                overViewLl.setVisibility(View.VISIBLE);
                team1RV.setVisibility(View.GONE);
                team2RV.setVisibility(View.GONE);
                overviewText.setTextColor(getResources().getColor(R.color.red));
                team1Text.setTextColor(getResources().getColor(R.color.black));
                team2Text.setTextColor(getResources().getColor(R.color.black));
            }
        });
        team1Text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                overViewLl.setVisibility(View.GONE);
                team1RV.setVisibility(View.VISIBLE);
                team2RV.setVisibility(View.GONE);
                overviewText.setTextColor(getResources().getColor(R.color.black));
                team1Text.setTextColor(getResources().getColor(R.color.red));
                team2Text.setTextColor(getResources().getColor(R.color.black));

            }
        });
        team2Text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                overViewLl.setVisibility(View.GONE);
                team1RV.setVisibility(View.GONE);
                team2RV.setVisibility(View.VISIBLE);
                overviewText.setTextColor(getResources().getColor(R.color.black));
                team1Text.setTextColor(getResources().getColor(R.color.black));
                team2Text.setTextColor(getResources().getColor(R.color.red));
            }
        });
    }

    private void setOverview(DbDataFetcher dbDataFetcher) {
        mData1 = dbDataFetcher.getRegionCountData(-1,region);
        mData2 = dbDataFetcher.getRegionCountData(-2,region);
        MyAdapter team1Adapter = new MyAdapter();
        MyAdapter team2Adapter = new MyAdapter();
        team1Adapter.setDataList(mData1);
        team2Adapter.setDataList(mData2);
        team1Adapter.setTeam(1);
        team2Adapter.setTeam(2);
        team1DataRV.setLayoutManager(new LinearLayoutManager(this));
        team1DataRV.setAdapter(team1Adapter);
        team2DataRV.setLayoutManager(new LinearLayoutManager(this));
        team2DataRV.setAdapter(team2Adapter);
        TeamAdapter team1ConcreteAdapter = new TeamAdapter();
        TeamAdapter team2ConcreteAdapter = new TeamAdapter();
        team1Data = getTeamData(1);
        team2Data = getTeamData(2);
        team1ConcreteAdapter.setDataList(team1Data);
        team2ConcreteAdapter.setDataList(team2Data);
        team1ConcreteAdapter.setTeam(1);
        team2ConcreteAdapter.setTeam(2);
        team1RV.setAdapter(team1ConcreteAdapter);
        team2RV.setAdapter(team2ConcreteAdapter);
        team1RV.setLayoutManager(new LinearLayoutManager(this));
        team2RV.setLayoutManager(new LinearLayoutManager(this));
    }

    private List<List<Integer>> getTeamData(int team){
        List<List<Integer>> list = new ArrayList<>();
        if(team == 1){
            for(int i=0;i<Constants.TEAM1PLAYERID.length;i++){
                list.add(dbDataFetcher.getRegionCountData(Constants.TEAM1PLAYERID[i], region));
            }
        }
        else {
            for(int i=0;i<Constants.TEAM2PLAYERID.length;i++){
                list.add(dbDataFetcher.getRegionCountData(Constants.TEAM2PLAYERID[i], region));
            }
        }
        return list;
    }
    private void findViews() {
        team1DataRV = (RecyclerView) findViewById(R.id.Team1DataRecyclerView);
        team2DataRV = (RecyclerView) findViewById(R.id.Team2DataRecyclerView);
        team1Text = (TextView)findViewById(R.id.text_team1);
        overviewText = (TextView)findViewById(R.id.text_overview);
        team2Text = (TextView)findViewById(R.id.text_team2);
        team1RV = (RecyclerView)findViewById(R.id.rc_team1);
        team2RV = (RecyclerView)findViewById(R.id.rc_team2);
        overViewLl = (LinearLayout)findViewById(R.id.ll_overview);
    }

    class MyAdapter extends RecyclerView.Adapter<MyAdapter.MyViewHolder> {
        private List<Integer> dataList;
        private int team;
        @Override
        public MyViewHolder onCreateViewHolder(final ViewGroup parent, int viewType) {
            return new MyViewHolder(LayoutInflater.from(mContext).inflate(R.layout.team_data_view, parent, false),null);
        }

        public void setDataList(List<Integer> dataList){
            this.dataList = dataList;
        }

        public void setTeam(int team){
            this.team = team;
        }

        @Override
        public void onBindViewHolder(MyViewHolder holder, int position) {
                holder.titleText.setText(Constants.dataArray[position]);
                holder.dataText.setText("" + dataList.get(position));
                holder.dataText.setVisibility(View.VISIBLE);
        }

        @Override
        public int getItemCount() {
            return dataList.size();
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

    class TeamAdapter extends RecyclerView.Adapter<TeamAdapter.TeamViewHolder> {
        private List<List<Integer>> dataList = new ArrayList<>();
        private int team;
        @Override
        public TeamViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new TeamViewHolder(LayoutInflater.from(mContext).inflate(R.layout.team_rv_view, parent, false));
        }

        public void setTeam(int team){
            this.team = team;
        }

        public void setDataList(List<List<Integer>> dataList){
            this.dataList = dataList;
        }

        @Override
        public void onBindViewHolder(TeamViewHolder holder, int position) {
            if(position<8) {
                Log.e("fuck", "position  " + position + ";     pass" + dataList.get(position).get(0));
            }
            if(position>0) {
                holder.playerText.setText("player" + position);
                holder.passText.setText(String.valueOf(dataList.get(position-1).get(0)));
                holder.tackleText.setText(String.valueOf(dataList.get(position-1).get(1)));
                holder.cornerText.setText(String.valueOf(dataList.get(position-1).get(2)));
                holder.interceptionText.setText(String.valueOf(dataList.get(position-1).get(3)));
                holder.saveText.setText(String.valueOf(dataList.get(position-1).get(4)));
                holder.turnoverText.setText(String.valueOf(dataList.get(position-1).get(5)));
                holder.claimText.setText(String.valueOf(dataList.get(position-1).get(6)));
                holder.clearanceText.setText(String.valueOf(dataList.get(position-1).get(7)));
                holder.cardYellowText.setText(String.valueOf(dataList.get(position-1).get(8)));
                holder.cardRedText.setText(String.valueOf(dataList.get(position-1).get(9)));
                holder.goalText.setText(String.valueOf(dataList.get(position-1).get(10)));
                holder.shootText.setText(String.valueOf(dataList.get(position-1).get(11)));
            }
            else if(position == 0){
                holder.playerText.setText("player");
                holder.passText.setText("pass");
                holder.tackleText.setText("tackle");
                holder.cornerText.setText("corner");
                holder.interceptionText.setText("interception");
                holder.saveText.setText("save");
                holder.turnoverText.setText("turnover");
                holder.claimText.setText("claim");
                holder.clearanceText.setText("clearance");
                holder.cardYellowText.setText("yellow card");
                holder.cardRedText.setText("red card");
                holder.goalText.setText("goal");
                holder.shootText.setText("shoot");
                holder.playerText.setBackgroundColor(getResources().getColor(R.color.white));
            }
        }

        @Override
        public int getItemCount() {
            return dataList.size()+1;
            //return 0;
        }
        class TeamViewHolder extends RecyclerView.ViewHolder  {
            TextView playerText, passText, tackleText, cornerText, interceptionText, saveText, turnoverText, claimText,
                    clearanceText, cardYellowText, cardRedText, goalText, shootText;
            LinearLayout linearLayout;

            public TeamViewHolder(View view) {
                super(view);
                playerText = (TextView)view.findViewById(R.id.team_rv_player);
                passText = (TextView)view.findViewById(R.id.team_rv_pass);
                tackleText = (TextView)view.findViewById(R.id.team_rv_tackle);
                cornerText = (TextView)view.findViewById(R.id.team_rv_corner);
                interceptionText = (TextView)view.findViewById(R.id.team_rv_interception);
                saveText = (TextView)view.findViewById(R.id.team_rv_save);
                turnoverText = (TextView)view.findViewById(R.id.team_rv_turnover);
                claimText = (TextView)view.findViewById(R.id.team_rv_claim);
                clearanceText = (TextView)view.findViewById(R.id.team_rv_clearance);
                cardYellowText = (TextView)view.findViewById(R.id.team_rv_yellow_card);
                cardRedText = (TextView)view.findViewById(R.id.team_rv_red_card);
                goalText = (TextView)view.findViewById(R.id.team_rv_goal);
                shootText = (TextView)view.findViewById(R.id.team_rv_shoot);
                linearLayout = (LinearLayout)view.findViewById(R.id.team_rv_ll);
            }
        }
    }
}
