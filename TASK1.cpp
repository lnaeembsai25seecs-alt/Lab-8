#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

// A Huffman tree node
struct MinHeapNode {
    char data;
    unsigned freq;
    MinHeapNode* left, * right;

    MinHeapNode(char data, unsigned freq) {
        left = right = nullptr;
        this->data = data;
        this->freq = freq;
    }
};

// Comparison object to be used to order the heap
struct compare {
    bool operator()(MinHeapNode* l, MinHeapNode* r) {
        return (l->freq > r->freq);
    }
};

// Print Huffman codes from the built Huffman Tree using Preorder Traversal
void printPreorderCodes(MinHeapNode* root, string str) {
    if (!root)
        return;

    // If this is a leaf node, it contains one of the input characters
    if (root->data != '$') {
        cout << root->data << ": " << str << "\n";
    }

    printPreorderCodes(root->left, str + "0");
    printPreorderCodes(root->right, str + "1");
}

// Main function to build Huffman Tree and generate codes
void buildHuffmanTree(char data[], int freq[], int size) {
    MinHeapNode* left, * right, * top;

    // Create a min heap and inserts all characters of data[]
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

    for (int i = 0; i < size; ++i)
        minHeap.push(new MinHeapNode(data[i], freq[i]));

    // Iterate while size of heap doesn't become 1
    while (minHeap.size() > 1) {
        // Extract the two minimum freq items from min heap
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

       
        top = new MinHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    cout << "Huffman Codes (Preorder Traversal):\n";
    printPreorderCodes(minHeap.top(), "");
}

int main() {
    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = { 5, 9, 12, 13, 16, 45 };
    int size = sizeof(arr) / sizeof(arr[0]);

    buildHuffmanTree(arr, freq, size);
    return 0;
}