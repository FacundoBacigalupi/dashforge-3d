#include "glad/glad.h"
#include <string.h>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  static HMODULE libgl;
  static void open_libgl(void) { libgl = LoadLibraryA("opengl32.dll"); }
  static void close_libgl(void) { if (libgl) { FreeLibrary(libgl); libgl = NULL; } }
  static void* get_proc(const char* name) {
      void* p = (void*)wglGetProcAddress(name);
      if (!p || p == (void*)0x1 || p == (void*)0x2 || p == (void*)0x3 || p == (void*)-1)
          p = (void*)GetProcAddress(libgl, name);
      return p;
  }
#elif defined(__APPLE__)
  #include <dlfcn.h>
  static void* libgl;
  static void open_libgl(void) { libgl = dlopen("/System/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY | RTLD_GLOBAL); }
  static void close_libgl(void) { if (libgl) { dlclose(libgl); libgl = NULL; } }
  static void* get_proc(const char* name) { return dlsym(libgl, name); }
#else
  #include <dlfcn.h>
  #include <GL/glx.h>
  static void* libgl;
  static void open_libgl(void) { libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL); }
  static void close_libgl(void) { if (libgl) { dlclose(libgl); libgl = NULL; } }
  static void* get_proc(const char* name) {
      void* p = (void*)glXGetProcAddress((const GLubyte*)name);
      if (!p) p = dlsym(libgl, name);
      return p;
  }
#endif

static GLADloadproc glad_loader_fn = NULL;

static void* glad_get(const char* name) {
    if (glad_loader_fn) return glad_loader_fn(name);
    return get_proc(name);
}

/* ---- Function pointer definitions ---- */
PFNGLVIEWPORTPROC                glad_glViewport = NULL;
PFNGLCLEARPROC                   glad_glClear = NULL;
PFNGLCLEARCOLORPROC              glad_glClearColor = NULL;
PFNGLCLEARDEPTHPROC              glad_glClearDepth = NULL;
PFNGLCLEARDEPTHFPROC             glad_glClearDepthf = NULL;
PFNGLENABLEPROC                  glad_glEnable = NULL;
PFNGLDISABLEPROC                 glad_glDisable = NULL;
PFNGLDEPTHFUNCPROC               glad_glDepthFunc = NULL;
PFNGLDEPTHMASKPROC               glad_glDepthMask = NULL;
PFNGLBLENDFUNCPROC               glad_glBlendFunc = NULL;
PFNGLBLENDFUNCSEPARATEPROC       glad_glBlendFuncSeparate = NULL;
PFNGLBLENDEQUATIONPROC           glad_glBlendEquation = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC   glad_glBlendEquationSeparate = NULL;
PFNGLCULLFACEPROC                glad_glCullFace = NULL;
PFNGLFRONTFACEPROC               glad_glFrontFace = NULL;
PFNGLPOINTSIZEPROC               glad_glPointSize = NULL;
PFNGLLINEWIDTHPROC               glad_glLineWidth = NULL;
PFNGLPOLYGONMODEPROC             glad_glPolygonMode = NULL;
PFNGLPOLYGONOFFSETPROC           glad_glPolygonOffset = NULL;
PFNGLSCISSORPROC                 glad_glScissor = NULL;
PFNGLCOLORMASKPROC               glad_glColorMask = NULL;
PFNGLFINISHPROC                  glad_glFinish = NULL;
PFNGLFLUSHPROC                   glad_glFlush = NULL;
PFNGLGETERRORPROC                glad_glGetError = NULL;
PFNGLGETBOOLEANVPROC             glad_glGetBooleanv = NULL;
PFNGLGETINTEGERVPROC             glad_glGetIntegerv = NULL;
PFNGLGETFLOATVPROC               glad_glGetFloatv = NULL;
PFNGLGETDOUBLEVPROC              glad_glGetDoublev = NULL;
PFNGLGETSTRINGPROC               glad_glGetString = NULL;
PFNGLGETSTRINGIPROC              glad_glGetStringi = NULL;
PFNGLPIXELSTOREIPROC             glad_glPixelStorei = NULL;
PFNGLPIXELSTOREFPROC             glad_glPixelStoref = NULL;
PFNGLREADPIXELSPROC              glad_glReadPixels = NULL;
PFNGLGENTEXTURESPROC             glad_glGenTextures = NULL;
PFNGLBINDTEXTUREPROC             glad_glBindTexture = NULL;
PFNGLDELETETEXTURESPROC          glad_glDeleteTextures = NULL;
PFNGLTEXIMAGE2DPROC              glad_glTexImage2D = NULL;
PFNGLTEXIMAGE1DPROC              glad_glTexImage1D = NULL;
PFNGLTEXIMAGE3DPROC              glad_glTexImage3D = NULL;
PFNGLTEXSUBIMAGE2DPROC           glad_glTexSubImage2D = NULL;
PFNGLTEXPARAMETERIPROC           glad_glTexParameteri = NULL;
PFNGLTEXPARAMETERFPROC           glad_glTexParameterf = NULL;
PFNGLACTIVETEXTUREPROC           glad_glActiveTexture = NULL;
PFNGLGENERATEMIPMAPPROC          glad_glGenerateMipmap = NULL;
PFNGLCOPYTEXIMAGE2DPROC          glad_glCopyTexImage2D = NULL;
PFNGLGENBUFFERSPROC              glad_glGenBuffers = NULL;
PFNGLBINDBUFFERPROC              glad_glBindBuffer = NULL;
PFNGLDELETEBUFFERSPROC           glad_glDeleteBuffers = NULL;
PFNGLBUFFERDATAPROC              glad_glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC           glad_glBufferSubData = NULL;
PFNGLMAPBUFFERPROC               glad_glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC             glad_glUnmapBuffer = NULL;
PFNGLBINDBUFFERBASEPROC          glad_glBindBufferBase = NULL;
PFNGLBINDBUFFERRANGEPROC         glad_glBindBufferRange = NULL;
PFNGLGENVERTEXARRAYSPROC         glad_glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC         glad_glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSPROC      glad_glDeleteVertexArrays = NULL;
PFNGLVERTEXATTRIBPOINTERPROC     glad_glVertexAttribPointer = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC    glad_glVertexAttribIPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBDIVISORPROC     glad_glVertexAttribDivisor = NULL;
PFNGLDRAWARRAYSPROC              glad_glDrawArrays = NULL;
PFNGLDRAWELEMENTSPROC            glad_glDrawElements = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC     glad_glDrawArraysInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC   glad_glDrawElementsInstanced = NULL;
PFNGLDRAWBUFFERPROC              glad_glDrawBuffer = NULL;
PFNGLDRAWBUFFERSPROC             glad_glDrawBuffers = NULL;
PFNGLCREATESHADERPROC            glad_glCreateShader = NULL;
PFNGLDELETESHADERPROC            glad_glDeleteShader = NULL;
PFNGLSHADERSOURCEPROC            glad_glShaderSource = NULL;
PFNGLCOMPILESHADERPROC           glad_glCompileShader = NULL;
PFNGLGETSHADERIVPROC             glad_glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC        glad_glGetShaderInfoLog = NULL;
PFNGLCREATEPROGRAMPROC           glad_glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC           glad_glDeleteProgram = NULL;
PFNGLATTACHSHADERPROC            glad_glAttachShader = NULL;
PFNGLDETACHSHADERPROC            glad_glDetachShader = NULL;
PFNGLLINKPROGRAMPROC             glad_glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC              glad_glUseProgram = NULL;
PFNGLGETPROGRAMIVPROC            glad_glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC       glad_glGetProgramInfoLog = NULL;
PFNGLVALIDATEPROGRAMPROC         glad_glValidateProgram = NULL;
PFNGLISPROGRAMPROC               glad_glIsProgram = NULL;
PFNGLISSHADERPROC                glad_glIsShader = NULL;
PFNGLBINDATTRIBLOCATIONPROC      glad_glBindAttribLocation = NULL;
PFNGLGETATTRIBLOCATIONPROC       glad_glGetAttribLocation = NULL;
PFNGLGETUNIFORMLOCATIONPROC      glad_glGetUniformLocation = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC    glad_glGetUniformBlockIndex = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC     glad_glUniformBlockBinding = NULL;
PFNGLUNIFORM1IPROC               glad_glUniform1i = NULL;
PFNGLUNIFORM2IPROC               glad_glUniform2i = NULL;
PFNGLUNIFORM3IPROC               glad_glUniform3i = NULL;
PFNGLUNIFORM4IPROC               glad_glUniform4i = NULL;
PFNGLUNIFORM1FPROC               glad_glUniform1f = NULL;
PFNGLUNIFORM2FPROC               glad_glUniform2f = NULL;
PFNGLUNIFORM3FPROC               glad_glUniform3f = NULL;
PFNGLUNIFORM4FPROC               glad_glUniform4f = NULL;
PFNGLUNIFORM1IVPROC              glad_glUniform1iv = NULL;
PFNGLUNIFORM2IVPROC              glad_glUniform2iv = NULL;
PFNGLUNIFORM3IVPROC              glad_glUniform3iv = NULL;
PFNGLUNIFORM4IVPROC              glad_glUniform4iv = NULL;
PFNGLUNIFORM1FVPROC              glad_glUniform1fv = NULL;
PFNGLUNIFORM2FVPROC              glad_glUniform2fv = NULL;
PFNGLUNIFORM3FVPROC              glad_glUniform3fv = NULL;
PFNGLUNIFORM4FVPROC              glad_glUniform4fv = NULL;
PFNGLUNIFORMMATRIX2FVPROC        glad_glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC        glad_glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC        glad_glUniformMatrix4fv = NULL;
PFNGLUNIFORMMATRIX2X3FVPROC      glad_glUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX3X2FVPROC      glad_glUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX2X4FVPROC      glad_glUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX4X2FVPROC      glad_glUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX3X4FVPROC      glad_glUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4X3FVPROC      glad_glUniformMatrix4x3fv = NULL;
PFNGLGENFRAMEBUFFERSPROC         glad_glGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC         glad_glBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERSPROC      glad_glDeleteFramebuffers = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC    glad_glFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC      glad_glFramebufferTexture = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC  glad_glCheckFramebufferStatus = NULL;
PFNGLBLITFRAMEBUFFERPROC         glad_glBlitFramebuffer = NULL;
PFNGLGENRENDERBUFFERSPROC        glad_glGenRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC        glad_glBindRenderbuffer = NULL;
PFNGLDELETERENDERBUFFERSPROC     glad_glDeleteRenderbuffers = NULL;
PFNGLRENDERBUFFERSTORAGEPROC     glad_glRenderbufferStorage = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample = NULL;

static int load_gl(void) {
    glad_glViewport              = (PFNGLVIEWPORTPROC)              glad_get("glViewport");
    glad_glClear                 = (PFNGLCLEARPROC)                 glad_get("glClear");
    glad_glClearColor            = (PFNGLCLEARCOLORPROC)            glad_get("glClearColor");
    glad_glClearDepth            = (PFNGLCLEARDEPTHPROC)            glad_get("glClearDepth");
    glad_glClearDepthf           = (PFNGLCLEARDEPTHFPROC)           glad_get("glClearDepthf");
    glad_glEnable                = (PFNGLENABLEPROC)                 glad_get("glEnable");
    glad_glDisable               = (PFNGLDISABLEPROC)               glad_get("glDisable");
    glad_glDepthFunc             = (PFNGLDEPTHFUNCPROC)             glad_get("glDepthFunc");
    glad_glDepthMask             = (PFNGLDEPTHMASKPROC)             glad_get("glDepthMask");
    glad_glBlendFunc             = (PFNGLBLENDFUNCPROC)             glad_get("glBlendFunc");
    glad_glBlendFuncSeparate     = (PFNGLBLENDFUNCSEPARATEPROC)     glad_get("glBlendFuncSeparate");
    glad_glBlendEquation         = (PFNGLBLENDEQUATIONPROC)         glad_get("glBlendEquation");
    glad_glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) glad_get("glBlendEquationSeparate");
    glad_glCullFace              = (PFNGLCULLFACEPROC)              glad_get("glCullFace");
    glad_glFrontFace             = (PFNGLFRONTFACEPROC)             glad_get("glFrontFace");
    glad_glPointSize             = (PFNGLPOINTSIZEPROC)             glad_get("glPointSize");
    glad_glLineWidth             = (PFNGLLINEWIDTHPROC)             glad_get("glLineWidth");
    glad_glPolygonMode           = (PFNGLPOLYGONMODEPROC)           glad_get("glPolygonMode");
    glad_glPolygonOffset         = (PFNGLPOLYGONOFFSETPROC)         glad_get("glPolygonOffset");
    glad_glScissor               = (PFNGLSCISSORPROC)               glad_get("glScissor");
    glad_glColorMask             = (PFNGLCOLORMASKPROC)             glad_get("glColorMask");
    glad_glFinish                = (PFNGLFINISHPROC)                glad_get("glFinish");
    glad_glFlush                 = (PFNGLFLUSHPROC)                 glad_get("glFlush");
    glad_glGetError              = (PFNGLGETERRORPROC)              glad_get("glGetError");
    glad_glGetBooleanv           = (PFNGLGETBOOLEANVPROC)           glad_get("glGetBooleanv");
    glad_glGetIntegerv           = (PFNGLGETINTEGERVPROC)           glad_get("glGetIntegerv");
    glad_glGetFloatv             = (PFNGLGETFLOATVPROC)             glad_get("glGetFloatv");
    glad_glGetDoublev            = (PFNGLGETDOUBLEVPROC)            glad_get("glGetDoublev");
    glad_glGetString             = (PFNGLGETSTRINGPROC)             glad_get("glGetString");
    glad_glGetStringi            = (PFNGLGETSTRINGIPROC)            glad_get("glGetStringi");
    glad_glPixelStorei           = (PFNGLPIXELSTOREIPROC)           glad_get("glPixelStorei");
    glad_glPixelStoref           = (PFNGLPIXELSTOREFPROC)           glad_get("glPixelStoref");
    glad_glReadPixels            = (PFNGLREADPIXELSPROC)            glad_get("glReadPixels");
    glad_glGenTextures           = (PFNGLGENTEXTURESPROC)           glad_get("glGenTextures");
    glad_glBindTexture           = (PFNGLBINDTEXTUREPROC)           glad_get("glBindTexture");
    glad_glDeleteTextures        = (PFNGLDELETETEXTURESPROC)        glad_get("glDeleteTextures");
    glad_glTexImage2D            = (PFNGLTEXIMAGE2DPROC)            glad_get("glTexImage2D");
    glad_glTexImage1D            = (PFNGLTEXIMAGE1DPROC)            glad_get("glTexImage1D");
    glad_glTexImage3D            = (PFNGLTEXIMAGE3DPROC)            glad_get("glTexImage3D");
    glad_glTexSubImage2D         = (PFNGLTEXSUBIMAGE2DPROC)         glad_get("glTexSubImage2D");
    glad_glTexParameteri         = (PFNGLTEXPARAMETERIPROC)         glad_get("glTexParameteri");
    glad_glTexParameterf         = (PFNGLTEXPARAMETERFPROC)         glad_get("glTexParameterf");
    glad_glActiveTexture         = (PFNGLACTIVETEXTUREPROC)         glad_get("glActiveTexture");
    glad_glGenerateMipmap        = (PFNGLGENERATEMIPMAPPROC)        glad_get("glGenerateMipmap");
    glad_glCopyTexImage2D        = (PFNGLCOPYTEXIMAGE2DPROC)        glad_get("glCopyTexImage2D");
    glad_glGenBuffers            = (PFNGLGENBUFFERSPROC)            glad_get("glGenBuffers");
    glad_glBindBuffer            = (PFNGLBINDBUFFERPROC)            glad_get("glBindBuffer");
    glad_glDeleteBuffers         = (PFNGLDELETEBUFFERSPROC)         glad_get("glDeleteBuffers");
    glad_glBufferData            = (PFNGLBUFFERDATAPROC)            glad_get("glBufferData");
    glad_glBufferSubData         = (PFNGLBUFFERSUBDATAPROC)         glad_get("glBufferSubData");
    glad_glMapBuffer             = (PFNGLMAPBUFFERPROC)             glad_get("glMapBuffer");
    glad_glUnmapBuffer           = (PFNGLUNMAPBUFFERPROC)           glad_get("glUnmapBuffer");
    glad_glBindBufferBase        = (PFNGLBINDBUFFERBASEPROC)        glad_get("glBindBufferBase");
    glad_glBindBufferRange       = (PFNGLBINDBUFFERRANGEPROC)       glad_get("glBindBufferRange");
    glad_glGenVertexArrays       = (PFNGLGENVERTEXARRAYSPROC)       glad_get("glGenVertexArrays");
    glad_glBindVertexArray       = (PFNGLBINDVERTEXARRAYPROC)       glad_get("glBindVertexArray");
    glad_glDeleteVertexArrays    = (PFNGLDELETEVERTEXARRAYSPROC)    glad_get("glDeleteVertexArrays");
    glad_glVertexAttribPointer   = (PFNGLVERTEXATTRIBPOINTERPROC)   glad_get("glVertexAttribPointer");
    glad_glVertexAttribIPointer  = (PFNGLVERTEXATTRIBIPOINTERPROC)  glad_get("glVertexAttribIPointer");
    glad_glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)  glad_get("glEnableVertexAttribArray");
    glad_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) glad_get("glDisableVertexAttribArray");
    glad_glVertexAttribDivisor   = (PFNGLVERTEXATTRIBDIVISORPROC)   glad_get("glVertexAttribDivisor");
    glad_glDrawArrays            = (PFNGLDRAWARRAYSPROC)            glad_get("glDrawArrays");
    glad_glDrawElements          = (PFNGLDRAWELEMENTSPROC)          glad_get("glDrawElements");
    glad_glDrawArraysInstanced   = (PFNGLDRAWARRAYSINSTANCEDPROC)   glad_get("glDrawArraysInstanced");
    glad_glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC) glad_get("glDrawElementsInstanced");
    glad_glDrawBuffer            = (PFNGLDRAWBUFFERPROC)            glad_get("glDrawBuffer");
    glad_glDrawBuffers           = (PFNGLDRAWBUFFERSPROC)           glad_get("glDrawBuffers");
    glad_glCreateShader          = (PFNGLCREATESHADERPROC)          glad_get("glCreateShader");
    glad_glDeleteShader          = (PFNGLDELETESHADERPROC)          glad_get("glDeleteShader");
    glad_glShaderSource          = (PFNGLSHADERSOURCEPROC)          glad_get("glShaderSource");
    glad_glCompileShader         = (PFNGLCOMPILESHADERPROC)         glad_get("glCompileShader");
    glad_glGetShaderiv           = (PFNGLGETSHADERIVPROC)           glad_get("glGetShaderiv");
    glad_glGetShaderInfoLog      = (PFNGLGETSHADERINFOLOGPROC)      glad_get("glGetShaderInfoLog");
    glad_glCreateProgram         = (PFNGLCREATEPROGRAMPROC)         glad_get("glCreateProgram");
    glad_glDeleteProgram         = (PFNGLDELETEPROGRAMPROC)         glad_get("glDeleteProgram");
    glad_glAttachShader          = (PFNGLATTACHSHADERPROC)          glad_get("glAttachShader");
    glad_glDetachShader          = (PFNGLDETACHSHADERPROC)          glad_get("glDetachShader");
    glad_glLinkProgram           = (PFNGLLINKPROGRAMPROC)           glad_get("glLinkProgram");
    glad_glUseProgram            = (PFNGLUSEPROGRAMPROC)            glad_get("glUseProgram");
    glad_glGetProgramiv          = (PFNGLGETPROGRAMIVPROC)          glad_get("glGetProgramiv");
    glad_glGetProgramInfoLog     = (PFNGLGETPROGRAMINFOLOGPROC)     glad_get("glGetProgramInfoLog");
    glad_glValidateProgram       = (PFNGLVALIDATEPROGRAMPROC)       glad_get("glValidateProgram");
    glad_glIsProgram             = (PFNGLISPROGRAMPROC)             glad_get("glIsProgram");
    glad_glIsShader              = (PFNGLISSHADERPROC)              glad_get("glIsShader");
    glad_glBindAttribLocation    = (PFNGLBINDATTRIBLOCATIONPROC)    glad_get("glBindAttribLocation");
    glad_glGetAttribLocation     = (PFNGLGETATTRIBLOCATIONPROC)     glad_get("glGetAttribLocation");
    glad_glGetUniformLocation    = (PFNGLGETUNIFORMLOCATIONPROC)    glad_get("glGetUniformLocation");
    glad_glGetUniformBlockIndex  = (PFNGLGETUNIFORMBLOCKINDEXPROC)  glad_get("glGetUniformBlockIndex");
    glad_glUniformBlockBinding   = (PFNGLUNIFORMBLOCKBINDINGPROC)   glad_get("glUniformBlockBinding");
    glad_glUniform1i             = (PFNGLUNIFORM1IPROC)             glad_get("glUniform1i");
    glad_glUniform2i             = (PFNGLUNIFORM2IPROC)             glad_get("glUniform2i");
    glad_glUniform3i             = (PFNGLUNIFORM3IPROC)             glad_get("glUniform3i");
    glad_glUniform4i             = (PFNGLUNIFORM4IPROC)             glad_get("glUniform4i");
    glad_glUniform1f             = (PFNGLUNIFORM1FPROC)             glad_get("glUniform1f");
    glad_glUniform2f             = (PFNGLUNIFORM2FPROC)             glad_get("glUniform2f");
    glad_glUniform3f             = (PFNGLUNIFORM3FPROC)             glad_get("glUniform3f");
    glad_glUniform4f             = (PFNGLUNIFORM4FPROC)             glad_get("glUniform4f");
    glad_glUniform1iv            = (PFNGLUNIFORM1IVPROC)            glad_get("glUniform1iv");
    glad_glUniform2iv            = (PFNGLUNIFORM2IVPROC)            glad_get("glUniform2iv");
    glad_glUniform3iv            = (PFNGLUNIFORM3IVPROC)            glad_get("glUniform3iv");
    glad_glUniform4iv            = (PFNGLUNIFORM4IVPROC)            glad_get("glUniform4iv");
    glad_glUniform1fv            = (PFNGLUNIFORM1FVPROC)            glad_get("glUniform1fv");
    glad_glUniform2fv            = (PFNGLUNIFORM2FVPROC)            glad_get("glUniform2fv");
    glad_glUniform3fv            = (PFNGLUNIFORM3FVPROC)            glad_get("glUniform3fv");
    glad_glUniform4fv            = (PFNGLUNIFORM4FVPROC)            glad_get("glUniform4fv");
    glad_glUniformMatrix2fv      = (PFNGLUNIFORMMATRIX2FVPROC)      glad_get("glUniformMatrix2fv");
    glad_glUniformMatrix3fv      = (PFNGLUNIFORMMATRIX3FVPROC)      glad_get("glUniformMatrix3fv");
    glad_glUniformMatrix4fv      = (PFNGLUNIFORMMATRIX4FVPROC)      glad_get("glUniformMatrix4fv");
    glad_glUniformMatrix2x3fv    = (PFNGLUNIFORMMATRIX2X3FVPROC)    glad_get("glUniformMatrix2x3fv");
    glad_glUniformMatrix3x2fv    = (PFNGLUNIFORMMATRIX3X2FVPROC)    glad_get("glUniformMatrix3x2fv");
    glad_glUniformMatrix2x4fv    = (PFNGLUNIFORMMATRIX2X4FVPROC)    glad_get("glUniformMatrix2x4fv");
    glad_glUniformMatrix4x2fv    = (PFNGLUNIFORMMATRIX4X2FVPROC)    glad_get("glUniformMatrix4x2fv");
    glad_glUniformMatrix3x4fv    = (PFNGLUNIFORMMATRIX3X4FVPROC)    glad_get("glUniformMatrix3x4fv");
    glad_glUniformMatrix4x3fv    = (PFNGLUNIFORMMATRIX4X3FVPROC)    glad_get("glUniformMatrix4x3fv");
    glad_glGenFramebuffers       = (PFNGLGENFRAMEBUFFERSPROC)       glad_get("glGenFramebuffers");
    glad_glBindFramebuffer       = (PFNGLBINDFRAMEBUFFERPROC)       glad_get("glBindFramebuffer");
    glad_glDeleteFramebuffers    = (PFNGLDELETEFRAMEBUFFERSPROC)    glad_get("glDeleteFramebuffers");
    glad_glFramebufferTexture2D  = (PFNGLFRAMEBUFFERTEXTURE2DPROC)  glad_get("glFramebufferTexture2D");
    glad_glFramebufferTexture    = (PFNGLFRAMEBUFFERTEXTUREPROC)    glad_get("glFramebufferTexture");
    glad_glFramebufferRenderbuffer=(PFNGLFRAMEBUFFERRENDERBUFFERPROC)glad_get("glFramebufferRenderbuffer");
    glad_glCheckFramebufferStatus= (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glad_get("glCheckFramebufferStatus");
    glad_glBlitFramebuffer       = (PFNGLBLITFRAMEBUFFERPROC)       glad_get("glBlitFramebuffer");
    glad_glGenRenderbuffers      = (PFNGLGENRENDERBUFFERSPROC)      glad_get("glGenRenderbuffers");
    glad_glBindRenderbuffer      = (PFNGLBINDRENDERBUFFERPROC)      glad_get("glBindRenderbuffer");
    glad_glDeleteRenderbuffers   = (PFNGLDELETERENDERBUFFERSPROC)   glad_get("glDeleteRenderbuffers");
    glad_glRenderbufferStorage   = (PFNGLRENDERBUFFERSTORAGEPROC)   glad_get("glRenderbufferStorage");
    glad_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) glad_get("glRenderbufferStorageMultisample");

    return (glad_glCreateShader != NULL && glad_glGenVertexArrays != NULL);
}

int gladLoadGLLoader(GLADloadproc load) {
    glad_loader_fn = load;
    return load_gl();
}

int gladLoadGL(void) {
    open_libgl();
    int result = load_gl();
    if (!result) close_libgl();
    return result;
}
