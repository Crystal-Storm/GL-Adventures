#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// Globals
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

// Error checking
static void GLClearError(){
    while(glGetError()!=GL_NO_ERROR);
}
static bool GLCheckError(const char* function, int line){
    while(GLenum error = glGetError()){
        std::cout << "OpenGL Error: " << error << ", Line: " << line << ", Function: " << function << std::endl;
        return true;
    }
    return false;
}
#define GLCheck(x) GLClearError(); x; GLCheckError(#x, __LINE__);

// VAO, VBO, and IBO
GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;
GLuint gIndexBufferObject = 0;

// Program Object (for shader)
GLuint gGraphicsPipelineShaderProgram = 0;

// Main Loop
bool gQuit = false;

// Globals for Uniform
float gOffsetX=0.0;
float gOffsetZ=1.0;

// Function to load shader source code from file
std::string LoadShaderAsString(const std::string filename){
    std::string result = "";
    std::string line = "";

    std::ifstream myFile(filename);

    while (std::getline(myFile, line)){
        result+=line+"\n";
    }
    myFile.close();

    return result;
}

void GetOpenGLVersionInfo(){
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

// Function to set up vertex data and buffers
void VertexSpecification(){
    // Vertex Positions
    GLfloat vertexData[] = {
        // Triangle data for quad
        -0.5f,-0.5f,0.0f, // Bottom Left
        1.0f,0.0f,0.0f, // Red
        0.5f,-0.5f,0.0f, // Bottom Right
        0.0f,1.0f,0.0f, // Green
        -0.5f,0.5f,0.0f, // Top Left
        0.0f,0.0f,1.0f, // Blue
        0.5f,0.5f,0.0f, // Top Right
        0.5f,0.5f,0.5f, // Gray
    };

    GLuint indexData[] = {
        0,1,2,
        2,1,3
    };

    // Setup VAO
    glGenVertexArrays(1,&gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);
    
    // Setup VBO
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    // Send vertex data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Setup IBO
    glGenBuffers(1, &gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

    // Locations
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Send to vertex shader
    glBindVertexArray(0);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

GLuint CompileShader(GLuint type, const std::string& source){
    GLuint shaderObject;

    if(type==GL_VERTEX_SHADER){
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }else if(type==GL_FRAGMENT_SHADER){
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    } else {
        std::cout << "Only Vertex shader and Fragment shader are supported" << std::endl;
        exit(1);
    }

    const char* charSource = source.c_str();
    glShaderSource(shaderObject, 1, &charSource, nullptr);
    glCompileShader(shaderObject);

    // Check for compilation errors, rewrite later
    GLint isCompiled = 0;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){
        GLint maxLength = 0;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetShaderInfoLog(shaderObject, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(shaderObject);

        std::cout << "Shader compilation error: " << std::string(infoLog.begin(), infoLog.end()) << std::endl;
        exit(1);
    }

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource){
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    // Check for linking errors, rewrite later
    GLint isLinked = 0;
    glGetProgramiv(programObject, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE){
        GLint maxLength = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(programObject, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(programObject);
        glDeleteShader(myVertexShader);
        glDeleteShader(myFragmentShader);

        std::cout << "Program linking error: " << std::string(infoLog.begin(), infoLog.end()) << std::endl;
        exit(1);
    }

    // Detach and delete shaders after linking
    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}

void CreateGraphicsPipeline(){
    // std::string vertexShaderSource = LoadShaderAsString(std::string(SHADER_PATH)+"vertex.glsl");
    // std::string fragmentShaderSource = LoadShaderAsString(std::string(SHADER_PATH)+"fragment.glsl");
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vertex.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/fragment.glsl");

    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

// Function to initialize the SDL and OpenGL context
void InitializeProgram(){
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::cout << "SDL2 could not initialize video subsystem" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gGraphicsApplicationWindow = SDL_CreateWindow("SDL game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);

    if(gGraphicsApplicationWindow==nullptr){
        std::cout << "SDL_Window was not able to be created" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

    if(gOpenGLContext==nullptr){
        std::cout << "OpenGL context not available" << std::endl;
        exit(1);
    }

    // Initialize Glad Library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
        std::cout << "Glad failed to initialize" << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
}

// Function to handle input events
void Input(){
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0){
        if (e.type==SDL_QUIT){
            std::cout << "Goodbye!" << std::endl;
            gQuit = true;
        }
    }

    //Get keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_UP]){
        gOffsetZ-=.001;
        std::cout << "Z Offset: " << gOffsetZ << std::endl;
    }
    if(state[SDL_SCANCODE_DOWN]){
        gOffsetZ+=.001;
        std::cout << "Z Offset: " << gOffsetZ << std::endl;
    }
    if(state[SDL_SCANCODE_LEFT]){
        gOffsetX-=.001;
        std::cout << "X Offset: " << gOffsetX<< std::endl;
    }
    if(state[SDL_SCANCODE_RIGHT]){
        gOffsetX+=.001;
        std::cout << "X Offset: " << gOffsetX<< std::endl;
    }
}

void PreDraw(){
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glViewport(0,0,gScreenWidth,gScreenHeight);
    glClearColor(.2f,.0f,.1f,1.f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Use Shader Program
    glUseProgram(gGraphicsPipelineShaderProgram);

    // Create transformation matrix
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-gOffsetX,0.0f,-gOffsetZ));
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), ((float)gScreenWidth)/((float)gScreenHeight), 0.1f, 10.0f);

    // Find uniform locations
    GLint uTranslateLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "uTranslate");
    GLint uPerspectiveLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "uPerspective");
    // Set uniforms
    if (uTranslateLocation>=0){
        glUniformMatrix4fv(uTranslateLocation,1,GL_FALSE,&translate[0][0]);
    } else {
        std::cout << "Translate uniform not found, does name match?" << std::endl;
        exit(1);
    }
    if (uPerspectiveLocation>=0){
        glUniformMatrix4fv(uPerspectiveLocation,1,GL_FALSE,&perspective[0][0]);
    } else {
        std::cout << "Perspective uniform not found, does name match?" << std::endl;
        exit(1);
    }
}

void Draw(){
    glBindVertexArray(gVertexArrayObject);

    // glDrawArrays(GL_TRIANGLES,0,6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void MainLoop(){
    while(!gQuit){
        Input();
        PreDraw();
        Draw();

        // Update screen
        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
}

void CleanUp(){
    // Destroy the window
    SDL_DestroyWindow(gGraphicsApplicationWindow);

    // Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* argv[]){
    // Setup graphics program
    InitializeProgram();

    // Set up geometry, VAO, and VBO
    VertexSpecification();

    // Create graphics pipeline
    // At the moment we set up the
    // vertex and fragment shaders
    CreateGraphicsPipeline();

    // Call main loop
    MainLoop();

    // When program terminates clean up
    CleanUp();

    return 0;
}