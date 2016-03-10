/*
Assumption in priority is :
iteration > concatenation > union
*/



#include <bits/stdc++.h>
#define MAX 100

using namespace std;

char transition[MAX][MAX]; 

typedef struct {
	int total_states;
	int start;
	int final;
}NFA;

NFA NFA_machines[26];



NFA create_NFA(string str)
{
	int current_NFA=-1,last_state=0,start=0,final=0,NFA_1=-1,NFA_2=-1;
	// cout<<str<<endl;
	const char *itr = str.c_str();
	while(*itr!='\0')
	{
		if(isalpha(*itr))
		{
			NFA_1=current_NFA;
			current_NFA++;
			NFA_2=current_NFA;
			// current_NFA %=2;
			start = ++last_state;
			final = start +1;
			last_state = final;
			// cout<<start<<endl;
			transition[start][final]=*itr;
			NFA_machines[current_NFA].total_states=2;
			NFA_machines[current_NFA].start=start;
			NFA_machines[current_NFA].final=final;
		}
		else if(*itr == '*')   //klnee star
		{
			//cout<<*itr<<endl;
			start = ++last_state;
			final = start+1;
			last_state = final;
			transition[start][final]= char(36);
			transition[NFA_machines[NFA_2].final][NFA_machines[NFA_2].start]= char(36);
			NFA_machines[NFA_2].total_states +=2;
			NFA_machines[NFA_2].start=start;
			NFA_machines[NFA_2].final=final;	
		}
		else if(*itr == '.')  //concatenation
		{
			transition[NFA_machines[NFA_1].final][NFA_machines[NFA_2].start]= char(36);
			NFA_machines[NFA_1].total_states += NFA_machines[NFA_2].total_states;
			NFA_machines[NFA_1].final = NFA_machines[NFA_2].final;
			NFA_1--; NFA_2--;
			current_NFA--;
		}
		else if (*itr == '|')
		{
			start = ++last_state;
			final = start+1;
			last_state = final;
			transition[start][NFA_machines[NFA_2].start]='$';
			transition[start][NFA_machines[NFA_1].start]='$';
			transition[NFA_machines[NFA_2].final][final]='$';
			transition[NFA_machines[NFA_1].final][final]='$';
			NFA_machines[NFA_1].total_states += (NFA_machines[NFA_2].total_states+2);
			NFA_machines[NFA_1].start= start;
			NFA_machines[NFA_1].final= final;
			NFA_1--; NFA_2--;
			current_NFA--; 
		}
		itr++; 
	}
	cout<<NFA_machines[0].total_states<<"----"<<NFA_machines[0].start<<"----"<<NFA_machines[0].final<<endl;
	//cout<<NFA_machines[1].total_states<<"----"<<NFA_machines[1].start<<"----"<<NFA_machines[1].final<<endl;
	for (int i=1; i<=50;i++)
		for (int j=1; j<=50;j++)
			if(transition[i][j]!='\0')
			{
				cout<<i<<"-->"<<transition[i][j]<<"-->"<<j<<endl;
			}
		cout<<endl;
}

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


int main()
{
	
	for (int i=0; i<MAX;i++)
		for (int j=0; j<MAX;j++)
			// for (int k=0; k<MAX;k++)
				transition[i][j]='\0';

	string post, str ;
	cout<<"enter the regex : ";
	cin>>str;
	
	post = infixTOpostfix (str);
	//const char *cstr = str.c_str();
	//cout << *(cstr+2)<<endl;
	cout<<post<<endl;
	create_NFA(post);
	  
	return 0;
}