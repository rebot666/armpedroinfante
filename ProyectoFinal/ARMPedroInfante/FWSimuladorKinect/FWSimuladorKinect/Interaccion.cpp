#include "Interaccion.h"

Camara *camaraDefault;
Camara *camaraZenital;
Camara *activa;

void defineCamaras()
{
    // Definiciones de camaras - Mantener al menos la definicion de camaraDefault
    camaraDefault = new Camara(0.0,0.0,-5.0,0.0,0.0,0.0,100000.0,0.1,90.0);
    camaraZenital = new Camara(0.0,0.0,-5.0,90.0,0.0,0.0,100.0,1.0,45.0);
    
    //Asignar a activa la camara con la que se desea iniciar
    
    activa=camaraDefault;
}

void keyboard(unsigned char key,int x,int y)
{	
		/*switch(key)
		{
			  case '+': activa->zview+=dtran;
					  redimensiona(currWidth,currHeight);		                       
					  break;

			  case '-': activa->zview-=dtran;
					  redimensiona(currWidth,currHeight);
					  break;

			  case 'x': activa->xrot+=drot;
						redimensiona(currWidth,currHeight);
						break;
			  case 'y': activa->yrot+=drot;
						redimensiona(currWidth,currHeight);
						break;
			  case 'z': activa->zrot+=drot;		  
						redimensiona(currWidth,currHeight);
						break;

			  case 'X': activa->xrot-=drot;
						redimensiona(currWidth,currHeight);
						break;

			  case 'Y': activa->yrot-=drot;
						redimensiona(currWidth,currHeight);
						break;

			  case 'Z': activa->zrot-=drot;
						redimensiona(currWidth,currHeight);
						break;
						//mods
			  case 'a': escena->juego->vistaActual = JUEGO_ACTIVO;
				  break;
			  case 'A': escena->juego->vistaActual = JUEGO_ACTIVO;
				  break;

			  case 'p': if (pausalfa == 0.0)
			  {
				pausalfa = 0.6;
			  }				  
						else if (pausalfa == 0.6)
						{
							exit(0);
							pausalfa = 0.0;
						}
						printf("pausalfa: %4.2f", pausalfa);
					
				  break;
						

			case ESCAPE: exit(0);
						 break;
		}*/
}

void keyboardSpec(int key,int x,int y)
{
		/*switch(key)
		{
		  case GLUT_KEY_UP:        activa->yview+=dtran;
								   redimensiona(currWidth,currHeight);
								   break;

		  case GLUT_KEY_DOWN:      activa->yview-=dtran;
								   redimensiona(currWidth,currHeight);
								   break;

		  case GLUT_KEY_PAGE_UP:   activa->zview+=dtran;
								   redimensiona(currWidth,currHeight);
								   break;

		  case GLUT_KEY_PAGE_DOWN: activa->zview-=dtran;
								   redimensiona(currWidth,currHeight);
								   break;

		  case GLUT_KEY_LEFT:      activa->xview-=dtran;
								   redimensiona(currWidth,currHeight);
								   break;

		  case GLUT_KEY_RIGHT:     activa->xview+=dtran;
								   redimensiona(currWidth,currHeight);
								   break;
								   
		  case GLUT_KEY_F1:        escena->toggleLuz("light0");
								   break;
								   
		  case GLUT_KEY_F2:        escena->toggleLuz("light1");
								   break;
								   
		  case GLUT_KEY_F3:        escena->toggleLuz("light2");
								   break;
								   
		  case GLUT_KEY_F4:        escena->toggleLuz("light3");
								   break;
								   
		  case GLUT_KEY_F5:        escena->toggleLuz("light4");
								   break;
								   
		  case GLUT_KEY_F6:        escena->toggleLuz("light5");
								   break;
								   
		  case GLUT_KEY_F7:        escena->toggleLuz("light6");
								   break;
								   
		  case GLUT_KEY_F8:        escena->toggleLuz("light7");
								   break;
			 
		}*/
}


void mouseClick(int button, int state, int x, int y)
{
	/*switch (button) {
		case GLUT_LEFT_BUTTON:   leftMouse = (state==GLUT_DOWN);
								 break;
								 
		case GLUT_MIDDLE_BUTTON: middleMouse = (state==GLUT_DOWN);
							     break;
								 
		case GLUT_RIGHT_BUTTON:  rightMouse = (state==GLUT_DOWN);
							     break;

	}
	if (state==GLUT_UP) 
	{
	      //Reinicia las posiciones previas para permitir movimientos acumulativos
		  lastMouseX=0;
          lastMouseY=0;
	}*/
}

void mouseDrag(int x, int y)
{
  /*GLfloat deltaX=(lastMouseX == 0) ? MOUSE_DELTA_MIN : (GLfloat)(x-lastMouseX)/currWidth;
  GLfloat deltaY=(lastMouseY == 0) ? MOUSE_DELTA_MIN : (GLfloat)(lastMouseY-y)/currHeight;
  
    if (leftMouse) //Desplazamiento horizontal/vertical
	{
		activa->xview+=MOUSE_MUL_TX * deltaX;
		activa->yview+=MOUSE_MUL_TY * deltaY;
		redimensiona(currWidth, currHeight);
	}
    if (middleMouse) //Zoom in/Zoom out
	{
		activa->zview+=MOUSE_MUL_TZ * deltaY;
		redimensiona(currWidth, currHeight);
	}
	if (rightMouse) //Movimiento horizontal, rotacion respecto a eje Y; Movimiento vertical, rotacion respecto a eje X
	{
	   activa->yrot+=MOUSE_MUL_RY * deltaX;
	   activa->xrot+=MOUSE_MUL_RX * deltaY;
	   redimensiona(currWidth, currHeight);
	}
  lastMouseX=x;
  lastMouseY=y;*/
}




