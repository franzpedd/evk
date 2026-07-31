# vecmath

A standalone C library for vector and matrix mathematics, designed for computer graphics applications. 
It's dependencies are: ```math.h``` and ```string.h```

## features

- **dual precision** – Single (`f` prefix) and double (`d` prefix) precision support
- **multiple build modes** – Static library, dynamic library, or header-only
- **cross platform** – Windows, Linux, macOS

## types

- **float2 / double2** – 2D vector
- **float3 / double3** – 3D vector
- **float4 / double4** – 4D vector
- **fmat2 / dmat2** – 2x2 matrix
- **fmat3 / dmat3** – 3x3 matrix
- **fmat4 / dmat4** – 4x4 matrix
- **fquat / dquat** – quaternion
- **fray / dray** – ray

## functions
- **vecmath_basic_op:** Basic math operations common for all types
    - **zero/one:** fills the structure with zeroes/ones
    - **add/sub/mul/div:** addition/subtraction/multiplication/division
    - **equals:** checks if equals in value (uses epsilon)

- **vecmath_vec_op:** Vector specific operations
    - **float4_scalar/mat_mul:** multiplication by scalar/matrix
    - **length/distance:** pythagorean theorems
    - **normalize/dot/cross:** usefull functions for graphics
    - **lerp:** linear interpolation
    - **reflect/project:** vector reflection and projection operations

- **vecmath_mat_op:** Matrix specific operations
    - **identity:** returns identity matrices
    - **vec_mul:** multiplication by vector
    - **transpose:** switch lines by colums
    - **determinant:** calculates the determinant
    - **inverse:** returns the inverse matrix
    - **get_translation/get_scale/get_rotation:** matrix decomposition (both col and row major)
    - **translate/scale/rotate:** matrix operations (both col and row major)
    - **decompose:** retrieves translation/scale/rotation and once (both col and row major)
    - **lookat/perspective/ortographic:** projections (available as opengl/directx/vulkan variants)

- **vecmath_quat_op:** Quaternion specific operations
    - **identity:** returns the identity quaternion
    - **length:** returns the length of the quaternion
    - **conjulgate/normalize/dot:** usefull functions for quaternions
    - **lerp/slerp:** interpolation functions
    - **from_euler/to_euler:** converts between types
    - **to_mat:** converts into matrix representation (both col and row major)

- **vecmath_ray_op:** Ray specific operations (vulkan only at the momment)
    - **fray_from_screen_point:** creates a ray from screen coordinates
    - **ray_screen_to_world_point:** gets world point from screen position + distance

- **vecmath_util:** General utility functions
    - **to_radians/to_deg:** convert angles between degrees and radians
    - **cos/sin/tan:** standard trigonometric functions
    - **power:** fast exponentiation using exponentiation by squaring
    - **log2/log10/logn:** logarithmic operations
    - **floor/ceil/frac/round:** rounding and fractional part functions
    - **min/max/clamp:** value clamping and bounds checking