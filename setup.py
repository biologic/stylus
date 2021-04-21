from setuptools import setup, Extension
import subprocess
import glob
import platform
import os.path
import urllib.request, urllib.parse, urllib.error
from zipfile import ZipFile

sources = glob.glob('src/*.cpp') + glob.glob('src/randomc/*.cpp') + glob.glob('src/*.i')
sources = [source for source in sources if '_wrap' not in source]

include_dirs = ['src']

if platform.system() == 'Darwin':
    extra_compile_args = subprocess.check_output(['xml2-config', '--cflags']).decode('utf-8').split()
    extra_link_args = subprocess.check_output(['xml2-config', '--libs']).decode('utf-8').split()
    libraries = []
elif platform.system() == 'Linux':
    extra_compile_args = subprocess.check_output(['xml2-config', '--cflags']).split()
    extra_link_args = subprocess.check_output(['xml2-config', '--libs']).split()
    libraries = []
elif platform.system() == 'Windows':
    if not os.path.exists("extern"):
        os.mkdir("extern")
    if not os.path.exists("extern/swigwin-3.0.7.zip"):
        urllib.request.urlretrieve("http://prdownloads.sourceforge.net/swig/swigwin-3.0.7.zip", "extern/swigwin-3.0.7.zip")
    if not os.path.exists("extern/swigwin-3.0.7"):        
        with ZipFile("extern/swigwin-3.0.7.zip") as zipfile:
            zipfile.extractall("extern")

    os.environ['PATH'] += ';' + os.path.abspath("extern/swigwin-3.0.7")            
    libraries = ["libeay32", "libxml2"] 
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
print(extra_compile_args)
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
