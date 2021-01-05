#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<ctime>
#include<utility>
#include<math.h>
#include<time.h>
#include<sstream>
#include<iomanip>

using namespace std;
int T;
int INTERVALO = 1;
int max_iter1 = 3500;
int max_iter2 = 5;
float alpha = 0.9;
int maxT=100;
string instancia ="Carleton91";//"EdHEC92";//"St.Andrews83";//"Carleton91";//"LSE91";//"TorontoE92";//"Trent92";//"YorkMills83"
//EdHEC92 140.322 [s]
//St.Andrews83 640.731 [s]
//LSE91 12112.9 [s]
//Carleton91 9hr 20min aprox
//TorontoE92 1599.35 [s]
//Trent92 3296.26 [s]
//YorkMills83 1083.37 [s]

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

bool checkeo(std::vector<int> x, int **C, int E){
    int suma = 0;
    for(int i=0; i <E-1;i++){
        for (int j = i+1; j < E; j++){
            if(x[i]!=0 && x[i]==x[j])
                suma += C[i][j];
            if(suma>0) break;
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

std::vector<int> solucionAleatoria2(int E, int** C){
    std::vector<int> v(E,0);
    bool verificar,hecho;
    int max,pos, bloque;
    std::vector<int> conflictos;
    int suma;
    for(int i=0;i<E;i++){
        suma=0;
        for(int j=0;j<E;j++)
            suma +=C[i][j];
        conflictos.push_back(suma);
    }
    for(int i=0;i<E;i++){
        max=-1;
        hecho = false;
        for(int j=0;j<E;j++){
            if(conflictos[j]>max){
                max = conflictos[j];
                pos = j;
                hecho=true;
            }
        }
        if(hecho){
            conflictos[pos]=-1;
            bloque=1;
            do{
                v.at(pos)= bloque;
                verificar = checkeo(v,C,E);
                bloque++;
            }while(!verificar);
        }
    }
    cout<<"sol inicial pasa?: "<<checkeo(v,C,E)<<endl;
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

auto solucionCandidata1(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0;
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
    }

    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        return std::make_pair(false,x);
    }
}

//Random y vecinos a 1 distancia.
auto solucionCandidata2(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0,aux2,pos;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    for (int i=0; i< x.size();i++){
        aux = x;
        do{
            aux[i]= std::rand()%max+1;
        }while(aux[i]<min);
        
        if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        if(aux[i]==min){
            aux[i]=aux[i]+1;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }
        else if(aux[i]==max){
            aux[i]=aux[i]-1;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }
        else{
            aux[i]=aux[i]-1;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }
    }
    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        cout<<"No hubo vecindario factible."<<endl;
        return std::make_pair(false,x);
    }
}

auto solucionCandidata3(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0,aux2,pos;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    for (int i=0; i< x.size();i++){
        aux = x;
        for(int j=min; j<max;j++){
            aux[i]=j;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }
    }
    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        cout<<"No hubo vecindario factible."<<endl;
        return std::make_pair(false,x);
    }
}

//SWAP
auto solucionCandidata4(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int aux2,num;
    num = std::rand()%E;
    for(int i=0;i<x.size();i++){
        aux = x;
        aux2= aux[i];
        aux[i]= aux[num];
        aux[num]= aux2;
        if(checkeo(aux,C,E)) {
            vector_soluciones.push_back(aux);
        }
    }
    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        cout<<"No hubo vecindario factible."<<endl;
        return std::make_pair(false,x);
    }
}

//SWAP MAX y SWAP-1
auto solucionCandidata5(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> y,aux;
    int min=10000000, max=0,aux2,pos;
    for(int i=0; i<x.size(); i++){
        if (x[i]<min) min = x[i];
        if(x[i]>max) {
            max = x[i];
            pos = i;
        }
    }
    
    for(int i=0;i<x.size();i++){
        aux = x;
        aux2= aux[i];
        aux[i]= aux[pos];
        aux[pos]= aux2;
        if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        if(aux[i]>1) {
            aux[i]= aux[i]-1;
            if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
        }
    }
    
    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        cout<<"No hubo vecindario factible."<<endl;
        return std::make_pair(false,x);
    }
}

