/*
Copyright (c) 2014, Sam Schetterer, Nathan Kutz, University of Washington
Authors: Sam Schetterer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#include "defs.hpp"
#include "engine/item.h"
#include <map>
#include <set>
extern "C"{
#include <complex.h>
#include <acml.h>
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

//Since everything has fftw wrappers, I'm writing in terms of that although we use the acml functions

/*!
 * Lifetime control for fft_plans
 */
class fft_plan_holder{
    public: 
        doublecomplex* comm;
        int info;
        size_t len;
        fft_plan_holder():comm(0){}
        fft_plan_holder(size_t n):comm(new doublecomplex[3*n+100]), len(n){
            zfft1dx(0, 1.0, 0, (int)n, 0, 1, 0, 1, comm, &info);
        }
        fft_plan_holder(const fft_plan_holder& cpy):comm(new doublecomplex[cpy.len*3+100]), len(cpy.len){
            zfft1dx(0, 1.0, 0, (int)len, 0, 1, 0, 1, comm, &info);
        }
        ~fft_plan_holder(){
            if(comm){
                delete[] comm;
            }
        }
};

//!Actual fft function that fft and ifft wrap
void _fft(comp* _in, comp* _out, const size_t len, int dir, double scale){
    doublecomplex* in = (doublecomplex*)_in;
    doublecomplex* out = (doublecomplex*)_out;
    static std::map<size_t, fft_plan_holder> fft_plans;
    if(in != out && (size_t)std::abs((std::ptrdiff_t)(_in - _out)) < len){
        err("Input arrays to fft alias each other, and are not equal", "fft", "utils/utils.cpp",
                FATAL_ERROR);
    }
    if(fft_plans.count(len)==0){
        fft_plans.insert(std::pair<size_t, fft_plan_holder>(len, fft_plan_holder(len)));
    }
    auto& fplan = fft_plans[len];
    zfft1dx(dir, scale, in==out, (int)len, in, 1, out, 1, fplan.comm, &(fplan.info));
}
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
void fft(comp* in, comp* out, const size_t len){
    _fft(in, out, len, -1, 1.0);
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
void ifft(comp* in, comp* out, const size_t len){
    _fft(in, out, len, 1, 1.0/len);
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
