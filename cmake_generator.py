#!/usr/bin/env python3
import json
from os import makedirs, listdir
from os.path import exists, join, abspath

# ==============================================================================
# DEFAULT LIBRARY DATABASE
# ==============================================================================
DEFAULT_LIBRARY_DB = {
    "Vulkan": {
        "type": "find_package",
        "find_args": "Vulkan REQUIRED",
        "link_libraries": ["Vulkan::Vulkan"],
        "compile_definitions": []
    },
    "Shaderc": {
        "type": "find_library",
        "var_name": "SHADERC_LIB",
        "names": ["shaderc_shared", "shaderc"],
        "link_libraries": ["${SHADERC_LIB}"],
        "compile_definitions": []
    },
    "volk": {
        "type": "fetch_content",
        "repo": "https://github.com/zeux/volk.git",
        "tag": "master",
        "options": {
            "VOLK_INSTALL": "OFF"
        },
        "link_libraries": ["volk::volk"],
        "compile_definitions": ["VK_NO_PROTOTYPES"],
        "test_code": {
            "main.cpp": {
                "position": "line 3",
                "code": "#define VOLK_IMPLEMENTATION\n#include <volk.h>"
            }
        }
    },
    "Slint": {
        "type": "fetch_content",
        "repo": "https://github.com/slint-ui/slint",
        "tag": "v1.18.0",
        "options": {
        },
        "link_libraries": ["Slint"],
        "compile_definitions": []
    },
    "VulkanMemoryAllocator": {
        "type": "fetch_content",
        "repo": "https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git",
        "tag": "master",
        "options": {
            "VMA_BUILD_DOCUMENTATION": "OFF",
            "VMA_BUILD_SAMPLE": "OFF"
        },
        "link_libraries": ["VulkanMemoryAllocator"],
        "compile_definitions": []
    },
    "SDL3": {
        "type": "fetch_content",
        "repo": "https://github.com/libsdl-org/SDL.git",
        "tag": "main",
        "options": {
            "SDL_TEST": "OFF",
            "SDL_EXAMPLES": "OFF"
        },
        "link_libraries": ["SDL3::SDL3"],
        "special_target_logic": """if(TARGET SDL3::SDL3_main)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3_main SDL3::SDL3)
elseif(TARGET SDL3_main)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_main SDL3::SDL3)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
endif()"""
    },
    "glad": {
        "type": "fetch_content",
        "repo": "https://github.com/Dav1dde/glad.git",
        "tag": "v0.1.36",
        "options": {
            "GLAD_PROFILE": "core",
            "GLAD_API": "gl=4.6"
        },
        "link_libraries": ["glad"],
        "compile_definitions": ["GLAD_GL_CLI"]
    },
    "glfw": {
        "type": "find_package",
        "find_args": "glfw3 3.3 REQUIRED",
        "link_libraries": ["glfw"],
        "compile_definitions": []
    }
}

DEFAULT_TEMPLATES = {
    "vulkan_app": {
        "project_name": "VulkanApp",
        "cpp_standard": "20",
        "folders": {"src": "src", "include": "include", "assets": "assets", "shaders": "none"},
        "find_libraries": ["Vulkan", "Shaderc"],
        "fetch_libraries": ["SDL3", "volk", "VulkanMemoryAllocator"],
        "copy_assets_post_build": True
    },
    "opengl_app": {
        "project_name": "OpenGLApp",
        "cpp_standard": "17",
        "folders": {"src": "src", "include": "include", "assets": "assets"},
        "find_libraries": ["glfw"],
        "fetch_libraries": ["glad"],
        "copy_assets_post_build": True
    }
}

DB_FILE = "library_db.json"
TEMPLATE_FILE = "templates.json"


# ==============================================================================
# UTILITY FUNCTIONS
# ==============================================================================
def load_database(file_path: str, default_data: dict) -> dict:
    if exists(file_path):
        try:
            with open(file_path, "r", encoding="utf-8") as f:
                return json.load(f)
        except Exception as e:
            print(f"[!] Error loading {file_path}: {e}. Using defaults.")
    else:
        with open(file_path, "w", encoding="utf-8") as f:
            json.dump(default_data, f, indent=4)
    return default_data


def save_json(file_path: str, data: dict):
    with open(file_path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4)


