// main.cpp
#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

extern GLuint loadAVIF(const char* filepath);
extern GLuint createQuad();

const char* vertexShaderSource = R"(
    #version 100
    attribute vec4 position;
    void main() {
        gl_Position = position;
    }
)";

const char* fragmentShaderSource = R"(
    #version 100
    precision mediump float;
    void main() {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0); // red
    }
)";

std::string loadShaderSource(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderSource = buffer.str();
    
    shaderSource = "#version 130\n" + shaderSource;
    
    return shaderSource;
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetShaderInfoLog(shader, logLength, nullptr, log);
        printf("Shader compilation failed: %s\n", log);
        delete[] log;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetProgramInfoLog(program, logLength, nullptr, log);
        printf("Program linking failed: %s\n", log);
        delete[] log;
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("My Singing Monsters", 100, 100, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (win == NULL) {
        SDL_Quit();
        return 1;
    }

    // create gl context
    SDL_GLContext glContext = SDL_GL_CreateContext(win);
    if (glContext == NULL) {
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // init gles 2
    const char* gl_version = (const char*)glGetString(GL_VERSION);
    if (!gl_version) {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    printf("OpenGL ES version: %s\n", gl_version);
    
    // create program
    GLuint program = createProgram(vertexShaderSource, fragmentShaderSource);
    if (!program) {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    glUseProgram(program);

    bool running = true;
    SDL_Event event;

    GLuint quadVAO = createQuad();
    GLuint texture = loadAVIF("data/gfx/menu/BBB_logo_loading_screen.avif");

    GLuint textureUniformLocation = glGetUniformLocation(program, "textureSampler");

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureUniformLocation, 0);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
        SDL_GL_SwapWindow(win);

        SDL_Delay(16); // ~60 FPS
    }

    glDeleteProgram(program);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
