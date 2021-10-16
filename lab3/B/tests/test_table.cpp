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
    
    char info3[N_CHAR + 1] = "abcdef";
    ASSERT_THROW(Item i3(2, info3), invalid_len);
}

TEST(TableConstructor, DefaultConstructor)
{
    Table t;
    ASSERT_EQ(t.get_size(), N_ITEMS);
    ASSERT_EQ(t.get_info_len(), N_CHAR - 1);
    ASSERT_EQ(t.get_n(), 0);
    const Item *items = t.get_table();
    for(int i = 0; i < N_ITEMS; i++) {
        ASSERT_EQ(0, items[i].busy);
    }
    
    Table arr[2];
    
    ASSERT_EQ(arr[0].get_size(), N_ITEMS);
    ASSERT_EQ(arr[0].get_info_len(), N_CHAR - 1);
    ASSERT_EQ(arr[0].get_n(), 0);
    items = arr[0].get_table();
    for(int i = 0; i < N_ITEMS; i++) {
        ASSERT_EQ(0, items[i].busy);
    }
    
    ASSERT_EQ(arr[1].get_size(), N_ITEMS);
    ASSERT_EQ(arr[1].get_info_len(), N_CHAR - 1);
    ASSERT_EQ(arr[1].get_n(), 0);
    items = arr[1].get_table();
    for(int i = 0; i < N_ITEMS; i++) {
        ASSERT_EQ(0, items[i].busy);
    }
}

TEST(TableConstructor, InitConstructor)
{
    const char info1[N_CHAR] = "abcde"; 
    const char info2[N_CHAR] = "vrvrv";
    std::pair<int, char*> arr_info_key[2];
    arr_info_key[0].first = 27;
    arr_info_key[0].second = new char[N_CHAR];
    memcpy(arr_info_key[0].second, info1, N_CHAR);
    arr_info_key[1].first = 5;
    arr_info_key[1].second = new char[N_CHAR];
    memcpy(arr_info_key[1].second, info2, N_CHAR);
    
    Table t(arr_info_key, 2);
    
    ASSERT_EQ(t.get_size(), N_ITEMS);
    ASSERT_EQ(t.get_info_len(), N_CHAR - 1);
    ASSERT_EQ(t.get_n(), 2);
    const Item *items = t.get_table();
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
    
    delete [] arr_info_key[0].second;
    delete [] arr_info_key[1].second;
    
    ASSERT_THROW(Table(arr_info_key, N_ITEMS + 1), invalid_table_size);
}

TEST(Methods, Add)
{
    Table t;
    const Item *table;
    char info[N_CHAR] = "aaaaa";
    
    std::pair<int, char*> key_info1 = std::make_pair(-10, info);
    t.add(key_info1);
    table = t.get_table();
    ASSERT_EQ(table[0].key, -10);
    
    std::pair<int, char*> key_info2 = std::make_pair(0, info);
    t.add(key_info2);
    table = t.get_table();
    ASSERT_EQ(table[1].key, 0);
    
    std::pair<int, char*> key_info3 = std::make_pair(3, info);
    t.add(key_info3);
    table = t.get_table();
    ASSERT_EQ(table[2].key, 3);
    
    std::pair<int, char*> key_info4 = std::make_pair(-19, info);
    t.add(key_info4);
    table = t.get_table();
    ASSERT_EQ(table[3].key, -19);
    
    std::pair<int, char*> key_info5 = std::make_pair(-19, info);
    ASSERT_THROW(t.add(key_info5), equal_key);
    
    std::pair<int, char*> key_info6 = std::make_pair(100, info);
    t.add(key_info6);
    table = t.get_table();
    ASSERT_EQ(table[4].key, 100);
    
    std::pair<int, char*> key_info7 = std::make_pair(11, info);
    ASSERT_THROW(t.add(key_info7), table_overflow);
}

TEST(Methods, SearchItem)
{
    Table t;
    char info[N_CHAR] = "aaaaa";
    
    const std::pair<int, char*> key_info1 = std::make_pair(-10, info);
    t.add(key_info1);
    const std::pair<int, char*> key_info2 = std::make_pair(0, info);
    t.add(key_info2);
    const std::pair<int, char*> key_info3 = std::make_pair(3, info);
    t.add(key_info3);
    
    Item i;
    ASSERT_EQ(t.search_item(i, -10), SUCCESS);
    ASSERT_EQ(t.search_item(i, 0), SUCCESS);
    ASSERT_EQ(t.search_item(i, 3), SUCCESS);
    
    ASSERT_EQ(t.search_item(i, 1), FAIL);
    ASSERT_EQ(t.search_item(i, 2), FAIL);
    ASSERT_EQ(t.search_item(i, -1), FAIL);
}

