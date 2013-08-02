from setuptools import setup, Extension
import subprocess
import glob
import platform

sources = glob.glob('src/*.cpp') + glob.glob('src/randomc/*.cpp') + glob.glob('src/*.i')
sources = [source for source in sources if '_wrap' not in source]

if platform.system() == 'Darwin':
    libraries = []
elif platform.system() == 'Linux':
    libraries = ['uuid']
else:
    libraries = [] # hope for the best

setup(
    name = 'stylusengine',
    ext_modules = [
        Extension(
            '_stylusengine',
            sources = sources,
            extra_compile_args = subprocess.check_output(['xml2-config', '--cflags']).split(),
            extra_link_args = subprocess.check_output(['xml2-config', '--libs']).split(),
            swig_opts = ['-c++'],
            include_dirs = ['src'],
            libraries = libraries
        )
    ],
    py_modules = ['stylusengine'],
    package_dir = {'' : 'src'}
)
