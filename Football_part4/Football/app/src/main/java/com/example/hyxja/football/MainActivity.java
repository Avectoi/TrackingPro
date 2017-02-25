package com.example.hyxja.football;

import android.app.DownloadManager;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Environment;
import android.os.Parcelable;
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

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private RecyclerView team1DataRV, team2DataRV, team1RV, team2RV;
    LinearLayout overViewLl;
    private List<Integer> mData1, mData2;
    private List<List<Integer>>team1Data, team2Data;
    private TextView overviewText, team1Text, team2Text, team1InfoText, team2InfoText;
    private Context mContext;
    private ImageView regionLeftUp, regionLeftMiddle, regionLeftDown, regionMiddleUp,
    regionMiddle, regionMiddleDown, regionRightUp, regionRightMiddle, regionRightDown;
    private float distance1, distance2;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mContext = this;
        findViews();
        MyTask myTask = new MyTask();
        myTask.execute("hi");
        setClickListener();
    }

    private void setClickListener() {
        regionLeftUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.LEFT_UP);
                startActivity(intent);
            }
        });
        regionLeftMiddle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.LEFT_MIDDLE);
                startActivity(intent);
            }
        });
        regionLeftDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.LEFT_DOWN);
                startActivity(intent);
            }
        });
        regionMiddleUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.MIDDLE_UP);
                startActivity(intent);
            }
        });
        regionMiddle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.MIDDLE);
                startActivity(intent);
            }
        });
        regionMiddleDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.MIDDLE_DOWN);
                startActivity(intent);
            }
        });
        regionRightUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.RIGHT_UP);
                startActivity(intent);
            }
        });
        regionRightMiddle.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.RIGHT_MIDDLE);
                startActivity(intent);
            }
        });
        regionRightDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, RegionDataActivity.class);
                intent.putExtra("region", Constants.RIGHT_DOWN);
                startActivity(intent);
            }
        });
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
        mData1 = dbDataFetcher.getCountData(1);
        mData2 = dbDataFetcher.getCountData(2);
        List<List<Pair<Float, Float>>> allPlayerPosList = dbDataFetcher.getPlayerPosList();
        MyAdapter team1Adapter = new MyAdapter();
        MyAdapter team2Adapter = new MyAdapter();
        team1Adapter.setDistance(distance1);
        team2Adapter.setDistance(distance2);
        team1Adapter.setDataList(mData1);
        team2Adapter.setDataList(mData2);
        team1Adapter.setTeam(1);
        team2Adapter.setTeam(2);
        team1DataRV.setLayoutManager(new LinearLayoutManager(this));
        team1DataRV.setAdapter(team1Adapter);
        team2DataRV.setLayoutManager(new LinearLayoutManager(this));
        team2DataRV.setAdapter(team2Adapter);
        MatchAnimationView matchAnimation = new MatchAnimationView(mContext);
        matchAnimation.setAllPosList(allPlayerPosList);
        RelativeLayout ll = (RelativeLayout) findViewById(R.id.match_animation);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        if (ll != null) {
            ll.addView(matchAnimation, layoutParams);
        }
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
        team1InfoText = (TextView)findViewById(R.id.team1_info_text);
        team2InfoText = (TextView)findViewById(R.id.team2_info_text);
        regionLeftUp = (ImageView)findViewById(R.id.region_left_up);
        regionLeftMiddle = (ImageView)findViewById(R.id.region_left_middle);
        regionLeftDown = (ImageView)findViewById(R.id.region_left_down);
        regionMiddleUp = (ImageView)findViewById(R.id.region_middle_up);
        regionMiddle = (ImageView)findViewById(R.id.region_middle);
        regionMiddleDown = (ImageView)findViewById(R.id.region_middle_down);
        regionRightUp = (ImageView)findViewById(R.id.region_right_up);
        regionRightMiddle = (ImageView)findViewById(R.id.region_right_middle);
        regionRightDown = (ImageView)findViewById(R.id.region_right_down);
    }

    class MyTask extends AsyncTask<String, Integer, String> {
        @Override
        protected String doInBackground(String... params) {
            String urlStr = "http://github.com/publicsjtu/football/raw/master/test.db";
            String fileName = "test.db";
            File file = new File(Environment.getExternalStorageDirectory()+"/football/test.db");
            if (file.exists()) {
                file.delete();
            }
            String serviceString = Context.DOWNLOAD_SERVICE;
            DownloadManager downloadManager;
            downloadManager = (DownloadManager)getSystemService(serviceString);

            Uri uri = Uri.parse("http://github.com/publicsjtu/football/raw/master/test.db");
            DownloadManager.Request request = new DownloadManager.Request(uri);
            request.setDestinationInExternalPublicDir("football", "test.db");
            long reference = downloadManager.enqueue(request);
            DownloadManager.Query query = new DownloadManager.Query();
            query.setFilterById(reference);
            Cursor cursor = downloadManager.query(query);
            while (true) {
                if (cursor.moveToFirst()) {
                    int status = cursor.getInt(cursor.getColumnIndex(DownloadManager.COLUMN_STATUS));
                    if (status != DownloadManager.STATUS_RUNNING) {
                        break;
                    }
                }
            }

            /*OutputStream output = null;
            try {
                URL url = new URL(urlStr);
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                String pathName = Environment.getExternalStorageDirectory().getPath() + "/"+ fileName;//文件存储路径
                File file = new File(pathName);
                InputStream input = conn.getInputStream();
                if (file.exists()) {
                    file.delete();
                }
                file.createNewFile();//新建文件
                output = new FileOutputStream(file);
                byte[] buffer = new byte[1];
                while (input.read(buffer) != -1) {
                    Log.e("fuck","hi");
                    output.write(buffer);
                }
                output.flush();
            } catch (IOException e) {
                Log.e("fuck", "catch");
                e.printStackTrace();
            } finally {
                try {
                    if(output != null)
                        output.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }*/
            return null;
        }

        protected void onPostExecute(String result) {
            DbDataFetcher dbDataFetcher = new DbDataFetcher(mContext);
            distance1 = dbDataFetcher.getRunningDistance(-1);
            distance2 = dbDataFetcher.getRunningDistance(-2);
            setOverview(dbDataFetcher);
            team1Data = new ArrayList<>();
            team2Data = new ArrayList<>();
            for (int i = 0; i < 8; i++) {
                team1Data.add(dbDataFetcher.getCountData(Constants.TEAM1PLAYERID[i] + 2));
                team2Data.add(dbDataFetcher.getCountData(Constants.TEAM2PLAYERID[i] + 2));
            }
            team1InfoText.setText("team1" + "   " + dbDataFetcher.getCountData(1).get(10));
            team2InfoText.setText("team2" + "   " + dbDataFetcher.getCountData(2).get(10));
            dbDataFetcher.closeDataBase();
            TeamAdapter teamAdapter1 = new TeamAdapter();
            TeamAdapter teamAdapter2 = new TeamAdapter();
            teamAdapter1.setDataList(team1Data);
            teamAdapter1.setTeam(1);
            teamAdapter2.setTeam(2);
            teamAdapter2.setDataList(team2Data);
            team1RV.setLayoutManager(new LinearLayoutManager(mContext));
            team2RV.setLayoutManager(new LinearLayoutManager(mContext));
            team1RV.setAdapter(teamAdapter1);
            team2RV.setAdapter(teamAdapter2);
        }
    }
    class MyAdapter extends RecyclerView.Adapter<MyAdapter.MyViewHolder> {
        private List<Integer> dataList;
        private int team;
        private float distance;
        @Override
        public MyViewHolder onCreateViewHolder(final ViewGroup parent, int viewType) {
            return new MyViewHolder(LayoutInflater.from(mContext).inflate(R.layout.team_data_view, parent, false),
                    new MyViewHolderClicks() {
                        @Override
                        public void onFollowStatusChange(int position) {
                            if (position == 0) {
                                Intent intent = new Intent(MainActivity.this, PassActivity.class);
                                intent.putExtra("team",team);
                                intent.putExtra("isTeam", true);
                                startActivity(intent);
                            }
                            else if(position == 12){
                                Intent intent = new Intent(MainActivity.this,ThermActivity.class);
                                if(team == 1) {
                                    intent.putExtra("team", -1);
                                }
                                else{
                                    intent.putExtra("team",-2);
                                }
                                startActivity(intent);
                            }
                            else if(position == 13){
                                Intent intent = new Intent(MainActivity.this,RealPositionActivity.class);
                                if(team == 1) {
                                    intent.putExtra("team", -1);
                                }
                                else{
                                    intent.putExtra("team",-2);
                                }
                                startActivity(intent);
                            }
                            else if(position == 11){
                                Intent intent = new Intent(MainActivity.this, ShootActivity.class);
                                intent.putExtra("isTeam", true);
                                intent.putExtra("team",team);
                                startActivity(intent);
                            }
                            else if(position == 1 || position == 3){
                                Intent intent = new Intent(MainActivity.this, TackleActivity.class);
                                int type;
                                if(position == 1){
                                    type = 7;
                                }
                                else {
                                    type = 8;
                                }
                                intent.putExtra("type", type);
                                intent.putExtra("isTeam", true);
                                intent.putExtra("team",team);
                                startActivity(intent);
                            }
                        }
                    });
        }

        public void setDistance(float distance){
            this.distance = distance;
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
                holder.titleText.setText("Therm chart");
                holder.dataText.setText(""+distance);
                holder.dataText.setVisibility(View.VISIBLE);
            }
            else if(position == 13){
                holder.titleText.setText("real position");
                holder.dataText.setVisibility(View.INVISIBLE);
            }
            else {
                holder.titleText.setText(Constants.dataArray[position]);
                holder.dataText.setText("" + dataList.get(position));
                holder.dataText.setVisibility(View.VISIBLE);
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

    class TeamAdapter extends RecyclerView.Adapter<TeamAdapter.TeamViewHolder> {
        private List<List<Integer>> dataList = new ArrayList<>();
        private int team;
        @Override
        public TeamViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new TeamViewHolder(LayoutInflater.from(mContext).inflate(R.layout.team_rv_view, parent, false),
                    new TeamViewHolderClicks() {
                        @Override
                        public void onFollowStatusChange(int clickPosition) {
                            if(clickPosition>0){
                                Intent intent = new Intent(MainActivity.this, PlayerActivity.class);
                                intent.putExtra("pos", clickPosition);
                                intent.putExtra("team", team);
                                startActivity(intent);
                            }
                        }
                    });
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
        class TeamViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
            TextView playerText, passText, tackleText, cornerText, interceptionText, saveText, turnoverText, claimText,
                clearanceText, cardYellowText, cardRedText, goalText, shootText;
            TeamViewHolderClicks mListener;
            LinearLayout linearLayout;

            public TeamViewHolder(View view, TeamViewHolderClicks listener) {
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
                linearLayout.setOnClickListener(this);
                mListener = listener;
            }

            @Override
            public void onClick(View v) {
                mListener.onFollowStatusChange(getLayoutPosition());
            }
        }
    }

    private interface TeamViewHolderClicks {
        void onFollowStatusChange(int position);
    }

}