def normalize_folders_dict(config: dict) -> dict:
    folders_cfg = config.get("folders", {})
    if isinstance(folders_cfg, list):
        normalized = {}
        for f in folders_cfg:
            normalized[f] = f if f in ["src", "include", "assets"] else "none"
        return normalized
    return folders_cfg


# ==============================================================================
# CMAKE GENERATION LOGIC
# ==============================================================================
def generate_cmakelists(config: dict, lib_db: dict) -> str:
    proj_name = config.get("project_name", "MyProject")
    cpp_std = config.get("cpp_standard", "20")
    find_libs = config.get("find_libraries", [])
    fetch_libs = config.get("fetch_libraries", [])

    folders_cfg = normalize_folders_dict(config)

    src_dirs = [k.replace('\\', '/') for k, v in folders_cfg.items() if v == "src"]
    inc_dirs = [k.replace('\\', '/') for k, v in folders_cfg.items() if v == "include"]
    ast_dirs = [k.replace('\\', '/') for k, v in folders_cfg.items() if v == "assets"]

    if not src_dirs: src_dirs = ["src"]
    if not inc_dirs: inc_dirs = ["include"]

    out = []
    out.append("cmake_minimum_required(VERSION 3.25)")
    out.append(f"project({proj_name} VERSION 1.0.0 LANGUAGES CXX C)\n")

    out.append("# " + "-" * 78)
    out.append("# 1. Compiler Settings")
    out.append("# " + "-" * 78)
    out.append(f"set(CMAKE_CXX_STANDARD {cpp_std})")
    out.append("set(CMAKE_CXX_STANDARD_REQUIRED ON)")
    out.append("set(CMAKE_CXX_EXTENSIONS OFF)")
    out.append("set(CMAKE_EXPORT_COMPILE_COMMANDS ON)\n")

    all_find_libs = [lib_db[name] for name in find_libs if name in lib_db]
    if all_find_libs:
        out.append("# " + "-" * 78)
        out.append("# 2. Find Local Dependencies")
        out.append("# " + "-" * 78)
        for lib in all_find_libs:
            if lib["type"] == "find_package":
                out.append(f"find_package({lib['find_args']})")
            elif lib["type"] == "find_library":
                names = " ".join(lib["names"])
                out.append(f"find_library({lib['var_name']} NAMES {names} REQUIRED)")
        out.append("")

    all_fetch_libs = [lib_db[name] for name in fetch_libs if name in lib_db]
    if all_fetch_libs:
        out.append("# " + "-" * 78)
        out.append("# 3. Fetch GitHub Dependencies")
        out.append("# " + "-" * 78)
        out.append("include(FetchContent)\n")
        for lib_name in fetch_libs:
            if lib_name not in lib_db:
                continue
            lib = lib_db[lib_name]
            out.append(f'message(STATUS "Fetching {lib_name}...")')
            out.append(f"FetchContent_Declare(\n    {lib_name}\n    GIT_REPOSITORY {lib['repo']}\n    GIT_TAG        {lib['tag']}\n)")
            for opt_key, opt_val in lib.get("options", {}).items():
                out.append(f'set({opt_key} {opt_val} CACHE BOOL "Option for {lib_name}" FORCE)')
            out.append(f"FetchContent_MakeAvailable({lib_name})\n")

    out.append("# " + "-" * 78)
    out.append("# 4. Target & Sources")
    out.append("# " + "-" * 78)

    src_globs = " ".join([f'"{d}/*.cpp"' for d in src_dirs])
    inc_globs = " ".join([f'"{d}/*.h" "{d}/*.hpp"' for d in inc_dirs])
    out.append(f'file(GLOB_RECURSE SOURCES {src_globs})')
    out.append(f'file(GLOB_RECURSE HEADERS {inc_globs})\n')

    out.append("if(ANDROID)")
    out.append("    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})")
    out.append("else()")
    out.append("    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})")
    out.append("endif()\n")

    out.append("target_include_directories(${PROJECT_NAME} PRIVATE")
    for d in inc_dirs:
        out.append(f"    ${{CMAKE_CURRENT_SOURCE_DIR}}/{d}")
    out.append(")\n")

    selected_libs = [lib_db[k] for k in (find_libs + fetch_libs) if k in lib_db]
    link_targets = []
    compile_defs = []
    special_logics = []

    for lib in selected_libs:
        link_targets.extend(lib.get("link_libraries", []))
        compile_defs.extend(lib.get("compile_definitions", []))
        if "special_target_logic" in lib:
            special_logics.append(lib["special_target_logic"])

    out.append("# " + "-" * 78)
    out.append("# 5. Link Dependencies & Definitions")
    out.append("# " + "-" * 78)

    if link_targets:
        out.append("target_link_libraries(${PROJECT_NAME} PRIVATE")
        for target in link_targets:
            out.append(f"    {target}")
        out.append(")\n")

    for logic in special_logics:
        out.append(f"{logic}\n")

    if compile_defs:
        out.append("target_compile_definitions(${PROJECT_NAME} PRIVATE")
        for cdef in compile_defs:
            out.append(f"    {cdef}")
        out.append(")\n")

    if ast_dirs:
        out.append("# " + "-" * 78)
        out.append("# 6. Assets Handling")
        out.append("# " + "-" * 78)
        out.append("if(NOT ANDROID)")
        for asset_dir in ast_dirs:
            out.append("    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD")
            out.append("        COMMAND ${CMAKE_COMMAND} -E copy_directory")
            out.append(f'        "${{CMAKE_CURRENT_SOURCE_DIR}}/{asset_dir}"')
            out.append(f'        "$<TARGET_FILE_DIR:${{PROJECT_NAME}}>/{asset_dir}"')
            out.append(f'        COMMENT "Copying {asset_dir} folder to binary directory..."')
            out.append("    )")
        out.append("endif()\n")

    return "\n".join(out)


