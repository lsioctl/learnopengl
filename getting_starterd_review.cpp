#include <vector>
#include <iostream>
#include <math.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "CubeWoodSmileMesh.h"

// Global variables
// delta_time
float delta_time = 0.0f;	// Time between current frame and last frame
float last_frame_time = 0.0f; // Time of last frame

// Camera global object
Camera camera{};

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.updatePosition(Camera::Movement::Front, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.updatePosition(Camera::Movement::Back, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.updatePosition(Camera::Movement::Left, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.updatePosition(Camera::Movement::Right, delta_time);
    }
}

void mouseCallback(GLFWwindow* window, double x_pos, double y_pos) {
    camera.updateOrientation(x_pos, y_pos);
}

void printNVertexAttribute()
{
    int nVertexAttributes{0};
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nVertexAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nVertexAttributes << std::endl;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // First two parametres set the location of the lower left corner
    // of the window.
    // Note2: viewport could be smaller than window, so we could render
    // things outside the viewport
    // OpenGL use the view port to translate it's 2D coordinates
    // to coordinate on the window. eg (-0.5, 0.5) => (200, 450)
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    // OpenGL stores all its depth inforamtion in z-buffer
    // glfw creates this buffer automatically for us (same as color buffer for the colors
    // of the output image)
    // depth is stored within each fragment (its z value)
    // and when a fragment want to output its color, it compare its depth value
    // with the z-buffer, if its behind and discards or overwrite the fragment accordingly
    // this process is called depth-testing
    // it is disabled by default in OpenGL, so enable it
    glEnable(GL_DEPTH_TEST);

    std::vector<glm::vec3> cube_position_list{
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    std::size_t Ncube{cube_position_list.size()};

    std::vector<CubeWoodSmileMesh> cube_list;
    cube_list.reserve(Ncube);

    // Create the cube meshes
    for (std::size_t i = 0; i < Ncube; i++) {
        // emplace back with temporary object is identical to push_back
        // so we use explicitely push_back
        // see: https://andreasfertig.blog/2023/04/push_back-vs-emplace_back-when-to-use-what/
        cube_list.push_back(CubeWoodSmileMesh{});
    }

    // TODO: harcoded relative path
    auto main_shader{ShaderProgram{"./shaders/world_coo1_vtx.glsl", "./shaders/world_coo1_frag.glsl"}};
    auto main_shader_id{main_shader.id};

    // Projection matrix
    // we want a standard perspective
    glm::mat4 projection_matrix{};
    // glm::perspective creates a 'frustrum' that define visible space
    // each coordinate inside this box will be mapped to a point in the clip space
    // the others not (they will be 'clipped')
    projection_matrix = glm::perspective(
        glm::radians(45.0f), // field of view
        800.0f / 600.0f, // aspect ratio, dividing the viewport width by its height
        0.1f, // near distance
        100.0f // far distance
    );

    const std::string model_matrix_uniform_name{"model_matrix"};
    const std::string view_matrix_uniform_name{"view_matrix"};

    // The projection matrix value does not change per frame, so we can set its value here
    //glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    main_shader.setMat4("projection_matrix", projection_matrix);

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // delta_time
        float current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        processInput(window);

        // rendering commands here
        // state-setting function
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // state-using function
        glClear(GL_COLOR_BUFFER_BIT);
        // clear the previous frame z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // View matrix
        // transform from world coordinate to 'camera' coordinates
        auto view_matrix = camera.getUpdatedViewMatrix();

        // send the view_matrix to the shader with the camera position updated
        main_shader.setMat4(view_matrix_uniform_name, view_matrix);

        

        for (std::size_t i = 0; i < Ncube; i++) {
            // Model matrix
            // Used to transform local (object coordinates) to world coordinates
            // always start with identity
            glm::mat4 model_matrix{glm::mat4(1.0f)};
            model_matrix = glm::translate(model_matrix, cube_position_list[i]);
            float angle{20.0f * i}; 
            model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            
            // set the model in the shader
            main_shader.setMat4(model_matrix_uniform_name, model_matrix);

            // draw the curent cube
            auto& current_cube = cube_list[i];

            current_cube.draw(main_shader);
        }


        // swap buffer and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();

    return 0;
}