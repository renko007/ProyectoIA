
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<vector>
#include<math.h>

using namespace std;
int T;
int INTERVALO = 1;
string instancia ="St.Andrews83";//"EdHEC92";//"St.Andrews83";//"Carleton91";
int max_iter1 = 3500;
int max_iter2 = 5;
float alpha = 0.8;
int maxT=100;

void getSE(int *maxStu,int *maxE){
    std::ifstream archivo;
    archivo.open("./instances/"+instancia+".stu",ios::in);
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
    archivo.open("./instances/"+instancia+".stu",ios::in);
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
                    C[fila[i][j]][fila[i][k]] += 1;
                    C[fila[i][k]][fila[i][j]] += 1;
                }
            }
        }
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
    /*int min=1000000, max=0;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    int delta = max-min;*/
    int suma = 0;
    for(int i=0;i<x.size()-1;i++){
        for(int j=i+1;j<x.size();j++){
            suma += C[i][j]*penalizacion(x[i],x[j]);
        }
    }
    return suma;
}

std::vector<int> solucionReal(){
    std::ifstream archivo;
    archivo.open("./instances/"+instancia+".sol",ios::in);
    string a;
    int b;
    std::vector<int> x;
    while(archivo>>a>>b)
    {
        x.push_back(b);
    }
    archivo.close();
    return x;
}

int main(int argc, char const *argv[])
{
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
    

    /*for(int i=0;i<E;i++){
        for(int j=0;j<E;j++){
            cout<<conflictMatrix[i][j]<<" ";
        }
        cout<<endl;
    }*/

    cout<<"Solucion Conocida:"<<endl;
    std::vector<int> z = solucionReal();
    for (auto i : z) {
        cout << i<<" ";
    }
    cout<<endl;
    cout<<"Penalizacion Promedio: "<<calidad(z,conflictMatrix,E,S)/S<<endl;

    return 0;
}