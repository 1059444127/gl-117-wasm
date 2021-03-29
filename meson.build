project(
  'gl-117',
  'cpp',
  version: '1.3.2',
  license: 'GPL-2.0-or-later',
  default_options: [
    'cpp_std=c++03'
  ]
)

#sdl = dependency('sdl2')
#sdlmixer = dependency('SDL2_mixer', required: false)
#sdlnet = dependency('SDL2_net', required: false)
#gl = dependency('gl')
#glu = dependency('glu')

gl_117_datadir = get_option('datadir') / meson.project_name()

subdir('src')
subdir('doc')

subdir('maps')
subdir('models')
subdir('music')
subdir('sounds')
subdir('shaders')
subdir('textures')
subdir('units')

install_data(
  'gl-117.desktop',
  install_dir: get_option('datadir')/'applications')
install_data(
  'gl-117.png',
  install_dir: get_option('datadir')/'icons'/'hicolor'/'256x256'/'apps')
