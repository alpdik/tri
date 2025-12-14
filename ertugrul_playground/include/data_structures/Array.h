#pragma once

#include <cstddef>

namespace data_structures {
    template<typename T, size_t S>
    class Array {
        public:
            constexpr size_t Size() noexcept { return S; }

            T& operator[](size_t index) { return m_Data[index]; }
            const T& operator[](size_t index) const { return m_Data[index]; }
            
            T* Data() { return m_Data; }
            const T* Data() const { return m_Data; }

        private:
            T m_Data[S];

    };
}