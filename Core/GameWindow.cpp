#include "GameWindow.h"

#include <gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// resizeing handle
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "width" << width << "height" << height << '\n';
}

// Run main game loop
void GameWindow::run()
{
    shader.bind();
    shader.setUniform1i("texture1", 0);
    //shader.setUniform1i("texture2", 1);

    glfwSwapInterval(1);
    
    ImGui::CreateContext();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glm::vec3 transA(100.0f,100.0f,0.0f);
    glm::vec3 transB(200.0f,200.0f,0.0f);
    bool increment = true;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        
        texture.bind();

        /* Poll for and process events */
        /* Render here */
        {
            glm::mat4 proj = glm::ortho(0.0f, float(m_width), 0.0f, (float)m_height, -1.0f, 1.0f);
            glm::mat4 view = glm::translate(glm::mat4(1.0f),transA);
            glm::mat4 model = glm::rotate(view, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

            glm::mat4 MVP = (proj * view * model);
            shader.setUniformMat4f("u_MVP", MVP);

            shader.bind();
            VertexBuffer.bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        {
            glm::mat4 proj = glm::ortho(0.0f, float(m_width), 0.0f, (float)m_height, -1.0f, 1.0f);
            glm::mat4 view = glm::translate(glm::mat4(1.0f), transB);
            glm::mat4 model = glm::rotate(view, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

            glm::mat4 MVP = (proj * view * model);
            shader.setUniformMat4f("u_MVP", MVP);

            shader.bind();
            VertexBuffer.bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        ImGui::Begin("Hello, world!");
        ImGui::SliderFloat3("Img 1", &transA.x, 0.0f, 980.0f);
        ImGui::SliderFloat3("Img 2", &transB.x, 0.0f, 980.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        glfwPollEvents();
    }
}

void GameWindow::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    shader.unbind();
    texture.unbind();
    glfwTerminate();
}

bool GameWindow::init()
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    /* Create a windowed mode window and its OpenGL context */
    if (!m_window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);
    m_inputHandler = InputHandler(m_window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        // Initialization failed
        return false;
    }

    VertexBuffer.bind();

    shader.init("Core/Shaders/vertex.shader", "Core/Shaders/fragment.shader");
    texture = Texture(shader.m_RenderID);
    texture.CreateTexture("Core/Assets/wall.jpg");
    //texture.CreateTexture("Core/Assets/mask.jpg");
    return true;
}
