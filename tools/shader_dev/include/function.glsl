/// @brief rotates the uv coordinates within it's center
vec2 func_rotate_uv(vec2 uv, float angle)
{
    vec2 center = vec2(0.5, 0.5);
    uv -= center;

    float xcos = cos(angle);
    float xsin = sin(angle);
    mat2 rot = mat2(xcos, -xsin, xsin, xcos);

    uv = rot * uv;
    uv += center;
    return uv;
}

/// @brief applies the rotation, scale and translation on uv coordinates
vec2 func_transform_uv(vec2 uv, vec2 offset, vec2 scale, float angle) {
    uv = func_rotate_uv(uv, angle);
    uv = (uv + offset) * scale;
    return uv;
}

/// @brief returns a billboard matrix for the given camera and model matrices, applys axis locking if enabled
mat4 func_get_billboard_matrix(const mat4 model, const mat4 view, float billboard_on, float lock_x, float lock_y)
{
    // if billboarding is completely disabled, return original model matrix
    if (billboard_on == 0.0 || lock_x == 1.0 && lock_y == 1.0) {
        return model;
    }

    // retrieve scale from model matrix
    vec3 scale = vec3(length(model[0].xyz), length(model[1].xyz), length(model[2].xyz));
    vec3 model_translation = model[3].xyz;

    // default camera vectors
    vec3 world_up = vec3(0.0, 1.0, 0.0);
    vec3 camera_forward = normalize(vec3(view[0][2], view[1][2], view[2][2]));
    
    // lock X-axis only: vertical billboard
    if (lock_x == 1.0) {
        vec3 camera_forward_xz = normalize(vec3(0.0, camera_forward.y, camera_forward.z));
        vec3 camera_right = vec3(1.0, 0.0, 0.0); // fixed right vector (X-axis locked)
        vec3 camera_up = normalize(cross(camera_right, camera_forward_xz));
        return mat4(vec4(camera_right * scale.x, 0.0), vec4(camera_up * scale.y, 0.0), vec4(camera_forward_xz * scale.z, 0.0), vec4(model_translation, 1.0));
    }

    // lock Y-axis only: horizontal billboard)
    if (lock_y == 1.0) {
        vec3 camera_forward_xz = normalize(vec3(camera_forward.x, 0.0, camera_forward.z));
        vec3 camera_right = normalize(cross(world_up, camera_forward_xz));
        vec3 camera_up = vec3(0.0, 1.0, 0.0); // fixed up vector (Y-axis locked)
        return mat4(vec4(camera_right * scale.x, 0.0), vec4(camera_up * scale.y, 0.0), vec4(camera_forward_xz * scale.z, 0.0), vec4(model_translation, 1.0));
    }

    // full billboard: no axis locking
    vec3 camera_right = normalize(cross(world_up, camera_forward));
    vec3 camera_up = normalize(cross(camera_forward, camera_right));
    return mat4(vec4(camera_right * scale.x, 0.0), vec4(camera_up * scale.y, 0.0), vec4(camera_forward * scale.z, 0.0), vec4(model_translation, 1.0));
}