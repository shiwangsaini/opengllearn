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
    std::vector<float> cude{
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 0,   //0
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 0,   //1
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 1,   //2
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 1,   //3

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 0,   //4
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 0,   //5
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 1,   //6
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 1,   //7

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 0,   //8
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 0,   //9
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 1,   //10
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 1,   //11

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0, 0,   //12
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1, 0,   //13
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1, 1,   //14
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0, 1,   //15 

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0, 0,   //16
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1, 0,   //17
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1, 1,   //18
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0, 1,   //19

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0, 0,   //20
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1, 0,   //21
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1, 1,   //22
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0, 1,   //23
    };

    std::vector<unsigned> indicies{
        // front
        0, 1, 2,
        2, 3, 0,

        // back
        4, 5, 6,
        6, 7, 4,

        // left
        8, 9, 10,
        10, 11, 8,

        // right
        12, 13, 14,
        14, 15, 12,

        // bottom
        16, 17, 18,
        18, 19, 16,

        // top
        20, 21, 22,
        22, 23, 20
    };

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Gui gui(m_window);
    InputHandler inHandler({ m_window, m_Shaders });

    // test
    glEnable(GL_DEPTH_TEST);

    float prevT = 0.0f;
    ImGuiIO& io = ImGui::GetIO();

    //Buffers::Vertex VB;
    //VB.AddDrawData(cude, GL_STATIC_DRAW);
    //TextureManager texture;
    //texture.CreateTexture("Core/Assets/container2.png", "container");
    //texture.CreateTexture("Core/Assets/container2_specular.png", "constainer_specular");
    //m_Shaders.use(ShaderType::OBJECT);
    //m_Shaders.setUniform1Int("material.diffuse", 0, ShaderType::OBJECT);
    //m_Shaders.setUniform1Int("material.specular", 1, ShaderType::OBJECT);
    //Buffers::Layout layout;
    //layout.Push<float>(3);
    //layout.Push<float>(3);
    //layout.Push<float>(2);
    //VB.AddVertex(layout);

    //Buffers::Index IB;
    //IB.AddIndex(indicies);
    //VB.Bind();
    //IB.bind();
    //texture.bind();

    /* Loop until the user closes the window */
    Dungeon dungeon{inHandler};
    bool playMode = false;
    float dt, current;

    //Model::Manager backpack;
    //backpack.LoadModel("Core/Assets/untitled.obj");

    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        gui.CreateFrame();

        current = (float)glfwGetTime();
        dt = (current - prevT);
        prevT = current;

        if (!io.WantCaptureKeyboard || !io.WantCaptureMouse)
        {
            if (!dungeon.GetPlayMode()) {
                inHandler.keyHandle(dt);
                inHandler.scrollHandle();
            }
            else {
                dungeon.keyHandle(dt);
            }
            inHandler.cameraHandle(dt);
        }

        dungeon.Render(dt);
        
        gui.Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);
    }
}

void GameWindow::shutdown()
{
    glfwTerminate();
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


    m_Shaders.AddShader("Core/Shaders/objVer1.shader", "Core/Shaders/objFrag1.shader");
    m_Shaders.AddShader("Core/Shaders/lightVer.shader", "Core/Shaders/lightFrag.shader");
    m_Shaders.AddShader("Core/Shaders/EnemyVer.shader", "Core/Shaders/EnemyFrag.shader");
    //m_Shaders.push_back(Shader("Core/Shaders/EnemyVer.shader", "Core/Shaders/EnemyFrag.shader"));
}

