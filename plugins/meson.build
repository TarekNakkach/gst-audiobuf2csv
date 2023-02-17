lib_args = common_args + []

plugin_sources = [
  'gstaudiobuff2csvplugin.c',
  'gstaudiobuff2csv.c',
  'gstaudiobuff2csv.h'
]

shlib = shared_library('gstaudiobuff2csv',
  plugin_sources,
  c_args : lib_args,
  include_directories: [configinc],
  dependencies : plugin_deps,
  gnu_symbol_visibility : 'hidden',
  install : true,
  install_dir : plugins_install_dir,
)

gst_audiobuff2csv_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with : shlib)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : 'gst-audiobuff2csv',
  filebase : 'gst-audiobuff2csv',
  description : 'Meson sample project.',
  subdirs : 'src',
  libraries : shlib,
  version : '"@0@"'.format(meson.project_version()),
)
