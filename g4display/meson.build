sub_dir_name = meson.current_source_dir().split('/').get(-1)
LD += {
    'name' : sub_dir_name,
    'sources' : files(
        'g4displayOptions.cc',
        'g4SceneProperties.cc',
        'g4Text.cc',
        'g4controls.cc',
        'tabs/g4displaycamera.cc',
        'tabs/g4displayslice.cc'
    ),
    'headers' : files(
        'g4displayOptions.cc',
        'g4SceneProperties.cc',
        'g4Text.cc',
        'g4controls.cc',
        'tabs/g4displaycamera.cc',
        'tabs/g4displayslice.cc'
    ),
    'moc_headers' : files(
        'tabs/g4displaycamera.h',
        'tabs/g4displayslice.h'
    ),
    'dependencies' : [yaml_cpp_dep, qt5_deps, clhep_deps, geant4_deps],
    'additional_includes' : ['g4display/tabs'],
    'examples' : {
        'test_basic_display' : [files('examples/example.cc'),  meson.current_source_dir() + '/examples/example.yaml'],
    }
}
