#ifndef FLIGHT_OPTIMIZER_FLIGHTHEAP_H
#define FLIGHT_OPTIMIZER_FLIGHTHEAP_H
#include <vector>
#include <functional>
#include <cstddef>
#include "../models/FlightOffer.h"

/// @class FlightHeap
/// @brief A custom min-heap data structure that stores FlightOffer objects
///        with an injected comparator for flexible ordering.
class FlightHeap {
private:
    std::vector<FlightOffer> data;
    std::function<bool(const FlightOffer&, const FlightOffer&)> comparator;

    /// @brief Restores heap property by moving an element up the tree
    /// @param index The index of the element to move up
    void heapUp(int index);

    /// @brief Restores heap property by moving an element down the tree
    /// @param index The index of the element to move down
    void heapDown(int index);

    /// @brief Calculates the parent index for a given node index
    /// @param i The child index
    /// @return The parent index
    int parentIndex(int i);

    /// @brief Calculates the left child index for a given node index
    /// @param i The parent index
    /// @return The left child index
    int leftChild(int i);

    /// @brief Calculates the right child index for a given node index
    /// @param i The parent index
    /// @return The right child index
    int rightChild(int i);

public:
    /// @brief Constructs a FlightHeap with an injected comparator
    /// @param comp A function that returns true if the first element should come before the second
    explicit FlightHeap(std::function<bool(const FlightOffer&, const FlightOffer&)> comp);

    /// @brief Inserts a new FlightOffer into the heap
    /// @param offer The FlightOffer to insert
    void push(FlightOffer offer);

    /// @brief Removes the top (best) element from the heap
    /// @throws std::runtime_error if the heap is empty
    FlightOffer pop();

    /// @brief Returns a reference to the top (best) element in the heap
    /// @return A const reference to the top element
    /// @throws std::runtime_error if the heap is empty
    const FlightOffer& top() const;

    /// @brief Checks if the heap is empty
    /// @return true if the heap contains no elements, false otherwise
    bool isEmpty() const;

    /// @brief Returns the number of elements in the heap
    /// @return The size of the heap
    std::size_t size() const;
};


#endif //FLIGHT_OPTIMIZER_FLIGHTHEAP_H