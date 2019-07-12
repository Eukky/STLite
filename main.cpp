#include <iostream>
#include "vector.h"
using namespace std;
int main(){
    STLite::vector<int> a(3, 5);
    for(int i = 0; i < a.size(); i++){
        cout << a[i] << " ";
    }
    return 0;
}