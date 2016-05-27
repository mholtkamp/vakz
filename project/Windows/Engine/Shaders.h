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
"out vec3 vPointLightDir[3];"
"uniform mat4 uMatrixMVP;\n"
"uniform mat4 uMatrixModel;\n"
"uniform mat4 uMatrixNormal;\n"
"uniform vec3 uPointLightPositions[3];\n"
"uniform int uNumPointLights;\n"

"void main()\n"
"{\n"
"   vTexCoord = aTexCoord;\n"
"   vNormal   = (uMatrixNormal * vec4(aNormal, 0.0)).xyz;\n"
"   vec3 lWorldPos = (uMatrixModel * vec4(aPosition, 1.0)).xyz;\n"
"   for (int i = 0; (i < uNumPointLights) && (i < 3); i++)\n"
"   {\n"
"       vPointLightDir[i] = uPointLightPositions[i] - lWorldPos;\n"
"   }\n"
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
"in vec3 vPointLightDir[3];"
"out vec4 oFragColor;\n"
"uniform vec4 uDiffuseColor;\n"
"uniform vec3 uDirLightVector;\n"
"uniform vec3 uDirLightColor;\n"
"uniform int  uDirLightOn;\n"
"uniform vec3 uPointLightColors[3];\n"
"uniform float uPointLightIntensities[3];\n"
"uniform int uNumPointLights;\n"
"uniform sampler2D uTexture;\n"
"uniform vec4 uAmbientColor;\n"
"uniform int uTextureMode;\n"

"void main()\n"
"{\n"

// Directional Light Power
"   vec3  lLightVector = normalize(-1.0*uDirLightVector);\n"
"   vec3  lNormalVector = normalize(vNormal);\n"
"   float lPower = clamp(dot(lLightVector, lNormalVector), 0.0, 1.0);\n"

"   vec4  lObjectColor = uDiffuseColor;\n"
"   if (uTextureMode == 1)\n"
"   {\n"
"       lObjectColor = lObjectColor * texture(uTexture,  vTexCoord);\n"
"   }\n"
"   vec3  lAmbient = lObjectColor.rgb * uAmbientColor.rgb;\n"

// Add point lighting
"   float lPointPowers[3] = float[](0.0, 0.0, 0.0);\n"
"   for (int i = 0; (i < uNumPointLights) && (i < 3); i++)\n"
"   {\n"
"       float lPointDist = length(vPointLightDir[i]);\n"
"       float lPointPower = clamp((uPointLightIntensities[i] - lPointDist)/uPointLightIntensities[i], 0.0f, 1.0);\n"
"       lPointPowers[i] = lPointPower * clamp(dot(normalize(vPointLightDir[i]), lNormalVector), 0.0, 1.0);\n"
"   }\n"

"   vec3 lLightColor = (lPower         * uDirLightColor + \n"
"                      lPointPowers[0] * uPointLightColors[0] + \n"
"                      lPointPowers[1] * uPointLightColors[1] + \n"
"                      lPointPowers[2] * uPointLightColors[2])/float(uDirLightOn + uNumPointLights);\n"

"   vec3  lDiffuse = clamp(lLightColor * lObjectColor.rgb, 0.0, 1.0);\n"

"   oFragColor.rgb = lDiffuse.rgb + lAmbient.rgb;\n"
"   oFragColor.a   = lObjectColor.a;\n"
"   if (oFragColor.a < 0.004)\n"
"   {\n"
"       discard;\n"
"   }\n"
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
"out vec3 vPointLightDir[3];\n"
"uniform mat4 uMatrixMVP;\n"
"uniform mat4 uMatrixModel;\n"
"uniform mat4 uMatrixNormal;\n"
"uniform vec3 uPointLightPositions[3];\n"
"uniform int uNumPointLights;\n"
"uniform float uMix;\n"

"void main()\n"
"{\n"
"   float lMix = clamp(uMix, 0.0, 1.0);\n"
"   vec3 lPosition = mix(aPosition1, aPosition2, lMix);\n"
"   vec2 lTexCoord = mix(aTexCoord1, aTexCoord2, lMix);\n"
"   vec3 lNormal   = mix(aNormal1,   aNormal2,   lMix);\n"
"   vTexCoord = lTexCoord;\n"
"   vNormal   = (uMatrixNormal * vec4(lNormal, 0.0)).xyz;\n"
"   vec3 lWorldPos = (uMatrixModel * vec4(lPosition, 1.0)).xyz;\n"
"   for (int i = 0; (i < uNumPointLights) && (i < 3); i++)\n"
"   {\n"
"       vPointLightDir[i] = uPointLightPositions[i] - lWorldPos;\n"
"   }\n"
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
"       lObjectColor = lObjectColor * texture(uTexture,  vTexCoord);\n"
"   }\n"
"   if (lObjectColor.a < 0.004)\n"
"   {\n"
"       discard;\n"
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

