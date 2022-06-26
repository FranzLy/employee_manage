/*
 * @Descripttion: 哈希表单测
 * @version:
 * @Author: liyu
 * @Date: 2022-06-24 00:35:58
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "hash_table.h"
#include "log.h"

#ifdef __cplusplus
};
#endif

#include <gtest/gtest.h>
#include <iostream>

struct test_struct {
    int n;
    char buf[BUFSIZ];
};

void test_update_int_cbk(void* src, const void* dst)
{
    if (dst == NULL) {
        return;
    }

    src = (int*)src;
    dst = (int*)dst;

    *((int*)src) = *((int*)dst);
}

bool test_compare_int_cbk(const void* src, const void* dst)
{
    if (dst == NULL) {
        return true;
    }

    if (src == NULL) {
        return false;
    }

    return (*((int*)src) == *((int*)dst));
}

void test_update_struct_cbk(void* src, const void* dst)
{
    if (dst == NULL) {
        return;
    }

    test_struct* src_struc = (test_struct*)src;
    test_struct* dst_struc = (test_struct*)dst;

    memset(src_struc, 0, BUFSIZ);
    src_struc->n = dst_struc->n;
    strncpy(src_struc->buf, dst_struc->buf, strlen(dst_struc->buf));
}

bool test_compare_struct_cbk(const void* src, const void* dst)
{
    if (dst == NULL) {
        return true;
    }

    if (src == NULL) {
        return false;
    }

    test_struct* src_struc = (test_struct*)src;
    test_struct* dst_struc = (test_struct*)dst;
    return ((src_struc->n == dst_struc->n) && (strncmp(src_struc->buf, dst_struc->buf, strlen(src_struc->buf)) == 0));
}

TEST(TEST_HASH_CREATE, test_hash_create)
{
    //创建哈希桶值为0,失败
    EXPECT_TRUE(hash_create(0, 0, test_update_int_cbk, test_compare_int_cbk) == NULL);

    //元素值为0,失败
    EXPECT_TRUE(hash_create(HASH_SIZE, 0, test_update_int_cbk, test_compare_int_cbk) == NULL);

    // int类型哈希表,成功
    EXPECT_TRUE(hash_create(HASH_SIZE, sizeof(int), test_update_int_cbk, test_compare_int_cbk) != NULL);

    // struct类型哈希表,成功
    EXPECT_TRUE(hash_create(HASH_SIZE, sizeof(test_struct), test_update_struct_cbk, test_compare_struct_cbk) != NULL);
}

TEST(TEST_HASH_INSERT, test_hash_insert)
{
    ///> 测试int类型的哈希表插入
    hash_table_t* int_hash_table = hash_create(HASH_SIZE, sizeof(int), test_update_int_cbk, test_compare_int_cbk);
    int n1 = 5, n2 = 5, n3 = 1000;

    //哈希表空
    EXPECT_TRUE(hash_insert_node(NULL, 0, NULL) == -1);

    //插入非法元素
    EXPECT_TRUE(hash_insert_node(int_hash_table, 0, NULL) == -1);

    //插入两个值相同的元素
    EXPECT_TRUE(hash_insert_node(int_hash_table, 1, &n1) == 0);
    EXPECT_TRUE(hash_insert_node(int_hash_table, 2, &n2) == 0);

    //插入key相同的元素
    EXPECT_TRUE(hash_insert_node(int_hash_table, 1, &n2) != 0);

    //插入同一桶位上的元素
    EXPECT_TRUE(hash_insert_node(int_hash_table, HASH_SIZE + 1, &n1) == 0);

    //插入其他元素
    EXPECT_TRUE(hash_insert_node(int_hash_table, HASH_SIZE / 2, &n3) == 0);

    ///> 测试struct类型的哈希表插入
    hash_table_t* struct_hash_table = hash_create(HASH_SIZE, sizeof(test_struct), test_update_struct_cbk, test_compare_struct_cbk);
    test_struct item1 = { 0, "abc" }, item2 = { 0, "abc" }, item3 = { 132, "qwr" };

    //哈希表空
    EXPECT_TRUE(hash_insert_node(NULL, 0, NULL) == -1);

    //插入非法元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 0, NULL) != 0);

    //插入两个值相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 1, &item1) == 0);
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 2, &item2) == 0);

    //插入key相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 1, &item2) != 0);

    //插入同一桶位上的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE + 1, &item2) == 0);

    //插入其他元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE / 2, &item3) == 0);
}

TEST(TEST_HASH_DELETE, test_hash_delete)
{
    ///> 测试struct类型的哈希表插入
    hash_table_t* struct_hash_table = hash_create(HASH_SIZE, sizeof(test_struct), test_update_struct_cbk, test_compare_struct_cbk);
    test_struct item1 = { 0, "abc" }, item2 = { 0, "abc" }, item3 = { 132, "qwr" };

    ///> 先插入元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 1, &item1) == 0); //插入两个值相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 2, &item2) == 0);
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE + 1, &item2) == 0); //插入同一桶位上的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE / 2, &item3) == 0); //插入其他元素

    ///> 删除元素
    EXPECT_TRUE(hash_delete_node(NULL, 1) == -1); //哈希表为空,失败
    EXPECT_TRUE(hash_delete_node(struct_hash_table, 1) == 0); //删除存在的元素
    EXPECT_TRUE(hash_delete_node(struct_hash_table, 2) == 0);
    EXPECT_TRUE(hash_delete_node(struct_hash_table, HASH_SIZE + 1) == 0);
    EXPECT_TRUE(hash_delete_node(struct_hash_table, HASH_SIZE / 2) == 0);
    EXPECT_TRUE(hash_delete_node(struct_hash_table, HASH_SIZE / 3) != 0); //删除不存在的元素
}

TEST(TEST_HASH_UPDATE, test_hash_update)
{
    ///> 测试struct类型的哈希表插入
    hash_table_t* struct_hash_table = hash_create(HASH_SIZE, sizeof(test_struct), test_update_struct_cbk, test_compare_struct_cbk);
    test_struct item1 = { 0, "abc" }, item2 = { 0, "abc" }, item3 = { 132, "qwr" }, item4 = { 666, "update" };

    ///> 先插入元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 1, &item1) == 0); //插入两个值相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 2, &item2) == 0);
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE + 1, &item2) == 0); //插入同一桶位上的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE / 2, &item3) == 0); //插入其他元素

    ///> 修改元素
    EXPECT_TRUE(hash_update_node(NULL, 1, NULL) == -1); //哈希表为空,失败
    EXPECT_TRUE(hash_update_node(struct_hash_table, 1, NULL) == -1); //修改存在的元素,但传入值为空,失败
    EXPECT_TRUE(hash_update_node(struct_hash_table, 1, &item4) == 0); //修改存在的元素
    EXPECT_TRUE(hash_update_node(struct_hash_table, 2, &item4) == 0);
    EXPECT_TRUE(hash_update_node(struct_hash_table, HASH_SIZE + 1, &item4) == 0);
    EXPECT_TRUE(hash_update_node(struct_hash_table, HASH_SIZE / 2, &item4) == 0);
    EXPECT_TRUE(hash_update_node(struct_hash_table, HASH_SIZE / 3, &item4) != 0); //修改不存在的元素
}

TEST(TEST_HASH_SEARCH, test_hash_search)
{
    ///> 测试struct类型的哈希表插入
    hash_table_t* struct_hash_table = hash_create(HASH_SIZE, sizeof(test_struct), test_update_struct_cbk, test_compare_struct_cbk);
    test_struct item1 = { 0, "abc" }, item2 = { 0, "abc" }, item3 = { 132, "qwr" }, item4 = { 666, "update" };
    test_struct* item = NULL;

    ///> 先插入元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 1, &item1) == 0); //插入两个值相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 2, &item2) == 0);
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE + 1, &item2) == 0); //插入同一桶位上的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE / 2, &item3) == 0); //插入其他元素

    ///> 查找元素
    ASSERT_EQ(hash_search_node(NULL, 1) == NULL, true);
    item = (test_struct*)hash_search_node(struct_hash_table, 1);
    EXPECT_TRUE(test_compare_struct_cbk(item, &item1));
    item = (test_struct*)hash_search_node(struct_hash_table, 2);
    EXPECT_TRUE(test_compare_struct_cbk(item, &item2));
    item = (test_struct*)hash_search_node(struct_hash_table, HASH_SIZE + 1);
    EXPECT_TRUE(test_compare_struct_cbk(item, &item2));
    item = (test_struct*)hash_search_node(struct_hash_table, HASH_SIZE / 2);
    EXPECT_TRUE(test_compare_struct_cbk(item, &item3));
    item = (test_struct*)hash_search_node(struct_hash_table, HASH_SIZE / 3);
    EXPECT_TRUE(test_compare_struct_cbk(item, NULL));
}

TEST(TEST_HASH_TRAVERSE, test_hash_traverse)
{
    ///> 测试struct类型的哈希表
    hash_table_t* struct_hash_table = hash_create(HASH_SIZE, sizeof(test_struct), test_update_struct_cbk, test_compare_struct_cbk);
    test_struct item1 = { 0, "abc" }, item2 = { 0, "abc" }, item3 = { 132, "qwr" }, item4 = { 666, "update" };
    test_struct** itemes = NULL;
    int count = 0;

    ///> 先插入元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 1, &item1) == 0); //插入两个值相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 2, &item2) == 0);
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE + 1, &item2) == 0); //插入key相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE / 2, &item3) == 0); //插入其他元素

    ///> 遍历整表
    ASSERT_EQ(hash_traverse_all_nodes(NULL, NULL, NULL) == NULL, true);
    itemes = (test_struct**)hash_traverse_all_nodes(struct_hash_table, &count, NULL);
    EXPECT_TRUE(itemes != NULL);
    EXPECT_TRUE(count == 4);
}

TEST(TEST_HASH_DESTORY, test_hash_destory)
{
    ///> 测试struct类型的哈希表
    hash_table_t* struct_hash_table = hash_create(HASH_SIZE, sizeof(test_struct), test_update_struct_cbk, test_compare_struct_cbk);
    test_struct item1 = { 0, "abc" }, item2 = { 0, "abc" }, item3 = { 132, "qwr" }, item4 = { 666, "update" };
    test_struct* item = NULL;

    ///> 先插入元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 1, &item1) == 0); //插入两个值相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, 2, &item2) == 0);
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE + 1, &item2) == 0); //插入key相同的元素
    EXPECT_TRUE(hash_insert_node(struct_hash_table, HASH_SIZE / 2, &item3) == 0); //插入其他元素

    ///> 销毁整表
    EXPECT_TRUE(hash_destory(NULL) == -1);
    EXPECT_TRUE(hash_destory(struct_hash_table) == 0);
}
