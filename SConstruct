import os
import sys
import buildutils

#****************************************************************************
# The options are saved to a file and reapplied automatically
#****************************************************************************
opts = Options(['build.opts'], ARGUMENTS)
opts.Add('INSTALLDIR'                           , 'Sets the project installation directory', '/usr/local')
opts.Add(BoolOption('STB_DEBUG'                 , 'Enables debugging', 0))

#****************************************************************************
# Set up environment and save options to disk
#****************************************************************************
env = Environment(options = opts, ENV = os.environ)
opts.Save('build.opts', env)

#****************************************************************************
# Make sure ACE is available
#****************************************************************************
conf = Configure(env)
if conf.TryAction('pkg-config --exists ACE')[0]:
    env.ParseConfig('pkg-config --cflags --libs ACE')
else:
    print "****************************************************************************"
    print "ERROR: You need to have ACE installed and ACE.pc in your PKG_CONFIG_PATH"
    print "Make sure you installed ACE using the ./configure && make && make install"
    print "process..."
    print "****************************************************************************"
    sys.exit(-1)

#****************************************************************************
# Make sure Coin is available
#****************************************************************************
if conf.TryAction('coin-config')[0]:
    env.ParseConfig('coin-config --cxxflags --libs')
else:
    print "****************************************************************************"
    print "ERROR: You need to have Coin installed, coin-config is not available"
    print "Make sure you installed Coin on your system"
    print "****************************************************************************"
    sys.exit(-1)

#****************************************************************************
# Make sure TinyXML is available
#****************************************************************************
if conf.TryAction('pkg-config --exists tinyxml')[0]:
    env.ParseConfig('pkg-config --cflags --libs tinyxml')
else:
    print "****************************************************************************"
    print "ERROR: You need to have TinyXml installed and tinyxml.pc in your PKG_CONFIG_PATH"
    print "Make sure you installed TinyXml from CVS using scons install"
    print "****************************************************************************"
    sys.exit(-1)

if env['STB_DEBUG']:
    env.AppendUnique(CPPDEFINES = ['STB_DEBUG=1'])

conf.Finish()

# Need to specify where to look for local include files
env.AppendUnique(CPPPATH = [Dir('#').abspath + os.sep + 'include'])

# Add the platform to the defines. Treat all linux platforms in common.
if sys.platform.upper().startswith('LINUX'):
    env.AppendUnique(CPPDEFINES = ['LINUX'])
    env.AppendUnique(CPPDEFINES = ['USE_SOQT'])
else:
    env.AppendUnique(CPPDEFINES = [sys.platform.upper()])

#****************************************************************************
# Set project details used in the package-config (.pc) file
# See OpenVideo.pc.in
#****************************************************************************
env['STB_PROJECT_NAME']        = "Studierstube"
env['STB_PROJECT_DESCRIPTION'] = "Studierstube - The Augmented Reality Environment"
env['STB_PROJECT_VERSION']     = "4.0.0"
env['STB_PROJECT_LIBNAME']     = "stbkernel"

buildutils.appendbuilders(env)
outname = env.AlwaysBuild(env.Substitute('stb.pc', 'stb.pc.in'))
env.Alias(target = ["install"], source = env.AlwaysBuild(env.Install(dir = '${INSTALLDIR}/lib/pkgconfig', source = outname)))

#****************************************************************************
# Generate string of defines
#****************************************************************************
defines = ''
if env.has_key('CPPDEFINES'):
    for define in env['CPPDEFINES']:
        defines += '-D' + define + ' '

env['STB_PROJECT_DEFINES'] = defines
#****************************************************************************
# Generate help message
#****************************************************************************
Help(opts.GenerateHelpText(env))

#****************************************************************************
# Specify source filters
#****************************************************************************
env['SOURCE_FILTER']  = '\.cxx$|\.cpp$'
env['INCLUDE_FILTER'] = '\.h$'
env['IGNORE_FILTER']  = '/ignore/|/ignore$|\.scon|\.svn|\.#|bak|~$|SConscript'

#****************************************************************************
# Build by executing SConscript file
#****************************************************************************

# Print welcoming message that makes you feel warm and fuzzy inside;)
print "\n...Building...\n"

# Call subsidiary build scripts.
env['BUILDDIR'] = 'build' + os.sep + sys.platform
SConscript(['src/SConscript'], exports='env', build_dir=env['BUILDDIR'])
SConscript(['include/SConscript'], exports='env')
SConscript(['bin/linux/SConscript'], exports='env')

###########################################################################
# Done:)
###########################################################################

