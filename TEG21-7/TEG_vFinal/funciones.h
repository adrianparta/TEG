#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED
#include "rlutil.h"
#include <ctime>
#include "TileMap"
#include <cstdlib>

using namespace rlutil;

// funciones
int menuEjercitos( const char texto[],int maximo,int posX,int posY);
void IncorporarEjercitos();
void DeclararAtaque(int CantFichas[],int PaisAtq,int PaisDfs, bool Pertenencia[]);
void crearListaPaises(int listaARellenar[],bool jugadorSeleccionado,bool limitrofe,int paisLimitrofes,bool atacando,bool reagrupar);
int DibujarPaisSeleccionable(bool jugador,bool limitrofe,int paisLimitrofe,bool atacando,bool reagrupar,int continente);
void DibujarPantalla();
bool checkrep(int n, int num[]);
void PintarPais(int Indice, bool jugador, bool seleccionado);
void InicializarPartida();
bool Ganar();
void Atacar();
void Reagrupar();
void RecompensaPorConquista();
void DibujarCantCartas();
void PantallaDeInicio();

//constantes
int CantidadFichas[50];
bool Pertenencia[50];
bool turnoDe=1;
bool seguir=1;
bool conquistoPais=0;
int pendiente[50];
int pertenenciaTarjetas[50];
int sumaDibujos[2][3] = {{0,0,0},{0,0,0}};
int conteoDeCanjes[2] = {0,0};
int objetivos[2] = {0,0};

void DibujarPantalla(){

    //Dibuja el mapa segun el array guardado en "TileMap"
    hidecursor();
    setColor(WHITE);
    //dibuja los bordes de los paises
    for(int i=0; i<50;i++){
        for(int j =0;j<200;j++){
            if(mapa[i][j] == 1){
            locate(j+1,i+1);
            cout<<(char)219;
            }
            //dibuja los puentes
            if(mapaPuentes[i][j] == 1){
            locate(j+1,i+1);
            cout<<(char)176;
            }
        }
    }

    setColor(WHITE);
}
//toda la logica para la fase de ataque de un turno
void Atacar(){
    //ataque:
    locate(22,54);
    cout<<"                                                      ";
    locate(22,54);
    cout<<"<Fase De Ataque>";


    conquistoPais=0;

    locate(1,55);
    printf("\33[2K\r");
    locate(1,55);
    cout<<"Queres atacar? 1 para si, 0 para no: ";
    cin>>seguir;
    locate(1,55);
    printf("\33[2K\r");

    while(seguir){
        int t=0;
        locate(1,55);
        cout<<"Con que pais queres atacar?";
        t=DibujarPaisSeleccionable(turnoDe,0,0,1,0,-1);
        locate(1,55);
        printf("\33[2K\r");

        if(t!=-1){
            locate(1,55);
            printf("\33[2K\r");
            cout<<"A que pais queres atacar?";
            DeclararAtaque(CantidadFichas,t,DibujarPaisSeleccionable(turnoDe,1,t,1,0,-1),Pertenencia);

            bool ganador = Ganar();
            if(ganador == true){
                locate(100,25);
                system("pause");
                rlutil::cls();
                locate(100,25);
                cout<<"GANO EL JUGADOR "<<turnoDe<<"!!";
                exit(0);
            }

            locate(1,55);
            cout<<"                                                              ";

            locate(1,55);
            cout<<"Queres seguir atacando? 1 para si, 0 para no: ";
            cin>>seguir;
            locate(1,55);
            printf("\33[2K\r");
            locate(1,56);
            printf("\33[2K\r");
            locate(1,57);
            printf("\33[2K\r");
            locate(1,58);
            printf("\33[2K\r");
        }
        else{
            locate(1,56);
            printf("\33[2K\r");
            locate(1,56);
            cout<<"No tenes paises que puedan atacar";
            seguir = 0;
        }
    }
}

