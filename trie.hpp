#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ece250_socket.h"
#include "trieNode.hpp"
#include "illegal_exception.hpp"


class trie {
    public:
        trie();
        ~trie();
        void initTrie(std::string filename);
        void insert(std::string classification);
        void classify(std::string input);
        void erase(std::string classification);
        void print();
        void empty();
        void printsize();
        void clear();

    private:
        std::ifstream class_id;
        trieNode* root = new trieNode();
        int size{};
        bool is_upper(std::string line);
        bool no_subclasses(trieNode* class_type);
        bool is_empty();
        void display_path(trieNode* root, trieNode* current, std::string currpath, int level_trie);
        void clear_trie(trieNode* current);
        bool valid_path(std::string classification);

};

trie::trie() {
    root -> isterminal = true;
}

void trie::initTrie(std::string filename) {
    class_id.open(filename);
    std::string line;
    /*
        CITATION:
        
        I referenced a website (https://www.geeksforgeeks.org/cpp/read-a-file-line-by-line-in-cpp/) to understand how to read a file line by line, in order to 
        properly read the txt files.
    */
    while (std::getline(class_id, line)) { //This while loop runs for each line in the text file to insert each classification.
        /*
            CITATION:

            I referenced a website (https://stackoverflow.com/questions/1474790/how-to-read-write-into-from-text-file-with-comma-separated-values) to understand
            how to retrieve strings within a line of text, seperated by a certain character.
        */
        trieNode* temp = root; //Creates a temporary pointer to begin inserting different classifications, starting from the root.
        std::istringstream singleline(line);
        std::string word{};
        while(std::getline(singleline, word, ',')) { //This while loop is used to extract every classification, and insert it into the trie
            bool exists = false;
            int pos{};
            for (int k{}; k < 15; ++k) { //For loop is used to see if the word trying to insert already exists in the trie.
                if (temp -> subclasses[k] != nullptr) { //Coniditional statement is used so do not access memory not allocated to program.
                    if (((temp -> subclasses[k]) -> class_type) == word) {
                    exists = true; //Sets boolean to true to skip this string, and continue to the next one in the line.
                    pos = k;
                    break; //If find string then do not continue iterating through array of pointers.
                    }
                }
                else {
                    break;
                }
            }
            if (exists == true) { //If the string is already inserted in the trie, then set temp pointer to that node to continue insertion of classifications.
                temp = temp -> subclasses[pos];
            }
            else { //If the string is not inserted, then must insert it into the trie.
                trieNode* node = new trieNode(); //Allocates new node, and assigns the classification and terminal indicator to respective values.
                node -> class_type = word;
                node -> isterminal = true;
                int i{};
                while (temp -> subclasses[i] != nullptr) { //While loop used to find an empty spot to place new classification.
                    ++i;
                }
                temp -> subclasses[i] = node; //Assigns node to be a child of the node pointed by temp.
                temp -> isterminal = false; //Ensure that parent is no longer considered terminal if the first child is being inserted.
                node -> parent = temp;
                temp = node; //Assigns temp to node for the next iteration to begin from the corect node.
                node = nullptr; //Properly deallocates pointer since this node is accesible by its parent.
                delete node;
            }
        }
        ++size; //Increments size every time a classification is added.
        temp -> uni_insert = true; //Every last classificatio in line added will be a unique insertion, so this boolean value is set to true;
        temp = nullptr; //Deallocating temp pointer, and setting to nullptr to prevent any dangling pointers.
        delete temp;
    }
    std::cout << "success" << std::endl;
    class_id.close(); //Used since done analysing the text file that was used for creating the trie.
}

bool trie::is_upper(std::string line) {
    /*
        CITATION:
        
        I referenced a website (https://www.geeksforgeeks.org/dsa/check-whether-the-given-character-is-in-upper-case-lower-case-or-non-alphabetic-character/), to 
        understand how to properly identify whether a character is an uppercase letter or not.
    */
    for (std::size_t k{}; line[k] != '\0'; ++k) { //For loop checks if any letter is an uppercase, to throw an exception since it is an invalid input.
        if (line[k] >= 'A' && line[k] <= 'Z') {
            return true;
        }
    }
    return false; //If no uppercase letters exist, then return false.
}

