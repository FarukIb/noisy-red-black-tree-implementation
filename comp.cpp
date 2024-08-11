#include "Node.cpp"
#include <algorithm>

int num_rep = 11;
int num_query = 0;

int get_num_query() {
    return num_query;
}

void reset_num_query() {
    num_query = 0;
}

int comp(Node *a, Node *b) {
    num_query++;
    if (a == nullptr || b == nullptr)
        return rand() % 3 - 1;
    
    int out;
    if (a->key < b->key)
        out = -1;
    else if (a->key > b->key)
        out = 1;
    else
        out = 0;
    
    int random_numer = rand() % 10;
    if (random_numer == 0) { 
        int rand2 = (rand() % 2) + 1;
        if (out == -1)
            return out + rand2;
        else if (out == 1)
            return out - rand2;
        else
            return out + ((rand2 - 1) * 2 - 1);
    }
    return out;
}

int my_comp(Node *a, Node *b) {
    if (a->left == a || b->right == b)
        return -1;
    if (a->right == a || b->left == b)
        return 1;
    if (a == b)
        return 0;

    // here the actual comparison of strings needs to be inserted
    return comp(a, b);
}

int super_comp(Node *a, Node *b) {
    int guy[3] = {0, 0, 0};
    for (int i = 0; i < num_rep; i++)   
        guy[my_comp(a, b) + 1]++;
    return std::max_element(guy, guy + 3) - guy - 1;
}
