#include <cassert>

template <typename T, T max, T min>
static constexpr T constrain(const T input) {
    static_assert(max > min);
    return (input > max ? max : (input < min ? min : input));
}

template <typename T>
static constexpr T constrain(const T input, T max, T min) {
    assert(max > min);
    return (input > max ? max : (input < min ? min : input));
}