# EVK
This repository represents my attempt to create a header-only implementation of a custom Vulkan renderer.

While compilable libraries is typically more straightforward, it's less than ideal for fast prototyping and code experiments. This project aims to provide a self-contained Vulkan interface that can be easily integrated into any project.

This implementation will exclusively target Vulkan for the foreseeable future, with no immediate plans for abstraction layers or cross-API macros. That being said, the Vulkan code will be contained in certain files only (_vulkan) so if another API gets supported it'll not be a nightmare to implement it.

The focusing will be on Vulkan 1.1, avoiding newer features like dynamic rendering to maintain compatibility with entry-level hardware, so 
for projects requiring advanced Vulkan features, more established frameworks might be more appropriate.

# Build
There's no build, no linking, no references and no dependency outside what's in here. It does rely on some libraries but they're also developed in the same way EVK is and are permissive to be included alongside EVK when given proper credits. The two-step to use the library is the following:

1) This is a "header-only" style API.
Include the "evk" root's subfolder into your project libraries path, don't compile any of the source files as they'll be auto-included when compiling happens.
2) Set ```#define EVK_IMPLEMENTATION``` before ```#include "evk.h"``` in exactly one source file.

There are some pre-configurations that can be requested on your project settings (CMake, Premake, Ninja, etc). They are the following:

* EVK_ENABLE_VALIDATIONS: Requests Vulkan logging feedback to be propagated into the terminal;
* EVK_LINUX_USE_XLIB: If under Linux, hints the use of XLIB instead of XCB/Wayland(default);
* EVK_LINUX_USE_XCB: If under Linux, hints the use of XCB instead of XLIB/Wayland(default);

Now you're good to and initialize and use the API.

# Example - Emtpy example
This example is just an introduction to the API. For a complete example, check [examples](examples) directory.
```c
void evk_render(evkContext* context, float timestep)
{
}

void evk_renderui(evkContext* context, void* cmdbuffer)
{
}

int main()
{
    // the first thing is the initial configuration for API startup
    evkCreateInfo info = { 0 };
    info.appName = "My app";
    info.appVersion = EVK_MAKE_VERSION(0, 1, 0, 0);
    info.engineName = "EVK";
    info.engineVersion = EVK_MAKE_VERSION(0, 0, 1, 0);
    info.width = 1366;
    info.height = 728;
    info.MSAA = evk_Msaa_X4;
    info.vsync = false;
    // a custom viewport can be requested to render the world into
    info.viewport = false; 
    // other platforms will have their own objects for the window
    info.window.window = g_HWND; // WIN32
    
    // now we may initialize the API
    evkResult res = evk_init(&info);

    // we must register callbacks to gain control when to render stuff
    evk_set_render_callback(on_render); // render the world
    evk_set_renderui_callback(on_renderui); // render the ui

    // since EVK is just a renderer, you must handle window and it's event loop, calling:
    // void evk_update(float deltaTime); when appropriate
    // you must also resize the evk's framebuffer when a window resize happens with:
    // void evk_set_framebuffer_size(float2 size);

    // we can't forget to release all resources used
    res = evk_shutdown();

    return 0;
}
```

# Example - Displaying an image
Following from the previous example, we've defined two functions, one to render world objects and other to render ui objects.  

// TODO

# Notes
The ```tools/shader_dev/_compile.py``` script is a tool that compiles all shaders on that folders into a c array that get's embedded into ```evk/shaders/``` as code. You can generate them yourself if you want, just make sure to have [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) installed.

# Dependencies
Here's the list of dependencies used on this project:
* [vulkan-headers](https://github.com/KhronosGroup/Vulkan-Headers) : Interface that provide the API definitions, types, constants, and function prototypes;
* [volk](https://github.com/zeux/volk) : Dynamically load entrypoints required to use Vulkan without linking;
* [memm](https://github.com/franzpedd/memm) : Tracks all allocations made, report all leaks uppon application's request;
* [vecmath](https://github.com/franzpedd/vecmath) : Linear algebra maths library;
* [ctoolbox](https://github.com/franzpedd/ctoolbox) : Utility containers and functions for the c language;