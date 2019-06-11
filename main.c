//Librerias Necesarias
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <stdlib.h>
#include <time.h>
#include <pspctrl.h>

//Librerias Multimedia
#include <png.h>
#include <pspgu.h> 
#include <pspaudio.h>
#include <pspaudiolib.h>
#include "lib_multimedia/mp3player.h"
#include "lib_multimedia/multimedia.h"

#define V 30
#define H 67
#define N 1816

int puntos = 0;
int velocidad = 0;
char ruta = 'n';

PSP_MODULE_INFO("Snake", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common) {
          sceKernelExitGame();
          return 0;
 }

/* Llamada thread */
int CallbackThread(SceSize args, void *argp) {
          int cbid;

          cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
          sceKernelRegisterExitCallback(cbid);

          sceKernelSleepThreadCB();

          return 0;
}

/* Configura llamada thread y vuelve a su thread id */
int SetupCallbacks(void) {
          int thid = 0;

          thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
          if(thid >= 0) {
                    sceKernelStartThread(thid, 0, 0);
          }

          return thid;
}

typedef struct{
	int x,y;
	int ModX,ModY;
	char imagen;
}snk;

typedef struct{
	int x,y;
}frt;

snk snake[N];
frt fruta;

char pantallaInicio[50][100] =
{
"                                                             ",
"    B B           CCCC CC   C CCCC C C   CCCC                ",
"     B            C    C C  C C  C CCC   C                   ",
"     B            C    C  C C CCCC C  C  CCC                 ",
"   AAAAA          C    C   CC C  C C   C C                   ",
"  A AAA A     CCCCC    C   CC C  C C   C CCCC                ",
" AAAAAAAAA                                                   ",
" AAAAAAAAA                                                   ",
"  AAAAAAA           AAAAAAAAAAA            AAAAAAAAAAAAA     ",
"  AAAAAAA          AAAAAAAAAAAAAA         AAAAAAAAAAAAAAA    ",
"   AAAAAAA        AAAAAAAAAAAAAAAA       AAAAAAAAAAAAAAA     ",
"    AAAAAAA      AAAAAA     AAAAAAA     AAAAAAA              ",
"     AAAAAAA    AAAAAA       AAAAAAA   AAAAAA                ",
"      AAAAAAAAAAAAAAA         AAAAAAAAAAAAAA                 ",
"       AAAAAAAAAAAAA           AAAAAAAAAAAA                  ",
"        AAAAAAAAAAA             AAAAAAAAAA                   ",
"                                                             ",
};

void menu_inicial();
void iniciarElementos();
void inicio(int *tam, char campo[V][H]);
void Intro_Campo(char campo[V][H]);
void Intro_Datos(char campo[V][H], int tam);
void loop(char campo[V][H], int tam);
void draw (char campo[V][H]);
void input (char campo[V][H], int *tam, int *muerto);
void update (char campo[V][H], int tam);
void Intro_Datos2(char campo[V][H], int tam);
void dibujarMenuSalir();
void PAUSAR();
int menuSalir();
int main();

int main(){
    pspDebugScreenInit();
    SetupCallbacks();

    iniciarElementos();

    sceKernelSleepThread(); //detiene el programa solo se sale con home y no se regrese al menu al terminar el porgrama
    return 0;
}

