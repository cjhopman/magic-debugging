{
  'variables': {
    'magic_enabled%': 0,
    'android_app_targets': [
      '<(DEPTH)/magic/targets.gyp:*',
    ],
    'ABS_MAGIC': '<!(cd <(DEPTH)/magic && pwd)',

    # TODO(cjhopman): mac_pch fails because the magic.h -include appears in the
    # command before precompiled headers. Fix that instead of disabling pch.
    'chromium_mac_pch': 0,
  },
  'conditions': [
    ['magic_enabled == 1', {
      'target_defaults': {
        'variables': {
          'magic_disabled_targets': [
            'pdfium',
            'fxcodec',
            'fpdfapi',
            'harfbuzz-ng',
            'magic_library',
            'magic_java',
            'protobuf_lite',
            'libyuv',

            'yasm', # yasm uses cflags that cause compilation to fail
          ],
        },
        'target_conditions': [
          [('_toolset=="target" '
           'and not _target_name in magic_disabled_targets '
           'and not "pdfium" in _target_name ')
           , {
            'variables': {
              'input_jars_paths': ['<(PRODUCT_DIR)/lib.java/magic_java.jar'],
              'library_dexed_jars_paths': ['<(PRODUCT_DIR)/lib.java/magic_java.dex.jar'],
            },
            'include_dirs': [
              '<(ABS_MAGIC)',
            ],
            'conditions': [
              ['OS=="mac"', {
                'xcode_settings': {
                  'OTHER_CPLUSPLUSFLAGS': [
                    '-include', 'magic.h',
                  ],
                },
              }, {
                'cflags_cc': [
                  '-include', 'magic.h',
                ],
              }],
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
