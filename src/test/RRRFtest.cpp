#include "../ReadRouteRequestFile.hpp"
#include <iostream>

using namespace std;

//typedef std::pair <std::string,std::string> string_pair;

int test1(const char* filename = "testFile.txt"){
  cout<<"Test1: file <" << filename << ">:" << endl;

  vector<string_pair> request_name_vector;
  if(ReadRouteRequestPairs(filename, request_name_vector) == 0){
    cout << "\tSuccess" <<endl;
    return 0;
  }else{
    cout<<"\tFail"<<endl;
    return 1;
  }
}

int test2(const char* filename = "DoesNotExist.txt"){
  cout<<"Test2 [file exists]: file <" << filename << ">:" << endl;

  vector<string_pair> request_name_vector;
  if(ReadRouteRequestPairs(filename, request_name_vector) == 0){
    cout<<"\tSuccess"<<endl;
    return 0;
  }else{
    cout<<"\tFail"<<endl;
    return 1;
  }
}

int test3(const char* filename = "badFile.txt"){
  cout<<"Test3 [bad pair]: file <" << filename << ">:" << endl;

  vector<string_pair> request_name_vector;
  if(ReadRouteRequestPairs("badFile.txt", request_name_vector) == 0){
    cout<<"\tSuccess"<<endl;
    return 0;
  }else{
    cout<<"\tFail"<<endl;
    return 1;
  }

}

int main(int argc, const char* argv[]){
  if( argc < 2 ){
    cout << "Missing argument(s)." << endl;
    return 1;
  }

  unsigned int testVector[4] = {0};
  unsigned int answerVector[4] = {1,1,1,1};

  const char* filename = argv[1];

  testVector[1] = test1(filename);
  testVector[2] = test2();
  testVector[3] = test3();

  unsigned int numFails = 0;
  for(int i=1; i<4; ++i){
    cout << i << ' ' << testVector[i] << ' ' << answerVector[i] << endl;
    if( testVector[i] != answerVector[i] )
      numFails+=1;
  }

  if( numFails > 0 ){
    cout << "One or more tests failed" << endl;
    return 1;
  }
  else {
    cout << "All tests passed" << endl;
    return 0;
  }

}
