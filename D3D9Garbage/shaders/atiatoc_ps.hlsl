struct VS_OUTPUT
{
	float4 pos : POSITION;
};

struct PS_OUTPUT
{
	float4 color : COLOR1;
};

PS_OUTPUT main(uniform float alpha = 0)
{
    PS_OUTPUT p_out;
	p_out.color = float4(1.0, 0.0, 0.5, alpha);
	return p_out;
}