void Reagrupar(){
    //reagrupar
    locate(22,54);
    cout<<"                                                      ";
    locate(22,54);
    cout<<"<Fase De Reagrupacion>";

    locate(1,55);
    printf("\33[2K\r");
    locate(1,55);
    cout<<"Queres reagrupar? 1 para si, 0 para no: ";
    cin>>seguir;

    for(int j=0;j<50;j++){
        pendiente[j]=0;
    }

    while(seguir){


    int paisDa=DibujarPaisSeleccionable(turnoDe,0,0,0,1,-1);
    if(paisDa!=-1){
        locate(1,55);
        printf("\33[2K\r");
        locate(1,55);
        cout<<"Selecciona el pais que entrega ejercitos y luego el que los recibe (quedara pendeinte la transferencia y se sumara cuando la fase de reagrupar finalice)";

        int paisRecibe=DibujarPaisSeleccionable(turnoDe,1,paisDa,0,0,-1);
        locate(1,55);
        printf("\33[2K\r");
        int cantTransferencia=menuEjercitos("Cuantos ejercitos queres pasar al pais seleccionado? ",CantidadFichas[paisDa]-1,1,55);
        locate(1,55);
        printf("\33[2K\r");

        CantidadFichas[paisDa]-=cantTransferencia;
        PintarPais(paisDa,turnoDe,0);
        pendiente[paisRecibe]+=cantTransferencia;
        PintarPais(paisRecibe,turnoDe,0);

        rlutil::locate(1,55);
        printf("\33[2K\r");
        rlutil::locate(1,55);
        cout<<"Queres seguir reagrupando? 1 para si, 0 para no: ";
        cin>>seguir;
        rlutil::locate(1,55);
        printf("\33[2K\r");
    }
    else{
        locate(1,56);
        printf("\33[2K\r");
        locate(1,56);
        cout<<"No hay posibilidades de reagrupar, ";
        system("pause");
        locate(1,56);
        printf("\33[2K\r");
        seguir=0;
    }
    }
    for(int j=0;j<50;j++){
        if(pendiente[j]!=0){
            CantidadFichas[j]+=pendiente[j];
            pendiente[j]=0;
            PintarPais(j,Pertenencia[j],false);
        }
    }

}

void DibujarCantCartas(){
    locate(2,40);
    cout<<"Tus cartas: ";

    for(int h=0;h<3;h++){
        locate(2,41+h);
        cout<<dibujo[h]<<": "<<sumaDibujos[(int)turnoDe][h];
    }
    locate(2,45);
    cout<<"Tus canjes totales: "<<conteoDeCanjes[(int)turnoDe];
}

void RecompensaPorConquista(){
    //RECOMPENSA POR CONQUISTA:
    locate(22,54);
    cout<<"                                                      ";
    locate(22,54);
    cout<<"<Recompensa Por Conquista>";

    if(conquistoPais==1){
        int z=0;
            z = (1 + rand() % 50)-1;
            while(pertenenciaTarjetas[z]!=2){
                z = (1 + rand() % 50)-1;
            }

            pertenenciaTarjetas[z]=(int)turnoDe;
            //sumar tarjeta al acumulador de dibujos, para luego poder hacer el canje de tener 3 iguales o 3 distintas
            sumaDibujos[(int)turnoDe][dibujoTarjetas[z]]++;

            if(Pertenencia[z]==turnoDe){
                locate(1,56);
                printf("\33[2K\r");
                locate(1,56);
                cout<<"Como "<<paises[z]<<" te pertenece, se te sumo un ejercito adicional!";
                CantidadFichas[z]++;
                PintarPais(z,turnoDe,0);
            }

            locate(1,55);
            printf("\33[2K\r");
            locate(1,55);
            cout<<"Por conquistar un pais se te recompenzo con la tarjeta de pais de: "<<paises[z]<<" ("<<dibujo[dibujoTarjetas[z]]<<"). ";
            system("pause");
            locate(1,56);
            printf("\33[2K\r");
    }

    DibujarCantCartas();
}

