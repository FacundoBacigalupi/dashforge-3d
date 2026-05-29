#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
#error "OpenGL already included. Include glad/glad.h first."
#endif
#define __gl_h_
#define __gl2_h_
#define __gl3_h_

#ifdef _WIN32
  #ifndef APIENTRY
    #define APIENTRY __stdcall
  #endif
  #ifndef WINGDIAPI
    #define WINGDIAPI __declspec(dllimport)
  #endif
#else
  #ifndef APIENTRY
    #define APIENTRY
  #endif
#endif

#ifndef APIENTRYP
  #define APIENTRYP APIENTRY *
#endif
#ifndef GLAPIENTRY
  #define GLAPIENTRY APIENTRY
#endif

#include <stddef.h>
#include <stdint.h>

/* ---- Types ---- */
typedef unsigned int    GLenum;
typedef unsigned char   GLboolean;
typedef unsigned int    GLbitfield;
typedef void            GLvoid;
typedef int8_t          GLbyte;
typedef uint8_t         GLubyte;
typedef int16_t         GLshort;
typedef uint16_t        GLushort;
typedef int             GLint;
typedef unsigned int    GLuint;
typedef int             GLsizei;
typedef float           GLfloat;
typedef float           GLclampf;
typedef double          GLdouble;
typedef double          GLclampd;
typedef char            GLchar;
typedef ptrdiff_t       GLsizeiptr;
typedef ptrdiff_t       GLintptr;
typedef int64_t         GLint64;
typedef uint64_t        GLuint64;
typedef uint16_t        GLhalf;
typedef void*           GLsync;

/* ---- Constants ---- */
#define GL_FALSE                                0
#define GL_TRUE                                 1
#define GL_NONE                                 0
#define GL_ZERO                                 0
#define GL_ONE                                  1

/* clear bits */
#define GL_DEPTH_BUFFER_BIT                     0x00000100
#define GL_STENCIL_BUFFER_BIT                   0x00000400
#define GL_COLOR_BUFFER_BIT                     0x00004000

/* primitives */
#define GL_POINTS                               0x0000
#define GL_LINES                                0x0001
#define GL_LINE_LOOP                            0x0002
#define GL_LINE_STRIP                           0x0003
#define GL_TRIANGLES                            0x0004
#define GL_TRIANGLE_STRIP                       0x0005
#define GL_TRIANGLE_FAN                         0x0006

/* blending */
#define GL_SRC_COLOR                            0x0300
#define GL_ONE_MINUS_SRC_COLOR                  0x0301
#define GL_SRC_ALPHA                            0x0302
#define GL_ONE_MINUS_SRC_ALPHA                  0x0303
#define GL_DST_ALPHA                            0x0304
#define GL_ONE_MINUS_DST_ALPHA                  0x0305
#define GL_DST_COLOR                            0x0306
#define GL_ONE_MINUS_DST_COLOR                  0x0307
#define GL_SRC_ALPHA_SATURATE                   0x0308
#define GL_FUNC_ADD                             0x8006
#define GL_FUNC_SUBTRACT                        0x800A
#define GL_FUNC_REVERSE_SUBTRACT                0x800B
#define GL_BLEND_COLOR                          0x8005
#define GL_BLEND_EQUATION                       0x8009

/* depth / stencil */
#define GL_NEVER                                0x0200
#define GL_LESS                                 0x0201
#define GL_EQUAL                                0x0202
#define GL_LEQUAL                               0x0203
#define GL_GREATER                              0x0204
#define GL_NOTEQUAL                             0x0205
#define GL_GEQUAL                               0x0206
#define GL_ALWAYS                               0x0207

/* culling */
#define GL_FRONT                                0x0404
#define GL_BACK                                 0x0405
#define GL_FRONT_AND_BACK                       0x0408
#define GL_CW                                   0x0900
#define GL_CCW                                  0x0901

/* polygon */
#define GL_POINT                                0x1B00
#define GL_LINE                                 0x1B01
#define GL_FILL                                 0x1B02

/* enables */
#define GL_CULL_FACE                            0x0B44
#define GL_DEPTH_TEST                           0x0B71
#define GL_STENCIL_TEST                         0x0B90
#define GL_DITHER                               0x0BD0
#define GL_BLEND                                0x0BE2
#define GL_SCISSOR_TEST                         0x0C11
#define GL_LINE_SMOOTH                          0x0B20
#define GL_POLYGON_SMOOTH                       0x0B41
#define GL_PROGRAM_POINT_SIZE                   0x8642

