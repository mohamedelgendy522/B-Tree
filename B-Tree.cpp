#include <iostream>
#include <stack>
#include <vector>
using namespace std;

template<typename Type, int Ord>
struct Node
{
    bool is_leaf;
    int num_keys;
    Type key[Ord];

    Node(bool leaf)
    {
        is_leaf = leaf;
        num_keys = 0;
    }
};

template<typename Type, int Ord>
struct InternalNode : public Node<Type, Ord>
{
    Node<Type, Ord> *child[Ord + 1];

    InternalNode() : Node<Type, Ord>(false)
    {
        for (int i = 0; i <= Ord; i++)
        {
            child[i] = nullptr;
        }
    }
};

template<typename Type, int Ord>
struct LeafNode : public Node<Type, Ord>
{
    LeafNode() : Node<Type, Ord>(true)
    {
    }
};

template<typename Type, int Ord>
struct BTree
{
    Node<Type, Ord> *root;

    BTree()
    {
        root = nullptr;
    }

    void insert_to_Node(Node<Type, Ord> *node, Type val)
    {
        int i = node->num_keys - 1;

        //determines the index of the new value and adds it accordingly
        while (i >= 0)
        {
            if (val >= node->key[i]) break;
            node->key[i + 1] = node->key[i];
            i--;
        }

        node->key[i + 1] = val;
        node->num_keys++;
    }

    int findKeyIndex(Node<Type, Ord> *node, Type val)
    {
        // find index key in array of keys
        int low = 0;
        int high = node->num_keys - 1;

        while (low <= high)
        {
            int mid = low + (high - low) / 2;

            if (node->key[mid] == val)
            {
                return mid;
            }
            if (node->key[mid] < val)
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }

        return -1;
    }


    void deleteFromNode(Node<Type, Ord> *node, int keyIndex)
    {
        // shifting
        for (int i = keyIndex; i < node->num_keys - 1; i++)
        {
            node->key[i] = node->key[i + 1];
        }

        node->num_keys--;
    }

    void findParents(Type val, stack<Node<Type, Ord> *> &parents)
    {
        Node<Type, Ord> *target = search(val);
        Node<Type, Ord> *current = root;

        while (current != nullptr)
        {
            if (current == target || current->is_leaf) break;

            int i = 0;

            while (i < current->num_keys)
            {
                if (val <= current->key[i]) break;
                i++;
            }

            parents.push(current);

            InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(current);

            current = internal->child[i];
        }
    }

    int findChildIndex(Node<Type, Ord> *parent, Node<Type, Ord> *current)
    {
        InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(parent);

        for (int i = 0; i <= parent->num_keys; i++)
        {
            if (internal->child[i] == current)
            {
                return i;
            }
        }

        return -1;
    }

    Node<Type, Ord> *findPredecessor(Node<Type, Ord> *current, int keyIndex, stack<Node<Type, Ord> *> &parents)
    {
        InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(current);

        Node<Type, Ord> *predNode = internal->child[keyIndex];
        parents.push(current);

        while (!predNode->is_leaf)
        {
            parents.push(predNode);

            InternalNode<Type, Ord> *predInternal = static_cast<InternalNode<Type, Ord> *>(predNode);

            predNode = predInternal->child[predNode->num_keys];
        }

        return predNode;
    }

    Node<Type, Ord> *findSuccessor(Node<Type, Ord> *current, int keyIndex, stack<Node<Type, Ord> *> &parents)
    {
        InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(current);

        Node<Type, Ord> *succNode = internal->child[keyIndex + 1];
        parents.push(current);

        while (!succNode->is_leaf)
        {
            parents.push(succNode);

            InternalNode<Type, Ord> *succInternal = static_cast<InternalNode<Type, Ord> *>(succNode);

            succNode = succInternal->child[0];
        }

        return succNode;
    }

