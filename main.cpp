#include <iostream>
#include "Test/vector_test.h"
#include "vector.h"
using namespace std;
int main(){
    // STLite::VectorTest::test_case1();
    STLite::vector<int> a(10,10);
    a.insert(a.begin(),3);
    for(int i = 0; i < a.size(); ++i){
        cout << a[i] << " ";
    }
    return 0;
}