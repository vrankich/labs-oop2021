#include <fstream> 
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
    char info1[18] = "abababababababa";
    Item i1(-10, info1);
    ASSERT_EQ(1, i1.busy);
    ASSERT_EQ(-10, i1.key);
    ASSERT_STREQ(info1, i1.info);
    
    char info2[6] = "space";
    Item i2(100, info2);
    ASSERT_EQ(1, i2.busy);
    ASSERT_EQ(100, i2.key);
    ASSERT_STREQ(info1, i1.info);
}

TEST(TableConstructor, DefaultConstructor)
{
    Table t1;
    ASSERT_EQ(t1.get_size(), 0);
    ASSERT_EQ(t1.get_n(), 0);
    ASSERT_EQ(t1.get_table(), nullptr);

    Table t2(5);
    const Item *table = t2.get_table();
    ASSERT_EQ(t2.get_size(), 5);
    ASSERT_EQ(t2.get_n(), 0);
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(table[i].busy, 0);
        ASSERT_EQ(table[i].info, nullptr);
    }

    ASSERT_THROW(Table t3(-5), invalid_size);

    Table arr[2];
    ASSERT_EQ(arr[0].get_size(), 0);
    ASSERT_EQ(arr[0].get_n(), 0);
    ASSERT_EQ(arr[0].get_table(), nullptr);
    ASSERT_EQ(arr[1].get_size(), 0);
    ASSERT_EQ(arr[1].get_n(), 0);
    ASSERT_EQ(arr[1].get_table(), nullptr);
}

TEST(TableConstructor, InitConstructor)
{
    const int N_CHAR = 6;
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
    
    ASSERT_EQ(t.get_size(), 2);
    ASSERT_EQ(t.get_n(), 2);
    const Item *items = t.get_table();
    ASSERT_EQ(1, items[0].busy);
    ASSERT_EQ(27, items[0].key);
    ASSERT_STREQ(items[0].info, info1);
    ASSERT_EQ(1, items[1].busy);
    ASSERT_EQ(5, items[1].key);
    ASSERT_STREQ(items[1].info, info2);
    
    delete [] arr_info_key[0].second;
    delete [] arr_info_key[1].second;
}

TEST(TableConstructor, CopyConstructor)
{
    Table t1;
    char info[6] = "aaaaa";
    const std::pair<int, char*> key_info1 = std::make_pair(-10, info);
    t1.add(key_info1);
    const std::pair<int, char*> key_info2 = std::make_pair(0, info);
    t1.add(key_info2);
    const std::pair<int, char*> key_info3 = std::make_pair(3, info);
    t1.add(key_info3);
    const Item *table1 = t1.get_table();

    Table t2(t1);
    const Item *table2 = t2.get_table();
    ASSERT_EQ(t2.get_size(), t1.get_size());
    ASSERT_EQ(t2.get_n(), t1.get_n());
    ASSERT_EQ(table2[0].busy, table1[0].busy);
    ASSERT_EQ(table2[1].busy, table1[1].busy);
    ASSERT_EQ(table2[2].busy, table1[2].busy);
    ASSERT_EQ(table2[0].key, table1[0].key);
    ASSERT_EQ(table2[1].key, table1[1].key);
    ASSERT_EQ(table2[2].key, table1[2].key);
    ASSERT_STREQ(table2[0].info, table1[0].info);
    ASSERT_STREQ(table2[1].info, table1[1].info);
    ASSERT_STREQ(table2[2].info, table1[2].info);
}

TEST(Methods, Add)
{
    Table t;
    const Item *table;
    const int N_CHAR = 6;
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
}

TEST(Methods, SearchItem)
{
    Table t;
    char info[6] = "aaaaa";
    
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
    const int N_CHAR = 6;
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
  
    ASSERT_STREQ(t.search_info(-10), "aaaaa");
    ASSERT_STREQ(t.search_info(0), "aaabb");
    ASSERT_STREQ(t.search_info(3), "ababb");

    ASSERT_EQ(t.search_info(50), nullptr);
    ASSERT_EQ(t.search_info(55), nullptr);
    ASSERT_EQ(t.search_info(65), nullptr);
}

TEST(Methods, DeleteItemRefresh)
{
    Table t;
    const Item *table;
    const int N_CHAR = 6;
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
    for (int i = 0; i < t.get_size(); i++) {
        ASSERT_EQ(table[i].busy, 0);
    }
}

