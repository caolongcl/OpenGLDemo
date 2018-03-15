#version 450 core

in vec2 texCoord;

struct GaussCoef
{
   vec4 bi;
   float B;
}gaussCoef;

uniform sampler2D sampler;

void main()
{
    //gaussCoef = GaussCoef(vec4(1.019557357f, -0.161413923f, 0.011274796f, 2.447669268f), 0.644797504f);
    //gaussCoef = GaussCoef(vec4(8.560305595f, -3.842197418f, 0.647602916f, 6.943974495f), 0.227285266f);
    //gaussCoef = GaussCoef(vec4(9441.028320312f, -8891.517578125f, 2795.978515625f, 3347.070312500f), 0.000472367f);
    gaussCoef = GaussCoef(vec4(0.320051372f, -0.020726161f, 0.000638284f, 1.878213644f), 0.840293229f);
    float step = 1.0 / 200;

    vec4 sampleColor[7];

    for (int i = 0; i < 4; i++)
    {
        sampleColor[i] = texture(sampler, texCoord.st - vec2(i * step, 0.0));
    }

    for (int i = 4; i < 7; i++)
    {
        sampleColor[i] = texture(sampler, texCoord.st + vec2((i - 3) * step, 0.0));
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