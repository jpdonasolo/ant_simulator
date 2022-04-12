#pragma once

#include <string>


const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string reset("\033[0m");
const int TOTALCOLORS = 5;

std::string color(int idx)
{   
    if(idx==0){ return red; };
    if(idx==1){ return green; };
    if(idx==2){ return yellow; };
    if(idx==3){ return cyan; };
    if(idx==4){ return magenta; };
}