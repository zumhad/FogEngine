#define PI 3.14159265358979323846
#define FLT_MIN  1.175494351e-38

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float power;
};

struct PointLight
{
    float4 color;
    float3 position;
    float radius;
    float power; float3 pad;
};

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 FresnelSchlick(float cosTheta, float3 F0, float roughness)
{
    float r = 1.0f - roughness;
    return F0 + (max(float3(r, r, r), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0f);
}

float3 ApplyDirectionLight(DirectionalLight dirLight, float3 normal, float3 color, float3 position, float3 camera, float metallic, float roughness)
{
    float3 V = normalize(camera - position);
    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, color, metallic);

    float3 L = normalize(-dirLight.direction);
    float3 H = normalize(V + L);

    float3 radiance = dirLight.color.rgb * dirLight.power;

    float D = DistributionGGX(normal, H, roughness);
    float G = GeometrySmith(normal, V, L, roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0, roughness);

    float3 kD = (1.0f - F) * (1.0f - metallic);

    float3 numerator = D * G * F;
    float denominator = 4.0f * max(dot(normal, V), 0.0f) * max(dot(normal, L), 0.0f) + 0.0001f;
    float3 specular = numerator / denominator;

    float NdotL = max(dot(normal, L), 0.0f);
    return (kD * color / PI + specular) * radiance * NdotL;
}

float3 ApplyPointLight(PointLight pointLight, float3 normal, float3 color, float3 position, float3 camera, float metallic, float roughness)
{
    float3 distance = pointLight.position - position;
    float distanceSqr = dot(distance, distance);
    float radiusSqr = pointLight.radius * pointLight.radius;
    float attenuation = max(0.0f, 1.0f - distanceSqr / radiusSqr) / distanceSqr;
    float radiance = attenuation * pointLight.power;

    if (radiance == 0.0f) return float3(0.0f, 0.0f, 0.0f);

    float3 V = normalize(camera - position);
    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, color.rgb, metallic);

    float3 L = normalize(distance);
    float3 H = normalize(V + L);

    float D = DistributionGGX(normal, H, roughness);
    float G = GeometrySmith(normal, V, L, roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0, roughness);

    float3 kD = (1.0f - F) * (1.0f - metallic);

    float3 numerator = D * G * F;
    float denominator = 4.0f * max(dot(normal, V), 0.0f) * max(dot(normal, L), 0.0f) + 0.0001f;
    float3 specular = numerator / denominator;

    float NdotL = max(dot(normal, L), 0.0f);
    return (kD * color.rgb / PI + specular) * radiance * NdotL * pointLight.color.rgb;
}