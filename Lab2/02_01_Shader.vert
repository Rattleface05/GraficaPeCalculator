//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul II - 02_01_Shader.vert |
// ======================================
// 
//  Shaderul de varfuri / Vertex shader - afecteaza geometria scenei; 
//

#version 330

//  Variabile de intrare (dinspre programul principal);
layout (location = 0) in vec4 in_Position;     //  Se preia din buffer de pe prima pozitie (0) atributul care contine coordonatele;
layout (location = 1) in vec4 in_Color;        //  Se preia din buffer de pe a doua pozitie (1) atributul care contine culoarea;

// Uniforme
uniform int codColVert;


//  Variabile de iesire;
// out vec4 gl_Position;   //  Transmite pozitia actualizata in banda grafica (graphics pipeline);
out vec4 ex_Color;      //  Transmite culoarea (de modificat in Shader.frag); 


void main(void)
  {
    gl_Position = in_Position;
    switch (codColVert) {
      case 0:
        ex_Color = in_Color;
        break;
      case 1: 
        ex_Color = vec4(1.0,1.0,0.0,0.0);
        break;
      default:
			  ex_Color = vec4(0.0, 1.0, 1.0, 0.0);
			  break;
    } 
    
   } 
 