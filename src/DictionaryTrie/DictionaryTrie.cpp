/**
 * TODO: File Header
 *
 * Author:
 */
#include "DictionaryTrie.hpp"
#include <algorithm>
#include <iostream>
#include <stack>

void predictHelper(TSTNode* root, vector<Word>& vec, string prefix);

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
        for (unsigned int i = 1; i < word.size(); i++) {
            node = new TSTNode(word[i]);
            curr->mChild = node;
            curr = node;
        }
        // the end node
        curr->fq = freq;
        curr->isEnd = true;
        return true;
    } else {
        TSTNode* curr = root;
        unsigned int i = 0;
        while (true) {
            if (word[i] < curr->data) {
                if (curr->lChild == nullptr) {
                    curr->lChild = new TSTNode(word[i]);
                    curr = curr->lChild;
                    for (; i < word.size(); i++) {
                        curr->mChild = new TSTNode(word[i]);
                        curr = curr->mChild;
                    }
                    curr->isEnd = true;
                    return true;
                } else {
                    curr = curr->lChild;
                }
            } else if (word[i] > curr->data) {
                if (curr->rChild == nullptr) {
                    curr->rChild = new TSTNode(word[i]);
                    curr = curr->rChild;

                    for (; i < word.size(); i++) {
                        curr->mChild = new TSTNode(word[i]);
                        curr = curr->mChild;
                    }
                    curr->isEnd = true;
                    return true;
                } else {
                    curr = curr->rChild;
                }
            } else {
                if (i == word.size() - 1) {
                    curr->isEnd = true;
                    return true;
                } else {
                    if (curr->mChild != nullptr) {
                        curr = curr->mChild;
                        i++;
                    } else {
                        for (; i < word.size(); i++) {
                            curr->mChild = new TSTNode(word[i]);
                            curr = curr->mChild;
                        }
                        curr->isEnd = true;
                        return true;
                    }
                }
            }
        }
    }
}

/* TODO */
bool DictionaryTrie::find(string word) const {
    if (!root) return false;
    TSTNode* curr = root;
    // find the word
    int i = 0;
    while (true) {
        if (word[i] < curr->data) {
            if (curr->lChild == nullptr) {
                return false;
            } else {
                curr = curr->lChild;
            }
        } else if (word[i] > curr->data) {
            if (curr->rChild == nullptr) {
                return false;
            } else {
                curr = curr->rChild;
            }
        } else {
            // letter is the last letter
            if (i == word.size() - 1) {
                return curr->isEnd ? true : false;
            } else {
                if (curr->mChild == nullptr) {
                    return false;
                } else {
                    curr = curr->mChild;
                    i++;
                }
            }
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
    TSTNode* curr = root;
    vector<string> rt_vec;
    // find the last char of the prefix
    int i = 0;
    while (true) {
        if (prefix[i] < curr->data) {
            if (curr->lChild == nullptr) {
                return rt_vec;
            } else {
                curr = curr->lChild;
            }
        } else if (prefix[i] > curr->data) {
            if (curr->rChild == nullptr) {
                return rt_vec;
            } else {
                curr = curr->rChild;
            }
        } else {
            // letter is the last letter
            if (i == prefix.size() - 1) {
                break;
            } else {
                if (curr->mChild == nullptr) {
                    return rt_vec;
                } else {
                    curr = curr->mChild;
                    i++;
                }
            }
        }
    }
    // Performing an exhaustive search
    stack<TSTNode*> stk;
    vector<Word> result;
    string str = prefix;

    // if prefix is also a word
    if (curr->isEnd) {
        result.push_back(Word(prefix, curr->fq));
    }
    // If node has no middle child, return
    if (curr->mChild != nullptr) {
        curr = curr->mChild;
    } else {
        return rt_vec;
    }
    predictHelper(curr, result, str);

    std::sort(result.begin(), result.end(), compareObj);
    int n = ((result.size() < numCompletions) ? result.size() : numCompletions);
    for (int j = 0; j < n; j++) {
        rt_vec.push_back(result[j].word);
    }
    return rt_vec;
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

void predictHelper(TSTNode* root, vector<Word>& vec, string prefix) {
    if (root == nullptr) {
        return;
    }
    if (root->isEnd) {
        string str = prefix + root->data;
        vec.push_back(Word(str, root->fq));
        return;
    }
    // Leaf node
    if (root->lChild == nullptr && root->rChild == nullptr &&
        root->rChild == nullptr)
        return;
    if (root->lChild != nullptr) predictHelper(root->lChild, vec, prefix);
    if (root->mChild != nullptr)
        predictHelper(root->mChild, vec, (prefix + root->data));
    if (root->rChild != nullptr) predictHelper(root->rChild, vec, prefix);
}