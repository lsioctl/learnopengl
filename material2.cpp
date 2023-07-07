#include <vector>
#include <iostream>
#include <math.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "VAO2.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

#include <glm/gtx/string_cast.hpp>

// Global variables
// delta_time
float delta_time = 0.0f; // Time between current frame and last frame
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

void mouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    camera.updateOrientation(x_pos, y_pos);
}

void printNVertexAttribute()
{
    int nVertexAttributes{0};
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nVertexAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nVertexAttributes << std::endl;
}

int main()
{
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

    auto texture_wood{Texture{"./textures/container.jpg", GL_RGB}};
    auto texture_face{Texture{"./textures/awesomeface.png", GL_RGBA}};

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

    // vertices in normalized device coordinates (visible region of OpenGL)
    // We don't use EBO here, because of the texture coordinates (see world_coo2 result in this case)
    std::vector<float> cube_vertices{
        // positions          // normal vector
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    const auto N_cube_vertices{cube_vertices.size() / 6};

    // We do not use the custom and clumsy VAO2 class, has
    // we will have issues with strides and so on ..

    // Vertex Array Object
    // It will allow to store VBO and Vertex Attribute that
    // we will consequently declare
    // in Core OpenGL, VAO are mandatory
    GLuint cube_vao_id;
    glGenVertexArrays(1, &cube_vao_id);
    // once we bind it, all subsequent VBO and attributes
    // will be store on the VAO referenced by this id
    glBindVertexArray(cube_vao_id);

    // Vertex Buffer Object
    // will set up a memory on the graphic card
    // VBO will store its buffer id
    unsigned int cube_vbo;
    glGenBuffers(1, &cube_vbo);  
    // sGL_ARRAY_BUFFER is the type for a vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo); 
    // copy the vertex data into buffer memory for the currently
    // bound buffer (VBO)
    // the 4th parameters tells how we want the graphic card to manage the data
    // GL_STATIC_DRAW means the data is set up once and could be used many times
    // (so a memory region where read is fast could be good)
    // another example is GL_DYNAMIC_DRAW, which means thata data can change a lot
    // so it should be stored in a memory region where write (and read for usage) is fast
    // TODO: size dangerous if refactored: a wrong size will prevent any display
    glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(float), cube_vertices.data(), GL_STATIC_DRAW);

    // Now we have to have to tell OpenGL how to interpret the raw data
    // vertices
    glVertexAttribPointer(
        0, // location of the vertex attribute, will be used in vertex shader with layout (location = 0)
        3, // vertex attribue is a vec3, 3 elements
        GL_FLOAT, // element of the vec3 is float
        GL_FALSE, // don't normalize data
        6 * sizeof(float), // size of a stride is 6 float elements
        (void*)0 // offset is 0
    );
    // normals
    glVertexAttribPointer(
        1, // location of the normal attribute, will be used in vertex shader with layout (location = 1)
        3, // normal attribue is a vec3, 3 elements
        GL_FLOAT, // element of the vec3 is float
        GL_FALSE, // don't normalize data
        6 * sizeof(float), // size of a stride is 6 float elements
        (void*)(3 * sizeof(float)) // offset
    );

    // vertex attributes are disabled by default, so enable the one we created
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // textures
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);


    // We prepare another VAO has the light source will be always the same
    // but the cube will be enhanced (textures, ...)
    // Vertex Array Object
    // It will allow to store VBO and Vertex Attribute that
    // we will consequently declare
    // in Core OpenGL, VAO are mandatory
    GLuint light_source_vao_id;
    glGenVertexArrays(1, &light_source_vao_id);
    // once we bind it, all subsequent VBO and attributes
    // will be store on the VAO referenced by this id
    glBindVertexArray(light_source_vao_id);
    // We re-use the cube VBO, as it has already been loaded in GPU memory
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    // Now we have to have to tell OpenGL how to interpret the raw data
    // vertices
    glVertexAttribPointer(
        0, // location of the vertex attribute, will be used in vertex shader with layout (location = 0)
        3, // vertex attribue is a vec3, 3 elements
        GL_FLOAT, // element of the vec3 is float
        GL_FALSE, // don't normalize data
        6 * sizeof(float), // size of a stride is 6 float elements
        (void*)0 // offset is 0
    );
    // vertex attributes are disabled by default, so enable the one we created
    glEnableVertexAttribArray(0);

    // TODO: harcoded relative path
    auto lighting_cube_shader{ShaderProgram{"./shaders/lighting_cube_3_vtx.glsl", "./shaders/material_1_frag.glsl"}};
    auto lighting_cube_shader_id{lighting_cube_shader.id};

    auto lighting_source_shader{ShaderProgram{"./shaders/lighting_cube_1_vtx.glsl", "./shaders/lighting_source_1_frag.glsl"}};
    auto lighting_source_shader_id{lighting_source_shader.id};

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

    // activate the shader
    lighting_cube_shader.use();

    // Now shader is in use, we can set the uniforms
    lighting_cube_shader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    lighting_cube_shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    lighting_cube_shader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lighting_cube_shader.setFloat("material.shininess", 32.0f);

    const std::string model_matrix_uniform_name{"model_matrix"};
    const std::string view_matrix_uniform_name{"view_matrix"};

    // The projection matrix value does not change per frame, so we can set its value here
    //glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    lighting_cube_shader.setMat4("projection_matrix", projection_matrix);

    // activate the light source shader and set the uniforms
    lighting_source_shader.use();
    lighting_source_shader.setMat4("projection_matrix", projection_matrix);

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
        auto& view_matrix = camera.getUpdatedViewMatrix();

        // We will use the world coordinates for all the lighting
        // calculations, including specular lighting.
        // Most of the people use the view coordinates, because the camera
        // position there is always (0, 0, 0)
        auto& camera_position = camera.getPosition();

        // send the view_matrix to the shaders with the camera position updated
        lighting_cube_shader.use();
        lighting_cube_shader.setMat4(view_matrix_uniform_name, view_matrix);
        lighting_source_shader.use();
        lighting_source_shader.setMat4(view_matrix_uniform_name, view_matrix);

        // Model matrix
        // Used to transform local (object coordinates) to world coordinates
        // always start with identity
        glm::mat4 cube_model_matrix{glm::mat4(1.0f)};
        auto cube_position = glm::vec3(0.0f,  0.0f, 0.0f);
        cube_model_matrix = glm::translate(cube_model_matrix, cube_position);

        // Normal matrix
        // special matrix for normal vectors
        // it allow the normal to stay perpendicular
        // if the model is not scaled in a uniform maner
        // as inversion is pretty costly, and to avoid to
        // be computed for every vertex it should not be in
        // the shaders
        glm::mat3 cube_normal_matrix = glm::mat3(glm::transpose(glm::inverse(cube_model_matrix)));

        // std::cout << glm::to_string(cube_model_matrix) << std::endl;
        // std::cout << glm::to_string(cube_normal_matrix) << std::endl;
        //std::cout << glm::to_string(camera_position) << std::endl;

        glm::mat4 light_source_model_matrix{glm::mat4(1.0f)};
        auto light_source_position = glm::vec3(0.9f,  0.9f, 0.0f);
        light_source_model_matrix = glm::translate(light_source_model_matrix, light_source_position);
        light_source_model_matrix = glm::scale(light_source_model_matrix, glm::vec3(0.2f));

        // set the model in the shaders
        lighting_cube_shader.use();
        lighting_cube_shader.setMat4(model_matrix_uniform_name, cube_model_matrix);
        lighting_cube_shader.setMat3("normal_matrix", cube_normal_matrix);
        lighting_cube_shader.setVec3("light.position", light_source_position);
        lighting_cube_shader.setVec3("camera_pos", camera_position);

        glm::vec3 light_color;
        light_color.x = sin(glfwGetTime() * 2.0f);
        light_color.y = sin(glfwGetTime() * 0.7f);
        light_color.z = sin(glfwGetTime() * 1.3f);

        lighting_cube_shader.setVec3("light.ambient", (0.2f * light_color));
        // darken diffuse light a bit
        lighting_cube_shader.setVec3("light.diffuse", (0.5f * light_color));
        // lighting_cube_shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lighting_cube_shader.setVec3("light.specular", light_color);

        // render the cube
        glBindVertexArray(cube_vao_id);
        glDrawArrays(
            GL_TRIANGLES,  // we want to draw triangles
            0,
            N_cube_vertices  // we want this number of vertices in total
        );

        lighting_source_shader.use();
        lighting_source_shader.setVec3("light_color", light_color);
        lighting_source_shader.setMat4(model_matrix_uniform_name, light_source_model_matrix);

        // render the light source
        glBindVertexArray(light_source_vao_id);
        glDrawArrays(
            GL_TRIANGLES,  // we want to draw triangles
            0,
            N_cube_vertices  // we want this number of vertices in total
        );

        // swap buffer and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();

    return 0;
}