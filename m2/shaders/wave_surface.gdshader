shader_type spatial;

// Mostra os dois lados do plano, evitando que ele suma quando visto por tras.
render_mode cull_disabled;

uniform float amplitude : hint_range(0.0, 1.0) = 0.35;
uniform float frequency : hint_range(0.5, 10.0) = 4.0;
uniform float speed : hint_range(0.0, 5.0) = 1.4;
uniform vec4 deep_color : source_color = vec4(0.05, 0.24, 0.55, 1.0);
uniform vec4 crest_color : source_color = vec4(0.35, 0.95, 1.0, 1.0);

// Leva a altura calculada no vertex para colorir no fragment.
varying float wave_amount;

void vertex() {
	float phase = VERTEX.x * frequency + TIME * speed;
	float wave = sin(phase) * amplitude;
	VERTEX.y += wave;

	float slope = cos(phase) * amplitude * frequency;
	NORMAL = normalize(vec3(-slope, 1.0, 0.0));
	wave_amount = wave / max(amplitude, 0.001) * 0.5 + 0.5;
}

void fragment() {
	vec3 color = mix(deep_color.rgb, crest_color.rgb, wave_amount);
	ALBEDO = color;
	ROUGHNESS = mix(0.25, 0.75, wave_amount);
}
