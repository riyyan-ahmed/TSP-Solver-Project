#pragma once
#include <cstddef>
#include <ostream>


namespace subset {
    /** Subset of {0, ..., n - 1} stored as the bits of a 64-bit integer: bit i is 1 when i is in the subset. */
    class Subset {
        unsigned long long bits = 0;
        std::size_t n = 0;

        friend class SubsetIterator;

        public:
            /** Largest set that fits in the 64 bits */
            static constexpr std::size_t maxSize = 64;

            /** Empty subset of the empty set */
            Subset() = default;

            /** Subset {0, ..., k - 1} of {0, ..., n - 1} : throws if k > n or n > maxSize */
            Subset(std::size_t k, std::size_t n);

            /** True if i is in the subse*/
            bool contains(std::size_t i) const;

            /** Adds i to the subset */
            void add(std::size_t i);

            /** Removes i from the subset */
            void minus(std::size_t i);

            /** The bits read as one number: two different subsets never have the same index */
            unsigned long long index() const;

            /** Goes through the elements of a subset in increasing order */
            class ElementsIterator {
                unsigned long long remaining;

                public:
                    explicit ElementsIterator(unsigned long long remaining);

                    /** The smallest element not visited yet */
                    std::size_t operator*() const;

                    /** Moves to the next element */
                    ElementsIterator& operator++();

                    bool operator!=(const ElementsIterator& other) const;
            };

            /** First element, so a subset can be used in a range-based for loop */
            ElementsIterator begin() const;

            /** Position after the last element */
            ElementsIterator end() const;

            /** Prints the subset as {0,3} */
            friend std::ostream& operator<<(std::ostream& output, const Subset& s);
    };

    /** Goes through all the subsets of size k of {0, ..., n - 1}, using Gosper's hack */
    class SubsetIterator {
        Subset current;
        std::size_t k = 0;
        bool finished = false;

        SubsetIterator() = default;

        public:
            /** Start at the subset {0, ..., k - 1} */
            SubsetIterator(std::size_t n, std::size_t k);

            /** The value every SubsetIterator reaches after its last subset. */
            static SubsetIterator end();

            /** The current subset. */
            const Subset& operator*() const;

            /** Moves to the next subset of size k. */
            SubsetIterator& operator++();

            bool operator!=(const SubsetIterator& other) const;
    };
}
