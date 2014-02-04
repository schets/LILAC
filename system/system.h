#ifndef SYSTEM_H
#define SYSTEM_H
#include "defs.h"
#include "parser/item.h"
class system:public item{
    public:
        virtual void checkpoint();
        virtual void update()
};
#endif
