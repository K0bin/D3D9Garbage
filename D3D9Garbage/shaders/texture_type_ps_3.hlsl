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
sampler2D s_2d;

PS_OUTPUT main(VS_OUTPUT ps_in)
{
    PS_OUTPUT p_out;
    p_out.color = tex2D(s_2d, ps_in.texcoord.xy);
    p_out.color.rgb = p_out.color.aaa;

    return p_out;
}
