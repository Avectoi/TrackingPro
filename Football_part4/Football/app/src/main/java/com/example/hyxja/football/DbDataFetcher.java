package com.example.hyxja.football;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Environment;
import android.support.v4.util.Pair;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by hyxja on 2016/7/17.
 */
public class DbDataFetcher {
    private SQLiteDatabase mDb;
    private Context mContext;
    List<Integer> eventTimeList;

    DbDataFetcher(Context context){
        mContext = context;
        String dbPath = Environment.getExternalStorageDirectory().getPath()+"/test.db";
        mDb = SQLiteDatabase.openDatabase(dbPath,null,SQLiteDatabase.OPEN_READONLY);
    }

    //id=-1 for team1, id =-2 for team2, otherwise, id = playerId
    float getRunningDistance(int id){
        List<List<Pair<Float,Float>>> list = getPlayerPosList();
        float distance = 0;
        for(int i=0;i<list.size();i++){
            if(id == -1) {
                if(i==0||i==1||i==2||i==3||i==4||i==5||i==10||i==11) {
                    for (int j = 1; j < list.get(i).size(); j++) {
                        distance += Math.sqrt((list.get(i).get(j).first-list.get(i).get(j-1).first)*(list.get(i).get(j).first-list.get(i).get(j-1).first)*(105/800)*(105/800)
                                +(list.get(i).get(j).second-list.get(i).get(j-1).second)*(list.get(i).get(j).second-list.get(i).get(j-1).second)*70/400*70/400);
                    }
                }
            }
            else if(id == -2){
                if(i==6||i==7||i==9||i==12||i==13||i==14||i==15||i==16) {
                    for (int j = 1; j < list.get(i).size(); j++) {
                        distance += Math.sqrt((list.get(i).get(j).first-list.get(i).get(j-1).first)*(list.get(i).get(j).first-list.get(i).get(j-1).first)*(105/800)*(105/800)
                                +(list.get(i).get(j).second-list.get(i).get(j-1).second)*(list.get(i).get(j).second-list.get(i).get(j-1).second)*70/400*70/400);
                    }
                }
            }
            else{
                if(i == id) {
                    for (int j = 1; j < list.get(i).size(); j++) {
                        distance += Math.sqrt((list.get(i).get(j).first-list.get(i).get(j-1).first)*(list.get(i).get(j).first-list.get(i).get(j-1).first)*(105/800)*(105/800)
                                +(list.get(i).get(j).second-list.get(i).get(j-1).second)*(list.get(i).get(j).second-list.get(i).get(j-1).second)*70/400*70/400);
                    }
                }
            }
        }
        return distance;
    }

    List<Integer> getCountData(int rowId){
        List<Integer> dataList = new ArrayList<>();
        Cursor cursor;
        if(rowId == 2) {
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=2", null, null, null, null);
        }
        else if(rowId == 1){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=1", null, null, null, null);
        }
        else if(rowId == 3){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=3", null, null, null, null);
        }
        else if(rowId == 4){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=4", null, null, null, null);
        }
        else if(rowId == 5){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=5", null, null, null, null);
        }
        else if(rowId == 6){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=6", null, null, null, null);
        }
        else if(rowId == 7){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=7", null, null, null, null);
        }
        else if(rowId == 8){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=8", null, null, null, null);
        }
        else if(rowId == 9){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=9", null, null, null, null);
        }
        else if(rowId == 10){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=10", null, null, null, null);
        }
        else if(rowId == 11){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=11", null, null, null, null);
        }
        else if(rowId == 12){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=12", null, null, null, null);
        }
        else if(rowId == 13){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=13", null, null, null, null);
        }
        else if(rowId == 14){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=14", null, null, null, null);
        }
        else if(rowId == 15){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=15", null, null, null, null);
        }
        else if(rowId == 16){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=16", null, null, null, null);
        }
        else if(rowId == 17){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=17", null, null, null, null);
        }
        else if(rowId == 18){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=18", null, null, null, null);
        }
        else if(rowId == 19){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=19", null, null, null, null);
        }
        else if(rowId == 20){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=20", null, null, null, null);
        }
        else if(rowId == 21){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=21", null, null, null, null);
        }
        else if(rowId == 22){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=22", null, null, null, null);
        }
        else if(rowId == 23){
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=23", null, null, null, null);
        }
        else{
            cursor = mDb.query("Count",
                    Constants.UPPERDATAARRAY,
                    "id=24", null, null, null, null);
        }
        for(int i=0;i<Constants.UPPERDATAARRAY.length;i++){
            try {
                dataList.add(cursor.getInt(cursor.getColumnIndex(Constants.UPPERDATAARRAY[i])));
            }
            catch (Exception e){
                dataList.add(0);
            }
        }
        cursor.moveToFirst();
        cursor.close();
        dataList.add(Constants.THERM_CHART);
        return dataList;
    }

