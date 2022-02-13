#include <iostream>


class user_t {
public:
    int userId;
    int frequency;

    friend bool operator<(const user_t &l, const user_t &r) {
        return l.frequency < r.frequency;
    }
};

template<typename T>
class DefaultComparator {
public:
    bool operator()(const T &l, const T &r) {
        return l < r;
    }
};

template<typename T>
class vector_t {
public:
    vector_t<T>(int size_initial) : size(0), capacity(size_initial), array(new T[size_initial]) {};

    int Size() {
        return size;
    }

    void Add(T element) {
        array[size] = element;
        size++;
    }

    bool IsEmpty() {
        return (size == 0);
    }

    T Last() {
        return array[size];
    }

    void DeleteLast() {
        array[size] = 0;
        size--;
    }

    int size;
    int capacity;
    T *array;

    void vec_free() {
        delete[] array;
    }
};

template<typename T, typename Comparator = DefaultComparator<T>>
class heap_t {
public:
    explicit heap_t(int i) : arr(i) {
    }

    void buildHeap(Comparator cmp);

    void siftDown(int i, Comparator cmp);

    void siftUp(int i, Comparator cmp);

    T ExtractTop(Comparator cmp);

    void insert(T element, Comparator cmp);

    void print() {
        for (int i = 0; i < arr.size; i++) {
            std::cout << arr.array[i] << " ";
        }
    }

private:
public:
    vector_t<T> arr;

};

template<typename T, typename Comparator>
void heap_t<T, Comparator>::siftDown(int i, Comparator cmp) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    if (left < arr.Size() && cmp(arr.array[left], arr.array[i]))
        largest = left;
    if (right < arr.Size() && cmp(arr.array[right], arr.array[largest]))
        largest = right;
    if (largest != i) {
        std::swap(arr.array[i], arr.array[largest]);
        siftDown(largest, cmp);
    }
}


template<typename T, typename Comparator>
void heap_t<T, Comparator>::buildHeap(Comparator cmp) {
    for (int i = arr.Size() / 2 - 1; i >= 0; --i) {
        siftDown(i, cmp);
    }
}

template<typename T, typename Comparator>
void heap_t<T, Comparator>::siftUp(int index, Comparator cmp) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (!cmp(arr.array[index], arr.array[parent]))
            return;
        std::swap(arr.array[index], arr.array[parent]);
        index = parent;
    }
}


template<typename T, typename Comparator>
void heap_t<T, Comparator>::insert(T element, Comparator cmp) {
    if (this->arr.Size() > this->arr.capacity) {
        this->ExtractTop(cmp);
    }
    arr.Add(element);
    siftUp(arr.Size() - 1, cmp);
}

template<typename T, typename Comparator>
T heap_t<T, Comparator>::ExtractTop(Comparator cmp) {
    T result = arr.array[0];
    arr.array[0] = arr.array[arr.size - 1];
    arr.size -= 1;
    if (arr.size > 0) {
        siftDown(0, cmp);
    }
    return result;
}

int main(int argc, const char *argv[]) {
    int n = 0;
    int k = 0;
    std::cin >> n;
    std::cin >> k;
    heap_t<user_t> heap(k);
    DefaultComparator<user_t> c;
    user_t tmp;
    for (int i = 0; i < n; i++) {
        std::cin >> tmp.userId;
        std::cin >> tmp.frequency;
        heap.insert(tmp, c);
    }

    for (int i = 0; i < k; i++) {
        std::cout << heap.ExtractTop(c).userId << " ";
    }
    heap.arr.vec_free();
    return 0;
}
