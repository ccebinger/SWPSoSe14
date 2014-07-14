#ifndef LOCAL_VARIABLE_STASH_H
#define LOCAL_VARIABLE_STASH_H

#include <cstdint>
#include <map>

/**
 * Represents the local variable table of a bytecode method. It supports adding
 * variable names to the stash, automatically assigning them an index in the
 * method's variable table. The index to a local variable can be retrieved. Due
 * to bytecode restrictions, all methods in this class will assume that the
 * total number of local variables is always <= 255.
 * @brief Maps local variable name to index in bytecode method variable table.
 */
class LocalVariableStash
{
  public:
    LocalVariableStash(uint8_t start_at_index);

    ~LocalVariableStash();

    uint8_t current_var_count();

    uint8_t getIndexForVar(std::string variable);

    uint8_t reserved_var_count();

    std::map<std::string, uint8_t> get_variable_to_index_map();

  protected:

  private:
     /**
      * Maps the name of a variable to its position in the method's variable
      * table
      */
    std::map<std::string, uint8_t> variable_to_index_map_;

    /**
     * The number of named variables we currently know.
     */
    uint8_t current_var_count_;

    /**
     * The number of local variables reserved by the bytecode generator. We'll
     * only generate variable indices >= this.
     */
    uint8_t start_var_index_;
};

#endif // LOCAL_VARIABLE_STASH_H
