struct PS_OUTPUT
{
	float4 color : COLOR0;
};

PS_OUTPUT main(float4 texCoords : TEXCOORD0)
{
    PS_OUTPUT p_out;
	p_out.color.x = texCoords.y;
	p_out.color.y = texCoords.w;
	p_out.color.z = texCoords.z;
	p_out.color.w = texCoords.x;

	return p_out;
}
