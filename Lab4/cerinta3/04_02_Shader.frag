//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul IV - 04_02_Shader.frag |
// ======================================
// 
//  Shaderul de fragment / Fragment shader - afecteaza culoarea pixelilor;
//

#version 330

//	Variabile de intrare (dinspre Shader.vert);
in vec4 ex_Color;
in vec2 tex_Coord;		//	Coordonata de texturare;

//	Variabile de iesire	(spre programul principal);
out vec4 out_Color;		//	Culoarea actualizata;

//  Variabile uniforme;
uniform sampler2D myTexture;
uniform int codCol;

void main(void)
  {  
    switch (codCol)
    {
      case 0:
        out_Color = ex_Color;
        break;
      case 1:
        out_Color = mix(texture(myTexture, tex_Coord), ex_Color, 0.2);	//	Amestecarea texturii si a culorii;
        break;
      default:
        out_Color = ex_Color;
        break;
    }
  }