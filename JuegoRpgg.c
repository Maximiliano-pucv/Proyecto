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
    coordenadas pos;
    Info *datos;
    char *clase;
    int nivel;
    List *inventario;
    List *ataques;
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
void limpiarpantalla();

//funciones solo developers (fran)
void mostrar_perfiles (List *lista);
//main
int main(){

    pantallainesesariadecarga();
    mainmenu();
    List *listajugadores;
    CrearPerfil(listajugadores);
    //mostrar_perfiles(listajugadores);
    limpiarpantalla();
    pantallainesesariadecarga();

    return 0;
}

void limpiarpantalla(){
    for(int i=0; i<=64; i++){
        gotoxy(0,i);printf("                                                                                                           ");
    }
    /*for(int i=0; i<=80; i++){
        gotoxy(i,17);printf("-");
    }*/
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
    printf("  -Nivel = %i",usuario->nivel);
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
            printf("xd");
            printf("%i",seleccion);
            if(seleccion == 1){
              printf("aca comienza el juego\n");
              Beep(16,3);
              return;
            } 
            else if(seleccion ==2){
                return;
            }
        }
        if(GetAsyncKeyState(0x26)){
            gotoxy(60,35) ;printf("o Salir del Juego");
            gotoxy(60,34); printf("+ Nueva Partida");
            seleccion=1;
        }
        if(GetAsyncKeyState(0x28)){
            gotoxy(60,34) ;printf("o Nueva Partida");
            gotoxy(60 ,35) ;printf("+ Salir del Juego");
            seleccion=2;
        }

        
    }
}

void CrearPerfil(List *lista){
    Jugador *usuario = malloc(sizeof(Jugador));
    usuario ->datos = malloc(sizeof(Info));
    printf("ingresa nombre\n");
    
    scanf("%99[^\n]s",usuario ->datos->nombre);
    getchar();
    selccionarclase(usuario);
    estadisticasDeclase(usuario);

    //faltan el quipamiento , el inventario y los 4 ataques
    pushBack(lista,usuario);
}
void estadisticasDeclase(Jugador *usuario){
    if(strcmp("Espadachin",usuario->clase)==0){
        //estadisticas Espadachin
        usuario ->datos->ATK= 20;
        usuario -> datos ->DEF = 10;
        usuario -> datos ->HP = 10;
        usuario ->nivel = 1;

    }else if(strcmp("Mago",usuario->clase)==0){
        //estadisticas Mago
        usuario ->datos->ATK= 10;
        usuario -> datos ->DEF = 15;
        usuario -> datos ->HP = 15;
        usuario ->nivel = 1;
    }else if(strcmp("Ladron",usuario->clase)==0){
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
    printf("HELLO WORLD LEt'S GO");
    int i=0;
    char porciento ='%';
    while (true)
    {
        /* code */
        Sleep(100);
        gotoxy(50, 11);printf("Cargando ...%i%c",i,porciento);
        i++;
        if(i>100) break;
    }
}