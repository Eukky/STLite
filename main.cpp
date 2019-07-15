#include <iostream>
#include "vector.h"
#include <vector>
using namespace std;
int main(){
    int n = 3;
    STLite::vector<int> a(n, 5);
    int c = a.capacity();
    cout << c << endl;
    a.push_back(4);
    c = a.capacity();
    cout << c << endl;
    for(int i = 0; i < a.size(); i++){
        cout << a[i] << " ";
    }
    return 0;
}