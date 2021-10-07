#include "gtest/gtest.h"
#include "../table.h"

TEST(ItemConstructor, DefaultConstructor)
{
    Item i;
    ASSERT_EQ(0, i.busy);

    Item arr[3];
    ASSERT_EQ(0, arr[0].busy);
    ASSERT_EQ(0, arr[1].busy);
    ASSERT_EQ(0, arr[2].busy);
}

TEST(ItemConstructor, InitConstructor)
{
    char info1[N_CHAR] = "lalal";
    Item i1(-10, info1);
    ASSERT_EQ(1, i1.busy);
    ASSERT_EQ(-10, i1.key);
    for (int i = 0; i < N_CHAR - 1; i++) {
        EXPECT_EQ(i1.info[i], info1[i]);
    }

    char info2[N_CHAR] = "space";
    Item i2(100, info2);
    ASSERT_EQ(1, i2.busy);
    ASSERT_EQ(100, i2.key);
    for (int i = 0; i < N_CHAR - 1; i++) {
        EXPECT_EQ(i2.info[i], info2[i]);
    }
}

TEST(TableConstructor, DefaultConstructor)
{
    Table t;
    ASSERT_EQ(t.get_size(), N_ITEMS);
    ASSERT_EQ(t.get_info_len(), N_CHAR - 1);
    ASSERT_EQ(t.get_n(), 0);
    Item items[N_ITEMS];
    t.get_table(items, N_ITEMS);
    for(int i = 0; i < N_ITEMS; i++) {
        ASSERT_EQ(0, items[i].busy);
    }

	ASSERT_ANY_THROW(t.get_table(items, -1));
	ASSERT_ANY_THROW(t.get_table(items, N_ITEMS - 1));

    Table arr[2];

    ASSERT_EQ(arr[0].get_size(), N_ITEMS);
    ASSERT_EQ(arr[0].get_info_len(), N_CHAR - 1);
    ASSERT_EQ(arr[0].get_n(), 0);
    arr[0].get_table(items, N_ITEMS);
    for(int i = 0; i < N_ITEMS; i++) {
        ASSERT_EQ(0, items[i].busy);
    }

    ASSERT_EQ(arr[1].get_size(), N_ITEMS);
    ASSERT_EQ(arr[1].get_info_len(), N_CHAR - 1);
    ASSERT_EQ(arr[1].get_n(), 0);
    arr[1].get_table(items, N_ITEMS);
    for(int i = 0; i < N_ITEMS; i++) {
        ASSERT_EQ(0, items[i].busy);
    }
}

TEST(TableConstructor, InitConstructor)
{
    std::pair<int, char[N_CHAR]> arr_info_key[2];
    char info1[N_CHAR], info2[N_CHAR];
    arr_info_key[0].first = 27;
    memcpy(info1, "abcde", N_CHAR);
    memcpy(arr_info_key[0].second, info1, N_CHAR);
    arr_info_key[1].first = 5;
    memcpy(info2, "vrvrv", N_CHAR);
    memcpy(arr_info_key[1].second, info2, N_CHAR);

    Table t(arr_info_key, 2);

    ASSERT_EQ(t.get_size(), N_ITEMS);
    ASSERT_EQ(t.get_info_len(), N_CHAR - 1);
    ASSERT_EQ(t.get_n(), 2);
    Item items[N_ITEMS];
    t.get_table(items, N_ITEMS);
    ASSERT_EQ(1, items[0].busy);
    ASSERT_EQ(27, items[0].key);
    for (int i = 0; i < N_CHAR - 1; i++) {
        EXPECT_EQ(items[0].info[i], info1[i]);
    }
    ASSERT_EQ(1, items[1].busy);
    ASSERT_EQ(5, items[1].key);
    for (int i = 0; i < N_CHAR - 1; i++) {
        EXPECT_EQ(items[1].info[i], info2[i]);
    }

    ASSERT_ANY_THROW(Table(arr_info_key, 20));
}

