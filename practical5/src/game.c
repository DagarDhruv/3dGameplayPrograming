#include "../include/game.h"

// Global flag to control update state
bool updatable = false;

// Global constants
const int SCREEN_WIDTH = 800;       // Screen Width
const int SCREEN_HEIGHT = 600;      // Screen Height
const float ROTATION_SPEED = 45.0f; // Degrees per second
Matrix3 modelMatrix;

typedef struct
{
    float coordinate[3];
    float color[3];
} Vertex;
/* Global variables */
GLuint vbo[1];
GLuint index;

// 5 vertices (Apex and Base of Pyramid)
Vertex vertex[8];
GLubyte triangles[36];

void initialize(Game *game)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Create window
    game->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL VBO using GLFW", NULL, NULL);
    if (!game->window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make OpenGL context current
    glfwMakeContextCurrent(game->window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwDestroyWindow(game->window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Initialize game state
    game->isRunning = true;
    game->lastTime = glfwGetTime();
    game->rotationX = 0.0f;
    game->rotationY = 0.0f;
    game->rotationZ = 0.0f;

    // Pyramid vertices
    // Base vertices
    vertex[0].coordinate[0] = -1.0f; // bottom-left-front
    vertex[0].coordinate[1] = -1.0f;
    vertex[0].coordinate[2] = -3.0f;
    vertex[0].color[0] = 1.0f;
    vertex[0].color[1] = 0.0f;
    vertex[0].color[2] = 0.0f;

    vertex[1].coordinate[0] = 1.0f;  // bottom-right-front
    vertex[1].coordinate[1] = -1.0f;
    vertex[1].coordinate[2] = -3.0f;
    vertex[1].color[0] = 0.0f;
    vertex[1].color[1] = 1.0f;
    vertex[1].color[2] = 0.0f;

    vertex[2].coordinate[0] = 1.0f;  // top-right-front
    vertex[2].coordinate[1] = 1.0f;
    vertex[2].coordinate[2] = -3.0f;
    vertex[2].color[0] = 0.0f;
    vertex[2].color[1] = 0.0f;
    vertex[2].color[2] = 1.0f;

    vertex[3].coordinate[0] = -1.0f; // top-left-front
    vertex[3].coordinate[1] = 1.0f;
    vertex[3].coordinate[2] = -3.0f;
    vertex[3].color[0] = 1.0f;
    vertex[3].color[1] = 1.0f;
    vertex[3].color[2] = 0.0f;

// Back face vertices
    vertex[4].coordinate[0] = -1.0f; // bottom-left-back
    vertex[4].coordinate[1] = -1.0f;
    vertex[4].coordinate[2] = -5.0f;
    vertex[4].color[0] = 1.0f;
    vertex[4].color[1] = 0.0f;
    vertex[4].color[2] = 1.0f;

    vertex[5].coordinate[0] = 1.0f;  // bottom-right-back
    vertex[5].coordinate[1] = -1.0f;
    vertex[5].coordinate[2] = -5.0f;
    vertex[5].color[0] = 0.0f;
    vertex[5].color[1] = 1.0f;
    vertex[5].color[2] = 1.0f;

    vertex[6].coordinate[0] = 1.0f;  // top-right-back
    vertex[6].coordinate[1] = 1.0f;
    vertex[6].coordinate[2] = -5.0f;
    vertex[6].color[0] = 0.5f;
    vertex[6].color[1] = 0.5f;
    vertex[6].color[2] = 0.5f;

    vertex[7].coordinate[0] = -1.0f; // top-left-back
    vertex[7].coordinate[1] = 1.0f;
    vertex[7].coordinate[2] = -5.0f;
    vertex[7].color[0] = 1.0f;
    vertex[7].color[1] = 1.0f;
    vertex[7].color[2] = 1.0f;
    // Indices for pyramid triangles
    // Front face (0)
    triangles[0] = 0; triangles[1] = 1; triangles[2] = 2;
    triangles[3] = 0; triangles[4] = 2; triangles[5] = 3;
// Back face (1)
    triangles[6] = 4; triangles[7] = 7; triangles[8] = 6;
    triangles[9] = 4; triangles[10] = 6; triangles[11] = 5;
// Left face (2)
    triangles[12] = 0; triangles[13] = 3; triangles[14] = 7;
    triangles[15] = 0; triangles[16] = 7; triangles[17] = 4;
// Right face (3)
    triangles[18] = 1; triangles[19] = 5; triangles[20] = 6;
    triangles[21] = 1; triangles[22] = 6; triangles[23] = 2;
// Top face (4)
    triangles[24] = 3; triangles[25] = 2; triangles[26] = 6;
    triangles[27] = 3; triangles[28] = 6; triangles[29] = 7;
// Bottom face (5)
    triangles[30] = 0; triangles[31] = 4; triangles[32] = 5;
    triangles[33] = 0; triangles[34] = 5; triangles[35] = 1;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Update buffer sizes
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void handleInput(GLFWwindow *window, Game *game) {
    // Check if window should close
    if (glfwWindowShouldClose(window)) {
        game->isRunning = false;
        return;
    }

    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - game->lastTime;

    // Y-axis rotation (Left/Right arrows)
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        game->rotationY += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        game->rotationY -= ROTATION_SPEED * deltaTime;
    }

    // X-axis rotation (Up/Down arrows)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        game->rotationX += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        game->rotationX -= ROTATION_SPEED * deltaTime;
    }

    // Z-axis rotation (Z/X keys)
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        game->rotationZ += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        game->rotationZ -= ROTATION_SPEED * deltaTime;
    }

    // Normalize rotations
    game->rotationY = fmodf(game->rotationY, 360.0f);
    game->rotationX = fmodf(game->rotationX, 360.0f);
    game->rotationZ = fmodf(game->rotationZ, 360.0f);
}



