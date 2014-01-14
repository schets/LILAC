#ifndef ENGINEIMP_H
#define ENGINEIMP_H
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "item.h"
class engine;
class engineimp{
    //wrapper class for item pointer
    //facilitates easier memory management
    class iptr{
        public:
            item* ptr;
            iptr(item* p):ptr(p){}
            iptr():ptr(0){}
            ~iptr(){
                if(ptr){
                    delete ptr;
                }
            }
            inline operator item*(){
                return ptr;
            };
    };
    std::map<std::string, iptr> values;
    void read(std::ifstream& fstr);
    public:
        engineimp(const std::string fname);
        friend class engine;
};

#endif
