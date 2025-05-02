struct VS_OUTPUT
{
	float4 pos : POSITION;
	float4 col : COLOR0;
};

struct PS_OUTPUT
{
	float4 color : COLOR0;
};

PS_OUTPUT main(VS_OUTPUT v, uniform float4 col = float4(1.0, 1.0, 1.0, 1.0))
{
    PS_OUTPUT p_out;
	p_out.color.x = col.x * v.col.x;
	p_out.color.y = col.y * v.col.y;
    p_out.color.z = col.z * v.col.z;
    p_out.color.w = col.w * v.col.w;
	return p_out;
}
