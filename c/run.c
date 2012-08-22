#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_ITEM 'a'
#define MAX_ITEM 'z'

unsigned int rnd_state = 3;
unsigned int myrandom() {
    rnd_state = (rnd_state + 1) % 1001;
    return rnd_state;
}

char item(void) {
    return MIN_ITEM + myrandom() % (MAX_ITEM - MIN_ITEM + 1);
}

unsigned int width(void) {
    return myrandom() % 8 + 1;
}

typedef struct _MapItem {
    char key;
    void* value;
} MapItem;

typedef struct _Map {
    int length;
    MapItem* items;
} Map;

typedef struct _TreeItem {
    char leaf;
    Map* map;
} TreeItem, *Tree;

int hash(char c) {
    return (int)c;
}

Map* map_new_sz(int size) {
    Map* map = malloc(sizeof(Map));
    map->length = size;
    map->items = calloc(sizeof(MapItem), map->length);
}

Map* map_new() {
    return map_new_sz(16);
}

void map_insert(Map* map, char key, void* value);

void map_grow(Map* map) {
    Map* new_map = map_new_sz(map->length * 2);
    int i;
    for (i = 0; i < map->length; i++) {
        if (map->items[i].key) {
            map_insert(new_map, map->items[i].key, map->items[i].value);
        }
    }
    free(map->items);
    map->length = new_map->length;
    map->items = new_map->items;
    free(new_map);
}

void map_insert(Map* map, char key, void* value) {
    int start = hash(key) % map->length;
    int end = start ? start : map->length;
    int i;
    for (i = start; i != end - 1; i = (i + 1) % map->length) {
        if (map->items[i].value) {
            continue;
        }
        map->items[i].key = key;
        map->items[i].value = value;
        return;
    }
    map_grow(map);
    map_insert(map, key, value);
}

void* map_get(Map* map, char key) {
    if (map->length == 0) {
        return 0;
    } else {
        int start = hash(key) % map->length;
        int end = start ? start : map->length;
        int i;
        for (i = start; i != end - 1; i++) {
            if (key == map->items[i].key && map->items[i].value) {
                return map->items[i].value;
            }
        }
        return 0;
    }
}

void map_free(Map* map) {
    free(map->items);
    free(map);
}

Tree plain_item(char c) {
    Tree it = malloc(sizeof(TreeItem));
    it->leaf = c;
    it->map = 0;
    return it;
}

Tree generate(unsigned int depth) {
    if (depth == 0) {
        return plain_item(item());
    } else {
        Tree node = malloc(sizeof(TreeItem));
        node->map = map_new();

        unsigned int twidth = width();
        unsigned int i;
        for (i = 0; i < twidth; i++) {
            char c = item();
            map_insert(node->map, c, generate(depth - 1));
        }
        return node;
    }
}

void inc_accum(Map* map, char key) {
    int* count = map_get(map, key);
    (*count)++;
}

void count_accum(Tree tree, Map* accum) {
    if (!tree->map) {
        inc_accum(accum, tree->leaf);
    } else {
        int i;
        for (i = 0; i < tree->map->length; i++) {
            MapItem* item = &(tree->map->items[i]);
            if (!item->value) {
                continue;
            }
            inc_accum(accum, item->key);
            count_accum(item->value, accum);
        }
    }
}

Map* count(Tree tree) {
    Map* accum = map_new();
    char c;
    for (c = MIN_ITEM; c <= MAX_ITEM; c++) {
        int* i = malloc(sizeof(int));
        *i = 0;
        map_insert(accum, c, i);
    }
    count_accum(tree, accum);
    return accum;
}

void tree_print_prefix(Tree tree, char* prefix) {
    if (!tree->map) {
        printf("%c\n", tree->leaf);
    } else {
        printf("\n");
        char* subprefix = malloc(sizeof(char) * (strlen(prefix) + 2));
        subprefix[0] = '\t';
        strncpy(subprefix + 1, prefix, strlen(prefix) + 1);
        int i;
        for (i = 0; i < tree->map->length; i++) {
            MapItem* item = &(tree->map->items[i]);
            if (!item->value) {
                continue;
            }
            printf("%s%c -> ", prefix, item->key);
            tree_print_prefix(item->value, subprefix);
        }
        free(subprefix);
    }
}

void tree_print(Tree tree) {
    tree_print_prefix(tree, "");
}

void tree_free(Tree tree) {
    if (tree->map) {
        int i;
        for (i = 0; i < tree->map->length; i++) {
            MapItem* item = &(tree->map->items[i]);
            if (!item->value) {
                continue;
            }
            tree_free(item->value);
        }
        map_free(tree->map);
    }
    free(tree);
}

int main(int argc, char** argv) {
    unsigned int depth = atoi(argv[1]);
    Tree tree = generate(depth);

    if (depth < 4)
        tree_print(tree);

    Map* counts = count(tree);
    int i;
    for (i = 0; i < counts->length; i++) {
        MapItem* item = &(counts->items[i]);
        if (!item->key) {
            continue;
        }
        printf("%c\t%d\n", item->key, *((int*)(item->value)));
        free(item->value);
    }
    map_free(counts);

    tree_free(tree);
    
    return 0;
}
