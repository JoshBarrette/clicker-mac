#include "queue.hpp"

#include <iostream>

Queue::Queue() {
    for (int i = 0; i < QUEUE_SIZE; i++) {
        this->push(0);
    }
};

void Queue::fill_array(float arr[QUEUE_SIZE]) {
    int idx = 0;
    ListNode* current = this->head;

    while (current && idx < QUEUE_SIZE) {
        arr[idx] = current->val;
        current = current->next;
        idx++;
    }

    for (; idx < QUEUE_SIZE; idx++) {
        arr[idx] = 0;
    }
}

float Queue::pop() {
    if (!this->head) {
        return 0;
    }
    this->count--;

    ListNode* temp_node = this->head;
    float temp_f = temp_node->val;
    this->head = this->head->next;
    if (!this->head) {
        this->tail = nullptr;
    }

    delete temp_node;
    return temp_f;
};

void Queue::push(float f) {
    this->count++;
    if (!head) {
        this->head = new ListNode(f);
        this->tail = this->head;
    } else {
        ListNode* new_node = new ListNode(f);
        this->tail->next = new_node;
        this->tail = this->tail->next;
    }

    if (this->count > QUEUE_SIZE) {
        this->pop();
    }
};

void Queue::print_queue() {
    ListNode* current = this->head;

    std::cout << "Queue size: " << this->count << std::endl;
    while (current) {
        std::cout << current->val << std::endl;
        current = current->next;
    }
}