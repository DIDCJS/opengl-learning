#version 330 core  
#define NEED_POINT_LIGHT;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 viewPos;

layout(location = 0) out vec4 outColor;

#ifdef NEED_DIR_LIGHT 
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};  
uniform DirLight dirLight;
#endif

#ifdef NEED_POINT_LIGHT
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];
#endif

#ifdef NEED_SPOT_LIGHT
struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform SpotLight spotLight;
#endif

struct Material {
    vec3 ambient;
    float shininess;
	sampler2D diffuse; // the diffuse color 
	sampler2D specular;
    sampler2D normal;
}; 
uniform Material material;

#ifdef NEED_DIR_LIGHT 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) 
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
	//return texture(material.diffuse, TexCoords).rgb;
}
#endif

#ifdef NEED_POINT_LIGHT
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec;// * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
#endif

#ifdef NEED_SPOT_LIGHT
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	//聚光灯强度
	float theta     = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;	
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
	diffuse  *= intensity;
	specular *= intensity;
    return (ambient + diffuse + specular);
}
#endif


void main(){

	 // 属性
    vec3 normTex = texture(material.normal, TexCoords).rgb;
    vec3 norm = normalize(normTex * 2.0 - 1.0);
    // vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0.0);

    // 第一阶段：定向光照
#ifdef NEED_DIR_LIGHT
    result = CalcDirLight(dirLight, norm, viewDir);
#endif

    // 第二阶段：点光源
#ifdef NEED_POINT_LIGHT
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
#endif

    // 第三阶段：聚光
#ifdef NEED_SPOT_LIGHT
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
#endif
    outColor = vec4(result, 1.0);

    // outColor = vec4(1.0);
    // outColor = vec4(texture(material.normal, lightPos).rgb, 1.0);
}