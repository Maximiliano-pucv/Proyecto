#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <windows.h>
//librerias Para los Tdas
#include "list.h"
#include "stack.h"
#include "hashMap.h"
#include "heap.h"
#define TEST printf("this is a test be alert\n")
#define FILAS 41
#define COLUMNAS 100
//strucs
/* MORADO printf("\033[0;35m")
 AMARILLO printf("\033[0;33m")
 ROJO printf("\033[0;31m")
  VERDE printf("\033[0;32m")
  CYAN printf("\033[0;36m")
AZUL printf("\033[0;34m")*/
typedef struct{
    int x;
    int y;
}coordenadas;

typedef struct{
    
    char nombre[100];
    int HPMAX; //vida maxima
    int HP; //vida actual
    int ATK;
    int DEF;
    int PH;
    char *descripcion;
}Info;//Hace referencia a las estadisiticas

typedef struct{

    char *tipo; //si es arma, armadura, consumible
    char *tipoArmadura; //si es casco, pechera o pantalones (SOLO USAR PARA LAS ARMADURAS)
    Info *stats;
    bool equipado;

}TipoEquipamiento;//estadisticas del objeto en cuestion

typedef struct{
    char nombre[15];
    char descripicion[100];

}Opcion;

typedef struct{
    
    coordenadas pos;
    Info *datos;
    char *clase;
    int nivel;
    int PH;
    List *inventario;
    Opcion ataques[4];
    HashMap *equipamiento;
}Jugador;

typedef struct{
    char tamano[FILAS][COLUMNAS];
}sala;
//prototipos de ubicaciones
void gotoxy(int x, int y);
void pantallainesesariadecarga();
void mainmenu();

//prototipo de funciones
void CrearPerfil(List *lista);
void selccionarclase(Jugador *usuario);
void estadisticasDeclase(Jugador *usuario);
void inventarionuevo(Jugador *usuario);
void OpcionesBatalla(Jugador *usuario);
void limpiarpantalla();
void generarmapa();
void rellenarmapa(sala * sandbox, int posfila, int poscolum, int largo, char caracter);

void aplicarItem(Jugador *usuario,TipoEquipamiento *equipo);

HashMap* almacenarmounstruos();
HashMap* generaritems();
/*const*/ char *get_csv_field (char * tmp, int k);

int validarmov(sala * sandbox, int x, int y, Jugador *player, int *cont);
TipoEquipamiento* seleccionaritem(HashMap *Mapaitems,int numero);
/*equipamiento por clase*/
void equipamientoBase(Jugador *usuario);
TipoEquipamiento *createEquipoBase();
void equipobaseE(Jugador *usuario);
void equipobaseM(Jugador *usuario);
void equipobaseL(Jugador *usuario);
void equipobaseC(Jugador *usuario);

void faseDElanzamiento(List *listaJugadores,sala *sandbox,HashMap *mapamonster, char *);
//funciones solo developers (fran)
void developerfunctions(List* listaJugadores, HashMap* Mapamonster);

void mostrar_perfiles (List *lista);
bool Submenu(List *lista);
void submenu_Inventario(List *lista);
void mostrarInventario(List *lista);
void submenu_opciones(List *lista);
void mostrarDescrip(List *lista);
void mostrar(List *lista, int);
bool usar_item(List *lista );
bool asignar_item(List *lista);
bool eliminar_item(List *lista);

//funciones para batallas
void pantalla_batalla();
void batalla_final_limpiar();
bool empezarbatalla(Jugador *jugador,Info *Enemigo);
Info *seleccionarenemigo(HashMap *Mapa,int numero);
int comandoBatalla(Opcion *comandos);
bool Huir();
int Atacar(Info * atacante, Info * atacado, bool defensa);
void TurnoEnemigo(Jugador *Jugador, Info *Enemigo, int *ptri, bool *defensaJ, bool*defensaE);
bool usarobjetoenbatalla(Jugador *jugador);

//main
int main(){
    time_t t;
    srand((unsigned) time(&t)); 
    char *estado = (char *)malloc(sizeof(char)* 6);
    strcpy(estado, "vivo");
    List *listajugadores = createList();
    HashMap *mapamonster = almacenarmounstruos();

    do{
        if((strcmp(estado, "dead") == 0)||(strcmp(estado,"vivo")==0)){
            pantallainesesariadecarga();
            strcpy(estado, "vivo");
            mainmenu();
        }
        
        CrearPerfil(listajugadores);
         //mostrar_perfiles(listajugadores);
        limpiarpantalla();
        pantallainesesariadecarga();
        limpiarpantalla();
        sala *sandbox = (sala*)malloc(sizeof(sala));
        generarmapa(sandbox);
        faseDElanzamiento(listajugadores,sandbox,mapamonster,estado);

    }while(strcmp(estado, "win") != 0);
    limpiarpantalla();
    gotoxy(50,50); printf("HAS GANADO");
    for(int j = 0; j<50; j++)
    {
        Sleep(100);
    }
    //mostrar_perfiles(listajugadores);
    return 0;
}

void pantalla_batalla(){
    gotoxy(104,0); printf("--------------------------------------------------------------------------------------------");
    for(int i =1; i<41; i++){
        if(i == 32){
            gotoxy(104,i); printf("|------------------------------------------------------------------------------------------|");
        }else{
            gotoxy(104,i); printf("|                                                                                          |");
        }
    }
    gotoxy(104,41); printf("--------------------------------------------------------------------------------------------");
}
void batalla_final_limpiar(){
    gotoxy(104,0); printf("                                                                                            ");
    for(int i =1; i<41; i++){
        gotoxy(104,i); printf("                                                                                            ");
        
    }
    gotoxy(104,41); printf("                                                                                            ");
}
bool Huir(){
    int numero = rand()%3;

    switch (numero)
    {
    case 0:
    case 1:
        return false;
        break;
    case 2:
        return true;
        break;

    default:
        return false;
        break;
    }
}

int comandoBatalla(Opcion *comandos){
    gotoxy(105,34); printf("# %s                     O %s",comandos[0].nombre,comandos[1].nombre);
    gotoxy(105,36); printf("O %s                      O %s",comandos[2].nombre,comandos[3].nombre);
    int seleccion =0;
    coordenadas cursor;
    cursor.x = 105;
    cursor.y =34;

    while (true)
    {
        Sleep(100);
        
        //abajo
        if((GetAsyncKeyState(0x28)) && (cursor.y<=34))
        {
            gotoxy(cursor.x,cursor.y);printf("O");
            cursor.y+=2;
            gotoxy(cursor.x,cursor.y); printf("#");
        
        }
        //arriba
        if((GetAsyncKeyState(0x26)) && (cursor.y>=36))
        {
            gotoxy(cursor.x,cursor.y);printf("O");
            cursor.y-=2;
            gotoxy(cursor.x,cursor.y); printf("#");
            
        }
        //derecha131
        if((GetAsyncKeyState(0x27)) && (cursor.x<=134))
        {
            gotoxy(cursor.x,cursor.y);printf("O");
            cursor.x=134;
            gotoxy(cursor.x,cursor.y); printf("#");
            
        }

        if((GetAsyncKeyState(0x25)) && (cursor.x>=105))
        {
            gotoxy(cursor.x,cursor.y);printf("O");
            cursor.x=105;
            gotoxy(cursor.x,cursor.y); printf("#");
            
        }
        if((cursor.x==105)&&(cursor.y==34)&&(seleccion!=1)){
           seleccion = 1; 
           gotoxy(105,38); printf("                                                                                          ");
           gotoxy(105,38); printf("* %s", comandos[0].descripicion);
        } 
        if((cursor.x==134)&&(cursor.y==34)&&(seleccion!=2)){
            seleccion = 2;
            gotoxy(105,38); printf("                                                                                          ");
            gotoxy(105,38); printf("* %s", comandos[1].descripicion);
        } 
        if((cursor.x==105)&&(cursor.y==36)&&(seleccion!=3)){
            seleccion = 3;
            gotoxy(105,38); printf("                                                                                          ");
            gotoxy(105,38); printf("* %s", comandos[2].descripicion);
        } 
        if((cursor.x==134)&&(cursor.y==36)&&(seleccion!=4)){
            seleccion = 4;
            gotoxy(105,38); printf("                                                                                          ");
            gotoxy(105,38); printf("* %s", comandos[3].descripicion);
        } 
        if(GetAsyncKeyState(0x0D)){
            return seleccion;
        }
      
    }
    return 1;
}

int Atacar(Info * atacante, Info * atacado, bool defensa){
    int Dano;
    if(defensa == false){
        Dano = atacante->ATK-atacado->DEF;
        if(Dano<0)
        {
            return 0;
        }
        atacado ->HP -= Dano;
    }else{
        Dano = (int)abs(log((atacante->ATK)/2));
        atacado ->HP -= Dano;
    }
    
    return Dano;
}

