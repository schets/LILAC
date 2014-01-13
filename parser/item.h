#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <map>
#include <vector>


class item{
    std::vector<std::string> deps;
    std::string _name;
    public:
    virtual void parse(const std::string& inval) = 0;
    virtual void retrieve(void* inval) = 0;
    virtual void postprocess(std::map<std::string, item*>& indat){};
    inline const std::vector<std::string>& dependencies() const{
        return deps;//deps is initialized in constructors
    }
    const inline std::string& name() const{
        return _name;
    }
    virtual ~item(){}
};
class real:public item{
    double value;
    public:
        virtual void parse(const std::string& inval);
        virtual void retrieve(void* inval);
};
class integer:public item{
    int value;
    public:
        virtual void parse(const std::string& inval);
        virtual void retrieve(void* inval);
};
class string:public item{
    std::string value;
    public:
    virtual void parse(const std::string& inval);
    virtual void retrieve(void* inval);
};
typedef std::map<std::string, item*> data_str;
#endif

