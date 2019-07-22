#ifndef _STLITE_VECTOR_TEST_H_
#define _STLITE_VECTOR_TEST_H_

#include "../stlite/vector.h"

#include <vector>
#include <iostream>
#include <algorithm>

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
            std_vec<int> v1(10, 1);
            stlite_vec<int> v2(10, 1);
            for(int i = 0; i < v1.size(); ++i){
                cout << v1[i] << " ";
            }
        }

        void test_case2(){
            
        }
    }
}

#endif