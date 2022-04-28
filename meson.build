
project('CamPlay', 'cpp', version: '0.1',
        subproject_dir: 'libs', meson_version: '>=0.60',
        default_options: ['optimization=2', 'debug=false',
                          'strip=true', 'b_lto=true',
                          'warning_level=3', 'cpp_std=c++20'])

cmake = import('cmake')

#openpnp_capture = cmake.subproject('openpnp-capture')
tensorflow_lite = cmake.subproject('tensorflow-lite')

deps = [
    dependency('threads'),
    openpnp_capture.dependency('openpnp-capture'),
    tensorflow_lite.dependency('tensorflow-lite')
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
           install_dir: dir_install)
           #cpp_args: ['-Wno-unused-variable',
           #           '-Wno-unused-parameter',
           #           '-Wno-ignored-attributes'])