    void insert(Type val)
    {
        //Root Case
        if (root == nullptr)
        {
            LeafNode<Type, Ord> *newnode = new LeafNode<Type, Ord>(); //Create a New Node

            newnode->num_keys = 1; // Enter values and root points to it
            newnode->key[0] = val;
            root = newnode;
            return;
        }

        Node<Type, Ord> *current = root;

        stack<Node<Type, Ord> *> parents;

        //Traversal
        while (!current->is_leaf)
        {
            int i = 0;

            //which child to go into -_-
            while (i < current->num_keys)
            {
                if (val <= current->key[i]) break;
                i++;
            }

            InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(current);
            //get an a pointer of InternalNode type to point at children as current is of Node type

            parents.push(current); //Remember every current

            current = internal->child[i];
        }

        insert_to_Node(current, val);

        //Overflow handling
        while (current->num_keys == Ord)
        {
            //Splitting Handling
            int mid = current->num_keys / 2;
            Type promoted = current->key[mid]; //select promoted key


            Node<Type, Ord> *right; //initialize the right node based on the type of left node "current"
            if (current->is_leaf)
            {
                right = new LeafNode<Type, Ord>();
            }
            else
            {
                right = new InternalNode<Type, Ord>();
            }

            right->num_keys = current->num_keys - mid - 1; //Enter values to right node

            for (int i = 0; i < right->num_keys; i++)
            {
                right->key[i] = current->key[mid + 1 + i];
            }

            current->num_keys = mid; //modify left node size "current"


            if (!current->is_leaf)
            {
                // Internal Nodes Child pointers handling for splitting

                InternalNode<Type, Ord> *newNode = static_cast<InternalNode<Type, Ord> *>(right);
                InternalNode<Type, Ord> *oldNode = static_cast<InternalNode<Type, Ord> *>(current);

                // oldNode is current which is the left node , newNode is right which is the right node
                for (int i = 0; i <= right->num_keys; i++)
                {
                    newNode->child[i] = oldNode->child[mid + 1 + i];
                    oldNode->child[mid + 1 + i] = nullptr;
                }
            }

            //Parent Pointers handling
            if (parents.empty()) //root case
            {
                InternalNode<Type, Ord> *newroot = new InternalNode<Type, Ord>();
                newroot->num_keys = 1;
                newroot->key[0] = promoted;
                newroot->child[0] = current;
                newroot->child[1] = right;
                root = newroot;
                return;
            }

            Node<Type, Ord> *parent = parents.top(); //get new parent from stack and cast
            parents.pop();
            InternalNode<Type, Ord> *newparent = static_cast<InternalNode<Type, Ord> *>(parent);

            int i = 0; //find where promoted is supposed to go
            while (i < parent->num_keys)
            {
                if (parent->key[i] >= promoted) break;
                i++;
            }

            int j = parent->num_keys; //right shift everychild after promoted location
            while (j > i)
            {
                newparent->child[j + 1] = newparent->child[j];
                j--;
            }

            insert_to_Node(parent, promoted); //insert promoted and introduce it's new child "right"
            newparent->child[j + 1] = right;

            current = parent; // check parent for overflowing
        }
    }

