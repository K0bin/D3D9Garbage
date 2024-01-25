struct VS_INPUT
{
	float4 pos : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
};

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT v_out;
	v_out.pos = v.pos;
	return v_out;
}
