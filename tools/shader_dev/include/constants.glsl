/// @brief holds the unique identifier of an object and it's model matrix, maximum size must be 128 bytes
layout(push_constant) uniform constants
{
    uint id;
	mat4 model;
} push_const;

