// Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 1000000. Отсортировать массив методом MSD по битам (бинарный QuickSort).

#include <iostream>

int Bit(long long num, int pos) {
    long long bitMask = 1;  // именно такая запись иначе компилятор 1 в int превратит :(
    bitMask = bitMask << pos;
    return (bitMask & num ? 1 : 0);  //  вернем 1 если бит совпал с маской текущей позиции, иначе ноль, по сути получаем бит pos разряда
}

int Partition(unsigned long long* arr, int left, int right, int pos) {  // сортировка на два подмассива
    int i = left;
    int j = right;
    while (i != j) {
        while(Bit(arr[i], pos) == 0 && (i < j)) {  // сортировка массива вокруг мнимого пивота
            i++;
        }
        while(Bit(arr[j], pos) == 1 && (i < j)) {
            j--;
        }
        std::swap(arr[i], arr[j]);
    }
    return i;  //  возвращаем позицию первой единицы
}

void BinaryQuickSort(unsigned long long* arr, int left, int right, int pos) {
    if (left >= right || pos < 0) {
        return;
    }

    int pivot = Partition(arr, left, right, pos);  // возвращает крайний элемент с 1 в pos

    if (Bit(arr[right], pos) == 0) {  //  если только во всех числах 0 на позиции pos
        pivot++;
    }
    BinaryQuickSort(arr, left, pivot - 1, pos - 1);  //  с 0 
    BinaryQuickSort(arr, pivot, right, pos - 1);  // с 1
}

int main() {
    int n = 0;
    std::cin >> n;
    unsigned long long* array = new unsigned long long[n];
    for (size_t i = 0; i < n; i++) {
        std::cin >> array[i];
    }
    int digit_count = 63;  //  всего разрядов 8 * 8 = 64. От 0 до 63, идем от старшего к младшему.
    BinaryQuickSort(array, 0, n - 1, digit_count);

    for(size_t i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }
    delete[] array;
    return 0;
}