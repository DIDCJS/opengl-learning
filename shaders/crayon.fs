precision highp float;
uniform sampler2D Texture;
uniform float magnitude;
varying highp vec2 DestinationTexCoord;
varying lowp    vec4 v_color;//原图颜色
varying highp   vec3 v_texCoord;

void main()
{

        lowp vec4 color;
        color = v_color;
        color = clamp(color, 0.0, 1.0);
    //    lowp int      currTex;
        lowp vec4     texColor;
        highp vec3    texCoords;
    //    highp float   s, t;
        texCoords.x = DestinationTexCoord.x;
        texCoords.y = DestinationTexCoord.y;
        texColor = texture2D(Texture, texCoords.xy);
        color *= texColor;
        gl_FragColor = color;
    
}