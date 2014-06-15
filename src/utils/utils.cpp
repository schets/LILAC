#include "defs.hpp"
#include "engine/item.h"
#include <map>
#include <set>
extern "C"{
#include <complex.h>
#include "eigen3/fftw3.h"
}

/*!
 * This function returns a pointer that is aligned to at least whatever
 * FFTTW requires for data alignment.
 *
 * \note The mempool class should almost always be used instead of direct calls to al_malloc
 * @param ss The number of bytes to allocate
 */
void* al_malloc(size_t ss){
    void* rv = fftw_malloc(ss);
    if(!rv){
        std::stringstream str;
        str << "Memory allocation of " << ss << " bytes of data failed";
        err(str.str(), "malloc", "defs.h", FATAL_ERROR);
    }
    return rv;
}

/*!
 * Frees memory that has been allocated by al_malloc
 *
 * \note Only call on data allocated by al_malloc, not malloc or new
 *
 * @param ss The pointer to be freed
 */
void al_free(void* ss){
    if(!ss){
        err("Attempt to free a null pointer", "free", "defs.h", WARNING);
    }
    fftw_free(ss);
}


/*!
 * Throws a fatal error, and prints the message, function name, and file name.
 * Afterwards, program exits.
 * err("something failed", "some_func", "folder/some_file.cpp", FATAL_ERROR)-->
 *  
 *  A fatal error has occured in functions some_func in file "folder/some_file.cpp"
 *  Error message is:
 *  Something failed
 *  Exiting program
 */
void err(std::string message, std::string function, std::string file, _fatal f){
    std::cerr<<"A fatal error has occurred in function "<<function<<" in file "<<file<<
        std::endl<<"Error message is:"<<std::endl<<message<<
        std::endl<<"Exiting program"<<std::endl;
    exit(1);
}

/*!
 * Throws a warning, and prints the message, function name, and file name.
 * Afterwards, program continues.
 * err("something failed", "some_func", "folder/some_file.cpp", WARNING)-->
 *  
 *  A non-fatal error has occured in functions some_func in file "folder/some_file.cpp"
 *  Error message is:
 *  Something failed
 *
 */
void err(std::string message, std::string function, std::string file, _warning w){
    std::cerr<<"A non-fatal error has occurred in function "<<function<<" in file "<<file<<
        std::endl<<"Error message is:"<<std::endl<<message<<std::endl;
}
void err(std::string message, std::string function, std::string file, const item* p, _fatal f){
    std::cerr<<"A fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cerr << "\nExiting program\n";
    exit(1);
}
void err(std::string message, std::string function, std::string file, const item* p, _warning f){
    std::cerr<<"A non-fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cerr << "\nContinuing program\n";
}
void err(std::string message, std::string function, std::string file, std::shared_ptr<item> p, _fatal f){
    std::cerr<<"A fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cerr << "\nExiting program\n";
    exit(1);
}
void err(std::string message, std::string function, std::string file, std::shared_ptr<item> p, _warning f){
    std::cerr<<"A non-fatal error has occurred in function "<<function<<" in file "<<file<<
        "\nError message is:\n"<<message<<"\n";
    p->print();
    std::cerr << "\nContinuing program\n";
}

/*!
 * Lifetime control for fftw_plans
 */
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
/*
 * This function calculates the non-normalized forwards fourier transform of complex input data, and stores it 
 * in an output array. These arrays can point to the same location, but they must not alias in any other fashion
 *
 * \sa ifft
 *
 * @param _in array an array pointing towards the input values
 * @param _out an array pointing towards the output locations (may be equal to input)
 * @param len length of the arrays
 */
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
    if((size_t)std::abs((std::ptrdiff_t)(_in - _out)) < len){
        err("Input arrays to fft alias each other, and are not equal", "fft", "utils/utils.cpp",
                FATAL_ERROR);
    }
    if(!out_place.count(len)){
        out_place[len].p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    }
    fftw_execute_dft(out_place[len].p, in, out);
}


/*
 * This function calculates the normalized backwards fourier transform of complex input data, and stores it 
 * in an output array. These arrays can point to the same location, but they must not alias in any other fashion
 * 
 * \sa fft
 * 
 * @param _in array an array pointing towards the input values
 * @param _out an array pointing towards the output locations (may be equal to input)
 * @param len length of the arrays
 */
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
        if((size_t)std::abs((std::ptrdiff_t)(_in - _out)) < len){
            err("Input arrays to ifft alias each other, and are not equal", "ifft", "utils/utils.cpp",
                    FATAL_ERROR);
        }
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

//!Cleans up any data held by the fft code
void fft_cleanup(){
    fftw_cleanup();
}

/*!
 * Trims specified characters from beginning and end of string
 * "   \n \r stuff-we want \t  " --> "stuff we want"
 *
 * @param curline The string to be trimmed. Is modified by the function.
 * @param val The characters to be trimmed, are whitespace by default
 */

void trim(std::string& curline, const std::string& val){
    size_t startpos = curline.find_first_not_of(val);
    if(startpos != std::string::npos){
        curline=curline.substr(startpos);
    }
    startpos = curline.find_last_not_of(val);
    if(startpos != std::string::npos){
        curline=curline.substr(0, startpos+1);
    }
}

/*
 * Splits the input string into two at the specified delimiting character
 * With the delimiting string " ",
 * "relevant1  garbage" --> "relevant1", "garbage"
 *
 * Calling token with a string that doesn't contain the delim will result in
 * token containing the entirety of the string and str being erased
 * 
 * "stuffwithnospaces" -> "stuffwithnospaces", ""
 *
 * @param tok The string in which the left token will be placed
 * @param str The initial string, will have the left token removed
 * @param delim The string at which the input string is to be broken
 */
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

/*!
 * Returns a name that has not been created previously be the function, and should not exist in the engine
 * Will almost certainly not be human-readable or printable on a terminal
 *
 * @param base The base string with which to generate the new name
 * @return A unique name
 */
std::string get_unique_name(std::string base){
    static __HIDER__::_unique_name nn;
    return nn.get_unique_name(base);
}
