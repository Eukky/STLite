#ifndef _STLITE_VECTOR_TEST_H_
#define _STLITE_VECTOR_TEST_H_

#include "../stlite/vector.h"
#include "test_util.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
#include <string>
#include <cassert>
using namespace std;
namespace STLite{
    namespace VectorTest{

        template <class T>
        using std_vec = std::vector<T>;

        template <class T>
        using stlite_vec = STLite::vector<T>;

        void test_case();
        void test_case1();
        void test_case2();


        void test_case(){
            test_case1();
            test_case2();
        }


        void test_case1(){
            std_vec<std::string> v1(10, "abc");
			stlite_vec<std::string> v2(10, "abc");
            STLite::Test::print_container(v1, "v1");
            STLite::Test::print_container(v2, "v2");

			std_vec<std::string> v3(10);
			stlite_vec<std::string> v4(10);
            STLite::Test::print_container(v3, "v3");
            STLite::Test::print_container(v4, "v4");

			std::array<std::string, 3> arr = { "abc", "def", "ghi" };
			std_vec<std::string> v5(std::begin(arr), std::end(arr));
			stlite_vec<std::string> v6(std::begin(arr), std::end(arr));
			STLite::Test::print_container(v5, "v5");
            STLite::Test::print_container(v6, "v6");
        }

        void test_case2(){
            
        }
    }
}

#endif