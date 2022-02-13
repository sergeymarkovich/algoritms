// Реализовать дек с динамическим зацикленным буфером.
// Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
// Формат входных данных.
// В первой строке количество команд n. n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа: a b.
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back

#include <iostream>
#include <cassert>

template<class T>
class Deque {
 public:
    Deque();
    ~Deque();

    Deque(const Deque&) = delete;
    Deque& operator= (const Deque&) = delete;

    size_t Size();

    void PushBack(const T& value);
    T PopBack();
    void PushFront(const T& value);
    T PopFront();

 private:
    size_t count;
    size_t head;
    size_t tail;
    T* buffer;
};

template<class T>
Deque<T>::Deque() {
    tail = 0;
    head = 0;
    count = 1;
    buffer = new T[count];
}

template<class T>
Deque<T>::~Deque() {
    if (count > 0) {
        delete[] buffer;
    }
}

template<class T>
size_t Deque<T>::Size() {
    if (tail > head) {
        return count - head + tail;
    } else {
        return tail - head;
    }
}

template<class T>
void Deque<T>::PushBack(const T& value) {
    if (head == (tail + 1) % count) {  //  место закончилось
        T* newbuf = new T[count * 2];
        for (size_t i = 0; i < count; ++i) {
            newbuf[i] = buffer[head];  // TODO(исправить для сложных типов)
            head = (head + 1) % count;
        }
        delete[] buffer;
        buffer = newbuf;
        head = 0;
        tail = count - 1;
        count *= 2;
    }
    buffer[tail] = value;
    tail = (tail + 1) % count;
}

template<class T>
T Deque<T>::PopBack() {
    assert(Size());
    if (Size() < count/4) {
        T* newbuf = new T[count / 2];
        size_t actual_size = Size();
        for (size_t i = 0; i < actual_size - 1; ++i) {
            newbuf[i] = buffer[head];  // TODO(исправить для сложных типов)
            head = (head + 1) % count;
        }
        delete[] buffer;
        buffer = newbuf;
        head = 0;
        tail = actual_size;
        count /= 2;
    }
    tail = (tail - 1 + count) % count;
    return buffer[tail];
}

template<class T>
void Deque<T>::PushFront(const T& value) {
    if (head == (tail + 1) % count) {  //  место закончилось
        T* newbuf = new T[count * 2];
        for (size_t i = 0; i < count; ++i) {
            newbuf[i] = buffer[head];  // TODO(исправить для сложных типов)
            head = (head + 1) % count;
        }
        delete[] buffer;
        buffer = newbuf;
        head = 0;
        tail = count - 1;
        count *= 2;
    }
    head = (head - 1) % count;
    buffer[head] = value;
}

template<class T>
T Deque<T>::PopFront() {
    assert(Size());
    if (Size() < count/4) {
        T* newbuf = new T[count / 2];
        size_t actual_size = Size();
        for (size_t i = 0; i < actual_size - 1; ++i) {
            newbuf[i] = buffer[head];  // TODO(исправить для сложных типов)
            head = (head + 1) % count;
        }
        delete[] buffer;
        buffer = newbuf;
        head = 0;
        tail = actual_size;
        count /= 2;
    }
    T return_value = buffer[head];
    head = (head + 1) % count;
    return return_value;
}


int main() {
    Deque<int> q1;
    int n = 0;
    std::cin >> n;
    bool result = true;
    for (size_t i = 0; i < n; ++i) {
        int command = 0;
        int value = 0;
        std::cin >> command >> value;
        switch (command) {
        case 1:
            q1.PushFront(value);
            break;
        case 2: {
            if (q1.Size() <= 0) {
                result = result && value == -1;
            } else { 
                result = result && q1.PopFront() == value;
            }
            break;
        } 
        case 3:
            q1.PushBack(value);
            break;
        case 4: {
            if (q1.Size() <= 0) {
                result = result && value == -1;
            } else {
                result = result && q1.PopBack() == value;
            }
            break;
        }
        default:
            assert(false);
        }
    }
    std::cout << (result ? "YES" : "NO");
    return 0;
}