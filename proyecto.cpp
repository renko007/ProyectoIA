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

int max_iter1 = 3000;
int max_iter2 = 5;
float alpha = 0.85;
string instancia ="EdHEC92";//"EdHEC92";//"St.Andrews83";//"Carleton91";//"LSE91";//"TorontoE92";//"Trent92";//"YorkMills83"

//Función para obtener la cantidad S de estudiantes y la cantidad E de exámenes.
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

//Función para crear la matriz de Estudiantes x Exámenes donde SE_ij = 1 si el estudiante i rinde el examen j, 0 si no.
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

//Función para crear la matriz de conflictos entre examenes donde C_ij = k si k alumnos tienen que rendir examenes i y j.
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

//Función que verifica que una solución x, examenes en conflictos no tengan el mismo timeslot
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

//Función para crear la solución inicial.
//Se realiza tomando la decisión greedy de que al examen con más conflictos se le asigna
//un timeslot lo más bajo posible
std::vector<int> solucionInicial(int E, int** C){
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
    return v;
}

// Movimiento "+1-1" a cada elemento del vector uno por uno.
auto movimiento1(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    for (int i=0; i< (int)x.size();i++){
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

//Movimiento: uno por uno, cada elemento del vector solución, se le asigna un timeslot al azar.
auto movimiento2(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0;
    for(auto i: x){
        if (i<min) min = i;
        if(i>max) max = i;
    }
    for (int i=0; i< (int)x.size();i++){
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

//Movimiento: uno por uno, cada elemento del vector solución, se le realiza
//un SWAP con otro elemento al azar del vector solución.
auto movimiento3(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int aux2,num;
    num = std::rand()%E;
    for(int i=0; i<(int)x.size();i++){
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


//Movimiento: uno por uno, cada elemento del vector solución,SWAP con
//otro elemento al azar del vector solución, donde, además, se reemplaza su valor por un
//timeslot al azar
auto movimiento4(std::vector<int> x, int** C, int E){
    std::vector<std::vector<int>> vector_soluciones; //vecindario factible de x
    std::vector<int> aux;
    int min=10000000, max=0,aux2,pos,num;
    for(int i=0; i<(int)x.size(); i++){
        if (x[i]<min) min = x[i];
        if(x[i]>max) {
            max = x[i];
        }
    }
    pos = std::rand()%E;
    do{
        num = std::rand()%max+1;
    }while(num<min);

    for(int i=0;i<(int)x.size();i++){
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

//Función que calcula la penalización entre dos timeslots.
int penalizacion(int i, int j){
    int intervalo = abs(i-j);
    int exp;
    if (1<=intervalo && intervalo<=5){
        exp = 5- intervalo;
        return pow(2,exp);
    }else return 0;
}

//Función de evaluación, que mide la penalización total en una solución x.
float calidad(std::vector<int> x, int** C, int E, int S){
    int suma = 0;
    for(int i=0;i<(int)x.size()-1;i++){
        for(int j=i+1;j<(int)x.size();j++){
            suma += C[i][j]*penalizacion(x[i],x[j]);
        }
    }
    return suma;
}

//Función para determinar si una solución sn reemplazará a la solución sc.
//Si calidad(sn)<calidad(sc) entonces la reemplaza,
//sino entonces verifica que exp(-delta/Temp)>aleatorio, si lo logra entonces reemplaza.
bool evaluacion(std::vector<int> sn, std::vector<int> sc, int Temp, int** C, int E, int S){
    float c_sn = calidad(sn,C,E,S);
    float c_sc = calidad(sc,C,E,S);
    float delta = c_sn-c_sc;
    float aleatorio = drand48();
    if(delta<0) return true;
    else if (exp(-delta/Temp)>aleatorio) return true;
    else return false;
}

//Función del simulated annealing
//crea una solución inicial y luego comienza a iterar para repararla de acuerdo
//al algoritmo visto en clase.
//Además le agregué otra situación de detención relacionada a que si
//la solución no mejora luego de 5 descensos de temperatura entonces se detenga.
auto simulatedAnnealing(int **C, int E, int S){
    int Temp=100;
    int contador1, contador2=0;
    bool cond1=true,cond2=true, fallo=false;
    std::vector<int> sc = solucionInicial(E, C);
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
            //auto sn = movimiento1(sc,C,E);
            //auto sn = movimiento2(sc,C,E);
            //auto sn = movimiento3(sc,C,E);
            auto sn = movimiento4(sc,C,E);
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

//Función para crear el .sol
void escribirSol(std::vector<int> v){
    ofstream txtOut;
    txtOut.open("./soluciones/"+instancia+".sol");//,ios::app);
    std::vector<std::stringstream> examenes (v.size());
    std::stringstream id;
    int aux;
    for (int i=0;i<(int)v.size();i++) {
        aux = i;
        examenes[i] << std::setw(4) << std::setfill('0') << aux+1;
        txtOut << examenes[i].str()<<"\t"<<v[i]<<"\n";
    }
    txtOut.close();
}

//Función para crear el .res
void escribirRes(std::vector<int> v){
    ofstream txtOut;
    txtOut.open("./soluciones/"+instancia+".res");
    int max=-1;
    for (int i=0;i<(int)v.size();i++) {
        if(v[i]>max) max=v[i];
    }
    txtOut << max+1<<"\n";
    txtOut.close();
}

//Función para crear el .pen
void escribirPen(std::vector<int> v, int **C, int E, int S){
    ofstream txtOut;
    txtOut.open("./soluciones/"+instancia+".pen");
    txtOut <<calidad(v,C,E,S)/S<<"\n";
    txtOut.close();
}

//Función para crear el .seg con el tiempo en segundos que demoró en procesarse el simulatedAnnealing.
//Este archivo no lo pidieron, pero lo quise agregar igual.
void escribirSeg(double sec){
    ofstream txtOut;
    txtOut.open("./soluciones/"+instancia+".seg");
    txtOut <<sec<<"\n";
    txtOut.close();
}

int main(int argc, char const *argv[])
{
    clock_t inicio = clock();
    std::srand(unsigned(std::time(nullptr)));
    int S,E;
    getSE(&S,&E);

    //Se crea la matriz SE
    int ** matrizSE = new int*[S];
    for (int i = 0; i<S;i++)
        matrizSE[i]= new int[E];
    matrixSE(matrizSE,S,E);

    //Se crea la amtriz de conflictos
    int **conflictMatrix = new int*[E];
    for(int i=0; i<E; i++)
        conflictMatrix[i] = new int[E];
    matrixC(matrizSE, conflictMatrix, S, E);

    //Se obtiene la solución final
    auto sol_final = simulatedAnnealing(conflictMatrix,E,S);

    double segundos = (double)(clock() - inicio)/CLOCKS_PER_SEC;
    
    cout<<"Tiempo Ejecución SA: "<<segundos<<" [s]"<<endl;
    
    std::vector<int> y = sol_final.first;

    for(int i=0;i<(int)y.size();i++) y.at(i)-=1;
    cout<<"Solución Final: "<<endl;
    int bloqueMax=-1;
    for (auto i : y) {
        cout << i <<" ";
        if(i>bloqueMax) bloqueMax=i;
    }
    cout<<endl;
    cout<<"N° de Timeslots Solución Final: "<<bloqueMax+1<<endl;
    cout<<"Penalización Promedio Solución Final: "<<calidad(y,conflictMatrix,E,S)/S<<endl;
    
    //se crean los archivos pedidos.
    escribirSol(y);
    escribirRes(y);
    escribirPen(y,conflictMatrix,E,S);
    escribirSeg(segundos);
    cout<<"Archivos escritos con éxito."<<endl;
    return 0;
}
