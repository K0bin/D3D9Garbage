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
sampler3D s_3d;

PS_OUTPUT main(VS_OUTPUT ps_in)
{
    PS_OUTPUT p_out;
    p_out.color = tex3D(s_3d, ps_in.texcoord.xyz);
    //p_out.color = ps_in.texcoord;

    return p_out;
}
