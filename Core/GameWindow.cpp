#include "GameWindow.h"

#include "Gui.h"
#include "BPS/Dungeon.h"

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

// Run main game loop
void GameWindow::run()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Gui gui(m_window);
    InputHandler inHandler({ m_window, m_Shaders });

    // test
    glEnable(GL_DEPTH_TEST);

    float prevT = 0.0f;
    ImGuiIO& io = ImGui::GetIO();

    /* Loop until the user closes the window */

    Dungeon dungeon{&inHandler};


    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        gui.CreateFrame();

        //texture.bind();
        /* Poll for and process events */
        /* Render here */
        ImGui::Begin("Hello, world!");
        //ImGui::SliderFloat3("Img 1", &transA.x, -10.0f, 10.0f);
        //ImGui::SliderFloat3("light pos:", &cubePositions.x, -10.0f, 10.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();


        float current = (float)glfwGetTime();
        float dt = (current - prevT);
        prevT = current;

        if (!io.WantCaptureKeyboard)
        {
            inHandler.keyHandle(dt);
        }

        if (!io.WantCaptureMouse)
        {
            inHandler.cameraHandle(dt);
            inHandler.scrollHandle();
            //inHandler.mouseHandle(dt, cameraPos.x, cameraPos.y, cameraPos.z,m_width, m_height);
        }

        dungeon.Render();
        
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
    /*for (int i = 0; i <= 1; i++) {
        inHandler.updateMVP(cubePositions[i]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }*/
}

GameWindow::GameWindow()
    : m_width(720), m_height(480), m_title("No Title")
{
}

GameWindow::GameWindow(int width, int height, const char* title)
    : m_width(width), m_height(height), m_title(title)
{
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

    std::cout << "GPU: "
        << glGetString(GL_RENDERER)
        << '\n';

    std::cout << "Vendor: "
        << glGetString(GL_VENDOR)
        << '\n';

    std::cout << "OpenGL: "
        << glGetString(GL_VERSION)
        << '\n';

    //shader.init("Core/Shaders/vertex.shader", "Core/Shaders/fragment.shader");

    m_Shaders.push_back(Shader("Core/Shaders/objVer.shader", "Core/Shaders/objFrag.shader"));
    m_Shaders.push_back(Shader("Core/Shaders/lightVer.shader", "Core/Shaders/lightFrag.shader"));
    //texture = Texture(shader.getId());
    //texture.CreateTexture("Core/Assets/wall.jpg");
}

