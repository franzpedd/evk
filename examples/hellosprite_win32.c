#if defined(_WIN32)

/// @brief let's ignore safety on this example
#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>

/// @brief enable log messages
#define EVK_ENABLE_VALIDATIONS
/// @brief this must be defined in exactly one .c file
#define EVK_IMPLEMENTATION
/// @brief this may be included in many files
#include <evk/evk.h>

#define WIN_WIDTH 1366
#define WIN_HEIGHT 768
#define WIN_CLASSNAME "ExampleClass"
#define WIN_TITLENAME "Example"

typedef struct example_t
{
    evkSprite* sprite;
} example;

typedef struct timestep_t
{
    LARGE_INTEGER start_time;
    LARGE_INTEGER frequency;
    float delta_time;       // time since last frame in seconds
    float elapsed_time;     // total elapsed time in seconds
    float frame_time;       // time of current frame
    unsigned int frame_count;
    float fps;
} timestep;

HWND g_HWND;
example g_Example;

/// @brief initialize timestep
static void timestep_init(timestep* ts) {
    QueryPerformanceFrequency(&ts->frequency);
    QueryPerformanceCounter(&ts->start_time);
    ts->delta_time = 0.0f;
    ts->elapsed_time = 0.0f;
    ts->frame_time = 0.0f;
    ts->frame_count = 0;
    ts->fps = 0.0f;
}

/// @brief update timestep and return delta time
static float timestep_update(timestep* ts) {
    static LARGE_INTEGER last_time = { 0 };
    static float fps_timer = 0.0f;

    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);

    if (last_time.QuadPart == 0) {
        last_time = current_time;
        return 0.0f;
    }

    // calculate delta time in seconds
    ts->delta_time = (float)(current_time.QuadPart - last_time.QuadPart) / (float)ts->frequency.QuadPart;
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

/// @brief handle windows events
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg)
    {
        case WM_CREATE:
        {
            return 0;
        }

        case WM_SIZE:
        {
            evk_resize((float2) { (float)LOWORD(lparam), (float)HIWORD(lparam) });
            return 0;
        }

        case WM_KEYDOWN:
        {
            switch (wparam)
            {
                case VK_ESCAPE: { PostQuitMessage(0); break; }
                case 'Z': case 'z': { evk_camera_set_lock(evk_get_main_camera(), !evk_camera_get_lock(evk_get_main_camera())); break; }
                case 'W': case 'w': { evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Forward, true); break; }
                case 'S': case 's': { evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Backward, true); break; }
                case 'A': case 'a': { evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Left, true); break; }
                case 'D': case 'd': { evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Right, true); break; }
            }

            return 0;
        }

        case WM_KEYUP:
        {
            switch (wparam)
            {
                case 'W': case 'w': { evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Forward, false); break; }
                case 'S': case 's': { evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Backward, false); break; }
                case 'A': case 'a': { evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Left, false); break; }
                case 'D': case 'd': { evk_camera_move(evk_get_main_camera(), evk_Camera_Direction_Right, false); break; }
            }

            return 0;
        }

        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        default:
        {
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }
}

/// @brief creates the window
static void win32_window_create() {
    HINSTANCE instance = GetModuleHandle(NULL);
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = WIN_CLASSNAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        printf("Failed to register win32 class\n");
        return;
    }

    RECT windowRect = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    g_HWND = CreateWindowEx(0, WIN_CLASSNAME, WIN_TITLENAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, NULL, NULL, instance, NULL);

    if (g_HWND == NULL) {
        printf("Failed to create window\n");
        return;
    }

    ShowWindow(g_HWND, SW_SHOWDEFAULT);
    UpdateWindow(g_HWND);
}

/// @brief releases the used objects on window creation
static void win32_window_destroy() {
    DestroyWindow(g_HWND);
    UnregisterClass(WIN_CLASSNAME, GetModuleHandle(NULL));
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
static void win32_event_loop() {
    MSG msg = { 0 };
    bool running = true;
    timestep ts;
    timestep_init(&ts);

    const float fixed_delta = 1.0f / 60.0f;  // 60 FPS physics
    float accumulator = 0.0f;

    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!running) break;

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

// @brief this is called multiple times per update, one for rendering the objects, other for rendering the objects id
void on_render(evkContext* context, float timestep) {
    evk_sprite_render(g_Example.sprite);
}

/// @brief this is called once per update
void on_renderui(evkContext* context, void* cmdbuffer) {
    //printf("Time to render ui\n");
}

/// @brief entry-point
int main(int argc, char** argv) {
    win32_window_create();

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
    info.window.window = g_HWND;

    evkResult res = evk_init(&info);
    evk_set_render_callback(on_render);        // this function will be called when it's time to render objects
    evk_set_renderui_callback(on_renderui);    // this function will be called when it's time to render ui objects
    
    examples_create();
    win32_event_loop();

    examples_destroy();
    res = evk_shutdown();

    win32_window_destroy();

    return 0;
}

#endif // _WIN32