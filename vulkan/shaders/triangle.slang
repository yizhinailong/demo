// 顶点与片元入口共用同一份 Slang 源码。

struct VertexOutput
{
    float4 position : SV_Position;
    float3 color : COLOR0;
};

struct FragmentInput
{
    float3 color : COLOR0;
};

static const float2 kPositions[3] =
{
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5)
};

static const float3 kColors[3] =
{
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
};

[shader("vertex")]
VertexOutput vertMain(uint vertex_id : SV_VertexID)
{
    VertexOutput output;
    output.position = float4(kPositions[vertex_id], 0.0, 1.0);
    output.color = kColors[vertex_id];
    return output;
}

[shader("fragment")]
float4 fragMain(FragmentInput input) : SV_Target
{
    return float4(input.color, 1.0);
}
