//
// ================================================
// | Grafica pe calculator                        |
// ================================================
// | Laboratorul III - 03_02_Shader.frag |
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

//	Variabilele uniforme;
uniform int codCol;
uniform sampler2D myTexture;



//	Actualizeaza culoarea in functie de codCol;
void main(void)
{
  vec4 texel = texture(myTexture, tex_Coord);

  switch (codCol)
  {
	case 0: 
		out_Color = vec4 (0.0, 0.0, 0.0, 1.0);
		break;
	case 1: 
		out_Color=vec4 (1.0, 1.0, 1.0, 1.0);
		break;
	case 2: 
		out_Color = vec4(mix(texel.rgb, vec3(1.0, 0.0, 0.0), 0.8), texel.a);
		break;
	case 3: 
		out_Color = vec4(mix(texel.rgb, vec3(0.0, 0.0, 1.0), 0.8), texel.a);
		break;
	case 4: 
		out_Color = vec4(mix(texel.rgb, vec3(0.2, 0.2, 0.2), 0.8), texel.a);
		break;
	case 5:
		out_Color = vec4(0.0, 0.35, 0.0, 1.0);
		break;
	default:
		out_Color = ex_Color;
		break;
  };
}
 