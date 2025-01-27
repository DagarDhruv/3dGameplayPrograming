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
float height = 2.0f;  // Height of the cone
float radius = 0.5f;  // Radius of the base of the cone
int numPoints = 100;  // Number of points to approximate the cone
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
    glColor3f(0.6f, 0.4f, 0.2f); // Brown color for the house base
    glVertex3f(-0.5f, -0.5f, 0.5f); // Front-bottom-left
    glVertex3f(0.5f, -0.5f, 0.5f);  // Front-bottom-right
    glVertex3f(0.5f, 0.0f, 0.5f);   // Front-top-right
    glVertex3f(-0.5f, 0.0f, 0.5f);  // Front-top-left

    glVertex3f(-0.5f, -0.5f, -0.5f); // Back-bottom-left
    glVertex3f(0.5f, -0.5f, -0.5f);  // Back-bottom-right
    glVertex3f(0.5f, 0.0f, -0.5f);   // Back-top-right
    glVertex3f(-0.5f, 0.0f, -0.5f);  // Back-top-left

    glVertex3f(-0.5f, -0.5f, 0.5f); // Left-bottom-front
    glVertex3f(-0.5f, -0.5f, -0.5f); // Left-bottom-back
    glVertex3f(-0.5f, 0.0f, -0.5f);  // Left-top-back
    glVertex3f(-0.5f, 0.0f, 0.5f);   // Left-top-front

    glVertex3f(0.5f, -0.5f, 0.5f); // Right-bottom-front
    glVertex3f(0.5f, -0.5f, -0.5f); // Right-bottom-back
    glVertex3f(0.5f, 0.0f, -0.5f);  // Right-top-back
    glVertex3f(0.5f, 0.0f, 0.5f);   // Right-top-front
    glEnd();

    // Draw the roof (using GL_TRIANGLES)
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the roof
    glVertex3f(-0.5f, 0.0f, 0.5f); // Front-left
    glVertex3f(0.5f, 0.0f, 0.5f);  // Front-right
    glVertex3f(0.0f, 0.5f, 0.0f);  // Top point of the roof

    glVertex3f(-0.5f, 0.0f, -0.5f); // Back-left
    glVertex3f(0.5f, 0.0f, -0.5f);  // Back-right
    glVertex3f(0.0f, 0.5f, 0.0f);   // Top point of the roof

    glVertex3f(-0.5f, 0.0f, 0.5f);  // Left-front
    glVertex3f(-0.5f, 0.0f, -0.5f); // Left-back
    glVertex3f(0.0f, 0.5f, 0.0f);   // Top point of the roof

    glVertex3f(0.5f, 0.0f, 0.5f);   // Right-front
    glVertex3f(0.5f, 0.0f, -0.5f);  // Right-back
    glVertex3f(0.0f, 0.5f, 0.0f);   // Top point of the roof
    glEnd();

    // Draw details like edges (using GL_LINES)
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for edges
    glVertex3f(-0.5f, -0.5f, 0.5f); // Base front edge
    glVertex3f(0.5f, -0.5f, 0.5f);

    glVertex3f(0.5f, -0.5f, -0.5f); // Base back edge
    glVertex3f(-0.5f, -0.5f, -0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f); // Base left edge
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glVertex3f(0.5f, -0.5f, 0.5f); // Base right edge
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f); // Back-bottom-left
    glVertex3f(0.5f, -0.5f, -0.5f);  // Back-bottom-right
    glVertex3f(0.5f, 0.0f, -0.5f);   // Back-top-right
    glVertex3f(-0.5f, 0.0f, -0.5f);  // Back-top-left
    glEnd();
   // glBegin(GL_POINTS);  // Draw using points

//    // Generate points along the cone's surface
//    for (int i = 0; i < numPoints; ++i) {
//        float theta = (2.0f * M_PI * i) / numPoints;  // Angle around the cone
//        float r = radius * (1 - i / (numPoints));  // Radius decreases as height increases
//        float y = height * (i / (numPoints));  // Height increases with each iteration
//
//        // cone: x = r * cos(theta), z = r * sin(theta)
//        float x = r * cos(theta);
//        float z = r * sin(theta);
//
//        glVertex3f(x, y, z);  // Plot the point at (x, y, z)
//    }
//    glEnd();
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
        game->rotationAngleY += ROTATION_SPEED * (deltaTime); // 45 degrees per second
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