// crea un menu para retornar un numero seleccionado, entre 0 y el maximo que se le indique(enteros), en una posicion determinada de la pantalla.
int menuEjercitos(const char texto[],int maximo,int posX,int posY){

    //controla cuando se sale del while
    bool seleccionFinalizada = false;
    int n=1;

    do{
        showcursor();
        rlutil::locate(posX,posY);
        locate(posX,posY);
        cout<<"                                                                                   ";
        locate(posX,posY);
        cout<<texto<<n;
        int key = rlutil::getkey();
        switch(key)
        {
        case 14: //flecha arriba
            if(n < maximo){n++;}
            break;
        case 15: //flecha abajo
            if(n > 1){n--;}
            break;
        case 1: //enter
            //cout<<indiceSeleccion;
            seleccionFinalizada = true;
            break;
        default:
            break;
        }
    }while(seleccionFinalizada == false);

    return n;
}

//maneja la seccion de incorporacion de ejercitos del turno de un jugador
void IncorporarEjercitos(){

    locate(22,54);
    cout<<"                                                      ";
    locate(22,54);
    cout<<"<Fase De Incorporacion De Ejercitos>";

    int Incorporaciones, Incorporacionesrestantes=0;
    int incorporacionContinente[6]={5,3,5,7,3,2};
    for(int i=0;i<6;i++){
    int contador=0;
        for(int j=0;j<15;j++){
            if(Pertenencia[continentes[i][j]]==turnoDe || continentes[i][j]==-1){
            contador++;
            }
        }
        if(contador>=15){

            locate(1,55);
            printf("\33[2K\r");
            locate(1,55);
            cout<<"Por tener completo el continente "<<nombreContinentes[i]<<" se te otorgan "<< incorporacionContinente[i]<<" ejercitos para sumarle";
            //Incorporacionesrestantes+=incorporacionContinente[i];
            Incorporacionesrestantes=incorporacionContinente[i];

            while(Incorporacionesrestantes>0){
                int p=DibujarPaisSeleccionable(turnoDe,0,0,0,0,i);
                Incorporaciones=menuEjercitos("cuantos queres poner en este pais?",Incorporacionesrestantes,1,56);
                locate(1,56);
                printf("\33[2K\r");
                CantidadFichas[p]+=Incorporaciones;
                Incorporacionesrestantes-=Incorporaciones;
                PintarPais(p,turnoDe,0);
            }

            locate(1,55);
            printf("\33[2K\r");
        }
    }

    //calcula cuantos ejercitos tiene disponibles el jugador para añadir a sus paises
    for(int i=0;i<50;i++){
            if(Pertenencia[i]==turnoDe){
                Incorporacionesrestantes++;
            }
    }

    Incorporacionesrestantes=Incorporacionesrestantes/2;

    //incorporaciones por canje de tarjetas

    int c = 0;
    for (int i = 0;i<3;i++){
        if(sumaDibujos[(int)turnoDe][i] >=1){
            c++;
        }
        //si tiene 1 de cada dibujo
        if(c == 3){
            sumaDibujos[(int)turnoDe][0]-=1;
            sumaDibujos[(int)turnoDe][1]-=1;
            sumaDibujos[(int)turnoDe][2]-=1;

            for(int e = 0;e<3;e++){
                bool encontrado = 0;
                while(encontrado == 0){
                    for(int r = 0;r<50;r++){
                        if(pertenenciaTarjetas[r] == turnoDe && dibujoTarjetas[r] == e && encontrado == 0){
                            pertenenciaTarjetas[r] = 2;
                            encontrado = 1;
                        }
                    }
                }
            }

            conteoDeCanjes[(int)turnoDe]++;


            if(conteoDeCanjes[(int)turnoDe] <=3 && conteoDeCanjes[(int)turnoDe] >=1){
                Incorporacionesrestantes += 4 + (conteoDeCanjes[(int)turnoDe]-1) * 3;

                locate(1,57);
                printf("\33[2K\r");
                locate(1,57);
                cout<<"Por tener 1 de cada dibujo se te recompenza con "<< 4 + (conteoDeCanjes[(int)turnoDe]-1) * 3 <<" ejercitos";
                DibujarCantCartas();
            }

            if(conteoDeCanjes[(int)turnoDe] > 3){
                Incorporacionesrestantes += 10 + (conteoDeCanjes[(int)turnoDe]-3) * 5;

                locate(1,57);
                printf("\33[2K\r");
                locate(1,57);
                cout<<"Por tener 1 de cada dibujo se te recompenza con "<< 10 + (conteoDeCanjes[(int)turnoDe]-3) * 5 <<" ejercitos";
                DibujarCantCartas();
            }


        }

        //si tiene 3 iguales
        if(sumaDibujos[(int)turnoDe][i] == 3){
            sumaDibujos[(int)turnoDe][i]-= 3;

            for(int e=0;e<50;e++){
                if(pertenenciaTarjetas[e] == turnoDe && dibujoTarjetas[e] == i){
                    pertenenciaTarjetas[e] = 2;
                }
            }

            conteoDeCanjes[(int)turnoDe]++;

            DibujarCantCartas();

            if(conteoDeCanjes[(int)turnoDe] <=3 && conteoDeCanjes[(int)turnoDe] >=1){
                Incorporacionesrestantes += 4 + (conteoDeCanjes[(int)turnoDe]-1) * 3;

                locate(1,57);
                printf("\33[2K\r");
                locate(1,57);
                cout<<"Por tener 3 de un mismo dibujo se te recompenza con "<< 4 + (conteoDeCanjes[(int)turnoDe]-1) * 3 <<" ejercitos";
                DibujarCantCartas();
            }

            if(conteoDeCanjes[(int)turnoDe] > 3){
                Incorporacionesrestantes += 10 + (conteoDeCanjes[(int)turnoDe]-3) * 5;

                locate(1,57);
                printf("\33[2K\r");
                locate(1,57);
                cout<<"Por tener 3 de un mismo dibujo se te recompenza con "<< 10 + (conteoDeCanjes[(int)turnoDe]-3) * 5 <<" ejercitos";
                DibujarCantCartas();
            }
        }
    }


    while(Incorporacionesrestantes > 0){
        locate(1,55);
        printf("\33[2K\r");
        locate(1,55);
        cout<<"seleccione el pais al que quiere incorporar ejercitos "<<"(incorporaciones restantes): "<<Incorporacionesrestantes;
        int j=DibujarPaisSeleccionable(turnoDe,0,0,0,0,-1);
        //crea el menu para seleccionar la cantidad de ejercitos que se desea asignar al pais indice j, se lo asigna a las incorporaciones
        Incorporaciones = menuEjercitos("cuantos ejercitos queres poner en el pais seleccionado? ", Incorporacionesrestantes,1,56);
        locate(1,56);
        printf("\33[2K\r");
        //suma las fichas al pais correspondiente
        CantidadFichas[j]+=Incorporaciones;
        //actualiza las fichas restantes para sumar
        Incorporacionesrestantes -= Incorporaciones;
        PintarPais(j,turnoDe,0);
    }

}