TEST(Methods, SearchInfo)
{
    Table t;
    const Item *table;
    char info[N_CHAR];
    
    memcpy(info, "aaaaa", N_CHAR);
    std::pair<int, char*> key_info1 = std::make_pair(-10, info);
    t.add(key_info1);
    memcpy(info, "aaabb", N_CHAR);
    std::pair<int, char*> key_info2 = std::make_pair(0, info);
    t.add(key_info2);
    memcpy(info, "ababb", N_CHAR);
    std::pair<int, char*> key_info3 = std::make_pair(3, info);
    t.add(key_info3);
    table = t.get_table();
    
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
    const Item *table;
    char info[N_CHAR] = "aaaaa";
    
    std::pair<int, char*> key_info1 = std::make_pair(-10, info);
    t.add(key_info1);
    std::pair<int, char*> key_info2 = std::make_pair(0, info);
    t.add(key_info2);
    std::pair<int, char*> key_info3 = std::make_pair(3, info);
    t.add(key_info3);
    std::pair<int, char*> key_info4 = std::make_pair(-19, info);
    t.add(key_info4);
    
    ASSERT_EQ(t.delete_item(-10), SUCCESS);
    ASSERT_EQ(t.delete_item(3), SUCCESS);
    ASSERT_EQ(t.delete_item(27), FAIL);
    
    table = t.get_table();
    ASSERT_EQ(table[0].busy, 0);
    ASSERT_EQ(table[1].busy, 1);
    ASSERT_EQ(table[2].busy, 0);
    ASSERT_EQ(table[3].busy, 1);
    ASSERT_EQ(t.get_n(), 4);
    
    t.refresh();
    table = t.get_table();
    ASSERT_EQ(t.get_n(), 2);
    ASSERT_EQ(table[0].busy, 1);
    ASSERT_EQ(table[0].key, 0);
    ASSERT_EQ(table[1].busy, 1);
    ASSERT_EQ(table[1].key, -19);
    ASSERT_EQ(table[2].busy, 0);
    ASSERT_EQ(table[3].busy, 0);
    
    ASSERT_EQ(t.delete_item(0), SUCCESS);
    ASSERT_EQ(t.delete_item(-19), SUCCESS);
    t.refresh();
    table = t.get_table();
    ASSERT_EQ(t.get_n(), 0);
    for (int i = 0; i < N_ITEMS; i++) {
        ASSERT_EQ(table[i].busy, 0);
    }
}

TEST(Operator, Indexation)
{
    Table t;
    Item item(3, "aaaaa");
    t += item;
    item.key = 0;
    t += item;
    item.key = 10;
    t += item;

    ASSERT_EQ(t[0].key, 3);
    ASSERT_EQ(t[1].key, 0);
    ASSERT_EQ(t[2].key, 10);

    ASSERT_THROW(t[N_ITEMS + 1], invalid_index);
}

TEST(Operator, PlusEqual)
{
    Table t;
    Item item(3, "aaaaa");
    t += item;
    item.key = 0;
    t += item;
    item.key = 10;
    t += item;

    ASSERT_EQ(t.get_n(), 3);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(t[i].busy, 1);
    }
    ASSERT_EQ(t[0].key, 3);
    ASSERT_EQ(t[1].key, 0);
    ASSERT_EQ(t[2].key, 10);

    for (int i = t.get_n(); i < N_ITEMS; i++) {
        ASSERT_EQ(t[i].busy, 0);
    }

    ASSERT_THROW(t += item, equal_key);
    item.key = -1;
    t += item;
    item.key = -10;
    t += item;

    for (int i = 0; i < t.get_n(); i++) {
        ASSERT_EQ(t[i].busy, 1);
    }

    item.key = 100;
    ASSERT_THROW(t += item, table_overflow);
}

TEST(Operator, Addition)
{
    Table t1, t2;
    Item item(1, "aaaaa");
    t1 += item;
    item.key = 0;
    t1 += item;
    item.key = 10;
    t1 += item;
    item.key = 3;
    t2 += item;
    item.key = 11;
    t2 += item;

    int sum_n = t1.get_n() + t2.get_n();
    Table t = t1 + t2;
    ASSERT_EQ(t.get_n(), sum_n);
    for (int i = 0; i < sum_n; i++) {
        ASSERT_EQ(t[i].busy, 1);
    }
    for (int i = sum_n; i < N_ITEMS; i++) {
        ASSERT_EQ(t[i].busy, 0);
    }
    ASSERT_EQ(t[0].key, 1);
    ASSERT_EQ(t[1].key, 0);
    ASSERT_EQ(t[2].key, 10);
    ASSERT_EQ(t[3].key, 3);
    ASSERT_EQ(t[4].key, 11);
}

