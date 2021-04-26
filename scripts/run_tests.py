import os
import shutil
import subprocess
import sys


def clean_up(folder):
    shutil.rmtree(tmp_folder_path)


def print_result(folder, file, output, pair_error_reason=None):
    result = folder + ": " + output + "\n"
    print(result)
    f.write(result)
    if pair_error_reason:
        f.write(pair_error_reason[1] + ": \n" + pair_error_reason[0] + "\n")
    f.write("\n")


root = sys.path[0] + "/"
tests_folder = root + "../mito/tests/"
if not os.path.isdir(tests_folder):
    raise NameError("Please check that tests folder exists")

# List of all folders
folders = [x for x in os.listdir(
    tests_folder) if os.path.isdir(tests_folder + x)]

output_file = root + 'out.log'

with open(output_file, 'w') as f:
    for folder in folders:
        folder_path = tests_folder + folder + "/"
        os.chdir(folder_path)
        test_name = "main"
        test_path = folder_path + test_name
        test_ext = ".cc"
        if not os.path.isfile(test_path + test_ext):
            raise NameError("Please name the test as main.cc")

        # Temporary folder for compile and run the test
        tmp_folder_path = folder_path + "build_temp/"
        if os.path.isdir(tmp_folder_path):
            clean_up(tmp_folder_path)
        os.mkdir(tmp_folder_path)

        # Commands to execute
        compile_cmd = 'g++ -g -std=c++2a -Wall -Wextra -pedantic -Werror ' + \
            test_path + test_ext + ' -o ' + tmp_folder_path + \
            test_name
        run_cmd = tmp_folder_path + test_name

        compile_process = subprocess.run(
            compile_cmd.split(' '), capture_output=True, text=True)
        if compile_process.stderr:
            print_result(folder, f, "FAIL",
                         (compile_process.stderr, "Compilation error"))
            clean_up(tmp_folder_path)
            continue

        run_process = subprocess.run(
            run_cmd.split(' '), capture_output=True, text=True)
        if run_process.stderr:
            print_result(folder, f, "FAIL",
                         (run_process.stderr, "Runtime error"))
            clean_up(tmp_folder_path)
            continue

        print_result(folder, f, "SUCCESS")
        clean_up(tmp_folder_path)

print("all done!")
