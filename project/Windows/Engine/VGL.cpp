#include "VGL.h"
#include "Mesh.h"
#include "Material.h"
#include "Log.h"
#include <stdio.h>
#include <string.h>

// Buffer used to store shader compilation logs
#define LOG_SIZE 1024
static char pLogBuffer[LOG_SIZE];

// Array of shaders
static unsigned int s_arPrograms[MAX_PROGRAMS];

#if defined(WINDOWS)
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
PFNGLCLEARBUFFERIVPROC glClearBufferiv;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETSTRINGIPROC glGetStringi;
PFNGLFENCESYNCPROC glFenceSync;
PFNGLUNIFORM3UIPROC glUniform3ui;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv;
PFNGLENDQUERYPROC glEndQuery;
PFNGLBINDSAMPLERPROC glBindSampler;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
PFNGLGENQUERIESPROC glGenQueries;
PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v;
PFNGLDEPTHRANGEFPROC glDepthRangef;
PFNGLUNIFORM4IVPROC glUniform4iv;
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
PFNGLISVERTEXARRAYPROC glIsVertexArray;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv;
PFNGLPROGRAMBINARYPROC glProgramBinary;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLISBUFFERPROC glIsBuffer;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
PFNGLUNIFORM2IPROC glUniform2i;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;
PFNGLBLENDEQUATIONPROC glBlendEquation;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback;
PFNGLTEXSTORAGE2DPROC glTexStorage2D;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLISSAMPLERPROC glIsSampler;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLWAITSYNCPROC glWaitSync;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
PFNGLUNIFORM3IPROC glUniform3i;
PFNGLCLEARBUFFERFVPROC glClearBufferfv;
PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks;
PFNGLDELETESAMPLERSPROC glDeleteSamplers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
PFNGLGETINTEGER64I_VPROC glGetInteger64i_v;
PFNGLSHADERBINARYPROC glShaderBinary;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
PFNGLUNIFORM2IVPROC glUniform2iv;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLUNIFORM1IVPROC glUniform1iv;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
PFNGLTEXSTORAGE3DPROC glTexStorage3D;
PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
PFNGLBLENDCOLORPROC glBlendColor;
PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat;
PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
PFNGLISSYNCPROC glIsSync;
PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLUNIFORM1UIPROC glUniform1ui;
PFNGLISPROGRAMPROC glIsProgram;
PFNGLGETSYNCIVPROC glGetSynciv;
PFNGLGETUNIFORMIVPROC glGetUniformiv;
PFNGLUNIFORM4IPROC glUniform4i;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
PFNGLUNIFORM2UIVPROC glUniform2uiv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
PFNGLDELETESYNCPROC glDeleteSync;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
PFNGLUNIFORM3IVPROC glUniform3iv;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLDRAWBUFFERSPROC glDrawBuffers;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLUNIFORM1FVPROC glUniform1fv;
PFNGLBEGINQUERYPROC glBeginQuery;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
PFNGLUNIFORM4UIVPROC glUniform4uiv;
PFNGLISQUERYPROC glIsQuery;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv;
PFNGLGETQUERYIVPROC glGetQueryiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv;
PFNGLUNIFORM4UIPROC glUniform4ui;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
PFNGLISSHADERPROC glIsShader;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGETINTEGER64VPROC glGetInteger64v;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui;
PFNGLDELETEQUERIESPROC glDeleteQueries;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLTEXIMAGE3DPROC glTexImage3D;
PFNGLGENSAMPLERSPROC glGenSamplers;
PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;
PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer;
PFNGLUNIFORM1UIVPROC glUniform1uiv;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
PFNGLUNIFORM2UIPROC glUniform2ui;
PFNGLGETUNIFORMFVPROC glGetUniformfv;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
PFNGLCLEARBUFFERFIPROC glClearBufferfi;
PFNGLCLEARDEPTHFPROC glClearDepthf;
PFNGLGETSHADERSOURCEPROC glGetShaderSource;
PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv;
PFNGLVALIDATEPROGRAMPROC glValidateProgram;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv;
PFNGLUNIFORM3UIVPROC glUniform3uiv;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
#endif

