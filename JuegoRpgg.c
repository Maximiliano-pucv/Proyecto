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
bool validarmov(sala * sandbox, int x, int y);

/*equipamiento por clase*/
void equipamientoBase(Jugador *usuario);
void equipobaseE(Jugador *usuario);
void equipobaseM(Jugador *usuario);
void equipobaseL(Jugador *usuario);
void equipobaseC(Jugador *usuario);

void faseDElanzamiento(List *listaJugadores,sala *sandbox);
//funciones solo developers (fran)
void mostrar_perfiles (List *lista);
void Submenu();
void submenu_Inventario();
void mostrarInventario();
void submenu_opciones();
void mostrarDescrip();

//main
int main(){
    time_t t;
    srand((unsigned) time(&t));

    pantallainesesariadecarga();
    mainmenu();
    List *listajugadores = createList();
    CrearPerfil(listajugadores);
    //mostrar_perfiles(listajugadores);
    limpiarpantalla();
    pantallainesesariadecarga();
    limpiarpantalla();
    sala *sandbox = (sala*)malloc(sizeof(sala));
    generarmapa(sandbox);
    faseDElanzamiento(listajugadores,sandbox);
    
    //mostrar_perfiles(listajugadores);
    return 0;
}

void Submenu(){
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
    gotoxy(106,4); printf("O salir-*-*-*-*-*-*-*-*O Ajustes");
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
            gotoxy(cursor.x,cursor.y);printf("O");
            cursor.y++;
            gotoxy(cursor.x,cursor.y); printf("#");
            selecc = 1;
        }
        //arriba
        if((GetAsyncKeyState(0x26)) && (cursor.y>=4))
        {
            gotoxy(cursor.x,cursor.y);printf("O");
            cursor.y--;
            gotoxy(cursor.x,cursor.y); printf("#");
            selecc = 2;
        }
        //derecha131
        if((GetAsyncKeyState(0x27)) && (cursor.x<=131))
        {
            gotoxy(cursor.x,cursor.y);printf("O");
            cursor.x=129;
            gotoxy(cursor.x,cursor.y); printf("#");
            selecc = 3;
        }

        if((GetAsyncKeyState(0x25)) && (cursor.x>=106))
        {
            gotoxy(cursor.x,cursor.y);printf("O");
            cursor.x=106;
            gotoxy(cursor.x,cursor.y); printf("#");
            selecc = 4;
        }
        if(GetAsyncKeyState(0x1B)){
            for(int i = 0; i< 9; i++)
            {
                gotoxy(103,i);printf("                                       ");
            }
            return;
        }

        if(GetAsyncKeyState(0x0D) && selecc == 3){
            submenu_Inventario();
        }
        //gotoxy(0,0);("%i%i",cursor.x,cursor.y);

    }

}

void submenu_Inventario(){
    printf("\033[0;35m");
    gotoxy(104, 12); printf("-------------------------------------");
    gotoxy(104, 38); printf("-------------------------------------");
    for(int i = 13; i < 38; i++){
        gotoxy(104, i); printf("|                                   |");
    }  
    printf("\033[0;0m");

    gotoxy(113, 13); printf("     Inventario     ");
    mostrarInventario();

    coordenadas mov;
    mov.x = 106;
    mov.y = 15;
    gotoxy(mov.x, mov.y); printf("-");
    int opcion;
    while(true){
        Sleep(100);
        if(GetAsyncKeyState(0x26) && mov.y >= 16){
            gotoxy(mov.x, mov.y);printf("+");
            mov.y--;
            gotoxy(mov.x, mov.y); printf(">");
            opcion = 1;
        }

        if(GetAsyncKeyState(0x28) && mov.y <= 34){
            gotoxy(mov.x, mov.y);printf("+");
            mov.y++;
            gotoxy(mov.x, mov.y); printf(">");
            opcion = 2;
        }

        if(GetAsyncKeyState(0x0D) && opcion == 1 || GetAsyncKeyState(0x0D) && opcion == 2){
            submenu_opciones();
        }
    }
}


void mostrarInventario(){
    
    Jugador *item = (Jugador *) malloc(sizeof(Jugador ));

    for(int i = 15; i < 36; i++){
        gotoxy(106, i); printf("  -%s ", item->datos->nombre);
    }

}

