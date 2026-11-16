//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul III - 03_02_animatie.cpp |
// ============================================
//
//	Program ce deseneaza un dreptunghi ce se deplaseaza stanga-dreapta si are un patrat ce orbiteaza in jurul sau, folosindu-se tehnicile MODERN OpenGL;
//	ELEMENTE DE NOUTATE:
//	- utilizeaza diverse transformari si compunerea acestora folosind biblioteca glm,
//	- functii pentru utilizarea mouse-ului;
//
//
//	Biblioteci

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>			//	Biblioteca necesara pentru afisarea in consola;
#include <cmath>
#include <vector>
// #include <windows.h>        //	Utilizarea functiilor de sistem Windows (crearea de ferestre, manipularea fisierelor si directoarelor);
#include <stdlib.h>         //  Biblioteci necesare pentru citirea shaderelor;
#include <stdio.h>
#include <GL/glew.h>        //  Define?te prototipurile functiilor OpenGL si constantele necesare pentru programarea OpenGL moderna; 
#include <GL/freeglut.h>    //	Include functii pentru: 
							//	- gestionarea ferestrelor si evenimentelor de tastatura si mouse, 
							//  - desenarea de primitive grafice precum dreptunghiuri, cercuri sau linii, 
							//  - crearea de meniuri si submeniuri;
#include "loadShaders.h"	//	Fisierul care face legatura intre program si shadere;
#include "glm/glm.hpp"		//	Bibloteci utilizate pentru transformari grafice;
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <soil2/SOIL2.h>		


using namespace std;

//  Identificatorii obiectelor de tip OpenGL;
GLuint
	CarVaoId,
	CarVboId,
	CarColorBufferId,
	RoadVaoId,
	RoadVboId,
	RoadColorBufferId,
	TreeVaoId,
	TreeVboId,
	ProgramId,
	myMatrixLocation,
	matrScaleLocation,
	matrTranslLocation,
	matrRotlLocation,
	codColLocation,
	texture,
	CarTextureBufferId;
//	Dimensiunile ferestrei de afisare;
GLfloat
	winWidth = 1200, winHeight = 900;
//	Variabile catre matricile de transformare;
glm::mat4
	myMatrix, resizeMatrix, matrTransl1, matrTransl2, matrTransl3, matrScale1, matrScale2, matrRot, matrDepl;

//	Variabila ce determina schimbarea culorii pixelilor in shader;
int codCol;			
//	Variabile pentru proiectia ortogonala;
float xMin = -450.f, xMax = 450.f, yMin = -300.f, yMax = 300.f;
//	Variabile pentru deplasarea pe axa Ox si pentru rotatie;
float middleCarY = 0.0, viteza = 0.6, Bbeta = 0.02, angle = -0.0f;

// Coordonate pentru masina din dreapta (acolo de decor)
float rightCarY = 0.0;

// Coordonate pentru masina care face depasirea
float passingCarX = 130.0, passingCarY = -200.0;

// -2 inseamna ca e in spatele masinii, -1 inseamna ca trece in depasire, 0 inseamna ca inca nu poate depasi masina, 1 inseamna ca se depaseste, 2 inseamna ca s-a terminat depasirea
int codDepasire = -2;
float vitezaDepasire = 3.0f * 2;

// Copaceii
int treeSegments = 24;
int treeVertexCount = treeSegments + 2;
int treeCount = 6;


