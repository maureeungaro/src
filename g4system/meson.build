sub_dir_name = meson.current_source_dir().split('/').get(-1)
LD += {
    'name' : sub_dir_name,
    'sources' : files(
                          'g4volume.cc',
                          'g4world.cc',
                          'g4systemOptions.cc',
                          'g4objectsFactories/geant4Dependencies.cc',
                          'g4objectsFactories/g4objectsCreators.cc',
                          'g4objectsFactories/g4native/g4SolidConstructorDescription.cc',
                          'g4objectsFactories/g4native/checkAndReturnParameters.cc',
                          'g4objectsFactories/g4native/buildSolid.cc',
                          'g4objectsFactories/g4native/buildLogical.cc',
                          'g4objectsFactories/g4native/buildPhysical.cc',
                          'g4objectsFactories/cad/buildSolid.cc',
                          'g4objectsFactories/cad/buildLogical.cc',
                          'g4objectsFactories/cad/buildPhysical.cc'
                      ),
    'headers' : files(
                          'g4volume.h',
                          'g4world.h',
                          'g4systemOptions.h',
                          'g4systemConventions.h',
                          'g4objectsFactories/g4objectsFactory.h',
                          'g4objectsFactories/g4native/g4NativeObjectsFactory.h',
                          'g4objectsFactories/cad/CADMesh.hh',
                          'g4objectsFactories/cad/cadSystemFactory.h',
                      ),
    'dependencies' : [yaml_cpp_dep, clhep_deps, geant4_deps, assimp_dep],
    'additional_includes' : ['g4system/g4objectsFactories', 'g4system/g4objectsFactories/g4native', 'g4system/g4objectsFactories/cad'],
    'examples' : {
        'test_basic_g4system' : [files('examples/basic_example.cc'), ['']],
    }
}
