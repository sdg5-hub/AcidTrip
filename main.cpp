#if __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#if defined(_WIN32)
#define APIENTRYP APIENTRY *
#endif

// Minimal OpenGL 3.3 function pointers loaded through SDL.
typedef GLuint(APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
typedef void(APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
typedef void(APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void(APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void(APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void(APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
typedef GLuint(APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
typedef void(APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void(APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void(APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void(APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void(APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void(APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef GLint(APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void(APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void(APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void(APIENTRYP PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void(APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void(APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void(APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
typedef void(APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void(APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void(APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void(APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void(APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void(APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

static PFNGLCREATESHADERPROC glCreateShaderPtr = nullptr;
static PFNGLSHADERSOURCEPROC glShaderSourcePtr = nullptr;
static PFNGLCOMPILESHADERPROC glCompileShaderPtr = nullptr;
static PFNGLGETSHADERIVPROC glGetShaderivPtr = nullptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLogPtr = nullptr;
static PFNGLDELETESHADERPROC glDeleteShaderPtr = nullptr;
static PFNGLCREATEPROGRAMPROC glCreateProgramPtr = nullptr;
static PFNGLATTACHSHADERPROC glAttachShaderPtr = nullptr;
static PFNGLLINKPROGRAMPROC glLinkProgramPtr = nullptr;
static PFNGLGETPROGRAMIVPROC glGetProgramivPtr = nullptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLogPtr = nullptr;
static PFNGLUSEPROGRAMPROC glUseProgramPtr = nullptr;
static PFNGLDELETEPROGRAMPROC glDeleteProgramPtr = nullptr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocationPtr = nullptr;
static PFNGLUNIFORM1FPROC glUniform1fPtr = nullptr;
static PFNGLUNIFORM1IPROC glUniform1iPtr = nullptr;
static PFNGLUNIFORM2FPROC glUniform2fPtr = nullptr;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArraysPtr = nullptr;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArrayPtr = nullptr;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArraysPtr = nullptr;
static PFNGLGENBUFFERSPROC glGenBuffersPtr = nullptr;
static PFNGLBINDBUFFERPROC glBindBufferPtr = nullptr;
static PFNGLBUFFERDATAPROC glBufferDataPtr = nullptr;
static PFNGLDELETEBUFFERSPROC glDeleteBuffersPtr = nullptr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArrayPtr = nullptr;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointerPtr = nullptr;

template <typename T>
static bool LoadProc(T &out, const char *name) {
  out = reinterpret_cast<T>(SDL_GL_GetProcAddress(name));
  if (!out) {
    std::cerr << "Failed to load OpenGL symbol: " << name << std::endl;
    return false;
  }
  return true;
}

static bool LoadGLFunctions() {
  return LoadProc(glCreateShaderPtr, "glCreateShader") &&
         LoadProc(glShaderSourcePtr, "glShaderSource") &&
         LoadProc(glCompileShaderPtr, "glCompileShader") &&
         LoadProc(glGetShaderivPtr, "glGetShaderiv") &&
         LoadProc(glGetShaderInfoLogPtr, "glGetShaderInfoLog") &&
         LoadProc(glDeleteShaderPtr, "glDeleteShader") &&
         LoadProc(glCreateProgramPtr, "glCreateProgram") &&
         LoadProc(glAttachShaderPtr, "glAttachShader") &&
         LoadProc(glLinkProgramPtr, "glLinkProgram") &&
         LoadProc(glGetProgramivPtr, "glGetProgramiv") &&
         LoadProc(glGetProgramInfoLogPtr, "glGetProgramInfoLog") &&
         LoadProc(glUseProgramPtr, "glUseProgram") &&
         LoadProc(glDeleteProgramPtr, "glDeleteProgram") &&
         LoadProc(glGetUniformLocationPtr, "glGetUniformLocation") &&
         LoadProc(glUniform1fPtr, "glUniform1f") &&
         LoadProc(glUniform1iPtr, "glUniform1i") &&
         LoadProc(glUniform2fPtr, "glUniform2f") &&
         LoadProc(glGenVertexArraysPtr, "glGenVertexArrays") &&
         LoadProc(glBindVertexArrayPtr, "glBindVertexArray") &&
         LoadProc(glDeleteVertexArraysPtr, "glDeleteVertexArrays") &&
         LoadProc(glGenBuffersPtr, "glGenBuffers") &&
         LoadProc(glBindBufferPtr, "glBindBuffer") &&
         LoadProc(glBufferDataPtr, "glBufferData") &&
         LoadProc(glDeleteBuffersPtr, "glDeleteBuffers") &&
         LoadProc(glEnableVertexAttribArrayPtr, "glEnableVertexAttribArray") &&
         LoadProc(glVertexAttribPointerPtr, "glVertexAttribPointer");
}

static std::string ReadFileText(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Could not open file: " << path << std::endl;
    return "";
  }
  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

static GLuint CompileShader(GLenum type, const std::string &source, const std::string &name) {
  GLuint shader = glCreateShaderPtr(type);
  const char *src = source.c_str();
  glShaderSourcePtr(shader, 1, &src, nullptr);
  glCompileShaderPtr(shader);

  GLint success = 0;
  glGetShaderivPtr(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint length = 0;
    glGetShaderivPtr(shader, GL_INFO_LOG_LENGTH, &length);
    std::string log(length > 0 ? length : 1, '\0');
    glGetShaderInfoLogPtr(shader, static_cast<GLsizei>(log.size()), nullptr, &log[0]);
    std::cerr << "Shader compile error in " << name << ":\n" << log << std::endl;
    glDeleteShaderPtr(shader);
    return 0;
  }
  return shader;
}

static GLuint CreateProgramFromFiles(const std::string &vertPath, const std::string &fragPath) {
  const std::string vertSource = ReadFileText(vertPath);
  const std::string fragSource = ReadFileText(fragPath);
  if (vertSource.empty() || fragSource.empty()) {
    return 0;
  }

  GLuint vs = CompileShader(GL_VERTEX_SHADER, vertSource, vertPath);
  GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragSource, fragPath);
  if (!vs || !fs) {
    if (vs) glDeleteShaderPtr(vs);
    if (fs) glDeleteShaderPtr(fs);
    return 0;
  }

  GLuint program = glCreateProgramPtr();
  glAttachShaderPtr(program, vs);
  glAttachShaderPtr(program, fs);
  glLinkProgramPtr(program);

  GLint linked = 0;
  glGetProgramivPtr(program, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLint length = 0;
    glGetProgramivPtr(program, GL_INFO_LOG_LENGTH, &length);
    std::string log(length > 0 ? length : 1, '\0');
    glGetProgramInfoLogPtr(program, static_cast<GLsizei>(log.size()), nullptr, &log[0]);
    std::cerr << "Program link error:\n" << log << std::endl;
    glDeleteShaderPtr(vs);
    glDeleteShaderPtr(fs);
    glDeleteProgramPtr(program);
    return 0;
  }

  glDeleteShaderPtr(vs);
  glDeleteShaderPtr(fs);
  return program;
}

struct Params {
  float warp = 1.0f;
  float speed = 1.0f;
  int segments = 8;
  float paletteSeed = 3.17f;
  bool freeze = false;
};

int main(int, char **) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#if defined(__APPLE__)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_Window *window = SDL_CreateWindow(
      "AcidTrip",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      1280,
      720,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (!context) {
    std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_GL_MakeCurrent(window, context);
  SDL_GL_SetSwapInterval(1);

  if (!LoadGLFunctions()) {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  GLuint program = CreateProgramFromFiles("shader.vert", "shader.frag");
  if (!program) {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  const float quadVertices[] = {
      -1.0f, -1.0f,
      1.0f, -1.0f,
      -1.0f, 1.0f,
      -1.0f, 1.0f,
      1.0f, -1.0f,
      1.0f, 1.0f,
  };

  GLuint vao = 0;
  GLuint vbo = 0;
  glGenVertexArraysPtr(1, &vao);
  glBindVertexArrayPtr(vao);

  glGenBuffersPtr(1, &vbo);
  glBindBufferPtr(GL_ARRAY_BUFFER, vbo);
  glBufferDataPtr(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

  glEnableVertexAttribArrayPtr(0);
  glVertexAttribPointerPtr(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

  const GLint uTime = glGetUniformLocationPtr(program, "u_time");
  const GLint uResolution = glGetUniformLocationPtr(program, "u_resolution");
  const GLint uMouse = glGetUniformLocationPtr(program, "u_mouse");
  const GLint uWarp = glGetUniformLocationPtr(program, "u_warp");
  const GLint uSpeed = glGetUniformLocationPtr(program, "u_speed");
  const GLint uSegments = glGetUniformLocationPtr(program, "u_segments");
  const GLint uPaletteSeed = glGetUniformLocationPtr(program, "u_paletteSeed");

  Params params;
  std::mt19937 rng(static_cast<unsigned int>(SDL_GetPerformanceCounter()));
  std::uniform_real_distribution<float> paletteDist(0.0f, 100.0f);

  bool running = true;
  float frozenTime = 0.0f;
  float mouseX = 0.0f;
  float mouseY = 0.0f;
  const Uint64 startCounter = SDL_GetPerformanceCounter();
  const double counterFreq = static_cast<double>(SDL_GetPerformanceFrequency());

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        glViewport(0, 0, event.window.data1, event.window.data2);
      } else if (event.type == SDL_MOUSEMOTION) {
        mouseX = static_cast<float>(event.motion.x);
        mouseY = static_cast<float>(event.motion.y);
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            running = false;
            break;
          case SDLK_w:
            params.warp = std::min(4.0f, params.warp + 0.08f);
            break;
          case SDLK_s:
            params.warp = std::max(0.1f, params.warp - 0.08f);
            break;
          case SDLK_d:
            params.speed = std::min(5.0f, params.speed + 0.08f);
            break;
          case SDLK_a:
            params.speed = std::max(0.1f, params.speed - 0.08f);
            break;
          case SDLK_q:
            params.segments = std::max(3, params.segments - 1);
            break;
          case SDLK_e:
            params.segments = std::min(16, params.segments + 1);
            break;
          case SDLK_SPACE:
            params.freeze = !params.freeze;
            break;
          case SDLK_r:
            params.paletteSeed = paletteDist(rng);
            break;
          default:
            break;
        }
      }
    }

    int width = 0;
    int height = 0;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    const Uint64 nowCounter = SDL_GetPerformanceCounter();
    float timeSec = static_cast<float>((nowCounter - startCounter) / counterFreq);

    if (params.freeze) {
      if (frozenTime == 0.0f) {
        frozenTime = timeSec;
      }
      timeSec = frozenTime;
    } else {
      frozenTime = 0.0f;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgramPtr(program);
    glUniform1fPtr(uTime, timeSec);
    glUniform2fPtr(uResolution, static_cast<float>(width), static_cast<float>(height));
    glUniform2fPtr(uMouse, mouseX, static_cast<float>(height) - mouseY);
    glUniform1fPtr(uWarp, params.warp);
    glUniform1fPtr(uSpeed, params.speed);
    glUniform1iPtr(uSegments, params.segments);
    glUniform1fPtr(uPaletteSeed, params.paletteSeed);

    glBindVertexArrayPtr(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    SDL_GL_SwapWindow(window);
  }

  glDeleteBuffersPtr(1, &vbo);
  glDeleteVertexArraysPtr(1, &vao);
  glDeleteProgramPtr(program);

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