TEST(Operator, Indexation)
{
    Table t;
    char info[6] = "aaaaa";
    const std::pair<int, char*> key_info1 = std::make_pair(3, info);
    t.add(key_info1);
    const std::pair<int, char*> key_info2 = std::make_pair(0, info);
    t.add(key_info2);
    const std::pair<int, char*> key_info3 = std::make_pair(10, info);
    t.add(key_info3);

    ASSERT_EQ(t[0].key, 3);
    ASSERT_EQ(t[1].key, 0);
    ASSERT_EQ(t[2].key, 10);

    ASSERT_THROW(t[t.get_size() + 1], invalid_index);
}

//TEST(Operator, PlusEqual)
//{
//    Table t;
//    char info[6] = "aaaaa";
//    Item item(3, info);
//    t += item;
//    item.key = 0;
//    t += item;
//    item.key = 10;
//    t += item;
//
//    ASSERT_EQ(t.get_n(), 3);
//    for (int i = 0; i < 3; i++) {
//        ASSERT_EQ(t[i].busy, 1);
//    }
//    ASSERT_EQ(t[0].key, 3);
//    ASSERT_EQ(t[1].key, 0);
//    ASSERT_EQ(t[2].key, 10);
//
//   for (int i = t.get_n(); i < t.get_size(); i++) {
//       ASSERT_EQ(t[i].busy, 0);
//   }
//
//    ASSERT_THROW(t += item, equal_key);
//    item.key = -1;
//    t += item;
//    item.key = -10;
//    t += item;
//
//    for (int i = 0; i < t.get_n(); i++) {
//        ASSERT_EQ(t[i].busy, 1);
//    }
//}

TEST(Operator, Addition)
{
    Table t1, t2;
    char info[6] = "aaaaa";
    std::pair<int, char*> key_info = std::make_pair(1, info);
    t1.add(key_info);
    key_info.first = 0;
    t1.add(key_info);
    key_info.first = 10;
    t1.add(key_info);
    key_info.first = 3;
    t2.add(key_info);
    key_info.first = 11;
    t2.add(key_info);

    int sum_n = t1.get_n() + t2.get_n();
    Table t = t1 + t2;
    ASSERT_EQ(t.get_n(), sum_n);
    ASSERT_EQ(t.get_size(), sum_n);
    for (int i = 0; i < sum_n; i++) {
        ASSERT_EQ(t[i].busy, 1);
    }
    ASSERT_EQ(t[0].key, 1);
    ASSERT_EQ(t[1].key, 0);
    ASSERT_EQ(t[2].key, 10);
    ASSERT_EQ(t[3].key, 3);
    ASSERT_EQ(t[4].key, 11);

    key_info.first = 100;
    t1.add(key_info);
    key_info.first = 100;
    t2.add(key_info);
    ASSERT_THROW(t1 + t2, equal_key);
}

TEST(Operator, MinusEqual)
{
    Table t1, t2;
    const int N_CHAR = 5;
    char info[N_CHAR] = "vrvr";
    std::pair<int, char*> key_info = std::make_pair(3, info);
    t1.add(key_info);
    key_info.first = 0;
    t1.add(key_info);
    key_info.first = 10;
    t1.add(key_info);
    key_info.first = 9;
    t2.add(key_info);
    
    /* no equal items */
    t1 -= t2;
    ASSERT_EQ(t1[0].busy, 1);
    ASSERT_EQ(t1[1].busy, 1);
    ASSERT_EQ(t1[2].busy, 1);

    key_info.first = 10;
    t2.add(key_info);

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

    key_info.first = 9;
    t1.add(key_info);
    ASSERT_EQ(t1[t1.get_n() - 1].busy, 1);
    ASSERT_EQ(t1[t1.get_n() - 1].key, 9);
    t1 -= t2;
    ASSERT_EQ(t1[t1.get_n() - 1].busy, 0);

    /* equal keys but different information */
    key_info.first = 50;
    t1.add(key_info);
    memcpy(key_info.second, info, N_CHAR);
    ASSERT_EQ(t1[t1.get_n() - 1].busy, 1);
    ASSERT_EQ(t1[t1.get_n() - 1].key, 50);
    t1 -= t2;
    ASSERT_EQ(t1[t1.get_n() - 1].busy, 1);
}

