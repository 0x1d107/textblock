project(
  'textblock',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'],
)

dependencies = [
]

exe = executable(
  'textblock',
  'textblock.cpp',
  'blocks.cpp',
  'blocks.h',
  'input_handler.h',
  'input_handler.cpp',
  'tokens.h',
  'tokens.cpp',
  'fexp_evaluator.cpp',
  install : true,
  dependencies : dependencies,
)

test('basic', exe)
