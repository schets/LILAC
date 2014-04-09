#include "defs.hpp"
#include "parser/item.h"
void* al_malloc(size_t ss){
    void* rv = fftw_malloc(ss);
    if(!rv){
        std::stringstream str;
        str << "Memory allocation of " << ss << " bytes of data failed";
        err(str.str(), "malloc", "defs.h", FATAL_ERROR);
    }
    return rv;
}
void al_free(void* ss){
    if(!ss){
        err("Attempt to free a null pointer", "free", "defs.h", WARNING);
    }
    fftw_free(ss);
}
void err(std::string message, std::string function, std::string file, _fatal f){
    std::cout<<"A fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\nExiting program\n";
    exit(1);
}
void err(std::string message, std::string function, std::string file, _warning w){
    std::cout<<"A non-fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\nContinuing program\n";
}
void err(std::string message, std::string function, std::string file, item* p, _fatal f){
    std::cout<<"A fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cout << "\nExiting program\n";
    exit(1);
}
void err(std::string message, std::string function, std::string file, item* p, _warning f){
    std::cout<<"A non-fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cout << "\nContinuing program\n";
}
void err(std::string message, std::string function, std::string file, std::shared_ptr<item> p, _fatal f){
    std::cout<<"A fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cout << "\nExiting program\n";
    exit(1);
}
void err(std::string message, std::string function, std::string file, std::shared_ptr<item> p, _warning f){
    std::cout<<"A non-fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cout << "\nContinuing program\n";
}


