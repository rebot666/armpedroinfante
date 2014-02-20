#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "Escena.h"
#include "Interaccion.h"
#include <Windows.h>
#include <NuiApi.h>
#include <iostream>
#include "Gestures.h"
//mod
#include "globales.h"

//Deshabilita la consola  (exclusivo para Windows - Visual Studio)
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//Variables globales
void juegoActivo();
void juegomenu();
void creaEscena();
void juego_activo_idle();
void juego_menu_idle();
void juego_win_idle();
void juego_pause_idle();
void juego_lose_idle();
Escena *escena = new Escena();  //Escena actual
int currWidth,currHeight;       //Ancho y alto de la ventana grafica
GLfloat dtran=DELTA_TRAN;       //Delta de traslacion interactiva
GLfloat drot=DELTA_ROT;         //Delta de rotacion interactiva
bool leftMouse=false;           //Boton izquierdo oprimido
bool rightMouse=false;          //Boton derecho oprimido
bool middleMouse=false;         //Boton central oprimido
int lastMouseX=0;		        //Posicion previa de mouse en X
int lastMouseY=0;               //Posicion previa de mouse en Y
GLfloat x = .4;
GLfloat y = -.4;
Objeto *barco1;
Objeto *cielo1;
Objeto *mar1;
Objeto *avion1;
Objeto *helice1;
Objeto *avion2;
Objeto *helice2;
Objeto *barco2;
Objeto *mar2;
Objeto *window1;
Objeto *avion3;
Objeto *indicadorAltActual;
Objeto *estatusGestura;
//mods
Objeto *menu;
Objeto *mouse, *play, *loader;
GLfloat pausalfa = 0.6;
Objeto *cuerpo1;
bool startMenu = true;
Objeto *windowInit;
Objeto *PressStart; //es el canvas del título inicial
NUI_SKELETON_FRAME ourframe;
bool cargando_juego;
float der;
float izq;
bool terminado = false;

void pausa(){
    x = 0;
    y = 0;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	escena->ilumina();
	escena->dibujaEscena();
	glutSwapBuffers();	
}


int init(int left,int top,int width,int height,string title) {
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	if(!(NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX) == S_OK)){
		return -1;
	}
	glutInitWindowPosition(left,top);
	glutInitWindowSize(width,height);
	glutCreateWindow(title.c_str());
	glClearColor(0.0,0.0,0.0,0.0);			
	cargando_juego = false;
	return 0;
}

void redimensiona(int width,int height) {
  currWidth=width;
  currHeight=height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();    
  gluPerspective(activa->fovy,(GLdouble)width/(GLdouble)height,activa->nearCam,activa->farCam);
  glTranslatef(activa->xview,activa->yview,activa->zview); 
  glRotatef(activa->xrot,1.0,0.0,0.0);
  glRotatef(activa->yrot,0.0,1.0,0.0);
  glRotatef(activa->zrot,0.0,0.0,1.0);
  glViewport(0,0,width,height);
  glMatrixMode(GL_MODELVIEW);
}


void defineLuces()
{
    escena->defineLuz("light0",'D', 1.0, 1.0, 1.0);
	escena->posicionLuz("light0", 0.0, 3.0, 1.0);
	
    escena->defineLuz("light1",'E', 1.0, 1.0, 1.0);
	escena->posicionLuz("light1", 1.0, -2.0, 1.0);
	
	escena->defineLuz("light2",'S', 1.0, 1.0, 1.0);
	escena->posicionLuz("light2", 0.0, 8.0, 0.0);
	escena->direccionLuz("light2", 0.0, -1.0, 0.0);
	escena->anguloLuz("light2", 30.0);
}