//maneja la logica para atacar un pais con otro, y que ocurre si se conquista dicho pais
void DeclararAtaque(int CantFichas[],int PaisAtq,int PaisDfs, bool Pertenencia[]){
    locate(100,55);
    //inicializa una seed para generacion de un numero aleatorio
    srand(time(NULL));
    int PaisesIntegrar;

    //se crean las variables a rellenar con los nuemeros aleatorios, 0 significa que ese dado no se tiró
    int DadosAtq[3]={0,0,0},DadosDfs[3]={0,0,0};


    //se generan los dados
    //dados j1
    cout<<"dados atacante: ";
    //se tira un numero de dados segun la cantidad de fichas del atacante -1
    //(ya que el atacante debe tener al menos 1 ficha para ocupar en caso de conquista)
    //pero no mayor a 3, la cant maxima de dados en juego(por jugador)
    for(int i=0;i<CantFichas[PaisAtq]-1 && i<3;i++){
        DadosAtq[i]=1+rand()%6;
        locate(100,56+i);
        cout<<DadosAtq[i];
    }

    //dados j2
    locate(120,55);
    cout<<"dados defensor: ";
    //en el caso del defensos se utilizan todas las fichas pero no mas de 3 dados
    //de quedar con 0 fichas el pais es conquistado por el atacante
    for(int j=0;j<CantFichas[PaisDfs]&&j<3;j++){
        DadosDfs[j]=1+rand()%6;
        locate(120,56+j);
        cout<<DadosDfs[j];
    }

    //se ordenan los dados de cada jugador de mayor a menor
    for(int j=0;j<2;j++){
        for(int i=0;i<2;i++){
            if(DadosAtq[i]<DadosAtq[i+1]){
                int aux= DadosAtq[i];
                DadosAtq[i]=DadosAtq[i+1];
                DadosAtq[i+1]=aux;
            }
            if(DadosDfs[i]<DadosDfs[i+1]){
                int aux= DadosDfs[i];
                DadosDfs[i]=DadosDfs[i+1];
                DadosDfs[i+1]=aux;
            }
        }
    }


    locate(140,55);
    cout<<"dados ordenados:";
    //se comparan los dados en orden, se define quien gana esa "ronda"(el numero mas grande) y se resta una ficha por ronda al perdedor
    //en el caso de ser numeros iguales gana el defensor
    for (int a=0;a<3;a++){
        //se ignoran los dados no tirados
        if(DadosAtq[a]>0 && DadosDfs[a]>0){
            locate(140,56+a);
            cout<<DadosAtq[a]<<" vs "<<DadosDfs[a];
            if(DadosAtq[a]>DadosDfs[a]){
                locate(148,56+a);
                cout<<"gana atacante";
                CantFichas[PaisDfs]--;
            }
            else{
            locate(148,56+a);
            cout<<"gana defensa";
            CantFichas[PaisAtq]--;
            }
        }
    }

    //en caso de que el pais defensor quede sin fichas, se le asigna la pertenencia al jugador atacante
    //se crea un menu para resolver la transferencia de fichas del pais ganador al perdedor
    if(CantFichas[PaisDfs]<=0){
        //ejercitosTransferibles debe ser un numero entre 1 y 3, y no mayor a CantFichas[PaisAtq]-1
        int ejercitosTransferibles = 0;
        if(CantFichas[PaisAtq]-1 > 3){
            ejercitosTransferibles = 3;
        }
        else{
            ejercitosTransferibles = CantFichas[PaisAtq]-1;
        }

        //(se le asigna el valor opuesto previo a la conquista)
        Pertenencia[PaisDfs]=!Pertenencia[PaisDfs];

        //paisesintegrar debe ser un numero entre 1 y 3, y no mayor a CantFichas[PaisAtq]-1
        PaisesIntegrar = menuEjercitos("El pais fue conquistado: cuantos ejercitos queres transferirle? ",ejercitosTransferibles,1,55);
        locate(1,55);
        cout<<"                                                                                         ";
        //se transfieren los ejercitos del pais atacante al conquistado
        CantFichas[PaisDfs]+=PaisesIntegrar;
        CantFichas[PaisAtq]-=PaisesIntegrar;
        conquistoPais=1;
    }
    //se actualizan los valores de fichas de ambos paises
    PintarPais(PaisAtq,Pertenencia[PaisAtq],0);
    PintarPais(PaisDfs,Pertenencia[PaisDfs],0);
}

