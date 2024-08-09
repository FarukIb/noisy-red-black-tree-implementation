#include <string>

const bool BLACK = false;
const bool RED = true;

struct Node {
    std::string key;
    int value;
    int c;
    bool color;
    int ctim;
    Node *left, *right, *parent, *most_right, *most_left;

    Node(std::string key, int value) : key(key), value(value), color(RED), left(nullptr), right(nullptr), parent(nullptr), most_left(nullptr), most_right(nullptr), c(1), ctim(0) {}
};