/* errors */
#define GL_NO_ERROR                             0
#define GL_INVALID_ENUM                         0x0500
#define GL_INVALID_VALUE                        0x0501
#define GL_INVALID_OPERATION                    0x0502
#define GL_STACK_OVERFLOW                       0x0503
#define GL_STACK_UNDERFLOW                      0x0504
#define GL_OUT_OF_MEMORY                        0x0505

/* types */
#define GL_BYTE                                 0x1400
#define GL_UNSIGNED_BYTE                        0x1401
#define GL_SHORT                                0x1402
#define GL_UNSIGNED_SHORT                       0x1403
#define GL_INT                                  0x1404
#define GL_UNSIGNED_INT                         0x1405
#define GL_FLOAT                                0x1406
#define GL_DOUBLE                               0x140A
#define GL_HALF_FLOAT                           0x140B

/* pixel formats */
#define GL_RED                                  0x1903
#define GL_GREEN                                0x1904
#define GL_BLUE                                 0x1905
#define GL_ALPHA                                0x1906
#define GL_RGB                                  0x1907
#define GL_RGBA                                 0x1908
#define GL_BGR                                  0x80E0
#define GL_BGRA                                 0x80E1
#define GL_RG                                   0x8227

/* pixel internal formats */
#define GL_R8                                   0x8229
#define GL_RG8                                  0x822B
#define GL_RGB8                                 0x8051
#define GL_RGBA8                                0x8058
#define GL_DEPTH_COMPONENT                      0x1902
#define GL_DEPTH_COMPONENT16                    0x81A5
#define GL_DEPTH_COMPONENT24                    0x81A6
#define GL_DEPTH_COMPONENT32                    0x81A7
#define GL_DEPTH_STENCIL                        0x84F9
#define GL_UNSIGNED_INT_24_8                    0x84FA
#define GL_DEPTH24_STENCIL8                     0x88F0

/* textures */
#define GL_TEXTURE_1D                           0x0DE0
#define GL_TEXTURE_2D                           0x0DE1
#define GL_TEXTURE_3D                           0x806F
#define GL_TEXTURE_CUBE_MAP                     0x8513
#define GL_TEXTURE_WRAP_S                       0x2802
#define GL_TEXTURE_WRAP_T                       0x2803
#define GL_TEXTURE_WRAP_R                       0x8072
#define GL_TEXTURE_MAG_FILTER                   0x2800
#define GL_TEXTURE_MIN_FILTER                   0x2801
#define GL_NEAREST                              0x2600
#define GL_LINEAR                               0x2601
#define GL_NEAREST_MIPMAP_NEAREST               0x2700
#define GL_LINEAR_MIPMAP_NEAREST                0x2701
#define GL_NEAREST_MIPMAP_LINEAR                0x2702
#define GL_LINEAR_MIPMAP_LINEAR                 0x2703
#define GL_REPEAT                               0x2901
#define GL_CLAMP_TO_EDGE                        0x812F
#define GL_MIRRORED_REPEAT                      0x8370
#define GL_TEXTURE_MAX_LEVEL                    0x813D
#define GL_TEXTURE0                             0x84C0
#define GL_TEXTURE1                             0x84C1
#define GL_TEXTURE2                             0x84C2
#define GL_TEXTURE3                             0x84C3
#define GL_TEXTURE4                             0x84C4
#define GL_TEXTURE5                             0x84C5
#define GL_TEXTURE6                             0x84C6
#define GL_TEXTURE7                             0x84C7
#define GL_TEXTURE8                             0x84C8
#define GL_TEXTURE9                             0x84C9
#define GL_TEXTURE10                            0x84CA
#define GL_TEXTURE11                            0x84CB
#define GL_TEXTURE12                            0x84CC
#define GL_TEXTURE13                            0x84CD
#define GL_TEXTURE14                            0x84CE
#define GL_TEXTURE15                            0x84CF
#define GL_ACTIVE_TEXTURE                       0x84E0
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS     0x8B4D
#define GL_MAX_TEXTURE_IMAGE_UNITS              0x8872
#define GL_MAX_TEXTURE_SIZE                     0x0D33

