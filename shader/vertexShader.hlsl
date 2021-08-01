cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	float3x3 gTransInvWorld;
	float4 gColor;
};


void main(float3 iPosL  : POSITION, float3 iNormal	: NORMAL,
	out float4 oPosH  : SV_POSITION, out float4 oColor : COLOR)
{
	float3 n = normalize(mul(gTransInvWorld, iNormal));
	float4 posW = mul(gWorld, float4(iPosL, 1.0f));

	// Transform to homogeneous clip space.
	oPosH = mul(gWorldViewProj, float4(iPosL, 1.0f));
	
	// Just pass vertex color into the pixel shader.
	//oColor = gColor;  float4(iNormal, 1.0f);
}