void TurnoEnemigo(Jugador *Jugador, Info *Enemigo, int *ptri, bool *defensaJ, bool*defensaE){
    int E_select = rand()%2;
    printf("\033[0;31m");
    switch (E_select)
    {
    case 0:    
        gotoxy(105,(*ptri)); printf("%s Logro Infringir %i de dano", Enemigo->nombre, Atacar(Enemigo,Jugador ->datos, *defensaJ));
        *defensaJ = false;
        break;
    case 1:
        gotoxy(105,(*ptri)); printf("%s se defiende ante el siguiente ataque", Enemigo->nombre);
        (*defensaE) = true;
        break;
    }
    (*ptri)++;
}
bool usarobjetoenbatalla(Jugador *jugador){

    printf("\033[0;34m");
    List *inventario = jugador->inventario;
    TipoEquipamiento *item = firstList(inventario);

    gotoxy(0,41); printf("-----------------------que objeto vas a usar?----------------------");
    for(int i=42; i<45;i++){
        gotoxy(0,i); printf("|                                                                 |");
    }
    gotoxy(0,45); printf("|-------------Utiliza <- y -> Para seleccionar objeto-------------|");
    gotoxy(0,46); printf("-------------------------------------------------------------------");
    printf("\033[0;0m");
    gotoxy(5,43); printf("->%s", item->stats->nombre);
    gotoxy(1,44);printf("-*%s", item->stats->descripcion);
    while(true){
        Sleep(100);
        if(GetAsyncKeyState(0x27)){
            item = nextList(inventario);
            if(item != NULL){
                gotoxy(1,42); printf("                                           ");
                gotoxy(5,43); printf("->                                                  ");
                gotoxy(0,44); printf("|                                                                 |");
                gotoxy(5,43); printf("->%s", item->stats->nombre);
                gotoxy(1,44); printf("-*%s", item->stats->descripcion);
            }
            
        }
        if(GetAsyncKeyState(0x25)){
            item = prevList(inventario);
            if(item != NULL){
                gotoxy(1,42); printf("                                           ");
                gotoxy(5,43); printf("->                                                  ");
                gotoxy(0,44); printf("|                                                                 |");
                gotoxy(5,43); printf("->%s", item->stats->nombre);
                gotoxy(1,44); printf("-*%s", item->stats->descripcion);
            }
            
        }
        if(GetAsyncKeyState(0x1B)){
            
            for(int i=41; i<=46;i++){
                gotoxy(0,i); printf("                                                                   ");
            }
            return true;
        }

        if(GetAsyncKeyState(0x0D)){
            if(item !=NULL){
                if((strcmp(item->tipo,"Consumible")!=0)){
                    printf("\033[0;31m");
                    gotoxy(1,42); printf("este objeto no se puede consumir en batalla");
                    
                    printf("\033[0;0m");
                } else {
                    if((jugador->datos->HP)+(item->stats->HP)>jugador->datos->HPMAX) jugador->datos->HP = jugador->datos->HPMAX;
                    else jugador->datos->HP+= item->stats->HP;
                    jugador->datos->ATK += item->stats->ATK;
                    jugador->datos->DEF += item->stats->DEF;
                    popCurrent(inventario);
                    for(int i=41; i<=46;i++){
                        gotoxy(0,i); printf("                                                                   ");
                    }
                    return false;
                } 
            }
        }
    }
    return false;
    printf("\033[0;36m");
}

bool empezarbatalla(Jugador *jugador,Info *Enemigo){
    gotoxy(105,1);printf("o no aparecio un %s, rapido %s, Ataca", Enemigo->nombre, jugador ->datos->nombre);
    printf("\033[0;36m");
    gotoxy(105,33); printf("-------------------------------------Elige una opcion-------------------------------------");
    bool Def_enemigo = false , Def_jugador = false,turnoporobjeto=false;
    int i=2;
    while ((Enemigo ->HP>0)&&(jugador->datos->HP>0))
    {
        Sleep(500);
        gotoxy(105,39); printf(".%s - %i ",jugador ->datos->nombre,  jugador ->datos ->HP);
        gotoxy(105,40); printf(".%s - %i ",Enemigo ->nombre, Enemigo ->HP);
        turnoporobjeto=false;
        switch (comandoBatalla(jugador->ataques))
        {
        case 1:
            gotoxy(105,i); printf("%s Decidio atacar", jugador->datos->nombre);
            Sleep(20);
            gotoxy(105,i); printf("%s Logro Infringir %i de dano", jugador->datos, Atacar(jugador->datos,Enemigo,Def_enemigo));
            Def_enemigo =false;
            break;
        case 2:
            gotoxy(105,i); printf("%s se defiende ante el siguiente ataque", jugador->datos->nombre);
            Def_jugador =  true;
            break;
        case 3:
            gotoxy(105,i); printf("%s Decide utilizar un objeto", jugador->datos->nombre);
            turnoporobjeto = usarobjetoenbatalla(jugador);
            break;
        case 4:
            gotoxy(105,i); printf("%s intenta huir", jugador->datos->nombre);
            if((Huir())==true){
               i++;
                gotoxy(105,i); printf("%s Logro huir con exito", jugador->datos->nombre);
                Enemigo ->HP = Enemigo ->HPMAX;
                Sleep(500);
                batalla_final_limpiar();
                return true;
            }
            else{
                i++;
                gotoxy(105,i); printf("%s No pudo huir, vuelve a internarlo o lucha", jugador->datos->nombre);
            }  
            break;
        }
        i++;
        if((Enemigo->HP>=0)&&(turnoporobjeto == false)) TurnoEnemigo(jugador,Enemigo,&i,&Def_jugador,&Def_enemigo);
        printf("\033[0;36m");
        
        if(i>=31){
            printf("\033[0;35m");
            i=2;
            for(int j=2; j<=31;j++){
                gotoxy(104,j); printf("|                                                                                          |");
            }
            printf("\033[0;36m");
        }
    }
    
    if(Enemigo ->HP<=0){
        Enemigo ->HP = Enemigo ->HPMAX;
        gotoxy(105,33); printf("VICTORIA");
        Sleep(500);
        jugador->datos->PH += Enemigo->PH;
        batalla_final_limpiar();
        return true;
    } 
    else{
        gotoxy(105,33); printf("DERROTA");
        Sleep(500);
        batalla_final_limpiar();
        return false;
    }
    
}

Info *seleccionarenemigo(HashMap *Mapa,int numero){
    char *cadena = malloc(sizeof(char)*2);
    sprintf(cadena,"%i",numero);
    Pair *dato = searchMap(Mapa,cadena);
    return dato ->value;
}

void developerfunctions(List* listaJugadores, HashMap* Mapamonster){
     
    
    pantalla_batalla();
    
    if(empezarbatalla(firstList(listaJugadores),seleccionarenemigo(Mapamonster, rand()%10))==false){
        //printf("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO *SE ARDE*");
    }

    
}

bool Submenu(List *listaJugadores){
    printf("\033[0;33m");
    gotoxy(104,0); printf("--------------------------------------");
    gotoxy(104,8); printf("--------------------------------------");
    for(int i = 1; i< 8; i++)
    {
        gotoxy(103,i);printf("|*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|");
    }
    printf("\033[0;0m");
    gotoxy(113,2); printf("------MENU------");
    gotoxy(106,3); printf("O Stats-*-*-*-*-*-*-*-*O Inventario");
    gotoxy(106,4); printf("O Salir-*-*-*-*-*-*-*-*O Reiniciar");
    gotoxy(106,9); printf("ENTER--Seleccionar");
    coordenadas cursor;
    cursor.x = 106;
    cursor.y = 3;
    gotoxy(cursor.x,cursor.y); printf("#");
    int selecc = 0;
    while (true)
    {
        Sleep(100);
        //abajo
        if((GetAsyncKeyState(0x28)) && (cursor.y<=3))
        {
            gotoxy(cursor.x,cursor.y); printf("O");
            cursor.y++;
            gotoxy(cursor.x,cursor.y); printf("#");
            
            
        }
        //arriba
        if((GetAsyncKeyState(0x26)) && (cursor.y>=4))
        {
            gotoxy(cursor.x,cursor.y); printf("O");
            cursor.y--;
            gotoxy(cursor.x,cursor.y); printf("#");
            
        }
        //derecha131
        if((GetAsyncKeyState(0x27)) && (cursor.x<=131))
        {
            gotoxy(cursor.x,cursor.y); printf("O");
            cursor.x=129;
            gotoxy(cursor.x,cursor.y); printf("#");
            
            
        }

        if((GetAsyncKeyState(0x25)) && (cursor.x>=106))
        {
            gotoxy(cursor.x,cursor.y); printf("O");
            cursor.x=106;
            gotoxy(cursor.x,cursor.y); printf("#");
            
        }
        if(GetAsyncKeyState(0x1B)){
            for(int i = 0; i< 9; i++)
            {
                gotoxy(103,i); printf("                                       ");
            }
            return false;
        }

        if((cursor.x==106)&&(cursor.y==3)) selecc = 1;
        if((cursor.x==106)&&(cursor.y==4)) selecc = 2;
        if((cursor.x==129)&&(cursor.y==3)) selecc = 3;
        if((cursor.x==129)&&(cursor.y==4)) selecc = 4;
        
        gotoxy(104,0); printf("%i",selecc);
        if(GetAsyncKeyState(0x0D) && selecc == 1)
        {
            mostrar_perfiles(listaJugadores);
        }
        if(GetAsyncKeyState(0x0D) && selecc == 3){
            submenu_Inventario(listaJugadores);
        }
        if(GetAsyncKeyState(0x0D) && selecc == 2){
            exit(0);
        }
        //gotoxy(0,0);("%i%i",cursor.x,cursor.y);
        if(GetAsyncKeyState(0x0D) && selecc == 4){
            return true;
        }
    }
}


void submenu_Inventario(List *lista){
    printf("\033[0;35m");
    gotoxy(104, 12); printf("-------------------------------------");
    gotoxy(104, 32); printf("-------------------------------------");
    for(int i = 13; i < 32; i++){
        gotoxy(104, i); printf("|                                   |");
    }  
    printf("\033[0;0m");
    gotoxy(105, 31); printf("Presiona 'Enter' para ver opciones");

    gotoxy(113, 13); printf("     Inventario     ");
    mostrarInventario(lista);
    List *inventario = ((Jugador *)firstList(lista))->inventario;
    TipoEquipamiento *item = firstList(inventario);

    coordenadas mov;
    mov.x = 106;
    mov.y = 16;
    gotoxy(mov.x, mov.y); printf("-");
    int opcion;

    while(true){
        Sleep(100);

        if(GetAsyncKeyState(0x26) && mov.y >= 17){
            item = prevList(inventario);
            if(item != NULL){
                gotoxy(mov.x, mov.y); printf("+");
                mov.y--;
                gotoxy(mov.x, mov.y); printf(">");
                opcion = 1;
            }
        }

        if(GetAsyncKeyState(0x28) && mov.y <= 30){
            item = nextList(inventario);
            if(item != NULL){
                gotoxy(mov.x, mov.y); printf("+");
                mov.y++;
                gotoxy(mov.x, mov.y); printf(">");
                opcion = 2;
            }
        }

        if(GetAsyncKeyState(0x0D) && opcion == 1 || GetAsyncKeyState(0x0D) && opcion == 2){
            submenu_opciones(lista);
        }

        if(GetAsyncKeyState(0x5A)){
            gotoxy(104, 12); printf("                                     ");
            gotoxy(104, 32); printf("                                     ");
            for(int i = 13; i < 33; i++){
                gotoxy(104, i); printf("                                     ");
            }  
            return;
        }
    }
}


