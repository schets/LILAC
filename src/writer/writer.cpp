#include "writer.h"

/*!
 * This functions adds the data to the list of values to be written,
 * as long as the rule corresponding to the type is set to true
 *
 * @param inval The data to be saved
 * @param dat_t The type of the input data
 */
void writer::add_data(std::shared_ptr<const data> inval, const w_type dat_t){
    if(will_write[dat_t]){
        dvals.push_back(inval);
    }
}
/*!
 * This functions sets the rule for the input type
 * @param inval The type which is having a rule reset
 * @param rule the new rule for the type
 */
void writer::set_rule(w_type inval, bool rule){
    will_write[inval] = rule;
}

/*!
 * This creates a new writer class with all of the type rules set to def
 * @param def The default rule for all of the types
 */
writer::writer(bool def){
    for(int i = BEGIN; i <= END; i++){
        will_write[i] = def;
    }
}

/*!
 * This constructs a writer that has the same rules as the map writevals, with any
 * rule that isn't in writevals set to def
 *
 * @param writevals The current set of rules being used
 * @param def The default rule for values not mapped by writevals
 */
writer::writer(const std::map<int, bool>& writevals, bool def):writer(def){
    for(auto val : writevals){
        if(is_w_type(val.first)){
            will_write[val.first] = val.second;
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


void writer::write(std::ostream& ofile) const{
    for(const auto& val : dvals){
        ofile << val->to_string();
        ofile << std::endl;
    }
}
