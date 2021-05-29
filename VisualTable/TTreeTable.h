#pragma once
#include "TArrayTable.h"
#include <stack>

struct TNode : public TRecord
{
	TNode* pLeft, * pRight;
	TNode(TRecord rec)
	{
		key = rec.key;
		val = rec.val;
		pLeft = pRight = NULL;
	}
};

class TTreeTable : public TTable
{
protected:
	TNode* pRoot, * pCurr, * pPrev;
	std::stack<TNode*> st;
	int MaxSize;
public:
	TTreeTable(int _size = 10) : MaxSize(_size) { pRoot = pCurr = pPrev = NULL; Eff = 0; DataCount = 0; }
	~TTreeTable()
	{
		while (!st.empty())
			st.pop();
		pCurr = pRoot;
		while (!st.empty())
		{
			pCurr = st.top();
			st.pop();
			if (pCurr->pLeft)
				st.push(pCurr->pLeft);
			else if (pCurr->pRight)
				st.push(pCurr->pRight);
			delete pCurr;
		}
	}
	bool IsFull() const override { return false; }
	bool Find(TKey key) override
	{
		pCurr = pRoot; pPrev = NULL;
		while (pCurr)
		{
			Eff++;
			if (pCurr->key == key) break;
			else
			{
				pPrev = pCurr;
				if (pCurr->key > key)
					pCurr = pCurr->pLeft;
				else
					pCurr = pCurr->pRight;
			}
		}
		if (pCurr)
			return true;
		else
		{
			pCurr = pPrev;
			return false;
		}
	}
	bool Insert(const TRecord rec) override
	{
		if (IsFull())
			throw MaxSize;
		if (Find(rec.key))
			return false;
		Eff++; DataCount++;
		TNode* tmp = new TNode(rec);
		if (!pRoot) pRoot = tmp;
		else
			if (pCurr->key > rec.key)
				pCurr->pLeft = tmp;
			else
				pCurr->pRight = tmp;
		return true;
	}
	bool Delete(TKey key) override
	{
		if (IsEmpty())
			throw DataCount;
		if (Find(key))
		{
			Eff++;
			if (!pCurr->pLeft && !pCurr->pRight)
			{
				if (pPrev) {
					if (pPrev->pLeft == pCurr)
						pPrev->pLeft = NULL;
					else
						pPrev->pRight = NULL;
				}
				delete pCurr;
			}
			else if (pCurr->pLeft && !pCurr->pRight)
			{
				if (pPrev->pLeft == pCurr)
					pPrev->pLeft = pCurr->pLeft;
				else
					pPrev->pRight = pCurr->pLeft;
				delete pCurr;
			}
			else if (pCurr->pRight && !pCurr->pLeft)
			{
				if (pPrev->pRight == pCurr)
					pPrev->pRight = pCurr->pRight;
				else
					pPrev->pLeft = pCurr->pRight;
				delete pCurr;
			}
			else if (pCurr->pLeft && pCurr->pRight)
			{
				TNode* tmp = pCurr->pLeft;
				pPrev = pCurr;
				while (tmp->pRight)
				{
					pPrev = tmp;
					tmp = tmp->pRight;
				}
				pCurr->val = tmp->val;
				pCurr->key = tmp->key;
				if (pPrev->pLeft == tmp)
					pPrev->pLeft = tmp->pLeft;
				else
					pPrev->pRight = tmp->pLeft;
				delete tmp;
			}
			DataCount--;
			return true;
		}
		return false;
	}
	int GetSize() const override { return MaxSize; }
	TRecord GetCurrRec() const override { return static_cast<TRecord>(*pCurr); }
	void Reset() override {}
	void GoNext() override {}
	bool IsEnd() override { return false; }
};