// Miscarea masinii depasitoare
void AnimeazaDepasire(void){
	// Daca merg offscreen, they loop
	if (rightCarY > 600.0f){
		rightCarY = -200.0f;
	}

	if (middleCarY > 600.0f){
		middleCarY = -200.0f;
	}

	if (passingCarY > 600.0f){
		passingCarY = -500.0f;
		passingCarX = 130.0f;
		angle = 0.0f;
		codDepasire = -2;
	}

	// Codul din MoveUp
	middleCarY += viteza * 2.5;
	rightCarY += viteza * 3.1;

	float distanta = passingCarY - middleCarY;

	switch (codDepasire)
    {
		case -2:
			passingCarY += vitezaDepasire - 0.3f;
			if (distanta < -130.0f && distanta > -150.0f)			{
				codDepasire = -1;
			}
			break;

		case -1:
			passingCarY += vitezaDepasire;
			passingCarX -= 4.5f * 2;

			if (angle < 0.51f)
				angle += 0.05f;

			if (passingCarX <= 0.0f)
			{
				passingCarX = 0.0f;
				codDepasire = 0;
			}
			break;

        case 0:
            passingCarY += vitezaDepasire - 0.3f;
			if (angle > 0.05f)
				angle -= 0.05f;

            if (distanta > 130.0f && distanta < 150.0f)
            {
                codDepasire = 1;
            }
            break;

        case 1:
            passingCarY += vitezaDepasire;
            passingCarX += 4.5f * 2;

			middleCarY -= 0.9f;

			// Unghi gradual, frumos. deosebit
			if (angle > -0.51f)
            	angle -= 0.05f;

            if (passingCarX > 130.0f)
            {
                codDepasire = 2;
            }
            break;

        case 2:
            passingCarY += viteza + 2;
			
			if (angle < -0.00f)
			angle += 0.05f;
			
            break;
    }
	glutPostRedisplay();
}

//	Schimba sensul animatiei spre jos;
// void MoveDown(void)
// {
// 	middleCarY = middleCarY + viteza;
// 	if (middleCarY > 350.0)			//	Se asigura deplasarea stanga-dreapta pe Ox in limitele ecranului,
// 		viteza = -step;		//	mai intai efectuandu-se deplasarea spre DREAPTA;
// 	else if (middleCarY < -350.0)
// 		viteza = step;
// 	glutPostRedisplay();	//	Forteza redesenarea scenei;
// }

//	Schimba sensul animatiei spre sus;
// void MoveUp(void)
// {
// 	middleCarY = middleCarY + viteza;
// 	if (middleCarY < -350.0)			//	Se asigura deplasarea stanga-dreapta pe Ox in limitele ecranului,
// 		viteza = 0;		//	mai intai efectuandu-se deplasarea spre STANGA;
// 	else if (middleCarY > 600.0)
// 		viteza = -0;
// 	// angle = angle + Bbeta;	//	Se actualizeaza unghiul de rotatie constant (mentine orbitarea);
// 	glutPostRedisplay();	//	Forteza redesenarea scenei;
// }

//	Functie ce modifica deplasarea dreptunghiurilor in functie de apasarea butoanelor de pe mouse;
void UseMouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:			//	CLICK stanga => dreptunghiurile se misca in sus;
		glutIdleFunc(AnimeazaDepasire);
		break;
	case GLUT_RIGHT_BUTTON:			//	CLICK dreapta => dreptunghiurile se misca in nicaieri;
		glutIdleFunc(nullptr);
		break;
	default:
		break;
	}
}

