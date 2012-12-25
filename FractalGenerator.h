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

	static state _currentAppState;
	static SDL_Surface* _appMainWindow;
	static SDL_Event _currentAppEvent;

	static vector<Color> _fracColors;
	static vector<Point> _fracNormalPoints;

	static Vertex _portionStart;
	static Vertex _portionEnd;

	static Vertex _fracStart;
	static Vertex _fracEnd;
	static Vertex _fracNormalCenter;
	 
	static float _pointPrecision;
	static float _colorPrecision;

	static float _scaleFactor;

	static Vertex _pointOffSet;
	static float _offSetStep;
	
	static Vertex _mouseStart;
	static Vertex _mouseEnd;
	static bool _isMouseDown;
	static bool _isDraggingMouse;

	static unsigned int _vertexHandle;
	static unsigned int _colorHandle;
	static vector<float> _vertexBuffer;
	static vector<float> _colorBuffer;
};

