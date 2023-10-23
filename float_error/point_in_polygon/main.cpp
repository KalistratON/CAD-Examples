#include <iostream>
#include <memory>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderLoader.h"

static GLboolean INPUT_FLAG_ENABLE = GL_FALSE;
static GLboolean FIRST_MISSED = GL_FALSE;
const GLfloat PI = 3.141592f;
const GLfloat PRECISION = 0.001;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);

GLfloat dot(const std::unique_ptr<GLfloat[]>&, GLuint, GLuint);
GLfloat cross(const std::unique_ptr<GLfloat[]>&, GLuint, GLuint);

GLboolean inPolygon();

static GLboolean isCalculate = GL_FALSE;
static GLboolean isDrawnPolygon = GL_FALSE;
static GLboolean isDrawnPoint = GL_FALSE;

const GLuint WIDTH = 700, HEIGHT = 900;

const GLchar* vertexShaderSource =  "#version 400 core\n"
                                    "layout (location = 0) in vec3 position;\n"
                                    "void main()\n"
                                    "{\n"
                                    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                                    "}\0";

const GLchar* fragmentShaderSource ="#version 400 core\n"
                                    "out vec4 color;\n"
                                    "void main()\n"
                                    "{\n"
                                    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                    "}\n\0";

const   GLuint  SIZE_POLYGON = 100;
static  GLuint  CURRENT_INDEX = 0;
static  GLfloat VERTICES[3 * SIZE_POLYGON];
static  GLuint  INDEXES[2 * SIZE_POLYGON];
static  GLfloat POINT[3] = {-2., -2., .0};


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    ShaderLoader shaderLoader;
    shaderLoader.compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    shaderLoader.compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = shaderLoader.linkShader();
    shaderLoader.useProgram(shaderProgram);

    GLuint VBO, VAO, EBO;
    GLuint PBO, PAO;

    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &PAO);

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &PBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * CURRENT_INDEX, VERTICES, GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * (CURRENT_INDEX + 1) * sizeof(GLuint), INDEXES, GL_DYNAMIC_DRAW);

        glLineWidth(2.0f);
        glPointSize(2.0f);
        glDrawElements(GL_LINES, 2 * CURRENT_INDEX + 2, GL_UNSIGNED_INT, 0);
                

        if (INPUT_FLAG_ENABLE && FIRST_MISSED)
        {
            std::cin >> POINT[0] >> POINT[1];
            isDrawnPoint = GL_TRUE;
        }

        if (isDrawnPoint)
        {
            auto ans = inPolygon() ? "Yes" : "No";
            std::cout << "in Polygon ?: " << ans << std::endl;

            isDrawnPoint = GL_FALSE;
        }

        if (INPUT_FLAG_ENABLE)
            FIRST_MISSED = GL_TRUE;
        
        glBindVertexArray(PAO);
        glBindBuffer(GL_ARRAY_BUFFER, PBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat), POINT, GL_DYNAMIC_DRAW);

        glDrawArrays(GL_POINTS, 0, 1);

        glEnable(GL_PROGRAM_POINT_SIZE);

        

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &PAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &PBO);

    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && !isDrawnPolygon)
    {
        double x;
        double y;

        glfwGetCursorPos(window, &x, &y);

        VERTICES[3 * CURRENT_INDEX] = x / WIDTH * 2 - 1;
        VERTICES[3 * CURRENT_INDEX + 1] = 1 - y / HEIGHT * 2;
        VERTICES[3 * CURRENT_INDEX + 2] = 0;

        std::cout << VERTICES[3 * CURRENT_INDEX] << " : " << VERTICES[3 * CURRENT_INDEX + 1] << std::endl;

        if (CURRENT_INDEX)
        {
            INDEXES[2 * CURRENT_INDEX - 2] = CURRENT_INDEX - 1;
            INDEXES[2 * CURRENT_INDEX - 1] = CURRENT_INDEX;
        }

        ++CURRENT_INDEX;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE && !isDrawnPolygon)
    {
        if (CURRENT_INDEX)
        {
            INDEXES[2 * CURRENT_INDEX - 2] = CURRENT_INDEX - 1;
            INDEXES[2 * CURRENT_INDEX - 1] = INDEXES[0];

            isDrawnPolygon = GL_TRUE;
            INPUT_FLAG_ENABLE = GL_TRUE;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && isDrawnPolygon)
    {
        double x;
        double y;

        glfwGetCursorPos(window, &x, &y);

        POINT[0] = x / WIDTH * 2 - 1;
        POINT[1] = 1 - y / HEIGHT * 2;
        POINT[2] = 0;

        std::cout << "\nDefined point :\n" << POINT[0] << " : " << POINT[1] << std::endl;

        isDrawnPoint = GL_TRUE;
    }
}

GLfloat dot(const std::unique_ptr<GLfloat[]>& vectors, GLuint i, GLuint j)
{
    return vectors[2 * i] * vectors[2 * j] + vectors[2 * i + 1] * vectors[2 * j + 1];
}

GLfloat cross(const std::unique_ptr<GLfloat[]>& vectors, GLuint i, GLuint j)
{
    return vectors[2 * i] * vectors[2 * j + 1] - vectors[2 * i + 1] * vectors[2 * j] >= .0 ? 1.0 : -1.0;
}

GLboolean inPolygon()
{
    auto spokes = std::make_unique<GLfloat[]>(2 * CURRENT_INDEX);
    GLfloat sum_angles = .0;
    for (size_t i = 0; i < CURRENT_INDEX; i++)
    {
        GLfloat t = 4.0 / std::sqrtf((VERTICES[3 * i] - POINT[0]) *
            (VERTICES[3 * i] - POINT[0]) +
            (VERTICES[3 * i + 1] - POINT[1]) *
            (VERTICES[3 * i + 1] - POINT[1]));

        spokes[2 * i] = VERTICES[3 * i] * t + POINT[0] * (1 - t);
        spokes[2 * i + 1] = VERTICES[3 * i + 1] * t + POINT[1] * (1 - t);

        if (i)
        {           
            sum_angles += cross(spokes, i - 1, i) * std::acos(dot(spokes, i - 1, i) /
                std::sqrtf(dot(spokes, i - 1, i - 1)) /
                std::sqrtf(dot(spokes, i, i)));
        }
    }

    sum_angles += cross(spokes, CURRENT_INDEX - 1, 0) * std::acos(dot(spokes, CURRENT_INDEX - 1, 0) /
        std::sqrtf(dot(spokes, CURRENT_INDEX - 1, CURRENT_INDEX - 1)) /
        std::sqrtf(dot(spokes, 0, 0)));

    isCalculate = GL_TRUE;

    return fabs(fabs(sum_angles) - 2 * PI) < PRECISION ? GL_TRUE : 
        fabs(fabs(sum_angles) - PI) < PRECISION ? GL_TRUE : GL_FALSE;
}