    //rowid is -1,-2for team1,team2. positive number for player
    List<Integer> getRegionCountData(int rowId, int region){
        List<Integer> dataList = new ArrayList<>();
        List<List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>>> tempList = new ArrayList<>();
        if(rowId == -1){
            for(int i=0;i<Constants.EVENT_ID_ARRAY.length;i++){
                tempList.add(getOneEventOfTeamList(Constants.EVENT_ID_ARRAY[i],0));
            }
        }
        else if(rowId == -2){
            for(int i=0;i<Constants.EVENT_ID_ARRAY.length;i++){
                tempList.add(getOneEventOfTeamList(Constants.EVENT_ID_ARRAY[i],1));
            }
        }
        else {
            for(int i=0;i<Constants.EVENT_ID_ARRAY.length;i++){
                tempList.add(getOneEventOfPlayerList(Constants.EVENT_ID_ARRAY[i],rowId));
            }
        }
        for(int i=0;i<tempList.size();i++){
            int itemNum = 0;
            /*if(i==0){
                for(int j=0;i<tempList.get(i).size();j++){
                    Log.e("fuck","x,y: "+tempList.get(i).get(j).first.first.first+" "+tempList.get(i).get(j).first.first.second);
                }
            }*/
            if(region == Constants.LEFT_UP){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first<228 && tempList.get(i).get(j).first.first.second<87){
                        itemNum++;
                    }
                }
            }
            else if(region == Constants.LEFT_MIDDLE){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first<228
                            && tempList.get(i).get(j).first.first.second>=87 && tempList.get(i).get(j).first.first.second<=313){
                        itemNum++;
                    }
                }
            }
            else if(region == Constants.LEFT_DOWN){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first<228
                            && tempList.get(i).get(j).first.first.second>313){
                        itemNum++;
                    }
                }
            }
            else if(region == Constants.MIDDLE_UP){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first>=228 && tempList.get(i).get(j).first.first.first<=572
                            && tempList.get(i).get(j).first.first.second<87){
                        itemNum++;
                    }
                }
            }
            else if(region == Constants.MIDDLE){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first>=228 && tempList.get(i).get(j).first.first.first<=572
                            && tempList.get(i).get(j).first.first.second>=87 && tempList.get(i).get(j).first.first.second<=313){
                        itemNum++;
                    }
                }
            }
            else if(region == Constants.MIDDLE_DOWN){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first>=228 && tempList.get(i).get(j).first.first.first<=572
                            && tempList.get(i).get(j).first.first.second>313){
                        itemNum++;
                    }
                }
            }
            else if(region == Constants.RIGHT_UP){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first>572
                            && tempList.get(i).get(j).first.first.second<87 ){
                        itemNum++;
                    }
                }
            }
            else if(region == Constants.RIGHT_MIDDLE){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first>572
                            && tempList.get(i).get(j).first.first.second>=87 && tempList.get(i).get(j).first.first.second<=313){
                        itemNum++;
                    }
                }
            }
            else if(region == Constants.RIGHT_DOWN){
                for(int j=0;j<tempList.get(i).size();j++){
                    if(tempList.get(i).get(j).first.first.first>572
                            && tempList.get(i).get(j).first.first.second>313){
                        itemNum++;
                    }
                }
            }
            dataList.add(itemNum);
        }
        int shootNum = dataList.get(dataList.size()-1)+dataList.get(dataList.size()-2)+dataList.get(dataList.size()-3);
        dataList.remove(dataList.size()-1);
        dataList.remove(dataList.size()-1);
        dataList.add(shootNum);
        for(int i=0;i<dataList.size();i++){
            Log.e("fuck","datalist num: "+dataList.get(i));
        }
        return dataList;
    }

    List<List<Pair<Float,Float>>> getPlayerPosList(){
        List<List<Pair<Float,Float>>> allPlayersPosList = new ArrayList<>();
        for(int i=0;i<22;i++){
            allPlayersPosList.add(new ArrayList<Pair<Float, Float>>());
        }
        Cursor cursor = mDb.query("Position", null,
               null, null, null, null, null);
        cursor.moveToFirst();
        for(int i=0;i<cursor.getCount();i++){
            for(int j=0;j<22;j++) {
                Float x,y;
                try {
                    x = cursor.getFloat(2 * j+1);
                    y = cursor.getFloat(2 * j +2);
                    if(x > 800){
                        x = 800f;
                    }
                    if(y > 500){
                        y = 500f;
                    }
                }
                catch (Exception e){
                    x = 0f;
                    y = 0f;
                }
                Pair<Float, Float> pos = new Pair<>(x, y);
                allPlayersPosList.get(j).add(pos);
            }
            cursor.moveToNext();
        }
        cursor.close();
        return allPlayersPosList;
    }

    boolean isBelongTeam(int team, int id){
        if(team == 0){
            for(int i=0;i<8;i++){
                if(id == Constants.TEAM1PLAYERID[i]){
                    return true;
                }
            }
        }
        else if(team == 1){
            for(int i=0;i<8;i++){
                if(id == Constants.TEAM2PLAYERID[i]){
                    return true;
                }
            }
        }
        return false;
    }

    List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> getOneEventOfTeamList(int eventId, int team){
        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> dataList = new ArrayList<>();
        eventTimeList = new ArrayList<>();
        eventTimeList.clear();
        Cursor cursor = mDb.query("Event",
                null, null, null, null,null,null);
        cursor.moveToFirst();
        for(int i=0;i<cursor.getCount();i++){
            Pair<Float, Float> startPos;
            Pair<Float, Float> endPos;
            Pair<Pair<Float,Float>,Pair<Float,Float>> posData;
            Pair<Integer,Integer> nameData;
            Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>> data;
            int id = cursor.getInt(cursor.getColumnIndex("TYPE"));
            int player = cursor.getInt(cursor.getColumnIndex("PLAYER1"));
            Log.e("fuck","isteam: "+team+"  "+player+"  "+isBelongTeam(team,player));
            if(id == eventId && isBelongTeam(team,player)){
                float startX = cursor.getFloat(cursor.getColumnIndex("X1"));
                float startY = cursor.getFloat(cursor.getColumnIndex("Y1"));
                float endX = cursor.getFloat(cursor.getColumnIndex("X2"));
                float endY = cursor.getFloat(cursor.getColumnIndex("Y2"));
                startPos = new Pair<>(startX, startY);
                endPos = new Pair<>(endX, endY);
                posData = new Pair<>(startPos, endPos);
                Integer player1 = cursor.getInt(cursor.getColumnIndex("PLAYER1"));
                Integer player2 = cursor.getInt(cursor.getColumnIndex("PLAYER2"));
                nameData = new Pair<>(player1, player2);
                eventTimeList.add(cursor.getInt(cursor.getColumnIndex("FRAME")));
                data = new Pair<>(posData, nameData);
                dataList.add(data);
            }
            cursor.moveToNext();
        }
        cursor.close();
        return dataList;
    }

    List<Integer> getEventTimeList(){
        return eventTimeList;
    }

    List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> getOneEventOfPlayerList(int eventId, int playerId){
        eventTimeList = new ArrayList<>();
        eventTimeList.clear();
        List<Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>>> dataList = new ArrayList<>();
        Cursor cursor = mDb.query("Event",
                null, null, null, null,null,null);
        cursor.moveToFirst();
        for(int i=0;i<cursor.getCount();i++){
            Pair<Float, Float> startPos;
            Pair<Float, Float> endPos;
            Pair<Pair<Float,Float>,Pair<Float,Float>> posData;
            Pair<Integer, Integer> playerData;
            Pair<Pair<Pair<Float,Float>,Pair<Float,Float>>, Pair<Integer,Integer>> data;
            int id = cursor.getInt(cursor.getColumnIndex("TYPE"));
            int player = cursor.getInt(cursor.getColumnIndex("PLAYER1"));
            if(id == eventId && player == playerId){
                float startX = cursor.getFloat(cursor.getColumnIndex("X1"));
                float startY = cursor.getFloat(cursor.getColumnIndex("Y1"));
                float endX = cursor.getFloat(cursor.getColumnIndex("X2"));
                float endY = cursor.getFloat(cursor.getColumnIndex("Y2"));
                startPos = new Pair<>(startX, startY);
                endPos = new Pair<>(endX, endY);
                posData = new Pair<>(startPos, endPos);
                playerData = new Pair<>(cursor.getInt(cursor.getColumnIndex("PLAYER1")), cursor.getInt(cursor.getColumnIndex("PLAYER1")));
                eventTimeList.add(cursor.getInt(cursor.getColumnIndex("FRAME")));
                data = new Pair<>(posData, playerData);
                dataList.add(data);
            }
            cursor.moveToNext();
        }
        cursor.close();
        return dataList;
    }

    void closeDataBase(){
        mDb.close();
    }
}