//*****************************************************************************
// LoadVGL
//*****************************************************************************
int LoadVGL()
{
#if defined(WINDOWS)
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	glBlendColor = (PFNGLBLENDCOLORPROC)wglGetProcAddress("glBlendColor");
	glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
	glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)wglGetProcAddress("glBlendEquationSeparate");
	glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)wglGetProcAddress("glBlendFuncSeparate");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
	glClearDepthf = (PFNGLCLEARDEPTHFPROC)wglGetProcAddress("glClearDepthf");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)wglGetProcAddress("glCompressedTexImage2D");
	glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)wglGetProcAddress("glCompressedTexSubImage2D");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glDepthRangef = (PFNGLDEPTHRANGEFPROC)wglGetProcAddress("glDepthRangef");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
	glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)wglGetProcAddress("glGetAttachedShaders");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetBufferParameteriv");
	glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)wglGetProcAddress("glGetShaderPrecisionFormat");
	glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)wglGetProcAddress("glGetShaderSource");
	glGetUniformfv = (PFNGLGETUNIFORMFVPROC)wglGetProcAddress("glGetUniformfv");
	glGetUniformiv = (PFNGLGETUNIFORMIVPROC)wglGetProcAddress("glGetUniformiv");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)wglGetProcAddress("glGetVertexAttribfv");
	glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)wglGetProcAddress("glGetVertexAttribiv");
	glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)wglGetProcAddress("glGetVertexAttribPointerv");
	glIsBuffer = (PFNGLISBUFFERPROC)wglGetProcAddress("glIsBuffer");
	glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebuffer");
	glIsProgram = (PFNGLISPROGRAMPROC)wglGetProcAddress("glIsProgram");
	glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");
	glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)wglGetProcAddress("glReleaseShaderCompiler");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
	glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)wglGetProcAddress("glSampleCoverage");
	glShaderBinary = (PFNGLSHADERBINARYPROC)wglGetProcAddress("glShaderBinary");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)wglGetProcAddress("glStencilFuncSeparate");
	glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)wglGetProcAddress("glStencilMaskSeparate");
	glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)wglGetProcAddress("glStencilOpSeparate");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)wglGetProcAddress("glValidateProgram");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)wglGetProcAddress("glVertexAttrib2f");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)wglGetProcAddress("glVertexAttrib3f");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)wglGetProcAddress("glVertexAttrib4f");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)wglGetProcAddress("glDrawRangeElements");
	glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
	glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)wglGetProcAddress("glTexSubImage3D");
	glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)wglGetProcAddress("glCopyTexSubImage3D");
	glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)wglGetProcAddress("glCompressedTexImage3D");
	glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)wglGetProcAddress("glCompressedTexSubImage3D");
	glGenQueries = (PFNGLGENQUERIESPROC)wglGetProcAddress("glGenQueries");
	glDeleteQueries = (PFNGLDELETEQUERIESPROC)wglGetProcAddress("glDeleteQueries");
	glIsQuery = (PFNGLISQUERYPROC)wglGetProcAddress("glIsQuery");
	glBeginQuery = (PFNGLBEGINQUERYPROC)wglGetProcAddress("glBeginQuery");
	glEndQuery = (PFNGLENDQUERYPROC)wglGetProcAddress("glEndQuery");
	glGetQueryiv = (PFNGLGETQUERYIVPROC)wglGetProcAddress("glGetQueryiv");
	glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)wglGetProcAddress("glGetQueryObjectuiv");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)wglGetProcAddress("glGetBufferPointerv");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
	glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)wglGetProcAddress("glUniformMatrix2x3fv");
	glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)wglGetProcAddress("glUniformMatrix3x2fv");
	glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)wglGetProcAddress("glUniformMatrix2x4fv");
	glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)wglGetProcAddress("glUniformMatrix4x2fv");
	glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)wglGetProcAddress("glUniformMatrix3x4fv");
	glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)wglGetProcAddress("glUniformMatrix4x3fv");
	glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)wglGetProcAddress("glBlitFramebuffer");
	glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)wglGetProcAddress("glRenderbufferStorageMultisample");
	glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)wglGetProcAddress("glFramebufferTextureLayer");
	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
	glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)wglGetProcAddress("glFlushMappedBufferRange");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glIsVertexArray = (PFNGLISVERTEXARRAYPROC)wglGetProcAddress("glIsVertexArray");
	glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)wglGetProcAddress("glGetIntegeri_v");
	glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)wglGetProcAddress("glBeginTransformFeedback");
	glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)wglGetProcAddress("glEndTransformFeedback");
	glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)wglGetProcAddress("glBindBufferRange");
	glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)wglGetProcAddress("glBindBufferBase");
	glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)wglGetProcAddress("glTransformFeedbackVaryings");
	glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)wglGetProcAddress("glGetTransformFeedbackVarying");
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
	glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)wglGetProcAddress("glGetVertexAttribIiv");
	glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)wglGetProcAddress("glGetVertexAttribIuiv");
	glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)wglGetProcAddress("glVertexAttribI4i");
	glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)wglGetProcAddress("glVertexAttribI4ui");
	glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)wglGetProcAddress("glVertexAttribI4iv");
	glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)wglGetProcAddress("glVertexAttribI4uiv");
	glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)wglGetProcAddress("glGetUniformuiv");
	glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)wglGetProcAddress("glGetFragDataLocation");
	glUniform1ui = (PFNGLUNIFORM1UIPROC)wglGetProcAddress("glUniform1ui");
	glUniform2ui = (PFNGLUNIFORM2UIPROC)wglGetProcAddress("glUniform2ui");
	glUniform3ui = (PFNGLUNIFORM3UIPROC)wglGetProcAddress("glUniform3ui");
	glUniform4ui = (PFNGLUNIFORM4UIPROC)wglGetProcAddress("glUniform4ui");
	glUniform1uiv = (PFNGLUNIFORM1UIVPROC)wglGetProcAddress("glUniform1uiv");
	glUniform2uiv = (PFNGLUNIFORM2UIVPROC)wglGetProcAddress("glUniform2uiv");
	glUniform3uiv = (PFNGLUNIFORM3UIVPROC)wglGetProcAddress("glUniform3uiv");
	glUniform4uiv = (PFNGLUNIFORM4UIVPROC)wglGetProcAddress("glUniform4uiv");
	glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)wglGetProcAddress("glClearBufferiv");
	glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)wglGetProcAddress("glClearBufferuiv");
	glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)wglGetProcAddress("glClearBufferfv");
	glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)wglGetProcAddress("glClearBufferfi");
	glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
	glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)wglGetProcAddress("glCopyBufferSubData");
	glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)wglGetProcAddress("glGetUniformIndices");
	glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)wglGetProcAddress("glGetActiveUniformsiv");
	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)wglGetProcAddress("glGetUniformBlockIndex");
	glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)wglGetProcAddress("glGetActiveUniformBlockiv");
	glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)wglGetProcAddress("glGetActiveUniformBlockName");
	glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)wglGetProcAddress("glUniformBlockBinding");
	glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)wglGetProcAddress("glDrawArraysInstanced");
	glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");
	glFenceSync = (PFNGLFENCESYNCPROC)wglGetProcAddress("glFenceSync");
	glIsSync = (PFNGLISSYNCPROC)wglGetProcAddress("glIsSync");
	glDeleteSync = (PFNGLDELETESYNCPROC)wglGetProcAddress("glDeleteSync");
	glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)wglGetProcAddress("glClientWaitSync");
	glWaitSync = (PFNGLWAITSYNCPROC)wglGetProcAddress("glWaitSync");
	glGetInteger64v = (PFNGLGETINTEGER64VPROC)wglGetProcAddress("glGetInteger64v");
	glGetSynciv = (PFNGLGETSYNCIVPROC)wglGetProcAddress("glGetSynciv");
	glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)wglGetProcAddress("glGetInteger64i_v");
	glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)wglGetProcAddress("glGetBufferParameteri64v");
	glGenSamplers = (PFNGLGENSAMPLERSPROC)wglGetProcAddress("glGenSamplers");
	glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)wglGetProcAddress("glDeleteSamplers");
	glIsSampler = (PFNGLISSAMPLERPROC)wglGetProcAddress("glIsSampler");
	glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
	glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)wglGetProcAddress("glSamplerParameteri");
	glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)wglGetProcAddress("glSamplerParameteriv");
	glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)wglGetProcAddress("glSamplerParameterf");
	glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)wglGetProcAddress("glSamplerParameterfv");
	glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)wglGetProcAddress("glGetSamplerParameteriv");
	glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)wglGetProcAddress("glGetSamplerParameterfv");
	glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress("glVertexAttribDivisor");
	glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)wglGetProcAddress("glBindTransformFeedback");
	glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)wglGetProcAddress("glDeleteTransformFeedbacks");
	glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)wglGetProcAddress("glGenTransformFeedbacks");
	glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)wglGetProcAddress("glIsTransformFeedback");
	glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)wglGetProcAddress("glPauseTransformFeedback");
	glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)wglGetProcAddress("glResumeTransformFeedback");
	glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)wglGetProcAddress("glGetProgramBinary");
	glProgramBinary = (PFNGLPROGRAMBINARYPROC)wglGetProcAddress("glProgramBinary");
	glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)wglGetProcAddress("glProgramParameteri");
	glInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC)wglGetProcAddress("glInvalidateFramebuffer");
	glInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC)wglGetProcAddress("glInvalidateSubFramebuffer");
	glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)wglGetProcAddress("glTexStorage2D");
	glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)wglGetProcAddress("glTexStorage3D");
	glGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC)wglGetProcAddress("glGetInternalformativ");

    if( glActiveTexture != 0 &&
	    glAttachShader != 0 &&
	    glBindAttribLocation != 0 &&
	    glBindBuffer != 0 &&
	    glBindFramebuffer != 0 &&
	    glBindRenderbuffer != 0 &&
	    glBlendColor != 0 &&
	    glBlendEquation != 0 &&
	    glBlendEquationSeparate != 0 &&
	    glBlendFuncSeparate != 0 &&
	    glBufferData != 0 &&
	    glBufferSubData != 0 &&
	    glCheckFramebufferStatus != 0 &&
	    glClearDepthf != 0 &&
	    glCompileShader != 0 &&
	    glCompressedTexImage2D != 0 &&
	    glCompressedTexSubImage2D != 0 &&
	    glCreateProgram != 0 &&
	    glCreateShader != 0 &&
	    glDeleteBuffers != 0 &&
	    glDeleteFramebuffers != 0 &&
	    glDeleteProgram != 0 &&
	    glDeleteRenderbuffers != 0 &&
	    glDeleteShader != 0 &&
	    glDepthRangef != 0 &&
	    glDetachShader != 0 &&
	    glDisableVertexAttribArray != 0 &&
	    glEnableVertexAttribArray != 0 &&
	    glFramebufferRenderbuffer != 0 &&
	    glFramebufferTexture2D != 0 &&
	    glGenBuffers != 0 &&
	    glGenerateMipmap != 0 &&
	    glGenFramebuffers != 0 &&
	    glGenRenderbuffers != 0 &&
	    glGetActiveAttrib != 0 &&
	    glGetActiveUniform != 0 &&
	    glGetAttachedShaders != 0 &&
	    glGetAttribLocation != 0 &&
	    glGetBufferParameteriv != 0 &&
	    glGetFramebufferAttachmentParameteriv != 0 &&
	    glGetProgramiv != 0 &&
	    glGetProgramInfoLog != 0 &&
	    glGetRenderbufferParameteriv != 0 &&
	    glGetShaderiv != 0 &&
	    glGetShaderInfoLog != 0 &&
	    glGetShaderPrecisionFormat != 0 &&
	    glGetShaderSource != 0 &&
	    glGetUniformfv != 0 &&
	    glGetUniformiv != 0 &&
	    glGetUniformLocation != 0 &&
	    glGetVertexAttribfv != 0 &&
	    glGetVertexAttribiv != 0 &&
	    glGetVertexAttribPointerv != 0 &&
	    glIsBuffer != 0 &&
	    glIsFramebuffer != 0 &&
	    glIsProgram != 0 &&
	    glIsRenderbuffer != 0 &&
	    glIsShader != 0 &&
	    glLinkProgram != 0 &&
	    glReleaseShaderCompiler != 0 &&
	    glRenderbufferStorage != 0 &&
	    glSampleCoverage != 0 &&
	    glShaderBinary != 0 &&
	    glShaderSource != 0 &&
	    glStencilFuncSeparate != 0 &&
	    glStencilMaskSeparate != 0 &&
	    glStencilOpSeparate != 0 &&
	    glUniform1f != 0 &&
	    glUniform1fv != 0 &&
	    glUniform1i != 0 &&
	    glUniform1iv != 0 &&
	    glUniform2f != 0 &&
	    glUniform2fv != 0 &&
	    glUniform2i != 0 &&
	    glUniform2iv != 0 &&
	    glUniform3f != 0 &&
	    glUniform3fv != 0 &&
	    glUniform3i != 0 &&
	    glUniform3iv != 0 &&
	    glUniform4f != 0 &&
	    glUniform4fv != 0 &&
	    glUniform4i != 0 &&
	    glUniform4iv != 0 &&
	    glUniformMatrix2fv != 0 &&
	    glUniformMatrix3fv != 0 &&
	    glUniformMatrix4fv != 0 &&
	    glUseProgram != 0 &&
	    glValidateProgram != 0 &&
	    glVertexAttrib1f != 0 &&
	    glVertexAttrib1fv != 0 &&
	    glVertexAttrib2f != 0 &&
	    glVertexAttrib2fv != 0 &&
	    glVertexAttrib3f != 0 &&
	    glVertexAttrib3fv != 0 &&
	    glVertexAttrib4f != 0 &&
	    glVertexAttrib4fv != 0 &&
	    glVertexAttribPointer != 0 &&
	    glDrawRangeElements != 0 &&
	    glTexImage3D != 0 &&
	    glTexSubImage3D != 0 &&
	    glCopyTexSubImage3D != 0 &&
	    glCompressedTexImage3D != 0 &&
	    glCompressedTexSubImage3D != 0 &&
	    glGenQueries != 0 &&
	    glDeleteQueries != 0 &&
	    glIsQuery != 0 &&
	    glBeginQuery != 0 &&
	    glEndQuery != 0 &&
	    glGetQueryiv != 0 &&
	    glGetQueryObjectuiv != 0 &&
	    glUnmapBuffer != 0 &&
	    glGetBufferPointerv != 0 &&
	    glDrawBuffers != 0 &&
	    glUniformMatrix2x3fv != 0 &&
	    glUniformMatrix3x2fv != 0 &&
	    glUniformMatrix2x4fv != 0 &&
	    glUniformMatrix4x2fv != 0 &&
	    glUniformMatrix3x4fv != 0 &&
	    glUniformMatrix4x3fv != 0 &&
	    glBlitFramebuffer != 0 &&
	    glRenderbufferStorageMultisample != 0 &&
	    glFramebufferTextureLayer != 0 &&
	    glMapBufferRange != 0 &&
	    glFlushMappedBufferRange != 0 &&
	    glBindVertexArray != 0 &&
	    glDeleteVertexArrays != 0 &&
	    glGenVertexArrays != 0 &&
	    glIsVertexArray != 0 &&
	    glGetIntegeri_v != 0 &&
	    glBeginTransformFeedback != 0 &&
	    glEndTransformFeedback != 0 &&
	    glBindBufferRange != 0 &&
	    glBindBufferBase != 0 &&
	    glTransformFeedbackVaryings != 0 &&
	    glGetTransformFeedbackVarying != 0 &&
	    glVertexAttribIPointer != 0 &&
	    glGetVertexAttribIiv != 0 &&
	    glGetVertexAttribIuiv != 0 &&
	    glVertexAttribI4i != 0 &&
	    glVertexAttribI4ui != 0 &&
	    glVertexAttribI4iv != 0 &&
	    glVertexAttribI4uiv != 0 &&
	    glGetUniformuiv != 0 &&
	    glGetFragDataLocation != 0 &&
	    glUniform1ui != 0 &&
	    glUniform2ui != 0 &&
	    glUniform3ui != 0 &&
	    glUniform4ui != 0 &&
	    glUniform1uiv != 0 &&
	    glUniform2uiv != 0 &&
	    glUniform3uiv != 0 &&
	    glUniform4uiv != 0 &&
	    glClearBufferiv != 0 &&
	    glClearBufferuiv != 0 &&
	    glClearBufferfv != 0 &&
	    glClearBufferfi != 0 &&
	    glGetStringi != 0 &&
	    glCopyBufferSubData != 0 &&
	    glGetUniformIndices != 0 &&
	    glGetActiveUniformsiv != 0 &&
	    glGetUniformBlockIndex != 0 &&
	    glGetActiveUniformBlockiv != 0 &&
	    glGetActiveUniformBlockName != 0 &&
	    glUniformBlockBinding != 0 &&
	    glDrawArraysInstanced != 0 &&
	    glDrawElementsInstanced != 0 &&
	    glFenceSync != 0 &&
	    glIsSync != 0 &&
	    glDeleteSync != 0 &&
	    glClientWaitSync != 0 &&
	    glWaitSync != 0 &&
	    glGetInteger64v != 0 &&
	    glGetSynciv != 0 &&
	    glGetInteger64i_v != 0 &&
	    glGetBufferParameteri64v != 0 &&
	    glGenSamplers != 0 &&
	    glDeleteSamplers != 0 &&
	    glIsSampler != 0 &&
	    glBindSampler != 0 &&
	    glSamplerParameteri != 0 &&
	    glSamplerParameteriv != 0 &&
	    glSamplerParameterf != 0 &&
	    glSamplerParameterfv != 0 &&
	    glGetSamplerParameteriv != 0 &&
	    glGetSamplerParameterfv != 0 &&
	    glVertexAttribDivisor != 0 &&
	    glBindTransformFeedback != 0 &&
	    glDeleteTransformFeedbacks != 0 &&
	    glGenTransformFeedbacks != 0 &&
	    glIsTransformFeedback != 0 &&
	    glPauseTransformFeedback != 0 &&
	    glResumeTransformFeedback != 0 &&
	    glGetProgramBinary != 0 &&
	    glProgramBinary != 0 &&
	    glProgramParameteri != 0 &&
	    glInvalidateFramebuffer != 0 &&
	    glInvalidateSubFramebuffer != 0 &&
	    glTexStorage2D != 0 &&
	    glTexStorage3D != 0 &&
	    glGetInternalformativ != 0
        )
        {
            // All function pointers are non-zero, so any of them
            // should be useable.
            return 1;
        }
        else
        {
            // At least one "extension" failed to load.
            return 0;
        }
