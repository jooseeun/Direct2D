
#include "TransformHeader.fx"
#include "RenderOption.fx"


struct Input
{
    float4 Pos : POSITION;
    float4 Tex : TEXCOORD;
};

struct Output
{
    float4 Pos : SV_POSITION;
    float4 PosLocal : POSITION;
    float4 Tex0 : TEXCOORD0;
    float4 Tex1 : TEXCOORD1;
};


cbuffer MyAtlasData : register(b1)
{
    float2 TextureFramePos;
    float2 TextureFrameSize;
    float4 PivotPos;
};


Output TextureMask_VS(Input _Input)
{


    Output NewOutPut = (Output)0;
    _Input.Pos += PivotPos;
    NewOutPut.Pos = mul(_Input.Pos, WorldViewProjection);
    NewOutPut.PosLocal = _Input.Pos;

    NewOutPut.Tex0.x = (_Input.Tex.x * TextureFrameSize.x) + TextureFramePos.x;
    NewOutPut.Tex0.y = (_Input.Tex.y * TextureFrameSize.y) + TextureFramePos.y;

    NewOutPut.Tex1 = _Input.Tex;

    return NewOutPut;
}

cbuffer MyPixelData : register(b0)
{
    float4 MulColor;
    float4 PlusColor;
    float4 Slice;
}

Texture2D Tex : register(t0);
Texture2D Mask : register(t1);
SamplerState Smp : register(s0);

float4 TextureMask_PS(Output _Input) : SV_Target0
{
    if (1 == IsMask)
    {
        float4 MskColor = Mask.Sample(Smp, _Input.Tex1.xy);

        _Input.Tex0.y -= (1.0f-EnergyGage);
        float4 RenderColor = ((Tex.Sample(Smp, _Input.Tex0.xy) * MulColor) + PlusColor) * MskColor.a;
        return RenderColor;
    }
    else {


        float4 TexColor = Tex.Sample(Smp, _Input.Tex0.xy);

        if (TexColor.a == 0)
        {
            clip(-1);
        }

        return (Tex.Sample(Smp, _Input.Tex0.xy) * MulColor) + PlusColor;;
    }

    return (Tex.Sample(Smp, _Input.Tex0.xy) * MulColor) + PlusColor;;
}
