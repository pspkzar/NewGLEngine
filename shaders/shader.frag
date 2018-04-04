#version 450 core

#define M_PI 3.14159265358979323846
#define F0 0.04f

layout(location=0) out vec4 vFragColor;

layout(location=0) in VertexData{
	vec4 Position;
	vec3 Normal;
	vec2 UVCoord;
	vec3 Tangent;
	vec3 BiTangent;
};

uniform vec4 Kd;
uniform vec4 Ks;
uniform float Ns;
layout(binding=0) uniform sampler2D MapKd;
layout(binding=1) uniform sampler2D MapKs;
layout(binding=2) uniform sampler2D MapBump;

uniform mat4 V;
uniform vec4 lightDir;

void main()
{
	vec3 l=(V*lightDir).xyz;
	
	vec4 diffuse = Kd * texture(MapKd, UVCoord);
	vec4 spec = Ks * texture(MapKs, UVCoord);
	
	vec3 normal;
	normal = vec3(texture(MapBump, UVCoord));
	
	if(normal!=vec3(1)){
		mat3 TBN = mat3(normalize(Tangent), normalize(BiTangent), normalize(Normal));
		normal = normal*2-1;
		normal.z = sqrt(1 - normal.x * normal.x - normal.y * normal.y);
		normal = normalize(TBN*normal);
	}
	else
		normal = normalize(Normal);

    vec3 n=normal;
    /*
	float intensity = max(dot(normal, lightDirView), 0)*.8 +.2;

	vec3 lookDir = normalize(Position.xyz);
	vec3 reflected = reflect(lookDir, normal);

	float spec_intensity = max(0, dot(lightDirView, reflected));
	spec_intensity = pow(spec_intensity, Ns);

	vFragColor=intensity*diffuse + spec_intensity * spec;
    */

    vec3 v = -normalize(Position.xyz);
    vec3 h = normalize(l+v);

    float roughness=1-spec.r;

    float alpha = roughness*roughness;
    float alpha2 = alpha*alpha;
    float D_den = dot(n,h) * dot(n,h) * (alpha2-1) + 1;
    D_den *= D_den * M_PI;

    float k = roughness+1;
    k *= k/8;
    float G1v = dot(n,v)/(dot(n,v)*(1-k)+k);
    float G1l = dot(n,l)/(dot(n,l)*(1-k)+k);

    float F = F0 + (1-F0) * pow(1-dot(l,h),5);
    //float D = (alpha+2)*pow(dot(n,h), alpha)/(2*M_PI);
    float D = alpha2/D_den;
    //float G = min(1, min(2*dot(n,h)*dot(n,v)/dot(v,h), dot(n,h)*dot(n,l)/dot(v,h)));
    float G = G1l*G1v;

    vec3 res = vec3(diffuse-F0)/M_PI + max(0.f, F*D*G/(4*dot(n,l)*dot(n,v)));

    float intensity = max(0.f,dot(n,l));

    vFragColor = vec4(res*intensity,1)*M_PI;
    //vFragColor =vec4(alpha);
    //vFragColor = vec4(max(0.f, F*D*G/(4*dot(n,l)*dot(n,v)))*intensity);
}