void mostrarInventario(List *lista){
    
    List *inventario = ((Jugador *)firstList(lista))->inventario;
    TipoEquipamiento *item = firstList(inventario);
    Jugador *usuario = firstList(lista);
    int i = 16;
    gotoxy(113,14); printf("   Jugador: %s ", usuario->datos->nombre);
    while(item != NULL){
        gotoxy(106, i); printf("  -%s ", item->stats->nombre);
        item = nextList(inventario);
        i++;
    }

}

void submenu_opciones(List *lista){
    printf("\033[0;34m");
    gotoxy(144, 12); printf("--------------------------------------------");
    gotoxy(144, 21); printf("--------------------------------------------");
    for(int i = 13; i < 21; i++){
        gotoxy(144, i); printf("|                                          |");
    }  
    printf("\033[0;0m");

    coordenadas mov;
    mov.x = 146;
    mov.y = 15;

    gotoxy(149, 13); printf("      OPTIONS");
    gotoxy(146, 15); printf("  1. Use");
    gotoxy(146, 16); printf("  2. Assign");
    gotoxy(146, 17); printf("  3. Drop");
    gotoxy(146, 18); printf("  4. Description");
    gotoxy(145, 20); printf("'Enter' para entrar y 'z' para devolverse");


    int opcion = 0;
    int marca = 0;
    Jugador *usuario = firstList(lista);
    while(true){
        Sleep(100);
        //flecha arriba
        if(GetAsyncKeyState(0x26) && mov.y >= 16){
            gotoxy(mov.x, mov.y); printf(" ");
            mov.y--;
            gotoxy(mov.x, mov.y); printf(">");
            opcion = 1;
        }

        //flecha abajo
        if(GetAsyncKeyState(0x28) && mov.y <= 17){
            gotoxy(mov.x, mov.y); printf(" ");
            mov.y++;
            gotoxy(mov.x, mov.y); printf(">");
            opcion = 2;
        }
        
        if(GetAsyncKeyState(0x0D) && mov.y > 17){
           mostrarDescrip(lista);

        }

        if(mov.y == 15){
            marca = 1;
        }
        else if(mov.y == 16){
            marca = 2;
        }
        else{
            marca = 3;
        }

        if(GetAsyncKeyState(0x0D) && opcion == 1|| GetAsyncKeyState(0x0D) && mov.y <= 17){
            mostrar(lista, marca);
        }

        if(GetAsyncKeyState(0x5A)){
            gotoxy(144, 12); printf("                                            ");
            gotoxy(144, 21); printf("                                            ");
            for(int i = 13; i < 21; i++){
                gotoxy(144, i); printf("                                             ");
            }  
            return;
        }

    }
}

void mostrarDescrip(List *lista){
    
    printf("\033[0;34m");
    gotoxy(104, 34); printf("---------------------------------------------------------------------------------");
    gotoxy(104, 48); printf("---------------------------------------------------------------------------------");
    for(int i = 35; i < 48; i++){
        gotoxy(104, i); printf("|                                                                               |");
    }  
    printf("\033[0;0m");

    List *inventario = ((Jugador *)firstList(lista))->inventario;
    TipoEquipamiento *item = firstList(inventario);
    Jugador *usuario = firstList(lista);
    
    gotoxy(134, 35); printf("Tipo clase: %s", usuario->clase);
    
    coordenadas mov;
    mov.x = 105;
    mov.y = 36;

    printf("\033[0;33m");
    gotoxy(109, 36); printf("Anterior");
    gotoxy(172, 36); printf("Siguiente");
    
    printf("\033[0;0m");
    gotoxy(106, 46); printf(" Flecha arriba para el anterior y flecha abajo para el siguiente");
    gotoxy(106, 47); printf("'esc' para salir");

    
    while(true){
        Sleep(100);

        if(GetAsyncKeyState(0x26) && mov.x >= 107){
            gotoxy(mov.x, mov.y); printf(" ");
            mov.x = 107;
            gotoxy(mov.x, mov.y); printf(">");
        }

        if(GetAsyncKeyState(0x28) && mov.x <= 170){
            gotoxy(mov.x, mov.y); printf(" ");
            mov.x = 170;
            gotoxy(mov.x, mov.y); printf(">");
        }

        printf("\033[0;0m");
        if(GetAsyncKeyState(0x26) && mov.x == 107){
            item = prevList(inventario);
            if(item != NULL){
                for(int j = 38; j < 44; j++){
                    gotoxy(105, j); printf("                                     ");
                }
                gotoxy(105, 38); printf(" -Tipo: %s ", item->tipo);
                gotoxy(105, 39); printf(" -Tipo armadura: %s ", item->tipoArmadura);
                gotoxy(105, 40); printf(" -Nombre: %s ", item->stats->nombre);
                gotoxy(105, 41); printf(" -Puntos ataque: %i ", item->stats->ATK);
                gotoxy(105, 42); printf(" -Puntos defensa: %i ", item->stats->DEF);
                gotoxy(105, 43); printf(" -Puntos vida max: %i ", item->stats->HPMAX);
                gotoxy(105, 44); printf(" -Descripcion: %s\n", item->stats->descripcion);
            }
        }

        if(GetAsyncKeyState(0x28) && mov.x == 170){
            item = nextList(inventario);
            if(item != NULL){
                for(int j = 38; j < 44; j++){
                    gotoxy(105, j); printf("                                     ");
                }
                gotoxy(105, 38); printf(" -Tipo: %s ", item->tipo);
                gotoxy(105, 39); printf(" -Tipo armadura: %s ", item->tipoArmadura);
                gotoxy(105, 40); printf(" -Nombre: %s ", item->stats->nombre);
                gotoxy(105, 41); printf(" -Puntos ataque: %i ", item->stats->ATK);
                gotoxy(105, 42); printf(" -Puntos defensa: %i ", item->stats->DEF);
                gotoxy(105, 43); printf(" -Puntos vida max: %i ", item->stats->HPMAX);
                gotoxy(105, 44); printf(" -Descripcion: %s\n", item->stats->descripcion);
            }
        }
        if(GetAsyncKeyState(0x1B)){
            for(int i = 34; i < 49; i++){
                gotoxy(104, i); printf("                                                                                 ");
            } 
            return;
        }
    }

}

void mostrar(List *lista, int marca){
    List *inventario = ((Jugador *)firstList(lista))->inventario;
    TipoEquipamiento *item = firstList(inventario);
    bool hecho = false;
    int opcion = 0;

    printf("\033[0;31m");
    gotoxy(144, 26); printf("-----------------------------------------");
    gotoxy(144, 46); printf("-----------------------------------------");
    for(int i = 27; i < 46; i++){
        gotoxy(144, i); printf("|                                       |");
    }  

    printf("\033[0;0m");
    gotoxy(152, 27); printf("Items");

    int j = 31;
    while(item != NULL){
        opcion++;
        gotoxy(146, j); printf("  %i. %s", opcion, item->stats->nombre);
        item = nextList(inventario);
        j++;
    }
    
    while(true){
        if(marca == 1){
            if(usar_item(inventario)){
                gotoxy(146, 45); printf("Accion realizada");
                Sleep(100);
                for(int i = 26; i < 47; i++){
                    gotoxy(144, i); printf("                                         "); 
                }   
                return;
            }
        }
        else if(marca == 2){
            if(asignar_item(lista)){
                gotoxy(146, 45); printf("Accion realizada");
                Sleep(100);
                for(int i = 26; i < 47; i++){
                    gotoxy(144, i); printf("                                         "); 
                }   
                return;
            }
        }
        else{
            if(eliminar_item(inventario)){
                gotoxy(146, 45); printf("Accion realizada");
                Sleep(100);
                for(int i = 26; i < 47; i++){
                    gotoxy(144, i); printf("                                         "); 
                }   
                return;
            }
        }
    }

}

bool usar_item(List *lista){
    bool hecho = false;
    coordenadas pos;
    pos.x = 146;
    pos.y = 31;

    TipoEquipamiento *item = firstList(lista);

    gotoxy(146, 29); printf("Seleccione 'x' para usar");

    while(true){
        Sleep(100);
        if(GetAsyncKeyState(0x26) && pos.y >= 32){
           item = prevList(lista);
            if(item != NULL){
                gotoxy(pos.x, pos.y); printf(" ");
                pos.y--;
                gotoxy(pos.x, pos.y); printf(">");
            }
        }
        if(GetAsyncKeyState(0x28) && pos.y <= 42){
            item = nextList(lista);
            if(item != NULL){
                gotoxy(pos.x, pos.y); printf(" ");
                pos.y++;
                gotoxy(pos.x, pos.y); printf(">");
            }
        }

        if(GetAsyncKeyState(0x58)){
            if(item != NULL){
                if(strcmp(item->tipo, "Consumible") == 0){
                    gotoxy(146, 44); printf("Puedes usarlo");
                    popCurrent(lista);
                    hecho = true;
                }
                else{
                    gotoxy(146, 44); printf("No puedes usarlo");
                }
            }
            return hecho;
        }
    }
    return hecho;

}

