/// @brief returns the vertex of a sprite
vec3 Sprite_Vertex[6] = vec3[]
(
    // first triangle
    vec3(-0.5, -0.5, 0.0),  // bottom-left
    vec3( 0.5, -0.5, 0.0),  // bottom-right  
    vec3( 0.5,  0.5, 0.0),  // top-right
    
    // second triangle
    vec3( 0.5,  0.5, 0.0),  // top-right
    vec3(-0.5,  0.5, 0.0),  // top-left
    vec3(-0.5, -0.5, 0.0)   // bottom-left
);

/// @brief returns the uv coords of a sprite
vec2 Sprite_UV[6] = vec2[](
    vec2(0.0, 1.0),  // top-left
    vec2(1.0, 1.0),  // top-right
    vec2(1.0, 0.0),  // bottom-right
    
    vec2(1.0, 0.0),  // bottom-right
    vec2(0.0, 0.0),  // bottom-left
    vec2(0.0, 1.0)   // top-left
);