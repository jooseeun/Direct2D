// #include "myshader.hlsli"

// SV_POSITION �ø�ƽ
// �׷���ī�忡�� �̳༮�� �̷� �η��ϱ� �ϰ� �ڵ����� ó���ϴ� �༮�� ������ �ϰ�.

// t1

// Create("TransformData");
#include "TransformHeader.fx"

// cbuffer Color : 
// TestColor;
struct Input
{
    float4 Pos : POSITION;
};

struct Output
{
    // �����Ͷ��������� ����Ʈ�� ���ؼ� �̳༮���� �ȼ��� ��������.
    float4 Pos : SV_POSITION;
    // �����Ͷ��������� ����Ʈ�� ���ؼ� �̳༮���� �ȼ��� ��������.
    float4 PosWorld : POSITION;
};

//cbuffer ResultColor : register(b2)
//{
//    float4 PlusColor;
//    float4 MultyplyColor;
//}



// 1000
// 0100
// 2010
// 0301

// 1020
// 0103
// 0010
// 0001

Output Debug3D_VS(Input _Input)
{
    // ���̴��� ��쿡�� ��κ��� ��Ȳ���� ����ȯ�� �����ϴ�.
    Output NewOutPut = (Output)0;
    NewOutPut.Pos = mul(_Input.Pos, WorldViewProjection);
    NewOutPut.PosWorld.xyz = _Input.Pos.xyz;
    NewOutPut.PosWorld.w = 1.0f;
    return NewOutPut;
}

cbuffer DebugInfo : register(b8)
{
    int4 Type;
    float4 Color;
}

float4 Debug3D_PS(Output _Input) : SV_Target0
{
    // ���� �׸��ų�
    // �ڽ��� �׸����� ����?
    
    
    if (Type.x == 0 && length(_Input.PosWorld.xyz) > 0.7f)
    {
        clip(-1);
    }
        
    
    return Color;
}
