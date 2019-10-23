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
};

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
  private:
    // TODO: add private members and helper methods here
    TSTNode* root;

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

    /* TODO: add function header */
    ~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
