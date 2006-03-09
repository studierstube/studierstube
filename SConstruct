#!/usr/bin/env python
import os
import sys
import buildutils

# Studierstube version.
version='4.0'

ace_env = Environment (ENV = os.environ)
tinyxml_env = Environment (ENV = os.environ)
coin_env = Environment (ENV = os.environ)
soqt_env = Environment (ENV = os.environ)
opentracker_env =  Environment (ENV = os.environ)
openvideo_env =  Environment (ENV = os.environ)
env = Environment (ENV = os.environ)

if sys.platform == 'linux2' or sys.platform == 'linux-i386':
    config_file       = 'config.opts'
    root_build_dir    = 'build/linux'
    install_dir       = '/usr/local'
    install_root_dir  = ''
    
    use_soqt = 'true'
    release_flags = ['-O2']
    debug_flags = ['-O2', '-g']
    extra_flags = ['-pipe']
    cxxflags = []
    defines = []
    warn_flags = ['-Wall', '-W']
    source_filter = '\.cxx$|\.cpp$'
    include_filter = '\.h$'
    ignore_filter = '/ignore/|/ignore$|\.scon|\.svn|\.#|bak|~$|SConscript'

    # OpenGL library information
    opengl_lib = ['GL', 'GLU']
    opengl_libpath = ['/usr/lib', '/usr/X11R6/lib']
    opengl_include = ['/usr/include', '/usr/X11R6/include']
    # ACE library information
    ace_env.ParseConfig ('pkg-config --cflags --libs ACE')
    ace_cflags = ace_env.Dictionary()['CCFLAGS']
    ace_include = ace_env.Dictionary()['CPPPATH']
    ace_lib = ace_env.Dictionary()['LIBS']
    ace_libpath = ace_env.Dictionary()['LIBPATH']
    # TinyXML library information
    tinyxml_env.ParseConfig ('pkg-config --cflags --libs tinyxml')
    tinyxml_cflags = tinyxml_env.Dictionary()['CCFLAGS']
    tinyxml_include = tinyxml_env.Dictionary()['CPPPATH']
    tinyxml_lib = tinyxml_env.Dictionary()['LIBS']
    tinyxml_libpath = tinyxml_env.Dictionary()['LIBPATH']
    # Coin library information
    coin_env.ParseConfig ('coin-config --ldflags --cxxflags --libs')
    coin_cflags = coin_env.Dictionary()['CCFLAGS']
    coin_include = coin_env.Dictionary()['CPPPATH']
    coin_lib = coin_env.Dictionary()['LIBS']
    coin_libpath = coin_env.Dictionary()['LIBPATH']
    # Soqt library information
    soqt_env.ParseConfig ('soqt-config --cppflags --ldflags --libs')
    soqt_cflags = soqt_env.Dictionary()['CCFLAGS']
    soqt_include = soqt_env.Dictionary()['CPPPATH']
    soqt_lib = soqt_env.Dictionary()['LIBS']
    soqt_libpath = soqt_env.Dictionary()['LIBPATH']
    # Opentracker library information
    opentracker_env.ParseConfig ('opentracker-config --cppflags --libs')
    opentracker_cflags = opentracker_env.Dictionary()['CCFLAGS']
    opentracker_include = opentracker_env.Dictionary()['CPPPATH']
    opentracker_lib = opentracker_env.Dictionary()['LIBS']
    opentracker_libpath = opentracker_env.Dictionary()['LIBPATH']
    # Openvideo library information
    openvideo_env.ParseConfig ('pkg-config --cflags --libs OpenVideo')
    openvideo_cflags = openvideo_env.Dictionary()['CCFLAGS']
    openvideo_include = openvideo_env.Dictionary()['CPPPATH']
    openvideo_lib = openvideo_env.Dictionary()['LIBS']
    openvideo_libpath = openvideo_env.Dictionary()['LIBPATH']

    build_example_app = 'true'
    enable_openvideo = 'false'
else:
    print "Other platforms not supported by scons!"
    exit

#-----------------------------------------------------------------------------
# User configurable options to be saved in a config file.
#-----------------------------------------------------------------------------
# Checking for an existing config file - use that one if it exists,
# otherwise create one.
my_defines = []
my_ccflags = []
my_cxxflags = ['-Iinclude']
#my_ldflags = [' -L' + root_build_dir + '/lib ']
my_ldflags = []