void iniciarElementos(){
	int tam, respuesta;
	char campo[V][H];
	menuInicial();
	
	if (ruta == 'n')
	{
		ruta = 's';
		IniciarSonido();
  		MP3_Init(1);
  		MP3_Load("menu/audio_juego.mp3");
	}
	MP3_Play();
	do{
		inicio(&tam,campo);//Iniciar todos nuestros elementos
		loop(campo,tam);
	
		pspDebugScreenClear();//Función para limpiar pantalla
		respuesta = menuSalir();
	}while(respuesta == 1);
	main();
}
void menuInicial(){

    int i, j, salir = 0;
    SceCtrlData pad;

    //Dibujar el menú principal
	pspDebugScreenSetXY(25,27);
	pspDebugScreenSetTextColor(0xFFFFFF);
    pspDebugScreenPrintf("ELEGIR DIFICULTAD");
    pspDebugScreenSetXY(6,30);
    pspDebugScreenSetTextColor(0xFFFFFF);
    pspDebugScreenPrintf("[%c] FACIL", 216);
    pspDebugScreenSetXY(28,30);
    pspDebugScreenSetTextColor(0xFFFFFF);
    pspDebugScreenPrintf("[X] NORMAL");
    pspDebugScreenSetXY(50,30);
    pspDebugScreenSetTextColor(0xFFFFFF);
    pspDebugScreenPrintf("[O] DIFICIL");

	for(i = 0 ; i < 17 ; i++){
        for(j = 0 ; j < 71 ; j++){
            if(pantallaInicio[i][j] == 'A'){
                pspDebugScreenSetXY(j + 5, i + 9);
                pspDebugScreenSetTextColor(0x40FF00);
                pspDebugScreenPrintf("*");
            }
            else if(pantallaInicio[i][j] == 'B'){
            	pspDebugScreenSetXY(j + 5, i + 9);
                pspDebugScreenSetTextColor(0x0000FF);
                pspDebugScreenPrintf("*");
            }
            else if(pantallaInicio[i][j] == 'C'){
            	pspDebugScreenSetXY(j + 5, i + 9);
                pspDebugScreenSetTextColor(0x00FFFF);
                pspDebugScreenPrintf("*");
            }
        }
    }

    do{
         sceCtrlReadBufferPositive(&pad, 1);

         if (pad.Buttons & PSP_CTRL_TRIANGLE){
         	velocidad = 15;
         	salir = 1;
         }
         else if (pad.Buttons & PSP_CTRL_CROSS){
         	velocidad = 7;
         	salir = 1;
         }
         else if (pad.Buttons & PSP_CTRL_CIRCLE){
         	velocidad = 0;
         	salir = 1;
         }
    }while(!salir);

    for(i = 0 ; i< 17 ; i++){
        for(j = 0 ; j < 71 ; j++){
            if(pantallaInicio[i][j] == 'A'){
                pspDebugScreenSetXY(j+5,i+9); 
                pspDebugScreenPrintf(" ");
            }
        }
    }
}

//Iniciar todos nuestros elementos
void inicio(int *tam, char campo[V][H]){
	int i;
	//La cabeza de nuestra serpiente
	snake[0].x = 32;
	snake[0].y = 10;
	
	//Tamaño de la serpiente
	*tam = 4;
	
	//Coordenadas Fruta
	srand(time(NULL));
	
	fruta.x = rand() % (H - 1);
	fruta.y = rand() % (V - 1);
	
	while(fruta.x == 0){
		fruta.x = rand() % (H - 1);
	}
	
	while(fruta.y == 0){
		fruta.y = rand() % (V - 1);
	}
	
	for (i = 0; i < *tam; i++){
		snake[i].ModX = 1;
		snake[i].ModY = 0;
	}
	
	Intro_Campo(campo);
	Intro_Datos(campo,*tam);
}

//Creacion del campo de juego
void Intro_Campo(char campo[V][H]){
	int i, j;
	
	pspDebugScreenSetXY(2,1);
	pspDebugScreenSetTextColor(0xFFFFFF);
	pspDebugScreenPrintf("PUNTUACION");
	pspDebugScreenSetXY(15,1);
	pspDebugScreenPrintf("%i", puntos);

	pspDebugScreenSetXY(1,32);
	pspDebugScreenSetTextColor(0xFFFFFF);
	pspDebugScreenPrintf("[HOME] SALIR");
	pspDebugScreenSetXY(53,32);
	pspDebugScreenSetTextColor(0xFFFFFF);
	pspDebugScreenPrintf("[START] PAUSAR");
	
	// Dinuja el campo de juego
	for (i = 0 ; i < V; i++)
	{
        for (j = 0; j < H; j++)
		{
            if (i == 0 || i == V - 1)
			{
                campo [i][j] = '-';
            }
            else if (j == 0 || j == H - 1)
			{
                campo [i][j] = '|';
            }
            else
			{
                campo [i][j] = ' ';
            }
        }
    }
}

