#include "local_variable_stash.h"

LocalVariableStash::LocalVariableStash(uint8_t start_at_index) :
                                               current_var_count_(0),
                                               start_var_index_(start_at_index)

{
  // do nothing
}

LocalVariableStash::~LocalVariableStash()
{
  // do nothing.
}

uint8_t LocalVariableStash::current_var_count() {
  return current_var_count_;
}

uint8_t LocalVariableStash::getIndexForVar(std::string variable) {
  if (variable_to_index_map_.find(variable) == variable_to_index_map_.end()) {
    variable_to_index_map_[variable] = current_var_count_ + start_var_index_;
    ++current_var_count_;
  }
  return variable_to_index_map_[variable];
}
