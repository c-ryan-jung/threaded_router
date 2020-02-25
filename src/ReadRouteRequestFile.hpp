#ifndef READROUTEREQUESTFILE_HPP
#define READROUTEREQUESTFILE_HPP



#include <string>
#include <vector>

typedef std::pair <std::string,std::string> string_pair; 

extern unsigned int ReadRouteRequestPairs(const char* filename, std::vector<string_pair>& request_name_vector);

#endif