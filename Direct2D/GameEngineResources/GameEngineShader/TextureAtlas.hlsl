// #include "myshader.hlsli"

// SV_POSITION 시맨틱
// 그래픽카드에게 이녀석은 이런 부류니까 니가 자동으로 처리하는 녀석이 있으면 하고.

#include "TransformHeader.fx"

// 0                                                                                                1 
// 0□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□1

struct Input
{
    float4 Pos : POSITION;
    float4 Tex : TEXCOORD;
};

struct Output
{
    float4 Pos : SV_POSITION;
    float4 PosLocal : POSITION;
    float4 Tex : TEXCOORD;
};

// 1000
// 0100
// 2010
// 0301

// 1020
// 0103
// 0010
// 0001

cbuffer AtlasData : register(b1)
{
    float2 TextureFramePos;
    float2 TextureFrameSize;
};


Output TextureAtlas_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    NewOutPut.Pos = mul(_Input.Pos, WorldViewProjection);
    NewOutPut.PosLocal = _Input.Pos;
    
    // 버텍스가 몇번째 버텍스 인지 알수가 없다.
    // NewOutPut.Tex
    // 00    10
    
    // 10    11
    
    NewOutPut.Tex.x = (_Input.Tex.x * TextureFrameSize.x) + TextureFramePos.x;
    NewOutPut.Tex.y = (_Input.Tex.y * TextureFrameSize.y) + TextureFramePos.y;
    
    return NewOutPut;
}

Texture2D Tex : register(t0);
SamplerState Sam : register(s0);
float4 TextureAtlas_PS(Output _Input) : SV_Target0
{
    return Tex.Sample(Sam, _Input.Tex.xy);
}
