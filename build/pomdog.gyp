{
  'includes': ['common.gypi'],
  'make_global_settings': [
    ['CXX','/usr/bin/clang++'],
    ['LINK','/usr/bin/clang++'],
  ],
  'conditions': [
    ['OS == "win"', {
      'variables': {
        'renderer%': 'd3d11',
        #'input_device%': 'dinput8',
      },
    }],
    ['OS == "mac"', {
      'variables': {
        'renderer%': 'gl',
		#'input_device%': 'none',
      },
    }],
    ['OS == "linux"', {
      'variables': {
        'renderer%': 'gl',
		#'input_device%': 'none',
      },
    }],
  ],
  'target_defaults': {
    'dependencies': [
      #'glew.gyp:glew',
    ],
    'include_dirs': [
      '../include',
      '../third_party/boost/include',
    ],
    'msvs_settings':{
      'VCCLCompilerTool': {
        'WarningLevel': '4',   # /W4
        'WarnAsError': 'true', # /WX
      },
    },
    'xcode_settings': {
      'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',
      'CLANG_CXX_LANGUAGE_STANDARD': 'c++0x',
      'MACOSX_DEPLOYMENT_TARGET': '10.8', # OS X Deployment Target: 10.8
    },
  },
  'targets': [
    {
      'target_name': 'pomdog-core',
      'product_name': 'PomdogCore',
      'type': 'static_library',
      'include_dirs': [
        '../include',
      ],
      'sources': [
        '../include/Pomdog/Config/FundamentalTypes.hpp',
        '../include/Pomdog/Config/Platform.hpp',
        '../include/Pomdog/Event/Event.hpp',
        '../include/Pomdog/Event/EventCode.hpp',
        '../include/Pomdog/Event/EventCodeHelper.hpp',
        '../include/Pomdog/Event/EventConnection.hpp',
        '../include/Pomdog/Event/EventHandler.hpp',
        '../include/Pomdog/Event/EventQueue.hpp',
        '../include/Pomdog/Event/ScopedConnection.hpp',
        '../include/Pomdog/Event/detail/EventArguments.hpp',
        '../include/Pomdog/GamePlay/GameObject.hpp',
        '../include/Pomdog/GamePlay/detail/GameComponent.hpp',
        '../include/Pomdog/Logging/Log.hpp',
        '../include/Pomdog/Logging/LogChannel.hpp',
        '../include/Pomdog/Logging/LogEntry.hpp',
        '../include/Pomdog/Logging/LoggingLevel.hpp',
        '../include/Pomdog/Logging/LogStream.hpp',
        '../include/Pomdog/Math/Color.hpp',
        '../include/Pomdog/Math/ContainmentType.hpp',
        '../include/Pomdog/Math/Degree.hpp',
        '../include/Pomdog/Math/MathHelper.hpp',
        '../include/Pomdog/Math/Matrix2x2.hpp',
        '../include/Pomdog/Math/Matrix3x3.hpp',
        '../include/Pomdog/Math/Matrix4x4.hpp',
        '../include/Pomdog/Math/Point2D.hpp',
        '../include/Pomdog/Math/Point3D.hpp',
        '../include/Pomdog/Math/Quaternion.hpp',
        '../include/Pomdog/Math/Radian.hpp',
        '../include/Pomdog/Math/Rectangle.hpp',
        '../include/Pomdog/Math/Vector2.hpp',
        '../include/Pomdog/Math/Vector3.hpp',
        '../include/Pomdog/Math/Vector4.hpp',
        '../include/Pomdog/Math/detail/Coordinate2D.hpp',
        '../include/Pomdog/Math/detail/Coordinate2DImplementation.hpp',
        '../include/Pomdog/Math/detail/Coordinate3D.hpp',
        '../include/Pomdog/Math/detail/Coordinate3DImplementation.hpp',
        '../include/Pomdog/Math/detail/FloatingPointColor.hpp',
        '../include/Pomdog/Math/detail/FloatingPointMatrix2x2.hpp',
        '../include/Pomdog/Math/detail/FloatingPointMatrix3x3.hpp',
        '../include/Pomdog/Math/detail/FloatingPointMatrix4x4.hpp',
        '../include/Pomdog/Math/detail/FloatingPointQuaternion.hpp',
        '../include/Pomdog/Math/detail/FloatingPointVector2.hpp',
        '../include/Pomdog/Math/detail/FloatingPointVector3.hpp',
        '../include/Pomdog/Math/detail/FloatingPointVector4.hpp',
        '../include/Pomdog/Math/detail/ForwardDeclarations.hpp',
        '../include/Pomdog/Math/detail/TaggedArithmetic.hpp',
        '../include/Pomdog/Utility/Assert.hpp',
        '../include/Pomdog/Utility/Exception.hpp',
        '../include/Pomdog/Utility/Noncopyable.hpp',
        '../include/Pomdog/Utility/detail/Tagged.hpp',
        '../src/Event/Event.cpp',
        '../src/Event/EventCodeHelper.cpp',
        '../src/Event/EventConnection.cpp',
        '../src/Event/EventHandler.cpp',
        '../src/Event/EventQueue.cpp',
        '../src/Event/detail/EventSlot.cpp',
        '../src/Event/detail/EventSlot.hpp',
        '../src/Event/detail/EventSlotCollection.cpp',
        '../src/Event/detail/EventSlotCollection.hpp',
        '../src/GamePlay/GameObject.cpp',
        '../src/Logging/Log.cpp',
        '../src/Logging/LogChannel.cpp',
        '../src/Logging/LogStream.cpp',
        '../src/Math/MathHelper.cpp',
        '../src/Math/Rectangle.cpp',
        '../src/Math/detail/FloatingPointColor.cpp',
        '../src/Math/detail/FloatingPointMatrix2x2.cpp',
        '../src/Math/detail/FloatingPointMatrix3x3.cpp',
        '../src/Math/detail/FloatingPointMatrix4x4.cpp',
        '../src/Math/detail/FloatingPointQuaternion.cpp',
        '../src/Math/detail/FloatingPointVector2.cpp',
        '../src/Math/detail/FloatingPointVector3.cpp',
        '../src/Math/detail/FloatingPointVector4.cpp',
        '../src/Utility/CRC32.cpp',
        '../src/Utility/CRC32.hpp',
        '../src/Utility/HashingHelper.hpp',
        '../src/Utility/ScopeGuard.hpp',
      ],
      'xcode_settings': {
        'OTHER_CPLUSPLUSFLAGS': ['-std=c++11','-stdlib=libc++'],
      },
    },
    {
      'target_name': 'pomdog',
      'product_name': 'PomdogEngine',
      'type': 'static_library',
      'dependencies': [
        'pomdog-core',
      ],
      'include_dirs': [
        '../include',
      ],
      'sources': [
        '../include/Pomdog/Application/Bootstrapper.hpp',
        '../include/Pomdog/Application/GameSystem.hpp',
        '../include/Pomdog/Application/detail/SystemHub.hpp',
        '../include/Pomdog/Graphics/GraphicsContext.hpp',
        '../include/Pomdog/Graphics/Viewport.hpp',
        '../src/Application/GameSystem.cpp',
        '../src/Application/detail/SystemHub.cpp',
        '../src/Graphics/GraphicsContext.cpp',
        '../src/Graphics/Viewport.cpp',
        '../src/RenderSystem/NativeGraphicsContext.hpp',
        '../src/RenderSystem/InternalAttorney.hpp',
      ],
      'conditions': [
        ['renderer == "d3d11"', {
          'sources': [
          ],
          'link_settings': {
            'libraries': [
              '-ldxgi.lib',
              '-ld3d11.lib',
              '-ld3dcompiler.lib',
            ],
          },
        }],
        ['renderer == "gl"', {
          'sources': [
            '../src/GL4/GL4Prerequisites.hpp',
            '../src/GL4/GLContext.hpp',
            '../src/GL4/GraphicsContextGL4.cpp',
            '../src/GL4/GraphicsContextGL4.hpp',
          ],
        }],
        ['OS == "mac" and renderer == "gl"', {
          'sources': [
            #'../src/OSX/CocoaOpenGLView.h',
            #'../src/OSX/CocoaOpenGLView.mm',
            '../src/OSX/GLContextOSX.hpp',
            '../src/OSX/GLContextOSX.mm',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
              '$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
            ],
          },
        }], # OS == "mac"
      ],
      'xcode_settings': {
        'CLANG_ENABLE_OBJC_ARC': 'YES',
        'OTHER_CPLUSPLUSFLAGS': ['-std=c++11','-stdlib=libc++'],
      },
    },
  ],# "targets"
}
