#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform vec3 u_camera_pos;

uniform vec3 u_tint_color = vec3(0.8, 0.9, 1.0);

void main() {
    float view_dot = dot(normalize(u_camera_pos - FragPos), normalize(Normal));
    float fresnel = pow(1.0 - view_dot, 3.0);
    float alpha = 0.6 + fresnel * 0.3;

    FragColor = vec4(u_tint_color, clamp(alpha, 0.0, 1.0));
}