/* buffers */
#define GL_ARRAY_BUFFER                         0x8892
#define GL_ELEMENT_ARRAY_BUFFER                 0x8893
#define GL_UNIFORM_BUFFER                       0x8A11
#define GL_PIXEL_PACK_BUFFER                    0x88EB
#define GL_PIXEL_UNPACK_BUFFER                  0x88EC
#define GL_STREAM_DRAW                          0x88E0
#define GL_STREAM_READ                          0x88E1
#define GL_STATIC_DRAW                          0x88E4
#define GL_STATIC_READ                          0x88E5
#define GL_DYNAMIC_DRAW                         0x88E8
#define GL_DYNAMIC_READ                         0x88E9

/* shaders */
#define GL_VERTEX_SHADER                        0x8B31
#define GL_FRAGMENT_SHADER                      0x8B30
#define GL_GEOMETRY_SHADER                      0x8DD9
#define GL_COMPILE_STATUS                       0x8B81
#define GL_LINK_STATUS                          0x8B82
#define GL_VALIDATE_STATUS                      0x8B83
#define GL_INFO_LOG_LENGTH                      0x8B84
#define GL_SHADER_TYPE                          0x8B4F
#define GL_DELETE_STATUS                        0x8B80
#define GL_ATTACHED_SHADERS                     0x8B85
#define GL_ACTIVE_UNIFORMS                      0x8B86
#define GL_ACTIVE_ATTRIBUTES                    0x8B89
#define GL_MAX_VERTEX_ATTRIBS                   0x8869
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS        0x8B4A
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS      0x8B49
#define GL_SHADING_LANGUAGE_VERSION             0x8B8C
#define GL_CURRENT_PROGRAM                      0x8B8D
#define GL_FLOAT_VEC2                           0x8B50
#define GL_FLOAT_VEC3                           0x8B51
#define GL_FLOAT_VEC4                           0x8B52
#define GL_INT_VEC2                             0x8B53
#define GL_INT_VEC3                             0x8B54
#define GL_INT_VEC4                             0x8B55
#define GL_BOOL                                 0x8B56
#define GL_FLOAT_MAT2                           0x8B5A
#define GL_FLOAT_MAT3                           0x8B5B
#define GL_FLOAT_MAT4                           0x8B5C
#define GL_SAMPLER_2D                           0x8B5E
#define GL_SAMPLER_3D                           0x8B5F

/* framebuffers */
#define GL_FRAMEBUFFER                          0x8D40
#define GL_READ_FRAMEBUFFER                     0x8CA8
#define GL_DRAW_FRAMEBUFFER                     0x8CA9
#define GL_RENDERBUFFER                         0x8D41
#define GL_COLOR_ATTACHMENT0                    0x8CE0
#define GL_COLOR_ATTACHMENT1                    0x8CE1
#define GL_DEPTH_ATTACHMENT                     0x8D00
#define GL_STENCIL_ATTACHMENT                   0x8D20
#define GL_DEPTH_STENCIL_ATTACHMENT             0x821A
#define GL_FRAMEBUFFER_COMPLETE                 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT    0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER   0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER   0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED              0x8CDD
#define GL_FRAMEBUFFER_BINDING                  0x8CA6

/* pixel store */
#define GL_UNPACK_ALIGNMENT                     0x0CF5
#define GL_PACK_ALIGNMENT                       0x0D05
#define GL_UNPACK_ROW_LENGTH                    0x0CF2

/* queries */
#define GL_VIEWPORT                             0x0BA2
#define GL_SCISSOR_BOX                          0x0C10
#define GL_SCISSOR_TEST                         0x0C11
#define GL_COLOR_CLEAR_VALUE                    0x0C22
#define GL_DEPTH_CLEAR_VALUE                    0x0B73

/* string queries */
#define GL_VENDOR                               0x1F00
#define GL_RENDERER                             0x1F01
#define GL_VERSION                              0x1F02
#define GL_EXTENSIONS                           0x1F03
#define GL_NUM_EXTENSIONS                       0x821D

/* instancing */
#define GL_DRAW_INDIRECT_BUFFER                 0x8F3F

