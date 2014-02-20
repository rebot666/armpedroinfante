#include "Escena.h"
#include <iostream>

//Metodos de Objeto

// Constructor
Objeto::Objeto()
{
	colorR=1.0;
	colorG=1.0;
	colorB=1.0;
	colorA=1.0;
	posX=0.0;
	posY=0.0;
	posZ=0.0;
	velX=0.0;
	velY=0.0;
	velZ=0.0;
	rotX=0.0;
	rotY=0.0;
	rotZ=0.0;
	velRotX=0.0;
	velRotY=0.0;
	velRotZ=0.0;
	escalaX=1.0;
	escalaY=1.0;
	escalaZ=1.0;
	velEscalaX=0.0;
	velEscalaY=0.0;
	velEscalaZ=0.0;
	displayList=0;
	loopFrames=0;
	pivotX=0.0;
    pivotY=0.0;
	pivotZ=0.0;
	usaMaterial=false;
	material.textureid=0;
	limpia(material.ambient);
	limpia(material.diffuse);
	limpia(material.specular);
	limpia(material.emmissive);
	mappingMode=GL_EYE_LINEAR;
}

// Destructor (vacio)
Objeto::~Objeto()
{
  delete this;
}

// Actualiza las transformaciones del objeto
void Objeto::actualiza()
{
	if(loopFrames)
		if((escenaActual->currentFrame % loopFrames)==0)
		// Invierte el sentido de las transformaciones cada loopFrames
		{
			velX= -1.0*velX;
			velY= -1.0*velY;
			velZ= -1.0*velZ;
			velRotX= -1.0*velRotX;
			velRotY= -1.0*velRotY;
			velRotZ= -1.0*velRotZ;
			velEscalaX = -1.0*velEscalaX;
			velEscalaY = -1.0*velEscalaY;
			velEscalaZ = -1.0*velEscalaZ;
		}

	posX+=velX;
	posY+=velY;
	posZ+=velZ;
	rotX+=velRotX;
	rotY+=velRotY;
	rotZ+=velRotZ;
	escalaX+=velEscalaX;
	escalaY+=velEscalaY;
	escalaZ+=velEscalaZ;
	map<string, Objeto *>::const_iterator iter;   

	for (iter=descendientes.begin(); iter != descendientes.end(); ++iter) 
    {
	   Objeto *o=(Objeto *)iter->second;
	   o->actualiza();	  	   
    }
}


// Aplica las transformaciones y dibuja el objeto transformado
void Objeto::dibuja()
{    
    glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslatef(posX,posY,posZ);
	glScalef(escalaX,escalaY,escalaZ);
	glRotatef(rotX,1.0+pivotX,0.0+pivotY,0.0+pivotZ);
    glRotatef(rotY,0.0+pivotX,1.0+pivotY,0.0+pivotZ);
    glRotatef(rotZ,0.0+pivotX,0.0+pivotY,1.0+pivotZ);
	if (usaMaterial) 
	{
		aplicaMaterial();
		forma();
		anulaMaterial();
	}
	else 
	{
		glColor3f(colorR,colorG,colorB);
        forma();
		glColor3f(1.0,1.0,1.0);
	}
    map<string, Objeto *>::const_iterator iter;   
    for (iter=descendientes.begin(); iter != descendientes.end(); ++iter) 
    {
	   Objeto *o=(Objeto *)iter->second;
	   o->dibuja();	  	   
    }
    glPopMatrix();
}

void Objeto::escalaUniforme(GLfloat escala)
{
   this->escalaX=escala;
   this->escalaY=escala;
   this->escalaZ=escala;
}

void Objeto::copiaMaterial(string nombreModelo,string nombreMaterial,GLuint	mappingMode)
{
	Modelo *modelo=(Modelo *)this->escenaActual->objetos[nombreModelo];
	GLMmodel *m=modelo->modelo;
	int i;

	for(i=0; i<m->nummaterials; i++)
		if(strcmp(nombreMaterial.c_str(),m->materials[i].name)==0)
			break;
	if(i < m->nummaterials)
	{
	    this->usaMaterial=true;
	    this->material.textureid=m->materials[i].textureid;
		this->material.wrapmode=m->materials[i].wrapmode;
		this->material.alpha=m->materials[i].alpha;
		this->material.scale=m->materials[i].scale;
		this->material.stexspeed=m->materials[i].stexspeed;
		this->material.ttexspeed=m->materials[i].ttexspeed;
		this->mappingMode=mappingMode;
        printf("copiaMaterial:El material %s del modelo %s fue copiado \n",nombreMaterial.c_str(),nombreModelo.c_str());	
	}
	else
		printf("copiaMaterial:El material %s no fue encontrado en el modelo %s\n",nombreMaterial.c_str(),nombreModelo.c_str());

}

