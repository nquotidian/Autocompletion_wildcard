/**
 * TODO: File Header
 *
 * Author:
 */
#include "DictionaryTrie.hpp"
#include <iostream>
#include <stack>

/* TODO */
DictionaryTrie::DictionaryTrie() { root = nullptr; }

/* TODO */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    if (find(word) || word.empty()) return false;
    TSTNode* node = nullptr;
    // if root is null
    if (!root) {
        root = new TSTNode(word[0]);
        TSTNode* curr = root;
        for (int i = 1; i < word.size(); i++) {
            node = new TSTNode(word[i]);
            curr->mChild = node;
            curr = curr->mChild;
        }
        // the end node
        curr->fq = freq;
        curr->isEnd = true;
    } else {
        TSTNode* curr = root;
        for (int i = 0; i < word.size(); i++) {
            if (!curr->mChild && i > 0) {
                curr->mChild = new TSTNode(word[i]);
                curr = curr->mChild;
            }
            if (word[i] == curr->data) {
                curr = curr->mChild;
            } else if (word[i] < curr->data) {
                if (!curr->lChild) {
                    curr->lChild = new TSTNode(word[i]);
                }
                curr = curr->lChild;
            } else {
                if (!curr->rChild) {
                    curr->rChild = new TSTNode(word[i]);
                }
                curr = curr->rChild;
            }
        }
        curr->fq = freq;
        curr->isEnd = true;
    }
}

/* TODO */
bool DictionaryTrie::find(string word) const {
    if (!root) return false;
    TSTNode* curr = root;
    // find the head of the word
    int i = 0;
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

/** TODO
 * DFS the tree, first we find the last char of the prefix
 * then we DFS the sub-tree of that node
 * push root, string word = prefix
 * while stack is not empty
 *  pop out, if not root, word = prefix + char
 *      check whether the char been poped out is the end node, if it is,
 * result.push(word)
 *
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    vector<Word> result;
    TSTNode* curr = root;
    // find the last char of the prefix
    int i = 0;
    while (true) {
        if (prefix[i] == curr->data) {
            if (i == prefix.size() - 1) break;
            curr = curr->mChild;
            i++;
        } else if (prefix[i] < curr->data) {
            curr = curr->lChild;
        } else {
            curr = curr->rChild;
        }
    }

    // Performing an exhaustive search
    stack<char> stk;
    string suffix;
    stk.push(curr->data);
    while (!stk.empty()) {
    }
    return {};
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* TODO */
DictionaryTrie::~DictionaryTrie() { deleteAll(root); }

/** Helper method for ~BST() */
void DictionaryTrie::deleteAll(TSTNode* n) {
    if (n) {
        deleteAll(n->lChild);
        deleteAll(n->mChild);
        deleteAll(n->rChild);
        delete n;
    }
}