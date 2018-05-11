// test_dlmalloc.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include "DLMalloc285.h"
#include <vector>
#include "BuddySystem.h"
#include "PageRangeLiner.h"
#include <sysinfoapi.h>
#include "Psapi.h"

#include <tchar.h>

int* malloc_t(size_t count)
{
	int * p = (int*)dlmalloc(count);
	*p = 100;
	return p;
}

void stat()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	_tprintf(TEXT("There are %*I64d total MB of virtual memory.\n"),
		7, (statex.ullTotalVirtual - statex.ullAvailVirtual) / (1024 * 1024));


	// 	HANDLE handle = GetCurrentProcess(); 
	// 
	// 	PROCESS_MEMORY_COUNTERS pmc;
	// 
	// 	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
}
int main()
{
	// 	for (int j = 0; j < 25; ++j)
	std::vector<int*> res;
// 	for (int i = 0; i <= 25; ++i)
// 	{
// 		res.push_back(malloc_t(1 << i));
// 	}
	res.push_back(malloc_t(1 << 15));
	for (auto prt : res)
	{
		dlfree(prt);
	}

	

	// 	stat();
	system("pause");
	// 	PageRangeTree tree(1024);
	// 	auto a = tree.Acquire(1);
	// 	auto b = tree.Acquire(512);
	// 	auto c = tree.Release(a.first,a.second);
	// 	auto d = tree.Release(b, 512);


	// 	PageRangeLiner liner(1024);
	// 	PageRange out;
	// 	bool res = false;
	// 	res = liner.Acquire(1,out);
	// 	res = liner.Acquire(512,out);
	// 	liner.Release(PageRange(0, 1));
	// 	liner.Release(out);

	return 0;
}

