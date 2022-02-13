// 4_4. Скользящий максимум.
// Дан массив натуральных чисел A[0..n), n не превосходит 10^8. Так же задан размер некоторого окна (последовательно расположенных элементов массива) в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума в окне.
// Требования: Скорость работы O(n log n), память O(n).
// Формат входных данных. Вначале вводится n - количество элементов массива. Затем вводится n строк со значением каждого элемента. Затем вводится k  - размер окна.
// Формат выходных данных. Разделенные пробелом значения максимумов для каждого положения окна.

#include <iostream>
#include <algorithm>
#include <cassert>

struct Obj {
    Obj() : value(0), position(0) {}
    int value;
    size_t position;

    Obj& operator=(const Obj c2) {
        value = c2.value;
        position = c2.position;
        return *this;
    }
};

bool CompareByValueEq(const Obj& l, const Obj& r) {
    return l.value <= r.value;
}

bool CompareByValue(const Obj& l, const Obj& r) {
    return l.value < r.value;
}


template<class T>
class Heap {
 public:
    Heap(size_t heap_size, bool isLess(const T& l, const T& r), bool lessOrEqual(const T& l, const T& r));
    ~Heap();

    Heap(const Heap&) = delete;
    Heap& operator= (const Heap&) = delete;

    void Build(const T* array, size_t arr_size);
    void Insert(T element);
    T ExtractMax();
    T PeekMax();

 private:
 T* arr;
 bool (*compareFunction)(const T&, const T&);
 bool (*compareOrEqualFunction)(const T&, const T&);
 void siftDown(size_t i);
 void siftUp(size_t i);
 size_t size;
 size_t buffer_size;
};

template<class T>
Heap<T>::Heap(size_t heap_size, bool isLess(const T& l, const T& r), bool lessOrEqual(const T& l, const T& r)) : buffer_size(heap_size), size(0) {
    arr = new T[heap_size];
    compareFunction = isLess;
    compareOrEqualFunction = lessOrEqual;
}

template<class T>
Heap<T>::~Heap() {
    delete[] arr;
}

template<class T>
void Heap<T>::Build(const T* array, size_t arr_size) {
    size = arr_size;
    for (size_t i = 0; i < arr_size; ++i) {
        arr[i] = array[i];
    }
    for (int i = (size - 1) / 2; i >= 0; i--) {  // buildheap
        siftDown(i);
    }
}
template<class T>
void Heap<T>::siftDown(size_t i) {
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    size_t largest = i;  // Ищем большого сына
    if (left < size && compareFunction(arr[i], arr[left])) {
        largest = left;
    }
    if(right < size && compareFunction(arr[largest], arr[right])) {
        largest = right;
    }

    if(largest != i) {  // если больший сын есть, то проталкиваем корень в него
        std::swap(arr[i], arr[largest]);
        siftDown(largest);
    }
}

template<class T>
void Heap<T>::siftUp(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (compareOrEqualFunction(arr[i], arr[parent])) {
            return;
        }
        std::swap(arr[i], arr[parent]);
        i = parent;
    }
}

template<class T>
void Heap<T>::Insert(T element) {
    if (size == buffer_size) {  //  если место закончилось
        T* new_arr = new T[buffer_size * 2];
        for (size_t i = 0; i < size; ++i) {
            new_arr[i] = arr[i];
        }
        delete[] arr;
        arr = new_arr;
        buffer_size *= 2;
    }
    arr[size] = element;
    size++;
    siftUp(size - 1);
}

template<class T>
T Heap<T>::PeekMax() {
    assert(size > 0);
    return arr[0];
}

template<class T>
T Heap<T>::ExtractMax() {
    assert(size > 0);
    T result = arr[0];
    arr[0] = arr[size - 1];
    size--;
    if (size > 0) {
        siftDown(0);
    }
    return result;
}

void SearchWindows(Obj* array, size_t array_size, size_t window) {
    Heap<Obj> Heap(array_size, CompareByValue, CompareByValueEq);
    Heap.Build(array, window);
    std::cout << Heap.PeekMax().value << " ";
    size_t left = 1;
    size_t right = left + window - 1;
    for(; right < array_size; ++left, ++right) {
        Heap.Insert(array[right]);
        while (Heap.PeekMax().position < left) {
            Heap.ExtractMax();
        }
        std::cout << Heap.PeekMax().value << " ";
    }
}


int main() {
    int n = 0;
    std::cin >> n;
    Obj* array = new Obj[n];
    for (size_t i = 0; i < n; ++i) {
        std::cin >> array[i].value;
        array[i].position = i;
    }
    int window = 0;
    std::cin >> window;
    SearchWindows(array, n, window);
    delete[] array;
    return 0;
}