#version 150

// input tex coordinates (computed by the interpolator)
in vec2 tc;
// output color (the final fragment color)
out vec4 c;

// the texture image
uniform sampler2D textureImage;

void main()
{
  // compute the final fragment color
  // by looking up into the texture map
  c = texture(textureImage, tc);
}