//## **************************************************************************
//## Particle Update Vertex Shader
//## **************************************************************************
static const char* pParticleUpdateVertexShader =
GLSL_VERSION_STRING

// Randomizer uniforms
"uniform float uMinLifetime;\n"
"uniform float uMaxLifetime;\n"
"uniform vec4 uMinColor;\n"
"uniform vec4 uMaxColor;\n"
"uniform vec3 uMinVelocity;\n"
"uniform vec3 uMaxVelocity;\n"
"uniform float uMinSize;\n"
"uniform float uMaxSize;\n"

// Other uniforms
"uniform vec3 uGravity;\n"
"uniform int uSeed;\n"
"uniform float uDeltaTime;\n"
"uniform int uGenerate;\n"
"uniform vec3 uOrigin;\n"
"uniform vec3 uSpawnVariance;\n"

// Particle data attributes
"layout(location = 0) in vec3 aPosition;\n"
"layout(location = 1) in vec3 aVelocity;\n"
"layout(location = 2) in vec4 aColor;\n"
"layout(location = 3) in float aLife;\n"
"layout(location = 4) in float aSize;\n"

// Particle data ouput
"out vec3 oPosition;\n"
"out vec3 oVelocity;\n"
"out vec4 oColor;\n"
"flat out float oLife;\n"
"flat out float oSize;\n"

// Random function
"float rand(inout int lRandSeed)\n"
"{\n"
//"   lRandSeed = ((lRandSeed * 1103515245) + 12345) & 0x7fffffff;\n"
//"   return float(lRandSeed % 32768)/32768.0;\n"
"   lRandSeed = ((lRandSeed * 104417) + 12345);\n"
"   return float(lRandSeed % 32768)/32768.0;\n"
"}\n"

// Main
"void main()\n"
"{\n"
"   int   lSeed    = uSeed + gl_VertexID;\n"
"   float lRand1    = 0.0;\n"
"   float lRand2    = 0.0;\n"
"   float lRand3    = 0.0;\n"

// Generate a new particle
"   if (aLife     <= 0.0f && \n"
"       uGenerate != 0)\n"
"   {\n"
"       lRand1 = rand(lSeed);\n"
"       lRand2 = rand(lSeed);\n"
"       lRand3 = rand(lSeed);\n"
"       oPosition = vec3(uOrigin.x + uSpawnVariance.x*(lRand1 - 0.5)*2.0,\n"
"                        uOrigin.y + uSpawnVariance.y*(lRand2 - 0.5)*2.0,\n"
"                        uOrigin.z + uSpawnVariance.z*(lRand3 - 0.5)*2.0);\n"
"       lRand1 = rand(lSeed);\n"
"       lRand2 = rand(lSeed);\n"
"       lRand3 = rand(lSeed);\n"
"       oVelocity = vec3(mix(uMinVelocity.x, uMaxVelocity.x, lRand1),\n"
"                        mix(uMinVelocity.y, uMaxVelocity.y, lRand2),\n"
"                        mix(uMinVelocity.z, uMaxVelocity.z, lRand3));\n"
"       lRand1 = rand(lSeed);\n"
"       oColor = vec4(mix(uMinColor.x, uMaxColor.x, lRand1),\n"
"                     mix(uMinColor.y, uMaxColor.y, lRand1),\n"
"                     mix(uMinColor.z, uMaxColor.z, lRand1),\n"
"                     mix(uMinColor.w, uMaxColor.w, lRand1));\n"
"       lRand1 = rand(lSeed);\n"
"       oLife = mix(uMinLifetime, uMaxLifetime, lRand1);\n"
"       lRand1 = rand(lSeed);\n"
"       oSize = mix(uMinSize, uMaxSize, lRand1);\n"
"   }\n"
"   else\n"
"   {\n"
"       oPosition = aPosition + aVelocity*uDeltaTime;\n"
"       oVelocity = aVelocity + uGravity*uDeltaTime;\n"
"       oColor    = aColor;\n"
"       oLife     = aLife - uDeltaTime;\n"
"       oSize     = aSize;\n"
"   }\n"
"}\n";

