#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;

    Node(int val) : key(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    int height(Node* N) {
        return N ? N->height : 0;
    }

    int getBalance(Node* N) {
        return N ? height(N->left) - height(N->right) : 0;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node* insertNode(Node* node, int key) {
        if (!node) return new Node(key);

        if (key < node->key)
            node->left = insertNode(node->left, key);
        else if (key > node->key)
            node->right = insertNode(node->right, key);
        else
            return node; 

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    Node* deleteNode(Node* root, int key) {
        if (!root) return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;
                delete temp;
            }
            else {
                Node* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }

        if (!root) return root;

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);

        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }


    void getTopKUtil(Node* root, int& k, vector<int>& result) {
        if (!root || k <= 0) return;

        // Traverse right first to get larger elements
        getTopKUtil(root->right, k, result);
        if (k > 0) {
            result.push_back(root->key);
            k--;
            getTopKUtil(root->left, k, result);
        }
    }

    void printInOrder(Node* root) {
        if (root) {
            printInOrder(root->left);
            cout << root->key << " ";
            printInOrder(root->right);
        }
    }

public:
    Node* root;
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insertNode(root, key);
        cout << "Inserted " << key << ". Tree (Inorder): ";
        printInOrder(root);
        cout << "\n";
    }

    void remove(int key) {
        root = deleteNode(root, key);
        cout << "Deleted " << key << ". Tree (Inorder): ";
        printInOrder(root);
        cout << "\n";
    }

    vector<int> queryTopK(int k) {
        vector<int> result;
        getTopKUtil(root, k, result);
        return result;
    }
};

int main() {
    AVLTree tree;

    // Insert multiple elements
    int elements[] = { 20, 4, 15, 71, 50, 9, 32 };
    cout << "--- Inserting Elements into AVL Tree ---\n";
    for (int val : elements) {
        tree.insert(val);
    }

    int k = 3;
    vector<int> topK = tree.queryTopK(k);
    cout << "\nTop " << k << " elements (maximum criteria): ";
    for (int val : topK) {
        cout << val << " ";
    }
    cout << "\n\n";

 
    cout << "--- Deleting Elements from AVL Tree ---\n";
    tree.remove(71);
    tree.remove(50);

    return 0;
}