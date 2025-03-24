#include "./include/Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Vertex and Fragment Shaders")
{
}

Game::~Game() {}

std::string readShaderFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Could not open shader file " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Game::run()
{
    initialize();

    Event event;
    while (isRunning)
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                isRunning = false;
            }
        }
        update();
        render();
    }
}

typedef struct
{
    float coordinate[3];
    float color[3];
} Vertex;

Vertex vertex[8];
GLubyte triangles[36];

/* OpenGL Variables */
GLuint index, vsid, fsid, progID, vbo, vao, ebo, positionID, colorID, modelID;

void Game::initialize()
{
    isRunning = true;

    glewInit();
    std::string vertexShaderSource = readShaderFile("./src/vertexShader.glsl");
    std::string fragmentShaderSource = readShaderFile("./src/fragmentShader.glsl");

    const char* vs_src = vertexShaderSource.c_str();
    const char* fs_src = fragmentShaderSource.c_str();

    // Compile Vertex Shader
    vsid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsid, 1, &vs_src, NULL);
    glCompileShader(vsid);
    checkShaderCompilation(vsid, "Vertex Shader");

    // Compile Fragment Shader
    fsid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsid, 1, &fs_src, NULL);
    glCompileShader(fsid);
    checkShaderCompilation(fsid, "Fragment Shader");

    // Create and Link Shader Program
    progID = glCreateProgram();
    glAttachShader(progID, vsid);
    glAttachShader(progID, fsid);
    glLinkProgram(progID);
    checkProgramLinking(progID);

    glUseProgram(progID);

    positionID = glGetAttribLocation(progID, "sv_position");
    colorID = glGetAttribLocation(progID, "sv_color");
    modelID = glGetUniformLocation(progID, "model");

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Setup Vertices
    Vertex vertices[] = {
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // Red
            {{0.5f, -0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}}, // Green
            {{0.5f,  0.5f, -0.5f},  {0.0f, 0.0f, 1.0f}}, // Blue
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}, // Yellow
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f}}, // Magenta
            {{0.5f, -0.5f,  0.5f},  {0.0f, 1.0f, 1.0f}}, // Cyan
            {{0.5f,  0.5f,  0.5f},  {1.0f, 1.0f, 1.0f}}, // White
            {{-0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f}}  // Gray
    };

    GLubyte indices[] = {
            0, 1, 2,  2, 3, 0,
            1, 5, 6,  6, 2, 1,
            5, 4, 7,  7, 6, 5,
            4, 0, 3,  3, 7, 4,
            3, 2, 6,  6, 7, 3,
            4, 5, 1,  1, 0, 4
    };

    // Generate and Bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and Bind VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and Bind EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define Vertex Attributes
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(positionID);

    glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 3));
    glEnableVertexAttribArray(colorID);
}

void Game::update()
{
    elapsed = clock.getElapsedTime();
    float angle = elapsed.asSeconds();

    float cosA = cos(angle);
    float sinA = sin(angle);

    float rotationMatrix[16] = {
            cosA,  0, sinA,  0,
            0,     1, 0,     0,
            -sinA, 0, cosA,  0,
            0,     0, 0,     1
    };

    glUseProgram(progID);
    glUniformMatrix4fv(modelID, 1, GL_FALSE, rotationMatrix);
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(progID);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
    window.display();
}

void Game::unload()
{
    glDeleteProgram(progID);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}


void Game::checkShaderCompilation(GLuint shader, const std::string& shaderName) {
    GLint isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        GLint maxLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());
        std::cerr << "ERROR: " << shaderName << " Compilation Failed\n" << errorLog.data() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Game::checkProgramLinking(GLuint program) {
    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());
        std::cerr << "ERROR: Shader Program Linking Failed\n" << errorLog.data() << std::endl;
        exit(EXIT_FAILURE);
    }
}

