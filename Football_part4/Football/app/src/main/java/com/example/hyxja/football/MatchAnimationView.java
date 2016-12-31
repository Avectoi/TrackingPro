package com.example.hyxja.football;

import android.animation.Animator;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.animation.TypeEvaluator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.graphics.PointF;
import android.support.v4.util.Pair;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.animation.LinearInterpolator;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by hyxja on 2016/7/19.
 */
public class MatchAnimationView extends LinearLayout {
    private List<List<Pair<Float,Float>>> allPosList;
    private Context mContext;

    public MatchAnimationView(Context context) {
        super(context);
        mContext = context;
        LayoutInflater mInflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mInflater.inflate(R.layout.match_animation_layout, this);
    }

    public MatchAnimationView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;
        LayoutInflater.from(context).inflate(R.layout.match_animation_layout, this);
    }

    public MatchAnimationView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mContext = context;
        LayoutInflater.from(context).inflate(R.layout.match_animation_layout, this);
    }

    public void setAllPosList(List<List<Pair<Float,Float>>> allPosList){
        this.allPosList = allPosList;
        matchBegin();
    }

    private void matchBegin(){
        final TextView[] allPlayersView = initialPlayersView();
        final List<MyInt> myIntList = new ArrayList<>();
        for(int i=0;i<allPosList.size();i++){
            myIntList.add(new MyInt(0));
        }
        for(int i = 0; i< allPosList.size(); i++) {
            final int k = i;
            allPlayersView[i].setText(""+(i+1));
                ObjectAnimator animatorX = ObjectAnimator.ofFloat(allPlayersView[i], "translationX",allPosList.get(i).get(k).first*1.8f ,allPosList.get(i).get(k+1).first*1.8f);
                ObjectAnimator animatorY = ObjectAnimator.ofFloat(allPlayersView[i], "translationY", (500-allPosList.get(i).get(k).second)*1.8f , (500-allPosList.get(i).get(k+1).second)*1.8f);
                AnimatorSet set = new AnimatorSet();
                set.play(animatorX).with(animatorY);
                set.setDuration(40);
                set.addListener(new Animator.AnimatorListener() {
                    @Override
                    public void onAnimationStart(Animator animation) {

                    }

                    @Override
                    public void onAnimationEnd(Animator animation) {
                        setAnimSet();
                    }

                    private void setAnimSet() {
                        if(myIntList.get(k).getValue()<allPosList.get(k).size()-2) {
                            myIntList.get(k).setValue(myIntList.get(k).getValue() + 1);
                            int position = myIntList.get(k).getValue();
                            ObjectAnimator animatorX = ObjectAnimator.ofFloat(allPlayersView[k], "translationX", allPosList.get(k).get(position).first*1.8f, allPosList.get(k).get(position + 1).first*1.8f);
                            ObjectAnimator animatorY = ObjectAnimator.ofFloat(allPlayersView[k], "translationY", (500-allPosList.get(k).get(position).second)*1.8f, (500-allPosList.get(k).get(position + 1).second)*1.8f);
                            AnimatorSet set = new AnimatorSet();
                            set.play(animatorX).with(animatorY);
                            set.setDuration(40);set.addListener(new Animator.AnimatorListener() {
                                @Override
                                public void onAnimationStart(Animator animation) {

                                }

                                @Override
                                public void onAnimationEnd(Animator animation) {
                                    setAnimSet();
                                }

                                @Override
                                public void onAnimationCancel(Animator animation) {

                                }

                                @Override
                                public void onAnimationRepeat(Animator animation) {

                                }
                            });
                            set.start();
                        }
                    }

                    @Override
                    public void onAnimationCancel(Animator animation) {

                    }

                    @Override
                    public void onAnimationRepeat(Animator animation) {

                    }
                });
                set.start();
        }
    }

    private TextView[] initialPlayersView(){
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
        return new TextView[]{player1,player2,player3,player4,player5,player6,
        player7,player8,player9,player10,player11,player12,player13,player14,player15,player16,player17,
        player18,player19,player20,player21,player22};
    }


}
