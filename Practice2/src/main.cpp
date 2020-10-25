
#define LITE_GFX_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <litegfx.h>
#include <glfw3.h>
#include <iostream>
#include <sstream>
#include "vec2.h"
#include <stb_image.h>

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
		GLFWwindow* window = glfwCreateWindow(iWidth, iHeight, "Practica2", NULL, NULL);
		if (window)
		{
			// Crear contexto
			glfwMakeContextCurrent(window);

			// Iniciar para pintado
			lgfx_setup2d(iWidth, iHeight);

			///////////////// Datos //////////////////
			// Cursor 
			double dXCursor = 0;
			double dYCursor = 0;
			glfwGetCursorPos(window, &dXCursor, &dYCursor);
			Vec2 vCursorPos(static_cast<float>(dXCursor), static_cast<float>(dYCursor));

			// Cargar fondo
			ltex_t* pFondoText = LoadTexture("data/wall.jpg");

			// Cargar rejilla
			ltex_t* pGrilleText = LoadTexture("data/grille.png");

			// Cargar llama
			ltex_t* pLlamaText = LoadTexture("data/fire.png");

			// Cargar luz
			ltex_t* pLuzText = LoadTexture("data/light.png");

			// Angulo de rotacion de la llama
			float fCurrenteAngle = 0;
			int iAngleSign = 1;

			// Escalador de la llama
			float fCurrentSize = 1;
			int iSizeSign = 1;
			/////////////////////////////////////////

			// Ocultar el cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 

			// Bucle de pintado
			while (!glfwWindowShouldClose(window))
			{
				// Actualizar tiempo
				float fElapsedSeconds = static_cast<float>(glfwGetTime() - deltaTime);
				deltaTime = glfwGetTime();

				// Leer input
				glfwGetCursorPos(window, &dXCursor, &dYCursor);

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

				// Actualizar rotacion de la llama
				if (fCurrenteAngle >= 10)
				{
					iAngleSign = -1;
				}
				else if (fCurrenteAngle <= -10)
				{
					iAngleSign = 1;
				}
				fCurrenteAngle += iAngleSign * 10 * fElapsedSeconds;

				// Actualizar tamanio de la llama
				if (fCurrentSize >= 1.2f)
				{
					iSizeSign = -1;
				}
				else if (fCurrentSize <= 0.8f)
				{
					iSizeSign = 1;
				}
				fCurrentSize += iSizeSign * 0.5f * fElapsedSeconds;

				// Actualizar coordenadas del raton
				vCursorPos.SetCoords(static_cast<float>(dXCursor), static_cast<float>(dYCursor));
				///////////////////////////////////////////////////////////////////////

				// Limpiar el backbuffer
				lgfx_clearcolorbuffer(0,0,0);

				//////////////////////// Renderizar escena ////////////////////////////
				// Renderizar fondo
				lgfx_setblend(BLEND_SOLID);
				ltex_drawrotsized(pFondoText, 0, 0, 0, 0, 0, iWidth, iHeight, 0, 0, iWidth / pFondoText->width, iHeight / pFondoText->height);

				// Renderizar llama
				lgfx_setblend(BLEND_ADD);
				ltex_drawrotsized(pLlamaText, vCursorPos.GetXCoor(), vCursorPos.GetYCoor(), fCurrenteAngle, 0.5, 0.5, pLlamaText->width * fCurrentSize, pLlamaText->height * fCurrentSize, 0, 0, 1, 1);

				// Rederizar rejilla
				lgfx_setblend(BLEND_ALPHA);
				ltex_drawrotsized(pGrilleText, 0, 0, 0, 0, 0, iWidth, iHeight, 0, 0, iWidth / pGrilleText->width, iHeight / pGrilleText->height);

				// Renderizar luz
				lgfx_setblend(BLEND_MUL);
				ltex_draw(pLuzText, vCursorPos.GetXCoor() - pLuzText->width/2, vCursorPos.GetYCoor() - pLuzText->height/2);

				// Renderizar bordes de la luz
				float fAnchoIzquierdo = vCursorPos.GetXCoor() - pLuzText->width / 2;
				float fAnchoDerecho = vCursorPos.GetXCoor() + pLuzText->height / 2;
				float fAltoArriba = vCursorPos.GetYCoor() - pLuzText->height / 2;
				float fAltoAbajo = vCursorPos.GetYCoor() + pLuzText->height / 2;
				lgfx_setcolor(0, 0, 0, 0);
				lgfx_drawrect(0, 0, fAnchoIzquierdo, iHeight); // Rectangulo del lateral izquierdo
				lgfx_drawrect(fAnchoIzquierdo, 0, iWidth - fAnchoIzquierdo, fAltoArriba); // Rectangulo de la parte superior
				lgfx_drawrect(fAnchoDerecho, fAltoArriba, iWidth - fAnchoDerecho, iHeight - fAltoArriba); // Rectangulo del lateral derecho
				lgfx_drawrect(fAnchoIzquierdo, fAltoAbajo, iWidth - fAnchoIzquierdo - (iWidth - fAnchoDerecho), iHeight - fAltoAbajo); // Rectangulo de la parte inferior

				// Volvemos a poner el color a transparente para que se vean las texturas
				lgfx_setcolor(1, 1, 1, 1);
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
