#include "FractalGenerator.h"

float FractalGenerator::_currentScreenWidth = SCREEN_WIDTH;
float FractalGenerator::_currentScreenHeight = SCREEN_HEIGHT;
SDL_Surface* FractalGenerator::_mainWindow;
SDL_Event FractalGenerator::_event;
FractalGenerator::state FractalGenerator::_state = Uninitialized;
vector<FractalGenerator::Color> FractalGenerator::_colors;
vector<FractalGenerator::Point> FractalGenerator::_points;
unsigned int FractalGenerator::_vertexHandle;
unsigned int FractalGenerator::_colorHandle;
vector<float> FractalGenerator::_vertexBuffer;
vector<float> FractalGenerator::_colorBuffer;

float FractalGenerator::_fracxstart = -2;
float FractalGenerator::_fracxend = 2;
float FractalGenerator::_fracystart = -2;
float FractalGenerator::_fracyend = 2;

float FractalGenerator::_pointPrecision = 0.01f;
float FractalGenerator::_colorPrecision = 75;

float FractalGenerator::_moveStep = 0.5;
float FractalGenerator::_xOffSet = 1;
float FractalGenerator::_yOffSet = 1;

float FractalGenerator::_scaleFactor = 1;

float FractalGenerator::_startx = 0;
float FractalGenerator::_starty = 0;
float FractalGenerator::_currentx = 0;
float FractalGenerator::_currenty = 0;
bool FractalGenerator::_isMouseDown = false;
bool FractalGenerator::_isDraggingMouse = false;

void FractalGenerator::OnStart()
{
	if (_state != Uninitialized)
	{
		return;
	}

	if (OnInit())
	{
		_state = Running;
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

	if((_mainWindow = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL)
	{
		return false;
	}
	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 1, -1);

	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapBuffers();

	GenerateFractal();

	return true;
}

bool FractalGenerator::IsExiting()
{
	if (_state == Exiting)
	{
		return true;
	}
	return false;
}

void FractalGenerator::OnLoop()
{
	switch(_state)
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
	/*cout << "Start: " << _startx << " , " << _starty << endl;
	cout << "Current: " << _currentx << " , " << _currenty << endl;
	cout << "Zoom Factor: " << _scaleFactor << endl;*/
}

void FractalGenerator::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Vertex center;
	center.x = (SCREEN_WIDTH / 2);
	center.y = (SCREEN_HEIGHT / 2);
	// Draw fractal
	glPushMatrix();

	glTranslated(center.x, center.y, 0);
	glScaled(_scaleFactor, _scaleFactor, 1);
	glTranslated(-center.x, -center.y, 0);

	glTranslated(_xOffSet, _yOffSet, 0);

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
		float newFracWidth = (_currentx - _startx);
		float newFracHeight = newFracWidth / ratio;

		glPushMatrix();
		glColor3f(1, 1, 1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(_startx, _starty);
		glVertex2f(_startx + newFracWidth, _starty);
		glVertex2f(_startx + newFracWidth, _starty + newFracHeight);
		glVertex2f(_startx, _starty + newFracHeight);
		glEnd();
		glPopMatrix();
	}

	glPushMatrix();
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	glVertex2f(center.x, center.y);
	glEnd();
	glPopMatrix();

	SDL_GL_SwapBuffers();
}

