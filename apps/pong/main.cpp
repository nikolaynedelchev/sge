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

void PrintAllGlobals(lua_State *L)
{
    lua_pushglobaltable(L);

        // Итерация през _G
        lua_pushnil(L); // Първоначално nil за началото на таблицата
        while (lua_next(L, -2) != 0) {
            // Ключът е на индекс -2, стойността е на индекс -1
            const char *key = lua_tostring(L, -2); // Ключът
            if (lua_isstring(L, -1)) {
                // Ако стойността е стринг
                printf("%s = %s\n", key, lua_tostring(L, -1));
            } else if (lua_isnumber(L, -1)) {
                // Ако стойността е число
                printf("%s = %g\n", key, lua_tonumber(L, -1));
            } else if (lua_istable(L, -1)) {
                // Ако стойността е таблица
                printf("%s = (table)\n", key);
            } else {
                // Други типове
                printf("%s = (unsupported type)\n", key);
            }

            // Премахваме стойността, за да продължим към следващия елемент
            lua_pop(L, 1);
        }

        // Премахваме таблицата _G от стека
        lua_pop(L, 1);
}

void TestLua()
{
    lua_State *L = luaL_newstate();
    if (L == nullptr) {
        fprintf(stderr, "Cannot create Lua state.\n");
        return;
    }

    // Зареждане на стандартните библиотеки на Lua
    luaL_openlibs(L);

    // Зареждане на Lua скрипт
    std::string luaScr = R"(
        my_test_string = "ehooo"
        function add(a, b)
            return a + b
        end
        my_test_num = 3.14
        my_test_table = {ehoo=2,type="test"}
    )";
    if (luaL_dostring(L, luaScr.c_str())) {
        fprintf(stderr, "Error loading Lua script: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return;
    }

    // Подготвяне за извикване на Lua функцията
    lua_getglobal(L, "add"); // Зарежда Lua функцията "add"
    if (!lua_isfunction(L, -1)) {
        fprintf(stderr, "'add' is not a valid function.\n");
        lua_close(L);
        return;
    }

    // Подаване на аргументи към функцията
    lua_pushnumber(L, 10); // Първи аргумент
    lua_pushnumber(L, 20); // Втори аргумент

    // Извикване на функцията (2 аргумента, 1 резултат)
    if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
        fprintf(stderr, "Error calling function: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return;
    }

    // Извличане на резултата
    if (lua_isnumber(L, -1)) {
        double result = lua_tonumber(L, -1);
        printf("Result: %f\n", result);
    } else {
        fprintf(stderr, "Unexpected return type.\n");
    }

    // Почистване на Lua стека
    lua_pop(L, 1);

    PrintAllGlobals(L);

    // Затваряне на Lua средата
    lua_close(L);
}

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

static std::string luaScr = R"(

SpriteSheet = ""

function Sprite(input)
    local sprite = {}
    sprite.resource_type = "sprite"
    sprite.x = input.x or 0
    sprite.y = input.y or 0
    sprite.w = input.w or 0
    sprite.h = input.h or 0
    sprite.scale = input.scale or 0.0
    sprite.rotation = input.rotation or 0.0
    sprite.file = input.fileName or SpriteSheet

    local pivot_offsets = {
            center = { 0.5, 0.5 },
            up = { 0.5, 0.0 },
            up_left = { 0.0, 0.0 },
            up_right = { 1.0, 0.0 },
            down = { 0.5, 1.0 },
            down_left = { 0.0, 1.0 },
            down_right = { 1.0, 1.0 },
            left = { 0.0, 0.5 },
            right = { 1.0, 0.5 }
        }

    if input.pivot and pivot_offsets[input.pivot] then
        local offset = pivot_offsets[input.pivot]
        sprite.pivotX = math.floor(sprite.w * offset[1])
        sprite.pivotY = math.floor(sprite.h * offset[2])
    else
        sprite.pivotX = input.pivotX or 0
        sprite.pivotY = input.pivotY or 0
    end

    return sprite
end

function Animation(input)
    local animation = {}
    animation.resource_type = "animation"
    animation.pivotX = input.pivotX or 0
    animation.pivotY = input.pivotY or 0
    animation.scale = input.scale or 0.0
    animation.rotation = input.rotation or 0.0
    animation.fps = input.fps or 0.0

    animation.frames = {}
    if input.frames then
        for i, sprite in ipairs(input.frames) do
            animation.frames[i] = Sprite(sprite)
        end
    end

    function animation:Frame(sprite)
        table.insert(self.frames, Sprite(sprite))
        return self
    end

    return animation
end

SpriteSheet = "./resources/pong.png"
PONG = Sprite({x=100,y=500,w=66,h=55, pivot="center"})
PONG_MOVE = Animation({pivotX=11, pivotY=15, fps=7.5})

)";

void GameLoop()
{
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
        if (ImGui::Button("Test button"))
        {
            rendImage = !rendImage;
        }

        if (rendImage)
        {
            Rectangle srcRect = {0, 0, float(target.texture.width), float(target.texture.height)};
            rlImGuiImageRect(&target.texture, target.texture.width, target.texture.height, srcRect);
        }

        te.Render("Editor title", {1200, 700}, true);
        //zepEdit.Display();

        if (ImGui::Button("Test button 2"))
        {
            auto resources = tools::rss_loader::LoadFromLuaGenerator(te.GetText());
            fmt::println("Resources:\n{}", resources);
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
    ndn::pong::TestLua();
    //return 0;

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