void Objeto::defineMaterial(GLfloat aR,GLfloat aG,GLfloat aB, GLfloat aA,
					GLfloat dR, GLfloat dG, GLfloat dB, GLfloat dA,
					GLfloat sR, GLfloat sG, GLfloat sB, GLfloat sA,
					GLfloat eR,GLfloat eG,GLfloat eB, GLfloat eA,
					string  nombreTextura,
					GLuint  wrapmode,              
					GLfloat alpha,
					GLfloat stexdis,			   
					GLfloat ttexdis,			    
					GLfloat stexspeed,			
					GLfloat ttexspeed,			
					GLfloat scale,
					GLuint	mappingMode,
					bool lighting)
{
        usaMaterial=true;
		if(nombreTextura.length()>0)
		{
			if(escenaActual->texturas.count(nombreTextura)==0)
			  // La textura no ha sido cargada, se carga con parametros por default
			   escenaActual->cargaTextura(nombreTextura,GL_RGBA,wrapmode,TEX_FILTER);

		}
		copia(material.ambient,aR,aG,aB, aA);
		copia(material.diffuse,dR,dG,dB, dA);
		copia(material.specular,sR,sG,sB,sA);
		copia(material.emmissive,eR,eG,eB, eA);
		material.textureid=this->escenaActual->texturas[nombreTextura];
		material.wrapmode = wrapmode;
		material.alpha = alpha;
		material.diffuse[3]=alpha;
		material.stexdis=stexdis;
		material.ttexspeed=ttexdis;
		material.ttexdis=stexspeed;
		material.ttexspeed=ttexspeed;
		material.scale=scale;
		material.lighting=lighting;
		this->mappingMode=mappingMode;
		
}

void Objeto::aplicaMaterial()
{
        // Guarda Enable/Disable para reestablecerlos en anulaMaterial()
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glEnable(GL_COLOR_MATERIAL);
		
		if (!material.lighting) {
			glDisable(GL_LIGHTING);
		}
		
		if(material.alpha < 1.0) //Habilita blending
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            material.diffuse[3]=material.alpha;
		}
		if(material.textureid > 0) //Habilita texturizado
		{          
	      glEnable(GL_TEXTURE_2D);
		  glBindTexture(GL_TEXTURE_2D, material.textureid);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, material.wrapmode);
	      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, material.wrapmode);		  		  
		  // Cambia las componentes de difusse para aplicar colores de textura
		  material.diffuse[0]=1.0;
          material.diffuse[1]=1.0;
          material.diffuse[2]=1.0;
		  if((material.scale != 1.0) || (material.stexspeed != 0.0) || (material.ttexspeed != 0.0))
		  {
			  // Aplica transformaciones de textura
			  glMatrixMode(GL_TEXTURE);
	          glPushMatrix();
	          glLoadIdentity();
			  material.stexdis+=material.stexspeed;
              material.ttexdis+=material.ttexspeed;
			  glTranslatef(material.stexdis,material.ttexdis,0.0);
			  glScalef(1.0/material.scale,1.0/material.scale,0.0);
			  glPopMatrix();
		      glMatrixMode(GL_MODELVIEW);
		  }
		  // Habilita el modo de generacion de coordenadas de textura en S y T
		  glEnable(GL_TEXTURE_GEN_S);
	      glEnable(GL_TEXTURE_GEN_T);
		  glTexGenf(GL_S,GL_TEXTURE_GEN_MODE,mappingMode);
		  glTexGenf(GL_T,GL_TEXTURE_GEN_MODE,mappingMode);
		}
		if (definido(material.ambient)) 
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
		}
		if (definido(material.diffuse)) 
		{
		    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
		}
		if (definido(material.specular)) 
		{
		    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
		    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
		}
		if (definido(material.emmissive)) 
		{
		    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material.emmissive);
		}
        glColor4fv(material.diffuse);
}



void Objeto::anulaMaterial()
{
    // Restaura Enable/Disable
    glPopClientAttrib();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);	
	glEnable(GL_LIGHTING);
}


