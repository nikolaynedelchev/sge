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
#include <sge/image_mgr.h>

namespace ndn::rssedit
{
static sge::engine engine;

static tools::rss_loader::Resources s_resources;
static std::string s_editorSelectedText;
static std::shared_ptr<sge::ImageMgr> s_imgMgr;

//static std::string s_testImg="C:/Users/nikolay.nedelchev/Documents/CPP_Projects/assets/2d-shooter/New_All_Fire_Bullet_Pixel_16x1/All_Fire_Bullet_Pixel_16x16_01.png";
//static std::string s_testImg = "/home/nikolay/tmp/test_png_transp.png";
static std::string s_testImg = "/home/nikolay/Downloads/2dShooterAssets/Foozle_2DS0012_Void_EnemyFleet_1/Kla'ed/Weapons/PNGs/Kla'ed - Frigate - Weapons.png";
static std::string s_emptyBackground = "/home/nikolay/tmp/empty_background.png";
static tools::rss_loader::SpriteArray s_autoExtracted;

void RssEdit::Loop()
{
    std::string luaScr = R"(
-- rss_key1 = Sprite({x=0,y=0,w=32,h=24,pivot="center",scale=1.0,rotation=0.0})
-- rss_key2 = Sprite({x=0,y=0,w=32,h=24,pivotX=16,pivotY=8,scale=1.0,rotation=0.0})
--      pivot: center, up, up_left, up_right, down, down_left, down_right, left, right
--
-- rss_key3 = SpriteArray({})
-- rss_key3:Add({x=0,y=0,w=32,h=24,pivot="center",scale=1.0,rotation=0.0})
-- rss_key3:Add({x=0,y=0,w=32,h=24,pivotX=16,pivotY=8,scale=1.0,rotation=0.0})
--
-- rss_key4 = Animation({spriteArray="rss_key3", fps=8.0,pivotX=16,pivotY=8,scale=1.0,rotation=0.0})

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

    s_imgMgr = std::make_shared<sge::ImageMgr>();

    m_textEditor.SetColorizerEnable(true);
    m_textEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

    m_imgViewer.SetImageMgr(s_imgMgr);
    m_imgViewer.SetImage(s_testImg);
    m_imgViewer.SetDecorator([](Vector2 zoom, Vector2 offset){

        auto drawer = [zoom, offset](const tools::rss_loader::Sprite& sprite, const std::string& key)
        {
            auto pos = Vec(sprite.x, sprite.y) * zoom + offset;
            auto sz = Vec(sprite.w, sprite.h) * zoom;

            Rectangle r = Rect(pos, sz);

            if (key == s_editorSelectedText)
            {
                auto thick = 6.0f;
                if (zoom.x > 15.0f)
                {
                    thick = 12.0f;
                }
                GuiTools::DrawBorders(r, thick, BLUE);
            }
            else
            {
                GuiTools::DrawBorders(r, 2, WHITE);
            }
        };
        for (const auto& p : s_resources.sprites)
        {
            const auto& key = p.first;
            const auto& sprite = p.second;
            drawer(sprite, key);
        }

        for (const auto& sprite : s_autoExtracted)
        {
            drawer(sprite, "auto");
        }
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
    m_textEditor.Render("Editor title", {}, true);
}

void RssEdit::DrawRssBrowser()
{
    if (ImGui::Button("AutoExtract"))
    {
        auto pixels = s_imgMgr->Pixels(s_testImg);
        s_autoExtracted = tools::rss_loader::AutoSprites(pixels);
    }
    if(ImGui::Button("Generate Trnasparency"))
    {

        int width = 64;
        int height = 64;
        int blockSz = 32;
        Image image = GenImageColor(width, height, BLACK);
        ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        Color *pixels = (Color *)image.data;
        for (int y = 0; y < height; y++)
        {
            bool yOdd = (y / blockSz) % 2;
            bool y2Odd = (y % blockSz) % 2;
            for (int x = 0; x < width; x++)
            {
                bool xOdd = (x / blockSz) % 2;
                bool x2Odd = (x % blockSz) % 2;

                int index = y * width + x;
                Color c;
                c.a = 150;
                if (xOdd == yOdd)
                {
                    c.r = (x2Odd == y2Odd) ? 50 : 60;
                    c.g = (x2Odd == y2Odd) ? 30 : 40;
                    c.b = (x2Odd == y2Odd) ? 10 : 20;
                }
                else
                {
                    c.r = (x2Odd == y2Odd) ? 10 : 20;
                    c.g = (x2Odd == y2Odd) ? 30 : 40;
                    c.b = (x2Odd == y2Odd) ? 50 : 60;
                }
                pixels[index] = c;
            }
        }
        ExportImage(image, s_emptyBackground.c_str());
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