void crearListaPaises(int listaARellenar[],bool jugadorSeleccionado,bool limitrofe,int paisLimitrofes,bool atacando,bool reagrupar){
    int c = 0;
    if(atacando && !limitrofe){
        for(int p=0;p<50;p++){
            int comprobador=0;
            for(int l=0;l<8;l++){
                if(Pertenencia[limitrofes[p][l]]!=turnoDe && limitrofes[p][l]!=-1){
                comprobador++;
                }
            }
            if(Pertenencia[p] == jugadorSeleccionado && CantidadFichas[p]>1 && comprobador != 0){
                listaARellenar[c] = p;
                c++;
            }
        }

    }

    if(!limitrofe && !atacando && !reagrupar){
        for(int p=0;p<50;p++){
            if(Pertenencia[p] == jugadorSeleccionado){
                listaARellenar[c] = p;
                c++;
            }
        }
    }

    if(limitrofe && atacando){
        for(int p=0;p<8;p++){
            if(limitrofes[paisLimitrofes][p]!=-1 && Pertenencia[limitrofes[paisLimitrofes][p]]!=jugadorSeleccionado){
                listaARellenar[c] = limitrofes[paisLimitrofes][p];
                c++;
            }
        }
    }

    if(limitrofe && !atacando){
        for(int p=0;p<8;p++){
            if(limitrofes[paisLimitrofes][p]!=-1 && Pertenencia[limitrofes[paisLimitrofes][p]]==jugadorSeleccionado){
                listaARellenar[c] = limitrofes[paisLimitrofes][p];
                c++;
            }
        }
    }

    if(reagrupar){
        for(int p=0;p<50;p++){
            int comprobador=0;
            for(int l=0;l<8;l++){
                if(Pertenencia[limitrofes[p][l]]==turnoDe && limitrofes[p][l]!=-1 && CantidadFichas[p]>1){
                comprobador++;
                }
            }
            if(Pertenencia[p] == jugadorSeleccionado && CantidadFichas[p]>1 && comprobador != 0){
                listaARellenar[c] = p;
                c++;
            }
        }

    }

    listaARellenar[50] = c-1;

}

