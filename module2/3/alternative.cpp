#include <iostream>
#include <list>

namespace trees {
    template<class T>
    bool defaultComparison(const T& l, const T& r) {
        return l < r;
    }
    
    template<class T>
    class NaiveBinTree {
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

        public:
        NaiveBinTree(bool (*isLess)(const T& l, const T& r) = defaultComparison): head(nullptr), mCompare(isLess){};

        NaiveBinTree(const NaiveBinTree&) = delete;
        NaiveBinTree& operator=(const NaiveBinTree&) = delete;

        void InsertData(const T& data);
        mTreeNode* GetHead() {return head;}
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
    class Treap {
        private:
        struct mTreapNode{
            T key;
            int priority;

            mTreapNode* mLeft;
            mTreapNode* mRight;

            mTreapNode(T data, int prior, mTreapNode* left, mTreapNode* right):
                key(data), priority(prior), mLeft(left), mRight(right) {};
        };
        mTreapNode* head;
        bool (*mCompare)(const T&, const T&);
        void split(mTreapNode* cur, T key, mTreapNode*& left, mTreapNode*& right);


        public:
        Treap(bool (*isLess)(const T& l, const T& r) = defaultComparison): head(nullptr), mCompare(isLess){};
        Treap(const Treap&) = delete;
        Treap& operator=(const Treap&) = delete;

        void InsertData(const T& data, int priority);
        mTreapNode* GetHead() {return head;}
    };

    template<class T>
    void Treap<T>::split(mTreapNode* cur, T key, mTreapNode*& left, mTreapNode*& right) {
        if (cur == nullptr) {
            left = nullptr;
            right = nullptr;
        } else if (mCompare(cur->key, key)) {
            split(cur->mRight, key, cur->mRight, right);
            left = cur;
        } else {
            split(cur->mLeft, key, left, cur->mLeft);
            right = cur;
        }
    }

    template<class T>
    void Treap<T>::InsertData(const T& key, int priority) {
        mTreapNode* pointer = head;
        mTreapNode* parent = nullptr;

        while (pointer && pointer->priority >= priority) {
            parent = pointer;
            pointer = mCompare(key, pointer->key) ? pointer->mLeft : pointer->mRight;
        }

        mTreapNode* left = nullptr;
        mTreapNode* right = nullptr;

        split(pointer, key, left, right);

        pointer = new mTreapNode(key, priority, left, right);

        if (!parent) {
            head = pointer;
            return;
        }
        (mCompare(parent->key, key) ? parent->mRight : parent->mLeft) = pointer;
    }
}

template<class T>
int MaxLayerNodeCount(T* head) {
    std::list<T*> mList;
    T* current = head;

    mList.push_back(current);
    mList.push_back(nullptr);
    int MaxLen = 1;
    int CurLen = 0;

    while(!mList.empty()) {
        current = mList.front();
        if (current != nullptr) {
            if (current->mLeft) {
                mList.push_back(current->mLeft);
                CurLen++;
            }
            if (current->mRight) {
                mList.push_back(current->mRight);
                CurLen++;
            }
            mList.pop_front();
        } else {
            if (MaxLen < CurLen) {
                MaxLen = CurLen;
            }
            mList.pop_front();
            CurLen = 0;
            if (mList.empty()) {
                break;
            }
            mList.push_back(nullptr);
        }
    }
    return MaxLen;
}

template<class T>
void clear(T* head) {
    std::list<T*> mList;
    mList.push_front(head);

    while (!mList.empty()) {
        T* tmp = mList.front();
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


int main() {
    trees::NaiveBinTree<int> bin;
    trees::Treap<int> treap;
    int n = 0;
    std::cin >> n;
    int key = 0;
    int priority = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> key >> priority;
        bin.InsertData(key);
        treap.InsertData(key, priority);
    }
    std::cout <<  MaxLayerNodeCount(treap.GetHead()) - MaxLayerNodeCount(bin.GetHead()) << std::endl;
    clear(treap.GetHead());
    clear(bin.GetHead());

    return 0;
}