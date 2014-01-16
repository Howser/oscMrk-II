// Lighting shader using normal maps and diffuse texture
// The lighting model is the Lambertian Illumination Model
// This shader can only support up to 30 lights atm, (easily changed)
// GLSL doesn't seem to support variable arrays

uniform sampler2D texture; // diffuse map
uniform sampler2D normal;  // normal map

uniform vec2 Resolution;
uniform vec3 LightPos[30];
uniform vec4 LightColor[30];
uniform vec4 AmbientColor;
uniform vec3 Falloff[30];
uniform float LightCount;

void main()
{
 vec4 DiffuseColor = texture2D(texture, gl_TexCoord[0].xy);
 vec3 NormalMap = texture2D(normal, gl_TexCoord[0].xy).rgb;
 
 vec3 Sum = vec3(0.0);

 for (int i = 0; i < LightCount; i++)
 {
  // Delta position of light
  vec3 LightDir = vec3(LightPos[i].xy - (gl_FragCoord.xy / Resolution.xy), LightPos[i].z);

  // Correct for aspect ration
  LightDir.x *= Resolution.x / Resolution.y;

  // determine distance
  float D = length(LightDir);

  // Normalize our vectors
  vec3 N = normalize(NormalMap * 2.0 - 1.0);
  vec3 L = normalize(LightDir);

  // Pre multiply light color with intesity
  // Then preform N dot L to determine our diffuse term
  vec3 Diffuse = (LightColor[i].rgb * LightColor[i].a) * max(dot(N, L), 0.0);

  // calculate attenuation
  float Attenuation = 1.0 / (Falloff[i].x + (Falloff[i].y*D) + (Falloff[i].z*D*D));

  vec3 Intesity = Diffuse * Attenuation;
  vec3 FinalColor = DiffuseColor.rgb * Intesity;
  Sum += FinalColor;
 }
 
 // pre-multiply ambient color with intensity
 vec3 Ambient = AmbientColor.rgb * AmbientColor.a;

 Sum += Ambient; 
 

 gl_FragColor =  vec4(Sum, DiffuseColor.a);
}

/*uniform sampler2D texture; // diffuse map
uniform sampler2D normal;  // normal map

uniform vec2 Resolution;
uniform vec3 LightPos[30];
uniform vec4 LightColor[30];
uniform vec4 AmbientColor;
uniform vec3 Falloff[30];
uniform float LightCount;

void main()
{
	vec4 DiffuseColor = texture2D(texture, gl_TexCoord[0].xy);
	vec3 NormalMap = texture2D(normal, gl_TexCoord[0].xy).rgb;
	
	vec3 Sum = vec3(0.0);

	for (int i = 0; i < LightCount; i++)
	{
		// Delta position of light
		vec3 LightDir = vec3(LightPos[i].xy - (gl_FragCoord.xy / Resolution.xy), LightPos[i].z);

		// Correct for aspect ration
		LightDir.x *= Resolution.x / Resolution.y;

		// determine distance
		float D = length(LightDir);

		// Normalize our vectors
		vec3 N = normalize(NormalMap * 2.0 - 1.0);
		vec3 L = normalize(LightDir);

		// Pre multiply light color with intesity
		// Then preform N dot L to determine our diffuse term
		vec3 Diffuse = (LightColor[i].rgb * LightColor[i].a) * max(dot(N, L), 0.0);

		// pre-multiply ambient color with intensity
		vec3 Ambient = AmbientColor.rgb * AmbientColor.a;

		// calculate attenuation
		float Attenuation = 1.0 / (Falloff[i].x + (Falloff[i].y*D) + (Falloff[i].z*D*D));

		vec3 Intesity = Ambient + Diffuse * Attenuation;
		//vec3 Intesity = Diffuse * Attenuation;
		vec3 FinalColor = DiffuseColor.rgb * Intesity;
		Sum += FinalColor;
	}
	

	gl_FragColor =  vec4(Sum, DiffuseColor.a);
}*/