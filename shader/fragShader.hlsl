

float4 main(float4 PosH  : SV_POSITION, float3 NormalW : NORMAL,
    float4 Color : COLOR) : SV_Target
{
    float3 light = normalize(float3(1.0f ,1.0f ,0.0f));
    float3 normal = normalize(NormalW);

    float3 diff = max(dot(normal, light), 0.0f);

    float fragColor = diff;//float4(1.0f, 1.0f, 1.0f, 1.0f);

    return fragColor;
}
