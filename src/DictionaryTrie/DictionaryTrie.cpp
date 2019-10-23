/**
 * TODO: File Header
 *
 * Author:
 */
#include "DictionaryTrie.hpp"
#include <iostream>

/* TODO */
DictionaryTrie::DictionaryTrie() { root = nullptr; }

/* TODO */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    if (find(word)) return false;
    TSTNode* node = nullptr;
    // if root is null
    if (!root) {
        root = new TSTNode(word[0], freq);
        TSTNode* curr = root;
        for (int i = 1; i < word.size(); i++) {
            node = new TSTNode(word[i], freq);
            curr->mChild = node;
            curr = curr->mChild;
        }
        curr->isEnd = true;
    } else {
        TSTNode* curr = root;
        for (int i = 0; i < word.size(); i++) {
            if (!curr->mChild && i > 0) {
                curr->mChild = new TSTNode(word[i], freq);
                curr = curr->mChild;
            }
            if (word[i] == curr->data) {
                curr = curr->mChild;
            } else if (word[i] < curr->data) {
                if (!curr->lChild) {
                    curr->lChild = new TSTNode(word[i], freq);
                }
                curr = curr->lChild;
            } else {
                if (!curr->rChild) {
                    curr->rChild = new TSTNode(word[i], freq);
                }
                curr = curr->rChild;
            }
        }
        curr->isEnd = true;
    }
}

/* TODO */
bool DictionaryTrie::find(string word) const {
    if (!root) return false;
    TSTNode* curr = root;
    // find the head of the word
    int i = 0;
    bool found = false;
    while (true) {
        if (word[i] == curr->data && i < word.size() - 1) {
            curr = curr->mChild;
            i++;
        } else if (word[i] < curr->data) {
            curr = curr->lChild;
        } else {
            curr = curr->rChild;
        }
        // the last char of word
        if (curr->isEnd) {
            return true;
        }
        // Curr is null
        if (!curr) {
            return false;
        }
    }
}

/* TODO */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    return {};
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* TODO */
DictionaryTrie::~DictionaryTrie() {}