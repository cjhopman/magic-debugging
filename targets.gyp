{
  'targets': [
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
