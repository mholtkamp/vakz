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
"precision mediump float;\n"
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

"   if (oFragColor.a < 0.003)\n"
"   {\n"
"       discard;\n"
"   }\n"
"}\n";


//## **************************************************************************
//## Text Shader
//## 
//## Used by Text objects to render a string of characters in screen space.
//##
//## Attributes:
//##   aPosition - X/Y screen position of vertex in normalized format.
//##   aTexCoord - U/V texture coordinate used in fragment shader.
//##
//## Uniforms:
//##   uColor   - color of text that will be rendered.
//##   uTexture - the font texture unit that will be sampled.
//## **************************************************************************

static const char* pTextVertexShader = 
GLSL_VERSION_STRING
"in vec2 aPosition;\n"
"in vec2 aTexCoord;\n"
"out vec2 vTexCoord;\n"
"uniform vec2 uOrigin;\n"
"uniform vec2 uScale;\n"

"void main()\n"
"{\n"
"   vTexCoord = aTexCoord;\n"
"   vec2 lPosition = aPosition * uScale + uOrigin;\n"
"   gl_Position = vec4(lPosition, 0.0, 1.0);\n"
"}\n";

static const char* pTextFragmentShader = 
GLSL_VERSION_STRING
"precision mediump float;\n"
"uniform vec4 uColor;\n"
"uniform sampler2D uTexture;\n"
"in vec2 vTexCoord;\n"
"out vec4 oFragColor;\n"

"void main()\n"
"{\n"
"   vec4 lTexColor = texture(uTexture, vTexCoord);\n"
"   if (lTexColor.a < 0.003)\n"
"   {\n"
"       discard;\n"
"   }\n"
"   oFragColor = uColor * lTexColor.a;\n"
"}\n";

//## **************************************************************************
//## StaticMesh Vertex Shader
//## **************************************************************************
static const char* pStaticMeshVertexShader = 
GLSL_VERSION_STRING
"in vec3 aPosition;\n"
"in vec2 aTexCoord;\n"
"in vec3 aNormal;\n"
"out vec2 vTexCoord;\n"
"out vec3 vNormal;\n"
"uniform mat4 uMatrixMVP;\n"
"uniform mat4 uMatrixM;\n"

"void main()\n"
"{\n"
"   vTexCoord = aTexCoord;\n"
"   vNormal   = (uMatrixM * vec4(aNormal, 0.0)).xyz;\n"
//"   vNormal   = (transpose(inverse(uMatrixM)) * vec4(aNormal, 0.0)).xyz;\n" 
"   gl_Position = uMatrixMVP * vec4(aPosition, 1.0);\n"
"}\n";


//## **************************************************************************
//## Diffuse Fragment Shader
//## **************************************************************************
static const char* pDiffuseFragmentShader = 
GLSL_VERSION_STRING
"precision mediump float;\n"
"in vec2 vTexCoord;\n"
"in vec3 vNormal;\n"
"out vec4 oFragColor;\n"
"uniform vec4 uDiffuseColor;\n"
"uniform vec3 uDirLightVector;\n"
"uniform vec4 uDirLightColor;\n"
"uniform sampler2D uTexture;\n"
"uniform vec4 uAmbientColor;\n"
"uniform int uTextureMode;\n"

"void main()\n"
"{\n"

// Directional Light Power
"   vec3  lLightVector = normalize(-1.0*uDirLightVector);\n"
"   vec3  lNormalVector = normalize(vNormal);\n"
"   float lPower = dot(lLightVector, lNormalVector);\n"

"   vec4  lObjectColor = uDiffuseColor;\n"
"   if (uTextureMode == 1)\n"
"   {\n"
"       lObjectColor.rgb = lObjectColor.rgb * texture(uTexture,  vTexCoord).rgb;\n"
"   }\n"
"   vec3  lAmbient = lObjectColor.rgb * uAmbientColor.rgb;\n"
"   vec3  lDiffuse = clamp(lPower * uDirLightColor.rgb * lObjectColor.rgb, 0.0, 1.0);\n"
"   oFragColor.rgb = lDiffuse.rgb + lAmbient.rgb;\n"
"   oFragColor.a   = lObjectColor.a;\n"
"}\n";


