#include "engineimp.h"
#include "writer/writer.h"
#include "utils/retrieve_checker.hpp"
void engineimp::add_writer(std::shared_ptr<const writer> wval){
    dats.push_back(wval);
}
//write data to disk and clear from ram
void engineimp::write_dat(){
    std::string oname;
    retrieve(oname, values["!out_file"], 0);
    std::ofstream ofile(oname.c_str(), std::ofstream::app);
    for(auto& dat : dats){
        write_data(dat, ofile);
    }
    ofile.close();
    dats.clear();
}
void engineimp::write_data(std::shared_ptr<const writer> dat, std::ofstream& ofile){
    dat->write(ofile);
    ofile << "****\n";
}
