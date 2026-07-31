#if defined(__linux__)

/// @brief let's ignore safety on this example
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <time.h>

/// @brief enable log messages
#define EVK_ENABLE_VALIDATIONS
/// @brief force X11 as the window manager
#define EVK_LINUX_USE_XLIB
/// @brief this must be defined in exactly one .c file
#define EVK_IMPLEMENTATION
/// @brief this may be included in many files
#include <evk/evk.h>

#define WIN_WIDTH 1366
#define WIN_HEIGHT 768
#define WIN_TITLENAME "Example"

typedef struct example_t
{
    evkSprite* sprite;
} example;

typedef struct timestep_t
{
    struct timespec start_time;
    float delta_time;       // time since last frame in seconds
    float elapsed_time;     // total elapsed time in seconds
    float frame_time;       // time of current frame
    unsigned int frame_count;
    float fps;
} timestep;

Display* g_Display;
Window g_Window;
Atom g_WMDeleteMessage;
Atom g_WMProtocols;
example g_Example;

/// @brief initialize timestep
static void timestep_init(timestep* ts) {
    clock_gettime(CLOCK_MONOTONIC, &ts->start_time);
    ts->delta_time = 0.0f;
    ts->elapsed_time = 0.0f;
    ts->frame_time = 0.0f;
    ts->frame_count = 0;
    ts->fps = 0.0f;
}

/// @brief get time in seconds from timespec
static float timespec_to_seconds(struct timespec ts) {
    return (float)ts.tv_sec + (float)ts.tv_nsec / 1000000000.0f;
}

/// @brief update timestep and return delta time
static float timestep_update(timestep* ts) {
    static struct timespec last_time = {0};
    static float fps_timer = 0.0f;
    
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    
    if (last_time.tv_sec == 0 && last_time.tv_nsec == 0) {
        last_time = current_time;
        return 0.0f;
    }
    
    // calculate delta time in seconds
    float current_seconds = timespec_to_seconds(current_time);
    float last_seconds = timespec_to_seconds(last_time);
    ts->delta_time = current_seconds - last_seconds;
    last_time = current_time;
    ts->elapsed_time += ts->delta_time;
    fps_timer += ts->delta_time;
    ts->frame_count++;
    
    if (fps_timer >= 1.0f) {
        ts->fps = (float)ts->frame_count / fps_timer;
        ts->frame_count = 0;
        fps_timer = 0.0f;
    }
    
    return ts->delta_time;
}

/// @brief creates the window object
static void x11_window_create() {
    g_Display = XOpenDisplay(NULL);
    if (g_Display == NULL) {
        printf("Failed to open X display\n");
        return;
    }
    
    int screen = DefaultScreen(g_Display);
    Window root = RootWindow(g_Display, screen);
    
    // set up window attributes with decorations
    XSetWindowAttributes swa = {0};
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask;
    swa.background_pixel = WhitePixel(g_Display, screen);
    swa.override_redirect = False;  // allow window manager to decorate
    
    // create window with standard border width for decorations, border width 0 - WM will handle
    g_Window = XCreateWindow g_Display, root, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWEventMask | CWOverrideRedirect, &swa);
    
    if (g_Window == None) {
        printf("Failed to create window\n");
        return;
    }
    
    // set window title and icon name
    XStoreName(g_Display, g_Window, WIN_TITLENAME);
    XSetIconName(g_Display, g_Window, WIN_TITLENAME);
    
    // Sset window size hints for the window manager
    XSizeHints size_hints;
    size_hints.flags = PMinSize | PMaxSize | PSize | PPosition;
    size_hints.x = 0;
    size_hints.y = 0;
    size_hints.width = WIN_WIDTH;
    size_hints.height = WIN_HEIGHT;
    size_hints.min_width = 320;     // minimum width
    size_hints.min_height = 240;    // minimum height
    size_hints.max_width = 3840;    // maximum width (4K)
    size_hints.max_height = 2160;   // maximum height (4K)
    XSetWMNormalHints(g_Display, g_Window, &size_hints);
    
    // set window class for better WM integration
    XClassHint class_hint;
    class_hint.res_name = (char*)WIN_TITLENAME;
    class_hint.res_class = (char*)WIN_TITLENAME;
    XSetClassHint(g_Display, g_Window, &class_hint);
    
    // set up window close event
    g_WMDeleteMessage = XInternAtom(g_Display, "WM_DELETE_WINDOW", False);
    g_WMProtocols = XInternAtom(g_Display, "WM_PROTOCOLS", False);
    XSetWMProtocols(g_Display, g_Window, &g_WMDeleteMessage, 1);
    
    
    XMapWindow(g_Display, g_Window);    // request window to be mapped
    XFlush(g_Display);                  // force the window manager to apply decorations
    
    // wait for map event to ensure window is ready
    XEvent event;
    while (1) {
        XNextEvent(g_Display, &event);
        if (event.type == MapNotify) {
            break;
        }
    }
    
    printf("Window created with decorations\n");
}

