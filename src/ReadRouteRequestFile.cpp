#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include "ReadRouteRequestFile.hpp"

using namespace std;

bool fileCheck(const char* filename){
  if ( filename == 0 )
    return false;

  struct stat buffer;
  return (stat (filename, &buffer) == 0) ? true : false;
}

unsigned int ReadRouteRequestPairs(const char* filename,
				   vector<string_pair>& request_name_vector){
  if ( filename == 0 )
    return 1;

  ifstream file(filename);

  string line;
  string word;
  vector<string> things;
  bool first = true;
  if( !file.is_open() || file.bad() ){
    cout << "\tReadRouteRequestPairs: Error opening file <"
	 << filename << "> line: " << __LINE__ << endl;
    return 1;
  }

  while(getline(file, line)){
    if(first){
      first = false;
      continue;
    }
    istringstream myline(line);
    while(myline.good()){
      string substr;
      getline(myline,substr, ',');
      things.push_back(substr);
    }
  }
  for(int i = 0; i < things.size(); i+=2){
    if(fileCheck(things[i].c_str())){
      //HSM: why use printf?
      //printf("%s",things[i].c_str()); //for testing purposes
      //printf("%s",things[i+1].c_str());
      string_pair newPair(things[i],things[i+1]);
      request_name_vector.push_back(newPair);
    }else{
      cout<<"\tReadRouteRequestPairs: Non-existing file specified or invalid entry. <"
	  << things[i].c_str() << ">" << endl;
      return 1;
    }
  }
  return 0;
}
