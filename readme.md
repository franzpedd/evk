### Structure of this project

```
./build/headeronly                  - header-only version of the project for easy integration
./evk:                              - main project source code
./evk/shader                        - SPIR-V binary code as headers (path exists because of utility shader_dev.c)
./memm                              - memory/leaks tracking project source code
./thirdparty                        - dependency projects/source code
./thirdparty/stb                    - API for loading image in various formats
./thirdparty/volk                   - vulkan meta-loader
./toolbox                           - usefull structures project source code
./utilities                         - utility separate projects to aid the API development
./utilities/headeronly_assembler    - auxiliar code to generate header only version of the API
./utilities/shader_dev              - shader development and compilation
./vecmath                           - algebra/mathmatics project source code
```

### TODO-LIST:
toolbox: Make so it's memory function accepts __LINE__ and __FILE__ to work well with memm.