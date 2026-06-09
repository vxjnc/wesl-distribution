import package::math::square;

@vertex
fn vs_main() -> @builtin(position) vec4f {
    let scale = square(0.5);
    return vec4f(scale, 0.0, 0.0, 1.0);
}