bool asignar_item(List *lista){
    List *lista_items = ((Jugador *)firstList(lista))->inventario;
    TipoEquipamiento *item = firstList(lista_items);
    Jugador *usuario = firstList(lista);

    HashMap *mapa = usuario->equipamiento;
    TipoEquipamiento *aux = (TipoEquipamiento *)malloc(sizeof(TipoEquipamiento));
    
    bool hecho = false;
    coordenadas pos;
    pos.x = 146;
    pos.y = 31;

    gotoxy(146, 29); printf("Seleccione 'x' para asignar item");

    while(true){
        Sleep(100);
        if(GetAsyncKeyState(0x26) && pos.y >= 32){
           item = prevList(lista_items);
            if(item != NULL){
                gotoxy(pos.x, pos.y); printf(" ");
                pos.y--;
                gotoxy(pos.x, pos.y); printf(">");
            }
        }
        if(GetAsyncKeyState(0x28) && pos.y <= 42){
            item = nextList(lista_items);
            if(item != NULL){
                gotoxy(pos.x, pos.y); printf(" ");
                pos.y++;
                gotoxy(pos.x, pos.y); printf(">");
            }
        }

        if(GetAsyncKeyState(0x58)){
            if(item != NULL){
                if(strcmp(item->tipo, "Arma") == 0){
                    if(searchMap(mapa, item->tipo)){
                        aux = (TipoEquipamiento* )searchMap(mapa, item->tipo);
                        item->equipado = false;
                        eraseMap(mapa, aux->tipo);
                    }
                    insertMap(mapa, item->tipo, item);
                    item->equipado = true;
                    hecho = true;
                }
                else{
                    if(strcmp(item->tipo, "Armadura") == 0){
                        if(searchMap(mapa, item->tipo)){
                            aux = (TipoEquipamiento *) searchMap(mapa, item->tipo);
                            if(strcmp(aux->tipo, "Casco") == 0){
                                item->equipado = false;
                                eraseMap(mapa, aux->tipo);
                            }
                            else if(strcmp(aux->tipo, "Pecho") == 0){
                                item->equipado = false;
                                eraseMap(mapa, aux->tipo);
                            }
                            else if(strcmp(aux->tipo, "Piernas") == 0){
                                item->equipado = false;
                                eraseMap(mapa, aux->tipo);
                            }
                            else{
                                if(strcmp(aux->tipo, "Botas")== 0){
                                    item->equipado = false;
                                    eraseMap(mapa, aux->tipo);
                                }
                            }
                        }
                        insertMap(mapa, item->tipo, item);
                        item->equipado = true;
                        hecho = true;
                    }
                }
            }
            return hecho;
        }
    }

    return hecho;
}


bool eliminar_item(List *lista){
    
    bool hecho = false;
    coordenadas pos;
    pos.x = 146;
    pos.y = 31;
    TipoEquipamiento *item = firstList(lista);
    
    gotoxy(146, 29); printf("Seleccione 'x' para eliminar");

    while(true){
        Sleep(100);
        if(GetAsyncKeyState(0x26) && pos.y >= 32){
           item = prevList(lista);
            if(item != NULL){
                gotoxy(pos.x, pos.y); printf(" ");
                pos.y--;
                gotoxy(pos.x, pos.y); printf(">");
            }
        }
        if(GetAsyncKeyState(0x28) && pos.y <= 42){
            item = nextList(lista);
            if(item != NULL){
                gotoxy(pos.x, pos.y); printf(" ");
                pos.y++;
                gotoxy(pos.x, pos.y); printf(">");
            }
        }

        if(GetAsyncKeyState(0x58)){
            if(item != NULL){
                popCurrent(lista);
            }
            hecho = true;
            return hecho;
        }
    }

    return hecho;
}


int validarmov(sala *sandbox, int x, int y,Jugador *player, int *cont)
{
    if(sandbox->tamano[y][x] == ' ') return 0;
    if(sandbox->tamano[y][x]=='>')
    {
        limpiarpantalla();
        generarmapa(sandbox);
        player->pos.x = 2;
        player->pos.y = 19;
        (*cont)++;
        return 1;
    }
    if(sandbox->tamano[y][x] == '@')
    {
        return 2;
    }
    if(sandbox->tamano[y][x] == '$')
    {
        return 3;
    }
    return 1;
}

TipoEquipamiento* seleccionaritem(HashMap *Mapaitems,int numero){
    char *cadena = malloc(sizeof(char)*4);
    sprintf(cadena,"%i",numero);
    Pair *dato = searchMap(Mapaitems,cadena);
    if(dato != NULL){
        return dato ->value;
    }
}


void faseDElanzamiento(List *listaJugadores,sala *sandbox,HashMap *Mapamonster, char *estado){
    Jugador *mainPlayer = firstList(listaJugadores);
    HashMap *Mapaitems = generaritems();
    int cont = 0;
    while(true)
    {
        if(strcmp(estado,"dead") == 0)
        {
            limpiarpantalla();
            return;
        }
        if(cont == 4)
        {
            strcpy(estado,"win");
            return;
        }
        Sleep(100);
        printf("\033[0;35m");
        //Moverse a la izquierda
        if((GetAsyncKeyState(0x25)))
        {
            if(validarmov(sandbox,mainPlayer->pos.x-1,mainPlayer->pos.y,mainPlayer,&cont) == 0)
            {
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.x--;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else if (validarmov(sandbox,mainPlayer->pos.x-1,mainPlayer->pos.y,mainPlayer,&cont) == 2)
            {
                /* code */
                pantalla_batalla();
                if(empezarbatalla(firstList(listaJugadores),seleccionarenemigo(Mapamonster, rand()%10))==false){
                    strcpy(estado,"dead");
                }
                else
                {
                    gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                    mainPlayer->pos.x--;
                    gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
                }
            }
            else if(validarmov(sandbox,mainPlayer->pos.x-1,mainPlayer->pos.y,mainPlayer,&cont) == 3)
            {
                if(strcmp(mainPlayer->clase,"Chef") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,rand()%10));
                }
                else if(strcmp(mainPlayer->clase,"Ladron") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,10+rand()%10));
                }
                else if(strcmp(mainPlayer->clase,"Espadachin") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,20+rand()%10));
                }
                else
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,30+rand()%10));
                }
                gotoxy(25,FILAS); printf("HAS OBTENIDO UN ITEM");
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.x--;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else continue;

        }
        //derecha
        if((GetAsyncKeyState(0x27)))
        {
            if(validarmov(sandbox,mainPlayer->pos.x+1,mainPlayer->pos.y,mainPlayer,&cont) == 0)
            {
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.x++;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else if (validarmov(sandbox,mainPlayer->pos.x+1,mainPlayer->pos.y,mainPlayer,&cont) == 2)
            {
                /* code */
                pantalla_batalla();
                if(empezarbatalla(firstList(listaJugadores),seleccionarenemigo(Mapamonster, rand()%10))==false){
                    strcpy(estado,"dead");
                }
                else
                {
                    gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                    mainPlayer->pos.x++;
                    gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
                }
            }
            else if(validarmov(sandbox,mainPlayer->pos.x+1,mainPlayer->pos.y,mainPlayer,&cont) == 3)
            {
                if(strcmp(mainPlayer->clase,"Chef") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,rand()%10));
                }
                else if(strcmp(mainPlayer->clase,"Ladron") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,10+rand()%10));
                }
                else if(strcmp(mainPlayer->clase,"Espadachin") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,20+rand()%10));
                }
                else
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,30+rand()%10));
                }
                gotoxy(25,FILAS); printf("HAS OBTENIDO UN ITEM");
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.x++;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }

            else continue;
        }

        //abajo
        if((GetAsyncKeyState(0x28)))
        {
            if(validarmov(sandbox,mainPlayer->pos.x,mainPlayer->pos.y+1,mainPlayer,&cont) == 0)
            {
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.y++;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else if (validarmov(sandbox,mainPlayer->pos.x,mainPlayer->pos.y+1,mainPlayer,&cont) == 2)
            {
                /* code */
                pantalla_batalla();
                if(empezarbatalla(firstList(listaJugadores),seleccionarenemigo(Mapamonster, rand()%10))==false){
                    strcpy(estado,"dead");
                }
                else
                {
                    gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                    mainPlayer->pos.y++;
                    gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
                }
            }
            else if(validarmov(sandbox,mainPlayer->pos.x,mainPlayer->pos.y+1,mainPlayer,&cont) == 3)
            {
                if(strcmp(mainPlayer->clase,"Chef") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,rand()%10));
                }
                else if(strcmp(mainPlayer->clase,"Ladron") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,10+rand()%10));
                }
                else if(strcmp(mainPlayer->clase,"Espadachin") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,20+rand()%10));
                }
                else
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,30+rand()%10));
                }
                gotoxy(25,FILAS); printf("HAS OBTENIDO UN ITEM");
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.y++;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }

            else continue;
        }
        //arriba
        if((GetAsyncKeyState(0x26)))
        {
            if(validarmov(sandbox,mainPlayer->pos.x,mainPlayer->pos.y-1,mainPlayer,&cont) == 0)
            {
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.y--;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else if (validarmov(sandbox,mainPlayer->pos.x,mainPlayer->pos.y-1,mainPlayer,&cont) == 2)
            {
                /* code */
                pantalla_batalla();
                if(empezarbatalla(firstList(listaJugadores),seleccionarenemigo(Mapamonster, rand()%10))==false){
                    strcpy(estado,"dead");
                }
                else
                {
                    gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                    mainPlayer->pos.y--;
                    gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
                }
            }
            else if(validarmov(sandbox,mainPlayer->pos.x,mainPlayer->pos.y-1,mainPlayer,&cont) == 3)
            {
                if(strcmp(mainPlayer->clase,"Chef") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,rand()%10));
                }
                else if(strcmp(mainPlayer->clase,"Ladron") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,10+rand()%10));
                }
                else if(strcmp(mainPlayer->clase,"Espadachin") == 0)
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,20+rand()%10));
                }
                else
                {
                    pushBack(mainPlayer->inventario,seleccionaritem(Mapaitems,30+rand()%10));
                }
                gotoxy(25,FILAS); printf("HAS OBTENIDO UN ITEM");
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.y--;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }

            else continue;
        }

        if(GetAsyncKeyState(0x1B)){
            if(Submenu(listaJugadores)){
                strcpy(estado, "reset");
                return;
            }
        }
        gotoxy(0,FILAS); printf("ESC--Menu de pausa");

        if(GetAsyncKeyState(0x09)){
            
            //developerfunctions(listaJugadores,Mapamonster);
        }
    }
}


void rellenarmapa(sala * sandbox, int posfila, int poscolum, int largo, char caracter)
{
    if(largo >1)
    {
        for(size_t j = 0; j< largo; j++)
        {
            if(caracter == '-')
            {
                
                sandbox->tamano[posfila][j+poscolum] = caracter;
                gotoxy(j+poscolum,posfila);printf("%c",sandbox->tamano[posfila][j+poscolum]);
            }
            else if(caracter == '|' || caracter == '>')
            {
                sandbox->tamano[j+posfila][poscolum] = caracter;
                gotoxy(poscolum,j+posfila);printf("%c",sandbox->tamano[j+posfila][poscolum]);
            }
        }
    }
    else
    {
        sandbox->tamano[posfila][poscolum] = caracter;
        gotoxy(poscolum,posfila); printf("%c",sandbox->tamano[posfila][poscolum]);
    }
}