TEST(Methods, Add)
{
    Table t;
    Item table[N_ITEMS];
    char info[N_CHAR];

    memcpy(info, "aaaaa", N_CHAR);
    Item i1(-10, info);
    t.add(i1);
    t.get_table(table, N_ITEMS);
    ASSERT_EQ(table[0].key, -10);

    memcpy(info, "aaabb", N_CHAR);
    Item i2(0, info);
    t.add(i2);
    t.get_table(table, N_ITEMS);
    ASSERT_EQ(table[1].key, 0);

    memcpy(info, "ababb", N_CHAR);
    Item i3(3, info);
    t.add(i3);
    t.get_table(table, N_ITEMS);
    ASSERT_EQ(table[2].key, 3);

    memcpy(info, "vvvvv", N_CHAR);
    Item i4(-19, info);
    t.add(i4);
    t.get_table(table, N_ITEMS);
    ASSERT_EQ(table[3].key, -19);

    memcpy(info, "bbbvv", N_CHAR);
    Item i5(3, info);
    ASSERT_ANY_THROW(t.add(i5));

    memcpy(info, "abbvv", N_CHAR);
    Item i6(100, info);
    t.add(i6);
    t.get_table(table, N_ITEMS);
    ASSERT_EQ(table[4].key, 100);

    memcpy(info, "abbvv", N_CHAR);
    Item i7(11, info);
    ASSERT_ANY_THROW(t.add(i7));
}

TEST(Methods, SearchItem)
{
    Table t;
    char info[N_CHAR];

    memcpy(info, "aaaaa", N_CHAR);
    Item i1(-10, info);
    t.add(i1);
    memcpy(info, "aaabb", N_CHAR);
    Item i2(0, info);
    t.add(i2);
    memcpy(info, "ababb", N_CHAR);
    Item i3(3, info);
    t.add(i3);

    ASSERT_EQ(t.search_item(i1, -10), SUCCESS);
    ASSERT_EQ(t.search_item(i1, 0), SUCCESS);
    ASSERT_EQ(t.search_item(i1, 3), SUCCESS);

    ASSERT_EQ(t.search_item(i1, 1), FAIL);
    ASSERT_EQ(t.search_item(i1, 2), FAIL);
    ASSERT_EQ(t.search_item(i1, -1), FAIL);
}

TEST(Methods, SearchInfo)
{
    Table t;
    Item table[N_ITEMS];
    char info[N_CHAR];

    memcpy(info, "aaaaa", N_CHAR);
    Item i1(-10, info);
    t.add(i1);
    memcpy(info, "aaabb", N_CHAR);
    Item i2(0, info);
    t.add(i2);
    memcpy(info, "ababb", N_CHAR);
    Item i3(3, info);
    t.add(i3);
    t.get_table(table, N_ITEMS);

    ASSERT_EQ(t.search_info(info, -10, N_CHAR), SUCCESS);
    for (int i = 0; i < N_CHAR; i++) {
        ASSERT_EQ(info[i], table[0].info[i]);
    }
    ASSERT_EQ(t.search_info(info, 0, N_CHAR), SUCCESS);
    for (int i = 0; i < N_CHAR; i++) {
        ASSERT_EQ(info[i], table[1].info[i]);
    }
    ASSERT_EQ(t.search_info(info, 3, N_CHAR), SUCCESS);
    for (int i = 0; i < N_CHAR; i++) {
        ASSERT_EQ(info[i], table[2].info[i]);
    }

    ASSERT_EQ(t.search_info(info, 50, N_CHAR), FAIL);
    ASSERT_EQ(t.search_info(info, 55, N_CHAR), FAIL);
    ASSERT_EQ(t.search_info(info, 65, N_CHAR), FAIL);

	ASSERT_ANY_THROW(t.search_info(info, 65, N_CHAR - 2));
}

TEST(Methods, DeleteItemRefresh)
{
    Table t;
    Item table[N_ITEMS];
    char info[N_CHAR];

    memcpy(info, "aaaaa", N_CHAR);
    Item i1(-10, info);
    t.add(i1);
    memcpy(info, "aaabb", N_CHAR);
    Item i2(0, info);
    t.add(i2);
    memcpy(info, "ababb", N_CHAR);
    Item i3(3, info);
    t.add(i3);
    memcpy(info, "vvvvv", N_CHAR);
    Item i4(-19, info);
    t.add(i4);

    ASSERT_EQ(t.delete_item(-10), SUCCESS);
    ASSERT_EQ(t.delete_item(3), SUCCESS);
    ASSERT_EQ(t.delete_item(27), FAIL);

    t.get_table(table, N_ITEMS);
    ASSERT_EQ(table[0].busy, 0);
    ASSERT_EQ(table[1].busy, 1);
    ASSERT_EQ(table[2].busy, 0);
    ASSERT_EQ(table[3].busy, 1);

    t.refresh();
    t.get_table(table, N_ITEMS);
    ASSERT_EQ(table[0].busy, 1);
    ASSERT_EQ(table[0].key, 0);
    ASSERT_EQ(table[1].busy, 1);
    ASSERT_EQ(table[1].key, -19);
    ASSERT_EQ(table[2].busy, 0);
    ASSERT_EQ(table[3].busy, 0);
}

