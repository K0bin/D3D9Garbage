struct PS_OUTPUT
{
	float4 color : COLOR0;
};

PS_OUTPUT main(float4 texCoords : TEXCOORD0)
{
    PS_OUTPUT p_out;
	p_out.color = texCoords;

	p_out.color.x = 0.0;
	p_out.color.y = 0.0;
	p_out.color.z = 0.0;
	p_out.color.w = 0.0;

	//p_out.color.x = texCoords.z;
	p_out.color.y = texCoords.w;
	//p_out.color.z = 0.0;
	return p_out;
}
