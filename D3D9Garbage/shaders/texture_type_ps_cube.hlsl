struct PS_OUTPUT
{
    float4 color : COLOR0;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float4 texcoord : TEXCOORD0;
};

texture g_texture;
samplerCUBE s_Cube;

PS_OUTPUT main(VS_OUTPUT ps_in)
{
    PS_OUTPUT p_out;
    p_out.color = texCUBE(s_Cube, ps_in.texcoord.xyz);

    return p_out;
}
