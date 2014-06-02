#include <iostream>
using namespace std;
int& test(int& inval){
    return inval;
}
auto test2(int& inval) -> decltype(test(inval)){
    return test(inval);
}
int main(){
    int x = 1;
    cout<<x<<endl;
    int& try1=test(x);
    cout<<try1<<endl;
    x=2;
    cout<<try1<<endl;
    x=1;
    int &try2=test2(x);
    cout<<try2<<endl;
    x=2;
    cout<<try2<<endl;
    try2=3;
    cout<<x<<endl;
    return 0;
}