void Objeto::agregaDescendiente(string nombre,Objeto *nuevoObjeto, Escena *escena)
{
	descendientes[nombre]=nuevoObjeto;	
	descendientes[nombre]->escenaActual=this->escenaActual;
	escena->objetos_totales[nombre]=nuevoObjeto;
}

//Metodos rectangulo
Rectangulo::Rectangulo(GLfloat a, GLfloat base,GLfloat ab,GLfloat baseb):Objeto(){
    this->a = a;
	this->base = base;
    this->ab = ab;
	this->baseb = baseb;
}

void Rectangulo::forma(){
  glRectf(a,base, ab, baseb);
}

//Metodos del cubo

// Constructor
Cubo::Cubo(GLfloat lado,GLfloat r,GLfloat g,GLfloat b, GLfloat a):Objeto()
{
	this->lado = lado;
	this->colorR = r;
	this->colorG = g;
	this->colorB = b;
	this->colorA = a;
}

// Forma de cubo
void Cubo::forma()
{
   glutSolidCube(lado);
}


//Metodos de la tetera

// Constructor
Tetera::Tetera(GLfloat tam,GLfloat r,GLfloat g,GLfloat b):Objeto()
{
	this->tam = tam;
	this->colorR = r;
	this->colorG = g;
	this->colorB = b;
}

// Forma de cubo
void Tetera::forma()
{
   glutSolidTeapot(tam);
}


//Metodos de Esfera

// Constructor
Esfera::Esfera(GLfloat radio,GLfloat meridianos,GLfloat paralelos,GLfloat r,GLfloat g,GLfloat b):Objeto()
{
	this->radio = radio;
	this->meridianos = meridianos;
	this->paralelos = paralelos;
	this->colorR = r;
	this->colorG = g;
	this->colorB = b;
}


// Forma de esfera
void Esfera::forma()
{
   glutSolidSphere(radio,meridianos,paralelos);
}


//Metodos de avion
Avion::Avion(GLfloat r,GLfloat g,GLfloat b):Objeto()
{
  	this->colorR = r;
	this->colorG = g;
	this->colorB = b;
}

void Avion::forma()
{
    typedef M3DVector3f Triangulo[3];
    Triangulo avion[]=
    {
	    {{ 0.0f, 0.0f, 60.0f},{ -15.0f, 0.0f, 30.0f},{ 15.0f,0.0f,30.0f}},
	    {{ 15.0f, 0.0f,  30.0f},{ 0.0f,  15.0f, 30.0f},{ 0.0f,  0.0f,  60.0f}},
        {{ 0.0f, 0.0f, 60.0f },{ 0.0f, 15.0f, 30.0f },{ -15.0f, 0.0f, 30.0f }},
	    {{ -15.0f, 0.0f, 30.0f },{ 0.0f, 15.0f, 30.0f },{ 0.0f, 0.0f, -56.0f }},
	    {{ 0.0f, 0.0f, -56.0f },{ 0.0f, 15.0f, 30.0f },{ 15.0f,0.0f,30.0f }},
	    {{ 15.0f,0.0f,30.0f },{-15.0f, 0.0f, 30.0f  },{ 0.0f, 0.0f, -56.0f }},
	    {{ 0.0f,2.0f,27.0f },{ -60.0f, 2.0f, -8.0f },{ 60.0f, 2.0f, -8.0f }},
	    {{ 60.0f, 2.0f, -8.0f},{0.0f, 7.0f, -8.0f},{0.0f,2.0f,27.0f }},
	    {{ 60.0f, 2.0f, -8.0f},{-60.0f, 2.0f, -8.0f},{0.0f,7.0f,-8.0f }},
	    {{ 0.0f,2.0f,27.0f},{0.0f, 7.0f, -8.0f},{-60.0f, 2.0f, -8.0f}},
	    {{ -30.0f, -0.50f, -57.0f},{30.0f, -0.50f, -57.0f},{0.0f,-0.50f,-40.0f}},
	    {{ 0.0f,-0.5f,-40.0f },{30.0f, -0.5f, -57.0f},{0.0f, 4.0f, -57.0f }},
	    {{ 0.0f, 4.0f, -57.0f },{ -30.0f, -0.5f, -57.0f },{ 0.0f,-0.5f,-40.0f }},
	    {{ 30.0f,-0.5f,-57.0f },{ -30.0f, -0.5f, -57.0f },{ 0.0f, 4.0f, -57.0f }},
	    {{ 0.0f,0.5f,-40.0f },{ 3.0f, 0.5f, -57.0f },{ 0.0f, 25.0f, -65.0f }},
	    {{ 0.0f, 25.0f, -65.0f },{ -3.0f, 0.5f, -57.0f},{ 0.0f,0.5f,-40.0f }},
	    {{ 3.0f,0.5f,-57.0f },{ -3.0f, 0.5f, -57.0f },{ 0.0f, 25.0f, -65.0f }},
    };
    
    glBegin(GL_TRIANGLES);
	for (int i=0; i<17; i++) {
		//TODO Agregar normales
		M3DVector3f normal;
		m3dFindNormal(normal, avion[i][0], avion[i][1], avion[i][2]);
		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3fv(avion[i][0]);
		glVertex3fv(avion[i][1]);
		glVertex3fv(avion[i][2]);
	}
    glEnd();
}