void PintarPais(int Indice, bool jugador, bool seleccionado){

    int c;

    if(jugador==1){
        //rlutil::setColor(BLUE);
        c=BLUE;

    }
    else{

        c=RED;
        //rlutil::setColor(RED);
    }

    if(seleccionado){
        c=GREEN;
        //rlutil::setColor(GREEN);
    }
    rlutil::setColor(c);
    for(int i=0; i<15;i++){
        for(int j =0;j<25;j++){
            if(rellenoPaises[Indice][i][j] ==219 || rellenoPaises[Indice][i][j] ==2){
                locate(j+startCoord[Indice][0],i+startCoord[Indice][1]);
                cout<<(char)219;
            }
            if(rellenoPaises[Indice][i][j] != 0 && rellenoPaises[Indice][i][j] != 219 && rellenoPaises[Indice][i][j] != 1 && rellenoPaises[Indice][i][j] !=2){
                rlutil::setColor(WHITE);
                locate(j+startCoord[Indice][0],i+startCoord[Indice][1]);
                cout<<(char)rellenoPaises[Indice][i][j];
            }
            if(rellenoPaises[Indice][i][j] == 1){
                rlutil::setColor(WHITE);
                locate(j+startCoord[Indice][0]-1,i+startCoord[Indice][1]);
                cout<<CantidadFichas[Indice];
                if(CantidadFichas[Indice] < 10){
                    locate(j+startCoord[Indice][0],i+startCoord[Indice][1]);
                    rlutil::setColor(c);
                    cout<<(char)219;
                }
            }

            if(rellenoPaises[Indice][i][j] == 2 && pendiente[Indice]!=0){
                rlutil::setColor(LIGHTRED);
                locate(j+startCoord[Indice][0]-2,i+startCoord[Indice][1]);
                cout<<"+";
                locate(j+startCoord[Indice][0]-1,i+startCoord[Indice][1]);
                cout<<pendiente[Indice];

                if(pendiente[Indice] < 10){
                    locate(j+startCoord[Indice][0],i+startCoord[Indice][1]);
                    rlutil::setColor(c);
                    cout<<(char)219;
                }
            }

            rlutil::setColor(c);
        }
    }

    setColor(WHITE);
}

