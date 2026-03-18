#pragma once
#ifndef CT_CORE_HPP
    #define CT_CORE_HPP

    #include <vector>
    #include <stdexcept>

/**
 * @namespace ct
 * @brief The core namespace encapsulating all CTool utilities.
 */
namespace ct {

    /**
     * @brief Converts a C-style array to an `std::vector`.
     * * This function safely copies elements from a raw C-style array into a 
     * managed C++ standard vector.
     * @tparam T The data type of the array elements.
     * @param arr Pointer to the first element of the C-style array.
     * @param size The total number of elements in the array.
     * @return std::vector<T> A newly constructed vector containing the copied elements.
     * @throws std::invalid_argument If the input pointer `arr` is `nullptr`.
     */
    template <typename T>
    std::vector<T> vectorize(T* arr, size_t size);

}    // namespace ct

    #include "ct_core.tpp"

#endif