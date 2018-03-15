#version 450 core

in vec2 texCoord;

struct GaussCoef
{
   vec4 bi;
   float B;
};

uniform GaussCoef gaussCoef;
uniform float step;
uniform sampler2D sampler;

void main()
{
    vec4 sampleColor[7];

    for (int i = 0; i < 4; i++)
    {
        sampleColor[i] = texture(sampler, texCoords.st - vec2(0.0f, i * step));
    }

    for (int i = 4; i < 7; i++)
    {
        sampleColor[i] = texture(sampler, texCoords.st + vec2(0.0f, (i - 3) * step));
    }

    vec4 wColor = vec4(gaussCoef.B * sampleColor[0].rgb +
                  (gaussCoef.bi.x * sampleColor[1].rgb +
                  gaussCoef.bi.y * sampleColor[2].rgb +
                  gaussCoef.bi.z * sampleColor[3].rgb) / gaussCoef.bi.w, 1.0f);

    gl_FragColor = vec4(gaussCoef.B * wColor.rgb +
                   (gaussCoef.bi.x * sampleColor[4].rgb +
                   gaussCoef.bi.y * sampleColor[5].rgb +
                   gaussCoef.bi.z * sampleColor[6].rgb) / gaussCoef.bi.w, 1.0f);
}