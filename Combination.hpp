#pragma once
#include <unordered_map>

class Combination
{
private:
    struct Node
    {
        Node *father = this;
    };

    Node *findRoot(Node *node)
    {
        if (node->father==node) return node;
        else node->father = findRoot(node->father);
        return node->father;
    }

    std::unordered_map<int, Node> storage;

public:
    void merge(int x, int y)
    {
        findRoot(&storage[x])->father=findRoot(&storage[y]);
    }
    bool ask(int x, int y)
    {
        return findRoot(&storage[x])==findRoot(&storage[y]);
    }
};