if os.path.exists (config_file):
	print "Using config file: " + config_file
else:
	print "Creating new config file: " + config_file
	env_dict = env.Dictionary()
	config=open (config_file, 'w')
	config.write ("# Configuration file containing user definable options.\n")
	config.write ("VERSION = %r\n"%(version))
	config.write ("BUILD_BINARY = 'release'\n")
	config.write ("BUILD_DIR = %r\n"%(root_build_dir))
	config.write ("INSTALL_DIR = %r\n"%(install_dir))
	config.write ("INSTALL_ROOT_DIR = %r\n"%(install_root_dir))
	
	config.write ("\n# Extra compiler flags can be defined here.\n")
	config.write ("DEFINES = %s\n"%(my_defines))
	config.write ("CCFLAGS = %s\n"%(my_ccflags))
	config.write ("CXXFLAGS = %s\n"%(my_cxxflags))
	config.write ("LDFLAGS = %s\n"%(my_ldflags))

        config.write ("SOURCE_FILTER = %r\n"%(source_filter))
	config.write ("INCLUDE_FILTER = %r\n"%(include_filter))
        config.write ("IGNORE_FILTER = %r\n"%(ignore_filter))
        
        #config.write ("USE_OPENAL = %r\n"%(use_openal))
        #config.write ("USE_FMOD = %r\n"%(use_fmod))

        config.write ("USE_SOQT = %r\n"%(use_soqt))
        
        config.write ("\n# ACE library.\n")
        config.write ("ACE_CFLAGS = %r\n"%(ace_cflags))
	config.write ("ACE_INCLUDE = %r\n"%(ace_include))
	config.write ("ACE_LIBPATH = %r\n"%(ace_libpath))
	config.write ("ACE_LIBRARY = %r\n"%(ace_lib))

        config.write ("\n# TinyXML library.\n")
        config.write ("TINYXML_CFLAGS = %r\n"%(tinyxml_cflags))
	config.write ("TINYXML_INCLUDE = %r\n"%(tinyxml_include))
	config.write ("TINYXML_LIBPATH = %r\n"%(tinyxml_libpath))
	config.write ("TINYXML_LIBRARY = %r\n"%(tinyxml_lib))

        config.write ("\n# Coin library.\n")
        config.write ("COIN_CFLAGS = %r\n"%(coin_cflags))
	config.write ("COIN_INCLUDE = %r\n"%(coin_include))
	config.write ("COIN_LIBPATH = %r\n"%(coin_libpath))
	config.write ("COIN_LIBRARY = %r\n"%(coin_lib))

        config.write ("\n# Soqt library.\n")
        config.write ("SOQT_CFLAGS = %r\n"%(soqt_cflags))
	config.write ("SOQT_INCLUDE = %r\n"%(soqt_include))
	config.write ("SOQT_LIBPATH = %r\n"%(soqt_libpath))
	config.write ("SOQT_LIBRARY = %r\n"%(soqt_lib))

        config.write ("\n# OpenTracker library.\n")
        config.write ("OPENTRACKER_CFLAGS = %r\n"%(opentracker_cflags))
	config.write ("OPENTRACKER_INCLUDE = %r\n"%(opentracker_include))
	config.write ("OPENTRACKER_LIBPATH = %r\n"%(opentracker_libpath))
	config.write ("OPENTRACKER_LIBRARY = %r\n"%(opentracker_lib))

        config.write ("\n# OpenVideo library.\n")
        config.write ("OPENVIDEO_CFLAGS = %r\n"%(openvideo_cflags))
	config.write ("OPENVIDEO_INCLUDE = %r\n"%(openvideo_include))
	config.write ("OPENVIDEO_LIBPATH = %r\n"%(openvideo_libpath))
	config.write ("OPENVIDEO_LIBRARY = %r\n"%(openvideo_lib))

	config.write ("OPENGL_INCLUDE = %r\n"%(opengl_include))
	config.write ("OPENGL_LIBPATH = %r\n"%(opengl_libpath))
	config.write ("OPENGL_LIBRARY = %r\n"%(opengl_lib))

        config.write ("BUILD_EXAMPLE_APP = %r\n"%(build_example_app))
        config.write ("ENABLE_OPENVIDEO = %r\n"%(enable_openvideo))
        config.close ()

