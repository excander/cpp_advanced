#include <iostream>
#include "numbers.dat"

const int prime = 0;                   // простое
const int composite = -1;              // составное 
const int composite_in_data = -2;      // составное, находящееся в Data


void build_sieve(int *sieve, const int n){

    // строит Решето Эратосфена
    
    sieve[0] = composite;
    sieve[1] = composite;
    for (int i = 2; i < n; i++){
        sieve[i] = prime;
    }

    for (int i = 2; i*i <= n; i++)
        if(sieve[i] == prime){
            for(int j = i*i; j <= n; j+= i)
                sieve[j] = composite;
    }
}

void remake_sieve(int *sieve, const int n, const int *Data, const int k){

    // преобразует решето, накапливая частичную сумму (кол-во простых чисел до данного, включительно);
    // при этом разделяет составные числа на те, которые есть в Data и те, которых там нет

    int prime_count = 0;

    for (int i = 0; i < k; i++){
        int item = Data[i];
        if (sieve[item] == composite)
            sieve[item] = composite_in_data;
        else if (sieve[item] >= 0)
            sieve[item] = ++prime_count;
    }
}

int get_prime_count(int *sieve, const int n, int a, int b){

    // подсчитывает количество простых в sieve между a и b

    if (a > b || sieve[a] == composite || sieve[b] == composite){
        return 0;
    }
    else if (a == b && sieve[a] >= 0)
        return sieve[a];
    else if (a == b) 
        return 0;
    else {
        while (sieve[b] <= 0 && b > a)
            b--;
        if (b == a){
            return 0;
        }

        int diff = 0;
        if (sieve[a] >= 0)
            diff = 2;
        else while (sieve[a] <= 0 && a > 0)
            a--;
        int left = (sieve[a] >= 0) * sieve[a];
        return sieve[b] - left + diff;
    }
}

int main(int argc, char* argv[]){

    if (argc % 2 == 0 || argc < 3)
        return -1;

    else {
        int *sieve = new int [Size + 1];

        build_sieve(sieve, Size + 1);
        remake_sieve(sieve, Size + 1, Data, Size);

        for (int i = 1; i < argc; i++){
        
            int v1 = atoi(argv[i]);
            int v2 = atoi(argv[++i]);

            std::cout << get_prime_count(sieve, Size + 1, v1, v2) << std::endl;
        }

        delete []sieve;
        return 0;
    }
}