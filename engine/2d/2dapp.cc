//------------------------------------------------------------------------------
// 2dapp.cc
// (C) 2015-2017 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "2dapp.h"
#include <cstring>
#include "fontstash.h"

const GLchar* vs =
"#version 420\n"
"layout(location=0) in vec2 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos, -1, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 420\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
namespace App2D
{

//------------------------------------------------------------------------------
/**
*/
BaseApp::BaseApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
BaseApp::~BaseApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
BaseApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
	});

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = std::strlen(vs);
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = std::strlen(ps);
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}

		// setup vbo
		glGenBuffers(1, &this->vertexBufferObject);

		// setup nanovg
		NVGcontext* vg = this->window->GetNanoVG();
		this->nvFont = nvgCreateFont(vg, "sans", "../exts/nanovg/example/Roboto-Regular.ttf");
		if (this->nvFont == FONS_INVALID)
		{
			printf("Failed to load font, no text rendering will be available\n");
		}
		this->window->SetNanoVGRender([this](NVGcontext * vg)
		{
			this->RenderNanoVG(vg);
		});

		this->Setup();
		return true;		
		
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
BaseApp::Run()
{
	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		this->window->Update();

		this->Update();

		if(!this->vertices.empty())
		{
			// do stuff
			glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferObject);				
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(LineData), &(this->vertices.front()), GL_STATIC_DRAW);			

			glUseProgram(this->program);
			glEnableVertexAttribArray(0);		
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineData) >> 1, NULL);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineData) >> 1, (GLvoid*)(sizeof(float) * 2));
			glDrawArrays(GL_LINES, 0, this->vertices.size()<<1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			this->vertices.clear();
		}		
		this->window->SwapBuffers();		
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
BaseApp::AddLine( const LineData & data )
{	
	this->vertices.push_back(data);
}

//------------------------------------------------------------------------------
/**
*/
void
BaseApp::RenderNanoVG(NVGcontext * vg)
{
	int32 w, h;	
	this->window->GetSize(w, h);

	nvgFontFace(vg, "sans");	
	for (const TextRow & row : this->texts)
	{
		nvgFontSize(vg, row.size);
		nvgFillColor(vg, NVGcolor(row.colour));
		nvgText(vg, row.x*w, row.y*h, row.msg.c_str(), NULL);
	}
	this->texts.clear();
}

//------------------------------------------------------------------------------
/**
*/
void
BaseApp::PrintText(const char * msg, float x, float y)
{
	this->PrintText(msg, x, y, 18.0f, Colour(0.0f, 0.0f, 0.0f));		
}

//------------------------------------------------------------------------------
/**
*/
void
BaseApp::PrintText(const char * msg, float x, float y, float size, const Colour & colour)
{
	TextRow row = { msg, x, y, size, colour };
	this->texts.push_back(row);
}


} // namespace App2D