#pragma once
#include <vector>


struct PageRange
{
	PageRange(int b, int e)
	{
		beg = b;
		end = e;
	}

	PageRange() {}
	int beg;
	int end;
};

class PageRangeLiner
{
public:
	PageRangeLiner(int count);
	~PageRangeLiner();

	bool Acquire(int size, PageRange& out);
	void Release(PageRange range);

private:
	std::vector<bool> flags;
};

