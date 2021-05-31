#pragma once

#include "TTreeTable.h"

class HashTable : public TTable
{
protected:
	int MaxSize, Step, Curr, DelPos;
	TRecord* mas;
	const int Free = -1;
	const int Del = -2;
	int HashFunc(TKey k) { return (int)(k % MaxSize); }
public:
	HashTable(int _size = 10, int _step = 3) 
	{
		mas = new TRecord[_size];
		MaxSize = _size;
		Step = _step;
		DelPos = -1;
		Curr = -1;
		Eff = 0;
		DataCount = 0;
		for (int i = 0; i < MaxSize; i++)
		{
			mas[i].key = Free;
		}
	}
	~HashTable() { delete[]mas; }
	bool Find(TKey _key) override
	{
		DelPos = -1;
		Curr = HashFunc(_key);
		for (int i = 0; i < MaxSize; i++)
		{
			Eff++;
			if (mas[Curr].key == _key)
				return true;
			else
				if (mas[Curr].key == Del && DelPos == -1)
					DelPos = Curr;
				else
					if (mas[Curr].key == Free)
						break;
			Curr = (Step + Curr) % MaxSize;
		}
		if (DelPos != -1)
			Curr = DelPos;
		return false;
	}
	bool Insert(TRecord rec) override
	{
		if (IsFull())
			throw MaxSize;
		if (Find(rec.key) == false)
		{
			mas[Curr] = rec;
			DataCount++;
			Eff++;
			return true;
		}
		else
			return false;
	}
	bool Delete(TKey key) override
	{
		if (IsEmpty())
			throw DataCount;
		if (Find(key)) {
			mas[Curr].key = Del;
			DataCount--;
			Eff++;
			return true;
		}
		return false;
	}
	void Reset() override
	{
		for (Curr = 0; Curr < MaxSize; Curr++)
		{
			Eff++;
			if (mas[Curr].key != Free && mas[Curr].key != Del)
				break;
		}
	}
	void GoNext() override
	{
		for (Curr++; Curr < MaxSize; Curr++)
		{
			Eff++;
			if (mas[Curr].key != Del && mas[Curr].key != Free)
				break;
		}
	}
	bool IsEnd() override
	{
		if (Curr == MaxSize)
			return true;
		else return false;
	}
	int GetSize() const override { return MaxSize; }
	TRecord GetCurrRec() const override { return mas[Curr]; }
	bool IsFull() const override { return MaxSize == DataCount; }
};