//	Functia de incarcare a texturilor in program;
void LoadTexture(const char* texturePath)
{
	// Generarea unui obiect textura si legarea acestuia;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//	Desfasurarea imaginii pe orizontala/verticala in functie de parametrii de texturare;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Modul in care structura de texeli este aplicata pe cea de pixeli;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Incarcarea texturii si transferul datelor in obiectul textura; 
	int width, height;
	unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	
	// Eliberarea resurselor
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//  Crearea si compilarea obiectelor de tip shader;
//	Trebuie sa fie in acelasi director cu proiectul actual;
//  Shaderul de varfuri / Vertex shader - afecteaza geometria scenei;
//  Shaderul de fragment / Fragment shader - afecteaza culoarea pixelilor;
void CreateShaders(void)
{
	ProgramId = LoadShaders("03_02_Shader.vert", "03_02_Shader.frag");
	glUseProgram(ProgramId);
}

//  Se initializeaza un Vertex Buffer Object (VBO) pentru tranferul datelor spre memoria placii grafice (spre shadere);
//  In acesta se stocheaza date despre varfuri (coordonate, culori, indici, texturare etc.);
void CreateCarVBO(void)
{
	//  Coordonatele varfurilor;
	GLfloat Vertices[] = {
	//	Varfuri pentru masina din mijloc;
		-45.0f,  -245.0f, 0.0f, 1.0f,
		45.0f, -245.0f, 0.0f, 1.0f,
		44.0f,  -95.0f, 0.0f, 1.0f,
		-44.0f,  -95.0f, 0.0f, 1.0f,
	// Varfuri pt masina din dreapta
		85.0f,  -265.0f, 0.0f, 1.0f,
		175.0f, -265.0f, 0.0f, 1.0f,
		175.0f,  -115.0f, 0.0f, 1.0f,
		85.0f,  -115.0f, 0.0f, 1.0f,
	// Varfuri pt masina din stanga aka aia de depaseste
		-175.0f,  -295.0f, 0.0f, 1.0f,
		-85.0f, -295.0f, 0.0f, 1.0f,
		-85.0f,  -145.0f, 0.0f, 1.0f,
		-175.0f,  -145.0f, 0.0f, 1.0f,		
	
	};

	GLfloat Colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
	};

	GLfloat TextureCoords[] = {
    0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
    0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
    0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f
};

	//  Transmiterea datelor prin buffere;

	//  Se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO;
	glGenVertexArrays(1, &CarVaoId);                                                   //  Generarea VAO si indexarea acestuia catre variabila VaoId;
	glBindVertexArray(CarVaoId);

	//  Se creeaza un buffer pentru VARFURI;
	glGenBuffers(1, &CarVboId);                                                        //  Generarea bufferului si indexarea acestuia catre variabila VboId;
	glBindBuffer(GL_ARRAY_BUFFER, CarVboId);                                           //  Setarea tipului de buffer - atributele varfurilor;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);      //  Punctele sunt "copiate" in bufferul curent;
	//  Se asociaza atributul (0 = coordonate) pentru shader;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &CarTextureBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, CarTextureBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextureCoords), TextureCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//  Se creeaza un buffer pentru CULOARE;
	glGenBuffers(1, &CarColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, CarColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	//  Se asociaza atributul (1 =  culoare) pentru shader;
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void CreateRoadVBO(void)
{
	//  Coordonatele varfurilor;
	GLfloat Vertices[] = {
	// Drumu in sine
		-200.0f, -300.0f, 0.0f, 1.0f,
		-200.0f, 300.0f, 0.0f, 1.0f,
		200.0f, 300.0f, 0.0f, 1.0f,
		200.0f, -300.0f, 0.0f, 1.0f,

	//	Varfuri pentru axe, adica liniile de pe drum
		-66.6f, 300.0f, 0.0f, 1.0f,
		-66.6f, -300.0f, 0.0f, 1.0f,
		66.6f, 300.0f, 0.0f, 1.0f,
		66.6f, -300.0f, 0.0f, 1.0f,
	};

	//  Culorile axelor;
	GLfloat Colors[] = {
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	//  Transmiterea datelor prin buffere;

	//  Se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO;
	glGenVertexArrays(1, &RoadVaoId);                                                   //  Generarea VAO si indexarea acestuia catre variabila VaoId;
	glBindVertexArray(RoadVaoId);

	//  Se creeaza un buffer pentru VARFURI;
	glGenBuffers(1, &RoadVboId);                                                        //  Generarea bufferului si indexarea acestuia catre variabila VboId;
	glBindBuffer(GL_ARRAY_BUFFER, RoadVboId);                                           //  Setarea tipului de buffer - atributele varfurilor;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);      //  Punctele sunt "copiate" in bufferul curent;
	//  Se asociaza atributul (0 = coordonate) pentru shader;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//  Se creeaza un buffer pentru CULOARE;
	glGenBuffers(1, &RoadColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, RoadColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	//  Se asociaza atributul (1 =  culoare) pentru shader;
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void CreateTreeVBO(void)
{	
	std::vector<glm::vec2> centers = {
		{-270.0f, -220.0f}, {270.0f, -140.0f},
		{-270.0f,   20.0f}, {270.0f,  100.0f},
		{-270.0f,  250.0f}, {270.0f,  260.0f}
	};

	float radius = 40.0f;
	std::vector<GLfloat> vertices;
	vertices.reserve(treeCount * treeVertexCount * 4);

	for (glm::vec2& center : centers)
	{
		vertices.insert(vertices.end(), {center.x, center.y, 0.0f, 1.0f});
		for (int i = 0; i <= treeSegments; ++i)
		{
			float angle = 2.0f * 3.1415926f * i / treeSegments;
			vertices.insert(vertices.end(), {center.x + radius * std::cos(angle), center.y + radius * std::sin(angle), 0.0f, 1.0f});
		}
	}

	glGenVertexArrays(1, &TreeVaoId);
	glBindVertexArray(TreeVaoId);

	glGenBuffers(1, &TreeVboId);
	glBindBuffer(GL_ARRAY_BUFFER, TreeVboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
		vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDisableVertexAttribArray(1);
}

//  Elimina obiectele de tip shader dupa rulare;
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

//  Eliminarea obiectelor de tip VBO dupa rulare;
void DestroyCarVBO(void)
{
	//  Eliberarea atributelor din shadere (pozitie, culoare, texturare etc.);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//  Stergerea bufferelor pentru varfuri, culori;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &CarColorBufferId);
	glDeleteBuffers(1, &CarVboId);
	glDeleteBuffers(1, &CarTextureBufferId);

	//  Eliberaea obiectelor de tip VAO;
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &CarVaoId);
}

void DestroyRoadVBO(void)
{
	//  Eliberarea atributelor din shadere (pozitie, culoare, texturare etc.);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//  Stergerea bufferelor pentru varfuri, culori;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &RoadColorBufferId);
	glDeleteBuffers(1, &RoadVboId);

	//  Eliberaea obiectelor de tip VAO;
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &RoadVaoId);
}

void DestroyTreeVBO(void)
{
	glBindVertexArray(TreeVaoId);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &TreeVboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &TreeVaoId);
}

//  Functia de eliberare a resurselor alocate de program;
void Cleanup(void)
{
	DestroyShaders();
	DestroyCarVBO();
	DestroyRoadVBO();
	DestroyTreeVBO();
}

//  Setarea parametrilor necesari pentru fereastra de vizualizare;
void Initialize(void)
{
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);		//  Culoarea de fond a ecranului;
	CreateCarVBO();								//  Trecerea datelor de randare spre bufferul folosit de shadere;
	CreateRoadVBO();
	CreateTreeVBO();
	CreateShaders();							//  Initilizarea shaderelor;
	LoadTexture("masina-removebg-preview.png");

	// Pentru transparenta
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	Instantierea variabilelor uniforme pentru a "comunica" cu shaderele;
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
}

//  Functia de desenarea a graficii pe ecran;
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);			//  Se curata ecranul OpenGL pentru a fi desenat noul continut;
	
	// TO DO: schimbati transformarile (de exemplu deplasarea are loc pe axa Oy sau pe o alta dreapta);
	resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);							//	"Aducem" scena la "patratul standard" [-1,1]x[-1,1];
	matrTransl1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, middleCarY, 0.0));		//	Translatie de-a lungul axei Oy;
	matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, rightCarY, 0.0));		
	matrTransl3 = glm::translate(glm::mat4(1.0f), glm::vec3(passingCarX, passingCarY, 0.0));		
	matrDepl = glm::translate(glm::mat4(1.0f), glm::vec3(0, 80.0, 0.0));		//	Translatie pentru patratul ROSU fata de patratul ALBASTRU;
	matrScale1 = glm::scale(glm::mat4(1.0f), glm::vec3(1.1, 0.3, 0.0));			//	Scalare a coordonatelor initiale; se obtine dreptunghiul ALABSTRU;
	matrScale2 = glm::scale(glm::mat4(1.0f), glm::vec3(0.25, 0.25, 0.0));		//	Scalare a coordonatelor initiale; se obtine patratul ROSU;
	matrRot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 1.0));	//	Roatie folosita la deplasarea patratului ROSU;

	
	//	Matricea de redimensionare (pentru elementele "fixe" - axele);
	myMatrix = resizeMatrix;
	
	// Road VAO
	glBindVertexArray(RoadVaoId);
	//	Transmiterea variabilelor uniforme pentru MATRICE DE TRANSFORMARE si COLORARE spre shadere;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	
	//  Functia de desenare primeste 3 argumente:
	//  - arg1 = tipul primitivei desenate,
	//  - arg2 = indicele primului punct de desenat din buffer,
	//  - arg3 = numarul de puncte consecutive de desenat;
	//	Culoarea drumului
	codCol = 0; // Negru
	glUniform1i(codColLocation, codCol);
	glLineWidth(3.0);
	glDrawArrays(GL_POLYGON, 0, 4);
	//	Culoarea;
	codCol = 1; // Alb
	glUniform1i(codColLocation, codCol);
	glLineWidth(6.0);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(15,0x07FF);
	glDrawArrays(GL_LINES, 4, 4);
	
	// Copacei
	codCol = 4;
	glBindVertexArray(TreeVaoId);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &resizeMatrix[0][0]);
	glUniform1i(codColLocation, codCol);
	for (int i = 0; i < treeCount; ++i)
	{
		glDrawArrays(GL_TRIANGLE_FAN, i * treeVertexCount, treeVertexCount);
	}

	
	//	Masinile

	// Activarea / legarea texturii active
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//	Transmiterea variabilei uniforme pentru texturare spre shaderul de fragmente;
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

	myMatrix = resizeMatrix * matrTransl1;
	codCol = 2; // Rosu
	// Car VAO
	glBindVertexArray(CarVaoId);
	//	Transmiterea variabilelor uniforme pentru MATRICE DE TRANSFORMARE si COLORARE spre shadere;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(codColLocation, codCol);
	glDrawArrays(GL_POLYGON, 0, 4);

	myMatrix = resizeMatrix * matrTransl2;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawArrays(GL_POLYGON, 4, 4);

	codCol = 3;
	glUniform1i(codColLocation, codCol);
	myMatrix = resizeMatrix * matrTransl3 * glm::translate(glm::mat4(1.0f), glm::vec3(-130.0f, -220.0f, 0.0f)) * matrRot * glm::translate(glm::mat4(1.0f), glm::vec3(130.0f, 220.0f, 0.0f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawArrays(GL_POLYGON, 8, 4);

	glutSwapBuffers();	//	Inlocuieste imaginea deseneata in fereastra cu cea randata; 
	glFlush();	//  Asigura rularea tuturor comenzilor OpenGL apelate anterior;
}

//	Punctul de intrare in program, se ruleaza rutina OpenGL;
int main(int argc, char* argv[])
{
	//  Se initializeaza GLUT si contextul OpenGL si se configureaza fereastra si modul de afisare;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);					//	Se folosesc 2 buffere (unul pentru afisare si unul pentru randare => animatii cursive) si culori RGB;
	glutInitWindowPosition(100, 100);								//  Pozitia initiala a ferestrei;
	glutInitWindowSize(winWidth, winHeight);									//  Dimensiunea ferestrei;
	glutCreateWindow("Depasire de masini");		//	Creeaza fereastra de vizualizare, indicand numele acesteia;

	//	Se initializeaza GLEW si se verifica suportul de extensii OpenGL modern disponibile pe sistemul gazda;
	//  Trebuie initializat inainte de desenare;

	glewInit();
	
	Initialize();						//  Setarea parametrilor necesari pentru fereastra de vizualizare; 
	glutDisplayFunc(RenderFunction);	//  Desenarea scenei in fereastra;
	glutMouseFunc(UseMouse);			//	Activarea utilizarii mouseului;
	glutCloseFunc(Cleanup);				//  Eliberarea resurselor alocate de program;

	//  Bucla principala de procesare a evenimentelor GLUT (functiile care incep cu glut: glutInit etc.) este pornita;
	//  Prelucreaza evenimentele si deseneaza fereastra OpenGL pana cand utilizatorul o inchide;

	glutMainLoop();

	return 0;
}

