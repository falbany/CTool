

#include <vector>
#include <stdexcept>

namespace ct {
    /**
     * @brief Converts raw C-arrays to STL vectors.
     */
    template <typename T>
    std::vector<T> vectorize(T* arr, size_t size) {
        if (!arr) throw std::invalid_argument("Null array pointer.");
        return std::vector<T>(arr, arr + size);
    }
}    // namespace ct
