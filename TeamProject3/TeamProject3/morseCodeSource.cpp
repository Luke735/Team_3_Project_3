#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <iterator>
#include <vector>
#include <sstream>
using namespace std;

struct Tree_Node {
    int val;
    Tree_Node* left, * right;
    Tree_Node(int val, Tree_Node* left = NULL, Tree_Node* right = NULL) :
        val(val), left(left), right(right) {}
};

Tree_Node* treehead; // fix this, currently this is a global and should become a private data member.

/** Takes one letter of morse code and inserts the ascii value of the letter it represents into a binary tree.
    @param line: string of morse code
    @param pos: represents how far through line the recursion has gotten
    @param root: the head of the morse code binary tree
*/
Tree_Node* treeBuilder(string line, int pos, Tree_Node* root) {
    pos++;
    bool finished = false;
    //if root is now at the position to insert the new letter, do it
    if (pos == line.length()) {
        finished = true;
        if (!root) {
            Tree_Node* newNode = new Tree_Node(line.at(0));
            root = newNode;
        }
        else {
            root->val = line.at(0);
        }

    }
    //if we are not finished, recursively travel left for '.' and right for '-'
    if (finished == false) {
        if (line.at(pos) == '.') {
            if (!root) {
                //if we must travel through nodes that are not yet generated, generate them with val '0'
                //if any '0's remain after the function's completion, then the tree must not be filled out completely
                Tree_Node* newNode = new Tree_Node('0');
                root = newNode;
                root->left = treeBuilder(line, pos, root->left);
            }
            else {
                root->left = treeBuilder(line, pos, root->left);
            }
        }

        if (line.at(pos) == '-') {
            if (!root) {
                Tree_Node* newNode = new Tree_Node('0');
                root = newNode;
                root->right = treeBuilder(line, pos, root->right);
            }
            else {
                root->right = treeBuilder(line, pos, root->right);
            }
        }
    }
    return root;
}//end treeBuilder

void treeBuilder(string line, Tree_Node* head) {
    treehead = treeBuilder(line, 0, head); // currently treehead is global, FIX THIS LATER
}//end treeBuilder

/** Encodes a string of lowercase letters into morse code.
    @param str: string of lowercase letters
    @return: morse code string of a string of lowercase letters (no spaces)
*/
string findchar(char target, Tree_Node* root){
  string returnstr;
  string tempstr = "";
  char node = root->val;
  
  if(root == NULL){return "error";}
  if(target == node){
    return (" ");
  }
  else{
    if(root->left != NULL){
      tempstr = findchar(target, root->left);
      if(tempstr != ""){
        returnstr = ".";
        returnstr.append(tempstr);
        return (returnstr);
      }
    }
    if(root->right != NULL){
      tempstr = findchar(target,root->right);
      if(tempstr != ""){
        returnstr = "-";
        returnstr.append(tempstr);
        return (returnstr);
      }
    }
    return ("");
  }
}

string encode(string& str, Tree_Node* root) {
  int i = 0;
  string returnstr = "";
  while(i <= str.size()){
    if(str[i] == ' '){
    }
    else{
      returnstr.append(findchar(str[i],root));
      if(i == str.size()-1){
        return (returnstr);
      }
    }
    i++;
  }
  return (returnstr);

/** Encodes a string of lowercase letters into morse code.
    @param str: string of lowercase letters
    @return: morse code string of a string of lowercase letters (no spaces)
*/
string& encode(string& str) {
    return str;
}

/** Wrapper function to recursively decode the morse code string of lowercase letters.
    @param node: current location node on the binary tree
    @param value: current morse code string.
    @return: the characters we are searching for.
*/
int decode_node(Tree_Node* node, string& value) {
    // This outputs the characters after traversing the tree
    if (value.length() == 0) {
        char letters = node->val;
        cout << letters;
        return node->val;
    }
    // Moves to the right node, then deletes the character processed.
    else if (value[0] == '-') {
        decode_node(node->right, value.erase(0, 1));
    }
    // Moves to the left node, then deletes the character processed.
    else if (value[0] == '.') {
        decode_node(node->left, value.erase(0, 1));
    }
}

/** Decodes morse code into a string of lowercase letters.
    @param str: string of morse code delimited with spaces
    @return: string of lowercase letters
*/
string& decode(string& str) {
    istringstream iss(str);
    ostringstream oss;
    string token;
    while (iss >> token) {
        decode_node(treehead, token); // need to fix treehead later, this is currently a global.
    }
    cout << "" << endl;
    return str;
}




int main() {

    ifstream fin("Morse_Code.txt");

    if (!fin.is_open()) {
        cout << "Could not open file" << endl;
        return -1;
    }

    else {
        Tree_Node* head = new Tree_Node(' ');
        string line;
        while (getline(fin, line)) { //Gets each line from the input file
            cout << line << endl;
            treeBuilder(line, head);
        }

        string option = "r";
        string userinput;
        string output;
        //the menu is in a while loop until the option selected is q
        //the menu only reads the first character of the input as the option
        while (option.at(0) != 'q') {
            cout << "MENU" << endl;
            cout << "e - Encode a string" << endl;
            cout << "d - Decode morse code" << endl;
            cout << "q - Quit" << endl;
            cout << endl;
            cout << "Choose an option: ";
            cin >> option;

           //Encode a string of lowercase letters with no spaces
            if (option.at(0) == 'e') {
                cout << "Text here: ";
                cin>>userinput;
                output = encode(userinput, head);
                cout<<output<<endl;
            }

            //Decode morse code with spaces as delimiters
            if (option.at(0) == 'd') {
                cout << "Text here: " << endl;
                string morse;
                cin.ignore(256, '\n');
                getline(cin, morse, '\n'); // might need to find a better way to do this later.
                decode(morse);
            }
        }
    }//end else
   return 0;
}//end main

