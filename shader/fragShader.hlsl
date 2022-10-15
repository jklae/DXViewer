struct FSInput
{
    float4 PosH    : SV_POSITION;
    float3 NormalW : NORMAL;
    float4 fColor  : COLOR;
};

float4 main(FSInput fsIn) : SV_Target
{
    float3 light = normalize(float3(1.0f, 1.0f ,1.0f));
    float3 normal = normalize(fsIn.NormalW);

    float3 diffuse = max(dot(normal, light), 0.1f);
    float3 ambient = fsIn.fColor.xyz;

    float4 fragColor = float4(diffuse + ambient, fsIn.fColor.w);

    return fragColor;
}