int DibujarPaisSeleccionable(bool jugador,bool limitrofe,int paisLimitrofe,bool atacando,bool reagrupar,int continente){
    int listaPaisesSeleccionables[51];
    bool seleccionFinalizada = false;

    if(continente!=-1){
        listaPaisesSeleccionables[50]=-1;
        for(int i=0;i<15;i++){
            if(continentes[continente][i]!=-1){

                listaPaisesSeleccionables[50]++;
                listaPaisesSeleccionables[i]=continentes[continente][i];
            }
        }

    }
    else{
        crearListaPaises(listaPaisesSeleccionables,jugador,limitrofe,paisLimitrofe,atacando,reagrupar);

        if(listaPaisesSeleccionables[50] == -1){
            return -1;
        }
    }

    int n = 0;
    int nAnterior = -1;

    do{
        if(listaPaisesSeleccionables[50] != -1){
            if(n > listaPaisesSeleccionables[50])n = 0;
            if(n < 0)n = listaPaisesSeleccionables[50];
            PintarPais(listaPaisesSeleccionables[n],turnoDe,true);
        }

        int key = rlutil::getkey();
        switch(key)
        {
        case 14: //flecha arriba
            nAnterior = n;
            n++;
            break;
        case 15: //flecha abajo
            nAnterior = n;
            n--;
            break;
        case 1: //enter
            //cout<<indiceSeleccion;
            seleccionFinalizada = true;
            break;
        default:
            break;
        }



        if(nAnterior != -1 && listaPaisesSeleccionables[50] != -1){
            PintarPais(listaPaisesSeleccionables[nAnterior],Pertenencia[listaPaisesSeleccionables[nAnterior]],false);
        }

    }while(seleccionFinalizada == false);

    /*int comprobador=0;
    for(int l=0;l<8;l++){
        if(Pertenencia[limitrofes[listaPaisesSeleccionables[n]][l]]!=turnoDe && limitrofes[listaPaisesSeleccionables[n]][l]!=-1){
            comprobador++;
        }
    }
    if(comprobador==0 && atacando && limitrofe){
        locate(1,55);
        cout<<"comprobador = 0";
        return -1;
    }*/
    //if(listaPaisesSeleccionables[50] != -1){
    return listaPaisesSeleccionables[n];
    //}else{return -1;}
}

bool checkrep(int n, int num[]){
    for(int i=0; i<50; i++)
        if(n == num[i])
            return true;
    return false;
}

