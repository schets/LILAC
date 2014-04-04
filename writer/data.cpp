#include "data.h"
data::to_string(){
    return name + std::string(": ") + extra_info + "\n" + dat.str();
}
