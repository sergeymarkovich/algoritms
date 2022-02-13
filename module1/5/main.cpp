// 5_3. Закраска прямой 1.
// На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri). Найти длину окрашенной части числовой прямой.

#include <iostream>
#include <cstring>

struct Point {
    Point() : pos(0), sign(0) {}
    int pos;
    bool sign;  // 0 - левая, 1 - правая

    Point& operator= (const Point r) {  // перегрузка структуры для универсальности mergesort
        this->pos = r.pos;
        this->sign = r.sign;
        return *this;
    }
};

bool pointComparasion(const Point& l, const Point& r) {  // сортировка для задания
    return l.pos < r.pos;
}

template <class T>
bool isLessDefault (const T& l, const T& r) {  // сортировка по умолчанию
    return l < r;
}

template <class T>
void Merge(T* first_half, int first_len, T* second_half, int second_len, T* new_arr, bool isLess(const T&, const T&)) {
    int count = 0;
    int i = 0;
    int j = 0;
    while (i < first_len && j < second_len) {
        if (isLess(first_half[i], second_half[j])) {
            new_arr[count] = first_half[i];
            i++;
            count++;
        } else {
            new_arr[count] = second_half[j];
            j++;
            count++;
        }
    }
    if (i >= first_len) {
        for (; j < second_len; j++, count++) {
            new_arr[count] = second_half[j];
        }
    }
    if (j >= second_len) {
        for (; i < first_len; i++, count++) {
            new_arr[count] = first_half[i];
        }
    }
}

template <class T>
void MergeSort(T* arr, int arr_len, bool isLess(const T&, const T&) = isLessDefault) {
    if (arr_len <= 1) {
        return;
    }
    int first_len = arr_len/2;
    int second_len = arr_len - first_len;
    MergeSort(arr, first_len, isLess);
    MergeSort(arr + first_len, second_len, isLess);
    T* new_arr = new T[arr_len];
    Merge(arr, first_len, arr + first_len, second_len, new_arr, isLess);
    memcpy(arr, new_arr, sizeof(T) * arr_len);
    delete[] new_arr;
}

int find_longest_line(Point* sorted_arr, int arr_len) {
    int len = 0;
    int sequence = 0;
    for(int i = 0; i < arr_len - 1; ++i) {
        if (sorted_arr[i].sign == 0) {
            ++sequence;
        } else {
            --sequence;
        }
        if (sequence > 0) {
            len += sorted_arr[i + 1].pos - sorted_arr[i].pos;
        }
    }
    return len;
}

int main() {
    int n = 0;
    std::cin >> n;
    Point* arr = new Point[2 * n];
    for (size_t i = 0; i < 2 * n; ++i) {
        if (i % 2 == 0) {
            arr[i].sign = 0;
        } else {
            arr[i].sign = 1;
        }
        std::cin >> arr[i].pos;
    }
    MergeSort(arr, 2 * n, pointComparasion);
    std::cout << find_longest_line(arr, 2 * n);
    delete[] arr;
    return 0;
}