void trie::insert(std::string classification) {
    bool inc_upper = is_upper(classification);
    if (inc_upper == true) { //If inc_upper is true, then there is an uppercase letter, and must throw an exception since it is an illegal argument.
        throw illegal_exception(classification);
    }
    
    else { //If no uppercase letters, then can begin to insert the new classification.
        trieNode* temp = root;
        bool inserted = false;
        std::stringstream line(classification);
        std::string word;
        while(std::getline(line, word, ',')) { //While loop used to analyze each classification, which are commma seperated in the input.
            bool exists = false;
            int pos{};
            for (int k{}; k < 15; ++k) { //For loop is used to check if the current word analyzying is already inserted in the trie
                if (temp -> subclasses[k] != nullptr) {
                    if (((temp -> subclasses[k]) -> class_type) == word) {
                        exists = true; //Sets exists to true to ensure correct steps, and breaks for loop since found that the classification already exists.
                        pos = k;
                        break;
                    }
                }
           } 
            if (exists == true) { //If the current classification already exists, assign temp to that node to continue computing rest of insertion.
                temp = temp -> subclasses[pos];
            }
           else { //If the classification does not exist, then proceed to enter the node.
                trieNode* node = new trieNode(); //Creates new node to add a classification to the trie, and assigns the classifcation and sets the isterminal indicator.
                node -> class_type = word;
                node -> isterminal = true;
                int i{};
                while (temp -> subclasses[i] != nullptr) { //While loop is used to search for position to place the child pointer.
                    ++i;
                }
                temp -> subclasses[i] = node; //Assigns the child pointer from node's parent, and sets terminal to false since no longer terminal if it has a child.
                temp -> isterminal = false;
                node -> parent = temp;
                temp = node;
                inserted = true; //Sets inserted boolean to true for proper computations.
                node = nullptr; //Deallocates pointer since node is accesible by its parent.
                delete node;
            }
        }
        if ((temp -> uni_insert == true) && (temp -> uni_insert == true)) { //If statement used to print failure in the case where nothing was inserted and no node was updated to be unique.
            std::cout << "failure" << std::endl;
        }
        else { //Else statement if the there was a succesful insertion, including updating a current classification to be unique.
            if (inserted == false) {
                temp -> uni_insert = true;
            }
            ++size; //Increments size and prints success after a succesful insertion.
            std::cout << "success" << std::endl;
        } 
        temp -> uni_insert = true; //The final insertion when succesful is always a unique one, so update the unique indicator.
        temp = nullptr; //Deallocating temporary pointer to prevent dangling pointers.
        delete temp;
    }
}

void trie::classify(std::string word_class) {
    if (is_upper(word_class) == true) { //If statement used to check if any upper case letters, and throws an exception if there is.
        throw illegal_exception(word_class);
    }
    else { //If no uppercase letters, then can begin to classify a word.
        trieNode* temp = root; //Sets temporary pointer to root as classification always begins at root and works way down to more specific classifications.
        std::string candidate_labels{};
        std::string curr_label{};
        while (temp -> isterminal != true) { //While loop used to run until a terminal node, because that node is most specific classification possible.
            for (std::size_t i{}; i < 15; ++i) { //For loop used to collect all the candidate labels for that level of the trie for LLM to choose.
                if (temp -> subclasses[i] != nullptr) { //If statement used to ensure only accessing memory that contains nodes.
                    candidate_labels += (temp -> subclasses[i] -> class_type) + ',';
                }

            }
            candidate_labels.pop_back(); //Used to remove the trailing comma before sending to LLM.
            std::string label = labelText(word_class, candidate_labels);
            candidate_labels.clear(); //Clear candidate_labels string to be used for next level of trie for classification.
            if (curr_label == "") { //If statement used to ensure curr_label contains proper formatting.
                curr_label = label + ',';
            }
            else {
                curr_label += label + ',';
            }
            for (std::size_t k{}; k < 15; ++k) { //For loop used to assign to node of label LLM classified, to check lower classifications.
                if ((temp -> subclasses[k]) != nullptr) {
                    if ((temp -> subclasses[k]) -> class_type == label) {
                        temp = temp -> subclasses[k];
                        break;
                    }
                }
            }
        }
        curr_label.pop_back(); //Used to remove trailing comma to print in correct format.
        std::cout << curr_label << std::endl;
        temp = nullptr; //Deallocating temp pointer to prevent dangling pointer.
        delete temp;
    }
}

bool trie::no_subclasses(trieNode* class_type) { //Function used to check if a node has no subclasses.
    for (std::size_t k{}; k < 15; ++k) {
        if (class_type -> subclasses[k] != nullptr) {
            return false; //If contains a subclass, then return false
        }
    }
    return true; //Will return true if there was no children, after it analyzed the 15 slots of potential children.
}

bool trie::valid_path(std::string classification) { //Function is used to check if the given path is a valid one of the trie.
    std::istringstream line(classification);
    std::string word;
    trieNode* temp = root;
    while (std::getline(line, word,',')) { //While loop used to run for every classification that was given in the input.
        for (std::size_t k{}; k < 15; ++k) { //For loop used to find if the current classification exists, and assigns temp to that node to continue checking.
            if (temp -> subclasses[k] != nullptr) {
                if ((temp -> subclasses[k]) -> class_type == word) {
                    temp = (temp -> subclasses[k]);
                    break; //Breaks since already find node with the given classification.
                }
            }
            else if ((k == 14) && ((temp -> class_type) != word)) { //If on last iteration and did not find, then return false, since is not in the children of current node.
                temp = nullptr; //Properly deallocates temp to prevent a dangling pointer.
                delete temp;
                return false;
            }
        }
    }
    if (((temp -> isterminal == true) && (temp -> class_type == word)) || ((temp -> isterminal == false) && (temp -> uni_insert) == true)) { //If statement used if a a classification given does not go to terminal, but is one that was inserted, so still a unique classification.
        temp = nullptr; //Properly deallocates temp to prevent a dangling pointer.
        delete temp;
        return true;
    }
    temp = nullptr; //Properly deallocates temp to prevent a dangling pointer.
    delete temp;
    return false; //If did not return at this point then it is not a valid path, and return false.
}

void trie::erase(std::string classification) {
    if (is_upper(classification) == true) { //If statement used to check if input contains uppercase, and throws exception if true.
        throw illegal_exception(classification);
    }
    else if (valid_path(classification) == false) { //If valid_path returns false, then the path given is not a valid one, so print failure since does not exist.
        std::cout << "failure" << std::endl;
    }
    else { //If a valid path, then can proceed with erasing the classification.
        std::istringstream line(classification);
        std::string word;
        trieNode* temp = root;
        bool success_erase = false;
        while(std::getline(line, word, ',')) { //While loop will run for all classifications included in the input.
                for (std::size_t k{}; k < 15; ++k) { //For loop is used to find where the current classification is located in the trie.
                    if (temp -> subclasses[k] != nullptr) { //If statement used to ensure not trying to access memory not allocated.
                        if ((temp -> subclasses[k]) -> class_type == word) {
                            temp = (temp -> subclasses[k]); //Once find node with classification, assign temp to that node and break loop since already found.
                            break;
                        }
                    }
                }
                    if (no_subclasses(temp) == true) { //Once reach terminal node, then can begin erasing in a bottom up order.
                        for (std::size_t k{}; k < 15; ++k) {  //Used to properly deallocate any pointers to children.
                            if ((temp -> parent) -> subclasses[k] == temp) {
                                (temp -> parent) -> subclasses[k] = nullptr;
                            }
                        }
                        trieNode* temp_delete = temp; //Allocates a temp_delete pointer to temp to delete that node, and then assign temp to the deleted nodes parent.
                        temp = temp -> parent;
                        delete temp_delete; //Deletes the current node since it is a terminal node.
                        temp_delete = nullptr;
                        bool nowempty = no_subclasses(temp);
                        if (nowempty == true) { //If node pointed by temp is now terminal, then sets its terminal indicator to true.
                            temp -> isterminal = true;
                        }
                        while (temp != root) { //This while loop is used to check if any classification above the terminal one can be removed.
                            if ((temp -> uni_insert == false) && (nowempty == true)) { //If the above node is not a unique insertion, and contains no children, then can also remove it.
                                for (std::size_t k{}; k < 15; ++k) {
                                    if ((temp -> parent) -> subclasses[k] == temp) { //Properly deallocates pointer from parent to the child.
                                        (temp -> parent) -> subclasses[k] = nullptr;
                                    }
                                } 
                                if (no_subclasses(temp -> parent) == true) { //If temps parent now has no children, then updates its terminal indicator to true.
                                    (temp -> parent) -> isterminal = true;
                                }
                                trieNode* temp_del_par = temp; //Pointers used to delete current temp, and sets temp to its parent.
                                temp = temp -> parent;
                                delete temp_del_par;
                                temp_del_par = nullptr;
                            }
                            else { //Once reach a node we cannot delete, then cannot delete any above, so break this while loop.
                                break;
                            }
                        }
                            
                    }
        }
        if ((temp -> uni_insert) == true && ((temp -> class_type) == word)) { //If erasing a classification that is not a terminal node, then changes its uniquness indicator to false since erasing the fact that its not an indepedent classification.
            temp -> uni_insert = false;
        }
        temp = nullptr; //Properly deallocates temp pointer to prevent any dangling pointers
        delete temp;
        --size; //Any erase that is succesful, will always decrement size by 1.
         std::cout << "success" << std::endl;
    }
}

bool trie::is_empty() {
    for (std::size_t k{}; k < 15; ++k) { //For loop used to search through roots children, if it has no children, then trie is empty, and will return true.
        if (root -> subclasses[k] != nullptr) {
            return false;
        }
    }
    return true;
}

