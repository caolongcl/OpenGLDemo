#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 viewPos;

//材质属性
struct Material
{
	sampler2D diffuse;//漫反射贴图
	sampler2D specular;//镜面贴图
	sampler2D emission;//放射光贴图
	float shininess;//发光值
};
uniform Material material;
in vec2 TexCoords;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
    float linear;
    float quadratic;  

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float inCutOff;
	float outCutOff;

	float constant;
    float linear;
    float quadratic; 

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform vec3 lightType;

vec3 caclDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 caclPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 caclSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 objectColor = caclDirLight(dirLight, normal, viewDir) * lightType.x;
	objectColor += caclPointLight(pointLight, normal, FragPos, viewDir) * lightType.y;
	objectColor += caclSpotLight(spotLight, normal, FragPos, viewDir) * lightType.z;

	color = vec4(objectColor, 1.0f);
}

vec3 caclDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);//漫反射

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//镜面高光

	vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
	vec3 diffuse = (vec3(texture(material.diffuse, TexCoords)) * diff) * light.diffuse;
	vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * light.specular;

	return (ambient + diffuse + specular);
}

vec3 caclPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);//漫反射

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//镜面高光

	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear*distance +light.quadratic*(distance*distance));

	vec3 ambient = attenuation * vec3(texture(material.diffuse, TexCoords)) * light.ambient;
	vec3 diffuse = attenuation * (vec3(texture(material.diffuse, TexCoords)) * diff) * light.diffuse;
	vec3 specular = attenuation * (vec3(texture(material.specular, TexCoords)) * spec) * light.specular;

	return (ambient + diffuse + specular);
}

vec3 caclSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);//漫反射

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//镜面高光

	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear*distance +light.quadratic*(distance*distance));

	//聚光边缘平滑/软化
	float theta = dot(-lightDir, normalize(light.direction));
	float epsilon = light.inCutOff - light.outCutOff;
	float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = attenuation * intensity * vec3(texture(material.diffuse, TexCoords)) * light.ambient;
	vec3 diffuse = attenuation * intensity * (vec3(texture(material.diffuse, TexCoords)) * diff) * light.diffuse;
	vec3 specular = attenuation * intensity * (vec3(texture(material.specular, TexCoords)) * spec) * light.specular;

	return (ambient + diffuse + specular);
}