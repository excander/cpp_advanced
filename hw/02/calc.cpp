#include<iostream>
#include<exception>

using namespace std;

char getlex(string &str);
int expr(string &str, char &curlex);
int add(string &str, char &curlex);
int mult(string &str, char &curlex);
size_t ind = 0;

char my_getchar(string &str){
    if (ind < str.size())
        return str[ind++];
    else {
        return '\n';
    }
}

char getlex(string &str){
    char curlex;
    while ( (curlex = my_getchar(str)) == ' ' );
    return curlex;
}

int expr(string &str, char &curlex){
    int e = add(str, curlex);
    while (curlex == '+' || curlex == '-') {
        if (curlex == '+') {
            curlex = getlex(str);
            e += add(str, curlex);
        }
        else if (curlex == '-') {
            curlex = getlex(str);
            e -= add(str, curlex);
        }
    }
    return e;
}

int add(string &str, char &curlex){
    int a=mult(str, curlex);
    while (curlex == '*' || curlex == '/') {
        if (curlex == '*'){
            curlex = getlex(str);
            a *= mult(str, curlex);
        }
        else if (curlex == '/') {
            curlex = getlex(str);
            int t=mult(str, curlex);
            if (t)
                a /= t;
            else{
                throw exception();
            }
        }
    }
    return a;
}

int mult(string &str, char &curlex){
    int m;
    switch(curlex){
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':{
            m= curlex-'0';
            break;
        }
        case '(': curlex = getlex(str); m=expr(str, curlex);
            if (curlex == ')'){
                break;
            }
        case '-': curlex = getlex(str); m=-1*mult(str, curlex);
                break;
        default : {
            throw exception();
        }
    }
    if (curlex!='\n' && curlex!='/'&& curlex!='*' && curlex!='+' && curlex!='-')
        curlex = getlex(str);
    return m;
}

int main(int argc, char* argv[]){

    int res;
    string str;
    char curlex;

    try{
        if(argc != 2)
            throw exception();
        else {
        str = argv[1];
            curlex = getlex(str);
            res=expr(str, curlex);
            cout << res << endl;
        }
    }
    catch (std::exception){
        cout << "error" << endl;
        return 1;
    }

    return 0;
}