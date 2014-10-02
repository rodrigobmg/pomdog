﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

char const* Builtin_GLSL_FXAA_PS =
"#version 330\n"
"in VertexData{\n"
"vec4 TextureCoord;\n"
"}In;\n"
"uniform Constants{\n"
"vec2 RenderTargetPixelSize;\n"
"};\n"
"uniform sampler2D Texture;\n"
"out vec4 FragColor;\n"
"float FxaaLuma(vec3 rgb)\n"
"{\n"
"const vec3 luma=vec3(0.299,0.587,0.114);\n"
"return dot(rgb,luma);\n"
"}\n"
"vec4 FxaaPixelShader(sampler2D tex,vec4 posPos,vec2 rcpFrame)\n"
"{\n"
"vec3 rgbNW=textureLod(tex,posPos.zw,0.0).xyz;\n"
"vec3 rgbNE=textureLodOffset(tex,posPos.zw,0.0,ivec2(1,0)).xyz;\n"
"vec3 rgbSW=textureLodOffset(tex,posPos.zw,0.0,ivec2(0,1)).xyz;\n"
"vec3 rgbSE=textureLodOffset(tex,posPos.zw,0.0,ivec2(1,1)).xyz;\n"
"vec3 rgbM =textureLod(tex,posPos.xy,0.0).xyz;\n"
"float lumaNW=FxaaLuma(rgbNW);\n"
"float lumaNE=FxaaLuma(rgbNE);\n"
"float lumaSW=FxaaLuma(rgbSW);\n"
"float lumaSE=FxaaLuma(rgbSE);\n"
"float lumaM =FxaaLuma(rgbM);\n"
"float lumaMin=min(lumaM,min(min(lumaNW,lumaNE),min(lumaSW,lumaSE)));\n"
"float lumaMax=max(lumaM,max(max(lumaNW,lumaNE),max(lumaSW,lumaSE)));\n"
"vec2 dir=vec2(\n"
"-((lumaNW+lumaNE)-(lumaSW+lumaSE)),\n"
"((lumaNW+lumaSW)-(lumaNE+lumaSE)));\n"
"const float FxaaReduceMin=1.0/128.0;\n"
"const float FxaaReduceMul=1.0/8.0;\n"
"const float FxaaSpanMax=8.0;\n"
"float dirReduce=max(\n"
"(lumaNW+lumaNE+lumaSW+lumaSE)*(0.25*FxaaReduceMul),\n"
"FxaaReduceMin);\n"
"float rcpDirMin=1.0/(min(abs(dir.x),abs(dir.y))+dirReduce);\n"
"dir=min(vec2(FxaaSpanMax,FxaaSpanMax),\n"
"max(vec2(-FxaaSpanMax,-FxaaSpanMax),dir*rcpDirMin))*rcpFrame.xy;\n"
"vec3 rgbA=(1.0/2.0)*(\n"
"textureLod(tex,posPos.xy+dir*(1.0/3.0-0.5),0.0).xyz +\n"
"textureLod(tex,posPos.xy+dir*(2.0/3.0-0.5),0.0).xyz);\n"
"vec3 rgbB=rgbA*(1.0/2.0)+(1.0/4.0)*(\n"
"textureLod(tex,posPos.xy+dir*(0.0/3.0-0.5),0.0).xyz +\n"
"textureLod(tex,posPos.xy+dir*(3.0/3.0-0.5),0.0).xyz);\n"
"float lumaB=FxaaLuma(rgbB);\n"
"if((lumaB<lumaMin)||(lumaB>lumaMax)){\n"
"return vec4(rgbA,1.0);\n"
"}\n"
"return vec4(rgbB,1.0);\n"
"}\n"
"void main(){\n"
"vec2 rcpFrame=vec2(1.0,1.0)/RenderTargetPixelSize.xy;\n"
"FragColor=FxaaPixelShader(Texture,In.TextureCoord,rcpFrame);\n"
"}\n";
