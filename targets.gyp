{
  'targets': [
    {
      'target_name': 'magic_java',
      'type': 'none',
      'variables': {
        'java_in_dir': './',
      },
      'includes': ['../build/java.gypi'],
    },

    {
      'target_name': 'magic_library',
      'type': '<(component)',
      'sources': [
        'magic.cc',
      ],
      'defines': [
        'MAGIC_IMPLEMENTATION',
      ],
      'link_settings': {
        'libraries': [
          '-llog',
        ],
      },
    },
  ],
}
