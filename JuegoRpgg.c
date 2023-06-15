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
    int HP;
    int ATK;
    int DEF;
    char *descripcion;
}Info;//Hace referencia a las estadisiticas

typedef struct{

    char *tipo; //si es arma, armadura, consumible
    Info *stats;

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
    List *inventario;
    Opcion ataques[4];
    HashMap *equipamiento;
}Jugador;

typedef struct{
    coordenadas pos;
    int largo;
    int ancho;
    
}sala;
//prototipos de ubicaciones
void gotoxy(int x, int y);
void pantallainesesariadecarga();
void mainmenu();

//protoripo de funciones
void CrearPerfil(List *lista);
void selccionarclase(Jugador *usuario);
void estadisticasDeclase(Jugador *usuario);
void inventarionuevo(Jugador *usuario);
void OpcionesBatalla(Jugador *usuario);
void limpiarpantalla();
void generarmapa();
void faseDElanzamiento(List *lista);
//funciones solo developers (fran)
void mostrar_perfiles (List *lista);
void Submenu();
//main
int main(){

    pantallainesesariadecarga();
    mainmenu();
    List *listajugadores = createList();
    CrearPerfil(listajugadores);
    //mostrar_perfiles(listajugadores);
    limpiarpantalla();
    pantallainesesariadecarga();
    limpiarpantalla();
    
    generarmapa();
    faseDElanzamiento(listajugadores);
    
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
    gotoxy(106,3); printf("O Stats                O Inventario");
    gotoxy(106,4); printf("O salir                O Ajustes");
}

//△
void faseDElanzamiento(List *listaJugadores){
    Jugador *mainPlayer = firstList(listaJugadores);
    
    while(true)
    {
        Sleep(100);
        printf("\033[0;35m");
        //Moverse a la izquierda
        if((GetAsyncKeyState(0x25))&&(mainPlayer->pos.x>=2))
        {
            gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
            mainPlayer->pos.x--;
            gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
        }
        //derecha
        if((GetAsyncKeyState(0x27))&&(mainPlayer->pos.x<=99))
        {
            gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf(" ");
            mainPlayer->pos.x++;
            gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
        }

        //abajo
        if((GetAsyncKeyState(0x28)) && (mainPlayer->pos.y<=38))
        {
            gotoxy(mainPlayer->pos.x,mainPlayer->pos.y);printf(" ");
            mainPlayer->pos.y++;
            gotoxy(mainPlayer->pos.x,mainPlayer->pos.y); printf("O");
        }
        //arriba
        if((GetAsyncKeyState(0x26)) && (mainPlayer->pos.y>=2))
        {
            gotoxy(mainPlayer->pos.x,mainPlayer->pos.y) ;printf(" ");
            mainPlayer->pos.y--;
            gotoxy(mainPlayer->pos.x,mainPlayer->pos.y) ;printf("O");
        }

        if(GetAsyncKeyState(0x1B)){
            Submenu();
        }
    }
}


void generarmapa()
{
    printf("\033[0;32m");
    gotoxy(0,0);printf("-----------------------------------------------------------------------------------------------------");
    gotoxy(0,40);printf("-----------------------------------------------------------------------------------------------------");
    //101 largo 40 ancho
    for(int i = 1; i< 40; i++)
    {
        gotoxy(0,i);printf("|                                                                                                    |");
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
    printf("  -Puntos de vida = %i", usuario -> datos ->HP);
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
                printf("Hora de comenzar una nueva aventura\n");
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
    printf("\ningresa nombre\n");
    
    scanf("%99[^\n]s",usuario->datos->nombre);
    getchar();
    selccionarclase(usuario);
    estadisticasDeclase(usuario);
    inventarionuevo(usuario);
    OpcionesBatalla(usuario);
    usuario ->pos.x=10;
    usuario ->pos.y=10;

    //faltan el quipamiento 
    pushBack(lista ,usuario);
}

void OpcionesBatalla(Jugador *usuario){
    strcpy(usuario->ataques[0].nombre,"ATACAR");
    strcpy(usuario->ataques[1].nombre,"DEFENDER");
    strcpy(usuario->ataques[2].nombre,"ITEMS");
    strcpy(usuario->ataques[3].nombre,"HUIR");
    
    strcpy(usuario->ataques[0].descripicion,"Utiliza tu arma para poder hacerle daño al enemigo");
    strcpy(usuario->ataques[1].descripicion,"Si nesesitas Pensar o no pudes hacer nada, Defiendete para recibir menos daño");
    strcpy(usuario->ataques[2].descripicion,"Juntaste algun objeto que puedas usar?, que esperas?, solo hay una forma de saber que hacer");
    strcpy(usuario->ataques[3].descripicion,"Ya no te queda de otra? :(, HUYE ANTES QUE TE MATEN, o te da flojera pelear, vete");

}

void inventarionuevo(Jugador *usuario){
    usuario ->inventario = createList();

}

void estadisticasDeclase(Jugador *usuario){
    if(strcmp("Espadachin",usuario->clase)==0 || strcmp("espadachin",usuario->clase) == 0){
        //estadisticas Espadachin
        usuario ->datos->ATK= 20;
        usuario -> datos ->DEF = 10;
        usuario -> datos ->HP = 10;
        usuario ->nivel = 1;

    }else if(strcmp("Mago",usuario->clase)==0 || strcmp("mago",usuario->clase) == 0){
        //estadisticas Mago
        usuario ->datos->ATK= 10;
        usuario -> datos ->DEF = 15;
        usuario -> datos ->HP = 15;
        usuario ->nivel = 1;
    }else if(strcmp("Ladron",usuario->clase)==0 || strcmp("ladron",usuario->clase) == 0){
        //estadisticas Ladron
        usuario ->datos->ATK= 15;
        usuario -> datos ->DEF = 20;
        usuario -> datos ->HP = 10;
        usuario ->nivel = 1;
    }else{
        //estadisticas Chef
        usuario ->datos->ATK= 10;
        usuario -> datos ->DEF = 15;
        usuario -> datos ->HP = 20;
        usuario ->nivel = 1;
    }
}
void selccionarclase(Jugador *usuario){
    int clase;
    printf("ingresa la clase a eleccion\n1.-Espadachin\n2.-Mago\n3.-Ladron\n4.-Chef\n");
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