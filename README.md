# Proyecto "RPGG"
IMPORTANTE: LA APLICACIÓN SOLO COMPILA Y HACE TODOS LOS PROCESOS EN WINDOWS

En el siguiente archivo README, se especificará: 

1. Cómo compilar y ejecutar la aplicación:
   
   LA SIGUIETE APLICACIÓN SE PODRÁ EJECUTAR SOLO EN DISPOSITIVOS WINDOWS

   RECOMENDAMOS REALIZAR EL EJECUTABLE EN LA TERMINAL (gcc *.c -o NOMBREQUEDESEA)

   Y LUEGO PUEDE ABRIR EL EJECTUABLE DESDE LA TERMINAL (.\\NOMBREQUEDESEA) O EN LA DIRECCIÓN QUE GUARDO LA CARPETA. 

   RECOMENDAACIÓN: AL MOMENTO DE ABRIR EL EJECUTABLE SE RECOMIENDA REDUCIR EL ZOOM DE ESTE, DADO A TEMAS DE ESPACIO QUE PUEDEN GENERAR PROBLEMAS AL MOMENTO DE MOSTRAR LA APLICACIÓN.

   
2. Las funciones que se ejecutan correctamente y los posibles errores
 
   TODAS LAS FUNCIONES RELACIONADAS A CREACIÓN DE PERSONAJE SE EJECUTAN CORRECTAMENTE
   
   LAS FUNCIONES QUE PODRÍAN INDICAR UN PROBLEMA SON:
   
   -Al momento de ingresar al submenú y escoge la opción de reiniciar, es posible que el programa presente fallas y no tenemos una explicación exacta del error.
   -Hay ocasiones que no seleccionamos nada pero se realiza la funcion al momento de ingresar al submenu.
   -En la funcion fase de batalla, cuando se elimina un enemigo, o se escapa, si bien la posicion queda en blanco, el enemigo seguira estado ahi.
   
   
3. Explicación en relación a como se desarrolla la aplicación
   Al momento de iniciar la ejecución del programa el jugador deberá utilizar las flechas de arriba, abajo para indicar la acción que desea realizar (iniciar partida o salir de la         aplicación) una vez elegida la acción que desea realizar, se presiona el botón escape (esc) para seleccionar dicha acción.

   SI EL JUGADOR REALIZA LA ACCIÓN DE INICIAR PARTIDA:
   -Debe ingresar el nombre que desea agregar al personaje y luego presionar tecla enter.
   -Luego, el programa solicitara elegir una de las 4 clases existentes(espadachín, mago, ladrón y/o chef) y el usuario deberá ingresar el número desde el 1 al 4 dependiendo de clase       elegida, finalmente presiona enter y comienza la partida.


   UNA VEZ INICIALIZADA LA PARTIDA EL JUGADOR PODRÁ DESPLAZARSE LIBREMENTE POR EL MAPA ENCONTRANDO TESOROS($) Y ENEMIGOS(@). El desplazamiento se realiza con las teclas que contienen flechas generadndo moviemientos hacía la derecha, izquierda, arriba y abajo.
   El juego desarrolla batallas en las cuales el jugador deberá enfrentarse.


   RECORDAMOS QUE EXISTEN 4 ESTADOS:
   
   1. VIVO: El jugador se puede desplazar libremente por el mapa
   2. DEAD: El juego finaliza ya que se perdio una batalla
   3. RESET: El juego se reinicia con el mismo personaje y las estádisticas desde cero.
   4. WIN: El jugador ha recorrido una cierta cantidad de mapas y se denomina como ganador, se cierra la aplicación. 
   
  
   
   
   

En uno de los mapas pensará que no hay salida, pero lo hay, solo hay que tener un buen ojo y buscar. SUERTE!!!
Secreto: Hay un exploit.
