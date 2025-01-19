#include <./include/game.h>

// Global flag to control update state
bool updatable = false;

// Global constants
const int SCREEN_WIDTH = 800;       // Screen Width
const int SCREEN_HEIGHT = 600;      // Screen Height
const float ROTATION_SPEED = 45.0f; // Degrees per second
const float FOV = 45.0f;            // Field of view in degrees
const float NEAR_PLANE = 1.0f;      // Near clipping plane for the camera
const float FAR_PLANE = 200.0f;     // Far clipping plane for the camera
const float Distance = 1.0f;       //speed at which the cube moves from camera 
float Scale = 1.0f;
/**
 * Initializes the game state and OpenGL settings
 * Sets up projection matrix, creates display lists, and initializes timing
 */
void initialize(Game* game)
{
    // Set initial game state
    game->isRunning = 1;

    // Set background color to black (R=0, G=0, B=0, A=0)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Setup perspective projection matrix
    glMatrixMode(GL_PROJECTION); // Switch to projection matrix mode
    glLoadIdentity();            // Reset projection matrix

    // Set up perspective: 45 Degrees field of view, 4:3 aspect ratio, near=1.0, far=500.0
    gluPerspective(FOV, SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
    glMatrixMode(GL_MODELVIEW); // Switch back to modelview matrix

    // Create a new display list for the cube
    game->index = glGenLists(1);        // Generate one display list
    glNewList(game->index, GL_COMPILE); // Start recording display list

    // Begin defining cube geometry using quads
    glBegin(GL_QUADS);
    {
        // Front face of cube (Blue)
        glColor3f(0.0f, 0.0f, 1.0f); // Set color to blue
        glVertex3f(1.0f, 1.0f, -5.0f);  // Top-right vertex
        glVertex3f(-1.0f, 1.0f, -5.0f); // Top-left vertex
        glVertex3f(-1.0f, -1.0f, -5.0f); // Bottom-left vertex
        glVertex3f(1.0f, -1.0f, -5.0f);  // Bottom-right vertex

        // Back face of cube (Green)
        glColor3f(0.0f, 1.0f, 0.0f); // Set color to green
        glVertex3f(1.0f, 1.0f, -7.0f);  // Top-right vertex
        glVertex3f(-1.0f, 1.0f, -7.0f); // Top-left vertex
        glVertex3f(-1.0f, -1.0f, -7.0f); // Bottom-left vertex
        glVertex3f(1.0f, -1.0f, -7.0f);  // Bottom-right vertex

        // Left face of cube (Red)
        glColor3f(1.0f, 0.0f, 0.0f); // Set color to red
        glVertex3f(-1.0f, 1.0f, -5.0f);  // Top-right vertex
        glVertex3f(-1.0f, 1.0f, -7.0f); // Top-left vertex
        glVertex3f(-1.0f, -1.0f, -7.0f); // Bottom-left vertex
        glVertex3f(-1.0f, -1.0f, -5.0f);  // Bottom-right vertex

        // Right face of cube (Yellow)
        glColor3f(1.0f, 1.0f, 0.0f); // Set color to yellow
        glVertex3f(1.0f, 1.0f, -5.0f);  // Top-right vertex
        glVertex3f(1.0f, 1.0f, -7.0f); // Top-left vertex
        glVertex3f(1.0f, -1.0f, -7.0f); // Bottom-left vertex
        glVertex3f(1.0f, -1.0f, -5.0f);  // Bottom-right vertex

        // Top face of cube (Cyan)
        glColor3f(0.0f, 1.0f, 1.0f); // Set color to cyan
        glVertex3f(1.0f, 1.0f, -5.0f);  // Top-right vertex
        glVertex3f(-1.0f, 1.0f, -5.0f); // Top-left vertex
        glVertex3f(-1.0f, 1.0f, -7.0f); // Bottom-left vertex
        glVertex3f(1.0f, 1.0f, -7.0f);  // Bottom-right vertex

        // Bottom face of cube (Magenta)
        glColor3f(1.0f, 0.0f, 1.0f); // Set color to magenta
        glVertex3f(1.0f, -1.0f, -5.0f);  // Top-right vertex
        glVertex3f(-1.0f, -1.0f, -5.0f); // Top-left vertex
        glVertex3f(-1.0f, -1.0f, -7.0f); // Bottom-left vertex
        glVertex3f(1.0f, -1.0f, -7.0f);  // Bottom-right vertex
    }
    glEnd();     // End geometry definition
    glEndList(); // End display list compilation

    // Initialize timing for animation
    game->lastTime = glfwGetTime();
}

/**
 * Handles Game Input
 */
void handleInput(GLFWwindow* window, Game* game)
{
    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - game->lastTime;

    // Y-axis rotation (Left/Right arrows)
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        // Update rotation continuously using deltaTime
        game->rotationAngleY += ROTATION_SPEED * deltaTime; // 45 degrees per second
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        // Update rotation continuously using deltaTime
        game->rotationAngleY -= ROTATION_SPEED * deltaTime; // 45 degrees per second
    }

    // Z-axis rotation (Up/Down arrows)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        game->rotationAngleZ += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        game->rotationAngleZ -= ROTATION_SPEED * deltaTime;
    }
    // X-axis rotation (W/S keys)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        game->rotationAngleX += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        game->rotationAngleX -= ROTATION_SPEED * deltaTime;
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
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        Scale += Distance * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        Scale -= Distance * deltaTime;
    }

    // Keep rotation angles between 0 and 360 degrees
    if (game->rotationAngleY > 360.0f)
    {
        game->rotationAngleY -= 360.0f;
    }
    if (game->rotationAngleZ > 360.0f)
    {
        game->rotationAngleZ -= 360.0f;
    }
    if (game->rotationAngleX > 360.0f)
    {
        game->rotationAngleX -= 360.0f;
    }
    if (game->Distancebwcubeandcamera > FAR_PLANE)
    {
        game->Distancebwcubeandcamera = FAR_PLANE;
    }
    if (game->Distancebwcubeandcamera > NEAR_PLANE)
    {
        game->Distancebwcubeandcamera = NEAR_PLANE;
    }
}

