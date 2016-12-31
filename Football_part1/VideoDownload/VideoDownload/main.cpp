#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <cstring>

//#include "DataType.h"
#include "HCNetSDK.h"
#include "windows.h"
#include "plaympeg4.h"
using namespace std;

typedef HWND(WINAPI *PROCGETCONSOLEWINDOW)();

int x;

const char* NumToString(int num)
{
	char* OutputString=new char[3];
	OutputString[0] = num / 10 + '0';
	OutputString[1] = num % 10 + '0';
	OutputString[2] = '\0';
	return OutputString;

}
char* GetFileName(int Camera, int Sequence)
{
	string FileName = "./camera";
	//FileName = FileName + NumToString(Camera);
	//FileName = FileName + "/camera";
	FileName = FileName + NumToString(Camera);
	FileName = FileName + "-";
	FileName = FileName + NumToString(Sequence);
	FileName = FileName + ".mp4";
	const char* ConstName = FileName.c_str();
	cout << ConstName << endl;
	int length = FileName.length();
	char* OutputName=new char[length+1];
	for (int i = 0; i < length+1; i++)OutputName[i] = ConstName[i];
	cout << "Convert Complete" << endl;
	return OutputName;
}
void main()
{
	//Initialization
	NET_DVR_Init();

	//Set connect time and reconnect time
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//Device login
	LONG UserID1,UserID2,UserID3,UserID4;
	NET_DVR_DEVICEINFO_V30 StruDeviceInfo;
	char* IPAddress = "192.168.1.64";
	WORD DVRPort = 8000;
	char* Username = "admin";
	char* Password = "A12345678";
	UserID1 = NET_DVR_Login_V30(IPAddress, DVRPort, Username, Password, &StruDeviceInfo);
	if (UserID1 < 0)
	{
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		cin >> x;
		return;
	}
	IPAddress = "192.168.1.48";
	UserID2 = NET_DVR_Login_V30(IPAddress, DVRPort, Username, Password, &StruDeviceInfo);
	if (UserID2 < 0)
	{
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		cin >> x;
		return;
	}
	IPAddress = "192.168.1.32";
	UserID3 = NET_DVR_Login_V30(IPAddress, DVRPort, Username, Password, &StruDeviceInfo);
	if (UserID3 < 0)
	{
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		cin >> x;
		return;
	}
	IPAddress = "192.168.1.16";
	UserID4 = NET_DVR_Login_V30(IPAddress, DVRPort, Username, Password, &StruDeviceInfo);
	if (UserID4 < 0)
	{
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		cin >> x;
		return;
	}
	//Enable data stream
	LONG RealPlayHandle1, RealPlayHandle2, RealPlayHandle3, RealPlayHandle4;
	//HWND hWnd = GetConsoleWindow();
	NET_DVR_PREVIEWINFO StruPlayInfo = { 0 };
	StruPlayInfo.hPlayWnd = NULL;
	StruPlayInfo.lChannel = 1;
	StruPlayInfo.dwStreamType = 0;
	StruPlayInfo.dwLinkMode = 0;
	StruPlayInfo.bBlocked = 1;

	RealPlayHandle1 = NET_DVR_RealPlay_V40(UserID1, &StruPlayInfo, NULL, NULL);
	if (RealPlayHandle1 < 0)
	{
		printf("NET_DVR_RealPlay_V40 error\n");
		NET_DVR_Logout(UserID1);
		NET_DVR_Cleanup();
		cin >> x;
		return;
	}
	RealPlayHandle2 = NET_DVR_RealPlay_V40(UserID2, &StruPlayInfo, NULL, NULL);
	if (RealPlayHandle2 < 0)
	{
		printf("NET_DVR_RealPlay_V40 error\n");
		NET_DVR_Logout(UserID2);
		NET_DVR_Cleanup();
		cin >> x;
		return;
	}
	RealPlayHandle3 = NET_DVR_RealPlay_V40(UserID3, &StruPlayInfo, NULL, NULL);
	if (RealPlayHandle3 < 0)
	{
		printf("NET_DVR_RealPlay_V40 error\n");
		NET_DVR_Logout(UserID3);
		NET_DVR_Cleanup();
		cin >> x;
		return;
	}
	RealPlayHandle4 = NET_DVR_RealPlay_V40(UserID4, &StruPlayInfo, NULL, NULL);
	if (RealPlayHandle4 < 0)
	{
		printf("NET_DVR_RealPlay_V40 error\n");
		NET_DVR_Logout(UserID4);
		NET_DVR_Cleanup();
		cin >> x;
		return;
	}
	//TODO
	int RecordLength;

	for (int i = 0; i < 100; i++)
	{
		NET_DVR_SaveRealData(RealPlayHandle1, GetFileName(1, i));
		NET_DVR_SaveRealData(RealPlayHandle2, GetFileName(2, i));
		NET_DVR_SaveRealData(RealPlayHandle3, GetFileName(3, i));
		NET_DVR_SaveRealData(RealPlayHandle4, GetFileName(4, i));
		//NET_DVR_SaveRealData(RealPlayHandle1, "./test1.mp4");
		//NET_DVR_SaveRealData(RealPlayHandle2, "./test2.mp4");
		Sleep(30000);
		NET_DVR_StopSaveRealData(RealPlayHandle1);
		NET_DVR_StopSaveRealData(RealPlayHandle2);
		NET_DVR_StopSaveRealData(RealPlayHandle3);
		NET_DVR_StopSaveRealData(RealPlayHandle4);
	}
	
	NET_DVR_StopRealPlay(RealPlayHandle1);
	NET_DVR_StopRealPlay(RealPlayHandle2);
	NET_DVR_StopRealPlay(RealPlayHandle3);
	NET_DVR_StopRealPlay(RealPlayHandle4);

	NET_DVR_Logout(UserID1);
	NET_DVR_Logout(UserID2);
	NET_DVR_Logout(UserID3);
	NET_DVR_Logout(UserID4);
	NET_DVR_Cleanup();
	printf("Process Success\n");
	//cin >> x;
	return;
}