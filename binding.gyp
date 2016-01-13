{
  "targets": [
    {
      "target_name": "texttoimage",
      "sources": [
        "src/NodeWrapper.cpp"
      ],
      "libraries": [
        "<!@(pkg-config cairo --libs)"
      ],
      "include_dirs": [
        '<!(node -e "require(\'nan\');")',
        "<!@(pkg-config cairo --cflags-only-I | sed s/-I//g)"
      ],
      'cflags_cc': [
        '-std=c++0x',
        '-fexceptions',
        '-Wall',
        '-O3'
      ],
      'xcode_settings': {
        'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7',
        'OTHER_CPLUSPLUSFLAGS': [
         '-fexceptions',
         '-Wall',
         '-O3'
        ]
      }
    }
  ]
}