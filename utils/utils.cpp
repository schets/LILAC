#include "defs.hpp"
#include "engine/item.h"
#include <map>
#include <set>
extern "C"{
#include <complex.h>
#include "eigen3/fftw3.h"
}
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
class fftw_plan_holder{
    public: 
        fftw_plan p;
        fftw_plan_holder():p(0){}
        fftw_plan_holder(fftw_plan h):p(h){}
        ~fftw_plan_holder(){
            if(p){
                fftw_destroy_plan(p);
            }
        }
};
void fft(comp* _in, comp* _out, const size_t len){
    fftw_complex* in = (fftw_complex*)_in;
    fftw_complex* out = (fftw_complex*)_out;
    static std::map<size_t, fftw_plan_holder> in_place, out_place;
    if(in == out){
        if(!in_place.count(len)){
            in_place[len].p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        }
        fftw_execute_dft(in_place[len].p, in, out);
        return;
    }
    if(!out_place.count(len)){
        out_place[len].p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    }
    fftw_execute_dft(out_place[len].p, in, out);
}
void ifft(comp* _in, comp* _out, const size_t len){
    fftw_complex*  in = (fftw_complex*)_in;
    fftw_complex*  out = (fftw_complex*)_out;
    static std::map<size_t, fftw_plan_holder> in_place, out_place;
    if(in == out){
        if(!in_place.count(len)){
            in_place[len].p = fftw_plan_dft_1d(len, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
        }
        fftw_execute_dft(in_place[len].p, in, out);
    }
    else{
        if(!out_place.count(len)){
            out_place[len].p = fftw_plan_dft_1d(len, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
        }
        fftw_execute_dft(out_place[len].p, in, out);
    }
    double nval;
    nval=1.00/len;
    for(size_t i = 0; i < len; i++){
        _out[i]*=nval;
    }
}

void fft_cleanup(){
    fftw_cleanup();
}

void trim(std::string& curline){
    size_t startpos = curline.find_first_not_of("\n\r\t ");
    if(startpos != std::string::npos){
        curline=curline.substr(startpos);
    }
    startpos = curline.find_last_not_of("\n\r\t ");
    if(startpos != std::string::npos){
        curline=curline.substr(0, startpos+1);
    }
}
void trim(std::string& curline, char val){
    size_t startpos = curline.find_first_not_of(val);
    if(startpos != std::string::npos){
        curline=curline.substr(startpos);
    }
    startpos = curline.find_last_not_of(val);
    if(startpos != std::string::npos){
        curline=curline.substr(0, startpos+1);
    }
}
void ltoken(std::string& tok, std::string& str, std::string delim){
    size_t tpos = str.find(delim);
    tok=str.substr(0, tpos);
    if(tpos == std::string::npos || tpos+delim.length() >= str.size()){
        str.clear();
    }
    else{
        str.erase(0, tpos + delim.length());
    }
}
namespace __HIDER__{
    //!Create a unique name that cannot already exist in the engine, should never reference directly
    class _unique_name{
        std::set<std::string> names;
        public:
        std::string get_unique_name(std::string nbase){
            nbase.push_back('_');
            while(names.count(nbase)){
                nbase.push_back(1 | (char)(rand()));
            }
            names.insert(nbase);
            //ensure that the name will not be alread in the engine
            nbase.push_back('!');
            return nbase;
        }
        _unique_name(){
            srand(time(0));
        }
    };
}
std::string get_unique_name(std::string base){
    static __HIDER__::_unique_name nn;
    return nn.get_unique_name(base);
}