# ==============================================================================
# DIRECTORY & FILE GENERATION
# ==============================================================================
def create_project_structure(base_path: str, config: dict, cmake_content: str, lib_db: dict):
    makedirs(base_path, exist_ok=True)

    # Write CMakeLists.txt
    with open(join(base_path, "CMakeLists.txt"), "w", encoding="utf-8") as f:
        f.write(cmake_content)

    folders_cfg = normalize_folders_dict(config)

    # Resolve primary src and include directories for our default boilerplate
    primary_src_dir = next((k for k, v in folders_cfg.items() if v == "src"), "src")
    primary_inc_dir = next((k for k, v in folders_cfg.items() if v == "include"), "include")

    # Create all configured directories and handle empty children
    for folder_path, ftype in folders_cfg.items():
        full_path = join(base_path, folder_path)
        makedirs(full_path, exist_ok=True)
        gitkeep = join(full_path, ".gitkeep")
        if ftype not in ["src", "include"] and not exists(gitkeep) and len(listdir(full_path)) == 0:
            open(gitkeep, "a").close()

    # Gather test code block from selected libraries
    selected_libs = config.get("find_libraries", []) + config.get("fetch_libraries", [])
    line_3_injections = []

    for lib_name in selected_libs:
        lib = lib_db.get(lib_name, {})
        test_code_info = lib.get("test_code", {}).get("main.cpp")
        if test_code_info and test_code_info.get("code"):
            line_3_injections.append(test_code_info["code"])

    # Build main.cpp contents (Insert test code at line 3)
    main_cpp_lines = [
        "#include <iostream>",
        '#include "utils.h"'
    ]
    if line_3_injections:
        main_cpp_lines.append("\n// --- Library Test Code ---")
        main_cpp_lines.append("\n\n".join(line_3_injections))
        main_cpp_lines.append("// -------------------------\n")
    else:
        main_cpp_lines.append("")

    main_cpp_lines.extend([
        "int main() {",
        '    std::cout << "Project initialized successfully!" << std::endl;',
        "    printUtilsMessage();",
        "    return 0;",
        "}"
    ])

    # Build utils.h contents
    utils_h_content = """#pragma once\n\nvoid printUtilsMessage();\n"""

    # Build utils.cpp contents
    utils_cpp_content = """#include "utils.h"
#include <iostream>

void printUtilsMessage() {
    std::cout << "Hello from utils.cpp!" << std::endl;
}
"""

    # Write source files
    src_full_path = join(base_path, primary_src_dir)
    makedirs(src_full_path, exist_ok=True)
    with open(join(src_full_path, "main.cpp"), "w", encoding="utf-8") as f:
        f.write("\n".join(main_cpp_lines))

    with open(join(src_full_path, "utils.cpp"), "w", encoding="utf-8") as f:
        f.write(utils_cpp_content)

    # Write include file
    inc_full_path = join(base_path, primary_inc_dir)
    makedirs(inc_full_path, exist_ok=True)
    with open(join(inc_full_path, "utils.h"), "w", encoding="utf-8") as f:
        f.write(utils_h_content)

    print(f"\n[+] Project created successfully at: {abspath(base_path)}")


