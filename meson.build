cmake = import('cmake')

openpnp_capture = cmake.subproject('libs/openpnp-capture')
tensorflow_lite = cmake.subproject('libs/tensorflow-lite')

project('CamPlay', 'cpp')

deps = [
    openpnp_capture.dependency('openpnp-capture'),
    tensorflow_lite.dependency('tensorflow-lite'),
]

executable('CamPlay', 'src/main.cpp', dependencies: deps, override_options: ['cpp_std=c++20'])
