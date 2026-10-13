//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul II - 02_01_Shader.frag |
// ======================================
// 
//  Shaderul de fragment / Fragment shader - afecteaza culoarea pixelilor;
//

#version 330

//	Variabile de intrare (dinspre Shader.vert);
in vec4 ex_Color;

//	Variabile de iesire	(spre framebuffer / bufferul de cadru);
out vec4 out_Color;		//	Culoarea actualizata;

//	Variabilele uniforme;
uniform int codColShader;

//	Actualizeaza culoarea in functie de codColShader;
void main(void)
  {	
	switch ( codColShader ){
		case 0:
			out_Color = vec4 (1.0, 0.0, 1.0, 0.0);
			break;

		case 1:
			out_Color = vec4 (1.0, 0.0, 0.0, 0.0);
			break;
		default:
			out_Color = ex_Color;
			break;
	}
  }
 