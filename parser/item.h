#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <map>
#include <vector>


class item{
    std::string _name;
    public:
    virtual void parse(const std::string& inval) = 0;
    virtual void retrieve(void* inval) const = 0;
    virtual void postprocess(std::map<std::string, item*>& indat){};
    virtual std::vector<std::string> dependencies() const = 0;
    virtual std::string type() const = 0;
    virtual ~item(){}
    void setname(const std::string n);
    const std::string& name()const ;
};
class real:public item{
    double value;
    public:
        virtual std::vector<std::string> dependencies() const;
        virtual  std::string type() const;
        virtual void parse(const std::string& inval);
        virtual void retrieve(void* inval) const ;
};
class string:public item{
    std::string value;
    public:
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual void parse(const std::string& inval);
    virtual void retrieve(void* inval) const ;
};
typedef std::map<std::string, item*> data_str;
#endif

