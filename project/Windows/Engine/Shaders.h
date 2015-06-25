#ifndef SHADERS_H
#define SHADERS_H

#define GLSL_VERSION_STRING "#version 300 es\n"

//## **************************************************************************
//## Quad Shader
//## 
//## Used by Quad objects to render simple 2D boxes to the screen. The quad
//## can be textured or just colored.
//##
//## Attributes:
//##   aPosition - X/Y screen position of vertex in normalized format.
//##   aTexCoord - U/V texture coordinate used in fragment shader.
//##
//## Uniforms:
//##   uType    - type of quad. 0 = Color, 1 = Textured.
//##   uColor   - the color of quad. Will modulate texture color if applicable.
//##   uTexture - texture unit that will be sampled if applicable.
//## **************************************************************************

static const char* pQuadVertexShader = 
GLSL_VERSION_STRING
"in vec2 aPosition;\n"
"in vec2 aTexCoord;\n"
"out vec2 vTexCoord;\n"

"void main()\n"
"{\n"
"   vTexCoord = aTexCoord;\n"
"   gl_Position = vec4(aPosition, 0.0, 1.0);\n"
"}\n";

static const char* pQuadFragmentShader = 
GLSL_VERSION_STRING
"precision mediump float;"
"uniform int uType;\n"
"uniform vec4 uColor;\n"
"uniform sampler2D uTexture;\n"
"in vec2 vTexCoord;\n"
"out vec4 oFragColor;\n"

"void main()\n"
"{\n"

// Color Quad 
"   if (uType == 0)\n"
"   {\n"
"       oFragColor = uColor;\n"
"   }\n"

// Textured Quad
"   else if (uType == 1)\n"
"   {\n"
"       oFragColor = texture(uTexture, vTexCoord) * uColor;\n"
"   }\n"

"}\n";

#endif
