#include "local_variable_stash.h"

/**
  * Constructs a new LocalVariableStash, reserving a number of entries for
  * variables generated by the compiler.
  * @param start_at_index  The number of entries reserved, so that variable
  *                        table indices [0, start_at_index) won't be used.
  */
LocalVariableStash::LocalVariableStash(uint8_t start_at_index) :
                                               current_var_count_(0),
                                               start_var_index_(start_at_index)

{
  // do nothing
}

/** Default destructor */
LocalVariableStash::~LocalVariableStash()
{
  // do nothing.
}

/**
  * Getter to count the number of named local variables currently held.
  * @return The number of variables, does not include the number of
  *         reserved entries!
  *         length(variable_table) = current_var_count() + start_at_index
  */
uint8_t LocalVariableStash::current_var_count() {
  return current_var_count_;
}

uint8_t LocalVariableStash::reserved_var_count() {
  return start_var_index_ + 1;
}

/**
  * The central method of this class. Used to retrieve the index of a named
  * variable in the bytecode method's variable table.
  * @param variable The name of the variable.
  * @return The index of the variable within the table. If the variable is
  *         not yet known, will assign it to a new index and
  *         current_var_count():Post = current_var_count():Pre + 1
  */
uint8_t LocalVariableStash::getIndexForVar(std::string variable) {
  if (variable_to_index_map_.find(variable) == variable_to_index_map_.end()) {
    variable_to_index_map_[variable] = current_var_count_ + start_var_index_;
    ++current_var_count_;
  }
  return variable_to_index_map_[variable];
}

bool LocalVariableStash::exists(std::string& var) {
  return variable_to_index_map_.count(var) > 0 ? true : false;
}

std::map<std::string, uint8_t> LocalVariableStash::get_variable_to_index_map()
{
  return variable_to_index_map_;
}