/* ---- Function pointer typedefs ---- */
typedef void (APIENTRYP PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei w, GLsizei h);
typedef void (APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);
typedef void (APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
typedef void (APIENTRYP PFNGLCLEARDEPTHPROC)(GLdouble depth);
typedef void (APIENTRYP PFNGLCLEARDEPTHFPROC)(GLfloat depth);
typedef void (APIENTRYP PFNGLENABLEPROC)(GLenum cap);
typedef void (APIENTRYP PFNGLDISABLEPROC)(GLenum cap);
typedef GLboolean (APIENTRYP PFNGLISENABLEDIPROC)(GLenum target, GLuint index);
typedef void (APIENTRYP PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void (APIENTRYP PFNGLDEPTHMASKPROC)(GLboolean flag);
typedef void (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEPROC)(GLenum srcRGB, GLenum dstRGB, GLenum srcA, GLenum dstA);
typedef void (APIENTRYP PFNGLBLENDEQUATIONPROC)(GLenum mode);
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB, GLenum modeA);
typedef void (APIENTRYP PFNGLCULLFACEPROC)(GLenum mode);
typedef void (APIENTRYP PFNGLFRONTFACEPROC)(GLenum mode);
typedef void (APIENTRYP PFNGLPOINTSIZEPROC)(GLfloat size);
typedef void (APIENTRYP PFNGLLINEWIDTHPROC)(GLfloat width);
typedef void (APIENTRYP PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
typedef void (APIENTRYP PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
typedef void (APIENTRYP PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei w, GLsizei h);
typedef void (APIENTRYP PFNGLCOLORMASKPROC)(GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (APIENTRYP PFNGLFINISHPROC)(void);
typedef void (APIENTRYP PFNGLFLUSHPROC)(void);
typedef GLenum (APIENTRYP PFNGLGETERRORPROC)(void);
typedef void (APIENTRYP PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean* data);
typedef void (APIENTRYP PFNGLGETINTEGERVPROC)(GLenum pname, GLint* data);
typedef void (APIENTRYP PFNGLGETFLOATVPROC)(GLenum pname, GLfloat* data);
typedef void (APIENTRYP PFNGLGETDOUBLEVPROC)(GLenum pname, GLdouble* data);
typedef const GLubyte* (APIENTRYP PFNGLGETSTRINGPROC)(GLenum name);
typedef const GLubyte* (APIENTRYP PFNGLGETSTRINGIPROC)(GLenum name, GLuint index);
typedef void (APIENTRYP PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
typedef void (APIENTRYP PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei w, GLsizei h, GLenum fmt, GLenum type, void* data);

/* textures */
typedef void (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint* textures);
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint* textures);
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internal, GLsizei w, GLsizei h, GLint border, GLenum format, GLenum type, const void* data);
typedef void (APIENTRYP PFNGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internal, GLsizei w, GLint border, GLenum format, GLenum type, const void* data);
typedef void (APIENTRYP PFNGLTEXIMAGE3DPROC)(GLenum target, GLint level, GLint internal, GLsizei w, GLsizei h, GLsizei d, GLint border, GLenum format, GLenum type, const void* data);
typedef void (APIENTRYP PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoff, GLint yoff, GLsizei w, GLsizei h, GLenum format, GLenum type, const void* data);
typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internal, GLint x, GLint y, GLsizei w, GLsizei h, GLint border);

/* buffers */
typedef void (APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef void* (APIENTRYP PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
typedef GLboolean (APIENTRYP PFNGLUNMAPBUFFERPROC)(GLenum target);
typedef void (APIENTRYP PFNGLBINDBUFFERBASEPROC)(GLenum target, GLuint index, GLuint buffer);
typedef void (APIENTRYP PFNGLBINDBUFFERRANGEPROC)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);

/* vertex arrays */
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (APIENTRYP PFNGLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);

/* draw */
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void* indices);
typedef void (APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
typedef void (APIENTRYP PFNGLDRAWBUFFERPROC)(GLenum buf);
typedef void (APIENTRYP PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum* bufs);

/* shaders */
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC)(GLuint program);
typedef GLboolean (APIENTRYP PFNGLISPROGRAMPROC)(GLuint program);
typedef GLboolean (APIENTRYP PFNGLISSHADERPROC)(GLuint shader);
typedef void (APIENTRYP PFNGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const GLchar* name);
typedef GLint (APIENTRYP PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar* name);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef GLuint (APIENTRYP PFNGLGETUNIFORMBLOCKINDEXPROC)(GLuint program, const GLchar* uniformBlockName);
typedef void (APIENTRYP PFNGLUNIFORMBLOCKBINDINGPROC)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

/* uniforms */
typedef void (APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (APIENTRYP PFNGLUNIFORM2IPROC)(GLint location, GLint v0, GLint v1);
typedef void (APIENTRYP PFNGLUNIFORM3IPROC)(GLint location, GLint v0, GLint v1, GLint v2);
typedef void (APIENTRYP PFNGLUNIFORM4IPROC)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRYP PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRYP PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRYP PFNGLUNIFORM1IVPROC)(GLint location, GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM2IVPROC)(GLint location, GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM3IVPROC)(GLint location, GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM4IVPROC)(GLint location, GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

/* framebuffers */
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint* framebuffers);
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint* framebuffers);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTUREPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void (APIENTRYP PFNGLBLITFRAMEBUFFERPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint* renderbuffers);
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint* renderbuffers);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

