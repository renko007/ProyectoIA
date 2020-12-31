#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include <ctime>
using namespace std;

int T=3;

void checkeo(std::vector<int> x){
    cout<<x[4]<<endl;
}
int numeroAleatorio() {
    return (std::rand()%5+1);
}

std::vector<int> solucionAleatoria(int E){
    std::vector<int> v(E);
    std::generate(v.begin(), v.end(), numeroAleatorio);
    return v;
}

void escribir(std::vector<int> v){
    ofstream txtOut;
    txtOut.open("solucion.txt",ios::app);
    txtOut<<"Nuevo:\n";
    for (auto i : v) {
        txtOut << i<<"\n";
    }
    txtOut.close();
}

int main(int argc, char const *argv[])
{
    int x[4] ={1,2,3,4};
    //checkeo(x);
    std::srand(1);
    std::vector<int> v = solucionAleatoria(10);
    for (auto i : v) {
        cout << i << " ";
    }
    cout<<endl;
    escribir(v);
    /*std::generate(v.begin(), v.end(), numeroAleatorio(10));
    for (int i=0;i< v.size();i++) {
        cout << v[i] << " ";
    }
    cout<<endl;*/
    //checkeo(v);
    return 0;
}
