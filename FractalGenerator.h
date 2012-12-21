#pragma once

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <vector>
#include <iostream>
#include <stdlib.h>

#include "ComplexNum.h"
#include "ComplexNumUtils.h"

using namespace std;

class FractalGenerator
{
public:
	static void OnStart();
	const static int SCREEN_WIDTH = 640;
	const static int SCREEN_HEIGHT = 480;
	
private:

	enum state{ Uninitialized, Running, Exiting};

	struct Vertex
	{
		float x;
		float y;
	};

	struct Color
	{
		float r, g, b;
	};

	struct Point
	{
		float x, y;
		Color c;
	};

	static bool OnInit();
	static void GenerateFractal();
	static Vertex ComplexToNormal(Vertex vertex);
	static Vertex NormalToComplex(Vertex vertex);

	static void OnLoop();
	static void OnEvent();
	static void OnUpdate();
	static void OnRender();

	static bool IsExiting();
	static void OnExit();

	static float _currentScreenWidth;
	static float _currentScreenHeight;
	static state _state;
	static SDL_Surface* _mainWindow;
	static SDL_Event _event;
	static vector<Color> _colors;
	static vector<Point> _points;

	static float _fracxstart;
	static float _fracxend;
	static float _fracystart;
	static float _fracyend;

	static float _pointPrecision;
	static float _colorPrecision;

	static float _scaleFactor;

	static float _xOffSet;
	static float _yOffSet;
	static float _moveStep;
	
	static float _startx;
	static float _starty;
	static float _currentx;
	static float _currenty;
	static bool _isMouseDown;
	static bool _isDraggingMouse;

	static unsigned int _vertexHandle;
	static unsigned int _colorHandle;
	static vector<float> _vertexBuffer;
	static vector<float> _colorBuffer;
};

