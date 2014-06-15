#ifndef GRAPHS_H
#define GRAPHS_H

#include <map>
#include <locale>         // std::locale, std::isdigit
#include <fstream>
#include <sstream>

#include <common/Env.h>
#include <frontend/adjacency_list.h>
#include <frontend/Invalid_Format_Exception.h>
#include <frontend/IO_Exception.h>


/**
 * Represents the Graphs class which contains a map of Graph objects or
 * more exact Rail functions.
 * The Graphs/functions are saved in a map so they can accessed over there identifier (name).
 * That means the Graphs class returns for a given name a Graph object, if it exists.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>
 */
class Graphs
{
  public:
   /**
    * Represents the type of shared_ptr of node.
    */
    typedef std::shared_ptr<Node> Node_ptr;

   /**
    * Represents the type of shared_ptr of Graph.
    */
    typedef std::shared_ptr<Graph> Graph_ptr;

   /**
    * Represents the type for the Graph map, which consist as key of std::string
    * and as value of Graph_ptr.
    */
    typedef std::map<std::string, Graph_ptr> Graph_map;

   /**
    * Represents the type for the const reference of std::string.
    */
    typedef const std::string& str;

  private:
   /**
    * The map of graphs or Rail functions where are the name of the function/Graph represents the
    * key in the map and the hole graph is the corresponding value.
    */
    Graph_map graphs;
  public:

   /**
    * The default ctor which creats a Graphs object.
    */
    Graphs();

   /**
    * The destructor which cleans up the existing Graphs object.
    */
    virtual ~Graphs();

   /**
    * Puts a new Graph into the Graphs map.
    *
    * @param key      the key for which the Graph is saved in the map
    *                      (normaly the name of the Graph/function)
    * @param graph    the graph which will be saved as value
    * @return         returns true if the processing was successful, false otherwise
    */
    virtual bool put(str key, Graph_ptr graph);

   /**
    * Finds for the given key (the Graph name) the corresponding Graph object.
    *
    * @param key        the key (name of the Graph)
    * @return           the corresponding Graph
    */
    virtual Graph_ptr find(str key);

   /**
    * Returns the begin iterator of the Graphs map.
    *
    * @return         the begin iterator of the map
    */
    virtual Graph_map::iterator begin();

   /**
    * Returns the end iterator of the Graphs map.
    *
    * @return         the end iterator of the map
    */
    virtual Graph_map::iterator end();

   /**
    * Returns the size of the map (the count of graph's).
    *
    * @return         the size
    */
    virtual size_t size();

   /**
    * Marshalls (serialize) the current Graphs map into a csv file.
    * The format looks like this: <br\>
    * <pre>
    * [GRAPH-NAME]
    *  ID ; COMMAND-ARG ; SUCCESSOR 1 ; SUCCESSOR 2 (OPTIONAL)
    * </pre>
    * Example:
    * <pre>
    * [main]
    * 1 ;[Hello] ; 2
    * 2 ; o ; 3
    * 3 ; # ; 0
    * </pre>
    *
    * @param file       the name of the file in which the Graphs should be saved
    */
    virtual void marshall(str file, char delimiter);

   /**
    * Unmarshalls (deserializes) the given csv file into the current Graphs object.
    * Tries to unmarshalles after the specified format,
    * if the format is not valid an exception will be thrown.
    *
    * @see Graphs::marshall()
    * @param file             the name of the file which will be unmarshalled
    * @param delimeter        the delimeter which is used in the csv file
    */
    virtual void unmarshall(str file, char delimeter);

   /**
    * Writes the current Graphs into a file in dot format in order to illustrate the graphs.
    *
    * @param file       the name of the file
    */
    void writeGraphViz(Graphs::str file);


    /**
     * Determine graphviz Node Styling based on the Node
     */
    std::string gvGetNodeStyles(std::shared_ptr<Node> node) const;

  private:
   /**
    * Finds a node with the given id in the given Graph.
    *
    * @param adj      the graph which will be searched
    * @param id       the id of the node
    * @return         the shared_ptr of the searched node
    */
    Node_ptr findNode(Graph_ptr adj, std::string id);

   /**
    * Returns for the given std::string a corresponding Command (struct).
    *
    * @param cmd          the string which should mapped into a Command
    * @return             the corresponding Command of the std::string
    */
    Command getCommand(std::string& cmd);

   /**
    * Unmarshalls the given line from the csv file and returns for the line a corresponding Node_ptr.
    *
    * @param adj              the Graph on which the Node belongs to
    * @param line             the line of the csv file which should be unmarshalled
    * @param delimiter        the delimiter which is used in the csv file
    * @return                 the corresponding Node_ptr of the given line
    */
    Node_ptr unmarshall_line(Graph_ptr adj, std::string& line, char delimiter);

   /**
    * Unmarshalls a function/Graph from the given std::ifstream (file).
    *
    * @param infile             the std::ifstream which will be used for the unmarshalling
    * @param line               the last line which was read from the std::ifstream (contains the name of the Graph)
    * @param delimiter          the delimiter which is used in the csv file
    * @return                   the unmarshalled Graph
    */
    Graph_ptr unmarshall_Function(std::ifstream& infile, std::string& line, char delimiter);

   /**
    * Skips the empty lines in the ifstream.
    *
    * @param infile       the std::ifstream in which the empty lines are skipped
    * @param line         the last line which was read from the std::ifstream
    */
    void skip_empty_lines(std::ifstream& infile, std::string& line);

    /**
    * Checks if the given line contains a function name.
    * The given std::string should contains [ on the begin and ] on the end.
    *
    * @param line       the std::string which will be checked
    * @return           true if line contains a function name, false otherwise
    */
    bool containsFunctionName(str line);

   /**
    * Checks if the given string contains at the begin a given char and at the end a given char.
    *
    * @param cmd         the std::string  which will be checked
    * @param begin       the char which should be at the begin of the given std::string
    * @param end         the char which should be at the end of the given std::string
    * @return            true if the std::string contains the chars, false otherwise
    */
    bool containsBeginAndEndChar(std::string& cmd, char begin, char end);
};

/**
* Prints the given Graphs::Node_ptr on the command line.
*
* @param n      the Graphs::Node_ptr which points to the Node which should be printed
*/
void printNode(Graphs::Node_ptr n);
#endif // GRAPHS_H
