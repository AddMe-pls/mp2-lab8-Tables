#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include "../VisualTable/HashTable.h"
#include "gtest.h"

TEST(TScanTable, can_create_scan_table)
{
	ASSERT_NO_THROW(TScanTable tab);
}

TEST(TScanTable, can_copy_scan_table)
{
	TScanTable t;
	ASSERT_NO_THROW(TScanTable t1(t));
}

TEST(TScanTable, can_find_record_in_scan_table)
{
	TRecord r;
	TScanTable t;
	t.Insert(r);
	ASSERT_NO_THROW(t.Find(r.key));
}

TEST(TScanTable, can_insert_record_to_scan_table)
{
	TRecord r;
	TScanTable t;
	ASSERT_NO_THROW(t.Insert(r));
}

TEST(TScanTable, can_delete_record_from_scan_table)
{
	TRecord r;
	TScanTable t;
	t.Insert(r);
	ASSERT_NO_THROW(t.Delete(r.key));
}

TEST(TSortTable, can_create_sort_table)
{
	ASSERT_NO_THROW(TSortTable tab);
}

TEST(TScanTable, can_copy_sort_table)
{
	TSortTable t;
	ASSERT_NO_THROW(TSortTable t1(t));
}

TEST(TScanTable, can_find_record_in_sort_table)
{
	TRecord r;
	TSortTable t;
	t.Insert(r);
	ASSERT_NO_THROW(t.Find(r.key));
}

TEST(TScanTable, can_insert_record_to_sort_table)
{
	TRecord r;
	TSortTable t;
	ASSERT_NO_THROW(t.Insert(r));
}

TEST(TScanTable, can_delete_record_from_sort_table)
{
	TRecord r;
	TSortTable t;
	t.Insert(r);
	ASSERT_NO_THROW(t.Delete(r.key));
}

TEST(TTreeTable, can_create_tree_table)
{
	ASSERT_NO_THROW(TTreeTable tab);
}

TEST(TTreeTable, can_find_record_in_tree_table)
{
	TRecord r;
	TTreeTable t;
	t.Insert(r);
	ASSERT_NO_THROW(t.Find(r.key));
}

TEST(TTreeTable, can_insert_record_to_tree_table)
{
	TRecord r;
	TTreeTable t;
	ASSERT_NO_THROW(t.Insert(r));
}

TEST(TTreeTable, can_delete_record_from_tree_table)
{
	TRecord r;
	TNode n(r);
	TTreeTable t;
	t.Insert(n);
	ASSERT_NO_THROW(t.Delete(n.key));
}

TEST(HashTable, can_create_hash_table)
{
	ASSERT_NO_THROW(HashTable tab);
}

TEST(HashTable, can_find_record_in_hash_table)
{
	TRecord r;
	HashTable t;
	t.Insert(r);
	ASSERT_NO_THROW(t.Find(r.key));
}

TEST(HashTable, can_insert_record_to_hash_table)
{
	TRecord r;
	HashTable t;
	ASSERT_NO_THROW(t.Insert(r));
}

TEST(HashTable, can_delete_record_from_hash_table)
{
	TRecord r;
	HashTable t;
	t.Insert(r);
	ASSERT_NO_THROW(t.Delete(r.key));
}

TEST(TScanTable, cant_delete_form_empty)
{
	TScanTable t;
	ASSERT_ANY_THROW(t.Delete(1));
}

TEST(TSortTable, cant_delete_form_empty)
{
	TSortTable t;
	ASSERT_ANY_THROW(t.Delete(1));
}

TEST(TTreeTable, cant_delete_form_empty)
{
	TTreeTable t;
	ASSERT_ANY_THROW(t.Delete(1));
}

TEST(HashTable, cant_delete_form_empty)
{
	HashTable t;
	ASSERT_ANY_THROW(t.Delete(1));
}

TEST(TScanTable, cant_insert_to_full)
{
	TScanTable t(0);
	ASSERT_ANY_THROW(t.Insert(1));
}

TEST(TSortTable, cant_insert_to_full)
{
	TSortTable t(0);
	ASSERT_ANY_THROW(t.Insert(1));
}

TEST(HashTable, cant_insert_to_full)
{
	HashTable t(0, 1);
	ASSERT_ANY_THROW(t.Insert(1));
}

TEST(TTable, can_get_eff)
{
	TScanTable t;
	ASSERT_NO_THROW(t.GetEff());
}

TEST(TTable, can_get_curr_rec)
{
	TScanTable t;
	ASSERT_NO_THROW(t.GetCurrRec());
}

TEST(TTable, can_get_data_count)
{
	TScanTable t;
	ASSERT_NO_THROW(t.GetDataCount());
}

TEST(TTable, can_clear_eff)
{
	TScanTable t;
	ASSERT_NO_THROW(t.ClearEff());
}
