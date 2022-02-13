// 2_4. Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс элемента массива A[k], ближайшего по значению к B[i].
// Требования: Время работы поиска для каждого элемента B[i]: O(log(k)). Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с 
// помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.

#include <iostream>

class Find {
public:
    Find(size_t sorted_size, int* sorted_arr, size_t size, int* arr) {
        _sorted_arr = sorted_arr;
        _sorted_size = sorted_size;
        _size = size;
        _arr = arr;
        left = 0;
        right = 0;
    }

    ~Find() = default;

    void FindTwin(size_t* index_arr) {
        for (size_t i = 0; i < _size; ++i) {
            index_arr[i] = find_closest(_arr[i]);
        }
    }

private:
    size_t _sorted_size;
    int* _sorted_arr;
    size_t _size;
    int* _arr;

    size_t left;
    size_t right;

    void find_bound(int elem) {
        left = 0;
        right = 1;
        while (right < _sorted_size && elem > _sorted_arr[right]) {
            left = right;
            right *= 2;
        }
        if (right >= _sorted_size) {
            right = _sorted_size - 1;
        }
    }

    size_t find_closest(int elem) {
        find_bound(elem);
        while(left < right) {
            if (elem == _sorted_arr[(left + right) / 2]) {
                return (left + right) / 2;
            }
            if (elem > _sorted_arr[(left + right) / 2]) {
                left = (left + right) / 2 + 1;
            } 
            if (elem < _sorted_arr[(left + right) / 2])  {
                right = (left + right) / 2;
            }
        }

        if (left == 0) {
            return left;
        }

        if (elem - _sorted_arr[left - 1] <= _sorted_arr[left] - elem && left > 0) {
            left--;
        }

        while (left > 0 && _sorted_arr[left - 1] == _sorted_arr[left]) {
            left--;
        }
        return left;
    }
};


int main() {
    size_t n = 0;
    std::cin >> n;
    int* sorted_arr = new int[n];
    for (size_t i = 0; i < n; ++i) {
        std::cin >> sorted_arr[i];
    }

    size_t m = 0;
    std::cin >> m;
    int* unsorted_arr = new int[m];
    for (size_t i = 0; i < m; ++i) {
        std::cin >> unsorted_arr[i];
    }

    Find task(n, sorted_arr, m, unsorted_arr);
    size_t* index_arr = new size_t[m];
    task.FindTwin(index_arr);
    
    for (size_t i = 0; i < m; ++i) {
        std::cout << index_arr[i] << " ";
    }
    delete [] sorted_arr;
    delete [] unsorted_arr;
    delete [] index_arr;
    return 0;
}