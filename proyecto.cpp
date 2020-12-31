#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<vector>
#include <algorithm>
#include <ctime>

using namespace std;
int T;
int INTERVALO = 1;
string instancia ="St.Andrews83.stu";//"EdHEC92.stu";

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
    int aux, max=5;
    for(int i=0; i< v.size();i++){
        do{
            aux = numeroAleatorio(max);
            v.at(i)= aux;
            verificar = checkeo(v,C,E);
        }while(!verificar);
        if(max<aux) max = aux;
    }
    cout<<max<<endl;
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

    std::vector<int> x = solucionAleatoria(E, conflictMatrix);
    escribir(x);


    for (auto i : x) {
        cout << i<<" ";
    }
    cout<<endl;
    // PRINT DE LA MATRIZ DE CONFLICTOS
    /*for(int i=0; i<E;i++){
        for(int j=0; j<E;j++){
            cout<<conflictMatrix[i][j];
        }
        cout<<"\n";
    }*/
    return 0;
}