//## **************************************************************************
//## Particle Update Fragment Shader
//## **************************************************************************
static const char* pParticleUpdateFragmentShader =
GLSL_VERSION_STRING
"precision mediump float;\n"
"out vec4 oFragColor;\n"
"void main()\n"
"{\n"
"   oFragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";

//## **************************************************************************
//## Particle Update Transform Feedback Varyings
//## **************************************************************************
static const char* pParticleUpdateFeedbackVaryings[5] =
{
    "oPosition",
    "oVelocity",
    "oColor",
    "oLife",
    "oSize"
};
static int nParticleUpdateFeedbackVaryingsCount = 5;

//## **************************************************************************
//## Particle Render Vertex Shader
//## **************************************************************************
static const char* pParticleRenderVertexShader =
GLSL_VERSION_STRING

"uniform mat4 uMatrixVP;\n"

"in vec3 aPosition;\n"
"in vec4 aColor;\n"
"in float aLife;\n"
"in float aSize;\n"

"out vec4 vColor;\n"

"void main()\n"
"{\n"
"   vColor = aColor;\n"
"   gl_Position = uMatrixVP * vec4(aPosition, 1.0);\n"
"   gl_PointSize = aSize;\n"
"}\n";


//## **************************************************************************
//## Particle Render Fragment Shader
//## **************************************************************************
static const char* pParticleRenderFragmentShader =
GLSL_VERSION_STRING
"precision mediump float;\n"
"uniform int uTexType;\n"
"uniform sampler2D uTexture;\n"
"in vec4 vColor;\n"
"out vec4 oFragColor;\n"

"void main()\n"
"{\n"
"    if (uTexType != 0)\n"
"    {\n"
"       oFragColor = vColor * texture(uTexture, vec2(gl_PointCoord.x, 1.0 - gl_PointCoord.y));\n"
"    }\n"
"    else\n"
"    {\n"
"       oFragColor = vColor;\n"
"    }\n"
"    if (oFragColor.a < 0.004)\n"
"    {\n"
"        discard;\n"
"    }\n"
"}\n";

//## **************************************************************************
//## Rimlit Fragment Shader
//## **************************************************************************
static const char* pRimlitFragmentShader = 
GLSL_VERSION_STRING
"precision mediump float;\n"
"in vec2 vTexCoord;\n"
"in vec3 vNormal;\n"
"out vec4 oFragColor;\n"
"uniform vec4 uDiffuseColor;\n"
"uniform vec4 uRimColor;\n"
"uniform vec3 uViewVector;\n"
"uniform vec3 uDirLightVector;\n"
"uniform vec3 uDirLightColor;\n"
"uniform sampler2D uTexture;\n"
"uniform vec4 uAmbientColor;\n"
"uniform int uTextureMode;\n"
"uniform float uRimSize;\n"
"uniform int uRimStyle;\n"

"void main()\n"
"{\n"

// Directional Light Power
"   vec3  lLightVector = normalize(-1.0*uDirLightVector);\n"
"   vec3  lNormalVector = normalize(vNormal);\n"
"   float lPower = dot(lLightVector, lNormalVector);\n"
"   float lRimPower = 1.0 - abs(dot(uViewVector, lNormalVector));\n"

"   vec4  lObjectColor = uDiffuseColor;\n"
"   if (uTextureMode == 1)\n"
"   {\n"
"       lObjectColor.rgb = lObjectColor.rgb * texture(uTexture,  vTexCoord).rgb;\n"
"   }\n"
"   vec3  lAmbient = lObjectColor.rgb * uAmbientColor.rgb;\n"
"   vec3  lDiffuse = clamp(lPower * uDirLightColor * lObjectColor.rgb, 0.0, 1.0);\n"
"   oFragColor.rgb = lDiffuse.rgb + lAmbient.rgb;\n"
"   oFragColor.a   = lObjectColor.a;\n"

// Rim light part
"   float lRimThresh = 1.0 - uRimSize;\n"

// RimStyle == 0 : Hard
"   if (uRimStyle == 0)\n"
"   {\n"
"       oFragColor = mix(oFragColor, uRimColor, step(lRimThresh, lRimPower));\n"
"   }\n"

// RimStyle == 1: Soft
"   else if(uRimStyle == 1)\n"
"   {\n"
"       oFragColor = mix(oFragColor, uRimColor, lRimPower * step(lRimThresh, lRimPower) * ((lRimPower - lRimThresh)/(1.0 - lRimThresh)));\n"
"   }\n"
"}\n";

#endif
