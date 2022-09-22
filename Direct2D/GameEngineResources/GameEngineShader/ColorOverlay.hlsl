struct Input
{
    float4 Pos : POSITION;
    float4 Tex : TEXCOORD;
};


struct Output
{
    float4 Pos : SV_POSITION;
    float4 Tex : TEXCOORD;
};


Output ColorOverlay_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    NewOutPut.Pos = _Input.Pos;
    NewOutPut.Tex = _Input.Tex;
    return NewOutPut;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 ColorOverlay_PS(Output _Input) : SV_Target0
{
    
    float4 Color = Tex.Sample(Smp, _Input.Tex.xy);

    if (Color.r >= 0.5f)
    {
        Color.r = 1 - 2 * (1 - Color.r) * (1 - 0.51f);
    }
    else
    {
        Color.r = 2 * Color.r * 0.51f;
    }

    if (Color.g >= 0.5f)
    {
        Color.g = 1 - 2 * (1 - Color.g) * (1 - 0.55f);
    }
    else
    {
        Color.g = 2 * Color.g * 0.55f;
    }

    if (Color.b >= 0.5f)
    {
        Color.b = 1 - 2*(1 - Color.b)*(1 - 0.58f);
    }
    else
    {
        Color.b = 2 * Color.b * 0.58f;
    }

    return Color;
}