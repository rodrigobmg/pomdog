﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

char const* Builtin_GLSL_SpriteRenderer_VS =
"#version 330\n"
"layout(location=0)in vec4 PositionTextureCoord;\n"
"layout(location=1)in vec4 TransformMatrix1;\n"
"layout(location=2)in vec4 TransformMatrix2;\n"
"layout(location=3)in vec4 SourceRect;\n"
"layout(location=4)in vec4 OriginScale;\n"
"layout(location=5)in vec4 Color;\n"
"out VertexData{\n"
"vec2 TextureCoord;\n"
"vec4 Color;\n"
"}Out;\n"
"uniform Matrices{\n"
"mat4x4 ViewProjection;\n"
"};\n"
"uniform TextureConstants{\n"
"vec2 InverseTextureWidth;\n"
"};\n"
"void main()\n"
"{\n"
"mat2x2 scaling=mat2x2(\n"
"vec2(OriginScale.z*SourceRect.z,0.0),\n"
"vec2(0.0,OriginScale.w*SourceRect.w));\n"
"float cosRotation=cos(TransformMatrix2.w);\n"
"float sinRotation=sin(TransformMatrix2.w);\n"
"mat2x2 rotate=mat2x2(\n"
"vec2(cosRotation,-sinRotation),\n"
"vec2(sinRotation,cosRotation));\n"
"mat2x2 localTransform=(scaling*rotate);\n"
"vec2 position=(PositionTextureCoord.xy-OriginScale.xy).xy*localTransform;\n"
"mat3x2 worldMatrix=mat3x2(\n"
"vec2(TransformMatrix1.xy),\n"
"vec2(TransformMatrix1.zw),\n"
"vec2(TransformMatrix2.xy));\n"
"position=(worldMatrix*vec3(position,1.0)).xy;\n"
"gl_Position=vec4(position.xy,0.0,1.0)*ViewProjection;\n"
"Out.TextureCoord=(PositionTextureCoord.zw*SourceRect.zw+SourceRect.xy)*InverseTextureWidth.xy;\n"
"Out.Color=Color;\n"
"}\n";