# ==============================================================================
# CLI WIZARD
# ==============================================================================
def run_wizard():
    lib_db = load_database(DB_FILE, DEFAULT_LIBRARY_DB)
    templates = load_database(TEMPLATE_FILE, DEFAULT_TEMPLATES)

    print("==================================================")
    print("           CMake Generator & Scaffold             ")
    print("==================================================")

    use_tpl = input("Do you want to load a project template? (y/n): ").strip().lower() == "y"
    config = {}

    if use_tpl:
        print("\nAvailable templates:")
        tpl_keys = list(templates.keys())
        for idx, key in enumerate(tpl_keys, 1):
            print(f"  [{idx}] {key}")

        choice = int(input("Select template number: ")) - 1
        selected_tpl_name = tpl_keys[choice]
        config = templates[selected_tpl_name].copy()

        custom_name = input(f"Project Name [{config['project_name']}]: ").strip()
        if custom_name:
            config["project_name"] = custom_name
    else:
        config["project_name"] = input("Project Name [MyProject]: ").strip() or "MyProject"
        config["cpp_standard"] = input("C++ Standard (17/20/23) [20]: ").strip() or "20"

        # Note: Added colon syntax logic (e.g. custom_src:src) to streamline custom setups
        print("\nFolders to create. You can map types inline using a colon (e.g., custom_src:src).")
        raw_folders = input("Folders (comma-separated) [src,include,assets]: ").strip()
        config["folders"] = {}

        if not raw_folders:
            config["folders"] = {"src": "src", "include": "include", "assets": "assets", "shaders": "none"}
        else:
            for f in raw_folders.split(","):
                f = f.strip()
                if not f: continue

                # Allow assigning types inline (e.g. `core:src, math:include`)
                if ":" in f:
                    fname, ftype = f.split(":", 1)
                    config["folders"][fname.strip()] = ftype.strip().lower()
                # Assume child if it contains slashes
                elif "/" in f or "\\" in f:
                    config["folders"][f] = "child"
                # Interactive prompt fallback for custom folder names that aren't children
                else:
                    ftype = input(f"  > What is the role of '{f}'? (src/include/assets/none) [none]: ").strip().lower()
                    if ftype not in ["src", "include", "assets"]:
                        ftype = "none"
                    config["folders"][f] = ftype

        find_options = [k for k, v in lib_db.items() if v["type"] in ("find_package", "find_library")]
        print("\nSelect local libraries to FIND (comma-separated numbers or leave blank):")
        for i, k in enumerate(find_options, 1):
            print(f"  [{i}] {k} ({lib_db[k]['type']})")
        selected_find = input("> ").strip()
        config["find_libraries"] = [find_options[int(i)-1] for i in selected_find.split(",") if i.strip().isdigit()]

        fetch_options = [k for k, v in lib_db.items() if v["type"] == "fetch_content"]
        print("\nSelect GitHub libraries to FETCH (comma-separated numbers or leave blank):")
        for i, k in enumerate(fetch_options, 1):
            print(f"  [{i}] {k}")
        selected_fetch = input("> ").strip()
        config["fetch_libraries"] = [fetch_options[int(i)-1] for i in selected_fetch.split(",") if i.strip().isdigit()]

        save_tpl = input("\nSave these settings as a new template? (y/n): ").strip().lower() == "y"
        if save_tpl:
            tpl_name = input("Template identifier name: ").strip()
            templates[tpl_name] = config
            save_json(TEMPLATE_FILE, templates)
            print(f"[+] Saved template '{tpl_name}'")

    target_dir = input(f"Output directory [{config['project_name']}]: ").strip() or config["project_name"]

    cmake_code = generate_cmakelists(config, lib_db)
    # Passed lib_db through to allow access to testing logic
    create_project_structure(target_dir, config, cmake_code, lib_db)


if __name__ == "__main__":
    run_wizard()
