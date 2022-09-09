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


Output BlueLight_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    NewOutPut.Pos = _Input.Pos;
    NewOutPut.Tex = _Input.Tex;
    return NewOutPut;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);
float4 BlueLight_PS(Output _Input) : SV_Target0
{
    float4 Color = Tex.Sample(Smp, _Input.Tex.xy);
    float Total = Color.r + Color.g + Color.b;
    //if (0.0 < Total <2.5f)
    //{
    //    Color.b += 0.01f;
    //    Color.g += 0.002f; 
    //    Color.r += 0.001f;
    //}

    if (Color.a <= 0.0f)
    {
        clip(-1);
    }

    return Color;
}