//Metera todos los datos en la matriz campo
void Intro_Datos(char campo[V][H], int tam){
	int i;
	
	//Dibuja el cuerpo de la serpiente
	for (i = 1; i < tam; i++)
	{
		snake[i].x = snake[i - 1].x - 1;
		snake[i].y = snake[i - 1].y;
		
		snake[i].imagen = 219;
	}
	
	snake[1].imagen = 8;
	snake[0].imagen = 207;
	
	for(i = 0; i < tam; i++)
	{
		campo[snake[i].y][snake[i].x] = snake[i].imagen;
	}
	campo[fruta.y][fruta.x] = 233;
}

void loop(char campo[V][H], int tam){
	int muerto;
	float time;
	time = 0.000;
	muerto = 0;

	do{
		if (!time)
		{
			pspDebugScreenSetXY(0,2);
			draw (campo);
			input(campo,&tam,&muerto);
			update(campo,tam);
		}
		time += 0.001;
		if (time > velocidad)
		{
			time = 0.000;
		}
	}while (muerto == 0);
}

//Funcion para dibuar por pantalla todo lo que hay en la matriz campo
void draw (char campo[V][H])
{
	int i, j;
	/*
		Los números a los que están igualados las siguientes variables
		equivalen a un caractér en la tabla de valores ASCCI del PSP
	*/
	char snakeBody = 219;
	char snakeHead = 8;
	char fruit = 233;

	char Tongue1 = 205;
	char Tongue2 = 206;
	char Tongue3 = 207;
	char Tongue4 = 208;

	for (i = 0; i < V; i++)
	{
		for (j = 0; j < H; j++)
		{
			if (campo[i][j] == snakeBody || campo[i][j] == snakeHead || campo[i][j] == Tongue1 || campo[i][j] == Tongue2 || campo[i][j] == Tongue3 || campo[i][j] == Tongue4)
			{
				pspDebugScreenSetTextColor(0x31B404);
				pspDebugScreenPrintf("%c",campo[i][j]);
			}
			else if (campo[i][j] == fruit)
			{
				pspDebugScreenSetTextColor(0x00FFFF);
				pspDebugScreenPrintf("%c",campo[i][j]);
			}
			else
			{
				pspDebugScreenSetTextColor(0xFFFFFF);
				pspDebugScreenPrintf("%c",campo[i][j]);
			}
		}
		pspDebugScreenPrintf("\n");
	}
}

void input (char campo[V][H], int *tam, int *muerto){
	int i = 1;
	SceCtrlData pad;
	
	//Comprobacion de si hemos muerto
	if(snake[0].x == 0 || snake[0].x == H - 1 || snake[0].y == 0 || snake[0].y == V - 1)
	{
		*muerto = 1;
	}
	
	while(i < *tam){
		if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			*muerto = 1;
		}
		i++;
	}
	
	//comprobar si nos hemos comido la fruta y aumenta el puntaje
	if(snake[0].x == fruta.x && snake[0].y == fruta.y){
		*tam += 1;
		puntos += 10;
		
		snake [*tam - 1].imagen = 219;
		
		fruta.x = rand() % (H - 1);
		fruta.y = rand() % (V - 1);
		
		while(fruta.x == 0){
			fruta.x = rand() % (H - 1);
		}
		
		while(fruta.y == 0){
			fruta.y = rand() % (V - 1);
		}
	}
	
	if(*muerto == 0){
		sceCtrlReadBufferPositive(&pad,1);

		//Controla los movimientos de la serpiente en el campo de juego
		if (pad.Buttons & PSP_CTRL_UP && snake[0].y == snake[1].y){
			snake[0].imagen = 206;
			snake[0].ModX = 0;
			snake[0].ModY = -1;
		}
		else if (pad.Buttons & PSP_CTRL_DOWN && snake[0].y == snake[1].y){
			snake[0].imagen = 205;
			snake[0].ModX = 0;
			snake[0].ModY = 1;
		}
		else if(pad.Buttons & PSP_CTRL_RIGHT && snake[0].x == snake[1].x){
			snake[0].imagen = 207;
			snake[0].ModX = 1;
			snake[0].ModY = 0;
		}
		else if (pad.Buttons & PSP_CTRL_LEFT && snake[0].x == snake[1].x){
			snake[0].imagen = 208;
			snake[0].ModX = -1;
			snake[0].ModY = 0;
		}
		else if(pad.Buttons & PSP_CTRL_START)
			for (i = 0; i <= 5; i++)
			{
				PAUSAR();
			}
		}
}

