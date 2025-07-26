#version 330 core
in vec2 vUV;
in float vAlpha;
flat in float vFrame;

out vec4 FragColor;

uniform sampler2D u_particle_texture;
uniform int u_columns = 4;
uniform int u_rows = 4;

void main() {
    int totalFrames = u_columns * u_rows;
    int frameIndex = int(mod(vFrame, float(totalFrames)));

    int col = frameIndex % u_columns;
    int row = frameIndex / u_columns;

    vec2 uv = vUV;
    uv.x = uv.x / float(u_columns) + float(col) / float(u_columns);
    uv.y = uv.y / float(u_rows) + float(row) / float(u_rows);

    vec4 texColor = texture(u_particle_texture, uv);
    FragColor = vec4(texColor.rgb, texColor.a * vAlpha);

    if (FragColor.a < 0.05) discard;
}
