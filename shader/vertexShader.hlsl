cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	float4x4 gTransInvWorld;
	float4 gColor;
	float4 gLightPos;
};

struct VSInput
{
	float3 iPosL	: POSITION;
	float3 iNormalL : NORMAL;
};

struct VSOutput
{
	float4 oPosH		: SV_POSITION;
	float3 oNormalW		: NORMAL;
	float4 oColor		: COLOR;
	float3 oLightPos	: POSITION;
};

VSOutput main(VSInput vsIn)
{
	VSOutput vsOut;

	// Normalize the vertex normal
	float3x3 transInvWorld = (float3x3) gTransInvWorld;
	vsOut.oNormalW = normalize(mul(transInvWorld, vsIn.iNormalL));

	// Transform to homogeneous clip space.
	vsOut.oPosH = mul(gWorldViewProj, float4(vsIn.iPosL, 1.0f));
	
	// Just pass vertex color and light position into the fragment shader.
	vsOut.oColor = gColor;
	vsOut.oLightPos = gLightPos.xyz;

	return vsOut;
}