#include <iostream>
using namespace std;

template <typename Type, int Ord>
struct Node
{
    bool is_leaf;
    int num_keys;
    Type key[Ord];

    Node(bool leaf) {
        is_leaf = leaf;
        num_keys = 0;
    }
};

template <typename Type, int Ord>
struct InternalNode : public Node<Type, Ord>
{
    Node<Type, Ord>* child[Ord + 1];

    InternalNode() : Node<Type, Ord>(false) {
        for (int i = 0; i <= Ord; i++) {
            child[i] = nullptr;
        }
    }
};

template <typename Type, int Ord>
struct LeafNode : public Node<Type, Ord>
{
    LeafNode() : Node<Type, Ord>(true) {}
};

template <typename Type, int Ord>
struct BTree
{
    Node<Type, Ord>* root;

    BTree() {
        root = nullptr;
    }


    void insert(Type val) {
        // TODO
    }

    void remove(Type val) {
        // TODO
    }

    Node<Type, Ord>* search(Type val) {
        // TODO
        return nullptr;
    }

    void display( Node<Type, Ord> * root) {  // Pre-order (node first, then children left to right)

        if (root == nullptr) {
            cout << "Empty Tree" << endl;
            return;
        }
        for (int i = 0; i <= root->num_keys; i++) {
            cout << root->key[i] << " ";
        }
        for (int i = 0; i < root->num_keys; i++) {
            display(root->child[i]);
        }
    }


};

int main() {
}
