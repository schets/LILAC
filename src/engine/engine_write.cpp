#include "engineimp.h"
#include "writer/writer.h"
#include "utils/item_heads.hpp"
void engineimp::add_writer(const std::shared_ptr<writer>& wval){
    writers[index].push_back(wval);
}
//write data to disk and clear from ram
void engineimp::write_individual_dat(const std::list<std::shared_ptr<writer>>& dats, size_t ind){
    ofile << "index: " << ind << std::endl;
    for(auto& dat : dats){
        write_data(dat, ofile);
    }
    ofile << "&" << std::endl;
}
void engineimp::write_dat(){
    for(auto& writes : writers){
        write_individual_dat(writes.second, writes.first);
    }
    writers.clear();
}
void engineimp::write_data(std::shared_ptr<writer> dat, std::ofstream& ofile){
    dat->write(ofile);
    ofile << "&&" << std::endl;
}
