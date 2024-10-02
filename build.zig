const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addStaticLibrary(.{
        .name = "hiv",
        .target = target,
        .optimize = optimize,
    });

    exe.linkLibC();

    exe.addIncludePath(b.path("include"));
    exe.addLibraryPath(b.path("lib"));

    const source_files = &[_][]const u8{
        "src/ArrayList.c",
    };

    const test_files = &[_][]const u8{
        "tests/test_ArrayList.c",
    };

    const flags = &[_][]const u8{
        "-std=c11",
        "-pedantic",
        "-Wall",
        "-W",
        "-Wno-missing-field-initializers",
        "-fno-sanitize=undefined",
        "-Wunused-result",
    };

    exe.addCSourceFiles(.{
        .files = source_files ++ test_files,
        .flags = flags,
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Run");
    run_step.dependOn(&run_cmd.step);

    const clang_tidy_cmd = b.addSystemCommand(.{"clang-tidy"} ++ source_files ++ test_files);
    const tidy_step = b.step("tidy", "Run clang-tidy");
    tidy_step.dependOn(&clang_tidy_cmd.step);
}
