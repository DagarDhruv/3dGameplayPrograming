#include <./include/Debug.h>
#include <./include/Game.h>
#include<cmath>

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Texturing")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning)
	{

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

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
	float color[4];
	float texel[2];
} Vertex;

Vertex vertex[9];
GLubyte triangles[48];

/* Variable to hold the VBO identifier and shader data */

GLuint index,	// Index to draw
vsid,		// Vertex Shader ID
fsid,		// Fragment Shader ID
progID,		// Program ID
vbo = 1,	// Vertex Buffer ID
positionID, // Position ID
colorID,	// Color ID
to,			// Texture ID 1 to 32
textureID,	// Texture ID
texelID;	// Texel ID

// const string filename = "./assets/texture.tga";
 const string filename = "./assets/cube.tga";

//const string filename = "./assets/texture.tga";

int width;			  // width of texture
int height;			  // height of texture
int comp_count;		  // Component of texture
const int number = 4; // 4 = RGBA

unsigned char* img_data;

void Game::initialize()
{
	isRunning = true;

	GLint isCompiled = 0;
	GLint isLinked = 0;

	glewInit();

	/* Vertices counter-clockwise winding */
	////back face VVVVVVV
	vertex[0].coordinate[0] = -0.5f; //front
	vertex[0].coordinate[1] = -0.5f;//bottom
	vertex[0].coordinate[2] = -0.5f;//left

	vertex[1].coordinate[0] = 0.5f;
	vertex[1].coordinate[1] = -0.5f;  //bottom
	vertex[1].coordinate[2] = -0.5f;//right

	vertex[2].coordinate[0] = 0.5f;
	vertex[2].coordinate[1] = 0.5f;   //top
	vertex[2].coordinate[2] = -0.5f;//right

	vertex[3].coordinate[0] = -0.5f;//left
	vertex[3].coordinate[1] = 0.5f;  //top
	vertex[3].coordinate[2] = -0.5f;

	/////Front face VVVVVVV

	vertex[4].coordinate[0] = -0.5f;//left
	vertex[4].coordinate[1] = -0.5f;  //bottom
	vertex[4].coordinate[2] = 0.5f;

	vertex[5].coordinate[0] = 0.5f;
	vertex[5].coordinate[1] = -0.5f;  //bottom
	vertex[5].coordinate[2] = 0.5f;//right

	vertex[6].coordinate[0] = 0.5f;
	vertex[6].coordinate[1] = 0.5f;   //top
	vertex[6].coordinate[2] = 0.5f;//right

	vertex[7].coordinate[0] = -0.5f;//left
	vertex[7].coordinate[1] = 0.5f;   //top
	vertex[7].coordinate[2] = 0.5f;


	// Add a new vertex for the top center (roof peak) at y = 1.0f
	vertex[8].coordinate[0] = 0.0f;  // X: Center of the top face
	vertex[8].coordinate[1] = 1.0f;  // Y: At the top of the cube (height = 1.0f)
	vertex[8].coordinate[2] = 0.0f;  // Z: Center of the top face


	/* Colors counter-clockwise winding */

	vertex[0].color[0] = 0.1f;
	vertex[0].color[1] = 1.0f;
	vertex[0].color[2] = 0.0f;

	vertex[1].color[0] = 0.2f;
	vertex[1].color[1] = 1.0f;
	vertex[1].color[2] = 0.0f;

	vertex[2].color[0] = 0.3f;
	vertex[2].color[1] = 1.0f;
	vertex[2].color[2] = 0.0f;

	vertex[3].color[0] = 0.1f;
	vertex[3].color[1] = 1.0f;
	vertex[3].color[2] = 0.0f;

	vertex[4].color[0] = 0.2f;
	vertex[4].color[1] = 1.0f;
	vertex[4].color[2] = 0.0f;

	vertex[5].color[0] = 0.3f;
	vertex[5].color[1] = 1.0f;
	vertex[5].color[2] = 0.0f;

	vertex[6].color[0] = 0.1f;
	vertex[6].color[1] = 1.0f;
	vertex[6].color[2] = 0.0f;

	vertex[7].color[0] = 0.2f;
	vertex[7].color[1] = 1.0f;
	vertex[7].color[2] = 0.0f;

	vertex[8].color[0] = 1.0f;  // R: Roof color
	vertex[8].color[1] = 0.0f;  // G: Roof color
	vertex[8].color[2] = 0.0f;  // B: Roof color





	GLubyte triangles[] = {
			0, 1, 2,  2, 3, 0,
			1, 5, 6,  6, 2, 1,
			5, 4, 7,  7, 6, 5,
			4, 0, 3,  3, 7, 4,
			3, 2, 6,  6, 7, 3,
			4, 5, 1,  1, 0, 4,

			// Roof (connecting the top center vertex to the corners of the top face)
			8, 2, 3,  // Center to front-left fronttt
			8, 6, 7,  // Center to front-right
			8, 7, 3,  // Center to back-right
			8, 6, 2,  // Center to back-left
	};

	/* Texel UV, ST, xy texture mappings counter-clockwise winding */
	vertex[0].texel[0] = 0.0f;
	vertex[0].texel[1] = 0.0f;

	vertex[1].texel[0] = 1.0f;
	vertex[1].texel[1] = 0.0f;

	vertex[2].texel[0] = 1.0f;
	vertex[2].texel[1] = 1.0f;

	vertex[3].texel[0] = 0.0f;
	vertex[3].texel[1] = 1.0f;

	vertex[4].texel[0] = 0.0f;
	vertex[4].texel[1] = 0.0f;

	vertex[5].texel[0] = 1.0f;
	vertex[5].texel[1] = 0.0f;

	vertex[6].texel[0] = 1.0f;
	vertex[6].texel[1] = 1.0f;

	vertex[7].texel[0] = 0.0f;
	vertex[7].texel[1] = 1.0f;


	/* Create a new VBO using VBO id */
	glGenBuffers(1, &vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 9, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 48, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */
	const char* vs_src = "#version 400\n\r"
		"in vec4 sv_position;"
		"in vec4 sv_color;"
		"in vec2 sv_texel;"
		"out vec4 color;"
		"out vec2 texel;"
		"void main() {"
		"	color = sv_color;"
		"	texel = sv_texel;"
		"	gl_Position = sv_position;"
		"}"; // Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER);				 // Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid);									 // Check that the shader compiles

	// Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE)
	{
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	/* Fragment Shader which would normally be loaded from an external file */
	const char* fs_src = "#version 400\n\r"
		"uniform sampler2D f_texture;"
		"in vec4 color;"
		"in vec2 texel;"
		"out vec4 fColor;"
		"void main() {"
		//"	fColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);"
		"	fColor = texture(f_texture, texel.st);"
		"}"; // Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);
	// Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE)
	{
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	  // Create program in GPU
	glAttachShader(progID, vsid); // Attach Vertex Shader to Program
	glAttachShader(progID, fsid); // Attach Fragment Shader to Program
	glLinkProgram(progID);

	// Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1)
	{
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	// Setup the Texture Data and send to GPU
	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to);
	glBindTexture(GL_TEXTURE_2D, to);

	// Wrap around
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Filtering
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Bind to OpenGL
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(GL_TEXTURE_2D, // 2D Texture Image
		0,				// Mipmapping Level
		GL_RGBA,		// GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA
		width,			// Width
		height,		// Height
		0,				// Border
		GL_RGBA,		// Bitmap
		GL_UNSIGNED_BYTE, img_data);

	// Find variables in the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	texelID = glGetAttribLocation(progID, "sv_texel");
	textureID = glGetUniformLocation(progID, "f_texture");
}

void Game::update()
{
	elapsed = clock.getElapsedTime();
	for (int i = 0; i < 9; i++)
	{
		float x = vertex[i].coordinate[0];
		float y = vertex[i].coordinate[1];
		float z = vertex[i].coordinate[2];

		// Rotate around the Y-axis
		vertex[i].coordinate[0] = x * cos(0.0001f) - z * sin(0.0001f);
		vertex[i].coordinate[2] = x * sin(0.0001f) + z * cos(0.0001f);
	}

	for (int i = 0; i < 9; i++)
	{
		// Store original coordinates
		float x = vertex[i].coordinate[0];
		float y = vertex[i].coordinate[1];

		// Rotate around the Z-axis
		// Z-axis rotation affects X and Y coordinates
		vertex[i].coordinate[0] = x * cos(0.0001f) - y * sin(0.0001f);
		vertex[i].coordinate[1] = x * sin(0.0001f) + y * cos(0.0001f);
	}
}

	void Game::render()
	{

#if (DEBUG >= 2)
		DEBUG_MSG("Drawing...");
#endif

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

		/*	As the data positions will be updated by the this program on the
			CPU bind the updated data to the GPU for drawing	*/
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 9, vertex, GL_STATIC_DRAW);

		/*	Draw Triangle from VBO	(set where to start from as VBO can contain
			model components that 'are' and 'are not' to be drawn )	*/

			// Set pointers for each parameter
			// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
		glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (float*)NULL + 3);
		glVertexAttribPointer(texelID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));;

		// Enable Arrays
		glEnableVertexAttribArray(positionID);
		glEnableVertexAttribArray(colorID);
		glEnableVertexAttribArray(texelID);

		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, (char*)NULL + 0);

		window.display();
	}

	void Game::unload()
	{
		cout << "Cleaning up" << endl;

#if (DEBUG >= 2)
		DEBUG_MSG("Cleaning up...");
#endif

		glDeleteProgram(progID);
		glDeleteBuffers(1, &vbo);
	}
