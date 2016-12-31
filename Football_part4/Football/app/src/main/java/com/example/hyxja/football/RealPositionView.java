package com.example.hyxja.football;

import android.animation.Animator;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.content.Context;
import android.support.v4.util.Pair;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by hyxja on 2016/11/8.
 */
public class RealPositionView extends LinearLayout{
    private List<List<Pair<Float,Float>>> allPosList;
    private Context mContext;
    private int team;

    public RealPositionView(Context context) {
        super(context);
        mContext = context;
        LayoutInflater mInflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mInflater.inflate(R.layout.real_position_layout, this);
    }

    public RealPositionView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;
        LayoutInflater.from(context).inflate(R.layout.real_position_layout, this);
    }

    public RealPositionView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mContext = context;
        LayoutInflater.from(context).inflate(R.layout.real_position_layout, this);
    }

    public void setAllPosList(List<List<Pair<Float,Float>>> allPosList){
        this.allPosList = allPosList;
        matchBegin();
    }

    public void setTeam(int team){
        this.team = team;
    }

    private void matchBegin(){
        final TextView[] allPlayersView = initialPlayersView(team);
        final List<Pair<Float,Float>> realPosList = new ArrayList<>();
        for(int i=0;i<allPlayersView.length;i++){
            allPlayersView[i].setVisibility(VISIBLE);
        }
        for(int i=0;i<allPosList.size();i++){
            float realX = 0;
            float realY = 0;
            int count=0;
            for(int j=0;j<allPosList.get(i).size();j++){
                realX += allPosList.get(i).get(j).first;
                realY += allPosList.get(i).get(j).second;
                count++;
            }
            Pair<Float,Float> realPos = new Pair<>(realX/count,realY/count);
            realPosList.add(realPos);
        }
        for(int i = 0; i< allPosList.size(); i++) {
            allPlayersView[i].setText(""+(i+1));
            ObjectAnimator animatorX = ObjectAnimator.ofFloat(allPlayersView[i], "translationX",realPosList.get(i).first*1.8f);
            ObjectAnimator animatorY = ObjectAnimator.ofFloat(allPlayersView[i], "translationY",(500-realPosList.get(i).second)*1.8f);
            AnimatorSet set = new AnimatorSet();
            set.play(animatorX).with(animatorY);
            set.start();
        }

    }

    private TextView[] initialPlayersView(int team){
        TextView player1 = (TextView)findViewById(R.id.player1);
        TextView player2 = (TextView)findViewById(R.id.player2);
        TextView player3 = (TextView)findViewById(R.id.player3);
        TextView player4 = (TextView)findViewById(R.id.player4);
        TextView player5 = (TextView)findViewById(R.id.player5);
        TextView player6 = (TextView)findViewById(R.id.player6);
        TextView player7 = (TextView)findViewById(R.id.player7);
        TextView player8 = (TextView)findViewById(R.id.player8);
        TextView player9 = (TextView)findViewById(R.id.player9);
        TextView player10 = (TextView)findViewById(R.id.player10);
        TextView player11 = (TextView)findViewById(R.id.player11);
        TextView player12 = (TextView)findViewById(R.id.player12);
        TextView player13 = (TextView)findViewById(R.id.player13);
        TextView player14 = (TextView)findViewById(R.id.player14);
        TextView player15 = (TextView)findViewById(R.id.player15);
        TextView player16 = (TextView)findViewById(R.id.player16);
        TextView player17 = (TextView)findViewById(R.id.player17);
        TextView player18 = (TextView)findViewById(R.id.player18);
        TextView player19 = (TextView)findViewById(R.id.player19);
        TextView player20 = (TextView)findViewById(R.id.player20);
        TextView player21= (TextView)findViewById(R.id.player21);
        TextView player22= (TextView)findViewById(R.id.player22);
        if(team == 1) {
            return new TextView[]{player1, player2, player3, player4, player5, player6,
                    player11, player12};
        }
        else  {
            return new TextView[]{player7, player8, player10, player13, player14, player15,
                    player16, player17};
        }
    }
}
