//
// Created by Natalie McWhorter on 4/14/26.
//

#include "FlightHeap.h"

#include <utility>

using namespace std;

//CONSTRUCTOR
FlightHeap::FlightHeap(std::function<bool(const FlightOffer&, const FlightOffer&)> comp)
    : comparator(std::move(comp)) {
}

//PUBLIC METHODS
void FlightHeap::push(FlightOffer offer) {
    data.push_back(offer);
    heapUp(data.size() - 1);
}

FlightOffer FlightHeap::pop() {
    if (data.empty()) throw runtime_error("Heap is empty");
    FlightOffer temp = data[0];
    data[0] = data.back();
    data.pop_back();
    heapDown(0);
    return temp;
}

const FlightOffer& FlightHeap::top() const {
    if (data.empty()) throw runtime_error("Heap is empty");
    return data[0];
}

bool FlightHeap::isEmpty() const {
    return (data.empty());
}

size_t FlightHeap::size() const {
    return data.size();
}


//PRIVATE METHODS

void FlightHeap::heapUp(int i) {
    while (i>0) {
        if (comparator(data[i], data[parentIndex(i)])) {
            swap(data[parentIndex(i)], data[i]);
            i = parentIndex(i);
        }
        else {
            break;
        }
    }
}

void FlightHeap::heapDown(int i) {
    int maxIndex = i;
    int left = leftChild(i);
    int right = rightChild(i);
    if (left < data.size() && comparator(data[left], data[maxIndex])) {
        maxIndex = left;
    }
    if (right < data.size() && comparator(data[right], data[maxIndex])) {
        maxIndex = right;
    }
    if (maxIndex != i) {
        swap(data[maxIndex], data[i]);
        heapDown(maxIndex);
    }
}

int FlightHeap::parentIndex(int i) {
    return (i - 1) / 2;
}

int FlightHeap::leftChild(int i) {
    return 2 * i + 1;
}

int FlightHeap::rightChild(int i) {
    return 2 * i + 2;
}
