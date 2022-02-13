#include <iostream>
#include <list>

template<class T>
class AvlTree {
public:
    AvlTree() : mHead(nullptr) {};
    ~AvlTree();
    void InsertData(const T& data);
    void DeleteData(const T& data);
    T FindKStatistic(size_t pos);

private:

    struct AvlTreeNode {
        T mData;
        unsigned short mHeight;
        size_t mNumChilds;
        AvlTreeNode* mLeft;
        AvlTreeNode* mRight;
        AvlTreeNode(const T& some_data, AvlTreeNode* left, AvlTreeNode* right) :
                mData(some_data), mLeft(left), mRight(right), mHeight(1), mNumChilds(1) {}

        friend AvlTree;
    };

    AvlTreeNode* mHead;

    // вставка элемента
    AvlTreeNode* insertData(AvlTreeNode* a, const T& data);

    // удаление элемента
    AvlTreeNode* find_min(AvlTreeNode* a);
    AvlTreeNode* removemin(AvlTreeNode* a);
    AvlTreeNode* deleteData(AvlTreeNode* a, const T& data);

    //  функции балансировки дерева
    AvlTreeNode* RotateLeft(AvlTreeNode* a);
    AvlTreeNode* RotateRight(AvlTreeNode* a);
    AvlTreeNode* Balance(AvlTreeNode* a);

    unsigned short FindHeight(AvlTreeNode *a) {
        return a ? a->mHeight : 0;
    }

    short BalanceFactor(AvlTreeNode* a) {
        return FindHeight(a->mRight) - FindHeight(a->mLeft);
    }

    void ChangeHeight(AvlTreeNode* a) {
        unsigned short left = FindHeight(a->mLeft);
        unsigned short right = FindHeight(a->mRight);
        a->mHeight = (left > right ? left : right) + 1;
    }

    unsigned int FindChilds(AvlTreeNode *a) {
        return a ? a->mNumChilds : 0;
    }

    unsigned int BalanceChilds(AvlTreeNode* a) {
        return FindChilds(a->mRight) + FindChilds(a->mLeft) + 1;
    }

    size_t GetChilds(AvlTreeNode* a) {
        return a ? a->mNumChilds : 0;
    }

public:  //  дебаг штука
    void Inorder() {
        inorder(mHead);
    }

    void inorder(AvlTreeNode* a) {
        if (!a) {
            return;
        }
        inorder(a->mLeft);
        std::cout << a->mData << " ";
        inorder(a->mRight);
    }

};


template<class T>
typename AvlTree<T>::AvlTreeNode* AvlTree<T>::RotateRight(AvlTreeNode* a) {
    AvlTreeNode* q = a->mLeft;
    a->mLeft = q->mRight;
    q->mRight = a;
    ChangeHeight(a);
    a->mNumChilds = BalanceChilds(a);
    ChangeHeight(q);
    q->mNumChilds = BalanceChilds(q);
    return q;
}

template<class T>
typename AvlTree<T>::AvlTreeNode* AvlTree<T>::RotateLeft(AvlTreeNode* q) {
    AvlTreeNode* a = q->mRight;
    q->mRight = a->mLeft;
    a->mLeft = q;
    ChangeHeight(q);
    q->mNumChilds = BalanceChilds(q);
    ChangeHeight(a);
    a->mNumChilds = BalanceChilds(a);
    return a;
}

template<class T>
typename AvlTree<T>::AvlTreeNode* AvlTree<T>::Balance(AvlTreeNode* a) {
    a->mNumChilds = BalanceChilds(a);
    ChangeHeight(a);
    if (BalanceFactor(a) == 2) {
        if (BalanceFactor(a->mRight) < 0) {
            a->mRight = RotateRight(a->mRight);
        }
        return RotateLeft(a);
    }

    if (BalanceFactor(a) == -2) {
        if (BalanceFactor(a->mLeft) > 0) {
            a->mLeft = RotateLeft(a->mLeft);
        }
        return RotateRight(a);
    }
    return a;
}

template<class T>
typename AvlTree<T>::AvlTreeNode* AvlTree<T>::insertData(AvlTreeNode* a, const T& data) {
    if (!a) {
        return new AvlTreeNode(data, nullptr, nullptr);
    }
    if (data < a->mData) {
        a->mLeft = insertData(a->mLeft, data);
    } else {
        a->mRight = insertData(a->mRight, data);
    }
    return Balance(a);
}

template<class T>
void AvlTree<T>::InsertData(const T& data) {
    mHead = insertData(mHead, data);
}

template<class T>
typename AvlTree<T>::AvlTreeNode* AvlTree<T>::find_min(AvlTreeNode* a) {
    return a->mLeft ? find_min(a->mLeft) : a;
}

template<class T>
typename AvlTree<T>::AvlTreeNode* AvlTree<T>::removemin(AvlTreeNode* a) {
    if (a->mLeft == nullptr) {
        return a->mRight;
    }
    a->mLeft = removemin(a->mLeft);
    return Balance(a);
}

template<class T>
typename AvlTree<T>::AvlTreeNode* AvlTree<T>::deleteData(AvlTreeNode* a, const T& data) {
    if (a == nullptr) {
        return nullptr;
    }
    if (data < a->mData) {
        a->mLeft = deleteData(a->mLeft, data);
    } else if (data > a->mData) {
        a->mRight = deleteData(a->mRight, data);
    } else {
        AvlTreeNode* l = a->mLeft;
        AvlTreeNode* r = a->mRight;

        delete a;
        if (r == nullptr) {
            return l;
        }

        AvlTreeNode* min = find_min(r);
        min->mRight = removemin(r);
        min->mLeft = l;
        return Balance(min);
    }
    return Balance(a);
}

template<class T>
void AvlTree<T>::DeleteData(const T& data) {
    mHead = deleteData(mHead, data);
}

template<class T>
AvlTree<T>::~AvlTree() {
    std::list<AvlTreeNode*> mList;
    mList.push_front(mHead);

    while (!mList.empty()) {
        AvlTreeNode* tmp = mList.front();
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
T AvlTree<T>::FindKStatistic(size_t pos) {
    AvlTreeNode* cur = mHead;
    size_t num = GetChilds(cur->mLeft);

    while (num != pos) {
        if (num >= pos) {
            cur = cur->mLeft;
            num -= (1 + GetChilds(cur->mRight));
        }

        if (num < pos) {
            cur = cur->mRight;
            num += (GetChilds(cur->mLeft) + 1);
        }
    }
    return cur->mData;
}


int main() {
    AvlTree<int> a;
    int n = 0;
    std::cin >> n;
    int num = 0;
    int statistic = 0;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> num >> statistic;
        num > 0 ? a.InsertData(num) : a.DeleteData(abs(num));
        std::cout << a.FindKStatistic(statistic) << "\n";
    }
    return 0;
}
