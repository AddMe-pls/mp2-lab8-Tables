#pragma once
#include "TTable.h"


class TArrayTable : public TTable
{
protected:
	TRecord* pRec;
	int size, Curr;
public:
	TArrayTable(int _size = 10) : TTable()
	{
		pRec = new TRecord[_size];
		size = _size;
		Curr = -1;
		Eff = 0;
	}
	~TArrayTable()
	{
		delete[]pRec;
		size = 0;
		Curr = -1;
	}
	TArrayTable(const TArrayTable& table)
	{	
		if (size != table.size)
		{
			delete[] pRec;
			size = table.size;
			pRec = new TRecord[size];
		}
		Curr = -1;
		DataCount = table.DataCount;
		Eff = 0;
		std::copy(table.pRec, table.pRec + size, pRec);
	}
	bool IsFull() const
	{
		if (DataCount == size)
			return true;
		else return false;
	}
	int GetSize() const { return size; }
	int GetDataCount() const { return DataCount; }
	TRecord GetCurrRec() const { return pRec[Curr]; }
	void Reset() { Curr = 0; }
	void GoNext() { Curr++; }
	bool IsEnd() { return bool(Curr == DataCount); }
};

class TScanTable : public TArrayTable
{
public:
	TScanTable(int _size = 10) : TArrayTable(_size){}
	bool Find(TKey key)
	{
		bool flag = false;
		int i;
		for (i = 0; i<DataCount; i++)
			if (pRec[i].key == key)
			{
				flag = true;
				Curr = i;
				break;
			}
		Eff += i;
		if (!flag)
			Curr = DataCount;
		return flag;
	}
	bool Delete(TKey key)
	{
		if (IsEmpty())
			throw DataCount;
		if (Find(key))
		{
			pRec[Curr] = pRec[DataCount - 1];
		DataCount--;
		Eff++;
		return true;
		}
		return false;
	}
	bool Insert(TRecord rec)
	{
		if (IsFull())
			throw size;
		if (!Find(rec.key))
		{
			pRec[Curr] = rec;
			DataCount++;
			Eff++;
			return true;
		}
		else return false;
	}
};

class TSortTable : public TScanTable
{
public:
	TSortTable(int _size = 10) : TScanTable(_size) {}
	bool Find(TKey key)
	{
		int first = 0, last = DataCount - 1, mid;
		bool flag = false;
		while (first <= last)
		{
			Eff++;
			mid = (first + last) / 2;
			if (pRec[mid].key == key)
			{
				Curr = mid;
				flag = true;
				break;
			}
			else
				if (pRec[mid].key > key)
					last = mid - 1;
				else first = ++mid;
		}
		if (flag == false)
			Curr = first;
		return flag;
	}
	bool Insert(TRecord rec)
	{
		if (IsFull())
			throw size;
		if (Find(rec.key))
			return false;
		else
		{
			for (int i = DataCount - 1; i >= Curr; i--)
			{
				pRec[i + 1] = pRec[i];
				Eff++;
			}
			pRec[Curr] = rec;
			DataCount++;
			return true;
		}
	}
	bool Delete(TKey key)
	{
		if (IsEmpty())
			throw DataCount;
		if (Find(key))
		{
			Eff++;
			DataCount--;
			for (int i = Curr; i < DataCount; i++)
			{
				Eff++;
				pRec[i] = pRec[i + 1];
			}
			return true;
		}		
		return false;
	}
	void sort(int left, int right)
	{
		int l = left, r = right;
		TRecord mid = pRec[(l + r) / 2];
		while (l <= r)
		{
			Eff++;
			while (pRec[l].key < mid.key)
			{
				Eff++; l++;
			}
			while (pRec[l].key > mid.key)
			{
				Eff++;
				r--;
			}
			if (l <= r)
			{
				std::swap(pRec[l++], pRec[r--]);
			}
		}
		if (left < r)
			sort(left, r);
		else if (right > l)
			sort(l, right);
	}
	TSortTable& operator = (const TScanTable& st)
	{
		int StSize = st.GetSize();
		if (size != StSize)
		{
			delete[]pRec;
			pRec = new TRecord[StSize];
			size = StSize;
		}
		DataCount = st.GetDataCount();
		for (int i = 0; i < DataCount; i++)
			pRec[Curr] = st.GetCurrRec();
		sort(0, DataCount-1);
		return *this;
	}
};
