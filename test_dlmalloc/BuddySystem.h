#pragma once
#include <assert.h>
#include <utility>

struct PageRangeNode
{
	PageRangeNode(int b, int e, PageRangeNode* parent)
	{
		beg = b;
		end = e;
		left = right = nullptr;
		this->parent = parent;
	}

	~PageRangeNode()
	{
		if (left)
			delete left;
		if (right)
			delete right;
	}
	int Size()
	{
		return end - beg;
	}

	bool IsLeafNode()
	{
		return left == nullptr && right == nullptr;
	}

	int beg, end;
	PageRangeNode* left;
	PageRangeNode* right;
	PageRangeNode* parent;
};

class PageRangeTree
{
public:
	const std::pair<int, int> INVALID_RANGE;
	PageRangeTree(int pageCount):
		INVALID_RANGE(std::make_pair(0, 0))
	{
		assert((pageCount & pageCount - 1) == 0);
		this->pageCount = pageCount;
		root = nullptr;
	}

	std::pair<int, int> Acquire(int size)
	{
		if (size > pageCount || size <= 0)
		{
			return INVALID_RANGE;
		}

		if (root == nullptr)
		{
			root = new PageRangeNode(0, pageCount, nullptr);
			return AcquireNewNode(root, size);
		}
		else
		{
			return Acquire(root, size);
		}

		
	}

	std::pair<int, int> Release(int beg, int size)
	{
		if (root)
		{
			return Release(beg, size, root);
		}
		else
		{
			assert(0);
			return std::make_pair(0, 0);
		}
	}

private:

	std::pair<int, int> Acquire(PageRangeNode* node, int size)
	{
		if (node->IsLeafNode())
			return INVALID_RANGE;

		if (size > node->Size() / 2)
			return INVALID_RANGE;

		std::pair<int, int> res = INVALID_RANGE;
		if (!node->left)
		{
			node->left = new PageRangeNode(node->beg, node->Size() / 2, node);
			return AcquireNewNode(node->left,size);
		}
		else
		{
			res = Acquire(node->left, size);
			if (res != INVALID_RANGE)
				return res;
		}

		if (!node->right)
		{
			node->right = new PageRangeNode(node->Size() / 2, node->end, node);
			return AcquireNewNode(node->right, size);
		}
		else
		{
			res = Acquire(node->right, size);
			if (res != INVALID_RANGE)
				return res;
		}
		return INVALID_RANGE;
	}

	std::pair<int, int> AcquireNewNode(PageRangeNode* node, int size) // must success
	{
		assert(size <= node->Size());
		assert(node->IsLeafNode());

		if (size > node->Size() / 2)
		{
			return std::make_pair(node->beg,node->end) ;
		}
		else
		{
			node->left = new PageRangeNode(node->beg, node->Size() / 2, node);
			return AcquireNewNode(node->left, size);
		}
	}

	std::pair<int, int> Release(int beg, int size, PageRangeNode* node)
	{
		if (beg == node->beg && size > node->Size() / 2)
		{
			return NotifyReleaseChildNode(node);
		}
		else
		{
			int end = beg + size;
			if (beg >= node->beg && end <= node->beg + node->Size() / 2)
			{
				assert(node->left != nullptr);
				return Release(beg, size, node->left);
			}
			else if (beg >= node->beg + node->Size() / 2 && end <= node->end)
			{
				assert(node->right != nullptr);
				return Release(beg, size, node->right);
			}
			else
			{
				assert(0 && "impossible");
				return std::make_pair(0, 0);
			}
		}
	}


	std::pair<int,int> NotifyReleaseChildNode(PageRangeNode* childNode)
	{
		if (childNode == root)
		{
			delete childNode;
			root = nullptr;
			return std::make_pair(0,pageCount);
		}

		PageRangeNode* parent = childNode->parent;
		assert(parent);
		assert(childNode == parent->left || childNode == parent->right);
		
		if (childNode == parent->left)
		{
			parent->left = nullptr;
		}
		if (childNode == parent->right)
		{
			parent->right = nullptr;
		}

		auto ret = std::make_pair(childNode->beg, childNode->end);
		if (!parent->IsLeafNode())
		{			
			delete childNode;			
		}
		else
		{
			NotifyReleaseChildNode(parent);
		}
		return ret;
	}
private:
	PageRangeNode * root;
	int pageCount;
};


