
#define LITE_GFX_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#define _CRT_SECURE_NO_WARNINGS


#include <glfw3.h>
#include <iostream>
#include <sstream>

#ifndef STB_IMAGE_H
#define STB_IMAGE_H
#include <stb_image.h>
#endif

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

// Funcion para actualizar el tamaño de la ventana
void updateViewportSize(GLFWwindow* _Window, Vec2& _WindowSize_)
{
	int iNewWidth, iNewHeight;
	glfwGetWindowSize(_Window, &iNewWidth, &iNewHeight);
	if (iNewWidth != _WindowSize_.GetXCoor() || iNewHeight != _WindowSize_.GetYCoor())
	{
		_WindowSize_.SetXCoor(iNewWidth);
		_WindowSize_.SetYCoor(iNewHeight);
		lgfx_setviewport(0, 0, static_cast<int>(_WindowSize_.GetXCoor()), static_cast<int>(_WindowSize_.GetYCoor()));
		lgfx_setresolution(static_cast<int>(_WindowSize_.GetXCoor()), static_cast<int>(_WindowSize_.GetYCoor()));
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

void updatePlayerSprite(Sprite& _sValue, float _fDeltaTime)
{
	_sValue.setTime(_sValue.getTime() + _fDeltaTime);
	float currentFrame = static_cast<int>(_sValue.getTime() / (1.0f / 8.0f)) % _sValue.getFps();
	_sValue.setCurrentFrame(currentFrame);
}

int main() {

	Vec2 vWindowSize(800.f, 600.f);
	
	// Inicializar glfw
	if (glfwInit()) 
	{
		// Leer tiempo transcurrido
		double previousTime = glfwGetTime();

		// Crear ventana
		GLFWwindow* window = glfwCreateWindow(vWindowSize.GetXCoor(), vWindowSize.GetYCoor(), "Practica7", NULL, NULL);
		if (window)
		{
			// Crear contexto
			glfwMakeContextCurrent(window);

			///////////////// Datos //////////////////
			// Textures
			vector<ltex_t*> vTextures = { LoadTexture("data/clouds.png"), LoadTexture("data/idle.png"), LoadTexture("data/run.png") };

			// Player sprite
			Sprite* playerSprite = new Sprite(vTextures[1]);
			playerSprite->setPivot(Vec2(0, 0));
			playerSprite->setPosition(Vec2(128, 960));
			playerSprite->setFps(1);
			playerSprite->setCollisionType(Sprite::COLLISION_RECT);
			playerSprite->setCallback(updatePlayerSprite);

			// Player control
			int direction = 0;
			bool bJump = false;
			float jumpForce = -0.4;
			bool bFalling = false;
			float fJumpingTime = 0;
			
			// Crear mundo
			World* world = new World(0.9f, 0.85f, 0.9f, vTextures[0]);
			world->loadMap("data/map.tmx", 1);
			vWindowSize = world->getMapSize();
			glfwSetWindowSize(window, static_cast<int>(vWindowSize.GetXCoor()), static_cast<int>(vWindowSize.GetYCoor()));

			// Añadir Sprite
			world->addSprite(*playerSprite);

			// Variable para controlar la posicion del raton en coordenadas del mundo
			Vec2 mousePosWorldCoord(0, 0);
			/////////////////////////////////////////
			
			// Iniciar para pintado
			lgfx_setup2d(static_cast<int>(vWindowSize.GetXCoor()), static_cast<int>(vWindowSize.GetYCoor()));

			// Ocultar el cursor
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 

			double fElapsedSeconds;
			// Bucle de pintado
			while (!glfwWindowShouldClose(window))
			{
				// Actualizar tiempo
				double currentTime = glfwGetTime();
				fElapsedSeconds = currentTime - previousTime;
				previousTime = currentTime;

				// Leer input
				if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				{
					if (direction <= 0)
					{
						direction = 1;
						playerSprite->setTexture(vTextures[2], 6);
						playerSprite->setFps(6);
						playerSprite->setTime(0);
						playerSprite->setScale(Vec2(direction, 1));
					}

					world->moveSprite(*playerSprite, Vec2(0.25, 0));
				}
				else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				{
					if (direction >= 0)
					{
						direction = -1;
						playerSprite->setTexture(vTextures[2], 6);
						playerSprite->setFps(6);
						playerSprite->setTime(0);
						playerSprite->setScale(Vec2(direction, 1));
					}

					world->moveSprite(*playerSprite, Vec2(-0.25, 0));
				}
			    else
			    {
					if (direction != 0)
					{
						direction = 0;
						playerSprite->setTexture(vTextures[1]);
						playerSprite->setFps(1);
						playerSprite->setTime(0);
					}
			    }

			    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				{
					if (!bJump && !bFalling)
					{
				        bJump = true;
						fJumpingTime = 0;    
					}
				}

				if (bJump)
				{
					if (fJumpingTime < 0.75)
					{
						if (world->moveSprite(*playerSprite, Vec2(0, jumpForce)))
						{
							fJumpingTime = 1000; // numeric_limits<float>::max();
						}
						fJumpingTime += fElapsedSeconds;
						jumpForce += 0.00025;
					}
				}

				///////////////////////// Actualizar logica ///////////////////////////
				// Actualizar tamanio de pintada
				updateViewportSize(window, vWindowSize);

				// Actualizar la posicion del raton en coordenadas de mundo
				//mousePosWorldCoord = world->getCameraPosition() + getMouseCoor(window);

				// Simulate gravity
				bool bGrounded = world->moveSprite(*playerSprite, Vec2(0, 0.15));
				if (bGrounded)
				{
					bJump = false;
					bFalling = false;
					jumpForce = -0.4;
				}
				else
				{
					bFalling = true;
				}

				// Actualizar world
				world->update(static_cast<float>(fElapsedSeconds));

				//////////////////////// Actualizar pintada /////////////////////////////
				world->draw(vWindowSize);
				///////////////////////////////////////////////////////////////////////


				// Intercambio de buffers
				glfwSwapBuffers(window);

				// Procesar eventos
				glfwPollEvents();
			}

			// Eliminamos la memoria reservada
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
