/**
 * TODO: File Header
 *
 * Author: Qing Niu
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <string>
#include <utility>
#include <vector>

using namespace std;

class Word {
  public:
    string word;
    int fq;
    Word(string w, int f) {
        word = w;
        fq = f;
    }
};

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
    // TODO: add private members and helper methods here
    TSTNode* root;

    /* Helper method for ~DictionaryTrie() */
    void deleteAll(TSTNode* n);

  public:
    /* TODO: add function header */
    DictionaryTrie();

    /* TODO: add function header */
    bool insert(string word, unsigned int freq);

    /* TODO: add function header */
    bool find(string word) const;

    /* TODO: add function header */
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
    bool operator()(const Word& w1, const Word& w2) { return w1.fq < w2.fq; }
};

#endif  // DICTIONARY_TRIE_HPP
