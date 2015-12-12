from setuptools import setup, Extension
import subprocess
import glob
import platform

sources = glob.glob('src/*.cpp') + glob.glob('src/randomc/*.cpp') + glob.glob('src/*.i')
sources = [source for source in sources if '_wrap' not in source]

include_dirs = ['src']

if platform.system() == 'Darwin':
    extra_compile_args = subprocess.check_output(['xml2-config', '--cflags']).split()
    extra_link_args = subprocess.check_output(['xml2-config', '--libs']).split()
    libraries = ['ssl']
elif platform.system() == 'Linux':
    extra_compile_args = subprocess.check_output(['xml2-config', '--cflags']).split()
    extra_link_args = subprocess.check_output(['xml2-config', '--libs']).split()
    libraries = ['ssl', 'crypto']
elif platform.system() == 'Windows':
    libraries = ["libeay32", r"ssleay32", "libxml2"] 
    extra_compile_args = ['/D_USE_MATH_DEFINES']
    extra_link_args = [r"/LIBPATH:C:\extern\lib\vc\static", r"/LIBPATH:C:\extern\lib"]
    include_dirs.append(r"C:\extern\include")
elif "CYGWIN" in platform.system():
    extra_compile_args = subprocess.check_output(['xml2-config', '--cflags']).split()
    extra_link_args = subprocess.check_output(['xml2-config', '--libs']).split()
    libraries = ['uuid', 'crypto']
else:
    libraries = [] # hope for the best
    extra_compile_args = []
    extra_link_args = []

setup(
    name = 'stylusengine',
    ext_modules = [
        Extension(
            '_stylusengine',
            sources = sources,
	    extra_compile_args = extra_compile_args,
	    extra_link_args = extra_link_args,
	    include_dirs = include_dirs,
            swig_opts = ['-c++'],
            libraries = libraries
        )
    ],
    py_modules = ['stylusengine'],
    package_dir = {'' : 'src'}
)
