// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N. 
// Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е., 
// при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется 
// в правое поддерево root; иначе в левое поддерево root. Выведите элементы в порядке pre-order (сверху вниз).
// Рекурсия запрещена.

#include <iostream>
#include <list>

template<class T>
bool defaultComparison(const T& l, const T& r) {
    return l < r;
}

template<class T>
class NaiveBinTree {
    public:
    NaiveBinTree(bool (*isLess)(const T& l, const T& r) = defaultComparison): head(nullptr), mCompare(isLess){};
    ~NaiveBinTree();

    NaiveBinTree(const NaiveBinTree&) = delete;
    NaiveBinTree& operator=(const NaiveBinTree&) = delete;

    void InsertData(const T& data);
    void PreOrder(void visit (T));

    private:
    struct mTreeNode{
        T mData;

        mTreeNode* mLeft;
        mTreeNode* mRight;
        mTreeNode(T data, mTreeNode* left, mTreeNode* right): 
            mData(data), mLeft(left), mRight(right) {}; 
    };

    mTreeNode* head;
    bool (*mCompare)(const T&, const T&);

};

template<class T>
void NaiveBinTree<T>::InsertData(const T& data) {
    mTreeNode* pointer = head;

    if (pointer == nullptr) {
        pointer = new mTreeNode(data, nullptr, nullptr);
        head = pointer;
        return;
    }

    while (pointer != nullptr) {
        if (mCompare(data, pointer->mData)) {
            if (pointer->mLeft != nullptr) {
                pointer = pointer->mLeft;
            } else {
                mTreeNode* q = new mTreeNode(data, nullptr, nullptr);
                pointer->mLeft = q;
                return;
            }
        } else if (!mCompare(data, pointer->mData)) {
            if (pointer->mRight != nullptr) {
                pointer = pointer->mRight;
            } else {
                mTreeNode* q = new mTreeNode(data, nullptr, nullptr);
                pointer->mRight = q;
                return;
            }
        }
    }
}

template<class T>
NaiveBinTree<T>::~NaiveBinTree() {
    std::list<mTreeNode*> mList;
    mList.push_front(head);
    
    while (!mList.empty()) {
        mTreeNode* tmp = mList.front();
        mList.pop_front();
        if (tmp->mRight != nullptr) {
            mList.push_front(tmp->mRight);
        }
        if (tmp->mLeft != nullptr) {
            mList.push_front(tmp->mLeft);
        }
        delete tmp;
    }
}

template<class T>
void NaiveBinTree<T>::PreOrder(void visit (T)) {
    std::list<mTreeNode*> mList;
    mList.push_front(head);
    
    while (!mList.empty()) {
        mTreeNode* tmp = mList.front();
        visit(tmp->mData);
        mList.pop_front();
        if (tmp->mRight != nullptr) {
            mList.push_front(tmp->mRight);
        }
        if (tmp->mLeft != nullptr) {
            mList.push_front(tmp->mLeft);
        }
    }
}


int main() {
    NaiveBinTree<int> a;
    int n = 0;
    std::cin >> n;
    int temp = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> temp;
        a.InsertData(temp);
    }
    a.PreOrder([](int key) {std::cout << key << " "; });
    return 0;
}