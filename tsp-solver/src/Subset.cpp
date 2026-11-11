#include "Subset.hpp"
#include <stdexcept>

namespace subset {

Subset::Subset(std::size_t k, std::size_t n) : n(n) {
    if (n > maxSize) {
        throw std::invalid_argument("A subset can have at most 64 elements");
    }
    if (k > n) {
        throw std::invalid_argument("A subset cannot be larger than its set");
    }
    // the k lowest bits set to 1
    bits = (k == maxSize) ? ~0ULL : (1ULL << k) - 1;
}

bool Subset::contains(std::size_t i) const {
    if (i >= n) {
        throw std::out_of_range("Element out of range");
    }
    return (bits >> i) & 1ULL;
}

void Subset::add(std::size_t i) {
    if (i >= n) {
        throw std::out_of_range("Element out of range");
    }
    bits |= 1ULL << i;
}

void Subset::minus(std::size_t i) {
    if (i >= n) {
        throw std::out_of_range("Element out of range");
    }
    bits &= ~(1ULL << i);
}

unsigned long long Subset::index() const {
    return bits;
}

Subset::ElementsIterator::ElementsIterator(unsigned long long remaining) : remaining(remaining) {}

std::size_t Subset::ElementsIterator::operator*() const {
    // position of the lowest bit set to 1
    std::size_t position = 0;
    while (((remaining >> position) & 1ULL) == 0) {
        position++;
    }
    return position;
}

Subset::ElementsIterator& Subset::ElementsIterator::operator++() {
    // removes the lowest bit set to 1
    remaining &= remaining - 1;
    return *this;
}

bool Subset::ElementsIterator::operator!=(const ElementsIterator& other) const {
    return remaining != other.remaining;
}

Subset::ElementsIterator Subset::begin() const {
    return ElementsIterator(bits);
}

Subset::ElementsIterator Subset::end() const {
    // no element left to visit
    return ElementsIterator(0);
}

std::ostream& operator<<(std::ostream& output, const Subset& s) {
    output << "{";
    bool first = true;
    for (std::size_t element : s) {
        output << (first ? "" : ",") << element;
        first = false;
    }
    return output << "}";
}

}
