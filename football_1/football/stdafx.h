// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#ifndef SQLITE3
#define SQLITE3
#include "sqlite3.h"
#endif

int showTableInfo(void *para, int n_column, char **column_value, char **column_name);

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