void generarmapa(sala *sandbox)
{
    //rand()%6
    int variable = rand()%6;
    for(size_t i = 0; i<FILAS ; i++)
    {
        for(size_t j = 0; j<COLUMNAS ; j++)
        {
            sandbox->tamano[i][j] = ' ';
            if(i == 0 || i == FILAS -1)
            {
                sandbox->tamano[i][j] = '-';
            }
            if( i != 0 && i != FILAS-1 && (j == 0 || j == COLUMNAS -1))
            {
                sandbox->tamano[i][j] = '|';
            }
            
            
        }
    }
    printf("\033[0;32m");

    for(size_t i = 0; i<FILAS; i++)
    {
        for(size_t j = 0; j< COLUMNAS; j++)
        {
            gotoxy(j,i);
            printf("%c",sandbox->tamano[i][j]);
        }
    }
    if (variable == 0)  //MEJORAR
    {

//PAREDES  VERTICALES
        rellenarmapa(sandbox, 0, 8, 8, '|'); 
        rellenarmapa(sandbox, 10, 5, 15, '|'); 
        rellenarmapa(sandbox, 5, 10, 5, '|'); 
        rellenarmapa(sandbox, 27, 5, 11, '|'); 
        rellenarmapa(sandbox, 22, 10, 5, '|'); 
        rellenarmapa(sandbox, 25, 28, 13, '|'); 
        rellenarmapa(sandbox, 25, 30, 15, '|'); 
        rellenarmapa(sandbox, 30, 32, 8, '|'); 
        rellenarmapa(sandbox, 25, 58, 2, '|'); 
        rellenarmapa(sandbox, 20, 58, 2, '|'); 
        rellenarmapa(sandbox, 32, 54, 6, '|'); 
        rellenarmapa(sandbox, 34, 58, 6, '|'); 
        rellenarmapa(sandbox, 5, 30, 15, '|'); 
        rellenarmapa(sandbox, 0, 32, 22, '|'); 
        rellenarmapa(sandbox, 10, 40, 8, '|'); 
        rellenarmapa(sandbox, 18, 60, 12, '|'); 
        rellenarmapa(sandbox, 10, 70, 22, '|'); 
        rellenarmapa(sandbox, 0, 70, 8, '|'); 
        rellenarmapa(sandbox, 10, 73, 22, '|'); 
        rellenarmapa(sandbox, 32, 80, 2, '|'); 
        rellenarmapa(sandbox, 10, 82, 28, '|'); 
        rellenarmapa(sandbox, 10, 90, 20, '|');
        rellenarmapa(sandbox, 10, 94, 20, '|');
        rellenarmapa(sandbox, 0, 82, 8, '|');



        //PAREDES HORIZONTALES
        rellenarmapa(sandbox, 30, 90, 4, '-'); 
        rellenarmapa(sandbox, 30, 32, 28, '-'); 
        rellenarmapa(sandbox, 8, 0, 8, '-'); 
        rellenarmapa(sandbox, 10, 5, 5, '-'); 
        rellenarmapa(sandbox, 25, 0, 5, '-');
        rellenarmapa(sandbox, 5, 10, 20, '-');
        rellenarmapa(sandbox, 20, 10, 20, '-');
        rellenarmapa(sandbox, 22, 10, 48, '-');
        rellenarmapa(sandbox, 25, 14, 16, '-');
        rellenarmapa(sandbox, 38, 5, 23, '-');
        rellenarmapa(sandbox, 10, 40, 30, '-');
        rellenarmapa(sandbox, 18, 40, 20, '-');
        rellenarmapa(sandbox, 25, 30, 28, '-');
        rellenarmapa(sandbox, 27, 30, 28, '-');
        rellenarmapa(sandbox, 20, 32, 26, '-');
        rellenarmapa(sandbox, 32, 54, 16, '-');
        rellenarmapa(sandbox, 38,32, 22, '-');
        rellenarmapa(sandbox, 34, 58, 22, '-');
        rellenarmapa(sandbox, 32, 73, 7, '-');
        rellenarmapa(sandbox, 8, 70, 12, '-');
        rellenarmapa(sandbox, 10, 73, 9, '-');
        rellenarmapa(sandbox, 10, 90, 10, '-');
        rellenarmapa(sandbox, 32, 82, 18, '-');
        rellenarmapa(sandbox, 27, 5, 4, '-');

        printf("\e[1;33m");
        rellenarmapa(sandbox, 20,99,5,'>');

        //ENTIDADES
        printf("\033[0;31m");
        rellenarmapa(sandbox,32,71,0,'@');
        rellenarmapa(sandbox,29,72,0,'@');
        rellenarmapa(sandbox,25,71,0,'@');
        rellenarmapa(sandbox,20,72,0,'@');

        //TESOROS
        printf("\033[0;33m");
        rellenarmapa(sandbox,1,92,0,'$');
        rellenarmapa(sandbox,3,20,0,'$');
        rellenarmapa(sandbox,3,35,0,'$');
    }
    else if(variable == 1) //"FALTA TERMINAR MAPA"
    {
        printf("\033[0;35m");
        //rellenarmapa(sandbox, 5, 5, 15, '-');    
        //rellenarmapa(sandbox, 5, 5, 10, '|');    
        //rellenarmapa(sandbox, 5, 20, 10, '|');   
        //rellenarmapa(sandbox, 15, 5, 20, '-');   

        //rellenarmapa(sandbox, 10, 5, 10, '-');   

        //PAREDES VERTICALES F2
        rellenarmapa(sandbox, 0, 50, 10, '|');    
        rellenarmapa(sandbox, 0, 55, 10, '|');    
        rellenarmapa(sandbox, 12, 50, 13, '|');
        rellenarmapa(sandbox, 10, 65, 8, '|');    
        rellenarmapa(sandbox, 20, 65, 5, '|');

        rellenarmapa(sandbox, 20, 95, 2, '|');
        rellenarmapa(sandbox, 23, 95, 2, '|');

        rellenarmapa(sandbox, 18, 80, 2, '|');
        rellenarmapa(sandbox, 20, 75, 3, '|');

        rellenarmapa(sandbox, 12, 45, 18, '|');
        rellenarmapa(sandbox, 12, 5, 20, '|');

        rellenarmapa(sandbox, 28, 80, 4, '|');
        rellenarmapa(sandbox, 32,60, 8, '|');

        //PAREDES HORIZONTALES F2
        rellenarmapa(sandbox, 10, 0, 50, '-');
        rellenarmapa(sandbox, 12, 5, 40, '-');
        rellenarmapa(sandbox, 25, 50, 15, '-');
        rellenarmapa(sandbox, 10, 55, 10, '-');
        //verrr

        rellenarmapa(sandbox, 32, 5, 40, '-');
        //
        rellenarmapa(sandbox, 18, 65, 15, '-');
        rellenarmapa(sandbox, 20, 65, 10, '-');

        rellenarmapa(sandbox, 32, 60, 20, '-');
        rellenarmapa(sandbox, 28, 80, 20, '-');

        rellenarmapa(sandbox, 23, 75, 20, '-');
        rellenarmapa(sandbox, 20, 80, 15, '-');

        rellenarmapa(sandbox, 19, 95, 5, '-');
        rellenarmapa(sandbox, 25, 95, 5, '-');

        printf("\e[1;33m");
        rellenarmapa(sandbox, 20,99,5,'>');


        //entidades
        printf("\033[0;31m");
        rellenarmapa(sandbox,21,77,0,'@');
        rellenarmapa(sandbox,20,96,0,'@');
        rellenarmapa(sandbox,23,96,0,'@');
        rellenarmapa(sandbox,30,70,0,'@');

        //Entidades
        printf("\033[0;31m");
        rellenarmapa(sandbox,21,77,0,'@');
        rellenarmapa(sandbox,20,96,0,'@');
        rellenarmapa(sandbox,23,96,0,'@');
        rellenarmapa(sandbox,30,70,0,'@');

        //TESOROS
        printf("\033[0;33m");
        rellenarmapa(sandbox,25,20,0,'$');
        rellenarmapa(sandbox,28,25,0,'$');
        rellenarmapa(sandbox,21,96,0,'$');

    }
    else if (variable == 2) //"TERMINADO"
    {
        printf("\033[0;35m"); 

        //PAREDES VERTICALES F3
        rellenarmapa(sandbox, 5, 30, 10, '|');    
        rellenarmapa(sandbox, 9, 35, 9, '|');    
        rellenarmapa(sandbox, 25, 15, 2, '|');
        rellenarmapa(sandbox, 15, 50, 10, '|');    
        rellenarmapa(sandbox, 9, 75, 6, '|');

        rellenarmapa(sandbox, 5, 80, 13, '|');    
        rellenarmapa(sandbox, 18, 66, 2, '|');    
        rellenarmapa(sandbox, 20, 78, 5, '|');
        rellenarmapa(sandbox, 27, 50, 3, '|');    
        rellenarmapa(sandbox, 19, 85, 6, '|');
        rellenarmapa(sandbox, 22, 88, 8, '|');
        rellenarmapa(sandbox, 22, 95, 6, '|');

        //PAREDES HORIZONTALES F3

        rellenarmapa(sandbox, 15, 0, 30, '-');
        rellenarmapa(sandbox, 18, 15, 20, '-');
        rellenarmapa(sandbox, 18, 0, 13, '-');
        rellenarmapa(sandbox, 5, 30, 50, '-');
        rellenarmapa(sandbox, 9, 35, 40, '-');
        rellenarmapa(sandbox, 15, 50, 25, '-');
        rellenarmapa(sandbox, 18, 66, 14, '-');
        rellenarmapa(sandbox, 20, 66, 12, '-');
        rellenarmapa(sandbox, 25, 78, 7, '-');
        //rellenarmapa(sandbox, 20, 15, 35, '-');
        rellenarmapa(sandbox, 27, 15, 35, '-');
        rellenarmapa(sandbox, 30, 50, 38, '-');
        rellenarmapa(sandbox, 25, 15, 35, '-');
        rellenarmapa(sandbox, 18, 85, 15, '-');
        rellenarmapa(sandbox, 22, 88, 7, '-');
        rellenarmapa(sandbox, 28, 95, 5, '-');

        printf("\e[1;33m");
        rellenarmapa(sandbox, 20,99,5,'>');

        printf("\033[0;31m");
        rellenarmapa(sandbox,26,92,0,'@');
        rellenarmapa(sandbox,20,86,0,'@');
        rellenarmapa(sandbox,8,32,0,'@');
        //rellenarmapa(sandbox,,,0,'@');

        //TESOROS
        printf("\033[0;33m");
        rellenarmapa(sandbox,25,90,0,'$');
        rellenarmapa(sandbox,17,66,0,'$');
        rellenarmapa(sandbox,6,32,0,'$');

    }
    else if(variable == 3)   //TERMINADO
    {

        printf("\033[0;34m");
        //PAREDES VERTICALES 
        rellenarmapa(sandbox, 5, 10, 15, '|');
        rellenarmapa(sandbox, 24, 16, 8, '|');
        rellenarmapa(sandbox, 28, 13, 7, '|');
        rellenarmapa(sandbox, 20, 20, 12, '|');
        rellenarmapa(sandbox, 12, 23, 23, '|');
        rellenarmapa(sandbox, 5, 23, 3, '|');
        rellenarmapa(sandbox, 12, 40, 23, '|');
        rellenarmapa(sandbox, 8, 43, 24, '|');
        rellenarmapa(sandbox, 12, 46, 20, '|');
        rellenarmapa(sandbox, 5, 55, 7, '|');
        rellenarmapa(sandbox, 8, 60, 4, '|');
        rellenarmapa(sandbox, 12, 63, 23, '|');
        rellenarmapa(sandbox, 8, 80, 20, '|');



        //PAREDES HORIZONTALES 
        rellenarmapa(sandbox, 5, 10, 13, '-');
        rellenarmapa(sandbox, 8, 23, 20, '-');
        rellenarmapa(sandbox, 12, 23, 17, '-');
        rellenarmapa(sandbox, 5, 55, 45, '-');
        rellenarmapa(sandbox, 8, 60, 20, '-');
        rellenarmapa(sandbox, 12, 46, 9, '-');
        rellenarmapa(sandbox, 12, 60, 3, '-');
        rellenarmapa(sandbox, 20, 10, 10, '-');
        rellenarmapa(sandbox, 24, 3, 13, '-');
        rellenarmapa(sandbox, 28, 0, 13, '-');
        rellenarmapa(sandbox, 32, 16, 4, '-');
        rellenarmapa(sandbox, 35, 13, 10, '-');
        rellenarmapa(sandbox, 32, 43, 3, '-');
        rellenarmapa(sandbox, 35, 40, 23, '-');
        rellenarmapa(sandbox, 28, 80, 20, '-');
        
        printf("\e[1;33m");
        rellenarmapa(sandbox, 20,99,5,'>');
        
        printf("\033[0;31m");
        rellenarmapa(sandbox,25,90,0,'@');
        rellenarmapa(sandbox,33,14,0,'@');
        rellenarmapa(sandbox,34,41,0,'@');
        rellenarmapa(sandbox,20,60,0,'@');
        rellenarmapa(sandbox,25,60,0,'@');

        //TESOROS
        printf("\033[0;33m");
        rellenarmapa(sandbox,5,5,0,'$');
        rellenarmapa(sandbox,10,11,0,'$');
        rellenarmapa(sandbox,23,95,0,'$');
        rellenarmapa(sandbox,6,58,0,'$');

    }
    else if(variable ==4)
    {
        printf("\033[0;35m");
        //PAREDES VERTICALES 
        rellenarmapa(sandbox, 5,15, 5, '|');
        rellenarmapa(sandbox, 10,5, 5, '|');
        rellenarmapa(sandbox, 30,25, 3, '|');
        rellenarmapa(sandbox, 33,10, 4, '|');
        rellenarmapa(sandbox, 30,35, 6, '|');
        rellenarmapa(sandbox, 30,45, 6, '|');


        rellenarmapa(sandbox, 5, 5, 4, '|');    
        rellenarmapa(sandbox, 5, 20, 9, '|');  

        rellenarmapa(sandbox, 2,50,13,'|');
        rellenarmapa(sandbox, 15,55, 5, '|');
        rellenarmapa(sandbox, 25,55, 10, '|');
        rellenarmapa(sandbox, 28,65, 7, '|');
        rellenarmapa(sandbox, 18,68, 5, '|');
        rellenarmapa(sandbox, 1, 55, 2, '|');
        rellenarmapa(sandbox, 3,60, 7, '|');
        rellenarmapa(sandbox, 1, 70, 8, '|');    
        rellenarmapa(sandbox, 0, 75, 19, '|'); 
        rellenarmapa(sandbox, 20, 20, 5, '|');




        //PAREDES HORIZONTALES
        rellenarmapa(sandbox, 30,1,25,'-');
        rellenarmapa(sandbox, 32,10,15,'-');
        rellenarmapa(sandbox, 36,10,25,'-');
        rellenarmapa(sandbox, 30,35,10,'-');
        rellenarmapa(sandbox, 10,5,10,'-');
        rellenarmapa(sandbox, 36,45,30,'-');


        rellenarmapa(sandbox, 5, 5, 15, '-');    
        rellenarmapa(sandbox, 15, 5, 50, '-'); 

        rellenarmapa(sandbox, 1,55,15,'-');
        rellenarmapa(sandbox, 3,55,5,'-');
        //rellenarmapa(sandbox, 15,45,10,'-');
        rellenarmapa(sandbox, 20,20,35,'-');
        rellenarmapa(sandbox, 25,20,35,'-');
        rellenarmapa(sandbox, 34,55,10,'-');
        rellenarmapa(sandbox, 28,65,30,'-');
        rellenarmapa(sandbox, 23, 68, 7, '-');    
        rellenarmapa(sandbox, 18, 68, 31, '-');  
        rellenarmapa(sandbox, 10,60, 10, '-');
        rellenarmapa(sandbox, 20,45, 10, '-');

        printf("\e[1;33m");
        rellenarmapa(sandbox, 20,99,5,'>');

        printf("\033[0;31m");
        rellenarmapa(sandbox,7,10,0,'@');
        rellenarmapa(sandbox,18,50,0,'@');
        rellenarmapa(sandbox,20,67,0,'@');
        //rellenarmapa(sandbox,,,0,'@');

        //TESOROS
        printf("\033[0;33m");
        rellenarmapa(sandbox,23,54,0,'$');
        rellenarmapa(sandbox,8,12,0,'$');
        rellenarmapa(sandbox,20,65,0,'$');
    }
    else
    {
        printf("\033[0;34m");
        //PAREDES VERTICALES 
        rellenarmapa(sandbox, 5,5, 7, '|');
        rellenarmapa(sandbox, 30,5, 4, '|');
        rellenarmapa(sandbox, 25,10, 5, '|');
        rellenarmapa(sandbox, 20,12, 2, '|');
        rellenarmapa(sandbox, 10,15, 5, '|');
        rellenarmapa(sandbox, 0,18, 5, '|');

        rellenarmapa(sandbox, 22,25, 8, '|');
        rellenarmapa(sandbox, 0,25, 15, '|');
        rellenarmapa(sandbox, 22,30, 8, '|');
        rellenarmapa(sandbox, 22,60, 8, '|');
        rellenarmapa(sandbox, 0,40, 20, '|');
        rellenarmapa(sandbox, 0,45, 20, '|');
        rellenarmapa(sandbox, 34,70, 4, '|');
        rellenarmapa(sandbox, 5,70, 15, '|');
        rellenarmapa(sandbox, 10,73,24, '|');
        rellenarmapa(sandbox, 5,90, 5, '|');
       

       //PAREDES HORIZONTALES

        rellenarmapa(sandbox, 12, 1, 4, '-');    
        rellenarmapa(sandbox, 5, 5, 13, '-'); 

        rellenarmapa(sandbox, 10,15,4,'-');
        rellenarmapa(sandbox, 15,15,10,'-');
        rellenarmapa(sandbox, 25,1,9,'-');
        rellenarmapa(sandbox, 30,1,4,'-');
        rellenarmapa(sandbox, 30,10,5,'-');
        rellenarmapa(sandbox, 30,20,5,'-');
        rellenarmapa(sandbox, 22,12,13,'-');
        rellenarmapa(sandbox, 20, 12,28, '-');    
        rellenarmapa(sandbox, 22, 30, 30, '-');  
        rellenarmapa(sandbox, 30,30, 30, '-');
        rellenarmapa(sandbox, 34,5, 65, '-');

         rellenarmapa(sandbox, 20, 45, 25, '-');    
        rellenarmapa(sandbox, 5, 70, 20, '-'); 

        rellenarmapa(sandbox, 10,73,17,'-');
        rellenarmapa(sandbox, 34,73,24,'-');
        rellenarmapa(sandbox, 38,70,30,'-');

        printf("\e[1;33m");
        rellenarmapa(sandbox, 20,99,5,'>');

        //entidades
        printf("\033[0;31m");
        rellenarmapa(sandbox,32,50,0,'@');
        rellenarmapa(sandbox,32,60,0,'@');
        rellenarmapa(sandbox,8,72,0,'@');
        rellenarmapa(sandbox,8,15,0,'@');
        rellenarmapa(sandbox,33,55,0,'@');

        //TESOROS
        printf("\033[0;33m");
        rellenarmapa(sandbox,25,28,0,'$');
        rellenarmapa(sandbox,33,50,0,'$');
        rellenarmapa(sandbox,26,63,0,'$');
        rellenarmapa(sandbox,28,1,0,'$');
    }

}