void update(Game *game) {
    // Get current time
    double currentTime = glfwGetTime();

    // Update model matrix with transformations
    Matrix3 rotationMatrixX = Matrix3_rotation(game->rotationX);
    Matrix3 rotationMatrixY = Matrix3_rotation(game->rotationY);
    Matrix3 rotationMatrixZ = Matrix3_rotation(game->rotationZ);
    Matrix3 translationMatrix = Matrix3_translate(0.0f, 0.0f, -4.0f);
    Matrix3 scaleMatrix = Matrix3_scale(0.5f, 0.5f, 0.5f);

    // Combine transformations: scale -> rotate -> translate
    Matrix3 tempMatrix = Matrix3_multiply(rotationMatrixZ, Matrix3_multiply(rotationMatrixY, Matrix3_multiply(rotationMatrixX, scaleMatrix)));
    modelMatrix = Matrix3_multiply(translationMatrix, tempMatrix);

    // Update last time
    game->lastTime = currentTime;

    // Optional: Periodic logging
    static double lastLogTime = 0.0;
    if (currentTime - lastLogTime >= 1.0) {
        printf("Update: rotationX = %.2f rotationY = %.2f rotationZ = %.2f\n",
               game->rotationX, game->rotationY, game->rotationZ);
        lastLogTime = currentTime;
    }
}




void draw(Game *game) {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Create perspective matrix manually
    float fovy = 45.0f; // Field of view in degrees
    float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    float zNear = 0.1f; // Near clipping plane
    float zFar = 100.0f; // Far clipping plane

    float f = 1.0f / tanf(fovy * 3.14159265358979323846f / 360.0f);
    float nf = 1.0f / (zNear - zFar);

    float m[16] = {
            f / aspect, 0.0f, 0.0f, 0.0f,
            0.0f, f, 0.0f, 0.0f,
            0.0f, 0.0f, (zFar + zNear) * nf, -1.0f,
            0.0f, 0.0f, (2.0f * zFar * zNear) * nf, 0.0f
    };

    glMultMatrixf(m);

    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply the model matrix
    glMultMatrixf(modelMatrix.m);

    // Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

    // Enable client states
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Set up vertex and color pointers
    glColorPointer(3, GL_FLOAT, sizeof(Vertex), (void*)(intptr_t)12);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)(intptr_t)0);

    // Draw the cube
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (void*)(intptr_t)0);

    // Disable client states
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    // Swap buffers and poll events
    glfwSwapBuffers(game->window);
    glfwPollEvents();
}





void destroy(Game *game)
{
    printf("Cleaning up...\n");

    // Delete buffers
    glDeleteBuffers(1, vbo);
    glDeleteBuffers(1, &index);

    // Destroy window and terminate GLFW
    glfwDestroyWindow(game->window);
    glfwTerminate();
}

void run(Game *game)
{
    // Initialize game
    initialize(game);

    // Main game loop
    while (game->isRunning)
    {
        handleInput(game->window, game);
        update(game);
        draw(game);
    }

    // Cleanup
    destroy(game);
}