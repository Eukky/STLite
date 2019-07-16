#include <iostream>
#include "vector.h"
#include <vector>
using namespace std;
int main(){
    int n = 3;
    STLite::vector<int> a(n, 5);
    STLite::vector<int> &b = a;
    for(int i = 0; i < b.size(); i++){
        cout << b[i] << " ";
    }
    return 0;
}