void juegoActivo()
{
	escena->ocultaEscena();
	escena->agregaObjeto("window1", (Objeto *) new Viewport(1));
    window1 = escena->objetos["window1"];
	window1->escalaX = 0.9;
	window1->agregaDescendiente("barco1", (Objeto *) new Modelo("barco_final"), escena);
	window1->agregaDescendiente("gestura", (Objeto *) new Canvas(1.0, 1.0), escena);
	estatusGestura = window1->descendientes["gestura"];
	estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	"torre.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
	estatusGestura->posX = 3.55;
	estatusGestura->posY = 1.52;
	estatusGestura->posZ = 3.0;
	//mod
	//window1->agregaDescendiente("menu1", (Objeto *) new Canvas(9, pausalfa), escena);

	switch(escena->juego->no_vidas){
	case 1:
		//mod
		//window1->agregaDescendiente("avion1", (Objeto *) new Modelo("Avion1new"), escena);
		window1->agregaDescendiente("avion1", (Objeto *) new Modelo("CuerpoAvion1"), escena);
		window1->agregaDescendiente("vida1", (Objeto *) new Modelo("CuerpoAvion1"), escena);
		//window1->descendientes["avion1"]->agregaDescendiente("helice1", (Objeto *) new Modelo("HelicesAvion1"), escena);
		window1->descendientes["vida1"]->escalaUniforme(0.35);

        window1->descendientes["vida1"]->posX = -8;
        window1->descendientes["vida1"]->posY = 4;
        window1->descendientes["vida1"]->rotX= 20;
        window1->descendientes["vida1"]->velRotY= 1;
            
        escena->modificaMaterial("avion1", "lambert2SG", "MetalAvion1.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
        escena->modificaMaterial("vida1", "lambert2SG", "MetalAvion1.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
    
		break;
	case 2:
		window1->agregaDescendiente("avion1", (Objeto *) new Modelo("CuerpoAvion2"), escena);
		window1->agregaDescendiente("vida1", (Objeto *) new Modelo("CuerpoAvion1"), escena);
		window1->agregaDescendiente("vida2", (Objeto *) new Modelo("CuerpoAvion2"), escena);
		window1->descendientes["vida1"]->escalaUniforme(0.35);
		window1->descendientes["vida2"]->escalaUniforme(0.35);

        window1->descendientes["vida1"]->posX = -8;
        window1->descendientes["vida1"]->posY = 4;
        window1->descendientes["vida1"]->rotX= 20;
        window1->descendientes["vida1"]->velRotY= 1;
            
        window1->descendientes["vida2"]->posX = -7;
        window1->descendientes["vida2"]->posY = 4;
        window1->descendientes["vida2"]->rotX= 20;
        window1->descendientes["vida2"]->velRotY= 1;
            
        escena->modificaMaterial("avion1", "lambert2SG", "MetalAvion2.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
        escena->modificaMaterial("vida1", "lambert2SG", "MetalAvion1.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
        escena->modificaMaterial("vida2", "lambert2SG", "MetalAvion2.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
    
		break;
	case 3:
		window1->agregaDescendiente("avion1", (Objeto *) new Modelo("CuerpoAvion3"), escena);
		window1->agregaDescendiente("vida1", (Objeto *) new Modelo("CuerpoAvion1"), escena);
		window1->agregaDescendiente("vida2", (Objeto *) new Modelo("CuerpoAvion2"), escena);
		window1->agregaDescendiente("vida3", (Objeto *) new Modelo("CuerpoAvion3"), escena);
		window1->descendientes["vida1"]->escalaUniforme(0.35);
		window1->descendientes["vida2"]->escalaUniforme(0.35);
		window1->descendientes["vida3"]->escalaUniforme(0.35);

		window1->descendientes["vida1"]->posX = -8;
		window1->descendientes["vida1"]->posY = 4;
		window1->descendientes["vida1"]->rotX= 20;
		window1->descendientes["vida1"]->velRotY= 1;

        window1->descendientes["vida2"]->posX = -7;
		window1->descendientes["vida2"]->posY = 4;
		window1->descendientes["vida2"]->rotX= 20;
		window1->descendientes["vida2"]->velRotY= 1;

        window1->descendientes["vida3"]->posX = -6;
		window1->descendientes["vida3"]->posY = 4;
		window1->descendientes["vida3"]->rotX= 20;
		window1->descendientes["vida3"]->velRotY= 1;
            
        escena->modificaMaterial("avion1", "lambert2SG", "MetalAvion3.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
        escena->modificaMaterial("vida1", "lambert2SG", "MetalAvion1.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
        escena->modificaMaterial("vida2", "lambert2SG", "MetalAvion2.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
        escena->modificaMaterial("vida3", "lambert2SG", "MetalAvion3.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
		break;
	}

	window1->descendientes["avion1"]->agregaDescendiente("helice1", (Objeto *) new Modelo("HelicesAvion2"), escena);
	helice1 = window1->descendientes["avion1"]->descendientes["helice1"];
	helice1->posY = -0.05;
	helice1->posZ = 1.0;
	helice1->velRotZ = 30.0;
	
	window1->agregaDescendiente("cielo1", (Objeto *) new Canvas(700, 1.0), escena);
	window1->agregaDescendiente("mar1", (Objeto *) new Canvas(600, 1.0), escena);


	//definici'on de objetos?
	barco1 = window1->descendientes["barco1"];
	//mod
	//menu = window1->descendientes["menu1"];
	avion1 = window1->descendientes["avion1"];
	
	//helice1 = avion1->descendientes["helice1"];
	

	//mod

	cielo1 = window1->descendientes["cielo1"];
	mar1 = window1->descendientes["mar1"];
	barco1->escalaUniforme(20.0);
	barco1->rotY = 90;
	barco1->rotX = -10;
	barco1->posZ = 15;

	//mod
	/*menu->posZ = 4.4;
	menu->posX = -0.5;
	menu->posY = -0.5;*/

	//barco1->posY = -1;
	//barco1->velZ = -1;
	//barco1->loopFrames = 30;
	//avion1->rotX = 5;
    avion1->rotX = 30;
	avion1->posY = 50;
    //avion1->posY = 50;
	//helice1->posY = 50;
	avion1->velZ = .4;
	//avion1->loopFrames = 500;
    avion1->posZ = -100;
	//helice1->posZ = -100;
	//avion1->posZ = -5;


	cielo1->posZ = -300;
	cielo1->posY = 300;
	cielo1->defineMaterial(1.0,1.0,1.0,1.0,
                        1.0,1.0,1.0,1.0,
                        0.0,0.0,0.0,1.0,
                        0.0,0.0,0.0,1.0,
                        "cielo1.tga",GL_REPEAT,0.0,0.0,0.0,0.0,0.0,0.0,GL_SPHERE_MAP,false);
	mar1->rotX = 90;
	mar1->posY = -10;
	mar1->defineMaterial(1.0,1.0,1.0,1.0,
                        1.0,1.0,1.0,1.0,
                        0.0,0.0,0.0,0.0,
                        0.0,0.0,0.0,0.0,
                        "agua.tga",GL_REPEAT,0.0,0.0,0.0,0.0,0.0,0.0,GL_OBJECT_LINEAR,false);
	

	//mod
	/*menu->defineMaterial(1.0, 1.0, 1.0,0.0,
			1.0, 1.0, 1.0,0.0,
			0.0, 0.0, 0.0,0.0,
			0.0, 0.0, 0.0,0.0,
			"menu.tga", GL_CLAMP, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);*/
	
    escena->barcos[0] = barco1;
    escena->aviones[0] = avion1;
    

	escena->agregaObjeto("window2", (Objeto *) new Viewport(2));
	Objeto *window2;
	window2 = escena->objetos["window2"];
	window2->escalaX = 4.0;
	window2->agregaDescendiente("barco2", (Objeto *) new Modelo("barco_final"), escena);
	switch(escena->juego->no_vidas){
        case 1:
            window2->agregaDescendiente("avion2", (Objeto *) new Modelo("CuerpoAvion1"), escena);
            escena->modificaMaterial("avion2", "lambert2SG", "MetalAvion1.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
            break;
        case 2:
            window2->agregaDescendiente("avion2", (Objeto *) new Modelo("CuerpoAvion2"), escena);
            escena->modificaMaterial("avion2", "lambert2SG", "MetalAvion2.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
            break;
        case 3:
            window2->agregaDescendiente("avion2", (Objeto *) new Modelo("CuerpoAvion3"), escena);
            escena->modificaMaterial("avion2", "lambert2SG", "MetalAvion3.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
            break;
	}

	window2->descendientes["avion2"]->agregaDescendiente("helice2", (Objeto *) new Modelo("HelicesAvion2"), escena);
	helice2 = window2->descendientes["avion2"]->descendientes["helice2"];
	helice2->posY = -0.05;
	helice2->posZ = 1.0;
	helice2->velRotZ = 30.0;


	window2->agregaDescendiente("mar2", (Objeto *) new Canvas(30, 1.0), escena);
	barco2 = window2->descendientes["barco2"];
	avion2 = window2->descendientes["avion2"];
	mar2 = window2->descendientes["mar2"];
	barco2->rotY = 90;
	barco2->rotX = 90;
	barco2->escalaUniforme(5.0);
	//barco2->posY = -10;
    barco2->posY = -7;
	//barco2->velY = 0.05;
	//barco2->loopFrames = 80;
	avion2->rotX = 90;
	avion2->escalaUniforme(0.5);
	//avion2->posY = 100;
    avion2->posY = 94;
	avion2->velY = -0.4;
	//avion2->loopFrames = 80;
	mar2->posZ = -3;
	mar2->defineMaterial(1.0,1.0,1.0,1.0,
                        1.0,1.0,1.0,1.0,
                        0.0,0.0,0.0,0.0,
                        0.0,0.0,0.0,0.0,
                        "mar.tga",GL_REPEAT,0.0,0.0,0.0,0.0,0.0,0.0,GL_OBJECT_LINEAR,false);
	//barco2->velY = -1.0;
	//barco2->loopFrames = 30;
    escena->barcos[1] = barco2;
    escena->aviones[1] = avion2;
    
	escena->agregaObjeto("window3", (Objeto *) new Viewport(3));
	Objeto *window3;

	window3 = escena->objetos["window3"];
	window3->escalaY = 3.0;
	switch(escena->juego->no_vidas){
		case 1:
			window3->agregaDescendiente("avion3", (Objeto *) new Modelo("CuerpoAvion1"), escena);
			escena->modificaMaterial("avion3", "lambert2SG", "MetalAvion1.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
			break;
		case 2:
			window3->agregaDescendiente("avion3", (Objeto *) new Modelo("CuerpoAvion2"), escena);
			escena->modificaMaterial("avion3", "lambert2SG", "MetalAvion2.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
			break;
		case 3:
			window3->agregaDescendiente("avion3", (Objeto *) new Modelo("CuerpoAvion3"), escena);
			escena->modificaMaterial("avion3", "lambert2SG", "MetalAvion3.tga", GL_REPEAT, 1.0,1.0,0.0,0.0);
			break;
	}
	window3->agregaDescendiente("cielo3", (Objeto *) new Canvas(40,1.0), escena);
    window3->agregaDescendiente("altitud", (Objeto *) new Modelo("VelAltCaras/BaseAltCaras"), escena);
    //window3->agregaDescendiente("velocidad", (Objeto *) new Modelo("VelAltCaras/BaseVelCaras"), escena);
	Objeto *cielo3;
	
    //Objeto *velocidad;
    Objeto *altitud;
	Objeto *helice3;
	//Objeto *indicadorVelocidad;
	Objeto *indicadorAltDeseada;
	
    
	avion3 = window3->descendientes["avion3"];
	avion3->agregaDescendiente("helice3", (Objeto *) new Modelo("HelicesAvion2"), escena);
	helice3 = avion3->descendientes["helice3"];
	cielo3 = window3->descendientes["cielo3"];
    //velocidad = window3->descendientes["velocidad"];
    altitud = window3->descendientes["altitud"];
	//escena->agregaObjeto("indicadorVel", (Objeto *) new Modelo("Vel/IndicadorVel"));
	//indicadorVelocidad = escena->objetos["indicadorVel"];
	altitud->agregaDescendiente("altActual", (Objeto *) new Modelo("Altitud/AltitudActual"),escena);
	indicadorAltActual = altitud->descendientes["altActual"];
	altitud->agregaDescendiente("altDeseada", (Objeto *) new Modelo("Altitud/AltitudDeseada"),escena);
	indicadorAltDeseada = altitud->descendientes["altDeseada"];

	//escena->modificaMaterial("leon", "lambert3SG", "leonTexture.tga",GL_CLAMP, 1.0);
	//escena->modificaMaterial("altitud", "labert5SG", "texturaAlt.tga", GL_CLAMP, 1.0);
	//escena->modificaMaterial("velocidad", "labert5SG", "texturaVel.tga", GL_CLAMP, 1.0);
	//ajustes de helice
	helice3->posZ = 1.0;
	helice3->posY = -0.05;
	helice3->velRotZ = 30.0;

	cielo3->defineMaterial(1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0,
		0.0, 0.0, 0.0,0.0,
		0.0, 0.0, 0.0,0.0,
		"cielo2.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_SPHERE_MAP, false);
	//avion3->rotY = 90;
	//avion3->rotX = 10;
	avion3->escalaUniforme(1.0);
	//avion3->velRotX = 1;
	//avion3->velRotZ = 1;
	//avion3->loopFrames = 10;
	//avion3->velY = 1;
	cielo3->posZ = -20;
    escena->aviones[2] = avion3;
	//avion3->loopFrames = 30;
	//barco3->velY = -1.0;
	//barco3->loopFrames = 30;
    //velocidad->posX = 5.5;
    //velocidad->posY = -.5;
    //velocidad->rotY = -30;
	//velocidad->rotX = -10;
	//indicadorVelocidad->posZ = 1.5;
	//indicadorVelocidad->posX = -0.3;
	//indicadorVelocidad->posY = -0.3;
	//indicadorVelocidad->velRotZ = 10;
	//indicadorVelocidad->posX = 4.3;
	//indicadorVelocidad->posY = -1.0;
	//indicadorVelocidad->loopFrames = 50;
	indicadorAltActual->posZ = 1.5;
	indicadorAltDeseada->posZ = 1.5;
	indicadorAltActual->posX = 2.6;
	indicadorAltDeseada->posX = 1.0;
	indicadorAltActual->rotY = 45.0;
	indicadorAltDeseada->rotY = 225.0; 
	indicadorAltActual->posY = 0.6;
	indicadorAltDeseada->posY = -0.3;
	indicadorAltActual->escalaUniforme(0.5);
	//velocidad->velRotX = 0.5;
    altitud->posX = -6;
}

void juegoMenu()
{
	//escena->ocultaEscena();
	escena->agregaObjeto("Menu", (Objeto *) new Canvas(5, 1.0));
	PressStart = escena->objetos["Menu"];
	PressStart->defineMaterial(1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	"titulo.tga", GL_CLAMP, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
	PressStart->posZ = 4.8;
	PressStart->posX = -0.5;
	PressStart->posY = -0.5;

	PressStart->agregaDescendiente("Mouse", (Objeto *) new Modelo("HUD/mouse"), escena);
	mouse = PressStart->descendientes["Mouse"];
	mouse -> posZ = 0;
	mouse -> posX = .5;
	mouse -> posY = .5;
	mouse -> escalaUniforme(.03);

	PressStart->agregaDescendiente("Play",(Objeto *) new Modelo("HUD/play"), escena);
	play = PressStart->descendientes["Play"];
	play -> escalaUniforme(.03);
	play -> posX = .77;
	play -> posY = .35;

	PressStart->agregaDescendiente("Loader",(Objeto *) new Modelo("HUD/loader"), escena);
	loader = PressStart->descendientes["Loader"];
	loader -> escalaUniforme(.02);
	loader -> posX = .77;
	loader -> posY = .40;
}

void juegoWin(){
	/*
    escena->ocultaEscena();
    escena->agregaObjeto("fin",(Objeto *) new Canvas(5, 1.0));
    escena->objetos["fin"]->defineMaterial(0.0,0.0,0.0,0.0,
                                               0.0,0.0,0.0,0.0,
                                               0.0,0.0,0.0,0.0,
                                               0.0,0.0,0.0,0.0,
                                               "titulo.tga",GL_CLAMP,1,0.0,0.0,0.0,0.0,0.0,GL_OBJECT_LINEAR,false);
	Objeto *fin;
	fin = escena->objetos["fin"];
	fin->posZ = 4.8;
	fin->posX = -0.5;
	fin->posY = -0.5;
	*/

	Objeto *windowWin;
	Objeto *win;
	escena->ocultaEscena();
	escena->agregaObjeto("windowWin", (Objeto *) new Viewport(4));
	windowWin = escena->objetos["windowWin"];
	windowWin->agregaDescendiente("win", (Objeto *) new Canvas(10, 1.0), escena);
	win = windowWin->descendientes["win"];
	win->posZ = 4.8;
	win->defineMaterial(0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		"exito.tga", GL_REPEAT, 1, 0.0, 0.0, 0.0, 0.0, 0.0, GL_EYE_LINEAR, false);
	/*win->posZ = 4.7;
	win->posX = -0.5;
	win->posY = -0.5;*/

}

void juegoLose(){
	Objeto *windowFin;
	Objeto *fin;
	escena->ocultaEscena();
	escena->agregaObjeto("windowFin", (Objeto *) new Viewport(4));
	windowFin = escena->objetos["windowFin"];
	windowFin->agregaDescendiente("finale", (Objeto *) new Canvas(10, 1.0), escena);
	fin = windowFin->descendientes["finale"];
	fin->posZ = 4.8;
	fin->defineMaterial(0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		"fin.tga", GL_REPEAT, 1, 0.0, 0.0, 0.0, 0.0, 0.0, GL_EYE_LINEAR, false);
}

void juegoPause(){
	/*
	escena->ocultaEscena();
    escena->agregaObjeto("pause",(Objeto *) new Canvas(5, 1.0));
    escena->objetos["pause"]->defineMaterial(0.0,0.0,0.0,0.0,
                                               0.0,0.0,0.0,0.0,
                                               0.0,0.0,0.0,0.0,
                                               0.0,0.0,0.0,0.0,
                                               "menu.tga",GL_CLAMP,1,0.0,0.0,0.0,0.0,0.0,GL_OBJECT_LINEAR,false);
	Objeto *pause;
	//pause = escena->objetos["pause"];
	pause->posZ = 4.0;
	pause->posX = -0.5;
	pause->posY = -0.5;
	*/
	if(!cargando_juego){
		Objeto *windowFin;
		Objeto *pause;
		escena->ocultaEscena();
		escena->agregaObjeto("WindowPause", (Objeto *) new Viewport(4));
		windowFin = escena->objetos["WindowPause"];
		windowFin->agregaDescendiente("pause", (Objeto *) new Canvas(10, 1.0), escena);
		pause = windowFin->descendientes["pause"];
		//pause->posZ = 4.8;
		pause->defineMaterial(0.0,0.0,0.0,0.0,
								0.0,0.0,0.0,0.0,
								0.0,0.0,0.0,0.0,
								0.0,0.0,0.0,0.0,
								"menu.tga",GL_CLAMP,1,0.0,0.0,0.0,0.0,0.0,GL_OBJECT_LINEAR,false);
		pause->posZ = 4.5;
		pause->posX = -0.5;
		pause->posY = -0.5;
	}else{
		Objeto *windowFin;
		Objeto *pause;
		escena->ocultaEscena();
		escena->agregaObjeto("WindowPauseCarga", (Objeto *) new Viewport(4));
		windowFin = escena->objetos["WindowPauseCarga"];
		windowFin->agregaDescendiente("pauseCarga", (Objeto *) new Canvas(10, 1.0), escena);
		pause = windowFin->descendientes["pauseCarga"];
		//pause->posZ = 4.8;
		pause->defineMaterial(0.0,0.0,0.0,0.0,
								0.0,0.0,0.0,0.0,
								0.0,0.0,0.0,0.0,
								0.0,0.0,0.0,0.0,
								"menu_cargando.tga",GL_CLAMP,1,0.0,0.0,0.0,0.0,0.0,GL_OBJECT_LINEAR,false);
		pause->posZ = 4.5;
		pause->posX = -0.5;
		pause->posY = -0.5;
	}
	
}

void creaEscena()
{
	escena->currentFrame = 0;
	switch(escena->juego->vistaActual){
		case JUEGO_MENU:
			juegoMenu();
			break;
		case JUEGO_ACTIVO:
			juegoActivo();
			break;
		case JUEGO_WIN:
			escena->juego->tren = 0;
			juegoWin();
			break;
		case JUEGO_LOSE:
			juegoLose();
			break;
		case JUEGO_PAUSE:
			juegoPause();
			break;
	}
	
    
}

float angle = 0;
float radX = .05;
float radY = .05;
float centerX = .77;
float centerY = .35;
float orbitSpeed = .05;

void idle()
{
    static unsigned long time=glutGet(GLUT_ELAPSED_TIME);
    static unsigned long frametime=1000/30;
    
	

    if((glutGet(GLUT_ELAPSED_TIME)-time) > frametime)
    {
        escena->cuentaframe();
        time=glutGet(GLUT_ELAPSED_TIME);
        escena->actualizaEscena();
        
		switch(escena->juego->vistaActual){
			case JUEGO_MENU:
				juego_menu_idle();
				break;
			case JUEGO_PAUSE:
				juego_pause_idle();
				break;
			case JUEGO_ACTIVO:
				juego_activo_idle();
				break;
			case JUEGO_WIN:
				juego_win_idle();
				break;
			case JUEGO_LOSE:
				juego_lose_idle();
				break;
	
		}
		/*
        if(escena->juego->vistaActual == JUEGO_ACTIVO){			
            juego_activo_idle();
        }
		if(escena->juego->vistaActual == JUEGO_WIN) {
            //creaEscena();
			juego_win_idle();
        }
		if (escena->juego->vistaActual == JUEGO_MENU) {
			//creaEscena();
			juego_menu_idle();
		}
		*/
		glutPostRedisplay();
    }
	
		
}

void juego_activo_idle(){
	//Inicializar las velocidades
    avion1->velZ = x;
	avion2->velY = (-1)*x;
	avion1->posY -=.2;
	indicadorAltActual->posY -=0.0022;
	NuiSkeletonGetNextFrame(0, &ourframe);
	
	Gestures *gestures = new Gestures();		
	for(int i=0; i<6; i++){
		if(ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED){
			gestures->setGesture(
							gestures->decodeGesture(
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z
							)
							);
						switch(gestures->getGesture()){
							case CROSS_HANDS:
								avion1->velX = 0;
								avion1->rotZ = 0;
								avion1->rotX = 0;
								avion2->velX = 0;
								avion2->rotZ = 0;
								avion2->rotX = 0;
								avion3->velX = 0;
								avion3->rotZ = 0;
								avion3->rotX = 0;
								x = 0;
								//cout << "CROSS HANDS" << endl;
								break;
							case HORIZONTAL_HANDS:
								//x = 0;
								//cout << "HORIZONTAL HANDS" << endl;
								izq = ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y;
								der = ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y;
								if(izq < der){
									avion1->rotZ += 1;
									avion1->velX = -.05;
									avion2->velX = -.05;
									avion3->rotZ += 1;
									estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
									1.0, 1.0, 1.0, 1.0,
									0.0, 0.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 0.0,
									"izquierda.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
								} else if(izq > der){
									avion1->rotZ -= 1;
									avion1->velX = .05;
									avion2->velX = .05;
									avion3->rotZ -= 1;
									estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
									1.0, 1.0, 1.0, 1.0,
									0.0, 0.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 0.0,
									"derecha.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
								}
								break;
							case EQUAL_RIGHT_HANDS:
								//x = 0;
								avion1->velX = 0;
								avion1->rotZ = 0;
								avion1->rotX = 30;
								estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
								1.0, 1.0, 1.0, 1.0,
								0.0, 0.0, 0.0, 0.0,
								0.0, 0.0, 0.0, 0.0,
								"tren.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
								//cout << "EQUAL RIGHT HANDS"  << endl;
								escena->juego->tren = 1;
								break;
							case EQUAL_LEFT_HANDS:
								//x = 0;
								avion1->velX = 0;
								avion1->rotZ = 0;
								avion1->rotX = 30;
								//cout << "EQUAL LEFT HANDS"  << endl;
								estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
								1.0, 1.0, 1.0, 1.0,
								0.0, 0.0, 0.0, 0.0,
								0.0, 0.0, 0.0, 0.0,
								"tren.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
								escena->juego->tren = 1;
								break;
							case  HANDS_UP:
								//cout << "HANDS UP"  << endl;
								//TODO
								avion1->rotX -=1;
								//avion1->posY -=1;
								avion1->velX = 0;
								avion3->rotX -=1; 
								estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
								1.0, 1.0, 1.0, 1.0,
								0.0, 0.0, 0.0, 0.0,
								0.0, 0.0, 0.0, 0.0,
								"sube.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
								//x= 0;
								break;
							case  HANDS_DOWN:
								//cout << "HANDS DOWN"  << endl;
								avion1->rotX +=1;
								//avion1->posY +=1;
								avion1->velX = 0;
								avion3->rotX +=1; 
								estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
								1.0, 1.0, 1.0, 1.0,
								0.0, 0.0, 0.0, 0.0,
								0.0, 0.0, 0.0, 0.0,
								"baja.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
								//x = 0;
								break;
							case  DOWN_L_LEFT_HANDS:
								//cout << "DOWN L LEFT HANDS"  << endl;
								x -= .005;
								avion1->velX = 0;
								estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
								1.0, 1.0, 1.0, 1.0,
								0.0, 0.0, 0.0, 0.0,
								0.0, 0.0, 0.0, 0.0,
								"desacelera.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
								break;
							case  DOWN_L_RIGHT_HANDS:
								//cout << "DOWN L RIGHT HANDS"  << endl;
								x += .005;
								avion1->velX = 0;
								estatusGestura->defineMaterial(1.0, 1.0, 1.0, 1.0,
								1.0, 1.0, 1.0, 1.0,
								0.0, 0.0, 0.0, 0.0,
								0.0, 0.0, 0.0, 0.0,
								"acelera.tga", GL_REPEAT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, GL_OBJECT_LINEAR, false);
								break;
							default:
								//cout << "NO DEFINIDO"  << endl;
								break;
						}
		}
		
		if(escena->currentFrame >= 200){
			x -= .005;
			
			/*if(avion1->rotX >= 0 && avion1->rotX<= 1){
				avion1->rotX = 0;
			} else {
				avion1->rotX -= 1;
			} */   
			//printf("Rotacion %f \n", avion1->rotX);
			//printf("Velocidad %f \n", x);
		}
		//Hacer un método para manejar el marco del cubo de posiciones
		//Mantener la velocidad al momento de que el avión llegue
		if(x < .1 && x != 0){
			x = .1;
		}
		
		//Posicionar el avion a la hora del aterrizaje
		if(avion1->posY <= -3.7){
			//avion1->rotX = -10;
			avion1->posY = -3.8;
			//avion3->rotX = -10;
		}

		if(indicadorAltActual->posY <= -0.3){
			indicadorAltActual->posY = -0.3;
		}   
		//printf("%d", escena->juego->tren);
		if(avion1->posZ >= .5 && avion1->posZ <= 3){
			//printf("LLEGUE A 1000");
			//printf("Pos Y %f \n", avion2->posY);
			if((avion1->posX < -3.0 || avion1->posX > 3.0 || avion1->rotY > 10 || avion1->rotY < -10 || escena->juego->tren == 0) ){
				escena->juego->muerte++;
			}
			x = 0;
			y = 0;
			escena->juego->no_vidas -= 1;
                
			if(escena->juego->no_vidas <= 0){
				if(escena->juego->muerte <= 0){
					//escena->juego->tren = 0;
					escena->juego->vistaActual = JUEGO_WIN;
					escena->juego->muerte = 0;
					escena->currentFrame = 0;
					avion1->rotX = 30;
					avion1->posY = 50;
					x = .4;
					avion1->posZ = -100;
                    
					avion2->rotX = 90;
					avion2->posY = 94;
					y = - .4;
					creaEscena();
				}else{	
					escena->juego->tren = 0;
					escena->juego->vistaActual = JUEGO_LOSE;
					escena->juego->muerte = 0;
					escena->currentFrame = 0;
					avion1->rotX = 30;
					avion1->posY = 50;
					x = .4;
					avion1->posZ = -100;
                    
					avion2->rotX = 90;
					avion2->posY = 94;
					y = - .4;
					creaEscena();
					
				}
				
			} else {
				if(escena->juego->tren >= 1 && avion1->posY > -3.0){
					avion1->rotX = 30;
					avion1->posY = 50;
					x = .4;
					avion1->posZ = -100;
                    
					avion2->rotX = 90;
					avion2->posY = 94;
					y = - .4;
					escena->juego->tren = 0;
					escena->juego->vistaActual = JUEGO_LOSE;
					escena->currentFrame = 0;
					creaEscena();
				}else

                if(escena->juego->muerte <= 0){    
					juegoActivo();
                    
					avion1->rotX = 30;
					avion1->posY = 50;
					x = .4;
					avion1->posZ = -100;
                    
					avion2->rotX = 90;
					avion2->posY = 94;
					y = - .4;
					escena->currentFrame = 0;
				}else{
					escena->juego->tren = 0;
					avion1->rotX = 30;
					avion1->posY = 50;
					x = .4;
					avion1->posZ = -100;
                    
					avion2->rotX = 90;
					avion2->posY = 94;
					y = - .4;
					escena->juego->vistaActual = JUEGO_LOSE;
					escena->currentFrame = 0;
					creaEscena();
				}
			}
		}
	}
}

void juego_win_idle(){
	NuiSkeletonGetNextFrame(0, &ourframe);
	Gestures *gestures = new Gestures();		
	for(int i=0; i<6; i++){
		if(ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED){
			gestures->setGesture(
							gestures->decodeGesture(
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z
							)
							);
						switch(gestures->getGesture()){
							case CROSS_HANDS:
								//cout << "CROSS_HANDS"  << endl;
								escena->currentFrame = 0;
								escena->juego->tren = 0;
								escena->juego->no_vidas = 3;
								escena->juego->vistaActual = JUEGO_PAUSE;
								creaEscena();
								break;
							case HORIZONTAL_HANDS:
								//exit(0);
								break;
							case HANDS_DOWN:
								
								break;
							default:
								
								break;

						}
		}
	}
}

void juego_lose_idle(){
	NuiSkeletonGetNextFrame(0, &ourframe);
	Gestures *gestures = new Gestures();		
	for(int i=0; i<6; i++){
		if(ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED){
			gestures->setGesture(
							gestures->decodeGesture(
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z
							)
							);
						switch(gestures->getGesture()){
							case CROSS_HANDS:
								//cout << "CROSS_HANDS"  << endl;
								escena->currentFrame = 0;
								escena->juego->tren = 0;
								escena->juego->no_vidas = 3;
								escena->juego->vistaActual = JUEGO_PAUSE;
								creaEscena();
								break;
							case HORIZONTAL_HANDS:
								//exit(0);
								break;
							case HANDS_DOWN:
								//exit(0);
								break;
							default:
								
								break;

						}
		}
	}

}

void juego_menu_idle(){
	NuiSkeletonGetNextFrame(0, &ourframe);

	for(int i=0; i<6; i++){

		if(ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED){
			//int gestura = decodeGesture(
			GLfloat a,b,c;
			//ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELET]
			a = ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x;
			b = ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y;
			c = 1 - ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z;
			if(mouse->posX >= .80){
				mouse->posX = .79;
			} else if(mouse->posX <= .20){
				mouse->posX = .20;
			} else {
				mouse->posX += a/50;
			}

			if(mouse->posY >= .68){
				mouse->posY = .67;
			} else if(mouse->posY <= .30){
				mouse->posY = .30;
			} else {
				mouse -> posY += b/50;
			}

			//printf("Mouse -> pos y: %lf\n", mouse->posY);
			mouse -> posY += b/50;

			if(mouse->posX >= play->posX && mouse->posX < .80) {
				if(angle < 8){
					loader->posX = centerX + cos(angle) * radX;
					loader->posY = centerY + sin(angle) * radY;
					angle += orbitSpeed;	
				} else {
					escena->juego->vistaActual = JUEGO_PAUSE;
					escena->currentFrame = 0;
					creaEscena();
				}
			} else {
				loader->posX = .77;
				loader->posY = .37;
			}
			/*printf("HAND RIGHT X %lf/n", a);
			printf("HAND RIGHT Y %lf/n", b);
			printf("HAND RIGHT Z %lf/n", c);*/
		}
	}
}

void juego_pause_idle(){
	NuiSkeletonGetNextFrame(0, &ourframe);
	Gestures *gestures = new Gestures();		
	for(int i=0; i<6; i++){
		if(ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED){
			gestures->setGesture(
							gestures->decodeGesture(
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y,
							ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z
							)
							);
						switch(gestures->getGesture()){
							case CROSS_HANDS:
								//cout << "CROSS_HANDS"  << endl;
								cargando_juego = true;
								escena->currentFrame = 0;
								creaEscena();
								break;
							case HORIZONTAL_HANDS:
								//cout << "HORIZONTAL HANDS"  << endl;
								break;
							case HANDS_DOWN:
								if(cargando_juego){
									escena->juego->vistaActual = JUEGO_ACTIVO;
									escena->currentFrame = 0;
									creaEscena();
								}
								break;
							default:
								//cout << "NO DEFINIDO"  << endl;
								break;

						}
		}
	}
}

int main( int argc, char **argv) {
	ISoundEngine* engine = createIrrKlangDevice();
	
    if (!engine)
       return 0; // error starting up the engine
	engine->play2D("./musica/pedro.mp3", true);
	glutInit(&argc,argv);
	if(init(WIN_POSX,WIN_POSY,WIN_WIDTH,WIN_HEIGHT,"ARM Pedro Infante") == -1){
		system("pause");
		return -1;
	}
	glutDisplayFunc(render);
	glutIdleFunc(idle);
	glutReshapeFunc(redimensiona);
    glutSpecialFunc(keyboardSpec);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
    glutMotionFunc(mouseDrag);
	defineLuces();
	defineCamaras();
    creaEscena();
	glutMainLoop();
	return 0;
}
