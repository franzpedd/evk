#include "vecmath_ray_op.h"
#include "vecmath_vec_op.h"
#include "vecmath_basic_op.h"
#include <math.h>

/* HEADER_ONLY_ASSEMBLER_START */

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ray from screen point (vulkan only for now)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC fray fray_from_screen_point_vulkan(const float2* screenPos, const float2* windowSize, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp) {
    fray result = fray_zero();
    if (!screenPos || !windowSize || !cameraPos || !cameraFront || !cameraUp) return result;
    
    float ndcX = (2.0f * screenPos->x) / windowSize->x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPos->y) / windowSize->y;

    float3 cameraRight = float3_cross(cameraUp, cameraFront);
    cameraRight = float3_normalize(&cameraRight);

    float3 up = float3_cross(cameraFront, &cameraRight);
    float3 normalizedUp = float3_normalize(&up);

    float halfHeight = tanf(fov * 0.5f);
    float halfWidth = halfHeight * aspectRatio;

    float3 direction = {
        cameraFront->x + cameraRight.x * (ndcX * halfWidth) + normalizedUp.x * (ndcY * halfHeight),
        cameraFront->y + cameraRight.y * (ndcX * halfWidth) + normalizedUp.y * (ndcY * halfHeight),
        cameraFront->z + cameraRight.z * (ndcX * halfWidth) + normalizedUp.z * (ndcY * halfHeight)
    };

    result.origin = *cameraPos;
    result.direction = float3_normalize(&direction);

    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// screen to world point (vulkan only for now)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VECMATH_FUNC float3 fray_screen_to_world_point_vulkan(const float2* screenPos, const float2* windowSize, float distance, float fov, float aspectRatio, const float3* cameraPos, const float3* cameraFront, const float3* cameraUp) {
    float3 result = float3_zero();
    if (!screenPos || !windowSize || !cameraPos || !cameraFront || !cameraUp) return result;

    float ndcX = (2.0f * screenPos->x) / windowSize->x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPos->y) / windowSize->y;

    float3 cameraRight = float3_cross(cameraUp, cameraFront);
    cameraRight = float3_normalize(&cameraRight);

    float3 up = float3_cross(cameraFront, &cameraRight);
    float3 normalizedUp = float3_normalize(&up);

    float halfHeight = distance * tanf(fov * 0.5f);
    float halfWidth = halfHeight * aspectRatio;

    float3 worldPoint = {
        cameraPos->x + cameraFront->x * distance + cameraRight.x * (ndcX * halfWidth) + normalizedUp.x * (ndcY * halfHeight),
        cameraPos->y + cameraFront->y * distance + cameraRight.y * (ndcX * halfWidth) + normalizedUp.y * (ndcY * halfHeight),
        cameraPos->z + cameraFront->z * distance + cameraRight.z * (ndcX * halfWidth) + normalizedUp.z * (ndcY * halfHeight)
    };

    return worldPoint;
}

#ifdef __cplusplus
}
#endif

/* HEADER_ONLY_ASSEMBLER_STOP */