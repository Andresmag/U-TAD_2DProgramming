
#define LITE_GFX_IMPLEMENTATION

#include <litegfx.h>
#include <glfw3.h>
#include <iostream>
#include <sstream>
#include "vec2.h"

using namespace std;

template <typename T>
std::string stringFromNumber(T val) {
	std::ostringstream stream;
	stream << std::fixed << val;
	return stream.str();
}

int main() {
	
	int iWidth = 1200, iHeight = 800;

	// Inicializar glfw
	if (glfwInit()) 
	{
		// Leer tiempo transcurrido
		double deltaTime = glfwGetTime();

		// Crear ventana
		GLFWwindow* window = glfwCreateWindow(iWidth, iHeight, "Practica1", NULL, NULL);
		if (window)
		{
			// Crear contexto
			glfwMakeContextCurrent(window);

			// Iniciar para pintado
			lgfx_setup2d(iWidth, iHeight);

			// Datos
			Vec2 vWindowCentre(iWidth / 2, iHeight / 2);
			double dXCursor = 0;
			double dYCursor = 0;
			glfwGetCursorPos(window, &dXCursor, &dYCursor);
			Vec2 vCursorPos(static_cast<float>(dXCursor), static_cast<float>(dYCursor));
			Vec2 vCircle(100, 0);

			// Bucle de pintado
			while (!glfwWindowShouldClose(window))
			{
				// Actualizar tiempo
				float fElapsedSeconds = glfwGetTime() - deltaTime;
				deltaTime = glfwGetTime();

				// Leer input
				glfwGetCursorPos(window, &dXCursor, &dYCursor);

				// Actualizar logica
				int iNewWidth, iNewHeight;
				glfwGetWindowSize(window, &iNewWidth, &iNewHeight);
				if (iNewWidth != iWidth || iNewHeight != iHeight)
				{
					iWidth = iNewWidth;
					iHeight = iNewHeight;
					lgfx_setviewport(0, 0, iWidth, iHeight);
					vWindowCentre.SetCoords(iWidth / 2.f, iHeight / 2.f);
				}

				vCursorPos.SetCoords(static_cast<float>(dXCursor), static_cast<float>(dYCursor));
				float fAngle = 32 * fElapsedSeconds;
				float fXCircle = (vCircle.GetXCoor() * cos(fAngle * PI / 180) + vCircle.GetYCoor() * -sin(fAngle * PI / 180));
				float fYCircle = (vCircle.GetXCoor() * sin(fAngle * PI / 180) + vCircle.GetYCoor() * cos(fAngle * PI / 180));
				vCircle.SetCoords(fXCircle, fYCircle);

				// Update Title
				std::string title = "Distancia: "
					+ stringFromNumber(vWindowCentre.Distance(vCursorPos))
					+ " -- Angulo: "
					+ stringFromNumber(vCursorPos.Angle(vCircle + vCursorPos));
				glfwSetWindowTitle(window, title.c_str());

				// Limpiar el backbuffer
				lgfx_clearcolorbuffer(0,0,0);

				// Renderizar escena
				lgfx_setcolor(0, 0, 1, 0);
				lgfx_drawrect(vWindowCentre.GetXCoor() - 50, vWindowCentre.GetYCoor() - 50, 100, 100); // Cuadrado en el centro

				lgfx_setcolor(0, 1, 0, 0);
				lgfx_drawrect(vCursorPos.GetXCoor() - 10, vCursorPos.GetYCoor() - 10, 20, 20); // Cuadrado en la posicion del cursor

				lgfx_setcolor(1, 0, 0, 0);
				lgfx_drawoval(vCircle.GetXCoor() + vCursorPos.GetXCoor() - 25, vCircle.GetYCoor() + vCursorPos.GetYCoor() - 25, 50, 50); // Circulo dando vueltas alrededor del cursor


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