#endif

    return 1;
}

//*****************************************************************************
// BuildProgram
//*****************************************************************************
static int BuildProgram(int          nProgramIndex,
                        const char*  pVertex,
                        const char*  pFragment,
                        const char** pFeedbackVaryings      = 0,
                        const int    nFeedbackVaryingsCount = 0)
{
    int nStatus   = 1;
    int nCompiled = 0;
    int nLinked   = 0;

    unsigned int hVertex   = glCreateShader(GL_VERTEX_SHADER);
    unsigned int hFragment = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int hProgram  = glCreateProgram();

    // Assign shader source code to strings passed into function.
    glShaderSource(hVertex,     // Handle to Shader
                   1,           // Number of strings in source array
                   &pVertex,    // Pointer to address of first (and only) string
                   0);          // Length array, 0 = strings are null terminated
    glShaderSource(hFragment,
                   1,
                   &pFragment,
                   0);

    // Compile individual shaders
    glCompileShader(hVertex);
    glCompileShader(hFragment);

    // Check compilation status of vertex shader
    glGetShaderiv(hVertex,
                  GL_COMPILE_STATUS,
                  &nCompiled);

    // If shader didnt compile, print info log.
    if (nCompiled == 0)
    {
        // Clear buffer to hold 
        memset(pLogBuffer, 0, LOG_SIZE);

        // Get compilation log
        glGetShaderInfoLog(hVertex,        // Handle to shader
                           LOG_SIZE - 1,   // Max size of buffer
                           0,              // Returned actual size, 0 = don't care
                           pLogBuffer);    // Pointer to buffer

        LogError("Vertex shader failed to compile");
        LogError(pLogBuffer);

        nStatus = 0;
    }

    // Check compilation status of fragment shader
    glGetShaderiv(hFragment,
                  GL_COMPILE_STATUS,
                  &nCompiled);

    // If shader didnt compile, print info log.
    if (nCompiled == 0)
    {
        // Clear buffer to hold 
        memset(pLogBuffer, 0, LOG_SIZE);

        // Get compilation log
        glGetShaderInfoLog(hFragment,      // Handle to shader
                           LOG_SIZE - 1,   // Max size of buffer
                           0,              // Returned actual size, 0 = don't care
                           pLogBuffer);    // Pointer to buffer

        LogError("Fragment shader failed to compile");
        LogError(pLogBuffer);

        nStatus = 0;
    }

    // If both shaders compiled succesfully, attach them to the program
    // object and link them.
    if (nStatus != 0)
    {
        // Attach individual shaders to the shader program
        glAttachShader(hProgram, hVertex);
	    glAttachShader(hProgram, hFragment);

        // Check to see if there are any feedback varyings to setup
        // before linking the program
        if (pFeedbackVaryings != 0)
        {
            while(glGetError() != GL_NO_ERROR)
            {
            
            }
            glTransformFeedbackVaryings(hProgram,
                                        nFeedbackVaryingsCount,
                                        pFeedbackVaryings,
                                        GL_INTERLEAVED_ATTRIBS);

            printf("glError() = %d\n", glGetError());
        }

        // Link program
        glLinkProgram(hProgram);

        // Check if program linked successfully.
        glGetProgramiv(hProgram,
                       GL_LINK_STATUS,
                       &nLinked);

        // If program didn't link correctly, log an error 
        // and return failure.
        if (nLinked == 0)
        {
            LogError("Shader program failed to link");
            nStatus = 0;
        }
        // Otherwise assign the shader program to specified target
        // in the program array.
        else
        {
            s_arPrograms[nProgramIndex] = hProgram;
        }
    }
    return nStatus;
}

