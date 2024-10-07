const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const static_lib = b.addStaticLibrary(.{
        .name = "hiv",
        .target = target,
        .optimize = optimize,
    });

    static_lib.linkLibC();
    static_lib.addIncludePath(b.path("include"));
    static_lib.addLibraryPath(b.path("lib"));

    // const shared_lib = b.addSharedLibrary(.{
    //     .name = "hiv",
    //     .target = target,
    //     .optimize = optimize,
    // });

    // shared_lib.linkLibC();
    // shared_lib.addIncludePath(b.path("include"));
    // shared_lib.addLibraryPath(b.path("lib"));

    const source_files = &[_][]const u8{
        "src/ArrayList.c",
        //"src/EventLoop.c",
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

    static_lib.addCSourceFiles(.{
        .files = source_files,
        .flags = flags,
    });

    // shared_lib.addCSourceFiles(.{
    //     .files = source_files,
    //     .flags = flags,
    // });

    b.installArtifact(static_lib);
    // b.installArtifact(shared_lib);

    const test_files = &[_][]const u8{
        "tests/test_ArrayList.c",
        //"tests/test_EventLoop.c",
    };

    const clang_tidy_cmd = b.addSystemCommand(.{"clang-tidy"} ++ source_files ++ test_files);
    const tidy_step = b.step("tidy", "Run clang-tidy");
    tidy_step.dependOn(&clang_tidy_cmd.step);

    const test_step = b.step("test", "Run tests");
    inline for (test_files, 0..) |test_file, i| {
        const test_exe = b.addExecutable(.{
            .name = "test_hiv" ++ [_]u8{ '_', i + 48 },
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
        b.installArtifact(test_exe);
    }
}
