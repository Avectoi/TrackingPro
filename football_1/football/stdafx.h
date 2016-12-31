// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
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

// TODO:  在此处引用程序需要的其他头文件
