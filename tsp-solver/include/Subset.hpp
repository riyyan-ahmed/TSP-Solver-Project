#pragma once
#include <cstddef>
#include <ostream>

namespace subset {

  
    class Subset {
        unsigned long long bits = 0;
        std::size_t n = 0;

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

}
