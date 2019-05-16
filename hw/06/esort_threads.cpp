// Выполнить сортировку бинарного файла содержащего числа uint64_t в 2 потока.
// Доступно 8 Мб памяти, больше выделять нельзя ни явно, ни неявно (например, в виде контейнеров),
// при этом файл может быть размером превышающим доступную память.
// Пространство на диске можно считать бесконечным. Сортировку выполнять в новый файл.
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <cmath>
#include <thread>
#include <limits>    
#include <stdexcept> 

int Uint64_tToInt(uint64_t u){
    if (u > std::numeric_limits<int>::max()){
        throw std::overflow_error(
            "uint64_t value cannot be stored in a variable of type int.");
    }
    return static_cast<int>(u);
}

int tell_size(const std::string &filePath){
    // возвращает количество чисел в filePath
    std::streampos fsize = 0;
    std::ifstream file(filePath, std::ios::binary);

    fsize = file.tellg();
    file.seekg(0, std::ios::end);
    fsize = file.tellg() - fsize;
    file.close();

    return Uint64_tToInt(fsize / sizeof(uint64_t));
}

void merge(const std::string &ifilename, const std::string &of1name, const std::string &of2name, int N, int k){
    std::ofstream f_(ifilename, std::ios::binary);
    std::ifstream f_1(of1name, std::ios::binary);
    std::ifstream f_2(of2name, std::ios::binary);

    int i, j;
    uint64_t s_1, s_2;
    f_1.read((char*)&s_1, sizeof(uint64_t));
    f_2.read((char*)&s_2, sizeof(uint64_t));
    for (int q = 0; q < N/(1<<k); q++){
        i = j = 1;
        while(i <= (1 << (k - 1)) || j <= (1 << (k - 1))) {
            while (i <= (1 << (k - 1)) && (s_1 <= s_2 || j > (1 << (k-1)))) {
                f_.write((char*)&s_1, sizeof(uint64_t));
                // std::cout << "W: f_: " << s_1 << std::endl;
                f_1.read((char*)&s_1, sizeof(uint64_t));
                // std::cout << "R: f_1: " << s_1 << std::endl;
                i++; 
            }
            while (j <= (1 << (k - 1)) && (s_1 >= s_2 || i > (1 << (k-1)))) {
                f_.write((char*)&s_2, sizeof(uint64_t));
                // std::cout << "W: f_: " << s_2 << std::endl;
                f_2.read((char*)&s_2, sizeof(uint64_t));
                // std::cout << "R: f_2: " << s_2 << std::endl;
                j++;
            }
        }
    }
    if (N % (1 << k)){
        int a, b, ost = (N - (N / (1 << k)) * (1 << k));
        a = ost >= (1 << (k - 1)) ? (1 << (k - 1)) : ost;
        b = ost - a;
        i = j = 1;
        while(i <= a || j <= b) {
            while (i <= a && (s_1 <= s_2  ||  j > b)) {
                f_.write((char*)&s_1, sizeof(uint64_t));
                // std::cout << "W: f_: " << s_1 << std::endl;
                f_1.read((char*)&s_1, sizeof(uint64_t));
                // std::cout << "R: f_1: " << s_1 << std::endl;
                i++; 
            }
            while (j <= b && (s_1 >= s_2 || i > a)) {
                f_.write((char*)&s_2, sizeof(uint64_t));
                // std::cout << "W: f_: " << s_2 << std::endl;
                f_2.read((char*)&s_2, sizeof(uint64_t));
                // std::cout << "R: f_2: " << s_2 << std::endl;
                j++;
            }
        }
    }
}

void separate(const std::string &ifilename, const std::string &of1name, const std::string &of2name, int N){
    uint64_t s1, s2;

    int flag1,flag2;
    int k = 1, M = N % (1 << k) == 0 ? N / (1 << k) : N/(1 << k) + 1;
    while ((1 << (k - 1)) < N){

        std::ifstream ifile(ifilename);
        std::ofstream of1(of1name);
        std::ofstream of2(of2name);

        flag1 = flag2 = 1;
        for (int j = 0; (j < M && (flag1 || flag2)); j++) {
            for (int i = 0; i < ((1 << k) / 2); i++) {
                if (!ifile.read((char*)&s1, sizeof(uint64_t)).eof()) {
                    of1.write((char*)&s1, sizeof(uint64_t));
                    // std::cout << "W: of1: " << s1 << std::endl;
                } else {
                    flag1 = 0;
                    break;
                }
            }
            for (int i = 0; i < ((1 << k) / 2); i++) {
                if (!ifile.read((char*)&s2, sizeof(uint64_t)).eof()) {
                    of2.write((char*)&s2, sizeof(uint64_t));
                    // std::cout << "W: of2: " << s2 << std::endl;
                } else {
                    flag2 = 0;
                    break;
                }
            }
        }
        ifile.close();
        of1.close();
        of2.close();
        merge(ifilename, of1name, of2name, N, k);
        k++;
    }
}

void divide_file(const std::string &ifilename, const std::string &of1name, const std::string &of2name, int N){
    // как separate, только выполняется 1 раз и не вызывает merge
    uint64_t s1, s2;

    int flag1, flag2;
    int k = 1, M = N %(1 << k) == 0 ? N / (1 << k) : N / (1 << k) + 1;

    std::ifstream ifile(ifilename);
    std::ofstream of1(of1name);
    std::ofstream of2(of2name);

    flag1 = flag2 = 1;
    for (int j = 0; (j < M && (flag1 || flag2)); j++) {
        for (int i = 0; i < ((1 << k) / 2); i++) {
            if (!ifile.read((char*)&s1, sizeof(uint64_t)).eof()) {
                of1.write((char*)&s1, sizeof(uint64_t));
            } else {
                flag1 = 0;
                break;
            }
        }
        for (int i = 0; i < ((1 << k) / 2); i++) {
            if (!ifile.read((char*)&s2, sizeof(uint64_t)).eof()) {
                of2.write((char*)&s2, sizeof(uint64_t));
            } else {
                flag2 = 0;
                break;
            }
        }
    }
    ifile.close();
    of1.close();
    of2.close();
}

int main(int argc, char **argv)
{
    std::string ifilename = "in.bin";
    std::string of1name = "of1.bin";
    std::string of2name = "of2.bin";
    std::string of3name = "of3.bin";
    std::string of4name = "of4.bin";

    int N = tell_size(ifilename);

    std::string if1name = "if1.bin";
    std::string if2name = "if2.bin";
    divide_file(ifilename, if1name, if2name, N);

    std::thread t1(&separate, cref(if1name), cref(of1name), cref(of2name), tell_size(if1name));
    std::thread t2(&separate, cref(if2name), cref(of3name), cref(of4name), tell_size(if2name));
    t1.join();
    t2.join();

    std::string ofilename = "out.bin";
    merge(ofilename, if1name, if2name, N, std::log2(N));
    
    remove("of1.bin");
    remove("of2.bin");
    remove("of3.bin");
    remove("of4.bin");
    remove("if1.bin");
    remove("if2.bin");

    return 0;
}