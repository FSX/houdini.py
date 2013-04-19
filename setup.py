import glob
import shutil
import os.path

try:
    from setuptools import setup, Extension, Command
except ImportError:
    from distutils.core import setup, Extension, Command


dirname = os.path.dirname(os.path.abspath(__file__))


class BaseCommand(Command):
    user_options = []
    def initialize_options(self):
        pass
    def finalize_options(self):
        pass


class CleanCommand(BaseCommand):
    description = 'cleanup directories created by packaging and build processes'
    def run(self):
        for path in ['build', 'dist', 'houdini.py.egg-info']:
            path = os.path.join(dirname, path)
            if os.path.exists(path):
                print('removing %s' % path)
                shutil.rmtree(path)


class VendorCommand(BaseCommand):
    description = 'update Houdini files. Use `git submodule update` to the most recent files'
    def run(self):
        files = []
        dest = os.path.join(dirname, 'src/houdini')

        for path in ['vendor/houdini/*.h', 'vendor/houdini/*.c']:
            files += glob.glob(os.path.join(dirname, path))

        for path in files:
            if os.path.exists(path):
                print('copy %s -> %s' % (path, dest))
                shutil.copy(path, dest)


setup(
    name='houdini.py',
    version='0.1.1',
    description='A Python binding for Houdini.',
    author='Frank Smit',
    author_email='frank@61924.nl',
    url='http://python-houdini.61924.nl/',
    license='MIT',
    long_description=open(os.path.join(dirname, 'README.rst')).read(),
    cmdclass={
        'clean': CleanCommand,
        'vendor': VendorCommand
    },
    ext_modules=[Extension('houdini', [
        'src/houdini.py.c',
        'src/houdini/buffer.c',
        'src/houdini/houdini_js_e.c',
        'src/houdini/houdini_js_u.c',
        'src/houdini/houdini_uri_e.c',
        'src/houdini/houdini_uri_u.c',
        'src/houdini/houdini_xml_e.c',
        'src/houdini/houdini_href_e.c',
        'src/houdini/houdini_html_e.c',
        'src/houdini/houdini_html_u.c'
    ])],
    classifiers = [
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: C',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.2',
        'Programming Language :: Python :: 3.3',
        'Topic :: Text Processing',
        'Topic :: Text Processing :: Filters',
        'Topic :: Utilities'
    ]
)
