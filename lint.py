#!/usr/bin/env python3

import argparse
import difflib
import shutil
import subprocess
import sys
from pathlib import Path


def file_format(file: Path, cmd: list[str]):
    with open(file, encoding="utf8") as f:
        original = f.read()
    r = subprocess.run(cmd, check=False, text=True, input=original, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if r.returncode != 0:
        print(f"{r.stdout}\n")
        return False

    patch = list(difflib.unified_diff(original.splitlines(), r.stdout.splitlines()))[2:]
    if not patch:
        return True

    for line in patch:
        if line[0] == "+":
            print(f"\033[32m{line}\033[0m")
        elif line[0] == "-":
            print(f"\033[31m{line}\033[0m")
        else:
            print(line)
    print()
    return False


def file_lint(cmd: list[str]):
    r = subprocess.run(
        cmd,
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    if r.returncode == 0:
        return True

    print(f"{r.stdout}\n")
    return False


def first_executable(names: list[str]):
    for name in names[:-1]:
        if shutil.which(name):
            return name
    return names[-1]


def llvm_executable(name: str):
    return first_executable([f"{name}-19", name])


def qt_executable(name: str):
    return first_executable([f"/usr/lib/qt6/bin/{name}", name])


def main():
    root = Path(__file__).parent

    parser = argparse.ArgumentParser()
    parser.add_argument("files", nargs="*", type=Path, help="Files to lint")
    args = parser.parse_args()

    fails = 0

    for file in args.files:
        if file.suffix in [".cpp", ".hpp"]:
            fails += not file_format(file, [llvm_executable("clang-format"), "-assume-filename", f"{file}"])
            if file.suffix == ".cpp":
                fails += not file_lint(
                    [
                        llvm_executable("clang-tidy"),
                        "--quiet",
                        "--use-color",
                        "--warnings-as-errors=*",
                        "-p=.",
                        "--extra-arg=-Wno-unused-command-line-argument",
                        f"{file}",
                    ]
                )
        elif file.suffix == ".py":
            fails += not file_format(file, ["black", "--quiet", "-"])
            fails += not file_lint(["pylint", "--score=n", f"--rcfile={root}/pyproject.toml", f"{file}"])
        elif file.suffix == ".qml":
            fails += not file_format(file, [qt_executable("qmlformat"), f"{file}"])
            fails += not file_lint([qt_executable("qmllint"), "-Iqml", f"{file}"])

    return fails


if __name__ == "__main__":
    sys.exit(main())