void submenu_opciones(){
    printf("\033[0;34m");
    gotoxy(144, 12); printf("-------------------------------------");
    gotoxy(144, 20); printf("-------------------------------------");
    for(int i = 13; i < 20; i++){
        gotoxy(144, i); printf("|                                   |");
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

    int opcion = 0;
    while(true){
        Sleep(100);
        //flecha arriba
        if(GetAsyncKeyState(0x26) && mov.y >= 16){
            gotoxy(mov.x, mov.y);printf(" ");
            mov.y--;
            gotoxy(mov.x, mov.y); printf(">");
            opcion = 1;
        }

        //flecha abajo
        if(GetAsyncKeyState(0x28) && mov.y <= 17){
            gotoxy(mov.x, mov.y);printf(" ");
            mov.y++;
            gotoxy(mov.x, mov.y); printf(">");
            opcion = 2;
        }
        /*
        if(GetAsyncKeyState(0x5A)){
            //return;
        }*/

        if(GetAsyncKeyState(0x0D) && opcion == 1 || GetAsyncKeyState(0x0D) && opcion == 2){
           mostrarDescrip();
        }

        if(GetAsyncKeyState(0x5A)){
            return;
        }
        //cerrar la pestaña (falta)

    }
}

void mostrarDescrip(){
    
    printf("\033[0;34m");
    gotoxy(144, 26); printf("-------------------------------------");
    gotoxy(144, 36); printf("-------------------------------------");
    for(int i = 27; i < 36; i++){
        gotoxy(144, i); printf("|                                   |");
    }  
    printf("\033[0;0m");

    //gotoxy(148, 36); printf(" ");

    coordenadas mov;
    mov.x = 148;
    mov.y = 28;

     while(true){
        Sleep(100);
        //flecha arriba
        if(GetAsyncKeyState(0x26) && mov.y >= 27){
            gotoxy(mov.x, mov.y);printf(" ");
            mov.y--;
            gotoxy(mov.x, mov.y); printf(">");
            //opcion = 1;
        }

        //flecha abajo
        if(GetAsyncKeyState(0x28) && mov.y <= 30){
            gotoxy(mov.x, mov.y);printf(" ");
            mov.y++;
            gotoxy(mov.x, mov.y); printf(">");
           // opcion = 2;
        }

        if(GetAsyncKeyState(0x1B)){
            return;
        }
    }
    Jugador *descrip = (Jugador *) malloc(sizeof(Jugador));
   
}

bool validarmov(sala *sandbox, int x, int y)
{
    if(sandbox->tamano[y][x] == ' ' || isalpha(sandbox->tamano[y][x])) return true;
    return false;
}

//△
void faseDElanzamiento(List *listaJugadores,sala *sandbox){
    Jugador *mainPlayer = firstList(listaJugadores);
    
    while(true)
    {
        Sleep(100);
        printf("\033[0;35m");
        //Moverse a la izquierda
        if((GetAsyncKeyState(0x25)))
        {
            if(validarmov(sandbox,mainPlayer->pos.x-1,mainPlayer->pos.y) == true)
            {
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.x--;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else continue;

        }
        //derecha
        if((GetAsyncKeyState(0x27)))
        {
            if(validarmov(sandbox,mainPlayer->pos.x+1,mainPlayer->pos.y) == true)
            {
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.x++;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else continue;
        }

        //abajo
        if((GetAsyncKeyState(0x28)))
        {
            if(validarmov(sandbox,mainPlayer->pos.x,mainPlayer->pos.y+1) == true)
            {
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.y++;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else continue;
        }
        //arriba
        if((GetAsyncKeyState(0x26)))
        {
            if(validarmov(sandbox,mainPlayer->pos.x,mainPlayer->pos.y-1) == true)
            {
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
                mainPlayer->pos.y--;
                gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
            }
            else continue;
        }

        if(GetAsyncKeyState(0x1B)){
            Submenu();
        }
    }
}


void rellenarmapa(sala * sandbox, int posfila, int poscolum, int largo, char caracter)
{
    if(largo >0)
    {
        for(size_t j = 0; j< largo; j++)
        {
            if(caracter == '-')
            {
                
                sandbox->tamano[posfila][j+poscolum] = caracter;
                gotoxy(j+poscolum,posfila);printf("%c",sandbox->tamano[posfila][j+poscolum]);
            }
            else if(caracter == '|')
            {
                sandbox->tamano[j+posfila][poscolum] = caracter;
                gotoxy(poscolum,j+posfila);printf("%c",sandbox->tamano[j+posfila][poscolum]);
            }
        }
    }   
}

void generarmapa(sala *sandbox)
{
    
    int variable = 1;
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
    int salas[7] ={0}; //arreglo para evitar la repeticion de las salas
    printf("\033[0;32m");

    for(size_t i = 0; i<FILAS; i++)
    {
        for(size_t j = 0; j< COLUMNAS; j++)
        {
            gotoxy(j,i);
            printf("%c",sandbox->tamano[i][j]);
        }
    }

    if (variable == 1)
    {
        rellenarmapa(sandbox,15,1,7,'-');

        gotoxy(1,21); printf("-------");
    }
    else if(variable == 2)
    {
        gotoxy(1,20); printf("---------");
        gotoxy(1,23); printf("---------");
    }
    else if (variable == 3)
    {
        gotoxy(20,8); printf("---");
        gotoxy(23,9); printf("C");
    }
    else if(variable == 4)
    {
        gotoxy(2,1); printf("|");
    }
    else if(variable == 5)
    {
        gotoxy(60,25); printf("---");
    }
    else if(variable == 6)
    {
        gotoxy(50,20); printf("A");
    }
    else
    {
        gotoxy(51,25); printf("B");
    }
    printf("\033[0;0m");

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
   printf("\n");
   while(usuario != NULL){
    printf("Nombre = %s\n",usuario ->datos->nombre);
    printf("Clase = %s\n",usuario->clase);
    printf("Estadisticas:\n");
    printf("  -Ataque = %i",usuario ->datos->ATK);
    printf("  -Defensa = %i",usuario ->datos->DEF);
    printf("  -Puntos de vida = %i", usuario -> datos ->HPMAX);
    printf("  -Nivel = %i\n",usuario->nivel);
    printf("Opciones de batalla\n");
    printf("%s\n%s\n%s\n%s\n",usuario ->ataques[0].nombre,usuario ->ataques[1].nombre,usuario ->ataques[2].nombre,usuario ->ataques[3].nombre);
    usuario = nextList(lista);
    printf("\n");
    
   }
   
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
    gotoxy(50,21) ;printf("      sss    sssssssss    sss");
    gotoxy(50,21) ;printf("     sss     sssssssss     sss");
    gotoxy(50,22) ;printf("   ssss     ssssssssss     ssss");
    gotoxy(50,23) ;printf("    sssssssssssssssssssssssss");
    gotoxy(50,24) ;printf("              ssssssss");
    gotoxy(50,25) ;printf("               ssssss");
    gotoxy(50,26) ;printf("               ssssss");
    gotoxy(50,27) ;printf("               ssssss");
    gotoxy(50,28) ;printf("               ssssss");
    gotoxy(50,29) ;printf("             ssssssss");
    gotoxy(50,30) ;printf("                sssss");
    gotoxy(50,31) ;printf("             sssssssss");
    gotoxy(50,32) ;printf("               ssssss");
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

void CrearPerfil(List *lista){
    Jugador *usuario = malloc(sizeof(Jugador));
    usuario ->datos = malloc(sizeof(Info));
    Sleep(100);
    gotoxy(50,17); printf("Pero primero, debes decirme tu nombre\n");
    gotoxy(50,18);
    scanf("%99[^\n]s",usuario->datos->nombre);
    getchar();
    limpiarpantalla();
    gotoxy(50,17); printf("Asi que tu nombre es %s",usuario->datos->nombre);
    gotoxy(50,18); printf("Muy bien, pues ahora veamos que es lo que quieres ser");
    selccionarclase(usuario);
    estadisticasDeclase(usuario);
    //inventarionuevo(usuario);
    OpcionesBatalla(usuario);
    usuario ->pos.x=10;
    usuario ->pos.y=18;

    //faltan el quipamiento 
    pushBack(lista ,usuario);
}

void OpcionesBatalla(Jugador *usuario){
    strcpy(usuario->ataques[0].nombre,"ATACAR");
    strcpy(usuario->ataques[1].nombre,"DEFENDER");
    strcpy(usuario->ataques[2].nombre,"ITEMS");
    strcpy(usuario->ataques[3].nombre,"HUIR");
    
    strcpy(usuario->ataques[0].descripicion,"Utiliza tu arma para poder hacerle daño al enemigo");
    strcpy(usuario->ataques[1].descripicion,"Si nesesitas pensar o no pudes hacer nada, defiendete para recibir menos daño");
    strcpy(usuario->ataques[2].descripicion,"Juntaste algun objeto que puedas usar? Que esperas? solo hay una forma de saber que hacer");
    strcpy(usuario->ataques[3].descripicion,"Ya no te queda de otra? :( HUYE ANTES QUE TE MATEN, o te da flojera pelear? vete");

}

void inventarionuevo(Jugador *usuario){
    usuario ->inventario = createList();
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

void equipobaseE(Jugador *usuario)
{
    /*
    TipoEquipamiento *equipoBase = (TipoEquipamiento*)malloc(sizeof(TipoEquipamiento));
    equipoBase->stats = (Info*)malloc(sizeof(Info));
    equipoBase->tipo = (char*)malloc(sizeof(char));
    equipoBase->tipoArmadura = (char*)malloc(sizeof(char));
    equipoBase->stats->descripcion = (char*)malloc(sizeof(char));


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Casco");
    strcpy(equipoBase->stats->nombre,"Casco de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Pecho");
    strcpy(equipoBase->stats->nombre,"Armadura de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Una armadura basica, lo sufucientemente robusta para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Piernas");
    strcpy(equipoBase->stats->nombre,"Pantalones de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un Pantalon basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Botas");
    strcpy(equipoBase->stats->nombre,"Botas de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Unas Botas basicas, lo sufucientemente robustas para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Arma");
    strcpy(equipoBase->stats->nombre,"Espada larga");
    equipoBase->stats->ATK = 5;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);*/

}

void equipobaseM(Jugador *usuario)
{
   /* TipoEquipamiento *equipoBase = (TipoEquipamiento*)malloc(sizeof(TipoEquipamiento));
    equipoBase->stats = (Info*)malloc(sizeof(Info));
    equipoBase->tipo = (char*)malloc(sizeof(char));
    equipoBase->tipoArmadura = (char*)malloc(sizeof(char));
    equipoBase->stats->descripcion = (char*)malloc(sizeof(char));


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Casco");
    strcpy(equipoBase->stats->nombre,"Casco de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Pecho");
    strcpy(equipoBase->stats->nombre,"Armadura de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Una armadura basica, lo sufucientemente robusta para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Piernas");
    strcpy(equipoBase->stats->nombre,"Pantalones de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un Pantalon basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Botas");
    strcpy(equipoBase->stats->nombre,"Botas de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Unas Botas basicas, lo sufucientemente robustas para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Arma");
    strcpy(equipoBase->stats->nombre,"Espada larga");
    equipoBase->stats->ATK = 5;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);*/

}

void equipobaseL(Jugador *usuario)
{
    /*
    TipoEquipamiento *equipoBase = (TipoEquipamiento*)malloc(sizeof(TipoEquipamiento));
    equipoBase->stats = (Info*)malloc(sizeof(Info));
    equipoBase->tipo = (char*)malloc(sizeof(char));
    equipoBase->tipoArmadura = (char*)malloc(sizeof(char));
    equipoBase->stats->descripcion = (char*)malloc(sizeof(char));


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Casco");
    strcpy(equipoBase->stats->nombre,"Casco de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Pecho");
    strcpy(equipoBase->stats->nombre,"Armadura de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Una armadura basica, lo sufucientemente robusta para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Piernas");
    strcpy(equipoBase->stats->nombre,"Pantalones de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un Pantalon basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Botas");
    strcpy(equipoBase->stats->nombre,"Botas de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Unas Botas basicas, lo sufucientemente robustas para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Arma");
    strcpy(equipoBase->stats->nombre,"Espada larga");
    equipoBase->stats->ATK = 5;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);
    */
}

/*void equipobaseC(Jugador *usuario)
{
    TipoEquipamiento *equipoBase = (TipoEquipamiento*)malloc(sizeof(TipoEquipamiento));
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

}*/

void equipobaseC(Jugador *usuario)
{
    /*TipoEquipamiento *equipoBase = (TipoEquipamiento*)malloc(sizeof(TipoEquipamiento));
    equipoBase->stats = (Info*)malloc(sizeof(Info));
    equipoBase->tipo = (char*)malloc(sizeof(char));
    equipoBase->tipoArmadura = (char*)malloc(sizeof(char));
    equipoBase->stats->descripcion = (char*)malloc(sizeof(char));


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Casco");
    strcpy(equipoBase->stats->nombre,"Casco de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Pecho");
    strcpy(equipoBase->stats->nombre,"Armadura de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Una armadura basica, lo sufucientemente robusta para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Piernas");
    strcpy(equipoBase->stats->nombre,"Pantalones de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Un Pantalon basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Armadura");
    strcpy(equipoBase->tipoArmadura,"Botas");
    strcpy(equipoBase->stats->nombre,"Botas de guerrero");
    equipoBase->stats->HPMAX = 5;
    equipoBase->stats->DEF = 2;
    strcpy(equipoBase->stats->descripcion,"Unas Botas basicas, lo sufucientemente robustas para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);


    strcpy(equipoBase->tipo,"Arma");
    strcpy(equipoBase->stats->nombre,"Espada larga");
    equipoBase->stats->ATK = 5;
    strcpy(equipoBase->stats->descripcion,"Un casco basico, lo sufucientemente robusto para defenderte");
    equipoBase->equipado = true;


    pushBack(usuario->inventario,equipoBase);
    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);

    insertMap(usuario->equipamiento,equipoBase->tipoArmadura,equipoBase);*/

}






/*fin de chantarle equipamiento a las clases*/


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