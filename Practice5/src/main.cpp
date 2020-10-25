
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

#include "rectCollider.h"

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

// Funcion para actualizar el sprite que sigue la posicion del raton
void updateMouseSprite(Sprite& _sValue, float _fDeltaTime)
{
	_sValue.setPosition(getMouseCoor(static_cast<GLFWwindow*>(_sValue.getUserData())));
	_sValue.setCollisionType(_sValue.getCollisionType());
}

// Funcion para actualizar el sprite de la pelota
void updateSpriteOscillating(Sprite& _sValue, float _fDeltaTime)
{
	float newScale = 0.1 * sin((2*PI)/((0.1/0.25)*4) * glfwGetTime()) + 1;
	_sValue.setScale(Vec2(newScale, newScale));
	_sValue.setCollisionType(_sValue.getCollisionType());
}

int main() {

	int iWidth = 1200, iHeight = 800;
	
	// Inicializar glfw
	if (glfwInit()) 
	{
		// Leer tiempo transcurrido
		double previousTime = glfwGetTime();

		// Crear ventana
		GLFWwindow* window = glfwCreateWindow(iWidth, iHeight, "Practica5", NULL, NULL);
		if (window)
		{
			// Crear contexto
			glfwMakeContextCurrent(window);

			// Iniciar para pintado
			lgfx_setup2d(iWidth, iHeight);

			///////////////// Datos //////////////////
			// Sprites
			vector<ltex_t*> vTexs = { LoadTexture("data/ball.png"), LoadTexture("data/bee.png"), LoadTexture("data/box.png"), LoadTexture("data/circle.png"), LoadTexture("data/rect.png") };
			vector<Sprite*> vSprites = { new Sprite(vTexs[0]), new Sprite(vTexs[1]), new Sprite(vTexs[2]), new Sprite(vTexs[3])};

			// Ball Sprite
			vSprites[0]->setPosition(Vec2(static_cast<float>(iWidth) / 4.f, static_cast<float>(iHeight) / 2.f));
			//vSprites[0]->setPivot(Vec2(0.0f, 0.0f));
			vSprites[0]->setCollisionType(Sprite::COLLISION_CIRCLE);
			vSprites[0]->setFps(1);
			vSprites[0]->setCallback(updateSpriteOscillating);
			
			// Bee Sprite
			vSprites[1]->setPosition(Vec2(static_cast<float>(iWidth) / 2.f, static_cast<float>(iHeight) / 2.f));
			vSprites[1]->setCollisionType(Sprite::COLLISION_PIXELS);
			vSprites[1]->setFps(1);
			
			// Box Sprite
			vSprites[2]->setPosition(Vec2(3.f * static_cast<float>(iWidth) / 4.f, static_cast<float>(iHeight) / 2.f));
			vSprites[2]->setCollisionType(Sprite::COLLISION_RECT);
			vSprites[2]->setFps(1);
			vSprites[2]->setCallback(updateSpriteOscillating);
			//vSprites[2]->setPivot(Vec2(0.0f, 0.0f));

			// Mouse Sprite
			vSprites[3]->setPosition(getMouseCoor(window));
			vSprites[3]->setCollisionType(Sprite::COLLISION_CIRCLE);
			vSprites[3]->setFps(1);
			vSprites[3]->setUserData(static_cast<void*>(window));
			vSprites[3]->setCallback(updateMouseSprite);
			/////////////////////////////////////////

			// Ocultar el cursor
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 

			double fElapsedSeconds = 0;
			// Bucle de pintado
			while (!glfwWindowShouldClose(window))
			{
				// Actualizar tiempo
				double currentTime = glfwGetTime();
				fElapsedSeconds = currentTime - previousTime;
				previousTime = currentTime;

				// Leer input
				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				{
					vSprites[3]->setTexture(vTexs[3]);
					vSprites[3]->setCollisionType(Sprite::COLLISION_CIRCLE);
				}
				else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				{
					vSprites[3]->setTexture(vTexs[4]);
					vSprites[3]->setCollisionType(Sprite::COLLISION_RECT);
				}
				else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
				{
					vSprites[3]->setTexture(vTexs[1]);
					vSprites[3]->setCollisionType(Sprite::COLLISION_PIXELS);
				}

				// Limpiar el backbuffer
				lgfx_clearcolorbuffer(0, 0, 0);

				//////////////////////// Renderizar escena ////////////////////////////
				// Pintar Sprites
				for each (Sprite * sprite in vSprites)
				{
					sprite->draw();
				}
				///////////////////////////////////////////////////////////////////////

				
				///////////////////////// Actualizar logica ///////////////////////////
				// Actualizar tamanio de pintada
				updateViewportSize(window, iWidth, iHeight);

				// Actualizar sprites
				for each (Sprite* sprite in vSprites)
				{
					sprite->setColor(1, 1, 1, 1);
					sprite->update(static_cast<float>(fElapsedSeconds));
				}

				// Comprobar colisiones
				for (vector<Sprite*>::iterator it = vSprites.begin(); it != vSprites.end(); ++it)
				{
					if ((*it)->getCollider() != nullptr) {
						for (vector<Sprite*>::iterator jt = it + 1; jt != vSprites.end(); ++jt)
						{
							if ((*jt)->getCollider() != nullptr)
							{
								if ((*it)->collides(*(*jt)))
								{
									(*it)->setColor(1, 0, 0, 1);
									(*jt)->setColor(1, 0, 0, 1);
								}
								
							}
						}
					}
				}

				//printf("Mouse X: %f, Mouse Y: %f\n", getMouseCoor(window).GetXCoor(), getMouseCoor(window).GetYCoor());
				///////////////////////////////////////////////////////////////////////

				


				// Intercambio de buffers
				glfwSwapBuffers(window);

				// Procesar eventos
				glfwPollEvents();
			}

			// Eliminamos la memoria reservada
			for each (Sprite* sprite in vSprites)
			{
				delete sprite;
			}
			vSprites.clear();
			for each (ltex_t * tex in vTexs)
			{
				delete tex;
			}
			vTexs.clear();

			// Finalizar ejecucion
			glfwTerminate();
		}

	}
	
    return 0;
}