/// @brief releases the used objects on window creation
static void x11_window_destroy() {
    XDestroyWindow(g_Display, g_Window);
    XCloseDisplay(g_Display);
}

/// @brief creates the used assets
static void examples_create() {
    memset(&g_Example, 0, sizeof(example));
    g_Example.sprite = evk_sprite_create_from_path("assets/dev.png", 1); // hardcodding an id, user should handle object ids
}

/// @brief destroys the used assets
static void examples_destroy() {
    evk_sprite_destroy(g_Example.sprite);
}

/// @brief application main-loop
static void x11_event_loop() {
    bool running = true;
    timestep ts;
    timestep_init(&ts);
    
    const float fixed_delta = 1.0f / 60.0f;  // 60 FPS physics
    float accumulator = 0.0f;
    
    XEvent event;
    
    while (running)
    {
        while (XPending(g_Display) > 0)
        {
            XNextEvent(g_Display, &event);
            
            switch (event.type)
            {
                case ConfigureNotify:
                {
                    // window was resized
                    if (event.xconfigure.width != WIN_WIDTH || event.xconfigure.height != WIN_HEIGHT) {
                        evk_resize((float2) { (float)event.xconfigure.width, (float)event.xconfigure.height });
                    }
                    break;
                }
                
                case KeyPress:
                {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    
                    switch (keysym)
                    {
                        case XK_Escape: { running = false; break; }
                        case XK_z: case XK_Z: 
                        { 
                            evk_camera_set_lock(evk_get_main_camera(), !evk_camera_get_lock(evk_get_main_camera())); 
                            break; 
                        }
                        case XK_w: case XK_W: 
                        { 
                            evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Forward, true); 
                            break; 
                        }
                        case XK_s: case XK_S: 
                        { 
                            evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Backward, true); 
                            break; 
                        }
                        case XK_a: case XK_A: 
                        { 
                            evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Left, true); 
                            break; 
                        }
                        case XK_d: case XK_D: 
                        { 
                            evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Right, true); 
                            break; 
                        }
                    }
                    break;
                }
                
                case KeyRelease:
                {
                    KeySym keysym = XLookupKeysym(&event.xkey, 0);
                    
                    switch (keysym)
                    {
                        case XK_w: case XK_W: 
                        { 
                            evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Forward, false); 
                            break; 
                        }
                        case XK_s: case XK_S: 
                        { 
                            evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Backward, false); 
                            break; 
                        }
                        case XK_a: case XK_A: 
                        { 
                            evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Left, false); 
                            break; 
                        }
                        case XK_d: case XK_D: 
                        { 
                            evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Right, false); 
                            break; 
                        }
                    }
                    break;
                }
                
                case ClientMessage:
                {
                    if (event.xclient.message_type == g_WMProtocols && (Atom)event.xclient.data.l[0] == g_WMDeleteMessage) {
                        running = false;
                    }
                    break;
                }
                
                case FocusIn:
                {
                    // window gained focus
                    break;
                }
                
                case FocusOut:
                {
                    // window lost focus
                    break;
                }
                
                default: break;
            }
        }
        
        float delta_time = timestep_update(&ts);
        
        // clamp delta time to avoid spiral of death
        if (delta_time > 0.25f) delta_time = 0.25f;
        
        // fixed timestep update
        accumulator += delta_time;
        
        while (accumulator >= fixed_delta) {
            evk_update(fixed_delta); // FIXED TIMESTEP
            accumulator -= fixed_delta;
        }
        evk_render(delta_time);
    }
}

/// @brief this is called multiple times per update, one for rendering the objects, other for rendering the objects id
void on_render(evkContext* context, float timestep) {
    evk_sprite_render(g_Example.sprite);
}

/// @brief this is called once per update
void on_renderui(evkContext* context, void* cmdbuffer) {
    //printf("Time to render ui\n");
}

/// @brief entry-point
int main(int argc, char** argv) {
    x11_window_create();
    
    evkCreateInfo info = { 0 };
    info.appName = WIN_TITLENAME;
    info.appVersion = EVK_MAKE_VERSION(0, 1, 0, 0);
    info.engineName = "EVK";
    info.engineVersion = EVK_MAKE_VERSION(0, 0, 1, 0);
    info.width = WIN_WIDTH;
    info.height = WIN_HEIGHT;
    info.MSAA = evk_Msaa_X4;
    info.vsync = false;
    info.viewport = false;
    info.window.display = g_Display;
    info.window.window = g_Window;
    
    evkResult res = evk_init(&info);
    evk_set_render_callback(on_render);        // this function will be called when it's time to render objects
    evk_set_renderui_callback(on_renderui);    // this function will be called when it's time to render ui objects
    
    examples_create();
    x11_event_loop();
    
    examples_destroy();
    res = evk_shutdown();
    
    x11_window_destroy();
    
    return 0;
}

#endif // __linux__