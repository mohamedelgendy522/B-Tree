#include <iostream>
#include <stack>
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

    void insert_to_Node(Node<Type, Ord>* node, Type val) 
    {
        int i = node->num_keys - 1;

        while (i>=0 && val < node->key[i])              //detemines the index of the new value and adds it accordingly
        {
            node->key[i + 1] = node->key[i];
            i--;
        }

        node->key[i + 1] = val;
        node->num_keys++;

    }



    void insert(Type val) {
        //Root Case
        if (root == nullptr) {
            LeafNode<Type, Ord>* newnode = new LeafNode<Type, Ord>();  //Create a New Node

            newnode->num_keys = 1;                                  // Enter values and root points to it       
            newnode->key[0] = val;
            root = newnode;
            return;
        }

        Node<Type, Ord>* current = root;

        stack<Node<Type, Ord>*> parents;

        //Traversal
        while (!current->is_leaf) {


            int i = 0;

            while(i<current->num_keys && val > current->key[i])     //which child to go into -_-
            {
                i++;
            }

            InternalNode<Type, Ord>* internal = static_cast<InternalNode<Type, Ord>*>(current);     //get an a pointer of InternalNode type to point at children as current is of Node type

            parents.push(current);                      //Remember every current

            current = internal->child[i];

        }

        insert_to_Node(current, val);

        //Overflow handling
        while (current->num_keys == Ord)
        {
            //Splitting Handling
            int mid = current->num_keys / 2;
            Type promoted = current->key[mid];          //select promoted key


            Node<Type, Ord>* right;                     //initialize the right node based on the type of left node "current"
            if (current->is_leaf) {
                right = new LeafNode<Type, Ord>();
            }
            else {
                right = new InternalNode<Type, Ord>();
            }

            right->num_keys = current->num_keys - mid - 1;            //Enter values to right node

            for (int i = 0; i < right->num_keys; i++)
            {
                right->key[i] = current->key[mid + 1 + i];
            }

            current->num_keys = mid;                                  //modify left node size "current"


            if (!current->is_leaf) {                                  // Internal Nodes Child pointers handling for splitting

                InternalNode<Type, Ord>* newNode = static_cast<InternalNode<Type, Ord>*>(right);
                InternalNode<Type, Ord>* oldNode = static_cast<InternalNode<Type, Ord>*>(current);

                for (int i = 0; i <= right->num_keys; i++)              // oldNode is current which is the left node , newNode is right which is the right node
                {
                    newNode->child[i] = oldNode->child[mid + 1 + i];
                    oldNode->child[mid + 1 + i] = nullptr;
                }
            }

            //Parent Pointers handling
            if (parents.empty())      //root case
            {
                InternalNode<Type, Ord>* newroot = new InternalNode<Type, Ord>();
                newroot->num_keys = 1;
                newroot->key[0] = promoted;
                newroot->child[0] = current;
                newroot->child[1] = right;
                root = newroot;
                return;
            }

            Node<Type, Ord>* parent = parents.top();                                                //get new parent from stack and cast
            parents.pop();
            InternalNode<Type, Ord>* newparent = static_cast<InternalNode<Type, Ord>*>(parent);

            int i = 0;                                                                          //find where promoted is supposed to go
            while (i < parent->num_keys && parent->key[i] < promoted)
            {
                i++;
            }

            int j = parent->num_keys;                                                           //right shift everychild after promoted location
             while (j > i) 
             {
                newparent->child[j + 1] = newparent->child[j];
                j--;
                    
             }

             insert_to_Node(parent, promoted);                                                //insert promoted and introduce it's new child "right"              
             newparent->child[j + 1] = right;

             current = parent;                                                                // check parent for overflowing
        }

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
        for (int i = 0; i < root->num_keys; i++) {
            cout << root->key[i] << " ";
        }
        for (int i = 0; i <= root->num_keys; i++) {
            display(root->child[i]);
        }
    }


};

int main() {
}