/**
 * Updates game logic
 * Handles rotation timing and angle calculations
 */
void update(Game* game)
{
    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();

    // Update last time
    game->lastTime = currentTime;

    static double lastLogTime = 0.0;
    if (currentTime - lastLogTime >= 1.0)
    {
        printf("Update : rotationAngleY = %.2f, rotationAngleZ = %.2f, rotationAngleX = %.2f, distance between camera and cube  = %.2f,  scale  = %.2f\n", game->rotationAngleY, game->rotationAngleZ, game->rotationAngleX,game->Distancebwcubeandcamera ,Scale);
        lastLogTime = currentTime;
    }
}

/**
 * Renders the scene
 * Clears buffers, applies transformations, and draws the cube
 */
void draw(Game* game)
{
    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();

    // Clear color and depth buffers for new frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update last time
    game->lastTime = currentTime;

    glLoadIdentity(); // Reset modelview matrix

    // Apply translation to move the cube away from the camera
    glTranslatef(0.0f, 0.0f, game->Distancebwcubeandcamera);

    glScalef(Scale, Scale, Scale);
    // Apply rotation around Y and Z axes
    glRotatef(game->rotationAngleY, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
    glRotatef(game->rotationAngleZ, 0.0f, 0.0f, 1.0f); // Rotate around Z-axis
    glRotatef(game->rotationAngleX, 1.0f, 0.0f, 0.0f); // Rotate around X-axis

    // Draw the cube using the display list
    glCallList(game->index);

    // Swap front and back buffers to display the rendered frame
    glfwSwapBuffers(game->window);
}

/**
 * Main game loop
 * Initializes GLFW, creates window, and runs the game loop
 */
void run(Game* game)
{
    // Initialize GLFW library
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Create a windowed mode window and its OpenGL context
    game->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLFW OpenGL Cube", NULL, NULL);
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
void destroy(Game* game)
{
    printf("Cleaning up\n");
    glDeleteLists(game->index, 1); // Delete the display list
}