//SWAP random
auto solucionCandidata6(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0,aux2,pos,num;
    float aleatorio = drand48();
    for(int i=0; i<x.size(); i++){
        if (x[i]<min) min = x[i];
        if(x[i]>max) {
            max = x[i];
        }
    }
    pos = std::rand()%E;
    do{
        num = std::rand()%max+1;//std::rand()%max+1;
    }while(num<min);

    for(int i=0;i<x.size();i++){
        aux = x;
        aux2= aux[i];
        aux[i]= num;
        aux[pos]= aux2;
        if(checkeo(aux,C,E)) {
            vector_soluciones.push_back(aux);
        }
    }
    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        cout<<"No hubo vecindario factible."<<endl;
        return std::make_pair(false,x);
    }
}

//Random unico
auto solucionCandidata7(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0,aux2,pos;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    for (int i=0; i< x.size();i++){
        aux = x;
        do{
            aux[i]= std::rand()%max+1;
        }while(aux[i]<min);
        if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
    }
    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        cout<<"No hubo vecindario factible."<<endl;
        return std::make_pair(false,x);
    }
}

//doble random
auto solucionCandidata8(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0,aux2,pos;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    int num1, num2;
    for (int i=0; i< 2*x.size();i++){
        aux = x;
        num1 = std::rand()%x.size();
        num2 = std::rand()%x.size();
        do{
            aux[num1]= std::rand()%max+1;
            aux[num2]= std::rand()%max+1;
        }while(aux[num1]<min && aux[num2]<min);
        if(checkeo(aux,C,E)) vector_soluciones.push_back(aux);
    }
    int L = vector_soluciones.size();
    if(L>0){
        return std::make_pair(true,vector_soluciones[std::rand()%L]);
    }
    else{
        cout<<"No hubo vecindario factible."<<endl;
        return std::make_pair(false,x);
    }
}

