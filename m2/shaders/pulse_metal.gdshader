shader_type spatial;

uniform float pulse_strength : hint_range(0.0, 0.5) = 0.18;
uniform float pulse_speed : hint_range(0.1, 6.0) = 2.0;
uniform vec4 base_color : source_color = vec4(0.95, 0.24, 0.28, 1.0);

void vertex() {
	float pulse = sin(TIME * pulse_speed + VERTEX.y * 3.0) * pulse_strength;
	VERTEX += NORMAL * pulse;
}

void fragment() {
	float shine = sin(TIME * pulse_speed) * 0.5 + 0.5;
	ALBEDO = base_color.rgb;
	METALLIC = mix(0.15, 0.85, shine);
	ROUGHNESS = mix(0.18, 0.55, 1.0 - shine);
}
