
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
#include "sprite.h"
#ifndef LITE
#include "litegfx.h"
#endif // !LITE

using namespace std;

// Funcion para imprimir textos
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

// Funcion para actualizar el tamaño de la ventana
void updateViewportSize(GLFWwindow* _Window, int& _iWidth_, int& _iHeight_)
{
	int iNewWidth, iNewHeight;
	glfwGetWindowSize(_Window, &iNewWidth, &iNewHeight);
	if (iNewWidth != _iWidth_ || iNewHeight != _iHeight_)
	{
		_iWidth_ = iNewWidth;
		_iHeight_ = iNewHeight;
		lgfx_setviewport(0, 0, _iWidth_, _iHeight_);
	}
}

// Funcion para obtener las coordenadas del raton
Vec2 getMouseCoor(GLFWwindow* _Window)
{
	double dXCursor = 0;
	double dYCursor = 0;
	glfwGetCursorPos(_Window, &dXCursor, &dYCursor);
	return Vec2(static_cast<float>(dXCursor), static_cast<float>(dYCursor));
}

// Funcion update del sprite de la abeja
void updateSpritePosition(Sprite& _sValue, float _fDeltaTime)
{
	_sValue.setCurrentFrame(_sValue.getCurrentFrame() + 1);
	if (_sValue.getCurrentFrame() >= _sValue.getFps())
	{
		_sValue.setCurrentFrame(0);
	}

	Vec2 vMousePos = getMouseCoor(static_cast<GLFWwindow*>(_sValue.getUserData()));

	if (_sValue.getPosition().Distance(vMousePos) > 5)
	{
		Vec2 vDir = vMousePos - _sValue.getPosition();
		vDir.Normalize();
		vDir = Vec2(vDir.GetXCoor() * 128 * _fDeltaTime, vDir.GetYCoor() * 128 * _fDeltaTime); // Escalamos a la velocidad
		_sValue.setPosition(_sValue.getPosition() + vDir);

		if (_sValue.getPosition().GetXCoor() < vMousePos.GetXCoor() && _sValue.getAngle() > -15)
		{
			_sValue.setAngle(_sValue.getAngle() - 32 * _fDeltaTime);
		}
		else if (_sValue.getPosition().GetXCoor() > vMousePos.GetXCoor() && _sValue.getAngle() < 15)
		{
			_sValue.setAngle(_sValue.getAngle() + 32 * _fDeltaTime);
		}
	}
	else
	{
		if (_sValue.getAngle() < 0)
		{
			_sValue.setAngle(_sValue.getAngle() + 32 * _fDeltaTime);
		}
		else if (_sValue.getAngle() > 0)
		{
			_sValue.setAngle(_sValue.getAngle() - 32 * _fDeltaTime);
		}
	}
}

int main() {

	int iWidth = 1200, iHeight = 800;
	
	// Inicializar glfw
	if (glfwInit()) 
	{
		// Leer tiempo transcurrido
		double previousTime = glfwGetTime();

		// Crear ventana
		GLFWwindow* window = glfwCreateWindow(iWidth, iHeight, "Practica4", NULL, NULL);
		if (window)
		{
			// Crear contexto
			glfwMakeContextCurrent(window);

			// Iniciar para pintado
			lgfx_setup2d(iWidth, iHeight);

			///////////////// Datos //////////////////
			// Cargar sprite abeja
			Sprite* sAbeja = new Sprite(LoadTexture("data/bee_anim.png"), 8);
			sAbeja->setFps(8);
			sAbeja->setPosition(Vec2(50, 50));
			sAbeja->setCallback(updateSpritePosition);
			sAbeja->setUserData(static_cast<void*>(window));
			/////////////////////////////////////////

			// Ocultar el cursor
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 

			float fElapsedSeconds = 0;
			// Bucle de pintado
			while (!glfwWindowShouldClose(window))
			{
				// Actualizar tiempo
				float currentTime = glfwGetTime();
				fElapsedSeconds += currentTime - previousTime;
				previousTime = currentTime;

				// Leer input

				///////////////////////// Actualizar logica ///////////////////////////
				// Actualizar tamanio de pintada
				updateViewportSize(window, iWidth, iHeight);

				while (fElapsedSeconds >= 1.f / 8.f)
				{
					sAbeja->update(fElapsedSeconds, getMouseCoor(window));
					fElapsedSeconds -= 1.f / 8.f;
				}
				///////////////////////////////////////////////////////////////////////

				// Limpiar el backbuffer
				lgfx_clearcolorbuffer(0, 0, 0);

				//////////////////////// Renderizar escena ////////////////////////////
				sAbeja->draw();
				///////////////////////////////////////////////////////////////////////

				// Intercambio de buffers
				glfwSwapBuffers(window);

				// Procesar eventos
				glfwPollEvents();
			}

			// Finalizar ejecucion
			glfwTerminate();
			delete sAbeja;
		}
		
	}
	
    return 0;
}
