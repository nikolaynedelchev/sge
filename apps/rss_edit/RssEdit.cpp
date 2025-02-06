#include <tools/tools.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <sge/sge.h>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <rlImGui/rlImGui.h>
#include <imgui/imgui.h>

extern "C" {
#include <lua/lua.h>

#include <lua/lauxlib.h>
#include <lua/lualib.h>
}


#include <tools/LuaRss.h>
#include "TextEditor/TextEditor.h"
#include "RssEdit.h"
#include "GuiTools.h"
#include <raymath.h>

namespace ndn::rssedit
{
static sge::engine engine;

static tools::rss_loader::Resources s_resources;
static std::string s_editorSelectedText;

void RssEdit::Loop()
{
    std::string luaScr = R"(
-- rss_key1 = Sprite({x=0,y=0,w=32,h=24,pivot="center",scale=1.0,rotation=0.0})
-- rss_key2 = Sprite({x=0,y=0,w=32,h=24,pivotX=16,pivotY=8,scale=1.0,rotation=0.0})
--
-- rss_key3 = Animation({fps=8.0,pivotX=16,pivotY=8,scale=1.0,rotation=0.0})
-- rss_key3:Frame({x=0,y=0,w=32,h=24,pivot="center",scale=1.0,rotation=0.0})

SpriteSheet = "full/filename"

)";

    m_textEditor.SetText(luaScr);
    while(!engine.should_close())
    {
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

        Draw();

        rlImGuiEnd();

        //DrawTexture(target.texture, 100, 100, RAYWHITE);

        engine.end_frame();
    }
}

int RssEdit::Run()
{
    try
    {
        engine.init("./", 1280, 1280);
        rlImGuiSetup(true);

        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = 1.5f;
        ImGui::GetStyle().ScaleAllSizes(1.5f);
    }
    catch (...)
    {
        rlImGuiShutdown();
        engine.close();
        return false;
    }

    m_textEditor.SetColorizerEnable(true);
    m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

    m_imgViewer.SetImage("/home/nikolay/tmp/test_png_transp.png");
    m_imgViewer.SetDecorator([](Vector2 zoom, Vector2 offset){

        for (const auto& p : s_resources.sprites)
        {
            const auto& key = p.first;
            const auto& sprite = p.second;
            auto pos = Vec(sprite.x, sprite.y) * zoom + offset;
            auto sz = Vec(sprite.w, sprite.h) * zoom;

            Rectangle r = Rect(pos, sz);

            if (key == s_editorSelectedText)
            {
                GuiTools::DrawBorders(r, 6, BLUE);
            }
            else
            {
                GuiTools::DrawBorders(r, 2, WHITE);
            }
        }

        Vector2 p1 = {10.0f, 15.0f}, p2 = Vector2{25.0f, 90.0f}, p3 = Vector2{85.0f, 40.0f};

        p1 = p1 * zoom + offset;
        p2 = p2 * zoom + offset;
        p3 = p3 * zoom + offset;

        DrawTriangle(p1, p2, p3, BLUE);
        DrawText("Hello from raylib!", 10 * zoom.x + offset.x, 10 * zoom.y + offset.y, 20 * zoom.x, DARKGRAY);
    });
    Loop();

    rlImGuiShutdown();
    engine.close();
    return 0;
}

void RssEdit::DrawTextEditor()
{
    ImGuiIO& io = ImGui::GetIO();

    // Проверка за Ctrl + S
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S))
    {
        s_resources = tools::rss_loader::LoadFromLuaGenerator(m_textEditor.GetText());
        fmt::println("Resources:\n{}", s_resources);
    }
    s_editorSelectedText = m_textEditor.GetSelectedText();

    if (ImGui::Button("Test button 2"))
    {
        s_resources = tools::rss_loader::LoadFromLuaGenerator(m_textEditor.GetText());
        fmt::println("Resources:\n{}", s_resources);
    }
    m_textEditor.Render("Editor title", {}, true);
}

void RssEdit::DrawRssBrowser()
{
    if (ImGui::Button("Rss browser"))
    {
    }
}

void RssEdit::DrawRssRend()
{
    m_imgViewer.Draw();
}

void RssEdit::Draw()
{
    ImGui::Begin("RssEdit", nullptr,       ImGuiWindowFlags_NoTitleBar
                                         | ImGuiWindowFlags_NoResize
                                         | ImGuiWindowFlags_NoMove
                                         | ImGuiWindowFlags_NoCollapse
                                         | ImGuiWindowFlags_NoScrollbar
                                         | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::BeginChild("Rss", ImVec2(0, 0), ImGuiChildFlags_ResizeY|ImGuiChildFlags_Borders);
    //

    if (ImGui::BeginTable("RssTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV))
    {
        ImGui::TableNextColumn();
        DrawRssRend();
        ImGui::TableNextColumn();
        DrawRssBrowser();
        ImGui::EndTable();
    }
    ImGui::EndChild();

    ImGui::BeginChild("TextEditor");
    //
    DrawTextEditor();
    ImGui::EndChild();

    ImGui::End();
}

}
