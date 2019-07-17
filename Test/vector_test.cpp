#include "vector_test.h"
#include <iostream>
using namespace std;
namespace STLite{
    namespace VectorTest{

        void test_case1(){
            std_vec<int> v1(10, 1);
            stlite_vec<int> v2(10, 1);
            for(int i = 0; i < v1.size(); ++i){
                cout << v1[i] << " ";
            }
        }
    }
}