//Metodos del cilindro

Cilindro::Cilindro(GLfloat radio,GLfloat altura,int lados,
	               GLfloat r,GLfloat g,GLfloat b)
{
    this->radio = radio;
    this->altura = altura;
    this->lados = lados;
    this->colorR = r;
    this->colorG = g;
    this->colorB = b;
}

void Cilindro::forma()
{
    glPushMatrix();
    tapa(-1.0);
    cuerpo();
    glTranslatef(0.0, 0.0, altura);
    tapa(1.0);
    glPopMatrix();
}

void Cilindro::tapa(GLfloat signoNormal)
{
    glBegin(GL_TRIANGLE_FAN);
    GLfloat angulo=0.0;
    GLfloat dangulo=2*M_PI/lados;
    GLfloat x,y;
    glNormal3f(0.0,0.0,signoNormal);
    glVertex3f(0.0,0.0,0.0);
    for (int i=0; i<=lados; i++) {
        x=radio*cos(angulo);
        y=radio*sin(angulo);
        glVertex3f(x,y,0.0);
        angulo+=dangulo;
    }
    glEnd();
}

void Cilindro::cuerpo()
{
    glBegin(GL_QUAD_STRIP);
    GLfloat angulo=0.0;
    GLfloat dangulo=2*M_PI/lados;
    GLfloat x,y;
    for (int i=0; i<=lados; i++) {
        x=cos(angulo);
        y=sin(angulo);
        glNormal3f(x,y,0.0);
        x*=radio;
        y*=radio;
        glVertex3f(x,y,0.0);
        glVertex3f(x,y,altura);
        angulo+=dangulo;
    }
    glEnd();
}

//Metodos del viewport
Viewport::Viewport(GLint ventanta) : Objeto(){
	no_ventana = ventanta;
}

void Viewport::forma(){
	switch (no_ventana)
	{
		case 1:
			glViewport (0, (WIN_WIDTH * .15), (WIN_WIDTH*.80), (WIN_HEIGHT*.80));
			break;
		case 2: 
			glViewport ((WIN_WIDTH * .80), (WIN_WIDTH * .15), (WIN_WIDTH*.20), (WIN_HEIGHT*.80));
			break;
		case 3:
			glViewport (0, 0, (WIN_WIDTH), (WIN_HEIGHT*.24));
			break;
		case 4:
			glViewport (0, 0, (WIN_WIDTH), (WIN_HEIGHT));
			break;

	}
}

//Metodos del canvas
Canvas::Canvas(GLint s, GLfloat a){
	size = s;
	alfa = a;
}

void Canvas::forma(){
	glColor4f(1.0,1.0,1.0, alfa);
	glBegin(GL_TRIANGLE_FAN);
	//modificación: los *(0.5)
        glVertex3f(-size, size*(0.5), 0);
		glVertex3f(size, size*(0.5), 0);
		glVertex3f(size, -size*(0.5), 0);
		glVertex3f(-size, -size*(0.5), 0);
    glEnd();
    glFlush();
}

//Metodos del juego
Juego::Juego(){
	no_vidas = 3;
	muerte = 0;
	tren = 0;
    //vistaActual = JUEGO_WIN;
	vistaActual = JUEGO_MENU;
	//vistaActual = JUEGO_ACTIVO;
	//vistaActual = JUEGO_PAUSE;
	//vistaActual = JUEGO_LOSE;
}

