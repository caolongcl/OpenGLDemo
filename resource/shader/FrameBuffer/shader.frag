#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300;  

void main()
{
	//原来的颜色
	vec4 texColor = texture(screenTexture, TexCoords);
	//反转颜色
	vec4 inverseColor = vec4(1.0f - vec3(texColor), 1.0f);
	//灰度
	vec4 grayColor = vec4(vec3((texColor.r + texColor.g + texColor.b)/3.0f), 1.0f);
	//符合人眼实际的灰度
	float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
	vec4 grayColor1 = vec4(vec3(average), 1.0f);

	//kernel effects
	vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right
    );

	//锐化kernel
    float kernel0[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

	//模糊kernel
	float kernel1[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
	);

	//边检测kernel
	float kernel2[9] = float[](
		1, 1, 1,
        1,-9, 1,
        1, 1, 1
	);

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col;
    for(int i = 0; i < 9; i++)
	{
        col += sampleTex[i] * kernel2[i] * 1;
	}

    //color = vec4(col, 1.0);
	
	//color = texColor;
	//color = inverseColor;
	//color = grayColor;
	//color = grayColor1;
}
