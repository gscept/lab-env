#pragma once
//------------------------------------------------------------------------------
/**
	2D Application class
	
	(C) 2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include <vector>

namespace App2D
{
class BaseApp : public Core::App
{
public:
	
	/// constructor
	BaseApp();
	/// destructor
	~BaseApp();

	/// open app
	bool Open();
	/// run app
	void Run();

	/// setup function, called once by open
	virtual void Setup() = 0;
	/// update function, overload to implement your own drawing functions
	virtual void Update() = 0;
	
	struct Colour
	{
		float r,g,b;
		Colour() : r(0.0f), g(0.0f), b(1.0f) 
		{
			// empty
		};
	};

	struct LineData
	{
		float x1, y1;
		Colour c1;
		float x2, y2;
		Colour c2;
	};

	/// 
	void AddLine(const LineData & data);

private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint vertexBufferObject;
	Display::Window* window;
	std::vector<LineData> vertices;	
};


} // namespace App2D