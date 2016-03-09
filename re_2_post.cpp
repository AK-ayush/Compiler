#include <iostream>
#include <stack>
#include <string>

using namespace std;

// precedence: return precedence value for various common math
//   operators (multiply and divide have a higher precedence
//   than add and subtract)
int precedence(char c) {
  switch (c) {
    case '|' :return 2;
    case '.' : return 3; case '*' : return 4;
    default : return 0;
    }
} // end precedence


// infixTOpostfix: function to convert infix string to postfix
//   Note: No error checking; routine assumes input infix
//   expression is valid
string infixTOpostfix (const string infix) {
  stack<char> aStack;
  string postfix = "";
  string::size_type chPos = 0;

  while (chPos < infix.length()) {
    char c = infix[chPos];
    switch (c) {
      case '(' :
        aStack.push(c);
        break;
      case ')' : // pop stack until find matching '('
        while (aStack.top() != '(') {
          postfix = postfix + aStack.top();
          aStack.pop();
          }
        aStack.pop(); // remove '('
        break;
      case '|' : case '.' : // operator
        while (!aStack.empty() && aStack.top() != '(' &&
                  precedence(c) <= precedence(aStack.top())) {
          postfix = postfix + aStack.top();
          aStack.pop();
          }
        aStack.push(c);
        break;
      case ' ' : // ignore spaces
        break;
      default : // operand
        postfix = postfix + c;
        break;
      }
    ++chPos; // move to the next character in infix string
    }

  // append the remaining operators on the stack
  while (!aStack.empty()) {
    postfix = postfix + aStack.top();
    aStack.pop();
    }
  return postfix;
} // end infixTOpostfix


int main() {
  string infix, postfix;

    infix = "a|b|c.d";
    //infix = infix.substr(0,infix.length()-1); // remove \n
    //if (infix.length() == 0)

    postfix = infixTOpostfix (infix);
    cout << postfix << endl;
  return 0;
} // end main
