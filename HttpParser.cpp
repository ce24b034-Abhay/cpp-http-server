#include "HttpParser.h"
#include <sstream>

void parseHttpRequest(const std::string& request, std::string& method, std::string& route) 
{
    std::istringstream iss(request);
    std::string http_version;
    // Extract method, route, and version from the first line
    iss >> method >> route >> http_version;
}