int penalizacion(int i, int j){
    int intervalo = abs(i-j);
    int exp;
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

bool evaluacion(std::vector<int> sn, std::vector<int> sc, int Temp, int** C, int E, int S){
    float c_sn = calidad(sn,C,E,S);
    float c_sc = calidad(sc,C,E,S);
    float delta = c_sn-c_sc;
    float aleatorio = drand48();
    if(delta<0) {
        //cout<<"Aceptó una mejor"<<endl;
        return true;
    }
    else if (exp(-delta/Temp)>aleatorio){ 
        //cout<<"Aceptó una peor."<<endl;
        return true;
    }
    else return false;
}

auto simulatedAnnealing(int **C, int E, int S){
    int t=0;
    int Temp=maxT;
    int contador1, contador2=0;
    bool cond1=true,cond2=true, fallo=false;
    std::vector<int> sc = solucionAleatoria2(E, C);
    cout<<"Solución inicial: "<<endl;
    int bloqueMax=-1;
    for (auto i : sc) {
        cout << i-1<<" ";
        if(i>bloqueMax) bloqueMax=i;
    }
    cout<<endl;
    cout<<"N° de Timeslots Solución Inicial: "<<bloqueMax<<endl;
    cout<<"Penalización Promedio Solución Inicial: "<<calidad(sc,C,E,S)/S<<endl;
    cout<<"Calidad Solución Inicial: "<<calidad(sc,C,E,S)<<endl;
    std::vector<int> sb = sc;
    std::vector<int> aux;
    
    do
    {
        contador1 = 0;
        cond1=true;
        aux = sb;
        do
        {
            //auto sn = solucionCandidata1(sc,C,E);
            //auto sn = solucionCandidata2(sc,C,E);
            //auto sn = solucionCandidata3(sc,C,E);
            //auto sn = solucionCandidata4(sc,C,E);
            //auto sn = solucionCandidata5(sc,C,E);
            auto sn = solucionCandidata6(sc,C,E);
            //auto sn = solucionCandidata7(sc,C,E);
            //auto sn = solucionCandidata8(sc,C,E);
            /*for (auto i : sn.second) {
                cout << i<<" ";
            }
            cout<<endl;*/
            if(!sn.first) {
                fallo=true;
                cout<<"falló en encontrar vecindad"<<endl;
            }
            else if (evaluacion(sn.second,sc,Temp,C,E,S)) sc=sn.second;
            if(calidad(sc,C,E,S)<calidad(sb,C,E,S)) {
                sb=sc;
                cout<<"Mejoró con calidad: "<<calidad(sb,C,E,S)<<endl;
                contador2=0;
            }
            
            contador1++;
            if(contador1%500==0) cout<<"contador1: "<<contador1<<endl;
            if(contador1==max_iter1) cond1 = false;
        } while (cond1 && !fallo);
        if(Temp<=5) {
            cond2 = false;
            cout<<"Fin del SA por alcanzar Temperatura mínima."<<endl;
        }
        else Temp = alpha*Temp;
        cout<<"Temperatura: "<<Temp<<endl;
        if(calidad(sb,C,E,S)==calidad(aux,C,E,S)) contador2++;
        cout<<"contador2: "<<contador2<<endl;
        if(contador2==max_iter2) {
            cout<<"Fin del SA por no mejorar en "<<max_iter2<<" iteraciones seguidas."<<endl;
            cond2=false;
        }
    } while (cond2 && !fallo);
    
    return std::make_pair(sb,calidad(sb,C,E,S));
}

void escribirSol(std::vector<int> v){
    ofstream txtOut;
    txtOut.open("./soluciones/"+instancia+".sol");//,ios::app);
    std::vector<std::stringstream> examenes (v.size());
    std::stringstream id;
    int aux;
    for (int i=0;i<v.size();i++) {
        aux = i;
        examenes[i] << std::setw(4) << std::setfill('0') << aux+1;
        txtOut << examenes[i].str()<<"\t"<<v[i]<<"\n";
    }
    txtOut.close();
}

void escribirRes(std::vector<int> v){
    ofstream txtOut;
    txtOut.open("./soluciones/"+instancia+".res");//,ios::app);
    int max=-1;
    for (int i=0;i<v.size();i++) {
        if(v[i]>max) max=v[i];
    }
    txtOut << max+1<<"\n";
    txtOut.close();
}

void escribirPen(std::vector<int> v, int **C, int E, int S){
    ofstream txtOut;
    txtOut.open("./soluciones/"+instancia+".pen");//,ios::app);
    txtOut <<calidad(v,C,E,S)/S<<"\n";
    txtOut.close();
}

int main(int argc, char const *argv[])
{
    clock_t inicio = clock();
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

    auto sol_final = simulatedAnnealing(conflictMatrix,E,S);
    
    cout<<"Tiempo Ejecución SA: "<<(double)(clock() - inicio)/CLOCKS_PER_SEC<<" [s]"<<endl;
    
    std::vector<int> y = sol_final.first;

    for(int i=0;i<y.size();i++) y.at(i)-=1;
    cout<<"Solución Final: "<<endl;
    int bloqueMax=-1;
    for (auto i : y) {
        cout << i <<" ";
        if(i>bloqueMax) bloqueMax=i;
    }
    cout<<endl;
    cout<<"N° de Timeslots Solución Final: "<<bloqueMax+1<<endl;
    cout<<"Penalización Promedio Solución Final: "<<calidad(y,conflictMatrix,E,S)/S<<endl;
    escribirSol(y);
    escribirRes(y);
    escribirPen(y,conflictMatrix,E,S);
    cout<<"Archivos escritos con éxito."<<endl;
    return 0;
}