void update (char campo[V][H], int tam){
	//Borrar todos los datos de la matriz
	Intro_Campo(campo);
	
	//Introducir los nuevos datos
	Intro_Datos2(campo,tam);
}

void Intro_Datos2(char campo[V][H], int tam){
	int i;
	//Crear la persecucion de los elementos del cuerpo
	
	for (i = tam-1 ; i > 0; i--){
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	
	snake[0].x += snake[0].ModX;
	snake[0].y += snake[0].ModY;
	
	for(i = 0; i < tam; i++){
		campo[snake[i].y][snake[i].x] = snake[i].imagen;
	}
	
	campo[fruta.y][fruta.x] = 233;
}

//Regresar valor para continuar o salir
int menuSalir(){
	int salir = 0;
	int teclaPres, x, y;
	x = 18;
	y = 14;
	SceCtrlData pad;
	
	teclaPres = 1;

	MP3_Stop();

	pspDebugScreenSetXY(x,y);
	pspDebugScreenSetTextColor(0xFFFFFF);
	//Se dibujan las flechas de selección del menu salir
	pspDebugScreenPrintf("%c%c%c%c%c%c", 207,207,207,207,207,207);
	dibujarMenuSalir();
	
	//Detectamos que tecla se a presionado
	while (teclaPres == 1){
		sceCtrlReadBufferPositive(&pad,1);

		if (pad.Buttons & PSP_CTRL_UP){
			x = 18;
			y = 14;
			pspDebugScreenClear();
			pspDebugScreenSetXY(x,y);
			pspDebugScreenSetTextColor(0xFFFFFF);
			pspDebugScreenPrintf("%c%c%c%c%c%c", 207,207,207,207,207,207);
			dibujarMenuSalir();
		}
		else if (pad.Buttons & PSP_CTRL_DOWN){
			x = 18;
			y = 17;
			pspDebugScreenClear();
			pspDebugScreenSetXY(x,y);
			pspDebugScreenSetTextColor(0xFFFFFF);
			pspDebugScreenPrintf("%c%c%c%c%c%c", 207,207,207,207,207,207);
			dibujarMenuSalir();
		}
		else if (pad.Buttons & PSP_CTRL_CROSS){
			if (x == 18 && y == 14){
				puntos = 0;
				teclaPres = 0;
				MP3_Pause();
				return 1;
			}
			else if(x == 18 && y == 17){
				puntos = 0;
				teclaPres = 0;

				SceCtrlData pad;
    			
    			pspDebugScreenSetXY(19,23); 
    			pspDebugScreenSetTextColor(0xFFFFFF);
    			pspDebugScreenPrintf("PRESIONA [START] PARA CONFIRMAR");
    			do{
        			sceCtrlReadBufferPositive(&pad, 1);
        			if (pad.Buttons & PSP_CTRL_START){
        				salir = 1;
        			}
    			}while(!salir);

				return 0;
			}
		}
	}
}

void dibujarMenuSalir(){
	pspDebugScreenSetXY(29,7);
	pspDebugScreenSetTextColor(0xFFFFFF);
	pspDebugScreenPrintf("%cHas Perdido!", 173);
	pspDebugScreenSetXY(27,14);
	pspDebugScreenSetTextColor(0xFFFFFF);
	pspDebugScreenPrintf("Continuar Jugando");
	pspDebugScreenSetXY(27,17);
	pspDebugScreenSetTextColor(0xFFFFFF);
	pspDebugScreenPrintf("Regresar al menu");
}

void PAUSAR(){
	SceCtrlData pad;
    int salir = 0;

    pspDebugScreenSetXY(29,15);
    pspDebugScreenSetTextColor(0xFFFFFF);
    pspDebugScreenPrintf("** PAUSE **");
    do{
    	sceCtrlReadBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_START){
        	salir = 1;
        }
    }while(!salir);
}