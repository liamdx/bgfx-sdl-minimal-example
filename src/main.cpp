#include <iostream>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL.h>
#include <SDL_syswm.h>

SDL_Window* window = nullptr;
const int WIDTH = 1024;
const int HEIGHT = 600;

int main(int argc, char* args[]) {

    // Initialize SDL systems
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error : " << SDL_GetError();
    }
    else {
        //Create a window
        window = SDL_CreateWindow("BGFX + SDL + CMake Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

        if (window == nullptr) {
            std::cout << "SDL Window could not be created! : " << SDL_GetError();
        }
    }

    // Collect information about the window from SDL
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi)) {
        return 1;
    }

    void* native_window_handle = nullptr;

#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
    wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
    if (!win_impl)
    {
        int width, height;
        SDL_GetWindowSize(_window, &width, &height);
        struct wl_surface* surface = wmi.info.wl.surface;
        if (!surface)
            native_window_handle = nullptr;
        win_impl = wl_egl_window_create(surface, width, height);
        SDL_SetWindowData(_window, "wl_egl_window", win_impl);
    }
    native_window_handle = (void*)(uintptr_t)win_impl;
#		else
    native_window_handle = (void*)wmi.info.x11.window;
#		endif
#	elif BX_PLATFORM_OSX || BX_PLATFORM_IOS
    native_window_handle =  wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
    native_window_handle = wmi.info.win.window;
#   elif BX_PLATFORM_ANDROID
    native_window_handle =  wmi.info.android.window;
#	endif // BX_PLATFORM_

    bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
    pd.ndt = wmi.info.wl.display;
#		else
    pd.ndt = wmi.info.x11.display;
#		endif
#	else
    pd.ndt = NULL;
#	endif // BX_PLATFORM_
    pd.nwh = native_window_handle;

    pd.context = NULL;
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);

    // Render an empty frame
    bgfx::renderFrame();

    // Initialize bgfx
    bgfx::init();

    // Reset window
    bgfx::reset(WIDTH, HEIGHT, BGFX_RESET_VSYNC);

    // Enable debug text.
    bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

    // Set view rectangle for 0th view
    bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));

    // Clear the view rect
    bgfx::setViewClear(0,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0x443355FF, 1.0f, 0);

    // Set empty primitive on screen
    bgfx::touch(0);

    // Poll for events and wait till user closes window
    bool quit = false;
    SDL_Event currentEvent;
    while (!quit) {
        bgfx::frame();
        while (SDL_PollEvent(&currentEvent) != 0) {
            if (currentEvent.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Free up window
    SDL_DestroyWindow(window);
    // close BGFX
    bgfx::shutdown();
    // Shutdown SDL
    SDL_Quit();

    return 0;
}