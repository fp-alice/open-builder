#include <catch2/catch.hpp>

#include <client/gui/overlay.h>
#include <client/lua/client_lua_api.h>
#include <common/scripting/script_engine.h>

const std::string guiCreateScript = R"(

    function create(overlay)
        local image = overlay:addImage()
        image.image = 52
    end

    game.gui.addGui{
        id = "test_gui",
        title = "Test GUI",
        create = create
    }

    game.gui.push("test_gui")
)";

TEST_CASE("GUI API Tests")
{
    ScriptEngine scriptEngine;
    gui::OverlayFactory overlayFactory;
    gui::OverlayStack overlayStack;
    luaInitGuiApi(scriptEngine, overlayFactory, overlayStack);
    luaInitGuiWidgetApi(scriptEngine);

    SECTION("GUIs can have widgets added to them by Lua scripts")
    {
        scriptEngine.runLuaString(guiCreateScript);

        auto& overlay = overlayStack.overlays.back();
        REQUIRE(overlay->rectangleComponents.size() == 1);
        REQUIRE(overlay->widgetCount() == 1);
    }
}