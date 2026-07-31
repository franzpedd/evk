#include "evk_core.h"
#include "evk_vulkan_core.h"

#include "toolbox.h"
#include "memm.h"
#include "vecmath.h"

#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus
extern "C" {
#endif

struct evkContext
{
    bool hint_viewport;
    bool hint_minimized;
    bool hint_vsync;
    bool hint_resize;

    evkCamera* mainCamera;
    idgen* idgen;
    evkMSAA msaa;

    float2 viewportSize;
    float2 framebufferSize;

    void* userpointer_callback;
    evkCallback_Render render_callback;
    evkCalllback_RenderUI renderui_callback;
};

struct evkCamera
{
    // definitions
    float fov;
    float nearDist;
    float farDist;
    float aspectRatio;
    float movementSpeed;
    float rotationSpeed;
    float modifierSpeed;

    // math
    fmat4 view;
    fmat4 viewInverse;
    fmat4 perspective;
    fmat4 perspectiveInverse;
    float3 rotation;
    float3 position;
    float3 scale;
    float3 viewPosition;
    float3 frontPosition;

    // movement
    bool shouldMove;
    bool modifierPressed;
    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;
};

static evkContext* g_EVKContext = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// context
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC evkResult evk_init(const evkCreateInfo* ci) {
    // general initialization
    memm_init();

    if (g_EVKContext == NULL) {
        g_EVKContext = (evkContext*)m_malloc(sizeof(evkContext));
        if (!g_EVKContext) {
            EVK_LOG(evk_Fatal, "Failed to allocate memory resources for evkContext");
            return evk_Failure;
        }
    }

    g_EVKContext->hint_viewport = ci->viewport;
    g_EVKContext->hint_minimized = false;
    g_EVKContext->hint_vsync = ci->vsync;
    g_EVKContext->hint_resize = false;
    g_EVKContext->msaa = ci->MSAA;
    g_EVKContext->idgen = idgen_create(1);
    g_EVKContext->mainCamera = evk_camera_create((float)(ci->width / ci->height));
    g_EVKContext->framebufferSize.x = (float)ci->width;
    g_EVKContext->framebufferSize.y = (float)ci->height;

    // vulkan initialization
    evkResult res = evk_initialize_backend(ci);

    return evk_Success;
}

EVK_FUNC evkResult evk_shutdown() {
    evk_shutdown_backend();
    idgen_destroy(g_EVKContext->idgen);
    evk_camera_destroy(g_EVKContext->mainCamera);

    m_free(g_EVKContext);
    memm_print_leaks();
    memm_shutdown();

    return evk_Success;
}

EVK_FUNC void evk_update(float timestep) {
    if (g_EVKContext->hint_minimized) return;
    evk_update_backend(timestep);
}

EVK_FUNC void evk_render(float timestep) {
    if (g_EVKContext->hint_minimized) return;
    evk_render_backend(timestep, &g_EVKContext->hint_resize);
}

EVK_FUNC void evk_resize(float2 size) {
    if (g_EVKContext == NULL) {
        return;
    }

    bool is_valid_size = (size.x > VECMATH_FLT_EPSILON) && (size.y > VECMATH_FLT_EPSILON);
    g_EVKContext->hint_minimized = !is_valid_size;

    if (is_valid_size) {
        g_EVKContext->framebufferSize = size;
    }

    g_EVKContext->hint_resize = true;
}

EVK_FUNC void evk_resize_viewport(float2 size) {
    if (!g_EVKContext) return;
    if (!g_EVKContext->hint_viewport) return;
    g_EVKContext->viewportSize = size;
}

EVK_FUNC void evk_minimize() {
    if (g_EVKContext == NULL) {
        return;
    }

    g_EVKContext->hint_minimized = true;
}

EVK_FUNC void evk_restore() {
    if (g_EVKContext == NULL) {
        return;
    }

    g_EVKContext->hint_minimized = false;
}

EVK_FUNC uint32_t evk_pick_object(float2 xy) {
    return evk_pick_object_backend(xy);
}

EVK_FUNC evkContext* evk_get_context() {
    if (!g_EVKContext) return NULL;
    return g_EVKContext;
}

EVK_FUNC evkCamera* evk_get_main_camera() {
    return g_EVKContext != NULL ? g_EVKContext->mainCamera : NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// general getters/setters
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC bool evk_using_vsync() {
    if (!g_EVKContext) return false;
    return g_EVKContext->hint_vsync;
}

EVK_FUNC bool evk_using_viewport() {
    if (!g_EVKContext) return false;
    return g_EVKContext->hint_viewport;
}

EVK_FUNC bool evk_currently_minimized() {
    if (!g_EVKContext) return false;
    return g_EVKContext->hint_minimized;
}

EVK_FUNC evkMSAA evk_get_msaa() {
    if (!g_EVKContext) return evk_Msaa_Off;
    return g_EVKContext->msaa;
}

EVK_FUNC float2 evk_get_size() {
    float2 res = { 0 };
    memset(&res, 0, sizeof(float2));

    if (!g_EVKContext) return res;
    return g_EVKContext->framebufferSize;
}

EVK_FUNC float2 evk_get_viewport_size() {
    float2 res = { 0 };
    memset(&res, 0, sizeof(float2));

    if (!g_EVKContext) return res;

    if (!g_EVKContext->hint_viewport) return res;
    return g_EVKContext->viewportSize;
}

EVK_FUNC void evk_set_viewport_size(const float2 size) {
    if (!g_EVKContext) return;
    if (!g_EVKContext->hint_viewport) return;

    g_EVKContext->viewportSize = size;
    g_EVKContext->hint_resize = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// callback related
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVK_FUNC void evk_set_user_pointer(void* pointer) {
    if (!g_EVKContext) {
        EVK_LOG(evk_Error, "EVK's context is NULL");
        return;
    }

    g_EVKContext->userpointer_callback = pointer;
}

EVK_FUNC void* evk_get_user_pointer() {
    return g_EVKContext == NULL ? NULL : g_EVKContext->userpointer_callback;
}

EVK_FUNC void evk_set_render_callback(evkCallback_Render callback) {
    if (!g_EVKContext) {
        EVK_LOG(evk_Error, "EVK's context is NULL");
        return;
    }

    g_EVKContext->render_callback = callback;
}

EVK_FUNC evkCallback_Render evk_get_render_callback() {
    return g_EVKContext == NULL ? NULL : g_EVKContext->render_callback;
}

EVK_FUNC void evk_set_renderui_callback(evkCalllback_RenderUI callback) {
    if (!g_EVKContext) {
        EVK_LOG(evk_Error, "EVK's context is NULL");
        return;
    }

    g_EVKContext->renderui_callback = callback;
}

EVK_FUNC evkCalllback_RenderUI evk_get_renderui_callback() {
    return g_EVKContext == NULL ? NULL : g_EVKContext->renderui_callback;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// logs and errors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static EVK_FUNC const char* ievk_severity_to_str(evkSeverity severity) {
    switch (severity)
    {
        case evk_Trace: return "TRACE";
        case evk_Todo: return "TODO";
        case evk_Info: return "INFO";
        case evk_Warn: return "WARN";
        case evk_Error: return "ERROR";
        case evk_Fatal: return "FATAL";
    }
    return "UNKNOWN";
}

static EVK_FUNC void ievk_log_format(char* outBuffer, size_t size, const struct tm* localTime, const char* file, int line, evkSeverity severity, const char* buffer) {
    snprintf
    (
        outBuffer,
        size,
        "[%02d/%02d/%04d - %02d:%02d:%02d][%s - %d][%s]: %s",
        localTime->tm_mday,
        localTime->tm_mon + 1,
        localTime->tm_year + 1900,
        localTime->tm_hour,
        localTime->tm_min,
        localTime->tm_sec,
        file,
        line,
        ievk_severity_to_str(severity),
        buffer
    );
}

EVK_FUNC void evk_log_message(evkSeverity severity, const char* file, unsigned int line, const char* fmt, ...) {
    char buffer[EVK_MAX_ERROR_LEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, EVK_MAX_ERROR_LEN, fmt, args);
    va_end(args);

    char log_message[EVK_MAX_ERROR_LEN + 256];
    time_t now = time(NULL);
    struct tm* local_time = localtime(&now);

    ievk_log_format(log_message, sizeof(log_message), local_time, file, line, severity, buffer);

    if (severity == evk_Fatal) {
        #if defined(__ANDROID__)
            __android_log_print(ANDROID_LOG_ERROR, "EVK", "%s", log_message);
        #else
            printf("%s\n", log_message);
        #endif
        abort();
    }

    else
    {
        #if defined(__ANDROID__)
            __android_log_print(ANDROID_LOG_DEBUG, "EVK", "%s", log_message);
        #else
            printf("%s\n", log_message);
        #endif
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// camera related
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static EVK_FUNC void ievk_camera_update_view_matrix(evkCamera* camera) {
    // calculate target point
    float3 target = float3_add(&camera->position, &camera->frontPosition);
    const float3 worldUp = { 0.0f, 1.0f, 0.0f };

    // create the view matrix
    camera->view = fmat4_lookat_vulkan(&camera->position, &target, &worldUp);
    camera->viewPosition = camera->position;
}

EVK_FUNC evkCamera* evk_camera_create(float aspectRatio) {
    evkCamera* camera = (evkCamera*)m_malloc(sizeof(evkCamera));
    EVK_ASSERT(camera != NULL, "Failed to allocate memory for evkCamera");
    memset(camera, 0, sizeof(evkCamera));

    camera->fov = 45.0f;
    camera->nearDist = 0.1f;
    camera->farDist = 256.0f;
    camera->aspectRatio = aspectRatio;
    camera->movementSpeed = 1.0f;
    camera->rotationSpeed = 1.0f;
    camera->modifierSpeed = 2.5f;

    camera->perspective = fmat4_identity();
    camera->perspectiveInverse = fmat4_identity();
    camera->view = fmat4_identity();
    camera->rotation.x = 0.0f; camera->rotation.y = 0.0f; camera->rotation.z = 0.0f;
    camera->position.x = 0.0f; camera->position.y = 1.0f; camera->position.z = 0.0f;
    camera->scale.x = 1.0f; camera->scale.y = 1.0f; camera->scale.z = 1.0f;
    camera->viewPosition.x = 0.0f; camera->viewPosition.y = 0.0f; camera->viewPosition.z = 0.0f;
    camera->frontPosition.x = 1.0f; camera->frontPosition.y = 0.0f; camera->frontPosition.z = 0.0f;

    // calculate initial perspective
    camera->perspective = fmat4_perspective_vulkan(to_fradians(camera->fov), aspectRatio, camera->nearDist, camera->farDist);
    camera->perspectiveInverse = fmat4_inverse(&camera->perspective);
    camera->shouldMove = camera->movingBackward = camera->movingForward = camera->movingLeft = camera->movingRight = false;

    // update initial view
    ievk_camera_update_view_matrix(camera);

    return camera;
}

EVK_FUNC void evk_camera_destroy(evkCamera* camera) {
    if (camera) {
        m_free(camera);
    }
}

EVK_FUNC void evk_camera_update(evkCamera* camera, float timestep) {
    if (!camera->shouldMove) return;

    // calculate front vector
    float yaw = to_fradians(camera->rotation.y);
    float pitch = to_fradians(camera->rotation.x);
    camera->frontPosition.x = f_cos(yaw) * f_cos(pitch);
    camera->frontPosition.y = f_sin(pitch);
    camera->frontPosition.z = f_sin(yaw) * f_cos(pitch);
    camera->frontPosition = float3_normalize(&camera->frontPosition);

    // calculate movement speed and right vector
    float moveSpeed = timestep * camera->movementSpeed;
    if (camera->modifierPressed) {
        moveSpeed *= camera->modifierSpeed;
    }

    const float3 worldUp = { 0.0f, 1.0f, 0.0f };
    float3 right = float3_cross(&worldUp, &camera->frontPosition);
    right = float3_normalize(&right);

    // apply movement
    if (camera->movingForward) {
        float3 movement = float3_scalar(&camera->frontPosition, moveSpeed);
        camera->position = float3_add(&camera->position, &movement);
    }
    if (camera->movingBackward) {
        float3 movement = float3_scalar(&camera->frontPosition, moveSpeed);
        camera->position = float3_sub(&camera->position, &movement);
    }
    if (camera->movingLeft) {
        float3 movement = float3_scalar(&right, moveSpeed);
        camera->position = float3_sub(&camera->position, &movement);
    }
    if (camera->movingRight) {
        float3 movement = float3_scalar(&right, moveSpeed);
        camera->position = float3_add(&camera->position, &movement);
    }

    // update view matrix
    ievk_camera_update_view_matrix(camera);

    camera->viewInverse = fmat4_inverse(&camera->view);
}

EVK_FUNC void evk_camera_set_aspect_ratio(evkCamera* camera, float aspect) {
    camera->perspective = fmat4_perspective_vulkan(to_fradians(camera->fov), aspect, camera->nearDist, camera->farDist);
    camera->perspectiveInverse = fmat4_inverse(&camera->perspective);
    camera->aspectRatio = aspect;
}

EVK_FUNC float evk_camera_get_aspect_ratio(evkCamera* camera) {
    if (!camera) return 1.0f;
    return camera->aspectRatio;
}

EVK_FUNC float evk_camera_get_fov(evkCamera* camera) {
    if (!camera) return 1.0f;
    return camera->fov;
}

EVK_FUNC void evk_camera_translate(evkCamera* camera, float3 dir) {
    camera->position = float3_add(&camera->position, &dir);
    ievk_camera_update_view_matrix(camera);
}

EVK_FUNC void evk_camera_rotate(evkCamera* camera, float3 dir) {
    // avoid scene flip
    if (camera->rotation.x >= 89.0f) camera->rotation.x = 89.0f;
    if (camera->rotation.x <= -89.0f) camera->rotation.x = -89.0f;

    // reset rotation on 360 degrees
    if (camera->rotation.x >= 360.0f) camera->rotation.x = 0.0f;
    if (camera->rotation.x <= -360.0f) camera->rotation.x = 0.0f;
    if (camera->rotation.y >= 360.0f) camera->rotation.y = 0.0f;
    if (camera->rotation.y <= -360.0f) camera->rotation.y = 0.0f;

    // apply rotation speed
    dir.x *= camera->rotationSpeed * 0.5f;
    dir.y *= camera->rotationSpeed * 0.5f;

    camera->rotation = float3_add(&camera->rotation, &dir);
    ievk_camera_update_view_matrix(camera);
}

EVK_FUNC fmat4 evk_camera_get_view(evkCamera* camera) {
    if (!camera) return fmat4_identity();
    return camera->view;
}

EVK_FUNC fmat4 evk_camera_get_view_inverse(evkCamera* camera) {
    if (!camera) return fmat4_identity();
    return camera->viewInverse;
}

EVK_FUNC fmat4 evk_camera_get_perspective(evkCamera* camera) {
    if (!camera) return fmat4_identity();
    return camera->perspective;
}

EVK_FUNC fmat4 evk_camera_get_perspective_inverse(evkCamera* camera) {
    if (!camera) return fmat4_identity();
    return camera->perspectiveInverse;
}

EVK_FUNC void evk_camera_set_lock(evkCamera* camera, bool value) {
    if (!camera) return;
    camera->shouldMove = value;
}

EVK_FUNC bool evk_camera_get_lock(evkCamera* camera) {
    if (!camera) return false;
    return camera->shouldMove;
}

EVK_FUNC void evk_camera_move(evkCamera* camera, evkCameraDir dir, bool moving) {
    if (!camera) return;

    switch (dir)
    {
        case evk_Camera_Direction_Forward: { camera->movingForward = moving; break; }
        case evk_Camera_Direction_Backward: { camera->movingBackward = moving; break; }
        case evk_Camera_Direction_Left: { camera->movingLeft = moving; break; }
        case evk_Camera_Direction_Right: { camera->movingRight = moving; break; }
    }
}

EVK_FUNC bool evk_camera_get_speed_modifier(evkCamera* camera, float* value) {
    if (!camera) return false;
    if (value) *(value) = camera->modifierSpeed;
    return camera->modifierPressed;
}

EVK_FUNC void evk_camera_set_speed_modifier(evkCamera* camera, bool status, float value) {
    if (!camera) return;
    camera->modifierPressed = status;
    camera->modifierSpeed = value;
}

EVK_FUNC float3 evk_camera_get_position(evkCamera* camera) {
    float3 res = { 0.0f };
    memset(&res, 0, sizeof(float3));
    if (!camera) return res;
    return camera->position;
}

EVK_FUNC float3 evk_camera_get_rotation(evkCamera* camera) {
    float3 res = { 0.0f };
    memset(&res, 0, sizeof(float3));
    if (!camera) return res;
    return camera->rotation;
}

EVK_FUNC float3 evk_camera_get_front(evkCamera* camera) {
    float3 res = { 0.0f };
    memset(&res, 0, sizeof(float3));
    if (!camera) return res;
    return camera->frontPosition;
}

#ifdef __cplusplus 
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */