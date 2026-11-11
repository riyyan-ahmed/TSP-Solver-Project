#pragma once
#include <cstddef>
#include <ostream>

namespace subset {

  
    class Subset {
        unsigned long long bits = 0;
        std::size_t n = 0;
        friend class SubsetIterator;


        public:
           
            static constexpr std::size_t maxSize = 64;

            Subset() = default;


            Subset(std::size_t k, std::size_t n);

            bool contains(std::size_t i) const;
            void add(std::size_t i);
            void minus(std::size_t i);


            unsigned long long index() const;

          
            class ElementsIterator {
                unsigned long long remaining;  // elements not visited yet

                public:
                    explicit ElementsIterator(unsigned long long remaining);
                    std::size_t operator*() const;
                    ElementsIterator& operator++();
                    bool operator!=(const ElementsIterator& other) const;
            };
             
            ElementsIterator begin() const;
            ElementsIterator end() const;

            friend std::ostream& operator<<(std::ostream& output, const Subset& s);
    };

    // goes through all the subsets of size k of {0, ..., n-1}
    class SubsetIterator {
        Subset current;
        std::size_t k = 0;
        bool finished = false;

        SubsetIterator() = default;

        public:
            SubsetIterator(std::size_t n, std::size_t k);

            static SubsetIterator end();

            const Subset& operator*() const;
            SubsetIterator& operator++();
            bool operator!=(const SubsetIterator& other) const;
    };



}
