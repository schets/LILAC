#pragma once
#ifndef PRINTABLE_HPP
#define PRINTABLE_HPP
class printable{
    public:
        virtual void print() const = 0;
        virtual ~printable(){};
};
#endif
