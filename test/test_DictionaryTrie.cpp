/**
 * File HEADER
 * The test cases of Dictionary trie, including the
 * insert, find and autocomplete tests.
 *
 * Author: Qing Niu
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty test */
TEST(DictTrieTests, EMPTY_TEST) {
    DictionaryTrie dict;
    ASSERT_EQ(dict.find("abrakadabra"), false);
}

/* Build the following Dictionary based on TST node */
class DictTest : public ::testing::Test {
  protected:
    DictionaryTrie dict;

  public:
    DictTest() {
        // initialization code here
        dict.insert("jog", 5);
        dict.insert("a", 1);
        dict.insert("abc", 2);
        dict.insert("as", 3);
        dict.insert("cse", 4);
        dict.insert("mike", 6);
        dict.insert("char", 3);
        dict.insert("david", 2);
        dict.insert("ant", 4);
        dict.insert("and", 4);
    }
};

TEST_F(DictTest, FIND_TEST) {
    // assert that the word found
    ASSERT_TRUE(dict.find("ant"));
}

TEST_F(DictTest, NOT_FOUND_TEST) {
    // assert that the word not found
    ASSERT_FALSE(dict.find("abrak"));
}

TEST_F(DictTest, PREDICT_TEST) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string prefix = "a";
    vector<string> v1 = dict.predictCompletions(prefix, 4);
    vector<string> v2{"and", "ant", "as", "abc"};
    ASSERT_EQ((unsigned)v1.size(), (unsigned)v2.size());
    for (unsigned int i = 0; i < v1.size(); i++) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST_F(DictTest, PREDICT_TEST_2) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string prefix = "amg";
    vector<string> v1 = dict.predictCompletions(prefix, 6);
    ASSERT_EQ((unsigned)v1.size(), 0);
}

TEST_F(DictTest, PREDICT_TEST_3) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string prefix = "a g";
    vector<string> v1 = dict.predictCompletions(prefix, 6);
    ASSERT_EQ((unsigned)v1.size(), 0);
}

TEST_F(DictTest, PREDICT_TEST_4) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string prefix = "a";
    vector<string> v1 = dict.predictCompletions(prefix, 6);
    vector<string> v2{"and", "ant", "as", "abc", "a"};
    ASSERT_EQ((unsigned)v1.size(), (unsigned)v2.size());
    for (unsigned int i = 0; i < v1.size(); i++) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST_F(DictTest, WILDCARD_TEST) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string wildcard = "a_";
    vector<string> v1 = dict.predictUnderscores(wildcard, 6);
    vector<string> v2{"and", "ant", "as", "abc"};
    ASSERT_EQ((unsigned)v1.size(), (unsigned)v2.size());
    for (unsigned int i = 0; i < v1.size(); i++) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST_F(DictTest, WILDCARD_TEST_2) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string wildcard = "a__";
    vector<string> v1 = dict.predictUnderscores(wildcard, 6);
    vector<string> v2{"and", "ant", "abc"};
    ASSERT_EQ((unsigned)v1.size(), (unsigned)v2.size());
    for (unsigned int i = 0; i < v1.size(); i++) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST_F(DictTest, WILDCARD_TEST_3) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string wildcard = "a_c";
    vector<string> v1 = dict.predictUnderscores(wildcard, 3);
    vector<string> v2{"abc"};
    ASSERT_EQ((unsigned)v1.size(), (unsigned)v2.size());
    for (unsigned int i = 0; i < v1.size(); i++) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST_F(DictTest, WILDCARD_TEST_4) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string wildcard = "_nt";
    vector<string> v1 = dict.predictUnderscores(wildcard, 3);
    vector<string> v2{"ant"};
    ASSERT_EQ((unsigned)v1.size(), (unsigned)v2.size());
    for (unsigned int i = 0; i < v1.size(); i++) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST_F(DictTest, WILDCARD_TEST_5) {
    // assert that the size of vectors are equal
    // expect that the element of vectors are the same
    string wildcard = "_n_";
    vector<string> v1 = dict.predictUnderscores(wildcard, 6);
    vector<string> v2{"and", "ant"};
    ASSERT_EQ((unsigned)v1.size(), (unsigned)v2.size());
    for (unsigned int i = 0; i < v1.size(); i++) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}