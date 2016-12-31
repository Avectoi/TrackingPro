package com.example.hyxja.football;

import android.widget.TextView;

/**
 * Created by hyxja on 2016/7/17.
 */
public interface Constants {
    int PASSID = 1;
    int TACKLEID = 7;
    int CORNERID = 6;
    int INTERCEPTIONID = 8;
    int SAVEID = 10;
    String[] dataArray = {"pass","tackle", "corner","interception", "save", "turnover", "claim", "clearance", "card_yellow",
    "card_red", "goal", "shoot"};
    String[] UPPERDATAARRAY = {"PASS","TACKLE","CORNER","INTERCEPTION","SAVE", "TURNOVER", "CLAIM", "CLEARANCE", "CARD_YELLOW",
    "CARD_RED", "GOAL", "SHOOT"};
    int[] EVENT_ID_ARRAY = {1,7,6,8,10,9,11,12,17,17,16,13,14};
    String[] PLAYERPOS = {"X1","Y1","X2","Y2","X3","Y3","X4","Y4","X5","Y5","X6","Y6",
            "X7","Y7","X8","Y8","X9","Y9","X10","Y10","X11","Y11","X12","Y12","X13","Y13",
            "X14","Y14","X15","Y15","X16","Y16","X17","Y17","X18","Y18","X19","Y19","X20","Y20",
            "X21","Y21","X22","Y22",};
    int[] TEAM1PLAYERID = {1,2,3,4,5,6,11,12};
    int[] TEAM2PLAYERID = {7,8,10,13,14,15,16,17};
    int THERM_CHART = -1;
    int LEFT_MIDDLE = 1;
    int LEFT_DOWN = 2;
    int LEFT_UP = 3;
    int MIDDLE_UP = 4;
    int MIDDLE_DOWN = 5;
    int MIDDLE = 6;
    int RIGHT_UP = 7;
    int RIGHT_MIDDLE = 8;
    int RIGHT_DOWN = 9;
}
