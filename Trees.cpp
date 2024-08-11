#include "comp.cpp"
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

class NoisyRedBlackTree {
private:
    bool version2;
    int siz, tim;
    Node *root, *neginf, *posinf;

    Node *get_new_leaf(Node *par) {
        Node* neww = new Node("", 0);
        neww->parent = par;
        neww->color = BLACK;
        return neww;
    }

    bool is_leaf(Node *nod) {
        return nod->left == nullptr;
    }

    void preOrderHelper(Node *node) {
        if (!is_leaf(node)) {
            cout << node->key << " : " << node->value << endl;
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    void inOrderHelper(Node *node) {
        if (!is_leaf(node)) {
            inOrderHelper(node->left);
            cout << node->key << " : " << node->value << endl;
            inOrderHelper(node->right);
        }
    }

    void postOrderHelper(Node *node) {
        if (!is_leaf(node)) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            cout << node->key << " : " << node->value << endl;
        }
    }

    Node* get_l(Node* nod) {
        if (is_leaf(nod)) {
            if (nod->parent->right == nod)
                return nod->parent;
            else
                return nod->parent->most_left;
        }
        return nod->most_left;
    }

    Node *get_r(Node *nod) {
        if (is_leaf(nod))
        {
            if (nod->parent->left == nod)
                return nod->parent;
            else
                return nod->parent->most_right;
        }
        return nod->most_right;
    }

    void macro_step(Node *&v, Node *key) {
        if (v->ctim != tim)
        {
            v->c = 1, v->ctim = tim;
            refresh_r_and_l(v);
        }
        if (v->c == 1) {
            // now check if in range
            if (my_comp(get_l(v), key) < 0 and my_comp(key, get_r(v)) < 0) {
                if (is_leaf(v))
                    v->c++;
                else {
                    int res = my_comp(key, v);
                    if (res == -1)
                        v = v->left;
                    else if (res == 1)
                        v = v->right;
                    else if (res == 0)
                        v->c++;
                }
            } else {
                v = v->parent;
            }
        } else {
            if (is_leaf(v)) {
                if (my_comp(get_l(v), key) < 0 and my_comp(key, get_r(v)) < 0)
                    v->c++;
                else
                    v->c--;
            } else {
                if (my_comp(v, key) == 0)
                    v->c++;
                else
                    v->c--;
            }
        }
    }

    Node* searchTreeHelper(Node *node, Node *key) {
        tim++;
        int num_steps = 3 * (ceil(log2((double)siz + 1.0) + 3));
        while (num_steps--)
            macro_step(node, key);

        if (is_leaf(node) and version2) {
            if (super_comp(get_l(node), key) < 0 and super_comp(key, get_r(node)) < 0) 
                return node;
            else
                return searchTreeHelper(this->root, key);
        } else if (version2) {
            if (super_comp(node, key) == 0)
                return node;
            else
                return searchTreeHelper(this->root, key);
        }
        
        return node;
    }

    void balanceInsert(Node *k) {
        Node *u;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;

                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK;
    }

    void leftRotate(Node *x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;

        refresh_r_and_l(y);
        refresh_r_and_l(x);
    }

    void rightRotate(Node *x) {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;

        refresh_r_and_l(y);
        refresh_r_and_l(x);
    }

    void refresh_r_and_l(Node *x) {
        if (x == root) {
            x->most_left = neginf;
            x->most_right = posinf;
        } else {
            if (x->parent->right == x) {
                x->most_left = x->parent;
                x->most_right = x->parent->most_right;
            } else {
                x->most_left = x->parent->most_left;
                x->most_right = x->parent;
            }
        }
    }

public:
    NoisyRedBlackTree() {
        
        siz = 0;
        tim = 0;
        root = get_new_leaf(nullptr);
        neginf = new Node("I AM LOWEST", 0);
        neginf->left = neginf;
        posinf = new Node("I AM HIGHEST", 0);
        posinf->right = posinf;
    }
    
    void set_mode(bool mod) {
        version2 = mod;
    }

    void preOrder() {
        preOrderHelper(this->root);
    }

    void inOrder() {
        inOrderHelper(this->root);
    }

    void postOrder() {
        postOrderHelper(this->root);
    }

    Node* searchTree(string k) {
        return searchTreeHelper(this->root, new Node(k, 0));
    }

    void set_num_rep(int numr) {
        num_rep = numr;
    }

    Node* insert(string key, int value) {
        Node *node = new Node(key, value);
        node->parent = nullptr;
        node->key = key;
        node->value = value;
        node->left = get_new_leaf(node);
        node->right = get_new_leaf(node);
        node->color = RED;

        if (is_leaf(this->root))
        {
            root = node;
            siz++;
            refresh_r_and_l(root);
            return root;
        }

        Node *y = searchTreeHelper(this->root, node);
        if (!is_leaf(y)) {
            return y;
        } else 
            y = y->parent;
        // here we attach
        
        int outcome = super_comp(node, y);
        siz++;
        node->parent = y;
        if (outcome == -1) {
            y->left = node;
        } else if (outcome == 1) {
            y->right = node;
        }
        
        Node *curr = node;
        refresh_r_and_l(curr);

        if (node->parent == nullptr) {
            node->color = BLACK;
            return node;
        }

        if (node->parent->parent == nullptr) {
            return node;
        }

        balanceInsert(node);
        return node;
    }   
};

class BoostingRedBlackTree {
private:
    Node *root;
    Node *TNULL;

    void initializeNULLNode(Node *node, Node *parent) {
        node->key = "";
        node->value = 0;
        node->color = BLACK;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = parent;
    }

    void preOrderHelper(Node *node) {
        if (node != TNULL) {
            cout << node->key << " : " << node->value << endl;
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    void inOrderHelper(Node *node) {
        if (node != TNULL) {
            inOrderHelper(node->left);
            cout << node->key << " : " << node->value << endl;
            inOrderHelper(node->right);
        }
    }

    void postOrderHelper(Node *node) {
        if (node != TNULL) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            cout << node->key << " : " << node->value << endl;
        }
    }

    Node* searchTreeHelper(Node *node, string key) {
        if (node == TNULL || super_comp(new Node(key, 0), node) == 0) {
            return node;
        }

        if (super_comp(new Node(key, 0), node) == -1) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    void balanceInsert(Node *k) {
        Node *u;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;

                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK;
    }

    void leftRotate(Node *x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *x) {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

public:
    BoostingRedBlackTree() {
        TNULL = new Node("", 0);
        TNULL->color = BLACK;
        root = TNULL;
    }

    void set_num_rep(int numr) {
        num_rep = numr;
    }

    void preOrder() {
        preOrderHelper(this->root);
    }

    void inOrder() {
        inOrderHelper(this->root);
    }

    void postOrder() {
        postOrderHelper(this->root);
    }

    Node* searchTree(string k) {
        return searchTreeHelper(this->root, k);
    }

    Node* insert(string key, int value) {
        Node *sr = searchTree(key);
        if (sr != TNULL) {
            return sr;
        }

        Node *node = new Node(key, value);
        node->parent = nullptr;
        node->key = key;
        node->value = value;
        node->left = TNULL;
        node->right = TNULL;
        node->color = RED;

        Node *y = nullptr;
        Node *x = this->root;

        while (x != TNULL) {
            y = x;
            if (super_comp(node, x) == -1) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->key < y->key) {
            y->left = node;
        } else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = BLACK;
            return node;
        }

        if (node->parent->parent == nullptr) {
            return node;
        }

        balanceInsert(node);
        return node;
    }
};