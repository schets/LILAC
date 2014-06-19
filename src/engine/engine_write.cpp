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
