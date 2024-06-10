#include "storage.h"
#include <gtest/gtest.h>

// Тестирование метода PUT
TEST(StorageTest, PutTest) {
    Storage storage;
    EXPECT_EQ(storage.put("key1", "value1"), "OK\n");
    EXPECT_EQ(storage.put("key1", "value2"), "OK value1\n");
}

// Тестирование метода GET
TEST(StorageTest, GetTest) {
    Storage storage;
    storage.put("key1", "value1");
    EXPECT_EQ(storage.get("key1"), "OK value1\n");
    EXPECT_EQ(storage.get("key2"), "NE\n");
}

// Тестирование метода DEL
TEST(StorageTest, DelTest) {
    Storage storage;
    storage.put("key1", "value1");
    EXPECT_EQ(storage.del("key1"), "OK value1\n");
    EXPECT_EQ(storage.del("key2"), "NE\n");
}

// Тестирование метода COUNT
TEST(StorageTest, CountTest) {
    Storage storage;
    storage.put("key1", "value1");
    storage.put("key2", "value2");
    EXPECT_EQ(storage.count(), "OK 2\n");
    storage.del("key1");
    EXPECT_EQ(storage.count(), "OK 1\n");
}

// Тестирование методов DUMP и LOAD
TEST(StorageTest, DumpLoadTest) {
    Storage storage;
    storage.put("key1", "value1");
    storage.put("key2", "value2");
    storage.dump("test_dump.txt");

    Storage new_storage;
    EXPECT_TRUE(new_storage.load("test_dump.txt"));
    EXPECT_EQ(new_storage.get("key1"), "OK value1\n");
    EXPECT_EQ(new_storage.get("key2"), "OK value2\n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
