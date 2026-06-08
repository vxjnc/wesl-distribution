#include <assert.h>
#include <stdio.h>
#include <wesl.h>

int main(void) {
    printf("wesl version: %s\n", wesl_version());

    const char* math_source = "fn square(x: f32) -> f32 {\n"
                              "    return x * x;\n"
                              "}\n";

    const char* main_source = "import package::math::square;\n"
                              "\n"
                              "@vertex\n"
                              "fn vs_main() -> @builtin(position) vec4f {\n"
                              "    let scale = square(0.5);\n"
                              "    return vec4f(scale, 0.0, 0.0, 1.0);\n"
                              "}\n";

    const char* modules[] = {"package::math", "package::main"};
    const char* sources[] = {math_source, main_source};

    static_assert(sizeof(modules) == sizeof(sources), "modules and sources must be the same length");
    WeslStringMap files = {
        .keys = modules,
        .values = sources,
        .len = sizeof(modules) / sizeof(modules[0]),
    };

    WeslCompileOptions opts = {
        .mangler = WESL_MANGLER_NONE,
        .imports = true,
        .condcomp = true,
    };

    WeslStringArray keep = {.items = NULL, .len = 0};
    WeslBoolMap features = {.keys = NULL, .values = NULL, .len = 0};

    WeslResult result = wesl_compile(&files, "package::main", &opts, &keep, &features);

    if (result.success) {
        printf("compile OK:\n%s\n", result.data);
    }
    else {
        printf("compile FAILED: %s\n", result.error.message);
    }

    wesl_free_result(&result);
    return result.success ? 0 : 1;
}