void trie::display_path(trieNode* root, trieNode* current, std::string currpath, int level_trie) {
    /*
        CITATION:

        I referenced a webiste (https://www.tutorialspoint.com/print-all-possible-joints-of-a-trie-constructed-from-a-given-list-of-string), to understand how to
        properly print all the possible paths for a trie data structure.
    */
    int stringlength = currpath.size(); //Used to remove end of completed paths to ensure other path's are printed correctly.
    for (std::size_t k{}; k < 15; ++k) { //For loop used to iterate through all children to print all paths.
        if ((current -> subclasses[k]) != nullptr) {
            if (level_trie == 0) { //Used to ensure currpath is printed in the corect format.
                currpath = (((current -> subclasses[k]) -> class_type) + ',');
            }
            else {
                currpath += ((current -> subclasses[k]) -> class_type) + ',';
            }
            display_path(root, current -> subclasses[k], currpath, ++level_trie); //Recursive call on each child to print all possible paths.
        }
        else if (current -> isterminal == true) { //Once reach a terminal node, then full path is in currpath.
            currpath.pop_back(); //Removes trailing comma and replaces with '_' for correct output.
            currpath += '_';
            std::cout << currpath;
            break; //Once printed path, will break to go back one level higher and continue printing paths.
        }
       /*
            CITATION:

            I referenced a website (https://www.geeksforgeeks.org/cpp/stdstringerase-in-cpp/) to understand how to 
            properly remove the end of a string, without simply inserting '\0', as this would cause an error when the paths
            get large.
        */
        currpath.erase(stringlength, (currpath.length()) - 1); //Used to remove last node of string, to continue printing other paths correctly.
        if (currpath.length() == 1) { 
            currpath.erase(0,1);
        }
    }
}

void trie::print() { //Function that gets called from main for printing.
    if (is_empty() == true) { //If the trie is empty, then outputs correct command.
        std::cout << "trie is empty" << std::endl;
    }
    else { //If not empty, then call display_path to display all possible classification of trie.
       std::string currpath{};
       display_path(root, root, currpath, 0);
       std::cout << std::endl;
    }
}

void trie::empty() { //Function used to print if the trie is empty or not.
    if (is_empty() == true) { //If it is empty, output empty 1 to indicate this.
        std::cout << "empty 1" << std::endl;
    }
    else { //If it is not empty, output empty 0 to indicate this.
        std::cout << "empty 0" << std::endl;
    }
}

void trie::printsize() { //Function used to print the size of the trie.
    std::cout << "number of classifications is " << size << std::endl;
}

void trie::clear_trie(trieNode* current) {
    for (std::size_t k{}; k < 15; ++k) { //For each level of trie, will make recursive call on child if it exists.
        if (current -> subclasses[k] != nullptr) {
            clear_trie(current -> subclasses[k]);
        }
    }
    if (current != root) { //If statement used because want to keep root if after clear begin building tree again
        for (std::size_t k{}; k < 15; ++k) {
            if ((current -> parent) -> subclasses[k] == current) { //Used to proeprly deallocate pointer from parent to child.
                ((current -> parent) -> subclasses[k]) = nullptr;
                delete ((current -> parent) -> subclasses[k]);
                break; //Break once this pointer is found.
            }
        }
        current -> parent = nullptr;
        delete current -> parent; //Properly deallocating the pointer from a child to its parent.
        for (std::size_t k{}; k < 15; ++k) { //Deallocating all children pointers.
            delete current -> subclasses[k];
            current -> subclasses[k] = nullptr;
        }
        delete current; //Delete the current node clearing, and prevent any dangling pointers.
        current = nullptr;
    }
}

void trie::clear() { //Function that is called when clearing trie.
    if (is_empty() == false) { //If not empty then call clear_trie to remove all nodes.
        clear_trie(root);
    }
    root -> isterminal = true;
    for (std::size_t k{}; k < 15; ++k) { //Used to deallocate all child pointers of root since it is now empty.
        root -> subclasses[k] = nullptr;
    }
    size = 0; //Since after clear, there is no classifications in the trie
    std::cout << "success" << std::endl;

}

trie::~trie() {
    if (is_empty() == false) { //Calls clear_trie() to deallocate all nodes if the trie is not empty.
        clear_trie(root);
    }
    for (std::size_t k{}; k < 15; ++k) { //Properly deallocates all children of root.
        root -> subclasses[k] = nullptr;
    }
    delete root; //Deletes the root node, and prevents dangling pointer, to free all memory program allocated for root.
    root = nullptr;
}