//Metodos de Modelo
Modelo::Modelo(string nombreArchivo):Objeto()
{
	char ruta[MAX_NOMBRE];

	this->nombreArchivo=nombreArchivo;
	sprintf(ruta,"modelos/%s.obj",nombreArchivo.c_str());
	modelo=glmReadOBJ(ruta);
	usaTexturas=false;
	if(modelo)
	{
       glmUnitize(modelo);	   
	   glmFacetNormals(modelo);
	   glmVertexNormals(modelo,90.0);
       glmLinearTexture(modelo);
	}
}

Modelo::Modelo(string nombreArchivo,int modoTextura):Objeto()
{
	char ruta[MAX_NOMBRE];

	this->nombreArchivo=nombreArchivo;
	sprintf(ruta,"modelos/%s.obj",nombreArchivo.c_str());
	modelo=glmReadOBJ(ruta);
	usaTexturas=true;
	desplazaTexturas=false;
	if(modelo)
	{
       glmUnitize(modelo);	   
	   glmFacetNormals(modelo);
	   glmVertexNormals(modelo,90.0);
	   if(modoTextura==TEX_LINEAR)
	     glmLinearTexture(modelo);
	   else
         glmSpheremapTexture(modelo);
	}
	
}

Modelo::Modelo(string nombreArchivo,bool calcular):Objeto(){
	char ruta[MAX_NOMBRE]; 
	this->nombreArchivo=nombreArchivo;
	sprintf(ruta,"modelos/%s.obj",nombreArchivo.c_str());
	modelo=glmReadOBJ(ruta);
	usaTexturas=false;
	if(modelo){
		glmUnitize(modelo);
		if(calcular){
			glmFacetNormals(modelo);
			glmVertexNormals(modelo,90.0);
			glmLinearTexture(modelo);
		}

	}
}


void Modelo::forma()
{
        if (desplazaTexturas) {
		   // Si hay texturas desplazadas, debe dibujarse el modelo siempre
			glmDraw(modelo,GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
		}
		else 
		{
			// La primera vez que despliega construye displayList
			if(!displayList)
			  if (usaTexturas)  
				  displayList = glmList(modelo,GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
				else 
				  displayList = glmList(modelo,GLM_SMOOTH | GLM_MATERIAL);
			// Si displayList esta disponible la utiliza, de lo contrario dibuja a partir de la geometria
			if(displayList)
			   glCallList(displayList);
			else
			  if (usaTexturas) 
				  glmDraw(modelo,GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
			  else 
				  glmDraw(modelo,GLM_SMOOTH | GLM_MATERIAL);
		}
}

Modelo::~Modelo()
{
	if(modelo)
	  glmDelete(modelo);
}

//Metodos AlphaQuad

AlphaQuad::	AlphaQuad(Escena *escenaActual,string nombreTextura,GLuint modoTextura,GLfloat alpha,GLfloat ancho,GLfloat alto,GLfloat texVelS,GLfloat texVelT,GLfloat texEscala,GLfloat texEscalaVel,GLfloat alphaVel):Objeto()
{
	this->modoTextura=modoTextura;
	this->ancho=ancho; 
	this->alto=alto; 
	this->texVelS=texVelS; 
	this->texVelT=texVelT;
	this->texPosS=0.0;
	this->texPosT=0.0;
	this->alpha=alpha;
	this->alphaVel=alphaVel;
	this->texEscala=texEscala;
	this->texEscalaVel=texEscalaVel;
	this->escenaActual=escenaActual;
	if(escenaActual->texturas.count(nombreTextura)==0)
	    // La textura no ha sido cargada, se carga al usarse por primera vez
	    escenaActual->cargaTextura(nombreTextura,GL_RGBA,modoTextura,TEX_FILTER);
	this->textura=escenaActual->texturas[nombreTextura];
	this->usaMaterial=false;
	this->loopFrames=DEFAULT_LOOP_FRAMES;
}

void AlphaQuad::forma()
{

    //Habilita texturas
	glEnable(GL_TEXTURE_2D);
	// Deshabilita iluminacion
	glDisable(GL_LIGHTING);

	//MOD
	glEnable(GL_RGBA);

	//Habilita blend con Alpha
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0,1.0,1.0,this->alpha);
	
	//Habilita Alpha Test
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_LESS,0.2);

	//Centra en el origen
	GLfloat x = 0.0 - (this->ancho/ 2.0);
	GLfloat y = 0.0 - (this->alto / 2.0);

	// Seleccion de la textura
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->modoTextura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->modoTextura);

	//Aplicacion del transformaciones de textura

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(this->texPosS,this->texPosT,0.0);
	glScalef(1.0/this->texEscala,1.0/this->texEscala,1.0);
	
	// Trazo del quad y mapeo de textura
	glBegin(GL_QUADS);				
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + this->ancho,y,0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + this->ancho, y + this->alto, 0.0); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y + this->alto,0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y,0.0);		
	glEnd();
	glPopMatrix();

    //Regresa las transformaciones a MODELVIEW
	glMatrixMode(GL_MODELVIEW);

	//Habilita luz
	glEnable(GL_LIGHTING);	

	//Deshabilita blend
	glDisable(GL_BLEND);
	
	//Deshabilita Alpha TEst
	glDisable(GL_ALPHA_TEST);
	
	//Actualiza transformaciones de textura
	this->texPosS+=this->texVelS;
	this->texPosT+=this->texVelT;
	this->texEscala+=this->texEscalaVel;
	this->alpha+=this->alphaVel;
	if (this->escenaActual->currentFrame % this->loopFrames == 0) {
		this->texEscalaVel=-1.0*this->texEscalaVel;
	    this->alphaVel=-1.0*this->alphaVel;
	}
}


