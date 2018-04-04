#version 450 core
layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec4 vTangent;
layout(location = 3) in vec4 vBiTangent;
layout(location = 4) in vec2 vUVCoord;

smooth out vec4 outPosition;
smooth out vec3 outNormal;
smooth out vec2 outUVCoord;
smooth out vec3 outTangent;
smooth out vec3 outBiTangent;

layout(location=0) out VertexData{
	vec4 Position;
	vec3 Normal;
	vec2 UVCoord;
	vec3 Tangent;
	vec3 BiTangent;
};

uniform mat4 PVM;
uniform mat4 V;
uniform mat3 normalM;

out gl_PerVertex {
	vec4 gl_Position;
};

void main()
{
	Position = V*vVertex;
	UVCoord = vUVCoord;
	Normal=normalize(normalM*vNormal.xyz);
	Tangent=normalize(normalM*vTangent.xyz);
	BiTangent=normalize(normalM*vBiTangent.xyz);
	gl_Position = PVM*vVertex;
}