//*****************************************************************************
// LoadShaders
//*****************************************************************************
int LoadShaders()
{
    int nStatus = 1;

    // Clear all the shader program values to 0.
    memset(s_arPrograms, 0, MAX_PROGRAMS*sizeof(int));

    // Quad Shader
    if (nStatus != 0)
    {
        LogDebug("Building Quad Shader");
        nStatus = BuildProgram(QUAD_PROGRAM,
                               pQuadVertexShader,
                               pQuadFragmentShader);
    }

    // Static Diffuse Shader
    if (nStatus != 0)
    {
        LogDebug("Building StaticMesh Diffuse Shader");
        nStatus = BuildProgram(STATIC_DIFFUSE_PROGRAM,
                               pStaticMeshVertexShader,
                               pDiffuseFragmentShader);
    }

    // Animated Diffuse Shader
    if (nStatus != 0)
    {
        LogDebug("Building AnimatedMesh Diffuse Shader");
        nStatus = BuildProgram(ANIMATED_DIFFUSE_PROGRAM,
                               pAnimatedMeshVertexShader,
                               pDiffuseFragmentShader);
    }

    // Text Shader
    if (nStatus != 0)
    {
        LogDebug("Building Text Shader");
        nStatus = BuildProgram(TEXT_PROGRAM,
                               pTextVertexShader,
                               pTextFragmentShader);
    }

    // Static Fullbright Shader
    if (nStatus != 0)
    {
        LogDebug("Building StaticMesh Fullbright Shader");
        nStatus = BuildProgram(STATIC_FULLBRIGHT_PROGRAM,
                               pStaticMeshVertexShader,
                               pFullbrightFragmentShader);
    }

    // Animated Fullbright Shader
    if (nStatus != 0)
    {
        LogDebug("Building AnimatedMesh Fullbright Shader");
        nStatus = BuildProgram(ANIMATED_FULLBRIGHT_PROGRAM,
                               pAnimatedMeshVertexShader,
                               pFullbrightFragmentShader);
    }

    // Blur Effect Shader
    if (nStatus != 0)
    {
        LogDebug("Building BlurEffect Shader");
        nStatus = BuildProgram(BLUR_EFFECT_PROGRAM,
                               pEffectVertexShader,
                               pBlurFragmentShader);
    }

    // Particle Update Shader
    if (nStatus != 0)
    {
        LogDebug("Building Particle Update Shader");
        nStatus = BuildProgram(PARTICLE_UPDATE_PROGRAM,
                               pParticleUpdateVertexShader,
                               pParticleUpdateFragmentShader,
                               pParticleUpdateFeedbackVaryings,
                               nParticleUpdateFeedbackVaryingsCount);
    }

    // Particle Render Shader
    if (nStatus != 0)
    {
        LogDebug("Building Particle Render Shader");
        nStatus = BuildProgram(PARTICLE_RENDER_PROGRAM,
                               pParticleRenderVertexShader,
                               pParticleRenderFragmentShader);
    }

    return nStatus;
}

//*****************************************************************************
// GetShaderProgram
//*****************************************************************************
unsigned int GetShaderProgram(int nIndex)
{
    unsigned int hProg = 0;

    if (nIndex >= 0 &&
        nIndex < MAX_PROGRAMS)
    {
        hProg = s_arPrograms[nIndex];
    }

    return hProg;
}

//*****************************************************************************
// GetMatterShaderProgram
//*****************************************************************************
unsigned int GetMatterShaderProgram(int nMeshType,
                                    int nMaterialType)
{
    if (nMeshType     == MESH_STATIC &&
        nMaterialType == MATERIAL_DIFFUSE)
    {
        return s_arPrograms[STATIC_DIFFUSE_PROGRAM];
    }

    if (nMeshType     == MESH_ANIMATED &&
        nMaterialType == MATERIAL_DIFFUSE)
    {
        return s_arPrograms[ANIMATED_DIFFUSE_PROGRAM];
    }

    return 0;
}