//Metodos de Luz
Luz::Luz(char tipo):Objeto()
{
	this->tipo=tipo;
	this->activa=false;
	this->posicion[0]=0.0;
	this->posicion[1]=0.0;
	this->posicion[2]=0.0;
	this->posicion[3]=0.0;
	this->direccion[0]=0.0;
	this->direccion[1]=0.0;
	this->direccion[2]=0.0;
	this->color[0]=1.0;
	this->color[1]=1.0;
	this->color[2]=1.0;
	this->color[3]=1.0;
	this->angulo=0.0;
	this->glLight=0;
}

void Luz::forma()
{
  if (this->escenaActual->muestraLuces && this->activa)
  {
	glColor3fv(this->color);
	glutWireSphere(0.3, 10, 10);
  }
}


//Metodos de Escena


// Constructor
Escena::Escena()
{
  juego = new Juego();
  currentFrame=0;
  muestraLuces=false;
  char nombre[MAX_NOMBRE];
  for (int i=0; i<MAX_LUCES; i++) 
  {
     sprintf(nombre,"light%d",i);
	 objetos[nombre]=(Objeto *) new Luz('I');	
	 objetos[nombre]->escenaActual=this;  
	 Luz *l=(Luz *) objetos[nombre];
	 l->activa=false;
	 l->glLight=GL_LIGHT0+i;	  
  }
}


// Destructor
Escena::~Escena()
{
   map<string, Objeto *>::const_iterator iter;   
   for (iter=objetos.begin(); iter != objetos.end(); ++iter) 
   {
	   Objeto *o=(Objeto *)iter->second;
	   delete o;
   }
}

void Escena::actualizaEscena()
{
   map<string, Objeto *>::const_iterator iter;   
   for (iter=objetos.begin(); iter != objetos.end(); ++iter) 
   {
	   Objeto *o=(Objeto *)iter->second;
	   o->actualiza();
   } 
   for (int i=0; i<MAX_LUCES; i++) 
   {
	 Luz *l=luz(i);
	 Objeto *o=(Objeto *) l;
	 l->posicion[0]=o->posX;
	 l->posicion[1]=o->posY;
	 l->posicion[2]=o->posZ;
	 l->posicion[3]=0.0;
	 l->color[0]=o->colorR;
	 l->color[1]=o->colorG;
	 l->color[2]=o->colorB;
	 l->color[3]=1.0;
   }
}

void Escena::agregaObjeto(string nombre,Objeto *nuevoObjeto)
{
	objetos[nombre]=nuevoObjeto;	
	objetos[nombre]->escenaActual=this;
	objetos_totales[nombre] = nuevoObjeto;
}


void Escena::dibujaEscena()
{
   map<string, Objeto *>::const_iterator iter;   
   for (iter=objetos.begin(); iter != objetos.end(); ++iter) 
   {
	   Objeto *o=(Objeto *)iter->second;
	   o->dibuja();	  	   
   } 
}

void Escena::ocultaEscena()
{
    map<string, Objeto *>::const_iterator iter;
    for (iter=objetos_totales.begin(); iter != objetos_totales.end(); ++iter)
    {
        Objeto *o=(Objeto *)iter->second;
        o->escalaUniforme(0);
    } 
}

void Escena::cuentaframe()
{	  
	  currentFrame++; 
}

