#ifndef _STLITE_VECTOR_TEST_H_
#define _STLITE_VECTOR_TEST_H_

#include "../vector.h"

#include <vector>
#include <iostream>

namespace STLite{
    namespace VectorTest{

        template <class T>
        using std_vec = std::vector<T>;

        template <class T>
        using stlite_vec = STLite::vector<T>;

        void test_case1();
    }
}

#endif