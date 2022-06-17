//------------------------------------------------------------------------------
//  grid.cc
//  (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "grid.h"
#include <array>

namespace Render
{

static const GLchar* vs =
    "#version 430\n"
    "layout(location = 0) in vec3 pos;\n"
    "layout(location = 0) uniform mat4 ViewProjection;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = ViewProjection * vec4(pos, 1);\n"
    "}\n";

static const GLchar* ps =
    "#version 430\n"
    "out vec4 Color;\n"
    "void main()\n"
    "{\n"
    "	Color = vec4(0.5f,0.5f,0.5f, 0.5f);\n"
    "}\n";

static constexpr size_t gridSize = 9;

//------------------------------------------------------------------------------
/**
*/
template<size_t GRIDSIZE>
const std::array<float32, GRIDSIZE * 16>
GenerateLineBuffer()
{
	std::array<float32, GRIDSIZE * 16> arr = {};

	const float32 scale = 0.25f;
	const float32 max = (GRIDSIZE / 2); // top right
	const float32 min = -max; // top left
	
	for (size_t row = 0; row < GRIDSIZE; row++)
	{
		const size_t offset = row * 8;
		
		arr[offset]		= (min) * scale;
		arr[offset + 1] = 0.0f;
		arr[offset + 2] = (min + row) * scale;
		arr[offset + 3] = 1.0f;

		arr[offset + 4] = (max) * scale;
		arr[offset + 5] = 0.0f;
		arr[offset + 6] = (min + row) * scale;
		arr[offset + 7] = 1.0f;
	}
	for (size_t col = 0; col < GRIDSIZE; col++)
	{
		const size_t offset = GRIDSIZE * 8 + col * 8;

		arr[offset]		= (min + col) * scale;
		arr[offset + 1] = 0.0f;
		arr[offset + 2] = (min) * scale;
		arr[offset + 3] = 1.0f;

		arr[offset + 4] = (min + col) * scale;
		arr[offset + 5] = 0.0f;
		arr[offset + 6] = (max) * scale;
		arr[offset + 7] = 1.0f;
	}

	return arr;
}

//------------------------------------------------------------------------------
/**
*/
Grid::Grid() : lineBuffer(0)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint length = static_cast<GLint>(std::strlen(vs));
	glShaderSource(vertexShader, 1, &vs, &length);
	glCompileShader(vertexShader);

	GLuint pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	length = static_cast<GLint>(std::strlen(ps));
	glShaderSource(pixelShader, 1, &ps, &length);
	glCompileShader(pixelShader);

	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, pixelShader);
	glLinkProgram(this->program);

	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);

	const auto buf = GenerateLineBuffer<gridSize>();
	glGenBuffers(1, &this->lineBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->lineBuffer);
	glBufferData(GL_ARRAY_BUFFER, buf.size() * sizeof(float32), buf.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

//------------------------------------------------------------------------------
/**
*/
Grid::~Grid()
{
	glDeleteProgram(this->program);
	glDeleteBuffers(1, &this->lineBuffer);
}
    
//------------------------------------------------------------------------------
/**
*/
void
Grid::Draw(float const* const viewProjection)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->lineBuffer);
	glUseProgram(this->program);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 4, NULL);
	glUniformMatrix4fv(0, 1, false, viewProjection);
	glDrawArrays(GL_LINES, 0, gridSize * 2 * 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

}

} // namespace Render