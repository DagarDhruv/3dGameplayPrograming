#include <./include/game.h>

// Global flag to control update state
bool updatable = false;

// Global constants
const int SCREEN_WIDTH = 800;       // Screen Width
const int SCREEN_HEIGHT = 600;      // Screen Height
const float ROTATION_SPEED = 45.0f; // Degrees per second
const float FOV = 45.0f;            // Field of view in degrees
const float NEAR_PLANE = 1.0f;      // Near clipping plane for the camera
const float FAR_PLANE = 500.0f;     // Far clipping plane for the camera
const float Distance = 1.0f;       //speed at which the cube moves from camera
float Scale = 1.0f;
// Define vertex positions for a Pyramid
// Format: X, Y, Z coordinates for each vertex
// Vertex positions (8 unique vertices)
float vertices[] = {
        // Front face
        -1.0f, -1.0f, -5.0f, // 0 - Bottom-left
        1.0f, -1.0f, -5.0f, // 1 - Bottom-right
        1.0f,  1.0f, -5.0f, // 2 - Top-right
        -1.0f,  1.0f, -5.0f, // 3 - Top-left

        // Back face
        -1.0f, -1.0f, -7.0f, // 4 - Bottom-left
        1.0f, -1.0f, -7.0f, // 5 - Bottom-right
        1.0f,  1.0f, -7.0f, // 6 - Top-right
        -1.0f,  1.0f, -7.0f  // 7 - Top-left
};


// Define colors for each vertex
// Format: R, G, B values for each vertex
float colors[] = {
        1.0f, 0.0f, 0.0f,  // 0 - Red
        0.0f, 1.0f, 0.0f,  // 1 - Green
        0.0f, 0.0f, 1.0f,  // 2 - Blue
        1.0f, 1.0f, 0.0f,  // 3 - Yellow

        1.0f, 0.0f, 1.0f,  // 4 - Magenta
        0.0f, 1.0f, 1.0f,  // 5 - Cyan
        1.0f, 1.0f, 1.0f,  // 6 - White
        0.0f, 0.0f, 0.0f   // 7 - Black
};


// Define indices to connect vertices into triangles
// Format: Groups of 3 indices that define each triangle
// Indices to form 12 triangles (2 per face)
unsigned int indices[] = {
        // Front face
        0, 1, 2,  0, 2, 3,

        // Back face
        4, 6, 5,  4, 7, 6,

        // Left face
        4, 0, 3,  4, 3, 7,

        // Right face
        1, 5, 6,  1, 6, 2,

        // Top face
        3, 2, 6,  3, 6, 7,

        // Bottom face
        4, 5, 1,  4, 1, 0
};


/**
 * Initializes the game state and OpenGL settings
 * Sets up projection matrix, creates display lists, and initializes timing
 */
void initialize(Game *game)
{
    // Set initial game state
    game->isRunning = 1;

    // Set background color to black (R=0, G=0, B=0, A=0)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Setup view perspective projection matrix
    glMatrixMode(GL_PROJECTION); // Switch to projection matrix mode
    glLoadIdentity();            // Reset projection matrix

    // Set up perspective: 45 Degrees field of view, 4:3 aspect ratio, near=1.0, far=500.0
    gluPerspective(FOV, SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
    glMatrixMode(GL_MODELVIEW); // Switch back to modelview matrix

    // Initialize timing for animation
    game->lastTime = glfwGetTime();
}

/**
 * Handles Game Input
 */
void handleInput(GLFWwindow *window, Game *game)
{
    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - game->lastTime;

    // Y-axis rotation (Left/Right arrows)
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        // Update rotation continuously using deltaTime
        game->rotationY += ROTATION_SPEED * deltaTime; // 45 degrees per second
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        // Update rotation continuously using deltaTime
        game->rotationY -= ROTATION_SPEED * deltaTime; // 45 degrees per second
    }

    // Z-axis rotation (Up/Down arrows)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        game->rotationZ += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        game->rotationZ -= ROTATION_SPEED * deltaTime;
    }
    // X-axis rotation (W/S keys)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        game->rotationX += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        game->rotationX -= ROTATION_SPEED * deltaTime;
    }
    //distance between camera and cube  (A/D keys)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        game->Distancebwcubeandcamera += Distance * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        game->Distancebwcubeandcamera -= Distance * deltaTime;
    }
    //set the scale for the cube  (+/- keys)
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        Scale += Distance * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        Scale -= Distance * deltaTime;
    }

    // Keep rotations between 0 and 360 degrees
    if (game->rotationY > 360.0f)
        game->rotationY -= 360.0f;
    if (game->rotationY < 0.0f)
        game->rotationY += 360.0f;
    if (game->rotationX > 360.0f)
        game->rotationX -= 360.0f;
    if (game->rotationX < 0.0f)
        game->rotationX += 360.0f;
}

/**
 * Updates game logic
 * Handles rotation timing and angle calculations
 */
void update(Game *game)
{
    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();

    // Update last time
    game->lastTime = currentTime;

    static double lastLogTime = 0.0;
    if (currentTime - lastLogTime >= 1.0)
    {
        printf("Update : rotationY = %.2f\n", game->rotationY);
        lastLogTime = currentTime;
    }
}

/**
 * Renders the scene
 * Clears buffers, applies transformations, and draws the Vertex Array
 */
void draw(Game *game)
{
    // Clear color and depth buffers for new frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();

    // Update last time
    game->lastTime = currentTime;

    static double lastLogTime = 0.0;
    if (currentTime - lastLogTime >= 1.0)
    {
        printf("Drawing Model\n");
        lastLogTime = currentTime;
    }

    // Reset modelview matrix before applying transformations
    glLoadIdentity();

    // Apply translation to move the cube away from the camera
    glTranslatef(0.0f, 0.0f, game->Distancebwcubeandcamera);

    glScalef(Scale, Scale, Scale);
    // Apply rotation around Y and Z axes
    glRotatef(game->rotationY, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
    glRotatef(game->rotationZ, 0.0f, 0.0f, 1.0f); // Rotate around Z-axis
    glRotatef(game->rotationX, 1.0f, 0.0f, 0.0f); // Rotate around X-axis

    // Enable vertex and color arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Set up vertex and color pointers
    glVertexPointer(3, GL_FLOAT, 0, &vertices);
    glColorPointer(3, GL_FLOAT, 0, &colors);

    // Draw the pyramid using indices
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, &indices);


    // Disable vertex and color arrays
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Swap front and back buffers to display the rendered frame
    glfwSwapBuffers(game->window);
}

/**
 * Main game loop
 * Initializes GLFW, creates window, and runs the game loop
 */
void run(Game *game)
{
    // Initialize GLFW library
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Create a windowed mode window and its OpenGL context
    game->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLFW OpenGL VBA Vertex Arrays", NULL, NULL);
    if (!game->window)
    {
        glfwTerminate();
        printf("Failed to create GLFW window\n");
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(game->window);

    // Initialize game state and OpenGL settings
    initialize(game);

    // Main game loop
    while (!glfwWindowShouldClose(game->window))
    {
        handleInput(game->window, game); // Handle game input
        update(game);                    // Update game logic
        draw(game);                      // Render frame
        glfwPollEvents();                // Process window events
    }

    // Cleanup resources
    destroy(game);
    glfwDestroyWindow(game->window);
    glfwTerminate();
}

/**
 * Cleanup function
 * Releases allocated resources
 */
void destroy(Game *game)
{
    printf("Cleaning up\n");
    (void)game; // Unused
}