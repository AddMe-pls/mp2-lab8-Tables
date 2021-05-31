#pragma once
#include <iostream>
#include <fstream>
#include<stdio.h>

typedef	int TKey;
typedef int TVal;

struct TRecord
{
	TKey key;
	TVal val;
	
	TRecord(TKey _key = 0, TVal _val = 0) : key(_key), val(_val) {}
	//TKey GetKey() { return key; }
};

class TTable
{
protected:
	int DataCount, Eff;
public:
	virtual ~TTable() = default;
	bool IsEmpty() { if (DataCount == 0) return true; else return false; }
	virtual bool Find(TKey key) = 0; 
	virtual bool IsFull() const = 0; 
	virtual bool Insert(TRecord rec) = 0; 
	virtual bool Delete(TKey key) = 0; 
	virtual void Reset() = 0; 
	virtual void GoNext() = 0; 
	virtual bool IsEnd() = 0; 
	virtual int GetSize() const = 0;
	virtual TRecord GetCurrRec() const = 0;
	virtual int GetDataCount() const { return DataCount; }
	int GetEff() const { return Eff; }
	void ClearEff() { Eff = 0; }
	virtual void Print()
	{
		std::ofstream osf("Table.txt");
		for (Reset(); !IsEnd(); GoNext())
		{
			osf << GetCurrRec().key << std::endl;
		}
		osf.close();
	}
};

