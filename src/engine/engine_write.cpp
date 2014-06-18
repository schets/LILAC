#include "engineimp.h"
#include "writer/writer.h"
#include "utils/item_heads.hpp"
void engineimp::add_writer(const std::shared_ptr<writer>& wval){
    writers[index].push_back(wval);
}
//write data to disk and clear from ram
void engineimp::write_individual_dat(const std::list<std::shared_ptr<writer>>& dats, size_t ind, std::ostream& out_stream){
	ofile << "index: " << ind << std::endl;
	for(auto& dat : dats){
		write_data(dat, out_stream);
	}
	out_stream << "&\n";
}
void engineimp::write_dat(){
	std::stringstream out_stream;
	for(auto& writes : writers){
		write_individual_dat(writes.second, writes.first, out_stream);
	}
	ofile << out_stream;
	writers.clear();
}
void engineimp::write_data(std::shared_ptr<writer> dat, std::ostream& wfile){
	dat->write(wfile);
	wfile << "&&\n";
}
