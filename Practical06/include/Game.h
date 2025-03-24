#include <iostream>

#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


using namespace std;
using namespace sf;

class Game
{
public:
    Game();
    ~Game();
    void run();
    void checkShaderCompilation(GLuint shader, const std::string& shaderName);
    void checkProgramLinking(GLuint program);
private:
    Window window;
    bool isRunning = false;
    void initialize();
    void update();
    void render();
    void unload();

    Clock clock;
    Time elapsed;
};