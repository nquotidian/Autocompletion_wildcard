/**
 * File Header
 *
 * Author: Qing Niu, the implementino of the header file DictionaryTrie.hpp
 */
#include "DictionaryTrie.hpp"
#include <algorithm>
#include <iostream>

// Helper methods
void predictHelper(TSTNode* root, string prefix, my_pri_queue& p_que);
void traverse(TSTNode* root);

/* Initialize DictionaryTrie */
DictionaryTrie::DictionaryTrie() { root = nullptr; }

/* Insert node to dictionary*
 * Parameter: word - the word in the TST node
 *            freq - the frequency of the word
 * Retur value: true - insert successfully
 *              false - failed on inserting
 * */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    if (find(word) || word.empty()) return false;
    TSTNode* node = nullptr;
    // if root is null
    if (root == nullptr) {
        root = new TSTNode(word[0]);
        TSTNode* curr = root;
        for (unsigned int i = 1; i < word.size(); i++) {
            node = new TSTNode(word[i]);
            curr->mChild = node;
            curr = node;
        }
        // the end node
        curr->isEnd = true;
        curr->fq = freq;
        return true;
    } else {
        TSTNode* curr = root;
        unsigned int i = 0;
        while (true) {
            if (word[i] < curr->data) {
                if (curr->lChild == nullptr) {
                    curr->lChild = new TSTNode(word[i]);
                    curr = curr->lChild;
                    for (i = i + 1; i < word.size(); i++) {
                        curr->mChild = new TSTNode(word[i]);
                        curr = curr->mChild;
                    }
                    curr->isEnd = true;
                    curr->fq = freq;
                    return true;
                } else {
                    curr = curr->lChild;
                }
            } else if (word[i] > curr->data) {
                if (curr->rChild == nullptr) {
                    curr->rChild = new TSTNode(word[i]);
                    curr = curr->rChild;

                    for (i = i + 1; i < word.size(); i++) {
                        curr->mChild = new TSTNode(word[i]);
                        curr = curr->mChild;
                    }
                    curr->isEnd = true;
                    curr->fq = freq;
                    return true;
                } else {
                    curr = curr->rChild;
                }
            } else {
                if (i == word.size() - 1) {
                    curr->isEnd = true;
                    curr->fq = freq;
                    return true;
                } else {
                    if (curr->mChild != nullptr) {
                        curr = curr->mChild;
                        i++;
                    } else {
                        for (i = i + 1; i < word.size(); i++) {
                            curr->mChild = new TSTNode(word[i]);
                            curr = curr->mChild;
                        }
                        curr->isEnd = true;
                        curr->fq = freq;
                        return true;
                    }
                }
            }
        }
    }
}

/* Find a word in the dictionary
 * Parameter: word - the word to find
 * Retur value: true - found successfully
 *              false - not found
 * */
bool DictionaryTrie::find(string word) const {
    if (!root) return false;
    TSTNode* curr = root;
    // find the word
    unsigned int i = 0;
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
            if (i == word.size() - 1 && curr->isEnd) {
                return true;
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

/* Autocomplete function a word
 * Parameter: prfix - the prefix of the autocompletion
 *            numCompletions - the number of words on the vector
 *                             if bigger than the size of vector,
 *                              return the size of the vector
 * Retur value: vector<string> - the results of the autocompletion
 *              on descending order of frequency of each word;
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    // The result vector to return
    vector<string> rt_vec;
    // find the last char of the prefix
    TSTNode* curr = find_last_char_node(prefix);

    if (curr == nullptr) {
        return rt_vec;
    }

    // Performing an exhaustive search
    vector<Word> result;
    my_pri_queue que;

    // if prefix is also a word
    if (curr->isEnd) {
        que.push((Word(prefix, curr->fq)));
    }
    // If node has no middle child, return directly
    if (curr->mChild != nullptr) {
        curr = curr->mChild;
    } else {
        rt_vec.push_back(prefix);
        return rt_vec;
    }
    predictHelper(curr, prefix, que);
    int n = ((que.size() <= numCompletions) ? que.size() : numCompletions);
    // Get all the first n words in the queue
    string w;
    for (int i = 0; i < n; i++) {
        w = que.top().word;
        que.pop();
        rt_vec.push_back(w);
    }
    return rt_vec;
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* Find the last char node of prefix or word */
TSTNode* DictionaryTrie::find_last_char_node(string prefix) {
    TSTNode* curr = root;
    unsigned int i = 0;
    while (true) {
        if (prefix[i] < curr->data) {
            if (curr->lChild == nullptr) {
                return nullptr;
            } else {
                curr = curr->lChild;
            }
        } else if (prefix[i] > curr->data) {
            if (curr->rChild == nullptr) {
                return nullptr;
            } else {
                curr = curr->rChild;
            }
        } else {
            // letter is the last letter
            if (i == prefix.size() - 1) {
                break;
            } else {
                if (curr->mChild == nullptr) {
                    return nullptr;
                } else {
                    curr = curr->mChild;
                    i++;
                }
            }
        }
    }
    return curr;
}

/* Destructor of DictionaryTrie */
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

/* Helper method of autocomplete
 * Parameter: root - the current node
 *            vec - the vector to return
 *            prefix - the prefix of the word
 */
void predictHelper(TSTNode* root, string prefix, my_pri_queue& p_que) {
    if (root == nullptr) {
        return;
    }
    if (root->isEnd) {
        string str = prefix + root->data;
        p_que.push(Word(str, root->fq));
    }
    if (root->lChild != nullptr) {
        predictHelper(root->lChild, prefix, p_que);
    }
    if (root->mChild != nullptr) {
        predictHelper(root->mChild, (prefix + root->data), p_que);
    }
    if (root->rChild != nullptr) {
        predictHelper(root->rChild, prefix, p_que);
    }
}

/** For test
 * void traverse(TSTNode* root) {
    if (root->lChild != nullptr) {
        cout << "------" << root->lChild->data;
        traverse(root->lChild);
    }
    if (root->mChild != nullptr) {
        cout << "-----" << root->mChild->data;
        traverse(root->mChild);
    }
    if (root->rChild != nullptr) {
        cout << "------" << root->rChild->data << endl;
        traverse(root->rChild);
    }
}
*/