/* ---- Extern function pointer declarations ---- */
extern PFNGLVIEWPORTPROC                glad_glViewport;
extern PFNGLCLEARPROC                   glad_glClear;
extern PFNGLCLEARCOLORPROC              glad_glClearColor;
extern PFNGLCLEARDEPTHPROC              glad_glClearDepth;
extern PFNGLCLEARDEPTHFPROC             glad_glClearDepthf;
extern PFNGLENABLEPROC                  glad_glEnable;
extern PFNGLDISABLEPROC                 glad_glDisable;
extern PFNGLDEPTHFUNCPROC               glad_glDepthFunc;
extern PFNGLDEPTHMASKPROC               glad_glDepthMask;
extern PFNGLBLENDFUNCPROC               glad_glBlendFunc;
extern PFNGLBLENDFUNCSEPARATEPROC       glad_glBlendFuncSeparate;
extern PFNGLBLENDEQUATIONPROC           glad_glBlendEquation;
extern PFNGLBLENDEQUATIONSEPARATEPROC   glad_glBlendEquationSeparate;
extern PFNGLCULLFACEPROC                glad_glCullFace;
extern PFNGLFRONTFACEPROC               glad_glFrontFace;
extern PFNGLPOINTSIZEPROC               glad_glPointSize;
extern PFNGLLINEWIDTHPROC               glad_glLineWidth;
extern PFNGLPOLYGONMODEPROC             glad_glPolygonMode;
extern PFNGLPOLYGONOFFSETPROC           glad_glPolygonOffset;
extern PFNGLSCISSORPROC                 glad_glScissor;
extern PFNGLCOLORMASKPROC               glad_glColorMask;
extern PFNGLFINISHPROC                  glad_glFinish;
extern PFNGLFLUSHPROC                   glad_glFlush;
extern PFNGLGETERRORPROC                glad_glGetError;
extern PFNGLGETBOOLEANVPROC             glad_glGetBooleanv;
extern PFNGLGETINTEGERVPROC             glad_glGetIntegerv;
extern PFNGLGETFLOATVPROC               glad_glGetFloatv;
extern PFNGLGETDOUBLEVPROC              glad_glGetDoublev;
extern PFNGLGETSTRINGPROC               glad_glGetString;
extern PFNGLGETSTRINGIPROC              glad_glGetStringi;
extern PFNGLPIXELSTOREIPROC             glad_glPixelStorei;
extern PFNGLPIXELSTOREFPROC             glad_glPixelStoref;
extern PFNGLREADPIXELSPROC              glad_glReadPixels;
extern PFNGLGENTEXTURESPROC             glad_glGenTextures;
extern PFNGLBINDTEXTUREPROC             glad_glBindTexture;
extern PFNGLDELETETEXTURESPROC          glad_glDeleteTextures;
extern PFNGLTEXIMAGE2DPROC              glad_glTexImage2D;
extern PFNGLTEXIMAGE1DPROC              glad_glTexImage1D;
extern PFNGLTEXIMAGE3DPROC              glad_glTexImage3D;
extern PFNGLTEXSUBIMAGE2DPROC           glad_glTexSubImage2D;
extern PFNGLTEXPARAMETERIPROC           glad_glTexParameteri;
extern PFNGLTEXPARAMETERFPROC           glad_glTexParameterf;
extern PFNGLACTIVETEXTUREPROC           glad_glActiveTexture;
extern PFNGLGENERATEMIPMAPPROC          glad_glGenerateMipmap;
extern PFNGLCOPYTEXIMAGE2DPROC          glad_glCopyTexImage2D;
extern PFNGLGENBUFFERSPROC              glad_glGenBuffers;
extern PFNGLBINDBUFFERPROC              glad_glBindBuffer;
extern PFNGLDELETEBUFFERSPROC           glad_glDeleteBuffers;
extern PFNGLBUFFERDATAPROC              glad_glBufferData;
extern PFNGLBUFFERSUBDATAPROC           glad_glBufferSubData;
extern PFNGLMAPBUFFERPROC               glad_glMapBuffer;
extern PFNGLUNMAPBUFFERPROC             glad_glUnmapBuffer;
extern PFNGLBINDBUFFERBASEPROC          glad_glBindBufferBase;
extern PFNGLBINDBUFFERRANGEPROC         glad_glBindBufferRange;
extern PFNGLGENVERTEXARRAYSPROC         glad_glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC         glad_glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC      glad_glDeleteVertexArrays;
extern PFNGLVERTEXATTRIBPOINTERPROC     glad_glVertexAttribPointer;
extern PFNGLVERTEXATTRIBIPOINTERPROC    glad_glVertexAttribIPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBDIVISORPROC     glad_glVertexAttribDivisor;
extern PFNGLDRAWARRAYSPROC              glad_glDrawArrays;
extern PFNGLDRAWELEMENTSPROC            glad_glDrawElements;
extern PFNGLDRAWARRAYSINSTANCEDPROC     glad_glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC   glad_glDrawElementsInstanced;
extern PFNGLDRAWBUFFERPROC              glad_glDrawBuffer;
extern PFNGLDRAWBUFFERSPROC             glad_glDrawBuffers;
extern PFNGLCREATESHADERPROC            glad_glCreateShader;
extern PFNGLDELETESHADERPROC            glad_glDeleteShader;
extern PFNGLSHADERSOURCEPROC            glad_glShaderSource;
extern PFNGLCOMPILESHADERPROC           glad_glCompileShader;
extern PFNGLGETSHADERIVPROC             glad_glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC        glad_glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC           glad_glCreateProgram;
extern PFNGLDELETEPROGRAMPROC           glad_glDeleteProgram;
extern PFNGLATTACHSHADERPROC            glad_glAttachShader;
extern PFNGLDETACHSHADERPROC            glad_glDetachShader;
extern PFNGLLINKPROGRAMPROC             glad_glLinkProgram;
extern PFNGLUSEPROGRAMPROC              glad_glUseProgram;
extern PFNGLGETPROGRAMIVPROC            glad_glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC       glad_glGetProgramInfoLog;
extern PFNGLVALIDATEPROGRAMPROC         glad_glValidateProgram;
extern PFNGLISPROGRAMPROC               glad_glIsProgram;
extern PFNGLISSHADERPROC                glad_glIsShader;
extern PFNGLBINDATTRIBLOCATIONPROC      glad_glBindAttribLocation;
extern PFNGLGETATTRIBLOCATIONPROC       glad_glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC      glad_glGetUniformLocation;
extern PFNGLGETUNIFORMBLOCKINDEXPROC    glad_glGetUniformBlockIndex;
extern PFNGLUNIFORMBLOCKBINDINGPROC     glad_glUniformBlockBinding;
extern PFNGLUNIFORM1IPROC               glad_glUniform1i;
extern PFNGLUNIFORM2IPROC               glad_glUniform2i;
extern PFNGLUNIFORM3IPROC               glad_glUniform3i;
extern PFNGLUNIFORM4IPROC               glad_glUniform4i;
extern PFNGLUNIFORM1FPROC               glad_glUniform1f;
extern PFNGLUNIFORM2FPROC               glad_glUniform2f;
extern PFNGLUNIFORM3FPROC               glad_glUniform3f;
extern PFNGLUNIFORM4FPROC               glad_glUniform4f;
extern PFNGLUNIFORM1IVPROC              glad_glUniform1iv;
extern PFNGLUNIFORM2IVPROC              glad_glUniform2iv;
extern PFNGLUNIFORM3IVPROC              glad_glUniform3iv;
extern PFNGLUNIFORM4IVPROC              glad_glUniform4iv;
extern PFNGLUNIFORM1FVPROC              glad_glUniform1fv;
extern PFNGLUNIFORM2FVPROC              glad_glUniform2fv;
extern PFNGLUNIFORM3FVPROC              glad_glUniform3fv;
extern PFNGLUNIFORM4FVPROC              glad_glUniform4fv;
extern PFNGLUNIFORMMATRIX2FVPROC        glad_glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC        glad_glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC        glad_glUniformMatrix4fv;
extern PFNGLUNIFORMMATRIX2X3FVPROC      glad_glUniformMatrix2x3fv;
extern PFNGLUNIFORMMATRIX3X2FVPROC      glad_glUniformMatrix3x2fv;
extern PFNGLUNIFORMMATRIX2X4FVPROC      glad_glUniformMatrix2x4fv;
extern PFNGLUNIFORMMATRIX4X2FVPROC      glad_glUniformMatrix4x2fv;
extern PFNGLUNIFORMMATRIX3X4FVPROC      glad_glUniformMatrix3x4fv;
extern PFNGLUNIFORMMATRIX4X3FVPROC      glad_glUniformMatrix4x3fv;
extern PFNGLGENFRAMEBUFFERSPROC         glad_glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC         glad_glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC      glad_glDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC    glad_glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTUREPROC      glad_glFramebufferTexture;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC  glad_glCheckFramebufferStatus;
extern PFNGLBLITFRAMEBUFFERPROC         glad_glBlitFramebuffer;
extern PFNGLGENRENDERBUFFERSPROC        glad_glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC        glad_glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC     glad_glDeleteRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC     glad_glRenderbufferStorage;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample;

