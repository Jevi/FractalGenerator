#include "FractalGenerator.h"

SDL_Surface* FractalGenerator::_appMainWindow;
SDL_Event FractalGenerator::_currentAppEvent;
FractalGenerator::state FractalGenerator::_currentAppState = Uninitialized;
vector<FractalGenerator::Color> FractalGenerator::_fracColors;
vector<FractalGenerator::Point> FractalGenerator::_fracNormalPoints;
unsigned int FractalGenerator::_vertexHandle;
unsigned int FractalGenerator::_colorHandle;
vector<float> FractalGenerator::_vertexBuffer;
vector<float> FractalGenerator::_colorBuffer;

FractalGenerator::Vertex FractalGenerator::_fracNormalCenter;

FractalGenerator::Vertex FractalGenerator::_fracStart;
FractalGenerator::Vertex FractalGenerator::_fracEnd;

float FractalGenerator::_pointPrecision = 0.01f;
float FractalGenerator::_colorPrecision = 75;

float FractalGenerator::_offSetStep = 2;
FractalGenerator::Vertex FractalGenerator::_pointOffSet;

float FractalGenerator::_scaleFactor = 1;

FractalGenerator::Vertex FractalGenerator::_mouseStart;
FractalGenerator::Vertex FractalGenerator::_mouseEnd;

bool FractalGenerator::_isMouseDown = false;
bool FractalGenerator::_isDraggingMouse = false;

void FractalGenerator::OnStart()
{
	if (_currentAppState != Uninitialized)
	{
		return;
	}

	if (OnInit())
	{
		_currentAppState = Running;
		while(!IsExiting())
		{
			OnLoop();
		}
	}
	OnExit();
}

bool FractalGenerator::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	SDL_WM_SetCaption("Fractal Generator", NULL);

	if((_appMainWindow = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL)
	{
		return false;
	}
	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1);

	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapBuffers();
	
	_pointOffSet.x = 0;
	_pointOffSet.y = 0;

	_fracStart.x = -2;
	_fracStart.y = -2;
	_fracEnd.x = 2;
	_fracEnd.y = 2;

	_fracNormalCenter.x = (SCREEN_WIDTH / 2);
	_fracNormalCenter.y = (SCREEN_HEIGHT / 2);

	GenerateFractal();

	return true;
}

bool FractalGenerator::IsExiting()
{
	if (_currentAppState == Exiting)
	{
		return true;
	}
	return false;
}

void FractalGenerator::OnLoop()
{
	switch(_currentAppState)
	{
	case Running:
		{
			OnEvent();
			OnUpdate();
			OnRender();
			break;
		}
	}
}

void FractalGenerator::OnUpdate()
{
}