    void remove(Type val)
    {
        if (root == nullptr)
        {
            cout << "Empty Tree" << endl;
        }
        else
        {
            Node<Type, Ord> *current = search(val);

            if (current == nullptr)
            {
                return;
            }

            stack<Node<Type, Ord> *> parents;
            findParents(val, parents);

            int keyIndex = findKeyIndex(current, val);
            int minKeys = (Ord + 1) / 2 - 1;

            if (keyIndex == -1)
                return;

            if (!current->is_leaf)
            {
                Node<Type, Ord> *predNode = findPredecessor(current, keyIndex, parents);

                Type predVal = predNode->key[predNode->num_keys - 1];

                current->key[keyIndex] = predVal;

                current = predNode;
                keyIndex = current->num_keys - 1;
            }

            if (current->is_leaf)
            {
                // Case 1: Remove without Underflow
                if (current->num_keys - 1 >= minKeys)
                {
                    deleteFromNode(current, keyIndex);
                }

                // Case 2: Remove with Underflow
                else
                {
                    deleteFromNode(current, keyIndex);

                    while (current != root)
                    {
                        if (current->num_keys >= minKeys) break;

                        Node<Type, Ord> *parent = nullptr;

                        if (!parents.empty())
                        {
                            parent = parents.top();
                            parents.pop();
                        }

                        InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(parent);

                        int childIndex = findChildIndex(internal, current);

                        Node<Type, Ord> *Rightsibling = nullptr;
                        Node<Type, Ord> *Leftsibling = nullptr;

                        if (childIndex < parent->num_keys)
                        {
                            Rightsibling = internal->child[childIndex + 1];
                        }

                        if (childIndex > 0)
                        {
                            Leftsibling = internal->child[childIndex - 1];
                        }

                        if (current->is_leaf)
                        {
                            //  Borrow from Right
                            if (Rightsibling != nullptr && Rightsibling->num_keys - 1 >= minKeys)
                            {
                                current->key[current->num_keys] = parent->key[childIndex];

                                current->num_keys++;

                                parent->key[childIndex] = Rightsibling->key[0];

                                deleteFromNode(Rightsibling, 0);

                                break;
                            }

                            //  Borrow from Left
                            else if (Leftsibling != nullptr && Leftsibling->num_keys - 1 >= minKeys)
                            {
                                for (int i = current->num_keys; i > 0; i--)
                                {
                                    current->key[i] = current->key[i - 1];
                                }

                                current->key[0] = parent->key[childIndex - 1];
                                current->num_keys++;

                                parent->key[childIndex - 1] = Leftsibling->key[Leftsibling->num_keys - 1];

                                deleteFromNode(Leftsibling, Leftsibling->num_keys - 1);

                                break;
                            }

                            // Merge with Right Sibling
                            else if (Rightsibling != nullptr)
                            {
                                current->key[current->num_keys] = parent->key[childIndex];

                                current->num_keys++;

                                for (int i = 0; i < Rightsibling->num_keys; i++)
                                {
                                    current->key[current->num_keys] = Rightsibling->key[i];
                                    current->num_keys++;
                                }

                                deleteFromNode(internal, childIndex);

                                for (int i = childIndex + 1; i < internal->num_keys + 1; i++)
                                {
                                    internal->child[i] = internal->child[i + 1];
                                }

                                internal->child[internal->num_keys + 1] = nullptr;

                                delete Rightsibling;
                            }

                            // Merge with Left Sibling
                            else if (Leftsibling != nullptr)
                            {
                                for (int i = current->num_keys; i > 0; i--)
                                {
                                    current->key[i] = current->key[i - 1];
                                }

                                current->key[0] = parent->key[childIndex - 1];
                                current->num_keys++;

                                for (int i = current->num_keys - 1; i >= 0; i--)
                                {
                                    current->key[i + Leftsibling->num_keys] = current->key[i];
                                }

                                for (int i = 0; i < Leftsibling->num_keys; i++)
                                {
                                    current->key[i] = Leftsibling->key[i];
                                }

                                current->num_keys += Leftsibling->num_keys;

                                deleteFromNode(internal, childIndex - 1);

                                for (int i = childIndex - 1; i < internal->num_keys + 1; i++)
                                {
                                    internal->child[i] = internal->child[i + 1];
                                }

                                internal->child[internal->num_keys + 1] = nullptr;

                                delete Leftsibling;
                            }
                        }
                        else
                        {
                            InternalNode<Type, Ord> *currInt = static_cast<InternalNode<Type, Ord> *>(current);
                            InternalNode<Type, Ord> *rightInt = static_cast<InternalNode<Type, Ord> *>(Rightsibling);
                            InternalNode<Type, Ord> *leftInt = static_cast<InternalNode<Type, Ord> *>(Leftsibling);

                            if (Rightsibling != nullptr && Rightsibling->num_keys - 1 >= minKeys)
                            {
                                current->key[current->num_keys] = parent->key[childIndex];
                                current->num_keys++;
                                currInt->child[current->num_keys] = rightInt->child[0];
                                parent->key[childIndex] = Rightsibling->key[0];
                                deleteFromNode(Rightsibling, 0);
                                for (int i = 0; i <= Rightsibling->num_keys; i++)
                                {
                                    rightInt->child[i] = rightInt->child[i + 1];
                                }

                                rightInt->child[Rightsibling->num_keys + 1] = nullptr;
                                break;
                            }

                            // Try Borrow from Left
                            else if (Leftsibling != nullptr && Leftsibling->num_keys - 1 >= minKeys)
                            {
                                for (int i = current->num_keys; i > 0; i--)
                                {
                                    current->key[i] = current->key[i - 1];
                                }

                                current->key[0] = parent->key[childIndex - 1];
                                current->num_keys++;


                                for (int i = current->num_keys; i > 0; i--)
                                {
                                    currInt->child[i] = currInt->child[i - 1];
                                }
                                currInt->child[0] = leftInt->child[Leftsibling->num_keys];
                                parent->key[childIndex - 1] = Leftsibling->key[Leftsibling->num_keys - 1];

                                deleteFromNode(Leftsibling, Leftsibling->num_keys - 1);


                                leftInt->child[Leftsibling->num_keys + 1] = nullptr;
                                break;
                            }

                            // Merge with Right Sibling
                            else if (Rightsibling != nullptr)
                            {
                                current->key[current->num_keys] = parent->key[childIndex];
                                current->num_keys++;

                                for (int i = 0; i < Rightsibling->num_keys; i++)
                                {
                                    current->key[current->num_keys] = Rightsibling->key[i];
                                    current->num_keys++;
                                }

                                deleteFromNode(parent, childIndex);

                                for (int i = childIndex + 1; i < internal->num_keys + 1; i++)
                                {
                                    internal->child[i] = internal->child[i + 1];
                                }

                                for (int i = 0; i <= Rightsibling->num_keys; i++)
                                {
                                    currInt->child[current->num_keys - Rightsibling->num_keys + i] = rightInt->child[i];
                                }

                                internal->child[internal->num_keys + 1] = nullptr;

                                delete Rightsibling;
                            }

                            // Merge with Left Sibling
                            else if (Leftsibling != nullptr)
                            {

                                int shift = Leftsibling->num_keys + 1;

                                for (int i = current->num_keys - 1; i >= 0; i--)
                                {
                                    current->key[i + shift] = current->key[i];
                                }

                                for (int i = current->num_keys; i >= 0; i--)
                                {
                                    currInt->child[i + shift] = currInt->child[i];
                                }

                                for (int i = 0; i < Leftsibling->num_keys; i++)
                                {
                                    current->key[i] = Leftsibling->key[i];
                                }

                                current->key[Leftsibling->num_keys] = parent->key[childIndex - 1];

                                for (int i = 0; i <= Leftsibling->num_keys; i++)
                                {
                                    currInt->child[i] = leftInt->child[i];
                                }

                                current->num_keys += Leftsibling->num_keys + 1;

                                deleteFromNode(parent, childIndex - 1);

                                for (int i = childIndex - 1; i < internal->num_keys + 1; i++)
                                {
                                    internal->child[i] = internal->child[i + 1];
                                }

                                internal->child[internal->num_keys + 1] = nullptr;

                                delete Leftsibling;
                            }
                        }

                        current = parent;
                    }
                }
                if (root->num_keys == 0)
                {
                    if (root->is_leaf)
                    {
                        delete root;
                        root = nullptr;
                    }
                    else
                    {
                        InternalNode<Type, Ord> *oldRoot = static_cast<InternalNode<Type, Ord> *>(root);
                        root = oldRoot->child[0];
                        delete oldRoot;
                    }
                }
            }
        }
    }

