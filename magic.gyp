{
  'variables': {
    'android_app_targets': [
      '<(magic_path)/targets.gyp:*',
    ],
  },
  'target_defaults': {
    'variables': {
      'magic_disabled_targets': [
        'magic_android',
        'protobuf_lite',
        'libyuv',
        'skia',
      ],
    },
    'target_conditions': [
      ['_toolset=="target" and not _target_name in magic_disabled_targets', {
        'cflags_cc': [
          '-Wno-gnu',
          '-isystem', '<(magic_path)', # don't care about warnings in the magic library
          '-include', 'magic.h',
        ],
      }],
      ['_toolset=="target" and (_type=="shared_library" or _type=="executable") and not _target_name in magic_disabled_targets', {
        'conditions': [
          ['OS=="android"', {
            'dependencies': [
              '<(magic_path)/targets.gyp:magic_android#target',
            ],
          }, {
            # Non-android builds don't provide a hook in build/all.gyp to
            # inform gyp of new targets, so we have to compile magic.cc
            # directly into shared libraries. This means that the indent level
            # is not shared across different libraries. TODO: maybe tag lines
            # from different libraries so that they can be differentiated.
            'sources': [
              '<(magic_path)/magic.cc',
            ],
          }],
        ],
      }],
    ]
  },
}
