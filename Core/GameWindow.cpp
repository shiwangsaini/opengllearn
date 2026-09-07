#include "GameWindow.h"

#include "Gui.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window,
        &framebufferWidth,
        &framebufferHeight);

    std::cout
        << "Window: "
        << windowWidth << " x " << windowHeight
        << "\nFramebuffer: "
        << framebufferWidth << " x " << framebufferHeight
        << '\n';

}

glm::vec3 transA(0.0f, 0.0f, -3.0f);
glm::vec3 transB(0.0f, 0.0f, -2.0f);
float rad = 0.0f;
float Rot[3]{ 1.0f,0.0f,0.0f };
float prevT = 0.0f;
float angle = 20.0f;

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -5.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Run main game loop
void GameWindow::run()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    shader.bind();
    shader.setUniform1i("texture1", 0);
    //shader.setUniform1i("texture2", 1);

    glfwSwapInterval(1);

    Gui gui(m_window);
    inHandler = InputHandler(m_window, &shader);

    // test
    glEnable(GL_DEPTH_TEST);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glfwPollEvents();
        gui.CreateFrame();

        texture.bind();
        /* Poll for and process events */
        /* Render here */
        ImGui::Begin("Hello, world!");
        //ImGui::SliderFloat3("Img 1", &transA.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Img 1", &Rot[0], -10.0f, 10.0f);
        ImGui::SliderFloat("Img 1", &rad, -180.0f, 180.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGuiIO& io = ImGui::GetIO();

        float current = (float)glfwGetTime();
        float dt = (current - prevT);
        prevT = current;

        if (!io.WantCaptureKeyboard)
        {
            inHandler.keyHandle(dt);
        }

        if (!io.WantCaptureMouse)
        {
            //inHandler.mouseHandle(dt, cameraPos.x, cameraPos.y, cameraPos.z,m_width, m_height);
        }
        inHandler.cameraHandle(dt);
        inHandler.scrollHandle();

        Draw();
       
        gui.Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

    }
}

void GameWindow::shutdown()
{
    texture.unbind();
    glfwTerminate();
}

void GameWindow::Draw()
{
    inHandler.updateMVP(cubePositions[0]);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    
}

void GameWindow::init()
{
    /* Initialize the library */
    if (!glfwInit())
        throw "Bro gets the L GLFW just dont want to look at you!!";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    /* Create a windowed mode window and its OpenGL context */
    if (!m_window)
    {
        glfwTerminate();
        throw "Bro gets the L window is useless just like you!!";
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        // Initialization failed
        throw "Bro gets the L Glad wont load!!";
    }

    shader.init("Core/Shaders/vertex.shader", "Core/Shaders/fragment.shader");
    texture = Texture(shader.getId());
    texture.CreateTexture("Core/Assets/wall.jpg");

}

