#ifndef ITEM_IMP_H
#define ITEM_IMP_H
class real:public item{
    double value;
    public:
    virtual void print() const;
    virtual std::vector<std::string> dependencies() const;
    virtual  std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval);
};
class string:public item{
    std::string value;
    public:
    virtual void print() const;
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval);
};
class test: public real{
    public: 
        virtual std::string type() const;
        virtual std::vector<std::string> dependencies() const;
};
class integer:public item{
    int value;
    public:
    virtual void print() const;
    virtual std::vector<std::string> dependencies() const;
    virtual std::string type() const;
    virtual void parse(std::string inval);
    virtual void retrieve(void* inval);
};
#endif
