#pragma once
#include "TArrayTable.h"
#include <stack>

constexpr int BalOK = 0;
constexpr int BalLeft = -1;
constexpr int BalRight = 1;


struct TNode : public TRecord
{
	TNode* pLeft, * pRight;
	int Balance;
	TNode(TRecord rec)
	{
		key = rec.key;
		val = rec.val;
		pLeft = pRight = NULL;
		Balance = BalOK;
	}
	TNode(TKey k = 0, TVal v = 0)
	{
		key = k;
		val = v;
		pLeft = pRight = NULL;
		Balance = BalOK;
	}
	TNode()
	{
		pLeft = pRight = NULL;
		Balance = BalOK;
	}
};

class TTreeTable : public TTable
{
protected:
	TNode* pRoot, * pCurr, * pPrev;
	std::stack<TNode*> st;
	int MaxSize, CurrPos, level;

	void DeleteTreeTable(TNode* pNode)
	{
		if (pNode)
		{
			DeleteTreeTable(pNode->pLeft);
			DeleteTreeTable(pNode->pRight);
			delete pNode;
		}
	}
	void PrintTreeTable(std::ostream& os, TNode* pNode)
	{
		if (pNode)
		{
			for (int i = 0; i < level; i++)
				os << " ";
			os << pNode->key << std::endl;
			level++;
			int	rlevel = level;
			PrintTreeTable(os, pNode->pLeft);
			level = rlevel;
			PrintTreeTable(os, pNode->pRight);
			//level = 0;
		}
	}

public:
	TTreeTable(int _size = 10) : MaxSize(_size) { pRoot = pCurr = pPrev = NULL; Eff = 0; DataCount = 0; CurrPos = 0; level = 0; }
	~TTreeTable()
	{
		/*while (!st.empty())
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
		}*/
		DeleteTreeTable(pRoot);
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
				else
					pRoot = NULL;
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
	void Print() override
	{
		std::ofstream osf("Table.txt");
		/*for (Reset(); !IsEnd(); GoNext())
		{
			PrintTreeTable(osf, pCurr);
			level = 0;
		}*/
		PrintTreeTable(osf, pRoot);
		osf.close();
	}
	void Reset() override 
	{
		TNode* pNode = pCurr = pRoot;
		while (!st.empty())
			st.pop();
		CurrPos = 0;
		while (pNode)
		{
			st.push(pNode);
			pCurr = pNode;
			pNode = pNode->pLeft;
		}
	}
	void GoNext() override 
	{
		if (!IsEnd() && pCurr)
		{
			TNode* pNode = pCurr = pCurr->pRight;
			st.pop();
			while (pNode)
			{
				st.push(pNode);
				pCurr = pNode;
				pNode = pNode->pLeft;
			}
			if (!pCurr && !st.empty())
			{
				pCurr = st.top();
			}
			CurrPos++;
		}
	}
	bool IsEnd() override { return CurrPos >= DataCount; }
};


class TBalanceTreeTable : public TTreeTable
{
protected:
	const int HeightOK = 0;
	const int HeightInc = 1;
	const int HeightDec = -1;
	int InsBalanceTree(TNode*& pNode, TKey k, TVal v)
	{
		Eff++;
		int HeightIndex = HeightOK;
		if (!pNode)
		{
			pNode = new TNode(k, v);
			HeightIndex = HeightInc;
			DataCount++;
		}
		else if (k < pNode->key)
		{
			if (InsBalanceTree(pNode->pLeft, k, v) == HeightInc)
			{
				HeightIndex = LeftTreeBalancing(pNode);
			}
		}
		else if (k > pNode->key)
		{
			if (InsBalanceTree(pNode->pRight, k, v) == HeightInc)
			{
				HeightIndex = RightTreeBalancing(pNode);
			}
		}
		else
		{
			HeightIndex = HeightOK;
		}
		return HeightIndex;
	}
	int LeftTreeBalancing(TNode*& pNode)
	{
		Eff++;
		int HeightIndex = HeightOK;
		switch (pNode->Balance) 
		{
		case BalRight: 
			pNode->Balance = BalOK;
			HeightIndex = HeightOK;
			break;
		case BalOK:
			pNode->Balance = BalLeft;
			HeightIndex = HeightInc;
			break;
		case BalLeft:
			TNode* p1, * p2;
			p1 = pNode->pLeft;
			if (p1->Balance == BalLeft)
			{
				pNode->pLeft = p1->pRight;
				p1->pRight = pNode;
				pNode->Balance = BalOK;
				pNode = p1;
			}
			else
			{
				p2 = p1->pRight;
				p1->pRight = p2->pLeft;
				p2->pLeft = p1;
				pNode->pLeft = p2->pRight;
				p2->pRight = pNode;
				if (p2->Balance == BalLeft)
					pNode->Balance = BalRight;
				else
					pNode->Balance = BalOK;
				if (p2->Balance == BalRight)
					pNode->Balance = BalLeft;
				else
					p1->Balance = BalOK;
				pNode = p2;
			}
			pNode->Balance = BalOK;
			HeightIndex = HeightOK;
		}
		return HeightIndex;
	}
	int RightTreeBalancing(TNode*& pNode)
	{
		Eff++;
		int HeightIndex = HeightOK;
		switch (pNode->Balance)
		{
		case BalLeft:
			pNode->Balance = BalOK;
			HeightIndex = HeightOK;
			break;
		case BalOK:
			pNode->Balance = BalRight;
			HeightIndex = HeightInc;
			break;
		case BalRight:
			TNode* p1, * p2;
			p1 = pNode->pRight;
			if (p1->Balance == BalRight)
			{
				pNode->pRight = p1->pLeft;
				p1->pLeft = pNode;
				pNode->Balance = BalOK;
				pNode = p1;
			}
			else
			{
				p2 = p1->pLeft;
				p1->pLeft = p2->pRight;
				p2->pRight = p1;
				pNode->pRight = p2->pLeft;
				p2->pLeft = pNode;
				if (p2->Balance == BalRight)
					pNode->Balance = BalLeft;
				else
					pNode->Balance = BalOK;
				if (p2->Balance == BalLeft)
					pNode->Balance = BalRight;
				else
					p1->Balance = BalOK;
				pNode = p2;
			}
			pNode->Balance = BalOK;
			HeightIndex = HeightOK;
		}
		return HeightIndex;
	}
	int DelBalanceTree(TNode*& pNode, TKey k)
	{
		Eff++;
		int HeightIndex = HeightOK;
		if (!pNode) 
		{
			HeightIndex = HeightOK;
		}
		else
		{
			if (k < pNode->key)
			{
				if (DelBalanceTree(pNode->pLeft, k) != HeightOK)
				{
					HeightIndex = RightTreeBalancing(pNode);
				}
			}
			else
			{
				if (k > pNode->key)
				{
					if (DelBalanceTree(pNode->pRight, k) != HeightOK)
					{
						HeightIndex = LeftTreeBalancing(pNode);
					}
				}
				else
				{
					DataCount--;
					if (pNode->pLeft == NULL && pNode->pRight == NULL)
					{
						delete pNode;
						pNode = NULL;
						HeightIndex = HeightDec;
					}
					else if (pNode->pLeft != NULL && pNode->pRight == NULL)
					{
						pNode->val = pNode->pLeft->val;
						pNode->key = pNode->pLeft->key;
						pNode->Balance = BalOK;
						HeightIndex = HeightDec;
						delete pNode->pLeft;
						pNode->pLeft = NULL;
					}
					else if (pNode->pLeft == NULL && pNode->pRight != NULL)
					{
						pNode->val = pNode->pRight->val;
						pNode->key = pNode->pRight->key;
						pNode->Balance = BalOK;
						HeightIndex = HeightDec;
						delete pNode->pRight;
						pNode->pRight = NULL;
					}
					else
					{
						TNode* l = pNode->pLeft;
						TNode* r = pNode->pRight;
						TNode* min = FindMin(r);
						HeightIndex = DelMin(r);
						pNode->val = min->val;
						pNode->key = min->key;
						delete min;
						pNode->pLeft = l;
						pNode->pRight = r;
						if (HeightIndex != HeightOK)
						{
							HeightIndex = LeftTreeBalancing(pNode);
						}
					}
				}
			}
		}
		return HeightIndex;
	}
	int DelMin(TNode*& pNode)
	{
		Eff++;
		int HeightIndex = HeightOK;
		if (!pNode->pLeft)
		{
			pNode = pNode->pRight;
			HeightIndex = HeightDec;
		}
		else
		{
			if (DelMin(pNode->pLeft) != HeightOK)
			{
				HeightIndex = RightTreeBalancing(pNode);
			}
		}
		return HeightIndex;
	}
	TNode* FindMin(TNode* pNode)
	{
		while (pNode->pLeft)
		{
			Eff++;
			pNode = pNode->pLeft;
		}
		return pNode;
	}
public:
	TBalanceTreeTable(int _size = 10) : TTreeTable(_size) {}
	bool Insert(TRecord rec) override
	{
		if (IsFull())
		{
			throw DataCount;
			return false;
		}
		InsBalanceTree(pRoot, rec.key, rec.val);
		return true;
	}
	bool Delete(TKey key) override
	{
		if (IsEmpty())
		{
			return false;
		}
		DelBalanceTree(pRoot, key);
		return true;
	}
};
