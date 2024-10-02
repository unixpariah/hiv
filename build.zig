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
        "src/EventLoop.c",
    };

    const test_files = &[_][]const u8{
        "tests/test_ArrayList.c",
        //"tests/test_EventLoop.c",
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
        .files = source_files,
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

    var valgrind_cmd = b.addSystemCommand(&.{"valgrind"});
    if (b.args) |args| {
        for (args) |arg| valgrind_cmd.addArg(arg);
    }
    valgrind_cmd.addArg("./zig-out/bin/eeyelop");
    const valgrind_step = b.step("valgrind", "Run valgrind");
    valgrind_step.dependOn(&valgrind_cmd.step);
    valgrind_step.dependOn(&run_cmd.step);

    const test_step = b.step("test", "Run tests");
    inline for (test_files) |test_file| {
        const test_exe = b.addExecutable(.{
            .name = "test_hiv",
            .target = target,
            .optimize = optimize,
        });

        test_exe.linkLibC();

        test_exe.addIncludePath(b.path("include"));
        test_exe.addLibraryPath(b.path("lib"));

        test_exe.addCSourceFiles(.{
            .files = source_files ++ &[_][]const u8{test_file},
            .flags = flags,
        });

        const test_cmd = b.addRunArtifact(test_exe);
        test_step.dependOn(&test_cmd.step);
    }
}
