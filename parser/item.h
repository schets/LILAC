#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <map>
#include <vector>


class item{
    std::string _name;
    public:
    static item* create(const std::string name);
    virtual void print();
    virtual void parse(std::string inval) = 0;
    virtual void retrieve(void* inval){}; //need to make retrievable class
    virtual void postprocess(std::map<std::string, item*>& indat){};
    virtual std::vector<std::string> dependencies() const = 0;
    virtual std::string type() const = 0;
    virtual ~item(){}
    void setname(const std::string n);
    const std::string& name()const ;
};

#endif

