#pragma once
//------------------------------------------------------------------------------
/**
	2D Application class
	
	(C) 2015-2017 Individual contributors, see AUTHORS file
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
		Colour(float r, float g, float b)
		{
			this->r = r;
			this->g = g;
			this->b = b;
		}
		operator NVGcolor() const
		{
			NVGcolor c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = 1.0f;
			return c;
		}
	};

	struct LineData
	{
		float x1, y1;
		Colour c1;
		float x2, y2;
		Colour c2;
	};

	/// draws a line as defined in the LineData struct
	void AddLine(const LineData & data);
	/// print text at location
	void PrintText(const char * msg, float x, float y);
	/// print text at location with size and colour
	void PrintText(const char * msg, float x, float y, float size, const Colour & colour);

private:
	/// renders the collected text 
	void RenderNanoVG(NVGcontext * vg);

	struct TextRow
	{
		std::string msg;
		float x, y;
		float size;
		Colour colour;		
	};
	
	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint vertexBufferObject;
	int nvFont;
	Display::Window* window;
	std::vector<LineData> vertices;	
	std::vector<TextRow> texts;
};


} // namespace App2D