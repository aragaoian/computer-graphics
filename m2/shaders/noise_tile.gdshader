// Shader espacial usado em um objeto 3D.
shader_type spatial;

// Uniforms: textura de ruido e parametros editaveis pelo material.
uniform sampler2D noise_texture : repeat_enable;
uniform vec4 tint_a : source_color = vec4(0.08, 0.62, 0.34, 1.0);
uniform vec4 tint_b : source_color = vec4(0.95, 0.87, 0.43, 1.0);
uniform float noise_scale : hint_range(0.5, 8.0) = 3.0;
uniform float time_speed : hint_range(0.0, 1.0) = 0.08;

// Fragment: le a textura de ruido e usa o valor para misturar cores.
void fragment() {
	vec2 uv = UV * noise_scale + vec2(TIME * time_speed, TIME * time_speed * 0.4);
	float noise_value = texture(noise_texture, uv).r;
	float bands = smoothstep(0.28, 0.72, noise_value);

	ALBEDO = mix(tint_a.rgb, tint_b.rgb, bands);
	ROUGHNESS = mix(0.35, 0.9, noise_value);
}
