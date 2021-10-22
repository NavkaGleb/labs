#pragma once

#include <cmath>
#include <limits>
#include <algorithm>

template <typename T>
bool POINTER_EQ(const T* a, const T* b) {

    if (a == nullptr && b == nullptr)
        return true;

    if (a == nullptr || b == nullptr)
        return false;

    return *a == *b;

}