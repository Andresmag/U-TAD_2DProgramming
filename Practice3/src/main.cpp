
#define LITE_GFX_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS


#include <glfw3.h>
#include <iostream>
#include <sstream>
#include <stb_image.h>
#include <vector>
#include "vec2.h"
#include "font.h"
#include "text.h"
#ifndef LITE
#include "litegfx.h"
#endif // !LITE

using namespace std;

template <typename T>
string stringFromNumber(T val) 
{
	ostringstream stream;
	stream << fixed << val;
	return stream.str();
}

// Funcion para cargar texturas
ltex_t* LoadTexture(const char* _pFileName)
{
	int iX, iY;
	unsigned char* pPixels = stbi_load(_pFileName, &iX, &iY, nullptr, 4);
	ltex_t* pText = ltex_alloc(iX, iY, 0);
	ltex_setpixels(pText, pPixels);
	stbi_image_free(pPixels);

	return pText;
}

int main() {

	int iWidth = 1200, iHeight = 800;
	
	// Inicializar glfw
	if (glfwInit()) 
	{
		// Leer tiempo transcurrido
		double deltaTime = glfwGetTime();

		// Crear ventana
		GLFWwindow* window = glfwCreateWindow(iWidth, iHeight, "Practica3", NULL, NULL);
		if (window)
		{
			// Crear contexto
			glfwMakeContextCurrent(window);

			// Iniciar para pintado
			lgfx_setup2d(iWidth, iHeight);

			///////////////// Datos //////////////////
			// Fuentes
			Font* pFonts[] = { Font::Load("data/Orange.ttf", 40), Font::Load("data/SFSlapstickComic.ttf", 40) };

			// Cadenas de texto
			vector<Text*> vTexts;
			/////////////////////////////////////////

			// Ocultar el cursor
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 

			// Bucle de pintado
			while (!glfwWindowShouldClose(window))
			{
				// Actualizar tiempo
				float fElapsedSeconds = static_cast<float>(glfwGetTime() - deltaTime);
				deltaTime = glfwGetTime();

				// Leer input

				///////////////////////// Actualizar logica ///////////////////////////
				// Actualizar tamanio de pintada
				int iNewWidth, iNewHeight;
				glfwGetWindowSize(window, &iNewWidth, &iNewHeight);
				if (iNewWidth != iWidth || iNewHeight != iHeight)
				{
					iWidth = iNewWidth;
					iHeight = iNewHeight;
					lgfx_setviewport(0, 0, iWidth, iHeight);
				}

				// Comprobar eleminación de textos
				unsigned int uCurrentIndex = 0;
				while (uCurrentIndex < vTexts.size())
				{
					if ((*vTexts[uCurrentIndex]).getPos().GetXCoor() + (*vTexts[uCurrentIndex]).getFont()->GetTextSize((*vTexts[uCurrentIndex]).getText()).GetXCoor() <= 0)
					{
						delete vTexts[uCurrentIndex];
						vTexts.erase(vTexts.begin() + uCurrentIndex);
					}
					else
					{
						uCurrentIndex++;
					}
				}

				// Actualizar posicion
				for (unsigned int uIndex = 0; uIndex < vTexts.size(); uIndex++)
				{
					(*vTexts[uIndex]).UpdatePosition(fElapsedSeconds);
				}

				// Comprobar creacion de textos
				if (rand() % 101 == 0)
				{
					unsigned int uCurrentFont = rand() % 2; // Fuente
					unsigned int uCurrentHeight = rand() % (iHeight + 1); // altura en la ventana
					Vec2 vTextNewPos(static_cast<float>(iWidth), static_cast<float>(uCurrentHeight)); // posicion de salida
					unsigned int uCurrentSpeed = rand() % 180 + 20; // velocidad
					float pCurrentColor[3]; // color rgb
					for (unsigned int uIndex = 0; uIndex < 3; ++uIndex)
					{
						pCurrentColor[uIndex] = static_cast<float>(rand() % 256) / 255.f; // normalizado
					}
					
					vTexts.push_back(new Text("Hello, world!", pFonts[uCurrentFont], vTextNewPos, uCurrentSpeed, pCurrentColor)); // añadir la nueva cadena con sus caracteristicas a la lista
				}
				///////////////////////////////////////////////////////////////////////

				// Limpiar el backbuffer
				lgfx_clearcolorbuffer(0,0,0);

				//////////////////////// Renderizar escena ////////////////////////////
				lgfx_setblend(BLEND_ALPHA);
				for (unsigned int uIndex = 0; uIndex < vTexts.size(); uIndex++)
				{
					lgfx_setcolor((*vTexts[uIndex]).getColor()[0], (*vTexts[uIndex]).getColor()[1], (*vTexts[uIndex]).getColor()[2], 1);
					(*vTexts[uIndex]).Draw();
				}
				///////////////////////////////////////////////////////////////////////

				// Intercambio de buffers
				glfwSwapBuffers(window);

				// Procesar eventos
				glfwPollEvents();
			}

			// Finalizar ejecucion
			glfwTerminate();
		}

	}
	
    return 0;
}
