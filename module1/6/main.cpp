// Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..109] размера n.
// Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
// Напишите нерекурсивный алгоритм.
// Требования к дополнительной памяти: O(n).
// Требуемое среднее время работы: O(n).
// Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
// 6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

#include <iostream>
#include <algorithm>

template <class T>
bool isLessDefault(const T& l, const T& r) {
    return l < r;
}

template <class T>
T* FindMedian(T* first, T* middle, T* last, bool isLess(const T&, const T&)) {  // находим элемент который больше одного, но меньше другого
    if ((isLess(*middle, *first) || isLess(*last, *first)) && (isLess(*first, *last) || isLess(*first, *middle))) {
        return first;
    } else if ((isLess(*first, *middle) || isLess(*last, *middle)) && (isLess(*middle, *last) || isLess(*middle, *first))) {
        return middle;
    }
    return last;
}

template <class T>
int Partition(T *arr, int left, int right, bool isLess(const T&, const T&)) {
    T* pivot = FindMedian(&arr[left], &arr[(left + right) / 2], &arr[right - 1], isLess);
    std::swap(*pivot, arr[left]);

    int pos = right - 1;

    for (size_t i = right - 1; i > left; --i) {
        if(!isLess(arr[i], arr[left])) {
            std::swap(arr[pos], arr[i]);
            pos--;
        }
    }

    std::swap(arr[pos], arr[left]);
    return pos;
}

template <class T>
T FindKStatistic(T *arr, int right, int left, int k, bool isLess(const T&, const T&) = isLessDefault) {
    int pivot_pos = Partition(arr, left, right, isLess);

    while(pivot_pos != k) {
        if(pivot_pos < k) {
            left = pivot_pos + 1;
        } else {
            right = pivot_pos;
        }
        pivot_pos = Partition(arr, left, right, isLess);
    }

    return arr[pivot_pos];
}

int main () {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    int* arr = new int [n];

    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    std::cout << FindKStatistic(arr, n, 0, k);
    delete[] arr;
    return 0;
}