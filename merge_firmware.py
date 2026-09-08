Import("env")
import os


def merge_bin(source, target, env):
    build_dir = env.subst("$BUILD_DIR")
    project_dir = env.subst("$PROJECT_DIR")
    pioenv = env["PIOENV"]
    python_exe = env.subst("$PYTHONEXE")

    bootloader = os.path.join(build_dir, "bootloader.bin")
    partitions = os.path.join(build_dir, "partitions.bin")
    firmware = os.path.join(build_dir, "firmware.bin")
    spiffs = os.path.join(build_dir, "spiffs.bin")
    output = os.path.join(project_dir, "merged-flash.bin")

    # Always rebuild the filesystem image (from data/) before merging,
    # to make sure spiffs.bin always matches the latest contents of data/
    print(">>> Rebuilding filesystem image (buildfs)...")
    env.Execute(
        '"%s" -m platformio run --target buildfs -e %s' % (python_exe, pioenv)
    )

    if not os.path.exists(spiffs):
        print(">>> ERROR: spiffs.bin not found after buildfs. Check that the data/ folder exists.")
        return

    esptool_py = os.path.join(
        env.PioPlatform().get_package_dir("tool-esptoolpy"), "esptool.py"
    )

    cmd = [
        python_exe,
        esptool_py,
        "--chip", "esp32",
        "merge_bin",
        "-o", output,
        "--flash_mode", "dio",
        "--flash_freq", "40m",
        "--flash_size", "4MB",
        "0x1000", bootloader,
        "0x8000", partitions,
        "0x10000", firmware,
        "0x290000", spiffs,
    ]

    quoted_cmd = " ".join('"%s"' % c if " " in c else c for c in cmd)
    print(">>> Merging binaries into merged-flash.bin...")
    env.Execute(quoted_cmd)
    print(">>> DONE! merged-flash.bin is ready at: " + output)


env.AddPostAction("buildprog", merge_bin)