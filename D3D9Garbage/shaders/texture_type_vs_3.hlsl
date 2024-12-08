struct VS_INPUT
{
    float4 pos : POSITION;
    float4 texcoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos : POSITION;
    float4 texcoord : TEXCOORD0;
};

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT v_out;
    v_out.pos = v.pos;
    v_out.texcoord = v.texcoord;
    return v_out;
}