#-----------------------------------------------------------------------------
# Read the options from the config file and update the various necessary flags
#-----------------------------------------------------------------------------
list_opts = []
user_options = Options (config_file, ARGUMENTS)
user_options_env = Environment (ENV = os.environ, options = user_options)
user_options.AddOptions (
		('VERSION', 'Studierstube version', version),
		(EnumOption ('BUILD_BINARY', 'release',
					'Select a release or debug binary.',
					 allowed_values = ('release', 'debug'))),
		('BUILD_DIR', 'Target directory for intermediate files.',
					root_build_dir),
		('INSTALL_DIR', 'Target directory for installed files.',
					install_dir),
		('INSTALL_ROOT_DIR', 'Target directory for building packages.',
					install_root_dir),
		#(BoolOption ('USE_OPENAL',
		#			'Set to 1 to build the game engine with OpenAL support.',
		#			'false')),
		#(BoolOption ('USE_FMOD',
		#			'Set to 1 to build the game engine with FMod support.',
		#			'false')),
		(BoolOption ('USE_SOQT',
					'Set to 1 to use SoQt (default).',
					'true')),
		('ACE_CFLAGS', 'Necessary CFLAGS when using ACE functionality.'),
		('ACE_INCLUDE', 'Include directory for ACE header files.'),
		('ACE_LIBPATH', 'Library path where the ACE library is located.'),
		('ACE_LIBRARY', 'ACE library name.'),
		('COIN_CFLAGS', 'Necessary CFLAGS when using COIN functionality.'),
		('COIN_INCLUDE', 'Include directory for COIN header files.'),
		('COIN_LIBPATH', 'Library path where the COIN library is located.'),
		('COIN_LIBRARY', 'COIN library name.'),
		('SOQT_CFLAGS', 'Necessary CFLAGS when using SOQT functionality.'),
		('SOQT_INCLUDE', 'Include directory for SOQT header files.'),
		('SOQT_LIBPATH', 'Library path where the SOQT library is located.'),
		('SOQT_LIBRARY', 'SOQT library name.'),
		('OPENTRACKER_CFLAGS', 'Necessary CFLAGS when using OPENTRACKER functionality.'),
		('OPENTRACKER_INCLUDE', 'Include directory for OPENTRACKER header files.'),
		('OPENTRACKER_LIBPATH', 'Library path where the OPENTRACKER library is located.'),
		('OPENTRACKER_LIBRARY', 'OPENTRACKER library name.'),
		('OPENVIDEO_CFLAGS', 'Necessary CFLAGS when using OPENVIDEO functionality.'),
		('OPENVIDEO_INCLUDE', 'Include directory for OPENVIDEO header files.'),
		('OPENVIDEO_LIBPATH', 'Library path where the OPENVIDEO library is located.'),
		('OPENVIDEO_LIBRARY', 'OPENVIDEO library name.'),
		('TINYXML_CFLAGS', 'Necessary CFLAGS when using TINYXML functionality.'),
		('TINYXML_INCLUDE', 'Include directory for TINYXML header files.'),
		('TINYXML_LIBPATH', 'Library path where the TINYXML library is located.'),
		('TINYXML_LIBRARY', 'TINYXML library name.'),
		('DEFINES', 'Extra Preprocessor defines.'),
		('CCFLAGS', 'Extra C Compiler flags.'),
		('CXXFLAGS','Extra C++ Compiler flags.'),
		('LDFLAGS', 'Extra Linker flags.'),
                ('SOURCE_FILTER', 'Specify the source filter for compiling sources'),
                ('INCLUDE_FILTER', 'Specify the header files which should be considered'),
                ('IGNORE_FILTER', 'Specify the ignore filter for excluding source files'),
		(BoolOption ('BUILD_EXAMPLE_APP',
					'Set to 1 to build the example application.',
					'true')),
		(BoolOption ('ENABLE_OPENVIDEO',
					'Set to 1 to use the OpenVideo library.',
					'false'))
	)
