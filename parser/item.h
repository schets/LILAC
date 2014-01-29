#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <map>
#include <vector>
#include <iostream>
class engineimp;
class item{
    std::string _name;
    engineimp* holder;
    public:
    static item* create(std::string name, engineimp* rval);
    virtual void print() const;
    virtual void parse(std::string inval) = 0;
    virtual void retrieve(void* inval){std::cout << "calling item retr" << std::endl;}; //need to make retrievable class
    virtual void postprocess(std::map<std::string, item*>& indat){};
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const = 0;
    virtual ~item(){}
    void setname(const std::string n);
    const std::string& name()const ;
};

//class that describes items with a dimension
class item_dim:public item{
    public:
        size_t dimension;
        virtual void postprocess(std::map<std::string, item*>& dat);
        virtual std::vector<std::string> dependencies() const = 0;
};
#endif

