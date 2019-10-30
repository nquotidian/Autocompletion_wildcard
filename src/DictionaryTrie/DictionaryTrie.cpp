/**
 * File Header
 *
 * Author: Qing Niu, the implementino of the header file DictionaryTrie.hpp
 * predictCompletions enlightened by Xavier, which stores the max frequency of
 * all of the subtree node in the CSE100 disscussion session
 * Part of the algorithm come from the slides of disscussion 5.
 */
#include "DictionaryTrie.hpp"
#include <algorithm>
#include <iostream>

// Maintain a priority queue of size numCompletions
void my_queue_push(Word word, my_pri_queue& r_que, int num);
// Convert the result form the queue to a vector
vector<string> convert_queue_to_vector(my_pri_queue& r_que, int num);
// Backtracking help function
void backtrack(string pre, string suff, TSTNode* curr, unsigned pos,
               vector<Word>& results);
// Traverse method for debug
// void traverse(TSTNode* root);

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
        root = new TSTNode(word[0], freq);
        TSTNode* curr = root;
        for (unsigned int i = 1; i < word.size(); i++) {
            node = new TSTNode(word[i], freq);
            curr->mChild = node;
            curr = node;
        }
        // the end node
        curr->isEnd = true;
        return true;
    } else {
        TSTNode* curr = root;
        unsigned int i = 0;
        while (true) {
            if (word[i] < curr->data) {
                if (curr->max < freq) {
                    curr->max = freq;
                }
                if (curr->lChild == nullptr) {
                    curr->lChild = new TSTNode(word[i], freq);
                    curr = curr->lChild;
                    for (i = i + 1; i < word.size(); i++) {
                        curr->mChild = new TSTNode(word[i], freq);
                        curr = curr->mChild;
                    }
                    curr->isEnd = true;
                    return true;
                } else {
                    curr = curr->lChild;
                }
            } else if (word[i] > curr->data) {
                if (curr->max < freq) {
                    curr->max = freq;
                }
                if (curr->rChild == nullptr) {
                    curr->rChild = new TSTNode(word[i], freq);
                    curr = curr->rChild;

                    for (i = i + 1; i < word.size(); i++) {
                        curr->mChild = new TSTNode(word[i], freq);
                        curr = curr->mChild;
                    }
                    curr->isEnd = true;
                    // curr->fq = freq;
                    // curr->max = freq;
                    return true;
                } else {
                    // curr->max = (curr->max < freq) ? freq : curr->max;
                    curr = curr->rChild;
                }
            } else {
                if (curr->max < freq) {
                    curr->max = freq;
                }
                if (i == word.size() - 1) {
                    curr->isEnd = true;
                    curr->fq = freq;
                    // curr->max = freq;
                    return true;
                } else {
                    if (curr->mChild != nullptr) {
                        curr->max = (curr->max < freq) ? freq : curr->max;
                        curr = curr->mChild;
                        i++;
                    } else {
                        for (i = i + 1; i < word.size(); i++) {
                            curr->mChild = new TSTNode(word[i], freq);
                            curr = curr->mChild;
                        }
                        curr->isEnd = true;
                        // curr->fq = freq;
                        // curr->max = freq;
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
    TSTNode* curr = find_last_char_node(word);
    // find the word
    if (curr == nullptr) {
        return false;
    } else if (curr->isEnd) {
        return true;
    } else {
        return false;
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
    // Create a priority queue with ascending order of priority
    my_pri_queue asd_que(my_comp(true));
    // if prefix is also a word
    if (curr->isEnd) {
        asd_que.push(Word(prefix, curr->fq));
    }
    // If node has no middle child, return directly
    if (curr->mChild != nullptr) {
        curr = curr->mChild;
    } else {
        if (numCompletions > 0) {
            rt_vec.push_back(prefix);
        }
        return rt_vec;
    }
    unsigned max = curr->max;
    predictHelper(curr, prefix, asd_que, numCompletions);
    // Get all the first n words in the queue
    rt_vec = convert_queue_to_vector(asd_que, numCompletions);
    return rt_vec;
}

/* Predict the underscore words */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    vector<string> result;
    vector<Word> w_vec;
    // Find the pre-underscore part
    TSTNode* curr = nullptr;
    string pre, suff;
    char un_s = '_';
    // Whether the pattern starts with "_"
    if (pattern.front() == un_s) {
        // front is the underscore
        curr = root;
        backtrack("", pattern, curr, 0, w_vec);
    } else {
        pre = pattern.substr(0, pattern.find("_"));
        suff = pattern.substr(pattern.find("_"));
        curr = find_last_char_node(pre);
        // Pre doesn't exist in the dict
        if (curr == nullptr) {
            return result;
        } else {
            curr = find_last_char_node(pre);
            backtrack(pre, suff, curr->mChild, 0, w_vec);
        }
    }
    std::sort(w_vec.begin(), w_vec.end(), compareObj);
    int n = ((w_vec.size() < numCompletions) ? w_vec.size() : numCompletions);
    for (int j = 0; j < n; j++) {
        result.push_back(w_vec[j].word);
    }
    return result;
}
/* Find the last char node of prefix or word */
TSTNode* DictionaryTrie::find_last_char_node(string prefix) const {
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
void DictionaryTrie::predictHelper(TSTNode* root, string prefix,
                                   my_pri_queue& r_que, int num) {
    // root is null
    if (root == nullptr) {
        return;
    }
    // Pri queue is not full
    if (r_que.size() < num) {
        if (root->isEnd) {
            string str = prefix + root->data;
            my_queue_push(Word(str, root->fq), r_que, num);
        }
        if (root->lChild != nullptr) {
            predictHelper(root->lChild, prefix, r_que, num);
        }
        if (root->mChild != nullptr) {
            predictHelper(root->mChild, (prefix + root->data), r_que, num);
        }
        if (root->rChild != nullptr) {
            predictHelper(root->rChild, prefix, r_que, num);
        }
    } else {
        // Queue is full
        // cout << "size -----" << r_que.size() << endl;
        if (root->isEnd) {
            string str = prefix + root->data;
            my_queue_push(Word(str, root->fq), r_que, num);
            // cout << "str---" << str << endl;
        }
        if (root->lChild != nullptr) {
            // cout << "node--" << root->lChild->data << "--max---"
            //      << root->lChild->max << endl;
            // cout << "l fq---" << r_que.top().fq << endl;
            if (!r_que.empty()) {
                if (r_que.top().fq <= root->lChild->max) {
                    predictHelper(root->lChild, prefix, r_que, num);
                }
            }
        }
        if (root->mChild != nullptr) {
            // cout << "root->m->max---" << root->mChild->max << endl;
            // cout << "node--" << root->mChild->data << "--max---"
            //      << root->mChild->max << endl;
            // cout << "m fq---" << r_que.top().fq << endl;
            if (!r_que.empty()) {
                if (r_que.top().fq <= root->mChild->max) {
                    predictHelper(root->mChild, (prefix + root->data), r_que,
                                  num);
                }
            }
        }
        if (root->rChild != nullptr) {
            // cout << "root->r->max---" << root->rChild->max << endl;
            // cout << "node--" << root->rChild->data << "--max---"
            //      << root->rChild->max << endl;
            // cout << "r fq---" << r_que.top().fq << endl;
            if (!r_que.empty()) {
                if (r_que.top().fq <= root->rChild->max) {
                    predictHelper(root->rChild, prefix, r_que, num);
                }
            }
        }
    }
}

/* Maintain a ascending priority queue
 * Parameter: word - the word in the TST node
 *            r_que - the ascending priority queue
 * */
void my_queue_push(Word word, my_pri_queue& r_que, int num) {
    // The queue is not full
    if (r_que.size() < num) {
        r_que.push(word);
    } else if (!r_que.empty()) {
        if (word.fq > r_que.top().fq ||
            (word.fq == r_que.top().fq && word.word < r_que.top().word)) {
            // The queue is full, kick out the top one with smallest
            // priority
            r_que.pop();
            r_que.push(word);
        }
    }
}

/* Convert the results from queue to a vector
 * Parameter: r_que - the queue storing the results
 *            num - num of autocompletions
 * */
vector<string> convert_queue_to_vector(my_pri_queue& r_que, int num) {
    int n = ((r_que.size() < num) ? r_que.size() : num);
    string str;
    my_pri_queue que;
    vector<string> vec;
    int size = r_que.size();
    // Convert the ascending priority queue to a normal priority queue
    for (int i = 0; i < size; i++) {
        if (!r_que.empty()) {
            Word wd = r_que.top();
            que.push(wd);
            r_que.pop();
        }
    }
    // Get all of the words found
    for (int j = 0; j < n; j++) {
        if (!que.empty()) {
            str = que.top().word;
            vec.push_back(str);
            que.pop();
        }
    }
    return vec;
}

/* Help function for predictUnderscore
 * Parameter: pre - prefix before first '_'
 *            suff - suffix after first '_'
 *            pos - current position of suffix
 *            results - to store results
 * */
void backtrack(string pre, string suff, TSTNode* curr, unsigned pos,
               vector<Word>& results) {
    // First got the last node of the prefix node
    // then recursively push all of the nodes
    // if the last node of the
    if (curr == nullptr) {
        return;
    }
    // Deal with the underscore
    if (pos == suff.size() - 1) {
        // The end node is valid
        if (curr->isEnd && (suff[pos] == curr->data || suff[pos] == '_')) {
            string str = pre + curr->data;
            results.push_back(Word(str, curr->fq));
        }
        // To right
        if (curr->rChild != nullptr) {
            backtrack(pre, suff, curr->rChild, pos, results);
        }
        // To left
        if (curr->lChild != nullptr) {
            backtrack(pre, suff, curr->lChild, pos, results);
        }
    } else {
        if (suff[pos] == '_') {
            if (curr->mChild != nullptr) {
                backtrack(pre + curr->data, suff, curr->mChild, pos + 1,
                          results);
            }
            if (curr->lChild != nullptr) {
                backtrack(pre, suff, curr->lChild, pos, results);
            }
            if (curr->rChild != nullptr) {
                backtrack(pre, suff, curr->rChild, pos, results);
            }
        } else if (suff[pos] == curr->data) {
            if (curr->mChild != nullptr)
                backtrack(pre + curr->data, suff, curr->mChild, pos + 1,
                          results);
        } else if (suff[pos] != curr->data) {
            if (curr->lChild != nullptr) {
                backtrack(pre, suff, curr->lChild, pos, results);
            }
            if (curr->rChild != nullptr) {
                backtrack(pre, suff, curr->rChild, pos, results);
            }
        }
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