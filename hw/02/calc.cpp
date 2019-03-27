#include<iostream>
#include<exception>

using namespace std;

char curlex;
void getlex(void);
int expr(void);
int add(void);
int mult(void);
void error();
string str;
size_t ind = 0;

char my_getchar(){
	if (ind < str.size())
		return str[ind++];
	else {
		// cout << "mY_getchar_return_"<< endl;
		return '\n';
	}
}

void getlex(){
    while ( (curlex = my_getchar()) == ' ' );
}

void error(void){
    printf("\nERROR!\n");
    while(my_getchar()!='\n');
    // longjmp(begin,1);

}

int expr(){
    int e = add();
    // cout << "curlex_add = " << curlex << endl;
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
            	cout<< "error 1";
                error();
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
        	// cout << "yep";
            m= curlex-'0';
            // m = trypow(m);
            break;
        }
        case '(': getlex(); m=expr();
            if (curlex == ')'){
                // m = trypow(m);
                break;
            }
        case '-': getlex(); m=-1*mult();
     			break;
        default : {
        	cout << "curlex = " << curlex << endl;
        	// cout << str[ind+1] << endl;
        	cout<< "error 3";
         	error();
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
            // str = "2+2";
        	getlex();
    		res=expr();
        	cout << res << endl;
        }
    }
    catch (std::exception){
        cout << "Error!" << endl;
        return 1;
    }

    return 0;
}