Luz *Escena::luz(int numero)
{
  char nombre[MAX_NOMBRE];
  sprintf(nombre,"light%d",numero);
  return (Luz *) objetos[nombre];
}


void Escena::defineLuz(string nombre,char tipo,GLfloat r,GLfloat g,GLfloat b)
{
	Luz *l=(Luz *) objetos[nombre];
	Objeto *o=(Objeto *) l;
	l->tipo=tipo;
	o->colorR=r;
	o->colorG=g;
	o->colorB=b;
	l->color[0]=o->colorR;
	l->color[1]=o->colorG;
	l->color[2]=o->colorB;
	l->color[3]=1.0;
	l->posicion[3]=(l->tipo == 'A') ? 0.0 : 1.0;
	l->activa=true;
}

void Escena::posicionLuz(string nombre,GLfloat x,GLfloat y,GLfloat z)
{
	Luz *l=(Luz *) objetos[nombre];
	Objeto *o=(Objeto *) l;
    o->posX=x;
	o->posY=y;
	o->posZ=z;
	l->posicion[0]=o->posX;
	l->posicion[1]=o->posY;
	l->posicion[2]=o->posZ;
}

void Escena::direccionLuz(string nombre,GLfloat x,GLfloat y,GLfloat z)
{
	Luz *l=(Luz *) objetos[nombre];
	l->direccion[0]=x;
	l->direccion[1]=y;
	l->direccion[2]=z;
}

void Escena::anguloLuz(string nombre,GLfloat angulo)
{
	Luz *l=(Luz *) objetos[nombre];
	l->angulo=angulo;
}

void Escena::toggleLuz(string nombre)
{
	Luz *l=(Luz *) objetos[nombre];
	if(l->tipo=='I')
	  // Si la luz es Indefinida, siempre se desactiva
	  l->activa=false;
	else
	  // Si la luz es de otro tipo, se invierte el estado de activa
	  l->activa=!(l->activa);
}

void Escena::ilumina()
{  
  glLoadIdentity();
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
  glShadeModel(GL_SMOOTH);
  for (int i=0; i<MAX_LUCES; i++) 
  {
	 Luz *l=luz(i);
	 if (l->activa && l->tipo!='I') 
	   {
		 switch (l->tipo) {
			 case 'A': // Luz Ambiente
					   glLightfv(l->glLight,GL_AMBIENT,l->color);
				       break;
					   
			 case 'D': // Luz Difusa
					   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
					   glLightfv(l->glLight,GL_DIFFUSE,l->color);
					   glLightfv(l->glLight,GL_POSITION,l->posicion);
				       break;
					   
			 case 'E': // Luz Especular
					   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, l->color);
					   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,SPECULAR_SHININESS);
					   glLightfv(l->glLight,GL_SPECULAR,l->color);
					   glLightfv(l->glLight,GL_POSITION,l->posicion);
				       break;  
					    
			 case 'S': // Spot Light
				       glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
					   glLightfv(l->glLight,GL_DIFFUSE,l->color);
					   glLightfv(l->glLight,GL_POSITION,l->posicion);
					   glLightfv(l->glLight, GL_SPOT_DIRECTION,l->direccion);
					   glLightf(l->glLight, GL_SPOT_CUTOFF, l->angulo);				   
				       break; 
					   
		 }
		 glEnable(l->glLight);
	   }
	 else 
	   {
	     // Luz indefinida o inactiva
		 glDisable(l->glLight);
	   }

  }
}


////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *Escena::gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
	{
    FILE *pFile;			// File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;		// Size in bytes of image
    short sDepth;			// Pixel depth;
    GLbyte	*pBits = NULL;          // Pointer to bits
    
    // Default/Failed values
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_BGR_EXT;
    *iComponents = GL_RGB8;
    
    // Attempt to open the fil
    pFile = fopen(szFileName, "rb");
    if(pFile == NULL)
        return NULL;
	
    // Read in header (binary)
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
	
    // Get width, height, and depth of texture
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
    
    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
	
    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    
    // Allocate memory and check for success
    pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    if(pBits == NULL)
        return NULL;
    
    // Read in the bits
    // Check for read error. This should catch RLE or other 
    // weird formats that I don't want to recognize
    if(fread(pBits, lImageSize, 1, pFile) != 1)
		{
        free(pBits);
        return NULL;
		}
    
    // Set OpenGL format expected
    switch(sDepth)
		{
        case 3:     // Most likely case
            *eFormat = GL_BGR_EXT;
            *iComponents = GL_RGB8;
            break;
        case 4:
            *eFormat = GL_BGRA_EXT;
            *iComponents = GL_RGBA8;
            break;
        case 1:
            *eFormat = GL_LUMINANCE;
            *iComponents = GL_LUMINANCE8;
            break;
		};
	
    
    // Done with File
    fclose(pFile);
	
    // Return pointer to image data
    return pBits;
	}



