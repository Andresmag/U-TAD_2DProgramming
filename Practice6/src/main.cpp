
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
#include "world.h"

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

// Funcion para actualizar el sprite de la abeja
void updateBeeSprite(Sprite& _sValue, float _fDeltaTime)
{
	_sValue.setTime(_sValue.getTime() + _fDeltaTime);
	float currentFrame = static_cast<int>(_sValue.getTime() / (1.0f / 8.0f)) % _sValue.getFps();
	_sValue.setCurrentFrame(currentFrame);
	/*if (_sValue.getCurrentFrame() >= _sValue.getFps())
	{
		_sValue.setCurrentFrame(0);
	}*/

	Vec2 vMousePos = *static_cast<Vec2*>(_sValue.getUserData());

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

	int iWidth = 800, iHeight = 600;
	
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
			// Textures
			vector<ltex_t*> vTextures = { LoadTexture("data/clouds.png"), LoadTexture("data/trees2.png"), LoadTexture("data/trees1.png"), LoadTexture("data/level.png"), LoadTexture("data/bee_anim.png") };
			
			// Sprites
			vector<Sprite*> vSprites = { new Sprite(vTextures[4], 8) };
			
			// Bee Sprite configuration
			vSprites[0]->setPosition(Vec2(300.0f, 300.0f));
			vSprites[0]->setFps(8);
			vSprites[0]->setCollisionType(Sprite::COLLISION_PIXELS);
			vSprites[0]->setCallback(updateBeeSprite);
			
			// Crear mundo
			World* world = new World(0.9f, 0.85f, 0.9f, vTextures[0], vTextures[1], vTextures[2], vTextures[3]);
			world->addSprite(*vSprites[0]);
			world->setScrollSpeed(0, Vec2(-16.0f, -8.0f));
			world->setScrollRatio(0, 0.4f);
			world->setScrollRatio(1, 0.6f);
			world->setScrollRatio(2, 0.8f);
			world->setScrollRatio(3, 1.0f);

			// Variable para controlar la posicion del raton en coordenadas del mundo
			Vec2 mousePosWorldCoord(0, 0);
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

				///////////////////////// Actualizar logica ///////////////////////////
				// Actualizar tamanio de pintada
				updateViewportSize(window, iWidth, iHeight);

				// Actualizar la posicion del raton en coordenadas de mundo
				mousePosWorldCoord = world->getCameraPosition() + getMouseCoor(window);
				vSprites[0]->setUserData(static_cast<void*>(&mousePosWorldCoord));

				// Actualizar world
				world->update(static_cast<float>(fElapsedSeconds));

				//////////////////////// Actualizar pintada /////////////////////////////
				world->draw(Vec2(iWidth, iHeight));
				///////////////////////////////////////////////////////////////////////


				// Intercambio de buffers
				glfwSwapBuffers(window);

				// Procesar eventos
				glfwPollEvents();
			}

			// Eliminamos la memoria reservada
			for (Sprite* sprite : vSprites)
			{
				world->removeSprite(*sprite);
				delete sprite;
			}
			vSprites.clear();
			
			for (ltex_t * tex : vTextures)
			{
				delete tex;
			}
			vTextures.clear();

			delete world;

			// Finalizar ejecucion
			glfwTerminate();
		}

	}
	
    return 0;
}
