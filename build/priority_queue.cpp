#include "../include/priority_queue.h"
#include <stdexcept>

void PriorityQueue::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].priority > heap[parent].priority) break;
        std::swap(heap[index], heap[parent]);
        index = parent;
    }
}

void PriorityQueue::heapifyDown(int index) {
    int size = heap.size();
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;
        if (left < size && heap[left].priority < heap[smallest].priority) smallest = left;
        if (right < size && heap[right].priority < heap[smallest].priority) smallest = right;
        if (smallest == index) break;
        std::swap(heap[index], heap[smallest]);
        index = smallest;
    }
}

void PriorityQueue::insert(Event e) {
    heap.push_back(e);
    heapifyUp(heap.size() - 1);
}

Event PriorityQueue::extractMin() {
    if (heap.empty()) throw std::runtime_error("Heap is empty");
    Event min = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) heapifyDown(0);
    return min;
}

bool PriorityQueue::isEmpty() {
    return heap.empty();
}

int PriorityQueue::size() {
    return heap.size();
}