void Escena::cargaTextura(string nombre,int type,int wrap,int filter)
{
	int width,height,components;
	GLenum format;
	GLuint texid=0;    
    char ruta[MAX_NOMBRE];
	GLbyte *img;


	sprintf(ruta,"texturas/%s",nombre.c_str());
    img=gltLoadTGA(ruta, &width, &height, &components, &format);
	if(img==NULL) 
	{
		texturas[nombre]=0;
        printf("cargaTextura:Error al cargar la textura %s\n",nombre.c_str());		
	}
	else
	{

		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D,texid);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,filter);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,filter);

		gluBuild2DMipmaps(GL_TEXTURE_2D,components,width,height,format,GL_UNSIGNED_BYTE,img);	
		texturas[nombre]=texid;
		printf("cargaTextura:Textura %s cargada con id %d\n",nombre.c_str(),texid);
		free(img);
	}
}



void Escena::modificaMaterial(string nombreModelo,string nombreMaterial,string nombreTextura,GLuint wrap,GLfloat alpha)
{
   modificaMaterial(nombreModelo,nombreMaterial,nombreTextura,wrap,alpha,1.0,0.0,0.0);
}


void Escena::modificaMaterial(string nombreModelo,string nombreMaterial,string nombreTextura,GLuint wrap,GLfloat alpha,GLfloat escalaTex,GLfloat velTexS,GLfloat velTexT)
{
	//Modelo *modelo=(Modelo *)objetos[nombreModelo];
	Modelo *modelo=(Modelo *)objetos_totales[nombreModelo];
	modelo->desplazaTexturas=true;
	GLMmodel *m=modelo->modelo;
	int i;

	for(i=0; i<m->nummaterials; i++)
		if(strcmp(nombreMaterial.c_str(),m->materials[i].name)==0)
			break;
	if(i < m->nummaterials)
	{
		if(nombreTextura.length()>0)
		{
			if(texturas.count(nombreTextura)==0)
			  // La textura no ha sido cargada, se carga con parametros por default
			   cargaTextura(nombreTextura,GL_RGBA,wrap,TEX_FILTER);
			m->materials[i].textureid=texturas[nombreTextura];
			m->materials[i].wrapmode = wrap;
		}
		m->materials[i].alpha = alpha;
		m->materials[i].scale=escalaTex;
		m->materials[i].stexspeed=velTexS;
		m->materials[i].ttexspeed=velTexT;
        printf("modificaMaterial:El material %s del modelo %s fue modificado\n",nombreMaterial.c_str(),nombreModelo.c_str());		

	}
	else
		printf("modificaMaterial:El material %s no fue encontrado en el modelo %s\n",nombreMaterial.c_str(),nombreModelo.c_str());		
}


void Escena::defineNiebla(GLfloat inicio,GLfloat fin,GLfloat densidad,GLfloat r,GLfloat g,GLfloat b)
{
    inicioNiebla=inicio;
    finNiebla=fin;
    densidadNiebla=densidad;
	colorNiebla[0]=r;
	colorNiebla[1]=g;
	colorNiebla[2]=b;
	colorNiebla[3]=1.0;
}


void Escena::activaNiebla(bool activar)
{
	if(activar)
	{
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogf(GL_FOG_START, inicioNiebla);
		glFogf(GL_FOG_END, finNiebla);
		glFogf(GL_FOG_DENSITY, densidadNiebla);    		
		glFogfv(GL_FOG_COLOR, colorNiebla);
		
	}
	else
      glDisable(GL_FOG);
}

// Metodos de Camara


Camara::Camara(GLfloat xview,GLfloat yview,GLfloat zview,
               GLfloat xrot,GLfloat yrot,GLfloat zrot,
               GLfloat far,GLfloat near,GLfloat fovy)
{
  this->xview=xview;
  this->yview=yview;
  this->zview=zview;
  this->xrot=xrot;
  this->yrot=yrot;
  this->zrot=zrot;
  this->farCam=far;
  this->nearCam=near;
  this->fovy=fovy;
}