user_options.Update (user_options_env)
user_options_dict = user_options_env.Dictionary()
Help(user_options.GenerateHelpText(user_options_env))
root_build_dir = user_options_dict['BUILD_DIR']

if user_options_dict['USE_SOQT'] == 1:
    defines += ['USE_SOQT']

if user_options_dict['BUILD_BINARY'] == 'release':
	cflags = extra_flags + release_flags + warn_flags
	defines += ['NDEBUG']
else:
	cflags = extra_flags + debug_flags + warn_flags

defines += user_options_dict['DEFINES']
defines += ['LINUX']
cflags += user_options_dict['CCFLAGS']
cxxflags += user_options_dict['CXXFLAGS']

# Extract defines for pkg-config file
global_defs = ''
for gd in defines:
    global_defs += '-D' + gd + ' '

# Create the build directory. SCons does this automatically, but since we
# don't want to put scons-generated .sconsign files in the source tree, but in
# the root_build_dir, we have to create that dir ourselves before SCons tries
# to access/create the file containing .sconsign data.
if os.path.isdir (root_build_dir) == 0:
	os.makedirs (root_build_dir)
	os.makedirs (root_build_dir+os.sep+'src')

#****************************************************************************
# Set project details used in the package-config (.pc) file
# See OpenVideo.pc.in
#****************************************************************************
env['STB_PROJECT_NAME']        = "Studierstube"
env['STB_PROJECT_DESCRIPTION'] = "Studierstube - The Augmented Reality Environment"
env['STB_PROJECT_VERSION']     = "4.0"
env['STB_PROJECT_LIBNAME']     = "stbkernel"
env['STB_INSTALL_DIR']         = install_dir
env['STB_PROJECT_DEFINES']     = global_defs

user_options_dict['INSTALL_DIR'] = install_root_dir + install_dir

buildutils.appendbuilders(env)
outname = env.AlwaysBuild(env.Substitute('stb.pc', 'stb.pc.in'))
ic = env.Alias(target = ["install-config"], source = env.AlwaysBuild(env.Install(dir = user_options_dict['INSTALL_DIR']+'/lib/pkgconfig', source = outname)))
il = env.Alias('install-lib', user_options_dict['INSTALL_DIR'] + '/lib')
ib = env.Alias('install-bin', user_options_dict['INSTALL_DIR'] + '/bin')
ih = env.Alias('install-header', user_options_dict['INSTALL_DIR'] + '/include')
iss = env.Alias('install-share', os.sep+user_options_dict['INSTALL_DIR'] + '/share')
env.Alias('install', [ic, il, ib, ih, iss])

#****************************************************************************
# Specify source filters
#****************************************************************************
#env['SOURCE_FILTER']  = '\.cxx$|\.cpp$'
#env['INCLUDE_FILTER'] = '\.h$'
#env['IGNORE_FILTER']  = '/ignore/|/ignore$|\.scon|\.svn|\.#|bak|~$|SConscript'

#-----------------------------------------------------------------------------
# Generic library generation environment. This one is the basis for each
# library.
#-----------------------------------------------------------------------------
library_env = env.Copy ()
library_env.Append (CCFLAGS = cflags)
library_env.Append (CXXFLAGS = cxxflags)
library_env.Append (LIBPATH = user_options_dict['LDFLAGS'])
library_env.Append (CPPDEFINES = defines)
library_env.SConsignFile (root_build_dir+'scons-signatures')

libpath = (['#'+root_build_dir+'/lib'])
library_env.Append (LIBPATH = libpath)

#-----------------------------------------------------------------------------
# Settings to be exported to other SConscript files
#-----------------------------------------------------------------------------

Export ('cflags')
Export ('defines')
Export ('user_options_dict')
Export ('library_env')

#****************************************************************************
# Build by executing SConscript file
#****************************************************************************

# Print welcoming message that makes you feel warm and fuzzy inside;)
print "\n...Building...\n"

BuildDir (root_build_dir+'/src', 'src', duplicate=0)
SConscript (root_build_dir+'/src/SConscript')
SConscript (['include/SConscript'])
SConscript (['bin/linux/SConscript'])



###########################################################################
# Done:)
###########################################################################

