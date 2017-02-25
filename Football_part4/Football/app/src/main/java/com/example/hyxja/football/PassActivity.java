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

import java.util.List;

public class PassActivity extends AppCompatActivity {
    private Context mContext;
    private PassView passView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pass);
        mContext = this;
        RecyclerView passRv;
        passRv = (RecyclerView)findViewById(R.id.pass_rv);
        RelativeLayout passLayout = (RelativeLayout)findViewById(R.id.pass_view);
        DbDataFetcher dbDataFetcher = new DbDataFetcher(this);
        Intent intent = getIntent();
        boolean isTeam = intent.getExtras().getBoolean("isTeam");
        int team = intent.getExtras().getInt("team");
        List dataList;
        if(isTeam) {
            dataList = dbDataFetcher.getOneEventOfTeamList(1, team - 1);
        }
        else {
            dataList = dbDataFetcher.getOneEventOfPlayerList(1, team-2);
        }
        passView = new PassView(this);
        passView.setDataList(dataList);
        Log.e("fuck","dataList size: "+dataList.size());
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        if (passLayout != null) {
            passLayout.addView(passView, layoutParams);
        }
        PassAdapter passAdapter = new PassAdapter();
        passAdapter.setDataList(dataList);
        passAdapter.setTimeList(dbDataFetcher.getEventTimeList());
        passRv.setLayoutManager(new LinearLayoutManager(this));
        passRv.setAdapter(passAdapter);
    }

    class PassAdapter extends RecyclerView.Adapter<PassAdapter.MyViewHolder> {
        private List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> dataList;
        private List<Integer> timeList;
        @Override
        public MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new MyViewHolder(LayoutInflater.from(mContext).inflate(R.layout.pass_item_view, parent, false),
                    new passViewHolderClicks() {
                        @Override
                        public void onFollowStatusChange(int position) {
                            if(position > 0) {
                                passView.setEmphasizeItem(position-1);
                                passView.invalidate();
                            }
                        }
                    });
        }

        public void setDataList(List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> dataList){
            this.dataList = dataList;
        }

        public void setTimeList(List<Integer> timeList){
            this.timeList = timeList;
        }

        @Override
        public void onBindViewHolder(MyViewHolder holder, int position) {
            if(position == 0){
                holder.timeText.setText("time");
                holder.player1Text.setText("player1");
                holder.player2Text.setText("player2");
            }
            else{
                holder.player1Text.setText(""+dataList.get(position-1).second.first);
                holder.player2Text.setText(""+dataList.get(position-1).second.second);
                holder.timeText.setText(""+timeList.get(position-1));
            }
        }

        @Override
        public int getItemCount() {
            return dataList.size()+1;
        }

        class MyViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
            TextView player1Text, player2Text, timeText;
            passViewHolderClicks mListener;

            public MyViewHolder(View view, passViewHolderClicks listener) {
                super(view);
                player1Text = (TextView) view.findViewById(R.id.player1_name);
                player2Text = (TextView)view.findViewById(R.id.player2_name);
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

    private interface passViewHolderClicks {
        void onFollowStatusChange(int position);
    }

}
