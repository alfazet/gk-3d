#include "app.h"

void framebufferSizeCallback(GLFWwindow* window, int nWidth, int nHeight)
{
    App& app = *static_cast<App*>(glfwGetWindowUserPointer(window));
    app.width = nWidth;
    app.height = nHeight;
}

void init(uint& vao, uint& vboPos, uint& vboColor, uint& ebo)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboPos);
    glGenBuffers(1, &vboColor);
    glGenBuffers(1, &ebo);

    // float pos[] = {
    //     1.0f, 1.0f, 1.0f,
    //     -1.0f, -1.0f, 1.0f,
    //     -1.0f, 1.0f, -1.0f,
    //     1.0f, -1.0f, -1.0f,
    // };
    // float color[] = {
    //     1.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f,
    //     0.0f, 0.0f, 1.0f,
    //     0.5f, 0.0f, 0.5f,
    // };
    // uint indices[] = {
    //     0, 1, 2,
    //     0, 3, 1,
    //     0, 2, 3,
    //     1, 3, 2,
    // };

    float pos[] = {
        -0.5f, -0.5f, -5.0f,
        0.5f, -0.5f, -5.0f,
        0.5f, 0.5f, -5.0f,
        -0.5f, 0.5f, -5.0f,
    };
    float color[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f,
    };
    uint indices[] = {
        0, 1, 2,
        2, 0, 3,
    };

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex attribute 0 (positions)
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(float), pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // vertex attribute 1 (colors)
    glBindBuffer(GL_ARRAY_BUFFER, vboColor);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(float), color, GL_STATIC_DRAW);
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
    glfwSwapInterval(0);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        ERR_AND_DIE("gladLoadGL");
    }
    const std::string vertShaderPath = std::format("{}/shaders/basic.vert", PROJECT_DIR);
    const std::string fragShaderPath = std::format("{}/shaders/basic.frag", PROJECT_DIR);
    this->shaders = new Shaders(vertShaderPath, fragShaderPath);
    glUseProgram(this->shaders->programId);
    glViewport(0, 0, this->width, this->height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    init(this->m_vao, this->m_vboPos, this->m_vboColor, this->m_ebo);

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

void App::run() const
{
    while (!glfwWindowShouldClose(this->window))
    {
        float fov = glm::radians(30.0f);
        float aspect = static_cast<float>(this->width) / this->height;
        mat4 proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, glm::tan(fov / 2.0f), 100.0f);
        mat4 view = glm::translate(mat4(1.0f), vec3(0.0f));
        mat4 model = mat4(1.0f);
        this->shaders->setUniformMat4("proj", proj);
        this->shaders->setUniformMat4("view", view);
        this->shaders->setUniformMat4("model", model);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(this->m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
}