TEST(Operator, Minus)
{
    Table t1, t2, t;
    const int N_CHAR = 7;
    char info[N_CHAR] = "vrvrvr";
    std::pair<int, char*> key_info = std::make_pair(3, info);
    t1.add(key_info);
    key_info.first = 0;
    t1.add(key_info);
    key_info.first = 10;
    t1.add(key_info);
    key_info.first = 9;
    t2.add(key_info);
    
    /* no equal keys
     * all items from the first table should be copied */
    t = t1 - t2;
    ASSERT_EQ(t.get_size(), 3);
    ASSERT_EQ(t[0].busy, 1);
    ASSERT_EQ(t[1].busy, 1);
    ASSERT_EQ(t[2].busy, 1);
    ASSERT_EQ(t[0].key, 3);
    ASSERT_EQ(t[1].key, 0);
    ASSERT_EQ(t[2].key, 10);

    key_info.first = 11;
    t1.add(key_info);
    t2.add(key_info);
    key_info.first = 20;
    t1.add(key_info);
   memcpy(key_info.second, "aaaaa", N_CHAR);
    key_info.first = 30;
    t2.add(key_info);
   t = t1 - t2;
   ASSERT_EQ(t[0].busy, 1);
   ASSERT_EQ(t[1].busy, 1);
   ASSERT_EQ(t[2].busy, 1);
   ASSERT_EQ(t[3].busy, 1);
   ASSERT_EQ(t[0].key, 3);
   ASSERT_EQ(t[1].key, 0);
   ASSERT_EQ(t[2].key, 10);
   ASSERT_EQ(t[3].key, 20);
}

TEST(Operator, Comparation)
{
    char info1[6] = "ababa";
    Item i1(27, info1), i2(27, info1);

    ASSERT_TRUE(i1 == i2);
    ASSERT_FALSE(i1 != i2);

    i1.key = 2;
    ASSERT_TRUE(i1 != i2);
    ASSERT_FALSE(i1 == i2);

    i2.key = 2;
    const int N_CHAR = 6;
    char info2[N_CHAR] = "aaaaa";
    memcpy(i2.info, info2, N_CHAR);
    ASSERT_TRUE(i1 != i2);
    ASSERT_FALSE(i1 == i2);

    i1.key = 1;
    ASSERT_TRUE(i1 != i2);
    ASSERT_FALSE(i1 == i2);
}

TEST(Operator, ItemInOut)
{
    std::fstream file("test1.txt");
    
    /* write items to file */
    char info[5] = "aaaa";
    Item item1(30, info);
    Item item2(-100, info);
    file << item1;
    file << item2;
    
    /* read items from file */
    file.seekg(0, file.beg);
    std::pair<int, char*> read_item1;
    file >> read_item1;
    ASSERT_EQ(item1.key, read_item1.first);
    ASSERT_STREQ(item1.info, read_item1.second);
    delete [] read_item1.second;
    std::pair<int, char*> read_item2;
    file >> read_item2;
    ASSERT_EQ(item2.key, read_item2.first);
    ASSERT_STREQ(item2.info, read_item2.second);
    delete [] read_item2.second;
    
    file.close();
}

TEST(Operator, TableInOut)
{
    std::fstream file ("test2.txt");

    Table t1, t2;
    const int N_CHAR = 7;
    char info[N_CHAR] = "vrvrvr";
    std::pair<int, char*> key_info = std::make_pair(3, info);
    t1.add(key_info);
    key_info.first = 0;
    t1.add(key_info);
    key_info.first = 10;
    t1.add(key_info);
    key_info.first = 9;
    t2.add(key_info);
    key_info.first = -1;
    t2.add(key_info);
    file << t1.get_n() << std::endl;
    file << t1;
    file << t2.get_n() << std::endl;
    file << t2;

    file.seekg(0, file.beg);
    Table t1_read, t2_read;
    file >> t1_read;
    file >> t2_read;
    ASSERT_EQ(t1.get_size(), t1_read.get_size());
    ASSERT_EQ(t1.get_n(), t1_read.get_n());
//    Item item1, item2;
//    for (int i = 0; i < t1.get_size(); i++) {
//        item1 = t1[i];
//        item2 = t1_read[i];
//        ASSERT_TRUE(item1 == item2);
//    }
//    for (int i = 0; i < t2.get_size(); i++) {
//        item1 = t2[i];
//        item2 = t2_read[i];
//        ASSERT_TRUE(item1 == item2);
//    }

    file.close();
}
