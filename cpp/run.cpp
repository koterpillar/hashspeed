#include <cstdlib>
#include <map>

using std::map;

struct TreeItem {
    virtual void delete_all(void) = 0;
};
struct TreeLeaf: public TreeItem {
    char value;
    TreeLeaf(char c): value(c) {}
    virtual void delete_all(void) {}
};
struct TreeNode: public TreeItem {
    map<char, TreeItem*> value;
    virtual void delete_all(void) {
        for(auto it = value.begin(); it != value.end(); it++) {
            it->second->delete_all();
            delete it->second;
        }
    }
};

char item(void) {
    return 'a' + (unsigned int)(('z' - 'a' + 1) * rand() / RAND_MAX);
}

unsigned int width(void) {
    return rand() % 10;
}

TreeItem* generate(unsigned int depth) {
    if (depth == 0) {
        return new TreeLeaf(item());
    } else {
        TreeNode* node = new TreeNode();
        unsigned int twidth = width();
        for (unsigned int i = 0; i < twidth; i++) {
            node->value[item()] = generate(depth - 1);
        }
        return node;
    }
}

int main(int argc, char** argv) {
    unsigned int depth = atoi(argv[1]);
    TreeItem* tree = generate(depth);
    tree->delete_all();
    delete tree;
    
    return 0;
}
