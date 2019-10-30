/**
 * File Header
 *
 * Author: Qing Niu
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// The word node class
class Word {
  public:
    string word;
    int fq;
    Word(string w, int f) {
        word = w;
        fq = f;
    }
};

// Compare class for priority queue
class my_comp {
    bool ascending;

  public:
    my_comp(const bool& asd = false) { ascending = asd; }
    // Overload the compare
    bool operator()(const Word& w1, const Word& w2) {
        if (ascending) {
            if (w1.fq != w2.fq) {
                return w1.fq > w2.fq;
            } else {
                return w1.word < w2.word;
            }
        } else {
            if (w1.fq != w2.fq) {
                return w1.fq < w2.fq;
            } else {
                return w1.word > w2.word;
            }
        }
    }
};

// Define a type of priority queue to store the results of autocompletion
typedef priority_queue<Word, std::vector<Word>, my_comp> my_pri_queue;

/**
 * The class for a Trinary tree node.
 */
class TSTNode {
  public:
    TSTNode* lChild;
    TSTNode* rChild;
    TSTNode* mChild;
    char data;
    bool isEnd;
    int fq;

    TSTNode(char d, int q) {
        lChild = nullptr;
        rChild = nullptr;
        mChild = nullptr;
        data = d;
        isEnd = false;
        fq = q;
    }

    TSTNode(char d) {
        lChild = nullptr;
        rChild = nullptr;
        mChild = nullptr;
        data = d;
        isEnd = false;
        fq = 0;
    }
};

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
  private:
    // add private members and helper methods here
    TSTNode* root;

    /* Helper method for ~DictionaryTrie() */
    void deleteAll(TSTNode* n);

    /* Find last node of the prefix */
    TSTNode* find_last_char_node(string prefix) const;

    /* Helper function of autocompletion */
    void predictHelper(TSTNode* root, string prefix, my_pri_queue& r_que,
                       int num);

  public:
    /* Constructor */
    DictionaryTrie();

    /* Insert a word to a dictionary trie */
    bool insert(string word, unsigned int freq);

    /* Find out whether the word is in the dictionary */
    bool find(string word) const;

    /* Autocompletion */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* Destructor */
    ~DictionaryTrie();
};

/** The comparator used in sorting words based on frequency */
struct CompareFreq {
    bool operator()(const Word& w1, const Word& w2) {
        if (w1.fq != w2.fq) {
            return w1.fq > w2.fq;
        } else {
            return w1.word < w2.word;
        }
    }
} compareObj;

#endif  // DICTIONARY_TRIE_HPP
