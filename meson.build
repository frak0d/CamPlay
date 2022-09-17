
project('CamPlay', 'cpp', version: '0.2',
        subproject_dir: 'libs', meson_version: '>=0.60',
        default_options: ['optimization=2', 'debug=false',
                          'strip=true', 'b_lto=true',
                          'warning_level=3', 'cpp_std=c++20'])

deps = [
    dependency('threads'),
    dependency('eigen3'),
    dependency('opencv4')
]

dir_install = meson.current_source_dir() / 'build_output'

install_subdir('configs', strip_directory: true,
               install_dir: dir_install/'configs')

exe_name = 'CamPlay_v' + meson.project_version()

bl_exe = executable(
           exe_name,
           'src/main.cpp',
           dependencies: deps,
           install: true,
           install_dir: dir_install,
           include_directories: ['libs/FunctionalPlus/include',
           						 'libs/frugally-deep/include',
           						 'libs/nlohmann-json'])
           #cpp_args: ['-Wno-unused-variable',
           #           '-Wno-unused-parameter',
           #           '-Wno-ignored-attributes'])
