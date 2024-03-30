project('npak', 'c', version: '1.0.0')

project_description = 'Library to handle working with Alicia\'s NPAK files'

project_headers = [
  'include/npak/visibility.h',
  'include/npak/npak.h',
  'include/npak/types.h',
  'include/npak/errors.h'
]

project_source = [
  'src/npak.c',
  'src/errors.c',
  'src/types.c'
]

build_args = []

public_headers = include_directories('include')

project_dependencies = dependency('zlib')

build_args += [
  '-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
]

project_target = shared_library(
  meson.project_name(),
  project_source,
  install: true,
  c_args: build_args, 
  gnu_symbol_visibility: 'hidden',
  include_directories: public_headers,
  dependencies: project_dependencies
)

project_dep = declare_dependency(
  include_directories: public_headers,
  link_with : project_target
)

set_variable(meson.project_name() + '_dep', project_dep)

install_headers(project_headers, subdir : meson.project_name())

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : meson.project_name(),
  libraries : project_target,
)


if not meson.is_subproject()
  test('errors_test',
    executable(
      'errors',
      files('tests/errors.c'),
      dependencies : [project_dep],
      install : false
    )
  )

    test('npak_test',
    executable(
      'npak',
      files('tests/npak.c'),
      dependencies : [project_dep],
      install : false
    )
  )
endif