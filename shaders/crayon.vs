attribute vec4 Position;
attribute vec4 Normal;
attribute vec2 TexCoord;
attribute lowp vec4 a_color;
varying vec2 DestinationTexCoord;
varying lowp vec4  v_color;
uniform float width;
uniform float height;
void main(void)
{
    
    v_color = a_color;
    v_color = clamp(v_color, 0.0, 1.0);
//    v_texCoord = a_texCoord0;
    DestinationTexCoord = TexCoord;
  gl_Position = vec4(Position.x/width*2.0-1.0,Position.y/height*2.0-1.0,0.0,1.0 ) ;
}