TEST(Operator, UnaryMinus)
{
    Table t;
    Item item(3, "aaaaa");
    t += item;
    item.key = 0;
    t += item;
    item.key = 10;
    t += item;
    item.key = 9;
    t += item;
    item.key = -10;
    t += item;

    t.delete_item(10);
    ASSERT_EQ(t[2].busy, 0);
    Table t1 = --t;
    ASSERT_EQ(t1[2].busy, 1);
    ASSERT_EQ(t1[2].key, 9);
    ASSERT_EQ(t[2].busy, 1);
    ASSERT_EQ(t[2].key, 9);

    t.delete_item(9);
    t1 = t--;
    ASSERT_EQ(t.get_n(), t1.get_n() - 1);
    ASSERT_EQ(t1[2].busy, 0);
    ASSERT_EQ(t1[3].key, -10);
    ASSERT_EQ(t[2].busy, 1);
    ASSERT_EQ(t[2].key, -10);

    t.delete_item(3);
    t1 = t--;
    ASSERT_EQ(t.get_n(), t1.get_n() - 1);
    ASSERT_EQ(t1[0].busy, 0);
    ASSERT_EQ(t1[1].key, 0);
    ASSERT_EQ(t[0].busy, 1);
    ASSERT_EQ(t[0].key, 0);
}

TEST(Operator, MinusEqual)
{
    Table t1, t2;
    Item item(3, "aaaaa");
    t1 += item;
    item.key = 0;
    t1 += item;
    item.key = 10;
    t1 += item;
    item.key = 9;
    t2 += item;

    /* no equal items */
    t1 -= t2;
    ASSERT_EQ(t1[0].busy, 1);
    ASSERT_EQ(t1[1].busy, 1);
    ASSERT_EQ(t1[2].busy, 1);

    item.key = 10;
    t2 += item;

    t1 -= t2;
    ASSERT_EQ(t1[0].busy, 1);
    ASSERT_EQ(t1[1].busy, 1);
    ASSERT_EQ(t1[2].busy, 0);
    ASSERT_EQ(t2[0].busy, 1);
    ASSERT_EQ(t2[1].busy, 1);
    ASSERT_EQ(t1[0].key, 3);
    ASSERT_EQ(t1[1].key, 0);
    ASSERT_EQ(t2[0].key, 9);
    ASSERT_EQ(t2[1].key, 10);

    item.key = 9;
    t1 += item;
    ASSERT_EQ(t1[t1.get_n() - 1].busy, 1);
    ASSERT_EQ(t1[t1.get_n() - 1].key, 9);
    t1 -= t2;
    ASSERT_EQ(t1[t1.get_n() - 1].busy, 0);

    /* equal keys but different information */
    item.key = 50;
    t1 += item;
    char info[N_CHAR] = "vrvrv";
    memcpy(item.info, info, N_CHAR);
    ASSERT_EQ(t1[t1.get_n() - 1].busy, 1);
    ASSERT_EQ(t1[t1.get_n() - 1].key, 50);
    t1 -= t2;
    ASSERT_EQ(t1[t1.get_n() - 1].busy, 1);
}

TEST(Operator, Minus)
{
    Table t1, t2, t;
    Item item(3, "aaaaa");
    t1 += item;
    item.key = 0;
    t1 += item;
    item.key = 10;
    t1 += item;
    item.key = 9;
    t2 += item;

    /* no equal keys
     * all items from the first table should be copied */
    t = t1 - t2;
    ASSERT_EQ(t[0].busy, 1);
    ASSERT_EQ(t[1].busy, 1);
    ASSERT_EQ(t[2].busy, 1);
    ASSERT_EQ(t[3].busy, 0);
    ASSERT_EQ(t[4].busy, 0);
    ASSERT_EQ(t[0].key, 3);
    ASSERT_EQ(t[1].key, 0);
    ASSERT_EQ(t[2].key, 10);

    item.key = 11;
    t1 += item;
    t2 += item;
    item.key = 20;
    t1 += item;
    char info[N_CHAR] = "vrvrv";
    memcpy(item.info, info, N_CHAR);
    item.key = 30;
    t2 += item;
    t = t1 - t2;
    ASSERT_EQ(t[0].busy, 1);
    ASSERT_EQ(t[1].busy, 1);
    ASSERT_EQ(t[2].busy, 1);
    ASSERT_EQ(t[3].busy, 1);
    ASSERT_EQ(t[4].busy, 0);
    ASSERT_EQ(t[0].key, 3);
    ASSERT_EQ(t[1].key, 0);
    ASSERT_EQ(t[2].key, 10);
    ASSERT_EQ(t[3].key, 20);
}

TEST(Operator, Comparation)
{
    Item i1(27, "ababa"), i2(27, "ababa");

    ASSERT_TRUE(i1 == i2);
    ASSERT_FALSE(i1 != i2);

    i1.key = 2;
    ASSERT_TRUE(i1 != i2);
    ASSERT_FALSE(i1 == i2);

    i2.key = 2;
    char info[N_CHAR] = "aaaaa";
    memcpy(i2.info, info, N_CHAR);
    ASSERT_TRUE(i1 != i2);
    ASSERT_FALSE(i1 == i2);

    i1.key = 1;
    ASSERT_TRUE(i1 != i2);
    ASSERT_FALSE(i1 == i2);
}

