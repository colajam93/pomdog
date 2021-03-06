// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

char const* Builtin_GLSL_FXAA_PS =
"#version 330\n"
"float FxaaLuma(vec3 rgb){\n"
"const vec3 luma=vec3(0.299,0.587,0.114);\n"
"return dot(rgb,luma);}\n"
"vec4 FxaaPixelShader(sampler2D tex,vec4 posPos,vec2 rcpFrame){\n"
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
"float dirSwMinusNe=lumaSW-lumaNE;\n"
"float dirSeMinusNw=lumaSE-lumaNW;\n"
"vec2 dir=vec2(dirSwMinusNe+dirSeMinusNw,dirSwMinusNe-dirSeMinusNw);\n"
"const float FxaaReduceMin=1.0/128.0;\n"
"const float FxaaReduceMul=1.0/8.0;\n"
"const float FxaaSpanMax=8.0;\n"
"float dirReduce=max((lumaNW+lumaNE+lumaSW+lumaSE)*(0.25*FxaaReduceMul),\n"
"FxaaReduceMin);\n"
"float rcpDirMin=1.0/(min(abs(dir.x),abs(dir.y))+dirReduce);\n"
"dir=clamp(dir*rcpDirMin,vec2(-FxaaSpanMax),vec2(FxaaSpanMax))*rcpFrame.xy;\n"
"vec3 rgbN1=textureLod(tex,posPos.xy+dir*(1.0/3.0-0.5),0.0).xyz;\n"
"vec3 rgbP1=textureLod(tex,posPos.xy+dir*(2.0/3.0-0.5),0.0).xyz;\n"
"vec3 rgbN2=textureLod(tex,posPos.xy+dir*(0.0/3.0-0.5),0.0).xyz;\n"
"vec3 rgbP2=textureLod(tex,posPos.xy+dir*(3.0/3.0-0.5),0.0).xyz;\n"
"vec3 rgbA=(rgbN1+rgbP1);\n"
"vec3 rgbB=((rgbN2+rgbP2)*0.25)+(rgbA*0.25);\n"
"float lumaB=FxaaLuma(rgbB);\n"
"if((lumaB<lumaMin)||(lumaB>lumaMax)){\n"
"rgbB.xyz=rgbA.xyz*0.5;}\n"
"return vec4(rgbB,1.0);}\n"
"precision highp float;\n"
"in VertexData{\n"
"vec4 TextureCoord;}In;\n"
"uniform ImageEffectConstants{\n"
"vec2 RenderTargetPixelSize;\n"
"vec2 RcpFrame;};\n"
"uniform sampler2D Texture;\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor=FxaaPixelShader(Texture,In.TextureCoord,RcpFrame);}\n";
