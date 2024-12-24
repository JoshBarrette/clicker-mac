#pragma once

const int QUEUE_SIZE = 20;

class ListNode {
   public:
    ListNode() {}
    ListNode(float f) {
        this->val = f;
    };
    ListNode(float f, ListNode* n) {
        this->val = f;
        this->next = n;
    };

    float val;
    ListNode* next = nullptr;
};

class Queue {
   public:
    Queue();
    void fill_array(float arr[QUEUE_SIZE]);
    float pop();
    void push(float f);
    void print_queue();

   private:
    float arr[QUEUE_SIZE];
    int count = 0;
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
};