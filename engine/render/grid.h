#pragma once
//------------------------------------------------------------------------------
/**
    @class Grid

    Contains functions for rendering a basic grid

    (C) 2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <GL/glew.h>

namespace Render
{

class Grid
{
public:
    Grid();
    ~Grid();
    
    void Draw(float const* const viewProjection);
    
private:
    GLuint program;
    GLuint vao;
    GLuint lineBuffer;
};

} // namespace Render