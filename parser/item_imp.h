#ifndef ITEM_IMP_H
#define ITEM_IMP_H
class real:public item{
    double value;
    public:
    virtual std::vector<std::string> dependencies() const;
    virtual  std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval) const ;
};
class string:public item{
    std::string value;
    public:
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval) const ;
};
class test: public real{
    public: 
        virtual std::string type() const;
        virtual std::vector<std::string> dependencies() const;
};
#endif
