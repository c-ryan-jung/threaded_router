#include "ReadRouteRequestFile.hpp"
#include <iostream>

using namespace std;

typedef std::pair <std::string,std::string> string_pair;

void test1(){
    cout<<"Testing Basic function: ";
    vector<string_pair> request_name_vector;
    if(ReadRouteRequestPairs("testFile.txt", request_name_vector) == 0){
        cout<<"Test Succesful"<<endl;
    }else{
        cout<<"Test Failed"<<endl;
    }
}
void test2(){
    cout<<"Testing file not exist: "<<endl;
    vector<string_pair> request_name_vector;
    if(ReadRouteRequestPairs("DoesNotExist.txt", request_name_vector) == 1){
        cout<<"Test Succesful"<<endl;
    }else{
        cout<<"Test Failed"<<endl;
    }
}
void test3(){
    cout<<"Testing bad file pair check: "<<endl;
    vector<string_pair> request_name_vector;
    if(ReadRouteRequestPairs("badFile.txt", request_name_vector) == 1){
        cout<<"Test Succesful"<<endl;
    }else{
        cout<<"Test Failed"<<endl;
    }

}

int main(){
    test1();
    test2();
    test3();
    return 0;
}

