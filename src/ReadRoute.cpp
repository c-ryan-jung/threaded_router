#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

typedef std::pair <std::string,std::string> string_pair; 

unsigned int ReadRouteRequestPairs(const char* filename, vector<string_pair>& request_name_vector){
    ifstream file(filename);
    
    string line;
    string word;
    vector<string> things;
    bool first = true;
    if(!file.is_open()){
      cout << "There was an error opening the file of Request Pairs." << endl;
      cout << "Filename given: " << filename << endl;
      return(1);
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
        pair<string,string> newPair(things[i],things[i+1]);
        request_name_vector.push_back(newPair);
    }
    return 0;
}