void FractalGenerator::OnExit()
{
	SDL_FreeSurface(_mainWindow);
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
		_state = Exiting;
	}
	if(keystate[SDLK_a] || keystate[SDLK_LEFT])
	{
		_xOffSet += _moveStep;
	}
	if(keystate[SDLK_d] || keystate[SDLK_RIGHT])
	{
		_xOffSet -= _moveStep;
	}
	if(keystate[SDLK_w] || keystate[SDLK_UP])
	{
		_yOffSet += _moveStep;
	}
	if(keystate[SDLK_s] || keystate[SDLK_DOWN])
	{
		_yOffSet -= _moveStep;
	}

	if (keystate[SDLK_r])
	{
		_scaleFactor = 1;
		_xOffSet = 1;
		_yOffSet = 1;
	}

	if (SDL_GetMouseState(NULL, NULL) &SDL_BUTTON_LEFT)
	{
		_isMouseDown = true;
	}

	while(SDL_PollEvent(&_event))
	{
		switch(_event.type)
		{
		case SDL_QUIT:
			_state = Exiting;
			break;
		case SDL_MOUSEMOTION:

			if (_isMouseDown && !_isDraggingMouse)
			{
				_startx = _event.button.x;
				_starty = _event.button.y;
				_currentx = _startx;
				_currenty = _starty;
				_isDraggingMouse = true;
			}
			else if (_isMouseDown && _isDraggingMouse)
			{
				_currentx = _event.button.x;
				_currenty = _event.button.y;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (_event.button.button == SDL_BUTTON_LEFT && _isMouseDown && _isDraggingMouse)
			{
				_isMouseDown = false;
				_isDraggingMouse = false;

				// get width and height of selection
				float ratio = (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT;
				float newFracWidth = (_currentx - _startx);
				float newFracHeight = newFracWidth / ratio;

				if (newFracWidth > 0 && newFracHeight > 0)
				{
					Vertex starts;
					starts.x = _startx;
					starts.y = _starty;
					starts = NormalToComplex(starts);

					Vertex ends;
					ends.x = _startx + newFracWidth;
					ends.y = _starty + newFracHeight;
					ends = NormalToComplex(ends);

					_fracxstart = starts.x;
					_fracystart = starts.y;
					_fracxend = ends.x;
					_fracyend = ends.y;

					_scaleFactor = SCREEN_WIDTH / newFracWidth;

					cout << "Start: " << _fracxstart << " , " << _fracystart << endl;
					cout << "Current: " << _fracxend << " , " << _fracyend << endl;
					cout << "Zoom Factor: " << _scaleFactor << endl;
					GenerateFractal();
				}
			}
			break;
		}
	}
}

void FractalGenerator::GenerateFractal()
{
	if (_points.size() > 0)
	{
		cout << "\nRegenerating Fractal" << endl;
		_points.clear();
		_vertexBuffer.clear();
		_colorBuffer.clear();
		glDeleteBuffers(1, &_vertexHandle);
		glDeleteBuffers(1, &_colorHandle);
	}

	if (_colors.size() == 0)
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
					_colors.push_back(color);
				}
			}
		}
	}

	ComplexNum c(0, 0);
	ComplexNum z(0, 0);
	Point p;
	Vertex v;
	int iterations;
	int maxIterations = _colors.size() / _scaleFactor;
	_pointPrecision = _pointPrecision / _scaleFactor;

	for (float x = _fracxstart; x < _fracxend; x += _pointPrecision)
	{
		for (float y = _fracystart; y < _fracyend; y += _pointPrecision)
		{
			iterations = 0;
			c = ComplexNum(x, y);
			z = ComplexNum(0, 0);

			while (ComplexNumUtils::Abs(z) < 2 && iterations < maxIterations)
			{
				z = z * z + c;
				++iterations;
			}

			v.x = x;
			v.y = y;
			v = ComplexToNormal(v);

			p.x = v.x;
			p.y = v.y;
			p.c = _colors[_colors.size() % iterations];
			_points.push_back(p);
		}
	}

	for (unsigned int i = 0; i < _points.size(); i++)
	{
		_vertexBuffer.push_back(_points[i].x);
		_vertexBuffer.push_back(_points[i].y);

		_colorBuffer.push_back(_points[i].c.r);
		_colorBuffer.push_back(_points[i].c.g);
		_colorBuffer.push_back(_points[i].c.b);
	}

	cout << "_currentScreenWidth: " << _currentScreenWidth << endl;
	cout << "_currentScreenHeight: " << _currentScreenHeight << endl;
	cout << "Zoom Factor: " << _scaleFactor << endl;
	cout << "fracxstart: " << _fracxstart << endl;
	cout << "fracystart: " << _fracystart << endl;
	cout << "fracxend: " << _fracxend << endl;
	cout << "fracyend: " << _fracyend << endl;

	cout << "Max Iterations: " << maxIterations << endl;
	cout << "Total Points: " << _points.size() << endl;
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