    Node<Type, Ord> *search(Type val)
    {
        Node<Type, Ord> *current = root;

        while (current != nullptr)
        {
            // find the first key >= val
            int i = 0;
            while (i < current->num_keys)
            {
                if (val <= current->key[i]) break;
                i++;
            }

            // found it in this node
            if (i < current->num_keys && current->key[i] == val)
            {
                return current;
            }

            // reached a leaf without finding it
            if (current->is_leaf)
            {
                return nullptr;
            }

            // otherwise descend into child i
            InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(current);
            current = internal->child[i];
        }

        return nullptr; // empty tree
    }

    void display()
    {
        // Pre-order (node first, then children left to right)
        if (root == nullptr)
        {
            cout << "Empty Tree" << endl;
            return;
        }
        displayNode(root);
        cout << endl;
    }

    void displayNode(Node<Type, Ord> *node)
    {
        if (node == nullptr) return;

        for (int i = 0; i < node->num_keys; i++)
        {
            cout << node->key[i] << " ";
        }

        if (!node->is_leaf)
        {
            InternalNode<Type, Ord> *internal = static_cast<InternalNode<Type, Ord> *>(node);
            for (int i = 0; i <= node->num_keys; i++)
            {
                displayNode(internal->child[i]);
            }
        }
    }
};

int main()
{

}
