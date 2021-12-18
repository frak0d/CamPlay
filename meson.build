cmake = import('cmake')

openpnp-capture_proj = cmake.subproject('libs/openpnp-capture')
tensorflow-lite_proj = cmake.subproject('libs/tensorflow-lite')

project('CamPlay', 'cpp')

deps = [
	 openpnp-capture_proj.dependency('openpnp-capture'),
	 tensorflow-lite_proj.dependency('tensorflow-lite'),
]

executable('CamPlay', 'src/main.cpp', dependencies: deps, override_options: ['cpp_std=c++20'])
