
struct VS_INPUT
{
	float4 mPosition : POSITION;
}

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
}

float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjectionMatrix;

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT ret;

	ret.mPosition = mul(input.mPosition, gWorldMatrix);
	ret.mPosition = mul(ret.mPosition, gViewMatrix);
	ret.mPosition = mul(ret.mPosition, gProjectionMatrix);

	return ret;
}