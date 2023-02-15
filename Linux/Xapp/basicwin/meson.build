# public domain 
# migrate from two decades old autoconf/automake to meson
# it had been broken since the time

project('myXapp', 'c',
	license : 'public domain',
	version: '0.1')

c_compiler = meson.get_compiler('c')

lib_m = c_compiler.find_library('m')
lib_x11 = c_compiler.find_library('X11')

basic_x11_libs = [lib_m,lib_x11]

inc_dirs = ['.']

inc = include_directories(inc_dirs)

src_basicwin = ['basicwin.c']

src_basicwin_jis = ['basicwin-jis.c']

executable('basicwin',
	   src_basicwin,
	   include_directories: inc,
	   dependencies: basic_x11_libs)

executable('basicwin-jis',
	   src_basicwin_jis,
	   include_directories: inc,
	   dependencies: basic_x11_libs)
