#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include <ctime>
#include <utility>
#include <math.h>

using namespace std;
int T;
int INTERVALO = 1;
string instancia ="St.Andrews83.stu";//"EdHEC92.stu";
int max_iter1 = 10000;
int max_iter2 = 50000;
float alpha = 0.95;
int maxT=150;

void getSE(int *maxStu,int *maxE){
    std::ifstream archivo;
    archivo.open("./instances/"+instancia,ios::in);
    string a;
    int b,c; 
    *maxStu=0; 
    *maxE=0;
    while(archivo>>a>>b)
    {
        a = a.substr(1, a.size()-1);
        c = std::stoi(a);
        if(c> *maxStu) *maxStu = c;
        if(b> *maxE) *maxE = b;
    }
    archivo.close();
}

void matrixSE(int **SE, int S, int E){
    for(int i=0;i<S;i++){
        for(int j=0;j<E;j++){
            SE[i][j]=0;
        }
    }
    std::ifstream archivo;
    archivo.open("./instances/"+instancia,ios::in);
    string estudiante;
    int examenID, estudianteID;
    while(archivo>>estudiante>>examenID)
    {
        estudiante = estudiante.substr(1, estudiante.size()-1); //se le quita la "s" del id
        estudianteID = std::stoi(estudiante); //el id se transforma a int
        SE[estudianteID-1][examenID-1] = 1;
    }
    archivo.close();
}

void matrixC(int **matrizSE, int **C, int S,int E){
    std::vector<int> fila[S];
    for(int i=0;i<S;i++){
        for(int j=0;j<E;j++){
            if(matrizSE[i][j]==1)
                fila[i].push_back(j);
        }
    }
    
    for(int i=0; i<E; i++){
        for(int j=0; j<E; j++){
            C[i][j]=0;
        }
    }

    int largoVector;
    for(int i=0; i< S;i++){
        largoVector = fila[i].size();
        if(largoVector>1){
            for(int j=0; j<largoVector; j++){
                for(int k=j+1; k<largoVector;k++){
                    C[fila[i][j]][fila[i][k]] = 1;
                    C[fila[i][k]][fila[i][j]] = 1;
                }
            }
        }
    }
}

bool checkeo(std::vector<int> x, int **C, int E){
    int suma = 0;
    for(int i=0; i <E-1;i++){
        for (int j = i+1; j < E; j++){
            if(x[i]!=0 && x[i]==x[j])
                suma += C[i][j];
        }
    }
    if(suma==0) return true;
    else return false;
}

int numeroAleatorio (int max){
    if (max <T) return (std::rand()%(max+INTERVALO)+1);
    else return (std::rand()%T+1);
}

std::vector<int> solucionAleatoria(int E, int** C){
    std::vector<int> v(E,0);
    bool verificar;
    int aux, max=1;
    for(int i=0; i< v.size();i++){
        do{
            aux = numeroAleatorio(max);
            v.at(i)= aux;
            verificar = checkeo(v,C,E);
        }while(!verificar);
        if(max<aux) max = aux;
    }
    return v;
}

void escribir(std::vector<int> v){
    ofstream txtOut;
    txtOut.open("soluciones.txt",ios::app);
    txtOut<<"Nuevo:\n";
    for (auto i : v) {
        txtOut << i<<"\n";
    }
    txtOut.close();
}

auto solucionCandidata(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=1000000, max=0;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    for (int i=0; i< x.size();i++){
        aux = x;
        if(x[i]==min){
            aux[i]=aux[i]+1;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }
        else if(x[i]==max){
            aux[i]=aux[i]-1;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }
        else{
            aux[i]=aux[i]+1;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
            aux[i]=aux[i]-2;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }
        /*if(x[i]>1){
            aux[i]=aux[i]-1;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }*/
    }

    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        return std::make_pair(false,x);
    }
}

int penalizacion(int i, int j){
    int intervalo = abs(i-j);
    int exp;
    int sol;
    if (1<=intervalo && intervalo<=5){
        exp = 5- intervalo;
        return pow(2,exp);
    }else return 0;
}

float calidad(std::vector<int> x, int** C, int E, int S){
    int min=1000000, max=0;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    int delta = max-min;
    int suma = 0;
    for(int i=0;i<x.size()-1;i++){
        for(int j=i+1;j<x.size();j++){
            suma += C[i][j]*penalizacion(x[i],x[j]);
        }
    }
    return suma+delta*50;
}

bool evaluacion(std::vector<int> sn, std::vector<int> sc, int Temp, int** C, int E, int S){
    float c_sn = calidad(sn,C,E,S);
    float c_sc = calidad(sc,C,E,S);
    float delta = c_sn-c_sc;
    float aleatorio = drand48();
    if(delta<0) return true;
    else if (exp(-delta/Temp)>aleatorio) return true;
    else return false;
}

auto simulatedAnnealing(int **C, int E, int S){
    int t=0;
    int Temp=maxT;
    int contador1, contador2=0;
    bool cond1=true,cond2=true, fallo=false;
    std::vector<int> sc = solucionAleatoria(E, C);
    for (auto i : sc) {
        cout << i<<" ";
    }
    cout<<endl;
    cout<<"Calidad: "<<calidad(sc,C,E,S)<<endl;
    std::vector<int> sb = sc;
    std::vector<int> aux;
    
    do
    {
        contador1 = 0;
        aux = sb;
        do
        {
            auto sn = solucionCandidata(sc,C,E);
            /*for (auto i : sn.second) {
                cout << i<<" ";
            }
            cout<<endl;*/
            if(!sn.first) fallo=true;
            else if (evaluacion(sn.second,sc,Temp,C,E,S)) sc=sn.second;
            if(calidad(sc,C,E,S)<calidad(sb,C,E,S)) {
                sb=sc;
                cout<<"Mejoró con calidad: "<<calidad(sb,C,E,S)<<endl;
            }
            else contador1++;
            if(contador1==max_iter1) cond1 = false;
            
        } while (cond1 && !fallo);
        Temp = alpha*Temp;
        if(calidad(sb,C,E,S)==calidad(aux,C,E,S)) contador2++;
        if(contador2==max_iter2) cond2=false;
        if (contador2%10==0 && Temp<5) Temp= maxT;
    } while (cond2 && !fallo);
    
    return std::make_pair(sb,calidad(sb,C,E,S));

}

int main(int argc, char const *argv[])
{
    std::srand(unsigned(std::time(nullptr)));
    int S,E;
    getSE(&S,&E);
    T=E;

    int ** matrizSE = new int*[S];
    for (int i = 0; i<S;i++)
        matrizSE[i]= new int[E];
    matrixSE(matrizSE,S,E);

    int **conflictMatrix = new int*[E];
    for(int i=0; i<E; i++)
        conflictMatrix[i] = new int[E];
    matrixC(matrizSE, conflictMatrix, S, E);

    /*auto sol_final = simulatedAnnealing(conflictMatrix,E,S);
    for (auto i : sol_final.first) {
        cout << i<<" ";
    }
    cout<<endl;
    cout<<"Penalizacion: "<<sol_final.second<<endl;*/
    std::vector<int> conflictos;
    int suma;
    for(int i=0;i<E;i++){
        suma=0;
        for(int j=0;j<E;j++)
            suma +=conflictMatrix[i][j];
        conflictos.push_back(suma);
    }
    cout<<endl;
    for(auto i: conflictos)
        cout<<i<<" ";
    cout<<endl;
    return 0;
}
