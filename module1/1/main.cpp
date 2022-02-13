// 1_4. “Считалочка”. В круг выстроено N человек, пронумерованных числами от 1 до N. 
// Будем исключать каждого k-ого до тех пор, пока не уцелеет только один человек. 
// (Например, если N=10, k=3, то сначала умрет 3-й, потом 6-й, затем 9-й, затем 2-й, затем 7-й, потом 1-й, 
// потом 8-й, за ним - 5-й, и потом 10-й. Таким образом, уцелеет 4-й.) Необходимо определить номер уцелевшего.
// N, k ≤ 10000.
// Требования:  Решить перебором.

#include <iostream>

struct List {
    size_t num;
    List* prev;
    List* next;
};

void list_step(List** head, size_t k) {  // переход на k шагов вперед
    for (size_t i = 0; i < k; ++i) {
        *head = (*head)->next;
    }
}

List* delete_node_and_move_forward(List* node) {  // удаление нода, который передается, возвращается след.
    node->prev->next = node->next;
    node->next->prev = node->prev;
    List* shift = node->next;
    delete node;
    return shift;
}

void make_list(List**head, size_t list_size) {
    (*head)->num = 1;
    (*head)->next = nullptr;
    (*head)->prev = nullptr;
    List* some_pointer = *head;

    for (size_t i = 2; i <= list_size; ++i) {
        List* new_node = new List;
        new_node->num = i;
        new_node->prev = some_pointer;
        new_node->next = nullptr;
        some_pointer->next = new_node;
        some_pointer = some_pointer->next;
    }

    some_pointer->next = *head;
    (*head)->prev = some_pointer;
}

int main() {
    size_t n = 0;
    size_t k = 0;
    std::cin >> n >> k;

    List* head = new List;
    make_list(&head, n);

    while (head->next != head) {
        list_step(&head, k - 1);                   // тут k - 1, по условию задачи удаляем человека под этим номером, 
        head = delete_node_and_move_forward(head); // то есть с учетом номерации с 1 и при удалении переходим на следующего
    }                                              // удаляем до тех пор пока не останется только head

    std::cout << head->num << std::endl;
    delete head;
    return 0;
}