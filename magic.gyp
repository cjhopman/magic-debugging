{
  'variables': {
    'magic_enabled%': 0,
    'android_app_targets': [
      '<(DEPTH)/magic/targets.gyp:*',
    ],
    'ABS_MAGIC': '<!(cd <(DEPTH)/magic && pwd)',
  },
  'conditions': [
    ['magic_enabled == 1', {
      'target_defaults': {
        'variables': {
          'magic_disabled_targets': [
            'magic_library',
            'protobuf_lite',
            'libyuv',
          ],
        },
        'target_conditions': [
          ['_toolset=="target" and not _target_name in magic_disabled_targets', {
            'include_dirs': [
              '<(ABS_MAGIC)',
            ],
            'cflags_cc': [
              '-include', 'magic.h',
            ],
          }],
          ['_toolset=="target" and (_type=="shared_library" or _type=="executable") and not _target_name in magic_disabled_targets', {
            'conditions': [
              ['OS=="android"', {
                'dependencies': [
                  '<(ABS_MAGIC)/targets.gyp:magic_library#target',
                ],
              }, {
                # Non-android builds don't provide a hook in build/all.gyp to
                # inform gyp of new targets, so we have to compile magic.cc
                # directly into shared libraries. This means that the indent level
                # is not shared across different libraries. TODO: maybe tag lines
                # from different libraries so that they can be differentiated.
                'sources': [
                  '<(DEPTH)/magic/magic.cc',
                ],
              }],
            ],
          }],
        ]
      },
    }],
  ],
}
