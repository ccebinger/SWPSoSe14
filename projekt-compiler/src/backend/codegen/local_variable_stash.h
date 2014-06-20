#ifndef LOCAL_VARIABLE_STASH_H
#define LOCAL_VARIABLE_STASH_H

#include <cstdint>
#include <map>

class LocalVariableStash
{
  public:
    /** Default constructor */
    LocalVariableStash(uint8_t start_at_index);

    /** Default destructor */
    ~LocalVariableStash();

    uint8_t current_var_count();

    uint8_t getIndexForVar(std::string variable);

  protected:

  private:
    std::map<std::string, uint8_t> variable_to_index_map_;
    uint8_t current_var_count_;
    uint8_t start_var_index_;
};

#endif // LOCAL_VARIABLE_STASH_H
