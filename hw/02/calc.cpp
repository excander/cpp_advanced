#include<iostream>
#include<exception>

using namespace std;

char curlex;
void getlex(void);
int expr(void);
int add(void);
int mult(void);
string str;
size_t ind = 0;

char my_getchar(){
	if (ind < str.size())
		return str[ind++];
	else {
		return '\n';
	}
}

void getlex(){
    while ( (curlex = my_getchar()) == ' ' );
}

int expr(){
    int e = add();
    while (curlex == '+' || curlex == '-') {
        if (curlex == '+') {
            getlex();
            e += add();
        }
        else if (curlex == '-') {
            getlex();
            e -= add();
        }
    }
    return e;
}

int add(){
    int a=mult();
    while (curlex == '*' || curlex == '/') {
        if (curlex == '*'){
            getlex();
            a *= mult();
        }
        else if (curlex == '/') {
            getlex();
            int t=mult();
            if (t)
                a /= t;
            else{
            	throw exception();
            }
        }
    }
    return a;
}

int mult(){
    int m;
    switch(curlex){
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':{
            m= curlex-'0';
            break;
        }
        case '(': getlex(); m=expr();
            if (curlex == ')'){
                break;
            }
        case '-': getlex(); m=-1*mult();
     			break;
        default : {
        	throw exception();
        }
    }
    if (curlex!='\n' && curlex!='/'&& curlex!='*' && curlex!='+' && curlex!='-')
        getlex();
    return m;
}

int main(int argc, char* argv[]){

	int res;

	try{
        if(argc != 2)
            throw exception();
        else {
       	str = argv[1];
        	getlex();
    		res=expr();
        	cout << res << endl;
        }
    }
    catch (std::exception){
        cout << "error" << endl;
        return 1;
    }

    return 0;
}