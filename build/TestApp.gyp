# Examples(Xcode):
# gyp build/TestApp.gyp --depth=. -f xcode --generator-output=./build.xcodefiles/
#
# Examples(MSVS 2013):
# gyp build/TestApp.gyp --depth=. -f msvs -G msvs_version=2013 --generator-output=./build.msvc/

{
  'includes': ['common.gypi'],
  'make_global_settings': [
    ['CXX','/usr/bin/clang++'],
    ['LINK','/usr/bin/clang++'],
  ],
  'target_defaults': {
    'include_dirs': [
      '../include',
    ],
    'configurations': {
      'Debug': {
        'msvs_settings': {
          'VCLinkerTool': {
            'GenerateDebugInformation': 'true', # /DEBUG
          },
        },
      }, # Debug
    },
    'msvs_settings':{
      'VCCLCompilerTool': {
        'WarningLevel': '4',   # /W4
        'WarnAsError': 'true', # /WX
      },
    },
    'xcode_settings': {
      'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',
      'CLANG_CXX_LANGUAGE_STANDARD': 'c++0x',
      'MACOSX_DEPLOYMENT_TARGET': '10.9', # OS X Deployment Target: 10.9
      'CLANG_CXX_LIBRARY': 'libc++', # libc++ requires OS X 10.7 or later
	  #'SKIP_INSTALL': 'NO',
      # Warnings:
      'CLANG_WARN_EMPTY_BODY': 'YES',
      'GCC_WARN_64_TO_32_BIT_CONVERSION': 'YES',
      'GCC_WARN_ABOUT_DEPRECATED_FUNCTIONS': 'YES',
      'GCC_WARN_ABOUT_MISSING_FIELD_INITIALIZERS': 'YES',
      'GCC_WARN_ABOUT_MISSING_NEWLINE': 'YES',
      'GCC_WARN_ABOUT_RETURN_TYPE': 'YES',
      'GCC_WARN_CHECK_SWITCH_STATEMENTS': 'YES',
      'GCC_WARN_HIDDEN_VIRTUAL_FUNCTIONS': 'YES',
      #'GCC_WARN_INITIALIZER_NOT_FULLY_BRACKETED': 'YES',
      'GCC_WARN_MISSING_PARENTHESES': 'YES',
      'GCC_WARN_NON_VIRTUAL_DESTRUCTOR': 'YES',
      'GCC_WARN_SHADOW': 'YES',
      'GCC_WARN_SIGN_COMPARE': 'YES',
      'GCC_WARN_TYPECHECK_CALLS_TO_PRINTF': 'YES',
      'GCC_WARN_UNINITIALIZED_AUTOS': 'YES',
      'GCC_WARN_UNKNOWN_PRAGMAS': 'YES',
      'GCC_WARN_UNUSED_FUNCTION': 'YES',
      'GCC_WARN_UNUSED_LABEL': 'YES',
      'GCC_WARN_UNUSED_VALUE': 'YES',
      'GCC_WARN_UNUSED_VARIABLE': 'YES',
      'GCC_TREAT_WARNINGS_AS_ERRORS': 'YES',
    },
  },
  'targets': [
    {
      'target_name': 'testapp',
      'product_name': 'TestApp',
      'type': 'executable',
      'mac_bundle': 1,
      'include_dirs': [
        '../include',
        '../third_party/rapidjson/include',
      ],
      'sources': [
        '../test/TestApp/Particle2D/Beam.hpp',
        '../test/TestApp/Particle2D/BeamEmitter.hpp',
        '../test/TestApp/Particle2D/BeamSystem.cpp',
        '../test/TestApp/Particle2D/BeamSystem.hpp',
        '../test/TestApp/Particle2D/Particle.hpp',
        '../test/TestApp/Particle2D/ParticleCurveKey.hpp',
        '../test/TestApp/Particle2D/ParticleCurveLerp.hpp',
        '../test/TestApp/Particle2D/ParticleEmitter.hpp',
        '../test/TestApp/Particle2D/ParticleEmitterShape.hpp',
        '../test/TestApp/Particle2D/ParticleEmitterShapeBox.hpp',
        '../test/TestApp/Particle2D/ParticleEmitterShapeSector.hpp',
        '../test/TestApp/Particle2D/ParticleParameter.hpp',
        '../test/TestApp/Particle2D/ParticleParameterConstant.hpp',
        '../test/TestApp/Particle2D/ParticleParameterCurve.hpp',
        '../test/TestApp/Particle2D/ParticleParameterRandom.hpp',
        '../test/TestApp/Particle2D/ParticleParameterRandomCurves.hpp',
        '../test/TestApp/Particle2D/ParticleSystem.cpp',
        '../test/TestApp/Particle2D/ParticleSystem.hpp',
        '../test/TestApp/Skeletal2D/AnimationClip.cpp',
        '../test/TestApp/Skeletal2D/AnimationClip.hpp',
        '../test/TestApp/Skeletal2D/AnimationKeyHelper.hpp',
        '../test/TestApp/Skeletal2D/AnimationState.cpp',
        '../test/TestApp/Skeletal2D/AnimationState.hpp',
        '../test/TestApp/Skeletal2D/AnimationSystem.cpp',
        '../test/TestApp/Skeletal2D/AnimationSystem.hpp',
        '../test/TestApp/Skeletal2D/AnimationTimeInterval.hpp',
        '../test/TestApp/Skeletal2D/AnimationTimer.cpp',
        '../test/TestApp/Skeletal2D/AnimationTimer.hpp',
        '../test/TestApp/Skeletal2D/AnimationTrack.cpp',
        '../test/TestApp/Skeletal2D/AnimationTrack.hpp',
        '../test/TestApp/Skeletal2D/CompressedFloat.hpp',
        '../test/TestApp/Skeletal2D/Joint.hpp',
        '../test/TestApp/Skeletal2D/JointIndex.hpp',
        '../test/TestApp/Skeletal2D/JointPose.hpp',
        '../test/TestApp/Skeletal2D/RigidSlot.hpp',
        '../test/TestApp/Skeletal2D/RotationTrack.hpp',
        '../test/TestApp/Skeletal2D/ScaleTrack.hpp',
        '../test/TestApp/Skeletal2D/Skeleton.cpp',
        '../test/TestApp/Skeletal2D/Skeleton.hpp',
        '../test/TestApp/Skeletal2D/SkeletonHelper.cpp',
        '../test/TestApp/Skeletal2D/SkeletonHelper.hpp',
        '../test/TestApp/Skeletal2D/SkeletonPose.hpp',
        '../test/TestApp/Skeletal2D/Skin.cpp',
        '../test/TestApp/Skeletal2D/Skin.hpp',
        '../test/TestApp/Skeletal2D/SkinnedMesh.cpp',
        '../test/TestApp/Skeletal2D/SkinnedMesh.hpp',
        '../test/TestApp/Skeletal2D/SkinnedVertex.hpp',
        '../test/TestApp/Skeletal2D/SpriteAnimationTrack.cpp',
        '../test/TestApp/Skeletal2D/SpriteAnimationTrack.hpp',
        '../test/TestApp/Skeletal2D/TranslationTrack.hpp',
        '../test/TestApp/Spine/AnimationLoader.cpp',
        '../test/TestApp/Spine/AnimationLoader.hpp',
        '../test/TestApp/Spine/SkeletonDesc.hpp',
        '../test/TestApp/Spine/SkeletonDescLoader.cpp',
        '../test/TestApp/Spine/SkeletonDescLoader.hpp',
        '../test/TestApp/Spine/SkeletonLoader.cpp',
        '../test/TestApp/Spine/SkeletonLoader.hpp',
        '../test/TestApp/Spine/SkinLoader.cpp',
        '../test/TestApp/Spine/SkinLoader.hpp',
        '../test/TestApp/Spine/SkinnedMeshLoader.cpp',
        '../test/TestApp/Spine/SkinnedMeshLoader.hpp',
        '../test/TestApp/Spine/SpriteAnimationLoader.cpp',
        '../test/TestApp/Spine/SpriteAnimationLoader.hpp',
        '../test/TestApp/TexturePacker/TextureAtlas.hpp',
        '../test/TestApp/TexturePacker/TextureAtlasLoader.cpp',
        '../test/TestApp/TexturePacker/TextureAtlasLoader.hpp',
        '../test/TestApp/SceneEditor/EditorColorScheme.hpp',
        '../test/TestApp/SceneEditor/GradientPlane.cpp',
        '../test/TestApp/SceneEditor/GradientPlane.hpp',
        '../test/TestApp/SceneEditor/InGameEditor.cpp',
        '../test/TestApp/SceneEditor/InGameEditor.hpp',
        '../test/TestApp/SceneEditor/PrimitiveAxes.cpp',
        '../test/TestApp/SceneEditor/PrimitiveAxes.hpp',
        '../test/TestApp/SceneEditor/PrimitiveGrid.cpp',
        '../test/TestApp/SceneEditor/PrimitiveGrid.hpp',
        '../test/TestApp/UI/Control.hpp',
        '../test/TestApp/UI/DrawingContext.hpp',
        '../test/TestApp/UI/DebugNavigator.cpp',
        '../test/TestApp/UI/DebugNavigator.hpp',
        '../test/TestApp/UI/Panel.hpp',
        '../test/TestApp/UI/PointerEventType.hpp',
        '../test/TestApp/UI/PointerPoint.hpp',
        '../test/TestApp/UI/ScenePanel.cpp',
        '../test/TestApp/UI/ScenePanel.hpp',
        '../test/TestApp/UI/Slider.cpp',
        '../test/TestApp/UI/Slider.hpp',
        '../test/TestApp/UI/StackPanel.cpp',
        '../test/TestApp/UI/StackPanel.hpp',
        '../test/TestApp/UI/Thickness.hpp',
        '../test/TestApp/UI/ToggleSwitch.cpp',
        '../test/TestApp/UI/ToggleSwitch.hpp',
        '../test/TestApp/UI/UIElement.cpp',
        '../test/TestApp/UI/UIElement.hpp',
        '../test/TestApp/UI/UIElementHierarchy.cpp',
        '../test/TestApp/UI/UIElementHierarchy.hpp',
        '../test/TestApp/UI/UIHelper.hpp',
        '../test/TestApp/UI/UIView.cpp',
        '../test/TestApp/UI/UIView.hpp',
        '../test/TestApp/Graphics/FXAA.cpp',
        '../test/TestApp/Graphics/FXAA.hpp',
        '../test/TestApp/Graphics/LineRenderer.cpp',
        '../test/TestApp/Graphics/LineRenderer.hpp',
        '../test/TestApp/Graphics/SpriteBatch.cpp',
        '../test/TestApp/Graphics/SpriteBatch.hpp',
        '../test/TestApp/Graphics/SpriteFont.cpp',
        '../test/TestApp/Graphics/SpriteFont.hpp',
        '../test/TestApp/Graphics/SpriteFontLoader.cpp',
        '../test/TestApp/Graphics/SpriteFontLoader.hpp',
        '../test/TestApp/Graphics/SpriteLine.cpp',
        '../test/TestApp/Graphics/SpriteLine.hpp',
        '../test/TestApp/Graphics/SpriteSortMode.hpp',
        '../test/TestApp/Graphics/SpriteRenderer.cpp',
        '../test/TestApp/Graphics/SpriteRenderer.hpp',
        '../test/TestApp/Graphics/TextureRegion.hpp',
        '../test/TestApp/LogSkeletalInfo.cpp',
        '../test/TestApp/LogSkeletalInfo.hpp',
        '../test/TestApp/SandboxHelper.hpp',
        '../test/TestApp/SpriteDrawingContext.cpp',
        '../test/TestApp/SpriteDrawingContext.hpp',
        '../test/TestApp/GrassBlendingGame.cpp',
        '../test/TestApp/GrassBlendingGame.hpp',
        '../test/TestApp/MaidChanGame.cpp',
        '../test/TestApp/MaidChanGame.hpp',
        '../test/TestApp/LightningTestGame.cpp',
        '../test/TestApp/LightningTestGame.hpp',
        '../test/TestApp/ParticleTestGame.cpp',
        '../test/TestApp/ParticleTestGame.hpp',
        '../test/TestApp/TestAppGame.cpp',
        '../test/TestApp/TestAppGame.hpp',
      ],
      'conditions': [
        ['component == "shared_library"', {
          'dependencies': [
            'pomdog.gyp:pomdog-shared',
          ],
          'defines': [
            'POMDOG_USING_LIBRARY_EXPORTS=1',
          ],
        }, {
          'dependencies': [
            'pomdog.gyp:pomdog-static',
          ],
        }],
        ['OS == "win"', {
          'sources': [
          ],
        }], # OS == "win"
        ['OS == "mac"', {
          'sources': [
            '../test/TestApp/Platform.Cocoa/main.mm',
            '../test/TestApp/Platform.Cocoa/TestApp-Prefix.pch',
            '../test/TestApp/Platform.Cocoa/AppDelegate.h',
            '../test/TestApp/Platform.Cocoa/AppDelegate.mm',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
              '$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
            ],
          },
          'actions': [
            {
              'action_name': 'XcodeResourcesUpdater',
              'description': 'Always update resources on Xcode',
              'inputs': [],
              'outputs': [],
              'action': ['./update-bundle-resources.sh', '${SOURCE_ROOT}/../test/TestApp/Content/.'],
            },
          ],
        }], # OS == "mac"
      ],
      'mac_bundle_resources': [
        '../test/TestApp/Content/',
        '../test/TestApp/Platform.Cocoa/Base.lproj/MainMenu.xib',
        '../test/TestApp/Platform.Cocoa/English.lproj/InfoPlist.strings',
        #'../test/TestApp/Platform.Cocoa/Images.xcassets/',
      ],
      'xcode_settings': {
        'INFOPLIST_FILE': '../test/TestApp/Platform.Cocoa/TestApp-Info.plist',
        'CLANG_ENABLE_OBJC_ARC': 'YES',
      },
    },
  ],# "targets"
}
