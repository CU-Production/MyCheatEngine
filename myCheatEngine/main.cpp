#define SOKOL_IMPL
#define SOKOL_NO_ENTRY
#if defined WIN32 || defined _WIN32 || defined __WIN32__ || defined __NT__
#define SOKOL_D3D11
#else
#define SOKOL_GLCORE
#endif
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#include "imgui.h"
#include "util/sokol_imgui.h"

#include "Program.h"
#include "SystemFunctions.h"

constexpr uint32_t SCREEN_WIDTH = 800;
constexpr uint32_t SCREEN_HEIGHT = 600;

struct {
    struct {
        sg_pipeline pip;
        sg_pass_action pass_action;
    } graphics;
} state;

void init() {
    sg_desc _sg_desc{};
    _sg_desc.environment = sglue_environment();
    _sg_desc.logger.func = slog_func;
    sg_setup(&_sg_desc);

    simgui_desc_t _simgui_desc{};
    _simgui_desc.logger.func = slog_func;
    simgui_setup(&_simgui_desc);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
 }

static bool show_test_window = true;
static bool show_another_window = false;
void frame() {
    const double dt = sapp_frame_duration();

    const int width = sapp_width();
    const int height = sapp_height();
    simgui_new_frame({ width, height, sapp_frame_duration(), sapp_dpi_scale() });

    // imgui
    {
        // ImGui::ShowDemoWindow();

        ImVec2 vWindowSize = ImGui::GetMainViewport()->Size;
        ImVec2 vPos0 = ImGui::GetMainViewport()->Pos;
        ImGui::SetNextWindowPos(ImVec2((float)vPos0.x, (float)vPos0.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2((float)vWindowSize.x, (float)vWindowSize.y), 0);
        if (ImGui::Begin(
                "Editor",
                /*p_open=*/nullptr,
                ImGuiWindowFlags_MenuBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoTitleBar))
        {

            static const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
            ImGuiID dockSpace = ImGui::GetID("MainWindowDockspace");
            ImGui::DockSpace(dockSpace, ImVec2(0.0f, 0.0f), dockspaceFlags);
            static char name[256] = {};

            static bool openWindow=1;

            //static OppenedProgram oppenedProgram;
            static OpenProgram openProgram;
            static std::vector<OppenedProgram> programsOppened;

            if (ImGui::BeginMenuBar())
            {

                if (ImGui::BeginMenu("Open..."))
                {

                    openProgram.render();

                    openProgram.fileOpenLog.renderText();

                    ImGui::EndMenu();
                }


                ImGui::EndMenuBar();
            }


            if (openProgram.pid)
            {
                //first check if already oopened
                auto found = std::find_if(programsOppened.begin(), programsOppened.end(), [pid = openProgram.pid](const OppenedProgram& p)
                {
                    return p.pid == pid;
                });

                if (found == programsOppened.end())
                {
                    OppenedProgram newProgram;
                    newProgram.isOppened = true;

                    newProgram.pid = openProgram.pid;
                    openProgram.pid = 0;

                    newProgram.handleToProcess = openProgram.handleToProcess;
                    openProgram.handleToProcess = 0;

                    strcpy(newProgram.currentPocessName, openProgram.currentPocessName);
                    openProgram.currentPocessName[0] = 0;

                    programsOppened.push_back(newProgram);
                }
                else
                {
                    openProgram.fileOpenLog.setError("Process already oppened.", ErrorLog::ErrorType::info);
                    closeProcess(openProgram.handleToProcess);
                    openProgram.pid = 0;
                    openProgram.currentPocessName[0] = 0;
                    openProgram.handleToProcess = 0;

                }

            }

            //if (oppenedProgram.pid)
            for(int i=0; i<programsOppened.size(); i++)
            {
                auto& program = programsOppened[i];

                if (!program.isAlive())
                {
                    //openProgram.fileOpenLog.setError("process closed", openProgram.fileOpenLog.info);
                    program.writeLog.clearError();

                    program.close();
                    program.errorLog.setError("process closed", openProgram.fileOpenLog.info);
                }

                if(!program.render())
                {
                    program.close();
                    programsOppened.erase(programsOppened.begin() + i);
                    i--;
                }
            }

            ImGui::End();
        }
    }

    // graphics pass
    sg_pass _graphics_pass = { .action=state.graphics.pass_action, .swapchain=sglue_swapchain() };
    sg_begin_pass(&_graphics_pass);
    simgui_render();
    sg_end_pass();
    sg_commit();
}

void cleanup() {
    simgui_shutdown();
    sg_shutdown();
}

void input(const sapp_event* event) {
    simgui_handle_event(event);
}

int main() {
    sapp_desc desc = {0};
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup,
    desc.event_cb = input,
    desc.width  = SCREEN_WIDTH,
    desc.height = SCREEN_HEIGHT,
    desc.window_title = "My Cheat Engine",
    desc.icon.sokol_default = true,
    desc.logger.func = slog_func;
    sapp_run(&desc);

    return 0;
}
