#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace sf;

struct Vertex {
    float coordinate[3];
    float color[3];
};

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    RenderWindow window;
    bool isRunning;
    Clock clock;
    Time elapsed;

    GLuint index, vsid, fsid, progID, vbo = 1, positionID, colorID;
    Vertex vertex[8];  // Cube has 8 vertices
    GLubyte triangles[36];  // 36 indices to form 12 triangles (6 faces)

    void initialize();
    void update();
    void render();
    void unload();

    std::string readShaderFile(const char* filename);
    GLuint compileShader(GLenum type, const std::string& source);
    GLuint createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
};

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube with Shaders") {}

Game::~Game() {}

void Game::run() {
    initialize();
    Event event;

    while (isRunning) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                isRunning = false;
        }

        update();
        render();
    }

    unload();
}

std::string Game::readShaderFile(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Game::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, log);
        std::cerr << "Shader compilation failed: " << log << std::endl;
        delete[] log;
    }

    return shader;
}

GLuint Game::createShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) {
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
        glGetProgramInfoLog(program, logLength, NULL, log);
        std::cerr << "Program linking failed: " << log << std::endl;
        delete[] log;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void Game::initialize() {
    isRunning = true;
    glewInit();

    // Read shader files
    std::string vertexShaderSource = readShaderFile("vs.glsl");
    std::string fragmentShaderSource = readShaderFile("fs.glsl");

    // Create shader program from loaded source
    progID = createShaderProgram(vertexShaderSource, fragmentShaderSource);s
    glUseProgram(progID);

    // Get attribute locations
    positionID = glGetAttribLocation(progID, "sv_position");
    colorID = glGetAttribLocation(progID, "sv_color");

    // Cube vertices and colors
    vertex[0] = { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} }; // Red
    vertex[1] = { { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} }; // Green
    vertex[2] = { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f} }; // Blue
    vertex[3] = { {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f} }; // Yellow
    vertex[4] = { {-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f} }; // Cyan
    vertex[5] = { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f} }; // Magenta
    vertex[6] = { { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f} }; // White
    vertex[7] = { {-0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f} }; // Gray

    // Cube indices (6 faces)
    GLuint cubeIndices[36] = {
        0, 1, 2, 0, 2, 3,   // Front face
        4, 5, 6, 4, 6, 7,   // Back face
        0, 3, 7, 0, 7, 4,   // Left face
        1, 2, 6, 1, 6, 5,   // Right face
        3, 2, 6, 3, 6, 7,   // Top face
        0, 1, 5, 0, 5, 4    // Bottom face
    };

    // Create VBO and upload vertex data
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);

    // Create index buffer for cube faces
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableVertexAttribArray(positionID);
    glEnableVertexAttribArray(colorID);
}

void Game::update() {
    elapsed = clock.getElapsedTime();

    float angle = elapsed.asSeconds() * 45.0f;  // Rotate 45 degrees per second
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply the rotation matrix to each vertex
    for (int i = 0; i < 8; ++i) {
        glm::vec4 vertexPos = glm::vec4(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2], 1.0f);
        vertexPos = rotationMatrix * vertexPos;

        vertex[i].coordinate[0] = vertexPos.x;
        vertex[i].coordinate[1] = vertexPos.y;
        vertex[i].coordinate[2] = vertexPos.z;
    }

    // Update vertex data in GPU buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (float*)NULL + 3);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    window.display();
}

void Game::unload() {
    cout << "Cleaning up" << endl;
    glDeleteProgram(progID);
    glDeleteBuffers(1, &vbo);
}

int main() {
    Game game;
    game.run();
    return 0;
}
