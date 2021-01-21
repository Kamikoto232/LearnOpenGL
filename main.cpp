#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL/SOIL.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

// GLM Mathematics
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ObjLoader.h"

float vertices2[] = {
    //pos                
    -0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f, -0.5f,  0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f, 

    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 

     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 

    -0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f 
};

float uv2[]= {
 //UV
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,
 0.0f, 1.0f,
 1.0f, 1.0f,
 1.0f, 0.0f,
 1.0f, 0.0f,
 0.0f, 0.0f,
 0.0f, 1.0f,
 0.0f, 1.0f,
 1.0f, 1.0f,
 1.0f, 0.0f,
 1.0f, 0.0f,
 0.0f, 0.0f,
 0.0f, 1.0f
};

GLuint indices2[] = {  // Помните, что мы начинаем с 0!
    0, 1, 3,   // Первый треугольник
    1, 2, 3    // Второй треугольник
};

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -20.0f), 
  glm::vec3(-1.5f, -2.2f, -5.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -5.5f),  
  glm::vec3( 1.5f,  2.0f, -5.5f), 
  glm::vec3( 1.5f,  0.2f, -5.5f), 
  glm::vec3(-1.3f,  1.0f, -5.5f)  
};

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Window dimensions
const GLuint WIDTH = 500, HEIGHT = 400;

bool keys[1024];

GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра

GLfloat lastX = 400, lastY = 300;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
bool CompileVertexShader(GLuint *vertexShader);
bool CompileFragmentShader(GLuint *fragmentShader);
GLuint LinkShaders(GLuint *vertexShader, GLuint *fragmentShader);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    

    // Define the viewport dimensions
    int texwidth, texheight;
    glfwGetFramebufferSize(window, &texwidth, &texheight);  
    glViewport(0, 0, texwidth, texheight);
	printf("w=%i h=%i",texwidth, texheight);

	// Build and compile our shader program

    Shader ourShader("shader.vs", "shader.frag");
    Shader lampShader("shader.vs", "lamp.frag");

    // Читаем наш OBJ файл
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals; // Сейчас мы еще не работаем с нормалями
    bool res = loadOBJ("model.obj", vertices, uvs, normals);

	GLuint VAO, VBO, EBO; //id каждого буфера

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); //генерируем vbo буфер вершин
    glGenBuffers(1, &EBO);

    // 1. Привязываем VAO там будут все наши параметры которые мы задаем ниже
    glBindVertexArray(VAO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //привязали GL_ARRAY_BUFFER к VBO С этого момента любой вызов, использующий буфер, будет работать с VBO. 
    //Теперь мы можем вызвать glBufferData для копирования вершинных данных в этот буфер.
    // 2. Копируем наш массив вершин в буфер для OpenGL 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer); 
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer); //работаем с uvBuffer
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(
    1,                                // атрибут. Нет особых причин писать 1, главное, чтобы совпадало со значением в шейдере layout (location = 1)
    2,                                // размер
    GL_FLOAT,                         // тип
    GL_FALSE,                         // нормализировано ли?
    0,                                // шаг
    (GLvoid*)0                          //смещение в буфере
    );

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // Третий атрибутный буфер : нормали
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
    2,                                // атрибут
    3,                                // размер
    GL_FLOAT,                        // тип
    GL_FALSE,                        // нормализованный ли?
    0,                                // шаг
    (void*)0                         // смещение в буфере
    );
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    // 3. Устанавливаем указатели на вершинные атрибуты 
    // Position attribute
    
    // Color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    // TexCoord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(2);
    //glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	
    
    //4. Отвязываем VAO
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


    //про текстуры https://habr.com/ru/post/315294/
    GLuint texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    //освобождение участка памяти, выделенного под загруженное изображение, и 
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    //texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); //отвязка объекта текстуры

    //glEnable(GL_CULL_FACE);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement();
        // Render
        glEnable(GL_DEPTH_TEST);

        // Clear the uvBuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0); // Активируем текстурный блок перед привязкой текстуры

        ourShader.Use();// Activate shader

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 MVP;
        // Матрица модели
        glm::mat4 model;

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "M");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "V");
        GLint projectionLoc = glGetUniformLocation(ourShader.Program, "P");
        GLint MVPLoc = glGetUniformLocation(ourShader.Program, "MVP");
        GLint lightPosLoc = glGetUniformLocation(ourShader.Program, "WorldSpaceLightPos0");
        GLint viewPosLoc = glGetUniformLocation(ourShader.Program, "WorldSpaceCameraPos");

        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.01f, 1000.0f);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

        glBindVertexArray(VAO);
        model= glm::mat4(1.0f);
        lightPos.x = cos(glfwGetTime());
        lightPos.z = sin(glfwGetTime());
        //model = glm::lookAt(lightPos, lightPos + glm::vec3(0,0,0), glm::vec3(0,0,1));
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        MVP = projection * view * model;
        glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &MVP[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 100);

        for(GLuint i = 0; i < 10; i++)
        {
            GLfloat angle = 20.0f * i;
            model= glm::mat4(1.0f);

            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
           
            MVP = projection * view * model;

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &MVP[0][0]);
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

	// Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_1){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(key == GLFW_KEY_2){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if(action == GLFW_PRESS)
    keys[key] = true;
    else if(action == GLFW_RELEASE)
    keys[key] = false;
}

void do_movement()
{
   // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if(keys[GLFW_KEY_Q])
        camera.ProcessKeyboard(DOWN, deltaTime);
    if(keys[GLFW_KEY_E])
        camera.ProcessKeyboard(UP, deltaTime);
        
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
    // 1. Position = known
    // 2. Calculate cameraDirection
    glm::vec3 zaxis = glm::normalize(position - target);
    // 3. Get positive right axis vector
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
    // 4. Calculate camera up vector
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    // Create translation and rotation matrix
    // In glm we access elements as mat[col][row] due to column-major layout
    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    translation[3][0] = -position.x; // Third column, first row
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x; // First column, first row
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; // First column, second row
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; // First column, third row
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z; 

    // Return lookAt matrix as combination of translation and rotation matrix
    return rotation * translation; // Remember to read from right to left (first translation then rotation)
}