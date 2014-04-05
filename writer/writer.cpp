#include "writer.h"
void writer::add_data(std::shared_ptr<const data> inval, const w_type dat_t){
    if(will_write[dat_t]){
        dvals.push_back(inval);
    }
}
void writer::add_data(std::shared_ptr<const data> inval, const std::string dat_t){
    add_data(inval, from_name(dat_t));
}

void writer::set_rule(w_type inval, bool rule){
    will_write[inval] = rule;
}
void writer::set_rule(std::string inval, bool rule){
    set_rule(from_name_strict(inval), rule);
}

writer::writer(bool def){
    for(int i = BEGIN; i <= END; i++){
        will_write[i] = def;
    }
}
writer::writer(const std::map<int, bool>& writevals, bool def){
    //don't use initializer list since input needs to be sanitized 
    for(const auto& val : writevals){
        if(is_w_type(val.first)){
            will_write[val.first] = val.second;
        }
    }
    for(int i = BEGIN; i < END; i++){
        if(!will_write.count(i)){
            will_write[i] = def;
        }
    }
};

writer::~writer(){}

bool writer::is_w_type(int inval){
    for(int i = BEGIN; i < END; i++){
        if(inval == i){
            return true;
        }
    }
    return false;
}

writer::w_type writer::from_name(const std::string inval){
    const static std::map<std::string, w_type> stvals = {{ "FINAL_SCORE", FINAL_SCORE},
        {"FINAL_FUNC", FINAL_FUNC}, {"INTERMEDIATE_FUNC", INTERMEDIATE_FUNC},
        {"INTERMEDIATE_SCORE", INTERMEDIATE_SCORE}, {"OTHER", OTHER}, {"INDEX", INDEX}};
    if(!stvals.count(inval)){
        err(std::string("Data type of ") + inval + std::string(" not found, check capitalization and spelling. Returning OTHER."),
                "writer::from_name", "writer/writer.cpp", WARNING);
        return OTHER;
    }
    return stvals.at(inval);
}
writer::w_type writer::from_name_strict(const std::string inval){
    const static std::map<std::string, w_type> stvals = {{ "FINAL_SCORE", FINAL_SCORE},
        {"FINAL_FUNC", FINAL_FUNC}, {"INTERMEDIATE_FUNC", INTERMEDIATE_FUNC},
        {"INTERMEDIATE_SCORE", INTERMEDIATE_SCORE}, {"OTHER", OTHER}, {"INDEX", INDEX}};
    if(!stvals.count(inval)){
        err(std::string("Data type of ") + inval + std::string(" not found, check capitalization and spelling."),
                "writer::from_name", "writer/writer.cpp", FATAL_ERROR);
        //never reaches this point
        return OTHER;
    }
    return stvals.at(inval);
}
void writer::write(std::ostream& ofile) const{
    for(const auto& val : dvals){
        ofile << val->to_string();
        ofile << "\n";//std::endl;
    }
}