void FractalGenerator::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw fractal
	glPushMatrix();

	glTranslated((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0);
	glScaled(_scaleFactor, _scaleFactor, 1);
	glTranslated(-(SCREEN_WIDTH / 2), -(SCREEN_HEIGHT / 2), 0);
	glTranslated(_pointOffSet.x, _pointOffSet.y, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexHandle);
	glVertexPointer(2, GL_FLOAT, 0, 0L);

	glBindBuffer(GL_ARRAY_BUFFER, _colorHandle);
	glColorPointer(3, GL_FLOAT, 0, 0L);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_POINTS, 0, _vertexBuffer.size());
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	// Draw mouse selection
	if (_isMouseDown && _isDraggingMouse)
	{
		float ratio = (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT;
		float selectionWidth = (_mouseEnd.x - _mouseStart.x);
		float selectionHeight = selectionWidth / ratio;

		glPushMatrix();
		glColor3f(1, 1, 1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(_mouseStart.x, _mouseStart.y);
		glVertex2f(_mouseStart.x + selectionWidth, _mouseStart.y);
		glVertex2f(_mouseStart.x + selectionWidth, _mouseStart.y + selectionHeight);
		glVertex2f(_mouseStart.x, _mouseStart.y + selectionHeight);
		glEnd();
		glPopMatrix();
	}

	SDL_GL_SwapBuffers();
}

void FractalGenerator::OnExit()
{
	SDL_FreeSurface(_appMainWindow);
	glDeleteBuffers(1, &_vertexHandle);
	glDeleteBuffers(1, &_colorHandle);
	SDL_Quit();
}

void FractalGenerator::OnEvent()
{
	Uint8* keystate = SDL_GetKeyState(NULL);

	// Keyboard Events
	if (keystate[SDLK_ESCAPE])
	{
		_currentAppState = Exiting;
	}
	if(keystate[SDLK_a] || keystate[SDLK_LEFT])
	{
		_pointOffSet.x += _offSetStep / _scaleFactor;
	}
	if(keystate[SDLK_d] || keystate[SDLK_RIGHT])
	{
		_pointOffSet.x -= _offSetStep / _scaleFactor;
	}
	if(keystate[SDLK_w] || keystate[SDLK_UP])
	{
		_pointOffSet.y += _offSetStep / _scaleFactor;
	}
	if(keystate[SDLK_s] || keystate[SDLK_DOWN])
	{
		_pointOffSet.y -= _offSetStep / _scaleFactor;
	}
	if (keystate[SDLK_q])
	{
		_scaleFactor += 2;
	}
	else if (keystate[SDLK_e])
	{
		_scaleFactor -=2;
	}

	if (keystate[SDLK_r])
	{
		_scaleFactor = 1;
		_pointOffSet.x = 0;
		_pointOffSet.y = 0;
	}

	if (keystate[SDLK_p])
	{
		cout << "Scale: " << _scaleFactor << endl;
		cout << "xoffset: " << _pointOffSet.x << endl;
		cout << "yoffset: " << _pointOffSet.y << endl;
	}

	if (SDL_GetMouseState(NULL, NULL) &SDL_BUTTON_LEFT)
	{
		_isMouseDown = true;
	}

	while(SDL_PollEvent(&_currentAppEvent))
	{
		switch(_currentAppEvent.type)
		{
		case SDL_QUIT:
			_currentAppState = Exiting;
			break;
		case SDL_MOUSEMOTION:

			if (_isMouseDown && !_isDraggingMouse)
			{
				_mouseStart.x = _currentAppEvent.button.x;
				_mouseStart.y = _currentAppEvent.button.y;
				_mouseEnd.x = _mouseStart.x;
				_mouseEnd.y = _mouseStart.y;
				_isDraggingMouse = true;
			}
			else if (_isMouseDown && _isDraggingMouse)
			{
				_mouseEnd.x = _currentAppEvent.button.x;
				_mouseEnd.y = _currentAppEvent.button.y;
			}
			break;
		case SDL_MOUSEBUTTONUP:

			if (_currentAppEvent.button.button == SDL_BUTTON_LEFT && _isMouseDown && _isDraggingMouse)
			{
				_isMouseDown = false;
				_isDraggingMouse = false;

				// modify mouse coordinates to fit scaled image
				_mouseStart.x = _mouseStart.x;
				_mouseStart.y = _mouseStart.y;
				_mouseEnd.x = _mouseEnd.x;
				_mouseEnd.y = _mouseEnd.y;

				// get width and height of selection
				float ratio = (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT;
				float newFracWidth = (_mouseEnd.x - _mouseStart.x);
				float newFracHeight = newFracWidth / ratio;

				_fracNormalCenter.x = _mouseStart.x + (newFracWidth / 2);
				_fracNormalCenter.y = _mouseStart.y + (newFracHeight / 2);

				if (newFracWidth > 0 && newFracHeight > 0)
				{
					_scaleFactor = (SCREEN_WIDTH / newFracWidth) * _scaleFactor;

					Vertex starts;
					starts.x = _mouseStart.x;
					starts.y = _mouseStart.y;
					starts = NormalToComplex(starts);

					Vertex ends;
					ends.x = _mouseStart.x + newFracWidth;
					ends.y = _mouseStart.y + newFracHeight;
					ends = NormalToComplex(ends);

					// set new complex display dimensions
					_fracStart.x = starts.x;
					_fracStart.y = starts.y;
					_fracEnd.x = ends.x;
					_fracEnd.y = ends.y;

					_pointOffSet.x = (SCREEN_WIDTH / 2) - _fracNormalCenter.x;
					_pointOffSet.y = (SCREEN_HEIGHT / 2) - _fracNormalCenter.y;

					GenerateFractal();
				}
			}
			break;
		}
	}
}

void FractalGenerator::GenerateFractal()
{
	if (_fracNormalPoints.size() > 0)
	{
		cout << "\nRegenerating Fractal" << endl;
		_fracNormalPoints.clear();
		_vertexBuffer.clear();
		_colorBuffer.clear();
		glDeleteBuffers(1, &_vertexHandle);
		glDeleteBuffers(1, &_colorHandle);
	}

	if (_fracColors.size() == 0)
	{
		Color color;
		int maxColor = 256;
		for (float r = 0; r < maxColor; r += _colorPrecision)
		{
			for (float g = 0; g < maxColor; g += _colorPrecision)
			{
				for (float b = 0; b < maxColor; b += _colorPrecision)
				{
					color.r = r;
					color.g = g;
					color.b = b;
					_fracColors.push_back(color);
				}
			}
		}
	}

	ComplexNum c(0, 0);
	ComplexNum z(0, 0);
	Point currentNormalVertex;
	Vertex currentComplexVertex;
	int iterations;
	int maxIterations = _fracColors.size();
	_pointPrecision =  0.01f / _scaleFactor;

	for (float x = _fracStart.x; x < _fracEnd.x; x += _pointPrecision)
	{
		for (float y = _fracStart.y; y < _fracEnd.y; y += _pointPrecision)
		{
			iterations = 0;
			c = ComplexNum(x, y);
			z = ComplexNum(0, 0);

			while (ComplexNumUtils::Abs(z) < 2 && iterations < maxIterations)
			{
				z = z * z + c;
				++iterations;
			}

			currentComplexVertex.x = x;
			currentComplexVertex.y = y;
			currentComplexVertex = ComplexToNormal(currentComplexVertex);

			currentNormalVertex.x = currentComplexVertex.x;
			currentNormalVertex.y = currentComplexVertex.y;
			currentNormalVertex.c = _fracColors[_fracColors.size() % iterations];
			_fracNormalPoints.push_back(currentNormalVertex);
		}
	}

	for (unsigned int i = 0; i < _fracNormalPoints.size(); i++)
	{
		_vertexBuffer.push_back(_fracNormalPoints[i].x);
		_vertexBuffer.push_back(_fracNormalPoints[i].y);

		_colorBuffer.push_back(_fracNormalPoints[i].c.r);
		_colorBuffer.push_back(_fracNormalPoints[i].c.g);
		_colorBuffer.push_back(_fracNormalPoints[i].c.b);
	}

	// cout << "Center of Selection: " << _currentScreenWidth / 2 << " , " << _currentScreenHeight / 2 << endl;
	cout << "Start: " << _fracStart.x << " , " << _fracStart.y << endl;
	cout << "Current: " << _fracEnd.x << " , " << _fracEnd.y << endl;
	cout << "Zoom Factor: " << _scaleFactor << endl;

	cout << "Max Iterations: " << maxIterations << endl;
	cout << "Total Points: " << _fracNormalPoints.size() << endl;
	cout << "point buffer size: " << _vertexBuffer.size() << endl;
	cout << "color buffer size: " << _colorBuffer.size() << endl;

	glGenBuffers(1, &_vertexHandle);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexHandle);
	glBufferData(GL_ARRAY_BUFFER, _vertexBuffer.size() * sizeof(float), &_vertexBuffer.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &_colorHandle);
	glBindBuffer(GL_ARRAY_BUFFER, _colorHandle);
	glBufferData(GL_ARRAY_BUFFER, _colorBuffer.size() * sizeof(float), &_colorBuffer.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

FractalGenerator::Vertex FractalGenerator::ComplexToNormal(Vertex vertex)
{
	vertex.x = (vertex.x * (SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 2));
	vertex.y = (vertex.y * (SCREEN_HEIGHT / 4) + (SCREEN_HEIGHT / 2));
	return vertex;
}

FractalGenerator::Vertex FractalGenerator::NormalToComplex(Vertex vertex)
{
	vertex.x = (vertex.x - (SCREEN_WIDTH / 2)) / (SCREEN_WIDTH / 4);
	vertex.y = (vertex.y - (SCREEN_HEIGHT / 2)) / (SCREEN_HEIGHT / 4);
	return vertex;
}
