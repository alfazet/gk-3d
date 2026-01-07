#include "app.h"

void framebufferSizeCallback(GLFWwindow* window, int nWidth, int nHeight)
{
    App& app = *static_cast<App*>(glfwGetWindowUserPointer(window));
    app.width = nWidth;
    app.height = nHeight;
    glViewport(0, 0, app.width, app.height);
}

void initGLBuffers(uint& vao, uint& vboPos, uint& vboColor, uint& ebo)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboPos);
    glGenBuffers(1, &vboColor);
    glGenBuffers(1, &ebo);

    float pos[] = {
        1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
    };
    float color[] = {
        0.5f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.5f, 0.0f,
    };
    uint indices[] = {
        0, 1, 2,
        0, 3, 1,
        0, 2, 3,
        1, 3, 2,
    };

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex attribute 0 (positions)
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glBufferData(GL_ARRAY_BUFFER, N_FACES * 3 * sizeof(float), pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // vertex attribute 1 (colors)
    glBindBuffer(GL_ARRAY_BUFFER, vboColor);
    glBufferData(GL_ARRAY_BUFFER, N_FACES * 3 * sizeof(float), color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
}

App::App(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->width = WIN_WIDTH;
    this->height = WIN_HEIGHT;
    this->window = glfwCreateWindow(this->width, this->height, "gk-3d", nullptr, nullptr);
    if (this->window == nullptr)
    {
        ERR_AND_DIE("glfwCreateWindow");
    }
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(VSYNC);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        ERR_AND_DIE("gladLoadGL");
    }
    const std::string vertShaderPath = std::format("{}/shaders/basic.vert", PROJECT_DIR);
    const std::string fragShaderPath = std::format("{}/shaders/basic.frag", PROJECT_DIR);
    this->shaders = Shaders(vertShaderPath, fragShaderPath);
    glUseProgram(this->shaders.programId);
    glViewport(0, 0, this->width, this->height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    initGLBuffers(this->m_vao, this->m_vboPos, this->m_vboColor, this->m_ebo);

    this->camera = Camera(vec3(2.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init();

    glfwSetWindowUserPointer(this->window, this);
    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);
}

App::~App()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void App::run()
{
    float fov = glm::radians(120.0f);
    float aspect = static_cast<float>(this->width) / this->height;
    mat4 model = mat4(1.0f);
    mat4 view = this->camera.view();
    mat4 proj = glm::perspective(fov, aspect, 0.1f, 100.0f);
    this->shaders.setUniformMat4("proj", proj);
    this->shaders.setUniformMat4("view", view);
    constexpr vec3 rotationAxis = vec3(0.0f, 1.0f, 0.0f);

    int fps = 0, framesThisSecond = 0;
    auto prevTime = static_cast<float>(glfwGetTime()), prevSeconds = 0.0f;
    while (!glfwWindowShouldClose(this->window))
    {
        auto curTime = static_cast<float>(glfwGetTime());
        this->m_dt = curTime - prevTime;
        prevTime = curTime;
        framesThisSecond++;
        if (curTime - prevSeconds >= 1.0f)
        {
            fps = framesThisSecond;
            framesThisSecond = 0;
            prevSeconds += 1.0f;
        }

        model = glm::rotate(model, this->m_dt, rotationAxis);
        this->shaders.setUniformMat4("model", model);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(this->m_vao);
        glDrawElements(GL_TRIANGLES, N_FACES * 3, GL_UNSIGNED_INT, nullptr);

        this->handleKeys();
        this->renderImguiFrame(fps);
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
}

void App::renderImguiFrame(int fps) const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(75, 25), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (!ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoDecoration))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("%d FPS", fps);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::handleKeys() const
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(this->window, true);
    }
}