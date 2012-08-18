#include <iostream>
#include <map>
#include <string>

#define MIN_ITEM 'a'
#define MAX_ITEM 'z'

using std::cout;
using std::endl;
using std::map;
using std::string;

struct TreeItem;
typedef TreeItem* Tree;
struct TreeItem {
    virtual void delete_all(void) = 0;
};
struct TreeLeaf: public TreeItem {
    char value;
    TreeLeaf(char c): value(c) {}
    virtual void delete_all(void) {}
};
struct TreeNode: public TreeItem {
    map<char, Tree> value;
    virtual void delete_all(void) {
        for(auto it = value.begin(); it != value.end(); it++) {
            it->second->delete_all();
            delete it->second;
        }
    }
};

typedef map<char, unsigned int> Counts;

unsigned int rnd_state = 1;
unsigned int myrandom() {
    rnd_state = (rnd_state * 1103515245 + 12345) & 0x7FFFFFFF;
    return rnd_state;
}

char item(void) {
    return MIN_ITEM + myrandom() % (MAX_ITEM - MIN_ITEM + 1);
}

unsigned int width(void) {
    return myrandom() % 9 + 1;
}

Tree generate(unsigned int depth) {
    if (depth == 0) {
        return new TreeLeaf(item());
    } else {
        TreeNode* node = new TreeNode();
        unsigned int twidth = width();
        for (unsigned int i = 0; i < twidth; i++) {
            auto it = item();
            node->value[it] = generate(depth - 1);
        }
        return node;
    }
}

void count_accum(Tree tree, Counts* accum) {
    TreeNode* node = dynamic_cast<TreeNode*>(tree);
    if (node == nullptr) {
        TreeLeaf* leaf = dynamic_cast<TreeLeaf*>(tree);
        (*accum)[leaf->value]++;
    } else {
        for(auto it = node->value.begin(); it != node->value.end(); it++) {
            (*accum)[it->first]++;
            count_accum(it->second, accum);
        }
    }
}

Counts* count(Tree tree) {
    Counts* result = new Counts();
    for (char c = 'a'; c <= 'z'; c++) {
        (*result)[c] = 0;
    }
    count_accum(tree, result);
    return result;
}

void print(Tree tree, string prefix = "") {
    TreeNode* node = dynamic_cast<TreeNode*>(tree);
    if (node == nullptr) {
        TreeLeaf* leaf = dynamic_cast<TreeLeaf*>(tree);
        cout << leaf->value << endl;
    } else {
        cout << endl;
        for(auto it = node->value.begin(); it != node->value.end(); it++) {
            cout << prefix << it->first << " -> ";
            print(it->second, prefix + '\t');
        }
    }
}

int main(int argc, char** argv) {
    unsigned int depth = atoi(argv[1]);
    Tree tree = generate(depth);

    if (depth < 4)
        print(tree);

    auto counts = count(tree);
    for (auto it = counts->begin(); it != counts->end(); it++) {
        cout << it->first << '\t' << it->second << endl;
    }
    delete counts;

    tree->delete_all();
    delete tree;
    
    return 0;
}
