/*
Assumption in priority is :
iteration > concatenation > union
*/



#include <bits/stdc++.h>
#define MAX 100

using namespace std;

char transition[MAX][MAX]; 
char abcd[26];
int alpha=0;
typedef struct {
	int total_states;
	int start;
	int final;
}NFA;

typedef struct 
{
	int total_states;
	int start;
	int final[MAX];
} DFA;

DFA DFA_machine;
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
			abcd[alpha++]=*itr;
			//cout<<abcd[alpha-1]<<"sdf";
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
			transition[start][NFA_machines[NFA_2].start]=char(36);
			transition[NFA_machines[NFA_2].final][final]=char(36);
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
	//cout<<NFA_machines[0].total_states<<"----"<<NFA_machines[0].start<<"----"<<NFA_machines[0].final<<endl;
	//cout<<NFA_machines[1].total_states<<"----"<<NFA_machines[1].start<<"----"<<NFA_machines[1].final<<endl;
	cout<<"transition table of final NFA\n"<<endl;
	cout<<"_________ ________ __________"<<endl;
	cout<<"   From  |   To   |   Input  |"<<endl;
	cout<<"_________|________|__________|"<<endl;
	for (int i=1; i<=50;i++)
		for (int j=1; j<=50;j++)
			if(transition[i][j]!='\0')
			{
				cout<<"    "<<i<<"    |"<<"    "<<j<<"   "<<"|    "<<transition[i][j]<<"    "<<endl;
				//cout<<i<<"-->"<<transition[i][j]<<"-->"<<j<<endl;
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

int bool_arr[100]={0};
int epsilon_closure(int* arr,int size)
{
	stack<int> s;
	for(int j=0;j<size;j++)
	{bool_arr[arr[j]]=1;}
	for(int i=0;i<size;i++)
		if(bool_arr[arr[i]]==1)s.push(arr[i]);

	while(!s.empty())
	{
		int u=s.top();
		s.pop();
		for(int i=1;i<100;i++)
		{
			if(transition[u][i]== '$')
			{

			if(bool_arr[i]!=1)
				{
					bool_arr[i]=1;
					s.push(i);	
				}
			}
		}
	}
}

int* moves (int A , char b)
{

	int ctr=0;

	int* ans;
	ans = (int*)malloc(sizeof(int)*MAX);
	for(int i=0;i<100;i++)
	{
		ans[i]=-1;
	}
	for(int i=0;i<100;i++)
		{
			if(transition[A][i]==b) 
				{
					ans[ctr]=i;ctr++;
				}
		}

	// return ans;
	epsilon_closure(ans,ctr);
} 

int dfa_moves[MAX][MAX];
int dfa_states;
int dfa_trans[MAX][MAX];
NFA NFA_to_DFA()
{

	int final = NFA_machines[0].final;
	int start = NFA_machines[0].start;
	int size = NFA_machines[0].total_states;
	//state start
	int *ans,*ptr = &start;
	int ct=0,arr[MAX],arr_2[MAX];
	
	for(int i=0;i<100;i++)
	{bool_arr[i]=0;}

	epsilon_closure(ptr,1);
	for (int i=1;i<100;i++){
		if(bool_arr[i]==1)
			{
				//cout<<i<<"-->";
				arr[ct++]=i;dfa_moves[0][ct]=i;}
	}

	dfa_moves[0][0]=ct;
	dfa_states=1;
	int ctr=0,k=0,r=0,ckt=0;
	bool flag=0;
	//for(int i=0;i<=ct;i++)cout<<dfa_moves[0][i]<<"++"<<endl;
	while(1)
	{
		k=dfa_states;
		for(int j=0;j<alpha;j++)
		{
			ctr=0;flag=0;ckt=0;
			// cout<<"sdv"<<endl;
			for(int i=0;i<100;i++)bool_arr[i]=0;
			for(int i=1;i<=dfa_moves[r][0];i++) moves(dfa_moves[r][i],abcd[j]);
			
			for (int i=1;i<100;i++){
				if(bool_arr[i]==1)
					{

						//cout<<i<<"-->";//}
						arr_2[ctr++]=i;}
			}
			//for(int x=0;x<=ctr;x++)cout<<arr_2[x]<<"arr"<<endl;
			ckt=0;flag=0;
			if(ctr!=0 )
			{
				for(int l=0;l<dfa_states;l++)
					{
						//if (ctr!=dfa_moves[l][0])flag=0;
						//cout<<ctr<<"fe--"<<r<<endl;
						flag=0;
						if(ctr==dfa_moves[l][0])
						{
							ckt=0;
							for(int m=1;m<=ctr;m++){
								//if(m==5)cout<<"ewwere";
								//cout<<dfa_moves[l][m]<<"####"<<arr_2[m-1]<<"mmm"<<m<<endl;
								if(dfa_moves[l][m]==arr_2[m-1]){
									
									ckt++;}//cout<<"ckt--"<<ckt<<"ctr--"<<ctr<<endl;}
								}

							//cout<<ckt<<"sdv"<<ctr<<endl;
							if(ckt==ctr)
							{

								flag=1;

								dfa_trans[r+1][l+1]=abcd[j];/*cout<<r+1<<"---"<<l+1<<"asdads"<<endl;*/break;
							}
						}					

					}
					//cout<<"asdfa";
					if(flag==0)
					{
						
						dfa_moves[dfa_states][0]=ctr;
						for(int i=0;i<ctr;i++)dfa_moves[dfa_states][i+1]=arr_2[i];
						dfa_trans[r+1][dfa_states+1]=abcd[j];
						//cout<<r+1<<"++"<<dfa_states+1<<endl;
						dfa_states++;
					}
				//r++;
				}
				//cout<<r<<"===";
			
		}
		if(dfa_states==k && r==dfa_states-1)break;
		r++;
	}
		// cout<<"\ntransition of DFA:\n";
		cout<<"transition table of final DFA\n"<<endl;
		cout<<"_________ ________ __________"<<endl;
		cout<<"   From  |   To   |   Input  |"<<endl;
		cout<<"_________|________|__________|"<<endl;
		for (int i=1; i<=50;i++)
			for (int j=1; j<=50;j++)
				if(dfa_trans[i][j]!='\0')
				{
					cout<<"    "<<i<<"    |"<<"    "<<j<<"   "<<"|    "<<char(dfa_trans[i][j])<<"    "<<endl;
				//cout<<i<<"-->"<<transition[i][j]<<"-->"<<j<<endl;
				}
		cout<<endl;
		// for (int i=0; i<=10;i++)
		// {
		// 	for (int j=0; j<=10;j++)
		// 		cout<<dfa_moves[i][j]<<"----";
		// 	cout<<endl;
		// }

	DFA_machine.total_states = dfa_states; //total state in dfa
	DFA_machine.start=1;
	int z=0;
	for(int i=0;i<=dfa_states-1;i++)
	{
		for(int j=1;j<=dfa_moves[i][0];j++)
		{
			if(dfa_moves[i][j]==final){DFA_machine.final[++z]=i+1;}
		}
	}
	DFA_machine.final[0]=z;
	//cout<<"Final states of DFA:\n";
	cout<<"total final states: "<<DFA_machine.final[0]<<endl;
	for(int i=1;i<=DFA_machine.final[0];i++)cout<<DFA_machine.final[i]<<"  ";
	cout<<"\n";
	// cout<<DFA_machine.total_states<<"total_states"<<endl;
	// cout<<DFA_machine.final[1]<<endl;
	// cout<<DFA_machine.final[2]<<endl;
}
int group[MAX][MAX];
bool dfa_final[MAX];
void DFA_min()
{
	for(int i=0;i<MAX;i++)dfa_final[i]=0;
	int total_group=0;
	int current=0,ct=0;
	group[0][0]=DFA_machine.final[0];
	for(int i=1;i<=DFA_machine.final[0];i++){group[0][i]=DFA_machine.final[i]; dfa_final[i]=1;}
	for(int i=1;i<=DFA_machine.total_states;i++){if(dfa_final[i]==1){group[1][++ct]=i;}} 
	group[1][0]=ct; //total states in group 2
	
}

/*simulation code*/
bool simulation(const char* str)
{
	int start=1, current=1;
	bool flag=0;
	while(*str!='\0')
	{
			flag=0;

			for (int i=1;i<=DFA_machine.total_states;i++)
			{
				//cout<<char(dfa_trans[current][i])<<endl;
				if(dfa_trans[current][i]==int(*str)){
					flag=1;
					current=i;break;
				}
			}
			//cout<<current<<"current"<<flag<<endl;
			if(flag==0)return 0;
		str++;
	}
	//cout<<current<<"current--"<<endl;
	for(int i=1;i<=DFA_machine.final[0];i++)if(current==DFA_machine.final[i])return 1;
	return 0;
}

int main()
{
	
	for (int i=0; i<MAX;i++)
		for (int j=0; j<MAX;j++)
			// for (int k=0; k<MAX;k++)
				{transition[i][j]='\0';
					dfa_trans[i][j]='\0';}

	string post, str;
	cout<<"Enter the Regular expression: "<<endl;
	cin>>str;
	post = infixTOpostfix (str);
	//const char *cstr = str.c_str();
	//cout << *(cstr+2)<<endl;
	//cout<<post<<endl;
	cout<<"NFA creation starting...\n"<<endl;
	create_NFA(post);
	 cout<<"NFA to DFA...\n"<<endl;
	 NFA_to_DFA();
	 cout<<"\n";
	 string input_str;
	 cout<<"simulation starting....\n";
	 cout<<"enter the string to check:"<<endl;
	 cin>>input_str;
	 const char *cstr = input_str.c_str();
	 if(simulation(cstr))cout<<"accept!!"<<endl;
	 else cout<<"not accept!!"<<endl;
	return 0;
}