/* ---- Macro aliases ---- */
#define glViewport                  glad_glViewport
#define glClear                     glad_glClear
#define glClearColor                glad_glClearColor
#define glClearDepth                glad_glClearDepth
#define glClearDepthf               glad_glClearDepthf
#define glEnable                    glad_glEnable
#define glDisable                   glad_glDisable
#define glDepthFunc                 glad_glDepthFunc
#define glDepthMask                 glad_glDepthMask
#define glBlendFunc                 glad_glBlendFunc
#define glBlendFuncSeparate         glad_glBlendFuncSeparate
#define glBlendEquation             glad_glBlendEquation
#define glBlendEquationSeparate     glad_glBlendEquationSeparate
#define glCullFace                  glad_glCullFace
#define glFrontFace                 glad_glFrontFace
#define glPointSize                 glad_glPointSize
#define glLineWidth                 glad_glLineWidth
#define glPolygonMode               glad_glPolygonMode
#define glPolygonOffset             glad_glPolygonOffset
#define glScissor                   glad_glScissor
#define glColorMask                 glad_glColorMask
#define glFinish                    glad_glFinish
#define glFlush                     glad_glFlush
#define glGetError                  glad_glGetError
#define glGetBooleanv               glad_glGetBooleanv
#define glGetIntegerv               glad_glGetIntegerv
#define glGetFloatv                 glad_glGetFloatv
#define glGetDoublev                glad_glGetDoublev
#define glGetString                 glad_glGetString
#define glGetStringi                glad_glGetStringi
#define glPixelStorei               glad_glPixelStorei
#define glPixelStoref               glad_glPixelStoref
#define glReadPixels                glad_glReadPixels
#define glGenTextures               glad_glGenTextures
#define glBindTexture               glad_glBindTexture
#define glDeleteTextures            glad_glDeleteTextures
#define glTexImage2D                glad_glTexImage2D
#define glTexImage1D                glad_glTexImage1D
#define glTexImage3D                glad_glTexImage3D
#define glTexSubImage2D             glad_glTexSubImage2D
#define glTexParameteri             glad_glTexParameteri
#define glTexParameterf             glad_glTexParameterf
#define glActiveTexture             glad_glActiveTexture
#define glGenerateMipmap            glad_glGenerateMipmap
#define glCopyTexImage2D            glad_glCopyTexImage2D
#define glGenBuffers                glad_glGenBuffers
#define glBindBuffer                glad_glBindBuffer
#define glDeleteBuffers             glad_glDeleteBuffers
#define glBufferData                glad_glBufferData
#define glBufferSubData             glad_glBufferSubData
#define glMapBuffer                 glad_glMapBuffer
#define glUnmapBuffer               glad_glUnmapBuffer
#define glBindBufferBase            glad_glBindBufferBase
#define glBindBufferRange           glad_glBindBufferRange
#define glGenVertexArrays           glad_glGenVertexArrays
#define glBindVertexArray           glad_glBindVertexArray
#define glDeleteVertexArrays        glad_glDeleteVertexArrays
#define glVertexAttribPointer       glad_glVertexAttribPointer
#define glVertexAttribIPointer      glad_glVertexAttribIPointer
#define glEnableVertexAttribArray   glad_glEnableVertexAttribArray
#define glDisableVertexAttribArray  glad_glDisableVertexAttribArray
#define glVertexAttribDivisor       glad_glVertexAttribDivisor
#define glDrawArrays                glad_glDrawArrays
#define glDrawElements              glad_glDrawElements
#define glDrawArraysInstanced       glad_glDrawArraysInstanced
#define glDrawElementsInstanced     glad_glDrawElementsInstanced
#define glDrawBuffer                glad_glDrawBuffer
#define glDrawBuffers               glad_glDrawBuffers
#define glCreateShader              glad_glCreateShader
#define glDeleteShader              glad_glDeleteShader
#define glShaderSource              glad_glShaderSource
#define glCompileShader             glad_glCompileShader
#define glGetShaderiv               glad_glGetShaderiv
#define glGetShaderInfoLog          glad_glGetShaderInfoLog
#define glCreateProgram             glad_glCreateProgram
#define glDeleteProgram             glad_glDeleteProgram
#define glAttachShader              glad_glAttachShader
#define glDetachShader              glad_glDetachShader
#define glLinkProgram               glad_glLinkProgram
#define glUseProgram                glad_glUseProgram
#define glGetProgramiv              glad_glGetProgramiv
#define glGetProgramInfoLog         glad_glGetProgramInfoLog
#define glValidateProgram           glad_glValidateProgram
#define glIsProgram                 glad_glIsProgram
#define glIsShader                  glad_glIsShader
#define glBindAttribLocation        glad_glBindAttribLocation
#define glGetAttribLocation         glad_glGetAttribLocation
#define glGetUniformLocation        glad_glGetUniformLocation
#define glGetUniformBlockIndex      glad_glGetUniformBlockIndex
#define glUniformBlockBinding       glad_glUniformBlockBinding
#define glUniform1i                 glad_glUniform1i
#define glUniform2i                 glad_glUniform2i
#define glUniform3i                 glad_glUniform3i
#define glUniform4i                 glad_glUniform4i
#define glUniform1f                 glad_glUniform1f
#define glUniform2f                 glad_glUniform2f
#define glUniform3f                 glad_glUniform3f
#define glUniform4f                 glad_glUniform4f
#define glUniform1iv                glad_glUniform1iv
#define glUniform2iv                glad_glUniform2iv
#define glUniform3iv                glad_glUniform3iv
#define glUniform4iv                glad_glUniform4iv
#define glUniform1fv                glad_glUniform1fv
#define glUniform2fv                glad_glUniform2fv
#define glUniform3fv                glad_glUniform3fv
#define glUniform4fv                glad_glUniform4fv
#define glUniformMatrix2fv          glad_glUniformMatrix2fv
#define glUniformMatrix3fv          glad_glUniformMatrix3fv
#define glUniformMatrix4fv          glad_glUniformMatrix4fv
#define glUniformMatrix2x3fv        glad_glUniformMatrix2x3fv
#define glUniformMatrix3x2fv        glad_glUniformMatrix3x2fv
#define glUniformMatrix2x4fv        glad_glUniformMatrix2x4fv
#define glUniformMatrix4x2fv        glad_glUniformMatrix4x2fv
#define glUniformMatrix3x4fv        glad_glUniformMatrix3x4fv
#define glUniformMatrix4x3fv        glad_glUniformMatrix4x3fv
#define glGenFramebuffers           glad_glGenFramebuffers
#define glBindFramebuffer           glad_glBindFramebuffer
#define glDeleteFramebuffers        glad_glDeleteFramebuffers
#define glFramebufferTexture2D      glad_glFramebufferTexture2D
#define glFramebufferTexture        glad_glFramebufferTexture
#define glFramebufferRenderbuffer   glad_glFramebufferRenderbuffer
#define glCheckFramebufferStatus    glad_glCheckFramebufferStatus
#define glBlitFramebuffer           glad_glBlitFramebuffer
#define glGenRenderbuffers          glad_glGenRenderbuffers
#define glBindRenderbuffer          glad_glBindRenderbuffer
#define glDeleteRenderbuffers       glad_glDeleteRenderbuffers
#define glRenderbufferStorage       glad_glRenderbufferStorage
#define glRenderbufferStorageMultisample glad_glRenderbufferStorageMultisample

/* ---- Loader ---- */
int gladLoadGL(void);
typedef void* (*GLADloadproc)(const char* name);
int gladLoadGLLoader(GLADloadproc load);

#endif /* __glad_h_ */