//## **************************************************************************
//## AnimatedMesh Vertex Shader
//## **************************************************************************
static const char* pAnimatedMeshVertexShader = 
GLSL_VERSION_STRING
"in vec3 aPosition1;\n"
"in vec2 aTexCoord1;\n"
"in vec3 aNormal1;\n"
"in vec3 aPosition2;\n"
"in vec2 aTexCoord2;\n"
"in vec3 aNormal2;\n"
"out vec2 vTexCoord;\n"
"out vec3 vNormal;\n"
"uniform mat4 uMatrixMVP;\n"
"uniform mat4 uMatrixM;\n"
"uniform float uMix;\n"

"void main()\n"
"{\n"
"   float lMix = clamp(uMix, 0.0, 1.0);\n"
"   vec3 lPosition = mix(aPosition1, aPosition2, lMix);\n"
"   vec2 lTexCoord = mix(aTexCoord1, aTexCoord2, lMix);\n"
"   vec3 lNormal   = mix(aNormal1,   aNormal2,   lMix);\n"
"   vTexCoord = lTexCoord;\n"
"   vNormal   = (uMatrixM * vec4(lNormal, 0.0)).xyz;\n"
//"   vNormal   = (transpose(inverse(uMatrixM)) * vec4(lNormal, 0.0)).xyz;\n" 
"   gl_Position = uMatrixMVP * vec4(lPosition, 1.0);\n"
"}\n";

//## **************************************************************************
//## Fullbright Fragment Shader
//## **************************************************************************
static const char* pFullbrightFragmentShader = 
GLSL_VERSION_STRING
"precision mediump float;\n"
"in vec2 vTexCoord;\n"
"out vec4 oFragColor;\n"
"uniform vec4 uColor;\n"
"uniform sampler2D uTexture;\n"
"uniform int uTextureMode;\n"

"void main()\n"
"{\n"
"   vec4  lObjectColor = uColor;\n"
"   if (uTextureMode == 1)\n"
"   {\n"
"       lObjectColor.rgb = lObjectColor.rgb * texture(uTexture,  vTexCoord).rgb;\n"
"   }\n"
"   oFragColor = lObjectColor;\n"
"}\n";

//## **************************************************************************
//## Effect Vertex Shader
//## **************************************************************************
static const char* pEffectVertexShader = 
GLSL_VERSION_STRING
"in vec2 aPosition;\n"
"in vec2 aTexCoord;\n"
"out vec2 vTexCoord;\n"

"void main()\n"
"{\n"
"   vTexCoord = aTexCoord;\n"
"   gl_Position = vec4(aPosition, 0.0, 1.0);\n"
"}\n";

//## **************************************************************************
//## Blur Fragment Shader
//## **************************************************************************
static const char* pBlurFragmentShader =
GLSL_VERSION_STRING
"precision mediump float;\n"
"uniform vec2 uDimensions;\n"
"uniform int uBlurFactor;\n"
"uniform int uSampleDistance;\n"
"uniform sampler2D uTexture;\n"
"in vec2 vTexCoord;\n"
"out vec4 oFragColor;\n"

"void main()\n"
"{\n"
"   vec4 lSum = vec4(0.0, 0.0, 0.0, 0.0);\n"
"   float lSampleDistance = float(uSampleDistance);"
"   float lIncX = 1.0f/uDimensions.x;\n"
"   float lIncY = 1.0f/uDimensions.y;\n"
"   float lCount = pow(float(uBlurFactor)*2.0 + 1.0, 2.0);\n"
"   for (int x = -uBlurFactor; x <= uBlurFactor; x++)\n"
"   {\n"
"       for (int y = -uBlurFactor; y <= uBlurFactor; y++)\n"
"       {\n"
"           lSum += texture(uTexture, vec2(vTexCoord.x + float(x) * lSampleDistance * lIncX, vTexCoord.y + float(y) * lIncY * lSampleDistance)) / lCount;\n"
"       }\n"
"   }\n"
"   oFragColor = lSum;\n"
"}\n";

#endif