void limpiarpantalla(){
    
    for(int i=0; i<=64; i++){
        gotoxy(0,i);  
        printf("\033[2J\033[H");                                                                                                         
    }

    for(int i=0; i<=80; i++){
        gotoxy(i,17); printf("\033[2J\033[H");
    }
}

void mostrar_perfiles (List *lista){
   Jugador *usuario= firstList(lista);

   gotoxy(106,10); printf("Nombre = %s\n",usuario ->datos->nombre);
   gotoxy(106,11); printf("Clase = %s\n",usuario->clase);
   gotoxy(106,12); printf("Estadisticas:\n");
   gotoxy(106,13); printf("  -Ataque = %i",usuario ->datos->ATK);
   gotoxy(106,14); printf("  -Defensa = %i",usuario ->datos->DEF);
   gotoxy(106,15); printf("  -Puntos de vida = %i/%i",usuario->datos->HP, usuario -> datos->HPMAX);
   gotoxy(106,16); printf("  -PH = %i",usuario->datos->PH);

   
}
void gotoxy (int x, int y){
    HANDLE consola= GetStdHandle(STD_OUTPUT_HANDLE);
    COORD posicion;
    posicion.X = x;
    posicion.Y = y;
    SetConsoleCursorPosition(consola,posicion);
}


