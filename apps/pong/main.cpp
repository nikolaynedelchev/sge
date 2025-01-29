#include "../../libs/tools/tools.h"
#include <raylib.h>
#include <tools/tools.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <entt/entt.hpp>
#include <sge/sge.h>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include "types.h"
#include <rlImGui/rlImGui.h>
#include <imgui/imgui.h>
//#include <zep/include/zep.h>
//#include <zep/imgui/editor_imgui.h>

extern "C" {
#include <lua/lua.h>

#include <lua/lauxlib.h>
#include <lua/lualib.h>
}


#include <tools/LuaRss.h>

#include "TextEditor/TextEditor.h"

namespace ndn::pong
{
static sge::engine engine;

bool Init()
{
    try {
        auto runtime = std::make_unique<tools::Runtime>();
        SwitchRuntimeContext(std::move(runtime));
        engine.init("./", 1280, 1280);
        rlImGuiSetup(true);

        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = 2.0f;
        ImGui::GetStyle().ScaleAllSizes(2.0f);
    } catch (...) {
        rlImGuiShutdown();
        engine.close();
        SwitchRuntimeContext(nullptr);
        return false;
    }
    return true;
}

void GameLoop()
{
    std::string luaScr = R"(
        SpriteSheet = "./resources/pong.png"
        PONG = Sprite({x=100,y=500,w=66,h=55, pivot="center"})
        PONG_MOVE = Animation({pivotX=11, pivotY=15, fps=7.5})
    )";
    //Zep::ZepEditor_ImGui zepEdit(".", {1, 1});
    TextEditor te;
    te.SetColorizerEnable(true);
    te.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    te.SetText(luaScr);
    bool rendImage = false;
    RenderTexture2D target = LoadRenderTexture(800, 600); // Размер на framebuffer
    while(!engine.should_close())
    {
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        DrawRectangle(10, 10, 600, 400, RED);
        DrawText("Hello from raylib!", 10, 10, 20, DARKGRAY);
        EndTextureMode();


        engine.begin_frame();
        engine.clear_frame(sge::colors::white);

        rlImGuiBegin();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar     // Без заглавна лента
                                        | ImGuiWindowFlags_NoResize       // Без възможност за преоразмеряване
                                        | ImGuiWindowFlags_NoMove         // Неподвижен прозорец
                                        | ImGuiWindowFlags_NoCollapse     // Без бутон за свиване
                                        | ImGuiWindowFlags_NoScrollbar    // Без скролбар
                                        | ImGuiWindowFlags_NoBringToFrontOnFocus; // Без фокус

        ImGui::SetNextWindowPos(ImVec2(0, 0)); // Начална позиция (горен ляв ъгъл)
        ImGui::SetNextWindowSize(ImVec2(GetScreenWidth(), GetScreenHeight())); // Размер на прозореца

        ImGui::Begin("Test Wind", nullptr, window_flags);

        if (ImGui::BeginTable("SplitterTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableNextColumn();
            if (ImGui::Button("Test button"))
            {
                rendImage = !rendImage;
            }

            if (rendImage)
            {
                Rectangle srcRect = {0, 0, float(target.texture.width), float(target.texture.height)};
                rlImGuiImageRect(&target.texture, target.texture.width, target.texture.height, srcRect);
            }

            ImGui::TableNextColumn();
            if (ImGui::Button("Test button 2"))
            {
                auto resources = tools::rss_loader::LoadFromLuaGenerator(te.GetText());
                fmt::println("Resources:\n{}", resources);
            }
            te.Render("Editor title", {}, true);
            //zepEdit.Display();
            ImGui::EndTable();
        }
        ImGui::End();
        rlImGuiEnd();

        //DrawTexture(target.texture, 100, 100, RAYWHITE);

        engine.end_frame();
    }
    UnloadRenderTexture(target);
}

int Shutdown()
{
    rlImGuiShutdown();
    engine.close();
    SwitchRuntimeContext(nullptr);
    return 0;
}

}

int main()
{
    if (ndn::pong::Init())
    {
        ndn::pong::GameLoop();
        return ndn::pong::Shutdown();
    }
    return -1;

    glm::vec2 v1 = {0.5, 1.4};
    glm::vec2 v2 = {12.5, 1.6};


    auto dist = glm::distance(v1, v2);
    ndn::sge::engine eng;
    eng.init("./", 320, 200);
    ndn::tools::test_me();
    ndn::tools::test_me2();
    entt::registry registry;
    auto entity = registry.create();

    struct test_comp{int a;};
    registry.emplace<test_comp>(entity);
    (void)registry;


    fmt::println("Closing...");

    eng.close();
    fmt::println("Goodbye...");

    return 0;
}
