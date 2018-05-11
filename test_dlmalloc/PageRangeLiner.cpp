#include "stdafx.h"
#include "PageRangeLiner.h"
#include <assert.h>

PageRangeLiner::PageRangeLiner(int count)
{
	flags.resize(count, false);
}

PageRangeLiner::~PageRangeLiner()
{
}

bool PageRangeLiner::Acquire(int size, PageRange& out)
{
	for (int i = 0; i < flags.size(); ++i)
	{
		int j = 0;
		for (; j < size; ++j)
		{
			if (flags[i])
				break;
		}
		if (j == size)
		{
			out.beg = i;
			out.end = i + size;
			for (int index = out.beg; index < out.end; ++index)
			{
				flags[index] = true;
			}
			return true;
		}
	}
	return false;
}

void PageRangeLiner::Release(PageRange range)
{
	assert(range.beg >= 0 && range.end <= flags.size());
	for (int i = range.beg; i < range.end; ++i)
	{
		flags[i] = false;
	}
}
