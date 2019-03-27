#include <iostream>
#include <exception>

char getlex(const std::string &str);
int64_t expr(const std::string &str, char &curr);
int64_t add(const std::string &str, char &curr);
int64_t mult(const std::string &str, char &curr);
size_t ind = 0;

char my_getchar(const std::string &str){
    if (ind < str.size())
        return str[ind++];
    else return '\n';
}

char getlex(const std::string &str){
    char curr;
    while ((curr = my_getchar(str)) == ' ');
    return curr;
}

int64_t expr(const std::string &str, char &curr){
    int64_t e = add(str, curr);
    while (curr == '+' || curr == '-') {
        if (curr == '+') {
            curr = getlex(str);
            e += add(str, curr);
        }
        else if (curr == '-') {
            curr = getlex(str);
            e -= add(str, curr);
        }
    }
    return e;
}

int64_t add(const std::string &str, char &curr){
    int64_t a = mult(str, curr);
    while (curr == '*' || curr == '/') {
        if (curr == '*'){
            curr = getlex(str);
            a *= mult(str, curr);
        }
        else if (curr == '/') {
            curr = getlex(str);
            int64_t t = mult(str, curr);
            if (t)
                a /= t;
            else{
                throw std::exception();
            }
        }
    }
    return a;
}

int64_t mult(const std::string &str, char &curr){
    int64_t m;
    switch(curr){
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':{
            m = curr - '0';
            break;
        }
        case '(': curr = getlex(str); m = expr(str, curr);
            if (curr == ')'){
                break;
            }
        case '-': curr = getlex(str); m = -1 * mult(str, curr);
                break;
        default : {
            throw std::exception();
        }
    }
    if (curr != '\n' && curr != '/' && curr != '*' && curr != '+' && curr != '-')
        curr = getlex(str);
    return m;
}

int main(int argc, char* argv[]){

    int64_t res;
    std::string str;
    char curr;

    try{
        if(argc != 2)
            throw std::exception();
        else {
        str = argv[1];
            curr = getlex(str);
            res = expr(str, curr);
            std::cout << res << std::endl;
        }
    }
    catch (std::exception){
        std::cout << "error" << std::endl;
        return 1;
    }

    return 0;
}