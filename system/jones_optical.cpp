#include "jones_optical.h"
void jones_optical::postprocess(std::map<std::string, item*>& invals){
    rh = rhs::create("CNLS");
}
