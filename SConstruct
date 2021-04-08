env = Environment()

conan = env.SConscript("SConscript_conan")
if not conan:
    exit(1)
else:
    env.MergeFlags(conan["conan"])

if env['PLATFORM'] == 'nt':
    env.Append(CXXFLAGS=["-MD"])

env.VariantDir('build', 'src', duplicate=0)
langton_bin = env.SConscript('build/SConscript', exports='env')

Install("bin", langton_bin)