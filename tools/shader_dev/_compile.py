#!/usr/bin/env python3
"""Compile all GLSL shaders to SPIR-V and generate C headers."""
import sys
import subprocess
import struct
from pathlib import Path

# config
SHADER_DIR = ""             # where GLSL shaders are located (root)
OUTPUT_DIR = "bin"          # where SPIR-V files will be saved
HEADER_DIR = "../../evk/include/shader"  # where C headers will be generated
EXT_MAP = { '.vert':'vertex','.frag':'fragment','.comp':'compute','.geom':'geometry' }
SHADER_EXTS = { *EXT_MAP.keys(), '.glsl' }

# returns what compiler found
def find_compiler():
    for c in ['glslc','glslangValidator']:
        try:
            subprocess.run([c,'--version'], capture_output=True, timeout=5)
            print(f"Compiler: {c}")
            return c
        except: continue
    return None

# compiles a shader
def compile_shader(compiler, inp, out_dir, hdr_dir):
    inp = Path(inp)
    shader_type = inp.suffix[1:]
    out_name = f"{inp.stem}_{shader_type}.spv"
    out = Path(out_dir) / out_name
    out.parent.mkdir(parents=True, exist_ok=True)
    
    cmd = [compiler, str(inp), '-o', str(out)]
    if compiler == 'glslangValidator':
        cmd = [compiler, '-V', str(inp), '-o', str(out)]
        if stage:=EXT_MAP.get(inp.suffix.lower()):
            cmd.extend(['-S', stage])
    
    try:
        print(f"  {inp.name} -> {out.name}", end=' ')
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        if r.returncode:
            if r.stderr: 
                print(f"[FAILURE]: {r.stderr[:100]}...")
            return None
        size = out.stat().st_size
        print(f"[SUCCESS] ({size} bytes)")
        return out
    except Exception as e:
        print(f"[FAILURE] ({e})")
        return None

# generates a c source file with the spv binary data as an array
def generate_header(spv_path, hdr_dir):
    try:
        data = spv_path.read_bytes()
        var_name = f"{spv_path.stem}_spv"
        out = Path(hdr_dir) / f"{var_name}.c"
        out.parent.mkdir(parents=True, exist_ok=True)
        
        if len(data) % 4 != 0:
            data += b'\x00' * (4 - len(data) % 4)
        
        uint32_data = []
        for i in range(0, len(data), 4):
            value = struct.unpack('<I', data[i:i+4])[0]  # little-endian
            uint32_data.append(value)
        
        guard = f"{var_name.upper()}_H"
        lines = [f"// Auto-generated from {spv_path.name}",
                 f"#ifndef {guard}",
                 f"#define {guard}\n",
                 "#include <stdint.h>\n",
                 f"const uint32_t {var_name}[] = {{"]
        
        for i in range(0, len(uint32_data), 8):
            chunk = uint32_data[i:i+8]
            hex_values = ", ".join(f"0x{val:08x}" for val in chunk)
            lines.append(f"    {hex_values},")
        
        lines.extend([f"}};",
                     f"const uint32_t {var_name}_size = {len(uint32_data)};",
                     f"#endif // {guard}"])
        
        out.write_text("\n".join(lines))
        print(f"    Header: {out.name}")
        return True
    except Exception as e:
        print(f"    Header generation failed: {e}")
        return False

def main():

    # find shaders
    shader_path = Path(SHADER_DIR)
    if not shader_path.is_dir():
        print(f"ERROR: Shader directory '{SHADER_DIR}' not found!")
        sys.exit(1)
    
    shaders = [f for f in shader_path.iterdir() if f.suffix.lower() in SHADER_EXTS and f.is_file()]
    
    if not shaders:
        print("No shader files found!")
        sys.exit(1)
    
    print(f"\n Found {len(shaders)} shader(s):")
    for s in shaders: print(f" {s.name}")

    # find shader compiler
    compiler = find_compiler()
    if not compiler:
        print("ERROR: No GLSL compiler found!")
        print("Install Vulkan SDK or glslang")
        sys.exit(1)
    
    # compile shaders
    compiled = []
    for shader in shaders:
        spv = compile_shader(compiler, shader, OUTPUT_DIR, HEADER_DIR)
        if spv: compiled.append(spv)
    
    # generate headers
    headers = 0
    for spv in compiled:
        if generate_header(spv, HEADER_DIR): headers += 1

if __name__ == "__main__":
    main()