void mainmenu(){

    gotoxy(50,0) ; printf("                 s");
    gotoxy(50,1) ; printf("                sss");
    gotoxy(50,2) ; printf("                sss");
    gotoxy(50,3)  ;printf("               sssss");
    gotoxy(50,4)  ;printf("               sssss");
    gotoxy(50,5)  ;printf("               sssss");
    gotoxy(50,6)  ;printf("               sssss");
    gotoxy(50,7)  ;printf("              sssssss");
    gotoxy(50,8)  ;printf("              sssssss");
    gotoxy(50,9)  ;printf("              sssssss");
    gotoxy(50,10) ;printf("             sssssssss");
    gotoxy(50,11) ;printf("             sssssssss");
    gotoxy(50,12) ;printf("             sssssssss");
    gotoxy(50,13) ;printf("             sssssssss");
    gotoxy(50,14) ;printf("             sssssssss");
    gotoxy(50,15) ;printf("             sssssssss");
    gotoxy(50,16) ;printf("             sssssssss");
    gotoxy(50,17) ;printf("             sssssssss");
    gotoxy(50,18) ;printf("             sssssssss");
    gotoxy(50,19) ;printf("             sssssssss");
    gotoxy(50,20) ;printf("             sssssssss");
    gotoxy(50,21) ;printf("             sssssssss    \033[0;32m  ssss");
    gotoxy(50,21) ;printf("     sss    \033[0;0m sssssssss\033[0;32m     sss");
    gotoxy(50,22) ;printf("   ssss    \033[0;0m ssssssssss\033[0;32m     ssss");
    gotoxy(50,23) ;printf("    sssssssssssssssssssssssss");printf("\033[0;32m");
    gotoxy(50,24) ;printf("              ssssssss");
    gotoxy(50,25) ;printf("               ssssss");
    gotoxy(50,26) ;printf("               ssssss");
    gotoxy(50,27) ;printf("               ssssss");
    gotoxy(50,28) ;printf("               ssssss");
    gotoxy(50,29) ;printf("             ssssssss");
    gotoxy(50,30) ;printf("                sssss");
    gotoxy(50,31) ;printf("             sssssssss");
    gotoxy(50,32) ;printf("               ssssss");printf("\033[0;0m");
    printf("\033[0;34m");
    gotoxy(60,34) ;printf("o Nueva Partida");
    gotoxy(60,35) ;printf("o Salir del Juego");

    int seleccion =1;
    while(true){
        Sleep(100);
        
        if(GetAsyncKeyState(0x1B)){
            
            printf("%i",seleccion);
            if(seleccion == 1){
                limpiarpantalla();
                gotoxy(50,15); printf("Hora de comenzar una nueva aventura\n");
                Beep(16,3);
                return;
            } 
            else if(seleccion ==2){
                exit(0);
            }
        }
        if(GetAsyncKeyState(0x26)){

            gotoxy(60,34); printf("+ Nueva Partida");
            gotoxy(60,35) ;printf("o Salir del Juego");
            seleccion=1;
        }
        if(GetAsyncKeyState(0x28)){
            gotoxy(60,34) ;printf("o Nueva Partida");
            gotoxy(60 ,35) ;printf("+ Salir del Juego");
            seleccion=2;
            
        }

        //gotoxy(78,32);printf("%i", seleccion);
    }

}

/*const */char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){
        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }
    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }
    return NULL;
}

HashMap* generaritems(){
  HashMap *mapaaux = createMap(41);
  FILE *fp = fopen ("Items.txt", "r");
  if(fp == NULL){
    printf("el archivo no existe\n");
    return NULL; 
  } 
  char linea[1024];
  int i;
  int cant =0;
  fgets (linea, 1023, fp);
  //nombre,tipo,tipoarmor,ataque,defensa,hp,hpmax,Ph,descripcion
  while (fgets (linea, 1023, fp) != NULL) { // Se lee la linea
    TipoEquipamiento *equipo = (TipoEquipamiento*)malloc(sizeof(TipoEquipamiento));
    equipo->stats = (Info*)malloc(sizeof(Info));
    for(i = 0;i < 13;i++){
      char *aux = get_csv_field(linea, i); // Se obtiene el nombre
      
      if(aux != NULL){
        if(i == 0)
        {
          strcpy(equipo->stats->nombre, aux);
        }
        if(i == 1)
        {
            equipo->tipo=malloc(sizeof(char)*(strlen(aux)+1));
          strcpy(equipo->tipo,aux);
        }
        if(i == 2)
        {
            equipo->tipoArmadura=malloc(sizeof(char)*(strlen(aux)+1));
          strcpy(equipo->tipoArmadura,aux);
        }
        if(i == 3)
        {
          equipo->stats ->ATK = atoi(aux);
        }
        if(i == 4){
          equipo->stats ->DEF = atoi(aux);
        }
        if(i == 5){
          equipo->stats ->HP = atoi(aux);
        }
        if(i == 6){
          equipo->stats ->HPMAX = atoi(aux);
        }
        if(i == 7){
          equipo->stats ->PH =atoi(aux);
        }
        if(i==8){
          equipo->stats ->descripcion = malloc(sizeof(char)*(strlen(aux)+1));
          strcpy(equipo->stats ->descripcion, aux );
        }
      }  
    }
    equipo->equipado = false;
    char *ubicacion = malloc(sizeof(char)*3);
    sprintf(ubicacion,"%i",cant);
    insertMap(mapaaux, ubicacion, equipo);
    cant++;
  }
  return mapaaux; 
}

HashMap* almacenarmounstruos(){
  HashMap *mapaaux = createMap(12);
  FILE *fp = fopen ("Bestiario.txt", "r");
  if(fp == NULL){
    printf("el archivo no existe\n");
    return NULL; 
  } 
  char linea[1024];
  int i;
  int cant =0;
  fgets (linea, 1023, fp);
  while (fgets (linea, 1023, fp) != NULL) { // Se lee la linea
    Info *datos = malloc(sizeof(Info));
    for(i = 0;i < 13;i++){
      char *aux = get_csv_field(linea, i); // Se obtiene el nombre
      
      if(aux != NULL){
        if(i == 0)
        {
          strcpy(datos->nombre, aux);
        }
        if(i == 1)
        {
          datos ->ATK = atoi(aux);
        }
        if(i == 2){
          datos ->DEF = atoi(aux);
        }
        if(i == 3){
          datos ->HP = atoi(aux);
        }
        if(i == 4){
          datos ->HPMAX = atoi(aux);
        }
        if(i == 5){
          datos ->PH =atoi(aux);
        }
        if(i==5){
          datos ->descripcion = malloc(sizeof(char)*(strlen(aux)+1));
          strcpy(datos ->descripcion, aux );
        }
      }  
    }
    
    char *ubicacion = malloc(sizeof(char)*2);
    sprintf(ubicacion,"%i",cant);
    insertMap(mapaaux, ubicacion, datos);
    cant++;
  }
  return mapaaux; 
}

void CrearPerfil(List *lista){
    Jugador *usuario = malloc(sizeof(Jugador));
    usuario ->datos = malloc(sizeof(Info));
    Sleep(100);
    gotoxy(50,17); printf("Pero primero, debes decirme tu nombre\n");
    gotoxy(50,18);
    scanf("%99[^\n]s",usuario->datos->nombre);
    //getchar();
    limpiarpantalla();
    gotoxy(50,17); printf("Asi que tu nombre es %s",usuario->datos->nombre);
    gotoxy(50,18); printf("Muy bien, pues ahora veamos que es lo que quieres ser");
    selccionarclase(usuario);
    estadisticasDeclase(usuario);
    inventarionuevo(usuario);
    OpcionesBatalla(usuario);
    usuario ->pos.x=2;
    usuario ->pos.y=18;
    usuario ->datos->PH = 0;
    //faltan el quipamiento 
    pushBack(lista ,usuario);
}

void OpcionesBatalla(Jugador *usuario){
    strcpy(usuario->ataques[0].nombre,"ATACAR");
    strcpy(usuario->ataques[1].nombre,"DEFENDER");
    strcpy(usuario->ataques[2].nombre,"ITEMS");
    strcpy(usuario->ataques[3].nombre,"HUIR");
    
    strcpy(usuario->ataques[0].descripicion,"Utiliza tu arma para poder hacerle dano al enemigo");
    strcpy(usuario->ataques[1].descripicion,"Si nesesitas pensar o no pudes hacer nada, defiendete para recibir menos dano");
    strcpy(usuario->ataques[2].descripicion,"Juntaste algun objeto que puedas usar? Que esperas? USALO!!!!");
    strcpy(usuario->ataques[3].descripicion,"Ya no te queda de otra? :( HUYE ANTES QUE TE MATEN, o te da flojera pelear? vete");

}

void inventarionuevo(Jugador *usuario){
    usuario ->inventario = createList();
    usuario ->equipamiento = createMap(6);
    equipamientoBase(usuario);
}


void equipamientoBase(Jugador *usuario)
{
    if(strcmp("Espadachin",usuario->clase) == 0)
    {
        equipobaseE(usuario);
        return;
    }
    if(strcmp("Mago",usuario->clase) == 0)
    {
        equipobaseM(usuario);
        return;
    }
    if(strcmp("Ladron",usuario->clase) == 0)
    {
        equipobaseL(usuario);
        return;
    }
    if(strcmp("Chef",usuario->clase) == 0)
    {
        equipobaseC(usuario);
        return;
    }

}
TipoEquipamiento *createEquipoBase()
{
    TipoEquipamiento *equipoBase = (TipoEquipamiento*)malloc(sizeof(TipoEquipamiento));
    equipoBase->stats = (Info*)malloc(sizeof(Info));
    equipoBase->tipo = (char*)malloc(sizeof(char)*(strlen("Consumible")+1));
    equipoBase->tipoArmadura = (char*)malloc(sizeof(char)*(strlen("Piernas")+1));
    equipoBase->stats->descripcion = (char*)malloc(sizeof(char)*70);
    return equipoBase;
}