void InicializarPartida(){


    for(int j=0;j<50;j++){
        pendiente[j]=0;//inicializa la cantidad de pendientes
        pertenenciaTarjetas[j]=2;//inicializa la pertenencia de todas las tarjetas de pais en 2, para que no pertenezcan a ningun jugador
        CantidadFichas[j]=1;//inicializa la cantidad de fichas en 1 en cada pais
    }

    //asigna 25 paises aleatoriamente para cada jugador
    srand(time(NULL));
    int n, num[50];
    for(int i=0; i<50; i++){
        do
            n = 1 + rand() % 50;
        while(checkrep(n, num));
        num[i] = n;
    }

    for(int j=0;j<25;j++){
        Pertenencia[num[j]-1]=1;
        Pertenencia[num[j+25]-1]=0;

    }

    //asignar un objetivo secreto distinto para cada jugador
    objetivos[0] = (1 + rand() % 9)-1;
    objetivos[1] = (1 + rand() % 9)-1;

    while(objetivos[0] == objetivos[1]){
        objetivos[1] = (1 + rand() % 9)-1;
    }


    rlutil::cls();
    DibujarPantalla();

    DibujarCantCartas();

    for(int j=0;j<50;j++){
        PintarPais(j,Pertenencia[j],false);

    }

    //los jugadores agregan ejercitos en cualquiera de los paises que les pertenezcan, por turnos y en dos tandas, primero 5 y luego 3
    int w;

    for(int e=0;e<2;e++){
        locate(1,60);
        cout<<"Para ver su objetivo secreto, rueda hacia abajo";
        locate(1,100);
        printf("\33[2K\r");
        locate(1,100);
        cout<<"su objetivo secreto es: "<<textoSecreto[objetivos[(int)turnoDe]];
        locate(1,1);

        int fichasRestantes = 5;

        while(fichasRestantes > 0){

        locate(1,54);
        printf("\33[2K\r");
        locate(1,54);
        if(turnoDe == 0){
        cout<<"Turno del jugador 2";
        }else{
            cout<<"Turno del jugador 1";
        }

        locate(1,55);
        printf("\33[2K\r");
        locate(1,55);
        cout<<"Debe repartir "<<fichasRestantes<<" entre sus paises";


        w=DibujarPaisSeleccionable(turnoDe,0,0,0,0,-1);
        int f = menuEjercitos("Elija el numero de ejercitos para incroporar al pais: ",fichasRestantes,1,56);
        printf("\33[2K\r");
        CantidadFichas[w]+=f;
        fichasRestantes-=f;
        PintarPais(w,turnoDe,false);
        }

        turnoDe=!turnoDe;
    }

    for(int e=0;e<2;e++){
        locate(1,60);
        cout<<"Para ver su objetivo secreto, rueda hacia abajo";
        locate(1,100);
        printf("\33[2K\r");
        locate(1,100);
        cout<<"su objetivo secreto es: "<<textoSecreto[objetivos[(int)turnoDe]];
        locate(1,1);

        int fichasRestantes = 3;

        while(fichasRestantes > 0){

        locate(1,54);
        printf("\33[2K\r");
        locate(1,54);
        if(turnoDe == 0){
        cout<<"Turno del jugador 2";
        }else{
            cout<<"Turno del jugador 1";
        }

        locate(1,55);
        printf("\33[2K\r");
        locate(1,55);
        cout<<"Debe repartir "<<fichasRestantes<<" entre sus paises";

        w=DibujarPaisSeleccionable(turnoDe,0,0,0,0,-1);
        int f = menuEjercitos("Elija el numero de ejercitos para incroporar al pais: ",fichasRestantes,1,56);
        printf("\33[2K\r");
        CantidadFichas[w]+=f;
        fichasRestantes-=f;
        PintarPais(w,turnoDe,false);
        }



        turnoDe=!turnoDe;
    }

    for(int u = 0; u<2;u++){

        DibujarCantCartas();

        locate(1,54);
        printf("\33[2K\r");
        locate(1,54);
        if(turnoDe == 0){
        cout<<"Turno del jugador 2";
        }else{
            cout<<"Turno del jugador 1";
        }

        Atacar();

        Reagrupar();

        RecompensaPorConquista();

        turnoDe = !turnoDe;
    }

}

bool Ganar(){
    int puntos = 0;

    for(int i =0;i<50;i++){
        if(Pertenencia[i] == turnoDe){
            puntos++;
        }
    }

    if(puntos >= 40){
        return true;
    }

    puntos = 0;

    int cantidadesXContinente[6] = {0,0,0,0,0,0};

    for(int j = 0;j<6;j++){
        for(int i =0;i<15;i++){
            if(Pertenencia[continentes[j][i]] == turnoDe && continentes[j][i] != -1){
                cantidadesXContinente[j]++;
            }
        }
    }

    for(int i = 0;i<6;i++){
        if(cantidadesXContinente[i] >= objetivoSecreto[objetivos[turnoDe]][i]){
            puntos++;
        }
    }

    if(puntos >= 6){
        return true;
    }

    return false;
}

void PantallaDeInicio(){

    bool tegMap[10][50] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
        {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
        {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1},
        {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1},
        {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };


    setColor(WHITE);

    for(int y = 0;y<10;y++){
        for(int x =0;x<49;x++){
            if(tegMap[y][x] == 1){
                locate(2+x,1+y);
                cout<<(char) 219;
            }

        }
    }
    locate(1,15);
    cout<<"Realizado por Adrian Partarrieu y Santiago Campanella";
    locate(1,16);
    cout<<"presione alt+enter para poner el juego en pantalla completa";
    locate(1,17);
    system("pause");


}
#endif