void equipobaseE(Jugador *usuario)
{
    
    TipoEquipamiento *equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Casco");
    strcpy(equipoBase->stats->nombre,"Casco de guerrero");
    equipoBase->stats->ATK = 1;
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

    aplicarItem(usuario,equipoBase);
    equipoBase = createEquipoBase();


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Pecho");
    strcpy(equipoBase->stats->nombre,"Armadura de guerrero");
    equipoBase->stats->ATK = 3;
    equipoBase->stats->HPMAX = 7;
    equipoBase->stats->DEF = 3;
    strcpy(equipoBase->stats->descripcion,"Una armadura basica, lo sufucientemente robusta para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Piernas");
    strcpy(equipoBase->stats->nombre,"Pantalones de guerrero");
    equipoBase->stats->ATK = 2;
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un Pantalon basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Botas");
    strcpy(equipoBase->stats->nombre,"Botas de guerrero");
    equipoBase->stats->ATK = 0;
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Unas Botas basicas, lo sufucientemente robustas para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Arma");
    strcpy(equipoBase->stats->nombre,"Espada larga");
    equipoBase->stats->ATK = 5;
    equipoBase->stats->HPMAX = 0;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipo,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Consumible");
    strcpy(equipoBase->tipoArmadura,"None");
    strcpy(equipoBase->stats->nombre,"Pocion de curacion");
    equipoBase->stats->ATK = 0;
    equipoBase->stats->HPMAX = 0;
    equipoBase->stats->HP = 5;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Amargo pero te cura");

    pushBack(usuario->inventario,equipoBase);
    usuario->datos->HP = usuario->datos->HPMAX;
}

void equipobaseM(Jugador *usuario)
{
    TipoEquipamiento *equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Casco");
    strcpy(equipoBase->stats->nombre,"Sombrero de mago");
    equipoBase->stats->ATK = 3;
    equipoBase->stats->HPMAX = 2;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un sombrero simple");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Pecho");
    strcpy(equipoBase->stats->nombre,"Tunica de mago");
    equipoBase->stats->ATK = 5;
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Una ropa comun entre los magos");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Piernas");
    strcpy(equipoBase->stats->nombre,"Pantalones de mago");
    equipoBase->stats->ATK = 4;
    equipoBase->stats->HPMAX = 2;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Un Pantalon basico, pura vestimenta");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Botas");
    strcpy(equipoBase->stats->nombre,"Zapatos de mago");
    equipoBase->stats->ATK = 4;
    equipoBase->stats->HPMAX = 2;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Unas Botas basicas, lo sufucientemente robustas para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Arma");
    strcpy(equipoBase->tipoArmadura,"None");
    strcpy(equipoBase->stats->nombre,"Varita magica");
    equipoBase->stats->ATK = 10;
    equipoBase->stats->HPMAX = 0;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Una simple varita");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipo,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Consumible");
    strcpy(equipoBase->tipoArmadura,"None");
    strcpy(equipoBase->stats->nombre,"Pocion de curacion");
    equipoBase->stats->ATK = 0;
    equipoBase->stats->HPMAX = 0;
    equipoBase->stats->HP = 5;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Amargo pero te cura");

    pushBack(usuario->inventario,equipoBase);
    usuario->datos->HP = usuario->datos->HPMAX;
}

void equipobaseL(Jugador *usuario)
{
    TipoEquipamiento *equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Casco");
    strcpy(equipoBase->stats->nombre,"Capa de cuero");
    equipoBase->stats->ATK = 3;
    equipoBase->stats->HPMAX = 4;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Confundico como un bandido");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Pecho");
    strcpy(equipoBase->stats->nombre,"Armadura de cuero");
    equipoBase->stats->ATK = 5;
    equipoBase->stats->HPMAX = 8;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Ligero pero protege");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);
    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Piernas");
    strcpy(equipoBase->stats->nombre,"Pantalones de lino");
    equipoBase->stats->ATK = 2;
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un Pantalon de clase baja");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);
    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Botas");
    strcpy(equipoBase->stats->nombre,"Botas de cuero");
    equipoBase->stats->ATK = 4;
    equipoBase->stats->HPMAX = 3;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Unas Botas ruidosas");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Arma");
    strcpy(equipoBase->tipoArmadura,"None");
    strcpy(equipoBase->stats->nombre,"Daga");
    equipoBase->stats->ATK = 7;
    equipoBase->stats->HPMAX = 0;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Por lo menos corta");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipo,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Consumible");
    strcpy(equipoBase->tipoArmadura,"None");
    strcpy(equipoBase->stats->nombre,"Pocion de curacion");
    equipoBase->stats->ATK = 0;
    equipoBase->stats->HPMAX = 0;
    equipoBase->stats->HP = 5;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Amargo pero te cura");

    pushBack(usuario->inventario,equipoBase);
    usuario->datos->HP = usuario->datos->HPMAX;
}


void equipobaseC(Jugador *usuario)
{

    TipoEquipamiento *equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Casco");
    strcpy(equipoBase->stats->nombre,"Gorro de chef");
    equipoBase->stats->ATK = 3;
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Un gorro basico");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

    aplicarItem(usuario,equipoBase);
    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Pecho");
    strcpy(equipoBase->stats->nombre,"Uniforme de chef");
    equipoBase->stats->ATK = 4;
    equipoBase->stats->HPMAX = 10;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Un uniforme sucio");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);
    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Piernas");
    strcpy(equipoBase->stats->nombre,"Pantalones de chef");
    equipoBase->stats->ATK = 3;
    equipoBase->stats->HPMAX = 8;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Un Pantalon sucio");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Botas");
    strcpy(equipoBase->stats->nombre,"Zapatos de chef");
    equipoBase->stats->ATK = 3;
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un par de zapatos");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    aplicarItem(usuario,equipoBase);

    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Arma");
    strcpy(equipoBase->tipoArmadura,"None");
    strcpy(equipoBase->stats->nombre,"Sarten sucio");
    equipoBase->stats->ATK = 9;
    equipoBase->stats->HPMAX = 2;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un sarten muy usado");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipo,equipoBase);
    aplicarItem(usuario,equipoBase);
    equipoBase = createEquipoBase();

    strcpy(equipoBase->tipo,"Consumible");
    strcpy(equipoBase->tipoArmadura,"None");
    strcpy(equipoBase->stats->nombre,"Pocion de curacion");
    equipoBase->stats->ATK = 0;
    equipoBase->stats->HPMAX = 0;
    equipoBase->stats->HP = 5;
    equipoBase->stats->DEF = 0;
    strcpy(equipoBase->stats->descripcion,"Amargo pero te cura");

    pushBack(usuario->inventario,equipoBase);
    usuario->datos->HP = usuario->datos->HPMAX;
}

/*fin de chantarle equipamiento a las clases*/

//Apicar estadisticas de equipamiento
void aplicarItem(Jugador *usuario,TipoEquipamiento *item)
{
    Info *aux = usuario->datos;
    if(item->equipado == true)
    {
        aux->ATK += item->stats->ATK;
        aux->DEF += item->stats->DEF;
        aux->HPMAX += item->stats->HPMAX;
    }
}


void estadisticasDeclase(Jugador *usuario){
    if(strcmp("Espadachin",usuario->clase)==0){
        //estadisticas Espadachin
        usuario ->datos->ATK= 10;
        usuario -> datos ->DEF = 10;
        usuario -> datos ->HPMAX = 20;
        usuario -> datos ->HP = usuario -> datos ->HPMAX;
        usuario ->nivel = 1;
        usuario ->PH = 0;

    }else if(strcmp("Mago",usuario->clase)==0){
        //estadisticas Mago
        usuario ->datos->ATK= 20;
        usuario -> datos ->DEF = 5;
        usuario -> datos ->HPMAX = 10;
        usuario -> datos ->HP = usuario -> datos ->HPMAX;
        usuario ->nivel = 1;
        usuario ->PH = 0;
    }else if(strcmp("Ladron",usuario->clase)==0){
        //estadisticas Ladron
        usuario ->datos->ATK= 17;
        usuario -> datos ->DEF = 7;
        usuario -> datos ->HPMAX = 15;
        usuario -> datos ->HP = usuario -> datos ->HPMAX;
        usuario ->nivel = 1;
        usuario ->PH = 0;
    }else{
        //estadisticas Chef
        usuario ->datos->ATK= 10;
        usuario -> datos ->DEF = 15;
        usuario -> datos ->HPMAX = 20;
        usuario -> datos ->HP = usuario -> datos ->HPMAX;
        usuario ->nivel = 1;
        usuario ->PH = 0;
    }
}

void selccionarclase(Jugador *usuario){
    int clase;
    gotoxy(50,19); printf("Ingresa la clase a eleccion");
    gotoxy(52,20); printf("1.-Espadachin");
    gotoxy(52,21); printf("2.-Mago");
    gotoxy(52,22); printf("3.-Ladron");
    gotoxy(52,23); printf("4.-Chef"); 
    gotoxy(52,24);
    scanf("%i", &clase);
    switch (clase){
      case 1:
            usuario->clase = malloc(sizeof(char) * (strlen("Espadachin") + 1));
            strcpy(usuario->clase, "Espadachin");
            break;
        case 2:
            usuario->clase = malloc(sizeof(char) * (strlen("Mago") + 1));
            strcpy(usuario->clase, "Mago");
            break;
        case 3:
            usuario->clase = malloc(sizeof(char) * (strlen("Ladron") + 1));
            strcpy(usuario->clase, "Ladron");
            break;
        case 4:
            usuario->clase = malloc(sizeof(char) * (strlen("Chef") + 1));
            strcpy(usuario->clase, "Chef");
            break;
        default:
            break;
      
    }
}

void pantallainesesariadecarga(){
    printf("");
    gotoxy(50, 10);
    //printf("Bienvenido, porfavor espere");
    int i=0;
    char porciento ='%';
    while (true)
    {
        /* code */
        Sleep(100);
        gotoxy(50, 11);printf("Cargando ...%i%c",i,porciento);
        i++;
        if(i>10) break;
    }
}